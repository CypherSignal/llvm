//===-- WDC65816MCTargetDesc.h - WDC65816 Target Descriptions -------*- C++ -*-===//
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

#ifndef LLVM_LIB_TARGET_WDC65816_MCTARGETDESC_WDC65816MCTARGETDESC_H
#define LLVM_LIB_TARGET_WDC65816_MCTARGETDESC_WDC65816MCTARGETDESC_H

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

Target &getTheWDC65816Target();

/// Creates a machine code emitter for WDC65816.
MCCodeEmitter *createWDC65816MCCodeEmitter(const MCInstrInfo &MCII,
                                         const MCRegisterInfo &MRI,
                                         MCContext &Ctx);

MCAsmBackend *createWDC65816MCAsmBackend(const Target &T,
                                       const MCSubtargetInfo &STI,
                                       const MCRegisterInfo &MRI,
                                       const MCTargetOptions &Options);

MCTargetStreamer *
createWDC65816ObjectTargetStreamer(MCStreamer &S, const MCSubtargetInfo &STI);

std::unique_ptr<MCObjectTargetWriter>
createWDC65816ELFObjectWriter(uint8_t OSABI);

} // End llvm namespace

// Defines symbolic names for WDC65816 registers.
// This defines a mapping from register name to register number.
#define GET_REGINFO_ENUM
#include "WDC65816GenRegisterInfo.inc"

// Defines symbolic names for the WDC65816 instructions.
#define GET_INSTRINFO_ENUM
#include "WDC65816GenInstrInfo.inc"

#define GET_SUBTARGETINFO_ENUM
#include "WDC65816GenSubtargetInfo.inc"

#endif
