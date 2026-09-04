#include "types.h"

#define SceneDialogue_ShowLine255DAndReturnOne Func_0200134c

extern void Func_02002ef6(s32);
extern s32 Func_02002f06(s32, s32);

s32 SceneDialogue_ShowLine255DAndReturnOne(void)
{
    Func_02002ef6(0x255D);
    Func_02002f06(1, 0);
    return 1;
}
