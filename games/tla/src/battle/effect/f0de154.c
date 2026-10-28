#include "types.h"
#include "system.h"
#include "objdisp.h"
#include "object_effect_scripts.h"
#include "object_runtime.h"

struct ImpactVector {
  s32 x;
  s32 y;
  s32 z;
};

extern const s32 gData_080f0e78[];

struct ParticleTimer {
  u8 unk00[0x5e];
  u16 timer;
};

struct ObjectRuntime *Object_Spawn(s32 kind, s32 x, s32 y, s32 z);
void Motion_SetTargetPositionFromMagnitudeAngle(struct ObjectRuntime *object,
                                                s32 magnitude, s32 angle);

s32 Func_080de154(struct ObjectRuntime *src) {
  struct ImpactVector pos;
  struct ObjectRuntime *obj;
  struct ObjectRuntime *frag;
  s32 i;

  Sound_PlayFar(134);
  pos.x = src->x;
  pos.y = src->y;
  pos.z = src->z;
  obj = Object_Spawn(671, pos.x, pos.y - (32 << 16), pos.z);
  if (obj != NULL) {
    obj->motionFlags = 0;
    ((struct ParticleTimer *)obj)->timer = 20;
    ObjectDispatch_InitializeFar((struct DispatchObject *)obj,
                                 gBattleEffect_SharedParticleScript);
  }

  for (i = 0; i < 12; i++) {
    frag = Object_Spawn(673, pos.x, pos.y, pos.z);
    if (frag != NULL) {
      ObjectDispatch_InitializeFar((struct DispatchObject *)frag,
                                   gData_080f0e78);
      frag->speedLimit = Random16() + (1 << 16);
      frag->acceleration = 1 << 16;
      frag->motionFlags = 0;
      Motion_SetTargetPositionFromMagnitudeAngle(
          frag, Random16() * 24 + (8 << 16), Random16());
    }
  }
  return 0;
}
