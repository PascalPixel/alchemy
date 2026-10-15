/*
 * Draft of overlay 6b3 Func_02000244: centres a message and hands it to
 * Func_02000158. The logic is right; the listing keeps x and y in their stack
 * slots (it stores y in every branch and loads both once before the call),
 * where this draft forwards y as a constant and reloads x in each branch, and
 * the listing builds x's and y's zero in r4 apart from the mode's zero in r3.
 * Tried: integer-cast addresses, "y = x = 0", and the four values as one
 * struct (the stores then stay, but through a base register: 75 differ).
 */
#include "types.h"

extern s32 gData_02000a28;

void UiText_GetResourceDimensions();
void Func_02000158();

void Func_02000244(s32 message, s32 position, s32 wait)
{
    s32 x;
    s32 y;
    s32 width;
    s32 height;
    s32 mode = 0;
    s32 left;
    s32 line;
    s32 half;
    s32 wide;

    x = 0;
    y = 0;
    wide = message & 0x8000;
    UiText_GetResourceDimensions(gData_02000a28, &x, &y, &width, &height);
    line = 0;
    if (width <= 24) {
        if (!wide)
            x = (25 - width) / 2 + 5;
        else
            x = (30 - width) / 2;
        left = x - 5;
    } else {
        x = (30 - width) / 2;
        left = x;
    }
    half = (height - 1) / 2;
    if (position == 0) {
        y = 3;
        line = half + 1;
    } else if (position == 1) {
        y = 14 - half;
        line = y + half - 2;
    } else if (position == 2) {
        if (half == 3) {
            y = 13;
            line = 15;
            mode = 2;
        } else {
            y = 16;
            line = 15;
        }
    }
    Func_02000158(message, x, y, left, line, wait, mode);
}
