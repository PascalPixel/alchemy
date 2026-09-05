#include "types.h"

extern s32 Func_02002de8(s32, s32);

s32 SceneDialogue_IsPromptResultZero(void)
{
    return Func_02002de8(0, 0) == 0;
}
