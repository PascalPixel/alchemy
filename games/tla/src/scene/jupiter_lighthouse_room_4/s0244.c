#include "types.h"
#include "iwram_call.h"
#include "actor.h"
#include "field_sprite.h"
#include "macros.h"

s32 Math_Cosine(s32 angle);
s32 Math_Sine(s32 angle);
void Func_02001da8(struct Actor *object, s32 mode);

/// A particle's step, Jupiter Lighthouse room 2's Func_02000720 with other
/// numbers: it circles at its angle (action) around x 232 and rises with its
/// phase (unk66) from 120 pixels down, taking mode 7 at phase 1 and mode 2 at
/// phase 38 and starting over once above the ground; drawn in front while the
/// sine is negative and behind otherwise.
void Func_02000244(struct Actor *object) {
  s16 angle = object->action;
  s16 height = object->unk66;
  struct FieldSprite *sprite;
  s32 sine;

  SET_POSITION(object,
               Iwram_MulQ16(Math_Cosine(angle), object->speedLimit) +
                   (232 << 16),
               (height * 3 << 16) - (120 << 16), 130 << 18);
  if ((s16)object->unk66 == 1)
    Func_02001da8(object, 7);
  /* FAKEMATCH(one-pass): an empty one-pass block, after which the phase is
   * read again for the second test; as plain statements, 21 of 85
   * instructions differ in tla-en. */
  do {
  } while (0);
  if ((s16)object->unk66 == 38)
    Func_02001da8(object, 2);
  if (object->y > 0)
    object->unk66 = 0xffff;
  sine = Math_Sine(angle);
  sprite = object->animation;
  if (sine < 0)
    sprite->priority = 1;
  else
    sprite->priority = 3;
  object->action += object->acceleration;
  object->unk66++;
}
