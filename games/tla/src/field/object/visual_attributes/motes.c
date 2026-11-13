/// @file
/// The floating motes and sparkles on the field, and the blends they fade by.
// Part of visual_attributes.c.

#include "../visual_attributes_internal.h"
#include "module/field/event_commands.h"
#include "module/field/field.h"
#include "module/field/object.h"

#define Resource_DecodeByteLzInRam                                             \
  ((void (*)(const void *, void *))Resource_DecodeByteLzInRam)

/// Heap slot 0x74 while the motes run.
/* VIEW(motes): tag struct MoteWork as struct MoteWork__motes;
 * visual_attributes.c has another definition; tried with the definition the
 * rest of the object has, it does not compile in tla-ja:
 * games/tla/src/field/object/visual_attributes.c:3089: structure has no member
 * named `motes'. */
struct MoteWork__motes {
  s32 vramEntry;
  s32 tileBase;
  struct Mote motes[64];
  u8 pad808[12];
};

/// Starts the 64 floating motes (FieldMotes_Update): loads their tiles and puts
/// each under the leader with staggered timers, drawn semi-transparent.
// As ☀️'s function of the same name.
void FieldMotes_Start(void) {
  struct MoteWork__motes *work;
  struct Mote *mote;
  u8 *tiles;
  u32 i;
  u32 size;
  volatile u32 zero;
  volatile u16 *reg;
  struct HeapSlots *slots;
  /* FAKEMATCH(pin): clear held in r6, which keeps its zero set inside the
   * loop on every pass and ready for the last blend register; as a plain
   * local, 4 of 92 instructions differ in tla-en. */
  register s32 clear asm("r6");

  size = sizeof(struct MoteWork__motes);
  work = HeapSlot_AllocEwram(116, size);
  mote = work->motes;
  // size is reused for the fill's zero; a zero of its own differs.
  size = 0;
  zero = size;
  Dma_Set((void *)&zero, work,
          (DMA_ENABLE | DMA_32BIT | DMA_SRC_FIXED) << 16 | 0x205, REG_DMA3);
  tiles = HeapSlot_AllocEwram(56, 0x400);
  Resource_DecodeByteLzInRam(gData_080f385e, tiles);
  work->vramEntry = Resource_FindFreeEntry();
  work->tileBase = VramCache_Load(work->vramEntry, 0x300, tiles);
  HeapSlot_Release(56);
  i = 0;
  slots = Ram_HeapSlots;
  for (; i < 64; i++) {
    struct MapWork *view = slots->mapWork;
    s32 *pos;
    /* FAKEMATCH(temporary): one integer cursor for the attribute stores and
     * the height's x keeps the stores in r1; as an s32 * cursor and an x of
     * its own, 18 of 92 instructions differ in tla-en. */
    u32 p;
    s32 z;

    p = (u32)mote;
    clear = 0;
    *(s32 *)p = clear;
    p += sizeof(s32);
    pos = view->origin;
    *(s32 *)p = 0x40000400;
    p += sizeof(s32);
    *(s32 *)p = 0xd400;
    p = pos[0];
    z = pos[2];
    mote->posX = (s32)p;
    mote->posZ = z;
    mote->posY = MapGrid_GetTerrainHeightFar(0, (s32)p, z);
    mote->timer = (i & 15) + 1;
    mote++;
  }
  // The blend writes in a once-only block keep the task routine's address
  // ahead of the call's other argument; as a plain block, 4 of 92 instructions
  // differ in tla-en.
  do {
    /* FAKEMATCH(temporary): the values through v and the registers through
     * one stepped pointer, which builds each value from a small constant and
     * steps the address by 2; as REG_BLDCNT, REG_BLDALPHA and REG_BLDY
     * written directly, 31 of 92 instructions differ in tla-en. */
    s32 v;
    v = 0x3f00;
    reg = &REG_BLDCNT;
    *reg = v;
    v = 0x1008;
    reg++;
    *reg = v;
    reg++;
    *reg = clear;
  } while (0);
  Task_Add(FieldMotes_Update, TASK_PASS_VBLANK);
}

#undef Resource_DecodeByteLzInRam

/// Starts Weather_UpdateRain's particles: allocates and clears heap slot 0x74,
/// decodes the particle tiles into VRAM, sets each of the 64 particles' sprite
/// words and places it on the ground at the map's focus with a timer of
/// (i & 11) + 1, turns on alpha blending of all layers at 16/16, schedules the
/// update and, when asked, plays cue 137 (Scene_QueueMusic).
void Weather_StartRain(s32 withCue) {
  struct SparkParticleWork *work = HeapSlot_AllocEwram(116, 0x814);
  struct SparkParticle *particle = work->particles;
  volatile u32 fill;
  /* FAKEMATCH(pin): the fill's zero held in r1, built after the copy of
   * work; as a plain local, 8 of 100 instructions differ in tla-en. */
  register u32 zero asm("r1");
  u8 *tiles;
  u32 i;
  struct HeapSlots *slots;
  /* FAKEMATCH(pin): clear held in r6, which keeps its zero set inside the
   * loop on every pass and ready for the last blend register; as a plain
   * local, 4 of 100 instructions differ in tla-en. */
  register s32 clear asm("r6");

  zero = 0;
  Dma_Fill32Cell(fill, zero, work, 0x814);
  tiles = HeapSlot_AllocEwram(56, 0x400);
  Resource_DecodeByteLzInRam(Data_080f3901, tiles);
  work->vramEntry = Resource_FindFreeEntry();
  work->tile = VramCache_Load(work->vramEntry, 0x300, tiles);
  HeapSlot_Release(56);
  i = 0;
  slots = Ram_HeapSlots;
  for (; i < 64; i++, particle++) {
    struct MapWork *view = slots->mapWork;
    s32 *focus;
    /* FAKEMATCH(temporary): one integer cursor for the attribute stores and
     * the height's x keeps the stores in r1; as an s32 * cursor and an x of
     * its own, 18 of 100 instructions differ in tla-en. */
    u32 p;
    s32 z;

    p = (u32)particle;
    clear = 0;
    *(s32 *)p = clear;
    p += sizeof(s32);
    focus = view->origin;
    *(s32 *)p = 0x40000400;
    p += sizeof(s32);
    *(s32 *)p = 0xd400;
    p = focus[0];
    z = focus[2];
    particle->posX = (s32)p;
    particle->posZ = z;
    particle->posY = MapGrid_GetTerrainHeightFar(0, (s32)p, z);
    particle->timer = (i & 11) + 1;
  }
  Io_Write16(BLDCNT_TGT2_ALL, &REG_BLDCNT);
  Io_Write16(BLEND_ALPHA(16, 16), &REG_BLDALPHA);
  Io_Write16(clear, &REG_BLDY);
  Task_Add(Weather_UpdateRain, TASK_PASS_VBLANK);
  if (withCue != 0)
    Scene_QueueMusic(137);
}

void Func_080d7024(void) { Task_Remove(Weather_UpdateRain); }

struct SparkleWork {
  u8 padding00[4];
  s32 tileBase;
  struct Sparkle sparkles[64];
  u8 padding808[8];
  s32 stopped;
};

/* VIEW(motes): routine Func_080140d8 as void Func_080140d8(void *entry, s32
 * priority); f0d6674 has void Func_080140d8(struct GroundParticle *sprite, s32
 * depth). */
extern void Func_080140d8__motes(void *entry,
                                 s32 priority) __asm__("Func_080140d8");

void FieldEffect_UpdateSparkles(void) {
  struct SparkleWork *work;
  u32 spawned;
  u32 i;
  struct MapWork *map;
  s32 *camera;
  s32 spawnX;
  s32 spawnZ;
  struct Sparkle *sparkle;
  const u16 *frame;
  s32 *leader;
  s32 cameraX;
  s32 cameraZ;
  s32 sx;
  s32 sy;
  s32 delay;
  s32 x;
  u32 a;
  u32 b;
  u32 frames;

  work = Ram_HeapSlots->unk70[1];
  spawned = 0;
  map = Ram_HeapSlots->mapWork;
  camera = &map->viewX;
  spawnX = 0;
  spawnZ = 0;
  i = 0;
  sparkle = work->sparkles;
  delay = 0;
  sx = 0;
  sy = 0;
  do {
    if (sparkle->timer != 0) {
      cameraX = camera[0];
      cameraZ = camera[1];
      if (GameFlag_Test(0x166) != 0)
        sparkle->timer++;
      frame = (const u16 *)&gData_080f0b7c[sparkle->timer >> 1];
      a = Random16();
      b = Random16();
      x = ((sparkle->posX - cameraX) >> 16) + (((a & 1) + (b & 1)) >> 1);
      sx = x - 1;
      sy = (sparkle->posZ - sparkle->height - cameraZ) / 0x10000 +
           *(const s16 *)frame;
      frame++;
      if ((u32)(x + 15) <= 255 && sy >= -32 && sy <= 159) {
        sparkle->tile = work->tileBase + *frame;
        sparkle->x = sx;
        sparkle->shape = 0;
        sparkle->size = 1;
#ifdef UBFIX
        frames = gFrameCount;
#else
        // UB: this reads a u32 as unsigned long. The unsigned long alias
        // combines the size and flip stores; as gFrameCount or a u32 alias,
        // 29 of 232 instructions differ in tla-en. UBFIX reads the declared
        // type.
        frames = *(unsigned long *)&gFrameCount;
#endif
        sparkle->y = sy;
        sparkle->matrix = ((frames >> 1) & 1) << 3;
        Func_080140d8__motes(sparkle, 240);
      }
      sparkle->timer--;
    }
    if (spawned <= 3 && sparkle->timer == 0 && work->stopped == 0) {
      if (delay != 0) {
        sparkle->posX = spawnX;
        sparkle->posZ = spawnZ;
        sparkle->height = MapGrid_GetTerrainHeightFar(0, sx, sy);
        sparkle->timer = 62 - delay;
        sparkle->delay = 0;
        spawned++;
        delay += 4;
      } else if ((Random16() & 255) == 0) {
        leader = map->origin;
        spawnX = leader[0] + (Random16() << 8) - 0x800000;
        spawnZ = leader[2] + (Random16() << 8) - 0x800000;
        sparkle->posX = spawnX;
        sparkle->posZ = spawnZ;
        sparkle->height = MapGrid_GetTerrainHeightFar(0, sx, sy);
        sparkle->timer = 30;
        sparkle->delay = delay;
        spawned++;
        delay = 4;
      }
    }
    i++;
    sparkle++;
  } while (i <= 63);
}

#define Resource_DecodeByteLzInRam                                             \
  ((void (*)(const void *, void *))Resource_DecodeByteLzInRam)

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

void FieldEffect_InitializeSparkles(void) {
  u32 size = sizeof(struct DustWork);
  struct DustWork *work = HeapSlot_AllocEwram(116, size);
  struct DustParticle *p = work->particles;
  u8 *buf;
  u32 i;
  s32 clear;
  volatile u32 zero;
  struct HeapSlots *slots;

  // size is reused for the fill's zero; a zero of its own differs.
  size = 0;
  zero = size;
  Dma_Set(&zero, work,
          (DMA_ENABLE | DMA_32BIT | DMA_SRC_FIXED) << 16 |
              (sizeof(struct DustWork) / 4),
          REG_DMA3);
  buf = HeapSlot_AllocEwram(56, 0x400);
  Resource_DecodeByteLzInRam(gData_080f38b2, buf);
  work->vramEntry = Resource_FindFreeEntry();
  work->tileBase = VramCache_Load(work->vramEntry, 0x200, buf);
  HeapSlot_Release(56);

  i = 0;
  slots = Ram_HeapSlots;
  clear = 0;
loop:
  {
    struct MapWork *view = slots->mapWork;
    s32 *leader;
    /* FAKEMATCH(temporary): one integer cursor for the stores and the x keeps
     * the leader in r2; as a u32 * cursor and an x of its own, 16 of 79
     * instructions differ in tla-en. */
    u32 attr;
    s32 z;

    attr = (u32)p;
    *(u32 *)attr = clear;
    attr += sizeof(union DustWord);
    leader = view->origin;
    *(u32 *)attr = 0x40000400;
    attr += sizeof(union DustWord);
    *(u32 *)attr = 0xd400;
    attr = leader[0];
    z = leader[2];
    p->posX = clear;
    p->posZ = clear;
    p->posY = MapGrid_GetTerrainHeightFar(0, attr, z);
    p->timer = (i & 15) + 1;
  }
  i++;
  p++;
  if (i < 64)
    goto loop;
  Task_Add(FieldEffect_UpdateSparkles, TASK_PASS_VBLANK);
}

void BattleEffect_StartTwelveFrameBlend(void) {
  struct FieldBlendWork *work;
  struct BattleEffectBuffers *buffers;
  volatile u32 zero;

  work = HeapSlot_AllocEwram(120, sizeof(struct FieldBlendWork));
  buffers = Ram_HeapSlots->block32;
  zero = 0;
  Dma_Set((const void *)&zero, work,
          (DMA_ENABLE | DMA_32BIT | DMA_SRC_FIXED) << 16 | (sizeof(*work) / 4),
          REG_DMA3);
  BattleEffect_BuildBuffer(0x10003, (u16 *)buffers, (u16 *)work, 1);
  BattleEffect_BuildBuffer(0x10005, (u16 *)buffers, (u16 *)work->to, 1);
  BattleEffect_InterpolateBuffers(work->to, work->from, work->delta, 12);
  BattleEffect_BuildBuffer((s32)work, 0, buffers->target, 1);
  work->timer = 600;
  work->enabled = 1;
  Task_Add(Weather_UpdateLightning, TASK_PASS_VBLANK);
}

void BattleEffect_SetBlock30ValuesMaxZero(void) {
  struct FieldBlendWork *state =
      HeapSlot_AllocEwram(120, sizeof(struct FieldBlendWork));
  state->timer = 0x7fff;
  state->enabled = 0;
}

void BattleEffect_SetBlock30Values12Zero(void) {
  struct FieldBlendWork *state =
      HeapSlot_AllocEwram(120, sizeof(struct FieldBlendWork));
  state->timer = 12;
  state->enabled = 0;
}

void BattleEffect_SetBlock30Values128One(void) {
  struct FieldBlendWork *state =
      HeapSlot_AllocEwram(120, sizeof(struct FieldBlendWork));
  state->timer = 128;
  state->enabled = 1;
}

void BattleEffect_StartBufferBlend(s32 from, s32 to) {
  struct FieldBlendWork *work;
  struct BattleEffectBuffers *buffers;
  volatile u32 zero;
  s16 *target;

  work = HeapSlot_AllocEwram(120, sizeof(struct FieldBlendWork));
  buffers = Ram_HeapSlots->block32;
  zero = 0;
  Dma_Set((const void *)&zero, work,
          (DMA_ENABLE | DMA_32BIT | DMA_SRC_FIXED) << 16 | (sizeof(*work) / 4),
          REG_DMA3);
  BattleEffect_BuildBuffer(from, (u16 *)buffers, (u16 *)work, 1);
  target = work->to;
  BattleEffect_BuildBuffer(to, (u16 *)buffers, (u16 *)target, 1);
  BattleEffect_InterpolateBuffers(target, work->from, work->delta, 12);
  BattleEffect_BuildBuffer((s32)work, 0, buffers->target, 1);
  work->timer = 120;
  work->enabled = 0;
  Task_Add(Weather_UpdateLightning, TASK_PASS_VBLANK);
}
