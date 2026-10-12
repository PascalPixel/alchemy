/*
 * Draft of overlay 6a5 Func_02000090: an object circling object 31.
 * 2 instructions differ: the listing loads the timer step's constant before
 * the timer (ldr r1, =0xfffff800; ldrh r3), the draft after. The two empty
 * asm barriers below are trial devices:
 * without the first the shift comes before the second position read; without
 * the second the target store falls behind the timer load (6 differ).
 * Tried for the step: "-= 0x800", "+= -0x800" on the u16 field (builds
 * 0xf800 by a shift instead of the pool word), a signed halfword pointer, a
 * named step, and a barrier on the step (the constant moves to r2, 6 differ).
 */
#include "types.h"
#include "field_event.h"

struct FieldActor *Object_GetById();
s32 Math_Cosine();
s32 Math_Sine();

void Func_02000090(struct FieldActor *self)
{
    struct FieldActor *centre = Object_GetById(31);
    u16 angle = self->timer;
    s32 sine;
    s32 x;
    s32 z;

    self->x = centre->x + Math_Cosine(angle) * (self->speed + 3);
    sine = Math_Sine(angle);
    z = centre->z;
    x = self->x;
    asm volatile(""); /* FAKEMATCH: both positions are read before the sine is doubled; without it the shift comes before the second read */
    z += sine * 2;
    self->z = z;
    self->target_x = x;
    self->target_z = z;
    asm volatile(""); /* FAKEMATCH: the target store stays ahead of the timer load; without it the store falls behind (6 differ) */
    *(s16 *)&self->timer += -0x800;
}
