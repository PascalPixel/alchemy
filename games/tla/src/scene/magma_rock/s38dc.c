/// @file
/// The falling rocks' setup: loads their picture, sets up 64 small sprites,
/// marks each rock idle and starts the task that spawns them.
#include "types.h"
#include "system/task.h"
#include "event_inline.h"

extern char ResourceId_FallingRock;

struct Rock {
  u8 unk00[24];
  s32 timer;
};

struct RockSprite {
  u8 unk00[5];
  u8 unk05_0 : 5;
  u8 unk05_5 : 1;
  u8 unk05_6 : 2;
  u8 unk06[3];
  u8 unk09_0 : 2;
  u8 priority : 2;
  u8 palette : 4;
  u8 unk0a[20];
  u16 unk1e;
  u8 unk20[8];
};

struct RockWork {
  u8 tiles[0x500];
  struct Rock rocks[64];
  struct RockSprite sprites[64];
  s16 next;
  u16 tileBase;
  u16 resource;
};

struct RockWork *HeapSlot_AllocFar(s32 slot, s32 size);
void *Resource_GetTableEntry(s32 id);
void Func_02003a14(void *entry, void *destination);
s32 Resource_FindFreeEntry(void);
void Func_02003c34(struct RockSprite *sprite, s32 width, s32 height,
                   u32 attributes, s32 tile);
void Func_0200379c(void);

void Func_020038dc(void) {
  struct RockWork *work = HeapSlot_AllocFar(220, sizeof(struct RockWork));
  s32 entry;
  s32 tile;
  s32 i;
  struct Rock *rock;
  struct RockSprite *sprite;

  Func_02003a14(Resource_GetTableEntry((s32)&ResourceId_FallingRock), work);
  entry = Resource_FindFreeEntry();
  /* FAKEMATCH(wrapper): forwarded, so that its constant goes straight to the
   * argument; as a direct VramCache_Load call, 41 of 83 instructions differ in
   * tla-en. */
  tile = Vram_Load(entry, 0x500, work);
  work->tileBase = tile;
  work->resource = entry;
  rock = work->rocks;
  sprite = work->sprites;
  for (i = 63; i >= 0; i--, sprite++, rock++) {
    Func_02003c34(sprite, 8, 8, 0x40000000, tile);
    sprite->unk05_5 = 1;
    sprite->palette = 0;
    sprite->priority = 0;
    sprite->unk1e = 240;
    rock->timer = -1;
  }
  {
    s32 zero = 0;

    work->next = zero;
  }
  /* FAKEMATCH(wrapper): forwarded, so that its constant goes straight to the
   * argument; as a direct Task_Add call, 6 of 83 instructions differ in
   * tla-en. */
  Task_AddFrame(Func_0200379c, 0x480);
}
