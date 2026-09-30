#include "TYPES.H"
#include "RAM_BUFFER.H"

s32 WaitFrames(s32);
void QueueIoWriteDelay2(u32 first, u32 second);
void BattlePresentation_ConfigurePaletteFadeFar(s32, u16, s32);
void BattleFx_SetTransitionFlagAndDisplay(void)
{
  u8 *state;
  s32 one;
  s32 transfer;
  s32 *flag;

  flag = (s32 *)((u8 *)Ram_WorkSlot[44] + 0xC);
  state = (u8 *)Ram_WorkSlot[9];
  *flag = 1;
  transfer = 0x1541;
  QueueIoWriteDelay2(0x04000000, transfer);
  one = 1;
  WaitFrames(one);
  BattlePresentation_ConfigurePaletteFadeFar(2, *((u16 *)(state + 0x648)), 0);
  transfer = one;
  do
  {
    WaitFrames(transfer);
  }
  while (0);
}
