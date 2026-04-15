# CompilerRTSimpleOS.cmake
# CMake support for building compiler-rt builtins targeting
# x86_64-unknown-simpleos (freestanding, no pthread, no dlopen).
#
# Included automatically by lib/builtins/CMakeLists.txt when
# CMAKE_SYSTEM_NAME == "SimpleOS".  Do NOT include directly.

# Ensure the target triple is set correctly for the cross build.
if(NOT COMPILER_RT_DEFAULT_TARGET_TRIPLE MATCHES "simpleos")
  message(WARNING
    "CompilerRTSimpleOS: CMAKE_SYSTEM_NAME is SimpleOS but "
    "COMPILER_RT_DEFAULT_TARGET_TRIPLE ('${COMPILER_RT_DEFAULT_TARGET_TRIPLE}') "
    "does not contain 'simpleos'. Proceeding anyway.")
endif()

# Freestanding build: disable OS-level features that compiler-rt would
# otherwise try to use on a hosted system.
set(COMPILER_RT_BAREMETAL_BUILD ON CACHE BOOL
    "SimpleOS builtins are freestanding (no libc, no pthreads)" FORCE)

# No dynamic linking on SimpleOS kernel side.
set(COMPILER_RT_EXCLUDE_ATOMIC_BUILTIN ON CACHE BOOL
    "Exclude the shared-library atomic builtin for SimpleOS" FORCE)

# Output library name follows the standard compiler-rt convention:
#   libclang_rt.builtins-x86_64.a
# This is handled automatically by add_compiler_rt_runtime() given ARCHS x86_64.
message(STATUS "CompilerRTSimpleOS: configuring builtins for x86_64-unknown-simpleos (freestanding)")
