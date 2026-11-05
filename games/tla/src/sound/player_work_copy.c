/// @file
/// Two player routines of an older copy of the sound library that the ROM
/// keeps beside the driver. Nothing in the build calls them.

#include "sound_driver.h"

/// Unused: the work copy of MPlayContinue (sound.c).
void Func_081c0490(struct SoundPlayer *player) {
  if (player->lock == SOUND_LOCK) {
    player->lock++;
    player->status &= ~SOUND_PLAYER_PAUSED;
    player->lock = SOUND_LOCK;
  }
}

/// Unused: the work copy of MPlayFadeOut (sound.c).
void Func_081c04ac(struct SoundPlayer *player, u16 speed) {
  if (player->lock == SOUND_LOCK) {
    player->lock++;
    player->fadeCounter = speed;
    player->fadePeriod = speed;
    player->fadeVolume = SOUND_FADE_FULL_VOLUME;
    player->lock = SOUND_LOCK;
  }
}
