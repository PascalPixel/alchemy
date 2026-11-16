/// @file
/// The Menu test scene and the test scenes: a table both overlays keep,
/// included at its place in each.

#include "fixed_math.h"
#include "types.h"

/// Unused: eight rows of seven words, the same in both overlays. No other data
/// in the ROM has their layout. The fourth word is the third in 16.16.
// clang-format off
u32 gTest_UnusedRows[] = {
    0x500050, 0, 80,  toFIXED(80),  0x10003, 2, 0,
    0x780078, 0, 120, toFIXED(120), 0x10003, 2, 0,
    0x28003c, 0, 128, toFIXED(128), 0x10002, 1, 0,
    0x28003c, 0, 128, toFIXED(128), 0x10002, 3, 0,
    0x800080, 0, 128, toFIXED(128), 0x20003, 1, 0,
    0x800080, 0, 128, toFIXED(128), 0x20003, 1, 0,
    0x200020, 0, 0,   0,            0,       0, 0,
    0x800080, 0, 128, toFIXED(128), 0x20003, 1, 0,
};
// clang-format on
