/* Set the deck's slot actors 8 to 19 out for the crossing: every slot
   value to its top band, the slots active, the modes cleared, the standing
   slots' depths kept, and each slot placed by its value. */
#include "TYPES.H"
#include "FIELD_EVENT.H"

extern u16 FuneKanpan_SlotValue[];
extern s32 FuneKanpan_SlotMode[];
extern s32 FuneKanpan_SlotDepth[];

void OverlayObject_ActivateSlotWithMode3(s32 actor);
void SceneEffect_SelectSlotValueAndPosition(s32 actor, s32 slot, s32 row);
void ObjectVisual_CopyAttributes(s32 actor, s32 source);

void SceneState_InitActorSlots8To19(void)
{
    struct FieldActor *actor;
    u32 i;

    for (i = 0; i <= 7; i++)
        FuneKanpan_SlotValue[i] = 0xc000;
    OverlayObject_ActivateSlotWithMode3(8);
    Engine_ActorSetPosition(9, 0, 0);
    Engine_ActorSetPosition(10, 0, 0);
    Engine_ActorSetPosition(11, 0, 0);
    Engine_ActorSetPosition(12, 0, 0);
    OverlayObject_ActivateSlotWithMode3(13);
    OverlayObject_ActivateSlotWithMode3(14);
    OverlayObject_ActivateSlotWithMode3(15);
    FuneKanpan_SlotMode[0] = 0;
    FuneKanpan_SlotMode[1] = 0;
    FuneKanpan_SlotMode[2] = 0;
    FuneKanpan_SlotMode[3] = 0;
    FuneKanpan_SlotDepth[0] = Object_GetById(8)->z.fixed;
    FuneKanpan_SlotDepth[1] = Object_GetById(13)->z.fixed;
    FuneKanpan_SlotDepth[2] = Object_GetById(14)->z.fixed;
    FuneKanpan_SlotDepth[3] = Object_GetById(15)->z.fixed;
    OverlayObject_ActivateSlotWithMode3(16);
    OverlayObject_ActivateSlotWithMode3(17);
    OverlayObject_ActivateSlotWithMode3(18);
    OverlayObject_ActivateSlotWithMode3(19);
    Object_GetById(16)->scale_x = 0xffff0000;
    Object_GetById(17)->scale_x = 0xffff0000;
    Object_GetById(18)->scale_x = 0xffff0000;
    Object_GetById(19)->scale_x = 0xffff0000;
    FuneKanpan_SlotMode[4] = 0;
    FuneKanpan_SlotMode[5] = 0;
    FuneKanpan_SlotMode[6] = 0;
    FuneKanpan_SlotMode[7] = 0;
    FuneKanpan_SlotDepth[4] = Object_GetById(16)->z.fixed;
    FuneKanpan_SlotDepth[5] = Object_GetById(17)->z.fixed;
    FuneKanpan_SlotDepth[6] = Object_GetById(18)->z.fixed;
    FuneKanpan_SlotDepth[7] = Object_GetById(19)->z.fixed;
    actor = Object_GetById(0);
    if (actor != NULL)
        Engine_ActorSetPosition(8, actor->x.fixed, actor->z.fixed);
    Engine_TaskWait(1);
    ObjectVisual_CopyAttributes(13, 8);
    ObjectVisual_CopyAttributes(14, 8);
    ObjectVisual_CopyAttributes(15, 8);
    ObjectVisual_CopyAttributes(16, 8);
    ObjectVisual_CopyAttributes(17, 8);
    ObjectVisual_CopyAttributes(18, 8);
    ObjectVisual_CopyAttributes(19, 8);
    Object_GetById(8)->unknown_5c = 1;
    Object_GetById(13)->unknown_5c = 1;
    Object_GetById(14)->unknown_5c = 1;
    Object_GetById(15)->unknown_5c = 1;
    Object_GetById(16)->unknown_5c = 1;
    Object_GetById(17)->unknown_5c = 1;
    Object_GetById(18)->unknown_5c = 1;
    Object_GetById(19)->unknown_5c = 1;
    Engine_TaskWait(1);
    Engine_ActorSetPosition(8, 0x840000, 0x2780000);
    Engine_TaskWait(1);
    SceneEffect_SelectSlotValueAndPosition(8, 0, 2);
    SceneEffect_SelectSlotValueAndPosition(13, 1, 2);
    SceneEffect_SelectSlotValueAndPosition(14, 2, 2);
    SceneEffect_SelectSlotValueAndPosition(15, 3, 2);
    SceneEffect_SelectSlotValueAndPosition(16, 4, 3);
    SceneEffect_SelectSlotValueAndPosition(17, 5, 3);
    SceneEffect_SelectSlotValueAndPosition(18, 6, 3);
    SceneEffect_SelectSlotValueAndPosition(19, 7, 3);
}
