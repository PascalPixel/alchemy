#include "types.h"

/*
 * resource_383 owner at 0x02000de4, 1380 bytes (0x02000de4-0x02001347):
 * code 0x02000de4-0x02001206, a three-word literal pool at 0x02001208 that
 * the body branches over, code 0x02001214-0x0200133e, and a two-word literal
 * pool at 0x02001340.
 *
 * The long cutscene that Func_02000b48 plays.  It is entirely straight-line -
 * there is not one conditional branch in the whole owner, only the single
 * unconditional `b` at 0x02001206 that steps over the interior pool - so it
 * reads as a script: pose, walk, camera, line, wait, repeated for
 * participants 0, 1 and 2.  The dialogue line ids are all formed from the
 * base 0x125f held in r8 (0x125f, 0x1264, 0x1265, 0x126c, 0x126d, 0x1275).
 *
 * Between the beats it toggles bit 0 of the byte at +90 of participants 0 and
 * 1 (clear, then set) and clears the byte at +91 of participant 2.
 *
 * All 151 `bl` sites are placed and reach 25 distinct callees - the inventory
 * row's `calls=143` predates the corrected decoding.  Targets come from
 * tools/lib/overlay_call_targets.ts (target offset = stored displacement + 2),
 * never from the disassembler's `bl` annotations, which are wrong for every
 * overlay branch.  111 reach the import veneer table at 0x02004cxx-0x02004f2b
 * and are named by the main-image address in the veneer's trailing word; 40
 * reach this overlay's own Func_020045f4, Func_0200460c, Func_02004624,
 * Func_0200463c, Func_02004658 and Func_02004684, all byte-exact in
 * assets/code.
 *
 * Epilogue is `pop {r3} / mov r8, r3 / pop {r5, r6} / pop {r0} / bx r0`, so
 * r0 holds the popped return address and the owner is void.
 *
 * Uncertainties: the result of Func_0808a070 at 0x02000f84 is never tested -
 * it is called purely for its effect, unlike the same import elsewhere in
 * this overlay.  Func_02004684 and Func_0808a480 are reached with no argument
 * register set up, so they are treated as taking none.
 */

/* Import veneers, named by the main-image function each one reaches.
 * Old-style declarations: arities vary between call sites in this overlay. */
void Func_02005b2a();
u8 *Func_02005b70();
void Func_02005bf0();
void Func_02005b46();
void Func_02005c28();
void Func_02005b56();
void Func_02005416(s32, s32);
void Func_02005c68();
void Func_02005b6e();
void Func_0200542e(s32, s32);
void Func_02005bfc();
void Func_020054a8(s32, s32);
void Func_02005b92();
void Func_020054de(void);
void Func_02005c5e();
u8 *Func_02005be4();
void Func_02005c36();
void Func_02005bc4();
u8 *Func_02005c0a();
void Func_02005bda();
void Func_02005cca();
void Func_02005cf4();
void Func_02005cfe();
void Func_02005c9c();
void Func_0200550e(s32, s32, s32);
void Func_02005d18();
void Func_02005c1e();
void Func_02005ce6();
void Func_02005c2c();
void Func_02005536(s32, s32, s32);
void Func_02005d26();
void Func_02005d06();
void Func_02005c4c();
void Func_02005526(s32, s32, s32);
void Func_02005d1e();
void Func_02005c64();
void Func_0200553e(s32, s32, s32);
void Func_02005d7a();
void Func_02005d86();
void Func_02005c8c();
void Func_02005d98();
void Func_02005c9e();
void Func_02005d78();
void Func_02005d82();
s32 Func_02005cfa();
void Func_02005cc8();
void Func_020055d2(s32, s32, s32);
void Func_02005d58();
void Func_02005ca8();
void Func_02005dd0();
void Func_02005dd2();
void Func_020055dc(s32, s32, s32);
void Func_02005e18();
void Func_02005de6();
u8 *Func_02005d6c();
void Func_02005dba();
void Func_02005d48();
u8 *Func_02005d8e();
void Func_02005d5c();
void Func_02005dda();
void Func_02005d70();
void Func_02005df6();
void Func_02005e50();
void Func_02005e16();
void Func_020056b2(s32, s32);
void Func_020056a0(s32, s32, s32);
void Func_02005e90();
void Func_020056f4(void);
void Func_0200569e(s32, s32, s32);
void Func_02005ea6();
void Func_02005e86();
void Func_02005dcc();
void Func_020056d6(s32, s32, s32);
void Func_020056e0(s32, s32, s32);
void Func_02005ed0();
void Func_02005eb0();
void Func_02005ec2();
void Func_02005eea();
void Func_02005ed4();
void Func_02005716(s32, s32, s32);
void Func_02005ee8();
void Func_020056fa(s32, s32, s32);
void Func_02005f34();
void Func_02005f3e();
void Func_02005e44();
void Func_0200574e(s32, s32, s32);
void Func_02005f60();
void Func_02005e66();
void Func_02005740(s32, s32, s32);
void Func_02005f84();
void Func_02005e8a();
void Func_02005f7a();
void Func_02005f9e();
void Func_02005ea4();
void Func_02005f8e();
void Func_02005f90();
void Func_02005f92();
void Func_02005fd6();
void Func_02005edc();
void Func_02005800(s32, s32);
void Func_020060a8();
void Func_020057a8(s32, s32);
void Func_0200583c(void);
void Func_02005fce();
void Func_02005808(s32, s32, s32);
void Func_020057c8(s32, s32);
void Func_02005fea();
void Func_02005824(s32, s32, s32);
void Func_02005fec();
void Func_02005f32();
void Func_0200583c_b(s32, s32, s32);
void Func_0200580a(s32, s32);
void Func_02006024();
void Func_02006066();
void Func_02005f6c();
void Func_0200585e(s32, s32, s32);
void Func_02006016();
void Func_02005888(s32, s32, s32);
void Func_0200605a();
void Func_02006064();
void Func_0200606e();
void Func_020060b2();
void Func_02005fb8();
void Func_02005878(s32, s32);
void Func_02006060();
void Func_020058d2(s32, s32, s32);
void Func_020058dc(s32, s32, s32);
void Func_0200589c(s32, s32);
void Func_020060ee();
void Func_020060f8();
void Func_02005ffe();
void Func_02005908(s32, s32, s32);
void Func_020058c8(s32, s32);
void Func_0200611a();
void Func_02006020();
void Func_0200592a(s32, s32, s32);
void Func_020058ea(s32, s32);
void Func_0200593c(s32, s32, s32);
void Func_020060dc();
void Func_0200594e(s32, s32, s32);
void Func_02006076();
void Func_02006138();
void Func_02006128();
void Func_02006158();

                     
                    
                                /* returns the participant record */

                     

                     

                     

                     

/* This overlay's own routines; byte-exact sources in assets/code. */

                                  

                         

void Func_02000de4(void)
{
    u8 *record;
    s32 line;

    /* The dialogue base held in r8 for the whole owner. */
    s32 permuted_78;
    line = 0x125f;

    record[91] = 0;
    Func_02005b2a(20);
    record = Func_02005b70(2);
    Func_02005bf0(2, 4, 0);
    Func_02005b46(40);

    Func_02005c28(line);                /* 0x125f */
    Func_02005b56(20);
    Func_02005416(2, 20);
    Func_02005c68(2, 0x101, 0);
    Func_02005b6e(50);
    Func_0200542e(2, 30);
    Func_02005bfc(2, 376, 392);         /* 188 << 1, 196 << 1 */
    Func_020054a8(2, 0);
    Func_02005b92(40);
    Func_020054de();
    Func_02005c5e(0, 1);

    record = Func_02005be4(0);
    record[90] = (u8)(record[90] & 0xfe);
    record[90] = (u8)(record[90] | 1);
    Func_02005c36(0, 384, 424);         /* 192 << 1, 212 << 1 */
    Func_02005bc4(1);
    record = Func_02005c0a(0);
    Func_02005bda(30);

    Func_02005cca(2, 0);
    Func_02005cf4(0, 258, 0);           /* 129 << 1 */
    Func_02005cfe(1, 258, 0);
    Func_02005bfc(60);
    Func_02005c9c(0, 3);
    Func_0200550e(1, 3, 30);
    Func_02005d18(2, 0x101, 0);
    Func_02005c1e(60);
    Func_02005ce6(2, 1);
    Func_02005c2c(10);
    Func_02005536(2, 3, 20);
    Func_02005d26(2, 0);
    Func_02005d06(1, 2);
    Func_02005c4c(10);
    Func_02005526(1, 0, 30);
    Func_02005d1e(0, 2);
    Func_02005c64(10);
    Func_0200553e(0, 1, 40);
    Func_02005d7a(0, 0x102, 0);
    Func_02005d86(1, 0x102, 0);
    Func_02005c8c(60);
    Func_02005d98(2, 0x102, 0);
    Func_02005c9e(60);
    Func_02005d86(2, 0);
    Func_02005d78(0, 2, 0);
    Func_02005d82(1, 2, 0);

    /* Called for effect; the result is not tested here. */
    Func_02005cfa(0, 0);

    Func_02005cc8(30);
    Func_020055d2(2, 3, 10);
    Func_02005d58(2, 384, 408);         /* 204 << 1 */
    Func_02005ce6(10);
    Func_02005ca8(line + 5, 1);         /* 0x1264 */
    Func_02005dd0(line + 6);            /* 0x1265 */
    Func_02005dd2(2, 1, 0);
    Func_020055dc(0, 1, 20);
    Func_02005e18(1, 0x102, 0);
    Func_02005d1e(60);
    Func_02005de6(1, 1);

    record = Func_02005d6c(1);
    Func_02005dba(1, 352, 408);         /* 176 << 1 */
    Func_02005d48(1);
    record = Func_02005d8e(1);
    permuted_78 = (u8)(record[90] & 0xfe);
    record[90] = (u8)(record[90] | 1);
    record[90]  = permuted_78;
    Func_02005d5c(10);

    Func_02005dda(2, 368, 408);         /* 184 << 1 */
    Func_02005d70(20);
    Func_02005df6(0, 368, 424);
    Func_02005e50(0, 1, 0);
    Func_02005e16(2);
    Func_020056b2(2, 1);
    Func_020056a0(1, 4, 10);
    Func_02005e90(1, 0);
    Func_020056f4();
    Func_0200569e(2, 0, 30);
    Func_02005ea6(2, 0);
    Func_02005e86(0, 1);
    Func_02005dcc(30);
    Func_020056d6(0, 3, 30);
    Func_020056e0(2, 3, 10);
    Func_02005ed0(2, 0);
    Func_02005eb0(1, 2);
    Func_02005ec2(1, 0, 0);
    Func_02005eea(1, 0);
    Func_02005ed4(0, 1, 0);
    Func_02005716(0, 3, 40);
    Func_02005ee8(0, 2, 0);
    Func_020056fa(1, 2, 30);
    Func_02005f34(0, 0x101, 0);
    Func_02005f3e(1, 0x101, 0);
    Func_02005e44(60);
    Func_0200574e(2, 3, 20);
    Func_02005f3e(2, 0);
    Func_02005f60(1, 0x103, 0);
    Func_02005e66(60);
    Func_02005740(1, 0, 10);
    Func_02005f60(1, 0);
    Func_02005f84(2, 0x100, 0);
    Func_02005e8a(60);
    Func_02005f7a(2, 0);
    Func_02005f9e(1, 0x102, 0);
    Func_02005ea4(60);
    Func_02005e66(line + 13, 1);        /* 0x126c */
    Func_02005f8e(line + 14);           /* 0x126d */
    Func_02005f90(2, 0, 0);
    Func_02005f92(1, 0, 0);
    Func_02005fd6(0, 0x102, 0);
    Func_02005edc(60);
    Func_02005800(2, 0);
    Func_020060a8();
    Func_020057a8(1, 30);
    Func_0200583c();
    Func_02005fce(2, 1, 0);
    Func_02005808(2, 3, 10);
    Func_020057c8(2, 20);
    Func_02005fea(2, 0, 0);
    Func_02005824(0, 3, 20);
    Func_02005fec(1, 1);
    Func_02005f32(20);
    Func_0200583c_b(1, 4, 10);
    Func_0200580a(1, 30);

    Func_02006024(2, 1, 0);
    Func_02006066(2, 0x101, 0);
    Func_02005f6c(30);
    Func_0200585e(0, 1, 10);
    Func_02006016(0, 3);
    Func_02005888(1, 3, 30);
    Func_0200605a(0, 2, 0);
    Func_02006064(1, 2, 0);
    Func_0200606e(2, 0, 0);
    Func_020060b2(2, 0x106, 0);         /* 131 << 1 */
    Func_02005fb8(60);
    Func_02005878(2, 10);
    Func_02006060(0, 3);
    Func_020058d2(1, 3, 20);
    Func_020058dc(2, 4, 20);
    Func_0200589c(2, 20);
    Func_020060ee(0, 0x101, 0);
    Func_020060f8(1, 0x101, 0);
    Func_02005ffe(60);
    Func_02005908(2, 3, 10);
    Func_020058c8(2, 30);
    Func_0200611a(0, 0x105, 0);
    Func_02006020(60);
    Func_0200592a(2, 4, 10);
    Func_020058ea(2, 30);
    Func_0200593c(2, 3, 10);
    Func_020060dc(0, 3);
    Func_0200594e(1, 3, 20);
    Func_02006076(2, 1);
    Func_02006138(line + 22);           /* 0x1275 */
    Func_02006128(2, 1);
    Func_02006158(2, 0);
}
