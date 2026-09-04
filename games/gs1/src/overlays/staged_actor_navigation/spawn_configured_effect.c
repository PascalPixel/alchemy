#define EffectDescriptorTable Data_0200ad58
#define SpawnConfiguredEffect Func_0200013c
#define GetPartyEffect Func_02002c7a
#define SpawnEffect Func_02002bf8
#define SetEffectVariant Func_02002c02
#define SetEffectDescriptor Func_02002c1c
#define SetEffectMode Func_02002d86
#define ScaleEffectDeltaFromAccumulated Func_02002ca8
#define ScaleEffectDeltaFromOrigin Func_02002cc0
#define ScaleEffectVerticalDelta Func_02002cce
#define SetEffectCallbackMode Func_02002d1c
#define SetEffectCallbackArgument Func_02002d2c
#include "configured_effect_spawn.h"

void SpawnConfiguredEffect(s32 x, s32 y, s32 z, s32 vx, s32 vy, s32 vz,
                           u32 flags, const struct ConfiguredEffectOptions *options)
{
#include "configured_effect_spawn_body.inc"
}
