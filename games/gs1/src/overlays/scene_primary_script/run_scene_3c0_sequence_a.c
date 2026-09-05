#include "types.h"

#define FieldScene_RunScene3c0SequenceA Func_02000dc8

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_000000a5[];
extern u8 Data_02000240[];
extern u8 Data_03001ebc[];
extern s16 Data_02000240_t[][1];
s32 Func_02002042();
void Func_0200204a();
s32 Func_0200206e();
void Func_02002070();
s32 Func_0200207a();
void Func_020020ce();
void Func_02002180();
void Func_0200218c();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

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

s32 Func_02000dc8(s32 a0)
{
    u32 i;
    s32 record;
    s32 v5;

    if (Data_02000240_t[224][0] == (s32)Data_000000a5) {
        *(u8 *)(Func_0200206e(14) + 35) = 2;
        v5 = 0;
        *(u8 *)(Func_0200207a(14) + 85) = v5;
        Call3(Func_020020ce, 14, 0xf80000, 0x2c80000);
        Call6(Func_0200204a, 31, 95, 1, 1, 15, 44);
        Call3(Func_02002180, 100, -1, -1);
        Func_0200218c();
        Call6(Func_02002070, 127, 127, 1, 1, 12, 71);
        record = Value2(Func_02002042, 0x2008ce5, 0xc80);
        return record;
    }
    return a0;
}
