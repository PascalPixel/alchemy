#include "types.h"
#include "event_work.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "scene_tables.h"
#include "call.h"
#include "fixed_math.h"

extern char SceneId_Ov661Room10;

struct Actor *Actor_GetById(s32 id);
void Actor_SetSingleChildFlags(struct Actor *object, s32 flags);
void Camera_MoveWithinMapBounds(s32 x, s32 a, s32 z, s32 b);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void FieldEvent_Wait(s32 frames);
void wait(s32 frames);
void Motion_SetModeAndWaitAnimation(s32 id, s32 mode);
void Actor_ArmCallback(s32 id, s32 angle, s32 flags);
void Actor_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void Actor_SetPositionAndCommit(s32 id, s32 x, s32 z);
void Actor_SetModeById(u32 id, s32 mode);
void Party_SetFields1eeAnd1f0(s32 scene, s32 entrance);
void Func_02000b84(s32 x, s32 y, s32 z);
void Func_02003fd8(void);
void Func_02004040(void);
void Func_02004098(s32 actor, s32 x, s32 z, s32 angle);
void Func_02004108(s32 actor, s32 a);
void Func_02004140(s32 a, s32 b);
void Func_020041b8(s32 skip);
void Func_020041f8(s32 cue);

void Func_02002ebc(void) {
  struct MainRoutineWork *party = &gMainRoutineWork;
  struct Actor *leader = Actor_GetById(party->currentOwner);
  struct Actor *actor8 = Actor_GetById(8);
  /* FAKEMATCH(wrapper): the camera, walk and speed calls forwarded through
   * call.h's Call3 and Call4; as direct calls, 28 of 187 instructions differ
   * in tla-en. */

  Func_02004040();
  Func_020041b8(0);
  Func_020041f8(78);
  Call4(Camera_MoveWithinMapBounds, -1, -1, -1, 0);
  Call4(Camera_MoveWithinMapBounds, PIXELS(232), -1, PIXELS(184), 0);
  Func_02003fd8();
  wait(1);
  Actor_SetSingleChildFlags(Actor_GetById(party->currentOwner), 0);
  leader->motionFlags = 0;
  leader->y = PIXELS(130);
  leader->gravity = Q16_ONE / 2;
  leader->bounce = 0;
  Actor_SetSingleChildFlags(Actor_GetById(8), 0);
  {
    // The angle is held in a local; as a literal argument, its constant is
    // built last.
    s32 angle = TRIG_TURN / 4;

    Call4(Func_02004098, 8, PIXELS(232), PIXELS(148), angle);
  }
  actor8->motionFlags = 0;
  actor8->y = PIXELS(130);
  actor8->gravity = Q16_ONE / 2;
  actor8->bounce = 0;
  wait(1);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x203;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Func_020041f8(204);
  leader->motionFlags = 3;
  Actor_SetSingleChildFlags(Actor_GetById(party->currentOwner), 1);
  FieldEvent_Wait(24);
  Func_02000b84(leader->x, leader->y, leader->z);
  Func_020041f8(188);
  Func_02004108(party->currentOwner, 0x101);
  Motion_SetModeAndWaitAnimation(party->currentOwner, 49);
  Func_020041f8(204);
  actor8->motionFlags = 3;
  Actor_SetSingleChildFlags(Actor_GetById(8), 1);
  FieldEvent_Wait(24);
  Func_02000b84(actor8->x, actor8->y, actor8->z);
  Func_020041f8(188);
  Actor_ArmCallback(party->currentOwner, TRIG_TURN * 3 / 4, 0);
  FieldEvent_Wait(40);
  Func_020041f8(147);
  Actor_SetModeById(8, 3);
  FieldEvent_Wait(20);
  actor8->velocityY = Q16_ONE * 8;
  actor8->gravity = 0xe666;
  Call3(Actor_SetSpeedParameters, 8, 0x19999, 0xcccc);
  Actor_SetPositionAndCommit(8, 232, 184);
  party->battleRule24b = 2;
  Party_SetFields1eeAnd1f0((s32)&SceneId_Ov661Room10, 2);
  Func_02004140(10, 0);
}
