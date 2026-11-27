// The Atmel chip's settings: as the other chips see it, in 4 KB sectors, and as its own functions
// work, in 128-byte ones.

#include "types.h"
#include "sdk/flash.h"
#include "sdk/flash_data.h"

// The id the Atmel chip answers with: its device id above its maker id.
#define FLASH_ID_ATMEL (0x3d << 8 | 0x1f)

// clang-format off
// The Atmel chip's time limits in milliseconds, by enum FlashPhase (see FlashSetupInfo.maxTime): 40 for 128 programmed bytes and for the whole chip. Its sectors are erased by programming them, so that row is empty.
const u16 Flash_MaxTimeAtmel[] = {
	10, FLASH_TIMER_RELOAD, FLASH_TIMER_CONTROL,
	40, FLASH_TIMER_RELOAD, FLASH_TIMER_CONTROL,
	0, 0, 0,
	40, FLASH_TIMER_RELOAD, FLASH_TIMER_CONTROL,
};
// clang-format on

// The Atmel chip as Flash_Chips lists it, in 4 KB sectors.
const struct FlashSetupInfo Flash_ChipAtmel = {
	.program     = ProgramAtmelFlashBlock,
	.eraseChip   = EraseAtmelFlashChip,
	.eraseSector = EraseAtmelFlashBlock,
	.wait        = WaitForFlashWrite,
	.maxTime     = (u16 *)Flash_MaxTimeAtmel,
	.type        = {.romSize = 65536,
                    .sector  = {.size = 4096, .shift = 12, .count = 16, .top = 0},
                    .wait    = {3, 3},
                    .id      = {.joined = FLASH_ID_ATMEL}},
};

// The Atmel chip in its own 128-byte sectors, which its functions work in.
const struct FlashSetupInfo Flash_ChipAtmelLayout = {
	// ProgramAtmelFlashSector takes its sector as a word and its source as const, unlike the slot's
	// type.
	.program     = (u16 (*)(u16, u8 *))ProgramAtmelFlashSector,
	.eraseChip   = EraseAtmelFlashChip,
	.eraseSector = EraseAtmelFlashSector,
	.wait        = WaitForFlashWrite,
	.maxTime     = (u16 *)Flash_MaxTimeAtmel,
	.type        = {.romSize = 65536,
                    .sector  = {.size = 128, .shift = 7, .count = 512, .top = 0},
                    .wait    = {3, 3},
                    .id      = {.joined = FLASH_ID_ATMEL}},
};
