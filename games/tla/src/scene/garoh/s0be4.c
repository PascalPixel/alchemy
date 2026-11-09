#include "garoh.h"
#include "main_routine_work.h"
#include "event_work.h"
#include "io_reg.h"
#include "event_inline.h"
void Event_StartScreenTransition(void);
void Func_02000a68(void);
void Func_0200538c(void);

void Func_02000be4(void) {
  FieldTask_AddFrame(Func_02000a68, 0x480);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0;
  if (Event_GetFlag(0x91b) == 0 || Event_GetFlag(0x310) != 0) {
    Event_SetFlag(0x310);
    PaletteFade_ApplyTarget(0x10007, 1);
    if (Event_GetFlag(0x311))
      PaletteFade_ApplyTarget(0x10003, 2);
    PaletteFade_Interpolate(1);
    Event_WaitTransitionFrames();
    Func_0200538c();
    {
      u8 *palette = *(u8 **)((u8 *)Ram_HeapSlots + 0x80);

      *(volatile u16 *)&BG_PLTT_COLOR(1, 14) = 0x7fff;
      *(u16 *)(palette + 60) = *(volatile u16 *)&BG_PLTT_COLOR(1, 14);
    }
    PaletteFade_ApplyTarget(0x10000, 1);
  } else {
    MapGrid_CopyCellsTo(10, 32, 10, 10, 1, 1);
    MapGrid_CopyCellsTo(12, 32, 25, 22, 1, 1);
    MapGrid_CopyCellsTo(11, 33, 8, 22, 1, 1);
    MapGrid_CopyCellsTo(74, 32, 74, 10, 1, 1);
    MapGrid_CopyCellsTo(76, 32, 89, 22, 1, 1);
    MapGrid_CopyCellsTo(75, 33, 72, 22, 1, 1);
  }
  if (gMainRoutineWork.entrance == 12) {
    struct EventWork *work = Ram_HeapSlots->eventWork;

    work->screenEffect = 0x209;
    Event_ClearFlag(0x120);
    Event_ClearFlag(0x121);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0;
  }
}
