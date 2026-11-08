// Draft, differ 11 of 76 (tla-en, 2026-11-08)
// Under -Os and -mtune=gba; ja, de, es, fr and it measure the same 11.
// Overlay 655's Func_02000e9c (resource_655_c_02000e9c.c) instruction for
// instruction, with this overlay's own import stubs; written out here so this
// draft no longer moves when 655's does. Raises the cells under the held actor
// again.
// Left: the first cell's reload registers. The native copies the shape pointer
// to r1, the cell pointer to r3 for the read and to r1 again for the write;
// here the shape store takes r3, the read r1 and the write reuses that r1.
// Reload hands out r1, r2, r3 in turn (reload1.c, last_spill_reg), so the
// native's registers follow from this draft's own order (shape store first)
// when the turn already stands on r3 before the region: one more allocated
// reload than here between the 0xfdff0000 reload and the shape store. The
// likeliest is gMapShapeGrid loaded by reload at its add, `ldr r3` either way,
// instead of local-alloc's pseudo 59. A shape store through a one-byte struct
// view has its own alias set, which lets sched2 lift the read above the store
// as the native has it; a throwaway extra read before the region moved the
// turn, and the read, the 512 and the second cell then took the native's
// registers. No source form found yet makes the grid a reload: a grid local
// assigned at the top, before the cell load or at the add (35, 45, 11);
// &gMapShapeGrid[i], an int offset, a u32 sum or the grid assigned first and
// added to (11, 11, 11, 29).
// Also tried 8 November: the read held in a local before the shape store (u8,
// u16, s16, u32, s32, const, cast at the or, a constant local: 28 to 35;
// regmove ties the or's result to the read, not to 192); the or'ed value held
// across the store (33: the later shape store drops shapes below cells in
// local-alloc priority and swaps r8 and sl); the store in a statement
// expression inside the |= (31); the view stored after the |= (33). Six
// permuter minutes from the view found only a wrong-shift candidate (9).
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
