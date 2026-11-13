#include "shared/routines.h"
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

void Func_02000594(void) { Func_080d2b4c(8, 56); }

DEFINE_FUNC_020003D0(Func_020005a0)

extern u8 gMapShapeGrid[];

s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
s32 MapGrid_GetTerrainKind(s32 layer, s32 x, s32 z);
s32 Func_02003c24(s32 layer, s32 shape, s32 height);

DEFINE_FUNC_020005CC(Func_020005cc, MapGrid_GetTerrainKind, Func_02003c24)

#include "../field/map/cell_get.c"

#include "field.h"

DEFINE_FUNC_02000668(Func_02000668, Func_020005cc)

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
DEFINE_FUNC_020003E8(Func_020006cc, MapCell_GetAttributes)

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

DEFINE_FUNC_0200074C(Func_0200074c)

// For each id of a list ended by 0xffff: settles the object on its ground,
// copies the cell under it, and blocks the cells ahead of and behind it on a
// cell with attribute bit 6.
#include "text/msg_ids.h"

/// For each id of a list ended by 0xffff: settles the object on its ground,
/// copies the cell under it, and blocks the cells ahead of and behind it on a
/// cell with attribute bit 6.
void Func_0200079c(u16 *ids) {
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
    Actor_SetSingleChildFlags(object, 0);
    Func_020005cc(*terrainId, object->x >> 20, object->z >> 20,
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

extern const u8 gData_02003ea8[];
extern const u8 gData_02003ed8[];
extern const u8 gData_02003f88[];

const struct SceneEntrance *Scene_GetRegions(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov691Room2)
    return (const struct SceneEntrance *)gData_02003ed8;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov691Room3)
    return (const struct SceneEntrance *)gData_02003f88;
  return (const struct SceneEntrance *)gData_02003ea8;
}

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern const u8 gMagmaRock_Placements[];
extern const u8 gData_020041a4[];
extern const u8 gData_0200427c[];
extern const u8 gData_020043e4[];
extern const u8 gData_020044a4[];
extern const u8 gMagmaRock_Room3Placements[];

const struct Trigger *Scene_GetPlacements(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov691Room2) {
    if (gMainRoutineWork.entrance <= 3)
      return (const struct Trigger *)gData_020041a4;
    if (gMainRoutineWork.entrance <= 8)
      return (const struct Trigger *)gData_0200427c;
    if (gMainRoutineWork.entrance <= 12)
      return (const struct Trigger *)gData_020043e4;
    return (const struct Trigger *)gData_020044a4;
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov691Room3)
    return (const struct Trigger *)gMagmaRock_Room3Placements;
  return (const struct Trigger *)gMagmaRock_Placements;
}

void Func_02003c14(s16 *table);

void Func_02000940(void) {
  u8 *event = Ram_HeapSlots->eventWork;
  struct MagmaRockWork *work = Ram_HeapSlots->effectWork;

  if (gMainRoutineWork.scene == (s32)&SceneId_Ov691Room1) {
    MapGrid_CopyCellAttributes(98, 18, 4, 3, 78, 21);
    MapGrid_CopyCellAttributes(98, 24, 4, 3, 78, 24);
    Func_02003c14(gData_02003d98);
  } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov691Room2) {
    if (gMainRoutineWork.entrance <= 3) {
      Func_02003c14(gData_02003dac);
    } else if (gMainRoutineWork.entrance <= 7) {
      Func_02003c14(gData_02003dd2);
    } else if (gMainRoutineWork.entrance <= 11) {
      MapGrid_CopyCellAttributes(79, 46, 2, 1, 79, 36);
      Func_02003c14(gData_02003de4);
    } else {
      if (((s16 *)&work->target->z)[1] == 600)
        MapGrid_CopyCellAttributes(104, 38, 1, 1, 103, 38);
      else
        MapGrid_CopyCellAttributes(94, 44, 5, 1, 99, 38);
      Func_02003c14(gData_02003de8);
    }
  }
  Func_0200074c(*(struct Actor **)(event + 0xcbc));
}

void Event_Start();
void Func_02003c1c();
s32 Func_02003af4();

/// Rebuilds the room's opened passages on entry and sets the flag of the cell
/// the leader stands on.
s32 Func_02000a48(void) {
  struct Actor *leader =
      *(struct Actor **)((u8 *)Ram_HeapSlots->eventWork + 3260);
  s32 x, z;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Func_02003c1c();
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov691Room1) {
    MapGrid_CopyCellAttributes(98, 21, 4, 3, 78, 21);
    MapGrid_CopyCellAttributes(98, 27, 4, 3, 78, 24);
    Func_020006cc(gData_02003d98);
  } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov691Room2) {
    if (gMainRoutineWork.entrance <= 3) {
      Func_020006cc(gData_02003dac);
    } else if (gMainRoutineWork.entrance <= 8) {
      Func_020006cc(gData_02003dd2);
    } else if (gMainRoutineWork.entrance <= 12) {
      MapGrid_CopyCellAttributes(79, 37, 2, 1, 79, 36);
      Func_020006cc(gData_02003de4);
    } else {
      MapGrid_CopyCellAttributes(103, 39, 1, 1, 103, 38);
      MapGrid_CopyCellAttributes(99, 42, 5, 1, 99, 38);
      Func_020006cc(gData_02003de8);
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

void Func_02003bb4(s32 owner, u16 value);

void Func_02000c08(s16 value) {
  struct MainRoutineWork *party;

  BattleEffect_LoadActionEffectResources(151, 1);
  party = &gMainRoutineWork;
  Func_02003bb4(party->currentOwner, value);
  Field_DispatchTypeHandler(1);
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

extern const u8 gData_02003d74[];

void Func_0200015c(s32 x, s32 y, s32 z, s32 a, s32 b, s32 c, s32 flags,
                   s32 *params);

void Func_02000c40(struct Actor *object) {
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
      params.script = gData_02003d74;
      {
        s32 mask = 15;

        dx = mask & Random16Far();
      }
      dy = Random16Far() & 31;
      Func_0200015c(object->x + ((dx - 8) << 16),
                    object->y + (dy << 16) - 0x80000, object->z, 0, phase,
                    phase, 0x320001, &params);
    }
    if (gFrameCount & 4)
      Actor_SetSpriteColor(object, 10);
    else
      Actor_SetSpriteColor(object, 0);
  } else if (*timer < 100) {
    if (gFrameCount & 4)
      Actor_SetSpriteColor(object, 10);
    else
      Actor_SetSpriteColor(object, 0);
  } else if (*timer < 140) {
    if (gFrameCount & 2)
      Actor_SetSpriteColor(object, 10);
    else
      Actor_SetSpriteColor(object, 0);
  } else if (*timer < 144) {
    Actor_SetSpriteColor(object, 7);
  } else {
    Actor_SetSpriteColor(object, 0);
    object->update = NULL;
    linked->x = object->x;
    linked->y = object->y;
    linked->z = object->z;
    linked->update = Func_02000dc0;
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
void Func_02000d40(struct Actor *object) {
  u32 frame = gFrameCount;

  if ((frame & 3) == 0) {
    struct MagmaRockEffectConfig config;
    s32 dz;
    s32 velocityZ;
    s32 z;

    config.id = 403;
    config.kind = 2;
    config.data = gData_02003d50;
    if ((frame & 15) == 0 && !Event_GetFlag(608))
      Func_02003c4c(object, 136);
    dz = ((Random16Far() & 15) - 8) << 15;
    velocityZ = ((Random16Far() & 31) - 16) << 12;
    z = object->z;
    Func_0200015c(object->x, object->y, z + dz, 0, 0, velocityZ, 0x320001,
                  &config);
  }
}

struct MagmaRockEventWork {
  u8 unk000[362];
  u16 unk16a;
};

s32 Math_Cosine(s32 angle);

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))
#define HALF_AT(base, offset) (*(s16 *)((u8 *)(base) + (offset)))

void Func_02000dc0(struct Actor *object) {
  s32 step;
  struct Actor *partner;
  struct Actor *leader;

  step = Math_Cosine(object->facing);
  partner = Actor_Get(HALF_AT(object, 102) + BYTE_AT(object, 98));
  leader = Actor_Get(gMainRoutineWork.currentOwner);
  object->x += step * 2;
  Func_02000d40(object);
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
  if (Func_020005a0(&object->x, &partner->x) <= 8 &&
      object->y >> 20 == partner->y >> 20) {
    object->x = 0;
    object->update = NULL;
    object->facing ^= 0x9000;
    object->y = 0;
    object->z = 0;
    BYTE_AT(object, 98) ^= 1;
    partner->timer = 0;
    partner->update = (void *)Func_02000c40;
    partner->watchedObject = object;
  }
}

// Unless flag 0x109 is set, places the object at the watched object first +
// side, gives it Func_02000dc0 and faces it along the pair first and first + 1.

void Func_02000ee4(u16 id, s16 first, s16 side) {
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
  object->update = (void *)Func_02000dc0;
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

DEFINE_FUNC_02000DA4(Func_02000fcc, 10)
