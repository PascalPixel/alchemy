#include "types.h"
#include "object_efx.h"

s32 Object_SetCallback(void *, s32);
void BattleEffect_UpdateOrbitingParticleFade(void *object)
{
  s32 primary_fade;
  u8 *object_bytes;
  if (object != ((void *) 0))
  {
    primary_fade = *((s32 *) (((u8 *) object) + 0x18));
    primary_fade = primary_fade + 0xFFFFF000;
    *((s32 *) ((object_bytes = (u8 *) object) + 0x1C)) = (s32) ((*((s32 *) (object_bytes + 0x1C))) + 0xFFFFF000);
    *((s32 *) (object_bytes + 0x18)) = primary_fade;
    if (primary_fade <= 0x1000)
    {
      Object_SetCallback(object, Data_0809f0b0);
    }
  }
}
