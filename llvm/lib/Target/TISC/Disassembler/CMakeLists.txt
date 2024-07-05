add_llvm_component_library(LLVMTISCDisassembler
  TISCDisassembler.cpp

  LINK_COMPONENTS
  MCDisassembler
  MC
  TISCInfo
  Support

  ADD_TO_COMPONENT
  TISC
  )
