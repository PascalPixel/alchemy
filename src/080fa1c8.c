#include "audio_engine.h"

void Func_080fa1c8(struct MusicPlayerState *unused, struct MusicTrackState *track)
{
    const u8 *command = track->command;

    track->command = command + 1;
    *(volatile const u8 *)command;
}
