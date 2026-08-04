typedef unsigned short u16;
typedef unsigned char u8;

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
 * Call targets resolved with `tools/overlay_call_targets.ts` (an overlay `bl`
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
void Func_080770c8();
void Func_0808a018();
void Func_0808a020();
void Func_0808a010();
void Func_0808a460();
void Func_0808a170();
u8 *Func_0808a080();
void Func_0808a090();
void Func_0808a098();
void Func_0808a0d0();
void Func_0808a100();
void Func_0808a110();
void Func_0808a128();
void Func_0808a130();
void Func_0808a138();
void Func_0808a180();
void Func_0808a1b8();
void Func_0808a1e8();
void Func_0808a1f0();
void Func_0808a210();
void Func_0808a218();
void Func_0808a578();
void Func_0808a580();
void Func_080f9010();

extern u8 Data_020096e4[];

void Func_02000e40(void)
{
    u8 *object;
    u8 *sub;

    Func_080770c8(0x8ab);
    Func_0808a018();
    Func_0808a460();
    Func_0808a170(0x23eb);

    object = Func_0808a080(11);
    object[0x23] = 0;
    sub = *(u8 **)(object + 0x50);
    sub[9] |= 12;
    sub = *(u8 **)(object + 0x50);
    sub[21] |= 12;

    Func_0808a210(232 << 16, -1, 304 << 16, 1);
    Func_0808a090(0, 0x10000, 0x8000);
    Func_0808a0d0(0, 216, 272);
    Func_0808a1b8(0, 0x2000, 0);
    Func_0808a218();

    Func_0808a010(20);
    Func_0808a1e8(13, 258, 50);
    Func_0808a180(13, 0);
    Func_0808a010(10);

    Func_0808a1e8(10, 0x107, 50);
    Func_0808a1b8(10, 0, 0);
    Func_0808a128(10, 4, 13);
    Func_0808a128(10, 4, 30);
    Func_0808a180(10, 0);
    Func_0808a010(10);

    Func_0808a138(11, 2);
    Func_0808a010(20);
    Func_0808a180(11, 0);
    Func_0808a010(10);

    Func_0808a110(13, 4);
    Func_0808a010(20);
    Func_0808a180(13, 0);
    Func_0808a010(10);

    Func_0808a1e8(10, 0x103, 55);
    Func_0808a090(10, 0x20000, 0x10000);
    Func_0808a580(10, 16, 0);
    Func_0808a128(10, 7, 0);
    Func_0808a580(10, 24, 0);

    object = Func_0808a080(10);
    object[0x5a] &= 0xfe;

    Func_0808a578(10, -16, 0);
    Func_080f9010(153);

    Func_0808a090(13, 0x26666, 0x13333);
    Func_0808a580(13, 16, 0);
    Func_0808a010(10);
    Func_0808a100(10, 1);

    object = Func_0808a080(10);
    object[0x5a] |= 1;

    Func_0808a1f0(13, 258);
    Func_0808a130(13, 2);

    Func_080f9010(155);
    Func_0808a010(10);
    Func_080f9010(155);
    Func_0808a010(10);
    Func_080f9010(155);
    Func_0808a010(10);
    Func_0808a010(20);

    Func_0808a090(13, 0x6666, 0x3333);
    Func_0808a128(13, 6, 0);
    Func_080f9010(159);
    Func_0808a580(13, -8, 0);
    Func_0808a010(20);
    Func_0808a180(10, 0);
    Func_0808a010(10);

    Func_0808a1e8(13, 258, 70);
    Func_0808a090(16, 0x10000, 0x8000);
    Func_0808a580(16, -8, 0);
    Func_0808a1b8(16, 0x5000, 0);
    Func_0808a010(30);
    Func_0808a110(16, 4);
    Func_0808a010(20);
    Func_0808a180(16, 0);
    Func_0808a010(10);

    Func_0808a1b8(10, 0xe000, 0);
    Func_0808a010(35);
    Func_0808a138(10, 2);
    Func_0808a010(20);
    Func_0808a180(10, 0);
    Func_0808a010(10);

    Func_0808a1b8(16, 0x2000, 0);
    Func_0808a010(55);
    Func_0808a1b8(16, 0x5000, 0);
    Func_0808a010(30);
    Func_0808a180(16, 0);
    Func_0808a010(10);

    Func_0808a1b8(11, 0xe000, 0);
    Func_0808a010(20);
    Func_0808a1e8(11, 258, 50);
    Func_0808a180(11, 0);
    Func_0808a010(10);

    Func_0808a138(13, 2);
    Func_0808a010(20);
    Func_0808a180(13, 0);
    Func_0808a010(10);

    Func_0808a1b8(13, 0xa000, 0);
    Func_0808a010(60);
    Func_0808a1b8(13, 0x8000, 0);
    Func_0808a010(30);
    Func_0808a180(13, 0);
    Func_0808a010(10);

    Func_0808a1b8(10, 0, 0);
    Func_0808a1b8(11, 0, 0);
    Func_0808a010(20);
    Func_0808a138(10, 2);
    Func_0808a010(20);
    Func_0808a180(10, 0);

    Func_0808a090(10, 0x13333, 0x9999);
    Func_0808a580(10, 8, 0);

    Func_0808a090(16, 0x20000, 0x10000);
    Func_0808a580(16, -8, 16);
    Func_0808a1b8(16, 0x8000, 0);
    Func_0808a180(16, 0);
    Func_0808a010(10);

    Func_0808a1e8(10, 258, 50);

    object = Func_0808a080(10);
    object[0x5a] &= 0xfe;
    Func_0808a090(10, 0xcccc, 0x6666);
    Func_0808a580(10, -8, 0);
    object = Func_0808a080(10);
    object[0x5a] |= 1;

    Func_0808a010(20);
    Func_0808a138(10, 2);          /* reached by the `b.n` over the inner pool */
    Func_0808a010(20);
    Func_0808a180(10, 0);
    Func_0808a010(10);

    Func_0808a110(16, 4);
    Func_0808a010(20);
    Func_0808a180(16, 0);
    Func_0808a010(10);

    object = Func_0808a080(10);
    object[0x5a] &= 0xfe;
    Func_0808a090(10, 0xcccc, 0x6666);
    Func_0808a580(10, -16, 0);
    object = Func_0808a080(10);
    object[0x5a] |= 1;

    Func_0808a1b8(10, 0, 0);
    Func_0808a010(20);
    Func_0808a110(10, 4);
    Func_0808a010(20);

    Func_0808a090(10, 0x1cccc, 0xe666);
    Func_0808a580(10, 8, 0);
    Func_0808a128(10, 6, 0);
    Func_0808a580(10, 24, 0);
    Func_080f9010(133);

    Func_0808a128(16, 6, 0);
    Func_0808a098(16, Data_020096e4);

    object = Func_0808a080(10);
    object[0x5a] &= 0xfe;

    Func_0808a128(10, 6, 0);
    Func_0808a580(10, -12, 4);

    object = Func_0808a080(10);
    object[0x59] = 0;
    object[0x23] = 2;
    sub = *(u8 **)(object + 0x50);
    sub[9] |= 12;
    sub = *(u8 **)(object + 0x50);
    sub[0x26] = 0;
    sub = *(u8 **)(object + 0x50);
    *(u16 *)(sub + 30) = 0xc000;

    Func_0808a580(10, -12, 4);
    Func_0808a1b8(10, 0x4000, 0);

    object = Func_0808a080(10);
    object[0x5a] |= 1;

    Func_080f9010(159);
    Func_0808a010(20);

    Func_0808a1e8(11, 258, 50);
    Func_0808a090(11, 0x18000, 0xc000);
    Func_0808a580(11, 24, 0);
    Func_0808a1b8(11, 0xc000, 0);
    Func_0808a010(10);
    Func_0808a180(11, 0);
    Func_0808a010(10);

    Func_0808a110(16, 4);
    Func_0808a010(20);
    Func_0808a180(16, 0);
    Func_0808a010(20);

    Func_0808a090(16, 0xcccc, 0x6666);
    Func_0808a580(16, -8, 0);
    Func_0808a010(20);
    Func_0808a180(16, 0);
    Func_0808a010(10);

    Func_0808a138(10, 2);
    Func_0808a010(20);
    Func_0808a010(10);
    Func_0808a1b8(16, 0, 0);
    Func_0808a010(40);

    Func_0808a110(13, 3);
    Func_0808a010(10);
    Func_0808a110(13, 3);
    Func_0808a010(20);
    Func_0808a010(10);

    Func_0808a090(16, 0x10000, 0x8000);
    Func_0808a580(16, 24, -24);
    Func_0808a580(16, 8, 0);
    Func_0808a1b8(16, 0xe000, 0);
    Func_0808a010(20);

    Func_0808a090(13, 0x10000, 0x8000);
    Func_0808a580(13, 0, -8);
    Func_0808a010(10);

    Func_0808a020();
}
