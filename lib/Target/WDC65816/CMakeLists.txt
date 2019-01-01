set(LLVM_TARGET_DEFINITIONS WDC65816.td)

# tablegen(LLVM WDC65816GenAsmMatcher.inc -gen-asm-matcher)
tablegen(LLVM WDC65816GenAsmWriter.inc -gen-asm-writer)
tablegen(LLVM WDC65816GenCallingConv.inc -gen-callingconv)
tablegen(LLVM WDC65816GenDAGISel.inc -gen-dag-isel)
tablegen(LLVM WDC65816GenDisassemblerTables.inc -gen-disassembler)
tablegen(LLVM WDC65816GenInstrInfo.inc -gen-instr-info)
tablegen(LLVM WDC65816GenMCCodeEmitter.inc -gen-emitter)
tablegen(LLVM WDC65816GenRegisterInfo.inc -gen-register-info)
tablegen(LLVM WDC65816GenSubtargetInfo.inc -gen-subtarget)

add_public_tablegen_target(WDC65816CommonTableGen)

add_llvm_target(WDC65816CodeGen
  WDC65816BranchSelector.cpp
  WDC65816ISelDAGToDAG.cpp
  WDC65816ISelLowering.cpp
  WDC65816InstrInfo.cpp
  WDC65816FrameLowering.cpp
  WDC65816MachineFunctionInfo.cpp
  WDC65816RegisterInfo.cpp
  WDC65816Subtarget.cpp
  WDC65816TargetMachine.cpp
  WDC65816AsmPrinter.cpp
  WDC65816MCInstLower.cpp
  )

add_subdirectory(InstPrinter)
add_subdirectory(MCTargetDesc)
add_subdirectory(TargetInfo)
