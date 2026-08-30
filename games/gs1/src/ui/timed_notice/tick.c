#include "types.h"
#include "global_cells.h"

s32 ScheduleCallback(s32);
void UiWork_Finalize(struct Work *work, s32 release);
void UiTimedNotice_Tick(void)
{
  void *notice_state;
  s32 *work_slot;
  u16 remaining_ticks;
  void *state;
  int zero;
  state = *((void **) ADDR_03001EBC);
  notice_state = state;
  *((u16 *) (((u8 *) notice_state) + 0x234)) = (remaining_ticks = (*((u16 *) (((u8 *) notice_state) + 0x234))) + 0xFFFF);
  zero = 0;
  if ((remaining_ticks << 0x10) == zero)
  {
    UiWork_Finalize(*(work_slot = (s32 *) (((u8 *) notice_state) + 0x230)), 2);
    ScheduleCallback((s32)UiTimedNotice_Tick);
  }
}
