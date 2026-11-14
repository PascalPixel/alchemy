/// @file
/// Mars Lighthouse's lower floors, inside its gates, in the rooms of this
/// overlay: ice cones Pound flattens (MarsLighthouseEntrance_PoundIceCone),
/// ice sheets that burst and ice to Grind (MarsLighthouseEntrance_GrindIce),
/// floors of ice the leader slides across (MarsLighthouseEntrance_SlideOnIce),
/// flame throwers whose flames knock the leader back (Beam_Start,
/// Ride_UpdateCarried), a statue pushed into the flames
/// (MarsLighthouseEntrance_PushStatueIntoFlames) and the teleporters on
/// (MarsLighthouseEntrance_Teleport). The Mars Djinni Fugue and the Mercury
/// Djinni Balm fight here (MarsLighthouseEntrance_FightFugue,
/// MarsLighthouseEntrance_FightBalm).
///
/// Outside the gates (room 1) the storm rains. In the first hall (room 2) Grind
/// on the dead end cracks the ice sheet a floor below (room 3), which Burst
/// then breaks; there the flames are blocked with a statue, a dragon statue
/// Blazed shoots a fireball that cracks another sheet, and the first
/// teleport circle waits. Past the black ice (room 4) a second dragon statue
/// fires at the ice (room 5), where Reveal shows a hidden circle. In room 6 a
/// dragon face wakes and moves left while the party crosses the ice ahead of
/// it; cracked floors (room 7) drop the leader to the floor below.

#include "flags.h"
#include "module/scene/ride.h"
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
#include "system/main_routine_work.h"
#include "actor.h"
#include "field/field_sequence_event.h"
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
#include "field/field_sequence_event.h"
#include "battle/battle_unit.h"
#include "system/vector3.h"
#include "fixed_math.h"
#include "graphics/display_transition.h"
#include "module/system/system.h"
#include "field_sprite.h"
#include "macros.h"
#include "system/input.h"
#include "system/heap_release.h"
#include "field/cell_target.h"
#include "shared/tables.h"
#include "module/field/field_effect.h"
#include "system/dma.h"
#include "objcmd.h"

/// The Mars Star is set: the lighthouse has turned to fire and its ice is gone.
#define FLAG_MARS_LIGHTHOUSE_THAWED 0xa30
#define FLAG_TELEPORTED_ONCE 0xa36
/// Grind on the dead end cracked the ice sheet a floor below.
#define FLAG_ICE_GROUND 0xa3e
#define FLAG_ROOM3_SHEET_BURST 0xa3f
#define FLAG_ROOM3_SECOND_SHEET_BURST 0xa47
#define FLAG_ROOM3_SECOND_SHEET_CRACKED 0xa48 ///< a fireball cracked it
#define FLAG_ROOM5_ICE_MELTED 0xa49 ///< the fireball melted the ice in its path
#define FLAG_ROOM5_SHEET_CRACKED 0xa4c
#define FLAG_ROOM5_SHEET_BURST 0xa4d
#define FLAG_ROOM6_STATUE_BLOCKS_FLAMES 0xa4e
#define FLAG_ROOM3_STATUE_BLOCKS_FLAMES 0xa4f
#define FLAG_ROOM3_SHEET_CRACKED FLAG_SCENE_TEMP_30
#define FLAG_ROOM3_SECOND_SHEET_SHOWN FLAG_SCENE_TEMP_31
#define FLAG_ROOM5_SHEET_SHOWN FLAG_SCENE_TEMP_32
#define FLAG_DRAGON_FACE_AWAKE FLAG_SCENE_TEMP_34
#ifndef TLA_MARS_LIGHTHOUSE_ENTRANCE_RIDE_H
#define TLA_MARS_LIGHTHOUSE_ENTRANCE_RIDE_H

#include "types.h"
#include "system/ram_buffer.h"
#include "system/frame.h"
#include "actor.h"

struct RideRow {
  u8 unk00[20];
  s16 a;
  u8 unk16[2];
  s16 b;
  s16 c;
  u8 unk1c[4];
  s32 d;
};

extern u8 gInventorySnapshot[];

void Actor_SetSpriteColorMode(struct Actor *object, s32 value);

#endif

s32 MarsLighthouseEntrance_SetUpStatue(struct Actor *object) {
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

s32 MarsLighthouseEntrance_LightDragonFace(void) {
  Ride_SetEntryState(0, 65);
  return 0;
}

const struct ActorPlacement *Scene_GetPlacements(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_MarsLighthouseEntrance1)
    return gMarsLighthouseEntrance_Room1Placements;
  if (gMainRoutineWork.scene == (s32)&SceneId_MarsLighthouseEntrance2)
    return gMarsLighthouseEntrance_Room2Placements;
  if (gMainRoutineWork.scene == (s32)&SceneId_MarsLighthouseEntrance3)
    return gMarsLighthouseEntrance_Room3Placements;
  if (gMainRoutineWork.scene == (s32)&SceneId_MarsLighthouseEntrance4)
    return gMarsLighthouseEntrance_Room4Placements;
  if (gMainRoutineWork.scene == (s32)&SceneId_MarsLighthouseEntrance5)
    return gMarsLighthouseEntrance_Room5Placements;
  if (gMainRoutineWork.scene == (s32)&SceneId_MarsLighthouseEntrance6)
    return gMarsLighthouseEntrance_Room6Placements;
  if (gMainRoutineWork.scene == (s32)&SceneId_MarsLighthouseEntrance7)
    return gMarsLighthouseEntrance_Room7Placements;
  return gMarsLighthouseEntrance_Placements;
}

// XXX: what Blaze does otherwise once the field work's unkd38 is set (its
// mode 3 for 2 when it finds no torch); the rest of blaze.c's reading of the
// mode would settle it.
/// Unused: as the dragon hall's, readies Blaze for a dragon statue.
UNUSED void MarsLighthouseEntrance_BeginBlazeOnStatue(void) {
  struct FieldWork *event = Ram_HeapSlots->fieldWork;

  event->unkd38 = 1;
}

extern struct MapCell gMapCellBuffer[];

#include "shared/map_cell_get.h"

#include "shared/map_cell_swap.h"

#include "../field/object/set_sprite_priority.c"

#include "text/msg_ids.h"

void MapGrid_SetCellHeight();
void Teleport_PlayWhirl(void);
void Teleport_PlaySparks(void);
extern u16 gMarsLighthouseEntrance_HoleMaskVram;
extern u32 gMarsLighthouseEntrance_HoleMaskSprite[3];
extern s16 gMarsLighthouseEntrance_DragonFireTimer;
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
    object->scaleY = Q16_ONE;
    Event_SetFlag(object->timer);
    MarsLighthouseEntrance_SetIceConeCells(Actor_Get(holder->objectId));
    object->x = object->y = object->z = 0;
  }
}

/* VIEW(mars_lighthouse_entrance): routine MarsLighthouseEntrance_PoundIceCone
 * as void MarsLighthouseEntrance_PoundIceCone(void); the definition has void
 * MarsLighthouseEntrance_PoundIceCone(s32 mode, s32 index); tried as the
 * definition's void MarsLighthouseEntrance_PoundIceCone(s32 mode, s32 index),
 * which the object already declares, it does not compile in tla-ja:
 * games/tla/src/scene/mars_lighthouse_entrance.c:258: too few arguments to
 * function `MarsLighthouseEntrance_PoundIceCone'. */
extern void MarsLighthouseEntrance_PoundIceCone__mars_lighthouse_entrance(
    void) __asm__("MarsLighthouseEntrance_PoundIceCone");

void MarsLighthouseEntrance_StartPoundIceCone(void) {
  MarsLighthouseEntrance_PoundIceCone__mars_lighthouse_entrance();
}

extern u32 gMarsLighthouseEntrance_IceChipScript[];

/// Throws chips of ice from an actor as its ice cracks or breaks.
void MarsLighthouseEntrance_ScatterIceChips(s32 id) {
  struct Actor *source = Actor_Get(id);
  s32 i;

  i = 5;
next:
  {
    s32 x = source->x + (Random16() << 4) - (Random16() << 4);
    struct Actor *object =
        Func_02004bd8_(0x145, x, (Random16() << 3) + source->y + px(4),
                       source->z + (Random16() << 4) - (Random16() << 4));

    if (object != NULL) {
      s32 angle;
      s32 radius;
      s32 scale;

      angle = Random16();
      radius = Random16();
      scale = (Random16() >> 1) + 0x4000;
      Actor_StartScript(object, gMarsLighthouseEntrance_IceChipScript);
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
      object->bounce = Q16_ONE / 2;
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
  MarsLighthouseEntrance_ScatterIceChips(10);
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
  gFieldWork->fieldMenu = 1;
}

/// Teleport on the circle: the first time the camera follows the leader across;
/// after, the leader leaves at once.
void MarsLighthouseEntrance_Teleport(void) {
  if (!Flag_IsSet(FLAG_TELEPORTED_ONCE)) {
    Teleport_StartShimmer();
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    Teleport_PlaySparks();
    WORD_AT(Actor_Get(gMainRoutineWork.currentOwner), 108) = 0;
    FieldActor_SetSpriteColor(Actor_Get(gMainRoutineWork.currentOwner), 0);
    Func_02004cd8_(gMainRoutineWork.currentOwner, px(688), px(360));
    FieldCamera_MoveTo(688, CAMERA_KEEP, 360, 1);
    FieldCamera_WaitForArrival();
    Teleport_PlayWhirl();
    FieldEvent_Wait(40);
    Teleport_StopShimmer();
    FieldPsynergy_FadeFromCastColor();
    System_Wait(10);
    Event_End();
    Flag_Set(FLAG_TELEPORTED_ONCE);
  } else {
    Teleport_StartShimmer();
    Teleport_PlaySparks();
    {
      u8 *party = (u8 *)&gMainRoutineWork;

      party[0x2c2] = 2;
    }
    FieldEvent_SetExitRequest(18);
    FieldEvent_Wait(40);
    Teleport_StopShimmer();
  }
}

void MarsLighthouseEntrance_TeleportFromRoom3(void) {
  Teleport_StartShimmer();
  Teleport_PlaySparks();
  {
    u8 *party = (u8 *)&gMainRoutineWork;

    party[0x2c2] = 2;
  }
  FieldEvent_SetExitRequest(17);
  FieldEvent_Wait(40);
  Teleport_StopShimmer();
}

/// Burst on the cracked ice sheet, actor 18: a flash, the sheet gone, chips
/// flying (flag FLAG_ROOM3_SHEET_BURST).
void MarsLighthouseEntrance_BurstIceSheet(s32 state, s32 frame) {
  if (state == 1) {
    Event_PlaySound(SE_574);
    PaletteFade_ApplyTarget(0x10000, 0);
    PaletteFade_Interpolate(60);
    Func_02004c30_(47, 94, 10, 6, 47, 100);
    Func_02004c28_(110, 40, 12, 4, 46, 40);
    Actor_SetPosition(18, 0, 0);
    Flag_Set(FLAG_ROOM3_SHEET_BURST);
  }
  if (frame > 96 && frame < 272 && frame % 30 == 0)
    MarsLighthouseEntrance_ScatterIceChips(18);
  if (frame == 276) {
    PaletteFade_ApplyTarget(0x7fff, 0);
    PaletteFade_Interpolate(20);
  }
}

/// Blaze on the dragon statue: it shoots a fireball at the ice sheet.
void MarsLighthouseEntrance_BlazeRoom3Statue(void) {
  MarsLighthouseEntrance_ShootFireballAtRoom3IceSheet();
}

/// The camera follows the fireball to the ice sheet it cracks.
void MarsLighthouseEntrance_WatchFireballCrackIce(void) {
  gFieldWork->psynergy = 0;
  Flag_Clear(FLAG_MENU_OPEN);
  Flag_Clear(0x104);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_SetSpeed(1);
  Camera_SetBounds(px(168), 0, px(664), 1);
  FieldCamera_WaitForArrival();
  System_Wait(10);
  FieldCamera_SetSpeed(4);
  Camera_SetBounds(px(472), 0, px(664), 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(90);
  Event_End();
}

void MarsLighthouseEntrance_DrawRoom3CrackedSheet(void) {
  MapGrid_CopyCellAttributes(14, 55, 14, 3, 14, 41);
  MapGrid_CopyCellsLayered(64, 64, 14, 3, 78, 41);
  MapGrid_CopyCellsLayered(64, 64, 14, 3, 14, 104);
  MapGrid_CopyCellAttributes(64, 64, 14, 3, 78, 41);
  MapGrid_CopyCellAttributes(64, 64, 14, 3, 14, 105);
}

/// The fireball reaches the ice sheet and cracks it.
void MarsLighthouseEntrance_CrackRoom3IceSheet(void) {
  struct Actor *object = Actor_Get(20);

  if (!Flag_IsSet(FLAG_ROOM3_SECOND_SHEET_BURST)) {
    Event_PlaySound(SE_573);
    Flag_Set(FLAG_ROOM3_SECOND_SHEET_SHOWN);
    object->x = S16_AT(object, 100) << 16;
    object->z = S16_AT(object, 102) << 16;
    Flag_Set(FLAG_ROOM3_SECOND_SHEET_CRACKED);
    MarsLighthouseEntrance_DrawRoom3CrackedSheet();
  }
}

#define DEFINE_BURST_ICE_SHEET(name, n1, n2, n3, n4, n5, n6, n7, n8, n9)       \
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
      MarsLighthouseEntrance_ScatterIceChips(n8);                              \
    if (frame == 276) {                                                        \
      PaletteFade_ApplyTarget(0x7fff, 0);                                      \
      PaletteFade_Interpolate(20);                                             \
    }                                                                          \
  }

DEFINE_BURST_ICE_SHEET(MarsLighthouseEntrance_BurstRoom3IceSheet, 28, 92, 8,
                       100, 92, 38, 8, 20, FLAG_ROOM3_SECOND_SHEET_BURST)

/// Move on the statue, actor 8, to cell x 20: it blocks the last flame thrower
/// (flag FLAG_ROOM3_STATUE_BLOCKS_FLAMES).
void MarsLighthouseEntrance_PushStatueIntoFlames(s32 unused, s32 id) {
  if (id == 8) {
    struct Actor *object = Actor_Get(8);

    if (object->x >> 20 == 20) {
      FieldEvent_Wait(5);
      object->z += px(1);
      BYTE_AT(object, 98) = 1;
      Flag_Set(FLAG_ROOM3_STATUE_BLOCKS_FLAMES);
    }
  }
}

void MarsLighthouseEntrance_BlockRetreatAtChest(void) {
  GameFlag_Set(FLAG_RETREAT_BLOCKED);
}

void MarsLighthouseEntrance_TeleportFromRoom4(void) {
  Teleport_StartShimmer();
  Teleport_PlaySparks();
  {
    u8 *party = (u8 *)&gMainRoutineWork;

    party[0x2c2] = 2;
  }
  FieldEvent_SetExitRequest(14);
  FieldEvent_Wait(40);
  Teleport_StopShimmer();
}

/// Sends object id from column 11 to x 152, then from column 10 to x 200,
/// both at z 792, through Actor_SetMoveTarget.
/// Move sends the statue sliding over the ice to the next stop.
void MarsLighthouseEntrance_SlideStatueOnIce(s32 unused, s32 id) {
  struct Actor *object = Actor_Get(id);

  if (object->x >> 20 == 11) {
    s32 y;

    object->speedLimit = px(1) / 4;
    object->acceleration = px(1) / 8;
    y = object->y;
    Actor_SetMoveTarget(object, px(152), y, px(792));
  }
  if (object->x >> 20 == 10) {
    s32 y;

    object->speedLimit = px(1) / 4;
    object->acceleration = px(1) / 8;
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

void MarsLighthouseEntrance_BlazeRoom5WestStatue(void) {
  FieldCamera_SetSpeed(1);
  FieldCamera_MoveTo(88, CAMERA_KEEP, 696, 1);
  MarsLighthouseEntrance_ShootFireballAtRoom5IceSheet();
}

void MarsLighthouseEntrance_WatchFireballCrackRoom5Ice(void) {
  gFieldWork->psynergy = 0;
  Flag_Clear(FLAG_MENU_OPEN);
  Flag_Clear(0x104);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(120);
  Event_End();
}

void MarsLighthouseEntrance_CrackRoom5IceSheet(void) {
  struct Actor *object = Actor_Get(12);

  if (!Flag_IsSet(FLAG_ROOM5_SHEET_BURST)) {
    Event_PlaySound(SE_573);
    Flag_Set(FLAG_ROOM5_SHEET_SHOWN);
    object->x = S16_AT(object, 100) << 16;
    object->z = S16_AT(object, 102) << 16;
    Flag_Set(FLAG_ROOM5_SHEET_CRACKED);
  }
}

void MarsLighthouseEntrance_BlazeRoom5EastStatue(void) {
  MarsLighthouseEntrance_ShootFireballThroughIce();
}

void MarsLighthouseEntrance_WatchFireballMeltIce(void) {
  gFieldWork->psynergy = 0;
  Flag_Clear(FLAG_MENU_OPEN);
  Flag_Clear(0x104);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_SetSpeed(1);
  Camera_SetBounds(px(872), -1, px(168), 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(40);
  FieldCamera_MoveTo(872, CAMERA_KEEP, 264, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(120);
  Event_End();
}

void MarsLighthouseEntrance_TeleportFromRoom5(void) {
  Teleport_StartShimmer();
  Teleport_PlaySparks();
  {
    u8 *party = (u8 *)&gMainRoutineWork;

    party[0x2c2] = 2;
  }
  FieldEvent_SetExitRequest(14);
  FieldEvent_Wait(40);
  Teleport_StopShimmer();
}

/// Reveal shows the hidden teleport circle.
void MarsLighthouseEntrance_RevealTeleportCircle(void) {
  MapGrid_CopyCells(53, 30, 4, 3, 53, 48);
  MapGrid_CopyCellsLayered(117, 30, 4, 3, 117, 48);
}

void MarsLighthouseEntrance_HideTeleportCircle(void) {
  MapGrid_CopyCells(53, 40, 4, 3, 53, 48);
  MapGrid_CopyCellsLayered(117, 45, 4, 3, 117, 48);
}

DEFINE_BURST_ICE_SHEET(MarsLighthouseEntrance_BurstRoom5IceSheet, 12, 95, 6,
                       101, 76, 40, 5, 12, FLAG_ROOM5_SHEET_BURST)

/// The fireball melts the ice in its path and opens the way south.
void MarsLighthouseEntrance_MeltIceInPath(void) {
  MapGrid_CopyCellAttributes(35, 11, 3, 6, 53, 11);
  MapGrid_CopyCellsLayered(35, 75, 3, 6, 117, 11);
  MapGrid_CopyCellsLayered(35, 75, 3, 6, 53, 75);
  Flag_Set(FLAG_ROOM5_ICE_MELTED);
}

void MarsLighthouseEntrance_ClearFireballRow(void) {
  MapGrid_CopyCellAttributes(25, 9, 11, 3, 26, 9);
}

void MarsLighthouseEntrance_TeleportFromRoom6West(void) {
  Teleport_StartShimmer();
  Teleport_PlaySparks();
  {
    u8 *party = (u8 *)&gMainRoutineWork;

    party[0x2c2] = 2;
  }
  FieldEvent_SetExitRequest(12);
  FieldEvent_Wait(40);
  Teleport_StopShimmer();
}

void MarsLighthouseEntrance_TeleportFromRoom6East(void) {
  Teleport_StartShimmer();
  Teleport_PlaySparks();
  {
    u8 *party = (u8 *)&gMainRoutineWork;

    party[0x2c2] = 2;
  }
  FieldEvent_SetExitRequest(13);
  FieldEvent_Wait(40);
  Teleport_StopShimmer();
}

/// Move on the statue to column 5 sets it in the flames' way.
void MarsLighthouseEntrance_PushStatueIntoRoom6Flames(s32 unused, s32 id) {
  if (id == 8) {
    struct Actor *object = Actor_Get(8);

    if (object->x >> 20 == 5) {
      FieldEvent_Wait(5);
      object->z += px(1);
      Flag_Set(FLAG_ROOM6_STATUE_BLOCKS_FLAMES);
    }
  }
}

void MarsLighthouseEntrance_DrawHoleMask(void) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s32 *camera = (s32 *)((u8 *)map + 228);
  s32 x = camera[0] & 0xffff0000;
  s32 cameraZ = camera[1] & 0xffff0000;
  s32 originY = map->origin[1];
  s32 tile =
      gResourceTableEntries[gMarsLighthouseEntrance_HoleMaskVram].offset >> 5;
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
  word = gMarsLighthouseEntrance_HoleMaskSprite;
  *word++ = 0;
  *word++ = sy | (sx << 16) | 0x80008800;
  *word = tile | 0x400;
  Oam_AddSpriteAtDepthClamped(gMarsLighthouseEntrance_HoleMaskSprite, depth);
}

/// Fills 0x100 bytes with colour 1, loads them into a VRAM block of their own
/// and starts MarsLighthouseEntrance_DrawHoleMask.
/// Masks the leader's sprite as he drops through the floor.
void MarsLighthouseEntrance_StartHoleMask(void) {
  u32 *buffer = Heap_Alloc(0x100);
  u32 *word = buffer;
  s32 count = 63;

fill:
  *word++ = 0x11111111;
  if (--count >= 0)
    goto fill;
  gMarsLighthouseEntrance_HoleMaskVram = VramCache_FindFreeEntry();
  Vram_Load(gMarsLighthouseEntrance_HoleMaskVram, 0x100, buffer);
  Heap_Release(buffer);
  Task_AddFrame(MarsLighthouseEntrance_DrawHoleMask, TASK_PASS_VBLANK);
}

#define SET_SPRITE_WINDOW_LAYERS()                                             \
  do {                                                                         \
    u16 winout = 0xf1f;                                                        \
                                                                               \
    REG_WINOUT = winout;                                                       \
  } while (0)

/// The leader shrinks and sinks over 40 frames, then the screen transition
/// ends.
/// The leader drops through a hole in the floor to the room below.
void MarsLighthouseEntrance_FallIntoHole(s32 arg) {
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
    MarsLighthouseEntrance_StartHoleMask();
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
  gFieldWork->transition = 0x209;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(arg);
}

extern s32 gMarsLighthouseEntrance_DragonFaceGoesLeft[];
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
 * Func_02002c2c(struct Actor *object) {; tried as the definition's void
 * Func_02002c2c(struct Actor *object), which the object already declares, it
 * does not compile in tla-ja:
 * games/tla/src/scene/mars_lighthouse_entrance.c:1630: conflicting types for
 * `Func_02002c2c'. */
void Func_02002c2c__mars_lighthouse_entrance(void) __asm__("Func_02002c2c");
#define Func_02002c2c Func_02002c2c__mars_lighthouse_entrance

/// The dragon face wakes and begins its slow way left, breathing fire.
void MarsLighthouseEntrance_WakeDragonFace(void) {
  Event_PlaySound(SE_584);
  Flag_Set(FLAG_NO_PLACE_TO_SAVE);
  Actor_SetScript(11, gMarsLighthouseEntrance_DragonFaceGoesLeft);
  Ride_SetEntryState(0, 3);
  Flag_Set(FLAG_DRAGON_FACE_AWAKE);
}

void MarsLighthouseEntrance_SlideFromEdge(void) {
  Leader_SlideOnIceFromEdge();
  MarsLighthouseEntrance_SlideIfRiding();
}

void GameFlag_SetByte(s32 flag, s32 value);

/// The leader drops through a cracked floor tile.
void MarsLighthouseEntrance_FallThroughCrack(const struct MapCell *cell) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  s32 z = leader->z;
  s32 i = leader->x >> 20;

  GameFlag_SetByte(0x340, i);
  GameFlag_SetByte(0x348, z >> 20);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_SetSpriteFlags(Actor_Get(gMainRoutineWork.currentOwner), 0);
  leader->update = 0;
  FieldActor_ShowEmote(gMainRoutineWork.currentOwner,
                       EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
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
    MarsLighthouseEntrance_StartHoleMask();
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
  gFieldWork->transition = 0x209;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(11);
}

void MarsLighthouseEntrance_JumpOrSlide(void) {
  Field_TryJumpForward();
  Leader_SlideOnIceAhead();
}

void MarsLighthouseEntrance_CheckCrackedFloor(void) {
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
    MarsLighthouseEntrance_FallThroughCrack(cell + 128);
}

const struct Trigger *Scene_GetEvents(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_MarsLighthouseEntrance1)
    return gMarsLighthouseEntrance_Room1Triggers;
  if (gMainRoutineWork.scene == (s32)&SceneId_MarsLighthouseEntrance2)
    return gMarsLighthouseEntrance_Room2Triggers;
  if (gMainRoutineWork.scene == (s32)&SceneId_MarsLighthouseEntrance3)
    return gMarsLighthouseEntrance_Room3Triggers;
  if (gMainRoutineWork.scene == (s32)&SceneId_MarsLighthouseEntrance4)
    return gMarsLighthouseEntrance_Room4Triggers;
  if (gMainRoutineWork.scene == (s32)&SceneId_MarsLighthouseEntrance5)
    return gMarsLighthouseEntrance_Room5Triggers;
  if (gMainRoutineWork.scene == (s32)&SceneId_MarsLighthouseEntrance6)
    return gMarsLighthouseEntrance_Room6Triggers;
  if (gMainRoutineWork.scene == (s32)&SceneId_MarsLighthouseEntrance7)
    return gMarsLighthouseEntrance_Room7Triggers;
  return gMarsLighthouseEntrance_Triggers;
}

#include "shared/actor_freeze.h"
#undef SET_SPRITE_WINDOW_LAYERS

extern u32 gMarsLighthouseEntrance_DragonFireOn[];

/// The dragon face breathes fire for a while and stops, in turn.
void MarsLighthouseEntrance_PulseDragonFire(void) {
  s16 *state = &gMarsLighthouseEntrance_DragonFireOn;

  if (gMarsLighthouseEntrance_DragonFireTimer < 60) {
    if (*state == 0) {
      Ride_SetEntryState(0, 3);
      *state = 1;
    }
  } else {
    if (((struct HalfwordField *)state)->value == 1) {
      Ride_SetEntryState(0, 67);
      *state = 0;
    }
  }
  if (++gMarsLighthouseEntrance_DragonFireTimer > 300)
    gMarsLighthouseEntrance_DragonFireTimer = 0;
}

void MarsLighthouseEntrance_FlickerDragonFace(struct Actor *object) {
  switch (gFrameCount & 7) {
  case 0:
    Actor_SetSpriteColorMode(object, 0);
    break;
  case 6:
    Actor_SetSpriteColorMode(object, 10);
    break;
  }
}

/// An ice cone the dragon face reaches spins and flies off.
void MarsLighthouseEntrance_FlingIceCone(struct Actor *actor) {
  struct Sprite020012e4 *sprite = actor->sprite;

  sprite->unk12 += 0x800;
  if (++actor->timer == 8)
    actor->velocityY = px(4);
  if (actor->timer > 9) {
    actor->x += px(4);
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

void MarsLighthouseEntrance_WaitForDragonFace(struct Actor *object) {
  if (ABS(object->x - object->watchedObject->x) > 0x200000)
    return;
  object->timer = 0;
  object->update = (void (*)(void))MarsLighthouseEntrance_FlingIceCone;
}

void MarsLighthouseEntrance_StopSlideWhileRiding(void) {
  if (Ride_IsActive())
    Leader_SetSlideStopped(1);
}

void Beam_Start(u16 *targets, u16 *stops, s32 a2, s32 a3);
void Ride_SetEntryCount(s32 index, s16 value);
void Palette_SetColorsFade(u16 *slots, u16 *colours, u16 *a, u16 *b);
void MarsLighthouseEntrance_ShootRoom6UpperFireball(void);
extern u32 gMarsLighthouseEntrance_DragonFaceAtLeft[];
extern s16 gMarsLighthouseEntrance_Room2IceCones[];
extern u16 gMarsLighthouseEntrance_Room3BeamTargets[];
extern u16 gMarsLighthouseEntrance_Room3BeamStops[];
extern s16 gMarsLighthouseEntrance_Room3IceCones[];
extern u16 gMarsLighthouseEntrance_Room4BeamTargets[];
extern u16 gMarsLighthouseEntrance_Room4BeamStops[];
extern s16 gMarsLighthouseEntrance_Room4IceCones[];
extern u16 gMarsLighthouseEntrance_Room6UpperBeamTargets[];
extern u16 gMarsLighthouseEntrance_Room6UpperBeamStops[];
extern u16 gMarsLighthouseEntrance_Room6LowerBeamTargets[];
extern u16 gMarsLighthouseEntrance_Room6LowerBeamStops[];
extern s16 gMarsLighthouseEntrance_Room6IceCones[];
extern s16 gMarsLighthouseEntrance_FadeSlots[];
extern s16 gMarsLighthouseEntrance_FadeColorsFrom[];
extern s16 gMarsLighthouseEntrance_FadeColorsTo[];
extern u32 gMarsLighthouseEntrance_FadeOrder[];

s32 Scene_Initialize(void) {
  s32 scene = gMainRoutineWork.scene;
  s32 entrance = gMainRoutineWork.entrance;
  struct Actor *object;
  s32 i;

  // XXX: Data_02006878 is cleared here and read nowhere in this overlay; a
  // watch on the word in the running game would say what else reads it.
  Data_02006878 = 0;
  gFieldWork->transition = 0x204;
  Event_SetFlag(FLAG_MARS_LIGHTHOUSE_LIT);
  Ride_Create();
  if (scene == (s32)&SceneId_MarsLighthouseEntrance1) {
    Weather_StartRain(1);
  } else if (scene == (s32)&SceneId_MarsLighthouseEntrance2) {
    FieldEffect_StartBlankOverlay(0);
#if !TLA_PATCH_1
    FieldActor_SetSpritePriority(11, 3);
#endif
    Actor_Freeze(10);
    if (Event_GetFlag(FLAG_MARS_LIGHTHOUSE_THAWED)) {
      MapGrid_CopyCellsLayered(35, 41, 24, 10, 3, 53);
      MapGrid_CopyCellsLayered(35, 105, 24, 10, 3, 117);
    }
    MarsLighthouseEntrance_PlaceIceCones(
        (s32)gMarsLighthouseEntrance_Room2IceCones);
    if (Event_GetFlag(FLAG_ICE_GROUND))
      MapGrid_CopyCellsLayered(50, 125, 3, 3, 39, 85);
  } else if (scene == (s32)&SceneId_MarsLighthouseEntrance3) {
    FieldEffect_StartBlankOverlay(0);
    Beam_Start((s32)gMarsLighthouseEntrance_Room3BeamTargets,
               (s32)gMarsLighthouseEntrance_Room3BeamStops, 0, 88);
    MarsLighthouseEntrance_PlaceIceCones(
        (s32)gMarsLighthouseEntrance_Room3IceCones);
    if (Event_GetFlag(FLAG_ROOM3_SHEET_BURST)) {
      MapGrid_CopyCellsLayered(47, 94, 10, 6, 47, 100);
      MapGrid_CopyCellAttributes(110, 40, 12, 4, 46, 40);
      Actor_SetPosition(18, 0, 0);
    } else if (Event_GetFlag(FLAG_ICE_GROUND)) {
      Event_SetFlag(FLAG_ROOM3_SHEET_CRACKED);
      Actor_Get(18)->priorityFlags |= 2;
    } else {
      Actor_SetPosition(18, 0, 0);
    }
    Actor_Get(20)->priorityFlags |= 2;
    FieldActor_SetSpritePriority(20, 2);
    if (Event_GetFlag(FLAG_ROOM3_SECOND_SHEET_BURST)) {
      MapGrid_CopyCellsLayered(28, 92, 8, 8, 28, 100);
      MapGrid_CopyCellAttributes(92, 38, 8, 8, 28, 38);
      MarsLighthouseEntrance_DrawRoom3CrackedSheet();
      Actor_SetPosition(20, 0, 0);
    } else if (Event_GetFlag(FLAG_ROOM3_SECOND_SHEET_CRACKED)) {
      MarsLighthouseEntrance_DrawRoom3CrackedSheet();
      Event_SetFlag(FLAG_ROOM3_SECOND_SHEET_SHOWN);
    } else {
      Actor_SetPosition(20, 0, 0);
    }
    if (Event_GetFlag(FLAG_ROOM3_STATUE_BLOCKS_FLAMES)) {
      Actor_SetPosition(8, 328, 120);
      ((u8 *)Actor_Get(8))[98] = 1;
    }
    if (Event_GetFlag(FLAG_MARS_LIGHTHOUSE_THAWED))
      Actor_SetPosition(19, 0, 0);
    else
      Actor_SetPosition(15, 0, 0);
    if (entrance == 20) {
      Event_Start();
      FieldEvent_ResolvePendingActions(0);
      FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
      MapGrid_SetShake(0, 2, 1);
      Actor_SetPosition(gMainRoutineWork.currentOwner, 0, 0);
      Actor_SetPosition(18, 0, 0);
      Actor_Get(18)->priorityFlags |= 2;
      Event_StartScreenTransition();
      Event_WaitTransitionFrames();
      object = Actor_Get(18);
      Actor_SetPosition(18, object->timer, object->unk66);
      Event_PlaySound(SE_573);
      i = 2;
      do {
        MarsLighthouseEntrance_ScatterIceChips(18);
        System_Wait(1);
        i--;
      } while (i >= 0);
      Event_SetFlag(FLAG_ICE_GROUND);
      MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
      FieldEvent_Wait(120);
      FieldEvent_SetExitRequest(20);
      Event_SetFlag(FLAG_KEEP_PARTY_POSITION);
    }
  } else if (scene == (s32)&SceneId_MarsLighthouseEntrance4) {
    FieldEffect_StartBlankOverlay(0);
    if (entrance == 10 || entrance == 1) {
      Beam_Start((s32)gMarsLighthouseEntrance_Room4BeamTargets,
                 (s32)gMarsLighthouseEntrance_Room4BeamStops,
                 (s32)MarsLighthouseEntrance_ShootRoom4Fireball, 88);
      Ride_SetEntryCount(0, 1);
    }
    MarsLighthouseEntrance_PlaceIceCones(
        (s32)gMarsLighthouseEntrance_Room4IceCones);
  } else if (scene == (s32)&SceneId_MarsLighthouseEntrance5) {
    FieldEffect_StartBlankOverlay(0);
    Actor_Get(12)->priorityFlags |= 2;
    if (Event_GetFlag(FLAG_ROOM5_SHEET_BURST)) {
      MapGrid_CopyCellsLayered(12, 95, 8, 6, 12, 101);
      MapGrid_CopyCellAttributes(76, 40, 8, 5, 12, 40);
      Actor_SetPosition(12, 0, 0);
    } else if (Event_GetFlag(FLAG_ROOM5_SHEET_CRACKED)) {
      Event_SetFlag(FLAG_ROOM5_SHEET_SHOWN);
    } else {
      Actor_SetPosition(12, 0, 0);
    }
    if (Event_GetFlag(FLAG_ROOM5_ICE_MELTED))
      MarsLighthouseEntrance_MeltIceInPath();
  } else if (scene == (s32)&SceneId_MarsLighthouseEntrance6) {
    FieldEffect_StartBlankOverlay(0);
    if (entrance == 2 || entrance == 4 || entrance == 6 || entrance == 13) {
      Beam_Start((s32)gMarsLighthouseEntrance_Room6UpperBeamTargets,
                 (s32)gMarsLighthouseEntrance_Room6UpperBeamStops,
                 (s32)MarsLighthouseEntrance_ShootRoom6UpperFireball, 88);
      Ride_SetEntryCount(0, 1);
    }
    if ((u32)(entrance - 7) <= 1 || entrance == 11) {
      struct Actor *target;

      Beam_Start((s32)gMarsLighthouseEntrance_Room6LowerBeamTargets,
                 (s32)gMarsLighthouseEntrance_Room6LowerBeamStops,
                 (s32)MarsLighthouseEntrance_ShootRoom6LowerFireball, 88);
      Ride_SetEntryCount(0, 1);
      if (entrance == 11) {
        Actor_SetScript(11, gMarsLighthouseEntrance_DragonFaceAtLeft);
        Ride_SetEntryState(0, 3);
        Task_AddFrame(MarsLighthouseEntrance_PulseDragonFire, 0x480);
        Event_SetFlag(FLAG_NO_PLACE_TO_SAVE);
      }
      Task_AddFrame(MarsLighthouseEntrance_StopSlideWhileRiding, 0x480);
      Actor_Get(11)->update =
          (void (*)(void))MarsLighthouseEntrance_FlickerDragonFace;
      Ride_SetMode(1);
      MarsLighthouseEntrance_PlaceIceCones(
          (s32)gMarsLighthouseEntrance_Room6IceCones);
      target = Actor_Get(20);
      Actor_Get(13)->update =
          (void (*)(void))MarsLighthouseEntrance_WaitForDragonFace;
      Actor_Get(14)->update =
          (void (*)(void))MarsLighthouseEntrance_WaitForDragonFace;
      Actor_Get(15)->update =
          (void (*)(void))MarsLighthouseEntrance_WaitForDragonFace;
      Actor_Get(16)->update =
          (void (*)(void))MarsLighthouseEntrance_WaitForDragonFace;
      Actor_Get(17)->update =
          (void (*)(void))MarsLighthouseEntrance_WaitForDragonFace;
      Actor_Get(18)->update =
          (void (*)(void))MarsLighthouseEntrance_WaitForDragonFace;
      Actor_Get(13)->watchedObject = target;
      Actor_Get(14)->watchedObject = target;
      Actor_Get(15)->watchedObject = target;
      Actor_Get(16)->watchedObject = target;
      Actor_Get(17)->watchedObject = target;
      Actor_Get(18)->watchedObject = target;
    }
    Actor_Get(9)->priorityFlags |= 2;
    if (Event_GetFlag(FLAG_ROOM6_STATUE_BLOCKS_FLAMES))
      Actor_SetPosition(8, 88, 696);
  } else if (scene == (s32)&SceneId_MarsLighthouseEntrance7) {
    FieldEffect_StartBlankOverlay(0);
  }
  if (scene == (s32)&SceneId_MarsLighthouseEntrance6 && entrance == 13)
    Event_ClearFlag(0x104);
  Palette_SetColorsFade((s32)gMarsLighthouseEntrance_FadeSlots,
                        (s32)gMarsLighthouseEntrance_FadeColorsFrom,
                        (s32)gMarsLighthouseEntrance_FadeColorsTo,
                        (s32)gMarsLighthouseEntrance_FadeOrder);
  return 0;
}

extern s32 gMarsLighthouseEntrance_Room6UpperFireballPath[];
extern s32 gMarsLighthouseEntrance_Room6LowerFireballPath[];
extern s32 gMarsLighthouseEntrance_Room4FireballPath[];
extern s32 gMarsLighthouseEntrance_FireballToRoom5IceSheet[];
extern s32 gMarsLighthouseEntrance_FireballThroughIce[];
extern s32 gMarsLighthouseEntrance_FireballToRoom3IceSheet[];
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
 * a2, s32 a3, const void *script); tried as the definition's void
 * Actor_Attach(s32 id, s32 target, s32 kind, s32 a3, const void *script),
 * MarsLighthouseEntrance_ShootRoom6UpperFireball moves: 1 of 11 instructions,
 * MarsLighthouseEntrance_ShootRoom6LowerFireball moves: 1 of 11 instructions,
 * MarsLighthouseEntrance_ShootRoom4Fireball moves: 1 of 12 instructions, 3 more
 * in tla-ja. */
extern s32 Actor_Attach__s1930(s32 id, s32 a1, s32 a2, s32 a3,
                               const void *script) __asm__("Actor_Attach");
void Func_02002c2c(void);

s32 Scene_PrepareMap(void) {
  if (Flag_IsSet(FLAG_MARS_LIGHTHOUSE_THAWED))
    TileMap_ConvertBuffer(
        gMainRoutineWork.map +
            ((s32)&SceneId_MarsLighthouseRoomM3 - (s32)&SceneId_Prox2),
        0x5000000);
  return 0;
}

void MarsLighthouseEntrance_ShootRoom6UpperFireball(void) {
  Actor_Attach__s1930(10, 19, 20, 0,
                      gMarsLighthouseEntrance_Room6UpperFireballPath);
}

void MarsLighthouseEntrance_ShootRoom6LowerFireball(void) {
  Actor_Attach__s1930(12, 19, 20, 0,
                      gMarsLighthouseEntrance_Room6LowerFireballPath);
}

void MarsLighthouseEntrance_ShootRoom4Fireball(void) {
  Actor_Attach__s1930(9, 18, 20, 0x4000,
                      gMarsLighthouseEntrance_Room4FireballPath);
}

void MarsLighthouseEntrance_ShootFireballAtRoom5IceSheet(void) {
  Actor_Attach__s1930(9, 13, 14, 0,
                      gMarsLighthouseEntrance_FireballToRoom5IceSheet);
}

void MarsLighthouseEntrance_ShootFireballThroughIce(void) {
  Actor_Attach__s1930(11, 13, 14, 0x4000,
                      gMarsLighthouseEntrance_FireballThroughIce);
}

void MarsLighthouseEntrance_ShootFireballAtRoom3IceSheet(void) {
  Actor_Attach__s1930(13, 21, 22, 0,
                      gMarsLighthouseEntrance_FireballToRoom3IceSheet);
}

void Ride_UpdateDrift(struct Actor *object);

#include "shared/actor_is_touching.h"

#include "shared/actor_is_beside.h"

#include "shared/ride_update_carried.h"

extern u32 gScriptedPart_Script[];

void Ride_UpdateDrift(struct Actor *object);

#include "shared/actor_reset_scripted_for.h"

#include "shared/ride_spawn_front.h"

#include "shared/ride_update_landing.h"

extern u32 gScriptedPart_Script[];

void Ride_UpdateDrift(struct Actor *object);

/// Stops the object and sets its sprite part, then restarts its script at
/// gScriptedPart_Script. Overlays 6ab, 6ac and 6ad have the same function.
#include "shared/actor_setup_scripted_part.h"

#include "shared/ride_spawn_back.h"

void Func_02001ea8(s32 *position, s32 speed, s32 mode) {
  struct Actor *object =
      Func_02004bd8_(366, position[0], position[1], position[2]);

  if (object != NULL) {
    object->bounce = 0;
    object->gravity = 0;
    object->unk4c = speed;
    Actor_SetupScriptedPart(object, 3);
    object->update = Ride_UpdateLanding;
    if (mode)
      object->unk5c[0] = 2;
    Actor_SelectAnimation(object, 2);
  }
}

void Func_02001ef0(s32 *position, s32 speed, s32 mode) {
  struct Actor *object =
      Func_02004bd8_(366, position[0], position[1], position[2]);

  if (object != NULL) {
    object->bounce = 0;
    object->gravity = 0;
    object->unk4c = -speed;
    Actor_SetupScriptedPart(object, 3);
    object->update = Ride_UpdateLanding;
    if (mode)
      object->unk5c[0] = 2;
    Actor_SelectAnimation(object, 2);
  }
}

#include "shared/actor_is_above.h"

#include "shared/ride_update_drift.h"

#include "shared/actor_setup_scripted_for.h"

#include "shared/ride_spawn_spark.h"

/* VIEW(s20ec): tag struct Actor as struct Actor__s20ec; actor.h has another
 * definition; tried with the definition the rest of the object has, it does not
 * compile in tla-ja: games/tla/src/scene/shared/beam_draw_targets.h:39:
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
 * before it has another declaration; tried as the definition's void
 * Ride_SpawnSpark(s32 *position, s32 speed), it does not compile in tla-ja:
 * games/tla/src/scene/shared/beam_draw_targets.h:68: too many arguments to
 * function `Ride_SpawnSpark'. */
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

#include "shared/beam_draw_targets.h"
#undef IWRAM

#undef Func_02002c2c
void Ride_Create(void) {
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

UNUSED void Ride_Release(void) {
  struct RideWork *work = RIDE_WORK;

  Actor_Release(work->handle);
  work->handle = NULL;
}

void Ride_SetMode(s16 value) { RIDE_WORK->value0e = value; }

void Beam_DrawTargets(void);

#include "shared/beam_start.h"

UNUSED s32 Ride_GetEntryValue(s32 index) {
  return ((struct RideRow *)(gInventorySnapshot + 0x188 + index * 16))->a;
}

UNUSED void Ride_SetEntryValue(s32 index, s16 value) {
  ((struct RideRow *)(gInventorySnapshot + 0x188 + index * 16))->a = value;
}

UNUSED s32 Ride_GetCounter(void) { return RIDE_WORK->counter; }

UNUSED s32 Ride_StepCounter(s32 limit) {
  struct RideWork *work = RIDE_WORK;

  if (!Event_GetFlag(260)) {
    if (++work->counter >= limit)
      work->counter = 0;
  }
  return work->counter;
}

void Ride_SetEntryState(s32 index, s32 value) {
  struct RideWork *work = RIDE_WORK;
  struct RideEntry *entry = &work->entries[index];

  entry->d = (value & 15) << 16;
  entry->b = value & 0xf0;
}

void Ride_SetEntryCount(s32 index, s16 value) {
  RIDE_WORK->entries[index].c = value;
}

UNUSED s32 Ride_GetEntryFlags(s32 index) {
  return ((struct RideRow *)(gInventorySnapshot + 0x188 + index * 16))->b;
}

UNUSED void Ride_SetSlotActor(u32 index, s32 value) {
  struct RideWork *work = RIDE_WORK;

  if (index <= 3)
    work->slots[index].id = value;
}

/* VIEW(s2270): routine Unit_Get as struct BattleUnit *Unit_Get(s32 owner);
 * item_icon.h has void *Unit_Get(s32 owner); tried as the definition's void
 * *Unit_Get(s32 owner), which the object already declares, it does not compile
 * in tla-ja: games/tla/src/scene/shared/party_take_fall_damage.h:12: request
 * for member `hp' in something not a structure or union. */
struct BattleUnit *Unit_Get__s2270(s32 owner) __asm__("Unit_Get");
#define Unit_Get Unit_Get__s2270

#include "shared/party_take_fall_damage.h"

#include "../field/field_effect/advance_spin.c"

#include "shared/actor_blink.h"

#undef Unit_Get
void Party_TakeFallDamage();
void Actor_SelectAnimation(struct Actor *actor, s32 a);
s32 MapGrid_CheckCollision(struct Actor *actor, struct Vector3 *position);
void Actor_SetSpriteColorMode(struct Actor *actor, s32 a);
/* VIEW(s263c): routine Actor_Blink as void Actor_Blink(void);
 * scene/mars_lighthouse_entrance/s2270 has DEFINE_ACTOR_BLINK(Actor_Blink, 10,
 * 4, 0) #undef Beam_Start #undef Unit_Get // ----
 * scene/mars_lighthouse_entrance/s263c.c ---- /// Scene code of overlay 0x6aa,
 * #include "types.h" #include "call.h" #include "actor.h" #include
 * "main_routine_work.h" #include "ram_buffer.h" #include "vector3.h" #include
 * "fixed_math.h" #include "event_inline.h" #include "display_transition.h"
 * #include "field/field_sequence_event.h" #include "field.h" #include
 * "item_icon.h" #include "module/field/field.h" #include
 * "module/scene/mars_lighthouse_entrance.h" #include "module/system/system.h"
 * #include "scene_ids.h" void Party_TakeFallDamage(); tried as the definition's
 * void Actor_Blink(struct Actor *object), it does not compile in tla-ja:
 * games/tla/src/scene/mars_lighthouse_entrance.c:1457: conflicting types for
 * `Actor_Blink'. */
void Actor_Blink__s263c(void) __asm__("Actor_Blink");
#define Actor_Blink Actor_Blink__s263c
extern u8 gInventorySnapshot[];

#define KNOCK_BACK Leader_KnockBack
#define KNOCK_BACK_TAKE_DAMAGE Party_TakeFallDamage
#define KNOCK_BACK_BLINK Actor_Blink
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
 * s32 b); s263c has void Party_TakeFallDamage(); tried as the definition's void
 * Party_TakeFallDamage(void), which the object already declares, it does not
 * compile in tla-ja: games/tla/src/scene/mars_lighthouse_entrance.c:1490: too
 * many arguments to function `Party_TakeFallDamage'. */
extern s32 Party_TakeFallDamage__s2860(s32 a,
                                       s32 b) __asm__("Party_TakeFallDamage");
void Actor_Blink(void);

/// Stepping onto ice (cell behavior 90, as Imil's ice slide is in The Broken
/// Seal): the leader slides until the ice stops.
int MarsLighthouseEntrance_SlideOnIce(void) {
  struct FieldWork *event = Ram_HeapSlots->fieldWork;
  struct RideWork *ride;
  struct Actor *leader;
  // Unused: gives the stack frame its 12 bytes.
  s32 frame[3];

  if (event->transitionActive == 0) {
    ScreenTransition_Start(event->transition, event->transitionFrames);
    event->transitionActive = 1;
  }
  Flag_Clear(FLAG_MENU_OPEN);
  event->psynergy = 0;
  event->fieldMenu = 0;
  event->pauseMenu = 0;
  event->aButton = 0;
  event->encounter = 0;
  /* FAKEMATCH(signature): Party_TakeFallDamage declared returning s32, though
   * it returns nothing. */
  Party_TakeFallDamage__s2860(-10, 0);
  FieldCamera_SetSpeed(7);
  leader = Actor_Get(gMainRoutineWork.currentOwner);
  ride = (struct RideWork *)(gInventorySnapshot + 0x188);
  Flag_Set(FLAG_MENU_OPEN);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Flag_Clear(FLAG_MENU_OPEN);
  leader->update = Actor_Blink;
  leader->facing = 0x4000;
  Actor_SelectAnimation(leader, 49);
  while (ride->car->timer == 0) {
    if (MapGrid_GetCellAttributeLowNibbleOrBump(0, leader->x, leader->z) != 7) {
      leader->x += ((s32 *)ride->car->unk70)[0];
      leader->z += ((s32 *)ride->car->unk70)[2];
    } else {
      if (((s32 *)ride->car->unk70)[0] != 0)
        leader->x = (leader->x & 0xfff00000) + 0x80000;
      if (((s32 *)ride->car->unk70)[2] != 0)
        leader->z = (leader->z & 0xfff00000) + 0x80000;
    }
    ride->active = 0;
    System_Wait(1);
  }
  FieldEvent_Wait(30);
  leader->update = 0;
  Actor_SetSpriteColorMode(leader, 0);
  ride->active = 0;
#ifdef UBFIX
  return 0;
#endif
}

s32 MarsLighthouseEntrance_SlideIfRiding(void) {
  if (RIDE_WORK->active) {
    MarsLighthouseEntrance_SlideOnIce();
    return 1;
  }
  return 0;
}

s32 Ride_IsActive(void) { return RIDE_WORK->active; }

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

extern u32 gMarsLighthouseEntrance_CrumbScript[];

#define WORK ((struct Work *)(gInventorySnapshot + 0x188))

void MarsLighthouseEntrance_ScatterCrumbs(struct Actor *actor) {
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
    y = c * 8 + actor->y + px(4);
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
      Actor_StartScript(object, gMarsLighthouseEntrance_CrumbScript);
      Actor_SetSpriteFlags(object, 0);
      object->velocityY = angle * 4 + 0x20000;
      object->velocityZ = Iwram_MulQ16(radius, Trig_Cos(angle));
      object->velocityX = Iwram_MulQ16(radius, Trig_Sin(angle));
      object->gravity = 0x547a;
      object->scaleX = scale;
      object->scaleY = scale;
      object->acceleration = 0;
      object->bounce = Q16_ONE / 2;
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

extern u32 gEmbers_Script[];

/// Throws two sparks (object kind 325) from (x, y, z), each upward at about
/// speed plus 0x14000 with a random spread, at a random angle and size.
#include "shared/embers_spawn.h"

void Func_02002c2c(struct Actor *object) {
  object->movementState = Event_GetFlag(260);
}

#define ABS(x) ((x) < 0 ? -(x) : (x))

/* VIEW(s2860): tag struct Work020023c4 as struct Work020023c4__s2860; s263c.c
 * has another definition; tried with the definition the rest of the object has,
 * it does not compile in tla-ja:
 * games/tla/src/scene/mars_lighthouse_entrance.c:1751: structure has no member
 * named `unk6c'. */
struct Work020023c4__s2860 {
  u8 unk00[4];
  u16 unk04;
  u8 unk06[4];
  u16 unk0a;
  u8 unk0c[0x6c - 0x0c];
  struct Actor *unk6c;
};

void Actor_RollAndCrumble(struct Actor *actor) {
  struct Vector3 position;
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  s32 state = actor->timer;

  if (state == 0) {
    s32 dz;
    s32 dx;
    s32 tileX;
    struct MapCell *cellsA;
    struct MapCell *cellsB;
    s32 widthA;
    s32 widthB;
    s32 offset;
    s32 zB;
    s32 tileZ;
    s32 heightA;
    s32 heightB;
    s32 x;
    s32 z;
    s32 xA;
    s32 xB;
    s32 zA;
    s32 flag;
    struct MapWork *map;
    struct MapCell *cell;
    struct FieldWork *work2;
    struct Work020023c4__s2860 *other;
    struct Actor *target;

    flag = Event_GetFlag(FLAG_MENU_OPEN);
    if (work->psynergy != 0)
      flag |= 1;
    actor->movementState = flag;

    position.x = actor->x;
    position.y = actor->y;
    position.z = actor->z;
    FieldVector_AddPolarOffset(px(8), actor->facing, &position);
    if (MapGrid_GetCellAttributeLowNibbleOrBump(0, position.x, position.z) ==
        7) {
      actor->targetZ = 0x80000000;
      actor->targetY = 0x80000000;
      actor->targetX = 0x80000000;
      actor->acceleration = px(1) / 4;
      actor->timer = 1;
      Event_PlaySound(SE_CRASH);
      MapGrid_SetShake(5, 5, 1);
      MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
      if (actor->watchedObject != NULL)
        ((void (*)(struct Actor *))actor->watchedObject)(actor);
    }

    map = Ram_HeapSlots->mapWork;
    tileX = actor->x >> 20;
    tileZ = actor->z >> 20;
    cellsA = &map->layers[1].cells[tileX + (tileZ << 7)];
    cellsB = &map->layers[2].cells[tileX + (tileZ << 7)];
    widthA = *(s32 *)&map->layers[1].unk08[0] >> 20;
    heightA = *(s32 *)&map->layers[1].unk08[4] >> 20;
    widthB = *(s32 *)&map->layers[2].unk08[0] >> 20;
    heightB = *(s32 *)&map->layers[2].unk08[4] >> 20;

    for (dz = -1, zB = tileZ + heightB - 2, z = tileZ - 1,
        zA = tileZ + heightA - 1;
         dz <= 1; zB++, z++, zA++, dz++) {
      for (dx = -1, xB = tileX + widthB - 1, x = tileX - 1,
          xA = tileX + widthA - 1;
           dx <= 1; xB++, x++, xA++, dx++) {
        offset = ((dz << 7) + dx) * 4;
        cell = (struct MapCell *)(offset + (s32)cellsA);
        if (cell->collisionCode == 77) {
          cell->collisionCode = 0;
          MapGrid_CopyCellsLayered(64, 64, 1, 1, xA, zA);
          MapGrid_CopyCellAttributes(64, 64, 1, 1, x, z);
          Event_PlaySound(SE_573);
          Embers_Spawn((x << 20) + 0x80000, actor->y, (z << 20) + 0x80000,
                       actor->facing);
        }
        cell = (struct MapCell *)(offset + (s32)cellsB);
        if (cell->collisionCode == 77) {
          cell->collisionCode = 0;
          MapGrid_CopyCellsLayered(64, 64, 1, 2, xB, zB);
          MapGrid_CopyCellAttributes(64, 64, 1, 1, x, z);
          Event_PlaySound(SE_572);
          Embers_Spawn((x << 20) + 0x80000, actor->y, (z << 20) + 0x80000,
                       actor->facing);
        }
      }
    }

    if (actor->scaleX < Q16_ONE) {
      actor->scaleX += Q16_ONE / 16;
      actor->scaleY += Q16_ONE / 16;
    }
    FieldActor_SetMoveTargetFixed(actor, position.x, position.y, position.z);

    work2 = Ram_HeapSlots->fieldWork;
    other = (struct Work020023c4__s2860 *)(gInventorySnapshot + 0x188);
    target = Actor_Get(gMainRoutineWork.currentOwner);
    if (ABS(actor->x - target->x) < 0x140000 &&
        (u32)(actor->y - target->y + 0x7ffff) <= 0x1ffffe &&
        ABS(actor->z - target->z) < 0x140000) {
      other->unk04 = 2;
      other->unk6c = actor;
      work2->stepTrigger = other->unk0a + 2;
    }
  } else if (state == 1) {
    actor->movementState = 0;
    if (actor->scaleX > 0) {
      actor->scaleX -= Q16_ONE / 16;
      actor->scaleY -= Q16_ONE / 16;
      MarsLighthouseEntrance_ScatterCrumbs(actor);
    } else {
      actor->z = 0;
      actor->y = 0;
      actor->x = 0;
      actor->velocityZ = 0;
      actor->velocityY = 0;
      actor->velocityX = 0;
      actor->update = NULL;
    }
  }
}
#undef WORK
#undef ABS

extern char SceneId_MarsLighthouseRoomM3;
extern char SceneId_MarsLighthouseEntrance1;
extern char SceneId_MarsLighthouseEntrance2;
extern char SceneId_MarsLighthouseEntrance3;
extern char SceneId_MarsLighthouseEntrance4;
extern char SceneId_MarsLighthouseEntrance5;
extern char SceneId_MarsLighthouseEntrance6;
extern char SceneId_MarsLighthouseEntrance7;
extern struct Trigger gMarsLighthouseEntrance_Room1Triggers[];
extern struct Trigger gMarsLighthouseEntrance_Room2Triggers[];
extern struct Trigger gMarsLighthouseEntrance_Room3Triggers[];
extern struct Trigger gMarsLighthouseEntrance_Room4Triggers[];
extern struct Trigger gMarsLighthouseEntrance_Room5Triggers[];
extern struct Trigger gMarsLighthouseEntrance_Room6Triggers[];
extern struct Trigger gMarsLighthouseEntrance_Room7Triggers[];
extern s16 gMarsLighthouseEntrance_DragonFireTimer;
extern u16 gMarsLighthouseEntrance_HoleMaskVram;

void TileMap_ConvertBuffer(s32 scene, s32 a1);

#include "shared/actor_attach.h"

#include "shared/actor_launch_ahead.h"

void Actor_SetSpritePriorityDirect(struct Actor *object, s32 mode);

#include "shared/actor_pop_up_slow.h"

#include "shared/actor_pop_up.h"

#include "shared/actor_park.h"

extern s16 gSlideOnIce_SteerAngles[];

#include "shared/actor_steer_by_pad.h"

#include "shared/leader_stop_if_pending.h"

void Actor_SelectAnimation(struct Actor *actor, s32 mode);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
extern s16 gSlideOnIce_AnglesByKey[];

#include "shared/leader_slide_on_ice.h"

extern volatile s16 gSlideOnIce_Stopped;
void Actor_SteerByPad(struct Actor *object);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Actor_SelectAnimation(struct Actor *object, s32 mode);

#include "shared/slide_on_ice_from_edge.c"

void Leader_SetSlideStopped(s32 value) { gSlideOnIce_Stopped = value; }

#undef Actor_Blink
#include "shared/leader_slide_on_ice_ahead.h"

#include "../field/object/set_sprite_priority_b.c"

#include "../field/field_effect/move.c"

#undef TLA_MARS_LIGHTHOUSE_ENTRANCE_RIDE_H
#undef RIDE_WORK
#undef DEFINE_BURST_ICE_SHEET
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
#undef Func_02002c2c
#undef Unit_Get
#undef Actor_Blink
#undef Position
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

#define CELL_PUSH_SLOT_ENTRY Oam_AddSpriteAtDepthClamped
#include "../field/map/cell_sprite.c"

#include "shared/cracked_tiles_find.h"

#include "shared/leader_center_in_cell.h"

// Map_CopyCellsRect through this overlay's import.

// Centers the leader on its cell, plays sound 161 and copies the cell below
// the table's source cell over the leader's, keeping the source column's and
// row's 64 bit; CrackedTiles_Step calls it the first time the leader steps on
// one of the table's cells.
#include "shared/cracked_tile_crack.h"

/// A cracked tile gives way under the leader, who drops through it.
void MarsLighthouseEntrance_DropThroughCrack(struct PushEvent *event) {
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
  gFieldWork->transition = 0x209;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
}

#include "../field/field_effect/advance_motion.c"

#undef CELL_PUSH_SLOT_ENTRY
void Teleport_PlayWhirl(void);

/// The leader lands from the floor above in a puff of dust.
void MarsLighthouseEntrance_LandFromCrack(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  struct EffectOptions options;
  struct Vector3 velocity;
  u32 i;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Camera_SetBounds(-1, -1, -1, 0);
  Map_CenterOnFollowedPoint();
  System_Wait(1);
  leader->y = px(130);
  leader->gravity = px(1) / 2;
  leader->bounce = 0;
  leader->motionFlags = 0;
  gFieldWork->transition = 0x100;
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
  Actor_SetAttachedEffect(gMainRoutineWork.currentOwner,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 49);
  MapGrid_SetShake(5, 5, 1);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  FieldMapGrid_WaitForShakeToSettle();
  Actor_SetAttachedEffect(gMainRoutineWork.currentOwner,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_REMOVE);
  leader->gravity = px(1);
  leader->bounce = Q16_ONE / 4;
  FieldEvent_Wait(10);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 1);
  Event_End();
}

void GameFlag_SetByte(s32 flag, s32 value);
/* VIEW(s42f8): routine MarsLighthouseEntrance_DropThroughCrack as this part
 * declared it; the joined file has another; tried as the definition's void
 * MarsLighthouseEntrance_DropThroughCrack(struct PushEvent *event), which the
 * object already declares, it does not compile in tla-ja:
 * games/tla/src/scene/mars_lighthouse_entrance.c:2095: too many arguments to
 * function `MarsLighthouseEntrance_DropThroughCrack'. */
void MarsLighthouseEntrance_DropThroughCrack__s42f8(
    struct CellTarget *work,
    struct CellTargetEntry
        *entry) __asm__("MarsLighthouseEntrance_DropThroughCrack");
#define MarsLighthouseEntrance_DropThroughCrack                                \
  MarsLighthouseEntrance_DropThroughCrack__s42f8

s32 CrackedTiles_Step(void) {
  struct Actor *object = Actor_Get(gMainRoutineWork.currentOwner);
  s32 x = object->x >> 20;
  s32 z = object->z >> 20;
  struct CellTarget *work = &gCrackedTiles;
  struct CellTargetEntry *entry = work->entries;
  s32 i;

  for (i = 0; i < work->count; i++, entry++) {
    if (entry->x == x && entry->z == z) {
      if (Event_GetFlag(entry->flag) == 0) {
        CrackedTile_Crack(work, entry);
        Event_SetFlag(entry->flag);
        work->current = i;
        break;
      }
      if (i == work->current)
        break;
      work->current = i;
      MarsLighthouseEntrance_DropThroughCrack(work, entry);
      GameFlag_SetByte(work->flagByte, x);
      GameFlag_SetByte(work->flagByte + 8, z);
      return 1;
    }
  }
  return 0;
}

void CrackedTile_PlaceLanding(s32 x, s32 z, s32 effect) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  s32 dx = FieldGameFlag_GetByte(gCrackedTiles.flagByte);
  s32 dz = FieldGameFlag_GetByte(gCrackedTiles.flagByte + 8);

  if (dx != 0 || dz != 0) {
    struct Actor *marker;

    GameFlag_SetByte(gCrackedTiles.flagByte, 0);
    GameFlag_SetByte(gCrackedTiles.flagByte + 8, 0);
    leader->x = ((dx + x) << 20) + 0x80000;
    leader->z = ((dz + z) << 20) + 0x80000;
    marker = work->camera;
    marker->x = leader->x;
    marker->z = leader->z;
    Map_CenterOnFollowedPoint();
    MarsLighthouseEntrance_LandFromCrack();
    gFieldWork->transition = effect;
  }
}

extern u16 *gData_0200e88c;
extern u16 *gData_02006890;

// Records the four palette lists, writes each listed palette entry from its
// red, green and blue, then copies two palettes from block32 and starts the
// blend.
#include "shared/palette_set_colors_fade.h"

extern u16 gPaletteCycle_Count;

/// Starts the shimmer of the circle's colours as Teleport is cast.
void Teleport_StartShimmer(void) {
  gPaletteCycle_Step = 0;
  gPaletteCycle_Count = List_CountEntries(gData_0200e88c);
  gPaletteCycle_Period = 15;
  gPaletteCycle_Countdown = 16;
  gPaletteCycle_Flash = 1;
  Task_AddFrame(PaletteCycle_Fade, TASK_PASS_VBLANK);
}

/// Stops the shimmer.
void Teleport_StopShimmer(void) {
  gFieldWork->transition = 256;
  FieldTask_Remove(PaletteCycle_Fade);
}

void Teleport_StartFastShimmer(void) {
  gPaletteCycle_Step = 0;
  gPaletteCycle_Count = List_CountEntries(gData_0200e88c);
  gPaletteCycle_Period = 2;
  gPaletteCycle_Countdown = 2;
  gPaletteCycle_Flash = 1;
  Task_AddFrame(PaletteCycle_Fade, TASK_PASS_VBLANK);
}

#include "shared/list_count_entries.h"

void PaletteCycle_Fade(void) {
  s32 step;
  u32 i;
  u16 *src;
  volatile u16 *dst;
  int red, green, blue;

  step = 0;
  if (gPaletteCycle_Countdown != 0) {
    if (--gPaletteCycle_Countdown == 0) {
      step = 1;
      gPaletteCycle_Countdown = gPaletteCycle_Period;
      if (gPaletteCycle_Period > 2)
        gPaletteCycle_Period--;
    }
  }
  if (step == 0)
    return;
  src = Data_02006894;
  for (i = 0; i < gPaletteCycle_Count; i++) {
    dst = &BG_PLTT[Data_02006880[gPaletteCycle_Step * gPaletteCycle_Count + i]];
    red = *src++;
    green = *src++;
    blue = *src++;
    SET_RGB(*dst, red, green, blue);
  }
  dst = &BG_PLTT[gData_0200e88c[gPaletteCycle_Count]];
  if (gPaletteCycle_Flash != 0)
    src = gData_02006890 + gPaletteCycle_Count * 3;
  else
    src = Data_02006894 + gPaletteCycle_Count * 3;
  red = *src++;
  green = *src++;
  blue = *src++;
  SET_RGB(*dst, red, green, blue);
  gPaletteCycle_Flash ^= 1;
  if (++gPaletteCycle_Step == gPaletteCycle_Count)
    gPaletteCycle_Step = 0;
}

void Func_020048e8(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  s32 saved;

  saved = work->transition;
  PaletteFade_ApplyTarget(0x202108, 1);
  PaletteFade_ApplyTarget(0x10000, 2);
  gFieldWork->transition = 0x100;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Teleport_StartFastShimmer();
  Teleport_PlayWhirl();
  System_Wait(40);
  Teleport_StopShimmer();
  PaletteFade_ApplyTarget(0x10000, 1);
  PaletteFade_Interpolate(16);
  System_Wait(16);
  {
    struct MainRoutineWork *party = &gMainRoutineWork;

    party->unk2c2 = 0;
  }
  work->transition = saved;
}

#include "shared/actors_set_cells_open.h"

// Finds the current target's table row and shrinks it.
UNUSED void Pound_SinkTarget(u16 *list, s32 mode, s32 count) {
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
        object->scaleY = Q16_ONE;
      }
      break;
    }
    list += 3;
  test:;
  } while (*list != 0xffff);
}

#include "shared/actor_set_cell_open.h"

// clang-format off
u32 gMarsLighthouseEntrance_Room3WestFlameThrowerRoute[] = {                                                               
      SC_SET(SW_STEERING, 0),                                                  
      SC_SET(SW_SPEED, 0x8000),                                                
      SC_SET(SW_ACCEL, 0x800),                                                 
      SC_MOVE(px(200), 0, px(112)),                                     
      SC_SYNC,                                                                 
      SC_WAIT(20),                                                             
      SC_MOVE(px(104), 0, px(112)),                                     
      SC_SYNC,                                                                 
      SC_WAIT(20),                                                             
      SC_LOOP(SC_FOREVER, SC_TOP),                                             
  };

u32 gMarsLighthouseEntrance_Room3EastFlameThrowerRoute[] = {                                                               
      SC_SET(SW_STEERING, 0),                                                  
      SC_SET(SW_SPEED, 0x8000),                                                
      SC_SET(SW_ACCEL, 0x800),                                                 
      SC_MOVE(px(232), 0, px(112)),                                     
      SC_SYNC,                                                                 
      SC_WAIT(20),                                                             
      SC_MOVE(px(296), 0, px(112)),                                     
      SC_SYNC,                                                                 
      SC_WAIT(20),                                                             
      SC_LOOP(SC_FOREVER, SC_TOP),                                             
  };

u32 gMarsLighthouseEntrance_Room4FlameThrowerRoute[] = {                                                               
      SC_SET(SW_STEERING, 0),                                                  
      SC_SET(SW_MOTION, 2),                                                    
      SC_SET(SW_Y, px(48)),                                                
      SC_SET(SW_GROUND, px(48)),                                           
      SC_SET(SW_SPEED, 0x8000),                                                
      SC_SET(SW_ACCEL, 0x800),                                                 
      SC_MOVE(px(904), 0, px(528)),                                      
      SC_SYNC,                                                                 
      SC_WAIT(20),                                                             
      SC_MOVE(px(840), 0, px(528)),                                      
      SC_SYNC,                                                                 
      SC_WAIT(20),                                                             
      SC_LOOP(SC_FOREVER, SC_TOP),                                             
  };

u32 gMarsLighthouseEntrance_Room6WestFlameThrowerRoute[] = {
    SC_SET(SW_STEERING, 0),
    SC_SET(SW_MOTION, 2),
    SC_SET(SW_Y, px(48)),
    SC_SET(SW_GROUND, px(48)),
    SC_SET(SW_SPEED, 0x8000),
    SC_SET(SW_ACCEL, 0x800),
    SC_WAIT(10),
    SC_MOVE(px(88), 0, px(688)),
    SC_SYNC,
    SC_WAIT(30),
    SC_LABEL(1),
    SC_MOVE(px(152), 0, px(688)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(px(88), 0, px(688)),
    SC_SYNC,
    SC_LOOP(SC_FOREVER, SC_TO(1)),
};

u32 gMarsLighthouseEntrance_Room6EastFlameThrowerRoute[] = {                                                               
      SC_SET(SW_STEERING, 0),                                                  
      SC_SET(SW_MOTION, 0),                                                   
      SC_SET(SW_Y, px(32)),                                                
      SC_SET(SW_GROUND, px(32)),                                           
      SC_SET(SW_SPEED, 0x8000),                                                
      SC_SET(SW_ACCEL, 0x800),                                                 
      SC_END,                                                                  
  };

s32 gMarsLighthouseEntrance_DragonFaceGoesLeft[] = {
    SC_SET(SW_STEERING, 0),
    SC_SET(SW_MOTION, 0),
    SC_SET(SW_Y, px(32)),
    SC_SET(SW_GROUND, px(32)),
    SC_SET(SW_SPEED, 0x8000),
    SC_SET(SW_ACCEL, 0x800),
    SC_MOVE(px(808), px(32), px(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(px(808), px(48), px(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(px(728), px(48), px(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(px(728), px(32), px(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(px(712), px(32), px(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(px(712), px(48), px(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(px(616), px(48), px(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(px(616), px(32), px(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(px(552), px(32), px(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(px(552), px(48), px(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(px(536), px(48), px(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(px(536), px(32), px(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(px(520), px(32), px(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(px(520), px(48), px(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(px(296), px(48), px(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(px(296), px(32), px(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(px(264), px(32), px(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(px(264), px(48), px(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(px(136), px(48), px(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(px(136), px(32), px(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(px(120), px(32), px(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(px(120), px(48), px(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(px(56), px(48), px(128)),
    SC_SYNC,
    SC_WAIT(60),
    SC_CALL(MarsLighthouseEntrance_LightDragonFace),
    SC_END,
};

u32 gMarsLighthouseEntrance_DragonFaceAtLeft[] = {
    SC_SET(SW_STEERING, 0),
    SC_SET(SW_MOTION, 0),
    SC_SET(SW_X, px(56)),
    SC_SET(SW_Z, px(128)),
    SC_SET(SW_Y, px(48)),
    SC_SET(SW_GROUND, px(48)),
    SC_SET(SW_SPEED, 0x8000),
    SC_SET(SW_ACCEL, 0x800),
    SC_MOVE(px(56), px(48), px(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_END,
};

u32 gMarsLighthouseEntrance_IceChipScript[] = {
    0, 40, 0x80010000, 22, 25, 0,
    0, 1, 22, 25, 1, 0,
    1, 13, 10, 0xc0010000, 38,
};

s16 gMarsLighthouseEntrance_Room2IceCones[] = {
    8, 7, 512, 9, 7, 513, -1,
};

u16 gMarsLighthouseEntrance_Room3BeamTargets[] = {
    9, 3, 10, 3, 11, 3, 12, 3,
    0,
};

u16 gMarsLighthouseEntrance_Room3BeamStops[] = {
    8, 0, 0,
};

s16 gMarsLighthouseEntrance_Room3IceCones[] = {
    16, 7, 514, 17, 7, 515, -1,
};

u16 gMarsLighthouseEntrance_Room4BeamTargets[] = {
    8, 3, 0,
};

u16 gMarsLighthouseEntrance_Room4BeamStops[] = {
    9, 1, 0,
};

s16 gMarsLighthouseEntrance_Room4IceCones[] = {
    12, 7, 516, 13, 7, 517, 14, 7,
    518, 15, 7, 519, 16, 7, 520, -1,
};

u16 gMarsLighthouseEntrance_Room6UpperBeamTargets[] = {
    9, 3, 0,
};

u16 gMarsLighthouseEntrance_Room6UpperBeamStops[] = {
    8, 0, 10, 1, 0,
};

u16 gMarsLighthouseEntrance_Room6LowerBeamTargets[] = {
    11, 67, 0,
};

u16 gMarsLighthouseEntrance_Room6LowerBeamStops[] = {
    12, 1, 0,
};

s16 gMarsLighthouseEntrance_Room6IceCones[] = {
    13, 7, 528, 14, 7, 529, 15, 7,
    530, 16, 7, 531, 17, 7, 532, 18,
    7, 533, -1, 0,
};

s32 gMarsLighthouseEntrance_Room6UpperFireballPath[] = {                                                               
      SC_CALL(Actor_PopUpSlow), SC_MOVE(px(112), px(32), px(728)),             
      SC_SYNC,        SC_MOVE(px(112), px(22), px(728)),             
      SC_SYNC,        SC_MOVE(px(128), px(22), px(728)),             
      SC_SYNC,        SC_CALL(Actor_Park),                                          
      SC_END,                                                                  
  };

s32 gMarsLighthouseEntrance_Room6LowerFireballPath[] = {                                                               
      46, (s32)Actor_PopUpSlow, 4,  0x500000, 0x200000, 0xa80000, 1,  4,          0x500000, 0x160000,         0xa80000, 1,   
      4,  0x600000,         0x160000, 0xa80000, 1,  22, 37, (s32)MarsLighthouseEntrance_ClearFireballRow, 46, (s32)Actor_Park, 17,      
  };

s32 gMarsLighthouseEntrance_Room4FireballPath[] = {                                                               
      SC_CALL(Actor_PopUpSlow), SC_MOVE(px(872), px(32), px(576)),             
      SC_SYNC,        SC_MOVE(px(872), px(30), px(576)),             
      SC_SYNC,        SC_MOVE(px(872), px(30), px(584)),             
      SC_SYNC,        SC_CALL(Actor_Park),                                          
      SC_END,                                                                  
  };

s32 gMarsLighthouseEntrance_FireballToRoom5IceSheet[] = {                                                               
      46, (s32)Actor_PopUp, 4,  0x600000, 0x200000, 0x2a80000, 1,  4,          0x600000, 0x160000,         0x2a80000, 1,   
      4,  0x700000,         0x160000, 0x2a80000, 1,  22, 37, (s32)MarsLighthouseEntrance_CrackRoom5IceSheet, 46, (s32)Actor_Park, 17,      
  };

s32 gMarsLighthouseEntrance_FireballThroughIce[] = {
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
    (s32)MarsLighthouseEntrance_MeltIceInPath,
    46,
    (s32)Actor_Park,
    17,
};

s32 gMarsLighthouseEntrance_FireballToRoom3IceSheet[] = {                                                               
      46, (s32)Actor_PopUp, 4,  0x700000, 0x200000, 0x2a80000, 1,  4,          0x700000, 0x160000,         0x2a80000, 1,   
      4,  0x800000,         0x160000, 0x2a80000, 1,  22, 37, (s32)MarsLighthouseEntrance_CrackRoom3IceSheet, 46, (s32)Actor_Park, 17,      
  };

u32 gScriptedPart_Script[] = {                                                               
      SC_WAIT(30), SC_SET(SW_HOOK, 0), SC_POSITION(0, 0, 0), SC_WAIT(1),       
      SC_FREE,                                                                 
  };

u32 gMarsLighthouseEntrance_CrumbScript[] = {
    0, 20, 0x80010000, 22, 25, 0,
    0, 1, 22, 25, 1, 0,
    1, 13, 10, 0xc0010000, 38,
};

u32 gEmbers_Script[] = {
    0, 20, 0x80010000, 22, 25, 0,
    0, 1, 22, 25, 1, 0,
    1, 13, 10, 0xc0010000, 38,
};

s16 gSlideOnIce_SteerAngles[] = {
    -1, 0, -32768, -1, -16384, -8192, -24576, -16384,
    16384, 8192, 24576, 16384, -1, 0, -32768, -1,
    -1, 0, -32768, -1, -16384, -1, -1, -16384,
    16384, -1, -1, 16384, -1, 0, -32768, -1,
};

s16 gSlideOnIce_AnglesByKey[] = {
    -1, 0, -32768, -1, -16384, 0, -32768, -16384,
    16384, 0, -32768, 16384, -1, 0, -32768, -1,
};

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

s16 gMarsLighthouseEntrance_FadeSlots[] = {
    137, 138, 139, 140, 141, 142, 143, -1,
};

s16 gMarsLighthouseEntrance_FadeColorsFrom[] = {
    23, 25, 22, 23, 25, 22, 23, 25,
    22, 23, 25, 22, 23, 25, 22, 23,
    25, 22, 23, 25, 22, -1,
};

s16 gMarsLighthouseEntrance_FadeColorsTo[] = {
    30, 27, 31, 22, 21, 31, 20, 18,
    31, 13, 11, 27, 11, 8, 24, 8,
    5, 21, 31, 31, 31, -1,
};

u32 gMarsLighthouseEntrance_FadeOrder[] = {
    0x89008e, 0x8b008a, 0x8d008c, 0x8e008d, 0x8a0089, 0x8c008b,
    0x8d008c, 0x89008e, 0x8b008a, 0x8c008b, 0x8e008d, 0x8a0089,
    0x8b008a, 0x8d008c, 0x89008e, 0x8a0089, 0x8c008b, 0x8e008d,
};

s32 gMarsLighthouseEntrance_StatueScript[] = {
    SC_CALL(MarsLighthouseEntrance_SetUpStatue),
    SC_END,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 192, 0, 192, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(MarsLighthouseEntrance1),
    SCENE_EXIT(1, WorldMap, 84),
    SCENE_EXIT(2, MarsLighthouseEntrance3, 2),
    SCENE_EXITS(MarsLighthouseEntrance2),
    SCENE_EXIT(1, MarsLighthouseEntrance3, 1),
    SCENE_EXIT(2, MarsLighthouseRoomM1, 2),
    SCENE_EXIT(3, MarsLighthouseEntrance2, 4),
    SCENE_EXIT(4, MarsLighthouseEntrance2, 3),
    SCENE_EXIT(5, MarsLighthouseEntrance4, 5),
    SCENE_EXIT(6, MarsLighthouseEntrance4, 6),
    SCENE_EXIT(7, MarsLighthouseEntrance2, 8),
    SCENE_EXIT(8, MarsLighthouseEntrance2, 7),
    SCENE_EXIT(9, MarsLighthouseEntrance2, 10),
    SCENE_EXIT(10, MarsLighthouseEntrance2, 9),
    SCENE_EXIT(20, MarsLighthouseEntrance3, 20),
    SCENE_EXITS(MarsLighthouseEntrance3),
    SCENE_EXIT(1, MarsLighthouseEntrance2, 1),
    SCENE_EXIT(2, MarsLighthouseEntrance1, 2),
    SCENE_EXIT(3, MarsLighthouseEntrance4, 3),
    SCENE_EXIT(4, MarsLighthouseEntrance4, 4),
    SCENE_EXIT(5, MarsLighthouseEntrance5, 5),
    SCENE_EXIT(6, MarsLighthouseEntrance3, 7),
    SCENE_EXIT(7, MarsLighthouseEntrance3, 6),
    SCENE_EXIT(8, MarsLighthouseEntrance3, 9),
    SCENE_EXIT(9, MarsLighthouseEntrance3, 8),
    SCENE_EXIT(10, MarsLighthouseEntrance3, 11),
    SCENE_EXIT(11, MarsLighthouseEntrance3, 10),
    SCENE_EXIT(12, MarsLighthouseEntrance3, 13),
    SCENE_EXIT(13, MarsLighthouseEntrance3, 12),
    SCENE_EXIT(14, MarsLighthouseEntrance3, 15),
    SCENE_EXIT(15, MarsLighthouseEntrance3, 14),
    SCENE_EXIT(16, MarsLighthouseEntrance4, 16),
    SCENE_EXIT(17, MarsLighthouseEntrance3, 18),
    SCENE_EXIT(18, MarsLighthouseEntrance3, 17),
    SCENE_EXIT(20, MarsLighthouseEntrance2, 3),
    SCENE_EXITS(MarsLighthouseEntrance4),
    SCENE_EXIT(1, MarsLighthouseEntrance5, 1),
    SCENE_EXIT(2, MarsLighthouseEntrance5, 2),
    SCENE_EXIT(3, MarsLighthouseEntrance3, 3),
    SCENE_EXIT(4, MarsLighthouseEntrance3, 4),
    SCENE_EXIT(5, MarsLighthouseEntrance2, 5),
    SCENE_EXIT(6, MarsLighthouseEntrance2, 6),
    SCENE_EXIT(7, MarsLighthouseEntrance6, 7),
    SCENE_EXIT(8, MarsLighthouseEntrance4, 9),
    SCENE_EXIT(9, MarsLighthouseEntrance4, 8),
    SCENE_EXIT(10, MarsLighthouseEntrance4, 11),
    SCENE_EXIT(11, MarsLighthouseEntrance4, 10),
    SCENE_EXIT(14, MarsLighthouseEntrance5, 14),
    SCENE_EXIT(16, MarsLighthouseEntrance3, 16),
    SCENE_EXITS(MarsLighthouseEntrance5),
    SCENE_EXIT(1, MarsLighthouseEntrance4, 1),
    SCENE_EXIT(2, MarsLighthouseEntrance4, 2),
    SCENE_EXIT(3, MarsLighthouseEntrance5, 4),
    SCENE_EXIT(4, MarsLighthouseEntrance5, 3),
    SCENE_EXIT(5, MarsLighthouseEntrance3, 5),
    SCENE_EXIT(6, MarsLighthouseEntrance5, 7),
    SCENE_EXIT(7, MarsLighthouseEntrance5, 6),
    SCENE_EXIT(8, MarsLighthouseEntrance5, 9),
    SCENE_EXIT(9, MarsLighthouseEntrance5, 8),
    SCENE_EXIT(10, MarsLighthouseEntrance5, 11),
    SCENE_EXIT(11, MarsLighthouseEntrance5, 10),
    SCENE_EXIT(12, MarsLighthouseEntrance5, 13),
    SCENE_EXIT(13, MarsLighthouseEntrance5, 12),
    SCENE_EXIT(14, MarsLighthouseEntrance4, 14),
    SCENE_EXITS(MarsLighthouseEntrance6),
    SCENE_EXIT(1, MarsLighthouseEntrance7, 1),
    SCENE_EXIT(2, MarsLighthouseEntrance6, 3),
    SCENE_EXIT(3, MarsLighthouseEntrance6, 2),
    SCENE_EXIT(4, MarsLighthouseEntrance6, 5),
    SCENE_EXIT(5, MarsLighthouseEntrance6, 4),
    SCENE_EXIT(6, MarsLighthouseStarRoom1, 6),
    SCENE_EXIT(7, MarsLighthouseEntrance4, 7),
    SCENE_EXIT(8, MarsLighthouseStarRoom1, 8),
    SCENE_EXIT(9, MarsLighthouseStarRoom1, 9),
    SCENE_EXIT(10, MarsLighthouseEntrance6, 11),
    SCENE_EXIT(11, MarsLighthouseEntrance6, 10),
    SCENE_EXIT(12, MarsLighthouseEntrance6, 13),
    SCENE_EXIT(13, MarsLighthouseEntrance6, 12),
    SCENE_EXIT(14, MarsLighthouseEntrance6, 15),
    SCENE_EXIT(15, MarsLighthouseEntrance6, 14),
    SCENE_EXIT(16, MarsLighthouseStarRoom1, 10),
    SCENE_EXITS(MarsLighthouseEntrance7),
    SCENE_EXIT(1, MarsLighthouseEntrance6, 1),
    SCENE_EXIT(2, MarsLighthouseEntrance7, 3),
    SCENE_EXIT(3, MarsLighthouseEntrance7, 2),
    SCENE_EXIT(4, MarsLighthouseEntrance7, 6),
    SCENE_EXIT(5, MarsLighthouseStarRoom1, 5),
    SCENE_EXIT(6, MarsLighthouseEntrance7, 4),
    SCENE_EXIT(7, MarsLighthouseStarRoom1, 7),
    SCENE_EXIT(11, MarsLighthouseStarRoom1, 11),
    SCENE_EXITS(MarsLighthouseStarRoom1),
    SCENE_EXIT(1, MarsLighthouseStarRoom1, 2),
    SCENE_EXIT(2, MarsLighthouseStarRoom1, 1),
    SCENE_EXIT(3, MarsLighthouseStarRoom1, 4),
    SCENE_EXIT(4, MarsLighthouseStarRoom1, 3),
    SCENE_EXIT(5, MarsLighthouseEntrance7, 5),
    SCENE_EXIT(6, MarsLighthouseEntrance6, 6),
    SCENE_EXIT(7, MarsLighthouseEntrance7, 7),
    SCENE_EXIT(8, MarsLighthouseEntrance6, 8),
    SCENE_EXITS_END,
};

u32 gMarsLighthouseEntrance_IceConeScript[] = {
    39, 7, 17,
};

struct ActorPlacement gMarsLighthouseEntrance_Placements[] = {
    {SCENE_TABLE_END},
};

struct ActorPlacement gMarsLighthouseEntrance_Room1Placements[] = {
    ACTOR_PLACEMENT(413, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 280, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(413, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 280, FACING_WEST, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};
// clang-format on

struct ActorPlacement gMarsLighthouseEntrance_Room2Placements[] = {
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS,
                    (s32)gMarsLighthouseEntrance_IceConeScript, 0, 0, 0,
                    FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS,
                    (s32)gMarsLighthouseEntrance_IceConeScript, 136, 0, 424,
                    FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(489, 800, MOVEMENT_TYPE_STAND, 648, 0, 376, FACING_SOUTH,
                    TALK_FACING_KEEP, 0),
#if !TLA_PATCH_1
    ACTOR_PLACEMENT(245, 105, MOVEMENT_TYPE_STAND, 664, 0, 424, FACING_SOUTH,
                    TALK_FACING_PARTY, 0),
#else
    ACTOR_PLACEMENT(245, 105, MOVEMENT_TYPE_STAND, 728, 0, 408,
                    FACING_NORTHWEST, TALK_FACING_PARTY, 0),
#endif
    {SCENE_TABLE_END},
};

void MarsLighthouseEntrance_FightFugue(void);
void MarsLighthouseEntrance_FightBalm(void);
void MarsLighthouseEntrance_GrindIce(void);
void MarsLighthouseEntrance_TeleportFromRoom3(void);
void MarsLighthouseEntrance_WatchFireballCrackIce(void);
void MarsLighthouseEntrance_JumpOrSlide(void);
void MarsLighthouseEntrance_CheckCrackedFloor(void);

// clang-format off
struct ActorPlacement gMarsLighthouseEntrance_Room3Placements[] = {
    ACTOR_PLACEMENT(413, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 56, 0, 120, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(414, CONDITION_ALWAYS, (s32)gMarsLighthouseEntrance_Room3WestFlameThrowerRoute, 104, 0, 112, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(414, CONDITION_ALWAYS, (s32)gMarsLighthouseEntrance_Room3EastFlameThrowerRoute, 280, 0, 112, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(414, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 112, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(414, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 112, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(419, CONDITION_ALWAYS, (s32)gMarsLighthouseEntrance_StatueScript, 88, 0, 680, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(419, CONDITION_ALWAYS, (s32)gMarsLighthouseEntrance_StatueScript, 552, 0, 680, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(418, CONDITION_ALWAYS, (s32)gMarsLighthouseEntrance_StatueScript, 88, 0, 728, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, (s32)gMarsLighthouseEntrance_IceConeScript, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, (s32)gMarsLighthouseEntrance_IceConeScript, 552, 0, 776, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(291, CONDITION_ALWAYS, 7, 840, 0, 672, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(419, CONDITION_ALWAYS, (s32)gMarsLighthouseEntrance_StatueScript, 88, 0, 728, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(291, CONDITION_ALWAYS, 7, 480, 0, 704, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(673, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(412, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};
// clang-format on

// clang-format off
struct ActorPlacement gMarsLighthouseEntrance_Room4Placements[] = {
    ACTOR_PLACEMENT(414, CONDITION_ALWAYS, (s32)gMarsLighthouseEntrance_Room4FlameThrowerRoute, 840, 0, 528, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(419, CONDITION_ALWAYS, (s32)gMarsLighthouseEntrance_StatueScript, 872, 0, 568, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(413, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 792, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(413, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 792, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, (s32)gMarsLighthouseEntrance_IceConeScript, 840, 0, 680, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, (s32)gMarsLighthouseEntrance_IceConeScript, 904, 0, 744, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, (s32)gMarsLighthouseEntrance_IceConeScript, 840, 0, 808, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, (s32)gMarsLighthouseEntrance_IceConeScript, 936, 0, 824, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, (s32)gMarsLighthouseEntrance_IceConeScript, 904, 0, 872, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(244, 0x54, MOVEMENT_TYPE_STAND, 88, 0, 904, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(673, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(673, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(412, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(412, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};
// clang-format on

// clang-format off
struct ActorPlacement gMarsLighthouseEntrance_Room5Placements[] = {
    ACTOR_PLACEMENT(418, CONDITION_ALWAYS, (s32)gMarsLighthouseEntrance_StatueScript, 72, 0, 632, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(419, CONDITION_ALWAYS, (s32)gMarsLighthouseEntrance_StatueScript, 72, 0, 680, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(418, CONDITION_ALWAYS, (s32)gMarsLighthouseEntrance_StatueScript, 904, 0, 120, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(419, CONDITION_ALWAYS, (s32)gMarsLighthouseEntrance_StatueScript, 856, 0, 120, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(291, CONDITION_ALWAYS, 7, 240, 0, 688, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(673, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(412, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};
// clang-format on

// clang-format off
struct ActorPlacement gMarsLighthouseEntrance_Room6Placements[] = {
    ACTOR_PLACEMENT(413, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 696, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(414, CONDITION_ALWAYS, (s32)gMarsLighthouseEntrance_Room6WestFlameThrowerRoute, 120, 0, 688, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(419, CONDITION_ALWAYS, (s32)gMarsLighthouseEntrance_StatueScript, 88, 0, 728, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(414, CONDITION_ALWAYS, (s32)gMarsLighthouseEntrance_Room6EastFlameThrowerRoute, 792, 0, 128, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(419, CONDITION_ALWAYS, (s32)gMarsLighthouseEntrance_StatueScript, 56, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, (s32)gMarsLighthouseEntrance_IceConeScript, 200, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, (s32)gMarsLighthouseEntrance_IceConeScript, 328, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, (s32)gMarsLighthouseEntrance_IceConeScript, 360, 0, 152, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, (s32)gMarsLighthouseEntrance_IceConeScript, 424, 0, 152, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, (s32)gMarsLighthouseEntrance_IceConeScript, 664, 0, 152, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, (s32)gMarsLighthouseEntrance_IceConeScript, 760, 0, 152, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(673, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(412, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};
// clang-format on

// clang-format off
struct ActorPlacement gMarsLighthouseEntrance_Room7Placements[] = {
    ACTOR_PLACEMENT(413, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 88, 0, 152, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(413, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 152, FACING_WEST, TALK_FACING_KEEP, 0),
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
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 2, CONDITION_ALWAYS, 2},
    {0x51, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 6, CONDITION_ALWAYS, 6},
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
    {TALK_TRIGGER_FACING(FACING_WEST), 11, CONDITION_ALWAYS,
     EVENT(MarsLighthouseEntrance_FightFugue)},
    {TALK_TRIGGER_FACING(FACING_NORTH), 11, CONDITION_ALWAYS,
     EVENT(MarsLighthouseEntrance_FightFugue)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 8, 0x200,
     EVENT(MarsLighthouseEntrance_StartPoundIceCone)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 9, 0x201,
     EVENT(MarsLighthouseEntrance_StartPoundIceCone)},
    {PSYNERGY_ON_ACTOR(ABILITY_GRIND), 10, FLAG_ICE_GROUND,
     EVENT(MarsLighthouseEntrance_GrindIce)},
    {TRIGGER_TALK, 10, FLAG_ICE_GROUND, EVENT(MarsLighthouseEntrance_CheckIce)},
    {TRIGGER_TABLE_END},
};

struct Trigger gMarsLighthouseEntrance_Room3Triggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {0x51, 3, CONDITION_ALWAYS, 3},
    {0x51, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 5, CONDITION_ALWAYS, 5},
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
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 16, CONDITION_ALWAYS, 16},
    {TRIGGER_STEP, 88, CONDITION_ALWAYS, EVENT(Leader_KnockBack)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 16, 0x202,
     EVENT(MarsLighthouseEntrance_StartPoundIceCone)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 17, 0x203,
     EVENT(MarsLighthouseEntrance_StartPoundIceCone)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_TELEPORT, PSYNERGY_PHASE_AFTER_CAST), 17,
     CONDITION_ALWAYS, EVENT(MarsLighthouseEntrance_TeleportFromRoom3)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_TELEPORT, PSYNERGY_PHASE_AFTER_CAST), 18,
     CONDITION_ALWAYS, EVENT(MarsLighthouseEntrance_Teleport)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 8, FLAG_ROOM3_STATUE_BLOCKS_FLAMES,
     EVENT(MarsLighthouseEntrance_PushStatueIntoFlames)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS,
     EVENT(MarsLighthouseEntrance_PushStatueIntoFlames)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 80,
     IF_FLAG_SET(FLAG_ROOM3_SHEET_CRACKED),
     EVENT(MarsLighthouseEntrance_BurstIceSheet)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 81,
     IF_FLAG_SET(FLAG_ROOM3_SECOND_SHEET_SHOWN),
     EVENT(MarsLighthouseEntrance_BurstRoom3IceSheet)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST), 15,
     CONDITION_ALWAYS, 0},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_HITS), 13,
     CONDITION_ALWAYS, EVENT(MarsLighthouseEntrance_BlazeRoom3Statue)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_AFTER_END), 13,
     CONDITION_ALWAYS, EVENT(MarsLighthouseEntrance_WatchFireballCrackIce)},
    {TRIGGER_TALK, 64, CONDITION_ALWAYS,
     EVENT(MarsLighthouseEntrance_BlockRetreatAtChest)},
    {TRIGGER_TABLE_END},
};

struct Trigger gMarsLighthouseEntrance_Room4Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 7, CONDITION_ALWAYS, 7},
    {0x51, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {0x51, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_EXIT, 13, CONDITION_ALWAYS, 13},
    {TRIGGER_EXIT, 15, CONDITION_ALWAYS, 15},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 16, CONDITION_ALWAYS, 16},
    {TRIGGER_STEP, 30, CONDITION_ALWAYS, EVENT(Leader_SlideOnIce)},
    {TRIGGER_STEP, 90, CONDITION_ALWAYS,
     EVENT(MarsLighthouseEntrance_SlideOnIce)},
    {TRIGGER_TALK, 17, CONDITION_ALWAYS,
     EVENT(MarsLighthouseEntrance_FightBalm)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_ACTS), 10,
     CONDITION_ALWAYS, EVENT(MarsLighthouseEntrance_SlideStatueOnIce)},
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
    {PSYNERGY_PHASE_ON_CELL(ABILITY_TELEPORT, PSYNERGY_PHASE_AFTER_CAST), 14,
     CONDITION_ALWAYS, EVENT(MarsLighthouseEntrance_TeleportFromRoom4)},
    {TRIGGER_TABLE_END},
};

struct Trigger gMarsLighthouseEntrance_Room5Triggers[] = {
    {0x51, 1, CONDITION_ALWAYS, 1},
    {0x51, 2, CONDITION_ALWAYS, 2},
    {0x51, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 5, CONDITION_ALWAYS, 5},
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
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_HITS), 9,
     CONDITION_ALWAYS, EVENT(MarsLighthouseEntrance_BlazeRoom5WestStatue)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_AFTER_END), 9,
     CONDITION_ALWAYS,
     EVENT(MarsLighthouseEntrance_WatchFireballCrackRoom5Ice)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST), 10,
     CONDITION_ALWAYS, 0},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_HITS), 11,
     CONDITION_ALWAYS, EVENT(MarsLighthouseEntrance_BlazeRoom5EastStatue)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_AFTER_END), 11,
     CONDITION_ALWAYS, EVENT(MarsLighthouseEntrance_WatchFireballMeltIce)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 80,
     IF_FLAG_SET(FLAG_ROOM5_SHEET_SHOWN),
     EVENT(MarsLighthouseEntrance_BurstRoom5IceSheet)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_TELEPORT, PSYNERGY_PHASE_AFTER_CAST), 14,
     CONDITION_ALWAYS, EVENT(MarsLighthouseEntrance_TeleportFromRoom5)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0,
     CONDITION_ALWAYS, EVENT(MarsLighthouseEntrance_RevealTeleportCircle)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0,
     CONDITION_ALWAYS, EVENT(MarsLighthouseEntrance_HideTeleportCircle)},
    {TRIGGER_TABLE_END},
};

struct Trigger gMarsLighthouseEntrance_Room6Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {0x51, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {0x51, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {0x51, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 14, CONDITION_ALWAYS, 14},
    {0x51, 15, CONDITION_ALWAYS, 15},
    {TRIGGER_STEP, 9, CONDITION_ALWAYS,
     EVENT(MarsLighthouseEntrance_FallIntoHole)},
    {TRIGGER_STEP, 16, CONDITION_ALWAYS,
     EVENT(MarsLighthouseEntrance_FallIntoHole)},
    {TRIGGER_STEP, 35, FLAG_DRAGON_FACE_AWAKE,
     EVENT(MarsLighthouseEntrance_WakeDragonFace)},
    {TRIGGER_STEP, 30, CONDITION_ALWAYS,
     EVENT(MarsLighthouseEntrance_SlideFromEdge)},
    {TRIGGER_STEP, 90, CONDITION_ALWAYS,
     EVENT(MarsLighthouseEntrance_SlideOnIce)},
    {TRIGGER_STEP, 13, CONDITION_ALWAYS,
     EVENT(MarsLighthouseEntrance_SlideIfRiding)},
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
    {PSYNERGY_PHASE_ON_CELL(ABILITY_TELEPORT, PSYNERGY_PHASE_AFTER_CAST), 12,
     CONDITION_ALWAYS, EVENT(MarsLighthouseEntrance_TeleportFromRoom6West)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_TELEPORT, PSYNERGY_PHASE_AFTER_CAST), 13,
     CONDITION_ALWAYS, EVENT(MarsLighthouseEntrance_TeleportFromRoom6East)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 8, FLAG_ROOM6_STATUE_BLOCKS_FLAMES,
     EVENT(MarsLighthouseEntrance_PushStatueIntoRoom6Flames)},
    {TRIGGER_TABLE_END},
};

struct Trigger gMarsLighthouseEntrance_Room7Triggers[] = {
    {0x51, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {0x51, 3, CONDITION_ALWAYS, 3},
    {0x51, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 7, CONDITION_ALWAYS, 7},
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
    {TRIGGER_STEP, 31, CONDITION_ALWAYS,
     EVENT(MarsLighthouseEntrance_CheckCrackedFloor)},
    {STEP_TRIGGER_WALKING, 35, CONDITION_ALWAYS,
     EVENT(MarsLighthouseEntrance_JumpOrSlide)},
    {TRIGGER_TABLE_END},
};

u32 gMarsLighthouseEntrance_DragonFireOn[] = {
    0,
};

s32 gFieldEffect_DurationScripts[] = {
    (s32)gFieldEffect_ShortScript,
    (s32)gFieldEffect_MediumScript,
    (s32)gFieldEffect_LongScript,
};
#undef MarsLighthouseEntrance_DropThroughCrack
