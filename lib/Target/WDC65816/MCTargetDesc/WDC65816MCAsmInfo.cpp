//===-- WDC65816MCAsmInfo.cpp - WDC65816 asm properties -----------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the declarations of the WDC65816MCAsmInfo properties.
//
//===----------------------------------------------------------------------===//

#include "WDC65816MCAsmInfo.h"
using namespace llvm;

void WDC65816MCAsmInfo::anchor() { }

WDC65816MCAsmInfo::WDC65816MCAsmInfo(const Triple &TT) {
  CodePointerSize = CalleeSaveStackSlotSize = 2;

  CommentString = ";";

  AlignmentIsInBytes = false;
  UsesELFSectionDirectiveForBSS = true;
}
