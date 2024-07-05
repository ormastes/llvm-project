add_llvm_component_library(LLVMTISCAsmParser
  TISCAsmParser.cpp

  LINK_COMPONENTS
  CodeGenTypes
  MC
  MCParser
  TISCDesc
  TISCInfo
  Support

  ADD_TO_COMPONENT
  TISC
  )
