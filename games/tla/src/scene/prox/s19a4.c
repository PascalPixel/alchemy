#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_inline.h"

struct EventWork154 {
  u8 unk000[0x154];
  s16 returnEntrance;
};

extern const u8 gData_020046f8[];

void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_02004464(void);

void Func_020019a4(s32 a, s32 b, s32 c, s32 d, s32 mode, s32 f) {
  struct EventWork154 *work = Ram_HeapSlots->eventWork;
  struct Actor *object;
  s32 cell;

  Func_02004464();
  FieldEvent_ResolvePendingActions(0);
  if (!Event_GetFlag(0x200)) {
    if (mode == 2)
      Event_PlaySound(188);
    else
      Event_PlaySound(158);
    MapGrid_CopyCellsTo(a, b, c, d, mode, f);
    FieldEvent_Wait(20);
  }
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x8000, 0x4000);
  if (mode == 1) {
    object = Actor_Get(gMainRoutineWork.currentOwner);
    cell = object->x >> 16;
    FieldActor_SetMoveTarget(object, ((8 - cell % 16) << 16) + object->x, object->y,
                  object->z);
    Actor_WaitForMove(object);
    object->angle = 0xc000;
  }
  Actor_SetScript(gMainRoutineWork.currentOwner,
                                          gData_020046f8);
  System_Wait(12);
  Event_PlaySound(123);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(work->returnEntrance);
}
