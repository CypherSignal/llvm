//===-- WDC65816FixupKinds.h - WDC65816 Specific Fixup Entries ------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_WDC65816_MCTARGETDESC_WDC65816FIXUPKINDS_H
#define LLVM_LIB_TARGET_WDC65816_MCTARGETDESC_WDC65816FIXUPKINDS_H

#include "llvm/MC/MCFixup.h"

#undef WDC65816

namespace llvm {
namespace WDC65816 {

// This table must be in the same order of
// MCFixupKindInfo Infos[WDC65816::NumTargetFixupKinds]
// in WDC65816AsmBackend.cpp.
//
enum Fixups {
  // A 32 bit absolute fixup.
  fixup_32 = FirstTargetFixupKind,
  // A 10 bit PC relative fixup.
  fixup_10_pcrel,
  // A 16 bit absolute fixup.
  fixup_16,
  // A 16 bit PC relative fixup.
  fixup_16_pcrel,
  // A 16 bit absolute fixup for byte operations.
  fixup_16_byte,
  // A 16 bit PC relative fixup for command address.
  fixup_16_pcrel_byte,
  // A 10 bit PC relative fixup for complicated polymorphs.
  fixup_2x_pcrel,
  // A 16 bit relaxable fixup.
  fixup_rl_pcrel,
  // A 8 bit absolute fixup.
  fixup_8,
  // A 32 bit symbol difference fixup.
  fixup_sym_diff,

  // Marker
  LastTargetFixupKind,
  NumTargetFixupKinds = LastTargetFixupKind - FirstTargetFixupKind
};
} // end namespace WDC65816
} // end namespace llvm

#endif
