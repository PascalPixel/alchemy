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
 * all 166 call sites were resolved with `tools/lib/overlay_call_targets.ts` and
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
s32 Func_0200264a();
void Func_02002674();
void Func_020027ca();
void Func_02002668();
void Func_02002730();
void Func_02002752();
void Func_020026bc();
void Func_02002780();
void Func_02002706();
void Func_02002792();
u8 *Func_020026e0();
void Func_0200273e();
void Func_02002748();
void Func_02002712();
void Func_0200271c();
void Func_02002768();
void Func_02002788();
void Func_02002790();
void Func_02002798();
void Func_0200280c();
void Func_02002816();
void Func_02002828();
void Func_02002832();
void Func_0200283c();
void Func_02002840();
void Func_0200284c();
void Func_02002858();
void Func_02002884();
void Func_0200289c();
void Func_020028a8();
void Func_020027a6();
void Func_02002904();
void Func_0200282c();
void Func_0200288e();
void Func_020028a2();
void Func_0200285a();
void Func_020028b4();
void Func_020028d0();
void Func_02002890();
void Func_020028d2();
void Func_0200288a();
void Func_020028e4();
void Func_020028f6();
void Func_0200291a();
void Func_02002848();
void Func_0200292a();
void Func_0200293c();
void Func_0200293e();
void Func_0200294a();
void Func_02002954();
void Func_020028e0();
void Func_0200296a();
void Func_02002912();
void Func_020028a0();
void Func_020028fe();
void Func_02002918();
void Func_020029ac();
void Func_020029b8();
void Func_020029c4();
void Func_020029fa();
void Func_02002958();
void Func_0200296e();
void Func_0200298e();
void Func_02002996();
void Func_02002a0a();
void Func_02002a16();
void Func_020029d6();
void Func_02002a18();
void Func_02002a3c();
void Func_020029f4();
void Func_02002a36();
void Func_020029ee();
s32 Func_0200299e();
void Func_02002a42();
void Func_02002a72();
void Func_02002a94();
void Func_02002a64();
void Func_02002a8c();
void Func_02002aae();
void Func_02002a90();
void Func_02002ad4();
void Func_02002ae0();
void Func_02002ada();
void Func_02002aaa();
void Func_02002a20();
void Func_02002aa0();
void Func_02002afa();
void Func_02002aca();
void Func_02002aba();
void Func_02002b14();
void Func_02002ae4();
void Func_02002b38();
void Func_02002b20();
s32 Func_02002a88();
void Func_02002b2a();
void Func_02002b32();
void Func_02002b64();
void Func_02002b70();
void Func_02002b30();
void Func_02002b72();
void Func_02002b4c();
void Func_02002b56();
void Func_02002ba8();
void Func_02002b48();
void Func_02002b86();
void Func_02002bcc();
void Func_02002bd8();
void Func_02002be4();
void Func_02002b94();
void Func_02002b9c();
void Func_02002bac();
void Func_02002b22();
void Func_02002bba();
void Func_02002c0e();
void Func_02002c1a();
void Func_02002c14();
void Func_02002c36();
void Func_02002c30();
void Func_02002c4c();
void Func_02002bf4();
void Func_02002c4e();
void Func_02002c1e();
void Func_02002c78();
void Func_02002c84();
void Func_02002be2();
void Func_02002bf0();
u8 *Func_02002bee();
u8 *Func_02002c02();
void Func_02002c4a();
void Func_02002c60();
u8 *Func_02002c34();
u8 *Func_02002c46();
void Func_02002ca8();
void Func_02002cb0();
void Func_02002c46_b();
void Func_02002cde();
void Func_02002d20();
void Func_02002d3a();
void Func_02002cda();
void Func_02002cea();
void Func_02002dd0();
void Func_02002cf0();
u8 *Func_02002ca6();
void Func_02002ce0();
void Func_02002cfe();
void Func_02002d10();
u8 *Func_02002cd6();
void Func_02002d2e();
void Func_02002d40();
void Func_02002d8e();
void Func_02002d9a();
void Func_02002e46();
void Func_02002d0a();

                     

                     

                     

                     

                     

                     

                     

                     

                     

                     

void Func_02000be0(void)
{
    s32 line;
    s32 second_line;
    u8 *slot;
    u8 *actor;
    s32 x;
    s32 z;

    s32 permuted_156;
    if (Func_0200264a(0x808) != 0) {
        return;
    }

    Func_02002674();
    Func_020027ca(17);
    Func_02002668(0x808);

    Func_02002730(line);
    line = 0xf85;
    Func_02002752(14, 0, 10);

    Func_020026bc(0, 0x13333, 0x9999);
    Func_02002780(0, 0x100, 30);
    Func_02002706(0, 0x188, 0x148);
    Func_02002792(0, 0x4000, 10);

    slot = Func_020026e0(0);
    x = *(s16 *)(slot + 10) << 16;
    z = *(s16 *)(slot + 18) << 16;

    Func_0200273e(5, x, z);
    Func_02002748(1, x, z);
    Func_02002712(5, 0x13333, 0x9999);
    Func_0200271c(1, 0x13333, 0x9999);
    Func_02002752(5, 0x178, 0x148);
    Func_02002768(1, 0x198, 0x148);
    Func_02002788(0, 0);
    Func_02002790(5, 0);
    Func_02002798(1, 0);
    Func_0200280c(1, 0x8000, 0);
    Func_02002816(5, 0, 20);
    Func_02002828(0, 0x101, 0);
    Func_02002832(1, 0x101, 0);
    Func_0200283c(5, 0x101, 30);
    Func_02002840(0, 0x4000, 0);
    Func_0200284c(1, 0x4000, 0);
    Func_02002858(5, 0x4000, 0);
    Func_02002884(0x60000, 0xc000);
    Func_0200289c(0x00d70000, -1, 0x01590000, 1);
    Func_020028a8();
    Func_020027a6(20);
    Func_02002904(61);

    Func_0200282c(14, 4);
    Func_0200282c(14, 4);
    Func_0200288e(14, 0, 20);
    Func_020028a8(15, 0, 10);
    Func_020028a2(15, 0, 10);
    Func_0200285a(14, 3);
    Func_020028b4(14, 0, 10);
    Func_020028d0(15, 0x4000, 60);
    Func_02002890(15, 1);
    Func_020028d2(15, 0, 20);
    Func_0200288a(14, 3);
    Func_020028e4(14, 0, 10);
    Func_0200289c(15, 4);
    Func_020028f6(15, 0, 6);
    Func_0200291a(14, 0x100, 0);
    Func_020028d2(14, 2);
    Func_02002848(20);
    Func_0200292a(15, 0, 10);
    Func_0200293c(15, 0x101, 40);
    Func_0200293e(14, 0, 60);
    Func_0200294a(14, 0x8000, 40);
    Func_02002954(14, 0, 40);
    Func_020028e0(14, 0xe8, 0x168);
    Func_0200296a(14, 0, 10);
    Func_02002912(15, 3);
    Func_020028a0(10);
    Func_020028fe(14, 0x188, 0x168);
    Func_02002912(15, 0xd8, 0x168);
    Func_02002918(15, 0x178, 0x168);
    Func_020029ac(0, 0x4000, 0);
    Func_020029b8(1, 0x4000, 0);
    Func_020029c4(5, 0x4000, 0);
    Func_020029fa(0x01890000, -1, 0x01530000, 1);
    Func_02002958(14, 0x188, 0x168);
    Func_0200296e(15, 0x178, 0x168);
    Func_0200298e(14, 0);
    Func_02002996(15, 0);
    Func_02002a0a(14, 0xd000, 0);
    Func_02002a16(15, 0xd000, 30);
    Func_020029d6(14, 2);
    Func_02002a18(14, 0, 10);
    Func_02002a3c(1, 0x102, 60);
    Func_020029f4(1, 1);
    Func_02002a36(1, 0, 10);
    Func_020029ee(15, 4);

    Func_02002a36(0x100f, 0);
    if (Func_0200299e(0, 0) == 0) {
        Func_02002a42(line + 10);
    } else {
        Func_02002a72(line + 11);
    }

    Func_02002a94(0x100f, 0, 10);
    Func_02002a64(1, 2);

    second_line = 0xf91;
    Func_02002a8c(second_line);
    Func_02002aae(1, 0, 20);
    Func_02002a90(14, 15, 40);
    Func_02002ad4(14, 0xd000, 0);
    Func_02002ae0(15, 0xd000, 20);
    Func_02002ada(14, 0, 60);
    Func_02002aaa(15, 1);
    Func_02002a20(10);
    Func_02002aa0(15, 3);
    Func_02002afa(0x100f, 0, 10);
    Func_02002aca(5, 2);
    Func_02002aba(5, 3);
    Func_02002b14(0x1005, 0, 20);
    Func_02002ae4(14, 2);
    Func_02002b38(14, 0xa000, 20);

    Func_02002b20(14, 0);
    if (Func_02002a88(0, 0) == 0) {
        Func_02002b2a(second_line + 5);
    } else {
        Func_02002b32(second_line + 6);
    }

    Func_02002b64(5, 0, 0);
    Func_02002b70(1, 0x8000, 20);
    Func_02002b30(14, 2);
    Func_02002b72(14, 0, 10);
    Func_02002b4c(14, 1, 30);
    Func_02002b56(14, 5, 30);
    Func_02002ba8(14, 0x105, 80);
    Func_02002b48(14, 4);
    Func_02002b86(0xf98);
    Func_02002ba8(14, 0, 6);
    Func_02002bcc(0, 0x100, 0);
    Func_02002bd8(1, 0x100, 0);
    Func_02002be4(5, 0x100, 0);
    Func_02002b94(1, 1);
    Func_02002b9c(5, 1);
    Func_02002bac(0, 1);
    Func_02002b22(40);
    Func_02002bba(5, 2);
    Func_02002c0e(1, 0x4000, 0);
    Func_02002c1a(5, 0x4000, 10);
    Func_02002c14(0x1005, 0, 10);
    Func_02002be4(15, 2);
    Func_02002c36(15, 0, 10);
    Func_02002c30(0x100f, 0, 10);
    Func_02002c4c(14, 0x8000, 20);
    Func_02002bf4(14, 4);
    Func_02002c4e(14, 0, 10);
    Func_02002c1e(15, 2);
    Func_02002b94(10);
    Func_02002c78(14, 0xb000, 0);
    Func_02002c84(15, 0xd000, 20);
    Func_02002be2(15, 0x8000, 0x4000);
    Func_02002bf0(14, 0x8000, 0x4000);

    actor = Func_02002bee(14);
    permuted_156 = (u8)(*(u8 *)(actor + 0x5a) & 0xfe);
    actor = Func_02002c02(15);
    *(u8 *)(actor + 0x5a) = permuted_156;
    *(u8 *)(actor + 0x5a) = (u8)(*(u8 *)(actor + 0x5a) & 0xfe);

    Func_02002c4a(14, 0x188, 0x178);
    Func_02002c60(15, 0x178, 0x178);
    Func_02002c0e(6);

    actor = Func_02002c34(14);
    *(u8 *)(actor + 0x5a) = (u8)(*(u8 *)(actor + 0x5a) | 1);
    actor = Func_02002c46(15);
    *(u8 *)(actor + 0x5a) = (u8)(*(u8 *)(actor + 0x5a) | 1);

    Func_02002ca8(14, 0);
    Func_02002cb0(15, 0);
    Func_02002c46_b(20);
    Func_02002cde(1, 2);
    Func_02002d20(1, 0, 10);
    Func_02002d3a(0, 1, 20);
    Func_02002cda(0, 3);
    Func_02002cea(1, 3);
    Func_02002dd0(17);

    Func_02002cf0(1, 2);
    slot = Func_02002ca6(0);
    if (slot != 0) {
        Func_02002ce0(1, *(s16 *)(slot + 10), *(s16 *)(slot + 18));
    }
    Func_02002cfe(1);
    Func_02002d10(1, 0, 0);

    Func_02002d20(5, 2);
    slot = Func_02002cd6(0);
    if (slot != 0) {
        Func_02002d10(5, *(s16 *)(slot + 10), *(s16 *)(slot + 18));
    }
    Func_02002d2e(5);
    Func_02002d40(5, 0, 0);

    Func_02002d8e(14, 0x10000, (void *)0x02009ce0);
    Func_02002d9a(15, 0x10000, (void *)0x02009ce0);
    Func_02002e46();
    Func_02002d0a();
}
