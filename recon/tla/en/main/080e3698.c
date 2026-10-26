// Under -mthumb-slow-loads (2026-10-26): differ 4 of 141 in tla-en, instruction
// order only. After a store the native builds a constant's half before the next
// load, where alchemy-gcc issues the load at once (AGENTS.md's recurring row);
// no empty asm placement lowers it.
// Near miss, differ 4 of 141, instruction order only: in the loop the
// native loads the new y before the 0x400 step's shift and sets up
// WaitFrames' 1 before the z copy; here a row later and earlier. The saved
// facing as an s32 measured 4, as a u16 or s16 local 84.
// An event sequence that spins an actor up and back down. The camera
// stops following, the actor takes BattleEffect_SpinObject as its update, and a
// stretched object 280 rides six units below it while for 128 frames the
// actor rises and falls 96 units on a cosine and circles its start on a
// 32-unit cosine radius, turning 0x400 a frame. It then lands where it
// started (cue 136), object 280 plays mode 6 and is destroyed, the actor's
// update, facing and motion flags are restored and the camera follows it.
#include "types.h"
#include "fixed_math.h"
#include "object_runtime.h"

struct ObjectRuntime *Object_GetById(u32 actor);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
struct ObjectRuntime *Object_Spawn(s32 kind, s32 x, s32 y, s32 z);
void EventRuntime_Start(void);
void EventRuntime_ResolvePendingActions(s32 skip);
void EventRuntime_StartScreen(void);
void Vector_AddPolarOffset(s32 magnitude, s32 angle, s32 *position);
void WaitFrames(s32 frames);
void Object_SetPositionAndResetMotionFar(struct ObjectRuntime *object, s32 x,
                                         s32 y, s32 z);
void EventRuntime_Wait(s32 frames);
void Object_Destroy(struct ObjectRuntime *object);
void Camera_FollowActor(s32 actor, s32 mode);
void BattleEffect_SpinObject(void);

void Func_080e3698(s32 actor) {
  struct ObjectRuntime *object = Object_GetById(actor);
  s32 angle = object->angle;
  u8 motionFlags = object->motionFlags;
  struct ObjectRuntime *ring;
  s32 x, y, z;
  s32 spin;
  s32 position[3];
  s32 i;

  Camera_MoveWithinMapBounds(-1, -1, -1, 0);
  object->motionFlags = 0;
  object->update = BattleEffect_SpinObject;
  ring = Object_Spawn(280, object->x, object->y, object->z);
  Object_SetMode(ring, 2);
  ring->scaleX = 0x18000;
  ring->scaleY = 0x10000;
  x = object->x;
  y = object->y;
  z = object->z;
  spin = 0;
  EventRuntime_Start();
  EventRuntime_ResolvePendingActions(0);
  EventRuntime_StartScreen();
  for (i = 0; i < 128; i++) {
    position[0] = x;
    position[1] = y + Trig_Cos(i << 7) * 96;
    position[2] = z;
    Vector_AddPolarOffset(Trig_Cos(i << 7) << 5, spin, position);
    object->x = position[0];
    object->y = position[1];
    object->z = position[2];
    spin += 0x400;
    ring->x = object->x;
    ring->y = object->y - 0x60000;
    ring->z = object->z;
    WaitFrames(1);
  }
  Object_SetPositionAndResetMotionFar(object, x, y, z);
  Audio_PlayCue(136);
  Object_SetMode(ring, 6);
  EventRuntime_Wait(15);
  Object_Destroy(ring);
  object->update = NULL;
  object->angle = angle;
  object->motionFlags = motionFlags;
  WaitFrames(1);
  Camera_FollowActor(actor, 1);
}
