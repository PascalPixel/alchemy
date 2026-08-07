#include "types.h"

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
void Func_0200216e();
void Func_020020ec();
void Func_02002164();
void Func_020020fa();
void Func_0200219a();
void Func_02002108();
void Func_0200218a();
void Func_020021c4();
void Func_02002122();
void Func_020021ce();
void Func_02002134();
void Func_020021d4();
void Func_02002142();
void Func_020021aa();
void Func_02002150();
void Func_020021f0();
void Func_0200215e();
void Func_020021d6();
void Func_0200216c();
void Func_0200220c();
void Func_0200217a();
void Func_020021f2();
void Func_02002188();
void Func_02002228();
void Func_02002196();
void Func_0200220e();
void Func_020021a4();
void Func_02002222();
void Func_020021b8();
void Func_02002258();
void Func_020021c6();
void Func_0200222e();
void Func_02002274();
void Func_020021e2();
void Func_0200224a();
void Func_02002290();
void Func_020021fe();
void Func_02002266();
void Func_020022ac();
void Func_0200221a();
void Func_0200229c();
void Func_020022bc();

                     

                     

                     

void Func_02000eec(void)
{
    Func_0200216e(0x2880);
    Func_020020ec(20);

    Func_02002164(11, 2);
    Func_020020fa(20);
    Func_0200219a(11, 0);
    Func_02002108(10);

    Func_0200218a(1, 0, 50);
    Func_020021c4(0, 0x105, 60);
    Func_02002122(10);
    Func_020021ce(1, 0xc000, 0);
    Func_02002134(20);
    Func_020021d4(1, 0);
    Func_02002142(10);

    Func_020021aa(3, 4);
    Func_02002150(20);
    Func_020021f0(3, 0);
    Func_0200215e(10);

    Func_020021d6(2, 2);
    Func_0200216c(20);
    Func_0200220c(2, 0);
    Func_0200217a(10);

    Func_020021f2(12, 2);
    Func_02002188(20);
    Func_02002228(12, 0);
    Func_02002196(20);

    Func_0200220e(0, 2);
    Func_020021a4(20);
    Func_020021aa(25);

    Func_02002222(1, 2);
    Func_020021b8(30);
    Func_02002258(1, 0);
    Func_020021c6(30);

    Func_0200222e(1, 4);
    Func_020021d4(20);
    Func_02002274(1, 0);
    Func_020021e2(10);

    Func_0200224a(3, 4);
    Func_020021f0(20);
    Func_02002290(3, 0);
    Func_020021fe(10);

    Func_02002266(2, 3);
    Func_0200220c(30);
    Func_020022ac(2, 0);
    Func_0200221a(10);

    Func_0200229c(2, 0, 30);
    Func_020022bc(0x2002, 0);
}
