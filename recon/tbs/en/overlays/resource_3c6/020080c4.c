/* Draft of resource_3c6 0x020080c4 (SceneActor_UpdateObjectWithCue28be): it
 * matches the ROM byte for byte now that the message it loads from the
 * literal pool has a catalogue name (MsgRariberoCue28be). The listing keeps
 * these rows until the draft is adopted. */
#include "TYPES.H"
#include "FIELD_EVENT.H"
extern u8 MsgRariberoCue28be[];


void SceneActor_UpdateObjectWithCue28be(s32 obj)
{
    s32 cue = (s32)MsgRariberoCue28be;
    Event_SetMessage(cue);
    Event_OpenMessage(obj, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_Wait(10);
        Event_SetMessage(cue + 1);
    } else {
        Event_SetMessage(cue + 2);
    }
    Event_ShowMessage(obj, 0);
}
