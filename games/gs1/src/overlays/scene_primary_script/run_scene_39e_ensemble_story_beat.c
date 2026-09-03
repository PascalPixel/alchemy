#include "types.h"

#define FieldScene_RunEnsembleStoryBeat Func_02001dbc

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
s32 Func_020061c6();
s32 Func_020061d0();
s32 Func_020061d8();
s32 Func_020061e0();
s32 Func_020061e8();
void Func_02006234();
void Func_0200623e();
s32 Func_02006242();
void Func_02006260();
void Func_02006272();
void Func_02006288();
void Func_020062ac();
void Func_020062b2();
void Func_020062b6();
void Func_020062ba();
void Func_020062ca();
void Func_020062d4();
void Func_020062dc();
void Func_020062de();
void Func_020062e8();
void Func_020062ee();
void Func_020062f2();
void Func_020062f8();
void Func_020062fc();
void Func_02006304();
void Func_02006306();
void Func_0200630e();
void Func_0200631a();
void Func_02006332();
void Func_02006340();
void Func_02006350();
void Func_02006372();
void Func_0200637c();
void Func_02006392();
void Func_020063a0();
void Func_020063a8();
void Func_020063b2();
void Func_020063b4();
void Func_020063bc();
void Func_020063c2();
void Func_020063cc();
void Func_020063d0();
void Func_020063d2();
void Func_020063e6();
void Func_020063fc();
void Func_02006404();
void Func_0200641c();
void Func_0200642a();
void Func_0200643c();
void Func_02006448();
void Func_0200644c();
void Func_0200644e();
void Func_02006452();
void Func_02006456();
void Func_02006476();
void Func_0200647e();
void Func_02006480();
void Func_02006486();
void Func_0200648e();
void Func_02006492();
void Func_02006496();
void Func_02006498();
void Func_0200649e();
void Func_020064a6();
void Func_020064ae();
void Func_020064b6();
void Func_020064be();
void Func_020064c4();
void Func_020064c6();
void Func_020064d6();
void Func_020064e6();
void Func_02006506();
void Func_0200650e();
void Func_02006526();
void Func_02006534();
void Func_02006542();
s32 Func_0200654e();
void Func_02006568();
void Func_02006570();
void Func_02006578();
void Func_02006588();
void Func_0200658e();
void Func_02006598();
void Func_020065a8();
void Func_020065b2();
void Func_020065b8();
void Func_020065c4();
void Func_020065c8();
void Func_020065ca();
void Func_020065d6();
void Func_020065d8();
void Func_020065f0();
void Func_02006604();
void Func_02006606();
void Func_02006610();
void Func_02006612();
void Func_0200661a();
void Func_02006622();
void Func_02006628();
void Func_0200662c();
void Func_02006636();
void Func_02006640();
void Func_0200664a();
void Func_02006652();
void Func_02006654();
void Func_0200665a();
void Func_02006664();
void Func_0200667a();
void Func_02006682();
void Func_0200668a();
void Func_02006692();
void Func_0200669a();
void Func_0200669c();
void Func_020066a2();
void Func_020066aa();
void Func_020066b0();
void Func_020066cc();
void Func_020066da();
void Func_020066dc();
void Func_020066e4();
void Func_020066ec();
void Func_020066f4();
void Func_020066fc();
void Func_02006704();
void Func_0200670c();
void Func_02006714();
s32 Func_02006724();
void Func_02006734();
void Func_02006744();
void Func_0200674a();
void Func_0200674c();
void Func_0200675c();
void Func_02006786();
void Func_0200678e();
void Func_02006796();
void Func_0200679a();
void Func_0200679e();
void Func_020067a4();
void Func_020067a6();
void Func_020067ae();
void Func_020067b0();
void Func_020067b6();
void Func_020067c6();
void Func_020067ce();
s32 Func_020067d0();
void Func_020067d2();
void Func_020067d6();
void Func_020067de();
void Func_020067f0();
void Func_020067f2();
void Func_020067fa();
void Func_020067fc();
s32 Func_020067fe();
void Func_02006806();
s32 Func_0200680a();
void Func_0200680c();
void Func_0200680e();
s32 Func_02006816();
void Func_02006840();
void Func_0200685a();
void Func_02006864();
void Func_020068a2();
void Func_02006900();
void Func_0200699c();

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

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)0x03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunEnsembleStoryBeat(void)
{
    u32 i;
    s32 record;

    record = Func_020061c6(18);
    *(s32 *)(record + 108) = 0;
    record = Value1(Func_020061d0, 13);
    *(s32 *)(record + 108) = 0;
    record = Value1(Func_020061d8, 14);
    *(s32 *)(record + 108) = 0;
    record = Value1(Func_020061e0, 15);
    *(s32 *)(record + 108) = 0;
    record = Func_020061e8(16);
    *(s32 *)(record + 108) = 0;
    Value2(Func_02006242, 11, 1);
    Call2(Func_020062de, 0x8000, 0x1000);
    Call4(Func_020062f8, 0xe80000, -1, 0xc80000, 1);
    Func_02006304();
    Call1(Func_020062b2, 0x1883);
    Call3(Func_02006234, 10, 0xcccc, 0x6666);
    Call3(Func_0200623e, 12, 0xcccc, 0x6666);
    Func_02006260(10, 152, 200);
    Func_02006272(12, 144, 248);
    Func_02006288(10);
    Func_020062ca(9, 19, 0);
    Func_020062d4(11, 19, 0);
    Func_020062de(13, 19, 0);
    Func_020062e8(14, 19, 0);
    Func_020062f2(15, 19, 0);
    Func_020062fc(16, 19, 0);
    Func_02006306(18, 19, 0);
    Call3(Func_020062ac, 10, 0x18000, 0xc000);
    Call3(Func_020062ba, 12, 0x20000, 0x10000);
    Func_020062dc(10, 152, 200);
    Func_020062ee(12, 144, 248);
    Func_02006340(12, 19, 0);
    Func_0200630e(10);
    Func_02006350(10, 19, 0);
    Func_02006350(18, 2);
    Func_020062b6(20);
    Func_020063a0(18, 0, 40);
    Func_02006372(9, 18, 0);
    Func_0200637c(10, 18, 0);
    Call3(Func_020063d0, 11, 0x3000, 0);
    Func_02006392(12, 18, 0);
    Call3(Func_020063e6, 13, 0x3000, 0);
    Func_020063a8(14, 18, 0);
    Func_020063b2(15, 18, 0);
    Func_020063bc(16, 18, 0);
    Func_0200631a(20);
    Func_02006404(16, 0, 20);
    Func_020063b4(18, 3);
    Func_02006332(20);
    Func_020063c2(16, 3);
    Func_02006340(20);
    Func_0200642a(16, 0, 20);
    Call3(Func_0200644c, 18, 0x105, 60);
    Call3(Func_02006456, 16, 0x101, 60);
    Func_02006448(16, 0, 20);
    Func_02006452(18, 0, 20);
    Call3(Func_02006476, 16, 0x102, 60);
    Call3(Func_02006480, 15, 0x101, 60);
    Call3(Func_020063d2, 15, 0xcccc, 0x6666);
    Func_020063fc(15, 216, 176);
    Call3(Func_02006498, 15, 0x3000, 20);
    Func_02006492(15, 0, 20);
    Call3(Func_020064ae, 18, 0xb000, 20);
    Func_0200644e(18, 4);
    Func_020063cc(20);
    Func_020064b6(18, 0, 20);
    Func_02006476(9, 2);
    Func_0200647e(10, 2);
    Func_02006486(11, 2);
    Func_0200648e(12, 2);
    Func_02006496(13, 2);
    Func_0200649e(14, 2);
    Func_020064a6(15, 2);
    Func_020064ae(16, 2);
    Func_0200641c(40);
    Func_020064c4(13, 2);
    Func_0200650e(13, 0, 20);
    Func_020064be(18, 3);
    Func_0200643c(20);
    Func_02006526(18, 0, 20);
    Call3(Func_02006542, 0, 0xe000, 20);
    Call3(Func_0200654e, 18, 0x5000, 20);
    Value2(Func_0200654e, 18, 0);
    Call3(Func_02006568, 9, 0x101, 0);
    Func_02006476(5);
    Call3(Func_02006578, 10, 0x101, 0);
    Func_02006486(5);
    Call3(Func_02006588, 11, 0x101, 0);
    Func_02006496(5);
    Call3(Func_02006598, 12, 0x101, 0);
    Func_020064a6(5);
    Call3(Func_020065a8, 13, 0x101, 0);
    Func_020064b6(5);
    Call3(Func_020065b8, 14, 0x101, 0);
    Func_020064c6(5);
    Call3(Func_020065c8, 15, 0x101, 0);
    Func_020064d6(5);
    Call3(Func_020065d8, 16, 0x101, 0);
    Func_020064e6(60);
    Func_0200658e(16, 2);
    Func_020065d8(16, 0, 20);
    Func_02006588(18, 3);
    Func_02006506(20);
    Func_020065f0(18, 0, 20);
    Call3(Func_02006612, 15, 0x101, 60);
    Func_02006604(15, 0, 20);
    Func_020065d6(18, 15, 0);
    Func_02006534(20);
    Func_020065c4(18, 4);
    Func_02006542(20);
    Func_0200662c(18, 0, 40);
    Func_02006606(11, 10, 0);
    Func_02006610(12, 14, 0);
    Func_0200661a(13, 15, 0);
    Func_02006570(60);
    Func_02006622(10, 18, 0);
    Func_0200662c(11, 18, 0);
    Func_02006636(12, 18, 0);
    Func_02006640(13, 18, 0);
    Func_0200664a(14, 18, 0);
    Func_02006654(15, 18, 0);
    Func_020065b2(20);
    Func_0200669c(18, 0, 20);
    Func_02006664(18, 2);
    Func_020065ca(20);
    Func_02006652(9, 3);
    Func_0200665a(10, 3);
    Func_0200667a(11, 3);
    Func_02006682(12, 3);
    Func_0200668a(13, 3);
    Func_02006692(14, 3);
    Func_0200669a(15, 3);
    Func_020066aa(16, 3);
    Func_02006628(20);
    Call3(Func_02006724, 18, 0x5000, 20);
    Value2(Func_02006724, 18, 0);
    Func_020066cc(18, 3);
    Func_0200664a(20);
    Func_02006734(18, 0, 20);
    Func_020066dc(0, 3);
    Func_020066e4(9, 3);
    Func_020066ec(10, 3);
    Func_020066f4(11, 3);
    Func_020066fc(12, 3);
    Func_02006704(13, 3);
    Func_0200670c(14, 3);
    Func_02006714(15, 3);
    ((void (*)())Func_02006724)(16, 3);
    Func_020066a2(20);
    Func_0200674a(18, 2);
    Func_020066b0(20);
    Func_0200679a(18, 0, 20);
    Func_020067a4(18, 0, 20);
    Func_0200674c(0, 3);
    Func_0200675c(18, 3);
    Func_020066da(20);
    Call3(Func_020067d6, 18, 0x8000, 20);
    Func_0200678e(18, 2);
    Func_020066f4(20);
    Func_020067de(18, 0, 20);
    Func_02006786(9, 3);
    Func_0200678e(10, 3);
    Func_02006796(11, 3);
    Func_0200679e(12, 3);
    Func_020067a6(13, 3);
    Func_020067ae(14, 3);
    Func_020067b6(15, 3);
    Func_020067c6(16, 3);
    Func_02006744(20);
    Func_020067a6(10, 120, 200);
    Func_020067b0(12, 120, 248);
    Func_020067ce(10);
    Call3(Func_0200685a, 11, 0x8000, 20);
    Func_020067f2(10, 5);
    Func_020067fa(11, 5);
    Func_020067f0(12);
    Value2(Func_020067d0, 12, 0x200c638);
    Call3(Func_020067d2, 15, 0xcccc, 0x6666);
    Func_020067fc(15, 216, 168);
    Func_02006806(15, 232, 168);
    Call3(Func_020068a2, 15, 0xc000, 20);
    Func_0200685a(15, 3);
    Call3(Func_02006840, 19, 0xe80000, 0xa80000);
    record = Func_020067fe(19);
    *(s32 *)(record + 12) = 0xc0000;
    record = Func_0200680a(19);
    *(s32 *)(record + 60) = -0x80000000;
    record = Func_02006816(19);
    {
        s32 target = *(s32 *)(record + 80);
        s32 shown = 0x8000;
    
        *(u16 *)(target + 30) = shown;
    }
    Func_0200699c(124);
    Func_020067fa(40);
    Func_02006864(15, 216, 152);
    Call3(Func_02006900, 15, 0x4000, 30);
    Call1(Func_0200680e, 0x898);
    Call1(Func_0200680c, 0x89b);
}
