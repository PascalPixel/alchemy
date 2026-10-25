// Draft of overlay 6aa's Func_020002bc, the twin of overlay 68f's
// Func_02000404: its draft (resource_68f_c_02000404.c) with this overlay's
// symbols: differ 8 of 56. Left: what that draft's header records.

#include "types.h"
#include "field_event.h"

struct FieldActor *Object_GetById();
void ObjectDispatch_SetSingleChildFlags();
s32 GameFlag_Test();
void Func_02004bc0();
void Func_020001f8();

void Func_020002bc(u16 *table) {
  u16 *row;
  struct FieldActor *object;
  s16 flag;
  s16 animation;

  if (table[0] == 0xffff)
    return;
  row = table;
loop:
  {
    object = Object_GetById(row[0]);
    animation = row[1];
    flag = row[2];

    ObjectDispatch_SetSingleChildFlags(object, 0);
    Func_02004bc0(object, (u16)animation + GameFlag_Test((u16)flag));
    object->motionFlags = 0;
    object->collisionFlags &= 0xfe;
    object->timer = flag;
    row += 3;
    Func_020001f8(object);
  }
  if (row[0] != 0xffff)
    goto loop;
}
