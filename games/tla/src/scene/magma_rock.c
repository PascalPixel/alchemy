#include "../field/object/set_sprite_priority.c"

#include "../field/object/prepare_object.c"

#include "types.h"
#include "flags.h"
#include "system/ram_buffer.h"
#include "system/iwram_call.h"
#include "actor.h"
#include "field/event_inline.h"
#include "module/scene/scene.h"
#include "system/main_routine_work.h"
#include "field/field_coordinate.h"
#include "module/field/field.h"
#include "system/scene.h"
#include "map.h"
#include "field/field.h"
#include "module/field/event_commands.h"
#include "module/scene/magma_rock.h"
#include "battle/battle.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "lib.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "scene_tables.h"
#include "scene_ids.h"
#include "system/frame.h"
#include "field/event.h"
#include "field/field_sequence_event.h"
#include "flag_call.h"
#include "field/field_sequence_event.h"
#include "system/field_effect.h"
#include "macros.h"
#include "call.h"
#include "fixed_math.h"
#include "field/sprite.h"
#include "module/field/object.h"
#include "module/scene/mars_lighthouse_star_room.h"
#include "shared/tables.h"
#include "field_sprite.h"
#include "graphics/resource.h"
#include "system/heap_release.h"
#include "module/field/field_effect.h"
#include "field/particle.h"
#include "system/task.h"
#include "objcmd.h"

struct Actor *MagmaRock_CreateEffectActor(s32 x, s32 y, s32 z, s32 kind) {
  struct Actor *object = (FieldActor_Create(kind, x, y, z));

  if (object != NULL) {
    Scene_SetActorSpritePriority(object, 1);
    object->motionFlags = 0;
    object->collisionFlags = 8;
    Actor_SetSpriteFlags(object, 0);
    FieldActor_SetSpriteColor(object, 15);
    object->priorityFlags |= 34;
    return object;
  }
  return NULL;
}

#include "../field/field_effect/move.c"

#define EFFECT_SPAWN FieldEffect_Spawn
#define EFFECT_CREATE Import_ActorCreate
#define EFFECT_SET_MODE Actor_SelectAnimation
#define EFFECT_SET_SCRIPT Actor_StartScript
#define EFFECT_SCRIPTS gFieldEffect_DurationScripts
#define EFFECT_SPRITE_PRIORITY
#include "../field/field_effect/spawn.c"

#include "shared/distance.c"

#include "../field/object/find_in_cell.c"

#undef EFFECT_SPAWN

extern s32 gPushLeaderSteps[];

#include "shared/block_push_onto.h"

#include "../field/map/cell_fill_byte.c"

// Magma Rock's outside, the volcano north of Loho the party climbs to its
// summit: the Moai statues Burst wakes, which fire magma rocks or lift the
// leader on a column of flame, the cracked columns they break, the pillars
// Move pushes and the crumbling ones, Growth's vines and the slides, and at
// the top the Psynergy rock that blasts the way in.

void MagmaRock_FightFury(void) { Field_RequestDjinnBattle(8, 56); }

#include "shared/position_flat_distance.h"

extern u8 gMapShapeGrid[];

s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
s32 MapGrid_GetTerrainKind(s32 layer, s32 x, s32 z);
s32 MapCollision_FindOrAddTag(s32 layer, s32 shape, s32 height);

#include "shared/map_cell_set_shape.h"

#include "../field/map/cell_get.c"

#include "shared/actors_raise_cells.h"

#ifndef TLA_MAGMA_ROCK_H
#define TLA_MAGMA_ROCK_H

#include "types.h"

struct Actor;

struct MagmaRockWork {
  s32 unk00;
  s32 x;
  s32 y;
  s32 z;
  s32 unk10;
  struct Actor *target;
};

#endif

// For each id of a list ended by 0xffff: clears the object's mark and, on a
// cell with attribute bit 6, blocks the cells ahead of and behind it and sets
// the mark.
void Blocks_MarkCells(u16 *ids) {
  struct Actor *object;
  u8 *blocked;
  struct MapWork *map;
  struct MapCell **cells;
  struct MapCell *cell;
  s32 x, z;

  while (*ids != 0xffff) {
    object = Actor_Get(*ids);
    blocked = &object->blockMarked;
    *blocked = 0;
    if (MapCell_GetAttributes(0, object->x >> 20, object->z >> 20) & 0x40) {
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

// Clears the object's mark and the cells ahead of and behind it that the mark
// blocked.

#include "shared/block_clear_cells.h"

// For each id of a list ended by 0xffff: settles the object on its ground,
// copies the cell under it, and blocks the cells ahead of and behind it on a
// cell with attribute bit 6.
#include "text/msg_ids.h"

// For each id of a list ended by 0xffff: settles the object on its ground,
// copies the cell under it, and blocks the cells ahead of and behind it on a
// cell with attribute bit 6.
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
    MagmaRockInside_SetCellShape(*terrainId, object->x >> 20, object->z >> 20,
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

extern s16 gMagmaRock_Regions[];
extern s16 gMagmaRock_Room2Regions[];
extern s16 gMagmaRock_Room3Regions[];

const struct SceneEntrance *Scene_GetRegions(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_MagmaRock2)
    return (const struct SceneEntrance *)gMagmaRock_Room2Regions;
  if (gMainRoutineWork.scene == (s32)&SceneId_MagmaRock3)
    return (const struct SceneEntrance *)gMagmaRock_Room3Regions;
  return (const struct SceneEntrance *)gMagmaRock_Regions;
}

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern struct ActorPlacement gMagmaRock_Placements[];
extern struct ActorPlacement gMagmaRock_Room2LowerPlacements[];
extern struct ActorPlacement gMagmaRock_Room2MiddlePlacements[];
extern struct ActorPlacement gMagmaRock_Room2UpperPlacements[];
extern struct ActorPlacement gMagmaRock_Room2TopPlacements[];
extern struct ActorPlacement gMagmaRock_Room3Placements[];

const struct ActorPlacement *Scene_GetPlacements(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_MagmaRock2) {
    if (gMainRoutineWork.entrance <= 3)
      return gMagmaRock_Room2LowerPlacements;
    if (gMainRoutineWork.entrance <= 8)
      return gMagmaRock_Room2MiddlePlacements;
    if (gMainRoutineWork.entrance <= 12)
      return gMagmaRock_Room2UpperPlacements;
    return gMagmaRock_Room2TopPlacements;
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_MagmaRock3)
    return gMagmaRock_Room3Placements;
  return gMagmaRock_Placements;
}

void Actor_LiftTargetBlock(s16 *table);

void MagmaRock_UnblockMovedPillar(void) {
  u8 *event = Ram_HeapSlots->fieldWork;
  struct MagmaRockWork *work = Ram_HeapSlots->effectWork;

  if (gMainRoutineWork.scene == (s32)&SceneId_MagmaRock1) {
    MapGrid_CopyCellAttributes(98, 18, 4, 3, 78, 21);
    MapGrid_CopyCellAttributes(98, 24, 4, 3, 78, 24);
    Actor_LiftTargetBlock(gMagmaRock_Room1Pillars);
  } else if (gMainRoutineWork.scene == (s32)&SceneId_MagmaRock2) {
    if (gMainRoutineWork.entrance <= 3) {
      Actor_LiftTargetBlock(gMagmaRock_Room2LowerPillars);
    } else if (gMainRoutineWork.entrance <= 7) {
      Actor_LiftTargetBlock(gMagmaRock_Room2MiddlePillars);
    } else if (gMainRoutineWork.entrance <= 11) {
      MapGrid_CopyCellAttributes(79, 46, 2, 1, 79, 36);
      Actor_LiftTargetBlock(gMagmaRock_Room2UpperPillars);
    } else {
      if (((s16 *)&work->target->z)[1] == 600)
        MapGrid_CopyCellAttributes(104, 38, 1, 1, 103, 38);
      else
        MapGrid_CopyCellAttributes(94, 44, 5, 1, 99, 38);
      Actor_LiftTargetBlock(gMagmaRock_Room2TopPillars);
    }
  }
  MagmaRockInside_ClearBlockCells(*(struct Actor **)(event + 0xcbc));
}

void Event_Start();
void Actor_PlaceMovedBlock();
s32 Func_02003af4();

// Rebuilds the room's opened passages on entry and sets the flag of the cell
// the leader stands on.
s32 MagmaRock_BlockMovedPillar(void) {
  struct Actor *leader =
      *(struct Actor **)((u8 *)Ram_HeapSlots->fieldWork + 3260);
  s32 x, z;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_PlaceMovedBlock();
  if (gMainRoutineWork.scene == (s32)&SceneId_MagmaRock1) {
    MapGrid_CopyCellAttributes(98, 21, 4, 3, 78, 21);
    MapGrid_CopyCellAttributes(98, 27, 4, 3, 78, 24);
    Blocks_MarkCells(gMagmaRock_Room1Pillars);
  } else if (gMainRoutineWork.scene == (s32)&SceneId_MagmaRock2) {
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
  if (gMainRoutineWork.scene == (s32)&SceneId_MagmaRock1) {
    if (x == 15 && z == 25)
      Event_SetFlag(0x8d6);
    else if (x == 16 && z == 22)
      Event_SetFlag(0x8d5);
  } else if (gMainRoutineWork.scene == (s32)&SceneId_MagmaRock2) {
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

extern u32 gMagmaRock_FlameEmberScript[];

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

void Event_PlayCueNearFocus(struct Actor *object, s32 value);

// Every fourth frame, spawns effect 403 at the object with a random z offset
// and z velocity.
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
    if ((frame & 15) == 0 && !Event_GetFlag(FLAG_SCENE_TEMP_60))
      Event_PlayCueNearFocus(object, 136);
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
  if (leader->x - px(8) < object->x + px(6) &&
      object->x - px(6) < leader->x + px(8) &&
      leader->z - px(8) < object->z + px(6) &&
      object->z - px(6) < leader->z + px(8) && leader->y < object->y + px(8) &&
      object->y - px(8) < leader->y + px(15)) {
    ((struct MagmaRockEventWork *)Ram_HeapSlots->fieldWork)->unk16a = 200;
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

  if (Event_GetFlag(FLAG_KEEP_PARTY_POSITION))
    return;
  left = first;
  right = side;
  linked = left + right;
  object->x = Actor_Get(linked)->x;
  object->y = Actor_Get(linked)->y;
  object->z = Actor_Get(linked)->z;
  Scene_SetActorSpritePriority(object, 2);
  HALF_AT(object, 102) = first;
  /* FAKEMATCH(temporary): the byte is computed before its address. */
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

void MagmaRock_FlashStatue(struct Actor *object) {
  if (gFrameCount & object->unk5c[7])
    FieldActor_SetSpriteColor(object, 10);
  else
    FieldActor_SetSpriteColor(object, 0);
}

// Magma Rock's statues: the magma they fire, the flame columns that lift the
// leader, the cracked columns, slides, pillars, vines and the final blast.

/* VIEW(s0ff4): tag struct MagmaRockEventWork as struct
 * MagmaRockEventWork__s0ff4; scene/magma_rock has another definition; tried
 * with the definition the rest of the object has, it does not compile in tla-ja
 * (magma_rock.c): games/tla/src/scene/magma_rock.c:554: redefinition of `struct
 * MagmaRockEventWork'. */
#define MagmaRockEventWork MagmaRockEventWork__s0ff4
struct MagmaRockEventWork {
  u8 unk000[0x15a];
  s16 value15a;
  u8 unk15c[2];
  s16 value15e;
  u8 unk160[6];
  s16 value166;
  s16 value168;
  u8 unk16a[0x48];
  u16 value1b2;
};

void MagmaRock_WarnStatue(struct Actor *object) {
  struct MagmaRockEventWork *work = Ram_HeapSlots->fieldWork;

  if (object->timer > 528) {
    if (object->timer & 2)
      FieldActor_SetSpriteColor(object, 10);
    else
      FieldActor_SetSpriteColor(object, 0);
  } else if (object->timer > 400) {
    if (object->timer & 16)
      FieldActor_SetSpriteColor(object, 10);
    else
      FieldActor_SetSpriteColor(object, 0);
  } else {
    object->update = MagmaRock_BurnStatue;
    object->timer = 1;
  }
  if (work->value166 == 0 && work->value15a == 0 && work->value15e == 0 &&
      work->value168 == 0 && work->value1b2 != 0)
    object->timer--;
}

s32 MagmaRock_FindStatue(u16 *table) {
  struct MagmaRockWork *work = Ram_HeapSlots->effectWork;
  s32 result = 0;
  s32 code = MapCell_GetAttributes(0, work->x >> 20, work->z >> 20) >> 8;
  s32 id;

  goto check;
next:
  result = *table++;
check:
  if (*table == 0xffff)
    goto done;
  id = *table++;
  if (code != *table++)
    goto next;
  work->target = Actor_Get(id);
  result = *table;
done:
  return result;
}

s32 MagmaRock_LookUpCellStatue(s32 x, s32 z, u16 *table) {
  s32 result = 0;
  s32 code = MapCell_GetAttributes(0, x >> 20, z >> 20) >> 8;

  while (*table != 0xffff) {
    table++;
    if (code == *table) {
      result = table[1];
      break;
    }
    table++;
    result = *table++;
  }
  return result;
}

// Picks the cell table for the room and entrance, finds the target in it and,
// in modes 2 and 3, sets the target's byte 99 to 4 or 2 and makes it blink
// with MagmaRock_FlashStatue.
void MagmaRock_TargetStatue(s32 mode) {
  struct MagmaRockWork *work = Ram_HeapSlots->effectWork;
  s16 *table = gMagmaRock_Room1Statues;
  struct MainRoutineWork *party = &gMainRoutineWork;

  if (party->scene == (s32)&SceneId_MagmaRock2) {
    if (party->entrance <= 3)
      table = gMagmaRock_Room2LowerStatues;
    else if (party->entrance <= 8)
      table = gMagmaRock_Room2MiddleStatues;
    else if (party->entrance <= 12)
      table = gMagmaRock_Room2UpperStatues;
    else
      table = gMagmaRock_Room2TopStatues;
  } else if (party->scene == (s32)&SceneId_MagmaRock3) {
    table = gMagmaRock_Room3Statues;
  }
  if (mode == 2) {
    struct Actor *target;

    MagmaRock_FindStatue(table);
    target = work->target;
    ((u8 *)target)[99] = 4;
    target->update = (void (*)(void))MagmaRock_FlashStatue;
  }
  if (mode == 3) {
    struct Actor *target;

    MagmaRock_FindStatue(table);
    target = work->target;
    ((u8 *)target)[99] = 2;
    target->update = (void (*)(void))MagmaRock_FlashStatue;
  }
}

// Adds the velocity to the position and decays it by one eighth each
// update.
void MagmaRock_DriftAndSlow(struct Actor *object) {
  object->x += object->bounce;
  object->y += object->gravity;
  object->z += object->unk4c;
  object->bounce -= object->bounce >> 3;
  object->gravity -= object->gravity >> 3;
  object->unk4c -= object->unk4c >> 3;
}

s32 Position_FlatDistance(s32 *a, s32 *b);
// XXX: magma_rock.c defines this with an s16 value; this call matches only with
// u16. A match of magma_rock.c's MagmaRock_PlayBurst under u16 would settle
// which the game declares.
/* VIEW(s0ff4): routine MagmaRock_PlayBurst as void MagmaRock_PlayBurst(u16
 * index); scene/magma_rock has void MagmaRock_PlayBurst(s16 value) {; tried as
 * the definition's void MagmaRock_PlayBurst(s16 value), which the object
 * already declares, MagmaRock_FireFromStatue moves: 1 of 260 instructions in
 * tla-ja (magma_rock.c). */
void MagmaRock_PlayBurst__s0ff4(u16 index) __asm__("MagmaRock_PlayBurst");
#define MagmaRock_PlayBurst MagmaRock_PlayBurst__s0ff4
void Actor_Release(struct Actor *object);
s32 Func_02003af4(void);
void FieldCamera_WaitForArrival(void);

s32 MagmaRock_FireFromStatue(struct Actor *source) {
  s32 position[3];
  struct EffectOptions options;
  struct Actor *object;
  struct Actor *other;
  s32 step;
  s32 parity;
  s32 i;
  s32 attributes;

  {
    s32 x = source->x;
    s32 y = source->y;
    s32 z = source->z;

    object = FieldActor_Create(403, x, y, z);
  }
  step = Trig_Cos(source->facing) << 1;
  object->motionFlags = 0;
  Actor_SelectAnimation(object, 1);
  Actor_SetSpriteFlags(object, 0);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  position[0] = object->x;
  position[1] = object->y;
  position[2] = object->z;
  for (;;) {
    position[0] += step;
    attributes = MapCell_GetAttributes(2, position[0] >> 20,
                                       (position[2] - position[1]) >> 20) >>
                 8;
    if (attributes == 212 || attributes == 222)
      goto stop;
    for (i = 8; i != 64; i++) {
      other = gFieldWork->actors[i];
      if (other->script == NULL)
        continue;
      if (source->x >> 20 == other->x >> 20 &&
          source->y >> 20 == other->y >> 20 &&
          source->z >> 20 == other->z >> 20)
        continue;
      if (Position_FlatDistance(&other->x, position) > 8)
        continue;
      if (other->y >> 20 != position[1] >> 20)
        continue;
      if (*((struct ObjectAnimationState *)other->sprite)->value28 == 0x110)
        goto stop;
      goto hit;
    }
  }

stop:
  object->acceleration = px(2);
  object->speedLimit = px(2);
  object->update = MagmaRock_TrailEmbers;
  FieldActor_SetMoveTargetFixed(object, position[0], position[1], position[2]);
  FieldCamera_SetSpeed(2);
  FieldCamera_MoveToFixed(position[0], position[1], position[2], 1);
  FieldActor_WaitForMove(object);
  object->update = NULL;
  Actor_Release(object);
  for (i = 0; i != 16; i++) {
    parity = gFrameCount & 1;

    if (parity == 0) {
      s32 dx;
      s32 dz;

      options.type = 403;
      options.priority = 1;
      options.update = (void (*)(union FieldObject *))MagmaRock_DriftAndSlow;
      options.script = (const s32 *)gMagmaRock_EmberScript;
      dx = Random16() & 31;
      dz = Random16() & 31;
      FieldEffect_Spawn(position[0], position[1], position[2], (dx - 16) << 13,
                        (dz - 16) << 13, parity, 0x1320000, &options);
    }
    System_Wait(1);
  }
  System_Wait(24);
  EventCamera_FollowActor(gMainRoutineWork.currentOwner, 1);
  FieldCamera_WaitForArrival();
  System_Wait(10);
  return Func_02003af4();

hit:
  object->acceleration = px(2);
  object->speedLimit = px(2);
  object->update = MagmaRock_TrailEmbers;
  FieldActor_SetMoveTargetFixed(object, position[0], position[1], position[2]);
  FieldCamera_SetSpeed(2);
  FieldCamera_MoveToFixed(position[0], position[1], position[2], 1);
  FieldActor_WaitForMove(object);
  object->update = NULL;
  Actor_Release(object);
  Event_SetFlag(FLAG_SCENE_TEMP_F0);
  MagmaRock_PlayBurst(i);
  Event_ClearFlag(FLAG_SCENE_TEMP_F0);
  EventCamera_FollowActor(gMainRoutineWork.currentOwner, 1);
  FieldCamera_WaitForArrival();
  System_Wait(10);
  return Func_02003af4();
}

void MagmaRock_FireStatue(void) {
  struct MagmaRockWork *work = Ram_HeapSlots->effectWork;
  struct Actor *object;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_Wait(10);
  object = work->target;
  object->update = NULL;
  FieldActor_SetSpriteColor(object, 7);
  FieldEvent_Wait(3);
  FieldActor_SetSpriteColor(work->target, 0);
  FieldEvent_Wait(1);
  MagmaRock_FireFromStatue(work->target);
  Func_02003af4();
}

void MagmaRock_IgniteStatue(void) {
  struct MagmaRockWork *work = Ram_HeapSlots->effectWork;
  struct Actor *object;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  object = work->target;
  object->timer = 0x400;
  object->update = MagmaRock_WarnStatue;
  Func_02003af4();
}

void MagmaRock_BurstStatue(s32 mode, s32 cue) {
  struct MagmaRockWork *work = Ram_HeapSlots->effectWork;

  if (cue == 0)
    MagmaRock_StartFallingRocks();
  if (mode == 2) {
    struct Actor *object = work->target;

    MagmaRock_DropRocks(object->x, object->y + px(15), object->z, 1);
  }
  if (mode == 3) {
    struct Actor *object = work->target;
    struct Actor *target;

    MagmaRock_DropRocks(object->x, object->y + px(15), object->z, 8);
    target = work->target;
    SET_POSITION(target, 0, 0, 0);
  }
  if (cue == 0x3e7)
    MagmaRock_EndFallingRocks();
  if (cue == 178)
    Actor_SelectAnimation(work->target, 3);
}

// One of the four parts MagmaRock_BurnStatue builds: rises and falls with its
// owner's action counter.

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))

/* VIEW(s0ff4): tag union Position as union Position__s0ff4;
 * module/scene/scene.h has struct Position; tried with the definition the rest
 * of the object has, it does not compile in tla-ja (magma_rock.c):
 * games/tla/src/scene/magma_rock.c:858: `Position' defined as wrong kind of
 * tag. */
#define Position Position__s0ff4
// A 16.16 position word, read whole or by halves.
union Position {
  s32 word;
  s16 half[2];
};

#define PART_Y(part) (((union Position *)&(part)->y)->word)

void Actor_Release(struct Actor *part);

// One of the four parts MagmaRock_BurnStatue builds: rises and falls with its
// owner's action counter.
void MagmaRock_RaiseFlamePart(struct Actor *part) {
  struct Actor *owner = part->watchedObject;

  if (owner->timer > 35 && owner->timer < BYTE_AT(part, 98) * 2 + 36) {
    PART_Y(part) = owner->y + px(32);
    PART_Y(part) += (owner->timer << 19) + (-35 << 19);
  } else if (BYTE_AT(part, 98) * 2 + 36 < owner->timer && owner->timer < 332) {
    PART_Y(part) = owner->y + (BYTE_AT(part, 98) << 20) + 0x200000;
  } else if (owner->timer > 332 && owner->timer < BYTE_AT(part, 98) * 2 + 333) {
    PART_Y(part) = owner->y + (BYTE_AT(part, 98) << 20) + 0x200000;
    PART_Y(part) -= (owner->timer << 19) - (332 << 19);
  } else if (BYTE_AT(part, 98) * 2 + 333 < owner->timer) {
    Actor_Release(part);
  }
}

// The fifth part of MagmaRock_BurnStatue's group: follows its owner like the
// others, throws effect 403 every eighth frame and marks the ground beneath.

/* VIEW(s0ff4): tag struct MagmaRockEffectConfig as struct
 * MagmaRockEffectConfig__s0ff4; scene/magma_rock has another definition; tried
 * with the definition the rest of the object has, it does not compile in tla-ja
 * (magma_rock.c): games/tla/src/scene/magma_rock.c:894: redefinition of `struct
 * MagmaRockEffectConfig'. */
#define MagmaRockEffectConfig MagmaRockEffectConfig__s0ff4
struct MagmaRockEffectConfig {
  s32 kind;
  s32 variant;
  s32 unk08;
  s32 unk0c;
  u8 unk10[8];
  u16 id;
  u8 unk1a[2];
  const void *data;
  u8 unk20[4];
  void (*unk24)(void);
};

#define EFFECT_Y(object) (((union Position *)&(object)->effect.y)->word)

// The fifth part of MagmaRock_BurnStatue's group: follows its owner like the
// others, throws effect 403 every eighth frame and marks the ground beneath.
void MagmaRock_RaiseFlameTop(union FieldObject *object) {
  struct Actor *watched = (struct Actor *)object->effect.countdown;
  struct MagmaRockEffectConfig config;
  s32 step;
  s32 mask;

  if (watched->timer >= 35 && watched->timer <= 42) {
    EFFECT_Y(object) = watched->y + px(24);
    EFFECT_Y(object) = EFFECT_Y(object) + (watched->timer << 19) + (-34 << 19);
  } else {
    if (watched->timer >= 9 && watched->timer <= 331) {
      EFFECT_Y(object) =
          watched->y + (object->effect.unk5a[8] << 20) + 0x180000;
    } else if (watched->timer > 332 &&
               watched->timer < (object->effect.unk5a[8] << 1) + 333) {
      EFFECT_Y(object) =
          watched->y + (object->effect.unk5a[8] << 20) + 0x180000;
      EFFECT_Y(object) -= (watched->timer << 19) - (332 << 19);
    } else if ((object->effect.unk5a[8] << 1) + 333 < watched->timer) {
      object->effect.y = object->actor.terrainHeight;
      object->effect.update = 0;
    }
  }

  mask = 7;
  step = gFrameCount & mask;
  if (step == 0) {
    config.id = 403;
    config.variant = 10;
    config.kind = 2;
    config.unk24 = MagmaRock_DriftAndSlow;
    config.data = gMagmaRock_EmberScript;
    FieldEffect_Spawn(object->effect.x + px(1), object->effect.y - px(4),
                      object->effect.z, ((Random16() & mask) - 3) << 14,
                      -0x20000, step, 0x1330000, &config);
  }
  MagmaRockInside_SetCellShape(watched->terrainId, watched->x >> 20,
                               watched->z >> 20, (object->effect.y >> 19) + 1);
}
#undef BYTE_AT
#undef PART_Y
#undef EFFECT_Y

void Event_PlayCueNearFocus(struct Actor *object, s32 a);

struct AnimationEntry;

// The sprite as Sprite_Create fills it, laid out as vram_block_93.c
// declares it.
struct ResourceSprite {
  u32 imageAttributes[3];
  s32 scale;
  u8 slot;
  u8 retainCacheEntry : 1;
  u8 useOwnScale : 1;
  u8 unk11 : 6;
  u16 rotation;
  u8 width;
  u8 height;
  s8 offsetX;
  s8 offsetY;
  u8 lastNo;
  u8 dirty;
  u8 flags;
  u8 count;
  u32 shadowAttributes[3];
  struct AnimationEntry *entries[4];
};

void MagmaRock_BurnStatue(struct Actor *object) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  s32 type = 0x15f;

  if ((gFrameCount & 15) == 0)
    Event_PlayCueNearFocus(object, 136);
  FieldActor_SetSpriteColor(object, 10);
  if (object->timer <= 31) {
    u32 phase = gFrameCount & 3;

    if (phase == 0) {
      struct MagmaRockEffectConfig config;
      s32 dx;
      s32 dz;

      config.id = 403;
      config.variant = 10;
      config.kind = 2;
      config.unk24 = MagmaRock_DriftAndSlow;
      config.data = gMagmaRock_EmberScript;
      dx = Random16() & 15;
      dz = Random16() & 15;
      FieldEffect_Spawn(object->x + px(1), object->y + px(24), object->z,
                        (dx - 8) << 14, 0, (dz - 8) << 14, 0x1330000, &config);
    }
  } else if (object->timer == 33) {
    struct Actor *parts[4];
    struct Actor *fifthPart;
    s32 id;
    s32 i;

    Event_PlayCueNearFocus(object, 144);
    parts[0] = (FieldActor_Create(type, object->x + px(1), object->y + px(32),
                                  object->z));
    for (i = 1; i != 4; i++) {
      struct ResourceSprite *first = parts[0]->sprite;
      struct Actor *part = (FieldActor_Create(type, 0, 0, 0));
      struct ResourceSprite *sprite;

      parts[i] = part;
      sprite = part->sprite;
      sprite->slot = first->slot;
      sprite->retainCacheEntry = 1;
      part->x = parts[0]->x;
      part->y = parts[0]->y;
      part->z = parts[0]->z;
      part->motionFlags = 0;
      Actor_SetSpriteFlags(part, 0);
      Actor_SelectAnimation(parts[i], 3);
      /* FAKEMATCH(dead): part set again to parts[i], which it holds. */
      part = parts[i];
      // Byte 98 lies in actor.h's callback.
      part->unk5c[6] = 3 - i;
      part->watchedObject = object;
      part->update = MagmaRock_RaiseFlamePart;
      FieldActor_SetSpriteColor(part, 3);
    }
    {
      struct Actor *part = parts[0];

      part->motionFlags = 0;
      Actor_SetSpriteFlags(part, 0);
      Actor_SelectAnimation(parts[0], 3);
      Scene_SetActorSpritePriority(parts[0], 3);
      /* FAKEMATCH(dead): part set again to parts[0], which it holds. */
      part = parts[0];
      part->unk5c[6] = 3;
      part->watchedObject = object;
      part->update = MagmaRock_RaiseFlamePart;
      FieldActor_SetSpriteColor(part, 3);
    }
    id = 0;
    if (gMainRoutineWork.scene == (s32)&SceneId_MagmaRock1) {
      id = MagmaRock_LookUpCellStatue(object->x, object->z,
                                      gMagmaRock_Room1Statues);
    } else if (gMainRoutineWork.scene == (s32)&SceneId_MagmaRock2) {
      s16 entrance = gMainRoutineWork.entrance;
      s32 x = object->x;
      s32 z = object->z;

      if (entrance <= 3)
        id = MagmaRock_LookUpCellStatue(x, z, gMagmaRock_Room2LowerStatues);
      else if (entrance <= 8)
        id = MagmaRock_LookUpCellStatue(x, z, gMagmaRock_Room2MiddleStatues);
      else if (entrance <= 12)
        id = MagmaRock_LookUpCellStatue(x, z, gMagmaRock_Room2UpperStatues);
      else
        id = MagmaRock_LookUpCellStatue(x, z, gMagmaRock_Room2TopStatues);
    }
    fifthPart = Actor_Get(id);
    fifthPart->unk5c[6] = 4;
    fifthPart->watchedObject = object;
    fifthPart->update = MagmaRock_RaiseFlameTop;
    object->timer++;
  } else if (object->timer > 364) {
    FieldActor_SetSpriteColor(object, 0);
    object->update = NULL;
  }
  if (work->psynergy == 0 && *(s16 *)&work->fieldMenu == 0 &&
      work->pauseMenu == 0 && work->item == 0 &&
      (u16)work->transitionActive != 0)
    object->timer++;
}

// In room 1 or 2, targets actor 15 or 14, runs MagmaRock_BurstStatue and sets
// flag 0x200.
#define DEFINE_MAGMA_ROCK_BURST_COLUMN(name, n1, n2, n3)                       \
  void name(s32 mode, s32 cue) {                                               \
    struct MagmaRockWork *work = Ram_HeapSlots->effectWork;                    \
    struct MainRoutineWork *party = &gMainRoutineWork;                         \
                                                                               \
    {                                                                          \
      s32 scene = party->scene;                                                \
                                                                               \
      if (scene == (s32)&SceneId_MagmaRock1)                                   \
        work->target = Actor_Get(n1);                                          \
      else if (scene == (s32)&SceneId_MagmaRock2)                              \
        work->target = Actor_Get(n2);                                          \
      MagmaRock_BurstStatue(mode, cue);                                        \
    }                                                                          \
    Event_SetFlag(n3);                                                         \
  }

DEFINE_MAGMA_ROCK_BURST_COLUMN(MagmaRock_BurstColumn1, 15, 14, 0x200)

// In room 1 or 2, targets actor 16 or 10, runs MagmaRock_BurstStatue and sets
// flag 0x201.
DEFINE_MAGMA_ROCK_BURST_COLUMN(MagmaRock_BurstColumn2, 16, 10, 0x201)

#define DEFINE_MAGMA_ROCK_BURST_STATUE_COLUMN(name, n1, n2)                    \
  void name(s32 mode, s32 cue) {                                               \
    struct MagmaRockWork *work = Ram_HeapSlots->effectWork;                    \
                                                                               \
    work->target = Actor_Get(n1);                                              \
    MagmaRock_BurstStatue(mode, cue);                                          \
    Event_SetFlag(n2);                                                         \
  }

DEFINE_MAGMA_ROCK_BURST_STATUE_COLUMN(MagmaRock_BurstColumn3, 17, 0x202)

DEFINE_MAGMA_ROCK_BURST_STATUE_COLUMN(MagmaRock_BurstColumn4, 18, 0x203)

DEFINE_MAGMA_ROCK_BURST_STATUE_COLUMN(MagmaRock_BurstColumn5, 11, 0x204)

void MagmaRock_ClearSlideFlag(void) {
  struct MainRoutineWork *party = &gMainRoutineWork;

  if (party->renderMode != 2)
    Event_ClearFlag(FLAG_SCENE_TEMP_F8);
}

void MagmaRock_SlideDown(void) {
  struct MainRoutineWork *party = &gMainRoutineWork;

  if (party->renderMode != 2) {
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    Actor_Get(party->currentOwner)->motionFlags = 0;
    Actor_WalkTo(party->currentOwner, 264, 440);
    FieldEvent_Wait(5);
    Event_PlaySound(SE_MAP_EXIT);
    FieldEvent_SetExitRequest(2);
    Func_02003af4();
  }
  Event_SetFlag(FLAG_SCENE_TEMP_F8);
}

void MagmaRock_ResolveBeforeBurst(void) { Event_ResolvePendingActions(0); }

void MagmaRock_CheckCrumblingPillars(void) {
  MagmaRock_StepOnCrumblingPillar(gMagmaRock_CrumblingPillars);
}

void MagmaRock_SetLedgeBehavior(void) {
  Field_SetCellBehaviorAt(0, 0x1080000, 0x2280000, 30);
  Event_SetFlag(FLAG_SCENE_TEMP_12);
}

void MagmaRock_ClearLedgeBehavior(void) {
  Field_SetCellBehaviorAt(0, 0x1080000, 0x2280000, 0);
  Event_ClearFlag(FLAG_SCENE_TEMP_12);
}

void Actor_SetActionById(s32 id, s32 action);
/* VIEW(s1c00): routine FieldCamera_MoveToActor as void
 * FieldCamera_MoveToActor(s32 a, s32 b); event.h has s32
 * FieldCamera_MoveToActor(s32 actorId, s32 pan); tried as the definition's void
 * FieldCamera_MoveToActor(s32 id, s32 use_setter), it does not compile in
 * tla-ja (magma_rock.c): games/tla/src/scene/magma_rock.c:1170: conflicting
 * types for `FieldCamera_MoveToActor'. */
extern void Func_080d452c__s1c00(s32 a,
                                 s32 b) __asm__("FieldCamera_MoveToActor");
void FieldCamera_WaitForArrival(void);

void MagmaRock_AmplifyBurst(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct MagmaRockEffectOptions options;
  s32 position[3];
  struct MagmaRockEffectOptions *opt;
  s32 *offset;
  s32 i;
  u32 phase;

  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetFlag(FLAG_SCENE_TEMP_60);
  Event_PlaySound(SE_IMPACT);
  FieldActor_SetSpriteColor(Actor_GetById(gMainRoutineWork.currentOwner), 6);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 10);
  Actor_SetActionById(gMainRoutineWork.currentOwner, 32);
  opt = &options;
  opt->kind = 1;
  opt->palette = 14;
  opt->scaleX = 0xcccc;
  opt->scaleY = 0xcccc;
  i = 0;
first:
  {
    s32 r = Random16();

    offset = position;
    offset[0] = ((r & 15) << 16) - 0x80000;
  }
  offset[1] = (Random16() & 15) << 16;
  FieldEffect_Spawn(leader->x + offset[0], leader->y + offset[1], leader->z, 0,
                    0x13333, 0, 0xb0000, opt);
  wait(5);
  i++;
  if (i != 8)
    goto first;
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  gMainRoutineWork.renderMode = 0;
  leader->motionFlags = 3;
  leader->gravity = 0xcccc;
  leader->bounce = 0;
  Event_Wait(1);
  leader->facing = 0xc000;
  i = 60;
second:
  phase = gFrameCount & 1;
  if (phase == 0) {
    offset[0] = ((Random16() & 15) << 16) - 0x80000;
    offset[1] = (Random16() & 15) << 16;
    FieldEffect_Spawn(leader->x + offset[0], leader->y + offset[1], leader->z,
                      0, 0x13333, phase, 0xb0000, opt);
  }
  wait(1);
  if (leader->velocityY != 0) {
    i--;
    if (i != 0)
      goto second;
  }
  Event_PlaySound(SE_THUMP);
  Actor_SetSpriteFlags(leader, 1);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 22);
  MapGrid_SetShake(5, 8, 1);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  MapGrid_WaitForShakeToSettle();
  Func_080d452c__s1c00(gMainRoutineWork.currentOwner, 1);
  FieldCamera_WaitForArrival();
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 4);
  FieldActor_SetSpriteColor(Actor_GetById(gMainRoutineWork.currentOwner), 0);
  Actor_WaitForAnimationChange(gMainRoutineWork.currentOwner);
  Event_ClearFlag(FLAG_SCENE_TEMP_60);
  leader->gravity = px(1);
  leader->bounce = Q16_ONE / 4;
  Func_02003af4();
}

void MagmaRockInside_ClearBlockCells(struct Actor *object);
void MagmaRockInside_PushBlockOnto(void);

void MagmaRock_PushPillarEast(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  s32 position[3];
  s32 *cell = position;
  struct Actor *object;
  u8 *terrainId;

  cell[0] = leader->x + px(16);
  cell[1] = leader->y;
  cell[2] = leader->z;
  object = Actor_FindInCell(cell, leader);
  MagmaRockInside_ClearBlockCells(object);
  terrainId = &object->terrainId;
  {
    /* FAKEMATCH(temporary): the three values held in locals. */
    s32 x = object->x >> 20;
    s32 z = object->z >> 20;
    s32 height = object->terrainHeight >> 19;

    MagmaRockInside_SetCellShape(*terrainId, x, z, height);
  }
  MapGrid_CopyCellAttributes(79, 46, 2, 1, 79, 36);
  MagmaRockInside_PushBlockOnto();
  *terrainId = 0;
  MapGrid_CopyCellAttributes(79, 37, 2, 1, 79, 36);
  MagmaRockInside_SetCellShape(*terrainId, object->x >> 20, object->z >> 20,
                               (object->terrainHeight >> 19) + 4);
  Blocks_MarkCells(gMagmaRock_Room2UpperPillars);
  cell[0] = object->x >> 20;
  cell[2] = object->z >> 20;
  if (cell[0] == 16 && cell[2] == 36)
    Event_SetFlag(0x8d9);
}

void MagmaRock_PushPillarWest(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  s32 position[3];
  struct Actor *object;
  u8 *terrainId;

  position[0] = leader->x - px(16);
  position[1] = leader->y;
  position[2] = leader->z;
  object = Actor_FindInCell(position, leader);
  MagmaRockInside_ClearBlockCells(object);
  terrainId = &object->terrainId;
  {
    /* FAKEMATCH(temporary): the three values held in locals. */
    s32 x = object->x >> 20;
    s32 z = object->z >> 20;
    s32 height = object->terrainHeight >> 19;

    MagmaRockInside_SetCellShape(*terrainId, x, z, height);
  }
  MapGrid_CopyCellAttributes(94, 44, 5, 1, 99, 38);
  MagmaRockInside_PushBlockOnto();
  *terrainId = 0;
  MapGrid_CopyCellAttributes(99, 42, 5, 1, 99, 38);
  MagmaRockInside_SetCellShape(*terrainId, object->x >> 20, object->z >> 20,
                               (object->terrainHeight >> 19) + 4);
  Blocks_MarkCells(gMagmaRock_Room2TopPillars);
  position[0] = object->x >> 20;
  position[2] = object->z >> 20;
  if (position[0] == 35 && position[2] == 38)
    Event_SetFlag(0x8db);
}

void MagmaRock_GrowVine1(void) {
  MapGrid_CopyCellAttributes(121, 48, 1, 3, 123, 48);
  Event_SetFlag(FLAG_SCENE_TEMP_50);
}

void MagmaRock_GrowVine2(void) {
  MapGrid_CopyCellAttributes(121, 48, 1, 3, 125, 48);
  Event_SetFlag(FLAG_SCENE_TEMP_51);
}

void MagmaRock_GrowVine3(void) {
  MapGrid_CopyCellAttributes(108, 56, 1, 3, 109, 56);
  Event_SetFlag(FLAG_SCENE_TEMP_52);
}

void MagmaRock_ScatterEmbers(s32 x, s32 z) {
  struct MagmaRockEffectConfig config;
  s32 position[3];
  s32 i;
  s32 kind;

  config.unk08 = Q16_ONE * 2;
  config.unk0c = Q16_ONE * 2;
  config.id = 403;
  kind = 1; // Holds one constant: the native loads 1 before the data address.
  config.data = gMagmaRock_EmberScript;
  config.kind = kind;
  i = 0;
loop:
  position[0] = x + ((Random16() & 63) << 16) - Q16_ONE * 32;
  position[1] = (Random16() & 15) << 12;
  position[2] = z + ((Random16() & 63) << 16) - Q16_ONE * 32;
  FieldEffect_Spawn(position[0], 0, position[2], 0, position[1], 0, 0x3a0001,
                    &config);
  i++;
  if (i != 4)
    goto loop;
  Event_SetFlag(0x8dc);
}

void MagmaRock_FlashTarget(struct Actor *object) {
  if (gFrameCount & object->unk5c[7])
    FieldActor_SetSpriteColor(object, 7);
  else
    FieldActor_SetSpriteColor(object, 0);
}

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))

// MagmaRock_BurstStatue's counterpart: on cue 0 it hands the target to
// MagmaRock_FlashTarget; modes 2 and 3 run MagmaRock_ScatterEmbers at the
// target's cell, and mode 3 also clears its position and copies the cells at
// columns 32 and 96.
void MagmaRock_BlastOpenWall(s32 mode, s32 cue) {
  struct MagmaRockWork *work = Ram_HeapSlots->effectWork;

  if (cue == 0) {
    struct Actor *object;

    MagmaRock_StartFallingRocks();
    object = work->target;
    BYTE_AT(object, 99) = 4;
    object->update = (void *)MagmaRock_FlashTarget;
  }
  if (mode == 2) {
    MagmaRock_ScatterEmbers(work->target->x, work->target->z);
    MagmaRock_DropRocks(work->target->x, work->target->y + px(15),
                        work->target->z, 1);
  }
  if (mode == 3) {
    struct Actor *target;
    s32 last = 8;

    MagmaRock_ScatterEmbers(work->target->x, work->target->z);
    MagmaRock_ScatterEmbers(work->target->x, work->target->z);
    MagmaRock_ScatterEmbers(work->target->x, work->target->z - px(32));
    MagmaRock_DropRocks(work->target->x, work->target->y + px(15),
                        work->target->z, 16);
    target = work->target;
    SET_POSITION(target, 0, 0, 0);
    MapGrid_CopyCellAttributes(0, 32, 5, 8, 42, 17);
    MapGrid_CopyCellAttributes(0, 96, 5, 8, 42, 81);
    MapGrid_CopyCellsTo(0, 32, 42, 17, 5, last);
    MapGrid_CopyCellsTo(0, 96, 42, 81, 5, last);
  }
  if (cue == 0x3e7)
    MagmaRock_EndFallingRocks();
}

extern struct Trigger gMagmaRock_Triggers[];
extern struct Trigger gMagmaRock_Room2LowerTriggers[];
extern struct Trigger gMagmaRock_Room2MiddleTriggers[];
extern struct Trigger gMagmaRock_Room2UpperTriggers[];
extern struct Trigger gMagmaRock_Room2TopTriggers[];
extern struct Trigger gMagmaRock_Room3Triggers[];

const struct Trigger *Scene_GetEvents(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_MagmaRock2) {
    if (gMainRoutineWork.entrance <= 3)
      return gMagmaRock_Room2LowerTriggers;
    if (gMainRoutineWork.entrance <= 8)
      return gMagmaRock_Room2MiddleTriggers;
    if (gMainRoutineWork.entrance <= 12)
      return gMagmaRock_Room2UpperTriggers;
    return gMagmaRock_Room2TopTriggers;
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_MagmaRock3)
    return gMagmaRock_Room3Triggers;
  return gMagmaRock_Triggers;
}
#undef BYTE_AT

#undef MagmaRockEventWork
#undef MagmaRock_PlayBurst
#undef MagmaRockEffectConfig
#undef Blocks_MarkCells
#undef Position

#include "../field/field_effect/advance_motion.c"

#undef BYTE_AT
#undef PART_Y
#undef EFFECT_Y
#undef DEFINE_MAGMA_ROCK_BURST_COLUMN
#undef DEFINE_MAGMA_ROCK_BURST_STATUE_COLUMN
#undef EFFECT_SPAWN
#undef EFFECT_CREATE
#undef EFFECT_SET_MODE
#undef EFFECT_SET_SCRIPT
#undef EFFECT_SCRIPTS
#undef EFFECT_SPRITE_PRIORITY
#undef TLA_MAGMA_ROCK_H
#undef BYTE_AT
#undef HALF_AT
#undef gMagmaRock_Room2LowerPlacements
#undef gMagmaRock_Room2MiddlePlacements
#undef gMagmaRock_Room2UpperPlacements
#undef MagmaRockEventWork
#undef MagmaRock_PlayBurst
#undef Position
#undef MagmaRockEffectConfig
/* VIEW(s22ac): tag struct MagmaRockEventWork as this part has it; the joined
 * file has another; tried with the definition the rest of the object has, it
 * does not compile in tla-ja (magma_rock.c):
 * games/tla/src/scene/magma_rock.c:1470: redefinition of `struct
 * MagmaRockEventWork'. */
#define MagmaRockEventWork MagmaRockEventWork__s22ac
// @file
// Magma Rock's rooms set up, the leader launched and landed, and the
// crumbling pillars and traps.

struct MagmaRockEventWork {
  u8 unk000[0x1ac];
  s32 field1ac;
};

s32 Trig_Cos(s32 angle);
void FieldEffect_AdvanceMotion();
void Map_CenterOnFollowedPoint(void);

void MagmaRock_LaunchLeader(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  struct MagmaRockEffectOptions options;
  u32 i;
  s32 angle;
  s32 velocity[3];

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  Map_CenterOnFollowedPoint();
  System_Wait(1);
  leader->y += px(130);
  leader->gravity = px(1) / 2;
  leader->bounce = 0;
  leader->motionFlags = 0;
  ((struct MagmaRockEventWork *)Ram_HeapSlots->fieldWork)->field1ac = 0x100;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_PlaySound(SE_DUST_PUFF);
  leader->motionFlags = 3;
  FieldEvent_Wait(24);
  options.palette = 7;
  options.update = FieldEffect_AdvanceMotion;
  options.scaleX = 0xcccc;
  options.scaleY = 0xcccc;
  i = 0;
loop:
  angle = i << 12;
  velocity[0] = Trig_Cos(angle);
  velocity[1] = 0;
  velocity[2] = Math_Sin(angle);
  velocity[0] += velocity[0] >> 1;
  velocity[0] += ((Random16() * 3) << 14 >> 16) - 0x6000;
  velocity[2] += ((Random16() * 3) << 13 >> 16) - 0x3000;
  FieldEffect_Spawn(leader->x, leader->y, leader->z, velocity[0], velocity[1],
                    velocity[2], 0x1090001, &options);
  i++;
  if (i <= 16)
    goto loop;
  Event_PlaySound(SE_THUMP);
  Actor_SetAttachedEffect(gMainRoutineWork.currentOwner,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 22);
  MapGrid_SetShake(5, 5, 1);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  FieldMapGrid_WaitForShakeToSettle();
  Actor_SetAttachedEffect(gMainRoutineWork.currentOwner,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_REMOVE);
  leader->gravity = px(1);
  leader->bounce = Q16_ONE / 4;
  ((struct MagmaRockEventWork *)Ram_HeapSlots->fieldWork)->field1ac = 0x204;
  Func_02003af4();
}

#include "shared/leader_lift.h"

s32 Map_GetTerrainHeight(s32 terrain, s32 x, s32 z);

void MagmaRock_LandLeader(void) {
  struct Actor *object = Actor_Get(gMainRoutineWork.currentOwner);

  if (Event_GetFlag(FLAG_KEEP_PARTY_POSITION)) {
    s32 height;

    System_Wait(1);
    height = Map_GetTerrainHeight(object->terrainId, object->x, object->z);
    object->velocityY = 0;
    object->terrainHeight = height;
    object->y = height;
    object->targetY = px(-32768);
    EventCamera_FollowActor(gMainRoutineWork.currentOwner, 0);
  }
}

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))

struct LayeredSprite {
  u8 unk00[4];
  u32 attributes;
  u8 unk08[4];
  s32 scale;
  u8 slot;
  u8 retainCacheEntry : 1;
  u8 useOwnScale : 1;
  u8 unk11 : 6;
  u16 rotation;
  u8 width;
  u8 height;
  s8 offsetX;
  s8 offsetY;
  u8 lastNo;
  u8 dirty;
  u8 flags;
  u8 count;
  struct AnimationSpritePart shadow;
  struct AnimationEntry *entries[4];
};

static inline void AttachPart(struct Actor *part, struct Actor *anchor,
                              u8 order) {
  BYTE_AT(part, 98) = order;
  part->watchedObject = anchor;
  part->update = MagmaRock_RaiseFlamePart;
  FieldActor_SetSpriteColor(part, 3);
}

static inline void BuildParts(struct Actor *anchor) {
  struct Actor *parts[4];
  struct Actor *part;
  struct LayeredSprite *first;
  struct LayeredSprite *sprite;
  u32 i;

  parts[0] = ((struct Actor *)Import_ActorCreate(
      0x15f, anchor->x + px(1), anchor->y + px(32), anchor->z));
  i = 1;
next:
  first = parts[0]->sprite;
  part = ((struct Actor *)Import_ActorCreate(0x15f, 0, 0, 0));
  parts[i] = part;
  sprite = part->sprite;
  sprite->slot = first->slot;
  sprite->retainCacheEntry = 1;
  part->x = parts[0]->x;
  part->y = parts[0]->y;
  part->z = parts[0]->z;
  part->motionFlags = 0;
  Actor_SetSpriteFlags(part, 0);
  Actor_SelectAnimation(parts[i], 3);
  AttachPart(parts[i], anchor, i - 1);
  if (++i != 4)
    goto next;
  part = parts[0];
  part->motionFlags = 0;
  Actor_SetSpriteFlags(part, 0);
  Actor_SelectAnimation(parts[0], 3);
  Scene_SetActorSpritePriority(parts[0], 3);
  AttachPart(parts[0], anchor, 3);
}

void MagmaRock_RebuildFlameParts(u16 id) {
  if (Actor_Get(id)->update != MagmaRock_RaiseFlameTop)
    return;
  BuildParts(Actor_Get(id)->watchedObject);
}

void MagmaRock_SettleBlocks();
/// The overlay's import veneer to Rope_PlaceFar.
void Func_02003bfc();

s32 Scene_Initialize(void) {
  struct MainRoutineWork *work;
  s32 scene;
  s32 *owner, *leader;
  s16 *entrance;
  struct Actor *actor;
  // other serves in turn as a table, a step and a size.
  s32 other, rise;
  s32 offset;
  s32 value1, value2, value3, value4;
  u8 zero;

  gFieldWork->transition = 0x204;
  work = &gMainRoutineWork;
  work->retreatScene = (s32)&SceneId_MagmaRock1;
  work->retreatEntrance = 1;
  scene = work->scene;
  zero = 0;
  if (scene == (s32)&SceneId_MagmaRock1) {
    value1 = 32;
    Actor_GetById(work->currentOwner)->priorityFlags |= value1;
    Actor_GetById(8)->priorityFlags |= value1;
    Actor_GetById(10)->priorityFlags |= value1;
    MagmaRockInside_RaiseActorCells(gMagmaRock_Room1Statues);
    if (Event_GetFlag(FLAG_KEEP_PARTY_POSITION) == 0)
      MagmaRock_PlaceVolley(21, 13, 0);
    if (Event_GetFlag(0x8d5))
      Actor_SetPosition(8, 264, 360);
    if (Event_GetFlag(0x8d6))
      Actor_SetPosition(10, 248, 408);
    other = (s32)gMagmaRock_Room1Pillars;
    value2 = 78;
    Actor_PlaceBlocks(other);
    MapGrid_CopyCellAttributes(98, 21, 4, 3, value2, 21);
    MapGrid_CopyCellAttributes(98, 27, 4, 3, value2, 24);
    Blocks_MarkCells(other);
    if (Event_GetFlag(FLAG_KEEP_PARTY_POSITION) == 0) {
      actor = Actor_GetById(20);
      actor->x += px(1);
      actor->motionFlags = zero;
      actor->collisionFlags = zero;
      actor->terrainHeight = actor->y += px(24);
      Actor_SetSpriteFlags(actor, 0);
    }
    MagmaRock_RebuildFlameParts(20);
    switch (work->entrance) {
    case 3:
    case 4:
      offset = 0x100000;
      goto move;
    }
    goto settle;
  } else if (scene == (s32)&SceneId_MagmaRock2) {
    owner = &work->currentOwner;
    Actor_GetById(*owner)->priorityFlags |= 32;
    entrance = &work->entrance;
    if (work->entrance <= 3) {
      s32 id;

      MagmaRock_RestoreCrumblingPillars(gMagmaRock_CrumblingPillars);
      MagmaRockInside_RaiseActorCells(gMagmaRock_Room2LowerStatues);
      Tightrope_Initialize();
      Func_02003bf4(0, 11, 12);
      if (Event_GetFlag(0x8d7))
        Actor_SetPosition(15, 440, 536);
      Actor_PlaceBlocks(gMagmaRock_Room2LowerPillars);
      id = *entrance;
      if (id == 1) {
        if (Event_GetFlag(FLAG_KEEP_PARTY_POSITION) == 0) {
          actor = Actor_GetById(*owner);
          actor->z += px(16);
        }
        Leader_Lift(-0x100000);
      } else if (id == 2) {
        if (Event_GetFlag(FLAG_KEEP_PARTY_POSITION) == 0) {
          actor = Actor_GetById(*owner);
          actor->z += px(16);
        }
        Leader_Lift(-0x600000);
      }
    } else {
      if (*entrance <= 8) {
        s32 id;

        if (Event_GetFlag(FLAG_KEEP_PARTY_POSITION) == 0) {
          Actor_GetById(8)->motionFlags = zero;
          rise = 0x500000;
          Actor_GetById(8)->y += rise;
          Actor_GetById(9)->motionFlags = zero;
          Actor_GetById(9)->y += rise;
          MagmaRock_PlaceVolley(19, 8, 0);
          Actor_GetById(10)->motionFlags = zero;
          rise = 0xa00000;
          Actor_GetById(10)->y += rise;
          Actor_GetById(11)->motionFlags = zero;
          Actor_GetById(11)->y += rise;
          MagmaRock_PlaceVolley(20, 10, 1);
          Actor_GetById(12)->motionFlags = zero;
          rise = 0xf00000;
          Actor_GetById(12)->y += rise;
          Actor_GetById(13)->motionFlags = zero;
          Actor_GetById(13)->y += rise;
          MagmaRock_PlaceVolley(21, 12, 0);
        }
        if (Event_GetFlag(0x8d8))
          Actor_SetPosition(16, 888, 568);
        Actor_PlaceBlocks(gMagmaRock_Room2MiddlePillars);
        MagmaRockInside_RaiseActorCells(gMagmaRock_Room2MiddleStatues);
        if (Event_GetFlag(FLAG_KEEP_PARTY_POSITION) == 0) {
          actor = Actor_GetById(15);
          other = 0x10000;
          actor->x += other;
          actor->motionFlags = zero;
          actor->collisionFlags = zero;
          rise = 0x180000;
          actor->terrainHeight = actor->y += rise;
          Actor_SetSpriteFlags(actor, 0);
          actor = Actor_GetById(18);
          actor->x += other;
          actor->motionFlags = zero;
          actor->collisionFlags = zero;
          actor->terrainHeight = actor->y += rise;
          Actor_SetSpriteFlags(actor, 0);
        }
        MagmaRock_RebuildFlameParts(15);
        MagmaRock_RebuildFlameParts(18);
        id = *entrance;
        if (id == 5) {
          Leader_Lift(0x1200000);
        } else if (id == 6) {
          if (Event_GetFlag(FLAG_KEEP_PARTY_POSITION) == 0)
            MagmaRock_LaunchLeader();
        } else if (id == 7) {
          if (Event_GetFlag(FLAG_KEEP_PARTY_POSITION) == 0) {
            actor = Actor_GetById(*owner);
            actor->z += px(16);
          }
          Leader_Lift(-0x300000);
        } else if (id == 8) {
          offset = 0x680000;
          goto move;
        }
      } else if (*entrance <= 12) {
        if (Event_GetFlag(0x8d9))
          Actor_SetPosition(8, 264, 584);
        rise = (s32)gMagmaRock_Room2UpperPillars;
        Actor_PlaceBlocks(rise);
        Blocks_MarkCells(rise);
        Rope_Initialize();
        Call4((void (*)())Func_02003bfc, 0, 0x250, 14, 13);
        MagmaRockInside_RaiseActorCells(gMagmaRock_Room2UpperStatues);
        if (Event_GetFlag(FLAG_KEEP_PARTY_POSITION) == 0) {
          actor = Actor_GetById(10);
          other = 0x10000;
          actor->x += other;
          actor->motionFlags = zero;
          actor->collisionFlags = zero;
          rise = 0x180000;
          actor->terrainHeight = actor->y += rise;
          Actor_SetSpriteFlags(actor, 0);
          actor = Actor_GetById(12);
          actor->x += other;
          actor->motionFlags = zero;
          actor->collisionFlags = zero;
          actor->terrainHeight = actor->y += rise;
          Actor_SetSpriteFlags(actor, 0);
        }
        MagmaRock_RebuildFlameParts(10);
        MagmaRock_RebuildFlameParts(12);
        if (*entrance == 9 || *entrance == 10) {
          offset = 0x300000;
          goto move;
        }
      } else {
        MagmaRockInside_RaiseActorCells(gMagmaRock_Room2TopStatues);
        if (Event_GetFlag(FLAG_KEEP_PARTY_POSITION) == 0) {
          actor = Actor_GetById(13);
          actor->x += px(1);
          actor->motionFlags = zero;
          actor->collisionFlags = zero;
          actor->terrainHeight = actor->y += px(24);
          Actor_SetSpriteFlags(actor, 0);
        }
        MagmaRock_RebuildFlameParts(13);
        if (Event_GetFlag(0x8da))
          Actor_SetPosition(8, 648, 600);
        if (Event_GetFlag(0x8db))
          Actor_SetPosition(9, 568, 616);
        MagmaRock_SettleBlocks(gMagmaRock_Room2TopPillars);
        if (Event_GetFlag(FLAG_SCENE_TEMP_50))
          MapGrid_CopyCellAttributes(121, 48, 1, 3, 123, 48);
        if (Event_GetFlag(FLAG_SCENE_TEMP_51))
          MapGrid_CopyCellAttributes(121, 48, 1, 3, 125, 48);
        if (Event_GetFlag(FLAG_SCENE_TEMP_52))
          MapGrid_CopyCellAttributes(108, 56, 1, 3, 109, 56);
        if (*entrance == 14) {
          Leader_Lift(0x300000);
        } else if (*entrance == 16 || *entrance == 17) {
          if (Event_GetFlag(FLAG_KEEP_PARTY_POSITION) == 0) {
            actor = Actor_GetById(work->currentOwner);
            actor->z += px(16);
          }
          offset = -0x200000;
        move:
          Leader_Lift(offset);
        }
      }
    settle:
      MagmaRock_LandLeader();
    }
  } else if (scene == (s32)&SceneId_MagmaRock3) {
    if (Event_GetFlag(0x8dc)) {
      Actor_SetPosition(8, 0, 0);
      value3 = 42;
      MapGrid_CopyCellAttributes(0, 32, 5, 8, value3, 17);
      MapGrid_CopyCellAttributes(0, 96, 5, 8, value3, 81);
      value4 = 5;
      other = 8;
      MapGrid_CopyCellsTo(0, 32, 42, 17, value4, other);
      MapGrid_CopyCellsTo(0, 96, 42, 81, value4, other);
    }
    leader = &work->currentOwner;
    Actor_GetById(*leader)->priorityFlags |= 32;
    MagmaRockInside_RaiseActorCells(gMagmaRock_Room3Statues);
    if (work->entrance <= 2) {
      if (Event_GetFlag(FLAG_KEEP_PARTY_POSITION) == 0) {
        actor = Actor_GetById(*leader);
        actor->z += px(16);
      }
      Leader_Lift(-0x200000);
    }
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

// On entering, sets each listed pillar from its two flags, then lifts the
// leader onto the ground under it.

extern u8 gMapShapeGrid[];

s32 MapGrid_GetTerrainKind(s32 layer, s32 x, s32 z);

void MagmaRock_RestoreCrumblingPillars(const u16 *table) {
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
  object = Actor_Get(id);
  flag = table[1];
  nextFlag = flag + 1;
  cells = map->layers[object->terrainId].cells;
  shapes = gMapShapeGrid + (cells - (struct MapCell *)Ram_MapCellBuffer);
  if (Event_GetFlag(nextFlag)) {
    Actor_SetPosition(id, 0, 0);
  } else {
    s32 shape;
    u8 *slot;
    s32 height;

    index = Actor_GetCellIndex(object);
    cell = &cells[index];
    slot = shapes + index;
    shape = MapGrid_GetTerrainKind(object->terrainId, object->x, object->z);
    height = Map_GetTerrainHeight(object->terrainId, object->x, object->z);
    Actor_DisableById(id);
    cell->attributeB = 128;
    *slot =
        MapCollision_FindOrAddTag(object->terrainId, shape, (height >> 19) + 4);
    if (Event_GetFlag(flag))
      Actor_SelectAnimation(object, 0);
  }
  table += 2;
  if (*table != 0xffff)
    goto next;
done:
  leader = Actor_Get(gMainRoutineWork.currentOwner);
  ground = Map_GetTerrainHeight(leader->terrainId, leader->x, leader->z);
  if (leader->y < ground) {
    leader->terrainHeight = ground;
    leader->y = ground;
  }
}

void MagmaRock_SpawnDustRing(void);

void MagmaRock_StepOnCrumblingPillar(u16 *list) {
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
  if (target != NULL)
    goto test;
  return;
body:
  {
    s32 id = list[0];

    if (Actor_GetById(id) != target) {
      list += 2;
      goto test;
    }
    {
      s32 flag = list[1];

      Event_Start();
      if (!Event_GetFlag(flag)) {
        Event_PlaySound(SE_CHEST_CLOSE);
        FieldActor_SetSpriteColor(Actor_GetById(id), 7);
        wait(2);
        Actor_SelectAnimation(target, 0);
        FieldActor_SetSpriteColor(Actor_GetById(id), 0);
        wait(2);
        FieldActor_SetSpriteColor(Actor_GetById(id), 7);
        wait(4);
        FieldActor_SetSpriteColor(Actor_GetById(id), 0);
        MagmaRock_SpawnRubble(0);
        Event_SetFlag(flag);
      } else if (!Event_GetFlag(flag + 1)) {
        leader->motionFlags = 0;
        Event_PlaySound(SE_PUSH);
        MapGrid_SetShake(1, 4, 1);
        MagmaRock_SpawnRubble(0);
        wait(8);
        Actor_SelectAnimation(target, 2);
        MagmaRock_SetLeaderCellShape(-2, target->terrainId);
        MagmaRock_SpawnRubble(1);
        wait(16);
        MagmaRock_SetLeaderCellShape(-2, target->terrainId);
        wait(4);
        MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
        wait(8);
        leader->motionFlags = 3;
        wait(5);
        Actor_SetPositionAndResetMotion(target, 0, 0, 0);
        wait(2);
        Event_PlaySound(SE_THUMP);
        MagmaRock_SpawnDustRing();
        wait(20);
        Event_SetFlag(flag + 1);
      }
    }
    Func_02003af4();
    return;
  }
test:
  if (list[0] != 0xffff)
    goto body;
}

void MagmaRock_CrumblePillar(struct MagmaRockCrumblingPillar *entry, s32 id) {
  struct Actor *target;
  s32 flag;
  s32 nextFlag;

  target = Actor_Get(id);
  if (entry->id == 0xffff)
    return;
body:
  if (entry->id != id) {
    entry++;
    goto test;
  }
  flag = entry->flag;
  Event_Start();
  nextFlag = flag + 1;
  if (Event_GetFlag(nextFlag) == 0) {
    Event_PlaySound(SE_PUSH);
    MapGrid_SetShake(1, 4, 1);
    MagmaRock_SpawnRubble(0);
    System_Wait(8);
    Actor_SelectAnimation(target, 2);
    MagmaRock_SetActorCellShape(-4, target->terrainId, id);
    MagmaRock_SpawnRubble(1);
    System_Wait(16);
    MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
    System_Wait(8);
    FieldActor_SetPositionAndResetMotion(target, 0, 0, 0);
    System_Wait(2);
    Event_PlaySound(SE_THUMP);
    MagmaRock_SpawnDustRing();
    System_Wait(20);
    Event_SetFlag(flag);
    Event_SetFlag(nextFlag);
  }
  Func_02003af4();
  return;
test:
  if (entry->id != 0xffff)
    goto body;
}

void MagmaRockInside_SetPillarMode3(s32 id) {
  Actor_Get(id)->unk5c[0] = 3;
  Actor_DisableById(id);
}

// Places the listed actors on their cells.

void Actor_MoveToObject(s32 id, s32 owner);

// Places actors 8, 9 and 10, then walks a 0xffff-ended list of (actor, flag)
// halfword pairs: flag + 1 set parks the actor at (0, 0); otherwise its cell
// gets attributeB 128 and its shape-grid byte is rewritten, and flag set makes
// it copy actor 9's visual attributes.
#include "shared/pillars_restore.h"

void MagmaRock_TakePlaceOf(struct Actor *object, struct Actor *source) {
  FieldActor_SetPositionAndResetMotion(object, source->x, source->y, source->z);
  FieldActor_SetPositionAndResetMotion(source, 0, 0, 0);
}

void MagmaRock_StepOnTrap(u16 *list) {
  struct Actor *leader;
  struct Actor *actor8;
  struct Actor *target;
  s32 position[3];

  if (Field_TryJumpForward() != 0)
    return;
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  actor8 = Actor_GetById(8);
  position[0] = leader->x;
  position[1] = leader->y;
  position[2] = leader->z;
  target = Actor_FindAtCell(position, 0);
  if (target == 0)
    return;
  /* FAKEMATCH(dead): a test no u16 can pass enters the loop a second way. */
  if (list[0] & 0x10000)
    goto body;
  goto test;
body:
  {
    u32 id = list[0];

    if (Actor_GetById(id) != target) {
      list += 2;
      goto test;
    }
    {
      s32 flag = list[1];

      Event_Start();
      if (!Event_GetFlag(flag)) {
        Actor_SelectAnimation(actor8, 1);
        MagmaRock_TakePlaceOf(actor8, target);
        wait(1);
        Event_PlaySound(SE_CHEST_CLOSE);
        FieldActor_SetSpriteColor(Actor_GetById(8), 7);
        wait(2);
        Actor_SelectAnimation(actor8, 0);
        Actor_CopyGraphics(id, 9);
        FieldActor_SetSpriteColor(Actor_GetById(8), 0);
        wait(2);
        FieldActor_SetSpriteColor(Actor_GetById(8), 7);
        wait(4);
        FieldActor_SetSpriteColor(Actor_GetById(8), 0);
        MagmaRock_SpawnRubble(0);
        MagmaRock_TakePlaceOf(target, actor8);
        wait(1);
        Event_SetFlag(flag);
      } else if (!Event_GetFlag(flag + 1)) {
        u8 *flags;

        Actor_SelectAnimation(actor8, 0);
        MagmaRock_TakePlaceOf(actor8, target);
        wait(1);
        flags = &leader->motionFlags;
        *flags = 0;
        Event_PlaySound(SE_PUSH);
        MapGrid_SetShake(1, 4, 1);
        MagmaRock_SpawnRubble(0);
        wait(8);
        Actor_SelectAnimation(actor8, 2);
        MagmaRock_SetLeaderCellShape(-2, target->terrainId);
        MagmaRock_SpawnRubble(1);
        wait(16);
        MagmaRock_SetLeaderCellShape(-2, target->terrainId);
        wait(4);
        MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
        wait(8);
        *flags = 3;
        wait(5);
        Actor_SetPositionAndResetMotion(actor8, 0, 0, 0);
        wait(2);
        Event_PlaySound(SE_THUMP);
        MagmaRock_SpawnDustRing();
        wait(20);
        Event_SetFlag(flag + 1);
      }
      Func_02003af4();
      return;
    }
  }
test:
  if (list[0] != 0xffff)
    goto body;
}

#include "../field/field_effect/advance_motion_priority.c"

// Magma Rock's rubble and falling rocks, and its tables.

void Actor_StartScript(struct Actor *object, const s32 *script);

void MagmaRock_SpawnRubble(s32 spread) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  struct FieldEffect *effect;
  u32 frame = 0;
  u32 i = 0;

  do {
    {
      /* FAKEMATCH(temporary): y held in a local. */
      s32 y = leader->y - (((Random16() * 14) >> 16) << 16);

      effect = (struct FieldEffect *)FieldActor_Create(
          285, (i << 17) + leader->x - px(8), y - 0x20000, leader->z);
    }
    if (effect != 0) {
      frame = Sprite_ShareVramCacheEntry(effect->sprite, frame);
      effect->motionFlags = 0;
      effect->collisionFlags = 0;
      Actor_SetSpriteFlags(effect, 0);
      Actor_SelectAnimation(effect, 2);
      Actor_StartScript(effect, gMagmaRock_RubbleScript);
      effect->scaleY = effect->scaleX = 0xb333;
      effect->sprite->priority = 1;
      if (spread != 0) {
        effect->velocity.x = Iwram_MulQ16(Trig_Cos(i << 13), 0x13333);
        effect->velocity.z = Math_Sin(i << 13);
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

void MagmaRock_SpawnDustRing(void) {
  struct Actor *leader;
  struct FieldEffect *effect;
  s32 state;
  u32 i;
  s32 angle;
  u8 zero;

  leader = Actor_Get(gMainRoutineWork.currentOwner);
  if (leader == 0)
    return;
  zero = 0;
  state = 0;
  for (i = 0; i <= 16; i++) {
    effect = (struct FieldEffect *)FieldActor_Create(285, leader->x, leader->y,
                                                     leader->z);
    if (effect != 0) {
      state = Sprite_ShareVramCacheEntry(effect->sprite, state);
      zero = 0;
      effect->motionFlags = zero;
      effect->collisionFlags = zero;
      Actor_SetSpriteFlags((struct Actor *)effect, 0);
      Actor_SelectAnimation(effect, 2);
      Actor_StartScript(effect, gMagmaRock_DustScript);
      angle = i << 12;
      effect->velocity.x = Trig_Cos(angle);
      effect->velocity.y = zero;
      effect->velocity.z = Math_Sin(angle);
      effect->velocity.x += effect->velocity.x >> 1;
      effect->velocity.x += (Random16() * 0xc000 >> 16) - 0x6000;
      effect->velocity.z += (Random16() * 0x6000 >> 16) - 0x3000;
      effect->spin = (Random16() * 0x1000 >> 16) - 0x800;
      effect->scaleRateX = zero;
      effect->scaleRateY = zero;
      effect->update = Effect_AdvanceMotionAtLeaderPriority;
      effect->sprite->priority =
          ((struct FieldSprite *)leader->sprite)->priority;
    }
  }
}

s32 MapGrid_GetTerrainKind(s32 terrainId, s32 x, s32 z);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);

#include "shared/actor_set_cell_shape_at.h"

void MagmaRock_SetLeaderCellShape(s32 a, s32 b) {
  MagmaRock_SetActorCellShape(a, b, gMainRoutineWork.currentOwner);
}

struct ParticleSprite {
  u8 unk0[8];
  u16 attr;
  u8 unkA[30];
};

struct ParticleWork {
  u8 tiles[0x500];
  struct Particle particles[64];
  struct ParticleSprite sprites[64];
  s16 next;
  u16 tile;
};

void BattleEffect_StepVector3(struct Particle *particle, s32 unk1, s32 step);

#include "shared/particles_update_falling.h"

void MagmaRock_DropRocks(s32 x, s32 y, s32 z, s32 count) {
  struct EntryWork *work;
  s32 i;

  work = Ram_HeapSlots->menuRuntime;
  if (count > 0) {
    i = count;
  next:
    {
      struct Entry *entry;
      s32 angle = Random16();

      entry = &work->entries[work->unk1600];
      entry->unk18 = 0;
      entry->x = x;
      entry->y = y;
      entry->z = z;
      FieldVector_AddPolarOffset(Random16() * 8 + 0x80000, angle, entry);
      entry->velocityX = 0;
      entry->velocityY = px(5);
      entry->velocityZ = 0;
      FieldVector_AddPolarOffset(Random16() * 2 + 0x20000, angle,
                                 &entry->velocityX);
      {
        s32 mask = 63;

        work->unk1600 = (work->unk1600 + 1) & mask;
      }
      i--;
    }
    if (i != 0)
      goto next;
  }
}

// The falling rocks' setup: loads their picture, sets up 64 small sprites,
// marks each rock idle and starts the task that spawns them.

extern char ResourceId_FallingRock;

struct Rock {
  u8 unk00[24];
  s32 timer;
};

struct RockSprite {
  u8 unk00[5];
  u8 unk05_0 : 5;
  u8 unk05_5 : 1;
  u8 unk05_6 : 2;
  u8 unk06[3];
  u8 unk09_0 : 2;
  u8 priority : 2;
  u8 palette : 4;
  u8 unk0a[20];
  u16 unk1e;
  u8 unk20[8];
};

struct RockWork {
  u8 tiles[0x500];
  struct Rock rocks[64];
  struct RockSprite sprites[64];
  s16 next;
  u16 tileBase;
  u16 resource;
};

void Import_DecompressTaggedLz(void *entry, void *destination);

void MagmaRock_StartFallingRocks(void) {
  struct RockWork *work = HeapSlot_Alloc(220, sizeof(struct RockWork));
  s32 entry;
  s32 tile;
  s32 i;
  struct Rock *rock;
  struct RockSprite *sprite;

  Import_DecompressTaggedLz(
      Resource_GetTableEntry((s32)&ResourceId_FallingRock), work);
  entry = VramCache_FindFreeEntry();
  tile = Vram_Load(entry, 0x500, work);
  work->tileBase = tile;
  work->resource = entry;
  rock = work->rocks;
  sprite = work->sprites;
  for (i = 63; i >= 0; i--, sprite++, rock++) {
    EffectSprite_Init(sprite, 8, 8, 0x40000000, tile);
    sprite->unk05_5 = 1;
    sprite->palette = 0;
    sprite->priority = 0;
    sprite->unk1e = 240;
    rock->timer = -1;
  }
  {
    s32 zero = 0;

    work->next = zero;
  }
  Task_AddFrame(Particles_UpdateFalling, 0x480);
}

void MagmaRock_EndFallingRocks(void) {
  struct MagmaRockSparkWork *work = Ram_HeapSlots->menuRuntime;

  FieldTask_Remove(Particles_UpdateFalling);
  VramCache_FreeEntry(work->resource);
  HeapSlot_Release(220);
}

void MagmaRock_UnblockMovedPillar(void);

// clang-format off
u32 gFieldEffect_ShortScript[] = {                                                               
      SC_WAIT(1), SC_LOOP(22, SC_TOP),                                     
      SC_WAIT(1), SC_POSITION(0, 0, 0),                                        
      SC_WAIT(1), SC_FREE,                                                     
      SC_END,                                                                  
  };

u32 gFieldEffect_MediumScript[] = {                                                               
      SC_WAIT(1), SC_LOOP(44, SC_TOP),                                     
      SC_WAIT(1), SC_POSITION(0, 0, 0),                                        
      SC_WAIT(1), SC_FREE,                                                     
      SC_END,                                                                  
  };

u32 gFieldEffect_LongScript[] = {                                                               
      SC_WAIT(1), SC_LOOP(126, SC_TOP),                                     
      SC_WAIT(1), SC_POSITION(0, 0, 0),                                        
      SC_WAIT(1), SC_FREE,                                                     
      SC_END,                                                                  
  };

s32 gPushLeaderSteps[] = {                                                               
      PUSH_STEP(16, 0),  PUSH_STEP(16, 0),  PUSH_STEP(16, 0),                  
      PUSH_STEP(0, 16),  PUSH_STEP(0, 16),  PUSH_STEP(0, 16),                  
      PUSH_STEP(0, 16),  PUSH_STEP(-16, 0), PUSH_STEP(-16, 0),                 
      PUSH_STEP(-16, 0), PUSH_STEP(-16, 0), PUSH_STEP(0, -16),                 
      PUSH_STEP(0, -16), PUSH_STEP(0, -16), PUSH_STEP(0, -16),                 
      PUSH_STEP(16, 0),                                                        
  };

u32 gMagmaRock_EmberScript[] = {
    39, 2, 0, 18, 3, 0,
    1, 38, 17,
};

u32 gMagmaRock_FlameEmberScript[] = {
    39, 3, 0, 23, 3, 0,
    1, 38, 17,
};

s16 gMagmaRock_Room1Pillars[] = {
    8, 10, -1,
};

s16 gMagmaRock_Room1Statues[] = {
    9, 50, 20, 12, 30, -1, -1,
};

s16 gMagmaRock_Room2LowerPillars[] = {
    8, 15, -1,
};

s16 gMagmaRock_Room2LowerStatues[] = {
    13, 30, -1, -1,
};

s16 gMagmaRock_CrumblingPillars[] = {
    9, 512, 10, 514, -1,
};

s16 gMagmaRock_Room2MiddleStatues[] = {
    14, 50, 15, 17, 51, 18, -1,
};

s16 gMagmaRock_Room2MiddlePillars[] = {
    16, -1,
};

s16 gMagmaRock_Room2UpperStatues[] = {
    9, 50, 10, 11, 51, 12, -1,
};

s16 gMagmaRock_Room2UpperPillars[] = {
    8, -1,
};

s16 gMagmaRock_Room2TopPillars[] = {
    8, 9, -1,
};

s16 gMagmaRock_Room2TopStatues[] = {
    11, 30, -1, 12, 50, 13, 15, 31,
    -1, -1,
};

s16 gMagmaRock_Room3Statues[] = {
    9, 50, -1, -1, 0,
};

u32 gMagmaRock_RubbleScript[] = {                                                               
      SC_WAIT(33), SC_SET(SW_HOOK, 0), SC_POSITION(0, 0, 0), SC_WAIT(1),       
      SC_FREE,                                                                 
  };

u32 gMagmaRock_DustScript[] = {                                                               
      SC_WAIT(44), SC_SET(SW_HOOK, 0), SC_POSITION(0, 0, 0), SC_WAIT(1),       
      SC_FREE,                                                                 
  };

const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

s16 gMagmaRock_Regions[] = {
    240, 328, 352, 256, 344, 368, -1, 3,
    448, 328, 352, 464, 344, 368, -1, 4,
    -1, 0, 0, 0, 0, 0, 0, 0,
};

s16 gMagmaRock_Room2Regions[] = {
    272, 56, 576, 288, 72, 592, -1, 1,
    352, 40, 560, 368, 56, 576, -1, 2,
    752, 512, 536, 768, 528, 552, -1, 5,
    784, 80, 592, 800, 96, 608, -1, 7,
    896, 512, 528, 912, 528, 544, -1, 8,
    272, 48, 576, 288, 64, 592, -1, 9,
    352, 32, 560, 368, 48, 576, -1, 10,
    784, 56, 592, 800, 72, 608, -1, 14,
    752, 0xfea8, 672, 768, 0xfeb8, 688, -1, 16,
    976, 0xfea8, 672, 992, 0xfeb8, 688, -1, 17,
    -1, 0, 0, 0, 0, 0, 0, 0,
};

s16 gMagmaRock_Room3Regions[] = {
    240, -56, 448, 256, -40, 464, -1, 1,
    384, -56, 448, 400, -40, 464, -1, 2,
    416, -128, 448, 432, -112, 464, -1, 4,
    -1, 0, 0, 0, 0, 0, 0, 0,
};

u32 gSceneExits[] = {
    SCENE_EXITS(MagmaRock1),
    SCENE_EXIT(1, WorldMap, 51),
    SCENE_EXIT(2, MagmaRockInside7, 11),
    SCENE_EXIT(3, MagmaRock2, 16),
    SCENE_EXIT(4, MagmaRock2, 17),
    SCENE_EXITS(MagmaRock2),
    SCENE_EXIT(1, MagmaRock2, 9),
    SCENE_EXIT(2, MagmaRock2, 10),
    SCENE_EXIT(3, MagmaRock2, 4),
    SCENE_EXIT(4, MagmaRock2, 3),
    SCENE_EXIT(5, MagmaRock3, 1),
    SCENE_EXIT(6, MagmaRock3, 2),
    SCENE_EXIT(7, MagmaRock2, 14),
    SCENE_EXIT(8, MagmaRock3, 2),
    SCENE_EXIT(9, MagmaRock2, 1),
    SCENE_EXIT(10, MagmaRock2, 2),
    SCENE_EXIT(11, MagmaRock2, 13),
    SCENE_EXIT(12, MagmaRock2, 15),
    SCENE_EXIT(13, MagmaRock2, 11),
    SCENE_EXIT(14, MagmaRock2, 7),
    SCENE_EXIT(15, MagmaRock2, 12),
    SCENE_EXIT(16, MagmaRock1, 3),
    SCENE_EXIT(17, MagmaRock1, 4),
    SCENE_EXITS(MagmaRock3),
    SCENE_EXIT(1, MagmaRock2, 5),
    SCENE_EXIT(2, MagmaRock2, 8),
    SCENE_EXIT(3, MagmaRockInside2, 8),
    SCENE_EXIT(4, MagmaRock2, 6),
    SCENE_EXITS_END,
};

struct ActorPlacement gMagmaRock_Placements[] = {
    {272, CONDITION_ALWAYS, 1, 1, 0, 0, FACING_NORTH, 2, 2},
    {377, CONDITION_ALWAYS, 1, 2, 0, 0, FACING_SOUTH, 2, 2},
    {272, CONDITION_ALWAYS, 1, 3, 0, 0, FACING_NORTH, 2, 2},
    {404, CONDITION_ALWAYS, 1, 4, 0, 0, FACING_NORTH, 2, 2},
    {377, CONDITION_ALWAYS, 1, 5, 0, 0, FACING_WEST, 2, 2},
    {377, CONDITION_ALWAYS, 7, 6, 0, 0, FACING_EAST, 2, 2},
    {377, CONDITION_ALWAYS, 7, 7, 0, 0, FACING_WEST, 2, 2},
    {404, CONDITION_ALWAYS, 1, 8, 0, 0, FACING_NORTH, 2, 2},
    {404, CONDITION_ALWAYS, 1, 9, 0, 0, FACING_NORTH, 2, 2},
    {404, CONDITION_ALWAYS, 1, 10, 0, 0, FACING_NORTH, 2, 2},
    {404, CONDITION_ALWAYS, 1, 11, 0, 0, FACING_NORTH, 2, 2},
    {329, CONDITION_ALWAYS, 1, 12, 0, 0, FACING_NORTH, 2, 2},
    {379, CONDITION_ALWAYS, 1, 2, 0, 0, FACING_NORTH, 2, 2},
    ACTOR_PLACEMENT(403, CONDITION_ALWAYS, 7, 8, 0, 8, FACING_WEST, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gMagmaRock_Room2LowerPlacements[] = {
    {272, CONDITION_ALWAYS, 1, 1, 0, 0, FACING_NORTH, 2, 2},
    {404, CONDITION_ALWAYS, 1, 2, 0, 0, FACING_NORTH, 2, 2},
    {404, CONDITION_ALWAYS, 1, 3, 0, 0, FACING_NORTH, 2, 2},
    {407, CONDITION_ALWAYS, 1, 4, 0, 0, FACING_SOUTH, 2, 2},
    {407, CONDITION_ALWAYS, 1, 5, 0, 0, FACING_SOUTH, 2, 3},
    {377, CONDITION_ALWAYS, 1, 6, 0, 0, FACING_EAST, 2, 2},
    {404, CONDITION_ALWAYS, 1, 7, 0, 0, FACING_NORTH, 2, 2},
    {272, CONDITION_ALWAYS, 1, 8, 0, 0, FACING_NORTH, 2, 2},
    {SCENE_TABLE_END},
};

struct ActorPlacement gMagmaRock_Room2MiddlePlacements[] = {
    {377, CONDITION_ALWAYS, 7, 9, 0, 0, FACING_EAST, 2, 2},
    {377, CONDITION_ALWAYS, 7, 10, 0, 0, FACING_WEST, 2, 2},
    {377, CONDITION_ALWAYS, 7, 9, 0, 0, FACING_EAST, 2, 2},
    {377, CONDITION_ALWAYS, 7, 10, 0, 0, FACING_WEST, 2, 2},
    {377, CONDITION_ALWAYS, 7, 9, 0, 0, FACING_EAST, 2, 2},
    {377, CONDITION_ALWAYS, 7, 10, 0, 0, FACING_WEST, 2, 2},
    {377, CONDITION_ALWAYS, 1, 11, 0, 0, FACING_SOUTH, 2, 2},
    {379, CONDITION_ALWAYS, 1, 11, 0, 0, FACING_NORTH, 2, 2},
    {272, CONDITION_ALWAYS, 1, 12, 0, 0, FACING_NORTH, 2, 2},
    {377, CONDITION_ALWAYS, 1, 13, 0, 0, FACING_SOUTH, 2, 2},
    {379, CONDITION_ALWAYS, 1, 13, 0, 0, FACING_NORTH, 2, 2},
    ACTOR_PLACEMENT(403, CONDITION_ALWAYS, 7, 8, 0, 8, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(403, CONDITION_ALWAYS, 7, 8, 0, 8, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(403, CONDITION_ALWAYS, 7, 8, 0, 8, FACING_WEST, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gMagmaRock_Room2UpperPlacements[] = {
    {272, CONDITION_ALWAYS, 1, 14, 0, 0, FACING_NORTH, 2, 2},
    {377, CONDITION_ALWAYS, 1, 15, 0, 0, FACING_SOUTH, 2, 2},
    {379, CONDITION_ALWAYS, 1, 15, 0, 0, FACING_NORTH, 2, 2},
    {377, CONDITION_ALWAYS, 1, 16, 0, 0, FACING_SOUTH, 2, 2},
    {379, CONDITION_ALWAYS, 1, 16, 0, 0, FACING_NORTH, 2, 2},
    {407, CONDITION_ALWAYS, 1, 17, 0, 0, FACING_SOUTH, 2, 2},
    {407, CONDITION_ALWAYS, 1, 18, 0, 0, FACING_SOUTH, 2, 3},
    {SCENE_TABLE_END},
};

struct ActorPlacement gMagmaRock_Room2TopPlacements[] = {
    {272, CONDITION_ALWAYS, 1, 19, 0, 0, FACING_NORTH, 2, 2},
    {272, CONDITION_ALWAYS, 1, 20, 0, 0, FACING_NORTH, 2, 2},
    {404, CONDITION_ALWAYS, 1, 21, 0, 0, FACING_NORTH, 2, 2},
    {377, CONDITION_ALWAYS, 7, 22, 0, 0, FACING_WEST, 2, 2},
    {377, CONDITION_ALWAYS, 1, 23, 0, 0, FACING_SOUTH, 2, 2},
    {379, CONDITION_ALWAYS, 1, 23, 0, 0, FACING_NORTH, 2, 2},
    {404, CONDITION_ALWAYS, 1, 24, 0, 0, FACING_NORTH, 2, 2},
    {377, CONDITION_ALWAYS, 7, 25, 0, 0, FACING_WEST, 2, 2},
    {299, CONDITION_ALWAYS, 7, 26, 0, 0, FACING_NORTH, 2, 2},
    {299, CONDITION_ALWAYS, 7, 27, 0, 0, FACING_NORTH, 2, 2},
    {299, CONDITION_ALWAYS, 7, 28, 0, 0, FACING_NORTH, 2, 2},
    {SCENE_TABLE_END},
};

struct ActorPlacement gMagmaRock_Room3Placements[] = {
    {384, CONDITION_ALWAYS, 7, 1, 0, 0, FACING_NORTH, 2, 2},
    {377, CONDITION_ALWAYS, 7, 2, 0, 0, FACING_WEST, 2, 2},
    {SCENE_TABLE_END},
};
// clang-format on

struct Trigger gMagmaRock_Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {0xca02, 2, 0x2f8, EVENT(MagmaRock_SlideDown)},
    {TRIGGER_STEP, 2, CONDITION_ALWAYS, EVENT(MagmaRock_ClearSlideFlag)},
    {PSYNERGY_ON_ACTOR(ABILITY_LIFT), 19, CONDITION_ALWAYS, 0},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(MagmaRock_UnblockMovedPillar)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(MagmaRock_BlockMovedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 8,
     0x8d5, EVENT(MagmaRock_UnblockMovedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 8, 0x8d5,
     EVENT(MagmaRock_BlockMovedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 10,
     0x8d6, EVENT(MagmaRock_UnblockMovedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 10, 0x8d6,
     EVENT(MagmaRock_BlockMovedPillar)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 31, 0x200,
     EVENT(MagmaRock_BurstColumn1)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 32, 0x201,
     EVENT(MagmaRock_BurstColumn2)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 33, 0x202,
     EVENT(MagmaRock_BurstColumn3)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 34, 0x203,
     EVENT(MagmaRock_BurstColumn4)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 35, 0x204,
     EVENT(MagmaRock_BurstColumn5)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_FIRST), 30,
     CONDITION_ALWAYS, EVENT(MagmaRock_ResolveBeforeBurst)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 30, 0x2f0,
     EVENT(MagmaRock_TargetStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 30,
     CONDITION_ALWAYS, EVENT(MagmaRock_BurstStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_AFTER_CAST), 30,
     CONDITION_ALWAYS, EVENT(MagmaRock_FireStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_FIRST), 50,
     CONDITION_ALWAYS, EVENT(MagmaRock_ResolveBeforeBurst)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 50,
     CONDITION_ALWAYS, EVENT(MagmaRock_TargetStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_AFTER_CAST), 50,
     CONDITION_ALWAYS, EVENT(MagmaRock_IgniteStatue)},
    {TRIGGER_TABLE_END},
};

struct Trigger gMagmaRock_Room2LowerTriggers[] = {
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {STEP_TRIGGER_WALKING, 20, CONDITION_ALWAYS,
     EVENT(MagmaRock_CheckCrumblingPillars)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(MagmaRock_UnblockMovedPillar)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(MagmaRock_BlockMovedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 8,
     CONDITION_ALWAYS, EVENT(MagmaRock_UnblockMovedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 8, CONDITION_ALWAYS,
     EVENT(MagmaRock_BlockMovedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 15,
     0x8d7, EVENT(MagmaRock_UnblockMovedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 15, 0x8d7,
     EVENT(MagmaRock_BlockMovedPillar)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 31, 0x200,
     EVENT(MagmaRock_BurstColumn1)},
    {TRIGGER_STEP, 50, 0x212, EVENT(MagmaRock_SetLedgeBehavior)},
    {TRIGGER_STEP, 51, CONDITION_ALWAYS, EVENT(MagmaRock_ClearLedgeBehavior)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_FIRST), 30,
     CONDITION_ALWAYS, EVENT(MagmaRock_ResolveBeforeBurst)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 30, 0x2f0,
     EVENT(MagmaRock_TargetStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 30,
     CONDITION_ALWAYS, EVENT(MagmaRock_BurstStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_AFTER_CAST), 30,
     CONDITION_ALWAYS, EVENT(MagmaRock_FireStatue)},
    {TRIGGER_TABLE_END},
};

struct Trigger gMagmaRock_Room2MiddleTriggers[] = {
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(MagmaRock_UnblockMovedPillar)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(MagmaRock_BlockMovedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 16,
     0x8d8, EVENT(MagmaRock_UnblockMovedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 16, 0x8d8,
     EVENT(MagmaRock_BlockMovedPillar)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_FIRST), 50,
     CONDITION_ALWAYS, EVENT(MagmaRock_ResolveBeforeBurst)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 50,
     CONDITION_ALWAYS, EVENT(MagmaRock_TargetStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_AFTER_CAST), 50,
     CONDITION_ALWAYS, EVENT(MagmaRock_IgniteStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_FIRST), 51,
     CONDITION_ALWAYS, EVENT(MagmaRock_ResolveBeforeBurst)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 51,
     CONDITION_ALWAYS, EVENT(MagmaRock_TargetStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_AFTER_CAST), 51,
     CONDITION_ALWAYS, EVENT(MagmaRock_IgniteStatue)},
    {TRIGGER_RAISED, 200, CONDITION_ALWAYS, EVENT(MagmaRock_AmplifyBurst)},
    {TRIGGER_TABLE_END},
};

struct Trigger gMagmaRock_Room2UpperTriggers[] = {
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), 40, 0x8,
     EVENT(MagmaRock_PushPillarEast)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(MagmaRock_UnblockMovedPillar)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(MagmaRock_BlockMovedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 8,
     0x8d9, EVENT(MagmaRock_UnblockMovedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 8, 0x8d9,
     EVENT(MagmaRock_BlockMovedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), 14, 0x250, 0},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_FIRST), 50,
     CONDITION_ALWAYS, EVENT(MagmaRock_ResolveBeforeBurst)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 50,
     CONDITION_ALWAYS, EVENT(MagmaRock_TargetStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_AFTER_CAST), 50,
     CONDITION_ALWAYS, EVENT(MagmaRock_IgniteStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_FIRST), 51,
     CONDITION_ALWAYS, EVENT(MagmaRock_ResolveBeforeBurst)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 51,
     CONDITION_ALWAYS, EVENT(MagmaRock_TargetStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_AFTER_CAST), 51,
     CONDITION_ALWAYS, EVENT(MagmaRock_IgniteStatue)},
    {TRIGGER_TABLE_END},
};

struct Trigger gMagmaRock_Room2TopTriggers[] = {
    {TRIGGER_EXIT, 13, CONDITION_ALWAYS, 13},
    {TRIGGER_EXIT, 15, CONDITION_ALWAYS, 15},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), 40, CONDITION_ALWAYS,
     EVENT(MagmaRock_PushPillarWest)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(MagmaRock_UnblockMovedPillar)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(MagmaRock_BlockMovedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 8,
     0x8da, EVENT(MagmaRock_UnblockMovedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 8, 0x8da,
     EVENT(MagmaRock_BlockMovedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 9,
     0x8db, EVENT(MagmaRock_UnblockMovedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 9, 0x8db,
     EVENT(MagmaRock_BlockMovedPillar)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_FIRST), 30,
     CONDITION_ALWAYS, EVENT(MagmaRock_ResolveBeforeBurst)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 30, 0x2f0,
     EVENT(MagmaRock_TargetStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 30,
     CONDITION_ALWAYS, EVENT(MagmaRock_BurstStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_AFTER_CAST), 30,
     CONDITION_ALWAYS, EVENT(MagmaRock_FireStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_FIRST), 31,
     CONDITION_ALWAYS, EVENT(MagmaRock_ResolveBeforeBurst)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 31, 0x2f0,
     EVENT(MagmaRock_TargetStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 31,
     CONDITION_ALWAYS, EVENT(MagmaRock_BurstStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_AFTER_CAST), 31,
     CONDITION_ALWAYS, EVENT(MagmaRock_FireStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 32, 0x201,
     EVENT(MagmaRock_BurstColumn2)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_FIRST), 50,
     CONDITION_ALWAYS, EVENT(MagmaRock_ResolveBeforeBurst)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 50,
     CONDITION_ALWAYS, EVENT(MagmaRock_TargetStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_AFTER_CAST), 50,
     CONDITION_ALWAYS, EVENT(MagmaRock_IgniteStatue)},
    {PSYNERGY_ON_ACTOR(ABILITY_GROWTH), 16, 0x250, EVENT(MagmaRock_GrowVine1)},
    {PSYNERGY_ON_ACTOR(ABILITY_GROWTH), 17, 0x251, EVENT(MagmaRock_GrowVine2)},
    {PSYNERGY_ON_ACTOR(ABILITY_GROWTH), 18, 0x252, EVENT(MagmaRock_GrowVine3)},
    {TRIGGER_TABLE_END},
};

struct Trigger gMagmaRock_Room3Triggers[] = {
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_FIRST), 50,
     CONDITION_ALWAYS, EVENT(MagmaRock_ResolveBeforeBurst)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 50, 0x2f0,
     EVENT(MagmaRock_TargetStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 50,
     CONDITION_ALWAYS, EVENT(MagmaRock_BlastOpenWall)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_AFTER_CAST), 50,
     CONDITION_ALWAYS, EVENT(MagmaRock_FireStatue)},
    {TRIGGER_TABLE_END},
};
#undef BYTE_AT
#undef Func_02003af4
#undef FieldEffect_Spawn
#undef MagmaRock_RaiseFlameTop
#undef MagmaRock_RaiseFlamePart
#undef MagmaRockInside_RaiseActorCells
#undef Blocks_MarkCells
#undef MagmaRockEventWork
