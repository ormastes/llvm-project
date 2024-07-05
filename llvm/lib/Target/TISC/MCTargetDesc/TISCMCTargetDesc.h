//===-- TISCMCTargetDesc.h - TISC Target Descriptions -------*- C++ -*-===//
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

#ifndef LLVM_LIB_TARGET_TISC_MCTARGETDESC_TISCMCTARGETDESC_H
#define LLVM_LIB_TARGET_TISC_MCTARGETDESC_TISCMCTARGETDESC_H

#include "llvm/Support/DataTypes.h"
#include <memory>

namespace llvm {
class Target;
class MCAsmBackend;
class MCCodeEmitter;
class MCInstrInfo;
class MCSubtargetInfo;
class MCRegisterInfo;
class MCContext;
class MCTargetOptions;
class MCObjectTargetWriter;
class MCStreamer;
class MCTargetStreamer;

/// Creates a machine code emitter for TISC.
MCCodeEmitter *createTISCMCCodeEmitter(const MCInstrInfo &MCII,
                                         MCContext &Ctx);

MCAsmBackend *createTISCMCAsmBackend(const Target &T,
                                       const MCSubtargetInfo &STI,
                                       const MCRegisterInfo &MRI,
                                       const MCTargetOptions &Options);

MCTargetStreamer *
createTISCObjectTargetStreamer(MCStreamer &S, const MCSubtargetInfo &STI);

std::unique_ptr<MCObjectTargetWriter>
createTISCELFObjectWriter(uint8_t OSABI);

} // End llvm namespace

// Defines symbolic names for TISC registers.
// This defines a mapping from register name to register number.
#define GET_REGINFO_ENUM
#include "TISCGenRegisterInfo.inc"

// Defines symbolic names for the TISC instructions.
#define GET_INSTRINFO_ENUM
#define GET_INSTRINFO_MC_HELPER_DECLS
#include "TISCGenInstrInfo.inc"

#define GET_SUBTARGETINFO_ENUM
#include "TISCGenSubtargetInfo.inc"

#endif
