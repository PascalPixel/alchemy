#include "types.h"

u32 Func_080c2368(s32 arg0)
{
  u32 var_r0;
  u8 *new_var;
  new_var = (u8 *) ((arg0 * 8) + 0x080C7420);
  var_r0 = ((u8) (*((u8 *) (new_var + 3)))) >> 5;
  if (((s32) var_r0) > 4)
  {
    var_r0 = -1U;
  }
  return var_r0;
}

