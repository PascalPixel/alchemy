#include "types.h"

#define SceneState_ApplyArgMode0AndSet10 Func_02000894
#define SceneState_ForwardByRuntimeWordBits Func_0200211c
#define SceneState_ApplyPair140And0 Func_02002334
void Func_0200669e(s32, s32);
void Func_020065cc(s32);
s32 Func_02007d28(u32, s32);
void Func_02007e10(s32, s32);
s32 Func_02007d38(u32, s32);
void Func_0200434e(s32);
void Func_020081e8(s32, s32);

void SceneState_ApplyArgMode0AndSet10(s32 a)
{
    Func_0200669e(a, 0);
    Func_020065cc(10);
}

void SceneState_ForwardByRuntimeWordBits(s32 a)
{
    volatile u32 *p = (u32 *)0x03001e40;

    if (*p & 1) {
        Func_02007e10(a, Func_02007d28(*p >> 1, 6));
    }
    if (Func_02007d38(*p, 15) == 0) {
        Func_0200434e(a);
    }
}

void SceneState_ApplyPair140And0(void)
{
    Func_020081e8(140, 0);
}
