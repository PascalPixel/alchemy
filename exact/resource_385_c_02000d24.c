#include "types.h"

extern void Func_02001df6(s32 flag);
extern void Func_02001dda(s32 kind, s32 x, s32 enabled, s32 mode,
                          s32 first_value, s32 second_value);
void Func_02000d24(void)
{
    Func_02001df6(0x200);
    {
        s32 first_value = 23;
        s32 second_value = 26;
        Func_02001dda(55, 26, 4, 2, first_value, second_value);
    }
}
