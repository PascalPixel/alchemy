// Draft, differ 303 of 316: the Jupiter Lighthouse aerie's effect between objects a
// and b: allocates its work in heap slot 220, loads its two pictures
// (resources 0x1eb and 0x1f6) into VRAM, sets up 32 small sprites and one large
// one, picks its start and end points by side, points the event's
// object (event work word 460) at them (Func_02002a20, Func_02002a28), plays
// sound 142, runs Func_02001080 as a scheduled callback until its step counter
// reaches 999, then frees the VRAM entries and the work.
#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"

struct BeamSprite {
  u8 unk00[5];
  u8 attr5;
  u8 unk06[3];
  u8 attr9;
  u8 unk0a[0x0a];
  s32 unk14;
  s32 unk18;
  u8 unk1c[2];
  u16 depth;
  u8 unk20[8];
};

struct BeamSpark {
  u8 unk00[0x18];
  s32 age;
};

struct BeamWork {
  u8 tiles[0x200];
  struct BeamSprite sprites[32];
  struct BeamSpark sparks[32];
  struct BeamSprite large;
  u8 unkaa8[0x1c];
  s32 from[3];
  s32 to[3];
  s32 start[3];
  u8 unkae8[0x18];
  s16 step;
  s16 unkb02;
  s16 largeEntry;
  u16 tile;
  u8 unkb08;
  u8 unkb09;
  u8 unkb0a;
  u8 side;
  struct ObjectRuntime *first;
  struct ObjectRuntime *second;
};

void *Runtime_AllocateHeapBlockFar(s32 slot, s32 size);
void Runtime_ReleaseHeapBlock(s32 slot);
struct ObjectRuntime *Object_GetById(s32 id);
void *Resource_GetTableEntry(s32 id);
void Func_020029a0(void *resource, void *destination);
s32 Resource_FindFreeEntry(void);
s32 VramCache_Load(s32 entry, s32 size, void *data);
void Resource_ResetEntry(s32 entry);
void Func_02002b30(struct BeamSprite *sprite, s32 width, s32 height,
                   u32 attributes, s32 tile);
void Func_02002a20(struct ObjectRuntime *object);
void Func_02002a28(struct ObjectRuntime *object, s32 x, s32 y, s32 z);
void Func_02002b68(s32 sound);
void Func_0200105c(struct ObjectRuntime *object);
void FieldEvent_WaitTicks(s32 ticks);
void WaitTicks(s32 ticks);
void Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 priority);
void Scheduler_RemoveCallbackFar(void (*callback)(void));
void Func_02001080(void);

void Func_020017a8(s32 a, s32 b, s32 side) {
  struct BeamWork *work =
      Runtime_AllocateHeapBlockFar(220, sizeof(struct BeamWork));
  struct ObjectRuntime *carrier =
      *(struct ObjectRuntime **)((u8 *)Ram_HeapSlots->eventWork + 460);
  struct BeamSprite *sprite;
  struct BeamSpark *spark;
  s32 smallEntry;
  s32 largeEntry;
  s32 tile;
  s32 i;

  work->first = Object_GetById(a);
  work->second = Object_GetById(b);
  Func_020029a0(Resource_GetTableEntry(0x1eb), work);
  smallEntry = Resource_FindFreeEntry();
  tile = VramCache_Load(smallEntry, 256, work);
  work->tile = tile;
  sprite = work->sprites;
  spark = work->sparks;
  for (i = 31; i >= 0; i--) {
    Func_02002b30(sprite, 4, 4, 0, tile);
    sprite->attr5 |= 32;
    sprite->attr9 = (sprite->attr9 & 15) & ~12;
    sprite->depth = 240;
    spark->age = -1;
    sprite++;
    spark++;
  }
  Func_020029a0(Resource_GetTableEntry(0x1f6), work);
  largeEntry = Resource_FindFreeEntry();
  work->largeEntry = VramCache_Load(largeEntry, 512, work);
  Func_02002b30(&work->large, 30, 7, 0x80004000, work->largeEntry);
  work->large.depth = 240;
  work->large.attr5 |= 32;
  work->large.attr9 = ((work->large.attr9 & ~12) | 4) & 15;
  work->large.unk14 = 0;
  work->large.unk18 = 0;
  if (side == 0) {
    work->from[0] = 152 << 16;
    work->from[1] = 0x200000;
    work->from[2] = 164 << 16;
    work->to[0] = 235 << 17;
  } else {
    work->from[0] = 218 << 18;
    work->from[1] = 0x200000;
    work->from[2] = 164 << 16;
    work->to[0] = 0x2260000;
  }
  work->to[1] = 0x200000;
  work->to[2] = 164 << 16;
  work->start[0] = work->from[0];
  work->start[1] = work->from[1];
  work->start[2] = work->from[2];
  carrier->speedLimit = 0x8000;
  carrier->acceleration = 0x8000;
  Func_02002a20(carrier);
  Func_02002a28(carrier, work->start[0], work->start[2], work->start[1]);
  Func_02002b68(142);
  Func_0200105c(work->first);
  FieldEvent_WaitTicks(40);
  work->side = side;
  work->step = 0;
  work->unkb02 = 0;
  work->unkb09 = 1;
  work->unkb0a = 0;
  Scheduler_AddOrUpdateCallback(Func_02001080, 0x480);
  while (work->step != 999)
    WaitTicks(1);
  Scheduler_RemoveCallbackFar(Func_02001080);
  Resource_ResetEntry(largeEntry);
  Resource_ResetEntry(smallEntry);
  Runtime_ReleaseHeapBlock(220);
}
