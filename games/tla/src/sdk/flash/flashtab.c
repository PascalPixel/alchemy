/* @file */
/* The SDK's flash library's version and the chips IdentifyFlash knows. */

#include "types.h"
#include "sdk/flash_data.h"

/* The SDK's flash library's name and version, which nothing reads. */
const char Flash_Version[] = "FLASH_V123";

/* The chips IdentifyFlash knows, by id; the last is taken for any other. */
const struct FlashSetupInfo *const Flash_Chips[] = {
	&Flash_ChipSst,		  &Flash_ChipAtmel,	  &Flash_ChipMacronix,
	&Flash_ChipPanasonic, &Flash_ChipDefault,
};
