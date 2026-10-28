#include "local.h"

void Func_02001f60(s32 id) {
  struct Actor *object = Actor_GetById(id);

  if (object != 0) {
    u8 *meta = ResourceMetadata_Register(object->animation, 0x17d);

    meta[5] = 0;
    meta[6] = 0;
    Func_02002910(object, 0);
    Func_02002910(object, 2);
  }
}
