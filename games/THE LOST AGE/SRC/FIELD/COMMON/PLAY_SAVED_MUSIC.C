#include "TYPES.H"
#include "PARTY_STATE.H"
#include "FIELD_EVENT.H"

/* Plays the music cue the party state keeps. */
void Party_PlaySavedMusic(void)
{
    Audio_PlayCue(gPartyState.cue);
}
