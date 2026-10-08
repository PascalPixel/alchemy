#include "local.h"

void Func_02001308(s32 id)
{
    struct ObjectRuntime *object = Object_GetById(id);

    if (object != 0) {
        u8 *meta = ResourceMetadata_Register(object->animation, 0x17d);

        meta[5] = 0;
        meta[6] = 0;
        Func_02001c6c(object, 0);
        Func_02001c6c(object, 2);
    }
}
