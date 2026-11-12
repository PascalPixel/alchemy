#include "types.h"
#include "actor.h"
#include "system/dma.h"
#include "event_inline.h"
#include "field.h"

extern const u16 Field_Palette685[];
extern char ResourceId_FieldEffectExplosion;

struct FlashSprite {
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
  u8 unk0a[2];
  s32 x;
  s32 z;
  s32 scaleX;
  s32 scaleY;
  u8 unk1c[2];
  u16 unk1e;
};

struct FlashWork {
  u8 tiles[0x400];
  struct FlashSprite sprite;
};

struct FlashWork *HeapSlot_AllocFar(s32 slot, s32 size);
void *Resource_GetTableEntry(s32 resourceId);
void Func_020033dc(void *entry, void *destination);
s32 Resource_FindFreeEntry(void);
void Func_020035fc(struct FlashSprite *sprite, s32 width, s32 height,
                   u32 attributes, s32 tile);
void Func_020035c4(s32 *position);
void Func_02003604(struct FlashSprite *sprite);
s32 Resource_ResetEntry(u32 slot);
void HeapSlot_Release(s32 slot);

static __inline__ void Place(struct FlashSprite *a, s32 b, s32 c, u32 d,
                             s32 e) {
  /* FAKEMATCH(wrapper): forwarded inline; as a direct call, 4 of 181
   * instructions differ in tla-en. */
  Func_020035fc(a, b, c, d, e);
}

void Func_020029f8(s32 id, s32 mode) {
  struct FlashWork *work = HeapSlot_AllocFar(220, 0x428);
  struct Actor *object = Actor_GetById(id);
  s32 slot;
  s32 tile;
  struct FlashSprite *sprite;
  s32 position[3];
  s32 i;
  s32 z;

  if (mode == 0) {
    for (i = 15; i >= 0; i--) {
      s32 angle = object->facing;
      angle += 0x2000;
      object->facing = angle;
      wait(1);
    }
  }
  wait(2);
  object->scaleX = 0;
  Actor_SetSingleChildFlags(object, 0);
  Dma_Set(Field_Palette685, (void *)0x050003e0, 0x80000010,
          (volatile u32 *)0x040000d4);
  Func_020033dc(Resource_GetTableEntry((s32)&ResourceId_FieldEffectExplosion),
                work);
  slot = Resource_FindFreeEntry();
  tile = Vram_Load(slot, 0x400, work);
  sprite = &work->sprite;
  Place(sprite, 8, 16, 0x40000000, tile);
  sprite->unk1e = 240;
  sprite->priority = 1;
  sprite->fullColor = 0;
  sprite->palette = 15;
  position[0] = object->x;
  position[1] = object->y;
  position[2] = object->z;
  Func_020035c4(position);
  sprite->x = position[0];
  sprite->z = z = position[2];
  if (mode == 1) {
    sprite->scaleX = 0x20000;
    sprite->scaleY = 0x20000;
    sprite->z = z + 0xa0000;
  }
  Sound_Play(145);
  for (i = 0; i <= 71; i++) {
    sprite = &work->sprite;
    switch (i) {
    case 6:
      sprite->tile = tile + 4;
      break;
    case 12:
      sprite->tile = tile + 8;
      break;
    case 22:
      sprite->tile = tile + 12;
      break;
    case 32:
      sprite->tile = tile + 16;
      break;
    case 42:
      sprite->tile = tile + 20;
      break;
    case 52:
      sprite->tile = tile + 24;
      break;
    case 62:
      sprite->tile = tile + 28;
      break;
    }
    Func_02003604(sprite);
    wait(1);
  }
  Actor_SetHorizontalPositionWithTerrain(id, 0, 0);
  Resource_ResetEntry(slot);
  HeapSlot_Release(220);
}
