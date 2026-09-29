#include "CAVE.H"
extern u8 MsgRunpaGeraldAsksAboutThingsTo[];

u8 Gerald_AsksAboutThingsToDo(void)
{
    Event_SetMessage((s32)MsgRunpaGeraldAsksAboutThingsTo);
    Event_OpenMessage(ACTOR_GERALD, 0);
    return Event_ChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0;
}
