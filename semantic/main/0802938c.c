#include "types.h"

enum KeyMask_0802938c {
    KEY_A_0802938c      = 0x001,
    KEY_B_0802938c      = 0x002,
    KEY_SELECT_0802938c = 0x004,
    KEY_START_0802938c  = 0x008,
    KEY_RIGHT_0802938c  = 0x010,
    KEY_LEFT_0802938c   = 0x020,
    KEY_UP_0802938c     = 0x040,
    KEY_DOWN_0802938c   = 0x080,
    KEY_R_0802938c      = 0x100,
    KEY_L_0802938c      = 0x200
};

struct GridCoordinate_0802938c {
    s32 x;
    s32 y;
};

s32 Func_080770c0(s32 flag);
void Func_080770c8(s32 flag);
void Func_080770d0(s32 flag);

/*
 * Handle one frame of input for the 16-by-16 flag grid.  Coordinate changes
 * return zero, page changes or flag toggles return one, and cancel returns -1.
 */
s32 Func_0802938c(
    s32 window,
    s32 *page,
    struct GridCoordinate_0802938c *cursor)
{
    u32 pressed = *(volatile u32 *)0x03001b04;
    u32 repeated = *(volatile u32 *)0x03001c94;

    (void)window;

    if ((pressed & KEY_A_0802938c) != 0) {
        s32 flag = ((*page * 16 + cursor->y) * 16) + cursor->x;

        if (Func_080770c0(flag) != 0) {
            Func_080770d0(flag);
        } else {
            Func_080770c8(flag);
        }
        return 1;
    }

    if ((repeated & KEY_B_0802938c) != 0 ||
        (pressed & KEY_SELECT_0802938c) != 0) {
        return -1;
    }

    if ((pressed & KEY_UP_0802938c) != 0) {
        cursor->y--;
        if (cursor->y < 0) {
            cursor->y = 15;
        }
        return 0;
    }
    if ((pressed & KEY_DOWN_0802938c) != 0) {
        cursor->y++;
        if (cursor->y > 15) {
            cursor->y = 0;
        }
        return 0;
    }
    if ((pressed & KEY_LEFT_0802938c) != 0) {
        cursor->x--;
        if (cursor->x < 0) {
            cursor->x = 15;
        }
        return 0;
    }
    if ((pressed & KEY_RIGHT_0802938c) != 0) {
        cursor->x++;
        if (cursor->x > 15) {
            cursor->x = 0;
        }
        return 0;
    }

    if ((pressed & KEY_L_0802938c) != 0) {
        if ((pressed & KEY_START_0802938c) != 0) {
            *page -= 10;
        } else {
            (*page)--;
        }
        if (*page < 0) {
            *page = 15;
        }
        return 1;
    }

    if ((pressed & KEY_R_0802938c) != 0) {
        if ((pressed & KEY_START_0802938c) != 0) {
            *page += 10;
        } else {
            (*page)++;
        }
        if (*page > 15) {
            *page = 0;
        }
        return 1;
    }

    return 0;
}
