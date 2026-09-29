/* NONMATCHING: resource_377 at 0x02008118 (72 bytes with its pool),
 * FieldScene_RunActorCueBranch, between FIELD/COMMON/HAIDIA_BABI/FACING_TARGET.C
 * and EXITS.C, stays listing.
 *
 * Remaining difference: the reference loads message 0x22b9 once from its
 * pool into r5 and shows the answers as r5 + 1 and r5 + 2, as a link-time
 * message symbol does; a plain constant (with or without a do/while wrap)
 * is folded into three pool words, 76 bytes. The main image has no name
 * for the message.
 */

#include "../../../../../games/THE BROKEN SEAL/SRC/FIELD/COMMON/HAIDIA_BABI/HAIDIA_BABI.H"

void FieldScene_RunActorCueBranch(s32 object)
{
    s32 cue = 0x22b9;

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
