#include "types.h"
#include "iwram_call.h"
#include "actor.h"
#include "field_sprite.h"
#include "macros.h"
#include "event_inline.h"

s32 Math_Cosine(s32 angle);
void Func_020021b4(struct Actor *object, s32 a);

/// A particle's step: it circles at its angle (action) around x 214 and
/// rises with its phase (unk66), drawn in front while the sine is
/// negative and behind otherwise.
void Func_02000720(struct Actor *object) {
  s16 angle = object->action;
  s16 height = object->unk66;

  SET_POSITION(object,
               Iwram_MulQ16(Math_Cosine(angle), object->speedLimit) +
                   (214 << 16),
               (height * 3 << 16) + 0x80000, 170 << 18);
  // Read signed, as the phase is above; the field is declared unsigned.
  if ((s16)object->unk66 == 0)
    Func_020021b4(object, 7);
  if (object->y > 170 << 17)
    object->unk66 = 0xffff;
  if (Math_Sine(angle) < 0)
    ((struct FieldSprite *)object->animation)->priority = 1;
  else
    ((struct FieldSprite *)object->animation)->priority = 2;
  object->action += object->acceleration;
  object->unk66++;
}
