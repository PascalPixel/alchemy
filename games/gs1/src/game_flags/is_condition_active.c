#include "types.h"

s32 GameFlag_IsSet();

s32 GameFlag_IsConditionActive(s32 condition) {
    s32 flag_state;

    if (condition == -1) {
        return 1;
    }
    if (0x1000 & condition) {
        return GameFlag_IsSet();
    }
    flag_state = GameFlag_IsSet();
    return 1 - ((u32) ((0 - flag_state) | flag_state) >> 0x1F);
}
