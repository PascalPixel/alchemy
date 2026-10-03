/* Sound: play one sound command. The low twelve bits pick the cue: 17 fades
   the background music out once, 0x121 fades the second player, cues above
   99 start a song on its player (player 7 falls back to the first idle one
   of 7..4), 80..99 play a jingle at full-silence background volume, and the
   rest start a background track, restoring its volume unless bit 12 asks
   for a silent start. */
#include "AUDIO_ENGINE.H"

extern u8 Data_02003014;

void Audio_PlaySound(u16 id);

void AudioCommand_Play(s32 id)
{
    s32 flags;
    s32 player;

    flags = id & AUDIO_CUE_FLAGS_MASK;
    id &= AUDIO_CUE_ID_MASK;

    if (id == AUDIO_CUE_FADE_BGM) {
        if (Data_02003014 != 0)
            return;
        MusicPlayer_FadeOut(&gMusicPlayerBgm, 7);
        Data_02003014++;
        gAudioSecondaryState = 19;
    } else if (id == AUDIO_CUE_FADE_SLOT3) {
        gMusicPlayerVolumes[3] = 0;
        MusicPlayer_FadeOut(&Data_02004360, 3);
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
        Data_02003014 = 0;
    }
}
