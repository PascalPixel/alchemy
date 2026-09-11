#include "types.h"
#include "scene.h"

s32 WaitFrames(s32);

void BattleFx_WaitThenSetField18To4(void *arg0)
{
  unsigned int ofs;
  Battle_SetMode(*((s32 *)(arg0 + 8)), *((s16 *)(((u8 *)arg0) + 0x24)), 0x18, 0xC3333);
  WaitFrames(0x1D);
  ofs = 0x18;
  *((s32 *)(((u8 *)arg0) + ofs)) = 4;
  Battle_Apply(arg0, 2);
}
