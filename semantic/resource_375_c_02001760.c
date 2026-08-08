#include "types.h"

/*
 * resource_375 owner at 0x02001760, 580 bytes: code 0x02001760-0x02001971, an
 * alignment halfword at 0x02001972 and the literal pool
 * 0x02001974-0x020019a3.
 *
 * Conventions.  An overlay `bl` stores the target's image offset minus two, not
 * a pc-relative displacement, so the disassembler's printed targets are wrong;
 * all 60 call sites below were resolved with `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml --`
 * and every one lands in the 8-byte-per-entry import veneer table starting at
 * 0x02001a54, so each is named by that entry's trailing main-image word.  (The
 * inventory advertises calls=55; that field counts distinct targets and is
 * known to be low.)  resource_375 is linked at 0x02008000, so the pool words
 * 0x02009ce0 and 0x02009bdc are in-image data blocks at file offsets 0x1ce0
 * and 0x1bdc, not RAM globals.
 *
 * Return type: `pop {r5} / pop {r0} / bx r0` pops the return address into r0,
 * so nothing is returned and the owner is void.
 *
 * Behaviour: an event scene guarded by two flags.  Flag 0x0839 already set
 * means the whole scene is skipped.  Flag 0x082f set means the short "already
 * done" line 0x0e8b is shown and the scene ends.  Otherwise the full scene runs
 * from base line id 0x0e85: two actors are placed and faced, a yes/no prompt is
 * put through Func_0808a070(0, 0), and the answer decides between line
 * base + 2 (declined -- sets flag 0x082f) and line base + 3 followed by the
 * long sequence that ends by setting flag 0x0839.  Both scene paths converge on
 * a common teardown that snaps actor 1 onto slot 0's position and closes.
 *
 * r5 is reused for two unrelated values -- first the dialogue base id, then the
 * data-block address 0x02009ce0 -- so it is modelled as two locals.
 *
 * The Func_0808a080(0) record is read two different ways here: as the 16.16
 * words at +8 and +16 (`ldr`) at 0x020017f8 and as their integer-part signed
 * halfwords at +10 and +18 (`ldrsh`) at 0x0200194a.  That double view of one
 * coordinate pair is an idiom this project has already recorded, not an
 * inconsistency.
 *
 * Uncertainties: 0x0839, 0x082f, 0x0e85, 0x0e8b, 0x0101, 0x0105, 0x0117,
 * 0x0125, 0x011b0000 and 0x0001000b are identifiers or packed parameters whose
 * tables are not reconstructed, so they stay numeric.  Func_0808a210's second
 * argument is the sign-extended -1 produced by `movs r1,#1 / negs r1,r1`.
 */

/* Imports, old-style: arities are established per call site.  The two tested
 * imports need explicit return types. */
s32 Func_020031ca();
s32 Func_020031d6();
void Func_020031fe();
void Func_0200328e();
void Func_020032b4();
void Func_020032cc();
void Func_02003220();
void Func_0200321e();
void Func_02003254();
void Func_020032dc();
void Func_020032fe();
void Func_02003322();
void Func_02003352();
void Func_02003340();
u8 *Func_0200328e_b();
void Func_020032e4();
void Func_020032de();
void Func_0200336a();
void Func_0200332a();
void Func_020032a0();
void Func_02003360();
s32 Func_020032c8();
void Func_0200334c();
void Func_020032c2();
void Func_02003378();
void Func_02003390();
void Func_020032c6();
void Func_02003370();
void Func_020032e6();
void Func_0200339c();
void Func_020033be();
void Func_02003398();
void Func_02003392();
void Func_0200338a();
void Func_02003404();
void Func_020033f6();
void Func_020033a6();
void Func_0200333c();
void Func_020033bc();
void Func_02003416();
void Func_020033ce();
void Func_0200340a();
void Func_02003414();
void Func_020033b4();
void Func_020033b2();
void Func_020033b8();
void Func_02003464();
void Func_02003470();
void Func_02003482();
void Func_0200348c();
void Func_020033a2();
void Func_0200342a();
u8 *Func_020033e0();
void Func_0200341a();
void Func_02003438();
void Func_0200344a();

                     

                     

                     

                     

                     

                     

                     

                     

void Func_02001760(void)
{
    s32 line;
    void *block;
    u8 *slot;

    if (Func_020031ca(0x839) != 0) {
        return;
    }

    if (Func_020031d6(0x82f) != 0) {
        Func_020031fe();
        Func_0200328e(11, 2);
        Func_020032b4(0xe8b);
        Func_020032cc(11, 0);
        Func_02003220();
        return;
    }

    Func_0200321e();
    Func_02003254(11);
    Func_020032b4(11, 1);

    line = 0xe85;
    Func_020032dc(line);
    Func_020032fe(11, 0, 20);
    Func_02003322(0, 0x100, 30);
    Func_02003352(0x620000, -1, 0x011b0000, 1);
    Func_020032b4(0, 0x5e, 0x125);
    Func_02003340(0, 0xa000, 0);

    slot = Func_0200328e_b(0);
    if (slot != 0) {
        Func_020032e4(1, *(s32 *)(slot + 8), *(s32 *)(slot + 16));
    }

    Func_020032de(1, 0x6e, 0x117);
    Func_0200336a(1, 0xa000, 40);
    Func_0200332a(11, 2);
    Func_020032a0(40);
    Func_02003360(11, 0);

    if (Func_020032c8(0, 0) == 0) {
        Func_0200334c(11, 2);
        Func_020032c2(20);
        Func_02003378(line + 2);
        Func_02003390(11, 0);
        Func_020032c6(0x82f);
    } else {
        Func_02003370(11, 2);
        Func_020032e6(20);
        Func_0200339c(line + 3);
        Func_020033be(11, 0, 40);
        Func_02003398(11, 0, 0);
        Func_02003378(11, 1);
        Func_02003392(11, 4, 40);
        Func_0200338a(11, 6);
        Func_02003404(11, 0x101, 40);
        Func_020033f6(11, 0, 10);
        Func_020033a6(11, 1);
        Func_0200333c(10);
        Func_020033bc(11, 3);
        Func_02003416(11, 0, 10);
        Func_020033ce(11, 3);

        block = (void *)0x02009ce0;
        Func_0200340a(0, 0x0001000b, block);
        Func_02003414(1, 0x0001000b, block);
        Func_020033b4(11, (void *)0x02009bdc);
        Func_020033b2(0);
        Func_020033b8(1);
        Func_02003464(0, 0x4000, 0);
        Func_02003470(1, 0x4000, 60);
        Func_02003482(0, 0x105, 0);
        Func_0200348c(1, 0x105, 120);
        Func_020033a2(0x839);
    }

    Func_0200342a(1, 2);
    slot = Func_020033e0(0);
    if (slot != 0) {
        Func_0200341a(1, *(s16 *)(slot + 10), *(s16 *)(slot + 18));
    }
    Func_02003438(1);
    Func_0200344a(1, 0, 0);
    Func_020033f6();
}
