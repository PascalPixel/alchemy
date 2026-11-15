/// @file
/// Mars Lighthouse's lower floors, inside its gates, in the rooms of this
/// overlay: ice cones Pound flattens (MarsLighthouseEntrance_PoundIceCone),
/// ice sheets that burst and ice to Grind (MarsLighthouseEntrance_GrindIce),
/// floors of ice the leader slides across (MarsLighthouseEntrance_SlideOnIce),
/// flame throwers whose flames knock the leader back (FlameThrowers_Start,
/// Flame_UpdateForward), a statue pushed into the flames
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
#include "graphics/palette_buffers.h"
#include "sprite_ids.h"
#include "scene/ride.h"
#include "field/field_sequence.h"
#include "field/event_inline.h"
#include "field/field.h"
#include "scene/mars_lighthouse_entrance.h"
#include "scene/scene.h"
#include "battle/battle.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "field/scene_tables.h"
#include "shared/mars_lighthouse_entrance_room.h"
#include "field/map_grid.h"
#include "system/main_routine_work.h"
#include "field/actor.h"
#include "field/field_sequence_event.h"
#include "window/item_icon.h"
#include "scene_ids.h"
#include "field/field_event.h"
#include "system/field_effect.h"
#include "system/iwram_call.h"
#include "system/ram_buffer.h"
#include "graphics/vram_tab.h"
#include "system/task.h"
#include "system/io_reg.h"
#include "version.h"
#include "field/call.h"
#include "battle/battle_unit.h"
#include "system/vector3.h"
#include "fixed_math.h"
#include "graphics/display_transition.h"
#include "field/field_sprite.h"
#include "macros.h"
#include "system/input.h"
#include "system/heap_release.h"
#include "field/cell_target.h"
#include "shared/tables.h"
#include "field/field_effect.h"
#include "system/dma.h"
#include "field/actor_script.h"
#include "system/frame.h"
#include "text/msg_ids.h"

#define FLAG_TELEPORTED_ONCE 0xa36
/// Grind on the dead end cracked the ice sheet a floor below.
#define FLAG_ICE_GROUND 0xa3e
#define FLAG_LAPIS_SHEET_BURST 0xa3f
#define FLAG_LAPIS_SECOND_SHEET_BURST 0xa47
#define FLAG_LAPIS_SECOND_SHEET_CRACKED 0xa48 ///< a fireball cracked it
#define FLAG_HIDDEN_CIRCLE_ICE_MELTED                                          \
  0xa49 ///< the fireball melted the ice in its path
#define FLAG_HIDDEN_CIRCLE_SHEET_CRACKED 0xa4c
#define FLAG_HIDDEN_CIRCLE_SHEET_BURST 0xa4d
#define FLAG_VALKYRIE_STATUE_BLOCKS_FLAMES 0xa4e
#define FLAG_LAPIS_STATUE_BLOCKS_FLAMES 0xa4f
#define FLAG_LAPIS_SHEET_CRACKED FLAG_SCENE_TEMP_30
#define FLAG_LAPIS_SECOND_SHEET_SHOWN FLAG_SCENE_TEMP_31
#define FLAG_HIDDEN_CIRCLE_SHEET_SHOWN FLAG_SCENE_TEMP_32
#define FLAG_DRAGON_FACE_AWAKE FLAG_SCENE_TEMP_34

/// The first hall (Fugue's room): two ice cones, the dead end Grind cracks
/// (unseen, sprite 489) and Fugue.
enum MarsLighthouseEntranceFugueActorId {
  ACTOR_FUGUE_ICE_CONE_A = ACTOR_FIRST_PLACED,
  ACTOR_FUGUE_ICE_CONE_B,
  ACTOR_DEAD_END,
  ACTOR_FUGUE
};

/// The Teleport Lapis room: the dragon statue Move pushes into the flames, four
/// flame throwers, the torches (one lit in the same place as an unlit one, each
/// shown by whether the lighthouse is thawed), two ice cones and the two ice
/// sheets.
enum MarsLighthouseEntranceTeleportLapisActorId {
  ACTOR_PUSHED_STATUE = ACTOR_FIRST_PLACED,
  ACTOR_LAPIS_FLAME_THROWER_A,
  ACTOR_LAPIS_FLAME_THROWER_B,
  ACTOR_LAPIS_FLAME_THROWER_C,
  ACTOR_LAPIS_FLAME_THROWER_D,
  ACTOR_LAPIS_TORCH_A,
  ACTOR_LAPIS_TORCH_B,
  ACTOR_LIT_TORCH,
  ACTOR_LAPIS_ICE_CONE_A,
  ACTOR_LAPIS_ICE_CONE_B,
  ACTOR_ICE_SHEET,
  ACTOR_UNLIT_TORCH,
  ACTOR_SECOND_ICE_SHEET
};

/// Balm's room: a flame thrower and the torch its fireball stops at, two dragon
/// statues, five ice cones and Balm.
enum MarsLighthouseEntranceBalmActorId {
  ACTOR_BALM_FLAME_THROWER = ACTOR_FIRST_PLACED,
  ACTOR_BALM_TORCH,
  ACTOR_BALM_WEST_STATUE,
  ACTOR_BALM_EAST_STATUE,
  ACTOR_BALM_ICE_CONE_A,
  ACTOR_BALM_ICE_CONE_B,
  ACTOR_BALM_ICE_CONE_C,
  ACTOR_BALM_ICE_CONE_D,
  ACTOR_BALM_ICE_CONE_E,
  ACTOR_BALM
};

/// The hidden circle's room: a lit torch and a torch Blaze sends a fireball
/// from on each side, and the ice sheet the fireball cracks.
enum MarsLighthouseEntranceHiddenCircleActorId {
  ACTOR_WEST_LIT_TORCH = ACTOR_FIRST_PLACED,
  ACTOR_WEST_TORCH,
  ACTOR_EAST_LIT_TORCH,
  ACTOR_EAST_TORCH,
  ACTOR_HIDDEN_CIRCLE_ICE_SHEET
};

/// The Valkyrie Mail room: the dragon statue pushed into the flames, the upper
/// flame thrower and its torch, the dragon face that wakes and the torch below
/// it, six ice cones and the face's fireball.
enum MarsLighthouseEntranceValkyrieMailChestActorId {
  ACTOR_UPPER_FLAME_THROWER = ACTOR_FIRST_PLACED + 1,
  ACTOR_UPPER_TORCH,
  ACTOR_DRAGON_FACE,
  ACTOR_LOWER_TORCH,
  ACTOR_VALKYRIE_ICE_CONE_A,
  ACTOR_VALKYRIE_ICE_CONE_B,
  ACTOR_VALKYRIE_ICE_CONE_C,
  ACTOR_VALKYRIE_ICE_CONE_D,
  ACTOR_VALKYRIE_ICE_CONE_E,
  ACTOR_VALKYRIE_ICE_CONE_F,
  ACTOR_DRAGON_FACE_FIREBALL = ACTOR_FIRST_PLACED + 12
};
#ifndef TLA_MARS_LIGHTHOUSE_ENTRANCE_RIDE_H
#define TLA_MARS_LIGHTHOUSE_ENTRANCE_RIDE_H

/// The ride work seen from one flame thrower's entry: the work's header, then
/// the entry, as the getters below index it.
struct RideRow {
  u8 beforeEntries[20];
  struct RideEntry entry;
};

extern u8 gInventorySnapshot[];

void Actor_SetSpriteColorMode(struct Actor *object, s32 value);

#endif

s32 MarsLighthouseEntrance_SetUpStatue(struct Actor *object) {
  Actor_SetSpriteFlags(object, 0);
  return 0;
}

void MarsLighthouseEntrance_FightFugue(void) {
  Field_RequestDjinnBattle(ACTOR_FUGUE, DJINNI_FUGUE);
}

void MarsLighthouseEntrance_FightBalm(void) {
  Field_RequestDjinnBattle(ACTOR_BALM, DJINNI_BALM);
}

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern struct ActorPlacement gMarsLighthouseEntrance_Placements[];
extern struct ActorPlacement gMarsLighthouseEntrance_OutsidePlacements[];
extern struct ActorPlacement gMarsLighthouseEntrance_FuguePlacements[];
extern struct ActorPlacement gMarsLighthouseEntrance_LapisPlacements[];
extern struct ActorPlacement gMarsLighthouseEntrance_BalmPlacements[];
extern struct ActorPlacement gMarsLighthouseEntrance_HiddenCirclePlacements[];
extern struct ActorPlacement gMarsLighthouseEntrance_ValkyriePlacements[];
extern struct ActorPlacement gMarsLighthouseEntrance_SolBladePlacements[];

s32 MarsLighthouseEntrance_LightDragonFace(void) {
  Ride_SetEntryState(0, FLAME_THROWER_DRY | 1);
  return 0;
}

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  if (gMainRoutineWork.next == SCENE_MARS_LIGHTHOUSE_OUTSIDE)
    return gMarsLighthouseEntrance_OutsidePlacements;
  if (gMainRoutineWork.next == SCENE_MARS_LIGHTHOUSE_ENTRANCE_FUGUE)
    return gMarsLighthouseEntrance_FuguePlacements;
  if (gMainRoutineWork.next == SCENE_MARS_LIGHTHOUSE_ENTRANCE_TELEPORT_LAPIS)
    return gMarsLighthouseEntrance_LapisPlacements;
  if (gMainRoutineWork.next == SCENE_MARS_LIGHTHOUSE_ENTRANCE_BALM)
    return gMarsLighthouseEntrance_BalmPlacements;
  if (gMainRoutineWork.next == SCENE_MARS_LIGHTHOUSE_ENTRANCE_HIDDEN_CIRCLE)
    return gMarsLighthouseEntrance_HiddenCirclePlacements;
  if (gMainRoutineWork.next ==
      SCENE_MARS_LIGHTHOUSE_ENTRANCE_VALKYRIE_MAIL_CHEST)
    return gMarsLighthouseEntrance_ValkyriePlacements;
  if (gMainRoutineWork.next == SCENE_MARS_LIGHTHOUSE_ENTRANCE_SOL_BLADE_CHEST)
    return gMarsLighthouseEntrance_SolBladePlacements;
  return gMarsLighthouseEntrance_Placements;
}

// XXX: what Blaze does otherwise once the field work's blazeLongSpray is set
// (its mode 3 for 2 when it finds no torch); the rest of blaze.c's reading of
// the mode would settle it.
/// Unused: as the dragon hall's, readies Blaze for a dragon statue.
UNUSED void MarsLighthouseEntrance_BeginBlazeOnStatue(void) {
  struct FieldWork *event = Ram_HeapSlots->fieldWork;

  event->blazeLongSpray = 1;
}

extern struct MapCell gMapGridCellBuffer[];

#include "shared/map_cell_get.h"

/// Swaps a map cell's collision code and attribute for `value`, returning the
/// old.
s32 MapCell_Swap(u32 layer, s32 x, s32 y, s32 value) {
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  struct MapCell *cell;
  s32 old;

  if (map == NULL)
    return 0;
  if (layer <= 2)
    cell = map->layers[layer].cells;
  else
    cell = gMapGridCellBuffer;
  cell += x + (y << 7);
  old = (cell->collisionCode << 8) | cell->attributeB;
  cell->collisionCode = value >> 8;
  cell->attributeB = value;
  return old;
}

#include "../field/actor_set_sprite_priority.c"

void MapGrid_SetCellHeight();
void Teleport_PlayWhirl(void);
void Teleport_PlaySparks(void);
extern u16 gMarsLighthouseEntrance_HoleMaskVram;
extern u32 gMarsLighthouseEntrance_HoleMaskSprite[3];
extern s16 gMarsLighthouseEntrance_DragonFireTimer;
void Map_CopyPalette(s32 scene, s32 a1);
s32 FieldEffect_StartBlankOverlay();
s32 Weather_StartRain();
extern s32 Data_02006878;

s32 MapCell_Get();
s32 Map_GetTerrainHeight();
void Scene_SetActorSpritePriority();

void MarsLighthouseEntrance_SetIceConeCells(struct Actor *object) {
  s32 v10, v6, v7, v8, v9;
  struct Actor *v5;
  u8 *v11;

  v5 = object;
  v9 = FIELD_CELL(v5->x.fixed);
  v10 = FIELD_CELL(v5->z.fixed);
  v8 = MapCell_Get(0, v9, v10);
  v6 = MapCell_Get(2, v9, v10);
  v11 = &v5->layer;
  v7 = ((Map_GetTerrainHeight(v5->layer, v5->x.fixed, v5->z.fixed)) >> 19);
  if (Event_GetFlag(v5->timer)) {
    v6 &= -0xff01;
    v8 &= -129;
    if ((v5->groundHeight >> 19) == v7)
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
  object->collisionFlags &= ~ACTOR_SOLID;
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
  struct EffectHolder *holder = Ram_HeapSlots->fieldEffectWork;
  struct Actor *object = Actor_Get(holder->objectId);

  if (index <= 7) {
    struct EffectOptions options;

    options.priority = 0;
    options.type = SPRITE_BLUE_CRYSTAL;
    options.startScaleX = options.startScaleY = Q16_ONE;
    options.targetScaleX = options.targetScaleY = Q16_ONE / 2;
    options.rotation = (Random16() & 15) << 12;
    FieldEffect_Spawn(object->x.fixed,
                      object->y.fixed - index * 3 * px(1) + px(16),
                      object->z.fixed, ((Random16() & 15) - 8) << 13, 0, 0,
                      EFFECT_USE_ROTATION | EFFECT_USE_TYPE |
                          EFFECT_USE_START_SCALE | EFFECT_USE_PRIORITY,
                      &options);
    object->scaleY -= 0x1f00;
  }
  if (mode == 1) {
    object->scaleY = Q16_ONE;
    Event_SetFlag(object->timer);
    MarsLighthouseEntrance_SetIceConeCells(Actor_Get(holder->objectId));
    object->x.fixed = object->y.fixed = object->z.fixed = 0;
  }
}

/// Pound's trigger on an ice cone: its two arguments passed on as they come.
void MarsLighthouseEntrance_StartPoundIceCone(s32 mode, s32 index) {
  MarsLighthouseEntrance_PoundIceCone(mode, index);
}

extern u32 gMarsLighthouseEntrance_IceChipScript[];

/// Throws chips of ice from an actor as its ice cracks or breaks.
void MarsLighthouseEntrance_ScatterIceChips(s32 id) {
  struct Actor *source = Actor_Get(id);
  s32 i;

  i = 5;
next:
  {
    s32 x = source->x.fixed + (Random16() << 4) - (Random16() << 4);
    struct Actor *object = Import_ActorCreate_(
        0x145, x, (Random16() << 3) + source->y.fixed + px(4),
        source->z.fixed + (Random16() << 4) - (Random16() << 4));

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
      object->velocityY = angle * 4 + px(2);
      object->velocityZ = Iwram_MulQ16(radius * 8, Trig_Cos(angle));
      object->velocityX = Iwram_MulQ16(radius * 8, Math_Sin(angle));
      object->acceleration = 0;
      object->gravity = px(33) / 100;
      object->scaleX = scale;
      object->scaleY = scale;
      object->bounce = Q16_ONE / 2;
      Actor_SetSpriteColorMode(object, 1);
    }
  }
  if (--i >= 0)
    goto next;
}

/// Grind on the ice at the dead end: the crack runs to the ice sheet
/// a floor below.
void MarsLighthouseEntrance_GrindIce(void) {
  s32 x;
  s32 count;

  Event_PlaySound(SE_CRUMBLE);
  x = 32;
  count = 6;
step:
  MapGrid_CopyCellPictureCellsRect_(x, 125, 3, 3, 39, 85);
  MarsLighthouseEntrance_ScatterIceChips(ACTOR_DEAD_END);
  count--;
  System_Wait(10);
  x += 3;
  if (count >= 0)
    goto step;
  ObjectTable_Snapshot();
  FieldEvent_SetExitRequest(20);
}

void MarsLighthouseEntrance_CheckIce(void) {
  // "Felix checked the ice."
  FieldMessage_Show((s32)MsgMarsLighthouseEntranceCheckedIce, 1);
  // "A shaft of ice is poking up through the floor."
  FieldMessage_Show((s32)MsgMarsLighthouseEntranceCheckedIce + 1, 1);
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
    Actor_Get(gMainRoutineWork.leaderActorId)->update = NULL;
    FieldActor_SetSpriteColor(Actor_Get(gMainRoutineWork.leaderActorId), 0);
    Actor_SetHorizontalPositionWithTerrain_(gMainRoutineWork.leaderActorId,
                                            px(688), px(360));
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
    gMainRoutineWork.teleportState = 2;
    FieldEvent_SetExitRequest(18);
    FieldEvent_Wait(40);
    Teleport_StopShimmer();
  }
}

void MarsLighthouseEntrance_TeleportFromLapis(void) {
  Teleport_StartShimmer();
  Teleport_PlaySparks();
  gMainRoutineWork.teleportState = 2;
  FieldEvent_SetExitRequest(17);
  FieldEvent_Wait(40);
  Teleport_StopShimmer();
}

/// Burst on the cracked ice sheet: a flash, the sheet gone, chips
/// flying (FLAG_LAPIS_SHEET_BURST).
void MarsLighthouseEntrance_BurstIceSheet(s32 state, s32 frame) {
  if (state == 1) {
    Event_PlaySound(SE_FLASH);
    PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
    PaletteFade_Interpolate(60);
    MapGrid_CopyCellPictureCellsRect_(47, 94, 10, 6, 47, 100);
    Map_CopyCellAttributeRect_(110, 40, 12, 4, 46, 40);
    Actor_SetPosition(ACTOR_ICE_SHEET, 0, 0);
    Flag_Set(FLAG_LAPIS_SHEET_BURST);
  }
  if (frame > 96 && frame < 272 && frame % 30 == 0)
    MarsLighthouseEntrance_ScatterIceChips(ACTOR_ICE_SHEET);
  if (frame == 276) {
    PaletteFade_ApplyTarget(RGB(31, 31, 31), PALETTES_ALL);
    PaletteFade_Interpolate(20);
  }
}

/// Blaze on the dragon statue: it shoots a fireball at the ice sheet.
void MarsLighthouseEntrance_BlazeLapisStatue(void) {
  MarsLighthouseEntrance_ShootFireballAtLapisIceSheet();
}

/// The camera follows the fireball to the ice sheet it cracks.
void MarsLighthouseEntrance_WatchFireballCrackIce(void) {
  gFieldWork->psynergy = 0;
  Flag_Clear(FLAG_MENU_OPEN);
  Flag_Clear(FLAG_FIELD_HANDLING_REQUESTS);
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

void MarsLighthouseEntrance_DrawLapisCrackedSheet(void) {
  MapGrid_CopyCellAttributes(14, 55, 14, 3, 14, 41);
  MapGrid_CopyCellsLayered(64, 64, 14, 3, 78, 41);
  MapGrid_CopyCellsLayered(64, 64, 14, 3, 14, 104);
  MapGrid_CopyCellAttributes(64, 64, 14, 3, 78, 41);
  MapGrid_CopyCellAttributes(64, 64, 14, 3, 14, 105);
}

/// The fireball reaches the ice sheet and cracks it.
void MarsLighthouseEntrance_CrackLapisIceSheet(void) {
  struct Actor *object = Actor_Get(ACTOR_SECOND_ICE_SHEET);

  if (!Flag_IsSet(FLAG_LAPIS_SECOND_SHEET_BURST)) {
    Event_PlaySound(SE_ICE_CRACK);
    Flag_Set(FLAG_LAPIS_SECOND_SHEET_SHOWN);
    object->x.fixed = object->homeX << 16;
    object->z.fixed = object->homeZ << 16;
    Flag_Set(FLAG_LAPIS_SECOND_SHEET_CRACKED);
    MarsLighthouseEntrance_DrawLapisCrackedSheet();
  }
}

#define DEFINE_BURST_ICE_SHEET(name, n1, n2, n3, n4, n5, n6, n7, n8, n9)       \
  void name(s32 state, s32 frame) {                                            \
    if (state == 1) {                                                          \
      Event_PlaySound(SE_FLASH);                                               \
      PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);                \
      PaletteFade_Interpolate(60);                                             \
      MapGrid_CopyCellsLayered(n1, n2, 8, n3, n1, n4);                         \
      MapGrid_CopyCellAttributes(n5, n6, 8, n7, n1, n6);                       \
      Actor_SetPosition(n8, 0, 0);                                             \
      Flag_Set(n9);                                                            \
    }                                                                          \
    if (frame > 96 && frame < 272 && frame % 30 == 0)                          \
      MarsLighthouseEntrance_ScatterIceChips(n8);                              \
    if (frame == 276) {                                                        \
      PaletteFade_ApplyTarget(RGB(31, 31, 31), PALETTES_ALL);                  \
      PaletteFade_Interpolate(20);                                             \
    }                                                                          \
  }

DEFINE_BURST_ICE_SHEET(MarsLighthouseEntrance_BurstLapisIceSheet, 28, 92, 8,
                       100, 92, 38, 8, ACTOR_SECOND_ICE_SHEET,
                       FLAG_LAPIS_SECOND_SHEET_BURST)

/// Move on the dragon statue to cell x 20: it blocks the last flame thrower
/// (FLAG_LAPIS_STATUE_BLOCKS_FLAMES).
void MarsLighthouseEntrance_PushStatueIntoFlames(s32 unused, s32 id) {
  if (id == ACTOR_PUSHED_STATUE) {
    struct Actor *object = Actor_Get(ACTOR_PUSHED_STATUE);

    if (FIELD_CELL(object->x.fixed) == 20) {
      FieldEvent_Wait(5);
      object->z.fixed += px(1);
      BYTE_AT(object, 98) = 1;
      Flag_Set(FLAG_LAPIS_STATUE_BLOCKS_FLAMES);
    }
  }
}

void MarsLighthouseEntrance_BlockRetreatAtChest(void) {
  GameFlag_Set(FLAG_RETREAT_BLOCKED);
}

void MarsLighthouseEntrance_TeleportFromBalm(void) {
  Teleport_StartShimmer();
  Teleport_PlaySparks();
  gMainRoutineWork.teleportState = 2;
  FieldEvent_SetExitRequest(14);
  FieldEvent_Wait(40);
  Teleport_StopShimmer();
}

/// Move sends the statue sliding over the ice to its next stop: from cell
/// column 11 to x 152, and from column 10 to x 200, both at z 792.
void MarsLighthouseEntrance_SlideStatueOnIce(s32 unused, s32 id) {
  struct Actor *object = Actor_Get(id);

  if (FIELD_CELL(object->x.fixed) == 11) {
    object->speedLimit = px(1) / 4;
    object->acceleration = px(1) / 8;
    FieldActor_SetMoveTargetFixed(object, px(152), object->y.fixed, px(792));
  }
  if (FIELD_CELL(object->x.fixed) == 10) {
    object->speedLimit = px(1) / 4;
    object->acceleration = px(1) / 8;
    FieldActor_SetMoveTargetFixed(object, px(200), object->y.fixed, px(792));
  }
}

void MarsLighthouseEntrance_BlazeHiddenCircleWestStatue(void) {
  FieldCamera_SetSpeed(1);
  FieldCamera_MoveTo(88, CAMERA_KEEP, 696, 1);
  MarsLighthouseEntrance_ShootFireballAtHiddenCircleIceSheet();
}

void MarsLighthouseEntrance_WatchFireballCrackHiddenCircleIce(void) {
  gFieldWork->psynergy = 0;
  Flag_Clear(FLAG_MENU_OPEN);
  Flag_Clear(FLAG_FIELD_HANDLING_REQUESTS);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(120);
  Event_End();
}

void MarsLighthouseEntrance_CrackHiddenCircleIceSheet(void) {
  struct Actor *object = Actor_Get(ACTOR_HIDDEN_CIRCLE_ICE_SHEET);

  if (!Flag_IsSet(FLAG_HIDDEN_CIRCLE_SHEET_BURST)) {
    Event_PlaySound(SE_ICE_CRACK);
    Flag_Set(FLAG_HIDDEN_CIRCLE_SHEET_SHOWN);
    object->x.fixed = object->homeX << 16;
    object->z.fixed = object->homeZ << 16;
    Flag_Set(FLAG_HIDDEN_CIRCLE_SHEET_CRACKED);
  }
}

void MarsLighthouseEntrance_BlazeHiddenCircleEastStatue(void) {
  MarsLighthouseEntrance_ShootFireballThroughIce();
}

void MarsLighthouseEntrance_WatchFireballMeltIce(void) {
  gFieldWork->psynergy = 0;
  Flag_Clear(FLAG_MENU_OPEN);
  Flag_Clear(FLAG_FIELD_HANDLING_REQUESTS);
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

void MarsLighthouseEntrance_TeleportFromHiddenCircle(void) {
  Teleport_StartShimmer();
  Teleport_PlaySparks();
  gMainRoutineWork.teleportState = 2;
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

DEFINE_BURST_ICE_SHEET(MarsLighthouseEntrance_BurstHiddenCircleIceSheet, 12, 95,
                       6, 101, 76, 40, 5, ACTOR_HIDDEN_CIRCLE_ICE_SHEET,
                       FLAG_HIDDEN_CIRCLE_SHEET_BURST)

/// The fireball melts the ice in its path and opens the way south.
void MarsLighthouseEntrance_MeltIceInPath(void) {
  MapGrid_CopyCellAttributes(35, 11, 3, 6, 53, 11);
  MapGrid_CopyCellsLayered(35, 75, 3, 6, 117, 11);
  MapGrid_CopyCellsLayered(35, 75, 3, 6, 53, 75);
  Flag_Set(FLAG_HIDDEN_CIRCLE_ICE_MELTED);
}

void MarsLighthouseEntrance_ClearFireballRow(void) {
  MapGrid_CopyCellAttributes(25, 9, 11, 3, 26, 9);
}

void MarsLighthouseEntrance_TeleportFromValkyrieWest(void) {
  Teleport_StartShimmer();
  Teleport_PlaySparks();
  gMainRoutineWork.teleportState = 2;
  FieldEvent_SetExitRequest(12);
  FieldEvent_Wait(40);
  Teleport_StopShimmer();
}

void MarsLighthouseEntrance_TeleportFromValkyrieEast(void) {
  Teleport_StartShimmer();
  Teleport_PlaySparks();
  gMainRoutineWork.teleportState = 2;
  FieldEvent_SetExitRequest(13);
  FieldEvent_Wait(40);
  Teleport_StopShimmer();
}

/// Move on the statue to column 5 sets it in the flames' way.
void MarsLighthouseEntrance_PushStatueIntoValkyrieFlames(s32 unused, s32 id) {
  if (id == ACTOR_PUSHED_STATUE) {
    struct Actor *object = Actor_Get(ACTOR_PUSHED_STATUE);

    if (FIELD_CELL(object->x.fixed) == 5) {
      FieldEvent_Wait(5);
      object->z.fixed += px(1);
      Flag_Set(FLAG_VALKYRIE_STATUE_BLOCKS_FLAMES);
    }
  }
}

void MarsLighthouseEntrance_DrawHoleMask(void) {
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  s32 *camera = (s32 *)((u8 *)map + 228);
  s32 x = camera[0] & 0xffff0000;
  s32 cameraZ = camera[1] & 0xffff0000;
  s32 originY = map->origin[1];
  s32 tile =
      gVramCacheEntries[gMarsLighthouseEntrance_HoleMaskVram].offset >> 5;
  struct Actor *leader = Actor_Get(gMainRoutineWork.leaderActorId);
  s32 y, z, depth, sx, sy;
  u32 *word;

  x = (FIELD_CELL(leader->x.fixed) << 20) - x;
  y = Map_GetTerrainHeight(0, leader->x.fixed, leader->z.fixed) - originY;
  z = (FIELD_CELL(leader->z.fixed) << 20) - cameraZ - originY;
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
    u16 winout = WINOUT_WIN01_BG0 | WINOUT_WIN01_BG1 | WINOUT_WIN01_BG2 |      \
                 WINOUT_WIN01_BG3 | WINOUT_WIN01_OBJ | WINOUT_WINOBJ_BG0 |     \
                 WINOUT_WINOBJ_BG1 | WINOUT_WINOBJ_BG2 | WINOUT_WINOBJ_BG3;    \
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

  actor = Actor_Get(gMainRoutineWork.leaderActorId);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_SetSpriteFlags(Actor_Get(gMainRoutineWork.leaderActorId), 0);
  actor->update = NULL;
  {
    s32 value = 0x100;

    FieldActor_ShowEmote(gMainRoutineWork.leaderActorId, value, 0);
  }
  Actor_SelectAnimation(actor, 27);
  Actor_SetAnimationSpeed(actor, 48);
  System_Wait(30);
  actor->facing = FACING_SOUTH;
  actor->motionFlags = 0;
  buffer = gMapGridCellBuffer;
  x = FIELD_CELL(actor->x.fixed);
  z = FIELD_CELL(actor->z.fixed);
  map = Ram_HeapSlots->mapGrid;
  if (map != NULL)
    cells = map->layers[actor->layer & 3].cells;
  else
    cells = buffer;
  cells += x + (z << 7) + 128;
  if (!(cells->attributeB & 0x40)) {
    SET_SPRITE_WINDOW_LAYERS();
    REG_DISPCNT |= DISPCNT_OBJWIN_ON;
    MarsLighthouseEntrance_StartHoleMask();
  }
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_LEADER_ARMS_RAISED);
  FieldEvent_Wait(16);
  i = 0;
next:
  if (i == 5)
    Event_PlaySound(SE_DUST_PUFF);
  actor->scaleX -= Q16_ONE / 64;
  actor->scaleY -= 0x300;
  actor->y.fixed -= 0x9999;
  System_Wait(1);
  i++;
  if (i <= 39)
    goto next;
  Actor_Get(gMainRoutineWork.leaderActorId)->spriteStorageKind = 0;
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(arg);
}

extern s32 gMarsLighthouseEntrance_DragonFaceGoesLeft[];
extern struct Trigger gMarsLighthouseEntrance_Triggers[];
extern struct Trigger gMarsLighthouseEntrance_OutsideTriggers[];
extern struct Trigger gMarsLighthouseEntrance_FugueTriggers[];
extern struct Trigger gMarsLighthouseEntrance_LapisTriggers[];
extern struct Trigger gMarsLighthouseEntrance_BalmTriggers[];
extern struct Trigger gMarsLighthouseEntrance_HiddenCircleTriggers[];
extern struct Trigger gMarsLighthouseEntrance_ValkyrieTriggers[];
extern struct Trigger gMarsLighthouseEntrance_SolBladeTriggers[];

/// The dragon face wakes and begins its slow way left, breathing fire.
void MarsLighthouseEntrance_WakeDragonFace(void) {
  Event_PlaySound(SE_BUTTON_PRESS);
  Flag_Set(FLAG_NO_PLACE_TO_SAVE);
  Actor_SetScript(ACTOR_DRAGON_FACE,
                  gMarsLighthouseEntrance_DragonFaceGoesLeft);
  Ride_SetEntryState(0, FLAME_FORWARD | 3);
  Flag_Set(FLAG_DRAGON_FACE_AWAKE);
}

void MarsLighthouseEntrance_SlideFromEdge(void) {
  Leader_SlideOnIceFromEdge();
  MarsLighthouseEntrance_SlideIfRiding();
}

void GameFlag_SetByte(s32 flag, s32 value);

/// The leader drops through a cracked floor tile.
void MarsLighthouseEntrance_FallThroughCrack(const struct MapCell *cell) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.leaderActorId);
  s32 z = leader->z.fixed;
  s32 i = FIELD_CELL(leader->x.fixed);

  GameFlag_SetByte(0x340, i);
  GameFlag_SetByte(0x348, FIELD_CELL(z));
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_SetSpriteFlags(Actor_Get(gMainRoutineWork.leaderActorId), 0);
  leader->update = NULL;
  FieldActor_ShowEmote(gMainRoutineWork.leaderActorId,
                       EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_SelectAnimation(leader, ANIM_LEADER_WALK_BACKWARDS);
  Actor_SetAnimationSpeed(leader, 48);
  System_Wait(30);
  leader->facing = FACING_SOUTH;
  leader->motionFlags = 0;
  if ((cell->attributeB & 64) == 0) {
    {
      u16 winout = WINOUT_WIN01_BG0 | WINOUT_WIN01_BG1 | WINOUT_WIN01_BG2 |
                   WINOUT_WIN01_BG3 | WINOUT_WIN01_OBJ | WINOUT_WINOBJ_BG0 |
                   WINOUT_WINOBJ_BG1 | WINOUT_WINOBJ_BG2 | WINOUT_WINOBJ_BG3;

      REG_WINOUT = winout;
    }
    REG_DISPCNT |= DISPCNT_OBJWIN_ON;
    MarsLighthouseEntrance_StartHoleMask();
  }
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_LEADER_ARMS_RAISED);
  FieldEvent_Wait(16);
  i = 0;
next:
  if (i == 5)
    Event_PlaySound(SE_DUST_PUFF);
  leader->scaleX -= Q16_ONE / 64;
  leader->scaleY -= 0x300;
  leader->y.fixed -= 0x9999;
  System_Wait(1);
  i++;
  if (i <= 39)
    goto next;
  Actor_Get(gMainRoutineWork.leaderActorId)->spriteStorageKind = 0;
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(11);
}

void MarsLighthouseEntrance_JumpOrSlide(void) {
  Field_TryJumpForward();
  Leader_SlideOnIceAhead();
}

void MarsLighthouseEntrance_CheckCrackedFloor(void) {
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  struct MainRoutineWork *party = &gMainRoutineWork;
  struct Actor *object;
  s32 x;
  s32 z;
  struct MapCell *cell;

  object = Actor_Get(party->leaderActorId);
  x = FIELD_CELL(object->x.fixed);
  z = FIELD_CELL(object->z.fixed);
  cell = gMapGridCellBuffer;
  if (map != NULL)
    cell = map->layers[object->layer & 3].cells;
  cell += x + (z << 7);
  if (cell->attributeB & 64)
    MarsLighthouseEntrance_FallThroughCrack(cell + 128);
}

const struct Trigger *Scene_GetTriggers(void) {
  if (gMainRoutineWork.next == SCENE_MARS_LIGHTHOUSE_OUTSIDE)
    return gMarsLighthouseEntrance_OutsideTriggers;
  if (gMainRoutineWork.next == SCENE_MARS_LIGHTHOUSE_ENTRANCE_FUGUE)
    return gMarsLighthouseEntrance_FugueTriggers;
  if (gMainRoutineWork.next == SCENE_MARS_LIGHTHOUSE_ENTRANCE_TELEPORT_LAPIS)
    return gMarsLighthouseEntrance_LapisTriggers;
  if (gMainRoutineWork.next == SCENE_MARS_LIGHTHOUSE_ENTRANCE_BALM)
    return gMarsLighthouseEntrance_BalmTriggers;
  if (gMainRoutineWork.next == SCENE_MARS_LIGHTHOUSE_ENTRANCE_HIDDEN_CIRCLE)
    return gMarsLighthouseEntrance_HiddenCircleTriggers;
  if (gMainRoutineWork.next ==
      SCENE_MARS_LIGHTHOUSE_ENTRANCE_VALKYRIE_MAIL_CHEST)
    return gMarsLighthouseEntrance_ValkyrieTriggers;
  if (gMainRoutineWork.next == SCENE_MARS_LIGHTHOUSE_ENTRANCE_SOL_BLADE_CHEST)
    return gMarsLighthouseEntrance_SolBladeTriggers;
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
      Ride_SetEntryState(0, FLAME_FORWARD | 3);
      *state = 1;
    }
  } else {
    if (((struct HalfwordField *)state)->value == 1) {
      Ride_SetEntryState(0, FLAME_THROWER_DRY | 3);
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
  struct FieldSprite *sprite = actor->sprite;

  sprite->rotation += TRIG_TURN / 32;
  if (++actor->timer == 8)
    actor->velocityY = px(4);
  if (actor->timer > 9) {
    actor->x.fixed += px(4);
    actor->scaleX -= Q16_ONE / 32;
    actor->scaleY -= Q16_ONE / 32;
  }
  if (actor->timer == 32) {
    actor->x.fixed = 0;
    actor->z.fixed = 0;
    actor->update = NULL;
  }
}

extern struct Trigger gMarsLighthouseEntrance_Triggers[];
extern struct Trigger gMarsLighthouseEntrance_OutsideTriggers[];
extern struct Trigger gMarsLighthouseEntrance_FugueTriggers[];
extern struct Trigger gMarsLighthouseEntrance_LapisTriggers[];
extern struct Trigger gMarsLighthouseEntrance_BalmTriggers[];
extern struct Trigger gMarsLighthouseEntrance_HiddenCircleTriggers[];
extern struct Trigger gMarsLighthouseEntrance_ValkyrieTriggers[];
extern struct Trigger gMarsLighthouseEntrance_SolBladeTriggers[];

void MarsLighthouseEntrance_WaitForDragonFace(struct Actor *object) {
  if (ABS(object->x.fixed - object->watchedActor->x.fixed) > px(32))
    return;
  object->timer = 0;
  object->update = (void (*)(void))MarsLighthouseEntrance_FlingIceCone;
}

void MarsLighthouseEntrance_StopSlideWhileRiding(void) {
  if (Ride_IsActive())
    Leader_SetSlideStopped(1);
}

void FlameThrowers_Start(u16 *targets, u16 *stops, s32 onStopped,
                         s32 hitTrigger);
void Ride_SetEntryCount(s32 index, s16 value);
void Palette_SetColorsFade(u16 *slots, u16 *colours, u16 *a, u16 *b);
void MarsLighthouseEntrance_ShootValkyrieUpperFireball(void);
extern s32 gMarsLighthouseEntrance_DragonFaceAtLeft[];
extern s16 gMarsLighthouseEntrance_FugueIceCones[];
extern u16 gMarsLighthouseEntrance_LapisBeamTargets[];
extern u16 gMarsLighthouseEntrance_LapisBeamStops[];
extern s16 gMarsLighthouseEntrance_LapisIceCones[];
extern u16 gMarsLighthouseEntrance_BalmBeamTargets[];
extern u16 gMarsLighthouseEntrance_BalmBeamStops[];
extern s16 gMarsLighthouseEntrance_BalmIceCones[];
extern u16 gMarsLighthouseEntrance_ValkyrieUpperBeamTargets[];
extern u16 gMarsLighthouseEntrance_ValkyrieUpperBeamStops[];
extern u16 gMarsLighthouseEntrance_ValkyrieLowerBeamTargets[];
extern u16 gMarsLighthouseEntrance_ValkyrieLowerBeamStops[];
extern s16 gMarsLighthouseEntrance_ValkyrieIceCones[];
extern s16 gMarsLighthouseEntrance_FadeSlots[];
extern s16 gMarsLighthouseEntrance_FadeColorsFrom[];
extern s16 gMarsLighthouseEntrance_FadeColorsTo[];
extern u32 gMarsLighthouseEntrance_FadeOrder[];

s32 Scene_Initialize(void) {
  s32 scene = gMainRoutineWork.next;
  s32 entrance = gMainRoutineWork.entrance;
  struct Actor *object;
  s32 i;

  // XXX: Data_02006878 is cleared here and read nowhere in this overlay; a
  // watch on the word in the running game would say what else reads it.
  Data_02006878 = 0;
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  Event_SetFlag(FLAG_IN_MARS_LIGHTHOUSE);
  Ride_Create();
  if (scene == SCENE_MARS_LIGHTHOUSE_OUTSIDE) {
    Weather_StartRain(1);
  } else if (scene == SCENE_MARS_LIGHTHOUSE_ENTRANCE_FUGUE) {
    FieldEffect_StartBlankOverlay(0);
#if !TLA_PATCH_1
    FieldActor_SetSpritePriority(ACTOR_FUGUE, 3);
#endif
    Actor_Freeze(ACTOR_DEAD_END);
    if (Event_GetFlag(FLAG_MARS_LIGHTHOUSE_THAWED)) {
      MapGrid_CopyCellsLayered(35, 41, 24, 10, 3, 53);
      MapGrid_CopyCellsLayered(35, 105, 24, 10, 3, 117);
    }
    MarsLighthouseEntrance_PlaceIceCones(
        (s32)gMarsLighthouseEntrance_FugueIceCones);
    if (Event_GetFlag(FLAG_ICE_GROUND))
      MapGrid_CopyCellsLayered(50, 125, 3, 3, 39, 85);
  } else if (scene == SCENE_MARS_LIGHTHOUSE_ENTRANCE_TELEPORT_LAPIS) {
    FieldEffect_StartBlankOverlay(0);
    FlameThrowers_Start((s32)gMarsLighthouseEntrance_LapisBeamTargets,
                        (s32)gMarsLighthouseEntrance_LapisBeamStops, 0, 88);
    MarsLighthouseEntrance_PlaceIceCones(
        (s32)gMarsLighthouseEntrance_LapisIceCones);
    if (Event_GetFlag(FLAG_LAPIS_SHEET_BURST)) {
      MapGrid_CopyCellsLayered(47, 94, 10, 6, 47, 100);
      MapGrid_CopyCellAttributes(110, 40, 12, 4, 46, 40);
      Actor_SetPosition(ACTOR_ICE_SHEET, 0, 0);
    } else if (Event_GetFlag(FLAG_ICE_GROUND)) {
      Event_SetFlag(FLAG_LAPIS_SHEET_CRACKED);
      Actor_Get(ACTOR_ICE_SHEET)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    } else {
      Actor_SetPosition(ACTOR_ICE_SHEET, 0, 0);
    }
    Actor_Get(ACTOR_SECOND_ICE_SHEET)->priorityFlags |=
        ACTOR_PRIORITY_UNDERFOOT;
    FieldActor_SetSpritePriority(ACTOR_SECOND_ICE_SHEET, 2);
    if (Event_GetFlag(FLAG_LAPIS_SECOND_SHEET_BURST)) {
      MapGrid_CopyCellsLayered(28, 92, 8, 8, 28, 100);
      MapGrid_CopyCellAttributes(92, 38, 8, 8, 28, 38);
      MarsLighthouseEntrance_DrawLapisCrackedSheet();
      Actor_SetPosition(ACTOR_SECOND_ICE_SHEET, 0, 0);
    } else if (Event_GetFlag(FLAG_LAPIS_SECOND_SHEET_CRACKED)) {
      MarsLighthouseEntrance_DrawLapisCrackedSheet();
      Event_SetFlag(FLAG_LAPIS_SECOND_SHEET_SHOWN);
    } else {
      Actor_SetPosition(ACTOR_SECOND_ICE_SHEET, 0, 0);
    }
    if (Event_GetFlag(FLAG_LAPIS_STATUE_BLOCKS_FLAMES)) {
      Actor_SetPosition(ACTOR_PUSHED_STATUE, 328, 120);
      Actor_Get(ACTOR_PUSHED_STATUE)->riseCounter = 1;
    }
    if (Event_GetFlag(FLAG_MARS_LIGHTHOUSE_THAWED))
      Actor_SetPosition(ACTOR_UNLIT_TORCH, 0, 0);
    else
      Actor_SetPosition(ACTOR_LIT_TORCH, 0, 0);
    if (entrance == 20) {
      Event_Start();
      FieldEvent_ResolvePendingActions(0);
      FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
      MapGrid_SetShake(0, 2, 1);
      Actor_SetPosition(gMainRoutineWork.leaderActorId, 0, 0);
      Actor_SetPosition(ACTOR_ICE_SHEET, 0, 0);
      Actor_Get(ACTOR_ICE_SHEET)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
      Event_StartScreenTransition();
      Event_WaitTransitionFrames();
      object = Actor_Get(ACTOR_ICE_SHEET);
      Actor_SetPosition(ACTOR_ICE_SHEET, object->timer, object->userValue);
      Event_PlaySound(SE_ICE_CRACK);
      i = 2;
      do {
        MarsLighthouseEntrance_ScatterIceChips(ACTOR_ICE_SHEET);
        System_Wait(1);
        i--;
      } while (i >= 0);
      Event_SetFlag(FLAG_ICE_GROUND);
      MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
      FieldEvent_Wait(120);
      FieldEvent_SetExitRequest(ACTOR_SECOND_ICE_SHEET);
      Event_SetFlag(FLAG_KEEP_PARTY_POSITION);
    }
  } else if (scene == SCENE_MARS_LIGHTHOUSE_ENTRANCE_BALM) {
    FieldEffect_StartBlankOverlay(0);
    if (entrance == 10 || entrance == 1) {
      FlameThrowers_Start((s32)gMarsLighthouseEntrance_BalmBeamTargets,
                          (s32)gMarsLighthouseEntrance_BalmBeamStops,
                          (s32)MarsLighthouseEntrance_ShootBalmFireball, 88);
      Ride_SetEntryCount(0, 1);
    }
    MarsLighthouseEntrance_PlaceIceCones(
        (s32)gMarsLighthouseEntrance_BalmIceCones);
  } else if (scene == SCENE_MARS_LIGHTHOUSE_ENTRANCE_HIDDEN_CIRCLE) {
    FieldEffect_StartBlankOverlay(0);
    Actor_Get(ACTOR_HIDDEN_CIRCLE_ICE_SHEET)->priorityFlags |=
        ACTOR_PRIORITY_UNDERFOOT;
    if (Event_GetFlag(FLAG_HIDDEN_CIRCLE_SHEET_BURST)) {
      MapGrid_CopyCellsLayered(ACTOR_HIDDEN_CIRCLE_ICE_SHEET, 95, 8, 6, 12,
                               101);
      MapGrid_CopyCellAttributes(76, 40, 8, 5, 12, 40);
      Actor_SetPosition(ACTOR_HIDDEN_CIRCLE_ICE_SHEET, 0, 0);
    } else if (Event_GetFlag(FLAG_HIDDEN_CIRCLE_SHEET_CRACKED)) {
      Event_SetFlag(FLAG_HIDDEN_CIRCLE_SHEET_SHOWN);
    } else {
      Actor_SetPosition(ACTOR_HIDDEN_CIRCLE_ICE_SHEET, 0, 0);
    }
    if (Event_GetFlag(FLAG_HIDDEN_CIRCLE_ICE_MELTED))
      MarsLighthouseEntrance_MeltIceInPath();
  } else if (scene == SCENE_MARS_LIGHTHOUSE_ENTRANCE_VALKYRIE_MAIL_CHEST) {
    FieldEffect_StartBlankOverlay(0);
    if (entrance == 2 || entrance == 4 || entrance == 6 || entrance == 13) {
      FlameThrowers_Start(
          (s32)gMarsLighthouseEntrance_ValkyrieUpperBeamTargets,
          (s32)gMarsLighthouseEntrance_ValkyrieUpperBeamStops,
          (s32)MarsLighthouseEntrance_ShootValkyrieUpperFireball, 88);
      Ride_SetEntryCount(0, 1);
    }
    if ((u32)(entrance - 7) <= 1 || entrance == 11) {
      struct Actor *target;

      FlameThrowers_Start(
          (s32)gMarsLighthouseEntrance_ValkyrieLowerBeamTargets,
          (s32)gMarsLighthouseEntrance_ValkyrieLowerBeamStops,
          (s32)MarsLighthouseEntrance_ShootValkyrieLowerFireball, 88);
      Ride_SetEntryCount(0, 1);
      if (entrance == 11) {
        Actor_SetScript(ACTOR_DRAGON_FACE,
                        gMarsLighthouseEntrance_DragonFaceAtLeft);
        Ride_SetEntryState(0, FLAME_FORWARD | 3);
        Task_AddFrame(MarsLighthouseEntrance_PulseDragonFire, TASK_PASS_VBLANK);
        Event_SetFlag(FLAG_NO_PLACE_TO_SAVE);
      }
      Task_AddFrame(MarsLighthouseEntrance_StopSlideWhileRiding,
                    TASK_PASS_VBLANK);
      Actor_Get(ACTOR_DRAGON_FACE)->update =
          (void (*)(void))MarsLighthouseEntrance_FlickerDragonFace;
      Ride_SetMode(1);
      MarsLighthouseEntrance_PlaceIceCones(
          (s32)gMarsLighthouseEntrance_ValkyrieIceCones);
      target = Actor_Get(ACTOR_DRAGON_FACE_FIREBALL);
      Actor_Get(ACTOR_VALKYRIE_ICE_CONE_A)->update =
          (void (*)(void))MarsLighthouseEntrance_WaitForDragonFace;
      Actor_Get(ACTOR_VALKYRIE_ICE_CONE_B)->update =
          (void (*)(void))MarsLighthouseEntrance_WaitForDragonFace;
      Actor_Get(ACTOR_VALKYRIE_ICE_CONE_C)->update =
          (void (*)(void))MarsLighthouseEntrance_WaitForDragonFace;
      Actor_Get(ACTOR_VALKYRIE_ICE_CONE_D)->update =
          (void (*)(void))MarsLighthouseEntrance_WaitForDragonFace;
      Actor_Get(ACTOR_VALKYRIE_ICE_CONE_E)->update =
          (void (*)(void))MarsLighthouseEntrance_WaitForDragonFace;
      Actor_Get(ACTOR_VALKYRIE_ICE_CONE_F)->update =
          (void (*)(void))MarsLighthouseEntrance_WaitForDragonFace;
      Actor_Get(ACTOR_VALKYRIE_ICE_CONE_A)->watchedActor = target;
      Actor_Get(ACTOR_VALKYRIE_ICE_CONE_B)->watchedActor = target;
      Actor_Get(ACTOR_VALKYRIE_ICE_CONE_C)->watchedActor = target;
      Actor_Get(ACTOR_VALKYRIE_ICE_CONE_D)->watchedActor = target;
      Actor_Get(ACTOR_VALKYRIE_ICE_CONE_E)->watchedActor = target;
      Actor_Get(ACTOR_VALKYRIE_ICE_CONE_F)->watchedActor = target;
    }
    Actor_Get(ACTOR_UPPER_FLAME_THROWER)->priorityFlags |=
        ACTOR_PRIORITY_UNDERFOOT;
    if (Event_GetFlag(FLAG_VALKYRIE_STATUE_BLOCKS_FLAMES))
      Actor_SetPosition(ACTOR_PUSHED_STATUE, 88, 696);
  } else if (scene == SCENE_MARS_LIGHTHOUSE_ENTRANCE_SOL_BLADE_CHEST) {
    FieldEffect_StartBlankOverlay(0);
  }
  if (scene == SCENE_MARS_LIGHTHOUSE_ENTRANCE_VALKYRIE_MAIL_CHEST &&
      entrance == 13)
    Event_ClearFlag(FLAG_FIELD_HANDLING_REQUESTS);
  Palette_SetColorsFade((s32)gMarsLighthouseEntrance_FadeSlots,
                        (s32)gMarsLighthouseEntrance_FadeColorsFrom,
                        (s32)gMarsLighthouseEntrance_FadeColorsTo,
                        (s32)gMarsLighthouseEntrance_FadeOrder);
  return 0;
}

extern s32 gMarsLighthouseEntrance_ValkyrieUpperFireballPath[];
extern s32 gMarsLighthouseEntrance_ValkyrieLowerFireballPath[];
extern s32 gMarsLighthouseEntrance_BalmFireballPath[];
extern s32 gMarsLighthouseEntrance_FireballToHiddenCircleIceSheet[];
extern s32 gMarsLighthouseEntrance_FireballThroughIce[];
extern s32 gMarsLighthouseEntrance_FireballToLapisIceSheet[];
extern struct Trigger gMarsLighthouseEntrance_Triggers[];
extern struct Trigger gMarsLighthouseEntrance_OutsideTriggers[];
extern struct Trigger gMarsLighthouseEntrance_FugueTriggers[];
extern struct Trigger gMarsLighthouseEntrance_LapisTriggers[];
extern struct Trigger gMarsLighthouseEntrance_BalmTriggers[];
extern struct Trigger gMarsLighthouseEntrance_HiddenCircleTriggers[];
extern struct Trigger gMarsLighthouseEntrance_ValkyrieTriggers[];
extern struct Trigger gMarsLighthouseEntrance_SolBladeTriggers[];

s32 Scene_PrepareMap(void) {
  if (Flag_IsSet(FLAG_MARS_LIGHTHOUSE_THAWED))
    Map_CopyPalette(gMainRoutineWork.mapId +
                        (SCENE_MARS_LIGHTHOUSE_JUPITER_LEDGE - SCENE_PROX),
                    0x5000000);
  return 0;
}

void MarsLighthouseEntrance_ShootValkyrieUpperFireball(void) {
  Actor_Attach(10, 19, 20, 0,
               gMarsLighthouseEntrance_ValkyrieUpperFireballPath);
}

void MarsLighthouseEntrance_ShootValkyrieLowerFireball(void) {
  Actor_Attach(12, 19, 20, 0,
               gMarsLighthouseEntrance_ValkyrieLowerFireballPath);
}

void MarsLighthouseEntrance_ShootBalmFireball(void) {
  Actor_Attach(9, 18, 20, 0x4000, gMarsLighthouseEntrance_BalmFireballPath);
}

void MarsLighthouseEntrance_ShootFireballAtHiddenCircleIceSheet(void) {
  Actor_Attach(9, 13, 14, 0,
               gMarsLighthouseEntrance_FireballToHiddenCircleIceSheet);
}

void MarsLighthouseEntrance_ShootFireballThroughIce(void) {
  Actor_Attach(11, 13, 14, 0x4000, gMarsLighthouseEntrance_FireballThroughIce);
}

void MarsLighthouseEntrance_ShootFireballAtLapisIceSheet(void) {
  Actor_Attach(13, 21, 22, 0, gMarsLighthouseEntrance_FireballToLapisIceSheet);
}

void Ride_UpdateDrift(struct Actor *object);

#include "shared/actor_is_touching.h"

#include "shared/actor_is_beside.h"

#include "shared/ride_update_carried.h"

extern s32 gScriptedPart_Script[];

void Ride_UpdateDrift(struct Actor *object);

#include "shared/actor_reset_scripted_for.h"

/// Spawns a ride object moving forward at `speed`.
void Ride_SpawnForward(s32 *position, s32 speed, s32 mode) {
  struct Actor *object = Import_ActorCreate_(SPRITE_BLAZE_FLAME, position[0],
                                             position[1], position[2]);

  if (object != NULL) {
    object->bounce = 0;
    object->gravity = 0;
    object->riseBaseY = speed;
    Actor_ResetScriptedFor(object, 2);
    object->update = Flame_UpdateForward;
    if (mode)
      object->retainSpriteCache = 2;
    Actor_SelectAnimation(object, 2);
  }
}

#include "shared/ride_update_landing.h"

extern s32 gScriptedPart_Script[];

void Ride_UpdateDrift(struct Actor *object);

/// Stops the object and sets its sprite part, then restarts its script at
/// gScriptedPart_Script. Overlays 6ab, 6ac and 6ad have the same function.
#include "shared/actor_setup_scripted_part.h"

/// Spawns a ride object moving back at `speed`.
void Ride_SpawnBackward(s32 *position, s32 speed, s32 mode) {
  struct Actor *object = Import_ActorCreate_(SPRITE_BLAZE_FLAME, position[0],
                                             position[1], position[2]);

  if (object != NULL) {
    object->bounce = 0;
    object->gravity = 0;
    object->riseBaseY = -speed;
    Actor_SetupScriptedPart(object, 3);
    object->update = Flame_UpdateForward;
    if (mode)
      object->retainSpriteCache = 2;
    Actor_SelectAnimation(object, 2);
  }
}

void Ride_SpawnLandingForward(s32 *position, s32 speed, s32 mode) {
  struct Actor *object = Import_ActorCreate_(SPRITE_BLAZE_FLAME, position[0],
                                             position[1], position[2]);

  if (object != NULL) {
    object->bounce = 0;
    object->gravity = 0;
    object->riseBaseY = speed;
    Actor_SetupScriptedPart(object, 3);
    object->update = Flame_UpdateLanding;
    if (mode)
      object->retainSpriteCache = 2;
    Actor_SelectAnimation(object, 2);
  }
}

void Ride_SpawnLandingBackward(s32 *position, s32 speed, s32 mode) {
  struct Actor *object = Import_ActorCreate_(SPRITE_BLAZE_FLAME, position[0],
                                             position[1], position[2]);

  if (object != NULL) {
    object->bounce = 0;
    object->gravity = 0;
    object->riseBaseY = -speed;
    Actor_SetupScriptedPart(object, 3);
    object->update = Flame_UpdateLanding;
    if (mode)
      object->retainSpriteCache = 2;
    Actor_SelectAnimation(object, 2);
  }
}

#include "shared/actor_is_above.h"

#include "shared/ride_update_drift.h"

#include "shared/actor_setup_scripted_for.h"

/// Spawns a spark thrown up at `speed`.
void Ride_SpawnSpark(s32 *position, s32 speed, s32 mode) {
  struct Actor *object = Import_ActorCreate_(SPRITE_DUST_CLOUD, position[0],
                                             position[1], position[2] - px(2));
  s32 spin;

  if (object != NULL) {
    object->bounce = Trig_Cos((u16)(Random16() * 3 >> 4) + 0x3000);
    object->gravity = (u16)Random16();
    object->riseBaseY = (u16)(Random16() << 1) + speed;
    spin = (u16)Random16() + 0xffff8000;
    object->timer = spin;
    Actor_SetupScriptedFor(object, 2);
    object->update = (void (*)(void))Ride_UpdateDrift;
    Actor_SetBlendMode(object, 1);
  }
}

typedef struct Actor Actor;

/// A flame thrower of the ride work, as FlameThrowers_Puff reads it.
typedef struct Entry {
  s16 active;
  s16 timer; ///< frames to its next puff
  u16 flags; ///< enum FlameThrowerFlags: which flame, or none
  /// Puffs even off screen, and its flames keep their sprites.
  s16 offScreen;
  struct Actor *actor;
  s32 reach; ///< 16.16 pixels, from the flags' low four bits
} Entry;

/// The ride work as FlameThrowers_Puff reads it: a flame reaching its stop
/// sets stopped, which calls back once.
typedef struct State {
  u8 unusedBeforeStopped[6];
  s16 stopped;
  s16 wasStopped;
  u8 unusedAfterWasStopped[4];
  s16 runsDuringPsynergy; ///< Ride_SetMode: not paused by Psynergy
  void (*onStopped)();
  Entry entries[4];
} State;

/// The heap slots as FlameThrowers_Puff reads them: the field work through a
/// union, which keeps its read after the stores before it.
typedef struct HeapSlotsView {
  u8 unusedBeforeMapGrid[32];
  struct MapWork *mapGrid;
  u8 unusedBeforeFieldWork[108 - 36];
  union {
    s32 word;
    void *pointer;
  } fieldWork;
} HeapSlotsView;

/// Each frame, every flame thrower in view sends its flame: forward, landing
/// forward or back, back, or a spark, by its flags, every ninth frame. They
/// pause while a menu is open, or a Psynergy is used unless the ride work runs
/// them through it; reaching a stop calls the stop's callback once.
void FlameThrowers_Puff(void) {
  s32 saved[2];
  State *state;
  Entry *entry;
  struct MapWork *map;
  s32 *view;
  s32 cameraX, cameraZ;
  s32 v;
  s32 i;

  state = (State *)RIDE_WORK;
  map = ((HeapSlotsView *)Ram_HeapSlots)->mapGrid;
  entry = state->entries;
  view = &map->viewX;
  cameraX = view[0];
  cameraX &= FIELD_PIXEL_MASK;
  cameraZ = view[1] & FIELD_PIXEL_MASK;
  saved[1] = map->origin[1];
  saved[0] = ((HeapSlotsView *)Ram_HeapSlots)->fieldWork.word;
  if (state->stopped != 0 && state->wasStopped == 0 && state->onStopped != 0)
    state->onStopped();
  v = (u16)state->stopped;
  state->wasStopped = v;
  state->stopped = 0;
  for (i = 3; i >= 0; i--, entry++) {
    struct Actor *actor;
    s32 flag, x, y, offScreen, value, dx, dz;
    u8 *mark;

    if (entry->active == 0)
      continue;
    actor = entry->actor;
    if (actor == 0)
      continue;
    flag = GameFlag_Get(FLAG_MENU_OPEN);
    if (((struct FieldWork *)saved[0])->psynergy != 0)
      flag |= 1;
    mark = &actor->paused;
    *mark = flag;
    if (state->runsDuringPsynergy != 0)
      *mark = GameFlag_Get(FLAG_MENU_OPEN);
    dz = actor->z.fixed;
    dx = actor->x.fixed - cameraX;
    y = actor->y.fixed;
    dz -= cameraZ;
    y = dz - y;
    offScreen = entry->offScreen;
    x = dx >> 16;
    y = (y >> 16) - 8;
    if (offScreen == 0) {
      if ((u32)(x + 7) > 334)
        continue;
      if (y <= -48)
        continue;
      if (y > 239)
        continue;
    }
    y = (u16)entry->timer;
    if (entry->timer <= 0) {
      y = entry->flags & 0xf0;
      switch (y) {
      case FLAME_SPARK:
        Ride_SpawnSpark((void *)&entry->actor->x, entry->reach, offScreen);
        break;
      case FLAME_BACKWARD:
        Ride_SpawnBackward(&entry->actor->x.fixed, entry->reach, offScreen);
        break;
      case FLAME_LANDING_BACKWARD:
        Ride_SpawnLandingBackward(&entry->actor->x.fixed, entry->reach,
                                  offScreen);
        break;
      case FLAME_LANDING_FORWARD:
        Ride_SpawnLandingForward(&entry->actor->x.fixed, entry->reach,
                                 offScreen);
        break;
      case FLAME_FORWARD:
        Ride_SpawnForward(&entry->actor->x.fixed, entry->reach, offScreen);
        break;
      }
      value = 8;
    } else {
      value = y - 1;
    }
    entry->timer = value;
  }
}

/// The flame's sprite as Ride_Create reads it: its VRAM block, and the bit that
/// keeps the block cached.
struct RideAnimation {
  u8 beforeVramBlock[0x10];
  u8 vramBlock;
  u8 keepEntry : 1;
};

void Ride_Create(void) {
  struct MarsLighthouseEntranceRideHead *work =
      (struct MarsLighthouseEntranceRideHead *)RIDE_WORK;
  struct Actor *object;
  struct RideAnimation *animation;

  if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION))
    Iwram_ClearWordsEntry(work, 116);
  object = (struct Actor *)FieldActor_Create(SPRITE_SMALL_FLAME, 1, 0, 0);
  object->retainSpriteCache = 2;
  Actor_SelectAnimation(object, 1);
  animation = object->sprite;
  work->handle = object;
  work->value0c = animation->vramBlock;
  animation->keepEntry = 1;
}

UNUSED void Ride_Release(void) {
  struct FlameHitWork *work = RIDE_WORK;

  Actor_Release(work->handle);
  work->handle = NULL;
}

void Ride_SetMode(s16 value) { RIDE_WORK->value0e = value; }

void FlameThrowers_Puff(void);

#include "shared/beam_start.h"

UNUSED s32 Ride_GetEntryValue(s32 index) {
  return ((struct RideRow *)((u8 *)RIDE_WORK + index * 16))->entry.active;
}

UNUSED void Ride_SetEntryValue(s32 index, s16 value) {
  ((struct RideRow *)((u8 *)RIDE_WORK + index * 16))->entry.active = value;
}

UNUSED s32 Ride_GetCounter(void) { return RIDE_WORK->counter; }

UNUSED s32 Ride_StepCounter(s32 limit) {
  struct FlameHitWork *work = RIDE_WORK;

  if (!Event_GetFlag(FLAG_FIELD_HANDLING_REQUESTS)) {
    if (++work->counter >= limit)
      work->counter = 0;
  }
  return work->counter;
}

void Ride_SetEntryState(s32 index, s32 value) {
  struct FlameHitWork *work = RIDE_WORK;
  struct RideEntry *entry = &work->entries[index];

  entry->reach = (value & 15) << 16;
  entry->flags = value & 0xf0;
}

void Ride_SetEntryCount(s32 index, s16 value) {
  RIDE_WORK->entries[index].offScreen = value;
}

UNUSED s32 Ride_GetEntryFlags(s32 index) {
  return ((struct RideRow *)((u8 *)RIDE_WORK + index * 16))->entry.flags;
}

UNUSED void Ride_SetSlotActor(u32 index, s32 value) {
  struct FlameHitWork *work = RIDE_WORK;

  if (index <= 3)
    work->slots[index].id = value;
}

#include "shared/party_take_fall_damage.h"

#include "../field/field_effect_advance_spin.c"

#include "shared/actor_blink.h"

s32 MapGrid_CheckCollision(struct Actor *actor, struct Vector3 *position);
void Actor_SetSpriteColorMode(struct Actor *actor, s32 a);
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

/// Hit by the flames, the leader is knocked back.
void MarsLighthouseEntrance_KnockBackIfHit(void) {
  if (RIDE_WORK->active)
    Leader_KnockBack();
}

/// Stepping onto ice (cell behavior 90, as Imil's ice slide is in The Broken
/// Seal): the leader slides until the ice stops.
int MarsLighthouseEntrance_SlideOnIce(void) {
  struct FieldWork *event = Ram_HeapSlots->fieldWork;
  struct FlameHitWork *ride;
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
#ifdef UBFIX
  Party_TakeFallDamage();
#else
  // UB: the fall damage takes no arguments and returns nothing.
  ((s32 (*)())Party_TakeFallDamage)(-10, 0);
#endif
  FieldCamera_SetSpeed(7);
  leader = Actor_Get(gMainRoutineWork.leaderActorId);
  ride = RIDE_WORK;
  Flag_Set(FLAG_MENU_OPEN);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Flag_Clear(FLAG_MENU_OPEN);
  leader->update = (void (*)(void))Actor_Blink;
  leader->facing = FACING_SOUTH;
  Actor_SelectAnimation(leader, 49);
  while (ride->car->timer == 0) {
    if (MapGrid_GetCellAttributeLowNibbleOrBump(0, leader->x.fixed,
                                                leader->z.fixed) != 7) {
      leader->x.fixed += ride->car->stepX;
      leader->z.fixed += ride->car->stepZ;
    } else {
      if (ride->car->stepX != 0)
        leader->x.fixed = (leader->x.fixed & FIELD_CELL_MASK) + px(8);
      if (ride->car->stepZ != 0)
        leader->z.fixed = (leader->z.fixed & FIELD_CELL_MASK) + px(8);
    }
    ride->active = 0;
    System_Wait(1);
  }
  FieldEvent_Wait(30);
  leader->update = NULL;
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

/// A sprite as the crumbs copy it from the flame's: its OAM attributes, scale,
/// VRAM block, size and layer count, and its animation record.
struct SpriteView {
  u8 beforeOam[4];
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
  u8 afterOam[2];
  s32 scale; ///< 16.16, as FieldSprite (field_sprite.h) has it
  u8 vramBlock;
  u8 keepEntry : 1;
  u8 otherFlags : 7;
  u16 rotation;
  u8 dimensions[2]; ///< width and height
  u8 beforeLayerCount[5];
  u8 layerCount;
  u8 beforeRecord[12];
  void *record;
};

/// The ride work as the crumbs read it: the VRAM block of the flame Ride_Create
/// makes, whose sprite the crumbs share, and that flame.
struct Work {
  u8 beforeVramBlock[12];
  u16 vramBlock;
  u8 beforeFlame[98];
  struct Actor *flame;
};

extern u32 gMarsLighthouseEntrance_CrumbScript[];

#define WORK ((struct Work *)RIDE_WORK)

void MarsLighthouseEntrance_ScatterCrumbs(struct Actor *actor) {
  struct Work *work = WORK;
  s32 i;

  if (work->flame == NULL)
    return;
  for (i = 0; i >= 0; i--) {
    struct Actor *object;
    s32 a, b, c, d, e;
    s32 x, y, z;

    a = Random16();
    b = Random16();
    x = actor->x.fixed + a * 16 - b * 16;
    c = Random16();
    d = Random16();
    e = Random16();
    z = actor->z.fixed + d * 16 - e * 16;
    y = c * 8 + actor->y.fixed + px(4);
    object = (struct Actor *)Import_ActorCreate(0x1e9, x, y, z);
    if (object != NULL) {
      s32 angle, radius;
      u32 sum;
      s32 scale;
      struct SpriteView *sprite;
      struct SpriteView *source;

      angle = Random16();
      radius = Random16() * 8;
      sum = Random16() + Random16();
      scale = (sum >> 2) + 0x8000;
      Actor_StartScript(object, gMarsLighthouseEntrance_CrumbScript);
      Actor_SetSpriteFlags(object, 0);
      object->velocityY = angle * 4 + px(2);
      object->velocityZ = Iwram_MulQ16(radius, Trig_Cos(angle));
      object->velocityX = Iwram_MulQ16(radius, Trig_Sin(angle));
      object->gravity = px(33) / 100;
      object->scaleX = scale;
      object->scaleY = scale;
      object->acceleration = 0;
      object->bounce = Q16_ONE / 2;
      sprite = object->sprite;
      source = work->flame->sprite;
      VramCache_FreeEntry(sprite->vramBlock);
      sprite->keepEntry = 1;
      sprite->vramBlock = work->vramBlock;
      SpriteAnimation_Clear(sprite->record);
      sprite->record = NULL;
      sprite->layerCount = 0;
      sprite->dimensions[0] = source->dimensions[0];
      sprite->dimensions[1] = source->dimensions[1];
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

void Actor_HoldDuringEvent(struct Actor *object) {
  object->paused = Event_GetFlag(FLAG_FIELD_HANDLING_REQUESTS);
}

#define ABS(x) ((x) < 0 ? -(x) : (x))

/// The ride work as a rolling actor reads it when it reaches the leader:
/// whether a hit is under way, the step trigger a hit raises, and the actor
/// that carries the leader off.
struct RideHitView {
  u8 beforeActive[4];
  u16 active;
  u8 beforeHitTrigger[4];
  u16 hitTrigger;
  u8 beforeCar[0x6c - 0x0c];
  struct Actor *car;
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
    struct FieldWork *fieldWork;
    struct RideHitView *other;
    struct Actor *target;

    flag = Event_GetFlag(FLAG_MENU_OPEN);
    if (work->psynergy != 0)
      flag |= 1;
    actor->paused = flag;

    position.x = actor->x.fixed;
    position.y = actor->y.fixed;
    position.z = actor->z.fixed;
    FieldVector_AddPolarOffset(8, actor->facing, &position);
    if (MapGrid_GetCellAttributeLowNibbleOrBump(0, position.x, position.z) ==
        7) {
      actor->targetZ = TARGET_UNSET;
      actor->targetY = TARGET_UNSET;
      actor->targetX = TARGET_UNSET;
      actor->acceleration = px(1) / 4;
      actor->timer = 1;
      Event_PlaySound(SE_CRASH);
      MapGrid_SetShake(5, 5, 1);
      MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
      if (actor->watchedActor != NULL)
        ((void (*)(struct Actor *))actor->watchedActor)(actor);
    }

    map = Ram_HeapSlots->mapGrid;
    tileX = FIELD_CELL(actor->x.fixed);
    tileZ = FIELD_CELL(actor->z.fixed);
    cellsA = &map->layers[1].cells[tileX + (tileZ << 7)];
    cellsB = &map->layers[2].cells[tileX + (tileZ << 7)];
    widthA = FIELD_CELL(map->layers[1].offsetX);
    heightA = FIELD_CELL(map->layers[1].offsetY);
    widthB = FIELD_CELL(map->layers[2].offsetX);
    heightB = FIELD_CELL(map->layers[2].offsetY);

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
          Event_PlaySound(SE_ICE_CRACK);
          Embers_Spawn(FIELD_CELL_CENTER(x), actor->y.fixed,
                       FIELD_CELL_CENTER(z), actor->facing);
        }
        cell = (struct MapCell *)(offset + (s32)cellsB);
        if (cell->collisionCode == 77) {
          cell->collisionCode = 0;
          MapGrid_CopyCellsLayered(64, 64, 1, 2, xB, zB);
          MapGrid_CopyCellAttributes(64, 64, 1, 1, x, z);
          Event_PlaySound(SE_CRUMBLE);
          Embers_Spawn(FIELD_CELL_CENTER(x), actor->y.fixed,
                       FIELD_CELL_CENTER(z), actor->facing);
        }
      }
    }

    if (actor->scaleX < Q16_ONE) {
      actor->scaleX += Q16_ONE / 16;
      actor->scaleY += Q16_ONE / 16;
    }
    FieldActor_SetMoveTargetFixed(actor, position.x, position.y, position.z);

    fieldWork = Ram_HeapSlots->fieldWork;
    other = (struct RideHitView *)RIDE_WORK;
    target = Actor_Get(gMainRoutineWork.leaderActorId);
    if (ABS(actor->x.fixed - target->x.fixed) < px(20) &&
        (u32)(actor->y.fixed - target->y.fixed + 0x7ffff) <= 0x1ffffe &&
        ABS(actor->z.fixed - target->z.fixed) < px(20)) {
      other->active = 2;
      other->car = actor;
      fieldWork->stepTrigger = other->hitTrigger + 2;
    }
  } else if (state == 1) {
    actor->paused = 0;
    if (actor->scaleX > 0) {
      actor->scaleX -= Q16_ONE / 16;
      actor->scaleY -= Q16_ONE / 16;
      MarsLighthouseEntrance_ScatterCrumbs(actor);
    } else {
      actor->z.fixed = 0;
      actor->y.fixed = 0;
      actor->x.fixed = 0;
      actor->velocityZ = 0;
      actor->velocityY = 0;
      actor->velocityX = 0;
      actor->update = NULL;
    }
  }
}
#undef WORK
#undef ABS

extern struct Trigger gMarsLighthouseEntrance_OutsideTriggers[];
extern struct Trigger gMarsLighthouseEntrance_FugueTriggers[];
extern struct Trigger gMarsLighthouseEntrance_LapisTriggers[];
extern struct Trigger gMarsLighthouseEntrance_BalmTriggers[];
extern struct Trigger gMarsLighthouseEntrance_HiddenCircleTriggers[];
extern struct Trigger gMarsLighthouseEntrance_ValkyrieTriggers[];
extern struct Trigger gMarsLighthouseEntrance_SolBladeTriggers[];
extern s16 gMarsLighthouseEntrance_DragonFireTimer;
extern u16 gMarsLighthouseEntrance_HoleMaskVram;

void Map_CopyPalette(s32 scene, s32 a1);

/// Attaches an actor to another with a script and an update. Its callers take
/// it as returning a word; it returns none.
s32 Actor_Attach(s32 id, s32 target, s32 kind, s32 a3, const void *script) {
  struct Actor *source = Actor_Get(id);
  struct Actor *object = Actor_Get(target);

  FieldActor_SetPositionAndResetMotion(object, source->x.fixed, source->y.fixed,
                                       source->z.fixed);
  Actor_StartScript(object, script);
  Actor_SetSpriteFlags(object, 0);
  object->motionFlags = 0;
  Actor_SelectAnimation(object, 1);
  object->watchedActor = source;
  object->timer = 0;
  object->userValue = a3;
  BYTE_AT(object, 98) = kind;
  object->update = (void (*)(void))Actor_HoldDuringEvent;
  // UB: no value is returned, though the callers take one.
#ifdef UBFIX
  return 0;
#endif
}

#include "shared/actor_launch_ahead.h"

void Actor_SetSpritePriorityDirect(struct Actor *object, s32 mode);

#include "shared/actor_pop_up_slow.h"

#include "shared/actor_pop_up.h"

#include "shared/actor_park.h"

extern s16 gSlideOnIce_SteerAngles[];

#include "shared/actor_steer_by_pad.h"

#include "shared/leader_stop_if_pending.h"

s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
extern s16 gSlideOnIce_AnglesByKey[];

#include "shared/leader_slide_on_ice.h"

extern volatile s16 gSlideOnIce_Stopped;
void Actor_SteerByPad(struct Actor *object);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);

#include "shared/slide_on_ice_from_edge.c"

void Leader_SetSlideStopped(s32 value) { gSlideOnIce_Stopped = value; }

#include "shared/leader_slide_on_ice_ahead.h"

#include "../field/actor_set_sprite_priority_b.c"

#include "../field/field_effect_move.c"

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
#undef Position
extern s32 gFieldEffect_DurationScripts[];

void Actor_SetSpriteFlags(struct Actor *object, u32 value);

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options) {
  struct Actor *party = Actor_GetById(gMainRoutineWork.leaderActorId);
  struct FieldEffect *effect;
  struct FieldSprite *sprite;
  const s32 *script;

  if ((flags & EFFECT_USE_TYPE) != 0 && options != 0)
    effect = (struct FieldEffect *)Import_ActorCreate(options->type, x, y, z);
  else
    effect =
        (struct FieldEffect *)Import_ActorCreate(SPRITE_DUST_CLOUD, x, y, z);
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
    effect->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
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
#include "../field/map_cell_sprite.c"

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
  struct Actor *object = Actor_Get(gMainRoutineWork.leaderActorId);
  s32 x;
  s32 z;
  s32 mask;

  mask = 64;
  x = FIELD_CELL(object->x.fixed);
  x |= (s16)(event->x & mask);
  z = FIELD_CELL(object->z.fixed);
  z |= (s16)(event->z & mask);
  Leader_CenterInCell();
  Event_PlaySound(SE_SPLASH);
  MapGrid_CopyCells(event->x, event->z + 2, 1, 1, x, z);
  FieldEvent_Wait(12);
  {

    object->facing = FACING_SOUTH;
    object->motionFlags = 0;
  }
  Actor_SetSpriteFlags(Actor_Get(gMainRoutineWork.leaderActorId), 0);
  {
    u32 winout = WINOUT_WIN01_BG0 | WINOUT_WIN01_BG1 | WINOUT_WIN01_BG2 |
                 WINOUT_WIN01_BG3 | WINOUT_WIN01_OBJ | WINOUT_WINOBJ_BG0 |
                 WINOUT_WINOBJ_BG1 | WINOUT_WINOBJ_BG2 | WINOUT_WINOBJ_BG3;

    REG_WINOUT = winout;
  }
  REG_DISPCNT |= DISPCNT_OBJWIN_ON;
  event->busy = 1;
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_LEADER_ARMS_RAISED);
  FieldEvent_Wait(16);
  for (x = 0; x < 40; x++) {
    if (x == 5)
      Event_PlaySound(SE_DUST_PUFF);
    object->scaleX -= Q16_ONE / 64;
    object->scaleY -= 0x300;
    object->y.fixed -= 0x9999;
    System_Wait(1);
  }
  Actor_Get(gMainRoutineWork.leaderActorId)->spriteStorageKind = 0;
  event->busy = 0;
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
}

#include "../field/field_effect_advance_motion.c"

#undef CELL_PUSH_SLOT_ENTRY
void Teleport_PlayWhirl(void);

/// The leader lands from the floor above in a puff of dust.
void MarsLighthouseEntrance_LandFromCrack(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.leaderActorId);
  struct EffectOptions options;
  struct Vector3 velocity;
  u32 i;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Camera_SetBounds(-1, -1, -1, 0);
  MapGrid_CenterOnFollowedPoint();
  System_Wait(1);
  leader->y.fixed = px(130);
  leader->gravity = px(1) / 2;
  leader->bounce = 0;
  leader->motionFlags = 0;
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_PlaySound(SE_DUST_PUFF);
  leader->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  FieldEvent_Wait(24);
  options.palette = 7;
  options.update = FieldEffect_AdvanceMotion;
  options.startScaleX = Q16_ONE * 16 / 25;
  options.startScaleY = Q16_ONE * 16 / 25;
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
    FieldEffect_Spawn(leader->x.fixed, leader->y.fixed, leader->z.fixed,
                      velocity.x, velocity.y, velocity.z,
                      EFFECT_USE_UPDATE | EFFECT_USE_START_SCALE |
                          EFFECT_USE_PALETTE | 1,
                      &options);
  }
  if (++i <= 16)
    goto next;
  Event_PlaySound(SE_THUMP);
  Actor_SetAttachedEffect(gMainRoutineWork.leaderActorId,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, 49);
  MapGrid_SetShake(5, 5, 1);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  FieldMapGrid_WaitForShakeToSettle();
  Actor_SetAttachedEffect(gMainRoutineWork.leaderActorId,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_REMOVE);
  leader->gravity = px(1);
  leader->bounce = Q16_ONE / 4;
  FieldEvent_Wait(10);
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_STAND);
  Event_End();
}

void GameFlag_SetByte(s32 flag, s32 value);
/* VIEW(s42f8): routine MarsLighthouseEntrance_DropThroughCrack as this part
 * declared it; the joined file has another; typed as struct CellTarget (its
 * true argument), the leader's load waits on the store to its member, since
 * CellTarget holds a pointer and PushEvent only halfwords: 2 instructions move
 * in tla-en and tla-ja; tried as the definition's void
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
  struct Actor *object = Actor_Get(gMainRoutineWork.leaderActorId);
  s32 x = FIELD_CELL(object->x.fixed);
  s32 z = FIELD_CELL(object->z.fixed);
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
  struct Actor *leader = Actor_Get(gMainRoutineWork.leaderActorId);
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  s32 dx = FieldGameFlag_GetByte(gCrackedTiles.flagByte);
  s32 dz = FieldGameFlag_GetByte(gCrackedTiles.flagByte + 8);

  if (dx != 0 || dz != 0) {
    struct Actor *marker;

    GameFlag_SetByte(gCrackedTiles.flagByte, 0);
    GameFlag_SetByte(gCrackedTiles.flagByte + 8, 0);
    leader->x.fixed = FIELD_CELL_CENTER(dx + x);
    leader->z.fixed = FIELD_CELL_CENTER(dz + z);
    marker = work->camera;
    marker->x.fixed = leader->x.fixed;
    marker->z.fixed = leader->z.fixed;
    MapGrid_CenterOnFollowedPoint();
    MarsLighthouseEntrance_LandFromCrack();
    gFieldWork->transition = effect;
  }
}

extern u16 *gPaletteCycle_Slots;
extern u16 *gPaletteCycle_FlashColors;

// Records the four palette lists, writes each listed palette entry from its
// red, green and blue, then copies two palettes from block32 and starts the
// blend.
/// Palette_SetColors, then applies the palette fade.
void Palette_SetColorsFade(u16 *slots, u16 *colours, u16 *a, u16 *b) {
  u8 *saved = Ram_HeapSlots->paletteFade;

  gPaletteCycle_Slots = slots;
  gPaletteCycle_FlashColors = colours;
  gPaletteCycle_Colors = a;
  gPaletteCycle_Order = b;
  if (*slots != 0xffff && *colours != 0xffff) {
  next:
    {
      int red = *colours++;
      int index = *slots++;
      int green = *colours++;
      int blue = *colours++;
      SET_RGB(((u16 *)BG_PLTT)[index], red, green, blue);
    }
    if (*slots != 0xffff && *colours != 0xffff)
      goto next;
  }
  Dma_Set((const void *)BG_PLTT, saved, DMA_COPY_WORDS(112), REG_DMA3);
  Dma_Set((const void *)OBJ_PLTT, saved + 448, DMA_COPY_WORDS(112), REG_DMA3);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
  if (gMainRoutineWork.teleportState == 2)
    Teleport_ArriveWhirling();
}

extern u16 gPaletteCycle_Count;

/// Starts the shimmer of the circle's colours as Teleport is cast.
void Teleport_StartShimmer(void) {
  gPaletteCycle_Step = 0;
  gPaletteCycle_Count = List_CountEntries(gPaletteCycle_Slots);
  gPaletteCycle_Period = 15;
  gPaletteCycle_Countdown = 16;
  gPaletteCycle_Flash = 1;
  Task_AddFrame(PaletteCycle_Fade, TASK_PASS_VBLANK);
}

/// Stops the shimmer.
void Teleport_StopShimmer(void) {
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  FieldTask_Remove(PaletteCycle_Fade);
}

void Teleport_StartFastShimmer(void) {
  gPaletteCycle_Step = 0;
  gPaletteCycle_Count = List_CountEntries(gPaletteCycle_Slots);
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
  src = gPaletteCycle_Colors;
  for (i = 0; i < gPaletteCycle_Count; i++) {
    dst =
        &BG_PLTT[gPaletteCycle_Order[gPaletteCycle_Step * gPaletteCycle_Count +
                                     i]];
    red = *src++;
    green = *src++;
    blue = *src++;
    SET_RGB(*dst, red, green, blue);
  }
  dst = &BG_PLTT[gPaletteCycle_Slots[gPaletteCycle_Count]];
  if (gPaletteCycle_Flash != 0)
    src = gPaletteCycle_FlashColors + gPaletteCycle_Count * 3;
  else
    src = gPaletteCycle_Colors + gPaletteCycle_Count * 3;
  red = *src++;
  green = *src++;
  blue = *src++;
  SET_RGB(*dst, red, green, blue);
  gPaletteCycle_Flash ^= 1;
  if (++gPaletteCycle_Step == gPaletteCycle_Count)
    gPaletteCycle_Step = 0;
}

void Teleport_ArriveWhirling(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  s32 saved;

  saved = work->transition;
  PaletteFade_ApplyTarget(PALETTE_TINT_CUSTOM | RGB(8, 8, 8), PALETTES_BG);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_OBJECTS);
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Teleport_StartFastShimmer();
  Teleport_PlayWhirl();
  System_Wait(40);
  Teleport_StopShimmer();
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_BG);
  PaletteFade_Interpolate(16);
  System_Wait(16);
  {
    struct MainRoutineWork *party = &gMainRoutineWork;

    party->teleportState = 0;
  }
  work->transition = saved;
}

#include "shared/actors_set_cells_open.h"

/// Finds the current target's table row and shrinks it.
UNUSED void Pound_SinkTarget(u16 *list, s32 mode, s32 count) {
  struct GabombaStatueEffectWork *work = Ram_HeapSlots->fieldEffectWork;

  while (*list != 0xffff) {
    s32 entry = *list;
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
        Actor_SetCellOpen(id, a, flag, object->layer);
        object->scaleY = Q16_ONE;
      }
      break;
    }
    list += 3;
  }
}

#include "shared/actor_set_cell_open.h"

// clang-format off
s32 gMarsLighthouseEntrance_LapisWestFlameThrowerRoute[] = {                                                               
      SC_SET(SW_STEERING, 0),                                                  
      SC_SPEED(0.5),                                                
      SC_ACCELERATION(0.03125),                                                 
      SC_MOVE(200, 0, 112),                                     
      SC_SYNC,                                                                 
      SC_WAIT(20),                                                             
      SC_MOVE(104, 0, 112),                                     
      SC_SYNC,                                                                 
      SC_WAIT(20),                                                             
      SC_LOOP(SC_FOREVER, SC_TOP),                                             
  };

s32 gMarsLighthouseEntrance_LapisEastFlameThrowerRoute[] = {                                                               
      SC_SET(SW_STEERING, 0),                                                  
      SC_SPEED(0.5),                                                
      SC_ACCELERATION(0.03125),                                                 
      SC_MOVE(232, 0, 112),                                     
      SC_SYNC,                                                                 
      SC_WAIT(20),                                                             
      SC_MOVE(296, 0, 112),                                     
      SC_SYNC,                                                                 
      SC_WAIT(20),                                                             
      SC_LOOP(SC_FOREVER, SC_TOP),                                             
  };

s32 gMarsLighthouseEntrance_BalmFlameThrowerRoute[] = {                                                               
      SC_SET(SW_STEERING, 0),                                                  
      SC_SET(SW_MOTION, 2),                                                    
      SC_SET(SW_Y, px(48)),                                                
      SC_SET(SW_GROUND, px(48)),                                           
      SC_SPEED(0.5),                                                
      SC_ACCELERATION(0.03125),                                                 
      SC_MOVE(904, 0, 528),                                      
      SC_SYNC,                                                                 
      SC_WAIT(20),                                                             
      SC_MOVE(840, 0, 528),                                      
      SC_SYNC,                                                                 
      SC_WAIT(20),                                                             
      SC_LOOP(SC_FOREVER, SC_TOP),                                             
  };

s32 gMarsLighthouseEntrance_ValkyrieWestFlameThrowerRoute[] = {
    SC_SET(SW_STEERING, 0),
    SC_SET(SW_MOTION, 2),
    SC_SET(SW_Y, px(48)),
    SC_SET(SW_GROUND, px(48)),
    SC_SPEED(0.5),
    SC_ACCELERATION(0.03125),
    SC_WAIT(10),
    SC_MOVE(88, 0, 688),
    SC_SYNC,
    SC_WAIT(30),
    SC_LABEL(1),
    SC_MOVE(152, 0, 688),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(88, 0, 688),
    SC_SYNC,
    SC_LOOP(SC_FOREVER, SC_TO(1)),
};

s32 gMarsLighthouseEntrance_ValkyrieEastFlameThrowerRoute[] = {                                                               
      SC_SET(SW_STEERING, 0),                                                  
      SC_SET(SW_MOTION, 0),                                                   
      SC_SET(SW_Y, px(32)),                                                
      SC_SET(SW_GROUND, px(32)),                                           
      SC_SPEED(0.5),                                                
      SC_ACCELERATION(0.03125),                                                 
      SC_END,                                                                  
  };

s32 gMarsLighthouseEntrance_DragonFaceGoesLeft[] = {
    SC_SET(SW_STEERING, 0),
    SC_SET(SW_MOTION, 0),
    SC_SET(SW_Y, px(32)),
    SC_SET(SW_GROUND, px(32)),
    SC_SPEED(0.5),
    SC_ACCELERATION(0.03125),
    SC_MOVE(808, 32, 128),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(808, 48, 128),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(728, 48, 128),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(728, 32, 128),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(712, 32, 128),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(712, 48, 128),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(616, 48, 128),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(616, 32, 128),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(552, 32, 128),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(552, 48, 128),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(536, 48, 128),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(536, 32, 128),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(520, 32, 128),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(520, 48, 128),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(296, 48, 128),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(296, 32, 128),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(264, 32, 128),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(264, 48, 128),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(136, 48, 128),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(136, 32, 128),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(120, 32, 128),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(120, 48, 128),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(56, 48, 128),
    SC_SYNC,
    SC_WAIT(60),
    SC_CALL(MarsLighthouseEntrance_LightDragonFace),
    SC_END,
};

s32 gMarsLighthouseEntrance_DragonFaceAtLeft[] = {
    SC_SET(SW_STEERING, 0),
    SC_SET(SW_MOTION, 0),
    SC_SET(SW_X, px(56)),
    SC_SET(SW_Z, px(128)),
    SC_SET(SW_Y, px(48)),
    SC_SET(SW_GROUND, px(48)),
    SC_SPEED(0.5),
    SC_ACCELERATION(0.03125),
    SC_MOVE(56, 48, 128),
    SC_SYNC,
    SC_WAIT(20),
    SC_END,
};

u32 gMarsLighthouseEntrance_IceChipScript[] = {
    0, 40, 0x80010000, 22, 25, 0,
    0, 1, 22, 25, 1, 0,
    1, 13, 10, 0xc0010000, 38,
};

s16 gMarsLighthouseEntrance_FugueIceCones[] = {
    ACTOR_FUGUE_ICE_CONE_A, 7, FLAG_SCENE_TEMP_00,
    ACTOR_FUGUE_ICE_CONE_B, 7, FLAG_SCENE_TEMP_01,
    -1,
};

u16 gMarsLighthouseEntrance_LapisBeamTargets[] = {
    ACTOR_LAPIS_FLAME_THROWER_A, FLAME_FORWARD | 3, ACTOR_LAPIS_FLAME_THROWER_B, FLAME_FORWARD | 3,
    ACTOR_LAPIS_FLAME_THROWER_C, FLAME_FORWARD | 3, ACTOR_LAPIS_FLAME_THROWER_D, FLAME_FORWARD | 3,
    0,
};

u16 gMarsLighthouseEntrance_LapisBeamStops[] = {
    ACTOR_PUSHED_STATUE, 0, 0,
};

s16 gMarsLighthouseEntrance_LapisIceCones[] = {
    ACTOR_LAPIS_ICE_CONE_A, 7, FLAG_SCENE_TEMP_02,
    ACTOR_LAPIS_ICE_CONE_B, 7, FLAG_SCENE_TEMP_03,
    -1,
};

u16 gMarsLighthouseEntrance_BalmBeamTargets[] = {
    ACTOR_BALM_FLAME_THROWER, FLAME_FORWARD | 3, 0,
};

u16 gMarsLighthouseEntrance_BalmBeamStops[] = {
    ACTOR_BALM_TORCH, 1, 0,
};

s16 gMarsLighthouseEntrance_BalmIceCones[] = {
    ACTOR_BALM_ICE_CONE_A, 7, FLAG_SCENE_TEMP_04,
    ACTOR_BALM_ICE_CONE_B, 7, FLAG_SCENE_TEMP_05,
    ACTOR_BALM_ICE_CONE_C, 7, FLAG_SCENE_TEMP_06,
    ACTOR_BALM_ICE_CONE_D, 7, FLAG_SCENE_TEMP_07,
    ACTOR_BALM_ICE_CONE_E, 7, FLAG_SCENE_TEMP_08,
    -1,
};

u16 gMarsLighthouseEntrance_ValkyrieUpperBeamTargets[] = {
    ACTOR_UPPER_FLAME_THROWER, FLAME_FORWARD | 3, 0,
};

u16 gMarsLighthouseEntrance_ValkyrieUpperBeamStops[] = {
    ACTOR_PUSHED_STATUE, 0, ACTOR_UPPER_TORCH, 1, 0,
};

u16 gMarsLighthouseEntrance_ValkyrieLowerBeamTargets[] = {
    ACTOR_DRAGON_FACE, FLAME_THROWER_DRY | 3, 0,
};

u16 gMarsLighthouseEntrance_ValkyrieLowerBeamStops[] = {
    ACTOR_LOWER_TORCH, 1, 0,
};

s16 gMarsLighthouseEntrance_ValkyrieIceCones[] = {
    ACTOR_VALKYRIE_ICE_CONE_A, 7, FLAG_SCENE_TEMP_10,
    ACTOR_VALKYRIE_ICE_CONE_B, 7, FLAG_SCENE_TEMP_11,
    ACTOR_VALKYRIE_ICE_CONE_C, 7, FLAG_SCENE_TEMP_12,
    ACTOR_VALKYRIE_ICE_CONE_D, 7, FLAG_SCENE_TEMP_13,
    ACTOR_VALKYRIE_ICE_CONE_E, 7, FLAG_SCENE_TEMP_14,
    ACTOR_VALKYRIE_ICE_CONE_F, 7, FLAG_SCENE_TEMP_15,
    -1, 0,
};

s32 gMarsLighthouseEntrance_ValkyrieUpperFireballPath[] = {                                                               
      SC_CALL(Actor_PopUpSlow), SC_MOVE(112, 32, 728),             
      SC_SYNC,        SC_MOVE(112, 22, 728),             
      SC_SYNC,        SC_MOVE(128, 22, 728),             
      SC_SYNC,        SC_CALL(Actor_Park),                                          
      SC_END,                                                                  
  };

s32 gMarsLighthouseEntrance_ValkyrieLowerFireballPath[] = {                                                               
      46, (s32)Actor_PopUpSlow, 4,  0x500000, 0x200000, 0xa80000, 1,  4,          0x500000, 0x160000,         0xa80000, 1,   
      4,  0x600000,         0x160000, 0xa80000, 1,  22, 37, (s32)MarsLighthouseEntrance_ClearFireballRow, 46, (s32)Actor_Park, 17,      
  };

s32 gMarsLighthouseEntrance_BalmFireballPath[] = {                                                               
      SC_CALL(Actor_PopUpSlow), SC_MOVE(872, 32, 576),             
      SC_SYNC,        SC_MOVE(872, 30, 576),             
      SC_SYNC,        SC_MOVE(872, 30, 584),             
      SC_SYNC,        SC_CALL(Actor_Park),                                          
      SC_END,                                                                  
  };

s32 gMarsLighthouseEntrance_FireballToHiddenCircleIceSheet[] = {                                                               
      46, (s32)Actor_PopUp, 4,  0x600000, 0x200000, 0x2a80000, 1,  4,          0x600000, 0x160000,         0x2a80000, 1,   
      4,  0x700000,         0x160000, 0x2a80000, 1,  22, 37, (s32)MarsLighthouseEntrance_CrackHiddenCircleIceSheet, 46, (s32)Actor_Park, 17,      
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

s32 gMarsLighthouseEntrance_FireballToLapisIceSheet[] = {                                                               
      46, (s32)Actor_PopUp, 4,  0x700000, 0x200000, 0x2a80000, 1,  4,          0x700000, 0x160000,         0x2a80000, 1,   
      4,  0x800000,         0x160000, 0x2a80000, 1,  22, 37, (s32)MarsLighthouseEntrance_CrackLapisIceSheet, 46, (s32)Actor_Park, 17,      
  };

s32 gScriptedPart_Script[] = {                                                               
      SC_WAIT(30), SC_SET(SW_HOOK, 0), SC_POSITION(0, 0, 0), SC_WAIT(1),       
      SC_RELEASE,                                                                 
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
      SC_WAIT(1), SC_RELEASE,                                                     
      SC_END,                                                                  
  };

u32 gFieldEffect_MediumScript[] = {                                                               
      SC_WAIT(1), SC_LOOP(44, SC_TOP),                                     
      SC_WAIT(1), SC_POSITION(0, 0, 0),                                        
      SC_WAIT(1), SC_RELEASE,                                                     
      SC_END,                                                                  
  };

u32 gFieldEffect_LongScript[] = {                                                               
      SC_WAIT(1), SC_LOOP(126, SC_TOP),                                     
      SC_WAIT(1), SC_POSITION(0, 0, 0),                                        
      SC_WAIT(1), SC_RELEASE,                                                     
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
    SCENE_EXITS(MarsLighthouseOutside),
    SCENE_EXIT(MARS_LIGHTHOUSE_OUTSIDE_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_MARS_LIGHTHOUSE_OUTSIDE),
    SCENE_EXIT(MARS_LIGHTHOUSE_OUTSIDE_TO_MARS_LIGHTHOUSE_ENTRANCE_TELEPORT_LAPIS, MarsLighthouseEntranceTeleportLapis, MARS_LIGHTHOUSE_ENTRANCE_TELEPORT_LAPIS_FROM_MARS_LIGHTHOUSE_OUTSIDE),
    SCENE_EXITS(MarsLighthouseEntranceFugue),
    SCENE_EXIT(MARS_LIGHTHOUSE_ENTRANCE_FUGUE_TO_MARS_LIGHTHOUSE_ENTRANCE_TELEPORT_LAPIS_WEST, MarsLighthouseEntranceTeleportLapis, MARS_LIGHTHOUSE_ENTRANCE_TELEPORT_LAPIS_FROM_MARS_LIGHTHOUSE_ENTRANCE_FUGUE_WEST),
    SCENE_EXIT(MARS_LIGHTHOUSE_ENTRANCE_FUGUE_TO_MARS_LIGHTHOUSE_ROOM_M_INSCRIPTIONS, MarsLighthouseRoomMInscriptions, MARS_LIGHTHOUSE_ROOM_M_INSCRIPTIONS_FROM_MARS_LIGHTHOUSE_ENTRANCE_FUGUE),
    SCENE_EXIT(3, MarsLighthouseEntranceFugue, 4),
    SCENE_EXIT(4, MarsLighthouseEntranceFugue, MARS_LIGHTHOUSE_ENTRANCE_FUGUE_FROM_MARS_LIGHTHOUSE_ENTRANCE_TELEPORT_LAPIS_EAST_A),
    SCENE_EXIT(MARS_LIGHTHOUSE_ENTRANCE_FUGUE_TO_MARS_LIGHTHOUSE_ENTRANCE_BALM_WEST, MarsLighthouseEntranceBalm, MARS_LIGHTHOUSE_ENTRANCE_BALM_FROM_MARS_LIGHTHOUSE_ENTRANCE_FUGUE_WEST),
    SCENE_EXIT(MARS_LIGHTHOUSE_ENTRANCE_FUGUE_TO_MARS_LIGHTHOUSE_ENTRANCE_BALM_EAST, MarsLighthouseEntranceBalm, MARS_LIGHTHOUSE_ENTRANCE_BALM_FROM_MARS_LIGHTHOUSE_ENTRANCE_FUGUE_EAST),
    SCENE_EXIT(7, MarsLighthouseEntranceFugue, 8),
    SCENE_EXIT(8, MarsLighthouseEntranceFugue, 7),
    SCENE_EXIT(9, MarsLighthouseEntranceFugue, 10),
    SCENE_EXIT(10, MarsLighthouseEntranceFugue, 9),
    SCENE_EXIT(MARS_LIGHTHOUSE_ENTRANCE_FUGUE_TO_MARS_LIGHTHOUSE_ENTRANCE_TELEPORT_LAPIS_EAST_B, MarsLighthouseEntranceTeleportLapis, MARS_LIGHTHOUSE_ENTRANCE_TELEPORT_LAPIS_FROM_MARS_LIGHTHOUSE_ENTRANCE_FUGUE_EAST),
    SCENE_EXITS(MarsLighthouseEntranceTeleportLapis),
    SCENE_EXIT(MARS_LIGHTHOUSE_ENTRANCE_TELEPORT_LAPIS_TO_MARS_LIGHTHOUSE_ENTRANCE_FUGUE_WEST, MarsLighthouseEntranceFugue, MARS_LIGHTHOUSE_ENTRANCE_FUGUE_FROM_MARS_LIGHTHOUSE_ENTRANCE_TELEPORT_LAPIS_WEST),
    SCENE_EXIT(MARS_LIGHTHOUSE_ENTRANCE_TELEPORT_LAPIS_TO_MARS_LIGHTHOUSE_OUTSIDE, MarsLighthouseOutside, MARS_LIGHTHOUSE_OUTSIDE_FROM_MARS_LIGHTHOUSE_ENTRANCE_TELEPORT_LAPIS),
    SCENE_EXIT(MARS_LIGHTHOUSE_ENTRANCE_TELEPORT_LAPIS_TO_MARS_LIGHTHOUSE_ENTRANCE_BALM_NORTH, MarsLighthouseEntranceBalm, MARS_LIGHTHOUSE_ENTRANCE_BALM_FROM_MARS_LIGHTHOUSE_ENTRANCE_TELEPORT_LAPIS_NORTH),
    SCENE_EXIT(MARS_LIGHTHOUSE_ENTRANCE_TELEPORT_LAPIS_TO_MARS_LIGHTHOUSE_ENTRANCE_BALM_NORTHEAST, MarsLighthouseEntranceBalm, MARS_LIGHTHOUSE_ENTRANCE_BALM_FROM_MARS_LIGHTHOUSE_ENTRANCE_TELEPORT_LAPIS_NORTHEAST),
    SCENE_EXIT(MARS_LIGHTHOUSE_ENTRANCE_TELEPORT_LAPIS_TO_MARS_LIGHTHOUSE_ENTRANCE_HIDDEN_CIRCLE, MarsLighthouseEntranceHiddenCircle, MARS_LIGHTHOUSE_ENTRANCE_HIDDEN_CIRCLE_FROM_MARS_LIGHTHOUSE_ENTRANCE_TELEPORT_LAPIS),
    SCENE_EXIT(6, MarsLighthouseEntranceTeleportLapis, 7),
    SCENE_EXIT(7, MarsLighthouseEntranceTeleportLapis, 6),
    SCENE_EXIT(8, MarsLighthouseEntranceTeleportLapis, 9),
    SCENE_EXIT(9, MarsLighthouseEntranceTeleportLapis, 8),
    SCENE_EXIT(10, MarsLighthouseEntranceTeleportLapis, 11),
    SCENE_EXIT(11, MarsLighthouseEntranceTeleportLapis, 10),
    SCENE_EXIT(12, MarsLighthouseEntranceTeleportLapis, 13),
    SCENE_EXIT(13, MarsLighthouseEntranceTeleportLapis, 12),
    SCENE_EXIT(14, MarsLighthouseEntranceTeleportLapis, 15),
    SCENE_EXIT(15, MarsLighthouseEntranceTeleportLapis, 14),
    SCENE_EXIT(MARS_LIGHTHOUSE_ENTRANCE_TELEPORT_LAPIS_TO_MARS_LIGHTHOUSE_ENTRANCE_BALM_SOUTH, MarsLighthouseEntranceBalm, MARS_LIGHTHOUSE_ENTRANCE_BALM_FROM_MARS_LIGHTHOUSE_ENTRANCE_TELEPORT_LAPIS_SOUTH),
    SCENE_EXIT(17, MarsLighthouseEntranceTeleportLapis, 18),
    SCENE_EXIT(18, MarsLighthouseEntranceTeleportLapis, 17),
    SCENE_EXIT(MARS_LIGHTHOUSE_ENTRANCE_TELEPORT_LAPIS_TO_MARS_LIGHTHOUSE_ENTRANCE_FUGUE_EAST, MarsLighthouseEntranceFugue, MARS_LIGHTHOUSE_ENTRANCE_FUGUE_FROM_MARS_LIGHTHOUSE_ENTRANCE_TELEPORT_LAPIS_EAST_A),
    SCENE_EXITS(MarsLighthouseEntranceBalm),
    SCENE_EXIT(MARS_LIGHTHOUSE_ENTRANCE_BALM_TO_MARS_LIGHTHOUSE_ENTRANCE_HIDDEN_CIRCLE_SOUTH, MarsLighthouseEntranceHiddenCircle, MARS_LIGHTHOUSE_ENTRANCE_HIDDEN_CIRCLE_FROM_MARS_LIGHTHOUSE_ENTRANCE_BALM_SOUTH),
    SCENE_EXIT(MARS_LIGHTHOUSE_ENTRANCE_BALM_TO_MARS_LIGHTHOUSE_ENTRANCE_HIDDEN_CIRCLE_NORTH_A, MarsLighthouseEntranceHiddenCircle, MARS_LIGHTHOUSE_ENTRANCE_HIDDEN_CIRCLE_FROM_MARS_LIGHTHOUSE_ENTRANCE_BALM_NORTH_A),
    SCENE_EXIT(MARS_LIGHTHOUSE_ENTRANCE_BALM_TO_MARS_LIGHTHOUSE_ENTRANCE_TELEPORT_LAPIS_NORTH, MarsLighthouseEntranceTeleportLapis, MARS_LIGHTHOUSE_ENTRANCE_TELEPORT_LAPIS_FROM_MARS_LIGHTHOUSE_ENTRANCE_BALM_NORTH),
    SCENE_EXIT(MARS_LIGHTHOUSE_ENTRANCE_BALM_TO_MARS_LIGHTHOUSE_ENTRANCE_TELEPORT_LAPIS_NORTHEAST, MarsLighthouseEntranceTeleportLapis, MARS_LIGHTHOUSE_ENTRANCE_TELEPORT_LAPIS_FROM_MARS_LIGHTHOUSE_ENTRANCE_BALM_NORTHEAST),
    SCENE_EXIT(MARS_LIGHTHOUSE_ENTRANCE_BALM_TO_MARS_LIGHTHOUSE_ENTRANCE_FUGUE_WEST, MarsLighthouseEntranceFugue, MARS_LIGHTHOUSE_ENTRANCE_FUGUE_FROM_MARS_LIGHTHOUSE_ENTRANCE_BALM_WEST),
    SCENE_EXIT(MARS_LIGHTHOUSE_ENTRANCE_BALM_TO_MARS_LIGHTHOUSE_ENTRANCE_FUGUE_EAST, MarsLighthouseEntranceFugue, MARS_LIGHTHOUSE_ENTRANCE_FUGUE_FROM_MARS_LIGHTHOUSE_ENTRANCE_BALM_EAST),
    SCENE_EXIT(MARS_LIGHTHOUSE_ENTRANCE_BALM_TO_MARS_LIGHTHOUSE_ENTRANCE_VALKYRIE_MAIL_CHEST, MarsLighthouseEntranceValkyrieMailChest, MARS_LIGHTHOUSE_ENTRANCE_VALKYRIE_MAIL_CHEST_FROM_MARS_LIGHTHOUSE_ENTRANCE_BALM),
    SCENE_EXIT(8, MarsLighthouseEntranceBalm, 9),
    SCENE_EXIT(9, MarsLighthouseEntranceBalm, 8),
    SCENE_EXIT(10, MarsLighthouseEntranceBalm, 11),
    SCENE_EXIT(11, MarsLighthouseEntranceBalm, 10),
    SCENE_EXIT(MARS_LIGHTHOUSE_ENTRANCE_BALM_TO_MARS_LIGHTHOUSE_ENTRANCE_HIDDEN_CIRCLE_NORTH_B, MarsLighthouseEntranceHiddenCircle, MARS_LIGHTHOUSE_ENTRANCE_HIDDEN_CIRCLE_FROM_MARS_LIGHTHOUSE_ENTRANCE_BALM_NORTH_B),
    SCENE_EXIT(MARS_LIGHTHOUSE_ENTRANCE_BALM_TO_MARS_LIGHTHOUSE_ENTRANCE_TELEPORT_LAPIS_SOUTH, MarsLighthouseEntranceTeleportLapis, MARS_LIGHTHOUSE_ENTRANCE_TELEPORT_LAPIS_FROM_MARS_LIGHTHOUSE_ENTRANCE_BALM_SOUTH),
    SCENE_EXITS(MarsLighthouseEntranceHiddenCircle),
    SCENE_EXIT(MARS_LIGHTHOUSE_ENTRANCE_HIDDEN_CIRCLE_TO_MARS_LIGHTHOUSE_ENTRANCE_BALM_SOUTH, MarsLighthouseEntranceBalm, MARS_LIGHTHOUSE_ENTRANCE_BALM_FROM_MARS_LIGHTHOUSE_ENTRANCE_HIDDEN_CIRCLE_SOUTH),
    SCENE_EXIT(MARS_LIGHTHOUSE_ENTRANCE_HIDDEN_CIRCLE_TO_MARS_LIGHTHOUSE_ENTRANCE_BALM_NORTH_A, MarsLighthouseEntranceBalm, MARS_LIGHTHOUSE_ENTRANCE_BALM_FROM_MARS_LIGHTHOUSE_ENTRANCE_HIDDEN_CIRCLE_NORTH_A),
    SCENE_EXIT(3, MarsLighthouseEntranceHiddenCircle, 4),
    SCENE_EXIT(4, MarsLighthouseEntranceHiddenCircle, 3),
    SCENE_EXIT(MARS_LIGHTHOUSE_ENTRANCE_HIDDEN_CIRCLE_TO_MARS_LIGHTHOUSE_ENTRANCE_TELEPORT_LAPIS, MarsLighthouseEntranceTeleportLapis, MARS_LIGHTHOUSE_ENTRANCE_TELEPORT_LAPIS_FROM_MARS_LIGHTHOUSE_ENTRANCE_HIDDEN_CIRCLE),
    SCENE_EXIT(6, MarsLighthouseEntranceHiddenCircle, 7),
    SCENE_EXIT(7, MarsLighthouseEntranceHiddenCircle, 6),
    SCENE_EXIT(8, MarsLighthouseEntranceHiddenCircle, 9),
    SCENE_EXIT(9, MarsLighthouseEntranceHiddenCircle, 8),
    SCENE_EXIT(10, MarsLighthouseEntranceHiddenCircle, 11),
    SCENE_EXIT(11, MarsLighthouseEntranceHiddenCircle, 10),
    SCENE_EXIT(12, MarsLighthouseEntranceHiddenCircle, 13),
    SCENE_EXIT(13, MarsLighthouseEntranceHiddenCircle, 12),
    SCENE_EXIT(MARS_LIGHTHOUSE_ENTRANCE_HIDDEN_CIRCLE_TO_MARS_LIGHTHOUSE_ENTRANCE_BALM_NORTH_B, MarsLighthouseEntranceBalm, MARS_LIGHTHOUSE_ENTRANCE_BALM_FROM_MARS_LIGHTHOUSE_ENTRANCE_HIDDEN_CIRCLE_NORTH_B),
    SCENE_EXITS(MarsLighthouseEntranceValkyrieMailChest),
    SCENE_EXIT(MARS_LIGHTHOUSE_ENTRANCE_VALKYRIE_MAIL_CHEST_TO_MARS_LIGHTHOUSE_ENTRANCE_SOL_BLADE_CHEST, MarsLighthouseEntranceSolBladeChest, MARS_LIGHTHOUSE_ENTRANCE_SOL_BLADE_CHEST_FROM_MARS_LIGHTHOUSE_ENTRANCE_VALKYRIE_MAIL_CHEST),
    SCENE_EXIT(2, MarsLighthouseEntranceValkyrieMailChest, 3),
    SCENE_EXIT(3, MarsLighthouseEntranceValkyrieMailChest, 2),
    SCENE_EXIT(4, MarsLighthouseEntranceValkyrieMailChest, 5),
    SCENE_EXIT(5, MarsLighthouseEntranceValkyrieMailChest, 4),
    SCENE_EXIT(MARS_LIGHTHOUSE_ENTRANCE_VALKYRIE_MAIL_CHEST_TO_MARS_LIGHTHOUSE_STAR_ROOM_SOUTHEAST, MarsLighthouseStarRoom, MARS_LIGHTHOUSE_STAR_ROOM_FROM_MARS_LIGHTHOUSE_ENTRANCE_VALKYRIE_MAIL_CHEST_SOUTHEAST),
    SCENE_EXIT(MARS_LIGHTHOUSE_ENTRANCE_VALKYRIE_MAIL_CHEST_TO_MARS_LIGHTHOUSE_ENTRANCE_BALM, MarsLighthouseEntranceBalm, MARS_LIGHTHOUSE_ENTRANCE_BALM_FROM_MARS_LIGHTHOUSE_ENTRANCE_VALKYRIE_MAIL_CHEST),
    SCENE_EXIT(MARS_LIGHTHOUSE_ENTRANCE_VALKYRIE_MAIL_CHEST_TO_MARS_LIGHTHOUSE_STAR_ROOM_NORTHWEST, MarsLighthouseStarRoom, MARS_LIGHTHOUSE_STAR_ROOM_FROM_MARS_LIGHTHOUSE_ENTRANCE_VALKYRIE_MAIL_CHEST_NORTHWEST),
    SCENE_EXIT(MARS_LIGHTHOUSE_ENTRANCE_VALKYRIE_MAIL_CHEST_TO_MARS_LIGHTHOUSE_STAR_ROOM_EAST, MarsLighthouseStarRoom, MARS_LIGHTHOUSE_STAR_ROOM_FROM_MARS_LIGHTHOUSE_ENTRANCE_VALKYRIE_MAIL_CHEST_EAST),
    SCENE_EXIT(10, MarsLighthouseEntranceValkyrieMailChest, 11),
    SCENE_EXIT(11, MarsLighthouseEntranceValkyrieMailChest, 10),
    SCENE_EXIT(12, MarsLighthouseEntranceValkyrieMailChest, 13),
    SCENE_EXIT(13, MarsLighthouseEntranceValkyrieMailChest, 12),
    SCENE_EXIT(14, MarsLighthouseEntranceValkyrieMailChest, 15),
    SCENE_EXIT(15, MarsLighthouseEntranceValkyrieMailChest, 14),
    SCENE_EXIT(MARS_LIGHTHOUSE_ENTRANCE_VALKYRIE_MAIL_CHEST_TO_MARS_LIGHTHOUSE_STAR_ROOM_WEST, MarsLighthouseStarRoom, MARS_LIGHTHOUSE_STAR_ROOM_FROM_MARS_LIGHTHOUSE_ENTRANCE_VALKYRIE_MAIL_CHEST_WEST),
    SCENE_EXITS(MarsLighthouseEntranceSolBladeChest),
    SCENE_EXIT(MARS_LIGHTHOUSE_ENTRANCE_SOL_BLADE_CHEST_TO_MARS_LIGHTHOUSE_ENTRANCE_VALKYRIE_MAIL_CHEST, MarsLighthouseEntranceValkyrieMailChest, MARS_LIGHTHOUSE_ENTRANCE_VALKYRIE_MAIL_CHEST_FROM_MARS_LIGHTHOUSE_ENTRANCE_SOL_BLADE_CHEST),
    SCENE_EXIT(2, MarsLighthouseEntranceSolBladeChest, 3),
    SCENE_EXIT(3, MarsLighthouseEntranceSolBladeChest, 2),
    SCENE_EXIT(4, MarsLighthouseEntranceSolBladeChest, 6),
    SCENE_EXIT(MARS_LIGHTHOUSE_ENTRANCE_SOL_BLADE_CHEST_TO_MARS_LIGHTHOUSE_STAR_ROOM_SOUTHEAST, MarsLighthouseStarRoom, MARS_LIGHTHOUSE_STAR_ROOM_FROM_MARS_LIGHTHOUSE_ENTRANCE_SOL_BLADE_CHEST_SOUTHEAST),
    SCENE_EXIT(6, MarsLighthouseEntranceSolBladeChest, 4),
    SCENE_EXIT(MARS_LIGHTHOUSE_ENTRANCE_SOL_BLADE_CHEST_TO_MARS_LIGHTHOUSE_STAR_ROOM_NORTHWEST_A, MarsLighthouseStarRoom, MARS_LIGHTHOUSE_STAR_ROOM_FROM_MARS_LIGHTHOUSE_ENTRANCE_SOL_BLADE_CHEST_NORTHWEST_A),
    SCENE_EXIT(MARS_LIGHTHOUSE_ENTRANCE_SOL_BLADE_CHEST_TO_MARS_LIGHTHOUSE_STAR_ROOM_NORTHWEST_B, MarsLighthouseStarRoom, MARS_LIGHTHOUSE_STAR_ROOM_FROM_MARS_LIGHTHOUSE_ENTRANCE_SOL_BLADE_CHEST_NORTHWEST_B),
    SCENE_EXITS(MarsLighthouseStarRoom),
    SCENE_EXIT(1, MarsLighthouseStarRoom, 2),
    SCENE_EXIT(2, MarsLighthouseStarRoom, 1),
    SCENE_EXIT(3, MarsLighthouseStarRoom, 4),
    SCENE_EXIT(4, MarsLighthouseStarRoom, 3),
    SCENE_EXIT(MARS_LIGHTHOUSE_STAR_ROOM_TO_MARS_LIGHTHOUSE_ENTRANCE_SOL_BLADE_CHEST_SOUTHEAST, MarsLighthouseEntranceSolBladeChest, MARS_LIGHTHOUSE_ENTRANCE_SOL_BLADE_CHEST_FROM_MARS_LIGHTHOUSE_STAR_ROOM_SOUTHEAST),
    SCENE_EXIT(MARS_LIGHTHOUSE_STAR_ROOM_TO_MARS_LIGHTHOUSE_ENTRANCE_VALKYRIE_MAIL_CHEST_SOUTHEAST, MarsLighthouseEntranceValkyrieMailChest, MARS_LIGHTHOUSE_ENTRANCE_VALKYRIE_MAIL_CHEST_FROM_MARS_LIGHTHOUSE_STAR_ROOM_SOUTHEAST),
    SCENE_EXIT(MARS_LIGHTHOUSE_STAR_ROOM_TO_MARS_LIGHTHOUSE_ENTRANCE_SOL_BLADE_CHEST_NORTHWEST_A, MarsLighthouseEntranceSolBladeChest, MARS_LIGHTHOUSE_ENTRANCE_SOL_BLADE_CHEST_FROM_MARS_LIGHTHOUSE_STAR_ROOM_NORTHWEST_A),
    SCENE_EXIT(MARS_LIGHTHOUSE_STAR_ROOM_TO_MARS_LIGHTHOUSE_ENTRANCE_VALKYRIE_MAIL_CHEST_NORTHWEST, MarsLighthouseEntranceValkyrieMailChest, MARS_LIGHTHOUSE_ENTRANCE_VALKYRIE_MAIL_CHEST_FROM_MARS_LIGHTHOUSE_STAR_ROOM_NORTHWEST),
    SCENE_EXITS_END,
};

s32 gMarsLighthouseEntrance_IceConeScript[] = {
    SC_ANIMATION(7),
    SC_END,
};

struct ActorPlacement gMarsLighthouseEntrance_Placements[] = {
    {SCENE_TABLE_END},
};

struct ActorPlacement gMarsLighthouseEntrance_OutsidePlacements[] = {
    ACTOR_PLACEMENT(SPRITE_DRAGON_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 280, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_DRAGON_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 280, FACING_WEST, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};
// clang-format on

struct ActorPlacement gMarsLighthouseEntrance_FuguePlacements[] = {
    ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS,
                    gMarsLighthouseEntrance_IceConeScript, 0, 0, 0,
                    FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS,
                    gMarsLighthouseEntrance_IceConeScript, 136, 0, 424,
                    FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(489, 800, MOVEMENT_TYPE_STAND, 648, 0, 376, FACING_SOUTH,
                    TALK_FACING_KEEP, 0),
#if !TLA_PATCH_1
    ACTOR_PLACEMENT(SPRITE_MARS_DJINNI, FLAG_DJINNI(DJINNI_FUGUE),
                    MOVEMENT_TYPE_STAND, 664, 0, 424, FACING_SOUTH,
                    TALK_FACING_PARTY, 0),
#else
    ACTOR_PLACEMENT(SPRITE_MARS_DJINNI, FLAG_DJINNI(DJINNI_FUGUE),
                    MOVEMENT_TYPE_STAND, 728, 0, 408, FACING_NORTHWEST,
                    TALK_FACING_PARTY, 0),
#endif
    {SCENE_TABLE_END},
};

void MarsLighthouseEntrance_FightFugue(void);
void MarsLighthouseEntrance_FightBalm(void);
void MarsLighthouseEntrance_GrindIce(void);
void MarsLighthouseEntrance_TeleportFromLapis(void);
void MarsLighthouseEntrance_WatchFireballCrackIce(void);
void MarsLighthouseEntrance_JumpOrSlide(void);
void MarsLighthouseEntrance_CheckCrackedFloor(void);

// clang-format off
struct ActorPlacement gMarsLighthouseEntrance_LapisPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_DRAGON_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 56, 0, 120, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_DRAGON_HEAD, CONDITION_ALWAYS, gMarsLighthouseEntrance_LapisWestFlameThrowerRoute, 104, 0, 112, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_DRAGON_HEAD, CONDITION_ALWAYS, gMarsLighthouseEntrance_LapisEastFlameThrowerRoute, 280, 0, 112, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_DRAGON_HEAD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 112, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_DRAGON_HEAD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 112, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(419, CONDITION_ALWAYS, (s32)gMarsLighthouseEntrance_StatueScript, 88, 0, 680, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(419, CONDITION_ALWAYS, (s32)gMarsLighthouseEntrance_StatueScript, 552, 0, 680, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(418, CONDITION_ALWAYS, (s32)gMarsLighthouseEntrance_StatueScript, 88, 0, 728, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, gMarsLighthouseEntrance_IceConeScript, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, gMarsLighthouseEntrance_IceConeScript, 552, 0, 776, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ICE_CRACK, CONDITION_ALWAYS, 7, 840, 0, 672, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(419, CONDITION_ALWAYS, (s32)gMarsLighthouseEntrance_StatueScript, 88, 0, 728, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ICE_CRACK, CONDITION_ALWAYS, 7, 480, 0, 704, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPARKLE, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIREBALL, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};
// clang-format on

// clang-format off
struct ActorPlacement gMarsLighthouseEntrance_BalmPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_DRAGON_HEAD, CONDITION_ALWAYS, gMarsLighthouseEntrance_BalmFlameThrowerRoute, 840, 0, 528, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(419, CONDITION_ALWAYS, (s32)gMarsLighthouseEntrance_StatueScript, 872, 0, 568, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_DRAGON_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 792, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_DRAGON_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 792, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, gMarsLighthouseEntrance_IceConeScript, 840, 0, 680, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, gMarsLighthouseEntrance_IceConeScript, 904, 0, 744, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, gMarsLighthouseEntrance_IceConeScript, 840, 0, 808, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, gMarsLighthouseEntrance_IceConeScript, 936, 0, 824, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, gMarsLighthouseEntrance_IceConeScript, 904, 0, 872, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_MERCURY_DJINNI, FLAG_DJINNI(DJINNI_BALM), MOVEMENT_TYPE_STAND, 88, 0, 904, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SPARKLE, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPARKLE, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIREBALL, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIREBALL, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};
// clang-format on

// clang-format off
struct ActorPlacement gMarsLighthouseEntrance_HiddenCirclePlacements[] = {
    ACTOR_PLACEMENT(418, CONDITION_ALWAYS, (s32)gMarsLighthouseEntrance_StatueScript, 72, 0, 632, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(419, CONDITION_ALWAYS, (s32)gMarsLighthouseEntrance_StatueScript, 72, 0, 680, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(418, CONDITION_ALWAYS, (s32)gMarsLighthouseEntrance_StatueScript, 904, 0, 120, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(419, CONDITION_ALWAYS, (s32)gMarsLighthouseEntrance_StatueScript, 856, 0, 120, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ICE_CRACK, CONDITION_ALWAYS, 7, 240, 0, 688, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPARKLE, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIREBALL, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};
// clang-format on

// clang-format off
struct ActorPlacement gMarsLighthouseEntrance_ValkyriePlacements[] = {
    ACTOR_PLACEMENT(SPRITE_DRAGON_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 696, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_DRAGON_HEAD, CONDITION_ALWAYS, gMarsLighthouseEntrance_ValkyrieWestFlameThrowerRoute, 120, 0, 688, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(419, CONDITION_ALWAYS, (s32)gMarsLighthouseEntrance_StatueScript, 88, 0, 728, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_DRAGON_HEAD, CONDITION_ALWAYS, gMarsLighthouseEntrance_ValkyrieEastFlameThrowerRoute, 792, 0, 128, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(419, CONDITION_ALWAYS, (s32)gMarsLighthouseEntrance_StatueScript, 56, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, gMarsLighthouseEntrance_IceConeScript, 200, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, gMarsLighthouseEntrance_IceConeScript, 328, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, gMarsLighthouseEntrance_IceConeScript, 360, 0, 152, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, gMarsLighthouseEntrance_IceConeScript, 424, 0, 152, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, gMarsLighthouseEntrance_IceConeScript, 664, 0, 152, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, gMarsLighthouseEntrance_IceConeScript, 760, 0, 152, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPARKLE, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIREBALL, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};
// clang-format on

// clang-format off
struct ActorPlacement gMarsLighthouseEntrance_SolBladePlacements[] = {
    ACTOR_PLACEMENT(SPRITE_DRAGON_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 88, 0, 152, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_DRAGON_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 152, FACING_WEST, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct Trigger gMarsLighthouseEntrance_Triggers[] = {
    {TRIGGER_TABLE_END},
};

struct Trigger gMarsLighthouseEntrance_OutsideTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_TABLE_END},
};
// clang-format on

struct Trigger gMarsLighthouseEntrance_FugueTriggers[] = {
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
    {TALK_TRIGGER_FACING(FACING_WEST), ACTOR_FUGUE, CONDITION_ALWAYS,
     EVENT(MarsLighthouseEntrance_FightFugue)},
    {TALK_TRIGGER_FACING(FACING_NORTH), ACTOR_FUGUE, CONDITION_ALWAYS,
     EVENT(MarsLighthouseEntrance_FightFugue)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS),
     ACTOR_FUGUE_ICE_CONE_A, FLAG_SCENE_TEMP_00,
     EVENT(MarsLighthouseEntrance_StartPoundIceCone)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS),
     ACTOR_FUGUE_ICE_CONE_B, FLAG_SCENE_TEMP_01,
     EVENT(MarsLighthouseEntrance_StartPoundIceCone)},
    {PSYNERGY_ON_ACTOR(ABILITY_GRIND), ACTOR_DEAD_END, FLAG_ICE_GROUND,
     EVENT(MarsLighthouseEntrance_GrindIce)},
    {TRIGGER_TALK, ACTOR_DEAD_END, FLAG_ICE_GROUND,
     EVENT(MarsLighthouseEntrance_CheckIce)},
    {TRIGGER_TABLE_END},
};

struct Trigger gMarsLighthouseEntrance_LapisTriggers[] = {
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
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS),
     ACTOR_LAPIS_ICE_CONE_A, FLAG_SCENE_TEMP_02,
     EVENT(MarsLighthouseEntrance_StartPoundIceCone)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS),
     ACTOR_LAPIS_ICE_CONE_B, FLAG_SCENE_TEMP_03,
     EVENT(MarsLighthouseEntrance_StartPoundIceCone)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_TELEPORT, PSYNERGY_PHASE_AFTER_CAST), 17,
     CONDITION_ALWAYS, EVENT(MarsLighthouseEntrance_TeleportFromLapis)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_TELEPORT, PSYNERGY_PHASE_AFTER_CAST), 18,
     CONDITION_ALWAYS, EVENT(MarsLighthouseEntrance_Teleport)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PUSHED_STATUE,
     FLAG_LAPIS_STATUE_BLOCKS_FLAMES,
     EVENT(MarsLighthouseEntrance_PushStatueIntoFlames)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS,
     EVENT(MarsLighthouseEntrance_PushStatueIntoFlames)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 80,
     IF_FLAG_SET(FLAG_LAPIS_SHEET_CRACKED),
     EVENT(MarsLighthouseEntrance_BurstIceSheet)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 81,
     IF_FLAG_SET(FLAG_LAPIS_SECOND_SHEET_SHOWN),
     EVENT(MarsLighthouseEntrance_BurstLapisIceSheet)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_LIT_TORCH, CONDITION_ALWAYS, 0},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_HITS),
     ACTOR_LAPIS_TORCH_A, CONDITION_ALWAYS,
     EVENT(MarsLighthouseEntrance_BlazeLapisStatue)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_AFTER_END),
     ACTOR_LAPIS_TORCH_A, CONDITION_ALWAYS,
     EVENT(MarsLighthouseEntrance_WatchFireballCrackIce)},
    {TRIGGER_TALK, 64, CONDITION_ALWAYS,
     EVENT(MarsLighthouseEntrance_BlockRetreatAtChest)},
    {TRIGGER_TABLE_END},
};

struct Trigger gMarsLighthouseEntrance_BalmTriggers[] = {
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
    {TRIGGER_TALK, ACTOR_BALM, CONDITION_ALWAYS,
     EVENT(MarsLighthouseEntrance_FightBalm)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_ACTS),
     ACTOR_BALM_WEST_STATUE, CONDITION_ALWAYS,
     EVENT(MarsLighthouseEntrance_SlideStatueOnIce)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_BALM_EAST_STATUE, CONDITION_ALWAYS,
     0},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS),
     ACTOR_BALM_ICE_CONE_A, FLAG_SCENE_TEMP_04,
     EVENT(MarsLighthouseEntrance_StartPoundIceCone)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS),
     ACTOR_BALM_ICE_CONE_B, FLAG_SCENE_TEMP_05,
     EVENT(MarsLighthouseEntrance_StartPoundIceCone)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS),
     ACTOR_BALM_ICE_CONE_C, FLAG_SCENE_TEMP_06,
     EVENT(MarsLighthouseEntrance_StartPoundIceCone)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS),
     ACTOR_BALM_ICE_CONE_D, FLAG_SCENE_TEMP_07,
     EVENT(MarsLighthouseEntrance_StartPoundIceCone)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS),
     ACTOR_BALM_ICE_CONE_E, FLAG_SCENE_TEMP_08,
     EVENT(MarsLighthouseEntrance_StartPoundIceCone)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_TELEPORT, PSYNERGY_PHASE_AFTER_CAST), 14,
     CONDITION_ALWAYS, EVENT(MarsLighthouseEntrance_TeleportFromBalm)},
    {TRIGGER_TABLE_END},
};

struct Trigger gMarsLighthouseEntrance_HiddenCircleTriggers[] = {
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
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_WEST_LIT_TORCH, CONDITION_ALWAYS, 0},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_HITS),
     ACTOR_WEST_TORCH, CONDITION_ALWAYS,
     EVENT(MarsLighthouseEntrance_BlazeHiddenCircleWestStatue)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_AFTER_END),
     ACTOR_WEST_TORCH, CONDITION_ALWAYS,
     EVENT(MarsLighthouseEntrance_WatchFireballCrackHiddenCircleIce)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_EAST_LIT_TORCH, CONDITION_ALWAYS, 0},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_HITS),
     ACTOR_EAST_TORCH, CONDITION_ALWAYS,
     EVENT(MarsLighthouseEntrance_BlazeHiddenCircleEastStatue)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_AFTER_END),
     ACTOR_EAST_TORCH, CONDITION_ALWAYS,
     EVENT(MarsLighthouseEntrance_WatchFireballMeltIce)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 80,
     IF_FLAG_SET(FLAG_HIDDEN_CIRCLE_SHEET_SHOWN),
     EVENT(MarsLighthouseEntrance_BurstHiddenCircleIceSheet)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_TELEPORT, PSYNERGY_PHASE_AFTER_CAST), 14,
     CONDITION_ALWAYS, EVENT(MarsLighthouseEntrance_TeleportFromHiddenCircle)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0,
     CONDITION_ALWAYS, EVENT(MarsLighthouseEntrance_RevealTeleportCircle)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0,
     CONDITION_ALWAYS, EVENT(MarsLighthouseEntrance_HideTeleportCircle)},
    {TRIGGER_TABLE_END},
};

struct Trigger gMarsLighthouseEntrance_ValkyrieTriggers[] = {
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
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS),
     ACTOR_VALKYRIE_ICE_CONE_A, FLAG_SCENE_TEMP_10,
     EVENT(MarsLighthouseEntrance_StartPoundIceCone)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS),
     ACTOR_VALKYRIE_ICE_CONE_B, FLAG_SCENE_TEMP_11,
     EVENT(MarsLighthouseEntrance_StartPoundIceCone)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS),
     ACTOR_VALKYRIE_ICE_CONE_C, FLAG_SCENE_TEMP_12,
     EVENT(MarsLighthouseEntrance_StartPoundIceCone)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS),
     ACTOR_VALKYRIE_ICE_CONE_D, FLAG_SCENE_TEMP_13,
     EVENT(MarsLighthouseEntrance_StartPoundIceCone)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS),
     ACTOR_VALKYRIE_ICE_CONE_E, FLAG_SCENE_TEMP_14,
     EVENT(MarsLighthouseEntrance_StartPoundIceCone)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS),
     ACTOR_VALKYRIE_ICE_CONE_F, FLAG_SCENE_TEMP_15,
     EVENT(MarsLighthouseEntrance_StartPoundIceCone)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_TELEPORT, PSYNERGY_PHASE_AFTER_CAST), 12,
     CONDITION_ALWAYS, EVENT(MarsLighthouseEntrance_TeleportFromValkyrieWest)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_TELEPORT, PSYNERGY_PHASE_AFTER_CAST), 13,
     CONDITION_ALWAYS, EVENT(MarsLighthouseEntrance_TeleportFromValkyrieEast)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PUSHED_STATUE,
     FLAG_VALKYRIE_STATUE_BLOCKS_FLAMES,
     EVENT(MarsLighthouseEntrance_PushStatueIntoValkyrieFlames)},
    {TRIGGER_TABLE_END},
};

struct Trigger gMarsLighthouseEntrance_SolBladeTriggers[] = {
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
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};
#undef MarsLighthouseEntrance_DropThroughCrack
