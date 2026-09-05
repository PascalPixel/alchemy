#define EffectDescriptorTable Data_02009be0
#define GetPartyEffect Func_02001b5e
#define SpawnEffect Func_02001afc
#define SetEffectVariant Func_02001b06
#define SetEffectDescriptor Func_02001b20
#define SetEffectMode Func_02001c82
#define ScaleEffectDeltaFromAccumulated Func_02001b94
#define ScaleEffectDeltaFromOrigin Func_02001bac
#define ScaleEffectVerticalDelta Func_02001bba
#define SetEffectCallbackMode Func_02001c20
#define SetEffectCallbackArgument Func_02001c30
#include "configured_effect_spawn.h"

void SpawnConfiguredEffect(s32 x, s32 y, s32 z, s32 vx, s32 vy, s32 vz,
                           u32 flags, const struct ConfiguredEffectOptions *options)
{
#include "configured_effect_spawn_body.inc"
}
