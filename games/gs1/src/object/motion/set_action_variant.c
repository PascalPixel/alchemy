#include "object_lookup.h"
#include "types.h"

void ObjectMotion_SetActionVariant(s32 arg0, s32 arg1)
{
    u32 object_address;
    u8 *object_state;
    s32 variant_bits;
    s32 state_mask;
    s32 variant_mask;

    variant_mask = 3;
    object_address = (u32)ObjectTable_Get(arg0);
    if (object_address != 0 && (0xF & *(volatile s8 *)(object_address + 0x54)) == 1) {
        variant_bits = (variant_mask & arg1) * 4;
        state_mask = -0xD;
        object_state = *(u8 **)(object_address + 0x50);
        object_state[9] = (state_mask & object_state[9]) | variant_bits;
        object_state[0x15] = (state_mask & object_state[0x15]) | variant_bits;
        *(u8 *)(object_address + 0x23) = 0xFE & *(u8 *)(object_address + 0x23);
    }
}
