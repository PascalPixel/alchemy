#include "types.h"
#include "mem.h"
#include "dma.h"
#include "io_reg.h"
#include "resource.h"

struct DustParticle {
  u32 link;
  u32 attr01;
  u32 attr2;
  s32 posX;
  s32 posY;
  s32 posZ;
  u8 pad18[4];
  u16 timer;
  u8 pad1e[2];
};

struct DustWork {
  s32 vramEntry;
  s32 tileBase;
  struct DustParticle particles[64];
  u32 field808;
  u8 pad80c[8];
};

extern const u8 gField_Tiles679[];
void Func_02001940(void);
void Func_02001c24(const void *source, void *destination);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
s32 Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 order);

/// Starts the sparkles: 64 sprites on the ground at the origin, their tiles
/// decoded into VRAM, and Func_02001940 scheduled at priority 0x480 to move
/// them; the overlay's copy of FieldEffect_InitializeSparkles.
void Func_02001ad8(void) {
  struct DustWork *work = Runtime_AllocateBlock(116, sizeof(struct DustWork));
  struct DustParticle *p = work->particles;
  u8 *buf;
  u32 i;
  u32 size;
  volatile u32 zero;

  /* FAKEMATCH(temporary): one local is the fill word, the end word and each
   * particle's attribute cursor, so it lives on the stack and every step of
   * the cursor is stored there, as the native's; with a separate cursor, 55 of
   * 81 instructions differ in tla-en. */
  size = 0;
  zero = size;
  Dma_Set((const void *)&zero, work, 0x85000205, REG_DMA3);
  buf = Runtime_AllocateBlock(56, 0x400);
  Func_02001c24(gField_Tiles679, buf);
  work->vramEntry = Resource_FindFreeEntry();
  work->tileBase = VramCache_Load(work->vramEntry, 0x300, buf);
  Runtime_ReleaseHeapBlock(56);
  work->field808 = size;
  i = 0;
loop:
  size = (u32)p;
  *(u32 *)size = 0;
  size += 4;
  *(u32 *)size = 0x40000400;
  size += 4;
  *(u32 *)size = 0xb400;
  p->posX = 0;
  p->posZ = 0;
  p->posY = Map_GetTerrainHeight(0, 0, 0) << 16;
  p->timer = (i & 15) + 1;
  i++;
  p++;
  if (i < 64)
    goto loop;
  Scheduler_AddOrUpdateCallback(Func_02001940, 0x480);
}
