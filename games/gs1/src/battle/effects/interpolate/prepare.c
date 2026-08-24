#include "types.h"
#include "global_cells.h"

#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

void BattleEffect_ApplyColorToTargetBuffer(s32 arg0, s32 arg1);
void BattleEffect_StartBufferInterpolation(s32 value);

void BattleEffect_PrepareBufferInterpolation(void) {
    s32 value;
    u8 *state;

    state = *(u8 **)ADDR_03001EBC;
    value = (s32)(state + 0x236);
    BattleEffect_ApplyColorToTargetBuffer(value, 2);
    if (M2C_FIELD(state, s16 *, 0xCB8) != 0) {
        BattleEffect_ApplyColorToTargetBuffer(0x10001, 1);
    } else {
        BattleEffect_ApplyColorToTargetBuffer(value, 1);
    }
    BattleEffect_StartBufferInterpolation(8);
}
