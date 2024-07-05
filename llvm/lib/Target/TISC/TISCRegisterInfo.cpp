//===-- TISCRegisterInfo.cpp - TISC Register Information --------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains the TISC implementation of the TargetRegisterInfo class.
//
//===----------------------------------------------------------------------===//

#include "TISCRegisterInfo.h"
#include "TISC.h"
#include "TISCMachineFunctionInfo.h"
#include "TISCTargetMachine.h"
#include "llvm/ADT/BitVector.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"

using namespace llvm;

#define DEBUG_TYPE "tisc-reg-info"

#define GET_REGINFO_TARGET_DESC
#include "TISCGenRegisterInfo.inc"

// FIXME: Provide proper call frame setup / destroy opcodes.
TISCRegisterInfo::TISCRegisterInfo()
  : TISCGenRegisterInfo(TISC::PC) {}

const MCPhysReg*
TISCRegisterInfo::getCalleeSavedRegs(const MachineFunction *MF) const {
  const TISCFrameLowering *TFI = getFrameLowering(*MF);
  const Function* F = &MF->getFunction();
  static const MCPhysReg CalleeSavedRegs[] = {
    TISC::R4, TISC::R5, TISC::R6, TISC::R7,
    TISC::R8, TISC::R9, TISC::R10,
    0
  };
  static const MCPhysReg CalleeSavedRegsFP[] = {
    TISC::R5, TISC::R6, TISC::R7,
    TISC::R8, TISC::R9, TISC::R10,
    0
  };
  static const MCPhysReg CalleeSavedRegsIntr[] = {
    TISC::R4,  TISC::R5,  TISC::R6,  TISC::R7,
    TISC::R8,  TISC::R9,  TISC::R10, TISC::R11,
    TISC::R12, TISC::R13, TISC::R14, TISC::R15,
    0
  };
  static const MCPhysReg CalleeSavedRegsIntrFP[] = {
    TISC::R5,  TISC::R6,  TISC::R7,
    TISC::R8,  TISC::R9,  TISC::R10, TISC::R11,
    TISC::R12, TISC::R13, TISC::R14, TISC::R15,
    0
  };

  if (TFI->hasFP(*MF))
    return (F->getCallingConv() == CallingConv::TISC_INTR ?
            CalleeSavedRegsIntrFP : CalleeSavedRegsFP);
  else
    return (F->getCallingConv() == CallingConv::TISC_INTR ?
            CalleeSavedRegsIntr : CalleeSavedRegs);

}

BitVector TISCRegisterInfo::getReservedRegs(const MachineFunction &MF) const {
  BitVector Reserved(getNumRegs());
  const TISCFrameLowering *TFI = getFrameLowering(MF);

  // Mark 4 special registers with subregisters as reserved.
  Reserved.set(TISC::PCB);
  Reserved.set(TISC::SPB);
  Reserved.set(TISC::SRB);
  Reserved.set(TISC::CGB);
  Reserved.set(TISC::PC);
  Reserved.set(TISC::SP);
  Reserved.set(TISC::SR);
  Reserved.set(TISC::CG);

  // Mark frame pointer as reserved if needed.
  if (TFI->hasFP(MF)) {
    Reserved.set(TISC::R4B);
    Reserved.set(TISC::R4);
  }

  return Reserved;
}

const TargetRegisterClass *
TISCRegisterInfo::getPointerRegClass(const MachineFunction &MF, unsigned Kind)
                                                                         const {
  return &TISC::GR16RegClass;
}

bool
TISCRegisterInfo::eliminateFrameIndex(MachineBasicBlock::iterator II,
                                        int SPAdj, unsigned FIOperandNum,
                                        RegScavenger *RS) const {
  assert(SPAdj == 0 && "Unexpected");

  MachineInstr &MI = *II;
  MachineBasicBlock &MBB = *MI.getParent();
  MachineFunction &MF = *MBB.getParent();
  const TISCFrameLowering *TFI = getFrameLowering(MF);
  DebugLoc dl = MI.getDebugLoc();
  int FrameIndex = MI.getOperand(FIOperandNum).getIndex();

  unsigned BasePtr = (TFI->hasFP(MF) ? TISC::R4 : TISC::SP);
  int Offset = MF.getFrameInfo().getObjectOffset(FrameIndex);

  // Skip the saved PC
  Offset += 2;

  if (!TFI->hasFP(MF))
    Offset += MF.getFrameInfo().getStackSize();
  else
    Offset += 2; // Skip the saved FP

  // Fold imm into offset
  Offset += MI.getOperand(FIOperandNum + 1).getImm();

  if (MI.getOpcode() == TISC::ADDframe) {
    // This is actually "load effective address" of the stack slot
    // instruction. We have only two-address instructions, thus we need to
    // expand it into mov + add
    const TargetInstrInfo &TII = *MF.getSubtarget().getInstrInfo();

    MI.setDesc(TII.get(TISC::MOV16rr));
    MI.getOperand(FIOperandNum).ChangeToRegister(BasePtr, false);

    // Remove the now unused Offset operand.
    MI.removeOperand(FIOperandNum + 1);

    if (Offset == 0)
      return false;

    // We need to materialize the offset via add instruction.
    Register DstReg = MI.getOperand(0).getReg();
    if (Offset < 0)
      BuildMI(MBB, std::next(II), dl, TII.get(TISC::SUB16ri), DstReg)
        .addReg(DstReg).addImm(-Offset);
    else
      BuildMI(MBB, std::next(II), dl, TII.get(TISC::ADD16ri), DstReg)
        .addReg(DstReg).addImm(Offset);

    return false;
  }

  MI.getOperand(FIOperandNum).ChangeToRegister(BasePtr, false);
  MI.getOperand(FIOperandNum + 1).ChangeToImmediate(Offset);
  return false;
}

Register TISCRegisterInfo::getFrameRegister(const MachineFunction &MF) const {
  const TISCFrameLowering *TFI = getFrameLowering(MF);
  return TFI->hasFP(MF) ? TISC::R4 : TISC::SP;
}
