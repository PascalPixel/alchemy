#include "unit/sound/sound.h"
#include "unit/system/scheduler/runtime.h"
#include "unit/system/scheduler/runtime2.h"
#include "unit/system/scheduler/runtime3.h"
#include "unit/system/scheduler/runtime4.h"
#include "unit/system/scheduler/runtime5.h"
#include "unit/system/scheduler/runtime6.h"
#include "unit/system/scheduler/runtime7.h"
#include "unit/system/scheduler/runtime8.h"
#include "unit/system/scheduler/runtime9.h"
#include "unit/system/scheduler/runtim10.h"
#include "unit/system/scheduler/runtim11.h"
#include "unit/system/scheduler/runtim13.h"
#include "battle.h"
#include "keystate.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "audio_engine.h"
#include "audio_engine_types.h"
#include "audio_engine_symbols.h"
#include "types.h"
#include "io_reg.h"

void AudioCommand_Play(s32 id)
{
    s32 flags;
    s32 player;

    flags = id & AUDIO_CUE_FLAGS_MASK;
    id &= AUDIO_CUE_ID_MASK;

    if (id == AUDIO_CUE_FADE_BGM) {
        if (gMusicFadeRequested != 0)
            return;
        MusicPlayer_FadeOut(&gMusicPlayerBgm, 7);
        gMusicFadeRequested++;
        gAudioSecondaryState = 19;
    } else if (id == AUDIO_CUE_FADE_SLOT3) {
        gMusicPlayerVolumes[3] = 0;
        MusicPlayer_FadeOut(&Sound_Player3, 3);
    } else if (id >= AUDIO_SONG_FIRST) {
        player = Sound_SongTable[id].slot;
        if (player == 7) {
        next:
            if (((u8 *)&Sound_PlayerSlots[player].player->status)[0] != 0) {
                player--;
                if (player > 3)
                    goto next;
                player = 7;
            }
        }
        MusicPlayer_StartSong(Sound_PlayerSlots[player].player, Sound_SongTable[id].header);
        gMusicPlayerVolumes[player] = id;
    } else if (id >= AUDIO_JINGLE_FIRST) {
        MusicPlayer_SetVolume(&gMusicPlayerBgm, 255, 0);
        gMusicVolumeTarget = 0;
        gMusicVolume = 0;
        Audio_PlaySound(id);
        gMusicRestoreDelay = 10;
    } else {
        if (id == AUDIO_CUE_KEEP_BGM)
            return;
        if (id == gAudioSecondaryState)
            return;
        gAudioSecondaryState = id;
        Sound_LoadPresetParameters((id == 70 || id == 75 || id == 67) ? 3 : 2);
        Audio_PlaySound(id);
        if (flags & AUDIO_CUE_START_SILENT)
            gMusicVolume = 0;
        else
            gMusicVolume = 0x100;
        gMusicVolumeTarget = 0x100;
        gMusicVolumeStep = 4;
        gMusicFadeRequested = 0;
    }
}

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
            gMusicVolume += gMusicVolumeStep;
        } else {
            gMusicVolume -= gMusicVolumeStep;
        }
        if ((((s16)gMusicVolumeTarget - (s16)gMusicVolume) ^ delta) < 0) {
            gMusicVolume = gMusicVolumeTarget;
        }
        MusicPlayer_SetVolume(&gMusicPlayerBgm, 255, gMusicVolume);
    }
    if ((s16)gMusicPitchTarget != (s16)gMusicPitch) {
        delta = (s16)gMusicPitchTarget - (s16)gMusicPitch;
        if (delta > 0) {
            gMusicPitch += gMusicPitchStep;
        } else {
            gMusicPitch -= gMusicPitchStep;
        }
        if ((((s16)gMusicPitchTarget - (s16)gMusicPitch) ^ delta) < 0) {
            gMusicPitch = gMusicPitchTarget;
        }
        MusicPlayer_SetPitchAndUpdateFrequency(&gMusicPlayerBgm, gMusicPitch);
        MusicPlayer_SetPitch(&gMusicPlayerBgm, 255, (s16)gMusicPitch * 12 - 3072);
    }
    Sound_TickDmaRestartTimer();
}


void AudioTest_RunParameterController(void)
{
    s32 cues[3] = {0, 80, 100};
    s32 preset;
    s32 row;
    s32 timer;

    row = 0;
    preset = 2;
    /* FAKEMATCH: the one-pass block keeps the state-address load after preset setup and the state store before timer setup. Plain store: differ 8 or more in nine statement orders. */
    do {
        IwramHi_Data1 = 0;
    } while (0);
    timer = 20;

    for (;;) {
        if (timer != 0)
            timer--;

        if (gSoundClipFlag != 0) {
            gSoundClipFlag = 0;
            timer = 20;
        }

        if (gKeysRepeat & KEY_SELECT) {
            preset = (preset + 1) % 5;
            Sound_LoadPresetParameters(preset);
        }
        if (gKeysRepeat & KEY_R)
            cues[row] += 10;
        if (gKeysRepeat & KEY_L)
            cues[row] -= 10;
        if (gKeysRepeat & KEY_RIGHT)
            cues[row] += 1;
        if (gKeysRepeat & KEY_LEFT)
            cues[row] -= 1;
        if (gKeysRepeat & KEY_UP) {
            if (row > 0)
                row--;
        }
        if (gKeysRepeat & KEY_DOWN) {
            if (row <= 1)
                row++;
        }
        if (gKeysRepeat & KEY_A)
            AudioCommand_Play(cues[row]);
        if (gKeysRepeat & KEY_B)
            AudioCommand_Play(19);
        if (gKeysRepeat & KEY_START)
            AudioCommand_Play(17);
        if (gKeysRepeat & KEY_SELECT)
            AudioCommand_Play(0x121);

        WaitFrames(1);
    }
}

void Audio_InitializeRuntimeDefaults(void)
{
    s16 *player_volume;
    s32 remaining;

    Audio_Initialize();
    gAudioSecondaryState = 0xff;
    gMusicRestoreDelay = 0;
    gMusicVolumeTarget = 0x100;
    gMusicVolume = 0x100;
    gMusicVolumeStep = 4;
    gMusicPitchTarget = 0x100;
    gMusicPitch = 0x100;
    gMusicPitchStep = 4;
    gMusicFadeRequested = 0;
    Audio_CommandMask = 0;
    player_volume = gMusicPlayerVolumes;
    Ewram_Data1 = 0;
    remaining = 7;
    do {
        remaining--;
        *player_volume = 0;
        player_volume += 1;
    } while (remaining >= 0);
}

void MusicCommand_SetPitchAndUpdateFrequency(u16 value)
{
    MusicPlayer_SetPitchAndUpdateFrequency(&gMusicPlayerBgm, value);
}

void MusicCommand_SetPitch(u16 pitch)
{
    MusicPlayer_SetPitch(&gMusicPlayerBgm, 0xFF, pitch);
}

void Audio_SetWorkPairB(u16 primary, u16 secondary)
{
    gMusicPitchTarget = primary;
    gMusicPitchStep = secondary;
}

void MusicCommand_SetVolume(s16 volume)
{
    MusicPlayer_SetVolume(&gMusicPlayerBgm, 0xFF, volume);
    gMusicVolumeTarget = volume;
    gMusicVolume = volume;
}

void Audio_SetMusicVolumeTargetAndStep(u16 primary, u16 secondary)
{
    gMusicVolumeTarget = primary;
    gMusicVolumeStep = secondary;
}

s32 AudioCommand_GetStateByte(void)
{
    return gMusicRestoreDelay;
}

void AudioCommand_StopAllPlayers(void)
{
    Audio_StopAllPlayers();
}

void AudioCommand_ResumeAllPlayers(void)
{
    Audio_ResumeAllPlayers();
}

void AudioCommand_UpdateToggleMask(u32 command)
{
    u32 toggle = command & AUDIO_COMMAND_TOGGLE;

    command &= AUDIO_COMMAND_MASK;
    if (toggle != 0)
        Audio_CommandMask ^= command;
    else
        Audio_CommandMask = command;
}

s32 AudioCommand_GetSecondaryStateByte(void)
{
    return gAudioSecondaryState;
}

void AudioCommand_WaitForCompletion(void)
{
    s32 wait_count = 0;

    do {
        if (gMusicRestoreDelay == 0)
            break;
        WaitFrames(1);
        wait_count++;
    } while (wait_count <= 299);
}

s32 AudioCommand_GetWidth(s32 command)
{
    if (command == 0x46 || command == 0x4b || command == 0x43)
        return 3;
    return 2;
}
