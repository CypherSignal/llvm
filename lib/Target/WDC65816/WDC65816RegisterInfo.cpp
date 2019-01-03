//===-- WDC65816RegisterInfo.cpp - WDC65816 Register Information --------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the WDC65816 implementation of the TargetRegisterInfo class.
//
//===----------------------------------------------------------------------===//

#include "WDC65816RegisterInfo.h"
#include "WDC65816.h"
#include "WDC65816MachineFunctionInfo.h"
#include "WDC65816TargetMachine.h"
#include "llvm/ADT/BitVector.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"

using namespace llvm;

#define DEBUG_TYPE "WDC65816-reg-info"

#define GET_REGINFO_TARGET_DESC
#include "WDC65816GenRegisterInfo.inc"

// FIXME: Provide proper call frame setup / destroy opcodes.
WDC65816RegisterInfo::WDC65816RegisterInfo()
  : WDC65816GenRegisterInfo(WDC65816::PC) {}

const MCPhysReg*
WDC65816RegisterInfo::getCalleeSavedRegs(const MachineFunction *MF) const {
  // WDC65816-TODO 
  //const WDC65816FrameLowering *TFI = getFrameLowering(*MF);
  //const Function* F = &MF->getFunction();
  //static const MCPhysReg CalleeSavedRegs[] = {
  //  WDC65816::FP, WDC65816::R5, WDC65816::R6, WDC65816::R7,
  //  WDC65816::R8, WDC65816::R9, WDC65816::R10,
  //  0
  //};
  //static const MCPhysReg CalleeSavedRegsFP[] = {
  //  WDC65816::R5, WDC65816::R6, WDC65816::R7,
  //  WDC65816::R8, WDC65816::R9, WDC65816::R10,
  //  0
  //};
  //static const MCPhysReg CalleeSavedRegsIntr[] = {
  //  WDC65816::FP,  WDC65816::R5,  WDC65816::R6,  WDC65816::R7,
  //  WDC65816::R8,  WDC65816::R9,  WDC65816::R10, WDC65816::R11,
  //  WDC65816::R12, WDC65816::R13, WDC65816::R14, WDC65816::R15,
  //  0
  //};
  //static const MCPhysReg CalleeSavedRegsIntrFP[] = {
  //  WDC65816::R5,  WDC65816::R6,  WDC65816::R7,
  //  WDC65816::R8,  WDC65816::R9,  WDC65816::R10, WDC65816::R11,
  //  WDC65816::R12, WDC65816::R13, WDC65816::R14, WDC65816::R15,
  //  0
  //};

  //if (TFI->hasFP(*MF))
  //  return (F->getCallingConv() == CallingConv::WDC65816_INTR ?
  //          CalleeSavedRegsIntrFP : CalleeSavedRegsFP);
  //else
  //  return (F->getCallingConv() == CallingConv::WDC65816_INTR ?
  //          CalleeSavedRegsIntr : CalleeSavedRegs);
  return nullptr;

}

BitVector WDC65816RegisterInfo::getReservedRegs(const MachineFunction &MF) const {
  BitVector Reserved(getNumRegs());
  // WDC65816-TODO 
  //const WDC65816FrameLowering *TFI = getFrameLowering(MF);

  //// Mark 4 special registers with subregisters as reserved.
  //Reserved.set(WDC65816::PCB);
  //Reserved.set(WDC65816::SPB);
  //Reserved.set(WDC65816::SRB);
  //Reserved.set(WDC65816::CGB);
  //Reserved.set(WDC65816::PC);
  //Reserved.set(WDC65816::SP);
  //Reserved.set(WDC65816::SR);
  //Reserved.set(WDC65816::CG);

  //// Mark frame pointer as reserved if needed.
  //if (TFI->hasFP(MF)) {
  //  Reserved.set(WDC65816::FPB);
  //  Reserved.set(WDC65816::FP);
  //}

  return Reserved;
}

const TargetRegisterClass *
WDC65816RegisterInfo::getPointerRegClass(const MachineFunction &MF, unsigned Kind)
                                                                         const {
  // WDC65816-TODO 
  //return &WDC65816::GR16RegClass;
  return nullptr;
}

void
WDC65816RegisterInfo::eliminateFrameIndex(MachineBasicBlock::iterator II,
                                        int SPAdj, unsigned FIOperandNum,
                                        RegScavenger *RS) const {
  // WDC65816-TODO 
  //assert(SPAdj == 0 && "Unexpected");

  //MachineInstr &MI = *II;
  //MachineBasicBlock &MBB = *MI.getParent();
  //MachineFunction &MF = *MBB.getParent();
  //const WDC65816FrameLowering *TFI = getFrameLowering(MF);
  //DebugLoc dl = MI.getDebugLoc();
  //int FrameIndex = MI.getOperand(FIOperandNum).getIndex();

  //unsigned BasePtr = (TFI->hasFP(MF) ? WDC65816::FP : WDC65816::SP);
  //int Offset = MF.getFrameInfo().getObjectOffset(FrameIndex);

  //// Skip the saved PC
  //Offset += 2;

  //if (!TFI->hasFP(MF))
  //  Offset += MF.getFrameInfo().getStackSize();
  //else
  //  Offset += 2; // Skip the saved FP

  //// Fold imm into offset
  //Offset += MI.getOperand(FIOperandNum + 1).getImm();

  //if (MI.getOpcode() == WDC65816::ADDframe) {
  //  // This is actually "load effective address" of the stack slot
  //  // instruction. We have only two-address instructions, thus we need to
  //  // expand it into mov + add
  //  const TargetInstrInfo &TII = *MF.getSubtarget().getInstrInfo();

  //  MI.setDesc(TII.get(WDC65816::MOV16rr));
  //  MI.getOperand(FIOperandNum).ChangeToRegister(BasePtr, false);

  //  if (Offset == 0)
  //    return;

  //  // We need to materialize the offset via add instruction.
  //  unsigned DstReg = MI.getOperand(0).getReg();
  //  if (Offset < 0)
  //    BuildMI(MBB, std::next(II), dl, TII.get(WDC65816::SUB16ri), DstReg)
  //      .addReg(DstReg).addImm(-Offset);
  //  else
  //    BuildMI(MBB, std::next(II), dl, TII.get(WDC65816::ADD16ri), DstReg)
  //      .addReg(DstReg).addImm(Offset);

  //  return;
  //}

  //MI.getOperand(FIOperandNum).ChangeToRegister(BasePtr, false);
  //MI.getOperand(FIOperandNum + 1).ChangeToImmediate(Offset);
}

unsigned WDC65816RegisterInfo::getFrameRegister(const MachineFunction &MF) const {
  // WDC65816-TODO 
  //const WDC65816FrameLowering *TFI = getFrameLowering(MF);
  //return TFI->hasFP(MF) ? WDC65816::FP : WDC65816::SP;
  return WDC65816::SP;
}
