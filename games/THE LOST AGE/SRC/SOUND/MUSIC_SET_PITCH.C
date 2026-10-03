#include "AUDIO_ENGINE.H"

/* Sets the pitch of every track of the music player. */
void MusicCommand_SetPitch(s16 pitch)
{
  int player_address;
  int channel_mask;
  s16 pitch_value;
  player_address = (u32)&gMusicPlayerBgm;
  do
  {
    pitch_value = pitch;
    channel_mask = 0xFF;
    MusicPlayer_SetPitch((struct SoundPlayer *)player_address, channel_mask, pitch_value);
  }
  while (0);
}
