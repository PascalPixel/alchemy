#include "scene.h"
#include "effect_runtime.h"

s32 BattleFx_HasMatchingEvent5(s32 effectId)
{
    s32 local;
    s32 result = Battle_Place(0x70000005, (u16)effectId, &local);
    return (u32)((-result) | result) >> 31;
}
