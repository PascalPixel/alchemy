#include "audio_engine.h"

void MusicTrack_SetToneAttack(
    struct MusicPlayerState *unused,
    struct MusicTrackState *track)
{
    u8 *cursor;
    u8 value;

    cursor = (u8 *)track->command;
    value = *cursor;
    cursor = &track->tone_attack;
    *cursor = value;
    cursor = (u8 *)track->command;
    cursor++;
    track->command = cursor;
}
