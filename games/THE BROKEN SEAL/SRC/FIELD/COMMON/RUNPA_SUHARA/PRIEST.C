#include "INTERIORS.H"
extern u8 MsgRunpaTravelingPriest[];

void TravelingPriest_Talk(void)
{
    struct FieldActor *leader;

    leader = Actor_Get(ACTOR_PARTY_LEADER);
    if (FACING_IS_NORTH(leader->facing)) {
        Sanctum_Open(ACTOR_TEMPLE_PRIEST);
    } else {
        Event_SetMessage((s32)MsgRunpaTravelingPriest);
        Event_ShowMessage(ACTOR_TRAVELING_PRIEST, 0);
    }
}

