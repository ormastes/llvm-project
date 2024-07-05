//===-- TISCAttributeParser.h - TISC Attribute Parser -------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
///
/// \file
/// This file contains support routines for parsing TISC ELF build attributes.
///
//===----------------------------------------------------------------------===//

#ifndef LLVM_SUPPORT_TISCATTRIBUTEPARSER_H
#define LLVM_SUPPORT_TISCATTRIBUTEPARSER_H

#include "llvm/Support/ELFAttributeParser.h"
#include "llvm/Support/TISCAttributes.h"

namespace llvm {
class TISCAttributeParser : public ELFAttributeParser {
  struct DisplayHandler {
    TISCAttrs::AttrType Attribute;
    Error (TISCAttributeParser::*Routine)(TISCAttrs::AttrType);
  };
  static const std::array<DisplayHandler, 4> DisplayRoutines;

  Error parseISA(TISCAttrs::AttrType Tag);
  Error parseCodeModel(TISCAttrs::AttrType Tag);
  Error parseDataModel(TISCAttrs::AttrType Tag);
  Error parseEnumSize(TISCAttrs::AttrType Tag);

  Error handler(uint64_t Tag, bool &Handled) override;

public:
  TISCAttributeParser(ScopedPrinter *SW)
      : ELFAttributeParser(SW, TISCAttrs::getTISCAttributeTags(),
                           "tiscabi") {}
  TISCAttributeParser()
      : ELFAttributeParser(TISCAttrs::getTISCAttributeTags(), "tiscabi") {}
};
} // namespace llvm

#endif
