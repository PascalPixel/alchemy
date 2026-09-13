#include "scene.h"
#include "audio_engine.h"
#include "audio_engine_symbols.h"
#include "types.h"

/* audio/misc/read_track_command_byte.c */
void MusicTrack_ReadCommandByte(
    struct MusicPlayerState *unused_player,
    struct MusicTrackState *track)
{
    const u8 *command = track->command;

    track->command = command + 1;
    *(volatile const u8 *)command;
}
