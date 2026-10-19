/// @file
/// Effects: motion of the main object in an item-breaking effect.

#include "types.h"
#include "edition.h"
#include "objdisp.h"
#include "fixed_math.h"
#include "object_effect_scripts.h"
#include "system.h"
#include "field_event.h"

struct ItemBreakFragmentPosition {
  s32 x;
  s32 y;
  s32 z;
};

struct ItemBreakFragmentSource {
  u8 reserved00[6];
  u16 angle; ///< Facing angle (0x10000 is one turn).
  s32 x;
  s32 y;
  s32 z;
  u8 reserved14[70];
  u8 flag5a;
  u8 reserved5b[13];
  struct ItemBreakFragmentSource *target; ///< Facing target.
};

struct ItemBreakFragmentObject {
  u8 reserved00[72];
  s32 field48; ///< Gravity (same offset as gravity).
  u8 reserved4c[9];
  u8 mode55; ///< Same offset as motionFlags.
  u8 reserved56[8];
  u16 field5e;
};

extern s32 ArcTan2(s32, s32);

extern void Vector_AddPolarOffset(s32, s32, struct ItemBreakFragmentPosition *);
extern struct ItemBreakFragmentObject *Object_Spawn(s32, s32, s32, s32);

/// Update the broken item's main object.
/// If a facing target exists, turn toward it by at most 0x1000 (22.5 degrees),
/// then emit one fragment particle nearby.
/// src: main object.
void BattleFx_UpdateItemBreakFragment(struct ItemBreakFragmentSource *src) {
  struct ItemBreakFragmentSource *target;
  struct ItemBreakFragmentPosition pos;
  struct ItemBreakFragmentObject *obj;
  s32 turn;
  s32 dist;

  target = src->target;
  if (target != 0) {
    s32 dx = target->x - src->x;
    s32 dz = target->z - src->z;

    if (dx != 0 || dz != 0) {
      s32 dir = ArcTan2(dz, dx);
      s32 cur = src->angle;
      s32 lim = 128;

      turn = (s16)(dir - cur);
      lim <<= 5;
      if (turn > lim)
        turn = lim;
      if (turn < -0x1000)
        turn = -0x1000;
      src->angle += turn;
    }
    src->flag5a = 0;
  }

  // Emit the particle with a y offset of 8-24 and a random direction
  // at radius 0-48 from the main object.
  pos.x = src->x;
  pos.y = src->y - (Random16() << 4) - 0x80000;
  pos.z = src->z;
  dist = Random16() * 3;
  dist <<= 4;
  Vector_AddPolarOffset(dist, Random16(), &pos);

  obj = Object_Spawn(0x2a1, pos.x, pos.y, pos.z);
  if (obj != 0) {
    obj->mode55 = 2;
    obj->field48 = 0x1999;
    Object_SetMode(obj, 0);
    obj->field5e = 12;
    ObjectDispatch_InitializeFar((struct DispatchObject *)obj,
                                 gBattleFx_CommonParticleScript);
  }
}
