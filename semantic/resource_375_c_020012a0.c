#include "types.h"

/*
 * resource_375 owner at 0x020012a0, 620 bytes: code 0x020012a0-0x020014f1, an
 * alignment halfword at 0x020014f2 and the literal pool
 * 0x020014f4-0x0200150b.
 *
 * Conventions.  An overlay `bl` stores the target's image offset minus two, not
 * a pc-relative displacement, so the disassembler's printed targets are wrong;
 * all 63 call sites were resolved with `tools/lib/overlay_call_targets.ts` and
 * every one lands in the 8-byte-per-entry import veneer table starting at
 * 0x02001a54, so each is named by that entry's trailing main-image word.  (The
 * inventory advertises calls=56; that field counts distinct targets and is
 * known to be low.)  resource_375 is linked at 0x02008000; this owner's pool
 * holds only small constants, no in-image addresses.
 *
 * Return type: `pop {r5, r6} / pop {r0} / bx r0` pops the return address into
 * r0, so nothing is returned and the owner is void.
 *
 * Behaviour: a one-shot cutscene guarded by flag 0x0800 -- if the flag is
 * already set the owner does nothing, and the scene sets it on the way out.
 * Actors 1 and 5 are spawned at slot 0's position (captured once as the 16.16
 * pair r5/r6), actor 13 plays the line 0x0f9f with a long camera and animation
 * sequence, and both actors are then snapped back onto slot 0 and cleared.
 *
 * Coordinate handling: the signed halfwords at +10 and +18 of the slot record
 * are its 16.16 words at +8 and +16 seen as integer parts, the double view this
 * project has already recorded.  Here they are read with `ldrsh` and shifted
 * left 16 to rebuild the fixed-point pair, so the fractional bits are dropped
 * on purpose.
 *
 * Note the asymmetry, which is in the machine code and is not a transcription
 * slip: the first Func_0808a080(0) at 0x020012d2 is dereferenced with no null
 * test, while the two later ones at 0x0200148a and 0x020014ba are both tested.
 *
 * Uncertainties: 0x0800, 0x0f9f, 0x0101, 0xcccc/0x6666 and 0x3333/0x1999 are
 * identifiers or packed parameters whose tables are not reconstructed, so they
 * stay numeric.  Func_0808a210's second argument is the sign-extended -1
 * produced by `movs r1,#1 / negs r1,r1`.
 */

/* Imports, old-style: arities are established per call site.  The queried
 * imports are typed for their return values. */
s32 Func_080770c0();
void Func_080770c8();
void Func_0808a018();
void Func_0808a090();
void Func_0808a170();
void Func_0808a180();
u8 *Func_0808a080();
void Func_0808a0f0();
void Func_0808a0c8();
void Func_0808a0d0();
void Func_0808a100();
void Func_0808a1b8();
void Func_0808a210();
void Func_0808a218();
void Func_0808a138();
void Func_0808a010();
void Func_0808a188();
void Func_0808a128();
void Func_0808a1e8();
void Func_0808a110();
void Func_0808a200();
void Func_0808a0b8();
void Func_0808a0e8();
void Func_0808a020();

void Func_020012a0(void)
{
    u8 *slot;
    s32 x;
    s32 z;

    if (Func_080770c0(0x800) != 0) {
        return;
    }

    Func_0808a018();
    Func_0808a090(0, 0x10000, 0x8000);
    Func_0808a170(0xf9f);
    Func_0808a180(13, 0);

    slot = Func_0808a080(0);
    x = *(s16 *)(slot + 10) << 16;
    z = *(s16 *)(slot + 18) << 16;

    Func_0808a0f0(5, x, z);
    Func_0808a0f0(1, x, z);
    Func_0808a090(5, 0xcccc, 0x6666);
    Func_0808a090(1, 0xcccc, 0x6666);
    Func_0808a0c8(5, 0x128, 0x148);
    Func_0808a0d0(1, 0x118, 0x148);
    Func_0808a100(0, 0);
    Func_0808a100(5, 0);
    Func_0808a100(1, 0);
    Func_0808a1b8(5, 0xb000, 0);
    Func_0808a1b8(1, 0xb000, 0);
    Func_0808a1b8(0, 0xb000, 0);
    Func_0808a210(0x00e80000, -1, 0x00f00000, 1);
    Func_0808a218();

    Func_0808a1b8(13, 0x8000, 20);
    Func_0808a138(13, 2);
    Func_0808a180(13, 0);
    Func_0808a090(13, 0x3333, 0x1999);
    Func_0808a0d0(13, 0xd8, 0xe8);
    Func_0808a010(20);
    Func_0808a138(13, 1);
    Func_0808a010(10);
    Func_0808a188(13, 0, 40);
    Func_0808a128(13, 2, 10);
    Func_0808a1b8(13, 0, 10);
    Func_0808a188(13, 0, 10);
    Func_0808a0d0(13, 0xf8, 0xe8);
    Func_0808a010(40);
    Func_0808a138(13, 2);
    Func_0808a010(20);
    Func_0808a180(13, 0);
    Func_0808a1e8(13, 0x101, 60);
    Func_0808a138(13, 1);
    Func_0808a1b8(13, 0x8000, 20);
    Func_0808a188(13, 0, 2);
    Func_0808a0d0(13, 0xe8, 0xe8);
    Func_0808a010(2);
    Func_0808a1b8(13, 0x4000, 4);
    Func_0808a138(13, 2);
    Func_0808a010(4);
    Func_0808a188(13, 0, 2);
    Func_0808a110(13, 4);
    Func_0808a010(20);
    Func_0808a200(0, 1);
    Func_0808a218();

    Func_0808a100(1, 2);
    slot = Func_0808a080(0);
    if (slot != 0) {
        Func_0808a0b8(1, *(s16 *)(slot + 10), *(s16 *)(slot + 18));
    }
    Func_0808a0e8(1);
    Func_0808a0f0(1, 0, 0);

    Func_0808a100(5, 2);
    slot = Func_0808a080(0);
    if (slot != 0) {
        Func_0808a0b8(5, *(s16 *)(slot + 10), *(s16 *)(slot + 18));
    }
    Func_0808a0e8(5);
    Func_0808a0f0(5, 0, 0);

    Func_080770c8(0x800);
    Func_0808a020();
}
