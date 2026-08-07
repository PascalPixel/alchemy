/*
 * BYTE-EXACT and adopted 2026-08-07 with -fthumb-arg-before-shift-in-sheet:
 * the last residual was the pair at 0x02005ce0, where the reference writes
 * `movs r0, #8' ahead of the `lsls r1, r1, #8' that finishes the split
 * constant 0xd000 -- the same rule as -fthumb-arg-before-final-shift, except
 * the shift is not the sheet's last setup insn (`movs r2, #80' still stands
 * between the pair and the call).
 */

/*
 * Resource 3b1 unindexed helper at 0x02005ca4 (108 bytes, 1 call).
 *
 * Derived span: no inventory row (item 28's unindexed population). `push
 * {lr}` at 0x02005ca4, epilogue `pop {r0} / bx r0` at
 * 0x02005d0c-0x02005d0e; no pool, immediately followed by the next
 * owner's push {r5,r6,r7,lr} at 0x02005d10, already this overlay's row
 * `0x02005d10 | 1 call`, so the span is 0x02005ca4-0x02005d10,
 * 108 bytes.
 *
 * A flat setter sequence, no branches. Per-site call veneers (raw asm
 * confirms each callee slot uses its own local stub, distinct from the
 * generic main-image symbol name).
 *
 * STILL-OPEN (4 differing bytes of 108): the whole residual is one
 * instruction-order swap inside the `Func_0200c2b8(8, 0xd000, 0x50)` setup
 * at offset 0x3c. The reference interleaves the r0 load between the two
 * halves of the r1 constant -- `movs r1,#208 / movs r0,#8 / lsls r1,r1,#8`
 * -- where this compiles the shift first: `movs r1,#208 / lsls r1,r1,#8 /
 * movs r0,#8`. Both are the same three instructions; only the schedule
 * differs, and it sits inside ONE call expression, so statement reordering
 * cannot reach it (alchemist has no statement boundary to move).
 *
 * Tried and ruled out, both byte-for-byte unchanged: prototyping the callee
 * as `void Func_0200c2b8(int, int, int)` instead of the old-style empty
 * declaration, and spelling the constant as `(208 << 8)` rather than
 * `0xd000` (it folds before codegen either way). The neighbouring
 * `Func_0200a554(9, 0x1d4, 0x268, 0x8000)` call, which needs three
 * shifted constants, already matches exactly -- so the scheduler only
 * diverges when a single shifted argument competes with a plain one.
 *
 * overlay_mode_cohort.ts cannot be pointed at this owner: it is part of the
 * unindexed population and has no inventory row, so a compiler-mode search
 * would need a row added first.
 */

void Func_0200c170(void);
void Func_0200a59a();
void Func_0200a554();
void Func_0200a5b6();
void Func_0200c256();
void Func_0200c19c();
void Func_0200c2b8();
void Func_0200c2c2();
void Func_0200c25a();
void Func_0200c1c0();
void Func_0200a5f2();

void Func_02005ca4(void)
{
    Func_0200c170();
    Func_0200a59a(15, 1, 0);
    Func_0200a554(9, 0x1d4, 0x268, 0x8000);
    Func_0200a5b6(8, 1, 20);
    Func_0200c256(9, 2);
    Func_0200c19c(20);
    Func_0200c2b8(8, 0xd000, 0x50);
    Func_0200c2c2(8, 0, 20);
    Func_0200c25a(8, 3);
    Func_0200c1c0(20);
    Func_0200a5f2(9, 21, 0);
}
