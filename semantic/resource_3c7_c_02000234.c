typedef signed int s32;
typedef unsigned short u16;

/*
 * Resource 3c7 examine handler at 0x02000234.
 *
 * Complete owner: `push {r5, lr}` at 0x02000234 through the interworking return
 * `pop {r5} / pop {r0} / bx r0` at 0x02000292.  r0 receives the popped return
 * address, so the owner returns nothing (HANDOVER §0).
 *
 * Two interior literal pools sit inside the span and are branched over, so a
 * control-flow walk from the prologue never decodes them as instructions:
 * 0x02000276-0x02000283 (alignment halfword plus the 0xffffc000 mask) and
 * 0x02000298-0x0200029b (the flag id and the two message ids).  Neither is code.
 *
 * This is one of four near-identical owners in this overlay (0x0200015c,
 * 0x020001c8, 0x02000234, 0x0200029c).  They differ only in which "player is
 * facing the object" action is invoked and in the pair of message ids used
 * otherwise, which cross-checks all four.
 *
 * `Func_0808a080(0)` is the scene-record accessor (HANDOVER §0 records this
 * identity explicitly, against the reading that its argument shape suggests).
 * The halfword at record offset 6 is the player's heading; biasing it by
 * 0x2000 and keeping bits 14-15 quantises it to a quadrant, and the `lsls #16`
 * before the compare only moves that field into the top half of the word, so
 * the test is exactly "quadrant == 0xc000".
 *
 * Call accounting for this owner, per target, against
 * `overlay_call_targets.ts` (7 sites, 5 distinct):
 *   Func_0808a080  1    Func_080770c0  1    Func_080b0010  1
 *   Func_0808a170  2    Func_0808a180  2
 *
 * Uncertainty: Func_080770c0's parameter 0x9a7 is a story-flag id; only its
 * truth value is used here, and which event it records is not established.
 */

/* Import veneers.  Old-style declarations throughout: two `Func_` names in an
 * overlay can be one import and one name can take different argument counts,
 * so no arity is asserted.  Func_080770c0 is used in a condition and therefore
 * needs a return type. */
s32 Func_080770c0();
u16 *Func_0808a080();
void Func_0808a170();
void Func_0808a180();
void Func_080b0010();

void Func_02000234(s32 actor)
{
    u16 *scene = Func_0808a080(0);

    if ((((s32)scene[3] + 0x2000) & 0xc000) == 0xc000) {
        Func_080b0010(actor);
    } else if (Func_080770c0(0x9a7) != 0) {
        Func_0808a170(0x28fc);
        Func_0808a180(actor, 0);
    } else {
        Func_0808a170(0x26f6);
        Func_0808a180(actor, 0);
    }
}
