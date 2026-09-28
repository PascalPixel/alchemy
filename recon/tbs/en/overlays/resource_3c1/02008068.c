/* resource_3c1:02008068..020080b0 (72 bytes with pool), still linked from
 * the listing. Remaining difference: the game loads the first message once
 * (ldr r5, =0x25b8) and derives the answers as r5 + 1 and r5 + 2. With the
 * message as an integer, GCSE propagates the constant into both branches and
 * the pool holds 0x25b8, 0x25b9 and 0x25ba (40 of 72 bytes differ, including
 * the prologue's register choice). Only a symbol-plus-offset base keeps the
 * related value in r5, and a name for message 0x25b8 would need an equate. */
#include "TYPES.H"
#include "FIELD_EVENT.H"

void FieldScene_RunActorCue25b8Branch(s32 obj)
{
    s32 cue = 0x25b8;
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
