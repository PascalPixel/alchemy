#define EffectDescriptorTable Data_0200dfb8
#define SceneEffect_SpawnConfigured Func_0200013c
#define GetPartyEffect Func_02005eaa
#define SpawnEffect Func_02005df0
#define SetEffectVariant Func_02005dfa
#define SetEffectDescriptor Func_02005e14
#define SetEffectMode Func_02006006
#define ScaleEffectDeltaFromAccumulated Func_02005e68
#define ScaleEffectDeltaFromOrigin Func_02005e80
#define ScaleEffectVerticalDelta Func_02005e8e
#define SetEffectCallbackMode Func_02005f14
#define SetEffectCallbackArgument Func_02005f24
#include "configured_effect_spawn.h"

void SceneEffect_SpawnConfigured(s32 x, s32 y, s32 z, s32 vx, s32 vy, s32 vz,
                           u32 flags, const struct ConfiguredEffectOptions *options)
{
#include "configured_effect_spawn_body.inc"
}
