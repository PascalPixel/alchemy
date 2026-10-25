// Draft, differ 134 of 145: starts the spark ring between from and to: allocates
// its work in heap slot 220, loads the spark picture (resource 0x1ef) into
// VRAM, sets up its 96 4 by 4 sprites at the leader's priority with every
// spark idle (age -1), and schedules its update (Func_02001538).
#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "party_state.h"

struct Spark {
  u8 unk00[0x18];
  s32 age;
};

struct SparkSprite {
  u8 unk00[5];
  u8 attr5;
  u8 unk06[3];
  u8 attr9;
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

struct ObjectRuntime *Object_GetById(s32 id);
void *Runtime_AllocateHeapBlockFar(s32 slot, s32 size);
void *Resource_GetTableEntry(s32 id);
void Func_020027b8(void *resource, void *destination);
s32 Resource_FindFreeEntry(void);
s32 VramCache_Load(s32 entry, s32 size, void *data);
void Func_020029e0(struct SparkSprite *sprite, s32 width, s32 height,
                   s32 attributes, s32 tile);
s32 Func_020029f0(struct ObjectRuntime *object);
s32 Func_020029f8(struct ObjectRuntime *object);
void Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 priority);
void Func_02001538(void);

void Func_02001718(const s32 *from, const s32 *to) {
  struct SparkWork *work =
      Runtime_AllocateHeapBlockFar(220, sizeof(struct SparkWork));
  struct ObjectRuntime *leader = Object_GetById(gPartyState.currentOwner);
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
  Func_020027b8(Resource_GetTableEntry(0x1ef), work);
  entry = Resource_FindFreeEntry();
  tile = VramCache_Load(entry, 0x400, work);
  work->tile = tile;
  work->vramEntry = entry;
  spark = work->sparks;
  sprite = work->sprites;
  for (i = 0; i <= 95; i++) {
    Func_020029e0(sprite, 4, 4, 0, tile + (i & 15) * 2);
    sprite->attr5 |= 32;
    sprite->attr9 &= 15;
    sprite->attr9 = (sprite->attr9 & ~12) | ((Func_020029f0(leader) & 3) << 2);
    sprite->depth = Func_020029f8(leader) - 1;
    spark->age = -1;
    sprite++;
    spark++;
  }
  work->next = 0;
  Scheduler_AddOrUpdateCallback(Func_02001538, 0x480);
}
