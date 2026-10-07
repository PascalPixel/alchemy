#include "TYPES.H"

void ObjectList_CollectActive(u8 *object, u8 **out)
{
    u8 *end = object + 1792;

    do {
        if (*(u16 *)(object + 2) != 0)
            *out++ = object;
        object += 128;
    } while (object != end);
    *out = NULL;
}
