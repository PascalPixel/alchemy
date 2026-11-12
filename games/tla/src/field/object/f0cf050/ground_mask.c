/// @file
/// Routines whose twins are in The Broken Seal's
/// field/field_sequence/ground_mask.c.
// Part of f0cf050.c.

#include "../f0cf050_internal.h"

/// Suspends the fade overlay's drawing callback.
void FadeOverlay_Suspend(void) { Task_SuspendAll(Func_080d1840); }

/// Resumes the fade overlay's drawing callback.
void FadeOverlay_Resume(void) { Task_ResumeAll(Func_080d1840); }

/// The blank overlay's task, which Func_080d1cc0 starts: each frame it draws
/// a 16-pixel mark beside the actor where the map's third layer flags a cell
/// two rows up (left or right of the actor), and another for the row one up.
void Func_080d1ae4(void) {
  struct AnimationSpritePart *mark = Ram_HeapSlots->unk84[3];
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s16 *camera = (s16 *)&map->viewX;
  s16 cameraX = camera[1];
  s16 cameraZ = camera[3];
  struct FadeActor *actor = ((struct FadeOverlayWork *)mark)->actor;
  s32 x;
  s32 row;
  u32 tile;
  struct MapCell *cells;
  struct MapCell *a;
  struct MapCell *b;
  struct AnimationSpritePart *m;

  if (actor == 0)
    return;
  x = ((s16 *)&actor->x)[1];
  row = (actor->z - actor->y) >> 16;
  tile = gResourceTableEntries[94].offset >> 5;
  cells = map->layers[2].cells;
  a = cells + (x + 8) / 16 + (row / 16 - 2) * 128;
  b = cells + (x - 8) / 16 + (row / 16 - 2) * 128;
  if ((a->attributeB & 1) || (b->attributeB & 1)) {
    m = mark;
    ((u32 *)m)[1] = 0x40000000;
    ((u32 *)m)[2] = 0x400;
    m->tile = tile;
    m->x = (u16)(x - cameraX) - 8;
    m->y = (u16)(((u16)row & 0xf0) - cameraZ) - 32;
    mark = m + 1;
    Func_080140d8(m, 1);
  }
  a = cells + (x + 8) / 16 + (row / 16 - 1) * 128;
  b = cells + (x - 8) / 16 + (row / 16 - 1) * 128;
  if ((a->attributeB & 1) || (b->attributeB & 1)) {
    m = mark;
    ((u32 *)m)[1] = 0x40000000;
    ((u32 *)m)[2] = 0x400;
    m->tile = tile;
    m->x = (u16)(x - cameraX) - 8;
    m->y = (u16)(((u16)row & 0xf0) - cameraZ) - 16;
    Func_080140d8(m, 1);
  }
}

/// Like Func_080d1a18 (the fade overlay) with blank tiles and no blending:
/// loads 0x200 zeroed tiles to VRAM slot 94 and starts Func_080d1ae4 on
/// object (the controlled owner's when 0).
void Func_080d1cc0(s32 object) {
  u8 *work = HeapSlot_AllocEwram(144, 28);
  void *buffer = HeapSlot_Alloc(56, 0x400);

  Dma_Fill32(0, buffer, 0x200);
  VramCache_Load(94, 0x200, buffer);
  Task_Add(Func_080d1ae4, 0x480);
  HeapSlot_Release(56);
  if (object == 0)
    object = ObjectTable_Get(gMainRoutineWork.currentOwner);
  *(s32 *)(work + 24) = object;
}

/// Suspends the Func_080d1ae4 overlay task.
void Func_080d1d34(void) { Task_SuspendAll(Func_080d1ae4); }

/// Resumes the Func_080d1ae4 overlay task.
void Func_080d1d44(void) { Task_ResumeAll(Func_080d1ae4); }

/// Fills the overlay's 512 bytes of sprite tiles (VRAM cache slot 94) with
/// colour 1 when solid is set, else clears them.
void BattleEffect_FillOverlayTiles(s32 solid) {
  void *buffer = HeapSlot_Alloc(56, 0x400);

  if (solid == 0) {
    Dma_Fill32(0, buffer, 0x200);
  } else {
    Dma_Fill32(0x01010101, buffer, 0x200);
  }
  VramCache_Load(94, 0x200, buffer);
  HeapSlot_Release(56);
}

struct EffectSprite {
  u8 unk00[4];
  u16 y : 8;
  u16 affineMode : 2;
  u16 objectMode : 2;
  u16 mosaic : 1;
  u16 palette256 : 1;
  u16 shape : 2;
  u16 attr1;
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u8 unk0a[20];
  u16 depth;
};

void Func_080eaf98();

/// Sets up sprite through Func_080eaf98 with resource 94's tiles, at
/// priority 1 in palette 0.
void Func_080d1dac(struct EffectSprite *sprite, s32 a, s32 b, s32 c) {
  Func_080eaf98(sprite, a, b, c, gResourceTableEntries[94].offset >> 5);
  sprite->depth = 0;
  sprite->priority = 1;
  sprite->palette256 = 0;
  sprite->palette = 0;
}


void Camera_WorldToScreen(s32 *position);
void Func_080eb01c(void *sprite);

/// Projects the world position (x, y, z) to the screen, stores it as the
/// effect sprite's position and draws the sprite.
void BattleEffect_DrawSpriteAtWorldPosition(s32 *sprite, s32 x, s32 y, s32 z) {
  s32 position[3];

  SET_VECTOR(position, x, y, z);
  Camera_WorldToScreen(position);
  sprite[3] = position[0];
  sprite[4] = position[2];
  Func_080eb01c(sprite);
}

/// The twin of Func_080d1dac with object mode 1 and priority 0.
void Func_080d1e18(struct EffectSprite *sprite, s32 a, s32 b, s32 c) {
  Func_080eaf98(sprite, a, b, c, gResourceTableEntries[94].offset >> 5);
  sprite->depth = 0;
  sprite->priority = 0;
  sprite->palette256 = 0;
  sprite->palette = 0;
  sprite->objectMode = 1;
}

struct BattleFxDefinition {
  u16 id;
  u8 value;
  u8 flags;
};

extern struct BattleFxDefinition gData_080f0308[];

/// Finds the battle effect definition of id among the first 243 entries; when
/// none matches it returns entry 243.
struct BattleFxDefinition *BattleEffect_FindDefinition(u32 id) {
  struct BattleFxDefinition *entry = gData_080f0308;
  u32 index = 0;

  if (entry->id != id) {
    do {
      index += 1;
      entry++;
      if (index > 242)
        break;
    } while (entry->id != id);
  }
  return entry;
}

struct BattleRenderObject {
  u8 unk00[0x50];
  struct ObjectAnimationState *animation;
  u8 kind;
};

s32 BattleEffect_GetAnimationValue(u32 objectId) {
  struct BattleRenderObject *object =
      (struct BattleRenderObject *)ObjectTable_Get(objectId);

  if (object->kind != 1 || object->animation == NULL ||
      object->animation->value28 == NULL) {
    return 0;
  }
  return *object->animation->value28;
}
#undef MAP_LAYER_FLAGS
#undef COLOR_MAX
#undef COLOR_HALF
#undef gColorBuffer_ChannelCurve
#undef gColorBuffer_ChannelCurveLight
#undef gColorBuffer_ChannelCurveContrast
#undef PaletteFade_ClampComponent
#undef PaletteFade_ClampShiftedComponent
