#define EffectDescriptorTable Data_020092dc
#define GetPartyEffect Func_020012a2
#define SpawnEffect Func_02001238
#define SetEffectVariant Func_02001242
#define SetEffectDescriptor Func_0200125c
#define SetEffectMode Func_020013d6
#define ScaleEffectDeltaFromAccumulated Func_02001300
#define ScaleEffectDeltaFromOrigin Func_02001318
#define ScaleEffectVerticalDelta Func_02001326
#define SetEffectCallbackMode Func_0200135c
#define SetEffectCallbackArgument Func_0200136c
#include "configured_effect_spawn.h"

void SpawnConfiguredEffect(s32 x, s32 y, s32 z, s32 vx, s32 vy, s32 vz,
                           u32 flags, const struct ConfiguredEffectOptions *options)
{
#include "configured_effect_spawn_body.inc"
}
