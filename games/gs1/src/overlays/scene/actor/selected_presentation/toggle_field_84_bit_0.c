#include "types.h"
#include "scene.h"

/* Complete 16-byte flag-toggle leaf, including its alignment halfword. */
s32 OvObj_ToggleField84Bit0(u8 *obj)
{
    obj[84] ^= 1;
    return 1;
}
