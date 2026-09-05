#include "types.h"

#define FieldScene_RunScene380SequenceA Func_0200178c

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_000010b0[];
extern u8 Data_000010b2[];
void Func_02005a3e();
void Func_02005a70();
void Func_02005a9a();
void Func_02005abe();
void Func_02005adc();
void Func_02005af4();
void Func_02005b04();
void Func_02005b1a();
void Func_02005b46();
void Func_02005b56();
void Func_02005b6c();
void Func_02005b84();
void Func_02005b94();
void Func_02005ba4();
void Func_02005bde();
void Func_02005d08();
void Func_02005d3c();
void Func_02005d62();
void Func_02005e0e();
void Func_02006134();
void Func_02006158();
s32 Func_02006162();
void Func_0200617a();
void Func_02006198();
void Func_020061b8();
void Func_020061c2();
void Func_020061c6();
void Func_020061ca();
void Func_020061f4();
void Func_02006202();
void Func_02006204();
void Func_0200620c();
void Func_02006212();
void Func_0200621a();
void Func_0200621c();
void Func_02006220();
void Func_02006230();
void Func_02006244();
void Func_0200624c();
void Func_0200625e();
void Func_02006276();
void Func_0200627c();
void Func_02006286();
void Func_0200628a();
void Func_02006294();
void Func_0200629a();
void Func_020062a2();
void Func_020062a6();
void Func_020062b0();
void Func_020062be();
void Func_020062c8();
void Func_020062e0();
void Func_020062f0();
void Func_020062fe();
void Func_02006304();
void Func_0200630e();
void Func_0200631a();
void Func_02006340();
void Func_02006342();
void Func_02006358();
void Func_0200635a();
void Func_02006370();
void Func_02006380();
s32 Func_020063a2();
void Func_020063a6();
void Func_020063b2();
void Func_020063c0();
void Func_020063ce();
void Func_020063e4();
void Func_020063f4();
void Func_020063fe();
void Func_02006402();
void Func_0200640a();
void Func_02006410();
void Func_02006418();
void Func_0200641a();
void Func_02006426();
void Func_0200643e();
void Func_02006448();
s32 Func_0200644e();
void Func_0200645a();
void Func_0200646e();
void Func_0200647e();
void Func_02006486();
void Func_02006492();
void Func_020064a0();
void Func_020064b4();
void Func_020064b8();
void Func_020064ba();
void Func_020064c0();
void Func_020064c2();
void Func_020064c6();
s32 Func_020064ee();
s32 Func_020064fa();
void Func_020064fc();
void Func_02006500();
void Func_02006526();
void Func_0200652a();
void Func_02006536();
void Func_02006540();
void Func_0200654c();
void Func_02006556();
void Func_0200657e();
void Func_0200658c();
void Func_020065c0();
void Func_020065d2();
s32 Func_020065d6();
void Func_020065d8();
void Func_020065e2();
void Func_02006602();
void Func_0200661a();
void Func_0200661c();
void Func_0200662a();
void Func_02006632();
void Func_02006640();
void Func_02006648();
void Func_02006656();
void Func_0200665e();
void Func_02006678();
void Func_0200667a();
void Func_0200668a();
void Func_0200668e();
void Func_020066a4();
void Func_020066a6();
void Func_020066b6();
void Func_020066ba();
void Func_020066d0();
void Func_020066e6();
void Func_02006776();
void Func_020067a2();
void Func_020067ce();

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

void FieldScene_RunScene380SequenceA(void)
{
    u8 *rec;
    s32 record;
    s32 base6_10b0;
    s32 base6_10b2;
    s32 count;
    s32 n;
    u16 *slot;
    s32 item;

    Func_020062a6(161);
    Func_020061c6(12, 3);
    Func_02006134(40);
    record = Value1(Func_02006162, 12);
    if (record != 0) {
        Func_020061b8(13, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Func_020061c2(12, 0, 0);
    Func_02006158(20);
    Call3(Func_0200624c, 13, 0x3000, 40);
    Func_02006204(5, 3);
    Func_0200620c(5, 3);
    Func_0200617a(40);
    Func_0200621a(5, 2);
    Func_02005a3e(5, 20);
    Func_02006212(13, 3);
    Func_02006198(10);
    Func_02006230(5, 3);
    Call3(Func_02006294, 9, 0x8000, 10);
    Func_0200624c(9, 2);
    Func_02005a70(9, 40);
    Func_02006244(5, 3);
    Func_020061ca(40);
    Call3(Func_020062be, 9, 0xb000, 40);
    Func_02006276(13, 2);
    Func_02005a9a(13, 20);
    Func_02006286(5, 1);
    Func_020061f4(40);
    Func_0200627c(13, 3);
    Func_02006202(10);
    Func_02005abe(13, 40);
    Func_020062a2(10, 1);
    Func_0200629a(10, 3);
    Func_02006220(10);
    Func_02005adc(10, 10);
    Func_020062c8(11, 1);
    Func_020062b0(11, 3);
    Func_02005af4(11, 80);
    Func_020062e0(13, 2);
    Func_02005b04(13, 40);
    Func_020062f0(5, 2);
    Func_0200625e(10);
    Func_02005b1a(5, 10);
    Func_020062fe(13, 2);
    Func_0200630e(9, 2);
    Func_0200627c(80);
    Func_02006304(5, 4);
    Func_0200628a(20);
    Func_02005b46(5, 80);
    Func_0200631a(13, 4);
    Func_02005b56(13, 80);
    Func_02006342(5, 2);
    Func_020062b0(4);
    Func_02005b6c(5, 20);
    Func_02006358(10, 1);
    Func_02006340(10, 3);
    Func_02005b84(10, 10);
    Func_02006370(11, 1);
    Func_02005b94(11, 10);
    Func_02006380(10, 1);
    Func_02005ba4(10, 10);
    Call3(Func_020063e4, 9, 0x3000, 80);
    Call3(Func_020063fe, 9, 0x105, 80);
    Func_020063a6(11, 1);
    Call3(Func_02006402, 11, 0x5000, 40);
    Func_020063b2(11, 2);
    Func_02005bde(11, 20);
    Func_02006486();
    Func_02006492();
    Call4(Func_0200645a, 0x1050000, -1, 0x1d20000, 0);
    Func_02006286();
    Func_0200621c(1);
    Func_020064a0();
    Func_020064b4();
    Func_0200635a(20);
    Call3(Func_0200644e, 0, 0x3000, 0);
    Call3(Func_0200645a, 1, 0xb000, 20);
    Func_0200640a(0, 2);
    Func_0200641a(1, 2);
    Call3(Func_020063c0, 0, 0x8000, 0x4000);
    Call3(Func_020063ce, 1, 0x8000, 0x4000);
    Call3(Func_02006402, 0, 244, 0x1de);
    Call3(Func_02006418, 1, 0x104, 0x1ea);
    Func_02006426(0);
    Func_0200643e(0, 1);
    Call3(Func_020064ba, 0, 0x3000, 0);
    Call3(Func_020064c6, 1, 0xb000, 20);
    Func_0200647e(0, 2);
    Func_0200646e(1, 4);
    Func_020063f4(20);
    slot = (u16 *)Func_020063a2(1);
    count = 0;
    slot = (u16 *)((u8 *)slot + 0xd8);
    for (n = 14; n >= 0; n--) {
        item = *slot++ & 0x1ff;
        if ((u32)(item - 220) <= 1 || item == 223) {
            count++;
        }
    }
    Value2(Func_020064ee, 1, 0);
    if (Value2(Func_0200644e, 0, 0) == 0) {
        base6_10b0 = (s32)Data_000010b0;
        Call1((void (*)())Func_020064fa, base6_10b0);
        Func_020064c2(1, 3);
        Func_02006448(10);
        if (count <= 2) {
            Func_02005d08(1, 30);
            Call3(Func_020064c0, 1, 252, 0x1e6);
            Func_02006500(1, 2);
            Func_0200646e(10);
            Func_02006410((base6_10b0 + 1), 1, 0);
            goto L_02001bdc;
        }
        Call1(Func_02006540, 0x10b4);
        Func_02005d3c(1, 30);
    } else {
        if (count <= 2) {
            base6_10b2 = (s32)Data_000010b2;
            Func_02006556(base6_10b2);
            Func_02006536(1, 3);
            Func_02006526(1, 4);
            Func_02005d62(1, 10);
            Func_02006536(1, 4);
            Func_02006556(1, 1);
            Call3(Func_020064fc, 1, 0x20000, 0x10000);
            rec = Value1(Func_020064fa, 0);
            rec[90] &= 254;
            Call3(Func_0200654c, 1, 244, 0x1de);
            Call3(Func_0200652a, 0, 0x18000, 0xc000);
            Func_0200658c(0, 6, 0);
            Call3(Func_02006556, 0, 218, 0x1d7);
            Func_0200657e(0);
            Func_020064b8((base6_10b2 + 1), 1, 0);
            Func_020065c0(0, 2);
            Func_0200661a(0, 0, 30);
            {
                u8 value = *(volatile u8 *)&rec[90];
            
                rec[90] = (u8)(value | 1);
            }
        } else {
            Call1(Func_02006602, 0x10b5);
            Func_020065e2(1, 3);
            Func_020065d2(1, 4);
            Func_02005e0e(1, 10);
            Func_020065e2(1, 4);
            Call3(Func_02006656, 0, 0xe000, 30);
        }
    }
    L_02001bdc:;
    Call2(Func_0200668a, 0x8000, 0x1000);
    Func_0200668a(1, 1);
    Func_020066a6();
    Call3(Func_0200667a, 1, 0x8000, 30);
    Func_02006632(1, 2);
    Call3(Func_020065d8, 1, 0x8000, 0x4000);
    rec = Value1(Func_020065d6, 1);
    rec[90] &= 254;
    Call3(Func_0200662a, 1, 0x108, 0x1e2);
    {
        u8 value = *(volatile u8 *)&rec[90];
    
        rec[90] = (u8)(value | 1);
    }
    Call3(Func_02006640, 1, 0x116, 0x1e0);
    *(s32 *)(rec + 48) = 0x30000;
    *(s32 *)(rec + 52) = 0x20000;
    Func_02006776(153);
    *(s32 *)(rec + 40) = 0x60000;
    Func_02006678(1, 7);
    Call3(Func_0200665e, 1, 0x138, 0x1d6);
    Func_0200668e(1, 1);
    Func_0200661c(30);
    Func_020067a2(153);
    *(s32 *)(rec + 40) = 0x60000;
    Func_020066a4(1, 7);
    Call3(Func_0200668a, 1, 0x156, 0x1d6);
    Func_020066ba(1, 1);
    Func_02006648(30);
    Func_020067ce(153);
    *(s32 *)(rec + 40) = 0x60000;
    Func_020066d0(1, 7);
    Call3(Func_020066b6, 1, 0x178, 0x1d6);
    Func_020066e6(1, 1);
}
