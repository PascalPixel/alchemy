#include "airs_rock_inside.h"

s32 Math_Cosine(s32 angle);
s32 Math_Sine(s32 angle);

/// Circles the object around its centre (fields 68 and 76) on an ellipse twice
/// as wide as deep, the radius growing with its height; it rises by field 72
/// each frame and switches its part attribute between 0 and 9 every two frames.
void Func_02002c40(struct FieldActor *object) {
  s32 y = object->y;
  s32 phase;
  s32 radius;

  phase = WORD_AT(object, 48);
  radius = 0xc0000;
  /* FAKEMATCH(barrier): the radius constant is built before y >> 2, as the
   * native does; as radius = (y >> 2) + 0xc0000, 2 of 56 instructions differ
   * in tla-en. */
  __asm__("");
  radius += y >> 2;
  object->x = WORD_AT(object, 68) +
              Iwram_MulQ16(Math_Cosine((phase & 255) << 11), radius) * 2;
  object->z =
      WORD_AT(object, 76) +
      Iwram_MulQ16(Math_Sine((WORD_AT(object, 48) & 255) << 11), radius);
  object->y += WORD_AT(object, 72);
  WORD_AT(object, 48)++;
  Object_SetPartAttribute(object, ((gFrameCount & 2) >> 1) * 9);
}
