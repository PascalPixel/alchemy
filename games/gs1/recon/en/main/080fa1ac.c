#include "audio_engine.h"

#define MusicTrack_ClearModulationState Func_080fa1ac

void MusicTrack_ClearModulationState(
    struct MusicPlayerState *unused,
    struct MusicTrackState *track)
{
    s32 bits;

    track->unknown01[0x15] = 0;
    track->unknown01[0x19] = 0;
    if (track->unknown01[0x17] == 0)
        bits = 12;
    else
        bits = 3;
    track->flags |= bits;
}
