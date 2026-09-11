#include "types.h"
#include "scene.h"
#include "abi/battle/effects/particles/spawn_radial_particle_ring.h"


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
        sine = Battle_Check2(angle);
        FIELD_AT_OFFSET(velocity, s32 *, 8) = sine;
        Battle_SetRect(FIELD_AT_OFFSET(origin, s32 *, 8), FIELD_AT_OFFSET(origin, s32 *, 0xC), FIELD_AT_OFFSET(origin, s32 *, 0x10), FIELD_AT_OFFSET(velocity, s32 *, 0), FIELD_AT_OFFSET(velocity, s32 *, 4), sine, 0x01090001, config);
        particle_index += 1;
    } while (particle_index <= 0x10U);
}
