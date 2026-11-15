/// @file
/// The Atmel chip's settings: as the other chips see it, in 4 KB sectors, and
/// as its own routines work, in 128-byte ones.

#include "types.h"
#include "gamepak/flash.h"
#include "gamepak/flash_data.h"

/// The Atmel chip's waits by timing: milliseconds, the timer's reload value
/// and its control.
// clang-format off
const u16 Flash_MaxTimeAtmel[] = {
    10, 0xffbd, 0xc2,
    40, 0xffbd, 0xc2,
    0, 0,      0,
    40, 0xffbd, 0xc2,
};
// clang-format on

const struct FlashSetupInfo gFlash_ChipAtmel = {
    .program = ProgramAtmelFlashBlock,
    .eraseChip = EraseAtmelFlashChip,
    .eraseSector = EraseAtmelFlashBlock,
    .wait = WaitForFlashWrite,
    .maxTime = (u16 *)Flash_MaxTimeAtmel,
    .type = {.romSize = 65536,
             .sector = {.size = 4096, .shift = 12, .count = 16, .top = 0},
             .wait = {3, 3},
             .id = {.joined = 0x3d1f}},
};

const struct FlashSetupInfo gFlash_ChipAtmelLayout = {
    // ProgramAtmelFlashSector takes its sector as a word and its source as
    // const, unlike the slot's type.
    .program = (u16 (*)(u16, u8 *))ProgramAtmelFlashSector,
    .eraseChip = EraseAtmelFlashChip,
    .eraseSector = EraseAtmelFlashSector,
    .wait = WaitForFlashWrite,
    .maxTime = (u16 *)Flash_MaxTimeAtmel,
    .type = {.romSize = 65536,
             .sector = {.size = 128, .shift = 7, .count = 512, .top = 0},
             .wait = {3, 3},
             .id = {.joined = 0x3d1f}},
};
