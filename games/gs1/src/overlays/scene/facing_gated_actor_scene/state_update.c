#include "types.h"

#define SceneState_ResetObject14Word108 Func_02002660
#define SceneState_ApplyTwoRects Func_02003270
/* Resource 378 object reset at 0x02002660(28 bytes including alignment). */
extern u8 *Func_02005c76();
void Func_02005ccc();
void Func_020067fe(s32, s32, s32, s32, s32, s32);
void Func_0200681a(s32, s32, s32, s32, s32, s32);
void Func_020067c8(s32 frames);

void SceneState_ResetObject14Word108(void)
{
    u8 *state = Func_02005c76(14);
    *(s32 *)(state + 108) = 0;
    Func_02005ccc(14, 0, 0);
}

/* Publish the scene's upper prompt and lower dialogue panel. */
void SceneState_ApplyTwoRects(void)
{
    {
        s32 a5 = 3, a6 = 2;
        Func_020067fe(0, 64, 11, 68, a5, a6);
    }
    {
        s32 a5 = 11, a6 = 8;
        Func_0200681a(11, 10, 3, 2, a5, a6);
    }
    Func_020067c8(1);
}
