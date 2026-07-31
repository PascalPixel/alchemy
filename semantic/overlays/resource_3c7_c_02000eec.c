typedef signed int s32;

/*
 * Resource 3c7 cutscene script at 0x02000eec.
 *
 * Complete owner: `push {lr}` at 0x02000eec through the interworking return
 * `pop {r0} / bx r0` at 0x0200103a.  r0 receives the popped return address, so
 * the owner returns nothing (HANDOVER §0).  r0's entry value is overwritten
 * before use, so the owner takes no arguments.
 *
 * Straight-line: the body contains no branch of any kind.  The only non-code
 * bytes in the span are the alignment halfword at 0x0200103e and the literal
 * pool 0x02001040-0x0200104b, all past the epilogue; a control-flow walk from
 * the prologue confirms nothing else is skipped.
 *
 * The three pool words are small constants (0x2880, 0x0105, 0x2002), not
 * in-image addresses, so this owner is not a link-base witness either way.
 *
 * The script is a sequence of "act on channel N, then wait k frames" beats.
 * Func_0808a010 is the wait (its argument is a frame count), and channels 0,
 * 1, 2, 3, 11 and 12 are driven by Func_0808a110 / Func_0808a138 /
 * Func_0808a180.
 *
 * Call accounting, per target, against `overlay_call_targets.ts`
 * (46 sites, 9 distinct); each count was verified against the individual call
 * sites rather than inferred from the total:
 *   Func_0808a010  22   Func_0808a180  9    Func_0808a138  5
 *   Func_0808a110  4    Func_0808a148  2    Func_0808a170  1
 *   Func_0808a178  1    Func_0808a1b8  1    Func_0808a1e8  1
 *
 * Uncertainties: the channel ids and the beat constants are transcribed
 * literally; what each channel drives is not established here, and the meaning
 * of Func_0808a1e8's middle argument 0x105 (0x102 at the corresponding site in
 * Func_02000308) is unknown.
 */

/* Import veneers.  Old-style declarations: one import name can take different
 * argument counts at different sites in this overlay, so no arity is asserted
 * and none of these results is used. */
void Func_0808a010();
void Func_0808a110();
void Func_0808a138();
void Func_0808a148();
void Func_0808a170();
void Func_0808a178();
void Func_0808a180();
void Func_0808a1b8();
void Func_0808a1e8();

void Func_02000eec(void)
{
    Func_0808a170(0x2880);
    Func_0808a010(20);

    Func_0808a138(11, 2);
    Func_0808a010(20);
    Func_0808a180(11, 0);
    Func_0808a010(10);

    Func_0808a148(1, 0, 50);
    Func_0808a1e8(0, 0x105, 60);
    Func_0808a010(10);
    Func_0808a1b8(1, 0xc000, 0);
    Func_0808a010(20);
    Func_0808a180(1, 0);
    Func_0808a010(10);

    Func_0808a110(3, 4);
    Func_0808a010(20);
    Func_0808a180(3, 0);
    Func_0808a010(10);

    Func_0808a138(2, 2);
    Func_0808a010(20);
    Func_0808a180(2, 0);
    Func_0808a010(10);

    Func_0808a138(12, 2);
    Func_0808a010(20);
    Func_0808a180(12, 0);
    Func_0808a010(20);

    Func_0808a138(0, 2);
    Func_0808a010(20);
    Func_0808a010(25);

    Func_0808a138(1, 2);
    Func_0808a010(30);
    Func_0808a180(1, 0);
    Func_0808a010(30);

    Func_0808a110(1, 4);
    Func_0808a010(20);
    Func_0808a180(1, 0);
    Func_0808a010(10);

    Func_0808a110(3, 4);
    Func_0808a010(20);
    Func_0808a180(3, 0);
    Func_0808a010(10);

    Func_0808a110(2, 3);
    Func_0808a010(30);
    Func_0808a180(2, 0);
    Func_0808a010(10);

    Func_0808a148(2, 0, 30);
    Func_0808a178(0x2002, 0);
}
