#ifndef GUARD_CONFIGURED_EFFECT_SPAWN_H
#define GUARD_CONFIGURED_EFFECT_SPAWN_H

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

struct ConfiguredEffectOptions {
    u8 mode_bits;
    u8 pad01[3];
    s32 mode;
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

struct EffectDescriptor {
    s32 pad00[3];
    s32 duration;
};

extern struct EffectDescriptor *EffectDescriptorTable[];
struct Effect *GetPartyEffect();
struct Effect *SpawnEffect();
void SetEffectVariant();
void SetEffectDescriptor();
void SetEffectMode();
s32 ScaleEffectDeltaFromAccumulated();
s32 ScaleEffectDeltaFromOrigin();
s32 ScaleEffectVerticalDelta();
void SetEffectCallbackMode();
void SetEffectCallbackArgument();

#endif
