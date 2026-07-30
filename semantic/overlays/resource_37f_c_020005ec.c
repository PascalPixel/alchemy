typedef signed int s32;

/*
 * Resource 37f, owner at 0x020005ec (72 bytes: prologue at 0x020005ec through
 * the interworking return `pop {r5} / pop {r0} / bx r0`, plus the trailing
 * literal pool that the epilogue is followed by).  r0 is the popped return
 * address, so the owner is `void`.
 *
 * Sibling of 0x0200056c/0x020005ac and of the other five 72-byte members
 * (0x020005ec, 0x02000634, 0x0200067c, 0x020006c4, 0x0200070c, 0x02000754).
 * Across the eight members only the record id, the two cue ids and the two
 * heading values differ; that two-value-per-member diff proves the shared
 * callees rather than inferring them.
 *
 * All six call targets were resolved with `tools/overlay_call_targets.ts`
 * (an overlay `bl` stores target_offset - 2, so the disassembler annotations
 * are wrong): five import veneers plus one call into this overlay,
 * Func_020017c0, reached with r0 = 0.
 *
 * Func_0808a080(id) is the scene-record accessor, returning the record or 0.
 * Word 2 of the record (+8) is a 16.16 heading-like field; its top 12 bits
 * (arithmetic shift right by 20) select the variant.
 */

/* Old-style declarations: overlay import interfaces are not fully known. */
s32 Func_0808a080();
void Func_080770d0();
void Func_080770c8();
void Func_020017c0();

void Func_020005ec(void)
{
    s32 *record;
    s32 heading;

    record = (s32 *)Func_0808a080(9);
    if (record == 0) {
        return;
    }

    heading = record[2] >> 20;

    Func_080770d0(0x310);
    Func_080770d0(0x311);

    if (heading == 99) {
        Func_080770c8(0x311);
    } else if (heading == 101) {
        Func_080770c8(0x310);
    }

    Func_020017c0(0);
}
