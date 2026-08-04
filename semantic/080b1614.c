#include "types.h"

#define OFFSET_OF(type, member) ((u32)&(((type *)0)->member))

struct Sprite_080b1614 {
    u8 unknown_00[0x18];
    u16 attribute_18;
};

typedef char Assert_080b1614_sprite_attribute[
    OFFSET_OF(struct Sprite_080b1614, attribute_18) == 0x18 ? 1 : -1];

s32 Func_080022fc(s32 value, s32 divisor);
void Func_08002dd8(s32 allocation);
void Func_080030f8(s32 frames);
void Func_08003fa4(s32 tile, s32 size, void *source);
s32 Func_08004080(void);
u8 *Func_080048f4(s32 allocation, s32 size);
s32 Func_08015010(s32, s32, s32, s32, s32);
void Func_08015018(s32 window, s32 mode);
void Func_08015080(s32 text, s32 window, s32 x, s32 y);
void Func_080150b0(s32 value, s32 digits, s32 window, s32 x, s32 y);
struct Sprite_080b1614 *Func_080150c8(
    s32 tile, u32 flags, s32 window, s32 x, s32 y);
void Func_080b06c0(s32 count, s32 selector, u8 *base);
void Func_080f9010(s32 sound);

/*
 * Present a cyclic quantity picker. The selected quantity is one-based;
 * cancellation or a failed setup returns -1.
 */
s32 Func_080b1614(s32 lower, s32 upper, s32 unit_value)
{
    u8 *buffer = Func_080048f4(0x0e, 0x400);
    s32 count = upper - lower;
    s32 result = -1;
    s32 selection = 0;
    s32 dirty = 1;
    s32 window = Func_08015010(7, 4, 0x17, 3, 2);

    if (window != 0) {
        s32 tile = Func_08004080();

        if (tile != 0x60) {
            struct Sprite_080b1614 *sprite;

            Func_08003fa4(tile, 0x100, 0);
            Func_080150c8(tile, 0x40004000, window, 0, 0);
            sprite = Func_080150c8(
                tile, 0x40004000, window, 0x20, 0);
            sprite->attribute_18 =
                (sprite->attribute_18 & 0xfc00) |
                ((sprite->attribute_18 + 4) & 0x03ff);

            for (;;) {
                if ((*(volatile u32 *)0x03001c94 & 1) != 0) {
                    Func_080f9010(0x70);
                    result = selection + 1;
                    break;
                }

                if ((*(volatile u32 *)0x03001c94 & 2) != 0) {
                    Func_080f9010(0x71);
                    result = -1;
                    break;
                }

                if ((*(volatile u32 *)0x03001b04 & 0x20) != 0) {
                    Func_080f9010(0x6f);
                    dirty = 1;
                    selection--;
                }
                if ((*(volatile u32 *)0x03001b04 & 0x10) != 0) {
                    Func_080f9010(0x6f);
                    dirty = 1;
                    selection++;
                }

                if (dirty != 0) {
                    volatile u32 *dma3 = (volatile u32 *)0x040000d4;
                    s32 quantity;

                    dirty = 0;
                    selection = Func_080022fc(selection + count, count);

                    dma3[0] = 0x080b3f80;
                    dma3[1] = (u32)buffer;
                    dma3[2] = 0x84000040;

                    Func_080b06c0(0x1e, 0x0e, buffer);
                    Func_080b06c0(upper, 0, buffer);
                    Func_080b06c0(lower + selection + 1, 0x0a, buffer);
                    Func_080b06c0(lower, 2, buffer);
                    Func_08003fa4(tile, 0x100, buffer);

                    quantity = selection + 1;
                    Func_080150b0(quantity, 2, window, 0x48, 0);
                    Func_080150b0(
                        quantity * unit_value, 6, window, 0x58, 0);
                    Func_08015080(0x0c88, window, 0x88, 0);
                }

                Func_080030f8(1);
            }

            Func_080030f8(1);
            Func_08015018(window, 2);
        }
    }

    Func_08002dd8(0x0e);
    return result;
}
