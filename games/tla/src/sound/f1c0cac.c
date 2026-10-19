#include "audio_engine.h"
#include "audio_engine_symbols.h"

/// The interrupt handler clears unk03 every frame after the sound
/// update, reading it first as only a volatile access survives; while it is
/// set, sound commands are ignored.
struct SoundFrameState {
  u8 unk00[3];
  u8 unk03;
};

extern const struct SongEntry gSound_SongTable[];
extern const struct PlayerSlot gSound_PlayerSlots[];
extern volatile struct SoundFrameState gData_03001138;
extern u8 gData_02005814;
extern u16 gData_02005820[8];
extern u16 gData_02005830;
extern struct SoundPlayer gSound_Player3;
extern struct SoundPlayer gSound_Player5;
extern u16 gData_02005834;
extern u16 gData_0200583c;
extern u16 gData_0200580c;
extern struct SoundPlayer gMusicPlayerFanfare;

void SongFlag_Set(s32 cue);
void MusicPlayer_FadeOut(struct SoundPlayer *player, u16 speed);
void MusicPlayer_StartSong(struct SoundPlayer *player,
                           const struct SequenceHeader *header);
void MusicPlayer_SetVolume(struct SoundPlayer *player, u16 mask, u16 volume);
void Audio_PlaySound(u16 cue);
void Sound_LoadPresetParameters(s32 preset);
void MusicPlayer_SetPitchAndUpdateFrequency(struct SoundPlayer *player,
                                            u16 scale);
void MusicPlayer_SetPitch(struct SoundPlayer *player, u16 mask, s16 pitch);

s32 Func_081c0cac(s32 cue) { return 3; }

/// Plays sound command id: 78 fades the music out, 390 and 395 fade players
/// 3 and 5, 100 to 699 are songs started on their table's player (7 takes
/// the first idle one of 7 down to 4), 80 to 99 play over silenced music,
/// and the rest (79 aside) become the background music.
// As The Broken Seal's, with ⚓️'s ranges.
void AudioCommand_Play(s32 id) {
  s32 flags;
  s32 player;

  SongFlag_Set(id);
  flags = id & 0xf000;
  if (gData_03001138.unk03 != 0)
    return;
  id &= 0xfff;

  if (id == 78) {
    if (gData_02005814 != 0)
      return;
    MusicPlayer_FadeOut(&gMusicPlayerBgm, 7);
    gData_02005814++;
    gData_02005830 = 0;
  } else if (id == 390) {
    gData_02005820[3] = 0;
    MusicPlayer_FadeOut(&gSound_Player3, 3);
    gData_02005820[5] = 0;
    MusicPlayer_FadeOut(&gSound_Player5, 3);
  } else if (id == 395) {
    gData_02005820[5] = 0;
    MusicPlayer_FadeOut(&gSound_Player5, 3);
  } else if (id >= 700) {
    goto background;
  } else if (id > 99) {
    player = gSound_SongTable[id].slot;
    if (player == 7) {
    next:
      if (((u8 *)&gSound_PlayerSlots[player].player->status)[0] != 0) {
        player--;
        if (player > 3)
          goto next;
        player = 7;
      }
    }
    MusicPlayer_StartSong(gSound_PlayerSlots[player].player,
                          gSound_SongTable[id].header);
    gData_02005820[player] = id;
  } else if (id > 79) {
    MusicPlayer_SetVolume(&gMusicPlayerBgm, 255, 0);
    gMusicVolumeTarget = 0;
    gMusicVolume = 0;
    Audio_PlaySound(id);
    gMusicRestoreDelay = 10;
  } else {
    if (id == 79)
      return;
  background:
    if (id == gData_02005830)
      return;
    gData_02005830 = id;
    Sound_LoadPresetParameters(Func_081c0cac(id));
    Audio_PlaySound(id);
    if (flags & 0x1000)
      gMusicVolume = 0;
    else
      gMusicVolume = 0x100;
    gMusicVolumeTarget = 0x100;
    gMusicVolumeStep = 4;
    gData_02005814 = 0;
  }
}

/// Steps the music volume toward its target and the pitch toward its target,
/// each by its step, after the jingle's restore delay runs out, as The Broken
/// Seal's without the DMA restart tick.
void MusicPlayer_StepVolumeAndPitchTowardTargets(void) {
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
    if (delta > 0)
      gMusicVolume += gMusicVolumeStep;
    else
      gMusicVolume -= gMusicVolumeStep;
    if ((((s16)gMusicVolumeTarget - (s16)gMusicVolume) ^ delta) < 0)
      gMusicVolume = gMusicVolumeTarget;
    MusicPlayer_SetVolume(&gMusicPlayerBgm, 255, gMusicVolume);
  }
  if ((s16)gData_02005834 != (s16)gData_0200583c) {
    delta = (s16)gData_02005834 - (s16)gData_0200583c;
    if (delta > 0)
      gData_0200583c += gData_0200580c;
    else
      gData_0200583c -= gData_0200580c;
    if ((((s16)gData_02005834 - (s16)gData_0200583c) ^ delta) < 0)
      gData_0200583c = gData_02005834;
    MusicPlayer_SetPitchAndUpdateFrequency(&gMusicPlayerBgm, gData_0200583c);
    MusicPlayer_SetPitch(&gMusicPlayerBgm, 255,
                         (s16)gData_0200583c * 12 - 3072);
  }
}
