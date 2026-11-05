// Draft, differ 26: overlay 6a8's copy of the function drafted in
// resource_6b9_c_0200015c.c, whose header says what it is and what still differs.
#include "types.h"
#include "call.h"
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
  u32 tile : 10;
  u32 unk08_10 : 2;
  u32 unk08_12 : 4;
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
void Func_0200460c(struct SparkSprite *sprite, struct Spark *spark);
void Func_02004614(struct Spark *spark, s32 a, s32 b);

void Func_02004114(void) {
  struct SparkWork *work = Ram_HeapSlots->menuRuntime;
  struct Spark *spark = work->sparks;
  struct SparkSprite *sprite = work->sprites;
  s32 i;

  i = 63;
loop:
  {
    if ((u32)spark->timer < 20) {
      s32 pictureMask = 7;

      sprite->tile = work->tileBase + ((spark->timer >> 2) & pictureMask) * 8;
      Func_0200460c(sprite, spark);
      Call3((void (*)())Func_02004614, spark, 63, 0xffff8000);
      spark->timer++;
    }
    sprite++;
    spark++;
  }
  i--;
  if (i >= 0)
    goto loop;
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
  Task_Add(Func_02004114, 0x480);
}

void Func_02000358(void) {
  struct SparkWork *work = Ram_HeapSlots->menuRuntime;

  Task_RemoveFar(Func_02004114);
  Resource_ResetEntry(work->resource);
  HeapSlot_Release(220);
}
