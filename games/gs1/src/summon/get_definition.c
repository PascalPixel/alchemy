#include "types.h"
#include "scene.h"
#include "abi/summon/get_definition.h"
#include "battle_summon.h"

extern const u8 gRom[];

s32 SummonDefinition_GetNear(u32 summon_id)
{
    if (summon_id > 0xFU) {
        return 0;
    }
    return (s32)(gRom + summon_id * 8);
}
