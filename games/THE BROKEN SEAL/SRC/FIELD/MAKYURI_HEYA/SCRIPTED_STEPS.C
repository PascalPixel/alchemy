#include "PROBE.H"

void FieldScene_RunScriptedSteps0And1576(void)
{
    Event_Begin();
    Actor_SetAnimation(0, 1);
    Message_ShowCentered(MSG_FOUNTAIN_HEALING_WATER_HERMES_BRINGS, 1);
    Event_End();
}

void FieldScene_RunScriptedSteps0And953(void)
{
    Event_Begin();
    Actor_SetAnimation(0, 1);
    Message_ShowCentered(MSG_DOOR_TIGHTLY_LOCKED, 1);
    Event_End();
}

void FieldScene_RunFlag881Dialogue(void)
{

    Event_Begin();
    Actor_SetAnimation(0, 1);
    if (GameFlag_IsSet(0x881) == 0)
        Message_ShowCentered(MSG_FOUNTAIN_SEEMS_DRY, 1);
    else
        Message_ShowCentered(MSG_FOUNTAIN_FLOWING_WITH_WATER, 1);
    if (PartyInventory_FindOwner(0xb9) != -1) {
        s16 *slot = (s16 *)gEventWork + 185;
        s32 one = 1;

        *slot = one;
    }
    Event_End();
}

void FieldScene_RunActor184Sequence(void)
{
    Event_Begin();
    Audio_PlayCue(0x53);
    Item_ShowFound(ITEM_HERMES_WATER, 3);
    SceneState_SetRecordTableValue(0xb9, 0xb8);
    UiWork_PushValueSlot(PartyInventory_FindOwner(0xb8), 1);
    UiWork_PushValueSlot(0xb8, 2);
    Message_ShowCentered(MSG_ROBIN_GOT, 1);
    GameFlag_Set(512);
    Event_End();
}
