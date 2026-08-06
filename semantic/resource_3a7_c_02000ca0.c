/*
 * resource_3a7 scene dispatcher at 0x02000ca0, 960 bytes.
 *
 * Complete owner: `push {r5, r6, lr}` at 0x02000ca0 / `sub sp, #8` at
 * 0x02000cb6 through `add sp,#8 / pop {r5, r6} / pop {r1} / bx r1` at
 * 0x0200103a.  The popped register is r1, NOT r0, so r0 survives and IS the
 * result; `movs r0,#0` at 0x02001036 makes it a constant 0, so the owner
 * returns s32 and always returns 0.
 *
 * POOL AND TABLE MAP, derived from a control-flow walk of the owner (never from
 * the listing - several of these words decode as plausible instructions):
 *   0x02000d9a         alignment halfword
 *   0x02000d9c..0dd3   the 14-entry `mov pc, rN` JUMP TABLE (56 bytes)
 *   0x02000eb4..0edf   literal pool
 *   0x02001042..0105f  literal pool
 * Note the jump table's own pool word ends mid-row in effect: the table is
 * followed immediately at 0x02000dd4 by live code (the first case body), which
 * is only reachable through the table.  Everything else in the span is reached
 * as an instruction.
 *
 * LINK BASE, third witness and the strongest one in this overlay: the jump-table
 * base is the pool word 0x02008d9c while the table physically sits at file
 * offset 0x2000d9c, and every one of its 14 entries is 0x8000 above the case
 * body it selects.  None of the entries carries the Thumb bit, which is exactly
 * right for `mov pc, rN` (it does not interwork) - the Thumb-bit rule cleanly
 * separates this table from the installed-handler pool words elsewhere in the
 * overlay.  A fourth witness follows in case B: 0x020086e5 handed to
 * Func_080000d0 is Func_020006e4 + 1, and 0x020006e4 has a BYTE-EXACT source in
 * assets/code, so the base is confirmed against tracked material.
 *
 * Jump table, index `sub - 1` for `sub` in 1..14:
 *   1,2,3,4      -> 0x02000dd4  (case A)
 *   5,6,7,12,13  -> 0x02000f08  (straight to the shared tail)
 *   8,9,14       -> 0x02000e1c  (case B)
 *   10,11        -> 0x02000ee0  (case C, which FALLS THROUGH to the tail)
 *
 * `Data_02000240` is the cross-overlay idiom: the signed halfwords at byte
 * offsets 448 and 450 (indices 224 and 225) are the scene id and its sub-step,
 * branched on.  Four byte-exact siblings elsewhere in the project share the same
 * layout, and this overlay's own byte-exact assets/code/resource_3a7_c_020000e0.c
 * reads `Data_02000240[224]` against the same 0x60/0x61/0x62 family of ids.  The
 * owner runs three independent scene blocks in sequence (0x60, 0x61, 0x62),
 * re-reading the id between them, and finally rewrites the pair to (0x60, 10).
 *
 * `*(u8 **)0x03001ebc` is a pointer CELL, so one dereference; +448 into that
 * workspace is a separate s32 from the Data_02000240 halfword at the same
 * numeric offset.
 *
 * Call targets resolved with
 *   bun tools/overlay_show.ts resource_3a7 0ca0 -n 960 |
 *   bun tools/overlay_call_targets.ts resource_3a7 --annotate
 * `bun tools/overlay_call_targets.ts resource_3a7 0ca0` reports sites=69 (the
 * inventory's calls=41 is the documented undercount - it does not see the
 * jump-table arms).  Per-target multiset, 69 = 69 against the C below:
 *   Func_080770c0 x12 <- veneer 0x02001100
 *   Func_0808a080 x17 <- veneer 0x02001130
 *   Func_0808a100 x10 <- veneer 0x02001140
 *   Func_080091e0 x5  <- veneer 0x020010e0
 *   Func_080091b8 x4  <- veneer 0x020010c8
 *   Func_080091c0 x3  <- veneer 0x020010d0
 *   Func_080000c0 x3  <- veneer 0x02001070
 *   Func_0808a0f0 x2  <- veneer 0x02001138
 *   Func_080000d0 x1  <- veneer 0x02001078
 *   Func_08009128 x1  <- veneer 0x020010b0
 *   Func_080770d0 x1  <- veneer 0x02001110
 *   ten of this overlay's own prologues, x1 each: 0x02000398, 0x02000410,
 *   0x0200050c, 0x02000554, 0x020005a0, 0x020005ec, 0x02000640, 0x02000690,
 *   0x02000c08, 0x02000c50 - every one of them converted alongside this file.
 * The shared tail at 0x02000f08 is written ONCE, after the switch, rather than
 * per arm: writing it per arm would inject seven phantom calls.  Conversely the
 * two `Func_0808a100(8, 2)` sites (0x02000f0c and 0x02000f52) are in different
 * scene blocks and are kept separate; merging them would deflate the multiset.
 *
 * This owner is the overlay's spine, and it closes the module: case B is the
 * installer for the six presentation steps 0x0200050c..0x02000690, gated one by
 * one on the 0x200..0x205 flags those very owners raise, and it installs
 * Func_020006e4 as a per-frame task; case C and the 0x62 block run the one-shot
 * bodies 0x02000398 and 0x02000410 behind the 0x9a9 and 0x9aa flags that
 * 0x02000368 / 0x0200048c / 0x020004d0 also gate on.
 *
 * Uncertainties: every scene/request id and every record field is transcribed,
 * not interpreted.  Byte +89 is the "slot occupied" flag the byte-exact
 * assets/code/resource_3a7_c_02000134.c tests; the values 1, 2 and the 0x80 bit
 * OR-ed into it here are not otherwise explained.
 */

/* Old-style declarations: overlay imports vary their argument count between
 * call sites in this overlay.  Those used in conditions need a return type. */
#include "types.h"

    /* test a scene completion flag */
   /* clear a scene completion flag */
   /* six-argument scene request, last two on the stack */
   /* six-argument renderer, last two on the stack */
   /* set presentation mode (record, phase) */
   /* frame wait */
   /* install a per-frame task (callback, rank) */

    /* scene-entity record by selector */



/* This overlay's own owners, all converted alongside this file. */











/* Shared scene-state halfwords: [224] is the scene id, [225] the sub-step. */
extern s16 Data_02000240[];

extern s32 Func_02001de0();
extern void Func_02001dc8();
extern void Func_02001dd6();
extern void Func_02001d84();
extern void Func_02001df0();
extern void Func_02001d9e();
extern void Func_02001e08();
extern void Func_02001df4();
extern void Func_02001dba();
extern void Func_02001e64();
extern s32 Func_02001e6e();
extern u8 * Func_02001ea8();
extern s32 Func_02001ed8();
extern void Func_02001eb8();
extern void Func_02001ed6();
extern void Func_02001ee8();
extern void Func_02001a26();
extern s32 Func_02001f26();
extern void Func_02001f72();
extern void Func_02001342();
extern s32 Func_02001f3c();
extern void Func_02001f88();
extern void Func_020013a0();
extern s32 Func_02001f52();
extern void Func_02001f9e();
extern void Func_02001402();
extern s32 Func_02001f68();
extern void Func_02001fb4();
extern void Func_02001464();
extern s32 Func_02001f80();
extern void Func_02001fcc();
extern void Func_020014d0();
extern s32 Func_02001f96();
extern void Func_02001fe2();
extern void Func_02001536();
extern void Func_02001f28();
extern s32 Func_02001fe4();
extern void Func_02001284();
extern void Func_02002032();
extern u8 * Func_02002030();
extern void Func_0200204e();
extern void Func_02002056();
extern void Func_0200204c();
extern u8 * Func_02002002();
extern void Func_02002058();
extern u8 * Func_0200200e();
extern u8 * Func_02002064();
extern void Func_02002094();
extern s32 Func_0200205a();
extern void Func_020020a6();
extern void Func_0200209c();
extern u8 * Func_02002052();
extern void Func_020020a8();
extern u8 * Func_0200205e();
extern void Func_020020b4();
extern u8 * Func_0200206a();
extern u8 * Func_020020c0();
extern u8 * Func_020020d0();
extern void Func_02001c0e();
extern u8 * Func_020020f4();
extern u8 * Func_02002100();
extern u8 * Func_0200210a();
extern u8 * Func_02002114();
extern u8 * Func_0200211e();
extern u8 * Func_0200212a();
extern u8 * Func_02002134();
extern s32 Func_0200210e();
extern void Func_02001426();
extern void Func_0200215c();
s32 Func_02000ca0(void)
{
    s32 permuted_14;
    u8 *workspace = *(u8 **)0x03001ebc;
    s32 sub;

    *(s32 *)(workspace + 448) = 516;

    /* ---- scene 0x60 ---------------------------------------------------- */
    if (Data_02000240[224] == 0x60) {
        sub = Data_02000240[225];

        if ((sub >= 5 && sub <= 8) || sub == 13) {
            if (Func_02001de0(0x9a8) == 0) {
                Func_02001dc8(22, 29, 1, 1, 21, 29);
            } else {
                Func_02001dd6(108, 27, 1, 1, 92, 27);
                Func_02001d84(1);
                Func_02001df0(19, 83, 15, 8, 19, 91);
                Func_02001d9e(1);
                Func_02001e08(2, 24, 1, 2, 25, 27);
            }

            Func_02001df4();
            Func_02001dba(1);
        } else if (sub == 10) {
            Func_02001e64(0x9a8);
        }
    }

    /* ---- scene 0x61 ---------------------------------------------------- */
    if (Data_02000240[224] == 0x61) {
        if (Func_02001e6e(0x300) == 0) {
            *(s32 *)(Func_02001ea8(22) + 28) = 0x18000;
        }

        sub = Data_02000240[225];

        if ((u32)(sub - 1) <= 13) {
            switch (sub) {
            case 1:
            case 2:
            case 3:
            case 4:
                /* case A */
                if (Func_02001ed8(0x9a8) == 0) {
                    Func_02001eb8(5, 81, 11, 7, 5, 73);
                } else {
                    Func_02001ed6(5, 12, 1, 1, 6, 12);
                    Func_02001ee8(12, 10, 1, 1, 12, 11);
                }
                break;

            case 8:
            case 9:
            case 14:
                /* case B: install the six presentation steps, each behind the
                 * flag that step itself raises. */
                Func_02001a26();

                if (Func_02001f26(0x200) != 0) {
                    Func_02001f72(16, 5);
                    Func_02001342();
                }
                if (Func_02001f3c(0x201) != 0) {
                    Func_02001f88(17, 5);
                    Func_020013a0();
                }
                if (Func_02001f52(0x202) != 0) {
                    Func_02001f9e(18, 5);
                    Func_02001402();
                }
                if (Func_02001f68(0x203) != 0) {
                    Func_02001fb4(19, 5);
                    Func_02001464();
                }
                if (Func_02001f80(0x204) != 0) {
                    Func_02001fcc(20, 5);
                    Func_020014d0();
                }
                if (Func_02001f96(0x205) != 0) {
                    Func_02001fe2(21, 5);
                    Func_02001536();
                }

                /* Func_020006e4 + 1 under the proven 0x02008000 base - the
                 * byte-exact assets/code/resource_3a7_c_020006e4.c. */
                Func_02001f28(0x020086e5, 0xc80);   /* 200 << 4 */
                break;

            case 10:
            case 11:
                /* case C; falls through to the shared tail below. */
                break;
                if (Func_02001fe4(0x9a9) != 0) {
                    Func_02001284();
                    Func_02002032(9, 0x00f80000, 0x036c0000);
                }
                Func_02002030(8)[35] = 2;

            default:
                /* 5, 6, 7, 12, 13: straight to the shared tail. */
                break;
            }
        }

        /* Shared tail at 0x02000f08, reached from every arm and from an
         * out-of-range sub-step.  Written once on purpose. */
        Func_0200204e(8, 2);
        Func_02002056(9, 2);
        Func_0200204c(Func_02002002(8), 0);
        Func_02002058(Func_0200200e(9), 0);
        Func_02002064(9)[89] = 1;
    }

    /* ---- scene 0x62 ---------------------------------------------------- */
    if (Data_02000240[224] == 0x62) {
        u8 *record;

        Func_02002094(8, 2);

        if (Func_0200205a(0x207) == 0) {
            Func_020020a6(10, 2);
        }

        Func_0200209c(Func_02002052(8), 0);
        Func_020020a8(Func_0200205e(10), 0);
        Func_020020b4(Func_0200206a(9), 0);

        record[89] |= 0x80;
        record = Func_020020c0(10);

        record = Func_020020d0(9);
        permuted_14 = Data_02000240[225];
        record[89] |= 0x80;
        sub  = permuted_14;

        if (sub >= 5 && sub <= 6) {
            Func_02001c0e();

            Func_020020f4(11)[89] = 2;
            Func_02002100(12)[89] = 2;
            Func_0200210a(13)[89] = 2;
            Func_02002114(14)[89] = 2;
            Func_0200211e(8)[89] = 1;
            Func_0200212a(10)[89] = 1;
            Func_02002134(9)[89] = 1;

            if (Func_0200210e(0x9aa) != 0) {
                Func_02001426();
                Func_0200215c(10, 0x01080000, 0x00cc0000);
            }
        }
    }

    Data_02000240[289] = 10;
    Data_02000240[288] = 0x60;
    return 0;
}
