//===-- WDC65816RegisterInfo.td - WDC65816 Register defs -------*- tablegen -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source 
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
//  Declarations that describe the WDC65816 register file
//===----------------------------------------------------------------------===//

class WDC65816Reg<bits<4> num, string n, list<string> alt = []> : Register<n> {
  field bits<4> Num = num;
  let Namespace = "WDC65816";
  let HWEncoding{3-0} = num;
  let AltNames = alt;
}

def PC  : WDC65816Reg<0,  "r0", ["pc"]>;
def SP  : WDC65816Reg<1,  "r1", ["sp"]>;

def GR16 : RegisterClass<"WDC65816", [i16], 16, (add PC, SP)>;

// class WDC65816RegWithSubregs<bits<4> num, string n, list<Register> subregs,
//                            list<string> alt = []> 
//   : RegisterWithSubRegs<n, subregs> {
//   field bits<4> Num = num;
//   let Namespace = "WDC65816";
//   let HWEncoding{3-0} = num;
//   let AltNames = alt;
// }

//===----------------------------------------------------------------------===//
//  Registers
//===----------------------------------------------------------------------===//

// def PCB  : WDC65816Reg<0,  "r0", ["pc"]>;
// def SPB  : WDC65816Reg<1,  "r1", ["sp"]>;
// def SRB  : WDC65816Reg<2,  "r2", ["sr"]>;
// def CGB  : WDC65816Reg<3,  "r3", ["cg"]>;
// def FPB  : WDC65816Reg<4,  "r4", ["fp"]>;
// def R5B  : WDC65816Reg<5,  "r5">;
// def R6B  : WDC65816Reg<6,  "r6">;
// def R7B  : WDC65816Reg<7,  "r7">;
// def R8B  : WDC65816Reg<8,  "r8">;
// def R9B  : WDC65816Reg<9,  "r9">;
// def R10B : WDC65816Reg<10, "r10">;
// def R11B : WDC65816Reg<11, "r11">;
// def R12B : WDC65816Reg<12, "r12">;
// def R13B : WDC65816Reg<13, "r13">;
// def R14B : WDC65816Reg<14, "r14">;
// def R15B : WDC65816Reg<15, "r15">;

// def subreg_8bit : SubRegIndex<8> { let Namespace = "WDC65816"; }

// let SubRegIndices = [subreg_8bit] in {
// def PC  : WDC65816RegWithSubregs<0,  "r0",  [PCB], ["pc"]>;
// def SP  : WDC65816RegWithSubregs<1,  "r1",  [SPB], ["sp"]>;
// def SR  : WDC65816RegWithSubregs<2,  "r2",  [SRB], ["sr"]>;
// def CG  : WDC65816RegWithSubregs<3,  "r3",  [CGB], ["cg"]>;
// def FP  : WDC65816RegWithSubregs<4,  "r4",  [FPB], ["fp"]>;
// def R5  : WDC65816RegWithSubregs<5,  "r5",  [R5B]>;
// def R6  : WDC65816RegWithSubregs<6,  "r6",  [R6B]>;
// def R7  : WDC65816RegWithSubregs<7,  "r7",  [R7B]>;
// def R8  : WDC65816RegWithSubregs<8,  "r8",  [R8B]>;
// def R9  : WDC65816RegWithSubregs<9,  "r9",  [R9B]>;
// def R10 : WDC65816RegWithSubregs<10, "r10", [R10B]>;
// def R11 : WDC65816RegWithSubregs<11, "r11", [R11B]>;
// def R12 : WDC65816RegWithSubregs<12, "r12", [R12B]>;
// def R13 : WDC65816RegWithSubregs<13, "r13", [R13B]>;
// def R14 : WDC65816RegWithSubregs<14, "r14", [R14B]>;
// def R15 : WDC65816RegWithSubregs<15, "r15", [R15B]>;
// }

// def GR8 : RegisterClass<"WDC65816", [i8], 8,
//    // Volatile registers
//   (add R12B, R13B, R14B, R15B, R11B, R10B, R9B, R8B, R7B, R6B, R5B,
//    // Frame pointer, sometimes allocable
//    FPB,
//    // Volatile, but not allocable
//    PCB, SPB, SRB, CGB)>;

// def GR16 : RegisterClass<"WDC65816", [i16], 16,
//    // Volatile registers
//   (add R12, R13, R14, R15, R11, R10, R9, R8, R7, R6, R5,
//    // Frame pointer, sometimes allocable
//    FP,
//    // Volatile, but not allocable
//    PC, SP, SR, CG)>;
