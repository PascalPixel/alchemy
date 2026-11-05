// Draft of overlay 668's Func_02004004, translated from its listing.
#include "types.h"
#include "call.h"
s32 HeapSlot_AllocFar();
s32 Resource_GetTableEntry();
s32 Func_020049d4();
s32 Resource_FindFreeEntry();
s32 VramCache_Load();
s32 Func_02004c64();
s32 Task_Add();
extern u8 Func_02003ec4[];

extern char ResourceId_FallingRock;

struct SpriteRecord {
  u8 unk00[5];
  u8 flags5;
  u8 unk06[3];
  u8 flags9;
  u8 unk0a[30 - 10];
  u16 unk1e;
  u8 unk20[8];
};

struct ChainRecord {
  u8 unk00[24];
  s32 next;
};

struct SceneryWork {
  u8 unk0000[0x500];
  struct ChainRecord chain[64];
  u8 unk0c00[0];
  struct SpriteRecord sprites[64];
  u16 count;
  u16 tiles;
  u16 entry;
};

void Func_02004004(void) {
  struct SceneryWork *work = (struct SceneryWork *)HeapSlot_AllocFar(220, 0x1608);
  s32 entry, tiles, i;
  struct SpriteRecord *sprite;
  struct ChainRecord *chain;

  Func_020049d4(Resource_GetTableEntry((s32)&ResourceId_FallingRock), work);
  entry = Resource_FindFreeEntry();
  tiles = Value3((s32 (*)())VramCache_Load, entry, 0x500, (s32)work);
  work->tiles = tiles;
  work->entry = entry;
  chain = (struct ChainRecord *)((u8 *)work + 0x500);
  sprite = (struct SpriteRecord *)((u8 *)work + 0xc00);
  for (i = 63; i >= 0; i--) {
    Func_02004c64(sprite, 8, 8, 0x40000000, tiles);
    {
      s32 bit = sprite->flags5 | 32;

      sprite->flags5 = bit;
    }
    {
      s32 flags = sprite->flags9 & 15;

      flags &= ~12;
      sprite->flags9 = flags;
    }
    sprite->unk1e = 240;
    chain->next = -1;
    sprite++;
    chain++;
  }
  work->count = 0;
  Task_Add((s32)Func_02003ec4, 0x480);
}
