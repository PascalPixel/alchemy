// Draft, differ 8 of 56: sets up each switch object of a table that ends on
// 0xffff. As a goto loop behind a first test on the table argument, with the
// row copied after it and the animation read before the flag (34 to 8): as a
// while, for or guarded do-while the loop pass keeps the first test's 0xffff
// in r10. Left: at both tests the native loads the row before it shifts 255
// (a sched2 tie), it advances the row before the last call (here after it,
// wherever the source puts the increment), and it copies the object for the
// collision flags after the zero store. The rows are three halfwords; a
// struct row is padded to 8 bytes.

#include "types.h"
#include "field_event.h"

struct FieldActor *Object_GetById();
void ObjectDispatch_SetSingleChildFlags();
s32 GameFlag_Test();
void Func_02002f80();
void Func_02000338();

void Func_02000404(u16 *table) {
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
    Func_02002f80(object, (u16)animation + GameFlag_Test((u16)flag));
    object->motionFlags = 0;
    object->collisionFlags &= 0xfe;
    object->timer = flag;
    row += 3;
    Func_02000338(object);
  }
  if (row[0] != 0xffff)
    goto loop;
}
