#include "CAVE.H"

u8 Gerald_AsksAboutThingsToDo(void)
{
    Event_SetMessage(MSG_GERALD_ASKS_ABOUT_THINGS_TO_DO);
    Event_OpenMessage(ACTOR_GERALD, 0);
    return Event_ChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0;
}
