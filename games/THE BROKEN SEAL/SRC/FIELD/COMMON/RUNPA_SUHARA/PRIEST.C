#include "INTERIORS.H"

void TravelingPriest_Talk(void)
{
    struct FieldActor *leader;

    leader = Actor_Get(ACTOR_PARTY_LEADER);
    if (FACING_IS_NORTH(leader->facing)) {
        Sanctum_Open(ACTOR_TEMPLE_PRIEST);
    } else {
        Event_SetMessage(MSG_TRAVELING_PRIEST);
        Event_ShowMessage(ACTOR_TRAVELING_PRIEST, 0);
    }
}

