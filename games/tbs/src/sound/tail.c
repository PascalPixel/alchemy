#include "audio_engine.h"

void MusicTrack_OperateWorkByte(struct SoundPlayer *player, struct SoundTrack *track)
{
    u32 op;
    u8 *byte;
    u8 value;

    op = *track->cursor;
    track->cursor++;
    byte = player->work_bytes + *track->cursor;
    track->cursor++;
    value = *track->cursor;
    track->cursor++;

    switch (op) {
    case SOUND_WORK_SET:
        *byte = value;
        return;
    case SOUND_WORK_ADD:
        *byte += value;
        return;
    case SOUND_WORK_SUBTRACT:
        *byte -= value;
        return;
    case SOUND_WORK_COPY:
        *byte = player->work_bytes[value];
        return;
    case SOUND_WORK_ADD_BYTE:
        *byte += player->work_bytes[value];
        return;
    case SOUND_WORK_SUBTRACT_BYTE:
        *byte -= player->work_bytes[value];
        return;
    case SOUND_WORK_EQUAL:
        if (*byte == value)
            goto jump;
        goto skip;
    case SOUND_WORK_NOT_EQUAL:
        if (*byte != value)
            goto jump;
        goto skip;
    case SOUND_WORK_GREATER:
        if (*byte > value)
            goto jump;
        goto skip;
    case SOUND_WORK_GREATER_EQUAL:
        if (*byte >= value)
            goto jump;
        goto skip;
    case SOUND_WORK_LESS_EQUAL:
        if (*byte <= value)
            goto jump;
        goto skip;
    case SOUND_WORK_LESS:
        if (*byte < value)
            goto jump;
        goto skip;
    case SOUND_WORK_EQUAL_BYTE:
        if (*byte == player->work_bytes[value])
            goto jump;
        goto skip;
    case SOUND_WORK_NOT_EQUAL_BYTE:
        if (*byte != player->work_bytes[value])
            goto jump;
        goto skip;
    case SOUND_WORK_GREATER_BYTE:
        if (*byte > player->work_bytes[value])
            goto jump;
        goto skip;
    case SOUND_WORK_GREATER_EQUAL_BYTE:
        if (*byte >= player->work_bytes[value])
            goto jump;
        goto skip;
    case SOUND_WORK_LESS_EQUAL_BYTE:
        if (*byte <= player->work_bytes[value])
            goto jump;
        goto skip;
    case SOUND_WORK_LESS_BYTE:
        if (*byte < player->work_bytes[value])
            goto jump;
        goto skip;
    default:
        return;
    }

jump:
    (*(&Sound_CommandTable[1]))(player, track);
    return;
skip:
    track->cursor += 4;
}

void MusicTrack_DispatchStreamCommand(struct SoundPlayer *player, struct SoundTrack *track)
{
    s32 command;

    command = *track->cursor;
    track->cursor++;
    Sound_ExtendedCommandTable[command](player, track);
}

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
