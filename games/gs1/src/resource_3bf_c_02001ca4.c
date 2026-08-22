#include "types.h"

extern u8 Value_00002421;

void Func_02007396(s32);
void Func_020073ae(s32, s32);
void Func_02007386(s32, s32);
void Func_020073ac(s32);
void Func_020073c4();
void Func_020073dc(s32, s32);

void Func_02001ca4(s32 arg0)
{
    u8 *t = &Value_00002421;

    Func_02007396((s32)t);
    Func_020073ae(arg0, 0);
    Func_02007386(arg0, 1);
    Func_020073ac((s32)(t + 1));
    Func_020073c4(arg0, 0);
    Func_02007386(arg0, 4);
    Func_020073c4((s32)(t + 2));
    Func_020073dc(arg0, 0);
}
