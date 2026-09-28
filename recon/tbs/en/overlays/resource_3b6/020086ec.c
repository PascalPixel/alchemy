/* Draft of resource_3b6 0x020086ec (SceneDialogue_RunMessage239eStep), built with
 * games/THE BROKEN SEAL/SRC/FIELD/TOREBI_HEYA/TOREBI.H.
 * Remaining difference: the ROM loads the message number once from the
 * literal pool and forms the following lines by adding to it, as a
 * link-time message value would; the C constant folds each sum into its
 * own pool constant (4 or 8 bytes longer).
 * The listing keeps these rows. */
#include "TOREBI.H"

void SceneDialogue_RunMessage239eStep(s32 subject)
{
    s32 message;

    Event_Begin();

    message = MSG_YOU_WERE_WATCHING_COLOSSO;
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
