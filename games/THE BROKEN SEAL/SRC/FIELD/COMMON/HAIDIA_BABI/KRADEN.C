/* Asks whether the party knows Kraden, with a line for each answer. */
#include "HAIDIA_BABI.H"
extern u8 MsgHaidiaFolksSeemKnow[];

void HaidiaBabi_AskAboutKraden(s32 object)
{
    s32 msg = (s32)MsgHaidiaFolksSeemKnow;

    Event_SetMessage(msg);
    Event_OpenMessage(object, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_Wait(10);
        Event_SetMessage(msg + 1);
    } else {
        Event_SetMessage(msg + 2);
    }
    Event_ShowMessage(object, 0);
}
