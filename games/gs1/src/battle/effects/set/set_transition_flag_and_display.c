#include "types.h"
#include "scene.h"
#include "abi/battle/effects/set/set_transition_flag_and_display.h"
#include "gs1_edition.h"

#if defined(GS1_EDITION_DE)
#define TRANSITION_CELL_ADDR 0x03001F10
#else
#define TRANSITION_CELL_ADDR 0x03001F00
#endif

s32 WaitFrames(s32);

void BattleFx_SetTransitionFlagAndDisplay(void)
{
  u8 *state;
  s32 one;
  s32 transfer;
  s32 *flag;

  flag = (s32 *)((u8 *)*((void **)TRANSITION_CELL_ADDR) + 0xC);
  state = *((u8 **)(TRANSITION_CELL_ADDR - 0x8C));
  *flag = 1;
  transfer = 0x1541;
  Battle_Apply(0x04000000, transfer);
  one = 1;
  WaitFrames(one);
  Battle_Place(2, *((u16 *)(state + 0x648)), 0);
  transfer = one;
  do
  {
    WaitFrames(transfer);
  }
  while (0);
}
