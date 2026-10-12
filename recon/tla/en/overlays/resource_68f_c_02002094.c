/*
 * Draft of overlay 68f Func_02002094: places each door object of a table
 * that ends on 0xffff. Same cause as Func_02000404: the listing builds 0xffff
 * again at the bottom of the loop, the draft keeps it in r10.
 */
#include "types.h"
#include "field_event.h"

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))

struct FieldActor *Object_GetById();
void ObjectDispatch_SetSingleChildFlags();
void Func_02002184();

void Func_02002094(u16 *row)
{
    while (row[0] != 0xffff) {
        s32 id = row[0];
        struct FieldActor *object = Object_GetById(id);
        s16 x = row[1];
        s16 z = row[2];

        object->collision_flags &= 0xfe;
        ObjectDispatch_SetSingleChildFlags(object, 0);
        row += 3;
        Func_02002184(id, (u16)x, (u16)z, BYTE_AT(object, 34));
    }
}
