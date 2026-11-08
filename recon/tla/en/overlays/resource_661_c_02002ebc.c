// Matches: differ 0 (tla-en, 2026-11-08). The leader's id and the battle rule
// are read and set through one function-scope pointer to gMainRoutineWork, as
// the adopted scenes write it (party). That pointer gets no callee-saved
// register, so reload rebuilds it from its pool word at each use: the leader
// reads fold to one word at currentOwner, and the last store adds 587 to a
// fresh base in r3 that cannot be tied to its sum, giving the sum r2 and the
// value r3. Every earlier form (the plain store, a block-local pointer, s8, u8,
// s64 or held values, pinning) tied base and sum and left 4 to 82.
// Devices: only call.h's forwarders. Both camera calls go through Call4 (as
// direct calls, 20 and 2 of 187 instructions differ: -1 is kept in r5);
// Actor_SetSpeedParameters through Call3 (direct, 2 differ); the walk to
// (232, 148) through Call4 with its angle held in a local (as a literal, 4).
#include "types.h"
#include "event_work.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "scene_tables.h"
#include "call.h"

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
  struct Actor *monster = Actor_GetById(8);

  Func_02004040();
  Func_020041b8(0);
  Func_020041f8(78);
  Call4((void (*)())Camera_MoveWithinMapBounds, -1, -1, -1, 0);
  Call4((void (*)())Camera_MoveWithinMapBounds, PIXELS(232), -1, PIXELS(184),
        0);
  Func_02003fd8();
  wait(1);
  Actor_SetSingleChildFlags(Actor_GetById(party->currentOwner), 0);
  leader->motionFlags = 0;
  leader->y = 0x820000;
  leader->gravity = 0x8000;
  leader->bounce = 0;
  Actor_SetSingleChildFlags(Actor_GetById(8), 0);
  {
    // The angle is held in a local; as a literal argument, its constant is
    // built last.
    s32 angle = 0x4000;

    Call4((void (*)())Func_02004098, 8, PIXELS(232), PIXELS(148), angle);
  }
  monster->motionFlags = 0;
  monster->y = 0x820000;
  monster->gravity = 0x8000;
  monster->bounce = 0;
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
  monster->motionFlags = 3;
  Actor_SetSingleChildFlags(Actor_GetById(8), 1);
  FieldEvent_Wait(24);
  Func_02000b84(monster->x, monster->y, monster->z);
  Func_020041f8(188);
  Actor_ArmCallback(party->currentOwner, 0xc000, 0);
  FieldEvent_Wait(40);
  Func_020041f8(147);
  Actor_SetModeById(8, 3);
  FieldEvent_Wait(20);
  monster->velocityY = 0x80000;
  monster->gravity = 0xe666;
  Call3((void (*)())Actor_SetSpeedParameters, 8, 0x19999, 0xcccc);
  Actor_SetPositionAndCommit(8, 232, 184);
  party->battleRule24b = 2;
  Party_SetFields1eeAnd1f0((s32)&SceneId_Ov661Room10, 2);
  Func_02004140(10, 0);
}
