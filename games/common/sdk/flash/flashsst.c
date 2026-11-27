// The SST chip's settings, which an unknown chip takes too.

#include "types.h"
#include "sdk/flash.h"
#include "sdk/flash_data.h"

// The id the SST chip answers with: its device id above its maker id.
#define FLASH_ID_SST (0xd4 << 8 | 0xbf)

// clang-format off
// The SST chip's time limits in milliseconds, by enum FlashPhase (see

// FlashSetupInfo.maxTime): 10 to program a byte, 40 to erase a sector, 200 the whole chip.
const u16 Flash_MaxTimeSst[] = {
    10, FLASH_TIMER_RELOAD, FLASH_TIMER_CONTROL,
    10, FLASH_TIMER_RELOAD, FLASH_TIMER_CONTROL,
    40, FLASH_TIMER_RELOAD, FLASH_TIMER_CONTROL,
    200, FLASH_TIMER_RELOAD, FLASH_TIMER_CONTROL,
};
// clang-format on

// What a chip the SDK's flash library does not know is taken for: SST's functions and timings, with
// no id.
const struct FlashSetupInfo Flash_ChipDefault = {
    .program     = ProgramFlashSectorWithEraseRetry,
    .eraseChip   = EraseFlashChip,
    .eraseSector = EraseFlashSector,
    .wait        = WaitForFlashWrite,
    .maxTime     = (u16 *)Flash_MaxTimeSst,
    .type        = {.romSize = 65536,
                    .sector  = {.size = 4096, .shift = 12, .count = 16, .top = 0},
                    .wait    = {3, 3},
                    .id      = {.joined = 0}},
};

const struct FlashSetupInfo Flash_ChipSst = {
    .program     = ProgramFlashSectorWithEraseRetry,
    .eraseChip   = EraseFlashChip,
    .eraseSector = EraseFlashSector,
    .wait        = WaitForFlashWrite,
    .maxTime     = (u16 *)Flash_MaxTimeSst,
    .type        = {.romSize = 65536,
                    .sector  = {.size = 4096, .shift = 12, .count = 16, .top = 0},
                    .wait    = {1, 2},
                    .id      = {.joined = FLASH_ID_SST}},
};
