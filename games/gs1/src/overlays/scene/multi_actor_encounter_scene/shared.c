#include "types.h"

#define FieldScene_RunStepThen10 Func_020039ec
#define FieldScene_CallPairWith10 Func_02003a00
void Func_02007dae(s32, s32);
void Func_02007cdc(s32);
void Func_02007dda(s32, s32, s32);

void FieldScene_RunStepThen10(s32 a)
{
    Func_02007dae(a, 0);
    Func_02007cdc(10);
}

void FieldScene_CallPairWith10(s32 a, s32 b)
{
    Func_02007dda(a, b, 10);
}
