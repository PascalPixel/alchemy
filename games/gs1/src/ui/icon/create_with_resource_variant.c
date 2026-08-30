#include "types.h"

s32 Resource_CopyData(s32 entry_no, s32 mode, s32 data);
s32 Resource_FindFreeSlot(void);
s32 UiIcon_DrawWithFlags(s32 entry_no, s32 flags, s32 first, s32 second, s32 third);
extern u8 Data_080aea4c[];
s32 UiIcon_CreateWithResourceVariant(s32 first, s32 second, s32 third)
{
  s32 slot;
  unsigned char copy_mode;
  int resource_mode;
  s32 icon;
  icon = 0;
  icon = 0;
  slot = Resource_FindFreeSlot();
  resource_mode = 0x80;
  if (slot != 0)
  {
    Resource_CopyData(slot, copy_mode = resource_mode, Data_080aea4c);
    icon = UiIcon_DrawWithFlags(slot, 0x40000000, first, second, third);
  }
  return icon;
}
