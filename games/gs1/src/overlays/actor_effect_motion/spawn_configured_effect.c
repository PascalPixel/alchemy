#define EffectDescriptorTable Data_02009bb4
#define GetPartyEffect Func_02001b3e
#define SpawnEffect Func_02001abc
#define SetEffectVariant Func_02001ac6
#define SetEffectDescriptor Func_02001ae0
#define SetEffectMode Func_02001c62
#define ScaleEffectDeltaFromAccumulated Func_02001b7c
#define ScaleEffectDeltaFromOrigin Func_02001b94
#define ScaleEffectVerticalDelta Func_02001ba2
#define SetEffectCallbackMode Func_02001be0
#define SetEffectCallbackArgument Func_02001bf0
#define SpawnConfiguredEffect Func_0200013c
#include "configured_effect_spawn.h"

void SpawnConfiguredEffect(s32 x, s32 y, s32 z, s32 vx, s32 vy, s32 vz,
                           u32 flags, const struct ConfiguredEffectOptions *options)
{
#include "configured_effect_spawn_body.inc"
}
