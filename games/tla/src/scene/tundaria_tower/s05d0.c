#include "iwram_call.h"
#include "call.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "text/msg_ids.h"
#include "types.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void Func_02003048();
void Func_02003148();
void Func_020031a8();
void Func_02002fc0();
void FieldEvent_Wait();
void Actor_SetSpeedParameters();
s32 Actor_GetById();
void Actor_SetPositionAndReset();
void Actor_ResetAndSetPositionInMode2();
void Event_FinishScreenTransition();
void Event_WaitTransitionFrames();
void Actor_WaitForMoveById();
s32 Func_020030e8();
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
  Call1((void (*)())Func_02003148, 0);
  choice = (s16 *)(work + 340);
  if (*choice == 3)
    x = 25;
  Call1((void (*)())Func_020031a8, 158);
  one = 1;
  height = 2;
  Call6((void (*)())Func_02002fc0, 30, 38, x, 49, one, height);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call6((void (*)())Func_02002fc0, 32, 38, x, 49, one, height);
  Call1((void (*)())FieldEvent_Wait, 10);
  owner = (s32 *)&gMainRoutineWork;
  owner += 133;
  Call3((void (*)())Actor_SetSpeedParameters, *owner, 0x8000, 0x4000);
  ((struct Actor *)Actor_GetById(*owner))->motionFlags = 0;
  cell = (((struct Actor *)Actor_GetById(*owner))->x >> 19) | one;
  Call3((void (*)())Actor_SetPositionAndReset, *owner, cell << 3,
        *(s16 *)((u8 *)Actor_GetById(*owner) + 18) | one);
  Call1((void (*)())Func_020031a8, 123);
  cell = ((struct Actor *)Actor_GetById(*owner))->x;
  Call3((void (*)())Actor_ResetAndSetPositionInMode2, *owner, (cell >> 19) << 3,
        (*(s16 *)((u8 *)Actor_GetById(*owner) + 18) | one) - 16);
  Call1((void (*)())FieldEvent_Wait, 5);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Actor_WaitForMoveById(*owner);
  Func_020030e8(*choice);
  return Func_02003050();
}
