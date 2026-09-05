#include "types.h"

extern const u8 Data_080c7420[];

u32 Summon_GetEntryByte3Kind(s32 arg0)
{
  u32 kind;
  u8 *p;
  p = (u8 *) ((arg0 * 8) + (s32)Data_080c7420);
  kind = ((u8) (*((u8 *) (p + 3)))) >> 5;
  if (((s32) kind) > 4)
  {
    kind = -1U;
  }
  return kind;
}
