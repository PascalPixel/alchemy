#ifndef ALCHEMY_CARTBOOT_H
#define ALCHEMY_CARTBOOT_H

/// @file
/// Cart_Start, the cartridge's restart entry at 0x08000000.

#include "types.h"

/// The cartridge's first instruction. The soft reset in WaitTicks jumps
/// here, and SerialTest_Run, a link cable test nothing reaches, sends the
/// first 640 bytes from here.
#define Cart_Start FIXED_ADDRESS((u32), 0x08000000, Rom_Start)

#endif
