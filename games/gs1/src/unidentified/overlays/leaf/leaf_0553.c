#include "types.h"

/* Three adjacent collision cells used by the scene's lower doorway. */
extern void Func_02002b32(s32, s32, s32, s32, s32, s32);
extern void Func_02002b44(s32, s32, s32, s32, s32, s32);
extern void Func_02002b56(s32, s32, s32, s32, s32, s32);

void Func_0200118c(void)
{
    Func_02002b32(7, 12, 1, 1, 7, 11);
    Func_02002b44(7, 12, 1, 1, 8, 11);
    Func_02002b56(7, 12, 1, 1, 9, 11);
}
