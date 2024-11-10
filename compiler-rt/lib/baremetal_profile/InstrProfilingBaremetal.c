/*===- InstrProfilingBaremetal.c - profile name variable setup  -------------===*\
|*
|* Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
|* See https://llvm.org/LICENSE.txt for license information.
|* SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
|*
\*===----------------------------------------------------------------------===*/

#include "InstrProfilingBaremetal.h"
#include "InstrProfilingInternal.h"


fwrite_type *fwrite_ptr = 0;
fseek_type *fseek_ptr = 0;

void* memcpy(void* dest, const void* source, size_t num) {
    char* d = (char*)dest;
    const char* s = (const char*)source;
    for (size_t i = 0; i < num; ++i) {
        d[i] = s[i];
    }
    return dest;
}

void* memset(void* dest, const int val, size_t num) {
    unsigned char* d = (unsigned char*)dest;
    for (size_t i = 0; i < num; ++i) {
        d[i] = (unsigned char)val;
    }
    return dest;
}
#if !(COMPILER_RT_BUILD_BUILTINS)
uint64_t __umoddi3(uint64_t a, uint64_t b) {
    return a % b;
}
#endif

uint32_t baremetal_fileWriter(ProfDataWriter *This, ProfDataIOVec *IOVecs, uint32_t NumIOVecs) {
  uint32_t I;
  FILE *File = (FILE *)This->WriterCtx;
  char Zeroes[sizeof(uint64_t)] = {0};
  while (fseek_ptr==0 || fseek_ptr==0);
  for (I = 0; I < NumIOVecs; I++) {
    if (IOVecs[I].Data) {
      if ((*fwrite_ptr)(IOVecs[I].Data, IOVecs[I].ElmSize, IOVecs[I].NumElm, File) !=
          IOVecs[I].NumElm)
        return 1;
    } else if (IOVecs[I].UseZeroPadding) {
      size_t BytesToWrite = IOVecs[I].ElmSize * IOVecs[I].NumElm;
      while (BytesToWrite > 0) {
        size_t PartialWriteLen =
            (sizeof(uint64_t) > BytesToWrite) ? BytesToWrite : sizeof(uint64_t);
        if ((*fwrite_ptr)(Zeroes, sizeof(uint8_t), PartialWriteLen, File) !=
            PartialWriteLen) {
          return 1;
        }
        BytesToWrite -= PartialWriteLen;
      }
    } else {
      if ((*fseek_ptr)(File, IOVecs[I].ElmSize * IOVecs[I].NumElm, SEEK_CUR) == -1)
        return 1;
    }
  }
  return 0;
}


