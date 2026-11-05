// Under -Os, 5 November: differ 50 of 123 (tla-en); older figures below predate it.
// Draft, differ 78 of 123, skeleton gap 12 (tla-en, 2026-11-01)
// Raises each listed actor's cells: the shape grid and the cell flags of its
// cell and the one below.
// Left: allocation only; the instruction shapes agree. The native has shapes
// r5, cells r6, the object r8, level r10, index r9, and spills shape; this
// draft has cells r5, the object r6, level r8, index r10, shape r9, and keeps
// shapes in r4 saved around the call. The gap's lines are the reloads of that
// difference. The rebuilt order is local-alloc's: shapes (temporaries tied,
// refs 16) would have to outrank cells (20 over 33) and the object (20 over
// 37). Moving the shapes sum past the flag stores lifts it over the object
// but breaks the pool order and sends the object to r4.
// The native never holds r4 across a call anywhere in this overlay, though
// the object (refs 20, 3 calls), level (12, 2) and index (10, 2) all pass
// 4 * calls < refs. The raw counts (10, 6, 5) would fail it, so this looks
// like the loop weight on refs, not source; inference, not measured, and a
// compiler question for Pascal.
// Closed this round: object->terrainId read as the matched neighbours spell
// it (CSE makes the +34 pointer); a separate result local, since the native
// keeps it in r0 to the store; index <<= 2 in place, because with a new local
// or index *= 4 combine folds shapes + index into the store (gap 21); level
// >>= 19 as its own statement; FIXED_ADDRESS for the grid (gMapShapeGrid gives
// gap 14 and differ 90).
// No change: a terrain pointer local, locals outside the loop, do/while, for,
// a u8 result, separate cell, below, target or position locals.
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

void Func_02000c64(u16 *list) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct Actor *leader;
  s32 height;

  while (*list != 0xffff) {
    struct Actor *object = Actor_GetById(*list);
    s32 shape = Func_02003c10(object->terrainId, object->x, object->z);
    s32 level = Map_GetTerrainHeight(object->terrainId, object->x, object->z);
    s32 index;
    struct MapCell *cells;
    u8 *shapes;
    s32 raised;

    level >>= 19;
    index = Func_02003db0(object);
    cells = map->layers[object->terrainId].cells;
    shapes = FIXED_ADDRESS((u8 *), 0x02024000, gMapShapeGrid) +
             (cells - (struct MapCell *)Ram_MapCellBuffer);

    object->motionFlags = 0;
    object->collisionFlags = 0;
    Actor_SetSingleChildFlags(object, 0);
    level += 4;
    raised = Func_02003dd0(object->terrainId, shape, level);
    shapes += index;
    index <<= 2;
    cells = (struct MapCell *)((u8 *)cells + index);
    cells->attributeB |= 192;
    cells += 128;
    cells->attributeB |= 64;
    list++;
    *shapes = raised;
  }
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  height = Map_GetTerrainHeight(leader->terrainId, leader->x, leader->z);
  if (leader->y < height) {
    leader->terrainHeight = height;
    leader->y = height;
  }
}
