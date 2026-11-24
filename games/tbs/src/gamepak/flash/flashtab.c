/// @file
/// The flash library's version and the chips IdentifyFlash knows.

#include "types.h"
#include "gamepak/flash_data.h"

/// The flash routines' name and version, kept in the ROM; nothing reads it.
const char gFlash_Version[] = "FLASH_V123";

/// The chips IdentifyFlash tries, in order; the last, with id 0, ends the list.
const struct FlashSetupInfo *const gFlash_Chips[] = {
    &gFlash_ChipSst,       &gFlash_ChipAtmel,   &gFlash_ChipMacronix,
    &gFlash_ChipPanasonic, &gFlash_ChipDefault,
};
