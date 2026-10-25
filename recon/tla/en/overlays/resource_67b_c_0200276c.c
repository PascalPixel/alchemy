// Draft, differ 65 of 81: overlay 67b's copy of overlay 679's Func_02001ad8
// (the same listing), whose draft holds its notes.
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
void Func_020025d4(void);
void Func_020028b0(const void *source, void *destination);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
s32 Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 order);

void Func_0200276c(void) {
  u32 size = sizeof(struct DustWork);
  struct DustWork *work = Runtime_AllocateBlock(116, size);
  struct DustParticle *p = work->particles;
  u8 *buf;
  u32 i;
  volatile u32 zero;

  size = 0;
  zero = size;
  Dma_Set((const void *)&zero, work, 0x85000205, REG_DMA3);
  buf = Runtime_AllocateBlock(56, 0x400);
  Func_020028b0(gField_Tiles679, buf);
  work->vramEntry = Resource_FindFreeEntry();
  work->tileBase = VramCache_Load(work->vramEntry, 0x300, buf);
  Runtime_ReleaseHeapBlock(56);
  work->field808 = size;
  for (i = 0; i < 64; i++, p++) {
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
  }
  Scheduler_AddOrUpdateCallback(Func_020025d4, 0x480);
}
