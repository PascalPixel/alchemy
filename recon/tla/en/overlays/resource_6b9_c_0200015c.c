// Draft, differ 80 of 83, skeleton gap 28 (tla-en, 2026-11-01): the worst of
// Func_0200029c (83 instructions, differ 80, gap 13/15), Func_0200015c (57, 44,
// 5/5) and Func_020001e0 (88, 4, 2/2). Func_02000358 is adopted in
// games/tla/src/debug/psynergy_test_2/sparks.c.
// The native builds its constants inside the loops, so the first two loops are
// label and goto loops: a for or do loop hoists them (015c 14/16, 01e0 8/11).
// Func_020001e0 has only its incoming-argument saves scheduled differently.
// Func_0200015c has its pool loads scheduled apart; a local for the tile
// expression gives gap 4/4, but that is a forced temporary (S2) and is left out.
// Func_0200029c: the count takes r4 and is saved round the call, because its 7
// weighted refs against one call pass CALLER_SAVE_PROFITABLE; the native has it
// in r8 and the tile in r10, and rebuilds 0x500 where the draft keeps it in the
// sparks pointer's register. Those lead the gap. The goto form stops the hoist
// but measured 17/13 with a zero carried past the loop, so this one stays for.
#include "types.h"
#include "ram_buffer.h"

extern char ResourceId_FallingRock;

struct Spark {
  s32 position[3];
  s32 velocity[3];
  s32 timer;
};

struct SparkSprite {
  u8 unk00[5];
  u8 unk05_0 : 5;
  u8 unk05_5 : 1;
  u8 unk05_6 : 2;
  u8 unk06[2];
  u16 tile : 10;
  u16 unk08_10 : 2;
  u16 unk08_12 : 4;
  u8 unk0a[0x14];
  u16 unk1e;
  u8 unk20[8];
};

struct SparkWork {
  u8 unk0000[0x500];
  struct Spark sparks[64];
  struct SparkSprite sprites[64];
  s16 next;
  u16 tileBase;
  s16 resource;
};

s32 Random16Far(void);
void Vector_AddPolarOffsetFar(s32 radius, s32 angle, s32 *vector);
void *HeapSlot_AllocFar(s32 slot, s32 size);
void HeapSlot_Release(s32 slot);
void *Resource_GetTableEntry(s32 id);
s32 Resource_FindFreeEntry(void);
void Resource_ResetEntry(s32 entry);
s32 VramCache_Load(s32 entry, s32 size, void *data);
void Task_Add(void (*func)(void), s32 priority);
void Task_RemoveFar(void (*func)(void));
void Func_02000560(void *resource, void *destination);
void Func_020005f0(struct SparkSprite *sprite, s32 a, s32 b, s32 c, s32 tile);
void Func_020005f8(struct SparkSprite *sprite, struct Spark *spark);
void Func_02000600(struct Spark *spark, s32 a, s32 b);

void Func_0200015c(void) {
  struct SparkWork *work = Ram_HeapSlots->menuRuntime;
  struct Spark *spark = work->sparks;
  struct SparkSprite *sprite = work->sprites;
  s32 i;

  i = 63;
loop:
  {
    if ((u32)spark->timer < 20) {
      sprite->tile = work->tileBase + ((spark->timer >> 2) & 7) * 8;
      Func_020005f8(sprite, spark);
      Func_02000600(spark, 63, 0xffff8000);
      spark->timer++;
    }
    sprite++;
    spark++;
  }
  i--;
  if (i >= 0)
    goto loop;
}

void Func_020001e0(s32 x, s32 y, s32 z, s32 count) {
  struct SparkWork *work = Ram_HeapSlots->menuRuntime;
  struct Spark *spark;
  s32 angle;
  s32 i;

  if (count > 0) {
    i = count;
  loop:
    angle = Random16Far();
    spark = &work->sparks[work->next];
    spark->timer = 0;
    spark->position[0] = x;
    spark->position[1] = y;
    spark->position[2] = z;
    Vector_AddPolarOffsetFar((Random16Far() << 3) + 0x80000, angle,
                             spark->position);
    spark->velocity[0] = 0;
    spark->velocity[1] = 0x50000;
    spark->velocity[2] = 0;
    Vector_AddPolarOffsetFar((Random16Far() << 1) + 0x20000, angle,
                             spark->velocity);
    work->next++;
    work->next &= 63;
    i--;
    if (i != 0)
      goto loop;
  }
}

void Func_0200029c(void) {
  struct SparkWork *work =
      HeapSlot_AllocFar(220, sizeof(struct SparkWork));
  struct Spark *spark;
  struct SparkSprite *sprite;
  s32 resource;
  s32 tile;
  s32 i;

  Func_02000560(Resource_GetTableEntry((s32)&ResourceId_FallingRock), work);
  resource = Resource_FindFreeEntry();
  tile = VramCache_Load(resource, 0x500, work);
  work->tileBase = tile;
  work->resource = resource;
  spark = work->sparks;
  sprite = work->sprites;
  for (i = 63; i >= 0; i--) {
    Func_020005f0(sprite, 8, 8, 0x40000000, tile);
    sprite->unk05_5 = 1;
    sprite->unk08_12 = 0;
    sprite->unk08_10 = 0;
    sprite->unk1e = 240;
    spark->timer = -1;
    sprite++;
    spark++;
  }
  work->next = 0;
  Task_Add(Func_0200015c, 0x480);
}

void Func_02000358(void) {
  struct SparkWork *work = Ram_HeapSlots->menuRuntime;

  Task_RemoveFar(Func_0200015c);
  Resource_ResetEntry(work->resource);
  HeapSlot_Release(220);
}
