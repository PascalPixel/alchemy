/* Draft of resource_3b6 0x02008760 (SceneDialogue_RunMessage1FBBStep): it
 * matches the ROM byte for byte now that the message it loads from the
 * literal pool has a catalogue name (MsgTorebiWantStay). The listing keeps
 * these rows until the draft is adopted. */
#include "TOREBI.H"
extern u8 MsgTorebiWantStay[];

void SceneDialogue_RunMessage1FBBStep(s32 subject)
{
    s32 msg;

    Event_Begin();

    msg = (s32)MsgTorebiWantStay;
    Event_SetMessage(msg);
    Event_OpenMessage(subject, 0);

    /*
     * Both arguments are set to zero immediately before the call, so the
     * predicate is queried with no state from this owner.  Its meaning is not
     * established; the two arms present consecutive ids off the same base.
     */
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_Wait(10);
        Event_SetMessage(msg + 1);
    } else {
        Event_SetMessage(msg + 2);
    }

    Event_ShowMessage(subject, 0);
    Event_End();
}
