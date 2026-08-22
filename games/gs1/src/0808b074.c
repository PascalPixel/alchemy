#include "types.h"

u16 Func_0808b05c(s32 arg0, s32 arg1);
extern u8 Data_0809d8b0[];
unsigned char Func_0808b074(s32 arg0)
{
  s32 temp_r0;
  int new_var2;
  int new_var5;
  u16 new_var;
  u8 *new_var4;
  u8 *new_var3;
  temp_r0 = arg0 * 4;
  new_var4 = ((u8 *) temp_r0) + (s32)Data_0809d8b0;
  new_var5 = temp_r0 + (s32)Data_0809d8b0;
  new_var3 = new_var4;
  new_var = *((u16 *) new_var3);
  new_var2 = new_var5;
  Func_0808b05c(new_var, *((u16 *) (((u8 *) new_var2) + 2)));
}
