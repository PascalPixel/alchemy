/// @file
/// The save screen's progress bar, laid out by step. Its own file: it starts
/// where the progress bar's tiles end, on no word in French, and the tables
/// after it are word-aligned.

#include "types.h"

/// The save screen's progress bar by step, a row of 12 of the progress
/// bar's tiles for each of its seven steps.
// clang-format off
const u8 gWindow_ProgressBarRows[7 * 12] = {
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2,
    3, 5, 4, 1, 1, 1, 1, 1, 1, 1, 1, 2,
    3, 5, 5, 5, 4, 1, 1, 1, 1, 1, 1, 2,
    3, 5, 5, 5, 5, 5, 4, 1, 1, 1, 1, 2,
    3, 5, 5, 5, 5, 5, 5, 5, 4, 1, 1, 2,
    3, 5, 5, 5, 5, 5, 5, 5, 5, 5, 4, 2,
    3, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 7,
};
// clang-format on
