#include "types.h"

void SceneState_ClearWorkspaceWord24(void)
{
    *(s32 *)(*(u8 **)0x03001ee0 + 24) = 0;
}
