#include "types.h"

/*
 * Main-image owner at 0x08006f48, 36 bytes: count a run of consecutive
 * 0xff bytes starting at `ptr`, capped by a limit read from a global
 * object's field 4.
 *
 * Core-drive census row (work/claude/queue-core.md task; census file
 * /tmp/ALCHEMY_QUEUE_CORE.md): flagged `nonstandard_thumb_branch_module`
 * because the automated walk couldn't classify the `b .L_08006f5a`
 * jump-to-loop-test-first shape. Leaf function -- no `push`/`pop`, a
 * bare `bx lr` return -- so it never appeared as a `push`-rooted owner
 * to the structural walk either.
 *
 * Complete owner: `Func_08006f48:` at 0x08006f48 through `bx lr` at
 * 0x08006f64, followed by one alignment halfword (`.inst.n 0x0000`);
 * the one-word pool (`0x02004c08`) sits inline, mid-body, skipped by
 * the initial `b .L_08006f5a` -- the same inline-pool shape already
 * documented on the overlay lanes (e.g. resource_39c_c_02002844.c).
 * One argument, `s32` return (the remaining budget when the scan
 * stopped).
 *
 * `Data_02004c08` is a global whose stored value is itself a pointer;
 * its field 4 is read once as the starting budget. Not identified
 * beyond this call shape.
 */

extern u8 *Data_02004c08;

s32 Func_08006f48(u8 *ptr)
{
    s32 remaining = *(s32 *)(Data_02004c08 + 4);

    while (remaining != 0) {
        u8 byte = *ptr;
        ptr++;
        if (byte != 0xff) {
            break;
        }
        remaining--;
    }

    return remaining;
}
