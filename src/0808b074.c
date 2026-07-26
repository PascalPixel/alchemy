#include "types.h"

typedef int bool;
s32 Func_0808b05c(u16, u16);
unsigned char Func_0808b074(s32 arg0)
{
  s32 temp_r0;
  int new_var2;
  int new_var5;
  u16 new_var;
  u8 *new_var4;
  u8 *new_var3;
  temp_r0 = arg0 * 4;
  new_var4 = ((u8 *) temp_r0) + 0x0809D8B0;
  new_var5 = temp_r0 + 0x0809D8B0;
  new_var3 = new_var4;
  new_var = *((u16 *) new_var3);
  new_var2 = new_var5;
  Func_0808b05c(new_var, *((u16 *) (((u8 *) new_var2) + 2)));
}

