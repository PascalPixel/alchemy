// Near miss (128 native instructions, 83 differ in alignment, same calls and
// constants): the draft keeps the entrance in a halfword register across the
// first test and compares it shifted at the second (ldrh, lsls, cmp), where the
// native reloads it with ldrsh before each test; the first screenEffect store
// builds 0x209 as its own constant, where the native adds 93 to the field
// offset. Chained assignments and one work pointer measured the same.
#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_work.h"
#include "input.h"
#include "scene_ids.h"
#include "call.h"
#include "text/msg_ids.h"

s32 GameFlag_Test(s32 flag);
void GameFlag_ClearBit(s32 flag);
void wait(s32 frames);
void FieldEvent_Wait(s32 frames);
struct Actor *Actor_GetById(s32 id);
void Actor_ArmCallback(s32 id, s32 angle, s32 flags);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_02000658(void);
void Func_02001488(void);
void Func_02001500(s32 id, s32 x, s32 z);
void Func_02001538(s32 message);
void Func_02001550(s32 id, s32 flags);
void Func_02001578(s32 speed, s32 acceleration);
void Func_02001588(s32 scene, s32 entrance);

s32 Func_02000514(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;

  work->screenEffect = 0x209;
  Actor_GetById(15)->update = Func_02001488;
  if (gMainRoutineWork.entrance == 80) {
    gMainRoutineWork.entrance = gMainRoutineWork.secondary = 14;
    Actor_ArmCallback(14, 0x5000, 0);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    FieldEvent_Wait(10);
    Func_02001538((s32)&MsgAlhafraManor10);
    Func_02001550(14, 0);
    GameFlag_ClearBit(0x12f);
  }
  if (gMainRoutineWork.entrance == 81) {
    gMainRoutineWork.entrance = gMainRoutineWork.secondary = 8;
    Func_02000658();
  }
  if (!GameFlag_Test(0x8a7) && GameFlag_Test(0x8ff)) {
    Func_02001500(18, 0, 0);
    Func_02001500(19, 0, 0);
  }
  if (gMainRoutineWork.entrance == 99) {
    Camera_MoveWithinMapBounds(152 << 17, -1, 160 << 17, 0);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 256;
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    FieldEvent_Wait(10);
    Func_02001578(0x20000, 0x4000);
    Camera_MoveWithinMapBounds(144 << 16, -1, 160 << 17, 1);
    while (gInput.held == 0)
      wait(1);
    Func_02001588((s32)&SceneId_Ov667, 80);
  }
  return 0;
}
