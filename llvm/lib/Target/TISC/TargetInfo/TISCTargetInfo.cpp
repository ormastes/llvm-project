//===-- TISCTargetInfo.cpp - TISC Target Implementation ---------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "TargetInfo/TISCTargetInfo.h"
#include "llvm/MC/TargetRegistry.h"
using namespace llvm;

Target &llvm::getTheTISCTarget() {
  static Target TheTISCTarget;
  return TheTISCTarget;
}

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeTISCTargetInfo() {
  RegisterTarget<Triple::tisc> X(getTheTISCTarget(), "tisc",
                                   "TISC [experimental]", "TISC");
}
