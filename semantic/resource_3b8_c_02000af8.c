#include "types.h"

/*
 * Resource 3b8 cutscene at 0x02000af8.
 *
 * Complete owner: `push {r5, r6, lr}` at 0x02000af8 and the matching
 * `pop {r5, r6} / pop {r0} / bx r0` interworking return at 0x02000fe2.  There
 * is one mid-function literal pool at 0x02000b3e-0x02000b4b, branched over by
 * the `b.n` at 0x02000b3c, and a trailing pool at 0x02000fe8-0x02000ff7.  Both
 * are data.
 *
 * Two distinct exits reach the epilogue.  The early test at 0x02000b32 jumps
 * to 0x02000fde, so it *does* run the closing import at 0x020053c8; the second
 * early exit at 0x02000b5c jumps to 0x02000fe2 and skips it.  That asymmetry is
 * reproduced below.
 *
 * The opening halfword read repeats the idiom of 0x02000108: the halfword at
 * +6 of the record returned by the first import, biased by 0x2000, masked with
 * 0xffffc000, then sign extended from bit 15 so only bits 14-15 survive.  Here
 * the value is afterwards re-narrowed to an unsigned halfword (lsls/lsrs #16)
 * before the two comparisons, so the tests are against 0 and 0x8000, not
 * against a negative number.
 *
 * Resolver-confirmed main-image symbols name every external call below.
 * Old-style declarations keep the source honest where a shared engine helper
 * is reached with more than one argument shape.
 */

void *Func_02004efe();
s32 Func_02004ef2();
void Func_0200501e();
void Func_02004fb4();
void Func_02004fc6();
s32 Func_02004f26();
s32 Func_02004eda();
void Func_02004f2c();
void Func_0200509a();
void Func_02004f3a();
void Func_02004fac();
void Func_02004fbe();
void Func_02004fc8();
void Func_0200504a();
void Func_02004f78();
void Func_020050cc();
void Func_02004fca();
void Func_02004f90();
void Func_02005068();
void Func_02004f9e();
void Func_02005036();
void Func_02004fe8();
void Func_0200511a();
void Func_02005126();
void Func_020050b2();
void Func_02004fe0();
void Func_020050c2();
void Func_02004ff0();
void Func_02005146();
void Func_02005044();
void Func_0200500a();
void Func_020050e2();
void Func_02005018();
void Func_020050b0();
void Func_02005026();
void Func_02005062();
void Func_02005196();
void Func_020051a0();
void Func_0200512c();
void Func_0200505a();
void Func_0200513c();
void Func_0200506a();
void Func_020051be();
void Func_020050bc();
void Func_02005082();
void Func_0200515a();
void Func_02005090();
void Func_02005128();
void Func_0200509e();
void Func_020050da();
void Func_0200520c();
void Func_02005216();
void Func_020051a2();
void Func_020050d0();
void Func_020051da();
void Func_02005246();
void Func_02005252();
void Func_020051de();
void Func_0200510c();
void Func_020051a4();
void Func_020051f2();
void Func_020051ba();
void Func_02005130();
s32 Func_020050fe();
void Func_02005212();
void Func_020051ec();
void Func_02005162();
s32 Func_02005130_b();
void Func_02005244();
void Func_0200518c();
void Func_0200526e();
void Func_0200527a();
void Func_02005286();
void Func_02005290();
void Func_02005326();
void Func_020052a2();
void Func_020052ae();
void Func_020052ba();
void Func_020052c6();
void Func_020051f4();
void Func_0200528c();
void Func_02005202();
void Func_020052da();
void Func_02005218();
s32 Func_020051e6();
void Func_020052fa();
void Func_020052d4();
void Func_0200524a();
s32 Func_02005218_b();
void Func_0200532c();
void Func_020053d0();
void Func_020053d4();
void Func_020053e8();
void Func_020053ec();
void Func_02005406();
void Func_02005398();
void Func_02005424();
void Func_0200534a();
void Func_020053b6();
void Func_020052e4();
void Func_020053bc();
void Func_020052f2();
void Func_0200538a();
void Func_02005300();
void Func_0200533a();
void Func_02005344();
void Func_0200534e();
void Func_020053ae();
void *Func_02005354();
void Func_0200538e();
void Func_020053ce();
void Func_020053de();
void *Func_02005384();
void Func_020053be();
void Func_020053fe();
void Func_0200540e();
void *Func_020053b4();
void Func_020053ee();
void Func_0200541c();
void Func_0200542e();
void Func_02005384_b();
void Func_020053c8();

                     

                    

                     

                     

                     

                     

                     

                     

void Func_02000af8(void)
{
    u16 *context = Func_02004efe(0);
    s32 facing = (s16)(((s32)context[3] + 0x2000) & 0xc000);
    u16 quadrant;
    s16 *record;
    u8 **workspace = (u8 **)0x03001ebc;

    /* The opening result is passed directly into the scene initializer. */
    Func_0200501e(Func_02004ef2(context));
    Func_02004fb4(0x1ffb);
    Func_02004fc6(-1, 0);

    if (Func_02004f26(0, 0) != 0) {
        goto close;
    }
    if (Func_02004eda(0x96a) != 0) {
        Func_02004f2c(20);
        Func_0200509a(0);
        return;                 /* skips 0x020053c8 */
    }

    Func_02004f3a(20);
    if (quadrant == 0)
        Func_02004fac(0, 128, 120);
    if (quadrant == 0x8000)
    quadrant = (u16)facing;
        Func_02004fbe(0, 240, 120);

    Func_02004fc8(0, 184, 120);
    Func_0200504a(0, 0, 0);
    Func_02004f78(10);
    Func_020050cc(1, 16, 0, 0x8000);
    Func_02004fca(1);
    Func_02004f90(10);
    Func_02005068(1, 0);
    Func_02004f9e(10);
    Func_02005036(0, 3);
    Func_02004fac(20);
    Func_02004fe8(1, 0x10000, 0x8000);
    Func_0200511a(1, 40, 0);
    Func_02005126(1, 0, -32);
    Func_020050b2(1, 0x6000, 0);
    Func_02004fe0(20);
    Func_020050c2(0, 0x8000, 0);
    Func_02004ff0(20);
    Func_02005146(2, -16, 0, 0);
    Func_02005044(2);
    Func_0200500a(10);
    Func_020050e2(2, 0);
    Func_02005018(10);
    Func_020050b0(0, 3);
    Func_02005026(20);
    Func_02005062(2, 0x10000, 0x8000);
    Func_02005196(2, -40, 0);
    Func_020051a0(2, 0, 40);
    Func_0200512c(2, 0xe000, 0);
    Func_0200505a(20);
    Func_0200513c(0, 0, 0);
    Func_0200506a(20);
    Func_020051be(3, 16, 0, 0x8000);
    Func_020050bc(3);
    Func_02005082(10);
    Func_0200515a(3, 0);
    Func_02005090(10);
    Func_02005128(0, 3);
    Func_0200509e(20);
    Func_020050da(3, 0x10000, 0x8000);
    Func_0200520c(3, 40, 0);
    Func_02005216(3, 0, 40);
    Func_020051a2(3, 0xa000, 0);
    Func_020050d0(20);
    Func_020051da(-1, -1, -1, 0);
    Func_02005246(0, -56, 0);
    Func_02005252(0, 0, -32);
    Func_020051de(0, 0x2000, 0);
    Func_0200510c(30);
    Func_020051a4(3, 3);
    Func_0200511a(10);
    Func_020051f2(3, 0);
    Func_020051ba(1, 3);
    Func_02005130(10);

    if (Func_020050fe(0x96a) == 0)
        Func_02005212(1, 0);
    else
        *(u16 *)(*workspace + 0x1d8) += 1;

    Func_020051ec(2, 3);
    Func_02005162(10);

    if (Func_02005130_b(0x96a) == 0)
        Func_02005244(2, 0);
    else
        *(u16 *)(*workspace + 0x1d8) += 1;

    Func_0200518c(20);
    Func_0200526e(0, 0, 0);
    Func_0200527a(1, 0x8000, 0);
    Func_02005286(3, 0x8000, 0);
    Func_02005290(2, 0, 0);
    Func_02005326(0);
    Func_020052a2(0, 0x2000, 0);
    Func_020052ae(1, 0x6000, 0);
    Func_020052ba(3, 0xa000, 0);
    Func_020052c6(2, 0xe000, 0);
    Func_020051f4(20);
    Func_0200528c(1, 3);
    Func_02005202(10);
    Func_020052da(1, 0);
    Func_020052a2(2, 3);
    Func_02005218(10);

    if (Func_020051e6(0x96a) == 0)
        Func_020052fa(2, 0);
    else
        *(u16 *)(*workspace + 0x1d8) += 1;

    Func_020052d4(3, 3);
    Func_0200524a(10);

    if (Func_02005218_b(0x96a) == 0)
        Func_0200532c(3, 0);
    else
        *(u16 *)(*workspace + 0x1d8) += 1;

    Func_020053d0(1, 0, 32);
    Func_020053d4(1, -112, 0);
    Func_020053e8(3, 0, -40);
    Func_020053ec(3, -112, 0);
    Func_020052a2(50);
    Func_02005406(2, 0, -24);
    Func_0200532c(1);
    Func_02005398(0, 0x4000, 0);
    Func_02005424(1, 0, -16);
    Func_0200534a(3);
    Func_020053b6(3, 0xc000, 0);
    Func_020052e4(20);
    Func_020053bc(1, 0);
    Func_020052f2(10);
    Func_0200538a(0, 3);
    Func_02005300(20);
    Func_0200533a(1, 0x13333, 0x9999);
    Func_02005344(2, 0x13333, 0x9999);
    Func_0200534e(3, 0x13333, 0x9999);

    Func_020053ae(1, 2);
    record = Func_02005354(0);
    if (record != 0)
        Func_0200538e(1, record[5], record[9]);
    Func_020053bc(1);
    Func_020053ce(1, 0, 0);

    Func_020053de(3, 2);
    record = Func_02005384(0);
    if (record != 0)
        Func_020053be(3, record[5], record[9]);
    Func_020053ec(3);
    Func_020053fe(3, 0, 0);

    Func_0200540e(2, 2);
    record = Func_020053b4(0);
    if (record != 0)
        Func_020053ee(2, record[5], record[9]);
    Func_0200541c(2);
    Func_0200542e(2, 0, 0);

    /* Reached with the flag id, not as the record query above. */
    Func_02005384_b(0x96a);
close:
    Func_020053c8();
}
