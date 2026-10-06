#include "TYPES.H"
#include "PARTY_STATE.H"
#include "FIELD_EVENT.H"

void Party_PlaySavedMusic(void)
{
    Audio_PlayCue(gPartyState.cue);
}
