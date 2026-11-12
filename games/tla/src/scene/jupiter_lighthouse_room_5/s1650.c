#include "types.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "event_inline.h"

struct Spark {
  u8 unk00[0x0c];
  s32 dx;
  s32 dy;
  s32 speed;
  s32 age;
};

struct SparkSprite {
  u8 unk00[4];
  u8 y;
  u8 low : 5;
  u8 fullColor : 1;
  u8 shape : 2;
  u16 attr1;
  u8 tile;
  u8 tileHigh : 2;
  u8 priority : 2;
  u8 palette : 4;
  u8 unk0a[0x14];
  u16 depth;
  u8 unk20[8];
};

struct SparkWork {
  u8 tiles[0x400];
  struct Spark sparks[96];
  struct SparkSprite sprites[96];
  s32 from[3];
  s32 to[3];
  s16 next;
  u16 tile;
  s16 vramEntry;
};

u32 Random16Far(void);
s32 Math_Cosine(s32 angle);

/// Adds count sparks to the spark ring's 96 entries in heap slot 220, each
/// with a random angle from 0x6000 across three quarters of a turn, the
/// cosine and negated sine of it at 64 pixels as its direction, a random
/// speed of 16 to 31 and an age of 0.
void Func_02001650(s32 count) {
  struct SparkWork *work = Ram_HeapSlots->menuRuntime;
  struct Spark *spark;
  s32 angle;
  s32 i;
  s32 radius;
  u64 cosine;
  s32 speed;

  if (count > 0) {
    i = count;
  next:
    spark = &work->sparks[work->next];
    angle = ((Random16Far() * 3) >> 2) + 0x6000;
    radius = 0x400000;
    speed = ((Random16Far() << 4) >> 16) + 16;
    /* FAKEMATCH(temporary): the cosine held in a 64-bit temporary; as
     * Iwram_MulQ16(Math_Cosine(angle), radius), 14 of 92 instructions differ
     * in tla-en. */
    cosine = Math_Cosine(angle);
    spark->dx = Iwram_MulQ16(cosine, radius) >> 16;
    spark->dy = -Iwram_MulQ16(Math_Sin(angle), radius) >> 16;
    spark->speed = speed;
    spark->age = 0;
    work->next++;
    work->next %= 96;
    if (--i != 0)
      goto next;
  }
}

#include "actor.h"
#include "main_routine_work.h"

void *HeapSlot_AllocFar(s32 slot, s32 size);
void *Resource_GetTableEntry(s32 id);
extern char ResourceId_ColouredOrbs;
void Func_020027b8(void *resource, void *destination);
s32 Resource_FindFreeEntry(void);
void Func_020029e0(struct SparkSprite *sprite, s32 width, s32 height,
                   s32 attributes, s32 tile);
s32 Func_020029f0(struct Actor *object);
s32 Func_020029f8(struct Actor *object);
void Func_02001538(void);

void Func_02001718(const s32 *from, const s32 *to) {
  struct SparkWork *work = HeapSlot_AllocFar(220, sizeof(struct SparkWork));
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  struct SparkSprite *sprite;
  struct Spark *spark;
  s32 entry;
  s32 tile;
  s32 i;

  work->from[0] = from[0];
  work->from[1] = from[1];
  work->from[2] = from[2];
  work->to[0] = to[0];
  work->to[1] = to[1];
  work->to[2] = to[2];
  Func_020027b8(Resource_GetTableEntry((s32)&ResourceId_ColouredOrbs), work);
  entry = Resource_FindFreeEntry();
  tile = Vram_Load(entry, 0x400, work);
  work->tile = tile;
  work->vramEntry = entry;
  i = 0;
  spark = work->sparks;
  sprite = work->sprites;
  for (; i <= 95; i++, sprite++, spark++) {
    Func_020029e0(sprite, 4, 4, 0, tile + (i & 15) * 2);
    sprite->fullColor = 1;
    sprite->palette = 0;
    sprite->priority = Func_020029f0(leader) & 3;
    // Through a union, so this store may alias the age store after it.
    ((union {
       u16 v;
       s32 w;
     } *)&sprite->depth)
        ->v = Func_020029f8(leader) - 1;
    spark->age = -1;
  }
  {
    s32 zero = 0;

    work->next = zero;
  }
  /* FAKEMATCH(wrapper): the task through Value2; as a direct call, 8 of 145
   * instructions differ in tla-en. */
  Task_AddFrame(Func_02001538, 0x480);
}
