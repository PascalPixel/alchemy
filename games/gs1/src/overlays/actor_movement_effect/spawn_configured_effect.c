#define EffectDescriptorTable Data_0200929c
#define SpawnConfiguredEffect Func_0200013c
#define GetPartyEffect Func_0200125e
#define SpawnEffect Func_02001204
#define SetEffectVariant Func_0200120e
#define SetEffectDescriptor Func_02001228
#define SetEffectMode Func_0200137a
#define ScaleEffectDeltaFromAccumulated Func_020012d4
#define ScaleEffectDeltaFromOrigin Func_020012ec
#define ScaleEffectVerticalDelta Func_020012fa
#define SetEffectCallbackMode Func_02001328
#define SetEffectCallbackArgument Func_02001338
#include "configured_effect_spawn.h"

void SpawnConfiguredEffect(s32 x, s32 y, s32 z, s32 vx, s32 vy, s32 vz,
                           u32 flags, const struct ConfiguredEffectOptions *options)
{
#include "configured_effect_spawn_body.inc"
}
