#include "audio_engine.h"

struct AudioFrameState {
    u8 unknown_00[3];
    u8 busy;
};

extern const struct SongEntry Sound_SongTable[];
extern const struct PlayerSlot Sound_PlayerSlots[];
extern struct AudioFrameState Data_03001138;
extern u8 Data_02005814;
extern u16 Data_02005820[8];
extern u16 Data_02005830;
extern u16 gMusicVolumeStep;
extern u16 gMusicVolume;
extern u16 gMusicVolumeTarget;
extern u8 gMusicRestoreDelay;
extern struct SoundPlayer gMusicPlayerBgm;
extern struct SoundPlayer Data_02006b60;
extern struct SoundPlayer Data_02006990;

void SongFlag_Set(s32 cue);
void MusicPlayer_FadeOut(struct SoundPlayer *player, u16 speed);
void MusicPlayer_StartSong(struct SoundPlayer *player,
    const struct SequenceHeader *header);
void MusicPlayer_SetVolume(struct SoundPlayer *player, u16 mask, u16 volume);
void Audio_PlaySound(u16 cue);
void Sound_LoadPresetParameters(s32 preset);

s32 AudioCommand_GetDefaultPreset(s32 cue)
{
    return 3;
}

void AudioCommand_Play(s32 cue)
{
    s32 flags;
    s32 slot;
    u32 busy;

    SongFlag_Set(cue);
    flags = cue & 0xf000;
    {
        /* FAKEMATCH: eight ordinary type/lifetime forms load the busy byte directly into r7; the native sequence first captures r3, then hands it to a normal local before any call. */
        register u32 read __asm__("r3");
        read = Data_03001138.busy;
        /* FAKEMATCH: this empty boundary preserves the measured r3 capture and handoff; it changes no value and ends before the branch's calls. */
        __asm__ volatile("" : "=r"(busy) : "0"(read));
    }
    if (busy != 0)
        return;
    cue &= 0xfff;

    if (cue == 78) {
        if (Data_02005814 != 0)
            return;
        MusicPlayer_FadeOut(&gMusicPlayerBgm, 7);
        Data_02005814++;
        Data_02005830 = busy;
    } else if (cue == 390) {
        Data_02005820[3] = busy;
        MusicPlayer_FadeOut(&Data_02006b60, 3);
        Data_02005820[5] = busy;
        MusicPlayer_FadeOut(&Data_02006990, 3);
    } else if (cue == 395) {
        Data_02005820[5] = busy;
        MusicPlayer_FadeOut(&Data_02006990, 3);
    } else {
        if (cue >= 700)
            goto background;
        if (cue > 99) {
            slot = Sound_SongTable[cue].slot;
            if (slot == 7) {
            next:
                if (((u8 *)&Sound_PlayerSlots[slot].player->status)[0] != 0) {
                    slot--;
                    if (slot > 3)
                        goto next;
                    slot = 7;
                }
            }
            MusicPlayer_StartSong(Sound_PlayerSlots[slot].player,
                Sound_SongTable[cue].header);
            Data_02005820[slot] = cue;
        } else if (cue > 79) {
            MusicPlayer_SetVolume(&gMusicPlayerBgm, 255, 0);
            gMusicVolumeTarget = busy;
            gMusicVolume = busy;
            Audio_PlaySound(cue);
            gMusicRestoreDelay = 10;
        } else {
            if (cue == 79)
                return;
        background:
            if (cue == Data_02005830)
                return;
            Data_02005830 = cue;
            Sound_LoadPresetParameters(AudioCommand_GetDefaultPreset(cue));
            Audio_PlaySound(cue);
            if (flags & 0x1000)
                *(s16 *)&gMusicVolume = 0;
            else
                *(s16 *)&gMusicVolume = 0x100;
            *(s16 *)&gMusicVolumeTarget = 0x100;
            *(s16 *)&gMusicVolumeStep = 4;
            Data_02005814 = 0;
        }
    }
}
