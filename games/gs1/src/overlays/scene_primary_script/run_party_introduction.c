#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
s32 Func_02004f5c();
void Func_020053f2();
void Func_02005408();
void Func_0200542c();
void Func_0200543a();
void Func_02005450();
void Func_02005454();
void Func_02005460();
void Func_02005466();
void Func_02005490();
void Func_02005496();
void Func_020054aa();
void Func_020054bc();
void Func_020054c8();
void Func_020054ce();
void Func_020054d6();
void Func_020054fa();
void Func_02005506();
void Func_0200550e();
void Func_02005520();
void Func_02005532();
void Func_02005544();
void Func_0200554a();
void Func_0200554c();
void Func_02005564();
void Func_0200557c();
void Func_02005584();
void Func_02005592();
void Func_020055a8();
void Func_020055ae();
void Func_020055b2();
void Func_020055ba();
void Func_020055c0();
void Func_020055c6();
void Func_020055d0();
void Func_020055d2();
void Func_020055d4();
void Func_020055e0();
void Func_020055fa();
void Func_02005630();
void Func_02005648();
void Func_0200564e();
void Func_02005654();
void Func_02005666();
void Func_0200567e();
void Func_02005698();
void Func_020056a6();
void Func_020056be();
void Func_020056ca();
void Func_020056d0();
void Func_02005738();
void Func_02005746();
void Func_02005764();
void Func_02005770();
void Func_0200578e();
void Func_020057a2();
void Func_020057ce();

/* Inline calls keep argument constants local; direct calls permit sharing. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ s32 Value4(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    return f(a0, a1, a2, a3);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

void Func_02000a98(void)
{
    u32 i;
    s32 obj;

    Func_020055ba(141);
    for (i = 0; i != 6; i++) {
        Call2(Func_0200557c, 0x4049d2, 1);
        Func_02005592(8);
        Func_02005450(8);
        Call2(Func_02005592, 0x10000, 1);
        Func_020055a8(8);
        Func_02005466(8);
        if (i == 1) {
            Call3(Func_020053f2, 0x10000, 0x10000, 0x10000);
        }
    }
    Call3(Func_02005408, 0x20000, 0x10000, 0x10000);
    Func_02005496(30);
    Call2(Func_020055ae, 0x26666, 0x4ccc);
    Call4(Func_020055c6, 0xa70000, -1, 0x2110000, 1);
    Func_020055d2();
    Call3(Func_0200543a, 0x10000, 0x20000, 0x10000);
    Func_020054c8(20);
    Func_0200564e(144);
    Func_0200542c(0x200d088, 65, 31);
    Call6(Func_02005454, 0, 0, 1, 1, 10, 31);
    Call6(Func_02005460, 87, 42, 10, 33, 1, 2);
    Func_0200550e(40);
    Func_02005490(0, 0, 0);
    Call2(Func_02005630, 0x66666, 0xcccc);
    Call4(Func_02005648, 0x1870000, -1, 0xb10000, 1);
    Func_02005654();
    Call3(Func_020054bc, 0x10000, 0x20000, 0x10000);
    Func_0200554a(20);
    Func_020056d0(144);
    Func_020054aa(0x200d088, 79, 9);
    Call6(Func_020054ce, 0, 0, 1, 1, 24, 9);
    Call6(Func_020054d6, 87, 42, 24, 11, 1, 2);
    Func_02005584(40);
    Func_02005506(0, 0, 0);
    Call2(Func_020056a6, 0x26666, 0x4ccc);
    Call4(Func_020056be, 0x2470000, -1, 0xc10000, 1);
    Func_020056ca();
    Call3(Func_02005532, 0x10000, 0x20000, 0x10000);
    Func_020055c0(20);
    Func_02005746(144);
    Func_02005520(0x200d088, 91, 10);
    Call6(Func_02005544, 0, 0, 1, 1, 36, 10);
    Call6(Func_0200554c, 87, 42, 36, 12, 1, 2);
    Func_020055fa(40);
    *(s32 *)((*(u8 *volatile *)Data_03001ebc + 0x1c0)) = 0x202;
    Func_02005764();
    Func_02005770();
    Call4(Func_02005738, 0xe80000, -1, 0x1dd0000, 0);
    Func_02005564();
    Func_020054fa(1);
    Call3(Func_020055b2, 0x20000, 0x10000, 0x10000);
    Func_0200578e();
    Func_020057a2();
    Func_02005648(40);
    Call1(Func_020057ce, 0x121);
    Call3(Func_020055d4, -1, -1, 0xe666);
    Func_020055e0();
    Func_02005666(20);
    Call6(Func_020055d0, 0, 40, 13, 66, 3, 3);
    Func_0200567e(20);
    obj = Value4(Func_02004f5c, 223, 0xe80000, 0x100000, 0x1d00000);
    Func_02005698(40);
    Func_02005630(obj, 1);
    Call2(Func_02005630, 0x1077, 1);
}
