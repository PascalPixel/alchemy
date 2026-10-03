#include "AUDIO_ENGINE.H"

extern SoundCommand Sound_JumpCommand;

/*
 * Reads an operation, a work byte index and an operand from the track. The
 * first six operations write the byte; the rest compare it and either take
 * the jump command that follows or step over its four-byte target.
 */
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
    Sound_JumpCommand(player, track);
    return;
skip:
    track->cursor += 4;
}
