#include "types.h"

/* Complete paired scene-cell presentation update. */



extern void Func_02001ede(s32 kind, s32 x, s32 enabled, s32 mode,
                          s32 first_value, s32 second_value);
extern void Func_02001ee8(s32 kind, s32 first_x, s32 second_x, s32 value,
                          s32 first_mode, s32 second_mode);
void Func_02000e30(void)
{
    s32 first_value = 13;
    s32 second_value = 25;

    Func_02001ede(41, 43, 1, 1, first_value, second_value);
    Func_02001ee8(40, 42, 12, 22, 3, 3);
}
