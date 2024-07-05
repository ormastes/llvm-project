add_llvm_component_group(TISC)

set(LLVM_TARGET_DEFINITIONS TISC.td)

tablegen(LLVM TISCGenAsmMatcher.inc -gen-asm-matcher)
tablegen(LLVM TISCGenAsmWriter.inc -gen-asm-writer)
tablegen(LLVM TISCGenCallingConv.inc -gen-callingconv)
tablegen(LLVM TISCGenDAGISel.inc -gen-dag-isel)
tablegen(LLVM TISCGenDisassemblerTables.inc -gen-disassembler)
tablegen(LLVM TISCGenInstrInfo.inc -gen-instr-info)
tablegen(LLVM TISCGenMCCodeEmitter.inc -gen-emitter)
tablegen(LLVM TISCGenRegisterInfo.inc -gen-register-info)
tablegen(LLVM TISCGenSubtargetInfo.inc -gen-subtarget)

add_public_tablegen_target(TISCCommonTableGen)

add_llvm_target(TISCCodeGen
  TISCBranchSelector.cpp
  TISCISelDAGToDAG.cpp
  TISCISelLowering.cpp
  TISCInstrInfo.cpp
  TISCFrameLowering.cpp
  TISCMachineFunctionInfo.cpp
  TISCRegisterInfo.cpp
  TISCSubtarget.cpp
  TISCTargetMachine.cpp
  TISCAsmPrinter.cpp
  TISCMCInstLower.cpp

  LINK_COMPONENTS
  AsmPrinter
  CodeGen
  CodeGenTypes
  Core
  MC
  TISCDesc
  TISCInfo
  SelectionDAG
  Support
  Target

  ADD_TO_COMPONENT
  TISC
  )

add_subdirectory(MCTargetDesc)
add_subdirectory(TargetInfo)
add_subdirectory(AsmParser)
add_subdirectory(Disassembler)
