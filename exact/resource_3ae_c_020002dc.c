#include "types.h"

/*
 * Resource 3ae, owner at 0x020002dc (76 bytes including its trailing pool).
 *
 * Prologue `push {lr}` at 0x020002dc, interworking return `pop {r0} / bx r0`
 * at 0x02000316; r0 is the popped return address, so the owner is void.
 * The three words at 0x0200031c/0x02000320/0x02000324 are the owner's literal
 * pool (0x8a7, 0x8a9, 0x1d23), reached only by `ldr rN, [pc, ...]`.
 *
 * Call targets resolved with `tools/lib/overlay_call_targets.ts` (overlay `bl`
 * stores target offset - 2; the disassembler's annotation is wrong):
 *
 *   0x020002e0 -> 0x15a0 veneer -> Func_0808a080
 *   0x020002e4 -> 0x1580 veneer -> Func_0808a018
 *   0x020002ea -> 0x1550 veneer -> Func_080770c0
 *   0x020002f4 -> 0x1550 veneer -> Func_080770c0
 *   0x020002fe -> 0x1618 veneer -> Func_0808a170
 *   0x02000306 -> 0x1620 veneer -> Func_0808a178
 *   0x02000312 -> 0x1640 veneer -> Func_0808a1b8
 *
 * Uncertainties: r0 is not reloaded between the first two branches, so the
 * record pointer returned by Func_0808a080(0) is passed straight into
 * Func_0808a018; whether that callee reads r0 is unverified and the dataflow
 * is preserved as written.  0x8a7/0x8a9 are read as event-flag ids and
 * 0x1d23 as a cue id, from their use as sole arguments of the two guards and
 * of the trigger respectively.
 */

/* Old-style declarations: overlay imports vary in arity between call sites. */







extern void Func_02001882();
extern void * Func_02001866();
extern s32 Func_0200183c();
extern s32 Func_02001846();
extern void Func_02001918();
extern void Func_02001928();
extern void Func_02001954();
void Func_020002dc(void)
{
    Func_02001882(0);
    Func_02001866();

    if (Func_0200183c(0x8a7) != 0 && Func_02001846(0x8a9) != 0) {
        Func_02001918(0x1d23);
        Func_02001928(12, 0);
        /* movs r1,#0x80 / lsls r1,r1,#7 == 0x4000 */
        Func_02001954(12, 0x4000, 0);
    }
}
