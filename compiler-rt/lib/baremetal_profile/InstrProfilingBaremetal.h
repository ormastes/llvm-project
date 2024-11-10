/*===- InstrProfilingBaremeta.h - Support library for PGO instrumentation -----===*\
|*
|* Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
|* See https://llvm.org/LICENSE.txt for license information.
|* SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
|*
\*===----------------------------------------------------------------------===*/

#ifndef PROFILE_INSTRPROFILEBAREMETAL_H
#define PROFILE_INSTRPROFILEBAREMETAL_H

#include <limits.h>
#include <stdint.h>
#include <stddef.h>

#define FILE int

struct ProfDataWriter;
struct ProfDataIOVec;
#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

typedef struct MEM_FILE {
    size_t current_loc;
    size_t max;  // Maximum size of the buffer
    char* buffer;
} MEM_FILE;

void* memcpy (void* dest, const void* source, size_t num);
void* memset (void* dest, const int val, size_t num);
#if !(COMPILER_RT_BUILD_BUILTINS)
uint64_t __umoddi3 (uint64_t a, uint64_t b);
#endif

typedef size_t fwrite_type(const void *ptr, size_t size, size_t nmemb, FILE *stream);
typedef int fseek_type(FILE *stream, long offset, int whence);

extern fwrite_type *fwrite_ptr;
extern fseek_type *fseek_ptr;

uint32_t baremetal_fileWriter(struct ProfDataWriter *This, struct ProfDataIOVec *IOVecs, uint32_t NumIOVecs);


#endif /* PROFILE_INSTRPROFILEBAREMETAL_H */
