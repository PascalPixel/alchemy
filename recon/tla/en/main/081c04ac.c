// Draft, differ 26 of 13: an older sound library's fade-out, as ☀️'s
// MusicPlayer_BeginFadeOut (games/tbs/src/sound/audio.c) without the
// lock's increment around the stores. A sound-library function: the draft
// compiles with the game profile, where the build routes the library
// through old_agbcc. Other: the game compiler saves lr and narrows speed
// again inside the test; the listing narrows it once, up front.
#include "types.h"
#include "audio_engine.h"

void Func_081c04ac(struct SoundPlayer *player, u16 speed) {
  if (player->lock == SOUND_LOCK) {
    player->fadeCounter = speed;
    player->fadePeriod = speed;
    player->fadeVolume = SOUND_FADE_FULL_VOLUME;
  }
}
