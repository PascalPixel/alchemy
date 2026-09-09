#include "types.h"

#define RunMiddleEventSequence Func_02000af8

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
extern u8 Value_00001ffb;
s32 Func_02004eda();
void Func_02004ef2();
s32 Func_02004efe();
s32 Func_02004f26();
void Func_02004f2c();
void Func_02004f3a();
void Func_02004f78();
void Func_02004f90();
void Func_02004f9e();
void Func_02004fac();
void Func_02004fac_a();
void Func_02004fb4();
void Func_02004fbe();
s32 Func_02004fc6();
void Func_02004fc8();
void Func_02004fca();
void Func_02004fe0();
void Func_02004fe8();
void Func_02004ff0();
void Func_0200500a();
void Func_02005018();
void Func_0200501e();
void Func_02005026();
void Func_02005036();
void Func_02005044();
void Func_0200504a();
void Func_0200505a();
void Func_02005062();
void Func_02005068();
void Func_0200506a();
void Func_02005082();
void Func_02005090();
void Func_0200509a();
void Func_0200509e();
void Func_020050b0();
void Func_020050b2();
void Func_020050bc();
void Func_020050c2();
void Func_020050cc();
void Func_020050d0();
void Func_020050da();
void Func_020050e2();
s32 Func_020050fe();
void Func_0200510c();
void Func_0200511a();
void Func_0200511a_a();
void Func_02005126();
void Func_02005128();
void Func_0200512c();
s32 Func_02005130();
void Func_02005130_a();
void Func_0200513c();
void Func_02005146();
void Func_0200515a();
void Func_02005162();
void Func_0200518c();
void Func_02005196();
void Func_020051a0();
void Func_020051a2();
void Func_020051a4();
void Func_020051ba();
void Func_020051be();
void Func_020051da();
void Func_020051de();
s32 Func_020051e6();
void Func_020051ec();
void Func_020051f2();
void Func_020051f4();
void Func_02005202();
void Func_0200520c();
void Func_02005212();
void Func_02005216();
s32 Func_02005218();
void Func_02005218_a();
void Func_02005244();
void Func_02005246();
void Func_0200524a();
void Func_02005252();
void Func_0200526e();
void Func_0200527a();
void Func_02005286();
void Func_0200528c();
void Func_02005290();
void Func_020052a2();
void Func_020052a2_a();
void Func_020052a2_b();
void Func_020052ae();
void Func_020052ba();
void Func_020052c6();
void Func_020052d4();
void Func_020052da();
void Func_020052e4();
void Func_020052f2();
void Func_020052fa();
void Func_02005300();
void Func_02005326();
void Func_0200532c();
void Func_0200532c_a();
void Func_0200533a();
void Func_02005344();
void Func_0200534a();
void Func_0200534e();
s32 Func_02005354();
s32 Func_02005384();
void Func_02005384_a();
void Func_0200538a();
void Func_0200538e();
void Func_02005398();
void Func_020053ae();
s32 Func_020053b4();
void Func_020053b6();
void Func_020053bc();
void Func_020053bc_a();
void Func_020053be();
void Func_020053c8();
void Func_020053ce();
void Func_020053d0();
void Func_020053d4();
void Func_020053de();
void Func_020053e8();
void Func_020053ec();
void Func_020053ec_a();
void Func_020053ee();
void Func_020053fe();
void Func_02005406();
void Func_0200540e();
void Func_0200541c();
void Func_02005424();
void Func_0200542e();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void RunMiddleEventSequence(void)
{
    s32 record;
    s16 facing;
    s32 t;
    s32 c;

    record = Func_02004efe(0);
    facing = (s16)((*(u16 *)(record + 6) + 0x2000) & -0x4000);
    Func_02004ef2();
    Func_0200501e();
    Call1(Func_02004fb4, (s32)&Value_00001ffb);
    Value2(Func_02004fc6, -1, 0);
    if (Value2(Func_02004f26, 0, 0) != 0) {
    } else {
        if (Value1(Func_02004eda, 0x96a) != 0) {
            Func_02004f2c(20);
            Func_0200509a(0);
            goto done;
        }
        Func_02004f3a(20);
        t = (u16)facing;
        if (t == 0) {
            Func_02004fac(0, 128, 120);
        }
        c = 0x8000;
        if (t == c) {
            Func_02004fbe(0, 240, 120);
        }
        Func_02004fc8(0, 184, 120);
        Func_0200504a(0, 0, 0);
        Func_02004f78(10);
        Func_020050cc(1, 16, 0, c);
        Func_02004fca(1);
        Func_02004f90(10);
        Func_02005068(1, 0);
        Func_02004f9e(10);
        Func_02005036(0, 3);
        Func_02004fac_a(20);
        Call3(Func_02004fe8, 1, 0x10000, c);
        Func_0200511a(1, 40, 0);
        Call3(Func_02005126, 1, 0, -32);
        Call3(Func_020050b2, 1, 0x6000, 0);
        Func_02004fe0(20);
        Func_020050c2(0, c, 0);
        Func_02004ff0(20);
        Call4(Func_02005146, 2, -16, 0, 0);
        Func_02005044(2);
        Func_0200500a(10);
        Func_020050e2(2, 0);
        Func_02005018(10);
        Func_020050b0(0, 3);
        Func_02005026(20);
        Call3(Func_02005062, 2, 0x10000, c);
        Call3(Func_02005196, 2, -40, 0);
        Func_020051a0(2, 0, 40);
        Call3(Func_0200512c, 2, 0xe000, 0);
        Func_0200505a(20);
        Func_0200513c(0, 0, 0);
        Func_0200506a(20);
        Func_020051be(3, 16, 0, c);
        Func_020050bc(3);
        Func_02005082(10);
        Func_0200515a(3, 0);
        Func_02005090(10);
        Func_02005128(0, 3);
        Func_0200509e(20);
        Call3(Func_020050da, 3, 0x10000, c);
        Func_0200520c(3, 40, 0);
        Func_02005216(3, 0, 40);
        Call3(Func_020051a2, 3, 0xa000, 0);
        Func_020050d0(20);
        Call4(Func_020051da, -1, -1, -1, 0);
        Call3(Func_02005246, 0, -56, 0);
        Call3(Func_02005252, 0, 0, -32);
        Call3(Func_020051de, 0, 0x2000, 0);
        Func_0200510c(30);
        Func_020051a4(3, 3);
        Func_0200511a_a(10);
        Func_020051f2(3, 0);
        Func_020051ba(1, 3);
        Func_02005130(10);
        if (Value1(Func_020050fe, 0x96a) == 0) {
            Func_02005212(1, 0);
        } else {
            bump_step(1);
        }
        Func_020051ec(2, 3);
        Func_02005162(10);
        if (Value1(Func_02005130_a, 0x96a) == 0) {
            Func_02005244(2, 0);
        } else {
            bump_step(1);
        }
        Func_0200518c(20);
        Func_0200526e(0, 0, 0);
        Call3(Func_0200527a, 1, 0x8000, 0);
        Call3(Func_02005286, 3, 0x8000, 0);
        Func_02005290(2, 0, 0);
        Func_02005326(0);
        Call3(Func_020052a2, 0, 0x2000, 0);
        Call3(Func_020052ae, 1, 0x6000, 0);
        Call3(Func_020052ba, 3, 0xa000, 0);
        Call3(Func_020052c6, 2, 0xe000, 0);
        Func_020051f4(20);
        Func_0200528c(1, 3);
        Func_02005202(10);
        Func_020052da(1, 0);
        Func_020052a2_a(2, 3);
        Func_02005218(10);
        if (Value1(Func_020051e6, 0x96a) == 0) {
            Func_020052fa(2, 0);
        } else {
            bump_step(1);
        }
        Func_020052d4(3, 3);
        Func_0200524a(10);
        if (Value1(Func_02005218_a, 0x96a) == 0) {
            Func_0200532c(3, 0);
        } else {
            bump_step(1);
        }
        Func_020053d0(1, 0, 32);
        Call3(Func_020053d4, 1, -112, 0);
        Call3(Func_020053e8, 3, 0, -40);
        Call3(Func_020053ec, 3, -112, 0);
        Func_020052a2_b(50);
        Call3(Func_02005406, 2, 0, -24);
        Func_0200532c_a(1);
        Call3(Func_02005398, 0, 0x4000, 0);
        Call3(Func_02005424, 1, 0, -16);
        Func_0200534a(3);
        Call3(Func_020053b6, 3, 0xc000, 0);
        Func_020052e4(20);
        Func_020053bc(1, 0);
        Func_020052f2(10);
        Func_0200538a(0, 3);
        Func_02005300(20);
        Call3(Func_0200533a, 1, 0x13333, 0x9999);
        Call3(Func_02005344, 2, 0x13333, 0x9999);
        Call3(Func_0200534e, 3, 0x13333, 0x9999);
        Func_020053ae(1, 2);
        record = Value1(Func_02005354, 0);
        if (record != 0) {
            Func_0200538e(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Func_020053bc_a(1);
        Func_020053ce(1, 0, 0);
        Func_020053de(3, 2);
        record = Value1(Func_02005384, 0);
        if (record != 0) {
            Func_020053be(3, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Func_020053ec_a(3);
        Func_020053fe(3, 0, 0);
        Func_0200540e(2, 2);
        record = Value1(Func_020053b4, 0);
        if (record != 0) {
            Func_020053ee(2, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Func_0200541c(2);
        Func_0200542e(2, 0, 0);
        Call1(Func_02005384_a, 0x96a);
    }
    Func_020053c8();
done:
}
