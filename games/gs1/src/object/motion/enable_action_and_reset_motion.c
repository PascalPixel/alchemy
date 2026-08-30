#include "object_lookup.h"
#include "types.h"

s32 Object_ResetMotion();
void ObjectMotion_EnableActionAndResetMotion(void)
{
  void *object;
  unsigned char action_enabled;
  object = ObjectTable_Get();
  if (object != ((void *) 0))
  {
    action_enabled = 1;
    *((u8 *) (((u8 *) object) + 0x5A)) = (u8) (action_enabled | (*((u8 *) (((u8 *) object) + 0x5A))));
    Object_ResetMotion();
  }
}
