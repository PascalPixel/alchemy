#include "layout_guard.h"
#include "types.h"

typedef struct Position_08094e7c {
    s32 x;
    s32 padding04;
    s32 z;
} Position_08094e7c;

typedef struct ViewState_08094e7c {
    Position_08094e7c *source;
    u8 padding004[0xe0];
    s32 x;
    s32 z;
} ViewState_08094e7c;

typedef struct ParticleFrame_08094e7c {
    s16 y_offset;
    u16 tile_offset;
} ParticleFrame_08094e7c;

typedef struct Particle_08094e7c {
    u8 padding00[4];
    s8 screen_y;
    u8 attr0_high;
    u16 attr1;
    u16 attr2;
    u8 padding0a[2];
    s32 x;
    s32 ground_y;
    s32 z;
    s32 vertical_offset;
    u16 lifetime;
    u8 padding1e[2];
} Particle_08094e7c;

typedef struct ParticleManager_08094e7c {
    s32 resource;
    s32 tile_index;
    Particle_08094e7c particles[32];
    u8 padding408[4];
    s32 spawning_disabled;
} ParticleManager_08094e7c;

LAYOUT_OFFSET_GUARD(
    ViewState08094e7c_X,
    ViewState_08094e7c,
    x,
    0xe4);
LAYOUT_OFFSET_GUARD(
    Particle08094e7c_X,
    Particle_08094e7c,
    x,
    0x0c);
LAYOUT_OFFSET_GUARD(
    Particle08094e7c_Lifetime,
    Particle_08094e7c,
    lifetime,
    0x1c);
LAYOUT_SIZE_GUARD(
    Particle08094e7c_Size,
    Particle_08094e7c,
    0x20);
LAYOUT_OFFSET_GUARD(
    ParticleManager08094e7c_Particles,
    ParticleManager_08094e7c,
    particles,
    8);
LAYOUT_OFFSET_GUARD(
    ParticleManager08094e7c_SpawningDisabled,
    ParticleManager_08094e7c,
    spawning_disabled,
    0x40c);
LAYOUT_SIZE_GUARD(
    ParticleManager08094e7c_Size,
    ParticleManager_08094e7c,
    0x410);

extern u32 Data_03001e40;
extern ViewState_08094e7c *Data_03001e70;
extern ParticleManager_08094e7c *Data_03001ec4;

void Func_08003dec(void *, s32);
u32 Func_08004458(void);
s32 Func_080091a8(s32, s32, s32);
s32 Func_080770c0(s32);

static s32 FixedDeltaToPixel_08094e7c(s32 delta)
{
    if (delta < 0)
        delta += 0xffff;
    return delta >> 16;
}

static void SeedParticle_08094e7c(
    Particle_08094e7c *particle,
    s32 x,
    s32 z,
    u16 lifetime,
    s32 terrain_x,
    s32 terrain_z)
{
    particle->x = x;
    particle->z = z;
    particle->ground_y = Func_080091a8(0, terrain_x, terrain_z) << 16;
    particle->lifetime = lifetime;
    particle->vertical_offset = 0;
}

/*
 * Animate and draw the manager's thirty-two terrain particles.  Empty slots
 * can begin a four-particle burst around the view source; later empty slots in
 * the same pass inherit that position with staggered lifetimes.
 */
void Func_08094e7c(void)
{
    ParticleManager_08094e7c *manager = Data_03001ec4;
    ViewState_08094e7c *view = Data_03001e70;
    s32 burst_x = 0;
    s32 burst_z = 0;
    s32 screen_x = 0;
    s32 screen_y = 0;
    u32 spawned = 0;
    s32 burst_step = 0;
    s32 index;

    for (index = 0; index < 32; index++) {
        Particle_08094e7c *particle = &manager->particles[index];

        if (particle->lifetime != 0) {
            const ParticleFrame_08094e7c *frame;
            s32 projected_x;
            u32 first_random;
            u32 second_random;

            if (Func_080770c0(0x166) != 0)
                particle->lifetime++;

            frame = (const ParticleFrame_08094e7c *)(
                0x0809f024 + ((particle->lifetime >> 1) * 4));
            first_random = Func_08004458();
            second_random = Func_08004458();
            projected_x =
                ((particle->x - view->x) >> 16) +
                (((first_random & 1) + (second_random & 1)) >> 1);
            screen_x = projected_x - 1;
            screen_y = FixedDeltaToPixel_08094e7c(
                particle->z - particle->ground_y - view->z) +
                frame->y_offset;

            if ((u32)(projected_x + 15) <= 255 &&
                screen_y >= -32 && screen_y <= 159) {
                particle->attr2 =
                    (particle->attr2 & 0xfc00) |
                    ((manager->tile_index + frame->tile_offset) & 0x03ff);
                particle->attr1 =
                    (particle->attr1 & 0xfe00) | (screen_x & 0x01ff);
                particle->attr0_high &= 0x3f;
                particle->screen_y = (s8)screen_y;
                ((u8 *)&particle->attr1)[1] =
                    (((u8 *)&particle->attr1)[1] & 1) |
                    0x40 |
                    (((Data_03001e40 >> 1) & 1) << 4);
                Func_08003dec(particle, 0xf0);
            }

            particle->lifetime--;
        }

        if (spawned <= 3 &&
            particle->lifetime == 0 &&
            manager->spawning_disabled == 0) {
            if (burst_step != 0) {
                SeedParticle_08094e7c(
                    particle,
                    burst_x,
                    burst_z,
                    (u16)(0x3e - burst_step),
                    screen_x >> 16,
                    screen_y >> 16);
                spawned++;
                burst_step += 4;
            } else if ((u8)Func_08004458() == 0) {
                Position_08094e7c *source = view->source;

                burst_x = source->x + (Func_08004458() << 8) - 0x800000;
                burst_z = source->z + (Func_08004458() << 8) - 0x800000;
                SeedParticle_08094e7c(
                    particle,
                    burst_x,
                    burst_z,
                    30,
                    screen_x >> 16,
                    screen_y >> 16);
                spawned++;
                burst_step = 4;
            }
        }
    }
}
