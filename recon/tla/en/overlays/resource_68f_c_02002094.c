// Draft, differ 6 of 51: places each door object of a table that ends on
// 0xffff. As a goto loop behind a first test on the table argument, with the
// row copied after it, the listing's shape: 0xffff is built again at the
// bottom (as a while loop the loop pass keeps it in r10, 30 differ). Left:
// at both tests the native loads the row before it shifts 255 (a sched2 tie),
// and it advances the row before the last call; here after it, also with an
// asm on the row (non-volatile 6, volatile 6 elsewhere) or a comma argument.

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
    row += 3;
    Func_02002184(id, (u16)x, (u16)z, BYTE_AT(object, 34));
  }
  if (row[0] != 0xffff)
    goto loop;
}
