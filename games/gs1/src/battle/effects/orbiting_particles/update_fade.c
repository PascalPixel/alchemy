#include "types.h"
#include "object_efx.h"

s32 Object_SetCallback(void *, s32);
#define UpdateOrbitingParticleFade Func_08099040
void UpdateOrbitingParticleFade(void *object)
{
  s32 temp_r3;
  u8 *new_var;
  if (object != ((void *) 0))
  {
    temp_r3 = *((s32 *) (((u8 *) object) + 0x18));
    temp_r3 = temp_r3 + 0xFFFFF000;
    *((s32 *) ((new_var = (u8 *) object) + 0x1C)) = (s32) ((*((s32 *) (new_var + 0x1C))) + 0xFFFFF000);
    *((s32 *) (new_var + 0x18)) = temp_r3;
    if (temp_r3 <= 0x1000)
    {
      Object_SetCallback(object, Data_0809f0b0);
    }
  }
}
