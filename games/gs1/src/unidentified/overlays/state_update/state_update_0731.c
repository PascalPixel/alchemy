#include "types.h"

extern void Func_02001e26(s32 flag);
extern void Func_02001e02(s32 kind, s32 x, s32 enabled, s32 mode,
                          s32 first_value, s32 second_value);
void Func_02000d4c(void)
{
    Func_02001e26(0x200);
    {
        s32 first_value = 23;
        s32 second_value = 26;
        Func_02001e02(23, 23, 4, 2, first_value, second_value);
    }
}
