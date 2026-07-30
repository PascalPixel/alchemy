#include "types.h"

/*
 * resource_375 owner at 0x02000964, 580 bytes: code 0x02000964-0x02000b89, an
 * alignment halfword at 0x02000b8a and the literal pool
 * 0x02000b8c-0x02000ba7.
 *
 * Conventions.  An overlay `bl` stores the target's image offset minus two, not
 * a pc-relative displacement, so the disassembler's printed targets are wrong;
 * all 57 call sites below were resolved with `tools/overlay_call_targets.ts`
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
void Func_0808a018();
void Func_0808a0f0();
u8 *Func_0808a080();
void Func_08009178();
void Func_0808a360();
void Func_0808a370();
void Func_0808a010();
void Func_0808a090();
void Func_0808a0d0();
void Func_0808a110();
void Func_0808a170();
void Func_0808a188();
void Func_0808a138();
void Func_0808a1b8();
void Func_0808a100();
void Func_0808a0b8();
void Func_0808a0e8();
void Func_080770c8();
void Func_0808a338();
void Func_0808a020();

void Func_02000964(void)
{
    u8 *actor;
    u8 *slot;

    Func_0808a018();
    Func_0808a0f0(13, 0, 0);
    Func_0808a0f0(1, 0x00d80000, 0x01080000);
    Func_0808a0f0(5, 0x00f80000, 0x01080000);

    actor = Func_0808a080(1);
    *(u16 *)(actor + 6) = 0xc000;
    actor = Func_0808a080(5);
    *(u16 *)(actor + 6) = 0xc000;

    Func_08009178((void *)0x0200a0ac, 0x2b, 8);
    *(u32 *)(Data_03001ebc + 448) = 0x202;

    Func_0808a360();
    Func_0808a370();
    Func_0808a010(40);

    Func_0808a090(13, 0xcccc, 0x6666);
    Func_0808a0f0(13, 0x00e60000, 0x00dc0000);
    Func_0808a0d0(13, 0xe6, 0xe8);
    Func_0808a010(20);
    Func_0808a110(13, 3);
    Func_0808a170(0xfcc);
    Func_0808a188(13, 0, 10);
    Func_0808a138(13, 2);
    Func_0808a1b8(13, 0x3000, 10);
    Func_0808a188(13, 0, 10);

    Func_0808a1b8(1, 0, 0);
    Func_0808a1b8(0, 0x8000, 10);
    Func_0808a100(0, 3);
    Func_0808a110(1, 3);
    Func_0808a100(0, 0);

    Func_0808a010(20);

    Func_0808a1b8(0, 0, 0);
    Func_0808a1b8(5, 0x8000, 10);
    Func_0808a100(5, 3);
    Func_0808a110(0, 3);
    Func_0808a100(0, 0);

    Func_0808a090(1, 0xcccc, 0x6666);
    Func_0808a090(5, 0xcccc, 0x6666);
    Func_0808a1b8(0, 0xc000, 0);

    Func_0808a100(1, 2);
    slot = Func_0808a080(0);
    if (slot != 0) {
        Func_0808a0b8(1, *(s16 *)(slot + 10), *(s16 *)(slot + 18));
    }

    Func_0808a100(5, 2);
    slot = Func_0808a080(0);
    if (slot != 0) {
        Func_0808a0b8(5, *(s16 *)(slot + 10), *(s16 *)(slot + 18));
    }

    Func_0808a100(13, 2);
    slot = Func_0808a080(0);
    if (slot != 0) {
        Func_0808a0b8(13, *(s16 *)(slot + 10), *(s16 *)(slot + 18));
    }

    Func_0808a0e8(1);
    Func_0808a0f0(1, 0, 0);
    Func_0808a0f0(5, 0, 0);
    Func_0808a0e8(13);
    Func_0808a0f0(13, 0, 0);
    Func_0808a100(1, 1);
    Func_0808a100(5, 1);
    Func_0808a100(13, 1);
    Func_0808a0f0(14, 0, 0);
    Func_0808a0f0(15, 0, 0);

    Func_080770c8(0x801);
    *(u32 *)(Data_03001ebc + 448) = 0x100;
    Func_0808a338(0x10000, 0);
    Func_080770c8(0x242);
    Func_0808a020();
}
