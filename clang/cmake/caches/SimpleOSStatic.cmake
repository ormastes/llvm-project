# SimpleOSStatic.cmake — CMake initial-cache for a statically-linked clang
# targeting x86_64-unknown-simpleos.
#
# Usage:
#   cmake -S /path/to/llvm-project/llvm \
#         -B build/os/clang_static \
#         -C /path/to/llvm-project/clang/cmake/caches/SimpleOSStatic.cmake \
#         -G Ninja
#
# Requires the ormastes/llvm-project:simpleos fork (commit 3b33ba807+).
# DO NOT use this cache with upstream LLVM — the simpleos ToolChain class
# and __simpleos__ predefines live only in that fork.

# ---------------------------------------------------------------------------
# Target triple
# ---------------------------------------------------------------------------
set(LLVM_DEFAULT_TARGET_TRIPLE "x86_64-unknown-simpleos" CACHE STRING "" FORCE)
set(LLVM_HOST_TRIPLE           "x86_64-unknown-linux-gnu" CACHE STRING "" FORCE)

# ---------------------------------------------------------------------------
# Projects / Runtimes
# ---------------------------------------------------------------------------
set(LLVM_ENABLE_PROJECTS "clang;lld" CACHE STRING "" FORCE)
# compiler-rt builtins for SimpleOS are built separately via LLVM_ENABLE_RUNTIMES
# once a sysroot is available; skip here to keep the cache self-contained.
set(LLVM_ENABLE_RUNTIMES "" CACHE STRING "" FORCE)

# ---------------------------------------------------------------------------
# Targets
# ---------------------------------------------------------------------------
set(LLVM_TARGETS_TO_BUILD "X86" CACHE STRING "" FORCE)

# ---------------------------------------------------------------------------
# Static linking
# ---------------------------------------------------------------------------
# Force fully static ELF: no libstdc++.so / libc++.so dependencies.
set(LLVM_BUILD_STATIC           ON  CACHE BOOL "" FORCE)
set(LLVM_STATIC_LINK_CXX_STDLIB ON  CACHE BOOL "" FORCE)
set(LLVM_LINK_LLVM_DYLIB        OFF CACHE BOOL "" FORCE)
set(LLVM_BUILD_LLVM_DYLIB       OFF CACHE BOOL "" FORCE)
set(LLVM_ENABLE_LIBCXX          OFF CACHE BOOL "" FORCE)
set(LIBCLANG_BUILD_STATIC       ON  CACHE BOOL "" FORCE)

# Linker flags: fully static ELF (no PIE to reduce startup overhead in SimpleOS)
set(CMAKE_EXE_LINKER_FLAGS "-static" CACHE STRING "" FORCE)

# ---------------------------------------------------------------------------
# Linker / Toolchain
# ---------------------------------------------------------------------------
set(CLANG_DEFAULT_LINKER "lld"     CACHE STRING "" FORCE)
set(LLVM_ENABLE_LLD      ON        CACHE BOOL   "" FORCE)

# ---------------------------------------------------------------------------
# Build type — size-optimised release
# ---------------------------------------------------------------------------
set(CMAKE_BUILD_TYPE          "Release"        CACHE STRING "" FORCE)
set(CMAKE_C_FLAGS_RELEASE     "-Os -DNDEBUG"   CACHE STRING "" FORCE)
set(CMAKE_CXX_FLAGS_RELEASE   "-Os -DNDEBUG"   CACHE STRING "" FORCE)

# ThinLTO: shaves binary size at link time; requires lld (set above).
set(LLVM_ENABLE_LTO "Thin" CACHE STRING "" FORCE)

# ---------------------------------------------------------------------------
# Disable non-essential tools (keep clang + lld only)
# ---------------------------------------------------------------------------
# LLVM_BUILD_TOOLS governs the llvm/tools/ sub-projects (llc, opt, llvm-as, …).
# Setting it OFF does NOT disable clang or lld (they are ENABLE_PROJECTS entries).
set(LLVM_BUILD_TOOLS   OFF CACHE BOOL "" FORCE)
set(LLVM_INCLUDE_TOOLS ON  CACHE BOOL "" FORCE)   # keep so clang/lld targets exist

# Trim unused utilities further
set(LLVM_INCLUDE_BENCHMARKS OFF CACHE BOOL "" FORCE)
set(LLVM_INCLUDE_EXAMPLES   OFF CACHE BOOL "" FORCE)
set(LLVM_INCLUDE_TESTS      OFF CACHE BOOL "" FORCE)
set(LLVM_BUILD_TESTS        OFF CACHE BOOL "" FORCE)
set(LLVM_INCLUDE_DOCS       OFF CACHE BOOL "" FORCE)
set(LLVM_BUILD_DOCS         OFF CACHE BOOL "" FORCE)
set(CLANG_INCLUDE_DOCS      OFF CACHE BOOL "" FORCE)
set(CLANG_INCLUDE_TESTS     OFF CACHE BOOL "" FORCE)

# Disable features that pull in large optional deps
set(LLVM_ENABLE_TERMINFO    OFF CACHE BOOL "" FORCE)
set(LLVM_ENABLE_ZLIB        OFF CACHE BOOL "" FORCE)
set(LLVM_ENABLE_ZSTD        OFF CACHE BOOL "" FORCE)
set(LLVM_ENABLE_CURL        OFF CACHE BOOL "" FORCE)
set(LLVM_ENABLE_HTTPLIB     OFF CACHE BOOL "" FORCE)
set(LLVM_ENABLE_LIBXML2     OFF CACHE BOOL "" FORCE)
set(LLVM_ENABLE_BINDINGS    OFF CACHE BOOL "" FORCE)
set(LLVM_ENABLE_OCAMLDOC    OFF CACHE BOOL "" FORCE)

# LLDB / MLIR / polly — not needed for the compiler deliverable
set(LLDB_ENABLE_PYTHON  OFF CACHE BOOL "" FORCE)
