#include "local.h"

void Func_02002e04(s32 id)
{
    struct ObjectRuntime *object = Object_GetById(id);

    if (object != 0) {
        u8 *meta = ResourceMetadata_Register(object->animation, 0x17d);

        meta[5] = 0;
        meta[6] = 0;
        Func_02003cc4(object, 0);
        Func_02003cc4(object, 2);
    }
}
