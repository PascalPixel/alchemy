/// @file
/// Yallam, the village near Taopo Swamp where Sunshine the blacksmith lives
/// (its houses, inn and shops are in yallam_rooms.c). East of the village is a
/// meadow of stumps ringed by worn paths round a big tree, where four children
/// play the game that goes with Yepp's songs: asked, the boy standing there
/// leads Felix and Jenna round the rings to the words of the songs of the
/// stars, the moon and the sun, which tell the way through the Sea of Time.
/// Cyclone scatters weeds round the village: the pair south of the Sanctum's
/// door hide an Antidote and the three in the south-east corner a Nut. Once
/// the party has Force, the log standing beside the cave mouth near Sunshine's
/// house can be tipped into the gap below it, and Cyclone on the weed patch in
/// the north-west corner opens a hole down into the cave that leads to the
/// Masamune's chest.
#include "fixed_math.h"
#include "flags.h"
#include "macros.h"
#include "sprite_ids.h"
#include "types.h"
#include "version.h"
#include "battle/battle.h"
#include "field/actor.h"
#include "field/actor_script.h"
#include "field/event_inline.h"
#include "field/field.h"
#include "field/field_sprite.h"
#include "field/field_sequence_event.h"
#include "field/scene_tables.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "menu/menu.h"
#include "scene/event_layer.h"
#include "scene/scene.h"
#include "sound/sound.h"
#include "system/main_routine_work.h"
#include "system/math_table.h"
#include "system/ram_buffer.h"
#include "system/system.h"
#include "system/task.h"
#include "window/window.h"
#include "text/msg_ids.h"

#include "shared/tables.h"

/// Set once Force has pushed the log off its ledge, where it now lies as a
/// step.
#define FLAG_YALLAM_LOG_FALLEN FLAG_AREA_TEMP_00

/// Yallam's actors, numbering the rows of gYallam_Placements.
enum YallamActorId {
  /// The tree trunk on the ledge: its broken stump, which stays, and the log
  /// standing on it, which Force pushes off (Yallam_ForceLogDown).
  ACTOR_LOG_STUMP = ACTOR_FIRST_PLACED,
  ACTOR_FALLING_LOG,
  /// Unused: a standing log, placed at the map's corner out of sight, where
  /// nothing moves it.
  ACTOR_STANDING_LOG,
  /// "Yallam doesn't get many visitors. Hasn't for a long time."
  ACTOR_VILLAGER_FEW_VISITORS,
  /// "Our master boatsman, Yepp, died."
  ACTOR_VILLAGER_YEPP_DIED,
  /// "If Yepp were alive today, our village would be thriving."
  ACTOR_VILLAGER_IF_YEPP_LIVED,
  /// "Yepp found this huge bed of pearls and sold them in another town."
  ACTOR_VILLAGER_PEARL_BED,
  /// "There used to be a lot of pearl divers here in Yallam."
  ACTOR_VILLAGER_PEARL_DIVERS,
  /// "When we lost Yepp, all of Yallam practically fell to pieces."
  ACTOR_VILLAGER_LOST_YEPP,
  /// "Yepp was the one who taught me how to handle a boat."
  ACTOR_BOATSMAN_TAUGHT_BY_YEPP,
  /// "Yepp told me the only thing he could teach me was how..."
  ACTOR_BOATSMAN_TAUGHT_LITTLE
};

/// The children's meadow's actors, numbering the rows of
/// gYallam_SongGamePlacements: the four children who play the song game.
enum YallamSongGameActorId {
  /// "Yepp's songs are a lot of fun, because he made this game..."; he offers
  /// the song (Yallam_OfferYeppSong) and leads the game.
  ACTOR_SONG_LEADER = ACTOR_FIRST_PLACED,
  /// "Running around in circles here while singing one of..."
  ACTOR_CIRCLES_CHILD,
  /// "There's a game that goes along with the song, but it's tough."
  ACTOR_GAME_CHILD,
  /// "Yepp wrote all of his children's songs while he was out here..."
  ACTOR_STORY_CHILD
};

/// Yallam's hidden treasure, the rows of its item table.
enum YallamTreasureActorId {
  ACTOR_HIDDEN_NUT = FIELD_TREASURE_ACTOR,
  ACTOR_HIDDEN_COINS,
  ACTOR_HIDDEN_ANTIDOTE
};

/// How near in front of it, in pixels, a running child stops for the other
/// child or for the leader.
#define STOP_RANGE 24
/// From patch 1, a running child walks on while the leader is this near, in
/// pixels.
#define LEADER_WALK_ON_RANGE 11

/// The running children keep these flags in Actor.timer.
enum {
  /// Set on the circles child, who runs after ACTOR_GAME_CHILD.
  CHILD_CHASES_GAME_CHILD = 1,
  /// Stops the child for the leader, whatever its heading, while a talk or
  /// Mind Read is open; nothing here sets it, since a talk stops a child by its
  /// riseEnabled.
  CHILD_TALKING = 2
};

/// The map cells Cyclone answers on, by the behaviour the map gives them:
/// each set of weeds it scatters.
enum YallamCell {
  CELL_WEEDS_OVER_CAVE_HOLE = 20,
  CELL_WEEDS_BELOW_SHOPS = 21,
  CELL_WEEDS_OVER_ANTIDOTE = 22,
  CELL_WEEDS_IN_CORNER = 23,
  CELL_WEEDS_OVER_NUT = 24,
  CELL_WEEDS_BY_SANCTUM = 25
};

/// Where the Antidote and the Nut lie once Cyclone has scattered the weeds
/// over them, in pixels.
#define ANTIDOTE_SPOT_X 424
#define ANTIDOTE_SPOT_Z 232
#define NUT_SPOT_X 680
#define NUT_SPOT_Z 344

/// The falling log's spin, in angle units a frame: it gains this much each
/// frame, from rest as it tips off the ledge and from TOPPLE_ROLL_SPIN as it
/// rolls down.
#define TOPPLE_SPIN_GAIN 40
#define TOPPLE_ROLL_SPIN 400

void Yallam_SingMoonSong(void);

void Yallam_SingSunSong(void);
void Yallam_StartRunningChildren(void);

void Yallam_UncoverCaveHole(void);

void Yallam_ScatterWeedsBelowShops(void);

void Yallam_ScatterWeedsInCorner(void);

void Yallam_ScatterWeedsBySanctum(void);

void Yallam_OfferYeppSong(void);

void Yallam_TalkAboutSongGame(void);

void Yallam_ForceLogDown(void);

void Yallam_DoNothing(void);

extern s32 gYallam_LogRaised[];
extern s32 gYallam_StumpLowered[];
extern u32 gYallam_CirclesChildRuns[];
extern u32 gYallam_GameChildRuns[];
extern u16 gYallam_DoorOpening[];
extern u16 gYallam_SecondDoorOpening[];
extern u16 gYallam_ThirdDoorOpening[];
extern u16 gYallam_SanctumDoorOpening[];

/// Unused: turns an actor's sprite half a turn, as an actor script's call.
UNUSED s32 Yallam_TurnSpriteAround(struct Actor *actor) {
  struct FieldSprite *sprite = actor->sprite;

  sprite->rotation = DEGtoANG(180);
  return 0;
}

/// Takes the shadow off a log's part, as its actor script's call, once the
/// script has set it in place.
s32 Yallam_HideShadow(struct Actor *actor) {
  Actor_SetSpriteFlags(actor, 0);
  return 0;
}

#include "shared/distance.c"

void Yallam_RunSongPath(s32 id);
void Yallam_ShowSongGame(void);

/// A song game runner's Actor fields of the same names, kept while the game is
/// paused for a talk.
struct SongRunnerState {
  s32 timer;
  s32 userValue;
  s32 riseCounter;
  s32 riseEnabled;
};

/// The song leader's userValue in the song game: a command to every runner,
/// which Yallam_UpdateSongGame passes on and clears.
enum { SONG_GAME_GO = 1, SONG_GAME_HALT = 2, SONG_GAME_HALT_AND_STAND = 3 };

/// A runner's riseCounter in the song game: where it stands on its path.
enum {
  RUNNER_GOING = 0,            ///< walks on along its path
  RUNNER_HALTED = 1,           ///< waits where it stopped
  RUNNER_HALTING = 2,          ///< stops where it is this frame
  RUNNER_HALTING_TO_STAND = 3, ///< stops and stands this frame
};

/// The song game's five runners, kept while it is paused.
struct SongRunnerState gYallam_SavedSongGame[5] BSS_DATA ALIGNED(16);
extern struct MapGridCellAnimation gYallam_Doors[];

s32 SceneActor_CalculateFixedPointDistance(s32 *a, s32 *b);
s32 Yallam_StopIfFacingNear(struct Actor *actor, struct Actor *target,
                            s32 range, s32 force);
void Yallam_UpdateSongGame(void);

/// Stops the actor (paused, standing) when the target is nearer than range
/// pixels and lies ahead of it, within a sixteenth of a turn either side of its
/// heading; force stops it regardless, and so does a talk with it (its
/// riseEnabled set). Returns 1 when it stopped. A target out of range sets it
/// walking again.
s32 Yallam_StopIfFacingNear(struct Actor *actor, struct Actor *target,
                            s32 range, s32 force) {
  s32 result = 0;
  s32 *targetPos;
  s32 *actorPos;

  if (actor->riseEnabled != 0) {
    actor->paused = 1;
    return 1;
  }
  targetPos = &target->x.fixed;
  actorPos = &actor->x.fixed;
  if (SceneActor_CalculateFixedPointDistance(targetPos, actorPos) < range ||
      force != 0) {
    u32 angle = (u16)Trig_Atan(target->z.fixed - actor->z.fixed,
                               *targetPos - *actorPos);
    u32 left = FACING_SIXTEENTH(angle - FACING_STEP);
    u32 right = FACING_SIXTEENTH(angle + FACING_STEP);
    u32 ahead = FACING_SIXTEENTH(angle);
    u32 facing = FACING_SIXTEENTH(actor->facing);

    if (ahead == facing || right == facing || left == facing || force != 0) {
      actor->paused = 1;
      Actor_SelectAnimation(actor, ANIM_STAND);
      result = 1;
    }
  } else {
    actor->paused = 0;
    Actor_SelectAnimation(actor, ANIM_WALK);
  }
  return result;
}

#if TLA_PATCH_1
/// The update of the two children running their rings. Each stops when it runs
/// up behind the other, and otherwise when the leader is in its way; from patch
/// 1 it walks on while the leader is right beside it.
s32 Yallam_UpdateRunningChild(struct Actor *actor) {
  struct WindowWork *work = (struct WindowWork *)Ram_HeapSlots->windowWork;
  struct FieldWork *event = Ram_HeapSlots->fieldWork;
  struct Actor *leader;
  struct Actor *target;
  u16 *flags;
  s32 force = 0;

  leader = Actor_Get(gMainRoutineWork.leaderActorId);

  if (SceneActor_CalculateFixedPointDistance(
          &leader->x.fixed, &actor->x.fixed) <= LEADER_WALK_ON_RANGE) {
    actor->paused = 0;
    Actor_SelectAnimation(actor, ANIM_WALK);
    return 0;
  }
  flags = (u16 *)&actor->timer;
  if (*flags & CHILD_CHASES_GAME_CHILD)
    target = Actor_Get(ACTOR_GAME_CHILD);
  else
    target = Actor_Get(ACTOR_CIRCLES_CHILD);
  if (Yallam_StopIfFacingNear(actor, target, STOP_RANGE, 0) == 0) {
    leader = Actor_Get(gMainRoutineWork.leaderActorId);
    if ((event->talk != 0 || work->mindRead != 0) && (*flags & CHILD_TALKING))
      force = 1;
    Yallam_StopIfFacingNear(actor, leader, STOP_RANGE, force);
  }
  return 0;
}
#else
s32 Yallam_UpdateRunningChild(struct Actor *actor) {
  struct WindowWork *work = (struct WindowWork *)Ram_HeapSlots->windowWork;
  struct FieldWork *event = Ram_HeapSlots->fieldWork;
  s32 force = 0;
  struct Actor *target;
  u16 *flags = (u16 *)&actor->timer;

  if (*flags & CHILD_CHASES_GAME_CHILD)
    target = Actor_Get(ACTOR_GAME_CHILD);
  else
    target = Actor_Get(ACTOR_CIRCLES_CHILD);
  if (Yallam_StopIfFacingNear(actor, target, STOP_RANGE, 0) == 0) {
    struct Actor *leader = Actor_Get(gMainRoutineWork.leaderActorId);

    if ((event->talk != 0 || work->mindRead != 0) && (*flags & CHILD_TALKING))
      force = 1;
    Yallam_StopIfFacingNear(actor, leader, STOP_RANGE, force);
  }
  return 0;
}
#endif

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

/// The children's meadow, where the song game is played: the path off the
/// village's east edge (cells (45, 15) to (45, 17)) leads there, and its own
/// way out back to the village.
enum {
  YALLAM_TO_SONG_GAME = 12,
  YALLAM_FROM_SONG_GAME = 12,
  YALLAM_SONG_GAME = 15,
  YALLAM_EXIT_SONG_GAME = 15
};

extern struct ActorPlacement gYallam_Placements[];
extern struct ActorPlacement gYallam_CavePlacements[];
extern struct ActorPlacement gYallam_SongGamePlacements[];

const u8 *Scene_GetActorPlacements(void) {
  if (gMainRoutineWork.next == SCENE_YALLAM &&
      gMainRoutineWork.entrance == YALLAM_SONG_GAME)
    return gYallam_SongGamePlacements;
  if (gMainRoutineWork.next == SCENE_YALLAM_CAVE)
    return gYallam_CavePlacements;
  return gYallam_Placements;
}

/// Walking north into one of Yallam's doors opens it with a click and takes
/// Felix inside: the inn's (cell (35, 11)), Sunshine's by the log (cell
/// (15, 11)), the two other huts' (cells (34, 21) and (28, 23)), the three
/// shops' in the ledge (cells (20, 20), (16, 19) and (13, 20)) or the
/// Sanctum's wide door in the cliff (cells (26, 8) and (27, 8)).
/// Each door's cells carry its exit's number, which also picks its picture
/// as it opens from `gYallam_Doors`.
void Yallam_EnterDoor(void) {
  struct FieldWork *event = Ram_HeapSlots->fieldWork;
  u32 i;
  s32 index;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  for (i = ACTOR_FIRST_PLACED; i < FIELD_TREASURE_ACTOR; i++) {
    struct Actor *actor = Actor_Get(i);

    if (actor != NULL)
      actor->motionFlags = 0;
  }
  index = event->stepTrigger;
  Event_PlaySound(SE_DOOR_OPEN);
  index--;
  MapGrid_AnimateCells(gYallam_Doors[index].steps, gYallam_Doors[index].x,
                       gYallam_Doors[index].y);
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_WALK);
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 0.8);
  {
    s32 owner = gMainRoutineWork.leaderActorId;

    // The sanctum's door is two cells wide: the leader walks straight in.
    if (index == YALLAM_TO_MIKASALLA_ROOMS_SANCTUM - 1)
      Actor_SetDestinationOffset(owner, 0, -4);
    else
      Actor_CenterAndWalk(owner, 2, -4);
  }
  FieldEvent_Wait(10);
  FieldEvent_SetExitRequest(event->stepTrigger);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_End();
}

/// Cyclone cast in the weed patch in the village's north-west corner (cells
/// (3, 7) to (7, 11)) scatters the weeds and uncovers a wood-rimmed hole in
/// the middle of the patch, the way down into the cave that leads to the
/// Masamune's chest.
/// The hole's picture and its exit to the cave (`YALLAM_TO_YALLAM_CAVE_WEST`)
/// are copied from a spare cell below the village.
void Yallam_UncoverCaveHole(void) {
  TileCopies_CopyTouchedCells(67, 25, 5, 5, 67, 5, 1);
  MapGrid_CopyCellsTo(23, 29, 5, 6, 1, 1);
  MapGrid_CopyCellAttributes(23, 29, 1, 1, 5, 8);
}

/// Cyclone cast by the weeds on the ledge below the shops' doors (cells
/// (15, 22) and (16, 22)) scatters them; nothing lies under them.
/// The eastern of the two weeds south of the Sanctum's door (cell (27, 14) of
/// the raised ground's layer) also runs this. Cast there, Cyclone still
/// scatters that pair and shows the Antidote under them
/// (Yallam_UncoverAntidote).
/// Only the cells Cyclone's whirl touched are cleared, and only those inside
/// the two cells below the shops, so a cast at the Sanctum's pair clears
/// nothing here: the weeds below the shops stay until Cyclone is cast beside
/// them.
void Yallam_ScatterWeedsBelowShops(void) {
  FieldTileCopies_CopyTouchedCells(67, 25, 2, 1, 79, 22, 1);
}

/// Cyclone cast at the pair of weeds on the raised ground south of the
/// Sanctum's door (cell (26, 14)) scatters both and uncovers the Antidote
/// that lies under them, to be picked up there.
/// Until the Antidote is taken, its hidden item (`ACTOR_HIDDEN_ANTIDOTE`) is
/// set at the spot, and if Felix is standing on it he hops a cell east, off it.
void Yallam_UncoverAntidote(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.leaderActorId);

  FieldTileCopies_CopyTouchedCells(26, 95, 2, 1, 26, 76, 2);
  if (Event_GetFlag(FLAG_YALLAM_ANTIDOTE) == 0) {
    Actor_SetPosition(ACTOR_HIDDEN_ANTIDOTE, ANTIDOTE_SPOT_X, ANTIDOTE_SPOT_Z);
    if (FIELD_CELL(leader->x.fixed) == FIELD_CELL(toFIXED(ANTIDOTE_SPOT_X)) &&
        FIELD_CELL(leader->z.fixed) == FIELD_CELL(toFIXED(ANTIDOTE_SPOT_Z))) {
      Event_Start();
      FieldEvent_ResolvePendingActions(0);
      Actor_SetAttachedEffect(gMainRoutineWork.leaderActorId,
                              ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
      Actor_SetDestinationOffset(gMainRoutineWork.leaderActorId, 16, 0);
      leader->velocityY = toFIXED(6);
      FieldActor_WaitForMoveById(gMainRoutineWork.leaderActorId);
      Event_End();
    }
  }
}

/// Cyclone cast at the three weeds in the village's south-east corner (cells
/// (44, 22), (43, 23) and (44, 23)) scatters them all; the Nut lies under the
/// one at cell (42, 21) (Yallam_UncoverNut).
void Yallam_ScatterWeedsInCorner(void) {
  FieldTileCopies_CopyTouchedCells(67, 25, 3, 3, 106, 20, 1);
}

/// Cyclone cast at the westernmost of the three weeds in the village's
/// south-east corner (cell (42, 21)) scatters them all and uncovers the Nut
/// under it, to be picked up there.
/// Until the Nut is taken, its hidden item is set at the spot, and if Felix is
/// standing on it he hops a cell north, off it.
void Yallam_UncoverNut(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.leaderActorId);

  FieldTileCopies_CopyTouchedCells(67, 25, 3, 3, 106, 20, 1);
  if (Event_GetFlag(FLAG_YALLAM_NUT) == 0) {
    Actor_SetPosition(ACTOR_HIDDEN_NUT, NUT_SPOT_X, NUT_SPOT_Z);
    if (FIELD_CELL(leader->x.fixed) == FIELD_CELL(toFIXED(NUT_SPOT_X)) &&
        FIELD_CELL(leader->z.fixed) == FIELD_CELL(toFIXED(NUT_SPOT_Z))) {
      Event_Start();
      FieldEvent_ResolvePendingActions(0);
      Actor_SetAttachedEffect(gMainRoutineWork.leaderActorId,
                              ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
      Actor_SetDestinationOffset(gMainRoutineWork.leaderActorId, 0, -16);
      leader->velocityY = toFIXED(6);
      FieldActor_WaitForMoveById(gMainRoutineWork.leaderActorId);
      Event_End();
    }
  }
}

/// Cyclone cast at the two weeds just west of the Sanctum's door (cells
/// (24, 8) and (23, 9)) scatters them; nothing lies under them.
void Yallam_ScatterWeedsBySanctum(void) {
  FieldTileCopies_CopyTouchedCells(67, 25, 2, 2, 87, 8, 1);
}

extern struct Trigger gYallam_Triggers[];
extern struct Trigger gYallam_CaveTriggers[];
extern struct Trigger gYallam_SongGameTriggers[];

const struct Trigger *Scene_GetTriggers(void) {
  if (gMainRoutineWork.next == SCENE_YALLAM &&
      gMainRoutineWork.entrance == YALLAM_SONG_GAME)
    return gYallam_SongGameTriggers;
  if (gMainRoutineWork.next == SCENE_YALLAM_CAVE)
    return gYallam_CaveTriggers;
  return gYallam_Triggers;
}

/// The boy standing in the children's meadow asks "Do you want to hear one of
/// the songs Yepp wrote?". A yes starts the song game (Yallam_ShowSongGame); a
/// no, and he says this is where all the kids in Yallam come to play.
/// Felix talks to him facing any way but west, south-west or north-west.
void Yallam_OfferYeppSong(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId(MsgDoYouWant);
  // "Do you want to hear one of the songs Yepp wrote?"
  Event_OpenMessage(ACTOR_SONG_LEADER, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(0, 0) == 0)
    Yallam_ShowSongGame();
  else
    Event_ShowMessage(ACTOR_SONG_LEADER, 0);
  Event_End();
}

/// The boy running round the big tree in the children's meadow stops,
/// turns to Felix and says running round in circles while singing one of
/// Yepp's songs is pretty tough, then runs on; Mind Read hears that there is a
/// lot to remember for each song.
/// The boy keeps his heading through the talk, so he runs on the way he was
/// going; Mind Read stops him without turning him.
void Yallam_TalkAboutSongCircles(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  struct Actor *actor = Actor_Get(ACTOR_CIRCLES_CHILD);
  s16 angle = actor->facing;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  actor->riseEnabled = 1;
  if (work->psynergy == 0) {
    Actor_FaceActor(ACTOR_CIRCLES_CHILD, gMainRoutineWork.leaderActorId, 0);
    // "Running around in circles here while singing one of Yepp's songs is
    // pretty tough."
    FieldEvent_SetMessageId(MsgRunningAroundIn);
  } else {
    // "There's a lot of stuff to remember for each song. It's tough to play
    // without making any mistakes."
    FieldEvent_SetMessageId(MsgThereSALot);
  }
  Event_ShowMessage(ACTOR_CIRCLES_CHILD, 0);
  if (work->psynergy == 0)
    actor->facing = angle;
  actor->riseEnabled = 0;
  Event_End();
}

/// The girl with a topknot running round the big tree in the children's meadow
/// stops, turns to Felix and says the game that goes with the song is tough and
/// she wants to play something else, then runs on; Mind Read hears she once
/// heard that children's songs and stories have hidden meanings.
void Yallam_TalkAboutSongGame(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  struct Actor *actor = Actor_Get(ACTOR_GAME_CHILD);
  s16 angle = actor->facing;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  actor->riseEnabled = 1;
  if (work->psynergy == 0) {
    Actor_FaceActor(ACTOR_GAME_CHILD, gMainRoutineWork.leaderActorId, 0);
    // "There's a game that goes along with the song, but it's tough. I want to
    // play something else."
    FieldEvent_SetMessageId(MsgThereSAGame);
  } else {
    // "I once heard that children's songs and stories have hidden meanings."
    FieldEvent_SetMessageId(MsgIOnceHeard);
  }
  Event_ShowMessage(ACTOR_GAME_CHILD, 0);
  if (work->psynergy == 0)
    actor->facing = angle;
  actor->riseEnabled = 0;
  Event_End();
}

/// Whether an actor has reached its target, which is then unset.
s32 Yallam_HasArrived(struct Actor *actor) {
  s32 x = actor->targetX;

  if (x == ACTOR_NO_TARGET && actor->targetZ == ACTOR_NO_TARGET)
    return 1;
  return 0;
}

/// One step of a runner's path round the meadow's rings in the song game, by
/// its step counter, stopping or going on by the game's mode.
void Yallam_RunSongPath(s32 actorId) {
  struct Actor *actor = Actor_GetById(actorId);
  u8 s = actor->riseCounter;
  s16 *timer;

  if (s == RUNNER_HALTED)
    return;
  timer = &actor->timer;
  switch (s) {
  case RUNNER_GOING:
    Actor_SelectAnimation(actor, ANIM_WALK);
    break;
  case RUNNER_HALTED:
    break;
  case RUNNER_HALTING_TO_STAND:
    Actor_SelectAnimation(actor, ANIM_STAND);
  case RUNNER_HALTING:
    Actor_SetMoveTarget(actor, actor->x.fixed, actor->y.fixed, actor->z.fixed);
    *timer &= ~1;
    actor->riseCounter = RUNNER_HALTED;
    return;
  }
  switch (*timer) {
  case 0:
    /* FAKEMATCH(barrier): an empty asm that clobbers r5; as no asm. */
    asm("" : : : "r5");
    Actor_SetDestination(actorId, 344, 824);
    (*timer)++;
    return;
  case 2:
    Actor_SetDestination(actorId, 324, 820);
    (*timer)++;
    return;
  case 4:
    Actor_SetDestination(actorId, 320, 808);
    (*timer)++;
    return;
  case 6:
    Actor_SetDestination(actorId, 324, 796);
    (*timer)++;
    return;
  case 8:
    Actor_SetDestination(actorId, 344, 792);
    (*timer)++;
    return;
  case 10:
    Actor_SetDestination(actorId, 364, 796);
    (*timer)++;
    return;
  case 12:
    Actor_SetDestination(actorId, 368, 808);
    (*timer)++;
    return;
  case 14:
    Actor_SetDestination(actorId, 364, 820);
    (*timer)++;
    return;
  case 16:
    Actor_SetDestination(actorId, 344, 824);
    (*timer)++;
    return;
  case 18:
    Actor_SetDestination(actorId, 324, 820);
    (*timer)++;
    return;
  case 20:
    Actor_SetDestination(actorId, 320, 808);
    (*timer)++;
    return;
  case 22:
    Actor_SetDestination(actorId, 324, 796);
    (*timer)++;
    return;
  case 24:
    Actor_SetDestination(actorId, 344, 792);
    (*timer)++;
    return;
  case 26:
    Actor_SetDestination(actorId, 364, 796);
    (*timer)++;
    return;
  case 28:
    Actor_SetDestination(actorId, 368, 808);
    (*timer)++;
    return;
  case 30:
    Actor_SetDestination(actorId, 364, 820);
    (*timer)++;
    return;
  case 32:
    Actor_SetDestination(actorId, 344, 824);
    (*timer)++;
    return;
  case 34:
    Actor_SetSpeedAndAcceleration(actorId, 1.8, 1.8);
    Actor_SetDestination(actorId, 364, 820);
    (*timer)++;
    return;
  case 36:
    Actor_SetDestination(actorId, 368, 808);
    (*timer)++;
    return;
  case 38:
    Actor_SetDestination(actorId, 392, 808);
    (*timer)++;
    return;
  case 40:
    Actor_SetDestination(actorId, 392, 744);
    (*timer)++;
    return;
  case 42:
    Actor_SetSpeedAndAcceleration(actorId, 1.2, 1.2);
    Actor_SetDestination(actorId, 372, 740);
    (*timer)++;
    return;
  case 44:
    Actor_SetDestination(actorId, 368, 728);
    (*timer)++;
    return;
  case 46:
    Actor_SetDestination(actorId, 372, 716);
    (*timer)++;
    return;
  case 48:
    Actor_SetDestination(actorId, 392, 712);
    (*timer)++;
    return;
  case 50:
    Actor_SetDestination(actorId, 412, 716);
    (*timer)++;
    return;
  case 52:
    Actor_SetDestination(actorId, 416, 728);
    (*timer)++;
    return;
  case 54:
    Actor_SetDestination(actorId, 412, 740);
    (*timer)++;
    return;
  case 56:
    Actor_SetDestination(actorId, 392, 744);
    (*timer)++;
    return;
  case 58:
    Actor_SetDestination(actorId, 372, 740);
    (*timer)++;
    return;
  case 60:
    Actor_SetDestination(actorId, 368, 728);
    (*timer)++;
    return;
  case 62:
    Actor_SetDestination(actorId, 372, 716);
    (*timer)++;
    return;
  case 64:
    Actor_SetDestination(actorId, 392, 712);
    (*timer)++;
    return;
  case 66:
    Actor_SetDestination(actorId, 412, 716);
    (*timer)++;
    return;
  case 68:
    Actor_SetDestination(actorId, 416, 728);
    (*timer)++;
    return;
  case 70:
    Actor_SetDestination(actorId, 412, 740);
    (*timer)++;
    return;
  case 72:
    Actor_SetDestination(actorId, 392, 744);
    (*timer)++;
    return;
  case 74:
    Actor_SetDestination(actorId, 372, 740);
    (*timer)++;
    return;
  case 76:
    Actor_SetDestination(actorId, 368, 728);
    (*timer)++;
    return;
  case 78:
    Actor_SetDestination(actorId, 372, 716);
    (*timer)++;
    return;
  case 80:
    Actor_SetDestination(actorId, 392, 712);
    (*timer)++;
    return;
  case 82:
    Actor_SetDestination(actorId, 412, 716);
    (*timer)++;
    return;
  case 84:
    Actor_SetDestination(actorId, 416, 728);
    (*timer)++;
    return;
  case 86:
    Actor_SetDestination(actorId, 412, 740);
    (*timer)++;
    return;
  case 88:
    Actor_SetDestination(actorId, 392, 744);
    (*timer)++;
    return;
  case 90:
    Actor_SetDestination(actorId, 372, 740);
    (*timer)++;
    return;
  case 92:
    Actor_SetDestination(actorId, 368, 728);
    (*timer)++;
    return;
  case 94:
    Actor_SetDestination(actorId, 372, 716);
    (*timer)++;
    return;
  case 96:
    Actor_SetDestination(actorId, 392, 712);
    (*timer)++;
    return;
  case 98:
    Actor_SetDestination(actorId, 392, 696);
    (*timer)++;
    return;
  case 100:
    Actor_SetDestination(actorId, 412, 692);
    (*timer)++;
    return;
  case 102:
    Actor_SetDestination(actorId, 416, 680);
    (*timer)++;
    return;
  case 104:
    Actor_SetDestination(actorId, 412, 668);
    (*timer)++;
    return;
  case 106:
    Actor_SetDestination(actorId, 392, 664);
    (*timer)++;
    return;
  case 108:
    Actor_SetDestination(actorId, 372, 668);
    (*timer)++;
    return;
  case 110:
    Actor_SetDestination(actorId, 368, 680);
    (*timer)++;
    return;
  case 112:
    Actor_SetDestination(actorId, 320, 680);
    (*timer)++;
    return;
  case 114:
    Actor_SetDestination(actorId, 316, 692);
    (*timer)++;
    return;
  case 116:
    Actor_SetDestination(actorId, 296, 696);
    (*timer)++;
    return;
  case 118:
    Actor_SetDestination(actorId, 276, 692);
    (*timer)++;
    return;
  case 120:
    Actor_SetDestination(actorId, 272, 680);
    (*timer)++;
    return;
  case 122:
    Actor_SetDestination(actorId, 276, 668);
    (*timer)++;
    return;
  case 124:
    Actor_SetDestination(actorId, 296, 664);
    (*timer)++;
    return;
  case 126:
    Actor_SetDestination(actorId, 316, 668);
    (*timer)++;
    return;
  case 128:
    Actor_SetDestination(actorId, 320, 680);
    (*timer)++;
    return;
  case 130:
    Actor_SetDestination(actorId, 316, 692);
    (*timer)++;
    return;
  case 132:
    Actor_SetDestination(actorId, 296, 696);
    (*timer)++;
    return;
  case 134:
    Actor_SetDestination(actorId, 276, 692);
    (*timer)++;
    return;
  case 136:
    Actor_SetDestination(actorId, 272, 680);
    (*timer)++;
    return;
  case 138:
    Actor_SetDestination(actorId, 276, 668);
    (*timer)++;
    return;
  case 140:
    Actor_SetDestination(actorId, 296, 664);
    (*timer)++;
    return;
  case 142:
    Actor_SetDestination(actorId, 316, 668);
    (*timer)++;
    return;
  case 144:
    Actor_SetDestination(actorId, 320, 680);
    (*timer)++;
    return;
  case 146:
    Actor_SetDestination(actorId, 316, 692);
    (*timer)++;
    return;
  case 148:
    Actor_SetDestination(actorId, 296, 696);
    (*timer)++;
    return;
  case 150:
    Actor_SetDestination(actorId, 296, 728);
    (*timer)++;
    return;
  case 152:
    Actor_SetDestination(actorId, 272, 728);
    (*timer)++;
    return;
  case 154:
    Actor_SetDestination(actorId, 268, 740);
    (*timer)++;
    return;
  case 156:
    Actor_SetDestination(actorId, 248, 744);
    (*timer)++;
    return;
  case 158:
    Actor_SetDestination(actorId, 228, 740);
    (*timer)++;
    return;
  case 160:
    Actor_SetDestination(actorId, 224, 728);
    (*timer)++;
    return;
  case 162:
    Actor_SetDestination(actorId, 228, 716);
    (*timer)++;
    return;
  case 164:
    Actor_SetDestination(actorId, 248, 712);
    (*timer)++;
    return;
  case 166:
    Actor_SetDestination(actorId, 268, 716);
    (*timer)++;
    return;
  case 168:
    Actor_SetDestination(actorId, 272, 728);
    (*timer)++;
    return;
  case 170:
    Actor_SetDestination(actorId, 268, 740);
    (*timer)++;
    return;
  case 172:
    Actor_SetDestination(actorId, 248, 744);
    (*timer)++;
    return;
  case 174:
    Actor_SetDestination(actorId, 228, 740);
    (*timer)++;
    return;
  case 176:
    Actor_SetDestination(actorId, 224, 728);
    (*timer)++;
    return;
  case 178:
    Actor_SetDestination(actorId, 228, 716);
    (*timer)++;
    return;
  case 180:
    Actor_SetDestination(actorId, 248, 712);
    (*timer)++;
    return;
  case 182:
    Actor_SetDestination(actorId, 268, 716);
    (*timer)++;
    return;
  case 184:
    Actor_SetDestination(actorId, 272, 728);
    (*timer)++;
    return;
  case 186:
    Actor_SetDestination(actorId, 268, 740);
    (*timer)++;
    return;
  case 188:
    Actor_SetDestination(actorId, 248, 744);
    (*timer)++;
    return;
  case 190:
    Actor_SetDestination(actorId, 228, 740);
    (*timer)++;
    return;
  case 192:
    Actor_SetDestination(actorId, 224, 728);
    (*timer)++;
    return;
  case 194:
    Actor_SetDestination(actorId, 228, 716);
    (*timer)++;
    return;
  case 196:
    Actor_SetDestination(actorId, 248, 712);
    (*timer)++;
    return;
  case 198:
    Actor_SetDestination(actorId, 268, 716);
    (*timer)++;
    return;
  case 200:
    Actor_SetDestination(actorId, 272, 728);
    (*timer)++;
    return;
  case 202:
    Actor_SetDestination(actorId, 268, 740);
    (*timer)++;
    return;
  case 204:
    Actor_SetDestination(actorId, 248, 744);
    (*timer)++;
    return;
  case 206:
    Actor_SetDestination(actorId, 248, 760);
    (*timer)++;
    return;
  case 208:
    Actor_SetDestination(actorId, 228, 764);
    (*timer)++;
    return;
  case 210:
    Actor_SetDestination(actorId, 224, 776);
    (*timer)++;
    return;
  case 212:
    Actor_SetDestination(actorId, 228, 788);
    (*timer)++;
    return;
  case 214:
    Actor_SetDestination(actorId, 248, 792);
    (*timer)++;
    return;
  case 216:
    Actor_SetDestination(actorId, 248, 840);
    (*timer)++;
    return;
  case 218:
    Actor_SetDestination(actorId, 228, 844);
    (*timer)++;
    return;
  case 220:
    Actor_SetDestination(actorId, 224, 856);
    (*timer)++;
    return;
  case 222:
    Actor_SetDestination(actorId, 228, 868);
    (*timer)++;
    return;
  case 226:
    Actor_SetDestination(actorId, 268, 868);
    (*timer)++;
    return;
  case 228:
    Actor_SetDestination(actorId, 272, 856);
    (*timer)++;
    return;
  case 230:
    Actor_SetDestination(actorId, 268, 844);
    (*timer)++;
    return;
  case 232:
    Actor_SetDestination(actorId, 248, 840);
    (*timer)++;
    return;
  case 234:
    Actor_SetDestination(actorId, 228, 844);
    (*timer)++;
    return;
  case 236:
    Actor_SetDestination(actorId, 224, 856);
    (*timer)++;
    return;
  case 238:
    Actor_SetDestination(actorId, 228, 868);
    (*timer)++;
    return;
  case 223:
  case 239:
    if (Yallam_HasArrived(actor) != 0)
      actor->timer++;
  case 224:
  case 240:
    Actor_SetDestination(actorId, 248, 872);
    actor->timer++;
    return;
  case 242:
    Actor_SetDestination(actorId, 268, 868);
    (*timer)++;
    return;
  case 244:
    Actor_SetDestination(actorId, 272, 856);
    (*timer)++;
    return;
  case 246:
    Actor_SetDestination(actorId, 268, 844);
    (*timer)++;
    return;
  case 248:
    Actor_SetDestination(actorId, 248, 840);
    (*timer)++;
    return;
  case 250:
    Actor_SetDestination(actorId, 228, 844);
    (*timer)++;
    return;
  case 252:
    Actor_SetDestination(actorId, 224, 856);
    (*timer)++;
    return;
  case 254:
    Actor_SetSpeedAndAcceleration(actorId, 1.8, 1.8);
    Actor_SetDestination(actorId, 200, 856);
    (*timer)++;
    return;
  case 256:
    Actor_SetDestination(actorId, 200, 744);
    (*timer)++;
    return;
  case 258:
    Actor_SetSpeedAndAcceleration(actorId, 1.2, 1.2);
    Actor_SetDestination(actorId, 180, 740);
    (*timer)++;
    return;
  case 260:
    Actor_SetDestination(actorId, 176, 728);
    (*timer)++;
    return;
  case 262:
    Actor_SetDestination(actorId, 180, 716);
    (*timer)++;
    return;
  case 264:
    Actor_SetDestination(actorId, 200, 712);
    (*timer)++;
    return;
  case 266:
    Actor_SetDestination(actorId, 220, 716);
    (*timer)++;
    return;
  case 268:
    Actor_SetDestination(actorId, 224, 728);
    (*timer)++;
    return;
  case 270:
    Actor_SetDestination(actorId, 220, 740);
    (*timer)++;
    return;
  case 272:
    Actor_SetDestination(actorId, 200, 744);
    (*timer)++;
    return;
  case 274:
    Actor_SetDestination(actorId, 180, 740);
    (*timer)++;
    return;
  case 276:
    Actor_SetDestination(actorId, 176, 728);
    (*timer)++;
    return;
  case 278:
    Actor_SetDestination(actorId, 180, 716);
    (*timer)++;
    return;
  case 280:
    Actor_SetDestination(actorId, 200, 712);
    (*timer)++;
    return;
  case 282:
    Actor_SetDestination(actorId, 220, 716);
    (*timer)++;
    return;
  case 284:
    Actor_SetDestination(actorId, 224, 728);
    (*timer)++;
    return;
  case 286:
    Actor_SetDestination(actorId, 220, 740);
    (*timer)++;
    return;
  case 288:
    Actor_SetDestination(actorId, 200, 744);
    (*timer)++;
    return;
  case 290:
    Actor_SetDestination(actorId, 180, 740);
    (*timer)++;
    return;
  case 292:
    Actor_SetDestination(actorId, 176, 728);
    (*timer)++;
    return;
  case 294:
    Actor_SetDestination(actorId, 180, 716);
    (*timer)++;
    return;
  case 296:
    Actor_SetDestination(actorId, 200, 712);
    (*timer)++;
    return;
  case 298:
    Actor_SetDestination(actorId, 220, 716);
    (*timer)++;
    return;
  case 300:
    Actor_SetDestination(actorId, 224, 728);
    (*timer)++;
    return;
  case 302:
    Actor_SetDestination(actorId, 220, 740);
    (*timer)++;
    return;
  case 304:
    Actor_SetDestination(actorId, 200, 744);
    (*timer)++;
    return;
  case 306:
    Actor_SetDestination(actorId, 180, 740);
    (*timer)++;
    return;
  case 308:
    Actor_SetDestination(actorId, 176, 728);
    (*timer)++;
    return;
  case 310:
    Actor_SetDestination(actorId, 180, 716);
    (*timer)++;
    return;
  case 312:
    Actor_SetDestination(actorId, 200, 712);
    (*timer)++;
    return;
  case 314:
    Actor_SetDestination(actorId, 200, 680);
    (*timer)++;
    return;
  case 316:
    Actor_SetDestination(actorId, 212, 676);
    (*timer)++;
    return;
  case 318:
    Actor_SetDestination(actorId, 216, 664);
    (*timer)++;
    return;
  case 320:
    Actor_SetDestination(actorId, 212, 652);
    (*timer)++;
    return;
  case 322:
    Actor_SetDestination(actorId, 200, 648);
    (*timer)++;
    return;
  case 1:
  case 3:
  case 5:
  case 7:
  case 9:
  case 11:
  case 13:
  case 15:
  case 17:
  case 19:
  case 21:
  case 23:
  case 25:
  case 27:
  case 29:
  case 31:
  case 33:
  case 35:
  case 37:
  case 39:
  case 41:
  case 43:
  case 45:
  case 47:
  case 49:
  case 51:
  case 53:
  case 55:
  case 57:
  case 59:
  case 61:
  case 63:
  case 65:
  case 67:
  case 69:
  case 71:
  case 73:
  case 75:
  case 77:
  case 79:
  case 81:
  case 83:
  case 85:
  case 87:
  case 89:
  case 91:
  case 93:
  case 95:
  case 97:
  case 99:
  case 101:
  case 103:
  case 105:
  case 107:
  case 109:
  case 111:
  case 113:
  case 115:
  case 117:
  case 119:
  case 121:
  case 123:
  case 125:
  case 127:
  case 129:
  case 131:
  case 133:
  case 135:
  case 137:
  case 139:
  case 141:
  case 143:
  case 145:
  case 147:
  case 149:
  case 151:
  case 153:
  case 155:
  case 157:
  case 159:
  case 161:
  case 163:
  case 165:
  case 167:
  case 169:
  case 171:
  case 173:
  case 175:
  case 177:
  case 179:
  case 181:
  case 183:
  case 185:
  case 187:
  case 189:
  case 191:
  case 193:
  case 195:
  case 197:
  case 199:
  case 201:
  case 203:
  case 205:
  case 207:
  case 209:
  case 211:
  case 213:
  case 215:
  case 217:
  case 219:
  case 221:
  case 225:
  case 227:
  case 229:
  case 231:
  case 233:
  case 235:
  case 237:
  case 241:
  case 243:
  case 245:
  case 247:
  case 249:
  case 251:
  case 253:
  case 255:
  case 257:
  case 259:
  case 261:
  case 263:
  case 265:
  case 267:
  case 269:
  case 271:
  case 273:
  case 275:
  case 277:
  case 279:
  case 281:
  case 283:
  case 285:
  case 287:
  case 289:
  case 291:
  case 293:
  case 295:
  case 297:
  case 299:
  case 301:
  case 303:
  case 305:
  case 307:
  case 309:
  case 311:
  case 313:
  case 315:
  case 317:
  case 319:
  case 321:
  case 323:
    if (Yallam_HasArrived(actor) == 0)
      return;
    (*timer)++;
    return;
  default:
    return;
  }
  (*timer)++;
}

/// A task: sets every runner's mode from the lead child's and moves them on.
void Yallam_UpdateSongGame(void) {
  struct Actor *a = Actor_Get(ACTOR_SONG_LEADER);
  struct Actor *b = Actor_Get(ACTOR_GAME_CHILD);
  struct Actor *c = Actor_Get(ACTOR_CIRCLES_CHILD);
  struct Actor *d = Actor_Get(ACTOR_STORY_CHILD);
  struct Actor *leader = Actor_Get(gMainRoutineWork.leaderActorId);
  s32 value = (s16)a->userValue;

  switch (value) {
  case SONG_GAME_GO:
    a->riseCounter = RUNNER_GOING;
    b->riseCounter = RUNNER_GOING;
    c->riseCounter = RUNNER_GOING;
    d->riseCounter = RUNNER_GOING;
    leader->riseCounter = RUNNER_GOING;
    break;
  case SONG_GAME_HALT:
    a->riseCounter = RUNNER_HALTING;
    b->riseCounter = RUNNER_HALTING;
    c->riseCounter = RUNNER_HALTING;
    d->riseCounter = RUNNER_HALTING;
    leader->riseCounter = RUNNER_HALTING;
    break;
  case SONG_GAME_HALT_AND_STAND:
    a->riseCounter = RUNNER_HALTING_TO_STAND;
    b->riseCounter = RUNNER_HALTING_TO_STAND;
    c->riseCounter = RUNNER_HALTING_TO_STAND;
    d->riseCounter = RUNNER_HALTING_TO_STAND;
    leader->riseCounter = RUNNER_HALTING_TO_STAND;
    break;
  }
  a->userValue = 0;
  Yallam_RunSongPath(ACTOR_SONG_LEADER);
  Yallam_RunSongPath(ACTOR_GAME_CHILD);
  Yallam_RunSongPath(ACTOR_CIRCLES_CHILD);
  Yallam_RunSongPath(ACTOR_STORY_CHILD);
  Yallam_RunSongPath(gMainRoutineWork.leaderActorId);
}

/// Saves every runner's state and stops the game's task.
void Yallam_PauseSongGame(void) {
  struct Actor *actor;

  actor = Actor_Get(ACTOR_SONG_LEADER);
  gYallam_SavedSongGame[0].timer = actor->timer;
  gYallam_SavedSongGame[0].userValue = (s16)actor->userValue;
  gYallam_SavedSongGame[0].riseCounter = actor->riseCounter;
  gYallam_SavedSongGame[0].riseEnabled = actor->riseEnabled;
  actor = Actor_Get(ACTOR_GAME_CHILD);
  gYallam_SavedSongGame[1].timer = actor->timer;
  gYallam_SavedSongGame[1].userValue = (s16)actor->userValue;
  gYallam_SavedSongGame[1].riseCounter = actor->riseCounter;
  gYallam_SavedSongGame[1].riseEnabled = actor->riseEnabled;
  actor = Actor_Get(ACTOR_CIRCLES_CHILD);
  gYallam_SavedSongGame[2].timer = actor->timer;
  gYallam_SavedSongGame[2].userValue = (s16)actor->userValue;
  gYallam_SavedSongGame[2].riseCounter = actor->riseCounter;
  gYallam_SavedSongGame[2].riseEnabled = actor->riseEnabled;
  actor = Actor_Get(ACTOR_STORY_CHILD);
  gYallam_SavedSongGame[3].timer = actor->timer;
  gYallam_SavedSongGame[3].userValue = (s16)actor->userValue;
  gYallam_SavedSongGame[3].riseCounter = actor->riseCounter;
  gYallam_SavedSongGame[3].riseEnabled = actor->riseEnabled;
  actor = Actor_Get(gMainRoutineWork.leaderActorId);
  gYallam_SavedSongGame[4].timer = actor->timer;
  gYallam_SavedSongGame[4].userValue = (s16)actor->userValue;
  gYallam_SavedSongGame[4].riseCounter = actor->riseCounter;
  gYallam_SavedSongGame[4].riseEnabled = actor->riseEnabled;
  FieldTask_Remove(Yallam_UpdateSongGame);
}

/// Restores every runner's state and starts the game's task again.
void Yallam_ResumeSongGame(void) {
  struct Actor *actor;

  actor = Actor_Get(ACTOR_SONG_LEADER);
  actor->timer = gYallam_SavedSongGame[0].timer;
  actor->userValue = gYallam_SavedSongGame[0].userValue;
  actor->riseCounter = gYallam_SavedSongGame[0].riseCounter;
  actor->riseEnabled = gYallam_SavedSongGame[0].riseEnabled;
  actor = Actor_Get(ACTOR_GAME_CHILD);
  actor->timer = gYallam_SavedSongGame[1].timer;
  actor->userValue = gYallam_SavedSongGame[1].userValue;
  actor->riseCounter = gYallam_SavedSongGame[1].riseCounter;
  actor->riseEnabled = gYallam_SavedSongGame[1].riseEnabled;
  actor = Actor_Get(ACTOR_CIRCLES_CHILD);
  actor->timer = gYallam_SavedSongGame[2].timer;
  actor->userValue = gYallam_SavedSongGame[2].userValue;
  actor->riseCounter = gYallam_SavedSongGame[2].riseCounter;
  actor->riseEnabled = gYallam_SavedSongGame[2].riseEnabled;
  actor = Actor_Get(ACTOR_STORY_CHILD);
  actor->timer = gYallam_SavedSongGame[3].timer;
  actor->userValue = gYallam_SavedSongGame[3].userValue;
  actor->riseCounter = gYallam_SavedSongGame[3].riseCounter;
  actor->riseEnabled = gYallam_SavedSongGame[3].riseEnabled;
  actor = Actor_Get(gMainRoutineWork.leaderActorId);
  actor->timer = gYallam_SavedSongGame[4].timer;
  actor->userValue = gYallam_SavedSongGame[4].userValue;
  actor->riseCounter = gYallam_SavedSongGame[4].riseCounter;
  actor->riseEnabled = gYallam_SavedSongGame[4].riseEnabled;
  Task_AddFrame(Yallam_UpdateSongGame, TASK_PASS_FRAME);
}

/// Readies an actor to run the song game's path from its start.
void Yallam_StartRunner(s32 id) {
  struct Actor *actor = Actor_Get(id);

  Actor_SetSpeedAndAcceleration(id, 1.2, 1.2);
  Actor_SetAnimation(id, ANIM_WALK);
  actor->timer = 0;
  actor->userValue = 0;
  actor->riseCounter = RUNNER_HALTED;
  actor->riseEnabled = 0;
}

extern u32 gYallam_CirclesChildGathers[];
extern u32 gYallam_GameChildGathers[];
extern u32 gYallam_SongLeaderForms[];
extern u32 gYallam_CirclesChildForms[];
extern u32 gYallam_GameChildForms[];
extern u32 gYallam_StoryChildForms[];
extern u32 gYallam_SongLeaderStarts[];
extern u32 gYallam_GameChildStarts[];
extern u32 gYallam_CirclesChildStarts[];

/// The boy sings the song of the stars ("If you want to go to the stars...")
/// while the children lead Felix and Jenna round the meadow's rings, one verse
/// at a time; then he offers the second song.
void Yallam_ShowSongGame(void) {
  struct Actor *actor = Actor_Get(ACTOR_SONG_LEADER);
  struct Actor *leader;
  FieldEvent_SetMessageId(MsgHereLetMe);
  // "Here, let me show you!"
  Event_ShowMessage(ACTOR_SONG_LEADER, 0);
  FieldActor_Stop(ACTOR_CIRCLES_CHILD);
  FieldActor_Stop(ACTOR_GAME_CHILD);
  (Actor_Get(ACTOR_CIRCLES_CHILD))->update = 0;
  (Actor_Get(ACTOR_GAME_CHILD))->update = 0;
  System_Wait(1);
  Actor_SetSpeed(ACTOR_CIRCLES_CHILD, 1.6);
  Actor_SetSpeed(ACTOR_GAME_CHILD, 1.6);
  Actor_SetScript(ACTOR_CIRCLES_CHILD, gYallam_CirclesChildGathers);
  Actor_SetScript(ACTOR_GAME_CHILD, gYallam_GameChildGathers);
  (FieldCamera_GetActor())->motionFlags = 0;
  FieldCamera_SetSpeed(0.8);
  FieldCamera_MoveTo(304, CAMERA_KEEP, 878, 1);
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 0.8);
  Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 296, 872);
  Actor_FaceDirectionAndWait(gMainRoutineWork.leaderActorId, FACING_EAST);
  Event_PlaySound(SOUND_FADE_OUT_BGM);
  Actor_FaceDirection(ACTOR_SONG_LEADER, FACING_NORTH_NORTHWEST, 20);
  Actor_FaceDirectionAndWait(ACTOR_SONG_LEADER, FACING_NORTH_NORTHEAST);
  FieldActor_Jump(ACTOR_SONG_LEADER, 2, 10);
  FieldActor_Jump(ACTOR_SONG_LEADER, 4, 10);
  Actor_SetAnimationAndWait(ACTOR_SONG_LEADER, ANIM_NOD);
  Actor_SetSpeed(ACTOR_SONG_LEADER, 1.6);
  Actor_SetSpeed(ACTOR_STORY_CHILD, 1.6);
  Actor_SetScript(ACTOR_SONG_LEADER, gYallam_SongLeaderForms);
  Actor_SetScript(ACTOR_CIRCLES_CHILD, gYallam_CirclesChildForms);
  Actor_SetScript(ACTOR_GAME_CHILD, gYallam_GameChildForms);
  Actor_RunScriptAndWait(ACTOR_STORY_CHILD, gYallam_StoryChildForms);
  Actor_SetPosition(ACTOR_JENNA, 296, 872);
  System_Wait(1);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_NORTHEAST, 0);
  Actor_SetSpeed(ACTOR_JENNA, 1.2);
  Actor_WalkToAndWait(ACTOR_JENNA, 312, 854);
  Actor_FaceDirectionAndWait(ACTOR_JENNA, FACING_SOUTHWEST);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimationAndWait(gMainRoutineWork.leaderActorId, ANIM_NOD);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_WALK);
  leader = Actor_Get(gMainRoutineWork.leaderActorId);
  if (leader != 0) {
    Actor_SetDestination(ACTOR_JENNA, leader->x.part[0].integer,
                         leader->z.part[0].integer);
  }
  FieldActor_WaitForMoveById(ACTOR_JENNA);
  Actor_SetPosition(ACTOR_JENNA, 0, 0);
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 1.6);
  Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 316, 896);
  Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 344, 912);
  Actor_FaceDirectionAndWait(gMainRoutineWork.leaderActorId, FACING_NORTH);
  Event_PlaySound(MUS_CHILDRENS_SONG);
  Actor_FaceDirectionAndWait(ACTOR_SONG_LEADER, FACING_NORTH);
  // "If you want to go to the stars, if you want to go to the stars."
  Event_ShowMessage(ACTOR_SONG_LEADER, 0);
  EventCamera_FollowActor(ACTOR_SONG_LEADER, 1);
  Actor_SetScript(ACTOR_SONG_LEADER, gYallam_SongLeaderStarts);
  Actor_SetScript(ACTOR_GAME_CHILD, gYallam_GameChildStarts);
  Actor_RunScriptAndWait(ACTOR_CIRCLES_CHILD, gYallam_CirclesChildStarts);
  Actor_WalkToAndWait(ACTOR_STORY_CHILD, 344, 890);
  Actor_FaceDirectionAndWait(ACTOR_STORY_CHILD, FACING_NORTH);
  Yallam_StartRunner(ACTOR_SONG_LEADER);
  Yallam_StartRunner(ACTOR_GAME_CHILD);
  Yallam_StartRunner(ACTOR_CIRCLES_CHILD);
  Yallam_StartRunner(ACTOR_STORY_CHILD);
  Yallam_StartRunner(gMainRoutineWork.leaderActorId);
  Task_AddFrame(Yallam_UpdateSongGame, TASK_PASS_FRAME);
  // "Go north past the twins, passing two swirls, run deasil twice 'round the
  // trunk."
  Event_ShowMessage(ACTOR_SONG_LEADER, 0);
  actor->userValue = SONG_GAME_GO;
  do
    System_Wait(1);
  while (actor->timer != 34);
  actor->userValue = SONG_GAME_HALT;
  System_Wait(1);
  // "Haste without waste, and head to the east, past three to the north."
  Event_ShowMessage(ACTOR_SONG_LEADER, 0);
  actor->userValue = SONG_GAME_GO;
  do
    System_Wait(1);
  while (actor->timer != 42);
  actor->userValue = SONG_GAME_HALT;
  System_Wait(1);
  // "There, run 'round thrice and wait for the waves. When they stop, run
  // north and go to the stars."
  Event_ShowMessage(ACTOR_SONG_LEADER, 0);
  actor->userValue = SONG_GAME_GO;
  do
    System_Wait(1);
  while (actor->timer != 108);
  actor->userValue = SONG_GAME_HALT_AND_STAND;
  System_Wait(1);
  // "If you race full of folly and take the wrong way, you'll find yourself a
  // watery grave."
  Event_ShowMessage(ACTOR_SONG_LEADER, 0);
  Yallam_PauseSongGame();
  FieldActor_Jump(ACTOR_SONG_LEADER, 4, 0);
  FieldActor_Jump(ACTOR_GAME_CHILD, 4, 0);
  FieldActor_Jump(ACTOR_CIRCLES_CHILD, 4, 0);
  FieldActor_Jump(ACTOR_STORY_CHILD, 4, 40);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_SONG_LEADER, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_GAME_CHILD, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_CIRCLES_CHILD, FACING_WEST, 0);
  Actor_FaceDirectionAndWait(ACTOR_STORY_CHILD, FACING_WEST);
  // "Do you wanna hear the second children's song Yepp wrote?"
  Event_OpenMessage(ACTOR_SONG_LEADER, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(gMainRoutineWork.leaderActorId,
                                            0) == 0) {
    Yallam_SingMoonSong();
  } else {
    // "Well, that's just fine then. Give me a holler if you ever do."
    FieldEvent_ShowMessageThenWait(ACTOR_SONG_LEADER, 0, 20);
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Event_PlaySound(SE_MAP_EXIT);
    FieldEvent_Wait(40);
    FieldEvent_SetExitRequest(YALLAM_EXIT_SONG_GAME);
  }
}

/// The moon song's words, sung verse by verse.
extern u8 MsgIfYouWant[];

/// The song of the moon, sung and run round the meadow's rings verse by verse
/// as the stars song was; then the boy offers the third song.
void Yallam_SingMoonSong(void) {
  struct Actor *actor = Actor_Get(ACTOR_SONG_LEADER);

  Actor_FaceDirection(ACTOR_SONG_LEADER, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_GAME_CHILD, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_CIRCLES_CHILD, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_STORY_CHILD, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirectionAndWait(gMainRoutineWork.leaderActorId, FACING_EAST);
  FieldEvent_SetMessageId((s32)&MsgIfYouWant);
  // "If you want to go to the moon, if you want to go to the moon."
  Event_ShowMessage(ACTOR_SONG_LEADER, 0);
  Yallam_ResumeSongGame();
  actor->userValue = SONG_GAME_GO;
  do
    System_Wait(1);
  while (actor->timer != 114);
  actor->userValue = SONG_GAME_HALT;
  System_Wait(1);
  // "Face west from the stars and run straight past three swirls, then circle
  // 'round twice."
  Event_ShowMessage(ACTOR_SONG_LEADER, 0);
  actor->userValue = SONG_GAME_GO;
  do
    System_Wait(1);
  while (actor->timer != 150);
  actor->userValue = SONG_GAME_HALT;
  System_Wait(1);
  // "Haste without waste, and head to the south, then one swirl west."
  Event_ShowMessage(ACTOR_SONG_LEADER, 0);
  actor->userValue = SONG_GAME_GO;
  do
    System_Wait(1);
  while (actor->timer != 154);
  actor->userValue = SONG_GAME_HALT;
  System_Wait(1);
  // "There, run 'round thrice and wait for the waves. When they stop, run
  // south and go for the moon."
  Event_ShowMessage(ACTOR_SONG_LEADER, 0);
  actor->userValue = SONG_GAME_GO;
  do
    System_Wait(1);
  while (actor->timer != 216);
  actor->userValue = SONG_GAME_HALT_AND_STAND;
  System_Wait(1);
  // "If you race full of folly and take the wrong way, you'll find yourself a
  // watery grave."
  Event_ShowMessage(ACTOR_SONG_LEADER, 0);
  Yallam_PauseSongGame();
  FieldActor_Jump(ACTOR_SONG_LEADER, 4, 0);
  FieldActor_Jump(ACTOR_GAME_CHILD, 4, 0);
  FieldActor_Jump(ACTOR_CIRCLES_CHILD, 4, 0);
  FieldActor_Jump(ACTOR_STORY_CHILD, 4, 40);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_SONG_LEADER, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_GAME_CHILD, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_CIRCLES_CHILD, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirectionAndWait(ACTOR_STORY_CHILD, FACING_NORTH_NORTHEAST);
  // "OK, so, now do you want to hear the third children's song?"
  Event_OpenMessage(ACTOR_SONG_LEADER, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(gMainRoutineWork.leaderActorId,
                                            0) == 0) {
    Yallam_SingSunSong();
  } else {
    // "Well, that's just fine then. Give me a holler if you ever do."
    FieldEvent_ShowMessageThenWait(ACTOR_SONG_LEADER, 0, 20);
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Event_PlaySound(SE_MAP_EXIT);
    FieldEvent_Wait(40);
    FieldEvent_SetExitRequest(YALLAM_EXIT_SONG_GAME);
  }
}

/// The sun song's words, sung verse by verse.
extern u8 MsgIfYouWantTo[];

/// The song of the sun, sung and run round the meadow's rings verse by verse;
/// then each child hops away and back in turn, the boy says that is all the
/// games they have, and the screen fades back to the village.
void Yallam_SingSunSong(void) {
  struct Actor *actor = Actor_Get(ACTOR_SONG_LEADER);
  FIXED x, z;

  Actor_FaceDirection(ACTOR_SONG_LEADER, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_GAME_CHILD, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_CIRCLES_CHILD, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_STORY_CHILD, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirectionAndWait(gMainRoutineWork.leaderActorId, FACING_SOUTHWEST);
  FieldEvent_SetMessageId((s32)&MsgIfYouWantTo);
  // "If you want to go to the sun, if you want to go to the sun."
  Event_ShowMessage(ACTOR_SONG_LEADER, 0);
  Yallam_ResumeSongGame();
  actor->userValue = SONG_GAME_GO;
  do
    System_Wait(1);
  while (actor->timer != 218);
  actor->userValue = SONG_GAME_HALT;
  System_Wait(1);
  // "Face south from the moon and run straight through two swirls, then
  // circle 'round twice."
  Event_ShowMessage(ACTOR_SONG_LEADER, 0);
  actor->userValue = SONG_GAME_GO;
  do
    System_Wait(1);
  while (actor->timer != 254);
  actor->userValue = SONG_GAME_HALT;
  System_Wait(1);
  // "Haste without waste, and head to the west, then six to the north."
  Event_ShowMessage(ACTOR_SONG_LEADER, 0);
  actor->userValue = SONG_GAME_GO;
  do
    System_Wait(1);
  while (actor->timer != 258);
  actor->userValue = SONG_GAME_HALT;
  System_Wait(1);
  // "There, run 'round thrice and wait for the waves. When they stop, run
  // north and go for the sun."
  Event_ShowMessage(ACTOR_SONG_LEADER, 0);
  actor->userValue = SONG_GAME_GO;
  do
    System_Wait(1);
  while (actor->timer != 324);
  actor->userValue = SONG_GAME_HALT_AND_STAND;
  System_Wait(1);
  // "If you race full of folly and take the wrong way, you'll find yourself a
  // watery grave."
  Event_ShowMessage(ACTOR_SONG_LEADER, 0);
  Yallam_PauseSongGame();
  Actor_FaceDirectionAndWait(ACTOR_SONG_LEADER, FACING_SOUTH_SOUTHWEST);
  Actor_Get(ACTOR_SONG_LEADER)->steeringFlags &= ~ACTOR_FACES_MOTION;
  actor = Actor_Get(ACTOR_SONG_LEADER);
  z = actor->z.fixed;
  x = actor->x.fixed;
  Actor_SetSpeed(ACTOR_SONG_LEADER, 2.4);
  FieldActor_Jump(ACTOR_SONG_LEADER, 6, 0);
  Actor_MoveToAndWait(ACTOR_SONG_LEADER, 200, 670);
  FieldActor_Jump(ACTOR_SONG_LEADER, 4, 0);
  FieldActor_SetMoveTargetFX(actor, x, 0, z);
  FieldEvent_Wait(5);
  Actor_FaceDirectionAndWait(ACTOR_GAME_CHILD, FACING_SOUTH_SOUTHWEST);
  Actor_Get(ACTOR_GAME_CHILD)->steeringFlags &= ~ACTOR_FACES_MOTION;
  actor = Actor_Get(ACTOR_GAME_CHILD);
  z = actor->z.fixed;
  x = actor->x.fixed;
  Actor_SetSpeed(ACTOR_GAME_CHILD, 2.4);
  FieldActor_Jump(ACTOR_GAME_CHILD, 6, 0);
  Actor_MoveToAndWait(ACTOR_GAME_CHILD, 200, 670);
  FieldActor_Jump(ACTOR_GAME_CHILD, 4, 0);
  FieldActor_SetMoveTargetFX(actor, x, 0, z);
  FieldEvent_Wait(5);
  Actor_FaceDirectionAndWait(ACTOR_CIRCLES_CHILD, FACING_WEST);
  Actor_Get(ACTOR_CIRCLES_CHILD)->steeringFlags &= ~ACTOR_FACES_MOTION;
  actor = Actor_Get(ACTOR_CIRCLES_CHILD);
  z = actor->z.fixed;
  x = actor->x.fixed;
  Actor_SetSpeed(ACTOR_CIRCLES_CHILD, 2.4);
  FieldActor_Jump(ACTOR_CIRCLES_CHILD, 6, 0);
  Actor_MoveToAndWait(ACTOR_CIRCLES_CHILD, 200, 670);
  FieldActor_Jump(ACTOR_CIRCLES_CHILD, 4, 0);
  FieldActor_SetMoveTargetFX(actor, x, 0, z);
  FieldEvent_Wait(10);
  Actor_FaceDirectionAndWait(ACTOR_STORY_CHILD, FACING_NORTH_NORTHWEST);
  Actor_Get(ACTOR_STORY_CHILD)->steeringFlags &= ~ACTOR_FACES_MOTION;
  actor = Actor_Get(ACTOR_STORY_CHILD);
  z = actor->z.fixed;
  x = actor->x.fixed;
  Actor_SetSpeed(ACTOR_STORY_CHILD, 2.4);
  FieldActor_Jump(ACTOR_STORY_CHILD, 6, 0);
  Actor_MoveToAndWait(ACTOR_STORY_CHILD, 200, 670);
  FieldActor_Jump(ACTOR_STORY_CHILD, 4, 0);
  FieldActor_SetMoveTargetFX(actor, x, 0, z);
  FieldActor_WaitForMoveById(ACTOR_STORY_CHILD);
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 0.8);
  Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 184, 686);
  Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 186, 674);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_NORTHEAST, 0);
  Actor_Get(ACTOR_SONG_LEADER)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_Get(ACTOR_GAME_CHILD)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_Get(ACTOR_CIRCLES_CHILD)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_Get(ACTOR_STORY_CHILD)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_FaceDirection(ACTOR_SONG_LEADER, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_GAME_CHILD, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_CIRCLES_CHILD, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_STORY_CHILD, FACING_WEST, 20);
  // "That's it. That's all the games we have for the songs... If you want to
  // play some more, just give me a holler."
  FieldEvent_ShowMessageThenWait(ACTOR_SONG_LEADER, 0, 20);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_PlaySound(SE_MAP_EXIT);
  FieldEvent_Wait(40);
  FieldEvent_SetExitRequest(YALLAM_EXIT_SONG_GAME);
}

/// Once Force has pushed it, the log stands in the gap where it fell.
void Yallam_PlaceFallenLog(void) {
  if (Event_GetFlag(FLAG_YALLAM_LOG_FALLEN)) {
    SET_ACTOR_POSITION(Actor_Get(ACTOR_FALLING_LOG), toFIXED(168), 0,
                       toFIXED(192));
  }
}

s32 Scene_Initialize(void) {
  if (gMainRoutineWork.next == SCENE_YALLAM) {
    if (gMainRoutineWork.entrance == YALLAM_SONG_GAME)
      Yallam_StartRunningChildren();
    else
      Yallam_PlaceFallenLog();
  }
  return 0;
}

s32 Scene_PrepareMap(void) {
  if (gMainRoutineWork.next == SCENE_YALLAM &&
      Event_GetFlag(FLAG_YALLAM_LOG_FALLEN))
    MapGrid_CopyCellAttributes(8, 12, 1, 1, 10, 12);
  return 0;
}

/// Nothing happens as Force first reaches the log; the push itself is
/// Yallam_ForceLogDown.
void Yallam_DoNothing(void) {}

/// Force cast at the log standing on its stump east of the cave mouth near
/// Sunshine's house tips it over west: it spins off the ledge with a crash and
/// drops upright into the gap below (cell (10, 12)), a post Felix can now hop
/// onto.
/// The hop comes from the map's own hop cell at (8, 12), whose flags are
/// copied onto the log's cell.
void Yallam_ForceLogDown(void) {
  s32 step;
  s32 i;
  struct Actor *actor = Actor_Get(ACTOR_FALLING_LOG);
  struct FieldSprite *sprite = actor->sprite;

  Event_SetFlag(FLAG_YALLAM_LOG_FALLEN);
  actor->acceleration = toFIXED(3) / 2;
  actor->speedLimit = toFIXED(8);
  Event_PlaySound(SE_TOPPLE);
  FieldActor_SetMoveTarget(actor, 176, 40, 184);
  step = 0;
  for (i = 0; i < 16; i++) {
    sprite->rotation -= step;
    System_Wait(1);
    step += TOPPLE_SPIN_GAIN;
  }
  FieldActor_SetMoveTarget(actor, 168, 16, 184);
  step = TOPPLE_ROLL_SPIN;
  for (i = 0; i < 26; i++) {
    sprite->rotation -= step;
    System_Wait(1);
    step += TOPPLE_SPIN_GAIN;
  }
  FieldActor_WaitForMove(actor);
  FieldEvent_Wait(2);
  Event_PlaySound(SE_LOG_FALL);
  // The log stands upright and still where it fell.
  i = 0;
  sprite->rotation = i;
  actor->x.fixed = toFIXED(168);
  actor->z.fixed = toFIXED(192);
  actor->y.fixed = i;
  actor->velocityY = i;
  actor->velocityX = i;
  MapGrid_CopyCellAttributes(8, 12, 1, 1, 10, 12);
}

/// The boy and the girl with a topknot run round the big tree in the
/// children's meadow, each after the other (Yallam_UpdateRunningChild).
void Yallam_StartRunningChildren(void) {
  struct Actor *actor = Actor_Get(ACTOR_CIRCLES_CHILD);
  struct Actor *other;

  actor->timer = CHILD_CHASES_GAME_CHILD;
  actor->riseEnabled = 0;
  actor->update = Yallam_UpdateRunningChild;
  other = Actor_Get(ACTOR_GAME_CHILD);
  other->timer = 0;
  other->riseEnabled = 0;
  other->update = Yallam_UpdateRunningChild;
}

void Yallam_EnterDoor(void);
void Yallam_UncoverAntidote(void);
void Yallam_UncoverNut(void);
void Yallam_TalkAboutSongCircles(void);

extern s32 gYallam_StandingLogLowered[];

/// The log standing on its stump on the ledge, raised onto it.
s32 gYallam_LogRaised[] = {
    SC_ANIMATION(3),
    SC_SET(SW_MOTION, 0),
    SC_ADD(SW_Y, toFIXED(2)),
    SC_CALL((s32)Yallam_HideShadow),
    SC_END,
};

/// The stump, sunk into the ledge below the log.
s32 gYallam_StumpLowered[] = {
    SC_SET(SW_MOTION, 0),
    SC_ADD(SW_Y, -toFIXED(16)),
    SC_CALL((s32)Yallam_HideShadow),
    SC_END,
};

/// The standing log, shrunk and sunk.
s32 gYallam_StandingLogLowered[] = {
    SC_SET(SW_SCALE_X, FIXED_ONE * 3 / 5),
    SC_SET(SW_SCALE_Y, FIXED_ONE * 3 / 5),
    SC_SET(SW_MOTION, 0),
    SC_ADD(SW_Y, -toFIXED(16)),
    SC_CALL((s32)Yallam_HideShadow),
    SC_END,
};

// clang-format off
/// The boy runs round the big tree in the children's meadow, a six-sided lap.
u32 gYallam_CirclesChildRuns[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_SPEED(1),
    SC_ACCELERATION(0.4),
    SC_MOVE(328, 0, 752),
    SC_SYNC,
    SC_MOVE(360, 0, 752),
    SC_SYNC,
    SC_MOVE(376, 0, 776),
    SC_SYNC,
    SC_MOVE(360, 0, 792),
    SC_SYNC,
    SC_MOVE(328, 0, 792),
    SC_SYNC,
    SC_MOVE(312, 0, 776),
    SC_SYNC,
    SC_LOOP(SC_FOREVER, SC_TOP),
};

/// As gYallam_CirclesChildRuns, for the game child, half a lap ahead of it.
u32 gYallam_GameChildRuns[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_SPEED(1),
    SC_ACCELERATION(0.4),
    SC_MOVE(360, 0, 792),
    SC_SYNC,
    SC_MOVE(328, 0, 792),
    SC_SYNC,
    SC_MOVE(312, 0, 776),
    SC_SYNC,
    SC_MOVE(328, 0, 752),
    SC_SYNC,
    SC_MOVE(360, 0, 752),
    SC_SYNC,
    SC_MOVE(376, 0, 776),
    SC_SYNC,
    SC_LOOP(SC_FOREVER, SC_TOP),
};

/// The children who were running gather by the song leader.
u32 gYallam_CirclesChildGathers[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(336, 0, 792),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_WAIT(1),
    SC_SET(SW_FACING, FACING_SOUTH_SOUTHWEST),
    SC_WAIT(10),
    SC_END,
};

/// As gYallam_CirclesChildGathers, for the game child.
u32 gYallam_GameChildGathers[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(352, 0, 792),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_WAIT(1),
    SC_SET(SW_FACING, FACING_SOUTH_SOUTHWEST),
    SC_WAIT(10),
    SC_END,
};

/// The children take their places in the ring for the game.
u32 gYallam_SongLeaderForms[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(316, 0, 888),
    SC_SYNC,
    SC_MOVE(336, 0, 888),
    SC_SYNC,
    SC_MOVE(344, 0, 872),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_WAIT(1),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(10),
    SC_END,
};

/// As gYallam_SongLeaderForms, for the circles child.
u32 gYallam_CirclesChildForms[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(362, 0, 792),
    SC_SYNC,
    SC_MOVE(384, 0, 848),
    SC_SYNC,
    SC_MOVE(382, 0, 888),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_WAIT(1),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(10),
    SC_END,
};

/// As gYallam_SongLeaderForms, for the game child.
u32 gYallam_GameChildForms[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(364, 0, 808),
    SC_SYNC,
    SC_MOVE(374, 0, 878),
    SC_SYNC,
    SC_MOVE(360, 0, 888),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_WAIT(1),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(10),
    SC_END,
};

/// As gYallam_SongLeaderForms, for the story child.
u32 gYallam_StoryChildForms[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(274, 0, 888),
    SC_SYNC,
    SC_MOVE(328, 0, 888),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(10),
    SC_END,
};

/// The children step to where the song's run begins.
u32 gYallam_SongLeaderStarts[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(344, 0, 840),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_END,
};

/// As gYallam_SongLeaderStarts, for the game child.
u32 gYallam_GameChildStarts[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(344, 0, 888),
    SC_SYNC,
    SC_MOVE(344, 0, 856),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_WAIT(1),
    SC_END,
};

/// As gYallam_SongLeaderStarts, for the circles child.
u32 gYallam_CirclesChildStarts[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(344, 0, 888),
    SC_SYNC,
    SC_MOVE(344, 0, 874),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_WAIT(1),
    SC_END,
};

#include "shared/scene_entrances_default.c"

u32 gSceneExits[] = {
    SCENE_EXITS(Yallam),
    SCENE_EXIT(YALLAM_TO_YALLAM_ROOMS_INN_SOUTHEAST, YallamRoomsInn, YALLAM_ROOMS_INN_FROM_YALLAM_SOUTHEAST),
    SCENE_EXIT(YALLAM_TO_YALLAM_ROOMS_INN_SOUTH, YallamRoomsInn, YALLAM_ROOMS_INN_FROM_YALLAM_SOUTH),
    SCENE_EXIT(YALLAM_TO_YALLAM_ROOMS_SHOPS_EAST, YallamRoomsShops, YALLAM_ROOMS_SHOPS_FROM_YALLAM_EAST),
    SCENE_EXIT(YALLAM_TO_YALLAM_ROOMS_INN_WEST, YallamRoomsInn, YALLAM_ROOMS_INN_FROM_YALLAM_WEST),
    SCENE_EXIT(YALLAM_TO_YALLAM_ROOMS_SHOPS_NORTH, YallamRoomsShops, YALLAM_ROOMS_SHOPS_FROM_YALLAM_NORTH),
    SCENE_EXIT(YALLAM_TO_YALLAM_ROOMS_SHOPS_WEST, YallamRoomsShops, YALLAM_ROOMS_SHOPS_FROM_YALLAM_WEST),
    SCENE_EXIT(YALLAM_TO_YALLAM_ROOMS_INN_NORTHEAST, YallamRoomsInn, YALLAM_ROOMS_INN_FROM_YALLAM_NORTHEAST),
    SCENE_EXIT(YALLAM_TO_MIKASALLA_ROOMS_SANCTUM, MikasallaRoomsSanctum, MIKASALLA_ROOMS_SANCTUM_FROM_YALLAM),
    SCENE_EXIT(YALLAM_TO_YALLAM_CAVE_EAST, YallamCave, YALLAM_CAVE_FROM_YALLAM_EAST),
    SCENE_EXIT(YALLAM_TO_WORLD_MAP_SOUTH, WorldMap, WORLD_MAP_FROM_YALLAM_SOUTH_B),
    SCENE_EXIT(YALLAM_TO_WORLD_MAP_NORTH, WorldMap, WORLD_MAP_FROM_YALLAM_SOUTH_A),
    SCENE_EXIT(YALLAM_TO_SONG_GAME, Yallam, YALLAM_SONG_GAME),
    SCENE_EXIT(YALLAM_TO_YALLAM_CAVE_WEST, YallamCave, YALLAM_CAVE_FROM_YALLAM_WEST),
    SCENE_EXIT(YALLAM_EXIT_SONG_GAME, Yallam, YALLAM_FROM_SONG_GAME),
    SCENE_EXITS(YallamCave),
    SCENE_EXIT(YALLAM_CAVE_TO_YALLAM_EAST, Yallam, YALLAM_FROM_YALLAM_CAVE_EAST),
    SCENE_EXIT(YALLAM_CAVE_TO_YALLAM_WEST, Yallam, YALLAM_FROM_YALLAM_CAVE_WEST),
    SCENE_EXITS_END,
};

/// The village, in YallamActorId order.
struct ActorPlacement gYallam_Placements[] = {
    ACTOR_PLACEMENT(SPRITE_FALLING_LOG, CONDITION_ALWAYS,
                    (s32)gYallam_StumpLowered, 184, 0, 200, FACING_WEST,
                    TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_FALLING_LOG, CONDITION_ALWAYS,
                    (s32)gYallam_LogRaised, 184, 0, 200, FACING_WEST,
                    TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_STANDING_LOG, CONDITION_ALWAYS,
                    (s32)gYallam_StandingLogLowered, 8, 0, 8, FACING_WEST,
                    TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 616, 0, 216,
                    FACING_SOUTHEAST, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 712, 0, 232,
                    FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
#if !TLA_PATCH_1
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_CURLY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 552, 0, 232,
                    FACING_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
#else
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_CURLY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 552, 0,
                    248, FACING_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
#endif
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 360, 0, 360,
                    FACING_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_GREY_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 456, 0, 280,
                    FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN_WITH_HEADBAND, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 376,
                    FACING_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_YELLOW_BANDANA, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 464, 0, 136, FACING_SOUTHWEST,
                    TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_YELLOW_BANDANA, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 392, 0, 264, FACING_SOUTH,
                    TALK_FACING_PARTY_AND_BACK, 0),
    {SCENE_TABLE_END},
};

/// The cave places no actors.
struct ActorPlacement gYallam_CavePlacements[] = {
    {SCENE_TABLE_END},
};

/// The children's meadow, in YallamSongGameActorId order, and Jenna.
struct ActorPlacement gYallam_SongGamePlacements[] = {
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_BLACK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 872, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_BLACK_HAIR, CONDITION_ALWAYS, (s32)gYallam_CirclesChildRuns, 312, 0, 776, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_WITH_TOPKNOT, CONDITION_ALWAYS, (s32)gYallam_GameChildRuns, 376, 0, 776, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_WITH_TOPKNOT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 272, 0, 800, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

/// The huts' doors' cell animations as they open, one for each shape of door.
u16 gYallam_DoorOpening[] = {
    MAPGRID_ANIMATION_STEP(93, 28, 1, 2, 6),
    MAPGRID_ANIMATION_STEP(92, 28, 1, 2, 6),
    MAPGRID_ANIMATION_STEPS_END,
};

/// The second shape of hut door's opening, as gYallam_DoorOpening.
u16 gYallam_SecondDoorOpening[] = {
    MAPGRID_ANIMATION_STEP(95, 28, 1, 2, 6),
    MAPGRID_ANIMATION_STEP(94, 28, 1, 2, 6),
    MAPGRID_ANIMATION_STEPS_END,
};

/// The third shape of hut door's opening, as gYallam_DoorOpening.
u16 gYallam_ThirdDoorOpening[] = {
    MAPGRID_ANIMATION_STEP(89, 28, 1, 2, 6),
    MAPGRID_ANIMATION_STEP(90, 28, 1, 2, 6),
    MAPGRID_ANIMATION_STEPS_END,
};

/// The sanctum's door opening, two cells wide.
u16 gYallam_SanctumDoorOpening[] = {
    MAPGRID_ANIMATION_STEP(92, 30, 2, 2, 6),
    MAPGRID_ANIMATION_STEP(94, 30, 2, 2, 6),
    MAPGRID_ANIMATION_STEPS_END,
};

/// Each door's opening and the cell it is drawn at, by its exit's number.
struct MapGridCellAnimation gYallam_Doors[] = {
    {gYallam_DoorOpening, 98, 19},
    {gYallam_SecondDoorOpening, 92, 20},
    {gYallam_ThirdDoorOpening, 84, 18},
    {gYallam_DoorOpening, 79, 7},
    {gYallam_ThirdDoorOpening, 80, 16},
    {gYallam_ThirdDoorOpening, 77, 17},
    {gYallam_SecondDoorOpening, 99, 8},
    {gYallam_SanctumDoorOpening, 90, 6},
};

/// The village: its exits and doors, its people, the Cyclone spots and the log.
struct Trigger gYallam_Triggers[] = {
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), YALLAM_TO_YALLAM_ROOMS_INN_SOUTHEAST, CONDITION_ALWAYS, EVENT(Yallam_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), YALLAM_TO_YALLAM_ROOMS_INN_SOUTH, CONDITION_ALWAYS, EVENT(Yallam_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), YALLAM_TO_YALLAM_ROOMS_SHOPS_EAST, CONDITION_ALWAYS, EVENT(Yallam_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), YALLAM_TO_YALLAM_ROOMS_INN_WEST, CONDITION_ALWAYS, EVENT(Yallam_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), YALLAM_TO_YALLAM_ROOMS_SHOPS_NORTH, CONDITION_ALWAYS, EVENT(Yallam_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), YALLAM_TO_YALLAM_ROOMS_SHOPS_WEST, CONDITION_ALWAYS, EVENT(Yallam_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), YALLAM_TO_YALLAM_ROOMS_INN_NORTHEAST, CONDITION_ALWAYS, EVENT(Yallam_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), YALLAM_TO_MIKASALLA_ROOMS_SANCTUM, CONDITION_ALWAYS, EVENT(Yallam_EnterDoor)},
    {TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_NORTH, YALLAM_TO_YALLAM_CAVE_EAST, CONDITION_ALWAYS, YALLAM_TO_YALLAM_CAVE_EAST},
    {TRIGGER_EXIT, YALLAM_TO_WORLD_MAP_SOUTH, CONDITION_ALWAYS, YALLAM_TO_WORLD_MAP_SOUTH},
    {TRIGGER_EXIT, YALLAM_TO_WORLD_MAP_NORTH, CONDITION_ALWAYS, YALLAM_TO_WORLD_MAP_NORTH},
    {TRIGGER_EXIT, YALLAM_TO_SONG_GAME, CONDITION_ALWAYS, YALLAM_TO_SONG_GAME},
    {TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_SOUTH, YALLAM_TO_YALLAM_CAVE_WEST, CONDITION_ALWAYS, YALLAM_TO_YALLAM_CAVE_WEST},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), CELL_WEEDS_OVER_CAVE_HOLE, CONDITION_ALWAYS, EVENT(Yallam_UncoverCaveHole)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), CELL_WEEDS_BELOW_SHOPS, CONDITION_ALWAYS, EVENT(Yallam_ScatterWeedsBelowShops)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), CELL_WEEDS_OVER_ANTIDOTE, CONDITION_ALWAYS, EVENT(Yallam_UncoverAntidote)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), CELL_WEEDS_IN_CORNER, CONDITION_ALWAYS, EVENT(Yallam_ScatterWeedsInCorner)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), CELL_WEEDS_OVER_NUT, CONDITION_ALWAYS, EVENT(Yallam_UncoverNut)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), CELL_WEEDS_BY_SANCTUM, CONDITION_ALWAYS, EVENT(Yallam_ScatterWeedsBySanctum)},
    {TRIGGER_TALK, ACTOR_VILLAGER_FEW_VISITORS, CONDITION_ALWAYS, MSGNO(MsgYallamDoesnT)},
    {TRIGGER_TALK, ACTOR_VILLAGER_YEPP_DIED, CONDITION_ALWAYS, MSGNO(MsgOurMasterBoatsman)},
    {TRIGGER_TALK, ACTOR_VILLAGER_IF_YEPP_LIVED, CONDITION_ALWAYS, MSGNO(MsgIfYeppWere)},
    {TRIGGER_TALK, ACTOR_VILLAGER_PEARL_BED, CONDITION_ALWAYS, MSGNO(MsgYeppFoundThis)},
    {TRIGGER_TALK, ACTOR_VILLAGER_PEARL_DIVERS, CONDITION_ALWAYS, MSGNO(MsgThereUsedTo)},
    {TRIGGER_TALK, ACTOR_VILLAGER_LOST_YEPP, CONDITION_ALWAYS, MSGNO(MsgWhenWeLost)},
    {TRIGGER_TALK, ACTOR_BOATSMAN_TAUGHT_BY_YEPP, CONDITION_ALWAYS, MSGNO(MsgYeppWasThe)},
    {TRIGGER_TALK, ACTOR_BOATSMAN_TAUGHT_LITTLE, CONDITION_ALWAYS, MSGNO(MsgYeppToldMe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_FEW_VISITORS, CONDITION_ALWAYS, MSGNO(MsgYeppWasTheOne)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_YEPP_DIED, CONDITION_ALWAYS, MSGNO(MsgSingingWithYepp)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_IF_YEPP_LIVED, CONDITION_ALWAYS, MSGNO(MsgThosePearlsYepp)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_PEARL_BED, CONDITION_ALWAYS, MSGNO(MsgYeppNeverTold)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_PEARL_DIVERS, CONDITION_ALWAYS, MSGNO(MsgYeppWasAmazing)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_LOST_YEPP, CONDITION_ALWAYS, MSGNO(MsgSunshineIsA)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOATSMAN_TAUGHT_BY_YEPP, CONDITION_ALWAYS, MSGNO(MsgIVeSearched)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOATSMAN_TAUGHT_LITTLE, CONDITION_ALWAYS, MSGNO(MsgYeppAlwaysSang)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_FORCE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_LOG_STUMP, FLAG_YALLAM_LOG_FALLEN, (s32)(Yallam_DoNothing)},
    {PSYNERGY_ON_ACTOR(ABILITY_FORCE), ACTOR_LOG_STUMP, FLAG_YALLAM_LOG_FALLEN, EVENT(Yallam_ForceLogDown)},
    {TRIGGER_TABLE_END},
};

/// The cave's exits.
struct Trigger gYallam_CaveTriggers[] = {
    {TRIGGER_EXIT, YALLAM_CAVE_TO_YALLAM_EAST, CONDITION_ALWAYS, YALLAM_CAVE_TO_YALLAM_EAST},
    {TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_NORTH, YALLAM_CAVE_TO_YALLAM_WEST, CONDITION_ALWAYS, YALLAM_CAVE_TO_YALLAM_WEST},
    {TRIGGER_TABLE_END},
};
// clang-format on

/// The song game's exit and its children.
struct Trigger gYallam_SongGameTriggers[] = {
    {TRIGGER_EXIT, YALLAM_EXIT_SONG_GAME, CONDITION_ALWAYS,
     YALLAM_EXIT_SONG_GAME},
    {TALK_TRIGGER_FACING(FACING_EAST), ACTOR_SONG_LEADER, CONDITION_ALWAYS,
     EVENT(Yallam_OfferYeppSong)},
    {TALK_TRIGGER_FACING(FACING_SOUTHEAST), ACTOR_SONG_LEADER, CONDITION_ALWAYS,
     EVENT(Yallam_OfferYeppSong)},
    {TALK_TRIGGER_FACING(FACING_NORTHEAST), ACTOR_SONG_LEADER, CONDITION_ALWAYS,
     EVENT(Yallam_OfferYeppSong)},
    {TALK_TRIGGER_FACING(FACING_NORTH), ACTOR_SONG_LEADER, CONDITION_ALWAYS,
     EVENT(Yallam_OfferYeppSong)},
    {TALK_TRIGGER_FACING(FACING_SOUTH), ACTOR_SONG_LEADER, CONDITION_ALWAYS,
     EVENT(Yallam_OfferYeppSong)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SONG_LEADER, CONDITION_ALWAYS,
     MSGNO(MsgYeppSSongs)},
    {TRIGGER_TALK, ACTOR_CIRCLES_CHILD, CONDITION_ALWAYS,
     EVENT(Yallam_TalkAboutSongCircles)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CIRCLES_CHILD,
     CONDITION_ALWAYS, EVENT(Yallam_TalkAboutSongCircles)},
    {TRIGGER_TALK, ACTOR_GAME_CHILD, CONDITION_ALWAYS,
     EVENT(Yallam_TalkAboutSongGame)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GAME_CHILD, CONDITION_ALWAYS,
     EVENT(Yallam_TalkAboutSongGame)},
    {TRIGGER_TALK, ACTOR_STORY_CHILD, CONDITION_ALWAYS, MSGNO(MsgYeppWroteAll)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STORY_CHILD, CONDITION_ALWAYS,
     MSGNO(MsgTheStoryIs)},
    {TRIGGER_TABLE_END},
};
