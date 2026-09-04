#include "types.h"
#include "global_cells.h"

#define Runtime_BlankDisplayLoadValueAndRun Func_080f6008

s32 Func_080f7460(void);
s32 Audio_PlayCue(s32);
s32 Runtime_BlankDisplayLoadValueAndRun(void)
{
  s32 *p;
  u8 *src;
  if (1)
  {
    *((s16 *) 0x04000000) = 0x40;
    src = (u8 *) ((void *) 0x02000240);
    p = (s32 *) ADDR_03001CB4;
    *p = *((s32 *) (src + 4));
  }
  Audio_PlayCue(9);
  Func_080f7460();
  return 0;
}
