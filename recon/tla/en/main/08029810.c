/*
 * Draft of Func_08029810. 2 instructions differ: the listing halves the
 * second random number before the first (lsrs r0; lsrs r5), the draft the
 * first before the call returns. Tried: one expression and two named values.
 */
#include "types.h"
#include "field_event.h"

#define HALF_AT(base, offset) (*(u16 *)((u8 *)(base) + (offset)))

u32 Random16(void);

void Func_08029810(struct FieldActor *object)
{
    u32 first;
    u32 second;

    HALF_AT(object->sprite, 18) += 0x400;
    first = Random16();
    second = Random16();
    object->x += (first >> 1) - (second >> 1);
}
