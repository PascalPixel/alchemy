#include "types.h"
#include "actor.h"
#include "sequence_parent_work.h"
#include "call.h"

struct Actor *Actor_GetById(s32 id);
s32 GameFlag_Test();
void GameFlag_ClearBit(s32 flag);
void wait(s32 frames);
void FieldEvent_Wait(s32 frames);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
void Camera_FollowActor(s32 id, s32 flag);
void Func_02002bd4(void);
void Func_020009f8(s32 id, s32 x, s32 z, s32 dz, s32 dx);
void Func_020002f0(void);

/// Drops objects 18 and 19 once any of flags 2576 to 2581 is set: the camera
/// moves to (592, 248), each object drops (Func_020009f8) at the spot whose
/// flag is set ((35, 15), (31, 16) or (33, 10)), the six flags are cleared,
/// Func_020002f0 resets the room, both objects are waited on until they land
/// (at most 30 frames each) and stop, and the camera follows the leader again.
void Func_02000a74(void) {
  struct Actor *object;
  s32 i;

  if (Value1(GameFlag_Test, 2576) + Value1(GameFlag_Test, 2578) +
          Value1(GameFlag_Test, 2580) + Value1(GameFlag_Test, 2577) +
          Value1(GameFlag_Test, 2579) + Value1(GameFlag_Test, 2581) ==
      0)
    return;
  Camera_MoveWithinMapBounds(148 << 18, -1, 248 << 16, 1);
  Func_02002bd4();
  FieldEvent_Wait(10);
  if (Value1(GameFlag_Test, 2576))
    Func_020009f8(18, 35, 15, -16, 0);
  if (Value1(GameFlag_Test, 2577))
    Func_020009f8(19, 35, 15, -16, 0);
  if (Value1(GameFlag_Test, 2578))
    Func_020009f8(18, 31, 16, 16, 0);
  if (Value1(GameFlag_Test, 2579))
    Func_020009f8(19, 31, 16, 16, 0);
  if (Value1(GameFlag_Test, 2580))
    Func_020009f8(18, 33, 10, -16, 0);
  if (Value1(GameFlag_Test, 2581))
    Func_020009f8(19, 33, 10, -16, 0);
  GameFlag_ClearBit(2576);
  GameFlag_ClearBit(2577);
  GameFlag_ClearBit(2578);
  GameFlag_ClearBit(2579);
  GameFlag_ClearBit(2580);
  GameFlag_ClearBit(2581);
  Func_020002f0();
  wait(2);
  object = Actor_GetById(18);
  i = 0;
  if (object->y > object->terrainHeight || object->velocityY != 0) {
    do {
      wait(1);
      i++;
      if (i > 29)
        break;
    } while (object->y > object->terrainHeight || object->velocityY != 0);
  }
  object->motionFlags = 0;
  object = Actor_GetById(19);
  i = 0;
  if (object->y > object->terrainHeight || object->velocityY != 0) {
    do {
      wait(1);
      i++;
      if (i > 29)
        break;
    } while (object->y > object->terrainHeight || object->velocityY != 0);
  }
  object->motionFlags = 0;
  FieldEvent_Wait(30);
  Camera_FollowActor(gSequenceParentWork.currentOwner, 1);
  Func_02002bd4();
}
