/// @file
/// The scene code of overlay 0x655, in ROM order.

#include "types.h"
#include "system.h"
#include "actor.h"
#include "event_inline.h"
#include "ram_buffer.h"
#include "field.h"
#include "map.h"
#include "event_work.h"
#include "module/field/event_commands.h"
#include "module/scene/dehkan_plateau.h"
#include "module/scene/scene.h"
#include "module/tla.h"

void Actor_DisableById(s32 id);

void Pillars_Place(const u16 *list) {
  const u16 *spec;
  u16 id;
  struct Actor *object;
  s16 mode;
  s16 flag;
  s32 limit;
  s32 first;
  limit = 0xff;
  first = list[0];
  if (first == ((limit << 8) + 0xff)) {
    return;
  }
  spec = list;
next:
  id = spec[0];
  object = Actor_GetById(id);
  mode = spec[1];
  flag = spec[2];
  object->collisionFlags &= 0xfe;
  Actor_SetSpriteFlags(object, 0);
  Actor_SetCellOpen(id, (u16)mode, (u16)flag, object->terrainId);
  limit = 0xff;
  spec += 3;
  first = spec[0];
  if (first != ((limit << 8) + 0xff)) {
    goto next;
  }
}

void Pillars_Pound(const void *data, s32 mode, s32 timer) {
  const u16 *spec = data;
  struct EffectWork1a *work = Ram_HeapSlots->effectWork;
  while (*spec != 0xffff) {
    u32 value = *spec;
    s32 active = work->active;

    if (active == value) {
      struct Actor *object = Actor_GetById(active);
      s32 kind;
      s32 flag;

      spec++;
      kind = spec[0];
      flag = spec[1];
      if (timer <= 7)
        object->scaleY += -0x1f00;
      if (mode == 1) {
        GameFlag_Set(flag);
        Actor_SetCellOpen(active, kind, flag, object->terrainId);
        object->scaleY = 0x10000;
      }
      return;
    }
    spec += 3;
  }
}

void Actor_SetCellOpen(s32 id, u32 mode, s32 flag, s32 layer) {
  struct Actor *object = Actor_GetById(id);
  struct MapCell *cell = Ram_HeapSlots->mapWork->layers[layer].cells;

  cell += Actor_GetCellIndex(object);
  if (GameFlag_Get(flag)) {
    /* FAKEMATCH(temporary): the flags read into a local before the cell store
     * and ORed into the register that holds 2, as the native does; as one |=,
     * which builds the 0 for the cell before the read, 8 of 75 instructions
     * differ in tla-en. */
    u8 flags = object->priorityFlags;
    s32 bits;

    cell->collisionCode = 0;
    bits = 2;
    bits |= flags;
    object->priorityFlags = bits;
    switch (mode) {
    case 0:
      Func_02003ba0(object, 2);
      break;
    case 1:
      Func_02003ba0(object, 4);
      break;
    case 2:
      Func_02003ba0(object, 6);
      break;
    }
  } else {
    cell->collisionCode = 255;
    switch (mode) {
    case 0:
      Func_02003ba0(object, 1);
      break;
    case 1:
      Func_02003ba0(object, 3);
      break;
    case 2:
      Func_02003ba0(object, 5);
      break;
    }
  }
}

#include "main_routine_work.h"
#include "field_sprite.h"
#include "iwram_call.h"

void CrumblingPillars_Place(const void *data) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  const u16 *list = data;
  struct Actor *leader;
  s32 height;

  if (list[0] != 0xffff) {
    do {
      s32 id = list[0];
      struct Actor *object = Actor_GetById(id);
      s32 flag = list[1];
      s32 nextFlag = flag + 1;
      struct MapCell *cells = map->layers[object->terrainId].cells;
      u8 *shapes =
          gMapShapeGrid + (cells - (struct MapCell *)Ram_MapCellBuffer);

      if (GameFlag_Get(nextFlag)) {
        Actor_SetHorizontalPositionWithTerrain(id, 0, 0);
      } else {
        s32 index = Actor_GetCellIndex(object);
        struct MapCell *cell = &cells[index];
        u8 *shapeCell = &shapes[index];
        s32 shape =
            MapGrid_GetTerrainKind(object->terrainId, object->x, object->z);
        s32 terrainHeight =
            Map_GetTerrainHeight(object->terrainId, object->x, object->z);

        Actor_DisableById(id);
        cell->attributeB = 128;
        *shapeCell = MapCollision_FindOrAddTag(object->terrainId, shape,
                                               (terrainHeight >> 19) + 4);
        if (GameFlag_Get(flag))
          Func_02003ba0(object, 0);
      }
      list += 2;
    } while (list[0] != 0xffff);
  }
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  height = Map_GetTerrainHeight(leader->terrainId, leader->x, leader->z);
  if (leader->y < height) {
    leader->terrainHeight = height;
    leader->y = height;
  }
}

void CrumblingPillars_Step(u16 *list) {
  struct Actor *leader;
  struct Actor *target;
  s32 position[3];

  if (Field_TryJumpForward() != 0)
    return;
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  position[0] = leader->x;
  position[1] = leader->y;
  position[2] = leader->z;
  target = Actor_FindAtCell(position, 0);
  if (target != NULL) {
    goto test;
  next:
    {
      s32 id = list[0];
      s32 flag;

      if (Actor_GetById(id) != target) {
        list += 2;
        goto test;
      }
      flag = list[1];
      Event_Start();
      if (GameFlag_Get(flag) == 0) {
        Sound_Play(125);
        Actor_SetSpriteColor(Actor_GetById(id), 7);
        wait(2);
        Func_02003ba0(target, 0);
        Actor_SetSpriteColor(Actor_GetById(id), 0);
        wait(2);
        Actor_SetSpriteColor(Actor_GetById(id), 7);
        wait(4);
        Actor_SetSpriteColor(Actor_GetById(id), 0);
        Rubble_Spray(0);
        GameFlag_Set(flag);
      } else {
        s32 done = flag + 1;
        s32 result = GameFlag_Get(done);

        if (result == 0) {
          leader->motionFlags = result;
          Sound_Play(185);
          MapGrid_SetShake(0x10000, 0x40000, 0x10000);
          Rubble_Spray(0);
          wait(8);
          Func_02003ba0(target, 2);
          Leader_SetCellShapeAt(-2, target->terrainId);
          Rubble_Spray(1);
          wait(16);
          Leader_SetCellShapeAt(-2, target->terrainId);
          wait(4);
          MapGrid_SetShake(-1, -1, 0xe666);
          wait(8);
          leader->motionFlags = 3;
          wait(5);
          Actor_SetPositionAndResetMotion(target, 0, 0, 0);
          wait(2);
          Sound_Play(188);
          Rubble_Burst();
          wait(20);
          GameFlag_Set(done);
        }
      }
      Event_End();
      return;
    }
  test:
    if (*list != 0xffff)
      goto next;
  }
}

/// Runs the event of the listed object `id` once: the list holds (object id,
/// flag) pairs ended by 0xffff, and unless the flag after the entry's own is
/// set, cues 185 and 188 play around Actor_SetCellShape and both flags are set.
/// The twin of overlay 67f's CrumblingPillar_Collapse.
void CrumblingPillar_Collapse(u16 *list, s32 id) {
  struct Actor *object = Actor_GetById(id);
  s32 flag;

  if (list[0] == 0xffff)
    return;
loop:
  if (list[0] != id) {
    list += 2;
    goto test;
  }
  flag = list[1];
  Event_Start();
  if (!GameFlag_Get(flag + 1)) {
    Sound_Play(185);
    MapGrid_SetShake(0x10000, 0x40000, 0x10000);
    Rubble_Spray(0);
    wait(8);
    Func_02003ba0(object, 2);
    Actor_SetCellShape(-4, object->terrainId, id);
    Rubble_Spray(1);
    wait(16);
    MapGrid_SetShake(-1, -1, 0xe666);
    wait(8);
    Actor_SetPositionAndResetMotion(object, 0, 0, 0);
    wait(2);
    Sound_Play(188);
    Rubble_Burst();
    wait(20);
    GameFlag_Set(flag);
    GameFlag_Set(flag + 1);
  }
  Event_End();
  return;
test:
  if (list[0] != 0xffff)
    goto loop;
}

extern const u8 gData_02003f44[];
extern const u8 gData_02003f52[];
extern const u8 gData_02003f80[];
extern const u8 gData_02003f92[];
extern const u8 gData_02003f98[];
extern const u8 gData_02003fa0[];
extern const u8 gData_02003fa8[];
extern const u8 gData_02003fb0[];
extern const u8 gData_02003fb8[];
extern const u8 gData_02003fc0[];
extern const u8 gData_02003fc4[];

void Func_02002180(void);
void Func_02001804(void);

extern const u16 gData_02003fd2[];
extern const struct CellRecord gData_02004066[];

void CrumblingPillar_Prepare(s32 id) {
  Actor_GetById(id)->unk5c[0] = 3;
  Actor_DisableById(id);
}

void Actor_MoveToObject(s32 id, s32 target);

/// Places actors 8 to 10 at the leader, then walks a list of (actor, flag)
/// pairs ended by 0xffff: an actor whose flag + 1 is set is put back with
/// Actor_SetHorizontalPositionWithTerrain; any other has the cell under it
/// marked and its shape grid entry set from the terrain there, and copies
/// attributes 9 when its flag is set. The twin of overlay 67f's
/// CrumblingPillars_PlaceWithStandIns.
void CrumblingPillars_PlaceWithStandIns(u16 *list) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  // Holds the grid's address for the whole function.
  u8 *grid = gMapShapeGrid;

  Actor_MoveToObject(8, gMainRoutineWork.currentOwner);
  Actor_MoveToObject(9, gMainRoutineWork.currentOwner);
  Actor_MoveToObject(10, gMainRoutineWork.currentOwner);
  wait(1);
  CrumblingPillar_Prepare(8);
  CrumblingPillar_Prepare(9);
  CrumblingPillar_Prepare(10);
  Actor_SetModeById(9, 0);
  wait(1);
  Actor_SetHorizontalPositionWithTerrain(8, 0, 0);
  Actor_SetHorizontalPositionWithTerrain(9, 0, 0);
  Actor_SetHorizontalPositionWithTerrain(10, 0, 0);
  wait(1);
  while (list[0] != 0xffff) {
    u32 id = list[0];
    struct Actor *actor = Actor_GetById(id);
    s32 flag = list[1];
    s32 next = flag + 1;
    u8 *terrain = &actor->terrainId;
    struct MapCell *cells = map->layers[*terrain].cells;
    u8 *shapes = grid + (cells - (struct MapCell *)Ram_MapCellBuffer);

    actor->unk5c[0] = 3;
    if (GameFlag_Get(next)) {
      Actor_SetHorizontalPositionWithTerrain(id, 0, 0);
    } else {
      s32 index = Actor_GetCellIndex(actor);
      struct MapCell *cell;
      u8 *slot;
      s32 shape, ground;

      cell = cells + index;
      slot = shapes + index;
      shape = MapGrid_GetTerrainKind(*terrain, actor->x, actor->z);
      ground = Map_GetTerrainHeight(*terrain, actor->x, actor->z);

      Actor_DisableById(id);
      cell->attributeB = 128;
      *slot = MapCollision_FindOrAddTag(actor->terrainId, shape,
                                        (ground >> 19) + 4);
      if (GameFlag_Get(flag))
        Actor_CopyGraphics(id, 9);
    }
    list += 2;
  }
  wait(10);
}

void Actor_TakePlaceOf(struct Actor *object, struct Actor *target) {
  Actor_SetPositionAndResetMotion(object, target->x, target->y, target->z);
  Actor_SetPositionAndResetMotion(target, 0, 0, 0);
}

void Actor_StartScript(struct Actor *object, const void *script);

void CrumblingPillars_StepWithStandIn(u16 *list) {
  struct Actor *leader;
  struct Actor *helper;
  struct Actor *target;
  s32 position[3];

  if (Field_TryJumpForward() != 0)
    return;
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  helper = Actor_GetById(8);
  position[0] = leader->x;
  position[1] = leader->y;
  position[2] = leader->z;
  target = Actor_FindAtCell(position, 0);
  if (target != NULL) {
    goto test;
  next:
    {
      s32 id = list[0];
      s32 flag;

      if (Actor_GetById(id) != target) {
        list += 2;
        goto test;
      }
      flag = list[1];
      Event_Start();
      if (GameFlag_Get(flag) == 0) {
        Func_02003ba0(helper, 1);
        Actor_TakePlaceOf(helper, target);
        wait(1);
        Sound_Play(125);
        Actor_SetSpriteColor(Actor_GetById(8), 7);
        wait(2);
        Func_02003ba0(helper, 0);
        Actor_CopyGraphics(id, 9);
        Actor_SetSpriteColor(Actor_GetById(8), 0);
        wait(2);
        Actor_SetSpriteColor(Actor_GetById(8), 7);
        wait(4);
        Actor_SetSpriteColor(Actor_GetById(8), 0);
        Rubble_Spray(0);
        Actor_TakePlaceOf(target, helper);
        wait(1);
        GameFlag_Set(flag);
      } else {
        s32 done = flag + 1;
        s32 result = GameFlag_Get(done);

        if (result == 0) {
          Func_02003ba0(helper, 0);
          Actor_TakePlaceOf(helper, target);
          wait(1);
          leader->motionFlags = result;
          Sound_Play(185);
          MapGrid_SetShake(0x10000, 0x40000, 0x10000);
          Rubble_Spray(0);
          wait(8);
          Func_02003ba0(helper, 2);
          Leader_SetCellShapeAt(-2, target->terrainId);
          Rubble_Spray(1);
          wait(16);
          Leader_SetCellShapeAt(-2, target->terrainId);
          wait(4);
          MapGrid_SetShake(-1, -1, 0xe666);
          wait(8);
          leader->motionFlags = 3;
          wait(5);
          Actor_SetPositionAndResetMotion(helper, 0, 0, 0);
          wait(2);
          Sound_Play(188);
          Rubble_Burst();
          wait(20);
          GameFlag_Set(done);
        }
      }
      Event_End();
      return;
    }
  test:
    if (*list != 0xffff)
      goto next;
  }
}

#include "../field/field_effect/advance_motion_priority.c"

// ---- scene/dehkan_plateau/s0974.c ----

/// Scene code of overlay 0x655, in ROM order.

#include "types.h"
#include "system.h"
#include "iwram_call.h"
#include "actor.h"
#include "main_routine_work.h"
#include "field_sprite.h"
#include "field_effect.h"
#include "event_inline.h"
#include "ram_buffer.h"
#include "map.h"
#include "event_work.h"
#include "module/field/field_effect.h"
#include "module/scene/dehkan_plateau.h"
#include "module/scene/scene.h"
#include "module/tla.h"

extern const s32 gData_02003e00[];

void Actor_SetSpriteFlags(struct FieldEffect *effect, s32 flags);
void Actor_StartScript(struct FieldEffect *effect, const s32 *script);

/// Eight puffs (type 285) in a row over the leader, flung outwards when spread
/// is set.
void Rubble_Spray(s32 spread) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct FieldEffect *effect;
  u32 frame = 0;
  u32 i = 0;

  do {
    {
      /* FAKEMATCH(temporary): y held in a local, so x is computed before it; as
       * one expression, 14 of 122 instructions differ in tla-en. */
      s32 y = leader->y - (((Random16() * 14) >> 16) << 16);

      effect = (struct FieldEffect *)Import_ActorCreate(
          285, (i << 17) + leader->x - 0x80000, y - 0x20000, leader->z);
    }
    if (effect != 0) {
      frame = Sprite_ShareVramCacheEntry(effect->sprite, frame);
      effect->motionFlags = 0;
      effect->collisionFlags = 0;
      Actor_SetSpriteFlags(effect, 0);
      Func_02003ba0(effect, 2);
      Actor_StartScript(effect, gData_02003e00);
      effect->scaleY = effect->scaleX = 0xb333;
      effect->sprite->priority = 1;
      if (spread != 0) {
        effect->velocity.x = Iwram_MulQ16(Trig_Cos(i << 13), 0x13333);
        effect->velocity.z = Trig_Sin(i << 13);
      } else {
        effect->velocity.x = spread;
        effect->velocity.z = spread;
      }
      effect->velocity.y = -(((Random16() << 14) >> 16) + 0x6000);
      effect->spin = ((Random16() << 9) >> 16) - 0x100;
      effect->scaleRateX = -0x800;
      effect->scaleRateY = -0x600;
      effect->update = Effect_AdvanceMotionAtLeaderPriority;
    }
    i++;
  } while (i <= 7);
}

void Actor_TakePlaceOf(struct Actor *object, struct Actor *other);

void Actor_MoveToObject(s32 id, s32 target);

extern const u8 gData_02003e30[];

void Rubble_Burst(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 frame;
  u32 i;

  if (leader == NULL)
    return;
  frame = 0;
  for (i = 0; i <= 16; i++) {
    {
      struct Actor *object = ((struct Actor *)Import_ActorCreate(
          285, leader->x, leader->y, leader->z));
      struct EffectView *effect = (struct EffectView *)object;

      if (object == NULL)
        goto skip;
      frame = Sprite_ShareVramCacheEntry(effect->sprite, frame);
      object->motionFlags = 0;
      object->collisionFlags = 0;
      Actor_SetSpriteFlags(object, 0);
      Func_02003ba0(object, 2);
      Actor_StartScript(object, gData_02003e30);
      effect->velocityX = Trig_Cos(i << 12);
      effect->velocityY = 0;
      effect->velocityZ = Trig_Sin(i << 12);
      effect->velocityX += effect->velocityX >> 1;
      effect->velocityX += (((Random16() * 3) << 14) >> 16) + -0x6000;
      effect->velocityZ += (((Random16() * 3) << 13) >> 16) + -0x3000;
      effect->spin = (((u32)Random16() << 12) >> 16) - 0x800;
      effect->scaleRateX = 0;
      effect->scaleRateY = 0;
      effect->update = Effect_AdvanceMotionAtLeaderPriority;
      effect->sprite->priority =
          ((struct EffectView *)leader)->sprite->priority;
    }
  skip:
  }
}

// ---- scene/dehkan_plateau/s0bc0.c ----

/// Scene code of overlay 0x655, in ROM order.

#include "shared/map_cells.h"
#include "../../../../games/tla/src/scene/tundaria_tower/tundaria_tower.h"
#include "map.h"
#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_inline.h"
#include "field_sprite.h"
#include "iwram_call.h"
#include "field.h"
#include "event_work.h"
#include "module/field/event_commands.h"
#include "module/field/field.h"
#include "module/scene/dehkan_plateau.h"
#include "module/scene/scene.h"
#include "module/tla.h"

DEFINE_ACTOR_SET_CELL_SHAPE(Actor_SetCellShape, MapGrid_GetTerrainKind,
                            MapCollision_FindOrAddTag)

extern const u8 gData_02003f44[];
extern const u8 gData_02003f52[];
extern const u8 gData_02003f80[];
extern const u8 gData_02003f92[];
extern const u8 gData_02003f98[];
extern const u8 gData_02003fa0[];
extern const u8 gData_02003fa8[];
extern const u8 gData_02003fb0[];
extern const u8 gData_02003fb8[];
extern const u8 gData_02003fc0[];
extern const u8 gData_02003fc4[];

void CrumblingPillars_Step(const void *data);

void Func_02002180(void);
void Func_02001804(void);

extern const u16 gData_02003fd2[];
extern const struct CellRecord gData_02004066[];

void Leader_SetCellShapeAt(s32 a, s32 b) {
  Actor_SetCellShape(a, b, gMainRoutineWork.currentOwner);
}

void Actor_TakePlaceOf(struct Actor *object, struct Actor *other);
void Actor_StartScript(struct Actor *object, const void *script);
void Actor_MoveToObject(s32 id, s32 target);

// Raises each listed actor's cells: the shape grid and the cell flags under it,
// then lifts the leader if the new terrain is above the leader.
DEFINE_PILLARS_RAISE(Pillars_Raise, MapGrid_GetTerrainKind, Actor_GetCellIndex,
                     MapCollision_FindOrAddTag)

#define HELD_OBJECT(work) (*(struct Actor **)((u8 *)(work) + 0xcbc))

void Func_02000d78(u16 *list) {
  struct Actor *held = Leader_FindActorAhead();
  void *work = Ram_HeapSlots->eventWork;
  u8 *shapeGrid = gMapShapeGrid;

  if (held == NULL) {
    s32 id = Field_FindNearbyPsynergyTarget(gMainRoutineWork.currentOwner, 1);

    if (id != -1)
      held = Actor_GetById(id);
  }
  HELD_OBJECT(work) = NULL;
  if (*list != 0xffff) {
  loop:
    if (held == Actor_GetById(*list)) {
      u8 *layer = &held->terrainId;
      struct MapCell *cells = Ram_HeapSlots->mapWork->layers[*layer].cells;
      u8 *shapes = shapeGrid + (cells - (struct MapCell *)Ram_MapCellBuffer);
      s32 shape = MapGrid_GetTerrainKind(*layer, held->x, held->z);
      s32 level = Map_GetTerrainHeight(*layer, held->x, held->z);
      s32 index = Actor_GetCellIndex(held);
      s32 value;

      shapes += index;
      value = MapCollision_FindOrAddTag(*layer, shape, (level >> 19) - 4);
      cells += index;
      /* FAKEMATCH(signature): the shape grid byte stored through a bit-field
         view (struct ShapeBits), whose alias set lets the cell's attribute be
         read first; as `*shapes = value;`, 4 of 131 instructions differ in
         tla-en. */
      ((struct ShapeBits *)shapes)->shape = value;
      cells->attributeB &= 63;
      cells += 128;
      cells->attributeB &= 191;
      HELD_OBJECT(work) = held;
    }
    list++;
    if (*list != 0xffff)
      goto loop;
  }
}

// One-field view: the shape store is memory of its own to sched2, which sinks
// it past the cell read; a plain store keeps its order.

DEFINE_HELD_ACTOR_RAISE_CELL(HeldActor_RaiseCell, MapGrid_GetTerrainKind,
                             Actor_GetCellIndex, MapCollision_FindOrAddTag)

#include "../field/object/set_sprite_priority.c"
