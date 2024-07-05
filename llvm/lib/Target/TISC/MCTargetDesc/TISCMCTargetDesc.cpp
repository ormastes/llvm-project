//===-- TISCMCTargetDesc.cpp - TISC Target Descriptions ---------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file provides TISC specific target descriptions.
//
//===----------------------------------------------------------------------===//

#include "TISCMCTargetDesc.h"
#include "TISCInstPrinter.h"
#include "TISCMCAsmInfo.h"
#include "TargetInfo/TISCTargetInfo.h"
#include "llvm/MC/MCDwarf.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/TargetRegistry.h"

using namespace llvm;

#define GET_INSTRINFO_MC_DESC
#define ENABLE_INSTR_PREDICATE_VERIFIER
#include "TISCGenInstrInfo.inc"

#define GET_SUBTARGETINFO_MC_DESC
#include "TISCGenSubtargetInfo.inc"

#define GET_REGINFO_MC_DESC
#include "TISCGenRegisterInfo.inc"

static MCInstrInfo *createTISCMCInstrInfo() {
  MCInstrInfo *X = new MCInstrInfo();
  InitTISCMCInstrInfo(X);
  return X;
}

static MCRegisterInfo *createTISCMCRegisterInfo(const Triple &TT) {
  MCRegisterInfo *X = new MCRegisterInfo();
  InitTISCMCRegisterInfo(X, TISC::PC);
  return X;
}

static MCAsmInfo *createTISCMCAsmInfo(const MCRegisterInfo &MRI,
                                        const Triple &TT,
                                        const MCTargetOptions &Options) {
  MCAsmInfo *MAI = new TISCMCAsmInfo(TT);

  // Initialize initial frame state.
  int stackGrowth = -2;

  // Initial state of the frame pointer is sp+ptr_size.
  MCCFIInstruction Inst = MCCFIInstruction::cfiDefCfa(
      nullptr, MRI.getDwarfRegNum(TISC::SP, true), -stackGrowth);
  MAI->addInitialFrameState(Inst);

  // Add return address to move list
  MCCFIInstruction Inst2 = MCCFIInstruction::createOffset(
      nullptr, MRI.getDwarfRegNum(TISC::PC, true), stackGrowth);
  MAI->addInitialFrameState(Inst2);

  return MAI;
}

static MCSubtargetInfo *
createTISCMCSubtargetInfo(const Triple &TT, StringRef CPU, StringRef FS) {
  return createTISCMCSubtargetInfoImpl(TT, CPU, /*TuneCPU*/ CPU, FS);
}

static MCInstPrinter *createTISCMCInstPrinter(const Triple &T,
                                                unsigned SyntaxVariant,
                                                const MCAsmInfo &MAI,
                                                const MCInstrInfo &MII,
                                                const MCRegisterInfo &MRI) {
  if (SyntaxVariant == 0)
    return new TISCInstPrinter(MAI, MII, MRI);
  return nullptr;
}

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeTISCTargetMC() {
  Target &T = getTheTISCTarget();

  TargetRegistry::RegisterMCAsmInfo(T, createTISCMCAsmInfo);
  TargetRegistry::RegisterMCInstrInfo(T, createTISCMCInstrInfo);
  TargetRegistry::RegisterMCRegInfo(T, createTISCMCRegisterInfo);
  TargetRegistry::RegisterMCSubtargetInfo(T, createTISCMCSubtargetInfo);
  TargetRegistry::RegisterMCInstPrinter(T, createTISCMCInstPrinter);
  TargetRegistry::RegisterMCCodeEmitter(T, createTISCMCCodeEmitter);
  TargetRegistry::RegisterMCAsmBackend(T, createTISCMCAsmBackend);
  TargetRegistry::RegisterObjectTargetStreamer(
      T, createTISCObjectTargetStreamer);
}
