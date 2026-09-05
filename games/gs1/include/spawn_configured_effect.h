#ifndef GUARD_SPAWN_CONFIGURED_EFFECT_H
#define GUARD_SPAWN_CONFIGURED_EFFECT_H

#include "types.h"

struct Sprite {
    u8 pad00[9];
    u8 flags9;
    u8 pad0a[20];
    u16 angle;
    u8 pad20[6];
    u8 state26;
};

struct Effect {
    u8 pad00[24];
    s32 accum18;
    s32 accum1c;
    u8 pad20[3];
    u8 flags23;
    u8 pad24[12];
    s32 rate30;
    s32 rate34;
    u8 pad38[12];
    s32 velocity_x;
    s32 velocity_y;
    s32 velocity_z;
    struct Sprite *sprite;
    u8 pad54;
    u8 mode55;
    u8 pad56[14];
    u16 step64;
    u8 pad66[6];
    u32 callback;
};

struct Options {
    u8 mode_bits;
    u8 pad01[3];
    s32 link;
    s32 accum18;
    s32 accum1c;
    s32 target30;
    s32 target34;
    s16 kind;
    u16 pad1a;
    s32 callback_arg;
    u16 angle;
    u16 step;
    u32 callback;
};

struct Descriptor {
    s32 pad00[3];
    s32 duration;
};

extern struct Descriptor *ConfigurableEffectDescriptors[];

struct Effect *GetPrimaryEffectRecord();
struct Effect *SpawnConfiguredEffectObject();
void SetConfiguredEffectVariant();
void ApplyConfiguredEffectDescriptor();
void SetConfiguredEffectLink();
s32 ComputeConfiguredEffectRateFromDelta();
s32 ComputeConfiguredEffectRateFromOffset();
s32 ComputeConfiguredEffectSecondRate();
void SetConfiguredEffectCallbackMode();
void ApplyConfiguredEffectCallbackArgument();

#endif
