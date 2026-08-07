/*
 * resource_3a7 effect-burst spawner at 0x02000b8c, 124 bytes.
 *
 * Complete owner: `push {r5, r6, r7, lr}` at 0x02000b8c through `pop {r5, r6,
 * r7} / pop {r0} / bx r0` at 0x02000bf0.  The return address is popped into r0,
 * so the owner is `void`.  Four literal-pool words sit inside the span past the
 * epilogue - 0x02000bf8 (0x00008ccc), 0x02000bfc (0xffff0000), 0x02000c00
 * (0x0000cccc) and 0x02000c04 (0x02009884) - all data, not code.  Note the
 * listing decodes 0x02000bfc/0x02000c00 as plausible instructions; they are
 * pool, established by the control-flow walk (the epilogue at 0x02000bf0 is the
 * owner's only exit and nothing branches past it).
 *
 * r0 is read at 0x02000b8e before being written, so the owner takes one
 * argument: the source record whose 16.16 (x, y, z) at +8/+12/+16 seeds each
 * spawn.  Those are the same three coordinate words this overlay's 0x02000b54,
 * 0x02000458 and 0x02000944 read.
 *
 * Shape: spawn up to four effect records of kind 240 at the source's position;
 * stop early if the spawner returns NULL.  Each spawned record gets +24 and +28
 * set to 0x8ccc, byte +85 set to 2, +40 set to 0xffff0000, +48 set to a random
 * value biased by 0xcccc, byte +89 set to 1, is stepped a fixed 0x200000 along a
 * random heading by 0x02000b54, gets the halfword at +94 set to 8, and is handed
 * the in-image animation script at 0x02009884.
 *
 * 0x02009884 is EVEN, so under the proven 0x02008000 link base it is in-image
 * DATA at file offset 0x1884 - the animation script - not a callback.  (The
 * odd/even parity rule; the same overlay's odd 0x02008aa1 and 0x020089c1 are
 * task callbacks.)
 *
 * Call targets resolved with
 *   bun tools/overlay_show.ts resource_3a7 0b8c -n 124 |
 *   bun tools/overlay_call_targets.ts resource_3a7 --annotate
 * Per-target multiset over the 5 sites, matching the inventory's calls=5:
 *   Func_080090c8 x1 <- veneer 0x020010a8  (spawner; returns NULL on failure)
 *   Func_080000f8 x2 <- veneer 0x02001090  (random source, no arguments)
 *   Func_02000b54 x1  (this overlay's own step-and-place helper, converted
 *                      alongside this file)
 *   Func_08009098 x1 <- veneer 0x020010a0  (attach an animation script)
 * The loop is entered by the `b.n` at 0x02000b92 straight to its test, so each
 * site occurs once in the C body - the four iterations are the loop, not four
 * separate call sites.
 *
 * Uncertainty: the two `Func_080000f8` results are used differently (one biased
 * by 0xcccc and stored at +48, one passed as 0x02000b54's third argument), which
 * is consistent with the documented no-argument random source but is not proof
 * that both sites draw independently.  The bytes say two calls, so two are
 * written.
 */

/* Old-style declarations: overlay imports vary their argument count between
 * call sites in this overlay. */
#include "types.h"

u8 *Func_080090c8();    /* spawner: (kind, x, y, z) -> record, or NULL */
s32 Func_080000f8();    /* random source, no arguments */
void Func_08009098();   /* attach the animation script at its second argument */
void Func_02000b54();   /* step a record along a heading and place it */

/* In-image animation script at file offset 0x1884 (pool word 0x02009884 under
 * the proven 0x02008000 link base). */
extern u8 Data_02009884[];

void Func_02000b8c(s32 *source)
{
    s32 index = 0;
    u8 *record;

    goto spawn_test;

spawn_body:
        *(s32 *)(record + 28) = 0x00008ccc;
        *(s32 *)(record + 24) = 0x00008ccc;
        record[85] = 2;
        *(s32 *)(record + 40) = (s32)0xffff0000;

        *(s32 *)(record + 48) = Func_080000f8() + 0x0000cccc;
        record[89] = 1;

        Func_02000b54(record, 0x200000, Func_080000f8());

        *(u16 *)(record + 94) = 8;
        Func_08009098(record, Data_02009884);

        index++;

spawn_test:
    if (index > 3)
        return;
    record = Func_080090c8(240, source[2], source[3], source[4]);
    if (record != 0)
        goto spawn_body;
}
