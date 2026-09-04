#include "types.h"

#define FieldScene_RunScene376_020001e8 Func_020001e8

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_000011a4[];
s32 Func_02001376();
void Func_02001388();
void Func_020013aa();
void Func_020013c8();
void Func_020013e4();
void Func_020013ea();
void Func_02001440();
void Func_0200144c();
void Func_02001458();
void Func_0200145e();
void Func_02001482();

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

void FieldScene_RunScene376_020001e8(void)
{
    s32 callback;
    s32 base5_11a4;

    Func_02001388();
    if (Value1(Func_02001376, 0x81b) != 0) {
        Call1(Func_02001440, 0x11a6);
        Func_02001458(20, 0);
        callback = 0x20092fc;
        Call3(Func_0200144c, 20, 0x10000, callback);
    } else {
        base5_11a4 = (s32)Data_000011a4;
        Func_0200145e(base5_11a4);
        Func_02001482(20, 0, 20);
        Func_020013aa((base5_11a4 + 1), 1);
        Func_020013ea(180, 0);
        Call1(Func_020013c8, 0x81b);
    }
    Func_020013e4();
}
