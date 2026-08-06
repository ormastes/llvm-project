//===--- SimpleOS.cpp - SimpleOS ToolChain Implementations ----------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "SimpleOS.h"
#include "CommonArgs.h"
#include "clang/Driver/Compilation.h"
#include "clang/Driver/Driver.h"
#include "clang/Driver/DriverDiagnostic.h"
#include "clang/Driver/Options.h"
#include "llvm/Option/ArgList.h"
#include "llvm/Support/Path.h"

using namespace clang::driver;
using namespace clang::driver::toolchains;
using namespace clang::driver::tools;
using namespace clang;
using namespace llvm::opt;

static std::string getSimpleOSSysRoot(const ToolChain &TC,
                                      const ArgList &Args) {
  if (const Arg *A = Args.getLastArg(options::OPT__sysroot_EQ))
    return A->getValue();
  if (!TC.getDriver().SysRoot.empty())
    return TC.getDriver().SysRoot;

  SmallString<128> SysRoot(TC.getDriver().Dir);
  llvm::sys::path::remove_filename(SysRoot); // bin
  llvm::sys::path::remove_filename(SysRoot); // cross-<triple>
  llvm::sys::path::remove_filename(SysRoot); // llvm
  llvm::sys::path::append(SysRoot, "sysroot");
  return std::string(SysRoot);
}

static StringRef getSimpleOSCompilerRTArchName(const llvm::Triple &Triple) {
  switch (Triple.getArch()) {
  case llvm::Triple::x86_64:
    return "x86_64";
  case llvm::Triple::aarch64:
    return "aarch64";
  case llvm::Triple::arm:
    return "arm";
  case llvm::Triple::riscv64:
    return "riscv64";
  case llvm::Triple::riscv32:
    return "riscv32";
  default:
    return Triple.getArchName();
  }
}

/// SimpleOS Linker tool.
namespace {
class LLVM_LIBRARY_VISIBILITY Linker : public Tool {
public:
  Linker(const ToolChain &TC) : Tool("SimpleOS::Linker", "ld.lld", TC) {}

  bool hasIntegratedCPP() const override { return false; }
  bool isLinkJob() const override { return true; }

  void ConstructJob(Compilation &C, const JobAction &JA,
                    const InputInfo &Output, const InputInfoList &Inputs,
                    const ArgList &Args,
                    const char *LinkingOutput) const override;
};
} // namespace

void Linker::ConstructJob(Compilation &C, const JobAction &JA,
                          const InputInfo &Output, const InputInfoList &Inputs,
                          const ArgList &Args,
                          const char *LinkingOutput) const {
  const ToolChain &TC = getToolChain();
  ArgStringList CmdArgs;

  // Use lld as the linker.
  CmdArgs.push_back("-flavor");
  CmdArgs.push_back("gnu");

  // Output file.
  CmdArgs.push_back("-o");
  CmdArgs.push_back(Output.getFilename());

  std::string SysRoot = getSimpleOSSysRoot(TC, Args);
  if (!SysRoot.empty()) {
    CmdArgs.push_back("--sysroot");
    CmdArgs.push_back(Args.MakeArgString(SysRoot));

    SmallString<128> LibDir(SysRoot);
    llvm::sys::path::append(LibDir, "lib");
    CmdArgs.push_back("-L");
    CmdArgs.push_back(Args.MakeArgString(LibDir));

    SmallString<128> BuiltinsDir(SysRoot);
    llvm::sys::path::append(BuiltinsDir, "lib", "clang", "20", "lib");
    llvm::sys::path::append(BuiltinsDir, TC.getTriple().str());
    CmdArgs.push_back("-L");
    CmdArgs.push_back(Args.MakeArgString(BuiltinsDir));

    // Linker script from sysroot.
    SmallString<128> LDS(SysRoot);
    llvm::sys::path::append(LDS, "share", "simpleos", "simpleos.ld");
    if (llvm::sys::fs::exists(LDS)) {
      CmdArgs.push_back("-T");
      CmdArgs.push_back(Args.MakeArgString(LDS));
    }

    // crt0 object.
    SmallString<128> CRT0(SysRoot);
    llvm::sys::path::append(CRT0, "lib", "crt0.o");
    if (llvm::sys::fs::exists(CRT0))
      CmdArgs.push_back(Args.MakeArgString(CRT0));
  }

  // Add all input objects.
  AddLinkerInputs(TC, Inputs, Args, CmdArgs, JA);

  // Runtime libraries.
  CmdArgs.push_back("-lsimpleos_c");
  CmdArgs.push_back(Args.MakeArgString(Twine("-lclang_rt.builtins-") +
      getSimpleOSCompilerRTArchName(TC.getTriple())));

  const char *Exec = Args.MakeArgString(TC.GetLinkerPath());
  C.addCommand(std::make_unique<Command>(JA, *this,
                                         ResponseFileSupport::AtFileCurCP(),
                                         Exec, CmdArgs, Inputs, Output));
}

//===----------------------------------------------------------------------===//
// SimpleOS ToolChain
//===----------------------------------------------------------------------===//

SimpleOS::SimpleOS(const Driver &D, const llvm::Triple &Triple,
                   const ArgList &Args)
    : Generic_ELF(D, Triple, Args) {}

void SimpleOS::AddClangSystemIncludeArgs(const ArgList &DriverArgs,
                                         ArgStringList &CC1Args) const {
  if (DriverArgs.hasArg(options::OPT_nostdinc) ||
      DriverArgs.hasArg(options::OPT_nostdlibinc))
    return;

  SmallString<128> Inc(getSimpleOSSysRoot(*this, DriverArgs));
  llvm::sys::path::append(Inc, "include");
  addSystemInclude(DriverArgs, CC1Args, Inc);
}

void SimpleOS::AddClangCXXStdlibIncludeArgs(const ArgList &DriverArgs,
                                             ArgStringList &CC1Args) const {
  if (DriverArgs.hasArg(options::OPT_nostdinc) ||
      DriverArgs.hasArg(options::OPT_nostdincxx))
    return;

  SmallString<128> Inc(getSimpleOSSysRoot(*this, DriverArgs));
  llvm::sys::path::append(Inc, "include", "c++", "v1");
  addSystemInclude(DriverArgs, CC1Args, Inc);
}

std::string SimpleOS::getCompilerRT(const ArgList &Args, StringRef Component,
                                    FileType Type) const {
  SmallString<128> Path(getDriver().ResourceDir);
  llvm::sys::path::append(Path, "lib", getTriple().str());
  const char *Suffix = (Type == ToolChain::FT_Shared) ? ".so" : ".a";
  llvm::sys::path::append(Path, Twine("libclang_rt.") + Component + Suffix);
  return std::string(Path);
}

Tool *SimpleOS::buildLinker() const {
  return new Linker(*this);
}
