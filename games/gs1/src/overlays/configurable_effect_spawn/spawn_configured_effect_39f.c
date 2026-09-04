#define ConfigurableEffectDescriptors Data_0200b058
#define SceneEffect_SpawnConfigured Func_02000ae8
#define GetPrimaryEffectRecord Func_020038c6
#define SpawnConfiguredEffectObject Func_02003864
#define SetConfiguredEffectVariant Func_0200386e
#define ApplyConfiguredEffectDescriptor Func_02003888
#define SetConfiguredEffectLink Func_020039f2
#define ComputeConfiguredEffectRateFromDelta Func_0200390c
#define ComputeConfiguredEffectRateFromOffset Func_02003924
#define ComputeConfiguredEffectSecondRate Func_02003932
#define SetConfiguredEffectCallbackMode Func_02003988
#define ApplyConfiguredEffectCallbackArgument Func_02003998

#include "spawn_configured_effect.h"

void SceneEffect_SpawnConfigured(s32 x, s32 y, s32 z, s32 vx, s32 vy, s32 vz,
                           u32 flags, const struct Options *options)
{
#include "spawn_configured_effect_body.inc"
}
