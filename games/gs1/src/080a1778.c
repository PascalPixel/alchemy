#include "types.h"

s32 Resource_CopyData(s32 entry_no, s32 mode, s32 data);
s32 Resource_FindFreeSlot(void);
s32 UiIcon_DrawWithFlags(s32 entry_no, s32 flags, s32 first, s32 second, s32 third);
extern u8 Data_080aea4c[];
s32 Func_080a1778(s32 first, s32 second, s32 third)
{
  s32 temp_r0;
  unsigned char new_var;
  int new_var2;
  s32 var_r6;
  var_r6 = 0;
  var_r6 = 0;
  temp_r0 = Resource_FindFreeSlot();
  new_var2 = 0x80;
  if (temp_r0 != 0)
  {
    Resource_CopyData(temp_r0, new_var = new_var2, Data_080aea4c);
    var_r6 = UiIcon_DrawWithFlags(temp_r0, 0x40000000, first, second, third);
  }
  return var_r6;
}
