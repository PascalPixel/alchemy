#include "object_lookup.h"
#include "types.h"

s32 Object_ResetMotion();
void ObjectMotion_EnableActionAndResetMotion(void)
{
  void *temp_r0;
  unsigned char new_var;
  temp_r0 = ObjectTable_Get();
  if (temp_r0 != ((void *) 0))
  {
    new_var = 1;
    *((u8 *) (((u8 *) temp_r0) + 0x5A)) = (u8) (new_var | (*((u8 *) (((u8 *) temp_r0) + 0x5A))));
    Object_ResetMotion();
  }
}
