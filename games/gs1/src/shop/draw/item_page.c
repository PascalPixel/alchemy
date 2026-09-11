#include "types.h"
#include "scene.h"

/* shop/draw/item_page.c */
/* Main-image symbols: every pool word inside the ROM or the work RAM. */
extern u8 gVal[];
extern u8 gVal2[];
extern u8 gIw[];

s32 Shop_DrawItemPage(s32 a0, s32 a1, s32 a2)
{
    u32 i;
    s32 p10;
    s32 p10b;
    s32 p11;
    s32 p8;
    s32 p9;
    s32 p9b;
    s32 rec5;
    s32 record;
    s32 r10;
    s32 v10;
    s32 v5;
    s32 base6_0;
    s32 v6;
    s32 base5_af7;

    p9 = *(s32 *)gIw;
    rec5 = Sys_Check(*(u8 *)((0x21a + p9)));
    Sys_unk3_4(a0, 128, 8, 224, 96);
    p8 = ((*(s32 *)(a2 + 8) << 2) + *(s32 *)(a2 + 8));
    p10 = ((u32)(s32)((s32)(*(s32 *)(a2 + 20) - (s32)p8) << 24) >> 24);
    v10 = p10;
    if ((u32)p10 > 5) {
        v10 = 5;
    }
    Menu_SetPageIcons(5, p8, a0, 119, 52);
    Menu_DrawPageIndicator(a0, *(s32 *)(a2 + 20), 5, *(s32 *)(a2 + 8), 28);
    if (*(u8 *)((0x218 + p9)) == 0) {
        Sys_Run(0xad7, a0, 120, 8);
        v6 = r10;
    } else {
        v6 = 0;
        base6_0 = 0;
        /* The entry guard compares the count variable, not a literal zero:
           folded to a literal it becomes `!= 0` and a `beq`, where the
           original keeps the unsigned `bls`. */
        if ((u32)v10 > base6_0) {
            v5 = ((s32)(((s32)p8 << 1) + p9) + 0x1c8);
            do {
                Sys_unk6(((0x1ff & *(u16 *)(v5)) + (s32)gVal), a0, 128, ((base6_0 << 4) + 8));
                base6_0 = ((u32)((base6_0 + 1) << 24) >> 24);
                v5 = (v5 + 2);
            } while ((u32)v10 > base6_0);
            v6 = base6_0;
        }
    }
    UiText_DrawStringAtOffsetFar(rec5, a0, 40, 0);
    base5_af7 = (s32)gVal2;
    Sys_unk6(base5_af7, a0, 32, 16);
    Sys_unk6((base5_af7 + 1), a0, 32, 24);
    Sys_unk8(*(u16 *)(rec5 + 60), 3, a0, 80, 16);
    Sys_unk8(*(u16 *)(rec5 + 62), 3, a0, 80, 24);
    p9b = base5_af7 + 1;
    p10b = v6;
    p11 = a0;
    return 1;
}

/* menu/move_grid_cursor.c */
/* menu/input/move_grid_cursor.c */
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

/* menu/status/reserved_status_one.c */
s32 Menu_ReservedStatusOne(void)
{
    return 1;
}
