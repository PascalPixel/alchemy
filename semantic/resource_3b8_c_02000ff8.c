#include "types.h"

/*
 * Resource 3b8 cutscene at 0x02000ff8.
 *
 * Complete owner: `push {lr}` at 0x02000ff8 and the matching `pop {r0} / bx r0`
 * interworking return at 0x020017ca.  The body is one straight-line call
 * carpet with no conditional branches; the only branch, at 0x020013f4, hops the
 * mid-function literal pool at 0x020013f6-0x02001407.  A second pool at
 * 0x020017ce-0x020017e7 follows the return.  Both are data.
 *
 * No register value survives any call here: every argument is materialised
 * immediately before its `bl`, and no result is consumed, so the owner is a
 * pure sequence of 250 import calls.
 *
 * As elsewhere in this overlay, every `bl` reaches the import band above the
 * code and each import is named by the address its call site computes, the
 * convention the byte-exact sources in `assets/code/resource_3b8_c_*.c`
 * already use; the numeric name carries no further meaning.  Old-style
 * declarations are required because several of these addresses are reached
 * with different argument counts from this one owner (0x02005452, 0x020054aa,
 * 0x020056f2, 0x02005718, 0x0200579c, 0x020058a8, 0x02005958, 0x02005972,
 * 0x0200598e, 0x020059c4, 0x02005a22, 0x02005aa0, 0x02005b28, 0x02005b62 and
 * 0x02005c1c).
 *
 * Shifted immediates are written as their computed values (for example
 * `movs r1, #130 / lsls r1, #2` is spelled 520).  Pool words 0x00014ccc /
 * 0x0000a666 and 0x0000cccc / 0x00006666 are passed as plain constants; they
 * are fixed-point pairs, not addresses.
 */



























extern void Func_020053a6();
extern void Func_020053e2();
extern void Func_0200550e();
extern void Func_020054a4();
extern void Func_02005452();
extern void Func_020054d6();
extern void Func_02005508();
extern void Func_02005514();
extern void Func_02005584();
extern void Func_0200549a();
extern void Func_02005512();
extern void Func_02005448();
extern void Func_02005532();
extern void Func_02005536();
extern void Func_0200549c();
extern void Func_020054aa();
extern void Func_020054e0();
extern void Func_020054f6();
extern void Func_02005404();
extern void Func_0200552c();
extern void Func_020054b0();
extern void Func_02005548();
extern void Func_020054be();
extern void Func_02005596();
extern void Func_020054cc();
extern void Func_0200557c();
extern void Func_020054da();
extern void Func_0200563e();
extern void Func_020054ec();
extern void Func_020055c4();
extern void Func_020054fa();
extern void Func_0200558a();
extern void Func_0200559a();
extern void Func_02005510();
extern void Func_02005516();
extern void Func_02005600();
extern void Func_020055f8();
extern void Func_0200552e();
extern void Func_0200561a();
extern void Func_02005578();
extern void Func_020056aa();
extern void Func_02005558();
extern void Func_02005630();
extern void Func_02005566();
extern void Func_02005650();
extern void Func_02005576();
extern void Func_02005662();
extern void Func_020055c0();
extern void Func_020056f2();
extern void Func_020055a0();
extern void Func_02005678();
extern void Func_020055ae();
extern void Func_02005646();
extern void Func_020055bc();
extern void Func_02005694();
extern void Func_020055ca();
extern void Func_0200565a();
extern void Func_0200566a();
extern void Func_020055e0();
extern void Func_020055e6();
extern void Func_02005696();
extern void Func_020055f4();
extern void Func_020056cc();
extern void Func_02005602();
extern void Func_0200569a();
extern void Func_02005610();
extern void Func_020056e8();
extern void Func_0200561e();
extern void Func_02005708();
extern void Func_02005700();
extern void Func_02005636();
extern void Func_02005720();
extern void Func_02005718();
extern void Func_0200564e();
extern void Func_0200573a();
extern void Func_02005732();
extern void Func_02005668();
extern void Func_02005676();
extern void Func_0200574e();
extern void Func_02005684();
extern void Func_0200571c();
extern void Func_02005692();
extern void Func_02005698();
extern void Func_02005748();
extern void Func_020056a6();
extern void Func_0200577e();
extern void Func_020056b4();
extern void Func_020057a0();
extern void Func_02005798();
extern void Func_020056ce();
extern void Func_02005790();
extern void Func_02005792();
extern void Func_0200579c();
extern void Func_020056f8();
extern void Func_020057a8();
extern void Func_02005706();
extern void Func_020057de();
extern void Func_02005714();
extern void Func_02005800();
extern void Func_020057f8();
extern void Func_0200572e();
extern void Func_020057c6();
extern void Func_0200573c();
extern void Func_02005814();
extern void Func_0200574a();
extern void Func_0200583c();
extern void Func_02005804();
extern void Func_02005762();
extern void Func_0200583a();
extern void Func_02005770();
extern void Func_0200585a();
extern void Func_02005852();
extern void Func_02005788();
extern void Func_02005820();
extern void Func_02005796();
extern void Func_02005886();
extern void Func_0200587e();
extern void Func_020057b4();
extern void Func_0200584c();
extern void Func_020057c2();
extern void Func_0200589a();
extern void Func_020057e4();
extern void Func_020058a6();
extern void Func_020058a8();
extern void Func_020058b2();
extern void Func_02005808();
extern void Func_0200580e();
extern void Func_020058f8();
extern void Func_020058f0();
extern void Func_02005826();
extern void Func_020058d6();
extern void Func_02005834();
extern void Func_0200590c();
extern void Func_02005842();
extern void Func_020058da();
extern void Func_02005850();
extern void Func_02005928();
extern void Func_0200585e();
extern void Func_020058f6();
extern void Func_0200586c();
extern void Func_02005872();
extern void Func_02005922();
extern void Func_02005880();
extern void Func_02005958();
extern void Func_0200588e();
extern void Func_0200597a();
extern void Func_02005972();
extern void Func_020058b6();
extern void Func_0200598e();
extern void Func_020058c4();
extern void Func_0200595c();
extern void Func_020058d2();
extern void Func_020059aa();
extern void Func_020058e0();
extern void Func_020059cc();
extern void Func_020059c4();
extern void Func_020058fa();
extern void Func_020059e6();
extern void Func_020059de();
extern void Func_02005914();
extern void Func_020059fe();
extern void Func_020059f6();
extern void Func_0200592c();
extern void Func_0200593a();
extern void Func_02005a12();
extern void Func_02005948();
extern void Func_02005a0a();
extern void Func_02005a0c();
extern void Func_02005a16();
extern void Func_0200596c();
extern void Func_02005a22();
extern void Func_02005980();
extern void Func_02005a58();
extern void Func_02005a80();
extern void Func_02005a8a();
extern void Func_02005a4a();
extern void Func_02005a5a();
extern void Func_020059b8();
extern void Func_020059be();
extern void Func_02005aa0();
extern void Func_020059ce();
extern void Func_02005aa6();
extern void Func_020059dc();
extern void Func_02005a74();
extern void Func_020059ea();
extern void Func_02005ac2();
extern void Func_020059f8();
extern void Func_02005a90();
extern void Func_02005a06();
extern void Func_02005ade();
extern void Func_02005a14();
extern void Func_02005ac4();
extern void Func_02005b06();
extern void Func_02005a34();
extern void Func_02005b0c();
extern void Func_02005a42();
extern void Func_02005ada();
extern void Func_02005a50();
extern void Func_02005b28();
extern void Func_02005a5e();
extern void Func_02005b48();
extern void Func_02005b40();
extern void Func_02005a76();
extern void Func_02005b0e();
extern void Func_02005a84();
extern void Func_02005b5c();
extern void Func_02005a92();
extern void Func_02005b2a();
extern void Func_02005b62();
extern void Func_02005b42();
extern void Func_02005ab8();
extern void Func_02005b50();
extern void Func_02005ac6();
extern void Func_02005baa();
extern void Func_02005ad8();
extern void Func_02005b12();
extern void Func_02005b1c();
extern void Func_02005c46();
extern void Func_02005c58();
extern void Func_02005b8a();
extern void Func_02005b94();
extern void Func_02005b1a();
extern void Func_02005bca();
extern void Func_02005c96();
extern void Func_02005b44();
extern void Func_02005c1c();
extern void Func_02005b52();
extern void Func_02005bea();
extern void Func_02005b60();
extern void Func_02005c42();
extern void Func_02005b70();
extern void Func_02005bae();
extern void Func_02005ce0();
extern void Func_02005cea();
extern void Func_02005bb0();
void Func_02000ff8(void)
{
    Func_020053a6(0x96b);
    Func_020053e2();
    Func_0200550e();
    Func_020054a4(0x2021);
    Func_02005452(0, 520, 424);
    Func_020054d6(0, 0xe000, 0);
    Func_02005508(0x2300000, -1, 0x1700000, 1);
    Func_02005514();
    Func_02005452(20, 0x10000, 0x8000);
    Func_02005584(20, 40, 0);
    Func_0200549a(20, 584, 360);
    Func_02005512(21, 0);
    Func_02005448(10);
    Func_02005532(20, 0x101, 40);
    Func_02005536(20, 0x8000, 0);
    Func_0200549c(21, 0x20000, 0x10000);
    Func_020054aa(22, 0x20000, 0x10000);
    Func_020054e0(21, 528, 352);
    Func_020054f6(22, 528, 368);
    Func_02005404(3);
    Func_0200552c(21, 1);
    Func_020054aa(30);
    Func_020054b0(10);
    Func_02005548(22, 3);
    Func_020054be(30);
    Func_02005596(22, 0);
    Func_020054cc(10);
    Func_0200557c(20, 2);
    Func_020054da(20);
    Func_0200563e(20, -16, 0);
    Func_020054ec(10);
    Func_020055c4(20, 0);
    Func_020054fa(10);
    Func_0200558a(21, 4);
    Func_0200559a(22, 4);
    Func_02005510(20);
    Func_02005516(10);
    Func_02005600(20, 0x105, 40);
    Func_020055f8(20, 0);
    Func_0200552e(10);
    Func_0200561a(21, 0x102, 40);
    Func_02005578(21, 0x10000, 0x8000);
    Func_020056aa(21, 8, 0);
    Func_02005558(20);
    Func_02005630(21, 0);
    Func_02005566(10);
    Func_02005650(20, 0x101, 80);
    Func_02005576(10);
    Func_02005662(22, 0x102, 40);
    Func_020055c0(22, 0x10000, 0x8000);
    Func_020056f2(22, 8, 0);
    Func_020055a0(20);
    Func_02005678(22, 0);
    Func_020055ae(10);
    Func_02005646(20, 4);
    Func_020055bc(20);
    Func_02005694(20, 0);
    Func_020055ca(10);
    Func_0200565a(21, 3);
    Func_0200566a(22, 3);
    Func_020055e0(30);
    Func_020055e6(10);
    Func_02005696(21, 2);
    Func_020055f4(20);
    Func_020056cc(21, 0);
    Func_02005602(10);
    Func_0200569a(22, 4);
    Func_02005610(20);
    Func_020056e8(22, 0);
    Func_0200561e(10);
    Func_02005708(20, 0x105, 40);
    Func_02005700(20, 0);
    Func_02005636(10);
    Func_02005720(21, 0x101, 40);
    Func_02005718(21, 0);
    Func_0200564e(10);
    Func_0200573a(20, 0x102, 40);
    Func_02005732(20, 0);
    Func_02005668(10);
    Func_02005718(22, 2);
    Func_02005676(20);
    Func_0200574e(22, 0);
    Func_02005684(10);
    Func_0200571c(20, 3);
    Func_02005692(30);
    Func_02005698(10);
    Func_02005748(21, 2);
    Func_020056a6(20);
    Func_0200577e(21, 0);
    Func_020056b4(10);
    Func_020057a0(20, 0x102, 40);
    Func_02005798(20, 0);
    Func_020056ce(20);
    Func_02005790(21, 22, 60);
    Func_02005792(21, 20, 0);
    Func_0200579c(22, 20, 0);
    Func_020056f2(20);
    Func_020056f8(10);
    Func_020057a8(20, 2);
    Func_02005706(20);
    Func_020057de(20, 0);
    Func_02005714(10);
    Func_02005800(21, 0x100, 40);
    Func_020057f8(21, 0);
    Func_0200572e(10);
    Func_020057c6(22, 3);
    Func_0200573c(30);
    Func_02005814(22, 0);
    Func_0200574a(10);
    Func_0200583c(20, 0x102);
    Func_02005804(20, 2);
    Func_02005762(30);
    Func_0200583a(20, 0);
    Func_02005770(10);
    Func_0200585a(21, 0x101, 40);
    Func_02005852(21, 0);
    Func_02005788(10);
    Func_02005820(20, 3);
    Func_02005796(30);
    Func_0200579c(10);
    Func_02005886(22, 0x101, 40);
    Func_0200587e(22, 0);
    Func_020057b4(10);
    Func_0200584c(20, 4);
    Func_020057c2(20);
    Func_0200589a(20, 0);
    Func_020057e4(20);
    Func_020058a6(21, 22, 60);
    Func_020058a8(21, 20, 0);
    Func_020058b2(22, 20, 0);
    Func_02005808(20);
    Func_0200580e(10);
    Func_020058f8(20, 0x105, 40);
    Func_020058f0(20, 0);
    Func_02005826(10);
    Func_020058d6(21, 2);
    Func_02005834(20);
    Func_0200590c(21, 0);
    Func_02005842(10);
    Func_020058da(22, 3);
    Func_02005850(30);
    Func_02005928(22, 0);
    Func_0200585e(10);
    Func_020058f6(20, 3);
    Func_0200586c(30);
    Func_02005872(10);
    Func_02005922(22, 2);
    Func_02005880(20);
    Func_02005958(22, 0);
    Func_0200588e(10);
    Func_0200597a(20, 0x102, 40);
    Func_02005972(20, 0);
    Func_020058a8(10);
    Func_02005958(21, 2);
    Func_020058b6(20);
    Func_0200598e(21, 0);
    Func_020058c4(10);
    Func_0200595c(20, 4);
    Func_020058d2(20);
    Func_020059aa(20, 0);
    Func_020058e0(10);
    Func_020059cc(22, 0x100, 40);
    Func_020059c4(22, 0);
    Func_020058fa(10);
    Func_020059e6(20, 0x102, 40);
    Func_020059de(20, 0);
    Func_02005914(10);
    Func_020059fe(21, 0x101, 40);
    Func_020059f6(21, 0);
    Func_0200592c(10);
    Func_020059c4(20, 4);
    Func_0200593a(20);
    Func_02005a12(20, 0);
    Func_02005948(20);
    Func_02005a0a(21, 22, 60);
    Func_02005a0c(21, 20, 0);
    Func_02005a16(22, 20, 0);
    Func_0200596c(20);
    Func_02005972(10);
    Func_02005a22(20, 2);
    Func_02005980(20);
    Func_02005a58(20, 0);
    Func_0200598e(10);
    Func_02005a80(21, 0x102);
    Func_02005a8a(22, 0x102);
    Func_02005a4a(21, 2);
    Func_02005a5a(22, 2);
    Func_020059b8(30);
    Func_020059be(10);
    Func_02005aa0(20, 0, 0);
    Func_020059ce(20);
    Func_02005aa6(20, 0);
    Func_020059dc(10);
    Func_02005a74(21, 4);
    Func_020059ea(20);
    Func_02005ac2(21, 0);
    Func_020059f8(10);
    Func_02005a90(22, 4);
    Func_02005a06(20);
    Func_02005ade(22, 0);
    Func_02005a14(10);
    Func_02005ac4(20, 2);
    Func_02005a22(20);
    Func_02005b06(20, 0x8000, 0);
    Func_02005a34(20);
    Func_02005b0c(20, 0);
    Func_02005a42(10);
    Func_02005ada(21, 3);
    Func_02005a50(30);
    Func_02005b28(21, 0);
    Func_02005a5e(20);
    Func_02005b48(20, 0x105, 40);
    Func_02005b40(20, 0);
    Func_02005a76(10);
    Func_02005b0e(22, 3);
    Func_02005a84(30);
    Func_02005b5c(22, 0);
    Func_02005a92(10);
    Func_02005b2a(20, 3);
    Func_02005aa0(40);
    Func_02005b62(21, 22, 60);
    Func_02005b42(21, 3);
    Func_02005ab8(20);
    Func_02005b50(22, 3);
    Func_02005ac6(20);
    Func_02005baa(22, 0x4000, 0);
    Func_02005ad8(40);
    Func_02005b12(21, 0x14ccc, 0xa666);
    Func_02005b1c(22, 0x14ccc, 0xa666);
    Func_02005c46(21, 0, 120);
    Func_02005c58(22, 0, 120);
    Func_02005b8a(21, 0, 0);
    Func_02005b94(22, 0, 0);
    Func_02005b1a(10);
    Func_02005bca(20, 2);
    Func_02005b28(40);
    Func_02005b62(20, 0xcccc, 0x6666);
    Func_02005c96(20, -16, 0);
    Func_02005b44(30);
    Func_02005c1c(20, 0);
    Func_02005b52(10);
    Func_02005bea(20, 4);
    Func_02005b60(30);
    Func_02005c42(20, 0, 0);
    Func_02005b70(30);
    Func_02005bae(20, 0x10000, 0x8000);
    Func_02005ce0(20, 120, 0);
    Func_02005cea(20, 60, 0);
    Func_02005c1c(20, 0, 0);
    Func_02005bb0();
}
