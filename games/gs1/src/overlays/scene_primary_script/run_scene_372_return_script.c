#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_0200d7f8[];
extern u8 Data_0200d7fc[];
extern u8 Data_03001ebc[];
void Func_02005f86();
void Func_02005f90();
void Func_02005f9a();
void Func_02005fa4();
void Func_0200608e();
void Func_02006180();
s32 Func_02006206();
void Func_02006216();
void Func_0200622c();
void Func_02006230();
void Func_02006246();
void Func_02006254();
void Func_02006270();
void Func_020062a6();
s32 Func_020062ae();
void Func_020062c2();
void Func_020062da();
void Func_020062e8();
void Func_020062ea();
s32 Func_020062f8();
void Func_02006306();
void Func_0200630e();
void Func_02006318();
void Func_0200631c();
void Func_02006320();
void Func_0200632c();
s32 Func_02006350();
void Func_0200635e();
s32 Func_0200637e();
void Func_020063c8();
void Func_020063d0();
void Func_020063d8();
void Func_020063da();
void Func_020063e0();
void Func_020063e6();
s32 Func_020063f2();
s32 Func_020063fc();
void Func_020063fe();
s32 Func_02006406();
s32 Func_02006410();
void Func_02006414();
void Func_02006420();
void Func_0200642a();
void Func_02006430();
void Func_0200643a();
void Func_02006450();
void Func_02006462();
void Func_0200646a();
s32 Func_0200646c();
void Func_02006470();
void Func_02006472();
void Func_0200647a();
void Func_0200648e();
void Func_02006492();
void Func_0200649a();
void Func_020064a4();
void Func_020064a8();
void Func_020064c0();
void Func_020064c6();
void Func_020064cc();
void Func_020064d2();
void Func_020064de();
void Func_020064ea();
void Func_02006502();
void Func_02006506();
void Func_02006508();
void Func_0200650a();
void Func_02006514();
void Func_02006538();
void Func_0200653a();
void Func_02006540();
void Func_0200655c();
void Func_02006568();
void Func_02006570();
void Func_02006572();
void Func_02006576();
void Func_020065a6();
void Func_020065ac();
void Func_020065dc();
void Func_02006610();
void Func_0200662e();
void Func_02006632();
void Func_02006638();
void Func_0200664a();
void Func_02006656();
void Func_0200665c();
void Func_02006666();
void Func_0200666e();
void Func_02006670();
void Func_02006686();
void Func_0200668c();
void Func_0200669a();
void Func_0200669e();
void Func_020066a2();
void Func_020066a6();
void Func_020066be();
void Func_020066c4();
void Func_020066d4();
void Func_020066dc();
void Func_020066e8();
void Func_020066ec();
void Func_020066f2();
void Func_020066f8();
void Func_020066fe();
void Func_0200670a();
void Func_02006712();
void Func_02006718();
s32 Func_02006758();
void Func_0200675c();
void Func_02006768();
void Func_02006772();
void Func_02006780();
void Func_02006786();
void Func_02006788();
void Func_0200678e();
void Func_02006794();
void Func_0200679a();
void Func_020067a0();
void Func_020067a2();
void Func_020067a4();
void Func_020067a6();
void Func_020067b4();
void Func_020067ba();
void Func_020067c2();
void Func_020067c8();
void Func_020067d8();
void Func_020067da();
void Func_020067dc();
void Func_020067e8();
void Func_020067ee();
void Func_02006840();
void Func_0200686c();
void Func_0200c56d();

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
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunReturnScript(void)
{
    u32 i;
    s32 p10;
    s32 p11;
    s32 p9;
    s32 record;
    s32 r9;
    s32 r10;
    s32 r8;
    s32 base5_200d7fc;
    s32 base7_200c56d;
    s32 base_200c5a9;
    s32 shifted;
    s32 v5;
    s32 v6;
    s32 v7;

    if (Value1(Func_02006206, 0x838) != 0) {
        v5 = r9;
        v6 = r10;
        v7 = r8;
    } else {
        Func_02006230();
        Call1(Func_02006246, 0x200d4b0);
        Func_0200608e();
        Func_02006180(1);
        Func_020063fe(141);
        Call3(Func_02006216, 0x20000, 0x20000, 0x10000);
        Func_02006254(30);
        Call3(Func_0200622c, 0x30000, 0x30000, 0x10000);
        Func_0200642a(145);
        Func_02006270(30);
        record = Value1(Func_020062ae, 0);
        if (record != 0) {
            Func_0200631c(22, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
        }
        Call3(Func_020062da, 0, 0x20000, 0x10000);
        Call3(Func_020062e8, 22, 0x20000, 0x10000);
        Value2(Func_020062f8, 0, 0x200ca00);
        Call2(Func_02006318, 22, 0x200ca3c);
        Func_0200630e(0);
        Call3(Func_020063da, 0, 0x100, 0);
        Call3(Func_020063e6, 22, 0x100, 30);
        Call3(Func_020062a6, 0x40000, 0x40000, 0x10000);
        Func_020064a4(145);
        Func_020062ea(40);
        Call3(Func_020062c2, 0x50000, 0x50000, 0x10000);
        Func_020064c0(145);
        Func_02006306(20);
        Call2(Func_02006430, 0, 0x102);
        Call2(Func_0200643a, 22, 0x102);
        Func_02006320(40);
        Func_020063c8(32, 5);
        Func_020063d0(33, 5);
        Func_020063d8(30, 8);
        Func_020063e0(29, 8);
        record = Func_0200637e(30);
        *(volatile s32 *)(record + 24) = -0x10000;
        Func_02006462(32, 2);
        Func_0200646a(33, 2);
        Func_02006472(30, 3);
        Func_0200647a(29, 3);
        Call1(Func_02006450, 0xe7f);
        Func_02006472(28, 0, 20);
        Call3(Func_0200648e, 0, 0xc000, 0);
        Call3(Func_0200649a, 22, 0xc000, 20);
        Call2(Func_020064c6, 0x40000, 0x8000);
        Call4(Func_020064de, 0x700000, -1, 0x14b0000, 1);
        Func_020064ea();
        for (i = 0; i < 40; i++) {
            Func_02005f86(Func_020063f2(32));
            Func_02005f90(Func_020063fc(33));
            Func_02005f9a(Func_02006406(30));
            Func_02005fa4(Func_02006410(29));
            Func_0200632c(1);
        }
        base5_200d7fc = (s32)Data_0200d7fc;
        base7_200c56d = (s32)Func_0200c56d;
        v5 = base5_200d7fc;
        v6 = 0;
        v7 = base7_200c56d;
        *(volatile s32 *)Data_0200d7f8 = 0;
        *(volatile s32 *)base5_200d7fc = 0;
        Value2(Func_02006350, base7_200c56d, 0xc80);
        base_200c5a9 = 0x200c5a9;
        Call2(Func_0200635e, base_200c5a9, 0xc80);
        Func_02006414(40);
        *(volatile s32 *)base5_200d7fc = 1;
        Func_02006420(30);
        Call3(Func_020064c6, 19, 0x720000, 0x1220000);
        record = Func_0200646c(19);
        shifted = *(s32 *)(record + 12) + 0x400000;
        *(s32 *)(record + 12) = shifted;
        *(s32 *)(record + 60) = shifted;
        Call3(Func_02006492, 19, 0xcccc, 0x6666);
        Func_02006610(145);
        Call3(Func_020064d2, 19, 114, 0x14d);
        Func_02006502(19, 2);
        Call3(Func_0200643a, 0x20000, 0x20000, 0x10000);
        Func_02006638(145);
        *(volatile s32 *)base5_200d7fc = 0;
        Call3(Func_020064cc, 19, 0x6666, 0x3333);
        Call3(Func_02006508, 19, 114, 0x12c);
        Func_02006538(19, 2);
        Call3(Func_02006470, 0x50000, 0x50000, 0x10000);
        Func_0200666e(145);
        *(volatile s32 *)base5_200d7fc = 2;
        Call3(Func_02006506, 19, 0xcccc, 0x6666);
        Call3(Func_02006540, 19, 114, 0x14d);
        Func_02006570(19, 2);
        Call3(Func_020064a8, 0x20000, 0x20000, 0x10000);
        Func_020066a6(145);
        *(volatile s32 *)base5_200d7fc = 0;
        Call3(Func_0200653a, 19, 0x6666, 0x3333);
        Call3(Func_02006576, 19, 114, 0x12c);
        Func_020065a6(19, 2);
        Call3(Func_020064de, 0x40000, 0x40000, 0x10000);
        Func_020066dc(145);
        *(volatile s32 *)base5_200d7fc = 2;
        Call3(Func_02006572, 19, 0xcccc, 0x6666);
        Call3(Func_020065ac, 19, 114, 0x14d);
        Func_020065dc(19, 2);
        Call3(Func_02006514, 0x20000, 0x20000, 0x10000);
        Func_02006712(145);
        *(volatile s32 *)base5_200d7fc = 1;
        Func_0200655c(20);
        Call2(Func_02006686, 32, 0x102);
        Func_0200662e(32, 2);
        Func_02006666(31, 0);
        Call3(Func_0200669a, 33, 0x100, 0);
        Func_0200664a(33, 2);
        Func_0200668c(28, 0, 40);
        Call2(Func_020066be, 30, 0x102);
        Func_02006666(30, 2);
        Func_0200669e(30, 0);
        *(volatile s32 *)Data_0200d7f8 = 1;
        Func_0200665c(29, 1);
        Func_0200650a(1);
        Func_020066a2(29, 0);
        Call3(Func_020066ec, 29, 0x105, 20);
        Call3(Func_020066e8, 29, 0x8000, 40);
        Func_020066f2(29, 0, 20);
        Call3(Func_020066fe, 29, 0x8000, 20);
        Call3(Func_0200670a, 29, 0x4000, 40);
        Call3(Func_02006768, 29, 0x100, 0);
        Func_02006718(29, 2);
        Func_02006712(29, 4, 40);
        Func_0200670a(29, 9);
        Func_02006670(10);
        Func_02006772(29, 0, 20);
        Call1(Func_02006840, 0x121);
        Call3(Func_02006656, -1, -1, 0xe666);
        Call2(Func_020067c2, 0x60000, 0xc000);
        Call4(Func_020067dc, 0x540000, -1, 0x2340000, 1);
        Func_020067e8();
        Func_0200686c();
        Func_02006786(22, 0, 0);
        Func_020066c4(20);
        Call2(Func_020067ee, 22, 0x102);
        Func_020066d4(30);
        Func_02006632(base7_200c56d);
        Func_02006638(base_200c5a9);
        Func_020067d8(22, 0);
        Func_020067ba(0, 22, 0);
        Func_020066f8(20);
        Func_02006568();
        Func_020067a4(0, 3);
        Func_020067b4(22, 3);
        Func_02006712(20);
        Func_020067ba(22, 2);
        record = Value1(Func_02006758, 0);
        if (record != 0) {
            Func_020067a2(22, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Func_020067c8(22);
        Func_020067da(22, 0, 0);
        Func_02006788(31);
        Func_0200678e(28);
        Func_02006794(30);
        Func_0200679a(29);
        Func_020067a0(32);
        Func_020067a6(33);
        Call1(Func_0200675c, 0x838);
        Func_02006780();
    }
    p9 = v5;
    p10 = v6;
    p11 = v7;
}
