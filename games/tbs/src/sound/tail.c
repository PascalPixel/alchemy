#include "audio_engine.h"

union PackedWord {
    u32 value;
    struct {
        u32 byte0:8;
        u32 byte1:8;
        u32 byte2:8;
        u32 byte3:8;
    } part;
};

void Runtime_CallWorkEntryWithArgs(struct SoundPlayer *player, struct SoundTrack *track)
{
    SoundCommand func = *(&Sound_CommandTable[0]);

    func(player, track);
}

void MusicTrack_ReadVoiceTarget(struct SoundPlayer *player, struct SoundTrack *track)
{
    const u8 *pos = track->cursor;
    union PackedWord word;

    word.part.byte0 = pos[0];
    word.part.byte1 = pos[1];
    word.part.byte2 = pos[2];
    word.part.byte3 = pos[3];
    track->voice.target = (const void *)word.value;
    track->cursor = pos + 4;
}

void MusicTrack_SetVoiceKind(struct SoundPlayer *player, struct SoundTrack *track)
{
    track->voice.kind = *track->cursor;
    track->cursor++;
}

void MusicTrack_SetVoiceAttack(struct SoundPlayer *player, struct SoundTrack *track)
{
    track->voice.attack = *track->cursor;
    track->cursor++;
}

void MusicTrack_SetVoiceDecay(struct SoundPlayer *player, struct SoundTrack *track)
{
    track->voice.decay = *track->cursor;
    track->cursor++;
}

void MusicTrack_SetVoiceSustain(struct SoundPlayer *player, struct SoundTrack *track)
{
    track->voice.sustain = *track->cursor;
    track->cursor++;
}

void MusicTrack_SetVoiceRelease(struct SoundPlayer *player, struct SoundTrack *track)
{
    track->voice.release = *track->cursor;
    track->cursor++;
}

void MusicTrack_SetTailLevel(struct SoundPlayer *player, struct SoundTrack *track)
{
    track->tail_level = *track->cursor;
    track->cursor++;
}

void MusicTrack_SetTailLength(struct SoundPlayer *player, struct SoundTrack *track)
{
    track->tail_length = *track->cursor;
    track->cursor++;
}

void MusicTrack_SetVoiceLength(struct SoundPlayer *player, struct SoundTrack *track)
{
    track->voice.length = *track->cursor;
    track->cursor++;
}

void MusicTrack_SetVoicePanOrSweep(struct SoundPlayer *player, struct SoundTrack *track)
{
    track->voice.pan_or_sweep = *track->cursor;
    track->cursor++;
}
