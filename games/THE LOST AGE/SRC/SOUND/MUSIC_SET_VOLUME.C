#include "AUDIO_ENGINE_SYMBOLS.H"
#include "TYPES.H"
#include "AUDIO_ENGINE.H"
extern u8 gMusicPlayerBgm[];
extern u8 gMusicRestoreDelay[];

void Audio_Initialize(void);
extern u8 RomBytes_02003000;
extern u8 RomBytes_02003004;
extern u16 gMusicVolume;
extern u16 gMusicPitchStep;
extern u16 gMusicVolumeStep;
extern u8 Data_02003014;
extern u16 gMusicPlayerVolumes;
extern u16 gMusicPitchTarget;
extern u16 gMusicVolumeTarget;
extern u16 gMusicPitch;
extern u8 gAudioSecondaryState;
extern u8 Audio_CommandMask;

void Audio_InitializeRuntimeDefaults(void);

void MusicPlayer_SetPitchAndUpdateFrequency(s32 address, u16 value);

void MusicCommand_SetPitchAndUpdateFrequency(u16 value);

void MusicPlayer_SetPitch(u8 *state, u32 mask, u32 value);

void MusicCommand_SetPitch(s16 pitch);

extern u16 gMusicPitchTarget;
extern u16 gMusicPitchStep;

void Audio_SetWorkPairB(u16 primary, u16 secondary);

void MusicPlayer_SetVolume(struct Work *work, u32 mask, u32 value);
void MusicCommand_SetVolume(s16 volume)
{
  s16 volume_value;
  s16 *volume_cell;
  volume_value = volume;
  MusicPlayer_SetVolume((u32)gMusicPlayerBgm, 0xFF, (u16)volume);
  *((s16 *) &gMusicVolumeTarget) = volume_value;
  /* GCC 2.96 preserves this matched branch shape. */
 store_shared_volume:;
  if (volume_value)
  {
    *(volume_cell = (s16 *) &gMusicVolume) = volume_value;
  } else
  {
    *(volume_cell = (s16 *) &gMusicVolume) = volume_value;
  }
}

extern u16 gMusicVolumeTarget;
extern u16 gMusicVolumeStep;

void Audio_SetWorkPairA(u16 primary, u16 secondary);

void Audio_StopAllPlayers(void);
void Audio_ResumeAllPlayers(void);
s32 WaitFrames(s32);

u8 AudioCommand_GetStateByte(void);

void AudioCommand_StopAllPlayers(void);

void AudioCommand_ResumeAllPlayers(void);

void AudioCommand_UpdateToggleMask(u32 command);

u8 AudioCommand_GetSecondaryStateByte(void);

void AudioCommand_WaitForCompletion(void);

s32 AudioCommand_GetWidth(s32 command);
