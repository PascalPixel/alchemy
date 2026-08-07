#include "types.h"

/*
 * Resource 3ae, owner at 0x0200086c (96 bytes including its four-word pool at
 * 0x020008bc..0x020008ca).
 *
 * Exported entry: the overlay's veneer table at 0x02000000 opens with the pool
 * word 0x0200886d == Func_0200086c + 1, so this is the overlay's first public
 * entry point — the per-room setup hook.
 *
 * Prologue `push {r5,lr}` at 0x0200086c; epilogue `movs r0,#0 / pop {r5} /
 * pop {r1} / bx r1` at 0x020008b2.  The return address is popped into r1, so
 * r0 survives; r0 is deliberately zeroed immediately before, so the owner
 * returns a constant 0 (kept as s32 rather than void because the zero is
 * explicit and reachable from every arm).
 *
 * Workspace: r5 holds `Data_02000240`, the cross-overlay workspace whose
 * halfword at byte offset 448 (`[224]`) is the room id — the same selector the
 * byte-exact sibling `assets/code/resource_3ae_c_02000040.c` and the exported
 * accessors at 0x020000a0 / 0x0200019c use, against the same three ids.  The
 * halfword at offset 450 (`[225]`) is a second, independent selector compared
 * against 90.
 *
 * Call targets resolved with `tools/lib/overlay_call_targets.ts` (an overlay `bl`
 * stores target offset - 2; the disassembler's annotation is wrong):
 *
 *   0x02000882 -> 0x1558 veneer -> Func_080770c8
 *   0x02000896 -> Func_020008cc   (prologue, room 0x6b)
 *   0x020008a2 -> Func_02000958   (prologue, room 0x70)
 *   0x020008ae -> Func_02000ad4   (prologue, room 0x6c; byte-exact source)
 *
 * sites=4 equals the inventory's calls=4, so the call set is complete.  The
 * three per-room callees take no arguments and return void: Func_02000ad4 is
 * already byte-exact and has that signature, and neither of the other two is
 * passed anything in r0-r3 at its call site.
 *
 * Uncertainties: 0x950 is read as an event-flag id from its use as the sole
 * argument of Func_080770c8, whose result is discarded here (it is a setter or
 * a test called for effect, not a query).  Both selector compares are signed
 * 32-bit (`ldrsh` into r2/r3 then `cmp`).
 */

/* Old-style declaration: overlay imports vary in arity between call sites. */
void Func_080770c8();

void Func_020008cc(void);
void Func_02000958(void);
void Func_02000ad4(void);

extern s16 Data_02000240[];

s32 Func_0200086c(void)
{
    if (Data_02000240[225] == 90) {
        /* movs r0,#149 / lsls r0,r0,#4 == 0x950 */
        Func_080770c8(0x950);
    }

    switch ((s32)Data_02000240[224]) {
    case 0x6b:
        Func_020008cc();
        break;
    case 0x70:
        Func_02000958();
        break;
    case 0x6c:
        Func_02000ad4();
        break;
    default:
        break;
    }

    return 0;
}
