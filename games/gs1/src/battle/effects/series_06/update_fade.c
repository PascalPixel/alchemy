#include "types.h"
#include "object_efx.h"

s32 Func_08009098(void *, s32);
#define UpdateEffect06Fade Func_08099040
void UpdateEffect06Fade(void *object)
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
      Func_08009098(object, Data_0809f0b0);
    }
  }
}
