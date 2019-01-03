//===-- WDC65816RegisterInfo.td - WDC65816 Register defs -------*- tablegen -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source 
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
//  Registers
//===----------------------------------------------------------------------===//

class WDC65816Reg<string n, list<Register> subregs=[]> : Register<n> {
  let Namespace = "WDC65816";
  let SubRegs = subregs;
}

// Subregister indices
let Namespace = "WDC65816" in {
  def sub_lo  : SubRegIndex<8, 0>;
  def sub_hi  : SubRegIndex<8, 8>;
}

def A   : WDC65816Reg<"a">;   // low 8 bits of accumulator
def B   : WDC65816Reg<"b">;   // high 8 bits of accumulator

def DBR : WDC65816Reg<"dbr">; // Data Bank Register
def D   : WDC65816Reg<"d">;   // Direct register
def K   : WDC65816Reg<"k">;   // program banK register
def PC  : WDC65816Reg<"pc">;  // Program Counter
def P   : WDC65816Reg<"p">;   // Processor status

def SL  : WDC65816Reg<"sh">;  // low 8 bits of stackpointer
def SH  : WDC65816Reg<"sl">;  // high 8 bits of stackpointer

def XL  : WDC65816Reg<"xl">;  // low 8 bits of X index
def XH  : WDC65816Reg<"xh">;  // high 8 bits of X index
def YL  : WDC65816Reg<"yl">;  // low 8 bits of Y index
def YH  : WDC65816Reg<"yh">;  // high 8 bits of Y index

let SubRegIndices = [sub_lo, sub_hi], CoveredBySubRegs = 1 in {
  def C   : WDC65816Reg<"c", [A, B]>;   // full 16 bits of accumulator
  def S   : WDC65816Reg<"s", [SL, SH]>; // full 16 bits of stackpointer 
  def X   : WDC65816Reg<"x", [XL, XH]>; // full 16 bits of X Index
  def Y   : WDC65816Reg<"y", [YL, YH]>; // full 16 bits of Y Index
}

// note that due to the 816's addressing capabilities, S is what most other
// archs refer to as the framepointer, in that it represents the bottom of the
// active stack, not the top of the stack

//===----------------------------------------------------------------------===//
//  Register classes
//===----------------------------------------------------------------------===//

def GR8 : RegisterClass<"WDC65816", [i8], 8, 
  (add A, B, XL, XH, YL, YH, DBR, K, P, SL, SH)>;

def GR16 : RegisterClass<"WDC65816", [i16], 16, 
  (add C, X, Y, D, S, PC)>;
