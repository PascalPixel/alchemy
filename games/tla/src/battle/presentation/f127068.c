/// @file
/// Sets up the star field behind a summon, the twin of The Broken Seal's
/// ElementGlow_InitializeStarField: sixteen sparks on random rings around the
/// centre, three rings leaving it at thirds of a turn, the blue ring sheet and
/// its palette with the mode's colour (0 yellow, 1 blue, 2 red, else violet)
/// over the first 32 colours, BG2 unscaled at the origin, the additive and max
/// blitters (Func_08138048, the veneer of BattleEffect_LoadBlitter), and the
/// two frame callbacks.

#include "types.h"
#include "dma.h"
#include "io_reg.h"
#include "io_write.h"
#include "iwram_call.h"
#include "heap_release.h"
#include "ram_buffer.h"

struct Spark {
  s32 pos[6]; // x, y, z and their velocities, 16.16
  s32 life;
};

struct Ring {
  s32 x;
  s32 y;
  s32 velX;
  s32 velY;
  s32 age;
};

struct SparkWork {
  u8 sheet[0x11c0];
  struct Spark sparks[16];
  struct Ring rings[3];
  s32 frames;
  s32 ready;
  s32 originX;
  s32 originY;
  s32 originPending;
};

extern char ResourceId_YellowPaletteB;
extern char ResourceId_BlueRingSheet;
extern char ResourceId_RedPaletteC;
extern char ResourceId_VioletPaletteD;

typedef s32 (*TaskFunc)(void);

u8 *Resource_GetTableEntry(s32 id);
s32 Resource_DecodeType01(const void *source, void *destination);
s32 Random16(void);
s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);
s32 Func_08138048(s32 slot, s32 modeAndFlags);
s32 Task_Add(TaskFunc func, s32 order);
s32 Func_08126e00(void);
s32 Func_08127038(void);
// The Broken Seal's five-argument blitter loader, named but never called.
s32 BattleEffect_BuildBlitterFar(s32 slot, s32 source, s32 destination,
                                 s32 flags, s32 mode);

// The rings' vectors go through these two polar helpers, as the native
// rebuilds 0x200 at each call (movs, lsls) and keeps only the radius and the
// multiply's entry in registers; written out, loop.c hoists 0x200 too (81 of
// 255 differ, 2026-11-08).
static __inline__ s32 PolarX(s32 angle, s32 radius) {
  return Iwram_MulQ16(Trig_Cos(angle), radius);
}

static __inline__ s32 PolarY(s32 angle, s32 radius) {
  return Iwram_MulQ16(Trig_Sin(angle), radius);
}

s32 Func_08127068(s32 mode) {
  struct SparkWork *work;
  struct Spark *spark;
  struct Ring *ring;
  s32 i;
  u8 *data;
  s32 zero;

  ((s32 *)Ram_HeapSlots->transitionWork)[2] = 1;
  work = HeapSlot_AllocEwram(92, sizeof *work);
  HeapSlot_Alloc(96, 0x4000);
  for (i = 0; i < 16; i++) {
    s32 angle = Random16();
    u32 radius = Random16() + 0x10000;
    u32 half = radius >> 1;

    spark = &work->sparks[i];
    spark->pos[0] = Iwram_MulQ16(Trig_Cos(angle), half);
    spark->pos[1] = Iwram_MulQ16(Trig_Sin(angle), half);
    if (spark->pos[0] & 1)
      spark->pos[0] = -spark->pos[0];
    if (spark->pos[1] & 1)
      spark->pos[1] = -spark->pos[1];
    spark->pos[2] = (u32)(Random16() + 0x8000) >> 2;
    spark->pos[3] = (-spark->pos[0] >> 7) + (spark->pos[1] >> 8);
    spark->pos[4] = (-spark->pos[1] >> 7) + (-spark->pos[0] >> 8);
    spark->pos[5] = 0;
    spark->life = (radius >> 13) + 1;
  }
  for (i = 0; i < 3; i++) {
    s32 radius = 0x1000;
    s32 angle;

    ring = &work->rings[i];
    angle = i * 0x5555;
    ring->x = PolarX(angle, radius);
    ring->y = PolarY(angle, radius);
    ring->velX = PolarX(angle, 0x200);
    ring->velY = PolarY(angle, 0x200);
    ring->age = 0;
  }
  work->frames = 0;
  work->ready = 0;
  work->originPending = 0;
  Iwram_ClearWords(Ram_HeapSlots->battleFxWork[1], 0x4000);
  data = Resource_GetTableEntry((s32)&ResourceId_BlueRingSheet);
  Iwram_CopyWords((void *)BG_PLTT, data, 128);
  data += 128;
  Resource_DecodeType01(data, work);
  switch (mode) {
  case 0:
    data = Resource_GetTableEntry((s32)&ResourceId_YellowPaletteB);
    break;
  case 1:
    data = Resource_GetTableEntry((s32)&ResourceId_BlueRingSheet);
    break;
  case 2:
    data = Resource_GetTableEntry((s32)&ResourceId_RedPaletteC);
    break;
  default:
    data = Resource_GetTableEntry((s32)&ResourceId_VioletPaletteD);
    break;
  }
  Dma_Set(data, (void *)BG_PLTT, 0x84000000 | 32, REG_DMA3);
  // A once-only block, as in The Broken Seal: it keeps the zero ahead of the
  // BG2X address, which reload derives from the DMA register's (8 without it).
  do {
    zero = 0;
    REG_BG2X = zero;
    REG_BG2Y = zero;
  } while (0);
  Io_Write16(0x100, &REG_BG2PA);
  Io_Write16(0, &REG_BG2PB);
  Io_Write16(0, &REG_BG2PC);
  Io_Write16(0x100, &REG_BG2PD);
  /* FAKEMATCH(dead): a never-taken five-argument call reserves the frame's
   * outgoing argument word below mode; as no call, 10 of 255 instructions
   * differ in tla-en (6 with an unused u8[4] in its place). */
  if (0)
    BattleEffect_BuildBlitterFar(104, 7, 7, 3, 3);
  Func_08138048(104, 0x03);
  Func_08138048(188, 0x13);
  Task_Add(Func_08126e00, 0x480);
  return Task_Add(Func_08127038, 0x480);
}
