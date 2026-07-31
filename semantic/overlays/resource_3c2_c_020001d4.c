typedef signed int s32;

/*
 * Resource 3c2 dialogue step at 0x020001d4.
 *
 * Owner is complete: `push {r5, r6, lr}` at 0x020001d4 and the single
 * interworking epilogue at 0x02000210.  One literal pool follows it at
 * 0x02000216-0x0200021b (alignment halfword plus the message base 0x2624).
 *
 * Return type: `pop {r5, r6} / pop {r0} / bx r0` -- r0 holds the popped return
 * address, so the owner is void.
 *
 * This is exactly the raised-flag branch of 0x0200006c lifted into its own
 * owner, with the message base 0x2624 instead of 0x261c and with no facing or
 * flag test in front of it.  That correspondence is the correctness check for
 * both files.
 *
 * All seven call sites resolved with
 * `bun tools/overlay_call_targets.ts resource_3c2 01d4 --json`:
 *   0x020001dc -> veneer 0x0bc4 -> Func_0808a170
 *   0x020001e4 -> veneer 0x0bcc -> Func_0808a178
 *   0x020001ec -> veneer 0x0b5c -> Func_0808a070
 *   0x020001f6 -> veneer 0x0b44 -> Func_0808a010
 *   0x020001fc -> veneer 0x0bc4 -> Func_0808a170
 *   0x02000204 -> veneer 0x0bc4 -> Func_0808a170
 *   0x0200020c -> veneer 0x0bd4 -> Func_0808a180
 * Per target: Func_0808a170 x3, and one each of Func_0808a178, Func_0808a070,
 * Func_0808a010, Func_0808a180.  Seven C calls, target by target, against
 * seven sites.  The two follow-up arms are distinct call sites of
 * Func_0808a170 and are not merged.
 *
 * Uncertainties.  The two zero arguments of Func_0808a070 and the message ids
 * 0x2624/0x2625/0x2626 are transcribed, not interpreted.
 */

/* Overlay imports (through the veneer table).  Old-style declarations:
 * overlay imports vary their argument count between call sites. */
void Func_0808a170();
void Func_0808a178();
s32 Func_0808a070();
void Func_0808a010();
void Func_0808a180();

void Func_020001d4(s32 actor)
{
    s32 message;

    message = 0x2624;
    Func_0808a170(message);
    Func_0808a178(actor, 0);

    if (Func_0808a070(0, 0) == 0) {
        Func_0808a010(10);
        Func_0808a170(message + 1);
    } else {
        Func_0808a170(message + 2);
    }

    Func_0808a180(actor, 0);
}
