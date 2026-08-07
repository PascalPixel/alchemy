#include "types.h"

/*
 * resource_3c1 owner at 0x02000120, 116 bytes: place a scripted prop at the
 * coordinates the current map slot names, sound the placement, shake the view,
 * arm the workspace counter and hand the slot on.
 *
 * Reference member of a two-owner pair: 0x02000120 and 0x02000194 are
 * byte-identical over all 52 halfwords except the single pool word naming the
 * script (0x020085e8 against 0x020085fe).  Their BL halfwords are
 * bit-identical, so under the +2 rule the four call sites are provably the
 * same.  Found by the sort-by-span scan.
 *
 * Complete owner: 'push {r5, r6, lr}' plus the 'push {r5, r6}' banking r8/sl,
 * and the single interworking epilogue at 0x0200017c.  Control-flow walk: pure
 * straight line, so the three-word pool at 0x02000188 is everything after the
 * 'bx r0'.  104 code + 12 pool = 116, the advertised span.
 *
 * Return type from the epilogue rule: 'pop {r0} ; bx r0', so the owner is void.
 *
 * Calls: 4 sites over 4 targets, from
 * 'bun tools/lib/overlay_call_targets.ts resource_3c1 0120'.
 *
 * Link base: both 0x020085d0 and 0x020085e8 are EVEN, which under the proven
 * 0x02008000 base makes them in-image DATA at file offsets 0x05d0 and 0x05e8 -
 * not RAM globals and not callbacks.  The parity test settles that in one bit.
 * The 24 bytes between them are exactly the six four-byte coordinate entries
 * the slot index selects from.
 *
 * The coordinate read is 'ldrsh' followed by 'lsls #16 / lsrs #16', which is a
 * sign-extending load immediately truncated back to 16 bits - i.e. the entries
 * are consumed UNSIGNED.  Both halves are kept here; writing only the unsigned
 * load would drop the fact that the table is declared signed.
 *
 * UNCERTAINTY: the workspace slot at +456 is set to 16 alongside the shake, and
 * 0x0200022c sets the same slot to 24.  It reads as a duration in frames, but
 * nothing here proves the unit.
 */

/* Pointer CELL holding the scene workspace - one dereference, not two. */
#define PROP3C1_WORKSPACE_CELL ((u8 **)0x03001ebc)

/* In-image, at file offset 0x05d0: six {x, z} pairs indexed by the map slot. */
struct Prop3c1Placement {
    s16 x;
    s16 z;
};

#define PROP3C1_PLACEMENTS ((const struct Prop3c1Placement *)0x020085d0)

/* In-image, at file offset 0x05e8: this owner's placement script. */
#define PROP3C1_SCRIPT ((const void *)0x020085e8)

void Func_080f9010();               /* play a sound effect */
void Func_08009178();               /* place a scripted prop (script, x, z) */
void Func_0808a578();               /* shake the view (x, y, amount) */
void Func_0808a248();               /* hand the map slot on */

void Func_02000120(void)
{
    u8 *workspace = *PROP3C1_WORKSPACE_CELL;
    s32 slot = *(s16 *)(workspace + 364);
    u16 x = (u16)PROP3C1_PLACEMENTS[slot].x;
    u16 z = (u16)PROP3C1_PLACEMENTS[slot].z;

    Func_080f9010(158);
    Func_08009178(PROP3C1_SCRIPT, x, z);
    Func_0808a578(0, 0, -16);
    *(s32 *)(*PROP3C1_WORKSPACE_CELL + 456) = 16;
    Func_0808a248(slot);
}
