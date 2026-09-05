#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02001dde();
s32 Func_02001df0();
void Func_02001df2();
s32 Func_02001dfc();
void Func_02001e26();
void Func_02001e28();
void Func_02001e30();
void Func_02001e34();
void Func_02001e40();
void Func_02001e44();
void Func_02001e48();
void Func_02001e56();
void Func_02001e7c();
void Func_02001e9e();
void Func_02001eaa();
void Func_02001ecc();
void Func_02001ef4();
void Func_02001f0e();
void Func_02001f22();
void Func_02001f24();
void Func_02001f2a();
void Func_02001f3a();
void Func_02001f4e();
void Func_02001f50();
void Func_02001f74();
void Func_02001f7e();
void Func_02001f84();
void Func_02001f90();
void Func_02001f9c();
void Func_02001fa6();
void Func_02001fb2();

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

void FieldScene_RunOpeningAuxiliarySequence(void)
{
    s32 id;
    s32 v5;
    s32 v6;

    if (Value1(Func_02001df0, 0xf01) == 0) {
    } else {
        if (Value1(Func_02001dfc, 0x81a) != 0) {
        } else {
            Func_02001e26();
            Func_02001f3a();
            v5 = 1;
            Func_02001f50(182);
            Call6(Func_02001df2, 0, 70, 30, 42, v5, v5);
            Func_02001dde();
            Func_02001e44(40);
            id = 0x1032;
            Call2(Func_02001e30, id, 1);
            Func_02001e56(20);
            v6 = 3;
            Func_02001f84(183);
            Call6(Func_02001e28, 0, 29, 3, 1, v6, 2);
            Call6(Func_02001e40, 0, 29, 3, 2, v6, v5);
            Call6(Func_02001e48, 1, 109, 4, 81, v5, v5);
            Func_02001e34();
            Call3(Func_02001e7c, 0x10000, 0x10000, 0x10000);
            Func_02001eaa(20);
            Call3(Func_02001f7e, 0, 0x100, 0);
            Call3(Func_02001e9e, 0x20000, 0x20000, 0x10000);
            Func_02001ecc(20);
            Call3(Func_02001f90, 0, 0x4000, 40);
            Call3(Func_02001f9c, 0, 0x8000, 20);
            Func_02001fa6(0, 0, 20);
            Call3(Func_02001fb2, 0, 0x4000, 10);
            Func_02001f74(0, 4, 20);
            Func_02001f7e(0, 6, 40);
            Call3(Func_02001ef4, -1, -1, 0xe666);
            Func_02001f22(40);
            Call2(Func_02001f0e, id + 1, 1);
            Call1(Func_02001f24, 0x143);
            Call1(Func_02001f2a, 0x81a);
            Func_02001f4e();
        }
    }
}
