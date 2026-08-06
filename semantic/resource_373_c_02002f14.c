#include "types.h"

/*
 * resource_373 owner at 0x02002f14, 672 bytes: code 0x02002f14-0x0200318f and
 * the nine-word literal pool at 0x02003190-0x020031b3.  The next row begins at
 * 0x020031b4, which already has byte-exact C, so the owner is exactly its
 * advertised span.
 *
 * A sibling of the scene at 0x02002cb0 and structurally almost the same: the
 * Func_0808a018 / Func_0808a020 brackets, the same camera placement, the same
 * two record queries around actor selector 1, the same scene-counter bump, the
 * same closing Func_080770c8 cue.  It differs in its actor set (0, 1 and 9
 * rather than 0, 1 and 8) and in the branch polarity of the query at
 * 0x020030cc.
 *
 * Call accounting: 64 `bl` sites, all resolved with
 * `bun tools/overlay_call_targets.ts resource_373 2f14` (21 distinct import
 * veneers, no intra-overlay call, no `call_via` slot).  Each site appears
 * below exactly once.  The disassembler's `bl` annotations are unusable here
 * for the usual reason - an overlay `bl` stores `target_offset - 2` - and this
 * owner shows it plainly: 0x02003088 and 0x02003188 both print 0x0200914e
 * while resolving to Func_0808a1e8 and Func_0808a020.
 *
 * The epilogue is `pop {r0} / bx r0`, so r0 is the popped return address and
 * the owner is void.  The prologue is `push {lr}` with no frame; the first
 * branch is taken with no argument register written, so the caller's r0-r3
 * reach Func_0808a018 unchanged and nothing here reads them.  The owner takes
 * no arguments.
 *
 * Import shapes are as recorded in resource_373_c_02002cb0.c and in the
 * resource_39f sources that share this family.  Two readings this owner
 * sharpens:
 *  - Func_0808a210 takes 16.16 values, not addresses: 0x01650000 is 357.0 and
 *    0x02e20000 is 738.0, and 0x02e20000 is not a valid address on this
 *    machine.  The neighbouring Func_0808a0d0 call passes 0x016f (367) and
 *    0x02e9 (745) - the same placement in whole units.
 *  - Func_0808a1e8's second argument is a pooled or built small constant
 *    (0x100, 0x102, 0x103, 0x105) with a frame count third; it behaves as a
 *    keyed animation request.  Not proven beyond the shape.
 *
 * UNCERTAINTY: Func_0808a1b8's third argument is 0 at three sites here and a
 * frame count at the rest, the same immediate-versus-tweened split the sibling
 * owner shows.  Nothing in either owner proves it.
 */

/* The overlay's scene block, reached through the IWRAM pointer at 0x03001ebc.
 * The byte-exact assets/code/resource_373_c_02000eb4.c reads the same u16 at
 * +0x1d8 through the same pointer. */
#define RESOURCE_373_SCENE (*(u8 *volatile *)0x03001ebc)
#define RESOURCE_373_SCENE_COUNTER (*(u16 *)(RESOURCE_373_SCENE + 0x1d8))

/* Old-style declarations are mandatory in overlay sources: one import name can
 * legitimately take different argument counts at different sites. */






















extern void Func_02008ed4();
extern void Func_0200900a();
extern void Func_02008f54();
extern void Func_02008fe8();
extern u8 * Func_02008f26();
extern void Func_02008f8c();
extern void Func_02008f80();
extern void Func_02009014();
extern void Func_02008ff2();
extern void Func_02008fe2();
extern void Func_0200904e();
extern void Func_02009042();
extern void Func_0200905a();
extern void Func_02009054();
extern void Func_02009004();
extern void Func_02009034();
extern void Func_02009088();
extern void Func_02009082();
extern void Func_0200904a();
extern void Func_020090bc();
extern void Func_0200909e();
extern void Func_02009056();
extern void Func_020090b0();
extern void Func_020090cc();
extern void Func_020090d8();
extern void Func_02009080();
extern void Func_020090d0();
extern void Func_020090f4();
extern void Func_02009100();
extern void Func_020090c0();
extern void Func_02009016();
extern void Func_02009108();
extern void Func_0200911a();
extern void Func_0200914e();
extern void Func_0200915a();
extern void Func_0200913c();
extern void Func_0200910c();
extern void Func_0200916a();
extern void Func_02009152();
extern s32 Func_020090aa();
extern void Func_020091a0();
extern void Func_02009194();
extern void Func_020091b0();
extern void Func_0200918e();
extern void Func_020091a6();
extern void Func_02009166();
extern void Func_020091c0();
extern void Func_02009178();
extern void Func_020090e6();
extern void Func_020091ea();
extern void Func_02009192();
extern void Func_0200919a();
extern u8 * Func_02009140();
extern void Func_0200917a();
extern void Func_020091a8();
extern void Func_020091ba();
extern void Func_0200912a();
void Func_02002f14(void)
{
    u8 *record;

    /* No argument register is written before this branch. */
    Func_02008ed4();

    /* 357.0 and 738.0 in 16.16. */
    Func_0200900a(0x1650000, -1, 0x2e20000, 1);

    Func_02008f54(0, 0x16f, 0x2e9);
    Func_02008fe8(0, 0xa000, 0);        /* 160 << 8 */

    record = Func_02008f26(0);
    if (record != 0) {
        Func_02008f8c(1, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }

    Func_02008f80(1, 346, 0x2e9);       /* 173 << 1 */
    Func_02009014(1, 0xd000, 20);       /* 208 << 8 */
    Func_02008ff2(0x1c53);
    Func_0200900a(1, 0);
    Func_02008fe2(9, 2);
    Func_0200904e(9, 0x100, 0);         /* 128 << 1 */
    Func_02009042(9, 0x3000, 10);       /* 192 << 6 */
    Func_0200904e(9, 0x5000, 10);       /* 160 << 7 */
    Func_0200905a(9, 0x3000, 40);
    Func_02009054(9, 0, 20);
    Func_02009004(0, 3);
    Func_02009014(1, 3);
    Func_02009034(9, 1);
    Func_02009088(9, 0x5000, 10);
    Func_02009082(9, 0, 20);
    Func_0200904a(1, 1);
    Func_020090bc(1, 0x103, 40);
    Func_0200909e(1, 0, 10);
    Func_02009056(9, 3);
    Func_020090b0(9, 0, 20);
    Func_020090cc(0, 0x8000, 0);        /* 128 << 8 */
    Func_020090d8(1, 0x1000, 40);       /* 128 << 5 */
    Func_02009080(9, 4);
    Func_020090d0(9, 0);
    Func_020090f4(0, 0xb000, 0);        /* 176 << 8 */
    Func_02009100(1, 0xd000, 10);
    Func_020090c0(1, 2);
    Func_02009016(20);
    Func_02009108(1, 0, 10);
    Func_020090c0(9, 3);
    Func_0200911a(9, 0, 20);
    Func_0200914e(0, 0x102, 0);         /* 129 << 1 */
    Func_0200915a(1, 0x102, 80);
    Func_0200913c(1, 0, 20);
    Func_0200910c(9, 2);
    Func_0200914e(9, 0, 20);
    Func_0200916a(1, 0x1000, 20);
    Func_02009152(1, 0);

    /* `cmp r0, #0 / bne` - the counter runs on the nonzero result here, the
     * mirror of the `== 1` test the sibling owner uses. */
    if (Func_020090aa(0, 0) == 0) {
        Func_020091a0(1, 0x105, 60);
        RESOURCE_373_SCENE_COUNTER = RESOURCE_373_SCENE_COUNTER + 1;
    } else {
    }

    Func_02009194(1, 0, 20);
    Func_020091b0(1, 0xd000, 10);
    Func_0200918e(0x1c60);
    Func_020091a6(1, 0);
    Func_02009166(9, 3);
    Func_020091c0(9, 0, 20);
    Func_02009178(1, 3);
    Func_020090e6(20);
    Func_020091ea(1, 0x1000, 20);
    Func_02009192(1, 3);
    Func_0200919a(0, 3);
    Func_0200919a(1, 2);

    record = Func_02009140(0);
    if (record != 0) {
        /* The s16 integer views at +10 and +18 of the 16.16 X and Z. */
        Func_0200917a(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }

    Func_020091a8(1);
    Func_020091ba(1, 0, 0);
    Func_0200912a(772);                 /* 193 << 2 */

    /* Common exit; no argument registers are set. */
    Func_0200914e();
}
