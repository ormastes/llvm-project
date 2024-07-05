add_llvm_component_library(LLVMTISCDesc
  TISCAsmBackend.cpp
  TISCELFObjectWriter.cpp
  TISCELFStreamer.cpp
  TISCInstPrinter.cpp
  TISCMCAsmInfo.cpp
  TISCMCCodeEmitter.cpp
  TISCMCTargetDesc.cpp

  LINK_COMPONENTS
  MC
  TISCInfo
  Support

  ADD_TO_COMPONENT
  TISC
  )
