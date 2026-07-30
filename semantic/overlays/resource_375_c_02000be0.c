#include "types.h"

/*
 * resource_375 owner at 0x02000be0, 1728 bytes.  The span decomposes as
 * code 0x02000be0-0x02000f19, an interior literal pool
 * 0x02000f1c-0x02000f3f branched over by the `b.n 0x02000f48` at 0x02000f18,
 * code 0x02000f40-0x02001285, an alignment halfword at 0x02001286 and the
 * trailing pool 0x02001288-0x0200129f.  The interior pool is data: the site
 * scan below excludes both pool ranges outright rather than trusting an
 * instruction decode inside them.
 *
 * Conventions.  An overlay `bl` stores the target's image offset minus two, not
 * a pc-relative displacement, so the disassembler's printed targets are wrong;
 * all 166 call sites were resolved with `tools/overlay_call_targets.ts` and
 * every one lands in the 8-byte-per-entry import veneer table starting at
 * 0x02001a54, so each is named by that entry's trailing main-image word.  (The
 * inventory advertises calls=152; that field counts distinct targets and is
 * known to be low.)  resource_375 is linked at 0x02008000, so the pool word
 * 0x02009ce0 is the in-image data block at file offset 0x1ce0.
 *
 * Return type: `pop {r5, r6, r7} / pop {r0} / bx r0` pops the return address
 * into r0, so nothing is returned and the owner is void.
 *
 * Behaviour: the overlay's long one-shot cutscene, guarded by flag 0x0808 --
 * the flag is tested on entry and set immediately afterwards, so the scene runs
 * exactly once.  It plays dialogue from the base id 0x0f85, spawns actors 1 and
 * 5 on slot 0's position, runs two camera moves through Func_0808a210, drives
 * actors 14 and 15 through a long walk-and-turn sequence, asks two yes/no
 * questions through Func_0808a070(0, 0) whose answers pick between consecutive
 * line ids, briefly clears and restores bit 0 of each of actor 14's and 15's
 * byte at +0x5a, and finally returns both actors to slot 0 and closes.
 *
 * Register reuse: r7 holds the first dialogue base (0x0f85) for the whole first
 * half; r5 holds successively the x coordinate, the second dialogue base
 * (0x0f91), the byte mask 0xfe, the bit 1 and the data address 0x02009ce0.
 * Each is modelled as its own local, and no two lifetimes overlap.
 *
 * Coordinate handling: the signed halfwords at +10 and +18 of a slot record are
 * the integer parts of its 16.16 words at +8 and +16 (the double view recorded
 * for this project); at 0x02000c46 they are read with `ldrsh` and shifted left
 * 16 to rebuild a fixed-point pair, so the fractional bits are dropped on
 * purpose.  As in the sibling owner at 0x020012a0, the first Func_0808a080(0)
 * is dereferenced with no null test while the two at the end are both tested.
 * That asymmetry is in the machine code.
 *
 * Uncertainties: 0x0808, 0x0f85, 0x0f91, 0x0f98, 0x0100/0x0101/0x0102/0x0105,
 * 0x1005, 0x100f, 0x13333/0x9999 and the packed camera words 0x01590000,
 * 0x01890000 and 0x01530000 are identifiers or packed parameters whose tables
 * are not reconstructed, so they stay numeric.  The two Func_0808a210 second
 * arguments are the sign-extended -1 produced by `movs r1,#1 / negs r1,r1`.
 * The meaning of bit 0 of the actor byte at +0x5a is not established.
 */

/* Imports, old-style: arities are established per call site.  The queried
 * imports are typed for their return values. */
s32 Func_080770c0();
void Func_080770c8();
void Func_0808a018();
void Func_080f9010();
void Func_0808a170();
void Func_0808a188();
void Func_0808a090();
void Func_0808a1e8();
void Func_0808a0d0();
void Func_0808a1b8();
u8 *Func_0808a080();
void Func_0808a0f0();
void Func_0808a0c8();
void Func_0808a100();
void Func_0808a208();
void Func_0808a210();
void Func_0808a218();
void Func_0808a010();
void Func_0808a110();
void Func_0808a138();
void Func_0808a178();
s32 Func_0808a070();
void Func_0808a150();
void Func_0808a148();
void Func_0808a130();
void Func_0808a0b8();
void Func_0808a0e8();
void Func_0808a168();
void Func_0808a4f0();
void Func_0808a020();

void Func_02000be0(void)
{
    s32 line;
    s32 second_line;
    u8 *slot;
    u8 *actor;
    s32 x;
    s32 z;

    if (Func_080770c0(0x808) != 0) {
        return;
    }

    Func_0808a018();
    Func_080f9010(17);
    Func_080770c8(0x808);

    line = 0xf85;
    Func_0808a170(line);
    Func_0808a188(14, 0, 10);

    Func_0808a090(0, 0x13333, 0x9999);
    Func_0808a1e8(0, 0x100, 30);
    Func_0808a0d0(0, 0x188, 0x148);
    Func_0808a1b8(0, 0x4000, 10);

    slot = Func_0808a080(0);
    x = *(s16 *)(slot + 10) << 16;
    z = *(s16 *)(slot + 18) << 16;

    Func_0808a0f0(5, x, z);
    Func_0808a0f0(1, x, z);
    Func_0808a090(5, 0x13333, 0x9999);
    Func_0808a090(1, 0x13333, 0x9999);
    Func_0808a0c8(5, 0x178, 0x148);
    Func_0808a0d0(1, 0x198, 0x148);
    Func_0808a100(0, 0);
    Func_0808a100(5, 0);
    Func_0808a100(1, 0);
    Func_0808a1b8(1, 0x8000, 0);
    Func_0808a1b8(5, 0, 20);
    Func_0808a1e8(0, 0x101, 0);
    Func_0808a1e8(1, 0x101, 0);
    Func_0808a1e8(5, 0x101, 30);
    Func_0808a1b8(0, 0x4000, 0);
    Func_0808a1b8(1, 0x4000, 0);
    Func_0808a1b8(5, 0x4000, 0);
    Func_0808a208(0x60000, 0xc000);
    Func_0808a210(0x00d70000, -1, 0x01590000, 1);
    Func_0808a218();
    Func_0808a010(20);
    Func_080f9010(61);

    Func_0808a110(14, 4);
    Func_0808a100(14, 4);
    Func_0808a188(14, 0, 20);
    Func_0808a1b8(15, 0, 10);
    Func_0808a188(15, 0, 10);
    Func_0808a110(14, 3);
    Func_0808a188(14, 0, 10);
    Func_0808a1b8(15, 0x4000, 60);
    Func_0808a138(15, 1);
    Func_0808a188(15, 0, 20);
    Func_0808a110(14, 3);
    Func_0808a188(14, 0, 10);
    Func_0808a110(15, 4);
    Func_0808a188(15, 0, 6);
    Func_0808a1e8(14, 0x100, 0);
    Func_0808a138(14, 2);
    Func_0808a010(20);
    Func_0808a1b8(15, 0, 10);
    Func_0808a1e8(15, 0x101, 40);
    Func_0808a1b8(14, 0, 60);
    Func_0808a1b8(14, 0x8000, 40);
    Func_0808a1b8(14, 0, 40);
    Func_0808a0d0(14, 0xe8, 0x168);
    Func_0808a1b8(14, 0, 10);
    Func_0808a110(15, 3);
    Func_0808a010(10);
    Func_0808a0c8(14, 0x188, 0x168);
    Func_0808a0d0(15, 0xd8, 0x168);
    Func_0808a0c8(15, 0x178, 0x168);
    Func_0808a1b8(0, 0x4000, 0);
    Func_0808a1b8(1, 0x4000, 0);
    Func_0808a1b8(5, 0x4000, 0);
    Func_0808a210(0x01890000, -1, 0x01530000, 1);
    Func_0808a0c8(14, 0x188, 0x168);
    Func_0808a0d0(15, 0x178, 0x168);
    Func_0808a100(14, 0);
    Func_0808a100(15, 0);
    Func_0808a1b8(14, 0xd000, 0);
    Func_0808a1b8(15, 0xd000, 30);
    Func_0808a138(14, 2);
    Func_0808a188(14, 0, 10);
    Func_0808a1e8(1, 0x102, 60);
    Func_0808a138(1, 1);
    Func_0808a188(1, 0, 10);
    Func_0808a110(15, 4);

    Func_0808a178(0x100f, 0);
    if (Func_0808a070(0, 0) == 0) {
        Func_0808a170(line + 10);
    } else {
        Func_0808a170(line + 11);
    }

    Func_0808a188(0x100f, 0, 10);
    Func_0808a138(1, 2);

    second_line = 0xf91;
    Func_0808a170(second_line);
    Func_0808a188(1, 0, 20);
    Func_0808a150(14, 15, 40);
    Func_0808a1b8(14, 0xd000, 0);
    Func_0808a1b8(15, 0xd000, 20);
    Func_0808a188(14, 0, 60);
    Func_0808a138(15, 1);
    Func_0808a010(10);
    Func_0808a110(15, 3);
    Func_0808a188(0x100f, 0, 10);
    Func_0808a138(5, 2);
    Func_0808a110(5, 3);
    Func_0808a188(0x1005, 0, 20);
    Func_0808a138(14, 2);
    Func_0808a1b8(14, 0xa000, 20);

    Func_0808a178(14, 0);
    if (Func_0808a070(0, 0) == 0) {
        Func_0808a170(second_line + 5);
    } else {
        Func_0808a170(second_line + 6);
    }

    Func_0808a1b8(5, 0, 0);
    Func_0808a1b8(1, 0x8000, 20);
    Func_0808a138(14, 2);
    Func_0808a188(14, 0, 10);
    Func_0808a148(14, 1, 30);
    Func_0808a148(14, 5, 30);
    Func_0808a1e8(14, 0x105, 80);
    Func_0808a110(14, 4);
    Func_0808a170(0xf98);
    Func_0808a188(14, 0, 6);
    Func_0808a1e8(0, 0x100, 0);
    Func_0808a1e8(1, 0x100, 0);
    Func_0808a1e8(5, 0x100, 0);
    Func_0808a130(1, 1);
    Func_0808a130(5, 1);
    Func_0808a138(0, 1);
    Func_0808a010(40);
    Func_0808a138(5, 2);
    Func_0808a1b8(1, 0x4000, 0);
    Func_0808a1b8(5, 0x4000, 10);
    Func_0808a188(0x1005, 0, 10);
    Func_0808a138(15, 2);
    Func_0808a1b8(15, 0, 10);
    Func_0808a188(0x100f, 0, 10);
    Func_0808a1b8(14, 0x8000, 20);
    Func_0808a110(14, 4);
    Func_0808a188(14, 0, 10);
    Func_0808a138(15, 2);
    Func_0808a010(10);
    Func_0808a1b8(14, 0xb000, 0);
    Func_0808a1b8(15, 0xd000, 20);
    Func_0808a090(15, 0x8000, 0x4000);
    Func_0808a090(14, 0x8000, 0x4000);

    actor = Func_0808a080(14);
    *(u8 *)(actor + 0x5a) = (u8)(*(u8 *)(actor + 0x5a) & 0xfe);
    actor = Func_0808a080(15);
    *(u8 *)(actor + 0x5a) = (u8)(*(u8 *)(actor + 0x5a) & 0xfe);

    Func_0808a0c8(14, 0x188, 0x178);
    Func_0808a0d0(15, 0x178, 0x178);
    Func_0808a010(6);

    actor = Func_0808a080(14);
    *(u8 *)(actor + 0x5a) = (u8)(*(u8 *)(actor + 0x5a) | 1);
    actor = Func_0808a080(15);
    *(u8 *)(actor + 0x5a) = (u8)(*(u8 *)(actor + 0x5a) | 1);

    Func_0808a100(14, 0);
    Func_0808a100(15, 0);
    Func_0808a010(20);
    Func_0808a138(1, 2);
    Func_0808a188(1, 0, 10);
    Func_0808a1b8(0, 1, 20);
    Func_0808a100(0, 3);
    Func_0808a110(1, 3);
    Func_080f9010(17);

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

    Func_0808a168(14, 0x10000, (void *)0x02009ce0);
    Func_0808a168(15, 0x10000, (void *)0x02009ce0);
    Func_0808a4f0();
    Func_0808a020();
}
