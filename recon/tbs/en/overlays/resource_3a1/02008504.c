/* Draft of resource_3a1 0x02008504 (SceneDialogue_RunActor12Dialogue): it
 * matches the ROM byte for byte now that the message it loads from the
 * literal pool has a catalogue name (MsgShianDidMonstersInAltinSpit). The
 * listing keeps these rows until the draft is adopted. */

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
