#include "types.h"

#define FieldScene_RunScene373_02001490 Func_02001490

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02007460();
s32 Func_02007482();
void Func_0200748e();
void Func_020074ae();
void Func_020074c8();
void Func_020074ee();
void Func_020074f0();
s32 Func_020074f2();
void Func_020074fa();
void Func_02007508();
s32 Func_0200750a();
void Func_0200751c();
void Func_02007526();
void Func_02007568();
void Func_0200757c();
void Func_02007590();
void Func_0200759c();
void Func_020075a8();
void Func_020075c4();

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

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene373_02001490(s32 a0, s32 a1)
{
    u32 i;
    s32 p8;
    s32 rec8;
    s32 record;

    p8 = a1;
    rec8 = Value1(Func_02007482, 22);
    Func_02007460();
    Func_02007508(22, 2);
    Call3(Func_0200757c, 22, 0x100, 20);
    Func_0200751c(0, 2);
    Call3(Func_02007590, 0, 0x102, 40);
    Func_020074c8(0, a0);
    Func_0200748e(10);
    Call3(Func_020075a8, 22, 0x103, 0);
    Func_020074f0(22, p8);
    Func_020074ee(0);
    Func_020074ae(20);
    Func_02007568(22, 2);
    *(s32 *)(rec8 + 24) = 0x10000;
    *(s32 *)(rec8 + 28) = 0x10000;
    record = Value1(Func_020074f2, 0);
    *(s32 *)(record + 24) = 0x10000;
    *(s32 *)(record + 28) = 0x10000;
    Call1(Func_0200759c, 0xfce);
    Func_020075c4(22, 0);
    record = Func_0200750a(22);
    *(s32 *)(record + 108) = 0x200d72d;
    Call2(Func_02007526, 22, 0x200e248);
    Func_020074fa();
}
