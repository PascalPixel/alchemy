/* Actor 12 asks whether the monsters in Altin spat water. */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
extern u8 MsgShianDidMonstersInAltinSpit[];

void SceneDialogue_RunActor12Dialogue(void)
{
    void Event_SetMessage(s32);
    s32 Event_AskYesNo(s32, s32);

    Event_Begin();
    Event_SetMessage((s32)MsgShianDidMonstersInAltinSpit);
    Event_AskYesNo(12, 0);
    Event_End();
}
