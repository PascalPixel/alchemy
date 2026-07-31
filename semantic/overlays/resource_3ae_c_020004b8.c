typedef signed int s32;

/*
 * Resource 3ae, owner at 0x020004b8 (100 bytes including its five-word pool at
 * 0x02000508..0x0200051a).
 *
 * Role known in advance from the call graph: the in-image handler table holds
 * the odd word 0x020084b9 == Func_020004b8 + 1 as the payload of the entry
 * keyed 0xffff000d, so this is an interaction/examine handler invoked from the
 * table with no arguments.
 *
 * Prologue `push {lr}` at 0x020004b8; epilogue `pop {r0} / bx r0` at
 * 0x02000504.  The return address is popped into r0, so r0 does not survive
 * and the owner is void.
 *
 * Call targets resolved with `tools/overlay_call_targets.ts` (an overlay `bl`
 * stores target offset - 2; the disassembler's annotation is wrong):
 *
 *   0x020004ba -> 0x1580 veneer -> Func_0808a018
 *   0x020004c0 -> 0x1550 veneer -> Func_080770c0  (event-flag test)
 *   0x020004ca -> 0x1618 veneer -> Func_0808a170  (cue trigger)
 *   0x020004d2 -> 0x1620 veneer -> Func_0808a178
 *   0x020004da -> 0x1550 veneer -> Func_080770c0
 *   0x020004e4 -> 0x1618 veneer -> Func_0808a170
 *   0x020004ec -> 0x1628 veneer -> Func_0808a180
 *   0x020004f4 -> 0x1618 veneer -> Func_0808a170
 *   0x020004fc -> 0x1628 veneer -> Func_0808a180
 *   0x02000500 -> 0x1588 veneer -> Func_0808a020
 *
 * sites=10 equals the inventory's calls=10, so the call set is complete.
 *
 * The Func_0808a018 / Func_0808a020 pair brackets the whole body — the same
 * open/close bracket the sibling `semantic/overlays/resource_3ae_c_020002dc.c`
 * and the byte-exact `assets/code/resource_3ae_c_02000144.c` neighbourhood use
 * around a dialogue sequence.  Func_0808a018 is called with no argument set up
 * here (r0 still holds whatever the caller left), so it is declared old-style
 * and invoked with none.
 *
 * The three arms are similar in shape but are DISTINCT sites, and the first is
 * not even the same import: 0x020004d2 goes to veneer 0x1620 (Func_0808a178)
 * while the other two go to 0x1628 (Func_0808a180).  Reading the three arms as
 * copies of one another is exactly the mistake that hides this.
 *
 * Uncertainties: 0x8a7 and 0x8a5 are read as event-flag ids and 0x1d1f /
 * 0x1d1b / 0x1d19 as cue ids, from their use as the sole argument of
 * Func_080770c0 and Func_0808a170 respectively — the same reading the sibling
 * at 0x020002dc records for 0x8a7/0x8a9 and 0x1d23.  The constant 13 is the
 * object/slot id, used consistently across all three arms regardless of which
 * of the two imports takes it.
 */

/* Old-style declarations: overlay imports vary in arity between call sites. */
void Func_0808a018();
void Func_0808a020();
s32 Func_080770c0();
void Func_0808a170();
void Func_0808a178();
void Func_0808a180();

void Func_020004b8(void)
{
    Func_0808a018();

    if (Func_080770c0(0x8a7) != 0) {
        Func_0808a170(0x1d1f);
        Func_0808a178(13, 0);
    } else if (Func_080770c0(0x8a5) != 0) {
        Func_0808a170(0x1d1b);
        Func_0808a180(13, 0);
    } else {
        Func_0808a170(0x1d19);
        Func_0808a180(13, 0);
    }

    Func_0808a020();
}
