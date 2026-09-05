#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_00002399[];
extern u8 Data_03001ebc[];
s32 Func_02001184();
void Func_02001196();
void Func_020011ba();
s32 Func_020011c8();
void Func_020011da();
void Func_020011f6();
void Func_02001200();
void Func_02001204();
s32 Func_02001210();
void Func_02001220();
void Func_02001226();
void Func_02001228();
void Func_02001240();
void Func_02001248();
void Func_02001256();
void Func_02001260();
void Func_0200126a();
void Func_02001282();

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

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene3b6_020007b0(s32 a0)
{
    u32 i;
    s32 record;
    s32 base5_2399;

    Func_02001196();
    if (Value1(Func_02001184, 0x8bd) == 0) {
        base5_2399 = (s32)Data_00002399;
        Func_02001200(base5_2399);
        Value2(Func_02001210, a0, 0);
        if (Value2(Func_020011c8, 0, 0) == 0) {
            Func_020011ba(10);
            Func_02001220((base5_2399 + 1));
        } else {
            Func_02001228((base5_2399 + 2));
        }
        Func_02001240(a0, 0);
    } else {
        if (Value1(Func_020011c8, 0x8be) == 0) {
            Call1(Func_020011da, 0x8be);
            Call1(Func_02001248, 0x239c);
            Func_02001260(a0, 0);
            Func_020011f6(10);
            Func_02001256(a0, 2);
            Func_02001204(20);
        }
        Call1(Func_0200126a, 0x239d);
        Func_02001282(a0, 0);
    }
    Func_02001226();
}
