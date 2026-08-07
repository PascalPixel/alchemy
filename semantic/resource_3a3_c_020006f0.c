#include "types.h"

/*
 * Resource 3a3 per-frame actor callback at 0x020006f0 (200 bytes,
 * 0x020006f0 .. 0x020007b7, of which 0x020007ae .. 0x020007b7 is alignment
 * plus the literal pool).
 *
 * Complete owner: `push {r5, r6, r7, lr}` at 0x020006f0 and the interworking
 * return `pop {r5, r6, r7} / pop {r0} / bx r0` at 0x020007a8.  r0 is the
 * popped return address, so the owner returns nothing.
 *
 * This is the callback Func_020007b8 installs on actor 18's record: it stores
 * the pool word 0x020086f1 into +0x6c, which under the proven 0x02008000
 * overlay link base is file offset 0x06f1 = this function plus the Thumb bit.
 * That is also why the Func_080091f0(-1, -1, 0xe666) call below repeats one of
 * Func_020007b8's -- the same effect is re-triggered on a beat.
 *
 * All 6 call sites are placed (row reports calls=6): Func_080091f0 x2,
 * Func_08009080 x2, Func_080f9010 x2.
 *
 * Two halfword cursors are used, at +0x66 and +0x64.  Both are tested with the
 * `lsls r3,#16 / cmp` idiom, which is a comparison on the low 16 bits only:
 * 0x00010000 means "equals 1" and 0x003c0000 means "equals 60".  r6 addresses
 * +0x66 for the first half of the body and is re-pointed at +0x64 for the
 * second; the `strh r3,[r6,#0]` at 0x02000750 therefore writes +0x66, not
 * +0x64.
 */

/* Imports.  Old-style declarations are mandatory in overlay sources. */
void Func_0200159c();
void Func_0200156a();
void Func_02001704();
void Func_020015d8();
void Func_02001740();
void Func_020015cc();
                     
                            /* sets the record's activity mode */

void Func_020006f0(u8 *object)
{
    s16 *beat;              /* object + 0x66 */
    s16 *frame;             /* object + 0x64 */
    s32 pending;
    s32 limit;
    s32 position;
    u8 active;

    beat = (s16 *)(object + 102);

    if (*beat != 0) {
        *(u16 *)beat = (u16)(*(u16 *)beat - 1);
        if ((u16)*beat == 1) {
            Func_0200159c(-1, -1, 0xe666);
        }
    }

    pending = *(s32 *)(object + 40);
    if (pending == 0) {
        Func_0200156a(object, 1);

        limit = *(s32 *)(object + 20);
        position = *(s32 *)(object + 12) + (s32)0xfffe8000;   /* -0x18000 */
        *(s32 *)(object + 12) = position;

        if (position < limit) {
            if (*(s32 *)(object + 104) != 0) {
                Func_02001704(229);
                /* r7 is still the zero read from +0x28. */
                *(u16 *)beat = 4;
                Func_020015d8(0, 0x10000, 0x10000);
                *(s32 *)(object + 104) = 0;
                limit = *(s32 *)(object + 20);
            }
            *(s32 *)(object + 12) = limit;
        }
        active = 1;
    } else {
        active = 0;
    }
    object[91] = active;

    frame = (s16 *)(object + 100);
    if (*frame == 0) {
        Func_02001740(152);
        *(s32 *)(object + 104) = 1;
        Func_020015cc(object, 2);
        *(s32 *)(object + 40) = 0x00030000;     /* `movs r3,#192 / lsls #10` */
    }

    if ((u16)*frame == 60) {
        *(u16 *)frame = 0;
    }
    *(u16 *)frame = (u16)(*(u16 *)frame + 1);
}
