//===-- WDC65816TargetInfo.cpp - WDC65816 Target Implementation ---------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "WDC65816.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/TargetRegistry.h"
using namespace llvm;

Target &llvm::getTheWDC65816Target() {
  static Target TheWDC65816Target;
  return TheWDC65816Target;
}

extern "C" void LLVMInitializeWDC65816TargetInfo() {
  RegisterTarget<Triple::wdc65816> X(getTheWDC65816Target(), "WDC65816",
                                   "WDC65816 [experimental]", "WDC65816");
}
