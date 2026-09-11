#include "object_lookup.h"
#include "types.h"
#include "scene.h"
#include "abi/object/table/get.h"

extern u8 *gWork;

void *ObjectTable_Get(u32 arg0)
{
    u8 *base = gWork;
    u32 offset;
    if (arg0 > 0xbf)
        return 0;
    offset = (arg0 * 4) + 0x14;
    return *(void **)(base + offset);
}
