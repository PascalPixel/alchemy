/// @file
/// Gaia Rock's inside, under the great rock near Izumo where the Serpent
/// lives, in five scenes of one overlay.
///
/// The cave mouth on the first cliffs (gaia_rock.c) opens onto the first
/// altar's room (SceneId_GaiaRockInsideFirstAltar), where two great red faces
/// on the north wall stand in two doorways, one either side of an altar. The
/// Dancing Idol, found on the altar on top of the rock, dances on this altar
/// and the faces slide aside (GaiaRockInside_PlaceDollOnFirstAltar). Three
/// ways open: through the west and east doorways to the room of leafy
/// platforms (SceneId_GaiaRockInsidePlatforms), and a third, past two stumps
/// Felix Moves out of the way, to the maze.
///
/// Cyclone on the bushes of the platforms' room sets its leafy platforms
/// swaying (GaiaRockInside_CycloneStartsPlatforms), and they carry the party
/// on to the twin altars' room (SceneId_GaiaRockInsideTwinAltars) and the
/// altar room above it (SceneId_GaiaRockInsideAltar). There Cyclone flushes a
/// Mad Plant out of the bushes, three stone heads burst out of a wall of
/// handholds under Felix, and Cyclone tears a great cobweb so that the altar
/// it holds falls into the twin altars' room. Four beams of sunlight can be
/// let down into the Serpent's lair: the Dancing Idol makes the face over each
/// of three altars slide aside, and Cyclone clears the bushes under the
/// fourth beam in the twin altars' room. While the Serpent lives, the game
/// shows each new beam falling into the lair.
///
/// The maze (SceneId_GaiaRockInsideMaze by entrances 1 to 4) is fourteen
/// rooms alike, a crossroads with a little plant in the middle that Growth
/// turns into an arrow pointing the way on. Beyond it is the Serpent's lair
/// (the same scene by entrances 9 to 20), where Susa feeds the Serpent casks
/// of Dragonsbane to save Kushinada. The fewer beams reach the lair, the more
/// the Serpent heals in battle (GaiaRockInside_ChallengeSerpent). Once the
/// party beats it Susa slays it, and a slate rises from the floor that
/// teaches Felix Sand (GaiaRockInside_ReadSandTablet). With Sand he passes the
/// stone Serpent to the Cloud Brand behind it
/// (GaiaRockInside_FindCloudBrand).

#include "types.h"
#include "macros.h"
#include "item_ids.h"
#include "field/field_sequence.h"
#include "field/actor.h"
#include "field/event_inline.h"
#include "scene/event_layer.h"
#include "battle/battle.h"
#include "battle/battle_unit.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/palette_buffers.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "field/scene_tables.h"
#include "system/main_routine_work.h"
#include "version.h"
#include "system/ram_buffer.h"
#include "system/field_effect.h"
#include "field/field_sprite.h"
#include "field/field_sequence_event.h"
#include "field/event_commands.h"
#include "field/object.h"
#include "scene/scene.h"
#include "flags.h"
#include "field/field_effect.h"
#include "field/call.h"
#include "system/iwram_call.h"
#include "system/frame.h"
#include "system/io_reg.h"
#include "system/io_write.h"
#include "fixed_math.h"
#include "field/actor_view.h"
#include "field/map_grid.h"
#include "shared/tables.h"
#include "shared/gaia_rock.h"
#include "system/task.h"
#include "field/actor_script.h"
#include "../field/actor_set_sprite_priority.c"

#include "../field/actor_prepare.c"

#include "shared/scene_create_actor.h"

#include "../field/field_effect_move.c"

#define EFFECT_SET_MODE Actor_SelectAnimation
#define EFFECT_SET_SCRIPT Actor_StartScript
#define EFFECT_SPRITE_PRIORITY
#include "../field/field_effect_spawn.c"

/// The four beams of sunlight that reach the Serpent's lair. Three are set
/// once the face over the altar room's altar, the west altar or the east altar
/// has slid aside, the fourth once Cyclone has cleared the bushes under the
/// twin altars' room's beam.
/// GaiaRockInside_DrawSunbeams draws them in the lair.
#define FLAG_ALTAR_ROOM_SUNBEAM 0x9bc
#define FLAG_WEST_ALTAR_SUNBEAM 0x9bd
#define FLAG_EAST_ALTAR_SUNBEAM 0x9be
#define FLAG_BUSHES_SUNBEAM 0x9bf
/// A beam let in before the Serpent is slain, waiting to be shown falling into
/// the lair (Scene_Initialize, entrance 20), which then sets its flag above.
#define FLAG_ALTAR_ROOM_SUNBEAM_TO_SHOW FLAG_AREA_TEMP_07
#define FLAG_WEST_ALTAR_SUNBEAM_TO_SHOW FLAG_AREA_TEMP_08
#define FLAG_EAST_ALTAR_SUNBEAM_TO_SHOW FLAG_AREA_TEMP_09
#define FLAG_BUSHES_SUNBEAM_TO_SHOW FLAG_AREA_TEMP_0A
#define FLAG_SUSA_ATTACKED_SERPENT                                             \
  0x9ac ///< "At last... The serpent has finally had its fill."
#define FLAG_MET_SUSA 0x9de     ///< "Who are you? How did you get here?"
#define FLAG_LEARNED_SAND 0x9ba ///< Felix touched the slate and learned Sand
#define FLAG_SAND_TABLET_RAISED 0x9ee ///< the slate rose from the lair's floor
#define FLAG_ALTAR_FELL 0x9c4         ///< Cyclone tore the cobweb holding it up
#define FLAG_FIRST_ALTAR_FACES_MOVED 0xa62
#define FLAG_PLATFORMS_SWAYING 0xa63
/// The three stone heads that burst out of the altar room's wall of handholds
/// under Felix; each stays out until the party enters another area.
#define FLAG_STATUE_A_RISEN FLAG_AREA_TEMP_03
#define FLAG_STATUE_B_RISEN FLAG_AREA_TEMP_04
#define FLAG_STATUE_C_RISEN FLAG_AREA_TEMP_05
/// Where the first altar's room's two stumps were Moved to, kept so that they
/// stay there (GaiaRockInside_RecordStumpSpots).
#define FLAG_STUMP_SPOTS FLAG_SCENE_TEMP_10
/// While the party stands in Susa's way the cask waits for him, and while it
/// stands in the cask's way Susa waits for it (GaiaRockInside_CheckBlocked).
#define FLAG_SUSA_WAITS_FOR_CASK FLAG_SCENE_TEMP_05
#define FLAG_CASK_WAITS_FOR_SUSA FLAG_SCENE_TEMP_06

/// The cell behaviors of the trigger tables, by room.
enum GaiaRockInsideCellBehavior {
  /// The four ways out of a maze room, in the order of enum MazeWay.
  CELL_BEHAVIOR_MAZE_SOUTH = 1,
  CELL_BEHAVIOR_MAZE_WEST,
  CELL_BEHAVIOR_MAZE_EAST,
  CELL_BEHAVIOR_MAZE_NORTH,
  /// The bushes Cyclone clears: the patch in the platforms' room, and the
  /// cross of five under the beam of sunlight in the twin altars' room.
  CELL_BEHAVIOR_PLATFORMS_BUSHES = 20,
  CELL_BEHAVIOR_SUNBEAM_BUSHES = 20,
  /// The altar room's wall of handholds: its three trap holds, west to east,
  /// in its overhead layer (GaiaRock_TriggerWallCell reads them as Felix
  /// climbs).
  CELL_BEHAVIOR_WEST_STONE_HEAD = 30,
  CELL_BEHAVIOR_MIDDLE_STONE_HEAD,
  CELL_BEHAVIOR_EAST_STONE_HEAD,
  /// The altar room's bushes: those ringing the altar, those over the Mad
  /// Plant and the one it hides under, the cobweb, and the ledge at the
  /// cobweb's north edge (a cell of the web for Cyclone).
  CELL_BEHAVIOR_ALTAR_BUSHES = 40,
  CELL_BEHAVIOR_MAD_PLANT_BUSHES,
  CELL_BEHAVIOR_COBWEB,
  CELL_BEHAVIOR_COBWEB_LEDGE,
  CELL_BEHAVIOR_MAD_PLANT,
  /// The lair: the steps just east of where Susa works, the floor before the
  /// Serpent, the cup of Dragonsbane at its feet, the Serpent itself, and the
  /// Cloud Brand's spot behind its tail.
  CELL_BEHAVIOR_BY_SUSA = 40,
  CELL_BEHAVIOR_BEFORE_SERPENT = 42,
  CELL_BEHAVIOR_CUP = 50,
  CELL_BEHAVIOR_SERPENT = 55,
  CELL_BEHAVIOR_SERPENT_WEST,
  CELL_BEHAVIOR_CLOUD_BRAND = 77
};

/// The maze's one actor, in the middle of each of its rooms.
enum {
  /// A sprig that Growth turns into an arrow pointing the way on
  /// (GaiaRockInside_PlantPointsTheWay).
  ACTOR_ARROW_PLANT = 8
};

/// The exit the lair would take after showing a beam if none waited to be
/// shown: the scene has no exit of this number, but the party only comes in by
/// entrance 20 from an event that has just let a beam in.
#define EXIT_NO_SUNBEAM 20

/// The trigger Susa's hook raises when the party has stood in his or his
/// cask's way for half a second (GaiaRockInside_CheckBlocked).
#define RAISED_SUSA_BLOCKED 42

/// The lair's entrances: its way in from the south, and where the battle with
/// the Serpent brings the party back: won, lost without the Dancing Idol, and
/// lost holding it before any beam of sunlight reaches the lair
/// (GaiaRockInside_ChallengeSerpent).
enum {
  ENTRANCE_LAIR = 9,
  ENTRANCE_SERPENT_BEATEN = 10,
  ENTRANCE_BEATEN_BY_SERPENT = 11,
  ENTRANCE_BEATEN_HOLDING_IDOL = 12
};

/// The Serpent's lair (SceneId_GaiaRockInsideMaze by entrances 9 to 20).
/// The Serpent sits on the sand at its north end; Susa works among the casks
/// south of it.
enum {
  ACTOR_SERPENT = 8,
  /// The cask of Dragonsbane Susa feeds the Serpent from: "How many casks
  /// does it take to weaken this stupid serpent?"
  ACTOR_CASK = 9,
  ACTOR_SUSA = 10,
  /// Kraden, who points Felix to the slate once the Serpent is beaten.
  ACTOR_KRADEN = 11,
  /// The slate that teaches Sand (GaiaRockInside_ReadSandTablet).
  ACTOR_SAND_TABLET = 12
};

/// The room of the first altar (SceneId_GaiaRockInsideFirstAltar): the two
/// great red faces that stand in the doorways either side of the altar until
/// the Dancing Idol dances on it, and the two stumps Felix Moves out of the
/// way on the path to the maze.
enum {
  ACTOR_FIRST_ALTAR_WEST_FACE = 8,
  ACTOR_FIRST_ALTAR_EAST_FACE = 9,
  ACTOR_STUMP_A = 10,
  ACTOR_STUMP_B = 11,
  ACTOR_FIRST_ALTAR = 12,
  /// The Dancing Idol, set on the altar while it dances.
  ACTOR_FIRST_ALTAR_DOLL = 13
};

/// The room of the twin altars (SceneId_GaiaRockInsideTwinAltars): a face on
/// the north wall over each altar. The east altar is the one that falls
/// through from the altar room when Cyclone tears its cobweb.
enum {
  ACTOR_WEST_FACE = 8,
  ACTOR_EAST_FACE = 9,
  ACTOR_WEST_ALTAR = 10,
  ACTOR_EAST_ALTAR = 11,
  ACTOR_TWIN_ALTARS_DOLL = 12
};

/// The nine leafy platforms Cyclone sets swaying in the platforms' room
/// (GaiaRockInside_StartPlatforms).
enum { ACTOR_FIRST_PLATFORM = 8, ACTOR_LAST_PLATFORM = 16 };

/// The altar room (SceneId_GaiaRockInsideAltar): the altar ringed by bushes in
/// its west part, with a face on the wall beside it, the wall of handholds
/// along its south part, and the altar on the great cobweb in its east part.
enum {
  ACTOR_ALTAR_FACE = 8,
  ACTOR_ALTAR = 9,
  ACTOR_HANGING_VINE = 10,
  /// The three stone heads that burst out of the wall of handholds under Felix
  /// (StoneHead_BurstOut).
  ACTOR_STATUE_A = 11,
  ACTOR_STATUE_B = 12,
  ACTOR_STATUE_C = 13,
  ACTOR_ALTAR_DOLL = 14,
  /// The altar the cobweb holds up; Cyclone tears the web and it falls a floor
  /// (GaiaRockInside_CycloneClearsCobweb).
  ACTOR_FALLING_ALTAR = 15,
  ACTOR_GANGWAY = 16
};

struct Actor;

/// A grid cell's step that marks the grid's border.
#define MAZE_BORDER 99

/// One grid cell: the step that stands on it (0 for none, MAZE_BORDER for the
/// border) and the column and row of the next step.
typedef struct {
  s8 step;
  s8 nextX;
  s8 nextY;
} PathCell;

/// The steps of the maze's path, from the way in to the way on.
#define MAZE_STEPS 14

/// One step of the path: its number, then the step found below, to the left,
/// to the right and above it.
typedef struct {
  u8 number;
  u8 spare;
  u8 next[4]; ///< the step each way leads to, by enum MazeWay
  u8 padding[2];
} PathStep;

/// The four ways out of a step, in the order PathStep.next keeps them.
enum MazeWay { MAZE_SOUTH, MAZE_WEST, MAZE_EAST, MAZE_NORTH };

/// PathStep.next's marks for the ways out of the maze, each the exit it takes:
/// back out from its first step, and on from its last.
#define MAZE_BACK 250
#define MAZE_EXIT 251

/// The maze's path: the step the party stands on, set to the first once the
/// path is laid, and the steps, from 1 to MAZE_STEPS.
typedef struct {
  s32 current;
  PathStep steps[MAZE_STEPS + 1];
} Path;

/// The maze's path, kept in the inventory snapshot's spare room.
#define gPath ((Path *)(gInventorySnapshot + 648))

/// Set once the plant on a step of the maze has pointed the way on; it points
/// again whenever the party comes back to that step.
#define FLAG_MAZE_PLANT_POINTED(step) (0xa50 + (step))
/// The maze's path is laid for this visit (GaiaRockInside_LayMaze).
#define FLAG_MAZE_LAID FLAG_AREA_TEMP_F0
/// The plant of the step the party stands on points the way: Growth on it
/// does nothing more.
#define FLAG_PLANT_POINTING FLAG_SCENE_TEMP_00

/// A map grid cell, its column and row.
struct CellPoint {
  s16 position[2];
};

/// The platforms' sway (GaiaRockInside_SwayPlatforms).
struct SwayState {
  u16 frame;   ///< frames into the sway's 240
  u16 pending; ///< frames still to run, which a push holds back
};

extern u32 gGaiaRockInside_BurstScript[];
extern s32 gGaiaRockInside_CaskTossedLeftScript[];
extern s32 gGaiaRockInside_CaskTossedRightScript[];

extern struct ActorPlacement gGaiaRockInside_Placements[];
extern struct ActorPlacement gGaiaRockInside_LairPlacements[];
extern struct ActorPlacement gGaiaRockInside_TwinAltarsActorPlacements[];
extern struct ActorPlacement gGaiaRockInside_PlatformsActorPlacements[];
extern struct ActorPlacement gGaiaRockInside_AltarActorPlacements[];
extern s32 gGaiaRockInside_SquashYScript[];

void MadPlant_LeapAtLeader(s32 a, s32 b, s32 c);

extern u32 gGaiaRockInside_LeaderStepsBackScript[];
extern s32 gGaiaRockInside_AltarShakesScript[];
extern s32 gGaiaRockInside_AltarDropsScript[];

struct Actor gGaiaRockInside_SavedSusa BSS_DATA;
s32 gGaiaRockInside_SavedSusaX BSS_DATA;
s32 gGaiaRockInside_SavedSusaZ BSS_DATA;
s32 gGaiaRockInside_SavedSusaAnimation BSS_DATA;
s32 gGaiaRockInside_SavedSusaAnimationStep BSS_DATA;

extern s32 gGaiaRockInside_CaskThrownScript[];
extern s32 gGaiaRockInside_CaskEmptiesScript[];

void GaiaRockInside_SaveSusa(void);
void GaiaRockInside_RestoreSusa(void);

void Actor_Release(struct Actor *actor);

s32 Scene_TalkToActor();

void GaiaRockInside_PlacePlatform(s32 id, s32 x, s32 z);

void GaiaRockInside_SetPlatformMode(u32 id, s8 mode);
void GaiaRockInside_SwayPlatforms(void);

void GaiaRockInside_BlankLayer(s32 layer, s32 keep);

extern u16 gGaiaRockInside_LairBlendScript[12];

void GaiaRockInside_LayMaze(s32 level);
void GaiaRockInside_DrawMazeRoom(void);
s32 GaiaRockInside_GetMazeWayOn(void);

void Map_CopyRawCellsRect(s32 x, s32 z, s32 width, s32 height, s32 cellX,
                          s32 cellZ);

/// Copies whole cells as the map stores them, from an event.
static inline void MapGrid_CopyRawCells(s32 x, s32 z, s32 width, s32 height,
                                        s32 cellX, s32 cellZ) {
  Map_CopyRawCellsRect(x, z, width, height, cellX, cellZ);
}

extern u32 gFieldEffect_ShortScript[];
extern u32 gFieldEffect_MediumScript[];
extern u32 gFieldEffect_LongScript[];

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

extern struct SceneRegion gGaiaRockInside_AltarExitRegions[];

const struct SceneRegion *Scene_GetExitRegions(void) {
  if (gMainRoutineWork.next == SCENE_GAIA_ROCK_INSIDE_ALTAR)
    return gGaiaRockInside_AltarExitRegions;
  return 0;
}

/// An actor springs down off the ledge before it (Actor_JumpDownLedge).
static __inline__ void GaiaRockInside_JumpDownLedge(s32 id, s32 pose,
                                                    s32 param) {
  Actor_JumpDownLedge(id, pose, param);
}

/// Felix walks south off the ledge at the north edge of the altar room's
/// cobweb pit, cell (44, 20), springs down past the altar through the web and
/// drops out of sight; he lands in the twin altars' room below in a ring of
/// dust (GaiaRockInside_RaiseDust). Once the altar has fallen he jumps into the
/// empty pit the same way.
void GaiaRockInside_JumpDownCobwebPit(void) {
  FieldActor_SetSpritePriority(gMainRoutineWork.leaderActorId, 3);
  OverlayObject_PrepareObject(toFIXED(712), 0, toFIXED(468), SPRITE_CRATE);
  OverlayObject_PrepareObject(toFIXED(712), 0, toFIXED(490), SPRITE_CRATE);
  // 0x800 is no colour the dust's byte keeps, and not the bit that skips the
  // wait for the landing, which Taopo Swamp's and Ankohl Ruins' drops like this
  // one pass: the jump goes as with 0.
  GaiaRockInside_JumpDownLedge(gMainRoutineWork.leaderActorId,
                               ANIM_LEADER_SPRING, 0x800);
  Event_PlaySound(SE_FALL);
  Event_SetFlag(FLAG_ARRIVE_FALLING);
  FieldEvent_SetExitRequest(
      GAIA_ROCK_INSIDE_ALTAR_TO_GAIA_ROCK_INSIDE_TWIN_ALTARS_SOUTHEAST_A_B);
}

// Includes the shared FieldEffect_AdvanceMotion in this scene.
// Neither it nor its caller GaiaRockInside_RaiseDust exists in the Japanese
// scene.
#if TLA_PATCH_1
#include "../field/field_effect_advance_motion.c"
#endif

#if TLA_PATCH_1
/// Felix lands in the twin altars' room after jumping down the altar room's
/// cobweb pit (GaiaRockInside_JumpDownCobwebPit): he drops in from 130 pixels
/// up, east of the east altar's face, into a ring of dust with a thump that
/// shakes the room, and sits a moment before he stands. The Japanese edition
/// has no form of this function here.
void GaiaRockInside_RaiseDust(void) {
  struct Actor *actor = Actor_Get(gMainRoutineWork.leaderActorId);
  u32 i;
  struct EffectOptions options;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  MapGrid_CenterOnFollowedPoint();
  System_Wait(1);
  actor->y.fixed = toFIXED(130);
  actor->gravity = toFIXED(1) / 2;
  actor->bounce = 0;
  actor->motionFlags = 0;
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_PlaySound(SE_DUST_PUFF);
  actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  FieldEvent_Wait(24);
  options.palette = SPRITE_COLOR_WHITE;
  options.update = FieldEffect_AdvanceMotion;
  options.startScaleX = FIXED_ONE * 16 / 25;
  options.startScaleY = FIXED_ONE * 16 / 25;
  for (i = 0; i <= DUST_RING_PUFFS; i++) {
    s32 angle;
    FIXED velocity[3];
    FIXED cosine;

    angle = i * FACING_STEP;
    cosine = Trig_Cos(angle);
    velocity[0] = cosine;
    velocity[1] = 0;
    velocity[2] = Math_Sin(angle);
    velocity[0] += velocity[0] >> 1;
    velocity[0] += (Random_Below(toFIXED(3) / 4)) - toFIXED(3) / 8;
    velocity[2] += (Random_Below(toFIXED(3) / 8)) - toFIXED(3) / 16;
    FieldEffect_Spawn(actor->x.fixed, actor->y.fixed, actor->z.fixed,
                      velocity[0], velocity[1], velocity[2],
                      EFFECT_USE_UPDATE | EFFECT_USE_START_SCALE |
                          EFFECT_USE_PALETTE | 1,
                      &options);
  }
  Event_PlaySound(SE_THUMP);
  Actor_SetAttachedEffect(gMainRoutineWork.leaderActorId,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_LEADER_SIT);
  MapGrid_SetShake(5, 5, 1);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  FieldMapGrid_WaitForShakeToSettle();
  Actor_SetAttachedEffect(gMainRoutineWork.leaderActorId,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_REMOVE);
  actor->gravity = toFIXED(1);
  actor->bounce = FIXED_ONE / 4;
  FieldEvent_Wait(10);
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_STAND);
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  Event_End();
}
#endif

const u32 *Scene_GetExits(void) { return gSceneExits; }

/// Susa feeds the Serpent Dragonsbane cask by cask, over and over
/// (gGaiaRockInside_SusaFeedsSerpentScript): he sets a cask down among the
/// others south of the Serpent, pushes it a cell east, and throws it up onto
/// the cup at the Serpent's feet, where it tips and pours; then he tosses the
/// empty cask aside, where it bursts. The routines below are its steps.
///
/// The cask lands and holds still on its third animation.
s32 GaiaRockInside_CaskRests(void) {
  struct Actor *cask;

  cask = Actor_Get(ACTOR_CASK);
  Actor_SelectAnimation(cask, 3);
  Actor_SetAnimationSpeed(cask, 0);
  return 0;
}

/// The cask tips a step over the cup.
s32 GaiaRockInside_CaskTips(void) {
  struct Actor *actor;

  actor = Actor_Get(ACTOR_CASK);
  ((struct FieldSprite *)actor->sprite)->rotation += TRIG_TURN * 3 / 256;
  return 0;
}

extern u32 gGaiaRockInside_PourScript[];

/// The cask rights itself a step.
s32 GaiaRockInside_CaskRights(void) {
  struct FieldSprite *sprite = Actor_Get(ACTOR_CASK)->sprite;

  sprite->rotation -= TRIG_TURN * 3 / 256;
  return 0;
}

/// Each frame the pour's size varies at random; shrunk below a sixteenth, it
/// jumps back to an eighth.
void GaiaRockInside_FlickerPour(struct Actor *actor) {
  u32 random = Random16();
  s32 scale = actor->scaleX;
  s32 bias;
  s32 limit;

  bias = -FIXED_ONE / 32;
  scale += FIXEDtoINT(random << 12);
  scale += bias;
  actor->scaleX = scale;
  actor->scaleY = actor->scaleY + ((Random16() << 12) >> 16) + bias;
  scale = actor->scaleX;
  limit = FIXED_ONE / 16 - 1;
  if (scale <= limit)
    actor->scaleX = FIXED_ONE / 8;
  if (actor->scaleY <= limit)
    actor->scaleY = FIXED_ONE / 8;
}

/// The cask pours: a spray of Dragonsbane that flickers in size
/// (GaiaRockInside_FlickerPour).
s32 GaiaRockInside_CaskPours(void) {
  struct Actor *source = Actor_Get(ACTOR_CASK);
  struct Actor *actor;
  struct FieldSprite *sprite;

  actor = (FieldActor_Create(SPRITE_WADING_SPLASH, source->x.fixed + toFIXED(8),
                             source->y.fixed - toFIXED(2), source->z.fixed));
  actor->motionFlags = 0;
  Actor_SetSpriteFlags(actor, 0);
  Actor_StartScript(actor, gGaiaRockInside_PourScript);
  actor->scaleX = FIXED_ONE * 2 / 5;
  actor->scaleY = FIXED_ONE * 2 / 5;
  sprite = actor->sprite;
  sprite->priority = 1;
  actor->bounce = 0;
  actor->riseBaseY = 0;
  actor->update = (ObjectUpdate)GaiaRockInside_FlickerPour;
  return 0;
}

extern s32 gGaiaRockInside_CaskSetDownScript[];

/// Susa sets a new cask down at his feet.
s32 GaiaRockInside_SetDownCask(void) {
  Actor_StartScript(((s32)Actor_Get(ACTOR_CASK)),
                    (s32)gGaiaRockInside_CaskSetDownScript);
  return 0;
}

extern u32 gGaiaRockInside_CaskPushedScript[];

/// Susa pushes the cask a cell east, straining.
s32 GaiaRockInside_SusaPushesCask(void) {
  Actor_StartScript(((s32)Actor_Get(ACTOR_CASK)),
                    (s32)gGaiaRockInside_CaskPushedScript);
  Actor_SetAttachedEffect(ACTOR_SUSA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  return 0;
}

/// Susa throws the cask up onto the cup at the Serpent's feet.
s32 GaiaRockInside_SusaThrowsCask(void) {
  struct Actor *actor;

  actor = Actor_Get(ACTOR_CASK);
  Actor_StartScript(actor, (s32)gGaiaRockInside_CaskThrownScript);
  actor->x.fixed = toFIXED(472);
  Actor_SetAnimationSpeed(actor, ANIM_STEP_NORMAL);
  Actor_SetAttachedEffect(ACTOR_SUSA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  return 0;
}

/// On the cup the cask tips, pours and rights itself again.
s32 GaiaRockInside_CaskEmpties(void) {
  struct Actor *cask;

  cask = Actor_Get(ACTOR_CASK);
  Actor_StartScript(cask, (s32)gGaiaRockInside_CaskEmptiesScript);
  Actor_SetAnimationSpeed(cask, 0);
  Actor_SetAttachedEffect(ACTOR_SUSA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  return 0;
}

#include "../field/field_effect_advance_motion_priority.c"

#include "text/msg_ids.h"

/// The tossed cask bursts in four puffs of dust.
s32 GaiaRockInside_CaskBursts(void) {
  struct Actor *source = Actor_Get(ACTOR_CASK);
  s32 i;

  Event_PlaySound(SE_FLAME_FLARE);
  for (i = 0; i < 4; i++) {
    {
      struct Actor *dust =
          (FieldActor_Create(SPRITE_DUST_CLOUD, source->x.fixed,
                             source->y.fixed, source->z.fixed));

      if (dust != 0) {
        dust->motionFlags = 0;
        dust->collisionFlags = 0;
        Actor_SetSpriteFlags(dust, 0);
        Actor_SelectAnimation(dust, 1);
        Actor_StartScript(dust, gGaiaRockInside_BurstScript);
        ((struct FieldSprite *)dust->sprite)->priority = 1;
        dust->bounce = Trig_Cos(((Random16() << 3) >> 16) << 14) >> 1;
        dust->riseBaseY = Math_Sin(((Random16() << 3) >> 16) << 14) >> 1;
        dust->gravity = 0;
        dust->timer = ((Random16() << 9) >> 16) - 256;
        dust->speedLimit = 0;
        dust->acceleration = 0;
        dust->update = Effect_AdvanceMotionAtLeaderPriority;
      }
    }
  }
  return 0;
}

/// The empty cask is tossed aside, west or east, away from where the leader
/// stands.
s32 GaiaRockInside_TossCask(void) {
  struct Actor *actor = Actor_Get(ACTOR_CASK);
  struct Actor *leader = Actor_Get(gMainRoutineWork.leaderActorId);

  if (FIELD_TILE(leader->z.fixed) <= 45 && FIELD_TILE(leader->x.fixed) <= 57)
    Actor_StartScript(actor, gGaiaRockInside_CaskTossedRightScript);
  else
    Actor_StartScript(actor, gGaiaRockInside_CaskTossedLeftScript);
  return 0;
}

/// The frames Susa or the cask may be held up by the party before he tells it
/// to move, and the most the count keeps.
enum { SUSA_BLOCKED_FRAMES = 30, SUSA_BLOCKED_FRAMES_MAX = 35 };

/// Susa's hook while he works: when the party has stood in his or the cask's
/// way for SUSA_BLOCKED_FRAMES, he tells them to move
/// (GaiaRockInside_SusaSaysMoveAside); while one is held up the other waits.
void GaiaRockInside_CheckBlocked(struct Actor *actor) {
  struct Actor *other = Actor_Get(ACTOR_CASK);

  if (actor->refused == SUSA_BLOCKED_FRAMES ||
      other->refused == SUSA_BLOCKED_FRAMES) {
    gFieldWork->raisedTrigger = RAISED_SUSA_BLOCKED;
    if (actor->refused == SUSA_BLOCKED_FRAMES)
      actor->refused = SUSA_BLOCKED_FRAMES + 1;
    if (other->refused == SUSA_BLOCKED_FRAMES)
      other->refused = SUSA_BLOCKED_FRAMES + 1;
  }
  if (actor->refused > 2) {
    other->paused = 1;
    Event_SetFlag(FLAG_CASK_WAITS_FOR_SUSA);
  }
  {
    u8 *otherCount = &other->refused;

    if (*otherCount > 2 && !Event_GetFlag(FLAG_CASK_WAITS_FOR_SUSA) &&
        other->script != gGaiaRockInside_CaskTossedLeftScript &&
        other->script != gGaiaRockInside_CaskTossedRightScript) {
      actor->paused = 1;
      Event_SetFlag(FLAG_SUSA_WAITS_FOR_CASK);
    }
    if (actor->refused == 0) {
      other->paused = 0;
      Event_ClearFlag(FLAG_CASK_WAITS_FOR_SUSA);
    }
    if (*otherCount == 0) {
      actor->paused = 0;
      Event_ClearFlag(FLAG_SUSA_WAITS_FOR_CASK);
    }
    if (actor->refused > SUSA_BLOCKED_FRAMES_MAX)
      actor->refused = SUSA_BLOCKED_FRAMES_MAX;
    if (*otherCount > SUSA_BLOCKED_FRAMES_MAX)
      *otherCount = SUSA_BLOCKED_FRAMES_MAX;
  }
}

extern struct ActorPlacement gGaiaRockInside_FirstAltarActorPlacements[];

extern s32 gGaiaRockInside_ArrowFrame;
extern s32 gGaiaRockInside_SquashXScript[];

/// Returns the current room's actors; the maze's rooms and the lair share a
/// scene, told apart by entrance.
const void *Scene_GetActorPlacements(void) {
  if (gMainRoutineWork.next == SCENE_GAIA_ROCK_INSIDE_FIRST_ALTAR)
    return gGaiaRockInside_FirstAltarActorPlacements;
  if (gMainRoutineWork.next == SCENE_GAIA_ROCK_INSIDE_TWIN_ALTARS)
    return gGaiaRockInside_TwinAltarsActorPlacements;
  if (gMainRoutineWork.next == SCENE_GAIA_ROCK_INSIDE_PLATFORMS)
    return gGaiaRockInside_PlatformsActorPlacements;
  if (gMainRoutineWork.next == SCENE_GAIA_ROCK_INSIDE_ALTAR)
    return gGaiaRockInside_AltarActorPlacements;
  if (gMainRoutineWork.entrance > 8)
    return gGaiaRockInside_LairPlacements;
  return gGaiaRockInside_Placements;
}

/// The arrow plant's animation for the heading of the way on: east, south,
/// west and north from animation 2; and whether that heading is south or
/// north, the arrow up or down.
#define ARROW_ANIMATION(heading) ((((heading) & 0xc000) >> 14) + 2)
#define ARROW_UP_OR_DOWN(heading) ((heading) & 0x4000)

/// The update of a maze plant Growth has turned into an arrow: it flashes, and
/// about twice a second stretches along its arrow with a drip, sideways here
/// and up and down in GaiaRockInside_PulseUpDownArrow.
void GaiaRockInside_PulseSidewaysArrow(struct Actor *actor) {
  s32 value = 0;

  if ((gGaiaRockInside_ArrowFrame & 7) <= 0)
    value = 7;
  Actor_SetSpriteColorMode(actor, value & 7);
  if ((gGaiaRockInside_ArrowFrame & 31) == 0) {
    actor->scaleX = FIXED_ONE;
    Actor_StartScript(actor, gGaiaRockInside_SquashXScript);
    Event_PlaySound(SE_DRIP);
  }
  gGaiaRockInside_ArrowFrame++;
}

/// As GaiaRockInside_PulseSidewaysArrow, for an arrow pointing up or down.
void GaiaRockInside_PulseUpDownArrow(struct Actor *actor) {
  s32 value = 0;

  if ((gGaiaRockInside_ArrowFrame & 7) <= 0)
    value = 7;
  Actor_SetSpriteColorMode(actor, value & 7);
  if ((gGaiaRockInside_ArrowFrame & 31) == 0) {
    actor->scaleY = FIXED_ONE;
    Actor_StartScript(actor, gGaiaRockInside_SquashYScript);
    Event_PlaySound(SE_DRIP);
  }
  gGaiaRockInside_ArrowFrame++;
}

/// Felix casts Growth on the little plant in the middle of a maze room, and it
/// grows into a green arrow that flashes and pulses, pointing the way on to
/// the next room. It points again whenever the party comes back to that room
/// on this visit.
void GaiaRockInside_PlantPointsTheWay(s32 unused, u32 id) {
  struct Actor *actor = Actor_Get(id);
  u32 direction;
  s32 *state;

  direction = GaiaRockInside_GetMazeWayOn();
  state = &gPath->current;
  actor->collisionFlags = 0;
  Actor_SelectAnimation(actor, ARROW_ANIMATION(direction));
  if (ARROW_UP_OR_DOWN(direction))
    actor->update = (ObjectUpdate)GaiaRockInside_PulseUpDownArrow;
  else
    actor->update = (ObjectUpdate)GaiaRockInside_PulseSidewaysArrow;
  actor->collisionFlags = 1;
  Event_SetFlag(FLAG_MAZE_PLANT_POINTED(*state));
  Event_SetFlag(FLAG_PLANT_POINTING);
  System_Wait(20);
}

/// Felix casts Cyclone on the cross of five bushes south-east of the east face
/// in the twin altars' room, around cell (38, 25), where a beam of
/// sunlight shines down from the ceiling. Once the middle bush is gone the beam
/// falls through the hole it hid, cell (38, 25), and down into the Serpent's
/// lair; Felix hops aside if he stands on the hole. While the Serpent lives,
/// the game first shows the beam falling into the lair.
void GaiaRockInside_CycloneClearsTwinAltarsBushes(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.leaderActorId);

  FieldCyclone_CopyTouchedCells(37, 26, 0, 3, 3, 61, 0);
  if (FieldMapGrid_GetCellBehaviorAt(2, toFIXED(608), toFIXED(400)) == 0) {
    MapGrid_CopyCellAttributes(36, 23, 1, 1, 38, 25);
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    if (FIELD_CELL(leader->x.fixed) == 38 &&
        FIELD_CELL(leader->z.fixed) == 25) {
      Actor_SetAttachedEffect(gMainRoutineWork.leaderActorId,
                              ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
      Actor_SetDestinationOffset(gMainRoutineWork.leaderActorId, 16, 0);
      leader->velocityY = toFIXED(6);
      FieldActor_WaitForMoveById(gMainRoutineWork.leaderActorId);
    }
    if (!Event_GetFlag(FLAG_SERPENT_SLAIN)) {
      Event_SetFlag(FLAG_BUSHES_SUNBEAM_TO_SHOW);
      FieldEvent_Wait(30);
      ObjectTable_Snapshot();
      FieldEvent_SetExitRequest(
          GAIA_ROCK_INSIDE_TWIN_ALTARS_TO_GAIA_ROCK_INSIDE_MAZE);
    } else {
      Event_SetFlag(FLAG_BUSHES_SUNBEAM);
      Event_End();
    }
  }
}

static __inline__ s32 GameFlag_Test_(s32 flag) { return GameFlag_Get(flag); }

/// A Mad Plant leaps out at the leader (MadPlant_LeapAtLeader).
static __inline__ void GaiaRockInside_MadPlantLeaps(s32 sprite, s32 x, s32 z) {
  MadPlant_LeapAtLeader(sprite, x, z);
}

/// Felix casts Cyclone on the patch of bushes in the platforms' room, cells
/// (16, 25) to (20, 29): they blow away, and the first time nine leafy
/// platforms appear over the room's holes and start to sway, which carry the
/// party across.
void GaiaRockInside_CycloneStartsPlatforms(void) {
  FieldCyclone_CopyTouchedCells(16, 25, 0, 5, 5, 40, 8);
  if (GameFlag_Test_(FLAG_PLATFORMS_SWAYING) == 0) {
    Event_SetFlag(FLAG_PLATFORMS_SWAYING);
    GaiaRockInside_StartPlatforms(2);
  }
}

/// Felix casts Cyclone on the bushes north-west of the altar in the altar
/// room, at the south-east bush of the patch, cell (20, 21): they blow away,
/// and until it is beaten a Mad Plant hiding there leaps out at him and a
/// battle starts that the party cannot run from.
void GaiaRockInside_CycloneMadPlant(void) {
  FieldCyclone_CopyTouchedCells(18, 17, 0, 3, 3, 31, 0);
  if (Event_GetFlag(FLAG_MAD_PLANT(2)) == 0) {
    GaiaRockInside_MadPlantLeaps(SPRITE_MAD_PLANT, 328, 344);
    gMainRoutineWork.pendingDjinni = IF_FLAG_SET(FLAG_MAD_PLANT(2));
    Battle_SetType(BATTLE_TYPE_NO_ESCAPE);
    Field_RequestBattleFromEncounterTable(ENCOUNTER_TABLE_MAD_PLANTS, 2);
  }
}

/// Felix casts Cyclone on the bushes ringing the altar in the altar room's west
/// part, cells (19, 24) to (22, 26), and they blow away, leaving the altar
/// clear.
void GaiaRockInside_CycloneClearsAltarBushes(void) {
  FieldCyclone_CopyTouchedCells(19, 24, 0, 4, 3, 31, 4);
}

/// Felix casts Cyclone on the Mad Plant's bushes, north-west of the altar in
/// the altar room, away from the south-east bush it hides under, cells (18, 19)
/// to (20, 21): they blow away, and no Mad Plant leaps out
/// (GaiaRockInside_CycloneMadPlant).
void GaiaRockInside_CycloneClearsMadPlantBushes(void) {
  FieldCyclone_CopyTouchedCells(18, 17, 0, 3, 3, 31, 0);
}

extern struct CellPoint gGaiaRockInside_CobwebCells[12];

/// Felix casts Cyclone on the great cobweb in the altar room's east part, at
/// any of its edge cells around (40, 20) to (47, 27). The web tears; the
/// altar on it shakes, Felix steps aside if he stands just south of it, and
/// the altar drops into the pit. The game then shows it landing in the twin
/// altars' room below as the east altar, and brings the party back to the
/// pit's edge.
void GaiaRockInside_CycloneClearsCobweb(void) {
  struct Actor *guard;
  struct Actor *leader;
  s16 count;
  s16 i;
  s32 height = 8;
  s32 sourceX = 117;
  s32 sourceY = 20;

  Cyclone_CopyTouchedCells(104, sourceY, 1, height, height, sourceX, sourceY);
  if (GameFlag_Get(FLAG_ALTAR_FELL))
    return;
  count = 0;
  for (i = 0; i < (s16)ARRAY_COUNT(gGaiaRockInside_CobwebCells); i++) {
    if (MapGrid_GetCellBehaviorAt(
            2, FIELD_CELL_ORIGIN(gGaiaRockInside_CobwebCells[i].position[0]),
            FIELD_CELL_ORIGIN(gGaiaRockInside_CobwebCells[i].position[1])) == 0)
      count++;
  }
  if (count <= 0)
    return;
  guard = Actor_Get(ACTOR_FALLING_ALTAR);
  leader = Actor_Get(gMainRoutineWork.leaderActorId);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  if (FIELD_TILE(leader->z.fixed) > 53 && FIELD_TILE(leader->x.fixed) > 83 &&
      FIELD_TILE(leader->x.fixed) <= 91) {
    leader->speedLimit = toFIXED(2);
    Actor_ShowEmote(gMainRoutineWork.leaderActorId,
                    EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    Actor_SetScript(gMainRoutineWork.leaderActorId,
                    gGaiaRockInside_LeaderStepsBackScript);
  }
  Actor_SetScript(ACTOR_FALLING_ALTAR, gGaiaRockInside_AltarShakesScript);
  FieldEvent_Wait(60);
  Actor_SetScript(ACTOR_FALLING_ALTAR, gGaiaRockInside_AltarDropsScript);
  FieldEvent_Wait(30);
  MapGrid_CopyCellsLayered(117, 20, 7, 8, 104, 20);
  MapGrid_CopyCellsLayered(111, 12, 1, 6, 111, 20);
  MapGrid_CopyCellAttributes(49, 84, 8, 8, 40, 84);
  FieldEvent_Wait(30);
  OverlayObject_PrepareObject(toFIXED(700), 0, toFIXED(468), SPRITE_CRATE);
  OverlayObject_PrepareObject(toFIXED(700), 0, toFIXED(490), SPRITE_CRATE);
  OverlayObject_PrepareObject(toFIXED(724), 0, toFIXED(468), SPRITE_CRATE);
  OverlayObject_PrepareObject(toFIXED(724), 0, toFIXED(490), SPRITE_CRATE);
  Event_PlaySound(SE_DUST_PUFF);
  guard->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  ((struct FieldSprite *)guard->sprite)->priority = 3;
  FieldEvent_Wait(10);
  Actor_SetPosition(ACTOR_FALLING_ALTAR, 0, 0);
  ObjectTable_Snapshot();
  Event_SetFlag(FLAG_ALTAR_FELL);
  FieldEvent_SetExitRequest(
      GAIA_ROCK_INSIDE_ALTAR_TO_GAIA_ROCK_INSIDE_TWIN_ALTARS_SOUTHEAST_B);
}

/// Felix stands in the way of Susa or the cask he is pushing in the lair for
/// half a second, and Susa says "Move out of the way there."
void GaiaRockInside_SusaSaysMoveAside(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgGaiaRockInsideMoveOutOfTheWay);
  // "Move out of the way there."
  Event_ShowMessage(ACTOR_SUSA, 0);
  Event_End();
}

/// Stops Susa and the cask he is handling where they are, keeping Susa's
/// place and animation to be given back after a talk.
void GaiaRockInside_SaveSusa(void) {
  struct Actor *walker = Actor_Get(ACTOR_CASK);
  struct Actor *actor = Actor_Get(ACTOR_SUSA);
  struct FieldSprite *sprite = actor->sprite;
  struct SpriteAnimation *animation = sprite->record;

  Iwram_CopyWordsEntry(&gGaiaRockInside_SavedSusa, actor, sizeof(struct Actor));
  gGaiaRockInside_SavedSusaAnimationStep = animation->step;
  gGaiaRockInside_SavedSusaAnimation = sprite->lastNo;
  {
    FIXED x = actor->x.fixed;
    FIXED z;

    gGaiaRockInside_SavedSusaX = x;
    z = actor->z.fixed;
    actor->update = 0;
    gGaiaRockInside_SavedSusaZ = z;
    actor->velocityZ = 0;
    actor->velocityY = 0;
    actor->velocityX = 0;
    actor->targetX = x;
    actor->targetY = actor->y.fixed;
    actor->targetZ = z;
  }
  Actor_ResetTargetsAndVelocity(ACTOR_SUSA);
  Actor_SetAnimationSpeed(actor, ANIM_STEP_NORMAL * 5 / 8);
  actor->speedLimit = toFIXED(1);
  if (walker->script == gGaiaRockInside_CaskThrownScript ||
      walker->script == gGaiaRockInside_CaskPushedScript) {
    walker->paused = 1;
    Actor_SetAnimationSpeed(walker, 0);
  }
}

/// After a talk, Susa walks back to where GaiaRockInside_SaveSusa found him and
/// takes up his animation, and the cask goes on.
void GaiaRockInside_RestoreSusa(void) {
  struct Actor *walker = Actor_Get(ACTOR_CASK);
  struct Actor *actor = Actor_Get(ACTOR_SUSA);

  if (gGaiaRockInside_SavedSusaX != actor->x.fixed ||
      gGaiaRockInside_SavedSusaZ != actor->z.fixed) {
    s16 *savedZ = (s16 *)&gGaiaRockInside_SavedSusaZ;

    Actor_WalkTo(ACTOR_SUSA, FIXEDtoINT(gGaiaRockInside_SavedSusaX), savedZ[1]);
    FieldActor_WaitForMoveById(ACTOR_SUSA);
  }
  if (walker->script == gGaiaRockInside_CaskThrownScript ||
      walker->script == gGaiaRockInside_CaskPushedScript) {
    walker->paused = 0;
    Actor_SetAnimationSpeed(walker, ANIM_STEP_NORMAL);
  }
  Iwram_CopyWordsEntry(actor, &gGaiaRockInside_SavedSusa, sizeof(struct Actor));
  Actor_SetAnimation(ACTOR_SUSA, gGaiaRockInside_SavedSusaAnimation);
  Actor_SetAnimationSpeed(actor, gGaiaRockInside_SavedSusaAnimationStep);
}

/// Felix reaches the steps just east of where Susa works in the lair, cell
/// (32, 28), the first time. Susa turns on him ("Who are you? How did you get
/// here?"), explains he is getting the Serpent to drink Dragonsbane while the
/// view pans to it, tells the party to stand aside, and goes back to work. If
/// he was walking back from tossing a cask, he takes up where he was going.
void GaiaRockInside_MeetSusa(void) {
  struct Actor *actor = Actor_Get(ACTOR_CASK);

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgGaiaRockInsideWhoAreYouHowDid);
  EventCamera_FollowActor(ACTOR_SUSA, 1);
  if (actor->script == gGaiaRockInside_CaskEmptiesScript ||
      actor->script == gGaiaRockInside_CaskPushedScript)
    FieldActor_WaitForScriptEndById(ACTOR_CASK);
  GaiaRockInside_SaveSusa();
  Actor_SetAnimation(ACTOR_SUSA, ANIM_STAND);
  Actor_FaceActor(ACTOR_SUSA, gMainRoutineWork.leaderActorId, 0);
  Actor_ShowEmote(ACTOR_SUSA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  FieldEvent_Wait(30);
  // "Who are you? How did you get here?"
  Event_ShowMessage(ACTOR_SUSA, 0);
  Actor_WalkTo(ACTOR_SUSA, 496, 456);
  FieldActor_WaitForMoveById(ACTOR_SUSA);
  Actor_FaceDirection(ACTOR_SUSA, FACING_EAST_SOUTHEAST, 0);
  FieldEvent_Wait(10);
  // "Only Uzume, Kushinada, and I possess the ability to come here."
  Event_ShowMessage(ACTOR_SUSA, 0);
  Actor_ShowEmote(ACTOR_SUSA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  FieldEvent_Wait(45);
  // "Never mind. I don't have time to talk. I have my hands full getting..."
  Event_ShowMessage(ACTOR_SUSA, 0);
  Actor_FaceDirection(ACTOR_SUSA, FACING_NORTH_NORTHEAST, 0);
  FieldEvent_Wait(30);
  FieldCamera_MoveTo(488, CAMERA_KEEP, 392, 1);
  FieldCamera_WaitForArrival();
  // "Once it's groggy, that's when I'll strike..."
  Event_ShowMessage(ACTOR_SUSA, 0);
  EventCamera_FollowActor(ACTOR_SUSA, 1);
  FieldCamera_WaitForArrival();
  Actor_FaceDirection(ACTOR_SUSA, FACING_EAST_SOUTHEAST, 0);
  FieldEvent_Wait(10);
  // "If you understand, then get out of my way. Stand over there."
  Event_ShowMessage(ACTOR_SUSA, 0);
  Actor_SetAnimationAndWait(ACTOR_SUSA, ANIM_NOD);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  Event_SetFlag(FLAG_MET_SUSA);
  if (actor->script == gGaiaRockInside_CaskTossedLeftScript ||
      actor->script == gGaiaRockInside_CaskTossedRightScript) {
    gGaiaRockInside_SavedSusa.x.fixed = gGaiaRockInside_SavedSusaX =
        gGaiaRockInside_SavedSusa.targetX;
    gGaiaRockInside_SavedSusa.z.fixed = gGaiaRockInside_SavedSusaZ =
        gGaiaRockInside_SavedSusa.targetZ;
  }
  GaiaRockInside_RestoreSusa();
  Event_End();
}

/// Felix talks to Susa in the lair. While Susa feeds the Serpent he stops,
/// turns and tells the party to stand aside ("If you understand, then get out
/// of my way."). Once he has attacked it and lies hurt, he says the Serpent is
/// still too strong, or, once any beam of sunlight reaches the lair, "I'm done
/// for... That dragon has finished me off...". While the party is in his way
/// nothing happens (GaiaRockInside_SusaSaysMoveAside speaks instead).
void GaiaRockInside_TalkToSusa(void) {
  if (Actor_Get(ACTOR_SUSA)->refused > 2)
    return;
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  if (Event_GetFlag(FLAG_SUSA_ATTACKED_SERPENT)) {
    if (Event_GetFlag(FLAG_ALTAR_ROOM_SUNBEAM) ||
        Event_GetFlag(FLAG_WEST_ALTAR_SUNBEAM) ||
        Event_GetFlag(FLAG_EAST_ALTAR_SUNBEAM) ||
        Event_GetFlag(FLAG_BUSHES_SUNBEAM))
      // "I'm done for... That dragon has finished me off..."
      FieldEvent_SetMessageId((s32)MsgGaiaRockInsideImDoneForThatDragon);
    else
      // "The serpent is still too strong. If only there were a way to weaken
      // it further..."
      FieldEvent_SetMessageId((s32)MsgGaiaRockInsideTheSerpentIsStillToo);
    Event_ShowMessage(ACTOR_SUSA, 0);
  } else {
    if (Actor_Get(ACTOR_CASK)->script == gGaiaRockInside_CaskEmptiesScript)
      FieldActor_WaitForScriptEndById(ACTOR_CASK);
    GaiaRockInside_SaveSusa();
    FieldEvent_SetMessageId((s32)MsgGaiaRockInsideIfYouUnderstandThenGet);
    Actor_FaceActor(ACTOR_SUSA, gMainRoutineWork.leaderActorId, 0);
    FieldEvent_Wait(10);
    // "If you understand, then get out of my way. Stand over there."
    Event_ShowMessage(ACTOR_SUSA, 0);
    Actor_SetAnimationAndWait(ACTOR_SUSA, ANIM_NOD);
    FieldEvent_Wait(10);
    GaiaRockInside_RestoreSusa();
  }
  Event_End();
}

/// Felix reads Susa's mind in the lair. While Susa works: "How many casks does
/// it take to weaken this stupid serpent?"; once the party holds the Dancing
/// Idol: "Who do they think they are? I'm the only one who can save
/// Kushinada."
void GaiaRockInside_ReadSusasMind(void) {
  struct Actor *actor;
  s32 holder;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  holder = FieldPartyInventory_FindMember(ITEM_DANCING_IDOL);
  if (holder != -1) {
    FieldEvent_SetMessageId((s32)MsgGaiaRockInsideWhoDoTheyThinkThey);
    // "Who do they think they are? I'm the only one who can save Kushinada."
    Event_ShowMessage(ACTOR_SUSA, 0);
  } else {
    actor = Actor_Get(ACTOR_CASK);
    if (actor->script == (s32)gGaiaRockInside_CaskEmptiesScript) {
      FieldActor_WaitForScriptEndById(ACTOR_CASK);
    }
    GaiaRockInside_SaveSusa();
    FieldEvent_SetMessageId((s32)MsgGaiaRockInsideHowManyCasksDoesIt);
    // "How many casks does it take to weaken this stupid serpent?"
    Event_ShowMessage(ACTOR_SUSA, 0);
    GaiaRockInside_RestoreSusa();
  }
  Event_End();
}

extern u32 gOrbitSpark_Script[];

#include "shared/orbit_spark_spawn.h"

#include "shared/orbiter_spiral.h"

// Moves around in a circle.
#include "shared/orbit_update.h"

#include "shared/actor_aim_at_watched.h"

#include "shared/actor_step_to_watched.h"

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);

#include "shared/actor_emit_bubbles.h"

#include "text/msg_ids.h"

extern u32 gLesson_TabletRiseScript[];
extern u32 gGaiaRockInside_RisingSparkScript[];
extern s32 gGaiaRockInside_CirclingSparkScript[];
extern u32 gLesson_AdeptRiseScript[];
extern s32 gLesson_SeekingSparkScript[];
extern s32 gLesson_TabletSettleScript[];
extern s32 gLesson_AdeptSettleScript[];

/// Felix touches the slate in the lair and learns Sand: the screen warms to
/// gold, the slate and then Felix rise among circling sparks, a white flash,
/// and they settle again.
void GaiaRockInside_FelixLearnsSand(s32 idA, s32 idB) {
  struct Actor *a = Actor_GetById(idA);
  struct Actor *b = Actor_GetById(idB);
  s32 i;
  s16 *angle;
  /* LEFTOVER(frame): an unused local the ROM's frame reserves and never
   * touches. */
  UNUSED s32 unused[3];

  Sound_Play(SOUND_FADE_OUT_BGM);
  Event_Wait(30);
  FieldCamera_SetSpeed(0.5);
  FieldCamera_MoveWithinMapBounds(INTtoFIXED(a->x.part[0].integer),
                                  INTtoFIXED(a->y.part[0].integer),
                                  INTtoFIXED(a->z.part[0].integer), 1);
  Sound_Play(SE_RUMBLE);
  MapGrid_SetShake(2, 2, 1);
  PaletteFade_SetSource(PALETTE_TINT_NONE, 0);
  PaletteFade_SetTarget(PALETTE_TINT_WARM, 1);
  PaletteFade_StartInterpolation(60);
  a->motionFlags = 0;
  a->acceleration = toFIXED(1) / 5;
  a->speedLimit = toFIXED(1) / 5;
  Actor_SetMovementTypeById(idA, gLesson_TabletRiseScript);
  Sound_Play(SE_TABLET_RISE);
  Event_Wait(45);
  Actor_ConfigureSpriteColor(idA, SPRITE_COLOR_BLINK);
  REG_BLDALPHA = BLEND_ALPHA(8, 16);
  REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL;
  for (i = 0; i != TABLET_RISING_SPARKS; i++) {
    struct Actor *spark =
        Import_ActorCreate(SPRITE_PARTICLE, a->x.fixed, a->y.fixed, a->z.fixed);
    Sound_Play(SE_GLITTER);
    spark->x.fixed += INTtoFIXED((Random16() & 31) - 16);
    spark->y.fixed += INTtoFIXED((Random16() & 15) - 8);
    spark->speedLimit = toFIXED(8);
    spark->acceleration = toFIXED(1) / 5;
    spark->motionFlags = 0;
    Actor_SetSpriteColor(spark, 2);
    Scene_SetActorSpritePriority(spark, 1);
    Actor_SelectAnimation(spark, 7);
    Actor_SetSpriteFlags(spark, 0);
    Actor_SetBlendMode(spark, 1);
    Actor_StartScript(spark, gGaiaRockInside_RisingSparkScript);
    REG_BLDALPHA = BLEND_ALPHA(4, 16);
    wait(2);
    REG_BLDALPHA = BLEND_ALPHA(10, 16);
    wait(2);
    REG_BLDALPHA = BLEND_ALPHA(16, 16);
    wait(2);
    REG_BLDALPHA = BLEND_ALPHA(10, 16);
    wait(2);
  }
  REG_BLDALPHA = BLEND_ALPHA(8, 16);
  wait(30);
  for (i = 0; i != LEARN_SPARKS; i++) {
    struct Actor *spark =
        FieldActor_Create(SPRITE_PARTICLE, b->x.fixed, b->y.fixed, b->z.fixed);
    Sound_Play(SE_SPARKS);
    angle = &spark->timer;
    *angle = (Random16() & 0x7fff) + DEGtoANG(180);
    spark->userValue = ((i & 1) * 2 - 1) << ((Random16() & 3) + 9);
    spark->motionFlags = 0;
    spark->riseBaseY = toFIXED(122);
    spark->watchedActor = b;
    spark->riseCounter =
        (LEARN_SPARKS - i) * LEARN_SPARK_INTERVAL + LEARN_SPIRAL_FRAMES;
    Actor_SetSpriteColor(spark, 2);
    Scene_SetActorSpritePriority(spark, 1);
    Actor_SelectAnimation(spark, 7);
    Actor_SetSpriteFlags(spark, 0);
    Actor_SetBlendMode(spark, 1);
    FieldVector_AddPolarOffsetFX(spark->riseBaseY, *angle, &spark->x.fixed);
    Actor_StartScript(spark, gGaiaRockInside_CirclingSparkScript);
    if (i == 3) {
      Actor_SetSpriteColorById(idB, SPRITE_COLOR_BLINK);
      b->motionFlags = 0;
      b->acceleration = toFIXED(1) / 5;
      b->speedLimit = toFIXED(1) / 5;
      Actor_SetMovementTypeById(idB, gLesson_AdeptRiseScript);
    }
    wait(LEARN_SPARK_INTERVAL);
  }
  Sound_Play(SE_SHIMMER);
  wait(16);
  for (i = 2; i != ORBIT_HALF_TURN; i += 2) {
    struct Actor *spark =
        FieldActor_Create(SPRITE_PARTICLE, b->x.fixed,
                          b->y.fixed + toFIXED(i) - toFIXED(8), b->z.fixed);
    spark->timer = Random16();
    spark->userValue = TRIG_TURN / 32;
    spark->riseCounter = i;
    spark->riseEnabled = 1;
    spark->bounce = spark->x.fixed;
    spark->riseBaseY = 0;
    spark->gravity = spark->z.fixed;
    spark->watchedActor = b;
    spark->motionFlags = 0;
    spark->priorityFlags = 1;
    Actor_SetSpriteColor(spark, 2);
    Actor_SelectAnimation(spark, 7);
    Actor_SetSpriteFlags(spark, 0);
    Actor_SetBlendMode(spark, 1);
    Actor_StartScript(spark, gLesson_SeekingSparkScript);
  }
  Sound_Play(SE_SHIMMER);
  wait(50);
  PaletteFade_SetTarget(RGB_WHITE, 0);
  PaletteFade_StartInterpolation(8);
  wait(16);
  MapGrid_SetShake(0, 0, 0);
  Actor_ConfigureSpriteColor(idA, 0);
  Actor_ConfigureSpriteColor(idB, 0);
  Sound_Play(SE_FLASH);
  PaletteFade_SetTarget(PALETTE_TINT_NONE, 0);
  PaletteFade_StartInterpolation(80);
  Actor_SetMovementTypeById(idA, gLesson_TabletSettleScript);
  Actor_SetMovementTypeById(idB, gLesson_AdeptSettleScript);
  b->update = Actor_EmitBubbles;
  wait(120);
  b->update = 0;
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  Scene_PlayMusic();
  wait(1);
}

void Actor_PlaceOnActor(s32 id, s32 target);
void BlendScript_Suspend(void);
void BlendScript_Resume(void);

/// Felix searches the slate that rose beside the slain Serpent ("Wielder of
/// Earth's might, Lay your hands upon this stone."). Kraden steps out of the
/// party ("That'd be you, Felix. Touch it."), Felix touches it, the ground
/// shakes and the slate teaches him Sand (GaiaRockInside_FelixLearnsSand), and
/// Kraden walks back into the party.
void GaiaRockInside_ReadSandTablet(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.leaderActorId);
  u16 level;
  s32 message;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  message = (s32)MsgGaiaRockInsideSearchedTheSlateThereIs;
  ((s32 (*)())FieldMessage_Show)(message, 1);
  Actor_ShowEmote(gMainRoutineWork.leaderActorId,
                  EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  if (FIELD_TILE(leader->z.fixed) <= 39) {
    Actor_WalkTo(gMainRoutineWork.leaderActorId, 440, 328);
    FieldActor_WaitForMoveById(gMainRoutineWork.leaderActorId);
  }
  Actor_PlaceOnActor(ACTOR_KRADEN, gMainRoutineWork.leaderActorId);
  Actor_WalkTo(gMainRoutineWork.leaderActorId, 470, 354);
  Actor_WalkTo(ACTOR_KRADEN, 446, 354);
  FieldActor_WaitForMoveById(gMainRoutineWork.leaderActorId);
  FieldActor_WaitForMoveById(ACTOR_KRADEN);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_EAST, 0);
  FieldEvent_SetMessageId(message + 1);
  // "Wielder of Earth's might... That'd be you, Felix. Touch it. Go on..."
  Event_ShowMessage(ACTOR_KRADEN, 0);
  FieldEvent_Wait(10);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_WEST, 0);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(gMainRoutineWork.leaderActorId, ANIM_NOD);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  FieldEvent_Wait(10);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH, 0);
  FieldEvent_Wait(10);
  Actor_WalkTo(gMainRoutineWork.leaderActorId, 466, 342);
  FieldActor_WaitForMoveById(gMainRoutineWork.leaderActorId);
  Message_ShowCentered(message + 2, 1);
  MapGrid_SetShake(5, 7, 1);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  FieldMapGrid_WaitForShakeToSettle();
  BlendScript_Suspend();
  Io_Read16(&REG_BLDALPHA, &level);
  level &= BLEND_ALPHA_TOP_MASK;
  while (level != 0) {
    REG_BLDALPHA = level | BLEND_ALPHA(0, 16);
    level--;
    System_Wait(1);
  }
  REG_DISPCNT &= ~DISPCNT_BG1_ON;
  GaiaRockInside_FelixLearnsSand(ACTOR_SAND_TABLET, ACTOR_FELIX);
  REG_DISPCNT |=
      DISPCNT_BG0_ON | DISPCNT_BG1_ON | DISPCNT_BG2_ON | DISPCNT_BG3_ON;
  REG_BLDALPHA = level | BLEND_ALPHA(0, 16);
  REG_BLDCNT = BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  while (level != 8) {
    REG_BLDALPHA = level | BLEND_ALPHA(0, 16);
    level++;
    System_Wait(1);
  }
  BlendScript_Resume();
  Actor_SetPosition(ACTOR_SAND_TABLET, 0, 0);
  Party_LearnAbility(FELIX, ABILITY_SAND);
  Event_SetFlag(FLAG_LEARNED_SAND);
  Actor_SetSpeed(ACTOR_KRADEN, 1.2);
  Actor_SetAnimation(ACTOR_KRADEN, ANIM_WALK);
  {
    struct Actor *owner = Actor_Get(gMainRoutineWork.leaderActorId);

    if (owner != 0)
      Actor_SetDestination(ACTOR_KRADEN, owner->x.part[0].integer,
                           owner->z.part[0].integer);
  }
  FieldActor_WaitForMoveById(ACTOR_KRADEN);
  Actor_SetPosition(ACTOR_KRADEN, 0, 0);
  FieldEvent_Wait(20);
  Event_End();
}

// The same routine as FieldEffect_AdvanceMotion, under another name.
/// The dust's update: it moves by its velocity, slowing a little across the
/// ground each frame, and grows and spins.
void Effect_AdvanceMotionLocal(union FieldObject *object) {
  struct FieldEffect *effect = &object->effect;
  s32 velocityX;
  s32 velocityZ;
  s32 scaleRateX;
  struct FieldSprite *sprite;

  velocityX = effect->velocity.x;
  effect->x += velocityX;
  effect->y += effect->velocity.y;
  velocityZ = effect->velocity.z;
  effect->z += velocityZ;

  effect->velocity.x = velocityX - velocityX / FIELD_EFFECT_DRAG_X;
  effect->velocity.z = velocityZ - velocityZ / FIELD_EFFECT_DRAG_Z;

  scaleRateX = effect->scaleRateX;
  effect->scaleX += scaleRateX;
  sprite = effect->sprite;
  effect->scaleY += effect->scaleRateY;
  sprite->rotation += effect->spin;
}

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);

/// A ring of dust spreads from where the slate rises, one puff every eighth of
/// a turn.
void GaiaRockInside_RingOfDust(void) {
  FIXED velocity[3];
  struct EffectOptions options;
  u32 i;

  options.update = Effect_AdvanceMotionLocal;
  i = 0;
  do {
    {
      s32 angle = i * FACING_STEP;

      velocity[0] = Trig_Cos(angle);
      velocity[1] = 0;
      velocity[2] = Math_Sin(angle);
      velocity[0] += velocity[0] / 3;
      FieldEffect_Spawn(toFIXED(464), toFIXED(-32), toFIXED(328), velocity[0],
                        velocity[1], velocity[2], EFFECT_USE_UPDATE | 1,
                        &options);
      i += 2;
    }
  } while (i <= TRIG_TURN / FACING_STEP);
}

/// The pixels the slate rises from the lair's floor, one every 2 frames.
enum { TABLET_RISE_PIXELS = 20 };

/// Felix steps onto the floor before the slain Serpent, cells (29, 24) to (31,
/// 24), and the ground shakes and the slate rises from the floor west of it,
/// in two rings of dust, unless it has risen already.
void GaiaRockInside_RaiseSandTablet(void) {
  // Play never reaches this with the slate still down: only the lair's
  // entrances 10 to 12 use this table. The arrival after the battle won
  // (Scene_Initialize, entrance 10) raises the slate itself; the arrivals
  // after a battle lost (11 and 12) send the party on to the restart point
  // before the player has control; and later visits come by entrance 9,
  // whose table lacks this trigger.
  struct Actor *person;
  struct Actor *actor;
  u16 level;
  s32 i;

  if (GameFlag_Get(FLAG_SAND_TABLET_RAISED) || GameFlag_Get(FLAG_LEARNED_SAND))
    return;
  person = Actor_Get(ACTOR_SAND_TABLET);
  Io_Read16(&REG_BLDALPHA, &level);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  MapGrid_SetShake(10, 10, 1);
  Event_PlaySound(SE_TREMOR);
  BlendScript_Suspend();
  level &= BLEND_ALPHA_TOP_MASK;
  while (level != 0) {
    REG_BLDALPHA = level | BLEND_ALPHA(0, 16);
    level--;
    System_Wait(1);
  }
  REG_DISPCNT &= ~DISPCNT_BG1_ON;
  FieldEvent_Wait(30);
  actor = OverlayObject_PrepareObject(toFIXED(464), -toFIXED(32), toFIXED(348),
                                      SPRITE_SLATE);
  Actor_SetPosition(ACTOR_SAND_TABLET, 464, 348);
  GaiaRockInside_RingOfDust();
  for (i = 0; i < TABLET_RISE_PIXELS; i++) {
    if (i == TABLET_RISE_PIXELS / 2)
      GaiaRockInside_RingOfDust();
    person->z.fixed -= toFIXED(1);
    FieldEvent_Wait(2);
  }
  Event_SetFlag(FLAG_SAND_TABLET_RAISED);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  actor->x.fixed = 0;
  actor->z.fixed = 0;
  FieldEvent_Wait(1);
  Actor_Release(actor);
  Event_PlaySound(SOUND_FADE_OUT_ACTION_SE);
  FieldEvent_Wait(30);
  REG_DISPCNT |=
      DISPCNT_BG0_ON | DISPCNT_BG1_ON | DISPCNT_BG2_ON | DISPCNT_BG3_ON;
  REG_BLDALPHA = level | BLEND_ALPHA(0, 16);
  REG_BLDCNT = BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  while (level != 8) {
    REG_BLDALPHA = level | BLEND_ALPHA(0, 16);
    level++;
    System_Wait(1);
  }
  BlendScript_Resume();
  Event_End();
}

#include "text/msg_ids.h"

/// Felix examines the Serpent on the sand at the north end of the lair, and
/// the battle with it starts, one the party cannot run from. The fewer beams of
/// sunlight reach the lair, the more it heals each turn. Won, Susa comes back
/// and slays it, and the slate rises (Scene_Initialize, entrance 10). Lost
/// without the Dancing Idol, Susa scolds the party ("You! You cannot fight the
/// serpent!"); lost holding it before any beam reaches the lair, Susa, lying
/// hurt, says Dragonsbane alone cannot beat it. Either way the party is sent
/// back to its restart point. Once it is slain the Serpent, turned to stone,
/// only "looks almost alive".
void GaiaRockInside_ChallengeSerpent(void) {
  s32 count;

  if (Event_GetFlag(FLAG_SERPENT_SLAIN)) {
    // "The serpent, with its eyes closed, looks almost alive."
    Message_ShowCentered((s32)MsgGaiaRockInsideTheSerpentWithItsEyes, 1);
  } else {
    s32 a = Event_GetFlag(FLAG_ALTAR_ROOM_SUNBEAM);
    s32 b = Event_GetFlag(FLAG_WEST_ALTAR_SUNBEAM);
    s32 c = Event_GetFlag(FLAG_EAST_ALTAR_SUNBEAM);
    s32 scene;

    count = a + b + c + Event_GetFlag(FLAG_BUSHES_SUNBEAM);
    Battle_SetType(BATTLE_TYPE_NO_ESCAPE);
    scene = SCENE_GAIA_ROCK_INSIDE_MAZE;
    FieldEvent_SetAfterBattleDestination(scene, ENTRANCE_SERPENT_BEATEN);
    if (FieldPartyInventory_FindMember(ITEM_DANCING_IDOL) != -1) {
      if (count == 0)
        Event_SetDefeatDestination(scene, ENTRANCE_BEATEN_HOLDING_IDOL);
    } else {
      Event_SetDefeatDestination(scene, ENTRANCE_BEATEN_BY_SERPENT);
    }
    Field_RequestBattleFromEncounterTable(ENCOUNTER_TABLE_SERPENT, count);
  }
}

#include "text/msg_ids.h"

/// Felix casts Sand in the lair and sinks into the sand as a moving mound; he
/// now moves on the map's sand layer, by which he can pass the Serpent to the
/// Cloud Brand behind its tail.
void GaiaRockInside_SinkIntoSand(void) {
  Actor_Get(gMainRoutineWork.leaderActorId)->layer = 2;
}

/// Felix comes up out of the sand as Sand ends, and walks on the floor again.
void GaiaRockInside_RiseFromSand(void) {
  Actor_Get(gMainRoutineWork.leaderActorId)->layer = 0;
}

/// Felix examines the floor by the end of the Serpent's tail, cell (34, 18),
/// which he reaches with Sand, and finds the Cloud Brand. The game raises the
/// hidden item to the raised floor's height there and gives it without the
/// usual message of searching.
void GaiaRockInside_FindCloudBrand(void) {
  struct Actor *actor;

  actor = Actor_Get(FIELD_TREASURE_ACTOR + 2);
  actor->x.fixed = toFIXED(552);
  actor->z.fixed = toFIXED(296);
  actor->y.fixed = toFIXED(-30);
  actor->groundHeight = toFIXED(-32);
  Event_ClearFlag(FLAG_NOTHING_HAPPENS);
  Scene_TalkToActor(FIELD_TALK_SILENT | (FIELD_TREASURE_ACTOR + 2));
}

/// Felix examines one of the altars where the Dancing Idol dances: "Felix
/// checked the altar. It looks like something can be placed here." Holding the
/// Dancing Idol, the field menu then opens so that he can use it.
void GaiaRockInside_CheckAltar(void) {
  const u8 *message = MsgCheckedTheAltar;
  const u8 *next;
  s32 owner;

  Message_ShowCentered(message, 1);
  owner = FieldPartyInventory_FindMember(ITEM_DANCING_IDOL);
  next = message + 2;
  if (owner != -1) {
    struct FieldWork *work = Ram_HeapSlots->fieldWork;

    Message_ShowCentered(next, 1);
    work->fieldMenu = 1;
  } else {
    Message_ShowCentered(next, 1);
  }
}

#include "text/msg_ids.h"
void Actor_PlaceOnActor(s32 id, s32 target);

/// How a stone face trembles while the doll dances on its altar: for
/// FACE_TREMBLE_FRAMES, then on as it slides aside, for FACE_SLIDE_FRAMES_MAX
/// at most, and FACE_SETTLE_FRAMES more once there; a face already slid aside
/// only trembles, for FACE_TREMBLE_LONG_FRAMES.
enum {
  FACE_TREMBLE_FRAMES = 20,
  FACE_SLIDE_FRAMES_MAX = 300,
  FACE_SETTLE_FRAMES = 10,
  FACE_TREMBLE_LONG_FRAMES = 120
};

/// Felix uses the Dancing Idol on the altar in the first altar's room. The doll
/// hops onto the altar and dances, the two great red faces either side of it
/// glare and glow, and the first time they tremble and slide in toward the
/// altar, uncovering the doorways behind them, west at cell (25, 42) and east
/// at cell (36, 42). The doll hops back to Felix.
void GaiaRockInside_PlaceDollOnFirstAltar(void) {
  struct Actor *westFace = Actor_Get(ACTOR_FIRST_ALTAR_WEST_FACE);
  struct Actor *eastFace = Actor_Get(ACTOR_FIRST_ALTAR_EAST_FACE);
  struct Actor *doll = Actor_Get(ACTOR_FIRST_ALTAR_DOLL);
  struct Actor *leader;
  struct MainRoutineWork *work;
  s32 *leaderId;
  s32 i;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  // "Felix placed a doll on the altar."
  Message_ShowCentered((s32)&MsgGaiaRockInsidePlacedADollOnThe, 1);
  Event_PlaySound(SE_DOLL_PLACED);
  Actor_SetSpritePriorityDirect(doll, 1);
  work = &gMainRoutineWork;
  leaderId = &work->leaderActorId;
  Actor_PlaceOnActor(ACTOR_FIRST_ALTAR_DOLL, *leaderId);
  Actor_SetDestination(ACTOR_FIRST_ALTAR_DOLL, 496, 712);
  doll->velocityY = toFIXED(6);
  FieldActor_WaitForMoveById(ACTOR_FIRST_ALTAR_DOLL);
  FieldEvent_Wait(45);
  Actor_FaceActor(ACTOR_FIRST_ALTAR_WEST_FACE, *leaderId, 0);
  Actor_FaceActor(ACTOR_FIRST_ALTAR_EAST_FACE, *leaderId, 0);
  Event_PlaySound(SE_STATUE_STIRS);
  Actor_SetAnimation(ACTOR_FIRST_ALTAR_WEST_FACE, 5);
  Actor_SetAnimation(ACTOR_FIRST_ALTAR_EAST_FACE, 5);
  System_Wait(30);
  Actor_SetAnimation(ACTOR_FIRST_ALTAR_WEST_FACE, 2);
  Actor_SetAnimation(ACTOR_FIRST_ALTAR_EAST_FACE, 2);
  FieldEvent_Wait(30);
  Actor_SetAnimation(ACTOR_FIRST_ALTAR_WEST_FACE, 3);
  Actor_SetAnimation(ACTOR_FIRST_ALTAR_EAST_FACE, 3);
  FieldEvent_Wait(40);
  Event_PlaySound(SE_TABLET_RISE_HIGH);
  if (!Event_GetFlag(FLAG_FIRST_ALTAR_FACES_MOVED)) {
    for (i = 0; i < FACE_TREMBLE_FRAMES; i++) {
      westFace->velocityY = Random16();
      eastFace->velocityY = Random16();
      System_Wait(1);
    }
    Actor_SetSpeedAndAcceleration(ACTOR_FIRST_ALTAR_WEST_FACE, 0.25, 0.05);
    Actor_SetSpeedAndAcceleration(ACTOR_FIRST_ALTAR_EAST_FACE, 0.25, 0.05);
    Actor_SetDestinationOffset(ACTOR_FIRST_ALTAR_WEST_FACE, 24, 0);
    Actor_SetDestinationOffset(ACTOR_FIRST_ALTAR_EAST_FACE, -24, 0);
    for (i = 0; i < FACE_SLIDE_FRAMES_MAX && westFace->arrivalAxis != 0; i++) {
      westFace->velocityY = Random16();
      eastFace->velocityY = Random16();
      System_Wait(1);
    }
    for (i = 0; i < FACE_SETTLE_FRAMES; i++) {
      westFace->velocityY = Random16();
      eastFace->velocityY = Random16();
      System_Wait(1);
    }
    MapGrid_CopyCells(119, 31, 1, 2, 89, 40);
    MapGrid_CopyCells(119, 31, 1, 2, 100, 40);
    Event_SetFlag(FLAG_FIRST_ALTAR_FACES_MOVED);
  } else {
    for (i = 0; i < FACE_TREMBLE_LONG_FRAMES; i++) {
      westFace->velocityY = Random16();
      eastFace->velocityY = Random16();
      System_Wait(1);
    }
    FieldEvent_Wait(30);
  }
  Actor_SetAnimation(ACTOR_FIRST_ALTAR_WEST_FACE, 4);
  Actor_SetAnimation(ACTOR_FIRST_ALTAR_EAST_FACE, 4);
  FieldEvent_Wait(30);
  leader = Actor_Get(gMainRoutineWork.leaderActorId);
  if (leader != NULL)
    Actor_SetDestination(ACTOR_FIRST_ALTAR_DOLL, leader->x.part[0].integer,
                         leader->z.part[0].integer);
  doll->velocityY = toFIXED(6);
  FieldActor_WaitForMoveById(ACTOR_FIRST_ALTAR_DOLL);
  Actor_SetPosition(ACTOR_FIRST_ALTAR_DOLL, 0, 0);
  Event_End();
}

/// Felix uses the Dancing Idol on the west altar of the twin altars' room. The
/// doll hops onto the altar and dances, and the face on the wall above glares
/// and trembles. The first time, the face slides east and uncovers a shaft
/// of sunlight, whose beam falls into the hole in the floor before it and
/// down into the Serpent's lair; the doll hops back to Felix, and while the
/// Serpent lives the game first shows the beam falling into the lair. The
/// east altar's and the altar room's are alike.
void GaiaRockInside_PlaceDollOnWestAltar(void) {
  struct Actor *actor;
  struct Actor *partner;
  struct Actor *leader;
  s32 i;

  actor = Actor_GetById(ACTOR_WEST_FACE);
  partner = Actor_GetById(ACTOR_TWIN_ALTARS_DOLL);
  Event_Start();
  Event_ResolvePendingActions(0);
  // "Felix placed a doll on the altar."
  FieldMessage_Show((s32)MsgGaiaRockInsidePlacedADollOnThe, 1);
  Sound_Play(SE_DOLL_PLACED);
  Actor_SetSpritePriorityDirect(partner, 1);
  Actor_PlaceOnActor(ACTOR_TWIN_ALTARS_DOLL, gMainRoutineWork.leaderActorId);
  Actor_SetDestination(ACTOR_TWIN_ALTARS_DOLL, 528, 372);
  partner->velocityY = toFIXED(6);
  Actor_WaitForMoveById(ACTOR_TWIN_ALTARS_DOLL);
  Event_Wait(45);
  Actor_FaceActorById(ACTOR_WEST_FACE, gMainRoutineWork.leaderActorId, 0);
  Sound_Play(SE_STATUE_STIRS);
  Actor_SetAnimationById(ACTOR_WEST_FACE, 5);
  wait(30);
  Actor_SetAnimationById(ACTOR_WEST_FACE, 2);
  Event_Wait(30);
  Actor_SetAnimationById(ACTOR_WEST_FACE, 3);
  Event_Wait(40);
  Sound_Play(SE_TABLET_RISE_HIGH);
  if (!GameFlag_Get(FLAG_WEST_ALTAR_SUNBEAM)) {
    for (i = 0; i < FACE_TREMBLE_FRAMES; i++) {
      actor->velocityY = Random16();
      wait(1);
    }
    Actor_SetSpeedAndAcceleration(ACTOR_WEST_FACE, 0.25, 0.05);
    Actor_SetDestinationOffsetById(ACTOR_WEST_FACE, 24, 0);
    MapGrid_CopyCellAttributes(27, 23, 1, 1, 28, 22);
    Map_CopyCellAttributeRect(32, 16, 2, 1, 29, 22);
    i = 0;
    if (actor->arrivalAxis != 0) {
      do {
        actor->velocityY = Random16();
        wait(1);
        if (++i >= FACE_SLIDE_FRAMES_MAX)
          break;
      } while (actor->arrivalAxis != 0);
    }
    MapGrid_CopyCells(2, 93, 1, 5, 28, 85);
    for (i = 0; i < FACE_SETTLE_FRAMES; i++) {
      actor->velocityY = Random16();
      wait(1);
    }
    Sound_Play(SE_EYES_GLOW);
    Actor_SetAnimationById(ACTOR_WEST_FACE, 4);
    Event_Wait(30);
    leader = Actor_GetById(gMainRoutineWork.leaderActorId);
    if (leader != NULL)
      Actor_SetDestinationById(ACTOR_TWIN_ALTARS_DOLL,
                               leader->x.part[0].integer,
                               leader->z.part[0].integer);
    partner->velocityY = toFIXED(6);
    Actor_WaitForMoveById(ACTOR_TWIN_ALTARS_DOLL);
    Actor_SetPosition(ACTOR_TWIN_ALTARS_DOLL, 0, 0);
    if (!GameFlag_Get(FLAG_SERPENT_SLAIN)) {
      GameFlag_Set(FLAG_WEST_ALTAR_SUNBEAM_TO_SHOW);
      Event_Wait(30);
      ObjectTable_Snapshot();
      Import_EventSetExitRequest(
          GAIA_ROCK_INSIDE_TWIN_ALTARS_TO_GAIA_ROCK_INSIDE_MAZE);
    } else {
      GameFlag_Set(FLAG_WEST_ALTAR_SUNBEAM);
      Event_End();
    }
  } else {
    for (i = 0; i < FACE_TREMBLE_LONG_FRAMES; i++) {
      actor->velocityY = Random16();
      wait(1);
    }
    Event_Wait(30);
    Actor_SetAnimationById(ACTOR_WEST_FACE, 4);
    Event_Wait(30);
    leader = Actor_GetById(gMainRoutineWork.leaderActorId);
    if (leader != NULL)
      Actor_SetDestinationById(ACTOR_TWIN_ALTARS_DOLL,
                               leader->x.part[0].integer,
                               leader->z.part[0].integer);
    partner->velocityY = toFIXED(6);
    Actor_WaitForMoveById(ACTOR_TWIN_ALTARS_DOLL);
    Actor_SetPosition(ACTOR_TWIN_ALTARS_DOLL, 0, 0);
    Event_End();
  }
}

/// Felix uses the Dancing Idol on the east altar of the twin altars' room, the
/// one that fell from the altar room's cobweb; the face above it slides east
/// and lets in its beam, as on the west altar
/// (GaiaRockInside_PlaceDollOnWestAltar).
void GaiaRockInside_PlaceDollOnEastAltar(void) {
  struct Actor *actor;
  struct Actor *partner;
  struct Actor *leader;
  s32 i;

  actor = Actor_GetById(ACTOR_EAST_FACE);
  partner = Actor_GetById(ACTOR_TWIN_ALTARS_DOLL);
  Event_Start();
  Event_ResolvePendingActions(0);
  // "Felix placed a doll on the altar."
  FieldMessage_Show((s32)MsgGaiaRockInsidePlacedADollOnThe, 1);
  Sound_Play(SE_DOLL_PLACED);
  Actor_SetSpritePriorityDirect(partner, 1);
  Actor_PlaceOnActor(ACTOR_TWIN_ALTARS_DOLL, gMainRoutineWork.leaderActorId);
  Actor_SetDestination(ACTOR_TWIN_ALTARS_DOLL, 672, 372);
  partner->velocityY = toFIXED(6);
  Actor_WaitForMoveById(ACTOR_TWIN_ALTARS_DOLL);
  Event_Wait(45);
  Actor_FaceActorById(ACTOR_EAST_FACE, gMainRoutineWork.leaderActorId, 0);
  Sound_Play(SE_STATUE_STIRS);
  Actor_SetAnimationById(ACTOR_EAST_FACE, 5);
  wait(30);
  Actor_SetAnimationById(ACTOR_EAST_FACE, 2);
  Event_Wait(30);
  Actor_SetAnimationById(ACTOR_EAST_FACE, 3);
  Event_Wait(40);
  Sound_Play(SE_TABLET_RISE_HIGH);
  if (!GameFlag_Get(FLAG_EAST_ALTAR_SUNBEAM)) {
    for (i = 0; i < FACE_TREMBLE_FRAMES; i++) {
      actor->velocityY = Random16();
      wait(1);
    }
    Actor_SetSpeedAndAcceleration(ACTOR_EAST_FACE, 0.25, 0.05);
    Actor_SetDestination(ACTOR_EAST_FACE, 608, 352);
    MapGrid_CopyCellAttributes(35, 23, 1, 1, 36, 22);
    Map_CopyCellAttributeRect(32, 16, 2, 1, 37, 22);
    i = 0;
    if (actor->arrivalAxis != 0) {
      do {
        actor->velocityY = Random16();
        wait(1);
        if (++i >= FACE_SLIDE_FRAMES_MAX)
          break;
      } while (actor->arrivalAxis != 0);
    }
    MapGrid_CopyCells(2, 93, 1, 5, 36, 85);
    for (i = 0; i < FACE_SETTLE_FRAMES; i++) {
      actor->velocityY = Random16();
      wait(1);
    }
    Sound_Play(SE_EYES_GLOW);
    Actor_SetAnimationById(ACTOR_EAST_FACE, 4);
    Event_Wait(30);
    leader = Actor_GetById(gMainRoutineWork.leaderActorId);
    if (leader != NULL)
      Actor_SetDestinationById(ACTOR_TWIN_ALTARS_DOLL,
                               leader->x.part[0].integer,
                               leader->z.part[0].integer);
    partner->velocityY = toFIXED(6);
    Actor_WaitForMoveById(ACTOR_TWIN_ALTARS_DOLL);
    Actor_SetPosition(ACTOR_TWIN_ALTARS_DOLL, 0, 0);
    if (!GameFlag_Get(FLAG_SERPENT_SLAIN)) {
      GameFlag_Set(FLAG_EAST_ALTAR_SUNBEAM_TO_SHOW);
      Event_Wait(30);
      ObjectTable_Snapshot();
      Import_EventSetExitRequest(
          GAIA_ROCK_INSIDE_TWIN_ALTARS_TO_GAIA_ROCK_INSIDE_MAZE);
    } else {
      GameFlag_Set(FLAG_EAST_ALTAR_SUNBEAM);
      Event_End();
    }
  } else {
    for (i = 0; i < FACE_TREMBLE_LONG_FRAMES; i++) {
      actor->velocityY = Random16();
      wait(1);
    }
    Event_Wait(30);
    Actor_SetAnimationById(ACTOR_EAST_FACE, 4);
    Event_Wait(30);
    leader = Actor_GetById(gMainRoutineWork.leaderActorId);
    if (leader != NULL)
      Actor_SetDestinationById(ACTOR_TWIN_ALTARS_DOLL,
                               leader->x.part[0].integer,
                               leader->z.part[0].integer);
    partner->velocityY = toFIXED(6);
    Actor_WaitForMoveById(ACTOR_TWIN_ALTARS_DOLL);
    Actor_SetPosition(ACTOR_TWIN_ALTARS_DOLL, 0, 0);
    Event_End();
  }
}

/// Felix uses the Dancing Idol on the altar ringed by bushes in the altar
/// room's west part; the face on the wall beside it slides west and lets in
/// its beam, as on the twin altars' west altar
/// (GaiaRockInside_PlaceDollOnWestAltar). It is the first beam the walkthrough
/// lets in.
void GaiaRockInside_PlaceDollOnAltarAltar(void) {
  struct Actor *actor;
  struct Actor *partner;
  struct Actor *leader;
  s32 i;

  actor = Actor_GetById(ACTOR_ALTAR_FACE);
  partner = Actor_GetById(ACTOR_ALTAR_DOLL);
  Event_Start();
  Event_ResolvePendingActions(0);
  // "Felix placed a doll on the altar."
  FieldMessage_Show((s32)MsgGaiaRockInsidePlacedADollOnThe, 1);
  Sound_Play(SE_DOLL_PLACED);
  Actor_SetSpritePriorityDirect(partner, 1);
  Actor_PlaceOnActor(ACTOR_ALTAR_DOLL, gMainRoutineWork.leaderActorId);
  Actor_SetDestination(ACTOR_ALTAR_DOLL, 336, 396);
  partner->velocityY = toFIXED(6);
  Actor_WaitForMoveById(ACTOR_ALTAR_DOLL);
  Event_Wait(45);
  Actor_FaceActorById(ACTOR_ALTAR_FACE, gMainRoutineWork.leaderActorId, 0);
  Sound_Play(SE_STATUE_STIRS);
  Actor_SetAnimationById(ACTOR_ALTAR_FACE, 5);
  wait(30);
  Actor_SetAnimationById(ACTOR_ALTAR_FACE, 2);
  Event_Wait(30);
  Actor_SetAnimationById(ACTOR_ALTAR_FACE, 3);
  Event_Wait(40);
  Sound_Play(SE_TABLET_RISE_HIGH);
  if (!GameFlag_Get(FLAG_ALTAR_ROOM_SUNBEAM)) {
    for (i = 0; i < FACE_TREMBLE_FRAMES; i++) {
      actor->velocityY = Random16();
      wait(1);
    }
    Actor_SetSpeedAndAcceleration(ACTOR_ALTAR_FACE, 0.25, 0.05);
    Actor_SetDestinationOffset(ACTOR_ALTAR_FACE, -16, 0);
    Map_CopyCellAttributeRect(27, 15, 2, 1, 23, 23);
    Map_CopyCellAttributeRect(25, 25, 1, 1, 25, 23);
    i = 0;
    if (actor->arrivalAxis != 0) {
      do {
        actor->velocityY = Random16();
        wait(1);
        if (++i >= FACE_SLIDE_FRAMES_MAX)
          break;
      } while (actor->arrivalAxis != 0);
    }
    MapGrid_CopyCells(1, 85, 1, 4, 25, 85);
    for (i = 0; i < FACE_SETTLE_FRAMES; i++) {
      actor->velocityY = Random16();
      wait(1);
    }
    Sound_Play(SE_EYES_GLOW);
    Actor_SetAnimationById(ACTOR_ALTAR_FACE, 4);
    Event_Wait(30);
    leader = Actor_GetById(gMainRoutineWork.leaderActorId);
    if (leader != NULL)
      Actor_SetDestinationById(ACTOR_ALTAR_DOLL, leader->x.part[0].integer,
                               leader->z.part[0].integer);
    partner->velocityY = toFIXED(6);
    Actor_WaitForMoveById(ACTOR_ALTAR_DOLL);
    Actor_SetPosition(ACTOR_ALTAR_DOLL, 0, 0);
    if (!GameFlag_Get(FLAG_SERPENT_SLAIN)) {
      GameFlag_Set(FLAG_ALTAR_ROOM_SUNBEAM_TO_SHOW);
      Event_Wait(30);
      ObjectTable_Snapshot();
      Import_EventSetExitRequest(
          GAIA_ROCK_INSIDE_ALTAR_TO_GAIA_ROCK_INSIDE_MAZE_B);
    } else {
      GameFlag_Set(FLAG_ALTAR_ROOM_SUNBEAM);
      Event_End();
    }
  } else {
    for (i = 0; i < FACE_TREMBLE_LONG_FRAMES; i++) {
      actor->velocityY = Random16();
      wait(1);
    }
    Event_Wait(30);
    Actor_SetAnimationById(ACTOR_ALTAR_FACE, 4);
    Event_Wait(30);
    leader = Actor_GetById(gMainRoutineWork.leaderActorId);
    if (leader != NULL)
      Actor_SetDestinationById(ACTOR_ALTAR_DOLL, leader->x.part[0].integer,
                               leader->z.part[0].integer);
    partner->velocityY = toFIXED(6);
    Actor_WaitForMoveById(ACTOR_ALTAR_DOLL);
    Actor_SetPosition(ACTOR_ALTAR_DOLL, 0, 0);
    Event_End();
  }
}

/// The spots listed in gGaiaRockInside_StumpSpots, from 1; 0 is none.
enum { STUMP_SPOTS = 11 };

extern s16 gGaiaRockInside_StumpSpots[][2];

/// Felix Moves or pushes one of the two stumps on the way to the maze in the
/// first altar's room, east of the altar. Nothing more shows than the stump
/// sliding; the game remembers where both stumps now stand, so that they are
/// still there when the party comes back. Each is kept as one of the
/// STUMP_SPOTS places it can stand, the two in halves of FLAG_STUMP_SPOTS.
s32 GaiaRockInside_RecordStumpSpots(void) {
  struct Actor *first = Actor_Get(ACTOR_STUMP_A);
  s8 firstIndex = 0;
  s32 firstX = FIELD_TILE(first->x.fixed);
  s8 secondIndex = 0;
  s32 firstZ = FIELD_TILE(first->z.fixed);
  struct Actor *second = Actor_Get(ACTOR_STUMP_B);
  s32 secondX = FIELD_TILE(second->x.fixed);
  s32 secondZ = FIELD_TILE(second->z.fixed);
  s8 i;

  for (i = 1; i <= STUMP_SPOTS; i++) {
    if (firstX == gGaiaRockInside_StumpSpots[i][0] &&
        firstZ == gGaiaRockInside_StumpSpots[i][1])
      firstIndex = i;
    if (secondX == gGaiaRockInside_StumpSpots[i][0] &&
        secondZ == gGaiaRockInside_StumpSpots[i][1])
      secondIndex = i;
  }
  {
    s8 spots = firstIndex | secondIndex << 4;

    return GameFlag_SetByte(FLAG_STUMP_SPOTS, spots);
  }
}

/// Climbing the wall of handholds along the altar room's south part, Felix
/// reaches its west trap hold, drawn at cell (24, 37) (cell (24, 101) of the
/// wall's overhead layer): a yellow stone head with red eyes bursts out of the
/// wall under him in a shower of rocks, he flails, and he is left standing on
/// it, no longer climbing (StoneHead_BurstOut). Unlike at the
/// stone faces outside, he does not fall to the floor: he stays standing up
/// there beside the head.
void GaiaRockInside_ClimbOntoWestStoneHead(void) {
  StoneHead_BurstOut(ACTOR_STATUE_A, FLAG_STATUE_A_RISEN);
}

/// As GaiaRockInside_ClimbOntoWestStoneHead, for the middle trap hold, a
/// little lower and to the east: drawn at cell (28, 39), cell (28, 103) of the
/// overhead layer.
void GaiaRockInside_ClimbOntoMiddleStoneHead(void) {
  StoneHead_BurstOut(ACTOR_STATUE_B, FLAG_STATUE_B_RISEN);
}

/// As GaiaRockInside_ClimbOntoWestStoneHead, for the east trap hold: drawn at
/// cell (35, 39), cell (35, 103) of the overhead layer.
void GaiaRockInside_ClimbOntoEastStoneHead(void) {
  StoneHead_BurstOut(ACTOR_STATUE_C, FLAG_STATUE_C_RISEN);
}

extern struct Trigger gGaiaRockInside_Triggers[];
extern struct Trigger gGaiaRockInside_LairTriggers[];
extern struct Trigger gGaiaRockInside_SlainLairTriggers[];
extern struct Trigger gGaiaRockInside_FirstAltarTriggers[];
extern struct Trigger gGaiaRockInside_TwinAltarsTriggers[];
extern struct Trigger gGaiaRockInside_PlatformsTriggers[];
extern struct Trigger gGaiaRockInside_AltarTriggers[];

/// Returns the current room's triggers: the lair has one table while the
/// Serpent lives and the party comes in by its way in, another after the
/// battle.
const struct Trigger *Scene_GetTriggers(void) {
  if (gMainRoutineWork.next == SCENE_GAIA_ROCK_INSIDE_FIRST_ALTAR)
    return gGaiaRockInside_FirstAltarTriggers;
  if (gMainRoutineWork.next == SCENE_GAIA_ROCK_INSIDE_TWIN_ALTARS)
    return gGaiaRockInside_TwinAltarsTriggers;
  if (gMainRoutineWork.next == SCENE_GAIA_ROCK_INSIDE_PLATFORMS)
    return gGaiaRockInside_PlatformsTriggers;
  if (gMainRoutineWork.next == SCENE_GAIA_ROCK_INSIDE_ALTAR)
    return gGaiaRockInside_AltarTriggers;
  if (gMainRoutineWork.entrance == ENTRANCE_LAIR)
    return gGaiaRockInside_LairTriggers;
  if (gMainRoutineWork.entrance > 9)
    return gGaiaRockInside_SlainLairTriggers;
  return gGaiaRockInside_Triggers;
}

/// Turns a stone face toward its altar and lays its glowing eyes over its
/// sprite, drawn in front.
void GaiaRockInside_TurnFaceToAltar(s32 faceId, s32 altarId) {
  struct SpriteAnimation *eyes;
  struct Actor *face = Actor_GetById(faceId);

  Actor_FaceActorById(faceId, altarId, 0);
  if (face != NULL) {
    eyes = (struct SpriteAnimation *)Sprite_AddAnimation(face->sprite,
                                                         SPRITE_GLOWING_EYES);
    eyes->colorMode = SPRITE_COLOR_NORMAL;
    eyes->priority = 0;
    Actor_SelectAnimation(face, 0);
    Actor_SelectAnimation(face, 4);
  }
}

/// Readies a platform: one others stand on, 16 pixels up, its home its place
/// now, on animation mode.
void GaiaRockInside_SetPlatformMode(u32 id, s8 mode) {
  struct Actor *actor = Actor_Get(id);

  actor->motionFlags = 0;
  actor->priorityFlags |= ACTOR_PRIORITY_PLATFORM;
  actor->acceleration = toFIXED(1) / 16;
  actor->speedLimit = toFIXED(1);
  actor->y.fixed = toFIXED(16);
  actor->timer = FIXEDtoINT(actor->x.fixed);
  actor->userValue = FIXEDtoINT(actor->z.fixed);
  Actor_SetAnimation(id, mode);
}

/// Sends a platform toward its home moved dx and dz cells.
void GaiaRockInside_PlacePlatform(s32 id, s32 dx, s32 dz) {
  struct Actor *platform = Actor_GetById(id);

  FieldActor_SetMoveTargetFX(platform, INTtoFIXED((dx << 4) + platform->timer),
                             platform->y.fixed,
                             INTtoFIXED((dz << 4) + platform->userValue));
}

extern struct SwayState gSceneState;

/// The platforms' frame task: every four seconds each platform glides three
/// cells out one way, and two seconds later back home. They all stop while the
/// menu is open or Psynergy is cast.
void GaiaRockInside_SwayPlatforms(void) {
  struct SwayState *state = &gSceneState;
  u16 *pending = &state->pending;
  u16 *frame = &state->frame;
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  s32 stopped = Event_GetFlag(FLAG_MENU_OPEN);
  s32 id;

  if (work->psynergy != 0)
    stopped |= 1;
  id = ACTOR_FIRST_PLATFORM;
  do {
    Actor_Get(id++)->paused = stopped;
  } while (id <= ACTOR_LAST_PLATFORM);
  if (stopped != 0)
    return;
  ++*pending;
  if (work->pushCell != 0)
    return;
  if (*pending == 0)
    return;
  do {
    switch (*frame) {
    case 60:
      GaiaRockInside_PlacePlatform(ACTOR_FIRST_PLATFORM, 0, 3);
      GaiaRockInside_PlacePlatform(ACTOR_FIRST_PLATFORM + 1, 3, 0);
      GaiaRockInside_PlacePlatform(ACTOR_FIRST_PLATFORM + 2, 0, 3);
      GaiaRockInside_PlacePlatform(ACTOR_FIRST_PLATFORM + 3, 0, 3);
      GaiaRockInside_PlacePlatform(ACTOR_FIRST_PLATFORM + 4, 0, -3);
      GaiaRockInside_PlacePlatform(ACTOR_FIRST_PLATFORM + 5, 0, -3);
      GaiaRockInside_PlacePlatform(ACTOR_FIRST_PLATFORM + 6, 0, 3);
      GaiaRockInside_PlacePlatform(ACTOR_FIRST_PLATFORM + 7, 0, -3);
      GaiaRockInside_PlacePlatform(ACTOR_LAST_PLATFORM, 3, 0);
      break;
    case 180:
      for (id = ACTOR_FIRST_PLATFORM; id <= ACTOR_LAST_PLATFORM; id++) {
        GaiaRockInside_PlacePlatform(id, 0, 0);
      }
      break;
    }
    if (++*frame == 240)
      *frame = 0;
  } while (--*pending != 0);
}

extern s16 gGaiaRockInside_PlatformPlaces[][2];

/// Sets the platforms swaying: unless the party keeps its place, clears the
/// sway's frame and places each platform, then starts the sway's task.
void GaiaRockInside_StartPlatforms(u8 mode) {
  u16 *frame = &gSceneState.frame;
  u16 *pending = &gSceneState.pending;

  if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION)) {
    s32 i;
    s32 id;

    *frame = 0;
    *pending = 0;
    Actor_Get(FieldParty_GetLeaderActor())->priorityFlags |=
        ACTOR_PRIORITY_WALKS_ON_PLATFORMS;
    for (i = 0, id = ACTOR_FIRST_PLATFORM; id <= ACTOR_LAST_PLATFORM;
         i++, id++) {
      Actor_SetPositionFX(
          id, FIELD_TILE_ORIGIN(gGaiaRockInside_PlatformPlaces[i][0]),
          FIELD_TILE_ORIGIN(gGaiaRockInside_PlatformPlaces[i][1]));
      GaiaRockInside_SetPlatformMode(id, mode);
    }
  }
  Task_AddFrame(GaiaRockInside_SwayPlatforms, TASK_PASS_FRAME);
}

/// Draws the beams of sunlight let into the lair, pale shafts slanting down
/// from its top left and top right: the altar room's and the west altar's on
/// the west side, the east altar's and the bushes' on the east. Each side has
/// a picture for each one of its pair alone and one for both, and a strip
/// between the sides that changes with what each shows.
void GaiaRockInside_DrawSunbeams(void) {
  u8 a = GameFlag_Get(FLAG_ALTAR_ROOM_SUNBEAM);
  u8 third = GameFlag_Get(FLAG_EAST_ALTAR_SUNBEAM);
  s32 b = GameFlag_Get(FLAG_WEST_ALTAR_SUNBEAM);
  s8 first = a;
  s8 sideThird = third;
  u8 sumLeft = b + first;
  s32 d = GameFlag_Get(FLAG_BUSHES_SUNBEAM);
  u8 sumRight = sideThird + d;
  s8 left = sumLeft;
  s8 right = sumRight;

  if (left != 0) {
    if (left == 2) {
      MapGrid_CopyCellsLayered(74, 115, 7, 12, 24, 76);
      if (right == 0)
        return;
      if (right == 2)
        MapGrid_CopyCellsLayered(71, 110, 1, 3, 30, 85);
      else if (sideThird != 0)
        MapGrid_CopyCellsLayered(67, 110, 1, 3, 30, 85);
      else
        MapGrid_CopyCellsLayered(79, 110, 1, 3, 30, 85);
    } else if (first != 0) {
      MapGrid_CopyCellsLayered(66, 95, 7, 12, 24, 76);
      if (right == 0)
        return;
      if (right == 2)
        MapGrid_CopyCellsLayered(81, 110, 1, 3, 30, 85);
      else if (sideThird != 0)
        MapGrid_CopyCellsLayered(75, 110, 1, 3, 30, 85);
      else
        MapGrid_CopyCellsLayered(65, 110, 1, 3, 30, 85);
    } else {
      MapGrid_CopyCellsLayered(69, 82, 5, 12, 26, 76);
      if (right == 0)
        return;
      if (right == 2)
        MapGrid_CopyCellsLayered(69, 110, 1, 3, 30, 85);
      else if (sideThird != 0)
        MapGrid_CopyCellsLayered(73, 110, 1, 3, 30, 85);
      else
        MapGrid_CopyCellsLayered(77, 110, 1, 3, 30, 85);
    }
  }
  if (right == 0)
    return;
  if (right == 2) {
    MapGrid_CopyCellsLayered(83, 115, 6, 12, 31, 76);
    if (left == 0)
      MapGrid_CopyCellsLayered(82, 124, 1, 3, 30, 85);
  } else if (sideThird != 0) {
    MapGrid_CopyCellsLayered(76, 82, 4, 12, 31, 76);
    if (left == 0)
      MapGrid_CopyCellsLayered(75, 91, 1, 3, 30, 85);
  } else {
    MapGrid_CopyCellsLayered(82, 82, 6, 12, 31, 76);
    if (left == 0)
      MapGrid_CopyCellsLayered(81, 91, 1, 3, 30, 85);
  }
}

#include "field/call.h"
#include "system/main_routine_work.h"
#include "system/ram_buffer.h"
#include "system/io_reg.h"
#include "field/field.h"

#define Test(flag) Event_GetFlag(flag)
extern u32 gGaiaRockInside_SerpentGlowScript[];
extern u32 gGaiaRockInside_SerpentSmokeScript[];
extern u32 gGaiaRockInside_KushinadaScript[];
extern s32 gGaiaRockInside_SusaFeedsSerpentScript[];

struct Actor *OverlayObject_PrepareObject(s32 x, s32 y, s32 z, s32 kind);
s32 __modsi3(s32 numerator, s32 denominator);
void GaiaRockInside_TurnFaceToAltar();
void GaiaRockInside_RingOfDust(void);
void StoneHead_Place(s32 id, s32 x, s32 y, s32 z);
void GaiaRockInside_StartWallTriggers(void);
void Actor_Release();
void BlendScript_Resume(void);
void BlendScript_Suspend(void);
void Actor_SetSpritePriorityDirect();
void Event_Start(void);
void Event_End(void);
void FieldCamera_WaitForArrival(void);
void FieldCamera_MoveToActorById();
void Event_SetDestination();
void FieldEffect_StartBlankOverlay();
void HangingVine_Place();
void WorldMap_SetReturnLeavingScene();
void Actor_SetAnimationSpeed();
void Actor_SetSpriteColorMode();
void Event_FinishScreenTransition(void);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);

/// Sets up the room the party arrives in. In the lair: the beams of sunlight
/// let in, the stone Serpent and the slate once it is slain, the scene of a
/// new beam falling into the lair (entrance 20), Susa's attack on the Serpent
/// the first time the party comes holding the Dancing Idol, and the scenes
/// after the battle with it (entrances 10 to 12). In the first altar's room,
/// the faces and stumps where the party left them; in the platforms' room, the
/// platforms once they sway; in the twin altars' room, the faces, beams and
/// fallen altar, and the altar landing as it falls from the cobweb (entrance
/// 6); in the altar room, the vine, the stone heads, the face and the fallen
/// cobweb.
s32 Scene_Initialize(void) {
  struct MainRoutineWork *room;
  struct MainRoutineWork *work;
  s16 *entrance;
  s16 *early;

  work = &gMainRoutineWork;
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  if (work->next == SCENE_GAIA_ROCK_INSIDE_MAZE) {
    early = &work->entrance;
    if (work->entrance > 8) {
      Actor_SetSpriteFlags(Actor_GetById(ACTOR_SERPENT), 0);
      GaiaRockInside_DrawSunbeams();
    }
    if (*early == GAIA_ROCK_INSIDE_MAZE_FROM_GAIA_ROCK_INSIDE_TWIN_ALTARS_A) {
      Event_Start();
      Event_ResolvePendingActions(0);
      FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
      Actor_SetPosition(work->leaderActorId, 0, 0);
      Actor_SetPosition(ACTOR_CASK, 0, 0);
      Actor_SetPosition(ACTOR_SUSA, 0, 0);
      Actor_Stop(ACTOR_CASK);
      Actor_Stop(ACTOR_SUSA);
      Event_StartScreenTransition();
      Event_WaitTransitionFrames();
      Event_Wait(60);
      {
        s32 ending = EXIT_NO_SUNBEAM;
        if (Test(FLAG_ALTAR_ROOM_SUNBEAM_TO_SHOW)) {
          GameFlag_Clear(FLAG_ALTAR_ROOM_SUNBEAM_TO_SHOW);
          GameFlag_Set(FLAG_ALTAR_ROOM_SUNBEAM);
          ending = GAIA_ROCK_INSIDE_MAZE_TO_GAIA_ROCK_INSIDE_ALTAR;
        }
        if (Test(FLAG_WEST_ALTAR_SUNBEAM_TO_SHOW)) {
          GameFlag_Clear(FLAG_WEST_ALTAR_SUNBEAM_TO_SHOW);
          GameFlag_Set(FLAG_WEST_ALTAR_SUNBEAM);
          ending = GAIA_ROCK_INSIDE_MAZE_TO_GAIA_ROCK_INSIDE_TWIN_ALTARS_B;
        }
        if (Test(FLAG_EAST_ALTAR_SUNBEAM_TO_SHOW)) {
          GameFlag_Clear(FLAG_EAST_ALTAR_SUNBEAM_TO_SHOW);
          GameFlag_Set(FLAG_EAST_ALTAR_SUNBEAM);
          ending = GAIA_ROCK_INSIDE_MAZE_TO_GAIA_ROCK_INSIDE_TWIN_ALTARS_C;
        }
        if (Test(FLAG_BUSHES_SUNBEAM_TO_SHOW)) {
          GameFlag_Clear(FLAG_BUSHES_SUNBEAM_TO_SHOW);
          GameFlag_Set(FLAG_BUSHES_SUNBEAM);
          ending = GAIA_ROCK_INSIDE_MAZE_TO_GAIA_ROCK_INSIDE_TWIN_ALTARS_D;
        }
        Sound_Play(SE_EYES_GLOW);
        GaiaRockInside_DrawSunbeams();
        Event_Wait(15);
        Actor_SetAnimationById(ACTOR_SERPENT, 3);
        Sound_Play(SE_SEA_OF_TIME_OPENS);
        MapGrid_SetShake(2, 2, 1);
        MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.899993896484375);
        Event_Wait(120);
        Actor_SetAnimationById(ACTOR_SERPENT, 1);
        Event_Wait(60);
        GameFlag_Set(FLAG_KEEP_PARTY_POSITION);
        Import_EventSetExitRequest(ending);
      }
    }
    if ((u16)(gMainRoutineWork.entrance - ENTRANCE_LAIR) <=
        ENTRANCE_SERPENT_BEATEN - ENTRANCE_LAIR) {
      GameFlag_Set(FLAG_RETREAT_BLOCKED);
      if (Test(FLAG_SERPENT_SLAIN)) {
        Actor_SetAnimationById(ACTOR_SERPENT, 4);
        MapGrid_CopyCellsLayered(85, 72, 5, 2, 30, 19);
        Actor_SetPosition(ACTOR_CASK, 0, 0);
        Actor_SetPosition(ACTOR_SUSA, 0, 0);
      }
      if (Test(FLAG_SAND_TABLET_RAISED) && !Test(FLAG_LEARNED_SAND))
        Actor_SetPosition(ACTOR_SAND_TABLET, 464, 328);
    }
    room = &gMainRoutineWork;
    entrance = &room->entrance;
    if (*entrance == ENTRANCE_LAIR) {
      s32 found;

      if (!Test(FLAG_SERPENT_SLAIN)) {
        Sound_Play(MUS_BATTLE_WITH_FLAME_DRAGON);
        found = FieldPartyInventory_FindMember(ITEM_DANCING_IDOL);
        if (found != -1) {
          s32 flag;
          struct Actor *susa;
          struct Actor *serpent;
          s32 width;
          s32 height;
          s32 mask;
          FIXED fast;
          s32 i;
          s32 dx;
          s32 dz;
          s32 movement;
          struct Actor *actor;
          struct Actor *shards[4];

          flag = Test(FLAG_SUSA_ATTACKED_SERPENT);
          if (flag != 0) {
            Actor_SetAnimationById(ACTOR_SERPENT, 2);
            Actor_SetPosition(ACTOR_CASK, 0, 0);
            Actor_SetPosition(ACTOR_SUSA, 432, 320);
            Actor_SetSpritePriority(ACTOR_SUSA, 3);
            MapGrid_CopyCellAttributes(30, 19, 1, 1, 27, 19);
            Actor_SetAnimationById(ACTOR_SUSA, 5);
          } else {
            susa = Actor_GetById(ACTOR_SUSA);
            serpent = Actor_GetById(ACTOR_SERPENT);
            Sound_Play(SOUND_FADE_OUT_ACTION_SE);
            GameFlag_Set(FLAG_SUSA_ATTACKED_SERPENT);
            GameFlag_Set(FLAG_MET_SUSA);
            Event_Start();
            Event_ResolvePendingActions(0);
            Actor_SetPosition(ACTOR_CASK, 0, 0);
            Actor_SetMovementTypeById(ACTOR_CASK, MOVEMENT_TYPE_STAND);
            Actor_SetPosition(ACTOR_SUSA, 456, 360);
            Actor_SetMovementTypeById(ACTOR_SUSA, MOVEMENT_TYPE_STAND);
            Actor_FaceDirection(ACTOR_SUSA, FACING_NORTHEAST, 0);
            Actor_SetAnimationById(ACTOR_SERPENT, 2);
            Event_StartScreenTransition();
            Event_WaitTransitionFrames();
            FieldCamera_MoveToFX(toFIXED(488), -1, toFIXED(384), 1);
            FieldCamera_WaitForArrival();
            Actor_ShowEmoteById(ACTOR_SUSA, EMOTE_IN_FRONT | EMOTE_SMILE, 0);
            Event_Wait(40);
            Event_SetMessageId((s32)MsgFieldAtLastTheSerpentHas);
            Event_ShowMessageAndWait(ACTOR_SUSA, 0);
            Actor_SetAnimationAndWait(ACTOR_SUSA, ANIM_NOD);
            Event_ShowMessageAndWait(ACTOR_SUSA, 0);
            Actor_SetAnimationById(ACTOR_SUSA, 8);
            Event_ShowMessageAndWait(
                SPEAKER_WINDOW_ABOVE | SPEAKER_PORTRAIT_RIGHT | ACTOR_SUSA, 0);
            susa->speedLimit = toFIXED(3) / 2;
            Actor_SetDestinationById(ACTOR_SUSA, 488, 328);
            Event_Wait(5);
            Actor_SetAnimationById(ACTOR_SUSA, 6);
            Event_Wait(10);
            susa->velocityY = toFIXED(6);
            Event_Wait(10);
            susa->paused = 1;
            serpent->paused = 1;
            Actor_SetAnimationSpeed(susa, 0);
            Actor_SetAnimationSpeed(serpent, 0);
            Actor_SetSpriteColorMode(susa, 6);
            Actor_SetSpriteColorMode(serpent, 6);
            width = 30;
            height = 19;
            MapGrid_CopyCellPictureCellsRect(85, 76, 5, 2, width, height);
            Sound_Play(SE_CRUMBLE);
            PaletteFade_SetTarget(RGB_WHITE, 0);
            PaletteFade_StartInterpolation(10);
            Event_Wait(10);
            PaletteFade_SetTarget(PALETTE_TINT_NONE, 0);
            PaletteFade_StartInterpolation(10);
            Event_Wait(10);
            Event_Wait(60);
            Actor_SetSpriteColorMode(susa, 0);
            Actor_SetSpriteColorMode(serpent, 0);
            MapGrid_CopyCellPictureCellsRect(85, 74, 5, 2, width, height);
            susa->paused = 0;
            serpent->paused = 0;
            Actor_SetAnimationSpeed(susa, ANIM_STEP_NORMAL);
            Actor_SetAnimationSpeed(serpent, ANIM_STEP_NORMAL);
            mask = (u8)~ACTOR_FACES_MOTION;
            Actor_GetById(ACTOR_SUSA)->steeringFlags &= mask;
            Actor_WalkTo(ACTOR_SUSA, 456, 360);
            fast = toFIXED(4);
            susa->velocityY = fast;
            Actor_SetAttachedEffect(ACTOR_SUSA, ATTACHED_EFFECT_IN_FRONT |
                                                    ATTACHED_EFFECT_ONCE);
            Actor_WaitForMoveById(ACTOR_SUSA);
            Actor_ShowEmoteById(ACTOR_SUSA, EMOTE_IN_FRONT | EMOTE_QUESTION,
                                20);
            Event_Wait(30);
            Actor_SetAnimationById(ACTOR_SUSA, 8);
            Event_Wait(10);
            susa->speedLimit = toFIXED(3) / 2;
            Actor_SetDestinationById(ACTOR_SUSA, 488, 328);
            Event_Wait(14);
            susa->velocityY = toFIXED(6);
            Actor_SetAnimationById(ACTOR_SUSA, 6);
            Event_Wait(10);
            Sound_Play(SE_CRUMBLE);
            {
              struct Actor *a = Actor_GetById(ACTOR_SUSA);
              mask &= a->steeringFlags;
              a->steeringFlags = mask;
            }
            Actor_WalkToById(ACTOR_SUSA, 456, 360);
            susa->velocityY = fast;
            Actor_SetAttachedEffect(ACTOR_SUSA, ATTACHED_EFFECT_IN_FRONT |
                                                    ATTACHED_EFFECT_ONCE);
            Actor_WaitForMoveById(ACTOR_SUSA);
            Event_ShowMessageAndWait(ACTOR_SUSA, 0);
            Event_Wait(15);
            Actor_SetAnimationSpeed(serpent, 0);
            Event_Wait(15);
            Sound_Play(SE_SPLASH_SMALL);
            actor = FieldActor_Create(SPRITE_DUST_CLOUD, toFIXED(488),
                                      -toFIXED(16), toFIXED(345));
            Actor_SetSpriteFlags(actor, 0);
            Actor_StartScript(actor, gGaiaRockInside_SerpentSmokeScript);
            Actor_SelectAnimation(actor, 1);
            Actor_SetSpriteColorMode(actor, 6);
            Actor_Jump(ACTOR_SUSA, 2, 0);
            Event_Wait(5);
            Actor_ShowEmoteById(ACTOR_SUSA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
            Event_Wait(55);
            Event_Wait(5);
            Sound_Play(SE_WHOOSH);
            actor = FieldActor_Create(SPRITE_FIREBALL, toFIXED(488),
                                      -toFIXED(16), toFIXED(345));
            Actor_SetSpriteFlags(actor, 0);
            Actor_StartScript(actor, gGaiaRockInside_SerpentGlowScript);
            ((struct FieldSprite *)actor->sprite)->rotation = DEGtoANG(45);
            actor->speedLimit = toFIXED(3) / 2;
            *(u8 *)&actor->motionFlags = 0;
            actor->targetX = toFIXED(456);
            actor->targetY = -toFIXED(32);
            actor->targetZ = toFIXED(358);
            actor->scaleX = FIXED_ONE * 3 / 8;
            actor->scaleY = FIXED_ONE * 3 / 8;
            Event_Wait(10);
            Actor_SetAnimationSpeed(serpent, ANIM_STEP_NORMAL);
            Event_Wait(10);
            Sound_Play(SE_IMPACT);
            actor->x.fixed = 0;
            actor->z.fixed = 0;
            MapGrid_SetScreenShake(fast, fast, FIXED_ONE);
            for (i = 0; i < 4; i++) {
              dx = -8;
              if (i > 1)
                dx = 8;
              dz = -8;
              if ((i & 1) == 0)
                dz = 8;
              shards[i] = (struct Actor *)Import_ActorCreate(
                  SPRITE_FLAME_PUFF, toFIXED(456) + INTtoFIXED(dx),
                  -toFIXED(32), toFIXED(358) + INTtoFIXED(dz));
              Actor_SelectAnimation(shards[i], 2);
              Actor_SetSpriteFlags(shards[i], 0);
              Actor_StartScript(shards[i], gGaiaRockInside_SerpentSmokeScript);
            }
            susa->velocityY = toFIXED(10);
            Actor_GetById(ACTOR_SUSA)->steeringFlags &= ~ACTOR_FACES_MOTION;
            Actor_WalkToById(ACTOR_SUSA, 432, 320);
            Event_Wait(10);
            MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.899993896484375);
            Actor_SetAnimationById(ACTOR_SUSA, 5);
            Event_Wait(30);
            FieldCamera_MoveToActorById(ACTOR_SUSA, 1);
            FieldCamera_WaitForArrival();
            movement = (s32)gGaiaRockInside_SquashXScript;
            Actor_SetMovementTypeById(ACTOR_SUSA, movement);
            Actor_WaitForScriptEndById(ACTOR_SUSA);
            Event_Wait(20);
            Actor_SetMovementTypeById(ACTOR_SUSA, movement);
            Event_Wait(20);
            Event_ShowMessageAndWait(
                SPEAKER_WINDOW_ABOVE | SPEAKER_PORTRAIT_RIGHT | ACTOR_SUSA, 0);
            FieldCamera_FollowActor(gMainRoutineWork.leaderActorId, 1);
            FieldCamera_WaitForArrival();
            Actor_SetSpritePriority(ACTOR_SUSA, 3);
            MapGrid_CopyCellAttributes(30, 19, 1, 1, 27, 19);
            Event_End();
          }
        } else {
          if (!Test(FLAG_KEEP_PARTY_POSITION)) {
            Actor_SetPosition(ACTOR_SUSA, 472, 456);
            Actor_SetMovementTypeById(ACTOR_SUSA,
                                      gGaiaRockInside_SusaFeedsSerpentScript);
          }
        }
      }
    } else if (*entrance == ENTRANCE_SERPENT_BEATEN) {
      s32 flag;
      FIXED scale;
      struct Actor *susa;
      s32 slow;
      s32 *owner;
      struct Actor *other;
      struct Actor *actor;
      volatile u16 alpha;
      s32 i;

      Sound_Play(MUS_IZUMO);
      flag = Test(FLAG_SERPENT_SLAIN);
      if (flag == 0) {
        Event_Start();
        Event_ResolvePendingActions(0);
        GameFlag_Set(FLAG_SERPENT_SLAIN);
        Event_SetMessageId((s32)MsgFieldItLooksLikeTheDragon);
        Actor_SetPosition(ACTOR_CASK, 0, 0);
        Actor_SetAnimationById(ACTOR_SERPENT, 3);
        Actor_GetById(ACTOR_KRADEN)->speedLimit = scale = toFIXED(1);
        slow = FACING_WEST;
        susa = Actor_GetById(ACTOR_SUSA);
        susa->speedLimit = slow;
        Actor_SetPosition(ACTOR_KRADEN, 440, 312);
        owner = &room->leaderActorId;
        Actor_SetPosition(*owner, 456, 328);
        Actor_SetPosition(ACTOR_SUSA, 488, 424);
        Actor_FaceDirectionById(*owner, FACING_EAST, 0);
        Actor_FaceDirectionById(ACTOR_KRADEN, FACING_EAST, 0);
        Actor_FaceDirection(ACTOR_SUSA, FACING_NORTH, 0);
        Event_StartScreenTransition();
        Event_WaitTransitionFrames();
        Actor_ShowEmoteById(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
        Event_PlaySound(SE_SEA_OF_TIME_OPENS);
        Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
        Event_PlaySound(SE_SEA_OF_TIME_OPENS);
        Event_ShowMessageAndWait(ACTOR_SUSA, 0);
        Actor_ShowEmote(*owner, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
        Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
        Event_Wait(15);
        Actor_FaceDirection(*owner, FACING_SOUTH, 0);
        Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH, 0);
        Event_Wait(30);
        FieldCamera_MoveToActorById(ACTOR_SUSA, 1);
        FieldCamera_WaitForArrival();
        Actor_StartRepeatedMotion(ACTOR_SUSA, 2);
        FieldCamera_FollowActor(ACTOR_SUSA, 1);
        Actor_WalkTo(ACTOR_SUSA, 488, 376);
        Actor_WaitForMoveById(ACTOR_SUSA);
        Actor_StartRepeatedMotion(ACTOR_SUSA, 2);
        Event_Wait(20);
        Actor_WalkToById(ACTOR_SUSA, 472, 360);
        Actor_WaitForMoveById(ACTOR_SUSA);
        Actor_FaceActorById(*owner, ACTOR_SUSA, 0);
        Actor_FaceActorById(ACTOR_KRADEN, ACTOR_SUSA, 0);
        Actor_FaceActorById(ACTOR_SUSA, ACTOR_SERPENT, 0);
        FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
        Actor_SetAnimationById(ACTOR_SUSA, 8);
        Event_ShowMessageAndWait(ACTOR_SUSA, 0);
        susa->speedLimit = toFIXED(3) / 2;
        Actor_SetDestination(ACTOR_SUSA, 488, 328);
        Event_Wait(5);
        Actor_SetAnimationById(ACTOR_SUSA, 6);
        Event_Wait(10);
        susa->velocityY = toFIXED(6);
        Event_Wait(10);
        Sound_Play(SE_CRUMBLE);
        susa->paused = 1;
        PaletteFade_ApplyTarget(RGB_WHITE, 0);
        PaletteFade_StartInterpolation(10);
        Event_Wait(10);
        Sound_Play(SE_SEA_OF_TIME_OPENS);
        PaletteFade_SetTarget(scale, 0);
        PaletteFade_StartInterpolation(10);
        Event_Wait(10);
        PaletteFade_ApplyTarget(RGB_WHITE, 0);
        PaletteFade_StartInterpolation(10);
        Event_Wait(10);
        PaletteFade_SetTarget(scale, 0);
        PaletteFade_StartInterpolation(10);
        Event_Wait(10);
        PaletteFade_SetTarget(RGB_WHITE, 0);
        PaletteFade_StartInterpolation(10);
        Event_Wait(60);
        Actor_SetAnimationById(ACTOR_SERPENT, 4);
        MapGrid_CopyCellsLayered(85, 72, 5, 2, 30, 19);
        susa->y.fixed = -toFIXED(32);
        susa->velocityY = 0;
        susa->speedLimit = slow;
        Actor_SetPosition(ACTOR_SUSA, 456, 360);
        Actor_SetAnimationById(ACTOR_SUSA, ANIM_STAND);
        PaletteFade_SetTarget(scale, 0);
        PaletteFade_StartInterpolation(10);
        Event_Wait(10);
        Actor_FaceActorById(ACTOR_SUSA, *owner, 0);
        Event_ShowMessageAndWait(ACTOR_SUSA, 0);
        Actor_SetAnimationAndWait(ACTOR_SUSA, ANIM_NOD);
        Event_ShowMessageAndWait(ACTOR_SUSA, 0);
        Actor_FaceDirection(ACTOR_SUSA, FACING_SOUTH, 0);
        Actor_SetAnimationById(ACTOR_SUSA, ANIM_WALK);
        Actor_WalkTo(ACTOR_SUSA, 488, 392);
        Actor_StartRepeatedMotion(ACTOR_KRADEN, 1);
        Event_Wait(5);
        Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
        Actor_WaitForMoveById(ACTOR_SUSA);
        Actor_SetAnimationById(ACTOR_SUSA, ANIM_STILL);
        Actor_FaceActorById(ACTOR_SUSA, ACTOR_KRADEN, 0);
        Event_Wait(15);
        Event_ShowMessageAndWait(ACTOR_SUSA, 0);
        Actor_SetAnimationAndWait(ACTOR_SUSA, ANIM_NOD);
        Actor_FaceDirection(ACTOR_SUSA, FACING_SOUTH, 0);
        Actor_SetAnimationById(ACTOR_SUSA, ANIM_WALK);
        Actor_WalkTo(ACTOR_SUSA, 488, 536);
        FieldCamera_FollowActor(*owner, 1);
        Actor_FaceActorById(*owner, ACTOR_KRADEN, 0);
        Actor_FaceActorById(ACTOR_KRADEN, *owner, 0);
        Event_Wait(30);
        Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
        Actor_SetAnimationById(ACTOR_KRADEN, ANIM_WALK);
        {
          struct Actor *ownerActor = Actor_GetById(*owner);

          if (ownerActor != NULL)
            Actor_SetDestinationById(ACTOR_KRADEN,
                                     FIXEDtoINT(ownerActor->x.fixed),
                                     FIXEDtoINT(ownerActor->z.fixed));
        }
        Actor_WaitForMoveById(ACTOR_KRADEN);
        Actor_SetPosition(ACTOR_KRADEN, 0, 0);
        Actor_WaitForMoveById(ACTOR_KRADEN);
        Actor_SetPosition(ACTOR_SUSA, 0, 0);
        Actor_WalkToById(*owner, 488, 376);
        Actor_WaitForMoveById(*owner);
        MapGrid_SetShakeFX(toFIXED(5), toFIXED(5), scale);
        Sound_Play(SE_TREMOR);
        Event_Wait(30);
        Actor_ShowEmoteById(*owner, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
        Event_Wait(15);
        Actor_FaceDirectionById(*owner, FACING_NORTH, 0);
        Event_Wait(15);
        other = Actor_GetById(ACTOR_SAND_TABLET);
        alpha = REG_BLDALPHA;
        MapGrid_SetShakeFX(toFIXED(10), toFIXED(10), scale);
        Sound_Play(SE_TREMOR);
        BlendScript_Suspend();
        alpha = *(u8 *)&alpha;
        while (alpha != 0) {
          REG_BLDALPHA = alpha | BLEND_ALPHA(0, 16);
          alpha = alpha - 1;
          wait(1);
        }
        REG_DISPCNT &= (u16)~DISPCNT_BG1_ON;
        Event_Wait(30);
        actor = OverlayObject_PrepareObject(toFIXED(464), -toFIXED(32),
                                            toFIXED(348), SPRITE_SLATE);
        Actor_SetPosition(ACTOR_SAND_TABLET, 464, 348);
        for (i = 0; i < TABLET_RISE_PIXELS; i++) {
          if (i % 5 == 0)
            GaiaRockInside_RingOfDust();
          other->z.fixed += -toFIXED(1);
          Event_Wait(2);
        }
        GameFlag_Set(FLAG_SAND_TABLET_RAISED);
        MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.899993896484375);
        actor->x.fixed = 0;
        actor->z.fixed = 0;
        Event_Wait(1);
        Actor_Release(actor);
        Sound_Play(SOUND_FADE_OUT_ACTION_SE);
        Event_Wait(30);
        REG_DISPCNT |=
            DISPCNT_BG0_ON | DISPCNT_BG1_ON | DISPCNT_BG2_ON | DISPCNT_BG3_ON;
        REG_BLDALPHA = alpha | BLEND_ALPHA(0, 16);
        REG_BLDCNT = BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
        while (alpha != 8) {
          REG_BLDALPHA = alpha | BLEND_ALPHA(0, 16);
          alpha++;
          wait(1);
        }
        BlendScript_Resume();
        Event_End();
      }
    } else if (*entrance == ENTRANCE_BEATEN_BY_SERPENT) {
      Event_Start();
      Event_ResolvePendingActions(0);
      Actor_SetPosition(ACTOR_CASK, 0, 0);
      Actor_SetPosition(ACTOR_SUSA, 448, 344);
      {
        s32 *lead = &room->leaderActorId;

        Actor_FaceActorById(ACTOR_SUSA, *lead, 0);
        FieldActor_SetAnimation(*lead, 19);
      }
      Event_StartScreenTransition();
      Event_WaitTransitionFrames();
      Event_SetMessageId((s32)MsgFieldYouYouCannotFightThe);
      Event_ShowMessageAndWait(ACTOR_SUSA, 0);
      Event_FinishScreenTransition();
      Event_WaitTransitionFrames();
      Event_SetDestination((s16)room->restartScene, (s16)room->restartEntrance);
#if TLA_PATCH_1
      WorldMap_SetReturnLeavingScene(room->next, *entrance);
#endif
      Event_End();
    } else if (*entrance == ENTRANCE_BEATEN_HOLDING_IDOL) {
      Event_Start();
      Event_ResolvePendingActions(0);
      Actor_SetPosition(ACTOR_CASK, 0, 0);
      Actor_SetPosition(ACTOR_SUSA, 430, 320);
      Actor_SetAnimationById(ACTOR_SUSA, 5);
      Actor_SetAnimationById(room->leaderActorId, 19);
      Event_StartScreenTransition();
      Event_WaitTransitionFrames();
      Event_Wait(30);
      Value2((s32 (*)())Actor_StartRepeatedMotion, ACTOR_SUSA, 2);
      Event_Wait(30);
      Event_SetMessageId((s32)MsgFieldLikeISaidTheSerpent);
      Event_ShowMessageAndWait(ACTOR_SUSA, 0);
      Event_FinishScreenTransition();
      Event_WaitTransitionFrames();
      Event_SetDestination((s16)room->restartScene, (s16)room->restartEntrance);
#if TLA_PATCH_1
      WorldMap_SetReturnLeavingScene(room->next, *entrance);
#endif
      Event_End();
    }
  } else if (work->next == SCENE_GAIA_ROCK_INSIDE_FIRST_ALTAR) {
    s32 mask = ACTOR_PRIORITY_IGNORES_RISES;
    s32 low;
    s32 high;
    struct Actor *actor = Actor_Get(ACTOR_FIRST_ALTAR_WEST_FACE);

    actor->priorityFlags |= mask;
    Actor_SetSpritePriorityDirect(actor, 3);
    actor = Actor_Get(ACTOR_FIRST_ALTAR_EAST_FACE);
    actor->priorityFlags |= mask;
    Actor_SetSpritePriorityDirect(actor, 3);
    GaiaRockInside_TurnFaceToAltar(ACTOR_FIRST_ALTAR_WEST_FACE,
                                   ACTOR_FIRST_ALTAR);
    GaiaRockInside_TurnFaceToAltar(ACTOR_FIRST_ALTAR_EAST_FACE,
                                   ACTOR_FIRST_ALTAR);
    if (Test(FLAG_FIRST_ALTAR_FACES_MOVED)) {
      Actor_SetPosition(ACTOR_FIRST_ALTAR_WEST_FACE, 440, 688);
      Actor_SetPosition(ACTOR_FIRST_ALTAR_EAST_FACE, 552, 688);
    } else {
      s32 height = 40;

      Map_CopyCellsRect(88, 40, 1, 2, 89, height);
      Map_CopyCellsRect(99, 40, 1, 2, 100, height);
    }
    low = FieldGameFlag_GetByte(FLAG_STUMP_SPOTS) & 0xf;
    high = (u8)(FieldGameFlag_GetByte(FLAG_STUMP_SPOTS) >> 4);
    if (low != 0)
      Actor_SetHorizontalPositionWithTerrain(
          ACTOR_STUMP_A, FIELD_TILE_ORIGIN(gGaiaRockInside_StumpSpots[low][0]),
          FIELD_TILE_ORIGIN(gGaiaRockInside_StumpSpots[low][1]));
    if ((s8)high != 0)
      Actor_SetHorizontalPositionWithTerrain(
          ACTOR_STUMP_B,
          FIELD_TILE_ORIGIN(gGaiaRockInside_StumpSpots[(s8)high][0]),
          FIELD_TILE_ORIGIN(gGaiaRockInside_StumpSpots[(s8)high][1]));
  } else if (work->next == SCENE_GAIA_ROCK_INSIDE_PLATFORMS) {
    if (Test(FLAG_PLATFORMS_SWAYING))
      GaiaRockInside_StartPlatforms(1);
  } else if (work->next == SCENE_GAIA_ROCK_INSIDE_TWIN_ALTARS) {
    struct Actor *actor;
    struct Actor *other;

    actor = Actor_GetById(ACTOR_WEST_ALTAR);
    Actor_SetSpritePriorityDirect(actor, 3);
    GaiaRockInside_TurnFaceToAltar(ACTOR_WEST_FACE, ACTOR_WEST_ALTAR);
    GaiaRockInside_TurnFaceToAltar(ACTOR_EAST_FACE, ACTOR_EAST_ALTAR);
    if (!Test(FLAG_ALTAR_ROOM_SUNBEAM))
      MapGrid_CopyCells(25, 77, 1, 15, 26, 77);
    if (Test(FLAG_WEST_ALTAR_SUNBEAM)) {
      s32 width;
      s32 height;

      Actor_SetPosition(ACTOR_WEST_FACE, 480, 352);
      width = 28;
      Map_CopyCellsRect(2, 93, 1, 5, width, 85);
      height = 22;
      Map_CopyCellAttributeRect(27, 23, 1, 1, width, height);
      Map_CopyCellAttributeRect(32, 16, 2, 1, 29, height);
    }
    if (Test(FLAG_EAST_ALTAR_SUNBEAM)) {
      s32 width;
      s32 height;

      Actor_SetPosition(ACTOR_EAST_FACE, 608, 352);
      width = 36;
      Map_CopyCellsRect(2, 93, 1, 5, width, 85);
      height = 22;
      Map_CopyCellAttributeRect(35, 23, 1, 1, width, height);
      Map_CopyCellAttributeRect(32, 16, 2, 1, 37, height);
    }
    if (Test(FLAG_BUSHES_SUNBEAM)) {
      MapGrid_CopyCellsLayered(61, 0, 3, 3, 37, 26);
      MapGrid_CopyCellAttributes(36, 23, 1, 1, 38, 25);
    }
    if (work->entrance ==
        GAIA_ROCK_INSIDE_TWIN_ALTARS_FROM_GAIA_ROCK_INSIDE_ALTAR_SOUTHEAST_B) {
      s32 frames = 60;

      Event_Start();
      Event_ResolvePendingActions(0);
      FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
      Actor_SetPosition(ACTOR_EAST_ALTAR, 0, 0);
      Actor_SetPosition(work->leaderActorId, 0, 0);
      Event_StartScreenTransition();
      Event_Wait(15);
      Actor_SetPosition(ACTOR_EAST_ALTAR, 672, 384);
      Actor_SetSpriteFlags(Actor_GetById(ACTOR_EAST_ALTAR), 0);
      actor = Actor_GetById(ACTOR_EAST_ALTAR);
      other = FieldActor_Create(SPRITE_LIGHT, actor->x.fixed, actor->y.fixed,
                                actor->z.fixed);
      if (other != NULL) {
        Actor_SetSpriteFlags(other, 0);
        Actor_StartScript(other, gGaiaRockInside_KushinadaScript);
        Actor_SelectAnimation(other, 1);
        Actor_SetSpriteColorMode(other, SPRITE_COLOR_BLACK);
        // The high byte of its OAM attribute 2: drawn at priority 3.
        ((struct FieldSprite *)other->sprite)->priority = 3;
        other->scaleX = FIXED_ONE * 6 / 5;
        other->scaleY = FIXED_ONE * 6 / 5;
      }
      actor->y.fixed += toFIXED(128);
      for (; frames != 0; frames--) {
        wait(1);
        if (actor->velocityY > -1)
          break;
      }
      Sound_Play(SE_THUMP);
      Event_Wait(30);
      GameFlag_Set(FLAG_KEEP_PARTY_POSITION);
      Import_EventSetExitRequest(
          GAIA_ROCK_INSIDE_TWIN_ALTARS_TO_GAIA_ROCK_INSIDE_ALTAR_SOUTHEAST_A);
    }
    if (!Test(FLAG_ALTAR_FELL))
      Actor_SetPosition(ACTOR_EAST_ALTAR, 0, 0);
#if TLA_PATCH_1
    if (!Test(FLAG_KEEP_PARTY_POSITION) && gMainRoutineWork.entrance == 3)
      GaiaRockInside_RaiseDust();
#endif
  } else if (work->next == SCENE_GAIA_ROCK_INSIDE_ALTAR) {
    HangingVine_Place(ACTOR_HANGING_VINE, 1);
    GaiaRockInside_TurnFaceToAltar(ACTOR_ALTAR_FACE, ACTOR_ALTAR);
    if (work->entrance == 1 || work->entrance == 3 || work->entrance == 5)
      GaiaRockInside_StartWallTriggers();
    if (Test(FLAG_STATUE_A_RISEN))
      StoneHead_Place(ACTOR_STATUE_A, toFIXED(384), toFIXED(75), toFIXED(680));
    if (Test(FLAG_STATUE_B_RISEN))
      StoneHead_Place(ACTOR_STATUE_B, toFIXED(463), toFIXED(51), toFIXED(680));
    if (Test(FLAG_STATUE_C_RISEN))
      StoneHead_Place(ACTOR_STATUE_C, toFIXED(565), toFIXED(55), toFIXED(680));
    if (Test(FLAG_ALTAR_ROOM_SUNBEAM)) {
      s32 width;
      s32 height;

      Actor_SetPosition(ACTOR_ALTAR_FACE, 384, 368);
      width = 25;
      Map_CopyCellsRect(1, 85, 1, 4, width, 85);
      height = 23;
      Map_CopyCellAttributeRect(27, 15, 2, 1, height, height);
      Map_CopyCellAttributeRect(25, 25, 1, 1, width, height);
    }
    Actor_SetSpriteFlags(Actor_GetById(ACTOR_FALLING_ALTAR), 0);
    if (Test(FLAG_ALTAR_FELL)) {
      s32 height = 20;

      MapGrid_CopyCellPictureCellsRect(117, 20, 7, 8, 104, height);
      MapGrid_CopyCellPictureCellsRect(111, 12, 1, 6, 111, height);
      MapGrid_CopyCellAttributes(49, 84, 8, 8, 40, 84);
      Actor_SetPosition(ACTOR_FALLING_ALTAR, 0, 0);
    }
  }
  FieldEffect_StartBlankOverlay(0);
  return 0;
}

/// The parts of the altar rooms' overhead layer, each marked by its cells'
/// collision code: arriving keeps only the part around the entrance.
enum {
  OVERHEAD_PART_A = 100,
  OVERHEAD_PART_B,
  OVERHEAD_PART_C,
  OVERHEAD_PART_D
};

/// Blanks the overhead layer of an altar room but for the part keep, the one
/// around the entrance the party arrives by.
void GaiaRockInside_KeepOverheadPart(s32 keep) {
  GaiaRockInside_BlankLayer(1, keep);
}

struct Unit *Unit_Get(u32 unitId);
void BlendScript_Start(u16 *script);

/// Prepares the map. In the maze it lays the path, from the letters of the
/// leader's name, once a visit, and draws the room the party stands in, with
/// the Mimic's chest in the third room and the Rusty Mace's in the ninth, and
/// the plant's arrow if Growth has shown it; in the lair it starts the slow
/// shimmer of its overhead layer, where the beams of sunlight are drawn. In the
/// first altar's room and the altar room it blanks the overhead layer but for
/// the part around the entrance.
s32 Scene_PrepareMap(void) {
  if (gMainRoutineWork.next == SCENE_GAIA_ROCK_INSIDE_MAZE) {
    u8 *map = (u8 *)Ram_HeapSlots->mapGrid;

    if (gMainRoutineWork.entrance <= 4) {
      struct Unit *unit;
      s32 total;
      s32 *stage;
      s32 i;

      total = 0;
      unit = Unit_Get(gMainRoutineWork.leaderActorId);

      for (i = 0; i < UNIT_NAME_SIZE - 1; i++)
        total += unit->name[i];
      GaiaRockInside_LayMaze(total);
      Actor_GetById(Party_GetLeaderActor())->priorityFlags |=
          ACTOR_PRIORITY_IGNORES_RISES;
      ((struct MapWork *)map)->enabled[1] = 1;
      {
        u16 control;

        *&control = REG_BG2CNT & ~3;
        Io_Write16(control | 1, &REG_BG2CNT);
      }
      GaiaRockInside_DrawMazeRoom();
      stage = &gPath->current;
      if (*stage == 3 && !GameFlag_Get(FLAG_AREA_TEMP_F6))
        Actor_SetPosition(FIELD_TREASURE_ACTOR, 104, 88);
      if (*stage == 9)
        Actor_SetPosition(FIELD_TREASURE_ACTOR + 1, 104, 72);
      if (GameFlag_Get(FLAG_MAZE_PLANT_POINTED(*stage))) {
        struct Actor *plant = Actor_GetById(ACTOR_ARROW_PLANT);
        u32 angle = GaiaRockInside_GetMazeWayOn();

        plant->collisionFlags = 0;
        Actor_SelectAnimation(plant, ARROW_ANIMATION(angle));
        plant->update = ARROW_UP_OR_DOWN(angle) != 0
                            ? (ObjectUpdate)GaiaRockInside_PulseUpDownArrow
                            : (ObjectUpdate)GaiaRockInside_PulseSidewaysArrow;
        plant->collisionFlags = 1;
        GameFlag_Set(FLAG_PLANT_POINTING);
      }
    } else if (gMainRoutineWork.entrance > 8) {
      struct MapLayer *layer;

      gPath->current = 1;
      layer = &((struct MapWork *)map)->layers[2];
      layer->maskX = MAPGRID_SIZE - 1;
      layer->speedX = 0;
      layer->speedY = 0;
      layer->phaseX = 0;
      layer->phaseY = 0;
      layer->scaleX = FIXED_ONE;
      layer->scaleY = FIXED_ONE;
      layer->maskY = MAPGRID_ROW_MASK;
      layer->x = Iwram_MulFX(((struct MapWork *)map)->viewX, layer->scaleX) +
                 layer->offsetX;
      layer->y = Iwram_MulFX(((struct MapWork *)map)->viewZ, layer->scaleY) +
                 layer->offsetY;
      MapGrid_CenterOnFollowedPoint();
      System_Wait(1);
      Iwram_CopyWords(MAPGRID_BLEND_SCRIPT, gGaiaRockInside_LairBlendScript,
                      sizeof(gGaiaRockInside_LairBlendScript));
      BlendScript_Start((u16 *)MAPGRID_BLEND_SCRIPT);
    }
  } else if (gMainRoutineWork.next == SCENE_GAIA_ROCK_INSIDE_FIRST_ALTAR) {
    switch (gMainRoutineWork.entrance) {
    case GAIA_ROCK_INSIDE_FIRST_ALTAR_FROM_GAIA_ROCK_SUSA:
    case 2:
    case 5:
    case 8:
      GaiaRockInside_KeepOverheadPart(OVERHEAD_PART_A);
      break;
    case 9:
    case GAIA_ROCK_INSIDE_FIRST_ALTAR_FROM_GAIA_ROCK_INSIDE_PLATFORMS_EAST:
      GaiaRockInside_KeepOverheadPart(OVERHEAD_PART_C);
      break;
    case 3:
    case GAIA_ROCK_INSIDE_FIRST_ALTAR_FROM_GAIA_ROCK_INSIDE_MAZE:
      GaiaRockInside_KeepOverheadPart(OVERHEAD_PART_D);
      break;
    case 6:
    case GAIA_ROCK_INSIDE_FIRST_ALTAR_FROM_GAIA_ROCK_INSIDE_PLATFORMS_WEST:
      GaiaRockInside_KeepOverheadPart(OVERHEAD_PART_B);
      break;
    }
  } else if (gMainRoutineWork.next == SCENE_GAIA_ROCK_INSIDE_ALTAR) {
    switch (gMainRoutineWork.entrance) {
    case 1:
    case 3:
    case 5:
      GaiaRockInside_KeepOverheadPart(OVERHEAD_PART_A);
      break;
    case 2:
    case GAIA_ROCK_INSIDE_ALTAR_FROM_GAIA_ROCK_INSIDE_MAZE_A:
    case GAIA_ROCK_INSIDE_ALTAR_FROM_GAIA_ROCK_INSIDE_TWIN_ALTARS_NORTHWEST:
      GaiaRockInside_KeepOverheadPart(OVERHEAD_PART_B);
      break;
    case GAIA_ROCK_INSIDE_ALTAR_FROM_GAIA_ROCK_INSIDE_TWIN_ALTARS_SOUTHEAST_A_A:
      GaiaRockInside_KeepOverheadPart(OVERHEAD_PART_C);
      break;
    }
  }
  return 0;
}

/// The maze grid's width, in its cells.
#define GRID_WIDTH 28

/// The grid the maze's path is laid on (GaiaRockInside_LayMaze).
typedef struct {
  PathCell cells[GRID_WIDTH * GRID_WIDTH];
} PathGrid;

/// Takes size bytes of EWRAM's heap (Heap_AllocEwram).
static inline void *BumpAllocEwram(s32 size) { return Heap_AllocEwram(size); }

#define AT(x, y) grid->cells[(y) * GRID_WIDTH + (x)]
#define AT_ADDRESS(x, y)                                                       \
  ((PathCell *)(((y) * GRID_WIDTH + (x)) * 4 + (s32)grid))

/// The headings the walk tries from a step before it starts the path again.
#define MAZE_TRIES_PER_STEP 16

/// Lays the maze's path of MAZE_STEPS steps from seed, once a visit, on a
/// random walk over a 28 by 28 grid, and records each step's ways out.
void GaiaRockInside_LayMaze(s32 seed) {
  PathGrid *grid;
  Path *path;
  PathStep *steps;
  PathCell *cell;
  s32 x, y, nx, ny, step, tries, dir, v, next;
  u32 saved;

  saved = gRandomSeed;
  path = gPath;
  steps = path->steps;
  if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION))
    return;
  if (GameFlag_Get(FLAG_MAZE_LAID))
    return;
  gRandomSeed = seed;
restart:
  grid = BumpAllocEwram(sizeof *grid);
  Iwram_ClearWordsEntry(grid, sizeof *grid);
  // BUG: this runs 29 times, so row 28 and column 28 lie past the 28 by 28
  // cells and the last row's marks land after the block.
  {
    PathCell *a, *b, *e;
    s32 o, t;
    x = GRID_WIDTH / 2;
    y = GRID_WIDTH / 2;
    o = 0;
    b = &AT(GRID_WIDTH, 0);
    // step counts the border's rows here, before the walk numbers its steps.
    step = GRID_WIDTH;
    a = grid->cells;
    e = grid->cells;
    do {
      a->step = MAZE_BORDER;
      a += GRID_WIDTH;
      b->step = MAZE_BORDER;
      b += GRID_WIDTH;
      e->step = MAZE_BORDER;
      e++;
      t = o + GRID_WIDTH * GRID_WIDTH * 4;
      ((s8 *)grid)[t] = MAZE_BORDER;
      o += sizeof(PathCell);
      step--;
    } while (step >= 0);
  }
  for (step = 1; step <= MAZE_STEPS;) {
    tries = 0;
    for (;;) {
      if (++tries > MAZE_TRIES_PER_STEP)
        goto restart;
      nx = x;
      ny = y;
      dir = Random_Below(4);
      if (dir == MAZE_SOUTH && (step == 1 || step == MAZE_STEPS - 1))
        continue;
      switch (dir) {
      case MAZE_SOUTH:
        if (AT(x, y + 1).step != 0 || AT(x, y + 2).step != 0 ||
            AT(x - 1, y + 1).step != 0 || AT(x + 1, y + 1).step != 0)
          continue;
        ny = y + 1;
        break;
      case MAZE_WEST:
        if (AT(x - 1, y).step != 0 || AT(x - 2, y).step != 0 ||
            AT(x - 1, y - 1).step != 0 || AT(x - 1, y + 1).step != 0)
          continue;
        nx = x - 1;
        break;
      case MAZE_EAST:
        if (AT(x + 1, y).step != 0 || AT(x + 2, y).step != 0 ||
            AT(x + 1, y - 1).step != 0 || AT(x + 1, y + 1).step != 0)
          continue;
        nx = x + 1;
        break;
      case MAZE_NORTH:
        if (AT(x, y - 1).step != 0 || AT(x, y - 2).step != 0 ||
            AT(x - 1, y - 1).step != 0 || AT(x + 1, y - 1).step != 0)
          continue;
        ny = y - 1;
        break;
      }
      break;
    }
    cell = AT_ADDRESS(x, y);
    cell->step = step;
    step++;
    cell->nextX = nx;
    cell->nextY = ny;
    x = nx;
    y = ny;
  }

  x = GRID_WIDTH / 2;
  y = GRID_WIDTH / 2;
  for (step = 1; step <= MAZE_STEPS; step = next) {
    cell = AT_ADDRESS(x, y);
    next = step + 1;

    {
      s32 c;

      v = step;
      c = AT(x, y + 1).step;

      if (c == next)
        v = next;
      else if (c == step - 1)
        v = c;
      else {
        PathCell *p = cell;
        do {
          p -= GRID_WIDTH;
          c = p->step;
          if (c != v - 1) {
            break;
          }
          v = c;
        } while (1);
      }
      if (v == 0)
        v = 1;
    }
    steps[step].next[MAZE_SOUTH] = v;

    {
      s32 c;

      v = step;
      c = AT(x, y - 1).step;

      if (c == next)
        v = next;
      else if (c == step - 1)
        v = c;
      else {
        PathCell *p = AT_ADDRESS(x, y);
        do {
          p += GRID_WIDTH;
          c = p->step;
          if (c != v - 1) {
            break;
          }
          v = c;
        } while (1);
      }
      if (v == 0)
        v = 1;
    }
    steps[step].next[MAZE_NORTH] = v;

    {
      s32 c;

      v = step;
      c = AT(x - 1, y).step;

      if (c == next)
        v = next;
      else if (c == step - 1)
        v = c;
      else {
        nx = x;
        do {
          c = AT(++nx, y).step;
          if (c != v - 1) {
            break;
          }
          v = c;
        } while (1);
      }
      if (v == 0)
        v = 1;
    }
    steps[step].next[MAZE_WEST] = v;

    {
      s32 c;

      v = step;
      c = AT(x + 1, y).step;

      if (c == next)
        v = next;
      else if (c == step - 1)
        v = c;
      else {
        nx = x;
        do {
          c = AT(--nx, y).step;
          if (c != v - 1) {
            break;
          }
          v = c;
        } while (1);
      }
      if (v == 0)
        v = 1;
    }
    steps[step].next[MAZE_EAST] = v;
    x = cell->nextX;
    steps[step].number = step;
    y = cell->nextY;
  }
  steps[MAZE_STEPS].next[MAZE_NORTH] = MAZE_EXIT;
  steps[1].next[MAZE_SOUTH] = MAZE_BACK;
  path->current = 1;
  Heap_Release(grid);
  Event_SetFlag(FLAG_MAZE_LAID);
  gRandomSeed = saved;
}

/// Felix walks out of a maze room by one of its four ways, and a room just
/// like it appears. The way the plant's arrow points leads on to the next
/// room, and the way the party came in leads back to the last. Going straight
/// on where the path turns leads back along it, to the room where it last
/// turned, and the fourth way leads back into the same room. South out of the
/// first room leads back to the first altar's room, and the way on from the
/// fourteenth into the Serpent's lair.
void GaiaRockInside_TakeMazeExit(s32 direction) {
  s32 *state = &gPath->current;
  u8 *row = (u8 *)state + *state * 8;
  s32 next = row[((direction - 1) & 3) + 6];

  switch (next) {
  case MAZE_BACK:
  case MAZE_EXIT:
    FieldEvent_SetExitRequest(next);
    break;
  case 0:
    next = 1;
  default:
    *state = next;
    FieldEvent_SetExitRequest(direction);
    break;
  }
  Event_PlaySound(SE_MAP_EXIT);
}

extern s8 gGaiaRockInside_MazeOrigins[][2];

/// Draws the maze room the party stands in, a 15 by 10 cell screen copied from
/// the room's own place in the map (gGaiaRockInside_MazeOrigins), on both
/// layers; the first room is drawn where it stands.
void GaiaRockInside_DrawMazeRoom(void) {
  s32 state = gPath->current;
  s8 *origin = gGaiaRockInside_MazeOrigins[state];
  struct MapWork *map = Ram_HeapSlots->mapGrid;

  if (state > 1) {
    s32 originX = origin[0];
    s32 originZ = origin[1];
    struct MapLayer *layer = map->layers;
    s32 i;

    for (i = 0; i < 2; i++, layer++) {
      s32 x = FIELD_CELL(layer->offsetX);
      s32 z = FIELD_CELL(layer->offsetY);

      MapGrid_CopyRawCells(originX + x, originZ + z, 15, 10, x + 1, z + 1);
    }
  }
}

/// The way on from the step the party stands on: the facing toward the next
/// step or the exit, or -1 if no way leads on.
s32 GaiaRockInside_GetMazeWayOn(void) {
  Path *path = gPath;
  s32 step = path->current;
  PathStep *node = &path->steps[step];
  u32 link;

  // A way leads on when it reaches the exit or the step after this one.
  step++;
  link = node->next[MAZE_SOUTH];
  if (link == MAZE_EXIT || link == step)
    return FACING_SOUTH;
  link = node->next[MAZE_WEST];
  if (link == MAZE_EXIT || link == step)
    return FACING_WEST;
  link = node->next[MAZE_EAST];
  if (link == MAZE_EXIT || link == step)
    return FACING_EAST;
  link = node->next[MAZE_NORTH];
  if (link == MAZE_EXIT || link == step)
    return FACING_NORTH;
  return -1;
}

/// Fills every cell of a map layer within the map's bounds with its first
/// cell, but those of collision code 0 or keep.
void GaiaRockInside_BlankLayer(s32 layer, s32 keep) {
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  struct MapLayer *mapLayer = &map->layers[layer];
  u32 *cells = (u32 *)mapLayer->cells;
  s32 width = FIELD_CELL(map->maxX - map->minX);
  s32 height = FIELD_CELL(map->maxZ - map->minZ);
  u32 fill;
  u16 z;
  u16 x;

  cells += (FIELD_CELL(map->minZ) << 7) + FIELD_CELL(map->minX);
  fill = *cells;
  for (z = 0; z < height; z++) {
    u32 *cell = &cells[z * 128];

    for (x = 0; x < width; x++) {
      if (((struct MapCell *)cell)->collisionCode != 0 &&
          ((struct MapCell *)cell)->collisionCode != keep)
        *cell = fill;
      cell++;
    }
  }
}

#include "shared/gaia_rock_trigger_wall_cell.c"

#include "shared/stone_head_place.h"

extern s32 gStoneHead_RiseScript[];
extern u32 gDebris_Script[];

#include "shared/stone_head_burst_out.h"

/// Starts GaiaRock_TriggerWallCell each frame.
void GaiaRockInside_StartWallTriggers(void) {
  Task_AddFrame((s32)GaiaRock_TriggerWallCell, TASK_PASS_FRAME);
}

#include "text/msg_ids.h"

extern u32 gGaiaRockInside_IdleScript[];
extern u32 gGaiaRockInside_StumpScript[];

// clang-format off
#include "shared/field_effect_duration_scripts.c"

u32 gGaiaRockInside_PourScript[] = {
    SC_WAIT(230),
    SC_SET(SW_HOOK, 0),
    SC_POSITION_ORIGIN,
    SC_WAIT(1),
    SC_RELEASE,
};

u32 gGaiaRockInside_BurstScript[] = {
    SC_WAIT(25),
    SC_SET(SW_HOOK, 0),
    SC_POSITION_ORIGIN,
    SC_WAIT(1),
    SC_RELEASE,
};

s32 gGaiaRockInside_AltarShakesScript[] = {
    SC_MOVE_BY(0, 0, 2),
    SC_SYNC,
    SC_MOVE_BY(0, 0, -4),
    SC_SYNC,
    SC_MOVE_BY(0, 0, 2),
    SC_SYNC,
    SC_WAIT(2),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 gGaiaRockInside_AltarDropsScript[] = {
    SC_MOVE_BY(0, 0, 4),
    SC_SYNC,
    SC_MOVE_BY(0, 0, -8),
    SC_SYNC,
    SC_MOVE_BY(0, 0, 4),
    SC_SYNC,
    SC_WAIT(2),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

u32 gGaiaRockInside_LeaderStepsBackScript[] = {
    SC_MOVE(744, 0, 432),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_END,
};

/// The twelve cells of the cobweb, as column and row; Cyclone clears the web
/// when it reaches one of them (GaiaRockInside_CycloneClearsCobweb).
struct CellPoint gGaiaRockInside_CobwebCells[] = {
    {{42, 30}}, {{44, 20}}, {{46, 21}}, {{47, 23}}, {{47, 24}}, {{46, 26}},
    {{44, 27}}, {{43, 27}}, {{41, 26}}, {{40, 24}}, {{40, 23}}, {{41, 21}},
};

u32 gOrbitSpark_Script[] = {
    SC_WAIT(6),
    SC_RELEASE,
    SC_END,
};

u32 gGaiaRockInside_RisingSparkScript[] = {
    SC_MOVE_BY(0, 160, 0),
    SC_SYNC,
    SC_RELEASE,
    SC_END,
};

s32 gGaiaRockInside_CirclingSparkScript[] = {
    SC_CALL(Orbiter_Spiral),
    SC_RELEASE,
    SC_END,
};

#include "shared/lesson_seeking_spark_script.c"

#include "shared/lesson_scripts.c"

/// The cells the two stumps may be moved onto, numbered from 1; row 0 is none.
s16 gGaiaRockInside_StumpSpots[][2] = {{0, 0}, {113, 47}, {115, 47}, {115, 45}, {117, 45}, {119, 45}, {119, 47}, {121, 47}, {117, 49}, {119, 49}, {119, 51}, {121, 51}};

/// The swaying platforms' places, in cells.
s16 gGaiaRockInside_PlatformPlaces[][2] = {{45, 51}, {31, 53}, {31, 57}, {39, 59}, {41, 59}, {43, 63}, {45, 63}, {35, 69}, {37, 69}};

// The same script as gFieldEffect_MediumScript, under another name.
u32 gGaiaRockInside_SerpentGlowScript[] = {
    SC_WAIT(1),
    SC_LOOP(44, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_RELEASE,
    SC_END,
};

// The same script as gFieldEffect_MediumScript, under another name.
u32 gGaiaRockInside_SerpentSmokeScript[] = {
    SC_WAIT(1),
    SC_LOOP(44, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_RELEASE,
    SC_END,
};

u32 gGaiaRockInside_KushinadaScript[] = {
    SC_WAIT(1),
    SC_END,
};

/// The lair's blend, copied over the map's own blend script: the overhead
/// layer blended over the rest at 4, 6, 8 and 6 sixteenths, 30 frames each,
/// over and over.
u16 gGaiaRockInside_LairBlendScript[12] ALIGNED(4) = {
    BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL,
    BLEND_ALPHA(4, 16),
    30,
    BLEND_ALPHA(6, 16),
    30,
    BLEND_ALPHA(8, 16),
    30,
    BLEND_ALPHA(6, 16),
    30,
    BLEND_SCRIPT_JUMP | 0,
    BLEND_SCRIPT_LOOP,
};

/// Each maze room's place in the map, in cells.
s8 gGaiaRockInside_MazeOrigins[][2] = {{0, 0}, {1, 1}, {17, 1}, {33, 1}, {49, 1}, {49, 12}, {49, 23}, {49, 34}, {49, 45}, {33, 45}, {17, 45}, {1, 45}, {1, 34}, {1, 23}, {1, 12}, {0, 0}};

const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 24, 0, 408, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

/// A region below the floor of the altar room's cobweb pit, which leads down
/// to the twin altars' room.
struct SceneRegion gGaiaRockInside_AltarExitRegions[] = {
    {700, -30, 330, 720, -10, 350, CONDITION_ALWAYS,
     GAIA_ROCK_INSIDE_ALTAR_TO_GAIA_ROCK_INSIDE_TWIN_ALTARS_SOUTHEAST_A_B},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(GaiaRockInsideMaze),
    SCENE_EXIT(1, GaiaRockInsideMaze, 4),
    SCENE_EXIT(2, GaiaRockInsideMaze, 3),
    SCENE_EXIT(3, GaiaRockInsideMaze, 2),
    SCENE_EXIT(4, GaiaRockInsideMaze, GAIA_ROCK_INSIDE_MAZE_FROM_GAIA_ROCK_INSIDE_FIRST_ALTAR_A),
    SCENE_EXIT(GAIA_ROCK_INSIDE_MAZE_TO_GAIA_ROCK_INSIDE_FIRST_ALTAR_B, GaiaRockInsideFirstAltar, GAIA_ROCK_INSIDE_FIRST_ALTAR_FROM_GAIA_ROCK_INSIDE_MAZE),
    SCENE_EXIT(GAIA_ROCK_INSIDE_MAZE_TO_GAIA_ROCK_INSIDE_ALTAR, GaiaRockInsideAltar, GAIA_ROCK_INSIDE_ALTAR_FROM_GAIA_ROCK_INSIDE_MAZE_A),
    SCENE_EXIT(GAIA_ROCK_INSIDE_MAZE_TO_GAIA_ROCK_INSIDE_TWIN_ALTARS_B, GaiaRockInsideTwinAltars, GAIA_ROCK_INSIDE_TWIN_ALTARS_FROM_GAIA_ROCK_INSIDE_ALTAR_SOUTHEAST_A),
    SCENE_EXIT(GAIA_ROCK_INSIDE_MAZE_TO_GAIA_ROCK_INSIDE_TWIN_ALTARS_C, GaiaRockInsideTwinAltars, GAIA_ROCK_INSIDE_TWIN_ALTARS_FROM_GAIA_ROCK_INSIDE_ALTAR_SOUTHEAST_A),
    SCENE_EXIT(GAIA_ROCK_INSIDE_MAZE_TO_GAIA_ROCK_INSIDE_TWIN_ALTARS_D, GaiaRockInsideTwinAltars, GAIA_ROCK_INSIDE_TWIN_ALTARS_FROM_GAIA_ROCK_INSIDE_ALTAR_SOUTHEAST_A),
    SCENE_EXIT(GAIA_ROCK_INSIDE_MAZE_TO_GAIA_ROCK_INSIDE_FIRST_ALTAR_C, GaiaRockInsideFirstAltar, GAIA_ROCK_INSIDE_FIRST_ALTAR_FROM_GAIA_ROCK_INSIDE_MAZE),
    SCENE_EXIT(251, GaiaRockInsideMaze, GAIA_ROCK_INSIDE_MAZE_FROM_GAIA_ROCK_INSIDE_FIRST_ALTAR_B),
    SCENE_EXITS(GaiaRockInsideFirstAltar),
    SCENE_EXIT(GAIA_ROCK_INSIDE_FIRST_ALTAR_TO_GAIA_ROCK_SUSA, GaiaRockSusa, GAIA_ROCK_SUSA_FROM_GAIA_ROCK_INSIDE_FIRST_ALTAR),
    SCENE_EXIT(2, GaiaRockInsideFirstAltar, 3),
    SCENE_EXIT(3, GaiaRockInsideFirstAltar, 2),
    SCENE_EXIT(GAIA_ROCK_INSIDE_FIRST_ALTAR_TO_GAIA_ROCK_INSIDE_MAZE, GaiaRockInsideMaze, GAIA_ROCK_INSIDE_MAZE_FROM_GAIA_ROCK_INSIDE_FIRST_ALTAR_A),
    SCENE_EXIT(5, GaiaRockInsideFirstAltar, 6),
    SCENE_EXIT(6, GaiaRockInsideFirstAltar, 5),
    SCENE_EXIT(GAIA_ROCK_INSIDE_FIRST_ALTAR_TO_GAIA_ROCK_INSIDE_PLATFORMS_WEST, GaiaRockInsidePlatforms, GAIA_ROCK_INSIDE_PLATFORMS_FROM_GAIA_ROCK_INSIDE_FIRST_ALTAR_EAST_A),
    SCENE_EXIT(8, GaiaRockInsideFirstAltar, 9),
    SCENE_EXIT(9, GaiaRockInsideFirstAltar, 8),
    SCENE_EXIT(GAIA_ROCK_INSIDE_FIRST_ALTAR_TO_GAIA_ROCK_INSIDE_PLATFORMS_EAST, GaiaRockInsidePlatforms, GAIA_ROCK_INSIDE_PLATFORMS_FROM_GAIA_ROCK_INSIDE_FIRST_ALTAR_EAST_B),
    SCENE_EXITS(GaiaRockInsideTwinAltars),
    SCENE_EXIT(GAIA_ROCK_INSIDE_TWIN_ALTARS_TO_GAIA_ROCK_INSIDE_PLATFORMS, GaiaRockInsidePlatforms, GAIA_ROCK_INSIDE_PLATFORMS_FROM_GAIA_ROCK_INSIDE_TWIN_ALTARS),
    SCENE_EXIT(GAIA_ROCK_INSIDE_TWIN_ALTARS_TO_GAIA_ROCK_INSIDE_ALTAR_NORTHWEST, GaiaRockInsideAltar, GAIA_ROCK_INSIDE_ALTAR_FROM_GAIA_ROCK_INSIDE_TWIN_ALTARS_NORTHWEST),
    SCENE_EXIT(GAIA_ROCK_INSIDE_TWIN_ALTARS_TO_GAIA_ROCK_INSIDE_ALTAR_SOUTHEAST_A, GaiaRockInsideAltar, GAIA_ROCK_INSIDE_ALTAR_FROM_GAIA_ROCK_INSIDE_TWIN_ALTARS_SOUTHEAST_A_A),
    SCENE_EXIT(GAIA_ROCK_INSIDE_TWIN_ALTARS_TO_GAIA_ROCK_INSIDE_MAZE, GaiaRockInsideMaze, GAIA_ROCK_INSIDE_MAZE_FROM_GAIA_ROCK_INSIDE_TWIN_ALTARS_A),
    SCENE_EXITS(GaiaRockInsidePlatforms),
    SCENE_EXIT(GAIA_ROCK_INSIDE_PLATFORMS_TO_GAIA_ROCK_INSIDE_FIRST_ALTAR_EAST_A, GaiaRockInsideFirstAltar, GAIA_ROCK_INSIDE_FIRST_ALTAR_FROM_GAIA_ROCK_INSIDE_PLATFORMS_EAST),
    SCENE_EXIT(GAIA_ROCK_INSIDE_PLATFORMS_TO_GAIA_ROCK_INSIDE_FIRST_ALTAR_EAST_B, GaiaRockInsideFirstAltar, GAIA_ROCK_INSIDE_FIRST_ALTAR_FROM_GAIA_ROCK_INSIDE_PLATFORMS_WEST),
    SCENE_EXIT(GAIA_ROCK_INSIDE_PLATFORMS_TO_GAIA_ROCK_INSIDE_TWIN_ALTARS, GaiaRockInsideTwinAltars, GAIA_ROCK_INSIDE_TWIN_ALTARS_FROM_GAIA_ROCK_INSIDE_PLATFORMS),
    SCENE_EXITS(GaiaRockInsideAltar),
    SCENE_EXIT(1, GaiaRockInsideAltar, 2),
    SCENE_EXIT(2, GaiaRockInsideAltar, 1),
    SCENE_EXIT(3, GaiaRockInsideAltar, GAIA_ROCK_INSIDE_ALTAR_FROM_GAIA_ROCK_INSIDE_MAZE_A),
    SCENE_EXIT(4, GaiaRockInsideAltar, 3),
    SCENE_EXIT(5, GaiaRockInsideAltar, GAIA_ROCK_INSIDE_ALTAR_FROM_GAIA_ROCK_INSIDE_TWIN_ALTARS_SOUTHEAST_A_A),
    SCENE_EXIT(6, GaiaRockInsideAltar, 5),
    SCENE_EXIT(GAIA_ROCK_INSIDE_ALTAR_TO_GAIA_ROCK_INSIDE_TWIN_ALTARS_NORTHWEST, GaiaRockInsideTwinAltars, GAIA_ROCK_INSIDE_TWIN_ALTARS_FROM_GAIA_ROCK_INSIDE_ALTAR_NORTHWEST),
    SCENE_EXIT(GAIA_ROCK_INSIDE_ALTAR_TO_GAIA_ROCK_INSIDE_TWIN_ALTARS_SOUTHEAST_A_B, GaiaRockInsideTwinAltars, GAIA_ROCK_INSIDE_TWIN_ALTARS_FROM_GAIA_ROCK_INSIDE_ALTAR_SOUTHEAST_A),
    SCENE_EXIT(GAIA_ROCK_INSIDE_ALTAR_TO_GAIA_ROCK_INSIDE_TWIN_ALTARS_SOUTHEAST_B, GaiaRockInsideTwinAltars, GAIA_ROCK_INSIDE_TWIN_ALTARS_FROM_GAIA_ROCK_INSIDE_ALTAR_SOUTHEAST_B),
    SCENE_EXIT(GAIA_ROCK_INSIDE_ALTAR_TO_GAIA_ROCK_INSIDE_MAZE_B, GaiaRockInsideMaze, GAIA_ROCK_INSIDE_MAZE_FROM_GAIA_ROCK_INSIDE_TWIN_ALTARS_A),
    SCENE_EXITS_END,
};

u32 gGaiaRockInside_IdleScript[] = {
    SC_SET(SW_MOTION, 0),
    SC_SET(SW_Y, 0),
    SC_END,
};

s32 gGaiaRockInside_CaskSetDownScript[] = {
    SC_SPEED(1),
    SC_SET(SW_COLLISION, 129),
    SC_SET(SW_MOTION, 0),
    // Work value 2 is the facing, as SW_FACING is.
    SC_SET(2, FACING_SOUTH_SOUTHEAST),
    SC_POSITION(456, -32, 456),
    SC_SET(SW_GROUND, -toFIXED(32)),
    SC_CALL(GaiaRockInside_CaskRests),
    SC_END,
};

u32 gGaiaRockInside_CaskPushedScript[] = {
    SC_SPEED(0.25),
    SC_MOVE_BY(16, 0, 0),
    SC_SYNC,
    SC_END,
};

s32 gGaiaRockInside_CaskThrownScript[] = {
    SC_SPEED(0.5),
    SC_SET(SW_COLLISION, 129),
    SC_SET(SW_MOTION, 3),
    SC_MOVE(472, -32, 344),
    SC_SYNC,
    SC_SET(SW_COLLISION, 0),
    SC_CALL(GaiaRockInside_CaskRests),
    SC_END,
};

s32 gGaiaRockInside_CaskEmptiesScript[] = {
    SC_SPEED(1),
    SC_SET(SW_MOTION, 0),
    SC_SET(SW_COLLISION, 0),
    SC_MOVE_BY(4, 8, 0),
    SC_SYNC,
    SC_LABEL(1),
    SC_CALL(GaiaRockInside_CaskTips),
    SC_WAIT(2),
    SC_LOOP(8, SC_TO(1)),
    SC_SE_IN_VIEW(197),
    SC_CALL(GaiaRockInside_CaskPours),
    SC_WAIT(240),
    SC_LABEL(2),
    SC_CALL(GaiaRockInside_CaskRights),
    SC_WAIT(1),
    SC_LOOP(8, SC_TO(2)),
    SC_MOVE_BY(-4, -8, 0),
    SC_SYNC,
    SC_SET(SW_MOTION, 3),
    SC_END,
};

s32 gGaiaRockInside_CaskTossedLeftScript[] = {
    SC_SPEED(2),
    SC_MOVE(424, 0, 344),
    SC_SET(SW_VELOCITY_Y, toFIXED(8)),
    SC_SYNC,
    SC_CALL(GaiaRockInside_CaskBursts),
    SC_WAIT(2),
    SC_POSITION(0, 0, 0),
    SC_LABEL(1),
    SC_WAIT(1),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
};

s32 gGaiaRockInside_CaskTossedRightScript[] = {
    SC_SPEED(2),
    SC_MOVE(512, 0, 384),
    SC_SET(SW_VELOCITY_Y, toFIXED(8)),
    SC_SYNC,
    SC_CALL(GaiaRockInside_CaskBursts),
    SC_WAIT(2),
    SC_POSITION(0, 0, 0),
    SC_LABEL(1),
    SC_WAIT(1),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
};

s32 gGaiaRockInside_SusaFeedsSerpentScript[] = {
    SC_SPEED(1),
    SC_SET(SW_HOOK, (s32)GaiaRockInside_CheckBlocked),
    SC_SET(SW_COLLISION, 129),
    SC_LABEL(1),
    SC_SET(SW_FACING, FACING_WEST),
    SC_CALL(GaiaRockInside_SetDownCask),
    SC_WAIT(15),
    SC_SPEED(0.25),
    SC_SET(SW_STEERING, 0),
    SC_ANIMATION(9),
    SC_CALL(GaiaRockInside_SusaPushesCask),
    SC_MOVE_BY(16, 0, 0),
    SC_SYNC,
    SC_SPEED(1),
    SC_SET(SW_STEERING, 1),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_ANIMATION(2),
    SC_MOVE_BY(0, 0, 16),
    SC_SYNC,
    SC_SET(SW_FACING, FACING_WEST),
    SC_MOVE_BY(-16, 0, 0),
    SC_SYNC,
    SC_SET(SW_FACING, FACING_NORTH),
    SC_SPEED(0.5),
    SC_CALL(GaiaRockInside_SusaThrowsCask),
    SC_ANIMATION(7),
    SC_WAIT(15),
    SC_MOVE(472, 0, 360),
    SC_SYNC,
    SC_WAIT(10),
    SC_ANIMATION(0),
    SC_CALL(GaiaRockInside_CaskEmpties),
    SC_WAIT(20),
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(260),
    SC_SET(SW_FACING, FACING_NORTH),
    SC_WAIT(20),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(10),
    SC_CALL(GaiaRockInside_TossCask),
    SC_WAIT(10),
    SC_SPEED(1),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_ANIMATION(2),
    SC_MOVE(472, 0, 456),
    SC_SYNC,
    SC_SET(SW_FACING, FACING_WEST),
    SC_ANIMATION(0),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
};

struct ActorPlacement gGaiaRockInside_Placements[] = {
    ACTOR_PLACEMENT(SPRITE_ARROW_PLANT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136, 0, 104, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gGaiaRockInside_LairPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_SERPENT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 328, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BARREL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SUSA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 456, 0, 456, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SLATE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

u32 gGaiaRockInside_StumpScript[] = {
    SC_SET(SW_SCALE_X, FIXED_ONE * 3 / 4),
    SC_SET(SW_SCALE_Y, FIXED_ONE * 3 / 4),
    SC_WAIT(1),
    SC_END,
};

struct ActorPlacement gGaiaRockInside_FirstAltarActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_DEMON_MASK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 408, 0, 688, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_DEMON_MASK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 584, 0, 688, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STUMP, CONDITION_ALWAYS, (s32)gGaiaRockInside_StumpScript, 920, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STUMP, CONDITION_ALWAYS, (s32)gGaiaRockInside_StumpScript, 952, 0, 408, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ALTAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 496, 0, 728, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_DOLL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gGaiaRockInside_TwinAltarsActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_DEMON_MASK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 456, 0, 352, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_DEMON_MASK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 584, 0, 352, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ALTAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 528, 0, 384, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ALTAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 672, 0, 384, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_DOLL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gGaiaRockInside_PlatformsActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_LEAFY_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LEAFY_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LEAFY_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LEAFY_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LEAFY_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LEAFY_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LEAFY_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LEAFY_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LEAFY_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gGaiaRockInside_AltarActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_DEMON_MASK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 400, 0, 368, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ALTAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 336, 0, 408, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPROUTING_LEAVES, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 680, 0, 648, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_HEAD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_HEAD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_HEAD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_DOLL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ALTAR, CONDITION_ALWAYS, (s32)gGaiaRockInside_IdleScript, 704, 0, 384, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_GANGWAY, CONDITION_ALWAYS, (s32)gGaiaRockInside_IdleScript, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

s32 gGaiaRockInside_SquashYScript[] = {
    SC_LABEL(1),
    SC_ADD(SW_SCALE_Y, FIXED_ONE * 3 / 32),
    SC_WAIT(1),
    SC_LOOP(4, SC_TO(1)),
    SC_LABEL(2),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE * 3 / 32),
    SC_WAIT(1),
    SC_LOOP(4, SC_TO(2)),
    SC_LABEL(3),
    SC_ADD(SW_SCALE_Y, FIXED_ONE / 16),
    SC_WAIT(1),
    SC_LOOP(2, SC_TO(3)),
    SC_LABEL(4),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 16),
    SC_WAIT(1),
    SC_LOOP(2, SC_TO(4)),
    SC_ADD(SW_SCALE_Y, FIXED_ONE / 16),
    SC_WAIT(1),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 16),
    SC_WAIT(1),
    SC_END,
};

s32 gGaiaRockInside_SquashXScript[] = {
    SC_LABEL(1),
    SC_ADD(SW_SCALE_X, FIXED_ONE * 3 / 32),
    SC_WAIT(1),
    SC_LOOP(4, SC_TO(1)),
    SC_LABEL(2),
    SC_ADD(SW_SCALE_X, -FIXED_ONE * 3 / 32),
    SC_WAIT(1),
    SC_LOOP(4, SC_TO(2)),
    SC_LABEL(3),
    SC_ADD(SW_SCALE_X, FIXED_ONE / 16),
    SC_WAIT(1),
    SC_LOOP(2, SC_TO(3)),
    SC_LABEL(4),
    SC_ADD(SW_SCALE_X, -FIXED_ONE / 16),
    SC_WAIT(1),
    SC_LOOP(2, SC_TO(4)),
    SC_ADD(SW_SCALE_X, FIXED_ONE / 16),
    SC_WAIT(1),
    SC_ADD(SW_SCALE_X, -FIXED_ONE / 16),
    SC_WAIT(1),
    SC_END,
};

/// The frames the arrow plants flash and pulse by.
s32 gGaiaRockInside_ArrowFrame = 0;

struct Trigger gGaiaRockInside_Triggers[] = {
    {TRIGGER_STEP, CELL_BEHAVIOR_MAZE_SOUTH, CONDITION_ALWAYS, EVENT(GaiaRockInside_TakeMazeExit)},
    {TRIGGER_STEP, CELL_BEHAVIOR_MAZE_WEST, CONDITION_ALWAYS, EVENT(GaiaRockInside_TakeMazeExit)},
    {TRIGGER_STEP, CELL_BEHAVIOR_MAZE_EAST, CONDITION_ALWAYS, EVENT(GaiaRockInside_TakeMazeExit)},
    {TRIGGER_STEP, CELL_BEHAVIOR_MAZE_NORTH, CONDITION_ALWAYS, EVENT(GaiaRockInside_TakeMazeExit)},
    {PSYNERGY_ON_ACTOR(ABILITY_GROWTH), ACTOR_ARROW_PLANT, FLAG_PLANT_POINTING, EVENT(GaiaRockInside_PlantPointsTheWay)},
    {TRIGGER_TABLE_END},
};

struct Trigger gGaiaRockInside_LairTriggers[] = {
    {TRIGGER_RAISED, RAISED_SUSA_BLOCKED, CONDITION_ALWAYS, EVENT(GaiaRockInside_SusaSaysMoveAside)},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_STEP, CELL_BEHAVIOR_BY_SUSA, FLAG_MET_SUSA, EVENT(GaiaRockInside_MeetSusa)},
    {TRIGGER_TALK, ACTOR_SUSA, CONDITION_ALWAYS, EVENT(GaiaRockInside_TalkToSusa)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SUSA, CONDITION_ALWAYS, EVENT(GaiaRockInside_ReadSusasMind)},
    {TRIGGER_TALK, ACTOR_SAND_TABLET, CONDITION_ALWAYS, EVENT(GaiaRockInside_ReadSandTablet)},
    {EXAMINE_TRIGGER(EXAMINE_GROUND), CELL_BEHAVIOR_CUP, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgGaiaRockInsideTheCupsAreOverflowingWith))},
    {TRIGGER_EXAMINE, CELL_BEHAVIOR_SERPENT, CONDITION_ALWAYS, EVENT(GaiaRockInside_ChallengeSerpent)},
    {TRIGGER_EXAMINE, CELL_BEHAVIOR_SERPENT_WEST, CONDITION_ALWAYS, EVENT(GaiaRockInside_ChallengeSerpent)},
    {EXAMINE_TRIGGER(EXAMINE_GROUND), CELL_BEHAVIOR_CLOUD_BRAND, FLAG_CLOUD_BRAND_FOUND, EVENT(GaiaRockInside_FindCloudBrand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, EVENT(GaiaRockInside_SinkIntoSand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(GaiaRockInside_RiseFromSand)},
    {TRIGGER_TABLE_END},
};

struct Trigger gGaiaRockInside_SlainLairTriggers[] = {
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_STEP, CELL_BEHAVIOR_BEFORE_SERPENT, CONDITION_ALWAYS, EVENT(GaiaRockInside_RaiseSandTablet)},
    {TRIGGER_TALK, ACTOR_SAND_TABLET, CONDITION_ALWAYS, EVENT(GaiaRockInside_ReadSandTablet)},
    {EXAMINE_TRIGGER(EXAMINE_GROUND), CELL_BEHAVIOR_CUP, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgGaiaRockInsideTheCupsAreOverflowingWith))},
    {TRIGGER_EXAMINE, CELL_BEHAVIOR_SERPENT, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgGaiaRockInsideTheSerpentWithItsEyes))},
    {EXAMINE_TRIGGER(EXAMINE_GROUND), CELL_BEHAVIOR_CLOUD_BRAND, FLAG_CLOUD_BRAND_FOUND, EVENT(GaiaRockInside_FindCloudBrand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, EVENT(GaiaRockInside_SinkIntoSand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(GaiaRockInside_RiseFromSand)},
    {TRIGGER_TABLE_END},
};

struct Trigger gGaiaRockInside_FirstAltarTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_TALK, ACTOR_FIRST_ALTAR, CONDITION_ALWAYS, EVENT(GaiaRockInside_CheckAltar)},
    {USE_ITEM_ON_ACTOR(ITEM_DANCING_IDOL), TRIGGER_SUBJECT_OWN_EFFECT | ACTOR_FIRST_ALTAR, CONDITION_ALWAYS, EVENT(GaiaRockInside_PlaceDollOnFirstAltar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_STUMP_A, CONDITION_ALWAYS, EVENT(GaiaRockInside_RecordStumpSpots)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_STUMP_B, CONDITION_ALWAYS, EVENT(GaiaRockInside_RecordStumpSpots)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(GaiaRockInside_RecordStumpSpots)},
    {TRIGGER_TABLE_END},
};

struct Trigger gGaiaRockInside_TwinAltarsTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_TALK, ACTOR_WEST_ALTAR, CONDITION_ALWAYS, EVENT(GaiaRockInside_CheckAltar)},
    {USE_ITEM_ON_ACTOR(ITEM_DANCING_IDOL), TRIGGER_SUBJECT_OWN_EFFECT | ACTOR_WEST_ALTAR, CONDITION_ALWAYS, EVENT(GaiaRockInside_PlaceDollOnWestAltar)},
    {TRIGGER_TALK, ACTOR_EAST_ALTAR, CONDITION_ALWAYS, EVENT(GaiaRockInside_CheckAltar)},
    {USE_ITEM_ON_ACTOR(ITEM_DANCING_IDOL), TRIGGER_SUBJECT_OWN_EFFECT | ACTOR_EAST_ALTAR, CONDITION_ALWAYS, EVENT(GaiaRockInside_PlaceDollOnEastAltar)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), CELL_BEHAVIOR_SUNBEAM_BUSHES, CONDITION_ALWAYS, EVENT(GaiaRockInside_CycloneClearsTwinAltarsBushes)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), -1, 0, 0},
};

struct Trigger gGaiaRockInside_PlatformsTriggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), CELL_BEHAVIOR_PLATFORMS_BUSHES, CONDITION_ALWAYS, EVENT(GaiaRockInside_CycloneStartsPlatforms)},
    {TRIGGER_TABLE_END},
};

struct Trigger gGaiaRockInside_AltarTriggers[] = {
    {TRIGGER_EXIT | STEP_TRIGGER_FLAG_WALKING | STEP_TRIGGER_FLAG_QUICK | TRIGGER_FLAG_FACING | FACING_NORTH, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT | STEP_TRIGGER_FLAG_WALKING | STEP_TRIGGER_FLAG_QUICK | TRIGGER_FLAG_FACING | FACING_NORTH, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT | STEP_TRIGGER_FLAG_WALKING | STEP_TRIGGER_FLAG_QUICK | TRIGGER_FLAG_FACING | FACING_NORTH, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 7, CONDITION_ALWAYS, 7},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_BEHAVIOR_COBWEB_LEDGE, CONDITION_ALWAYS, EVENT(GaiaRockInside_JumpDownCobwebPit)},
    {TRIGGER_STEP, CELL_BEHAVIOR_WEST_STONE_HEAD, FLAG_STATUE_A_RISEN, EVENT(GaiaRockInside_ClimbOntoWestStoneHead)},
    {TRIGGER_STEP, CELL_BEHAVIOR_MIDDLE_STONE_HEAD, FLAG_STATUE_B_RISEN, EVENT(GaiaRockInside_ClimbOntoMiddleStoneHead)},
    {TRIGGER_STEP, CELL_BEHAVIOR_EAST_STONE_HEAD, FLAG_STATUE_C_RISEN, EVENT(GaiaRockInside_ClimbOntoEastStoneHead)},
    {TRIGGER_TALK, ACTOR_ALTAR, CONDITION_ALWAYS, EVENT(GaiaRockInside_CheckAltar)},
    {USE_ITEM_ON_ACTOR(ITEM_DANCING_IDOL), TRIGGER_SUBJECT_OWN_EFFECT | ACTOR_ALTAR, CONDITION_ALWAYS, EVENT(GaiaRockInside_PlaceDollOnAltarAltar)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_HANGING_VINE, CONDITION_ALWAYS, 0},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), CELL_BEHAVIOR_ALTAR_BUSHES, CONDITION_ALWAYS, EVENT(GaiaRockInside_CycloneClearsAltarBushes)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), CELL_BEHAVIOR_MAD_PLANT_BUSHES, CONDITION_ALWAYS, EVENT(GaiaRockInside_CycloneClearsMadPlantBushes)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), CELL_BEHAVIOR_COBWEB, CONDITION_ALWAYS, EVENT(GaiaRockInside_CycloneClearsCobweb)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), CELL_BEHAVIOR_COBWEB_LEDGE, CONDITION_ALWAYS, EVENT(GaiaRockInside_CycloneClearsCobweb)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), CELL_BEHAVIOR_MAD_PLANT, CONDITION_ALWAYS, EVENT(GaiaRockInside_CycloneMadPlant)},
    {TRIGGER_TABLE_END},
};

#include "shared/debris_script.c"

#include "shared/stone_head_rise_script.c"
// clang-format on
#undef EFFECT_SET_MODE
#undef EFFECT_SET_SCRIPT
#undef EFFECT_SPRITE_PRIORITY
#undef Test
#undef C1
#undef C2
#undef C3
#undef C4
#undef C6
#undef GRID_WIDTH
#undef gPath
#undef AT
#undef AT_ADDRESS
#undef GAIA_ROCK_INSIDE_PATH
#undef gSceneState
#undef GaiaRockInside_PulseUpDownArrow
#undef GaiaRockInside_PulseSidewaysArrow
