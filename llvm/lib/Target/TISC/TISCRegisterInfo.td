//===-- TISCRegisterInfo.td - TISC Register defs -------*- tablegen -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
//  Declarations that describe the TISC register file
//===----------------------------------------------------------------------===//

class TISCReg<bits<4> num, string n, list<string> alt = []> : Register<n> {
  field bits<4> Num = num;
  let Namespace = "TISC";
  let HWEncoding{3-0} = num;
  let AltNames = alt;
  let DwarfNumbers = [num];
}

class TISCRegWithSubregs<bits<4> num, string n, list<Register> subregs,
                           list<string> alt = []> 
  : RegisterWithSubRegs<n, subregs> {
  field bits<4> Num = num;
  let Namespace = "TISC";
  let HWEncoding{3-0} = num;
  let AltNames = alt;
  let DwarfNumbers = [num];
}

//===----------------------------------------------------------------------===//
//  Registers
//===----------------------------------------------------------------------===//

def PCB  : TISCReg<0,  "r0", ["pc"]>, DwarfRegNum<[16]>;
def SPB  : TISCReg<1,  "r1", ["sp"]>, DwarfRegNum<[17]>;
def SRB  : TISCReg<2,  "r2", ["sr"]>, DwarfRegNum<[18]>;
def CGB  : TISCReg<3,  "r3", ["cg"]>, DwarfRegNum<[19]>;
def R4B  : TISCReg<4,  "r4", ["fp"]>, DwarfRegNum<[20]>;
def R5B  : TISCReg<5,  "r5">,  DwarfRegNum<[21]>;
def R6B  : TISCReg<6,  "r6">,  DwarfRegNum<[22]>;
def R7B  : TISCReg<7,  "r7">,  DwarfRegNum<[23]>;
def R8B  : TISCReg<8,  "r8">,  DwarfRegNum<[24]>;
def R9B  : TISCReg<9,  "r9">,  DwarfRegNum<[25]>;
def R10B : TISCReg<10, "r10">, DwarfRegNum<[26]>;
def R11B : TISCReg<11, "r11">, DwarfRegNum<[27]>;
def R12B : TISCReg<12, "r12">, DwarfRegNum<[28]>;
def R13B : TISCReg<13, "r13">, DwarfRegNum<[29]>;
def R14B : TISCReg<14, "r14">, DwarfRegNum<[30]>;
def R15B : TISCReg<15, "r15">, DwarfRegNum<[31]>;

def subreg_8bit : SubRegIndex<8> { let Namespace = "TISC"; }

let SubRegIndices = [subreg_8bit] in {
def PC  : TISCRegWithSubregs<0,  "r0",  [PCB], ["pc"]>, DwarfRegNum<[0]>;
def SP  : TISCRegWithSubregs<1,  "r1",  [SPB], ["sp"]>, DwarfRegNum<[1]>;
def SR  : TISCRegWithSubregs<2,  "r2",  [SRB], ["sr"]>, DwarfRegNum<[2]>;
def CG  : TISCRegWithSubregs<3,  "r3",  [CGB], ["cg"]>, DwarfRegNum<[3]>;
def R4  : TISCRegWithSubregs<4,  "r4",  [R4B], ["fp"]>, DwarfRegNum<[4]>;
def R5  : TISCRegWithSubregs<5,  "r5",  [R5B]>,  DwarfRegNum<[5]>;
def R6  : TISCRegWithSubregs<6,  "r6",  [R6B]>,  DwarfRegNum<[6]>;
def R7  : TISCRegWithSubregs<7,  "r7",  [R7B]>,  DwarfRegNum<[7]>;
def R8  : TISCRegWithSubregs<8,  "r8",  [R8B]>,  DwarfRegNum<[8]>;
def R9  : TISCRegWithSubregs<9,  "r9",  [R9B]>,  DwarfRegNum<[9]>;
def R10 : TISCRegWithSubregs<10, "r10", [R10B]>, DwarfRegNum<[10]>;
def R11 : TISCRegWithSubregs<11, "r11", [R11B]>, DwarfRegNum<[11]>;
def R12 : TISCRegWithSubregs<12, "r12", [R12B]>, DwarfRegNum<[12]>;
def R13 : TISCRegWithSubregs<13, "r13", [R13B]>, DwarfRegNum<[13]>;
def R14 : TISCRegWithSubregs<14, "r14", [R14B]>, DwarfRegNum<[14]>;
def R15 : TISCRegWithSubregs<15, "r15", [R15B]>, DwarfRegNum<[15]>;
}

def GR8 : RegisterClass<"TISC", [i8], 8,
   // Volatile registers
  (add R12B, R13B, R14B, R15B, R11B, R10B, R9B, R8B, R7B, R6B, R5B,
   // Frame pointer, sometimes allocable
   R4B,
   // Volatile, but not allocable
   PCB, SPB, SRB, CGB)>;

def GR16 : RegisterClass<"TISC", [i16], 16,
   // Volatile registers
  (add R12, R13, R14, R15, R11, R10, R9, R8, R7, R6, R5,
   // Frame pointer, sometimes allocable
   R4,
   // Volatile, but not allocable
   PC, SP, SR, CG)>;
