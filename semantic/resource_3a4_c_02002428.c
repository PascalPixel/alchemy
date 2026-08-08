#include "types.h"

/*
 * resource_3a4 owner at 0x02002428, 104 bytes: if story value 0x8fe is set,
 * clear bit 9 of a third overlay workspace's +20 field; otherwise render
 * six fixed cells. Either way, then check a saved story value against a
 * threshold and fire a second story-value setter if it is not exceeded.
 *
 * Complete owner: `push {lr}` plus an 8-byte frame at 0x02002428, through
 * the matching interworking return `pop {r0} / bx r0` at 0x02002476-
 * 0x02002478, followed by the five-word literal pool 0x0200247c-0x0200248f
 * that closes the 104-byte span. No arguments, void.
 *
 * Not found by the structural inventory walk (unindexed); one incoming
 * call site per overlay_unindexed.ts. This owner makes three outgoing
 * calls, all resolved with the `+2` rule against the raw image (this
 * owner starts outside a recorded row, so overlay_call_targets.ts's own
 * table does not cover it -- resolved with `bun tools/overlay-show
 * resource_3a4 2428 | cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml -- resource_3a4
 * 2428 2478 --annotate`):
 *   0x200242e -> veneer -> Func_080770c0(0x8fe)     story-flag query, established
 *   0x2002454 -> veneer -> Func_080091c0(52, 42, 1, 1, 53, 42)   established, "six-argument renderer, last two on stack" (resource_3ba_c_02000a3c.c)
 *   0x2002470 -> veneer -> Func_080770d0(0x12f)     established, single-argument (resource_3c8_c_02001218.c, resource_37b_c_02000150.c)
 *
 * 0x03001e70 is a THIRD overlay workspace pointer, distinct from the
 * 0x03001ebc / 0x03001e68 / 0x03001f3c pointers already seen across this
 * queue; this owner only touches its +20 field. Data_02000240+450 is read
 * here as an UNSIGNED halfword (resource_3a4_c_020029dc.c reads the same
 * address signed), so the two owners' agreement on the field's numeric
 * range is not assumed -- transcribed as each instruction reads it.
 */

extern u8 Data_02000240[];

s32 Func_080770c0();           /* story-flag query, established */
void Func_080091c0();          /* six-argument renderer, established */
void Func_080770d0();          /* single-argument, established */

void Func_02002428(void)
{
    u8 *workspace;
    u32 cue;

    if (Func_080770c0(0x8fe) != 0) {
        workspace = *(u8 **)0x03001e70;
        *(u16 *)(workspace + 20) &= 0xfdff;
    } else {
        Func_080091c0(52, 42, 1, 1, 53, 42);
    }

    cue = *(u16 *)&Data_02000240[450];
    if (((cue - 6) << 16) <= (u32)(128 << 9)) {
        Func_080770d0(0x12f);
    }
}
