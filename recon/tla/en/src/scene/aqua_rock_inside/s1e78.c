#include "../../../../../../games/tla/src/scene/aqua_rock_inside/local.h"

void Func_02001e78(s32 id) {
  struct Actor *object = Actor_GetById(id);

  if (object != 0) {
    u8 *meta = ResourceMetadata_Register(object->animation, 0x17d);

    meta[5] = 0;
    meta[6] = 0;
    Func_02003cc4(object, 2);
  }
}
