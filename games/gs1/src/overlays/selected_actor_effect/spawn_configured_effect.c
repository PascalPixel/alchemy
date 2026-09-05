#define EffectDescriptorTable Data_02009424
#define GetPartyEffect Func_020013ea
#define SpawnEffect Func_020013a8
#define SetEffectVariant Func_020013b2
#define SetEffectDescriptor Func_020013cc
#define SetEffectMode Func_0200150e
#define ScaleEffectDeltaFromAccumulated Func_02001468
#define ScaleEffectDeltaFromOrigin Func_02001480
#define ScaleEffectVerticalDelta Func_0200148e
#define SetEffectCallbackMode Func_020014cc
#define SetEffectCallbackArgument Func_020014dc
#include "configured_effect_spawn.h"

void SpawnConfiguredEffect(s32 x, s32 y, s32 z, s32 vx, s32 vy, s32 vz,
                           u32 flags, const struct ConfiguredEffectOptions *options)
{
#include "configured_effect_spawn_body.inc"
}
