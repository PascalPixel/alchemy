#define EffectDescriptorTable Data_0200a814
#define SpawnConfiguredEffect Func_0200013c
#define GetPartyEffect Func_0200264a
#define SpawnEffect Func_020025d0
#define SetEffectVariant Func_020025da
#define SetEffectDescriptor Func_020025f4
#define SetEffectMode Func_02002776
#define ScaleEffectDeltaFromAccumulated Func_02002658
#define ScaleEffectDeltaFromOrigin Func_02002670
#define ScaleEffectVerticalDelta Func_0200267e
#define SetEffectCallbackMode Func_020026f4
#define SetEffectCallbackArgument Func_02002704
#include "configured_effect_spawn.h"

void SpawnConfiguredEffect(s32 x, s32 y, s32 z, s32 vx, s32 vy, s32 vz,
                           u32 flags, const struct ConfiguredEffectOptions *options)
{
#include "configured_effect_spawn_body.inc"
}
