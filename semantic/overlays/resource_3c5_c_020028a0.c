typedef unsigned char u8;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;

/*
 * resource_3c5 owner at 0x020028a0, 1240 bytes: 0x020028a0-0x02002d77.  The
 * overlay's scene-entry router -- it reads the saved scene id, and depending on
 * its value either poses a fixed tableau, dispatches an eight-way sub-scene
 * through an embedded jump table, or restores the room to its walk-around
 * state.
 *
 * FRAME AND SIGNATURE.  `push {r5,r6,lr} / mov r6,r8 / push {r6} / sub sp,#8`;
 * the 8 bytes are the two outgoing stack words of the six-argument renderer.
 * The epilogue is `add sp,#8 / pop {r3} / mov r8,r3 / pop {r5,r6} / pop {r1} /
 * bx r1` -- `pop {rN} ; bx rN` with N != 0, so r0 survives and IS the result.
 * r0 is set to 0 at the single exit 0x02002d3c, so the owner returns s32 0 on
 * every path.  It takes no arguments (r0 is written before any read).
 *
 * POOL MAP, derived from the owner's own control flow rather than from any
 * referenced-words listing.  Four regions are data and are NOT decoded as
 * instructions, but register state is carried straight across all of them:
 *   0x02002902-0x02002913  alignment + 4 words, jumped over by the `b.n` at
 *                          0x020028da; r1/ip/r6 stay live across it.
 *   0x020029ec-0x020029f3  2 words, jumped over by the `b.n` at 0x020029ea;
 *                          r1/r3/r5/r6 stay live across it (the `strb r5,[r3]`
 *                          at 0x020029f4 completes a store begun at 0x020029e8).
 *   0x02002a9a-0x02002abb  alignment + the 8-entry jump table reached by the
 *                          `mov pc,r3` at 0x02002a98.
 *   0x02002d4a-0x02002d77  alignment + 12 trailing pool words, past the return.
 *
 * LINK BASE.  The jump table gives this overlay its own witness for the proven
 * 0x02008000 base: the table's base pool word is 0x0200aa9c and the table is
 * physically at file offset 0x2a9c.  Its eight entries are 0x0200aabc,
 * 0x0200aac4, 0x0200ad24, 0x0200ad24, 0x0200aafc, 0x0200ad24, 0x0200ab28,
 * 0x0200ab54 -- file offsets 0x2abc, 0x2ac4, 0x2d24, 0x2d24, 0x2afc, 0x2d24,
 * 0x2b28, 0x2b54, every one of them a case body inside this owner, and none of
 * them carrying the Thumb bit (correct: `mov pc,rN` does not interwork).
 *
 * CALL ACCOUNTING.  `bun tools/overlay_call_targets.ts resource_3c5 28a0`
 * reports sites=69 / distinct_targets=18 (60 veneer, 9 prologue).  The
 * inventory's `calls=56` is the usual lower bound and is not used.  Every one
 * of the 69 sites is placed below; the multiset of `Func_...(` occurrences in
 * this file, counting the four POSE expansions, is 69 with exactly the same
 * per-name multiplicities:
 *   Func_080000c0 x1   Func_08009180 x1   Func_080091c0 x5   Func_080770c0 x8
 *   Func_080770c8 x2   Func_080770d0 x1   Func_0808a010 x2   Func_0808a080 x10
 *   Func_0808a090 x2   Func_0808a0f0 x9   Func_0808a100 x6   Func_0808a158 x2
 *   Func_0808a1b8 x11  Func_020009f4 x3   Func_02000d4c x1   Func_02000eac x3
 *   Func_020027f4 x1   Func_02002820 x1
 * One subtlety the count depends on: the site at 0x02002af6 is reached from
 * TWO control paths (the scene-177 head at 0x020028f8 and jump-table case 1),
 * which the assembly expresses as a branch into the middle of the case body.
 * It is therefore ONE call site, spelled here as a single `goto place`
 * target -- writing it twice would put a phantom call in the multiset.
 *
 * GLOBALS.  Data_02000240 is the overlay's shared halfword scene block; the
 * byte-exact siblings assets/code/resource_3b7_c_02000044.c and
 * resource_3b2_c_02000d48.c already index it as `Data_02000240[224]`, which is
 * the same halfword this owner reads at byte 448.  Data_03001ebc is the
 * workspace pointer variable at 0x03001ebc (see
 * assets/code/resource_3c4_c_0200006c.c); this owner writes 516 into its
 * workspace at byte 448.
 *
 * Uncertainties, recorded rather than guessed:
 *  - The scene ids 174/175/176/177 and 176 are pooled constants.  The
 *    byte-exact sibling above models such comparands as link-time symbols
 *    (`(s32)&Value_000000bd`); that is a byte-exactness concern, so plain
 *    integers are used here.
 *  - The eight jump-table entries collapse to five distinct bodies: index 0
 *    falls through into index 1's body, and indices 2, 3 and 5 all jump
 *    directly to 0x02002d24, bypassing the `Data_02000240[225] == 3` test that
 *    guards the same code on the fall-through path.  That asymmetry is in the
 *    table, not a transcription slip.
 *  - The slot numbers 8..14 are actor/camera slot indices passed to
 *    Func_0808a080 and friends; the meaning of record fields +12, +20, +35,
 *    +89, +90, +94 and of presentation-block fields +9, +30, +38 is taken from
 *    the neighbouring exact sources and is not re-derived here.
 */

extern s16 Data_02000240[];     /* shared scene block; [224] is byte 448 */
extern u8 *Data_03001ebc;       /* workspace pointer variable */

/* Old-style declarations are mandatory in overlay sources: one import name can
 * be reached with different argument counts at different sites. */
void Func_080000c0();           /* frame wait */
void Func_08009180();           /* six-argument renderer */
void Func_080091c0();           /* six-argument renderer */
s32 Func_080770c0();            /* progress-flag test */
void Func_080770c8();           /* set progress flag */
void Func_080770d0();           /* clear progress flag */
void Func_0808a010();
u8 *Func_0808a080();            /* record by slot index */
void Func_0808a090();
void Func_0808a0f0();           /* place slot at (x, z) */
void Func_0808a100();
void Func_0808a158();
void Func_0808a1b8();           /* face slot along (heading, 0) */
void Func_020009f4();           /* four register arguments */
void Func_02000d4c();           /* the seventeen-step effect ring */
void Func_02000eac();
void Func_020027f4();
void Func_02002820();

/* Poses one of the four background actors: clear its two state bytes, set its
 * timer halfword, force presentation bits 2-3, clear the sub-block's byte 38
 * and set its halfword 30. */
#define POSE(slot, timer, blockValue)                                   \
    do {                                                                \
        u8 *rec = Func_0808a080(slot);                                  \
        u8 *block;                                                      \
        rec[89] = 0;                                                    \
        rec[35] = 0;                                                    \
        *(u16 *)(rec + 94) = (timer);                                   \
        block = *(u8 **)(rec + 80);                                     \
        block[9] = (u8)(block[9] | 12);                                 \
        block = *(u8 **)(rec + 80);                                     \
        block[38] = 0;                                                  \
        block = *(u8 **)(rec + 80);                                     \
        *(u16 *)(block + 30) = (blockValue);                            \
        Func_0808a100((slot), 0);                                       \
    } while (0)

s32 Func_020028a0(void)
{
    u8 *record;
    s32 scene;
    s32 index;
    s32 first, second;      /* r0/r1 of the shared renderer call */
    s32 stack5, stack6;     /* its two stack words, sp+0 and sp+4 */

    Func_080000c0(1);

    *(s32 *)(Data_03001ebc + 448) = 516;     /* 129 << 2 */

    scene = Data_02000240[224];
    if (scene == 177) {
        record = Func_0808a080(12);
        if ((*(s32 *)(record + 8) >> 20) != 20) {
            return 0;
        }
        if ((*(s32 *)(record + 16) >> 20) != 12) {
            return 0;
        }
        stack5 = 20;
        stack6 = 12;
        first = 38;
        second = 12;
        goto place;
    }

    Data_02000240[289] = 1;
    Data_02000240[288] = 176;

    if (scene == 176) {
        Func_0808a158(8, 6);
        Func_0808a158(9, 6);

        if (Data_02000240[225] == 5 && Func_080770c0(265) == 0) {
            Func_0808a0f0(9, 0x01380000, 0x01480000);   /* 156 << 17, 164 << 17 */
        }

        Func_02002820();

        record = Func_0808a080(9);
        *(s32 *)(record + 12) = *(s32 *)(Func_0808a080(9) + 20);

        if (Func_080770c0(768) != 0) {
            Func_0808a100(10, 4);
            Func_0808a080(10)[89] = 0xfe;
            Func_020027f4();
        }

        POSE(11, 0, 0xc000);        /* 192 << 8 */
        POSE(12, 30, 0x4000);       /* 128 << 7 */
        POSE(13, 60, 0x8000);       /* 128 << 8 */
        POSE(14, 90, 0x8000);       /* the same r8-held 0x8000 */
        return 0;
    }

    if (scene == 175) {
        index = Data_02000240[225] - 10;
        if ((u32)index > 7) {
            return 0;
        }

        if (index == 0) {
            /* Case 0 falls through into case 1's body. */
            Func_080770c8(2432);                /* 152 << 4 */
        }
        if (index <= 1) {
            if (Func_080770c0(2432) == 0) {
                return 0;
            }
            stack5 = 1;
            stack6 = 3;
            Func_08009180(120, 7, 109, 7, stack5, stack6);
            stack5 = 45;
            stack6 = 9;
            first = 45;
            second = 8;
            goto place;
        }
        if (index == 4) {
            Func_020009f4(0x01b80000, 0, 0x01220000, 223);  /* 220<<17, 145<<17 */
            stack5 = 27;
            stack6 = 13;
            Func_080091c0(22, 13, 1, 1, stack5, stack6);
            Func_02000eac(14);
            return 0;
        }
        if (index == 6) {
            Func_020009f4(0x01c00000, 0, 0x01220000, 223);  /* 224<<17, 145<<17 */
            stack5 = 28;
            stack6 = 10;
            Func_080091c0(22, 12, 1, 1, stack5, stack6);
            Func_02000eac(16);
            return 0;
        }
        if (index == 7) {
            Func_020009f4(0x00e80000, 0, 0x02520000, 223);  /* 232<<16, pool */
            stack5 = 14;
            stack6 = 33;
            Func_080091c0(22, 12, 1, 1, stack5, stack6);
            Func_02000eac(17);
            return 0;
        }
        /* Indices 2, 3 and 5 jump straight to the gate at 0x02002d24. */
        goto gate;
    }

    if (scene != 174) {
        Func_0808a100(12, 2);
        return 0;
    }

    record = Func_0808a080(8);
    record[90] = (u8)(record[90] & 0xfe);
    record = Func_0808a080(9);
    record[90] = (u8)(record[90] & 0xfe);

    Func_0808a090(8, 0x00010000, 0x00008000);   /* 128 << 9, 128 << 8 */
    Func_0808a090(9, 0x00010000, 0x00008000);

    if (Func_080770c0(265) == 0) {
        if (Data_02000240[225] == 1) {
            Func_080770c8(769);
        } else {
            Func_080770d0(769);
        }
    }

    if (Func_080770c0(2440) == 0) {
        Func_0808a0f0(10, 0xffc00000, 0xffc00000);
        Func_0808a0f0(11, 0x01180000, 0x01280000);  /* 140<<17, 148<<17 */
        Func_0808a0f0(12, 0x01380000, 0x00f80000);  /* 156<<17, 248<<16 */
        Func_0808a0f0(13, 0x01280000, 0x00f80000);
        Func_0808a0f0(14, 0x01400000, 0x01280000);  /* 160<<17, 148<<17 */
        Func_0808a1b8(11, 0, 0);
        Func_0808a1b8(12, 0xc000, 0);               /* 192 << 8 */
        Func_0808a1b8(13, 0xc000, 0);
        Func_0808a1b8(14, 0x8000, 0);               /* 128 << 8 */
        Func_0808a010(5);
    } else if (Func_080770c0(2441) != 0) {
        Func_0808a0f0(10, 0x01380000, 0x01380000);
        Func_0808a1b8(10, 0xb000, 0);               /* 176 << 8 */
        Func_0808a1b8(11, 0xb000, 0);
        Func_0808a1b8(12, 0xb000, 0);
        Func_0808a1b8(13, 0xb000, 0);
        Func_0808a1b8(14, 0xb000, 0);
        Func_0808a010(5);
    }

    if (Func_080770c0(2437) != 0) {
        Func_0808a0f0(8, 0x01180000, 0x00f00000);   /* 140<<17, 240<<16 */
        Func_0808a0f0(9, 0x01480000, 0x00f00000);   /* 164<<17 */
        Func_0808a1b8(8, 0x8000, 0);
        Func_0808a1b8(9, 0, 0);
        stack5 = 17;
        stack6 = 14;
        Func_080091c0(81, 14, 4, 1, stack5, stack6);
    }

    if (Data_02000240[225] != 3) {
        return 0;
    }

gate:
    if (Func_080770c0(265) == 0) {
        Func_02000d4c();
    }
    return 0;

place:
    Func_080091c0(first, second, 1, 1, stack5, stack6);
    return 0;
}
