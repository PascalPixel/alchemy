#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/dialogue/topic/run_actor_cue_branch.h"

extern u8 Value_00000e39;

void Scene_RunActorCueBranch(s32 object)
{
    s32 cue = (s32)&Value_00000e39;
    Talk_Do(cue);
    Talk_Apply(object, 0);
    if (Talk_Apply2(0, 0) == 0) {
        Talk_Do2(10);
        Talk_Do3(cue + 1);
    } else {
        Talk_Do4(cue + 2);
    }
    Talk_Apply3(object, 0);
}
