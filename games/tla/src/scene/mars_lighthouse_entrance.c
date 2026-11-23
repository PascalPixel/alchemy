/// @file
/// The Mars Lighthouse from its doors up to the dragons' hall: the steps
/// outside in the storm and the frozen lower floors inside, in seven scenes
/// that share this overlay.
///
/// Felix climbs the ladders outside in the rain to the doors. Inside is the
/// main hall, a maze of ice cones that Pound hammers flat, with arched
/// doorways along its north wall. Up the stairs from one of them, at a dead
/// end, a shaft of ice pokes through the floor: Grind on it sends a crack down
/// into the ice wall in the small room east of the main hall, and Burst then
/// blows that wall away. Behind it a room holds a teleport circle, and west of
/// that a chamber of dragon heads breathes fire down at the floor until a
/// white dragon statue, pushed all the way east, blocks the last of them.
/// Past black ice where Felix slides until something stops him, white urn
/// torches lit by Blaze send fireballs that crack a sheet of ice for Burst and
/// melt the ice in their path, and Reveal shows a hidden circle in a room of
/// stone posts. Balm, the Mercury Djinni, waits at the end of a black ice
/// slide, and the Teleport Lapis lies in a chest on a ledge above the first
/// room's circle. Teleport from the hidden circle leads on to the Valkyrie
/// Mail rooms, where a dragon face wakes and crawls west breathing fire while
/// Felix crosses the ice ahead of it, and holes in the floor drop him to the
/// dragons' hall; the jumping puzzle round the Sol Blade chest has cracked
/// tiles that do the same. Last of all, Blaze at the main hall's westernmost
/// torch cracks its big sheet of ice, on the way to the Mars Djinni Fugue.

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
#include "shared/event_check_spheres_overlap.h"
#include "field/field_effect.h"
#include "system/dma.h"
#include "field/actor_script.h"
#include "system/frame.h"
#include "text/msg_ids.h"
#include "window/window.h"

/// Felix has teleported from the north circle once.
#define FLAG_TELEPORTED_ONCE 0xa36
/// Grind on the shaft of ice cracked the ice sheet a floor below.
#define FLAG_ICE_SHAFT_GROUND 0xa3e
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

/// The teleport circles that lead within one scene, each the cell behaviour
/// of its circle, the exit Teleport takes from it and the entrance on it. In
/// the Teleport Lapis rooms the south circle is the one behind the burst ice
/// wall and the north circle the one by the Teleport Lapis chest; in the
/// Valkyrie Mail rooms the north circle is in the room by the long hall and
/// the south one below it, by a dragon head on the wall. Each circle sends
/// the party to the other of its pair.
enum MarsLighthouseEntranceTeleportCircle {
  LAPIS_SOUTH_CIRCLE = 17,
  LAPIS_NORTH_CIRCLE = 18,
  VALKYRIE_NORTH_CIRCLE = 12,
  VALKYRIE_SOUTH_CIRCLE = 13
};

/// The other cell behaviours this overlay's trigger rows answer, besides the
/// circles' and the holes' (a hole's behaviour is the exit Felix drops
/// through). Black ice Felix slides across; the Sol Blade room's cracked
/// tiles, of which those with MAP_CELL_FLOOR_HOLE give way, and its pillar
/// tops; the cell at the head of the Valkyrie Mail rooms' slide that wakes
/// the dragon face; the ice wall Grind cracks and the sheet over the lower
/// passage, and the main hall's big sheet of ice, each answering Burst.
enum {
  CELL_BEHAVIOR_BLACK_ICE = 30,
  CELL_BEHAVIOR_CRACKED_ICE_TILE = 31,
  CELL_BEHAVIOR_PILLAR_TOP = 35,
  CELL_BEHAVIOR_WAKE_DRAGON_FACE = 35,
  CELL_BEHAVIOR_CRACKED_ICE_WALL = 80,
  CELL_BEHAVIOR_BIG_ICE_SHEET = 81
};

/// Balm's room's entrance at the north end of its east hall, where the flame
/// thrower is, which the set-up reads.
enum { BALM_EAST_HALL_NORTH_ENTRANCE = 10 };

/// The Valkyrie Mail room's entrances that the set-up reads: onto its southwest
/// floor from the southeast and the middle east floors, and onto its north
/// floor from the middle west one.
enum {
  VALKYRIE_SOUTHWEST_FROM_SOUTHEAST = 2,
  VALKYRIE_SOUTHWEST_FROM_MIDDLE_EAST = 4,
  VALKYRIE_NORTH_FROM_MIDDLE_WEST = 11
};

/// The attribute bit that makes a floor cell here a hole Felix drops through
/// to the room below; the field reads it elsewhere as MAP_CELL_CLIMBABLE.
#define MAP_CELL_FLOOR_HOLE MAP_CELL_CLIMBABLE

/// The frames Felix takes to shrink and sink out of sight through a hole.
#define SINK_FRAMES 40

/// The cell columns where a pushed white dragon statue stands in the flames'
/// way: under the last dragon head at the east end of the flame chamber, and
/// under the dragon head on the wall of the Valkyrie Mail rooms' urn torch.
enum { LAPIS_STATUE_BLOCKING_COLUMN = 20, VALKYRIE_STATUE_BLOCKING_COLUMN = 5 };

/// The cell columns Balm's room's dragon statue stops in on the black ice:
/// pushed west from its place, from where it slides on to the ice's west end,
/// and pushed east from there, from where it slides back.
enum { ICE_STATUE_PUSHED_WEST_COLUMN = 11, ICE_STATUE_PUSHED_EAST_COLUMN = 10 };

/// The rooms up the main hall's doorways (Fugue's scene): two ice cones, the
/// first placed out of sight at the map's corner, the shaft of ice Grind
/// cracks (an unseen marker on it) and Fugue.
enum MarsLighthouseEntranceFugueActorId {
  ACTOR_FUGUE_ICE_CONE_A = ACTOR_FIRST_PLACED,
  ACTOR_FUGUE_ICE_CONE_B,
  ACTOR_ICE_SHAFT,
  ACTOR_FUGUE
};

/// The main hall and the rooms behind it (the Teleport Lapis scene): the
/// white dragon statue pushed into the flames, the flame chamber's four dragon
/// heads, the urn torches at the main hall's west end (one lit in the same
/// place as an unlit one, each shown by whether the lighthouse has thawed),
/// two ice cones, the first placed out of sight at the map's corner, and the
/// two ice sheets.
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
  ACTOR_SECOND_ICE_SHEET,
  ACTOR_LAPIS_SPARKLE,  ///< the sparkle that leaves the lit torch
  ACTOR_LAPIS_FIREBALL, ///< the fireball the sparkle sets rolling
  /// The Teleport Lapis chest, the first actor its map's chests make.
  ACTOR_TELEPORT_LAPIS_CHEST = 64
};

/// Balm's rooms: the dragon head and the urn torch its flames light, which
/// sends fireballs down the corridor of ice cones, the two white dragon
/// statues beside the doorway onto the black ice, five ice cones and Balm.
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
  ACTOR_BALM,
  ACTOR_BALM_SPARKLE,        ///< the sparkle that leaves the lit torch
  ACTOR_BALM_SECOND_SPARKLE, ///< placed, and sent nowhere
  ACTOR_BALM_FIREBALL        ///< the fireball the sparkle sets rolling
};

/// The hidden circle's rooms: on each side a lit urn torch and the urn torch
/// Blaze sends a fireball from, and the ice sheet the west fireball cracks.
enum MarsLighthouseEntranceHiddenCircleActorId {
  ACTOR_WEST_LIT_TORCH = ACTOR_FIRST_PLACED,
  ACTOR_WEST_TORCH,
  ACTOR_EAST_LIT_TORCH,
  ACTOR_EAST_TORCH,
  ACTOR_HIDDEN_CIRCLE_ICE_SHEET,
  ACTOR_HIDDEN_CIRCLE_SPARKLE, ///< the sparkle that leaves the lit torch
  ACTOR_HIDDEN_CIRCLE_FIREBALL ///< the fireball the sparkle sets rolling
};

/// The Valkyrie Mail rooms: the white dragon statue pushed into the flames,
/// the dragon head on the wall and its urn torch, the dragon face that wakes
/// and the torch below it, six ice cones, and the sparkle and fireball a torch
/// sends.
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
  ACTOR_VALKYRIE_SPARKLE, ///< the sparkle that leaves the lit torch
  ACTOR_VALKYRIE_FIREBALL ///< the fireball the sparkle sets rolling
};
extern u8 gInventorySnapshot[];

void Actor_SetSpriteColorMode(struct Actor *actor, s32 value);

/// An urn torch starts with its sprite flags clear.
s32 MarsLighthouseEntrance_SetUpStatue(struct Actor *actor) {
  Actor_SetSpriteFlags(actor, 0);
  return 0;
}

/// Talking to the Mars Djinni Fugue, east of the shaft of ice in the room at
/// the dead end up the main hall's stairs, from its east or south side,
/// starts the battle with it.
void MarsLighthouseEntrance_FightFugue(void) {
  Field_RequestDjinnBattle(ACTOR_FUGUE, DJINNI_FUGUE);
}

/// Talking to the Mercury Djinni Balm, at the west end of the black ice below
/// the two white dragon statues, starts the battle with it.
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

/// The waking dragon face in the Valkyrie Mail rooms' long hall has crawled
/// to its west end and stops breathing fire.
s32 MarsLighthouseEntrance_DryDragonFace(void) {
  Ride_SetEntry(0, FLAME_THROWER_DRY | 1);
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

/// Unused: as the dragon hall's, makes the next cast of Blaze a long spray
/// (FieldWork.blazeLongSpray): cast at a target, Blaze then takes
/// BLAZE_LONG_SPRAY over BLAZE_SPRAY, and each cast clears it as it starts.
UNUSED void MarsLighthouseEntrance_StartBlazeOnStatue(void) {
  struct FieldWork *event = Ram_HeapSlots->fieldWork;

  event->blazeLongSpray = 1;
}

extern struct MapCell gMapGridCellBuffer[];

#include "../field/map_cell_get.c"

#include "shared/map_cell_set.c"

#include "../field/actor_set_sprite_priority.c"

void MapGrid_SetCellHeight();
void Teleport_PlayWhirl(void);
void Teleport_PlaySparks(void);
/// Set while a whirlwind carries the leader in the two Jupiter Lighthouse rooms
/// that raise one (jupiter_lighthouse_room_m.c and
/// jupiter_lighthouse_room_phaetons_blade_chest.c); shared/whirlwind_update.h
/// clears it as the ride ends. The rooms of this overlay have no whirlwind,
/// and nothing here but Scene_Initialize's clear touches it.
s32 gWhirlwindRiding BSS_DATA;
u16 gPaletteCycleBright BSS_DATA;
u16 *gPaletteCycleSlots BSS_DATA;
u16 gPaletteCycleTimer BSS_DATA;
u16 gPaletteCycleDelay BSS_DATA ALIGNED(4);
u16 *gPaletteSlots BSS_DATA;
u16 *gPaletteColors BSS_DATA;
u16 *gPaletteCycleColors BSS_DATA;
u16 gPaletteCycleLength BSS_DATA;
u16 gPaletteCycleIndex BSS_DATA ALIGNED(4);
s16 gMarsLighthouseEntrance_DragonFireTimer BSS_DATA ALIGNED(4);
u16 gMarsLighthouseEntrance_HoleMaskVram BSS_DATA;
u32 gMarsLighthouseEntrance_HoleMaskSprite[3] BSS_DATA;
volatile s16 gSlideOnIce_Stopped BSS_DATA;
struct CellTarget gCrackedTiles BSS_DATA;
void Map_CopyPalette(s32 scene, s32 a1);
s32 FieldEffect_StartBlankOverlay();
s32 Weather_StartRain();

s32 MapCell_GetAttributes();
s32 Map_GetTerrainHeight();
void Scene_SetActorSpritePriority();

/// Sets the cells under an ice cone: standing, it blocks the way, the ground
/// there is raised and it is drawn in front; flattened (its flag, kept in its
/// timer, set), the cells are clear and the ground is lowered again.
void MarsLighthouseEntrance_SetIceConeCells(struct Actor *actor) {
  s32 z, upperCell, height, groundCell, x;
  struct Actor *cone;
  u8 *layer;

  cone = actor;
  x = FIELD_CELL(cone->x.fixed);
  z = FIELD_CELL(cone->z.fixed);
  groundCell = MapCell_GetAttributes(0, x, z);
  upperCell = MapCell_GetAttributes(2, x, z);
  layer = &cone->layer;
  height = CELL_HEIGHT_STEPS(
      Map_GetTerrainHeight(cone->layer, cone->x.fixed, cone->z.fixed));
  if (Event_GetFlag(cone->timer)) {
    upperCell &= ~CELL_CODE_BITS;
    groundCell &= ~MAP_CELL_JUMP;
    if (CELL_HEIGHT_STEPS(cone->groundHeight) != height)
      height -= 4;
  } else {
    upperCell &= CELL_ATTRIBUTE_BITS;
    upperCell |= CELL_ATTRIBUTES(CELL_BEHAVIOR_WALL, 0);
    groundCell |= MAP_CELL_JUMP;
    height += 4;
    Scene_SetActorSpritePriority(cone, 2);
  }
  MapCell_SetAttributes(0, x, z, groundCell);
  MapCell_SetAttributes(2, x, z, upperCell);
  MapGrid_SetCellHeight(FIELD_CELL_ORIGIN(x), FIELD_CELL_ORIGIN(z), *layer,
                        height);
}

/// Places the ice cones of a list ended by ACTOR_LIST_END, each row three
/// halfwords: the cone, its standing pose and the flag set once Pound has
/// flattened it, which picks the flattened pose after it.
void MarsLighthouseEntrance_PlaceIceCones(u16 *table) {
  u16 *row;
  struct FieldActor *object;
  s16 flag;
  s16 animation;

  if (table[0] == ACTOR_LIST_END)
    return;
  for (row = table; row[0] != ACTOR_LIST_END; row += 3) {
    object = (struct FieldActor *)Actor_Get(row[0]);
    animation = row[1];
    flag = row[2];

    Actor_SetSpriteFlags(object, 0);
    Actor_SelectAnimation(object, (u16)animation + Event_GetFlag((u16)flag));
    object->motionFlags = 0;
    object->collisionFlags &= ~ACTOR_SOLID;
    object->timer = flag;
    MarsLighthouseEntrance_SetIceConeCells(object);
  }
}

/// One blow of Pound's hammer on an ice cone: for the first eight the cone
/// grows shorter and a chip of blue ice flies off it, lower each time; when
/// Pound is done the cone is gone, its flag set and its cells cleared
/// (MarsLighthouseEntrance_SetIceConeCells), so Felix can walk over the spot.
void MarsLighthouseEntrance_PoundIceCone(s32 mode, s32 index) {
  struct FieldEffectWork *holder = Ram_HeapSlots->fieldEffectWork;
  struct Actor *actor = Actor_Get(holder->secondActorId);

  if (index <= 7) {
    struct EffectOptions options;

    options.priority = 0;
    options.type = SPRITE_BLUE_CRYSTAL;
    options.startScaleX = options.startScaleY = FIXED_ONE;
    options.targetScaleX = options.targetScaleY = FIXED_ONE / 2;
    options.rotation = (Random16() & 15) << 12;
    FieldEffect_Spawn(actor->x.fixed,
                      actor->y.fixed - index * 3 * toFIXED(1) + toFIXED(16),
                      actor->z.fixed, ((Random16() & 15) - 8) << 13, 0, 0,
                      EFFECT_USE_ROTATION | EFFECT_USE_TYPE |
                          EFFECT_USE_START_SCALE | EFFECT_USE_PRIORITY,
                      &options);
    actor->scaleY -= FIXED_ONE * 31 / 256;
  }
  if (mode == 1) {
    actor->scaleY = FIXED_ONE;
    Event_SetFlag(actor->timer);
    MarsLighthouseEntrance_SetIceConeCells(Actor_Get(holder->secondActorId));
    actor->x.fixed = actor->y.fixed = actor->z.fixed = 0;
  }
}

/// Felix casts Pound on one of the ice cones that stand on the lighthouse's
/// frozen floors (one up the main hall's stairs, one in the main hall, five in
/// the corridor of Balm's rooms, six in the Valkyrie Mail rooms' long hall): a
/// great hammer beats it down into the floor, chips of ice flying, until it is
/// gone and the way is clear. Each blow comes here with the phase and its
/// count, and MarsLighthouseEntrance_PoundIceCone does the work.
void MarsLighthouseEntrance_StartPoundIceCone(s32 mode, s32 index) {
  MarsLighthouseEntrance_PoundIceCone(mode, index);
}

extern u32 gDebris_Script[];

/// Throws six chips of ice from an actor as its ice cracks or breaks.
void MarsLighthouseEntrance_ScatterIceChips(s32 id) {
  struct Actor *source = Actor_Get(id);
  s32 i;

  for (i = 0; i < 6; i++) {
    FIXED x = source->x.fixed + (Random16() << 4) - (Random16() << 4);
    struct Actor *actor = Import_ActorCreate_(
        SPRITE_WIND_STONE, x, (Random16() << 3) + source->y.fixed + toFIXED(4),
        source->z.fixed + (Random16() << 4) - (Random16() << 4));

    if (actor != NULL) {
      s32 angle;
      s32 radius;
      s32 scale;

      angle = Random16();
      radius = Random16();
      scale = (Random16() >> 1) + FIXED_ONE / 4;
      Actor_StartScript(actor, gDebris_Script);
      Actor_SetSpriteFlags(actor, 0);
      actor->velocityY = angle * 4 + toFIXED(2);
      actor->velocityZ = Iwram_MulFX(radius * 8, Trig_Cos(angle));
      actor->velocityX = Iwram_MulFX(radius * 8, Math_Sin(angle));
      actor->acceleration = 0;
      actor->gravity = toFIXED(33) / 100;
      actor->scaleX = scale;
      actor->scaleY = scale;
      actor->bounce = FIXED_ONE / 2;
      Actor_SetSpriteColorMode(actor, 1);
    }
  }
}

/// Felix casts Grind on the shaft of ice poking up through the floor of the
/// dead-end room up the main hall's stairs: it crumbles away in bursts of ice
/// chips, and the scene cuts to the small room east of the main hall below,
/// where the floor shakes and a crack runs through the ice wall, ready for
/// Burst, before Felix is brought back.
void MarsLighthouseEntrance_GrindIce(void) {
  s32 x;
  s32 count;

  Event_PlaySound(SE_CRUMBLE);
  x = 32;
  count = 6;
  do {
    MapGrid_CopyCellPictureCellsRect_(x, 125, 3, 3, 39, 85);
    MarsLighthouseEntrance_ScatterIceChips(ACTOR_ICE_SHAFT);
    count--;
    System_Wait(10);
    x += 3;
  } while (count >= 0);
  ObjectTable_Snapshot();
  FieldEvent_SetExitRequest(
      MARS_LIGHTHOUSE_ENTRANCE_FUGUE_TO_MARS_LIGHTHOUSE_ENTRANCE_TELEPORT_LAPIS_EAST_B);
}

/// Felix examines the shaft of ice in the dead-end room up the main hall's
/// stairs: "Felix checked the ice." and "A shaft of ice is poking up through
/// the floor."
void MarsLighthouseEntrance_CheckIce(void) {
  // "Felix checked the ice."
  FieldMessage_Show((s32)MsgMarsLighthouseEntranceCheckedIce,
                    FIELD_MESSAGE_ZOOM);
  // "A shaft of ice is poking up through the floor."
  FieldMessage_Show((s32)MsgMarsLighthouseEntranceCheckedIce + 1,
                    FIELD_MESSAGE_ZOOM);
  gFieldWork->fieldMenu = 1;
}

/// Felix casts Teleport on the north circle, beside the Teleport Lapis chest:
/// the screen turns grey, sparks swirl round him and he appears on the south
/// circle in the room behind the burst ice wall. The first time, the game
/// moves him and the camera there itself; after that it takes the exit
/// between the two circles.
void MarsLighthouseEntrance_TeleportFromLapisNorthCircle(void) {
  if (!Flag_IsSet(FLAG_TELEPORTED_ONCE)) {
    Teleport_StartShimmer();
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    Teleport_PlaySparks();
    Actor_Get(gMainRoutineWork.leaderActorId)->update = NULL;
    FieldActor_SetSpriteColor(Actor_Get(gMainRoutineWork.leaderActorId),
                              SPRITE_COLOR_NORMAL);
    Actor_SetHorizontalPositionWithTerrain_(gMainRoutineWork.leaderActorId,
                                            toFIXED(688), toFIXED(360));
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
    FieldEvent_SetExitRequest(LAPIS_NORTH_CIRCLE);
    FieldEvent_Wait(40);
    Teleport_StopShimmer();
  }
}

/// Felix casts Teleport on the south circle, in the room behind the burst
/// ice wall: in a swirl of sparks he appears on the north circle, on the
/// ledge beside the Teleport Lapis chest.
void MarsLighthouseEntrance_TeleportFromLapisSouthCircle(void) {
  Teleport_StartShimmer();
  Teleport_PlaySparks();
  gMainRoutineWork.teleportState = 2;
  FieldEvent_SetExitRequest(LAPIS_SOUTH_CIRCLE);
  FieldEvent_Wait(40);
  Teleport_StopShimmer();
}

/// Felix casts Burst on the cracked ice wall in the small room east of the
/// main hall (cells (51, 41) to (53, 41)): it blows apart in fiery blasts,
/// chips of ice flying, and the arched doorway behind it is clear.
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

/// Felix casts Blaze at the urn torch at the west end of the main hall: a
/// fireball sets off east along the hall toward the big sheet of ice.
void MarsLighthouseEntrance_BlazeLapisTorch(void) {
  MarsLighthouseEntrance_ShootFireballAtLapisIceSheet();
}

/// As the fireball from the main hall's west torch rolls east, the camera
/// follows it along the hall to the sheet of ice it cracks and holds there.
void MarsLighthouseEntrance_WatchFireballCrackIce(void) {
  gFieldWork->psynergy = 0;
  Flag_Clear(FLAG_MENU_OPEN);
  Flag_Clear(FLAG_FIELD_HANDLING_REQUESTS);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_SetSpeed(1);
  FieldCamera_MoveWithinMapBounds_(toFIXED(168), 0, toFIXED(664), 1);
  FieldCamera_WaitForArrival();
  System_Wait(10);
  FieldCamera_SetSpeed(4);
  FieldCamera_MoveWithinMapBounds_(toFIXED(472), 0, toFIXED(664), 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(90);
  Event_End();
}

/// Draws the main hall's big sheet of ice cracked, on both floors.
void MarsLighthouseEntrance_DrawLapisCrackedSheet(void) {
  MapGrid_CopyCellAttributes(14, 55, 14, 3, 14, 41);
  MapGrid_CopyCellsLayered(64, 64, 14, 3, 78, 41);
  MapGrid_CopyCellsLayered(64, 64, 14, 3, 14, 104);
  MapGrid_CopyCellAttributes(64, 64, 14, 3, 78, 41);
  MapGrid_CopyCellAttributes(64, 64, 14, 3, 14, 105);
}

/// The fireball from the main hall's west torch reaches the big sheet of ice
/// and cracks it, with a crack of ice.
void MarsLighthouseEntrance_CrackLapisIceSheet(void) {
  struct Actor *actor = Actor_Get(ACTOR_SECOND_ICE_SHEET);

  if (!Flag_IsSet(FLAG_LAPIS_SECOND_SHEET_BURST)) {
    Event_PlaySound(SE_ICE_CRACK);
    Flag_Set(FLAG_LAPIS_SECOND_SHEET_SHOWN);
    actor->x.fixed = INTtoFIXED(actor->homeX);
    actor->z.fixed = INTtoFIXED(actor->homeZ);
    Flag_Set(FLAG_LAPIS_SECOND_SHEET_CRACKED);
    MarsLighthouseEntrance_DrawLapisCrackedSheet();
  }
}

/// Defines what Burst does to a sheet of ice: as it breaks, the screen flashes;
/// eight columns from x are drawn broken at row z, rows high, from the cells at
/// row brokenZ, and take the ground of the cells at column groundSourceX,
/// groundRows high from row groundZ; the sheet's actor goes and flag is set.
/// Chips of ice scatter from it every half second, and the screen flashes white
/// at the end.
#define DEFINE_BURST_ICE_SHEET(name, x, brokenZ, rows, z, groundSourceX,       \
                               groundZ, groundRows, sheet, flag)               \
  void name(s32 state, s32 frame) {                                            \
    if (state == 1) {                                                          \
      Event_PlaySound(SE_FLASH);                                               \
      PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);                \
      PaletteFade_Interpolate(60);                                             \
      MapGrid_CopyCellsLayered(x, brokenZ, 8, rows, x, z);                     \
      MapGrid_CopyCellAttributes(groundSourceX, groundZ, 8, groundRows, x,     \
                                 groundZ);                                     \
      Actor_SetPosition(sheet, 0, 0);                                          \
      Flag_Set(flag);                                                          \
    }                                                                          \
    if (frame > 96 && frame < 272 && frame % 30 == 0)                          \
      MarsLighthouseEntrance_ScatterIceChips(sheet);                           \
    if (frame == 276) {                                                        \
      PaletteFade_ApplyTarget(RGB(31, 31, 31), PALETTES_ALL);                  \
      PaletteFade_Interpolate(20);                                             \
    }                                                                          \
  }

/// Felix casts Burst on the big sheet of ice in the east of the main hall
/// (cells (29, 43) and (30, 43)), once the west torch's fireball has cracked
/// it: it blows apart in fiery blasts and the way past it is clear.
DEFINE_BURST_ICE_SHEET(MarsLighthouseEntrance_BurstLapisIceSheet, 28, 92, 8,
                       100, 92, 38, 8, ACTOR_SECOND_ICE_SHEET,
                       FLAG_LAPIS_SECOND_SHEET_BURST)

/// Each time Felix pushes or Moves the white dragon statue along the flame
/// chamber, west of the room behind the burst ice wall, the game checks where
/// it stands. Once it is under the last dragon head at the chamber's east end
/// (cell (20, 7)), it settles a pixel south into the head's flames, blocks
/// them, and stays there from then on.
void MarsLighthouseEntrance_PushStatueIntoFlames(s32 unused, s32 id) {
  if (id == ACTOR_PUSHED_STATUE) {
    struct Actor *actor = Actor_Get(ACTOR_PUSHED_STATUE);

    if (FIELD_CELL(actor->x.fixed) == LAPIS_STATUE_BLOCKING_COLUMN) {
      FieldEvent_Wait(5);
      actor->z.fixed += toFIXED(1);
      *(u8 *)&actor->riseCounter = 1;
      Flag_Set(FLAG_LAPIS_STATUE_BLOCKS_FLAMES);
    }
  }
}

/// Felix examines the Teleport Lapis chest, on the ledge above the north
/// circle; besides the chest opening as usual, Retreat stops working, and
/// casting it answers "Retreat... It doesn't work here."
/// Retreat works again once the party enters another scene: every ordinary
/// arrival clears the flag (Scene_Arrive), but walking on within this scene
/// does not.
void MarsLighthouseEntrance_BlockRetreatAtChest(void) {
  GameFlag_Set(FLAG_RETREAT_BLOCKED);
}

/// Felix casts Teleport on the circle in the room of the Orihalcon chest: in a
/// swirl of sparks he appears on the hidden circle among the stone posts.
void MarsLighthouseEntrance_TeleportFromBalm(void) {
  Teleport_StartShimmer();
  Teleport_PlaySparks();
  gMainRoutineWork.teleportState = 2;
  FieldEvent_SetExitRequest(
      MARS_LIGHTHOUSE_ENTRANCE_BALM_TO_MARS_LIGHTHOUSE_ENTRANCE_HIDDEN_CIRCLE_NORTH_B);
  FieldEvent_Wait(40);
  Teleport_StopShimmer();
}

/// Felix Moves the west one of the two white dragon statues beside the
/// doorway onto Balm's black ice: pushed west, it slides along the ice to the
/// wall (x 152), where it will stop Felix's own slides; pushed east from
/// there, it slides back to its place by the doorway (x 200).
void MarsLighthouseEntrance_SlideStatueOnIce(s32 unused, s32 id) {
  struct Actor *actor = Actor_Get(id);

  if (FIELD_CELL(actor->x.fixed) == ICE_STATUE_PUSHED_WEST_COLUMN) {
    actor->speedLimit = toFIXED(1) / 4;
    actor->acceleration = toFIXED(1) / 8;
    FieldActor_SetMoveTargetFX(actor, toFIXED(152), actor->y.fixed,
                               toFIXED(792));
  }
  if (FIELD_CELL(actor->x.fixed) == ICE_STATUE_PUSHED_EAST_COLUMN) {
    actor->speedLimit = toFIXED(1) / 4;
    actor->acceleration = toFIXED(1) / 8;
    FieldActor_SetMoveTargetFX(actor, toFIXED(200), actor->y.fixed,
                               toFIXED(792));
  }
}

/// Felix casts Blaze at the urn torch at the west end of the room beyond the
/// first black ice: the camera moves to the torch as it sends a fireball east
/// toward the sheet of ice over the lower passage.
void MarsLighthouseEntrance_BlazeHiddenCircleWestTorch(void) {
  FieldCamera_SetSpeed(1);
  FieldCamera_MoveTo(88, CAMERA_KEEP, 696, 1);
  MarsLighthouseEntrance_ShootFireballAtHiddenCircleIceSheet();
}

/// The camera holds for two seconds on the sheet of ice the west torch's
/// fireball cracks.
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

/// The west torch's fireball reaches the sheet of ice before the hidden
/// circle's rooms, which cracks with a crack of ice, unless Burst has broken it
/// already.
void MarsLighthouseEntrance_CrackHiddenCircleIceSheet(void) {
  struct Actor *actor = Actor_Get(ACTOR_HIDDEN_CIRCLE_ICE_SHEET);

  if (!Flag_IsSet(FLAG_HIDDEN_CIRCLE_SHEET_BURST)) {
    Event_PlaySound(SE_ICE_CRACK);
    Flag_Set(FLAG_HIDDEN_CIRCLE_SHEET_SHOWN);
    actor->x.fixed = INTtoFIXED(actor->homeX);
    actor->z.fixed = INTtoFIXED(actor->homeZ);
    Flag_Set(FLAG_HIDDEN_CIRCLE_SHEET_CRACKED);
  }
}

/// Felix casts Blaze at the urn torch under the great carved dragon face in
/// the right-hand room of ice cones: the torch sends a fireball rolling south
/// through the ice.
void MarsLighthouseEntrance_BlazeHiddenCircleEastTorch(void) {
  MarsLighthouseEntrance_ShootFireballThroughIce();
}

/// The camera follows the east torch's fireball south as it melts the ice in
/// its path, and holds on the cleared way.
void MarsLighthouseEntrance_WatchFireballMeltIce(void) {
  gFieldWork->psynergy = 0;
  Flag_Clear(FLAG_MENU_OPEN);
  Flag_Clear(FLAG_FIELD_HANDLING_REQUESTS);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_SetSpeed(1);
  FieldCamera_MoveWithinMapBounds_(toFIXED(872), -1, toFIXED(168), 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(40);
  FieldCamera_MoveTo(872, CAMERA_KEEP, 264, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(120);
  Event_End();
}

/// Felix casts Teleport on the hidden circle among the stone posts: in a
/// swirl of sparks he appears on the circle in the room of the Orihalcon
/// chest.
void MarsLighthouseEntrance_TeleportFromHiddenCircle(void) {
  Teleport_StartShimmer();
  Teleport_PlaySparks();
  gMainRoutineWork.teleportState = 2;
  FieldEvent_SetExitRequest(
      MARS_LIGHTHOUSE_ENTRANCE_HIDDEN_CIRCLE_TO_MARS_LIGHTHOUSE_ENTRANCE_BALM_NORTH_B);
  FieldEvent_Wait(40);
  Teleport_StopShimmer();
}

/// While Felix's Reveal lasts, the teleport circle hidden in the floor among
/// the room's six stone posts shows (cells (53, 48) to (56, 50)).
void MarsLighthouseEntrance_RevealTeleportCircle(void) {
  MapGrid_CopyCells(53, 30, 4, 3, 53, 48);
  MapGrid_CopyCellsLayered(117, 30, 4, 3, 117, 48);
}

/// As Reveal fades, the hidden teleport circle among the stone posts is
/// covered again.
void MarsLighthouseEntrance_HideTeleportCircle(void) {
  MapGrid_CopyCells(53, 40, 4, 3, 53, 48);
  MapGrid_CopyCellsLayered(117, 45, 4, 3, 117, 48);
}

/// Felix casts Burst on the sheet of ice the west torch's fireball cracked
/// (cells (14, 42) and (15, 42)): it blows apart in fiery blasts and the
/// lower passage is open.
DEFINE_BURST_ICE_SHEET(MarsLighthouseEntrance_BurstHiddenCircleIceSheet, 12, 95,
                       6, 101, 76, 40, 5, ACTOR_HIDDEN_CIRCLE_ICE_SHEET,
                       FLAG_HIDDEN_CIRCLE_SHEET_BURST)

/// The east torch's fireball melts the ice in its path and opens the way
/// south to the doorway.
void MarsLighthouseEntrance_MeltIceInPath(void) {
  MapGrid_CopyCellAttributes(35, 11, 3, 6, 53, 11);
  MapGrid_CopyCellsLayered(35, 75, 3, 6, 117, 11);
  MapGrid_CopyCellsLayered(35, 75, 3, 6, 53, 75);
  Flag_Set(FLAG_HIDDEN_CIRCLE_ICE_MELTED);
}

/// The fireball in the Valkyrie Mail rooms' long hall has passed: the cells
/// of its row are cleared.
void MarsLighthouseEntrance_ClearFireballRow(void) {
  MapGrid_CopyCellAttributes(25, 9, 11, 3, 26, 9);
}

/// Felix casts Teleport on the north circle of the Valkyrie Mail rooms
/// (cells (12, 25) to (15, 27)): in a swirl of sparks he appears on the south
/// circle in the room below, by the dragon head on its west wall.
void MarsLighthouseEntrance_TeleportFromValkyrieNorth(void) {
  Teleport_StartShimmer();
  Teleport_PlaySparks();
  gMainRoutineWork.teleportState = 2;
  FieldEvent_SetExitRequest(VALKYRIE_NORTH_CIRCLE);
  FieldEvent_Wait(40);
  Teleport_StopShimmer();
}

/// Felix casts Teleport on the south circle of the Valkyrie Mail rooms
/// (cells (11, 44) to (14, 46)), by the dragon head on its west wall: in a
/// swirl of sparks he appears on the north circle.
void MarsLighthouseEntrance_TeleportFromValkyrieSouth(void) {
  Teleport_StartShimmer();
  Teleport_PlaySparks();
  gMainRoutineWork.teleportState = 2;
  FieldEvent_SetExitRequest(VALKYRIE_SOUTH_CIRCLE);
  FieldEvent_Wait(40);
  Teleport_StopShimmer();
}

/// Felix Moves the white dragon statue on the ledge at the west of the south
/// circle's room: pushed one cell east (cell (5, 43)), it stands
/// under the small dragon head on the wall and blocks the flames it breathes
/// at the urn torch, and stays there from then on.
void MarsLighthouseEntrance_PushStatueIntoValkyrieFlames(s32 unused, s32 id) {
  if (id == ACTOR_PUSHED_STATUE) {
    struct Actor *actor = Actor_Get(ACTOR_PUSHED_STATUE);

    if (FIELD_CELL(actor->x.fixed) == VALKYRIE_STATUE_BLOCKING_COLUMN) {
      FieldEvent_Wait(5);
      actor->z.fixed += toFIXED(1);
      Flag_Set(FLAG_VALKYRIE_STATUE_BLOCKS_FLAMES);
    }
  }
}

/// Each frame of a fall through the floor: the mask sprite is drawn over
/// Felix's cell, so that he sinks out of sight through the hole.
void MarsLighthouseEntrance_DrawHoleMask(void) {
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  s32 *view = &map->viewX;
  s32 x = view[0] & FIELD_PIXEL_MASK;
  s32 cameraZ = view[1] & FIELD_PIXEL_MASK;
  s32 originY = map->origin[1];
  s32 tile =
      gVramCacheEntries[gMarsLighthouseEntrance_HoleMaskVram].offset >> 5;
  struct Actor *leader = Actor_Get(gMainRoutineWork.leaderActorId);
  s32 y, z, depth, screenX, screenY;
  u32 *word;

  x = FIELD_CELL_ORIGIN(FIELD_CELL(leader->x.fixed)) - x;
  y = Map_GetTerrainHeight(0, leader->x.fixed, leader->z.fixed) - originY;
  z = FIELD_CELL_ORIGIN(FIELD_CELL(leader->z.fixed)) - cameraZ - originY;
  screenY = z - y;
  screenX = FIXEDtoINT(x);
  screenY = FIXEDtoINT(screenY) + 16;
  depth = FIXEDtoINT(y + z) + SCENE_SPRITE_DEPTH;
  if ((u32)(screenX + 15) > 334 || screenY < -15 || screenY > 239)
    return;
  screenX &= OBJ_X_MASK;
  screenY &= OBJ_Y_MASK;
  word = gMarsLighthouseEntrance_HoleMaskSprite;
  *word++ = 0;
  *word++ =
      screenY | OBJ_X(screenX) | OBJ_SIZE_16X32 | OBJ_MODE(OBJ_MODE_WINDOW);
  *word = tile | OBJ_PRIORITY(1);
  Oam_AddSpriteAtDepthClamped(gMarsLighthouseEntrance_HoleMaskSprite, depth);
}

/// Hides Felix's sprite below the floor as he drops through it: a sprite of
/// colour 1 in a VRAM block of its own, drawn each frame in the OBJ window
/// over his cell (MarsLighthouseEntrance_DrawHoleMask).
void MarsLighthouseEntrance_StartHoleMask(void) {
  u32 *buffer = Heap_Alloc(HOLE_MASK_BYTES);
  u32 *word = buffer;
  s32 count;

  for (count = 0; count < HOLE_MASK_BYTES / 4; count++) {
    *word++ = PIXELS_4BPP(1);
  }
  gMarsLighthouseEntrance_HoleMaskVram = VramCache_FindFreeEntry();
  Vram_Load(gMarsLighthouseEntrance_HoleMaskVram, HOLE_MASK_BYTES, buffer);
  Heap_Release(buffer);
  Task_AddFrame(MarsLighthouseEntrance_DrawHoleMask, TASK_PASS_FRAME);
}

/// Shows every layer and the sprites outside the windows, and every layer
/// inside the sprite window.
#define SET_SPRITE_WINDOW_LAYERS()                                             \
  do {                                                                         \
    u16 winout = WINOUT_WIN01_BG0 | WINOUT_WIN01_BG1 | WINOUT_WIN01_BG2 |      \
                 WINOUT_WIN01_BG3 | WINOUT_WIN01_OBJ | WINOUT_WINOBJ_BG0 |     \
                 WINOUT_WINOBJ_BG1 | WINOUT_WINOBJ_BG2 | WINOUT_WINOBJ_BG3;    \
                                                                               \
    REG_WINOUT = winout;                                                       \
  } while (0)

/// Felix walks onto one of the purple holes on the upper floor of the
/// Valkyrie Mail rooms (cells (15, 43) to (16, 46) east of the south circle,
/// and (54, 8) to (55, 11) in the long hall): startled, he raises his arms and
/// sinks out of sight, the screen closes in on him, and he drops into a room
/// of the dragons' hall below, landing in a puff of dust. The cell's behaviour
/// is the exit he takes.
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
    s32 value = EMOTE_IN_FRONT | EMOTE_EXCLAMATION;

    FieldActor_ShowEmote(gMainRoutineWork.leaderActorId, value, 0);
  }
  Actor_SelectAnimation(actor, ANIM_LEADER_WALK_BACKWARDS);
  Actor_SetAnimationSpeed(actor, ANIM_STEP_NORMAL * 3);
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
  cells += x + (z << 7) + MAPGRID_SIZE;
  if (!(cells->attributeB & MAP_CELL_FLOOR_HOLE)) {
    SET_SPRITE_WINDOW_LAYERS();
    REG_DISPCNT |= DISPCNT_OBJWIN_ON;
    MarsLighthouseEntrance_StartHoleMask();
  }
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_LEADER_ARMS_RAISED);
  FieldEvent_Wait(16);
  for (i = 0; i < SINK_FRAMES; i++) {
    if (i == 5)
      Event_PlaySound(SE_DUST_PUFF);
    actor->scaleX -= FIXED_ONE / 64;
    actor->scaleY -= FIXED_ONE * 3 / 256;
    actor->y.fixed -= toFIXED(6) / 10;
    System_Wait(1);
  }
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

/// Felix steps onto cell (52, 8) in the Valkyrie Mail rooms' long hall, at
/// the head of the slide down the ice: with a click the dragon face wakes,
/// and the game can no longer be saved. Great fireballs roll east along the
/// ice towards him, shattering the ice blocks in their way and bursting into
/// flames at the hall's east end, and the face crawls along the hall's north
/// wall, coming east almost to Felix, then turning and crawling back west out
/// of sight, still spitting fire. Felix has to cross the ice ahead of it.
/// While he stays at the head of the slide nothing else happens to him.
void MarsLighthouseEntrance_WakeDragonFace(void) {
  Event_PlaySound(SE_BUTTON_PRESS);
  Flag_Set(FLAG_NO_PLACE_TO_SAVE);
  Actor_SetScript(ACTOR_DRAGON_FACE,
                  gMarsLighthouseEntrance_DragonFaceGoesLeft);
  Ride_SetEntry(0, FLAME_FORWARD | 3);
  Flag_Set(FLAG_DRAGON_FACE_AWAKE);
}

/// Stepping onto the black ice in the Valkyrie Mail rooms' long hall, Felix
/// slides on across it; if a fireball has hit him he rides along with it.
void MarsLighthouseEntrance_SlideFromEdge(void) {
  Leader_SlideOnIceFromEdge();
  MarsLighthouseEntrance_SlideIfRiding();
}

void GameFlag_SetByte(s32 flag, s32 value);

/// Felix drops through a cracked tile of the Sol Blade room: startled, he
/// raises his arms and sinks out of sight, and lands in the room below. The
/// cell he fell through is kept, so the landing room can place him under it.
void MarsLighthouseEntrance_FallThroughCrack(const struct MapCell *cell) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.leaderActorId);
  FIXED z = leader->z.fixed;
  s32 i = FIELD_CELL(leader->x.fixed);

  GameFlag_SetByte(FLAG_BYTE_FALL_COLUMN, i);
  GameFlag_SetByte(FLAG_BYTE_FALL_ROW, FIELD_CELL(z));
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_SetSpriteFlags(Actor_Get(gMainRoutineWork.leaderActorId), 0);
  leader->update = NULL;
  FieldActor_ShowEmote(gMainRoutineWork.leaderActorId,
                       EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_SelectAnimation(leader, ANIM_LEADER_WALK_BACKWARDS);
  Actor_SetAnimationSpeed(leader, ANIM_STEP_NORMAL * 3);
  System_Wait(30);
  leader->facing = FACING_SOUTH;
  leader->motionFlags = 0;
  if ((cell->attributeB & MAP_CELL_FLOOR_HOLE) == 0) {
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
  for (i = 0; i < SINK_FRAMES; i++) {
    if (i == 5)
      Event_PlaySound(SE_DUST_PUFF);
    leader->scaleX -= FIXED_ONE / 64;
    leader->scaleY -= FIXED_ONE * 3 / 256;
    leader->y.fixed -= toFIXED(6) / 10;
    System_Wait(1);
  }
  Actor_Get(gMainRoutineWork.leaderActorId)->spriteStorageKind = 0;
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(
      MARS_LIGHTHOUSE_ENTRANCE_SOL_BLADE_CHEST_TO_MARS_LIGHTHOUSE_STAR_ROOM_NORTHWEST_B);
}

/// Felix walks onto one of the pillar tops of the Sol Blade room's jumping
/// puzzle (cells (13, 28), (7, 30), (11, 34), (8, 36) and (14, 37)): he hops
/// on across the gap the way he is walking where there is somewhere to land,
/// and otherwise slides on as on ice.
void MarsLighthouseEntrance_JumpOrSlide(void) {
  Field_TryJumpForward();
  Leader_SlideOnIceAhead();
}

/// Felix steps on one of the pale cracked tiles among the pillar tops of the
/// Sol Blade room (on its upper floor, cells (8, 29) to (14, 38)): those that
/// give way drop him through to the room below, as
/// MarsLighthouseEntrance_FallThroughCrack shows; the others hold.
void MarsLighthouseEntrance_CheckCrackedFloor(void) {
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  struct MainRoutineWork *party = &gMainRoutineWork;
  struct Actor *actor;
  s32 x;
  s32 z;
  struct MapCell *cell;

  actor = Actor_Get(party->leaderActorId);
  x = FIELD_CELL(actor->x.fixed);
  z = FIELD_CELL(actor->z.fixed);
  cell = gMapGridCellBuffer;
  if (map != NULL)
    cell = map->layers[actor->layer & 3].cells;
  cell += x + (z << 7);
  if (cell->attributeB & MAP_CELL_FLOOR_HOLE)
    MarsLighthouseEntrance_FallThroughCrack(cell + MAPGRID_SIZE);
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

/// The dragon face at the west end of the long hall breathes fire for a
/// second and stops for four, in turn.
void MarsLighthouseEntrance_PulseDragonFire(void) {
  s16 *state = &gMarsLighthouseEntrance_DragonFireOn;

  if (gMarsLighthouseEntrance_DragonFireTimer < 60) {
    if (*state == 0) {
      Ride_SetEntry(0, FLAME_FORWARD | 3);
      *state = 1;
    }
  } else {
    if (((struct HalfwordField *)state)->value == 1) {
      Ride_SetEntry(0, FLAME_THROWER_DRY | 3);
      *state = 0;
    }
  }
  if (++gMarsLighthouseEntrance_DragonFireTimer > 300)
    gMarsLighthouseEntrance_DragonFireTimer = 0;
}

/// Each frame the waking dragon face flickers: tinted red for two frames in
/// eight.
void MarsLighthouseEntrance_FlickerDragonFace(struct Actor *actor) {
  switch (gFrameCount & 7) {
  case 0:
    Actor_SetSpriteColorMode(actor, 0);
    break;
  case 6:
    Actor_SetSpriteColorMode(actor, SPRITE_COLOR_RED_TINT);
    break;
  }
}

/// An ice cone the dragon face's fireball reaches spins and flies off east,
/// shrinking.
void MarsLighthouseEntrance_FlingIceCone(struct Actor *actor) {
  struct FieldSprite *sprite = actor->sprite;

  sprite->rotation += TRIG_TURN / 32;
  if (++actor->timer == 8)
    actor->velocityY = toFIXED(4);
  if (actor->timer > 9) {
    actor->x.fixed += toFIXED(4);
    actor->scaleX -= FIXED_ONE / 32;
    actor->scaleY -= FIXED_ONE / 32;
  }
  if (actor->timer == 32) {
    actor->x.fixed = 0;
    actor->z.fixed = 0;
    actor->update = NULL;
  }
}

/// An ice cone waits until the dragon face's fireball comes within 32 pixels
/// across, then is flung away.
void MarsLighthouseEntrance_WaitForDragonFace(struct Actor *actor) {
  if (ABS(actor->x.fixed - actor->watchedActor->x.fixed) > toFIXED(32))
    return;
  actor->timer = 0;
  actor->update = (ObjectUpdate)MarsLighthouseEntrance_FlingIceCone;
}

/// Each frame, while a fireball carries Felix, his slide on the ice is
/// stopped.
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
extern u16 gTeleportCircle_PaletteSlots[];
extern u16 gTeleportCircle_Colors[];
extern u16 gTeleportCircle_ShimmerColors[];
extern u16 gTeleportCircle_ShimmerSlots[];

s32 Scene_Initialize(void) {
  s32 scene = gMainRoutineWork.next;
  s32 entrance = gMainRoutineWork.entrance;
  struct Actor *actor;
  s32 i;

  gWhirlwindRiding = 0;
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
    Actor_Freeze(ACTOR_ICE_SHAFT);
    if (Event_GetFlag(FLAG_MARS_LIGHTHOUSE_THAWED)) {
      MapGrid_CopyCellsLayered(35, 41, 24, 10, 3, 53);
      MapGrid_CopyCellsLayered(35, 105, 24, 10, 3, 117);
    }
    MarsLighthouseEntrance_PlaceIceCones(
        (s32)gMarsLighthouseEntrance_FugueIceCones);
    if (Event_GetFlag(FLAG_ICE_SHAFT_GROUND))
      MapGrid_CopyCellsLayered(50, 125, 3, 3, 39, 85);
  } else if (scene == SCENE_MARS_LIGHTHOUSE_ENTRANCE_TELEPORT_LAPIS) {
    FieldEffect_StartBlankOverlay(0);
    FlameThrowers_Start((s32)gMarsLighthouseEntrance_LapisBeamTargets,
                        (s32)gMarsLighthouseEntrance_LapisBeamStops, 0,
                        STEP_TRIGGER_FLAME_HIT);
    MarsLighthouseEntrance_PlaceIceCones(
        (s32)gMarsLighthouseEntrance_LapisIceCones);
    if (Event_GetFlag(FLAG_LAPIS_SHEET_BURST)) {
      MapGrid_CopyCellsLayered(47, 94, 10, 6, 47, 100);
      MapGrid_CopyCellAttributes(110, 40, 12, 4, 46, 40);
      Actor_SetPosition(ACTOR_ICE_SHEET, 0, 0);
    } else if (Event_GetFlag(FLAG_ICE_SHAFT_GROUND)) {
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
    if (entrance ==
        MARS_LIGHTHOUSE_ENTRANCE_TELEPORT_LAPIS_FROM_MARS_LIGHTHOUSE_ENTRANCE_FUGUE_EAST) {
      Event_Start();
      FieldEvent_ResolvePendingActions(0);
      FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
      MapGrid_SetShake(0, 2, 1);
      Actor_SetPosition(gMainRoutineWork.leaderActorId, 0, 0);
      Actor_SetPosition(ACTOR_ICE_SHEET, 0, 0);
      Actor_Get(ACTOR_ICE_SHEET)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
      Event_StartScreenTransition();
      Event_WaitTransitionFrames();
      actor = Actor_Get(ACTOR_ICE_SHEET);
      Actor_SetPosition(ACTOR_ICE_SHEET, actor->timer, actor->userValue);
      Event_PlaySound(SE_ICE_CRACK);
      for (i = 0; i < 3; i++) {
        MarsLighthouseEntrance_ScatterIceChips(ACTOR_ICE_SHEET);
        System_Wait(1);
      }
      Event_SetFlag(FLAG_ICE_SHAFT_GROUND);
      MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
      FieldEvent_Wait(120);
      FieldEvent_SetExitRequest(
          MARS_LIGHTHOUSE_ENTRANCE_TELEPORT_LAPIS_TO_MARS_LIGHTHOUSE_ENTRANCE_FUGUE_EAST);
      Event_SetFlag(FLAG_KEEP_PARTY_POSITION);
    }
  } else if (scene == SCENE_MARS_LIGHTHOUSE_ENTRANCE_BALM) {
    FieldEffect_StartBlankOverlay(0);
    if (entrance == BALM_EAST_HALL_NORTH_ENTRANCE ||
        entrance ==
            MARS_LIGHTHOUSE_ENTRANCE_BALM_FROM_MARS_LIGHTHOUSE_ENTRANCE_HIDDEN_CIRCLE_SOUTH) {
      FlameThrowers_Start((s32)gMarsLighthouseEntrance_BalmBeamTargets,
                          (s32)gMarsLighthouseEntrance_BalmBeamStops,
                          (s32)MarsLighthouseEntrance_ShootBalmFireball,
                          STEP_TRIGGER_FLAME_HIT);
      Ride_SetEntryCount(0, 1);
    }
    MarsLighthouseEntrance_PlaceIceCones(
        (s32)gMarsLighthouseEntrance_BalmIceCones);
  } else if (scene == SCENE_MARS_LIGHTHOUSE_ENTRANCE_HIDDEN_CIRCLE) {
    FieldEffect_StartBlankOverlay(0);
    Actor_Get(ACTOR_HIDDEN_CIRCLE_ICE_SHEET)->priorityFlags |=
        ACTOR_PRIORITY_UNDERFOOT;
    if (Event_GetFlag(FLAG_HIDDEN_CIRCLE_SHEET_BURST)) {
      MapGrid_CopyCellsLayered(12, 95, 8, 6, 12, 101);
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
    if (entrance == VALKYRIE_SOUTHWEST_FROM_SOUTHEAST ||
        entrance == VALKYRIE_SOUTHWEST_FROM_MIDDLE_EAST ||
        entrance ==
            MARS_LIGHTHOUSE_ENTRANCE_VALKYRIE_MAIL_CHEST_FROM_MARS_LIGHTHOUSE_STAR_ROOM_SOUTHEAST ||
        entrance == VALKYRIE_SOUTH_CIRCLE) {
      FlameThrowers_Start(
          (s32)gMarsLighthouseEntrance_ValkyrieUpperBeamTargets,
          (s32)gMarsLighthouseEntrance_ValkyrieUpperBeamStops,
          (s32)MarsLighthouseEntrance_ShootValkyrieUpperFireball,
          STEP_TRIGGER_FLAME_HIT);
      Ride_SetEntryCount(0, 1);
    }
    if (entrance ==
            MARS_LIGHTHOUSE_ENTRANCE_VALKYRIE_MAIL_CHEST_FROM_MARS_LIGHTHOUSE_ENTRANCE_BALM ||
        entrance ==
            MARS_LIGHTHOUSE_ENTRANCE_VALKYRIE_MAIL_CHEST_FROM_MARS_LIGHTHOUSE_STAR_ROOM_NORTHWEST ||
        entrance == VALKYRIE_NORTH_FROM_MIDDLE_WEST) {
      struct Actor *target;

      FlameThrowers_Start(
          (s32)gMarsLighthouseEntrance_ValkyrieLowerBeamTargets,
          (s32)gMarsLighthouseEntrance_ValkyrieLowerBeamStops,
          (s32)MarsLighthouseEntrance_ShootValkyrieLowerFireball,
          STEP_TRIGGER_FLAME_HIT);
      Ride_SetEntryCount(0, 1);
      if (entrance == VALKYRIE_NORTH_FROM_MIDDLE_WEST) {
        Actor_SetScript(ACTOR_DRAGON_FACE,
                        gMarsLighthouseEntrance_DragonFaceAtLeft);
        Ride_SetEntry(0, FLAME_FORWARD | 3);
        Task_AddFrame(MarsLighthouseEntrance_PulseDragonFire, TASK_PASS_FRAME);
        Event_SetFlag(FLAG_NO_PLACE_TO_SAVE);
      }
      Task_AddFrame(MarsLighthouseEntrance_StopSlideWhileRiding,
                    TASK_PASS_FRAME);
      Actor_Get(ACTOR_DRAGON_FACE)->update =
          (ObjectUpdate)MarsLighthouseEntrance_FlickerDragonFace;
      Ride_SetMode(1);
      MarsLighthouseEntrance_PlaceIceCones(
          (s32)gMarsLighthouseEntrance_ValkyrieIceCones);
      target = Actor_Get(ACTOR_VALKYRIE_FIREBALL);
      Actor_Get(ACTOR_VALKYRIE_ICE_CONE_A)->update =
          (ObjectUpdate)MarsLighthouseEntrance_WaitForDragonFace;
      Actor_Get(ACTOR_VALKYRIE_ICE_CONE_B)->update =
          (ObjectUpdate)MarsLighthouseEntrance_WaitForDragonFace;
      Actor_Get(ACTOR_VALKYRIE_ICE_CONE_C)->update =
          (ObjectUpdate)MarsLighthouseEntrance_WaitForDragonFace;
      Actor_Get(ACTOR_VALKYRIE_ICE_CONE_D)->update =
          (ObjectUpdate)MarsLighthouseEntrance_WaitForDragonFace;
      Actor_Get(ACTOR_VALKYRIE_ICE_CONE_E)->update =
          (ObjectUpdate)MarsLighthouseEntrance_WaitForDragonFace;
      Actor_Get(ACTOR_VALKYRIE_ICE_CONE_F)->update =
          (ObjectUpdate)MarsLighthouseEntrance_WaitForDragonFace;
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
      entrance == VALKYRIE_SOUTH_CIRCLE)
    Event_ClearFlag(FLAG_FIELD_HANDLING_REQUESTS);
  Palette_SetColorsFade(
      (s32)gTeleportCircle_PaletteSlots, (s32)gTeleportCircle_Colors,
      (s32)gTeleportCircle_ShimmerColors, (s32)gTeleportCircle_ShimmerSlots);
  return 0;
}

extern s32 gMarsLighthouseEntrance_ValkyrieUpperFireballPath[];
extern s32 gMarsLighthouseEntrance_ValkyrieLowerFireballPath[];
extern s32 gMarsLighthouseEntrance_BalmFireballPath[];
extern s32 gMarsLighthouseEntrance_FireballToHiddenCircleIceSheet[];
extern s32 gMarsLighthouseEntrance_FireballThroughIce[];
extern s32 gMarsLighthouseEntrance_FireballToLapisIceSheet[];

s32 Scene_PrepareMap(void) {
  if (Flag_IsSet(FLAG_MARS_LIGHTHOUSE_THAWED))
    Map_CopyPalette(gMainRoutineWork.mapId +
                        (SCENE_MARS_LIGHTHOUSE_JUPITER_LEDGE - SCENE_PROX),
                    (s32)BG_PLTT);
  return 0;
}

/// The flames of the Valkyrie Mail rooms' dragon head reach the urn torch
/// below it, and its fireball sets off east.
void MarsLighthouseEntrance_ShootValkyrieUpperFireball(void) {
  Actor_Attach(ACTOR_UPPER_TORCH, ACTOR_VALKYRIE_SPARKLE,
               ACTOR_VALKYRIE_FIREBALL, FACING_EAST,
               gMarsLighthouseEntrance_ValkyrieUpperFireballPath);
}

/// The flames reach the urn torch at the west end of the long hall, and its
/// fireball sets off east along the hall.
void MarsLighthouseEntrance_ShootValkyrieLowerFireball(void) {
  Actor_Attach(ACTOR_LOWER_TORCH, ACTOR_VALKYRIE_SPARKLE,
               ACTOR_VALKYRIE_FIREBALL, FACING_EAST,
               gMarsLighthouseEntrance_ValkyrieLowerFireballPath);
}

/// The dragon head's flames reach the urn torch at the head of the corridor
/// of ice cones in Balm's rooms, and its fireball sets off south down it.
void MarsLighthouseEntrance_ShootBalmFireball(void) {
  Actor_Attach(ACTOR_BALM_TORCH, ACTOR_BALM_SPARKLE, ACTOR_BALM_FIREBALL,
               FACING_SOUTH, gMarsLighthouseEntrance_BalmFireballPath);
}

/// The west urn torch's fireball sets off east for the sheet of ice.
void MarsLighthouseEntrance_ShootFireballAtHiddenCircleIceSheet(void) {
  Actor_Attach(ACTOR_WEST_TORCH, ACTOR_HIDDEN_CIRCLE_SPARKLE,
               ACTOR_HIDDEN_CIRCLE_FIREBALL, FACING_EAST,
               gMarsLighthouseEntrance_FireballToHiddenCircleIceSheet);
}

/// The urn torch under the dragon face sends its fireball south through the
/// ice.
void MarsLighthouseEntrance_ShootFireballThroughIce(void) {
  Actor_Attach(ACTOR_EAST_TORCH, ACTOR_HIDDEN_CIRCLE_SPARKLE,
               ACTOR_HIDDEN_CIRCLE_FIREBALL, FACING_SOUTH,
               gMarsLighthouseEntrance_FireballThroughIce);
}

/// The main hall's west torch sends its fireball east for the big sheet of
/// ice.
void MarsLighthouseEntrance_ShootFireballAtLapisIceSheet(void) {
  Actor_Attach(ACTOR_LAPIS_TORCH_A, ACTOR_LAPIS_SPARKLE, ACTOR_LAPIS_FIREBALL,
               FACING_EAST, gMarsLighthouseEntrance_FireballToLapisIceSheet);
}

void Ride_UpdateDrift(struct Actor *actor);

#include "shared/actor_is_touching.h"

#include "shared/actor_is_beside.h"

#include "shared/ride_update_carried.h"

extern s32 gScriptedPart_Script[];

void Ride_UpdateDrift(struct Actor *actor);

#include "shared/actor_reset_scripted_for.h"

#include "shared/ride_spawn_forward.c"

#include "shared/ride_update_landing.h"

extern s32 gScriptedPart_Script[];

void Ride_UpdateDrift(struct Actor *actor);

#include "shared/actor_setup_scripted_part.h"

#include "shared/ride_spawn_backward.c"

#include "shared/ride_spawn_landing_forward.c"

#include "shared/ride_spawn_landing_backward.c"

#include "shared/actor_is_above.h"

#include "shared/ride_update_drift.h"

#include "shared/actor_setup_scripted_for.h"

#include "shared/ride_spawn_spark.c"

typedef struct Actor Actor;

#include "shared/flame_throwers_puff.c"

#include "shared/ride_create.c"

/// Releases the kept flame.
UNUSED void Ride_Release(void) {
  struct FlameHitWork *work = RIDE_WORK;

  Actor_Release(work->handle);
  work->handle = NULL;
}

/// Sets whether the flame throwers keep puffing while Psynergy is used.
void Ride_SetMode(s16 value) { RIDE_WORK->runsDuringPsynergy = value; }

void FlameThrowers_Puff(void);

#include "shared/beam_start.h"

#include "shared/ride_get_entry_value.c"

#include "shared/ride_set_entry_value.c"

/// The flame throwers' counter, which Ride_AdvanceCounter steps.
UNUSED s32 Ride_GetCounter(void) { return RIDE_WORK->counter; }

#include "shared/ride_advance_counter.h"

#include "shared/ride_set_entry.h"

#include "shared/ride_set_entry_count.c"

/// Flame thrower `index`'s flags: which flame it sends and how far.
UNUSED s32 Ride_GetEntryFlags(s32 index) {
  struct FlameHitWork *work = RIDE_WORK;

  return (work->entries + index)->flags;
}

#include "shared/ride_set_slot_actor.h"

#include "shared/party_take_fall_damage.h"

#include "../field/field_effect_advance_spin.c"

#include "shared/actor_blink.h"

s32 MapGrid_CheckCollision(struct Actor *actor, struct Vector3 *position);
void Actor_SetSpriteColorMode(struct Actor *actor, s32 a);
extern u8 gInventorySnapshot[];

#include "shared/leader_knock_back.c"

/// Hit by the flames, Felix is knocked back.
void MarsLighthouseEntrance_KnockBackIfHit(void) {
  if (RIDE_WORK->active)
    Leader_KnockBack();
}

/// A rolling fireball hits Felix in Balm's corridor of ice cones or the
/// Valkyrie Mail rooms: the party takes damage, and he sits blinking on the
/// fireball and is carried along with it until it stops (FlameHitWork.car),
/// kept to a cell's centre over rounded ground.
int MarsLighthouseEntrance_RideFireball(void) {
  struct FieldWork *event = Ram_HeapSlots->fieldWork;
  struct FlameHitWork *ride;
  struct Actor *leader;
  /* LEFTOVER(frame): an unused local the ROM's frame reserves and never
   * touches. */
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
  // UB: the fall damage takes no arguments and returns nothing; it is passed
  // the arguments it gives Party_AddHp.
  ((s32 (*)())Party_TakeFallDamage)(-FALL_DAMAGE, 0);
#endif
  FieldCamera_SetSpeed(7);
  leader = Actor_Get(gMainRoutineWork.leaderActorId);
  ride = RIDE_WORK;
  Flag_Set(FLAG_MENU_OPEN);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Flag_Clear(FLAG_MENU_OPEN);
  leader->update = (ObjectUpdate)Actor_Blink;
  leader->facing = FACING_SOUTH;
  Actor_SelectAnimation(leader, ANIM_LEADER_SIT);
  while (ride->car->timer == 0) {
    if (MapGrid_GetCellAttributeLowNibbleOrBump(
            0, leader->x.fixed, leader->z.fixed) != TERRAIN_ROUNDED_SQUARE) {
      leader->x.fixed += ride->car->stepX;
      leader->z.fixed += ride->car->stepZ;
    } else {
      if (ride->car->stepX != 0)
        leader->x.fixed = (leader->x.fixed & FIELD_CELL_MASK) + toFIXED(8);
      if (ride->car->stepZ != 0)
        leader->z.fixed = (leader->z.fixed & FIELD_CELL_MASK) + toFIXED(8);
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

/// Stepping onto the south circle of the Valkyrie Mail rooms while a fireball
/// has hit him, Felix is carried along with it
/// (MarsLighthouseEntrance_RideFireball); otherwise nothing happens. The
/// slide from the ice's edge asks the same, ignoring the answer.
s32 MarsLighthouseEntrance_SlideIfRiding(void) {
  if (RIDE_WORK->active) {
    MarsLighthouseEntrance_RideFireball();
    return 1;
  }
  return 0;
}

/// Whether a flame or fireball has hit Felix.
s32 Ride_IsActive(void) { return RIDE_WORK->active; }

extern u32 gCrumbs_Script[];

#include "shared/actor_scatter_crumbs.c"

extern u32 gEmbers_Script[];

#include "shared/embers_spawn.h"

#include "shared/actor_hold_during_event.c"

#include "shared/actor_roll_and_crumble.c"

void Map_CopyPalette(s32 scene, s32 a1);

/// Sends a sparkle from a torch along its path, at whose end it sets a
/// fireball rolling toward `facing` (Actor_Park). Its callers take it as
/// returning a word; it returns none.
s32 Actor_Attach(s32 torchId, s32 sparkleId, s32 fireballId, s32 facing,
                 const void *path) {
  struct Actor *source = Actor_Get(torchId);
  struct Actor *actor = Actor_Get(sparkleId);

  FieldActor_SetPositionAndResetMotion(actor, source->x.fixed, source->y.fixed,
                                       source->z.fixed);
  Actor_StartScript(actor, path);
  Actor_SetSpriteFlags(actor, 0);
  actor->motionFlags = 0;
  Actor_SelectAnimation(actor, 1);
  actor->watchedActor = source;
  actor->timer = 0;
  actor->userValue = facing;
  actor->launchedActorId = fireballId;
  actor->update = (ObjectUpdate)Actor_HoldDuringEvent;
  // UB: no value is returned, though the callers take one.
#ifdef UBFIX
  return 0;
#endif
}

#include "shared/actor_launch_ahead.h"

void Actor_SetSpritePriorityDirect(struct Actor *actor, s32 mode);

#include "shared/actor_pop_up_slow.h"

#include "shared/actor_pop_up.h"

#include "shared/actor_park.h"

extern s16 gSlideOnIce_SteerAngles[];

#include "shared/actor_steer_by_pad.h"

#include "shared/leader_stop_if_pending.h"

s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
extern s16 gSlideOnIce_AnglesByKey[];

#include "shared/leader_slide_on_ice.h"

void Actor_SteerByPad(struct Actor *actor);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);

#include "shared/slide_on_ice_from_edge.c"

/// Stops the leader's slide on the ice where it is (nonzero), or lets it run.
void Leader_SetSlideStopped(s32 value) { gSlideOnIce_Stopped = value; }

#include "shared/leader_slide_on_ice_ahead.h"

#include "../field/actor_set_sprite_priority_b.c"

#include "../field/field_effect_move.c"

#undef DEFINE_BURST_ICE_SHEET
extern s32 gFieldEffect_DurationScripts[];

void Actor_SetSpriteFlags(struct Actor *actor, u32 value);

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

  if ((flags & EFFECT_OPTION_MASK) == 0 || options == 0)
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
    FIXED target;
    s32 frames;

    script = gFieldEffect_DurationScripts[flags & EFFECT_SCRIPT_MASK];
    target = options->targetScaleX;
    frames = script[3];
    if ((flags & EFFECT_USE_START_SCALE) != 0) {
      effect->scaleRateX = (target - effect->scaleX) / frames;
      effect->scaleRateY = (options->targetScaleY - effect->scaleY) / script[3];
    } else {
      effect->scaleRateX = (target - FIXED_ONE) / frames;
      effect->scaleRateY = (options->targetScaleY - FIXED_ONE) / script[3];
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

#include "shared/cracked_tile_find.h"

#include "shared/leader_center_in_cell.h"

#include "shared/cracked_tile_crack.h"

/// A cracked tile gives way under Felix, who drops through it.
void CrackedTile_Fall(struct CellTarget *table, struct CellTargetEntry *entry) {
  struct Actor *actor = Actor_Get(gMainRoutineWork.leaderActorId);
  s32 x;
  s32 z;
  s32 mask;

  mask = MAPGRID_SIZE / 2;
  x = FIELD_CELL(actor->x.fixed);
  x |= (s16)(table->x & mask);
  z = FIELD_CELL(actor->z.fixed);
  z |= (s16)(table->z & mask);
  Leader_CenterInCell();
  Event_PlaySound(SE_SPLASH);
  MapGrid_CopyCells(table->x, table->z + 2, 1, 1, x, z);
  FieldEvent_Wait(12);
  {

    actor->facing = FACING_SOUTH;
    actor->motionFlags = 0;
  }
  Actor_SetSpriteFlags(Actor_Get(gMainRoutineWork.leaderActorId), 0);
  {
    u32 winout = WINOUT_WIN01_BG0 | WINOUT_WIN01_BG1 | WINOUT_WIN01_BG2 |
                 WINOUT_WIN01_BG3 | WINOUT_WIN01_OBJ | WINOUT_WINOBJ_BG0 |
                 WINOUT_WINOBJ_BG1 | WINOUT_WINOBJ_BG2 | WINOUT_WINOBJ_BG3;

    REG_WINOUT = winout;
  }
  REG_DISPCNT |= DISPCNT_OBJWIN_ON;
  table->shown = 1;
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_LEADER_ARMS_RAISED);
  FieldEvent_Wait(16);
  for (x = 0; x < SINK_FRAMES; x++) {
    if (x == 5)
      Event_PlaySound(SE_DUST_PUFF);
    actor->scaleX -= FIXED_ONE / 64;
    actor->scaleY -= FIXED_ONE * 3 / 256;
    actor->y.fixed -= toFIXED(6) / 10;
    System_Wait(1);
  }
  Actor_Get(gMainRoutineWork.leaderActorId)->spriteStorageKind = 0;
  table->shown = 0;
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
}

#include "../field/field_effect_advance_motion.c"

#undef CELL_PUSH_SLOT_ENTRY
void Teleport_PlayWhirl(void);

#include "shared/cracked_tile_drop_in.h"

void GameFlag_SetByte(s32 flag, s32 value);
#include "shared/cracked_tile_step.h"

#include "shared/cracked_tile_place_landing.h"

#include "shared/teleport_shimmer.c"

#include "shared/actors_set_cells_open.h"

#include "shared/pound_sink_target.h"

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
      SC_SET(SW_Y, toFIXED(48)),                                                
      SC_SET(SW_GROUND, toFIXED(48)),                                           
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
    SC_SET(SW_Y, toFIXED(48)),
    SC_SET(SW_GROUND, toFIXED(48)),
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
      SC_SET(SW_Y, toFIXED(32)),                                                
      SC_SET(SW_GROUND, toFIXED(32)),                                           
      SC_SPEED(0.5),                                                
      SC_ACCELERATION(0.03125),                                                 
      SC_END,                                                                  
  };

s32 gMarsLighthouseEntrance_DragonFaceGoesLeft[] = {
    SC_SET(SW_STEERING, 0),
    SC_SET(SW_MOTION, 0),
    SC_SET(SW_Y, toFIXED(32)),
    SC_SET(SW_GROUND, toFIXED(32)),
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
    SC_CALL(MarsLighthouseEntrance_DryDragonFace),
    SC_END,
};

s32 gMarsLighthouseEntrance_DragonFaceAtLeft[] = {
    SC_SET(SW_STEERING, 0),
    SC_SET(SW_MOTION, 0),
    SC_SET(SW_X, toFIXED(56)),
    SC_SET(SW_Z, toFIXED(128)),
    SC_SET(SW_Y, toFIXED(48)),
    SC_SET(SW_GROUND, toFIXED(48)),
    SC_SPEED(0.5),
    SC_ACCELERATION(0.03125),
    SC_MOVE(56, 48, 128),
    SC_SYNC,
    SC_WAIT(20),
    SC_END,
};

#include "shared/debris_script.c"

/// Each room's ice cones (MarsLighthouseEntrance_PlaceIceCones), and its flame
/// throwers for FlameThrowers_Start, each with its flame's kind and reach, with
/// the actors their flames stop at, 1 where reaching one calls the routine
/// FlameThrowers_Start is given.
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
    SC_CALL(Actor_PopUpSlow),
    SC_MOVE(80, 32, 168),
    SC_SYNC,
    SC_MOVE(80, 22, 168),
    SC_SYNC,
    SC_MOVE(96, 22, 168),
    SC_SYNC,
    SC_SET(SW_WATCHED_ACTOR, (s32)MarsLighthouseEntrance_ClearFireballRow),
    SC_CALL(Actor_Park),
    SC_END,
};

s32 gMarsLighthouseEntrance_BalmFireballPath[] = {                                                               
      SC_CALL(Actor_PopUpSlow), SC_MOVE(872, 32, 576),             
      SC_SYNC,        SC_MOVE(872, 30, 576),             
      SC_SYNC,        SC_MOVE(872, 30, 584),             
      SC_SYNC,        SC_CALL(Actor_Park),                                          
      SC_END,                                                                  
  };

s32 gMarsLighthouseEntrance_FireballToHiddenCircleIceSheet[] = {
    SC_CALL(Actor_PopUp),
    SC_MOVE(96, 32, 680),
    SC_SYNC,
    SC_MOVE(96, 22, 680),
    SC_SYNC,
    SC_MOVE(112, 22, 680),
    SC_SYNC,
    SC_SET(SW_WATCHED_ACTOR, (s32)MarsLighthouseEntrance_CrackHiddenCircleIceSheet),
    SC_CALL(Actor_Park),
    SC_END,
};

s32 gMarsLighthouseEntrance_FireballThroughIce[] = {
    SC_CALL(Actor_PopUp),
    SC_MOVE(856, 32, 136),
    SC_SYNC,
    SC_MOVE(872, 32, 136),
    SC_SYNC,
    SC_MOVE(872, 22, 136),
    SC_SYNC,
    SC_MOVE(872, 22, 152),
    SC_SYNC,
    SC_SET(SW_WATCHED_ACTOR, (s32)MarsLighthouseEntrance_MeltIceInPath),
    SC_CALL(Actor_Park),
    SC_END,
};

s32 gMarsLighthouseEntrance_FireballToLapisIceSheet[] = {
    SC_CALL(Actor_PopUp),
    SC_MOVE(112, 32, 680),
    SC_SYNC,
    SC_MOVE(112, 22, 680),
    SC_SYNC,
    SC_MOVE(128, 22, 680),
    SC_SYNC,
    SC_SET(SW_WATCHED_ACTOR, (s32)MarsLighthouseEntrance_CrackLapisIceSheet),
    SC_CALL(Actor_Park),
    SC_END,
};

#include "shared/mars_lighthouse_part_scripts.c"

#include "shared/slide_on_ice_angles.c"

#include "shared/field_effect_duration_scripts.c"

#define TELEPORT_CIRCLE_FIRST_ENTRY 137
#include "shared/teleport_circle_palette.c"

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
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, 800, MOVEMENT_TYPE_STAND, 648, 0, 376,
                    FACING_SOUTH, TALK_FACING_KEEP, 0),
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

// clang-format off
struct ActorPlacement gMarsLighthouseEntrance_LapisPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_DRAGON_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 56, 0, 120, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_DRAGON_HEAD, CONDITION_ALWAYS, gMarsLighthouseEntrance_LapisWestFlameThrowerRoute, 104, 0, 112, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_DRAGON_HEAD, CONDITION_ALWAYS, gMarsLighthouseEntrance_LapisEastFlameThrowerRoute, 280, 0, 112, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_DRAGON_HEAD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 112, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_DRAGON_HEAD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 112, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MARS_TORCH, CONDITION_ALWAYS, (s32)gMarsLighthouseEntrance_StatueScript, 88, 0, 680, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MARS_TORCH, CONDITION_ALWAYS, (s32)gMarsLighthouseEntrance_StatueScript, 552, 0, 680, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MARS_TORCH_LIT, CONDITION_ALWAYS, (s32)gMarsLighthouseEntrance_StatueScript, 88, 0, 728, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, gMarsLighthouseEntrance_IceConeScript, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, gMarsLighthouseEntrance_IceConeScript, 552, 0, 776, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ICE_CRACK, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 840, 0, 672, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MARS_TORCH, CONDITION_ALWAYS, (s32)gMarsLighthouseEntrance_StatueScript, 88, 0, 728, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ICE_CRACK, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 480, 0, 704, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPARKLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIREBALL, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};
// clang-format on

// clang-format off
struct ActorPlacement gMarsLighthouseEntrance_BalmPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_DRAGON_HEAD, CONDITION_ALWAYS, gMarsLighthouseEntrance_BalmFlameThrowerRoute, 840, 0, 528, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MARS_TORCH, CONDITION_ALWAYS, (s32)gMarsLighthouseEntrance_StatueScript, 872, 0, 568, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_DRAGON_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 792, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_DRAGON_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 792, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, gMarsLighthouseEntrance_IceConeScript, 840, 0, 680, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, gMarsLighthouseEntrance_IceConeScript, 904, 0, 744, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, gMarsLighthouseEntrance_IceConeScript, 840, 0, 808, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, gMarsLighthouseEntrance_IceConeScript, 936, 0, 824, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, gMarsLighthouseEntrance_IceConeScript, 904, 0, 872, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_MERCURY_DJINNI, FLAG_DJINNI(DJINNI_BALM), MOVEMENT_TYPE_STAND, 88, 0, 904, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SPARKLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPARKLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIREBALL, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIREBALL, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};
// clang-format on

// clang-format off
struct ActorPlacement gMarsLighthouseEntrance_HiddenCirclePlacements[] = {
    ACTOR_PLACEMENT(SPRITE_MARS_TORCH_LIT, CONDITION_ALWAYS, (s32)gMarsLighthouseEntrance_StatueScript, 72, 0, 632, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MARS_TORCH, CONDITION_ALWAYS, (s32)gMarsLighthouseEntrance_StatueScript, 72, 0, 680, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MARS_TORCH_LIT, CONDITION_ALWAYS, (s32)gMarsLighthouseEntrance_StatueScript, 904, 0, 120, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MARS_TORCH, CONDITION_ALWAYS, (s32)gMarsLighthouseEntrance_StatueScript, 856, 0, 120, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ICE_CRACK, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 240, 0, 688, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPARKLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIREBALL, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};
// clang-format on

// clang-format off
struct ActorPlacement gMarsLighthouseEntrance_ValkyriePlacements[] = {
    ACTOR_PLACEMENT(SPRITE_DRAGON_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 696, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_DRAGON_HEAD, CONDITION_ALWAYS, gMarsLighthouseEntrance_ValkyrieWestFlameThrowerRoute, 120, 0, 688, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MARS_TORCH, CONDITION_ALWAYS, (s32)gMarsLighthouseEntrance_StatueScript, 88, 0, 728, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_DRAGON_HEAD, CONDITION_ALWAYS, gMarsLighthouseEntrance_ValkyrieEastFlameThrowerRoute, 792, 0, 128, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MARS_TORCH, CONDITION_ALWAYS, (s32)gMarsLighthouseEntrance_StatueScript, 56, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, gMarsLighthouseEntrance_IceConeScript, 200, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, gMarsLighthouseEntrance_IceConeScript, 328, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, gMarsLighthouseEntrance_IceConeScript, 360, 0, 152, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, gMarsLighthouseEntrance_IceConeScript, 424, 0, 152, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, gMarsLighthouseEntrance_IceConeScript, 664, 0, 152, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, gMarsLighthouseEntrance_IceConeScript, 760, 0, 152, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPARKLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIREBALL, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
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
    {TRIGGER_EXIT | EXIT_STYLE_HIDDEN_LAYER, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT | EXIT_STYLE_HIDDEN_LAYER, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT | EXIT_STYLE_HIDDEN_LAYER, 9, CONDITION_ALWAYS, 9},
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
    {PSYNERGY_ON_ACTOR(ABILITY_GRIND), ACTOR_ICE_SHAFT, FLAG_ICE_SHAFT_GROUND,
     EVENT(MarsLighthouseEntrance_GrindIce)},
    {TRIGGER_TALK, ACTOR_ICE_SHAFT, FLAG_ICE_SHAFT_GROUND,
     EVENT(MarsLighthouseEntrance_CheckIce)},
    {TRIGGER_TABLE_END},
};

struct Trigger gMarsLighthouseEntrance_LapisTriggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT | EXIT_STYLE_HIDDEN_LAYER, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT | EXIT_STYLE_HIDDEN_LAYER, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT | EXIT_STYLE_HIDDEN_LAYER, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT | EXIT_STYLE_HIDDEN_LAYER, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_EXIT, 13, CONDITION_ALWAYS, 13},
    {TRIGGER_EXIT | EXIT_STYLE_HIDDEN_LAYER, 14, CONDITION_ALWAYS, 14},
    {TRIGGER_EXIT, 15, CONDITION_ALWAYS, 15},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 16, CONDITION_ALWAYS, 16},
    {TRIGGER_STEP, STEP_TRIGGER_FLAME_HIT, CONDITION_ALWAYS,
     EVENT(Leader_KnockBack)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS),
     ACTOR_LAPIS_ICE_CONE_A, FLAG_SCENE_TEMP_02,
     EVENT(MarsLighthouseEntrance_StartPoundIceCone)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS),
     ACTOR_LAPIS_ICE_CONE_B, FLAG_SCENE_TEMP_03,
     EVENT(MarsLighthouseEntrance_StartPoundIceCone)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_TELEPORT, PSYNERGY_PHASE_AFTER_CAST),
     LAPIS_SOUTH_CIRCLE, CONDITION_ALWAYS,
     EVENT(MarsLighthouseEntrance_TeleportFromLapisSouthCircle)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_TELEPORT, PSYNERGY_PHASE_AFTER_CAST),
     LAPIS_NORTH_CIRCLE, CONDITION_ALWAYS,
     EVENT(MarsLighthouseEntrance_TeleportFromLapisNorthCircle)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PUSHED_STATUE,
     FLAG_LAPIS_STATUE_BLOCKS_FLAMES,
     EVENT(MarsLighthouseEntrance_PushStatueIntoFlames)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS,
     EVENT(MarsLighthouseEntrance_PushStatueIntoFlames)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS),
     CELL_BEHAVIOR_CRACKED_ICE_WALL, IF_FLAG_SET(FLAG_LAPIS_SHEET_CRACKED),
     EVENT(MarsLighthouseEntrance_BurstIceSheet)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS),
     CELL_BEHAVIOR_BIG_ICE_SHEET, IF_FLAG_SET(FLAG_LAPIS_SECOND_SHEET_SHOWN),
     EVENT(MarsLighthouseEntrance_BurstLapisIceSheet)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_LIT_TORCH, CONDITION_ALWAYS, 0},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_HITS),
     ACTOR_LAPIS_TORCH_A, CONDITION_ALWAYS,
     EVENT(MarsLighthouseEntrance_BlazeLapisTorch)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_AFTER_END),
     ACTOR_LAPIS_TORCH_A, CONDITION_ALWAYS,
     EVENT(MarsLighthouseEntrance_WatchFireballCrackIce)},
    {TRIGGER_TALK, ACTOR_TELEPORT_LAPIS_CHEST, CONDITION_ALWAYS,
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
    {TRIGGER_EXIT | EXIT_STYLE_HIDDEN_LAYER, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT | EXIT_STYLE_HIDDEN_LAYER, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_EXIT, 13, CONDITION_ALWAYS, 13},
    {TRIGGER_EXIT, 15, CONDITION_ALWAYS, 15},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 16, CONDITION_ALWAYS, 16},
    {TRIGGER_STEP, CELL_BEHAVIOR_BLACK_ICE, CONDITION_ALWAYS,
     EVENT(Leader_SlideOnIce)},
    {TRIGGER_STEP, STEP_TRIGGER_FIREBALL_HIT, CONDITION_ALWAYS,
     EVENT(MarsLighthouseEntrance_RideFireball)},
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
    {PSYNERGY_PHASE_ON_CELL(ABILITY_TELEPORT, PSYNERGY_PHASE_AFTER_CAST),
     MARS_LIGHTHOUSE_ENTRANCE_BALM_TO_MARS_LIGHTHOUSE_ENTRANCE_HIDDEN_CIRCLE_NORTH_B,
     CONDITION_ALWAYS, EVENT(MarsLighthouseEntrance_TeleportFromBalm)},
    {TRIGGER_TABLE_END},
};

struct Trigger gMarsLighthouseEntrance_HiddenCircleTriggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_HIDDEN_LAYER, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_HIDDEN_LAYER, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT | EXIT_STYLE_HIDDEN_LAYER, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT | EXIT_STYLE_HIDDEN_LAYER, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT | EXIT_STYLE_HIDDEN_LAYER, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT | EXIT_STYLE_HIDDEN_LAYER, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_EXIT, 13, CONDITION_ALWAYS, 13},
    {TRIGGER_STEP, CELL_BEHAVIOR_BLACK_ICE, CONDITION_ALWAYS,
     EVENT(Leader_SlideOnIce)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_WEST_LIT_TORCH, CONDITION_ALWAYS, 0},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_HITS),
     ACTOR_WEST_TORCH, CONDITION_ALWAYS,
     EVENT(MarsLighthouseEntrance_BlazeHiddenCircleWestTorch)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_AFTER_END),
     ACTOR_WEST_TORCH, CONDITION_ALWAYS,
     EVENT(MarsLighthouseEntrance_WatchFireballCrackHiddenCircleIce)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_EAST_LIT_TORCH, CONDITION_ALWAYS, 0},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_HITS),
     ACTOR_EAST_TORCH, CONDITION_ALWAYS,
     EVENT(MarsLighthouseEntrance_BlazeHiddenCircleEastTorch)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_AFTER_END),
     ACTOR_EAST_TORCH, CONDITION_ALWAYS,
     EVENT(MarsLighthouseEntrance_WatchFireballMeltIce)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS),
     CELL_BEHAVIOR_CRACKED_ICE_WALL,
     IF_FLAG_SET(FLAG_HIDDEN_CIRCLE_SHEET_SHOWN),
     EVENT(MarsLighthouseEntrance_BurstHiddenCircleIceSheet)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_TELEPORT, PSYNERGY_PHASE_AFTER_CAST),
     MARS_LIGHTHOUSE_ENTRANCE_HIDDEN_CIRCLE_TO_MARS_LIGHTHOUSE_ENTRANCE_BALM_NORTH_B,
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
    {TRIGGER_EXIT | EXIT_STYLE_HIDDEN_LAYER, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT | EXIT_STYLE_HIDDEN_LAYER, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT | EXIT_STYLE_HIDDEN_LAYER, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 14, CONDITION_ALWAYS, 14},
    {TRIGGER_EXIT | EXIT_STYLE_HIDDEN_LAYER, 15, CONDITION_ALWAYS, 15},
    {TRIGGER_STEP,
     MARS_LIGHTHOUSE_ENTRANCE_VALKYRIE_MAIL_CHEST_TO_MARS_LIGHTHOUSE_STAR_ROOM_EAST,
     CONDITION_ALWAYS, EVENT(MarsLighthouseEntrance_FallIntoHole)},
    {TRIGGER_STEP,
     MARS_LIGHTHOUSE_ENTRANCE_VALKYRIE_MAIL_CHEST_TO_MARS_LIGHTHOUSE_STAR_ROOM_WEST,
     CONDITION_ALWAYS, EVENT(MarsLighthouseEntrance_FallIntoHole)},
    {TRIGGER_STEP, CELL_BEHAVIOR_WAKE_DRAGON_FACE, FLAG_DRAGON_FACE_AWAKE,
     EVENT(MarsLighthouseEntrance_WakeDragonFace)},
    {TRIGGER_STEP, CELL_BEHAVIOR_BLACK_ICE, CONDITION_ALWAYS,
     EVENT(MarsLighthouseEntrance_SlideFromEdge)},
    {TRIGGER_STEP, STEP_TRIGGER_FIREBALL_HIT, CONDITION_ALWAYS,
     EVENT(MarsLighthouseEntrance_RideFireball)},
    {TRIGGER_STEP, VALKYRIE_SOUTH_CIRCLE, CONDITION_ALWAYS,
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
    {PSYNERGY_PHASE_ON_CELL(ABILITY_TELEPORT, PSYNERGY_PHASE_AFTER_CAST),
     VALKYRIE_NORTH_CIRCLE, CONDITION_ALWAYS,
     EVENT(MarsLighthouseEntrance_TeleportFromValkyrieNorth)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_TELEPORT, PSYNERGY_PHASE_AFTER_CAST),
     VALKYRIE_SOUTH_CIRCLE, CONDITION_ALWAYS,
     EVENT(MarsLighthouseEntrance_TeleportFromValkyrieSouth)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PUSHED_STATUE,
     FLAG_VALKYRIE_STATUE_BLOCKS_FLAMES,
     EVENT(MarsLighthouseEntrance_PushStatueIntoValkyrieFlames)},
    {TRIGGER_TABLE_END},
};

struct Trigger gMarsLighthouseEntrance_SolBladeTriggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_HIDDEN_LAYER, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT | EXIT_STYLE_HIDDEN_LAYER, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT | EXIT_STYLE_HIDDEN_LAYER, 4, CONDITION_ALWAYS, 4},
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
    {TRIGGER_STEP, CELL_BEHAVIOR_BLACK_ICE, CONDITION_ALWAYS,
     EVENT(Leader_SlideOnIce)},
    {TRIGGER_STEP, CELL_BEHAVIOR_CRACKED_ICE_TILE, CONDITION_ALWAYS,
     EVENT(MarsLighthouseEntrance_CheckCrackedFloor)},
    {STEP_TRIGGER_WALKING, CELL_BEHAVIOR_PILLAR_TOP, CONDITION_ALWAYS,
     EVENT(MarsLighthouseEntrance_JumpOrSlide)},
    {TRIGGER_TABLE_END},
};

/// 1 while the dragon face at the west end of the long hall breathes fire.
u32 gMarsLighthouseEntrance_DragonFireOn[] = {
    0,
};

s32 gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};
