typedef unsigned char u8;
typedef signed int s32;

/*
 * Resource 3a6 one-shot scene 0x200 at 0x020011a0 (182 bytes, 20 calls).
 *
 * Derived span, not an inventory row: this owner has no row in
 * out/decomp/overlays.json and no exact sibling.  It was found by sweeping the
 * two-byte gaps in metrics/gs1-en-executable.json for `push {..,lr}`
 * prologues.  Walking from the prologue at 0x020011a0 there is exactly one
 * branch — the `bne` at 0x020011ac to 0x02001252 — and the walk stops at the
 * interworking return `pop {r0} / bx r0` at 0x02001252-0x02001254.  So the
 * executable extent is exactly 0x020011a0-0x02001255 (182 bytes).  The popped
 * register is r0, so the popped value is the return address and the owner is
 * `void`.
 *
 * Pool map: none — every constant is built with `movs` + `lsls`.  The halfword
 * at 0x02001256 is `0x0000` alignment before the next prologue at 0x02001258,
 * and the walk never reaches it as an instruction.
 *
 * All twenty call sites were resolved with
 * `bun tools/overlay_call_targets.ts resource_3a6 11a0 1256`; the per-target
 * histogram is Func_0808a010 x4, Func_0808a0d0 x2, Func_0808a1b8 x2 and one
 * each of Func_080770c0, Func_080770c8, Func_0808a018, Func_0808a020,
 * Func_0808a080, Func_0808a090, Func_0808a138, Func_0808a1f0, Func_0808a200,
 * Func_0808a208, Func_0808a218, Func_080f9010.  Occurrences are assigned to
 * sites in address order; the last site is Func_0808a020 and the first inside
 * the guard is Func_0808a018, which is the convention's own sanity check.
 *
 * IT READS EVENT FLAG 0x200 AND THEN SETS IT, which settles "is this a
 * one-shot scene" in one line without any dataflow work: Func_080770c0 is the
 * test and Func_080770c8 the set, and `movs r0,#128 / lsls r0,#2` builds the
 * same 0x200 for both.  The early return is therefore the already-played path,
 * and it correctly skips the close as well as the open — this is not the
 * asymmetric bracket documented for resource_3ae, where an arm reaches the
 * close without the open.
 *
 * Every other constant is built the same `movs`+`lsls` way and every one of
 * them is a VALUE, not a displacement: 0x10000, 0x2000, 0xc000, 258, 0x8000,
 * 792, 0x80000.  The single store is to +40 of the record Func_0808a080
 * returns.
 */

s32 Func_080770c0();           /* test event flag */
void Func_080770c8();          /* set event flag */
void Func_0808a018();          /* scene open */
void Func_0808a208();
void Func_0808a200();
void Func_0808a218();
void Func_0808a010();          /* wait this many frames */
void Func_0808a1b8();
void Func_0808a1f0();
void Func_0808a138();
void Func_0808a090();
void Func_0808a0d0();
void Func_080f9010();
u8 *Func_0808a080();           /* record fetch, returns the record */
void Func_0808a020();          /* scene close */

void Func_020011a0(void)
{
    if (Func_080770c0(0x200) != 0) {
        return;
    }
    Func_080770c8(0x200);

    Func_0808a018();
    Func_0808a208(0x10000, 0x2000);
    Func_0808a200(8, 1);
    Func_0808a218();
    Func_0808a010(60);
    Func_0808a1b8(8, 0xc000, 20);
    Func_0808a1f0(8, 258);
    Func_0808a138(8, 2);
    Func_0808a010(20);
    Func_0808a090(8, 0x10000, 0x8000);
    Func_0808a0d0(8, 792, 248);
    Func_080f9010(152);
    *(s32 *)(Func_0808a080(8) + 40) = 0x80000;
    Func_0808a0d0(8, 792, 280);
    Func_0808a010(20);
    Func_0808a1b8(8, 0xc000, 20);
    Func_0808a010(30);
    Func_0808a020();
}
