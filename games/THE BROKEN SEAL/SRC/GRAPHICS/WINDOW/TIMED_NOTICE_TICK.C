#include "TYPES.H"
#include "GLOBAL_CELLS.H"
extern u8 Data_03001ebc[];

s32 Scheduler_RemoveCallback(s32);
void UiWork_Finalize(struct Work *work, s32 release);
void UiTimedNotice_Tick(void)
{
  void *work;
  s32 *slot;
  u16 cnt;
  void *state;
  int zero;
  state = *((void **)((u32)&Data_03001ebc));
  work = state;
  *((u16 *)(((u8 *)work) + 0x234)) = (cnt = (*((u16 *)(((u8 *)work) + 0x234))) + 0xFFFF);
  zero = 0;
  if ((cnt << 0x10) == zero)
  {
    UiWork_Finalize(*(slot = (s32 *)(((u8 *)work) + 0x230)), 2);
    Scheduler_RemoveCallback((s32)UiTimedNotice_Tick);
  }
}
