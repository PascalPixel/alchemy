#include "types.h"

extern s32 Func_08004080(void);
extern s32 Resource_CopyData();
extern s32 UiIcon_DrawWithFlags();
extern u8 Data_080aea4c[];
s32 UiIcon_CreateWithResource(s32 first, s32 unused, s32 second, s32 third)
{
    s32 entry_no;
    s32 result;
    result = 0;
    entry_no = Func_08004080();
    if (entry_no != 0) {
        Resource_CopyData(entry_no, 0x80, Data_080aea4c);
        result = UiIcon_DrawWithFlags(entry_no, 0x40000000, first, second, third);
    }
    return result;
}

s32 Resource_CopyData(s32 entry_no, s32 mode, s32 data);
s32 UiIcon_DrawWithFlags(s32 entry_no, s32 flags, s32 first, s32 second, s32 third);
s32 UiIcon_CreateWithResourceVariant(s32 first, s32 second, s32 third)
{
  s32 slot;
  unsigned char copy_mode;
  int resource_mode;
  s32 icon;
  icon = 0;
  icon = 0;
  slot = Func_08004080();
  resource_mode = 0x80;
  if (slot != 0)
  {
    Resource_CopyData(slot, copy_mode = resource_mode, Data_080aea4c);
    icon = UiIcon_DrawWithFlags(slot, 0x40000000, first, second, third);
  }
  return icon;
}

#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

struct UiIconObject { u8 unknown_00[0x16]; u16 value_16 : 9; u16 unknown_16b : 7; };

void UiIcon_PrepareObject(void *object)
{
    if (object != NULL) {
        FIELD_AT_OFFSET(object, s8, 5) = 1;
        ((struct UiIconObject *) object)->value_16 = FIELD_AT_OFFSET(object, u16, 6);
        FIELD_AT_OFFSET(object, s8, 0x14) = FIELD_AT_OFFSET(object, u16, 8);
        FIELD_AT_OFFSET(object, s8, 0x17) = -0x3F & FIELD_AT_OFFSET(object, s8, 0x17);
        FIELD_AT_OFFSET(object, s8, 0x15) = -4 & FIELD_AT_OFFSET(object, s8, 0x15);
    }
}
