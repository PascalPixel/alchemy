#include "types.h"

#define SceneDialogue_ShowLine16BF Func_020002c8

extern void Func_02001384(void);
extern void Func_02001402(s32);
extern s32 Func_02001422(s32, s32);
extern void Func_0200139e(void);

void SceneDialogue_ShowLine16BF(void)
{
    Func_02001384();
    Func_02001402(0x16BF);
    Func_02001422(21, 0);
    Func_0200139e();
}
