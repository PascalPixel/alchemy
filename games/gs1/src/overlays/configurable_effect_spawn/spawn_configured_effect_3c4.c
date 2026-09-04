#define ConfigurableEffectDescriptors Data_0200b36c
#define SceneEffect_SpawnConfiguredEffect Func_02000ae8
#define GetPrimaryEffectRecord Func_02003c16
#define SpawnConfiguredEffectObject Func_02003b94
#define SetConfiguredEffectVariant Func_02003b9e
#define ApplyConfiguredEffectDescriptor Func_02003bb8
#define SetConfiguredEffectLink Func_02003d2a
#define ComputeConfiguredEffectRateFromDelta Func_02003c3c
#define ComputeConfiguredEffectRateFromOffset Func_02003c54
#define ComputeConfiguredEffectSecondRate Func_02003c62
#define SetConfiguredEffectCallbackMode Func_02003cb8
#define ApplyConfiguredEffectCallbackArgument Func_02003cc8

#include "spawn_configured_effect.h"

void SceneEffect_SpawnConfiguredEffect(s32 x, s32 y, s32 z, s32 vx, s32 vy, s32 vz,
                           u32 flags, const struct Options *options)
{
#include "spawn_configured_effect_body.inc"
}
