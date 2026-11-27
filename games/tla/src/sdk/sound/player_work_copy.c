/* Two player functions of an older copy of the SDK's sound library that the ROM
   keeps beside the driver. Nothing in the build calls them. */

#include "sdk/sound_driver.h"

/* Unused: the work copy of MPlayContinue (games/common/sdk/sound/sound.c). */
void MPlayContinueWorkCopy(struct MusicPlayerInfo *player)
{
	if(player->ident == ID_NUMBER) {
		player->ident++;
		player->status &= ~MUSICPLAYER_STATUS_PAUSE;
		player->ident = ID_NUMBER;
	}
}

/* Unused: the work copy of MPlayFadeOut (games/common/sdk/sound/sound.c). */
void MPlayFadeOutWorkCopy(struct MusicPlayerInfo *player, Uint16 speed)
{
	if(player->ident == ID_NUMBER) {
		player->ident++;
		player->fadeCounter = speed;
		player->fadePeriod = speed;
		player->fadeVolume = SOUND_FADE_FULL_VOLUME;
		player->ident = ID_NUMBER;
	}
}
