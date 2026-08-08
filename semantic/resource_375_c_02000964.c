#include "types.h"

/*
 * resource_375 owner at 0x02000964, 580 bytes: code 0x02000964-0x02000b89, an
 * alignment halfword at 0x02000b8a and the literal pool
 * 0x02000b8c-0x02000ba7.
 *
 * Conventions.  An overlay `bl` stores the target's image offset minus two, not
 * a pc-relative displacement, so the disassembler's printed targets are wrong;
 * all 57 call sites below were resolved with `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml --`
 * and every one lands in the 8-byte-per-entry import veneer table that starts
 * at 0x02001a54, so each is named by that entry's trailing main-image word.
 * (The inventory row advertises calls=51; that field counts distinct targets
 * and is known to be low, so the site list is the authority here.)
 * resource_375 is linked at 0x02008000, so the pool word 0x0200a0ac is the
 * in-image data block at file offset 0x20ac.  0x03001ebc is the IWRAM
 * workspace pointer the byte-exact sources in other overlays already use.
 *
 * Return type: the epilogue `pop {r5, r6} / pop {r0} / bx r0` pops the return
 * address into r0, so nothing is returned and the owner is void.
 *
 * Behaviour: a scene setup routine.  It positions three actors (ids 1, 5 and
 * 13), sets each one's halfword at +6 to 0xc000 (a facing/heading field),
 * installs the descriptor block at 0x0200a0ac, raises the workspace u32 at
 * +448 to 0x202, plays a short camera and dialogue sequence, snaps actors 1, 5
 * and 13 onto the position of slot 0, then tears the scene down, lowers the
 * same workspace field to 0x100 and hands control on through Func_080770c8.
 *
 * The three `Func_0808a080(0)` blocks read the signed halfwords at +10 and +18
 * of the returned record.  Those are the integer parts of the 16.16 fixed-point
 * words at +8 and +16, the coordinate view this project has already recorded as
 * a shared idiom; they are read as s16 here because the instruction is `ldrsh`.
 *
 * Uncertainties: 0x0801, 0x0242 and 0x0fcc are identifiers whose tables are not
 * reconstructed, and the small first arguments (0, 1, 5, 13, 14, 15) are slot
 * or channel selectors, so all stay numeric.  The two workspace writes at +448
 * are not read anywhere in this owner, so only offset and width are asserted.
 */

extern u8 *Data_03001ebc;

/* Imports, old-style: arities are established per call site.  Func_0808a080
 * returns a record pointer and is tested, so it needs a return type. */
void Func_020023ec();
void Func_02002456();
void Func_02002464();
void Func_02002472();
u8 *Func_02002430();
u8 *Func_0200243c();
void Func_02002400();
void Func_02002572();
void Func_02002586();
void Func_02002444();
void Func_02002476();
void Func_020024c4();
void Func_020024be();
void Func_0200246c();
void Func_020024ec();
void Func_0200252a();
void Func_0200254c();
void Func_0200251c();
void Func_02002570();
void Func_0200256a();
void Func_02002584();
void Func_02002590();
void Func_02002530();
void Func_02002540();
void Func_020024d6();
void Func_020025b8();
void Func_020025c4();
void Func_02002564();
void Func_02002574();
void Func_02002536();
void Func_020025fa();
void Func_0200259a();
u8 *Func_02002550();
void Func_0200258a();
void Func_020025ba();
u8 *Func_02002570_b();
void Func_020025aa();
void Func_020025da();
u8 *Func_02002590_b();
void Func_020025ca();
void Func_020025e8();
void Func_02002604();
void Func_02002602();
void Func_02002614();
void Func_02002624();
void Func_0200262c();
void Func_02002634();
void Func_02002636();
void Func_02002640();
void Func_020025ce();
void Func_02002724();
void Func_020025ea();
void Func_0200260e();

                    

                     

                     

                     

                     

                     

void Func_02000964(void)
{
    u8 *actor;
    u8 *slot;

    Func_020023ec();
    Func_02002456(13, 0, 0);
    Func_02002464(1, 0x00d80000, 0x01080000);
    Func_02002472(5, 0x00f80000, 0x01080000);

    actor = Func_02002430(1);
    actor = Func_0200243c(5);
    *(u16 *)(actor + 6) = 0xc000;
    *(u16 *)(actor + 6) = 0xc000;

    Func_02002400((void *)0x0200a0ac, 0x2b, 8);
    *(u32 *)(Data_03001ebc + 448) = 0x202;

    Func_02002572();
    Func_02002586();
    Func_02002444(40);

    Func_02002476(13, 0xcccc, 0x6666);
    Func_020024c4(13, 0x00e60000, 0x00dc0000);
    Func_020024be(13, 0xe6, 0xe8);
    Func_0200246c(20);
    Func_020024ec(13, 3);
    Func_0200252a(0xfcc);
    Func_0200254c(13, 0, 10);
    Func_0200251c(13, 2);
    Func_02002570(13, 0x3000, 10);
    Func_0200256a(13, 0, 10);

    Func_02002584(1, 0, 0);
    Func_02002590(0, 0x8000, 10);
    Func_02002530(0, 3);
    Func_02002540(1, 3);
    Func_02002540(0, 0);

    Func_020024d6(20);

    Func_020025b8(0, 0, 0);
    Func_020025c4(5, 0x8000, 10);
    Func_02002564(5, 3);
    Func_02002574(0, 3);
    Func_02002574(0, 0);

    Func_02002536(1, 0xcccc, 0x6666);
    Func_02002540(5, 0xcccc, 0x6666);
    Func_020025fa(0, 0xc000, 0);

    Func_0200259a(1, 2);
    slot = Func_02002550(0);
    if (slot != 0) {
        Func_0200258a(1, *(s16 *)(slot + 10), *(s16 *)(slot + 18));
    }

    Func_020025ba(5, 2);
    slot = Func_02002570_b(0);
    if (slot != 0) {
        Func_020025aa(5, *(s16 *)(slot + 10), *(s16 *)(slot + 18));
    }

    Func_020025da(13, 2);
    slot = Func_02002590_b(0);
    if (slot != 0) {
        Func_020025ca(13, *(s16 *)(slot + 10), *(s16 *)(slot + 18));
    }

    Func_020025e8(1);
    Func_020025fa(1, 0, 0);
    Func_02002604(5, 0, 0);
    Func_02002602(13);
    Func_02002614(13, 0, 0);
    Func_02002624(1, 1);
    Func_0200262c(5, 1);
    Func_02002634(13, 1);
    Func_02002636(14, 0, 0);
    Func_02002640(15, 0, 0);

    Func_020025ce(0x801);
    *(u32 *)(Data_03001ebc + 448) = 0x100;
    Func_02002724(0x10000, 0);
    Func_020025ea(0x242);
    Func_0200260e();
}
