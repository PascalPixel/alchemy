typedef int s32;

/*
 * resource_39e owner at 0x0200254c, 40 bytes: the overlay's smallest
 * published owner -- a bracketed one-line dialogue beat.
 *
 * Complete owner: `push {lr}` at 0x0200254c through `pop {r0} / bx r0` at
 * 0x0200256c-0x0200256e, then the one-word literal pool at 0x02002570
 * (0x17df). The next owner's prologue is at 0x02002574. **40 bytes**,
 * measured to the epilogue.
 *
 * SPAN CORRECTION WORTH RECORDING. Bounding this row by the next entry in
 * the published sweep's own list gives 108 bytes, because 0x02002574 is
 * already a recorded owner and therefore does not appear in the residue.
 * The true span is 40. A bound taken from a residue list is a bound
 * against the next UNDRAFTED row, not the next row -- three of the five
 * rows in this cluster were over-estimated that way, and only measuring
 * to the epilogue caught it.
 *
 * Not found by the structural inventory walk (unindexed): reached only as
 * a published pointer. All 5 `bl` targets resolved with
 * `bun tools/overlay_call_targets.ts resource_39e 254c 2574` under the
 * `+2` rule. No branches.
 *
 * SHARED IDIOM: the Func_0808a018/Func_0808a020 scripted-scene bracket,
 * cited from resource_39e_c_02001494.c. The Func_0808a170 dialogue id
 * followed by Func_0808a180(id, 0) is the same pairing 0x02002508 uses on
 * its non-default arm, with a different actor id and a different line.
 *
 * Uncertainty: the four callees are not identified beyond call shape;
 * 0x17df is recorded as a passed dialogue id.
 */

extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern void Func_0808a138(s32 id, s32 arg1);
extern void Func_0808a170(s32 dialogue_id);
extern void Func_0808a180(s32 id, s32 arg1);

void Func_0200254c(void)
{
    Func_0808a018();
    Func_0808a138(8, 2);
    Func_0808a170(0x17df);
    Func_0808a180(8, 0);
    Func_0808a020();
}
