//===-- TISCELFObjectWriter.cpp - TISC ELF Writer ---------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "MCTargetDesc/TISCFixupKinds.h"
#include "MCTargetDesc/TISCMCTargetDesc.h"

#include "llvm/MC/MCELFObjectWriter.h"
#include "llvm/MC/MCFixup.h"
#include "llvm/MC/MCObjectWriter.h"
#include "llvm/MC/MCValue.h"
#include "llvm/Support/ErrorHandling.h"

using namespace llvm;

namespace {
class TISCELFObjectWriter : public MCELFObjectTargetWriter {
public:
  TISCELFObjectWriter(uint8_t OSABI)
    : MCELFObjectTargetWriter(false, OSABI, ELF::EM_TISC,
                              /*HasRelocationAddend*/ true) {}

  ~TISCELFObjectWriter() override = default;

protected:
  unsigned getRelocType(MCContext &Ctx, const MCValue &Target,
                        const MCFixup &Fixup, bool IsPCRel) const override {
    // Translate fixup kind to ELF relocation type.
    switch (Fixup.getTargetKind()) {
    case FK_Data_1:                   return ELF::R_TISC_8;
    case FK_Data_2:                   return ELF::R_TISC_16_BYTE;
    case FK_Data_4:                   return ELF::R_TISC_32;
    case TISC::fixup_32:            return ELF::R_TISC_32;
    case TISC::fixup_10_pcrel:      return ELF::R_TISC_10_PCREL;
    case TISC::fixup_16:            return ELF::R_TISC_16;
    case TISC::fixup_16_pcrel:      return ELF::R_TISC_16_PCREL;
    case TISC::fixup_16_byte:       return ELF::R_TISC_16_BYTE;
    case TISC::fixup_16_pcrel_byte: return ELF::R_TISC_16_PCREL_BYTE;
    case TISC::fixup_2x_pcrel:      return ELF::R_TISC_2X_PCREL;
    case TISC::fixup_rl_pcrel:      return ELF::R_TISC_RL_PCREL;
    case TISC::fixup_8:             return ELF::R_TISC_8;
    case TISC::fixup_sym_diff:      return ELF::R_TISC_SYM_DIFF;
    default:
      llvm_unreachable("Invalid fixup kind");
    }
  }
};
} // end of anonymous namespace

std::unique_ptr<MCObjectTargetWriter>
llvm::createTISCELFObjectWriter(uint8_t OSABI) {
  return std::make_unique<TISCELFObjectWriter>(OSABI);
}
