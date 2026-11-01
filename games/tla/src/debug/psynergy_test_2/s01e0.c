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
void Func_020005f8(struct SparkSprite *sprite, struct Spark *spark);
void Func_02000600(struct Spark *spark, s32 a, s32 b);

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
