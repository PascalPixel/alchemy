/* @file */
/* The Macronix and Panasonic chips' settings. */

#include "types.h"
#include "sdk/flash.h"
#include "sdk/flash_data.h"

/* The id a Panasonic chip answers with (Flash_ChipPanasonic's): its device id
 */
/* 0x1b above the maker id 0x32. */
#define FLASH_ID_PANASONIC (0x1b << 8 | 0x32)

/* clang-format off */
/* The Macronix chip's time limits in milliseconds, by enum FlashPhase (see */
/* FlashSetupInfo.maxTime): 10 to program a byte, 2 seconds to erase a sector */
/* or the whole chip. */
const u16 Flash_MaxTimeMacronix[] = {
    10, FLASH_TIMER_RELOAD, FLASH_TIMER_CONTROL,
    10, FLASH_TIMER_RELOAD, FLASH_TIMER_CONTROL,
    2000, FLASH_TIMER_RELOAD, FLASH_TIMER_CONTROL,
    2000, FLASH_TIMER_RELOAD, FLASH_TIMER_CONTROL,
};
/* clang-format on */

/* clang-format off */
/* The Panasonic chip's time limits, as Flash_MaxTimeMacronix's: half a second */
/* to erase. */
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
			 .id = {.joined = FLASH_ID_MACRONIX}},
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
			 .id = {.joined = FLASH_ID_PANASONIC}},
};
