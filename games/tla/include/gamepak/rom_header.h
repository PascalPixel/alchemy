#ifndef ALCHEMY_ROM_HEADER_H
#define ALCHEMY_ROM_HEADER_H

/// @file
/// The two places of the cartridge header the code reads: the cartridge's start
/// and the language letter of its game code.

#include "types.h"

/// The cartridge's first instruction, where a reset jumps; as The Broken
/// Seal's.
#define Cart_Start FIXED_ADDRESS((u32), 0x08000000, Rom_Start)

/// The last letter of the game code in the cartridge header (rom_header.s): J,
/// E, D, F, I or S, which the German, French, Italian and Spanish editions
/// read to tell which they are.
#define Cart_LanguageCode                                                      \
  (*FIXED_ADDRESS((const u8 *), 0x080000af, gRom_LanguageCode))

#endif
