// Matches: differ 0 (tla-en, 2026-11-08)
// Also differ 0 in tla-ja, tla-de, tla-es, tla-fr and tla-it, under -Os with
// -mtune=gba; pool words gMapShapeGrid (0x02024000) and 0xfdff0000 as the
// scaffold's. One tagged device, the ShapeCell view, as the adopted siblings
// Func_02000c64 (dehkan_plateau/s0c64.c) and Func_02000038
// (madra_catacombs/s0038.c) write it (untagged there). The grid as
// FIXED_ADDRESS, as ten adopted functions take it, is open: as the linked
// name gMapShapeGrid, 11 of 76 instructions differ in tla-en.
// Raises the cells under the held actor again: the shape grid entry and the
// two cells' attributes.
// What closed it: the grid as FIXED_ADDRESS, as the siblings take it. Linked
// through the symbol, the grid sat in its own pseudo, one reload fewer turned
// reload's register rotation by one and the shape store took r3, so the cell
// read and write shared one copy of the cell pointer (11). As a constant it is
// reloaded, the rotation lands where the native's does, and the ShapeCell view
// lets sched2 sink the shape store past the cell read; without the view, 6.
#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "map.h"
#include "field_sprite.h"
#include "iwram_call.h"
#include "call.h"

extern u8 gMapShapeGrid[];

s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void wait(s32 frames);
u32 Random16Far(void);
s32 Math_Cosine(s32 angle);
s32 Math_Sine(s32 angle);
struct Actor *Actor_GetById(s32 id);
void Actor_SetPartAttribute(struct Actor *object, s32 value);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void ObjectVisual_CopyAttributes(s32 id, s32 value);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Func_0200056c(s32 id);
void Func_020006ec(struct Actor *object, struct Actor *other);
void Func_02000974(s32 mode);
void Func_02000aa8(void);
void Func_02000c4c(s32 level, s32 layer);
void Func_02003ba0(struct Actor *object, s32 mode);
void Func_02003ba8(struct Actor *object, const void *script);
struct Actor *Func_02003bb0(s32 type, s32 x, s32 y, s32 z);
void Func_02003c00(s32 a, s32 b, s32 c);
s32 Func_02003c10(s32 layer, s32 x, s32 z);
void Func_02003c28(void);
void Func_02003c30(void);
void Func_02003c68(s32 id, s32 x, s32 z);
void Func_02003c78(s32 id, s32 target);
void Func_02003cc0(s32 id);
s32 Func_02003d20(void);
s32 Func_02003d80(void *sprite, s32 frame);
s32 Func_02003db0(struct Actor *object);
s32 Func_02003dd0(s32 layer, s32 shape, s32 height);
struct Actor *Func_02003dd8(s32 *position, s32 mode);
void Func_02003df8(s32 cue);
void Effect_AdvanceMotionAtLeaderPriority(struct Actor *object);

/// The effect fields of an Actor, as field_effect.h names them.
struct EffectView {
  u8 unk00[0x30];
  s32 scaleRateX;
  s32 scaleRateY;
  u8 unk38[0x0c];
  s32 velocityX;
  s32 velocityY;
  s32 velocityZ;
  struct FieldSprite *sprite;
  u8 unk54[0x10];
  u16 spin;
  u8 unk66[6];
  void (*update)(struct Actor *object);
};

// One-field view: the shape store is memory of its own to sched2, which sinks
// it past the cell read; a plain store keeps its order.
struct ShapeCell {
  u8 shape;
};

#define HELD_OBJECT(work) (*(struct Actor **)((u8 *)(work) + 0xcbc))

void Func_02000e9c(void) {
  struct Actor *held = HELD_OBJECT(Ram_HeapSlots->eventWork);
  u8 *layer;
  u8 *cells;
  u8 *shapes;
  s32 shape;
  s32 level;
  s32 index;
  u8 newShape;

  if (held == NULL)
    return;
  layer = &held->terrainId;
  cells = (u8 *)Ram_HeapSlots->mapWork->layers[*layer].cells;
  shapes = FIXED_ADDRESS((u8 *), 0x02024000, gMapShapeGrid) +
           (((struct MapCell *)cells) - (struct MapCell *)Ram_MapCellBuffer);
  shape = Func_02003c10(*layer, held->x, held->z);
  level = Map_GetTerrainHeight(*layer, held->x, held->z);
  index = Func_02003db0(held);
  level >>= 19;
  level += 4;
  newShape = Func_02003dd0(*layer, shape, level);
  shapes += index;
  index <<= 2;
  /* FAKEMATCH(signature): the grid byte stored through a one-field struct
   * view (struct ShapeCell), whose alias set lets sched2 sink it past the
   * cell read; as *shapes = newShape, 6 of 76 instructions differ in tla-en. */
  ((struct ShapeCell *)shapes)->shape = newShape;
  cells += index;
  ((struct MapCell *)cells)->attributeB |= 192;
  cells += 0x200;
  ((struct MapCell *)cells)->attributeB |= 64;
}
