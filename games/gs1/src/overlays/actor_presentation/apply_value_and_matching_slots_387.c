#include "types.h"

#define SceneActor_ApplyValueAndMatchingSlots Func_02000f8c

extern u8 *Func_0200207c(s32 group);
extern s32 Func_02002096(s32 group, s32 value);
extern void Func_020020b2(s32 group, s32 index);

/* Apply a value to every matching member of a fifteen-slot group. */
void SceneActor_ApplyValueAndMatchingSlots(s32 group, s32 value)
{
    u8 *work = Func_0200207c(group);
    s32 i;
    Func_02002096(group, value);
    for (i = 0; i < 15; i++) {
        if (*(u16 *)(work + 216 + i * 2) == value)
            Func_020020b2(group, i);
    }
}
