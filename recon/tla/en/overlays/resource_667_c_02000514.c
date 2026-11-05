// Draft, differ 2 of 128, skeleton gap 1 (tla-en, 2026-11-01)
// Left: one sched2 tie at the second camera call. The native sets the mode
// (movs r3, #1) before lsls r0; the draft sets it last, after lsls r2. All the
// candidates tie, so the order comes from insn position: the native has the set
// of r3 ahead of x's load, the draft has r0 to r3 in order. The first call, with
// mode 0, already matches. world_map's Func_02002e00 has the same pair and pins
// r3 for it; that is a device and is not used here.
// Tried for the pair, no gain: mode as a local at the top, first, in an inner
// block or `register`; x as a local; the camera unprototyped; other loop forms
// (a plain goto loop ties, a for loop is worse); s32 returns on the camera or
// Func_02001578 are worse (differ 4).
// What closed the rest, for the next reader: cameraY and cameraZ set once at the
// top go unallocated and rematerialize at each call, as written inline they
// shared r5 and r6. The entrance pointer keeps the second test a load of its
// own, as aqua_rock's s17f4.c does; read directly, gcse merges the two tests
// into a halfword load compared shifted. The loop is entered at its test, as in
// world_map, or the test is copied ahead and gInput hoisted.
#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_work.h"
#include "input.h"
#include "scene_ids.h"
#include "text/msg_ids.h"
#include "call.h"

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
    FieldEvent_Wait(10);
    Func_02001538((s32)&MsgAlhafraManor10);
    Func_02001550(14, 0);
    GameFlag_ClearBit(0x12f);
  }
  entrance = &gMainRoutineWork.entrance;
  if (*entrance == 81) {
    gMainRoutineWork.entrance = gMainRoutineWork.secondary = 8;
    Func_02000658();
  }
  if (!GameFlag_Test(0x8a7) && GameFlag_Test(0x8ff)) {
    Func_02001500(18, 0, 0);
    Func_02001500(19, 0, 0);
  }
  if (gMainRoutineWork.entrance == 99) {
    Camera_MoveWithinMapBounds(152 << 17, cameraY, cameraZ, 0);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 256;
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    FieldEvent_Wait(10);
    Func_02001578(0x20000, 0x4000);
    {
      Call4((void (*)())Camera_MoveWithinMapBounds, 144 << 16, cameraY, cameraZ, 1);
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
