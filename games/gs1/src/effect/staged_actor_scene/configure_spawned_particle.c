#include "types.h"

struct SourceEntity {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct ParticleRecord {
    u8 f00[9];
    u8 f09;
};

struct StagedParticle {
    u8 f00[0x48];
    s32 f48;
    s32 f4c;
    struct ParticleRecord *f50;
    u8 f54;
    u8 f55;
    u8 f56[8];
    u16 f5e;
};

extern s32 Func_0200b60e(struct SourceEntity *);
extern s32 Func_0200b622(void);
extern s32 Func_0200b628(void);
extern void Func_0200b656(s32, s32, s32 *);
extern struct StagedParticle *Func_0200b6a2(s32, s32, s32, s32);
extern void Func_0200b708(struct StagedParticle *, s32);
extern void Func_0200b6b8(struct StagedParticle *, s32);
extern void Func_0200b6c8(struct StagedParticle *, s32);
extern void Func_0200b92e(s32);
extern s32 Data_0200e6e0[];

void Effect_ConfigureSpawnedParticle(struct SourceEntity *source)
{
    s32 spawn_position[3];
    s32 particle_index;
    struct StagedParticle *particle;
    spawn_position[0] = source->f08;
    spawn_position[1] = source->f0c - (Func_0200b60e(source) << 4) + (s32)0xfff80000;
    spawn_position[2] = source->f10;
    particle_index = Func_0200b622();
    Func_0200b656(((particle_index << 1) + particle_index) << 4, Func_0200b628(), spawn_position);
    particle = Func_0200b6a2(0x11d, spawn_position[0], spawn_position[1], spawn_position[2]);
    if (particle != 0) {
        particle->f55 = 2;
        particle->f48 = 0x1999;
        particle->f5e = 12;
        Func_0200b708(particle, 0);
        Func_0200b6b8(particle, 0);
        Func_0200b6c8(particle, (s32)Data_0200e6e0);
        {
            struct ParticleRecord *record = particle->f50;
            s32 record_flags = ~12;
            record_flags &= record->f09;
            record_flags |= 4;
            record->f09 = record_flags;
        }
    }
    Func_0200b92e(0x8a);
}
