#define EffectDescriptorTable Data_0200a120
#define GetPartyEffect Func_02001ed2
#define SpawnEffect Func_02001e80
#define SetEffectVariant Func_02001e8a
#define SetEffectDescriptor Func_02001ea4
#define SetEffectMode Func_02001fee
#define ScaleEffectDeltaFromAccumulated Func_02001f00
#define ScaleEffectDeltaFromOrigin Func_02001f18
#define ScaleEffectVerticalDelta Func_02001f26
#define SetEffectCallbackMode Func_02001fa4
#define SetEffectCallbackArgument Func_02001fb4
#define SpawnConfiguredEffect Func_0200013c
#include "configured_effect_spawn.h"

void SpawnConfiguredEffect(s32 x, s32 y, s32 z, s32 vx, s32 vy, s32 vz,
                           u32 flags, const struct ConfiguredEffectOptions *options)
{
#include "configured_effect_spawn_body.inc"
}
