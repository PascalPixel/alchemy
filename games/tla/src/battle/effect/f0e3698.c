#include "types.h"
#include "fixed_math.h"
#include "object_runtime.h"

struct ObjectRuntime *Object_GetById(u32 actor);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
struct ObjectRuntime *Object_Spawn(s32 kind, s32 x, s32 y, s32 z);
void Event_Start(void);
void Event_ResolvePendingActions(s32 skip);
void Event_StartScreenTransition(void);
void Vector_AddPolarOffset(s32 magnitude, s32 angle, s32 *position);
void WaitFrames(s32 frames);
void Object_SetPositionAndResetMotionFar(struct ObjectRuntime *object, s32 x,
                                         s32 y, s32 z);
void Event_WaitFrames(s32 frames);
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
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_StartScreenTransition();
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
  Sound_PlayFar(136);
  Object_SetMode(ring, 6);
  Event_WaitFrames(15);
  Object_Destroy(ring);
  object->update = NULL;
  object->angle = angle;
  object->motionFlags = motionFlags;
  WaitFrames(1);
  Camera_FollowActor(actor, 1);
}
