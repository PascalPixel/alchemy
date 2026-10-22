#include "fixed_math.h"
#include "objdisp.h"
#include "types.h"
#include "object_effect_scripts.h"
#include "system.h"

struct ItemBreakFragmentPosition {
  s32 x;
  s32 y;
  s32 z;
};

struct ItemBreakFragmentSource {
  u8 reserved00[6];
  u16 angle;
  s32 x;
  s32 y;
  s32 z;
  u8 reserved14[70];
  u8 flag5a;
  u8 reserved5b[13];
  struct ItemBreakFragmentSource *target;
};

struct ItemBreakFragmentObject {
  u8 reserved00[72];
  s32 field48;
  u8 reserved4c[9];
  u8 mode55;
  u8 reserved56[8];
  u16 field5e;
};

extern s32 ArcTan2(s32, s32);
extern void Vector_AddPolarOffset(s32, s32, struct ItemBreakFragmentPosition *);
extern struct ItemBreakFragmentObject *Object_Spawn(s32, s32, s32, s32);
extern void Object_SetMode(struct ItemBreakFragmentObject *, s32);

void BattleEffect_UpdateItemBreakFragment(struct ItemBreakFragmentSource *source) {
  struct ItemBreakFragmentSource *target;
  struct ItemBreakFragmentPosition position;
  struct ItemBreakFragmentObject *object;
  s32 steeringDelta;
  s32 driftMagnitude;

  target = source->target;
  if (target != 0) {
    s32 dx = target->x - source->x;
    s32 dz = target->z - source->z;

    if (dx != 0 || dz != 0) {
      steeringDelta = (s16)(ArcTan2(dz, dx) - source->angle);
      if (steeringDelta > 0x1000)
        steeringDelta = 0x1000;
      if (steeringDelta < -0x1000)
        steeringDelta = -0x1000;
      source->angle += steeringDelta;
    }
    source->flag5a = 0;
  }

  position.x = source->x;
  position.y = source->y - (Random16() << 4) - 0x80000;
  position.z = source->z;
  driftMagnitude = Random16() * 3;
  driftMagnitude <<= 4;
  Vector_AddPolarOffset(driftMagnitude, Random16(), &position);

  object = Object_Spawn(0x11D, position.x, position.y, position.z);
  if (object != 0) {
    object->mode55 = 2;
    object->field48 = 0x1999;
    Object_SetMode(object, 0);
    object->field5e = 12;
    ObjectDispatch_InitializeFar((struct DispatchObject *)object,
                                 (u32)gBattleEffect_CommonParticleScript);
  }
}
