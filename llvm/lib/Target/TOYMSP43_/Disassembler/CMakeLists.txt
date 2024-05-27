add_llvm_component_library(LLVMTOYMSP43_Disassembler
  MSP430Disassembler.cpp

  LINK_COMPONENTS
  MCDisassembler
  MC
  TOYMSP43_Info
  Support

  ADD_TO_COMPONENT
  TOYMSP43_
  )
