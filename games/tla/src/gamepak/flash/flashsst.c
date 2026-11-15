/// @file
/// The SST chip's settings, which an unknown chip takes too.

#include "types.h"
#include "gamepak/flash.h"
#include "gamepak/flash_data.h"

/// The SST chip's waits by timing: milliseconds, the timer's reload value and
/// its control.
// clang-format off
const u16 Flash_MaxTimeSst[] = {
    10, 0xffbd, 0xc2,
    10, 0xffbd, 0xc2,
    40, 0xffbd, 0xc2,
    200, 0xffbd, 0xc2,
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

const struct FlashSetupInfo Flash_ChipSst = {
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
