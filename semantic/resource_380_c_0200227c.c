#include "types.h"

/*
 * resource_380 owner at 0x0200227c, 388 bytes: six-actor scene wiring.
 * Creates a shared handle, enables six actors (5, 9, 11, 10, 14, 13,
 * each through its own resident enable entry), places each at a fixed
 * fixed-point coordinate pair, then for each actor fetches its object,
 * stores the shared handle at +104, sets flag bit 1 at +90, and binds
 * the shared descriptor table 0x0200cbd0.  Actor 14 additionally gets
 * position 0x10000/0x10000 (+24/+28), copies actor 11's +85 byte,
 * zeroes +12, and actor 13's bind is followed by one trailing
 * parameterless call.
 *
 * Complete owner: `push {r5,r6,lr}` + r8-sl spill at 0x0200227c
 * through `pop {r0} / bx r0` at 0x020023e4, then the literal pool
 * 0x020023e8-0x020023ff (five fixed-point x coordinates and the
 * descriptor 0x0200cbd0); next owner's prologue at 0x02002400.
 *
 * CORRECTION (name sweep): this file's callee names came from a naive
 * pc-relative decode and were wrong; they are resolved here through the
 * overlay's import-veneer table under the +2 rule
 * (tools/overlay_call_targets.ts) to their main-ROM identities.  Two
 * earlier readings fall with those names.  The claim that "each actor
 * id has its own dedicated enable/fetch/bind entry points at
 * 6-to-20-byte strides" was pure decode artifact: the strides were the
 * call sites drifting, not distinct callees, and the real extern list
 * is short and ordinary.  The two "old-style" declarations existed
 * only because one phantom name covered two different functions
 * (Func_0808a098/Func_08009098 and Func_0808a080/Func_0808a020);
 * nothing here is old-style.  The final pair was written as a nested
 * call -- "the last call reuses whatever the preceding bind returned"
 * -- on the strength of that same phantom; the two bl at 0x020023d0
 * and 0x020023d4 are consecutive with no register setup between them,
 * so they are plain sequential calls and Func_0808a020 is the void
 * scene-bracket close it is everywhere else in the item-28 drafts.
 *
 * Uncertainty: callee roles beyond call shape remain open.
 */

extern void Func_08009098(s32 object, s32 table);
extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern s32 Func_0808a080(s32 arg0);
extern void Func_0808a098(s32 actor, s32 arg1);
extern void Func_0808a0f0(s32 actor, s32 x, s32 y);

void Func_0200227c(void)
{
    s32 handle = Func_0808a080(0);
    s32 table = 0x0200cbd0;
    s32 object;
    s32 helper;

    Func_0808a018();
    Func_0808a098(5, 1);
    Func_0808a098(9, 1);
    Func_0808a098(11, 1);
    Func_0808a098(10, 1);
    Func_0808a098(14, 1);
    Func_0808a098(13, 1);
    Func_0808a0f0(5, 0x01db0000, 0x14c0000);
    Func_0808a0f0(9, 0x01eb0000, 0x14c0000);
    Func_0808a0f0(11, 0x01cb0000, 0x15c0000);
    Func_0808a0f0(10, 0x01fb0000, 0x15c0000);
    Func_0808a0f0(14, 0x1cc0000, 0x1680000);
    Func_0808a0f0(13, 0x01d70000, 0x1320000);

    object = Func_0808a080(5);
    *(s32 *)(object + 104) = handle;
    *(u8 *)(object + 90) |= 1;
    Func_08009098(object, table);

    object = Func_0808a080(9);
    *(s32 *)(object + 104) = handle;
    *(u8 *)(object + 90) |= 1;
    Func_08009098(object, table);

    object = Func_0808a080(11);
    *(s32 *)(object + 104) = handle;
    *(u8 *)(object + 90) |= 1;
    Func_08009098(object, table);

    object = Func_0808a080(10);
    *(s32 *)(object + 104) = handle;
    *(u8 *)(object + 90) |= 1;
    Func_08009098(object, table);

    object = Func_0808a080(14);
    *(s32 *)(object + 104) = handle;
    *(u8 *)(object + 90) |= 1;
    *(s32 *)(object + 24) = 0x10000;
    *(s32 *)(object + 28) = 0x10000;
    helper = Func_0808a080(11);
    *(u8 *)(object + 85) = *(u8 *)(helper + 85);
    *(s32 *)(object + 12) = 0;
    Func_08009098(object, table);

    object = Func_0808a080(13);
    *(s32 *)(object + 104) = handle;
    *(u8 *)(object + 90) |= 1;
    Func_08009098(object, table);
    Func_0808a020();
}
