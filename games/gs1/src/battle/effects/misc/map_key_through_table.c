#include "types.h"
#include "scene.h"

extern s16 gRom[];

s32 BattleFx_MapKeyThroughTable(s32 key)
{
    s16 *entry = gRom;
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
