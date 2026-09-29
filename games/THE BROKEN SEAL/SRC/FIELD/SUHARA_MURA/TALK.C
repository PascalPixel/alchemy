#include "TYPES.H"
#include "FIELD_EVENT.H"

/*
 * Two villagers' yes/no talks: each opens its question at the speaker and
 * answers with one of the two lines after it in the catalogue.
 */

extern u8 MsgSuharaTryingGetLalivero[];
extern u8 MsgSuharaBroughtSuhallaSandstorm[];

void SuharaMura_TalkLalivero(s32 obj)
{
    s32 cue = (s32)MsgSuharaTryingGetLalivero;
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

void SuharaMura_TalkSandstorm(s32 obj)
{
    s32 cue = (s32)MsgSuharaBroughtSuhallaSandstorm;
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
