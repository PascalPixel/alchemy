// Draft, differ 43 of 145: starts the spark ring between from and to: allocates
// its work in heap slot 220, loads the spark picture (ResourceId_ColouredOrbs,
// entry 0x1ef of the resource directory, which needs that name added there
// before adoption) into VRAM, sets up its 96 4 by 4 sprites at the leader's
// priority with every spark idle (age -1), and schedules its update
// (Func_02001538). The sprites' bits are written through the OAM bitfields, the
// VRAM load goes through the call layer (CSE kept its 0x400 for the sparks'
// address) and the sprite loop is a goto loop (the native builds 15 again each
// pass). Left: the native reads the resource id from the pool after keeping the
// leader in r11, its from and to stores derive every second offset from the one
// before (adds r1, #8), and its priority mask is copied (adds r2, r1, #0)
// before the and.
#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "call.h"

struct Spark {
  u8 unk00[0x18];
  s32 age;
};

struct SparkSprite {
  u8 unk00[4];
  u16 y : 8;
  u16 affine : 2;
  u16 blendMode : 2;
  u16 mosaic : 1;
  u16 fullColor : 1;
  u16 shape : 2;
  u16 attr1;
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
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

struct Actor *Actor_GetById(s32 id);
void *HeapSlot_AllocFar(s32 slot, s32 size);
void *Resource_GetTableEntry(s32 id);
extern char ResourceId_ColouredOrbs;
void Func_020027b8(void *resource, void *destination);
s32 Resource_FindFreeEntry(void);
s32 VramCache_Load(s32 entry, s32 size, void *data);
void Func_020029e0(struct SparkSprite *sprite, s32 width, s32 height,
                   s32 attributes, s32 tile);
s32 Func_020029f0(struct Actor *object);
s32 Func_020029f8(struct Actor *object);
void Task_Add(void (*func)(void), s32 priority);
void Func_02001538(void);

void Func_02001718(const s32 *from, const s32 *to) {
  struct SparkWork *work =
      HeapSlot_AllocFar(220, sizeof(struct SparkWork));
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
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
  tile = Value3((s32 (*)())VramCache_Load, entry, 0x400, (s32)work);
  work->tile = tile;
  work->vramEntry = entry;
  i = 0;
  spark = work->sparks;
  sprite = work->sprites;
next:
  Func_020029e0(sprite, 4, 4, 0, tile + (i & 15) * 2);
  sprite->fullColor = 1;
  sprite->palette = 0;
  sprite->priority = Func_020029f0(leader);
  sprite->depth = Func_020029f8(leader) - 1;
  spark->age = -1;
  i++;
  sprite++;
  spark++;
  if (i <= 95)
    goto next;
  work->next = 0;
  Task_Add(Func_02001538, 0x480);
}
