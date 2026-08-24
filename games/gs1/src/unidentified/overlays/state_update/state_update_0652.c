#define ConfigurableEffectDescriptors Data_0200b134
#define GetPrimaryEffectRecord Func_0200395a
#define SpawnConfiguredEffectObject Func_020038d8
#define SetConfiguredEffectVariant Func_020038e2
#define ApplyConfiguredEffectDescriptor Func_020038fc
#define SetConfiguredEffectLink Func_02003a96
#define ComputeConfiguredEffectRateFromDelta Func_020039a0
#define ComputeConfiguredEffectRateFromOffset Func_020039b8
#define ComputeConfiguredEffectSecondRate Func_020039c6
#define SetConfiguredEffectCallbackMode Func_020039fc
#define ApplyConfiguredEffectCallbackArgument Func_02003a0c
#define SpawnConfiguredEffect Func_02000ae8

#include "spawn_configured_effect.h"

void SpawnConfiguredEffect(s32 x, s32 y, s32 z, s32 vx, s32 vy, s32 vz,
                           u32 flags, const struct Options *options)
{
#include "spawn_configured_effect_body.inc"
}
