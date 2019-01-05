//===-- WDC65816TargetMachine.cpp - Define TargetMachine for WDC65816 ---------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// Top-level implementation for the WDC65816 target.
//
//===----------------------------------------------------------------------===//

#include "WDC65816TargetMachine.h"
#include "WDC65816.h"
#include "llvm/CodeGen/Passes.h"
#include "llvm/CodeGen/TargetLoweringObjectFileImpl.h"
#include "llvm/CodeGen/TargetPassConfig.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/Support/TargetRegistry.h"
using namespace llvm;

extern "C" void LLVMInitializeWDC65816Target() {
  // Register the target.
  RegisterTargetMachine<WDC65816TargetMachine> X(getTheWDC65816Target());
}

static Reloc::Model getEffectiveRelocModel(Optional<Reloc::Model> RM) {
  if (!RM.hasValue())
    return Reloc::Static;
  return *RM;
}

static std::string computeDataLayout(const Triple &TT, StringRef CPU,
                                     const TargetOptions &Options) {
  return "e-p:16:8-i16:8-a:8-n8:16-S8";
}

WDC65816TargetMachine::WDC65816TargetMachine(const Target &T, const Triple &TT,
                                         StringRef CPU, StringRef FS,
                                         const TargetOptions &Options,
                                         Optional<Reloc::Model> RM,
                                         Optional<CodeModel::Model> CM,
                                         CodeGenOpt::Level OL, bool JIT)
    : LLVMTargetMachine(T, computeDataLayout(TT, CPU, Options), TT, CPU, FS,
                        Options, getEffectiveRelocModel(RM),
                        getEffectiveCodeModel(CM, CodeModel::Small), OL),
      TLOF(make_unique<TargetLoweringObjectFileELF>()),
      Subtarget(TT, CPU, FS, *this) {
  initAsmInfo();
}

WDC65816TargetMachine::~WDC65816TargetMachine() {}

namespace {
/// WDC65816 Code Generator Pass Configuration Options.
class WDC65816PassConfig : public TargetPassConfig {
public:
  WDC65816PassConfig(WDC65816TargetMachine &TM, PassManagerBase &PM)
    : TargetPassConfig(TM, PM) {}

  WDC65816TargetMachine &getWDC65816TargetMachine() const {
    return getTM<WDC65816TargetMachine>();
  }

  bool addInstSelector() override;
  void addPreEmitPass() override;
};
} // namespace

TargetPassConfig *WDC65816TargetMachine::createPassConfig(PassManagerBase &PM) {
  return new WDC65816PassConfig(*this, PM);
}

bool WDC65816PassConfig::addInstSelector() {
  // Install an instruction selector.
  addPass(createWDC65816ISelDag(getWDC65816TargetMachine(), getOptLevel()));
  return false;
}

void WDC65816PassConfig::addPreEmitPass() {
  // Must run branch selection immediately preceding the asm printer.
  addPass(createWDC65816BranchSelectionPass(), false);
}
