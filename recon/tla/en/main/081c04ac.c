// Draft, differ 26 of 13 with the game profile, and identical with old_agbcc
// -O2 -mthumb-interwork (2026-10-24): the work copy of the sound library's
// MPlayFadeOut, as in ☀️ (games/tbs/src/sound/audio.c),
// in pret's form; old_agbcc drops the lock's raise and restore, as the
// listing does (without them it narrows speed into r3, not r1). The listing's
// pool word is followed by the next function's first instructions, from
// 081c04cc. Adopting it needs a file the sound route (SOUND_C) compiles at
// this place: a routing change, Pascal's (K1).
#include "types.h"
#include "sound_driver.h"

void Func_081c04ac(struct SoundPlayer *player, u16 speed) {
  if (player->lock == SOUND_LOCK) {
    player->lock++;
    player->fadeCounter = speed;
    player->fadePeriod = speed;
    player->fadeVolume = SOUND_FADE_FULL_VOLUME;
    player->lock = SOUND_LOCK;
  }
}
