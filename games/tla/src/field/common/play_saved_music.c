#include "types.h"
#include "party_state.h"
#include "field_event.h"

void Party_PlaySavedMusic(void) { Audio_PlayCue(gPartyState.cue); }
