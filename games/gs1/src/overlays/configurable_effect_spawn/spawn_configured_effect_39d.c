#define EffectDescriptorTable Data_0200b92c
#define GetPartyEffect Func_0200384e
#define SpawnEffect Func_020037f4
#define SetEffectVariant Func_020037fe
#define SetEffectDescriptor Func_02003818
#define SetEffectMode Func_0200397a
#define ScaleEffectDeltaFromAccumulated Func_020038a4
#define ScaleEffectDeltaFromOrigin Func_020038bc
#define ScaleEffectVerticalDelta Func_020038ca
#define SetEffectCallbackMode Func_02003918
#define SetEffectCallbackArgument Func_02003928
#include "configured_effect_spawn.h"

void SceneEffect_SpawnConfigured(s32 x, s32 y, s32 z, s32 vx, s32 vy, s32 vz,
                           u32 flags, const struct ConfiguredEffectOptions *options)
{
#include "configured_effect_spawn_body.inc"
}
