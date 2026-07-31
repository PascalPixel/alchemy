typedef signed int s32;
typedef unsigned int u32;
typedef unsigned char u8;

/*
 * resource_3c4 owner at 0x020011e0, 144 bytes: code 0x020011e0-0x02001265, an
 * alignment halfword at 0x02001266 and the two pool words 0x02009069 and
 * 0x01030001 at 0x02001268-0x0200126f.
 *
 * A nine-step ring burst.  For each of the nine keys 0, 0x2000, ... 0x10000 it
 * asks two ROM dispatch stubs for a pair of values, lengthens the first by a
 * third with the relocated IWRAM helper, and spawns one effect at the
 * subject's position through this overlay's eight-argument spawner
 * Func_02000ae8 (four register arguments plus four stack words).
 *
 * The same construct as resource_39f's 0x02000e18, which is also 144 bytes;
 * the differences are the step of 2 in the loop counter and the richer options
 * record.
 *
 * Targets resolved with `tools/overlay_call_targets.ts` (overlay `bl` rule
 * `true_target_offset = stored_displacement + 2`): 5 sites, 5 distinct
 * callees.  The disassembler's 0x020042fa, 0x02004254, 0x02004258,
 * 0x0200423e and 0x02001d32 are artefacts.  The real callees are veneers
 * 0x0200310c (-> Func_0808a080), 0x02003044 (-> Func_08000120),
 * 0x0200303c (-> Func_08000118) and 0x02003014 (-> Func_03000380), plus this
 * overlay's prologue at 0x02000ae8.
 *
 * Func_08000120 and Func_08000118 are entries 12 and 11 of the ROM's fixed
 * dispatch stub table at 0x080000c0 (asm/080000c0.s), so they forward to
 * Func_0800231c and Func_08002322.  Func_03000380 is an ARM-mode helper
 * relocated into IWRAM, the same family as the 0x030001d8 square root; it is
 * called as (cursor, 3) and its result is added back to the cursor.
 *
 * Previously skipped because 0x02001d32 looked like an in-image address that
 * is not a function start; it was the decoding artefact.
 *
 * Link base 0x02008000, and the options record is a fourth witness for it: the
 * pool word 0x02009069 stored at options+36 is 0x02001068 plus the Thumb bit,
 * and 0x02001068 is a real prologue in this overlay.  Func_02000ae8 installs
 * that word at effect+108 as the effect's per-frame handler, which is exactly
 * what its 0x01000000 flag bit selects.
 *
 * flags = 0x01030001 selects three optional fields, and precisely those three
 * are initialised: 0x00010000 -> options+4 (7), 0x00020000 -> options+0 (1),
 * 0x01000000 -> options+36.  The remaining 28 bytes of the 40-byte record are
 * never written and never read.  The low nibble 1 is also the descriptor index
 * Func_02000ae8 uses.
 *
 * The epilogue is `add sp,#68 / pop {r3, r5, r6} / ... / pop {r0} / bx r0`, so
 * the owner is void.
 */

/* Returns the subject record whose words at +8, +12 and +16 are read. */
void Func_02000ae8();
s32 Func_03000380();
s32 Func_08000118();
s32 Func_08000120();
u8 *Func_0808a080();


/* This overlay's effect spawner: four register arguments plus four stack
 * words - (x, y, z, vx, vy, vz, flags, options). */

void Func_020011e0(s32 subject)
{
    u8 *record;
    u8 options[40];        /* sp+16 */
    s32 cursor;            /* sp+56 */
    s32 spare;             /* sp+60 */
    s32 extent;            /* sp+64 */
    s32 step;

    /* r0-r3 are untouched by the prologue, so the incoming argument reaches
     * this import unchanged. */
    record = Func_0808a080(subject);

    *(s32 *)&options[0] = 1;
    *(s32 *)&options[4] = 7;
    *(u32 *)&options[36] = 0x02009069;   /* Func_02001068 + Thumb bit */

    for (step = 0; step <= 16; step += 2) {
        s32 key = step << 12;

        cursor = Func_08000120(key);
        spare = 0;
        extent = Func_08000118(key);

        cursor = cursor + Func_03000380(cursor, 3);

        /* Register arguments r0-r3, then the four stack words sp+0..sp+12.
         * `spare` is stored 0 just above and reloaded here; it is kept as a
         * read of the local because that is what the assembly does. */
        Func_02000ae8(*(s32 *)(record + 8), *(s32 *)(record + 12),
                      *(s32 *)(record + 16),
                      cursor, spare, extent, 0x01030001, options);
    }
}
