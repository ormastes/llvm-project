//===-- TISCMCAsmInfo.cpp - TISC asm properties -----------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains the declarations of the TISCMCAsmInfo properties.
//
//===----------------------------------------------------------------------===//

#include "TISCMCAsmInfo.h"
using namespace llvm;

void TISCMCAsmInfo::anchor() { }

TISCMCAsmInfo::TISCMCAsmInfo(const Triple &TT) {
  // Since TISC-GCC already generates 32-bit DWARF information, we will
  // also store 16-bit pointers as 32-bit pointers in DWARF, because using
  // 32-bit DWARF pointers is already a working and tested path for LLDB
  // as well.
  CodePointerSize = 4;
  CalleeSaveStackSlotSize = 2;

  CommentString = ";";
  SeparatorString = "{";

  AlignmentIsInBytes = false;
  UsesELFSectionDirectiveForBSS = true;

  SupportsDebugInformation = true;

  ExceptionsType = ExceptionHandling::DwarfCFI;
}
