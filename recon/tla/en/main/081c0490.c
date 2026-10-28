// Draft, differ 18 of 10 with the game profile, and identical with old_agbcc
// -O2 -mthumb-interwork (2026-10-24): the work copy of the sound library's
// MPlayContinue, as in ☀️ (games/tbs/src/sound/audio.c),
// in pret's form; old_agbcc drops the lock's raise and restore, as the
// listing does. Adopting it needs a file the sound route (SOUND_C) compiles
// at this place, between noop_stub_work_copy.c and the driver's
// midi_key_to_frequency.s: a routing change, Pascal's (K1).
#include "audio_engine.h"

void Func_081c0490(struct SoundPlayer *player) {
  if (player->lock == SOUND_LOCK) {
    player->lock++;
    player->status &= ~SOUND_PLAYER_PAUSED;
    player->lock = SOUND_LOCK;
  }
}
