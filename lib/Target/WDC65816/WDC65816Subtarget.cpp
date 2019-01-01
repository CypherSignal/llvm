//===-- WDC65816Subtarget.cpp - WDC65816 Subtarget Information ----------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements the WDC65816 specific subclass of TargetSubtargetInfo.
//
//===----------------------------------------------------------------------===//

#include "WDC65816Subtarget.h"
#include "WDC65816.h"
#include "llvm/Support/TargetRegistry.h"

using namespace llvm;

#define DEBUG_TYPE "WDC65816-subtarget"

#define GET_SUBTARGETINFO_TARGET_DESC
#define GET_SUBTARGETINFO_CTOR
#include "WDC65816GenSubtargetInfo.inc"

void WDC65816Subtarget::anchor() { }

WDC65816Subtarget &
WDC65816Subtarget::initializeSubtargetDependencies(StringRef CPU, StringRef FS) {
  ExtendedInsts = false;
  HWMultMode = NoHWMult;

  std::string CPUName = CPU;
  if (CPUName.empty())
    CPUName = "WDC65816";

  ParseSubtargetFeatures(CPUName, FS);

  return *this;
}

WDC65816Subtarget::WDC65816Subtarget(const Triple &TT, const std::string &CPU,
                                 const std::string &FS, const TargetMachine &TM)
    : WDC65816GenSubtargetInfo(TT, CPU, FS), FrameLowering(),
      InstrInfo(initializeSubtargetDependencies(CPU, FS)), TLInfo(TM, *this) {}
