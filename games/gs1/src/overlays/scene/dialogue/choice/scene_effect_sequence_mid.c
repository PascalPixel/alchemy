#include "types.h"

#define FieldScene_RunScene3b8SequenceB Func_02000674

extern u8 Value_00002280[];
extern u8 Data_03001ebc[];
void Func_020049da();
void Func_02004a58();
void Func_02004a62();
s32 Func_02004a84();
s32 Func_02004a90();
s32 Func_02004ab6();
s32 Func_02004ac2();
s32 Func_02004ad4();
void Func_02004b16();
void Func_02004b26();
void Func_02004b2e();
void Func_02004b50();
void Func_02004b54();
void Func_02004b56();
void Func_02004b5e();
void Func_02004b64();
void Func_02004b6a();
void Func_02004b6c();
void Func_02004b72();
void Func_02004b9a();
s32 Func_02004ba0();
void Func_02004ba8();
s32 Func_02004bae();
void Func_02004bb6();
void Func_02004bc4();
void Func_02004bce();
void Func_02004bd2();
void Func_02004bd8();
void Func_02004be2();
void Func_02004be4();
void Func_02004bfc();
void Func_02004c06();
void Func_02004c0a();
void Func_02004c18();
void Func_02004c1c();
void Func_02004c28();
void Func_02004c32();
void Func_02004c34();
void Func_02004c38();
void Func_02004c3c();
void Func_02004c40();
void Func_02004c42();
void Func_02004c50();
void Func_02004c5e();
void Func_02004c66();
void Func_02004c70();
void Func_02004c7e();
void Func_02004c94();
void Func_02004c9a();
void Func_02004c9c();
void Func_02004c9c_a();
void Func_02004cb6();
void Func_02004cbc();
void Func_02004cc4();
void Func_02004cc6();
s32 Func_02004ccc();
void Func_02004cce();
void Func_02004cd2();
void Func_02004cda();
void Func_02004ce0();
void Func_02004ce2();
void Func_02004cee();
void Func_02004cfc();
void Func_02004d04();
void Func_02004d10();
void Func_02004d14();
void Func_02004d1a();
void Func_02004d20();
void Func_02004d28();
void Func_02004d36();
void Func_02004d42();
void Func_02004d44();
void Func_02004d48();
void Func_02004d4e();
void Func_02004d52();
void Func_02004d5c();
void Func_02004d60();
s32 Func_02004d6a();
void Func_02004d6c();
void Func_02004d78();
void Func_02004d7c();
void Func_02004d86();
void Func_02004d90();
void Func_02004daa();
void Func_02004db2();
void Func_02004dc0();
void Func_02004dc6();
s32 Func_02004dca();
void Func_02004dce();
void Func_02004dd4();
void Func_02004dd4_a();
void Func_02004df2();
void Func_02004e1c();
void Func_02004e1e();
void Func_02004e2c();
void Func_02004e36();
void Func_02004e40();
s32 Func_02004e46();
void Func_02004e4e();
void Func_02004e56();
void Func_02004e64();
void Func_02004e6c();
s32 Func_02004e76();
void Func_02004e7c();
void Func_02004e80();
void Func_02004e8c();
void Func_02004e98();
void Func_02004ea0();
s32 Func_02004ea6();
s32 Func_02004ea6_a();
void Func_02004eae();
void Func_02004eb0();
void Func_02004eba();
void Func_02004ec0();
void Func_02004ec4();
void Func_02004ed0();
void Func_02004ede();
void Func_02004ee0();
void Func_02004ef0();
void Func_02004ef4();
void Func_02004f00();
void Func_02004f0e();
void Func_02004f20();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

#define FieldScene_RunScene3b8SequenceA Func_02000af8
extern u8 Value_00001ffb[];
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
s32 Func_02005130_a();
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
s32 Func_02005218_a();
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
s32 Func_02005384_a();
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

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}
static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ __attribute__((always_inline)) void bump_step(s32 amount)
{
    u8 *scene = *(u8 **)Data_03001ebc;

    *(u16 *)(scene + 0x1d8) = (u16)(*(u16 *)(scene + 0x1d8) + amount);
}

void FieldScene_RunScene3b8SequenceB(void)
{
    u32 i;
    s32 record;
    s32 v5;

    Func_02004a58();
    Func_02004b16((s32)&Value_00002280);
    v5 = 0;
    *(u8 *)(Func_02004a84(0) + 84) = v5;
    *(u8 *)(Func_02004a90(10) + 84) = v5;
    Func_020049da(1);
    *(volatile u16 *)0x04000000 = 0x1140;
    Call2(Func_02004b54, -1, 0);
    *(volatile u16 *)0x04000000 = 0x140;
    v5 = 1;
    *(u8 *)(Func_02004ab6(0) + 84) = v5;
    *(u8 *)(Func_02004ac2(10) + 84) = v5;
    Func_02004b2e(0, 31);
    record = Func_02004ad4(0);
    Func_02004a62(record, 0);
    Call3(Func_02004b50, 1, 0x780000, 0x680000);
    Call3(Func_02004b5e, 3, 0x680000, 0x500000);
    Call3(Func_02004b6c, 2, 0x780000, 0x780000);
    Func_02004bce(1, 0, 0);
    Func_02004bd8(3, 0, 0);
    Call3(Func_02004be4, 2, 0xe000, 0);
    *(s32 *)(*(u8 **)Data_03001ebc + 0x1c8) = 60;
    Func_02004c34();
    Func_02004c40();
    Func_02004b26(20);
    *(s32 *)(*(u8 **)Data_03001ebc + 0x1c8) = 24;
    Call3(Func_02004b6a, 3, 0x10000, 0x8000);
    Func_02004c9c(3, 16, 0);
    Call3(Func_02004c28, 3, 0x2000, 0);
    Func_02004b56(20);
    Func_02004c06(3, 2);
    Func_02004b64(30);
    Func_02004c3c(3, 0);
    Func_02004b72(10);
    record = Func_02004ba0(0);
    *(s32 *)(record + 16) += -0x30000;
    record = Value1(Func_02004bae, 0);
    *(s32 *)(record + 64) += -0x30000;
    Func_02004c1c(0, 32);
    Func_02004b9a(40);
    Func_02004c32(0, 34);
    Func_02004ba8(30);
    Func_02004c38(0, 33);
    Func_02004bb6(50);
    Func_02004c66(1, 2);
    Func_02004bc4(30);
    Func_02004c9c_a(1, 0);
    Func_02004bd2(10);
    Call3(Func_02004cbc, 0, 0x105, 60);
    Func_02004be2(20);
    Call3(Func_02004cce, 1, 0x102, 60);
    Func_02004cc6(1, 0);
    Func_02004bfc(10);
    Func_02004c94(1, 4);
    Func_02004c0a(20);
    Func_02004ce2(1, 0);
    Func_02004c18(10);
    Call3(Func_02004d04, 0, 0x102, 80);
    Call3(Func_02004d10, 2, 0x106, 60);
    Call3(Func_02004d14, 2, 0xc000, 0);
    Func_02004c42(30);
    Func_02004cda(2, 4);
    Func_02004c50(20);
    Func_02004d28(2, 0);
    Func_02004c5e(10);
    Call3(Func_02004d42, 1, 0x4000, 0);
    Func_02004c70(30);
    Func_02004d20(1, 2);
    Func_02004c7e(45);
    Func_02004d60(1, 0, 0);
    Call3(Func_02004d6c, 2, 0xe000, 0);
    Func_02004c9a(30);
    Value2(Func_02004d6a, 1, 0);
    if (Value2(Func_02004ccc, -1, 0) != 0) {
        Func_02004cb6(10);
        Func_02004d4e(0, 34);
        Func_02004cc4(20);
        Func_02004d5c(1, 3);
        Func_02004cd2(20);
        Func_02004daa(1, 0);
        Func_02004ce0(10);
        Func_02004d78(0, 33);
        Func_02004cee(30);
        Func_02004d86(1, 3);
        Func_02004cfc(20);
        Func_02004dd4(1, 0);
        bump_step(1);
    } else {
        Func_02004d1a(10);
        Func_02004db2(0, 33);
        bump_step(2);
        Func_02004d36(30);
        Func_02004dce(1, 3);
        Func_02004d44(20);
        Func_02004e1c(1, 0);
    }
    Func_02004d52(10);
    Call3(Func_02004d90, 1, 0x10000, 0x8000);
    Call3(Func_02004ec4, 1, -16, 0);
    Func_02004e4e(1, 0, 0);
    Func_02004d7c(35);
    Func_02004e1e(0, 6, 0);
    Call3(Func_02004dc0, 0, 0x1e666, 0xf333);
    Call3(Func_02004ef4, 0, -32, 0);
    record = Func_02004dca(0);
    Func_02004d48(record, 1);
    Call3(Func_02004e8c, 3, 0x4000, 0);
    Call3(Func_02004e98, 2, 0xc000, 0);
    Func_02004dc6(40);
    Func_02004e56(0, 3);
    Func_02004dd4_a(30);
    Func_02004e64(2, 3);
    Func_02004e6c(1, 3);
    Func_02004e7c(3, 3);
    Func_02004df2(30);
    Call3(Func_02004e2c, 1, 0x13333, 0x9999);
    Call3(Func_02004e36, 3, 0x13333, 0x9999);
    Call3(Func_02004e40, 2, 0x13333, 0x9999);
    Func_02004ea0(1, 2);
    record = Value1(Func_02004e46, 0);
    if (record != 0) {
        Func_02004e80(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Func_02004eae(1);
    Func_02004ec0(1, 0, 0);
    Func_02004ed0(3, 2);
    record = Value1(Func_02004e76, 0);
    if (record != 0) {
        Func_02004eb0(3, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Func_02004ede(3);
    Func_02004ef0(3, 0, 0);
    Func_02004f00(2, 2);
    record = Value1(Func_02004ea6, 0);
    if (record != 0) {
        Func_02004ee0(2, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Func_02004f0e(2);
    Func_02004f20(2, 0, 0);
    ((void (*)())Func_02004ea6_a)(10);
    Func_02004eba();
}

void FieldScene_RunScene3b8SequenceA(void)
{
    s32 record;
    s16 dir;
    u16 facing;

    record = Func_02004efe(0);
    dir = (*(u16 *)(record + 6) + 0x2000) & -0x4000;
    Func_02004ef2();
    Func_0200501e();
    Func_02004fb4((s32)&Value_00001ffb);
    Value2(Func_02004fc6, -1, 0);
    if (Value2(Func_02004f26, 0, 0) != 0) {
    } else {
        if (Value1(Func_02004eda, 0x96a) != 0) {
            Func_02004f2c(20);
            Func_0200509a(0);
            goto L_02000fe2;
        }
        Func_02004f3a(20);
        facing = dir;
        if (facing == 0) {
            Func_02004fac(0, 128, 120);
        }
        if (facing == 0x8000) {
            Func_02004fbe(0, 240, 120);
        }
        Func_02004fc8(0, 184, 120);
        Func_0200504a(0, 0, 0);
        Func_02004f78(10);
        Func_020050cc(1, 16, 0, 0x8000);
        Func_02004fca(1);
        Func_02004f90(10);
        Func_02005068(1, 0);
        Func_02004f9e(10);
        Func_02005036(0, 3);
        Func_02004fac_a(20);
        Call3(Func_02004fe8, 1, 0x10000, 0x8000);
        Func_0200511a(1, 40, 0);
        Call3(Func_02005126, 1, 0, -32);
        Call3(Func_020050b2, 1, 0x6000, 0);
        Func_02004fe0(20);
        Func_020050c2(0, 0x8000, 0);
        Func_02004ff0(20);
        Call4(Func_02005146, 2, -16, 0, 0);
        Func_02005044(2);
        Func_0200500a(10);
        Func_020050e2(2, 0);
        Func_02005018(10);
        Func_020050b0(0, 3);
        Func_02005026(20);
        Call3(Func_02005062, 2, 0x10000, 0x8000);
        Call3(Func_02005196, 2, -40, 0);
        Func_020051a0(2, 0, 40);
        Call3(Func_0200512c, 2, 0xe000, 0);
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
        Call3(Func_020050da, 3, 0x10000, 0x8000);
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
        ((void (*)())Func_02005130)(10);
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
        ((void (*)())Func_02005218)(10);
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
    L_02000fe2:;
}
