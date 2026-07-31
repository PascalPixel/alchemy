typedef short s16;
typedef int s32;

/*
 * resource_3b9 owner at 0x02001a14, 56 bytes: NOT one of this mandate's
 * six planned rows -- found by boundary-checking 0x02001688's literal
 * pool, which ends at 0x02001a14 while the next KNOWN row (the parked
 * 66-case dispatcher) starts at 0x02001a4c.  That 56-byte gap is this
 * owner, previously invisible to the inventory the same way every
 * item-28 row is.
 *
 * Complete owner: `push {lr}` at 0x02001a14 through `pop {r1} / bx r1`
 * at 0x02001a3a-0x02001a3c, followed by the three-word literal pool
 * 0x02001a3e-0x02001a49 (aligned to 0x02001a3e); the next owner's
 * prologue is exactly at 0x02001a4c, no gap. The popped register is r1,
 * not r0 (HANDOVER's epilogue rule), so r0 is a genuine return value --
 * and both control paths set r0 = 0 immediately before the shared exit,
 * so the return value is unconditionally 0 regardless of which arm ran.
 *
 * Reads a signed 16-bit status id at a fixed address: 0x02000240 + 448,
 * i.e. `*(s16 *)(0x02000240 + 0x1c0)`.  0x02000240 is the shared
 * cross-overlay scene-record block already named in
 * resource_397_c_0200005c.c's header; the +448 displacement is the same
 * one the additive/subtractive workspace idiom uses on the OTHER shared
 * block, 0x03001ebc (resource_377_c_02000a0c.c) -- two different fixed
 * blocks, same displacement, most likely coincidence rather than a
 * shared layout.
 *
 * Two-way dispatch on that id: 140 calls the parked jump-table
 * dispatcher Func_02001a4c (declared old-style since it is not yet
 * drafted); 142 calls this overlay's own Func_02001c6c
 * (resource_3b9_c_02001c6c.c, byte-exact sibling, void/no-argument).
 * Neither call's result is used.
 *
 * Uncertainty: the two ids (140, 142) are recorded as passed constants,
 * not decoded further.
 */

extern void Func_02001a4c();
extern void Func_02001c6c(void);

s32 Func_02001a14(void)
{
    s16 status = *(s16 *)(0x02000240 + 448);

    if (status == 140) {
        Func_02001a4c();
    } else if (status == 142) {
        Func_02001c6c();
    }
    return 0;
}
