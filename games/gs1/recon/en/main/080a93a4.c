#include "types.h"

#define M2C_FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

extern void *Data_03001f2c;
extern u8 Value_00000075;

s32 Func_08015270(s32);
void Func_080030f8(s32);
void Func_08015080(s32, s32, s32, s32);
s32 Func_08015280(s32, s32, s32, s32, s32);
s32 Func_080a2268(s32, s32, s32, s32, s32, s32);
u8 *Func_08077018(s32);

s32 Func_080a93a4(s32 arg0, s32 arg1, void *ctx)
{
    void *menu;
    s32 row;
    s32 y;
    s32 height;
    u16 *slot;

    menu = Data_03001f2c;
    M2C_FIELD(ctx, u32, 24) =
        M2C_FIELD(ctx, u32, 8) * 5 + M2C_FIELD(ctx, u32, 16);

    Func_08015270(M2C_FIELD(menu, u32, 44));
    Func_080030f8(1);

    {
        u16 flagv = *(u16 *)((u8 *)menu + M2C_FIELD(ctx, u32, 24) * 2 + 456);
        if (flagv != 0) {
            Func_08015080(
                (flagv & 0x1ff) + (s32)&Value_00000075,
                M2C_FIELD(menu, u32, 44),
                0,
                0);
        }
    }

    height = 1;
    slot = (u16 *)((u8 *)menu + M2C_FIELD(ctx, u32, 8) * 5 * 2 + 456);
    for (row = 0, y = 1; row <= 4; row++, y += 2, slot++) {
        if (row == M2C_FIELD(ctx, u32, 16)) {
            u16 flagv =
                *(u16 *)((u8 *)menu + M2C_FIELD(ctx, u32, 24) * 2 + 456);
            s32 type = *(s32 *)(Func_08077018(flagv & 0x1ff) + 20);
            if (type == 4) {
                Func_080a2268(arg0, 14, y, 14, height, 14);
            } else {
                Func_08015280(arg0, type + 1, 27, y, 0);
                Func_080a2268(arg0, 14, y, 13, height, 14);
            }
        } else {
            u16 flagv = *slot;
            s32 type = *(s32 *)(Func_08077018(flagv & 0x1ff) + 20);
            s32 width = 14;
            if (type != 4) {
                Func_08015280(arg0, type + 1, 27, y, 4);
                width = 13;
            }
            Func_080a2268(arg0, 14, y, width, height, 15);
        }
    }

    {
        s32 n;
        y = 1;
        for (n = 3; n >= 0; n--) {
            Func_080a2268(M2C_FIELD(menu, u32, 48), 1, y, 12, 1, 15);
            y += 2;
        }
    }

    {
        u16 flagv = *(u16 *)((u8 *)menu + M2C_FIELD(ctx, u32, 24) * 2 + 456);
        if (flagv & 0x200) {
            u8 *item = Func_08077018(flagv & 0x1ff);
            switch (item[2]) {
                case 1:
                    Func_080a2268(M2C_FIELD(menu, u32, 48), 1, 1, 12, 1, 14);
                    break;
                case 2:
                    Func_080a2268(M2C_FIELD(menu, u32, 48), 1, 7, 12, 1, 14);
                    break;
                case 3:
                    Func_080a2268(M2C_FIELD(menu, u32, 48), 1, 5, 12, 1, 14);
                    break;
                case 4:
                    Func_080a2268(M2C_FIELD(menu, u32, 48), 1, 3, 12, 1, 14);
                    break;
            }
        }
    }

    Func_080030f8(1);
    return 1;
}
