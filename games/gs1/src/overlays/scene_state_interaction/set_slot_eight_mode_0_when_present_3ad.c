#include "types.h"

#define SceneActor_SetSlotEightMode0WhenPresent Func_0200007c

extern s32 Func_02001bbe(s32);
extern void Func_02001b80(s32, s32);

void SceneActor_SetSlotEightMode0WhenPresent(void)
{
    s32 handle = Func_02001bbe(8);

    if (handle != 0) {
        Func_02001b80(handle, 0);
    }
}
