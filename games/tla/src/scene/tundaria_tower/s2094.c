#include "types.h"
#include "field_event.h"

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))

struct FieldActor *Object_GetById();
void ObjectDispatch_SetSingleChildFlags();
void Func_02002184();

void Func_02002094(u16 *table) {
  u16 *row;
  s32 id;
  struct FieldActor *object;
  s16 x, z;

  if (table[0] == 0xffff)
    return;
  row = table;
loop:
  {
    id = row[0];
    object = Object_GetById(id);
    x = row[1];
    z = row[2];

    object->collisionFlags &= 0xfe;
    ObjectDispatch_SetSingleChildFlags(object, 0);
    Func_02002184(id, (u16)x, (u16)z, BYTE_AT(object, 34));
    row += 3;
  }
  if (row[0] != 0xffff)
    goto loop;
}
