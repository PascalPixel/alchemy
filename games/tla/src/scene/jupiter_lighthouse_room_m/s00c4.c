#include "jupiter_lighthouse_room_m.h"
#include "io_reg.h"
#include "dma.h"
#include "io_write.h"

extern u8 gCpuLoadDisplayEnabled;
extern char ResourceId_LightningTiles;
extern char ResourceId_LightningMap;
extern s16 gBgScroll[];

void *Heap_Alloc(s32 size);
void Heap_Release(void *block);
void *Resource_GetTableEntry(s32 id);
void Func_0200468c(void *resource, void *destination);
void Func_02004924(s32 a, s32 b);
void Func_0200491c(s32 a, s32 b);
void Func_0200492c(s32 frames);
void Func_020049c4(s32 sound);
void Func_0200475c(s32 a, s32 b, s32 c);
void Actor_Jump(s32 id, s32 mode, s32 value);
void Actor_OffsetPositionAndReset(s32 id, s32 dx, s32 dz);
void Func_0200474c(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
s32 Func_020049ac(s32 x, s32 z, s32 a, s32 b);
void Func_020046b4(void);
void Func_020046a4(void);
void Func_02004784(s32 hue, s32 brightness);
void Func_0200354c(void);

static __inline__ void Func_0200474c_(s32 a, s32 b, s32 c, s32 d, s32 e,
                                      s32 f) {
  /* FAKEMATCH(wrapper): forwarded inline; as direct calls, 8 of 281
   * instructions differ in tla-en. */
  Func_0200474c(a, b, c, d, e, f);
}

static __inline__ void Func_0200475c_(s32 a, s32 b, s32 c) {
  /* FAKEMATCH(wrapper): forwarded inline; as direct calls, 21 of 281
   * instructions differ in tla-en. */
  Func_0200475c(a, b, c);
}

static __inline__ void Func_0200491c_(s32 a, s32 b) {
  /* FAKEMATCH(wrapper): forwarded inline; as direct calls, 21 of 281
   * instructions differ in tla-en. */
  Func_0200491c(a, b);
}

static __inline__ void Func_02004924_(s32 a, s32 b) {
  /* FAKEMATCH(wrapper): forwarded inline; as direct calls, 21 of 281
   * instructions differ in tla-en. */
  Func_02004924(a, b);
}

void Func_020000c4(void) {

  u16 *buffer;
  u16 *p;
  u32 i;

  {
    u8 *enabled = &gCpuLoadDisplayEnabled;

    *enabled = 0;
  }
  REG_DISPCNT &= ~0x100;
  Io_Write16(0x400, &REG_BG0CNT);
  buffer = Heap_Alloc(0x800);
  Func_0200468c(Resource_GetTableEntry((s32)&ResourceId_LightningTiles),
                buffer);
  Dma_Set(buffer, (void *)(0x06000000), 0x84000200, REG_DMA3);
  Heap_Release(buffer);
  Dma_Fill16(0, (void *)0x06002000, 0x800);
  buffer = Heap_Alloc(128);
  Func_0200468c(Resource_GetTableEntry((s32)&ResourceId_LightningMap), buffer);
  p = buffer;
  for (i = 0; i != 64; i++)
    *p++ &= 0xfcff;
  p = buffer;
  for (i = 0; i != 16; i++) {
    Dma_Set(p, (void *)(0x06002000 + (i << 6)), 0x84000002, REG_DMA3);
    p += 4;
  }
  Heap_Release(buffer);
  REG_BLDALPHA = 0x1010;
  REG_BLDCNT = 0x3f41;
  gBgScroll[0] = -104;
  gBgScroll[1] = 16;
  wait(30);
  Func_02004924_(0x10003, 0);
  Func_0200491c_(0x7fff, 0);
  Func_0200492c(1);
  wait(1);
  Func_02004924_(0x7fff, 0);
  Func_0200491c_(0x10003, 0);
  Func_0200492c(8);
  REG_DISPCNT |= 0x100;
  Func_020049c4(144);
  Func_0200475c_(0x50000, 0x70000, 0x10000);
  Func_0200475c_(-1, -1, 0xe666);
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x20000, 0x10000);
  Actor_Jump(4, 6, 0);
  Actor_GetById(4)->actionFlags &= ~1;
  Actor_OffsetPositionAndReset(4, -16, 0);
  Actor_GetById(20)->update = Func_0200354c;
  for (i = 0; i != 32; i++) {
    REG_BLDALPHA = (16 - (i >> 1)) | 0x1000;
    wait(1);
  }
  Func_0200474c_(104, 42, 5, 11, 61, 42);
  Func_0200474c_(106, 52, 1, 1, 63, 84);
  Func_020049ac(130 << 19, 160 << 16, 2, 236);
  Func_02004924_(0x10003, 0);
  Func_0200491c_(0x10000, 0);
  Func_0200492c(60);
  Func_020046b4();
  Func_020046a4();
  Func_02004784(gMainRoutineWork.paletteGlow[0],
                gMainRoutineWork.paletteGlow[1]);
  REG_BLDALPHA = 0x1008;
  REG_BLDCNT = 0x3f10;
  REG_DISPCNT |= 0x100;
  gBgScroll[0] = 0;
  gBgScroll[1] = 0;
  wait(1);
  gCpuLoadDisplayEnabled = 1;
  Func_0200475c_(0, 0, 0);
}
