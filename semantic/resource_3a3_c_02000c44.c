#include "types.h"

/*
 * Resource 3a3 per-frame object callback at 0x02000c44 (196 bytes,
 * 0x02000c44 .. 0x02000d07, of which 0x02000cf8 .. 0x02000d07 is the pool).
 *
 * Complete owner: `push {r5, r6, lr}` at 0x02000c44 and the interworking
 * return `pop {r5, r6} / pop {r0} / bx r0` at 0x02000cf2.  r0 is the popped
 * return address, so the owner returns nothing.
 *
 * This is the callback installed by Func_02000d08, which stores 0x02008c45
 * (= this function's file offset 0x0c45 under the proven 0x02008000 link base,
 * plus the Thumb bit) into the spawned object's +0x6c slot.  The object is the
 * one Func_02000c0c initializes, so the +0x18/+0x1c pair starts at 0x4ccc and
 * the +0x64 halfword and +0x68 counter start at 20.
 *
 * All 6 call sites are placed (row reports calls=6): Func_080000f8 x4,
 * Func_0808a160 x1, Func_080090d0 x1.  Func_080000f8 is the random source; its
 * result is used four times with different shift/mask windows, and it is
 * re-called each time rather than cached.
 *
 * Uncertainties, all noted where they occur: the sign/zero extension windows
 * are transcribed exactly (`lsls #16 / asrs #1` is a sign extension followed
 * by a multiply-by-0x8000, not a plain shift), and r2 is deliberately reloaded
 * from +0x64 after Func_0808a160 because that call clobbers it.
 */

/* Imports.  Old-style declarations are mandatory in overlay sources. */
u32 Func_080000f8();        /* random source */
void Func_0808a160();
void Func_080090d0();       /* releases / deactivates the object */

void Func_02000c44(u8 *object)
{
    s16 *phase;             /* object + 0x64 */
    s32 step;
    u32 roll;
    s32 next;

    phase = (s16 *)(object + 100);

    /*
     * `lsls r0,#1 / lsrs r0,#16` takes bits 15..1 of the roll into the low
     * halfword; `subs #1 / lsls #16 / asrs #1` then sign-extends that halfword
     * and scales it by 0x8000.
     */
    roll = (Func_080000f8() << 1) >> 16;
    step = (s32)(s16)(roll - 1) << 15;
    *(s32 *)(object + 8) += ((s32)*phase << 12) + step;

    if (*phase > 3) {
        *(s32 *)(object + 16) += 0x00020000;    /* `movs r2,#128 / lsls #10` */
        *(s32 *)(object + 28) += 0x000007ae;
        *(s32 *)(object + 24) += 0x000007ae;
    } else {
        /* `lsls r0,#15 / lsrs r0,#16` keeps bits 16..1 of the roll. */
        roll = (Func_080000f8() << 15) >> 16;
        *(s32 *)(object + 16) = (s32)(*(s32 *)(object + 16) - (s32)roll)
                                + (s32)0xffff0000;
        *(s32 *)(object + 24) += 0x00002666;
        *(s32 *)(object + 28) += (s32)0xfe80f5c4;
    }

    /* A 1-in-N test scaled by the current phase; when it comes up zero the
     * object is retargeted. */
    if ((u32)((s32)*phase * (s32)Func_080000f8()) >> 16 == 0) {
        Func_0808a160(object, 7);
    }

    /*
     * The `lsls r3,r2,#16 / cmp #0` test is on the halfword's low 16 bits, so
     * a nonzero phase simply counts down; a zero phase draws a new odd-ish
     * value 2*((roll*5) >> 16) + 2.  r2 is reloaded from +0x64 after the
     * Func_0808a160 call above, which clobbers it.
     */
    if ((u16)*phase != 0) {
        next = (s32)((((roll << 2) + roll) >> 16) << 1) + 2;
        next = (s32)(u16)*phase - 1;
    } else {
        roll = Func_080000f8();
    }
    *(u16 *)(object + 100) = (u16)next;

    *(s32 *)(object + 104) -= 1;
    if (*(s32 *)(object + 104) == 0) {
        *(s32 *)(object + 108) = 0;
        Func_080090d0(object);
    }
}
