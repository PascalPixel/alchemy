#include "types.h"

s32 Func_0800231c(s32);
s32 Func_08002322(s32);
typedef s32 (*WorkFunc_08005208)(s32, s32);

struct Work_08005208 {
    s32 result;
    s32 param1;
    s32 param2;
};

void Func_08005208(u32 value, s32 param1, s32 param2)
{
    s32 half;
    s32 first;
    s32 result;

    half = (s32)(value + (value >> 31)) >> 1;
    first = Func_08002322(half);
    result = ((WorkFunc_08005208)0x0300013C)(
        first,
        Func_0800231c(half) * 0x50
    );
    ((struct Work_08005208 *)0x03001CE0)->param1 = param1;
    ((struct Work_08005208 *)0x03001CE0)->result = result;
    ((struct Work_08005208 *)0x03001CE0)->param2 = param2;
}
