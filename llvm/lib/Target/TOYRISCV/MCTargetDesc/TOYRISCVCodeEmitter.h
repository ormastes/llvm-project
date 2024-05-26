#if !defined TOYRISCVCODEEMITTER_H_INCLUDED
#define TOYRISCVCODEEMITTER_H_INCLUDED

#include "llvm/MC/MCCodeEmitter.h"
#include "llvm/Support/DataTypes.h"

namespace llvm {
class MCInstrInfo;
class MCContext;
class MCFixup;
class raw_ostream;

class TOYRISCVMCCodeEmitter : public MCCodeEmitter {
  MCInstrInfo const &MCII;
  MCContext &Ctx;
  bool IsLittleEndian;

public:
  TOYRISCVMCCodeEmitter(MCInstrInfo const &MCII, MCContext &Ctx, bool IsLittle);

  void encodeInstruction(const MCInst &MI,
                                           SmallVectorImpl<char> &CB,
                                           SmallVectorImpl<MCFixup> &Fixups,
                                           const MCSubtargetInfo &STI) const;
};

} // namespace llvm

#endif // TOYRISCVCODEEMITTER_H_INCLUDED

// vim: set ts=2 sw=2 sts=2:
