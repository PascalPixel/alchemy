#include "mars_lighthouse_star_room.h"

void Animation_SetStateFlags(struct Actor *object, s32 flags);
void Func_02001a9c();
void Func_02001b38(struct Actor *object, s32 priority);

void Func_02001b78(s32 *position, s32 base) {
  struct Actor *object =
      ObjectCreate_(0x11d, position[0], position[1], position[2] - 0x20000);
  s32 angle;

  if (object != 0) {
    object->bounce =
        Math_Cosine(((u32)(Random16Far() * 0x3000) >> 16) + 0x3000);
    object->gravity = (u16)Random16Far();
    object->unk4c = (u16)(Random16Far() << 1) + base;
    /* FAKEMATCH(temporary): the angle held in an s32 local keeps the zero
     * extension and the pool word 0xffff8000 (a direct store narrows the add
     * to 16 bits: movs, lsls for 0x8000, no extension); as a direct store, 10
     * of 50 instructions differ in tla-en. */
    angle = (u16)Random16Far() - 0x8000;
    object->action = angle;
    Func_02001b38(object, 2);
    object->update = Func_02001a9c;
    Animation_SetStateFlags(object, 1);
  }
}
