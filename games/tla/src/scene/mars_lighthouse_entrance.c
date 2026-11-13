/// @file
/// Mars Lighthouse's lower floors, inside its gates, in the rooms of this
/// overlay: ice cones Pound flattens (MarsLighthouseEntrance_PoundIceCone),
/// ice sheets that burst and ice to Grind (MarsLighthouseEntrance_GrindIce),
/// floors of ice the leader slides across (MarsLighthouseEntrance_SlideOnIce),
/// the platforms that carry the party (Ride_UpdateCarried), a statue pushed
/// into the flames (MarsLighthouseEntrance_PushStatueIntoFlames) and the
/// teleporters on (MarsLighthouseEntrance_Teleport). The Mars Djinni Fugue and
/// the Mercury Djinni Balm fight here (MarsLighthouseEntrance_FightFugue,
/// MarsLighthouseEntrance_FightBalm).

#include "shared/map_cells.h"
#include "flags.h"
#include "field/event_inline.h"
#include "field/field.h"
#include "module/scene/mars_lighthouse_entrance.h"
#include "module/scene/scene.h"
#include "battle/battle.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "lib.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "scene_tables.h"
#include "shared/mars_lighthouse_entrance_room.h"
#include "map.h"
#include "shared/mars_lighthouse.h"
#include "shared/actors.h"
#include "system/main_routine_work.h"
#include "actor.h"
#include "event_work.h"
#include "item_icon.h"
#include "module/field/field.h"
#include "module/tla.h"
#include "scene_ids.h"
#include "field_event.h"
#include "system/field_effect.h"
#include "system/iwram_call.h"
#include "system/ram_buffer.h"
#include "graphics/vram_tab.h"
#include "system/task.h"
#include "system/io_reg.h"
#include "version.h"
#include "call.h"
#include "field/event_work.h"
#include "battle/battle_unit.h"
#include "system/vector3.h"
#include "fixed_math.h"
#include "graphics/display_transition.h"
#include "module/system/system.h"
#include "field_sprite.h"
#include "macros.h"
#include "system/input.h"
#include "shared/cracked_tile.h"
#include "system/heap_release.h"
#include "field/cell_target.h"
#include "shared/tables.h"
#include "shared/effects.h"
#include "module/field/field_effect.h"
#include "system/dma.h"
#include "objcmd.h"
#ifndef TLA_MARS_LIGHTHOUSE_ENTRANCE_RIDE_H
#define TLA_MARS_LIGHTHOUSE_ENTRANCE_RIDE_H

#include "types.h"
#include "system/ram_buffer.h"
#include "system/frame.h"
#include "actor.h"

struct RideEntry {
  s16 a;
  u8 unk02[2];
  s16 b;
  s16 c;
  u8 unk08[4];
  s32 d;
};

struct RideRow {
  u8 unk00[20];
  s16 a;
  u8 unk16[2];
  s16 b;
  s16 c;
  u8 unk1c[4];
  s32 d;
};

struct RideSlot {
  u16 value;
  u8 unk02[2];
};

struct RideWork {
  s32 counter;
  s16 active;
  u8 unk06[8];
  s16 value0e;
  u8 unk10[4];
  struct RideEntry entries[4];
  struct RideSlot slots[4];
  u8 unk64[12];
  void *handle;
};

extern u8 gInventorySnapshot[];

#define RIDE_WORK ((struct RideWork *)(gInventorySnapshot + 0x188))

void Actor_SetSpriteColorMode(struct Actor *object, s32 value);

#endif

s32 Func_02000038(struct Actor *object) {
  Actor_SetSpriteFlags(object, 0);
  return 0;
}

void MarsLighthouseEntrance_FightFugue(void) {
  Field_RequestDjinnBattle(11, 57);
}

void MarsLighthouseEntrance_FightBalm(void) {
  Field_RequestDjinnBattle(17, 36);
}

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern struct ActorPlacement gMarsLighthouseEntrance_Placements[];
extern struct ActorPlacement gMarsLighthouseEntrance_Room1Placements[];
extern struct ActorPlacement gMarsLighthouseEntrance_Room2Placements[];
extern struct ActorPlacement gMarsLighthouseEntrance_Room3Placements[];
extern struct ActorPlacement gMarsLighthouseEntrance_Room4Placements[];
extern struct ActorPlacement gMarsLighthouseEntrance_Room5Placements[];
extern struct ActorPlacement gMarsLighthouseEntrance_Room6Placements[];
extern struct ActorPlacement gMarsLighthouseEntrance_Room7Placements[];

s32 Func_02000070(void) {
  Func_0200246c(0, 65);
  return 0;
}

const struct ActorPlacement *Scene_GetPlacements(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6aaRoom1)
    return gMarsLighthouseEntrance_Room1Placements;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6aaRoom2)
    return gMarsLighthouseEntrance_Room2Placements;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6aaRoom3)
    return gMarsLighthouseEntrance_Room3Placements;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6aaRoom4)
    return gMarsLighthouseEntrance_Room4Placements;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6aaRoom5)
    return gMarsLighthouseEntrance_Room5Placements;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6aaRoom6)
    return gMarsLighthouseEntrance_Room6Placements;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6aaRoom7)
    return gMarsLighthouseEntrance_Room7Placements;
  return gMarsLighthouseEntrance_Placements;
}

void Func_02000118(void) {
  u8 *event = Ram_HeapSlots->eventWork;

  event[0xd38] = 1;
}

extern struct MapCell gMapCellBuffer[];

DEFINE_MAP_CELL_GET(MapCell_Get)

DEFINE_MAP_CELL_SWAP(MapCell_Swap)

#include "../field/object/set_sprite_priority.c"

#include "text/msg_ids.h"

void MapGrid_SetCellHeight();
void Teleport_PlayWhirl(void);
void Teleport_PlaySparks(void);
extern u16 gData_020068a2;
extern u32 Data_020068a4[3];
extern s16 gData_020068a0;
void TileMap_ConvertBuffer(s32 scene, s32 a1);
s32 FieldEffect_StartBlankOverlay();
s32 Weather_StartRain();
extern s32 Data_02006878;

s32 MapCell_Get();
s32 Map_GetTerrainHeight();
void Scene_SetActorSpritePriority();

void MarsLighthouseEntrance_SetIceConeCells(struct Actor *object) {
  s32 t3, v10, v6, v7, v8, v9;
  struct Actor *v5;
  u8 *v11;

  v5 = object;
  v9 = (v5->x >> 20);
  v10 = (v5->z >> 20);
  v8 = MapCell_Get(0, v9, v10);
  v6 = MapCell_Get(2, v9, v10);
  v11 = &v5->terrainId;
  t3 = Map_GetTerrainHeight(v5->terrainId, v5->x, v5->z);
  v7 = (t3 >> 19);
  if (Event_GetFlag(v5->timer)) {
    v6 &= -0xff01;
    v8 &= -129;
    if ((v5->terrainHeight >> 19) == v7)
      goto L_02008282;
    v7 = (v7 + -4);
    goto L_02008282;
  }
  v6 &= 255;
  v6 |= 0xff00;
  v8 |= 128;
  v7 = (v7 + 4);
  Scene_SetActorSpritePriority(v5, 2);
L_02008282:
  MapCell_Swap(0, v9, v10, v8);
  MapCell_Swap(2, v9, v10, v6);
  MapGrid_SetCellHeight((v9 << 20), (v10 << 20), *v11, v7);
}

void Actor_SelectAnimation();

/// Each row: an ice cone's actor, its first animation and the flag set once
/// Pound has flattened it.
void MarsLighthouseEntrance_PlaceIceCones(u16 *table) {
  u16 *row;
  struct FieldActor *object;
  s16 flag;
  s16 animation;

  if (table[0] == 0xffff)
    return;
  row = table;
loop:
  object = (struct FieldActor *)Actor_Get(row[0]);
  animation = row[1];
  flag = row[2];

  Actor_SetSpriteFlags(object, 0);
  Actor_SelectAnimation(object, (u16)animation + Event_GetFlag((u16)flag));
  object->motionFlags = 0;
  object->collisionFlags &= 0xfe;
  object->timer = flag;
  MarsLighthouseEntrance_SetIceConeCells(object);
  row += 3;
  if (row[0] != 0xffff)
    goto loop;
}

/// While index is at most 7, spawns effect type 0x117 index steps below the
/// object the effect work names and shrinks the object a little; mode 1
/// restores its size, sets its flag, hands it to
/// MarsLighthouseEntrance_SetIceConeCells and clears its position.
void MarsLighthouseEntrance_PoundIceCone(s32 mode, s32 index) {
  struct EffectHolder *holder = Ram_HeapSlots->effectWork;
  struct Actor *object = Actor_Get(holder->objectId);

  if (index <= 7) {
    struct EffectOptions options;

    options.priority = 0;
    options.type = 0x117;
    options.startScaleX = options.startScaleY = 0x10000;
    options.targetScaleX = options.targetScaleY = 0x8000;
    options.rotation = (Random16() & 15) << 12;
    FieldEffect_Spawn(object->x, object->y - index * 3 * 0x10000 + 0x100000,
                      object->z, ((Random16() & 15) - 8) << 13, 0, 0, 0x5a0000,
                      &options);
    object->scaleY -= 0x1f00;
  }
  if (mode == 1) {
    object->scaleY = 0x10000;
    Event_SetFlag(object->timer);
    MarsLighthouseEntrance_SetIceConeCells(Actor_Get(holder->objectId));
    object->x = object->y = object->z = 0;
  }
}

/* VIEW(mars_lighthouse_entrance): routine MarsLighthouseEntrance_PoundIceCone
 * as void MarsLighthouseEntrance_PoundIceCone(void); the definition has void
 * MarsLighthouseEntrance_PoundIceCone(s32 mode, s32 index); not tried:
 * MarsLighthouseEntrance_PoundIceCone's bound declaration was not found. */
extern void Func_02000338__mars_lighthouse_entrance(void) __asm__(
    "MarsLighthouseEntrance_PoundIceCone");

void MarsLighthouseEntrance_StartPoundIceCone(void) {
  Func_02000338__mars_lighthouse_entrance();
}

extern u32 gData_02005394[];

void Func_02000414(s32 id) {
  struct Actor *source = Actor_Get(id);
  s32 i;

  i = 5;
next:
  {
    s32 x = source->x + (Random16() << 4) - (Random16() << 4);
    struct Actor *object =
        Func_02004bd8_(0x145, x, (Random16() << 3) + source->y + 0x40000,
                       source->z + (Random16() << 4) - (Random16() << 4));

    if (object != NULL) {
      s32 angle;
      s32 radius;
      s32 scale;

      angle = Random16();
      radius = Random16();
      scale = (Random16() >> 1) + 0x4000;
      Actor_StartScript(object, gData_02005394);
      Actor_SetSpriteFlags(object, 0);
      /* FAKEMATCH(dead): an empty once-only block holds the order around it. */
      do {
      } while (0);
      object->velocityY = angle * 4 + 0x20000;
      object->velocityZ = Iwram_MulQ16(radius * 8, Trig_Cos(angle));
      object->velocityX = Iwram_MulQ16(radius * 8, Math_Sin(angle));
      object->acceleration = 0;
      object->gravity = 0x547a;
      object->scaleX = scale;
      object->scaleY = scale;
      object->bounce = 0x8000;
      Actor_SetSpriteColorMode(object, 1);
    }
  }
  if (--i >= 0)
    goto next;
}

/// Grind on the ice at the dead end, actor 10: the crack runs to the ice sheet
/// a floor below.
void MarsLighthouseEntrance_GrindIce(void) {
  s32 x;
  s32 count;

  Event_PlaySound(SE_572);
  x = 32;
  count = 6;
step:
  Func_02004c30_(x, 125, 3, 3, 39, 85);
  Func_02000414(10);
  count--;
  System_Wait(10);
  x += 3;
  if (count >= 0)
    goto step;
  ObjectTable_Snapshot();
  FieldEvent_SetExitRequest(20);
}

void MarsLighthouseEntrance_CheckIce(void) {
  FieldMessage_Show((s32)MsgCheckedTheIce, 1);
  FieldMessage_Show((s32)MsgCheckedTheIce + 1, 1);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->unk15a = 1;
}

/// Teleport on the circle: the first time the camera follows the leader across;
/// after, the leader leaves at once.
void MarsLighthouseEntrance_Teleport(void) {
  if (!Flag_IsSet(0xa36)) {
    Func_020046b4();
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    Teleport_PlaySparks();
    WORD_AT(Actor_Get(gMainRoutineWork.currentOwner), 108) = 0;
    FieldActor_SetSpriteColor(Actor_Get(gMainRoutineWork.currentOwner), 0);
    Func_02004cd8_(gMainRoutineWork.currentOwner, PIXELS(688), PIXELS(360));
    FieldCamera_MoveTo(PIXELS(688), -1, PIXELS(360), 1);
    FieldCamera_WaitForArrival();
    Teleport_PlayWhirl();
    FieldEvent_Wait(40);
    Func_02004718();
    FieldPsynergy_FadeFromCastColor();
    System_Wait(10);
    Event_End();
    Flag_Set(0xa36);
  } else {
    Func_020046b4();
    Teleport_PlaySparks();
    {
      u8 *party = (u8 *)&gMainRoutineWork;

      party[0x2c2] = 2;
    }
    FieldEvent_SetExitRequest(18);
    FieldEvent_Wait(40);
    Func_02004718();
  }
}

DEFINE_SCENE_LEAVE_BY_EXIT(Func_02000648, Func_020046b4, Teleport_PlaySparks,
                           17, Func_02004718)

/// Burst on the cracked ice sheet, actor 18: a flash, the sheet gone, chips
/// flying (flag 0xa3f).
void MarsLighthouseEntrance_BurstIceSheet(s32 state, s32 frame) {
  if (state == 1) {
    Event_PlaySound(SE_574);
    PaletteFade_ApplyTarget(0x10000, 0);
    PaletteFade_Interpolate(60);
    Func_02004c30_(47, 94, 10, 6, 47, 100);
    Func_02004c28_(110, 40, 12, 4, 46, 40);
    Actor_SetPosition(18, 0, 0);
    Flag_Set(0xa3f);
  }
  if (frame > 96 && frame < 272 && frame % 30 == 0)
    Func_02000414(18);
  if (frame == 276) {
    PaletteFade_ApplyTarget(0x7fff, 0);
    PaletteFade_Interpolate(20);
  }
}

void Func_02000710(void) { Func_02001a00(); }

void Func_02000718(void) {
  SCENE_WORK->unk166 = 0;
  Flag_Clear(FLAG_MENU_OPEN);
  Flag_Clear(0x104);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_SetSpeed(0x10000, 0x2000);
  Camera_SetBounds(PIXELS(168), 0, PIXELS(664), 1);
  FieldCamera_WaitForArrival();
  System_Wait(10);
  FieldCamera_SetSpeed(0x40000, 0x8000);
  Camera_SetBounds(PIXELS(472), 0, PIXELS(664), 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(90);
  Event_End();
}

void Func_02000798(void) {
  MapGrid_CopyCellAttributes(14, 55, 14, 3, 14, 41);
  MapGrid_CopyCellsLayered(64, 64, 14, 3, 78, 41);
  MapGrid_CopyCellsLayered(64, 64, 14, 3, 14, 104);
  MapGrid_CopyCellAttributes(64, 64, 14, 3, 78, 41);
  MapGrid_CopyCellAttributes(64, 64, 14, 3, 14, 105);
}

void Func_02000808(void) {
  struct Actor *object = Actor_Get(20);

  if (!Flag_IsSet(0xa47)) {
    Event_PlaySound(SE_573);
    Flag_Set(FLAG_SCENE_TEMP_31);
    object->x = S16_AT(object, 100) << 16;
    object->z = S16_AT(object, 102) << 16;
    Flag_Set(0xa48);
    Func_02000798();
  }
}

#define DEFINE_FUNC_0200085C(name, n1, n2, n3, n4, n5, n6, n7, n8, n9)         \
  void name(s32 state, s32 frame) {                                            \
    if (state == 1) {                                                          \
      Event_PlaySound(SE_574);                                                 \
      PaletteFade_ApplyTarget(0x10000, 0);                                     \
      PaletteFade_Interpolate(60);                                             \
      MapGrid_CopyCellsLayered(n1, n2, 8, n3, n1, n4);                         \
      MapGrid_CopyCellAttributes(n5, n6, 8, n7, n1, n6);                       \
      Actor_SetPosition(n8, 0, 0);                                             \
      Flag_Set(n9);                                                            \
    }                                                                          \
    if (frame > 96 && frame < 272 && frame % 30 == 0)                          \
      Func_02000414(n8);                                                       \
    if (frame == 276) {                                                        \
      PaletteFade_ApplyTarget(0x7fff, 0);                                      \
      PaletteFade_Interpolate(20);                                             \
    }                                                                          \
  }

DEFINE_FUNC_0200085C(Func_0200085c, 28, 92, 8, 100, 92, 38, 8, 20, 0xa47)

/// Move on the statue, actor 8, to cell x 20: it blocks the last flame thrower
/// (flag 0xa4f).
void MarsLighthouseEntrance_PushStatueIntoFlames(s32 unused, s32 id) {
  if (id == 8) {
    struct Actor *object = Actor_Get(8);

    if (object->x >> 20 == 20) {
      FieldEvent_Wait(5);
      object->z += PIXELS(1);
      BYTE_AT(object, 98) = 1;
      Flag_Set(0xa4f);
    }
  }
}

void Func_02000930(void) { GameFlag_Set(FLAG_RETREAT_BLOCKED); }

DEFINE_SCENE_LEAVE_BY_EXIT(Func_0200093c, Func_020046b4, Teleport_PlaySparks,
                           14, Func_02004718)

/// Sends object id from column 11 to x 152, then from column 10 to x 200,
/// both at z 792, through Actor_SetMoveTarget.
void Func_0200096c(s32 unused, s32 id) {
  struct Actor *object = Actor_Get(id);

  if (object->x >> 20 == 11) {
    s32 y;

    object->speedLimit = 0x4000;
    object->acceleration = 0x2000;
    y = object->y;
    Actor_SetMoveTarget(object, PIXELS(152), y, PIXELS(792));
  }
  if (object->x >> 20 == 10) {
    s32 y;

    object->speedLimit = 0x4000;
    object->acceleration = 0x2000;
    y = object->y;
    {
      /* FAKEMATCH(pin): the actor pinned to r0 to keep the argument order. */
      register struct Actor *self asm("r0") = object;
      s32 x = 0xc80000;
      /* FAKEMATCH(pin): z pinned to r3 to keep the argument order. */
      register s32 z asm("r3") = 0x3180000;

      Actor_SetMoveTarget(self, x, y, z);
    }
  }
}

void Func_020009c0(void) {
  FieldCamera_SetSpeed(0x10000, 0x2000);
  FieldCamera_MoveTo(PIXELS(88), -1, PIXELS(696), 1);
  Func_020019c4();
}

void Func_020009e8(void) {
  SCENE_WORK->unk166 = 0;
  Flag_Clear(FLAG_MENU_OPEN);
  Flag_Clear(0x104);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(120);
  Event_End();
}

void Func_02000a24(void) {
  struct Actor *object = Actor_Get(12);

  if (!Flag_IsSet(0xa4d)) {
    Event_PlaySound(SE_573);
    Flag_Set(FLAG_SCENE_TEMP_32);
    object->x = S16_AT(object, 100) << 16;
    object->z = S16_AT(object, 102) << 16;
    Flag_Set(0xa4c);
  }
}

void Func_02000a74(void) { Func_020019e0(); }

void Func_02000a7c(void) {
  SCENE_WORK->unk166 = 0;
  Flag_Clear(FLAG_MENU_OPEN);
  Flag_Clear(0x104);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_SetSpeed(0x10000, 0x2000);
  Camera_SetBounds(PIXELS(872), -1, PIXELS(168), 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(40);
  FieldCamera_MoveTo(PIXELS(872), -1, PIXELS(264), 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(120);
  Event_End();
}

DEFINE_SCENE_LEAVE_BY_EXIT(Func_02000af4, Func_020046b4, Teleport_PlaySparks,
                           14, Func_02004718)

void Func_02000b24(void) {
  MapGrid_CopyCells(53, 30, 4, 3, 53, 48);
  MapGrid_CopyCellsLayered(117, 30, 4, 3, 117, 48);
}

void Func_02000b54(void) {
  MapGrid_CopyCells(53, 40, 4, 3, 53, 48);
  MapGrid_CopyCellsLayered(117, 45, 4, 3, 117, 48);
}

DEFINE_FUNC_0200085C(Func_02000b84, 12, 95, 6, 101, 76, 40, 5, 12, 0xa4d)

void Func_02000c1c(void) {
  MapGrid_CopyCellAttributes(35, 11, 3, 6, 53, 11);
  MapGrid_CopyCellsLayered(35, 75, 3, 6, 117, 11);
  MapGrid_CopyCellsLayered(35, 75, 3, 6, 53, 75);
  Flag_Set(0xa49);
}

void Func_02000c68(void) { MapGrid_CopyCellAttributes(25, 9, 11, 3, 26, 9); }

DEFINE_SCENE_LEAVE_BY_EXIT(Func_02000c84, Func_020046b4, Teleport_PlaySparks,
                           12, Func_02004718)

DEFINE_SCENE_LEAVE_BY_EXIT(Func_02000cb4, Func_020046b4, Teleport_PlaySparks,
                           13, Func_02004718)

void Func_02000ce4(s32 unused, s32 id) {
  if (id == 8) {
    struct Actor *object = Actor_Get(8);

    if (object->x >> 20 == 5) {
      FieldEvent_Wait(5);
      object->z += PIXELS(1);
      Flag_Set(0xa4e);
    }
  }
}

void Func_02000d18(void) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s32 *camera = (s32 *)((u8 *)map + 228);
  s32 x = camera[0] & 0xffff0000;
  s32 cameraZ = camera[1] & 0xffff0000;
  s32 originY = map->origin[1];
  s32 tile = gResourceTableEntries[gData_020068a2].offset >> 5;
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  s32 y, z, depth, sx, sy;
  u32 *word;

  x = ((leader->x >> 20) << 20) - x;
  y = Map_GetTerrainHeight(0, leader->x, leader->z) - originY;
  z = ((leader->z >> 20) << 20) - cameraZ - originY;
  sy = z - y;
  sx = x >> 16;
  sy = (sy >> 16) + 16;
  depth = ((y + z) >> 16) + 58;
  if ((u32)(sx + 15) > 334 || sy < -15 || sy > 239)
    return;
  sx &= 0x1ff;
  sy &= 0xff;
  word = Data_020068a4;
  *word++ = 0;
  *word++ = sy | (sx << 16) | 0x80008800;
  *word = tile | 0x400;
  Oam_AddSpriteAtDepthClamped(Data_020068a4, depth);
}

/// Fills 0x100 bytes with colour 1, loads them into a VRAM block of their own
/// and starts Func_02000d18.
void Func_02000df0(void) {
  u32 *buffer = Heap_Alloc(0x100);
  u32 *word = buffer;
  s32 count = 63;

fill:
  *word++ = 0x11111111;
  if (--count >= 0)
    goto fill;
  gData_020068a2 = VramCache_FindFreeEntry();
  Vram_Load(gData_020068a2, 0x100, buffer);
  Heap_Release(buffer);
  Task_AddFrame(Func_02000d18, TASK_PASS_VBLANK);
}

#define SET_SPRITE_WINDOW_LAYERS()                                             \
  do {                                                                         \
    u16 winout = 0xf1f;                                                        \
                                                                               \
    REG_WINOUT = winout;                                                       \
  } while (0)

/// The leader shrinks and sinks over 40 frames, then the screen transition
/// ends.
void Func_02000e3c(s32 arg) {
  struct Actor *actor;
  struct MapWork *map;
  struct MapCell *cells;
  struct MapCell *buffer;
  s32 x, z;
  s32 i;

  actor = Actor_Get(gMainRoutineWork.currentOwner);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_SetSpriteFlags(Actor_Get(gMainRoutineWork.currentOwner), 0);
  actor->update = NULL;
  {
    s32 value = 0x100;

    FieldActor_ShowEmote(gMainRoutineWork.currentOwner, value, 0);
  }
  Actor_SelectAnimation(actor, 27);
  Actor_SetAnimationSpeed(actor, 48);
  System_Wait(30);
  actor->facing = FACING_SOUTH;
  actor->motionFlags = 0;
  buffer = gMapCellBuffer;
  x = actor->x >> 20;
  z = actor->z >> 20;
  map = Ram_HeapSlots->mapWork;
  if (map != NULL)
    cells = map->layers[actor->terrainId & 3].cells;
  else
    cells = buffer;
  cells += x + (z << 7) + 128;
  if (!(cells->attributeB & 0x40)) {
    SET_SPRITE_WINDOW_LAYERS();
    REG_DISPCNT |= DISPCNT_OBJWIN_ON;
    Func_02000df0();
  }
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 28);
  FieldEvent_Wait(16);
  i = 0;
next:
  if (i == 5)
    Event_PlaySound(SE_DUST_PUFF);
  actor->scaleX -= 0x400;
  actor->scaleY -= 0x300;
  actor->y -= 0x9999;
  System_Wait(1);
  i++;
  if (i <= 39)
    goto next;
  Actor_Get(gMainRoutineWork.currentOwner)->animationKind = 0;
  ((struct Overlay6aaEventWork *)Ram_HeapSlots->eventWork)->screenEffect =
      0x209;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(arg);
}

extern s32 gData_0200503c[];
extern struct Trigger gMarsLighthouseEntrance_Triggers[];
extern struct Trigger gMarsLighthouseEntrance_Room1Triggers[];
extern struct Trigger gMarsLighthouseEntrance_Room2Triggers[];
extern struct Trigger gMarsLighthouseEntrance_Room3Triggers[];
extern struct Trigger gMarsLighthouseEntrance_Room4Triggers[];
extern struct Trigger gMarsLighthouseEntrance_Room5Triggers[];
extern struct Trigger gMarsLighthouseEntrance_Room6Triggers[];
extern struct Trigger gMarsLighthouseEntrance_Room7Triggers[];

/* VIEW(mars_lighthouse_entrance): routine Func_02002c2c as void
 * Func_02002c2c(void); scene/mars_lighthouse_entrance/s263c has void
 * Func_02002c2c(struct Actor *object) {; tried as the definition's
 * DEFINE_EMBERS_SPAWN(Embers_Spawn, gData_02005714) void Func_02002c2c(struct
 * Actor *object), which the object already declares, it does not compile in
 * tla-ja: games/tla/src/scene/mars_lighthouse_entrance.c:1641: conflicting
 * types for `Func_02002c2c'. */
void Func_02002c2c__mars_lighthouse_entrance(void) __asm__("Func_02002c2c");
#define Func_02002c2c Func_02002c2c__mars_lighthouse_entrance

void Func_02000fa4(void) {
  Event_PlaySound(SE_584);
  Flag_Set(0x17d);
  Actor_SetScript(11, gData_0200503c);
  Func_0200246c(0, 3);
  Flag_Set(FLAG_SCENE_TEMP_34);
}

void Func_02000fd4(void) {
  Leader_SlideOnIceFromEdge();
  Func_020029a0();
}

void GameFlag_SetByte(s32 flag, s32 value);

void Func_02000fe0(const struct MapCell *cell) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  s32 z = leader->z;
  s32 i = leader->x >> 20;

  GameFlag_SetByte(0x340, i);
  GameFlag_SetByte(0x348, z >> 20);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_SetSpriteFlags(Actor_Get(gMainRoutineWork.currentOwner), 0);
  leader->update = 0;
  FieldActor_ShowEmote(gMainRoutineWork.currentOwner, 0x100, 0);
  Actor_SelectAnimation(leader, 27);
  Actor_SetAnimationSpeed(leader, 48);
  System_Wait(30);
  leader->facing = FACING_SOUTH;
  leader->motionFlags = 0;
  if ((cell->attributeB & 64) == 0) {
    {
      u16 winout = 0xf1f;

      REG_WINOUT = winout;
    }
    REG_DISPCNT |= 0x8000;
    Func_02000df0();
  }
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 28);
  FieldEvent_Wait(16);
  i = 0;
next:
  if (i == 5)
    Event_PlaySound(SE_DUST_PUFF);
  leader->scaleX -= 0x400;
  leader->scaleY -= 0x300;
  leader->y -= 0x9999;
  System_Wait(1);
  i++;
  if (i <= 39)
    goto next;
  Actor_Get(gMainRoutineWork.currentOwner)->animationKind = 0;
  SCENE_WORK->unk1ac = 0x209;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(11);
}

void Func_0200112c(void) {
  Field_TryJumpForward();
  Leader_SlideOnIceAhead();
}

void Func_02001138(void) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct MainRoutineWork *party = &gMainRoutineWork;
  struct Actor *object;
  s32 x;
  s32 z;
  struct MapCell *cell;

  object = Actor_Get(party->currentOwner);
  x = object->x >> 20;
  z = object->z >> 20;
  cell = gMapCellBuffer;
  if (map != NULL)
    cell = map->layers[object->terrainId & 3].cells;
  cell += x + (z << 7);
  if (cell->attributeB & 64)
    Func_02000fe0(cell + 128);
}

const struct Trigger *Scene_GetEvents(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6aaRoom1)
    return gMarsLighthouseEntrance_Room1Triggers;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6aaRoom2)
    return gMarsLighthouseEntrance_Room2Triggers;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6aaRoom3)
    return gMarsLighthouseEntrance_Room3Triggers;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6aaRoom4)
    return gMarsLighthouseEntrance_Room4Triggers;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6aaRoom5)
    return gMarsLighthouseEntrance_Room5Triggers;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6aaRoom6)
    return gMarsLighthouseEntrance_Room6Triggers;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6aaRoom7)
    return gMarsLighthouseEntrance_Room7Triggers;
  return gMarsLighthouseEntrance_Triggers;
}

DEFINE_ACTOR_FREEZE(Actor_Freeze)
#undef SET_SPRITE_WINDOW_LAYERS

extern u32 gData_02006868[];

void Func_02001254(void) {
  s16 *state = &gData_02006868;

  if (gData_020068a0 < 60) {
    if (*state == 0) {
      Func_0200246c(0, 3);
      *state = 1;
    }
  } else {
    if (((struct HalfwordField *)state)->value == 1) {
      Func_0200246c(0, 67);
      *state = 0;
    }
  }
  if (++gData_020068a0 > 300)
    gData_020068a0 = 0;
}

DEFINE_ACTOR_BLINK(Func_020012bc, 0, 6, 10)

void Func_020012e4(struct Actor *actor) {
  struct Sprite020012e4 *sprite = actor->sprite;

  sprite->unk12 += 0x800;
  if (++actor->timer == 8)
    actor->velocityY = 0x40000;
  if (actor->timer > 9) {
    actor->x += 0x40000;
    actor->scaleX -= 0x800;
    actor->scaleY -= 0x800;
  }
  if (actor->timer == 32) {
    actor->x = 0;
    actor->z = 0;
    actor->update = NULL;
  }
}

extern struct Trigger gMarsLighthouseEntrance_Triggers[];
extern struct Trigger gMarsLighthouseEntrance_Room1Triggers[];
extern struct Trigger gMarsLighthouseEntrance_Room2Triggers[];
extern struct Trigger gMarsLighthouseEntrance_Room3Triggers[];
extern struct Trigger gMarsLighthouseEntrance_Room4Triggers[];
extern struct Trigger gMarsLighthouseEntrance_Room5Triggers[];
extern struct Trigger gMarsLighthouseEntrance_Room6Triggers[];
extern struct Trigger gMarsLighthouseEntrance_Room7Triggers[];

void Func_02002c2c(void);

void Func_02001344(struct Actor *object) {
  if (ABS(object->x - object->watchedObject->x) > 0x200000)
    return;
  object->timer = 0;
  object->update = (void (*)(void))Func_020012e4;
}

void Func_02001378(void) {
  if (Func_020029bc())
    Leader_SetSlideStopped(1);
}

void Beam_Start(u16 *targets, u16 *stops, s32 a2, s32 a3);
void Func_0200248c(s32 index, s16 value);
void Palette_SetColorsFade(u16 *slots, u16 *colours, u16 *a, u16 *b);
void Func_0200196c(void);
extern u32 gData_02005314[];
extern s16 gData_020053d8[];
extern u16 gData_020053e6[];
extern u16 gData_020053f8[];
extern s16 gData_020053fe[];
extern u16 gData_0200540c[];
extern u16 gData_02005412[];
extern s16 gData_02005418[];
extern u16 gData_02005438[];
extern u16 gData_0200543e[];
extern u16 gData_02005448[];
extern u16 gData_0200544e[];
extern s16 gData_02005454[];
extern s16 gData_0200586c[];
extern s16 gData_0200587c[];
extern s16 gData_020058a8[];
extern u32 gData_020058d4[];

s32 Scene_Initialize(void) {
  s32 scene = gMainRoutineWork.scene;
  s32 entrance = gMainRoutineWork.entrance;
  struct Actor *object;
  s32 i;

  Data_02006878 = 0;
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x204;
  Event_SetFlag(FLAG_MARS_LIGHTHOUSE_LIT);
  Func_02002270();
  if (scene == (s32)&SceneId_Ov6aaRoom1) {
    Call1((void (*)())Weather_StartRain, 1);
  } else if (scene == (s32)&SceneId_Ov6aaRoom2) {
    Call1((void (*)())FieldEffect_StartBlankOverlay, 0);
#if !TLA_PATCH_1
    FieldActor_SetSpritePriority(11, 3);
#endif
    Call1((void (*)())Actor_Freeze, 10);
    if (Event_GetFlag(0xa30)) {
      MapGrid_CopyCellsLayered(35, 41, 24, 10, 3, 53);
      MapGrid_CopyCellsLayered(35, 105, 24, 10, 3, 117);
    }
    Call1((void (*)())MarsLighthouseEntrance_PlaceIceCones,
          (s32)gData_020053d8);
    if (Event_GetFlag(0xa3e))
      MapGrid_CopyCellsLayered(50, 125, 3, 3, 39, 85);
  } else if (scene == (s32)&SceneId_Ov6aaRoom3) {
    Call1((void (*)())FieldEffect_StartBlankOverlay, 0);
    Call4((void (*)())Beam_Start, (s32)gData_020053e6, (s32)gData_020053f8, 0,
          88);
    Call1((void (*)())MarsLighthouseEntrance_PlaceIceCones,
          (s32)gData_020053fe);
    if (Event_GetFlag(0xa3f)) {
      MapGrid_CopyCellsLayered(47, 94, 10, 6, 47, 100);
      MapGrid_CopyCellAttributes(110, 40, 12, 4, 46, 40);
      Actor_SetPosition(18, 0, 0);
    } else if (Event_GetFlag(0xa3e)) {
      Event_SetFlag(FLAG_SCENE_TEMP_30);
      Actor_Get(18)->priorityFlags |= 2;
    } else {
      Actor_SetPosition(18, 0, 0);
    }
    Actor_Get(20)->priorityFlags |= 2;
    FieldActor_SetSpritePriority(20, 2);
    if (Event_GetFlag(0xa47)) {
      MapGrid_CopyCellsLayered(28, 92, 8, 8, 28, 100);
      MapGrid_CopyCellAttributes(92, 38, 8, 8, 28, 38);
      Func_02000798();
      Actor_SetPosition(20, 0, 0);
    } else if (Event_GetFlag(0xa48)) {
      Func_02000798();
      Event_SetFlag(FLAG_SCENE_TEMP_31);
    } else {
      Actor_SetPosition(20, 0, 0);
    }
    if (Event_GetFlag(0xa4f)) {
      Actor_SetPosition(8, 0x1480000, 0x780000);
      ((u8 *)Actor_Get(8))[98] = 1;
    }
    if (Event_GetFlag(0xa30))
      Actor_SetPosition(19, 0, 0);
    else
      Actor_SetPosition(15, 0, 0);
    if (entrance == 20) {
      Event_Start();
      FieldEvent_ResolvePendingActions(0);
      FieldCamera_MoveTo(-1, -1, -1, 0);
      MapGrid_SetShake(0, 0x20000, 0x10000);
      Actor_SetPosition(gMainRoutineWork.currentOwner, 0, 0);
      Actor_SetPosition(18, 0, 0);
      Actor_Get(18)->priorityFlags |= 2;
      Event_StartScreenTransition();
      Event_WaitTransitionFrames();
      object = Actor_Get(18);
      Actor_SetPosition(18, object->timer << 16, object->unk66 << 16);
      Event_PlaySound(SE_573);
      i = 2;
      do {
        Call1((void (*)())Func_02000414, 18);
        System_Wait(1);
        i--;
      } while (i >= 0);
      Event_SetFlag(0xa3e);
      MapGrid_SetShake(-1, -1, 0xe666);
      FieldEvent_Wait(120);
      FieldEvent_SetExitRequest(20);
      Event_SetFlag(FLAG_KEEP_PARTY_POSITION);
    }
  } else if (scene == (s32)&SceneId_Ov6aaRoom4) {
    Call1((void (*)())FieldEffect_StartBlankOverlay, 0);
    if (entrance == 10 || entrance == 1) {
      Call4((void (*)())Beam_Start, (s32)gData_0200540c, (s32)gData_02005412,
            (s32)Func_020019a4, 88);
      Call2((void (*)())Func_0200248c, 0, 1);
    }
    Call1((void (*)())MarsLighthouseEntrance_PlaceIceCones,
          (s32)gData_02005418);
  } else if (scene == (s32)&SceneId_Ov6aaRoom5) {
    Call1((void (*)())FieldEffect_StartBlankOverlay, 0);
    Actor_Get(12)->priorityFlags |= 2;
    if (Event_GetFlag(0xa4d)) {
      MapGrid_CopyCellsLayered(12, 95, 8, 6, 12, 101);
      MapGrid_CopyCellAttributes(76, 40, 8, 5, 12, 40);
      Actor_SetPosition(12, 0, 0);
    } else if (Event_GetFlag(0xa4c)) {
      Event_SetFlag(FLAG_SCENE_TEMP_32);
    } else {
      Actor_SetPosition(12, 0, 0);
    }
    if (Event_GetFlag(0xa49))
      Func_02000c1c();
  } else if (scene == (s32)&SceneId_Ov6aaRoom6) {
    Call1((void (*)())FieldEffect_StartBlankOverlay, 0);
    if (entrance == 2 || entrance == 4 || entrance == 6 || entrance == 13) {
      Call4((void (*)())Beam_Start, (s32)gData_02005438, (s32)gData_0200543e,
            (s32)Func_0200196c, 88);
      Call2((void (*)())Func_0200248c, 0, 1);
    }
    if ((u32)(entrance - 7) <= 1 || entrance == 11) {
      struct Actor *target;

      Call4((void (*)())Beam_Start, (s32)gData_02005448, (s32)gData_0200544e,
            (s32)Func_02001988, 88);
      Call2((void (*)())Func_0200248c, 0, 1);
      if (entrance == 11) {
        Actor_SetScript(11, gData_02005314);
        Func_0200246c(0, 3);
        Task_AddFrame(Func_02001254, 0x480);
        Event_SetFlag(0x17d);
      }
      Task_AddFrame(Func_02001378, 0x480);
      Actor_Get(11)->update = (void (*)(void))Func_020012bc;
      Func_020022d8(1);
      Call1((void (*)())MarsLighthouseEntrance_PlaceIceCones,
            (s32)gData_02005454);
      target = Actor_Get(20);
      Actor_Get(13)->update = (void (*)(void))Func_02001344;
      Actor_Get(14)->update = (void (*)(void))Func_02001344;
      Actor_Get(15)->update = (void (*)(void))Func_02001344;
      Actor_Get(16)->update = (void (*)(void))Func_02001344;
      Actor_Get(17)->update = (void (*)(void))Func_02001344;
      Actor_Get(18)->update = (void (*)(void))Func_02001344;
      Actor_Get(13)->watchedObject = target;
      Actor_Get(14)->watchedObject = target;
      Actor_Get(15)->watchedObject = target;
      Actor_Get(16)->watchedObject = target;
      Actor_Get(17)->watchedObject = target;
      Actor_Get(18)->watchedObject = target;
    }
    Actor_Get(9)->priorityFlags |= 2;
    if (Event_GetFlag(0xa4e))
      Actor_SetPosition(8, 0x580000, 0x2b80000);
  } else if (scene == (s32)&SceneId_Ov6aaRoom7) {
    Call1((void (*)())FieldEffect_StartBlankOverlay, 0);
  }
  if (scene == (s32)&SceneId_Ov6aaRoom6 && entrance == 13)
    Event_ClearFlag(0x104);
  Call4((void (*)())Palette_SetColorsFade, (s32)gData_0200586c,
        (s32)gData_0200587c, (s32)gData_020058a8, (s32)gData_020058d4);
  return 0;
}

extern s32 gData_0200547c[];
extern s32 gData_020054cc[];
extern s32 gData_02005528[];
extern s32 gData_02005578[];
extern s32 gData_020055d4[];
extern s32 gData_02005644[];
extern struct Trigger gMarsLighthouseEntrance_Triggers[];
extern struct Trigger gMarsLighthouseEntrance_Room1Triggers[];
extern struct Trigger gMarsLighthouseEntrance_Room2Triggers[];
extern struct Trigger gMarsLighthouseEntrance_Room3Triggers[];
extern struct Trigger gMarsLighthouseEntrance_Room4Triggers[];
extern struct Trigger gMarsLighthouseEntrance_Room5Triggers[];
extern struct Trigger gMarsLighthouseEntrance_Room6Triggers[];
extern struct Trigger gMarsLighthouseEntrance_Room7Triggers[];

/* VIEW(s1930): routine Actor_Attach as s32 Actor_Attach(s32 id, s32 a1, s32 a2,
 * s32 a3, const void *script); s1344 has void Actor_Attach(s32 id, s32 a1, s32
 * a2, s32 a3, const void *script); not tried: Actor_Attach's bound declaration
 * was not found. */
extern s32 Func_02002f4c__s1930(s32 id, s32 a1, s32 a2, s32 a3,
                                const void *script) __asm__("Actor_Attach");
void Func_02002c2c(void);

s32 Scene_PrepareMap(void) {
  if (Flag_IsSet(0xa30))
    TileMap_ConvertBuffer(gMainRoutineWork.map + ((s32)&SceneId_Ov6ACRoom3 -
                                                  (s32)&SceneId_Ov6a8Room2),
                          0x5000000);
  return 0;
}

void Func_0200196c(void) {
  Func_02002f4c__s1930(10, 19, 20, 0, gData_0200547c);
}

void Func_02001988(void) {
  Func_02002f4c__s1930(12, 19, 20, 0, gData_020054cc);
}

void Func_020019a4(void) {
  Func_02002f4c__s1930(9, 18, 20, 0x4000, gData_02005528);
}

void Func_020019c4(void) { Func_02002f4c__s1930(9, 13, 14, 0, gData_02005578); }

void Func_020019e0(void) {
  Func_02002f4c__s1930(11, 13, 14, 0x4000, gData_020055d4);
}

void Func_02001a00(void) {
  Func_02002f4c__s1930(13, 21, 22, 0, gData_02005644);
}

void Ride_UpdateDrift(struct Actor *object);

DEFINE_ACTOR_IS_TOUCHING(Actor_IsTouching)

DEFINE_ACTOR_IS_BESIDE(Actor_IsBeside)

/* VIEW(s1930): tag struct RideSlot as struct RideSlot__s1930; ride.h has
 * another definition; tried with the definition the rest of the object has, it
 * does not compile in tla-ja:
 * games/tla/src/scene/mars_lighthouse_entrance.c:1188: structure has no member
 * named `id'. */
struct RideSlot__s1930 {
  u16 id;
  u16 blocked;
};

/* VIEW(s1930): tag struct RideWork as struct RideWork__s1930; ride.h has
 * another definition; tried with the definition the rest of the object has, it
 * does not compile in tla-ja:
 * games/tla/src/scene/mars_lighthouse_entrance.c:1179: structure has no member
 * named `value68'. */
struct RideWork__s1930 {
  u8 unk00[4];
  u16 landed;
  u16 blocked;
  u8 unk08[2];
  u16 value0a;
  u8 unk0c[0x48];
  struct RideSlot__s1930 slots[4];
  u8 unk64[4];
  u16 value68;
  u16 direction;
};

DEFINE_RIDE_UPDATE_CARRIED(Ride_UpdateCarried, RideWork__s1930,
                           Actor_IsTouching, Actor_IsBeside)

extern u32 gData_020056a0[];

void Ride_UpdateDrift(struct Actor *object);

DEFINE_ACTOR_RESET_SCRIPTED_FOR(Actor_ResetScriptedFor, gData_020056a0)

DEFINE_RIDE_SPAWN_FRONT(Func_02001c74, Func_02004bd8_, Actor_ResetScriptedFor,
                        2, Ride_UpdateCarried, Actor_SelectAnimation)

/* VIEW(s1c44): tag struct RideSlot as struct RideSlot__s1c44; ride.h has
 * another definition; tried with the definition the rest of the object has, it
 * does not compile in tla-ja:
 * games/tla/src/scene/mars_lighthouse_entrance.c:1224: structure has no member
 * named `id'. */
struct RideSlot__s1c44 {
  u16 id;
  u16 blocked;
};

struct RideDirection {
  u16 unk00;
  u16 unk02;
};

/* VIEW(s1c44): tag struct RideWork as struct RideWork__s1c44; ride.h has
 * another definition; tried with the definition the rest of the object has, it
 * does not compile in tla-ja:
 * games/tla/src/scene/mars_lighthouse_entrance.c:1216: structure has no member
 * named `direction'. */
struct RideWork__s1c44 {
  u8 unk00[4];
  u16 landed;
  u16 blocked;
  u8 unk08[2];
  u16 value0a;
  u8 unk0c[0x48];
  struct RideSlot__s1c44 slots[4];
  u8 unk64[4];
  struct RideDirection direction;
};

DEFINE_RIDE_UPDATE_LANDING(Ride_UpdateLanding, RideWork__s1c44,
                           Actor_IsTouching, Actor_IsBeside)

extern u32 gData_020056a0[];

void Ride_UpdateDrift(struct Actor *object);

/// Stops the object and sets its sprite part, then restarts its script at
/// gData_020056a0. Overlays 6ab, 6ac and 6ad have the same function.
DEFINE_ACTOR_SETUP_SCRIPTED_PART(Actor_SetupScriptedPart, gData_020056a0)

DEFINE_RIDE_SPAWN_BACK(Func_02001e60, Func_02004bd8_, Actor_SetupScriptedPart,
                       Ride_UpdateCarried, Actor_SelectAnimation)

DEFINE_RIDE_SPAWN_FRONT(Func_02001ea8, Func_02004bd8_, Actor_SetupScriptedPart,
                        3, Ride_UpdateLanding, Actor_SelectAnimation)

DEFINE_RIDE_SPAWN_BACK(Func_02001ef0, Func_02004bd8_, Actor_SetupScriptedPart,
                       Ride_UpdateLanding, Actor_SelectAnimation)

DEFINE_ACTOR_IS_ABOVE(Actor_IsAbove)

DEFINE_RIDE_UPDATE_DRIFT(Ride_UpdateDrift, Actor_IsAbove)

DEFINE_ACTOR_SETUP_SCRIPTED_FOR(Actor_SetupScriptedFor, Actor_SelectAnimation,
                                gData_020056a0)

DEFINE_RIDE_SPAWN_SPARK(Ride_SpawnSpark, Func_02004bd8_, Actor_SetupScriptedFor,
                        Ride_UpdateDrift)

/* VIEW(s20ec): tag struct Actor as struct Actor__s20ec; actor.h has another
 * definition; tried with the definition the rest of the object has, it does not
 * compile in tla-ja: games/tla/src/scene/mars_lighthouse_entrance.c:1304:
 * structure has no member named `flag5b'. */
typedef struct Actor__s20ec {
  u8 pad0[8];
  s32 x;
  s32 y;
  s32 z;
  u8 pad14[91 - 20];
  u8 flag5b;
} Actor;

typedef struct Entry {
  s16 active;
  s16 timer;
  u16 flags;
  s16 unk_6;
  Actor *actor;
  s32 unk_c;
} Entry;

typedef struct State {
  u8 pad0[6];
  s16 unk_6;
  s16 unk_8;
  u8 pada[4];
  s16 unk_e;
  void (*callback)();
  Entry entries[4];
} State;

/* VIEW(s20ec): routine Ride_SpawnSpark as void Ride_SpawnSpark(); the parts
 * before it has another declaration; not tried: Ride_SpawnSpark's bound
 * declaration was not found. */
extern void Func_0200206c__s20ec() __asm__("Ride_SpawnSpark");

typedef struct Position {
  s32 x;
  s32 z;
} Position;

typedef struct Camera {
  u8 pad0[228];
  Position position;
} Camera;

typedef struct IwramGlobals {
  u8 pad0[32];
  Camera *camera;
  u8 pad24[108 - 36];
  union {
    s32 word;
    void *pointer;
  } unk_6c;
} IwramGlobals;

#define IWRAM ((IwramGlobals *)Ram_HeapSlots)

DEFINE_BEAM_DRAW_TARGETS(Beam_DrawTargets, Func_0200206c__s20ec, Func_02001e60,
                         Func_02001ef0, Func_02001ea8, Func_02001c74)
#undef IWRAM

#undef Oam_AddSpriteAtDepthClamped
#undef Func_02002c2c
void Func_02002270(void) {
  struct MarsLighthouseEntranceRideHead *work =
      (struct MarsLighthouseEntranceRideHead *)(gInventorySnapshot + 0x188);
  struct Actor *object;
  struct MarsLighthouseEntranceAnimation *animation;

  if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION))
    Iwram_ClearWordsEntry(work, 116);
  object = (struct Actor *)FieldActor_Create(365, 1, 0, 0);
  object->unk5c[0] = 2;
  Actor_SelectAnimation(object, 1);
  animation = object->sprite;
  work->handle = object;
  work->value0c = animation->value10;
  animation->flag11 = 1;
}

void Func_020022c4(void) {
  struct RideWork *work = RIDE_WORK;

  Actor_Release(work->handle);
  work->handle = NULL;
}

void Func_020022d8(s16 value) { RIDE_WORK->value0e = value; }

void Beam_DrawTargets(void);

DEFINE_BEAM_START(Beam_Start, Beam_DrawTargets)

s32 Func_02002418(s32 index) {
  return ((struct RideRow *)(gInventorySnapshot + 0x188 + index * 16))->a;
}

void Func_02002428(s32 index, s16 value) {
  ((struct RideRow *)(gInventorySnapshot + 0x188 + index * 16))->a = value;
}

s32 Func_02002438(void) { return RIDE_WORK->counter; }

s32 Func_02002444(s32 limit) {
  struct RideWork *work = RIDE_WORK;

  if (!Event_GetFlag(260)) {
    if (++work->counter >= limit)
      work->counter = 0;
  }
  return work->counter;
}

/* VIEW(s2270): routine Func_0200246c as void Func_0200246c(s32 index, s32
 * value); scene/shared/mars_lighthouse_entrance_room.h has s32
 * Func_0200246c(s32 index, s32 value); tried as the definition's void
 * Func_0200246c(s32 index, s32 value), which the object already declares, it
 * does not compile in tla-ja:
 * games/tla/src/scene/mars_lighthouse_entrance.c:1367: conflicting types for
 * `Func_0200246c'. */
void Func_0200246c__s2270(s32 index, s32 value) __asm__("Func_0200246c");
#define Func_0200246c Func_0200246c__s2270
void Func_0200246c(s32 index, s32 value) {
  struct RideWork *work = RIDE_WORK;
  struct RideEntry *entry = &work->entries[index];

  entry->d = (value & 15) << 16;
  entry->b = value & 0xf0;
}

void Func_0200248c(s32 index, s16 value) {
  RIDE_WORK->entries[index].c = value;
}

s32 Func_0200249c(s32 index) {
  return ((struct RideRow *)(gInventorySnapshot + 0x188 + index * 16))->b;
}

void Func_020024ac(u32 index, s32 value) {
  struct RideWork *work = RIDE_WORK;

  if (index <= 3)
    work->slots[index].value = value;
}

struct MarsLighthouseEventWork {
  u8 unk000[0x16a];
  u16 unk16a;
  u16 downCount;
  u16 standingCount;
  u16 downOwners[8];
};

/* VIEW(s2270): routine Unit_Get as struct BattleUnit *Unit_Get(s32 owner);
 * item_icon.h has void *Unit_Get(s32 owner); tried as the definition's void
 * *Unit_Get(s32 owner), which the object already declares, it does not compile
 * in tla-ja: games/tla/src/scene/mars_lighthouse_entrance.c:1402: request for
 * member `hp' in something not a structure or union. */
struct BattleUnit *Unit_Get__s2270(s32 owner) __asm__("Unit_Get");
#define Unit_Get Unit_Get__s2270

DEFINE_PARTY_TAKE_FALL_DAMAGE(Party_TakeFallDamage)

#include "../field/field_effect/advance_spin.c"

DEFINE_ACTOR_BLINK(Func_02002614, 10, 4, 0)

#undef Unit_Get
#undef Func_0200246c
void Party_TakeFallDamage();
void Actor_SelectAnimation(struct Actor *actor, s32 a);
s32 MapGrid_CheckCollision(struct Actor *actor, struct Vector3 *position);
void Actor_SetSpriteColorMode(struct Actor *actor, s32 a);
/* VIEW(s263c): routine Func_02002614 as void Func_02002614(void);
 * scene/mars_lighthouse_entrance/s2270 has DEFINE_ACTOR_BLINK(Func_02002614,
 * 10, 4, 0) #undef Beam_Start #undef Unit_Get // ----
 * scene/mars_lighthouse_entrance/s263c.c ---- /// Scene code of overlay 0x6aa,
 * in ROM order. #include "shared/mars_lighthouse.h" #include "version.h"
 * #include "types.h" #include "call.h" #include "actor.h" #include
 * "main_routine_work.h" #include "ram_buffer.h" #include "vector3.h" #include
 * "fixed_math.h" #include "event_inline.h" #include "display_transition.h"
 * #include "event_work.h" #include "field.h" #include "item_icon.h" #include
 * "module/field/field.h" #include "module/scene/mars_lighthouse_entrance.h"
 * #include "module/system/system.h" #include "scene_ids.h" void
 * Party_TakeFallDamage(); tried as the rest of the object declares it, it does
 * not compile in tla-ja: games/tla/src/scene/mars_lighthouse_entrance.c:1442:
 * conflicting types for `Func_02002614'. */
void Func_02002614__s263c(void) __asm__("Func_02002614");
#define Func_02002614 Func_02002614__s263c
extern u8 gInventorySnapshot[];

#define KNOCK_BACK Leader_KnockBack
#define KNOCK_BACK_TAKE_DAMAGE Party_TakeFallDamage
#define KNOCK_BACK_BLINK Func_02002614
#define KNOCK_BACK_SET_ANIMATION Actor_SelectAnimation
#define KNOCK_BACK_CELL_ATTRIBUTE MapGrid_GetCellAttributeLowNibbleOrBump
#define KNOCK_BACK_CHECK_COLLISION MapGrid_CheckCollision
#define KNOCK_BACK_SPHERES_OVERLAP Field_CheckSpheresOverlap
#include "shared/leader_knock_back.c"
#undef KNOCK_BACK
#undef KNOCK_BACK_TAKE_DAMAGE
#undef KNOCK_BACK_BLINK
#undef KNOCK_BACK_SET_ANIMATION
#undef KNOCK_BACK_CELL_ATTRIBUTE
#undef KNOCK_BACK_CHECK_COLLISION
#undef KNOCK_BACK_SPHERES_OVERLAP

void MarsLighthouseEntrance_ContinueSlide(void) {
  if (RIDE_WORK->active)
    Leader_KnockBack();
}

/* VIEW(s2860): routine Party_TakeFallDamage as s32 Party_TakeFallDamage(s32 a,
 * s32 b); s263c has void Party_TakeFallDamage(); not tried:
 * Party_TakeFallDamage's bound declaration was not found. */
extern s32 Func_020024c0__s2860(s32 a, s32 b) __asm__("Party_TakeFallDamage");
void Func_02002614(void);

struct RideEvent {
  u8 unk000[0x15a];
  u16 field15a;
  u16 field15c;
  u16 field15e;
  u8 unk160[4];
  u16 field164;
  u16 field166;
  u8 unk168[0x44];
  s32 field1ac;
  u8 unk1b0[2];
  u16 started;
  s32 field1b4;
};

struct RideCar {
  u8 unk00[0x64];
  s16 stopped;
  u8 unk66[0xa];
  s32 stepX;
  s32 unk74;
  s32 stepZ;
};

struct RideState {
  s32 counter;
  s16 active;
  u8 unk06[0x66];
  struct RideCar *car;
};

/// Stepping onto ice (cell behavior 90, as Imil's ice slide is in The Broken
/// Seal): the leader slides until the ice stops.
int MarsLighthouseEntrance_SlideOnIce(void) {
  struct RideEvent *event = Ram_HeapSlots->eventWork;
  struct RideState *ride;
  struct Actor *leader;
  // Unused: gives the stack frame its 12 bytes.
  s32 frame[3];

  /* FAKEMATCH(signature): typed as returning int, though it returns nothing. */
  if (event->started == 0) {
    ScreenTransition_Start(event->field1ac, event->field1b4);
    event->started = 1;
  }
  Flag_Clear(FLAG_MENU_OPEN);
  event->field166 = 0;
  event->field15a = 0;
  event->field15e = 0;
  event->field15c = 0;
  event->field164 = 0;
  /* FAKEMATCH(signature): Party_TakeFallDamage declared returning s32, though
   * it returns nothing. */
  Func_020024c0__s2860(-10, 0);
  FieldCamera_SetSpeed(0x70000, 0xe000);
  leader = Actor_Get(gMainRoutineWork.currentOwner);
  ride = (struct RideState *)(gInventorySnapshot + 0x188);
  Flag_Set(FLAG_MENU_OPEN);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Flag_Clear(FLAG_MENU_OPEN);
  leader->update = Func_02002614;
  leader->facing = 0x4000;
  Actor_SelectAnimation(leader, 49);
  while (ride->car->stopped == 0) {
    if (MapGrid_GetCellAttributeLowNibbleOrBump(0, leader->x, leader->z) != 7) {
      leader->x += ride->car->stepX;
      leader->z += ride->car->stepZ;
    } else {
      if (ride->car->stepX != 0)
        leader->x = (leader->x & 0xfff00000) + 0x80000;
      if (ride->car->stepZ != 0)
        leader->z = (leader->z & 0xfff00000) + 0x80000;
    }
    ride->active = 0;
    System_Wait(1);
  }
  FieldEvent_Wait(30);
  leader->update = 0;
  Actor_SetSpriteColorMode(leader, 0);
  ride->active = 0;
}

s32 Func_020029a0(void) {
  if (RIDE_WORK->active) {
    MarsLighthouseEntrance_SlideOnIce();
    return 1;
  }
  return 0;
}

s32 Func_020029bc(void) { return RIDE_WORK->active; }

struct Sprite {
  u8 unk00[4];
  u16 y : 8;
  u16 affine : 2;
  u16 blendMode : 2;
  u16 mosaic : 1;
  u16 fullColor : 1;
  u16 shape : 2;
  u16 x : 9;
  u16 affineIndex : 5;
  u16 size : 2;
  u16 tile : 10;
  u16 attr2Rest : 6;
  u8 unk0a[2];
  s32 scale; ///< 16.16, as FieldSprite (field_sprite.h) has it
  u8 vramBlock;
  u8 keepEntry : 1;
  u8 unk11 : 7;
  u16 rotation;
  u8 unk14[2];
  u8 unk16[5];
  u8 unk1b;
  u8 unk1c[12];
  void *record;
};

struct Work {
  u8 unk00[12];
  u16 vramBlock;
  u8 unk0e[98];
  struct Actor *owner;
};

extern u32 gData_020056d0[];

#define WORK ((struct Work *)(gInventorySnapshot + 0x188))

void Func_020029c8(struct Actor *actor) {
  struct Work *work = WORK;
  s32 i;

  if (work->owner == NULL)
    return;
  for (i = 0; i >= 0; i--) {
    struct Actor *object;
    s32 a, b, c, d, e;
    s32 x, y, z;

    a = Random16();
    b = Random16();
    x = actor->x + a * 16 - b * 16;
    c = Random16();
    d = Random16();
    e = Random16();
    z = actor->z + d * 16 - e * 16;
    y = c * 8 + actor->y + 0x40000;
    object = (struct Actor *)Import_ActorCreate(0x1e9, x, y, z);
    if (object != NULL) {
      s32 angle, radius;
      u32 sum;
      s32 scale;
      struct Sprite *sprite;
      struct Sprite *source;

      angle = Random16();
      radius = Random16() * 8;
      sum = Random16() + Random16();
      scale = (sum >> 2) + 0x8000;
      Actor_StartScript(object, gData_020056d0);
      Actor_SetSpriteFlags(object, 0);
      object->velocityY = angle * 4 + 0x20000;
      object->velocityZ = Iwram_MulQ16(radius, Trig_Cos(angle));
      object->velocityX = Iwram_MulQ16(radius, Trig_Sin(angle));
      object->gravity = 0x547a;
      object->scaleX = scale;
      object->scaleY = scale;
      object->acceleration = 0;
      object->bounce = 0x8000;
      sprite = object->sprite;
      source = work->owner->sprite;
      VramCache_FreeEntry(sprite->vramBlock);
      sprite->keepEntry = 1;
      sprite->vramBlock = work->vramBlock;
      SpriteAnimation_Clear(sprite->record);
      sprite->record = NULL;
      sprite->unk1b = 0;
      sprite->unk14[0] = source->unk14[0];
      sprite->unk14[1] = source->unk14[1];
      sprite->shape = source->shape;
      sprite->size = source->size;
      sprite->tile = source->tile;
    }
  }
}

extern u32 gData_02005714[];

/// Throws two sparks (object kind 325) from (x, y, z), each upward at about
/// speed plus 0x14000 with a random spread, at a random angle and size.
DEFINE_EMBERS_SPAWN(Embers_Spawn, gData_02005714)

void Func_02002c2c(struct Actor *object) {
  object->movementState = Event_GetFlag(260);
}

#define ABS(x) ((x) < 0 ? -(x) : (x))

/* VIEW(s2860): tag struct Work020023c4 as struct Work020023c4__s2860; s263c.c
 * has another definition; tried with the definition the rest of the object has,
 * it does not compile in tla-ja:
 * games/tla/src/scene/mars_lighthouse_entrance.c:1661: structure has no member
 * named `unk6c'. */
struct Work020023c4__s2860 {
  u8 unk00[4];
  u16 unk04;
  u8 unk06[4];
  u16 unk0a;
  u8 unk0c[0x6c - 0x0c];
  struct Actor *unk6c;
};

struct WorkEvent {
  u8 unk000[340];
  u16 unk154;
  u8 unk156[358 - 342];
  s16 unk166;
};

DEFINE_ACTOR_ROLL_AND_CRUMBLE(Actor_RollAndCrumble, Work020023c4__s2860,
                              MapGrid_GetCellAttributeLowNibbleOrBump,
                              Embers_Spawn, Func_020029c8)
#undef WORK
#undef ABS

extern char SceneId_Ov6ACRoom3;
extern char SceneId_Ov6aaRoom1;
extern char SceneId_Ov6aaRoom2;
extern char SceneId_Ov6aaRoom3;
extern char SceneId_Ov6aaRoom4;
extern char SceneId_Ov6aaRoom5;
extern char SceneId_Ov6aaRoom6;
extern char SceneId_Ov6aaRoom7;
extern struct Trigger gMarsLighthouseEntrance_Room1Triggers[];
extern struct Trigger gMarsLighthouseEntrance_Room2Triggers[];
extern struct Trigger gMarsLighthouseEntrance_Room3Triggers[];
extern struct Trigger gMarsLighthouseEntrance_Room4Triggers[];
extern struct Trigger gMarsLighthouseEntrance_Room5Triggers[];
extern struct Trigger gMarsLighthouseEntrance_Room6Triggers[];
extern struct Trigger gMarsLighthouseEntrance_Room7Triggers[];
extern s16 gData_020068a0;
extern u16 gData_020068a2;

void TileMap_ConvertBuffer(s32 scene, s32 a1);

DEFINE_ACTOR_ATTACH(Actor_Attach, Actor_SelectAnimation, Func_02002c2c)

/// Sets up the actor whose id self keeps in byte 0x62: places it 32 pixels out
/// from self, 12 pixels up, along self's angle (unk66), faces it that way,
/// gives it Actor_RollAndCrumble as its update hook and its speed, clears its
/// scale and action, has it watch what self watches, and calls
/// Event_PlaySound(SE_104). The twin of overlay 6ac's Func_02002e6c.
DEFINE_ACTOR_LAUNCH_AHEAD(Actor_LaunchAhead, Actor_RollAndCrumble)

void Actor_SetSpritePriorityDirect(struct Actor *object, s32 mode);

DEFINE_ACTOR_POP_UP_SLOW(Actor_PopUpSlow, Actor_SelectAnimation)

DEFINE_ACTOR_POP_UP(Actor_PopUp, Actor_SelectAnimation)

DEFINE_ACTOR_PARK(Actor_Park, Actor_LaunchAhead)

extern s16 gData_02005758[];

DEFINE_ACTOR_STEER_BY_PAD(Actor_SteerByPad, gData_02005758,
                          Actor_SelectAnimation)

DEFINE_LEADER_STOP_IF_PENDING(Leader_StopIfPending)

void Actor_SelectAnimation(struct Actor *actor, s32 mode);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
extern s16 gData_02005798[];

DEFINE_LEADER_SLIDE_ON_ICE(Leader_SlideOnIce, Leader_StopIfPending,
                           gData_02005798, Actor_SelectAnimation,
                           Actor_SteerByPad, Field_CheckSpheresOverlap)

extern volatile s16 gMarsLighthouseEntrance_SlideStopped;
void Actor_SteerByPad(struct Actor *object);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Actor_SelectAnimation(struct Actor *object, s32 mode);

#define SLIDE_FROM_EDGE Leader_SlideOnIceFromEdge
#define SLIDE_STOPPED gMarsLighthouseEntrance_SlideStopped
#define SLIDE_STOP_LEADER Leader_StopIfPending
#define SLIDE_ANGLES gData_02005798
#define SLIDE_SET_ANIMATION Actor_SelectAnimation
#define SLIDE_HAS_ARRIVED Actor_HasArrived
#define SLIDE_KEEP_SLIDING Actor_SteerByPad
#define SLIDE_SPHERES_OVERLAP Field_CheckSpheresOverlap
#include "shared/slide_on_ice_from_edge.c"
#undef SLIDE_FROM_EDGE
#undef SLIDE_STOPPED
#undef SLIDE_STOP_LEADER
#undef SLIDE_ANGLES
#undef SLIDE_SET_ANIMATION
#undef SLIDE_HAS_ARRIVED
#undef SLIDE_KEEP_SLIDING
#undef SLIDE_SPHERES_OVERLAP

void Leader_SetSlideStopped(s32 value) {
  gMarsLighthouseEntrance_SlideStopped = value;
}

#undef Func_02002614
DEFINE_LEADER_SLIDE_ON_ICE_AHEAD(Leader_SlideOnIceAhead, Leader_StopIfPending,
                                 Actor_SelectAnimation, Actor_SteerByPad,
                                 Field_CheckSpheresOverlap)

#include "../field/object/set_sprite_priority_b.c"

#include "../field/field_effect/move.c"

#undef TLA_MARS_LIGHTHOUSE_ENTRANCE_RIDE_H
#undef RIDE_WORK
#undef DEFINE_FUNC_0200085C
#undef Oam_AddSpriteAtDepthClamped
#undef SET_SPRITE_WINDOW_LAYERS
#undef IWRAM
#undef KNOCK_BACK
#undef KNOCK_BACK_TAKE_DAMAGE
#undef KNOCK_BACK_BLINK
#undef KNOCK_BACK_SET_ANIMATION
#undef KNOCK_BACK_CELL_ATTRIBUTE
#undef KNOCK_BACK_CHECK_COLLISION
#undef KNOCK_BACK_SPHERES_OVERLAP
#undef WORK
#undef ABS
#undef SLIDE_FROM_EDGE
#undef SLIDE_STOPPED
#undef SLIDE_STOP_LEADER
#undef SLIDE_ANGLES
#undef SLIDE_SET_ANIMATION
#undef SLIDE_HAS_ARRIVED
#undef SLIDE_KEEP_SLIDING
#undef SLIDE_SPHERES_OVERLAP
#undef gMarsLighthouseEntrance_Placements
#undef gMarsLighthouseEntrance_Room1Placements
#undef gMarsLighthouseEntrance_Room2Placements
#undef gMarsLighthouseEntrance_Room3Placements
#undef gMarsLighthouseEntrance_Room4Placements
#undef gMarsLighthouseEntrance_Room5Placements
#undef gMarsLighthouseEntrance_Room6Placements
#undef gMarsLighthouseEntrance_Room7Placements
#undef FieldEffect_Spawn
#undef gMarsLighthouseEntrance_Triggers
#undef gMarsLighthouseEntrance_Room1Triggers
#undef gMarsLighthouseEntrance_Room2Triggers
#undef gMarsLighthouseEntrance_Room3Triggers
#undef gMarsLighthouseEntrance_Room4Triggers
#undef gMarsLighthouseEntrance_Room5Triggers
#undef gMarsLighthouseEntrance_Room6Triggers
#undef gMarsLighthouseEntrance_Room7Triggers
#undef Func_02002c2c
#undef gMarsLighthouseEntrance_Triggers
#undef gMarsLighthouseEntrance_Room1Triggers
#undef gMarsLighthouseEntrance_Room2Triggers
#undef gMarsLighthouseEntrance_Room3Triggers
#undef gMarsLighthouseEntrance_Room4Triggers
#undef gMarsLighthouseEntrance_Room5Triggers
#undef gMarsLighthouseEntrance_Room6Triggers
#undef gMarsLighthouseEntrance_Room7Triggers
#undef gMarsLighthouseEntrance_Triggers
#undef gMarsLighthouseEntrance_Room1Triggers
#undef gMarsLighthouseEntrance_Room2Triggers
#undef gMarsLighthouseEntrance_Room3Triggers
#undef gMarsLighthouseEntrance_Room4Triggers
#undef gMarsLighthouseEntrance_Room5Triggers
#undef gMarsLighthouseEntrance_Room6Triggers
#undef gMarsLighthouseEntrance_Room7Triggers
#undef Func_0200246c
#undef Unit_Get
#undef Func_02002614
#undef gMarsLighthouseEntrance_Triggers
#undef gMarsLighthouseEntrance_Room1Triggers
#undef gMarsLighthouseEntrance_Room2Triggers
#undef gMarsLighthouseEntrance_Room3Triggers
#undef gMarsLighthouseEntrance_Room4Triggers
#undef gMarsLighthouseEntrance_Room5Triggers
#undef gMarsLighthouseEntrance_Room6Triggers
#undef gMarsLighthouseEntrance_Room7Triggers
#undef Position
#undef Position
// @file
extern s32 gFieldEffect_DurationScripts[];

void Actor_SetSpriteFlags(struct Actor *object, u32 value);

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options) {
  struct Actor *party = Actor_GetById(gMainRoutineWork.currentOwner);
  struct FieldEffect *effect;
  struct FieldSprite *sprite;
  const s32 *script;

  if ((flags & EFFECT_USE_TYPE) != 0 && options != 0)
    effect = (struct FieldEffect *)Import_ActorCreate(options->type, x, y, z);
  else
    effect = (struct FieldEffect *)Import_ActorCreate(285, x, y, z);
  if (effect == 0)
    return;

  sprite = effect->sprite;
  Actor_SelectAnimation(effect, (flags + 1) & EFFECT_SCRIPT_MASK);
  Actor_StartScript(effect,
                    gFieldEffect_DurationScripts[flags & EFFECT_SCRIPT_MASK]);
  effect->motionFlags = 0;
  Actor_SetSpriteFlags(effect, 0);
  effect->update = FieldEffect_UpdateMotionScaleAndSpin;
  effect->velocity.x = velocityX;
  effect->velocity.y = velocityY;
  effect->velocity.z = velocityZ;
  Actor_SetSpritePriorityB(effect,
                           ((struct FieldSprite *)party->sprite)->priority);
  effect->scaleRateX = 0;
  effect->scaleRateY = 0;
  effect->spin = 0;

  if ((flags & 0xffff0000) == 0 || options == 0)
    return;
  if ((flags & EFFECT_USE_PALETTE) != 0)
    FieldActor_SetSpriteColor(effect, options->palette);
  if ((flags & EFFECT_USE_PRIORITY) != 0) {
    effect->priorityFlags &= ~1;
    Actor_SetSpritePriorityB(effect, options->priority);
  }
  if ((flags & EFFECT_USE_START_SCALE) != 0) {
    effect->scaleX = options->startScaleX;
    effect->scaleY = options->startScaleY;
  }
  if ((flags & EFFECT_SCALE_TO_TARGET) != 0) {
    s32 target;
    s32 frames;

    script = gFieldEffect_DurationScripts[flags & EFFECT_SCRIPT_MASK];
    target = options->targetScaleX;
    frames = script[3];
    if ((flags & EFFECT_USE_START_SCALE) != 0) {
      effect->scaleRateX = (target - effect->scaleX) / frames;
      effect->scaleRateY = (options->targetScaleY - effect->scaleY) / script[3];
    } else {
      effect->scaleRateX = (target - 0x10000) / frames;
      effect->scaleRateY = (options->targetScaleY - 0x10000) / script[3];
    }
  }
  if ((flags & EFFECT_USE_SCRIPT) != 0) {
    Actor_SelectAnimation(effect, 1);
    Actor_StartScript(effect, options->script);
  }
  if ((flags & EFFECT_USE_ROTATION) != 0)
    sprite->rotation = options->rotation;
  if ((flags & EFFECT_USE_SPIN) != 0)
    effect->spin = options->spin;
  if ((flags & EFFECT_USE_UPDATE) != 0)
    effect->update = options->update;
}

#define CELL_TABLE Data_020068b4
#define CELL_SPRITE_DRAW CrackedTiles_Update
#define CELL_PUSH_SLOT_ENTRY Oam_AddSpriteAtDepthClamped
#include "../field/map/cell_sprite.c"

DEFINE_CRACKED_TILES_FIND(CrackedTiles_Find, Data_020068b4, CrackedTiles_Update)

DEFINE_LEADER_CENTER_IN_CELL(Leader_CenterInCell)

// Map_CopyCellsRect through this overlay's import.

// Centers the leader on its cell, plays sound 161 and copies the cell below
// the table's source cell over the leader's, keeping the source column's and
// row's 64 bit; CrackedTiles_Step calls it the first time the leader steps on
// one of the table's cells.
DEFINE_CRACKED_TILE_CRACK(CrackedTile_Crack, Leader_CenterInCell)

void Func_02004160(struct PushEvent *event) {
  struct Actor *object = Actor_Get(gMainRoutineWork.currentOwner);
  s32 x;
  s32 z;
  s32 mask;

  mask = 64;
  x = object->x >> 20;
  do {
  } while (0);
  x |= (s16)(event->x & mask);
  z = object->z >> 20;
  z |= (s16)(event->z & mask);
  Leader_CenterInCell();
  Event_PlaySound(SE_SPLASH);
  MapGrid_CopyCells(event->x, event->z + 2, 1, 1, x, z);
  FieldEvent_Wait(12);
  {
    u32 zero = 0;

    object->facing = 0x4000;
    object->motionFlags = zero;
  }
  Actor_SetSpriteFlags(Actor_Get(gMainRoutineWork.currentOwner), 0);
  {
    u32 winout = 0xf1f;

    REG_WINOUT = winout;
  }
  do {
  } while (0);
  REG_DISPCNT |= 0x8000;
  event->busy = 1;
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 28);
  FieldEvent_Wait(16);
  for (x = 0; x < 40; x++) {
    if (x == 5)
      Event_PlaySound(SE_DUST_PUFF);
    object->scaleX -= 0x400;
    object->scaleY -= 0x300;
    object->y -= 0x9999;
    System_Wait(1);
  }
  do {
    Actor_Get(gMainRoutineWork.currentOwner)->animationKind = 0;
  } while (0);
  event->busy = 0;
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x209;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
}

#include "../field/field_effect/advance_motion.c"

#undef CELL_TABLE
#undef CELL_SPRITE_DRAW
#undef CELL_PUSH_SLOT_ENTRY
// @file
void Teleport_PlayWhirl(void);

void Func_020042f8(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  struct EffectOptions options;
  struct Vector3 velocity;
  u32 i;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Camera_SetBounds(-1, -1, -1, 0);
  Map_CenterOnFollowedPoint();
  System_Wait(1);
  leader->y = 0x820000;
  leader->gravity = 0x8000;
  leader->bounce = 0;
  leader->motionFlags = 0;
  SCENE_WORK->unk1ac = 0x100;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_PlaySound(SE_DUST_PUFF);
  leader->motionFlags = 3;
  FieldEvent_Wait(24);
  options.palette = 7;
  options.update = FieldEffect_AdvanceMotion;
  options.startScaleX = 0xa3d7;
  options.startScaleY = 0xa3d7;
  i = 0;
next:
  {
    s32 angle = i << 12;

    velocity.x = Trig_Cos(angle);
    velocity.y = 0;
    velocity.z = Math_Sin(angle);
    velocity.x += velocity.x >> 1;
    velocity.x += ((u32)(Random16() * 3 << 14) >> 16) - 0x6000;
    velocity.z += ((u32)(Random16() * 3 << 13) >> 16) - 0x3000;
    FieldEffect_Spawn(leader->x, leader->y, leader->z, velocity.x, velocity.y,
                      velocity.z, 0x1090001, &options);
  }
  if (++i <= 16)
    goto next;
  Event_PlaySound(SE_THUMP);
  Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 0x101);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 49);
  MapGrid_SetShake(0x50000, 0x50000, 0x10000);
  MapGrid_SetShake(-1, -1, 0xe666);
  FieldMapGrid_WaitForShakeToSettle();
  Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 0x100);
  leader->gravity = 0x10000;
  leader->bounce = 0x4000;
  FieldEvent_Wait(10);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 1);
  Event_End();
}

void GameFlag_SetByte(s32 flag, s32 value);
/* VIEW(s42f8): routine Func_02004160 as this part declared it; the joined file
 * has another; tried as the definition's
 * DEFINE_CRACKED_TILES_FIND(CrackedTiles_Find, Data_020068b4,
 * CrackedTiles_Update) DEFINE_LEADER_CENTER_IN_CELL(Leader_CenterInCell)
 * DEFINE_CR..., which the object already declares, it does not compile in
 * tla-ja: games/tla/src/scene/mars_lighthouse_entrance.c:2042: too many
 * arguments to function `Func_02004160'. */
void Func_02004160__s42f8(
    struct CellTarget *work,
    struct CellTargetEntry *entry) __asm__("Func_02004160");
#define Func_02004160 Func_02004160__s42f8

DEFINE_CRACKED_TILES_STEP(CrackedTiles_Step, Data_020068b4, CrackedTile_Crack,
                          Func_02004160)

DEFINE_CRACKED_TILE_PLACE_LANDING(CrackedTile_PlaceLanding, Data_020068b4,
                                  Func_020042f8)

extern u16 *gData_0200e88c;
extern u16 *gData_02006890;

// Records the four palette lists, writes each listed palette entry from its
// red, green and blue, then copies two palettes from block32 and starts the
// blend.
DEFINE_PALETTE_SET_COLORS_FADE(Palette_SetColorsFade, gData_0200e88c,
                               gData_02006890, Data_02006894, Data_02006880,
                               Func_020048e8)

extern u16 gData_02006898;

DEFINE_PALETTE_CYCLE_START(Func_020046b4, gData_0200689c, gData_02006898,
                           List_CountEntries, gData_0200e88c, gData_0200e888,
                           15, gData_02006884, 16, gData_0200687c,
                           PaletteCycle_Fade)

void Func_02004718(void) {
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 428) = 256;
  FieldTask_Remove(PaletteCycle_Fade);
}

DEFINE_PALETTE_CYCLE_START(Func_02004738, gData_0200689c, gData_02006898,
                           List_CountEntries, gData_0200e88c, gData_0200e888, 2,
                           gData_02006884, 2, gData_0200687c, PaletteCycle_Fade)

DEFINE_LIST_COUNT_ENTRIES(List_CountEntries)

DEFINE_PALETTE_CYCLE_FADE(PaletteCycle_Fade, gData_02006884, gData_0200e888,
                          Data_02006894, gData_02006898, Data_02006880,
                          gData_0200689c, gData_0200e88c, gData_0200687c,
                          gData_02006890)

void Func_020048e8(void) {
  struct MarsLighthouseEntranceEventWork *work = Ram_HeapSlots->eventWork;
  s32 saved;

  saved = work->value1ac;
  PaletteFade_ApplyTarget(0x202108, 1);
  PaletteFade_ApplyTarget(0x10000, 2);
  ((struct MarsLighthouseEntranceEventWork *)Ram_HeapSlots->eventWork)
      ->value1ac = 0x100;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Func_02004738();
  Teleport_PlayWhirl();
  System_Wait(40);
  Func_02004718();
  PaletteFade_ApplyTarget(0x10000, 1);
  PaletteFade_Interpolate(16);
  System_Wait(16);
  {
    struct MainRoutineWork *party = &gMainRoutineWork;

    party->unk2c2 = 0;
  }
  work->value1ac = saved;
}

DEFINE_ACTORS_SET_CELLS_OPEN(Actors_SetCellsOpen, Actor_SetCellOpen)

// Finds the current target's table row and shrinks it.
void Func_020049dc(u16 *list, s32 mode, s32 count) {
  struct GabombaStatueEffectWork *work = Ram_HeapSlots->effectWork;

  /* FAKEMATCH(dead): the entry jump in a once-only block. */
  do {
    goto test;
  } while (0);
  do {
    u32 entry = *list;
    s32 id = work->objectId;

    if (id == entry) {
      struct Actor *object = Actor_Get(id);
      s32 a;
      s32 flag;

      list++;
      a = list[0];
      flag = list[1];
      if (count < 8)
        object->scaleY -= 0x1f00;
      if (mode == 1) {
        Event_SetFlag(flag);
        Actor_SetCellOpen(id, a, flag, object->terrainId);
        object->scaleY = 0x10000;
      }
      break;
    }
    list += 3;
  test:;
  } while (*list != 0xffff);
}

DEFINE_ACTOR_SET_CELL_OPEN(Actor_SetCellOpen, Actor_GetCellIndex,
                           Actor_SelectAnimation)

// clang-format off
DEFINE_G_DATA_02004DE8(gData_02004de8, 200, 104)

DEFINE_G_DATA_02004DE8(gData_02004e50, 232, 296)

DEFINE_G_DATA_02004EB8(gData_02004eb8, 48, 904, 528, 840)

u32 gData_02004f44[] = {
    SC_SET(SW_STEERING, 0),
    SC_SET(SW_MOTION, 2),
    SC_SET(SW_Y, PIXELS(48)),
    SC_SET(SW_GROUND, PIXELS(48)),
    SC_SET(SW_SPEED, 0x8000),
    SC_SET(SW_ACCEL, 0x800),
    SC_WAIT(10),
    SC_MOVE(PIXELS(88), 0, PIXELS(688)),
    SC_SYNC,
    SC_WAIT(30),
    SC_LABEL(1),
    SC_MOVE(PIXELS(152), 0, PIXELS(688)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(88), 0, PIXELS(688)),
    SC_SYNC,
    SC_LOOP(SC_FOREVER, SC_TO(1)),
};

DEFINE_G_DATA_02004FF0(gData_02004ff0, 0)

s32 gData_0200503c[] = {
    SC_SET(SW_STEERING, 0),
    SC_SET(SW_MOTION, 0),
    SC_SET(SW_Y, PIXELS(32)),
    SC_SET(SW_GROUND, PIXELS(32)),
    SC_SET(SW_SPEED, 0x8000),
    SC_SET(SW_ACCEL, 0x800),
    SC_MOVE(PIXELS(808), PIXELS(32), PIXELS(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(808), PIXELS(48), PIXELS(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(728), PIXELS(48), PIXELS(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(728), PIXELS(32), PIXELS(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(712), PIXELS(32), PIXELS(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(712), PIXELS(48), PIXELS(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(616), PIXELS(48), PIXELS(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(616), PIXELS(32), PIXELS(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(552), PIXELS(32), PIXELS(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(552), PIXELS(48), PIXELS(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(536), PIXELS(48), PIXELS(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(536), PIXELS(32), PIXELS(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(520), PIXELS(32), PIXELS(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(520), PIXELS(48), PIXELS(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(296), PIXELS(48), PIXELS(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(296), PIXELS(32), PIXELS(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(264), PIXELS(32), PIXELS(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(264), PIXELS(48), PIXELS(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(136), PIXELS(48), PIXELS(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(136), PIXELS(32), PIXELS(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(120), PIXELS(32), PIXELS(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(120), PIXELS(48), PIXELS(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(56), PIXELS(48), PIXELS(128)),
    SC_SYNC,
    SC_WAIT(60),
    SC_CALL(Func_02000070),
    SC_END,
};

u32 gData_02005314[] = {
    SC_SET(SW_STEERING, 0),
    SC_SET(SW_MOTION, 0),
    SC_SET(SW_X, PIXELS(56)),
    SC_SET(SW_Z, PIXELS(128)),
    SC_SET(SW_Y, PIXELS(48)),
    SC_SET(SW_GROUND, PIXELS(48)),
    SC_SET(SW_SPEED, 0x8000),
    SC_SET(SW_ACCEL, 0x800),
    SC_MOVE(PIXELS(56), PIXELS(48), PIXELS(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_END,
};

u32 gData_02005394[] = {
    0, 40, 0x80010000, 22, 25, 0,
    0, 1, 22, 25, 1, 0,
    1, 13, 10, 0xc0010000, 38,
};

s16 gData_020053d8[] = {
    8, 7, 512, 9, 7, 513, -1,
};

u16 gData_020053e6[] = {
    9, 3, 10, 3, 11, 3, 12, 3,
    0,
};

u16 gData_020053f8[] = {
    8, 0, 0,
};

s16 gData_020053fe[] = {
    16, 7, 514, 17, 7, 515, -1,
};

u16 gData_0200540c[] = {
    8, 3, 0,
};

u16 gData_02005412[] = {
    9, 1, 0,
};

s16 gData_02005418[] = {
    12, 7, 516, 13, 7, 517, 14, 7,
    518, 15, 7, 519, 16, 7, 520, -1,
};

u16 gData_02005438[] = {
    9, 3, 0,
};

u16 gData_0200543e[] = {
    8, 0, 10, 1, 0,
};

u16 gData_02005448[] = {
    11, 67, 0,
};

u16 gData_0200544e[] = {
    12, 1, 0,
};

s16 gData_02005454[] = {
    13, 7, 528, 14, 7, 529, 15, 7,
    530, 16, 7, 531, 17, 7, 532, 18,
    7, 533, -1, 0,
};

DEFINE_G_DATA_0200547C(gData_0200547c, Actor_PopUpSlow, 112, 728, 22, 128, 728,
    Actor_Park)

DEFINE_G_DATA_020054CC(gData_020054cc, Actor_PopUpSlow, 0x500000, 0x200000,
    0xa80000, 0x160000, 0x600000, 0xa80000, Func_02000c68, Actor_Park)

DEFINE_G_DATA_0200547C(gData_02005528, Actor_PopUpSlow, 872, 576, 30, 872, 584,
    Actor_Park)

DEFINE_G_DATA_020054CC(gData_02005578, Actor_PopUp, 0x600000, 0x200000,
    0x2a80000, 0x160000, 0x700000, 0x2a80000, Func_02000a24, Actor_Park)

s32 gData_020055d4[] = {
    46,
    (s32)Actor_PopUp,
    4,
    0x3580000,
    0x200000,
    0x880000,
    1,
    4,
    0x3680000,
    0x200000,
    0x880000,
    1,
    4,
    0x3680000,
    0x160000,
    0x880000,
    1,
    4,
    0x3680000,
    0x160000,
    0x980000,
    1,
    22,
    37,
    (s32)Func_02000c1c,
    46,
    (s32)Actor_Park,
    17,
};

DEFINE_G_DATA_020054CC(gData_02005644, Actor_PopUp, 0x700000, 0x200000,
    0x2a80000, 0x160000, 0x800000, 0x2a80000, Func_02000808, Actor_Park)

DEFINE_G_DATA_02001540(gData_020056a0, 30)

u32 gData_020056d0[] = {
    0, 20, 0x80010000, 22, 25, 0,
    0, 1, 22, 25, 1, 0,
    1, 13, 10, 0xc0010000, 38,
};

u32 gData_02005714[] = {
    0, 20, 0x80010000, 22, 25, 0,
    0, 1, 22, 25, 1, 0,
    1, 13, 10, 0xc0010000, 38,
};

s16 gData_02005758[] = {
    -1, 0, -32768, -1, -16384, -8192, -24576, -16384,
    16384, 8192, 24576, 16384, -1, 0, -32768, -1,
    -1, 0, -32768, -1, -16384, -1, -1, -16384,
    16384, -1, -1, 16384, -1, 0, -32768, -1,
};

s16 gData_02005798[] = {
    -1, 0, -32768, -1, -16384, 0, -32768, -16384,
    16384, 0, -32768, 16384, -1, 0, -32768, -1,
};

DEFINE_FIELD_EFFECT_DURATION_SCRIPT(gFieldEffect_ShortScript, 22)

DEFINE_FIELD_EFFECT_DURATION_SCRIPT(gFieldEffect_MediumScript, 44)

DEFINE_FIELD_EFFECT_DURATION_SCRIPT(gFieldEffect_LongScript, 126)

s16 gData_0200586c[] = {
    137, 138, 139, 140, 141, 142, 143, -1,
};

s16 gData_0200587c[] = {
    23, 25, 22, 23, 25, 22, 23, 25,
    22, 23, 25, 22, 23, 25, 22, 23,
    25, 22, 23, 25, 22, -1,
};

s16 gData_020058a8[] = {
    30, 27, 31, 22, 21, 31, 20, 18,
    31, 13, 11, 27, 11, 8, 24, 8,
    5, 21, 31, 31, 31, -1,
};

u32 gData_020058d4[] = {
    0x89008e, 0x8b008a, 0x8d008c, 0x8e008d, 0x8a0089, 0x8c008b,
    0x8d008c, 0x89008e, 0x8b008a, 0x8c008b, 0x8e008d, 0x8a0089,
    0x8b008a, 0x8d008c, 0x89008e, 0x8a0089, 0x8c008b, 0x8e008d,
};

s32 gData_0200591c[] = {
    SC_CALL(Func_02000038),
    SC_END,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 192, 0, 192, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(279),
    SCENE_EXIT(1, 2, 84),
    SCENE_EXIT(2, 281, 2),
    SCENE_EXITS(280),
    SCENE_EXIT(1, 281, 1),
    SCENE_EXIT(2, 287, 2),
    SCENE_EXIT(3, 280, 4),
    SCENE_EXIT(4, 280, 3),
    SCENE_EXIT(5, 282, 5),
    SCENE_EXIT(6, 282, 6),
    SCENE_EXIT(7, 280, 8),
    SCENE_EXIT(8, 280, 7),
    SCENE_EXIT(9, 280, 10),
    SCENE_EXIT(10, 280, 9),
    SCENE_EXIT(20, 281, 20),
    SCENE_EXITS(281),
    SCENE_EXIT(1, 280, 1),
    SCENE_EXIT(2, 279, 2),
    SCENE_EXIT(3, 282, 3),
    SCENE_EXIT(4, 282, 4),
    SCENE_EXIT(5, 283, 5),
    SCENE_EXIT(6, 281, 7),
    SCENE_EXIT(7, 281, 6),
    SCENE_EXIT(8, 281, 9),
    SCENE_EXIT(9, 281, 8),
    SCENE_EXIT(10, 281, 11),
    SCENE_EXIT(11, 281, 10),
    SCENE_EXIT(12, 281, 13),
    SCENE_EXIT(13, 281, 12),
    SCENE_EXIT(14, 281, 15),
    SCENE_EXIT(15, 281, 14),
    SCENE_EXIT(16, 282, 16),
    SCENE_EXIT(17, 281, 18),
    SCENE_EXIT(18, 281, 17),
    SCENE_EXIT(20, 280, 3),
    SCENE_EXITS(282),
    SCENE_EXIT(1, 283, 1),
    SCENE_EXIT(2, 283, 2),
    SCENE_EXIT(3, 281, 3),
    SCENE_EXIT(4, 281, 4),
    SCENE_EXIT(5, 280, 5),
    SCENE_EXIT(6, 280, 6),
    SCENE_EXIT(7, 284, 7),
    SCENE_EXIT(8, 282, 9),
    SCENE_EXIT(9, 282, 8),
    SCENE_EXIT(10, 282, 11),
    SCENE_EXIT(11, 282, 10),
    SCENE_EXIT(14, 283, 14),
    SCENE_EXIT(16, 281, 16),
    SCENE_EXITS(283),
    SCENE_EXIT(1, 282, 1),
    SCENE_EXIT(2, 282, 2),
    SCENE_EXIT(3, 283, 4),
    SCENE_EXIT(4, 283, 3),
    SCENE_EXIT(5, 281, 5),
    SCENE_EXIT(6, 283, 7),
    SCENE_EXIT(7, 283, 6),
    SCENE_EXIT(8, 283, 9),
    SCENE_EXIT(9, 283, 8),
    SCENE_EXIT(10, 283, 11),
    SCENE_EXIT(11, 283, 10),
    SCENE_EXIT(12, 283, 13),
    SCENE_EXIT(13, 283, 12),
    SCENE_EXIT(14, 282, 14),
    SCENE_EXITS(284),
    SCENE_EXIT(1, 285, 1),
    SCENE_EXIT(2, 284, 3),
    SCENE_EXIT(3, 284, 2),
    SCENE_EXIT(4, 284, 5),
    SCENE_EXIT(5, 284, 4),
    SCENE_EXIT(6, 286, 6),
    SCENE_EXIT(7, 282, 7),
    SCENE_EXIT(8, 286, 8),
    SCENE_EXIT(9, 286, 9),
    SCENE_EXIT(10, 284, 11),
    SCENE_EXIT(11, 284, 10),
    SCENE_EXIT(12, 284, 13),
    SCENE_EXIT(13, 284, 12),
    SCENE_EXIT(14, 284, 15),
    SCENE_EXIT(15, 284, 14),
    SCENE_EXIT(16, 286, 10),
    SCENE_EXITS(285),
    SCENE_EXIT(1, 284, 1),
    SCENE_EXIT(2, 285, 3),
    SCENE_EXIT(3, 285, 2),
    SCENE_EXIT(4, 285, 6),
    SCENE_EXIT(5, 286, 5),
    SCENE_EXIT(6, 285, 4),
    SCENE_EXIT(7, 286, 7),
    SCENE_EXIT(11, 286, 11),
    SCENE_EXITS(286),
    SCENE_EXIT(1, 286, 2),
    SCENE_EXIT(2, 286, 1),
    SCENE_EXIT(3, 286, 4),
    SCENE_EXIT(4, 286, 3),
    SCENE_EXIT(5, 285, 5),
    SCENE_EXIT(6, 284, 6),
    SCENE_EXIT(7, 285, 7),
    SCENE_EXIT(8, 284, 8),
    SCENE_EXITS_END,
};

u32 gData_02005ae8[] = {
    39, 7, 17,
};

struct ActorPlacement gMarsLighthouseEntrance_Placements[] = {
    {SCENE_TABLE_END},
};

struct ActorPlacement gMarsLighthouseEntrance_Room1Placements[] = {
    {413, CONDITION_ALWAYS, 1, PIXELS(232), 0, PIXELS(280), FACING_EAST, 2, 0},
    {413, CONDITION_ALWAYS, 1, PIXELS(296), 0, PIXELS(280), FACING_WEST, 2, 0},
    {SCENE_TABLE_END},
};
// clang-format on

struct ActorPlacement gMarsLighthouseEntrance_Room2Placements[] = {
    {406, CONDITION_ALWAYS, (s32)gData_02005ae8, 0, 0, 0, FACING_SOUTH, 2, 0},
    {406, CONDITION_ALWAYS, (s32)gData_02005ae8, PIXELS(136), 0, PIXELS(424),
     FACING_SOUTH, 2, 0},
    {489, 800, 1, PIXELS(648), 0, PIXELS(376), FACING_SOUTH, 2, 0},
#if !TLA_PATCH_1
    {245, 105, 1, PIXELS(664), 0, PIXELS(424), FACING_SOUTH, 0, 0},
#else
    {245, 105, 1, PIXELS(728), 0, PIXELS(408), FACING_NORTHWEST, 0, 0},
#endif
    {SCENE_TABLE_END},
};

void MarsLighthouseEntrance_FightFugue(void);
void MarsLighthouseEntrance_FightBalm(void);
void MarsLighthouseEntrance_GrindIce(void);
void Func_02000648(void);
void Func_02000718(void);
void Func_0200112c(void);
void Func_02001138(void);

// clang-format off
struct ActorPlacement gMarsLighthouseEntrance_Room3Placements[] = {
    {413, CONDITION_ALWAYS, 1, PIXELS(56), 0, PIXELS(120), FACING_SOUTH, 2, 0},
    {414, CONDITION_ALWAYS, (s32)gData_02004de8, PIXELS(104), 0, PIXELS(112), FACING_SOUTH, 2, 0},
    {414, CONDITION_ALWAYS, (s32)gData_02004e50, PIXELS(280), 0, PIXELS(112), FACING_SOUTH, 2, 0},
    {414, CONDITION_ALWAYS, 1, PIXELS(328), 0, PIXELS(112), FACING_SOUTH, 2, 0},
    {414, CONDITION_ALWAYS, 1, PIXELS(72), 0, PIXELS(112), FACING_SOUTH, 2, 0},
    {419, CONDITION_ALWAYS, (s32)gData_0200591c, PIXELS(88), 0, PIXELS(680), FACING_SOUTH, 2, 0},
    {419, CONDITION_ALWAYS, (s32)gData_0200591c, PIXELS(552), 0, PIXELS(680), FACING_SOUTH, 2, 0},
    {418, CONDITION_ALWAYS, (s32)gData_0200591c, PIXELS(88), 0, PIXELS(728), FACING_SOUTH, 2, 0},
    {406, CONDITION_ALWAYS, (s32)gData_02005ae8, 0, 0, 0, FACING_SOUTH, 2, 0},
    {406, CONDITION_ALWAYS, (s32)gData_02005ae8, PIXELS(552), 0, PIXELS(776), FACING_SOUTH, 2, 0},
    {291, CONDITION_ALWAYS, 7, PIXELS(840), 0, PIXELS(672), FACING_SOUTH, 2, 0},
    {419, CONDITION_ALWAYS, (s32)gData_0200591c, PIXELS(88), 0, PIXELS(728), FACING_SOUTH, 2, 0},
    {291, CONDITION_ALWAYS, 7, PIXELS(480), 0, PIXELS(704), FACING_SOUTH, 2, 0},
    {673, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {412, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};
// clang-format on

// clang-format off
struct ActorPlacement gMarsLighthouseEntrance_Room4Placements[] = {
    {414, CONDITION_ALWAYS, (s32)gData_02004eb8, PIXELS(840), 0, PIXELS(528), FACING_SOUTH, 2, 0},
    {419, CONDITION_ALWAYS, (s32)gData_0200591c, PIXELS(872), 0, PIXELS(568), FACING_SOUTH, 2, 0},
    {413, CONDITION_ALWAYS, 1, PIXELS(200), 0, PIXELS(792), FACING_EAST, 2, 0},
    {413, CONDITION_ALWAYS, 1, PIXELS(264), 0, PIXELS(792), FACING_WEST, 2, 0},
    {406, CONDITION_ALWAYS, (s32)gData_02005ae8, PIXELS(840), 0, PIXELS(680), FACING_SOUTH, 2, 0},
    {406, CONDITION_ALWAYS, (s32)gData_02005ae8, PIXELS(904), 0, PIXELS(744), FACING_SOUTH, 2, 1},
    {406, CONDITION_ALWAYS, (s32)gData_02005ae8, PIXELS(840), 0, PIXELS(808), FACING_SOUTH, 2, 1},
    {406, CONDITION_ALWAYS, (s32)gData_02005ae8, PIXELS(936), 0, PIXELS(824), FACING_SOUTH, 2, 1},
    {406, CONDITION_ALWAYS, (s32)gData_02005ae8, PIXELS(904), 0, PIXELS(872), FACING_SOUTH, 2, 1},
    {244, 0x54, 1, PIXELS(88), 0, PIXELS(904), FACING_SOUTH, 0, 0},
    {673, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {673, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {412, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {412, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};
// clang-format on

// clang-format off
struct ActorPlacement gMarsLighthouseEntrance_Room5Placements[] = {
    {418, CONDITION_ALWAYS, (s32)gData_0200591c, PIXELS(72), 0, PIXELS(632), FACING_SOUTH, 2, 0},
    {419, CONDITION_ALWAYS, (s32)gData_0200591c, PIXELS(72), 0, PIXELS(680), FACING_SOUTH, 2, 0},
    {418, CONDITION_ALWAYS, (s32)gData_0200591c, PIXELS(904), 0, PIXELS(120), FACING_SOUTH, 2, 0},
    {419, CONDITION_ALWAYS, (s32)gData_0200591c, PIXELS(856), 0, PIXELS(120), FACING_SOUTH, 2, 0},
    {291, CONDITION_ALWAYS, 7, PIXELS(240), 0, PIXELS(688), FACING_SOUTH, 2, 0},
    {673, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {412, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};
// clang-format on

// clang-format off
struct ActorPlacement gMarsLighthouseEntrance_Room6Placements[] = {
    {413, CONDITION_ALWAYS, 1, PIXELS(72), 0, PIXELS(696), FACING_SOUTH, 2, 0},
    {414, CONDITION_ALWAYS, (s32)gData_02004f44, PIXELS(120), 0, PIXELS(688), FACING_SOUTH, 2, 0},
    {419, CONDITION_ALWAYS, (s32)gData_0200591c, PIXELS(88), 0, PIXELS(728), FACING_SOUTH, 2, 0},
    {414, CONDITION_ALWAYS, (s32)gData_02004ff0, PIXELS(792), 0, PIXELS(128), FACING_SOUTH, 2, 0},
    {419, CONDITION_ALWAYS, (s32)gData_0200591c, PIXELS(56), 0, PIXELS(168), FACING_SOUTH, 2, 0},
    {406, CONDITION_ALWAYS, (s32)gData_02005ae8, PIXELS(200), 0, PIXELS(168), FACING_SOUTH, 2, 0},
    {406, CONDITION_ALWAYS, (s32)gData_02005ae8, PIXELS(328), 0, PIXELS(184), FACING_SOUTH, 2, 0},
    {406, CONDITION_ALWAYS, (s32)gData_02005ae8, PIXELS(360), 0, PIXELS(152), FACING_SOUTH, 2, 0},
    {406, CONDITION_ALWAYS, (s32)gData_02005ae8, PIXELS(424), 0, PIXELS(152), FACING_SOUTH, 2, 0},
    {406, CONDITION_ALWAYS, (s32)gData_02005ae8, PIXELS(664), 0, PIXELS(152), FACING_SOUTH, 2, 0},
    {406, CONDITION_ALWAYS, (s32)gData_02005ae8, PIXELS(760), 0, PIXELS(152), FACING_SOUTH, 2, 0},
    {673, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {412, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};
// clang-format on

// clang-format off
struct ActorPlacement gMarsLighthouseEntrance_Room7Placements[] = {
    {413, CONDITION_ALWAYS, 1, PIXELS(88), 0, PIXELS(152), FACING_EAST, 2, 0},
    {413, CONDITION_ALWAYS, 1, PIXELS(152), 0, PIXELS(152), FACING_WEST, 2, 0},
    {SCENE_TABLE_END},
};

struct Trigger gMarsLighthouseEntrance_Triggers[] = {
    {TRIGGER_TABLE_END},
};

struct Trigger gMarsLighthouseEntrance_Room1Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_TABLE_END},
};
// clang-format on

struct Trigger gMarsLighthouseEntrance_Room2Triggers[] = {
    {0x31, 1, CONDITION_ALWAYS, 1},
    {0x21, 2, CONDITION_ALWAYS, 2},
    {0x51, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {0x31, 5, CONDITION_ALWAYS, 5},
    {0x31, 6, CONDITION_ALWAYS, 6},
    {0x51, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {0x51, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_EXIT, 13, CONDITION_ALWAYS, 13},
    {TRIGGER_EXIT, 14, CONDITION_ALWAYS, 14},
    {TRIGGER_EXIT, 15, CONDITION_ALWAYS, 15},
    {TRIGGER_EXIT, 16, CONDITION_ALWAYS, 16},
    {0x8400, 11, CONDITION_ALWAYS, EVENT(MarsLighthouseEntrance_FightFugue)},
    {0xc400, 11, CONDITION_ALWAYS, EVENT(MarsLighthouseEntrance_FightFugue)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 8, 0x200,
     EVENT(MarsLighthouseEntrance_StartPoundIceCone)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 9, 0x201,
     EVENT(MarsLighthouseEntrance_StartPoundIceCone)},
    {PSYNERGY_ON_ACTOR(ABILITY_GRIND), 10, 0xa3e,
     EVENT(MarsLighthouseEntrance_GrindIce)},
    {TRIGGER_TALK, 10, 0xa3e, EVENT(MarsLighthouseEntrance_CheckIce)},
    {TRIGGER_TABLE_END},
};

struct Trigger gMarsLighthouseEntrance_Room3Triggers[] = {
    {0x21, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {0x51, 3, CONDITION_ALWAYS, 3},
    {0x51, 4, CONDITION_ALWAYS, 4},
    {0x31, 5, CONDITION_ALWAYS, 5},
    {0x51, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {0x51, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_EXIT, 13, CONDITION_ALWAYS, 13},
    {0x51, 14, CONDITION_ALWAYS, 14},
    {TRIGGER_EXIT, 15, CONDITION_ALWAYS, 15},
    {0x31, 16, CONDITION_ALWAYS, 16},
    {TRIGGER_STEP, 88, CONDITION_ALWAYS, EVENT(Leader_KnockBack)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 16, 0x202,
     EVENT(MarsLighthouseEntrance_StartPoundIceCone)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 17, 0x203,
     EVENT(MarsLighthouseEntrance_StartPoundIceCone)},
    {0x9c05, 17, CONDITION_ALWAYS, EVENT(Func_02000648)},
    {0x9c05, 18, CONDITION_ALWAYS, EVENT(MarsLighthouseEntrance_Teleport)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 8, 0xa4f,
     EVENT(MarsLighthouseEntrance_PushStatueIntoFlames)},
    {9, 0, CONDITION_ALWAYS,
     EVENT(MarsLighthouseEntrance_PushStatueIntoFlames)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 80, 0x1230,
     EVENT(MarsLighthouseEntrance_BurstIceSheet)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 81, 0x1231,
     EVENT(Func_0200085c)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST), 15,
     CONDITION_ALWAYS, 0},
    {0x60009a15, 13, CONDITION_ALWAYS, EVENT(Func_02000710)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_AFTER_END), 13,
     CONDITION_ALWAYS, EVENT(Func_02000718)},
    {TRIGGER_TALK, 64, CONDITION_ALWAYS, EVENT(Func_02000930)},
    {TRIGGER_TABLE_END},
};

struct Trigger gMarsLighthouseEntrance_Room4Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {0x21, 5, CONDITION_ALWAYS, 5},
    {0x21, 6, CONDITION_ALWAYS, 6},
    {0x31, 7, CONDITION_ALWAYS, 7},
    {0x51, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {0x51, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_EXIT, 13, CONDITION_ALWAYS, 13},
    {TRIGGER_EXIT, 15, CONDITION_ALWAYS, 15},
    {0x21, 16, CONDITION_ALWAYS, 16},
    {TRIGGER_STEP, 30, CONDITION_ALWAYS, EVENT(Leader_SlideOnIce)},
    {TRIGGER_STEP, 90, CONDITION_ALWAYS,
     EVENT(MarsLighthouseEntrance_SlideOnIce)},
    {TRIGGER_TALK, 17, CONDITION_ALWAYS,
     EVENT(MarsLighthouseEntrance_FightBalm)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_ACTS), 10,
     CONDITION_ALWAYS, EVENT(Func_0200096c)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 11, CONDITION_ALWAYS, 0},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 12, 0x204,
     EVENT(MarsLighthouseEntrance_StartPoundIceCone)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 13, 0x205,
     EVENT(MarsLighthouseEntrance_StartPoundIceCone)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 14, 0x206,
     EVENT(MarsLighthouseEntrance_StartPoundIceCone)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 15, 0x207,
     EVENT(MarsLighthouseEntrance_StartPoundIceCone)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 16, 0x208,
     EVENT(MarsLighthouseEntrance_StartPoundIceCone)},
    {0x9c05, 14, CONDITION_ALWAYS, EVENT(Func_0200093c)},
    {TRIGGER_TABLE_END},
};

struct Trigger gMarsLighthouseEntrance_Room5Triggers[] = {
    {0x51, 1, CONDITION_ALWAYS, 1},
    {0x51, 2, CONDITION_ALWAYS, 2},
    {0x51, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {0x21, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {0x51, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {0x51, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {0x51, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_EXIT, 13, CONDITION_ALWAYS, 13},
    {TRIGGER_STEP, 30, CONDITION_ALWAYS, EVENT(Leader_SlideOnIce)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST), 8,
     CONDITION_ALWAYS, 0},
    {0x60009a15, 9, CONDITION_ALWAYS, EVENT(Func_020009c0)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_AFTER_END), 9,
     CONDITION_ALWAYS, EVENT(Func_020009e8)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST), 10,
     CONDITION_ALWAYS, 0},
    {0x60009a15, 11, CONDITION_ALWAYS, EVENT(Func_02000a74)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_AFTER_END), 11,
     CONDITION_ALWAYS, EVENT(Func_02000a7c)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 80, 0x1232,
     EVENT(Func_02000b84)},
    {0x9c05, 14, CONDITION_ALWAYS, EVENT(Func_02000af4)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0,
     CONDITION_ALWAYS, EVENT(Func_02000b24)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0,
     CONDITION_ALWAYS, EVENT(Func_02000b54)},
    {TRIGGER_TABLE_END},
};

struct Trigger gMarsLighthouseEntrance_Room6Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {0x51, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {0x51, 5, CONDITION_ALWAYS, 5},
    {0x31, 6, CONDITION_ALWAYS, 6},
    {0x21, 7, CONDITION_ALWAYS, 7},
    {0x31, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {0x51, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 14, CONDITION_ALWAYS, 14},
    {0x51, 15, CONDITION_ALWAYS, 15},
    {TRIGGER_STEP, 9, CONDITION_ALWAYS, EVENT(Func_02000e3c)},
    {TRIGGER_STEP, 16, CONDITION_ALWAYS, EVENT(Func_02000e3c)},
    {TRIGGER_STEP, 35, 0x234, EVENT(Func_02000fa4)},
    {TRIGGER_STEP, 30, CONDITION_ALWAYS, EVENT(Func_02000fd4)},
    {TRIGGER_STEP, 90, CONDITION_ALWAYS,
     EVENT(MarsLighthouseEntrance_SlideOnIce)},
    {TRIGGER_STEP, 13, CONDITION_ALWAYS, EVENT(Func_020029a0)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 13, 0x210,
     EVENT(MarsLighthouseEntrance_StartPoundIceCone)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 14, 0x211,
     EVENT(MarsLighthouseEntrance_StartPoundIceCone)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 15, 0x212,
     EVENT(MarsLighthouseEntrance_StartPoundIceCone)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 16, 0x213,
     EVENT(MarsLighthouseEntrance_StartPoundIceCone)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 17, 0x214,
     EVENT(MarsLighthouseEntrance_StartPoundIceCone)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 18, 0x215,
     EVENT(MarsLighthouseEntrance_StartPoundIceCone)},
    {0x9c05, 12, CONDITION_ALWAYS, EVENT(Func_02000c84)},
    {0x9c05, 13, CONDITION_ALWAYS, EVENT(Func_02000cb4)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 8, 0xa4e, EVENT(Func_02000ce4)},
    {TRIGGER_TABLE_END},
};

struct Trigger gMarsLighthouseEntrance_Room7Triggers[] = {
    {0x51, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {0x51, 3, CONDITION_ALWAYS, 3},
    {0x51, 4, CONDITION_ALWAYS, 4},
    {0x31, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {0x31, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_EXIT, 13, CONDITION_ALWAYS, 13},
    {TRIGGER_EXIT, 14, CONDITION_ALWAYS, 14},
    {TRIGGER_EXIT, 15, CONDITION_ALWAYS, 15},
    {TRIGGER_EXIT, 16, CONDITION_ALWAYS, 16},
    {TRIGGER_STEP, 30, CONDITION_ALWAYS, EVENT(Leader_SlideOnIce)},
    {TRIGGER_STEP, 31, CONDITION_ALWAYS, EVENT(Func_02001138)},
    {0x202, 35, CONDITION_ALWAYS, EVENT(Func_0200112c)},
    {TRIGGER_TABLE_END},
};

u32 gData_02006868[] = {
    0,
};

s32 gFieldEffect_DurationScripts[] = {
    (s32)gFieldEffect_ShortScript,
    (s32)gFieldEffect_MediumScript,
    (s32)gFieldEffect_LongScript,
};
#undef Func_02004160
#undef MarsLighthouseEntrance_BurstIceSheet
#undef MarsLighthouseEntrance_PushStatueIntoFlames
