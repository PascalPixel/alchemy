#include "types.h"
#include "object_runtime.h"
#include "party_state.h"
#include "call.h"

void Camera_MoveWithinMapBounds();
void FieldEvent_WaitFrames();
void Func_02002698();
void Func_02003c28();
void Func_02003c30();
void Func_02003cc8();
void Func_02003cf0();
struct ObjectRuntime *Func_02003cf8();
void Func_02003d78();
void Func_02003df8();
void ObjectDispatch_SetSingleChildFlags();
void ObjectMotion_EnableActionAndResetMotion();
void ObjectMotion_SetAngleToward();
void ObjectMotion_SetPositionAndReset();
void ObjectMotion_SetSpeedParameters();
struct ObjectRuntime *Object_GetById();
void Object_LinkObjectAndSetCallback();

void Func_02002dd4(void) {
  struct ObjectRuntime *actor;

  actor = (struct ObjectRuntime *)Value1((s32 (*)())Object_GetById, 16);
  Func_02003c28();
  Call1(Func_02003d78, 0);
  Call2(Object_LinkObjectAndSetCallback, gPartyState.currentOwner, 16);
  ((struct ObjectRuntime *)Value0((s32 (*)())Func_02003cf8))->motionFlags = 0;
  Call4(Camera_MoveWithinMapBounds, 0x2f80000, 0x200000, 0xcc0000, 1);
  Func_02003cf0();
  Call3(ObjectMotion_SetAngleToward, 16, gPartyState.currentOwner, 0);
  // Called for its value, which is unused: as Call3 the actor id is loaded
  // before the 0x100's shift.
  Value3((s32 (*)())Func_02003cc8, 16, 0x100, 40);
  Call3(ObjectMotion_SetSpeedParameters, 16, 0x20000, 0x10000);
  Call1(Func_02003df8, 153);
  actor->velocityY = 0x80000;
  Call2(ObjectDispatch_SetSingleChildFlags,
        Value1((s32 (*)())Object_GetById, 16), 0);
  Call3(ObjectMotion_SetPositionAndReset, 16, 0x2f8, 200);
  Call2(ObjectDispatch_SetSingleChildFlags,
        Value1((s32 (*)())Object_GetById, 16), 1);
  Call1(FieldEvent_WaitFrames, 20);
  Call1(Func_02003df8, 153);
  actor->velocityY = 0x80000;
  Call2(ObjectDispatch_SetSingleChildFlags,
        Value1((s32 (*)())Object_GetById, 16), 0);
  Call3(ObjectMotion_SetPositionAndReset, 16, 0x318, 200);
  Call2(ObjectDispatch_SetSingleChildFlags,
        Value1((s32 (*)())Object_GetById, 16), 1);
  Call3(Func_02002698, 9, 16, 0x210);
  Call1(Func_02003df8, 153);
  actor->velocityY = 0x80000;
  Call2(ObjectDispatch_SetSingleChildFlags,
        Value1((s32 (*)())Object_GetById, 16), 0);
  Call3(ObjectMotion_SetPositionAndReset, 16, 0x338, 200);
  Call2(ObjectDispatch_SetSingleChildFlags,
        Value1((s32 (*)())Object_GetById, 16), 1);
  Call1(FieldEvent_WaitFrames, 20);
  Call2(Object_LinkObjectAndSetCallback, 16, gPartyState.currentOwner);
  Call1(ObjectMotion_EnableActionAndResetMotion, gPartyState.currentOwner);
  Call1(FieldEvent_WaitFrames, 20);
  Func_02003c30();
}
