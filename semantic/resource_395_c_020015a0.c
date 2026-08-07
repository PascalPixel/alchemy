typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

/*
 * Resource 395 periodic burst emitter at 0x020015a0 (396 bytes,
 * 0x020015a0-0x0200172b).
 *
 * Complete owner: `push {r5, r6, r7, lr}` plus the high-register save
 * `mov r7,sl / mov r6,r9 / mov r5,r8 / push {r5, r6, r7}` at 0x020015a0, and
 * the matching interworking return at 0x02001706
 * (`pop {r3, r5, r6}` -> r8/r9/sl, `pop {r5, r6, r7}`, `pop {r0} / bx r0`).
 * The popped return address lands in r0, so nothing is returned and the owner
 * is `void`.
 *
 * POOL / TABLE MAP, derived from a control-flow walk rather than from the
 * listing (overlay_show.ts happily decodes both regions as plausible
 * `str rN,[sp,#imm]` / `ldr rN,[sp,#imm]` pairs):
 *   0x020015c0-0x02001663  the 41-entry jump table dispatched by the
 *                          `mov pc, r3` at 0x020015be.  Never executed.
 *   0x02001714-0x0200172b  the owner's six-word literal pool, after the
 *                          epilogue.
 * Everything between 0x02001664 and 0x02001713 is code.  Note that the table
 * ends mid-listing at 0x02001664, which is itself a table target, so the two
 * regions abut with no branch between them.
 *
 * The jump table proves the 0x02008000 link base twice over.  Its base pool
 * word is 0x020095c0 while the table is physically at file offset 0x15c0, and
 * its entries are only two distinct values, 0x02009664 (= 0x1664, the spawn
 * block) and 0x020096f8 (= 0x16f8, the counter tail) — both even, as jump
 * table entries must be, because `mov pc, rN` does not interwork.  The
 * spawn entry occupies indices 0, 10, 20, 30 and 40 exactly, i.e. the emitter
 * fires once every ten frames.  A third witness is the pool word 0x0200952d
 * = Func_0200152c + 1 (Thumb), installed at record + 0x6c: that is this
 * overlay's own per-frame step routine, already converted in
 * semantic/overlays/resource_395_c_0200152c.c, and its record layout (+0x38
 * anchor triple, +0x64 step counter, +0x66 heading) is exactly what is
 * initialised here.
 *
 * The other two pointer pool words are ABOVE the image end (0x1dbc), so like
 * the 0x02009dd0/0x02009dd4 words in the sibling
 * semantic/overlays/resource_395_c_02001218.c they are the loader's working
 * RAM behind the loaded overlay, not image data:
 *   0x02009dcc — the frame counter this owner advances (s32).
 *   0x02009dc0 — a three-word (x, y, z) 16.16 origin written elsewhere.
 * 0x0000011d and 0x00019999 are plain constants.
 *
 * All six call sites were resolved with tools/lib/overlay_call_targets.ts
 * (--annotate, and site->target read from --json, never from the summary
 * histogram); overlay_show.ts's own `bl` annotations are wrong on every
 * overlay because an overlay `bl` stores target_offset - 2.  Sites, in address
 * order, and the placed multiset, which matches them one for one:
 *   0x02001666 -> Func_080f9010     0x0200167c -> Func_080090c8
 *   0x0200168a -> Func_0808a498     0x020016ae -> Func_080091e0
 *   0x020016b6 -> Func_08009080     0x020016c8 -> Func_030003f0
 * Six sites, six distinct targets, one C call each — verified per target, not
 * by the total.  All six sit inside the single loop body, which executes six
 * times; the six C call expressions are the six sites, not the eighteen
 * dynamic calls.  The inventory's calls=0 for this row is the documented
 * undercount: its scan mistook the embedded jump table for code.
 *
 * Behaviour: on frames 0, 10, 20, 30 and 40 of a 121-frame cycle, play cue
 * 220 and create six effect records fanned evenly around a circle — record i
 * is given heading (i * 60 degrees) converted to the hardware's 65536-unit
 * angle by the relocated IWRAM divide helper.  Each record is anchored at the
 * shared origin, put in presentation mode 1, and given Func_0200152c as its
 * per-frame step.  On every frame, advance the counter and wrap it after 120.
 *
 * Uncertainties, recorded rather than smoothed over:
 *  - r9 threads the result of Func_0808a498 from one loop iteration into the
 *    next as that call's second argument, and is seeded with 0 at 0x020015b0,
 *    before the dispatch.  It survives the `if (record == 0)` skip, so a
 *    failed creation leaves the previous value in place; modelled literally.
 *  - Func_030003f0 is the relocated IWRAM quotient helper (see
 *    semantic/overlays/resource_3ca_c_020000b0.c, which reaches the same
 *    address through this same kind of veneer with no r3/r4 load, so it is
 *    NOT a call_via site).  The call is preserved verbatim rather than written
 *    as `/`, because the quotient/remainder identification rests on the
 *    neighbouring entry point rather than on a disassembly of the IWRAM copy.
 *  - the bit edit at linked[9] is `(b & ~0x0c) | 0x04`: `movs r0,#13 /
 *    negs r0,r0` builds 0xfffffff3, i.e. ~0x0c, not ~0x0d.  The byte-exact
 *    family and the sibling 0x02001218 agree.
 *  - the +0x64 step counter is written as a halfword from a register the
 *    compiler holds at zero (sl); written as literal 0.
 */

void Func_080f9010();           /* play a sound cue */
u8 *Func_080090c8();            /* create an effect record, or 0 */
s32 Func_0808a498();            /* register the record with the emitter chain */
void Func_080091e0();           /* set presentation phase (record, phase) */
void Func_08009080();           /* select presentation mode (record, mode) */
s32 Func_030003f0();            /* relocated IWRAM quotient helper */

void Func_020015a0(void)
{
    s32 *counter = (s32 *)0x02009dcc;
    s32 *origin = (s32 *)0x02009dc0;   /* x, y, z as 16.16 */
    s32 frame;
    s32 chained;
    s32 i;
    s32 degrees;

    frame = *counter;
    chained = 0;

    /* 41-entry jump table: every tenth frame selects the spawn block. */
    if ((u32)frame <= 40 && (frame % 10) == 0) {
        Func_080f9010(220);

        degrees = 0;
        for (i = 0; i <= 5; i++) {
            u8 *record;

            record = Func_080090c8(0x11d, origin[0], origin[1], origin[2]);
            if (record != 0) {
                u8 *linked = *(u8 **)(record + 0x50);

                chained = Func_0808a498(linked, chained);

                record[0x55] = 0;

                linked = *(u8 **)(record + 0x50);
                linked[9] = (u8)((linked[9] & ~0x0c) | 0x04);

                Func_080091e0(record, 0);
                Func_08009080(record, 1);

                *(u16 *)(record + 0x64) = 0;
                *(u16 *)(record + 0x66) = (u16)Func_030003f0(degrees, 360);

                *(s32 *)(record + 0x38) = origin[0];
                *(s32 *)(record + 0x3c) = origin[1];
                *(s32 *)(record + 0x40) = origin[2];
                *(s32 *)(record + 0x30) = 0x19999;
                /* Func_0200152c + 1 under the proven 0x02008000 base. */
                *(s32 *)(record + 0x6c) = 0x0200952d;
            }

            /* 240 << 14 = 0x003c0000, i.e. 60.0 in 16.16 degrees. */
            degrees += 0x003c0000;
        }
    }

    frame = *counter + 1;
    *counter = frame;
    if (frame > 120) {
        *counter = 0;
    }
}
