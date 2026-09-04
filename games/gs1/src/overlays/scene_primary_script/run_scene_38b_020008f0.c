#include "types.h"

#define FieldScene_RunScene38b_020008f0 Func_020008f0

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_0200135e();
s32 Func_0200198e();
void Func_020019b2();
s32 Func_020019f4();
s32 Func_02001a04();
s32 Func_02001a0a();
void Func_02001a0e();
s32 Func_02001a30();
void Func_02001a34();
s32 Func_02001a3a();
void Func_02001a4e();
void Func_02001a5a();
void Func_02001a60();
void Func_02001a6c();
void Func_02001a8e();

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

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* Shared cross-overlay scene-record block; +450 is the scene sub-state. */
extern u8 Data_02000240[];

void FieldScene_RunScene38b_020008f0(void)
{
    s32 record;
    s16 sub_state;

    if (Value1(Func_0200198e, 0x845) != 0) {
        ((void (*)())Func_02001a04)(9, 0, 0);
        Call3(Func_02001a60, 14, 0x3000, 0);
        Call3(Func_02001a6c, 15, 0x5000, 0);
    } else {
        record = Func_020019f4(9);
        Func_020019b2(record, 0);
        Func_02001a34(21, 0, 0);
    }
    record = Func_02001a0a(8);
    *(volatile s32 *)(record + 28) = 0x18000;
    {
        s32 off = 450;
        sub_state = *(s16 *)(Data_02000240 + off);
    }
    if (sub_state == 10) {
        Func_02001a5a(8, 0, 0);
    } else {
        if (sub_state == 9) {
            Call1(Func_02001a0e, 0x12f);
        }
    }
    if (Value1(Func_02001a04, 0x109) == 0) {
        {
            s32 off = 450;
            sub_state = *(s16 *)(Data_02000240 + off);
        }
        if (sub_state == 11) {
            Call3(Func_02001a8e, 20, 0xf80000, 0xd80000);
        }
    }
    Func_0200135e();
    if (Value1(Func_02001a30, 0x84a) != 0) {
        if (Value1(Func_02001a3a, 0x84b) == 0) {
            Call1(Func_02001a4e, 0x304);
        }
    }
}
