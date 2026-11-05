// Draft, differ 23: overlay 6b9's copy of the function drafted in
// resource_691_c_020038dc.c, whose header says what it is and what still differs.
#include "call.h"
#include "types.h"
#include "system/task.h"

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
void Func_02000560(void *entry, void *destination);
s32 Resource_FindFreeEntry(void);
s32 VramCache_Load(s32 entry, s32 size, void *data);
void Func_020005f0(struct RockSprite *sprite, s32 width, s32 height,
                   u32 attributes, s32 tile);
void Func_0200015c(void);

void Func_0200029c(void) {
  struct RockWork *work = HeapSlot_AllocFar(220, sizeof(struct RockWork));
  s32 entry;
  s32 tile;
  s32 i;
  struct Rock *rock;
  struct RockSprite *sprite;

  Func_02000560(Resource_GetTableEntry((s32)&ResourceId_FallingRock), work);
  tile = VramCache_Load(entry, 0x500, work);
  entry = Resource_FindFreeEntry();
  work->tileBase = tile;
  rock = work->rocks;
  work->resource = entry;
  /* FAKEMATCH(barrier): an empty asm holds the order around it; as no asm,
   * 25 of 83 instructions differ in tla-en. */
  asm("");
  sprite = work->sprites;
  i = 63;
  do {
    Func_020005f0(sprite, 8, 8, 0x40000000, tile);
    sprite->unk05_5 = 1;
    sprite->palette = 0;
    sprite->priority = 0;
    sprite->unk1e = 240;
    rock->timer = -1;
    i--;
    sprite++;
    rock++;
  } while (i >= 0);
  {
    s32 zero = 0;

    work->next = zero;
  }
  Call2((void (*)())Task_Add, (s32)Func_0200015c, 0x480);
}
