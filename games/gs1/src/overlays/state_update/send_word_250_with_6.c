#include "types.h"

#define SceneState_SendWord250With6 Func_020002e8

extern s16 Data_02000240[];

extern void Func_020042aa(s32, s32, s32);

void SceneState_SendWord250With6(void)
{
    s16 *tbl = Data_02000240;

    Func_020042aa(*(s32 *)(tbl + 250), 6, 0);
}
