#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_00001076[];
extern u8 Data_03001ebc[];
s32 Func_02004bc6();
void Func_02004c2c();
void Func_02005056();
void Func_0200506c();
void Func_0200507c();
void Func_0200509c();
void Func_020050a0();
void Func_020050aa();
void Func_020050b4();
void Func_020050c4();
void Func_020050ca();
void Func_020050d2();
void Func_020050fa();
void Func_02005102();
void Func_02005112();
void Func_02005124();
void Func_02005134();
void Func_02005138();
void Func_0200513e();
void Func_02005162();
void Func_0200516e();
void Func_02005180();
void Func_02005186();
void Func_02005198();
void Func_020051a8();
void Func_020051b2();
void Func_020051cc();
void Func_020051e0();
void Func_020051ec();
void Func_020051f6();
void Func_0200520c();
void Func_0200521a();
void Func_0200521c();
void Func_0200521e();
void Func_02005220();
void Func_02005226();
void Func_02005236();
void Func_02005238();
void Func_0200523c();
void Func_02005242();
void Func_02005248();
void Func_02005260();
void Func_0200526a();
void Func_0200528a();
void Func_0200529a();
s32 Func_0200529c();
void Func_020052b0();
void Func_020052bc();
void Func_020052be();
void Func_020052ce();
void Func_020052d4();
void Func_020052e8();
void Func_02005302();
void Func_0200530e();
void Func_02005324();
void Func_02005330();
void Func_02005338();
void Func_0200535e();
void Func_02005380();
void Func_020053a0();
void Func_020053ac();
void Func_020053bc();
void Func_020053ca();
void Func_020053d6();
void Func_020053f2();
void Func_020053f6();
void Func_02005408();
void Func_0200540a();
void Func_02005414();
void Func_02005430();
void Func_02005436();
void Func_0200545e();
void Func_02005488();
void Func_02005494();
void Func_020054a4();
void Func_020054a8();
void Func_020054b8();
void Func_020054d4();
void Func_020054e0();
void Func_020054ee();
void Func_02005502();

/* Inline calls keep argument constants local; direct calls permit sharing. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
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

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

void Func_020006f4(void)
{
    u32 i;
    s32 obj;
    s32 pos;
    s32 mes;

    Func_020050a0();
    Func_0200521e(141);
    for (i = 0; i != 6; i++) {
        Call2(Func_020051e0, 0x404a4e, 1);
        Func_020051f6(8);
        Func_020050b4(8);
        Call2(Func_020051f6, 0x10000, 1);
        Func_0200520c(8);
        Func_020050ca(8);
        if (i == 1) {
            Call3(Func_02005056, 0x10000, 0x10000, 0x10000);
        }
    }
    Call3(Func_0200506c, 0x10000, 0x20000, 0x10000);
    Func_020050fa(30);
    Func_0200507c(0, 0, 0);
    Call2(Func_0200521c, 0x59999, 0xb333);
    Call4(Func_02005236, 0x1d80000, -1, 0x620000, 1);
    Func_02005242();
    Call3(Func_020050aa, 0x10000, 0x20000, 0x10000);
    Func_02005138(20);
    Func_020052be(144);
    Func_0200509c(0x200d088, 84, 4);
    Call6(Func_020050c4, 0, 0, 1, 1, 29, 4);
    Call6(Func_020050d2, 87, 42, 29, 6, 1, 2);
    Func_02005180(40);
    Func_02005102(0, 0, 0);
    Call4(Func_020052b0, 0x1570000, -1, 0x1710000, 1);
    Func_020052bc();
    Call3(Func_02005124, 0x10000, 0x20000, 0x10000);
    Func_020051b2(20);
    Func_02005338(144);
    pos = 21;
    Func_02005112(0x200d088, 76, 21);
    Call6(Func_02005134, 0, 0, 1, 1, pos, pos);
    Call6(Func_0200513e, 87, 42, 21, 23, 1, 2);
    Func_020051ec(40);
    Func_0200516e(0, 0, 0);
    Call2(Func_0200530e, 0x33333, 0x6666);
    Call4(Func_02005324, 0x1570000, -1, 0x1f10000, 1);
    Func_02005330();
    Call3(Func_02005198, 0x10000, 0x20000, 0x10000);
    Func_02005226(20);
    Func_020053ac(144);
    Func_02005186(0x200d088, 76, 29);
    Call6(Func_020051a8, 0, 0, 1, 1, pos, 29);
    Call6(Func_020051b2, 87, 42, 21, 31, 1, 2);
    Func_02005260(40);
    *(s32 *)((*(u8 *volatile *)Data_03001ebc + 0x1c0)) = 0x202;
    Func_020053ca();
    Func_020053d6();
    Call4(Func_020053a0, 0x2c80000, -1, 0x980000, 0);
    Func_020051cc();
    Func_02005162(1);
    Call3(Func_0200521a, 0x20000, 0x10000, 0x10000);
    Func_020053f6();
    Func_0200540a();
    Func_020052b0(40);
    Call1(Func_02005436, 0x121);
    Call3(Func_0200523c, -1, -1, 0xe666);
    Func_02005248();
    Func_020052ce(20);
    Call6(Func_02005238, 0, 40, 43, 46, 3, 3);
    Func_020052e8(20);
    obj = Func_02004bc6(221, 0x2c80000, 0x100000, 0x900000);
    Func_02005302(40);
    Func_0200529a(obj, 1);
    mes = (s32)Data_00001076;
    Value2(Func_0200529c, mes, 1);
    Call3(Func_02005408, 9, 0xe000, 0);
    Call3(Func_02005414, 5, 0xe000, 20);
    Func_02005488();
    Func_02005494();
    Call4(Func_0200545e, 0x1ce0000, -1, 0x15e0000, 0);
    Func_0200528a();
    Func_02005220(1);
    Func_020054a4();
    Func_020054b8();
    Func_0200535e(40);
    Func_020053f2(9, 4, 30);
    Func_02005430(mes - 2);
    Func_02004c2c(9, 20);
    Func_020054d4();
    Func_020054e0();
    Func_020054a8(0x2c80000, -1, 0x980000, 0);
    Func_020052d4();
    Func_0200526a(1);
    Func_020054ee();
    Func_02005502();
    Call1(Func_02005380, 0x83d);
    Func_020053bc();
}
