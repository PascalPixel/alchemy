#include "iwram_call.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "text/msg_ids.h"
#include "types.h"
#include "event_inline.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void Func_02003048();
void Event_FinishScreenTransition();
void Event_WaitTransitionFrames();
s32 Func_02003050();

s32 Func_020005d0(void) {
  u8 *work = (u8 *)Ram_HeapSlots->eventWork;
  s16 *choice;
  s32 x = 37;
  s32 *owner;
  s32 one;
  s32 height;
  s32 cell;

  Func_02003048();
  FieldEvent_ResolvePendingActions(0);
  choice = (s16 *)(work + 340);
  if (*choice == 3)
    x = 25;
  Event_PlaySound(158);
  one = 1;
  height = 2;
  MapGrid_CopyCellsTo(30, 38, x, 49, one, height);
  FieldEvent_Wait(10);
  MapGrid_CopyCellsTo(32, 38, x, 49, one, height);
  FieldEvent_Wait(10);
  owner = (s32 *)&gMainRoutineWork;
  owner += 133;
  Actor_SetSpeed(*owner, 0x8000, 0x4000);
  ((struct Actor *)((s32)Actor_GetById(*owner)))->motionFlags = 0;
  cell = (((struct Actor *)((s32)Actor_GetById(*owner)))->x >> 19) | one;
  Actor_WalkToAndWait(*owner, cell << 3,
                      *(s16 *)((u8 *)((s32)Actor_GetById(*owner)) + 18) | one);
  Event_PlaySound(123);
  cell = ((struct Actor *)((s32)Actor_GetById(*owner)))->x;
  Actor_WalkTo(*owner, (cell >> 19) << 3,
               (*(s16 *)((u8 *)((s32)Actor_GetById(*owner)) + 18) | one) - 16);
  FieldEvent_Wait(5);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Actor_WaitForMoveById(*owner);
  Import_EventSetExitRequest(*choice);
  return Func_02003050();
}
