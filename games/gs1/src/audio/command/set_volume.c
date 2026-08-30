#include "types.h"

void MusicPlayer_SetVolume(struct Work *arg0, u32 mask, u32 value);
void MusicCommand_SetVolume(s16 volume)
{
  s16 volume_value;
  s16 *volume_cell;
  volume_value = volume;
  MusicPlayer_SetVolume(0x02004290, 0xFF, (u16) volume);
  *((s16 *) 0x02003034) = volume_value;
  /* GCC 2.96 preserves this matched branch shape. */
 store_shared_volume: ;
  if (volume_value)
  {
    *(volume_cell = (s16 *) 0x02003008) = volume_value;
  }
  else
  {
    *(volume_cell = (s16 *) 0x02003008) = volume_value;
  }
}
