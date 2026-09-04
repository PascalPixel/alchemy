#include "types.h"

typedef struct {
    s32 reserved_00[3];
    s32 current_value;
    s32 index_radius;
    s32 entry_limit;
    s32 reserved_18[3];
    s16 values[1];
} BattleEffectIndexWindow;

#define BuildCenteredIndexList Func_080da24c
s32 BuildCenteredIndexList(BattleEffectIndexWindow *window, s16 *output) {
    s32 length = window->index_radius * 2 + 1;
    s32 entry_count = 0;
    s32 center = 0;
    s32 entry_index;

    for (entry_index = 0; entry_index != length; entry_index++) {
        if (window->current_value == window->values[entry_index]) {
            center = entry_index;
            break;
        }
    }
    for (entry_index = 0; entry_index != length; entry_index++) {
        s32 battle_value = center + entry_index - window->index_radius;

        if (battle_value >= 0 && battle_value < window->entry_limit) {
            output[entry_count++] = battle_value;
        }
    }
    return entry_count;
}
