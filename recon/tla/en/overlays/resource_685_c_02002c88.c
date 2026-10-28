// Draft, differ 24 of 78: loads the glow disc picture (resource 0x1d9, which
// would need a ResourceId_GlowDisc name in gamepak/resource_directory.s) into
// the work in heap slot 55, moves every tile byte with the high nibble 0xf to
// 0x6n, loads it into VRAM and sets up the first sprite. The sprites are
// 0x28 bytes, not FieldSprite's 0x2c. Left: in the goto loop the native keeps
// the pointer in r2 and the byte in r1 (here the other way round); a
// four-minute permute does not swap them.
#include "../../../../games/tla/src/scene/champa_cliff_caves/scene.h"

struct ChampaCliffCavesSprite {
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
  u16 unk1e;
  u8 unk20[8];
};

struct ChampaCliffCavesWork {
  u8 tiles[0x1000];
  s32 originX;
  s32 originY;
  s32 originZ;
  struct ChampaCliffCavesSprite spriteA;
  struct ChampaCliffCavesSprite spriteB;
  u16 entryA;
  u16 vramA;
  u16 entryB;
  u16 vramB;
  s32 field1064;
  s32 field1068;
  s32 field106c;
  s32 field1070;
  u16 field1074;
  u8 unk1076[2];
};

extern char ResourceId_Data1F7;

void *Resource_GetTableEntry(s32 id);
void Func_020033dc(void *resource, void *destination);
s32 Resource_FindFreeEntry(void);
s32 VramCache_Load(s32 entry, s32 size, void *source);
void Func_020035fc(struct ChampaCliffCavesSprite *sprite, s32 width, s32 height,
                   u32 attributes, s32 vram);


extern char ResourceId_GlowDisc;

void Func_02002c88(void) {
  struct ChampaCliffCavesWork *work = Ram_HeapSlots->menuRuntime;
  struct ChampaCliffCavesSprite *sprite;
  s32 i;
  u8 *tile;
  s32 entry;
  s32 vram;

  Func_020033dc(Resource_GetTableEntry((s32)&ResourceId_GlowDisc), work);
  tile = work->tiles;
  i = 0;
next:
  if ((*tile & 0xf0) == 0xf0)
    *tile = (*tile & 0xf) + 0x60;
  tile++;
  i++;
  if (i < 0x1000)
    goto next;
  entry = Resource_FindFreeEntry();
  vram = VramCache_Load(entry, 0x1000, work);
  work->entryA = entry;
  work->vramA = vram;
  sprite = &work->spriteA;
  Func_020035fc(sprite, 31, 31, 0xc0000000, vram);
  sprite->fullColor = 1;
  sprite->palette = 0;
  sprite->priority = 2;
  sprite->unk1e = 1;
  work->field1064 = 0;
  work->field1068 = 0;
}
