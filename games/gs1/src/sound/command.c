#include "audio_engine_symbols.h"
#include "types.h"
#include "audio_engine.h"

void Audio_Initialize(void);
extern u8 RomBytes_02003000;
extern u8 RomBytes_02003004;
extern u16 gOv3;
extern u16 gOv4;
extern u16 gOv5;
extern u8 gOv6;
extern u16 gOv7;
extern u16 gOv8;
extern u16 gOv9;
extern u16 gOv10;
extern u8 gOv11;
extern u8 Audio_CommandMask;

void Audio_InitializeRuntimeDefaults(void)
{
    s16 *player_volume;
    s32 remaining;

    Audio_Initialize();
    gOv11 = 0xff;
    RomBytes_02003000 = 0;
    gOv9 = 0x100;
    gOv3 = 0x100;
    gOv5 = 4;
    gOv8 = 0x100;
    gOv10 = 0x100;
    gOv4 = 4;
    gOv6 = 0;
    Audio_CommandMask = 0;
    player_volume = &gOv7;
    RomBytes_02003004 = 0;
    remaining = 7;
    do {
        remaining--;
        *player_volume = 0;
        player_volume += 1;
    } while (remaining >= 0);
}

void MusicPlayer_SetPitchAndUpdateFrequency(s32 address, u16 value);

void MusicCommand_SetPitchAndUpdateFrequency(u16 value)
{
    MusicPlayer_SetPitchAndUpdateFrequency(0x02004290, value);
}

void MusicPlayer_SetPitch(u8 *state, u32 mask, u32 value);
void MusicCommand_SetPitch(s16 pitch)
{
  int player_address;
  int channel_mask;
  s16 pitch_value;
  player_address = 0x02004290;
  do
  {
    pitch_value = pitch;
    channel_mask = 0xFF;
    MusicPlayer_SetPitch(player_address, channel_mask, pitch_value);
  }
  while (0);
}

extern u16 Data_02003030;
extern u16 Data_0200300c;

void Audio_SetWorkPairB(u16 primary, u16 secondary)
{
    Data_02003030 = primary;
    Data_0200300c = secondary;
}

void MusicPlayer_SetVolume(struct Work *work, u32 mask, u32 value);
void MusicCommand_SetVolume(s16 volume)
{
  s16 volume_value;
  s16 *volume_cell;
  volume_value = volume;
  MusicPlayer_SetVolume(0x02004290, 0xFF, (u16)volume);
  *((s16 *) 0x02003034) = volume_value;
  /* GCC 2.96 preserves this matched branch shape. */
 store_shared_volume:;
  if (volume_value)
  {
    *(volume_cell = (s16 *) 0x02003008) = volume_value;
  } else
  {
    *(volume_cell = (s16 *) 0x02003008) = volume_value;
  }
}

extern u16 Data_02003034;
extern u16 Data_02003010;

void Audio_SetWorkPairA(u16 primary, u16 secondary)
{
    Data_02003034 = primary;
    Data_02003010 = secondary;
}

void Func_080fa458(void);
void Func_080fa490(void);
s32 WaitFrames(s32);

u8 AudioCommand_GetStateByte(void)
{
    return *(u8 *)0x02003000;
}

void AudioCommand_StopAllPlayers(void)
{
    Func_080fa458();
}

void AudioCommand_ResumeAllPlayers(void)
{
    Func_080fa490();
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
    return *(u8 *)0x0200303c;
}

void AudioCommand_WaitForCompletion(void)
{
    s32 wait_count = 0;

    do {
        if (*(u8 *)0x02003000 == 0)
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
