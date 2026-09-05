#include "types.h"

#define FieldScene_RunScene3b2SequenceA Func_02001214

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_0000007e[];
extern u8 Data_02000240[];
extern u8 Data_03001ebc[];
extern s16 Data_02000240_t[][1];
s32 Func_0200428e();
void Func_02004290();
u8 *Func_020042a4();
void Func_020042b0();
s32 Func_020042da();
void Func_02004300();
s32 Func_02004318();
void Func_02004330();

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

void FieldScene_RunScene3b2SequenceA(void)
{
    u32 i;
    s32 rec7;
    u8 *rec8;
    s32 record;

    rec8 = Func_020042a4(8);
    rec7 = Value1(Func_0200428e, (Data_02000240_t[224][0] + (0x8d2 - (s32)Data_0000007e)));
    if (rec7 != 0) {
        Call3(Func_02004300, 8, 0x28a0000, 0xa80000);
        *(volatile s32 *)((s32)rec8 + 12) = -0x200000;
        record = Func_020042da(8);
        Func_02004290(record, 0);
        Func_02004330(8, 3);
        rec8[85] = 0;
        {
            u8 value = *(volatile u8 *)&rec8[35];
        
            rec8[35] = (u8)(value | 2);
        }
        Call6(Func_020042b0, 42, 10, 1, 1, 40, 10);
    } else {
        *(u8 *)(Func_02004318(8) + 85) = rec7;
    }
}
