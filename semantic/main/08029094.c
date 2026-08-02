#include "types.h"

struct Work;

extern volatile u32 Data_03001b04;

void Func_08028ef0(
    struct Work *work, s16 primary, const s16 *secondary);

enum MenuInput {
    MENU_INPUT_ACCEPT = 0x001,
    MENU_INPUT_CANCEL = 0x002,
    MENU_INPUT_UP = 0x010,
    MENU_INPUT_DOWN = 0x020,
    MENU_INPUT_LEFT = 0x040,
    MENU_INPUT_RIGHT = 0x080,
    MENU_INPUT_PAGE_UP = 0x100,
    MENU_INPUT_PAGE_DOWN = 0x200,
};

/*
 * Update the two-part numeric selector. The primary half spans 0..200; the
 * secondary half spans 0..99. Page movement deliberately uses the original
 * 89-point rollover, so adding ten to 90..99 produces 1..10 (and vice versa).
 */
s16 Func_08029094(
    struct Work *work,
    s16 primary,
    s16 *secondary,
    s16 *active_half)
{
    u32 input = Data_03001b04;

    if (input & MENU_INPUT_ACCEPT)
        return -1;

    if (input & MENU_INPUT_CANCEL)
        return -2;

    if (input & (MENU_INPUT_LEFT | MENU_INPUT_RIGHT)) {
        *active_half ^= 1;
        return primary;
    }

    if (input & MENU_INPUT_UP) {
        if (*active_half == 0) {
            primary++;
            if (primary > 200)
                primary = 0;
        } else {
            (*secondary)++;
            if (*secondary > 99)
                *secondary = 0;
        }
        return primary;
    }

    if (input & MENU_INPUT_DOWN) {
        if (*active_half == 0) {
            primary--;
        } else {
            (*secondary)--;
            if (*secondary < 0)
                *secondary = 99;
        }

        if (primary < 0)
            primary = 200;
        return primary;
    }

    if (input & MENU_INPUT_PAGE_UP) {
        if (*active_half == 0) {
            *secondary = 0;
            primary += 10;
        } else {
            s16 previous = *secondary;

            *secondary += 10;
            if (*secondary > 99)
                *secondary = previous - 89;
        }

        if (primary > 200)
            primary = 0;
        Func_08028ef0(work, primary, secondary);
    } else if (input & MENU_INPUT_PAGE_DOWN) {
        if (*active_half == 0) {
            *secondary = 0;
            primary -= 10;
        } else {
            s16 previous = *secondary;

            *secondary -= 10;
            if (*secondary < 0)
                *secondary = previous + 89;
        }

        if (primary < 0)
            primary = 200;
        Func_08028ef0(work, primary, secondary);
    }

    return primary;
}
