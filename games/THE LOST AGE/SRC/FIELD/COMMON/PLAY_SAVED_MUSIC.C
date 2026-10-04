#include "TYPES.H"
#include "PARTY_STATE.H"

void Audio_PlayCue(s32);

/* Plays the music cue the party state keeps. */
void Party_PlaySavedMusic(void)
{
    Audio_PlayCue(gPartyState.cue);
}
