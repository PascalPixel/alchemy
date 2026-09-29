/* Draft of resource_3b6 0x020086ec (SceneDialogue_RunMessage239eStep): it
 * matches the ROM byte for byte now that the message it loads from the
 * literal pool has a catalogue name (MsgTorebiLookLikeWarrior). The listing
 * keeps these rows until the draft is adopted. */
#include "TOREBI.H"
extern u8 MsgTorebiLookLikeWarrior[];

void SceneDialogue_RunMessage239eStep(s32 subject)
{
    s32 message;

    Event_Begin();

    message = (s32)MsgTorebiLookLikeWarrior;
    Event_SetMessage(message);
    Event_OpenMessage(subject, 0);

    /*
     * Both arguments are set to zero immediately before the call, so the
     * predicate is queried with no state from this owner.  Its meaning is not
     * established; the two arms present consecutive ids off the same base.
     */
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_Wait(10);
        Event_SetMessage(message + 1);
    } else {
        Event_SetMessage(message + 2);
    }

    Event_ShowMessage(subject, 0);
    Event_End();
}
