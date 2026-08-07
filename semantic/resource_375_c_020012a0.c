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
s32 Func_02002d0c();
void Func_02002d36();
void Func_02002d64();
void Func_02002df2();
void Func_02002e0a();
u8 *Func_02002d70();
void Func_02002dce();
void Func_02002dd8();
void Func_02002da2();
void Func_02002dac();
void Func_02002de2();
void Func_02002df8();
void Func_02002e18();
void Func_02002e20();
void Func_02002e28();
void Func_02002e9c();
void Func_02002ea8();
void Func_02002eb4();
void Func_02002eee();
void Func_02002efa();
void Func_02002ed6();
void Func_02002e96();
void Func_02002ece();
void Func_02002e40();
void Func_02002e7a();
void Func_02002ec0();
void Func_02002e36();
void Func_02002f08();
void Func_02002eca();
void Func_02002f2c();
void Func_02002f26();
void Func_02002e6e();
void Func_02002f06();
void Func_02002e7c();
void Func_02002f44();
void Func_02002f6e();
void Func_02002f7a();
void Func_02002f74();
void Func_02002f0e();
void Func_02002ebc();
void Func_02002fa0();
void Func_02002f60();
void Func_02002fa8();
void Func_02002fe6();
void Func_02003002();
void Func_02002f72();
u8 *Func_02002f28();
void Func_02002f62();
void Func_02002f80();
void Func_02002f92();
void Func_02002fa2();
u8 *Func_02002f58();
void Func_02002fb0();
void Func_02002fc2();
void Func_02002f52();
void Func_02002f76();

                     

                     

                     

                     

                     

                     

                     

                     

void Func_020012a0(void)
{
    u8 *slot;
    s32 x;
    s32 z;

    if (Func_02002d0c(0x800) != 0) {
        return;
    }

    Func_02002d36();
    Func_02002d64(0, 0x10000, 0x8000);
    Func_02002df2(0xf9f);
    Func_02002e0a(13, 0);

    slot = Func_02002d70(0);
    x = *(s16 *)(slot + 10) << 16;
    z = *(s16 *)(slot + 18) << 16;

    Func_02002dce(5, x, z);
    Func_02002dd8(1, x, z);
    Func_02002da2(5, 0xcccc, 0x6666);
    Func_02002dac(1, 0xcccc, 0x6666);
    Func_02002de2(5, 0x128, 0x148);
    Func_02002df8(1, 0x118, 0x148);
    Func_02002e18(0, 0);
    Func_02002e20(5, 0);
    Func_02002e28(1, 0);
    Func_02002e9c(5, 0xb000, 0);
    Func_02002ea8(1, 0xb000, 0);
    Func_02002eb4(0, 0xb000, 0);
    Func_02002eee(0x00e80000, -1, 0x00f00000, 1);
    Func_02002efa();

    Func_02002ed6(13, 0x8000, 20);
    Func_02002e96(13, 2);
    Func_02002ece(13, 0);
    Func_02002e40(13, 0x3333, 0x1999);
    Func_02002e7a(13, 0xd8, 0xe8);
    Func_02002e28(20);
    Func_02002ec0(13, 1);
    Func_02002e36(10);
    Func_02002f08(13, 0, 40);
    Func_02002eca(13, 2, 10);
    Func_02002f2c(13, 0, 10);
    Func_02002f26(13, 0, 10);
    Func_02002ec0(13, 0xf8, 0xe8);
    Func_02002e6e(40);
    Func_02002f06(13, 2);
    Func_02002e7c(20);
    Func_02002f44(13, 0);
    Func_02002f6e(13, 0x101, 60);
    Func_02002f26(13, 1);
    Func_02002f7a(13, 0x8000, 20);
    Func_02002f74(13, 0, 2);
    Func_02002f0e(13, 0xe8, 0xe8);
    Func_02002ebc(2);
    Func_02002fa0(13, 0x4000, 4);
    Func_02002f60(13, 2);
    Func_02002ed6(4);
    Func_02002fa8(13, 0, 2);
    Func_02002f60(13, 4);
    Func_02002eee(20);
    Func_02002fe6(0, 1);
    Func_02003002();

    Func_02002f72(1, 2);
    slot = Func_02002f28(0);
    if (slot != 0) {
        Func_02002f62(1, *(s16 *)(slot + 10), *(s16 *)(slot + 18));
    }
    Func_02002f80(1);
    Func_02002f92(1, 0, 0);

    Func_02002fa2(5, 2);
    slot = Func_02002f58(0);
    if (slot != 0) {
        Func_02002f92(5, *(s16 *)(slot + 10), *(s16 *)(slot + 18));
    }
    Func_02002fb0(5);
    Func_02002fc2(5, 0, 0);

    Func_02002f52(0x800);
    Func_02002f76();
}
