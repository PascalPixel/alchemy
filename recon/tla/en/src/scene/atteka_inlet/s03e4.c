#include "types.h"
#include "scene_tables.h"
#include "actor.h"
#include "main_routine_work.h"
#include "text/msg_ids.h"
#include "call.h"

s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
struct Actor *Actor_GetById(s32 id);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
void Motion_SetModeAndWaitAnimation(s32 id, s32 mode);
void Actor_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void Actor_SetPositionAndReset(s32 id, s32 x, s32 z);
void Actor_ArmCallback(s32 id, s32 angle, s32 frames);
void FieldEvent_Wait(s32 frames);
void Func_02001010(void);
void Func_02001018(void);
void Func_02001098(s32 message);
void Func_020010a8(s32 id, s32 value);
void Func_020010b8(s32 id, s32 value);
s32 Func_020010c8(s32 id, s32 emote, s32 frames);
void Func_020010d0(s32 a, s32 b);
void Func_020010e0(void);
struct Actor *Func_020010e8(void);
void Func_02001108(s32 value);

/// Once flag 0x9d0 is set, the camera pans over object 23, who greets the
/// party, turns, talks and walks to its place; flag 0x9b1 is set.
void Func_020003e4(void) {
  if (GameFlag_Test(0x9d0)) {
    Func_02001010();
    Func_02001108(0);
    Call4(Camera_MoveWithinMapBounds, -1, -1, -1, 0);
    Func_020010d0(0x26666, SPLIT_CONSTANT(0x4c00, 204));
    Call4(Camera_MoveWithinMapBounds, PIXELS(520), -1, PIXELS(372), 1);
    Func_020010e0();
    Value3(Func_020010c8, 23, 264, 0);
    Call3(Actor_ArmCallback, 23, 0x3000, 20);
    Func_02001098((s32)MsgIVeBeenWaiting);
    Func_020010a8(23, 0);
    Call2(Func_020010b8, 23, 0xb000);
    Call2(Func_020010a8, SPLIT_CONSTANT(0x2000, 23), 0);
    Func_020010e8()->motionFlags = 0;
    Call2(Func_020010d0, 0x4cccc, SPLIT_CONSTANT(0x9900, 153));
    Call4(Camera_MoveWithinMapBounds, PIXELS(440), PIXELS(32), PIXELS(368), 1);
    Func_020010e0();
    FieldEvent_Wait(40);
    Call4(Camera_MoveWithinMapBounds, PIXELS(432), PIXELS(32), PIXELS(232), 1);
    Func_020010e0();
    FieldEvent_Wait(40);
    Func_020010d0(0x4cccc, SPLIT_CONSTANT(0x9900, 153));
    Camera_MoveWithinMapBounds(PIXELS(520), PIXELS(32), PIXELS(372), 1);
    Func_020010e0();
    FieldEvent_Wait(20);
    Call2(Func_020010b8, 23, 0x3000);
    Motion_SetModeAndWaitAnimation(23, 3);
    Func_020010a8(23, 0);
    Value3(Func_020010c8, 23, 265, 20);
    Func_020010a8(23, 0);
    Actor_SetModeById(23, 4);
    Func_020010a8(23, 0);
    Func_020010a8(23, 0);
    Call2(Func_020010b8, 23, 0xb000);
    Func_020010a8(SPLIT_CONSTANT(0x2000, 23), 0);
    Call2(Func_020010b8, 23, 0x3000);
    Func_020010a8(23, 0);
    Motion_SetModeAndWaitAnimation(23, 3);
    Func_020010a8(23, 0);
    Actor_SetModeById(23, 4);
    Func_020010a8(23, 0);
    Func_020010c8(gMainRoutineWork.currentOwner, 257, 40);
    Func_020010c8(23, 265, 20);
    Func_020010a8(23, 0);
    Func_020010c8(23, 264, 20);
    Func_020010a8(23, 0);
    Motion_SetModeAndWaitAnimation(23, 3);
    Func_020010a8(23, 0);
    Actor_SetSpeedParameters(23, SPLIT_CONSTANT(0xcc00, 204), SPLIT_CONSTANT(0x6600, 102));
    Actor_GetById(23)->actionFlags &= ~1;
    Actor_SetPositionAndReset(23, 504, 360);
    FieldEvent_Wait(1);
    Actor_GetById(23)->actionFlags |= 1;
    Func_020010b8(23, 0x3000);
    Motion_SetModeAndWaitAnimation(23, 3);
    Func_020010a8(23, 0);
    GameFlag_SetBit(SPLIT_CONSTANT(0x900, 177));
    Func_02001018();
  }
}
