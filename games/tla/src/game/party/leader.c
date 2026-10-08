#include "types.h"
#include "party_state.h"

s32 Party_AddActiveOwner(s32 owner);
s32 Party_RemoveActiveOwner(s32 owner);

void Party_SetLeaderJasmine(void)
{
    gPartyState.current_owner = 5;
}

void Party_SetLeaderGarcia(void)
{
    gPartyState.current_owner = 4;
    Party_AddActiveOwner(4);
    Party_RemoveActiveOwner(5);
    Party_RemoveActiveOwner(6);
}
