#include "types.h"

#define BattleEffect_SpawnBurstParticleField Func_08098698

struct BurstParticleVisual {
    u8 reserved_00[5];
    u8 flags_a;
    u8 reserved_06;
    u8 flags_b;
    u16 attributes;
    u8 reserved_0a[2];
};

struct BurstParticleVisualGroup {
    struct BurstParticleVisual primary;
    struct BurstParticleVisual child;
};

struct BurstParticleObject {
    u8 reserved_00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 reserved_14[4];
    s32 scale_x;
    s32 scale_y;
    u8 reserved_20[16];
    s32 velocity_x;
    s32 velocity_y;
    u8 reserved_38[24];
    struct BurstParticleVisualGroup *visuals;
    u8 reserved_54;
    u8 mode;
};

struct BurstParticleState {
    s32 variant;
    s32 x;
    s32 y;
    s32 z;
    struct BurstParticleObject *target;
};

extern struct BurstParticleState *Data_03001f30;
extern u8 Data_0809f0b4;

void Func_08097384(void);
void Func_0800447c(s32, u32, s32 *);
struct BurstParticleObject *Func_08096c80(s32, s32, s32, s32);
void Func_08009240(struct BurstParticleObject *, s32);
void Func_08009080(struct BurstParticleObject *, s32);
void Func_08009098(struct BurstParticleObject *, void *);
void Func_080091e0(struct BurstParticleObject *, s32);
u32 Func_08004458(void);
void Func_08009150(struct BurstParticleObject *, s32, s32, s32);
void Func_080f9010(s32);
void Func_080030f8(s32);

void BattleEffect_SpawnBurstParticleField(void)
{
    struct BurstParticleState *state = Data_03001f30;
    struct BurstParticleObject *target = state->target;
    s32 position[3];
    s32 remaining = 23;

    Func_08097384();
    do {
        struct BurstParticleObject *object;
        struct BurstParticleVisual *visual;
        struct BurstParticleVisual *child;
        s32 random_distance;

        position[0] = target->x;
        position[2] = target->z;
        if (state->variant == 0x4000)
            position[1] = target->y + 0xa0000;
        else if (state->variant == 0xc000)
            position[1] = target->y + 0x180000;
        else {
            position[1] = target->y + 0xa0000;
            Func_0800447c(0xa0000, state->variant, position);
        }

        object = Func_08096c80(
            0x11c, position[0], position[1], position[2]);
        visual = &object->visuals->primary;
        child = &object->visuals->child;
        child->flags_a = (child->flags_a & ~0x20) |
            (visual->flags_a & 0x20);
        child->flags_a = (child->flags_a & 0x3f) |
            (visual->flags_a & 0xc0);
        child->flags_b = (child->flags_b & 0x3f) |
            (visual->flags_b & 0xc0);
        child->attributes = (child->attributes & 0xfc00) |
            (visual->attributes & 0x03ff);
        child->reserved_0a[0] = (child->reserved_0a[0] & 0x0f) |
            (visual->reserved_0a[0] & 0xf0);

        if (object != 0) {
            object->scale_x = 0xb333;
            object->scale_y = 0xb333;
            object->velocity_x = 0x18000;
            object->velocity_y = 0x18000;
            object->mode = 0;
            Func_08009240(object, 11);
            Func_08009080(object, 7);
            Func_08009098(object, &Data_0809f0b4);
            Func_080091e0(object, 1);

            position[0] = state->x;
            position[1] = state->y;
            position[2] = state->z;
            if (state->variant == 0xc000)
                Func_0800447c(0xe0000, state->variant, position);
            random_distance = Func_08004458() * 6 + 0x40000;
            Func_0800447c(random_distance, Func_08004458(), position);
            Func_08009150(
                object, position[0], position[1], position[2]);
        }
        Func_080f9010(0x83);
        Func_080030f8(2);
        remaining--;
    } while (remaining >= 0);
    Func_080030f8(8);
}
