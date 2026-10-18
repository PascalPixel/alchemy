// Near miss, differ 10 of 85: starts Func_080d69f4's rising particles:
// allocates and clears the heap slot 0x74 workspace, decodes the particle
// tiles into VRAM, sets each of the 64 particles' sprite words and places it
// on the ground at a random point of the strip, staggering their timers by 2
// frames, and schedules the update. No ☀️ twin found. Left: the fill word's
// zero: the native builds it in r2 and starts the DMA address (movs r3, #128)
// before it, this draft builds it in r3 first; and the callback's pointer
// load sits one slot later at the end (instruction order).
#include "types.h"
#include "dma.h"

struct RisingParticle {
  u32 sprite[3];
  s32 posX;
  s32 posY;
  s32 posZ;
  s32 drift;
  u16 timer;
  u8 unk1e[2];
};

struct RisingParticleWork {
  s32 vramEntry;
  s32 tile;
  struct RisingParticle particles[64];
};

void *Runtime_AllocateBlock(s32 slot, s32 size);
void Runtime_ReleaseHeapBlock(s32 slot);
s32 Resource_DecodeByteLzInRam(const void *source, void *destination);
s32 Resource_FindFreeEntry(void);
s32 VramCache_Load(u32 slot, u32 size, const void *source);
u32 Random16(void);
s32 Map_GetTerrainHeightFar(u8 layer, s32 x, s32 z);
void Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 priority);
void Func_080d69f4(void);

extern const u8 gData_080f385e[];

#define DMA3 ((volatile u32 *)0x040000d4)

void Func_080d6b90(void) {
  struct RisingParticleWork *work = Runtime_AllocateBlock(116, 0x814);
  struct RisingParticle *particle = work->particles;
  volatile u32 fill;
  u8 *tiles;
  u32 i;

  Dma_Fill32Cell(fill, 0, work, 0x814);
  tiles = Runtime_AllocateBlock(56, 0x400);
  Resource_DecodeByteLzInRam(gData_080f385e, tiles);
  work->vramEntry = Resource_FindFreeEntry();
  work->tile = VramCache_Load(work->vramEntry, 0x300, tiles);
  Runtime_ReleaseHeapBlock(56);
  for (i = 0; i < 64; i++, particle++) {
    u32 *sprite = particle->sprite;
    s32 x;
    s32 z;

    *sprite++ = 0;
    *sprite++ = 0x40000400;
    *sprite = 0xd400;
    x = Random16() * 200 + 0x1200000;
    z = 0x500000 - Random16() * 20;
    particle->posX = x;
    particle->posZ = z;
    particle->posY = Map_GetTerrainHeightFar(0, x, z);
    particle->drift = 0;
    particle->timer = i * 2;
  }
  Scheduler_AddOrUpdateCallback(Func_080d69f4, 0x480);
}
