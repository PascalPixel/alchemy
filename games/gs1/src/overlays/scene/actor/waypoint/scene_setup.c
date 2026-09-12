#include "types.h"

#define FieldScene_RunSixCallSetupSequence Func_02000ea8
void Func_02003d14(s32 no, s32 val);
s32 Func_02003d34(s32 no, s32 val);
void Func_02003d30(void);
void Func_02003c66(s32 no);
void Func_02001ce8(s32 no);
void Func_02003cfc(s32 no, s32 val);

void FieldScene_RunSixCallSetupSequence(s32 no, s32 val)
{
    s32 v0 = 0x20000;
    s32 v1 = 0x4000;

    Func_02003d14(v0, v1);
    Func_02003d34(no, 1);
    Func_02003d30();
    Func_02003c66(30);
    Func_02001ce8(no);
    Func_02003cfc(no, val);
}
