#include "types.h"
#include "global_cells.h"

u32 EffectRuntime_IsActive(void)
{
  s16 active_count;
  u32 is_active;
  void *runtime;
  runtime = *((void **) ADDR_03001EBC);
  is_active = 0;
  if (runtime != ((void *) 0))
  {
    active_count = *((s16 *) (((u8 *) runtime) + 0xCB8));
    is_active = (u32) ((0 - active_count) | active_count);
    is_active = is_active >> 0x1F;
  }
  return is_active;
}
