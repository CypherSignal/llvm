//===-- WDC65816ELFStreamer.cpp - WDC65816 ELF Target Streamer Methods --------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file provides WDC65816 specific target streamer methods.
//
//===----------------------------------------------------------------------===//

#include "WDC65816MCTargetDesc.h"
#include "llvm/BinaryFormat/ELF.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCELFStreamer.h"
#include "llvm/MC/MCSectionELF.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/MC/MCSubtargetInfo.h"

using namespace llvm;

namespace llvm {

class WDC65816TargetELFStreamer : public MCTargetStreamer {
public:
  MCELFStreamer &getStreamer();
  WDC65816TargetELFStreamer(MCStreamer &S, const MCSubtargetInfo &STI);
};

// This part is for ELF object output.
WDC65816TargetELFStreamer::WDC65816TargetELFStreamer(MCStreamer &S,
                                                 const MCSubtargetInfo &STI)
    : MCTargetStreamer(S) {
  MCAssembler &MCA = getStreamer().getAssembler();
  unsigned EFlags = MCA.getELFHeaderEFlags();
  MCA.setELFHeaderEFlags(EFlags);

  // Emit build attributes section according to
  // WDC65816 EABI (slaa534.pdf, part 13).
  MCSection *AttributeSection = getStreamer().getContext().getELFSection(
      ".WDC65816.attributes", ELF::SHT_MSP430_ATTRIBUTES, 0);
  Streamer.SwitchSection(AttributeSection);

  // Format version.
  Streamer.EmitIntValue(0x41, 1);
  // Subsection length.
  Streamer.EmitIntValue(22, 4);
  // Vendor name string, zero-terminated.
  Streamer.EmitBytes("mspabi");
  Streamer.EmitIntValue(0, 1);

  // Attribute vector scope tag. 1 stands for the entire file.
  Streamer.EmitIntValue(1, 1);
  // Attribute vector length.
  Streamer.EmitIntValue(11, 4);
  // OFBA_MSPABI_Tag_ISA(4) = 1, WDC65816
  Streamer.EmitIntValue(4, 1);
  Streamer.EmitIntValue(1, 1);
  // OFBA_MSPABI_Tag_Code_Model(6) = 1, Small
  Streamer.EmitIntValue(6, 1);
  Streamer.EmitIntValue(1, 1);
  // OFBA_MSPABI_Tag_Data_Model(8) = 1, Small
  Streamer.EmitIntValue(8, 1);
  Streamer.EmitIntValue(1, 1);
}

MCELFStreamer &WDC65816TargetELFStreamer::getStreamer() {
  return static_cast<MCELFStreamer &>(Streamer);
}

MCTargetStreamer *
createWDC65816ObjectTargetStreamer(MCStreamer &S, const MCSubtargetInfo &STI) {
  const Triple &TT = STI.getTargetTriple();
  if (TT.isOSBinFormatELF())
    return new WDC65816TargetELFStreamer(S, STI);
  return nullptr;
}

} // namespace llvm
