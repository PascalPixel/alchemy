#include "types.h"

typedef struct {
    s32 padding00[3];
    s32 current;
    s32 radius;
    s32 limit;
    s32 padding18[3];
    s16 values[1];
} BattleEffectIndexWindow;

#define BuildCenteredIndexList Func_080da24c

s32 BuildCenteredIndexList(BattleEffectIndexWindow *window, s16 *output) {
    s32 length = window->radius * 2 + 1;
    s32 count = 0;
    s32 center = 0;
    s32 index;

    for (index = 0; index != length; index++) {
        if (window->current == window->values[index]) {
            center = index;
            break;
        }
    }
    for (index = 0; index != length; index++) {
        s32 value = center + index - window->radius;

        if (value >= 0 && value < window->limit) {
            output[count++] = value;
        }
    }
    return count;
}
