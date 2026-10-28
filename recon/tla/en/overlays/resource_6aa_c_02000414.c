// Draft, differ 31 of 113 (was 85), rewritten from the listing as its sibling
// Func_02002b4c in mars_lighthouse_entrance/s2b4c.c: six sparks (object kind
// 0x145) around the object, each thrown upward at a random angle and speed. The
// create call goes through room.h's inline wrapper, which evaluates the random
// offsets before the loads as the native does. Left: register choice for the
// angle, the radius and Iwram_MulQ16's address: the native loads the address
// before shifting the radius (radius r6, angle r8, address sl), where this
// shifts first (address r6, radius r8, angle sl) and so needs one more
// instruction. Tried: the radius scaled at once (57), u16 angle (53), unsigned
// locals, the locals at function scope, the radius scaled in place (all 31),
// permute 5 minutes.
#include "../../../../games/tla/src/scene/mars_lighthouse_entrance/room.h"
#include "iwram_call.h"

extern const u8 gData_02005394[];

void Func_02000414(s32 id) {
  struct Actor *source = Actor_GetById(id);
  s32 i;

  i = 5;
next:
  {
    s32 x = source->x + (Random16Far() << 4) - (Random16Far() << 4);
    struct Actor *object =
        Func_02004bd8_(0x145, x, (Random16Far() << 3) + source->y + 0x40000,
                       source->z + (Random16Far() << 4) - (Random16Far() << 4));

    if (object != NULL) {
      s32 angle;
      s32 radius;
      s32 scale;

      angle = Random16Far();
      radius = Random16Far();
      scale = (Random16Far() >> 1) + 0x4000;
      Func_02004bd0(object, gData_02005394);
      Actor_SetSingleChildFlags(object, 0);
      object->velocityY = angle * 4 + 0x20000;
      object->velocityZ = Iwram_MulQ16(radius * 8, Math_Cosine(angle));
      object->velocityX = Iwram_MulQ16(radius * 8, Math_Sine(angle));
      object->acceleration = 0;
      object->gravity = 0x547a;
      object->scaleX = scale;
      object->scaleY = scale;
      object->bounce = 0x8000;
      Animation_ApplyChildValues(object, 1);
    }
  }
  if (--i >= 0)
    goto next;
}
