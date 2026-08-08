#include "types.h"

/*
 * Resource 3ae, owner at 0x02000e40 (1728 bytes).  TWO literal pools sit
 * INSIDE the body — 0x02001244..0x0200126a and 0x020014ec..0x020014fe.  The
 * first is branched over by the lone forward `b.n 0x0200126c` at 0x02001240,
 * which is what a control-flow pool walk finds and a straight read does not;
 * the second is the trailing pool after the epilogue at 0x020014ea.
 *
 * Role known in advance from the call graph: the in-image handler table holds
 * the odd word 0x02008e41 == Func_02000e40 + 1 as the payload of the entry
 * keyed 0x08ab0032, and the owner's very first action is
 * `Func_080770c8(0x8ab)` — it sets the same event flag its table key names, so
 * it is the one-shot scene for that trigger.  It takes no arguments.
 *
 * Prologue `push {r5,r6,lr}` + `mov r6,sl / mov r5,r8 / push {r5,r6}`;
 * epilogue `pop {r3,r5} / mov r8,r3 / mov sl,r5 / pop {r5,r6} / pop {r0} /
 * bx r0`.  The return address is popped into r0, so r0 does not survive and
 * the owner is void.  There is NO stack frame: every call's arguments fit in
 * r0-r2, which is what makes the body one flat script.
 *
 * Register caching: r8 = 12 and sl = 0 are set once and reused as the
 * object-flag bit mask and the zero byte; r5 = 0xfe and r6 = 1 are the
 * clear-bit-0 and set-bit-0 constants for field +0x5a.  Both r5 and r6 are
 * later CLOBBERED by the very `ands`/`orrs` that use them (`ands r5,r3` at
 * 0x0200133c, `orrs r6,r3` at 0x020013aa) and then either reassigned (r5 to
 * 0xc000 at 0x02001380) or never read again (r6).  The C below spells those as
 * plain `&= 0xfe` / `|= 1`, which is what the stores actually write.
 *
 * Call targets resolved with `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml --` (an overlay `bl`
 * stores target offset - 2; the disassembler's annotation is wrong).  Every
 * site is an import veneer; roughly 179 `bl`s against the inventory's
 * calls=162, and the inventory count is documented as systematically low.
 * Every `bl` in the 1728-byte span is transcribed below in source order, so
 * the set is complete.  Distinct callees: Func_080770c8, Func_0808a018,
 * Func_0808a460, Func_0808a170, Func_0808a080, Func_0808a210, Func_0808a090,
 * Func_0808a0d0, Func_0808a1b8, Func_0808a218, Func_0808a010, Func_0808a1e8,
 * Func_0808a180, Func_0808a128, Func_0808a138, Func_0808a110, Func_0808a580,
 * Func_0808a578, Func_080f9010, Func_0808a100, Func_0808a1f0, Func_0808a130,
 * Func_0808a098, Func_0808a020.
 *
 * Idioms shared with the overlay's other scene scripts (0x02000ba0 above all):
 * Func_0808a018 / Func_0808a020 bracket the scene, Func_0808a010(n) is the
 * beat delay, Func_0808a1b8(slot, angle, 0) sets a 16-bit facing, and
 * Func_0808a090(slot, x, y) takes the 16.16 pairs whose pool words
 * (0xcccc / 0x6666, 0x13333 / 0x9999, 0x26666 / 0x13333, 0x1cccc / 0xe666)
 * recur across this overlay unchanged.
 *
 * Repeated beats are NOT merged.  The script contains dozens of textually
 * identical calls — Func_0808a010(20), Func_0808a180(10, 0), the three
 * consecutive Func_080f9010(155)/Func_0808a010(10) pairs, the two identical
 * Func_0808a110(13, 3) beats — and each is a distinct script site with its own
 * timing.  Collapsing any of them would change the scene.
 *
 * Object record layout (from Func_0808a080's return):
 *   +0x23  u8   pose/state byte
 *   +0x50  ptr  sprite/attribute sub-record
 *   +0x59  u8   flag byte
 *   +0x5a  u8   bit 0 is toggled around every reposition (the "hold still
 *               movement latch) — cleared before Func_0808a090 +
 *               Func_0808a580 and set again after
 *   sub+9, sub+21  u8 flag bytes, |= 12
 *   sub+0x26 u8, sub+30 u16 (angle)
 * These are the same offsets Func_02000958 writes on slots 10 and 11, which
 * cross-validates the layout.
 *
 * Uncertainties: 0x8ab is read as an event-flag id and 0x23eb as a cue id from
 * their argument positions.  133/153/155/159 passed to Func_080f9010 sit where
 * Func_0200051c passes 113 and Func_020007dc passes 158, so they read as the
 * same id space (a sound or effect cue).  Func_0808a580's and Func_0808a578's
 * two trailing arguments are signed offsets (they are produced by `negs`), but
 * their units are not witnessed here.  Data_020096e4 is in-image data under
 * the proven 0x02008000 base (file offset 0x16e4), handed to Func_0808a098 in
 * the position that call takes a small integer at in Func_02000958 — the two
 * sites genuinely differ, which is why the declaration is old-style.
 */

/* Old-style declarations: overlay imports vary in arity between call sites. */
void Func_020023a4();
void Func_020023d0();
void Func_020024dc();
void Func_02002472();
u8 *Func_02002400();
void Func_020024f4();
void Func_0200244a();
void Func_02002476();
void Func_020024fa();
void Func_02002526();
void Func_0200243c();
void Func_02002520();
void Func_02002500();
void Func_02002456();
void Func_02002538();
void Func_02002532();
void Func_020024ec();
void Func_020024f6();
void Func_02002536();
void Func_0200248c();
void Func_0200251c();
void Func_0200249a();
void Func_02002552();
void Func_020024a8();
void Func_020024b6();
void Func_0200256e();
void Func_020024c4();
void Func_020025a6();
void Func_0200250c();
void Func_02002616();
void Func_02002568();
void Func_0200262a();
u8 *Func_02002528();
void Func_02002640();
void Func_02002656();
void Func_02002558();
void Func_02002662();
void Func_020025a8();
u8 *Func_0200256e_b();
void Func_0200263a();
void Func_020025e2();
void Func_020026a0();
void Func_020026ac();
void Func_0200257a();
void Func_020026b8();
void Func_02002586();
void Func_0200258c();
void Func_020025c6();
void Func_02002618();
void Func_020026de();
void Func_020026e2();
void Func_020025b8();
void Func_02002670();
void Func_020026aa();
void Func_02002610();
void Func_0200271c();
void Func_020026c0();
void Func_020025fe();
void Func_02002676();
void Func_0200260c();
void Func_020026c4();
void Func_0200261a();
void Func_020026ee();
void Func_0200262c();
void Func_020026bc();
void Func_020026f2();
void Func_02002648();
void Func_0200265a();
void Func_0200272e();
void Func_0200266c();
void Func_02002724();
void Func_0200267a();
void Func_0200274e();
void Func_0200268c();
void Func_02002770();
void Func_02002750();
void Func_020026a6();
void Func_02002736();
void Func_020026b4();
void Func_0200276c();
void Func_020026c2();
void Func_02002796();
void Func_020026d4();
void Func_020027a8();
void Func_020026e6();
void Func_0200279e();
void Func_020026f4();
void Func_020027c6();
void Func_020027d0();
void Func_0200270e();
void Func_020027d4();
void Func_0200275e();
void Func_02002868();
void Func_02002776();
void Func_02002882();
void Func_02002826();
void Func_02002816();
void Func_02002850();
u8 *Func_020027a6();
void Func_020027c2();
void Func_020028ce();
u8 *Func_020027cc();
void Func_020027b2();
void Func_0200286e();
void Func_020027ec();
void Func_020028a4();
void Func_020027fa();
void Func_02002872();
void Func_02002808();
void Func_020028c0();
u8 *Func_02002844();
void Func_02002860();
void Func_0200296c();
u8 *Func_0200286a();
void Func_0200291c();
void Func_0200285a();
void Func_020028d2();
void Func_020028a2();
void Func_020029ac();
void Func_020028fe();
void Func_020029c0();
void Func_020029ce();
void Func_02002918();
void Func_020028e0();
u8 *Func_020028d6();
void Func_02002938();
void Func_020029fc();
u8 *Func_020028fa();
void Func_02002a3a();
void Func_020029de();
u8 *Func_02002944();
void Func_02002a62();
void Func_02002930();
void Func_02002a14();
void Func_02002978();
void Func_02002a82();
void Func_02002a24();
void Func_02002962();
void Func_02002a1a();
void Func_02002970();
void Func_020029e8();
void Func_0200297e();
void Func_02002a36();
void Func_0200298c();
void Func_020029c6();
void Func_02002ad2();
void Func_020029a8();
void Func_02002a60();
void Func_020029b6();
void Func_02002a46();
void Func_020029c4();
void Func_020029ca();
void Func_02002a9c();
void Func_020029da();
void Func_02002a52();
void Func_020029f6();
void Func_02002b46();
void Func_02002b50();
void Func_02002af4();
void Func_02002a32();
void Func_02002a70();
void Func_02002b7c();
void Func_02002a66();

                     

                     

                     

                     

                     

                     

                     

                     

extern u8 Data_020096e4[];

void Func_02000e40(void)
{
    u8 *object;
    u8 *sub;

    Func_020023a4(0x8ab);
    Func_020023d0();
    Func_020024dc();
    Func_02002472(0x23eb);

    object = Func_02002400(11);
    object[0x23] = 0;
    sub = *(u8 **)(object + 0x50);
    sub = *(u8 **)(object + 0x50);
    sub[9] |= 12;
    sub[21] |= 12;

    Func_020024f4(232 << 16, -1, 304 << 16, 1);
    Func_0200244a(0, 0x10000, 0x8000);
    Func_02002476(0, 216, 272);
    Func_020024fa(0, 0x2000, 0);
    Func_02002526();

    Func_0200243c(20);
    Func_02002520(13, 258, 50);
    Func_02002500(13, 0);
    Func_02002456(10);

    Func_02002538(10, 0x107, 50);
    Func_02002532(10, 0, 0);
    Func_020024ec(10, 4, 13);
    Func_020024f6(10, 4, 30);
    Func_02002536(10, 0);
    Func_0200248c(10);

    Func_0200251c(11, 2);
    Func_0200249a(20);
    Func_02002552(11, 0);
    Func_020024a8(10);

    Func_02002520(13, 4);
    Func_020024b6(20);
    Func_0200256e(13, 0);
    Func_020024c4(10);

    Func_020025a6(10, 0x103, 55);
    Func_0200250c(10, 0x20000, 0x10000);
    Func_02002616(10, 16, 0);
    Func_02002568(10, 7, 0);
    Func_0200262a(10, 24, 0);

    object = Func_02002528(10);
    object[0x5a] &= 0xfe;

    Func_02002640(10, -16, 0);
    Func_02002656(153);

    Func_02002558(13, 0x26666, 0x13333);
    Func_02002662(13, 16, 0);
    Func_02002538(10);
    Func_020025a8(10, 1);

    object = Func_0200256e_b(10);
    object[0x5a] |= 1;

    Func_0200263a(13, 258);
    Func_020025e2(13, 2);

    Func_020026a0(155);
    Func_0200256e(10);
    Func_020026ac(155);
    Func_0200257a(10);
    Func_020026b8(155);
    Func_02002586(10);
    Func_0200258c(20);

    Func_020025c6(13, 0x6666, 0x3333);
    Func_02002618(13, 6, 0);
    Func_020026de(159);
    Func_020026e2(13, -8, 0);
    Func_020025b8(20);
    Func_02002670(10, 0);
    Func_020025c6(10);

    Func_020026aa(13, 258, 70);
    Func_02002610(16, 0x10000, 0x8000);
    Func_0200271c(16, -8, 0);
    Func_020026c0(16, 0x5000, 0);
    Func_020025fe(30);
    Func_02002676(16, 4);
    Func_0200260c(20);
    Func_020026c4(16, 0);
    Func_0200261a(10);

    Func_020026ee(10, 0xe000, 0);
    Func_0200262c(35);
    Func_020026bc(10, 2);
    Func_0200263a(20);
    Func_020026f2(10, 0);
    Func_02002648(10);

    Func_0200271c(16, 0x2000, 0);
    Func_0200265a(55);
    Func_0200272e(16, 0x5000, 0);
    Func_0200266c(30);
    Func_02002724(16, 0);
    Func_0200267a(10);

    Func_0200274e(11, 0xe000, 0);
    Func_0200268c(20);
    Func_02002770(11, 258, 50);
    Func_02002750(11, 0);
    Func_020026a6(10);

    Func_02002736(13, 2);
    Func_020026b4(20);
    Func_0200276c(13, 0);
    Func_020026c2(10);

    Func_02002796(13, 0xa000, 0);
    Func_020026d4(60);
    Func_020027a8(13, 0x8000, 0);
    Func_020026e6(30);
    Func_0200279e(13, 0);
    Func_020026f4(10);

    Func_020027c6(10, 0, 0);
    Func_020027d0(11, 0, 0);
    Func_0200270e(20);
    Func_0200279e(10, 2);
    Func_0200271c(20);
    Func_020027d4(10, 0);

    Func_0200275e(10, 0x13333, 0x9999);
    Func_02002868(10, 8, 0);

    Func_02002776(16, 0x20000, 0x10000);
    Func_02002882(16, -8, 16);
    Func_02002826(16, 0x8000, 0);
    Func_02002816(16, 0);
    Func_0200276c(10);

    Func_02002850(10, 258, 50);

    object = Func_020027a6(10);
    object[0x5a] &= 0xfe;
    Func_020027c2(10, 0xcccc, 0x6666);
    Func_020028ce(10, -8, 0);
    object = Func_020027cc(10);
    object[0x5a] |= 1;

    Func_020027b2(20);
    Func_0200286e(10, 2);          /* reached by the `b.n` over the inner pool */
    Func_020027ec(20);
    Func_020028a4(10, 0);
    Func_020027fa(10);

    Func_02002872(16, 4);
    Func_02002808(20);
    Func_020028c0(16, 0);
    Func_02002816(10);

    object = Func_02002844(10);
    object[0x5a] &= 0xfe;
    Func_02002860(10, 0xcccc, 0x6666);
    Func_0200296c(10, -16, 0);
    object = Func_0200286a(10);
    object[0x5a] |= 1;

    Func_0200291c(10, 0, 0);
    Func_0200285a(20);
    Func_020028d2(10, 4);
    Func_02002868(20);

    Func_020028a2(10, 0x1cccc, 0xe666);
    Func_020029ac(10, 8, 0);
    Func_020028fe(10, 6, 0);
    Func_020029c0(10, 24, 0);
    Func_020029ce(133);

    Func_02002918(16, 6, 0);
    Func_020028e0(16, Data_020096e4);

    object = Func_020028d6(10);
    object[0x5a] &= 0xfe;

    Func_02002938(10, 6, 0);
    Func_020029fc(10, -12, 4);

    object = Func_020028fa(10);
    object[0x59] = 0;
    object[0x23] = 2;
    sub = *(u8 **)(object + 0x50);
    sub[9] |= 12;
    sub = *(u8 **)(object + 0x50);
    sub[0x26] = 0;
    sub = *(u8 **)(object + 0x50);
    *(u16 *)(sub + 30) = 0xc000;

    Func_02002a3a(10, -12, 4);
    Func_020029de(10, 0x4000, 0);

    object = Func_02002944(10);
    object[0x5a] |= 1;

    Func_02002a62(159);
    Func_02002930(20);

    Func_02002a14(11, 258, 50);
    Func_02002978(11, 0x18000, 0xc000);
    Func_02002a82(11, 24, 0);
    Func_02002a24(11, 0xc000, 0);
    Func_02002962(10);
    Func_02002a1a(11, 0);
    Func_02002970(10);

    Func_020029e8(16, 4);
    Func_0200297e(20);
    Func_02002a36(16, 0);
    Func_0200298c(20);

    Func_020029c6(16, 0xcccc, 0x6666);
    Func_02002ad2(16, -8, 0);
    Func_020029a8(20);
    Func_02002a60(16, 0);
    Func_020029b6(10);

    Func_02002a46(10, 2);
    Func_020029c4(20);
    Func_020029ca(10);
    Func_02002a9c(16, 0, 0);
    Func_020029da(40);

    Func_02002a52(13, 3);
    Func_020029e8(10);
    Func_02002a60(13, 3);
    Func_020029f6(20);
    Func_020029fc(10);

    Func_02002a3a(16, 0x10000, 0x8000);
    Func_02002b46(16, 24, -24);
    Func_02002b50(16, 8, 0);
    Func_02002af4(16, 0xe000, 0);
    Func_02002a32(20);

    Func_02002a70(13, 0x10000, 0x8000);
    Func_02002b7c(13, 0, -8);
    Func_02002a52(10);

    Func_02002a66();
}
