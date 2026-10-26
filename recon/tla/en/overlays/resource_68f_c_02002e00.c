// Draft, differ 82 of 83: the falling rocks' setup (Func_02002d44 spawns them):
// loads ResourceId_FallingRock's tiles into heap slot 220 and resets the 64
// sprites and timers. Left: the native counts in r8 and shares the -1 of the
// timers with the count, and rebuilds 0x500 after the VRAM load; this form
// keeps 0x500 and the work in saved registers. The same function is overlay
// 668's Func_02004004 and 692's Func_02004b18, drafts with the same gap.
#include "types.h"
#include "ram_buffer.h"

/// One falling rock: where it is, how it moves and how long it has fallen.
struct Rock {
  s32 position[3];
  s32 velocity[3];
  s32 timer;
};

/// A falling rock's sprite.
struct RockSprite {
  u8 unk00[5];
  u8 unk05;
  u8 unk06[2];
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u8 unk0a[0x14];
  u16 unk1e;
  u8 unk20[8];
};

/// The falling rocks' work: their tiles, 64 rocks and their sprites, in a
/// ring that next walks round.
struct RockWork {
  u8 tiles[0x500];
  struct Rock rocks[64];
  struct RockSprite sprites[64];
  s16 next;
  u16 tileBase;
  s16 resource;
};

void *Runtime_AllocateHeapBlockFar(s32 slot, s32 size);
void *Resource_GetTableEntry(s32 id);
extern char ResourceId_FallingRock;
void Func_02002f40(void *source, void *destination);
s32 Resource_FindFreeEntry(void);
s32 VramCache_Load(u32 slot, u32 size, const void *source);
void Func_02003190(struct RockSprite *sprite, s32 width, s32 height, u32 attr,
                   s32 tile);
s32 Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 priority);
void Func_02002cc0(void);

void Func_02002e00(void) {
  struct RockWork *work =
      Runtime_AllocateHeapBlockFar(220, sizeof(struct RockWork));
  s32 resource;
  s32 tile;
  struct Rock *rock;
  struct RockSprite *sprite;
  s32 i;

  Func_02002f40(Resource_GetTableEntry((s32)&ResourceId_FallingRock), work);
  resource = Resource_FindFreeEntry();
  tile = VramCache_Load(resource, sizeof(work->tiles), work);
  work->tileBase = tile;
  work->resource = resource;
  rock = work->rocks;
  sprite = work->sprites;
  for (i = 63; i >= 0; i--) {
    Func_02003190(sprite, 8, 8, 0x40000000, tile);
    sprite->unk05 |= 32;
    sprite->palette = 0;
    sprite->priority = 0;
    sprite->unk1e = 240;
    rock->timer = -1;
    sprite++;
    rock++;
  }
  work->next = 0;
  Scheduler_AddOrUpdateCallback(Func_02002cc0, 0x480);
}
