#include "types.h"

#define SceneState_ResetObject14Word108 Func_02002660

/* Resource 378 object reset at 0x02002660 (28 bytes including alignment). */

extern u8 *Func_02005c76();
extern void Func_02005ccc();

void SceneState_ResetObject14Word108(void)
{
    u8 *state = Func_02005c76(14);
    *(s32 *)(state + 108) = 0;
    Func_02005ccc(14, 0, 0);
}
