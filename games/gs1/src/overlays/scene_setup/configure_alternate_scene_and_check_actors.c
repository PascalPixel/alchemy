#include "types.h"

s32 Func_020031fe();
void Func_02003912();
void Func_02003a30();

static __inline__ void ConfigureScene(s32 actor, s32 x, s32 y, s32 mode)
{
    Func_02003a30(actor, x, y, mode);
}

void Func_020015fc(void)
{
    ConfigureScene(2, 23068672, 7340032, 0);
    if (Func_020031fe(12, 21, 7) != 0) {
        Func_02003912();
    }
}
