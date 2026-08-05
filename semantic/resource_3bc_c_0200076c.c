#include "types.h"

/*
 * Resource 3bc, owner at 0x0200076c.  692 strict bytes: 660 bytes of code
 * (0x076c-0x0a03) and a seven-word literal pool at 0x0a04-0x0a1f.
 *
 * 42 call sites resolved with tools/overlay_call_targets.ts (38 veneers, four
 * internal prologues) — exactly the row's calls=42.  Per-target multiset,
 * reproduced verbatim by the C below:
 *   0808a080 x2, 08000128 x1, 02000714 x2, 080091c0 x6, 0808a018 x1,
 *   0808a100 x3, 0808a010 x4, 08009080 x5, 08009150 x2, 08000148 x1,
 *   080090e0 x1, 0808a090 x1, 080f9010 x5, 08009158 x3, 080770c8 x1,
 *   0808a0e0 x1, 0808a020 x1, 02004628 x1, 020005e0 x1  =  42.
 *
 * Two of the four internal callees are already byte-exact in assets/code and
 * their signatures are taken from there rather than rederived:
 *   assets/code/resource_3bc_c_02000714.c — `s32 Func_02000714(s32 x, s32 z)`,
 *     a four-probe collision test returning -1 on a hit;
 *   assets/code/resource_3bc_c_020005e0.c — `void Func_020005e0(void)`, and
 *     it also supplies the field names used here (mode at +0x23, state at
 *     +0x55) and the >>20 tile idiom.
 *
 * Pool words: 0x0200cc38 is in-image data at file offset 0x4c38 (even, so
 * data by the parity rule) — an 8-entry u16 table indexed by
 * `attributes >> 13`, i.e. the packed-heading table.  0xfff00000 is the
 * tile-cell mask, 0x03001ae8 an IWRAM flag word, and 0x3333 / 0x0ccc / 0x0369
 * are plain constants.
 *
 * Shift-built constants written as values: 0x80 << 12 = 0x80000,
 * 0x80 << 13 = 0x100000, 0x80 << 14 = 0x200000, 0x80 << 8 = 0x8000,
 * 166 << 19 = 0x5300000, 144 << 1 = 288, 240 << 1 = 480.
 *
 * Shared-call-site care: the two stepping loops each have their own
 * Func_02000714 site, but they converge on ONE join at 0x02000862.  That join
 * is written once, after the if/else, rather than duplicated per arm — the
 * documented multiset-inflation trap.  Likewise the tail from 0x020009de is a
 * single shared tail for both branches of the 0x5300000 test.
 *
 * Epilogue `add sp,#32 / pop {r3,r5,r6,r7} / mov r8..fp / pop {r5,r6,r7} /
 * pop {r0} / bx r0` — r0 is the popped return address, so the owner is void.
 *
 * Uncertainties:
 *  - Func_08000128 is given a three-word scratch buffer built on this frame
 *    ({(x & ~0xFFFFF) + 0x80000, field0C, (z & ~0xFFFFF) + 0x80000}); only its
 *    first and third words are read back, so the buffer's real width is not
 *    proven to be three.
 *  - the 0x03001ae8 bits 0x20 / 0x10 read as "step left" / "step right"; the
 *    naming is inferred from the -0x100000 / +0x100000 steps, not established.
 */

typedef struct Object {
    u8 filler00[6];
    u16 attributes;
    s32 x;              /* 16.16 */
    s32 field0C;
    s32 z;              /* 16.16 */
    u8 filler14[0x0F];
    u8 mode;            /* +0x23 */
    u8 filler24[0x0C];
    s32 field30;
    s32 field34;
    u8 filler38[0x1D];
    u8 state;           /* +0x55 */
} Object;

extern s32 Data_02000240[];       /* viewed as s32 here; byte offset 500 */
extern u16 Data_0200cc38[];       /* in-image heading table at file offset 0x4c38 */
extern s32 Data_03001ae8;

Object *Func_0808a080();
void Func_08000128();
s32 Func_02000714();
void Func_080091c0();
void Func_0808a018();
void Func_0808a100();
void Func_0808a010();
void Func_08009080();
void Func_08009150();
s32 Func_08000148();
void Func_080090e0();
void Func_0808a090();
void Func_080f9010();
void Func_08009158();
void Func_080770c8();
void Func_0808a0e0();
void Func_0808a020();
void Func_02004628();
void Func_020005e0();

void Func_0200076c(void)
{
    s32 probe[3];
    Object *actor;
    Object *walker;
    s32 sceneId;
    s32 x;
    s32 z;
    s32 delta;
    s32 next;
    s32 steps;
    s32 stride;
    s32 mode;
    s32 handle;

    sceneId = Data_02000240[125];         /* byte offset 500 */
    actor = Func_0808a080(sceneId);
    walker = Func_0808a080(31);

    steps = 0;
    probe[1] = actor->field0C;
    probe[2] = (actor->z & 0xFFF00000) + 0x80000;
    Func_08000128(0x100000, Data_0200cc38[actor->attributes >> 13], probe);
    probe[0] = (actor->x & 0xFFF00000) + 0x80000;

    x = walker->x;
    z = walker->z;

    delta = probe[0] - x;
    if (delta < 0) {
        delta = x - probe[0];
    }
    if (delta > 0x80000) {
        goto missed;
    }

    delta = probe[2] - z;
    if (delta >= 0) {
        if (delta > 0x200000) {
            goto missed;
        }
    } else {
        if (z - probe[2] > 0x200000) {
            goto missed;
        }
    }

    if ((Data_03001ae8 & 0x20) != 0) {
        stride = -8;
        for (;;) {
            steps++;
            next = x - 0x100000;
            if (Func_02000714(next, z) != 0) {
                break;
            }
            x = next;
        }
    } else if ((Data_03001ae8 & 0x10) != 0) {
        mode = 2;
        mode = 3;
        stride = 8;
        for (;;) {
            next = x + 0x100000;
            if (Func_02000714(next, z) != 0) {
                break;
            }
            steps++;
            x = next;
        }
    } else {
        return;
    }

    if (steps == 0) {
        return;
    }

    Func_080091c0(74, 8, 1, 4, walker->x >> 20, 9);
    Func_080091c0(120, 60, 8, 5, 74, 60);
    Func_0808a018();
    Func_0808a100(sceneId, 8);
    Func_0808a010(6);

    walker->field34 = 0x3333;
    walker->field30 = 0x8000;
    Func_08009080(walker, mode);
    Func_08009150(walker, x, 0, z);
    Func_0808a010(6);
    Func_0808a100(sceneId, 2);

    handle = Func_08000148(27, 0xCCC);
    Func_080090e0(*(s32 *)((u8 *)handle + 480), walker);
    Func_0808a090(sceneId, 0x8000, 0x3333);
    Func_080f9010(239);

    Func_08009080(actor, 2);
    Func_08009150(actor, actor->x + ((steps * stride) << 16), 0, actor->z);
    Func_08009158(actor);
    Func_08009080(actor, 1);
    Func_08009158(walker);

    if (x >= 0x5300000) {
        Func_080770c8(0x369);
        Func_0808a100(31, 3);
        Func_0808a0e0(31, 18, 6);
        Func_0808a010(30);
        Func_08009080(walker, 8);
        Func_08009158(walker);
        walker->mode = 2;
        Func_080091c0(86, 10, 1, 2, 84, 10);
        Func_080091c0(86, 9, 1, 1, 84, 12);
        Func_080f9010(288);
        Func_080f9010(240);
    } else {
        Func_08009080(walker, 1);
        Func_080f9010(288);
        Func_080f9010(213);
        Func_080091c0(85, 9, 1, 4, x >> 20, 9);
        Func_080091c0(85, 9, 1, 4, x >> 20, 61);
    }

    Func_0808a010(15);
    Func_0808a020();
    return;

missed:
    Func_02004628();
    Func_020005e0();
}
