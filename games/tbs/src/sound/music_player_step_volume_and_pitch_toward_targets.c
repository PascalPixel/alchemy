#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "audio_engine_types.h"
#include "audio_engine_symbols.h"

void MusicPlayer_StepVolumeAndPitchTowardTargets(void)
{
    s32 delta;

    if (gMusicRestoreDelay != 0) {
        if (gMusicRestoreDelay == 1) {
            if ((u8)gMusicPlayerFanfare.status == 0) {
                gMusicRestoreDelay = 0;
                gMusicVolumeTarget = 0x100;
            }
        } else {
            gMusicRestoreDelay -= 1;
        }
    }
    if ((s16)gMusicVolumeTarget != (s16)gMusicVolume) {
        delta = (s16)gMusicVolumeTarget - (s16)gMusicVolume;
        if (delta > 0) {
            gMusicVolume = gMusicVolume + gMusicVolumeStep;
        } else {
            gMusicVolume = gMusicVolume - gMusicVolumeStep;
        }
        if ((((s16)gMusicVolumeTarget - (s16)gMusicVolume) ^ delta) < 0) {
            gMusicVolume = gMusicVolumeTarget;
        }
        MusicPlayer_SetVolume(&gMusicPlayerBgm, 255, gMusicVolume);
    }
    if ((s16)gMusicPitchTarget != (s16)gMusicPitch) {
        delta = (s16)gMusicPitchTarget - (s16)gMusicPitch;
        if (delta > 0) {
            gMusicPitch = gMusicPitch + gMusicPitchStep;
        } else {
            gMusicPitch = gMusicPitch - gMusicPitchStep;
        }
        if ((((s16)gMusicPitchTarget - (s16)gMusicPitch) ^ delta) < 0) {
            gMusicPitch = gMusicPitchTarget;
        }
        MusicPlayer_SetPitchAndUpdateFrequency(&gMusicPlayerBgm, gMusicPitch);
        MusicPlayer_SetPitch(&gMusicPlayerBgm, 255, (s16)gMusicPitch * 12 - 3072);
    }
    Sound_TickDmaRestartTimer();
}
