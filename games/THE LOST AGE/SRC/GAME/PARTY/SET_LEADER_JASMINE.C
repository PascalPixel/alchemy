#include "TYPES.H"
#include "PARTY_STATE.H"

/* The prologue puts Jasmine (owner 5) at the head of the party before the
   Venus Lighthouse scenes. */
void Party_SetLeaderJasmine(void)
{
    gPartyState.current_owner = 5;
}
