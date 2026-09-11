#include "types.h"
#include "scene.h"

/* battle/effects/particles/update_motion_and_scale.c */
s32 FixedPoint_Ratio(s32, s32);

void ParticleEffect_UpdateMotionAndScale(void *arg0)
{
    u8 *a = arg0;
    s32 a44 = *(volatile s32 *)(a + 0x44);
    s32 a4c;

    *(volatile s32 *)(a + 0x08) += a44;
    *(volatile s32 *)(a + 0x0C) += *(s32 *)(a + 0x48);
    a4c = *(s32 *)(a + 0x4C);
    *(volatile s32 *)(a + 0x10) += a4c;
    *(volatile s32 *)(a + 0x44) = a44 - FixedPoint_Ratio(a44, 0x12);
    *(volatile s32 *)(a + 0x4C) = a4c - a4c / 16;
    *(volatile s32 *)(a + 0x18) += *(s32 *)(a + 0x30);
    *(volatile s32 *)(a + 0x1C) += *(s32 *)(a + 0x34);
    *(volatile u16 *)(*(u8 **)(a + 0x50) + 0x1E) += *(u16 *)(a + 0x64);
}

/* battle/effects/particles/spawn_radial_particle_ring.c */
extern u8 gRom;

void BattleFx_SpawnRadialParticleRing(void *origin)
{
    s32 particle_config[10];
    volatile u8 stack_padding[8];
    s32 work;
    s32 *velocity;
    s32 *config;
    s32 sine;
    s32 angle;
    u32 scaled_cosine;
    u32 particle_index;

    (void)stack_padding;
    config = particle_config;
    FIELD_AT_OFFSET(config, s32 *, 4) = 0;
    FIELD_AT_OFFSET(config, s32 *, 0x24) = (s32)&gRom;
    FIELD_AT_OFFSET(config, s32 *, 8) = 0xCCCC;
    FIELD_AT_OFFSET(config, s32 *, 0xC) = 0xCCCC;
    particle_index = 0;
    velocity = &work;
    do {
        angle = particle_index << 0xC;
        scaled_cosine = Battle_Check(angle)* 3;
        FIELD_AT_OFFSET(velocity, s32 *, 0) = (s32)((s32)(scaled_cosine + (scaled_cosine >> 0x1F)) >> 1);
        FIELD_AT_OFFSET(velocity, s32 *, 4) = 0;
        sine = Battle_unk2(angle);
        FIELD_AT_OFFSET(velocity, s32 *, 8) = sine;
        Battle_SetRect(FIELD_AT_OFFSET(origin, s32 *, 8), FIELD_AT_OFFSET(origin, s32 *, 0xC), FIELD_AT_OFFSET(origin, s32 *, 0x10), FIELD_AT_OFFSET(velocity, s32 *, 0), FIELD_AT_OFFSET(velocity, s32 *, 4), sine, 0x01090001, config);
        particle_index += 1;
    } while (particle_index <= 0x10U);
}

/* battle/effects/obj/shrink_object_scale_until_half.c */
void BattleFx_ShrinkObjectScaleUntilHalf(void *obj)
{
    s32 scale;

    scale = FIELD_AT_OFFSET(obj, s32 *, 0x18) - 0x80;
    FIELD_AT_OFFSET(obj, s32 *, 0x1C) = scale;
    FIELD_AT_OFFSET(obj, s32 *, 0x18) = scale;
    if (scale < 0x8000) {
        Battle_SetMode(obj, 0, 0, 0);
        FIELD_AT_OFFSET(obj, s32 *, 0x6C) = 0;
    }
}

/* battle/effects/call/call_effect_14.c */
void BattleFx_CallEffect14(void)
{
    Battle_Check();
}
