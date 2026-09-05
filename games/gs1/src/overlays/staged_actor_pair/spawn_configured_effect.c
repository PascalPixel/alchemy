#define EffectDescriptorTable Data_02009f60
#define SpawnConfiguredEffect Func_0200013c
#define GetPartyEffect Func_02001e1a
#define SpawnEffect Func_02001dc8
#define SetEffectVariant Func_02001dd2
#define SetEffectDescriptor Func_02001dec
#define SetEffectMode Func_02001f26
#define ScaleEffectDeltaFromAccumulated Func_02001e90
#define ScaleEffectDeltaFromOrigin Func_02001ea8
#define ScaleEffectVerticalDelta Func_02001eb6
#define SetEffectCallbackMode Func_02001eec
#define SetEffectCallbackArgument Func_02001efc
#include "configured_effect_spawn.h"

void SpawnConfiguredEffect(s32 x, s32 y, s32 z, s32 vx, s32 vy, s32 vz,
                           u32 flags, const struct ConfiguredEffectOptions *options)
{
#include "configured_effect_spawn_body.inc"
}
