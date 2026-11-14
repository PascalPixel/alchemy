/// @file
/// The flash library's version and the chips IdentifyFlash knows.

#include "types.h"
#include "gamepak/flash_data.h"

const char Flash_Version[] = "FLASH_V123";

const struct FlashSetupInfo *const gFlash_Chips[] = {
    &Flash_ChipSst,       &gFlash_ChipAtmel,  &Flash_ChipMacronix,
    &Flash_ChipPanasonic, &gFlash_ChipDefault,
};
