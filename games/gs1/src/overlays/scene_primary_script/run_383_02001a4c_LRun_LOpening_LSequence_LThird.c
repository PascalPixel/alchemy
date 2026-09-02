#include "types.h"

#define FieldScene_RunOpeningSequenceThird Func_02001a4c

struct SceneRecord {
    u8 pad[100];
    u16 value100;
};
void Func_020060d2();
void Func_020060d6();
void Func_020060ee();
void Func_020060fe();
void Func_020067a6();
void *Func_020067d0();
void *Func_020067d8();
void Func_020067da();
void Func_020067dc();
void Func_020067f0();
void Func_020067f8();
void Func_020067fa();
void Func_02006804();
void Func_02006810();
void Func_0200682e();
void Func_02006834();
void Func_02006840();
void Func_0200684c();
void Func_02006872();
void Func_0200687a();
void Func_0200689c();
void Func_020068aa();
void Func_020068ba();
void Func_020068c2();
void Func_020068c4();
void Func_020068ca();
void Func_020068d0();
void Func_020068e6();
void Func_020068ee();
void Func_020068f2();
void Func_020068fe();
void Func_02006930();
static __inline__ s32 Value0(s32 (*f)())
{
    return f();
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

static __inline__ void Call0(void (*f)())
{
    f();
}

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

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}


void Func_02001a4c(void)
{
    void *p1;
    void *p2;

    p1 = Func_020067d0(24);
    p2 = Func_020067d8(25);
    Call0(Func_020067a6);
    Call3(Func_020067f0, 0, 52428, 26214);
    Call3(Func_020067fa, 1, 52428, 26214);
    Call3(Func_02006804, 2, 52428, 26214);
    Call3(Func_02006840, 0, 232, 696);
    Call3(Func_0200684c, 0, 200, 696);
    Call1(Func_020067da, 10);
    Call3(Func_020068e6, 25, 256, 0);
    Call3(Func_020068f2, 24, 256, 0);
    Call1(Func_020067f8, 60);
    Call3(Func_020060d2, 25, 0, 10);
    Call2(Func_020068ca, 24, 2);
    Call1(Func_02006810, 20);
    Value1(Func_020068ee, 4758);
    Call2(Func_020060d6, 24, 20);
    Call2(Func_02006930, 25, 258);
    Call1(Func_0200682e, 60);
    Call2(Func_020060ee, 25, 20);
    Call2(Func_020068fe, 24, 1);
    Call2(Func_020060fe, 24, 30);
    Call3(Func_0200689c, 24, 262144, 131072);
    Call3(Func_020068aa, 25, 229376, 114688);
    Value2(Func_020068ba, 25, 33609776);
    Value2(Func_020068c2, 24, 33609056);
    Call1(Func_020068d0, 24);
    Call6(Func_02006834, 14, 45, 3, 1, 14, 44);
    Value1(Func_02006872, 2130);
    Call1(Func_0200687a, 768);
    Call2(Func_020067dc, 33598369, 3200);
    ((struct SceneRecord *)p1)->value100 = 1;
    ((struct SceneRecord *)p2)->value100 = 3;
    Value0(Func_020068c4);
}
