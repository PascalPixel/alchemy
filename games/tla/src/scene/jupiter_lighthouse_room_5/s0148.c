#include "types.h"
#include "field_event.h"
#include "event_inline.h"

s32 Math_Cosine(u32 angle);

/// Circles the object around actor 23, as overlay 6a5's Func_02000090 circles
/// actor 31.
void Func_02000148(struct FieldActor *self) {
  struct FieldActor *centre = ((struct FieldActor *)Actor_Get(23));
  u16 angle = self->timer;
  s32 sine;
  s32 x;
  s32 z;

  self->x = centre->x + Math_Cosine(angle) * (self->speed + 3);
  sine = Math_Sin(angle);
  z = centre->z;
  x = self->x;
  z += sine * 2;
  self->z = z;
  self->targetX = x;
  /* FAKEMATCH: the target z store stays ahead of the timer load; without it the
   * store falls behind (6 differ; placed by alchemy permute) */
  do {
  } while (0);
  self->targetZ = z;
  *(s16 *)&self->timer += -0x800;
}
