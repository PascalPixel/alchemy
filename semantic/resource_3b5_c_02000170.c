#include "types.h"

/*
 * Resource 3b5, owner at 0x02000170 (152 bytes advertised; 144 bytes of code
 * plus an 8-byte literal pool at 0x02000200-0x02000207).
 *
 * Complete owner: `push {r5, r6, r7, lr}` plus the high-register save at
 * 0x02000170-0x02000178, and the matching interworking return at
 * 0x020001f2-0x020001fe.  It pops into r1, so r0 survives and is the result;
 * the only value ever left in r0 is the `movs r0, #0` at 0x020001f0, so this
 * returns a constant 0.
 *
 * Role: this is a per-frame actor callback.  The overlay initialiser
 * Func_02000728 stores the pool word 0x02008171 into field +0x6c of actors 16
 * and 17; under this overlay's proven 0x02008000 link base that word is
 * Func_02000170 + the Thumb bit, which names this row's role before it is
 * disassembled.  Its argument is therefore the owning actor record.
 *
 * 0x03001e8c is a table of pointers; entry 12 is 0x03001ebc, the overlay
 * workspace pointer the rest of this overlay loads directly (see
 * Func_02000644, Func_02000894, Func_02000980).  Modelled that way rather than
 * as two unrelated globals.
 *
 * Call targets resolved with `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml --`.
 */

/* Old-style declarations: overlay import arities are not fixed per name. */
u8 *Func_0808a080();
s32 Func_0200007c();

s32 Func_02000170(u8 *self)
{
    u8 **globals = (u8 **)0x03001e8c;
    u8 *scene = globals[0];
    u8 *workspace = globals[12];        /* == *(u8 **)0x03001ebc */
    u16 *flags = (u16 *)(self + 100);
    s32 force = 0;
    s32 range = 18;
    u8 *player;

    /* Bit 0 of the actor's own flag halfword selects which partner to test. */
    if (Func_0200007c(self, Func_0808a080((*flags & 1) ? 17 : 16), 32, 0) != 0) {
        return 0;
    }

    player = Func_0808a080(0);

    /*
     * Widen the test when the scene counter at workspace + 376 is already
     * running, or when the scene byte at scene + 0x0ea4 is set.
     */
    if (*(s16 *)(workspace + 376) != 0 || scene[0x0ea4] != 0) {
        range = 26;
        if ((*flags & 2) != 0) {
            force = 1;
        }
    }

    Func_0200007c(self, player, range, force);
    return 0;
}
