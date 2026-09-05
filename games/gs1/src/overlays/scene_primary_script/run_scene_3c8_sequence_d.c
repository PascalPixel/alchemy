#include "types.h"

#define FieldScene_RunScene3c8SequenceD Func_02001218

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_020060ae();
void Func_020060c6();
void Func_020060ec();
void Func_020060f2();
void Func_020060f8();
s32 Func_020060fa();
s32 Func_02006104();
void Func_02006106();
void Func_0200610c();
void Func_02006116();
void Func_02006120();
s32 Func_02006126();
void Func_02006128();
void Func_02006136();
void Func_02006138();
void Func_02006146();
void Func_02006156();
void Func_02006166();
void Func_02006176();
void Func_02006186();
void Func_02006196();
void Func_020061a6();
void Func_020061c8();
void Func_020061dc();
void Func_020061ec();
void Func_020061fc();
void Func_02006200();
void Func_0200620c();
void Func_0200620e();
void Func_02006216();
void Func_0200621c();
void Func_02006220();
void Func_0200622c();
void Func_02006236();
void Func_0200623c();
void Func_0200624c();
void Func_0200625c();
void Func_02006262();
void Func_0200626c();
void Func_0200627c();
void Func_02006302();
void Func_02006328();
void Func_02006350();
void Func_020063fe();
void Func_02006416();
void Func_02006422();
void Func_0200642c();
void Func_02006430();
void Func_0200643c();

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

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene3c8SequenceD(void)
{
    u32 i;
    s32 record;
    u8 *p7;

    p7 = *(volatile s32 *)Data_03001ebc;
    {
        volatile u16 *target = (volatile u16 *)((s32)p7 + 0xcba);
        s32 shown = 0;

        *target = shown;
    }
    {
        s32 shown = 1;
    
        *(volatile u16 *)(((s32)p7 + 0xcb6)) = shown;
    }
    Func_020060c6();
    Func_02006136(0, 1);
    Call2(Func_020060ae, 0x2688, 1);
    Call2(Func_02006200, 0x10000, 0);
    Call2(Func_02006200, 0x10005, 0);
    Func_02006216(120);
    Func_020060ec(100);
    Func_02006262(142);
    Func_020060f8(30);
    Call2(Func_02006220, 0x7fff, 0);
    Func_02006236(60);
    Func_0200610c(70);
    if (Value1(Func_020060fa, 0x982) == 0) {
        if (Value1(Func_02006104, 0x983) == 0) {
            if ((*(volatile s32 *)0x03001e40 & 1) != 0) {
                Call1(Func_02006120, 0x982);
            } else {
                Call1(Func_02006128, 0x983);
            }
        }
    }
    if (Value1(Func_02006126, 0x982) == 0) {
        Call1(Func_02006138, 0x982);
        Call1(Func_02006146, 0x983);
        Call6(Func_020060f2, 103, 27, 89, 27, 7, 8);
        Call6(Func_02006106, 41, 90, 27, 92, 3, 2);
        Call6(Func_02006116, 41, 90, 29, 93, 3, 2);
        Call6(Func_02006126, 41, 90, 27, 94, 3, 2);
        Call6(Func_02006136, 41, 90, 27, 96, 3, 2);
        Call6(Func_02006146, 41, 90, 29, 97, 3, 2);
        Call6(Func_02006156, 41, 96, 25, 91, 3, 2);
        Call6(Func_02006166, 41, 92, 25, 93, 3, 2);
        Call6(Func_02006176, 41, 96, 25, 95, 3, 2);
        Call6(Func_02006186, 41, 96, 25, 97, 3, 2);
        Call6(Func_02006196, 41, 96, 27, 96, 3, 2);
        Call6(Func_020061a6, 41, 96, 29, 97, 3, 2);
    } else {
        Call1(Func_0200620e, 0x983);
        Call1(Func_0200621c, 0x982);
        Call6(Func_020061c8, 111, 27, 89, 27, 7, 8);
        Call6(Func_020061dc, 41, 90, 25, 91, 3, 2);
        Call6(Func_020061ec, 41, 90, 25, 93, 3, 2);
        Call6(Func_020061fc, 41, 90, 25, 95, 3, 2);
        Call6(Func_0200620c, 41, 90, 25, 97, 3, 2);
        Call6(Func_0200621c, 41, 90, 27, 96, 3, 2);
        Call6(Func_0200622c, 41, 90, 29, 97, 3, 2);
        Call6(Func_0200623c, 41, 94, 27, 92, 3, 2);
        Call6(Func_0200624c, 41, 96, 29, 93, 3, 2);
        Call6(Func_0200625c, 41, 94, 27, 94, 3, 2);
        Call6(Func_0200626c, 41, 96, 27, 96, 3, 2);
        Call6(Func_0200627c, 41, 96, 29, 97, 3, 2);
    }
    Call2(Func_02006416, 0x10000, 0);
    Func_0200642c(20);
    Func_02006302(40);
    Call2(Func_020063fe, 0x8000, 0x1000);
    Call4(Func_02006416, 0x1c80000, -1, 0x21e0000, 1);
    Func_02006422();
    Func_02006328(50);
    Call4(Func_02006430, 0x1c80000, -1, 0x1a70000, 1);
    Func_0200643c();
    Func_02006350();
    {
        volatile u16 *target = (volatile u16 *)((s32)p7 + 0xcb6);
        s32 shown = 0;

        *target = shown;
    }
}
