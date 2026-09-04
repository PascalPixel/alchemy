#include "types.h"
#include "global_cells.h"

#define Event_WaitForDisplayField358Clear Func_08093710

s32 WaitFrames(s32);
void *Runtime_AllocateBlock(s32 arg0, s32 arg1);
void Event_WaitForDisplayField358Clear(void)
{
  s32 cnt;
  void *work;
  u8 *p;
  work = *((void **) ADDR_03001E70);
  if ((*((s16 *) (((u8 *) Runtime_AllocateBlock(0x1B, 0xCCC)) + 0x19E))) == 3)
  {
    cnt = 0;
    if ((*((s16 *) (((u8 *) work) + 0x358))) != 0)
    {
 do { loop_3: WaitFrames(1); cnt += 1; if (cnt <= 0x12B) { if ((*((s16 *) ((p = (u8 *) work) + 0x358))) != 0) { goto loop_3; } } } while (0);
    }
  }
}
