#include "scene.h"
#include "effect_runtime.h"

u32 BattleFx_HasTrigger(u16 effectId)
{
    s32 result;

    result = Battle_Check(effectId);
    return (u32)((0 - result) | result) >> 0x1F;
}
