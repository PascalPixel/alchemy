#include "TYPES.H"

extern s16 BattleFx_TargetRangeByMode[];

s32 BattleFx_MapKeyThroughTable(s32 key)
{
    s16 *entry = BattleFx_TargetRangeByMode;
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
