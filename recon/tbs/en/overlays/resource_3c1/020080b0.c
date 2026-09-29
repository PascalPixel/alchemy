/* Draft of resource_3c1 0x020080b0 (FieldScene_RunActorCue25dcBranch): it
 * matches the ROM byte for byte now that the message it loads from the
 * literal pool has a catalogue name (MsgSuharaBroughtSuhallaSandstorm). The
 * listing keeps these rows until the draft is adopted. */
#include "TYPES.H"
#include "FIELD_EVENT.H"
extern u8 MsgSuharaBroughtSuhallaSandstorm[];

void FieldScene_RunActorCue25dcBranch(s32 obj)
{
    s32 cue = (s32)MsgSuharaBroughtSuhallaSandstorm;
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
