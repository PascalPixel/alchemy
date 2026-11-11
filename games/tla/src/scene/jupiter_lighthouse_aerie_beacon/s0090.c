#include "types.h"
#include "field_event.h"
#include "event_inline.h"

s32 Math_Cosine();

void Func_02000090(struct FieldActor *self) {
  struct FieldActor *centre = ((struct FieldActor *)Actor_Get(31));
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
  /* FAKEMATCH(dead): an empty once-only block, so the target z store stays
   * ahead of the timer load; without it, 6 instructions differ in tla-en. */
  do {
  } while (0);
  self->targetZ = z;
  *(s16 *)&self->timer += -0x800;
}

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))

/// Circles object 36, wider across than deep, by the actor's speed and its
/// byte at 98.
// As ☀️'s SceneEffect_UpdateCounterDrivenOrbit does.
void Func_020000ec(struct FieldActor *self) {
  struct FieldActor *centre = ((struct FieldActor *)Actor_Get(36));
  u16 angle = self->timer;
  s32 cosine = Math_Cosine(angle);
  s32 radius = self->speed + BYTE_AT(self, 98);
  /* FAKEMATCH(pin): the centre pinned to r1, which with the barrier below
   * copies it out of r8 ahead of the + 6, as the native does; as centre used
   * plainly, with neither device, 2 instructions differ in tla-en. */
  register struct FieldActor *anchor asm("r1") = centre;

  /* FAKEMATCH(barrier): an asm operand that says the centre's copy changes,
   * which it does not; with the pin above, as centre used plainly, 2
   * instructions differ in tla-en. */
  asm volatile("" : "+r"(anchor));
  self->x = anchor->x + cosine * (radius + 6);
  self->z = centre->z + Math_Sin(angle) * (BYTE_AT(self, 98) + 4);
  self->targetX = self->x;
  self->targetZ = self->z;
  *(s16 *)&self->timer += -0x800;
}
