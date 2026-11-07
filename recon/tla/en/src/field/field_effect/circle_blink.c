#include "types.h"
#include "field_event.h"
#include "iwram_call.h"

#define WORD_AT(base, offset) (*(s32 *)((u8 *)(base) + (offset)))

extern u32 gFrameCount;

s32 Math_Cosine(s32 angle);
s32 Math_Sine(s32 angle);
void Actor_SetPartAttribute(struct FieldActor *object, s32 value);

/// Circles the object around the centre in words 68 and 76, twice as wide
/// as it is deep, the radius growing with its height, one step of 256 per
/// lap, and blinks the part attribute every other frame. Overlay 663's
/// Func_020008ac is the same function.
void Effect_CircleAndBlink(struct FieldActor *object) {
  s32 radius = object->y;
  s32 step = WORD_AT(object, 48);

  /* FAKEMATCH: the barrier keeps both loads ahead of the radius; plainly the
   * radius's 0xc0000 is built before the step is loaded (differ 2). */
  __asm__("");
  radius >>= 2;
  radius += 0xc0000;
  object->x = WORD_AT(object, 68) +
              Iwram_MulQ16(Math_Cosine((step & 255) << 11), radius) * 2;
  object->z =
      WORD_AT(object, 76) +
      Iwram_MulQ16(Math_Sine((WORD_AT(object, 48) & 255) << 11), radius);
  object->y += WORD_AT(object, 72);
  WORD_AT(object, 48)++;
  Actor_SetPartAttribute(object, ((gFrameCount & 2) >> 1) * 9);
}
