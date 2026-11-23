#ifndef ALCHEMY_ROM_HEADER_H
#define ALCHEMY_ROM_HEADER_H

/// @file
/// The start of the cartridge, where the console begins the game: a soft
/// reset jumps back to it.

#include "types.h"

/// The cartridge's first instruction, the jump past the ROM header.
#define Cart_Start FIXED_ADDRESS((u32), 0x08000000, Rom_Start)
/// The cartridge's first byte, as the link places it (rom_header.s).
extern const u8 Rom_Start[];

#endif
