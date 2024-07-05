//===-- TISCAttributes.h - TISC Attributes ------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===-----------------------------------------------------------------------===//
///
/// \file
/// This file contains enumerations for TISC ELF build attributes as
/// defined in the TISC ELF psABI specification.
///
/// TISC ELF psABI specification
///
/// https://www.ti.com/lit/pdf/slaa534
///
//===----------------------------------------------------------------------===//
#ifndef LLVM_SUPPORT_TISCATTRIBUTES_H
#define LLVM_SUPPORT_TISCTTRIBUTES_H

#include "llvm/Support/ELFAttributes.h"

namespace llvm {
namespace TISCAttrs {

const TagNameMap &getTISCAttributeTags();

enum AttrType : unsigned {
  // Attribute types in ELF/.TISC.attributes.
  TagISA = 4,
  TagCodeModel = 6,
  TagDataModel = 8,
  TagEnumSize = 10
};

enum ISA { ISATISC = 1, ISATISCX = 2 };
enum CodeModel { CMSmall = 1, CMLarge = 2 };
enum DataModel { DMSmall = 1, DMLarge = 2, DMRestricted = 3 };
enum EnumSize { ESSmall = 1, ESInteger = 2, ESDontCare = 3 };

} // namespace TISCAttrs
} // namespace llvm

#endif
