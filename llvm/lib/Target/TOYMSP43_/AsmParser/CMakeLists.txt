add_llvm_component_library(LLVMTOYMSP43_AsmParser
  MSP430AsmParser.cpp

  LINK_COMPONENTS
  CodeGenTypes
  MC
  MCParser
  TOYMSP43_Desc
  TOYMSP43_Info
  Support

  ADD_TO_COMPONENT
  TOYMSP43_
  )
