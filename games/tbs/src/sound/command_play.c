#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "audio_engine.h"



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
