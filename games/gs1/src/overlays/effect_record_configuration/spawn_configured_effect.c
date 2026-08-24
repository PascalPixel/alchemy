#define EffectDescriptorTable Data_0200880c
#define GetPartyEffect Func_02000882
#define SpawnEffect Func_02000860
#define SetEffectVariant Func_0200086a
#define SetEffectDescriptor Func_02000884
#define SetEffectMode Func_0200096e
#define ScaleEffectDeltaFromAccumulated Func_02000928
#define ScaleEffectDeltaFromOrigin Func_02000940
#define ScaleEffectVerticalDelta Func_0200094e
#define SetEffectCallbackMode Func_02000984
#define SetEffectCallbackArgument Func_02000994
#define SpawnConfiguredEffect Func_0200013c
#include "configured_effect_spawn.h"

void SpawnConfiguredEffect(s32 x, s32 y, s32 z, s32 vx, s32 vy, s32 vz,
                           u32 flags, const struct ConfiguredEffectOptions *options)
{
#include "configured_effect_spawn_body.inc"
}
