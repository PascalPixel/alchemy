#include "types.h"
#include "iwram_call.h"
#include "actor.h"
#include "field_sprite.h"
#include "macros.h"

s32 Math_Cosine(s32 angle);
s32 Math_Sine(s32 angle);
void Func_020021b4(struct Actor *object, s32 a);

/// A particle's step: it circles at its angle (action) around x 214 and
/// rises with its phase (unk66), drawn in front while the sine is
/// negative and behind otherwise.
void Func_02000720(struct Actor *object) {
  s16 angle = object->action;
  s16 height = object->unk66;
  struct FieldSprite *sprite;
  s32 sine;
  s32 y;

  SET_POSITION(object,
               Iwram_MulQ16(Math_Cosine(angle), object->speedLimit) +
                   (214 << 16),
               (height * 3 << 16) + 0x80000, 170 << 18);
  if ((s16)object->unk66 == 0)
    Func_020021b4(object, 7);
  /* FAKEMATCH: the one-pass block loads the height before the limit is built,
   * as the native does (one reordered row without it). */
  do {
    y = object->y;
  } while (0);
  if (y > 170 << 17)
    object->unk66 = 0xffff;
  sine = Math_Sine(angle);
  sprite = object->animation;
  if (sine < 0)
    sprite->priority = 1;
  else
    sprite->priority = 2;
  object->action += object->acceleration;
  object->unk66++;
}
