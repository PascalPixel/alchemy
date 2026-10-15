#include "types.h"
#include "field_event.h"

struct FieldActor *Object_GetById();
s32 Math_Cosine();
s32 Math_Sine();

// Circles the object around actor 23, as overlay 6a5's Func_02000090 circles actor 31.
void Func_02000148(struct FieldActor *self)
{
    struct FieldActor *centre = Object_GetById(23);
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
    asm volatile(""); /* FAKEMATCH: the target z store stays ahead of the timer load; without it the store falls behind (6 differ; placed by alchemy permute) */
    self->target_z = z;
    *(s16 *)&self->timer += -0x800;
}
