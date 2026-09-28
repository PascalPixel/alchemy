/* NONMATCHING: resource_3c6 0x020080c4, SceneActor_UpdateObjectWithCue28be,
 * from FIELD/RARIBERO_MACHI/FLAGGED_CUE.C (2026-09-28).
 * The game loads message 0x28be from a literal once (ldr r5) and speaks the
 * next two lines as r5 + 1 and r5 + 2, so the base was a link-time value.
 * A plain constant folds every line into its own literal or mov/lsl pair.
 * Remaining: express the message base without an equate; until the text
 * build names its messages, the listing keeps these rows. */
#include "TYPES.H"
#include "FIELD_EVENT.H"

enum {
    MSG_CUE_28BE = 0x28be
};

void SceneActor_UpdateObjectWithCue28be(s32 obj)
{
    s32 cue = MSG_CUE_28BE;
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
