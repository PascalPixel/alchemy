#include "types.h"

struct Window_0801db70 {
    u8 padding0[0x0c];
    u16 width;
    u16 height;
};

s32 Func_080770c0(s32 flag);
void Func_0801d980(void);
struct Window_0801db70 *Func_0801d9d4(void);
s32 Func_08021620(
    s32 type, struct Window_0801db70 *window, s32 y, s32 x);
void Func_080030f8(s32 frames);
s32 Func_080022fc(s32 value, s32 modulus);
void Func_080a1038(void *object);
void Func_080216e8(s32 value, u8 attribute, s32 unselected);
void Func_080b0030(void *object, s32 width, s32 y, s32 mode);
void Func_080216b4(s32 cursor);
void Func_080f9010(s32 sound);
void Func_08016418(struct Window_0801db70 *window, s32 mode);
void Func_0801d9bc(void);

s32 Func_0801db70(void)
{
    u8 *runtime = *(u8 **)0x03001ea0;
    s32 eventActive;
    s32 baseOffset = 0;
    s32 count = 3;
    struct Window_0801db70 *window;
    s32 cursor;
    s32 selection;
    s32 dirty = 1;
    s32 result;

    eventActive = Func_080770c0(0x17e);
    Func_0801d980();
    window = Func_0801d9d4();

    cursor = Func_08021620(
        6, window, 40, *(u8 *)0x03001f54 != 0 ? -16 : -24);
    Func_080030f8(1);

    selection = *(u16 *)(runtime + 0x574);
    if (eventActive != 0) {
        baseOffset = 2;
        count = 1;
    }
    if (*(u8 *)0x03001f54 != 0)
        count += 3;

    for (;;) {
        if (dirty != 0) {
            s32 index;

            dirty = 0;
            selection = Func_080022fc(selection + count, count);
            *(u16 *)(runtime + 0x574) = selection;

            for (index = 0; index < count; index++) {
                void *row = *(void **)(runtime + 0x610 + index * 4);

                *((u8 *)row + 15) = 0xfb;
                Func_080a1038(row);
                Func_080216e8(
                    *(s8 *)(0x080367dc + baseOffset + index),
                    *((u8 *)row + 14),
                    index != *(u16 *)(runtime + 0x574));
            }

            Func_080b0030(
                runtime + 0x5a4,
                window->width * 8,
                (selection * 3 + window->height) * 8 + 16,
                3);
        }

        Func_080216b4(cursor);
        Func_080030f8(1);

        if (*(volatile u32 *)0x03001c94 & 1) {
            result = selection;
            Func_080f9010(0x70);
            break;
        } else if (*(volatile u32 *)0x03001c94 & 0x0a) {
            result = -1;
            Func_080f9010(0x71);
            break;
        } else if (*(volatile u32 *)0x03001b04 & 0x40) {
            Func_080f9010(0x6f);
            selection--;
            dirty = 1;
        } else if (*(volatile u32 *)0x03001b04 & 0x80) {
            Func_080f9010(0x6f);
            selection++;
            dirty = 1;
        }
    }

    Func_08016418(window, 2);
    Func_0801d9bc();
    Func_080030f8(1);

    if (result >= 0)
        result += baseOffset;
    return result;
}
