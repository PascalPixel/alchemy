#include "types.h"

extern u8 Value_00002428;

void Func_02007452(s32);
void Func_0200746a(s32, s32);
void Func_0200742a(s32, s32);
void Func_02007468(s32);
void Func_02007480(s32, s32);
void Func_02007458(s32, s32);
void Func_0200747e(s32);
void Func_02007496();
void Func_020074ae(s32, s32);

void Func_02001d60(s32 arg0)
{
    u8 *t = &Value_00002428;

    Func_02007452((s32)t);
    Func_0200746a(arg0, 0);
    Func_0200742a(arg0, 4);
    Func_02007468((s32)(t + 1));
    Func_02007480(arg0, 0);
    Func_02007458(arg0, 1);
    Func_0200747e((s32)(t + 2));
    Func_02007496(arg0, 0);
    Func_02007458(arg0, 3);
    Func_02007496((s32)(t + 3));
    Func_020074ae(arg0, 0);
}
