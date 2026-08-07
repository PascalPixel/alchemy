#include "types.h"

/*
 * resource_375 owner at 0x02001760, 580 bytes: code 0x02001760-0x02001971, an
 * alignment halfword at 0x02001972 and the literal pool
 * 0x02001974-0x020019a3.
 *
 * Conventions.  An overlay `bl` stores the target's image offset minus two, not
 * a pc-relative displacement, so the disassembler's printed targets are wrong;
 * all 60 call sites below were resolved with `tools/lib/overlay_call_targets.ts`
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
s32 Func_080770c0();
void Func_0808a018();
void Func_0808a138();
void Func_0808a170();
void Func_0808a180();
void Func_0808a020();
void Func_0808a0a8();
void Func_0808a188();
void Func_0808a1e8();
void Func_0808a210();
void Func_0808a0d0();
void Func_0808a1b8();
u8 *Func_0808a080();
void Func_0808a0f0();
void Func_0808a010();
void Func_0808a178();
s32 Func_0808a070();
void Func_080770c8();
void Func_0808a148();
void Func_0808a100();
void Func_0808a128();
void Func_0808a110();
void Func_0808a168();
void Func_0808a0b0();
void Func_0808a0b8();
void Func_0808a0e8();

void Func_02001760(void)
{
    s32 line;
    void *block;
    u8 *slot;

    if (Func_080770c0(0x839) != 0) {
        return;
    }

    if (Func_080770c0(0x82f) != 0) {
        Func_0808a018();
        Func_0808a138(11, 2);
        Func_0808a170(0xe8b);
        Func_0808a180(11, 0);
        Func_0808a020();
        return;
    }

    Func_0808a018();
    Func_0808a0a8(11);
    Func_0808a138(11, 1);

    line = 0xe85;
    Func_0808a170(line);
    Func_0808a188(11, 0, 20);
    Func_0808a1e8(0, 0x100, 30);
    Func_0808a210(0x620000, -1, 0x011b0000, 1);
    Func_0808a0d0(0, 0x5e, 0x125);
    Func_0808a1b8(0, 0xa000, 0);

    slot = Func_0808a080(0);
    if (slot != 0) {
        Func_0808a0f0(1, *(s32 *)(slot + 8), *(s32 *)(slot + 16));
    }

    Func_0808a0d0(1, 0x6e, 0x117);
    Func_0808a1b8(1, 0xa000, 40);
    Func_0808a138(11, 2);
    Func_0808a010(40);
    Func_0808a178(11, 0);

    if (Func_0808a070(0, 0) == 0) {
        Func_0808a138(11, 2);
        Func_0808a010(20);
        Func_0808a170(line + 2);
        Func_0808a180(11, 0);
        Func_080770c8(0x82f);
    } else {
        Func_0808a138(11, 2);
        Func_0808a010(20);
        Func_0808a170(line + 3);
        Func_0808a188(11, 0, 40);
        Func_0808a148(11, 0, 0);
        Func_0808a100(11, 1);
        Func_0808a128(11, 4, 40);
        Func_0808a100(11, 6);
        Func_0808a1e8(11, 0x101, 40);
        Func_0808a188(11, 0, 10);
        Func_0808a100(11, 1);
        Func_0808a010(10);
        Func_0808a110(11, 3);
        Func_0808a188(11, 0, 10);
        Func_0808a110(11, 3);

        block = (void *)0x02009ce0;
        Func_0808a168(0, 0x0001000b, block);
        Func_0808a168(1, 0x0001000b, block);
        Func_0808a0b0(11, (void *)0x02009bdc);
        Func_0808a0a8(0);
        Func_0808a0a8(1);
        Func_0808a1b8(0, 0x4000, 0);
        Func_0808a1b8(1, 0x4000, 60);
        Func_0808a1e8(0, 0x105, 0);
        Func_0808a1e8(1, 0x105, 120);
        Func_080770c8(0x839);
    }

    Func_0808a100(1, 2);
    slot = Func_0808a080(0);
    if (slot != 0) {
        Func_0808a0b8(1, *(s16 *)(slot + 10), *(s16 *)(slot + 18));
    }
    Func_0808a0e8(1);
    Func_0808a0f0(1, 0, 0);
    Func_0808a020();
}
