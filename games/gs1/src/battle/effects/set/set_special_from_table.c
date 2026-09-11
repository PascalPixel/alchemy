#include "types.h"
#include "scene.h"
#include "abi/battle/effects/set/set_special_from_table.h"

extern s16 gCell[];
extern s32 gRom[];

void BattleFx_SetSpecialFromTable(s32 arg0, s32 arg1)
{
    s32 target = gCell[224];
    s32 *table = gRom;
    s32 entry = *table++;
    s32 result = arg1;

    if (entry != 0 && entry != target) {
        do {
            if (entry & 0x80000000) {
                result = entry & 0xFFFF;
            }
            entry = *table++;
        } while (entry != 0 && entry != target);
    }
    gCell[235] = (s16)result;
}
