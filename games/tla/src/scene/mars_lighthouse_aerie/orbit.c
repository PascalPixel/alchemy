#include "types.h"
#include "field_event.h"
#include "flag_call.h"

struct FieldActor *Object_GetById(s32 id);
s32 Math_Sine(u32 angle);
void Func_02003210(struct FieldActor *self);
s32 Math_Cosine(u32 angle);

/// Circles the actor around actor 23 by its timer, at its speed plus 3 in x
/// and twice the sine in z, and turns the timer back by 0x800; Func_02000148
/// of the Jupiter Lighthouse's fifth room is the same function.
void Func_02003210(struct FieldActor *self) {
  struct FieldActor *centre = Object_GetById(23);
  u16 angle = self->timer;
  s32 sine;
  s32 x;
  s32 z;

  self->x = centre->x + Math_Cosine(angle) * (self->speed + 3);
  sine = Math_Sine(angle);
  z = centre->z;
  x = self->x;
  /* FAKEMATCH(barrier): both positions are read before the sine is doubled;
   * without it, 2 of 40 instructions differ in tla-en. */
  asm volatile("");
  z += sine * 2;
  self->z = z;
  self->targetX = x;
  /* FAKEMATCH(barrier): the target z store stays ahead of the timer load;
   * without it, 2 of 40 instructions differ in tla-en. */
  asm volatile("");
  self->targetZ = z;
  *(s16 *)&self->timer += -0x800;
}
