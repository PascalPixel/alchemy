#include "types.h"

/*
 * resource_3cd owner at 0x02000214, 668 bytes through the next prologue at
 * 0x020004b0.  The executable body is 0x02000214..0x0200046d (602 bytes);
 * 0x0200046e is alignment and 0x02000470..0x020004af is this owner's
 * literal pool.  The pool contains the two DMA3 bursts, three in-image label
 * addresses, the 0x1ff mask, the two id bases, and the two input words.
 *
 * This is the value-entry screen paired with resource_3cd's 0x020004b0
 * selector.  It opens a 30x7 value window and a 28x10 preview window, wraps a
 * 0..269 cursor, redraws the selected record, and closes on the B latch.  A
 * and the six d-pad masks are independent tests, as they are in the machine
 * code: multiple held directions can therefore update the cursor in one
 * pass.  The two availability searches are deliberately kept as separate
 * loops.  The first steps down by one and the second steps up by one, with
 * each candidate wrapped by the relocated modulo helper before its record's
 * halfword at +6 is tested.
 *
 * All 35 call sites in the owner resolve through this overlay's import veneer
 * table: Func_030003ac x3, Func_080000c0 x3, Func_08015010 x2,
 * Func_08015018 x2, Func_08015080 x2, Func_08015098 x3,
 * Func_080150a8 x1, Func_08015270 x3, Func_08015278 x1,
 * Func_08077018 x3, Func_08077030 x1, Func_08077250 x1,
 * Func_080a1048 x1, and Func_080f9010 x9.  No control-flow block is elided.
 */

/* Import arities vary between call sites, so retain the historical old-style
 * declarations while keeping every call's register argument visible. */
void Func_080000c0();
void Func_08015018();
void Func_08015080();
void Func_08015098();
void Func_080150a8();
void Func_08015270();
void Func_08015278();
void Func_080f9010();
void Func_080a1048();

s32 Func_030003ac();
s32 Func_08015010();
u8 *Func_08077018();
s32 Func_08077030();
s32 Func_08077250();

/* In-image label records reached through this owner's literal pool. */
extern u8 Data_020088d4[];
extern u8 Data_020088e0[];
extern u8 Data_020088f8[];

void Func_02000214(void)
{
    volatile u32 *dma3 = (volatile u32 *)0x040000d4;
    volatile u32 *held = (volatile u32 *)0x03001c94;
    volatile u32 *pressed = (volatile u32 *)0x03001b04;
    s32 window;
    s32 preview;
    s32 cursor;
    s32 redraw;
    s32 direction;
    s32 masked;
    s32 value;
    u32 keys;
    u8 *entry;

    direction = 0;
    Func_080f9010(0x70);

    window = Func_08015010(0, 0, 30, 7, 2);
    preview = Func_08015010(0, 8, 28, 10, 2);

    cursor = 1;
    redraw = 1;

    /* DMA3SAD is rewritten between bursts, so both transfers start there. */
    dma3[0] = 0x05000200;
    dma3[1] = 0x050001c0;
    dma3[2] = 0x80000010;
    dma3[0] = 0x050001e8;
    dma3[1] = 0x050001c0 + 28;
    dma3[2] = 0x80000001;

    Func_080000c0(1);

    for (;;) {
        if (redraw != 0) {
            redraw = 0;
            cursor = Func_030003ac(cursor + 270, 270);

            Func_08015270(window);
            Func_08015278(window);
            Func_08015098(Data_020088d4, window, 0, 0);
            Func_080150a8(cursor, 0, window, 0x50, redraw);

            if (Func_08077250() != 0) {
                masked = cursor & 0x1ff;
                Func_08015098(Data_020088e0, window, 0, 32);
                Func_08077018(masked);
                Func_08015080(masked + 0x182, window, 120, 0);
                masked += 0x75;
                Func_08015080(masked, window, 0, 16);
                Func_08015270(preview);
                Func_080a1048(preview, cursor);
            } else {
                Func_08015098(Data_020088f8, window, 0, 32);
            }
        }

        keys = *held;
        if ((keys & 1) != 0) {
            if (Func_08077030(cursor) != -1)
                Func_080f9010(0xaf);
        }

        keys = *held;
        if ((keys & 2) != 0) {
            Func_080f9010(0x71);
            goto close;
        }

        if ((*pressed & 0x40) != 0) {
            cursor -= 1;
            direction = -1;
            redraw = 1;
            Func_080f9010(0x6f);
        }
        if ((*pressed & 0x80) != 0) {
            cursor += 1;
            direction = 1;
            redraw = 1;
            Func_080f9010(0x6f);
        }
        if ((*pressed & 0x10) != 0) {
            cursor += 10;
            direction = 1;
            redraw = 1;
            Func_080f9010(0x6f);
        }
        if ((*pressed & 0x20) != 0) {
            cursor -= 10;
            direction = -1;
            redraw = 1;
            Func_080f9010(0x6f);
        }
        if ((*pressed & 0x100) != 0) {
            cursor += 30;
            direction = 1;
            redraw = 1;
            Func_080f9010(0x6f);
        }
        if ((*pressed & 0x200) != 0) {
            cursor -= 30;
            direction = -1;
            redraw = 1;
            Func_080f9010(0x6f);
        }

        if (direction == -1) {
            value = cursor + 270;
            for (;;) {
                cursor = Func_030003ac(value, 270);
                entry = Func_08077018(cursor & 0x1ff);
                if (*(u16 *)(entry + 6) != 0)
                    break;
                value = cursor + 0x10d;
            }
        }

        if (direction == 1) {
            value = cursor + 270;
            for (;;) {
                cursor = Func_030003ac(value, 270);
                entry = Func_08077018(cursor & 0x1ff);
                if (*(u16 *)(entry + 6) != 0)
                    break;
                value = cursor + 0x10f;
            }
        }

        direction = 0;
        Func_080000c0(1);
    }

close:
    Func_08015270(window);
    Func_080000c0(1);
    Func_08015018(window, 1);
    Func_08015018(preview, 1);
}
