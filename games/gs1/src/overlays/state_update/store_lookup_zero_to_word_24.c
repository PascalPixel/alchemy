#include "types.h"

extern s32 Func_02005752(s32);

void SceneState_StoreLookupZeroToWord24(void)
{
    *(s32 *)(*(u8 **)0x03001ee0 + 24) = Func_02005752(0);
}
