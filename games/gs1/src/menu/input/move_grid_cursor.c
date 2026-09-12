#include "types.h"

s32 Menu_MoveGridCursor(s32 *arg0, s32 *arg1, s32 arg2)
{
    s32 row;
    s32 col;

    col = *arg0;
    row = *arg1;
    switch (arg2) {
    case 0x40:
        row -= 1;
        if (row < 0) {
            row = 5;
        }
        if (row <= 3) {
            if (row == 3) {
                if (col <= 4) {
                    col = 0;
                } else {
                    goto set_one;
                }
            } else if (col > 1) {
set_one:
                col = 1;
            }
            if ((row == 3) && (col == 1)) {
                row = 2;
            }
        }
        break;
    case 0x80:
        row += 1;
        if (row > 5) {
            row = 0;
        }
        if ((row == 3) && (col == 1)) {
            row = 4;
        }
        if (row == 4) {
            goto set_zero;
        }
        break;
    case 0x20:
        col -= 1;
        if (row == 3) {
            col += 1;
        } else if (row > 3) {
            if (col < 0) {
                col = 7;
            }
        } else if (col < 0) {
            col = 1;
        }
        break;
    case 0x10:
        col += 1;
        if (row == 3) {
            col -= 1;
        } else if (row > 3) {
            if (col > 7) {
                goto set_zero;
            }
        } else if (col > 1) {
            goto set_zero;
        }
        break;
set_zero:
        col = 0;
        break;
    }
    *arg0 = col;
    *arg1 = row;
    return (row * 9) + col;
}
