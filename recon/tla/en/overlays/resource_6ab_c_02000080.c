// Draft of overlay 6ab's Func_02000080, the twin of overlay 6aa's Func_02000414
// (resource_6aa_c_02000414.c), whose header says what differs: differ 31 of 113
// (was 85).
#include "../../../../games/tla/src/scene/mars_lighthouse_star_room/mars_lighthouse_star_room.h"
#include "iwram_call.h"

extern const u8 gData_02004474[];

s32 Math_Sine(s32 angle);
void Animation_ApplyChildValues(struct Actor *object, s32 value);

void Func_02000080(s32 id) {
  struct Actor *source = Actor_GetById(id);
  s32 i;

  i = 5;
next:
  {
    s32 x = source->x + (Random16Far() << 4) - (Random16Far() << 4);
    struct Actor *object =
        ObjectCreate_(0x145, x, (Random16Far() << 3) + source->y + 0x40000,
                       source->z + (Random16Far() << 4) - (Random16Far() << 4));

    if (object != NULL) {
      s32 angle;
      s32 radius;
      s32 scale;

      angle = Random16Far();
      radius = Random16Far();
      scale = ((u32)Random16Far() >> 1) + 0x4000;
      Func_020041dc(object, gData_02004474);
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
