#include "types.h"

/*
 * resource_38f owner at 0x020008ec, 7,356 bytes: code 0x020008ec-0x02002589
 * with nine interior literal pools that are branched over and are data, not
 * code (0x02000cba-0x02000ce3, 0x02000f8a-0x02000faf, 0x020011ea-0x02001213,
 * 0x020015bc-0x020015e7, 0x020018e8-0x020018f3, 0x02001a62-0x02001a67,
 * 0x02001e6a-0x02001e93, 0x02002378-0x0200239b, 0x0200258a-0x020025a7).  The
 * next inventory row starts at 0x020025a8 with its own `push {r5, lr}` and has
 * a byte-exact source, so the owner is exactly its advertised span.
 *
 * Prologue `push {r5, r6, r7, lr} / mov r7, sl / mov r6, r8 / push {r6, r7}`;
 * single epilogue at 0x0200257e `pop {r3, r5} / mov r8, r3 / mov sl, r5 /
 * pop {r5, r6, r7} / pop {r0} / bx r0`.  r0 holds the popped return address,
 * so the owner returns nothing, and no incoming argument register is read
 * before the first branch, so it takes none - the caller's r0-r3 reach
 * Func_0808a018 unchanged.  There is no stack frame: this owner passes at most
 * three arguments anywhere.
 *
 * Call accounting: 732 `bl` sites, 47 distinct targets, all resolved with
 * `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml -- resource_38f 08ec` - 43 import veneers
 * and four intra-overlay prologues (Func_020008ac, Func_020008d8,
 * Func_020025a8, Func_020025c0), each of which has a byte-exact source in
 * assets/code whose signature is used here.  Every site appears below exactly
 * once.  (The inventory row advertises `calls=667`; the difference is the
 * sites inside the two `Func_080000c0(1)` spin loops and the repeated bodies
 * the row folds.)  The disassembler's own `bl` annotations are wrong in the
 * usual overlay way - an overlay `bl` stores `target_offset - 2` - and were
 * not used anywhere.
 *
 * LINK BASE, confirmed again and independently of the jump-table witness in
 * semantic/overlays/resource_38f_c_02000304.c: three pool words in this owner
 * are a known function start plus the Thumb bit under the 0x02008000 base.
 * 0x0200a609 - 0x8000 = 0x2609 = Func_02002608 + 1, 0x0200a7ad = 0x27ad =
 * Func_020027ac + 1, and 0x02008801 = 0x0801 = Func_02000800 + 1.  All three
 * are handed to Func_080000d0 / Func_080000d8 in the argument position that
 * takes a per-frame task, and all three of those functions are genuine
 * overlay owners (the first and third byte-exact in assets/code, the second
 * reconstructed as semantic/overlays/resource_38f_c_020027ac.c).  So they are
 * function pointers, not data addresses, and every Data_0200a/b* word below is
 * likewise in-image at `value - 0x8000`.
 *
 * That also settles what the three Data_0200b69x globals are.
 * Func_080000d0(Func_020027ac, 3200) starts the emitter this file's sibling
 * reconstructs; this owner then writes its state directly - Data_0200b690 = 0
 * (ramp), Data_0200b68c = 0x800000 (height, 128.0 in 16.16), Data_0200b694 = 1
 * (phase) - steps the phase to 2 and then 3, and spins on
 * `while (Data_0200b694 != 0) Func_080000c0(1)` until the emitter's phase-3
 * arm resets it.  The two are one mechanism read from both ends.
 *
 * Structure: one long scripted cutscene bracketed by Func_0808a018 /
 * Func_0808a020, driving actors 0/1/2/3 plus the fixed channels 17-22.
 * Data_0200b69c, set once from Func_080770c0(3) at entry, gates every
 * appearance of actor 3; each place where it is false instead bumps the scene
 * counter at scene+0x1d8, the same halfword resource_373's scripted owners
 * advance.  Three Func_0808a070 queries choose between alternative
 * continuations, the last of which splits the tail into two whole variants.
 *
 * Import shapes are the ones resource_373 and resource_39f settled for the
 * same family: Func_0808a010(frames) waits, Func_0808a080(selector) returns
 * the actor record (+8/+12/+16 the 16.16 X/Y/Z, +10/+18 their s16 integer
 * views), Func_0808a090(selector, sx, sz) sets a 16.16 scale pair,
 * Func_0808a0f0(selector, x, z) a 16.16 position, Func_0808a0b8 the integer
 * one, Func_080770c8(id) is the one-argument service.
 *
 * UNCERTAINTIES, recorded rather than guessed:
 *  - Func_0808a1b8/Func_020025c0's second argument is always a multiple of
 *    0x2000 built by shifting a small immediate; the values are reproduced
 *    exactly and read as 16.16 angles (0x10000 = one full turn would make
 *    0x4000 a quarter turn), but nothing here proves the unit.
 *  - Func_0808a330's first argument is 0x7fff, 0x10000, 0x00405210 or
 *    0x00406218 at different sites.  The last two are not addresses on this
 *    machine and are passed with a mode selector of 1, so they are packed
 *    constants; they are left as literals.
 *  - The +0x23 bit 0 and +0x5a bit 0 flips on the actor record are done with
 *    `& 0xfe` / `| 1` on a byte; which state each represents is not
 *    reconstructed.
 *  - Func_0808a218 does not appear here; Func_08009128, Func_0808a360,
 *    Func_0808a370 and Func_0808a4f0 are each reached with no argument
 *    register written since the preceding call, so they are spelled with no
 *    arguments.
 *  - At 0x02001e66 the original loads r0 = 0 and branches to the join at
 *    0x02001e94, where r0 is the first argument of Func_0808a130.  That is
 *    written out as Func_0808a130(0, 2) rather than preserved as a branch.
 */

/* The scene block's counter halfword, through the IWRAM pointer at
 * 0x03001ebc; 236 << 1 = 0x1d8 in the original. */
#define ADVANCE_SCENE_COUNTER() do {                                      \
    u16 *scene_counter_ = (u16 *)(*(u8 **)0x03001ebc + 0x1d8);           \
    *scene_counter_ = *scene_counter_ + 1;                                \
} while (0)

/* In-image data under the 0x02008000 link base. */
extern u8 Data_0200abd4[];
extern u8 Data_0200ac08[];
extern u8 Data_0200ac3c[];
extern u8 Data_0200ac70[];
extern u8 Data_0200acfc[];
extern u8 Data_0200ad20[];
extern u8 Data_0200ad7c[];
extern s32 Data_0200b68c;
extern s32 Data_0200b690;
extern s32 Data_0200b694;
extern s32 Data_0200b698;
extern s32 Data_0200b69c;
extern s32 Data_0200b6a0;

/* Intra-overlay callees; signatures from their byte-exact sources. */
void Func_0200332a();
s32 Func_0200331a();
u8 *Func_0200335a();
void Func_02003318();
u8 *Func_02003366();
void Func_02003324();
u8 *Func_02003372();
void Func_02003330();
u8 *Func_0200337e();
void Func_0200333c();
void Func_0200348e();
void Func_02003408();
void Func_020032f6();
void Func_020034d2();
void Func_020034de();
void Func_020033d0();
void Func_02003412();
void Func_020033e4();
void Func_020033ee();
u8 *Func_020033e4_b();
void Func_02003452();
u8 *Func_020033f8();
void Func_02003466();
void Func_02003426();
void Func_0200342e();
void Func_02003436();
u8 *Func_0200342c();
void Func_0200349a();
void Func_0200345a();
void Func_02003468();
void Func_02003524();
void Func_02003530();
void Func_0200353c();
void Func_02003550();
void Func_0200355c();
void Func_02003568();
void Func_02003574();
void Func_02003586();
void Func_02003592();
void Func_02003566();
void Func_02003576();
void Func_020034c4();
void Func_020035aa();
void Func_02003056(s32, s32);
void Func_0200359c();
void Func_02003584();
void Func_020034f2();
void Func_0200356e();
void Func_0200357a();
void Func_020035ce();
void Func_020035e0();
void Func_0200352e();
void Func_020035f0();
void Func_020035fa();
void Func_0200360a();
void Func_02003558();
void Func_020035fe();
void Func_0200360e();
void Func_0200357c();
void Func_0200361c();
void Func_0200358a();
void Func_0200368c();
void Func_0200359a();
void Func_02001422(s32, s32);
void Func_020035a8();
void Func_02001458(void);
void Func_020036c0();
u8 *Func_020035f4();
void Func_02003654();
void Func_020035ea();
u8 *Func_02003618();
void Func_0200370e();
void Func_0200318a(s32, s32);
void Func_02003722();
void Func_0200319e(s32, s32);
void Func_020036ea();
void Func_020036fc();
void Func_02003746();
void Func_02003752();
void Func_0200375e();
void Func_02003716();
void Func_020031f2(s32, s32);
void Func_020036bc();
u8 *Func_020036b2();
void Func_02003710();
void Func_020036a6();
u8 *Func_020036d4();
void Func_020037d4();
void Func_020036d2();
void Func_02003256(s32, s32);
void Func_02003772();
void Func_020037e0();
void Func_020037c0();
void Func_020037c8();
void Func_020037d8();
void Func_02003860();
void Func_02003756();
void Func_020032f6_b(s32, s32, s32);
void Func_02003302(s32, s32, s32);
void Func_02003856();
s32 Func_0200379e();
void Func_02003788();
void Func_02003820();
void Func_02003832();
void Func_02003332(s32, s32);
void Func_02003354(s32, s32, s32);
void Func_02003360(s32, s32, s32);
void Func_02003370(s32, s32, s32);
void Func_020038ee();
void Func_020037ee();
void Func_020038f2();
void Func_02003398(s32, s32, s32);
void Func_020033a2(s32, s32, s32);
void Func_02003914();
void Func_020033ba(s32, s32, s32);
void Func_02003938();
void Func_02003942();
void Func_02003840();
void Func_020033e8(s32, s32, s32);
void Func_0200390c();
void Func_0200393a();
void Func_020033e6(s32, s32);
void Func_0200340e(s32, s32, s32);
void Func_0200391a();
void Func_02003406(s32, s32);
void Func_02003428(s32, s32, s32);
void Func_02003432(s32, s32, s32);
void Func_0200343c(s32, s32, s32);
void Func_02003446(s32, s32, s32);
void Func_02003450(s32, s32, s32);
void Func_0200345c(s32, s32, s32);
void Func_02003980();
void Func_020039e4();
void Func_020038e2();
void Func_0200348a(s32, s32, s32);
void Func_02003494(s32, s32, s32);
void Func_02003aa6();
void Func_02003aac();
void Func_02003a4c();
void Func_02003a5a();
void Func_02003890();
void Func_020038a8();
void Func_020038a6();
void Func_02003a76();
void Func_02003a80();
void Func_02003a8e();
void Func_020038c4();
void Func_0200399e();
void Func_020039a6();
void Func_020039ae();
void Func_020039b6();
void Func_0200397c();
void Func_02003500(s32, s32);
void Func_02003508(s32, s32);
void Func_0200399a();
void Func_02003ab4();
void Func_020039aa();
void Func_0200352e_b(s32, s32);
void Func_020039f0();
u8 *Func_02003a26();
void Func_02003a0a();
void Func_02003a98();
void Func_02003a68();
u8 *Func_02003a56();
void Func_02003a14();
void Func_02003acc();
void Func_02003a42();
u8 *Func_02003a70();
void Func_02003a54();
void Func_02003a96();
void Func_02003aa8();
u8 *Func_02003a96_b();
void Func_02003b0c();
void Func_02003a82();
u8 *Func_02003ab0();
void Func_02003a90();
void Func_02003ad2();
void Func_02003ae2();
u8 *Func_02003ad0();
void Func_02003b46();
void Func_02003abc();
u8 *Func_02003aea();
void Func_02003aca();
void Func_02003b12();
u8 *Func_02003b00();
void Func_02003abe();
void Func_02003b76();
void Func_02003af2();
void Func_02003a78();
void Func_02003afe();
void Func_02003c3e();
void Func_02003c4c();
void Func_02003b34();
void Func_02003cda();
void Func_020036be(s32, s32);
void Func_020036c6(s32, s32);
void Func_02003c64();
void Func_02003c6e();
void Func_02003c78();
void Func_02003c82();
void Func_02003b78();
void Func_020036fc_b(s32, s32);
void Func_02003c34();
void Func_02003b92();
void Func_02003c42();
void Func_02003ba0();
void Func_02003c50();
void Func_02003c58();
void Func_02003bb6();
void Func_02003c66();
void Func_02003bc4();
void Func_02003c74();
void Func_02003bd2();
void Func_02003be0();
void Func_02003c90();
void Func_02003d02();
void Func_02003776(s32, s32);
void Func_02003d1a();
void Func_0200378e(s32, s32);
u8 *Func_02003c82_b();
u8 *Func_02003c94();
u8 *Func_02003ca4();
u8 *Func_02003cb4();
void Func_02003d9e();
void Func_02003da6();
void Func_02003dae();
void Func_02003db6();
void Func_02003e6c();
u8 *Func_02003cfa();
void Func_02003de4();
void Func_02003d7a();
void Func_02003d3c();
u8 *Func_02003d2a();
void Func_02003e14();
void Func_02003daa();
void Func_02003d6a();
u8 *Func_02003d60();
void Func_02003e4a();
void Func_02003de0();
void Func_02003da0();
u8 *Func_02003d8e();
void Func_02003e78();
void Func_02003e0e();
void Func_02003dce();
void Func_02003d0c();
void Func_02003d30();
void Func_02003f5c();
void Func_02003efe();
void Func_02003f0c();
void Func_02003d42();
void Func_02003e28();
void Func_02003e2e();
void Func_02003e3c();
void Func_02003e42();
void Func_02003d68();
void Func_02003e58();
void Func_02003e26();
void Func_02003ede();
void Func_02003e34();
void Func_02003f1c();
void Func_02003ed2();
void Func_02003ee0();
s32 Func_02003e86();
void Func_02003f32();
void Func_02003e88();
void Func_02003f40();
void Func_02003a14_b(s32, s32);
void Func_02003f56();
void Func_02003eac();
void Func_02003f8a();
void Func_02003a36(s32, s32);
void Func_02003f6a();
void Func_02003fd4();
void Func_02003fb8();
void Func_02003a64(s32, s32);
void Func_02003fa0();
void Func_02003ef6();
void Func_02003ffe();
u8 *Func_02003f2c();
u8 *Func_02003f40_b();
void Func_02003fc8();
void Func_02003fc0();
void Func_02003ad0_b(s32, s32, s32);
void Func_02003ac0(s32, s32);
void Func_02003ff4();
void Func_02003ad2_b(s32, s32);
void Func_02004010();
void Func_02003afe_b(s32, s32, s32);
void Func_0200407e();
void Func_02003f7e();
void Func_02003b1c(s32, s32, s32);
void Func_02003b26(s32, s32, s32);
void Func_02003b30(s32, s32, s32);
void Func_02003b3a(s32, s32, s32);
void Func_02004050();
void Func_02003fb6();
void Func_02004060();
void Func_02003fc6();
void Func_02004070();
void Func_02003fd6();
void Func_02003b5a(s32, s32);
void Func_02004126();
void Func_02004024();
void Func_020040dc();
void Func_02004032();
void Func_0200413a();
u8 *Func_02004068();
u8 *Func_02004078();
void Func_02004036();
void Func_02004100();
void Func_020040e4();
void Func_02004104();
void Func_02003c14(s32, s32, s32);
void Func_02004138();
void Func_0200408e();
void Func_02003c12(s32, s32);
void Func_02004156();
void Func_02003c56(s32, s32, s32);
void Func_02004164();
void Func_02003c6a(s32, s32, s32);
void Func_02003c5a(s32, s32);
void Func_0200417e();
void Func_020040ec();
void Func_020041a4();
void Func_020040fa();
void Func_020041b2();
void Func_02004108();
void Func_02004210();
u8 *Func_0200413e();
u8 *Func_0200414e();
void Func_0200410c();
void Func_020041d6();
void Func_020041ce();
void Func_02004242();
void Func_02004202();
void Func_02004158();
void Func_02004260();
u8 *Func_0200418e();
u8 *Func_0200419c();
void Func_0200415a();
void Func_02004226();
void Func_02004220();
void Func_02003d2e(s32, s32, s32);
void Func_020042ac();
void Func_020042b6();
void Func_020041b4();
void Func_02003d54(s32, s32, s32);
void Func_02004268();
void Func_02003d6e(s32, s32, s32);
void Func_02003d78(s32, s32, s32);
void Func_020042e8();
void Func_02004290();
void Func_02003d7c(s32, s32);
void Func_02004298();
void Func_020042a0();
void Func_020042b0();
void Func_0200421e();
void Func_020042d6();
void Func_0200422c();
void Func_02004314();
void Func_020042cc();
void Func_02004340();
void Func_0200434a();
s32 Func_0200427a();
void Func_020042f6();
void Func_02004306();
void Func_02004338();
void Func_02003e26_b(s32, s32, s32);
void Func_02004342();
void Func_0200438a();
void Func_02003e42_b(s32, s32, s32);
void Func_0200434e();
void Func_02003e3a(s32, s32);
void Func_02003e5e(s32, s32, s32);
void Func_0200436a();
void Func_02003e56(s32, s32);
void Func_0200439a();
void Func_02003e88_b(s32, s32, s32);
void Func_02003e94(s32, s32, s32);
void Func_02004398();
void Func_02003e8c(s32, s32);
void Func_02004436();
void Func_02003edc(s32, s32, s32);
void Func_020043e0();
void Func_020043f0();
void Func_0200435e();
void Func_02004466();
void Func_02003f0c_b(s32, s32, s32);
void Func_02004418();
void Func_02004386();
void Func_0200449a();
void Func_020044a6();
void Func_02003f3e(s32, s32, s32);
void Func_0200445a();
void Func_02003f36(s32, s32);
void Func_020044be();
void Func_02003f64(s32, s32, s32);
void Func_020044d2();
void Func_02003f7a(s32, s32, s32);
void Func_02003f86(s32, s32, s32);
void Func_02004492();
void Func_02003f7e_b(s32, s32);
void Func_0200451c();
void Func_02004412();
void Func_02003f96(s32, s32);
void Func_020044ba();
void Func_02004428();
void Func_02004542();
void Func_02004438();
void Func_02003fbc(s32, s32);
void Func_020044d8();
void Func_02003fcc(s32, s32);
void Func_02004508();
void Func_02003ff8(s32, s32, s32);
void Func_0200454c();
void Func_0200456e();
s32 Func_0200449e();
void Func_02004522();
void Func_02004498();
void Func_02004550();
void Func_020045a6();
void Func_02004664();
void Func_02004604();
void Func_02004612();
void Func_02004448();
void Func_0200446e();
void Func_020044fc();
void Func_020045ac();
void Func_020045b4();
void Func_020045bc();
void Func_020045ce();
void Func_02004526();
void Func_020040c4(s32, s32, s32);
void Func_0200460e();
void Func_020040ba(s32, s32);
void Func_020040dc_b(s32, s32, s32);
void Func_020040e6(s32, s32, s32);
void Func_020040f8(s32, s32, s32);
u8 *Func_02004592();
u8 *Func_020045a4();
u8 *Func_020045b4_b();
u8 *Func_020045c4();
void Func_020046ac();
void Func_020046b4();
void Func_020046bc();
void Func_020046c4();
void Func_0200476e();
void Func_02004626();
void Func_0200467c();
void Func_0200463c();
void Func_02004698();
void Func_02004658();
void Func_020046ae();
void Func_0200466e();
void Func_02004634();
void Func_020045b0();
void Func_020041ca(s32, s32);
void Func_020041d2(s32, s32);
void Func_02004768();
void Func_02004772();
void Func_0200477c();
void Func_02004786();
void Func_02004684();
void Func_02004208(s32, s32);
void Func_02004724();
void Func_0200472c();
void Func_02004734();
void Func_02004744();
void Func_02004230(s32, s32);
void Func_0200474c();
void Func_02004754();
void Func_0200475c();
void Func_0200476c();
void Func_02004258(s32, s32);
void Func_020047f0();
void Func_020047fc();
void Func_02004808();
void Func_02004814();
void Func_02004712();
void Func_02004296(s32, s32);
void Func_0200481e();
void Func_0200482e();
void Func_02004838();
void Func_020042de(s32, s32, s32);
void Func_020042ce(s32, s32);
void Func_0200486c();
void Func_02004876();
void Func_02004880();
void Func_0200488a();
void Func_0200487c();
void Func_02004886();
void Func_02004890();
void Func_02004336(s32, s32, s32);
void Func_02004326(s32, s32);
void Func_020048ae();
void Func_020048ba();
void Func_020048c4();
void Func_0200436a_b(s32, s32, s32);
void Func_0200435a(s32, s32);
void Func_020048e0();
void Func_020048ea();
void Func_020048f4();
void Func_0200439a_b(s32, s32, s32);
void Func_0200489e();
void Func_020048a6();
void Func_020048be();
void Func_0200482c();
void Func_020043b0(s32, s32);
void Func_020048cc();
void Func_020048d4();
void Func_020048dc();
void Func_020048ec();
void Func_020043d8(s32, s32);
void Func_02004960();
void Func_0200496c();
void Func_02004976();
void Func_0200441c(s32, s32, s32);
void Func_0200440c(s32, s32);
void Func_02004974();
void Func_0200497c();
void Func_0200498c();
void Func_020049de();
void Func_020049e8();
void Func_020049f2();
void Func_020049fc();
void Func_02004488(s32, s32);
void Func_02004a10();
void Func_02004a1c();
void Func_02004a26();
void Func_020044cc(s32, s32, s32);
void Func_020044bc(s32, s32);
void Func_02004a5a();
void Func_02004a64();
void Func_02004a6e();
void Func_02004a78();
void Func_0200496e();
void Func_020044f2(s32, s32);
void Func_02004a82();
void Func_02004a8c();
void Func_02004532(s32, s32, s32);
void Func_02004522_b(s32, s32);
void Func_02004a3e();
void Func_02004a46();
void Func_02004a4e();
void Func_02004a5e();
void Func_020049cc();
void Func_02004abc();
void Func_02004ac4();
void Func_02004962();
void Func_02004b2a();
void Func_02004b38();
void Func_02004a54();
void Func_02004a62();
void Func_02004a68();
void Func_0200498e();
void Func_02004a70();
void Func_02004a7e();
void Func_02004a9e();
void Func_02004a4c();
void Func_02004b54();
void Func_02004b5c();
void Func_02004b64();
void Func_02004b6c();
u8 *Func_02004a9a();
u8 *Func_02004aaa();
u8 *Func_02004ab8();
u8 *Func_02004ac6();
void Func_02004b62();
void Func_02004650(s32, s32, s32);
void Func_02004ba4();
void Func_02004bc8();
void Func_02004bd2();
s32 Func_02004b02();
void Func_02004b9e();
void Func_02004af4();
void Func_02004bdc();
s32 Func_02004b24();
void Func_020046a6(s32, s32, s32);
void Func_02004c24();
void Func_02004c2e();
void Func_02004c38();
void Func_02004b36();
void Func_020046d6(s32, s32, s32);
void Func_020046c6(s32, s32);
void Func_020046e8(s32, s32, s32);
void Func_020046f2(s32, s32, s32);
void Func_02004bfe();
void Func_020046ea(s32, s32);
void Func_0200470e(s32, s32, s32);
void Func_0200471a(s32, s32, s32);
void Func_02004c9a();
void Func_02004ca6();
void Func_02004cb2();
void Func_0200474a(s32, s32, s32);
void Func_02004c94();
void Func_02004740(s32, s32);
void Func_02004c64();
void Func_02004750(s32, s32);
void Func_02004c6c();
void Func_02004c7c();
void Func_02004bec();
void Func_02004c8c();
void Func_02004bfa();
void Func_02004cd8();
void Func_02004784(s32, s32);
void Func_02004d0c();
void Func_020047b4(s32, s32, s32);
void Func_02004cb8();
void Func_02004cc8();
void Func_02004c36();
void Func_02004cd6();
void Func_02004d26();
s32 Func_02004c6e();
void Func_02004c5a();
void Func_02004d6c();
void Func_02004c6a();
void Func_02004808_b(s32, s32, s32);
void Func_020047f8(s32, s32);
void Func_02004820(s32, s32, s32);
void Func_02004d3c();
void Func_02004818(s32, s32);
void Func_02004dc8();
void Func_02004dd2();
void Func_02004d80();
void Func_02004854(s32, s32);
void Func_02004dea();
void Func_02004ce8();
void Func_0200486c_b(s32, s32);
void Func_02004898(s32, s32, s32);
void Func_02004da4();
void Func_02004890_b(s32, s32);
void Func_02004d2c();
void Func_02004de4();
void Func_020048dc_b(s32, s32, s32);
void Func_020048e8(s32, s32, s32);
void Func_020048d8(s32, s32);
void Func_02004e0c();
void Func_02004e1c();
void Func_02004d72();
void Func_020048f6(s32, s32);
void Func_02004e1a();
void Func_02004e22();
void Func_02004d90();
void Func_02004e28();
void Func_02004e38();
void Func_02004ed8();
void Func_02004dd6();
void Func_02004e76();
void Func_02004ebc();
void Func_02004968(s32, s32);
void Func_02004990(s32, s32, s32);
void Func_02004eac();
void Func_02004988(s32, s32);
void Func_02004f38();
void Func_02004f42();
void Func_02004ef0();
void Func_020049c4(s32, s32);
void Func_02004f5a();
void Func_02004e58();
void Func_020049dc(s32, s32);
void Func_02004a08(s32, s32, s32);
void Func_02004f0c();
void Func_02004a00(s32, s32);
void Func_02004f4e();
void Func_02004ea4();
void Func_02004a28(s32, s32);
void Func_02004f5c();
void Func_02004f6c();
void Func_02004ec2();
void Func_02004a46_b(s32, s32);
void Func_02005070();
void Func_02004f12();
void Func_02004f1c();
void Func_02004f26();
void Func_02004f86();
u8 *Func_02004f24();
void Func_02004f6e();
void Func_02004f94();
void Func_02004fa6();
void Func_02004fb6();
u8 *Func_02004f54();
void Func_02004f9e();
void Func_02004fc4();
void Func_02004fd6();
void Func_02004fe0();
void Func_02004fea();
void Func_02005002();
u8 *Func_02004fa0();
void Func_02005010();
void Func_02005022();
void Func_02004f98();
void Func_0200512c();
void Func_02004fb8();

                             
                                  
void Func_02000800(void);
void Func_02002608(void);
void Func_020027ac(void);

/* Old-style declarations are mandatory in overlay sources: one import name can
 * legitimately take different argument counts at different sites. */

                     

                     

                     

                     

                     

                     

                     

                     

                     

                     

                     

                     

                     

                     

/* `ldrb / and 0xfe / strb` and `ldrb / orr 1 / strb` on the record byte.
 * These were once four macros; they are written out at every use because each
 * use is a SEPARATE Func_0808a080 call site in the assembly and the per-target
 * call multiset is taken over sites. */

void Func_020008ec(void) {
    u8 *record;
    s32 *has_actor;

    Func_0200332a();
    has_actor = &Data_0200b69c;
    *has_actor = Func_0200331a(3);

    Func_02003318(Func_0200335a(19), 0);
    Func_02003324(Func_02003366(20), 0);
    Func_02003330(Func_02003372(21), 0);
    Func_0200333c(Func_0200337e(22), 0);

    /* 208 << 15 and 128 << 17. */
    Func_0200348e(0x00680000, -1, 0x01000000, 0);
    Func_0200332a();
    /* 184 << 13 and 247 << 16. */
    Func_02003408(0, 0x00170000, 0x00f70000);
    Func_020032f6(1);
    Func_020034d2();
    Func_020034de();

    Func_020033d0(0, 0xcccc, 0x6666); /* 0.8 / 0.4 */
    Func_02003412(0, 121, 238);
    Func_020033e4(1, 0x9999, 0x4ccc); /* 0.6 / 0.3 */
    Func_020033ee(2, 0x9999, 0x4ccc);

    record = Func_020033e4_b(0);
    if (record != 0) {
        Func_02003452(1, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    record = Func_020033f8(0);
    if (record != 0) {
        Func_02003466(2, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Func_02003426(1, Data_0200abd4);
    Func_0200342e(2, Data_0200ac08);

    if (Data_0200b69c != 0) {
        Func_02003436(3, 0x9999, 0x4ccc);
        record = Func_0200342c(0);
        if (record != 0) {
            Func_0200349a(3, *(s32 *)(record + 8), *(s32 *)(record + 16));
        }
        Func_0200345a(3, Data_0200ac3c);
    }

    Func_02003468(2);
    Func_02003524(0, 0xe000, 20);
    Func_02003530(2, 0xe000, 20);
    Func_0200353c(1, 0x2000, 20);
    if (Data_0200b69c != 0) {
        Func_02003550(3, 0x2000, 0);
    }
    Func_0200355c(0, 0x2000, 20);
    Func_02003568(2, 0x2000, 20);
    Func_02003574(1, 0xe000, 60);
    if (Data_0200b69c != 0) {
        Func_02003586(3, 0xe000, 0);
    }
    Func_02003592(0, 0x8000, 0);
    Func_0200355c(2, 0, 0);
    Func_02003566(1, 0, 0);
    if (Data_0200b69c != 0) {
        Func_02003576(3, 0, 0);
    }

    Func_020034c4(40);
    Func_02003574(1, 1);
    Func_020035aa(0x1473);
    Func_02003056(1, 20);
    Func_0200359c(0, 1, 0);
    Func_02003584(0, 3);
    Func_020034f2(30);

    Func_0200356e(2, 72, 286);
    Func_0200357a(2, 72, 302);
    Func_02003586(2, 88, 310);
    Func_020035ce(2, 1);
    Func_020035e0(2, 0, 0);
    Func_0200352e(20);
    Func_020035f0(0, 2, 0);
    Func_020035fa(1, 2, 0);
    if (Data_0200b69c != 0) {
        Func_0200360a(3, 2, 0);
    }
    Func_02003558(30);
    Func_020035f0(1, 3);
    if (Data_0200b69c != 0) {
        Func_020035fe(3, 3);
    }
    Func_0200360e(0, 3);
    Func_0200357c(20);
    Func_0200361c(2, 3);
    Func_0200358a(30);
    Func_0200368c(2, 0, 0);
    Func_0200359a(20);
    Func_02001422(2, 9);
    Func_020035a8(40);
    Func_02001458();

    Func_020036c0(2, 256, 40);
    Func_020035fe(2, 0x10000, 0x8000); /* 1.0 / 0.5 */
    {
        u8 *r_ = Func_020035f4(2);
        r_[0x5a] = r_[0x5a] & 0xfe;
    }
    Func_02003654(2, 80, 310);
    Func_020035ea(1);
    {
        u8 *r_ = Func_02003618(2);
        r_[0x5a] = r_[0x5a] | 1;
    }
    Func_0200370e(1, 258, 40);
    Func_0200318a(1, 20);
    Func_02003722(2, 258, 40);
    Func_0200319e(2, 20);
    if (Data_0200b69c != 0) {
        Func_020036ea(3, 0, 0);
    }
    Func_020036fc(2, 1, 60);
    if (Data_0200b69c != 0) {
        Func_02003746(3, 0xc000, 0);
    }
    Func_02003752(1, 0xc000, 0);
    Func_0200375e(0, 0xe000, 0);
    Func_02003716(1, 1);
    Func_020031f2(1, 20);
    Func_020036bc(2, 0x8000, 0x4000); /* 0.5 / 0.25 */
    {
        u8 *r_ = Func_020036b2(2);
        r_[0x5a] = r_[0x5a] & 0xfe;
    }
    Func_02003710(2, 72, 286);
    Func_020036a6(1);
    {
        u8 *r_ = Func_020036d4(2);
        r_[0x5a] = r_[0x5a] | 1;
    }
    Func_020036fc(2, Data_0200ac08);

    if (Data_0200b69c != 0) {
        Func_020037d4(3, 0x105, 0);
        Func_020036d2(60);
        Func_02003256(3, 20);
        Func_02003772(3, 3);
    } else {
        ADVANCE_SCENE_COUNTER();
    }

    Func_020037e0(2, 0, 0);
    Func_020037c0(0, 3);
    Func_020037c8(1, 3);
    Func_020037d8(2, 3);
    Func_02003746(20);
    Func_02003860(1, 258);
    Func_02003756(10);
    Func_020032f6_b(1, 0x2000, 10);
    Func_02003302(0, 0xa000, 10);
    Func_02003856(1, 0);

    if (Func_0200379e(0, 0) == 0) {
        Func_02003788(20);
        Func_02003820(1, 3);
    } else {
        Func_02003832(1, 4);
        ADVANCE_SCENE_COUNTER();
    }

    {
        s32 angle_4000;
        s32 angle_6000;
        s32 angle_2000;
        s32 angle_a000;
        s32 angle_c000;

        angle_2000 = 0x2000;
        Func_02003332(1, 40);
        Func_02003354(2, angle_2000, 40);
        Func_02003360(2, 0x8000, 20);
        angle_4000 = 0x4000;
        Func_02003370(2, angle_4000, 40);
        Func_020038ee(2, 0x101, 0);
        Func_020037ee(60);
        Func_020038f2(1, angle_4000, 0);
        angle_6000 = 0x6000;
        Func_02003398(0, angle_6000, 60);
        Func_020033a2(3, angle_2000, 10);
        Func_02003914(1, angle_2000, 0);
        angle_a000 = 0xa000;
        Func_020033ba(0, angle_a000, 10);
        Func_02003938(1, 0x101, 0);
        Func_02003942(0, 0x101, 0);
        Func_02003840(40);
        Func_02003942(1, angle_4000, 0);
        Func_020033e8(0, angle_6000, 10);
        Func_0200390c(1, 2);
        Func_0200393a(0x147b);
        Func_020033e6(1, 10);
        angle_c000 = 0xc000;
        Func_0200340e(2, angle_c000, 20);
        Func_0200391a(2, 3);
        Func_02003406(2, 20);
        Func_02003428(1, 0, 20);
        Func_02003432(0, angle_a000, 40);
        Func_0200343c(1, angle_4000, 20);
        Func_02003446(0, angle_6000, 30);
        Func_02003450(1, angle_6000, 20);
        Func_0200345c(0, 0xe000, 30);
        Func_02003980(2, 2);
        Func_020039e4(2, 256, 0);
        Func_020038e2(40);
        Func_020039e4(1, angle_4000, 0);
        Func_0200348a(0, angle_6000, 20);
        Func_02003494(2, angle_c000, 10);
    }

    Func_02003aa6(17);
    Func_02003aac(206);
    Func_02003a4c(0x7fff, 0);
    Func_02003a5a(1);
    Func_02003890(1);

    Data_0200b6a0 = 1;
    Func_020038a8(Func_02002608, 3200);
    Func_020038a6(20);
    Func_02003a76(0x00405210, 1);
    Func_02003a80(0x10000, 2);
    Func_02003a8e(120);
    Func_020038c4(60);

    Func_0200399e(0, Data_0200ac70);
    Func_020039a6(1, Data_0200ac70);
    Func_020039ae(2, Data_0200ac70);
    Func_020039b6(3, Data_0200ac70);
    Func_0200397c(100);
    Func_02003500(1, 20);
    Func_02003508(2, 40);

    if (Data_0200b69c != 0) {
        Func_0200399a(40);
        Func_02003ab4(3, 258);
        Func_020039aa(40);
        Func_0200352e_b(3, 40);
    } else {
        ADVANCE_SCENE_COUNTER();
    }

    Func_020039f0(20);

    if (Data_0200b69c != 0) {
        s32 scale_20000;

        record = Func_02003a26(3);
        scale_20000 = 0x20000;
        *(s32 *)(record + 0x28) = scale_20000;
        Func_02003a0a(10);
        Func_02003a4c(3, scale_20000, scale_20000);
        Func_02003a98(3, -2, 0);
        Func_02003a68(3, Data_0200acfc);
        Func_02003a14(Func_02003a56(3), 0);
        Func_02003acc(3, 19);
        Func_02003a42(10);
    }

    {
        s32 scale_20000;

        record = Func_02003a70(0);
        scale_20000 = 0x20000;
        *(s32 *)(record + 0x28) = scale_20000;
        Func_02003a54(10);
        Func_02003a96(0, scale_20000, scale_20000);
        Func_02003aa8(0, Data_0200acfc);
        Func_02003a54(Func_02003a96_b(0), 0);
        Func_02003b0c(0, 19);
        Func_02003a82(20);

        record = Func_02003ab0(1);
        *(s32 *)(record + 0x28) = scale_20000;
        Func_02003a90(10);
        Func_02003ad2(1, scale_20000, scale_20000);
        Func_02003ae2(1, Data_0200acfc);
        Func_02003a8e(Func_02003ad0(1), 0);
        Func_02003b46(1, 19);
        Func_02003abc(40);

        record = Func_02003aea(2);
        *(s32 *)(record + 0x28) = scale_20000;
        Func_02003aca(10);
        Func_02003b12(2, Data_0200acfc);
        Func_02003abe(Func_02003b00(2), 0);
        Func_02003b76(2, 19);
    }

    Data_0200b6a0 = 0;
    Func_02003af2(160);
    Func_02003a78(Func_02002608);
    Func_02003afe(120);
    Func_02003c3e(0x00406218, 1);
    Func_02003c4c(60);
    Func_02003a82(60);

    Data_0200b690 = 0;
    Data_0200b68c = 0x00800000; /* 128.0 in 16.16 */
    Data_0200b694 = 1;
    Func_02003aa6(Func_020027ac, 3200);
    Func_02003b34(180);
    Func_02003cda(21);
    Func_020036be(1, 80);
    Func_020036c6(2, 40);
    Func_02003c64(0, 258);
    Func_02003c6e(1, 258);
    Func_02003c78(2, 258);
    Func_02003c82(3, 258);
    Func_02003b78(60);
    Func_020036fc_b(2, 20);

    Data_0200b694 = 2;
    Func_02003c34(2, 2);
    Func_02003b92(20);
    Func_02003c42(1, 1);
    Func_02003ba0(40);
    Func_02003c50(0, 2);
    Func_02003c58(3, 1);
    Func_02003bb6(20);
    Func_02003c66(2, 3);
    Func_02003bc4(40);
    Func_02003c74(0, 1);
    Func_02003bd2(20);
    Func_02003c82(1, 2);
    Func_02003be0(20);
    Func_02003c90(3, 2);
    Func_02003d02(1, 258);
    Func_02003776(1, 20);

    if (Data_0200b69c != 0) {
        Func_02003d1a(3, 258);
        Func_0200378e(3, 10);
    } else {
        ADVANCE_SCENE_COUNTER();
    }

    {
        u8 *actor_data;
        s32 zero;

        zero = 0;
        Data_0200b694 = 3;
        {
            u8 *r_ = Func_02003c82_b(0);
            r_[0x23] = r_[0x23] & 0xfe;
        }
        {
            u8 *r_ = Func_02003c94(1);
            r_[0x23] = r_[0x23] & 0xfe;
        }
        {
            u8 *r_ = Func_02003ca4(2);
            r_[0x23] = r_[0x23] & 0xfe;
        }
        {
            u8 *r_ = Func_02003cb4(3);
            r_[0x23] = r_[0x23] & 0xfe;
        }
        Func_02003d9e(0, 3);
        Func_02003da6(1, 3);
        Func_02003dae(2, 3);
        Func_02003db6(3, 3);

        Data_0200b698 = zero;
        Func_02003c3e(Func_02000800, 3200);
        Func_02003e6c(220);

        {
            u8 *r_ = Func_02003cfa(19);
            r_[0x23] = r_[0x23] & 0xfe;
        }
        Func_02003de4(19, 2);
        Func_02003d7a(19, 0x00780000, 0x00f80000);
        actor_data = Data_0200ad20;
        Func_02003d3c(19, actor_data);

        {
            u8 *r_ = Func_02003d2a(20);
            r_[0x23] = r_[0x23] & 0xfe;
        }
        Func_02003e14(20, 2);
        Func_02003daa(20, 0x00640000, 0x01120000);
        Func_02003d6a(20, actor_data);

        if (Data_0200b69c != 0) {
            {
                u8 *r_ = Func_02003d60(21);
                r_[0x23] = r_[0x23] & 0xfe;
            }
            Func_02003e4a(21, 2);
            Func_02003de0(21, 0x004a0000, 0x00fe0000);
            Func_02003da0(21, actor_data);
        }

        {
            u8 *r_ = Func_02003d8e(22);
            r_[0x23] = r_[0x23] & 0xfe;
        }
        Func_02003e78(22, 2);
        Func_02003e0e(22, 0x005e0000, 0x00e10000);
        Func_02003dce(22, actor_data);
    }

    /* Wait for the emitter's phase-3 arm to clear the phase. */
    while (Data_0200b694 != 0) {
        Func_02003d0c(1);
    }

    Func_02003daa(300);
    Func_02003d30(Func_020027ac);
    Func_02003db6(120);
    Func_02003f5c(17);
    Func_02003efe(0x10000, 1);
    Func_02003f0c(60);
    Func_02003d42(60);

    Func_02003e28(19);
    Func_02003e2e(20);
    if (Data_0200b69c != 0) {
        Func_02003e3c(21);
    }
    Func_02003e42(22);
    Func_02003d68(1);
    Func_02003e42(19, Data_0200ad7c);
    Func_02003e4a(20, Data_0200ad7c);
    if (Data_0200b69c != 0) {
        Func_02003e58(21, Data_0200ad7c);
    }
    Func_02003e78(22, Data_0200ad7c);

    Func_02003e26(80);
    Func_02003ede(1, 2);
    Func_02003e34(40);
    Func_02003f1c(1, 0);
    Func_02003ed2(17, 0x00570000, 0x008b0000);
    Func_02003ee0(18, 0x00570000, 0x008b0000);
    Func_02003dce(1);
    if (Func_02003e86(17, 0) == 1) {
        ADVANCE_SCENE_COUNTER();
    }
    Func_02003f32(0, 1);
    Func_02003e88(20);
    Func_02003f40(2, 2);
    Func_02003a14_b(2, 20);

    if (Data_0200b69c != 0) {
        Func_02003f56(3, 2);
        Func_02003eac(10);
        Func_02003f8a(0x1488);
        Func_02003a36(3, 40);
    }

    Func_02003f6a(1, 1);
    Func_02003fd4(1, 0x101, 0);
    Func_02003ed2(80);
    Func_02003f8a(2, 2);
    Func_02003fb8(0x1489);
    Func_02003a64(2, 40);
    Func_02003fa0(1, 3);
    Func_02003ef6(40);
    Func_02003ffe(1, 2);
    {
        u8 *r_ = Func_02003f2c(1);
        r_[0x23] = r_[0x23] | 1;
    }
    Func_02003efe(Func_02003f40_b(1), 1);
    Func_02003fc8(1, 6, 0);
    Func_02003fc0(1, 1);
    Func_02003ad0_b(1, 0x4000, 60);
    Func_02003ac0(1, 20);
    {
        s32 angle_2000;
        s32 angle_6000;

        Func_02003ff4(1, 2);
        Func_02003ad2_b(1, 10);
        angle_2000 = 0x2000;
        Func_02004010(0, 3);
        Func_02003afe_b(1, angle_2000, 20);
        Func_0200407e(1, 0x101, 0);
        angle_6000 = 0x6000;
        Func_02003f7e(40);
        Func_02003b1c(1, angle_6000, 40);
        Func_02003b26(1, angle_2000, 20);
        Func_02003b30(1, angle_6000, 20);
        Func_02003b3a(1, angle_2000, 10);
    }
    Func_02004050(1, 2, 0);
    Func_02003fb6(40);
    Func_02004060(1, 2, 0);
    Func_02003fc6(10);
    Func_02004070(1, 4, 0);
    Func_02003fd6(20);
    Func_02003b5a(1, 20);

    if (Data_0200b69c != 0) {
        Func_02004126(3, 256, 0);
        Func_02004024(60);
        Func_020040dc(3, 2);
        Func_02004032(80);
        Func_0200413a(3, 2);
        {
            u8 *r_ = Func_02004068(3);
            r_[0x23] = r_[0x23] | 1;
        }
        Func_02004036(Func_02004078(3), 1);
        Func_02004100(3, 4, 0);
        Func_020040e4(3, -2, 0);
        Func_02004104(3, 1);
        Func_02003c14(3, 0xe000, 60);
        Func_02004138(3, 2);
        Func_0200408e(20);
        Func_02003c12(3, 20);
    } else {
        ADVANCE_SCENE_COUNTER();
    }

    {
        s32 angle_2000;
        s32 angle_4000;
        s32 angle_6000;

        angle_4000 = 0x80;
        Func_02004156(1, 2, 0);
        angle_4000 <<= 7;
        angle_2000 = 0x80;
        angle_2000 <<= 6;
        Func_02003c56(1, angle_4000, 20);
        Func_02004164(1, 3);
        Func_02003c6a(1, angle_2000, 10);
        Func_02003c5a(1, 20);
        Func_0200417e(1, 3);
        Func_020040ec(10);
        Func_020041a4(2, 1);
        Func_020040fa(40);
        Func_020041b2(2, 2);
        Func_02004108(20);
        Func_02004210(2, 2);
        {
            u8 *r_ = Func_0200413e(2);
            r_[0x23] = r_[0x23] | 1;
        }
        Func_0200410c(Func_0200414e(2), 1);
        Func_020041d6(2, 4, 0);
        Func_020041ce(2, 1);
        Func_02004242(2, 0xc000, 0);
        Func_02004202(0, 2);
        Func_02004158(10);
        Func_02004260(0, 2);
        {
            u8 *r_ = Func_0200418e(0);
            r_[0x23] = r_[0x23] | 1;
        }
        Func_0200415a(Func_0200419c(0), 1);
        angle_6000 = 0x6000;
        Func_02004226(0, 4, 0);
        Func_02004220(0, 1);
        Func_02003d2e(0, angle_6000, 60);
        Func_020042ac(0, 0x105, 0);
        Func_020042b6(2, 0x105, 0);
        Func_020041b4(60);
        Func_02003d54(0, 0xa000, 20);
        Func_02004260(1, 3);
        Func_02004268(0, 3);
        Func_02003d6e(0, angle_6000, 10);
        Func_02003d78(1, angle_4000, 10);
        Func_020042e8(2, 0xc000, 0);
        Func_02004290(2, 3);
        Func_02003d7c(2, 20);
        Func_02004298(0, 3);
        Func_020042a0(3, 3);
        Func_020042b0(1, 3);
        Func_0200421e(20);
        Func_020042d6(2, 1);
        Func_0200422c(20);
        Func_02004314(2, 0);
        Func_020042cc(2, 3);
        Func_02004340(2, 0xe000, 0);
        Func_0200434a(1, angle_2000, 0);

        if (Func_0200427a(0, 0) == 0) {
            Func_020042f6(2, 3);
            Func_02004306(1, 3);
            ADVANCE_SCENE_COUNTER();
        } else {
            Func_02004338(1, 2);
            Func_02003e26_b(1, angle_2000, 10);
            Func_02004342(1, 2);
            Func_0200438a(1, 0);
        }
    }

    Func_02003e42_b(1, 0x4000, 10);
    Func_0200434e(1, 4);
    Func_02003e3a(1, 20);
    Func_02003e5e(2, 0xc000, 10);
    Func_0200436a(2, 3);
    Func_02003e56(2, 10);

    if (Data_0200b69c != 0) {
        Func_0200439a(3, 2);
        Func_02003e88_b(3, 0, 20);
        Func_02003e94(3, 0x2000, 10);
        Func_02004398(3, 4);
        Func_02003e8c(3, 10);
    } else {
        ADVANCE_SCENE_COUNTER();
    }

    {
        s32 angle_2000;
        s32 angle_4000;
        s32 angle_6000;
        s32 angle_a000;

        angle_2000 = 0x2000;
        angle_a000 = 0xa000;
        Func_02004436(1, angle_2000, 0);
        Func_02003edc(0, angle_a000, 10);
        Func_020043e0(0, 3);
        Func_020043f0(1, 3);
        Func_0200435e(20);
        angle_4000 = 0x80;
        angle_6000 = 0x6000;
        Func_02004466(0, angle_6000, 0);
        angle_4000 <<= 7;
        Func_02003f0c_b(1, angle_4000, 10);
        Func_02004418(2, 4);
        Func_02004386(20);
        Func_0200449a(0, 258, 0);
        Func_020044a6(1, 258, 80);
        Func_02003f3e(2, 0xe000, 10);
        Func_0200445a(2, 2);
        Func_02003f36(2, 20);
        Func_020044be(1, 0x2000, 0);
        Func_02003f64(0, angle_a000, 40);
        Func_020044d2(1, angle_4000, 0);
        Func_02003f7a(0, 0x6000, 10);
        Func_02003f86(2, 0xc000, 10);
        Func_02004492(2, 3);
        Func_02003f7e_b(2, 10);
        Func_0200451c(1, 258);
        Func_02004412(40);
        Func_02003f96(1, 20);
        Func_020044ba(2, 3);
        Func_02004428(20);
        Func_02004542(1, 258);
        Func_02004438(40);
        Func_02003fbc(1, 20);
        Func_020044d8(2, 3);
        Func_02003fcc(2, 10);
        Func_02004508(1, 2);
        Func_02003ff8(1, 0x2000, 10);
        Func_0200454c(1, 0);
        Func_0200456e(0, angle_a000, 0);
    }

    if (Func_0200449e(0, 0) == 0) {
        Func_02004522(1, 3);
    } else {
        Func_02004498(20);
        Func_02004550(1, 2);
        Func_020044a6(40);
        ADVANCE_SCENE_COUNTER();
    }

    Func_020045a6(1, 0);
    Func_02004664(21);
    Func_02004604(0x00406218, 1);
    Func_02004612(60);
    Func_02004448(60);

    Data_0200b690 = 0;
    Data_0200b68c = 0x00800000;
    Data_0200b694 = 1;
    Func_0200446e(Func_020027ac, 3200);
    Func_020044fc(80);
    {
        s32 angle_c000;

        Func_020045ac(0, 2);
        Func_020045b4(1, 2);
        Func_020045bc(3, 2);
        Func_020045ce(2, 2);
        angle_c000 = 0xc000;
        Func_02004526(60);
        Func_020040c4(2, angle_c000, 10);
        Func_0200460e(0x149d);
        Func_020040ba(2, 10);
        Func_020040dc_b(1, angle_c000, 10);
        Func_020040e6(0, angle_c000, 10);
        if (Data_0200b69c != 0) {
            Func_020040f8(3, angle_c000, 10);
        }
    }

    {
        u8 *r_ = Func_02004592(0);
        r_[0x23] = r_[0x23] & 0xfe;
    }
    {
        u8 *r_ = Func_020045a4(1);
        r_[0x23] = r_[0x23] & 0xfe;
    }
    {
        u8 *r_ = Func_020045b4_b(2);
        r_[0x23] = r_[0x23] & 0xfe;
    }
    {
        u8 *r_ = Func_020045c4(3);
        r_[0x23] = r_[0x23] & 0xfe;
    }
    Func_020046ac(0, 3);
    Func_020046b4(1, 3);
    Func_020046bc(2, 3);
    Func_020046c4(3, 3);

    Data_0200b694 = 2;
    Func_0200476e(220);
    Func_02004664(19, 0x00780000, 0x00f80000);
    Func_02004626(19, Data_0200ad20);
    Func_0200467c(20, 0x00640000, 0x01120000);
    Func_0200463c(20, Data_0200ad20);
    if (Data_0200b69c != 0) {
        Func_02004698(21, 0x004a0000, 0x00fe0000);
        Func_02004658(21, Data_0200ad20);
    }
    Func_020046ae(22, 0x005e0000, 0x00e10000);
    Func_0200466e(22, Data_0200ad20);
    Func_02004634(120);

    Data_0200b694 = 3;
    while (Data_0200b694 != 0) {
        Func_020045b0(1);
    }

    Func_020041ca(17, 80);
    Func_020041d2(18, 20);
    Func_02004768(0, 0x101, 0);
    Func_02004772(1, 0x101, 0);
    Func_0200477c(2, 0x101, 0);
    Func_02004786(3, 0x101, 0);
    Func_02004684(60);
    Func_02004208(18, 20);
    Func_02004724(0, 3);
    Func_0200472c(1, 3);
    Func_02004734(3, 3);
    Func_02004744(2, 3);
    Func_02004230(18, 10);
    Func_0200474c(0, 3);
    Func_02004754(1, 3);
    Func_0200475c(3, 3);
    Func_0200476c(2, 3);
    Func_02004258(18, 10);
    Func_020047f0(0, 256, 0);
    Func_020047fc(1, 256, 0);
    Func_02004808(2, 256, 0);
    Func_02004814(3, 256, 0);
    Func_02004712(40);
    Func_02004296(17, 10);
    Func_0200481e(0, 0x8000, 0);
    {
        s32 angle_c000 = 0xc0;
        s32 angle_e000;

        angle_c000 <<= 8;

        Func_0200482e(1, 0x4000, 0);
        Func_02004838(3, 0, 0);
        Func_020042de(2, angle_c000, 40);
        Func_020042ce(18, 10);
        Func_0200486c(0, 258);
        Func_02004876(1, 258);
        Func_02004880(2, 258);
        Func_0200488a(3, 258);
        Func_0200487c(0, angle_c000, 0);
        Func_02004886(1, angle_c000, 0);
        Func_02004890(2, angle_c000, 0);
        Func_02004336(3, angle_c000, 80);
        Func_02004326(18, 10);
        Func_020048ae(0, 0x8000, 0);
        Func_020048ba(1, 0x4000, 0);
        Func_020048c4(2, angle_c000, 0);
        Func_0200436a_b(3, 0, 40);
        Func_0200435a(17, 10);
        Func_020048e0(0, angle_c000, 0);
        Func_020048ea(1, angle_c000, 0);
        Func_020048f4(2, angle_c000, 0);
        Func_0200439a_b(3, angle_c000, 10);
        Func_0200489e(0, 4);
        Func_020048a6(1, 4);
        Func_020048ae(3, 4);
        Func_020048be(2, 4);
        Func_0200482c(60);
        Func_020043b0(18, 10);
        Func_020048cc(0, 3);
        Func_020048d4(1, 3);
        Func_020048dc(3, 3);
        Func_020048ec(2, 3);
        Func_020043d8(18, 20);
        Func_02004960(0, 0x8000, 0);
        Func_0200496c(1, 0x4000, 0);
        Func_02004976(2, angle_c000, 0);
        Func_0200441c(3, 0, 20);
        Func_0200440c(18, 10);

        Func_0200496c(0, 2);
        Func_02004974(1, 2);
        Func_0200497c(3, 2);
        Func_0200498c(2, 2);
        Func_020049de(0, angle_c000, 0);
        Func_020049e8(1, angle_c000, 0);
        Func_020049f2(2, angle_c000, 0);
        Func_020049fc(3, angle_c000, 0);
        Func_02004488(18, 20);
        Func_02004a10(0, 0x8000, 0);
        Func_02004a1c(1, 0x4000, 0);
        Func_02004a26(2, angle_c000, 0);
        Func_020044cc(3, 0, 20);
        Func_020044bc(17, 20);
        Func_02004a5a(0, 258);
        Func_02004a64(1, 258);
        Func_02004a6e(3, 258);
        Func_02004a78(2, 258);
        Func_0200496e(40);
        Func_020044f2(18, 10);
        Func_02004a78(0, angle_c000, 0);
        Func_02004a82(1, angle_c000, 0);
        Func_02004a8c(2, angle_c000, 0);
        Func_02004532(3, angle_c000, 10);
        Func_02004522_b(18, 10);
        Func_02004a3e(0, 3);
        Func_02004a46(1, 3);
        Func_02004a4e(3, 3);
        Func_02004a5e(2, 3);
        Func_020049cc(60);
        Func_02004abc(18, 0);
        Func_02004ac4(17, 0);

        Func_02004962(Func_020027ac);
        Func_020049e8(80);
        Func_02004b2a(0x10000, 1);
        Func_02004b38(60);
        Func_0200496e(80);
        {
            s32 *has_actor_late;

            Func_02004a54(19);
            Func_02004a5a(20);
            has_actor_late = &Data_0200b69c;
            Func_02004a62(21);
            Func_02004a68(22);
            Func_0200498e(1);
            Func_02004a68(19, Data_0200ad7c);
            Func_02004a70(20, Data_0200ad7c);
            if (*has_actor_late != 0) {
                Func_02004a7e(21, Data_0200ad7c);
            }
            Func_02004a9e(22, Data_0200ad7c);
        }
        Func_02004a4c(20);

        Func_02004b54(0, 2);
        Func_02004b5c(1, 2);
        Func_02004b64(2, 2);
        Func_02004b6c(3, 2);
        {
            u8 *r_ = Func_02004a9a(0);
            r_[0x23] = r_[0x23] | 1;
        }
        {
            u8 *r_ = Func_02004aaa(1);
            r_[0x23] = r_[0x23] | 1;
        }
        {
            u8 *r_ = Func_02004ab8(2);
            r_[0x23] = r_[0x23] | 1;
        }
        {
            u8 *r_ = Func_02004ac6(3);
            r_[0x23] = r_[0x23] | 1;
        }

        angle_e000 = 0xe000;
        Func_02004b62(2, 2);
        Func_02004650(2, angle_e000, 10);
        Func_02004ba4(2, 0);
        Func_02004bc8(1, 0x2000, 0);
        Func_02004bd2(3, 0, 0);

        if (Func_02004b02(0, 0) == 0) {
            Func_02004b9e(1, 2);
            Func_02004af4(10);
            Func_02004bdc(1, 0);
            if (Func_02004b24(0, 0) == 0) {
                Func_020046a6(3, 0, 20);
                Func_02004c24(1, 0x101, 0);
                Func_02004c2e(2, 0x101, 0);
                Func_02004c38(3, 0x101, 0);
                Func_02004b36(40);
                Func_020046d6(1, 0x4000, 20);
                Func_020046c6(1, 10);
                Func_020046e8(2, angle_c000, 20);
                Func_020046f2(2, angle_e000, 20);
                Func_02004bfe(2, 3);
                Func_020046ea(2, 20);
                Func_0200470e(1, 0x2000, 20);
            } else {
                Func_0200471a(3, 0, 20);
                Func_02004c9a(1, 258, 0);
                Func_02004ca6(2, 258, 0);
                Func_02004cb2(3, 258, 40);
                Func_0200474a(1, 0x4000, 20);
                Func_02004c94(0x14b4);
                Func_02004740(1, 20);
                Func_02004c64(2, 3);
                Func_02004750(2, 20);
            }
            Func_02004c6c(3, 3);
            Func_02004c7c(1, 3);
        } else {
            Func_02004bec(20);
            Func_02004c8c(1, 3);
            Func_02004bfa(10);
            Func_02004cd8(0x14b6);
            Func_02004784(1, 10);
            Func_02004d0c(1, 0x4000, 0);
            Func_020047b4(0, 0x6000, 20);
            Func_02004cb8(1, 3);
            Func_02004cc8(0, 3);
            Func_02004c36(10);
            Func_02004cd6(2, 4);
            Func_02004d26(2, 0);

            if (Func_02004c6e(0, 0) == 0) {
                Func_02004c5a(20);
                Func_02004d6c(2, 0x103, 0);
                Func_02004c6a(40);
                Func_02004808_b(2, angle_e000, 10);
                Func_020047f8(2, 10);
                if (Data_0200b69c != 0) {
                    Func_02004820(3, 0, 10);
                    Func_02004d3c(3, 3);
                    Func_02004818(3, 20);
                } else {
                    ADVANCE_SCENE_COUNTER();
                }
                Func_02004dc8(0, 258);
                Func_02004dd2(1, 258);
                Func_02004cc8(40);
                Func_02004d80(1, 2);
                Func_02004854(1, 20);
                Func_02004dea(1, 0x105, 0);
                Func_02004ce8(120);
                Func_0200486c_b(2, 40);
                if (Data_0200b69c != 0) {
                    Func_02004898(3, 0x2000, 10);
                    Func_02004da4(3, 4);
                    Func_02004890_b(3, 10);
                } else {
                    ADVANCE_SCENE_COUNTER();
                }
                Func_02004d2c(60);
                Func_02004de4(2, 2);
                if (Data_0200b69c != 0) {
                    Func_020048dc_b(2, 0xa000, 40);
                    Func_020048e8(2, 0xe000, 20);
                }
                Func_020048d8(2, 10);
                Func_02004e0c(0, 2);
                Func_02004e1c(1, 2);
                Func_02004d72(40);
                Func_020048f6(2, 20);
                Func_02004e1a(0, 3);
                Func_02004e22(1, 3);
                Func_02004d90(20);
                Func_02004e28(3, 3);
                Func_02004e38(2, 3);
            } else {
                Func_02004ed8(2, 0x105, 0);
                Func_02004dd6(40);
                Func_02004e76(2, 3);
                Func_02004ebc(0x14bf);
                Func_02004968(2, 20);
                if (Data_0200b69c != 0) {
                    Func_02004990(3, 0, 10);
                    Func_02004eac(3, 1);
                    Func_02004988(3, 20);
                } else {
                    ADVANCE_SCENE_COUNTER();
                }
                Func_02004f38(1, 258);
                Func_02004f42(0, 258);
                Func_02004e38(40);
                Func_02004ef0(1, 2);
                Func_020049c4(1, 20);
                Func_02004f5a(2, 0x105, 0);
                Func_02004e58(80);
                Func_020049dc(2, 40);
                if (Data_0200b69c != 0) {
                    Func_02004a08(3, 0x2000, 20);
                    Func_02004f0c(3, 4);
                    Func_02004a00(3, 40);
                } else {
                    ADVANCE_SCENE_COUNTER();
                }
                Func_02004f4e(2, 2);
                Func_02004ea4(20);
                Func_02004a28(2, 20);
                Func_02004f5c(1, 2);
                Func_02004f6c(0, 2);
                Func_02004ec2(40);
                Func_02004a46_b(2, 20);
            }
        }
    }

    Func_02005070(17);
    Func_02004f12(1, 0x13333, 0x9999); /* 1.2 / 0.6 */
    Func_02004f1c(2, 0x13333, 0x9999);
    Func_02004f26(3, 0x13333, 0x9999);

    Func_02004f86(1, 2);
    record = Func_02004f24(0);
    if (record != 0) {
        Func_02004f6e(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Func_02004f94(1);
    Func_02004fa6(1, 0, 0);

    Func_02004fb6(2, 2);
    record = Func_02004f54(0);
    if (record != 0) {
        Func_02004f9e(2, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Func_02004fc4(2);
    Func_02004fd6(2, 0, 0);

    Func_02004fe0(17, 0, 0);
    Func_02004fea(18, 0, 0);

    if (Data_0200b69c != 0) {
        Func_02005002(3, 2);
        record = Func_02004fa0(0);
        if (record != 0) {
            Func_02004fea(3, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Func_02005010(3);
        Func_02005022(3, 0, 0);
    }

    Func_02004f98(0x843);
    Func_0200512c();
    Func_02004fb8();
}
