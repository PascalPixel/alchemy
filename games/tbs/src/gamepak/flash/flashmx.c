/// @file
/// The Macronix and Panasonic chips' settings.

#include "types.h"
#include "gamepak/flash.h"
#include "gamepak/flash_data.h"

/// The Macronix chip's waits by timing: milliseconds, the timer's reload value
/// and its control.
const u16 Flash_MaxTimeMacronix[] = {
    10, 0xffbd, 0xc2, //
    10, 0xffbd, 0xc2, //
    2000, 0xffbd, 0xc2, //
    2000, 0xffbd, 0xc2,
};

/// The Panasonic chip's waits, as Flash_MaxTimeMacronix's.
const u16 Flash_MaxTimePanasonic[] = {
    10, 0xffbd, 0xc2, //
    10, 0xffbd, 0xc2, //
    500, 0xffbd, 0xc2, //
    500, 0xffbd, 0xc2,
};

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
