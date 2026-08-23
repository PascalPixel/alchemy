#include "types.h"
#include "battle_summon.h"

extern const u8 Data_08084a9c[];

s32 SummonDefinition_GetNear(u32 summon_id) {
    if (summon_id > 0xFU) {
        return 0;
    }
    return (s32)(Data_08084a9c + summon_id * 8);
}
