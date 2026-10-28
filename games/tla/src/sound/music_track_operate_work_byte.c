#include "audio_engine.h"

extern SoundCommand gSound_JumpCommand;

void ply_memacc(struct SoundPlayer *player,
                                struct SoundTrack *track) {
  u32 op;
  u8 *byte;
  u8 value;

  op = *track->cursor;
  track->cursor++;
  byte = player->workBytes + *track->cursor;
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
    *byte = player->workBytes[value];
    return;
  case SOUND_WORK_ADD_BYTE:
    *byte += player->workBytes[value];
    return;
  case SOUND_WORK_SUBTRACT_BYTE:
    *byte -= player->workBytes[value];
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
    if (*byte == player->workBytes[value])
      goto jump;
    goto skip;
  case SOUND_WORK_NOT_EQUAL_BYTE:
    if (*byte != player->workBytes[value])
      goto jump;
    goto skip;
  case SOUND_WORK_GREATER_BYTE:
    if (*byte > player->workBytes[value])
      goto jump;
    goto skip;
  case SOUND_WORK_GREATER_EQUAL_BYTE:
    if (*byte >= player->workBytes[value])
      goto jump;
    goto skip;
  case SOUND_WORK_LESS_EQUAL_BYTE:
    if (*byte <= player->workBytes[value])
      goto jump;
    goto skip;
  case SOUND_WORK_LESS_BYTE:
    if (*byte < player->workBytes[value])
      goto jump;
    goto skip;
  default:
    return;
  }

jump:
  gSound_JumpCommand(player, track);
  return;
skip:
  track->cursor += 4;
}
