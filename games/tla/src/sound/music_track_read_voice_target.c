// TBS original: games/tbs/src/sound/tail.c
#include "sound_driver.h"

union PackedWord {
  u32 value;
  struct {
    u32 byte0 : 8;
    u32 byte1 : 8;
    u32 byte2 : 8;
    u32 byte3 : 8;
  } part;
};

void System_CallWorkEntryWithArgs(struct SoundPlayer *player,
                                   struct SoundTrack *track) {
  SoundCommand func = *(&gSound_CommandTable[0]);

  func(player, track);
}

void ply_xwave(struct SoundPlayer *player,
                                struct SoundTrack *track) {
  const u8 *pos = track->cursor;
  union PackedWord word;

  word.part.byte0 = pos[0];
  word.part.byte1 = pos[1];
  word.part.byte2 = pos[2];
  word.part.byte3 = pos[3];
  track->voice.target = (const void *)word.value;
  track->cursor = pos + 4;
}
