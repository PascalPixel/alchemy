#include "types.h"
#include "dma.h"
#include "edition.h"

s32 Map_GetTerrainHeightFar(s32 layer, s32 x, s32 z);

/// One rising particle: its sprite's attribute words, world position, drift
/// and frames left.
struct RisingParticle {
  u32 sprite[3];
  s32 posX;
  s32 posY;
  s32 posZ;
  s32 drift;
  u16 timer;
  u8 unk1e[2];
};

/// Heap slot 0x74 while the rising particles run.
struct RisingParticleWork {
  s32 vramEntry;
  s32 tile;
  struct RisingParticle particles[64];
};

void *HeapSlot_AllocEwram(s32 slot, s32 size);
void HeapSlot_Release(s32 slot);
s32 Resource_DecodeByteLzInRam(const void *source, void *destination);
s32 Resource_FindFreeEntry(void);
s32 VramCache_Load(u32 slot, u32 size, const void *source);
u32 Random16(void);
void Task_Add(void (*func)(void), s32 priority);
void Func_080d69f4(void);

extern const u8 gData_080f385e[];

/// Starts Func_080d69f4's rising particles: allocates and clears heap slot
/// 0x74, decodes the particle tiles into VRAM, sets each of the 64 particles'
/// sprite words and places it on the ground at a random point of the strip
/// (x 288 to 488 pixels, z 60 to 80), staggering their timers by 2 frames, and
/// schedules the update.
void Func_080d6b90(void) {
  struct RisingParticleWork *work = HeapSlot_AllocEwram(116, 0x814);
  struct RisingParticle *particle = work->particles;
  volatile u32 fill;
  /* FAKEMATCH: the register variable builds the fill word's zero in r2,
   * after the DMA address is started (differ 8 without it: the zero
   * takes r3 first). */
  register u32 zero asm("r2");
  s32 priority;
  u8 *tiles;
  u32 i;

  zero = 0;
  Dma_Fill32Cell(fill, zero, work, 0x814);
  tiles = HeapSlot_AllocEwram(56, 0x400);
  Resource_DecodeByteLzInRam(gData_080f385e, tiles);
  work->vramEntry = Resource_FindFreeEntry();
  work->tile = VramCache_Load(work->vramEntry, 0x300, tiles);
  HeapSlot_Release(56);
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
  priority = 0x480;
  /* FAKEMATCH: the barrier builds the priority before the task function's pool
   * load, as the native does (differ 2 without it). */
  asm volatile("");
  Task_Add(Func_080d69f4, priority);
}
