#include "audio_engine_symbols.h"
#include "types.h"
#include "scene.h"
#include "audio_engine.h"

extern u8 RomBytes_0200300c[];
extern u8 RomBytes_02003010[];
extern u8 RomBytes_02003030[];
extern u8 RomBytes_02003034[];
extern u8 RomBytes_02003040[];

/* audio/init/initialize_runtime_defaults.c */
void Audio_Initialize(void);
extern u8 RomBytes_02003000[];
extern u16 RomBytes_02003004;
extern u16 gOv3;
extern u16 gOv4;
extern u16 gOv5;
extern u8 gOv6;
extern u16 gOv7;
extern u16 gOv8;
extern u16 gOv9;
extern u16 gOv10;
extern u8 gOv11;
extern u8 gOv12;

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
    gOv12 = 0;
    player_volume = &gOv7;
    RomBytes_02003004 = 0;
    remaining = 7;
    do {
        remaining--;
        *player_volume = 0;
        player_volume += 1;
    } while (remaining >= 0);
}

/* audio/command/set_pitch_and_update_frequency.c */
void MusicPlayer_SetPitchAndUpdateFrequency(s32 address, u16 value);

void MusicCommand_SetPitchAndUpdateFrequency(u16 value)
{
    MusicPlayer_SetPitchAndUpdateFrequency(0x02004290, value);
}

/* audio/command/set_pitch.c */
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

/* audio/mix/set_work_pair_b.c */

void Audio_SetWorkPairB(u16 primary, u16 secondary)
{
    RomBytes_02003030 = primary;
    RomBytes_0200300c = secondary;
}

/* audio/command/set_volume.c */
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

/* audio/mix/set_work_pair_a.c */

void Audio_SetWorkPairA(u16 primary, u16 secondary)
{
    RomBytes_02003034 = primary;
    RomBytes_02003010 = secondary;
}

/* audio/command/get_state_byte.c */
u8 AudioCommand_GetStateByte(void)
{
    /* Current sound command/state byte. */
    return *(u8 *)0x02003000;
}

/* audio/command/stop_all_players.c */
void AudioCommand_StopAllPlayers(void)
{
    FunctionHead_080fa458();
}

/* audio/command/resume_all_players.c */
void AudioCommand_ResumeAllPlayers(void)
{
    FunctionHead_080fa490();
}

/* audio/command/update_toggle_mask.c */

void AudioCommand_UpdateToggleMask(u32 command)
{
    u32 toggle = command & 0x80;

    command &= 0x7f;
    if (toggle != 0)
        RomBytes_02003040[0] ^= command;
    else
        RomBytes_02003040[0] = command;
}

/* audio/command/get_secondary_state_byte.c */
u8 AudioCommand_GetSecondaryStateByte(void)
{
    /* Current secondary sound status byte. */
    return *(u8 *)0x0200303c;
}

/* audio/command/wait_for_completion.c */
void WaitFrames(s32);

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

/* audio/command/get_command_width.c */
s32 AudioCommand_GetWidth(s32 command)
{
    if (command == 0x46 || command == 0x4b || command == 0x43)
        return 3;
    return 2;
}

/* audio/cgb/unlink_channel.c */
void CgbChannel_Unlink(struct CgbChannel *channel)
{
    struct MusicTrackState *track = channel->track;

    if (track == 0) {
        return;
    }

    {
        struct CgbChannel *next = channel->next_channel;
        struct CgbChannel *previous = channel->previous_channel;

        if (previous != 0) {
            previous->next_channel = next;
        } else {
            track->channel = next;
        }
        if (next != 0) {
            next->previous_channel = previous;
        }

        /* Preserve the compiler's r1 null carrier after its final list use. */
        next = 0;
        channel->track = (struct MusicTrackState *)next;
    }
}
