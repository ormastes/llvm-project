add_llvm_component_library(LLVMTOYMSP43_Desc
  MSP430AsmBackend.cpp
  MSP430ELFObjectWriter.cpp
  MSP430ELFStreamer.cpp
  MSP430InstPrinter.cpp
  MSP430MCAsmInfo.cpp
  MSP430MCCodeEmitter.cpp
  MSP430MCTargetDesc.cpp

  LINK_COMPONENTS
  MC
  TOYMSP43_Info
  Support

  ADD_TO_COMPONENT
  TOYMSP43_
  )
