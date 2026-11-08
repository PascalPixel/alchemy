// Differ 6 of 187 (tla-en, 2026-11-08, -mtune=gba).
// Draft, differ 6 of 187 (tla-en, 5 November), from 82: reload's spill registers rotate through the function, and the native's set is r1, r2, r3.
// The battle rule store is what brings r1 in: with a local value it needs three.
// Left: only that store. Native: ldr r3, pool; r1 = offset; adds r2, r3, r1; movs r3, #2; strb r3, [r2]: the value in r3 after an address pseudo in r2.
// Here the sum stays in r3 and the value takes r2. Pinning the value to r3 leaves 4. p set from the field folds to one pool word (78).
#include "types.h"
#include "event_work.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "call.h"

extern char SceneId_Ov661Room10;
/// gMainRoutineWork.currentOwner by its own address.
extern s32 Data_02000454;

struct Actor *Actor_GetById(s32 id);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
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

static inline void CamBounds(s32 x, s32 a, s32 z, s32 b) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Camera_MoveWithinMapBounds(x, a, z, b);
}

void Func_02002ebc(void) {
  struct Actor *leader = Actor_GetById(Data_02000454);
  struct Actor *monster = Actor_GetById(8);

  Func_02004040();
  Func_020041b8(0);
  Func_020041f8(78);
  CamBounds(-1, -1, -1, 0);
  CamBounds(232 << 16, -1, 184 << 16, 0);
  Func_02003fd8();
  wait(1);
  Actor_SetSingleChildFlags(Actor_GetById(Data_02000454), 0);
  leader->motionFlags = 0;
  leader->y = 0x820000;
  leader->gravity = 0x8000;
  leader->bounce = 0;
  Actor_SetSingleChildFlags(Actor_GetById(8), 0);
  {
    s32 held = 0x4000;

    Call4((void (*)())Func_02004098, (s32)(8), (s32)(232 << 16), (s32)(148 << 16), (s32)(held));
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
  Actor_SetSingleChildFlags(Actor_GetById(Data_02000454), 1);
  FieldEvent_Wait(24);
  Func_02000b84(leader->x, leader->y, leader->z);
  Func_020041f8(188);
  Func_02004108(Data_02000454, 0x101);
  Motion_SetModeAndWaitAnimation(Data_02000454, 49);
  Func_020041f8(204);
  monster->motionFlags = 3;
  Actor_SetSingleChildFlags(Actor_GetById(8), 1);
  FieldEvent_Wait(24);
  Func_02000b84(monster->x, monster->y, monster->z);
  Func_020041f8(188);
  Actor_ArmCallback(Data_02000454, 0xc000, 0);
  FieldEvent_Wait(40);
  Func_020041f8(147);
  Actor_SetModeById(8, 3);
  FieldEvent_Wait(20);
  monster->velocityY = 0x80000;
  monster->gravity = 0xe666;
  Call3((void (*)())Actor_SetSpeedParameters, 8, 0x19999, 0xcccc);
  Actor_SetPositionAndCommit(8, 232, 184);
  { s32 v = 2; gMainRoutineWork.battleRule24b = v; }
  Party_SetFields1eeAnd1f0((s32)&SceneId_Ov661Room10, 2);
  Func_02004140(10, 0);
}
