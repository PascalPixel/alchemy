#include "types.h"

struct GridOrigin_080a1ac0 {
    u8 unknown_00[0x0C];
    u16 x;
    u16 y;
};

struct CursorObject_080a1ac0 {
    u8 unknown_00[6];
    s16 x;
    s16 y;
    u8 unknown_0A[0x0A];
    u8 attribute_y;
    u8 unknown_15;
    u16 attribute_x;
};

struct CursorState_080a1ac0 {
    u8 unknown_000[0x10];
    struct GridOrigin_080a1ac0 *origin;
    struct CursorObject_080a1ac0 *cursor;
    u8 unknown_018[0x20A];
    u16 cancel_move;
};

extern struct CursorState_080a1ac0 *Data_03001f2c;

s32 Func_080022ec(s32, s32);
void Func_080030f8(s32);

/*
 * Move the cursor to a local coordinate in two interpolation steps, updating
 * both its software coordinates and packed hardware attributes.
 */
void Func_080a1ac0(s32 target_x, s32 target_y)
{
    struct CursorState_080a1ac0 *state = Data_03001f2c;
    struct CursorObject_080a1ac0 *cursor;
    s32 fixed_x;
    s32 fixed_y;
    s32 step_x;
    s32 step_y;
    s32 remaining;

    if (state->cancel_move != 0) {
        state->cancel_move = 0;
        return;
    }

    cursor = state->cursor;
    cursor->x = (s16)((cursor->attribute_x & 0x01FF) + 0x40);
    cursor->y = (s16)(cursor->attribute_y + 0x40);

    if ((u16)cursor->x > 8) {
        cursor->x = (s16)((u16)cursor->x - 8);
    }
    if ((u16)cursor->y > 8) {
        cursor->y = (s16)((u16)cursor->y - 8);
    }

    fixed_x = (u16)cursor->x * 0x10;
    step_x = Func_080022ec(
        ((target_x + 0x40) * 0x10 - fixed_x) + 1, 2);
    fixed_y = (u16)cursor->y * 0x10;
    step_y = Func_080022ec(
        ((target_y + 0x40) * 0x10 - fixed_y) + 1, 2);

    remaining = 2;
    do {
        struct GridOrigin_080a1ac0 *origin = state->origin;
        s32 x;
        s32 y;

        fixed_x += step_x;
        x = (fixed_x >> 4) + origin->x * 8 - 0x38;
        cursor->x = (s16)x;
        cursor->attribute_x =
            (cursor->attribute_x & 0xFE00) | (x & 0x01FF);

        fixed_y += step_y;
        y = (fixed_y >> 4) + origin->y * 8 - 0x38;
        cursor->y = (s16)y;
        cursor->attribute_y = (u8)y;

        remaining--;
        if (remaining != 0) {
            Func_080030f8(1);
        }
    } while (remaining != 0);
}
