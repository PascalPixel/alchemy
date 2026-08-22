#include "types.h"

/*
 * resource_39e owner at 0x02002764, 20 bytes: play sound 123, then
 * Func_02006c74(1).
 *
 * PUBLISHED, NOT CALLED. No `bl` in the image reaches it; its Thumb
 * pointer is written into a script record. Found by
 * `cargo run --release --manifest-path tools/overlay-published/Cargo.toml -- resource_39e`.
 *
 * Argument order is worth stating because the two calls set r0 twice
 * and a histogram of callees cannot tell them apart: the sound id 123
 * is loaded first and consumed by Func_02006ce6 at 0x02002768, then r0
 * is reloaded with 1 for Func_02006c74 at 0x0200276e. Resolved
 * per-site with `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml -- resource_39e 0x2764
 * 0x2778 --annotate`, not by pairing the summary histogram against call
 * shapes.
 *
 * Complete owner: `push {lr}` at 0x02002764 through `pop {r0} / bx r0`
 * at 0x02002772-0x02002774; no literal pool, and the next published
 * owner (0x02002778, also found by the same sweep and not yet drafted)
 * begins at 0x02002778.
 *
 * Uncertainty: 123 is a sound id by the established role of
 * Func_02006ce6; Func_02006c74's single argument role is unknown.
 */

extern void Func_02006ce6(s32 sound_id);
extern void Func_02006c74(s32 arg0);

void Func_02002764(void)
{
    Func_02006ce6(123);
    Func_02006c74(1);
}
