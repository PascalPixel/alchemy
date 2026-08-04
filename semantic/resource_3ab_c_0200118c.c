#include "types.h"

/* Three adjacent collision cells used by the scene's lower doorway. */
extern void Func_080091c0(s32, s32, s32, s32, s32, s32);

void Func_0200118c(void)
{
    Func_080091c0(7, 12, 1, 1, 7, 11);
    Func_080091c0(7, 12, 1, 1, 8, 11);
    Func_080091c0(7, 12, 1, 1, 9, 11);
}
