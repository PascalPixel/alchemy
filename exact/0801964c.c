#include "types.h"

s32 Func_080196c4(s32, s32, s32);

/* 上限を指定しない呼出し。 */
void Func_0801964c(s32 no, s32 value)
{
    Func_080196c4(no, value, 0x7fffffff);
}
