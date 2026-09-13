#include "audio_engine.h"

const u8 *MusicTrack_SetPseudoEchoLength(
    struct MusicPlayerState *player, struct MusicTrackState *track)
{
    const u8 *cursor;
    u8 value;

    cursor = track->command;
    value = *cursor;
    track->pseudo_echo_length = value;
    cursor++;
    track->command = cursor;
    return cursor;
}
