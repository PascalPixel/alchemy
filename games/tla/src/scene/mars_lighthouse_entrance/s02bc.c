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
    /* FAKEMATCH(barrier): the collision byte's address is built after the
     * motion store, as in the native; as plain statements, 2 of 56
     * instructions differ in tla-en. */
    asm("");
    object->collisionFlags &= 0xfe;
    object->timer = flag;
    Func_020001f8(object);
    row += 3;
  }
  if (row[0] != 0xffff)
    goto loop;
}
