//==-- WDC65816.h - Top-level interface for WDC65816 representation --*- C++ -*-==//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the entry points for global functions defined in
// the LLVM WDC65816 backend.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_WDC65816_WDC65816_H
#define LLVM_LIB_TARGET_WDC65816_WDC65816_H

#include "MCTargetDesc/WDC65816MCTargetDesc.h"
#include "llvm/Target/TargetMachine.h"

namespace WDC65816CC {
  // WDC65816 specific condition code.
  enum CondCodes {
    COND_E  = 0,  // aka COND_Z
    COND_NE = 1,  // aka COND_NZ
    COND_HS = 2,  // aka COND_C
    COND_LO = 3,  // aka COND_NC
    COND_GE = 4,
    COND_L  = 5,
    COND_N  = 6,  // jump if negative
    COND_NONE,    // unconditional

    COND_INVALID = -1
  };
}

namespace llvm {
  class WDC65816TargetMachine;
  class FunctionPass;
  class formatted_raw_ostream;

  FunctionPass *createWDC65816ISelDag(WDC65816TargetMachine &TM,
                                    CodeGenOpt::Level OptLevel);

  FunctionPass *createWDC65816BranchSelectionPass();

} // end namespace llvm;

#endif
