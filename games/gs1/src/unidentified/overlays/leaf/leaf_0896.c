#include "types.h"

/* Every callee here is overlay-local. The overlay's own assembly names them:
   `bl sub_02002680` and so on, declared at the head of the file. */
void Func_02002680(void);
s32 Func_02002656(s32);
void Func_02002728(s32);
void Func_02002730(s32);
void Func_0200161e(s32);
void Func_020026aa(void);

#define BeginSceneStep    Func_02002680
#define SceneFlagIsClear  Func_02002656
#define PlaySceneCueA     Func_02002728
#define PlaySceneCueB     Func_02002730
#define SetSceneActor     Func_0200161e
#define EndSceneStep      Func_020026aa
#define RunSceneStep      Func_02000cc4

void RunSceneStep(void) {
    BeginSceneStep();
    if (SceneFlagIsClear(0x855) == 0) {
        PlaySceneCueA(0x123b);
    } else {
        PlaySceneCueB(0x1348);
    }
    SetSceneActor(13);
    EndSceneStep();
}
