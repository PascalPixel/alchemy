#include "types.h"
#include "global_cells.h"
#include "gs1_edition.h"

s32 Func_080f9048();
s32 UiWork_CheckCancelByInput(void *obj)
{
  int zero;
  s32 flag;
  flag = 0;
  if (((*((u8 *)(((u8 *)(*((void **)ADDR_03001E8C))) + RENDER_BUSY_OFS))) != 0) && (Func_080f9048() == 0))
  {
    flag = 1;
  }
  zero = 0;
  if ((*((s32 *)ADDR_03001AE8)) & 0x303)
  {
    flag = 1;
  }
  if (flag != zero)
  {
    *((s16 *)(((u8 *)obj) + 0x14)) = zero;
    return 1;
  }
  return zero;
}

s32 UiWork_CheckCancelByModeInput(void *obj)
{
  void *p;
  s32 tmp;
  unsigned char zero;
  s32 key;
  s32 flag;
  void *work;
  p = *((void **)ADDR_03001E8C);
  work = p;
  flag = 0;
  if (((*((u8 *)(((u8 *)work) + RENDER_BUSY_OFS))) != 0) && (Func_080f9048() == 0))
  {
    flag = 1;
  }
  key = (tmp = *((s32 *)ADDR_03001C94));
  zero = 0;
  if ((*((u8 *)(work + RENDER_MODE_OFS))) != zero)
  {
    key = *((s32 *)ADDR_03001AF8);
  }
  if (0x303 & key)
  {
    flag = 1;
  }
  if (flag != 0)
  {
    *((s16 *)(((u8 *)obj) + 0x14)) = zero;
    return 1;
  }
  return 0;
}
