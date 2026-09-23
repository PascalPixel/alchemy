#include "DMA.H"

struct UiSprite {
    u8 unknown_00[24];
    u16 tile : 10;
    u16 flags : 6;
};

void *Func_080048f4(s32 kind, s32 size);
s32 Func_08015010(s32 x, s32 y, s32 width, s32 height, s32 flags);
s32 Func_08004080(void);
s32 Func_08003fa4(u32 slot, u32 size, const void *source);
struct UiSprite *Func_080150c8(s32 slot, s32 attributes, s32 window, s32 x, s32 y);
void Func_080f9010(s32 cue);
s32 Func_080022fc(s32 value, s32 divisor);
void Func_080b06c0(s32 value, s32 x, void *buffer);
void Func_080150b0(s32 value, s32 digits, s32 window, s32 x, s32 y);
void Func_08015080(s32 message, s32 window, s32 x, s32 y);
s32 Func_080030f8(s32 frames);
void Func_08015018(s32 window, s32 mode);
void Func_08002dd8(s32 kind);

/* Spins a quantity from minimum + 1 to maximum with left and right, redrawing
   the count and its total price; returns the chosen count, or -1 when
   cancelled. */
s32 Shop_SelectQuantity(s32 minimum, s32 maximum, s32 price)
{
    void *buffer;
    s32 window;
    s32 changed;
    s32 slot;
    s32 count;
    s32 result;
    struct UiSprite *sprite;

    buffer = Func_080048f4(14, 0x400);
    changed = 1;
    maximum -= minimum;
    result = -1;
    count = 0;
    window = Func_08015010(7, 4, 23, 3, 2);
    if (window != 0) {
        slot = Func_08004080();
        if (slot != 96) {
            Func_08003fa4(slot, 0x100, 0);
            Func_080150c8(slot, 0x40004000, window, 0, 0);
            sprite = Func_080150c8(slot, 0x40004000, window, 32, 0);
            sprite->tile += 4;
            for (;;) {
                if (*(volatile u32 *)0x03001c94 & 1) {
                    Func_080f9010(112);
                    result = count + 1;
                    break;
                }
                if (*(volatile u32 *)0x03001c94 & 2) {
                    Func_080f9010(113);
                    result = -1;
                    break;
                }
                if (*(volatile u32 *)0x03001b04 & 32) {
                    Func_080f9010(111);
                    changed = 1;
                    count--;
                }
                if (*(volatile u32 *)0x03001b04 & 16) {
                    Func_080f9010(111);
                    changed = 1;
                    count++;
                }
                if (changed) {
                    changed = 0;
                    count = Func_080022fc(count + maximum, maximum);
                    Dma_Set((const void *)0x080b3f80, buffer, 0x84000040, (volatile u32 *)0x040000d4);
                    Func_080b06c0(30, 14, buffer);
                    Func_080b06c0(minimum + maximum, 0, buffer);
                    Func_080b06c0(minimum + count + 1, 10, buffer);
                    Func_080b06c0(minimum, 2, buffer);
                    Func_08003fa4(slot, 0x100, buffer);
                    result = count + 1;
                    Func_080150b0(result, 2, window, 72, 0);
                    Func_080150b0(price * result, 6, window, 88, 0);
                    Func_08015080(0xc88, window, 136, 0);
                }
                Func_080030f8(1);
            }
            Func_080030f8(1);
            Func_08015018(window, 2);
        }
    }
    Func_08002dd8(14);
    return result;
}
