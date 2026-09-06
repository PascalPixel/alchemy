#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
s32 Func_02001e82();
void Func_02001e8a();
void Func_02001e96();
s32 Func_02001ea4();
void Func_02001eb0();
void Func_02001ede();
s32 Func_02001eee();
void Func_02001ef0();
s32 Func_02001efe();
void Func_02001f00();
void Func_02001f2c();
u8 *Func_02001f34();
void Func_02001f6e();
u8 *Func_02001f8e();
s32 Func_02001f9a();
void Func_02001f9c();
void Func_02001fc8();
void Func_02001fce();
void Func_02002000();

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

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
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

void FieldScene_RunSupplementalSequenceOne(s32 a0)
{
    u32 i;
    u8 *record;
    s32 v3;
    s32 v0;

    record = Value1(Func_02001ea4, 0);
    v3 = *(volatile s32 *)((s32)record + 8) / 0x100000;
    v0 = *(volatile s32 *)((s32)record + 16) / 0x100000;
    if (Value1(Func_02001e82, 0xf27) == 0) {
        if (v3 == 7) {
            if (v0 == 16) {
                Func_02002000(0, 0, 16);
            }
        }
        Call3(Func_02001fce, 102, -1, -1);
        Call6(Func_02001e8a, 28, 31, 1, 1, 7, 16);
    }
    Call6(Func_02001e96, 47, 4, 1, 1, 46, 4);
    Call6(Func_02001eb0, 34, 37, 3, 3, 13, 3);
    Call3(Func_02001f6e, 8, 0xe80000, 0x480000);
    record = Func_02001f34(8);
    *(volatile s32 *)((s32)record + 12) = 0;
    if (Value1(Func_02001efe, 0x202) != 0) {
        Call6(Func_02001ede, 41, 49, 3, 4, 1, 14);
        Call6(Func_02001ef0, 44, 49, 3, 4, 33, 14);
        Call6(Func_02001f00, 47, 49, 3, 4, 1, 46);
    } else {
        Call3(Func_02001fc8, 19, 0x380000, 0x10c0000);
        record = Func_02001f8e(19);
        Func_02001f2c((s32)record, 0);
        record = Value1(Func_02001f9a, 19);
        if ((s32)record != 0) {
            record[85] = 8;
            *(volatile s32 *)((s32)record + 12) = 0x100000;
            *(u8 *)((((s32)record + 85) - 50)) = 2;
            *(volatile s32 *)((s32)record + 24) = 0x13333;
            *(volatile s32 *)((s32)record + 28) = 0x18000;
        }
    }
    Value2(Func_02001eee, 0x2008469, 0xc80);
    Call1(Func_02001f9c, 0x201);
}
