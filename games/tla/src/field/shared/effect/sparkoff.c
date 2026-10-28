#include "types.h"
#include "callback_scheduler.h"
#include "dma.h"
#include "edition.h"
#include "io_reg.h"
#include "iowrite.h"
#include "map.h"
#include "ram_buffer.h"

s32 Map_GetTerrainHeightFar(s32 layer, s32 x, s32 z);

/// One of Func_080d6674's particles: its sprite's attribute words, its world
/// position and its frames left.
struct SparkParticle {
  u32 sprite[3];
  s32 posX;
  s32 posY;
  s32 posZ;
  u8 unk18[4];
  u16 timer;
  u8 unk1e[2];
};

/// Heap slot 0x74 while the particles run.
struct SparkParticleWork {
  s32 vramEntry;
  s32 tile;
  struct SparkParticle particles[64];
};

void *Runtime_AllocateBlock(s32 slot, s32 size);
void Runtime_ReleaseHeapBlock(s32 slot);
s32 Resource_DecodeByteLzInRam(const void *source, void *destination);
s32 Resource_FindFreeEntry(void);
s32 VramCache_Load(u32 slot, u32 size, const void *source);
void Func_080d6674(void);
void Func_080d2c64(s32 cue);

extern const u8 Data_080f3901[];

/// Starts Func_080d6674's particles: allocates and clears heap slot 0x74,
/// decodes the particle tiles into VRAM, sets each of the 64 particles' sprite
/// words and places it on the ground at the map's focus with a timer of
/// (i & 11) + 1, turns on alpha blending of all layers at 16/16, schedules the
/// update and, when asked, plays cue 137 (Func_080d2c64).
void Func_080d6f34(s32 withCue) {
  struct SparkParticleWork *work = Runtime_AllocateBlock(116, 0x814);
  struct SparkParticle *particle = work->particles;
  volatile u32 fill;
  /* FAKEMATCH: the register variable builds the fill word's zero in r1
   * (differ 8 without it: it takes r3, set before the work pointer copy). */
  register u32 zero asm("r1");
  u8 *tiles;
  u32 i;
  struct HeapSlots *slots;
  /* FAKEMATCH: the register variable keeps the cleared word's zero set inside
   * the loop on every pass and in r6 for the last blend register (differ 4
   * without it). */
  register s32 clear asm("r6");

  zero = 0;
  Dma_Fill32Cell(fill, zero, work, 0x814);
  tiles = Runtime_AllocateBlock(56, 0x400);
  Resource_DecodeByteLzInRam(Data_080f3901, tiles);
  work->vramEntry = Resource_FindFreeEntry();
  work->tile = VramCache_Load(work->vramEntry, 0x300, tiles);
  Runtime_ReleaseHeapBlock(56);
  i = 0;
  slots = Ram_HeapSlots;
  for (; i < 64; i++, particle++) {
    struct MapWork *view = slots->mapWork;
    s32 *focus;
    /* FAKEMATCH: reusing the integer field cursor for the height's x argument
     * keeps the stores in r1 (a word pointer with x of its own differs 18). */
    u32 p;
    s32 z;

    /* FAKEMATCH: keeps the map work load before the cursor copy (differ 4
     * without it). */
    asm("" : : "r"(view));
    p = (u32)particle;
    clear = 0;
    *(s32 *)p = clear;
    p += sizeof(s32);
    /* FAKEMATCH: keeps the first store before the focus load (differ 8
     * without it). */
    asm("" : : "r"(view));
    focus = view->origin;
    *(s32 *)p = 0x40000400;
    p += sizeof(s32);
    *(s32 *)p = 0xd400;
    p = focus[0];
    z = focus[2];
    particle->posX = (s32)p;
    particle->posZ = z;
    particle->posY = Map_GetTerrainHeightFar(0, (s32)p, z);
    particle->timer = (i & 11) + 1;
  }
  Io_Write16(0x3f00, &REG_BLDCNT);
  Io_Write16(0x1010, &REG_BLDALPHA);
  Io_Write16(clear, &REG_BLDY);
  Scheduler_AddOrUpdateCallback(Func_080d6674, 0x480);
  if (withCue != 0)
    Func_080d2c64(137);
}

void Func_080d7024(void) { Scheduler_RemoveCallback(Func_080d6674); }
