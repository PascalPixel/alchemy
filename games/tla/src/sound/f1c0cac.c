#include "audio_engine.h"
#include "audio_engine_symbols.h"

// The interrupt handler clears unknown_03 every frame after the sound
// update, reading it first as only a volatile access survives; while it is
// set, sound commands are ignored.
struct SoundFrameState
{
    u8 unknown_00[3];
    u8 unknown_03;
};

extern const struct SongEntry Sound_SongTable[];
extern const struct PlayerSlot Sound_PlayerSlots[];
extern volatile struct SoundFrameState Data_03001138;
extern u8 Data_02005814;
extern u16 Data_02005820[8];
extern u16 Data_02005830;
extern struct SoundPlayer Data_02006b60;
extern struct SoundPlayer Data_02006990;

void SongFlag_Set(s32 cue);
void MusicPlayer_FadeOut(struct SoundPlayer *player, u16 speed);
void MusicPlayer_StartSong(struct SoundPlayer *player, const struct SequenceHeader *header);
void MusicPlayer_SetVolume(struct SoundPlayer *player, u16 mask, u16 volume);
void Audio_PlaySound(u16 cue);
void Sound_LoadPresetParameters(s32 preset);

s32 Func_081c0cac(s32 cue)
{
    return 3;
}

// Plays sound command id: 78 fades the music out, 390 and 395 fade players
// 3 and 5, 100 to 699 are songs started on their table's player (7 takes
// the first idle one of 7 down to 4), 80 to 99 play over silenced music,
// and the rest (79 aside) become the background music, as The Broken Seal's
// with ⚓️'s ranges.
void AudioCommand_Play(s32 id)
{
    s32 flags;
    s32 player;

    SongFlag_Set(id);
    flags = id & 0xf000;
    if (Data_03001138.unknown_03 != 0)
        return;
    id &= 0xfff;

    if (id == 78)
    {
        if (Data_02005814 != 0)
            return;
        MusicPlayer_FadeOut(&gMusicPlayerBgm, 7);
        Data_02005814++;
        Data_02005830 = 0;
    }
    else if (id == 390)
    {
        Data_02005820[3] = 0;
        MusicPlayer_FadeOut(&Data_02006b60, 3);
        Data_02005820[5] = 0;
        MusicPlayer_FadeOut(&Data_02006990, 3);
    }
    else if (id == 395)
    {
        Data_02005820[5] = 0;
        MusicPlayer_FadeOut(&Data_02006990, 3);
    }
    else if (id >= 700)
    {
        goto background;
    }
    else if (id > 99)
    {
        player = Sound_SongTable[id].slot;
        if (player == 7)
        {
        next:
            if (((u8 *)&Sound_PlayerSlots[player].player->status)[0] != 0)
            {
                player--;
                if (player > 3)
                    goto next;
                player = 7;
            }
        }
        MusicPlayer_StartSong(Sound_PlayerSlots[player].player, Sound_SongTable[id].header);
        Data_02005820[player] = id;
    }
    else if (id > 79)
    {
        MusicPlayer_SetVolume(&gMusicPlayerBgm, 255, 0);
        gMusicVolumeTarget = 0;
        gMusicVolume = 0;
        Audio_PlaySound(id);
        gMusicRestoreDelay = 10;
    }
    else
    {
        if (id == 79)
            return;
    background:
        if (id == Data_02005830)
            return;
        Data_02005830 = id;
        Sound_LoadPresetParameters(Func_081c0cac(id));
        Audio_PlaySound(id);
        if (flags & 0x1000)
            gMusicVolume = 0;
        else
            gMusicVolume = 0x100;
        gMusicVolumeTarget = 0x100;
        gMusicVolumeStep = 4;
        Data_02005814 = 0;
    }
}
