typedef unsigned int u32;
typedef signed int s32;

/*
 * Resource 3cb idle-timeout poll at 0x020002d8 (104 bytes,
 * 0x020002d8-0x0200033f).
 *
 * Complete owner: `push {lr}` prologue at 0x020002d8 and the matching
 * interworking return `pop {r1} / bx r1` at 0x0200032e, which every path
 * reaches.  The popped return address goes to r1, so r0 survives and IS the
 * result.
 *
 * The twelve bytes at 0x02000334-0x0200033f are this owner's literal pool
 * (0x00000203, 0x02009f50, 0x0000292e), with two alignment bytes at
 * 0x02000332.  Execution never falls into them.
 *
 * All nine call sites were resolved with tools/overlay_call_targets.ts
 * (true_target_offset = stored_displacement + 2), matching the inventory's
 * calls=9:
 *   0x020002dc -> veneer 0x0200180c -> Func_080770c0
 *   0x020002fa -> veneer 0x0200181c -> Func_080770d0
 *   0x02000302 -> veneer 0x0200180c -> Func_080770c0
 *   0x0200030a -> veneer 0x02001844 -> Func_0808a018
 *   0x02000310 -> veneer 0x02001884 -> Func_0808a170
 *   0x02000318 -> veneer 0x0200188c -> Func_0808a178
 *   0x0200031e -> veneer 0x02001714 -> Func_080000c0
 *   0x02000326 -> veneer 0x02001814 -> Func_080770c8
 *   0x0200032a -> veneer 0x0200184c -> Func_0808a020
 *
 * Link base: resource_3cb is linked at 0x02008000 — the handler table at the
 * tail of assets/code/resource_3cb_overlay.s lists 0x02009051, 0x020082d9 and
 * 0x02008341, which are Func_02001050 (byte-exact in assets/code),
 * Func_020002d8 (this owner) and Func_02000340 plus the Thumb bit.  So the
 * pool word 0x02009f50 is an IN-IMAGE datum at file offset 0x1f50, not a RAM
 * global.  The overlay image is writable EWRAM, so this counter genuinely
 * lives in the overlay's own data.
 *
 * Behaviour: this is the per-frame poll for one of the overlay's flag-gated
 * scenes.  It does nothing while flag 0x203 is set.  Otherwise it advances the
 * in-image idle counter, wrapping it to zero and clearing flag 512 when it
 * reaches 300 frames.  While flag 512 is clear it then runs the scene body
 * once: a Func_0808a018 barrier, cue 0x292e, Func_0808a178(8, 0), a
 * Func_080000c0(5) wait, re-setting flag 512 via Func_080770c8, and a closing
 * Func_0808a020.  So the scene fires once every 300 frames.
 *
 * The flag trio is Func_080770c0 = test, Func_080770c8 = set,
 * Func_080770d0 = clear.  That assignment is forced here: if Func_080770d0
 * set flag 512 the body below could never run, whereas clear/set gives the
 * one-shot-per-300-frames reading.  Func_020010e8's toggle and
 * Func_02000340's three-flag teardown are consistent with it.  The meanings of
 * the flag words 0x203 and 512 themselves are not established.  The
 * store of r0 at 0x020002f4 reuses the register the first Func_080770c0 left
 * as zero on this path, which is why the reset is written as a literal 0.
 * The result is whatever r0 holds at the shared exit: the nonzero flag reading
 * on either early return, or Func_0808a020's value on the full path; nothing
 * in this owner establishes that a caller consumes it, but the epilogue shape
 * is the evidence for typing it s32.
 */

extern u32 Data_02009f50[];

/* Old-style declarations: arities vary per site across this overlay. */
void Func_080000c0();
s32 Func_080770c0();
void Func_080770c8();
void Func_080770d0();
void Func_0808a018();
s32 Func_0808a020();
void Func_0808a170();
void Func_0808a178();

/* Used for their return values. */

s32 Func_020002d8(void)
{
    s32 gate;
    u32 idle;

    gate = Func_080770c0(0x203);
    if (gate != 0) {
        return gate;
    }

    idle = Data_02009f50[0] + 1;
    Data_02009f50[0] = idle;
    if (idle == 300) {
        Data_02009f50[0] = 0;
        Func_080770d0(512);
    }

    gate = Func_080770c0(512);
    if (gate != 0) {
        return gate;
    }

    Func_0808a018();
    Func_0808a170(0x292e);
    Func_0808a178(8, 0);
    Func_080000c0(5);
    Func_080770c8(512);
    return Func_0808a020();
}
