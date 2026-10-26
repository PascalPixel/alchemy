#include "types.h"
#include "object_runtime.h"
#include "party_state.h"
#include "call.h"

struct ObjectRuntime *Object_GetById(s32 id);
s32 GameFlag_Test();
void GameFlag_ClearBit(s32 flag);
void WaitFrames(s32 frames);
void FieldEvent_WaitFrames(s32 frames);
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
  struct ObjectRuntime *object;
  s32 i;
  s32 y;
  s32 height;

  if (Value1(GameFlag_Test, 2576) + Value1(GameFlag_Test, 2578) +
          Value1(GameFlag_Test, 2580) + Value1(GameFlag_Test, 2577) +
          Value1(GameFlag_Test, 2579) + Value1(GameFlag_Test, 2581) ==
      0)
    return;
  Camera_MoveWithinMapBounds(148 << 18, -1, 248 << 16, 1);
  Func_02002bd4();
  FieldEvent_WaitFrames(10);
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
  WaitFrames(2);
  object = Object_GetById(18);
  /* FAKEMATCH(temporary): the heights read into locals before the test's
   * label, so they load ahead of the jump into the test; read in the test,
   * 14 of 222 instructions differ in tla-en. */
  y = object->y;
  height = object->terrainHeight;
  i = 0;
  goto test1;
wait1:
  WaitFrames(1);
  i++;
  if (i > 29)
    goto done1;
  y = object->y;
  height = object->terrainHeight;
test1:
  if (y > height || object->velocityY != 0)
    goto wait1;
done1:
  object->motionFlags = 0;
  object = Object_GetById(19);
  /* FAKEMATCH(temporary): the heights read into locals, as above. */
  y = object->y;
  height = object->terrainHeight;
  i = 0;
  goto test2;
wait2:
  WaitFrames(1);
  i++;
  if (i > 29)
    goto done2;
  y = object->y;
  height = object->terrainHeight;
test2:
  if (y > height || object->velocityY != 0)
    goto wait2;
done2:
  object->motionFlags = 0;
  FieldEvent_WaitFrames(30);
  Camera_FollowActor(gPartyState.currentOwner, 1);
  Func_02002bd4();
}
