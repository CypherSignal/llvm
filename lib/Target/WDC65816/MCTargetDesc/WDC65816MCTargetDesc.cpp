//===-- WDC65816MCTargetDesc.cpp - WDC65816 Target Descriptions ---------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file provides WDC65816 specific target descriptions.
//
//===----------------------------------------------------------------------===//

#include "WDC65816MCTargetDesc.h"
#include "InstPrinter/WDC65816InstPrinter.h"
#include "WDC65816MCAsmInfo.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/Support/TargetRegistry.h"

using namespace llvm;

#define GET_INSTRINFO_MC_DESC
#include "WDC65816GenInstrInfo.inc"

#define GET_SUBTARGETINFO_MC_DESC
#include "WDC65816GenSubtargetInfo.inc"

#define GET_REGINFO_MC_DESC
#include "WDC65816GenRegisterInfo.inc"

static MCInstrInfo *createWDC65816MCInstrInfo() {
  MCInstrInfo *X = new MCInstrInfo();
  InitWDC65816MCInstrInfo(X);
  return X;
}

static MCRegisterInfo *createWDC65816MCRegisterInfo(const Triple &TT) {
  MCRegisterInfo *X = new MCRegisterInfo();
  InitWDC65816MCRegisterInfo(X, WDC65816::PC);
  return X;
}

static MCSubtargetInfo *
createWDC65816MCSubtargetInfo(const Triple &TT, StringRef CPU, StringRef FS) {
  return createWDC65816MCSubtargetInfoImpl(TT, CPU, FS);
}

static MCInstPrinter *createWDC65816MCInstPrinter(const Triple &T,
                                                unsigned SyntaxVariant,
                                                const MCAsmInfo &MAI,
                                                const MCInstrInfo &MII,
                                                const MCRegisterInfo &MRI) {
  if (SyntaxVariant == 0)
    return new WDC65816InstPrinter(MAI, MII, MRI);
  return nullptr;
}

extern "C" void LLVMInitializeWDC65816TargetMC() {
  Target &T = getTheWDC65816Target();

  RegisterMCAsmInfo<WDC65816MCAsmInfo> X(T);
  TargetRegistry::RegisterMCInstrInfo(T, createWDC65816MCInstrInfo);
  TargetRegistry::RegisterMCRegInfo(T, createWDC65816MCRegisterInfo);
  TargetRegistry::RegisterMCSubtargetInfo(T, createWDC65816MCSubtargetInfo);
  TargetRegistry::RegisterMCInstPrinter(T, createWDC65816MCInstPrinter);
  TargetRegistry::RegisterMCCodeEmitter(T, createWDC65816MCCodeEmitter);
  TargetRegistry::RegisterMCAsmBackend(T, createWDC65816MCAsmBackend);
  TargetRegistry::RegisterObjectTargetStreamer(
      T, createWDC65816ObjectTargetStreamer);
}
