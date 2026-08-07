#include "types.h"

/*
 * resource_3b2 owner at 0x0200167c, 196 bytes: code 0x0200167c-0x02001729,
 * a two-byte alignment `movs r0, r0` at 0x0200172a, and the five-word literal
 * pool 0x0200172c-0x0200173f.
 *
 * Complete owner: `push {lr}` prologue, an 8-byte outgoing-argument frame, and
 * the single epilogue at 0x02001724 ending `pop {r0} / bx r0` — so the owner is
 * **void**.
 *
 * Call targets resolved with `bun tools/lib/overlay_call_targets.ts resource_3b2
 * 167c --json` (13 sites, 4 distinct targets, all import veneers):
 *   0x02001682, 0x020016ba, 0x020016de, 0x02001702 -> 0x02003058 Func_080770c0
 *   0x0200169a                                     -> 0x02003028 Func_080091c0
 *   0x020016a8, 0x020016cc, 0x020016f0, 0x02001714 -> 0x020030b8 Func_0808a0f0
 *   0x020016b0, 0x020016d4, 0x020016f8, 0x0200171c -> 0x02003088 Func_0808a080
 *
 * Behaviour: the scene's party-placement pass.  For each of the four companion
 * ids 0x08c4..0x08c7 (mapping to slots 15..18 in order): if that companion is
 * in the party, cue its slot off-stage at (968.0, 968.0); otherwise fetch the
 * slot and park its field at +28 at the constant 0x00019999.  The very first
 * present-companion arm additionally repaints one collision rectangle.
 *
 * The coordinate 0x03c80000 is built as `242 << 18`, i.e. 968.0 in 16.16 —
 * far outside any playfield, the usual "hide this slot" position.
 *
 * The four blocks are written out rather than looped so that the call multiset
 * matches the assembly exactly (4 + 4 + 4 + 1 = 13 sites).
 *
 * UNCERTAINTY: field +28 of a scene entity and the constant 0x00019999 (1.6 in
 * 16.16) are not otherwise identified; only that the same constant is stored by
 * all four absent-companion arms.
 */

/* Old-style declarations: overlay imports vary in arity between call sites. */
u8 *Func_080770c0();                    /* companion entity by selector, or 0 */
u8 *Func_0808a080();                    /* scene entity by selector */
void Func_0808a0f0();                   /* cue a slot to (x, z) in 16.16 */
void Func_080091c0();                   /* collision repaint, six arguments */

#define OFFSTAGE  (242 << 18)           /* 968.0 in 16.16 */
#define PARKED    0x00019999

void Func_0200167c(void)
{
    if (Func_080770c0(0x8c4) != 0) {
        Func_080091c0(0, 0, 1, 1, 8, 21);
        Func_0808a0f0(15, OFFSTAGE, OFFSTAGE);
    } else {
        *(s32 *)(Func_0808a080(15) + 28) = PARKED;
    }

    if (Func_080770c0(0x8c5) != 0) {
        Func_0808a0f0(16, OFFSTAGE, OFFSTAGE);
    } else {
        *(s32 *)(Func_0808a080(16) + 28) = PARKED;
    }

    if (Func_080770c0(0x8c6) != 0) {
        Func_0808a0f0(17, OFFSTAGE, OFFSTAGE);
    } else {
        *(s32 *)(Func_0808a080(17) + 28) = PARKED;
    }

    if (Func_080770c0(0x8c7) != 0) {
        Func_0808a0f0(18, OFFSTAGE, OFFSTAGE);
    } else {
        *(s32 *)(Func_0808a080(18) + 28) = PARKED;
    }
}
