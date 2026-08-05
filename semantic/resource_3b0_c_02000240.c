#include "types.h"

/*
 * resource_3b0 owner at 0x02000240, 452 bytes: the overlay's ROOT — publish the
 * scene id, run the one-shot arrival bookkeeping, then dispatch on the scene
 * mode to one of six sub-scenes.
 *
 * It is the root by construction: 0x02000240 is entry 0 of the offset-0
 * exported-entry veneer table (`ldr r4,[pc,#0] / bx r4 / .word 0x02008241`,
 * spelled literally as `0x47204c00 / 0x02008241` in
 * `assets/code/resource_3b0_overlay.s`), and 0x02008241 - 0x8000 - 1 = 0x0240.
 *
 * Link base 0x02008000, proved without disassembly: the table's other odd words
 * include 0x02008031 and 0x020080a5, which are `Func_02000030 + 1` and
 * `Func_020000a4 + 1` — both already tracked byte-exact as
 * `assets/code/resource_3b0_c_02000030.c` and `..._020000a4.c` — and the tracked
 * 0x02000030 source already types 0x02009930 and 0x02009938 as in-image data
 * arrays at their runtime addresses.  The same spelling is used here.
 *
 * POOL AND TABLE MAP, from a control-flow walk of the owner, not a linear read.
 * The row's 452 bytes are NOT 452 bytes of code — the inventory reports only
 * 168 because its linear walk stops at the jump table:
 *   0x02000240..0x020002df   code (head, 160 bytes)
 *   0x020002e0..0x02000317   14-entry `mov pc,r3` JUMP TABLE (56 bytes)
 *   0x02000318..0x020003d7   code (the arms and the epilogue, 192 bytes)
 *   0x020003d8..0x02000403   literal pool (44 bytes)
 * The table base 0x020082e0 is the pool word the dispatch loads, its entry
 * count is the `cmp r3,#13 / bhi` bound, and its entries are runtime addresses
 * under the same base: 0x02008318 for index 0, 0x020083d0 (the default arm, and
 * also the `bhi` target) for indices 1..8, then 0x02008368, 0x0200837e,
 * 0x02008398, 0x020083b2 and 0x020083cc.  Seeding the table before believing
 * the gap is what HANDOVER requires; without it the walk reports the 56 bytes
 * and everything after them as unreachable.
 *
 * Prologue `push {r5, lr}` at 0x02000240; single epilogue `movs r0,#0 /
 * pop {r5} / pop {r1} / bx r1` at 0x020003d0.  The popped register is r1, not
 * r0, so **r0 survives and IS the result** (HANDOVER's interworking-epilogue
 * rule) — the owner returns s32, and every path returns 0.
 *
 * All 28 call sites resolved with `bun tools/overlay_call_targets.ts
 * resource_3b0 0240` (an overlay `bl` stores target offset - 2).  Per-target:
 *   Func_080770c0 8   Func_080770c8 1   Func_080000f8 2   Func_080000d0 1
 *   Func_080000c0 2   Func_0808a0f0 1   Func_0808a080 1   Func_0808a018 1
 *   Func_0808a2b0 1   Func_0808a210 1   Func_0808a200 1   Func_08009128 1
 *   Func_0808a020 1   and one call each to the in-overlay prologues 0x040c,
 *   0x0468, 0x0564, 0x07b0, 0x0af8 and 0x0e78.
 * The tool's naive overlapping scan reports the same 28, which corroborates the
 * walk — no pool word or table entry here decodes as a BL pair.  0x02000468 is
 * already tracked byte-exact as `assets/code/resource_3b0_c_02000468.c`.
 *
 * `Func_080000d0(0x020090a1, 3200)` is the task install, and 0x020090a1 is
 * file offset 0x10a0 plus the Thumb bit — an in-image per-frame task, which is
 * a further link-base witness.
 *
 * Uncertainties: 0x0927, 0x0928, 0x093e, 0x08a0, 0x0925 and 0x0109 are read as
 * event-flag ids from their argument position on Func_080770c0, and 0x0144 (162
 * << 1) as the flag Func_080770c8 SETS on entry — the gate-and-setter pairing
 * HANDOVER documents, except that here the flag set is not one of the flags
 * tested, so the one-shot partner lives in another owner.  The +448 slot off
 * the 0x03001ebc workspace pointer is the documented s32 scene/phase id and is
 * published here as 0x209.  The three sub-scene arms that write 0x6f and 30 to
 * `Data_02000240 + 452 / + 454` set the same pair of fields before three
 * different sub-scenes; the fields' meaning is not established here.
 */

/* Old-style declarations: overlay imports vary in arity between call sites. */
s32 Func_080770c0();            /* read an event flag */
void Func_080770c8();           /* set an event flag */
s32 Func_080000f8();            /* scheduler/frame query, 16-bit result */
void Func_080000d0();           /* install a per-frame task */
void Func_080000c0();           /* advance the task scheduler */
void Func_0808a0f0();
u8 *Func_0808a080();            /* scene entity by selector */
void Func_0808a018();
void Func_0808a020();
void Func_0808a200();
void Func_0808a210();
void Func_0808a2b0();
void Func_08009128();

/* In-overlay sub-scenes. */
void Func_0200040c();
void Func_02000468();
void Func_02000564();
void Func_020007b0();
void Func_02000af8();
void Func_02000e78();

/* RAM: the shared work area. */
extern u8 Data_02000240[];

/* In-image s32 slots. */
extern s32 Data_02009928[];
extern s32 Data_02009940[];

s32 Func_02000240(void)
{
    u8 *work = Data_02000240;
    s32 mode;

    Func_080770c8(324);                             /* 162 << 1 */

    *(s32 *)(*(u8 **)0x03001ebc + 448) = 0x209;

    if (Func_080770c0(0x927) != 0 || Func_080770c0(0x928) != 0) {
        if (Func_080770c0(0x93e) == 0 && Func_080770c0(0x8a0) == 0) {
            Data_02009940[0] = Func_080000f8() & 0xffff;
            Data_02009928[0] = Func_080000f8() & 0xffff;
            /* 0x020090a1 == the in-image task at 0x10a0, plus the Thumb bit. */
            Func_080000d0(0x020090a1, 3200);        /* 200 << 4 */
        }
    }

    if (Func_080770c0(0x925) != 0 && Func_080770c0(0x93e) == 0) {
        Func_0808a0f0(8, 0x00a40000, 0x01480000);   /* 164 << 16, 164 << 17 */
    }

    mode = *(s16 *)(work + 450) - 1;
    if ((u32)mode > 13) {
        return 0;
    }

    switch (mode) {
    case 0: {
        u8 *entity;

        if (Func_080770c0(0x109) != 0) {
            return 0;
        }

        entity = Func_0808a080(0);
        Func_0808a018();
        Func_0808a2b0();

        *(s32 *)(entity + 12) = 0x00380000;         /* 224 << 14 */

        Func_0808a210(-1, -1, -1, 0);
        Func_080000c0(1);
        Func_0808a200(0, 0);
        Func_08009128();
        Func_080000c0(1);
        Func_0808a020();
        break;
    }

    case 9:
        if (Func_080770c0(0x928) != 0) {
            Func_02000468();
        } else {
            Func_0200040c();
        }
        break;

        *(u16 *)(work + 452) = 0x6f;
        *(u16 *)(work + 454) = 30;
        Func_02000564();
    case 10:
        break;

    case 11:
        *(u16 *)(work + 452) = 0x6f;
        *(u16 *)(work + 454) = 30;
        Func_020007b0();
        break;

    case 12:
        *(u16 *)(work + 452) = 0x6f;
        *(u16 *)(work + 454) = 30;
        Func_02000af8();
        break;

    case 13:
        Func_02000e78();
        break;

    default:
        /* Indices 1..8 share the jump table's default entry. */
        break;
    }

    return 0;
}
