#include "types.h"

s32 BattleObject_IsValidId(u32 object_id) {
    if (object_id <= 7) {
        return 0;
    }
    if (object_id - 128 <= 5) {
        return 0;
    }
    return -1;
}
