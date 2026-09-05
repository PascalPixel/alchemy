#include "types.h"

#define FieldScene_RunScene3a3SequenceD Func_02000b2c

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
s32 Func_020019bc();
s32 Func_020019e8();
u8 *Func_02001a12();
s32 Func_02001a16();
void Func_02001a3c();
u8 *Func_02001a3e();
void Func_02001a52();
void Func_02001a68();
u8 *Func_02001a6c();
s32 Func_02001a7a();
void Func_02001a80();
s32 Func_02001a8c();
void Func_02001a96();
s32 Func_02001aa2();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
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

void FieldScene_RunScene3a3SequenceD(void)
{
    u32 i;
    u8 *record;

    if (Value1(Func_020019bc, 0x240) == 0) {
        Call3(Func_02001a3c, 8, 0x3280000, 0x2d70000);
        record = Func_02001a12(8);
        {
            s32 shown = 0x3000;
        
            *(volatile u16 *)((s32)record + 6) = shown;
        }
        Call3(Func_02001a52, 9, 0x31a0000, 0x3390000);
    }
    if (Value1(Func_020019e8, 0x241) == 0) {
        Call3(Func_02001a68, 10, 0x2300000, 0x2c60000);
        record = Func_02001a3e(10);
        {
            s32 shown = 0x1000;
        
            *(volatile u16 *)((s32)record + 6) = shown;
        }
        Call3(Func_02001a80, 11, 0x2400000, 0x2c60000);
    }
    if (Value1(Func_02001a16, 0x242) == 0) {
        Call3(Func_02001a96, 15, 0x1270000, 0x2e80000);
        record = Func_02001a6c(15);
        {
            s32 shown = 0xb000;
        
            *(volatile u16 *)((s32)record + 6) = shown;
        }
    } else {
        record = Value1(Func_02001a7a, 15);
        {
            u8 value = *(volatile u8 *)&record[89];
        
            record[89] = (u8)(value | 4);
        }
    }
    record = Value1(Func_02001a8c, 17);
    if ((s32)record != 0) {
        {
            u8 value = *(volatile u8 *)&record[89];
        
            record[89] = (u8)(value | 4);
        }
    }
    record = Value1(Func_02001aa2, 16);
    if ((s32)record != 0) {
        {
            u8 value = *(volatile u8 *)&record[89];
        
            record[89] = (u8)(value | 4);
        }
    }
}
