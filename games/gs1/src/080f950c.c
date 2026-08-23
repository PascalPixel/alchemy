#include "types.h"

void MusicPlayer_SetVolume(struct Work *arg0, u32 mask, u32 value);
void Func_080f950c(s16 arg0)
{
  s16 temp_r5;
  s16 *new_var;
  temp_r5 = arg0;
  MusicPlayer_SetVolume(0x02004290, 0xFF, (u16) arg0);
  *((s16 *) 0x02003034) = temp_r5;
 dummy_label_303982: ;
  if (temp_r5)
  {
    *(new_var = (s16 *) 0x02003008) = temp_r5;
  }
  else
  {
    *(new_var = (s16 *) 0x02003008) = temp_r5;
  }
}
