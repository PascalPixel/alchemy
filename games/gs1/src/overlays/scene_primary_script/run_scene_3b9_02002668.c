#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_0200adac[];
extern u8 Data_03001ebc[];
void Func_02004308();
void Func_02004316();
void Func_02004324();
void Func_0200433a();
void Func_020043b8();
void Func_02005260();
void Func_02005290();
void Func_020052ce();
void Func_020052dc();
void Func_020052ea();
void Func_02005310();
void Func_02005328();
void Func_0200532c();
void Func_0200533a();
void Func_02005340();
void Func_02005344();
void Func_02005348();
void Func_0200534e();
void Func_02005352();
void Func_0200535a();
void Func_02005360();
s32 Func_02005362();
void Func_0200537a();
void Func_0200538e();
void Func_0200539c();
void Func_020053a6();
void Func_020053b6();
void Func_020053ba();
void Func_020053c6();
void Func_020053ca();
void Func_020053d0();
void Func_020053d6();
void Func_020053d8();
void Func_020053e0();
void Func_020053e2();
void Func_020053e8();
void Func_020053ec();
void Func_0200542c();
void Func_02005432();
void Func_0200549e();
void Func_020054b8();
void Func_020054fe();
void Func_0200551c();
void Func_02005528();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

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

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene3b9_02002668(void)
{
    u32 i;
    s32 record;
    s32 base5_200adac;

    Func_02005260();
    Call3(Func_020052ce, 1, 0x3180000, 0x880000);
    Call3(Func_020052dc, 2, 0x3380000, 0x880000);
    Call3(Func_020052ea, 3, 0x3280000, 0x980000);
    Func_020053b6();
    Func_020053ca();
    Func_02005290(40);
    Func_02005328(8, 1);
    Func_02005310(8, 3);
    Call1(Func_0200534e, 0x2134);
    Func_02004308(8);
    Func_02005344(9, 1);
    Func_02004316(9);
    Func_02005352(10, 1);
    Func_02004324(10);
    Func_02005360(11, 1);
    Func_02005348(11, 3);
    Func_0200433a(11);
    Call3(Func_020053ba, 1, 0xe000, 0);
    Call3(Func_020053c6, 2, 0xa000, 20);
    Call3(Func_0200532c, 1, 0x10000, 0x8000);
    Call3(Func_0200533a, 2, 0x10000, 0x8000);
    Call3(Func_02005348, 3, 0x10000, 0x8000);
    base5_200adac = (s32)Data_0200adac;
    Func_0200535a(1, base5_200adac);
    Value2(Func_02005362, 2, base5_200adac);
    Func_0200537a(3, base5_200adac);
    Func_02005340(20);
    Func_020043b8(0, 0);
    Func_020053d0(0, 3);
    Func_020053d8(11, 3);
    Call3(Func_0200538e, 11, 0x10000, 0x8000);
    Call3(Func_0200539c, 0, 0x10000, 0x8000);
    Func_020053ec(11, 2);
    Call3(Func_020053d6, 11, 0x33e, 152);
    Call3(Func_020053e2, 11, 0x328, 164);
    Call3(Func_020053e8, 11, 0x328, 0x138);
    Func_020053a6(20);
    Call2(Func_0200549e, 0x6666, 0xccc);
    Call4(Func_020054b8, 0x3280000, -1, 0x1380000, 1);
    Call3(Func_0200542c, 0, 0x328, 164);
    Call3(Func_02005432, 0, 0x328, 0x138);
    Func_020053e0(60);
    Func_0200551c();
    Func_02005528();
    Func_020054fe(67);
}
