// Draft, differ 56 of 100: starts Func_080d6674's ground particles: allocates
// and clears the heap slot 0x74 workspace, decodes the particle tiles into
// VRAM, sets each of the 64 particles' sprite words and places it on the
// ground at the map's focus with a timer of (i & 11) + 1, turns on alpha
// blending of all layers at 16/16, schedules the update and, when asked,
// calls Func_080d2c64(137). No ☀️ twin found. Left: loop.c's invariant: the
// native hoists the heap slots' base 0x03000000 into r8 (argument in r10, a
// second callee-saved register pushed); agscc builds it in the loop. The rest
// follows: the fill's zero in r1 (here r3), the sprite pointer in r1 (here
// r2), and the focus loads ordered x before z. The blend registers are
// written with Io_Write16 (as REG_ stores their constants come from the pool).
#include "types.h"
#include "dma.h"
#include "io_reg.h"
#include "iowrite.h"
#include "ram_buffer.h"

struct GroundParticle {
  u32 sprite[3];
  s32 posX;
  s32 posY;
  s32 posZ;
  u8 unk18[4];
  u16 timer;
  u8 unk1e[2];
};

struct GroundParticleWork {
  s32 vramEntry;
  s32 tile;
  struct GroundParticle particles[64];
};

void *Runtime_AllocateBlock(s32 slot, s32 size);
void Runtime_ReleaseHeapBlock(s32 slot);
s32 Resource_DecodeByteLzInRam(const void *source, void *destination);
s32 Resource_FindFreeEntry(void);
s32 VramCache_Load(u32 slot, u32 size, const void *source);
s32 Map_GetTerrainHeightFar(u8 layer, s32 x, s32 z);
void Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 priority);
void Func_080d6674(void);
void Func_080d2c64(s32 value);

extern const u8 Data_080f3901[];

#define DMA3 ((volatile u32 *)0x040000d4)

void Func_080d6f34(s32 withCue) {
  struct GroundParticleWork *work = Runtime_AllocateBlock(116, 0x814);
  struct GroundParticle *particle = work->particles;
  volatile u32 fill;
  u8 *tiles;
  u32 i;

  Dma_Fill32Cell(fill, 0, work, 0x814);
  tiles = Runtime_AllocateBlock(56, 0x400);
  Resource_DecodeByteLzInRam(Data_080f3901, tiles);
  work->vramEntry = Resource_FindFreeEntry();
  work->tile = VramCache_Load(work->vramEntry, 0x300, tiles);
  Runtime_ReleaseHeapBlock(56);
  for (i = 0; i < 64; i++, particle++) {
    s32 *focus = *(s32 **)Ram_HeapSlots->mapWork;
    u32 *sprite = particle->sprite;
    s32 x;
    s32 z;

    *sprite++ = 0;
    *sprite++ = 0x40000400;
    *sprite = 0xd400;
    x = focus[0];
    z = focus[2];
    particle->posX = x;
    particle->posZ = z;
    particle->posY = Map_GetTerrainHeightFar(0, x, z);
    particle->timer = (i & 11) + 1;
  }
  Io_Write16(0x3f00, &REG_BLDCNT);
  Io_Write16(0x1010, &REG_BLDALPHA);
  Io_Write16(0, &REG_BLDY);
  Scheduler_AddOrUpdateCallback(Func_080d6674, 0x480);
  if (withCue != 0)
    Func_080d2c64(137);
}
