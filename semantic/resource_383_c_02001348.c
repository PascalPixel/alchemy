#include "types.h"

/*
 * resource_383 owner at 0x02001348, 1608 bytes (0x02001348-0x0200198f):
 * code 0x02001348-0x02001748, a six-word literal pool at 0x0200174c that the
 * body branches over, code 0x02001764-0x02001972, and a five-word literal
 * pool at 0x02001974.
 *
 * The overlay's largest cutscene.  Participants 8, 10, 11 and 12 join the
 * three background layers for a staged arrival: the camera is set up, the
 * first dialogue pair 0x12c5 / 0x12c6 plays, the party walks in, and after a
 * Func_0808a070 prompt the scene either plays the short reaction (no line) or
 * the long one (lines 0x12dc and 0x12d9).  Both paths rejoin for the closing
 * walk, then the story flag 0x854 is posted, the workspace word at +448 is
 * set to 0x200, the RAM byte at 0x0200046b is set to 3 and the scene hands
 * over.
 *
 * Apart from that one prompt the owner is straight-line; the `b` at
 * 0x02001748 merely steps over the interior literal pool.
 *
 * All 169 `bl` sites are placed and reach 30 distinct callees - the inventory
 * row's `calls=163` predates the corrected decoding.  Targets come from
 * tools/lib/overlay_call_targets.ts (target offset = stored displacement + 2),
 * never from the disassembler's `bl` annotations, which are wrong for every
 * overlay branch.  127 reach the import veneer table at 0x02004cxx-0x02004f2b
 * and are named by the main-image address in the veneer's trailing word; 42
 * reach this overlay's own Func_020045f4, Func_0200460c, Func_02004624 and
 * Func_0200463c, all byte-exact in assets/code.
 *
 * The dialogue ids are carried in r5 as a small cursor: 0x12c5 then +1 for
 * the reply, and later 0x12dc then -3.  That is reproduced literally below.
 *
 * Epilogue is `pop {r5} / pop {r0} / bx r0`, so r0 holds the popped return
 * address and the owner is void.
 *
 * Uncertainty: 0x02000240 + 0x22b = 0x0200046b is written as an absolute
 * EWRAM byte.  It is below the overlay's 0x02008000 link base, so unlike the
 * 0x0200dxxx / 0x0200exxx words elsewhere in this overlay it is a RAM global
 * and not in-image data.
 */

/* Import veneers, named by the main-image function each one reaches.
 * Old-style declarations: arities vary between call sites in this overlay. */
void Func_02006090();
void Func_020060da();
void Func_020060e4();
void Func_020060ee();
void Func_0200616e();
void Func_020060b4();
void Func_02006078();
void Func_0200619e();
void Func_020060cc();
void Func_0200616c();
void Func_020059e4(s32, s32, s32);
void Func_0200616a();
void Func_020059cc(s32, s32, s32);
void Func_0200619a();
void Func_020061a8();
void Func_02006196();
void Func_020061a4();
void Func_020061c2();
void Func_0200620c();
void Func_020061d2();
void Func_02005a24(s32, s32, s32);
void Func_02006260();
void Func_02006166();
void Func_02005a26(s32, s32);
void Func_0200620e();
void Func_02005a80(s32, s32, s32);
void Func_02006248();
void Func_0200618e();
void Func_02005a4e(s32, s32);
void Func_020062a2();
void Func_020062ae();
void Func_020061b4();
void Func_020062b0();
void Func_020061c6();
void Func_02005a86(s32, s32);
void Func_020062ca();
void Func_020061e0();
void Func_020062ec();
void Func_020061f2();
void Func_020062ee();
void Func_02006204();
void Func_02006300();
void Func_02006216();
void Func_02005af0(s32, s32, s32);
void Func_02005b2a(s32, s32, s32);
void Func_02005aea(s32, s32);
void Func_020062fa();
void Func_02005afa(s32, s32);
void Func_020062e0();
void Func_0200643e();
void Func_02005b16(s32, s32);
void Func_0200631e();
void Func_02006326();
void Func_0200632e();
void Func_0200633e();
void Func_02006284();
void Func_02006356();
void Func_02006360();
void Func_0200636a();
void Func_02005b7c(s32, s32, s32);
void Func_020062fe();
void Func_0200630c();
void Func_0200631a();
void Func_02006370();
void Func_0200637e();
void Func_02006374();
void Func_020063f8();
void Func_0200630e();
void Func_020063e0();
void Func_020063ea();
void Func_020063f4();
void Func_020063fe();
void Func_0200638e();
void Func_02006344();
void Func_0200639c();
void Func_020063aa();
void Func_0200644e();
void Func_0200645a();
void Func_02005c7a(s32, s32, s32);
void Func_02005c3a(s32, s32);
void Func_0200644a();
void Func_02006390();
void Func_02005c50(s32, s32);
void Func_02006460();
void Func_020063a6();
void Func_02005c66(s32, s32);
void Func_020064b8();
void Func_020063be();
void Func_02005cc8(s32, s32, s32);
void Func_02005c88(s32, s32);
void Func_020064da();
void Func_020064e4();
void Func_020064f6();
void Func_020063fc();
void Func_020064c4();
void Func_0200640a();
void Func_02005cca(s32, s32);
void Func_020064ee();
void Func_02005d00(s32, s32, s32);
void Func_02005d3a(s32, s32, s32);
void Func_02005cfa(s32, s32);
void Func_0200650a();
void Func_02006450();
void Func_02006522();
void Func_02005d34(s32, s32, s32);
void Func_02005d24(s32, s32);
void Func_02006578();
void Func_0200647e();
void Func_02005d5a(s32, s32);
void Func_0200656a();
void Func_020064b0();
void Func_02005d70(s32, s32);
void Func_0200658a();
void Func_02006594();
void Func_02005da6(s32, s32, s32);
void Func_02005de0(s32, s32, s32);
void Func_02005da0(s32, s32);
void Func_02005df2(s32, s32, s32);
void Func_02005db2(s32, s32);
void Func_02006606();
void Func_02006612();
void Func_0200661e();
void Func_02006524();
void Func_02005e2e(s32, s32, s32);
void Func_02006616();
void Func_0200653c();
void Func_0200660e();
void Func_02005e20(s32, s32, s32);
s32 Func_02006590();
void Func_02006562();
void Func_02006634();
void Func_0200663e();
void Func_02006584();
void Func_02006674();
void Func_02006666();
void Func_02006670();
void Func_02006688();
void Func_02006678();
void Func_020065be();
void Func_020066b0();
void Func_020066a6();
void Func_020066d8();
void Func_020065de();
void Func_02005e9e(s32, s32);
void Func_02005ef0(s32, s32, s32);
void Func_02005eb0(s32, s32);
void Func_02005eea(s32, s32, s32);
void Func_020066a2();
void Func_02005f14(s32, s32, s32);
void Func_02005f06(s32, s32, s32);
void Func_020066be();
void Func_02005f30(s32, s32, s32);
void Func_0200672c();
void Func_02006738();
void Func_02006744();
void Func_0200665a();
void Func_02005f64(s32, s32, s32);
void Func_02006754();
void Func_02006652();
void Func_020067ca();
void Func_020067da();
void Func_020067dc();
void Func_020066b8();

                     

                    

                     

                     

                     

                     

                     

                     

/* This overlay's own routines; byte-exact sources in assets/code. */

                                  

/* In-image outfit descriptors at file offsets 0x5248 and 0x52ac. */
extern u8 Data_0200d248[];
extern u8 Data_0200d2ac[];

void Func_02001348(void)
{
    u8 *workspace;
    s32 line;

    Func_02006090();
    Func_020060da(0, 0xcccc, 0x6666);
    Func_020060e4(1, 0xcccc, 0x6666);
    Func_020060ee(2, 0xcccc, 0x6666);
    Func_0200616e(0, 3);
    Func_020060b4(20);

    Func_02006078(line, 1);
    Func_0200619e(line);                /* 0x12c6 */
    line = line + 1;
    line = 0x12c5;
    Func_020060cc(30);

    Func_0200616c(8, 1);
    Func_020060da(60);
    Func_020059e4(8, 3, 40);
    Func_0200616a(0, 792, 440);
    Func_020059cc(0, 8, 20);
    Func_0200619a(1, 0x3180000, 0x1b80000);
    Func_020061a8(2, 0x3180000, 0x1b80000);
    Func_02006196(1, 808, 432);
    Func_020061a4(2, 792, 456);
    Func_020061c2(1);
    Func_0200620c(1, 8, 0);
    Func_020061d2(2);
    Func_02005a24(2, 8, 60);
    Func_02006260(8, 0x102, 0);
    Func_02006166(60);
    Func_02005a26(8, 20);
    Func_0200620e(2, 3);
    Func_02005a80(1, 3, 30);
    Func_02006248(8, 2);
    Func_0200618e(20);
    Func_02005a4e(8, 20);
    Func_020062a2(2, 0x102, 0);
    Func_020062ae(1, 0x102, 0);
    Func_020061b4(60);
    Func_020062b0(8, 0x3000, 0);
    Func_020061c6(10);
    Func_02005a86(8, 30);
    Func_020062ca(8, 0xd000, 0);
    Func_020061e0(30);
    Func_020062ec(8, 0x100, 0);
    Func_020061f2(60);
    Func_020062ee(8, 0xb000, 0);
    Func_02006204(40);
    Func_02006300(8, 0xd000, 0);
    Func_02006216(40);
    Func_02005af0(8, 0, 20);
    Func_02005b2a(8, 4, 30);
    Func_02005aea(8, 20);
    Func_020062fa(2, 1);
    Func_02005afa(2, 40);

    Func_020062e0(10, 0x2e80000, 0x1980000);
    Func_0200643e(61);
    Func_02005b16(10, 20);
    Func_0200631e(0, 1);
    Func_02006326(1, 1);
    Func_0200632e(2, 1);
    Func_0200633e(8, 1);
    Func_02006284(30);
    Func_02006356(0, 10, 0);
    Func_02006360(1, 10, 0);
    Func_0200636a(2, 10, 0);
    Func_02005b7c(8, 10, 40);

    Func_020062fe(10, 0xcccc, 0x6666);
    Func_0200630c(11, 0x18000, 0xc000);
    Func_0200631a(12, 0x18000, 0xc000);
    Func_02006370(11, 0x2e80000, 0x1980000);
    Func_0200637e(12, 0x2e80000, 0x1980000);
    Func_02006374(10, 792, 416);
    Func_020063f8(10, 0x3000, 0);
    Func_0200630e(70);
    Func_020063e0(0, 10, 0);
    Func_020063ea(1, 10, 0);
    Func_020063f4(2, 10, 0);
    Func_020063fe(8, 10, 0);
    Func_0200638e(11, Data_0200d248);
    Func_02006344(40);
    Func_0200639c(12, Data_0200d2ac);
    Func_020063aa(12);
    Func_0200644e(11, 0x2000, 0);
    Func_0200645a(12, 0x2000, 0);
    Func_02006370(40);
    Func_02005c7a(10, 4, 20);
    Func_02005c3a(10, 20);
    Func_0200644a(11, 1);
    Func_02006390(10);
    Func_02005c50(11, 20);
    Func_02006460(12, 1);
    Func_020063a6(10);
    Func_02005c66(12, 40);
    Func_020064b8(2, 0x101, 0);
    Func_020063be(60);
    Func_02005cc8(10, 4, 20);
    Func_02005c88(10, 20);
    Func_020064da(0, 0x101, 0);
    Func_020064e4(1, 0x101, 0);
    Func_020063ea(60);
    Func_020064f6(8, 0x100, 0);
    Func_020063fc(40);
    Func_020064c4(8, 2);
    Func_0200640a(20);
    Func_02005cca(8, 20);
    Func_020064e4(0, 8, 0);
    Func_020064ee(1, 8, 0);
    Func_02005d00(2, 8, 20);
    Func_02005d3a(8, 4, 30);
    Func_02005cfa(8, 20);
    Func_0200650a(2, 2);
    Func_02006450(20);
    Func_02006522(0, 2, 0);
    Func_02005d34(1, 2, 30);
    Func_02005d24(1, 20);
    Func_02006578(8, 0x102, 0);
    Func_0200647e(60);
    Func_02005d5a(8, 20);

    Func_0200656a(10, 1);
    Func_020064b0(10);
    Func_02005d70(10, 20);
    Func_0200658a(0, 10, 0);
    Func_02006594(1, 10, 0);
    Func_02005da6(2, 10, 20);
    Func_02005de0(11, 3, 20);
    Func_02005da0(11, 20);
    Func_02005df2(12, 4, 20);
    Func_02005db2(12, 20);
    Func_02006606(0, 0x102, 0);
    Func_02006612(1, 0x102, 0);
    Func_0200661e(2, 0x102, 0);
    Func_02006524(60);
    Func_02005e2e(10, 3, 20);
    Func_02006616(10, 0);
    Func_0200653c(50);
    Func_0200660e(2, 0, 0);
    Func_02005e20(1, 0, 30);

    if (Func_02006590(0, 0) == 0) {
        Func_02006562(40);
        Func_02006634(1, 10, 0);
        Func_0200663e(2, 10, 0);
        Func_0200663e(10, 2);
        Func_02006584(20);
        Func_02006674(10, 0);
    } else {
        Func_02006594(40);
        Func_02006666(1, 10, 0);
        Func_02006670(2, 10, 0);
        line = 0x12dc;
        Func_02006688(line);
        Func_02006678(10, 2);
        Func_020065be(20);
        Func_020066b0(10, 0);
        line = line - 3;
        Func_020066a6(line);            /* 0x12d9 */
    }

    Func_020066d8(11, 0x103, 0);
    Func_020065de(60);
    Func_02005e9e(11, 20);
    Func_02005ef0(12, 4, 20);
    Func_02005eb0(12, 30);
    Func_02005eea(10, 11, 30);
    Func_020066a2(10, 3);
    Func_02005f14(11, 3, 30);
    Func_02005f06(10, 12, 30);
    Func_020066be(10, 3);
    Func_02005f30(12, 3, 40);
    Func_0200672c(10, 0x3000, 0);
    Func_02006738(11, 0x3000, 0);
    Func_02006744(12, 0x3000, 0);
    Func_0200665a(20);
    Func_02005f64(10, 4, 20);
    Func_02006754(10, 0);

    Func_02006652(0x854);

    /* 224 << 1, then + 64: the workspace word at +448 becomes 0x200. */
    *(s32 *)(workspace + 448) = 512;
    workspace = *(u8 **)0x03001ebc;

    Func_020067ca(0x15, 17);
    Func_020067da(0x15, 16);

    *(u8 *)(0x02000240 + 0x22b) = 3;

    Func_020067dc(12, 5);
    Func_020066b8();
}
