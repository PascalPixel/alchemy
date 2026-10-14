#include "audio_engine.h"

void MusicTrack_DispatchStreamCommand(struct SoundPlayer *player, struct SoundTrack *track)
{
    s32 command;

    command = *track->cursor;
    track->cursor++;
    Sound_ExtendedCommandTable[command](player, track);
}
