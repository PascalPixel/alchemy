#ifndef ALCHEMY_GOMA_H
#define ALCHEMY_GOMA_H

#include "TYPES.H"

#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

#include "CONFIGURED_EFFECT_SPAWN.H"

typedef struct {
    u8 pad_to_angle[30];
    u16 angle;
} EffectRecord_020004c4;

typedef struct {
    u8 pad_to_record[80];
    EffectRecord_020004c4 *record;
} EffectWork_020004c4;

void Engine_TaskWait(s32 frames);

void BattleFx_UpdateObjectMotionScaleAndLinkedAngle(void *object);

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

#endif
