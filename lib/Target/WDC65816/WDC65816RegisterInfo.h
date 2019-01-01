//===-- WDC65816RegisterInfo.h - WDC65816 Register Information Impl -*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the WDC65816 implementation of the MRegisterInfo class.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_WDC65816_WDC65816REGISTERINFO_H
#define LLVM_LIB_TARGET_WDC65816_WDC65816REGISTERINFO_H

#include "llvm/CodeGen/TargetRegisterInfo.h"

#define GET_REGINFO_HEADER
#include "WDC65816GenRegisterInfo.inc"

namespace llvm {

struct WDC65816RegisterInfo : public WDC65816GenRegisterInfo {
public:
  WDC65816RegisterInfo();

  /// Code Generation virtual methods...
  const MCPhysReg *getCalleeSavedRegs(const MachineFunction *MF) const override;

  BitVector getReservedRegs(const MachineFunction &MF) const override;
  const TargetRegisterClass*
  getPointerRegClass(const MachineFunction &MF,
                     unsigned Kind = 0) const override;

  void eliminateFrameIndex(MachineBasicBlock::iterator II,
                           int SPAdj, unsigned FIOperandNum,
                           RegScavenger *RS = nullptr) const override;

  // Debug information queries.
  unsigned getFrameRegister(const MachineFunction &MF) const override;
};

} // end namespace llvm

#endif
