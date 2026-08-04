/*
 * resource_38d owner at 0x020008c0, 3,790 bytes: 0x020008c0-0x0200218d.
 *
 * NO INVENTORY ROW.  This owner has no entry in `out/decomp/overlays.json`,
 * no exact sibling and no semantic source; it was found by sweeping the
 * two-byte gaps in `metrics/gs1-en-executable.json` for `push {..,lr}`
 * prologues, so its boundary is DERIVED and is the substance of the work.
 *
 * How the span was established.  A control-flow walk from the prologue at
 * 0x020008c0, following every branch and every `bl` whose `+2` target is not a
 * veneer / `call_via` / prologue (those are long tail branches, see below),
 * reaches its last instruction at 0x0200178c and closes on the single
 * matching epilogue `pop {r5} / pop {r0} / bx r0` at 0x02001788-0x0200178d.
 * There is exactly one epilogue and no `mov pc, rN` dispatch anywhere in the
 * body, so nothing is hidden behind a jump table.  Extent 0x020008c0-0x0200178e
 * = 3,790 bytes, of which 3,568 are instructions the walk actually reaches.
 *
 * Both boundaries are pinned by their neighbours rather than by alignment:
 *   * BELOW, the halfwords at 0x020008bc-0x020008bf (`0x0000`, `0xfffc`) are
 *     the tail of the previous owner's literal pool - objdump mis-pairs them
 *     into one 4-byte `0xfffc b520`, which is what hides this prologue from a
 *     linear scan in the first place.
 *   * ABOVE, 0x0200178e is a single `0x0000` alignment halfword, then this
 *     owner's own trailing pool 0x02001790-0x0200179b (0x0000037e,
 *     0x03001ebc, 0x00000321 - all three loaded from inside the body), and
 *     the NEXT function's prologue `push {lr}` begins at 0x0200179c with no
 *     further gap.  That neighbour is already converted as
 *     `resource_38d_c_0200179c.c`, which is an independent confirmation of
 *     this end.  The registered span stops at the last instruction, so the
 *     trailing pool is excluded and no coverage is claimed for it.
 *
 * POOL MAP - five interior literal pools, 222 bytes, none of them reachable as
 * an instruction, each jumped over by an unconditional `b.n` or stranded after
 * a tail branch:
 *   0x02000cd0-0x02000d1b  (76 bytes, 19 words)  jumped by `b.n` at 0x02000cce
 *   0x02000e1a-0x02000e2b  (18 bytes: one 0x0000 alignment halfword and four
 *                           words) stranded after the tail branch at 0x02000e16
 *   0x02001206-0x02001243  (62 bytes)            jumped by `b.n` at 0x02001204
 *   0x0200129e-0x020012a3  (6 bytes: alignment halfword + one word)
 *                                                jumped by `b.n` at 0x0200129c
 *   0x02001690-0x020016cb  (60 bytes)            jumped by `b.n` at 0x0200168e
 * Every `ldr rN, [pc, #imm]` in the body resolves into one of those five or
 * into the trailing pool, and no pool word is reached as code.
 *
 * ONE `bl` IS A LONG BRANCH, NOT A CALL.  At 0x02000e16 the stored
 * displacement resolves (rule: target = displacement + 2) to 0x0200177e, which
 * is inside this owner's own body and is neither a veneer nor a prologue - it
 * is the shared close `Func_0808a248(29); Func_0808a020();`.  Control does not
 * return, which is why the 18 bytes after it are pool rather than code; it is
 * written below as `goto close`, and `overlay_multiset_check.ts` excludes it
 * from the call multiset for the same reason.  Treating it as a call instead
 * would have run the walk straight into that pool and reported a much larger
 * function.
 *
 * Call accounting: 367 `bl` sites, one of them the long branch above, so 366
 * calls over 39 distinct targets, ALL of them import veneers - no intra-overlay
 * call and no `call_via` slot.  Resolved with
 * `bun tools/overlay_call_targets.ts resource_38d 08c0 178e`, never from
 * `overlay_show.ts`'s printed annotations.  Per-target multiset:
 * Func_0808a1b8 x59, Func_0808a100 x31, Func_0808a188 x30, Func_0808a110 x26,
 * Func_0808a010 x25, Func_0808a1e8 x24, Func_0808a0f0 x19, Func_0808a080 x16,
 * Func_0808a090 x13, Func_0808a138 x12, Func_0808a0d0 x10, Func_080770c0 x8,
 * Func_0808a170 x8, Func_0808a210 x8, Func_0808a0b8 x7, Func_0808a0e8 x7,
 * Func_0808a1f0 x7, Func_0808a0e0 x6, Func_0808a178 x6, Func_0808a070 x5,
 * Func_0808a168 x5, Func_080000c0 x4, Func_0808a0a8 x3, Func_0808a128 x3,
 * Func_0808a180 x3, Func_0808a370 x3, Func_080770c8 x2, Func_0808a0c8 x2,
 * Func_0808a130 x2, Func_0808a208 x2, Func_0808a368 x2, and one each of
 * Func_08009128, Func_080091e0, Func_0808a018, Func_0808a020, Func_0808a0c0,
 * Func_0808a228, Func_0808a248, Func_0808a360.
 *
 * WHAT IT IS.  The long scripted scene on the router at 0x020006e0, which
 * `bl`s here directly.  It opens the scene bracket, resets the camera, and
 * then runs in three variants selected by story flag 0x85f:
 *   * flag 0x85f ALREADY SET on entry  -> `replay`, the shortened re-entry.
 *   * flag clear -> the full staging pass, which sets 0x85f itself only on the
 *     aborted path.
 *   * either pass may be cut short by `Func_0808a070(0, 0)`, the skip
 *     predicate, which jumps to `aborted`: that arm shows line 0x1440, snaps
 *     every actor to its mark, sets 0x85f so a later entry takes the `replay`
 *     path, and closes.
 * The completed scene ends by setting story flag 0x321, which is exactly the
 * flag `resource_38d_c_0200179c.c` documents as its own entry condition, so
 * the two are consecutive scenes of one cutscene.
 *
 * The epilogue pops into r0, so the popped value is the return address and the
 * owner is `void`.  The prologue is `push {r5, lr}` and no argument register is
 * read before it is written, so the owner takes no arguments.  r5 is used once,
 * from 0x020015b8 onward, holding the in-image pointer 0x0200a5c0.  Under this
 * overlay's link base that is image offset 0x25c0 (`pool_word - 0x8000`), an
 * EVEN word and therefore in-image DATA, not a Thumb entry - it is passed as
 * the third argument of the five `Func_0808a168` calls.
 *
 * Immediates are written as the assembly builds them.  The `movs rN, #C /
 * lsls rN, rN, #k` habit appears throughout; the shifted values used here are
 * 192<<6 = 0x3000, 128<<5 = 0x1000, 128<<6 = 0x2000, 192<<7 = 0x6000,
 * 160<<7 = 0x5000, 224<<7 = 0x7000, 128<<8 = 0x8000, 160<<8 = 0xa000,
 * 192<<8 = 0xc000, 224<<8 = 0xe000, 128<<9 = 0x10000, 128<<10 = 0x20000,
 * 166<<18, 219<<18 and 129<<1 = 258.  Where the same shift produces a
 * displacement rather than a heading it is spelled as the product
 * (174<<2 = 696, 159<<2 = 636, 171<<2 = 684, 188<<2 = 752, 167<<2 = 668,
 * 213<<2 = 852, 216<<2 = 864, 168<<2 = 672, 219<<2 = 876, 224<<1 = 448,
 * 228<<1 = 456).  -1 is built six times by `movs rN, #1 / negs rN, rN`.
 *
 * UNCERTAINTIES:
 *  - Func_0808a360, Func_0808a368 and Func_0808a370 are called with no
 *    argument register written; spelled with no arguments, like the
 *    Func_0808a018 bracket.
 *  - Func_0808a228 returns a record pointer whose byte at +85 is cleared, the
 *    same +85 slot `resource_38d_c_020006e0.c` clears on four other records.
 *    Nothing here names the record.
 *  - Func_0808a210's four arguments are set explicitly at all eight sites and
 *    are asserted as such; its interface is not established.
 *  - The loop at 0x020010d2 enters when the skip predicate returns 1 and
 *    repeats while it does NOT return 1.  That asymmetry is in the assembly
 *    (`cmp r0, #1 / bne` at both the head and the back edge) and is preserved
 *    rather than tidied into a single sense.
 *  - The shared close is reached three ways and is written once, under
 *    `close:`; copying it per arm would put Func_0808a248 and Func_0808a020 in
 *    the source more often than the histogram has them.
 */

typedef signed short s16;
typedef signed int s32;
typedef unsigned char u8;

/* Imports. Old-style declarations: one name takes different argument counts at
 * different sites in this overlay. */
void Func_080000c0();           /* wait n frames */
void Func_08009128();           /* scene request; no arguments set */
void Func_080091e0();           /* set presentation mode (record, phase) */
s32 Func_080770c0();            /* story-flag test; used in a condition */
void Func_080770c8();           /* set a story flag */
void Func_0808a010();           /* wait n frames */
void Func_0808a018();           /* scripted-scene bracket: open */
void Func_0808a020();           /* scripted-scene bracket: close */
s32 Func_0808a070();            /* skip predicate; result selects the branch */
u8 *Func_0808a080();            /* record by slot index */
void Func_0808a090();           /* 16.16 scale pair */
void Func_0808a0a8();           /* per-slot release, one argument */
void Func_0808a0b8();           /* move slot to (x, z) */
void Func_0808a0c0();           /* place an entity, variant */
void Func_0808a0c8();           /* place an entity */
void Func_0808a0d0();           /* place an entity */
void Func_0808a0e0();           /* timed per-slot adjustment */
void Func_0808a0e8();           /* per-slot release */
void Func_0808a0f0();           /* place slot at (x, z), 16.16 */
void Func_0808a100();           /* set a slot's presentation mode */
void Func_0808a110();           /* scene-presentation request */
void Func_0808a128();           /* per-slot pairing request with a duration */
void Func_0808a130();           /* select a slot's pose */
void Func_0808a138();           /* select an entity presentation */
void Func_0808a168();           /* scene request with an in-image data table */
void Func_0808a170();           /* show a dialogue line by id */
void Func_0808a178();           /* dialogue-line variant with a mode word */
void Func_0808a180();           /* wait for the slot's action to finish */
void Func_0808a188();           /* wait for the slot's action, with a duration */
void Func_0808a1b8();           /* face slot along heading */
void Func_0808a1e8();           /* play animation id for n frames */
void Func_0808a1f0();           /* scene request; interface not established */
void Func_0808a210();           /* scene request; interface not established */
u8 *Func_0808a228();            /* record fetch; only its +85 byte is used */
void Func_0808a248();           /* scene-index handoff */
void Func_0808a360();           /* scene bookkeeping, no arguments */
void Func_0808a368();           /* scene bookkeeping, no arguments */
void Func_0808a370();           /* scene bookkeeping, no arguments */

void Func_020008c0(void)
{
    /* The in-image data table at image offset 0x25c0; held in r5 across the
     * five Func_0808a168 calls that use it. */
    const void *table = (const void *)0x0200a5c0;
    u8 *record;
    u8 *workspace;

    Func_0808a018();
    Func_0808a210(-1, -1, -1, 0);
    Func_080000c0(1);

    record = Func_0808a228();
    record[85] = 0;
    Func_0808a210(0x037e0000, -1, 166 << 18, 0);
    Func_080000c0(1);
    Func_0808a0f0(0, 0, 0);

    if (Func_080770c0(0x85f)) {
        Func_0808a210(0x037e0000, -1, 0x02ba0000, 0);
        Func_0808a0f0(19, 219 << 18, 0x027a0000);
        Func_0808a1b8(19, 0x3000, 0);
        Func_0808a0f0(0, 0x037e0000, 0x031e0000);
    }

    Func_08009128();
    Func_080000c0(1);

    workspace = *(u8 **)0x03001ebc;
    *(s32 *)(workspace + 448) = 256;
    *(s32 *)(workspace + 456) = 40;

    Func_0808a360();
    Func_0808a370();

    if (Func_080770c0(0x85f)) goto replay;

    Func_0808a010(80);
    Func_0808a0f0(19, 0x037e0000, 0x031e0000);
    Func_0808a208(0x9999, 0x1333);
    Func_0808a210(0x037e0000, -1, 0x02ba0000, 1);
    Func_0808a090(19, 0xcccc, 0x6666);
    Func_0808a0c8(19, 0x37e, 696);
    Func_0808a010(80);
    Func_0808a210(0x037e0000, -1, 166 << 18, 1);
    Func_0808a0e8(19);
    Func_0808a0d0(19, 0x34a, 696);
    Func_0808a0d0(19, 0x34a, 636);
    Func_0808a1b8(18, 0x7000, 20);
    Func_0808a0d0(19, 876, 0x27a);
    Func_0808a110(19, 3);
    Func_0808a010(20);
    Func_0808a110(18, 3);
    Func_0808a010(10);

    Func_0808a170(0x1437);
    Func_0808a188(0x2012, 0, 10);
    Func_0808a138(19, 2);
    Func_0808a188(19, 0, 20);
    Func_0808a138(18, 1);
    Func_0808a188(0x2012, 0, 10);
    Func_0808a110(19, 3);
    Func_0808a010(40);
    Func_0808a1e8(18, 0x105, 60);
    Func_0808a180(0x2012, 0);
    Func_0808a138(18, 1);
    Func_0808a188(0x2012, 0, 10);
    Func_0808a1f0(19, 258);
    Func_0808a010(60);
    Func_0808a1b8(19, 0x3000, 10);
    Func_0808a1b8(18, 0x5000, 10);

    Func_0808a210(0x037e0000, -1, 0x02ba0000, 1);
    Func_0808a0f0(0, 0x037e0000, 0x031e0000);
    Func_0808a090(0, 0x9999, 0x4ccc);
    Func_0808a0d0(0, 0x37e, 0x2d6);
    Func_0808a010(20);
    Func_0808a110(0, 3);
    Func_0808a010(10);
    Func_0808a138(18, 1);
    Func_0808a180(0x2012, 0);
    Func_0808a210(0x037e0000, -1, 166 << 18, 1);
    Func_0808a0d0(0, 0x37e, 684);

    record = Func_0808a080(0);
    if (record != 0) Func_0808a0f0(1, *(s32 *)(record + 8), *(s32 *)(record + 16));
    record = Func_0808a080(0);
    if (record != 0) Func_0808a0f0(2, *(s32 *)(record + 8), *(s32 *)(record + 16));
    if (Func_080770c0(3)) {
        record = Func_0808a080(0);
        if (record != 0) Func_0808a0f0(3, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }

    Func_0808a090(1, 0x9999, 0x4ccc);
    Func_0808a090(2, 0x9999, 0x4ccc);
    Func_0808a090(3, 0x10000, 0x8000);
    Func_0808a100(1, 2);
    Func_0808a100(2, 2);
    Func_0808a100(3, 2);
    Func_0808a0e0(1, -16, 16);
    Func_0808a0e0(2, 16, 16);
    if (Func_080770c0(3)) Func_0808a0e0(3, 32, 16);
    Func_0808a0e8(2);
    Func_0808a100(1, 1);
    Func_0808a100(2, 1);
    Func_0808a100(3, 1);
    Func_0808a010(10);
    Func_0808a1b8(3, 0xc000, 0);
    Func_0808a1b8(1, 0xc000, 0);
    Func_0808a1b8(2, 0xc000, 20);

    Func_0808a128(18, 2, 20);
    Func_0808a1b8(18, 0x7000, 10);
    Func_0808a188(0x2012, 0, 10);
    Func_0808a1b8(19, 0x1000, 10);
    Func_0808a110(19, 3);
    Func_0808a1b8(18, 0x5000, 40);
    Func_0808a1b8(18, 0x7000, 10);
    Func_0808a110(18, 4);
    Func_0808a188(0x2012, 0, 10);
    Func_0808a1f0(19, 258);
    Func_0808a010(40);
    Func_0808a1b8(18, 0x5000, 20);
    Func_0808a1e8(18, 0x105, 40);
    Func_0808a178(0x2012, 0);
    Func_0808a1b8(1, 0xe000, 0);
    Func_0808a1b8(2, 0xa000, 0);

    if (Func_0808a070(0, 0) != 0) goto aborted;
    goto staged;

aborted:
    Func_0808a170(0x1440);
    Func_0808a188(0x2012, 0, 10);
    Func_0808a1b8(19, 0x3000, 0);
    Func_0808a110(18, 4);
    Func_0808a188(0x2012, 0, 10);
    Func_0808a1b8(1, 0xc000, 0);
    Func_0808a1b8(2, 0xc000, 0);
    Func_0808a110(0, 3);

    Func_0808a100(1, 2);
    record = Func_0808a080(0);
    if (record != 0) Func_0808a0b8(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    Func_0808a100(2, 2);
    record = Func_0808a080(0);
    if (record != 0) Func_0808a0b8(2, *(s16 *)(record + 10), *(s16 *)(record + 18));
    if (Func_080770c0(3)) {
        Func_0808a100(3, 2);
        record = Func_0808a080(0);
        if (record != 0) Func_0808a0b8(3, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }

    Func_0808a0e8(2);
    Func_0808a0f0(1, 0, 0);
    Func_0808a0f0(2, 0, 0);
    Func_0808a0f0(3, 0, 0);
    Func_080770c8(0x85f);
    Func_0808a090(0, 0x10000, 0x8000);
    Func_0808a0d0(0, 0x37e, 752);

    workspace = *(u8 **)0x03001ebc;
    *(s32 *)(workspace + 456) = 16;

    Func_0808a368();
    Func_0808a370();
    goto close;

replay:
    Func_0808a090(0, 0x9999, 0x4ccc);
    Func_0808a0c8(0, 0x37e, 684);
    Func_0808a010(80);
    Func_0808a208(0x9999, 0x1333);
    Func_0808a210(0x037e0000, -1, 166 << 18, 1);
    Func_0808a0e8(0);
    Func_0808a100(0, 1);

    record = Func_0808a080(0);
    if (record != 0) Func_0808a0f0(1, *(s32 *)(record + 8), *(s32 *)(record + 16));
    record = Func_0808a080(0);
    if (record != 0) Func_0808a0f0(2, *(s32 *)(record + 8), *(s32 *)(record + 16));
    if (Func_080770c0(3)) {
        record = Func_0808a080(0);
        if (record != 0) Func_0808a0f0(3, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }

    Func_0808a090(1, 0x9999, 0x4ccc);
    Func_0808a090(2, 0x9999, 0x4ccc);
    Func_0808a090(3, 0x10000, 0x8000);
    Func_0808a100(1, 2);
    Func_0808a100(2, 2);
    Func_0808a100(3, 2);
    Func_0808a0e0(1, -16, 16);
    Func_0808a0e0(2, 16, 16);
    if (Func_080770c0(3)) Func_0808a0e0(3, 32, 16);
    Func_0808a0e8(2);
    Func_0808a100(1, 1);
    Func_0808a100(2, 1);
    Func_0808a100(3, 1);
    Func_0808a010(10);
    Func_0808a1b8(3, 0xc000, 0);
    Func_0808a1b8(1, 0xc000, 0);
    Func_0808a1b8(2, 0xc000, 20);
    Func_0808a1e8(18, 0x101, 60);
    Func_0808a170(0x1442);
    Func_0808a178(0x2012, 0);

    if (Func_0808a070(0, 0) == 1) goto aborted;

staged:
    Func_0808a1b8(3, 0xc000, 0);
    Func_0808a1b8(0, 0xc000, 0);
    Func_0808a1b8(1, 0xc000, 0);
    Func_0808a1b8(2, 0xc000, 20);
    Func_0808a100(3, 3);
    Func_0808a100(0, 3);
    Func_0808a100(1, 3);
    Func_0808a110(2, 3);
    Func_0808a1e8(18, 0x105, 60);
    Func_0808a170(0x1443);
    Func_0808a180(0x2012, 0);

    Func_080091e0(Func_0808a080(20), 0);
    record = Func_0808a080(20);
    *(s32 *)(record + 24) = 0x8000;
    *(s32 *)(record + 28) = 0x8000;

    record = Func_0808a080(18);
    if (record != 0) Func_0808a0f0(20, *(s32 *)(record + 8), *(s32 *)(record + 16));
    Func_080000c0(1);
    Func_0808a128(20, 6, 0);
    Func_0808a090(20, 0x20000, 0x10000);
    Func_0808a0c0(20, 0x37e, 668);
    Func_0808a010(40);
    Func_0808a188(0x2012, 0, 10);

    Func_0808a1e8(3, 0x101, 0);
    Func_0808a1e8(0, 0x101, 0);
    Func_0808a1e8(1, 0x101, 0);
    Func_0808a1e8(2, 0x101, 60);
    Func_0808a110(18, 4);
    Func_0808a188(0x2012, 0, 10);
    Func_0808a1e8(1, 0x103, 60);
    Func_0808a1b8(1, 0xe000, 10);
    Func_0808a178(0x4001, 0);
    Func_0808a1b8(3, 0xa000, 0);
    Func_0808a1b8(2, 0xa000, 0);
    Func_0808a1b8(0, 0x6000, 0);

    if (Func_0808a070(0, 0) == 1) {
        do {
            Func_0808a130(1, 2);
            Func_0808a138(2, 2);
            Func_0808a170(0x1447);
            Func_0808a178(0x4001, 0);
        } while (Func_0808a070(0, 0) != 1);
    }

    Func_0808a110(1, 3);
    Func_0808a170(0x1448);
    Func_0808a188(0x4001, 0, 10);
    Func_0808a1b8(3, 0xc000, 0);
    Func_0808a1b8(0, 0xc000, 0);
    Func_0808a1b8(1, 0xc000, 0);
    Func_0808a1b8(2, 0xc000, 10);
    Func_0808a100(0, 3);
    Func_0808a110(0, 3);
    Func_0808a010(20);
    Func_0808a1e8(18, 0x105, 60);
    Func_0808a188(0x2012, 0, 20);
    Func_0808a1e8(2, 258, 60);
    Func_0808a1b8(18, 0x3000, 10);
    Func_0808a1e8(18, 0x101, 60);
    Func_0808a1e8(1, 0x101, 40);
    Func_0808a1b8(1, 0, 20);
    Func_0808a138(1, 1);
    Func_0808a188(0x4001, 0, 10);
    Func_0808a1b8(3, 0x8000, 0);
    Func_0808a1b8(0, 0x2000, 20);
    Func_0808a1e8(2, 258, 60);
    Func_0808a1b8(2, 0x8000, 10);
    Func_0808a188(0x4002, 0, 10);
    Func_0808a138(1, 1);

    Func_0808a010(40);
    Func_0808a110(1, 3);
    Func_0808a188(0x4001, 0, 10);
    Func_0808a1b8(1, 0xe000, 10);
    Func_0808a130(1, 1);
    Func_0808a178(0x4001, 0);
    Func_0808a1b8(3, 0xa000, 0);
    Func_0808a1b8(2, 0xa000, 0);
    Func_0808a1b8(0, 0x6000, 0);

    do {
        Func_0808a170(0x144e);
        Func_0808a178(0x4001, 0);
    } while (Func_0808a070(0, 0) != 0);

    Func_0808a010(10);
    Func_0808a1b8(3, 0x8000, 0);
    Func_0808a1b8(1, 0, 10);
    Func_0808a1b8(0, 0x2000, 10);
    Func_0808a110(1, 3);
    Func_0808a1e8(2, 0x105, 60);
    Func_0808a1b8(2, 0x8000, 10);
    Func_0808a110(2, 4);
    Func_0808a170(0x144f);
    Func_0808a188(0x4002, 0, 20);
    Func_0808a138(18, 1);
    Func_0808a1b8(18, 0x5000, 10);
    Func_0808a110(18, 4);
    Func_0808a188(0x2012, 0, 10);
    Func_0808a1b8(3, 0xc000, 0);
    Func_0808a1b8(0, 0xc000, 0);
    Func_0808a1b8(1, 0xc000, 0);
    Func_0808a1b8(2, 0xc000, 20);
    Func_0808a100(3, 3);
    Func_0808a100(0, 3);
    Func_0808a100(1, 3);
    Func_0808a110(2, 3);
    Func_0808a010(40);
    Func_0808a1e8(18, 0x105, 80);
    Func_0808a1b8(19, 0x1000, 10);
    Func_0808a1f0(19, 258);
    Func_0808a010(40);
    Func_0808a188(19, 0, 10);
    Func_0808a138(18, 1);
    Func_0808a010(40);
    Func_0808a1b8(18, 0x7000, 20);
    Func_0808a188(0x2012, 0, 10);
    Func_0808a110(19, 3);
    Func_0808a010(20);
    Func_0808a110(18, 4);
    Func_0808a100(18, 4);
    Func_0808a188(0x2012, 0, 10);
    Func_0808a128(20, 6, 0);

    record = Func_0808a080(18);
    if (record != 0) Func_0808a0b8(20, *(s16 *)(record + 10), *(s16 *)(record + 18));

    Func_0808a0e8(20);
    Func_0808a0f0(20, 0, 0);
    Func_0808a010(20);
    Func_0808a1f0(3, 258);
    Func_0808a1f0(0, 258);
    Func_0808a1f0(1, 258);
    Func_0808a1f0(2, 258);
    Func_0808a010(40);
    Func_0808a138(19, 2);
    Func_0808a188(19, 0, 10);
    Func_0808a110(18, 3);
    Func_0808a188(0x2012, 0, 20);
    Func_0808a110(19, 3);
    Func_0808a138(2, 2);
    Func_0808a010(20);
    Func_0808a188(0x4002, 0, 10);
    Func_0808a1b8(19, 0x3000, 0);
    Func_0808a1b8(18, 0x3000, 10);
    Func_0808a110(18, 4);
    Func_0808a188(0x2012, 0, 10);
    Func_0808a1e8(1, 0x103, 60);
    Func_0808a188(0x4001, 0, 10);
    Func_0808a1b8(18, 0x5000, 10);
    Func_0808a100(18, 4);
    Func_0808a188(0x2012, 0, 10);
    Func_0808a100(2, 4);
    Func_0808a188(0x4002, 0, 10);
    Func_0808a1b8(18, 0x3000, 10);
    Func_0808a110(18, 3);
    Func_0808a188(0x2012, 0, 10);
    Func_0808a1e8(3, 0x107, 0);
    Func_0808a1e8(0, 0x107, 0);
    Func_0808a1e8(1, 0x107, 0);
    Func_0808a1e8(2, 0x107, 60);
    Func_0808a1b8(18, 0x7000, 10);
    Func_0808a110(18, 3);
    Func_0808a010(10);
    Func_0808a188(0x2012, 0, 10);
    Func_0808a138(19, 2);
    Func_0808a1b8(19, 0x1000, 10);
    Func_0808a110(19, 3);
    Func_0808a010(20);

    Func_0808a168(0, 0x10013, table);
    Func_0808a168(1, 0x10013, table);
    Func_0808a168(2, 0x10013, table);
    Func_0808a168(3, 0x10013, table);
    Func_0808a090(19, 0x9999, 0x4ccc);
    Func_0808a0d0(19, 852, 0x286);
    Func_0808a0d0(19, 852, 0x29a);
    Func_0808a0d0(19, 864, 672);
    Func_0808a1b8(19, 0x1000, 10);
    Func_0808a188(0x4013, 0, 20);
    Func_0808a0a8(0);
    Func_0808a0a8(1);
    Func_0808a0a8(2);
    Func_0808a1e8(0, 0x105, 0);
    Func_0808a1e8(1, 0x105, 0);
    Func_0808a1e8(2, 0x105, 60);
    Func_0808a100(0, 3);
    Func_0808a100(1, 3);
    Func_0808a110(2, 3);
    Func_0808a168(19, 0x10000, table);

    Func_0808a100(1, 2);
    record = Func_0808a080(0);
    if (record != 0) Func_0808a0b8(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    Func_0808a100(2, 2);
    record = Func_0808a080(0);
    if (record != 0) Func_0808a0b8(2, *(s16 *)(record + 10), *(s16 *)(record + 18));
    if (Func_080770c0(3)) {
        Func_0808a100(3, 2);
        record = Func_0808a080(0);
        if (record != 0) Func_0808a0b8(3, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }

    Func_0808a0e8(2);
    Func_0808a0f0(1, 0, 0);
    Func_0808a0f0(2, 0, 0);
    Func_0808a0f0(3, 0, 0);
    Func_0808a090(0, 0x10000, 0x8000);
    Func_0808a0d0(0, 0x37e, 752);

    workspace = *(u8 **)0x03001ebc;
    *(s32 *)(workspace + 456) = 16;

    Func_0808a368();
    Func_0808a370();
    Func_080770c8(0x321);

close:
    Func_0808a248(29);
    Func_0808a020();
}
