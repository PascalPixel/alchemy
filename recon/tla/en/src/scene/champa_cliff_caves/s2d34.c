#include "../../../../../../games/tla/src/scene/champa_cliff_caves/scene.h"

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
  // A union: the two stores below keep their place after the sprite's.
  union {
    s32 value;
    u16 half;
  } field106c, field1070;
  u16 field1074;
  u8 unk1076[2];
};

void *Resource_GetTableEntry(s32 id);
void Func_020033dc(void *resource, void *destination);
s32 Resource_FindFreeEntry(void);
s32 VramCache_Load(s32 entry, s32 size, void *source);
void Func_020035fc(struct ChampaCliffCavesSprite *sprite, s32 width, s32 height,
                   u32 attributes, s32 vram);

extern char ResourceId_FireColumn;

static __inline__ void Place(struct ChampaCliffCavesSprite *a, s32 b, s32 c,
                             u32 d, s32 e) {
  /* FAKEMATCH(wrapper): forwarded inline; as a direct call, 4 of 78
   * instructions differ in tla-en. */
  Func_020035fc(a, b, c, d, e);
}

void Func_02002d34(void) {
  struct ChampaCliffCavesWork *work = Ram_HeapSlots->menuRuntime;
  struct ChampaCliffCavesSprite *sprite;
  s32 i;
  u8 *tile;
  s32 entry;
  s32 vram;

  Func_020033dc(Resource_GetTableEntry((s32)&ResourceId_FireColumn), work);
  i = 0;
  tile = work->tiles;
next:
  if ((*tile & 0xf0) == 0xf0)
    *tile = (*tile & 0xf) + 0x60;
  i++;
  tile++;
  if (i < 0x1000)
    goto next;
  entry = Resource_FindFreeEntry();
  vram = VramCache_Load(entry, 0x800, work);
  work->entryB = entry;
  work->vramB = vram;
  sprite = &work->spriteB;
  Place(sprite, 15, 47, 0xc0008000, vram);
  sprite->fullColor = 1;
  sprite->palette = 1;
  sprite->priority = 1;
  sprite->unk1e = 1;
  work->field106c.value = 0;
  work->field1070.value = 0;
}
