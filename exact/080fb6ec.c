#include "audio_engine.h"

void Func_080fb6ec(
    struct MusicPlayerState *unused,
    struct MusicTrackState *track)
{
    u8 *cursor;
    u8 value;

    cursor = (u8 *)track->command;
    value = *cursor;
    cursor = &track->byte_24;
    *cursor = value;
    cursor = (u8 *)track->command;
    cursor++;
    track->command = cursor;
}
