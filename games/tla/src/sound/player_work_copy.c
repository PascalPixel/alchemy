/// @file
/// Two player routines of an older copy of the sound library that the ROM
/// keeps beside the driver. Nothing in the build calls them.

#include "sound/sound_driver.h"

/// Unused: the work copy of MPlayContinue (sound.c).
void MPlayContinueWorkCopy(struct MusicPlayerInfo *player) {
  if (player->ident == ID_NUMBER) {
    player->ident++;
    player->status &= ~MUSICPLAYER_STATUS_PAUSE;
    player->ident = ID_NUMBER;
  }
}

/// Unused: the work copy of MPlayFadeOut (sound.c).
void MPlayFadeOutWorkCopy(struct MusicPlayerInfo *player, u16 speed) {
  if (player->ident == ID_NUMBER) {
    player->ident++;
    player->fadeCounter = speed;
    player->fadePeriod = speed;
    player->fadeVolume = SOUND_FADE_FULL_VOLUME;
    player->ident = ID_NUMBER;
  }
}
