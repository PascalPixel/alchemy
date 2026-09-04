#include "types.h"

extern s16 Data_02000240[];
extern u8 Data_02009c04[];
extern u8 Data_020099f4[];

u32 Func_02001796(void);
void Func_0200179a(void);
void Func_02000472();
void Func_02001b6e();
void Func_02001b74();
void Func_02001ba0();
void Func_02001bf0();
void Func_02001c00();
void Func_02001b96();
s32 Func_02001bca();
s32 Func_02001d76(void);
void Func_02002762(s32);
void Func_020027f0(s32);
s32 Func_02002774(void);
void Func_02002aa6();
s32 Func_02002992();
void Func_020028ec();
s32 Func_020029a8();
void Func_020029d8();
s32 Func_020029d8_a();
void Func_020029a0();
void Func_02002906();
s32 Func_020029c2();
void Func_0200291c();
s32 Func_020029cc();
void Func_02002b12();
s32 Func_020029fe();
void Func_02002958();
s32 Func_02002a14();
void Func_02002a44();
s32 Func_02002a44_a();
void Func_02002a0c();
void Func_02002972();
s32 Func_02002a2e();
void Func_02002988();
s32 Func_02002a38();
s32 Func_020029b8(s32, s32);
void Func_02002a4a(s32, s32, s32, s32, s32, s32);
s32 Func_020029ca(s32, s32);
s32 Func_02002a56(void);

/*
 * resource_3cb owner at 0x02000030, 8 bytes: `ldr r0, [pc, #0] / bx lr`
 * plus the one-word literal pool at 0x02000034 holding 0x2009948.
 *
 * HEAD EXPORT STUB. Published from this overlay's own header word at image
 * offset 0xc. It lies BEFORE the first recorded owner, in the region
 * `gapsBetween` never read until 2026-08-01, and it is visible to the head
 * sweep only because the veneer predicate compares registers: `ldr r0,=X /
 * bx lr` is a LEAF, not a veneer, and the old predicate masked every one of
 * these as structure.
 *
 * SPAN IS 8 BYTES, NOT 4. The pool word sits PAST the `bx lr` and is read by
 * this row's own `ldr r0, [pc, #0]` -- pc 0x02000034 after alignment, plus 0 --
 * so it belongs to this owner. Recording 4 would orphan a word and
 * manufacture a phantom gap.
 *
 * The pool word 0x2009948 is an ADDRESS, loaded and returned WITHOUT being
 * dereferenced, so the row is a getter for an in-image table. One of the 29
 * head getters on ten overlays, and 28 of the 29 pool words are distinct.
 * The one repeat is 0x2009c34, returned by BOTH resource_395 0x48 and
 * resource_3ad 0x30 -- and that is a coincidence of ADDRESS, not shared
 * identity: only one overlay is resident at 0x02000000 at a time, so the
 * same VA designates different bytes depending on which is loaded.
 * Identical bytes are not identical semantics.
 *
 * 0 of 0 callees.
 */

/* Return this overlay's state block. */

/*
 * Resource 3cb scene-teardown step at 0x02000340 (88 bytes,
 * 0x02000340-0x02000397).
 *
 * Complete owner: `push {r5, lr}` prologue at 0x02000340 and the matching
 * interworking return `pop {r5} / pop {r1} / bx r1` at 0x02000380.  The popped
 * return address goes to r1, so r0 survives and IS the result — the value
 * returned by the final Func_0808a020 call.
 *
 * The sixteen bytes at 0x02000388-0x02000397 are this owner's literal pool
 * (0x03001ebc, 0x00000203, 0x00002927, 0x00000205), with two alignment bytes
 * at 0x02000386.  Execution reaches `bx r1` at 0x02000384 and never falls into
 * them; the `subs`/`lsls`/`cmp` the disassembler prints there is data.
 *
 * All eight call sites were resolved with cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml --
 * (true_target_offset = stored_displacement + 2), matching the inventory's
 * calls=8:
 *   0x02000348 -> 0x02000128  (this overlay's own leaf, see below)
 *   0x02000350 -> veneer 0x0200181c -> GameFlag_Clear
 *   0x02000356 -> veneer 0x0200181c -> GameFlag_Clear
 *   0x0200035a -> veneer 0x02001844 -> Func_0808a018
 *   0x0200036a -> veneer 0x02001884 -> Func_0808a170
 *   0x02000372 -> veneer 0x0200188c -> Func_0808a178
 *   0x02000378 -> veneer 0x0200181c -> GameFlag_Clear
 *   0x0200037c -> veneer 0x0200184c -> Func_0808a020
 *
 * 0x02000128 is reported `unknown` by the resolver only because it has no
 * `push` opening: it is a genuine prologue-less leaf in this overlay's own
 * image, ending in `bx lr` at 0x02000138, that copies one word of a table
 * into a slot selected by a byte lookup.  It is an ordinary call, not a
 * hidden-context target — nothing of this owner's frame or register state is
 * required by it beyond r0.  (It also uses r4 as unsaved call-clobbered
 * scratch, the idiom HANDOVER §0 records for this family; that is the image's
 * behaviour, not a decoding error.)
 *
 * Link base: resource_3cb is linked at 0x02008000, confirmed here by the
 * handler table at the tail of games/gs1/assets/code/resource_3cb_overlay.s, whose
 * entries 0x02009051, 0x02008861, 0x020082d9, 0x02008341 and 0x02009159 are
 * exactly Func_02001050, Func_02000860, Func_020002d8, Func_02000340 and
 * Func_02001158 plus the Thumb bit — and Func_02001050 is byte-exact in
 * games/gs1/assets/code.  Consequently the pool word 0x03001ebc here is NOT an in-image
 * address: it is the IWRAM workspace-pointer cell the rest of the overlay
 * loads directly (0x03001e70 + 76), so r5 is the workspace base.
 *
 * Behaviour: reset one workspace field and issue the fixed teardown sequence —
 * a table install through Func_02000128(4), three GameFlag_Clear flag clears
 * (512, 0x203, 0x205), a Func_0808a018 barrier, cue 0x2927 through
 * Func_0808a170, Func_0808a178(8, 0), and finally Func_0808a020 whose result
 * is returned.
 *
 * GameFlag_Clear is the overlay's flag-clear entry (GameFlag_IsSet tests and
 * GameFlag_Set sets; the polarity is forced by Func_020002d8's one-shot
 * timer).  Uncertainties: the meanings of the three flag words and of the
 * workspace halfword at +386 are not established; 386 is built as 193 << 1 and
 * the store is a halfword, so the field is 16-bit.  Func_0808a018 and
 * Func_0808a020 are reached with no argument register set by this owner.
 */

/* Old-style declarations: two Func_ names in this overlay can be one import
 * and arities vary per site, so interfaces are left open. */

/* Used for its return value. */

/* Deliberate no-op callback. */

u8 *Func_02000030(void)
{
    return (u8 *)0x02009948;
}

s32 Func_02000038(void)
{
    return 0;
}

u32 Func_0200003c(void) {
    volatile u16 *interruptMaster = (volatile u16 *)0x04000208;
    u32 savedInterruptMaster = *interruptMaster;
    u32 result;

    *interruptMaster = (u16)(u32)interruptMaster;
    Func_0200179a();
    result = Func_02001796();
    *interruptMaster = savedInterruptMaster;
    return result;
}

u8 *Func_02000058(void) { return (u8 *)0x020099f0; }

s32 Func_02000060(void) {
    s16 v = Data_02000240[225];

    if (v == 11 || v == 9) {
        return (s32)Data_02009c04;
    }
    return (s32)Data_020099f4;
}

s32 Func_02000340(void)
{
    u16 *workspace = *(u16 **)0x03001ebc;

    Func_02000472(4);
    Func_02001b6e(512);
    Func_02001b74(0x203);
    Func_02001ba0();

    /*
     * The halfword store goes through a pointer local and then an s32 value
     * local, in that order.  Storing the literal straight into the halfword
     * makes gcc build the constant in HImode and fetch it from the literal
     * pool (`ldrh r3, .L7'), which costs a pool word the reference does not
     * have; splitting the address out first also fixes which register holds
     * the address.
     */
    {
        /*
         * The halfword store goes through a pointer local and then an s32 value local,
         * in that order.  Storing the literal straight into the halfword makes gcc
         * build the constant in HImode and fetch it from the literal pool
         * (`ldrh r3, .L7'), which costs a pool word the reference does not have;
         * splitting the address out first also fixes which register holds the address.
         */
        u16 *frame = (u16 *)((u32)workspace + 386);
        s32 zero = 0;
        *frame = (u16)zero;

    }

    Func_02001bf0(0x2927);
    Func_02001c00(8, 0);
    Func_02001b96(0x205);
    return Func_02001bca();
}

s32 Func_0200053c(u16 *dest)
{
    s32 count = Func_02001d76();
    if (count > 3) count = 3;
    if (count > 0) {
        s16 *p = Data_02000240;
        const u8 *src;
        s32 remaining;
        p += 252;
        src = (const u8 *)p;
        remaining = count;
        do {
            u8 entry = *src++;
            if (dest != 0) { *dest = (u16)entry; dest++; }
            remaining--;
        } while (remaining != 0);
    }
    if (dest != 0) *dest = 0x00ff;
    return count;
}

s32 Func_02000f18(s32 arg0)
{
    Func_02002762(arg0);
    Func_020027f0(arg0);
    return Func_02002774();
}

void Func_02000f88(void) {}

u8 *Func_020011b4(void) { return (u8 *)0x02009e14; }

s32 Func_020011bc(void)
{
    s32 handle;
    Func_02002aa6(85);
    handle = Func_02002992(0x292a, 5, 4, 1);
    while (Func_020029a8() == 0)
        Func_020028ec(1);
    Func_020029d8();
    Func_020029a0(handle, 1);
    Func_02002906(1);
    handle = Func_020029c2(0x292b, 5, 4, 1);
    while (Func_020029d8_a() == 0)
        Func_0200291c(1);
    return Func_020029cc(handle, 1);
}

s32 Func_02001228(void)
{
    s32 handle;
    Func_02002b12(85);
    handle = Func_020029fe(0x292c, 5, 4, 1);
    while (Func_02002a14() == 0)
        Func_02002958(1);
    Func_02002a44();
    Func_02002a0c(handle, 1);
    Func_02002972(1);
    handle = Func_02002a2e(0x292d, 5, 4, 1);
    while (Func_02002a44_a() == 0)
        Func_02002988(1);
    return Func_02002a38(handle, 1);
}

s32 Func_02001294(s32 value)
{
    s32 column;

    if (value > 999) {
        value = 999;
    }

    for (column = 0; column <= 2; column++) {
        s32 digit = Func_020029b8(value, 10);

        Func_02002a4a(27, digit, 16 - column, 8, 1, 1);
        value = Func_020029ca(value, 10);
    }

    return Func_02002a56();
}
