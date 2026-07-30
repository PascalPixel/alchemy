/*
 * resource_38d owner at 0x02000150, 100 bytes: code 0x02000150-0x020001ad
 * (0x020001ae is the two-byte alignment `movs r0, r0`) and the single pool
 * word at 0x020001b0.  The control-flow walk from the prologue is straight
 * line - no branch at all - and ends at `bx r0`, so the word is never reached
 * as an instruction.
 *
 * One scripted beat driving slot 14: it brackets the body with the
 * Func_0808a018 / Func_0808a020 pair, runs an animation, selects a
 * presentation, waits, shows a dialogue line, then alternates waits with two
 * more actions and finally faces the slot along heading 0xb000.
 *
 * Call accounting: 11 `bl` sites, all resolved with
 * `bun tools/overlay_call_targets.ts resource_38d 0150 --json` (9 distinct
 * import veneers, Func_0808a010 and Func_0808a188 twice each; no intra-overlay
 * call, no `call_via` slot - neither r3 nor r4 is written anywhere in the
 * owner).  Every site appears below exactly once.  The listing's own `bl`
 * annotations are wrong in the usual overlay way, so the names come from the
 * veneers' trailing words.
 *
 * The epilogue is `pop {r0} / bx r0`, so r0 holds the popped return address
 * and the owner is void.  The prologue is `push {lr}` and the first branch is
 * taken with no argument register written, so the owner takes no arguments.
 *
 * The immediates below are written as the assembly builds them: 129 << 1 =
 * 258 and 176 << 8 = 0xb000.
 *
 * UNCERTAINTY: Func_0808a148's two zero arguments and Func_0808a1e8's trailing
 * zero are set explicitly at each site, so they are asserted here, but neither
 * import's interface is established.
 */

/* Imports. Old-style declarations: one name can take different argument
 * counts at different sites in this overlay. */
void Func_0808a010();           /* wait n frames */
void Func_0808a018();           /* scripted-scene bracket: open */
void Func_0808a020();           /* scripted-scene bracket: close */
void Func_0808a138();           /* select an entity presentation */
void Func_0808a148();           /* scene request; interface not established */
void Func_0808a170();           /* show a dialogue line by id */
void Func_0808a188();           /* wait for the slot's action, with a duration */
void Func_0808a1b8();           /* face slot along heading */
void Func_0808a1e8();           /* play animation id for n frames */

void Func_02000150(void)
{
    Func_0808a018();

    Func_0808a1e8(14, 258, 0);
    Func_0808a138(14, 2);
    Func_0808a010(40);

    Func_0808a170(0x1764);

    Func_0808a188(14, 0, 20);
    Func_0808a148(14, 0, 0);
    Func_0808a010(20);
    Func_0808a188(14, 0, 10);
    Func_0808a1b8(14, 0xb000, 10);

    Func_0808a020();
}
