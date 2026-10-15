#include "types.h"

typedef struct {
    s32 reserved00[3];
    s32 currentValue;
    s32 indexRadius;
    s32 entryLimit;
    s32 reserved18[3];
    s16 values[1];
} BattleEffectIndexWindow;

s32 BattleFx_BuildCenteredIndexList(BattleEffectIndexWindow *window, s16 *output)
{
    s32 length = window->indexRadius * 2 + 1;
    s32 entryCount = 0;
    s32 center = 0;
    s32 entry_index;

    for (entry_index = 0; entry_index != length; entry_index++) {
        if (window->currentValue == window->values[entry_index]) {
            center = entry_index;
            break;
        }
    }
    for (entry_index = 0; entry_index != length; entry_index++) {
        s32 battle_value = center + entry_index - window->indexRadius;

        if (battle_value >= 0 && battle_value < window->entryLimit) {
            output[entryCount++] = battle_value;
        }
    }
    return entryCount;
}
