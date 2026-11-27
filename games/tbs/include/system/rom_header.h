#ifndef ALCHEMY_ROM_HEADER_H
#define ALCHEMY_ROM_HEADER_H

// The start of the cartridge, where the console begins the game: a soft reset jumps back to it.

#include "types.h"

// The cartridge's first instruction, the jump past the ROM header.
#define CartStart FIXED_ADDRESS((Uint32), 0x08000000, RomStart)
// The cartridge's first byte, as the link places it (rom_header.s).
extern const Uint8 RomStart[];

#endif
