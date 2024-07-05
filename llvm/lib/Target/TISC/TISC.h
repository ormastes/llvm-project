//==-- TISC.h - Top-level interface for TISC representation --*- C++ -*-==//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains the entry points for global functions defined in
// the LLVM TISC backend.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_TISC_TISC_H
#define LLVM_LIB_TARGET_TISC_TISC_H

#include "MCTargetDesc/TISCMCTargetDesc.h"
#include "llvm/Target/TargetMachine.h"

namespace TISCCC {
  // TISC specific condition code.
  enum CondCodes {
    COND_E  = 0,  // aka COND_Z
    COND_NE = 1,  // aka COND_NZ
    COND_HS = 2,  // aka COND_C
    COND_LO = 3,  // aka COND_NC
    COND_GE = 4,
    COND_L  = 5,
    COND_N  = 6,  // jump if negative
    COND_NONE,    // unconditional

    COND_INVALID = -1
  };
}

namespace llvm {
class FunctionPass;
class TISCTargetMachine;
class PassRegistry;

FunctionPass *createTISCISelDag(TISCTargetMachine &TM,
                                  CodeGenOptLevel OptLevel);

FunctionPass *createTISCBranchSelectionPass();

void initializeTISCDAGToDAGISelPass(PassRegistry &);

} // namespace llvm

#endif
