#define EffectDescriptorTable Data_02009da8
#define GetPartyEffect Func_02001c56
#define SpawnEffect Func_02001bf4
#define SetEffectVariant Func_02001bfe
#define SetEffectDescriptor Func_02001c18
#define SetEffectMode Func_02001d62
#define ScaleEffectDeltaFromAccumulated Func_02001c8c
#define ScaleEffectDeltaFromOrigin Func_02001ca4
#define ScaleEffectVerticalDelta Func_02001cb2
#define SetEffectCallbackMode Func_02001d18
#define SetEffectCallbackArgument Func_02001d28
#include "configured_effect_spawn.h"

void SpawnConfiguredEffect(s32 x, s32 y, s32 z, s32 vx, s32 vy, s32 vz,
                           u32 flags, const struct ConfiguredEffectOptions *options)
{
#include "configured_effect_spawn_body.inc"
}
