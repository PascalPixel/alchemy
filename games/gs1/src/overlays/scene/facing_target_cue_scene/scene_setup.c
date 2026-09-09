#include "types.h"

#define FieldScene_RunActorCueBranch Func_02000118
#define FieldScene_RunActor8TwoStep Func_020017c4
#define FieldScene_RunSixStepSequence17e4 Func_020017e4
extern u8 Value_000022b9;

void Func_02001ae6(s32);
s32 Func_02001af6(s32, s32);
s32 Func_02001a56(s32, s32);
void Func_02001a40(s32);
void Func_02001b06(s32);
void Func_02001b0e(s32);
void Func_02001b26(s32, s32);
void Func_020030f6(s32);
void Func_02002d4e(void);
void Func_02003268(s32, s32);
s32 Func_02003278(s32, s32);
void Func_02003294(void);
void Func_02003272(s32);
void Func_0200328e(void);
void Func_0200329a(void);

void FieldScene_RunActorCueBranch(s32 object)
{
    s32 cue = (s32) &Value_000022b9;

    Func_02001ae6(cue);
    Func_02001af6(object, 0);
    if (Func_02001a56(0, 0) == 0) {
        Func_02001a40(10);
        Func_02001b06(cue + 1);
    } else {
        Func_02001b0e(cue + 2);
    }
    Func_02001b26(object, 0);
}

void FieldScene_RunActor8TwoStep(void)
{
    Func_020030f6(8);
    Func_02002d4e();
}

void FieldScene_RunSixStepSequence17e4(void)
{
    Func_02003268(0x94, 1);
    Func_02003278(8, 0x11);
    Func_02003294();
    Func_02003272(1);
    Func_0200328e();
    Func_0200329a();
}
