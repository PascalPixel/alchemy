#include "types.h"

/*
 * resource_38d owner at 0x020019b0, 2060 bytes, exactly the advertised row.
 * Ranges, settled by a control-flow walk from the prologue (follow every
 * branch; whatever is never reached as an instruction is pool):
 *   code            0x020019b0-0x02001caf
 *   INTERIOR pool   0x02001cb0-0x02001ceb   (15 words)
 *   code            0x02001cec-0x02001e27
 *   INTERIOR pool   0x02001e28-0x02001e47   (8 words)
 *   code            0x02001e48-0x02002181   (0x02002182 is alignment)
 *   trailing pool   0x02002184-0x020021bb   (14 words)
 * Both interior pools are hopped by a lone unconditional `b.n` (0x02001cae ->
 * 0x02001cf2 and 0x02001e26 -> 0x02001e4e) and each is immediately followed by
 * the other arm of the same conditional, whose branch target is the first
 * instruction after the pool.  Register state carries straight across them.
 *
 * `overlay_show.ts` lists 0x02002004 as a fourteenth "pool word referenced";
 * that is the documented false entry produced by the real word 0x00004ccc at
 * 0x02001cd0 decoding as `ldr r4, [pc, #816]`.  It is not part of this owner.
 *
 * The scene body the router at 0x020006e0 selects when the scene selector is
 * 29, story flag 0x84e is clear, 0x85e is clear and 0x845 is set.  It is the
 * overlay's longest scene: it stages the cast, publishes two workspace words,
 * runs a long sequence of pose / turn / wait / dialogue beats across slots 0,
 * 1, 2, 3, 18 and 19, asks two yes-no prompts whose answers only steer a beat
 * counter, and finishes by setting story flags 0x322, 0x84f and 0x84a.
 *
 * BEAT COUNTER.  Nine sites bump the u16 at workspace + 472 by one and one
 * site bumps it by four.  This is the documented skip-beat idiom: an r5 flag
 * is set to 1, cleared to 0 on the arm that bumps immediately, and the *other*
 * arm bumps after the following wait, so exactly one bump happens per gated
 * beat either way.  The counter is written and never read here.  Workspace is
 * the pointer at 0x03001ebc (IWRAM); the owner keeps its *address* in r6 for
 * the whole body and reloads the pointer at each use, which is what the macro
 * below reproduces.  472 is built as 236 << 1 every time.
 *
 * The two workspace stores at 0x02001a0e/0x02001a16 are the documented "one
 * register is both value and displacement" shape: r3 is 224 << 1 = 448 as the
 * displacement, `adds r3, #65` makes 513 (the stored word), `subs r3, #57`
 * makes 456 (the next displacement) and 16 is a fresh value.  The owners at
 * 0x020021bc and 0x020005f4 perform the same pair.
 *
 * COORDINATE VIEWS.  0x02001b1e reads the record's s32 words at +8 and +16 and
 * passes them to Func_0808a0f0 (16.16); 0x0200204e reads the s16 halfwords at
 * +10 and +18 and passes them to Func_0808a0b8.  Those are the integer parts
 * of the same two 16.16 words, the double view the byte-exact overlay sources
 * already model.  The `movs r3, #10 / ldrsh r1, [r0, r3]` offset register is
 * NOT a fourth argument to the following call - it is the load offset.
 *
 * Call accounting: 186 `bl` sites over 37 distinct targets, all resolved with
 * `bun tools/overlay_call_targets.ts resource_38d 19b0 --json`, which reports
 * exactly `sites=186 distinct_targets=37 veneer=178 prologue=8`.  The eight
 * prologue sites are four calls each to the byte-exact intra-overlay owners
 * Func_02001958 and Func_02001984 (`assets/code/resource_38d_c_02001958.c` and
 * `_02001984.c`), which take no arguments.  No `call_via` slot: r3 is written
 * only as an immediate argument, a load offset or workspace scratch, never
 * with an IWRAM code address.  Every site appears below exactly once and the
 * per-target multisets were compared target by target.  The inventory row says
 * calls=178, the documented lower bound.
 *
 * The epilogue is `pop {r5, r6} / pop {r0} / bx r0`, so r0 holds the popped
 * return address and the owner is void.  The prologue is `push {r5, r6, lr}`
 * and the first branch is taken with no argument register written, so the
 * owner takes no arguments.
 *
 * Link base 0x02008000 (witnessed by the installed handler words 0x020085f5 =
 * Func_020005f4 + 1 and 0x02008569 = Func_02000568 + 1).  Two pool words are
 * in-image addresses under it: 0x0200a5d4 (file offset 0x25d4) passed to
 * Func_0808a098 as a motion script, and 0x0200a5c0 (0x25c0) passed to
 * Func_0808a168.  Both are even, which is the documented one-bit test for
 * in-image data rather than a Thumb entry point.
 *
 * UNCERTAINTIES:
 *  - Func_0808a070's result is compared against 0 at two sites and against 1
 *    at a third; it is the established dialogue-prompt result but the encoding
 *    of the answers is not settled here.
 *  - Func_0808a188/Func_0808a180/Func_0808a178 are reached both with small
 *    slot numbers and with the pooled selectors 0x2012, 0x4001, 0x4002 and
 *    0x4003.  0x2012 is 0x2000 | 18 and the 0x400N group is 0x4000 | N, i.e.
 *    a flag field packed above the slot, but nothing here proves the split, so
 *    they are left as literals.
 *  - Func_0808a208, Func_0808a210, Func_0808a0a0, Func_0808a0e8, Func_0808a168
 *    and Func_0808a368 have no established interface; their argument registers
 *    are set explicitly at each site and are asserted as written.
 *  - Func_0808a360/Func_0808a368/Func_0808a370 are called with no argument
 *    register written, like the Func_0808a018 bracket.
 *  - The three `if (record != 0)` guards are in the bytes: Func_0808a080 can
 *    return 0 and the code tests it before dereferencing.
 */

/* The scene's beat counter: u16 at workspace + 472, workspace being the
 * pointer stored at 0x03001ebc.  Reloaded at every use, as the assembly does. */
#define BEAT_COUNTER (*(u16 *)(*(u8 **)0x03001ebc + 472))

/* Imports. Old-style declarations: one name can take different argument
 * counts at different sites in this overlay. */
           /* wait n frames */
           /* scene request; no arguments set */
           /* wait n frames */
           /* scripted-scene bracket: open */
           /* scripted-scene bracket: close */
            /* dialogue prompt; result selects the branch */
            /* record by slot index, or 0 */
           /* 16.16 scale pair */
           /* start a scripted motion (slot, script) */
           /* scene request; interface not established */
           /* place slot at an integer (x, z) */
           /* place an entity */
           /* place an entity */
           /* start a move; interface not established */
           /* scene request; interface not established */
           /* place slot at (x, z), 16.16 */
           /* per-record scene request */
           /* scene-presentation request */
           /* select an entity presentation */
           /* scene request with an in-image data table */
           /* show a dialogue line by id */
           /* dialogue-related request */
           /* wait for the slot's action to finish */
           /* wait for the slot's action, with a duration */
           /* face slot along heading */
           /* play animation id for n frames */
           /* scene request; interface not established */
           /* scene request; interface not established */
            /* returns a record; its byte at +85 is cleared */
           /* scene request keyed by a selector */
           /* scene bracket helper; no arguments set */
           /* scene bracket helper; no arguments set */
           /* scene bracket helper; no arguments set */
            /* story-flag test; used in a condition */
           /* set a story flag */

/* Intra-overlay, both byte-exact in assets/code and both void(void). */



extern void Func_02003e24();
extern void Func_02003f46();
extern void Func_02003dd4();
extern u8 * Func_02003f58();
extern void Func_02003f66();
extern void Func_02003df4();
extern void Func_02003eee();
extern void Func_02003e1a();
extern void Func_02003e08();
extern void Func_02003fb2();
extern void Func_02003fc6();
extern void Func_02003e8c();
extern void Func_02003f28();
extern void Func_02003f34();
extern void Func_02003fbc();
extern void Func_02003fd2();
extern void Func_02003efc();
extern void Func_02003f06();
extern void Func_02003f44();
extern void Func_02003f52();
extern void Func_02003ef0();
extern void Func_02003f7e();
extern void Func_02003f96();
extern void Func_02003f8c();
extern void Func_02003fa4();
extern void Func_02003f12();
extern void Func_02003ff0();
extern s32 Func_02003f10();
extern void Func_02004024();
extern void Func_0200407e();
extern void Func_02003fae();
extern void Func_02003fea();
extern u8 * Func_02003fb0();
extern void Func_0200401e();
extern u8 * Func_02003fc4();
extern void Func_02004032();
extern u8 * Func_02003fd8();
extern void Func_02004046();
extern void Func_02003ff8();
extern void Func_02004002();
extern void Func_02004010();
extern void Func_02004078();
extern void Func_02004080();
extern void Func_02004088();
extern void Func_0200407c();
extern void Func_02004086();
extern void Func_02004090();
extern void Func_0200409e();
extern void Func_020040b6();
extern void Func_020040be();
extern void Func_020040c6();
extern void Func_02004034();
extern void Func_02004138();
extern void Func_02004144();
extern void Func_020040da();
extern void Func_02004156();
extern void Func_020040ac();
extern void Func_0200406a();
extern s32 Func_0200405a();
extern void Func_02004146();
extern void Func_02004180();
extern s32 Func_0200409a();
extern void Func_02004186();
extern void Func_020041c0();
extern void Func_020035de();
extern void Func_020040f4();
extern s32 Func_020040e2();
extern void Func_0200420a();
extern void Func_02004214();
extern void Func_02004158();
extern void Func_0200425c();
extern void Func_02004256();
extern void Func_02004272();
extern void Func_02004222();
extern void Func_0200426a();
extern void Func_0200428e();
extern void Func_0200423e();
extern void Func_02004290();
extern void Func_02004250();
extern void Func_020041b6();
extern void Func_020042a8();
extern void Func_020042ca();
extern void Func_020042d6();
extern s32 Func_020041c4();
extern void Func_020042a0();
extern void Func_020042d0();
extern void Func_020036ec();
extern s32 Func_0200422e();
extern void Func_02004320();
extern void Func_02003748();
extern void Func_02004232();
extern void Func_02004324();
extern void Func_0200435c();
extern void Func_0200434e();
extern void Func_0200376a();
extern s32 Func_020042aa();
extern void Func_02004384();
extern void Func_020037d4();
extern void Func_020043aa();
extern void Func_02004382();
extern void Func_020043a0();
extern void Func_020043c2();
extern void Func_020043de();
extern void Func_020043ea();
extern void Func_020043f4();
extern void Func_02004400();
extern void Func_020043c8();
extern void Func_02004402();
extern void Func_0200441e();
extern void Func_02003846();
extern void Func_02004330();
extern void Func_0200443a();
extern void Func_0200442c();
extern void Func_020043e4();
extern void Func_020043ec();
extern void Func_02004404();
extern void Func_0200436a();
extern void Func_02004412();
extern void Func_02004464();
extern void Func_02004488();
extern void Func_0200447a();
extern void Func_02004496();
extern void Func_02004490();
extern void Func_02004450();
extern void Func_020038ac();
extern s32 Func_020043ee();
extern void Func_020038fc();
extern void Func_020044d2();
extern void Func_02004502();
extern void Func_020044ca();
extern void Func_02004516();
extern void Func_020044c6();
extern void Func_020044d4();
extern void Func_02004526();
extern void Func_02004542();
extern void Func_0200454e();
extern void Func_02004550();
extern void Func_02004510();
extern void Func_02004562();
extern void Func_0200451a();
extern void Func_02004522();
extern void Func_0200452a();
extern void Func_0200453a();
extern void Func_020044a0();
extern void Func_02004540();
extern u8 * Func_020044de();
extern void Func_02004518();
extern void Func_02004560();
extern u8 * Func_020044fe();
extern void Func_02004538();
extern void Func_02004580();
extern u8 * Func_0200451e();
extern void Func_02004558();
extern void Func_0200458e();
extern void Func_020045a0();
extern void Func_020045aa();
extern void Func_020045a8();
extern void Func_020045ba();
extern void Func_02004530();
extern void Func_02004634();
extern void Func_0200460e();
extern void Func_020045c2();
extern void Func_020045ce();
extern void Func_020045dc();
extern void Func_02004670();
extern void Func_02004638();
extern void Func_02004586();
extern void Func_02004678();
extern void Func_0200460c();
extern void Func_02004610();
extern void Func_02004624();
extern void Func_020046f0();
extern void Func_020046fc();
extern void Func_020045b2();
extern s32 Func_020045b0();
extern void Func_020045c8();
extern void Func_02004706();
extern void Func_020045f2();
void Func_020019b0(void)
{
    u8 *workspace;
    u8 *record;
    s32 pending;

    Func_02003e24();
    Func_02003f46(-1, -1, -1, 0);
    Func_02003dd4(1);

    record = Func_02003f58();
    record[85] = 0;
    Func_02003f66(0x037e0000, -1, 166 << 18, 0);
    Func_02003df4(1);

    Func_02003eee(0, 0, 0);
    Func_02003e1a();
    Func_02003e08(1);

    workspace = *(u8 **)0x03001ebc;
    *(s32 *)(workspace + 456) = 16;
    *(s32 *)(workspace + 448) = 513;

    Func_02003fb2();
    Func_02003fc6();
    Func_02003e8c(40);

    Func_02003f28(19, 222 << 18, 0x031e0000);
    Func_02003f34(0, 226 << 18, 0x031e0000);
    Func_02003fbc(0x9999, 0x1333);
    Func_02003fd2(0x037e0000, -1, 0x02ba0000, 1);

    Func_02003efc(19, 0xcccc, 0x6666);
    Func_02003f06(0, 0x9999, 0x4ccc);
    Func_02003f44(19, 888, 720);
    Func_02003f52(0, 904, 736);
    Func_02003ef0(60);

    Func_02003f7e(19);
    Func_02003f96(19, 1);
    Func_02003f8c(0);
    Func_02003fa4(0, 1);
    Func_02003f12(20);

    Func_02003fd2(19, 2);
    Func_02003ff0(0x1728);

    if (Func_02003f10(0x84f) == 0) {
        BEAT_COUNTER = (u16)(BEAT_COUNTER + 1);
        pending = 0;
    }
    pending = 1;
    Func_02004024(19, 0);
    if (pending != 0) {
        BEAT_COUNTER = (u16)(BEAT_COUNTER + 1);
    }

    Func_0200407e(0x037e0000, -1, 166 << 18, 1);
    Func_02003fae(19, (void *)0x0200a5d4);
    Func_02003fea(0, 0x037e, 684);

    /* Slots 1, 2 and 3 are put where slot 0's record already is. */
    record = Func_02003fb0(0);
    if (record != 0) {
        Func_0200401e(1, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    record = Func_02003fc4(0);
    if (record != 0) {
        Func_02004032(2, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    record = Func_02003fd8(0);
    if (record != 0) {
        Func_02004046(3, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }

    Func_02003ff8(1, 0x9999, 0x4ccc);
    Func_02004002(2, 0x9999, 0x4ccc);
    Func_02004010(3, 0x10000, 0x8000);

    Func_02004078(1, 2);
    Func_02004080(2, 2);
    Func_02004088(3, 2);

    Func_0200407c(1, -16, 16);
    Func_02004086(2, 16, 16);
    Func_02004090(3, 32, 16);

    Func_0200409e(2);
    Func_020040b6(1, 1);
    Func_020040be(2, 1);
    Func_020040c6(3, 1);
    Func_02004034(10);

    Func_02004138(1, 0xc000, 0);
    Func_02004144(2, 0xc000, 0);
    Func_020040da(3);
    Func_02004156(3, 0xa000, 0);

    Func_020040ac(19);
    Func_0200406a(20);

    if (Func_0200405a(0x84f) == 0) {
        BEAT_COUNTER = (u16)(BEAT_COUNTER + 1);
        pending = 0;
    }
    pending = 1;
    Func_02004146(18, 3);
    Func_02004180(0x2012, 0, 20);
    if (pending != 0) {
        BEAT_COUNTER = (u16)(BEAT_COUNTER + 1);
    }

    pending = 1;
    if (Func_0200409a(0x84f) == 0) {
        BEAT_COUNTER = (u16)(BEAT_COUNTER + 1);
        pending = 0;
    }
    Func_02004186(18, 1);
    Func_020041c0(0x2012, 0, 10);
    if (pending != 0) {
        BEAT_COUNTER = (u16)(BEAT_COUNTER + 1);
    }

    Func_020035de();
    Func_020040f4(20);

    if (Func_020040e2(0x84f) != 0) {
        Func_0200420a(0, 258, 60);
        Func_02004214(1, 0x105, 40);
    } else {
        Func_02004158(40);
    }

    Func_0200425c(1, 0xc000, 10);
    Func_02004256(0x4001, 0, 10);
    Func_02004272(2, 0xc000, 10);
    Func_02004222(2, 3);
    Func_0200426a(0x4002, 0);
    Func_0200428e(3, 0xa000, 10);
    Func_0200423e(3, 3);
    Func_02004290(0x4003, 0, 20);
    Func_02004250(18, 3);
    Func_020041b6(20);
    Func_020042a8(0x2012, 0, 20);

    Func_020042ca(1, 0x103, 0);
    Func_020042d6(0, 258, 60);

    if (Func_020041c4(0x84f) != 0) {
        Func_020042a0(18, 1);
        Func_02004290(18, 4);
        Func_020042d0(0x2012, 0);
        Func_020036ec();

        if (Func_0200422e(0, 0) != 0) {
            BEAT_COUNTER = (u16)(BEAT_COUNTER + 1);
            pending = 0;
        }
        pending = 1;
        Func_02004320(18, 0x5000, 0);
        Func_02003748();
        Func_02004232(10);
        Func_02004324(0x2012, 0, 10);
        if (pending != 0) {
            BEAT_COUNTER = (u16)(BEAT_COUNTER + 1);
        }
        Func_0200435c(18, 258, 60);
    } else {
        /* The whole gated block above is worth four beats. */
        BEAT_COUNTER = (u16)(BEAT_COUNTER + 4);
    }

    Func_0200434e(0x2012, 0);
    Func_0200376a();
    if (Func_020042aa(0, 0) == 0) {
        Func_0200435c(0x1737);
    } else {
        Func_02004384(0x1738);
    }
    Func_020037d4();
    Func_020043aa(0x2012, 0, 20);

    Func_02004382(19, 1);
    Func_020043a0(0x1739);
    Func_020043c2(19, 0, 10);

    Func_020043de(0, 0x6000, 0);
    Func_020043ea(1, 0xe000, 0);
    Func_020043f4(2, 0, 0);
    Func_02004400(3, 0x8000, 40);

    Func_020043c8(18, 2);
    Func_02004402(0x2012, 0, 10);
    Func_0200441e(0, 0xc000, 0);
    Func_02003846();
    Func_02004330(10);

    Func_0200443a(18, 0x105, 60);
    Func_0200442c(0x2012, 0, 10);

    Func_020043e4(0, 3);
    Func_020043ec(1, 3);
    Func_020043f4(2, 3);
    Func_02004404(3, 3);
    Func_0200436a(20);
    Func_02004412(18, 3);
    Func_02004464(0x2012, 0, 10);

    Func_02004488(18, 264, 60);
    Func_0200447a(0x2012, 0, 10);
    Func_02004496(18, 0x3000, 10);
    Func_02004490(0x2012, 0, 10);
    Func_02004450(18, 3);

    Func_02004490(0x2012, 0);
    Func_020038ac();

    pending = 1;
    if (Func_020043ee(0, 0) == 1) {
        BEAT_COUNTER = (u16)(BEAT_COUNTER + 1);
        pending = 0;
    }
    Func_020038fc();
    Func_020044d2(0x2012, 0, 10);
    if (pending != 0) {
        BEAT_COUNTER = (u16)(BEAT_COUNTER + 1);
    }

    Func_02004502(18, 0x7000, 10);
    Func_020044ca(19, 1);
    Func_02004516(19, 0x1000, 20);
    Func_020044c6(18, 3);
    Func_0200442c(20);
    Func_020044d4(19, 3);
    Func_02004526(19, 0, 10);
    Func_02004542(19, 0x3000, 10);
    Func_0200454e(18, 0x3000, 20);
    Func_02004516(18, 1);
    Func_02004550(0x2012, 0, 10);
    Func_02004510(18, 3);
    Func_02004562(0x2012, 0, 10);

    Func_0200451a(0, 3);
    Func_02004522(1, 3);
    Func_0200452a(2, 3);
    Func_0200453a(3, 3);
    Func_020044a0(20);

    /* Integer (x, z) views of slot 0's position, offsets +10 and +18. */
    Func_02004540(1, 2);
    record = Func_020044de(0);
    if (record != 0) {
        Func_02004518(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Func_02004560(2, 2);
    record = Func_020044fe(0);
    if (record != 0) {
        Func_02004538(2, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Func_02004580(3, 2);
    record = Func_0200451e(0);
    if (record != 0) {
        Func_02004558(3, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }

    Func_0200458e(1);
    Func_020045a0(1, 0, 0);
    Func_020045aa(2, 0, 0);
    Func_020045a8(3);
    Func_020045ba(3, 0, 0);
    Func_02004530(20);

    Func_02004634(18, 0x5000, 0);
    Func_0200460e(0, 0x00010013, (void *)0x0200a5c0);

    Func_020045c2(19, 852, 0x286);
    Func_020045ce(19, 852, 0x29a);
    Func_020045dc(19, 864, 672);
    Func_02004670(19, 0x1000, 10);
    Func_02004638(19, 1);
    Func_02004586(10);
    Func_02004678(19, 0, 10);
    Func_0200460c(19, 0x376, 708);
    Func_02004610(19, 0x37e, 764);
    Func_02004624(0, 0x37e, 764);

    Func_020046f0();
    Func_020046fc();

    Func_020045b2(0x322);
    if (Func_020045b0(0x84f) == 0) {
        Func_020045c2(0x84f);
        Func_020045c8(0x84a);
    }

    Func_02004706(6);
    Func_020045f2();
}
