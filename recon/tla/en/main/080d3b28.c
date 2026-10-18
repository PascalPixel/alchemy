// Near miss, differ 14 of 86: redraws a one-sprite object's picture as `kind`
// (0 when flag 0x8000 is set): clears a 128-byte tile buffer in a heap block,
// draws the picture into it, uploads it to the sprite's VRAM block, releases
// the sprite's old resource record and resets its frame state. No ☀️ twin found
// (☀️'s BattleFx_StartRandomParticleEmitter counterpart 080cf17c draws the same
// way). Left: register choice only: the native builds 0x400 in r2 and the
// fill's zero in r3, this draft the other way round (the zero is kept in r10
// for the later stores in both); the offset added in its own statement changes
// nothing.
#include "types.h"
#include "dma.h"
#include "field_sprite.h"
#include "object_runtime.h"

struct ObjectRuntime *Object_GetById(s32 id);
void *Runtime_AllocateHeapBlock(s32 slot, s32 size);
void Runtime_ReleaseHeapBlock(s32 slot);
void Func_08038248(s32 kind);
s32 VramCache_Load(u32 slot, u32 size, const void *source);
void ResourceMetadata_ClearRecordFar(s32 record);

void Func_080d3b28(s32 id, s32 kind) {
  struct ObjectRuntime *object = Object_GetById(id);
  struct FieldSprite *sprite;
  u8 *tiles;
  s32 tile;
  volatile u32 fill;

  if (object == NULL)
    return;
  if (object->animationKind != 1)
    return;
  sprite = object->animation;
  if (kind & 0x8000)
    kind = 0;
  tiles = (u8 *)Runtime_AllocateHeapBlock(68, 0x608) + 0x400;
  Dma_Fill32Cell(fill, 0, tiles, 0x80);
  Func_08038248(kind);
  tile = VramCache_Load(sprite->vramBlock, 128, tiles);
  Runtime_ReleaseHeapBlock(68);
  object->unk5c[0] = 1;
  ResourceMetadata_ClearRecordFar(*(s32 *)((u8 *)sprite + 40));
  *(s32 *)((u8 *)sprite + 40) = 0;
  sprite->unk1b[0] = 0;
  sprite->fullColor = 0;
  sprite->tile = tile;
  sprite->unk19 = 0;
  sprite->nextFrame = 0;
}
