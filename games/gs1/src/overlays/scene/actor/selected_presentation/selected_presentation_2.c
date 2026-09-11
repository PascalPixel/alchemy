#include "types.h"
#include "scene.h"

/* overlays/scene/actor/selected_presentation/apply_zero_and_clear_byte_89.c */
/* Complete 24-byte entity reset owner. */
s32 OvObj_ApplyZeroAndClearByte89(u8 *obj)
{
    Actor_Run(obj, 0);
    obj[89] = 0;
    return 0;
}

/* overlays/scene/actor/selected_presentation/toggle_field_84_bit_0.c */
/* Complete 16-byte flag-toggle leaf, including its alignment halfword. */
s32 OvObj_ToggleField84Bit0(u8 *obj)
{
    obj[84] ^= 1;
    return 1;
}
