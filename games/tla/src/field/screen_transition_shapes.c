/// @file
/// The shapes of the window transition WINDOW_TRANSITION_SHAPE, as The Broken
/// Seal has them.

#include "types.h"
#include "field/screen_transition.h"
#include "system/io_reg.h"

/// Each shape by number: the windows' layers, then its runs of lines, a
/// WIN_RANGE for each line of a run or, for 0, its left and right edges at its
/// first and last lines; 0xffff puts both edges at 255.
// clang-format off
struct TransitionShape gScreenTransition_Shapes[] = {
    {0x0001, TRANSITION_SHOW_ALL, {{160, 0xffff}}},
    {0x0001, TRANSITION_SHOW_ALL, {{80, 0, 120, 120, 134, 120}, {80, 0xffff}}},
    {0x0001, TRANSITION_SHOW_ALL, {{80, 0, 120, 120, 150, 120}, {80, 0xffff}}},
    {0x0001, TRANSITION_SHOW_ALL, {{80, 0, 120, 120, 167, 120}, {80, 0xffff}}},
    {0x0001, TRANSITION_SHOW_ALL, {{80, 0, 120, 120, 188, 120}, {80, 0xffff}}},
    {0x0001, TRANSITION_SHOW_ALL, {{80, 0, 120, 120, 217, 120}, {80, 0xffff}}},
    {0x0001, TRANSITION_SHOW_ALL, {{36, WIN_RANGE(120, 240)}, {44, 0, 120, 120, 240, 120}, {80, 0xffff}}},
    {0x0001, TRANSITION_SHOW_ALL, {{59, WIN_RANGE(120, 240)}, {21, 0, 120, 120, 240, 120}, {80, 0xffff}}},
    {0x0001, TRANSITION_SHOW_ALL, {{80, WIN_RANGE(120, 240)}, {80, 0xffff}}},
    {0x0001, TRANSITION_SHOW_ALL, {{80, WIN_RANGE(120, 240)}, {21, 0, 120, 240, 240, 240}, {59, 0xffff}}},
    {0x0001, TRANSITION_SHOW_ALL, {{80, WIN_RANGE(120, 240)}, {44, 0, 120, 240, 240, 240}, {36, 0xffff}}},
    {0x0001, TRANSITION_SHOW_ALL, {{80, WIN_RANGE(120, 240)}, {80, 0, 120, 217, 240, 240}}},
    {0x0001, TRANSITION_SHOW_ALL, {{80, WIN_RANGE(120, 240)}, {80, 0, 120, 188, 240, 240}}},
    {0x0001, TRANSITION_SHOW_ALL, {{80, WIN_RANGE(120, 240)}, {80, 0, 120, 167, 240, 240}}},
    {0x0001, TRANSITION_SHOW_ALL, {{80, WIN_RANGE(120, 240)}, {80, 0, 120, 150, 240, 240}}},
    {0x0001, TRANSITION_SHOW_ALL, {{80, WIN_RANGE(120, 240)}, {80, 0, 120, 134, 240, 240}}},
    {0x0001, TRANSITION_SHOW_ALL, {{160, WIN_RANGE(120, 240)}}},
    {0x007f, 0x0101, {{80, WIN_RANGE(0, 120)}, {80, 0, 0, 0, 120, 106}}},
    {0x007f, 0x0101, {{80, WIN_RANGE(0, 120)}, {80, 0, 0, 0, 120, 90}}},
    {0x007f, 0x0101, {{80, WIN_RANGE(0, 120)}, {80, 0, 0, 0, 120, 73}}},
    {0x007f, 0x0101, {{80, WIN_RANGE(0, 120)}, {80, 0, 0, 0, 120, 52}}},
    {0x007f, 0x0101, {{80, WIN_RANGE(0, 120)}, {80, 0, 0, 0, 120, 23}}},
    {0x007f, 0x0101, {{80, WIN_RANGE(0, 120)}, {44, 0, 0, 0, 120, 0}, {36, 0xffff}}},
    {0x007f, 0x0101, {{80, WIN_RANGE(0, 120)}, {21, 0, 0, 0, 120, 0}, {59, 0xffff}}},
    {0x007f, 0x0101, {{80, WIN_RANGE(0, 120)}, {80, 0xffff}}},
    {0x007f, 0x0101, {{59, WIN_RANGE(0, 120)}, {21, 0, 0, 120, 120, 120}, {80, 0xffff}}},
    {0x007f, 0x0101, {{36, WIN_RANGE(0, 120)}, {44, 0, 0, 120, 120, 120}, {80, 0xffff}}},
    {0x007f, 0x0101, {{80, 0, 23, 120, 120, 120}, {80, 0xffff}}},
    {0x007f, 0x0101, {{80, 0, 52, 120, 120, 120}, {80, 0xffff}}},
    {0x007f, 0x0101, {{80, 0, 73, 120, 120, 120}, {80, 0xffff}}},
    {0x007f, 0x0101, {{80, 0, 90, 120, 120, 120}, {80, 0xffff}}},
    {0x007f, 0x0101, {{80, 0, 106, 120, 120, 120}, {80, 0xffff}}},
};
// clang-format on
