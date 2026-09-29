/* The two yes/no questions the spring's attendants ask. Each answer is the
 * line after its question, yes first. */
#include "TYPES.H"
#include "FIELD_EVENT.H"
extern u8 MsgTorebiLuckyMedal[];
extern u8 MsgTorebiYerFirstTime[];

void TorebiIzumi_AskForLuckyMedal(s32 object)
{
    s32 question = (s32)MsgTorebiLuckyMedal;
    Event_SetMessage(question);
    Event_OpenMessage(object, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_Wait(10);
        Event_SetMessage(question + 1);
    } else {
        Event_SetMessage(question + 2);
    }
    Event_ShowMessage(object, 0);
}

void TorebiIzumi_AskIfFirstTime(s32 object)
{
    s32 question = (s32)MsgTorebiYerFirstTime;
    Event_SetMessage(question);
    Event_OpenMessage(object, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_Wait(10);
        Event_SetMessage(question + 1);
    } else {
        Event_SetMessage(question + 2);
    }
    Event_ShowMessage(object, 0);
}
