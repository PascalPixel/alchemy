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
 * all 203 call sites were resolved with `tools/lib/overlay_call_targets.ts` and
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
s32 Func_02001bda();
void Func_02001c04();
void Func_02001c32();
void Func_02001cc0();
void Func_02001ca8();
void Func_02001c7c();
void Func_02001c9c();
void Func_02001cd6();
void Func_02001cce();
void Func_02001d10();
u8 *Func_02001c6e();
void Func_02001ccc();
void Func_02001ca4();
void Func_02001cb2();
void Func_02001ce6();
void Func_02001cfa();
void Func_02001d1a();
void Func_02001d22();
void Func_02001d2a();
void Func_02001da4();
void Func_02001db0();
void Func_02001d58();
void Func_02001db8();
void Func_02001d88();
void Func_02001cfe();
void Func_02001de2();
void Func_02001ddc();
void Func_02001df8();
void Func_02001dfa();
void Func_02001dca();
void Func_02001e1e();
void Func_02001dbe();
void Func_02001e20();
void Func_02001e3c();
void Func_02001e36();
void Func_02001e52();
void Func_02001e5a();
void Func_02001e7c();
void Func_02001e86();
void Func_02001e90();
void Func_02001e94();
void Func_02001ea0();
void Func_02001e60();
void Func_02001ea2();
void Func_02001ec6();
void Func_02001e66();
void Func_02001ec0();
void Func_02001eda();
void Func_02001ee6();
void Func_02001efa();
void Func_02001efe();
void Func_02001f0a();
void Func_02001f16();
void Func_02001f22();
void Func_02001f2e();
void Func_02001f3a();
s32 Func_02001e8a();
void Func_02001f2c();
void Func_02001f44();
void Func_02001e9a();
void Func_02001f1a();
void Func_02001ea8();
void Func_02001f7a();
void Func_02001f68();
void Func_02001f50();
void Func_02001fa4();
void Func_02001f9e();
void Func_02001fc0();
void Func_02001f6a();
void Func_02001fcc();
void Func_02001fc6();
void Func_02001fa8();
void Func_02001fb2();
void Func_02001fb4();
void Func_02001fbe();
void Func_02001fa6();
void Func_02001fb6();
void Func_02001fbc();
void Func_02001fc4();
void Func_02002040();
void Func_0200204c();
void Func_02002058();
void Func_02002072();
void Func_02002022();
void Func_02001f98();
void Func_0200206a();
void Func_0200208e();
void Func_0200209a();
void Func_020020a6();
void Func_02002046();
void Func_02001fd4();
void Func_02002076();
void Func_020020b8();
void Func_020020d4();
void Func_020020ce();
void Func_020020ea();
void Func_02002092();
void Func_02002020();
void Func_020020f2();
void Func_020020b4();
void Func_020020be();
void Func_020020c8();
void Func_0200212c();
void Func_02002138();
void Func_02002132();
void Func_0200214e();
void Func_020020f6();
void Func_02002084();
void Func_0200212e();
void Func_02002160();
void Func_02002130();
void Func_02002184();
void Func_0200217e();
void Func_0200219a();
void Func_020021a6();
void Func_020021b8();
void Func_020021c2();
void Func_020021cc();
void Func_0200216c();
void Func_020021b4();
s32 Func_0200211c();
void Func_020021be();
void Func_020021c6();
void Func_020021e8();
void Func_020021d8();
void Func_0200220a();
void Func_020021ca();
void Func_020021fa();
s32 Func_02002162();
void Func_02002204();
void Func_0200220c();
void Func_0200222e();
void Func_0200221c();
void Func_02002250();
void Func_02002210();
void Func_02002240();
void Func_0200218e();
s32 Func_020021ae();
void Func_02002224();
void Func_02002276();
void Func_0200224a();
void Func_02002252();
void Func_02002262();
void Func_020021f0();
void Func_02002268();
void Func_02002288();
void Func_02002216();
void Func_020022e8();
void Func_02002304();
void Func_02002310();
void Func_0200231c();
void Func_020022bc();
void Func_020022c4();
void Func_020022d4();
void Func_020022da();
void Func_02002270();
void Func_02002308();
void Func_02002338();
void Func_02002286();
s32 Func_020022a6();
void Func_02002348();
void Func_02002350();
void Func_020022a6_b();
void Func_0200233e();
void Func_02002390();
void Func_0200238a();
void Func_020023a6();
void Func_0200234e();
void Func_020023a8();
void Func_02002378();
void Func_020023cc();
void Func_020023de();
void Func_020023d0();
void Func_020023f4();
void Func_0200239e();
void Func_02002402();
void Func_0200240e();
void Func_0200241a();
void Func_02002426();
void Func_020024ac();
void Func_02002336();
void Func_02002394();
void Func_020023ce();
void Func_020023e8();
void Func_0200246a();
void Func_02002474();
void Func_0200247e();
void Func_020024f0();
void Func_020024fc();
void Func_020024ca();

                     

                     

                     

                     

                     

                    

                     

                     

                     

void Func_02000170(void)
{
    u8 *slot;
    s32 x;
    s32 z;
    s32 line;

    if (Func_02001bda(0x801) != 0) {
        return;
    }

    Func_02001c04();
    Func_02001c32(0, 0x10000, 0x8000);
    Func_02001cc0(0xfa6);
    Func_02001ca8(13, 1);
    Func_02001c7c(0, 0xe8, 0x108);
    Func_02001c9c(0, 0);
    Func_02001cd6(0, 13, 20);
    Func_02001cce(13, 2);
    Func_02001d10(13, 0, 10);

    slot = Func_02001c6e(0);
    x = *(s16 *)(slot + 10) << 16;
    z = *(s16 *)(slot + 18) << 16;

    Func_02001ccc(5, x, z);
    Func_02001cd6(1, x, z);
    Func_02001ca4(5, 0x8000, 0x4000);
    Func_02001cb2(1, 0x8000, 0x4000);
    Func_02001ce6(5, 0xf8, 0x108);
    Func_02001cfa(1, 0xd8, 0x108);
    Func_02001d1a(0, 1);
    Func_02001d22(5, 1);
    Func_02001d2a(1, 1);
    Func_02001cc0(4);
    Func_02001da4(5, 0xb000, 0);
    Func_02001db0(1, 0xd000, 20);
    Func_02001d58(5, 4);
    Func_02001ce6(10);
    Func_02001db8(5, 0, 20);
    Func_02001d88(13, 1);
    Func_02001cfe(10);
    Func_02001de2(13, 0x3000, 10);
    Func_02001ddc(13, 0, 10);
    Func_02001df8(1, 0x3000, 40);
    Func_02001db8(1, 2);
    Func_02001dfa(1, 0, 10);
    Func_02001dca(13, 2);
    Func_02001e1e(13, 0x5000, 10);
    Func_02001dbe(13, 3);
    Func_02001e20(13, 0, 8);
    Func_02001e3c(5, 0x3000, 20);
    Func_02001e36(5, 0, 10);
    Func_02001e52(13, 0x3000, 10);
    Func_02001dfa(13, 3);
    Func_02001d88(10);
    Func_02001e5a(13, 0, 6);
    Func_02001e7c(0, 0x101, 0);
    Func_02001e86(1, 0x101, 0);
    Func_02001e90(5, 0x101, 60);
    Func_02001e94(1, 0xd000, 0);
    Func_02001ea0(5, 0xb000, 20);
    Func_02001e60(5, 2);
    Func_02001ea2(5, 0, 10);
    Func_02001ec6(13, 0x102, 60);
    Func_02001e66(13, 4);
    Func_02001ec0(13, 0, 10);
    Func_02001eda(1, 0, 0);
    Func_02001ee6(5, 0x8000, 20);
    Func_02001efa(0, 0x100, 60);
    Func_02001efe(1, 0xd000, 0);
    Func_02001f0a(5, 0xb000, 0);
    Func_02001f16(13, 0x5000, 20);
    Func_02001f22(13, 0x3000, 40);
    Func_02001f2e(13, 0x5000, 40);
    Func_02001f3a(13, 0x3000, 20);

    Func_02001f22(13, 0);
    if (Func_02001e8a(0, 0) == 0) {
        Func_02001f2c(0xfb0);
    } else {
        Func_02001f44(0xfb1);
    }

    Func_02001e9a(20);
    Func_02001f1a(13, 3);
    Func_02001ea8(10);
    Func_02001f7a(13, 0, 10);
    Func_02001f68(0xfb2);
    Func_02001f50(5, 2);
    Func_02001fa4(5, 0x8000, 10);
    Func_02001f9e(5, 0, 6);
    Func_02001fc0(1, 0x103, 30);
    Func_02001f6a(1, 4, 30);
    Func_02001fcc(1, 0, 10);
    Func_02001fc6(1, 0, 6);
    Func_02001fa8(0, 1, 10);
    Func_02001fb2(0, 5, 0);
    Func_02001fb4(13, 1, 10);
    Func_02001fbe(13, 5, 10);
    Func_02001f9e(0, 3);
    Func_02001fa6(1, 3);
    Func_02001fb6(5, 3);
    Func_02001f44(10);
    Func_02001fbc(5, 1);
    Func_02001fc4(1, 1);
    Func_02001fcc(0, 0);
    Func_02002040(0, 0x4000, 0);
    Func_0200204c(1, 0x4000, 0);
    Func_02002058(5, 0x4000, 16);
    Func_02002072(13, 0x102);
    Func_02002022(13, 3);
    Func_02001f98(10);
    Func_0200206a(13, 0, 6);
    Func_0200208e(0, 0x100, 0);
    Func_0200209a(1, 0x100, 0);
    Func_020020a6(5, 0x100, 40);
    Func_02002046(13, 4);
    Func_02001fd4(10);
    Func_020020a6(13, 0, 6);
    Func_02002076(13, 1);
    Func_020020b8(13, 0, 6);
    Func_020020d4(5, 0xb000, 10);
    Func_020020ce(5, 0, 6);
    Func_020020ea(13, 0x3000, 10);
    Func_02002092(13, 3);
    Func_02002020(6);
    Func_020020f2(13, 0, 10);
    Func_020020b4(0, 2, 0);
    Func_020020be(1, 2, 0);
    Func_020020c8(5, 2, 10);
    Func_0200212c(0, 0xc000, 0);
    Func_02002138(1, 0xd000, 20);
    Func_02002132(1, 0, 6);
    Func_0200214e(13, 0x5000, 10);
    Func_020020f6(13, 3);
    Func_02002084(16);
    Func_0200212e(0, 5, 40);
    Func_02002160(5, 0, 10);
    Func_02002130(13, 2);
    Func_02002184(13, 0x3000, 10);
    Func_0200217e(13, 0, 6);
    Func_0200219a(0, 0xc000, 0);
    Func_020021a6(5, 0xb000, 30);
    Func_020021b8(0, 0x105, 0);
    Func_020021c2(1, 0x105, 0);
    Func_020021cc(5, 0x105, 80);
    Func_0200216c(13, 4);

    Func_020021b4(13, 0);
    if (Func_0200211c(0, 0) == 0) {
        Func_020021be(0xfbd);
    } else {
        Func_020021c6(0xfbe);
    }

    Func_020021e8(13, 0, 20);

    Func_020021d8(line);
    line = 0xfbf;
    Func_0200220a(1, 0, 10);
    Func_020021ca(1, 2);

    Func_020021fa(1, 0);
    if (Func_02002162(0, 0) == 0) {
        Func_02002204(line + 1);
    } else {
        Func_0200220c(line + 2);
    }

    Func_0200222e(1, 0, 6);
    Func_0200221c(0xfc2);
    Func_02002250(5, 0x8000, 10);
    Func_02002210(5, 1);
    Func_02002240(5, 0);
    Func_0200218e(4);

    /* This test is against 1, not against 0 as the four above are. */
    if (Func_020021ae(0, 0) == 1) {
        Func_02002224(5, 2, 20);
        Func_02002276(5, 0, 10);
    } else {
        Func_0200224a(0, 3);
        Func_02002252(1, 3);
        Func_02002262(5, 3);
        Func_020021f0(8);
        Func_02002268(0, 0);
        *(u16 *)(Data_03001ebc + 472) = (u16)(*(u16 *)(Data_03001ebc + 472) + 1);
    }

    Func_02002288(13, 3);
    Func_02002216(10);
    Func_020022e8(13, 0, 10);
    Func_02002304(0, 0xc000, 0);
    Func_02002310(1, 0xd000, 0);
    Func_0200231c(5, 0xb000, 10);
    Func_020022bc(0, 3);
    Func_020022c4(1, 3);
    Func_020022d4(5, 3);
    Func_02002262(10);
    Func_020022da(0, 0);
    Func_02002270(20);
    Func_02002308(13, 2);
    Func_02002338(13, 0);
    Func_02002286(4);

    if (Func_020022a6(0, 0) == 0) {
        Func_02002348(0xfc6);
    } else {
        Func_02002350(0xfc9);
    }

    Func_020022a6_b(10);
    Func_0200233e(1, 2);
    Func_02002390(1, 0, 10);
    Func_0200238a(1, 0, 6);
    Func_020023a6(5, 0x8000, 10);
    Func_0200234e(5, 4);
    Func_020023a8(5, 0, 6);
    Func_02002378(1, 2);
    Func_020023cc(1, 0x8000, 10);
    Func_020023de(1, 0x103, 30);
    Func_020023d0(1, 0, 10);
    Func_020023f4(13, 0x100, 40);
    Func_0200239e(13, 4, 40);
    Func_02002402(0, 0xc000, 0);
    Func_0200240e(1, 0xc000, 0);
    Func_0200241a(5, 0xb000, 20);
    Func_02002426(13, 0xb000, 10);

    Func_020024ac(158);
    Func_02002336((void *)0x0200a0ac, 0x2b, 8);
    Func_02002394(13, 0x10000, 0x8000);
    Func_020023ce(13, 0xe8, 0xda);
    Func_020023e8(13, 0, 0);
    Func_0200246a(0, 0x101, 0);
    Func_02002474(1, 0x101, 0);
    Func_0200247e(5, 0x101, 60);

    *(u32 *)(Data_03001ebc + 448) = 0x202;

    Func_020024f0();
    Func_020024fc();
    Func_020024ca(13);
    Func_020023ce();
}
