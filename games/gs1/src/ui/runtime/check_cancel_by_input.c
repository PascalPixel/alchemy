#include "types.h"
#include "global_cells.h"
#include "gs1_edition.h"

#define UiWork_CheckCancelByInput Func_0801999c

s32 Func_080f9048();
s32 UiWork_CheckCancelByInput(void *obj)
{
  int zero;
  s32 flag;
  flag = 0;
  if (((*((u8 *) (((u8 *) (*((void **) ADDR_03001E8C))) + RENDER_BUSY_OFS))) != 0) && (Func_080f9048() == 0))
  {
    flag = 1;
    if (1)
    {
    }
  }
  zero = 0;
 ;
  if ((*((s32 *) ADDR_03001AE8)) & 0x303)
  {
    flag = 1;
  }
  if (flag != zero)
  {
    *((s16 *) (((u8 *) obj) + 0x14)) = zero;
    return 1;
  }
  return zero;
}
