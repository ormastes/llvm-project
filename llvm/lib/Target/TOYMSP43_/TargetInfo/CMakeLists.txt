add_llvm_component_library(LLVMTOYMSP43_Info
  MSP430TargetInfo.cpp

  LINK_COMPONENTS
  MC
  Support

  ADD_TO_COMPONENT
  TOYMSP43_
  )
