#include "mars_lighthouse_room_m.h"

void Func_02001f14(struct MarsLighthouseRoomMVector *position, s32 value) {
  struct Actor *object =
      (struct Actor *)Value4((s32 (*)())Func_02003128, 285, position->x,
                                     position->y, position->z + 0xfffe0000);
  s32 angle;

  if (object != 0) {
    object->bounce =
        Math_Cosine(((u32)(Random16Far() * 3 << 12) >> 16) + 0x3000);
    object->gravity = (u16)Random16Far();
    object->unk4c = ((u32)(Random16Far() << 17) >> 16) + value;
    /* FAKEMATCH: the s32 temporary keeps the zero extension and the pool word
     * 0xffff8000; stored directly, the add narrows to 16 bits (movs, lsls for
     * 0x8000, no extension). */
    angle = (u16)Random16Far() - 0x8000;
    object->action = angle;
    ((void (*)())Func_02001ed4)(object, 2);
    object->update = Func_02001e38;
    Animation_SetStateFlags(object, 1);
  }
}
