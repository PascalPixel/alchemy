#ifndef ALCHEMY_ROM_HEADER_H
#define ALCHEMY_ROM_HEADER_H

// The two places of the cartridge header the code reads: the cartridge's start and the language
// letter of its game code.

#include "types.h"

// The cartridge's first instruction, where a reset jumps; as The Broken Seal's.
#define CartStart        FIXED_ADDRESS((Uint32), 0x08000000, RomStart)

// The last letter of the game code in the cartridge header (rom_header.s): J, E, D, F, I or S,
// which the German, French, Italian and Spanish editions read to tell which they are.
#define CartLanguageCode (*FIXED_ADDRESS((const Uint8 *), 0x080000af, RomLanguageCode))

#endif
