#include "TYPES.H"
#include "PARTY_STATE.H"

s32 Party_AddActiveOwner(s32 owner);
s32 Party_RemoveActiveOwner(s32 owner);

/* The prologue puts Jasmine (owner 5) at the head of the party before the
   Venus Lighthouse scenes. */
void Party_SetLeaderJasmine(void)
{
    gPartyState.current_owner = 5;
}

/* Hands the party to Garcia (owner 4): he joins, and Jasmine and Sheba
   leave it. */
void Party_SetLeaderGarcia(void)
{
    gPartyState.current_owner = 4;
    Party_AddActiveOwner(4);
    Party_RemoveActiveOwner(5);
    Party_RemoveActiveOwner(6);
}
