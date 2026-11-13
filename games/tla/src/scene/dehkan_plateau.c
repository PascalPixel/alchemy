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

/* VIEW(first): routine gData_02003f44 as this part declared it; the joined file
 * has another. */
extern const u8 gData_02003f44__first[] __asm__("gData_02003f44");
#define gData_02003f44 gData_02003f44__first
/* VIEW(first): routine gData_02003f52 as this part declared it; the joined file
 * has another. */
extern const u8 gData_02003f52__first[] __asm__("gData_02003f52");
#define gData_02003f52 gData_02003f52__first
/* VIEW(first): routine gData_02003f80 as this part declared it; the joined file
 * has another. */
extern const u8 gData_02003f80__first[] __asm__("gData_02003f80");
#define gData_02003f80 gData_02003f80__first
/* VIEW(first): routine gData_02003f92 as this part declared it; the joined file
 * has another. */
extern const u8 gData_02003f92__first[] __asm__("gData_02003f92");
#define gData_02003f92 gData_02003f92__first
/* VIEW(first): routine gData_02003f98 as this part declared it; the joined file
 * has another. */
extern const u8 gData_02003f98__first[] __asm__("gData_02003f98");
#define gData_02003f98 gData_02003f98__first
/* VIEW(first): routine gData_02003fa0 as this part declared it; the joined file
 * has another. */
extern const u8 gData_02003fa0__first[] __asm__("gData_02003fa0");
#define gData_02003fa0 gData_02003fa0__first
/* VIEW(first): routine gData_02003fa8 as this part declared it; the joined file
 * has another. */
extern const u8 gData_02003fa8__first[] __asm__("gData_02003fa8");
#define gData_02003fa8 gData_02003fa8__first
/* VIEW(first): routine gData_02003fb0 as this part declared it; the joined file
 * has another. */
extern const u8 gData_02003fb0__first[] __asm__("gData_02003fb0");
#define gData_02003fb0 gData_02003fb0__first
/* VIEW(first): routine gData_02003fb8 as this part declared it; the joined file
 * has another. */
extern const u8 gData_02003fb8__first[] __asm__("gData_02003fb8");
#define gData_02003fb8 gData_02003fb8__first
/* VIEW(first): routine gData_02003fc0 as this part declared it; the joined file
 * has another. */
extern const u8 gData_02003fc0__first[] __asm__("gData_02003fc0");
#define gData_02003fc0 gData_02003fc0__first
/* VIEW(first): routine gData_02003fc4 as this part declared it; the joined file
 * has another. */
extern const u8 gData_02003fc4__first[] __asm__("gData_02003fc4");
#define gData_02003fc4 gData_02003fc4__first

void Func_02002180(void);
void Func_02001804(void);

/* VIEW(first): routine gData_02003fd2 as this part declared it; the joined file
 * has another. */
extern const u16 gData_02003fd2__first[] __asm__("gData_02003fd2");
#define gData_02003fd2 gData_02003fd2__first
/* VIEW(first): routine gData_02004066 as this part declared it; the joined file
 * has another. */
extern const struct CellRecord
    gData_02004066__first[] __asm__("gData_02004066");
#define gData_02004066 gData_02004066__first

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

#include "field_effect.h"
#include "module/field/field_effect.h"

/* VIEW(first): routine gData_02003e00 as this part declared it; the joined file
 * has another. */
extern const s32 gData_02003e00__first[] __asm__("gData_02003e00");
#define gData_02003e00 gData_02003e00__first

void Actor_SetSpriteFlags(struct Actor *object, u32 value);

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

/* VIEW(first): routine gData_02003e30 as this part declared it; the joined file
 * has another. */
extern const u8 gData_02003e30__first[] __asm__("gData_02003e30");
#define gData_02003e30 gData_02003e30__first

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

#include "shared/map_cells.h"
#include "../../games/tla/src/scene/tundaria_tower/tundaria_tower.h"
#include "module/field/field.h"

DEFINE_ACTOR_SET_CELL_SHAPE(Actor_SetCellShape, MapGrid_GetTerrainKind,
                            MapCollision_FindOrAddTag)

void Leader_SetCellShapeAt(s32 a, s32 b) {
  Actor_SetCellShape(a, b, gMainRoutineWork.currentOwner);
}

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

#include "../field/field_effect/move.c"

#undef HELD_OBJECT
#undef gData_02003f44
#undef gData_02003f52
#undef gData_02003f80
#undef gData_02003f92
#undef gData_02003f98
#undef gData_02003fa0
#undef gData_02003fa8
#undef gData_02003fb0
#undef gData_02003fb8
#undef gData_02003fc0
#undef gData_02003fc4
#undef gData_02003fd2
#undef gData_02004066
#undef gData_02003e00
#undef gData_02003e30
// ---- from scene/dehkan_plateau/dehkan_plateau.c ----
// @file
// Scene code of overlay 0x655, in ROM order.

#define EFFECT_SPAWN FieldEffect_Spawn
#define EFFECT_CREATE Import_ActorCreate
#define EFFECT_SET_MODE Func_02003ba0
#define EFFECT_SET_SCRIPT Actor_StartScript
#define EFFECT_SCRIPTS gFieldEffect_DurationScripts
#include "../field/field_effect/spawn.c"

#undef EFFECT_SET_SCRIPT

// ---- scene/dehkan_plateau/dehkan_plateau.c ----

// Scene code of overlay 0x655, in ROM order.

// ---- games/tla/src/scene/dehkan_plateau.c ----

#include "flags.h"
#include "shared/map_cells.h"
#include "shared/actors.h"
#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_inline.h"
#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "scene_tables.h"
#include "scene_ids.h"
#include "map.h"
#include "field_sprite.h"
#include "iwram_call.h"
#include "event_work.h"
#include "event.h"
#include "flag_call.h"
#include "module/field/event_commands.h"
#include "module/scene/dehkan_plateau.h"
#include "module/scene/scene.h"
#include "module/tla.h"
#include "scene_call.h"

void Actor_DisableById(s32 id);

/* VIEW(dehkan_plateau): routine gData_02003fd2 as this part declared it; the
 * joined file has another. */
extern const u16 gData_02003fd2__dehkan_plateau[] __asm__("gData_02003fd2");
#define gData_02003fd2 gData_02003fd2__dehkan_plateau
/* VIEW(dehkan_plateau): routine gData_02004066 as this part declared it; the
 * joined file has another. */
extern const struct CellRecord
    gData_02004066__dehkan_plateau[] __asm__("gData_02004066");
#define gData_02004066 gData_02004066__dehkan_plateau

/* VIEW(dehkan_plateau): routine gData_02003f44 as this part declared it; the
 * joined file has another. */
extern const u8 gData_02003f44__dehkan_plateau[] __asm__("gData_02003f44");
#define gData_02003f44 gData_02003f44__dehkan_plateau
/* VIEW(dehkan_plateau): routine gData_02003f52 as this part declared it; the
 * joined file has another. */
extern const u8 gData_02003f52__dehkan_plateau[] __asm__("gData_02003f52");
#define gData_02003f52 gData_02003f52__dehkan_plateau
/* VIEW(dehkan_plateau): routine gData_02003f80 as this part declared it; the
 * joined file has another. */
extern const u8 gData_02003f80__dehkan_plateau[] __asm__("gData_02003f80");
#define gData_02003f80 gData_02003f80__dehkan_plateau
/* VIEW(dehkan_plateau): routine gData_02003f92 as this part declared it; the
 * joined file has another. */
extern const u8 gData_02003f92__dehkan_plateau[] __asm__("gData_02003f92");
#define gData_02003f92 gData_02003f92__dehkan_plateau
/* VIEW(dehkan_plateau): routine gData_02003f98 as this part declared it; the
 * joined file has another. */
extern const u8 gData_02003f98__dehkan_plateau[] __asm__("gData_02003f98");
#define gData_02003f98 gData_02003f98__dehkan_plateau
/* VIEW(dehkan_plateau): routine gData_02003fa0 as this part declared it; the
 * joined file has another. */
extern const u8 gData_02003fa0__dehkan_plateau[] __asm__("gData_02003fa0");
#define gData_02003fa0 gData_02003fa0__dehkan_plateau
/* VIEW(dehkan_plateau): routine gData_02003fa8 as this part declared it; the
 * joined file has another. */
extern const u8 gData_02003fa8__dehkan_plateau[] __asm__("gData_02003fa8");
#define gData_02003fa8 gData_02003fa8__dehkan_plateau
/* VIEW(dehkan_plateau): routine gData_02003fb0 as this part declared it; the
 * joined file has another. */
extern const u8 gData_02003fb0__dehkan_plateau[] __asm__("gData_02003fb0");
#define gData_02003fb0 gData_02003fb0__dehkan_plateau
/* VIEW(dehkan_plateau): routine gData_02003fb8 as this part declared it; the
 * joined file has another. */
extern const u8 gData_02003fb8__dehkan_plateau[] __asm__("gData_02003fb8");
#define gData_02003fb8 gData_02003fb8__dehkan_plateau
/* VIEW(dehkan_plateau): routine gData_02003fc0 as this part declared it; the
 * joined file has another. */
extern const u8 gData_02003fc0__dehkan_plateau[] __asm__("gData_02003fc0");
#define gData_02003fc0 gData_02003fc0__dehkan_plateau
/* VIEW(dehkan_plateau): routine gData_02003fc4 as this part declared it; the
 * joined file has another. */
extern const u8 gData_02003fc4__dehkan_plateau[] __asm__("gData_02003fc4");
#define gData_02003fc4 gData_02003fc4__dehkan_plateau

void Func_0200118c(void) {
  GameFlag_Set(0x209);
  Field_RequestDjinnBattle(16, 47);
}

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

/* VIEW(dehkan_plateau): routine gData_020045e0 as this part declared it; the
 * joined file has another. */
extern const u8 gData_020045e0__dehkan_plateau[] __asm__("gData_020045e0");
#define gData_020045e0 gData_020045e0__dehkan_plateau
/* VIEW(dehkan_plateau): routine gData_020045f8 as this part declared it; the
 * joined file has another. */
extern const u8 gData_020045f8__dehkan_plateau[] __asm__("gData_020045f8");
#define gData_020045f8 gData_020045f8__dehkan_plateau
/* VIEW(dehkan_plateau): routine gData_020047d8 as this part declared it; the
 * joined file has another. */
extern const u8 gData_020047d8__dehkan_plateau[] __asm__("gData_020047d8");
#define gData_020047d8 gData_020047d8__dehkan_plateau
/* VIEW(dehkan_plateau): routine gData_02004ac0 as this part declared it; the
 * joined file has another. */
extern const u8 gData_02004ac0__dehkan_plateau[] __asm__("gData_02004ac0");
#define gData_02004ac0 gData_02004ac0__dehkan_plateau
/* VIEW(dehkan_plateau): routine gData_02004d78 as this part declared it; the
 * joined file has another. */
extern const u8 gData_02004d78__dehkan_plateau[] __asm__("gData_02004d78");
#define gData_02004d78 gData_02004d78__dehkan_plateau
/* VIEW(dehkan_plateau): routine gData_02004dd8 as this part declared it; the
 * joined file has another. */
extern const u8 gData_02004dd8__dehkan_plateau[] __asm__("gData_02004dd8");
#define gData_02004dd8 gData_02004dd8__dehkan_plateau
/* VIEW(dehkan_plateau): routine gData_02004e20 as this part declared it; the
 * joined file has another. */
extern const u8 gData_02004e20__dehkan_plateau[] __asm__("gData_02004e20");
#define gData_02004e20 gData_02004e20__dehkan_plateau
/* VIEW(dehkan_plateau): routine gData_02004f10 as this part declared it; the
 * joined file has another. */
extern const u8 gData_02004f10__dehkan_plateau[] __asm__("gData_02004f10");
#define gData_02004f10 gData_02004f10__dehkan_plateau
/* VIEW(dehkan_plateau): routine gData_02004f1c as this part declared it; the
 * joined file has another. */
extern const u8 gData_02004f1c__dehkan_plateau[] __asm__("gData_02004f1c");
#define gData_02004f1c gData_02004f1c__dehkan_plateau
/* VIEW(dehkan_plateau): routine gData_02005078 as this part declared it; the
 * joined file has another. */
extern const u8 gData_02005078__dehkan_plateau[] __asm__("gData_02005078");
#define gData_02005078 gData_02005078__dehkan_plateau
/* VIEW(dehkan_plateau): routine gData_020051bc as this part declared it; the
 * joined file has another. */
extern const u8 gData_020051bc__dehkan_plateau[] __asm__("gData_020051bc");
#define gData_020051bc gData_020051bc__dehkan_plateau
/* VIEW(dehkan_plateau): routine gData_020053b4 as this part declared it; the
 * joined file has another. */
extern const u8 gData_020053b4__dehkan_plateau[] __asm__("gData_020053b4");
#define gData_020053b4 gData_020053b4__dehkan_plateau
/* VIEW(dehkan_plateau): routine gData_02005450 as this part declared it; the
 * joined file has another. */
extern const u8 gData_02005450__dehkan_plateau[] __asm__("gData_02005450");
#define gData_02005450 gData_02005450__dehkan_plateau
/* VIEW(dehkan_plateau): routine gData_020055b8 as this part declared it; the
 * joined file has another. */
extern const u8 gData_020055b8__dehkan_plateau[] __asm__("gData_020055b8");
#define gData_020055b8 gData_020055b8__dehkan_plateau

const u8 *Scene_GetPlacements(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov655Room1)
    return gData_020045f8;
  if (scene == (s32)&SceneId_Ov655Room2)
    return gData_020047d8;
  if (scene == (s32)&SceneId_Ov655Room3)
    return gData_02004ac0;
  if (scene == (s32)&SceneId_Ov655Room4)
    return gData_02004d78;
  if (scene == (s32)&SceneId_Ov655Room5)
    return gData_02004dd8;
  if (scene == (s32)&SceneId_Ov655Room6)
    return gData_02004e20;
  return gData_020045e0;
}

const u8 *Scene_GetEvents(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov655Room1)
    return gData_02004f1c;
  if (scene == (s32)&SceneId_Ov655Room2)
    return gData_02005078;
  if (scene == (s32)&SceneId_Ov655Room3)
    return gData_020051bc;
  if (scene == (s32)&SceneId_Ov655Room4)
    return gData_020053b4;
  if (scene == (s32)&SceneId_Ov655Room5)
    return gData_02005450;
  if (scene == (s32)&SceneId_Ov655Room6)
    return gData_020055b8;
  return gData_02004f10;
}

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ void Map_CopyAttributes(s32 a, s32 b, s32 c, s32 d, s32 e,
                                          s32 f) {
  Map_CopyCellAttributeRect(a, b, c, d, e, f);
}

void Func_020012c8(void) { CrumblingPillars_Step(gData_02003f44); }

void Func_020012d8(void) {
  if (GameFlag_Get(0x213) == 0)
    Map_CopyAttributes(0, 0, 1, 1, 9, 18);
  if (GameFlag_Get(0x215) == 0)
    Map_CopyAttributes(0, 0, 1, 1, 11, 18);
  if (GameFlag_Get(0x221) == 0)
    Map_CopyAttributes(0, 0, 1, 1, 14, 28);
  if (GameFlag_Get(0x223) == 0)
    Map_CopyAttributes(0, 0, 1, 1, 16, 28);
}

void Func_02001368(void) {
  s32 y;

  CrumblingPillars_StepWithStandIn(gData_02003f52);
  Func_020012d8();
  y = Actor_GetById(gMainRoutineWork.currentOwner)->y;
  if (y < 0x80000) {
    Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 2);
    Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 1;
  }
}

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.

void Func_020013b0(void) {
  if (GameFlag_Get(0x213) == 0)
    Map_CopyAttributes(63, 0, 1, 1, 50, 20);
  if (GameFlag_Get(0x215) == 0)
    Map_CopyAttributes(63, 0, 1, 1, 52, 20);
}

void Func_020013fc(void) {
  CrumblingPillars_Step(gData_02003f80);
  Func_020013b0();
  if (Actor_GetById(gMainRoutineWork.currentOwner)->y < -0x180000) {
    Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 2);
    Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 1;
  }
}

void Func_02001444(void) {
  s32 y;

  CrumblingPillars_Step(gData_02003f92);
  y = Actor_GetById(gMainRoutineWork.currentOwner)->y;
  if (y < 0x180000)
    Func_02002180();
}

void Actors_SetHidden(s32 show, s32 first, u32 count);

void Func_02001474(s32 show) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov655Room1)
    Actors_SetHidden(show, 13, 14);
  else if (scene == (s32)&SceneId_Ov655Room2)
    Actors_SetHidden(show, 25, 13);
  else if (scene == (s32)&SceneId_Ov655Room3)
    Actors_SetHidden(show, 26, 10);
  else if (scene == (s32)&SceneId_Ov655Room4)
    Actors_SetHidden(show, 8, 3);
  else if (scene == (s32)&SceneId_Ov655Room6)
    Actors_SetHidden(show, 10, 6);
}

void Func_020014ec(void) {
  Func_02001474(1);
  GameFlag_Set(0x200);
}

void Func_02001500(void) {
  Func_02001474(0);
  GameFlag_Clear(0x200);
}

DEFINE_ACTORS_SET_HIDDEN(Actors_SetHidden)

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.

void Func_02001554(s32 a, s32 b) { Pillars_Pound(gData_02003f98, a, b); }

void Func_02001568(s32 a, s32 b) { Pillars_Pound(gData_02003fa0, a, b); }

void Func_0200157c(s32 a, s32 b) { Pillars_Pound(gData_02003fa8, a, b); }

void Func_02001590(s32 a, s32 b) { Pillars_Pound(gData_02003fb0, a, b); }

void Func_020015a4(s32 a, s32 b) { Pillars_Pound(gData_02003fb8, a, b); }

DEFINE_ACTOR_DROP_AND_WAIT(Actor_DropAndWait)

void Func_020015e0(void) {
  struct Actor *object = Actor_GetById(12);
  s32 x;

  x = object->x >> 20;
  if (x == 56 && (object->z >> 20) == 32) {
    Event_Start();
    Event_ResolvePendingActions(0);
    Actor_DropAndWait(object);
    Map_CopyCellAttributeRect(54, 32, 2, 1, x, 64);
    GameFlag_Set(0x301);
    Event_End();
  }
}

void Func_02001630(void) { Actor_LiftTargetBlock(gData_02003fc0); }

void Func_02001640(void) {
  struct Actor *object = Actor_GetById(23);
  s32 x = object->x >> 20;

  Actor_PlaceMovedBlock();
  if (x == 52) {
    Event_Start();
    Event_ResolvePendingActions(0);
    Map_CopyCellAttributeRect(51, 7, 1, 1, x, 7);
    wait(1);
    Actor_DropAndWait(object);
    Map_CopyCellAttributeRect(50, 7, 1, 1, x, 7);
    GameFlag_Set(0x840);
    Event_End();
  }
}

void Func_020016a0(void) { Actor_LiftTargetBlock(gData_02003fc4); }

void Func_020016b0(void) {
  struct Actor *object = Actor_GetById(19);
  s32 x = object->x >> 20;

  Actor_PlaceMovedBlock();
  if (x == 47) {
    Event_Start();
    Event_ResolvePendingActions(0);
    Map_CopyCellAttributeRect(47, 12, 1, 1, x, 13);
    wait(1);
    Actor_DropAndWait(object);
    Map_CopyCellAttributeRect(50, 16, 1, 1, x, 13);
    Actor_SetSpritePriority(19, 2);
    Event_End();
  } else if (x == 46) {
    Map_CopyAttributes(44, 13, 1, 1, 48, 13);
  }
}

static __inline__ void Actor_PlaceOnTerrain(s32 id, s32 x, s32 z) {
  Actor_SetHorizontalPositionWithTerrain(id, x, z);
}

void Func_0200172c(void) {
  Actor_PlaceOnTerrain(20, 150 << 18, 134 << 18);
  Map_CopyAttributes(38, 32, 1, 1, 37, 33);
  Map_CopyAttributes(30, 32, 1, 2, 38, 32);
  Map_CopyAttributes(40, 64, 4, 3, 36, 64);
  GameFlag_Set(0x841);
}

void Func_02001788(void) {
  if ((Actor_GetById(20)->x >> 20) == 37)
    Func_0200172c();
}

void Func_020017a0(void) {
  u8 *flags;

  if ((Actor_GetById(20)->x >> 20) == 38) {
    flags = &Actor_GetById(gMainRoutineWork.currentOwner)->motionFlags;
    Event_Start();
    Event_ResolvePendingActions(0);
    *flags = 0;
    wait(1);
    Leader_PushObjectAhead();
    Func_0200172c();
    wait(1);
    *flags = 3;
    Event_End();
  }
}

void Func_020017f4(void) { GameFlag_Clear(0x201); }

void Func_02001804(void) {
  s32 flag;

  flag = 0x320;
loop:
  GameFlag_Clear(flag);
  if (flag != 0x35f) {
    flag++;
    goto loop;
  }
}

#define DEFINE_DEHKAN_PLATEAU_RESTORE_CELLS(name, data1, n1, data2)            \
  void name(void) {                                                            \
    const u16 *cell = &data1[0].present;                                       \
    s32 flag = 0x240;                                                          \
    u32 i;                                                                     \
    const u16 *dest;                                                           \
                                                                               \
    for (i = 0; i <= n1; i++) {                                                \
      if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) != 0) {                       \
        if (GameFlag_Get(flag) != 0) {                                         \
          s32 x;                                                               \
          s32 y;                                                               \
                                                                               \
          cell = &data1[i].x;                                                  \
          x = *cell++;                                                         \
          y = *cell++;                                                         \
          /* FAKEMATCH(temporary): the destination table through a local set   \
           * here, which loads its first halfword before the 1 is built; as    \
           * gData_02003fd2[0], 2 of 56 instructions differ in tla-en. */      \
          dest = data2;                                                        \
          MapGrid_CopyCellPictureIndicesRect(dest[0], dest[1], x, y, 1, 1);    \
        }                                                                      \
      } else {                                                                 \
        if (*cell != 0)                                                        \
          GameFlag_Set(flag);                                                  \
        cell += 6;                                                             \
      }                                                                        \
      flag++;                                                                  \
    }                                                                          \
    if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0)                           \
      Func_02001804();                                                         \
  }

DEFINE_DEHKAN_PLATEAU_RESTORE_CELLS(Func_02001820, gData_02004066, 13,
                                    gData_02003fd2)

/* VIEW(dehkan_plateau): routine gData_02003fda as this part declared it; the
 * joined file has another. */
extern const u16 gData_02003fda__dehkan_plateau[] __asm__("gData_02003fda");
#define gData_02003fda gData_02003fda__dehkan_plateau
/* VIEW(dehkan_plateau): routine gData_0200410e as this part declared it; the
 * joined file has another. */
extern const struct CellRecord
    gData_0200410e__dehkan_plateau[] __asm__("gData_0200410e");
#define gData_0200410e gData_0200410e__dehkan_plateau

DEFINE_DEHKAN_PLATEAU_RESTORE_CELLS(Func_020018a8, gData_0200410e, 7,
                                    gData_02003fda)

/* VIEW(dehkan_plateau): routine gData_02003fe2 as this part declared it; the
 * joined file has another. */
extern const u16 gData_02003fe2__dehkan_plateau[] __asm__("gData_02003fe2");
#define gData_02003fe2 gData_02003fe2__dehkan_plateau
/* VIEW(dehkan_plateau): routine gData_0200416e as this part declared it; the
 * joined file has another. */
extern const struct CellRecord
    gData_0200416e__dehkan_plateau[] __asm__("gData_0200416e");
#define gData_0200416e gData_0200416e__dehkan_plateau

DEFINE_DEHKAN_PLATEAU_RESTORE_CELLS(Func_02001930, gData_0200416e, 5,
                                    gData_02003fe2)

void Func_020019b8(void) {
  u16 *record = gData_020041b6;
  s32 flag = 0x240;
  u32 i;

  for (i = 0; i <= 15; flag++, i++) {
    if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
      if (GameFlag_Get(flag)) {
        s32 x;
        s32 y;

        record = &gData_020041b8[i * 6];
        x = *record++;
        y = *record++;
        MapGrid_CopyCellPictureIndicesRect(gData_02003fea[0], gData_02003fea[1],
                                           x, y, 1, 1);
      }
    } else {
      if (*record != 0)
        GameFlag_Set(flag);
      record += 6;
    }
  }
}

void Actor_TakePlaceOf(struct Actor *object, struct Actor *other);
void Actor_MoveToObject(s32 id, s32 target);

/* VIEW(dehkan_plateau): routine gData_02004420 as this part declared it; the
 * joined file has another. */
extern struct Actor *gData_02004420__dehkan_plateau __asm__("gData_02004420");
#define gData_02004420 gData_02004420__dehkan_plateau
extern s32 gData_02005678;

#define SCENE(name) ((s32)&SceneId_##name)

// Steps onto a listed cell in rooms 1, 2, 3 and 5 (SceneId_Ov655Room1 to
// Room5): either falls through to exit work->exit or opens the cell and sets
// its flag. cellX is reused for the doubled index and the fall's frame count.
void Func_02001a30(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 cellX = ((leader->x >> 20) << 1) + 1;
  s32 cellZ = ((leader->z >> 20) << 1) + 1;
  s32 n, flag;
  u16 *record, *next;

  if (gMainRoutineWork.scene == SCENE(Ov655Room1)) {
    n = work->exit - 11;
    flag = work->exit + 0x235;
    record = &gData_02004068[n * 6];
  } else if (gMainRoutineWork.scene == SCENE(Ov655Room2)) {
    n = work->exit - 14;
    flag = work->exit + 0x232;
    record = &gData_02004110[n * 6];
  } else if (gMainRoutineWork.scene == SCENE(Ov655Room3)) {
    n = work->exit - 12;
    flag = work->exit + 0x234;
    record = &gData_02004170[n * 6];
  } else if (gMainRoutineWork.scene == SCENE(Ov655Room5)) {
    n = work->exit - 18;
    flag = work->exit + 0x22e;
    record = &gData_020041b8[n * 6];
    if (GameFlag_Get(0x33a))
      gData_020041b8[26] = 3;
  } else {
    return;
  }
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x40000, 0x20000);
  Actor_MoveToAndWaitById(gMainRoutineWork.currentOwner, cellX << 3,
                          cellZ << 3);
  Actor_SetPositionAndResetMotion(leader, cellX << 19, leader->y, cellZ << 19);
  Event_Wait(4);
  FieldCamera_GetActor()->y += -0x80000;
  next = record + 1;
  if (GameFlag_Get(flag)) {
    s32 x, y, index, toX, toZ;

    Sound_Play(229);
    x = *record;
    record = next;
    {
      s16 scene = gMainRoutineWork.scene;
      y = *record++;
      if (scene == SCENE(Ov655Room1))
        MapGrid_CopyCellPictureIndicesRect(gData_02003fd2[2], gData_02003fd2[3],
                                           x, y, 1, 1);
      else if (scene == SCENE(Ov655Room2))
        MapGrid_CopyCellPictureIndicesRect(gData_02003fda[2], gData_02003fda[3],
                                           x, y, 1, 1);
      else if (scene == SCENE(Ov655Room3))
        MapGrid_CopyCellPictureIndicesRect(gData_02003fe2[2], gData_02003fe2[3],
                                           x, y, 1, 1);
      else if (scene == SCENE(Ov655Room5))
        MapGrid_CopyCellPictureIndicesRect(gData_02003fea[2], gData_02003fea[3],
                                           x, y, 1, 1);
    }
    index = *record++;
    cellX = index * 2;
    toX = record[0];
    toZ = record[1];
    {
      s16 scene = gMainRoutineWork.scene;
      if (scene == SCENE(Ov655Room1)) {
        {
          s32 srcX = gData_02003ff2[cellX], srcY = gData_02003ff2[cellX + 1],
              width = 1, height = 2;
          MapGrid_CopyCellPictureIndicesRect(srcX, srcY, toX, toZ, width,
                                             height);
        }
      } else if (scene == SCENE(Ov655Room2)) {
        {
          s32 srcX = gData_02004012[cellX], srcY = gData_02004012[cellX + 1],
              width = 1, height = 2;
          MapGrid_CopyCellPictureIndicesRect(srcX, srcY, toX, toZ, width,
                                             height);
        }
      } else if (scene == SCENE(Ov655Room3)) {
        {
          s32 srcX = gData_02004032[cellX], srcY = gData_02004032[cellX + 1],
              width = 1, height = 2;
          MapGrid_CopyCellPictureIndicesRect(srcX, srcY, toX, toZ, width,
                                             height);
        }
      } else if (scene == SCENE(Ov655Room5)) {
        if ((n == 0 && GameFlag_Get(0x241)) ||
            (n == 11 && GameFlag_Get(0x24c))) {
          s32 srcX = gData_02004052[8], srcY = gData_02004052[9], width = 1,
              height = 2;
          MapGrid_CopyCellPictureIndicesRect(srcX, srcY, toX, toZ, width,
                                             height);
        } else {
          s32 srcX = gData_02004052[cellX], srcY = gData_02004052[cellX + 1],
              width = 1, height = 2;
          MapGrid_CopyCellPictureIndicesRect(srcX, srcY, toX, toZ, width,
                                             height);
        }
      }
    }
    Event_Wait(12);
    leader->facing = 0x4000;
    leader->motionFlags = 0;
    Actor_SetSpriteFlags(Actor_GetById(gMainRoutineWork.currentOwner), 0);
    {
      s16 scene = gMainRoutineWork.scene;
      if (scene == SCENE(Ov655Room1)) {
        Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 3);
      } else if (scene == SCENE(Ov655Room2) || scene == SCENE(Ov655Room3)) {
        Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 2);
      } else if (scene == SCENE(Ov655Room5)) {
        if (gData_02004420 != NULL)
          ((struct FieldSprite *)gData_02004420->sprite)->priority = 3;
        Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 3);
      }
    }
    Actor_SetModeById(gMainRoutineWork.currentOwner, 28);
    Event_Wait(16);
    for (cellX = 0; (u32)cellX <= 39; cellX++) {
      if (cellX == 5)
        Sound_Play(204);
      leader->scaleX += -0x400;
      leader->scaleY += -0x300;
      leader->y += -0x9999;
      wait(1);
    }
    ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x209;
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Import_EventSetExitRequest(work->exit);
  } else {
    s32 x, y;

    Sound_Play(161);
    y = *next;
    x = *record;
    {
      s16 scene = gMainRoutineWork.scene;
      if (scene == SCENE(Ov655Room1))
        MapGrid_CopyCellPictureIndicesRect(gData_02003fd2[0], gData_02003fd2[1],
                                           x, y, 1, 1);
      else if (scene == SCENE(Ov655Room2))
        MapGrid_CopyCellPictureIndicesRect(gData_02003fda[0], gData_02003fda[1],
                                           x, y, 1, 1);
      else if (scene == SCENE(Ov655Room3))
        MapGrid_CopyCellPictureIndicesRect(gData_02003fe2[0], gData_02003fe2[1],
                                           x, y, 1, 1);
      else if (scene == SCENE(Ov655Room5))
        MapGrid_CopyCellPictureIndicesRect(gData_02003fea[0], gData_02003fea[1],
                                           x, y, 1, 1);
    }
    Event_Wait(12);
    GameFlag_Set(flag);
    GameFlag_Set(0x201);
    {
      s16 scene = gMainRoutineWork.scene;
      if (scene == SCENE(Ov655Room5) || scene == SCENE(Ov655Room3)) {
        gData_0200567c = leader->x >> 20;
        gData_02005678 = leader->z >> 20;
      }
    }
    Event_End();
  }
}

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ s32 Flag_Test(s32 flag) { return GameFlag_Get(flag); }

void Func_02001ee8(void) {
  struct Actor *object;

  if (Flag_Test(0x201) != 0) {
    object = Actor_GetById(gMainRoutineWork.currentOwner);
    if (gData_0200567c != (object->x >> 20) ||
        gData_02005678 != (object->z >> 20))
      GameFlag_Clear(0x201);
  }
}

DEFINE_ACTOR_HIDE(Actor_Hide, Actor_DisableById)

void Scene_RunUnlessFlag(void) {
  if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) != 0) {
    if (GameFlag_Get(0x200) == 0)
      Func_02001500();
  } else {
    GameFlag_Set(0x200);
  }
}

DEFINE_ACTORS_FOR_RANGE(Func_02001f94, 13, Actor_Hide, 13, Scene_RunUnlessFlag)

DEFINE_ACTORS_FOR_RANGE(Func_02001fac, 12, Actor_Hide, 25, Scene_RunUnlessFlag)

DEFINE_ACTORS_FOR_RANGE(Func_02001fc4, 9, Actor_Hide, 26, Scene_RunUnlessFlag)

DEFINE_ACTORS_FOR_RANGE(Func_02001fdc, 2, Actor_Hide, 8, Scene_RunUnlessFlag)

DEFINE_ACTORS_FOR_RANGE(Func_02001ff4, 5, Actor_Hide, 10, Scene_RunUnlessFlag)

/* VIEW(dehkan_plateau): routine HeldActor_RaiseCell as this part declared it;
 * the joined file has another. */
void HeldActor_RaiseCell__dehkan_plateau(struct Actor *object) __asm__(
    "HeldActor_RaiseCell");
#define HeldActor_RaiseCell HeldActor_RaiseCell__dehkan_plateau

void Func_0200200c(struct Actor *object) {
  s32 x;
  u8 *cell;

  HeldActor_RaiseCell(object);
  x = object->x >> 20;
  if (x == 12) {
    Event_Start();
    Event_ResolvePendingActions(0);
    Map_CopyCellAttributeRect(0, 32, 1, 1, x, 11);
    wait(1);
    Actor_DropAndWait(object);
    Map_CopyCellAttributeRect(1, 32, 1, 1, x, 11);
    Event_End();
  } else if (x == 18) {
    cell = ((struct MapCells *)Ram_HeapSlots->mapWork)->cells;
    cell += Actor_GetCellIndex(object) * 4;
    cell[3] &= 0x7f;
  } else if (x == 54) {
    cell = ((struct MapCells *)Ram_HeapSlots->mapWork)->cells;
    cell += Actor_GetCellIndex(object) * 4;
    cell[3] &= 0x7f;
    cell[2] = 11;
  } else if (x == 55) {
    Map_CopyAttributes(53, 20, 1, 1, 54, 20);
    Map_CopyCellAttributeRect(57, 20, 1, 1, 56, 20);
  }
}

/* VIEW(dehkan_plateau): routine Func_02000d78 as this part declared it; the
 * joined file has another. */
void Func_02000d78__dehkan_plateau(const void *data) __asm__("Func_02000d78");
#define Func_02000d78 Func_02000d78__dehkan_plateau
/* VIEW(dehkan_plateau): routine gData_02003fc8 as this part declared it; the
 * joined file has another. */
extern const u8 gData_02003fc8__dehkan_plateau[] __asm__("gData_02003fc8");
#define gData_02003fc8 gData_02003fc8__dehkan_plateau

DEFINE_SCENE_RUN_IF_HELD(Scene_RunIfHeld, Leader_CanPushObjectAhead,
                         Field_FindNearbyTalkTarget, Func_02000d78,
                         gData_02003fc8, Leader_PushObjectAhead, Func_0200200c)

void Func_02002114(void) { Func_02000d78(gData_02003fc8); }

void Func_02002124(void) {
  struct EventWorkCbc *work = Ram_HeapSlots->eventWork;
  struct Actor *active;

  active = work->active;
  if (active != 0)
    Func_0200200c(active);
}

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.

void Func_02002140(void) {
  s32 set = GameFlag_Get(0x207);

  if (set == 0) {
    Actor_GetById(8)->motionFlags = set;
    Map_CopyAttributes(51, 12, 1, 1, 47, 12);
    GameFlag_Set(0x202);
  }
}

void Func_02002180(void) {
  if (GameFlag_Get(0x207) == 0) {
    Map_CopyAttributes(48, 11, 1, 1, 47, 12);
    GameFlag_Clear(0x202);
  }
}

void Func_020021b4(void) {
  struct Actor *object = FieldCamera_GetActor();
  u32 i;

  object->y += -0x80000;
  wait(1);
  for (i = 0; i <= 7; i++) {
    object->y += 0x10000;
    wait(1);
  }
}
#undef SCENE

// ---- games/tla/src/scene/dehkan_plateau/events_g.c ----
// Scene code of overlay 0x655, in ROM order, from events_g's offset up to the
// next object the overlay links.

void MapGrid_CopyCellPictureIndicesRect(s32 srcX, s32 srcY, s32 destX,
                                        s32 destY, s32 width, s32 height);

/* VIEW(events_g): routine CrumblingPillars_Step as this part declared it; the
 * joined file has another. */
void CrumblingPillars_Step__events_g(const void *data) __asm__(
    "CrumblingPillars_Step");
#define CrumblingPillars_Step CrumblingPillars_Step__events_g
/* VIEW(events_g): routine CrumblingPillars_StepWithStandIn as this part
 * declared it; the joined file has another. */
void CrumblingPillars_StepWithStandIn__events_g(const void *data) __asm__(
    "CrumblingPillars_StepWithStandIn");
#define CrumblingPillars_StepWithStandIn                                       \
  CrumblingPillars_StepWithStandIn__events_g
void Actor_SetHorizontalPositionWithTerrain(s32 id, s32 x, s32 z);
void MapGrid_CopyCellPictureIndicesRect(s32 srcX, s32 srcY, s32 destX,
                                        s32 destY, s32 width, s32 height);
/* VIEW(events_g): routine Actor_SetHorizontalPositionAndAngleWithTerrain as
 * void Actor_SetHorizontalPositionAndAngleWithTerrain(u32 id, s32 x, s32 z, s16
 * angle); event.h has void Actor_SetHorizontalPositionAndAngleWithTerrain(s32
 * actorId, s32 x, s32 z, s32 angle); tried as the definition's void
 * Actor_SetHorizontalPositionAndAngleWithTerrain(u32 objectId, s32 x, s32 z,
 * s16 angle), it does not compile in tla-ja:
 * games/tla/src/scene/dehkan_plateau/dehkan_plateau.c:720: conflicting types
 * for `Actor_SetHorizontalPositionAndAngleWithTerrain'. */
extern void Actor_SetHorizontalPositionAndAngleWithTerrain__events_g(
    u32 id, s32 x, s32 z,
    s16 angle) __asm__("Actor_SetHorizontalPositionAndAngleWithTerrain");
void Actor_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void Actor_WalkToAndWaitById(s32 id, s32 x, s32 z);
void Actor_Jump(s32 id, s32 a, s32 b);
void Actor_WatchAndSetScript(s32 id, s32 target);
/* VIEW(events_g): routine Task_Add as void Task_Add(void(*func) (void), s32
 * priority); task.h has s32 Task_Add(TaskFunc func, s32 order); tried as the
 * definition's s32 Task_Add(TaskFunc func, s32 order), which the object already
 * declares, Func_02002604 moves: 1 of 20 instructions in tla-ja. */
extern void Task_Add__events_g(void (*func)(void),
                               s32 priority) __asm__("Task_Add");

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ void Camera_SetBounds(s32 a, s32 b, s32 c, s32 d) {
  FieldCamera_MoveWithinMapBounds(a, b, c, d);
}

static __inline__ void Actor_MoveTo(s32 id, s32 x, s32 z) {
  Actor_WalkToAndWaitById(id, x, z);
}

static __inline__ void Actor_PlaceFacing(s32 id, s32 x, s32 z, s32 angle) {
  Actor_SetHorizontalPositionAndAngleWithTerrain__events_g(id, x, z, angle);
}

void Func_020021e8(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  FieldCamera_GetActor()->motionFlags = 0;
  FieldCamera_MoveWithinMapBounds(658 << 16, -1, 164 << 16, 1);
  FieldCamera_WaitForArrival();
  Actor_FaceActorById(24, gMainRoutineWork.currentOwner, 0);
  Actor_ShowEmoteById(24, 0x100, 40);
  Actor_SetSpeed(24, 0x19999, 0xcccc);
  Actor_MoveTo(24, 664, 136);
  MapGrid_CopyCellPictureIndicesRect(68, 33, 105, 6, 1, 1);
  Sound_Play(161);
  Func_020021b4();
  GameFlag_Set(0x247);
  Actor_MoveTo(24, 664, 104);
  MapGrid_CopyCellPictureIndicesRect(69, 33, 105, 4, 1, 1);
  Sound_Play(229);
  Func_020021b4();
  Event_Wait(10);
  Actor_Jump(24, 6, 16);
  Actor_PlaceFacing(24, 222 << 18, 168 << 16, FACING_SOUTH);
  Event_Wait(20);
  GameFlag_Set(0x86c);
  Event_End();
}

void Func_020022e0(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  FieldCamera_MoveWithinMapBounds(245 << 18, -1, 168 << 16, 1);
  FieldCamera_WaitForArrival();
  Actor_FaceActorById(24, gMainRoutineWork.currentOwner, 0);
  Actor_ShowEmoteById(24, 0x100, 40);
  Actor_GetById(24)->motionFlags = 0;
  Actor_SetSpritePriority(24, 1);
  Actor_SetSpeed(24, 0x19999, 0xcccc);
  Actor_MoveTo(24, 940, 168);
  Actor_MoveTo(24, 984, 152);
  Actor_MoveTo(24, 1040, 152);
  Actor_SetHorizontalPositionWithTerrain(24, 0, 0);
  Event_Wait(20);
  GameFlag_Set(0x86d);
  Event_End();
}

void Func_02002390(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  FieldCamera_MoveWithinMapBounds(144 << 16, -1, 214 << 15, 1);
  FieldCamera_WaitForArrival();
  Actor_FaceActorById(25, gMainRoutineWork.currentOwner, 0);
  Actor_ShowEmoteById(25, 0x100, 40);
  Actor_SetSpeed(25, 0x19999, 0xcccc);
  Actor_WalkToAndWaitById(25, 168, 104);
  Event_Wait(10);
  Sound_Play(153);
  Actor_GetById(25)->velocityY = 0x80000;
  Actor_SetSpriteFlags(Actor_GetById(25), 0);
  Actor_WalkToAndWaitById(25, 200, 104);
  Actor_SetSpriteFlags(Actor_GetById(25), 1);
  Actor_MoveTo(25, 294, 104);
  Actor_PlaceFacing(25, 212 << 17, 208 << 15, FACING_SOUTH);
  GameFlag_Set(0x86e);
  Event_End();
}

void Func_02002458(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_WatchAndSetScript(gMainRoutineWork.currentOwner, 25);
  FieldCamera_GetActor()->motionFlags = 0;
  Camera_SetBounds(212 << 17, -1, 208 << 15, 1);
  FieldCamera_WaitForArrival();
  Actor_FaceActorById(25, gMainRoutineWork.currentOwner, 0);
  Actor_ShowEmoteById(25, 0x100, 20);
  FieldCamera_MoveWithinMapBounds(212 << 17, -1, 152 << 16, 1);
  Actor_SetSpeed(25, 0x19999, 0xcccc);
  Actor_MoveTo(25, 424, 152);
  Actor_ArmCallback(25, FACING_EAST, 20);
  Func_02002604();
  Actor_MoveTo(25, 440, 152);
  Actor_MoveTo(25, 456, 168);
  Actor_MoveTo(25, 456, 184);
  GameFlag_Set(0x241);
  MapGrid_CopyCellPictureIndicesRect(69, 51, 92, 11, 1, 1);
  Sound_Play(161);
  Func_020021b4();
  Actor_MoveTo(25, 456, 216);
  Actor_MoveTo(25, 584, 216);
  Actor_PlaceFacing(25, 154 << 18, 140 << 17, FACING_SOUTH);
  GameFlag_Set(0x86f);
  Event_End();
}

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.

void Func_02002574(void) {
  struct EffectWork7c4 *work = Ram_HeapSlots->effectWork;
  struct Actor *object = Actor_GetById(18);

  if (work->timer != -1 && work->timer <= 7)
    object->scaleY += -0x1f00;
  if (work->active != 0) {
    Map_CopyAttributes(27, 8, 1, 1, 27, 9);
    Actor_SetModeById(18, 4);
    object->priorityFlags |= 2;
    ((u8 *)object)[89] &= 0xfe;
    object->scaleY = 0x10000;
    GameFlag_Set(0x205);
  }
}

void Func_02002604(void) {
  BattleEffect_LoadActionEffectResources(134, 1);
  Task_Add__events_g(Func_02002574, 0x480);
  FieldPsynergy_SetupActorPair(25, 18);
  EventObject_Initialize();
  FieldPsynergy_RunHandler(1);
  BattleEffect_StopMainActor();
  Task_Remove(Func_02002574);
  BattleEffect_CleanupSceneObjects();
}

#include "../field/field_effect/advance_motion.c"

#undef DEFINE_DEHKAN_PLATEAU_RESTORE_CELLS
#undef SCENE
#undef EFFECT_SPAWN
#undef EFFECT_CREATE
#undef EFFECT_SET_MODE
#undef EFFECT_SET_SCRIPT
#undef EFFECT_SCRIPTS
#undef gData_02003fd2
#undef gData_02004066
#undef gData_02003f44
#undef gData_02003f52
#undef gData_02003f80
#undef gData_02003f92
#undef gData_02003f98
#undef gData_02003fa0
#undef gData_02003fa8
#undef gData_02003fb0
#undef gData_02003fb8
#undef gData_02003fc0
#undef gData_02003fc4
#undef gData_020045e0
#undef gData_020045f8
#undef gData_020047d8
#undef gData_02004ac0
#undef gData_02004d78
#undef gData_02004dd8
#undef gData_02004e20
#undef gData_02004f10
#undef gData_02004f1c
#undef gData_02005078
#undef gData_020051bc
#undef gData_020053b4
#undef gData_02005450
#undef gData_020055b8
#undef gData_02003fda
#undef gData_0200410e
#undef gData_02003fe2
#undef gData_0200416e
#undef gData_02004420
#undef HeldActor_RaiseCell
#undef Func_02000d78
#undef gData_02003fc8
#undef CrumblingPillars_Step
#undef CrumblingPillars_StepWithStandIn
// ---- from scene/dehkan_plateau/s2698.c ----
// @file
// Scene code of overlay 0x655, in ROM order.

// ---- games/tla/src/scene/dehkan_plateau/s2698.c ----

#include "flags.h"
#include "types.h"
#include "system.h"
#include "actor.h"
#include "field_sprite.h"
#include "fixed_math.h"
#include "event_inline.h"
#include "scene_tables.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "event.h"
#include "field.h"
#include "flag_call.h"
#include "gamepak/far_call/system.h"
#include "heap_release.h"
#include "module/field/event_commands.h"
#include "module/scene/dehkan_plateau.h"
#include "module/scene/scene.h"
#include "module/tla.h"
#include "scene_call.h"

void Actor_DisableById(s32 id);
/* VIEW(s2698): routine gData_02004420 as this part declared it; the joined file
 * has another. */
extern struct Actor *gData_02004420__s2698 __asm__("gData_02004420");
#define gData_02004420 gData_02004420__s2698

void Actor_StartScript(struct Actor *object, const void *script);
/* VIEW(s2698): routine FieldEffect_AdvanceMotion as void
 * FieldEffect_AdvanceMotion(struct Actor *object); module/field/field_effect.h
 * has void FieldEffect_AdvanceMotion(union FieldObject *object); not tried:
 * FieldEffect_AdvanceMotion's bound declaration was not found. */
extern void Effect_AdvanceMotion__s2698(struct Actor *object) __asm__(
    "FieldEffect_AdvanceMotion");

/* VIEW(s2698): routine gData_02003f14 as this part declared it; the joined file
 * has another. */
extern const u8 gData_02003f14__s2698[] __asm__("gData_02003f14");
#define gData_02003f14 gData_02003f14__s2698

// Flashes actor id, then raises eight puffs over actor center, then sets
// flag and waits 20 frames.
void Func_02002698(s32 id, s32 center, s32 flag) {
  struct Actor *leader;
  s32 frame;
  u32 i;

  Sound_Play(125);
  Actor_SetSpriteColor(Actor_GetById(id), 7);
  wait(2);
  Actor_SetModeById(id, 0);
  Actor_SetSpriteColor(Actor_GetById(id), 7);
  wait(4);
  Actor_SetSpriteColor(Actor_GetById(id), 0);
  leader = Actor_GetById(center);
  frame = 0;
  for (i = 0; i <= 7; i++) {
    struct Actor *object;
    struct EffectView *effect;
    s32 y;

    /* FAKEMATCH(temporary): the form kept; as one call argument, 10 of 129
     * instructions differ in tla-en. */
    y = leader->y - ((((Random16() * 7) << 1) >> 16) << 16);
    object = ((struct Actor *)Import_ActorCreate(
        285, (i << 17) + leader->x - Q16_ONE * 8, y - Q16_ONE * 2, leader->z));
    if (object != NULL) {
      effect = (struct EffectView *)object;
      frame = Sprite_ShareVramCacheEntry(effect->sprite, frame);
      object->motionFlags = 0;
      object->collisionFlags = 0;
      Actor_SetSpriteFlags(object, 0);
      Func_02003ba0(object, 2);
      Actor_StartScript(object, gData_02003f14);
      object->scaleX = 0xb333;
      object->scaleY = 0xb333;
      effect->sprite->priority = 1;
      effect->velocityX = 0;
      effect->velocityZ = 0;
      effect->velocityY = -((((u32)Random16() << 14) >> 16) + Q16_ONE * 3 / 8);
      effect->spin = (((u32)Random16() << 9) >> 16) - 256;
      effect->scaleRateX = -(Q16_ONE / 32);
      effect->scaleRateY = -(Q16_ONE * 3 / 128);
      effect->update = Effect_AdvanceMotion__s2698;
    }
  }
  GameFlag_Set(flag);
  Event_Wait(20);
}

void Func_020027e0(void) {
  struct Actor *actor;

  actor = Actor_GetById(25);
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_WatchAndSetScript(gMainRoutineWork.currentOwner, 25);
  FieldCamera_GetActor()->motionFlags = 0;
  FieldCamera_MoveTo(PIXELS(616), -1, PIXELS(280), 1);
  FieldCamera_WaitForArrival();
  Actor_FaceActorById(25, gMainRoutineWork.currentOwner, 0);
  // A direct call: as Call3 the actor id is loaded before the 0x100's shift.
  Actor_ShowEmoteById(25, 0x100, 40);
  Actor_SetSpritePriority(25, 1);
  Actor_SetSpeed(25, 0x19999, 0xcccc);
  Actor_WalkToAndWait(25, 632, 280);
  Actor_SetSpeed(25, 0x20000, 0x10000);
  Sound_Play(153);
  actor->velocityY = 0x80000;
  Actor_SetSpriteFlags(Actor_GetById(25), 0);
  Actor_WalkToAndWait(25, 664, 280);
  Actor_SetSpriteFlags(Actor_GetById(25), 1);
  Func_02002698(21, 25, 0x210);
  Sound_Play(153);
  actor->velocityY = 0x80000;
  Actor_SetSpriteFlags(Actor_GetById(25), 0);
  Actor_WalkToAndWait(25, 696, 280);
  Actor_SetSpriteFlags(Actor_GetById(25), 1);
  Event_Wait(10);
  Actor_SetSpeed(25, 0x19999, 0xcccc);
  Actor_WalkToAndWait(25, 712, 280);
  Camera_SetSpeedParameters(0x26666, 0x4ccc);
  FieldCamera_MoveWithinMapBounds(PIXELS(712), -1, PIXELS(136), 1);
  MapGrid_CopyCellPictureIndicesRect(112, 6, 110, 4, 1, 1);
  Actor_SetSpritePriority(25, 1);
  Actor_WalkToAndWait(25, 712, 200);
  Actor_WalkToAndWaitById(25, 712, 168);
  Actor_Stop(gMainRoutineWork.currentOwner);
  Actor_FaceDirectionById(25, FACING_EAST);
  Actor_SetSpeedParameters(25, 0x20000, 0x10000);
  Sound_Play(153);
  actor->velocityY = 0x80000;
  Actor_SetSpriteFlags(Actor_GetById(25), 0);
  Actor_WalkToAndWait(25, 744, 168);
  Actor_SetSpriteFlags(Actor_GetById(25), 1);
  Func_02002698(24, 25, 0x216);
  Actor_FaceDirectionById(25, FACING_NORTH);
  Sound_Play(153);
  actor->velocityY = 0x80000;
  Actor_SetSpriteFlags(Actor_GetById(25), 0);
  Actor_WalkToAndWait(25, 744, 136);
  Actor_SetSpriteFlags(Actor_GetById(25), 1);
  Event_Wait(10);
  Actor_WalkToAndWait(25, 744, 72);
  MapGrid_CopyCellPictureIndicesRect(66, 51, 110, 4, 1, 1);
  Sound_Play(229);
  Func_020021b4();
  Event_Wait(10);
  Actor_Jump(25, 6, 16);
  Actor_SetHorizontalPositionWithTerrain(25, 0, 0);
  Event_Wait(20);
  GameFlag_Set(0x870);
  Event_End();
}

void Func_02002a5c(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  u32 i;

  Event_Start();
  Event_ResolvePendingActions(0);
  {
    s32 facing = FACING_NORTH;

    Actor_SetHorizontalPositionAndAngleWithTerrain(
        gMainRoutineWork.currentOwner, PIXELS(248), PIXELS(136), facing);
  }
  MapGrid_CopyCellPictureIndicesRect(65, 64, 15, 8, 1, 1);
  GameFlag_Set(0x245);
  Sound_Play(161);
  Func_020021b4();
  Actor_FaceActorById(9, gMainRoutineWork.currentOwner, 0);
  Actor_ShowEmoteById(9, 0x100, 20);
  Func_02002bcc();
  Event_Wait(20);
  leader->facing = FACING_SOUTH;
  leader->motionFlags = 0;
  Actor_SetSpritePriority(9, 3);
  Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 3);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 28);
  Event_Wait(16);
  for (i = 0; i < 40; i++) {
    if (i == 5)
      Sound_Play(204);
    leader->scaleX -= 0x400;
    leader->scaleY -= 0x300;
    leader->y -= 0x9999;
    wait(1);
  }
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x209;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(23);
}

void Func_02002b68(void) {
  if (((struct EffectWork7c4 *)Ram_HeapSlots->effectWork)->active != 0) {
    Sound_Play(229);
    MapGrid_CopyCellPictureIndicesRect(64, 64, 15, 8, 1, 1);
    MapGrid_CopyCellPictureIndicesRect(64, 66, 15, 72, 1, 2);
    Actor_SetSpriteFlags(Actor_GetById(gMainRoutineWork.currentOwner), 0);
  }
}

/* VIEW(events_i): routine gData_02003f44 as this part declared it; the joined
 * file has another. */
// ---- games/tla/src/scene/dehkan_plateau/events_i.c ----
extern const u8 gData_02003f44__events_i[] __asm__("gData_02003f44");
#define gData_02003f44 gData_02003f44__events_i
/* VIEW(events_i): routine gData_02003f52 as this part declared it; the joined
 * file has another. */
extern const u8 gData_02003f52__events_i[] __asm__("gData_02003f52");
#define gData_02003f52 gData_02003f52__events_i
/* VIEW(events_i): routine gData_02003f80 as this part declared it; the joined
 * file has another. */
extern const u8 gData_02003f80__events_i[] __asm__("gData_02003f80");
#define gData_02003f80 gData_02003f80__events_i
/* VIEW(events_i): routine gData_02003f92 as this part declared it; the joined
 * file has another. */
extern const u8 gData_02003f92__events_i[] __asm__("gData_02003f92");
#define gData_02003f92 gData_02003f92__events_i
/* VIEW(events_i): routine gData_02003f98 as this part declared it; the joined
 * file has another. */
extern const u8 gData_02003f98__events_i[] __asm__("gData_02003f98");
#define gData_02003f98 gData_02003f98__events_i
/* VIEW(events_i): routine gData_02003fa0 as this part declared it; the joined
 * file has another. */
extern const u8 gData_02003fa0__events_i[] __asm__("gData_02003fa0");
#define gData_02003fa0 gData_02003fa0__events_i
/* VIEW(events_i): routine gData_02003fa8 as this part declared it; the joined
 * file has another. */
extern const u8 gData_02003fa8__events_i[] __asm__("gData_02003fa8");
#define gData_02003fa8 gData_02003fa8__events_i
/* VIEW(events_i): routine gData_02003fb0 as this part declared it; the joined
 * file has another. */
extern const u8 gData_02003fb0__events_i[] __asm__("gData_02003fb0");
#define gData_02003fb0 gData_02003fb0__events_i
/* VIEW(events_i): routine gData_02003fb8 as this part declared it; the joined
 * file has another. */
extern const u8 gData_02003fb8__events_i[] __asm__("gData_02003fb8");
#define gData_02003fb8 gData_02003fb8__events_i
/* VIEW(events_i): routine gData_02003fc0 as this part declared it; the joined
 * file has another. */
extern const u8 gData_02003fc0__events_i[] __asm__("gData_02003fc0");
#define gData_02003fc0 gData_02003fc0__events_i
/* VIEW(events_i): routine gData_02003fc4 as this part declared it; the joined
 * file has another. */
extern const u8 gData_02003fc4__events_i[] __asm__("gData_02003fc4");
#define gData_02003fc4 gData_02003fc4__events_i

/* VIEW(events_i): routine CrumblingPillars_Step as this part declared it; the
 * joined file has another. */
void CrumblingPillars_Step__events_i(const void *data) __asm__(
    "CrumblingPillars_Step");
#define CrumblingPillars_Step CrumblingPillars_Step__events_i
/* VIEW(events_i): routine CrumblingPillars_StepWithStandIn as this part
 * declared it; the joined file has another. */
void CrumblingPillars_StepWithStandIn__events_i(const void *data) __asm__(
    "CrumblingPillars_StepWithStandIn");
#define CrumblingPillars_StepWithStandIn                                       \
  CrumblingPillars_StepWithStandIn__events_i
void Func_02002180(void);
void MapGrid_CopyCellPictureIndicesRect(s32 srcX, s32 srcY, s32 destX,
                                        s32 destY, s32 width, s32 height);
void Actor_SetHorizontalPositionWithTerrain(s32 id, s32 x, s32 z);
void Func_02001804(void);

/* VIEW(events_i): routine gData_02003fd2 as this part declared it; the joined
 * file has another. */
extern const u16 gData_02003fd2__events_i[] __asm__("gData_02003fd2");
#define gData_02003fd2 gData_02003fd2__events_i
/* VIEW(events_i): routine gData_02004066 as this part declared it; the joined
 * file has another. */
extern const struct CellRecord
    gData_02004066__events_i[] __asm__("gData_02004066");
#define gData_02004066 gData_02004066__events_i

void Actor_SetHorizontalPositionAndAngleWithTerrain(s32 id, s32 x, s32 z,
                                                    s32 angle);
/* VIEW(s2698): routine ItemDisplay_ShowPicture as void
 * ItemDisplay_ShowPicture(struct Actor *object, s32 mode);
 * module/scene/airs_rock_inside.h has void ItemDisplay_ShowPicture(void); not
 * tried: ItemDisplay_ShowPicture's bound declaration was not found. */
extern void Func_02003d40__s2698(struct Actor *object,
                                 s32 mode) __asm__("ItemDisplay_ShowPicture");

s32 PartyInventory_GiveItem(s32 item, s32 unused);
void Actor_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void Actor_WalkToAndWaitById(s32 id, s32 x, s32 z);
void Actor_Jump(s32 id, s32 a, s32 b);
void Actor_WatchAndSetScript(s32 id, s32 target);
/* VIEW(events_i): routine Task_Add as void Task_Add(void(*func) (void), s32
 * priority); task.h has s32 Task_Add(TaskFunc func, s32 order); tried as the
 * definition's s32 Task_Add(TaskFunc func, s32 order), which the object already
 * declares, Func_02002bcc moves: 1 of 24 instructions in tla-ja. */
extern void Task_Add__events_i(void (*func)(void),
                               s32 priority) __asm__("Task_Add");

void Func_02002bcc(void) {
  BattleEffect_LoadActionEffectResources(134, 1);
  Task_Add__events_i(Func_02002b68, 0x480);
  FieldPsynergy_SetupActorPair(9, gMainRoutineWork.currentOwner);
  EventObject_Initialize();
  FieldPsynergy_RunHandler(1);
  BattleEffect_StopMainActor();
  Task_Remove(Func_02002b68);
  BattleEffect_CleanupSceneObjects();
}

// ---- games/tla/src/scene/dehkan_plateau/s2c14.c ----
// Scene code of overlay 0x655, in ROM order, from s2c14's offset up to the next
// object the overlay links.

#include "map.h"
#include "iwram_call.h"
#include "scene_ids.h"

void Actor_TakePlaceOf(struct Actor *object, struct Actor *other);
void Actor_MoveToObject(s32 id, s32 target);

void Func_02002c14(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct Actor *helper = Actor_GetById(9);

  Event_Start();
  Event_ResolvePendingActions(0);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Map_CenterOnFollowedPoint();
  wait(1);
  leader->y = 0x820000;
  leader->gravity = 0x8000;
  leader->bounce = 0;
  leader->motionFlags = 0;
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x100;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Sound_Play(204);
  leader->motionFlags = 3;
  Event_Wait(24);
  Sound_Play(176);
  Func_020021b4();
  if (gData_02004420 != NULL) {
    FieldActor_SetPositionAndResetMotion(gData_02004420, 0xf80000, 0, 0x780000);
    wait(1);
    gData_02004420->speedLimit = 0x19999;
    gData_02004420->acceleration = 0x19999;
    gData_02004420->velocityY = 0x80000;
  }
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x19999, 0xcccc);
  Actor_SetSpeed(9, 0x19999, 0xcccc);
  leader->velocityY = 0x60000;
  helper->velocityY = 0x60000;
  FieldActor_SetMoveTarget(leader, 0xd80000, 0, 0x780000);
  FieldActor_SetMoveTarget(helper, 0x1180000, 0, 0x780000);
  if (gData_02004420 != NULL)
    FieldActor_SetMoveTarget(gData_02004420, 0xf80000, 0, 0x580000);
  Actor_WaitForMove(leader);
  Event_Wait(20);
  Actor_ShowEmoteById(9, 0x103, 0);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, 0, 0);
  Actor_ArmCallback(9, 0x8000, 60);
  Actor_WalkToAndWait(9, 296, 104);
  Actor_WalkToAndWait(9, 296, 80);
  Actor_SetHorizontalPositionWithTerrain(9, 0, 0);
  Event_Wait(20);
  if (gData_02004420 != NULL) {
    gData_02004420->motionFlags = 0;
    MapGrid_CopyCellAttributes(0, 0, 1, 1, 15, 5);
    wait(1);
  }
  GameFlag_Set(0x871);
  Event_End();
}

void Func_02002dd4(void) {
  struct Actor *actor;

  actor = Actor_GetById(16);
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_WatchAndSetScript(gMainRoutineWork.currentOwner, 16);
  FieldCamera_GetActor()->motionFlags = 0;
  FieldCamera_MoveWithinMapBounds(PIXELS(760), PIXELS(32), PIXELS(204), 1);
  FieldCamera_WaitForArrival();
  Actor_FaceActorById(16, gMainRoutineWork.currentOwner, 0);
  // Called for its value, which is unused: as Call3 the actor id is loaded
  // before the 0x100's shift.
  Actor_ShowEmote(16, 0x100, 40);
  Actor_SetSpeedParameters(16, 0x20000, 0x10000);
  Sound_Play(153);
  actor->velocityY = 0x80000;
  Actor_SetSpriteFlags(Actor_GetById(16), 0);
  Actor_WalkToAndWaitById(16, 760, 200);
  Actor_SetSpriteFlags(Actor_GetById(16), 1);
  Event_Wait(20);
  Sound_Play(153);
  actor->velocityY = 0x80000;
  Actor_SetSpriteFlags(Actor_GetById(16), 0);
  Actor_WalkToAndWaitById(16, 792, 200);
  Actor_SetSpriteFlags(Actor_GetById(16), 1);
  Func_02002698(9, 16, 0x210);
  Sound_Play(153);
  actor->velocityY = 0x80000;
  Actor_SetSpriteFlags(Actor_GetById(16), 0);
  Actor_WalkToAndWaitById(16, 824, 200);
  Actor_SetSpriteFlags(Actor_GetById(16), 1);
  Event_Wait(20);
  Actor_WatchAndSetScript(16, gMainRoutineWork.currentOwner);
  Actor_Stop(gMainRoutineWork.currentOwner);
  Event_Wait(20);
  Event_End();
}

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ void Actor_SetAngle(s32 id, s32 angle) {
  Actor_FaceDirectionById(id, angle);
}

void Func_02002f08(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_FaceActorById(16, gMainRoutineWork.currentOwner, 0);
  Actor_ShowEmoteById(16, 0x100, 40);
  Actor_FaceDirectionById(16, FACING_EAST);
  Event_Wait(20);
  Motion_SetVarCbAndRefresh(16, 3);
  Actor_SetAngle(16, FACING_WEST);
  Actor_WatchAndSetScript(16, gMainRoutineWork.currentOwner);
  FieldEffect_SetAttached(16, 0x102);
  Event_Wait(40);
  Event_End();
}

void Func_02002f74(void) {
  if (GameFlag_Get(0x207) != 0)
    Func_02002f08();
  else
    Func_02002dd4();
  GameFlag_Set(0x209);
}

void Func_02002f9c(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  if (gData_02004420 != 0)
    Func_02003d40__s2698(gData_02004420, 3);
  PartyInventory_GiveItem(199, 0);
  if (gData_02004420 != 0)
    Actor_Release(gData_02004420);
  Map_CopyAttributes(1, 0, 1, 1, 15, 5);
  GameFlag_Set(0x878);
  Event_End();
}

void Func_02002ff4(void) {
  u8 *work = Ram_HeapSlots->eventWork;

  Event_Start();
  Event_ResolvePendingActions(0);
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov655Room1)
    Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 3);
  Sound_Play(123);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(*(s16 *)(work + 0x154));
}

s32 Scene_Initialize(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov655Room1)
    Func_020030e4();
  else if (scene == (s32)&SceneId_Ov655Room2)
    Func_02003164();
  else if (scene == (s32)&SceneId_Ov655Room3)
    Func_02003284();
  else if (scene == (s32)&SceneId_Ov655Room4)
    Func_0200369c();
  else if (scene == (s32)&SceneId_Ov655Room5)
    Func_0200390c();
  else if (scene == (s32)&SceneId_Ov655Room6)
    Func_02003aa0();
  return 0;
}

/* VIEW(s2698): routine Func_020030c0 as this part defines it; a header the
 * joined file includes has another. */
void Func_020030c0__s2698(void) __asm__("Func_020030c0");
void Func_020030c0__s2698(void) {
  Actor_SetSpritePriority(64, ((struct AnimationFacing *)Actor_GetById(
                                   gMainRoutineWork.currentOwner)
                                   ->sprite)
                                  ->facing);
}

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.

/* VIEW(s2c14): routine Pillars_Place as this part declared it; the joined file
 * has another. */
void Pillars_Place__s2c14(const void *data) __asm__("Pillars_Place");
#define Pillars_Place Pillars_Place__s2c14
void Func_02001820(void);

void Func_020030e4(void) {
  CrumblingPillars_Place(gData_02003f44);
  Actor_SetSpritePriority(8, 3);
  Pillars_Place(gData_02003f98);
  Actor_DisableById(12);
  if (GameFlag_Get(0x301) != 0) {
    Map_CopyAttributes(54, 32, 2, 1, 56, 64);
    Actor_SetPositionAndResetMotion(Actor_GetById(12), 226 << 18, 128 << 15,
                                    130 << 18);
    wait(10);
  }
  Func_02001820();
  Func_02001f94();
  {
    s32 priority = 0x480;

    Task_Add(Func_020030c0, priority);
  }
}

// ---- games/tla/src/scene/dehkan_plateau/s3164.c ----

/* VIEW(s3164): routine CrumblingPillars_PlaceWithStandIns as this part declared
 * it; the joined file has another. */
void CrumblingPillars_PlaceWithStandIns__s3164(const void *data) __asm__(
    "CrumblingPillars_PlaceWithStandIns");
#define CrumblingPillars_PlaceWithStandIns                                     \
  CrumblingPillars_PlaceWithStandIns__s3164
void Func_020018a8(void);
void MapGrid_CopyCellPictureIndicesRect(s32 srcX, s32 srcY, s32 destX,
                                        s32 destY, s32 width, s32 height);
void Actor_SetHorizontalPositionAndAngleWithTerrain(s32 id, s32 x, s32 z,
                                                    s32 angle);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.

// Places an actor and resets its motion (Actor_SetPositionAndResetMotion).
static __inline__ void Actor_PlaceAndResetMotion(struct Actor *object, s32 x,
                                                 s32 y, s32 z) {
  Actor_SetPositionAndResetMotion(object, x, y, z);
}
/* VIEW(s2698): routine Actor_PlaceFacing as this part has it; dehkan_plateau
 * has its own. */
#define Actor_PlaceFacing Actor_PlaceFacing__s2698

static __inline__ void Actor_PlaceFacing(s32 id, s32 x, s32 z, s32 angle) {
  Actor_SetHorizontalPositionAndAngleWithTerrain(id, x, z, angle);
}

void Func_02003164(void) {
  if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0) {
    Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 32;
    if (gMainRoutineWork.entrance == 5) {
      Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 2);
      Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 1;
    }
  }
  CrumblingPillars_PlaceWithStandIns(gData_02003f52);
  Func_020012d8();
  Pillars_Place(gData_02003fa0);
  if (GameFlag_Get(0x840) != 0) {
    Actor_DisableById(23);
    Map_CopyAttributes(50, 7, 1, 1, 52, 7);
    Actor_PlaceAndResetMotion(Actor_GetById(23), 210 << 18, 0, 240 << 15);
  } else {
    Actor_PlaceBlocks(gData_02003fc0);
  }
  Func_020018a8();
  if (GameFlag_Get(0x86c) != 0) {
    s32 angle = 0x4000;

    Actor_PlaceFacing(24, 222 << 18, 168 << 16, angle);
    MapGrid_CopyCellPictureIndicesRect(68, 33, 105, 6, 1, 1);
    MapGrid_CopyCellPictureIndicesRect(69, 33, 105, 4, 1, 1);
    GameFlag_Set(583);
  }
  if (GameFlag_Get(0x86d) != 0)
    Actor_SetHorizontalPositionWithTerrain(24, 0, 0);
  Func_02001fac();
}

// ---- games/tla/src/scene/dehkan_plateau/s3284.c ----
// Scene code of overlay 0x655, in ROM order, from s3284's offset up to the next
// object the overlay links.

#include "task.h"
#include "field_effect.h"

/* VIEW(s3284): routine gData_02003fc8 as this part declared it; the joined file
 * has another. */
extern const u8 gData_02003fc8__s3284[] __asm__("gData_02003fc8");
#define gData_02003fc8 gData_02003fc8__s3284
extern s32 gData_02005678;

/* VIEW(s3284): routine Pillars_Raise as this part declared it; the joined file
 * has another. */
void Pillars_Raise__s3284(const void *data) __asm__("Pillars_Raise");
#define Pillars_Raise Pillars_Raise__s3284
void Func_02001ee8(void);
/* VIEW(s3284): routine Actor_GetCellIndex as s32 Actor_GetCellIndex(void);
 * s2c14 has s32 Actor_GetCellIndex(struct Actor *object); not tried:
 * Actor_GetCellIndex's bound declaration was not found. */
extern s32 Func_02003db0__s3284(void) __asm__("Actor_GetCellIndex");

// The room entry written in the scene layer of Func_02003164 and Func_0200390c:
// sets actors' variants, links, cells and flags for the room.
void Func_02003284(void) {
  struct Actor *object;
  s32 zero, value;

  if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0) {
    Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 32;
    Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 2);
    Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 1;
  }
  Actor_SetSpritePriority(8, 1);
  Actor_SetSpritePriority(9, 1);
  Actor_SetSpritePriority(10, 1);
  Actor_SetSpritePriority(11, 1);
  /* FAKEMATCH(dead): an empty once-only block ends the argument loads' tie; as
   * no block, 2 of 276 instructions differ in tla-en. */
  do {
  } while (0);
  Tightrope_Initialize();
  Tightrope_Place(0, 8, 9);
  Tightrope_Place(1, 10, 11);
  Actor_SetSpritePriority(12, 1);
  Actor_SetSpritePriority(13, 1);
  Func_02003d90();
  Rope_Place(0, 520, 12, 13);
  Actor_PlaceBlocks(gData_02003fc4);
  if (Actor_GetById(19)->y > (s32)0xfff80000)
    Actor_SetSpritePriority(19, 1);
  else
    Actor_SetSpritePriority(19, 2);
  Actor_DisableById(20);
  Actor_GetById(20)->priorityFlags |= 32;
  Actor_SetSpritePriority(20, 2);
  if (GameFlag_Get(0x841) != 0)
    Func_0200172c();
  else
    MapGrid_CopyCellAttributes(38, 32, 1, 1, 38, 33);
  Actor_GetById(14)->priorityFlags &= ~1;
  Actor_GetById(15)->priorityFlags &= ~1;
  Actor_GetById(16)->priorityFlags &= ~1;
  Actor_GetById(17)->priorityFlags &= ~1;
  Pillars_Raise(gData_02003fc8);
  value = Actor_GetById(17)->x;
  zero = 0;
  if ((value >> 20) == 54) {
    struct MapCell *cell = Ram_HeapSlots->mapWork->layers[0].cells;

    cell += Func_02003db0__s3284();
    cell->attributeB &= 0x7f;
    cell->collisionCode = 11;
  }
  Actor_SetSpritePriority(14, 1);
  Actor_SetSpritePriority(15, 1);
  Func_02001930();
  if (GameFlag_Get(0x86e) != 0) {
    s32 angle = 0x4000;

    Actor_SetPositionAndFacing(25, 212 << 17, 208 << 15, angle);
  }
  Actor_SetModeById(18, 3);
  if (GameFlag_Get(0x86f) != 0) {
    object = Actor_GetById(18);
    MapGrid_CopyCellAttributes(27, 8, 1, 1, 27, 9);
    Actor_SetModeById(18, 4);
    object->priorityFlags |= 2;
    *((u8 *)object + 89) &= ~1;
    GameFlag_Set(517);
    GameFlag_Set(577);
    MapGrid_CopyCellPictureIndicesRect(69, 51, 92, 11, 1, 1);
    {
      s32 angle = 0x4000;

      Actor_SetPositionAndFacing(25, 154 << 18, 140 << 17, angle);
    }
  }
  if (GameFlag_Get(0x870) != 0) {
    GameFlag_Set(528);
    GameFlag_Set(534);
    Actor_SetHorizontalPositionWithTerrain(25, 0, 0);
  }
  CrumblingPillars_Place(gData_02003f80);
  Func_020013b0();
  Func_02001fc4();
  {
    struct Actor *last = Actor_GetById(27);

    *((u8 *)last + 85) = zero;
    last->y = zero;
  }
  gData_0200567c = zero;
  gData_02005678 = zero;
  Task_Add(Func_02001ee8, TASK_PASS_VBLANK);
}

/* VIEW(s3284): routine gData_02004276 as this part declared it; the joined file
 * has another. */
extern const u16 gData_02004276__s3284[] __asm__("gData_02004276");
#define gData_02004276 gData_02004276__s3284

#define DEFINE_DEHKAN_PLATEAU_PLACE_BY_FLAG(name, n1, n2, n3, n4, n5, n6)      \
  void name(void) {                                                            \
    /* FAKEMATCH(pin): flag in r5, which leaves r6 to the entry; as a plain    \
     * local, 61 of 79 instructions differ in tla-en. */                       \
    register s32 flag asm("r5") = 800;                                         \
    s32 offset = 0;                                                            \
                                                                               \
    while (1) {                                                                \
      if (GameFlag_Get(flag)) {                                                \
        u16 *entry = (u16 *)((u8 *)gData_02004276 + offset);                   \
        s32 y;                                                                 \
        s32 x;                                                                 \
        s32 one;                                                               \
                                                                               \
        flag -= 800;                                                           \
        x = *entry++;                                                          \
        y = *entry++;                                                          \
        one = 1;                                                               \
        MapGrid_CopyCellsTo(n1, n2, x, y, one, one);                           \
        x = entry[0];                                                          \
        y = entry[1];                                                          \
        MapGrid_CopyCellsTo(n3, n4, x, y, one, 5);                             \
        if (flag == 1)                                                         \
          MapGrid_CopyCellsTo(67, 4, 69, 3, one, one);                         \
        x -= 64;                                                               \
        y += n5;                                                               \
        MapGrid_CopyCellsTo(n1, n6, x, y, one, 4);                             \
        return;                                                                \
      }                                                                        \
      if (flag == 863)                                                         \
        return;                                                                \
      offset += 8;                                                             \
      flag++;                                                                  \
    }                                                                          \
  }

DEFINE_DEHKAN_PLATEAU_PLACE_BY_FLAG(Func_0200353c, 3, 8, 67, 3, 66, 69)

DEFINE_DEHKAN_PLATEAU_PLACE_BY_FLAG(Func_020035ec, 13, 5, 77, 0, 65, 65)

void Func_0200369c(void) {
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x204;
  Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 32;
  Func_02001fdc();
  if (GameFlag_Get(0x86c) != 0) {
    MapGrid_CopyCellPictureIndicesRect(3, 8, 30, 33, 1, 1);
    MapGrid_CopyCellPictureIndicesRect(67, 3, 94, 28, 1, 5);
    MapGrid_CopyCellPictureIndicesRect(3, 69, 30, 93, 1, 4);
  }
  {
    s32 entrance = *(u16 *)&gMainRoutineWork.entrance;

    if ((u16)(entrance - 11) <= 28) {
      Func_02001804();
      entrance <<= 16;
      entrance >>= 16;
      entrance += 789;
      GameFlag_Set(entrance);
    }
  }
  Func_0200353c();
  if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0) {
    if (gMainRoutineWork.entrance > 10) {
      if (gMainRoutineWork.entrance <= 39)
        Func_02003764();
    }
  }
}

void Func_02003764(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct EffectOptions options;
  u32 i;
  struct EffectOptions *opts;
  s32 spawn;

  Event_Start();
  Event_ResolvePendingActions(0);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Map_CenterOnFollowedPoint();
  wait(1);
  leader->y = 0x820000;
  leader->gravity = 0x8000;
  leader->bounce = 0;
  leader->motionFlags = 0;
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x100;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Sound_Play(204);
  leader->motionFlags = 3;
  Event_Wait(24);
  opts = &options;
  opts->palette = 7;
  opts->update = Effect_AdvanceMotion__s2698;
  opts->startScaleX = 0xa3d7;
  opts->startScaleY = 0xa3d7;
  opts = &options;
  spawn = 1;
  i = 0;
loop:
  {
    s32 angle;
    s32 velocity[3];
    s32 cosine;

    angle = i << 12;
    cosine = Trig_Cos(angle);
    if (spawn) {
      velocity[0] = cosine;
      velocity[1] = 0;
      velocity[2] = Trig_Sin(angle);
      velocity[0] += velocity[0] >> 1;
      velocity[0] += ((Random16() * 3) << 14) >> 16;
      velocity[0] += -0x6000;
      velocity[2] += (((Random16() * 3) << 13) >> 16) + -0x3000;
      FieldEffect_Spawn(leader->x, leader->y, leader->z, velocity[0],
                        velocity[1], velocity[2], 0x01090001, opts);
    }
    i++;
  }
  if (i <= 16)
    goto loop;
  Sound_Play(188);
  FieldEffect_SetAttached(gMainRoutineWork.currentOwner, 0x101);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 49);
  MapGrid_SetShake(0x50000, 0x50000, 0x10000);
  MapGrid_SetShake(-1, -1, 0xe666);
  MapGrid_WaitForShakeToSettle();
  FieldEffect_SetAttached(gMainRoutineWork.currentOwner, 0x100);
  leader->gravity = 0x10000;
  leader->bounce = 0x4000;
  Event_Wait(10);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 1);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x204;
  Event_End();
}

// ---- games/tla/src/scene/dehkan_plateau/s390c.c ----

void Func_0200390c(void) {
  struct Actor *owner = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 entrance;

  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x204;
  owner->priorityFlags |= 32;
  if (GameFlag_Get(0x878) == 0) {
    Func_02003a30();
    if (GameFlag_Get(0x871) != 0) {
      Map_CopyAttributes(0, 0, 1, 1, 15, 5);
      if (gData_02004420 != NULL) {
        Actor_PlaceAndResetMotion(gData_02004420, 248 << 16, 0, 176 << 15);
        wait(1);
      }
    }
  }
  Pillars_Place(gData_02003fb0);
  Func_020019b8();
  entrance = gMainRoutineWork.entrance;
  if (entrance > 10) {
    Func_02001804();
    GameFlag_Set(entrance + 789);
  }
  if (Flag_Test(0x871) != 0)
    Actor_SetHorizontalPositionWithTerrain(9, 0, 0);
  Func_020035ec();
  if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0) {
    if (gMainRoutineWork.entrance > 16) {
      if (gMainRoutineWork.entrance == 37 && GameFlag_Get(0x871) == 0)
        Func_02002c14();
      else
        Func_02003764();
    }
  }
  gData_0200567c = 0;
  gData_02005678 = 0;
  Task_Add(Func_02001ee8, TASK_PASS_VBLANK);
}

// ---- games/tla/src/scene/dehkan_plateau/s3a30.c ----
// Scene code of overlay 0x655, in ROM order, from s3a30's offset up to the next
// object the overlay links.

void Func_02003a30(void) {
  struct Actor **slot = &gData_02004420;
  s32 zero = 0;
  struct Actor *object = ((struct Actor *)Import_ActorCreate(489, 0, 0, 0));

  *slot = object;
  if (object != 0) {
    struct FieldSprite *sprite = object->sprite;
    u8 *buffer;

    sprite->fullColor = 0;
    sprite->palette = 0;
    object->motionFlags = zero;
    object->unk5c[0] = 1;
    sprite->nextFrame = zero;
    sprite->unk1b[0] = zero;
    buffer = HeapSlot_Alloc(68, 0x608);
    Icon_DrawItem(199);
    VramCache_Load(sprite->vramBlock, 128, buffer + 0x400);
    HeapSlot_Release(68);
  }
}

void Func_02003aa0(void) {
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x204;
  Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 32;
  if (GameFlag_Get(95) != 0)
    GameFlag_Set(521);
  Pillars_Place(gData_02003fb8);
  CrumblingPillars_Place(gData_02003f92);
  if (GameFlag_Get(519) == 0) {
    Func_02002140();
    wait(10);
  }
  Func_02001ff4();
  if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0) {
    if (gMainRoutineWork.entrance > 5) {
      if (gMainRoutineWork.entrance <= 39)
        Func_02003764();
    }
  }
}

s32 Scene_PrepareMap(void) { return 0; }

#undef DEFINE_DEHKAN_PLATEAU_PLACE_BY_FLAG
#undef gData_02004420
#undef gData_02003f14
#undef gData_02003f44
#undef gData_02003f52
#undef gData_02003f80
#undef gData_02003f92
#undef gData_02003f98
#undef gData_02003fa0
#undef gData_02003fa8
#undef gData_02003fb0
#undef gData_02003fb8
#undef gData_02003fc0
#undef gData_02003fc4
#undef CrumblingPillars_Step
#undef CrumblingPillars_StepWithStandIn
#undef gData_02003fd2
#undef gData_02004066
#undef Pillars_Place
#undef CrumblingPillars_PlaceWithStandIns
#undef gData_02003fc8
#undef Pillars_Raise
#undef gData_02004276
#undef Actor_PlaceFacing
// ---- from scene/data/dehkan_plateau.c ----
#include "types.h"
#include "scene_tables.h"
#include "module/scene/dehkan_plateau.h"

/* VIEW(data): routine Func_02001554 as this part declared it; the joined file
 * has another. */
void Func_02001554__data(void) __asm__("Func_02001554");
#define Func_02001554 Func_02001554__data
/* VIEW(data): routine Func_02001568 as this part declared it; the joined file
 * has another. */
void Func_02001568__data(void) __asm__("Func_02001568");
#define Func_02001568 Func_02001568__data
/* VIEW(data): routine Func_0200157c as this part declared it; the joined file
 * has another. */
void Func_0200157c__data(void) __asm__("Func_0200157c");
#define Func_0200157c Func_0200157c__data
/* VIEW(data): routine Func_02001590 as this part declared it; the joined file
 * has another. */
void Func_02001590__data(void) __asm__("Func_02001590");
#define Func_02001590 Func_02001590__data
/* VIEW(data): routine Func_020015a4 as this part declared it; the joined file
 * has another. */
void Func_020015a4__data(void) __asm__("Func_020015a4");
#define Func_020015a4 Func_020015a4__data
void Func_020016b0(void);
void Func_02001788(void);
void Func_02002180(void);
void Func_020022e0(void);

extern u32 gData_02003ed8[];

// clang-format off
u32 gData_02003e00[] = {
    0, 33, 22, 38, 0, 2,
    0, 0, 0, 0, 1, 38,
};

u32 gData_02003e30[] = {
    0, 44, 22, 38, 0, 2,
    0, 0, 0, 0, 1, 38,
};

u32 gData_02003e60[] = {
    0, 1, 13, 22, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gData_02003e9c[] = {
    0, 1, 13, 44, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gData_02003ed8[] = {
    0, 1, 13, 126, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gData_02003f14[] = {
    0, 33, 22, 38, 0, 3,
    0, 1, 38,
};

const s32 *gFieldEffect_DurationScripts[] = {
    gData_02003e60,
    gData_02003e9c,
    gData_02003ed8,
};

s16 gData_02003f44[] = {
    8, 528, 9, 530, 10, 532, -1,
};

s16 gData_02003f52[] = {
    11, 528, 12, 530, 13, 532, 14, 534,
    15, 536, 16, 538, 17, 540, 18, 542,
    19, 544, 20, 546, 21, 548, -1,
};

s16 gData_02003f80[] = {
    21, 528, 22, 530, 23, 532, 24, 534,
    -1,
};

s16 gData_02003f92[] = {
    9, 528, -1,
};

s16 gData_02003f98[] = {
    11, 1, 515, -1,
};

s16 gData_02003fa0[] = {
    22, 1, 516, -1,
};

s16 gData_02003fa8[] = {
    18, 1, 517, -1,
};

s16 gData_02003fb0[] = {
    8, 1, 518, -1,
};

s16 gData_02003fb8[] = {
    8, 1, 519, -1,
};

s16 gData_02003fc0[] = {
    23, -1,
};

s16 gData_02003fc4[] = {
    19, -1,
};

s16 gData_02003fc8[] = {
    14, 15, 16, 17, -1,
};

u16 gData_02003fd2[] = {
    4, 33, 5, 33,
};

u16 gData_02003fda[] = {
    68, 33, 69, 33,
};

u16 gData_02003fe2[] = {
    69, 51, 70, 51,
};

u16 gData_02003fea[] = {
    65, 64, 64, 64,
};

u16 gData_02003ff2[] = {
    1, 36, 2, 36, 3, 36, 4, 36,
    5, 36, 6, 36, 7, 36, 8, 36,
};

u16 gData_02004012[] = {
    65, 36, 66, 36, 67, 36, 68, 36,
    69, 36, 70, 36, 71, 36, 72, 36,
};

u16 gData_02004032[] = {
    66, 54, 67, 54, 68, 54, 69, 54,
    70, 54, 71, 54, 72, 54, 73, 54,
};

u16 gData_02004052[] = {
    64, 66, 65, 66, 66, 66, 67, 66,
    68, 66,
};

u16 gData_02004066[] = {
    0,
};

u16 gData_02004068[] = {
    76, 12, 0, 76, 48, 0, 77, 13,
    0, 77, 49, 1, 79, 12, 0, 79,
    48, 0, 79, 14, 1, 79, 50, 0,
    81, 12, 0, 81, 48, 0, 118, 17,
    4, 118, 53, 1, 119, 16, 0, 119,
    52, 0, 119, 18, 7, 119, 54, 1,
    120, 12, 7, 120, 48, 0, 120, 14,
    7, 120, 50, 0, 120, 17, 2, 120,
    53, 0, 122, 12, 7, 122, 48, 1,
    122, 16, 0, 122, 52, 0, 122, 18,
    5, 122, 54,
};

u16 gData_0200410e[] = {
    1,
};

u16 gData_02004110[] = {
    105, 4, 0, 41, 36, 0, 109, 17,
    0, 45, 49, 0, 110, 19, 3, 46,
    51, 0, 111, 17, 3, 47, 49, 0,
    112, 21, 0, 48, 53, 0, 113, 20,
    3, 49, 52, 0, 114, 18, 0, 50,
    50, 0, 105, 6, 0, 41, 38,
};

u16 gData_0200416e[] = {
    1,
};

u16 gData_02004170[] = {
    91, 11, 0, 27, 43, 0, 92, 11,
    0, 28, 43, 1, 93, 9, 0, 29,
    41, 1, 93, 12, 0, 29, 44, 1,
    109, 7, 0, 45, 39, 1, 112, 6,
    0, 48, 38,
};

u16 gData_020041b6[] = {
    1,
};

u16 gData_020041b8[] = {
    13, 8, 2, 13, 72, 0, 13, 9,
    0, 13, 73, 0, 13, 11, 1, 13,
    75, 0, 13, 6, 0, 13, 70, 1,
    15, 6, 0, 15, 70, 0, 15, 8,
    0, 15, 72, 0, 15, 10, 1, 15,
    74, 1, 14, 10, 0, 14, 74, 0,
    14, 12, 0, 14, 76, 1, 15, 12,
    0, 15, 76, 0, 16, 6, 0, 16,
    70, 1, 16, 8, 2, 16, 72, 0,
    16, 9, 0, 16, 73, 0, 16, 11,
    0, 16, 75, 1, 18, 9, 0, 18,
    73, 0, 18, 11, 0, 18, 75,
};

u16 gData_02004276[] = {
    4, 7, 68, 2, 5, 8, 69, 3,
    7, 7, 71, 2, 7, 9, 71, 4,
    9, 7, 73, 2, 40, 12, 104, 7,
    41, 11, 105, 6, 41, 13, 105, 8,
    42, 5, 106, 0, 42, 9, 106, 4,
    42, 12, 106, 7, 44, 7, 108, 2,
    44, 11, 108, 6, 44, 13, 108, 8,
    11, 23, 75, 18, 12, 25, 76, 20,
    13, 23, 77, 18, 14, 27, 78, 22,
    15, 26, 79, 21, 16, 24, 80, 19,
    30, 35, 94, 30, 26, 9, 90, 4,
    27, 9, 91, 4, 28, 7, 92, 2,
    28, 10, 92, 5, 12, 8, 76, 3,
    15, 7, 79, 2, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 30, 33, 94, 28,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,
};

u32 gData_02004420[] = {
    0,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 752, 0, 348, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(34),
    SCENE_EXIT(1, 2, 6),
    SCENE_EXIT(2, 37, 1),
    SCENE_EXIT(3, 37, 2),
    SCENE_EXIT(4, 37, 4),
    SCENE_EXIT(5, 35, 1),
    SCENE_EXIT(6, 34, 7),
    SCENE_EXIT(7, 34, 6),
    SCENE_EXIT(11, 37, 11),
    SCENE_EXIT(12, 37, 12),
    SCENE_EXIT(13, 37, 13),
    SCENE_EXIT(14, 37, 14),
    SCENE_EXIT(15, 37, 15),
    SCENE_EXIT(16, 37, 16),
    SCENE_EXIT(17, 37, 17),
    SCENE_EXIT(18, 37, 18),
    SCENE_EXIT(19, 37, 19),
    SCENE_EXIT(20, 37, 20),
    SCENE_EXIT(21, 37, 21),
    SCENE_EXIT(22, 37, 22),
    SCENE_EXIT(23, 37, 23),
    SCENE_EXIT(24, 37, 24),
    SCENE_EXITS(35),
    SCENE_EXIT(1, 34, 5),
    SCENE_EXIT(2, 37, 6),
    SCENE_EXIT(3, 37, 5),
    SCENE_EXIT(4, 36, 1),
    SCENE_EXIT(5, 35, 6),
    SCENE_EXIT(6, 35, 5),
    SCENE_EXIT(15, 37, 25),
    SCENE_EXIT(16, 37, 26),
    SCENE_EXIT(17, 37, 27),
    SCENE_EXIT(18, 37, 28),
    SCENE_EXIT(19, 37, 29),
    SCENE_EXIT(20, 37, 30),
    SCENE_EXIT(21, 37, 31),
    SCENE_EXIT(14, 37, 60),
    SCENE_EXITS(36),
    SCENE_EXIT(1, 35, 4),
    SCENE_EXIT(2, 37, 3),
    SCENE_EXIT(3, 38, 1),
    SCENE_EXIT(4, 38, 4),
    SCENE_EXIT(5, 38, 5),
    SCENE_EXIT(6, 2, 7),
    SCENE_EXIT(7, 36, 8),
    SCENE_EXIT(8, 36, 7),
    SCENE_EXIT(12, 37, 32),
    SCENE_EXIT(13, 37, 33),
    SCENE_EXIT(14, 37, 34),
    SCENE_EXIT(15, 37, 35),
    SCENE_EXIT(16, 38, 36),
    SCENE_EXIT(17, 38, 37),
    SCENE_EXITS(37),
    SCENE_EXIT(1, 34, 2),
    SCENE_EXIT(2, 34, 3),
    SCENE_EXIT(3, 36, 2),
    SCENE_EXIT(4, 34, 4),
    SCENE_EXIT(5, 35, 3),
    SCENE_EXIT(6, 35, 2),
    SCENE_EXIT(40, 37, 42),
    SCENE_EXIT(41, 37, 43),
    SCENE_EXIT(42, 37, 40),
    SCENE_EXIT(43, 37, 41),
    SCENE_EXITS(38),
    SCENE_EXIT(1, 36, 3),
    SCENE_EXIT(2, 39, 1),
    SCENE_EXIT(3, 39, 2),
    SCENE_EXIT(4, 36, 4),
    SCENE_EXIT(5, 36, 5),
    SCENE_EXIT(6, 39, 3),
    SCENE_EXIT(7, 38, 10),
    SCENE_EXIT(8, 38, 9),
    SCENE_EXIT(9, 38, 8),
    SCENE_EXIT(10, 38, 7),
    SCENE_EXIT(18, 39, 8),
    SCENE_EXIT(19, 39, 9),
    SCENE_EXIT(20, 39, 10),
    SCENE_EXIT(21, 39, 11),
    SCENE_EXIT(22, 39, 12),
    SCENE_EXIT(23, 39, 13),
    SCENE_EXIT(24, 39, 14),
    SCENE_EXIT(25, 39, 15),
    SCENE_EXIT(26, 39, 16),
    SCENE_EXIT(27, 39, 17),
    SCENE_EXIT(28, 39, 18),
    SCENE_EXIT(29, 39, 19),
    SCENE_EXIT(30, 39, 20),
    SCENE_EXIT(31, 39, 21),
    SCENE_EXIT(32, 39, 22),
    SCENE_EXIT(33, 39, 23),
    SCENE_EXITS(39),
    SCENE_EXIT(1, 38, 2),
    SCENE_EXIT(2, 38, 3),
    SCENE_EXIT(3, 38, 6),
    SCENE_EXIT(40, 39, 41),
    SCENE_EXIT(41, 39, 40),
    SCENE_EXIT(42, 39, 43),
    SCENE_EXIT(43, 39, 42),
    SCENE_EXITS_END,
};

struct ActorPlacement gData_020045e0[] = {
    {SCENE_TABLE_END},
};

s16 gData_020045f8[] = {
    409, -1, 1, 0, 0, 632, 0, 0,
    0, 232, FACING_SOUTH, 2, 409, -1, 1, 0,
    0, 616, 0, 0, 0, 280, FACING_SOUTH, 2,
    409, -1, 1, 0, 0, 696, 0, 0,
    0, 296, FACING_SOUTH, 2, 406, -1, 1, 0,
    0, 792, 0, 0, 0, 232, FACING_SOUTH, 2,
    272, -1, 1, 0, 0, 920, 0, 0,
    0, 520, FACING_SOUTH, 2, 337, -1, 1, 0,
    0, 728, 0, 0, 0, 376, FACING_SOUTH, 2,
    337, -1, 1, 0, 0, 728, 0, 0,
    0, 360, FACING_SOUTH, 258, 337, -1, 1, 0,
    0, 744, 0, 0, 0, 360, FACING_SOUTH, 258,
    337, -1, 1, 0, 0, 744, 0, 0,
    0, 344, FACING_SOUTH, 258, 337, -1, 1, 0,
    0, 712, 0, 0, 0, 376, FACING_SOUTH, 258,
    337, -1, 1, 0, 0, 696, 0, 0,
    0, 376, FACING_SOUTH, 258, 337, -1, 1, 0,
    0, 680, 0, 0, 0, 376, FACING_SOUTH, 258,
    337, -1, 1, 0, 0, 664, 0, 0,
    0, 376, FACING_SOUTH, 258, 337, -1, 1, 0,
    0, 664, 0, 0, 0, 392, FACING_SOUTH, 258,
    337, -1, 1, 0, 0, 656, 0, 0,
    0, 392, FACING_SOUTH, 258, 337, -1, 1, 0,
    0, 648, 0, 0, 0, 392, FACING_SOUTH, 258,
    337, -1, 1, 0, 0, 632, 0, 0,
    0, 392, FACING_SOUTH, 258, 337, -1, 1, 0,
    0, 632, 0, 0, 0, 408, FACING_SOUTH, 258,
    337, -1, 1, 0, 0, 616, 0, 0,
    0, 408, FACING_SOUTH, 258, -1, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
};

struct ActorPlacement gData_020047d8[] = {
    {409, CONDITION_ALWAYS, 1, PIXELS(10), 0, PIXELS(10), FACING_SOUTH, 2, 0},
    {409, CONDITION_ALWAYS, 1, PIXELS(10), 0, PIXELS(10), FACING_SOUTH, 2, 0},
    {409, CONDITION_ALWAYS, 1, PIXELS(10), 0, PIXELS(10), FACING_SOUTH, 2, 0},
    {409, CONDITION_ALWAYS, 1, 0x1, 0, 0, FACING_SOUTH, 2, 3},
    {409, CONDITION_ALWAYS, 1, 0x2, 0, 0, FACING_SOUTH, 2, 3},
    {409, CONDITION_ALWAYS, 1, 0x3, 0, 0, FACING_SOUTH, 2, 3},
    {409, CONDITION_ALWAYS, 1, 0x4, 0, 0, FACING_SOUTH, 2, 3},
    {409, CONDITION_ALWAYS, 1, 0x5, 0, 0, FACING_SOUTH, 2, 3},
    {409, CONDITION_ALWAYS, 1, 0x6, 0, 0, FACING_SOUTH, 2, 3},
    {409, CONDITION_ALWAYS, 1, 0x7, 0, 0, FACING_SOUTH, 2, 3},
    {409, CONDITION_ALWAYS, 1, 0x8, 0, 0, FACING_SOUTH, 2, 3},
    {409, CONDITION_ALWAYS, 1, 0x9, 0, 0, FACING_SOUTH, 2, 3},
    {409, CONDITION_ALWAYS, 1, 0xa, 0, 0, FACING_SOUTH, 2, 3},
    {409, CONDITION_ALWAYS, 1, 0xb, 0, 0, FACING_SOUTH, 2, 3},
    {406, CONDITION_ALWAYS, 1, 0xc, 0, 0, FACING_SOUTH, 2, 2},
    {272, CONDITION_ALWAYS, 1, 0xd, 0, 0, FACING_SOUTH, 2, 2},
    {245, CONDITION_ALWAYS, 1, PIXELS(664), 0, PIXELS(168), FACING_SOUTH, 2, 0},
    {337, CONDITION_ALWAYS, 1, PIXELS(328), 0, PIXELS(488), FACING_SOUTH, 2, 0},
    {337, CONDITION_ALWAYS, 1, PIXELS(312), 0, PIXELS(488), FACING_SOUTH, 2, 1},
    {337, CONDITION_ALWAYS, 1, PIXELS(296), 0, PIXELS(488), FACING_SOUTH, 2, 1},
    {337, CONDITION_ALWAYS, 1, PIXELS(296), 0, PIXELS(504), FACING_SOUTH, 2, 1},
    {337, CONDITION_ALWAYS, 1, PIXELS(760), 0, PIXELS(280), FACING_SOUTH, 2, 1},
    {337, CONDITION_ALWAYS, 1, PIXELS(776), 0, PIXELS(280), FACING_SOUTH, 2, 1},
    {337, CONDITION_ALWAYS, 1, PIXELS(776), 0, PIXELS(296), FACING_SOUTH, 2, 1},
    {337, CONDITION_ALWAYS, 1, PIXELS(792), 0, PIXELS(296), FACING_SOUTH, 2, 1},
    {337, CONDITION_ALWAYS, 1, PIXELS(808), 0, PIXELS(296), FACING_SOUTH, 2, 1},
    {337, CONDITION_ALWAYS, 1, PIXELS(808), 0, PIXELS(312), FACING_SOUTH, 2, 1},
    {337, CONDITION_ALWAYS, 1, PIXELS(824), 0, PIXELS(312), FACING_SOUTH, 2, 1},
    {337, CONDITION_ALWAYS, 1, PIXELS(840), 0, PIXELS(312), FACING_SOUTH, 2, 1},
    {337, CONDITION_ALWAYS, 1, PIXELS(840), 0, PIXELS(328), FACING_SOUTH, 2, 1},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_02004ac0[] = {
    {407, CONDITION_ALWAYS, 1, 0x1, 0, 0, FACING_SOUTH, 2, 2},
    {407, CONDITION_ALWAYS, 1, 0x2, 0, 0, FACING_SOUTH, 2, 3},
    {407, CONDITION_ALWAYS, 1, 0x3, 0, 0, FACING_SOUTH, 2, 3},
    {407, CONDITION_ALWAYS, 1, 0x4, 0, 0, FACING_SOUTH, 2, 3},
    {407, CONDITION_ALWAYS, 1, 0x5, 0, 0, FACING_SOUTH, 2, 2},
    {407, CONDITION_ALWAYS, 1, 0x6, 0, 0, FACING_SOUTH, 2, 3},
    {250, CONDITION_ALWAYS, 1, 0x7, 0, 0, FACING_SOUTH, 2, 2},
    {250, CONDITION_ALWAYS, 1, 0x8, 0, 0, FACING_SOUTH, 2, 3},
    {250, CONDITION_ALWAYS, 1, 0x9, 0, 0, FACING_SOUTH, 2, 3},
    {250, CONDITION_ALWAYS, 1, 0xa, 0, 0, FACING_SOUTH, 2, 3},
    {406, CONDITION_ALWAYS, 1, 0xb, 0, 0, FACING_SOUTH, 2, 2},
    {272, CONDITION_ALWAYS, 1, 0xc, 0, 0, FACING_SOUTH, 2, 2},
    {272, CONDITION_ALWAYS, 1, 0xd, 0, 0, FACING_SOUTH, 2, 3},
    {409, CONDITION_ALWAYS, 1, 0xe, 0, 0, FACING_SOUTH, 2, 2},
    {409, CONDITION_ALWAYS, 1, 0xf, 0, 0, FACING_SOUTH, 2, 2},
    {409, CONDITION_ALWAYS, 1, 0x10, 0, 0, FACING_SOUTH, 2, 2},
    {409, CONDITION_ALWAYS, 1, PIXELS(744), 0, PIXELS(168), FACING_SOUTH, 2, 0},
    {245, CONDITION_ALWAYS, 1, PIXELS(152), 0, PIXELS(104), FACING_SOUTH, 2, 0},
    {337, CONDITION_ALWAYS, 1, PIXELS(120), 0, PIXELS(72), FACING_SOUTH, 2, 0},
    {337, CONDITION_ALWAYS, 1, PIXELS(408), 0, PIXELS(72), FACING_SOUTH, 2, 1},
    {337, CONDITION_ALWAYS, 1, PIXELS(584), 0, PIXELS(376), FACING_SOUTH, 2, 1},
    {337, CONDITION_ALWAYS, 1, PIXELS(600), 0, PIXELS(376), FACING_SOUTH, 2, 1},
    {337, CONDITION_ALWAYS, 1, PIXELS(600), 0, PIXELS(392), FACING_SOUTH, 2, 1},
    {337, CONDITION_ALWAYS, 1, PIXELS(600), 0, PIXELS(408), FACING_SOUTH, 2, 1},
    {337, CONDITION_ALWAYS, 1, PIXELS(616), 0, PIXELS(408), FACING_SOUTH, 2, 1},
    {337, CONDITION_ALWAYS, 1, PIXELS(616), 0, PIXELS(424), FACING_SOUTH, 2, 1},
    {337, CONDITION_ALWAYS, 1, PIXELS(632), 0, PIXELS(424), FACING_SOUTH, 2, 1},
    {337, CONDITION_ALWAYS, 1, PIXELS(920), 0, PIXELS(424), FACING_SOUTH, 2, 1},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_02004d78[] = {
    {337, CONDITION_ALWAYS, 1, PIXELS(760), 0, PIXELS(232), FACING_SOUTH, 2, 0},
    {337, CONDITION_ALWAYS, 1, PIXELS(760), 0, PIXELS(216), FACING_SOUTH, 2, 1},
    {337, CONDITION_ALWAYS, 1, PIXELS(776), 0, PIXELS(216), FACING_SOUTH, 2, 1},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_02004dd8[] = {
    {406, CONDITION_ALWAYS, 1, PIXELS(120), 0, PIXELS(536), FACING_SOUTH, 2, 0},
    {245, CONDITION_ALWAYS, 1, PIXELS(248), 0, PIXELS(120), FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_02004e20[] = {
    {406, CONDITION_ALWAYS, 1, PIXELS(760), 0, PIXELS(200), FACING_SOUTH, 2, 0},
    {409, CONDITION_ALWAYS, 1, PIXELS(792), 0, PIXELS(200), FACING_SOUTH, 2, 0},
    {337, CONDITION_ALWAYS, 1, PIXELS(88), 0, PIXELS(264), FACING_SOUTH, 2, 0},
    {337, CONDITION_ALWAYS, 1, PIXELS(104), 0, PIXELS(264), FACING_SOUTH, 2, 1},
    {337, CONDITION_ALWAYS, 1, PIXELS(104), 0, PIXELS(280), FACING_SOUTH, 2, 1},
    {337, CONDITION_ALWAYS, 1, PIXELS(872), 0, PIXELS(296), FACING_SOUTH, 2, 1},
    {337, CONDITION_ALWAYS, 1, PIXELS(872), 0, PIXELS(312), FACING_SOUTH, 2, 1},
    {337, CONDITION_ALWAYS, 1, PIXELS(888), 0, PIXELS(296), FACING_SOUTH, 2, 1},
    {245, 0x5f, 1, PIXELS(728), 0, PIXELS(200), FACING_SOUTH, 0, 0},
    {SCENE_TABLE_END},
};

struct Trigger gData_02004f10[] = {
    {TRIGGER_TABLE_END},
};

struct Trigger gData_02004f1c[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_STEP, 2, CONDITION_ALWAYS, EVENT(Func_02002ff4)},
    {0x31, 3, CONDITION_ALWAYS, 3},
    {0x31, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 30, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 31, CONDITION_ALWAYS, 7},
    {0x202, 9, CONDITION_ALWAYS, EVENT(Func_020012c8)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, 5), 11, 0x203, EVENT(Func_02001554)},
    {0x9, 0, 0x301, EVENT(Func_020015e0)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 12, 0x301, EVENT(Func_020015e0)},
    {TRIGGER_STEP, 6, 0x200, EVENT(Func_020014ec)},
    {TRIGGER_STEP, 7, IF_FLAG_SET(0x200), EVENT(Func_02001500)},
    {TRIGGER_STEP, 10, CONDITION_ALWAYS, EVENT(Func_020017f4)},
    {TRIGGER_STEP, 11, 0x201, EVENT(Func_02001a30)},
    {TRIGGER_STEP, 12, 0x201, EVENT(Func_02001a30)},
    {TRIGGER_STEP, 13, 0x201, EVENT(Func_02001a30)},
    {TRIGGER_STEP, 14, 0x201, EVENT(Func_02001a30)},
    {TRIGGER_STEP, 15, 0x201, EVENT(Func_02001a30)},
    {TRIGGER_STEP, 16, 0x201, EVENT(Func_02001a30)},
    {TRIGGER_STEP, 17, 0x201, EVENT(Func_02001a30)},
    {TRIGGER_STEP, 18, 0x201, EVENT(Func_02001a30)},
    {TRIGGER_STEP, 19, 0x201, EVENT(Func_02001a30)},
    {TRIGGER_STEP, 20, 0x201, EVENT(Func_02001a30)},
    {TRIGGER_STEP, 21, 0x201, EVENT(Func_02001a30)},
    {TRIGGER_STEP, 22, 0x201, EVENT(Func_02001a30)},
    {TRIGGER_STEP, 23, 0x201, EVENT(Func_02001a30)},
    {TRIGGER_STEP, 24, 0x201, EVENT(Func_02001a30)},
    {TRIGGER_TABLE_END},
};

struct Trigger gData_02005078[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {0x31, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_STEP, 3, CONDITION_ALWAYS, EVENT(Func_02002ff4)},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 6},
    {TRIGGER_STEP, 30, 0x86c, EVENT(Func_020021e8)},
    {TRIGGER_STEP, 31, 0x86d, EVENT(Func_020022e0)},
    {0x8602, 8, CONDITION_ALWAYS, EVENT(Func_02001368)},
    {0x202, 9, CONDITION_ALWAYS, EVENT(Func_02001368)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, 5), 22, 0x204, EVENT(Func_02001568)},
    {0x8, 0, 0x840, EVENT(Func_02001630)},
    {0x9, 0, 0x840, EVENT(Func_02001640)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, 1), 23, 0x840, EVENT(Func_02001630)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 23, 0x840, EVENT(Func_02001640)},
    {TRIGGER_STEP, 6, 0x200, EVENT(Func_020014ec)},
    {TRIGGER_STEP, 7, IF_FLAG_SET(0x200), EVENT(Func_02001500)},
    {TRIGGER_STEP, 10, CONDITION_ALWAYS, EVENT(Func_020017f4)},
    {TRIGGER_STEP, 14, 0x201, EVENT(Func_02001a30)},
    {TRIGGER_STEP, 15, 0x201, EVENT(Func_02001a30)},
    {TRIGGER_STEP, 16, 0x201, EVENT(Func_02001a30)},
    {TRIGGER_STEP, 17, 0x201, EVENT(Func_02001a30)},
    {TRIGGER_STEP, 18, 0x201, EVENT(Func_02001a30)},
    {TRIGGER_STEP, 19, 0x201, EVENT(Func_02001a30)},
    {TRIGGER_STEP, 20, 0x201, EVENT(Func_02001a30)},
    {TRIGGER_STEP, 21, 0x201, EVENT(Func_02001a30)},
    {TRIGGER_TABLE_END},
};

struct Trigger gData_020051bc[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {0x31, 2, CONDITION_ALWAYS, 2},
    {0x31, 3, CONDITION_ALWAYS, 3},
    {0x31, 4, CONDITION_ALWAYS, 4},
    {0x31, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 24, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 25, CONDITION_ALWAYS, 8},
    {TRIGGER_STEP, 30, 0x86e, EVENT(Func_02002390)},
    {TRIGGER_STEP, 31, 0x86f, EVENT(Func_02002458)},
    {TRIGGER_STEP, 32, 0x870, EVENT(Func_020027e0)},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), 12, 0x208, 0},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, 5), 18, 0x205, EVENT(Func_0200157c)},
    {0x8, 0, CONDITION_ALWAYS, EVENT(Func_020016a0)},
    {0x9, 0, CONDITION_ALWAYS, EVENT(Func_020016b0)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, 1), 19, CONDITION_ALWAYS, EVENT(Func_020016a0)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 19, CONDITION_ALWAYS, EVENT(Func_020016b0)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 20, 0x841, EVENT(Func_02001788)},
    {0x8602, 9, 0x841, EVENT(Func_020017a0)},
    {0x602, 20, CONDITION_ALWAYS, EVENT(Scene_RunIfHeld)},
    {0x8602, 21, CONDITION_ALWAYS, EVENT(Scene_RunIfHeld)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, 1), 14, CONDITION_ALWAYS, EVENT(Func_02002114)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, 1), 15, CONDITION_ALWAYS, EVENT(Func_02002114)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, 1), 16, CONDITION_ALWAYS, EVENT(Func_02002114)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, 1), 17, CONDITION_ALWAYS, EVENT(Func_02002114)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 14, CONDITION_ALWAYS, EVENT(Func_02002124)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 15, CONDITION_ALWAYS, EVENT(Func_02002124)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 16, CONDITION_ALWAYS, EVENT(Func_02002124)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 17, CONDITION_ALWAYS, EVENT(Func_02002124)},
    {0x202, 10, CONDITION_ALWAYS, EVENT(Func_020013fc)},
    {0x8602, 11, CONDITION_ALWAYS, EVENT(Func_020013fc)},
    {0x602, 26, CONDITION_ALWAYS, EVENT(Func_020013fc)},
    {0x4602, 27, CONDITION_ALWAYS, EVENT(Func_020013fc)},
    {TRIGGER_STEP, 7, 0x200, EVENT(Func_020014ec)},
    {TRIGGER_STEP, 8, IF_FLAG_SET(0x200), EVENT(Func_02001500)},
    {TRIGGER_STEP, 12, 0x201, EVENT(Func_02001a30)},
    {TRIGGER_STEP, 13, 0x201, EVENT(Func_02001a30)},
    {TRIGGER_STEP, 14, 0x201, EVENT(Func_02001a30)},
    {TRIGGER_STEP, 15, 0x201, EVENT(Func_02001a30)},
    {TRIGGER_STEP, 16, 0x201, EVENT(Func_02001a30)},
    {TRIGGER_STEP, 17, 0x201, EVENT(Func_02001a30)},
    {TRIGGER_TABLE_END},
};

struct Trigger gData_020053b4[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {0x21, 2, CONDITION_ALWAYS, 2},
    {0x21, 3, CONDITION_ALWAYS, 3},
    {0x21, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {0x21, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 40, CONDITION_ALWAYS, 40},
    {TRIGGER_EXIT, 41, CONDITION_ALWAYS, 41},
    {TRIGGER_EXIT, 42, CONDITION_ALWAYS, 42},
    {TRIGGER_EXIT, 43, CONDITION_ALWAYS, 43},
    {TRIGGER_STEP, 8, 0x200, EVENT(Func_020014ec)},
    {TRIGGER_STEP, 9, IF_FLAG_SET(0x200), EVENT(Func_02001500)},
    {TRIGGER_TABLE_END},
};

struct Trigger gData_02005450[] = {
    {0x21, 1, CONDITION_ALWAYS, 1},
    {0x31, 2, CONDITION_ALWAYS, 2},
    {0x31, 3, CONDITION_ALWAYS, 3},
    {0x21, 4, CONDITION_ALWAYS, 4},
    {0x21, 5, CONDITION_ALWAYS, 5},
    {0x31, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_STEP, 23, 0x871, EVENT(Func_02002a5c)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, 5), 8, 0x206, EVENT(Func_02001590)},
    {TRIGGER_EXAMINE, 12, 0x878, EVENT(Func_02002f9c)},
    {TRIGGER_STEP, 18, 0x201, EVENT(Func_02001a30)},
    {TRIGGER_STEP, 19, 0x201, EVENT(Func_02001a30)},
    {TRIGGER_STEP, 20, 0x201, EVENT(Func_02001a30)},
    {TRIGGER_STEP, 21, 0x201, EVENT(Func_02001a30)},
    {TRIGGER_STEP, 22, 0x201, EVENT(Func_02001a30)},
    {TRIGGER_STEP, 23, 0x201, EVENT(Func_02001a30)},
    {TRIGGER_STEP, 24, 0x201, EVENT(Func_02001a30)},
    {TRIGGER_STEP, 25, 0x201, EVENT(Func_02001a30)},
    {TRIGGER_STEP, 26, 0x201, EVENT(Func_02001a30)},
    {TRIGGER_STEP, 27, 0x201, EVENT(Func_02001a30)},
    {TRIGGER_STEP, 28, 0x201, EVENT(Func_02001a30)},
    {TRIGGER_STEP, 29, 0x201, EVENT(Func_02001a30)},
    {TRIGGER_STEP, 30, 0x201, EVENT(Func_02001a30)},
    {TRIGGER_STEP, 31, 0x201, EVENT(Func_02001a30)},
    {TRIGGER_STEP, 32, 0x201, EVENT(Func_02001a30)},
    {TRIGGER_STEP, 33, 0x201, EVENT(Func_02001a30)},
    {TRIGGER_TABLE_END},
};

struct Trigger gData_020055b8[] = {
    {0x21, 1, CONDITION_ALWAYS, 1},
    {0x21, 2, CONDITION_ALWAYS, 2},
    {0x21, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 40, CONDITION_ALWAYS, 40},
    {TRIGGER_EXIT, 41, CONDITION_ALWAYS, 41},
    {TRIGGER_EXIT, 42, CONDITION_ALWAYS, 42},
    {TRIGGER_EXIT, 43, CONDITION_ALWAYS, 43},
    {0x202, 8, CONDITION_ALWAYS, EVENT(Func_02001444)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, 5), 8, 0x207, EVENT(Func_020015a4)},
    {TRIGGER_STEP, 9, 0x202, EVENT(Func_02002140)},
    {TRIGGER_STEP, 10, IF_FLAG_SET(0x202), EVENT(Func_02002180)},
    {TRIGGER_STEP, 5, 0x200, EVENT(Func_020014ec)},
    {TRIGGER_STEP, 6, IF_FLAG_SET(0x200), EVENT(Func_02001500)},
    {TRIGGER_STEP, 11, 0x209, EVENT(Func_02002f74)},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS, EVENT(Func_0200118c)},
    {TRIGGER_TABLE_END},
};
#undef Func_02001554
#undef Func_02001568
#undef Func_0200157c
#undef Func_02001590
#undef Func_020015a4
