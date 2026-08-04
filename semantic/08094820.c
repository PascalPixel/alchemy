#include "layout_guard.h"
#include "types.h"

typedef struct Particle_08094820 {
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
} Particle_08094820;

typedef struct ParticleManager_08094820 {
    u8 padding00[4];
    s32 tile_index;
    Particle_08094820 particles[32];
} ParticleManager_08094820;

typedef struct Position_08094820 {
    s32 x;
    s32 padding04;
    s32 z;
} Position_08094820;

typedef struct ViewState_08094820 {
    Position_08094820 *source;
    u8 padding04[0xe0];
    s32 x;
    s32 z;
} ViewState_08094820;

/* Entries are ten bytes apart; this compiler rounds the C type's size up. */
typedef struct ParticleFrame_08094820 {
    s16 x_offset;
    s16 y_offset;
    u16 tile_offset;
    u8 attr0_upper;
    u8 padding07;
    u8 attr1_upper;
} ParticleFrame_08094820;

LAYOUT_OFFSET_GUARD(
    Particle08094820_X,
    Particle_08094820,
    x,
    0x0c);
LAYOUT_OFFSET_GUARD(
    Particle08094820_GroundY,
    Particle_08094820,
    ground_y,
    0x10);
LAYOUT_OFFSET_GUARD(
    Particle08094820_Z,
    Particle_08094820,
    z,
    0x14);
LAYOUT_OFFSET_GUARD(
    Particle08094820_Lifetime,
    Particle_08094820,
    lifetime,
    0x1c);
LAYOUT_SIZE_GUARD(
    Particle08094820_Size,
    Particle_08094820,
    0x20);
LAYOUT_OFFSET_GUARD(
    ParticleManager08094820_Particles,
    ParticleManager_08094820,
    particles,
    8);
LAYOUT_OFFSET_GUARD(
    ViewState08094820_X,
    ViewState_08094820,
    x,
    0xe4);
LAYOUT_OFFSET_GUARD(
    ParticleFrame08094820_TileOffset,
    ParticleFrame_08094820,
    tile_offset,
    4);
LAYOUT_OFFSET_GUARD(
    ParticleFrame08094820_Attr1Upper,
    ParticleFrame_08094820,
    attr1_upper,
    8);

extern ViewState_08094820 *Data_03001e70;

void Func_08003dec(void *entry, s32 priority);
u32 Func_08004458(void);
s32 Func_080091a8(s32 layer, s32 x, s32 z);
s32 Func_080770c0(s32 flag);

static s32 FixedDeltaToPixel_08094820(s32 delta)
{
    if (delta < 0)
        delta += 0xffff;
    return delta >> 16;
}

static const ParticleFrame_08094820 *GetFrame_08094820(u16 lifetime)
{
    return (const ParticleFrame_08094820 *)(
        (const u8 *)0x0809ef84 + lifetime * 10);
}

static void RespawnParticle_08094820(
    Particle_08094820 *particle, const Position_08094820 *source)
{
    s32 x = source->x + (Func_08004458() << 8) - 0x800000;
    s32 z = source->z + (Func_08004458() << 8) - 0x800000;

    particle->x = x;
    particle->z = z;
    particle->ground_y =
        (s32)((u32)Func_080091a8(0, x >> 16, z >> 16) << 16);
    particle->lifetime = 16;
}

/*
 * Animate, project, and draw the manager's thirty-two terrain particles.
 * Expired entries immediately respawn in a square around the view source;
 * field flag 0x166 freezes their animation by restoring the decremented age.
 */
void Func_08094820(void)
{
    ViewState_08094820 *view = Data_03001e70;
    ParticleManager_08094820 *manager =
        *(ParticleManager_08094820 **)0x03001ec4;
    s32 index;

    for (index = 0; index < 32; index++) {
        Particle_08094820 *particle = &manager->particles[index];

        particle->lifetime--;
        if (particle->lifetime != 0xffff) {
            const ParticleFrame_08094820 *frame;
            s32 screen_x;
            s32 screen_y;
            u8 *attr1_high;

            if (Func_080770c0(0x166) != 0)
                particle->lifetime++;

            frame = GetFrame_08094820(particle->lifetime);
            screen_x = FixedDeltaToPixel_08094820(
                particle->x - view->x) + frame->x_offset;
            screen_y = FixedDeltaToPixel_08094820(
                particle->z - particle->ground_y - view->z) +
                frame->y_offset;

            if ((u32)(screen_x + 16) <= 255 &&
                screen_y >= -32 && screen_y <= 159) {
                particle->attr0_high =
                    (particle->attr0_high & (u8)~0x0c) | 4;
                particle->attr1 =
                    (particle->attr1 & 0xfe00) |
                    (screen_x & 0x01ff);
                particle->screen_y = (s8)screen_y;
                particle->attr2 =
                    (particle->attr2 & 0xfc00) |
                    ((manager->tile_index + frame->tile_offset) & 0x03ff);
                particle->attr0_high =
                    (particle->attr0_high & 0x3f) |
                    (frame->attr0_upper << 6);
                attr1_high = ((u8 *)&particle->attr1) + 1;
                *attr1_high =
                    (*attr1_high & 0x3f) |
                    (frame->attr1_upper << 6);
                Func_08003dec(particle, 0xf0);
            }

            if (particle->lifetime == 0)
                RespawnParticle_08094820(particle, view->source);
        }
    }
}
