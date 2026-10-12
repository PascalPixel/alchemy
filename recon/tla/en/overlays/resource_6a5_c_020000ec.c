/*
 * Draft of overlay 6a5 Func_020000ec: an object circling object 36.
 * 2 instructions differ: the listing reloads the centre (mov r1, r8) before
 * the radius is finished and multiplied, the draft after the multiply.
 * The multiply takes the cosine first, and the timer steps through a signed
 * halfword pointer: with "-= 0x800" on the u16 field the step's constant is
 * loaded after the timer (4 differ).
 */
#include "types.h"
#include "field_event.h"

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))

struct FieldActor *Object_GetById();
s32 Math_Cosine();
s32 Math_Sine();

void Func_020000ec(struct FieldActor *self)
{
    struct FieldActor *centre = Object_GetById(36);
    u16 angle = self->timer;

    self->x = centre->x + Math_Cosine(angle) * (self->speed + BYTE_AT(self, 98) + 6);
    self->z = centre->z + Math_Sine(angle) * (BYTE_AT(self, 98) + 4);
    self->target_x = self->x;
    self->target_z = self->z;
    *(s16 *)&self->timer += -0x800;
}
