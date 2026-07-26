#include "types.h"

typedef int bool;
u32 Func_0808f304(void)
{
  s16 temp_r3_2;
  u32 var_r0;
  void *temp_r3;
  temp_r3 = *((void **) 0x03001EBC);
  var_r0 = 0;
  if (temp_r3 != ((void *) 0))
  {
    temp_r3_2 = *((s16 *) (((u8 *) temp_r3) + 0xCB8));
    var_r0 = (u32) ((0 - temp_r3_2) | temp_r3_2);
    var_r0 = var_r0 >> 0x1F;
  }
  return var_r0;
}

