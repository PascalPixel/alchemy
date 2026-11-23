/// @file
/// The Islet Cave, which the lonely turtle on the Sea of Time Islet opens once
/// he has Li'l Turtle for a friend. Its three maps are all here.
///
/// The halls (SCENE_ISLET_CAVE_HALLS): stairs come down from the islet into a
/// small south room, and more stairs lead down from it into the west hall, a
/// lake with an island holding a Teleport circle. Golden logs float in the
/// water and carry Felix across as he walks onto them (IsletCave_RideLog): one
/// from the island to the west shore and the Turtle Boots chest, one across
/// the north of the lake to the ledge of the Rusty Staff chest, four that
/// cross the east hall's long channel in pairs, and a seventh that carries him
/// all the way up that channel to Meld, a Venus Djinni.
///
/// With Teleport, the island's circle sends the party to a second circle on a
/// ledge at the north of the lake, behind which a doorway leads into five
/// corridors (SCENE_ISLET_CAVE_CORRIDORS), each darker and bluer than the last
/// (IsletCave_DarkenPalette). Serac, a Mercury Djinni, hides in the winged
/// statue of the second corridor until Tremor shakes him out. The fifth
/// corridor's door opens on the Sentinel's room (SCENE_ISLET_CAVE_SENTINEL),
/// where the Sentinel guards the Catastrophe tablet.

#include "flags.h"
#include "djinn_ids.h"
#include "field/field.h"
#include "graphics/palette_buffers.h"
#include "system/main_routine_work.h"
#include "types.h"
#include "macros.h"
#include "system/ram_buffer.h"
#include "system/dma.h"
#include "system/io_reg.h"
#include "field/event_inline.h"
#include "field/field_sequence_event.h"
#include "scene/scene.h"
#include "system/task.h"
#include "field/actor.h"
#include "field/field_sprite.h"
#include "field/scene_tables.h"
#include "battle/battle.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "scene_ids.h"
#include "stddef.h"
#include "shared/tables.h"
#include "field/actor_script.h"

void Teleport_ArriveWhirling(void);

s32 List_CountEntries(u16 *list);

void PaletteCycle_Fade(void);

void IsletCave_PlaceLogs(void);

void IsletCave_FightMeld(void);

void IsletCave_FightSerac(void);

void IsletCave_TeleportToNorthCircle(void);

void IsletCave_TeleportToIslandCircle(void);

void IsletCave_OpenDoorAndEnter(void);

void IsletCave_SettleFirstLog(void);

void IsletCave_SettleSecondLog(void);

void IsletCave_SettleThirdLog(void);

void IsletCave_SettleFourthLog(void);

void IsletCave_SettleFifthLog(void);

void IsletCave_SettleSixthLog(void);

void IsletCave_SettleSeventhLog(void);
void IsletCave_RideFirstLogEast(void);
void IsletCave_RideSecondLogEast(void);
void IsletCave_RideSecondLogWest(void);

void IsletCave_RideThirdLogEast(void);

void IsletCave_RideFourthLogEast(void);
void IsletCave_RideFourthLogWest(void);
void IsletCave_RideFifthLogEast(void);
void IsletCave_RideFifthLogWest(void);
void IsletCave_RideSixthLogEast(void);
void IsletCave_RideSixthLogWest(void);
void IsletCave_RideSeventhLogNorth(void);
void IsletCave_RideSeventhLogSouth(void);
void IsletCave_SentinelAwaits(void);

void IsletCave_TremorRevealsSerac(void);

extern u32 gIsletCave_SentinelScript[];

extern s32 gIsletCave_ShrunkFireScript[];
extern s16 gIsletCave_DoorFrames[];

u16 gPaletteCycleBright BSS_DATA;
u16 *gPaletteCycleSlots BSS_DATA;
u16 gPaletteCycleTimer BSS_DATA;
u16 gPaletteCycleDelay BSS_DATA ALIGNED(4);
u16 *gPaletteSlots BSS_DATA;
u16 *gPaletteColors BSS_DATA;
u16 *gPaletteCycleColors BSS_DATA;
u16 gPaletteCycleLength BSS_DATA;
u16 gPaletteCycleIndex BSS_DATA ALIGNED(4);
void Teleport_PlaySparks(void);
void FieldEffect_StartBlankOverlay(s32 mode);
void PaletteFade_PackTarget(void);

#include "shared/teleport_shimmer.c"

#include "text/msg_ids.h"

/// Meld, the Djinni on the ledge at the north end of the east hall.
enum { ACTOR_MELD = 15 };

/// Talking to Meld, the Venus Djinni on the ledge the seventh log carries
/// Felix up to at the north end of the east hall, starts a battle with him.
void IsletCave_FightMeld(void) {
  Field_RequestDjinnBattle(ACTOR_MELD, DJINNI_MELD);
  return;
}

/// Serac, the Djinni hiding in the second corridor, and the winged statue
/// Tremor shakes him out of.
enum { ACTOR_WINGED_STATUE = 8, ACTOR_SERAC = 9 };

/// The Sentinel, and the first of the ten small fires burning in pairs down
/// both sides of its room.
enum { ACTOR_SENTINEL = 8, ACTOR_FIRST_FIRE = 9 };

/// Set once the Sentinel is beaten, as the party comes back to its room.
#define FLAG_SENTINEL_DEFEATED 0x9cf

/// Set once Tremor has shaken Serac out of the winged statue, or on entering
/// once he is caught or has run off to hide: the statue then stays still.
#define FLAG_SERAC_REVEALED FLAG_SCENE_TEMP_30

/// Where the battle with the Sentinel puts the party back in its room: the
/// spot it stood on.
enum { ENTRANCE_AFTER_SENTINEL_BATTLE = 2 };

/// The cells Teleport answers on: the west hall's two circles.
enum { CELL_BEHAVIOR_ISLAND_CIRCLE = 50, CELL_BEHAVIOR_NORTH_CIRCLE = 51 };

/// The stone doors at the north ends of the five corridors: each door's cell
/// behaviour is the exit it leaves by.
enum {
  EXIT_FIRST_CORRIDOR_DOOR = 2,
  EXIT_SECOND_CORRIDOR_DOOR = 4,
  EXIT_THIRD_CORRIDOR_DOOR = 6,
  EXIT_FOURTH_CORRIDOR_DOOR = 8,
  EXIT_FIFTH_CORRIDOR_DOOR = 10
};

/// The ground just before the Sentinel, where walking up to it wakes it.
enum { CELL_BEHAVIOR_BEFORE_SENTINEL = 10 };

/// Talking to Serac, the Mercury Djinni Tremor has shaken out of the winged
/// statue in the second corridor, starts a battle with him.
void IsletCave_FightSerac(void) {
  Field_RequestDjinnBattle(ACTOR_SERAC, DJINNI_SERAC);
  return;
}

/// The last step of each fire's script in the Sentinel's room: clears its
/// sprite flags.
s32 IsletCave_ClearScriptedActorSpriteFlags(struct Actor *actor) {
  Actor_SetSpriteFlags(actor, 0);
  return 0;
}

/// The winged statue's hop: one every STATUE_HOP_FRAMES, its sprite rocking
/// to and fro over the first half, once every STATUE_ROCK_FRAMES, swung by the
/// cosine's full turn shrunk by STATUE_ROCK_SHRINK.
#define STATUE_HOP_FRAMES 64
#define STATUE_ROCK_FRAMES 16
#define STATUE_ROCK_SHRINK 128

/// The winged statue's script while Serac hides in it: it hops, rocking its
/// sprite through the first half of each hop.
s32 IsletCave_HopWingedStatue(struct Actor *actor) {
  s32 step = actor->riseCounter & (STATUE_HOP_FRAMES - 1);
  struct FieldSprite *sprite = actor->sprite;

  actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  if (step < STATUE_HOP_FRAMES / 2) {
    if (step == 0)
      actor->velocityY = toFIXED(1);
    sprite->rotation =
        Trig_Cos(step * (TRIG_TURN / STATUE_ROCK_FRAMES)) / STATUE_ROCK_SHRINK;
  }
  actor->riseCounter++;
  return -1;
}

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern struct ActorPlacement gIsletCave_SentinelRoomPlacements[];
extern struct ActorPlacement gIsletCave_HallsPlacements[];
extern struct ActorPlacement gIsletCave_CorridorsPlacements[];
extern struct Trigger gIsletCave_HallsTriggers[];
extern struct Trigger gIsletCave_CorridorsTriggers[];
extern struct Trigger gIsletCave_SentinelRoomTriggers[];

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  s16 scene = gMainRoutineWork.next;

  if (scene == SCENE_ISLET_CAVE_HALLS)
    return gIsletCave_HallsPlacements;
  if (scene == SCENE_ISLET_CAVE_CORRIDORS)
    return gIsletCave_CorridorsPlacements;
  return gIsletCave_SentinelRoomPlacements;
}

const struct Trigger *Scene_GetTriggers(void) {
  s16 scene = gMainRoutineWork.next;

  if (scene == SCENE_ISLET_CAVE_CORRIDORS)
    return gIsletCave_CorridorsTriggers;
  if (scene == SCENE_ISLET_CAVE_SENTINEL)
    return gIsletCave_SentinelRoomTriggers;
  return gIsletCave_HallsTriggers;
}

/// The end of a Teleport between the two circles: the circle shimmers while
/// the sparks whirl in, then the cave's colours come back and Felix is shown
/// on it.
void IsletCave_FinishTeleport(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  s32 effect = work->transition;

  Teleport_StartFastShimmer();
  Teleport_PlayWhirl();
  wait(40);
  Teleport_StopShimmer();
  PaletteFade_SetTarget(PALETTE_TINT_NONE, 1);
  PaletteFade_StartInterpolation(16);
  wait(16);

  gMainRoutineWork.teleportState = 0;
  Actor_ConfigureSpriteColor(gMainRoutineWork.leaderActorId, 0);
  work->transition = effect;
}

/// Teleport cast on a circle: the cave turns grey, the circle shimmers and
/// sparks carry Felix off, and he comes down on the other circle at (x, z).
#define DEFINE_ISLET_CAVE_ARRIVE_AT(name, x, z)                                \
  void name(void) {                                                            \
    Teleport_StartShimmer();                                                   \
    Teleport_PlaySparks();                                                     \
                                                                               \
    gMainRoutineWork.teleportState = 1;                                        \
    Event_Wait(40);                                                            \
    Actor_SetHorizontalPositionWithTerrain(gMainRoutineWork.leaderActorId,     \
                                           toFIXED(x), toFIXED(z));            \
    IsletCave_FinishTeleport();                                                \
  }

/// Teleport cast on the circle on the west hall's island (cells (14, 29) to
/// (15, 30)) carries Felix to the circle on the ledge at the north of the lake,
/// whose doorway leads to the corridors. The cave turns grey, sparks whirl
/// round him and he is gone; the view slides north and he comes down in a
/// whirl of sparks on the other circle, and the colours come back.
DEFINE_ISLET_CAVE_ARRIVE_AT(IsletCave_TeleportToNorthCircle, 232, 136)

/// Teleport cast on the circle on the ledge at the north of the west hall's
/// lake (cells (13, 7) to (15, 9)) carries Felix back south to the circle on
/// the island, the way he came, with the same grey and sparks.
DEFINE_ISLET_CAVE_ARRIVE_AT(IsletCave_TeleportToIslandCircle, 240, 480)

/// One level of a palette fade's colour channel, which holds its 0 to 31
/// shifted up 10 bits: also the scale that leaves a colour as it is.
#define FADE_CHANNEL_LEVEL (1 << 10)

/// Darkens the room by the scale given, out of FADE_CHANNEL_LEVEL, so that
/// each corridor is darker than the last. Through the bug below only the reds
/// and greens dim, so the deeper corridors and the Sentinel's room look bluer
/// as well as darker.
void IsletCave_DarkenPalette(s32 scale) {
  struct PaletteFade *fade = Ram_HeapSlots->paletteFade;
  u16 *source = fade->palette;
  u16 *channel = fade->target;
  u32 count = ARRAY_COUNT(fade->palette) / 2;
  u32 i = 0;
  u32 color, value, mask;

  do {
    color = *source++;
    mask = ~(FADE_CHANNEL_LEVEL - 1);
    value = RGB_BLUE(color);
#ifdef BUGFIX
    value *= scale;
#else
    // BUG: blue is multiplied by the full scale, not by scale, so it never
    // dims.
    value *= FADE_CHANNEL_LEVEL;
#endif
    value &= mask;
    if (value > RGB_LEVEL_MAX * FADE_CHANNEL_LEVEL)
      value = RGB_LEVEL_MAX * FADE_CHANNEL_LEVEL;
    *channel++ = value;
    value = RGB_GREEN(color);
    value *= scale;
    value &= mask;
    if (value > RGB_LEVEL_MAX * FADE_CHANNEL_LEVEL)
      value = RGB_LEVEL_MAX * FADE_CHANNEL_LEVEL;
    *channel++ = value;
    value = RGB_RED(color);
    value *= scale;
    value &= mask;
    if (value > RGB_LEVEL_MAX * FADE_CHANNEL_LEVEL)
      value = RGB_LEVEL_MAX * FADE_CHANNEL_LEVEL;
    *channel++ = value;
    i++;
  } while (i < count);
}

extern struct MapGridCellAnimation gIsletCave_Doors[];

/// The last of the actor ids the scene's placed actors take.
#define LAST_PLACED_ACTOR 63

/// Walking north into the grey stone door at the north end of a corridor
/// opens it: everything stops, the door slides open, Felix steps into the
/// dark doorway, and the next corridor comes up with him at its south end.
/// The fifth corridor's door leads to the Sentinel's room. The doors are at
/// cells (8, 6), (20, 6), (32, 6), (43, 6) and (54, 6), each cell's behaviour
/// the exit it leaves by.
void IsletCave_OpenDoorAndEnter(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  u32 i;
  u32 index;

  Event_Start();
  Event_ResolvePendingActions(0);
  for (i = ACTOR_FIRST_PLACED; i <= LAST_PLACED_ACTOR; i++) {
    struct Actor *actor = Actor_GetById(i);

    if (actor != 0)
      actor->motionFlags = 0;
  }
  index = work->stepTrigger - 1;
  Sound_Play(SE_DOOR_OPEN);
  index >>= 1;
  {
    s32 x = gIsletCave_Doors[index].x;
    s32 y = gIsletCave_Doors[index].y;

    MapGrid_AnimateCells(gIsletCave_Doors[index].steps, x, y);
  }
  Actor_SetAnimationById(gMainRoutineWork.leaderActorId, ANIM_WALK);
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 0.5);
  Actor_SetDestinationOffset(gMainRoutineWork.leaderActorId, 0, -4);
  Event_Wait(6);
  Import_EventSetExitRequest(work->stepTrigger);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_End();
}

extern u16 gTeleportCircle_PaletteSlots[];
extern u16 gTeleportCircle_Colors[];
extern u16 gTeleportCircle_ShimmerColors[];
extern u16 gTeleportCircle_ShimmerSlots[];
extern s32 gIsletCave_TremorTargetScript[];

/// Sets up the map being entered: in the halls, the logs where the party left
/// them and the Teleport circles' colours; in the Sentinel's room, the
/// Sentinel gone once beaten; in the corridors, Serac hidden in the winged
/// statue until Tremor. The corridors darken an eighth at a time from the
/// third, and the Sentinel's room is as dark as the fifth.
s32 Scene_Initialize(void) {
  s32 dim;

  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
  FieldEffect_StartBlankOverlay(0);
  if (gMainRoutineWork.next == SCENE_ISLET_CAVE_HALLS) {
    IsletCave_PlaceLogs();
    Palette_SetColorsFade(gTeleportCircle_PaletteSlots, gTeleportCircle_Colors,
                          gTeleportCircle_ShimmerColors,
                          gTeleportCircle_ShimmerSlots);
  } else if (gMainRoutineWork.next == SCENE_ISLET_CAVE_SENTINEL) {
    Actor_SetSpriteFlags(Actor_GetById(ACTOR_FIRST_FIRE), 0);
    if (gMainRoutineWork.entrance == ENTRANCE_AFTER_SENTINEL_BATTLE) {
      Actor_SetPosition(ACTOR_SENTINEL, 0, 0);
      GameFlag_Set(FLAG_SENTINEL_DEFEATED);
    }
  } else if (gMainRoutineWork.next == SCENE_ISLET_CAVE_CORRIDORS) {
    if (GameFlag_Get(FLAG_DJINNI(DJINNI_SERAC)) ||
        GameFlag_Get(FLAG_DJINNI_HIDING(DJINNI_SERAC))) {
      GameFlag_Set(FLAG_SERAC_REVEALED);
    } else if (!GameFlag_Get(FLAG_SERAC_REVEALED)) {
      Actor_SetPosition(ACTOR_SERAC, 0, 0);
      Actor_SetMovementTypeById(ACTOR_WINGED_STATUE,
                                gIsletCave_TremorTargetScript);
    }
  }
  if (gMainRoutineWork.next == SCENE_ISLET_CAVE_CORRIDORS ||
      gMainRoutineWork.next == SCENE_ISLET_CAVE_SENTINEL) {
    if (gMainRoutineWork.next == SCENE_ISLET_CAVE_CORRIDORS)
      dim = (gMainRoutineWork.entrance - 3) / 2;
    else
      dim = 3;
    IsletCave_DarkenPalette(FADE_CHANNEL_LEVEL -
                            dim * (FADE_CHANNEL_LEVEL / 8));
    PaletteFade_PackTarget();
    PaletteFade_StartInterpolation(1);
    Event_WaitTransitionFrames();
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

/// The golden logs floating in the halls' water. The first lies between the
/// Teleport circle's island and the west shore, the second at the north of the
/// west hall's lake, and the third to sixth in pairs at the sides of the east
/// hall's channel, the third and fourth at its north end and the fifth and
/// sixth halfway down. These six lie north to south and float east and west.
/// The seventh lies west to east across the channel's south end and floats
/// north and south.
enum {
  ACTOR_FIRST_LOG = ACTOR_FIRST_PLACED,
  ACTOR_SECOND_LOG,
  ACTOR_THIRD_LOG,
  ACTOR_FOURTH_LOG,
  ACTOR_FIFTH_LOG,
  ACTOR_SIXTH_LOG,
  ACTOR_SEVENTH_LOG
};

/// The cells under each log, which IsletCave_PlaceLogs copies in under it from
/// the map's top row: stepping onto them dips the log, and walking on across
/// them rides it.
enum {
  CELL_BEHAVIOR_ISLET_CAVE_FIRST_LOG = 20,
  CELL_BEHAVIOR_ISLET_CAVE_SECOND_LOG,
  CELL_BEHAVIOR_ISLET_CAVE_THIRD_LOG,
  CELL_BEHAVIOR_ISLET_CAVE_FOURTH_LOG,
  CELL_BEHAVIOR_ISLET_CAVE_FIFTH_LOG,
  CELL_BEHAVIOR_ISLET_CAVE_SIXTH_LOG,
  CELL_BEHAVIOR_ISLET_CAVE_SEVENTH_LOG
};

/// Set while a log lies at the far side of its water from where it starts, so
/// that it is still there when the party comes back into the hall;
/// IsletCave_PlaceLogs reads them. They are scene flags, so every log is back
/// at its start once the party has left the cave.
///
/// The fourth log is the odd one: its ride west, away from its start, clears
/// its flag as the ride back does, so the next time the party comes into the
/// east hall that log is back at the channel's east side.
enum {
  FLAG_ISLET_CAVE_FIRST_LOG_MOVED = FLAG_SCENE_TEMP_80,
  FLAG_ISLET_CAVE_SECOND_LOG_MOVED = FLAG_SCENE_TEMP_81,
  FLAG_ISLET_CAVE_THIRD_LOG_MOVED = FLAG_SCENE_TEMP_82,
  FLAG_ISLET_CAVE_FOURTH_LOG_MOVED = FLAG_SCENE_TEMP_83,
  FLAG_ISLET_CAVE_FIFTH_LOG_MOVED = FLAG_SCENE_TEMP_84,
  FLAG_ISLET_CAVE_SIXTH_LOG_MOVED = FLAG_SCENE_TEMP_85,
  FLAG_ISLET_CAVE_SEVENTH_LOG_MOVED = FLAG_SCENE_TEMP_86
};

/// Places a log lying west to east, three cells wide: at (x, z) if its flag
/// says it is at the far side, then copies its cells in under it from the
/// stored row at (sourceColumn, sourceRow). Its cell behaviour is passed and
/// not used.
void IsletCave_PlaceWestEastLog(s32 actorId, s32 sourceColumn, s32 sourceRow,
                                s32 x, s32 z, s32 cellBehavior, s32 flag) {
  struct Actor *actor = Actor_GetById(actorId);

  if (actor != NULL) {
    if (GameFlag_Get(flag))
      Actor_SetPosition(actorId, x, z);
    Actor_SetSpritePriority(actorId, 3);
    actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    MapGrid_CopyCellAttributes(sourceColumn, sourceRow, 3, 1,
                               FIELD_CELL(actor->x.fixed) - 1,
                               FIELD_CELL(actor->z.fixed));
  }
}

/// As IsletCave_PlaceWestEastLog, for a log lying north to south, three cells
/// tall.
void IsletCave_PlaceNorthSouthLog(s32 actorId, s32 sourceColumn, s32 sourceRow,
                                  s32 x, s32 z, s32 cellBehavior, s32 flag) {
  struct Actor *actor;

  if (Actor_GetById(actorId) != 0) {
    Actor_SetSpritePriority(actorId, 3);
    if (GameFlag_Get(flag))
      Actor_SetPosition(actorId, x, z);
    actor = Actor_GetById(actorId);
    actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    MapGrid_CopyCellAttributes(sourceColumn, sourceRow, 1, 3,
                               FIELD_CELL(actor->x.fixed),
                               FIELD_CELL(actor->z.fixed) - 1);
  }
}

/// Stepping onto a log plays its dip under Felix's step, set back to floating
/// first so that the dip plays from its start: as he steps onto it, the log
/// spreads wider and flashes darker orange for about a third of a second, then
/// settles back to its narrow yellow look.
void IsletCave_SettleLog(s32 actorId) {
  Actor_SetAnimationById(actorId, FLOATING_LOG_ANIMATION_FLOAT);
  Actor_SetAnimationById(actorId, FLOATING_LOG_ANIMATION_SETTLE);
  return;
}

/// Places the seven logs where the party left them, each with its cells.
void IsletCave_PlaceLogs(void) {
  IsletCave_PlaceNorthSouthLog(ACTOR_FIRST_LOG, 27, 0, 136, 504,
                               CELL_BEHAVIOR_ISLET_CAVE_FIRST_LOG,
                               FLAG_ISLET_CAVE_FIRST_LOG_MOVED);
  IsletCave_PlaceNorthSouthLog(ACTOR_SECOND_LOG, 28, 0, 136, 232,
                               CELL_BEHAVIOR_ISLET_CAVE_SECOND_LOG,
                               FLAG_ISLET_CAVE_SECOND_LOG_MOVED);
  IsletCave_PlaceNorthSouthLog(ACTOR_THIRD_LOG, 29, 0, 856, 152,
                               CELL_BEHAVIOR_ISLET_CAVE_THIRD_LOG,
                               FLAG_ISLET_CAVE_THIRD_LOG_MOVED);
  IsletCave_PlaceNorthSouthLog(ACTOR_FOURTH_LOG, 30, 0, 792, 152,
                               CELL_BEHAVIOR_ISLET_CAVE_FOURTH_LOG,
                               FLAG_ISLET_CAVE_FOURTH_LOG_MOVED);
  IsletCave_PlaceNorthSouthLog(ACTOR_FIFTH_LOG, 31, 0, 856, 376,
                               CELL_BEHAVIOR_ISLET_CAVE_FIFTH_LOG,
                               FLAG_ISLET_CAVE_FIFTH_LOG_MOVED);
  IsletCave_PlaceNorthSouthLog(ACTOR_SIXTH_LOG, 32, 0, 792, 376,
                               CELL_BEHAVIOR_ISLET_CAVE_SIXTH_LOG,
                               FLAG_ISLET_CAVE_SIXTH_LOG_MOVED);
  IsletCave_PlaceWestEastLog(ACTOR_SEVENTH_LOG, 33, 0, 824, 120,
                             CELL_BEHAVIOR_ISLET_CAVE_SEVENTH_LOG,
                             FLAG_ISLET_CAVE_SEVENTH_LOG_MOVED);
  return;
}

/// Stepping onto the first log, between the Teleport circle's island and the
/// west shore of the west hall, plays the log's dip under Felix's step.
void IsletCave_SettleFirstLog(void) {
  IsletCave_SettleLog(ACTOR_FIRST_LOG);
  return;
}

/// Stepping onto the second log, at the north of the west hall's lake, plays
/// the log's dip under Felix's step.
void IsletCave_SettleSecondLog(void) {
  IsletCave_SettleLog(ACTOR_SECOND_LOG);
  return;
}

/// Stepping onto the third log, at the north end of the east hall's channel,
/// plays the log's dip under Felix's step.
void IsletCave_SettleThirdLog(void) {
  IsletCave_SettleLog(ACTOR_THIRD_LOG);
  return;
}

/// Stepping onto the fourth log, at the north end of the east hall's channel,
/// plays the log's dip under Felix's step.
void IsletCave_SettleFourthLog(void) {
  IsletCave_SettleLog(ACTOR_FOURTH_LOG);
  return;
}

/// Stepping onto the fifth log, halfway down the east hall's channel, plays the
/// log's dip under Felix's step.
void IsletCave_SettleFifthLog(void) {
  IsletCave_SettleLog(ACTOR_FIFTH_LOG);
  return;
}

/// Stepping onto the sixth log, halfway down the east hall's channel, plays the
/// log's dip under Felix's step.
void IsletCave_SettleSixthLog(void) {
  IsletCave_SettleLog(ACTOR_SIXTH_LOG);
  return;
}

/// Stepping onto the seventh log, which lies across the east hall's channel,
/// plays the log's dip under Felix's step.
void IsletCave_SettleSeventhLog(void) {
  IsletCave_SettleLog(ACTOR_SEVENTH_LOG);
  return;
}

/// Rides a log one stage, `dx` and `dz` pixels, carrying Felix with it: the
/// log rolls over as it floats, he treads on top facing back the way he came,
/// and it settles where it stops.
void IsletCave_RideLog(s32 actorId, s32 dx, s32 dz) {
  struct Actor *leader;
  struct Actor *actor;
  FIXED x;
  FIXED z;

  leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  actor = Actor_GetById(actorId);
  Event_Start();
  Event_ResolvePendingActions(0);
  x = ((leader->x.fixed + toFIXED(dx)) & FIELD_CELL_MASK) + toFIXED(8);
  z = ((leader->z.fixed + toFIXED(dz)) & FIELD_CELL_MASK) + toFIXED(8);
  leader->speedLimit = toFIXED(1);
  leader->acceleration = toFIXED(1) / 2;
  Actor_SetMoveTarget(leader, x, leader->y.fixed, z);
  Actor_SelectAnimation(leader, ANIM_LEADER_WALK_BACKWARDS);
  x = ((actor->x.fixed + toFIXED(dx)) & FIELD_CELL_MASK) + toFIXED(8);
  z = ((actor->z.fixed + toFIXED(dz)) & FIELD_CELL_MASK) + toFIXED(8);
  actor->speedLimit = toFIXED(1);
  actor->acceleration = toFIXED(1) / 2;
  Actor_SetMoveTarget(actor, x, actor->y.fixed, z);
  if (dx < 0 || dz < 0) {
    Actor_SelectAnimation(actor, FLOATING_LOG_ANIMATION_ROLL_WEST_OR_NORTH);
  } else {
    Actor_SelectAnimation(actor, FLOATING_LOG_ANIMATION_ROLL_EAST_OR_SOUTH);
  }
  Sound_Play(SE_LOG_PUSH);
  Actor_WaitForMove(leader);
  Actor_SelectAnimation(actor, FLOATING_LOG_ANIMATION_SETTLE);
  Sound_Play(SE_STOP_EXIT_SE);
  Event_End();
}

/// Rides log `logId` `dx` pixels across the water and moves its cells with it,
/// then sets or clears its flag (`flagCall`) to say which side it lies at.
#define DEFINE_ISLET_CAVE_RIDE_LOG_ACROSS(name, logId, dx, flagCall, flag)     \
  void name(void) {                                                            \
    s32 fromX = Actor_GetById(logId)->x.fixed;                                 \
    s32 fromZ = Actor_GetById(logId)->z.fixed;                                 \
                                                                               \
    IsletCave_RideLog(logId, dx, 0);                                           \
    wait(2);                                                                   \
    Map_CopyCellAttributeRect(FIELD_CELL(fromX), FIELD_CELL(fromZ) - 1, 1, 3,  \
                              FIELD_CELL(Actor_GetById(logId)->x.fixed),       \
                              FIELD_CELL(fromZ) - 1);                          \
    Map_CopyCellAttributeRect(0, 0, 1, 3, FIELD_CELL(fromX),                   \
                              FIELD_CELL(fromZ) - 1);                          \
    flagCall(flag);                                                            \
  }

/// Walking east from the west shore of the west hall, by the Turtle Boots
/// chest, onto the first log (cells (8, 30) to (8, 32)) floats it two cells
/// east with Felix aboard, back to the Teleport circle's island.
DEFINE_ISLET_CAVE_RIDE_LOG_ACROSS(IsletCave_RideFirstLogEast, ACTOR_FIRST_LOG,
                                  32, GameFlag_Clear,
                                  FLAG_ISLET_CAVE_FIRST_LOG_MOVED)

/// Walking west from the Teleport circle's island onto the first log (cells
/// (10, 30) to (10, 32)) floats it two cells west with Felix aboard, across to
/// the west shore and the Turtle Boots chest.
DEFINE_ISLET_CAVE_RIDE_LOG_ACROSS(IsletCave_RideFirstLogWest, ACTOR_FIRST_LOG,
                                  -32, GameFlag_Set,
                                  FLAG_ISLET_CAVE_FIRST_LOG_MOVED)

/// Walking east from the little ledge of the Rusty Staff chest, at the west of
/// the west hall's lake, onto the second log (cells (8, 13) to (8, 15)) floats
/// it 13 cells east with Felix aboard, with a stop halfway, back to the ledge
/// where the passage from the east hall comes in.
void IsletCave_RideSecondLogEast(void) {
  FIXED fromX = Actor_GetById(ACTOR_SECOND_LOG)->x.fixed;
  FIXED fromZ = Actor_GetById(ACTOR_SECOND_LOG)->z.fixed;

  IsletCave_RideLog(ACTOR_SECOND_LOG, 112, 0);
  IsletCave_RideLog(ACTOR_SECOND_LOG, 96, 0);
  wait(2);
  Map_CopyCellAttributeRect(
      FIELD_CELL(fromX), FIELD_CELL(fromZ) - 1, 1, 3,
      FIELD_CELL(Actor_GetById(ACTOR_SECOND_LOG)->x.fixed),
      FIELD_CELL(fromZ) - 1);
  Map_CopyCellAttributeRect(0, 0, 1, 3, FIELD_CELL(fromX),
                            FIELD_CELL(fromZ) - 1);
  GameFlag_Clear(FLAG_ISLET_CAVE_SECOND_LOG_MOVED);
}

/// Walking west from the ledge at the east of the west hall's lake, where the
/// passage from the east hall's north-west comes in, onto the second log (cells
/// (21, 13) to (21, 15)) floats it 13 cells west with Felix aboard, with a stop
/// halfway, across the north of the lake to the little ledge of the Rusty Staff
/// chest.
void IsletCave_RideSecondLogWest(void) {
  FIXED fromX = Actor_GetById(ACTOR_SECOND_LOG)->x.fixed;
  FIXED fromZ = Actor_GetById(ACTOR_SECOND_LOG)->z.fixed;

  IsletCave_RideLog(ACTOR_SECOND_LOG, -112, 0);
  IsletCave_RideLog(ACTOR_SECOND_LOG, -96, 0);
  wait(2);
  Map_CopyCellAttributeRect(
      FIELD_CELL(fromX), FIELD_CELL(fromZ) - 1, 1, 3,
      FIELD_CELL(Actor_GetById(ACTOR_SECOND_LOG)->x.fixed),
      FIELD_CELL(fromZ) - 1);
  Map_CopyCellAttributeRect(0, 0, 1, 3, FIELD_CELL(fromX),
                            FIELD_CELL(fromZ) - 1);
  GameFlag_Set(FLAG_ISLET_CAVE_SECOND_LOG_MOVED);
}

/// Walking east from the ledge at the north-west of the east hall onto the
/// third log (cells (48, 8) to (48, 10)) floats it five cells east with Felix
/// aboard, across the channel to its narrow east bank.
DEFINE_ISLET_CAVE_RIDE_LOG_ACROSS(IsletCave_RideThirdLogEast, ACTOR_THIRD_LOG,
                                  80, GameFlag_Set,
                                  FLAG_ISLET_CAVE_THIRD_LOG_MOVED)

/// Walking west from the east hall's narrow east bank onto the third log (cells
/// (53, 8) to (53, 10)) floats it five cells west with Felix aboard, back to
/// the ledge at the channel's north-west.
DEFINE_ISLET_CAVE_RIDE_LOG_ACROSS(IsletCave_RideThirdLogWest, ACTOR_THIRD_LOG,
                                  -80, GameFlag_Clear,
                                  FLAG_ISLET_CAVE_THIRD_LOG_MOVED)

/// Walking east from the ledge at the north-west of the east hall onto the
/// fourth log (cells (49, 8) to (49, 10)) floats it five cells east with Felix
/// aboard, back across the channel to its narrow east bank.
DEFINE_ISLET_CAVE_RIDE_LOG_ACROSS(IsletCave_RideFourthLogEast, ACTOR_FOURTH_LOG,
                                  80, GameFlag_Clear,
                                  FLAG_ISLET_CAVE_FOURTH_LOG_MOVED)

/// Walking west from the east hall's narrow east bank onto the fourth log
/// (cells (54, 8) to (54, 10)) floats it five cells west with Felix aboard,
/// across the channel to the ledge at its north-west, where the passage to the
/// west hall's north leaves.
DEFINE_ISLET_CAVE_RIDE_LOG_ACROSS(IsletCave_RideFourthLogWest, ACTOR_FOURTH_LOG,
                                  -80, GameFlag_Clear,
                                  FLAG_ISLET_CAVE_FOURTH_LOG_MOVED)

/// Walking east from the east hall's south-west shore, where the passage from
/// the west hall comes in, onto the fifth log (cells (48, 22) to (48, 24))
/// floats it five cells east with Felix aboard, across the channel to its east
/// bank.
DEFINE_ISLET_CAVE_RIDE_LOG_ACROSS(IsletCave_RideFifthLogEast, ACTOR_FIFTH_LOG,
                                  80, GameFlag_Set,
                                  FLAG_ISLET_CAVE_FIFTH_LOG_MOVED)

/// Walking west from the east hall's east bank onto the fifth log (cells (53,
/// 22) to (53, 24)) floats it five cells west with Felix aboard, back to the
/// south-west shore.
DEFINE_ISLET_CAVE_RIDE_LOG_ACROSS(IsletCave_RideFifthLogWest, ACTOR_FIFTH_LOG,
                                  -80, GameFlag_Clear,
                                  FLAG_ISLET_CAVE_FIFTH_LOG_MOVED)

/// Walking east from the east hall's south-west shore onto the sixth log (cells
/// (49, 22) to (49, 24)) floats it five cells east with Felix aboard, back
/// across the channel to its east bank.
DEFINE_ISLET_CAVE_RIDE_LOG_ACROSS(IsletCave_RideSixthLogEast, ACTOR_SIXTH_LOG,
                                  80, GameFlag_Clear,
                                  FLAG_ISLET_CAVE_SIXTH_LOG_MOVED)

/// Walking west from the east hall's east bank onto the sixth log (cells (54,
/// 22) to (54, 24)) floats it five cells west with Felix aboard, across the
/// channel to the south-west shore.
DEFINE_ISLET_CAVE_RIDE_LOG_ACROSS(IsletCave_RideSixthLogWest, ACTOR_SIXTH_LOG,
                                  -80, GameFlag_Set,
                                  FLAG_ISLET_CAVE_SIXTH_LOG_MOVED)

/// Walking north from the ledge at the south end of the east hall onto the
/// seventh log (cells (50, 29) to (52, 29)) floats it 22 cells north up the
/// channel with Felix aboard, past the other logs, stopping twice on the way,
/// to the foot of Meld's ledge.
void IsletCave_RideSeventhLogNorth(void) {
  FIXED fromX = Actor_GetById(ACTOR_SEVENTH_LOG)->x.fixed;
  s32 column = fromX;
  s32 dz = 112;
  s32 fromRow = Actor_GetById(ACTOR_SEVENTH_LOG)->z.fixed;

  dz = -dz;
  fromRow = FIELD_CELL(fromRow);
  IsletCave_RideLog(ACTOR_SEVENTH_LOG, 0, dz);
  IsletCave_RideLog(ACTOR_SEVENTH_LOG, 0, dz);
  IsletCave_RideLog(ACTOR_SEVENTH_LOG, 0, -128);
  wait(2);
  column = FIELD_CELL(column) - 1;
  MapGrid_CopyCellAttributes(
      column, fromRow, 3, 1, column,
      FIELD_CELL(Actor_GetById(ACTOR_SEVENTH_LOG)->z.fixed));
  Map_CopyCellAttributeRect(0, 0, 3, 1, column, fromRow);
  GameFlag_Set(FLAG_ISLET_CAVE_SEVENTH_LOG_MOVED);
}

/// Walking south from Meld's ledge at the north end of the east hall onto the
/// seventh log (cells (50, 7) to (52, 7)) floats it 22 cells south down the
/// channel with Felix aboard, stopping twice on the way, back to the ledge at
/// the channel's south end.
void IsletCave_RideSeventhLogSouth(void) {
  FIXED fromX = Actor_GetById(ACTOR_SEVENTH_LOG)->x.fixed;
  s32 column = fromX;
  FIXED fromZ = Actor_GetById(ACTOR_SEVENTH_LOG)->z.fixed;
  s32 fromRow = fromZ;

  IsletCave_RideLog(ACTOR_SEVENTH_LOG, 0, 112);
  IsletCave_RideLog(ACTOR_SEVENTH_LOG, 0, 112);
  IsletCave_RideLog(ACTOR_SEVENTH_LOG, 0, 128);
  wait(2);
  column = FIELD_CELL(column) - 1;
  fromRow = FIELD_CELL(fromRow);
  MapGrid_CopyCellAttributes(
      column, fromRow, 3, 1, column,
      FIELD_CELL(Actor_GetById(ACTOR_SEVENTH_LOG)->z.fixed));
  Map_CopyCellAttributeRect(0, 0, 3, 1, column, fromRow);
  GameFlag_Clear(FLAG_ISLET_CAVE_SEVENTH_LOG_MOVED);
}

/// Walking up the Sentinel's room to the great armoured warrior at its north
/// end (cells (9, 11) to (13, 13), the ground just before it) wakes it. It
/// springs twice, the room glows warm, and it says across the screen "I am the
/// guardian of wind. If you would claim my power, you must prove your worth."
/// Then it comes at Felix and a battle with it starts, one the party cannot run
/// from. Beating it leaves the Catastrophe tablet free behind its spot.
void IsletCave_SentinelAwaits(void) {
  s32 cue;
  struct Actor *actor;

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_StartRepeatedMotion(ACTOR_SENTINEL, 2);
  Event_Wait(20);
  PaletteFade_ApplySource(PALETTE_TINT_NONE, 0);
  PaletteFade_SetTarget(PALETTE_TINT_WARM, 1);
  PaletteFade_StartInterpolation(60);
  Event_Wait(60);
  EventMessage_ShowCentered((s32)MsgSummonGuardianOfWind, 0, 2);
  PaletteFade_SetTarget(PALETTE_TINT_NONE, 0);
  PaletteFade_StartInterpolation(60);
  Event_Wait(60);
  actor = Actor_GetById(gMainRoutineWork.leaderActorId);
  if (actor != 0) {
    Actor_SetDestinationById(ACTOR_SENTINEL, actor->x.part[0].integer,
                             actor->z.part[0].integer);
  }
  Event_Wait(20);
  FieldEvent_SetAfterBattleDestination(SCENE_ISLET_CAVE_SENTINEL,
                                       ENTRANCE_AFTER_SENTINEL_BATTLE);
  Battle_SetType(BATTLE_TYPE_NO_ESCAPE);
  Field_RequestBattleFromEncounterTable(ENCOUNTER_TABLE_HIDDEN_BOSSES,
                                        HIDDEN_BOSS_SENTINEL);
  Event_End();
}

/// Tremor cast on the winged statue that rocks and hops on its pillar at the
/// west side of the second corridor shakes Serac out. The Mercury Djinni leaps
/// from the statue and lands in the corridor beside Felix, and the statue
/// stands still. While Serac hides, nothing marks him but the rocking statue.
void IsletCave_TremorRevealsSerac(void) {
  if (!Event_GetFlag(FLAG_SERAC_REVEALED)) {
    struct Actor *actor = Actor_GetById(ACTOR_SERAC);
    struct Actor *owner = Actor_GetById(gMainRoutineWork.leaderActorId);

    Event_SetFlag(FLAG_SERAC_REVEALED);
    Actor_SetPosition(ACTOR_SERAC, 296, 160);
    actor->speedLimit = toFIXED(3) / 2;
    actor->velocityY = toFIXED(15);
    Actor_MoveToAndWait(ACTOR_SERAC, 344,
                        FIELD_HALF_CELL(owner->z.fixed) * 8 + 24);
    Actor_Stop(ACTOR_WINGED_STATUE);
    actor->speedLimit = toFIXED(1);
    Actor_TurnToFaceEachOther(gMainRoutineWork.leaderActorId, ACTOR_SERAC, 0);
  }
}

#undef DEFINE_ISLET_CAVE_ARRIVE_AT
#undef DEFINE_ISLET_CAVE_RIDE_LOG_ACROSS

void IsletCave_RideFirstLogWest(void);
void IsletCave_RideThirdLogWest(void);

// clang-format off
#define TELEPORT_CIRCLE_FIRST_ENTRY 9
#include "shared/teleport_circle_palette.c"

u32 gIsletCave_SentinelScript[] = {
    SC_SET(SW_SCALE_Y, FIXED_ONE * 13 / 10),
    SC_SET(SW_SCALE_X, FIXED_ONE * 13 / 10),
    SC_END,
};

s32 gIsletCave_ShrunkFireScript[] = {
    SC_SET(SW_MOTION, 0),
    SC_ADD(SW_Y, toFIXED(-4)),
    SC_SET(SW_SCALE_X, FIXED_ONE * 4 / 5),
    SC_SET(SW_SCALE_Y, FIXED_ONE * 4 / 5),
    SC_CALL(IsletCave_ClearScriptedActorSpriteFlags),
    SC_END,
};

s32 gIsletCave_TremorTargetScript[] = {
    SC_CALL(IsletCave_HopWingedStatue),
    SC_END,
};

#include "shared/scene_entrances_default.c"

u32 gSceneExits[] = {
    SCENE_EXITS(IsletCaveHalls),
    SCENE_EXIT_UNLESS(1, SeaOfTimeIslet, 5, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, IsletCaveHalls, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, IsletCaveHalls, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, IsletCaveHalls, 5, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(5, IsletCaveHalls, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(6, IsletCaveHalls, 7, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(7, IsletCaveHalls, 6, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(8, IsletCaveCorridors, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(9, IsletCaveHalls, 10, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(10, IsletCaveHalls, 9, EXIT_FLAG_NONE),
    SCENE_EXITS(IsletCaveCorridors),
    SCENE_EXIT_UNLESS(1, IsletCaveHalls, 8, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, IsletCaveCorridors, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, IsletCaveCorridors, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, IsletCaveCorridors, 5, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(5, IsletCaveCorridors, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(6, IsletCaveCorridors, 7, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(7, IsletCaveCorridors, 6, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(8, IsletCaveCorridors, 9, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(9, IsletCaveCorridors, 8, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(10, IsletCaveSentinel, 1, EXIT_FLAG_NONE),
    SCENE_EXITS(IsletCaveSentinel),
    SCENE_EXIT_UNLESS(1, IsletCaveCorridors, 10, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

struct ActorPlacement gIsletCave_SentinelRoomPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_SENTINEL, FLAG_SENTINEL_DEFEATED, (s32)gIsletCave_SentinelScript, 184, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, (s32)gIsletCave_ShrunkFireScript, 152, 0, 144, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, (s32)gIsletCave_ShrunkFireScript, 216, 0, 144, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, (s32)gIsletCave_ShrunkFireScript, 136, 0, 176, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, (s32)gIsletCave_ShrunkFireScript, 232, 0, 176, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, (s32)gIsletCave_ShrunkFireScript, 152, 0, 224, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, (s32)gIsletCave_ShrunkFireScript, 216, 0, 224, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, (s32)gIsletCave_ShrunkFireScript, 136, 0, 256, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, (s32)gIsletCave_ShrunkFireScript, 232, 0, 256, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, (s32)gIsletCave_ShrunkFireScript, 136, 0, 304, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, (s32)gIsletCave_ShrunkFireScript, 232, 0, 304, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    {SCENE_TABLE_END},
};

struct ActorPlacement gIsletCave_HallsPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_FLOATING_LOG_NORTH_SOUTH, CONDITION_ALWAYS, 7, 168, 0, 504, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FLOATING_LOG_NORTH_SOUTH, CONDITION_ALWAYS, 7, 344, 0, 232, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FLOATING_LOG_NORTH_SOUTH, CONDITION_ALWAYS, 7, 776, 0, 152, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FLOATING_LOG_NORTH_SOUTH, CONDITION_ALWAYS, 7, 872, 0, 152, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FLOATING_LOG_NORTH_SOUTH, CONDITION_ALWAYS, 7, 776, 0, 376, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FLOATING_LOG_NORTH_SOUTH, CONDITION_ALWAYS, 7, 872, 0, 376, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FLOATING_LOG_WEST_EAST, CONDITION_ALWAYS, 7, 824, 0, 472, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_VENUS_DJINNI, FLAG_DJINNI(DJINNI_MELD), MOVEMENT_TYPE_STAND, 824, 0, 88, FACING_SOUTH, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gIsletCave_CorridorsPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_WINGED_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MERCURY_DJINNI, FLAG_DJINNI(DJINNI_SERAC), MOVEMENT_TYPE_STAND, 336, 0, 184, FACING_SOUTH, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

struct Trigger gIsletCave_HallsTriggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), CELL_BEHAVIOR_ISLET_CAVE_FIRST_LOG, CONDITION_ALWAYS, EVENT(IsletCave_RideFirstLogEast)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), CELL_BEHAVIOR_ISLET_CAVE_FIRST_LOG, CONDITION_ALWAYS, EVENT(IsletCave_RideFirstLogWest)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), CELL_BEHAVIOR_ISLET_CAVE_SECOND_LOG, CONDITION_ALWAYS, EVENT(IsletCave_RideSecondLogEast)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), CELL_BEHAVIOR_ISLET_CAVE_SECOND_LOG, CONDITION_ALWAYS, EVENT(IsletCave_RideSecondLogWest)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), CELL_BEHAVIOR_ISLET_CAVE_THIRD_LOG, CONDITION_ALWAYS, EVENT(IsletCave_RideThirdLogEast)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), CELL_BEHAVIOR_ISLET_CAVE_THIRD_LOG, CONDITION_ALWAYS, EVENT(IsletCave_RideThirdLogWest)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), CELL_BEHAVIOR_ISLET_CAVE_FOURTH_LOG, CONDITION_ALWAYS, EVENT(IsletCave_RideFourthLogEast)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), CELL_BEHAVIOR_ISLET_CAVE_FOURTH_LOG, CONDITION_ALWAYS, EVENT(IsletCave_RideFourthLogWest)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), CELL_BEHAVIOR_ISLET_CAVE_FIFTH_LOG, CONDITION_ALWAYS, EVENT(IsletCave_RideFifthLogEast)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), CELL_BEHAVIOR_ISLET_CAVE_FIFTH_LOG, CONDITION_ALWAYS, EVENT(IsletCave_RideFifthLogWest)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), CELL_BEHAVIOR_ISLET_CAVE_SIXTH_LOG, CONDITION_ALWAYS, EVENT(IsletCave_RideSixthLogEast)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), CELL_BEHAVIOR_ISLET_CAVE_SIXTH_LOG, CONDITION_ALWAYS, EVENT(IsletCave_RideSixthLogWest)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_ISLET_CAVE_SEVENTH_LOG, CONDITION_ALWAYS, EVENT(IsletCave_RideSeventhLogNorth)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_BEHAVIOR_ISLET_CAVE_SEVENTH_LOG, CONDITION_ALWAYS, EVENT(IsletCave_RideSeventhLogSouth)},
    {TRIGGER_STEP, CELL_BEHAVIOR_ISLET_CAVE_FIRST_LOG, CONDITION_ALWAYS, EVENT(IsletCave_SettleFirstLog)},
    {TRIGGER_STEP, CELL_BEHAVIOR_ISLET_CAVE_SECOND_LOG, CONDITION_ALWAYS, EVENT(IsletCave_SettleSecondLog)},
    {TRIGGER_STEP, CELL_BEHAVIOR_ISLET_CAVE_THIRD_LOG, CONDITION_ALWAYS, EVENT(IsletCave_SettleThirdLog)},
    {TRIGGER_STEP, CELL_BEHAVIOR_ISLET_CAVE_FOURTH_LOG, CONDITION_ALWAYS, EVENT(IsletCave_SettleFourthLog)},
    {TRIGGER_STEP, CELL_BEHAVIOR_ISLET_CAVE_FIFTH_LOG, CONDITION_ALWAYS, EVENT(IsletCave_SettleFifthLog)},
    {TRIGGER_STEP, CELL_BEHAVIOR_ISLET_CAVE_SIXTH_LOG, CONDITION_ALWAYS, EVENT(IsletCave_SettleSixthLog)},
    {TRIGGER_STEP, CELL_BEHAVIOR_ISLET_CAVE_SEVENTH_LOG, CONDITION_ALWAYS, EVENT(IsletCave_SettleSeventhLog)},
    {TRIGGER_TALK, ACTOR_MELD, CONDITION_ALWAYS, EVENT(IsletCave_FightMeld)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_TELEPORT, PSYNERGY_PHASE_AFTER_CAST), CELL_BEHAVIOR_ISLAND_CIRCLE, CONDITION_ALWAYS, EVENT(IsletCave_TeleportToNorthCircle)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_TELEPORT, PSYNERGY_PHASE_AFTER_CAST), CELL_BEHAVIOR_NORTH_CIRCLE, CONDITION_ALWAYS, EVENT(IsletCave_TeleportToIslandCircle)},
    {TRIGGER_TABLE_END},
};

struct Trigger gIsletCave_CorridorsTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), EXIT_FIRST_CORRIDOR_DOOR, CONDITION_ALWAYS, EVENT(IsletCave_OpenDoorAndEnter)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), EXIT_SECOND_CORRIDOR_DOOR, CONDITION_ALWAYS, EVENT(IsletCave_OpenDoorAndEnter)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), EXIT_THIRD_CORRIDOR_DOOR, CONDITION_ALWAYS, EVENT(IsletCave_OpenDoorAndEnter)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), EXIT_FOURTH_CORRIDOR_DOOR, CONDITION_ALWAYS, EVENT(IsletCave_OpenDoorAndEnter)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), EXIT_FIFTH_CORRIDOR_DOOR, CONDITION_ALWAYS, EVENT(IsletCave_OpenDoorAndEnter)},
    {PSYNERGY_ON_ACTOR(ABILITY_TREMOR), ACTOR_WINGED_STATUE, FLAG_SERAC_REVEALED, EVENT(IsletCave_TremorRevealsSerac)},
    {TRIGGER_TALK, ACTOR_SERAC, CONDITION_ALWAYS, EVENT(IsletCave_FightSerac)},
    {TRIGGER_TABLE_END},
};

struct Trigger gIsletCave_SentinelRoomTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_STEP, CELL_BEHAVIOR_BEFORE_SENTINEL, FLAG_SENTINEL_DEFEATED, EVENT(IsletCave_SentinelAwaits)},
    {TRIGGER_TABLE_END},
};

s16 gIsletCave_DoorFrames[] = {
    72, 4, 2, 2, 2, 73, 4, 2,
    2, 2, -1, 0,
};
// clang-format on

struct MapGridCellAnimation gIsletCave_Doors[] = {
    {gIsletCave_DoorFrames, 72, 4},  {gIsletCave_DoorFrames, 84, 4},
    {gIsletCave_DoorFrames, 96, 4},  {gIsletCave_DoorFrames, 107, 4},
    {gIsletCave_DoorFrames, 118, 4},
};
