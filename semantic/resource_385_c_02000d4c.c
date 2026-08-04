#include "types.h"

/* Complete flag-0x200 scene-cell disable wrapper through alignment. */
extern void Func_080770d0(s32 flag);
extern void Func_080091c0(s32 kind, s32 x, s32 enabled, s32 mode,
                          s32 first_value, s32 second_value);

void Func_02000d4c(void)
{
    Func_080770d0(0x200);
    Func_080091c0(23, 23, 4, 2, 23, 26);
}
