//==- WDC65816CallingConv.td - Calling Conventions for WDC65816 -*- tablegen -*-==//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
// This describes the calling conventions for WDC65816 architecture.
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// WDC65816 Return Value Calling Convention
//===----------------------------------------------------------------------===//
def RetCC_WDC65816 : CallingConv<[
  // WDC65816-TODO: do we want to use all low/high regs? Would it be costlier
  // to get data from YH back into A or C, compared to just reading from stack...?
  // i8 are returned in accumulator and index registers
  CCIfType<[i8], CCAssignToReg<[A, B, XL, YL, XH, YH]>>,

  // i16 are returned in accumulator and index registers
  CCIfType<[i16], CCAssignToReg<[C, X, Y]>>

  // WDC65816-TODO: can we return on the stack for larger data?
]>;

//===----------------------------------------------------------------------===//
// WDC65816 Argument Calling Conventions
//===----------------------------------------------------------------------===//
def CC_WDC65816_AssignStack : CallingConv<[
  // try to assign 8b values into 8b registers, then spill to stack
  CCIfType<[i8], CCAssignToReg<[A, B, XL, YL, XH, YH]>>,
  CCIfType<[i8], CCAssignToStack<1, 1>>,

  // try to assign 16b values into 16b registers, then spill to stack
  CCIfType<[i16], CCAssignToReg<[C, X, Y]>>,
  CCIfType<[i16], CCAssignToStack<2, 1>>
]>;

