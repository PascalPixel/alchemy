/* NONMATCHING: resource_3bd 0x0200b4bc, FieldScene_RunTwoArmSequenceWithValue217f,
 * from FIELD/ARUTAMIRA_DOU/EXTENDED_PRESENTATION.C (2026-09-28).
 * The game loads message 0x217f once from a literal and speaks base + 1 and
 * base + 2 from it; a plain constant folds each line into its own literal.
 * Remaining: the message base without an equate. */
#include "ARUTAMIRA.H"

void FieldScene_RunTwoArmSequenceWithValue217f(void)
{
    s32 val;

    Event_Begin();
    val = (s32)&Value_0000217f;
    Event_SetMessage(val);
    Event_OpenMessage(8, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_Wait(20);
        Event_SetMessage(val + 1);
        Event_ShowMessage(8, 0);
    } else {
        Event_Wait(20);
        Event_SetMessage(val + 2);
        Event_ShowMessage(8, 0);
    }
    Event_End();
}
