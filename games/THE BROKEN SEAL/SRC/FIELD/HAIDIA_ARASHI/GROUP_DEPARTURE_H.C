#include "GROUP_DEPARTURE.H"
extern u8 MsgHaidiaDoorWontOpen[];
extern u8 MsgHaidiaSChestValuables[];

void SceneState_SetValue140Mode0(void)
{

    Psynergy_Begin(140, 0);
}

void FieldScene_RunSingleStep(void)
{
    BattleEffect_CleanupSceneObjects();
}

void FieldScene_RunFourPairedSteps(void)
{
    OverlayObject_UpdateRandomSlotByFrame((s32)Actor_Get(32));
    OverlayObject_UpdateRandomSlotByFrame((s32)Actor_Get(33));
    OverlayObject_UpdateRandomSlotByFrame((s32)Actor_Get(30));
    if (HaidiaArashi_ShakeDone == 0) {
        OverlayObject_UpdateRandomSlotByFrame((s32)Actor_Get(29));
    }
}

void SceneState_SetValue19ThenCall(void)
{

    OverlayObject_ApplyIwramWord1e40((s32)Actor_Get(19));
}

void OverlayObject_CopyRecordField1ToSlots22And8(void)
{
    Ent *src;
    Ent *dst;
    Ent *dst2;

    src = ((Rec *)Object_GetById(0))->f50;
    dst = ((Rec *)Object_GetById(22))->f50;
    dst->f = src->f;
    dst2 = ((Rec *)Object_GetById(8))->f50;
    dst2->f = src->f;
}

void SceneState_SetValueEe4(void)
{

    Event_Begin();
    Message_ShowCentered((s32)MsgHaidiaDoorWontOpen, 1);
    Event_End();
}

void FieldScene_ShowChestValuables(void)
{
    Event_Begin();
    Message_ShowCentered((s32)MsgHaidiaSChestValuables, 1);
    Event_End();
}
