typedef signed int s32;

/*
 * resource_38d owner at 0x020001b4, 64 bytes: code 0x020001b4-0x020001e5 and
 * the three-word literal pool at 0x020001e8-0x020001f3 (0x020001e6 is the
 * two-byte alignment `movs r0, r0` that the pool needs).  The pool is reached
 * only through `ldr rN, [pc, #imm]`; the control-flow walk from the prologue
 * never falls into it, because the last instruction before it is `bx r0`.
 *
 * One short scripted beat: it brackets the body with the
 * Func_0808a018 / Func_0808a020 pair, shows a dialogue line, shows a second
 * line only when a story flag is already set, waits for slot 15's action, and
 * finally sets that same story flag.
 *
 * Call accounting: 7 `bl` sites, all resolved with
 * `bun tools/overlay_call_targets.ts resource_38d 01b4 --json` (six distinct
 * import veneers, no intra-overlay call, no `call_via` slot; the owner loads
 * neither r3 nor r4 anywhere).  Every site appears below exactly once.  The
 * disassembler's own `bl` annotations are wrong in the usual overlay way - the
 * halfword pair stores `target_offset - 2`, not a pc-relative displacement -
 * so the names below come from the veneers' trailing words, not from the
 * listing.
 *
 * The epilogue is `pop {r0} / bx r0`, so r0 holds the popped return address
 * and the owner is void.  The prologue is `push {lr}` and the first branch is
 * taken with no argument register written, so nothing here reads an incoming
 * argument: the owner takes none.
 *
 * Import identities are the main-image addresses in each veneer's trailing
 * word, the spelling the rest of the semantic overlay tree uses.
 *
 * UNCERTAINTY: 0x1750 and 0x1768 are passed to the same import in the same
 * position and are 24 apart, which reads as two entries of one dialogue block;
 * only their use as line ids is established here.
 */

/* Imports. Old-style declarations: one name can take different argument
 * counts at different sites in this overlay. */
void Func_0808a018();           /* scripted-scene bracket: open */
void Func_0808a020();           /* scripted-scene bracket: close */
void Func_0808a170();           /* show a dialogue line by id */
void Func_0808a180();           /* wait for the slot's action to finish */
s32 Func_080770c0();            /* story-flag test; used in a condition */
void Func_080770c8();           /* set a story flag */

void Func_020001b4(void)
{
    Func_0808a018();
    Func_0808a170(0x1750);

    /* The flag is tested before it is set below, so the second line is the
     * "already seen this beat" variant. */
    if (Func_080770c0(0x302) != 0) {
        Func_0808a170(0x1768);
    }

    Func_0808a180(15, 0);
    Func_080770c8(0x302);
    Func_0808a020();
}
