//===-- WDC65816InstrInfo.h - WDC65816 Instruction Information ------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the WDC65816 implementation of the TargetInstrInfo class.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_WDC65816_WDC65816INSTRINFO_H
#define LLVM_LIB_TARGET_WDC65816_WDC65816INSTRINFO_H

#include "WDC65816RegisterInfo.h"
#include "llvm/CodeGen/TargetInstrInfo.h"

#define GET_INSTRINFO_HEADER
#include "WDC65816GenInstrInfo.inc"

namespace llvm {

class WDC65816Subtarget;

class WDC65816InstrInfo : public WDC65816GenInstrInfo {
  const WDC65816RegisterInfo RI;
  virtual void anchor();
public:
  explicit WDC65816InstrInfo(WDC65816Subtarget &STI);

  /// getRegisterInfo - TargetInstrInfo is a superset of MRegister info.  As
  /// such, whenever a client has an instance of instruction info, it should
  /// always be able to get register info as well (through this method).
  ///
  const TargetRegisterInfo &getRegisterInfo() const { return RI; }

  void copyPhysReg(MachineBasicBlock &MBB, MachineBasicBlock::iterator I,
                   const DebugLoc &DL, unsigned DestReg, unsigned SrcReg,
                   bool KillSrc) const override;

  void storeRegToStackSlot(MachineBasicBlock &MBB,
                           MachineBasicBlock::iterator MI,
                           unsigned SrcReg, bool isKill,
                           int FrameIndex,
                           const TargetRegisterClass *RC,
                           const TargetRegisterInfo *TRI) const override;
  void loadRegFromStackSlot(MachineBasicBlock &MBB,
                            MachineBasicBlock::iterator MI,
                            unsigned DestReg, int FrameIdx,
                            const TargetRegisterClass *RC,
                            const TargetRegisterInfo *TRI) const override;

  unsigned getInstSizeInBytes(const MachineInstr &MI) const override;

  // Branch folding goodness
  bool
  reverseBranchCondition(SmallVectorImpl<MachineOperand> &Cond) const override;
  bool isUnpredicatedTerminator(const MachineInstr &MI) const override;
  bool analyzeBranch(MachineBasicBlock &MBB, MachineBasicBlock *&TBB,
                     MachineBasicBlock *&FBB,
                     SmallVectorImpl<MachineOperand> &Cond,
                     bool AllowModify) const override;

  unsigned removeBranch(MachineBasicBlock &MBB,
                        int *BytesRemoved = nullptr) const override;
  unsigned insertBranch(MachineBasicBlock &MBB, MachineBasicBlock *TBB,
                        MachineBasicBlock *FBB, ArrayRef<MachineOperand> Cond,
                        const DebugLoc &DL,
                        int *BytesAdded = nullptr) const override;

  int64_t getFramePoppedByCallee(const MachineInstr &I) const {
    assert(isFrameInstr(I) && "Not a frame instruction");
    assert(I.getOperand(1).getImm() >= 0 && "Size must not be negative");
    return I.getOperand(1).getImm();
  }
};

}

#endif
