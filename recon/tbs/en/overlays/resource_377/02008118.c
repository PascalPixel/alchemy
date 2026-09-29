/* Draft of resource_377 0x02008118 (FieldScene_RunActorCueBranch): it matches
 * the ROM byte for byte now that the message it loads from the literal pool
 * has a catalogue name (MsgHaidiaFolksSeemKnow). The listing keeps these rows
 * until the draft is adopted. */

#include "../../../../../games/THE BROKEN SEAL/SRC/FIELD/COMMON/HAIDIA_BABI/HAIDIA_BABI.H"
extern u8 MsgHaidiaFolksSeemKnow[];

void FieldScene_RunActorCueBranch(s32 object)
{
    s32 cue = (s32)MsgHaidiaFolksSeemKnow;

    Event_SetMessage(cue);
    Event_OpenMessage(object, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_Wait(10);
        Event_SetMessage(cue + 1);
    } else {
        Event_SetMessage(cue + 2);
    }
    Event_ShowMessage(object, 0);
}
