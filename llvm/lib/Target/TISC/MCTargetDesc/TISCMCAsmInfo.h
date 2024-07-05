//===-- TISCMCAsmInfo.h - TISC asm properties --------------*- C++ -*--===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains the declaration of the TISCMCAsmInfo class.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_TISC_MCTARGETDESC_TISCMCASMINFO_H
#define LLVM_LIB_TARGET_TISC_MCTARGETDESC_TISCMCASMINFO_H

#include "llvm/MC/MCAsmInfoELF.h"

namespace llvm {
class Triple;

class TISCMCAsmInfo : public MCAsmInfoELF {
  void anchor() override;

public:
  explicit TISCMCAsmInfo(const Triple &TT);
};

} // namespace llvm

#endif
