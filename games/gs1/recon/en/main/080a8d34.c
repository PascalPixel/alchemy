#include "types.h"
#include "item.h"

/*
 * Data_03001f2c is the polymorphic menu-runtime cell shared across many
 * owners. This owner's view exposes a saved window handle at +44 and a
 * 5-column-per-row u16 item-slot grid at +456 (row = *(sel+8), selected
 * column = *(sel+16)).
 */
extern u8 *Data_03001f2c;

void Func_08015270(s32 window);
void Func_080030f8(s32 flag);
void Func_08015080(s32 msg, s32 target, s32 x, s32 y);
void Func_08015068(s32 obj, s32 x, s32 y, s32 w, s32 h);
void Func_08015280(s32 window, s32 tile, s32 x, s32 y, s32 style);
void Func_080a2268(const u8 *object, s32 x, s32 y, s32 width, s32 height,
    u32 field);
struct ItemDefinition *Func_08077080(s32 item);

s32 Func_080a8d34(const u8 *obj, s32 unused, u8 *sel)
{
    u8 *menu;
    s32 i;
    s32 y;

    menu = Data_03001f2c;
    *(s32 *)(sel + 24) = *(s32 *)(sel + 8) * 5 + *(s32 *)(sel + 16);

    Func_08015270(*(s32 *)(menu + 44));
    Func_080030f8(1);

    {
        u16 *grid = (u16 *)(menu + 456);
        s32 cell = *(s32 *)(sel + 24);

        if (grid[cell] != 0) {
            struct ItemDefinition *item;
            u8 b;
            s32 flags;
            s32 mask = 0x3fff;
            s32 base = 0x53a;

            Func_08015080((grid[cell] & mask) + base,
                *(s32 *)(menu + 44), 0, 0);

            cell = *(s32 *)(sel + 24);
            item = Func_08077080(grid[cell] & mask);

            Func_08015068((s32)obj, 0, 96, 224, 104);

            flags = 0;
            if (((u8 *)item)[12] != 0) {
                b = ((u8 *)item)[1];
                flags = 2;
            } else {
                b = ((u8 *)item)[1];
                if ((b & 0x40) != 0)
                    flags = 2;
            }
            if ((b & 0x80) != 0)
                flags |= 1;

            if (flags == 3)
                Func_08015080(0xb15, (s32)obj, 0, 96);
            else if (flags == 2)
                Func_08015080(0xb14, (s32)obj, 0, 96);
            else if (flags == 1)
                Func_08015080(0xb13, (s32)obj, 0, 96);
        }
    }

    {
        u16 *p = (u16 *)(menu + 456) + *(s32 *)(sel + 8) * 5;
        s32 height = 1;

        y = 2;
        for (i = 0; i <= 4; i++) {
            s32 field;

            if (i == *(s32 *)(sel + 16)) {
                struct ItemDefinition *item = Func_08077080(*p & 0x3fff);
                u8 kind = ((u8 *)item)[2];

                if (kind == 4) {
                    Func_080a2268(obj, 9, y, 19, height, 14);
                    goto next;
                }
                Func_08015280((s32)obj, kind + 1, 24, y, 0);
                field = 14;
            } else {
                struct ItemDefinition *item = Func_08077080(*p & 0x3fff);
                u8 kind = ((u8 *)item)[2];

                if (kind == 4) {
                    Func_080a2268(obj, 9, y, 19, height, 15);
                    goto next;
                }
                Func_08015280((s32)obj, kind + 1, 24, y, 4);
                field = 15;
            }
            Func_080a2268(obj, 9, y, 15, height, field);
            Func_080a2268(obj, 25, y, 3, height, field);
        next:
            y += 2;
            p++;
        }
    }

    Func_080030f8(1);
    return 1;
}
