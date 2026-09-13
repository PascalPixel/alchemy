#include "types.h"
#include "gs1_edition.h"

#if defined(GS1_EDITION_DE)
#define TRANSITION_CELL_ADDR 0x03001F10
#else
#define TRANSITION_CELL_ADDR 0x03001F00
#endif

s32 WaitFrames(s32);
void Func_0800387c(u32 first, u32 second);
void Func_080b5038(s32, u16, s32);
void BattleEffect_SetTransitionFlagAndDisplay(void)
{
  u8 *state;
  s32 one;
  s32 transfer;
  s32 *flag;

  flag = (s32 *)((u8 *)*((void **)TRANSITION_CELL_ADDR) + 0xC);
  state = *((u8 **)(TRANSITION_CELL_ADDR - 0x8C));
  *flag = 1;
  transfer = 0x1541;
  Func_0800387c(0x04000000, transfer);
  one = 1;
  WaitFrames(one);
  Func_080b5038(2, *((u16 *)(state + 0x648)), 0);
  transfer = one;
  do
  {
    WaitFrames(transfer);
  }
  while (0);
}
