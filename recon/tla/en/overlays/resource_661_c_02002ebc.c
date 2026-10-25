// Draft, differ 90 of 187: the leader and monster 8 drop into the sand pit,
// each in a ring of dust, the monster leaps to its spot and the battle starts
// with escape forbidden and the defeat destination set. Left: register
// choice and the native keeping 0 and 3 in r5 for the motion flags (permute
// found 18 by passing the 2 through a temporary, a register-allocation
// ripple rather than the source's form).
#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "party_state.h"
#include "call.h"

extern char SceneId_Ov661Room10;
/// gPartyState.currentOwner by its own address.
extern s32 Data_02000454;

struct ObjectRuntime *Object_GetById(s32 id);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object,
                                        s32 flags);
void Camera_MoveWithinMapBounds(s32 x, s32 a, s32 z, s32 b);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void FieldEvent_WaitFrames(s32 frames);
void WaitFrames(s32 frames);
void Motion_SetModeAndWaitAnimation(s32 id, s32 mode);
void ObjectMotion_ArmCallback(s32 id, s32 angle, s32 flags);
void ObjectMotion_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void ObjectMotion_SetPositionAndCommit(s32 id, s32 x, s32 z);
void Object_SetModeById(u32 id, s32 mode);
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
  struct ObjectRuntime *leader = Object_GetById(Data_02000454);
  struct ObjectRuntime *monster = Object_GetById(8);

  Func_02004040();
  Func_020041b8(0);
  Func_020041f8(78);
  CamBounds(-1, -1, -1, 0);
  CamBounds(232 << 16, -1, 184 << 16, 0);
  Func_02003fd8();
  WaitFrames(1);
  ObjectDispatch_SetSingleChildFlags(Object_GetById(Data_02000454), 0);
  leader->motionFlags = 0;
  leader->y = 0x820000;
  leader->gravity = 0x8000;
  leader->bounce = 0;
  ObjectDispatch_SetSingleChildFlags(Object_GetById(8), 0);
  Func_02004098(8, 232 << 16, 148 << 16, 0x4000);
  monster->motionFlags = 0;
  monster->y = 0x820000;
  monster->gravity = 0x8000;
  monster->bounce = 0;
  WaitFrames(1);
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x203;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Func_020041f8(204);
  leader->motionFlags = 3;
  ObjectDispatch_SetSingleChildFlags(Object_GetById(Data_02000454), 1);
  FieldEvent_WaitFrames(24);
  Func_02000b84(leader->x, leader->y, leader->z);
  Func_020041f8(188);
  Func_02004108(Data_02000454, 0x101);
  Motion_SetModeAndWaitAnimation(Data_02000454, 49);
  Func_020041f8(204);
  monster->motionFlags = 3;
  ObjectDispatch_SetSingleChildFlags(Object_GetById(8), 1);
  FieldEvent_WaitFrames(24);
  Call3((void (*)())Func_02000b84, monster->x, monster->y, monster->z);
  Func_020041f8(188);
  ObjectMotion_ArmCallback(Data_02000454, 0xc000, 0);
  FieldEvent_WaitFrames(40);
  Func_020041f8(147);
  Object_SetModeById(8, 3);
  FieldEvent_WaitFrames(20);
  monster->velocityY = 0x80000;
  monster->gravity = 0xe666;
  ObjectMotion_SetSpeedParameters(8, 0x19999, 0xcccc);
  ObjectMotion_SetPositionAndCommit(8, 232, 184);
  gPartyState.battleRule24b = 2;
  Party_SetFields1eeAnd1f0((s32)&SceneId_Ov661Room10, 2);
  Func_02004140(10, 0);
}
