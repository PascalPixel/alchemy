#include "types.h"

#define SceneState_ClearWorkspaceWord24 Func_020008b8

void SceneState_ClearWorkspaceWord24(void)
{
    *(s32 *)(*(u8 **)0x03001ee0 + 24) = 0;
}
