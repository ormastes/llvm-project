//===-- TISCTargetMachine.cpp - Define TargetMachine for TISC ---------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// Top-level implementation for the TISC target.
//
//===----------------------------------------------------------------------===//

#include "TISCTargetMachine.h"
#include "TISC.h"
#include "TISCMachineFunctionInfo.h"
#include "TargetInfo/TISCTargetInfo.h"
#include "llvm/CodeGen/Passes.h"
#include "llvm/CodeGen/TargetLoweringObjectFileImpl.h"
#include "llvm/CodeGen/TargetPassConfig.h"
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/TargetRegistry.h"
#include <optional>
using namespace llvm;

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeTISCTarget() {
  // Register the target.
  RegisterTargetMachine<TISCTargetMachine> X(getTheTISCTarget());
  PassRegistry &PR = *PassRegistry::getPassRegistry();
  initializeTISCDAGToDAGISelPass(PR);
}

static Reloc::Model getEffectiveRelocModel(std::optional<Reloc::Model> RM) {
  return RM.value_or(Reloc::Static);
}

static std::string computeDataLayout(const Triple &TT, StringRef CPU,
                                     const TargetOptions &Options) {
  return "e-m:e-p:16:16-i32:16-i64:16-f32:16-f64:16-a:8-n8:16-S16";
}

TISCTargetMachine::TISCTargetMachine(const Target &T, const Triple &TT,
                                         StringRef CPU, StringRef FS,
                                         const TargetOptions &Options,
                                         std::optional<Reloc::Model> RM,
                                         std::optional<CodeModel::Model> CM,
                                         CodeGenOptLevel OL, bool JIT)
    : LLVMTargetMachine(T, computeDataLayout(TT, CPU, Options), TT, CPU, FS,
                        Options, getEffectiveRelocModel(RM),
                        getEffectiveCodeModel(CM, CodeModel::Small), OL),
      TLOF(std::make_unique<TargetLoweringObjectFileELF>()),
      Subtarget(TT, std::string(CPU), std::string(FS), *this) {
  initAsmInfo();
}

TISCTargetMachine::~TISCTargetMachine() = default;

namespace {
/// TISC Code Generator Pass Configuration Options.
class TISCPassConfig : public TargetPassConfig {
public:
  TISCPassConfig(TISCTargetMachine &TM, PassManagerBase &PM)
    : TargetPassConfig(TM, PM) {}

  TISCTargetMachine &getTISCTargetMachine() const {
    return getTM<TISCTargetMachine>();
  }

  void addIRPasses() override;
  bool addInstSelector() override;
  void addPreEmitPass() override;
};
} // namespace

TargetPassConfig *TISCTargetMachine::createPassConfig(PassManagerBase &PM) {
  return new TISCPassConfig(*this, PM);
}

MachineFunctionInfo *TISCTargetMachine::createMachineFunctionInfo(
    BumpPtrAllocator &Allocator, const Function &F,
    const TargetSubtargetInfo *STI) const {
  return TISCMachineFunctionInfo::create<TISCMachineFunctionInfo>(Allocator,
                                                                      F, STI);
}

void TISCPassConfig::addIRPasses() {
  addPass(createAtomicExpandPass());

  TargetPassConfig::addIRPasses();
}

bool TISCPassConfig::addInstSelector() {
  // Install an instruction selector.
  addPass(createTISCISelDag(getTISCTargetMachine(), getOptLevel()));
  return false;
}

void TISCPassConfig::addPreEmitPass() {
  // Must run branch selection immediately preceding the asm printer.
  addPass(createTISCBranchSelectionPass());
}
