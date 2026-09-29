/* Draft of resource_3bd 0x0200b4bc
 * (FieldScene_RunTwoArmSequenceWithValue217f): it matches the ROM byte for
 * byte now that the message it loads from the literal pool has a catalogue
 * name (MsgArutamiraForgetOrderRock). The listing keeps these rows until the
 * draft is adopted. */
#include "ARUTAMIRA.H"
extern u8 MsgArutamiraForgetOrderRock[];

void FieldScene_RunTwoArmSequenceWithValue217f(void)
{
    s32 val;

    Event_Begin();
    val = (s32)MsgArutamiraForgetOrderRock;
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
