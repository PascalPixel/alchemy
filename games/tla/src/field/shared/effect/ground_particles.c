#include "types.h"
#include "task.h"
#include "dma.h"
#include "io_reg.h"
#include "iowrite.h"
#include "map.h"
#include "mem.h"
#include "ram_buffer.h"
#include "resource.h"
#include "edition.h"

struct GroundParticle {
  u32 attributes[3];
  s32 posX;
  s32 posY;
  s32 posZ;
  s32 delay;
  u16 timer;
  u8 unk1e[2];
};

struct GroundParticleWork {
  s32 entry;
  s32 tileBase;
  struct GroundParticle particles[64];
  s32 unk808;
  s32 stopped;
  s32 unk810;
};

void Func_080d2c64(s32 sound);
void Func_080d64f0(void);
s32 Resource_DecodeByteLzInRam(const void *source, void *destination);
s32 Map_GetTerrainHeightFar(s32 layer, s32 x, s32 z);
extern const u8 gData_080f3798[];

/// Starts the ground particles: 64 sprites placed at the leader's position on
/// the terrain, their tiles decoded into VRAM, the blend set, and
/// Func_080d64f0 scheduled at priority 0x480 to move them.
void Func_080d690c(void) {
  struct GroundParticleWork *work;
  s32 *leader;
  struct GroundParticle *particle;
  u32 *words;
  u32 i;

  /* FAKEMATCH: as in The Broken Seal, one pointer, words, holds the fill's
   * zero, the decode buffer and each particle's cell cursor, all in r6 as the
   * native keeps them; a plain zero for the fill differs 6. */
  work = HeapSlot_AllocEwram(116, sizeof(struct GroundParticleWork));
  leader = Ram_HeapSlots->mapWork->origin;
  Func_080d2c64(170);
  words = 0;
  particle = work->particles;
  Dma_Fill32((u32)words, work, sizeof(struct GroundParticleWork));
  words = HeapSlot_AllocEwram(56, 0x400);
  Resource_DecodeByteLzInRam(gData_080f3798, words);
  work->entry = Resource_FindFreeEntry();
  work->tileBase = VramCache_Load(work->entry, 0x300, words);
  HeapSlot_Release(56);
  for (i = 0; i < 64; i++) {
    s32 x;
    s32 z;

    words = (u32 *)particle;
    *words++ = 0;
    /* FAKEMATCH: the one-pass block stores the first attribute word before the
     * second is loaded from the pool and the terrain call's layer 0 is set (two
     * reordered rows, differ 4 without it). */
    do {
    } while (0);
    *words++ = 0x40000400;
    *words = 0xd400;
    x = leader[0];
    z = leader[2];
    particle->posX = x;
    particle->posZ = z;
    particle->posY = Map_GetTerrainHeightFar(0, x, z);
    particle->timer = (i & 15) + 1;
    particle++;
  }
  Io_Write16(0x3f00, &REG_BLDCNT);
  Io_Write16(0x1008, &REG_BLDALPHA);
  Io_Write16(0, &REG_BLDY);
  Task_Add(Func_080d64f0, TASK_PASS_VBLANK);
}
