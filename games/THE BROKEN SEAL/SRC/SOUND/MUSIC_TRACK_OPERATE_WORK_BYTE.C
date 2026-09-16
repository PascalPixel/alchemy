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
    case 0:
        *byte = value;
        return;
    case 1:
        *byte += value;
        return;
    case 2:
        *byte -= value;
        return;
    case 3:
        *byte = player->work_bytes[value];
        return;
    case 4:
        *byte += player->work_bytes[value];
        return;
    case 5:
        *byte -= player->work_bytes[value];
        return;
    case 6:
        if (*byte == value)
            goto jump;
        goto skip;
    case 7:
        if (*byte != value)
            goto jump;
        goto skip;
    case 8:
        if (*byte > value)
            goto jump;
        goto skip;
    case 9:
        if (*byte >= value)
            goto jump;
        goto skip;
    case 10:
        if (*byte <= value)
            goto jump;
        goto skip;
    case 11:
        if (*byte < value)
            goto jump;
        goto skip;
    case 12:
        if (*byte == player->work_bytes[value])
            goto jump;
        goto skip;
    case 13:
        if (*byte != player->work_bytes[value])
            goto jump;
        goto skip;
    case 14:
        if (*byte > player->work_bytes[value])
            goto jump;
        goto skip;
    case 15:
        if (*byte >= player->work_bytes[value])
            goto jump;
        goto skip;
    case 16:
        if (*byte <= player->work_bytes[value])
            goto jump;
        goto skip;
    case 17:
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
