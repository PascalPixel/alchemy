/// @file
/// The SST chip's settings, which an unknown chip takes too.

#include "types.h"
#include "gamepak/flash.h"
#include "gamepak/flash_data.h"

/// The SST chip's time limits, a row per enum FlashPhase (row 0 unused):
/// milliseconds, the timer's reload value and its control.
// clang-format off
const u16 Flash_MaxTimeSst[] = {
    10, FLASH_TIMER_RELOAD, FLASH_TIMER_CONTROL,
    10, FLASH_TIMER_RELOAD, FLASH_TIMER_CONTROL,
    40, FLASH_TIMER_RELOAD, FLASH_TIMER_CONTROL,
    200, FLASH_TIMER_RELOAD, FLASH_TIMER_CONTROL,
};
// clang-format on

const struct FlashSetupInfo gFlash_ChipDefault = {
    .program = ProgramFlashSectorWithEraseRetry,
    .eraseChip = EraseFlashChip,
    .eraseSector = EraseFlashSector,
    .wait = WaitForFlashWrite,
    .maxTime = (u16 *)Flash_MaxTimeSst,
    .type = {.romSize = 65536,
             .sector = {.size = 4096, .shift = 12, .count = 16, .top = 0},
             .wait = {3, 3},
             .id = {.joined = 0}},
};

const struct FlashSetupInfo gFlash_ChipSst = {
    .program = ProgramFlashSectorWithEraseRetry,
    .eraseChip = EraseFlashChip,
    .eraseSector = EraseFlashSector,
    .wait = WaitForFlashWrite,
    .maxTime = (u16 *)Flash_MaxTimeSst,
    .type = {.romSize = 65536,
             .sector = {.size = 4096, .shift = 12, .count = 16, .top = 0},
             .wait = {1, 2},
             .id = {.joined = 0xd4bf}},
};
