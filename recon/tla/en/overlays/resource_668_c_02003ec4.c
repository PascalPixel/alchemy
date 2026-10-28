// Draft of overlay 668's falling rocks, copied from overlay 6a8's
// Func_02004114, Func_02004198 and Func_02004254 (the same listings), whose
// draft holds the notes: Func_02003ec4 (differ 44 of 57) and Func_02004004 (83 of
// 83); the spawn between them, Func_02003f48, is adopted as
// eastern_alhafra/s3f48.c; diff each with --symbol.
#include "types.h"
#include "ram_buffer.h"

struct Rock {
  s32 position[3];
  s32 velocity[3];
  s32 timer;
};

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

struct RockWork {
  u8 tiles[0x500];
  struct Rock rocks[64];
  struct RockSprite sprites[64];
  s16 next;
  u16 tileBase;
  s16 resource;
};

extern char ResourceId_FallingRock;

s32 Random16Far(void);
void Vector_AddPolarOffsetFar(s32 radius, s32 angle, s32 *vector);
void *HeapSlot_AllocFar(s32 slot, s32 size);
void *Resource_GetTableEntry(s32 id);
s32 Resource_FindFreeEntry(void);
s32 VramCache_Load(s32 entry, s32 size, void *data);
void Task_Add(void (*func)(void), s32 priority);
void Func_020049d4(void *resource, void *destination);
void Func_02004c64(struct RockSprite *sprite, s32 a, s32 b, s32 c, s32 tile);
void Func_02004c6c(struct RockSprite *sprite, struct Rock *rock);
void Func_02004c74(struct Rock *rock, s32 a, s32 b);

static __inline__ s32 VramLoad(s32 entry, s32 size, void *data) {
  /* FAKEMATCH: as in the scene layer, the inline parameters keep the size its
   * own constant; called directly, the rock offset reuses it across the call.
   */
  return VramCache_Load(entry, size, data);
}

void Func_02003ec4(void) {
  struct RockWork *work = Ram_HeapSlots->menuRuntime;
  struct Rock *rock = work->rocks;
  struct RockSprite *sprite = work->sprites;
  s32 i;

  /* FAKEMATCH: the loop is written with goto so that it carries no loop notes:
   * the bytes rebuild the constants on every pass and count in r8, and a for
   * loop hoists them. */
  i = 63;
loop:
  {
    s32 timer = rock->timer;

    if ((u32)timer <= 19) {
      sprite->tile = work->tileBase + ((timer >> 2) & 7) * 8;
      Func_02004c6c(sprite, rock);
      Func_02004c74(rock, 63, -0x8000);
      rock->timer++;
    }
    sprite++;
    rock++;
  }
  i--;
  if (i >= 0)
    goto loop;
}

void Func_02004004(void) {
  struct RockWork *work =
      HeapSlot_AllocFar(220, sizeof(struct RockWork));
  struct Rock *rock;
  struct RockSprite *sprite;
  s32 entry;
  s32 tile;
  s32 i;

  Func_020049d4(Resource_GetTableEntry((s32)&ResourceId_FallingRock), work);
  entry = Resource_FindFreeEntry();
  tile = VramLoad(entry, 0x500, work);
  work->tileBase = tile;
  work->resource = entry;
  rock = work->rocks;
  sprite = work->sprites;
  /* FAKEMATCH: the loop is written with goto so that it carries no loop notes:
   * the bytes rebuild the constants on every pass and count in r8, and a for
   * loop hoists them. */
  i = 63;
loop:
  {
    Func_02004c64(sprite, 8, 8, 0x40000000, tile);
    sprite->unk05 |= 0x20;
    sprite->palette = 0;
    sprite->priority = 0;
    sprite->unk1e = 240;
    i += -1;
    rock->timer = -1;
    sprite++;
    rock++;
  }
  if (i >= 0)
    goto loop;
  work->next = 0;
  Task_Add(Func_02003ec4, 0x480);
}
