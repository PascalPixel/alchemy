// Draft, differ 55 of 94: spawns a drifting particle (sprite 285) at an object:
// one of two scripts at random, an optional part attribute, a random drift (5
// to 14 steps of -6553) and spread (-7 to 7 steps of 13106), and the source
// sprite's priority. No ☀️ twin found. Left: the drift: the native multiplies
// drift + 5 by a pooled -6553 (muls); agscc synthesizes x * 6553 from shifts
// and adds and negates it (nine instructions), for drift as s32 or u32 and
// in one expression or two (in one expression it also distributes the + 5).
// The spread, multiplied by 6553 after its own doubling, does match as muls.
// Elsewhere register choice: the native keeps the attribute in r5 and then
// the shared zero for the two stores there, and the sprite in r8; this draft
// the other way round. The coin toss is written % 2 == 1 (the native keeps
// cmp #1; & 1 == 1 differs 57).
#include "types.h"
#include "field_sprite.h"
#include "object_runtime.h"

struct ObjectRuntime *Func_080200c0(s32 sprite, s32 x, s32 y, s32 z);
u32 Random16(void);
void ObjectDispatch_InitializeFar(struct ObjectRuntime *object,
                                  const void *script);
void Object_SetPartAttribute(struct ObjectRuntime *object, s32 attribute);
void Object_DriftAndDamp(void);

extern const u8 Data_080f085c[];
extern const u8 Data_080f0874[];

void Func_080d3378(struct ObjectRuntime *source, s32 attribute) {
  struct ObjectRuntime *object;
  struct FieldSprite *sprite;
  s32 drift;
  s32 spread;

  object = Func_080200c0(285, source->x, source->y, source->z);
  if (object == NULL)
    return;
  sprite = object->animation;
  if (Random16() % 2 == 1) {
    Object_SetMode(object, 2);
    ObjectDispatch_InitializeFar(object, Data_080f085c);
  } else {
    Object_SetMode(object, 1);
    ObjectDispatch_InitializeFar(object, Data_080f0874);
  }
  if (attribute != 0)
    Object_SetPartAttribute(object, attribute);
  object->motionFlags = 0;
  drift = Random16() % 10 + 5;
  object->acceleration = drift * -6553;
  spread = (Random16() % 15 - 7) * 2;
  object->speedLimit = spread * 6553;
  object->action = 0;
  object->priorityFlags |= 2;
  ((u8 *)object)[97] = 1;
  object->update = Object_DriftAndDamp;
  sprite->nextFrame = 0;
  sprite->priority = ((struct FieldSprite *)source->animation)->priority;
}
