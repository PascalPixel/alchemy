#include "types.h"

void Func_0801ce6c(void *arg0)
{
  unsigned int new_var;
  unsigned long temp_r3;
  temp_r3 = 1 + (*((u16 *) (0x574 + ((u8 *) arg0))));
  new_var = 0U;
  *((u16 *) (((u8 *) arg0) + 0x574)) = temp_r3;
  if (((u32) (temp_r3 << 0x10)) >= (((unsigned long) 0x20000U) + 1))
  {
    *((u16 *) (((u8 *) arg0) + 0x574)) = new_var;
  }
}

