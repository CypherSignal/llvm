//===-- WDC65816TargetMachine.h - Define TargetMachine for WDC65816 -*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file declares the WDC65816 specific subclass of TargetMachine.
//
//===----------------------------------------------------------------------===//


#ifndef LLVM_LIB_TARGET_WDC65816_WDC65816TARGETMACHINE_H
#define LLVM_LIB_TARGET_WDC65816_WDC65816TARGETMACHINE_H

#include "WDC65816Subtarget.h"
#include "llvm/CodeGen/TargetFrameLowering.h"
#include "llvm/Target/TargetMachine.h"

namespace llvm {

/// WDC65816TargetMachine
///
class WDC65816TargetMachine : public LLVMTargetMachine {
  std::unique_ptr<TargetLoweringObjectFile> TLOF;
  WDC65816Subtarget        Subtarget;

public:
  WDC65816TargetMachine(const Target &T, const Triple &TT, StringRef CPU,
                      StringRef FS, const TargetOptions &Options,
                      Optional<Reloc::Model> RM, Optional<CodeModel::Model> CM,
                      CodeGenOpt::Level OL, bool JIT);
  ~WDC65816TargetMachine() override;

  const WDC65816Subtarget *getSubtargetImpl(const Function &F) const override {
    return &Subtarget;
  }
  TargetPassConfig *createPassConfig(PassManagerBase &PM) override;

  TargetLoweringObjectFile *getObjFileLowering() const override {
    return TLOF.get();
  }
}; // WDC65816TargetMachine.

} // end namespace llvm

#endif
