// The version of the SDK's flash library and the chips IdentifyFlash knows.

#include "types.h"
#include "sdk/flash_data.h"

// The flash functions' name and version, kept in the ROM; nothing reads it.
const char                         Flash_Version[] = "FLASH_V123";

// The chips IdentifyFlash tries, in order; the last, with id 0, ends the list.
const struct FlashSetupInfo *const Flash_Chips[]   = {
    &Flash_ChipSst,
    &Flash_ChipAtmel,
    &Flash_ChipMacronix,
    &Flash_ChipPanasonic,
    &Flash_ChipDefault,
};
