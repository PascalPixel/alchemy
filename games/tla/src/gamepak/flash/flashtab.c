/// @file
/// The flash library's version and the chips IdentifyFlash knows.

#include "types.h"
#include "gamepak/flash_data.h"

/// The library's name and version, which nothing reads.
const char Flash_Version[] = "FLASH_V123";

/// The chips IdentifyFlash knows, by id; the last is taken for any other.
const struct FlashSetupInfo *const gFlash_Chips[] = {
    &Flash_ChipSst,       &gFlash_ChipAtmel,   &Flash_ChipMacronix,
    &Flash_ChipPanasonic, &gFlash_ChipDefault,
};
