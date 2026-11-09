#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_work.h"
#include "input.h"
#include "scene_ids.h"
#include "text/msg_ids.h"
#include "event_inline.h"

void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_02000658(void);
void Func_02001488(void);
void Func_02001588(s32 scene, s32 entrance);

s32 Func_02000514(void) {
  s32 cameraY = -1;
  s32 cameraZ = 160 << 17;
  s16 *entrance;

  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x209;
  Actor_GetById(15)->update = Func_02001488;
  if (gMainRoutineWork.entrance == 80) {
    gMainRoutineWork.entrance = gMainRoutineWork.secondary = 14;
    Actor_ArmCallback(14, 0x5000, 0);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Event_Wait(10);
    Event_SetMessageId((s32)&MsgAlhafraManor10);
    Event_ShowMessageAndWait(14, 0);
    GameFlag_ClearBit(0x12f);
  }
  entrance = &gMainRoutineWork.entrance;
  if (*entrance == 81) {
    gMainRoutineWork.entrance = gMainRoutineWork.secondary = 8;
    Func_02000658();
  }
  if (!GameFlag_Test(0x8a7) && GameFlag_Test(0x8ff)) {
    Actor_SetHorizontalPositionWithTerrain(18, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(19, 0, 0);
  }
  if (gMainRoutineWork.entrance == 99) {
    Camera_MoveWithinMapBounds(152 << 17, cameraY, cameraZ, 0);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 256;
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Event_Wait(10);
    Camera_SetSpeedParameters(0x20000, 0x4000);
    {
      /* FAKEMATCH(pin): the mode in r3 ahead of the other arguments; as a
       * literal, 2 of 128 instructions differ in tla-en. */
      register s32 mode asm("r3") = 1;

      Camera_MoveWithinMapBounds(144 << 16, cameraY, cameraZ, mode);
    }
    goto check;
    do {
      wait(1);
    check:;
    } while (gInput.held == 0);
    Func_02001588((s32)&SceneId_Ov667, 80);
  }
  return 0;
}
