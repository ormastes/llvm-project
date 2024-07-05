add_llvm_component_library(LLVMTISCInfo
  TISCTargetInfo.cpp

  LINK_COMPONENTS
  MC
  Support

  ADD_TO_COMPONENT
  TISC
  )
