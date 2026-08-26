#include "audio_engine.h"

#define MusicTrack_ClearModulationState Func_080fa1ac

void MusicTrack_ClearModulationState(
    struct MusicPlayerState *unused,
    struct MusicTrackState *track)
{
    track->unknown01[0x15] = 0;
    track->unknown01[0x19] = 0;
    if (track->unknown01[0x17] == 0)
        track->flags |= 12;
    else
        track->flags |= 3;
}
