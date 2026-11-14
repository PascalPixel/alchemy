#ifndef ALCHEMY_ROM_HEADER_H
#define ALCHEMY_ROM_HEADER_H

#include "types.h"

/// Cart_Start, the cartridge's restart entry at 0x08000000, as The Broken
/// Seal's.
#define Cart_Start FIXED_ADDRESS((u32), 0x08000000, Rom_Start)

/// The last letter of the game code in the cartridge header (rom_header.s):
/// J, E, D, F, I or S. The tune builds this address in a register rather than
/// reading it from a literal pool, so it is written as a fixed address.
#define Cart_LanguageCode                                                      \
  (*FIXED_ADDRESS((const u8 *), 0x080000af, gRom_LanguageCode))

#endif
