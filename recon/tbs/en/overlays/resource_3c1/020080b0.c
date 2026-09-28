/* resource_3c1:020080b0..020080f8 (72 bytes with pool), still linked from
 * the listing. Remaining difference: as 02008068.c, the game derives the
 * answer messages from one loaded base (r5 + 1, r5 + 2), while an integer
 * message 0x25dc is propagated into three separate pool constants. */
#include "TYPES.H"
#include "FIELD_EVENT.H"

void FieldScene_RunActorCue25dcBranch(s32 obj)
{
    s32 cue = 0x25dc;
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
