#include "types.h"

/*
 * resource_389 owner at 0x02000bd0, 272 bytes: the per-frame TASK that trails
 * effects behind scene actor 9 while the 0x02000ce0 set-piece is running.
 *
 * Named before it was opened.  `assets/code/resource_389_overlay.s` holds the
 * odd in-image pool word 0x02008bd1, which under the proven 0x02008000 link
 * base is file offset 0x0bd0 plus the Thumb bit; its two uses are
 * Func_080000d0 (install task) at 0x02000d56 and Func_080000d8 (remove task)
 * at 0x02000dd0, both inside semantic/overlays/resource_389_c_02000ce0.c.  So
 * the task identity, the installer and the remover were all known in advance.
 *
 * Complete owner: `push {r5, r6, r7, lr}` plus the r8/r9/sl saves at
 * 0x02000bd0 through the single epilogue at 0x02000cca, ending `pop {r0} /
 * bx r0` - void.
 *
 * Pool map from a control-flow walk: 0x02000bd0-0x02000cd9 is reached as
 * instructions; 0x02000cda-0x02000cdf is not.  That trailing pool is SIX bytes
 * - one alignment halfword plus the single word 0x03001e40 - which is the
 * documented small-pool case, not the customary 8-12.  266 code + 6 pool =
 * 272, exactly the inventory row's code_bytes and span_bytes.
 *
 * Calls: 14 sites, resolved with `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml --
 * resource_389 bd0`, matching the advertised 14 exactly.  Per-target multiset:
 *   Func_0808a080  x6   0xbec 0xc08 0xc10 0xc62 0xc7e 0xc86
 *   Func_080000f8  x6   0xbf2 0xc1c 0xc36 0xc68 0xc92 0xcac
 *   Func_02000a68  x2   0xc50 0xcc6 (prologue sites)
 *
 * DELIBERATELY NOT FOLDED.  The two emission blocks are behaviourally
 * identical - same three lookups, same three random draws, same seven
 * arguments - and differ only in the gate that reaches them (frame & 7 for the
 * first, frame & 15 for the second, so the second fires on every other run of
 * the first).  Folding them into one block, or into a loop, would deflate every
 * one of the three multisets above by half (HANDOVER: "behaviourally identical
 * skip-beat arms are still DISTINCT call sites").
 *
 * The gate variable is also the seventh argument: r7 holds the masked frame
 * counter, is proven zero by the branch that reaches the block, and is what is
 * stored to sp+8.  It is spelled 0 here with that noted, rather than as a
 * variable, because that is what it provably is at both sites.
 *
 * The random scaling is the same shift-and-add family as 0x02000ce0:
 * rand*12>>16 gives 0..11 whole 16.16 units of eastward offset, rand*5>>16
 * then *6553 gives 0..4 tenths of a unit (6553 = 0x10000/10) applied
 * downwards, and rand*2>>16 gives 0 or 1.
 */

/* Free-running frame counter shared across the image. */
extern u32 Data_03001e40;

struct SceneActor {
    u8 unknown_00[8];
    s32 x;                          /* 0x08 */
    s32 y;                          /* 0x0c */
    s32 z;                          /* 0x10 */
};

/* Old-style declarations: overlay imports vary in arity between call sites. */
struct SceneActor *Func_0200203e();
s32 Func_02001fb4();
struct SceneActor *Func_0200205a();
struct SceneActor *Func_02002062();
s32 Func_02001fde();
s32 Func_02001ff8();
void Func_020016ba();
struct SceneActor *Func_020020b4();
s32 Func_0200202a();
struct SceneActor *Func_020020d0();
struct SceneActor *Func_020020d8();
s32 Func_02002054();
s32 Func_0200206e();
void Func_02001730();
                                     /* scene actor by selector */
                                     /* random source, no arguments */

/* This overlay's own seven-argument placement helper at 0x02000a68. */

void Func_02000bd0(void)
{
    struct SceneActor *origin;
    struct SceneActor *anchor;
    s32 spawnX;
    s32 spawnZ;
    s32 drift;
    s32 lift;

    /* One frame in eight. */
    if ((Data_03001e40 & 7) != 0) return;

    origin = Func_0200203e(9);
    spawnX = origin->x + (((Func_02001fb4() * 12) >> 16) << 16);

    anchor = Func_0200205a(9);
    spawnZ = Func_02002062(9)->z + 0x00060000;  /* 192 << 11 */

    drift = ((Func_02001fde() * 5) >> 16) * 6553;
    lift = (Func_02001ff8() * 2) >> 16;

    /* The last argument is the gate value, proven 0 on this path. */
    Func_020016ba(spawnX, anchor->y, spawnZ, 0, -drift, lift, 0);

    /* One frame in sixteen, i.e. every other run of the block above. */
    if ((Data_03001e40 & 15) != 0) return;

    origin = Func_020020b4(9);
    spawnX = origin->x + (((Func_0200202a() * 12) >> 16) << 16);

    anchor = Func_020020d0(9);
    spawnZ = Func_020020d8(9)->z + 0x00060000;

    drift = ((Func_02002054() * 5) >> 16) * 6553;
    lift = (Func_0200206e() * 2) >> 16;

    Func_02001730(spawnX, anchor->y, spawnZ, 0, -drift, lift, 0);
}
