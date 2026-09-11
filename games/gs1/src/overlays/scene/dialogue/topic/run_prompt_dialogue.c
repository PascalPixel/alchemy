#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/dialogue/topic/run_prompt_dialogue.h"

extern u8 Value_00000e19;

void Scene_RunPromptDialogueE19(s32 object)
{
    s32 cue = (s32)&Value_00000e19;
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
