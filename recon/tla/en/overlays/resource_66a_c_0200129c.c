// Near miss, differ 27 of 61 (was 51): finds the entry of the list (u16
// triples ended by 0xffff, as Func_02001230's) for the object the effect work
// names; before frame 8 it lowers the object's y scale, and in mode 1 it sets
// the entry's flag and places it again at full scale. The native is a while
// loop that jump.c does not rotate: it jumps to the test at the bottom and
// rebuilds 0xffff and the object id there on every pass. A goto loop gives that
// layout (as a for or while loop the test is copied to the top and loop.c
// hoists the constant into ip). kind and flag as s32 and read through the
// stepped pointer (*++entry, entry[1]) give the native's ldrh pair. Left: the
// native puts the first insn of the test (movs r1, #255) before its jump to
// the test, and allocates the id to r7 and the object to r6 (here the other
// way; declaring either at the top does not change it).
#include "../../../../games/tla/src/scene/alhafran_cave/local.h"

void Func_0200129c(const u16 *entry, s32 mode, s32 frame) {
  struct AlhafranCaveEffectWork *work = Ram_HeapSlots->effectWork;
  s32 id;

  goto test;
loop:
  id = work->objectId;
  if (id == entry[0]) {
    struct Actor *object = Actor_GetById(id);
    s32 kind = *++entry;
    s32 flag = entry[1];

    if (frame <= 7)
      object->scaleY -= 0x1f00;
    if (mode == 1) {
      GameFlag_SetBit(flag);
      Func_02001320(id, kind, flag, object->terrainId);
      object->scaleY = 0x10000;
    }
    return;
  }
  entry += 3;
test:
  if (entry[0] != 0xffff)
    goto loop;
}
