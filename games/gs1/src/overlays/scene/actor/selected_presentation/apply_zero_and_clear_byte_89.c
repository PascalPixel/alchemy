#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/actor/selected_presentation/apply_zero_and_clear_byte_89.h"

/* Complete 24-byte entity reset owner. */
s32 OvObj_ApplyZeroAndClearByte89(u8 *obj)
{
    Actor_Run(obj, 0);
    obj[89] = 0;
    return 0;
}
