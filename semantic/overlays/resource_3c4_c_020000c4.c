typedef signed short s16;
typedef signed int s32;
typedef unsigned short u16;
typedef unsigned char u8;

/*
 * resource_3c4 owner at 0x020000c4, 384 bytes: code 0x020000c4-0x02000235, an
 * alignment halfword at 0x02000236 and the three pool words 0x0200b1f0,
 * 0xffff0000 and 0x00003333 at 0x02000238-0x02000243.
 *
 * The party push routine.  It looks one step ahead of the party along its
 * current heading; if a pushable object occupies that cell, it checks that the
 * cell beyond the object and the cell above it are both clear and unlocked,
 * that the terrain test passes, and that the object is not already busy - and
 * then starts both the object and the party moving to the pushed position.
 *
 * SHARED ROUTINE.  This owner is halfword-identical to resource_39f's
 * 0x020000c4 over all 192 halfwords except the seven `bl` displacements (which
 * differ only because the two overlays place their import veneer tables at
 * different offsets) and the one pool word naming this overlay's own copy of
 * the direction table.  `tools/overlay_call_targets.ts` reports the *same*
 * resolved profile for both - 13 sites, 9 distinct targets, the same eight
 * imports plus 0x0200006c three times - and resource_3c4's 0x0200006c is
 * itself byte-identical to resource_39f's, with byte-exact C already in
 * assets/code/resource_3c4_c_0200006c.c.  The reconstruction below is
 * therefore the same as resource_39f_c_020000c4.c with this overlay's data
 * symbol.
 *
 * Link base: 0x02008000, so Data_0200b1f0 is the in-image table at file offset
 * 0x31f0.  Its sixteen words were read out and match resource_39f's table
 * word for word: entries 0-2 and 15 are (+16, 0), 3-6 are (0, +16), 7-10 are
 * (-16, 0) and 11-14 are (0, -16), X step in the high halfword and Z step in
 * the low halfword.  The X step is promoted to 16.16 by masking with
 * 0xffff0000 and the Z step by shifting left 16 - never by a multiply.
 *
 * Call targets were resolved with `tools/overlay_call_targets.ts`, i.e. under
 * the rule `true_target_offset = stored_displacement + 2`.  The three lookups
 * at 0x02000108, 0x02000134 and 0x02000160 carry the same stored displacement,
 * so pc-relative decoding prints three different targets - 0x02000176,
 * 0x020001a2 and 0x020001ce - each a plain join point inside this very
 * function.  All three are 0x0200006c.
 *
 * The epilogue is `add sp, #12 / pop {r3, r5, r6} / ... / pop {r0} / bx r0`, so
 * the owner is void.
 */

extern s32 Data_0200b1f0[];   /* 16 packed direction words, file offset 0x31f0 */

/* Returns the party record. */
u8 *Func_0808a080();

/* Byte-exact in assets/code: returns the slot occupying the probe cell, or 0.
 * Old-style because these sites also set r1, which that source does not read. */
s32 *Func_0200006c();

/* Returns a signed terrain result; positive blocks the push. */
s32 Func_080091d8();

void Func_08009080();
void Func_080000c0();      /* ROM dispatch[0] -> Func_080030f8 */
void Func_080f9010();
void Func_08009150();
void Func_08009158();
void Func_0808a5e8();

void Func_020000c4(void)
{
    u8 *party;
    s32 step;
    s32 stepIndex;
    s32 probe[3];
    u8 *object;
    s32 *blocker;
    u8 busy;

    party = Func_0808a080(0);

    /* heading is the top nibble of the halfword at +6; the scaled index is
     * kept live in a register and the table is re-read from it below. */
    stepIndex = *(u16 *)(party + 6) >> 12;
    step = Data_0200b1f0[stepIndex];

    probe[0] = *(s32 *)(party + 8) + (step & (s32)0xffff0000);
    probe[1] = *(s32 *)(party + 12);
    probe[2] = *(s32 *)(party + 16) + (step << 16);

    object = (u8 *)Func_0200006c(probe, party);
    if (object == 0) return;

    /* One further step in the same direction, measured from the object. */
    step = Data_0200b1f0[stepIndex];
    probe[0] = *(s32 *)(object + 8) + (step & (s32)0xffff0000);
    probe[1] = *(s32 *)(object + 12);
    probe[2] = *(s32 *)(object + 16) + (step << 16);

    blocker = Func_0200006c(probe, object);
    if (blocker != 0 && (((u8 *)blocker)[89] & 1) != 0) return;

    /* And the cell directly above the object: 128 << 13 in Y. */
    probe[0] = *(s32 *)(object + 8);
    probe[1] = *(s32 *)(object + 12) + 0x100000;
    probe[2] = *(s32 *)(object + 16);

    blocker = Func_0200006c(probe, object);
    if (blocker != 0 && (((u8 *)blocker)[89] & 1) != 0) return;

    object[34] = 2;

    /* The destination is rebuilt one step past the object; the probe now holds
     * the pushed position and is reused as the target for both movers. */
    step = Data_0200b1f0[stepIndex];
    probe[0] = *(s32 *)(object + 8) + (step & (s32)0xffff0000);
    probe[1] = *(s32 *)(object + 12);
    probe[2] = *(s32 *)(object + 16) + (step << 16);

    if (Func_080091d8(object, probe) > 0) return;

    busy = object[98];
    if (busy != 0) return;

    Func_08009080(party, 8);
    Func_080000c0(15);
    Func_080f9010(185);

    /* 0x3333 is the shared move speed for both records. */
    *(s32 *)(object + 48) = 0x3333;
    *(s32 *)(object + 52) = 0x3333;
    Func_08009150(object, probe[0], probe[1], probe[2]);

    *(s32 *)(party + 48) = 0x3333;
    *(s32 *)(party + 52) = 0x3333;
    Func_08009150(party, probe[0], probe[1], probe[2]);

    Func_08009158(object);

    /* No argument registers are set for this one. */
    Func_0808a5e8();

    *(s32 *)(object + 8) = probe[0];
    *(s32 *)(object + 16) = probe[2];

    /* `busy` is provably 0 on this path; it is the register the original
     * reuses as the zero source for all four of these stores. */
    *(s32 *)(object + 36) = busy;
    *(s32 *)(object + 44) = busy;

    *(s32 *)(party + 56) = (s32)0x80000000;
    *(s32 *)(party + 64) = (s32)0x80000000;
    *(s32 *)(party + 36) = busy;
    *(s32 *)(party + 44) = busy;

    /* The party is snapped to its own tile centres, read as signed halfwords
     * at +10 and +18 and promoted to 16.16. */
    *(s32 *)(party + 8) = (s32)(*(s16 *)(party + 10)) << 16;
    *(s32 *)(party + 16) = (s32)(*(s16 *)(party + 18)) << 16;

    Func_08009080(party, 1);
}
