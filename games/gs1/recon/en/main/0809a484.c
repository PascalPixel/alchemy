#include "types.h"

#define BattleEffect_CreateRadialParticle Func_0809a484

struct ParticleVisual {
    u8 reserved_00[9];
    u8 flags;
    u8 reserved_0a[20];
    u16 attribute;
    u8 reserved_20[6];
    u8 state;
};

struct ParticleObject {
    u8 reserved_00[24];
    s32 x;
    s32 y;
    u8 reserved_20[3];
    u8 flags;
    u8 reserved_24[12];
    s32 phase;
    s32 velocity;
    u8 reserved_38[12];
    s32 initial_x;
    s32 initial_y;
    s32 initial_z;
    struct ParticleVisual *visual;
    u8 reserved_54;
    u8 mode;
    u8 reserved_56[14];
    u16 angle;
    u8 reserved_66[6];
    void *callback;
};

struct ParticleConfig {
    u8 visual_flags;
    u8 reserved_01[3];
    struct ParticleObject *parent;
    s32 x;
    s32 y;
    s32 target_x;
    s32 target_y;
    s16 resource;
    u8 reserved_1a[2];
    void *animation;
    u16 attribute;
    u16 angle;
    void *callback;
};

extern void *Data_080a012c[];
extern u8 Data_0809a44d;

struct ParticleObject *Func_08092054(s32);
struct ParticleObject *Func_080090c8(s16, s32, s32, s32);
void Func_08009080(struct ParticleObject *, s32);
void Func_08009098(struct ParticleObject *, void *);
void Func_080929d8(struct ParticleObject *, struct ParticleObject *);
s32 Func_080022ec(s32, s32);

void BattleEffect_CreateRadialParticle(
    s32 x,
    s32 y,
    s32 z,
    s32 initial_x,
    s32 initial_y,
    s32 initial_z,
    u32 flags,
    struct ParticleConfig *config)
{
    struct ParticleObject *shared = Func_08092054(0);
    struct ParticleObject *object;
    struct ParticleVisual *visual;
    void *animation;
    s32 variant = flags & 15;

    if ((flags & 0x100000) != 0 && config != 0)
        variant = config->resource;
    else
        variant = 0xde;

    object = Func_080090c8(variant, x, y, z);
    if (object == 0)
        return;

    visual = object->visual;
    Func_08009080(object, (flags + 1) & 15);
    animation = Data_080a012c[flags & 15];
    Func_08009098(object, animation);
    object->mode = 0;
    visual->state = 0;
    object->callback = &Data_0809a44d;
    object->initial_x = initial_x;
    object->initial_y = initial_y;
    object->initial_z = initial_z;
    visual->flags = (visual->flags & ~12) |
        (shared->visual->flags & 12);
    object->phase = 0;
    object->velocity = 0;
    object->angle = 0;

    if ((flags & 0xffff0000) == 0 || config == 0)
        return;
    if (flags & 0x10000)
        Func_080929d8(object, config->parent);
    if (flags & 0x20000) {
        object->flags &= ~1;
        visual->flags = (visual->flags & ~12) |
            ((config->visual_flags & 3) << 2);
    }
    if (flags & 0x80000) {
        object->x = config->x;
        object->y = config->y;
    }
    if (flags & 0x40000) {
        s32 duration = *(s32 *)((u8 *)animation + 12);

        if (flags & 0x80000) {
            object->phase = Func_080022ec(config->target_x - object->x, duration);
            object->velocity = Func_080022ec(config->target_y - object->y, duration);
        } else {
            object->phase = Func_080022ec(config->target_x - 0x10000, duration);
            object->velocity = Func_080022ec(config->target_y - 0x10000, duration);
        }
    }
    if (flags & 0x200000) {
        Func_08009080(object, 1);
        Func_08009098(object, config->animation);
    }
    if (flags & 0x400000)
        visual->attribute = config->attribute;
    if (flags & 0x800000)
        object->angle = config->angle;
    if (flags & 0x01000000)
        object->callback = config->callback;
}
