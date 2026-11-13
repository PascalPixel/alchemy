#include "../shared/map_cells.h"
#include "tundaria_tower.h"
#include "call.h"
#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "field_event.h"
#include "event_inline.h"
#include "field.h"
#include "module/field/field.h"
#include "module/field/object.h"
#include "module/scene/scene.h"
#include "module/scene/tundaria_tower.h"
#include "module/tla.h"

s32 MapCollision_FindOrAddTag(s32 layer, s32 shape, s32 height);

void Func_02001e5c(s32 value) { gData_02003cd0 = value; }

void Func_020017dc(void);
void Import_ActorSelectAnimation(struct Actor *object, s32 mode);

void Func_02001e68(void) {
  struct Actor *object;
  s16 angle;
  s32 startCell;
  s32 targetX;
  s32 targetZ;
  s32 radius;
  s32 position[3];
  s32 *p;
  s32 x;
  s32 z;
  u8 *layer;
  s32 cell;
  s32 *q;
  struct Actor *entry;
  s32 j;
  u8 *flags;

  object = ObjectTable_Get(gMainRoutineWork.currentOwner);
  angle = (object->facing + 0x2000) & 0xc000;
retry:
  Func_02001868();
  object->speedLimit = 0x10000;
  object->acceleration = 0x8000;
  object->targetX = 0x80000000;
  object->targetZ = 0x80000000;
  object->velocityX = 0;
  object->velocityZ = 0;
  p = position;
  p[0] = x = (object->x & 0xfff00000) + 0x80000;
  p[1] = object->y;
  p[2] = z = (object->z & 0xfff00000) + 0x80000;
  targetX = x;
  targetZ = z;
  layer = &object->terrainId;
  startCell = MapGrid_GetCellBehaviorAt(*layer, x, z);
  Vector_AddPolarOffset(0x100000, angle, p);
  cell = MapGrid_GetCellBehaviorAt(*layer, p[0], p[2]);
  if (cell == 255 ||
      Map_GetTerrainHeight(*layer, p[0], p[2]) - object->y > 0x80000) {
    object->facing = angle;
    object->velocityX = 0;
    object->velocityZ = 0;
    object->x = x;
    object->z = z;
    goto end;
  }
  p[0] = x;
  p[2] = z;
  object->speedLimit = 0x20000;
  object->acceleration = 0x1999;
  object->timer = 0;
  Import_ActorSelectAnimation(object, 2);
  Actor_SetAnimationSpeed(object, 48);
  object->update = Func_020017dc;
  goto walk;
walkNext:
  if (Map_GetTerrainHeight(*layer, q[0], q[2]) - object->y > 0x80000)
    goto blocked;
  radius = object->radius - 2;
  entry = Ram_HeapSlots->scriptObjects;
  j = 0;
  flags = &entry->collisionFlags;
overlap:
  if (entry->script != NULL && (1 & *flags) && entry != object) {
    if (Value4((s32 (*)())Import_FieldCheckSpheresOverlap, (s32)&entry->x,
               radius, (s32)position, entry->radius - 2) >= 0)
      goto blocked;
  }
  j++;
  flags += sizeof(*entry);
  entry++;
  if (j <= 63)
    goto overlap;
  targetX = q[0];
  targetZ = q[2];
  object->speedLimit = 0x20000;
  object->acceleration = 0x1999;
  Actor_SetMoveTarget(object, q[0], q[1], q[2]);
  Actor_WaitForMove(object);
  if (cell != startCell)
    goto moved;
walk:
  /* FAKEMATCH(barrier): an empty asm clobbering sfp stops gcse hoisting the
   * position's address; as no asm, 43 of 246 instructions differ in tla-en. */
  __asm__("" : : : SFP_CLOBBER);
  Call3((void (*)())Vector_AddPolarOffset, 0x100000, angle, (s32)position);
  q = position;
  cell = MapGrid_GetCellBehaviorAt(*layer, q[0], q[2]);
  if (cell != 255)
    goto walkNext;
blocked:
  object->speedLimit = 0x20000;
  object->acceleration = 0x10000;
  Actor_SetMoveTarget(object, targetX, object->y, targetZ);
  Actor_WaitForMove(object);
  wait(2);
  goto retry;
moved:
  wait(10);
end:
  object->update = NULL;
  object->actionFlags |= 1;
  object->acceleration = 0x4000;
  Import_ActorSelectAnimation(object, 1);
}

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))

void Func_02002094(u16 *table) {
  u16 *row;
  s32 id;
  struct Actor *object;
  s16 x, z;

  if (table[0] == 0xffff)
    return;
  row = table;
loop:
  id = row[0];
  object = Actor_GetById(id);
  x = row[1];
  z = row[2];

  object->collisionFlags &= 0xfe;
  Actor_SetSpriteFlags(object, 0);
  Func_02002184(id, (u16)x, (u16)z, BYTE_AT(object, 34));
  row += 3;
  if (row[0] != 0xffff)
    goto loop;
}

// Finds the current target's table row and shrinks it.
void Func_02002100(u16 *list, s32 mode, s32 count) {
  struct GabombaStatueEffectWork *work = Ram_HeapSlots->effectWork;

  if (*list == 0xffff)
    return;
  // A goto loop: as a do-while loop, 42 of 61 instructions differ in tla-en.
top:
  {
    u32 entry = *list;
    s32 id = work->objectId;

    if (id == entry) {
      struct Actor *object = Actor_GetById(id);
      s32 a;
      s32 flag;

      list++;
      a = list[0];
      flag = list[1];
      if (count < 8)
        object->scaleY -= 0x1f00;
      if (mode == 1) {
        GameFlag_Set(flag);
        Func_02002184(id, a, flag, object->terrainId);
        object->scaleY = 0x10000;
      }
      return;
    }
  }
  list += 3;
  if (*list != 0xffff)
    goto top;
}

#include "map.h"

DEFINE_ACTOR_SET_CELL_OPEN(Func_02002184, Func_02003160,
                           Import_ActorSelectAnimation)

// On entering, sets each listed pillar from its two flags (pushed: moved to its
// place; else its cell raised and its shape redrawn), then lifts the leader
// onto the ground under it.

/// On entering, sets each listed pillar from its two flags (pushed: moved to
/// its place; else its cell raised and its shape redrawn), then lifts the
/// leader onto the ground under it.
void Func_0200222c(const u16 *table) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct Actor *object;
  s32 id;
  s32 flag;
  s32 nextFlag;
  struct MapCell *cells;
  u8 *shapes;
  struct MapCell *cell;
  s32 index;
  struct Actor *leader;
  s32 ground;

  if (*table == 0xffff)
    goto done;
next:
  id = table[0];
  object = Actor_GetById(id);
  flag = table[1];
  nextFlag = flag + 1;
  cells = map->layers[object->terrainId].cells;
  shapes = Ram_MapShapeGrid + (cells - (struct MapCell *)Ram_MapCellBuffer);
  if (GameFlag_Get(nextFlag)) {
    Actor_SetHorizontalPositionWithTerrain(id, 0, 0);
  } else {
    s32 shape;
    u8 *slot;
    s32 height;

    index = Func_02003160(object);
    cell = &cells[index];
    slot = shapes + index;
    shape = MapGrid_GetTerrainKind(object->terrainId, object->x, object->z);
    height = Map_GetTerrainHeight(object->terrainId, object->x, object->z);
    Actor_DisableById(id);
    cell->attributeB = 128;
    *slot = MapCollision_FindOrAddTag(object->terrainId, shape, (height >> 19) + 4);
    if (GameFlag_Get(flag))
      Import_ActorSelectAnimation(object, 0);
  }
  table += 2;
  if (*table != 0xffff)
    goto next;
done:
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  ground = Map_GetTerrainHeight(leader->terrainId, leader->x, leader->z);
  if (leader->y < ground) {
    leader->terrainHeight = ground;
    leader->y = ground;
  }
}

void Func_02002354(const u16 *table) {
  struct Actor *leader;
  struct Actor *trap;
  s32 position[3];
  s32 id;
  s32 flag;
  s32 second;

  if (Field_TryJumpForward() != 0)
    return;
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  position[0] = leader->x;
  position[1] = leader->y;
  position[2] = leader->z;
  trap = Actor_FindAtCell(position, 0);
  if (trap != 0)
    goto test;
  return;
next:
  id = table[0];
  if (Actor_GetById(id) != trap) {
    table += 2;
    goto test;
  }
  {
    flag = table[1];
    Event_Start();
    if (!GameFlag_Get(flag)) {
      Sound_Play(125);
      Actor_SetSpriteColor(Actor_GetById(id), 7);
      wait(2);
      Import_ActorSelectAnimation(trap, 0);
      Actor_SetSpriteColor(Actor_GetById(id), 0);
      wait(2);
      Actor_SetSpriteColor(Actor_GetById(id), 7);
      wait(4);
      Actor_SetSpriteColor(Actor_GetById(id), 0);
      Func_020029d0(0);
      GameFlag_Set(flag);
    } else {
      second = flag + 1;
      if (!GameFlag_Get(second)) {
        leader->motionFlags = 0;
        Sound_Play(185);
        MapGrid_SetShake(0x10000, 0x40000, 0x10000);
        Func_020029d0(0);
        wait(8);
        Import_ActorSelectAnimation(trap, 2);
        Func_02002ca8(-2, trap->terrainId);
        Func_020029d0(1);
        wait(16);
        Func_02002ca8(-2, trap->terrainId);
        wait(4);
        MapGrid_SetShake(-1, -1, 0xe666);
        wait(8);
        leader->motionFlags = 3;
        wait(5);
        Actor_SetPositionAndResetMotion(trap, 0, 0, 0);
        wait(2);
        Sound_Play(188);
        Func_02002b04();
        wait(20);
        GameFlag_Set(second);
      }
    }
    Func_02003050();
    return;
  }
test:
  if (*table != 0xffff)
    goto next;
}

void Func_020024e8(u16 *row, s32 id) {
  struct Actor *object = Actor_GetById(id);
  s32 flag;
  s32 nextFlag;

  if (*row == 0xffff)
    return;
loop:
  if (*row != id) {
    row += 2;
    goto test;
  }
  flag = row[1];
  Event_Start();
  nextFlag = flag + 1;
  if (!GameFlag_Get(nextFlag)) {
    Sound_Play(185);
    TundariaTower_SetWork(0x10000, 0x40000, 0x10000);
    Func_020029d0(0);
    wait(8);
    Import_ActorSelectAnimation(object, 2);
    Func_02002c1c(-4, object->terrainId, id);
    Func_020029d0(1);
    wait(16);
    TundariaTower_SetWork(-1, -1, 0xe666);
    wait(8);
    Actor_SetPositionAndResetMotion(object, 0, 0, 0);
    wait(2);
    Sound_Play(188);
    Func_02002b04();
    wait(20);
    GameFlag_Set(flag);
    GameFlag_Set(nextFlag);
  }
  Func_02003050();
  return;
test:
  if (*row != 0xffff)
    goto loop;
}

void Func_020025c8(s32 id) {
  Actor_GetById(id)->unk5c[0] = 3;
  Actor_DisableById(id);
}

// Places the listed actors on their cells.

/// Places actors 8, 9 and 10, then walks a 0xffff-ended list of (actor, flag)
/// halfword pairs: flag + 1 set parks the actor at (0, 0); otherwise its cell
/// gets attributeB 128 and its shape-grid byte is rewritten, and flag set makes
/// it copy actor 9's visual attributes.
void Func_020025e0(u16 *list) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct Actor *object;
  s32 id;
  s32 flag;
  s32 nextFlag;
  struct MapCell *cells;
  u8 *shapes;
  struct MapCell *cell;
  s32 index;
  u8 *grid = gMapShapeGrid; // the shape grid, read once

  Actor_MoveToObject(8, gMainRoutineWork.currentOwner);
  Actor_MoveToObject(9, gMainRoutineWork.currentOwner);
  Actor_MoveToObject(10, gMainRoutineWork.currentOwner);
  wait(1);
  Func_020025c8(8);
  Func_020025c8(9);
  Func_020025c8(10);
  Actor_SetModeById(9, 0);
  wait(1);
  Actor_SetHorizontalPositionWithTerrain(8, 0, 0);
  Actor_SetHorizontalPositionWithTerrain(9, 0, 0);
  Actor_SetHorizontalPositionWithTerrain(10, 0, 0);
  wait(1);
  if (*list == 0xffff)
    goto done;
next:
  id = list[0];
  object = Actor_GetById(id);
  flag = list[1];
  nextFlag = flag + 1;
  cells = map->layers[object->terrainId].cells;
  shapes = grid + (cells - (struct MapCell *)Ram_MapCellBuffer);
  object->unk5c[0] = 3;
  if (GameFlag_Get(nextFlag)) {
    Actor_SetHorizontalPositionWithTerrain(id, 0, 0);
  } else {
    s32 shape;
    u8 *slot;
    s32 height;

    index = Func_02003160(object);
    cell = &cells[index];
    slot = shapes + index;
    shape = MapGrid_GetTerrainKind(object->terrainId, object->x, object->z);
    height = Map_GetTerrainHeight(object->terrainId, object->x, object->z);
    Actor_DisableById(id);
    cell->attributeB = 128;
    *slot = MapCollision_FindOrAddTag(object->terrainId, shape, (height >> 19) + 4);
    if (GameFlag_Get(flag))
      Actor_CopyGraphics(id, 9);
  }
  list += 2;
  if (*list != 0xffff)
    goto next;
done:
  wait(10);
}

void Func_02002748(struct Actor *object, struct Actor *target) {
  Actor_SetPositionAndResetMotion(object, target->x, target->y, target->z);
  Actor_SetPositionAndResetMotion(target, 0, 0, 0);
}

void Func_02002764(const u16 *table) {
  struct Actor *leader;
  struct Actor *partner;
  struct Actor *trap;
  s32 position[3];
  s32 id;
  s32 flag;
  s32 second;
  u8 *motion;

  if (Field_TryJumpForward() != 0)
    return;
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  partner = Actor_GetById(8);
  position[0] = leader->x;
  position[1] = leader->y;
  position[2] = leader->z;
  trap = Actor_FindAtCell(position, 0);
  if (trap != 0) {
    goto test;
  next:
    id = table[0];
    if (Actor_GetById(id) != trap) {
      table += 2;
      goto test;
    }
    flag = table[1];
    Event_Start();
    if (!GameFlag_Get(flag)) {
      Import_ActorSelectAnimation(partner, 1);
      Func_02002748(partner, trap);
      wait(1);
      Sound_Play(125);
      Actor_SetSpriteColor(Actor_GetById(8), 7);
      wait(2);
      Import_ActorSelectAnimation(partner, 0);
      Actor_CopyGraphics(id, 9);
      Actor_SetSpriteColor(Actor_GetById(8), 0);
      wait(2);
      Actor_SetSpriteColor(Actor_GetById(8), 7);
      wait(4);
      Actor_SetSpriteColor(Actor_GetById(8), 0);
      Func_020029d0(0);
      Func_02002748(trap, partner);
      wait(1);
      GameFlag_Set(flag);
    } else {
      second = flag + 1;
      if (!GameFlag_Get(second)) {
        Import_ActorSelectAnimation(partner, 0);
        Func_02002748(partner, trap);
        motion = &leader->motionFlags;
        wait(1);
        *motion = 0;
        Sound_Play(185);
        MapGrid_SetShake(0x10000, 0x40000, 0x10000);
        Func_020029d0(0);
        wait(8);
        Import_ActorSelectAnimation(partner, 2);
        Func_02002ca8(-2, trap->terrainId);
        Func_020029d0(1);
        wait(16);
        Func_02002ca8(-2, trap->terrainId);
        wait(4);
        MapGrid_SetShake(-1, -1, 0xe666);
        wait(8);
        *motion = 3;
        wait(5);
        Actor_SetPositionAndResetMotion(partner, 0, 0, 0);
        wait(2);
        Sound_Play(188);
        Func_02002b04();
        wait(20);
        GameFlag_Set(second);
      }
    }
    Func_02003050();
    return;
  test:
    if (*table != 0xffff)
      goto next;
  }
}

#include "../../field/field_effect/advance_motion_priority.c"
