#include "types.h"
#include "battle.h"
#include "graphics.h"
#include "mem.h"
#include "ram_buffer.h"
#include "callback_scheduler.h"
#include "palbuf.h"
#include "fxblend.h"
#include "dma.h"
#include "io_reg.h"
#include "map.h"
#include "resource.h"
#include "edition.h"

void Func_080d67e8(void);

void BattleEffect_InterpolateBuffers(s16 *source, s16 *target, s16 *step,
                                     s32 frames);

#define Resource_DecodeByteLzInRam                                             \
  ((void (*)(const void *, void *))Resource_DecodeByteLzInRam)

union DustWord {
  u32 value;
  void *link;
};

struct DustParticle {
  union DustWord link;
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
  u8 pad808[12];
};

extern const u8 gData_080f38b2[];
void Func_080d7034(void);
s32 Map_GetTerrainHeightFar(s32 layer, s32 x, s32 z);

void FieldEffect_InitializeSparkles(void) {
  u32 size = sizeof(struct DustWork);
  struct DustWork *work = Runtime_AllocateBlock(116, size);
  struct DustParticle *p = work->particles;
  u8 *buf;
  u32 i;
  s32 clear;
  volatile u32 zero;
  struct HeapSlots *slots;

  /* FAKEMATCH: needed to match: reusing the allocation size for the fill word
   * keeps zero in r1; a separate helper value differs by four instructions. */
  size = 0;
  zero = size;
  Dma_Set(&zero, work, 0x85000205, REG_DMA3);
  buf = Runtime_AllocateBlock(56, 0x400);
  Resource_DecodeByteLzInRam(gData_080f38b2, buf);
  work->vramEntry = Resource_FindFreeEntry();
  work->tileBase = VramCache_Load(work->vramEntry, 0x200, buf);
  Runtime_ReleaseHeapBlock(56);

  i = 0;
  slots = Ram_HeapSlots;
  clear = 0;
loop:
  {
    struct MapWork *view = slots->mapWork;
    s32 *leader;
    /* FAKEMATCH: needed to match: an integer field cursor keeps the view load
     * order; reusing it for the height x argument keeps the leader in r2.
     * Separate temporaries differ by 14 instructions. */
    u32 attr;
    s32 z;

    /* FAKEMATCH: keeps the map work load before the cursor copy (differ 2
     * without) */
    __asm__("" : : "r"(view));
    attr = (u32)p;
    *(u32 *)attr = clear;
    attr += sizeof(union DustWord);
    /* FAKEMATCH: keeps the first store before the leader load (differ 4
     * without) */
    __asm__("" : : "r"(view));
    leader = view->origin;
    *(u32 *)attr = 0x40000400;
    attr += sizeof(union DustWord);
    *(u32 *)attr = 0xd400;
    attr = leader[0];
    z = leader[2];
    p->posX = clear;
    p->posZ = clear;
    p->posY = Map_GetTerrainHeightFar(0, attr, z);
    p->timer = (i & 15) + 1;
  }
  i++;
  p++;
  if (i < 64)
    goto loop;
  Scheduler_AddOrUpdateCallback(Func_080d7034, 0x480);
}

void BattleEffect_StartTwelveFrameBlend(void) {
  struct FieldBlendWork *work;
  struct BattleEffectBuffers *buffers;
  volatile u32 zero;

  work = Runtime_AllocateBlock(120, sizeof(struct FieldBlendWork));
  buffers = Ram_HeapSlots->block32;
  zero = 0;
  Dma_Set((const void *)&zero, work, 0x850007e2, REG_DMA3);
  BattleEffect_BuildBuffer(0x10003, (u16 *)buffers, (u16 *)work, 1);
  BattleEffect_BuildBuffer(0x10005, (u16 *)buffers, (u16 *)work->to, 1);
  BattleEffect_InterpolateBuffers(work->to, work->from, work->delta, 12);
  BattleEffect_BuildBuffer((s32)work, 0, buffers->target, 1);
  work->timer = 600;
  work->enabled = 1;
  Scheduler_AddOrUpdateCallback(Func_080d67e8, 0x480);
}

void BattleEffect_SetBlock30ValuesMaxZero(void) {
  struct FieldBlendWork *state =
      Runtime_AllocateBlock(120, sizeof(struct FieldBlendWork));
  state->timer = 0x7fff;
  state->enabled = 0;
}

void BattleEffect_SetBlock30Values12Zero(void) {
  struct FieldBlendWork *state =
      Runtime_AllocateBlock(120, sizeof(struct FieldBlendWork));
  state->timer = 12;
  state->enabled = 0;
}

void BattleEffect_SetBlock30Values128One(void) {
  struct FieldBlendWork *state =
      Runtime_AllocateBlock(120, sizeof(struct FieldBlendWork));
  state->timer = 128;
  state->enabled = 1;
}

void BattleEffect_StartBufferBlend(s32 from, s32 to) {
  struct FieldBlendWork *work;
  struct BattleEffectBuffers *buffers;
  volatile u32 zero;
  s16 *target;

  work = Runtime_AllocateBlock(120, sizeof(struct FieldBlendWork));
  buffers = Ram_HeapSlots->block32;
  zero = 0;
  Dma_Set((const void *)&zero, work, 0x850007e2, REG_DMA3);
  BattleEffect_BuildBuffer(from, (u16 *)buffers, (u16 *)work, 1);
  target = work->to;
  BattleEffect_BuildBuffer(to, (u16 *)buffers, (u16 *)target, 1);
  BattleEffect_InterpolateBuffers(target, work->from, work->delta, 12);
  BattleEffect_BuildBuffer((s32)work, 0, buffers->target, 1);
  work->timer = 120;
  work->enabled = 0;
  Scheduler_AddOrUpdateCallback(Func_080d67e8, 0x480);
}
