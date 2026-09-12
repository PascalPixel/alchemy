#include "types.h"
#include "scene.h"

/* overlays/scene_data_get_table9_a08.c */
/* overlays/scene/dialogue/topic/get_table_9a08.c */
/*
 * resource_3b7 owner at 0x02000030: a leaf that loads its literal pool word
 * and returns it. The eight-byte owner includes that pool word at
 * 0x02000034, holding the address 0x02009a08 -- image offset 0x1a08 --
 * returned without being dereferenced. Many rows across the tree share this
 * body but each returns a different address.
 */

u8 *SceneData_GetTable9A08(void)
{
    return (u8 *)0x02009a08;
}

/* overlays/shared/get_default_result.c */
s32 Sys_Run(void)
{
    return 0;
}

/* overlays/scene/dialogue/topic/topic.c */
/* overlays/scene/dialogue/topic/run_scene_steps.c */
/* Contiguous unnamed leaf-owner run for resource_3b7. */

void Dialogue_RunMessage0e34(void)
{
    Talk_Run();
    Talk_DoTopic(0xE34);
    Talk_ApplyTopic(-1, 0);
    Talk_unk2_4();
}

void Dialogue_RunMessage0e35(void)
{
    Talk_unk3_4();
    Talk_unk2_2Topic(0xE35);
    Talk_Apply2Topic(-1, 0);
    Talk_unk4_4();
}

void Scene_RunIndexedStep0(void)
{
    Talk_unk3_2Topic(0);
}

extern s16 gCell[];
extern u8 Value_000000bd;
extern u8 gOv[];
extern u8 gOv2[];

s32 SceneData_SelectTable9e1cByState(void)
{
    if (gCell[224] == (s32)&Value_000000bd) {
        return (s32)gOv;
    }
    return (s32)gOv2;
}

/* overlays/scene/dialogue/topic/table_accessors.c */
/* Contiguous unnamed leaf-owner run for resource_3b7. */

/*
 * resource_3b7 owner at 0x0200003c: a leaf that loads its literal pool word
 * and returns it. The eight-byte owner includes that pool word at
 * 0x02000040, holding the address 0x02009ac8 -- image offset 0x1ac8 --
 * returned without being dereferenced. Many rows across the tree share this
 * body but each returns a different address.
 */

u8 *SceneData_GetTable9ac8(void)
{
    return (u8 *)0x02009ac8;
}

extern u8 gOv3[];
extern u8 gOv4[];

s32 SceneData_SelectTable9cfcByState(void)
{
    if (gCell[224] == (s32)&Value_000000bd) {
        return (s32)gOv3;
    }
    return (s32)gOv4;
}

/* overlays/scene/dialogue/topic/scene_run_actor_cue_branch.c */
/* overlays/scene/dialogue/topic/run_actor_cue_branch.c */
extern u8 Value_00000e39;

void Scene_RunActorCueBranch(s32 object)
{
    s32 cue = (s32)&Value_00000e39;
    Talk_DoActorCueBranch(cue);
    Talk_ApplyActorCueBranch(object, 0);
    if (Talk_Apply2ActorCueBranch(0, 0) == 0) {
        Talk_unk2_2ActorCueBranch(10);
        Talk_unk3_2ActorCueBranch(cue + 1);
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
    Talk_DoActorCueBranch(cue);
    Talk_ApplyActorCueBranch(object, 0);
    if (Talk_Apply2ActorCueBranch(0, 0) == 0) {
        Talk_unk2_2ActorCueBranch(10);
        Talk_unk3_2ActorCueBranch(cue + 1);
    } else {
        Talk_unk4_2(cue + 2);
    }
    Talk_Apply3(object, 0);
}
