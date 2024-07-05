//===-- TISCSubtarget.cpp - TISC Subtarget Information ----------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements the TISC specific subclass of TargetSubtargetInfo.
//
//===----------------------------------------------------------------------===//

#include "TISCSubtarget.h"
#include "TISC.h"
#include "llvm/MC/TargetRegistry.h"

using namespace llvm;

#define DEBUG_TYPE "tisc-subtarget"

static cl::opt<TISCSubtarget::HWMultEnum>
HWMultModeOption("mhwmult", cl::Hidden,
           cl::desc("Hardware multiplier use mode for TISC"),
           cl::init(TISCSubtarget::NoHWMult),
           cl::values(
             clEnumValN(TISCSubtarget::NoHWMult, "none",
                "Do not use hardware multiplier"),
             clEnumValN(TISCSubtarget::HWMult16, "16bit",
                "Use 16-bit hardware multiplier"),
             clEnumValN(TISCSubtarget::HWMult32, "32bit",
                "Use 32-bit hardware multiplier"),
             clEnumValN(TISCSubtarget::HWMultF5, "f5series",
                "Use F5 series hardware multiplier")));

#define GET_SUBTARGETINFO_TARGET_DESC
#define GET_SUBTARGETINFO_CTOR
#include "TISCGenSubtargetInfo.inc"

void TISCSubtarget::anchor() { }

TISCSubtarget &
TISCSubtarget::initializeSubtargetDependencies(StringRef CPU, StringRef FS) {
  ExtendedInsts = false;
  HWMultMode = NoHWMult;

  StringRef CPUName = CPU;
  if (CPUName.empty())
    CPUName = "tisc";

  ParseSubtargetFeatures(CPUName, /*TuneCPU*/ CPUName, FS);

  if (HWMultModeOption != NoHWMult)
    HWMultMode = HWMultModeOption;

  return *this;
}

TISCSubtarget::TISCSubtarget(const Triple &TT, const std::string &CPU,
                                 const std::string &FS, const TargetMachine &TM)
    : TISCGenSubtargetInfo(TT, CPU, /*TuneCPU*/ CPU, FS),
      InstrInfo(initializeSubtargetDependencies(CPU, FS)), TLInfo(TM, *this),
      FrameLowering(*this) {}
