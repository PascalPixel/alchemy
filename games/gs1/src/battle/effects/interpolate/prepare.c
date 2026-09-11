#include "types.h"
#include "global_cells.h"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

void BattleFx_ApplyColorToTargetBuffer(s32 arg0, s32 arg1);
void BattleFx_StartBufferInterpolation(s32 value);

void BattleFx_PrepareBufferInterpolation(void)
{
    s32 value;
    u8 *state;

    state = *(u8 **)ADDR_03001EBC;
    value = (s32)(state + 0x236);
    BattleFx_ApplyColorToTargetBuffer(value, 2);
    if (FIELD_AT_OFFSET(state, s16 *, 0xCB8) != 0) {
        BattleFx_ApplyColorToTargetBuffer(0x10001, 1);
    } else {
        BattleFx_ApplyColorToTargetBuffer(value, 1);
    }
    BattleFx_StartBufferInterpolation(8);
}
