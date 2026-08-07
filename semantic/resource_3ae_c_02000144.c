#include "types.h"

/*
 * Resource 3ae, owner at 0x02000144 (88 bytes including its three-word pool at
 * 0x02000190..0x0200019a).
 *
 * Role known in advance from the call graph: the in-image handler table holds
 * the odd word 0x02008145 == Func_02000144 + 1 as the payload of the entry
 * keyed 0x08aa0014, so this is a table-dispatched scene beat invoked with no
 * arguments.
 *
 * Prologue `push {lr}` at 0x02000144; epilogue `pop {r0} / bx r0` at
 * 0x0200018c.  The return address is popped into r0, so r0 does not survive
 * and the owner is void.  No frame: every call's arguments fit in r0-r3.
 *
 * Call targets resolved with `tools/overlay_call_targets.ts` (an overlay `bl`
 * stores target offset - 2; the disassembler's annotation is wrong):
 *
 *   0x02000146 -> Func_0808a018   (scene open)
 *   0x0200014c -> Func_080770c8   (event-flag set, id 0x8aa)
 *   0x0200015a -> Func_0808a0d0
 *   0x02000164 -> Func_0808a090
 *   0x02000172 -> Func_0808a0d0
 *   0x0200017e -> Func_0808a1b8   (facing)
 *   0x02000184 -> Func_0808a010   (beat delay)
 *   0x02000188 -> Func_0808a020   (scene close)
 *
 * sites=8 equals the inventory's calls=8, so the call set is complete.
 *
 * The Func_0808a018 / Func_0808a020 bracket, Func_0808a010(n) as the beat
 * delay and Func_0808a1b8(slot, angle, 0) as the facing are the same idioms
 * the neighbouring scene scripts 0x020002dc, 0x020004b8 and 0x02000ba0 use.
 * The two Func_0808a0d0 calls are distinct sites — one for slot 0, one for
 * slot 8 — and are not merged.
 *
 * Uncertainties: 0x8aa is read as an event-flag id from its position as the
 * sole argument of Func_080770c8, whose result is discarded.  The Func_0808a090
 * arguments 0x13333 and 0x9999 are the same pair Func_02000ba0 passes to the
 * same import, which corroborates the arity but not the units.
 */

/* Old-style declarations: overlay imports vary in arity between call sites. */
void Func_0808a018();
void Func_0808a020();
void Func_0808a010();
void Func_080770c8();
void Func_0808a090();
void Func_0808a0d0();
void Func_0808a1b8();

void Func_02000144(void)
{
    Func_0808a018();
    Func_080770c8(0x8aa);

    /* movs r1,#196 / lsls r1,r1,#1 == 392; movs r2,#148 / lsls #1 == 296 */
    Func_0808a0d0(0, 392, 296);

    Func_0808a090(8, 0x13333, 0x9999);

    /* movs r1,#204 / lsls r1,r1,#1 == 408 */
    Func_0808a0d0(8, 408, 296);

    Func_0808a1b8(8, 0x8000, 0);
    Func_0808a010(20);
    Func_0808a020();
}
