#include "types.h"

/*
 * Resource 3b5, owner at 0x020006e8 (62 bytes, all code; no literal pool).
 *
 * Complete owner: `push {r5, lr}` at 0x020006e8 and the matching
 * `pop {r5} / pop {r0} / bx r0` at 0x02000720-0x02000724.  The epilogue pops
 * the return address into r0, so nothing is returned: this owner is void
 * (HANDOVER 0, epilogue rule).
 *
 * Role: per-frame actor callback.  Func_02000728 stores the pool word
 * 0x020086e9 into field +0x6c of actor 14; under this overlay's proven
 * 0x02008000 link base that is Func_020006e8 + the Thumb bit, which names the
 * row and fixes its argument as the owning actor record.
 *
 * Behaviour: mirror the player's two-bit palette/shade selector (bits 2-3 of
 * the byte at +9 of the sprite record hanging off +0x50) onto this actor's own
 * sprite record, at both +9 and +21, and clear the actor's byte at +0x23.
 *
 * Uncertainty: r4 is used as scratch at 0x02000704 although `push {r5, lr}`
 * never saves it.  Recognised idiom in this codebase, reproduced as-is.
 *
 * Call target resolved with `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml --`.
 */

/* Old-style declaration: overlay import arities are not fixed per name. */
u8 *Func_02001470();

void Func_020006e8(u8 *self)
{
    u8 *player;
    u8 *player_sprite;
    u8 *sprite;
    s32 shade;

    if (self == 0) {
        return;
    }

    player = Func_02001470(0);
    player_sprite = *(u8 **)(player + 0x50);

    self[0x23] = 0;

    shade = player_sprite[9] & 12;

    sprite = *(u8 **)(self + 0x50);
    sprite[9] = (u8)((sprite[9] & ~12) | shade);

    sprite = *(u8 **)(self + 0x50);
    sprite[21] = (u8)((sprite[21] & ~12) | shade);
}
