/* Sound test: three cue numbers, one per row (a background track, a jingle
   and a song). Up and down pick the row; right and left step its cue by one
   and R and L by ten; A plays it, B plays cue 19 and START fades the music
   out. SELECT cycles the mixer preset and fades the second player. A clip in
   the mixer restarts the twenty-frame indicator timer. Never returns. */
#include "TYPES.H"
#include "IO_REG.H"
#include "SYSTEM.H"
#include "AUDIO_ENGINE_SYMBOLS.H"
#include "AUDIO_ENGINE.H"

extern volatile u32 gKeysRepeat;
extern s32 gSoundClipFlag;
extern s32 Data_03007804;
void AudioCommand_Play(s32 id);

void Audio_Initialize(void);
extern u8 RomBytes_02003000;
extern u8 RomBytes_02003004;
extern u8 Data_02003014;
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
    /* FAKEMATCH: removing this one-pass block changes instruction scheduling.
       2026-10-02: a plain assignment still moves the state address load ahead
       of the preset setup and the state store after the timer setup. */
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
    RomBytes_02003000 = 0;
    gMusicVolumeTarget = 0x100;
    gMusicVolume = 0x100;
    gMusicVolumeStep = 4;
    gMusicPitchTarget = 0x100;
    gMusicPitch = 0x100;
    gMusicPitchStep = 4;
    Data_02003014 = 0;
    Audio_CommandMask = 0;
    player_volume = (s16 *)gMusicPlayerVolumes;
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
    MusicPlayer_SetPitchAndUpdateFrequency(&gMusicPlayerBgm, value);
}

void MusicCommand_SetPitch(s16 pitch)
{
    /* FAKEMATCH: the player pointer temporary preserves measured argument allocation or instruction order.
       2026-10-02: replacing this temporary and one-pass block with a direct
       call moves pitch sign extension before the player address load. */
  int player_address;
  player_address = (u32)&gMusicPlayerBgm;
  /* FAKEMATCH: removing this one-pass block changes instruction scheduling. */
  do
  {
    MusicPlayer_SetPitch((struct SoundPlayer *)player_address, 0xFF, pitch);
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
    MusicPlayer_SetVolume(&gMusicPlayerBgm, 0xFF, (u16)volume);
    gMusicVolumeTarget = volume;
    gMusicVolume = volume;
}

void Audio_SetWorkPairA(u16 primary, u16 secondary)
{
    gMusicVolumeTarget = primary;
    gMusicVolumeStep = secondary;
}

u8 AudioCommand_GetStateByte(void)
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

u8 AudioCommand_GetSecondaryStateByte(void)
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
