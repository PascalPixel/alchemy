#include "types.h"

/*
 * Resource 3b5, owner at 0x02000314 (152 bytes advertised; 144 bytes of code
 * plus an 8-byte literal pool at 0x02000200-0x02000207).
 *
 * Complete owner: `push {r5, r6, r7, lr}` plus the high-register save at
 * 0x02000314-0x02000178, and the matching interworking return at
 * 0x020001f2-0x020001fe.  It pops into r1, so r0 survives and is the result;
 * the only value ever left in r0 is the `movs r0, #0` at 0x020001f0, so this
 * returns a constant 0.
 *
 * Role: this is a per-frame actor callback.  The overlay initialiser
 * Func_02000728 stores the pool word 0x02008171 into field +0x6c of actors 14
 * and 15; under this overlay's proven 0x02008000 link base that word is
 * Func_02000314 + the Thumb bit, which names this row's role before it is
 * disassembled.  Its argument is therefore the owning actor record.
 *
 * 0x03001e8c is a table of pointers; entry 12 is 0x03001ebc, the overlay
 * workspace pointer the rest of this overlay loads directly (see
 * Func_02000644, Func_02000894, Func_02000980).  Modelled that way rather than
 * as two unrelated globals.
 *
 * Call targets resolved with `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml --`.
 */

/*
 * Old-style declarations: overlay import arities are not fixed per name.
 *
 * Each site names the veneer IT reaches, not the import behind it: the four
 * calls here are Func_0808a080, Func_0200007c, Func_0808a080, Func_0200007c,
 * and the reference reaches them through four DIFFERENT veneers.  Naming the
 * import instead made the linker synthesise fresh ones and every `bl` missed.
 * Same convention as 6e7d6a12e and games/gs1/src/resource_371_c_0200037c.c, where one
 * import reached from three sites carries three names.
 *
 * Func_02001a06 is `void` because its result is discarded (b7c1a35a0).
 *
 * Func_020019c6 returns s32: the reference tests its result with `cmp r0, #0`
 * and never truncates it, so a u8 return costs an `lsls r0, r0, #24` the ROM
 * does not have.  An earlier reading measured that widening as expensive, but
 * that was against the conditional-expression form of the partner select
 * below; once that is a branch, the wide return is what matches.
 */
u8 *Func_02001d26();
s32 Func_020019c6();
u8 *Func_02001d3c();
void Func_02001a06();

s32 Func_02000314(u8 *self)
{
    u8 **globals = (u8 **)0x03001e8c;
    u8 *scene = globals[0];
    u8 *workspace = globals[12];        /* == *(u8 **)0x03001ebc */
    u16 *flags = (u16 *)(self + 100);
    s32 force = 0;
    s32 range = 18;
    u8 *partner;
    u8 *player;

    /*
     * Bit 0 of the actor's own flag halfword selects which partner to test.
     * Written as two calls, not `Func_02001d26(bit ? 15 : 14)`: the reference
     * branches and joins on one `bl`, which is what cross-jumping the two calls
     * produces.  As a conditional expression gcc folds it to `14 + (bit != 0)`
     * and emits the negs/orrs/lsrs boolean instead.
     */
    if ((*flags & 1) != 0) {
        partner = Func_02001d26(15);
    } else {
        partner = Func_02001d26(14);
    }
    if (Func_020019c6(self, partner, 32, 0) != 0) {
        return 0;
    }

    player = Func_02001d3c(0);

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

    Func_02001a06(self, player, range, force);
    return 0;
}
