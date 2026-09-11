#include "types.h"
#include "scene.h"

/* overlays/scene/dialogue/topic/run_actor_cue_branch.c */
extern u8 Value_00000e39;

void Scene_RunActorCueBranch(s32 object)
{
    s32 cue = (s32)&Value_00000e39;
    Talk_Do(cue);
    Talk_Apply(object, 0);
    if (Talk_Apply2(0, 0) == 0) {
        Talk_unk2_2(10);
        Talk_unk3_2(cue + 1);
    } else {
        Talk_unk4_2(cue + 2);
    }
    Talk_Apply3(object, 0);
}

/* overlays/scene/dialogue/topic/run_prompt_dialogue.c */
extern u8 Value_00000e19;

void Scene_RunPromptDialogueE19(s32 object)
{
    s32 cue = (s32)&Value_00000e19;
    Talk_Do(cue);
    Talk_Apply(object, 0);
    if (Talk_Apply2(0, 0) == 0) {
        Talk_unk2_2(10);
        Talk_unk3_2(cue + 1);
    } else {
        Talk_unk4_2(cue + 2);
    }
    Talk_Apply3(object, 0);
}
