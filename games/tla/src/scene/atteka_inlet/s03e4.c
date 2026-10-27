#include "types.h"
#include "object_runtime.h"
#include "party_state.h"
#include "text/msg_ids.h"
#include "call.h"

s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
struct ObjectRuntime *Object_GetById(s32 id);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
void Motion_SetModeAndWaitAnimation(s32 id, s32 mode);
void ObjectMotion_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void ObjectMotion_SetPositionAndReset(s32 id, s32 x, s32 z);
void ObjectMotion_ArmCallback(s32 id, s32 angle, s32 frames);
void FieldEvent_WaitTicks(s32 ticks);
void Func_02001010(void);
void Func_02001018(void);
void Func_02001098(s32 message);
void Func_020010a8(s32 id, s32 value);
void Func_020010b8(s32 id, s32 value);
s32 Func_020010c8(s32 id, s32 emote, s32 frames);
void Func_020010d0(s32 a, s32 b);
void Func_020010e0(void);
struct ObjectRuntime *Func_020010e8(void);
void Func_02001108(s32 value);

/// Once flag 0x9d0 is set, the camera pans over object 23, who greets the
/// party, turns, talks and walks to its place; flag 0x9b1 is set.
void Func_020003e4(void) {
  if (GameFlag_Test(0x9d0)) {
    Func_02001010();
    Call1(Func_02001108, 0);
    Call4(Camera_MoveWithinMapBounds, -1, -1, -1, 0);
    Call2(Func_020010d0, 0x26666, 0x4ccc);
    Call4(Camera_MoveWithinMapBounds, 0x2080000, -1, 0x1740000, 1);
    Func_020010e0();
    Value3(Func_020010c8, 23, 264, 0);
    Call3(ObjectMotion_ArmCallback, 23, 0x3000, 20);
    Call1(Func_02001098, (s32)MsgIVeBeenWaiting);
    Call2(Func_020010a8, 23, 0);
    Call2(Func_020010b8, 23, 0xb000);
    Call2(Func_020010a8, 0x2017, 0);
    Func_020010e8()->motionFlags = 0;
    Call2(Func_020010d0, 0x4cccc, 0x9999);
    Call4(Camera_MoveWithinMapBounds, 0x1b80000, 0x200000, 0x1700000, 1);
    Func_020010e0();
    Call1(FieldEvent_WaitTicks, 40);
    Call4(Camera_MoveWithinMapBounds, 0x1b00000, 0x200000, 0xe80000, 1);
    Func_020010e0();
    Call1(FieldEvent_WaitTicks, 40);
    Call2(Func_020010d0, 0x4cccc, 0x9999);
    Call4(Camera_MoveWithinMapBounds, 0x2080000, 0x200000, 0x1740000, 1);
    Func_020010e0();
    Call1(FieldEvent_WaitTicks, 20);
    Call2(Func_020010b8, 23, 0x3000);
    Call2(Motion_SetModeAndWaitAnimation, 23, 3);
    Call2(Func_020010a8, 23, 0);
    Value3(Func_020010c8, 23, 265, 20);
    Call2(Func_020010a8, 23, 0);
    Call2(Object_SetModeById, 23, 4);
    Call2(Func_020010a8, 23, 0);
    Call2(Func_020010a8, 23, 0);
    Call2(Func_020010b8, 23, 0xb000);
    Call2(Func_020010a8, 0x2017, 0);
    Call2(Func_020010b8, 23, 0x3000);
    Call2(Func_020010a8, 23, 0);
    Call2(Motion_SetModeAndWaitAnimation, 23, 3);
    Call2(Func_020010a8, 23, 0);
    Call2(Object_SetModeById, 23, 4);
    Call2(Func_020010a8, 23, 0);
    Value3(Func_020010c8, gPartyState.currentOwner, 257, 40);
    Value3(Func_020010c8, 23, 265, 20);
    Call2(Func_020010a8, 23, 0);
    Value3(Func_020010c8, 23, 264, 20);
    Call2(Func_020010a8, 23, 0);
    Call2(Motion_SetModeAndWaitAnimation, 23, 3);
    Call2(Func_020010a8, 23, 0);
    Call3(ObjectMotion_SetSpeedParameters, 23, 0xcccc, 0x6666);
    Object_GetById(23)->actionFlags &= ~1;
    Call3(ObjectMotion_SetPositionAndReset, 23, 504, 360);
    Call1(FieldEvent_WaitTicks, 1);
    Object_GetById(23)->actionFlags |= 1;
    Call2(Func_020010b8, 23, 0x3000);
    Call2(Motion_SetModeAndWaitAnimation, 23, 3);
    Call2(Func_020010a8, 23, 0);
    Call1(GameFlag_SetBit, 0x9b1);
    Func_02001018();
  }
}
