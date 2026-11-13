/// @file
/// Scene code of overlay 0x692, in ROM order.

// ---- games/tla/src/scene/magma_rock_inside/s03b4.c ----
#include "../shared/routines.h"
#include "types.h"
#include "main_routine_work.h"
#include "event_inline.h"
#include "event_work.h"
#include "field.h"
#include "field_coordinate.h"
#include "module/field/field.h"
#include "module/field/object.h"
#include "module/scene/magma_rock_inside.h"
#include "module/scene/mars_lighthouse_aerie.h"
#include "module/scene/scene.h"
#include "module/system/system.h"
#include "module/tla.h"
#include "scene.h"
#include "text/msg_ids.h"

extern s32 gData_02004fec[];

DEFINE_FUNC_0200038C(Func_020003b4, gData_02004fec, MapGrid_CheckCollision,
    Func_02004c98)

#include "ram_buffer.h"
#include "map.h"

extern struct MapCell gMapCellBuffer[];

/// Sets the collision code of each cell in a layer rectangle to value.
/// Layers 3 and above use the shared buffer.
s32 Func_02000538(u32 layer, u32 x, u32 y, u32 width, u32 height, s32 value) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct MapCell *cells;

  if (map == NULL)
    return 0;
  if (layer <= 2)
    cells = map->layers[layer].cells;
  else
    cells = gMapCellBuffer;
  cells += x + (y << 7);
  for (x = 0; x < height; x++) {
    struct MapCell *cell = cells + (x << 7);

    for (y = 0; y < width; y++, cell++)
      cell->collisionCode = value;
  }
  return 0;
}

// ---- games/tla/src/scene/magma_rock_inside/s0594.c ----
#include "../magma_rock_inside.h"

void Func_02000594(void) { Func_080d2b4c(15, 56); }

void Func_020005a0(void) { Func_080d7788(17, 2, 19); }

DEFINE_FUNC_020003D0(Func_020005b0)

#include "actor.h"

s32 MapGrid_GetTerrainKind(s32 layer, s32 x, s32 z);
s32 Func_02004ef0(s32 layer, s32 shape, s32 height);

DEFINE_FUNC_020005CC(Func_020005dc, MapGrid_GetTerrainKind, Func_02004ef0)

void Func_0200063c(struct Actor *object) {
  s32 count;
  s32 busy;

  count = 60;
  while (count != 0) {
    System_Wait(1);
    busy = object->velocityY;
    count--;
    if (busy == 0) {
      break;
    }
  }
}

DEFINE_FUNC_0200012C(Func_02000658)

DEFINE_FUNC_02000168(Func_02000694)

/// For each id of a list ended by 0xffff: clears the object's mark and, on a
/// cell with attribute bit 6, blocks the cells ahead of and behind it and sets
/// the mark.
DEFINE_FUNC_020003E8(Func_020006dc, Func_02000658)

/// Clears the object's mark and the cells ahead of and behind it that the mark
/// blocked.
DEFINE_FUNC_0200074C(Func_0200075c)

void Func_020007ac(struct Actor *object) {
  u8 *work = (u8 *)Ram_HeapSlots->eventWork;
  u8 *state = &object->movementState;

  if (*(s16 *)(work + 358) != 0 || *(s16 *)(work + 346) != 0 ||
      *(s16 *)(work + 350) != 0 || *(s16 *)(work + 360) != 0 ||
      *(u16 *)(work + 434) == 0)
    *state = 1;
  else
    *state = 0;
}

// ---- games/tla/src/scene/magma_rock_inside/s0800.c ----
#include "field.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void Func_02004c98();
void Actor_StartScript();
s32 GameFlag_Test();
/* VIEW(s0800): routine Actor_GetById as s32 Actor_GetById(); event.h has struct Actor *Actor_GetById(u32 objectId); tried as the definition's struct Actor *Actor_GetById(u32 objectId), which the object already declares, it does not compile in tla-ja: games/tla/src/scene/magma_rock_inside.c:294: too few arguments to function `Actor_GetById'. */
extern s32 Actor_GetById__s0800() __asm__("Actor_GetById");

void Func_02000800(s32 a0, s32 a1, s32 a2) {
  s32 t1, t4, t5, v5, v6, v7;

  v5 = (a2 << 16);
  v7 = a1;
  t1 = Actor_GetById__s0800();
  v6 = t1;
  Func_02004c98(t1, 4);
  FIELD_AT(v6, u8, 90) = 0;
  FIELD_AT(v6, s32, 48) = 0x8000;
  FIELD_AT(v6, s32, 52) = 0x8000;
  FIELD_AT(v6, u8, 85) = 2;
  /* FAKEMATCH(barrier): an empty asm ends the scheduling block here, as
   * the listing orders it; without it, 4 of 53 instructions differ in
   * tla-en. */
  asm("");
  v5 = (v5 >> 16);
  v5 = (v5 << 16);
  FIELD_AT(v6, u8, 35) = (128 | FIELD_AT(v6, u8, 35));
  v5 = ((u32)v5 >> 16);
  Actor_SetSingleChildFlags(v6, 0);
  t4 = Event_GetFlag(v5);
  /* FAKEMATCH(barrier): an empty asm ends the scheduling block here, as
   * the listing orders it; without it, 2 of 53 instructions differ in
   * tla-en. */
  asm("");
  if (t4 == 0)
    goto L_0200886a;
  FIELD_AT(v6, s32, 20) = 0x100000;
  FIELD_AT(v6, s32, 12) = 0x100000;
  t5 = Event_GetFlag(0x109);
  if (t5 != 0)
    goto L_02008870;
  Actor_StartScript(v6, v7);
  goto L_02008870;
L_0200886a:
  FIELD_AT(v6, s32, 20) = -0xe0000;
  FIELD_AT(v6, s32, 12) = -0xe0000;
L_02008870:
  FIELD_AT(v6, s32, 108) = (s32)Func_020007ac;
}
#undef FIELD_AT

// ---- games/tla/src/scene/magma_rock_inside.c ----
// The scene code of overlay 0x692 that its scene tables belong to, in ROM order; the rest of the overlay is in magma_rock_inside/.

s32 Func_020005dc(s32 terrain, s32 x, s32 z, s32 height);

DEFINE_FUNC_02000668(Func_02000880, Func_020005dc)

#include "battle.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "scene_tables.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern u8 gMagmaRockInside_Placements[];
extern u8 gMagmaRockInside_Room2Placements[];
extern u8 gMagmaRockInside_Room3Placements[];
extern u8 gMagmaRockInside_Room4Placements[];
extern u8 gMagmaRockInside_Room5Placements[];
extern u8 gMagmaRockInside_Room6Placements[];
extern u8 gMagmaRockInside_Room7Placements[];
extern u8 gMagmaRockInside_Room8Placements[];

extern s16 gData_02005528[];

u8 *Scene_GetPlacements(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene != (s32)&SceneId_Ov692Room1) {
    if (scene == (s32)&SceneId_Ov692Room2)
      return gMagmaRockInside_Room2Placements;
    if (scene == (s32)&SceneId_Ov692Room3)
      return gMagmaRockInside_Room3Placements;
    if (scene == (s32)&SceneId_Ov692Room4)
      return gMagmaRockInside_Room4Placements;
    if (scene == (s32)&SceneId_Ov692Room5)
      return gMagmaRockInside_Room5Placements;
    if (scene == (s32)&SceneId_Ov692Room6)
      return gMagmaRockInside_Room6Placements;
    if (scene == (s32)&SceneId_Ov692Room7)
      return gMagmaRockInside_Room7Placements;
    if (scene == (s32)&SceneId_Ov692Room8)
      return gMagmaRockInside_Room8Placements;
  }
  return gMagmaRockInside_Placements;
}

void Func_0200099c(void) {
  u8 *work;
  s16 scene;

  work = (u8 *)Ram_HeapSlots->eventWork;
  scene = gMainRoutineWork.scene;
  if (scene == (s32)&SceneId_Ov692Room2)
    Func_02004ee0(gData_020054fc);
  else if (scene == (s32)&SceneId_Ov692Room3)
    Func_02004ee0(gData_02005508);
  else if (scene == (s32)&SceneId_Ov692Room4)
    Func_02004ee0(gData_02005520);
  else if (scene == (s32)&SceneId_Ov692Room6)
    Func_02004ee0(gData_02005528);
  else if (scene == (s32)&SceneId_Ov692Room7)
    Func_02004ee0(gData_0200552e);
  Func_0200075c(*(s32 *)(work + 0xcbc));
}

#include "io_reg.h"

void Func_02000a30(void) {
  struct Actor *object =
      *(struct Actor **)((u8 *)Ram_HeapSlots->eventWork + 0xcbc);
  /* FAKEMATCH(pin): copy in r5, as the native keeps it; as a plain local,
   * 78 of 281 instructions differ in tla-en. */
  register struct Actor *copy asm("r5");
  s32 scene;
  s32 x;
  s32 z;

  Event_Start();
  Event_ResolvePendingActions(0);
  Func_02004ee8();
  scene = gMainRoutineWork.scene;
  if (scene == (s32)&SceneId_Ov692Room6) {
    Func_020006dc(gData_02005528);
    if (Map_GetTerrainHeight(2, object->x, object->z) != object->y) {
      object->terrainId = 2;
      object->motionFlags = 3;
      System_Wait(1);
      if (Event_GetFlag(770)) {
        object->gravity = 0x1000;
        object->priorityFlags |= 2;
        REG_BLDALPHA = 0x1000;
        REG_BLDCNT = 0x3f10;
        copy = OverlayObject_PrepareObject(object->x, object->terrainHeight,
                                           object->z + 0x100000, 312);
        Func_0200063c(object);
        Event_PlaySound(213);
        Actor_Release(copy);
        Func_02004c98(object, 4);
        System_Wait(1);
        REG_BLDCNT = 0;
      } else {
        Func_0200063c(object);
        Event_PlaySound(188);
      }
      object->motionFlags = 0;
      object->terrainId = 0;
    }
  } else if (scene == (s32)&SceneId_Ov692Room7) {
    Func_020006dc(gData_0200552e);
    if (Map_GetTerrainHeight(2, object->x, object->z) != object->y) {
      object->terrainId = 2;
      object->motionFlags = 3;
      System_Wait(1);
      object->gravity = 0x1000;
      object->priorityFlags |= 2;
      REG_BLDALPHA = 0x1000;
      REG_BLDCNT = 0x3f10;
      copy = OverlayObject_PrepareObject(object->x, object->terrainHeight,
                                         object->z + 0x100000, 312);
      Func_0200063c(object);
      Event_PlaySound(213);
      Actor_Release(copy);
      Func_02004c98(object, 4);
      System_Wait(1);
      REG_BLDCNT = 0;
      Func_0200063c(object);
      object->motionFlags = 0;
      object->terrainId = 0;
    }
  }
  x = object->x >> 20;
  z = object->z >> 20;
  scene = gMainRoutineWork.scene;
  if (scene == (s32)&SceneId_Ov692Room2) {
    if (z == 16)
      Event_SetFlag(2544);
  } else if (scene == (s32)&SceneId_Ov692Room3) {
    if (z == 41)
      Event_SetFlag(2545);
  } else if (scene == (s32)&SceneId_Ov692Room4) {
    if (x == 21) {
      if (z == 43)
        Event_SetFlag(2546);
      if (z == 45)
        Event_SetFlag(2547);
      if (z == 47)
        Event_SetFlag(2548);
    }
  } else if (scene == (s32)&SceneId_Ov692Room6) {
    if (x == 47) {
      Event_SetFlag(2549);
      {
        s32 column = 46;
        s32 row = 73;

        MapGrid_CopyCellAttributes(column, 74, 1, 1, column, row);
      }
    }
    if (x == 57)
      Event_SetFlag(2550);
  } else if (scene == (s32)&SceneId_Ov692Room7) {
    if (x == 43)
      Event_SetFlag(2551);
  }
  Event_End();
}

#include "frame.h"
#include "field_effect.h"

s32 Func_02004e80(void);

void Func_02000ce4(s16 value) {
  struct MainRoutineWork *party;

  BattleEffect_LoadActionEffectResources(151, 1);
  party = &gMainRoutineWork;
  Func_02004e78(party->currentOwner, value);
  Field_DispatchTypeHandler(1);
  Func_02004e80();
  BattleEffect_CleanupSceneObjects();
}

extern const s32 gData_0200502c[];

void Func_0200015c(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   const struct EffectOptions *options);

/// An object's update: every fourth frame it spawns an effect of type 403 with
/// its own script at a random point within 4 pixels of the object along z,
/// moving along z at a random speed; every sixteenth frame it also plays sound
/// 136 at the object.
void Func_02000d1c(struct Actor *object) {
  struct EffectOptions options;
  u32 frame = gFrameCount;
  s32 dz;
  s32 vz;
  s32 z;

  if ((frame & 3) != 0)
    return;
  options.type = 403;
  options.priority = 2;
  options.script = gData_0200502c;
  if ((frame & 15) == 0)
    Func_02004f28(object, 136);
  dz = Random16Far() & 15;
  vz = ((Random16Far() & 31) - 16) << 12;
  /* FAKEMATCH(temporary): z read into its own local; as object->z in the
   * call, 4 of 60 instructions differ in tla-en. */
  z = object->z;
  Func_0200015c(object->x, object->y, z + ((dz - 8) << 15), 0, 0, vz, 0x320001,
                &options);
}

DEFINE_FUNC_02000DA4(Func_02000da4, 10)

// The cell watch's table lookup over (id, cell, value) rows ended by 0xffff:
// for the row of the cell under the watch, sets the watch's object to the row's
// object and returns its value; otherwise returns the value of the last row
// passed (0 for none). The loop is spelled with gotos in the native's rotated
// layout, which rebuilds 0xffff on every pass, where loop.c would hoist it out
// of a C loop.

s32 Func_02000dcc(u16 *table) {
  struct CellWatch *watch = Ram_HeapSlots->effectWork;
  s32 result = 0;
  s32 cell = Func_02000658(0, watch->x >> 20, watch->z >> 20) >> 8;
  u32 id;

  goto test;
again:
  result = *table++;
test:
  if (*table != 0xffff) {
    id = *table++;
    if (cell != *table++)
      goto again;
    watch->object = Actor_Get(id);
    result = *table;
  }
  return result;
}

void Func_02000e14(void) { Event_ResolvePendingActions(0); }

// ---- games/tla/src/scene/magma_rock_inside/s0e20.c ----

extern u16 gData_02005500[];
extern u16 gData_0200550c[];

void Func_02000da4(struct Actor *object);

/// The cell watch's callback: in modes 2 and 3 it finds the object on the
/// leader's cell in the room's table (room 4 has its own) and sets it blinking
/// (Func_02000da4) by frame-count bit 2 in mode 2 or bit 1 in mode 3.
void Func_02000e20(s32 mode) {
  u16 *table;
  struct CellWatch *watch = Ram_HeapSlots->effectWork;
  s32 scene;

  table = gData_02005500;
  FieldEvent_ResolvePendingActions(0);
  scene = gMainRoutineWork.scene;
  if (scene == (s32)&SceneId_Ov692Room2) {
  } else if (scene == (s32)&SceneId_Ov692Room3) {
  } else if (scene == (s32)&SceneId_Ov692Room4) {
    table = gData_0200550c;
  }
  if (mode == 2) {
    struct Actor *object;

    Func_02000dcc(table);
    object = watch->object;
    object->unk5c[7] = 4;
    object->update = (void (*)(void))Func_02000da4;
  }
  if (mode == 3) {
    struct Actor *object;

    Func_02000dcc(table);
    object = watch->object;
    object->unk5c[7] = 2;
    object->update = (void (*)(void))Func_02000da4;
  }
}

void Func_02000ea4(struct Actor *object) {
  s32 dx, dy, dz;

  object->x += dx = object->bounce;
  object->y += dy = object->gravity;
  object->z += dz = object->unk4c;
  dx -= dx >> 3;
  dy -= dy >> 3;
  dz -= dz >> 3;
  object->bounce = dx;
  object->gravity = dy;
  object->unk4c = dz;
}

extern const s32 gData_0200502c[];

void Func_0200015c(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   const struct EffectOptions *options);
/* VIEW(s0e20): routine Func_02000ce4 as void Func_02000ce4(u16 id); the definition has void Func_02000ce4(s16 value); tried as the definition's void Func_02000ce4(s16 value), which the object already declares, Func_02000ed0 moves: 1 of 270 instructions in tla-ja. */
extern void Func_02000ce4__s0e20(u16 id) __asm__("Func_02000ce4");

static __inline__ struct Actor *Import_ActorCreate_(s32 kind, s32 x, s32 y,
                                                    s32 z) {
  return (struct Actor *)Import_ActorCreate(kind, x, y, z);
}

void Func_02000ed0(struct Actor *source) {
  struct Actor *flame =
      Import_ActorCreate_(403, source->x, source->y, source->z);
  struct Actor *other;
  s32 position[3];
  struct EffectOptions options;
  s32 step = Math_Cosine(source->facing) << 1;
  s32 cell;
  s32 id;
  s32 bit;

  flame->motionFlags = 0;
  Func_02004c98(flame, 1);
  Actor_SetSingleChildFlags(flame, 0);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  position[0] = flame->x;
  position[1] = flame->y;
  position[2] = flame->z;
next:
  position[0] += step;
  cell =
      Func_02000658(2, position[0] >> 20, (position[2] - position[1]) >> 20) >>
      8;
  if (cell == 212 || cell == 222)
    goto end;
  id = 8;
inner:
  other = ((struct ActorTable *)Ram_HeapSlots->eventWork)->actors[id];
  if (other->script == 0)
    goto skip;
  if (source->x >> 20 == other->x >> 20 && source->y >> 20 == other->y >> 20 &&
      source->z >> 20 == other->z >> 20)
    goto skip;
  if (Func_020005b0(&other->x, position) > 8)
    goto skip;
  if (other->y >> 20 != position[1] >> 20)
    goto skip;
  if (*(s16 *)(*(u8 **)((u8 *)other->sprite + 40)) == 272)
    goto end;
  goto hit;
skip:
  id++;
  if (id == 64)
    goto next;
  goto inner;
end:
  flame->acceleration = 0x20000;
  flame->speedLimit = 0x20000;
  flame->update = (void (*)(void))Func_02000d1c;
  FieldActor_SetMoveTarget(flame, position[0], position[1], position[2]);
  FieldCamera_SetSpeed(0x20000, 0x4000);
  FieldCamera_MoveTo(position[0], position[1], position[2], 1);
  Actor_WaitForMove(flame);
  flame->update = 0;
  Actor_Release(flame);
  id = 0;
spark:
  bit = gFrameCount;
  bit &= 1;
  if (bit == 0) {
    options.type = 403;
    options.priority = 1;
    options.update = (void (*)(union FieldObject *))Func_02000ea4;
    options.script = gData_0200502c;
    Func_0200015c(position[0], position[1], position[2],
                  ((Random16Far() & 31) - 16) << 13,
                  ((Random16Far() & 31) - 16) << 13, bit, 0x1320000, &options);
  }
  System_Wait(1);
  id++;
  if (id != 16)
    goto spark;
  System_Wait(24);
  if (cell == 222) {
    Func_02001350();
    Event_End();
    return;
  }
  FieldCamera_FollowActor(gMainRoutineWork.currentOwner, 1);
  FieldCamera_WaitForArrival();
  System_Wait(10);
  Event_End();
  return;
hit:
  flame->acceleration = 0x20000;
  flame->speedLimit = 0x20000;
  flame->update = (void (*)(void))Func_02000d1c;
  FieldActor_SetMoveTarget(flame, position[0], position[1], position[2]);
  FieldCamera_SetSpeed(0x20000, 0x4000);
  FieldCamera_MoveTo(position[0], position[1], position[2], 1);
  Actor_WaitForMove(flame);
  flame->update = 0;
  Actor_Release(flame);
  Event_SetFlag(0x2f0);
  Func_02000ce4__s0e20((u16)id);
  Event_ClearFlag(0x2f0);
  FieldCamera_FollowActor(gMainRoutineWork.currentOwner, 1);
  FieldCamera_WaitForArrival();
  System_Wait(10);
  Event_End();
}

#include "macros.h"

void Func_02004bd4(void);

void Func_02001148(void) {
  struct MagmaRockInsideWork *work = Ram_HeapSlots->effectWork;
  struct Actor *object;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_Wait(10);
  object = work->target;
  object->update = NULL;
  Actor_SetSpriteColor(object, 7);
  FieldEvent_Wait(3);
  Actor_SetSpriteColor(work->target, 0);
  FieldEvent_Wait(1);
  Func_02000ed0(work->target);
  Event_End();
}

void Func_02001190(s32 mode, s32 cue) {
  struct MagmaRockInsideWork *work = Ram_HeapSlots->effectWork;

  if (cue == 0)
    Func_02004b18();
  if (mode == 2) {
    struct Actor *object = work->target;

    Func_02004a5c(object->x, object->y + 0xf0000, object->z, 1);
  }
  if (mode == 3) {
    struct Actor *object = work->target;
    struct Actor *target;

    Func_02004a5c(object->x, object->y + 0xf0000, object->z, 8);
    target = work->target;
    SET_POSITION(target, 0, 0, 0);
  }
  if (cue == 0x3e7)
    Func_02004bd4();
  if (cue == 178)
    Func_02004c98(work->target, 3);
}

void Func_020011fc(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  struct Actor *target;
  s32 position[3];
  s32 stepX;
  s32 stepZ;
  s32 i;
  s32 angle = (leader->facing + 0x2000) & 0xc000;

  position[0] = (leader->x & 0xfff00000) + 0x80000;
  position[1] = leader->y - 0x100000;
  position[2] = (leader->z & 0xfff00000) + 0x80000;
  FieldVector_AddPolarOffset(0x200000, angle, position);
  target = Actor_FindInCell(position, leader);
  if (target == 0)
    return;
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Func_02004c98(leader, 6);
  System_Wait(6);
  Event_PlaySound(152);
  Func_02004c98(leader, 7);
  leader->speedLimit = 0x30000;
  leader->acceleration = 0x20000;
  leader->velocityY = 0x4cccc;
  leader->motionFlags &= 0x7e;
  Actor_SetSingleChildFlags(leader, 0);
  stepX = (position[0] - leader->x) / 12;
  stepZ = (position[2] - leader->z) / 12;
  i = 0;
  do {
    leader->x += target->x - position[0] + stepX;
    leader->z += target->z - position[2] + stepZ;
    position[0] = target->x;
    position[2] = target->z;
    System_Wait(1);
    i++;
  } while (i != 12);
  leader->x = target->x;
  leader->y = leader->terrainHeight = target->y + 0x100000;
  leader->z = target->z;
  Func_02004c98(leader, 6);
  Actor_SetSingleChildFlags(leader, 1);
  leader->motionFlags |= 3;
  Event_End();
}

extern const s32 gData_02005050[];

void Func_02001350(void) {
  s32 srcX;
  s32 srcZ;
  s32 baseX = 0;
  struct MapLayer *layer = &Ram_HeapSlots->mapWork->layers[1];
  struct EffectOptions options;
  struct EffectOptions *effectOptions;
  struct EffectOptions *lastOptions;
  s32 position[3];
  s32 *burstPosition;
  s32 *sparkPosition;
  s32 *roomPosition;
  u32 t;
  s32 scene;
  s32 closing;
  typeof(gMainRoutineWork) *work;
  s32 riseX;
  u32 burstRandom, sparkRandom;
  u32 mask;
  u32 i;
  u32 row;

  /* FAKEMATCH(dead): srcX and srcZ start from a zero only combine sees; as srcX
   * = 0 and srcZ = 0, 138 of 537 instructions differ in tla-en. */
  srcX = ((u32)layer & 3) >> 2;
  srcZ = ((u32)layer & 7) >> 3;
  // t holds 212 for a set flag, then the frame count.
  t = 0;
  effectOptions = &options;
  effectOptions->type = 403;
  effectOptions->priority = 1;
  effectOptions->script = gData_02005050;
  effectOptions->startScaleX = 0x1cccc;
  effectOptions->startScaleY = 0x1cccc;
  scene = gMainRoutineWork.scene;
  roomPosition = position;
  if (scene == (s32)&SceneId_Ov692Room2) {
    baseX = 164 << 17;
    roomPosition[1] = 0x200000;
    roomPosition[2] = 200 << 16;
    srcX = 20;
    srcZ = 69;
    if (Event_GetFlag(768))
      t = 212;
  } else if (scene == (s32)&SceneId_Ov692Room3) {
    baseX = 232 << 16;
    roomPosition[1] = 0x200000;
    roomPosition[2] = 248 << 16;
    srcX = 14;
    srcZ = 72;
    if (Event_GetFlag(769))
      t = 212;
  } else if (scene == (s32)&SceneId_Ov692Room4) {
    baseX = 196 << 17;
    roomPosition[1] = 0x200000;
    roomPosition[2] = 134 << 18;
    srcX = 24;
    srcZ = 90;
    if (Event_GetFlag(770))
      t = 212;
  }
  // The loops are written with goto: they carry no loop notes, so the masks,
  // the shake constants and the counters' shifts stay in each pass, as in the
  // native; as for loops, 431 of 537 instructions differ in tla-en.
  i = 0;
burst:
  /* FAKEMATCH(temporary): burstRandom holding the first call's value; as the
   * call inside the sum, 4 of 537 instructions differ in tla-en. */
  burstRandom = Random16Far();
  mask = 31;
  /* FAKEMATCH(barrier): the empty asm clobbering sfp; as no asm, 230 of 537
   * instructions differ in tla-en. */
  asm("" : : : SFP_CLOBBER);
  burstPosition = position;
  burstPosition[0] = baseX + ((burstRandom & mask) << 16);
  Event_PlaySound(145);
  Func_0200015c(burstPosition[0], burstPosition[1], burstPosition[2], 0,
                (Random16Far() & mask) << 12, 0, 0x3a0001, effectOptions);
  MapGrid_SetShake(0x50000, 0x50000, 0x10000);
  MapGrid_SetShake(-1, -1, 0xe666);
  i++;
  Event_Wait(8);
  if (i != 16)
    goto burst;
  Event_PlaySound(148);
  effectOptions->priority = 1;
  effectOptions->startScaleX = 0x20000;
  effectOptions->startScaleY = 0x20000;
  effectOptions->targetScaleX = 0x8000;
  effectOptions->targetScaleY = 0x8000;
  baseX -= 0x80000;
  i = 0;
  riseX = baseX;
rise:
  burstPosition[0] = riseX;
  Func_0200015c(burstPosition[0], burstPosition[1], burstPosition[2], 0, 0,
                (Random16Far() & 31) << 10, 0xe0001, effectOptions);
  i++;
  riseX += 0x60000;
  if (i != 8)
    goto rise;
  MapGrid_CopyCellsTo(68, 70, srcX, srcZ, 3, 6);
  MapGrid_SetShake(0x50000, 0x70000, 0x10000);
  MapGrid_SetShake(-1, -1, 0xe666);
  Event_Wait(30);
  MapGrid_CopyCellsTo(71, 70, srcX, srcZ, 3, 6);
  effectOptions->type = 403;
  effectOptions->priority = 1;
  effectOptions->palette = 10;
  effectOptions->script = gData_0200502c;
  if (t != 0) {
    MapGrid_CopyCellsTo(68, 70, srcX, srcZ, 3, 6);
    Event_Wait(30);
    burstPosition[2] -= 0x280000;
  } else {
    MapGrid_SetShake(0x20000, 0x10000, 0x10000);
    Event_PlaySound(107);
    row = 0;
  row:
    i = 0;
  column:
    burstPosition[0] = baseX + ((Random16Far() % 48) << 16);
    Func_0200015c(burstPosition[0], burstPosition[1],
                  burstPosition[2] + (row << 18) + 0x80000, 0, 0x10000, 0,
                  0x330000, effectOptions);
    i++;
    if (i != 6)
      goto column;
    wait(3);
    MapGrid_CopyCellsTo(74, 70, srcX, srcZ, 3, (row >> 2) + 6);
    row++;
    if (row != 12)
      goto row;
    MapGrid_CopyCellsTo(74, 70, srcX, srcZ, 3, 8);
    t = 0;
  frame:
    if ((t & 3) == 0) {
      i = 0;
    spark:
      /* FAKEMATCH(temporary): sparkRandom holding the call's value; as the call
       * inside the sum, 2 of 537 instructions differ in tla-en. */
      sparkRandom = Random16Far();
      sparkPosition = position;
      sparkPosition[0] = baseX + ((sparkRandom % 48) << 16);
      Func_0200015c(sparkPosition[0], sparkPosition[1],
                    sparkPosition[2] + 0x300000, 0, 0x10000, 0, 0x330000,
                    effectOptions);
      i++;
      if (i != 4)
        goto spark;
    }
    if (t == 0) {
      *(s32 *)&layer->unk08[4] -= 0x200000;
      Map_CenterOnFollowedPoint();
      Event_Wait(1);
    } else if (t <= 63) {
      REG_BLDALPHA = ((16 - (t >> 2)) << 8) | (t >> 2);
      REG_BLDCNT = 0x3f44;
    } else if (t == 64) {
      REG_BLDCNT = 0;
    } else if (t <= 127) {
      *(s32 *)&layer->unk08[4] += 0x8000;
      roomPosition = position;
      roomPosition[2] -= 0x8000;
      MapGrid_CopyCellsTo(71, 76, srcX, srcZ - ((t - 47) >> 5) + 8, 3, 1);
    } else if (t == 128) {
      MapGrid_SetShake(-1, -1, 0xe666);
    }
    t++;
    wait(1);
    if (t != 160)
      goto frame;
    MapGrid_WaitForShakeToSettle();
  }
  Event_PlaySound(288);
  Event_PlaySound(136);
  lastOptions = &options;
  lastOptions->startScaleY = 0x1cccc;
  lastOptions->startScaleX = 0x1cccc;
  i = 0;
last:
  /* FAKEMATCH(temporary): sparkRandom holding the call's value; as the call
   * inside the sum, 2 of 537 instructions differ in tla-en. */
  sparkRandom = Random16Far();
  sparkPosition = position;
  sparkPosition[0] = baseX + ((sparkRandom % 48) << 16);
  i++;
  Func_0200015c(sparkPosition[0], sparkPosition[1], sparkPosition[2] + 0x300000,
                0, 0, 0, 0x3b0000, lastOptions);
  if (i != 6)
    goto last;
  MapGrid_SetShake(0x50000, 0x70000, 0x10000);
  MapGrid_SetShake(-1, -1, 0xe666);
  MapGrid_CopyCellsTo(68, 64, srcX, srcZ, 3, 6);
  MapGrid_WaitForShakeToSettle();
  Event_Wait(30);
  work = &gMainRoutineWork;
  *(s32 *)&layer->unk08[4] = 0;
  closing = work->scene;
  if (closing == (s32)&SceneId_Ov692Room2)
    Func_020034bc();
  else if (closing == (s32)&SceneId_Ov692Room3)
    Func_020034f0();
  else if (closing == (s32)&SceneId_Ov692Room4)
    Func_02003548();
}

/// A Magma Rock room's opening event: the leader is centred on its cell, the
/// screen shakes, and by room (2, 3 or 5, the last by entrance) it picks the
/// cells to open, the flag that was set (768 to 770, cleared here) and where
/// the effects play. The cells are copied open; if that flag was set, ten rows
/// of six effects rise along the opening while it widens and the blend fades
/// over 64 steps. Then the cells close again and the leader steps back a cell.
void Func_02001828(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  u32 facing = (leader->facing + 0x2000) & 0xc000;
  s32 position[3];
  struct EffectOptions options;
  s32 baseX = 0;
  s32 dstX = 0;
  s32 dstZ = 0;
  s32 open = 0;
  s32 srcX = 0;
  u8 *flags;
  u32 i = 0;
  u32 j;
  s32 *p;
  s32 rowAbove;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  // One local reused for the position's address, the loop counters and the
  // owner's address: the native keeps all of them in r6.
  j = (u32)position;
  ((s32 *)j)[0] = (leader->x & 0xfff00000) + 0x80000;
  ((s32 *)j)[2] = (leader->z & 0xfff00000) + 0x80000;
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, ((s32 *)j)[0] >> 16,
                      ((s32 *)j)[2] >> 16);
  FieldEvent_Wait(20);
  Event_PlaySound(584);
  flags = &leader->priorityFlags;
  *flags &= ~64;
  FieldEvent_Wait(20);
  MapGrid_SetShake(0x10000, 0x20000, 0x10000);
  Event_PlaySound(107);
  FieldEvent_Wait(30);
  // Each test reads the scene again: the native reloads gMainRoutineWork for
  // room 3's test and reuses that load for room 5's.
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov692Room2) {
    srcX = 20;
    i = 89;
    dstX = 84;
    dstZ = 26;
    baseX = 160 << 17;
    ((s32 *)j)[1] = 0;
    ((s32 *)j)[2] = 212 << 17;
    if (Event_GetFlag(768))
      open = 212;
    Event_ClearFlag(768);
    facing = 0;
    ((s32 (*)())Func_02000694)(0, 15, 13, 64);
    FieldActor_SetSpritePriority(15, 3);
    for (j = 0; j != 7; j++)
      Actor_SetAnimation(j + 9, 1);
  } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov692Room3) {
    srcX = 28;
    i = 83;
    dstX = 92;
    dstZ = 21;
    baseX = 224 << 17;
    ((s32 *)j)[1] = 0;
    ((s32 *)j)[2] = 172 << 17;
    if (Event_GetFlag(769))
      open = 212;
    Event_ClearFlag(769);
    ((s32 (*)())Func_02000694)(0, 24, 15, 64);
    ((s32 (*)())Func_02000694)(0, 9, 16, 64);
    Actor_SetAnimation(11, 4);
  } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov692Room5) {
    // The native tests and clears flag 770 in both arms.
    if (gMainRoutineWork.entrance <= 8) {
      srcX = 49;
      /* FAKEMATCH(dead): j set again to the position's address, which it
       * already holds; as without it, 25 of 496 instructions differ in tla-en.
       */
      j = (u32)position;
      i = 75;
      dstX = 113;
      dstZ = 13;
      baseX = 198 << 18;
      ((s32 *)j)[1] = 0;
      ((s32 *)j)[2] = 216 << 16;
      if (Event_GetFlag(770))
        open = 212;
      Event_ClearFlag(770);
    } else {
      srcX = 51;
      i = 104;
      dstX = 115;
      dstZ = 42;
      baseX = 206 << 18;
      ((s32 *)j)[1] = 0;
      ((s32 *)j)[2] = 170 << 18;
      if (Event_GetFlag(770))
        open = 212;
      Event_ClearFlag(770);
    }
  }
  Event_PlaySound(181);
  MapGrid_SetShake(0x50000, 0x70000, 0x10000);
  MapGrid_SetShake(-1, -1, 0xe666);
  rowAbove = i - 1;
  MapGrid_CopyCellsTo(srcX, i, srcX, rowAbove, 3, 4);
  if (open == 0) {
    MapGrid_WaitForShakeToSettle();
    FieldEvent_Wait(20);
  } else {
    MapGrid_SetShake(0x20000, 0x10000, 0x10000);
    options.type = 403;
    options.priority = 2;
    options.palette = 10;
    options.script = gData_0200502c;
    for (i = 0; i != 10; i++) {
      for (j = 0; j != 6; j++) {
        u32 random = Random16Far();

        p = position;
        p[0] = baseX + ((random % 48) << 16);
        Func_0200015c(p[0], p[1], p[2] + (i << 18) + 0x170000, 0, 0x10000, 0,
                      0x330000, &options);
      }
      System_Wait(3);
      MapGrid_CopyCellsTo(75, 64, dstX, dstZ, 3, (i >> 1) + 1);
    }
    for (j = 0; j != 64; j++) {
      REG_BLDALPHA = ((j >> 2) << 8) | (16 - (j >> 2));
      REG_BLDCNT = 0x3f44;
      System_Wait(3);
    }
    MapGrid_CopyCellsTo(78, 64, dstX, dstZ, 3, 5);
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov692Room2)
    Actor_Get(15)->priorityFlags |= 1;
  Event_PlaySound(288);
  Event_PlaySound(181);
  MapGrid_SetShake(0x50000, 0x70000, 0x10000);
  MapGrid_SetShake(-1, -1, 0xe666);
  MapGrid_CopyCellsTo(72, 63, srcX, rowAbove, 3, 4);
  MapGrid_WaitForShakeToSettle();
  Event_PlaySound(584);
  {
    s32 bit = 64;
    bit |= *flags;
    *flags = bit;
  }
  FieldEvent_Wait(20);
  j = (u32)&gMainRoutineWork.currentOwner;
  Actor_Get(*(s32 *)j)->actionFlags &= ~1;
  /* FAKEMATCH(temporary): the position's address held in a pointer, so that
   * the two halfword reads are taken from it as the native does (movs r2, #2;
   * ldrsh r1, [r5, r2]); as position[] and ((s16 *)position)[1], 37 of 496
   * instructions differ in tla-en. */
  p = position;
  p[0] = (leader->x & 0xfff00000) + 0x80000;
  p[1] = leader->y;
  p[2] = (leader->z & 0xfff00000) + 0x80000;
  FieldVector_AddPolarOffset(0x100000, (s32)facing ^ 0x8000, p);
  leader->facing = facing;
  Actor_WalkToAndWait(*(s32 *)j, ((s16 *)p)[1], ((s16 *)p)[5]);
  Actor_Get(*(s32 *)j)->actionFlags |= 1;
  Event_End();
}

// ---- games/tla/src/scene/magma_rock_inside/s1ca0.c ----
DEFINE_FUNC_02001CA0(Func_02001ca0, 9, 0x350)

DEFINE_FUNC_02001CA0(Func_02001cd0, 11, 0x351)

void Func_02001d04(struct Actor *object) {
  if (++object->timer > 64)
    object->update = 0;
  if ((object->timer & 1) == 0) {
    struct EffectConfig config;

    config.id = 403;
    config.kind = 2;
    config.variant = 10;
    config.data = gData_0200502c;
    Func_0200015c(object->x, object->y, object->z, 0, 0x1000, 0, 0x330000,
                  &config);
  }
}

// ---- games/tla/src/scene/magma_rock_inside/s1d64.c ----
extern const s32 gData_02005050[];

static __inline__ void Arrive(s32 id, s32 x, s32 z) {
  Actor_SetPosition(id, 194 << 18, 248 << 16);
  Actor_SetDestination(id, x, z);
  Actor_GetById(id)->velocityY = 0xc0000;
  Actor_GetById(id)->gravity = 0x8000;
  Actor_GetById(id)->timer = 0;
  Actor_GetById(id)->update = Func_02001d04;
}

void Func_02001d64(void) {
  s32 position[3];
  struct EffectOptions options;
  s32 i;

  Event_Start();
  Event_ResolvePendingActions(0);
  Event_PlaySound(107);
  MapGrid_SetShake(0x30000, 0x30000, 0x10000);
  Event_Wait(30);
  FieldCamera_MoveTo(194 << 18, -1, 136 << 17, 1);
  FieldCamera_WaitForArrival();
  options.type = 403;
  options.priority = 2;
  options.palette = 10;
  options.script = gData_02005050;
  options.startScaleY = 0x18000;
  options.startScaleX = 0x18000;
  position[1] = 0;
  i = 0;
loop:
  position[0] = ((Random16Far() & 31) << 16) + (190 << 18);
  position[2] = ((Random16Far() & 31) << 16) + (128 << 17);
  Func_0200015c(position[0], position[1], position[2], 0, 0x10000, 0, 0x3b0000,
                &options);
  if ((i & 7) == 0)
    Event_PlaySound(567);
  if (i == 32) {
    Arrive(13, 744, 216);
  } else if (i == 40) {
    Arrive(20, 776, 328);
  } else if (i == 48) {
    Arrive(14, 856, 232);
  } else if (i == 56) {
    Arrive(19, 696, 312);
  } else if (i == 64) {
    Arrive(15, 728, 280);
  } else if (i == 72) {
    Arrive(18, 824, 280);
  } else if (i == 80) {
    Arrive(16, 824, 312);
  } else if (i == 88) {
    Arrive(17, 776, 232);
  } else if (i == 112) {
    Actor_SetPosition(12, 194 << 18, 248 << 16);
    Actor_SetDestination(12, 696, 232);
    FieldCamera_MoveTo(174 << 18, -1, 232 << 16, 1);
    Actor_GetById(12)->velocityY = 0xd0000;
    Actor_GetById(12)->gravity = 0x8000;
    Actor_GetById(12)->timer = 0;
    Actor_GetById(12)->update = Func_02001d04;
    MapGrid_SetShake(-1, -1, 0xe666);
  }
  wait(3);
  i++;
  if (i != 128)
    goto loop;
  FieldActor_WaitForMoveById(12);
  MapGrid_WaitForShakeToSettle();
  Event_PlaySound(288);
  Event_Wait(40);
  Event_SetFlag(2552);
  FieldCamera_FollowActor(gMainRoutineWork.currentOwner, 1);
  FieldCamera_WaitForArrival();
  wait(10);
  Event_End();
}

void Func_02002128(void) {
  u8 *work = (u8 *)Ram_HeapSlots->eventWork;

  *(work + 0xd38) = 1;
  if (!Event_GetFlag(0x304)) {
    Actor_SetSpriteColor(Actor_Get(22), 15);
    Actor_SetPosition(22, PIXELS(632), PIXELS(312));
    Actor_Get(22)->scaleX = Actor_Get(22)->scaleY = 0x1000;
  }
}

DEFINE_FUNC_0200217C(Func_0200217c, 0x303, 21, 600, 312)

struct Actor;

void Func_020021c0(void) {
  u8 *work = Ram_HeapSlots->eventWork;
  struct Actor *object;
  struct Actor *actor;
  struct Actor *other;
  s32 i;

  object = Actor_GetById(((s8 *)work)[0xd30]);
  Actor_SetSpriteColor(object, 0);
  Event_PlaySound(136);
  i = 0;
  do {
    object->scaleY = object->scaleX += 0x1000;
    wait(5);
    i++;
  } while (i != 16);
  Event_SetFlag(((s8 *)work)[0xd30] + 0x2ee);
  if (Event_GetFlag(0x303) && Event_GetFlag(0x304)) {
    other = Actor_GetById(11);
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    MapGrid_SetShake(0x20000, 0x10000, 0x10000);
    Event_PlaySound(141);
    FieldEvent_Wait(30);
    FieldCamera_MoveTo(0x2680000, -1, 0x1280000, 1);
    FieldCamera_WaitForArrival();
    FieldEvent_Wait(30);
    Event_PlaySound(0x22d);
    REG_BLDALPHA = 0x1000;
    REG_BLDCNT = 0x3f10;
    {
      struct Actor *prepared = OverlayObject_PrepareObject(
          other->x, other->terrainHeight, other->z + 0x300000, 312);

      other->motionFlags = 0;
      actor = prepared;
    }
    i = 0;
    do {
      other->y -= 0x10000;
      FieldEvent_Wait(5);
      i++;
    } while (i != 32);
    Event_PlaySound(0x22a);
    Func_02004c98(other, 4);
    Actor_Release(actor);
    FieldEvent_Wait(1);
    REG_BLDCNT = 0;
    Func_02000694(0, 38, 18, 0);
    Event_End();
  }
}

void Func_02002318(void) {
  if (*(s8 *)((u8 *)Ram_HeapSlots->eventWork + 0xd30) == -1) {
    if (!Event_GetFlag(0x304))
      Actor_SetPosition(22, 0, 0);
    if (!Event_GetFlag(0x303))
      Actor_SetPosition(21, 0, 0);
  }
}

/// ★ Named from the game: "There's something burning like an ember within the
/// stone." (MsgBlazeStoneEmber)
void MagmaRockInside_ExamineEmberStone(void) {
  const u8 *message;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  message = MsgBlazeStoneEmber;
  Message_ShowCentered(message, 1);
  FieldEvent_SetMessageId(message + 1);
  Event_OpenMessage(12, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) ==
      0) {
    Party_GiveItem(460, 0);
    if (FieldPartyInventory_FindMember(460) == -1) {
      Event_End();
      return;
    }
    BattleEffect_StartEffectObject22(460, 3);
    Actor_SetAnimation(12, 2);
    Event_SetFlag(0x9f9);
  }
  Event_End();
}

void Func_020023e8(void) {
  s32 width = 1;
  s32 height = 2;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Event_PlaySound(188);
  MapGrid_CopyCellsTo(72, 67, 15, 19, width, height);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(73, 67, 15, 19, width, height);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(74, 67, 15, 19, width, height);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(75, 67, 15, 19, width, height);
  FieldEvent_Wait(15);
  Actor_Get(gMainRoutineWork.currentOwner)->motionFlags = 0;
  Actor_Get(gMainRoutineWork.currentOwner)->speedLimit =
      Actor_Get(gMainRoutineWork.currentOwner)->acceleration = 0x8000;
  Event_PlaySound(123);
  Actor_WalkTo(gMainRoutineWork.currentOwner, 248, 328);
  FieldEvent_Wait(5);
  FieldEvent_SetExitRequest(1);
  Event_End();
}

void Func_020024ac(void) {
  struct Actor *object = Actor_Get(FieldParty_GetLeaderActor());
  s32 x = object->x >> 20;
  s32 z = object->z >> 20;
  s32 key = (x << 16) + z;
  s32 last;
  s32 value;
  u8 *work;

  if (object->velocityX != 0 || object->velocityZ != 0)
    return;
  last = gData_02006710;
  if (last == -1 || key == last)
    return;
  x = last >> 16;
  z = last & 0xffff;
  gData_02006710 = -1;
  MapGrid_CopyCellsLayered(73, 72, Data_020067ec, Data_020067f0, Data_020067f4, Data_020067f8);
  if (gMainRoutineWork.scene != (s32)&SceneId_Ov692Room8)
    return;
  work = Ram_HeapSlots->eventWork;
  value = Func_02000658(2, x, z) >> 8;
  if (value != 0) {
    /* FAKEMATCH(temporary): the value plus 200 held in delay before the
       store; as *(s16 *)(work + 362) = value + 200, 6 of 70 instructions
       differ in tla-en. */
    s32 delay = value + 200;
    *(s16 *)(work + 362) = delay;
  }
}

void Func_0200255c(void) {
  s32 none = -1;
  s32 priority;

  gData_02006710 = none;
  priority = 0x480;
  FieldTask_AddFrame(Func_020024ac, priority);
}

/// Saves the 1 by 2 cells 64 columns across from the controlled object at
/// (73, 72) and stamps the cells kept at (70, 72) in their place, once per
/// cell; Func_02003d40 of Air's Rock is the same with another source.
void Func_0200257c(void) {
  struct Actor *object = Actor_Get(FieldParty_GetLeaderActor());
  s32 x = object->x >> 20;
  s32 y = object->y >> 20;
  s32 z = object->z >> 20;
  s32 key = (x << 16) + z;

  if (key != gData_02006710) {
    gData_02006710 = key;
    Data_020067ec = 1;
    Data_020067f0 = 2;
    Data_020067f4 = x + 64;
    Data_020067f8 = z - y;
    MapGrid_CopyCellsLayered(Data_020067f4, Data_020067f8, 1, 2, 73, 72);
    MapGrid_CopyCellsLayered(70, 72, Data_020067ec, Data_020067f0, Data_020067f4,
              Data_020067f8);
  }
}

/// As Func_0200257c, stamping the cells kept at (71, 72).
void Func_02002610(void) {
  struct Actor *object = Actor_Get(FieldParty_GetLeaderActor());
  s32 x = object->x >> 20;
  s32 y = object->y >> 20;
  s32 z = object->z >> 20;
  s32 key = (x << 16) + z;

  if (key != gData_02006710) {
    gData_02006710 = key;
    Data_020067ec = 1;
    Data_020067f0 = 2;
    Data_020067f4 = x + 64;
    Data_020067f8 = z - y;
    MapGrid_CopyCellsLayered(Data_020067f4, Data_020067f8, 1, 2, 73, 72);
    MapGrid_CopyCellsLayered(71, 72, Data_020067ec, Data_020067f0, Data_020067f4,
              Data_020067f8);
  }
}

#include "field_sprite.h"

void Func_020026a4(s32 x, s32 z) {
  struct Actor *object;
  s32 i;
  s32 cellZ;

  object = FieldActor_Create(288, x << 16, 0, z << 16);
  REG_BLDALPHA = 16;
  REG_BLDCNT = 0x3f44;
  Scene_SetActorSpritePriority(object, 0);
  Actor_SetSingleChildFlags(object, 0);
  ((struct FieldSprite *)object->sprite)->blendMode = 2;
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  REG_WINOUT = 0x3f1f;
  REG_DISPCNT |= 0x8000;
  Event_PlaySound(106);
  i = 0;
fadeIn:
  REG_BLDALPHA = (i << 9) | 16;
  System_Wait(1);
  if (++i != 8)
    goto fadeIn;
  i = 0;
fadeOut:
  REG_BLDALPHA = (16 - i) | 0x1000;
  System_Wait(1);
  if (++i != 16)
    goto fadeOut;
  x >>= 4;
  cellZ = (z >> 4) - 1;
  MapGrid_CopyCellsTo(65, 1, x + 64, cellZ, 1, 2);
  Func_02000694(0, x, cellZ, 0xff00);
  Actor_Release(object);
  Event_End();
  REG_BLDCNT = 0;
  REG_DISPCNT &= 0x7fff;
}

/// Reads what the leader stands on (Func_02000658, kind 2): on cells 1 and 7
/// it only redraws the marker when flag 0x200 is set; elsewhere it clears
/// (Func_0200257c), redraws the marker in the row the event work's counter
/// gives, sets flag 0x200 and the counter's flag (312 on).
void Func_020027d4(void) {
  u8 *work = Ram_HeapSlots->eventWork;
  struct Actor *leader;
  s32 cell, row = 54;

  leader = Actor_Get(gMainRoutineWork.currentOwner);
  cell = Func_02000658(2, leader->x >> 20, leader->z >> 20);
  work += 0x16a;
  // From 54 in two steps: as one expression the constants fold into
  // 858 - 4 * counter.
  row -= (*(s16 *)work - 201) << 2;
  cell >>= 8;
  if (cell != 1 && cell != 7) {
    Func_0200257c();
    Func_020026a4(0x108, row * 8 + 10);
    if (!Event_GetFlag(0x200))
      Event_SetFlag(0x200);
    Event_SetFlag(*(s16 *)work + 312);
  } else if (Event_GetFlag(0x200)) {
    Func_020026a4(0x108, row * 8 + 10);
  }
}

DEFINE_FUNC_0200217C(Func_02002868, 0x305, 11, 136, 360)

void Func_020028ac(void) {
  struct Actor *object;
  struct Actor *actor;
  struct Actor *other;
  s32 i;

  object = Actor_Get(((s8 *)Ram_HeapSlots->eventWork)[0xd30]);
  Actor_SetSpriteColor(object, 0);
  Event_PlaySound(136);
  i = 0;
  do {
    object->scaleY = object->scaleX += 0x1000;
    System_Wait(5);
    i++;
  } while (i != 16);
  Event_SetFlag(0x305);
  other = Actor_Get(9);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  MapGrid_SetShake(0x20000, 0x10000, 0x10000);
  Event_PlaySound(141);
  FieldEvent_Wait(30);
  FieldCamera_MoveTo(0x780000, -1, 0x1580000, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(30);
  Event_PlaySound(0x22d);
  REG_BLDALPHA = 0x1000;
  REG_BLDCNT = 0x3f10;
  {
    struct Actor *prepared = OverlayObject_PrepareObject(
        other->x, other->terrainHeight, other->z + 0x300000, 312);

    other->motionFlags = 0;
    actor = prepared;
  }
  i = 0;
  do {
    other->y -= 0x10000;
    FieldEvent_Wait(5);
    i++;
  } while (i != 32);
  Event_PlaySound(0x22a);
  Func_02004c98(other, 4);
  Actor_Release(actor);
  FieldEvent_Wait(1);
  REG_BLDCNT = 0;
  Func_02000694(0, 7, 21, 128);
  Event_End();
}

#include "call.h"
#include "event_work.h"
#include "edition.h"

void Func_020029dc(void) {
  if (*(s8 *)((u8 *)Ram_HeapSlots->eventWork + 0xd30) == -1) {
    if (!Event_GetFlag(0x305))
      Actor_SetPosition(11, 0, 0);
  }
}

void Func_02002a14(struct Actor *source) {
  struct Actor *object = Create(0x2a0, source->x, source->y, source->z);

  object->scaleY = 0x6666;
  object->scaleX = 0x6666;
  object->motionFlags = 0;
  Actor_SetSpriteColor(object, 10);
  Scene_SetActorSpritePriority(object, 1);
  Func_02004c98(object, 1);
  Actor_SetSingleChildFlags(object, 0);
  Animation_SetStateFlags(object, 1);
  Actor_StartScript(object, gData_02005534);
}

/* VIEW(magma_rock_inside): tag struct Orbiter as struct Orbiter__magma_rock_inside; module/scene/scene.h has another definition; tried with the definition the rest of the object has, it does not compile in tla-ja: games/tla/src/scene/magma_rock_inside.c:1762: structure has no member named `parent'. */
struct Orbiter__magma_rock_inside {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[0x38];
  s32 radius;
  u8 unk50[0x12];
  u8 timer;
  u8 unk63;
  u16 angle;
  u16 speed;
  struct Orbiter__magma_rock_inside *parent;
};

s32 Func_02002a6c(struct Orbiter__magma_rock_inside *object) {
  u16 *angle = &object->angle;
  u16 *speed = &object->speed;
  struct Orbiter__magma_rock_inside *parent = object->parent;
  s32 *position;
  s16 heading;

  object->angle += (s16)*speed >> 1;
  object->x = parent->x;
  object->y = parent->y + 0x80000;
  object->z = parent->z;
  heading = *angle;
  FieldVector_AddPolarOffset(object->radius, heading, position = &object->x);
  if (--object->timer == 0)
    return 0;
  if (object->radius == 0x20000)
    return 1;
  Func_02002a14(object);
  object->radius -= 0x40000;
  object->angle += (s16)*speed >> 1;
  *position = parent->x;
  object->y = parent->y + 0x80000;
  object->z = parent->z;
  FieldVector_AddPolarOffset(object->radius, (s16)*angle, position);
  Func_02002a14(object);
  object->radius -= 0x40000;
  return 1;
}

// Moves around in a circle.
DEFINE_FUNC_0200147C(Func_02002b2c)

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))

DEFINE_FUNC_020014D0(Func_02002bd0)

s32 Func_02002c1c(struct Actor *object) {
  u8 *steps;
  s32 v;
  u8 n;

  object->x += object->bounce;
  object->y += object->unk4c;
  object->z += object->gravity;
  steps = (u8 *)object + 98;
  v = *steps + 255;
  *steps = v;
  n = v;
  return n != 0;
}

void Func_02002c48(struct Actor *object) {
  struct EffectOptions options;
  s32 position[3];
  u32 frame = gFrameCount & 7;

  if (frame != 0)
    return;
  options.palette = 7;
  options.type = 673;
  options.priority = 1;
  position[0] = object->x + (((Random16Far() & 31) - 16) << 16);
  position[1] = object->y + ((Random16Far() & 31) << 16) + 0x80000;
  position[2] = object->z;
  Func_0200015c(position[0], position[1], position[2] + 0x40000, 0, 0x8000,
                frame, 0x130000, &options);
}

union ActorView {
  struct Actor actor;
  struct {
    u8 unk00[0x62];
    u8 unk62;
    u8 unk63;
  } bytes;
};

extern const u8 gData_02005590[];
extern const s32 gData_02005544[];
extern const s32 gData_02005560[];
extern const u8 gData_020055b4[];
extern const s32 gData_02005570[];
extern const u8 gData_020055d8[];
extern const u8 gData_02005608[];

void Func_02002cd0(s32 a, s32 b) {
  struct Actor *first = Actor_GetById(a);
  struct Actor *second = Actor_GetById(b);
  struct Actor *object;
  struct Actor *spark;
  struct Actor *riser;
  s32 i;
  u16 noMotion;
  s32 probe;
  const u8 *script;
  // Unused: gives the stack frame its 12 bytes.
  s32 unused[3];

  script = gData_020055b4;
  Event_PlaySound(78);
  Event_Wait(30);
  FieldCamera_SetSpeed(0x8000, 0x1000);
  FieldCamera_MoveTo(((s16 *)&first->x)[1] << 16,
                             ((s16 *)&first->y)[1] << 16,
                             ((s16 *)&first->z)[1] << 16, 1);
  Event_PlaySound(141);
  MapGrid_SetShake(0x20000, 0x20000, 0x10000);
  BattleEffect_ApplyColorToSourceBuffer(0x10000, 0);
  BattleEffect_ApplyColorToTargetBuffer(0x10003, 1);
  BattleEffect_StartBufferInterpolation(60);
  first->motionFlags = 0;
  first->acceleration = 0x3333;
  first->speedLimit = 0x3333;
  Actor_SetScript(a, gData_02005590);
  Event_PlaySound(194);
  Event_Wait(45);
  Actor_SetSpriteColorById(a, 256);
  REG_BLDALPHA = 0x1008;
  REG_BLDCNT = 0x3f10;
  i = 0;
burst:
  object = (struct Actor *)(struct Actor *)Import_ActorCreate(
      672, first->x, first->y, first->z);
  Event_PlaySound(246);
  object->x += ((Random16Far() & 31) - 16) << 16;
  object->y += ((Random16Far() & 15) - 8) << 16;
  object->speedLimit = 0x80000;
  object->acceleration = 0x3333;
  object->motionFlags = 0;
  Actor_SetSpriteColor(object, 10);
  Scene_SetActorSpritePriority(object, 1);
  Func_02004c98(object, 7);
  Actor_SetSingleChildFlags(object, 0);
  Animation_SetStateFlags(object, 1);
  Actor_StartScript(object, gData_02005544);
  REG_BLDALPHA = 0x1004;
  wait(2);
  REG_BLDALPHA = 0x100a;
  wait(2);
  REG_BLDALPHA = 0x1010;
  wait(2);
  REG_BLDALPHA = 0x100a;
  wait(2);
  i++;
  if (i != 16)
    goto burst;
  REG_BLDALPHA = 0x1008;
  wait(30);
  i = 0;
  /* FAKEMATCH(dead): the never-taken jump into the spiral keeps its refs at
   * weight 1; as the spiral alone, 174 of 402 instructions differ in tla-en. */
  probe = second->unk57;
  if (probe & 0x100) {
    wait(1);
    goto tail;
  }
spiral:
  spark = FieldActor_Create(672, second->x, second->y, second->z);
  Event_PlaySound(195);
  spark->timer = (Random16Far() & 0x7fff) + 0x8000;
  spark->unk66 = (((i & 1) << 1) - 1) << ((Random16Far() & 3) + 9);
  noMotion = 0;
  spark->motionFlags = noMotion;
  spark->unk4c = 0x7a0000;
  spark->watchedObject = second;
  ((union ActorView *)spark)->bytes.unk62 = ((16 - i) << 3) + 15;
  Actor_SetSpriteColor(spark, 10);
  Scene_SetActorSpritePriority(spark, 1);
  Func_02004c98(spark, 7);
  Actor_SetSingleChildFlags(spark, 0);
  Animation_SetStateFlags(spark, 1);
  FieldVector_AddPolarOffset(spark->unk4c, spark->timer, &spark->x);
  Actor_StartScript(spark, gData_02005560);
  if (i == 3) {
    Actor_SetSpriteColorById(b, 256);
    second->motionFlags = noMotion;
    second->acceleration = 0x3333;
    second->speedLimit = 0x3333;
    Actor_SetScript(b, script);
  }
tail:
  wait(8);
  i++;
  if (i != 16)
    goto spiral;
  Event_PlaySound(220);
  wait(16);
  i = 2;
column:
  riser = FieldActor_Create(672, second->x, second->y + i * 0x10000 - 0x80000,
                            second->z);
  riser->timer = Random16Far();
  riser->unk66 = 0x800;
  ((union ActorView *)riser)->bytes.unk62 = i;
  ((union ActorView *)riser)->bytes.unk63 = 1;
  riser->bounce = riser->x;
  riser->unk4c = 0;
  riser->gravity = riser->z;
  riser->watchedObject = second;
  riser->motionFlags = (u16)0;
  riser->priorityFlags = 1;
  Actor_SetSpriteColor(riser, 10);
  Func_02004c98(riser, 7);
  Actor_SetSingleChildFlags(riser, 0);
  Animation_SetStateFlags(riser, 1);
  Actor_StartScript(riser, gData_02005570);
  i += 2;
  if (i != 32)
    goto column;
  Event_PlaySound(220);
  wait(50);
  BattleEffect_ApplyColorToTargetBuffer(0x7fff, 0);
  BattleEffect_StartBufferInterpolation(8);
  wait(16);
  MapGrid_SetShake(0, 0, 0);
  Actor_SetSpriteColorById(a, 0);
  Actor_SetSpriteColorById(b, 0);
  Event_PlaySound(574);
  BattleEffect_ApplyColorToTargetBuffer(0x10000, 0);
  BattleEffect_StartBufferInterpolation(80);
  Actor_SetScript(a, gData_020055d8);
  Actor_SetScript(b, gData_02005608);
  second->update = (void (*)(void))Func_02002c48;
  wait(120);
  second->update = 0;
  Event_PlaySound(390);
  Party_PlaySavedMusic();
  wait(1);
}

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
#define EVENT_WORK ((struct EventWork *)Ram_HeapSlots->eventWork)

s32 Actor_MoveToObject();

/// Reading the Blaze tablet shows its inscription; the first time (flag 0x9fa
/// clear) the scene with actor 5 follows.
void Func_020030cc(void) {
  struct Actor *object;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Message_ShowCentered(MsgBlazeTabletInscription, 1);
  if (Event_GetFlag(0x9fa) != 0) {
    Event_End();
  } else {
    FieldEvent_SetMessageId((s32)MsgBlazeTabletInscription);
    EVENT_WORK->messageId++;
    Actor_SetSpeed(gMainRoutineWork.currentOwner, 0xb333, 0x5999);
    Actor_SetSpeed(5, 0xb333, 0x5999);
    Actor_MoveToObject(5, gMainRoutineWork.currentOwner);
    FieldEvent_Wait(1);
    Actor_WalkTo(gMainRoutineWork.currentOwner, 256, 136);
    Actor_WalkTo(5, 272, 136);
    FieldActor_WaitForMoveById(gMainRoutineWork.currentOwner);
    FieldActor_WaitForMoveById(5);
    Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_EAST, 0);
    Actor_FaceDirection(5, FACING_WEST, 0);
    FieldEvent_Wait(10);
    Event_ShowMessage(5, 0);
    Actor_SetAnimation(gMainRoutineWork.currentOwner, 3);
    FieldEvent_Wait(30);
    Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 256, 152);
    Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
    FieldEvent_Wait(20);
    Actor_FaceDirection(5, FACING_SOUTH, 0);
    FieldEvent_Wait(20);
    Actor_SetAnimation(5, 3);
    FieldEvent_Wait(30);
    Actor_FaceDirection(5, FACING_NORTH, 0);
    FieldEvent_Wait(20);
    Actor_WalkToAndWait(5, 264, 137);
    Actor_FaceDirection(5, FACING_NORTH, 0);
    FieldEvent_Wait(20);
#if (LANGUAGE == LANGUAGE_JAPANESE) || (LANGUAGE == LANGUAGE_ENGLISH)
    Message_ShowCentered(MsgBlazeTabletInscription + 3, 1);
#else
    Message_ShowCentered(MsgBlazeTabletInscription + 2, 1);
#endif
    MapGrid_SetShake(0x50000, 0x70000, 0x10000);
    MapGrid_SetShake(-1, -1, 0xe666);
    MapGrid_WaitForShakeToSettle();
    Call2(Func_02002cd0, 8, 5);
    Party_LearnAbility(5, 0x9a);
    Event_SetFlag(0x9fa);
    Actor_FaceEachOther(gMainRoutineWork.currentOwner, 5, 0);
    FieldEvent_Wait(10);
    Actor_SetAnimation(5, 3);
    Actor_SetAnimation(gMainRoutineWork.currentOwner, 3);
    FieldEvent_Wait(20);
    Actor_SetSpeed(5, 0x13333, 0x9999);
    Actor_SetAnimation(5, 2);
    object = Actor_Get(gMainRoutineWork.currentOwner);
    if (object != 0)
      Actor_SetDestination(5, FIELD_AT(object, s16, 10),
                           FIELD_AT(object, s16, 18));
    FieldActor_WaitForMoveById(5);
    Actor_SetPosition(5, 0, 0);
    Actor_SetActionCallback(Actor_Get(8), 9);
    Event_End();
  }
}

void Func_020032d8(void) {
  if (Event_GetFlag(0x9fa)) {
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    Message_ShowCentered(MsgBlazeTabletInscription, 1);
    Event_End();
  } else {
    Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 248, 107);
    Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 248, 136);
    Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 264, 136);
    Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
    FieldEvent_Wait(10);
    Func_020030cc();
  }
}

DEFINE_FUNC_02003348(Func_02003348, 280)

DEFINE_FUNC_02003348(Func_020033b0, 248)

extern u8 gMagmaRockInside_Triggers[];
extern u8 gMagmaRockInside_Room2Triggers[];
extern u8 gMagmaRockInside_Room3Triggers[];
extern u8 gMagmaRockInside_Room4Triggers[];
extern u8 gMagmaRockInside_Room5Triggers[];
extern u8 gMagmaRockInside_Room6Triggers[];
extern u8 gMagmaRockInside_Room7Triggers[];
extern u8 gMagmaRockInside_Room8Triggers[];

u8 *Scene_GetEvents(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene != (s32)&SceneId_Ov692Room1) {
    if (scene == (s32)&SceneId_Ov692Room2)
      return gMagmaRockInside_Room2Triggers;
    if (scene == (s32)&SceneId_Ov692Room3)
      return gMagmaRockInside_Room3Triggers;
    if (scene == (s32)&SceneId_Ov692Room4)
      return gMagmaRockInside_Room4Triggers;
    if (scene == (s32)&SceneId_Ov692Room5)
      return gMagmaRockInside_Room5Triggers;
    if (scene == (s32)&SceneId_Ov692Room6)
      return gMagmaRockInside_Room6Triggers;
    if (scene == (s32)&SceneId_Ov692Room7)
      return gMagmaRockInside_Room7Triggers;
    if (scene == (s32)&SceneId_Ov692Room8)
      return gMagmaRockInside_Room8Triggers;
  }
  return gMagmaRockInside_Triggers;
}

void Func_020034bc(void) {
  s32 i;

  Event_SetFlag(0x300);
  Func_02000694(0, 15, 13, 0);
  for (i = 0; i != 6; i++)
    Actor_SetAnimation(i + 9, 2);
  Actor_SetAnimation(15, 4);
}

void Func_020034f0(void) {
  Event_SetFlag(0x301);
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov692Room3) {
    Func_02000694(0, 24, 15, 0);
    Func_02000694(0, 9, 16, 0);
    Actor_SetAnimation(11, 4);
  } else {
    Func_02000694(0, 26, 14, 0);
  }
}

void Func_02003548(void) {
  // Event_SetFlag, event.h's command, keeps 0x302 from being shared with the
  // script calls, which build it again after the scene test; GameFlag_SetBit
  // called directly differs in 56 instructions.
  Event_SetFlag(0x302);
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov692Room4) {
    s16 *script = gData_0200525c;

    Func_02000800(15, script, 0x302);
    Func_02000800(16, gData_0200525c, 0x302);
    Func_02000800(17, gData_020052bc, 0x302);
    Func_02000800(18, gData_020052bc, 0x302);
    Func_02000800(19, gData_0200531c, 0x302);
    Func_02000800(20, gData_0200531c, 0x302);
    Func_02000800(21, gData_0200537c, 0x302);
    Func_02000800(22, gData_0200537c, 0x302);
    if (Actor_Get(16)->step == 0) {
      Actor_Get(16)->step = 10;
      Actor_Get(18)->step = 10;
      Actor_Get(20)->step = 10;
      Actor_Get(22)->step = 10;
    }
    Actor_SetAnimation(23, 4);
    Actor_SetAnimation(24, 4);
    Actor_SetAnimation(25, 4);
    Actor_SetAnimation(26, 2);
  }
}

void Func_02003630(struct Actor *arg) {
  struct MainRoutineWork *party = &gMainRoutineWork;
  struct Actor *object;
  struct Actor *owner;
  s16 ownerY;
  s16 y;
  u8 *flags;

  do {
  } while (0);
  object = arg;
  owner = Actor_Get(party->currentOwner);
  ownerY = owner->y >> 16;
  y = object->y >> 16;
  flags = &object->priorityFlags;

  if (ownerY != y || (s16)(owner->z >> 16) < (s16)(object->z >> 16)) {
    *flags = 1;
  } else {
    *flags = 0;
    Scene_SetActorSpritePriority(object, 3);
  }
}
#undef BYTE_AT
#undef FIELD_AT
#undef EVENT_WORK
