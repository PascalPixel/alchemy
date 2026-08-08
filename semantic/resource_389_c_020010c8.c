#include "types.h"

/*
 * resource_389 owner at 0x020010c8, 148 bytes: the per-frame TASK that drives
 * the scene-99 timeline off a frame counter, and hands control to scene 99 when
 * the gate halfword clears.
 *
 * Named twice before it was opened, which is what root-down ordering buys:
 *  - the entry-0 root (semantic/overlays/resource_389_c_0200121c.c) zeroes
 *    Data_020098ec and then calls Func_080000d0(0x020090c9, 3200) - under the
 *    proven 0x02008000 link base 0x020090c9 is file offset 0x10c8 plus the
 *    Thumb bit.  So this is a task, its priority is 3200, and Data_020098ec is
 *    the counter it was given a clean start on.
 *  - the scene-99 callback (semantic/overlays/resource_389_c_0200115c.c) calls
 *    Func_080000d8(0x020090c9) as its first instruction, i.e. removes it.
 * Install, counter and removal were all established before disassembly.
 *
 * Complete owner: `push {r5, lr}` at 0x020010c8 through the single epilogue at
 * 0x02001148, ending `pop {r0} / bx r0` - void.
 *
 * Pool map from a control-flow walk: 0x020010c8-0x0200114d is reached as
 * instructions; 0x0200114e-0x0200115b is not.  That is 14 bytes - one alignment
 * halfword plus three words (0x03001ebc, 0x020098ec, 0x02000240) - so the pool
 * does NOT run the customary 8 or 12 bytes and does not begin on the word after
 * the last instruction.  134 code + 14 pool = 148, exactly the inventory row's
 * code_bytes and span_bytes.
 *
 * Calls: 5 sites, matching the advertised 5.  Per-target multiset from
 * `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml -- resource_389 10c8`:
 *   Func_0808a1b8  x1   0x1100
 *   Func_0808a1e8  x1   0x110a
 *   Func_0808a130  x1   0x1114
 *   Func_0808a128  x1   0x1120
 *   Func_0808a100  x1   0x112a
 *
 * The dispatch is a comparison chain, not a jump table, and it is NOT a dense
 * switch: 0x020010d6 tests 240 directly, 0x020010dc splits on greater-than, and
 * the high half tests 270 (135 << 1) and then 480 (270 + 210) by adjusting the
 * SAME register.  240 and 270 branch to the same body at 0x0200111a; that body
 * is written once under two case labels rather than duplicated, which keeps
 * Func_0808a128's site count at one.
 *
 * The counter is read-modify-written through a pooled pointer, so the value
 * tested is the POST-increment value; the first frame the task runs the counter
 * is 1, not 0.
 *
 * 0x02000240 is the shared cross-overlay scene workspace (the RAM block, not a
 * file offset - it is below the 0x02008000 link band, so the documented
 * "0x02000240 is both a RAM global and a plausible file offset" trap does not
 * bite here).  Its signed halfword index 282 is the same gate the entry-0 root
 * tests before installing this task: the root installs only when the gate is
 * NON-zero, and this task performs its handover only once the gate has become
 * zero, so the two readings are complementary rather than contradictory.
 */

/* The shared cross-overlay scene workspace; see resource_370_c_02000384.c. */
extern s16 Data_02000240[];

/* Pointer CELL holding the per-overlay workspace base, not the workspace. */
extern u8 *Data_03001ebc;

/* In-image counter word at file offset 0x18ec, zeroed by the entry-0 root. */
extern s32 Data_020098ec;

/* Old-style declarations: overlay imports vary in arity between call sites. */
void Func_020025da();
void Func_020025ec();
void Func_020025be();
void Func_020025c2();
void Func_020025bc();
                                     /* one-argument presentation call */
                                     /* three-argument presentation call */
                                     /* one-argument presentation call */
                                     /* three-argument presentation call */
                                     /* three-argument presentation call */

void Func_020010c8(void)
{
    u8 *workspace = Data_03001ebc;
    s32 frame;

    Data_020098ec += 1;
    frame = Data_020098ec;

    switch (frame) {
    case 60:
        Func_020025da(13, 0x00002000, 0);   /* 128 << 6 */
        Func_020025ec(13, 2, 0);
        break;
    case 180:
        Func_020025be(13, 3);
        break;
    case 240:                               /* both reach one body */
    case 270:                               /* 135 << 1 */
        Func_020025c2(13, 4, 0);
        break;
    case 480:                               /* 270 + 210 */
        Func_020025bc(13, 4);
        break;
    }

    /* Byte offset 141 << 2 = 564, i.e. halfword index 282. */
    if (Data_02000240[282] == 0) {
        *(s16 *)(workspace + 386) = 99;     /* 193 << 1 */
    }
}
