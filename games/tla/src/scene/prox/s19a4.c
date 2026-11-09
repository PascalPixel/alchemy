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
  Event_ResolvePendingActions(0);
  if (!GameFlag_Test(0x200)) {
    if (mode == 2)
      Sound_Play(188);
    else
      Sound_Play(158);
    Map_CopyCellPictureIndicesRect(a, b, c, d, mode, f);
    Event_Wait(20);
  }
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x8000, 0x4000);
  if (mode == 1) {
    object = Actor_GetById(gMainRoutineWork.currentOwner);
    cell = object->x >> 16;
    Actor_SetMoveTarget(object, ((8 - cell % 16) << 16) + object->x, object->y,
                  object->z);
    Script_WaitForEventTimeout(object);
    object->angle = 0xc000;
  }
  Actor_SetMovementTypeById(gMainRoutineWork.currentOwner,
                                          gData_020046f8);
  wait(12);
  Sound_Play(123);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(work->returnEntrance);
}
