#include "types.h"

s32 Func_080030f8(s32);
void Func_0800387c(u32 first, u32 second);
void Func_080b5038(s32, u16, s32);
void Func_080cdd14(void)
{
  u8 *state;
  s32 one;
  s32 transfer;
  s32 *flag;

  flag = (s32 *)((u8 *)*((void **)0x03001F00) + 0xC);
  state = *((u8 **)0x03001E74);
  *flag = 1;
  transfer = 0x1541;
  Func_0800387c(0x04000000, transfer);
  one = 1;
  Func_080030f8(one);
  Func_080b5038(2, *((u16 *)(state + 0x648)), 0);
  transfer = one;
  do
  {
    Func_080030f8(transfer);
  }
  while (0);
}
