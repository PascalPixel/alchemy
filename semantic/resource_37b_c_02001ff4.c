#include "types.h"

/*
 * Resource 37b scene driver at 0x02001ff4.
 *
 * Complete owner: `push {r5, r6, r7, lr}` / `push {r8}` / `sub sp, #8` at
 * 0x02001ff4 through the matching interworking return at 0x0200221a..0x02002224
 * (`add sp, #8` / `pop {r3}` / `mov r8, r3` / `pop {r5, r6, r7}` /
 * `pop {r0} / bx r0`).  The popped register is r0, so it holds the return
 * address and the owner is `void`.  The eight words at 0x02002228-0x02002243
 * are this owner's literal pool (a two-byte alignment `nop` at 0x02002226
 * precedes them); the next prologue is at 0x02002244.  The pool is loaded by
 * the `ldr rN, [pc, #imm]` sites at 0x02002004, 0x02002038, 0x02002098,
 * 0x020020c2, 0x020021b6, 0x020021c0, 0x020021ca, 0x020021d4, 0x020021de,
 * 0x020021e8 and 0x020021f4, and is data, not code.
 *
 * Link base 0x02008000: the pool word 0x0200a980 at 0x0200222c is the
 * in-image table at file offset 0x2980 that this owner walks (the same
 * relation as the 0x02008eb1 = Func_02000eb0 + Thumb witness in
 * semantic/overlays/resource_37b_c_020014b8.c).  The overlay image is
 * writable EWRAM, so the table is not declared `const`.
 *
 * Call sites resolved with `tools/overlay_call_targets.ts` (an overlay `bl`
 * stores `target_offset - 2`; the disassembler's own annotations are wrong).
 * 42 sites, 22 distinct targets: 40 import veneers and 2 real in-overlay
 * prologues, 0x02000054 (byte-exact in assets/code) at 0x020020b4 and
 * 0x02000c8c (converted alongside this file) at 0x020021ee.  The inventory's
 * calls=41 is a lower bound and is not used as the proof; the multiset of the
 * tool's per-site targets and of the span's `bl` halfword pairs agree at
 * 42 = 42, and every site appears exactly once below.
 *
 * The import roles used here, each fixed by its argument shape and by the
 * sibling owners in this overlay:
 *   Func_080770c0(id)   - test a story flag; used only in conditions
 *   Func_080770c8(id)   - set that flag        (0x080f is set at 0x020021ea)
 *   Func_080770d0(id)   - clear it
 *   Func_0808a080(n)    - scene-record accessor (see 0x02001624, 0x02000c8c)
 *   Func_08009180(...)  - the six-argument renderer ABI, four registers plus
 *                         [sp, #0] and [sp, #4], set explicitly at all 7 sites
 *   Func_0808a010(n)    - frame wait
 *   Func_080f9010(n)    - the paced step used before each renderer call
 *
 * Uncertainties recorded as found:
 *  - Func_0808a018 (0x02002000), Func_0808a218 (0x0200202e) and Func_0808a020
 *    (0x02002216) are reached with no argument register set by this owner;
 *    they are spelled with no arguments.
 *  - The 20-iteration loop at 0x02002052 re-reads the SAME two table records
 *    every pass — no index register advances — so it is a repeat, not a walk.
 *    That is transcribed literally rather than "corrected" into a stride.
 *  - Func_02000054 returns 1 or -1 in its byte-exact source, so the `== 0`
 *    arm below is unreachable through it today; the assembly tests it and it
 *    is preserved.
 *  - Bit 0 of the byte at +90 of the scene record is cleared twice and set
 *    once; its meaning is not established.
 */

/* Imports.  Old-style declarations: arity varies per site in this overlay. */





















/* In-overlay callees. */



/* In-image overlay data at file offset 0x2980 (writable EWRAM). */
extern s32 Data_0200a980[];

extern void Func_02004456();
extern s32 Func_0200443c();
extern void Func_0200452e();
extern void Func_02004548();
extern void Func_02004554();
extern void Func_0200459a();
extern void Func_02004450();
extern void Func_020045ba();
extern void Func_0200446e();
extern void Func_020044bc();
extern void Func_020045da();
extern void Func_02004490();
extern void Func_020044de();
extern void Func_020044b0();
extern void Func_020044ee();
extern s32 Func_0200210a(void);
extern s32 Func_020044fa();
extern void Func_020045e0();
extern u8 * Func_02004546();
extern void Func_02004560();
extern u8 * Func_0200455e();
extern void Func_020045c4();
extern void Func_0200458e();
extern void Func_02004530();
extern void Func_0200457c();
extern u8 * Func_020045ea();
extern void Func_0200461a();
extern u8 * Func_02004608();
extern void Func_020045f0();
extern s32 Func_020045ee();
extern s32 Func_020045f8();
extern s32 Func_02004602();
extern s32 Func_0200460c();
extern s32 Func_02004616();
extern void Func_02004628();
extern void Func_02002e7c(void);
extern s32 Func_0200462c();
extern void Func_0200472e();
extern void Func_0200475c();
extern void Func_02004768();
extern void Func_02004674();
void Func_02001ff4(void)
{
    s32 *table;
    u8 *record;
    s32 outcome;
    u32 i;

    outcome = 0;

    Func_02004456();
    if (Func_0200443c(0x80f) != 0) {
        goto finish;
    }

    Func_0200452e(0x20000, 0x4000);
    Func_02004548(0x2400000, -1, 0xac0000, 1);
    Func_02004554();

    table = Data_0200a980;

    Func_0200459a(186);
    Func_02004450(table[0], table[1], table[2], table[3], table[4], table[5]);

    for (i = 0; i < 20; i++) {
        Func_020045ba(246);
        Func_0200446e(table[6], table[7], table[8],
                      table[9], table[10], table[11]);
        Func_020044bc(4);
        Func_020045da(246);
        Func_02004490(table[12], table[13], table[14],
                      table[15], table[16], table[17]);
        Func_020044de(4);
    }

    Func_020044b0(table[18], table[19], table[20],
                  table[21], table[22], table[23]);
    Func_020044ee(table[24]);

    outcome = Func_0200210a();

    if (outcome == -1) {
        if (Func_020044fa(0x818) != 0) {
            goto finish;
        }

        Func_020045e0(0, 1);

        record = Func_02004546(0);
        *(u16 *)(record + 6) = (u16)table[25];

        Func_02004560(0, 0x20000, 0x20000);

        record = Func_0200455e(0);
        record[90] &= 0xfe;

        Func_020045c4(0, 4, 0);
        Func_0200458e(0, table[26], table[27]);

        Func_02004530(table[28], table[29], table[30],
                      table[31], table[32], table[33]);
        Func_02004554(table[34], table[35], table[36],
                      table[37], table[38], table[39]);
        Func_0200457c(table[40], table[41], table[42],
                      table[43], table[44], table[45]);

        record = Func_020045ea(table[46]);
        record[90] &= 0xfe;
        Func_0200461a(table[46], table[47], table[48]);

        record = Func_02004608(0);
        record[90] |= 1;

        goto finish;
        Func_020045f0(table[24]);
    }

    if (outcome != 0) {
        goto finish;
    }

    if (Func_020045ee(0x818) != 0 && Func_020045f8(0x80b) != 0 &&
        Func_02004602(0x80d) != 0 && Func_0200460c(0x80e) != 0) {
        if (Func_02004616(0x80f) != 0) {
            goto finish;
        }
        Func_02004628(0x80f);
        Func_02002e7c();
        goto finish;
    }

    if (Func_0200462c(0x812) != 0) {
        Func_0200472e(5);
        outcome = 1;
    }

finish:
    if (outcome == 1) {
        Func_0200475c();
        Func_02004768();
    }
    Func_02004674();
}
