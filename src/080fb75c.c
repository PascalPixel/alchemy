#include "audio_engine.h"

const u8 *Func_080fb75c(
    struct MusicPlayerState *player, struct MusicTrackState *track)
{
    const u8 *cursor;
    u8 value;

    cursor = track->command;
    value = *cursor;
    track->unknown1f = value;
    cursor++;
    track->command = cursor;
    return cursor;
}
