//===-- WDC65816InstrInfo.cpp - WDC65816 Instruction Information --------------===//
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

#include "WDC65816InstrInfo.h"
#include "WDC65816.h"
#include "WDC65816MachineFunctionInfo.h"
#include "WDC65816TargetMachine.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/TargetRegistry.h"

using namespace llvm;

#define GET_INSTRINFO_CTOR_DTOR
#include "WDC65816GenInstrInfo.inc"

// Pin the vtable to this file.
void WDC65816InstrInfo::anchor() {}

WDC65816InstrInfo::WDC65816InstrInfo(WDC65816Subtarget &STI)
    // WDC65816-TODO 
  : //WDC65816GenInstrInfo(WDC65816::ADJCALLSTACKDOWN, WDC65816::ADJCALLSTACKUP),
    RI() {}

void WDC65816InstrInfo::storeRegToStackSlot(MachineBasicBlock &MBB,
                                          MachineBasicBlock::iterator MI,
                                    unsigned SrcReg, bool isKill, int FrameIdx,
                                          const TargetRegisterClass *RC,
                                          const TargetRegisterInfo *TRI) const {
  // WDC65816-TODO 
  // DebugLoc DL;
  // if (MI != MBB.end()) DL = MI->getDebugLoc();
  // MachineFunction &MF = *MBB.getParent();
  // MachineFrameInfo &MFI = MF.getFrameInfo();

  // MachineMemOperand *MMO = MF.getMachineMemOperand(
  //     MachinePointerInfo::getFixedStack(MF, FrameIdx),
  //     MachineMemOperand::MOStore, MFI.getObjectSize(FrameIdx),
  //     MFI.getObjectAlignment(FrameIdx));

  // if (RC == &WDC65816::GR16RegClass)
  //   BuildMI(MBB, MI, DL, get(WDC65816::MOV16mr))
  //     .addFrameIndex(FrameIdx).addImm(0)
  //     .addReg(SrcReg, getKillRegState(isKill)).addMemOperand(MMO);
  // else if (RC == &WDC65816::GR8RegClass)
  //   BuildMI(MBB, MI, DL, get(WDC65816::MOV8mr))
  //     .addFrameIndex(FrameIdx).addImm(0)
  //     .addReg(SrcReg, getKillRegState(isKill)).addMemOperand(MMO);
  // else
  //   llvm_unreachable("Cannot store this register to stack slot!");
}

void WDC65816InstrInfo::loadRegFromStackSlot(MachineBasicBlock &MBB,
                                           MachineBasicBlock::iterator MI,
                                           unsigned DestReg, int FrameIdx,
                                           const TargetRegisterClass *RC,
                                           const TargetRegisterInfo *TRI) const{
  // WDC65816-TODO 
  // DebugLoc DL;
  // if (MI != MBB.end()) DL = MI->getDebugLoc();
  // MachineFunction &MF = *MBB.getParent();
  // MachineFrameInfo &MFI = MF.getFrameInfo();

  // MachineMemOperand *MMO = MF.getMachineMemOperand(
  //     MachinePointerInfo::getFixedStack(MF, FrameIdx),
  //     MachineMemOperand::MOLoad, MFI.getObjectSize(FrameIdx),
  //     MFI.getObjectAlignment(FrameIdx));

  // if (RC == &WDC65816::GR16RegClass)
  //   BuildMI(MBB, MI, DL, get(WDC65816::MOV16rm))
  //     .addReg(DestReg, getDefRegState(true)).addFrameIndex(FrameIdx)
  //     .addImm(0).addMemOperand(MMO);
  // else if (RC == &WDC65816::GR8RegClass)
  //   BuildMI(MBB, MI, DL, get(WDC65816::MOV8rm))
  //     .addReg(DestReg, getDefRegState(true)).addFrameIndex(FrameIdx)
  //     .addImm(0).addMemOperand(MMO);
  // else
  //   llvm_unreachable("Cannot store this register to stack slot!");
}

void WDC65816InstrInfo::copyPhysReg(MachineBasicBlock &MBB,
                                  MachineBasicBlock::iterator I,
                                  const DebugLoc &DL, unsigned DestReg,
                                  unsigned SrcReg, bool KillSrc) const {
  // WDC65816-TODO 
  // unsigned Opc;
  // if (WDC65816::GR16RegClass.contains(DestReg, SrcReg))
  //   Opc = WDC65816::MOV16rr;
  // else if (WDC65816::GR8RegClass.contains(DestReg, SrcReg))
  //   Opc = WDC65816::MOV8rr;
  // else
  //   llvm_unreachable("Impossible reg-to-reg copy");

  // BuildMI(MBB, I, DL, get(Opc), DestReg)
  //   .addReg(SrcReg, getKillRegState(KillSrc));
}

unsigned WDC65816InstrInfo::removeBranch(MachineBasicBlock &MBB,
                                       int *BytesRemoved) const {
  assert(!BytesRemoved && "code size not handled");

  MachineBasicBlock::iterator I = MBB.end();
  unsigned Count = 0;

  // WDC65816-TODO 
  //while (I != MBB.begin()) {
  //  --I;
  //  if (I->isDebugInstr())
  //    continue;
  //  if (I->getOpcode() != WDC65816::JMP &&
  //      I->getOpcode() != WDC65816::JCC &&
  //      I->getOpcode() != WDC65816::Br &&
  //      I->getOpcode() != WDC65816::Bm)
  //    break;
  //  // Remove the branch.
  //  I->eraseFromParent();
  //  I = MBB.end();
  //  ++Count;
  //}

  return Count;
}

bool WDC65816InstrInfo::
reverseBranchCondition(SmallVectorImpl<MachineOperand> &Cond) const {
  assert(Cond.size() == 1 && "Invalid Xbranch condition!");

  WDC65816CC::CondCodes CC = static_cast<WDC65816CC::CondCodes>(Cond[0].getImm());

  switch (CC) {
  default: llvm_unreachable("Invalid branch condition!");
  case WDC65816CC::COND_E:
    CC = WDC65816CC::COND_NE;
    break;
  case WDC65816CC::COND_NE:
    CC = WDC65816CC::COND_E;
    break;
  case WDC65816CC::COND_L:
    CC = WDC65816CC::COND_GE;
    break;
  case WDC65816CC::COND_GE:
    CC = WDC65816CC::COND_L;
    break;
  case WDC65816CC::COND_HS:
    CC = WDC65816CC::COND_LO;
    break;
  case WDC65816CC::COND_LO:
    CC = WDC65816CC::COND_HS;
    break;
  }

  Cond[0].setImm(CC);
  return false;
}

bool WDC65816InstrInfo::isUnpredicatedTerminator(const MachineInstr &MI) const {
  if (!MI.isTerminator())
    return false;

  // Conditional branch is a special case.
  if (MI.isBranch() && !MI.isBarrier())
    return true;
  if (!MI.isPredicable())
    return true;
  return !isPredicated(MI);
}

bool WDC65816InstrInfo::analyzeBranch(MachineBasicBlock &MBB,
                                    MachineBasicBlock *&TBB,
                                    MachineBasicBlock *&FBB,
                                    SmallVectorImpl<MachineOperand> &Cond,
                                    bool AllowModify) const {
  // WDC65816-TODO 
  //// Start from the bottom of the block and work up, examining the
  //// terminator instructions.
  //MachineBasicBlock::iterator I = MBB.end();
  //while (I != MBB.begin()) {
  //  --I;
  //  if (I->isDebugInstr())
  //    continue;

  //  // Working from the bottom, when we see a non-terminator
  //  // instruction, we're done.
  //  if (!isUnpredicatedTerminator(*I))
  //    break;

  //  // A terminator that isn't a branch can't easily be handled
  //  // by this analysis.
  //  if (!I->isBranch())
  //    return true;

  //  // Cannot handle indirect branches.
  //  if (I->getOpcode() == WDC65816::Br ||
  //      I->getOpcode() == WDC65816::Bm)
  //    return true;

  //  // Handle unconditional branches.
  //  if (I->getOpcode() == WDC65816::JMP) {
  //    if (!AllowModify) {
  //      TBB = I->getOperand(0).getMBB();
  //      continue;
  //    }

  //    // If the block has any instructions after a JMP, delete them.
  //    while (std::next(I) != MBB.end())
  //      std::next(I)->eraseFromParent();
  //    Cond.clear();
  //    FBB = nullptr;

  //    // Delete the JMP if it's equivalent to a fall-through.
  //    if (MBB.isLayoutSuccessor(I->getOperand(0).getMBB())) {
  //      TBB = nullptr;
  //      I->eraseFromParent();
  //      I = MBB.end();
  //      continue;
  //    }

  //    // TBB is used to indicate the unconditinal destination.
  //    TBB = I->getOperand(0).getMBB();
  //    continue;
  //  }

  //  // Handle conditional branches.
  //  assert(I->getOpcode() == WDC65816::JCC && "Invalid conditional branch");
  //  WDC65816CC::CondCodes BranchCode =
  //    static_cast<WDC65816CC::CondCodes>(I->getOperand(1).getImm());
  //  if (BranchCode == WDC65816CC::COND_INVALID)
  //    return true;  // Can't handle weird stuff.

  //  // Working from the bottom, handle the first conditional branch.
  //  if (Cond.empty()) {
  //    FBB = TBB;
  //    TBB = I->getOperand(0).getMBB();
  //    Cond.push_back(MachineOperand::CreateImm(BranchCode));
  //    continue;
  //  }

  //  // Handle subsequent conditional branches. Only handle the case where all
  //  // conditional branches branch to the same destination.
  //  assert(Cond.size() == 1);
  //  assert(TBB);

  //  // Only handle the case where all conditional branches branch to
  //  // the same destination.
  //  if (TBB != I->getOperand(0).getMBB())
  //    return true;

  //  WDC65816CC::CondCodes OldBranchCode = (WDC65816CC::CondCodes)Cond[0].getImm();
  //  // If the conditions are the same, we can leave them alone.
  //  if (OldBranchCode == BranchCode)
  //    continue;

  //  return true;
  //}

  return false;
}

unsigned WDC65816InstrInfo::insertBranch(MachineBasicBlock &MBB,
                                       MachineBasicBlock *TBB,
                                       MachineBasicBlock *FBB,
                                       ArrayRef<MachineOperand> Cond,
                                       const DebugLoc &DL,
                                       int *BytesAdded) const {
  // WDC65816-TODO 
  // Shouldn't be a fall through.
  //assert(TBB && "insertBranch must not be told to insert a fallthrough");
  //assert((Cond.size() == 1 || Cond.size() == 0) &&
  //       "WDC65816 branch conditions have one component!");
  //assert(!BytesAdded && "code size not handled");

  //if (Cond.empty()) {
  //  // Unconditional branch?
  //  assert(!FBB && "Unconditional branch with multiple successors!");
  //  BuildMI(&MBB, DL, get(WDC65816::JMP)).addMBB(TBB);
  //  return 1;
  //}

  //// Conditional branch.
  //unsigned Count = 0;
  //BuildMI(&MBB, DL, get(WDC65816::JCC)).addMBB(TBB).addImm(Cond[0].getImm());
  //++Count;

  //if (FBB) {
  //  // Two-way Conditional branch. Insert the second branch.
  //  BuildMI(&MBB, DL, get(WDC65816::JMP)).addMBB(FBB);
  //  ++Count;
  //}
  //return Count;
  return 0;
}

/// GetInstSize - Return the number of bytes of code the specified
/// instruction may be.  This returns the maximum number of bytes.
///
unsigned WDC65816InstrInfo::getInstSizeInBytes(const MachineInstr &MI) const {
  const MCInstrDesc &Desc = MI.getDesc();

  // WDC65816-TODO 
  //switch (Desc.getOpcode()) {
  //case TargetOpcode::CFI_INSTRUCTION:
  //case TargetOpcode::EH_LABEL:
  //case TargetOpcode::IMPLICIT_DEF:
  //case TargetOpcode::KILL:
  //case TargetOpcode::DBG_VALUE:
  //  return 0;
  //case TargetOpcode::INLINEASM: {
  //  const MachineFunction *MF = MI.getParent()->getParent();
  //  const TargetInstrInfo &TII = *MF->getSubtarget().getInstrInfo();
  //  return TII.getInlineAsmLength(MI.getOperand(0).getSymbolName(),
  //                                *MF->getTarget().getMCAsmInfo());
  //}
  //}

  return Desc.getSize();
}
