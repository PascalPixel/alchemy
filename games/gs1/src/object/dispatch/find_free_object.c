#include "types.h"
#include "scene.h"
#include "abi/object/dispatch/find_free_object.h"

extern u8 *gIw;

void *ObjectDispatch_FindFreeObject(void)
{
    u8 *entry = gIw;
    void *ret = 0;
    s32 index = 0;

    while (index <= 63) {
        if (*(u32 *)entry == 0) {
            ret = entry;
            break;
        }
        index++;
        entry += 112;
    }
    return ret;
}
