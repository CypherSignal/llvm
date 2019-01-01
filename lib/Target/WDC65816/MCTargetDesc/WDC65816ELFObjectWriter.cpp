//===-- WDC65816ELFObjectWriter.cpp - WDC65816 ELF Writer ---------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "MCTargetDesc/WDC65816FixupKinds.h"
#include "MCTargetDesc/WDC65816MCTargetDesc.h"

#include "MCTargetDesc/WDC65816MCTargetDesc.h"
#include "llvm/MC/MCELFObjectWriter.h"
#include "llvm/MC/MCFixup.h"
#include "llvm/MC/MCObjectWriter.h"
#include "llvm/MC/MCValue.h"
#include "llvm/Support/ErrorHandling.h"

using namespace llvm;

namespace {
class WDC65816ELFObjectWriter : public MCELFObjectTargetWriter {
public:
  WDC65816ELFObjectWriter(uint8_t OSABI)
    : MCELFObjectTargetWriter(false, OSABI, ELF::EM_WDC65816,
                              /*HasRelocationAddend*/ true) {}

  ~WDC65816ELFObjectWriter() override {}

protected:
  unsigned getRelocType(MCContext &Ctx, const MCValue &Target,
                        const MCFixup &Fixup, bool IsPCRel) const override {
    // Translate fixup kind to ELF relocation type.
    switch ((unsigned)Fixup.getKind()) {
    case FK_Data_1:                   return ELF::R_MSP430_8;
    case FK_Data_2:                   return ELF::R_MSP430_16_BYTE;
    case FK_Data_4:                   return ELF::R_MSP430_32;
    case WDC65816::fixup_32:            return ELF::R_MSP430_32;
    case WDC65816::fixup_10_pcrel:      return ELF::R_MSP430_10_PCREL;
    case WDC65816::fixup_16:            return ELF::R_MSP430_16;
    case WDC65816::fixup_16_pcrel:      return ELF::R_MSP430_16_PCREL;
    case WDC65816::fixup_16_byte:       return ELF::R_MSP430_16_BYTE;
    case WDC65816::fixup_16_pcrel_byte: return ELF::R_MSP430_16_PCREL_BYTE;
    case WDC65816::fixup_2x_pcrel:      return ELF::R_MSP430_2X_PCREL;
    case WDC65816::fixup_rl_pcrel:      return ELF::R_MSP430_RL_PCREL;
    case WDC65816::fixup_8:             return ELF::R_MSP430_8;
    case WDC65816::fixup_sym_diff:      return ELF::R_MSP430_SYM_DIFF;
    default:
      llvm_unreachable("Invalid fixup kind");
    }
  }
};
} // end of anonymous namespace

std::unique_ptr<MCObjectTargetWriter>
llvm::createWDC65816ELFObjectWriter(uint8_t OSABI) {
  return llvm::make_unique<WDC65816ELFObjectWriter>(OSABI);
}
