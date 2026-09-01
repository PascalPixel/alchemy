#include "types.h"
#include "global_cells.h"

/*
 * Draft reconstruction, not yet verified byte-exact against
 * games/gs1/asm/080b56e0.s. Traced directly from the retained assembly;
 * see games/gs1/recon/en/dossiers.json#main:080b56e0 for the evidence log.
 *
 * Structurally: an outer loop polls ADDR_03001AE8 (held-key level bits)
 * for bit 0x80 (Down) each pass. While Down is not held it calls two
 * flag-style calls and loops (Func_080b63c8 with a fixed 0x101). Once
 * Down is held it never returns to that outer poll again -- the
 * function has no epilogue in the retained assembly (push with no
 * matching pop anywhere), so both branches are provably infinite from
 * GCC's point of view.
 *
 * Once armed, an inner loop reads ADDR_03001B04 (newly-pressed trigger
 * bits) every pass, exactly mirroring the repeated-volatile-read idiom
 * already adopted in games/gs1/src/shop/select_repair_item.c and
 * games/gs1/src/shop/select_use_item.c (a fresh dereference per `if`,
 * not a cached local): Right/Left adjust val1 by +-1, Up/Down adjust it
 * by +-10, R/L adjust val2 by +-1, and A breaks out. While waiting for
 * A, Start calls Func_080b5534 and Select calls the already-adopted
 * BattleRuntime_ReservedNoOp2A08 (games/gs1/src/battle/runtime/
 * reserved_no_op_a.c); B (or the sticky `held` flag it sets) writes 5 to
 * *(u8*)0x0200046b every pass once triggered once. When val2 changes,
 * Func_08077098 (a plain far-call veneer, see games/gs1/asm/
 * 08077098.s) and Func_080b5368(val2) run once.
 *
 * The unusual `(u16 *)(0x0200046b - 85)` pointer is deliberate: the
 * reference computes it at runtime from the same r9=0x0200046b constant
 * (movs #85; negs; add) rather than loading a second literal-pool word,
 * mirroring games/gs1/src/game_flags/set.c's plain `(u8 *)0x02000040`
 * address-cast idiom for the base and letting the compiler synthesize
 * the second constant.
 */

extern void Func_08077098(void);
extern void Func_0800479c(void);
extern void Func_08004760(void);
extern void Func_08004858(void);
extern void Func_080770c8(s32);
extern void Func_080770d0(s32);
extern void Func_080b5534(void);
extern void Func_080b5368(s32);
extern void Func_080b63c8(s32);

void WaitFrames(s32);
void Resource_InitializeTable(void);
void Scheduler_ResetTaskTable(void);
void BattleRuntime_ReservedNoOp2A08(void);
void BattleUnit_Recalculate(s32);

void Func_080b56e0(void)
{
    s32 held;
    s32 val1;
    s32 val2;
    s32 prev2;
    u8 *bytePtr;
    u16 *halfPtr;

    held = 0;
    Func_08077098();

    for (;;) {
        Func_0800479c();
        Func_08004760();
        Scheduler_ResetTaskTable();
        Func_08004858();
        Resource_InitializeTable();
        Func_080770c8(362);
        val1 = 257;

        if ((*(volatile u32 *)ADDR_03001AE8 & 0x80) == 0) {
            Func_080770c8(354);
            Func_080b63c8(257);
            continue;
        }

        prev2 = -1;
        Func_080770d0(362);
        bytePtr = (u8 *)0x0200046b;
        halfPtr = (u16 *)(bytePtr - 85);
        val2 = 0;

        for (;;) {
            Func_080770d0(32);
            WaitFrames(1);

            for (;;) {
                if ((*(volatile u32 *)ADDR_03001B04 & 0x10) != 0)
                    val1 += 1;
                if ((*(volatile u32 *)ADDR_03001B04 & 0x20) != 0)
                    val1 -= 1;
                if ((*(volatile u32 *)ADDR_03001B04 & 0x40) != 0)
                    val1 -= 10;
                if ((*(volatile u32 *)ADDR_03001B04 & 0x80) != 0)
                    val1 += 10;
                if ((*(volatile u32 *)ADDR_03001B04 & 0x100) != 0)
                    val2 += 1;
                if ((*(volatile u32 *)ADDR_03001B04 & 0x200) != 0)
                    val2 -= 1;
                if ((*(volatile u32 *)ADDR_03001B04 & 1) != 0)
                    break;
                if ((*(volatile u32 *)ADDR_03001B04 & 8) != 0)
                    Func_080b5534();
                if ((*(volatile u32 *)ADDR_03001B04 & 4) != 0)
                    BattleRuntime_ReservedNoOp2A08();
                if ((*(volatile u32 *)ADDR_03001B04 & 2) != 0 || held != 0) {
                    held = 1;
                    *bytePtr = 5;
                }
                if (val2 != prev2) {
                    Func_08077098();
                    Func_080b5368(val2);
                    prev2 = val2;
                }
                WaitFrames(1);
            }

            if ((*(volatile u32 *)ADDR_03001AE8 & 0x80) != 0)
                Func_080770c8(364);
            BattleUnit_Recalculate(0);
            *halfPtr = 29;
            if (val1 == 28)
                Func_080770c8(366);
            Func_080770c8(354);
            Func_080b63c8(val1);
            Func_0800479c();
            Func_08004760();
            Scheduler_ResetTaskTable();
            Func_08004858();
            Resource_InitializeTable();
        }
    }
}
