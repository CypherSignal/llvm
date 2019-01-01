//===-- WDC65816.td - Describe the WDC65816 Target Machine -----*- tablegen -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source 
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
// This is the top level entry point for the WDC65816 target.
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// Target-independent interfaces
//===----------------------------------------------------------------------===//

include "llvm/Target/Target.td"

//===----------------------------------------------------------------------===//
// Subtarget Features. 
//===----------------------------------------------------------------------===//
def FeatureX
 : SubtargetFeature<"ext", "ExtendedInsts", "true",
                    "Enable WDC65816-X extensions">;

def FeatureHWMult16
 : SubtargetFeature<"hwmult16", "HWMultMode", "HWMult16",
                    "Enable 16-bit hardware multiplier">;

def FeatureHWMult32
 : SubtargetFeature<"hwmult32", "HWMultMode", "HWMult32",
                    "Enable 32-bit hardware multiplier">;

def FeatureHWMultF5
 : SubtargetFeature<"hwmultf5", "HWMultMode", "HWMultF5",
                    "Enable F5 series hardware multiplier">;

//===----------------------------------------------------------------------===//
// WDC65816 supported processors.
//===----------------------------------------------------------------------===//
class Proc<string Name, list<SubtargetFeature> Features>
 : Processor<Name, NoItineraries, Features>;

def : Proc<"generic",         []>;
def : Proc<"WDC65816",          []>;
def : Proc<"WDC65816x",         [FeatureX]>;

//===----------------------------------------------------------------------===//
// Register File Description
//===----------------------------------------------------------------------===//

include "WDC65816RegisterInfo.td"

//===----------------------------------------------------------------------===//
// Calling Convention Description
//===----------------------------------------------------------------------===//

include "WDC65816CallingConv.td"

//===----------------------------------------------------------------------===//
// Instruction Descriptions
//===----------------------------------------------------------------------===//

include "WDC65816InstrInfo.td"

def WDC65816InstrInfo : InstrInfo;

//===---------------------------------------------------------------------===//
// Assembly Printers
//===---------------------------------------------------------------------===//

def WDC65816AsmWriter : AsmWriter {
  string AsmWriterClassName = "InstPrinter";
}

//===----------------------------------------------------------------------===//
// Target Declaration
//===----------------------------------------------------------------------===//

def WDC65816 : Target {
  let InstructionSet = WDC65816InstrInfo;
}

