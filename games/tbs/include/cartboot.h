#ifndef ALCHEMY_CARTBOOT_H
#define ALCHEMY_CARTBOOT_H

/// @file
/// Cart_Restart, the cartridge's restart entry at 0x08000000.

#include "types.h"

#define Cart_Restart FIXED_ADDRESS((u32), 0x08000000, Rom_Start)

#endif
