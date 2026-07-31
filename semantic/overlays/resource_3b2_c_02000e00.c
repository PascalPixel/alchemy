typedef signed short s16;
typedef signed int s32;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned char u8;

/*
 * resource_3b2 owner at 0x02000e00, 136 bytes: code 0x02000e00-0x02000e7d,
 * a two-byte alignment `movs r0, r0` at 0x02000e7e, and the two-word literal
 * pool 0x02000e80-0x02000e87 (0x02008da5, 0x01000001).
 *
 * Complete owner.  Prologue `push {r5, r6, r7, lr}` + `push {r5, r6, r7}`
 * saving r8/r9/sl, a 68-byte local frame, and the single epilogue at
 * 0x02000e6e that restores everything and ends `pop {r0} / bx r0` — so by the
 * interworking-epilogue rule the owner is **void**.
 *
 * Call targets resolved with `bun tools/overlay_call_targets.ts resource_3b2
 * 0e00 --json` (5 sites, 5 distinct targets), NOT with the disassembler's
 * annotations:
 *   0x02000e0c -> veneer 0x02002fc8 -> Func_0808a080  (record accessor)
 *   0x02000e26 -> veneer 0x02002fd0 -> Func_08000120  (trig, "sine" lobe)
 *   0x02000e32 -> veneer 0x02002fc8 -> Func_08000118  (trig, "cosine" lobe)
 *   0x02000e40 -> veneer 0x02002fb0 -> Func_03000380  (relocated IWRAM divide)
 *   0x02000e60 -> prologue 0x02000ae8 -> Func_02000ae8 (this overlay's effect
 *                                                       spawner)
 *
 * Link base 0x02008000 is confirmed for this overlay by the pool word
 * 0x02008da5 = Func_02000da4 + the Thumb bit — Func_02000da4 is a byte-exact
 * sibling in assets/code and is exactly a per-frame position/velocity
 * integrator, which is what an effect record's +108 handler slot holds.
 *
 * Behaviour: read the actor record's world position, then emit a ring of nine
 * effects at headings 0, 0x2000, ... 0x10000 (the 16-bit angle wraps, so the
 * first and last coincide).  Each effect is given the outward velocity
 * (4/3 * sin, 0, cos) for its heading and the shared options block whose only
 * initialised field is the per-frame handler at +36.
 *
 * The options block at sp+16 is 40 bytes and only its +36 word is written; the
 * flag word 0x01000001 selects exactly one optional field in Func_02000ae8
 * (bit 0x01000000 -> "override the per-frame handler from options+36"), so the
 * other 36 bytes are genuinely never read.  Left uninitialised here as encoded.
 *
 * UNCERTAINTY: Func_08000118/Func_08000120 are the two lobes of the main
 * image's angle table; which is sine and which cosine is not established here,
 * only that the first result feeds the X step and the second the Z step.  The
 * 4/3 scaling of the X step (x + x/3, via the IWRAM divide by 3) reads as the
 * usual screen aspect correction.
 */

/* Old-style declarations: overlay imports may be reached with differing
 * argument counts from different sites. */
u8 *Func_0808a080();
s32 Func_08000118();
s32 Func_08000120();
s32 Func_03000380();

/* This overlay's effect spawner:
 * (x, y, z, vx, vy, vz, flags, options). */
void Func_02000ae8();

/* Actor record: only the world position triple at +8/+12/+16 is read, the same
 * f08/f0c/f10 fields the byte-exact sibling assets/code/resource_3b2_c_
 * 02000da4.c integrates. */
struct RingActor {
    u8 unknown_00[8];
    s32 x;                          /* 0x08 */
    s32 y;                          /* 0x0c */
    s32 z;                          /* 0x10 */
};

/* Options block passed to the spawner.  Only 'handler' is initialised. */
struct RingOptions {
    u8 unused_00[36];
    u32 handler;                    /* 0x24 */
};

void Func_02000e00(s32 slot)
{
    struct RingOptions options;
    s32 step[3];
    const struct RingActor *actor;
    s32 heading;

    actor = (const struct RingActor *)Func_0808a080(slot);

    /* Func_02000da4 + the Thumb bit: the per-frame integrator installed into
     * each spawned effect's +108 handler slot. */
    options.handler = 0x02008da5;

    for (heading = 0; (u32)heading <= 16; heading += 2) {
        s32 angle = heading << 12;
        s32 across;
        s32 along;

        across = Func_08000120(angle);
        step[1] = 0;
        step[0] = across;

        along = Func_08000118(angle);
        across = step[0];
        step[2] = along;

        /* x + x/3 == 4/3 * x, through the relocated IWRAM divide. */
        across = across + Func_03000380(across, 3);
        step[0] = across;

        Func_02000ae8(actor->x, actor->y, actor->z,
                      across, step[1], along,
                      0x01000001, &options);
    }
}
