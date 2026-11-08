// Differ 11 of 76 (tla-en, 2026-11-08, -mtune=gba), under -Os.
// Overlay 655's Func_02000e9c (resource_655_c_02000e9c.c) instruction for
// instruction, with this overlay's own import stubs; written out here since
// 8 November rather than included, so this draft no longer moves when 655's
// does. Raises the cells under the held actor again. Left: the native's shape
// store sits between the first cell's read and write (its reload of the cell
// pointer shows the order is there before scheduling); here it comes first.
// The one-field ShapeCell view 655 uses is free (11 either way) and is
// dropped; the store after the cell's |=, a read-store-write block, an
// indexed store, or the store folded into the call give 11 to 34. Eight
// minutes of permute found only invalid candidates (a pointer assigned twice
// in one expression).
// 8 November: 655's round applies here unchanged (same function, 11).
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
s32 Func_020010ec(s32 layer, s32 x, s32 z);
void Func_02003c28(void);
void Func_02003c30(void);
void Func_02003c68(s32 id, s32 x, s32 z);
void Func_02003c78(s32 id, s32 target);
void Func_02003cc0(s32 id);
s32 Func_02003d20(void);
s32 Func_02003d80(void *sprite, s32 frame);
s32 Func_020011d4(struct Actor *object);
s32 Func_020011f4(s32 layer, s32 shape, s32 height);
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

#define HELD_OBJECT(work) (*(struct Actor **)((u8 *)(work) + 0xcbc))

void Func_02000270(void) {
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
  shapes = gMapShapeGrid +
           (((struct MapCell *)cells) - (struct MapCell *)Ram_MapCellBuffer);
  shape = Func_020010ec(*layer, held->x, held->z);
  level = Map_GetTerrainHeight(*layer, held->x, held->z);
  index = Func_020011d4(held);
  level >>= 19;
  level += 4;
  newShape = Func_020011f4(*layer, shape, level);
  shapes += index;
  index <<= 2;
  *shapes = newShape;
  cells += index;
  ((struct MapCell *)cells)->attributeB |= 192;
  cells += 0x200;
  ((struct MapCell *)cells)->attributeB |= 64;
}
