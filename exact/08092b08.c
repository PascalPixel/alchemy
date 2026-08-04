#include "object_lookup.h"
#include "types.h"

void Func_08092b08(s32 arg0, s32 arg1)
{
    u32 object;
    u8 *state;
    s32 value;
    s32 mask;
    s32 three;

    three = 3;
    object = Func_0808ba1c(arg0);
    if (object != 0 && (0xF & *(volatile s8 *)(object + 0x54)) == 1) {
        value = (three & arg1) * 4;
        mask = -0xD;
        state = *(u8 **)(object + 0x50);
        state[9] = (mask & state[9]) | value;
        state[0x15] = (mask & state[0x15]) | value;
        *(u8 *)(object + 0x23) = 0xFE & *(u8 *)(object + 0x23);
    }
}
