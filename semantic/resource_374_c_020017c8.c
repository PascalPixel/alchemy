#include "types.h"
#define NULL ((void *)0)

/*
 * Resource 374 cutscene step at 0x020017c8 (2816 bytes, 293 call sites).
 *
 * Complete owner: prologue `push {r5, r6, r7, lr}` plus the high-register
 * saves (`fp, sl, r9` then `r8`) and `sub sp, #8` at 0x020017c8, and the
 * single interworking return at 0x0200229a restoring sp, r8-fp and
 * `pop {r0} ; bx r0`.  r0 holds the popped return address, so the owner is
 * `void`.  The body is entirely straight-line apart from one conditional
 * (0x02001b58) and the two `b.n` hops over interior literal pools.
 *
 * Three literal pools sit inside the span and are branched over, not executed:
 * 0x02001bf4-0x02001c3b (jumped by `b.n 0x02001c3c` at 0x02001bf2),
 * 0x02002082-0x020020a3 (jumped by `b.n 0x020020a4` at 0x02002080) and
 * 0x020022ac-0x020022c7 after the epilogue.  Each is reached only through
 * `ldr rN, [pc, #imm]`.  The next prologue at 0x020022c8 is the byte-exact
 * `assets/code/resource_374_c_020022c8.c`.
 *
 * The eight-byte stack frame carries the fifth and sixth arguments of the one
 * six-argument import, Func_08009180 at 0x020017f2 (`str r3, [sp, #0]` /
 * `str r2, [sp, #4]`), so it is spelled with six arguments here.
 *
 * Link base 0x02008000: the six in-image pool words 0x0200ac00, 0x0200ac14,
 * 0x0200ac90, 0x0200acf8, 0x0200ad74 and 0x0200adf0 are file offsets 0x2c00,
 * 0x2c14, 0x2c90, 0x2cf8, 0x2d74 and 0x2df0.  All are even, so they are data
 * (animation scripts, the same family 0x02000940, 0x02001274 and 0x02000bbc
 * pass to Func_0808a098 / Func_0808a0b0 / Func_0808a168), not Thumb entries.
 *
 * Call targets resolved with `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml --`, never from the
 * disassembler's `bl` annotations: 232 import veneers and 61 prologues in this
 * overlay, the latter all the byte-exact 0x020022c8 (22 sites) and 0x020022e0
 * (39 sites).  39 distinct callees for 293 sites.
 *
 * Long-lived register aliases recovered from the assembly, because the shift
 * chains that build them are far from their uses:
 *   r7 = 0x3000, fp = 0x1000, r9 = 0x5000 (fixed for the whole owner);
 *   r8 = 0x7000 until 0x02001d2a, then 0xb000;
 *   r6 = 0 at 0x02001cb4, then 0x9000 from 0x02001d00;
 *   r5 is reused for four unrelated values (0x0200ac00, the mask 254,
 *   0x0200ac90, 0xd000, 1, 0x0200adf0) and is tracked per use below;
 *   sl = 0xc000 from 0x02001d1a.
 * They are spelled as literals at each call site rather than as variables.
 *
 * Uncertainties:
 *  - Func_0808a010 appears at 30 sites with 10/20/40/60/80, consistent with a
 *    frame count but not asserted;  Func_0808a100 at 33 sites and
 *    Func_0808a128 at 22 always take a small mode number, so old-style
 *    declarations are used for every import;
 *  - the record returned by Func_0808a080 is touched only at +8/+16 (words,
 *    once) and at +0x23 (a byte whose bit 0 is cleared for handles 30/31 and
 *    set for handles 0/1), so byte offsets are used rather than a struct;
 *  - at 0x02002266 the workspace words written are at +448 (value 32) and
 *    +232 (value 60).  The second offset is built by `subs r3,#192` followed
 *    by `adds r3,#200` on the same register, i.e. 448-192+200 = 456 would be
 *    wrong: r3 is 32 when it is stored and 232 when it is used as the second
 *    displacement.  That reading is deliberate and is the only non-obvious
 *    arithmetic in the owner.
 */

/* Workspace pointer global. */
extern u8 *Data_03001ebc;

/* In-image data under the 0x02008000 link base. */
extern u8 Data_02002c00[];
extern u8 Data_02002c14[];
extern u8 Data_02002c90[];
extern u8 Data_02002cf8[];
extern u8 Data_02002d74[];
extern u8 Data_02002df0[];

/* Overlay-local callees (byte-exact siblings in assets/code). */
void Func_02003e68();
void Func_02003e7c();
void Func_02003e38();
void Func_02003fb8();
void Func_02003e3c();
void Func_02003dfa();
u8 *Func_02003ed8();
void Func_02003e6e();
u8 *Func_02003ee4();
void Func_02003e7a();
u8 *Func_02003ef0();
void Func_02003e86();
u8 *Func_02003efc();
void Func_02003e92();
u8 *Func_02003f08();
void Func_02003e9e();
u8 *Func_02003f14();
void Func_02003eaa();
void Func_02003ff2();
void Func_02003ffa();
void Func_02004002();
void Func_0200400a();
void Func_02004012();
void Func_02003f9e();
void Func_020040a2();
void Func_020040b6();
void Func_02003f2c();
void Func_0200403e();
void Func_02003b92(s32, s32, s32);
void Func_0200401c();
void Func_02003b88(s32, s32);
void Func_02004068();
void Func_02003bbc(s32, s32, s32);
void Func_02003bac(s32, s32);
void Func_02004010();
void Func_02003f86();
void Func_02004036();
void Func_02003bca(s32, s32);
void Func_020040aa();
void Func_02003fe8();
void Func_0200402a();
void Func_02003c10(s32, s32, s32);
void Func_02003c00(s32, s32);
void Func_02004054();
void Func_02003c3c(s32, s32, s32);
void Func_02004080();
void Func_02003c34(s32, s32);
void Func_02003c56(s32, s32, s32);
void Func_020040ba();
void Func_020040fa();
void Func_02004020();
void Func_02004062();
void Func_020040b8();
void Func_02003f96();
void Func_020040ce();
void Func_0200408e();
void Func_02004128();
void Func_02004132();
void Func_020040c6();
void Func_020040cc();
void Func_02004082();
void Func_02004194();
void Func_0200419e();
void Func_02004198();
void Func_02003cf6(s32, s32, s32);
void Func_02003d06(s32, s32, s32);
void Func_02003d10(s32, s32, s32);
void Func_0200415c();
void Func_020040d2();
void Func_02003d28(s32, s32, s32);
void Func_02003d32(s32, s32, s32);
void Func_02003d3c(s32, s32, s32);
void Func_02004202();
void Func_020041fc();
void Func_0200417e();
void Func_02004114();
void Func_02003d6a(s32, s32, s32);
void Func_02003d74(s32, s32, s32);
void Func_02003d7e(s32, s32, s32);
void Func_02004244();
void Func_0200424e();
void Func_020041c0();
void Func_02004156();
void Func_02003dac(s32, s32, s32);
void Func_02003db6(s32, s32, s32);
void Func_02003dc0(s32, s32, s32);
void Func_02004286();
void Func_02003dd4(s32, s32, s32);
void Func_02003dc4(s32, s32);
void Func_02004228();
void Func_02003dd4_b(s32, s32);
void Func_020042b4();
void Func_02003e02(s32, s32, s32);
void Func_02003e0c(s32, s32, s32);
void Func_02003e16(s32, s32, s32);
void Func_02003e20(s32, s32, s32);
void Func_0200426c();
void Func_02003e18(s32, s32);
u8 *Func_0200421a();
void Func_02004278();
u8 *Func_02004234();
u8 *Func_02004246();
void Func_02004326();
void Func_0200432e();
void Func_02004270();
void Func_020042c8();
void Func_0200428a();
void Func_02004250();
void Func_020042e0();
void Func_0200429a();
void Func_020042c2();
void Func_02004320();
void Func_02003ece(s32, s32, s32);
void Func_02003f08_b(s32, s32);
void Func_02004384();
void Func_020042e2();
void Func_02003f38(s32, s32, s32);
void Func_02003f30(s32, s32);
void Func_0200438c();
void Func_0200430a();
void Func_020043a2();
void Func_02004318();
void Func_0200435a();
void Func_02004364();
void Func_02004374();
void Func_0200433a();
void Func_0200445a();
u8 *Func_02004476();
void Func_0200447c();
void Func_02004362();
void Func_020043aa();
void Func_020043b8();
void Func_02003fc6(s32, s32, s32);
void Func_0200442a();
void Func_02004388();
void Func_02003fc6_b(s32, s32);
void Func_02004496();
void Func_02003ff4(s32, s32, s32);
void Func_02004004(s32, s32, s32);
void Func_02004014(s32, s32, s32);
void Func_02004458();
void Func_020043d6();
void Func_0200446e();
void Func_020043e4();
void Func_02004494();
void Func_020043f2();
void Func_0200402e(s32, s32);
void Func_02004440();
void Func_02004484();
void Func_02004518();
void Func_02004448();
void Func_020044a6();
void Func_0200453a();
void Func_02004556();
void Func_020040a6(s32, s32, s32);
void Func_02004096(s32, s32);
void Func_020044fa();
void Func_020040a6_b(s32, s32);
void Func_02004574();
void Func_0200458e();
void Func_02004528();
void Func_02004542();
void Func_020044a2();
void Func_020040f8(s32, s32, s32);
void Func_020040e8(s32, s32);
void Func_020045c8();
void Func_020045d4();
void Func_020045e0();
void Func_020045ec();
void Func_020045f8();
void Func_02004604();
void Func_02004138(s32, s32);
void Func_020046bc();
void Func_020046c4();
void Func_02004590();
void Func_02004624();
void Func_02004168(s32, s32);
void Func_0200455c();
void Func_02004592();
void Func_02004598();
void Func_020045c6();
void Func_0200465a();
void Func_02004612();
void Func_020041c0_b(s32, s32, s32);
void Func_020041b0(s32, s32);
void Func_0200467e();
void Func_020041dc(s32, s32, s32);
void Func_020046a2();
void Func_0200464a();
void Func_020041de(s32, s32);
void Func_02004202_b(s32, s32, s32);
void Func_020046c8();
void Func_02004658();
void Func_02004204(s32, s32);
void Func_02004668();
void Func_020046c0();
void Func_020046e2();
void Func_0200467a();
void Func_020046da();
void Func_020046ea();
void Func_020046aa();
void Func_02004628();
void Func_020046b8();
void Func_02004646();
void Func_020046d6();
void Func_02004654();
void Func_020046e4();
void Func_020046ec();
void Func_020046fc();
void Func_0200470e();
void Func_02004718();
void Func_020042bc(s32, s32);
void Func_0200478a();
void Func_020042e8(s32, s32, s32);
void Func_020042f2(s32, s32, s32);
void Func_020042e2_b(s32, s32);
void Func_02004304(s32, s32, s32);
void Func_0200430e(s32, s32, s32);
void Func_02004752();
void Func_0200475a();
void Func_02004762();
void Func_0200476a();
void Func_02004772();
void Func_0200477a();
void Func_02004782();
void Func_02004700();
void Func_02004790();
void Func_02004798();
void Func_020047c4();
void Func_020047cc();
void Func_020047d4();
void Func_020047dc();
void Func_020047ea();
void Func_020047f2();
void Func_020047fa();
void Func_02004802();
void Func_0200480a();
void Func_0200481a();
void Func_02004832();
void Func_0200483c();
void Func_02004846();
void Func_02004850();
void Func_0200485a();
void Func_02004864();
void Func_0200486e();
void Func_02004878();
void Func_02004882();
void Func_0200488c();
void Func_02004896();
void Func_020048a0();
void Func_020048aa();
void Func_020048b4();
void Func_020048be();
void Func_020048c8();
void Func_020048d2();
void Func_020048dc();
void Func_020048e6();
void Func_02004826();
void Func_0200485c();
u8 *Func_02004892();
u8 *Func_020048a2();
void Func_0200498e();
void Func_0200499a();
void Func_020044ea(s32, s32, s32);
void Func_020044f4(s32, s32, s32);
void Func_020048ea();
void Func_020048fc();
void Func_020048c2();
void Func_020049e2();
void Func_020049fa();
void Func_020048e0();
void Func_02004922();
void Func_02004932();
void Func_020048f8();
void Func_02004a9a();
void Func_02004aa6();
void Func_0200496c();
void Func_02004972();
void Func_02004a60();
void Func_0200493c();

/* Main-image imports reached through this overlay's veneer table.
 * Old-style declarations: one name may carry more than one interface. */
                               /* frame wait */
                     
                               /* six arguments, last two stack-carried */

                     

                     

                     

                     

                     

                     

                     

                     

                     

                     

                     

void Func_020017c8(void)
{
    u8 *workspace;
    u8 *record;

    Func_02003e68(1);
    Func_02003e7c();
    Func_02003e38(42, 53, 42, 54, 3, 1);
    Func_02003fb8(0x00b40000, 0x00100000, 0x026a0000, 0);
    Func_02003e3c();
    Func_02003dfa(1);

    Func_02003e6e(Func_02003ed8(22), 0);
    Func_02003e7a(Func_02003ee4(23), 0);
    Func_02003e86(Func_02003ef0(24), 0);
    Func_02003e92(Func_02003efc(25), 0);
    Func_02003e9e(Func_02003f08(26), 0);
    Func_02003eaa(Func_02003f14(29), 0);

    Func_02003ff2(0, 1);
    Func_02003ffa(1, 1);
    Func_02004002(17, 1);
    Func_0200400a(16, 1);
    Func_02004012(15, 1);

    Func_02003f9e(0, 0x00d00000, 0x032e0000);
    Func_020040a2();
    Func_020040b6();
    Func_02003f2c(80);

    Func_0200403e(12, 0x101, 40);
    Func_02003b92(12, 0x7000, 20);
    Func_0200401c(0x11fa);
    Func_02003b88(12, 10);
    Func_02004068(11, 258, 20);
    Func_02003bbc(11, 0x1000, 10);
    Func_02003bac(11, 10);
    Func_02004010(12, 3);
    Func_02003f86(10);
    Func_02004036(11, 2);
    Func_02003bca(11, 10);
    Func_020040aa(12, 256, 40);

    Func_02003fe8(12, 0x0000cccc, 0x00006666);
    Func_0200402a(12, 184, 0x26a);
    Func_02003c10(12, 0x3000, 60);
    Func_02003c00(12, 20);
    Func_02004012(11, 0x10000, 0x8000);
    Func_02004054(11, 168, 0x26a);
    Func_02003c3c(11, 0xf000, 10);
    Func_02004080(11, 4);
    Func_02003c34(11, 20);
    Func_02003c56(12, 0x7000, 10);
    Func_020040ba(12, 1);
    Func_020040fa(12, 0);
    Func_02004020(10);

    Func_02004062(30, 0x00026666, 0x00013333);
    Func_020040b8(30, 0x006e0000, 0x02e80000);
    Func_02003f96(2);
    Func_020040ce(30, 3);
    Func_0200408e(30, Data_02002c14);
    Func_02004054(40);
    Func_02004128(11, 0x0001001e, Data_02002c00);
    Func_02004132(12, 0x0001001e, Data_02002c00);
    Func_020040b8(30);
    Func_020040c6(11);
    Func_020040cc(12);
    Func_02004082(60);

    Func_02004194(11, 0x105, 0);
    Func_0200419e(12, 0x105, 120);
    Func_02004198(11, 0x1000, 0);
    Func_02003cf6(12, 0x7000, 80);
    Func_02003d06(11, 0x5000, 40);
    Func_02003d10(11, 0x1000, 20);
    Func_0200415c(11, 3);
    Func_020040d2(20);
    Func_02003d28(12, 0x5000, 60);
    Func_02003d32(12, 0x3000, 40);
    Func_02003d3c(12, 0x5000, 60);
    Func_02004202(12, 0x101, 80);
    Func_020041fc(11, 0x3000, 0);
    Func_0200417e(12, 184, 0x276);
    Func_02004114(20);
    Func_02003d6a(12, 0x3000, 20);
    Func_02003d74(12, 0x5000, 20);
    Func_02003d7e(12, 0x3000, 20);
    Func_02004244(12, 0x101, 40);
    Func_0200424e(11, 0x101, 40);
    Func_020041c0(11, 168, 0x276);
    Func_02004156(20);
    Func_02003dac(11, 0x3000, 40);
    Func_02003db6(11, 0x5000, 40);
    Func_02003dc0(11, 0x3000, 40);
    Func_02004286(11, 0x101, 40);
    Func_02003dd4(11, 0x1000, 10);
    Func_02003dc4(11, 20);
    Func_02004228(12, 3);
    Func_02003dd4_b(12, 10);
    Func_020042b4(11, 256, 20);
    Func_02003e02(11, 0x5000, 20);
    Func_02003e0c(11, 0x3000, 20);
    Func_02003e16(11, 0x5000, 20);
    Func_02003e20(11, 0x5000, 60);
    Func_0200426c(11, 3);
    Func_02003e18(11, 10);

    record = Func_0200421a(30);
    if (record != NULL) {
        Func_02004278(31, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Func_02004156(2);

    record = Func_02004234(30);
    record[0x23] &= 0xfe;
    record[0x23] &= 0xfe;
    record = Func_02004246(31);

    Func_02004326(30, 2);
    Func_0200432e(31, 2);
    Func_02004270(31, 0x00039999, 0x0001cccc);
    Func_020042c8(31, 2);
    Func_0200428a(31, Data_02002c90);
    Func_02004250(20);
    Func_020042e0(30, 3);
    Func_0200429a(30, 0x0004cccc, 0x00026666);
    Func_020042c2(30, Data_02002c90);
    Func_02004270(60);
    Func_02004320(12, 2);
    Func_02003ece(12, 0x7000, 10);
    Func_02003f08_b(12, 10);
    Func_02004384(11, 2);
    Func_020042e2(20);
    Func_02003f38(11, 0x1000, 10);
    Func_02004384(11, 3);
    Func_02003f30(11, 20);
    Func_0200438c(12, 3);
    Func_0200430a(10);
    Func_020043a2(11, 3);
    Func_02004318(20);

    Func_0200435a(11, 0x00026666, 0x00013333);
    Func_02004364(12, 0x00026666, 0x00013333);
    Func_02004374(11, Data_02002cf8);
    Func_0200433a(10);
    Func_0200445a(0x00026666, 0x00004ccc);

    record = Func_02004476();
    record[0x55] = 0;

    Func_0200447c(0x00d70000, 0x00100000, 0x03210000, 1);
    Func_02004362(10);
    Func_020043aa(12, Data_02002d74);
    Func_020043b8(12);
    Func_02003fc6(12, 0x3000, 120);
    Func_0200442a(13, 2);
    Func_02004388(20);
    Func_02003fc6_b(13, 20);

    Func_02004496(0, 0, 0);
    Func_02003ff4(1, 0x9000, 20);
    Func_02004004(0, 0xc000, 10);
    Func_02004014(1, 0xb000, 10);
    Func_02004458(0, 3);
    Func_020043d6(10);
    Func_0200446e(1, 3);
    Func_020043e4(40);
    Func_02004494(16, 2);
    Func_020043f2(20);
    Func_0200402e(16, 10);

    Func_02004440(16, 0x10000, 0x8000);
    Func_02004484(16, 216, 800);
    Func_02004518(16, 0x4000, 0);
    Func_02004448(180, 0);
    Func_020044a6(16, 264, 800);
    Func_0200453a(16, 0x6000, 0);
    Func_02004556(1, 258, 40);
    Func_020040a6(1, 0xf000, 10);
    Func_02004096(1, 10);
    Func_020044fa(17, 4);
    Func_020040a6_b(17, 10);
    Func_02004574(1, 0x1000, 0);
    Func_0200458e(1, 0x103, 20);
    Func_02004528(1, 4, 60);
    Func_02004542(14, 2);
    Func_020044a2(20);
    Func_020040f8(14, 0xd000, 10);
    Func_020040e8(14, 60);

    Func_020045c8(1, 258, 0);
    Func_020045d4(16, 258, 0);
    Func_020045e0(17, 258, 0);
    Func_020045ec(18, 258, 0);
    Func_020045f8(19, 258, 80);
    Func_02004604(17, 256, 0);
    Func_02004138(17, 60);
    Func_020046bc(0, 17);
    Func_020046c4(1, 17);
    Func_02004590(17, 216, 800);
    Func_02004624(17, 0x4000, 0);
    Func_02004168(17, 60);
    Func_0200455c(207, 0);
    Func_02004592(0);
    Func_02004598(1);
    Func_020045c6(17, 272, 816);
    Func_0200465a(17, 0x8000, 0);
    Func_02004612(1, 2);
    Func_020041c0_b(1, 0x9000, 10);
    Func_020041b0(1, 10);
    Func_0200467e(14, 0x3000, 0);
    Func_020041dc(0, 0, 10);
    Func_020046a2(0, 0x101, 60);
    Func_0200464a(16, 1);
    Func_020041de(16, 10);
    Func_02004202_b(1, 0xf000, 10);
    Func_020046c8(1, 0x101, 20);
    Func_02004658(16, 4);
    Func_02004204(16, 10);
    Func_02004668(18, 3);
    Func_020046c0(18, 0);
    Func_020046e2(1, 0xd000, 0);
    Func_0200467a(18, 4);
    Func_020046da(18, 0);
    Func_020046a2(18, 3);
    Func_020046ea(18, 0);
    Func_020046a2(16, 3);

    Func_020046a2(19, 3);
    Func_020046aa(17, 3);
    Func_02004628(10);
    Func_020046b8(24, 3);
    Func_020046c0(18, 3);
    Func_020046c8(27, 3);
    Func_02004646(10);
    Func_020046d6(28, 3);
    Func_02004654(10);
    Func_020046e4(25, 3);
    Func_020046ec(20, 3);
    Func_020046fc(21, 3);

    Func_0200470e(15, 2, 10);
    Func_02004718(15, 4, 40);
    Func_020042bc(15, 10);
    Func_0200478a(1, 0xb000, 0);
    Func_020042e8(0, 0xc000, 20);
    Func_020042f2(15, 0xd000, 10);
    Func_020042e2_b(15, 10);
    Func_02004304(15, 0x9000, 20);
    Func_0200430e(15, 0x5000, 10);

    Func_02004752(11, 3);
    Func_0200475a(14, 3);
    Func_02004762(17, 3);
    Func_0200476a(20, 3);
    Func_02004772(23, 3);
    Func_0200477a(26, 3);
    Func_02004782(29, 3);
    Func_02004700(10);
    Func_02004790(12, 3);
    Func_02004798(15, 3);
    Func_020047c4(18, 3);
    Func_020047cc(21, 3);
    Func_020047d4(24, 3);
    Func_020047dc(27, 3);
    Func_0200475a(10);
    Func_020047ea(13, 3);
    Func_020047f2(16, 3);
    Func_020047fa(19, 3);
    Func_02004802(22, 3);
    Func_0200480a(25, 3);
    Func_0200481a(28, 3);
    Func_02004790(80);

    Func_02004832(11, 4, 0);
    Func_0200483c(14, 4, 0);
    Func_02004846(17, 4, 0);
    Func_02004850(20, 4, 0);
    Func_0200485a(23, 4, 0);
    Func_02004864(26, 4, 0);
    Func_0200486e(29, 4, 0);
    Func_02004878(12, 4, 0);
    Func_02004882(15, 4, 0);
    Func_0200488c(18, 4, 0);
    Func_02004896(21, 4, 0);
    Func_020048a0(24, 4, 0);
    Func_020048aa(27, 4, 0);
    Func_020048b4(13, 4, 0);
    Func_020048be(16, 4, 0);
    Func_020048c8(19, 4, 0);
    Func_020048d2(22, 4, 0);
    Func_020048dc(25, 4, 0);
    Func_020048e6(28, 4, 0);

    Func_02004826(0x1214, 1);
    Func_0200485c(80);

    record = Func_02004892(0);
    record[0x23] |= 1;
    record = Func_020048a2(1);
    record[0x23] |= 1;

    Func_0200498e(0, 258, 0);
    Func_0200499a(1, 258, 80);
    Func_020044ea(0, 0x4000, 10);
    Func_020044f4(1, 0x5000, 20);
    Func_020048ea(0, 0x0000cccc, 0x00006666);
    Func_020048fc(0, Data_02002df0);
    Func_020048c2(20);
    Func_020049e2(0x00006666, 0x00000ccc);
    Func_020049fa(0x00d80000, 0x00100000, 0x03890000, 1);
    Func_020048e0(20);
    Func_02004922(1, 0x0000cccc, 0x00006666);
    Func_02004932(1, Data_02002df0);
    Func_020048f8(60);

    workspace = Data_03001ebc;
    *(s32 *)(workspace + 232) = 60;
    *(s32 *)(workspace + 448) = 32;

    Func_02004a9a();
    Func_02004aa6();
    Func_0200496c(0);
    Func_02004972(1);
    Func_02004a60(10);
    Func_0200493c();
}
