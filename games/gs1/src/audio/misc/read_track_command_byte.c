#include "audio_engine.h"

void MusicTrack_ReadCommandByte(
    struct MusicPlayerState *unused_player,
    struct MusicTrackState *track)
{
    const u8 *command = track->command;

    track->command = command + 1;
    *(volatile const u8 *)command;
}
