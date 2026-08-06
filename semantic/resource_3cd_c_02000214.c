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
















/* In-image label records reached through this owner's literal pool. */
extern u8 Data_020088d4[];
extern u8 Data_020088e0[];
extern u8 Data_020088f8[];

extern void Func_02000af0();
extern s32 Func_02000a28();
extern s32 Func_02000a38();
extern void Func_02000a52();
extern s32 Func_02000a62();
extern void Func_02000aba();
extern void Func_02000ac8();
extern void Func_02000ab4();
extern void Func_02000acc();
extern s32 Func_02000b38();
extern void Func_02000adc();
extern u8 * Func_02000b2a();
extern void Func_02000ae0();
extern void Func_02000b1e();
extern void Func_02000bb6();
extern void Func_02000b1c();
extern s32 Func_02000b7e();
extern void Func_02000bec();
extern void Func_02000bfc();
extern void Func_02000c1a();
extern void Func_02000c32();
extern void Func_02000c4a();
extern void Func_02000c64();
extern void Func_02000c7e();
extern void Func_02000c9a();
extern s32 Func_02000bd6();
extern u8 * Func_02000c60();
extern s32 Func_02000c04();
extern u8 * Func_02000c8e();
extern void Func_02000c26();
extern void Func_02000c76();
extern void Func_02000c34();
extern void Func_02000c4c();
extern void Func_02000c54();
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
    Func_02000af0(0x70);

    window = Func_02000a28(0, 0, 30, 7, 2);
    preview = Func_02000a38(0, 8, 28, 10, 2);

    cursor = 1;
    redraw = 1;

    /* DMA3SAD is rewritten between bursts, so both transfers start there. */
    dma3[0] = 0x05000200;
    dma3[1] = 0x050001c0;
    dma3[2] = 0x80000010;
    dma3[0] = 0x050001e8;
    dma3[1] = 0x050001c0 + 28;
    dma3[2] = 0x80000001;

    Func_02000a52(1);

    for (;;) {
        if (redraw != 0) {
            redraw = 0;
            cursor = Func_02000a62(cursor + 270, 270);

            Func_02000aba(window);
            Func_02000ac8(window);
            Func_02000ab4(Data_020088d4, window, 0, 0);
            Func_02000acc(cursor, 0, window, 0x50, redraw);

            if (Func_02000b38() != 0) {
                masked = cursor & 0x1ff;
                Func_02000adc(Data_020088e0, window, 0, 32);
                Func_02000b2a(masked);
                Func_02000ae0(masked + 0x182, window, 120, 0);
                masked += 0x75;
                Func_02000af0(masked, window, 0, 16);
                Func_02000b1e(preview);
                Func_02000bb6(preview, cursor);
            } else {
                Func_02000b1c(Data_020088f8, window, 0, 32);
            }
        }

        keys = *held;
        if ((keys & 1) != 0) {
            if (Func_02000b7e(cursor) != -1)
                Func_02000bec(0xaf);
        }

        keys = *held;
        if ((keys & 2) != 0) {
            Func_02000bfc(0x71);
            goto close;
        }

        if ((*pressed & 0x40) != 0) {
            cursor -= 1;
            direction = -1;
            redraw = 1;
            Func_02000c1a(0x6f);
        }
        if ((*pressed & 0x80) != 0) {
            cursor += 1;
            direction = 1;
            redraw = 1;
            Func_02000c32(0x6f);
        }
        if ((*pressed & 0x10) != 0) {
            cursor += 10;
            direction = 1;
            redraw = 1;
            Func_02000c4a(0x6f);
        }
        if ((*pressed & 0x20) != 0) {
            cursor -= 10;
            direction = -1;
            redraw = 1;
            Func_02000c64(0x6f);
        }
        if ((*pressed & 0x100) != 0) {
            cursor += 30;
            direction = 1;
            redraw = 1;
            Func_02000c7e(0x6f);
        }
        if ((*pressed & 0x200) != 0) {
            cursor -= 30;
            direction = -1;
            redraw = 1;
            Func_02000c9a(0x6f);
        }

        if (direction == -1) {
            value = cursor + 270;
            for (;;) {
                cursor = Func_02000bd6(value, 270);
                entry = Func_02000c60(cursor & 0x1ff);
                if (*(u16 *)(entry + 6) != 0)
                    break;
                value = cursor + 0x10d;
            }
        }

        if (direction == 1) {
            value = cursor + 270;
            for (;;) {
                cursor = Func_02000c04(value, 270);
                entry = Func_02000c8e(cursor & 0x1ff);
                if (*(u16 *)(entry + 6) != 0)
                    break;
                value = cursor + 0x10f;
            }
        }

        direction = 0;
        Func_02000c26(1);
    }

close:
    Func_02000c76(window);
    Func_02000c34(1);
    Func_02000c4c(window, 1);
    Func_02000c54(preview, 1);
}
