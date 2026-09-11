#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/actor/multi_encounter/run_scene_configure_three_actors.h"

extern u8 gWork[];

/* Phase/status word at 0x1c0 of the shared scene work record. */

/* Configures actors 20, 21 and 22 (position and movement/sprite flags) and
 * advances the shared scene phase before the scene runs. */
void Scene_ConfigureThreeActors(void)
{
    u32 i;
    s32 record;

    Battle_Reset_1();
    ObjectGroup_ConfigureChildValue_1(0, 15);
    record = Scene_GetRecord_1(0);
    Actor_Run(record, 0);
    Object_NotifyLastActiveOfEvent_1(0x200d160);
    Actor_Run2(1);
    Motion_SetHPosTerrain_1(20, 0xc40000, 0x1f60000);
    record = Scene_GetRecord_2(20);
    {
        /* Set the visibility/active flag at +6. */
        s32 shown = 0xa000;

        *(volatile u16 *)(record + 6) = shown;
    }
    Motion_SetHPosTerrain_2(22, 0xb80000, 0x20c0000);
    record = Scene_GetRecord_3(22);
    {
        /* Set the visibility/active flag at +6. */
        s32 shown = 0xb000;

        *(volatile u16 *)(record + 6) = shown;
    }
    Motion_SetActionVariant_1(21, 1);
    Motion_SetHPosTerrain_3(21, 0xb80000, 0x2780000);
    record = Scene_GetRecord_4(21);
    {
        /* Set the visibility/active flag at +6. */
        s32 shown = 0xb000;

        *(volatile u16 *)(record + 6) = shown;
    }
    SCENE_PHASE = 0x202;
    Battle_WaitMode0_1();
    Motion_SetSpeed_1();
    Battle_WaitMode0_2(20);
    Motion_Launch_1(22, 4, 10);
    Motion_Launch_2(22, 6, 20);
    SceneWork_SetStepValue_1(0x1ee5);
    Actor_Run3(22);
    Motion_CallWaitAnim_1(20, 3);
    Motion_SetSpeed_2(21, 0x30000, 0x18000);
    Motion_SetPosReset_1(21, 180, 0x222);
    Motion_ArmCb_1(21, 0xb000, 40);
    Motion_SetVarCbObj_1(21, 1);
    Actor_Run4(21);
    Actor_Run5(15);
}
