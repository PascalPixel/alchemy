/// @file
/// Magma Rock's outside, the volcano north of Loho the party climbs to its
/// summit: the Moai statues Burst wakes, which fire magma rocks or lift the
/// leader on a column of flame, the cracked columns they break, the pillars
/// Move pushes and the crumbling ones, Growth's vines and the slides, and at
/// the top the Psynergy rock that blasts the way in.

#include "shared/map_cells.h"
#include "shared/actors.h"
#include "types.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "actor.h"
#include "event_inline.h"
#include "map.h"
#include "field.h"
#include "module/field/event_commands.h"
#include "module/scene/magma_rock.h"

struct EventObjects {
  u8 unk00[20];
  struct Actor *objects[64];
};

void MagmaRock_FightFury(void) { Field_RequestDjinnBattle(8, 56); }

DEFINE_POSITION_FLAT_DISTANCE(Position_FlatDistance)

extern u8 gMapShapeGrid[];

s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
s32 MapGrid_GetTerrainKind(s32 layer, s32 x, s32 z);
s32 Func_02003c24(s32 layer, s32 shape, s32 height);

DEFINE_MAP_CELL_SET_SHAPE(MagmaRock_SetCellShape, MapGrid_GetTerrainKind,
                          Func_02003c24)

#include "../field/map/cell_get.c"

#include "field.h"

DEFINE_ACTORS_RAISE_CELLS(MagmaRock_RaiseActorCells, MagmaRock_SetCellShape)

#include "battle.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "scene_tables.h"
#include "magma_rock/magma_rock.h"
#include "main_routine_work.h"
#include "scene_ids.h"

// For each id of a list ended by 0xffff: clears the object's mark and, on a
// cell with attribute bit 6, blocks the cells ahead of and behind it and sets
// the mark.
DEFINE_BLOCKS_MARK_CELLS(Blocks_MarkCells, MapCell_GetAttributes)

// Clears the object's mark and the cells ahead of and behind it that the mark
// blocked.

struct Object0200074c {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[0x4f];
  u8 marked;
};

DEFINE_BLOCK_CLEAR_CELLS(MagmaRock_ClearBlockCells)

// For each id of a list ended by 0xffff: settles the object on its ground,
// copies the cell under it, and blocks the cells ahead of and behind it on a
// cell with attribute bit 6.
#include "text/msg_ids.h"

/// For each id of a list ended by 0xffff: settles the object on its ground,
/// copies the cell under it, and blocks the cells ahead of and behind it on a
/// cell with attribute bit 6.
void MagmaRock_SettleBlocks(u16 *ids) {
  while (*ids != 0xffff) {
    struct Actor *object = Actor_Get(*ids);
    u8 *terrainId = &object->terrainId;
    s32 height = Map_GetTerrainHeight(*terrainId, object->x, object->z);
    u8 *blocked;
    struct MapWork *map;
    struct MapCell **cells;
    struct MapCell *cell;
    s32 x, z;

    object->unk4c =
        255 & MapCell_GetAttributes(0, object->x >> 20, object->z >> 20);
    object->motionFlags = 0;
    object->collisionFlags = 0;
    Actor_SetSpriteFlags(object, 0);
    MagmaRock_SetCellShape(*terrainId, object->x >> 20, object->z >> 20,
                           (height >> 19) + 4);
    blocked = (u8 *)object + 99;
    *blocked = 0;
    if (MapCell_GetAttributes(0, object->x >> 20, object->z >> 20) & 64) {
      map = Ram_HeapSlots->mapWork;
      if (map == NULL)
        return;
      cells = &map->layers[2].cells;
      z = (object->z - object->y) >> 20;
      x = object->x >> 20;
      cell = *cells;
      cell += x + ((z + 1) << 7);
      cell->collisionCode = 255;
      cell = *cells;
      cell += x + ((z - 1) << 7);
      cell->collisionCode = 255;
      *blocked = 1;
    }
    ids++;
  }
}

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

extern const u8 gMagmaRock_Regions[];
extern const u8 gMagmaRock_Room2Regions[];
extern const u8 gMagmaRock_Room3Regions[];

const struct SceneEntrance *Scene_GetRegions(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov691Room2)
    return (const struct SceneEntrance *)gMagmaRock_Room2Regions;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov691Room3)
    return (const struct SceneEntrance *)gMagmaRock_Room3Regions;
  return (const struct SceneEntrance *)gMagmaRock_Regions;
}

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern const u8 gMagmaRock_Placements[];
extern const u8 gMagmaRock_Room2LowerPlacements[];
extern const u8 gMagmaRock_Room2MiddlePlacements[];
extern const u8 gMagmaRock_Room2UpperPlacements[];
extern const u8 gMagmaRock_Room2TopPlacements[];
extern const u8 gMagmaRock_Room3Placements[];

const struct Trigger *Scene_GetPlacements(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov691Room2) {
    if (gMainRoutineWork.entrance <= 3)
      return (const struct Trigger *)gMagmaRock_Room2LowerPlacements;
    if (gMainRoutineWork.entrance <= 8)
      return (const struct Trigger *)gMagmaRock_Room2MiddlePlacements;
    if (gMainRoutineWork.entrance <= 12)
      return (const struct Trigger *)gMagmaRock_Room2UpperPlacements;
    return (const struct Trigger *)gMagmaRock_Room2TopPlacements;
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov691Room3)
    return (const struct Trigger *)gMagmaRock_Room3Placements;
  return (const struct Trigger *)gMagmaRock_Placements;
}

void Func_02003c14(s16 *table);

void MagmaRock_UnblockMovedPillar(void) {
  u8 *event = Ram_HeapSlots->eventWork;
  struct MagmaRockWork *work = Ram_HeapSlots->effectWork;

  if (gMainRoutineWork.scene == (s32)&SceneId_Ov691Room1) {
    MapGrid_CopyCellAttributes(98, 18, 4, 3, 78, 21);
    MapGrid_CopyCellAttributes(98, 24, 4, 3, 78, 24);
    Func_02003c14(gMagmaRock_Room1Pillars);
  } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov691Room2) {
    if (gMainRoutineWork.entrance <= 3) {
      Func_02003c14(gMagmaRock_Room2LowerPillars);
    } else if (gMainRoutineWork.entrance <= 7) {
      Func_02003c14(gMagmaRock_Room2MiddlePillars);
    } else if (gMainRoutineWork.entrance <= 11) {
      MapGrid_CopyCellAttributes(79, 46, 2, 1, 79, 36);
      Func_02003c14(gMagmaRock_Room2UpperPillars);
    } else {
      if (((s16 *)&work->target->z)[1] == 600)
        MapGrid_CopyCellAttributes(104, 38, 1, 1, 103, 38);
      else
        MapGrid_CopyCellAttributes(94, 44, 5, 1, 99, 38);
      Func_02003c14(gMagmaRock_Room2TopPillars);
    }
  }
  MagmaRock_ClearBlockCells(*(struct Actor **)(event + 0xcbc));
}

void Event_Start();
void Func_02003c1c();
s32 Func_02003af4();

/// Rebuilds the room's opened passages on entry and sets the flag of the cell
/// the leader stands on.
s32 MagmaRock_BlockMovedPillar(void) {
  struct Actor *leader =
      *(struct Actor **)((u8 *)Ram_HeapSlots->eventWork + 3260);
  s32 x, z;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Func_02003c1c();
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov691Room1) {
    MapGrid_CopyCellAttributes(98, 21, 4, 3, 78, 21);
    MapGrid_CopyCellAttributes(98, 27, 4, 3, 78, 24);
    Blocks_MarkCells(gMagmaRock_Room1Pillars);
  } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov691Room2) {
    if (gMainRoutineWork.entrance <= 3) {
      Blocks_MarkCells(gMagmaRock_Room2LowerPillars);
    } else if (gMainRoutineWork.entrance <= 8) {
      Blocks_MarkCells(gMagmaRock_Room2MiddlePillars);
    } else if (gMainRoutineWork.entrance <= 12) {
      MapGrid_CopyCellAttributes(79, 37, 2, 1, 79, 36);
      Blocks_MarkCells(gMagmaRock_Room2UpperPillars);
    } else {
      MapGrid_CopyCellAttributes(103, 39, 1, 1, 103, 38);
      MapGrid_CopyCellAttributes(99, 42, 5, 1, 99, 38);
      Blocks_MarkCells(gMagmaRock_Room2TopPillars);
    }
  }
  x = leader->x >> 20;
  z = leader->z >> 20;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov691Room1) {
    if (x == 15 && z == 25)
      Event_SetFlag(0x8d6);
    else if (x == 16 && z == 22)
      Event_SetFlag(0x8d5);
  } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov691Room2) {
    if (gMainRoutineWork.entrance <= 3) {
      if (x == 27 && z == 33)
        Event_SetFlag(0x8d7);
    } else if (gMainRoutineWork.entrance <= 8) {
      if (x == 55 && z == 35)
        Event_SetFlag(0x8d8);
    } else if (gMainRoutineWork.entrance <= 12) {
      if (x == 16 && z == 36)
        Event_SetFlag(0x8d9);
    } else if (x == 40 && z == 37) {
      Event_SetFlag(0x8da);
    } else if (x == 35 && z == 38) {
      Event_SetFlag(0x8db);
    }
  }
  return Func_02003af4();
}

#include "frame.h"

void Import_FieldPsynergySetupActorPair(s32 owner, u16 value);

void MagmaRock_PlayBurst(s16 value) {
  struct MainRoutineWork *party;

  BattleEffect_LoadActionEffectResources(151, 1);
  party = &gMainRoutineWork;
  Import_FieldPsynergySetupActorPair(party->currentOwner, value);
  FieldPsynergy_RunHandler(1);
  BattleEffect_StopMainActor();
  BattleEffect_CleanupSceneObjects();
}

struct MagmaRockEffectParams {
  s32 unk00;
  u8 unk04[0x14];
  u16 kind;
  u8 unk1a[2];
  const void *script;
  u8 unk20[8];
};

extern const u8 gMagmaRock_FlameEmberScript[];

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 a, s32 b, s32 c, s32 flags,
                       s32 *params);

void MagmaRock_ThrowEmbers(struct Actor *object) {
  struct Actor *linked = object->watchedObject;
  s16 *timer = &object->timer;

  if (*timer < 50) {
    u32 phase = gFrameCount & 3;

    if (phase == 0) {
      struct MagmaRockEffectParams params;
      s32 dx;
      s32 dy;

      params.kind = 403;
      params.unk00 = 1;
      params.script = gMagmaRock_FlameEmberScript;
      {
        s32 mask = 15;

        dx = mask & Random16();
      }
      dy = Random16() & 31;
      FieldEffect_Spawn(object->x + ((dx - 8) << 16),
                        object->y + (dy << 16) - 0x80000, object->z, 0, phase,
                        phase, 0x320001, &params);
    }
    if (gFrameCount & 4)
      FieldActor_SetSpriteColor(object, 10);
    else
      FieldActor_SetSpriteColor(object, 0);
  } else if (*timer < 100) {
    if (gFrameCount & 4)
      FieldActor_SetSpriteColor(object, 10);
    else
      FieldActor_SetSpriteColor(object, 0);
  } else if (*timer < 140) {
    if (gFrameCount & 2)
      FieldActor_SetSpriteColor(object, 10);
    else
      FieldActor_SetSpriteColor(object, 0);
  } else if (*timer < 144) {
    FieldActor_SetSpriteColor(object, 7);
  } else {
    FieldActor_SetSpriteColor(object, 0);
    object->update = NULL;
    linked->x = object->x;
    linked->y = object->y;
    linked->z = object->z;
    linked->update = MagmaRock_RollMagmaRock;
  }
  (*timer)++;
}

struct MagmaRockEffectConfig {
  s32 kind;
  s32 variant;
  u8 unk08[16];
  u16 id;
  u8 unk1a[2];
  const void *data;
  u8 unk20[8];
};

void Func_02003c4c(struct Actor *object, s32 value);

/// Every fourth frame, spawns effect 403 at the object with a random z offset
/// and z velocity.
void MagmaRock_TrailEmbers(struct Actor *object) {
  u32 frame = gFrameCount;

  if ((frame & 3) == 0) {
    struct MagmaRockEffectConfig config;
    s32 dz;
    s32 velocityZ;
    s32 z;

    config.id = 403;
    config.kind = 2;
    config.data = gMagmaRock_EmberScript;
    if ((frame & 15) == 0 && !Event_GetFlag(608))
      Func_02003c4c(object, 136);
    dz = ((Random16() & 15) - 8) << 15;
    velocityZ = ((Random16() & 31) - 16) << 12;
    z = object->z;
    FieldEffect_Spawn(object->x, object->y, z + dz, 0, 0, velocityZ, 0x320001,
                      &config);
  }
}

struct MagmaRockEventWork {
  u8 unk000[362];
  u16 unk16a;
};

s32 Trig_Cos(s32 angle);

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))
#define HALF_AT(base, offset) (*(s16 *)((u8 *)(base) + (offset)))

void MagmaRock_RollMagmaRock(struct Actor *object) {
  s32 step;
  struct Actor *partner;
  struct Actor *leader;

  step = Trig_Cos(object->facing);
  partner = Actor_Get(HALF_AT(object, 102) + BYTE_AT(object, 98));
  leader = Actor_Get(gMainRoutineWork.currentOwner);
  object->x += step * 2;
  MagmaRock_TrailEmbers(object);
  if (leader->x - 0x80000 < object->x + 0x60000 &&
      object->x - 0x60000 < leader->x + 0x80000 &&
      leader->z - 0x80000 < object->z + 0x60000 &&
      object->z - 0x60000 < leader->z + 0x80000 &&
      leader->y < object->y + 0x80000 &&
      object->y - 0x80000 < leader->y + 0xf0000) {
    ((struct MagmaRockEventWork *)Ram_HeapSlots->eventWork)->unk16a = 200;
    object->update = NULL;
    object->x = 0;
    object->y = 0;
    object->z = 0;
  }
  if (Position_FlatDistance(&object->x, &partner->x) <= 8 &&
      object->y >> 20 == partner->y >> 20) {
    object->x = 0;
    object->update = NULL;
    object->facing ^= 0x9000;
    object->y = 0;
    object->z = 0;
    BYTE_AT(object, 98) ^= 1;
    partner->timer = 0;
    partner->update = (void *)MagmaRock_ThrowEmbers;
    partner->watchedObject = object;
  }
}

// Unless flag 0x109 is set, places the object at the watched object first +
// side, gives it MagmaRock_RollMagmaRock and faces it along the pair first and
// first + 1.

void MagmaRock_PlaceVolley(u16 id, s16 first, s16 side) {
  struct Actor *object = Actor_Get(id);
  struct Actor *a;
  struct Actor *b;
  u16 left;
  u16 right;
  s32 linked;
  s32 next;
  u8 *flags;
  u8 facing;

  if (Event_GetFlag(0x109))
    return;
  left = first;
  right = side;
  linked = left + right;
  object->x = Actor_Get(linked)->x;
  object->y = Actor_Get(linked)->y;
  object->z = Actor_Get(linked)->z;
  Scene_SetActorSpritePriority(object, 2);
  HALF_AT(object, 102) = first;
  /* FAKEMATCH(temporary): the byte is computed before its address; as
   * BYTE_AT(object, 98) = side ^ 1, 6 of 104 instructions differ in tla-en. */
  facing = side ^ 1;
  BYTE_AT(object, 98) = facing;
  object->update = (void *)MagmaRock_RollMagmaRock;
  next = left + 1;
  flags = &object->motionFlags;
  if (right != 0) {
    a = Actor_Get(left);
    b = Actor_Get(next);
    if (a->x < b->x)
      object->facing = FACING_WEST;
    else
      object->facing = 0;
  } else {
    a = Actor_Get(left);
    b = Actor_Get(next);
    if (a->x > b->x)
      object->facing = FACING_WEST;
    else
      object->facing = 0;
  }
  *flags = 0;
}

DEFINE_ACTOR_FLASH_COLOR(MagmaRock_FlashStatue, 10)
