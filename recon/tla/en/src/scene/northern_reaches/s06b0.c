#include "types.h"
#include "system/dma.h"
#include "call.h"

/// The sprite attributes, as The Broken Seal's render_input.h lays them out.
struct OamData {
  u16 y : 8;
  u16 affineMode : 2;
  u16 objectMode : 2;
  u16 mosaic : 1;
  u16 fullColor : 1;
  u16 shape : 2;
  u16 x : 9;
  u16 matrix : 5;
  u16 size : 2;
  u16 index : 10;
  u16 priority : 2;
  u16 palette : 4;
  u16 high;
};

/// The sprite words as one stored pair, or the attributes field by field.
union SparkOam {
  struct {
    u32 first;
    u32 second;
  } words;
  struct OamData oam;
};

struct Sprite6a7 {
  u32 unk_00;
  struct OamData oam;
  u8 unk_0c[18];
  u16 unk_1e;
  u8 unk_20[8];
};

struct Particle6a7 {
  s32 x;
  s32 y;
  s32 z;
  s32 velocityX;
  s32 velocityY;
  s32 unk_14;
  u32 age;
};

struct Spark6a7 {
  u32 unk_00;
  union SparkOam oam;
  s32 timer;
};

struct Work6a7 {
  u8 sheet[0x1000];
  struct Sprite6a7 sprites[54];
  struct Particle6a7 particles[54];
};

s32 *Func_020009f0(void *resource, void *to);
void Func_02000a10(struct Spark6a7 *spark, s32 value);
void Func_02000a50(s32 a, s32 b, s32 c);
void Func_02000b10(s32 color, s32 a);
void Func_02000b18(s32 frames);
void Func_02000b40(struct Sprite6a7 *sprite, s32 a, s32 b, s32 c, s32 tile);
void Func_02000b48(struct Sprite6a7 *sprite, struct Particle6a7 *particle);
void Func_02000b50(struct Particle6a7 *particle, s32 a, s32 b);
void Func_02000b58(s32 cue);
void FieldEvent_Wait(s32 frames);
void *HeapSlot_AllocFar(s32 slot, s32 size);
void HeapSlot_Release(s32 slot);
void *Heap_Alloc(s32 size);
void *Heap_AllocEwram(s32 size);
void Heap_Release(void *buffer);
u32 Random16Far(void);
void *Resource_GetTableEntry(s32 resourceId);
s32 Resource_FindFreeEntry(void);
s32 Resource_ResetEntry(u32 slot);
s32 VramCache_Load(u32 slot, u32 size, const void *source);
void wait(s32 frames);
extern const u16 Field_Palette685[];
extern char ResourceId_FieldEffectIceShard;
extern char ResourceId_FieldEffectExplosion;

void Func_020006b0(s32 unused, s32 *position, void (*callback)(void)) {
  struct Work6a7 *work;
  struct Particle6a7 *particle;
  struct Sprite6a7 *sprite;
  struct Sprite6a7 *flySprite;
  struct Spark6a7 *sparks;
  struct Spark6a7 *spark;
  s32 *timer;
  // Unused: gives the stack frame its 4 bytes.
  u8 frameSlot[4];
  void *sheet;
  s32 i, frame, column, zero, entry, sparkEntry, tiles, flying, sparkTiles;

  work = HeapSlot_AllocFar(220, SPLIT_CONSTANT(0x1e00, 88));
  flying = 0;
  Func_020009f0(Resource_GetTableEntry((s32)&ResourceId_FieldEffectIceShard),
                work);
  entry = Resource_FindFreeEntry();
  tiles = VramCache_Load(entry, 0x500, work);
  i = 0;
  particle = work->particles;
  sprite = work->sprites;
  for (; i <= 53; i++) {
    Func_02000b40(sprite, 0, 0, 0x40000000, (i & 3) * 8 + tiles);
    zero = 0;
    sprite->unk_1e = 250;
    sprite->oam.priority = zero;
    sprite->oam.fullColor = 1;
    sprite->oam.palette = zero;
    column = i / 6;
    particle->x = position[0] + (column << 20);
    particle->y = position[1] - ((i % 6) << 20);
    column -= 4;
    sprite++;
    particle->z = position[2];
    particle->velocityX = (Random16Far() * 3 >> 1) * column;
    particle->velocityY = (Random16Far() << 2) + 0x20000;
    particle->unk_14 = zero;
    particle->age = zero;
    particle++;
  }
  sparks = Heap_AllocEwram(0x600);
  /* FAKEMATCH(temporary): the size held in the local that then takes the
   * buffer; as Heap_Alloc(0x1000), 19 of 323 instructions differ in tbs-en. */
  sheet = (void *)0x1000;
  sheet = Heap_Alloc((s32)sheet);
  Func_020009f0(Resource_GetTableEntry((s32)&ResourceId_FieldEffectExplosion),
                sheet);
  sparkEntry = Resource_FindFreeEntry();
  sparkTiles = VramCache_Load(sparkEntry, 0x400, sheet);
  Heap_Release(sheet);
  timer = &sparks->timer;
  for (i = 95; i >= 0; i--) {
    *timer = -(Random16Far() << 5 >> 16);
    timer += 4;
  }
  Dma_Set(Field_Palette685, (void *)0x50003e0, SPLIT_CONSTANT(0x80000000, 16),
          (volatile u32 *)SPLIT_CONSTANT(0x4000000, 212));
  for (frame = 0; frame <= 149; frame++) {
    if ((frame & 31) == 0) {
      Func_02000b58(145);
    }
    if (frame == 60) {
      Func_02000b10(SPLIT_CONSTANT(0x7f00, 255), 0);
      Func_02000b18(90);
      Func_02000b58(141);
    }
    if (frame == 105) {
      callback();
      flying = 1;
      Call3((void (*)())Func_02000a50, 0x60000, 0x60000, 0x10000);
      Func_02000b58(228);
    }
    if (frame == 130) {
      flying = 1;
    }
    if (flying != 0) {
      particle = work->particles;
      flySprite = work->sprites;
      for (i = 53; i >= 0; i--) {
        if (particle->age < 60) {
          Func_02000b48(flySprite, particle);
          Func_02000b50(particle, 63, -0x3000);
          particle->age++;
        }
        flySprite++;
        particle++;
      }
    }
    spark = sparks;
    do {
      if (spark->timer == 0) {
        spark->oam.words.first = 0x40000000;
        spark->oam.words.second = 0;
        spark->oam.oam.x = (Random16Far() << 7 >> 16) + 52;
        spark->oam.oam.y = (Random16Far() << 6 >> 16) + 16;
        spark->oam.oam.palette = 15;
        spark->oam.oam.priority = 0;
      }
      if (spark->timer >= 0) {
        spark->oam.oam.index = sparkTiles + (spark->timer >> 2) * 4;
        Func_02000a10(spark, 100);
      }
      spark->timer++;
      if (spark->timer > 31) {
        spark->timer = -(Random16Far() * 7 >> 16);
      }
      spark++;
    } while ((s32)spark <= (s32)&sparks[95]);
    wait(1);
  }
  Resource_ResetEntry(sparkEntry);
  Heap_Release(sparks);
  FieldEvent_Wait(40);
  Func_02000a50(0, 0, 0);
  Resource_ResetEntry(entry);
  HeapSlot_Release(220);
}
