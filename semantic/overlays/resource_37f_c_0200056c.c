typedef signed int s32;

/*
 * Resource 37f, owner at 0x0200056c (56 bytes of code + one 8-byte literal
 * pool at 0x020005a4).
 *
 * Complete owner: `push {r5, lr}` at 0x0200056c and the matching interworking
 * return `pop {r5} / pop {r0} / bx r0` at 0x0200059e.  The popped register is
 * r0, so r0 holds the return address and nothing is returned: this is `void`.
 *
 * Call targets were resolved with `tools/overlay_call_targets.ts`, not from the
 * disassembler's `bl` annotations (an overlay `bl` stores target_offset - 2).
 * All five sites are import veneers.
 *
 * This is one of an eight-member sibling family at 0x0200056c, 0x020005ac,
 * 0x020005ec, 0x02000634, 0x0200067c, 0x020006c4, 0x0200070c and 0x02000754.
 * The members differ only in the record id passed to Func_0808a080, in the two
 * cue ids, and in the two heading values tested; the six 72-byte members also
 * make a trailing Func_020017c0(0) call that these two 56/64-byte members do
 * not.  That two-value-per-member diff is itself the proof that the resolved
 * call targets are shared.
 *
 * Func_0808a080(id) is the scene-record accessor: it returns the record or 0.
 * Word 2 of the record (+8) is a 16.16 heading-like field whose top 12 bits
 * (arithmetic shift right by 20) select the variant.
 */

/* Old-style declarations: overlay imports whose full interfaces are not known,
 * and one name can be reached with different argument counts elsewhere. */
s32 Func_0808a080();
void Func_080770d0();
void Func_080770c8();

void Func_0200056c(void)
{
    s32 *record;
    s32 heading;

    record = (s32 *)Func_0808a080(9);
    if (record == 0) {
        return;
    }

    heading = record[2] >> 20;

    Func_080770d0(0x302);
    Func_080770d0(0x303);

    if (heading == 93) {
        Func_080770c8(0x303);
    } else if (heading == 95) {
        Func_080770c8(0x302);
    }
}
