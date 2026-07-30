typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;

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
 * All eight call sites were resolved with tools/overlay_call_targets.ts
 * (true_target_offset = stored_displacement + 2), matching the inventory's
 * calls=8:
 *   0x02000348 -> 0x02000128  (this overlay's own leaf, see below)
 *   0x02000350 -> veneer 0x0200181c -> Func_080770d0
 *   0x02000356 -> veneer 0x0200181c -> Func_080770d0
 *   0x0200035a -> veneer 0x02001844 -> Func_0808a018
 *   0x0200036a -> veneer 0x02001884 -> Func_0808a170
 *   0x02000372 -> veneer 0x0200188c -> Func_0808a178
 *   0x02000378 -> veneer 0x0200181c -> Func_080770d0
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
 * handler table at the tail of assets/code/resource_3cb_overlay.s, whose
 * entries 0x02009051, 0x02008861, 0x020082d9, 0x02008341 and 0x02009159 are
 * exactly Func_02001050, Func_02000860, Func_020002d8, Func_02000340 and
 * Func_02001158 plus the Thumb bit — and Func_02001050 is byte-exact in
 * assets/code.  Consequently the pool word 0x03001ebc here is NOT an in-image
 * address: it is the IWRAM workspace-pointer cell the rest of the overlay
 * loads directly (0x03001e70 + 76), so r5 is the workspace base.
 *
 * Behaviour: reset one workspace field and issue the fixed teardown sequence —
 * a table install through Func_02000128(4), three Func_080770d0 mode words
 * (512, 0x203, 0x205), a Func_0808a018 barrier, cue 0x2927 through
 * Func_0808a170, Func_0808a178(8, 0), and finally Func_0808a020 whose result
 * is returned.
 *
 * Uncertainties: the meanings of the three Func_080770d0 words and of the
 * workspace halfword at +386 are not established; 386 is built as 193 << 1 and
 * the store is a halfword, so the field is 16-bit.  Func_0808a018 and
 * Func_0808a020 are reached with no argument register set by this owner.
 */

/* Old-style declarations: two Func_ names in this overlay can be one import
 * and arities vary per site, so interfaces are left open. */
void Func_02000128();
void Func_080770d0();
void Func_0808a170();
void Func_0808a178();
void Func_0808a018();

/* Used for its return value. */
s32 Func_0808a020();

s32 Func_02000340(void)
{
    u16 *workspace = *(u16 **)0x03001ebc;

    Func_02000128(4);
    Func_080770d0(512);
    Func_080770d0(0x203);
    Func_0808a018();

    *(u16 *)((u32)workspace + 386) = 0;

    Func_0808a170(0x2927);
    Func_0808a178(8, 0);
    Func_080770d0(0x205);
    return Func_0808a020();
}
