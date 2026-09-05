#include "types.h"

#define BattleEffect_CheckDescriptorKind3Result Func_0808d458

u32 BattleEffect_CheckDescriptorKind3Result(s32 descriptor, s32 value)
{
  u32 tmp;
  s32 kind;
  u32 result;
  if ((0xF & descriptor) != 3)
  {
    return 0U;
  }
  kind = 0x1FF & descriptor;
  result = 0;
  if (kind != 3)
  {
    tmp = ((u32)value & 0xFFF00000) ^ 0x500000;
    result = tmp | (0U - tmp);
    result = result >> 0x1F;
  }
  return result;
}
