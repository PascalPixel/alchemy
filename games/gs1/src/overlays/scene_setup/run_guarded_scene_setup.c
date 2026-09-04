#include "types.h"

extern s32 Func_02000e04();
extern void Func_02000e64();

typedef void (*SceneSetup)(s32, s32, s32, s32, s32, s32);

#define RunGuardedSceneSetup Func_020004b4
#define RunSceneCommand Func_02000e04
#define FinishSceneSetup Func_02000e64
void RunGuardedSceneSetup(void)
{
    if (RunSceneCommand(0x305) != 0) {
        s32 width = 8;
        s32 height = 13;

        ((SceneSetup)RunSceneCommand)(31, 0, 1, 1, width, height);
        FinishSceneSetup(8, 0);
    }
}
