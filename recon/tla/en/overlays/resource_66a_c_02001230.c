// Near miss, differ 28 of 51: the native rebuilds the 0xffff terminator at
// each test and copies the list pointer after the first test; the draft
// hoists the constant into r10. A goto loop leaves differ 10 (no hoisting,
// pointer copy and one shift placed differently). while, for, do-while and
// indexed forms were tried.
#include "../../../../games/tla/src/scene/alhafran_cave/local.h"

void Func_02001230(const u16 *entry) {
  while (entry[0] != 0xffff) {
    u32 id = entry[0];
    struct ObjectRuntime *object = Object_GetById(id);
    s16 kind = entry[1];
    s16 flag = entry[2];

    object->collisionFlags &= ~1;
    ObjectDispatch_SetSingleChildFlags(object, 0);
    Func_02001320(id, (u16)kind, (u16)flag, object->terrainId);
    entry += 3;
  }
}
