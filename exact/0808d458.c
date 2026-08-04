#include "types.h"

u32 Func_0808d458(s32 descriptor, s32 value)
{
  u32 masked_difference;
  s32 descriptor_kind;
  u32 result;
  if ((0xF & descriptor) != 3)
  {
    return 0U;
  }
  descriptor_kind = 0x1FF & descriptor;
  result = 0;
  if (descriptor_kind != 3)
  {
    masked_difference = ((u32)value & 0xFFF00000) ^ 0x500000;
    result = masked_difference | (0U - masked_difference);
    result = result >> 0x1F;
  }
  return result;
}
