#include "../../../../games/tla/src/scene/mars_lighthouse_room_f/room.h"
#include "iwram_call.h"

extern const u32 gData_02005b68[];
extern const u32 gData_02005bac[];
s32 Math_Sine(s32 angle);

void Func_0200055c(s32 id) {
  struct Actor *origin = Actor_GetById(id);
  s32 i;

  for (i = 11; i >= 0; i--) {
    s32 a = Random16Far();
    s32 x = origin->x + (a << 4) - (Random16Far() << 4);
    s32 b = Random16Far();
    s32 c = Random16Far();
    struct Actor *object =
        Func_02005498(0x149, x, (b << 3) + origin->y, (c << 4) + origin->z);

    if (object != NULL) {
      s32 angle = (Random16Far() >> 2) + 0xe000;
      s32 speed = Random16Far() << 3;
      s32 scale = (Random16Far() >> 1) + 0x4000;

      {
        u32 pick = Random16Far();

        if (pick <= 0x7fff)
          Func_02005490(object, gData_02005b68);
        else
          Func_02005490(object, gData_02005bac);
      }
      Actor_SetSingleChildFlags(object, 0);
      object->velocityY = (angle << 2) + 0x20000;
      object->velocityZ = Iwram_MulQ16(speed, Math_Cosine(angle));
      {
        s32 vx = Iwram_MulQ16(speed, Math_Sine(angle));

        object->acceleration = 0;
        object->scaleX = scale;
        object->scaleY = scale;
        object->gravity = 0x547a;
        object->velocityX = vx;
        object->bounce = 0x8000;
      }
      Animation_ApplyChildValues(object, 1);
    }
  }
}
