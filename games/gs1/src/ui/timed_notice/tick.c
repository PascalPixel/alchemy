#include "types.h"
#include "global_cells.h"

s32 ScheduleCallback(s32);
void UiWork_Finalize(struct Work *work, s32 release);
void UiTimedNotice_Tick(void)
{
  void *new_var2;
  s32 *new_var;
  u16 temp_r3;
  void *temp_r1;
  int new_var3;
  temp_r1 = *((void **) ADDR_03001EBC);
  new_var2 = temp_r1;
  *((u16 *) (((u8 *) new_var2) + 0x234)) = (temp_r3 = (*((u16 *) (((u8 *) new_var2) + 0x234))) + 0xFFFF);
  new_var3 = 0;
  if ((temp_r3 << 0x10) == new_var3)
  {
    UiWork_Finalize(*(new_var = (s32 *) (((u8 *) new_var2) + 0x230)), 2);
    ScheduleCallback((s32)UiTimedNotice_Tick);
  }
}
