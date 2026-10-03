#ifndef ALCHEMY_GOMA_H
#define ALCHEMY_GOMA_H

/* The Goma waterway (resource_38a): what its files share. */

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

void WaitFrames(s32 frames);

/* Shared 22-byte head leaf proved identical for this overlay family. */

/* Contiguous unnamed leaf-owner run for resource_38a. */

/* Complete four-byte leaf: movs r0,#0 followed by bx lr. */

/* Complete eight-byte literal-address getter, including its sole pool word. */

/* Wait at most sixty frames for the object to reach the requested height. */

void BattleFx_UpdateObjectMotionScaleAndLinkedAngle(void *object);

/* The object is accessed through word fields and a linked record. Keep the
 * shared storage view so the record load follows the position stores. */
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
