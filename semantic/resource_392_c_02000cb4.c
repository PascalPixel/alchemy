#include "types.h"

/*
 * Resource 392 scene-entry setup at 0x02000cb4 (216 bytes, 0x02000cb4 ..
 * 0x02000d8b, of which 0x02000d84 .. 0x02000d8b is the literal pool).
 *
 * Complete owner: `push {r5, r6, r7, lr}` plus the r8/r9/sl spill at
 * 0x02000cb6 .. 0x02000cbc, and the matching restore and interworking return
 * `pop {r3, r5, r6} / ... / pop {r0} / bx r0` at 0x02000d76.  r0 is the popped
 * return address, so the owner returns nothing and takes no arguments -- the
 * first `bl` is reached with r0 never set.
 *
 * TWIN.  The same 216 bytes appear in six overlays (tools-rs/overlay-twins):
 * resource_373:5b48, resource_38e:0974, resource_392:0cb4, resource_39f:2c0c,
 * resource_3a8:3a3c and resource_3b2:2ed8.  Converting it once settles all six.
 *
 * Three constants live in callee-saved registers across the body -- r8 = 0,
 * r9 = 1, and sl = the address of the byte at +92 -- so each is a
 * function-scope named local here rather than a repeated literal, which is
 * what puts them in callee-saved registers at all.
 *
 * The pool words are 0x00000109 (an id passed to Func_02001b0c, too large for
 * a Thumb immediate) and 0x02008c4d.  The latter is ODD, so under the proven
 * 0x02008000 overlay link base it is `Func_02000c4c + 1` -- an in-image
 * function pointer stored into the object at +108, this scene's per-frame
 * callback, the same +108 callback slot resource_3a3_c_02000d08.c documents.
 *
 * Per-target multiset over the 7 call sites, matching the row's calls=7:
 * Func_02001af4 x1 (fetch the current scene object), Func_02001ae6 x1,
 * Func_02001b0c x1, Func_02001aec x1, Func_02001b3c x1, Func_02001b14 x1 and
 * Func_02001b12 x1.
 *
 * RESIDUE (9 of 108 halfwords, 2026-08-07).  Size and every instruction match
 * except three one-slot scheduling swaps: the reference issues `ldrb r1,[r6,#5]'
 * one slot before `movs r3,#33', `movs r2,#1' one slot before the +35 store, and
 * `movs r1,#128' before `ldrb r0,[r6,#28]'.  Neither the 979-configuration mode
 * cohort (singles plus 900 pairs) nor any of eleven source orderings of the two
 * mask chains moves them, so the remaining difference is scheduler placement,
 * not shape.
 *
 * The two masking chains each have to start at their constant or at the loaded
 * value in exactly the order below: `mode = ~12; mode &= state[9]; mode |= 4;'
 * keeps -13 as the accumulating register, while writing the second mask as
 * `lock = state[5]; lock &= ~32;' (value first) is what stops cse deriving -33
 * from the live 4 as `subs r3, #37' -- 90 differing halfwords with the constant
 * first, 9 this way.
 *
 * Old-style declarations: the overlay imports vary their argument count
 * between call sites.
 */

extern u8 *Func_02001af4();
extern void Func_02001ae6();
extern s32 Func_02001b0c();
extern s32 Func_02001aec();
extern void Func_02001b3c();
extern void Func_02001b14();
extern void Func_02001b12();

void Func_02000cb4(void)
{
    u8 *object;
    u8 *state;
    u8 *flagAt92;
    s32 zero;
    s32 one;
    s32 amount;
    s32 mode;
    s32 lock;

    object = Func_02001af4();
    state = *(u8 **)(object + 80);

    mode = ~12;
    mode &= state[9];
    mode |= 4;
    lock = state[5];
    lock &= ~32;
    state[5] = (u8)lock;
    mode &= 15;
    state[9] = (u8)mode;

    zero = 0;
    state[39] = (u8)zero;

    /* r0 still holds the object returned above. */
    Func_02001ae6(object, 0);

    flagAt92 = object + 92;
    *flagAt92 = (u8)zero;
    object[85] = (u8)zero;

    if (Func_02001b0c(0x109) == 0) {
        *(s32 *)(object + 12) += 0x00200000;
    }

    object[35] = (u8)(object[35] & ~1);

    one = 1;
    object[97] = (u8)one;

    amount = Func_02001aec(17, 0x608);
    Func_02001b3c(181);
    amount += 0x400;

    Func_02001b14(state[28], 128, amount);
    Func_02001b12(17);

    *(s32 *)(object + 56) = *(s32 *)(object + 8);
    *(s32 *)(object + 48) = zero;
    *(s32 *)(object + 60) = *(s32 *)(object + 12);

    *flagAt92 = (u8)one;
    *(u32 *)(object + 108) = 0x02008c4d;
    object[86] = (u8)zero;
}
