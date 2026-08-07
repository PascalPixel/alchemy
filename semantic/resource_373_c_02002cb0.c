#include "types.h"

/*
 * resource_373 owner at 0x02002cb0, 612 bytes: code 0x02002cb0-0x02002eef and
 * the nine-word literal pool at 0x02002ef0-0x02002f13.  The next inventory row
 * begins at 0x02002f14, so the owner is exactly its advertised span.
 *
 * One scripted scene: it brackets the whole body with the Func_0808a018 /
 * Func_0808a020 pair that resource_39f's scripted owners use, then drives two
 * actors (selector 0 and selector 1) and a third channel (selector 8) through
 * a fixed sequence of pose, scale, position, motion and wait steps, bumping a
 * scene counter once when a query reports state 1.
 *
 * Call accounting: 60 `bl` sites, all resolved with
 * `bun tools/lib/overlay_call_targets.ts resource_373 2cb0` (25 distinct import
 * veneers, no intra-overlay call, no `call_via` slot).  Every site appears
 * below exactly once.  The disassembler's own `bl` annotations are wrong here
 * in the usual way - an overlay `bl` stores `target_offset - 2`, not a
 * pc-relative displacement - and two sites in this owner (0x02002ccc and
 * 0x02002d9c) print the *same* target 0x02008d52 while resolving to different
 * imports (Func_0808a170 and Func_0808a010).
 *
 * The epilogue is `pop {r5} / pop {r0} / bx r0`, so r0 holds the popped return
 * address and the owner is void.  The prologue is `push {r5, lr}` and the very
 * first branch is taken with no argument register written, so the caller's
 * r0-r3 reach Func_0808a018 unchanged; nothing here reads them, so the owner
 * itself takes no arguments.
 *
 * Import identities are the main-image addresses carried in the trailing word
 * of each overlay veneer, the same spelling resource_39f uses; several of them
 * are shared with that overlay and its files fix their shapes:
 *   Func_0808a080(selector)          -> actor record; +8/+12/+16 are the 16.16
 *                                       X/Y/Z and +10/+18 their s16 integer
 *                                       views (resource_39f_c_02000cd0.c,
 *                                       _02001078.c).
 *   Func_0808a090(selector, sx, sz)  -> 16.16 scale pair.
 *   Func_0808a0f0(selector, x, z)    -> 16.16 position.
 *   Func_0808a010(frames)            -> wait.
 *   Func_0808a018()/Func_0808a020()  -> scripted-section brackets.
 *   Func_080770c8(id)                -> the same one-argument service
 *                                       resource_39f_c_02000f94.c calls.
 *
 * UNCERTAINTIES, recorded rather than guessed:
 *  - The pooled word 0x02460000 at 0x02002ef4 is passed as the third argument
 *    of Func_0808a210 beside 199 << 17 = 398.0 and two plain 16.16 values
 *    (-1 and 1), so it is the 16.16 value 582.0.  It is numerically inside the
 *    overlay's own address band, which is why it is called out; the sibling
 *    owner at 0x02002f14 settles it by passing 0x01650000 and 0x02e20000 to
 *    the same import, and 0x02e20000 is not an address on this machine at all.
 *  - The pooled selectors 0x1001 and 0x4008 appear where small selectors 0, 1
 *    and 8 appear elsewhere, i.e. 0x1000 | 1 and 0x4000 | 8; the sibling owner
 *    passes plain 1 and 9 to the same three imports in the same order.  A flag
 *    field packed above the selector is the obvious reading, but nothing here
 *    proves the split, so they are left as literals.
 *  - Func_0808a1b8's third argument is 0 at two sites and a frame count at the
 *    rest, which reads as an immediate-versus-tweened distinction; not proven.
 */

/* The overlay's scene block, reached through the IWRAM pointer at 0x03001ebc.
 * The byte-exact assets/code/resource_373_c_02000eb4.c reads the same u16 at
 * +0x1d8 through the same pointer and advances it by 2; here it advances by 1. */
#define RESOURCE_373_SCENE (*(u8 *volatile *)0x03001ebc)
#define RESOURCE_373_SCENE_COUNTER (*(u16 *)(RESOURCE_373_SCENE + 0x1d8))

/* Old-style declarations are mandatory in overlay sources: one import name can
 * legitimately take different argument counts at different sites. */
void Func_080770c8();
void Func_0808a010();
void Func_0808a018();
void Func_0808a020();
s32 Func_0808a070();
u8 *Func_0808a080();
void Func_0808a090();
void Func_0808a0b8();
void Func_0808a0d0();
void Func_0808a0e8();
void Func_0808a0f0();
void Func_0808a100();
void Func_0808a110();
void Func_0808a130();
void Func_0808a138();
void Func_0808a148();
void Func_0808a158();
void Func_0808a170();
void Func_0808a178();
void Func_0808a180();
void Func_0808a188();
void Func_0808a1b8();
void Func_0808a1e8();
void Func_0808a208();
void Func_0808a210();

void Func_02002cb0(void)
{
    u8 *record;
    /* r5 is loaded once from the pool and reused twice; the second use is
     * `adds r0, r5, #6`. */
    s32 cue = 0x1c45;

    /* No argument register is written before this branch. */
    Func_0808a018();

    Func_0808a158(0, 0);
    Func_0808a148(8, 0, 20);
    Func_0808a170(cue);
    Func_0808a130(8, 2);
    Func_0808a188(8, 0, 20);

    /* 128 << 9 and 128 << 6: 1.0 and 0.125 in 16.16. */
    Func_0808a208(0x10000, 0x2000);
    /* 199 << 17 is 398.0; the pooled 0x02460000 is 582.0. */
    Func_0808a210(0x18e0000, -1, 0x2460000, 1);

    /* 0.8 and 0.4 in 16.16, applied to both actors. */
    Func_0808a090(0, 0xcccc, 0x6666);
    Func_0808a090(1, 0xcccc, 0x6666);

    Func_0808a0d0(0, 420, 608);       /* 210 << 1, 152 << 2 */
    Func_0808a1b8(0, 0xa000, 0);      /* 160 << 8 */
    Func_0808a1b8(8, 0x3000, 0);      /* 192 << 6 */

    record = Func_0808a080(0);
    if (record != 0) {
        /* The 16.16 X and Z words of the queried record, copied onto actor 1. */
        Func_0808a0f0(1, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }

    Func_0808a0d0(1, 402, 608);       /* 201 << 1, 152 << 2 */
    Func_0808a1b8(1, 0xd000, 20);     /* 208 << 8 */
    Func_0808a180(0x1001, 0);
    Func_0808a1b8(8, 0x5000, 20);     /* 160 << 7 */
    Func_0808a110(8, 3);
    Func_0808a180(0x4008, 0);
    Func_0808a100(0, 3);
    Func_0808a110(1, 3);
    Func_0808a010(20);
    Func_0808a138(8, 2);
    Func_0808a178(0x4008, 0);

    if (Func_0808a070(0, 0) == 1) {
        RESOURCE_373_SCENE_COUNTER = RESOURCE_373_SCENE_COUNTER + 1;
        Func_0808a130(8, 1);
    }

    Func_0808a188(0x4008, 0, 40);
    Func_0808a1e8(8, 0x105, 60);
    Func_0808a170(cue + 6);
    Func_0808a188(0x4008, 0, 20);
    Func_0808a138(1, 1);
    Func_0808a010(40);
    Func_0808a188(0x1001, 0, 40);
    Func_0808a138(8, 1);
    Func_0808a1b8(8, 0xd000, 20);     /* 208 << 8 */
    Func_0808a180(0x4008, 0);
    Func_0808a110(1, 3);
    Func_0808a188(0x1001, 0, 120);
    Func_0808a188(0x4008, 0, 20);
    Func_0808a1e8(1, 0x105, 40);
    Func_0808a188(0x1001, 0, 40);
    Func_0808a110(8, 4);
    Func_0808a188(0x4008, 0, 20);
    Func_0808a110(1, 3);
    Func_0808a010(40);
    Func_0808a1b8(8, 0x5000, 20);     /* 160 << 7 */
    Func_0808a188(0x4008, 0, 10);
    Func_0808a100(0, 3);
    Func_0808a110(1, 3);
    Func_0808a010(20);
    Func_0808a110(8, 3);
    Func_0808a100(1, 2);

    record = Func_0808a080(0);
    if (record != 0) {
        /* The s16 integer views at +10 and +18 of the same 16.16 X and Z. */
        Func_0808a0b8(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }

    Func_0808a0e8(1);
    Func_0808a0f0(1, 0, 0);
    Func_080770c8(0x303);

    /* Common exit; no argument registers are set. */
    Func_0808a020();
}
