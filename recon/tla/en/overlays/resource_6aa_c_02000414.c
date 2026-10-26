// Draft of overlay 6aa's Func_02000414.
#include "../../../../games/tla/src/scene/mars_lighthouse_entrance/room.h"
#include "iwram_call.h"

extern const u8 gData_02005394[];

void Func_02000414(s32 id) {
  struct ObjectRuntime *source = Object_GetById(id);
  s32 i;

  i = 5;
next:
  {
    s32 x = source->x + (Random16Far() << 4) - (Random16Far() << 4);
    struct ObjectRuntime *object =
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
      ObjectDispatch_SetSingleChildFlags(object, 0);
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
