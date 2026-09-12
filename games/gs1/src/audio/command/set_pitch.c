#include "types.h"

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
