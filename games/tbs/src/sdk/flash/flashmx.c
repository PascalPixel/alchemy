/* @file */
/* The Macronix and Panasonic chips' settings. */

#include "types.h"
#include "sdk/flash.h"
#include "sdk/flash_data.h"

/* The Macronix chip's time limits, a row per enum FlashPhase (row 0 unused): */
/* milliseconds, the timer's reload value and its control. Its erases may take
 */
/* two seconds. */
/* clang-format off */
const u16 Flash_MaxTimeMacronix[] = {
    10, FLASH_TIMER_RELOAD, FLASH_TIMER_CONTROL,
    10, FLASH_TIMER_RELOAD, FLASH_TIMER_CONTROL,
    2000, FLASH_TIMER_RELOAD, FLASH_TIMER_CONTROL,
    2000, FLASH_TIMER_RELOAD, FLASH_TIMER_CONTROL,
};
/* clang-format on */

/* The Panasonic chip's time limits, laid out as the Macronix chip's. */
/* clang-format off */
const u16 Flash_MaxTimePanasonic[] = {
    10, FLASH_TIMER_RELOAD, FLASH_TIMER_CONTROL,
    10, FLASH_TIMER_RELOAD, FLASH_TIMER_CONTROL,
    500, FLASH_TIMER_RELOAD, FLASH_TIMER_CONTROL,
    500, FLASH_TIMER_RELOAD, FLASH_TIMER_CONTROL,
};
/* clang-format on */

const struct FlashSetupInfo Flash_ChipMacronix = {
	.program = ProgramFlashSector,
	.eraseChip = EraseFlashChip,
	.eraseSector = EraseFlashSector,
	.wait = WaitForFlashWrite,
	.maxTime = (u16 *)Flash_MaxTimeMacronix,
	.type = {.romSize = 65536,
			 .sector = {.size = 4096, .shift = 12, .count = 16, .top = 0},
			 .wait = {3, 1},
			 .id = {.joined = 0x1cc2}},
};

const struct FlashSetupInfo Flash_ChipPanasonic = {
	.program = ProgramFlashSector,
	.eraseChip = EraseFlashChip,
	.eraseSector = EraseFlashSector,
	.wait = WaitForFlashWrite,
	.maxTime = (u16 *)Flash_MaxTimePanasonic,
	.type = {.romSize = 65536,
			 .sector = {.size = 4096, .shift = 12, .count = 16, .top = 0},
			 .wait = {0, 2},
			 .id = {.joined = 0x1b32}},
};
