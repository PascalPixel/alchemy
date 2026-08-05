#include "types.h"

/*
 * resource_375 owner at 0x02000170, 2036 bytes (0x02000170-0x02000963).
 * The span decomposes as code 0x02000170-0x02000403, an interior literal pool
 * 0x02000404-0x02000413 branched over by the `b.n 0x0200041a` at 0x02000402,
 * code 0x02000414-0x02000735, a second interior pool
 * 0x02000738-0x02000757 branched over by the `b.n 0x0200078e` at 0x02000734,
 * code 0x02000758-0x02000949 and the trailing pool
 * 0x0200094c-0x02000963.  Both interior pools are data and are excluded from
 * the call-site scan outright rather than being decoded as instructions.
 *
 * Conventions.  An overlay `bl` stores the target's image offset minus two, not
 * a pc-relative displacement, so the disassembler's printed targets are wrong;
 * all 203 call sites were resolved with `tools/overlay_call_targets.ts` and
 * every one lands in the 8-byte-per-entry import veneer table starting at
 * 0x02001a54, so each is named by that entry's trailing main-image word.  (The
 * inventory advertises calls=189; that field counts distinct targets and is
 * known to be low.)  resource_375 is linked at 0x02008000, so the pool word
 * 0x0200a0ac is the in-image data block at file offset 0x20ac.  0x03001ebc is
 * the IWRAM workspace pointer the byte-exact sources of other overlays use.
 *
 * This owner is the third independent witness for the link base in this
 * overlay: the handler word 0x02008171 in the table at file offset 0x1efc is
 * this function's own address plus the Thumb bit, so this is the entry the
 * table installs.
 *
 * Return type: `pop {r5, r6} / pop {r0} / bx r0` pops the return address into
 * r0, so nothing is returned and the owner is void.
 *
 * Behaviour: the overlay's longest cutscene.  Flag 0x0801 gates it -- if it is
 * already set the owner returns immediately.  Note that unlike the siblings at
 * 0x02000be0 and 0x020012a0, this owner never sets its own gate flag; the
 * writer is elsewhere (0x02001760 sets 0x0839 and 0x082f, and 0x02000964 sets
 * 0x0801 through Func_080770c8), so the two owners must be read together.
 *
 * The scene spawns actors 1 and 5 on slot 0's position, plays the dialogue run
 * from 0x0fa6 with actor 13 as the speaker, and asks five yes/no questions
 * through Func_0808a070(0, 0).  Four of them choose between two line ids; the
 * fifth, at 0x02000718, is compared against 1 rather than 0 and selects between
 * a short reaction and a longer one that also bumps the u16 counter at +472 of
 * the workspace.  The scene ends by installing the descriptor block at
 * 0x0200a0ac, raising the workspace u32 at +448 to 0x202 and closing.
 *
 * Register reuse: r5 holds the actor x coordinate for the first half and the
 * dialogue base 0x0fbf from 0x020006a6 onward; the two lifetimes do not
 * overlap and are modelled as separate locals.
 *
 * Coordinate handling: the signed halfwords at +10 and +18 of the slot record
 * are the integer parts of its 16.16 words at +8 and +16; they are read with
 * `ldrsh` and shifted left 16 to rebuild a fixed-point pair, so the fractional
 * bits are dropped on purpose.  As in the sibling owners, this first
 * Func_0808a080(0) is dereferenced with no null test.
 *
 * Uncertainties: 0x0801, the 0x0fxx line ids, 0x0100..0x0105 and the small
 * first arguments (0, 1, 5, 13) are identifiers whose tables are not
 * reconstructed, so they stay numeric.  The two workspace fields are written
 * but never read here, so only their offsets and widths are asserted.
 */

extern u8 *Data_03001ebc;

/* Imports, old-style: arities are established per call site.  The queried
 * imports are typed for their return values. */
s32 Func_080770c0();
void Func_0808a018();
void Func_0808a090();
void Func_0808a170();
void Func_0808a138();
void Func_0808a0d0();
void Func_0808a100();
void Func_0808a150();
void Func_0808a188();
u8 *Func_0808a080();
void Func_0808a0f0();
void Func_0808a0c8();
void Func_0808a010();
void Func_0808a1b8();
void Func_0808a110();
void Func_0808a1e8();
void Func_0808a178();
s32 Func_0808a070();
void Func_0808a128();
void Func_0808a148();
void Func_0808a1f0();
void Func_080f9010();
void Func_08009178();
void Func_0808a368();
void Func_0808a370();
void Func_0808a248();
void Func_0808a020();

void Func_02000170(void)
{
    u8 *slot;
    s32 x;
    s32 z;
    s32 line;

    if (Func_080770c0(0x801) != 0) {
        return;
    }

    Func_0808a018();
    Func_0808a090(0, 0x10000, 0x8000);
    Func_0808a170(0xfa6);
    Func_0808a138(13, 1);
    Func_0808a0d0(0, 0xe8, 0x108);
    Func_0808a100(0, 0);
    Func_0808a150(0, 13, 20);
    Func_0808a138(13, 2);
    Func_0808a188(13, 0, 10);

    slot = Func_0808a080(0);
    x = *(s16 *)(slot + 10) << 16;
    z = *(s16 *)(slot + 18) << 16;

    Func_0808a0f0(5, x, z);
    Func_0808a0f0(1, x, z);
    Func_0808a090(5, 0x8000, 0x4000);
    Func_0808a090(1, 0x8000, 0x4000);
    Func_0808a0c8(5, 0xf8, 0x108);
    Func_0808a0d0(1, 0xd8, 0x108);
    Func_0808a100(0, 1);
    Func_0808a100(5, 1);
    Func_0808a100(1, 1);
    Func_0808a010(4);
    Func_0808a1b8(5, 0xb000, 0);
    Func_0808a1b8(1, 0xd000, 20);
    Func_0808a110(5, 4);
    Func_0808a010(10);
    Func_0808a188(5, 0, 20);
    Func_0808a138(13, 1);
    Func_0808a010(10);
    Func_0808a1b8(13, 0x3000, 10);
    Func_0808a188(13, 0, 10);
    Func_0808a1b8(1, 0x3000, 40);
    Func_0808a138(1, 2);
    Func_0808a188(1, 0, 10);
    Func_0808a138(13, 2);
    Func_0808a1b8(13, 0x5000, 10);
    Func_0808a100(13, 3);
    Func_0808a188(13, 0, 8);
    Func_0808a1b8(5, 0x3000, 20);
    Func_0808a188(5, 0, 10);
    Func_0808a1b8(13, 0x3000, 10);
    Func_0808a110(13, 3);
    Func_0808a010(10);
    Func_0808a188(13, 0, 6);
    Func_0808a1e8(0, 0x101, 0);
    Func_0808a1e8(1, 0x101, 0);
    Func_0808a1e8(5, 0x101, 60);
    Func_0808a1b8(1, 0xd000, 0);
    Func_0808a1b8(5, 0xb000, 20);
    Func_0808a138(5, 2);
    Func_0808a188(5, 0, 10);
    Func_0808a1e8(13, 0x102, 60);
    Func_0808a110(13, 4);
    Func_0808a188(13, 0, 10);
    Func_0808a1b8(1, 0, 0);
    Func_0808a1b8(5, 0x8000, 20);
    Func_0808a1e8(0, 0x100, 60);
    Func_0808a1b8(1, 0xd000, 0);
    Func_0808a1b8(5, 0xb000, 0);
    Func_0808a1b8(13, 0x5000, 20);
    Func_0808a1b8(13, 0x3000, 40);
    Func_0808a1b8(13, 0x5000, 40);
    Func_0808a1b8(13, 0x3000, 20);

    Func_0808a178(13, 0);
    if (Func_0808a070(0, 0) == 0) {
        Func_0808a170(0xfb0);
    } else {
        Func_0808a170(0xfb1);
    }

    Func_0808a010(20);
    Func_0808a110(13, 3);
    Func_0808a010(10);
    Func_0808a188(13, 0, 10);
    Func_0808a170(0xfb2);
    Func_0808a138(5, 2);
    Func_0808a1b8(5, 0x8000, 10);
    Func_0808a188(5, 0, 6);
    Func_0808a1e8(1, 0x103, 30);
    Func_0808a128(1, 4, 30);
    Func_0808a1b8(1, 0, 10);
    Func_0808a188(1, 0, 6);
    Func_0808a150(0, 1, 10);
    Func_0808a150(0, 5, 0);
    Func_0808a148(13, 1, 10);
    Func_0808a148(13, 5, 10);
    Func_0808a100(0, 3);
    Func_0808a100(1, 3);
    Func_0808a110(5, 3);
    Func_0808a010(10);
    Func_0808a100(5, 1);
    Func_0808a100(1, 1);
    Func_0808a100(0, 0);
    Func_0808a1b8(0, 0x4000, 0);
    Func_0808a1b8(1, 0x4000, 0);
    Func_0808a1b8(5, 0x4000, 16);
    Func_0808a1f0(13, 0x102);
    Func_0808a138(13, 3);
    Func_0808a010(10);
    Func_0808a188(13, 0, 6);
    Func_0808a1e8(0, 0x100, 0);
    Func_0808a1e8(1, 0x100, 0);
    Func_0808a1e8(5, 0x100, 40);
    Func_0808a110(13, 4);
    Func_0808a010(10);
    Func_0808a188(13, 0, 6);
    Func_0808a138(13, 1);
    Func_0808a188(13, 0, 6);
    Func_0808a1b8(5, 0xb000, 10);
    Func_0808a188(5, 0, 6);
    Func_0808a1b8(13, 0x3000, 10);
    Func_0808a110(13, 3);
    Func_0808a010(6);
    Func_0808a188(13, 0, 10);
    Func_0808a128(0, 2, 0);
    Func_0808a128(1, 2, 0);
    Func_0808a128(5, 2, 10);
    Func_0808a1b8(0, 0xc000, 0);
    Func_0808a1b8(1, 0xd000, 20);
    Func_0808a188(1, 0, 6);
    Func_0808a1b8(13, 0x5000, 10);
    Func_0808a110(13, 3);
    Func_0808a010(16);
    Func_0808a150(0, 5, 40);
    Func_0808a188(5, 0, 10);
    Func_0808a138(13, 2);
    Func_0808a1b8(13, 0x3000, 10);
    Func_0808a188(13, 0, 6);
    Func_0808a1b8(0, 0xc000, 0);
    Func_0808a1b8(5, 0xb000, 30);
    Func_0808a1e8(0, 0x105, 0);
    Func_0808a1e8(1, 0x105, 0);
    Func_0808a1e8(5, 0x105, 80);
    Func_0808a110(13, 4);

    Func_0808a178(13, 0);
    if (Func_0808a070(0, 0) == 0) {
        Func_0808a170(0xfbd);
    } else {
        Func_0808a170(0xfbe);
    }

    Func_0808a188(13, 0, 20);

    Func_0808a170(line);
    line = 0xfbf;
    Func_0808a1b8(1, 0, 10);
    Func_0808a138(1, 2);

    Func_0808a178(1, 0);
    if (Func_0808a070(0, 0) == 0) {
        Func_0808a170(line + 1);
    } else {
        Func_0808a170(line + 2);
    }

    Func_0808a188(1, 0, 6);
    Func_0808a170(0xfc2);
    Func_0808a1b8(5, 0x8000, 10);
    Func_0808a138(5, 1);
    Func_0808a178(5, 0);
    Func_0808a010(4);

    /* This test is against 1, not against 0 as the four above are. */
    if (Func_0808a070(0, 0) == 1) {
        Func_0808a128(5, 2, 20);
        Func_0808a188(5, 0, 10);
    } else {
        Func_0808a100(0, 3);
        Func_0808a100(1, 3);
        Func_0808a110(5, 3);
        Func_0808a010(8);
        Func_0808a100(0, 0);
        *(u16 *)(Data_03001ebc + 472) = (u16)(*(u16 *)(Data_03001ebc + 472) + 1);
    }

    Func_0808a110(13, 3);
    Func_0808a010(10);
    Func_0808a188(13, 0, 10);
    Func_0808a1b8(0, 0xc000, 0);
    Func_0808a1b8(1, 0xd000, 0);
    Func_0808a1b8(5, 0xb000, 10);
    Func_0808a100(0, 3);
    Func_0808a100(1, 3);
    Func_0808a110(5, 3);
    Func_0808a010(10);
    Func_0808a100(0, 0);
    Func_0808a010(20);
    Func_0808a138(13, 2);
    Func_0808a178(13, 0);
    Func_0808a010(4);

    if (Func_0808a070(0, 0) == 0) {
        Func_0808a170(0xfc6);
    } else {
        Func_0808a170(0xfc9);
    }

    Func_0808a010(10);
    Func_0808a138(1, 2);
    Func_0808a1b8(1, 0, 10);
    Func_0808a188(1, 0, 6);
    Func_0808a1b8(5, 0x8000, 10);
    Func_0808a110(5, 4);
    Func_0808a188(5, 0, 6);
    Func_0808a138(1, 2);
    Func_0808a1b8(1, 0x8000, 10);
    Func_0808a1e8(1, 0x103, 30);
    Func_0808a188(1, 0, 10);
    Func_0808a1e8(13, 0x100, 40);
    Func_0808a128(13, 4, 40);
    Func_0808a1b8(0, 0xc000, 0);
    Func_0808a1b8(1, 0xc000, 0);
    Func_0808a1b8(5, 0xb000, 20);
    Func_0808a1b8(13, 0xb000, 10);

    Func_080f9010(158);
    Func_08009178((void *)0x0200a0ac, 0x2b, 8);
    Func_0808a090(13, 0x10000, 0x8000);
    Func_0808a0d0(13, 0xe8, 0xda);
    Func_0808a0f0(13, 0, 0);
    Func_0808a1e8(0, 0x101, 0);
    Func_0808a1e8(1, 0x101, 0);
    Func_0808a1e8(5, 0x101, 60);

    *(u32 *)(Data_03001ebc + 448) = 0x202;

    Func_0808a368();
    Func_0808a370();
    Func_0808a248(13);
    Func_0808a020();
}
