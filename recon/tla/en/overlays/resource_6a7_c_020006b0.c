// Draft, differ 30 of 323 (tla-en, 2026-11-05). Older notes below may predate this figure.
// Structure matches: all five loops, the bitfields, pools and calls. Left: the
// listing keeps the particle pointer in r7 and sprite and frame in r6, here
// swapped (particle outranks sprite in global.c whether or not it is shared
// with the flight loop, and for-loop weights did not change that); the frame is
// 24 bytes against 28 (sp+24 is unused in the listing); the scheduling of the
// first bitfield block and of the timer load (r3 in the listing, r2 here)
// follows from those. Forms that were measured to matter: only the first loop
// is written with a label, since as a for loop its 0 is hoisted; slot holds
// that 0 because a second set stops it being rematerialized, which puts it in
// r11 as the listing has it; the 0x1000 passes through sheet for r5; column
// carries the tile argument; the first timer read goes through a union because
// a second alias set is what stops PRE reusing the load at the join. Call3
// keeps the two 0x60000 unshared. The source form behind the slot, sheet and
// union shapes is unknown.
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

union SparkView {
  s32 s;
  u32 u;
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
extern char ResourceId_Data1F2;
extern char ResourceId_Data1E8;

void Func_020006b0(s32 unused, s32 *position, void (*callback)(void)) {
  struct Work6a7 *work;
  struct Particle6a7 *particle;
  struct Sprite6a7 *sprite;
  struct Sprite6a7 *flySprite;
  struct Spark6a7 *sparks;
  struct Spark6a7 *spark;
  s32 *timer;
  void *sheet;
  s32 i, frame, column, slot, entry, tiles, flying, sparkTiles;

  work = HeapSlot_AllocFar(220, 0x1e58);
  flying = 0;
  Func_020009f0(Resource_GetTableEntry((s32)&ResourceId_Data1F2), work);
  entry = Resource_FindFreeEntry();
  tiles = VramCache_Load(entry, 0x500, work);
  particle = work->particles;
  sprite = work->sprites;
  /* FAKEMATCH(barrier): the empty asm on sprite changes its allocation rank; as no
   * asm, 94 of 323 instructions differ in tla-en. */
  asm("" : "=r"(sprite) : "0"(sprite));
  /* FAKEMATCH(barrier): the empty asm on sprite changes its allocation rank; as no
   * asm, 109 of 323 instructions differ in tla-en. */
  asm("" : "=r"(sprite) : "0"(sprite));
  i = 0;
  /* FAKEMATCH(barrier): the empty asm on i changes its allocation rank; as no
   * asm, 32 of 323 instructions differ in tla-en. */
  asm("" : "=r"(i) : "0"(i));
sheetLoop:
  column = (i & 3) * 8 + tiles;
  Func_02000b40(sprite, 0, 0, 0x40000000, column);
  slot = 0;
  sprite->unk_1e = 250;
  sprite->oam.priority = slot;
  /* FAKEMATCH(barrier): an empty asm holds the order around it; as no asm,
   * 107 of 323 instructions differ in tla-en. */
  asm("");
  sprite->oam.fullColor = 1;
  sprite->oam.palette = slot;
  column = i / 6;
  particle->x = position[0] + (column << 20);
  particle->y = position[1] - ((i % 6) << 20);
  column -= 4;
  sprite++;
  particle->z = position[2];
  particle->velocityX = (Random16Far() * 3 >> 1) * column;
  particle->velocityY = (Random16Far() << 2) + 0x20000;
  particle->unk_14 = slot;
  particle->age = slot;
  particle++;
  i++;
  if (i <= 53)
    goto sheetLoop;
  sparks = Heap_AllocEwram(0x600);
  sheet = (void *)0x1000;
  sheet = Heap_Alloc((s32)sheet);
  Func_020009f0(Resource_GetTableEntry((s32)&ResourceId_Data1E8), sheet);
  slot = Resource_FindFreeEntry();
  sparkTiles = VramCache_Load(slot, 0x400, sheet);
  Heap_Release(sheet);
  timer = &sparks->timer;
  for (i = 95; i >= 0; i--) {
    *timer = -(Random16Far() << 5 >> 16);
    timer += 4;
  }
  Dma_Set(Field_Palette685, (void *)0x50003e0, 0x80000010,
          (volatile u32 *)0x40000d4);
  for (frame = 0; frame <= 149; frame++) {
    if ((frame & 31) == 0) {
      Func_02000b58(145);
    }
    if (frame == 60) {
      Func_02000b10(0x7fff, 0);
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
      if (((union SparkView *)&spark->timer)->s == 0) {
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
  Resource_ResetEntry(slot);
  Heap_Release(sparks);
  FieldEvent_Wait(40);
  Func_02000a50(0, 0, 0);
  Resource_ResetEntry(entry);
  HeapSlot_Release(220);
}
