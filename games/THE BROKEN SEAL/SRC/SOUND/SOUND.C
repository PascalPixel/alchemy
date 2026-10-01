/* Sound test: three cue numbers, one per row (a background track, a jingle
   and a song). Up and down pick the row; right and left step its cue by one
   and R and L by ten; A plays it, B plays cue 19 and START fades the music
   out. SELECT cycles the mixer preset and fades the second player. A clip in
   the mixer restarts the twenty-frame indicator timer. Never returns. */
#include "TYPES.H"
#include "SYSTEM.H"
#include "AUDIO_ENGINE_SYMBOLS.H"
#include "AUDIO_ENGINE.H"

extern volatile u32 gKeysRepeat;
extern s32 gSoundClipFlag;
extern s32 Data_03007804;
s32 Sound_LoadPresetParameters(s32 preset);
void AudioCommand_Play(s32 id);

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
void MusicPlayer_SetPitchAndUpdateFrequency(s32 address, u16 value);
void MusicPlayer_SetPitch(u8 *state, u32 mask, u32 value);
void MusicPlayer_SetVolume(struct Work *work, u32 mask, u32 value);
void Audio_StopAllPlayers(void);
void Audio_ResumeAllPlayers(void);

void AudioTest_RunParameterController(void)
{
    s32 cues[3] = {0, 80, 100};
    s32 preset;
    s32 row;
    s32 timer;

    row = 0;
    preset = 2;
    /* FAKEMATCH: removing this one-pass block changes instruction scheduling. */
    do {
        Data_03007804 = 0;
    } while (0);
    timer = 20;

    for (;;) {
        if (timer != 0)
            timer--;

        if (gSoundClipFlag != 0) {
            gSoundClipFlag = 0;
            timer = 20;
        }

        if (gKeysRepeat & 4) {
            preset = (preset + 1) % 5;
            Sound_LoadPresetParameters(preset);
        }
        if (gKeysRepeat & 0x100)
            cues[row] += 10;
        if (gKeysRepeat & 0x200)
            cues[row] -= 10;
        if (gKeysRepeat & 0x10)
            cues[row] += 1;
        if (gKeysRepeat & 0x20)
            cues[row] -= 1;
        if (gKeysRepeat & 0x40) {
            if (row > 0)
                row--;
        }
        if (gKeysRepeat & 0x80) {
            if (row <= 1)
                row++;
        }
        if (gKeysRepeat & 1)
            AudioCommand_Play(cues[row]);
        if (gKeysRepeat & 2)
            AudioCommand_Play(19);
        if (gKeysRepeat & 8)
            AudioCommand_Play(17);
        if (gKeysRepeat & 4)
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
    RomBytes_02003000 = 0;
    gMusicVolumeTarget = 0x100;
    gMusicVolume = 0x100;
    gMusicVolumeStep = 4;
    gMusicPitchTarget = 0x100;
    gMusicPitch = 0x100;
    gMusicPitchStep = 4;
    Data_02003014 = 0;
    Audio_CommandMask = 0;
    player_volume = &gMusicPlayerVolumes;
    RomBytes_02003004 = 0;
    remaining = 7;
    do {
        remaining--;
        *player_volume = 0;
        player_volume += 1;
    } while (remaining >= 0);
}

void MusicCommand_SetPitchAndUpdateFrequency(u16 value)
{
    MusicPlayer_SetPitchAndUpdateFrequency((u32)gMusicPlayerBgm, value);
}

void MusicCommand_SetPitch(s16 pitch)
{
    /* FAKEMATCH: the player pointer temporary preserves measured argument allocation or instruction order. */
  int player_address;
  player_address = (u32)gMusicPlayerBgm;
  /* FAKEMATCH: removing this one-pass block changes instruction scheduling. */
  do
  {
    MusicPlayer_SetPitch(player_address, 0xFF, pitch);
  }
  while (0);
}

void Audio_SetWorkPairB(u16 primary, u16 secondary)
{
    gMusicPitchTarget = primary;
    gMusicPitchStep = secondary;
}

void MusicCommand_SetVolume(s16 volume)
{
    /* FAKEMATCH: the duplicated volume branch preserves measured argument allocation or instruction order. */
  s16 volume_value;
  volume_value = volume;
  MusicPlayer_SetVolume((u32)gMusicPlayerBgm, 0xFF, (u16)volume);
  *((s16 *) &gMusicVolumeTarget) = volume_value;
  /* GCC 2.96 preserves this matched branch shape. */
 store_shared_volume:;
  if (volume_value)
  {
    *(s16 *) &gMusicVolume = volume_value;
  } else
  {
    *(s16 *) &gMusicVolume = volume_value;
  }
}

void Audio_SetWorkPairA(u16 primary, u16 secondary)
{
    gMusicVolumeTarget = primary;
    gMusicVolumeStep = secondary;
}

u8 AudioCommand_GetStateByte(void)
{
    return *(u8 *)gMusicRestoreDelay;
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
    u32 toggle = command & 0x80;

    command &= 0x7f;
    if (toggle != 0)
        Audio_CommandMask ^= command;
    else
        Audio_CommandMask = command;
}

u8 AudioCommand_GetSecondaryStateByte(void)
{
    return *(u8 *)&gAudioSecondaryState;
}

void AudioCommand_WaitForCompletion(void)
{
    s32 wait_count = 0;

    do {
        if (*(u8 *)gMusicRestoreDelay == 0)
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
