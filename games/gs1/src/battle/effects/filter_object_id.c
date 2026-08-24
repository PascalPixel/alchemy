#include "types.h"

s32 BattleEffect_GetFlags();

s32 BattleEffect_FilterObjectIdByFlags(s32 arg0) {
    if (BattleEffect_GetFlags() == 0xFF) {
        return -1;
    }
    return arg0;
}
