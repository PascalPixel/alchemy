#include "types.h"

s32 WaitFrames(s32);
void Func_080b5078(s32, s16, s32, s32);
s32 Func_080d4604(void *, s32);
void BattleEffect_WaitThenSetField18To4(void *arg0)
{
  unsigned int ofs;
  Func_080b5078(*((s32 *) (arg0 + 8)), *((s16 *) (((u8 *) arg0) + 0x24)), 0x18, 0xC3333);
  WaitFrames(0x1D);
  ofs = 0x18;
  *((s32 *) (((u8 *) arg0) + ofs)) = 4;
  Func_080d4604(arg0, 2);
}
