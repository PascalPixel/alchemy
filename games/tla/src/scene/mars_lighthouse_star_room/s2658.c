#include "mars_lighthouse_star_room.h"
#include "iwram_call.h"

extern const u8 gData_020047b8[];

struct ObjectRuntime *Func_020041e4(s32 kind, s32 x, s32 y, s32 z);
void Func_020041dc(struct ObjectRuntime *object, const void *script);
s32 Math_Cosine(s32 angle);
s32 Math_Sine(s32 angle);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object,
                                        s32 flags);
void Animation_ApplyChildValues(struct ObjectRuntime *object, s32 value);

/// Throws two sparks (object kind 325) from (x, y, z), each upward at about
/// speed plus 0x14000 with a random spread, at a random angle and size.
void Func_02002658(s32 x, s32 y, s32 z, s32 speed) {
  s32 i;

  /* FAKEMATCH(barrier): the four arguments are spilled before the counter is
   * set, as in the native; without it, 4 of 98 instructions differ in
   * tla-en. */
  asm("");
  i = 1;
next:
  {
    struct ObjectRuntime *object = Func_020041e4(325, x, y, z);

    if (object != NULL) {
      s32 angle;
      s32 rise;
      s32 radius;
      u32 size;
      s32 scale;

      rise = speed + ((u32)Random16Far() >> 4) - ((u32)Random16Far() >> 4);
      angle = 0x4000 + rise;
      radius = Random16Far() * 8;
      size = Random16Far();
      Func_020041dc(object, gData_020047b8);
      ObjectDispatch_SetSingleChildFlags(object, 0);
      /* FAKEMATCH(temporary): the scale is taken before the velocity stores, as
       * in the native; as object->scaleX = (size >> 1) + 0x4000, 14 of 98
       * instructions differ in tla-en. */
      scale = (size >> 1) + 0x4000;
      object->velocityY = rise + 0x14000;
      object->velocityZ = Iwram_MulQ16(radius, Math_Cosine(angle));
      object->velocityX = Iwram_MulQ16(radius, Math_Sine(angle));
      object->acceleration = 0;
      object->gravity = 0x547a;
      object->scaleX = scale;
      object->scaleY = object->scaleX;
      object->bounce = 0x8000;
      Animation_ApplyChildValues(object, 1);
    }
  }
  if (--i >= 0)
    goto next;
}
