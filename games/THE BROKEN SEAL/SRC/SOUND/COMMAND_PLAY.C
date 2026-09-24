/* Sound: play one sound command. The low twelve bits pick the cue: 17 fades
   the background music out once, 0x121 fades the second player, cues above
   99 start a song on its player (player 7 falls back to the first idle one
   of 7..4), 80..99 play a jingle at full-silence background volume, and the
   rest start a background track, restoring its volume unless bit 12 asks
   for a silent start. */
#include "TYPES.H"

struct SongEntry {
    u8 *header;
    u16 player;
    u16 unknown_06;
};

struct MusicPlayerEntry {
    u8 *info;
    u8 *tracks;
    u8 track_count;
    u8 unknown_09[3];
};

extern struct SongEntry Data_080fc684[];
extern struct MusicPlayerEntry Data_080fc624[];
extern u8 gMusicRestoreDelay;
extern u16 gMusicVolume;
extern u16 gMusicVolumeStep;
extern u16 gMusicVolumeTarget;
extern u16 gMusicPlayerVolumes[];
extern u8 Data_02003014;
extern u8 gAudioSecondaryState;
extern u8 gMusicPlayerBgm[];
extern u8 Data_02004360[];

void MusicPlayer_FadeOut(u8 *player, s32 speed);
void MusicPlayer_StartSong(u8 *player, u8 *header);
void MusicPlayer_SetVolume(u8 *player, s32 mask, s32 volume);
void Audio_PlaySound(u16 id);
void Sound_LoadPresetParameters(s32 preset);

void AudioCommand_Play(s32 id)
{
    s32 flags;
    s32 player;

    flags = id & 0xf000;
    id &= 0xfff;

    if (id == 17) {
        if (Data_02003014 != 0)
            return;
        MusicPlayer_FadeOut(gMusicPlayerBgm, 7);
        Data_02003014++;
        gAudioSecondaryState = 19;
    } else if (id == 0x121) {
        gMusicPlayerVolumes[3] = 0;
        MusicPlayer_FadeOut(Data_02004360, 3);
    } else if (id > 99) {
        player = Data_080fc684[id].player;
        if (player == 7) {
        next:
            if (Data_080fc624[player].info[4] != 0) {
                player--;
                if (player > 3)
                    goto next;
                player = 7;
            }
        }
        MusicPlayer_StartSong(Data_080fc624[player].info, Data_080fc684[id].header);
        gMusicPlayerVolumes[player] = id;
    } else if (id > 79) {
        MusicPlayer_SetVolume(gMusicPlayerBgm, 255, 0);
        gMusicVolumeTarget = 0;
        gMusicVolume = 0;
        Audio_PlaySound(id);
        gMusicRestoreDelay = 10;
    } else {
        if (id == 18)
            return;
        if (id == gAudioSecondaryState)
            return;
        gAudioSecondaryState = id;
        Sound_LoadPresetParameters((id == 70 || id == 75 || id == 67) ? 3 : 2);
        Audio_PlaySound(id);
        if (flags & 0x1000)
            gMusicVolume = 0;
        else
            gMusicVolume = 0x100;
        gMusicVolumeTarget = 0x100;
        gMusicVolumeStep = 4;
        Data_02003014 = 0;
    }
}
