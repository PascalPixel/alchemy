#include "types.h"

void ObjectList_CollectActive(u8 *object, u8 **out)
{
    u8 *end = object + 0x700;

    do {
        if (*(u16 *)(object + 2) != 0)
            *out++ = object;
        object += 128;
    } while (object != end);
    *out = NULL;
}
