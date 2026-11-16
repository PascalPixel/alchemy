/// @file
/// Gaia Rock's inside, the rooms of this overlay near Izumo: the party places
/// the doll on the altar by the entrance and on the altars deeper in
/// (GaiaRockInside_PlaceDollOnFirstAltar to
/// GaiaRockInside_PlaceDollOnAltarAltar), each one weakening the Serpent, and
/// clears overgrowth and cobwebs with Cyclone. In the Serpent's lair Susa
/// fights to save Kushinada (GaiaRockInside_MeetSusa); examining the Serpent
/// starts the battle with it (GaiaRockInside_ChallengeSerpent). Felix reads
/// the rock's tablet and learns Sand (GaiaRockInside_ReadSandTablet).

#include "types.h"
#include "macros.h"
#include "item_ids.h"
#include "field/field_sequence.h"
#include "field/actor.h"
#include "field/event_inline.h"
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
#include "system/task.h"
#include "field/actor_script.h"
#include "../field/actor_set_sprite_priority.c"

#include "../field/actor_prepare.c"

#include "shared/scene_create_actor.h"

#include "../field/field_effect_move.c"

#define EFFECT_SPAWN FieldEffect_Spawn
#define EFFECT_CREATE Import_ActorCreate
#define EFFECT_SET_MODE Actor_SelectAnimation
#define EFFECT_SET_SCRIPT Actor_StartScript
#define EFFECT_SCRIPTS gFieldEffect_DurationScripts
#define EFFECT_SPRITE_PRIORITY
#include "../field/field_effect_spawn.c"

#undef EFFECT_SPAWN

/// The four streams of Dragonsbane that reach the Serpent's cups: a carved face
/// slides aside when the doll dances on its altar, and Cyclone clears the
/// bushes in the twin altars' room (GaiaRockInside_OpenHallPassages).
#define FLAG_ALTAR_ROOM_STREAM_OPEN 0x9bc
#define FLAG_WEST_ALTAR_STREAM_OPEN 0x9bd
#define FLAG_EAST_ALTAR_STREAM_OPEN 0x9be
#define FLAG_BUSHES_STREAM_OPEN 0x9bf
/// A stream opened before the Serpent is slain, shown pouring into the lair
/// (Scene_Initialize, entrance 20) before its flag above is set.
#define FLAG_ALTAR_ROOM_STREAM_TO_SHOW FLAG_AREA_TEMP_07
#define FLAG_WEST_ALTAR_STREAM_TO_SHOW FLAG_AREA_TEMP_08
#define FLAG_EAST_ALTAR_STREAM_TO_SHOW FLAG_AREA_TEMP_09
#define FLAG_BUSHES_STREAM_TO_SHOW FLAG_AREA_TEMP_0A
#define FLAG_SUSA_ATTACKED_SERPENT                                             \
  0x9ac ///< "At last... The serpent has finally had its fill."
#define FLAG_MET_SUSA 0x9de     ///< "Who are you? How did you get here?"
#define FLAG_LEARNED_SAND 0x9ba ///< Felix touched the slate and learned Sand
#define FLAG_SAND_TABLET_RAISED 0x9ee ///< the slate rose from the lair's floor
#define FLAG_ALTAR_FELL 0x9c4         ///< Cyclone tore the cobweb holding it
#define FLAG_FIRST_ALTAR_FACES_MOVED 0xa62
#define FLAG_PLATFORMS_SWAYING 0xa63
#define FLAG_CLOUD_BRAND_FOUND 0xe41
/// The statues that rise where the leader steps in the altar room.
#define FLAG_STATUE_A_RISEN FLAG_AREA_TEMP_03
#define FLAG_STATUE_B_RISEN FLAG_AREA_TEMP_04
#define FLAG_STATUE_C_RISEN FLAG_AREA_TEMP_05
/// The two rocks' places in gGaiaRockInside_RockSpots, a half byte each.
#define FLAG_ROCK_SPOTS FLAG_SCENE_TEMP_10

/// The maze's one actor, in the middle of each of its screens.
enum {
  /// A sprig that Growth turns into an arrow pointing the way on
  /// (GaiaRockInside_PlantPointsTheWay).
  ACTOR_ARROW_PLANT = 8
};

/// The trigger Susa's hook raises when the party has blocked him or his cask
/// (GaiaRockInside_CheckBlocked).
#define RAISED_SUSA_BLOCKED 42

/// The lair's entrances: its way in, and where the battle with the Serpent
/// returns the party: won, lost without the Dancing Idol, and lost holding it
/// before any stream of Dragonsbane is open (GaiaRockInside_ChallengeSerpent).
enum {
  ENTRANCE_LAIR = 9,
  ENTRANCE_SERPENT_BEATEN = 10,
  ENTRANCE_BEATEN_BY_SERPENT = 11,
  ENTRANCE_BEATEN_HOLDING_IDOL = 12
};

/// The Serpent's lair (SceneId_GaiaRockInsideMaze by entrances 9 to 20).
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

/// The room of the first altar (SceneId_GaiaRockInsideFirstAltar): two carved
/// faces whose cups pour Dragonsbane once the doll dances on the altar between
/// them, and the two rocks of the Move puzzle on its east side.
enum {
  ACTOR_FIRST_ALTAR_WEST_FACE = 8,
  ACTOR_FIRST_ALTAR_EAST_FACE = 9,
  ACTOR_ROCK_A = 10,
  ACTOR_ROCK_B = 11,
  ACTOR_FIRST_ALTAR = 12,
  /// The Dancing Idol, set on the altar while it dances.
  ACTOR_FIRST_ALTAR_DOLL = 13
};

/// The room of the twin altars (SceneId_GaiaRockInsideTwinAltars): a face over
/// each altar.
enum {
  ACTOR_WEST_FACE = 8,
  ACTOR_EAST_FACE = 9,
  ACTOR_WEST_ALTAR = 10,
  ACTOR_EAST_ALTAR = 11,
  ACTOR_TWIN_ALTARS_DOLL = 12
};

/// The room whose leafy platforms Cyclone sets swaying
/// (GaiaRockInside_StartPlatforms): its nine platforms.
enum { ACTOR_FIRST_PLATFORM = 8, ACTOR_LAST_PLATFORM = 16 };

/// The altar room under the cobweb (SceneId_GaiaRockInsideAltar).
enum {
  ACTOR_ALTAR_FACE = 8,
  ACTOR_ALTAR = 9,
  ACTOR_HANGING_VINE = 10,
  /// The three statues that rise where the leader steps
  /// (GaiaRockInside_StatueRisesUnderLeader).
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

/// One grid cell: the step that stands on it (0 for none, 99 for the border)
/// and the column and row of the next step.
typedef struct {
  s8 step;
  s8 nextX;
  s8 nextY;
} PathCell;

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

/// At gInventorySnapshot + 0x288: the step the party stands on, set to the
/// first once the path is laid, and the steps.
typedef struct {
  s32 current;
  PathStep steps[15];
} Path;

#define gPath ((Path *)(gInventorySnapshot + 0x288))

/// Set once the plant on a step of the maze has pointed the way on, for this
/// visit.
#define FLAG_MAZE_PLANT_POINTED(step) (0xa50 + (step))

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

void Cyclone_CopyTouchedCells(s32 x, s32 z, s32 layerIndex, s32 width,
                              s32 height, s32 toX, s32 toZ);
void MadPlant_LeapAtLeader(s32 a, s32 b, s32 c);

extern u32 gGaiaRockInside_LeaderStepsBackScript[];
extern s32 gGaiaRockInside_AltarShakesScript[];
extern s32 gGaiaRockInside_AltarDropsScript[];

struct Actor gGaiaRockInside_SavedSusa BSS_DATA;
s32 gGaiaRockInside_SavedSusaX BSS_DATA;
s32 gGaiaRockInside_SavedSusaZ BSS_DATA;
s32 gGaiaRockInside_SavedSusaFrame BSS_DATA;
s32 gGaiaRockInside_SavedSusaAnimation BSS_DATA;

extern s32 gGaiaRockInside_CaskThrownScript[];
extern s32 gGaiaRockInside_CaskEmptiesScript[];

void GaiaRockInside_SaveSusa(void);
void GaiaRockInside_RestoreSusa(void);

void Actor_Release(struct Actor *object);

s32 Scene_TalkToActor();

void GaiaRockInside_PlacePlatform(s32 id, s32 x, s32 z);

void GaiaRockInside_SetPlatformMode(u32 id, s8 mode);
void GaiaRockInside_SwayPlatforms(void);

void GaiaRockInside_SetMazeLayer(s32 layer, s32 keep);

extern u32 gGaiaRockInside_SavedCollision[];

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

void GaiaRockInside_JumpDownByAltar(void);

void GaiaRockInside_CycloneStartsPlatforms(void);

void GaiaRockInside_CycloneMadPlant(void);

void GaiaRockInside_ClearPlatformsSmallBushes(void);

void GaiaRockInside_CycloneClearsCobweb(void);

void GaiaRockInside_SusaSaysMoveAside(void);

void GaiaRockInside_MeetSusa(void);
void GaiaRockInside_TalkToSusa(void);
void GaiaRockInside_ReadSusasMind(void);

void GaiaRockInside_RaiseSandTablet(void);
void GaiaRockInside_ChallengeSerpent(void);

void Leader_SetLayerByte(void);

void GaiaRockInside_EndSand(void);
void GaiaRockInside_FindCloudBrand(void);
void GaiaRockInside_CheckAltar(void);
void GaiaRockInside_PlaceDollOnFirstAltar(void);
void GaiaRockInside_PlaceDollOnWestAltar(void);
void GaiaRockInside_PlaceDollOnEastAltar(void);
void GaiaRockInside_PlaceDollOnAltarAltar(void);

void GaiaRockInside_StepOnFirstStatueSpot(void);

void GaiaRockInside_StepOnSecondStatueSpot(void);
void GaiaRockInside_StepOnThirdStatueSpot(void);

extern u32 gFieldEffect_ShortScript[];
extern u32 gFieldEffect_MediumScript[];
extern u32 gFieldEffect_LongScript[];

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

extern u32 gGaiaRockInside_Regions[];

const u8 *Scene_GetExitRegions(void) {
  if (gMainRoutineWork.next == SCENE_GAIA_ROCK_INSIDE_ALTAR)
    return gGaiaRockInside_Regions;
  return 0;
}

/// Actor_JumpDownLedge, called through this inline as the ROM's call compiles.
static __inline__ void GaiaRockInside_JumpDownLedge(s32 id, s32 pose,
                                                    s32 param) {
  Actor_JumpDownLedge(id, pose, param);
}

/// Stepping off the ledge by the altar: the leader jumps down to the room
/// below.
void GaiaRockInside_JumpDownByAltar(void) {
  FieldActor_SetSpritePriority(gMainRoutineWork.leaderActorId, 3);
  OverlayObject_PrepareObject(toFIXED(712), 0, toFIXED(468), SPRITE_CRATE);
  OverlayObject_PrepareObject(toFIXED(712), 0, toFIXED(490), SPRITE_CRATE);
  GaiaRockInside_JumpDownLedge(gMainRoutineWork.leaderActorId, 6, 0x800);
  Event_PlaySound(SE_FALL);
  Event_SetFlag(FLAG_ARRIVE_FALLING);
  FieldEvent_SetExitRequest(8);
}

// Includes the shared FieldEffect_AdvanceMotion in this scene.
// Neither it nor its caller GaiaRockInside_RaiseDust exists in the Japanese
// scene.
#if TLA_PATCH_1
#include "../field/field_effect_advance_motion.c"
#endif

#if TLA_PATCH_1
/// Arriving in the lair by entrance 3 from above: the leader drops from 130
/// pixels up and lands in a ring of dust with a thump that shakes the room.
/// The Japanese edition has no form of this function here.
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
  options.palette = 7;
  options.update = FieldEffect_AdvanceMotion;
  options.startScaleX = FIXED_ONE * 16 / 25;
  options.startScaleY = FIXED_ONE * 16 / 25;
  for (i = 0; i <= 16; i++) {
    s32 angle;
    FIXED velocity[3];
    FIXED cosine;

    angle = i << 12;
    cosine = Trig_Cos(angle);
    velocity[0] = cosine;
    velocity[1] = 0;
    velocity[2] = Math_Sin(angle);
    velocity[0] += velocity[0] >> 1;
    velocity[0] += (Random_Below(0xc000)) - 0x6000;
    velocity[2] += (Random_Below(0x6000)) - 0x3000;
    FieldEffect_Spawn(actor->x.fixed, actor->y.fixed, actor->z.fixed,
                      velocity[0], velocity[1], velocity[2],
                      EFFECT_USE_UPDATE | EFFECT_USE_START_SCALE |
                          EFFECT_USE_PALETTE | 1,
                      &options);
  }
  Event_PlaySound(SE_THUMP);
  Actor_SetAttachedEffect(gMainRoutineWork.leaderActorId,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, 49);
  MapGrid_SetShake(5, 5, 1);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  FieldMapGrid_WaitForShakeToSettle();
  Actor_SetAttachedEffect(gMainRoutineWork.leaderActorId,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_REMOVE);
  actor->gravity = toFIXED(1);
  actor->bounce = FIXED_ONE / 4;
  FieldEvent_Wait(10);
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_STAND);
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  Event_End();
}
#endif

const u32 *Scene_GetExits(void) { return gSceneExits; }

/// Susa feeds the Serpent Dragonsbane cask by cask
/// (gGaiaRockInside_SusaWalksOffScript): he sets a cask down, pushes it,
/// throws it to the Serpent's mouth, where it tips and pours, and the empty
/// cask is tossed aside and bursts. The routines below are its steps.
///
/// The cask lands and holds still on its third animation.
s32 GaiaRockInside_CaskRests(void) {
  struct Actor *cask;

  cask = Actor_Get(ACTOR_CASK);
  Actor_SelectAnimation(cask, 3);
  Actor_SetAnimationSpeed(cask, 0);
  return 0;
}

/// The cask tips a step toward the Serpent's mouth.
s32 GaiaRockInside_CaskTips(void) {
  struct Actor *object;

  object = Actor_Get(ACTOR_CASK);
  ((struct FieldSprite *)object->sprite)->rotation += 0x300;
  return 0;
}

extern u32 gGaiaRockInside_PourScript[];

/// The cask rights itself a step.
s32 GaiaRockInside_CaskRights(void) {
  struct FieldSprite *sprite = Actor_Get(ACTOR_CASK)->sprite;

  sprite->rotation -= 0x300;
  return 0;
}

/// Each frame the pour's size varies at random, never below an eighth.
void GaiaRockInside_FlickerPour(struct Actor *object) {
  u32 random = Random16();
  s32 scale = object->scaleX;
  s32 bias;
  s32 limit;

  bias = -0x800;
  scale += FIXEDtoINT(random << 12);
  scale += bias;
  object->scaleX = scale;
  object->scaleY = object->scaleY + ((Random16() << 12) >> 16) + bias;
  limit = 240;
  scale = object->scaleX;
  limit <<= 4;
  limit += 255;
  if (scale <= limit)
    object->scaleX = FIXED_ONE / 8;
  if (object->scaleY <= limit)
    object->scaleY = FIXED_ONE / 8;
}

/// The cask pours: a spray of Dragonsbane that flickers in size
/// (GaiaRockInside_FlickerPour).
s32 GaiaRockInside_CaskPours(void) {
  struct Actor *source = Actor_Get(ACTOR_CASK);
  struct Actor *object;
  struct FieldSprite *sprite;

  object = (FieldActor_Create(14, source->x.fixed + toFIXED(8),
                              source->y.fixed - toFIXED(2), source->z.fixed));
  object->motionFlags = 0;
  Actor_SetSpriteFlags(object, 0);
  Actor_StartScript(object, gGaiaRockInside_PourScript);
  object->scaleX = FIXED_ONE * 2 / 5;
  object->scaleY = FIXED_ONE * 2 / 5;
  sprite = object->sprite;
  sprite->priority = 1;
  object->bounce = 0;
  object->riseBaseY = 0;
  object->update = (void (*)(void))GaiaRockInside_FlickerPour;
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

/// Susa throws the cask up to the Serpent's mouth.
s32 GaiaRockInside_SusaThrowsCask(void) {
  struct Actor *object;

  object = Actor_Get(ACTOR_CASK);
  Actor_StartScript(object, (s32)gGaiaRockInside_CaskThrownScript);
  object->x.fixed = toFIXED(472);
  Actor_SetAnimationSpeed(object, 16);
  Actor_SetAttachedEffect(ACTOR_SUSA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  return 0;
}

/// At the mouth the cask tips, pours and rights itself again.
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
  i = 3;
  do {
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
        dust->timer = ((Random16() << 9) >> 16) - 0x100;
        dust->speedLimit = 0;
        dust->acceleration = 0;
        dust->update = Effect_AdvanceMotionAtLeaderPriority;
      }
    }
  } while (--i >= 0);
  return 0;
}

/// The empty cask is tossed aside, away from where the leader stands.
s32 GaiaRockInside_TossCask(void) {
  struct Actor *object = Actor_Get(ACTOR_CASK);
  struct Actor *leader = Actor_Get(gMainRoutineWork.leaderActorId);

  if (leader->z.fixed >> 19 <= 45 && leader->x.fixed >> 19 <= 57)
    Actor_StartScript(object, gGaiaRockInside_CaskTossedRightScript);
  else
    Actor_StartScript(object, gGaiaRockInside_CaskTossedLeftScript);
  return 0;
}

/// Susa's hook while he works: when he or the cask has been stopped by the
/// party for 30 frames, the trigger GaiaRockInside_SusaSaysMoveAside is raised;
/// while one is blocked the other waits.
void GaiaRockInside_CheckBlocked(struct Actor *object) {
  struct Actor *other = Actor_Get(ACTOR_CASK);

  if (object->refused == 30 || other->refused == 30) {
    gFieldWork->raisedTrigger = RAISED_SUSA_BLOCKED;
    if (object->refused == 30)
      object->refused = 31;
    if (other->refused == 30)
      other->refused = 31;
  }
  if (object->refused > 2) {
    other->paused = 1;
    Event_SetFlag(FLAG_SCENE_TEMP_06);
  }
  {
    u8 *otherCount = &other->refused;

    if (*otherCount > 2 && !Event_GetFlag(FLAG_SCENE_TEMP_06) &&
        other->script != gGaiaRockInside_CaskTossedLeftScript &&
        other->script != gGaiaRockInside_CaskTossedRightScript) {
      object->paused = 1;
      Event_SetFlag(FLAG_SCENE_TEMP_05);
    }
    if (object->refused == 0) {
      other->paused = 0;
      Event_ClearFlag(FLAG_SCENE_TEMP_06);
    }
    if (*otherCount == 0) {
      object->paused = 0;
      Event_ClearFlag(FLAG_SCENE_TEMP_05);
    }
    if (object->refused > 35)
      object->refused = 35;
    if (*otherCount > 35)
      *otherCount = 35;
  }
}

extern struct ActorPlacement gGaiaRockInside_FirstAltarActorPlacements[];

extern s32 gGaiaRockInside_ArrowFrame;
extern s32 gGaiaRockInside_SquashXScript[];

/// Return the table for the current room.
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

/// The update of a maze plant Growth has turned into an arrow: it flickers, and
/// every 32 frames stretches along its arrow, sideways here and up and down in
/// GaiaRockInside_PulseUpDownArrow.
void GaiaRockInside_PulseSidewaysArrow(struct Actor *object) {
  s32 value = 0;

  if ((gGaiaRockInside_ArrowFrame & 7) <= 0)
    value = 7;
  Actor_SetSpriteColorMode(object, value & 7);
  if ((gGaiaRockInside_ArrowFrame & 31) == 0) {
    object->scaleX = FIXED_ONE;
    Actor_StartScript(object, gGaiaRockInside_SquashXScript);
    Event_PlaySound(SE_DRIP);
  }
  gGaiaRockInside_ArrowFrame++;
}

/// As GaiaRockInside_PulseSidewaysArrow, for an arrow pointing up or down.
void GaiaRockInside_PulseUpDownArrow(struct Actor *object) {
  s32 value = 0;

  if ((gGaiaRockInside_ArrowFrame & 7) <= 0)
    value = 7;
  Actor_SetSpriteColorMode(object, value & 7);
  if ((gGaiaRockInside_ArrowFrame & 31) == 0) {
    object->scaleY = FIXED_ONE;
    Actor_StartScript(object, gGaiaRockInside_SquashYScript);
    Event_PlaySound(SE_DRIP);
  }
  gGaiaRockInside_ArrowFrame++;
}

/// Growth on the plant of a maze step: it grows into an arrow pointing the way
/// on, and stays one for this visit.
void GaiaRockInside_PlantPointsTheWay(s32 unused, u32 id) {
  struct Actor *object = Actor_Get(id);
  u32 direction;
  s32 *state;

  direction = GaiaRockInside_GetMazeWayOn();
  state = &gPath->current;
  object->collisionFlags = 0;
  Actor_SelectAnimation(object, ((direction & 0xc000) >> 14) + 2);
  if (direction & 0x4000)
    object->update = (void (*)(void))GaiaRockInside_PulseUpDownArrow;
  else
    object->update = (void (*)(void))GaiaRockInside_PulseSidewaysArrow;
  object->collisionFlags = 1;
  Event_SetFlag(FLAG_MAZE_PLANT_POINTED(*state));
  Event_SetFlag(FLAG_SCENE_TEMP_00);
  System_Wait(20);
}

/// Cyclone_CopyTouchedCells, each argument passed as a word.
static __inline__ void GaiaRockInside_CopyTouchedCells(s32 x, s32 z,
                                                       s32 layerIndex,
                                                       s32 width, s32 height,
                                                       s32 toX, s32 toZ) {
  Cyclone_CopyTouchedCells(x, z, layerIndex, width, height, toX, toZ);
}

/// Unless FieldMapGrid_GetCellBehaviorAt(2, ...) holds, copies map cell (36,
/// 23) to (38, 25), moves a leader standing on that cell aside, and sets flag
/// FLAG_BUSHES_STREAM_TO_SHOW, or FLAG_BUSHES_STREAM_OPEN once
/// FLAG_SERPENT_SLAIN is set.
void GaiaRockInside_CycloneClearsHallBushes(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.leaderActorId);

  GaiaRockInside_CopyTouchedCells(37, 26, 0, 3, 3, 61, 0);
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
      Event_SetFlag(FLAG_BUSHES_STREAM_TO_SHOW);
      FieldEvent_Wait(30);
      ObjectTable_Snapshot();
      FieldEvent_SetExitRequest(20);
    } else {
      Event_SetFlag(FLAG_BUSHES_STREAM_OPEN);
      Event_End();
    }
  }
}

static __inline__ s32 GameFlag_Test_(s32 flag) { return GameFlag_Get(flag); }

/// MadPlant_LeapAtLeader, called through this inline as the ROM's call
/// compiles.
static __inline__ void GaiaRockInside_MadPlantLeaps(s32 sprite, s32 x, s32 z) {
  MadPlant_LeapAtLeader(sprite, x, z);
}

/// Cyclone on the bushes by the platforms: they blow away, and the first time
/// the platforms start to sway.
void GaiaRockInside_CycloneStartsPlatforms(void) {
  GaiaRockInside_CopyTouchedCells(16, 25, 0, 5, 5, 40, 8);
  if (GameFlag_Test_(FLAG_PLATFORMS_SWAYING) == 0) {
    Event_SetFlag(FLAG_PLATFORMS_SWAYING);
    GaiaRockInside_StartPlatforms(2);
  }
}

/// Cyclone on the bushes hiding a Mad Plant: they blow away, and until it is
/// beaten the plant leaps out to fight.
void GaiaRockInside_CycloneMadPlant(void) {
  GaiaRockInside_CopyTouchedCells(18, 17, 0, 3, 3, 31, 0);
  if (Event_GetFlag(FLAG_MAD_PLANT(2)) == 0) {
    GaiaRockInside_MadPlantLeaps(SPRITE_MAD_PLANT, 328, 344);
    gMainRoutineWork.pendingDjinni = IF_FLAG_SET(FLAG_MAD_PLANT(2));
    Battle_SetType(BATTLE_TYPE_NO_ESCAPE);
    Field_RequestBattleFromEncounterTable(ENCOUNTER_TABLE_MAD_PLANTS, 2);
  }
}

/// Cyclone on the bushes by the platforms blows them away.
void GaiaRockInside_ClearPlatformsBushes(void) {
  GaiaRockInside_CopyTouchedCells(19, 24, 0, 4, 3, 31, 4);
}

/// Cyclone on the smaller bushes by the platforms blows them away.
void GaiaRockInside_ClearPlatformsSmallBushes(void) {
  GaiaRockInside_CopyTouchedCells(18, 17, 0, 3, 3, 31, 0);
}

extern struct CellPoint gGaiaRockInside_CobwebCells[12];

/// Cyclone on the cobweb: once it is torn, the altar it held shakes and drops,
/// the leader steps back from under it, and the party goes on below.
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
  for (i = 0; i <= 11; i++) {
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
  if (leader->z.fixed >> 19 > 53 && leader->x.fixed >> 19 > 83 &&
      leader->x.fixed >> 19 <= 91) {
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
  {
    s32 destX = 40;
    s32 destY = 84;
    MapGrid_CopyCellAttributes(49, destY, 8, 8, destX, destY);
  }
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
  FieldEvent_SetExitRequest(9);
}

/// Walking into Susa: "Move out of the way there."
void GaiaRockInside_SusaSaysMoveAside(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgGaiaRockInsideMoveOutOfTheWay);
  // "Move out of the way there."
  Event_ShowMessage(ACTOR_SUSA, 0);
  Event_End();
}

/// Stops Susa and the cask he is handling where they are, keeping Susa's actor,
/// place and animation to be given back after a talk.
void GaiaRockInside_SaveSusa(void) {
  struct Actor *walker = Actor_Get(ACTOR_CASK);
  struct Actor *object = Actor_Get(ACTOR_SUSA);
  struct FieldSprite *sprite = object->sprite;
  u8 *record = sprite->record;

  Iwram_CopyWordsEntry(&gGaiaRockInside_SavedSusa, object,
                       sizeof(struct Actor));
  gGaiaRockInside_SavedSusaAnimation = record[21];
  gGaiaRockInside_SavedSusaFrame = sprite->lastNo;
  {
    FIXED x = object->x.fixed;
    FIXED z;

    gGaiaRockInside_SavedSusaX = x;
    z = object->z.fixed;
    object->update = 0;
    gGaiaRockInside_SavedSusaZ = z;
    object->velocityZ = 0;
    object->velocityY = 0;
    object->velocityX = 0;
    object->targetX = x;
    object->targetY = object->y.fixed;
    object->targetZ = z;
  }
  Actor_ResetTargetsAndVelocity(ACTOR_SUSA);
  Actor_SetAnimationSpeed(object, 10);
  object->speedLimit = toFIXED(1);
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
  struct Actor *object = Actor_Get(ACTOR_SUSA);

  if (gGaiaRockInside_SavedSusaX != object->x.fixed ||
      gGaiaRockInside_SavedSusaZ != object->z.fixed) {
    s16 *savedZ = (s16 *)&gGaiaRockInside_SavedSusaZ;

    Actor_WalkTo(ACTOR_SUSA, FIXEDtoINT(gGaiaRockInside_SavedSusaX), savedZ[1]);
    FieldActor_WaitForMoveById(ACTOR_SUSA);
  }
  if (walker->script == gGaiaRockInside_CaskThrownScript ||
      walker->script == gGaiaRockInside_CaskPushedScript) {
    walker->paused = 0;
    Actor_SetAnimationSpeed(walker, ANIMATION_STEP_NORMAL);
  }
  Iwram_CopyWordsEntry(object, &gGaiaRockInside_SavedSusa,
                       sizeof(struct Actor));
  Actor_SetAnimation(ACTOR_SUSA, gGaiaRockInside_SavedSusaFrame);
  Actor_SetAnimationSpeed(object, gGaiaRockInside_SavedSusaAnimation);
}

/// Stepping near Susa the first time: he turns on the party ("Who are you? How
/// did you get here?"), tells them to stand aside while the camera looks at the
/// Serpent, and goes back to his place; if he was walking back while a cask
/// was tossed, his saved place is where he was going.
void GaiaRockInside_MeetSusa(void) {
  struct Actor *object = Actor_Get(ACTOR_CASK);

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgGaiaRockInsideWhoAreYouHowDid);
  EventCamera_FollowActor(ACTOR_SUSA, 1);
  if (object->script == gGaiaRockInside_CaskEmptiesScript ||
      object->script == gGaiaRockInside_CaskPushedScript)
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
  if (object->script == gGaiaRockInside_CaskTossedLeftScript ||
      object->script == gGaiaRockInside_CaskTossedRightScript) {
    gGaiaRockInside_SavedSusa.x.fixed = gGaiaRockInside_SavedSusaX =
        gGaiaRockInside_SavedSusa.targetX;
    gGaiaRockInside_SavedSusa.z.fixed = gGaiaRockInside_SavedSusaZ =
        gGaiaRockInside_SavedSusa.targetZ;
  }
  GaiaRockInside_RestoreSusa();
  Event_End();
}

/// Talking to Susa while he stands: once he has attacked the Serpent he says
/// it is still too strong, in other words once any stream of Dragonsbane is
/// open; before that he stops, turns to the leader and sends the party aside.
void GaiaRockInside_TalkToSusa(void) {
  if (Actor_Get(ACTOR_SUSA)->refused > 2)
    return;
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  if (Event_GetFlag(FLAG_SUSA_ATTACKED_SERPENT)) {
    if (Event_GetFlag(FLAG_ALTAR_ROOM_STREAM_OPEN) ||
        Event_GetFlag(FLAG_WEST_ALTAR_STREAM_OPEN) ||
        Event_GetFlag(FLAG_EAST_ALTAR_STREAM_OPEN) ||
        Event_GetFlag(FLAG_BUSHES_STREAM_OPEN))
      FieldEvent_SetMessageId((s32)MsgGaiaRockInsideImDoneForThatDragon);
    else
      FieldEvent_SetMessageId((s32)MsgGaiaRockInsideTheSerpentIsStillToo);
    // "The serpent is still too strong. If only there were a way to weaken..."
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

/// Mind Read on Susa: what he thinks of the party once they hold the Dancing
/// Idol, or otherwise of the Serpent and his casks.
void GaiaRockInside_ReadSusasMind(void) {
  struct Actor *object;
  s32 holder;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  holder = FieldPartyInventory_FindMember(ITEM_DANCING_IDOL);
  if (holder != -1) {
    FieldEvent_SetMessageId((s32)MsgGaiaRockInsideWhoDoTheyThinkThey);
    // "Who do they think they are? I'm the only one who can save Kushinada."
    Event_ShowMessage(ACTOR_SUSA, 0);
  } else {
    object = Actor_Get(ACTOR_CASK);
    if (object->script == (s32)gGaiaRockInside_CaskEmptiesScript) {
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

extern u32 gGaiaRockInside_SparkScript[];

/// Leaves a small spark where an orbiting one is, released after 6 frames: the
/// orbit's trail.
void GaiaRockInside_SpawnSpark(struct Orbiter *source) {
  struct Actor *spark =
      FieldActor_Create(SPRITE_PARTICLE, source->x, source->y, source->z);

  spark->scaleY = FIXED_ONE * 2 / 5;
  spark->scaleX = FIXED_ONE * 2 / 5;
  spark->motionFlags = 0;
  FieldActor_SetSpriteColor(spark, 2);
  Scene_SetActorSpritePriority(spark, 1);
  Actor_SelectAnimation(spark, 1);
  Actor_SetSpriteFlags(spark, 0);
  Actor_SetBlendMode(spark, 1);
  Actor_StartScript(spark, (s32)gGaiaRockInside_SparkScript);
}

/// Turns an orbiting object by half its speed and puts it out from its anchor,
/// raised 8 pixels, by its radius.
static inline void Orbiter_Step(struct Orbiter *orbiter, struct Actor *anchor) {
  orbiter->angle += (s32)(orbiter->speed << 16) >> 17;
  orbiter->x = anchor->x.fixed;
  orbiter->y = anchor->y.fixed + toFIXED(8);
  orbiter->z = anchor->z.fixed;
  FieldVector_AddPolarOffsetFX(orbiter->orbitRadius, orbiter->angle,
                               &orbiter->x);
}

/// Steps an orbiting object around its anchor: turns it by half its speed, puts
/// it on the anchor (raised 8 pixels) and pushes it out by its radius; when its
/// countdown runs out returns 0, else, unless its radius is down to 2 pixels,
/// takes a second, shrinking step. Returns 1 while it runs.
s32 Orbiter_Spiral(struct Orbiter *orbiter) {
  struct Actor *anchor = orbiter->anchor;

  Orbiter_Step(orbiter, anchor);
  if (--orbiter->countdown == 0)
    return 0;
  if (orbiter->orbitRadius == toFIXED(2))
    return 1;
  GaiaRockInside_SpawnSpark(orbiter);
  orbiter->orbitRadius -= toFIXED(4);
  Orbiter_Step(orbiter, anchor);
  GaiaRockInside_SpawnSpark(orbiter);
  orbiter->orbitRadius -= toFIXED(4);
  return 1;
}

// Moves around in a circle.
#include "shared/orbit_update.h"

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))

#include "shared/actor_aim_at_watched.h"

#include "shared/actor_step_to_watched.h"

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);

#include "shared/actor_emit_bubbles.h"
#undef BYTE_AT

#include "text/msg_ids.h"

extern u32 gGaiaRockInside_TabletRiseScript[];
extern u32 gGaiaRockInside_RisingSparkScript[];
extern s32 gGaiaRockInside_CirclingSparkScript[];
extern u32 gGaiaRockInside_FelixRiseScript[];
extern s32 gGaiaRockInside_FallingSparkScript[];
extern s32 gGaiaRockInside_TabletSettleScript[];
extern s32 gGaiaRockInside_FelixSettleScript[];

/// Felix touches the slate in the lair and learns Sand: he and the slate rise
/// among circling sparks, then settle.
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
                                  a->y.part[0].integer << 16,
                                  a->z.part[0].integer << 16, 1);
  Sound_Play(SE_RUMBLE);
  MapGrid_SetShake(2, 2, 1);
  PaletteFade_SetSource(PALETTE_TINT_NONE, 0);
  PaletteFade_SetTarget(PALETTE_TINT_WARM, 1);
  PaletteFade_StartInterpolation(60);
  a->motionFlags = 0;
  a->acceleration = toFIXED(1) / 5;
  a->speedLimit = toFIXED(1) / 5;
  Actor_SetMovementTypeById(idA, gGaiaRockInside_TabletRiseScript);
  Sound_Play(SE_TABLET_RISE);
  Event_Wait(45);
  Actor_ConfigureSpriteColor(idA, 0x100);
  REG_BLDALPHA = BLEND_ALPHA(8, 16);
  REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL;
  for (i = 0; i != 16; i++) {
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
  for (i = 0; i != 16; i++) {
    struct Actor *spark =
        FieldActor_Create(SPRITE_PARTICLE, b->x.fixed, b->y.fixed, b->z.fixed);
    Sound_Play(SE_SPARKS);
    angle = &spark->timer;
    *angle = (Random16() & 0x7fff) + 0x8000;
    spark->userValue = ((i & 1) * 2 - 1) << ((Random16() & 3) + 9);
    spark->motionFlags = 0;
    spark->riseBaseY = toFIXED(122);
    spark->watchedActor = b;
    spark->riseCounter = (16 - i) * 8 + 15;
    Actor_SetSpriteColor(spark, 2);
    Scene_SetActorSpritePriority(spark, 1);
    Actor_SelectAnimation(spark, 7);
    Actor_SetSpriteFlags(spark, 0);
    Actor_SetBlendMode(spark, 1);
    FieldVector_AddPolarOffsetFX(spark->riseBaseY, *angle, &spark->x.fixed);
    Actor_StartScript(spark, gGaiaRockInside_CirclingSparkScript);
    if (i == 3) {
      Actor_SetSpriteColorById(idB, 0x100);
      b->motionFlags = 0;
      b->acceleration = toFIXED(1) / 5;
      b->speedLimit = toFIXED(1) / 5;
      Actor_SetMovementTypeById(idB, gGaiaRockInside_FelixRiseScript);
    }
    wait(8);
  }
  Sound_Play(SE_SHIMMER);
  wait(16);
  for (i = 2; i != 32; i += 2) {
    struct Actor *spark =
        FieldActor_Create(SPRITE_PARTICLE, b->x.fixed,
                          b->y.fixed + i * FIXED_ONE - toFIXED(8), b->z.fixed);
    spark->timer = Random16();
    spark->userValue = 0x800;
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
    Actor_StartScript(spark, gGaiaRockInside_FallingSparkScript);
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
  Actor_SetMovementTypeById(idA, gGaiaRockInside_TabletSettleScript);
  Actor_SetMovementTypeById(idB, gGaiaRockInside_FelixSettleScript);
  b->update = Actor_EmitBubbles;
  wait(120);
  b->update = 0;
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  Scene_PlayMusic();
  wait(1);
}

void Actor_MoveToObject(s32 id, s32 target);
void BlendScript_Suspend(void);
void BlendScript_Resume(void);

/// Talking to the slate: Kraden urges Felix to touch it, the ground shakes and
/// the slate teaches him Sand (GaiaRockInside_FelixLearnsSand), and Kraden
/// walks back into the party.
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
  if (leader->z.fixed >> 19 <= 39) {
    Actor_WalkTo(gMainRoutineWork.leaderActorId, 440, 328);
    FieldActor_WaitForMoveById(gMainRoutineWork.leaderActorId);
  }
  Actor_MoveToObject(ACTOR_KRADEN, gMainRoutineWork.leaderActorId);
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
  level &= 0xff;
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

  effect->velocity.x = velocityX - velocityX / 18;
  effect->velocity.z = velocityZ - velocityZ / 16;

  scaleRateX = effect->scaleRateX;
  effect->scaleX += scaleRateX;
  sprite = effect->sprite;
  effect->scaleY += effect->scaleRateY;
  sprite->rotation += effect->spin;
}

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);

/// A ring of dust spreads from the slate's place, one puff every eighth of a
/// turn.
void GaiaRockInside_RingOfDust(void) {
  FIXED velocity[3];
  struct EffectOptions options;
  u32 i;

  options.update = Effect_AdvanceMotionLocal;
  i = 0;
  do {
    {
      s32 angle = i << 12;

      velocity[0] = Trig_Cos(angle);
      velocity[1] = 0;
      velocity[2] = Math_Sin(angle);
      velocity[0] += velocity[0] / 3;
      FieldEffect_Spawn(toFIXED(464), toFIXED(-32), toFIXED(328), velocity[0],
                        velocity[1], velocity[2], EFFECT_USE_UPDATE | 1,
                        &options);
      i += 2;
    }
  } while (i <= 16);
}

/// Stepping before the slain Serpent the first time: the ground shakes and the
/// slate rises from the floor a pixel every 2 frames, in two rings of dust.
void GaiaRockInside_RaiseSandTablet(void) {
  struct Actor *person;
  struct Actor *object;
  u16 level;
  s32 i;

  if (GameFlag_Get(FLAG_SAND_TABLET_RAISED) || GameFlag_Get(FLAG_LEARNED_SAND))
    return;
  person = Actor_Get(ACTOR_SAND_TABLET);
  Io_Read16(&REG_BLDALPHA, &level);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  MapGrid_SetShakeFX(FIXED_ONE * 10, FIXED_ONE * 10, FIXED_ONE);
  Event_PlaySound(SE_TREMOR);
  BlendScript_Suspend();
  level &= 0xff;
  while (level != 0) {
    REG_BLDALPHA = level | BLEND_ALPHA(0, 16);
    level--;
    System_Wait(1);
  }
  REG_DISPCNT &= ~DISPCNT_BG1_ON;
  FieldEvent_Wait(30);
  object = OverlayObject_PrepareObject(toFIXED(464), -FIXED_ONE * 32,
                                       toFIXED(348), SPRITE_SLATE);
  Actor_SetPosition(ACTOR_SAND_TABLET, 464, 348);
  GaiaRockInside_RingOfDust();
  for (i = 0; i <= 19; i++) {
    if (i == 10)
      GaiaRockInside_RingOfDust();
    person->z.fixed -= FIXED_ONE;
    FieldEvent_Wait(2);
  }
  Event_SetFlag(FLAG_SAND_TABLET_RAISED);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  object->x.fixed = 0;
  object->z.fixed = 0;
  FieldEvent_Wait(1);
  Actor_Release(object);
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

/// Examining the Serpent: once it is slain, it only looks almost alive;
/// otherwise the battle starts, as hard as the streams of Dragonsbane still
/// closed make it. Winning comes back by entrance 10; losing by entrance 11, or
/// by 12 when the party carries the doll and opened no stream.
void GaiaRockInside_ChallengeSerpent(void) {
  s32 count;

  if (Event_GetFlag(FLAG_SERPENT_SLAIN)) {
    // "The serpent, with its eyes closed, looks almost alive."
    Message_ShowCentered((s32)MsgGaiaRockInsideTheSerpentWithItsEyes, 1);
  } else {
    s32 a = Event_GetFlag(FLAG_ALTAR_ROOM_STREAM_OPEN);
    s32 b = Event_GetFlag(FLAG_WEST_ALTAR_STREAM_OPEN);
    s32 c = Event_GetFlag(FLAG_EAST_ALTAR_STREAM_OPEN);
    s32 scene;

    count = a + b + c + Event_GetFlag(FLAG_BUSHES_STREAM_OPEN);
    Battle_SetType(BATTLE_TYPE_NO_ESCAPE);
    scene = SCENE_GAIA_ROCK_INSIDE_MAZE;
    FieldEvent_SetAfterBattleDestination(scene, ENTRANCE_SERPENT_BEATEN);
    if (FieldPartyInventory_FindMember(ITEM_DANCING_IDOL) != -1) {
      if (count == 0)
        Event_SetDefeatDestination(scene, ENTRANCE_BEATEN_HOLDING_IDOL);
    } else {
      Event_SetDefeatDestination(scene, ENTRANCE_BEATEN_BY_SERPENT);
    }
    Field_RequestBattleFromEncounterTable(12, count);
  }
}

#include "text/msg_ids.h"

/// Puts the leader on map layer 2, through its byte.
void Leader_SetLayerByte(void) {
  Actor_Get(gMainRoutineWork.leaderActorId)->layer = 2;
}

/// Puts the leader back on map layer 0 as Sand ends.
void GaiaRockInside_EndSand(void) {
  Actor_Get(gMainRoutineWork.leaderActorId)->layer = 0;
}

/// The Cloud Brand's treasure spot is raised to where the party can reach it,
/// and its chest is opened without the message of checking it.
void GaiaRockInside_FindCloudBrand(void) {
  struct Actor *object;

  object = Actor_Get(FIELD_TREASURE_ACTOR + 2);
  object->x.fixed = toFIXED(552);
  object->z.fixed = toFIXED(296);
  object->y.fixed = toFIXED(-30);
  object->groundHeight = toFIXED(-32);
  Event_ClearFlag(FLAG_NOTHING_HAPPENS);
  Scene_TalkToActor(TALK_WITHOUT_CHECK_MESSAGE | (FIELD_TREASURE_ACTOR + 2));
}

/// Examines the altar.
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
void Actor_MoveToObject(s32 id, s32 target);

/// Placing the doll on the first altar: its two faces stir.
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
  Actor_MoveToObject(ACTOR_FIRST_ALTAR_DOLL, *leaderId);
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
  Event_PlaySound(SE_191);
  if (!Event_GetFlag(FLAG_FIRST_ALTAR_FACES_MOVED)) {
    for (i = 19; i >= 0; i--) {
      westFace->velocityY = Random16();
      eastFace->velocityY = Random16();
      System_Wait(1);
    }
    Actor_SetSpeedAndAcceleration(ACTOR_FIRST_ALTAR_WEST_FACE, 0.25, 0.05);
    Actor_SetSpeedAndAcceleration(ACTOR_FIRST_ALTAR_EAST_FACE, 0.25, 0.05);
    Actor_SetDestinationOffset(ACTOR_FIRST_ALTAR_WEST_FACE, 24, 0);
    Actor_SetDestinationOffset(ACTOR_FIRST_ALTAR_EAST_FACE, -24, 0);
    for (i = 0; i < 300 && westFace->arrivalAxis != 0; i++) {
      westFace->velocityY = Random16();
      eastFace->velocityY = Random16();
      System_Wait(1);
    }
    for (i = 9; i >= 0; i--) {
      westFace->velocityY = Random16();
      eastFace->velocityY = Random16();
      System_Wait(1);
    }
    i = 40;
    MapGrid_CopyCells(119, 31, 1, 2, 89, i);
    MapGrid_CopyCells(119, 31, 1, 2, 100, i);
    Event_SetFlag(FLAG_FIRST_ALTAR_FACES_MOVED);
  } else {
    for (i = 119; i >= 0; i--) {
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

/// The doll placed on an altar under a face (this one, the east altar's and
/// the altar room's are alike): the doll hops onto the altar and dances, the
/// face shakes, and the first time it slides aside and opens its stream of
/// Dragonsbane; the doll hops back to the leader. Before the Serpent is slain
/// the party is taken to the lair to see the stream pour (entrance 20).
void GaiaRockInside_PlaceDollOnWestAltar(void) {
  struct Actor *object;
  struct Actor *partner;
  struct Actor *leader;
  s32 i;

  object = Actor_GetById(ACTOR_WEST_FACE);
  partner = Actor_GetById(ACTOR_TWIN_ALTARS_DOLL);
  Event_Start();
  Event_ResolvePendingActions(0);
  // "Felix placed a doll on the altar."
  FieldMessage_Show((s32)MsgGaiaRockInsidePlacedADollOnThe, 1);
  Sound_Play(SE_DOLL_PLACED);
  Actor_SetSpritePriorityDirect(partner, 1);
  Actor_MoveToObject(ACTOR_TWIN_ALTARS_DOLL, gMainRoutineWork.leaderActorId);
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
  Sound_Play(SE_191);
  if (!GameFlag_Get(FLAG_WEST_ALTAR_STREAM_OPEN)) {
    for (i = 19; i >= 0; i--) {
      object->velocityY = Random16();
      wait(1);
    }
    Actor_SetSpeedAndAcceleration(ACTOR_WEST_FACE, 0.25, 0.05);
    Actor_SetDestinationOffsetById(ACTOR_WEST_FACE, 24, 0);
    MapGrid_CopyCellAttributes(27, 23, 1, 1, 28, 22);
    Map_CopyCellAttributeRect(32, 16, 2, 1, 29, 22);
    i = 0;
    if (object->arrivalAxis != 0) {
      do {
        object->velocityY = Random16();
        wait(1);
        if (++i > 299)
          break;
      } while (object->arrivalAxis != 0);
    }
    MapGrid_CopyCells(2, 93, 1, 5, 28, 85);
    for (i = 9; i >= 0; i--) {
      object->velocityY = Random16();
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
      GameFlag_Set(FLAG_WEST_ALTAR_STREAM_TO_SHOW);
      Event_Wait(30);
      ObjectTable_Snapshot();
      Import_EventSetExitRequest(20);
    } else {
      GameFlag_Set(FLAG_WEST_ALTAR_STREAM_OPEN);
      Event_End();
    }
  } else {
    for (i = 119; i >= 0; i--) {
      object->velocityY = Random16();
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

/// The doll placed on the east altar, as on the west one
/// (GaiaRockInside_PlaceDollOnWestAltar).
void GaiaRockInside_PlaceDollOnEastAltar(void) {
  struct Actor *object;
  struct Actor *partner;
  struct Actor *leader;
  s32 i;

  object = Actor_GetById(ACTOR_EAST_FACE);
  partner = Actor_GetById(ACTOR_TWIN_ALTARS_DOLL);
  Event_Start();
  Event_ResolvePendingActions(0);
  // "Felix placed a doll on the altar."
  FieldMessage_Show((s32)MsgGaiaRockInsidePlacedADollOnThe, 1);
  Sound_Play(SE_DOLL_PLACED);
  Actor_SetSpritePriorityDirect(partner, 1);
  Actor_MoveToObject(ACTOR_TWIN_ALTARS_DOLL, gMainRoutineWork.leaderActorId);
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
  Sound_Play(SE_191);
  if (!GameFlag_Get(FLAG_EAST_ALTAR_STREAM_OPEN)) {
    for (i = 19; i >= 0; i--) {
      object->velocityY = Random16();
      wait(1);
    }
    Actor_SetSpeedAndAcceleration(ACTOR_EAST_FACE, 0.25, 0.05);
    Actor_SetDestination(ACTOR_EAST_FACE, 608, 352);
    MapGrid_CopyCellAttributes(35, 23, 1, 1, 36, 22);
    Map_CopyCellAttributeRect(32, 16, 2, 1, 37, 22);
    i = 0;
    if (object->arrivalAxis != 0) {
      do {
        object->velocityY = Random16();
        wait(1);
        if (++i > 299)
          break;
      } while (object->arrivalAxis != 0);
    }
    MapGrid_CopyCells(2, 93, 1, 5, 36, 85);
    for (i = 9; i >= 0; i--) {
      object->velocityY = Random16();
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
      GameFlag_Set(FLAG_EAST_ALTAR_STREAM_TO_SHOW);
      Event_Wait(30);
      ObjectTable_Snapshot();
      Import_EventSetExitRequest(20);
    } else {
      GameFlag_Set(FLAG_EAST_ALTAR_STREAM_OPEN);
      Event_End();
    }
  } else {
    for (i = 119; i >= 0; i--) {
      object->velocityY = Random16();
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

/// The doll placed on the altar room's altar, as on the west one
/// (GaiaRockInside_PlaceDollOnWestAltar).
void GaiaRockInside_PlaceDollOnAltarAltar(void) {
  struct Actor *object;
  struct Actor *partner;
  struct Actor *leader;
  s32 i;

  object = Actor_GetById(ACTOR_ALTAR_FACE);
  partner = Actor_GetById(ACTOR_ALTAR_DOLL);
  Event_Start();
  Event_ResolvePendingActions(0);
  // "Felix placed a doll on the altar."
  FieldMessage_Show((s32)MsgGaiaRockInsidePlacedADollOnThe, 1);
  Sound_Play(SE_DOLL_PLACED);
  Actor_SetSpritePriorityDirect(partner, 1);
  Actor_MoveToObject(ACTOR_ALTAR_DOLL, gMainRoutineWork.leaderActorId);
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
  Sound_Play(SE_191);
  if (!GameFlag_Get(FLAG_ALTAR_ROOM_STREAM_OPEN)) {
    for (i = 19; i >= 0; i--) {
      object->velocityY = Random16();
      wait(1);
    }
    Actor_SetSpeedAndAcceleration(ACTOR_ALTAR_FACE, 0.25, 0.05);
    Actor_SetDestinationOffset(ACTOR_ALTAR_FACE, -16, 0);
    Map_CopyCellAttributeRect(27, 15, 2, 1, 23, 23);
    Map_CopyCellAttributeRect(25, 25, 1, 1, 25, 23);
    i = 0;
    if (object->arrivalAxis != 0) {
      do {
        object->velocityY = Random16();
        wait(1);
        if (++i > 299)
          break;
      } while (object->arrivalAxis != 0);
    }
    MapGrid_CopyCells(1, 85, 1, 4, 25, 85);
    for (i = 9; i >= 0; i--) {
      object->velocityY = Random16();
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
      GameFlag_Set(FLAG_ALTAR_ROOM_STREAM_TO_SHOW);
      Event_Wait(30);
      ObjectTable_Snapshot();
      Import_EventSetExitRequest(20);
    } else {
      GameFlag_Set(FLAG_ALTAR_ROOM_STREAM_OPEN);
      Event_End();
    }
  } else {
    for (i = 119; i >= 0; i--) {
      object->velocityY = Random16();
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

extern s16 gGaiaRockInside_RockSpots[][2];

/// Records which of eleven listed cells persons 10 and 11 stand on, as the two
/// halves of the byte flag FLAG_ROCK_SPOTS.
s32 GaiaRockInside_RecordRockSpots(void) {
  struct Actor *first = Actor_Get(ACTOR_ROCK_A);
  s8 firstIndex = 0;
  s32 firstX = first->x.fixed >> 19;
  s8 secondIndex = 0;
  s32 firstZ = first->z.fixed >> 19;
  struct Actor *second = Actor_Get(ACTOR_ROCK_B);
  s32 secondX = second->x.fixed >> 19;
  s32 secondZ = second->z.fixed >> 19;
  s8 i;

  for (i = 1; i <= 11; i++) {
    if (firstX == gGaiaRockInside_RockSpots[i][0] &&
        firstZ == gGaiaRockInside_RockSpots[i][1])
      firstIndex = i;
    if (secondX == gGaiaRockInside_RockSpots[i][0] &&
        secondZ == gGaiaRockInside_RockSpots[i][1])
      secondIndex = i;
  }
  {
    u32 high = secondIndex << 28;
    return GameFlag_SetByte(FLAG_ROCK_SPOTS,
                            (s32)((firstIndex << 24) | high) >> 24);
  }
}

/// Stepping on the first statue's spot raises it under the leader.
void GaiaRockInside_StepOnFirstStatueSpot(void) {
  GaiaRockInside_StatueRisesUnderLeader(ACTOR_STATUE_A, FLAG_STATUE_A_RISEN);
}

/// Stepping on the second statue's spot raises it under the leader.
void GaiaRockInside_StepOnSecondStatueSpot(void) {
  GaiaRockInside_StatueRisesUnderLeader(ACTOR_STATUE_B, FLAG_STATUE_B_RISEN);
}

/// Stepping on the third statue's spot raises it under the leader.
void GaiaRockInside_StepOnThirdStatueSpot(void) {
  GaiaRockInside_StatueRisesUnderLeader(ACTOR_STATUE_C, FLAG_STATUE_C_RISEN);
}

extern struct Trigger gGaiaRockInside_Triggers[];
extern struct Trigger gGaiaRockInside_LairTriggers[];
extern struct Trigger gGaiaRockInside_SlainLairTriggers[];
extern struct Trigger gGaiaRockInside_FirstAltarTriggers[];
extern struct Trigger gGaiaRockInside_TwinAltarsTriggers[];
extern struct Trigger gGaiaRockInside_PlatformsTriggers[];
extern struct Trigger gGaiaRockInside_AltarTriggers[];

/// Returns the current room's table.
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
  struct Actor *object = Actor_Get(id);

  object->motionFlags = 0;
  object->priorityFlags |= ACTOR_PRIORITY_PLATFORM;
  object->acceleration = toFIXED(1) / 16;
  object->speedLimit = toFIXED(1);
  object->y.fixed = toFIXED(16);
  object->timer = FIXEDtoINT(object->x.fixed);
  object->userValue = FIXEDtoINT(object->z.fixed);
  Actor_SetAnimation(id, mode);
}

/// Sends a platform toward its home moved dx and dz cells.
void GaiaRockInside_PlacePlatform(s32 id, s32 dx, s32 dz) {
  struct Actor *platform = Actor_GetById(id);

  FieldActor_SetMoveTargetFX(platform, INTtoFIXED((dx << 4) + platform->timer),
                             platform->y.fixed,
                             ((dz << 4) + platform->userValue) << 16);
}

extern struct SwayState gSceneState;

/// The platforms' frame task: they all stop while the menu is open or
/// Psynergy is cast; otherwise, at frame 60 of every 240 each leans out one way
/// and at frame 180 they come back.
void GaiaRockInside_SwayPlatforms(void) {
  // Pointers that stay on one global for their whole life.
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
      id = ACTOR_FIRST_PLATFORM;
      do {
        GaiaRockInside_PlacePlatform(id, 0, 0);
        id++;
      } while (id <= ACTOR_LAST_PLATFORM);
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
      Actor_SetPositionFX(id, gGaiaRockInside_PlatformPlaces[i][0] << 19,
                          gGaiaRockInside_PlatformPlaces[i][1] << 19);
      GaiaRockInside_SetPlatformMode(id, mode);
    }
  }
  {
    s32 priority = TASK_PASS_VBLANK;

    Task_AddFrame(GaiaRockInside_SwayPlatforms, priority);
  }
}

/// Opens the passages on each side of the hall by how many of its two pairs of
/// flags (FLAG_ALTAR_ROOM_STREAM_OPEN and FLAG_WEST_ALTAR_STREAM_OPEN,
/// FLAG_EAST_ALTAR_STREAM_OPEN and FLAG_BUSHES_STREAM_OPEN) are set: both open
/// the wide way, one a side way, and a single side opened alone also opens its
/// connecting cells.
void GaiaRockInside_OpenHallPassages(void) {
  u8 a = GameFlag_Get(FLAG_ALTAR_ROOM_STREAM_OPEN);
  u8 third = GameFlag_Get(FLAG_EAST_ALTAR_STREAM_OPEN);
  s32 b = GameFlag_Get(FLAG_WEST_ALTAR_STREAM_OPEN);
  s8 first = a;
  s8 sideThird = third;
  u8 sumLeft = b + first;
  s32 d = GameFlag_Get(FLAG_BUSHES_STREAM_OPEN);
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
extern s32 gGaiaRockInside_SusaWalksOffScript[];

struct Actor *OverlayObject_PrepareObject(s32 x, s32 y, s32 z, s32 kind);
s32 __modsi3(s32 numerator, s32 denominator);
void GaiaRockInside_TurnFaceToAltar();
void GaiaRockInside_RingOfDust(void);
void GaiaRockInside_PlaceStatue(s32 id, s32 x, s32 y, s32 z);
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

/// Room entry for the five Gaia Rock rooms, chosen by scene, then by entrance
/// in the first room.
s32 Scene_Initialize(void) {
  struct MainRoutineWork *room;
  struct MainRoutineWork *work;
  s16 *entrance;
  s16 *early;

  work = &gMainRoutineWork;
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  if (work->next == SCENE_GAIA_ROCK_INSIDE_MAZE) {
    early = &work->entrance;
    if (work->entrance > 8) {
      Actor_SetSpriteFlags(Actor_GetById(ACTOR_SERPENT), 0);
      GaiaRockInside_OpenHallPassages();
    }
    if (*early == 20) {
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
        s32 ending = 20;
        if (Test(FLAG_ALTAR_ROOM_STREAM_TO_SHOW)) {
          GameFlag_Clear(FLAG_ALTAR_ROOM_STREAM_TO_SHOW);
          GameFlag_Set(FLAG_ALTAR_ROOM_STREAM_OPEN);
          ending = 21;
        }
        if (Test(FLAG_WEST_ALTAR_STREAM_TO_SHOW)) {
          GameFlag_Clear(FLAG_WEST_ALTAR_STREAM_TO_SHOW);
          GameFlag_Set(FLAG_WEST_ALTAR_STREAM_OPEN);
          ending = 22;
        }
        if (Test(FLAG_EAST_ALTAR_STREAM_TO_SHOW)) {
          GameFlag_Clear(FLAG_EAST_ALTAR_STREAM_TO_SHOW);
          GameFlag_Set(FLAG_EAST_ALTAR_STREAM_OPEN);
          ending = 23;
        }
        if (Test(FLAG_BUSHES_STREAM_TO_SHOW)) {
          GameFlag_Clear(FLAG_BUSHES_STREAM_TO_SHOW);
          GameFlag_Set(FLAG_BUSHES_STREAM_OPEN);
          ending = 24;
        }
        Sound_Play(SE_EYES_GLOW);
        GaiaRockInside_OpenHallPassages();
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
    if ((u16)(gMainRoutineWork.entrance - 9) <= 1) {
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
      FIXED none;
      s32 found;

      if (Test(FLAG_SERPENT_SLAIN))
        goto end;
      Sound_Play(MUS_FLAME_DRAGONS);
      found = FieldPartyInventory_FindMember(ITEM_DANCING_IDOL);
      none = -1;
      if (found != none) {
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
        struct Actor *object;
        struct Actor *shards[4];

        flag = Test(FLAG_SUSA_ATTACKED_SERPENT);
        if (flag != 0) {
          Actor_SetAnimationById(ACTOR_SERPENT, 2);
          Actor_SetPosition(ACTOR_CASK, 0, 0);
          Actor_SetPosition(ACTOR_SUSA, 432, 320);
          Actor_SetSpritePriority(ACTOR_SUSA, 3);
          MapGrid_CopyCellAttributes(30, 19, 1, 1, 27, 19);
          Actor_SetAnimationById(ACTOR_SUSA, 5);
          goto end;
        }
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
        FieldCamera_MoveToFX(toFIXED(488), none, toFIXED(384), 1);
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
        Actor_SetAnimationSpeed(susa, ANIMATION_STEP_NORMAL);
        Actor_SetAnimationSpeed(serpent, ANIMATION_STEP_NORMAL);
        mask = 254;
        Actor_GetById(ACTOR_SUSA)->steeringFlags &= mask;
        Actor_WalkTo(ACTOR_SUSA, 456, 360);
        fast = toFIXED(4);
        susa->velocityY = fast;
        Actor_SetAttachedEffect(ACTOR_SUSA, ATTACHED_EFFECT_IN_FRONT |
                                                ATTACHED_EFFECT_ONCE);
        Actor_WaitForMoveById(ACTOR_SUSA);
        Actor_ShowEmoteById(ACTOR_SUSA, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
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
        object = FieldActor_Create(SPRITE_DUST_CLOUD, toFIXED(488),
                                   -toFIXED(16), toFIXED(345));
        Actor_SetSpriteFlags(object, 0);
        Actor_StartScript(object, gGaiaRockInside_SerpentSmokeScript);
        Actor_SelectAnimation(object, 1);
        Actor_SetSpriteColorMode(object, 6);
        Actor_Jump(ACTOR_SUSA, 2, 0);
        Event_Wait(5);
        Actor_ShowEmoteById(ACTOR_SUSA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
        Event_Wait(55);
        Event_Wait(5);
        Sound_Play(SE_WHOOSH);
        object = FieldActor_Create(SPRITE_FIREBALL, toFIXED(488), -toFIXED(16),
                                   toFIXED(345));
        Actor_SetSpriteFlags(object, 0);
        Actor_StartScript(object, gGaiaRockInside_SerpentGlowScript);
        ((struct FieldSprite *)object->sprite)->rotation = DEGtoANG(45);
        object->speedLimit = toFIXED(3) / 2;
        *(u8 *)&object->motionFlags = 0;
        object->targetX = toFIXED(456);
        object->targetY = -toFIXED(32);
        object->targetZ = toFIXED(358);
        object->scaleX = FIXED_ONE * 3 / 8;
        object->scaleY = FIXED_ONE * 3 / 8;
        Event_Wait(10);
        Actor_SetAnimationSpeed(serpent, ANIMATION_STEP_NORMAL);
        Event_Wait(10);
        Sound_Play(SE_IMPACT);
        object->x.fixed = 0;
        object->z.fixed = 0;
        MapGrid_SetScreenShake(fast, fast, 0x10000);
        for (i = 0; i < 4; i++) {
          dx = -8;
          if (i > 1)
            dx = 8;
          dz = -8;
          if ((i & 1) == 0)
            dz = 8;
          shards[i] = (struct Actor *)Import_ActorCreate(
              SPRITE_FLAME_PUFF, toFIXED(456) + INTtoFIXED(dx), -toFIXED(32),
              toFIXED(358) + INTtoFIXED(dz));
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
        goto end;
      } else {
        if (Test(FLAG_KEEP_PARTY_POSITION))
          goto end;
        Actor_SetPosition(ACTOR_SUSA, 472, 456);
        Actor_SetMovementTypeById(ACTOR_SUSA,
                                  gGaiaRockInside_SusaWalksOffScript);
        goto end;
      }
    } else if (*entrance == ENTRANCE_SERPENT_BEATEN) {
      s32 flag;
      FIXED scale;
      struct Actor *susa;
      s32 slow;
      s32 *owner;
      struct Actor *other;
      struct Actor *object;
      volatile u16 alpha;
      s32 i;

      Sound_Play(MUS_IZUMO);
      flag = Test(FLAG_SERPENT_SLAIN);
      if (flag != 0)
        goto end;
      Event_Start();
      Event_ResolvePendingActions(0);
      GameFlag_Set(FLAG_SERPENT_SLAIN);
      Event_SetMessageId((s32)MsgFieldItLooksLikeTheDragon);
      Actor_SetPosition(ACTOR_CASK, 0, 0);
      Actor_SetAnimationById(ACTOR_SERPENT, 3);
      Actor_GetById(ACTOR_KRADEN)->speedLimit = scale = 0x10000;
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
        alpha = alpha + 0xffff;
        wait(1);
      }
      REG_DISPCNT &= (u16)~DISPCNT_BG1_ON;
      Event_Wait(30);
      object = OverlayObject_PrepareObject(toFIXED(464), -toFIXED(32),
                                           toFIXED(348), SPRITE_SLATE);
      Actor_SetPosition(ACTOR_SAND_TABLET, 464, 348);
      for (i = 0; i < 20; i++) {
        if (i % 5 == 0)
          GaiaRockInside_RingOfDust();
        other->z.fixed += -toFIXED(1);
        Event_Wait(2);
      }
      GameFlag_Set(FLAG_SAND_TABLET_RAISED);
      MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.899993896484375);
      object->x.fixed = 0;
      object->z.fixed = 0;
      Event_Wait(1);
      Actor_Release(object);
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
    s32 mask = 32;
    s32 low;
    s32 high;
    struct Actor *object = Actor_Get(ACTOR_FIRST_ALTAR_WEST_FACE);

    object->priorityFlags |= mask;
    Actor_SetSpritePriorityDirect(object, 3);
    object = Actor_Get(ACTOR_FIRST_ALTAR_EAST_FACE);
    object->priorityFlags |= mask;
    Actor_SetSpritePriorityDirect(object, 3);
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
    low = FieldGameFlag_GetByte(FLAG_ROCK_SPOTS) & 15;
    high = (u8)(FieldGameFlag_GetByte(FLAG_ROCK_SPOTS) >> 4);
    if (low != 0)
      Actor_SetHorizontalPositionWithTerrain(
          ACTOR_ROCK_A, gGaiaRockInside_RockSpots[low][0] << 19,
          gGaiaRockInside_RockSpots[low][1] << 19);
    if ((s8)high != 0)
      Actor_SetHorizontalPositionWithTerrain(
          ACTOR_ROCK_B, gGaiaRockInside_RockSpots[(s8)high][0] << 19,
          gGaiaRockInside_RockSpots[(s8)high][1] << 19);
  } else if (work->next == SCENE_GAIA_ROCK_INSIDE_PLATFORMS) {
    if (Test(FLAG_PLATFORMS_SWAYING))
      GaiaRockInside_StartPlatforms(1);
  } else if (work->next == SCENE_GAIA_ROCK_INSIDE_TWIN_ALTARS) {
    struct Actor *object;
    struct Actor *other;

    object = Actor_GetById(ACTOR_WEST_ALTAR);
    Actor_SetSpritePriorityDirect(object, 3);
    GaiaRockInside_TurnFaceToAltar(ACTOR_WEST_FACE, ACTOR_WEST_ALTAR);
    GaiaRockInside_TurnFaceToAltar(ACTOR_EAST_FACE, ACTOR_EAST_ALTAR);
    if (!Test(FLAG_ALTAR_ROOM_STREAM_OPEN))
      MapGrid_CopyCells(25, 77, 1, 15, 26, 77);
    if (Test(FLAG_WEST_ALTAR_STREAM_OPEN)) {
      s32 width;
      s32 height;

      Actor_SetPosition(ACTOR_WEST_FACE, 480, 352);
      width = 28;
      Map_CopyCellsRect(2, 93, 1, 5, width, 85);
      height = 22;
      Map_CopyCellAttributeRect(27, 23, 1, 1, width, height);
      Map_CopyCellAttributeRect(32, 16, 2, 1, 29, height);
    }
    if (Test(FLAG_EAST_ALTAR_STREAM_OPEN)) {
      s32 width;
      s32 height;

      Actor_SetPosition(ACTOR_EAST_FACE, 608, 352);
      width = 36;
      Map_CopyCellsRect(2, 93, 1, 5, width, 85);
      height = 22;
      Map_CopyCellAttributeRect(35, 23, 1, 1, width, height);
      Map_CopyCellAttributeRect(32, 16, 2, 1, 37, height);
    }
    if (Test(FLAG_BUSHES_STREAM_OPEN)) {
      MapGrid_CopyCellsLayered(61, 0, 3, 3, 37, 26);
      MapGrid_CopyCellAttributes(36, 23, 1, 1, 38, 25);
    }
    if (work->entrance == 6) {
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
      object = Actor_GetById(ACTOR_EAST_ALTAR);
      other = FieldActor_Create(SPRITE_LIGHT, object->x.fixed, object->y.fixed,
                                object->z.fixed);
      if (other != NULL) {
        Actor_SetSpriteFlags(other, 0);
        Actor_StartScript(other, gGaiaRockInside_KushinadaScript);
        Actor_SelectAnimation(other, 1);
        Actor_SetSpriteColorMode(other, SPRITE_COLOR_BLACK);
        ((u8 *)other->sprite)[9] |= 12;
        other->scaleX = FIXED_ONE * 6 / 5;
        other->scaleY = FIXED_ONE * 6 / 5;
      }
      object->y.fixed += toFIXED(128);
      for (; frames != 0; frames--) {
        wait(1);
        if (object->velocityY > -1)
          break;
      }
      Sound_Play(SE_THUMP);
      Event_Wait(30);
      GameFlag_Set(FLAG_KEEP_PARTY_POSITION);
      Import_EventSetExitRequest(3);
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
      GaiaRockInside_PlaceStatue(ACTOR_STATUE_A, 0x1800000, 0x4b0000,
                                 0x2a80000);
    if (Test(FLAG_STATUE_B_RISEN))
      GaiaRockInside_PlaceStatue(ACTOR_STATUE_B, 0x1cf0000, 0x330000,
                                 0x2a80000);
    if (Test(FLAG_STATUE_C_RISEN))
      GaiaRockInside_PlaceStatue(ACTOR_STATUE_C, 0x2350000, 0x370000,
                                 0x2a80000);
    if (Test(FLAG_ALTAR_ROOM_STREAM_OPEN)) {
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
end:
  FieldEffect_StartBlankOverlay(0);
  return 0;
}

/// Clears map layer 1 of every cell but those of collision code keep.
void GaiaRockInside_ClearMazeLayer(s32 keep) {
  GaiaRockInside_SetMazeLayer(1, keep);
}

struct Unit *Unit_Get(u32 unitId);
void BlendScript_Start(u16 *script);

/// Prepares the map: in the maze, lays the path from the leader's name and
/// draws the step the party stands on, or readies the lair's blend; in the
/// altar rooms, clears the overhead layer of all but the entrance's part.
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

      for (i = 0; i <= 13; i++)
        total += unit->name[i];
      GaiaRockInside_LayMaze(total);
      Actor_GetById(Party_GetLeaderActor())->priorityFlags |=
          ACTOR_PRIORITY_IGNORES_RISES;
      ((struct MapWork *)map)->enabled[1] = 1;
      {
        volatile u16 control = REG_BG2CNT & ~3;
        // Once-only block: keeps the sky cell window's store after its read.
        do {
        } while (0);

        REG_BG2CNT = control | 1;
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
        Actor_SelectAnimation(plant, ((angle & 0xc000) >> 14) + 2);
        plant->update = (angle & 0x4000) != 0
                            ? (void (*)(void))GaiaRockInside_PulseUpDownArrow
                            : (void (*)(void))GaiaRockInside_PulseSidewaysArrow;
        plant->collisionFlags = 1;
        GameFlag_Set(FLAG_SCENE_TEMP_00);
      }
    } else if (gMainRoutineWork.entrance > 8) {
      struct MapLayer *layer;

      gPath->current = 1;
      layer = &((struct MapWork *)map)->layers[2];
      layer->maskX = 127;
      layer->speedX = 0;
      layer->speedY = 0;
      layer->phaseX = 0;
      layer->phaseY = 0;
      layer->scaleX = FIXED_ONE;
      layer->scaleY = FIXED_ONE;
      layer->maskY = 0x3f80;
      // Once-only block: keeps the scale defaults before the position sums.
      do {
      } while (0);
      layer->x = Iwram_MulFX(((struct MapWork *)map)->viewX, layer->scaleX) +
                 layer->offsetX;
      layer->y = Iwram_MulFX(((struct MapWork *)map)->viewZ, layer->scaleY) +
                 layer->offsetY;
      MapGrid_CenterOnFollowedPoint();
      System_Wait(1);
      Iwram_CopyWords(Ram_MapCollision + 0x1e00, gGaiaRockInside_SavedCollision,
                      24);
      BlendScript_Start((u16 *)(Ram_MapCollision + 0x1e00));
    }
  } else if (gMainRoutineWork.next == SCENE_GAIA_ROCK_INSIDE_FIRST_ALTAR) {
    switch (gMainRoutineWork.entrance) {
    case 1:
    case 2:
    case 5:
    case 8:
      GaiaRockInside_ClearMazeLayer(100);
      break;
    case 9:
    case 10:
      GaiaRockInside_ClearMazeLayer(102);
      break;
    case 3:
    case 4:
      GaiaRockInside_ClearMazeLayer(103);
      break;
    case 6:
    case 7:
      GaiaRockInside_ClearMazeLayer(101);
      break;
    }
  } else if (gMainRoutineWork.next == SCENE_GAIA_ROCK_INSIDE_ALTAR) {
    switch (gMainRoutineWork.entrance) {
    case 1:
    case 3:
    case 5:
      GaiaRockInside_ClearMazeLayer(100);
      break;
    case 2:
    case 4:
    case 7:
      GaiaRockInside_ClearMazeLayer(101);
      break;
    case 6:
      GaiaRockInside_ClearMazeLayer(102);
      break;
    }
  }
  return 0;
}

#define GRID_WIDTH 28

/// The grid the maze's path is laid on (GaiaRockInside_LayMaze).
typedef struct {
  PathCell cells[GRID_WIDTH * GRID_WIDTH];
} PathGrid;

/// Heap_AllocEwram, called through this inline as the ROM's call compiles.
static inline void *BumpAllocEwram(s32 size) { return Heap_AllocEwram(size); }

#define AT(x, y) grid->cells[(y) * GRID_WIDTH + (x)]
#define AT_ADDRESS(x, y)                                                       \
  ((PathCell *)(((y) * GRID_WIDTH + (x)) * 4 + (s32)grid))

/// Lays the maze's path of 14 steps from seed, once a visit, on a random walk
/// over a 28 by 28 grid, and records each step's ways out.
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
  if (GameFlag_Get(FLAG_AREA_TEMP_F0))
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
  border:
    a->step = 99;
    a += GRID_WIDTH;
    b->step = 99;
    b += GRID_WIDTH;
    e->step = 99;
    e++;
    t = o + GRID_WIDTH * GRID_WIDTH * 4;
    ((s8 *)grid)[t] = 99;
    o += sizeof(PathCell);
    step--;
    if (step >= 0)
      goto border;
  }
  for (step = 1; step <= 14;) {
    tries = 0;
  retry:
    if (++tries > 16)
      goto restart;
    nx = x;
    ny = y;
    dir = Random_Below(4);
    if (dir == 0 && (step == 1 || step == 13))
      goto retry;
    switch (dir) {
    case 0:
      if (AT(x, y + 1).step != 0 || AT(x, y + 2).step != 0 ||
          AT(x - 1, y + 1).step != 0 || AT(x + 1, y + 1).step != 0)
        goto retry;
      ny = y + 1;
      break;
    case 1:
      if (AT(x - 1, y).step != 0 || AT(x - 2, y).step != 0 ||
          AT(x - 1, y - 1).step != 0 || AT(x - 1, y + 1).step != 0)
        goto retry;
      nx = x - 1;
      break;
    case 2:
      if (AT(x + 1, y).step != 0 || AT(x + 2, y).step != 0 ||
          AT(x + 1, y - 1).step != 0 || AT(x + 1, y + 1).step != 0)
        goto retry;
      nx = x + 1;
      break;
    case 3:
      if (AT(x, y - 1).step != 0 || AT(x, y - 2).step != 0 ||
          AT(x - 1, y - 1).step != 0 || AT(x + 1, y - 1).step != 0)
        goto retry;
      ny = y - 1;
      break;
    }
    // The same cell variable the second loop reads its steps from.
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
  for (step = 1; step <= 14; step = next) {
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
      scanUp:
        p -= GRID_WIDTH;
        c = p->step;
        if (c == v - 1) {
          v = c;
          goto scanUp;
        }
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
      scanDown:
        p += GRID_WIDTH;
        c = p->step;
        if (c == v - 1) {
          v = c;
          goto scanDown;
        }
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
      scanRight:
        c = AT(++nx, y).step;
        if (c == v - 1) {
          v = c;
          goto scanRight;
        }
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
      scanLeft:
        c = AT(--nx, y).step;
        if (c == v - 1) {
          v = c;
          goto scanLeft;
        }
      }
      if (v == 0)
        v = 1;
    }
    steps[step].next[MAZE_EAST] = v;
    x = cell->nextX;
    steps[step].number = step;
    y = cell->nextY;
  }
  steps[14].next[MAZE_NORTH] = MAZE_EXIT;
  steps[1].next[MAZE_SOUTH] = MAZE_BACK;
  path->current = 1;
  Heap_Release(grid);
  Event_SetFlag(FLAG_AREA_TEMP_F0);
  gRandomSeed = saved;
}

/// Leaving a maze step by a way: out of the maze, or on to the step it leads
/// to (a dead end leads back to the first).
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

/// From the third state on, for each of the two map layers:
/// MapGrid_CopyRawCells with the state's origin plus the layer's cell, a 15 by
/// 10 size and the layer's cell plus one.
void GaiaRockInside_DrawMazeRoom(void) {
  s32 state = gPath->current;
  s8 *origin = gGaiaRockInside_MazeOrigins[state];
  struct MapWork *map = Ram_HeapSlots->mapGrid;

  if (state > 1) {
    s32 originX = origin[0];
    s32 originZ = origin[1];
    struct MapLayer *layer = map->layers;
    s32 i;

    for (i = 1; i >= 0; i--, layer++) {
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
void GaiaRockInside_SetMazeLayer(s32 layer, s32 keep) {
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

/// Each frame while the leader is moved by the keys alone: the collision code
/// of the wall cell he stands before, 1 to 230, is his step trigger.
void GaiaRockInside_TriggerWallCell(void) {
  if (gMainRoutineWork.movementMode == LEADER_MOVEMENT_KEYS) {
    struct FieldWork *event = Ram_HeapSlots->fieldWork;
    struct MapWork *map = Ram_HeapSlots->mapGrid;
    struct Actor *object = Actor_Get(gMainRoutineWork.leaderActorId);
    s32 x = object->x.fixed / toFIXED(16);
    s32 z = (object->z.fixed - object->y.fixed + 0x7fff) / toFIXED(16);
    struct MapCell *cells = map->layers[2].cells;
    struct MapCell *cell = &cells[x + (z << 7)];
    s32 code = cell->collisionCode;

    if (code >= 1 && code <= 230)
      event->stepTrigger = code;
  }
}

/// Stands a statue on the cell at (x, y, z), its wall cell blocked.
void GaiaRockInside_PlaceStatue(s32 id, s32 x, s32 y, s32 z) {
  struct Actor *object = Actor_Get(id);
  struct MapCell *cell = Ram_HeapSlots->mapGrid->layers[2].cells;
  s32 column = x / toFIXED(16);
  s32 row = (z - y + 0x7fff) / toFIXED(16);

  cell += column + (row << 7);
  cell->collisionCode = 255;
  object->x.fixed = (x & FIELD_CELL_MASK) + toFIXED(8);
  object->z.fixed = (z & FIELD_CELL_MASK) + toFIXED(8);
  object->y.fixed = object->z.fixed - FIELD_CELL_ORIGIN(row);
  object->motionFlags = 0;
  object->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
  Actor_SetSpriteFlags(object, 0);
}

extern s32 gGaiaRockInside_StatueRiseScript[];
extern u32 gGaiaRockInside_DustScript[];

/// A statue rises under the leader in a burst of rocks, lifting him with it;
/// he lands on top, and its flag is set.
void GaiaRockInside_StatueRisesUnderLeader(s32 id, s32 flag) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.leaderActorId);
  s32 gravity = leader->gravity;
  struct Actor *pillar;
  s32 i;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  pillar = Actor_Get(id);
  GaiaRockInside_PlaceStatue(id, leader->x.fixed, leader->y.fixed,
                             leader->z.fixed);
  Actor_StartScript(pillar, gGaiaRockInside_StatueRiseScript);
  i = 15;
  do {
    struct Actor *dust =
        FieldActor_Create(SPRITE_ROCK, leader->x.fixed,
                          leader->y.fixed + toFIXED(16), leader->z.fixed);

    if (dust != 0) {
      s32 angle;
      s32 speed;
      u32 size;
      s32 scale;

      Actor_StartScript(dust, gGaiaRockInside_DustScript);
      angle = Random16();
      speed = Random16();
      size = Random16();
      scale = (size >> 1) + FIXED_ONE / 8;
      dust->velocityY = Iwram_MulFX(speed * 8, Trig_Cos(angle));
      dust->velocityX = Iwram_MulFX(speed * 8, Math_Sin(angle));
      dust->acceleration = 0;
      dust->scaleX = scale;
      dust->scaleY = scale;
      dust->gravity = toFIXED(33) / 100;
      dust->bounce = FIXED_ONE / 2;
    }
  } while (--i >= 0);
  Event_PlaySound(SE_STATUE_RISE);
  Event_PlaySound(SE_FLOOR_SWITCH);
  MapGrid_SetShake(2, 2, 0.9);
  Actor_SetAttachedEffect(gMainRoutineWork.leaderActorId,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
  Actor_SelectAnimation(leader, 40);
  FieldEvent_Wait(40);
  Event_PlaySound(SE_DUST_PUFF);
  leader->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  leader->gravity = toFIXED(33) / 100;
  for (id = 0; id < 180; id++) {
    System_Wait(1);
    if (leader->y.fixed <= leader->groundHeight)
      break;
  }
  Event_PlaySound(SE_THUMP);
  leader->gravity = gravity;
  Actor_SetAttachedEffect(gMainRoutineWork.leaderActorId,
                          ATTACHED_EFFECT_REMOVE);
  Actor_SelectAnimation(leader, 38);
  MapGrid_SetShake(1, 4, 1);
  System_Wait(10);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  System_Wait(20);
  FieldCamera_MoveToFX(leader->x.fixed, leader->y.fixed, leader->z.fixed, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(20);
  Actor_SetSpriteFlags(leader, 1);
  Actor_SelectAnimation(leader, ANIM_STAND);
  leader->velocityY = toFIXED(4);
  System_Wait(20);
  gMainRoutineWork.movementMode = LEADER_MOVEMENT_WALK;
  Event_End();
  Event_SetFlag(flag);
}

/// Starts GaiaRockInside_TriggerWallCell each frame.
void GaiaRockInside_StartWallTriggers(void) {
  Task_AddFrame((s32)GaiaRockInside_TriggerWallCell, TASK_PASS_VBLANK);
}

#include "text/msg_ids.h"

void GaiaRockInside_CycloneClearsHallBushes(void);
void GaiaRockInside_ClearPlatformsBushes(void);

extern u32 gGaiaRockInside_IdleScript[];
extern u32 gGaiaRockInside_RockScript[];

// clang-format off
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

u32 gGaiaRockInside_SparkScript[] = {
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

s32 gGaiaRockInside_FallingSparkScript[] = {                                                               
      SC_CALL(Orbit_Update), SC_CALL(Actor_AimAtWatched), SC_CALL(Actor_StepToWatched), SC_RELEASE, SC_END,         
  };

u32 gGaiaRockInside_TabletRiseScript[] = {                                                               
      SC_MOVE_BY(0, 24, 0),                                            
      SC_SYNC,                                                                 
      SC_SET(SW_MOTION, 4),                                                    
      SC_END,                                                                  
  };

u32 gGaiaRockInside_FelixRiseScript[] = {                                                               
      SC_MOVE_BY(0, 16, 0),                                            
      SC_SYNC,                                                                 
      SC_SET(SW_MOTION, 4),                                                    
      SC_END,                                                                  
  };

s32 gGaiaRockInside_TabletSettleScript[] = {                                                               
      SC_SET(SW_MOTION, 0),                                                    
      SC_MOVE_BY(0, -24, 0),                                           
      SC_SYNC,                                                                 
      SC_SET(SW_MOTION, 0),                                                   
      SC_END,                                                                  
  };

s32 gGaiaRockInside_FelixSettleScript[] = {                                                               
      SC_SET(SW_MOTION, 0),                                                    
      SC_MOVE_BY(0, -16, 0),                                           
      SC_SYNC,                                                                 
      SC_SET(SW_MOTION, 3),                                                   
      SC_END,                                                                  
  };

s16 gGaiaRockInside_RockSpots[][2] = {{0, 0}, {113, 47}, {115, 47}, {115, 45}, {117, 45}, {119, 45}, {119, 47}, {121, 47}, {117, 49}, {119, 49}, {119, 51}, {121, 51}};

s16 gGaiaRockInside_PlatformPlaces[][2] = {{45, 51}, {31, 53}, {31, 57}, {39, 59}, {41, 59}, {43, 63}, {45, 63}, {35, 69}, {37, 69}};

u32 gGaiaRockInside_SerpentGlowScript[] = {
    SC_WAIT(1),
    SC_LOOP(44, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_RELEASE,
    SC_END,
};

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

u32 gGaiaRockInside_SavedCollision[] = {
    0x10043f42, 0x1006001e, 0x1008001e, 0x1006001e, 0xfe00001e, 0xffff,
};

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

u32 gGaiaRockInside_Regions[] = {
    0xffe202bc, 0x2d0014a, 0x15efff6, 0x8ffff, 0xffff, 0,
    0, 0,
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

s32 gGaiaRockInside_SusaWalksOffScript[] = {
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
    ACTOR_PLACEMENT(394, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136, 0, 104, FACING_SOUTH, TALK_FACING_KEEP, 0),
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

u32 gGaiaRockInside_RockScript[] = {
    SC_SET(SW_SCALE_X, 0xc000),
    SC_SET(SW_SCALE_Y, 0xc000),
    SC_WAIT(1),
    SC_END,
};

struct ActorPlacement gGaiaRockInside_FirstAltarActorPlacements[] = {
    ACTOR_PLACEMENT(388, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 408, 0, 688, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(388, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 584, 0, 688, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STUMP, CONDITION_ALWAYS, (s32)gGaiaRockInside_RockScript, 920, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STUMP, CONDITION_ALWAYS, (s32)gGaiaRockInside_RockScript, 952, 0, 408, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ALTAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 496, 0, 728, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(397, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gGaiaRockInside_TwinAltarsActorPlacements[] = {
    ACTOR_PLACEMENT(388, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 456, 0, 352, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(388, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 584, 0, 352, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ALTAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 528, 0, 384, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ALTAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 672, 0, 384, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(397, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gGaiaRockInside_PlatformsActorPlacements[] = {                                             
      {320, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},                
      {320, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},                
      {320, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},                
      {320, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},                
      {320, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},                
      {320, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},                  
      {320, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},                  
      {320, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},                  
      {320, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},                  
      {SCENE_TABLE_END},                                                       
  };

struct ActorPlacement gGaiaRockInside_AltarActorPlacements[] = {
    ACTOR_PLACEMENT(388, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 400, 0, 368, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ALTAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 336, 0, 408, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(372, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 680, 0, 648, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(378, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(378, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(378, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(397, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ALTAR, CONDITION_ALWAYS, (s32)gGaiaRockInside_IdleScript, 704, 0, 384, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_GANGWAY, CONDITION_ALWAYS, (s32)gGaiaRockInside_IdleScript, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

s32 gGaiaRockInside_SquashYScript[] = {
    SC_LABEL(1),
    SC_ADD(SW_SCALE_Y, 0x1800),
    SC_WAIT(1),
    SC_LOOP(4, SC_TO(1)),
    SC_LABEL(2),
    SC_ADD(SW_SCALE_Y, -0x1800),
    SC_WAIT(1),
    SC_LOOP(4, SC_TO(2)),
    SC_LABEL(3),
    SC_ADD(SW_SCALE_Y, 0x1000),
    SC_WAIT(1),
    SC_LOOP(2, SC_TO(3)),
    SC_LABEL(4),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 16),
    SC_WAIT(1),
    SC_LOOP(2, SC_TO(4)),
    SC_ADD(SW_SCALE_Y, 0x1000),
    SC_WAIT(1),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 16),
    SC_WAIT(1),
    SC_END,
};

s32 gGaiaRockInside_SquashXScript[] = {
    SC_LABEL(1),
    SC_ADD(SW_SCALE_X, 0x1800),
    SC_WAIT(1),
    SC_LOOP(4, SC_TO(1)),
    SC_LABEL(2),
    SC_ADD(SW_SCALE_X, -0x1800),
    SC_WAIT(1),
    SC_LOOP(4, SC_TO(2)),
    SC_LABEL(3),
    SC_ADD(SW_SCALE_X, 0x1000),
    SC_WAIT(1),
    SC_LOOP(2, SC_TO(3)),
    SC_LABEL(4),
    SC_ADD(SW_SCALE_X, -FIXED_ONE / 16),
    SC_WAIT(1),
    SC_LOOP(2, SC_TO(4)),
    SC_ADD(SW_SCALE_X, 0x1000),
    SC_WAIT(1),
    SC_ADD(SW_SCALE_X, -FIXED_ONE / 16),
    SC_WAIT(1),
    SC_END,
};

s32 gGaiaRockInside_ArrowFrame = 0;

struct Trigger gGaiaRockInside_Triggers[] = {
    {TRIGGER_STEP, 1, CONDITION_ALWAYS, EVENT(GaiaRockInside_TakeMazeExit)},
    {TRIGGER_STEP, 2, CONDITION_ALWAYS, EVENT(GaiaRockInside_TakeMazeExit)},
    {TRIGGER_STEP, 3, CONDITION_ALWAYS, EVENT(GaiaRockInside_TakeMazeExit)},
    {TRIGGER_STEP, 4, CONDITION_ALWAYS, EVENT(GaiaRockInside_TakeMazeExit)},
    {PSYNERGY_ON_ACTOR(ABILITY_GROWTH), ACTOR_ARROW_PLANT, FLAG_SCENE_TEMP_00, EVENT(GaiaRockInside_PlantPointsTheWay)},
    {TRIGGER_TABLE_END},
};

struct Trigger gGaiaRockInside_LairTriggers[] = {
    {TRIGGER_RAISED, RAISED_SUSA_BLOCKED, CONDITION_ALWAYS, EVENT(GaiaRockInside_SusaSaysMoveAside)},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_STEP, 40, FLAG_MET_SUSA, EVENT(GaiaRockInside_MeetSusa)},
    {TRIGGER_TALK, ACTOR_SUSA, CONDITION_ALWAYS, EVENT(GaiaRockInside_TalkToSusa)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SUSA, CONDITION_ALWAYS, EVENT(GaiaRockInside_ReadSusasMind)},
    {TRIGGER_TALK, ACTOR_SAND_TABLET, CONDITION_ALWAYS, EVENT(GaiaRockInside_ReadSandTablet)},
    {EXAMINE_TRIGGER(EXAMINE_GROUND), 50, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgGaiaRockInsideTheCupsAreOverflowingWith))},
    {TRIGGER_EXAMINE, 55, CONDITION_ALWAYS, EVENT(GaiaRockInside_ChallengeSerpent)},
    {TRIGGER_EXAMINE, 56, CONDITION_ALWAYS, EVENT(GaiaRockInside_ChallengeSerpent)},
    {EXAMINE_TRIGGER(EXAMINE_GROUND), 77, FLAG_CLOUD_BRAND_FOUND, EVENT(GaiaRockInside_FindCloudBrand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, EVENT(Leader_SetLayerByte)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(GaiaRockInside_EndSand)},
    {TRIGGER_TABLE_END},
};

struct Trigger gGaiaRockInside_SlainLairTriggers[] = {
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_STEP, 42, CONDITION_ALWAYS, EVENT(GaiaRockInside_RaiseSandTablet)},
    {TRIGGER_TALK, ACTOR_SAND_TABLET, CONDITION_ALWAYS, EVENT(GaiaRockInside_ReadSandTablet)},
    {EXAMINE_TRIGGER(EXAMINE_GROUND), 50, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgGaiaRockInsideTheCupsAreOverflowingWith))},
    {TRIGGER_EXAMINE, 55, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgGaiaRockInsideTheSerpentWithItsEyes))},
    {EXAMINE_TRIGGER(EXAMINE_GROUND), 77, FLAG_CLOUD_BRAND_FOUND, EVENT(GaiaRockInside_FindCloudBrand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, EVENT(Leader_SetLayerByte)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(GaiaRockInside_EndSand)},
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
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_ROCK_A, CONDITION_ALWAYS, EVENT(GaiaRockInside_RecordRockSpots)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_ROCK_B, CONDITION_ALWAYS, EVENT(GaiaRockInside_RecordRockSpots)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(GaiaRockInside_RecordRockSpots)},
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
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 20, CONDITION_ALWAYS, EVENT(GaiaRockInside_CycloneClearsHallBushes)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), -1, 0, 0},
};

struct Trigger gGaiaRockInside_PlatformsTriggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 20, CONDITION_ALWAYS, EVENT(GaiaRockInside_CycloneStartsPlatforms)},
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
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 43, CONDITION_ALWAYS, EVENT(GaiaRockInside_JumpDownByAltar)},
    {TRIGGER_STEP, 30, FLAG_STATUE_A_RISEN, EVENT(GaiaRockInside_StepOnFirstStatueSpot)},
    {TRIGGER_STEP, 31, FLAG_STATUE_B_RISEN, EVENT(GaiaRockInside_StepOnSecondStatueSpot)},
    {TRIGGER_STEP, 32, FLAG_STATUE_C_RISEN, EVENT(GaiaRockInside_StepOnThirdStatueSpot)},
    {TRIGGER_TALK, ACTOR_ALTAR, CONDITION_ALWAYS, EVENT(GaiaRockInside_CheckAltar)},
    {USE_ITEM_ON_ACTOR(ITEM_DANCING_IDOL), TRIGGER_SUBJECT_OWN_EFFECT | ACTOR_ALTAR, CONDITION_ALWAYS, EVENT(GaiaRockInside_PlaceDollOnAltarAltar)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_HANGING_VINE, CONDITION_ALWAYS, 0},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 40, CONDITION_ALWAYS, EVENT(GaiaRockInside_ClearPlatformsBushes)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 41, CONDITION_ALWAYS, EVENT(GaiaRockInside_ClearPlatformsSmallBushes)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 42, CONDITION_ALWAYS, EVENT(GaiaRockInside_CycloneClearsCobweb)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 43, CONDITION_ALWAYS, EVENT(GaiaRockInside_CycloneClearsCobweb)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 44, CONDITION_ALWAYS, EVENT(GaiaRockInside_CycloneMadPlant)},
    {TRIGGER_TABLE_END},
};

u32 gGaiaRockInside_DustScript[] = {
    0, 40, 0x80010000, 22, 25, 0,
    0, 1, 22, 25, 1, 0,
    1, 13, 10, 0xc0010000, 38,
};

s32 gGaiaRockInside_StatueRiseScript[] = {                                                               
      SC_ADD(SW_SCALE_X, 0x8000),                                              
      SC_ADD(SW_SCALE_Y, 0x8000),                                              
      SC_WAIT(2),                                                              
      SC_ADD(SW_SCALE_X, -0x4000),                                             
      SC_ADD(SW_SCALE_Y, -0x4000),                                             
      SC_WAIT(2),                                                              
      SC_ADD(SW_SCALE_X, -0x2000),                                             
      SC_ADD(SW_SCALE_Y, -0x2000),                                             
      SC_WAIT(2),                                                              
      SC_ADD(SW_SCALE_X, -0x2000),                                             
      SC_ADD(SW_SCALE_Y, -0x2000),                                             
      SC_WAIT(4),                                                              
      SC_ADD(SW_SCALE_X, 0x8000),                                              
      SC_ADD(SW_SCALE_Y, 0x8000),                                              
      SC_WAIT(2),                                                              
      SC_ADD(SW_SCALE_X, -0x4000),                                             
      SC_ADD(SW_SCALE_Y, -0x4000),                                             
      SC_WAIT(2),                                                              
      SC_ADD(SW_SCALE_X, -0x2000),                                             
      SC_ADD(SW_SCALE_Y, -0x2000),                                             
      SC_WAIT(2),                                                              
      SC_ADD(SW_SCALE_X, -0x2000),                                             
      SC_ADD(SW_SCALE_Y, -0x2000),                                             
      SC_WAIT(4),                                                              
      SC_ADD(SW_SCALE_X, 0x8000),                                              
      SC_ADD(SW_SCALE_Y, 0x8000),                                              
      SC_WAIT(2),                                                              
      SC_ADD(SW_SCALE_X, -0x4000),                                             
      SC_ADD(SW_SCALE_Y, -0x4000),                                             
      SC_WAIT(2),                                                              
      SC_ADD(SW_SCALE_X, -0x2000),                                             
      SC_ADD(SW_SCALE_Y, -0x2000),                                             
      SC_WAIT(2),                                                              
      SC_ADD(SW_SCALE_X, -0x2000),                                             
      SC_ADD(SW_SCALE_Y, -0x2000),                                             
      SC_WAIT(4),                                                              
      SC_WAIT(20),                                                             
      SC_LOOP(3, SC_TOP),                                                      
      SC_END,                                                                  
  };
// clang-format on
#undef EFFECT_SPAWN
#undef EFFECT_CREATE
#undef EFFECT_SET_MODE
#undef EFFECT_SET_SCRIPT
#undef EFFECT_SCRIPTS
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
