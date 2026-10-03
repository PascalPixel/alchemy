#include "AUDIO_ENGINE.H"

extern u16 gMusicVolume;
extern u16 gMusicVolumeTarget;

/* Sets the volume of every track of the music player and records it as
   both the current and the target music volume. */
void MusicCommand_SetVolume(s16 volume)
{
  s16 volume_value;
  s16 *volume_cell;
  volume_value = volume;
  MusicPlayer_SetVolume(&gMusicPlayerBgm, 0xFF, (u16)volume);
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
