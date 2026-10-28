#include "types.h"
#include "field_event.h"

struct FieldActor *Actor_GetById();
s32 Math_Cosine();
s32 Math_Sine();

void Func_02000090(struct FieldActor *self) {
  struct FieldActor *centre = Actor_GetById(31);
  u16 angle = self->timer;
  s32 sine;
  s32 x;
  s32 z;

  self->x = centre->x + Math_Cosine(angle) * (self->speed + 3);
  sine = Math_Sine(angle);
  z = centre->z;
  x = self->x;
  /* FAKEMATCH: both positions are read before the sine is doubled; without it
   * the shift comes before the second read */
  asm volatile("");
  z += sine * 2;
  self->z = z;
  self->targetX = x;
  /* FAKEMATCH: the target z store stays ahead of the timer load; without it,
   * 6 instructions differ in tla-en. */
  asm volatile("");
  self->targetZ = z;
  *(s16 *)&self->timer += -0x800;
}

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))

/// Circles object 36, wider across than deep, by the actor's speed and its
/// byte at 98.
// As ☀️'s SceneEffect_UpdateCounterDrivenOrbit does.
void Func_020000ec(struct FieldActor *self) {
  struct FieldActor *centre = Actor_GetById(36);
  u16 angle = self->timer;
  s32 cosine = Math_Cosine(angle);
  s32 radius = self->speed + BYTE_AT(self, 98);
  /* FAKEMATCH: the centre pinned to r1 behind an empty asm is copied out of r8
   * ahead of the + 6, as native; plainly, 2 instructions differ in tla-en. */
  register struct FieldActor *anchor asm("r1") = centre;

  asm volatile("" : "+r"(anchor));
  self->x = anchor->x + cosine * (radius + 6);
  self->z = centre->z + Math_Sine(angle) * (BYTE_AT(self, 98) + 4);
  self->targetX = self->x;
  self->targetZ = self->z;
  *(s16 *)&self->timer += -0x800;
}
