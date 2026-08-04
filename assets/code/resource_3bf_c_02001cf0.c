#include "types.h"

extern u8 Value_00002424;

void Func_020073e2(s32);
void Func_020073fa(s32, s32);
void Func_02007330(s32);
void Func_0200742a(s32, s32, s32);
void Func_02007400(s32);
void Func_02007418(s32, s32);
void Func_020073f0(s32, s32);
void Func_02007416(s32);
void Func_0200742e();
void Func_02007446(s32, s32);

void Func_02001cf0(s32 arg0)
{
    u8 *t = &Value_00002424;

    Func_020073e2((s32)t);
    Func_020073fa(arg0, 0);
    Func_02007330(120);
    Func_0200742a(arg0, 257, 60);
    Func_02007400((s32)(t + 1));
    Func_02007418(arg0, 0);
    Func_020073f0(arg0, 1);
    Func_02007416((s32)(t + 2));
    Func_0200742e(arg0, 0);
    Func_020073f0(arg0, 4);
    Func_0200742e((s32)(t + 3));
    Func_02007446(arg0, 0);
}
