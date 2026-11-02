#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"

struct EventWork154 {
  u8 unk000[0x154];
  s16 returnEntrance;
};

extern const u8 gData_020046f8[];

struct Actor *Actor_GetById(s32 id);
s32 GameFlag_Test(s32 flag);
void wait(s32 frames);
void FieldEvent_Wait(s32 frames);
void Actor_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void Actor_SetMovementTypeById(s32 id, const void *script);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_02004464(void);
void Func_020045ec(s32 skip);
void Func_0200463c(s32 cue);
void Func_020043f4(struct Actor *object, s32 x, s32 y, s32 z);
void Func_020043fc(struct Actor *object);
void Func_02004404(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_0200458c(s32 entrance);

void Func_020019a4(s32 a, s32 b, s32 c, s32 d, s32 mode, s32 f) {
  struct EventWork154 *work = Ram_HeapSlots->eventWork;
  struct Actor *object;
  s32 cell;

  Func_02004464();
  Func_020045ec(0);
  if (!GameFlag_Test(0x200)) {
    if (mode == 2)
      Func_0200463c(188);
    else
      Func_0200463c(158);
    Func_02004404(a, b, c, d, mode, f);
    FieldEvent_Wait(20);
  }
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x8000, 0x4000);
  if (mode == 1) {
    object = Actor_GetById(gMainRoutineWork.currentOwner);
    cell = object->x >> 16;
    Func_020043f4(object, ((8 - cell % 16) << 16) + object->x, object->y,
                  object->z);
    Func_020043fc(object);
    object->angle = 0xc000;
  }
  Actor_SetMovementTypeById(gMainRoutineWork.currentOwner,
                                          gData_020046f8);
  wait(12);
  Func_0200463c(123);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_0200458c(work->returnEntrance);
}
