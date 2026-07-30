typedef signed int s32;

/*
 * resource_38d owner at 0x020002cc, 108 bytes: code 0x020002cc-0x0200031b and
 * the seven-word literal pool at 0x0200031c-0x02000337.  A control-flow walk
 * from the prologue reaches the pool only through `ldr r0, [pc, #imm]`; the
 * last instruction before it is `bx r0` and every forward branch in the body
 * lands at or before 0x0200030c, so no pool word is decoded as an instruction.
 *
 * One dialogue beat on slot 17, choosing which line to show from three story
 * flags and then waiting for the slot's action to finish.  The join at
 * 0x0200030c is shared by all four arms, which is why the wait is written once
 * below rather than per arm.
 *
 * Call accounting: 10 `bl` sites, all resolved with
 * `bun tools/overlay_call_targets.ts resource_38d 02cc --json` (five distinct
 * import veneers: Func_0808a170 x4, Func_080770c0 x3, and one each of
 * Func_0808a018, Func_0808a180, Func_0808a020).  Every site appears below
 * exactly once - the multiset of C call expressions matches the multiset of
 * resolved sites target for target.  No intra-overlay call and no `call_via`
 * slot; neither r3 nor r4 is written anywhere in the owner.
 *
 * The epilogue is `pop {r0} / bx r0`, so r0 holds the popped return address
 * and the owner is void.  The prologue is `push {lr}` and the first branch is
 * taken with no argument register written, so the owner takes no arguments.
 *
 * UNCERTAINTY: 0x1434 and 0x1436 are two adjacent entries of one dialogue
 * block and 0x174c / 0x176f are from another; only their use as line ids is
 * established.
 */

/* Imports. Old-style declarations: one name can take different argument
 * counts at different sites in this overlay. */
void Func_0808a018();           /* scripted-scene bracket: open */
void Func_0808a020();           /* scripted-scene bracket: close */
void Func_0808a170();           /* show a dialogue line by id */
void Func_0808a180();           /* wait for the slot's action to finish */
s32 Func_080770c0();            /* story-flag test; used in a condition */

void Func_020002cc(void)
{
    Func_0808a018();

    if (Func_080770c0(0x202) != 0) {
        Func_0808a170(0x174c);
    } else if (Func_080770c0(0x845) == 0) {
        Func_0808a170(0x1436);
    } else {
        Func_0808a170(0x1434);
        if (Func_080770c0(0x84e) != 0) {
            Func_0808a170(0x176f);
        }
    }

    Func_0808a180(17, 0);
    Func_0808a020();
}
