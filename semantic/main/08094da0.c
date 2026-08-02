#include "layout_guard.h"
#include "types.h"

typedef struct DmaChannel_08094da0 {
    const void *source;
    void *destination;
    u32 control;
} DmaChannel_08094da0;

typedef struct BlendRegisters_08094da0 {
    u16 control;
    u16 alpha;
    u16 brightness;
} BlendRegisters_08094da0;

typedef struct Position_08094da0 {
    s32 x;
    s32 padding04;
    s32 z;
} Position_08094da0;

typedef struct ViewState_08094da0 {
    Position_08094da0 *source;
} ViewState_08094da0;

typedef struct Particle_08094da0 {
    u32 active;
    u32 oam_config;
    u32 tile_config;
    s32 x;
    s32 ground_y;
    s32 z;
    s32 vertical_offset;
    u16 lifetime;
    u8 padding1e[2];
} Particle_08094da0;

typedef struct ParticleManager_08094da0 {
    s32 resource;
    s32 tile_index;
    Particle_08094da0 particles[32];
    u8 padding408[8];
} ParticleManager_08094da0;

LAYOUT_OFFSET_GUARD(
    Particle08094da0_X,
    Particle_08094da0,
    x,
    0x0c);
LAYOUT_OFFSET_GUARD(
    Particle08094da0_Lifetime,
    Particle_08094da0,
    lifetime,
    0x1c);
LAYOUT_SIZE_GUARD(
    Particle08094da0_Size,
    Particle_08094da0,
    0x20);
LAYOUT_OFFSET_GUARD(
    ParticleManager08094da0_Particles,
    ParticleManager_08094da0,
    particles,
    8);
LAYOUT_SIZE_GUARD(
    ParticleManager08094da0_Size,
    ParticleManager_08094da0,
    0x410);

extern ViewState_08094da0 *Data_03001e70;

void Func_08002dd8(s32 allocation);
void *Func_080048f4(s32 allocation, s32 size);
s32 Func_08003fa4(s32 resource, s32 size, const void *source);
s32 Func_08004080(void);
void Func_080041d8(void (*callback)(void), s32 period);
void Func_080053e8(const void *source, void *destination);
s32 Func_080091a8(s32 layer, s32 x, s32 z);
void Func_08094bbc(void);

/* Allocate and seed the 32-particle terrain effect, then start its updater. */
void Func_08094da0(void)
{
    volatile DmaChannel_08094da0 *dma =
        (volatile DmaChannel_08094da0 *)0x040000d4;
    volatile BlendRegisters_08094da0 *blend =
        (volatile BlendRegisters_08094da0 *)0x04000050;
    ParticleManager_08094da0 *manager =
        Func_080048f4(0x1d, sizeof(*manager));
    Position_08094da0 *source = Data_03001e70->source;
    void *scratch;
    u32 zero = 0;
    s32 index;

    dma->source = &zero;
    dma->destination = manager;
    dma->control = 0x85000104;

    scratch = Func_080048f4(0x0e, 0x400);
    Func_080053e8((const void *)0x080a001e, scratch);
    manager->resource = Func_08004080();
    manager->tile_index =
        Func_08003fa4(manager->resource, 0x300, scratch);
    Func_08002dd8(0x0e);

    for (index = 0; index < 32; index++) {
        Particle_08094da0 *particle = &manager->particles[index];

        particle->active = 0;
        particle->oam_config = 0x40000400;
        particle->tile_config = 0x0000d400;
        particle->x = source->x;
        particle->z = source->z;
        particle->ground_y =
            Func_080091a8(0, source->x >> 16, source->z >> 16) << 16;
        particle->lifetime = (index & 0x0f) + 1;
    }

    blend->control = 0x3f00;
    blend->alpha = 0x1008;
    blend->brightness = 0;
    Func_080041d8(Func_08094bbc, 0xc80);
}
