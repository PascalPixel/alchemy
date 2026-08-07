#include "types.h"

/*
 * resource_3ba owner at 0x02000840, 276 bytes (0x02000840-0x02000953):
 * 264 bytes of code plus the three-word literal pool at 0x02000948, which ends
 * where the next owner (0x02000954, byte-exact in assets/code) begins.
 *
 * Prologue `push {r5, r6, lr}`, no frame; epilogue
 * `pop {r5, r6} / pop {r0} / bx r0` at 0x02000942.  r0 holds the popped return
 * address, so the owner returns nothing, and no argument register is read
 * before being written, so it takes none.
 *
 * CALL ACCOUNTING.  Twenty-six `bl` sites, all resolved with
 * `bun tools/lib/overlay_call_targets.ts resource_3ba --annotate`:
 *   Func_0808a100 x4, Func_0808a090 x4, Func_0808a010 x3,
 *   Func_0808a0c8 x2, Func_0808a0d0 x2,
 *   and one each of Func_02002f90, Func_0808a018, Func_020030ac,
 *   Func_02002fa0, Func_0808a0a0, Func_0808a150, Func_0808a110,
 *   Func_0808a250, Func_0808a260, Func_0808a268 and Func_080770c8.
 * The C below reproduces that multiset exactly (the Func_0808a0a0 site is
 * written once, inside its loop).  The inventory's `calls=24` is the usual
 * low lower bound and is not used as the proof.
 *
 * THE FIRST `bl` IS NOT AN INDIRECT CALL.  `overlay_call_targets.ts`
 * classifies 0x02002f90 as `call_via`, because the bank test matches any
 * halfword of the `bx rN` shape.  Nothing here loads r3 or r4, and the target's
 * own byte-exact source `assets/code/resource_3ba_c_02002f90.c` is an empty
 * `void Func_02002f90(void) {}` — a genuine no-op hook, exactly the collision
 * HANDOVER records for resource_3bb:3228.
 *
 * LINK BASE 0x02008000, proven for this overlay by the byte-exact sibling
 * `assets/code/resource_3ba_c_02000158.c` (pool word 0x0200804d =
 * Func_0200004c + the Thumb bit).  The pool word 0x02000240 here is *below*
 * that base, so it is the shared RAM table the byte-exact resource_36f and
 * resource_371 sources declare as `s16 Data_02000240[]`; this owner writes the
 * byte at +0x22b in it.  0x0000022b and 0x0000008f are plain constants.
 *
 * SHAPE.  A scripted scene: enter presentation mode, run the setup routine at
 * 0x020030ac and wait on the overlay's control word (0x02002fa0 spins until
 * Data_02001000 reads 9), then ten repeats of one cue, and a run of paired
 * camera/actor commands for the two participants (selectors 0 and 8) with two
 * waits.  It closes by advancing an actor group relative to the setup
 * routine's return value, marking the shared RAM byte with 3, issuing two
 * cue-pair commands for id 0x8f, and setting story flag 0x11a.
 *
 * UNCERTAINTIES.
 *  - The counted loop is `r5 = 9; do { call; r5--; } while (r5 >= 0)` with the
 *    decrement before the call, so it runs ten times; that is preserved below.
 *  - Func_020030ac's return value is used only as `1 - value` in the
 *    Func_0808a250 argument; its meaning is not asserted.  It is an ordinary
 *    intra-overlay prologue, taking (120, 127) here.
 *  - The 0x02000240 byte at +0x22b is written but never read in this owner.
 */

/* Import veneers, named by the main-image function each one reaches.
 * Old-style declarations: arities vary between call sites in this overlay. */
void Func_020037d4(void);
void Func_020044e8();
s32 Func_020038fc();
void Func_020037f6(void);
void Func_02004530();
void Func_0200453a();
void Func_0200455e();
void Func_02004554();
void Func_02004580();
void Func_020045a0();
void Func_020045d2();
void Func_02004548();
void Func_020045b8();
void Func_020045c8();
void Func_0200459c();
void Func_020045aa();
void Func_020045ce();
void Func_020045e2();
void Func_02004602();
void Func_0200460a();
void Func_020045a8();
void Func_020046ac();
void Func_020046c8();
void Func_020046d8();
                                /* enter cutscene presentation mode */
                                /* wait n frames */

                     

                     

                     
                     
                                /* set a story flag */

/* This overlay's own routines. */
                                /* byte-exact: an empty hook */
                                /* byte-exact: spin until the control word is 9 */

void Func_02000840(void)
{
    s32 setup;
    s32 i;

    Func_020037d4();
    Func_020044e8();

    setup = Func_020038fc(120, 127);

    Func_020037f6();

    for (i = 9; i >= 0; i--) {
        Func_02004530(8);
    }

    Func_0200453a(8, 0x10000, 0x8000);          /* 128 << 9, 128 << 8 */
    Func_0200455e(8, 0x528, 192);               /* 165 << 3 */
    Func_02004554(0, 0x10000, 0x8000);
    Func_02004580(0, 0x508, 192);               /* 161 << 3 */
    Func_020045a0(8, 1);
    Func_020045d2(0, 8, 0);
    Func_02004548(10);
    Func_020045b8(8, 3);
    Func_020045c8(0, 3);
    Func_0200455e(20);

    Func_0200459c(0, 0x20000, 0x10000);         /* 128 << 10, 128 << 9 */
    Func_020045aa(8, 0x20000, 0x10000);
    Func_020045ce(0, 0x510, 192);               /* 162 << 3 */
    Func_020045e2(8, 0x520, 192);               /* 164 << 3 */
    Func_02004602(0, 16);
    Func_0200460a(8, 9);
    Func_020045a8(10);

    Func_020046ac(72, 1 - setup);

    *(u8 *)(0x02000240 + 0x22b) = 3;

    Func_020046c8(0x8f, 4);
    Func_020046d8(0x8f, 5);

    Func_020045a0(0x11a);                       /* 141 << 1 */
}
