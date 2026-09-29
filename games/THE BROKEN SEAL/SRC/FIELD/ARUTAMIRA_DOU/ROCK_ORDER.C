/* Actor 8 asks whether the party forgot the order of the rock colours; each
 * answer is a line after the question, yes first. */
#include "ARUTAMIRA.H"
extern u8 MsgArutamiraForgetOrderRock[];

void ArutamiraDou_AskAboutRockOrder(void)
{
    s32 question;

    Event_Begin();
    question = (s32)MsgArutamiraForgetOrderRock;
    Event_SetMessage(question);
    Event_OpenMessage(8, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_Wait(20);
        Event_SetMessage(question + 1);
        Event_ShowMessage(8, 0);
    } else {
        Event_Wait(20);
        Event_SetMessage(question + 2);
        Event_ShowMessage(8, 0);
    }
    Event_End();
}
