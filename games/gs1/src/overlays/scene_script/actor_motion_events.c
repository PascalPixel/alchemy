#include "types.h"

#define SceneEffect_UpdateMotion Func_02000104

union MotionWork {
  struct {
    u32 unk_00[2];
    s32 x, y, z;
    u32 unk_14;
    s32 accum_x, accum_y;
    u32 unk_20[4];
    s32 rate_x, rate_y;
    u32 unk_38[3];
    s32 velocity_x, velocity_y, velocity_z;
    u16 *record;
    u8 unk_54[16];
    u16 angle_step;
  } fields;
  u8 bytes[102];
};

void SceneEffect_UpdateMotion(union MotionWork *work)
{
    u16 *record;
    work->fields.x += work->fields.velocity_x;
    work->fields.y += work->fields.velocity_y;
    work->fields.z += work->fields.velocity_z;
    work->fields.accum_x += work->fields.rate_x;
    work->fields.accum_y += work->fields.rate_y;
    record = work->fields.record;
    record[15] += work->fields.angle_step;
}
#define EffectDescriptorTable Data_0200929c
#define GetPartyEffect Func_0200125e
#define SpawnEffect Func_02001204
#define SetEffectVariant Func_0200120e
#define SetEffectDescriptor Func_02001228
#define SetEffectMode Func_0200137a
#define ScaleEffectDeltaFromAccumulated Func_020012d4
#define ScaleEffectDeltaFromOrigin Func_020012ec
#define ScaleEffectVerticalDelta Func_020012fa
#define SetEffectCallbackMode Func_02001328
#define SetEffectCallbackArgument Func_02001338
#include "configured_effect_spawn.h"

void SpawnConfiguredEffect(s32 x, s32 y, s32 z, s32 vx, s32 vy, s32 vz,
                           u32 flags, const struct ConfiguredEffectOptions *options)
{
#include "configured_effect_spawn_body.inc"
}
