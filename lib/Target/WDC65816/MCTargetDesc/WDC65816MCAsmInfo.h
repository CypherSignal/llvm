//===-- WDC65816MCAsmInfo.h - WDC65816 asm properties --------------*- C++ -*--===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the declaration of the WDC65816MCAsmInfo class.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_WDC65816_MCTARGETDESC_WDC65816MCASMINFO_H
#define LLVM_LIB_TARGET_WDC65816_MCTARGETDESC_WDC65816MCASMINFO_H

#include "llvm/MC/MCAsmInfoELF.h"

namespace llvm {
class Triple;

class WDC65816MCAsmInfo : public MCAsmInfoELF {
  void anchor() override;

public:
  explicit WDC65816MCAsmInfo(const Triple &TT);
};

} // namespace llvm

#endif
