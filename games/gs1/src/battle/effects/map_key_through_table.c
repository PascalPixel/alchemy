#include "types.h"

#define BattleEffect_MapKeyThroughTable Func_0808ddb8

extern s16 Data_0809e686[];

s32 BattleEffect_MapKeyThroughTable(s32 key)
{
    s16 *entry = Data_0809e686;
    s32 result = 16;
    s32 current = *entry;

    while (current != -1) {
        ++entry;
        if (key == current) {
            result = *entry;
            break;
        }
        ++entry;
        current = *entry;
    }
    return result;
}
