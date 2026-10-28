#include "mars_lighthouse_star_room.h"

void Func_02001b78(s32 *position, s32 base) {
  struct Actor *object =
      ObjectCreate_(0x11d, position[0], position[1], position[2] - 0x20000);
  s32 angle;

  if (object != 0) {
    object->bounce =
        Math_Cosine(((u32)(Random16Far() * 0x3000) >> 16) + 0x3000);
    object->gravity = (u16)Random16Far();
    object->unk4c = (u16)(Random16Far() << 1) + base;
    /* FAKEMATCH: the s32 temporary keeps the zero extension and the pool word
     * 0xffff8000; stored directly, the add narrows to 16 bits (movs, lsls for
     * 0x8000, no extension). */
    angle = (u16)Random16Far() - 0x8000;
    object->action = angle;
    Func_02001b38(object, 2);
    object->update = Func_02001a9c;
    Animation_SetStateFlags(object, 1);
  }
}
