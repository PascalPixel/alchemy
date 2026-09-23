/* Draft, not exact (2026-09-24): 13 differing halfwords, 194 of 196
   bytes. Residual: register allocation only; the reference keeps the DMA
   fill zero in r1, and in the loop the camera pointer in r2 and the OAM
   cursor in r1. Sibling of main:08094da0. */

#include "DMA.H"

struct FallingParticle {
    u32 oam[3];
    s32 x;
    s32 y;
    s32 z;
    s32 unknown_18;
    u16 delay;
    u16 unknown_1e;
};

struct ParticleWork {
    s32 slot;
    s32 tile;
    struct FallingParticle particles[32];
};

extern s32 **Data_03001e70;

void *Func_080048f4(s32 kind, s32 size);
s32 Func_080053e8(const void *source, void *destination);
s32 Func_08004080(void);
s32 Func_08003fa4(u32 slot, u32 size, const void *source);
void Func_08002dd8(s32 kind);
s32 Func_080091a8(s32 layer, s32 x, s32 z);
void Func_080041d8(void (*callback)(void), s32 flags);
void Func_08094e7c(void);

void Func_0809509c(void)
{
    struct ParticleWork *work;
    struct FallingParticle *particle;
    void *buffer;
    s32 *camera;
    u32 *oam;
    u32 i;
    s32 x;
    s32 z;
    volatile u32 zero;

    work = Func_080048f4(29, 0x410);
    particle = work->particles;
    zero = 0;
    Dma_Set((const void *)&zero, work, 0x85000104, (volatile u32 *)0x040000d4);
    buffer = Func_080048f4(14, 0x400);
    Func_080053e8((const void *)0x080a00b8, buffer);
    work->slot = Func_08004080();
    work->tile = Func_08003fa4(work->slot, 0x200, buffer);
    Func_08002dd8(14);
    for (i = 0; i <= 31; i++, particle++) {
        camera = *Data_03001e70;
        oam = particle->oam;
        *oam++ = 0;
        *oam++ = 0x40000400;
        *oam = 0xd400;
        x = camera[0];
        z = camera[2];
        particle->x = 0;
        particle->z = 0;
        particle->y = Func_080091a8(0, x >> 16, z >> 16) << 16;
        particle->delay = (i & 15) + 1;
    }
    Func_080041d8(Func_08094e7c, 0xc80);
}
