/// @file
/// Eastern Alhafra, the harbour east of the town, where the sailing ship
/// Briggs's Champa crew bought lies at the end of a pier with its broken mast
/// fallen across the water to the eastern dock, the sail spread under it and
/// a huge block of stone pinning its tip. On the first visit the party, coming
/// aboard, overhears the sailors at the mast give up
/// (EasternAlhafra_OverhearWorkers), then catches Briggs below decks
/// (eastern_alhafra_ship.c). Coming out again, Kraden asks whether the party
/// means to help (EasternAlhafra_AskToHelpWithMast), and the mast is cleared
/// as a puzzle: the rocks pushed off the sail's poles into the water
/// (EasternAlhafra_FinishPushing), the log rolled in (EasternAlhafra_RollLog),
/// the crates Moved in as stepping stones, the stake on the cliff Pounded to
/// free the ropes (EasternAlhafra_PoundStake), and at last, once the party has
/// Burst from Tundaria Tower, the block blasted apart
/// (EasternAlhafra_BurstMastBlock). Two workers see the block gone and run for
/// the mayor (EasternAlhafra_WorkersSeeBlockGone), who comes down the pier to
/// look and invites the party to his manor (EasternAlhafra_WorkersTellMayor),
/// where Briggs escapes. Back at the pier, Briggs sails off in the ship
/// (EasternAlhafra_BriggsStealsShip) and the mayor blames everyone
/// (EasternAlhafra_MayorFindsShipGone). The harbour's water ripples all the
/// while (EasternAlhafra_StartWave).
#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "system/scene.h"
#include "flags.h"
#include "field/scene_tables.h"
#include "system/main_routine_state.h"
#include "field/event_inline.h"
#include "scene/scene.h"
#include "field/particle.h"
#include "system/ram_buffer.h"
#include "field/field_sequence_event.h"
#include "field/actor.h"
#include "system/io_reg.h"
#include "system/dma.h"
#include "field/map_grid.h"
#include "system/frame.h"
#include "field/call.h"
#include "field/field_sprite.h"
#include "scene_ids.h"
#include "system/task.h"
#include "system/heap_release.h"
#include "window/item_icon.h"
#include "graphics/resource.h"
#include "system/vector3.h"
#include "field/field_event.h"
#include "field/actor_script.h"
#include "fixed_math.h"
#include "scene/falling_rocks.h"
#include "shared/tables.h"
#include "graphics/palette_buffers.h"
#include "macros.h"

/// Story flags of the mast puzzle and its events.
#define FLAG_DOCK_CRATE_SUNK 0x8a0 ///< ACTOR_DOCK_CRATE
#define FLAG_SAIL_CRATE_SUNK 0x8a1 ///< ACTOR_SAIL_CRATE
#define FLAG_EASTERN_ALHAFRA_STAKE_POUNDED 0x8a2
#define FLAG_EASTERN_ALHAFRA_ROPE_LASHED 0x8a3
#define FLAG_ROCK_UNDER_LOG_SUNK 0x8a4 ///< ACTOR_ROCK_UNDER_LOG
#define FLAG_EAST_POLE_ROCK_SUNK 0x8a5 ///< ACTOR_EAST_POLE_ROCK
#define FLAG_LOG_ROLLED 0x8a6
#define FLAG_TALKED_TO_SAILOR 0x8a8
#define FLAG_OVERHEARD_WORKERS 0x8a9
#define FLAG_ASKED_TO_HELP 0x8ac
#define FLAG_ASKED_IF_FINISHED 0x8ad
#define FLAG_REACHED_MAST_BLOCK 0x8be
#define FLAG_GAVE_UP_ON_MAST_BLOCK 0x8bf
#define FLAG_WORKERS_SAW_BLOCK_GONE 0x904

/// Where the party is brought back to the docks once Briggs has taken the
/// ship (EasternAlhafra_BriggsStealsShip), the workers knocked down.
enum { ENTRANCE_AFTER_SHIP_STOLEN = 99 };
/// Alhafra Manor's entrance where the party arrives as Briggs escapes, as
/// alhafra_manor.c names it.
enum { ENTRANCE_MANOR_BRIGGS_ESCAPES = 81 };

/// What the map's cells hold for this scene's triggers, besides the two exits
/// (whose cells hold their exit numbers).
enum EasternAlhafraCell {
  /// Under the huge block on the mast's tip, cell (30, 25).
  CELL_MAST_BLOCK = 10,
  /// Beside the log's west end, cells (19, 27) to (19, 31).
  CELL_BESIDE_LOG = 11,
  /// Along the sail's west pole, where the rock under the log rolls, cells
  /// (21, 28) to (21, 31), and the pole's south tip at (21, 32).
  ///@{
  CELL_WEST_POLE = 12,
  CELL_WEST_POLE_TIP = 13,
  ///@}
  /// The deck just southwest of the cabin door, cell (14, 24).
  CELL_BY_CABIN_DOOR = 20,
  /// Halfway down the pier, cells (8, 32) and (9, 32), and at its foot,
  /// (7, 34) to (9, 34).
  ///@{
  CELL_PIER = 21,
  CELL_PIER_FOOT = 22,
  ///@}
  /// Across the deck south of the cabin door, cells (14, 26) to (18, 26).
  CELL_ACROSS_DECK = 23,
  /// Two steps south of the huge block, cell (30, 27), and right beside it,
  /// (30, 26).
  ///@{
  CELL_BELOW_MAST_BLOCK = 24,
  CELL_BESIDE_MAST_BLOCK = 25,
  ///@}
};

/// The dock's things and people before Briggs is caught, numbering the rows of
/// gEasternAlhafra_Placements whose sprites are not the party's; the things
/// keep their rows in gEasternAlhafra_PlacementsAfterBriggsCaught.
enum EasternAlhafraActorId {
  /// The posts Lash ties a rope between (Rope_Place): one at the tip of the
  /// ship's bow, where Lash is cast, and one on the cliff to its east.
  ///@{
  ACTOR_ROPE_POST = ACTOR_FIRST_PLACED,
  ACTOR_SECOND_ROPE_POST,
  ///@}
  /// The two crates pushed or Moved into the water, where each sinks and
  /// stays as a stepping stone Felix hops across: the one on the dock east of
  /// the sail, and the one at the foot of the sail.
  ///@{
  ACTOR_DOCK_CRATE,
  ACTOR_SAIL_CRATE,
  ///@}
  /// The ends of the tightrope from the top of the sail's west pole across to
  /// its east pole (Tightrope_Place).
  ///@{
  ACTOR_TIGHTROPE_START,
  ACTOR_TIGHTROPE_END,
  ///@}
  /// The stake on the cliff top east of the sail, which Pound drives down,
  /// freeing the two ropes it holds taut.
  ACTOR_STAKE,
  /// The log lying across the sail's west pole, rolled into the water
  /// (EasternAlhafra_RollLog).
  ACTOR_LOG,
  /// The two pale round rocks among the junk: one on the sail's west pole just
  /// south of the log, which rolls only once this rock has sunk, and one on the
  /// sail's east pole.
  ///@{
  ACTOR_ROCK_UNDER_LOG,
  ACTOR_EAST_POLE_ROCK,
  ///@}
  /// The two ropes from the sail's east pole to the stake, each from its start
  /// on the pole to its end at the stake, until the stake is Pounded.
  ///@{
  ACTOR_FIRST_STAKE_ROPE_START,
  ACTOR_FIRST_STAKE_ROPE_END,
  ACTOR_SECOND_STAKE_ROPE_START,
  ACTOR_SECOND_STAKE_ROPE_END,
  ///@}
  /// The Champa sailors working on the mast: "It's... It's no use!"
  ACTOR_SAILOR_GIVING_UP,
  /// "We paid big money for this ship. It's ours now..."
  ACTOR_SAILOR_WHO_PAID_FOR_THE_SHIP,
  /// "We sent my pal into town to get some supplies..."
  ACTOR_SAILOR_WAITING_FOR_HIS_PAL,
  /// "If we could just raise the mast, we could head back to Champa..."
  ACTOR_SAILOR_LONGING_FOR_CHAMPA,
  /// Kraden, after the party's own rows, in both tables.
  ACTOR_KRADEN
};

/// The people of gEasternAlhafra_PlacementsAfterBriggsCaught, after the
/// things of the first table.
enum EasternAlhafraAfterBriggsActorId {
  ACTOR_BRIGGS = ACTOR_SAILOR_GIVING_UP,
  ACTOR_CHAUCHA,
  ACTOR_EOLEO,
  /// The Alhafran Mayor: "My prize ship, stolen by a bunch of filthy pirate
  /// scum!"
  ACTOR_ALHAFRAS_MAYOR,
  /// Jiya, the mayor's aide (AlhafraManor's): "Excellent news, Mayor!"
  ACTOR_JIYA = ACTOR_KRADEN + 1,
  /// The workers who find the block on the mast gone and tell the mayor,
  /// running by gEasternAlhafra_FirstWorkerCheerScript and the second's.
  ///@{
  ACTOR_FIRST_WORKER,
  ACTOR_SECOND_WORKER,
  ///@}
  /// The Alhafrans Briggs's pirates knock down as they take the ship:
  /// "Briggs's pirates... They attacked us..."
  ACTOR_YOUNG_MAN_KNOCKED_DOWN,
  /// "I just came down to see the repaired mast, and they jumped..."
  ACTOR_MAN_KNOCKED_DOWN
};

/// A townsperson's own animation 13: lying flat on the ground, as the people
/// Briggs's pirates knocked down lie at the pier's foot.
enum { ANIM_LYING_DOWN = 13 };

/// The junk's own animations: the log (SPRITE_LOG_WEST_EAST) rolling as it is
/// pushed south, dropping into the water with a splash, and bobbing there; a
/// crate (SPRITE_TALL_CRATE) settling into the water, and sunk to its lid (a
/// white rock's sprite has neither: a rock is gone once sunk).
enum EasternAlhafraJunkAnimation {
  ANIM_LOG_ROLL_SOUTH = 3,
  ANIM_LOG_FLOAT = 4,
  ANIM_LOG_SPLASH_DOWN = 8,
  ANIM_CRATE_SUNK = 2,
  ANIM_CRATE_SINK = 4
};

/// Briggs's own animations as he taunts the party from the ship: raising his
/// hands to his face, then sticking his tongue out (also the two at once, and
/// slower), and wagging his head from side to side.
enum EasternAlhafraBriggsAnimation {
  ANIM_BRIGGS_PULL_FACE = 5,
  ANIM_BRIGGS_TONGUE_OUT = 6,
  ANIM_BRIGGS_HANDS_TO_FACE = 7,
  ANIM_BRIGGS_WAG_HEAD = 8,
  ANIM_BRIGGS_PULL_FACE_SLOWLY = 9
};

/// The Alhafran Mayor's own animation 5: shaking his head again and again.
enum { ANIM_MAYOR_KEEP_SHAKING_HEAD = 5 };

struct Actor;
struct EffectSprite;
struct Vector3;

/// A push, as PushTarget_FindStop works it out: where the pushed actor comes
/// to rest, how big it is and which actor it is.
struct PushTarget {
  /// Where it stops, in 16.16 fixed-point pixels: as far as it slides before
  /// it would run into something, its height as it stood.
  FIXED x, y, z;
  s32 rectangle; ///< its footprint, a row of gPushRectangles
  s32 id;        ///< the pushed actor
  /// Run as the leader follows it; PushTarget_FindStop leaves none.
  void *callback;
};

void EasternAlhafra_PlaceClearedDebris(void);

/// A party member walks to the leader and is gone, back in the party.
static inline void Actor_RejoinLeader(s32 id) {
  struct Actor *leader;

  Actor_SetSpeed(id, 1.2);
  Actor_SetAnimation(id, ANIM_WALK);
  leader = Actor_GetById(gMainRoutineState.leaderActorId);
  if (leader != 0) {
    Actor_SetDestination(id, leader->x.part[0].integer,
                         leader->z.part[0].integer);
  }
  FieldActor_WaitForMoveById(id);
  Actor_SetPosition(id, 0, 0);
}

void Rock_Spawn(s32 x, s32 y, s32 z, s32 count);

void EasternAlhafra_StartFallingRocks(void);
void Rock_EndFalling(void);

void EasternAlhafra_AskIfFinished(void);
void EasternAlhafra_WorkersSeeBlockGone(void);
void EasternAlhafra_MayorFindsShipGone(void);

extern s32 gEasternAlhafra_FirstWorkerCheerScript[];
extern s32 gEasternAlhafra_SecondWorkerCheerScript[];
extern u32 gEasternAlhafra_FirstWorkerRunScript[];
extern u32 gEasternAlhafra_SecondWorkerRunScript[];
extern s32 gEasternAlhafra_RunOffScript[];

/// Sets up a falling rock's width by height sprite of the given shape at its
/// first tile.
void EasternAlhafra_InitializeRockSprite(struct EffectSprite *sprite, s32 width,
                                         s32 height, u32 attributes, s32 tile);

void Particle_UpdateFalling(void);

extern u16 gPushKinds[];

struct Actor *PushTarget_FindAhead(s32 *direction, s32 *number, s32 *kind);

s32 MapGrid_CheckCollision(struct Actor *actor, struct Point *position);

void Actor_StopAtPushTarget(struct Actor *actor);

struct Actor *Actor_FindAtCell(s32 *position, s32 mode);

void Actor_SetCellOpen(s32 id, u32 kind, s32 flag, s32 layer);

s32 Actor_GetCellIndex(struct Actor *actor);

s32 Tightrope_Place();
s32 Tightrope_Release();
s32 Leader_CanPushObjectAhead();
/// Leader_PushObjectAhead through this overlay's own veneer: pushes the object
/// ahead of the leader on by a cell.
s32 EasternAlhafra_PushObjectAhead();
/// The harbour water's two tables of BG1 offsets, a halfword a screen line: one
/// an HBlank DMA reads while the other is filled for the next frame.
u16 gEasternAlhafra_WaveLines[2][DISPLAY_HEIGHT] BSS_DATA ALIGNED(16);
void Actor_FallOverWithAnimationById(s32 id, u32 angle, s32 animation);
void Rope_Initialize(void);
void Tightrope_Initialize(void);
void MapGrid_SetCellAttributesAt();

extern u16 gEasternAlhafra_PoundStakes[];
extern struct ActorPlacement gEasternAlhafra_Placements[];
extern struct ActorPlacement gEasternAlhafra_PlacementsAfterBriggsCaught[];

s32 Actor_HasArrived();

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

void *Scene_GetActorPlacements(void) {
  if (Event_GetFlag(FLAG_BRIGGS_CAUGHT))
    return gEasternAlhafra_PlacementsAfterBriggsCaught;
  return gEasternAlhafra_Placements;
}

/// Felix casts Burst on the huge block pinning the mast's tip, at the east
/// end of the mast by the dock (cell (30, 25)), and it is blasted apart. Each
/// small blast (mode 2) throws two chips of rock from it and the great one
/// (mode 3) thirty; as the blasts die down (mode 1) the mast is clear.
void EasternAlhafra_BurstMastBlock(s32 mode, s32 frame) {
  if (frame == 0)
    EasternAlhafra_StartFallingRocks();
  if (mode == 1) {
    Event_SetFlag(FLAG_ALHAFRA_MAST_CLEARED);
    EasternAlhafra_PlaceClearedDebris();
  }
  if (mode == 2)
    Rock_Spawn(toFIXED(498), toFIXED(16), toFIXED(412), 2);
  if (mode == 3)
    Rock_Spawn(toFIXED(498), toFIXED(16), toFIXED(412), 30);
  if (frame == PSYNERGY_EVENT_LAST_FRAME)
    Rock_EndFalling();
}

/// Felix Pounds the stake on the cliff top east of the sail: it sinks into the
/// ground, and the two ropes it held taut fall slack onto the sail's east pole.
/// The ropes are redrawn each frame, for up to a second, until both their
/// loose ends have arrived.
void EasternAlhafra_PoundStake(s32 mode, s32 frame) {
  struct Actor *firstEnd;
  struct Actor *secondEnd;
  s32 frames;

  Pound_SinkTarget(gEasternAlhafra_PoundStakes, mode, frame);
  if (mode == POUND_STAGE_LANDED) {
    firstEnd = Actor_Get(ACTOR_FIRST_STAKE_ROPE_END);
    secondEnd = Actor_Get(ACTOR_SECOND_STAKE_ROPE_END);
    firstEnd->speedLimit = toFIXED(4);
    firstEnd->acceleration = toFIXED(1) / 2;
    secondEnd->acceleration = toFIXED(1) / 2;
    secondEnd->speedLimit = toFIXED(4);
    FieldActor_SetPositionAndResetMotion(firstEnd, toFIXED(488),
                                         firstEnd->y.fixed, toFIXED(312));
    FieldActor_SetMoveTarget(firstEnd, 440, 0, 312);
    FieldActor_SetPositionAndResetMotion(secondEnd, toFIXED(488),
                                         secondEnd->y.fixed, toFIXED(312));
    FieldActor_SetMoveTarget(secondEnd, 440, 0, 376);
    Tightrope_Release(1);
    Tightrope_Release(2);
    frames = 0;
    do {
      Tightrope_Place(1, ACTOR_FIRST_STAKE_ROPE_START,
                      ACTOR_FIRST_STAKE_ROPE_END);
      Tightrope_Place(2, ACTOR_SECOND_STAKE_ROPE_START,
                      ACTOR_SECOND_STAKE_ROPE_END);
      System_Wait(1);
      Tightrope_Release(1);
      Tightrope_Release(2);
      if (Actor_HasArrived(firstEnd) != 0 && Actor_HasArrived(secondEnd) != 0)
        break;
      frames++;
    } while (frames < 60);
  }
}

/// Nothing visible happens as Felix begins to push or Move one of the crates
/// or rocks of the mast's junk; the game only lets it keep its height as it
/// moves, instead of following the ground under it. When he stands just west of
/// the dock crate, on its first two cells, his own cell is walled until
/// EasternAlhafra_FinishPushing, so that a pull of Move cannot drag the crate
/// onto him.
void EasternAlhafra_StartPushing(s32 unused, s32 id) {
  struct Actor *leader;
  struct Actor *actor;
  s32 leaderX;
  s32 x;
  s32 z;

  leader = Actor_Get(gMainRoutineState.leaderActorId);
  actor = Actor_Get(id);
  actor->motionFlags = 0;
  if (id == ACTOR_DOCK_CRATE) {
    leaderX = FIELD_CELL(leader->x.fixed);
    x = FIELD_CELL(actor->x.fixed);
    z = FIELD_CELL(actor->z.fixed);
    if (x == 34 && z == 32 && leaderX == 33)
      FieldMapGrid_SetCellBehavior(toFIXED(528), toFIXED(512), 2,
                                   CELL_BEHAVIOR_WALL);
    if (x == 33 && z == 32 && leaderX == 32)
      FieldMapGrid_SetCellBehavior(toFIXED(512), toFIXED(512), 2,
                                   CELL_BEHAVIOR_WALL);
  }
}

#include "text/msg_ids.h"

extern struct MapGridCellAnimation gEasternAlhafra_ShipDoor;

s32 Party_FindOwnerWithAction();
void Event_Start();
void Event_End();

/// A crate or rock that Felix has pushed or Moved off the junk into the water
/// falls in and sinks with three splashes. Each has its own spot: the dock
/// crate at cell (31, 32), the crate at the foot of the sail at (25, 33), the
/// rock under the log at (21, 33) and the east pole's rock at (27, 33). A
/// crate stays there just under the water as a stepping stone Felix can hop
/// across; a rock is gone. Stopping anywhere else, nothing happens.
void EasternAlhafra_FinishPushing(s32 mode, s32 id) {
  struct Actor *actor;
  struct Actor *splashes[4];
  struct Actor *splash;
  s32 x;
  s32 z;
  s32 sinks;
  s32 i;

  actor = Actor_Get(id);
  x = FIELD_CELL(actor->x.fixed);
  sinks = 0;
  z = FIELD_CELL(actor->z.fixed);
  if (id == ACTOR_DOCK_CRATE) {
    FieldMapGrid_SetCellBehavior(toFIXED(528), toFIXED(512), 2, 0);
    FieldMapGrid_SetCellBehavior(toFIXED(512), toFIXED(512), 2, 0);
  }
  if (x == 31 && z == 32) {
    Event_SetFlag(FLAG_DOCK_CRATE_SUNK);
    sinks = 1;
  }
  if (x == 25 && z == 33) {
    Event_SetFlag(FLAG_SAIL_CRATE_SUNK);
    sinks = 1;
  }
  if (x == 21 && z == 33) {
    Event_SetFlag(FLAG_ROCK_UNDER_LOG_SUNK);
    sinks = 1;
  }
  if (x == 27 && z == 33) {
    Event_SetFlag(FLAG_EAST_POLE_ROCK_SUNK);
    sinks = 1;
  }
  if (sinks) {
    if (id != ACTOR_DOCK_CRATE)
      Event_PlaySound(SE_FALL);
    actor->gravity = toFIXED(9) / 100;
    actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
    Actor_SetSpriteFlags(Actor_Get(id), SPRITE_FLAG_SHADOW);
    System_Wait(5);
    for (i = 0; i < 60 && actor->y.fixed > 0; i++)
      System_Wait(1);
    Event_PlaySound(SE_STOP_EXIT_SE);
    System_Wait(1);
    Event_PlaySound(SE_LOG_FALL);
    actor->collisionFlags = 0;
    actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    Actor_SelectAnimation(actor, ANIM_CRATE_SINK);
    for (i = 0; i <= 2; i++) {
      splash = (FieldActor_Create(SPRITE_SPLASH, actor->x.fixed, actor->y.fixed,
                                  actor->z.fixed));
      splashes[i] = splash;
      splash->scaleX = FIXED_ONE * 13 / 20;
      Actor_SelectAnimation(splash, 1);
      Actor_SetSpriteFlags(splash, 0);
      Event_PlaySound(SE_CAST);
      System_Wait(5);
    }
    EasternAlhafra_PlaceClearedDebris();
    FieldActor_Stop(gMainRoutineState.leaderActorId);
    System_Wait(30);
    for (i = 0; i <= 2; i++)
      Actor_Release(splashes[i]);
  }
}

/// Nothing visible happens as Felix begins to push the rock under the log
/// along the sail's west pole. The game walls the cells on either side of the
/// pole, so that the rock can only roll along it, and opens the water at the
/// pole's south tip (cell (21, 33)) for it to fall into.
void EasternAlhafra_StartPushingRockUnderLog(s32 unused, s32 id) {
  EasternAlhafra_StartPushing(unused, id);
  FieldMapGrid_SetCellBehavior(toFIXED(320), toFIXED(464), 2,
                               CELL_BEHAVIOR_WALL);
  FieldMapGrid_SetCellBehavior(toFIXED(320), toFIXED(480), 2,
                               CELL_BEHAVIOR_WALL);
  FieldMapGrid_SetCellBehavior(toFIXED(320), toFIXED(496), 2,
                               CELL_BEHAVIOR_WALL);
  FieldMapGrid_SetCellBehavior(toFIXED(320), toFIXED(512), 2,
                               CELL_BEHAVIOR_WALL);
  FieldMapGrid_SetCellBehavior(toFIXED(352), toFIXED(464), 2,
                               CELL_BEHAVIOR_WALL);
  FieldMapGrid_SetCellBehavior(toFIXED(352), toFIXED(480), 2,
                               CELL_BEHAVIOR_WALL);
  FieldMapGrid_SetCellBehavior(toFIXED(352), toFIXED(496), 2,
                               CELL_BEHAVIOR_WALL);
  FieldMapGrid_SetCellBehavior(toFIXED(352), toFIXED(512), 2,
                               CELL_BEHAVIOR_WALL);
  FieldMapGrid_SetCellBehavior(toFIXED(336), toFIXED(528), 2, 0);
}

/// The rock under the log stops after a push along the sail's west pole. If it
/// has come off the pole's south tip into the water (cell (21, 33)), the
/// camera turns to watch it sink and comes back to Felix. The walls beside the
/// pole come down again.
void EasternAlhafra_FinishPushingRockUnderLog(s32 mode, s32 id) {
  struct Actor *actor;
  s32 x;
  s32 z;
  s32 open;

  actor = Actor_Get(id);
  x = FIELD_CELL(actor->x.fixed);
  z = FIELD_CELL(actor->z.fixed);
  open = 0;
  if (x == 21 && z == 33)
    open = 1;
  FieldMapGrid_SetCellBehavior(toFIXED(320), toFIXED(464), 2, 0);
  FieldMapGrid_SetCellBehavior(toFIXED(320), toFIXED(480), 2, 0);
  FieldMapGrid_SetCellBehavior(toFIXED(320), toFIXED(496), 2, 0);
  FieldMapGrid_SetCellBehavior(toFIXED(320), toFIXED(512), 2, 0);
  FieldMapGrid_SetCellBehavior(toFIXED(352), toFIXED(464), 2, 0);
  FieldMapGrid_SetCellBehavior(toFIXED(352), toFIXED(480), 2, 0);
  FieldMapGrid_SetCellBehavior(toFIXED(352), toFIXED(496), 2, 0);
  FieldMapGrid_SetCellBehavior(toFIXED(352), toFIXED(512), 2, 0);
  FieldMapGrid_SetCellBehavior(toFIXED(336), toFIXED(528), 2,
                               CELL_BEHAVIOR_WALL);
  if (open)
    FieldCamera_MoveTo(336, CAMERA_KEEP, 560, 1);
  EasternAlhafra_FinishPushing(mode, id);
  if (open)
    EventCamera_FollowActor(gMainRoutineState.leaderActorId, 1);
}

void EasternAlhafra_OverhearWorkers(void);
void PushTarget_Push(struct PushTarget target);

/// Felix walks into the log lying across the sail's west pole from its west
/// end (cells (19, 27) to (19, 31)). Once the rock under the log has sunk,
/// pushing it south: the camera pans down the pole, the log rolls south off it
/// and drops into the water with a splash, where it floats by the ship's
/// stern, and the camera comes back to Felix, the way along the pole open.
/// Pushing it north instead takes the ordinary push (PushTarget_Push).
/// Nothing visible happens when he walks into it, from the north or the south,
/// before the rock has sunk: the log does not move.
void EasternAlhafra_RollLog(void) {
  struct Actor *log;
  struct Actor *rock;
  struct PushTarget target;
  s32 facing;
  s32 frames;

  log = Actor_GetById(ACTOR_LOG);
  rock = Actor_GetById(ACTOR_ROCK_UNDER_LOG);
  // The cardinal heading nearest the leader's facing.
  facing = FACING_QUARTER(
      Actor_GetById(gMainRoutineState.leaderActorId)->facing + DEGtoANG(45));
  if (rock != NULL && Event_GetFlag(FLAG_ROCK_UNDER_LOG_SUNK) == 0)
    MapGrid_SetCellBehavior(rock->x.fixed, rock->z.fixed, 2,
                            CELL_BEHAVIOR_WALL);
  if (PushTarget_FindStop(&target) != 0) {
    if (Event_GetFlag(FLAG_ROCK_UNDER_LOG_SUNK) == 0 ||
        facing == FACING_NORTH) {
      PushTarget_Push(target);
    } else {
      Event_SetFlag(FLAG_LOG_ROLLED);
      Event_Start();
      FieldEvent_EndLastingPsynergy(0);
      FieldCamera_MoveTo(336, CAMERA_KEEP, 560, 1);
      PushTarget_Push(target);
      MapGrid_SetCellBehavior(log->x.fixed - toFIXED(24), log->z.fixed, 0, 0);
      MapGrid_SetCellBehavior(log->x.fixed + toFIXED(8), log->z.fixed, 0, 0);
      MapGrid_SetCellBehavior(log->x.fixed - toFIXED(24), log->z.fixed, 2, 0);
      MapGrid_SetCellBehavior(log->x.fixed - toFIXED(8), log->z.fixed, 2, 0);
      MapGrid_SetCellBehavior(log->x.fixed + toFIXED(8), log->z.fixed, 2, 0);
      MapGrid_SetCellBehavior(log->x.fixed + toFIXED(24), log->z.fixed, 2, 0);
      Actor_SetAnimation(ACTOR_LOG, ANIM_LOG_ROLL_SOUTH);
      Actor_SetDestinationOffset(ACTOR_LOG, 0, 16);
      FieldEvent_Wait(20);
      log->gravity = toFIXED(1) / 20;
      log->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
      frames = 0;
      if (log->y.fixed > 0) {
        for (; frames < 60 && log->y.fixed > 0; frames++) {
          System_Wait(1);
        }
      }
      EasternAlhafra_PlaceClearedDebris();
      Event_PlaySound(SE_LOG_FALL);
      Actor_SetAnimation(ACTOR_LOG, ANIM_LOG_SPLASH_DOWN);
      FieldEvent_Wait(60);
      EventCamera_FollowActor(gMainRoutineState.leaderActorId, 1);
      Event_End();
    }
  }
  if (rock != NULL && Event_GetFlag(FLAG_ROCK_UNDER_LOG_SUNK) == 0)
    MapGrid_SetCellBehavior(rock->x.fixed, rock->z.fixed, 2, 0);
}

/// Felix walks north or south into the rock under the log, along the sail's
/// west pole (cells (21, 28) to (21, 32)), and pushes it a cell along the pole;
/// if the log is ahead of him instead, he pushes the log
/// (EasternAlhafra_RollLog).
void EasternAlhafra_PushAhead(s32 mode) {
  s32 target;

  target = Field_FindNearbyTalkTarget(gMainRoutineState.leaderActorId);
  if (target == ACTOR_ROCK_UNDER_LOG) {
    EasternAlhafra_StartPushingRockUnderLog(mode, ACTOR_ROCK_UNDER_LOG);
    if (Leader_CanPushObjectAhead() != 0)
      EasternAlhafra_PushObjectAhead();
    EasternAlhafra_FinishPushingRockUnderLog(mode, ACTOR_ROCK_UNDER_LOG);
  } else if (target == ACTOR_LOG) {
    EasternAlhafra_RollLog();
  }
}

/// Felix talks to the Champa sailor standing by the log at the edge of the
/// ship, who has given up on the mast. He asks whether the party is new in
/// town and curious about the ship, and tells what a sailing ship is; Mind
/// Read on him before talking starts the same talk.
void EasternAlhafra_TalkToSailor(void) {
  Event_SetFlag(FLAG_TALKED_TO_SAILOR);
  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  FieldEvent_SetMessageId(MsgWhatSYour);
  Actor_ShowEmote(ACTOR_SAILOR_GIVING_UP, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Actor_FaceActor(ACTOR_SAILOR_GIVING_UP, gMainRoutineState.leaderActorId, 0);
  FieldEvent_Wait(20);
  // "What's your problem?"
  FieldEvent_ShowMessageThenWait(ACTOR_SAILOR_GIVING_UP, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_SAILOR_GIVING_UP, 2);
  FieldEvent_Wait(10);
  // "You're new in town, aren't you?" Yes: "You're probably curious about this
  // here ship..." No: "Well, I've never seen you before, but whatever..."
  Event_OpenMessage(ACTOR_SAILOR_GIVING_UP, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(ACTOR_FELIX, 0) != 0) {
    FieldEvent_Wait(30);
    // "Well, I've never seen you before, but whatever..."
    FieldEvent_ShowMessageThenWait(ACTOR_SAILOR_GIVING_UP, 0, 0);
  } else {
    FieldEvent_Wait(20);
    gFieldState->message += 1;
    // "You're probably curious about this here ship, huh?" Yes: "They call this
    // a "sailing" ship... Wind is..." No: "Masted ships like this were
    // originally built..."
    Event_OpenMessage(ACTOR_SAILOR_GIVING_UP, 0);
    if (FieldEvent_AskYesNoWithActorAnimation(ACTOR_FELIX, 0) != 0) {
      FieldEvent_Wait(30);
      // "Masted ships like this were originally built by the Champa... Oh..."
      FieldEvent_ShowMessageThenWait(ACTOR_SAILOR_GIVING_UP, 0, 0);
    } else {
      FieldEvent_Wait(20);
      gFieldState->message += 1;
      // "They call this a "sailing" ship... Wind is the only thing that'll..."
      // Yes: "Yeah, well, think how surprised we were to..." No: "You kids
      // today, with your "been there, done..."
      Event_OpenMessage(ACTOR_SAILOR_GIVING_UP, 0);
      if (FieldEvent_AskYesNoWithActorAnimation(ACTOR_FELIX, 0) != 0) {
        FieldEvent_Wait(30);
        // "You kids today, with your "been there, done that" attitudes...
        // It's..."
        FieldEvent_ShowMessageThenWait(ACTOR_SAILOR_GIVING_UP, 0, 0);
      } else {
        FieldEvent_Wait(20);
        gFieldState->message += 1;
        // "Yeah, well, think how surprised we were to find that Alhafra had..."
        FieldEvent_ShowMessageThenWait(ACTOR_SAILOR_GIVING_UP, 0, 0);
      }
    }
  }
  Actor_FaceDirection(ACTOR_SAILOR_GIVING_UP, FACING_EAST, 0);
  Event_End();
}

/// Felix walks over the deck just southwest of the cabin door (cell
/// (14, 24)). The first time, before Briggs is caught, the party overhears the
/// sailors at the mast (EasternAlhafra_OverhearWorkers). After he is caught,
/// once the dock crate has sunk while no one has Burst, Kraden asks whether
/// the party is finished here (EasternAlhafra_AskIfFinished). Once the block
/// is Burst, two workers see it gone (EasternAlhafra_WorkersSeeBlockGone).
/// Otherwise nothing happens.
void EasternAlhafra_StepByCabinDoor(void) {
  if (Event_GetFlag(FLAG_ALHAFRA_MAST_CLEARED)) {
    if (Event_GetFlag(FLAG_WORKERS_SAW_BLOCK_GONE) == 0) {
      Event_SetFlag(FLAG_WORKERS_SAW_BLOCK_GONE);
      EasternAlhafra_WorkersSeeBlockGone();
    }
  } else if (Event_GetFlag(FLAG_BRIGGS_CAUGHT)) {
    if (Event_GetFlag(FLAG_DOCK_CRATE_SUNK) &&
        Party_FindOwnerWithAction(ABILITY_BURST) == -1 &&
        Event_GetFlag(FLAG_ASKED_IF_FINISHED) == 0) {
      Event_SetFlag(FLAG_ASKED_IF_FINISHED);
      EasternAlhafra_AskIfFinished();
    }
  } else if (Event_GetFlag(FLAG_OVERHEARD_WORKERS) == 0) {
    Event_SetFlag(FLAG_OVERHEARD_WORKERS);
    EasternAlhafra_OverhearWorkers();
  }
}

#include "shared/leader_open_door_and_enter.h"

/// Felix walks north into the cabin door at the north end of the ship's deck
/// (cell (16, 23)); it opens and he goes inside, down into the ship.
void EasternAlhafra_EnterShip(s32 countdown) {
  Leader_OpenDoorAndEnter(&gEasternAlhafra_ShipDoor, countdown, 0);
}

/// Nothing happens when Felix walks diagonally into the log or the rock under
/// it on the sail's west pole (toward the northwest or southeast, or between
/// those and north or south); only walking straight north or south pushes
/// them.
void EasternAlhafra_IgnoreDiagonalStep(void) {}

/// Nothing visible happens when Felix steps up beside the huge block on the
/// mast's tip (cell (30, 26)); the game only notes it, so that walking away
/// again without Burst has Jenna speak (EasternAlhafra_GiveUpOnMastBlock).
void EasternAlhafra_ReachMastBlock(void) {
  GameFlag_Set(FLAG_REACHED_MAST_BLOCK);
}

/// Walking south away from the huge block on the mast (cell (30, 27)) after
/// standing beside it, while no one in the party has Burst, Jenna comes up to
/// Felix: "I don't know what we're going to do about this rock...". She says
/// it once; after that, and once anyone can Burst, nothing happens.
void EasternAlhafra_GiveUpOnMastBlock(void) {
  struct Actor *leader;

  while (Event_GetFlag(FLAG_BRIGGS_CAUGHT) &&
         Event_GetFlag(FLAG_ALHAFRA_MAST_CLEARED) == 0 &&
         Event_GetFlag(FLAG_REACHED_MAST_BLOCK)) {
    if (Party_FindOwnerWithAction(ABILITY_BURST) != -1)
      return;
    Event_SetFlag(FLAG_GAVE_UP_ON_MAST_BLOCK);
    Event_Start();
    FieldEvent_EndLastingPsynergy(0);
    Actor_WalkToAndWait(ACTOR_FELIX, 488, 440);
    FieldActor_LaunchFromLeader(ACTOR_JENNA, -16, 0, FACING_EAST);
    Actor_FaceDirectionAndWait(ACTOR_FELIX, FACING_WEST);
    FieldEvent_Wait(20);
    FieldEvent_SetMessageId(MsgIDonTKnowWhatWe);
    // "I don't know what we're going to do about this rock... I think we've..."
    FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 5);
    Actor_SetAnimation(ACTOR_JENNA, ANIM_WALK);
    leader = Actor_Get(gMainRoutineState.leaderActorId);
    if (leader != NULL)
      Actor_SetDestination(ACTOR_JENNA, leader->x.part[0].integer,
                           leader->z.part[0].integer);
    FieldActor_WaitForMoveById(ACTOR_JENNA);
    Actor_SetPosition(ACTOR_JENNA, 0, 0);
    Event_End();
    return;
  }
}

const struct Trigger *Scene_GetTriggers(void) { return gSceneEvents; }

extern u8 gEasternAlhafra_WaveTableIndex[];

// BG1's wave: each frame the finished table of scanline offsets is handed to
// an HBlank DMA, while the other table is filled for the next frame.

/// Starts DMA 0 copying the finished table into BG1's vertical offset at each
/// HBlank.
void EasternAlhafra_StartWaveDma(void) {
  u16 *line = gEasternAlhafra_WaveLines[gEasternAlhafra_WaveTableIndex[0] ^ 1];
  volatile u16 *channel;

  REG_BG1VOFS = line[0];
  channel = REG_DMA0;
  channel[5] &= ~DMA_START_TIMING_DRQ_REPEAT;
  channel[5] &= ~DMA_ENABLE;
  line++;
  (void)channel[5];
  Dma_Set(line, (void *)&REG_BG1VOFS, DMA_HBLANK_HALFWORD_PER_LINE,
          (volatile u32 *)channel);
}

/// Fills the other table: each line of the screen takes layer 2's vertical
/// scroll plus a sine of up to 2 pixels that moves up the screen a line each
/// frame.
void EasternAlhafra_FillWaveLines(void) {
  struct MapLayer *layer = MapGrid_GetLayer(2);
  u16 *line;
  s32 i = 0;

  line = gEasternAlhafra_WaveLines[gEasternAlhafra_WaveTableIndex[0] ^ 1];
  for (i = 0; i != DISPLAY_HEIGHT; i++) {
    *line++ = FIXEDtoINT(layer->y) +
              FIXEDtoINT_TWICE(Math_Sin(
                  ((u8)gFrameCount - i - FIXEDtoINT(layer->y) + DISPLAY_HEIGHT)
                  << 9));
  }
  gEasternAlhafra_WaveTableIndex[0] ^= 1;
}

/// The harbour's water ripples from the moment the scene starts: each line of
/// the background layer BG1 is shifted up or down by up to 2 pixels along a
/// wave that climbs the screen. A table of the lines is filled each frame and
/// handed to an HBlank DMA in the VBlank pass.
void EasternAlhafra_StartWave(void) {
  Task_AddFrame(EasternAlhafra_FillWaveLines, TASK_PASS_FRAME);
  Task_AddFrame(EasternAlhafra_StartWaveDma, TASK_PASS_VBLANK);
}

void Pillar_Place(const u16 *list);

/// Sets up the harbour: the rippling water; once the workers have fetched the
/// mayor, the ship gone from the pier with all the junk, and until Briggs has
/// sailed off, the people his pirates knocked down lying at the pier's foot;
/// the mayor finding the ship gone, when the party comes back after Briggs
/// sails off; and the junk the party has sunk, the Lashed rope, the ropes to
/// the stake and the stake.
s32 Scene_Initialize(void) {
  struct Actor *actor;
  s32 flag;

  gFieldState->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  EasternAlhafra_StartWave();
  Actor_Get(gMainRoutineState.leaderActorId)->priorityFlags |=
      ACTOR_PRIORITY_IGNORES_RISES;
  if (Event_GetFlag(FLAG_ALHAFRA_MAST_CLEARED) != 0) {
    if (Event_GetFlag(FLAG_WORKERS_TOLD_MAYOR) != 0 &&
        Event_GetFlag(FLAG_ALHAFRA_SHIP_GONE) == 0) {
      Actor_SetPosition(ACTOR_YOUNG_MAN_KNOCKED_DOWN, 184, 616);
      Actor_SetPosition(ACTOR_MAN_KNOCKED_DOWN, 136, 600);
      Actor_SetPosition(ACTOR_FIRST_WORKER, 168, 584);
      Actor_FaceDirection(ACTOR_FIRST_WORKER, FACING_SOUTH, 0);
      Actor_FallOverWithAnimationById(ACTOR_YOUNG_MAN_KNOCKED_DOWN, 0,
                                      ANIM_LYING_DOWN);
      Call3(Actor_FallOverWithAnimationById, ACTOR_MAN_KNOCKED_DOWN,
            FACING_WEST, ANIM_LYING_DOWN);
      Actor_FallOverWithAnimationById(ACTOR_FIRST_WORKER, 0, ANIM_LYING_DOWN);
      Actor_SetPosition(ACTOR_ALHAFRAS_MAYOR, 112, 432);
      Actor_SetPosition(ACTOR_JIYA, 176, 408);
      Actor_SetPosition(ACTOR_SECOND_WORKER, 152, 376);
      Actor_FaceDirection(ACTOR_SECOND_WORKER, FACING_SOUTH, 0);
      FieldActor_SetSpriteRotationById(ACTOR_ALHAFRAS_MAYOR, DEGtoANG(90));
      Actor_SetSpriteFlags(Actor_Get(ACTOR_ALHAFRAS_MAYOR), 0);
      FieldActor_SetSpriteRotationById(ACTOR_JIYA, TRIG_TURN * 3 / 4);
      Actor_SetSpriteFlags(Actor_Get(ACTOR_JIYA), 0);
      FieldActor_SetSpriteRotationById(ACTOR_SECOND_WORKER, DEGtoANG(90));
      Actor_SetSpriteFlags(Actor_Get(ACTOR_SECOND_WORKER), 0);
      Actor_SetAnimation(ACTOR_ALHAFRAS_MAYOR, ANIM_LYING_DOWN);
      Actor_SetAnimation(ACTOR_JIYA, ANIM_LYING_DOWN);
      Actor_SetAnimation(ACTOR_SECOND_WORKER, ANIM_LYING_DOWN);
    }
    if (Event_GetFlag(FLAG_WORKERS_TOLD_MAYOR) != 0) {
      MapGrid_CopyCellsLayered(40, 0, 24, 31, 11, 6);
      MapGrid_CopyCellsLayered(84, 15, 10, 10, 75, 15);
      MapGrid_CopyCellAttributes(10, 24, 1, 1, 11, 24);
      Actor_SetPosition(ACTOR_SECOND_ROPE_POST, 0, 0);
      Actor_SetPosition(ACTOR_ROPE_POST, 0, 0);
      Actor_SetPosition(ACTOR_DOCK_CRATE, 0, 0);
      Actor_SetPosition(ACTOR_SAIL_CRATE, 0, 0);
      Actor_SetPosition(ACTOR_LOG, 0, 0);
    }
  }
  if (gMainRoutineState.entrance == ENTRANCE_AFTER_SHIP_STOLEN) {
    gMainRoutineState.returnEntrance = 1;
    gMainRoutineState.entrance = 1;
    Actor_SetPosition(ACTOR_FELIX, 136, 552);
    Actor_SetPosition(ACTOR_KRADEN, 120, 552);
    Actor_SetPosition(ACTOR_YOUNG_MAN_KNOCKED_DOWN, 184, 616);
    Actor_SetPosition(ACTOR_MAN_KNOCKED_DOWN, 136, 600);
    Actor_SetPosition(ACTOR_FIRST_WORKER, 160, 584);
    FieldActor_SetSpriteRotationById(ACTOR_YOUNG_MAN_KNOCKED_DOWN,
                                     DEGtoANG(90));
    Actor_SetSpriteFlags(Actor_Get(ACTOR_YOUNG_MAN_KNOCKED_DOWN), 0);
    FieldActor_SetSpriteRotationById(ACTOR_MAN_KNOCKED_DOWN, TRIG_TURN * 3 / 4);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_MAN_KNOCKED_DOWN), 0);
    FieldActor_SetSpriteRotationById(ACTOR_FIRST_WORKER, DEGtoANG(90));
    Actor_SetSpriteFlags(Actor_Get(ACTOR_FIRST_WORKER), 0);
    FieldActor_SetSpritePriority(ACTOR_JIYA, 2);
    FieldActor_SetSpritePriority(ACTOR_ALHAFRAS_MAYOR, 2);
    FieldActor_SetSpritePriority(ACTOR_SECOND_WORKER, 2);
    Actor_SetPosition(ACTOR_ALHAFRAS_MAYOR, 160, 392);
    Actor_SetPosition(ACTOR_JIYA, 176, 408);
    Actor_SetPosition(ACTOR_SECOND_WORKER, 152, 376);
    FieldActor_SetSpriteRotationById(ACTOR_JIYA, TRIG_TURN * 3 / 4);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_JIYA), 0);
    FieldActor_SetSpriteRotationById(ACTOR_SECOND_WORKER, DEGtoANG(90));
    Actor_SetSpriteFlags(Actor_Get(ACTOR_SECOND_WORKER), 0);
    Actor_FaceDirection(ACTOR_ALHAFRAS_MAYOR, FACING_NORTH_NORTHEAST, 0);
    Actor_FaceDirection(ACTOR_SECOND_WORKER, FACING_SOUTH, 0);
    Actor_SetAnimation(ACTOR_JIYA, ANIM_LYING_DOWN);
    Actor_SetAnimation(ACTOR_SECOND_WORKER, ANIM_LYING_DOWN);
    Actor_SetAnimation(ACTOR_YOUNG_MAN_KNOCKED_DOWN, ANIM_LYING_DOWN);
    Actor_SetAnimation(ACTOR_MAN_KNOCKED_DOWN, ANIM_LYING_DOWN);
    Actor_SetAnimation(ACTOR_FIRST_WORKER, ANIM_LYING_DOWN);
    EasternAlhafra_MayorFindsShipGone();
  }
  EasternAlhafra_PlaceClearedDebris();
  if (Event_GetFlag(FLAG_LOG_ROLLED) == 0)
    PushTarget_FillRectangle(ACTOR_LOG);
  Rope_Initialize();
  Rope_Place(0, FLAG_EASTERN_ALHAFRA_ROPE_LASHED, ACTOR_ROPE_POST,
             ACTOR_SECOND_ROPE_POST);
  Actor_Get(ACTOR_ROPE_POST)->priorityFlags |= ACTOR_PRIORITY_IGNORES_RISES;
  Actor_Get(ACTOR_SECOND_ROPE_POST)->priorityFlags |=
      ACTOR_PRIORITY_IGNORES_RISES;
  Tightrope_Initialize();
  Tightrope_Place(0, ACTOR_TIGHTROPE_START, ACTOR_TIGHTROPE_END);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_TIGHTROPE_START), 0);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_TIGHTROPE_END), 0);
  Pillar_Place(gEasternAlhafra_PoundStakes);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_STAKE), 0);
  flag = Event_GetFlag(FLAG_EASTERN_ALHAFRA_STAKE_POUNDED);
  if (flag == 0) {
    actor = Actor_Get(ACTOR_FIRST_STAKE_ROPE_START);
    actor->collisionFlags |= ACTOR_NOT_TARGETED;
    actor = Actor_Get(ACTOR_FIRST_STAKE_ROPE_END);
    actor->motionFlags = flag;
    actor->y.fixed = toFIXED(16);
    Tightrope_Place(1, ACTOR_FIRST_STAKE_ROPE_START,
                    ACTOR_FIRST_STAKE_ROPE_END);
    Tightrope_Settle(1);
    actor = Actor_Get(ACTOR_SECOND_STAKE_ROPE_START);
    actor->collisionFlags |= ACTOR_NOT_TARGETED;
    actor = Actor_Get(ACTOR_SECOND_STAKE_ROPE_END);
    actor->motionFlags = flag;
    actor->y.fixed = toFIXED(16);
    Tightrope_Place(2, ACTOR_SECOND_STAKE_ROPE_START,
                    ACTOR_SECOND_STAKE_ROPE_END);
  }
  actor = Actor_Get(ACTOR_ROCK_UNDER_LOG);
  Actor_SetSpriteFlags(actor, 0);
  actor = Actor_Get(ACTOR_EAST_POLE_ROCK);
  Actor_SetSpriteFlags(actor, 0);
  FieldActor_SetSpritePriority(ACTOR_DOCK_CRATE, 2);
  Actor_Get(ACTOR_SAILOR_WHO_PAID_FOR_THE_SHIP)->sceneCounter = 1;
  if (Event_GetFlag(FLAG_BRIGGS_CAUGHT) != 0 &&
      Event_GetFlag(FLAG_ALHAFRA_MAST_CLEARED) == 0) {
    Actor_SetPosition(ACTOR_BRIGGS, 0, 0);
    Actor_SetPosition(ACTOR_CHAUCHA, 0, 0);
    Actor_SetPosition(ACTOR_EOLEO, 0, 0);
    Actor_SetPosition(ACTOR_ALHAFRAS_MAYOR, 0, 0);
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

/// Puts the log and crates the party has sunk under the water, where Felix
/// can hop across the crates, takes the sunk rocks away, and once the block is
/// Burst clears its cells from the mast's tip.
void EasternAlhafra_PlaceClearedDebris(void) {
  struct Actor *actor;

  if (Event_GetFlag(FLAG_LOG_ROLLED)) {
    actor = Actor_Get(ACTOR_LOG);
    Actor_SetPosition(ACTOR_LOG, 336, 536);
    Actor_SetAnimation(ACTOR_LOG, ANIM_LOG_FLOAT);
    actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    actor->motionFlags = ACTOR_FALLS;
    actor->velocityY = 0;
    actor->y.fixed = -toFIXED(4);
    actor->groundHeight = -toFIXED(4);
    FieldMapGrid_SetCellBehavior(toFIXED(336), toFIXED(528), 2, 0);
    FieldMapGrid_SetCellBehavior(toFIXED(336), toFIXED(528), 0, 0);
    FieldMapGrid_SetCellBehavior(toFIXED(352), toFIXED(528), 0, 0);
  }
  if (Event_GetFlag(FLAG_ROCK_UNDER_LOG_SUNK)) {
    Actor_SetPosition(ACTOR_ROCK_UNDER_LOG, 0, 0);
  }
  if (Event_GetFlag(FLAG_EAST_POLE_ROCK_SUNK)) {
    Actor_SetPosition(ACTOR_EAST_POLE_ROCK, 0, 0);
  }
  if (Event_GetFlag(FLAG_DOCK_CRATE_SUNK)) {
    actor = Actor_Get(ACTOR_DOCK_CRATE);
    Actor_SetPosition(ACTOR_DOCK_CRATE, 504, 520);
    Actor_SetAnimation(ACTOR_DOCK_CRATE, ANIM_CRATE_SUNK);
    actor->collisionFlags = 0;
    actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    FieldMapGrid_SetCellBehavior(actor->x.fixed, actor->z.fixed, 0, 0);
    MapGrid_SetCellAttributesAt(actor->x.fixed, actor->z.fixed, 0,
                                MAP_CELL_JUMP);
  }
  if (Event_GetFlag(FLAG_SAIL_CRATE_SUNK)) {
    actor = Actor_Get(ACTOR_SAIL_CRATE);
    Actor_SetPosition(ACTOR_SAIL_CRATE, 408, 536);
    Actor_SetAnimation(ACTOR_SAIL_CRATE, ANIM_CRATE_SUNK);
    actor->collisionFlags = 0;
    actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    FieldMapGrid_SetCellBehavior(actor->x.fixed, actor->z.fixed, 0, 0);
    MapGrid_SetCellAttributesAt(actor->x.fixed, actor->z.fixed, 0,
                                MAP_CELL_JUMP);
  }
  if (Event_GetFlag(FLAG_ALHAFRA_MAST_CLEARED)) {
    MapGrid_CopyCells(32, 24, 2, 2, 30, 24);
  }
}

/// The first time Felix comes aboard and walks by the cabin door, the party
/// lines up at the deck's edge to watch the Champa sailors at the mast:
/// "It's... It's no use!" They grumble that the junk has to come off the mast,
/// the rope has to be untangled and the huge rock is beyond them, and that they
/// can do nothing until Zack finishes the canoe.
void EasternAlhafra_OverhearWorkers(void) {

  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  FieldEvent_SetMessageId((s32)MsgItSIt);
  FieldCamera_SetSpeed(4);
  // "It's... It's no use!"
  FieldEvent_ShowMessageThenWait(ACTOR_SAILOR_GIVING_UP, 0, 10);
  Actor_WalkToAndWait(ACTOR_FELIX, 232, 392);
  FieldActor_LaunchFromLeader(ACTOR_KRADEN, 0, -16, FACING_SOUTHEAST);
  FieldActor_LaunchFromLeader(ACTOR_JENNA, 0, 0, FACING_SOUTHEAST);
  FieldActor_LaunchFromLeader(ACTOR_SHEBA, 0, 16, FACING_SOUTHEAST);
  Actor_WalkByAndWait(ACTOR_FELIX, 16, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTHEAST, 0);
  FieldEvent_Wait(30);
  EventCamera_MoveToActor(ACTOR_SAILOR_GIVING_UP, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_SAILOR_GIVING_UP, 2);
  FieldEvent_Wait(10);
  // "The mast just won't budge, no matter how many people we get to help us!"
  FieldEvent_ShowMessageThenWait(ACTOR_SAILOR_GIVING_UP, 0, 10);
  EventCamera_MoveToActor(ACTOR_SAILOR_WHO_PAID_FOR_THE_SHIP, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(10);
  FieldActor_Jump(ACTOR_SAILOR_WHO_PAID_FOR_THE_SHIP, 6, 15);
  FieldActor_Jump(ACTOR_SAILOR_WHO_PAID_FOR_THE_SHIP, 6, 23);
  // "We're going to have to get all this junk off the mast first."
  FieldEvent_ShowMessageThenWait(ACTOR_SAILOR_WHO_PAID_FOR_THE_SHIP, 0, 10);
  Actor_FaceDirection(ACTOR_SAILOR_GIVING_UP, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_SAILOR_WAITING_FOR_HIS_PAL, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_SAILOR_LONGING_FOR_CHAMPA, FACING_SOUTH_SOUTHWEST,
                      0);
  FieldEvent_Wait(20);
  EventCamera_MoveToActor(ACTOR_SAIL_CRATE, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(10);
  // "If we could just get some decent footing, we could have this thing..."
  FieldEvent_ShowMessageThenWait(ACTOR_SAILOR_WHO_PAID_FOR_THE_SHIP, 0, 10);
  EventCamera_MoveToActor(ACTOR_SAILOR_WAITING_FOR_HIS_PAL, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_SAILOR_WAITING_FOR_HIS_PAL, ANIM_SHAKE_HEAD);
  // "There's more to it than that, though..."
  FieldEvent_ShowMessageThenWait(ACTOR_SAILOR_WAITING_FOR_HIS_PAL, 0, 10);
  Actor_FaceDirection(ACTOR_SAILOR_WAITING_FOR_HIS_PAL, FACING_NORTHEAST, 0);
  FieldEvent_Wait(20);
  EventCamera_MoveToActor(ACTOR_SAILOR_LONGING_FOR_CHAMPA, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(10);
  Actor_FaceDirection(ACTOR_SAILOR_LONGING_FOR_CHAMPA, FACING_NORTHEAST, 0);
  FieldEvent_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_SAILOR_LONGING_FOR_CHAMPA, ANIM_NOD);
  FieldEvent_Wait(10);
  // "We still have to do something about that rope over there."
  FieldEvent_ShowMessageThenWait(ACTOR_SAILOR_LONGING_FOR_CHAMPA, 0, 10);
  Actor_FaceDirection(ACTOR_SAILOR_GIVING_UP, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_SAILOR_WHO_PAID_FOR_THE_SHIP,
                      FACING_NORTH_NORTHEAST, 0);
  FieldEvent_Wait(20);
  EventCamera_MoveToActor(ACTOR_STAKE, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(10);
  // "Untangling that'll be a piece of cake, once we get a boat we can use..."
  FieldEvent_ShowMessageThenWait(ACTOR_SAILOR_LONGING_FOR_CHAMPA, 0, 10);
  EventCamera_MoveToActor(ACTOR_SAILOR_GIVING_UP, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(10);
  Actor_ShowEmote(ACTOR_SAILOR_GIVING_UP, EMOTE_IN_FRONT | EMOTE_SCOWL, 40);
  // "Oh, yeah... A piece of cake... Except for that huge rock over there!"
  FieldEvent_ShowMessageThenWait(ACTOR_SAILOR_GIVING_UP, 0, 10);
  FieldCamera_MoveTo(488, 0, 416, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(20);
  Actor_ShowEmote(ACTOR_SAILOR_WHO_PAID_FOR_THE_SHIP,
                  EMOTE_IN_FRONT | EMOTE_WINCE, 50);
  Actor_FaceDirection(ACTOR_SAILOR_WHO_PAID_FOR_THE_SHIP, FACING_EAST, 0);
  FieldEvent_Wait(20);
  // "Oh yeah... The rock..."
  FieldEvent_ShowMessageThenWait(ACTOR_SAILOR_WHO_PAID_FOR_THE_SHIP, 0, 10);
  Actor_FaceDirection(ACTOR_SAILOR_WAITING_FOR_HIS_PAL, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_SAILOR_LONGING_FOR_CHAMPA, FACING_SOUTHEAST, 0);
  FieldEvent_Wait(20);
  FieldActor_StartRepeatedMotion(ACTOR_SAILOR_LONGING_FOR_CHAMPA, 3);
  Actor_SetAttachedEffect(ACTOR_SAILOR_LONGING_FOR_CHAMPA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(40);
  // "How are we supposed to get the mast out from under that thing? It's..."
  FieldEvent_ShowMessageThenWait(ACTOR_SAILOR_LONGING_FOR_CHAMPA, 0, 10);
  FieldCamera_MoveTo(392, 16, 416, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_SAILOR_WAITING_FOR_HIS_PAL, ANIM_SHAKE_HEAD);
  // "There's no way we're gonna move it by ourselves..."
  FieldEvent_ShowMessageThenWait(ACTOR_SAILOR_WAITING_FOR_HIS_PAL, 0, 10);
  Actor_FaceDirection(ACTOR_SAILOR_WHO_PAID_FOR_THE_SHIP, FACING_WEST, 0);
  FieldEvent_Wait(30);
  Actor_ShowEmote(ACTOR_SAILOR_GIVING_UP, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_ShowEmote(ACTOR_SAILOR_WHO_PAID_FOR_THE_SHIP,
                  EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_ShowEmote(ACTOR_SAILOR_LONGING_FOR_CHAMPA,
                  EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 50);
  FieldEvent_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_SAILOR_LONGING_FOR_CHAMPA, 2);
  FieldEvent_Wait(10);
  Actor_FaceDirection(ACTOR_SAILOR_LONGING_FOR_CHAMPA, FACING_SOUTH_SOUTHWEST,
                      0);
  FieldEvent_Wait(20);
  // "Well, either way, we can't do anything until Zack finishes the canoe."
  FieldEvent_ShowMessageThenWait(ACTOR_SAILOR_LONGING_FOR_CHAMPA, 0, 20);
  Actor_SetAnimation(ACTOR_SAILOR_GIVING_UP, ANIM_NOD);
  Actor_SetAnimation(ACTOR_SAILOR_WHO_PAID_FOR_THE_SHIP, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_SAILOR_WAITING_FOR_HIS_PAL, ANIM_NOD);
  FieldEvent_Wait(20);
  Actor_FaceDirection(ACTOR_SAILOR_GIVING_UP, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_SAILOR_WHO_PAID_FOR_THE_SHIP,
                      FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_SAILOR_WAITING_FOR_HIS_PAL, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_SAILOR_LONGING_FOR_CHAMPA, FACING_NORTHEAST, 0);
  FieldEvent_Wait(20);
  EventCamera_MoveToActor(gMainRoutineState.leaderActorId, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(20);
  Actor_RejoinLeader(ACTOR_JENNA);
  Actor_RejoinLeader(ACTOR_SHEBA);
  Actor_RejoinLeader(ACTOR_KRADEN);
  EventCamera_FollowActor(gMainRoutineState.leaderActorId, 1);
  FieldCamera_WaitForArrival();
  Event_End();
  return;
}

/// After Briggs is caught, when Felix crosses the deck from the cabin door
/// (cells (14, 26) to (18, 26)), Kraden asks: "You do plan to help with the
/// mast, right?" Sheba and Jenna talk it over, and the party agrees to get to
/// work. It happens once, and not after the mast is clear.
void EasternAlhafra_AskToHelpWithMast(void) {

  if (Event_GetFlag(FLAG_BRIGGS_CAUGHT) == 0)
    return;
  if (Event_GetFlag(FLAG_ALHAFRA_MAST_CLEARED) != 0)
    return;
  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  FieldEvent_SetMessageId((s32)MsgYouDoPlan);
  Event_SetFlag(FLAG_ASKED_TO_HELP);
  Actor_WalkToAndWait(ACTOR_FELIX, 264, 448);
  FieldActor_LaunchFromLeader(ACTOR_KRADEN, 0, -16, FACING_SOUTH);
  FieldActor_LaunchFromLeader(ACTOR_JENNA, 16, 0, FACING_WEST);
  FieldActor_LaunchFromLeader(ACTOR_SHEBA, 16, -16, FACING_SOUTHWEST);
  if (Event_GetFlag(FLAG_IN_PARTY(ACTOR_PIERS)) != 0) {
    FieldActor_LaunchFromLeader(ACTOR_PIERS, 8, 16, FACING_NORTH);
  }
  FieldActor_WaitForMoveById(ACTOR_SHEBA);
  Actor_FaceDirection(ACTOR_FELIX, FACING_EAST, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  FieldEvent_Wait(10);
  // "You do plan to help with the mast, right?"
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
  FieldEvent_Wait(20);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  // "Not even Briggs and his pirates could fix that mast. How are we..."
  FieldEvent_ShowMessageThenWait(ACTOR_SHEBA, 0, 10);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
  FieldEvent_Wait(10);
  // "They need our help... We at least have to try..."
  FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 10);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 0);
  FieldEvent_Wait(10);
  // "Isn't that right, Felix?"
  FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 10);
  Actor_FaceDirection(ACTOR_FELIX, FACING_EAST, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
  FieldEvent_Wait(30);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 30);
  // "Well then, we'd better get right to it, hadn't we?"
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
  if (Event_GetFlag(FLAG_IN_PARTY(ACTOR_PIERS)) != 0) {
    Actor_RejoinLeader(ACTOR_PIERS);
  }
  Actor_RejoinLeader(ACTOR_JENNA);
  Actor_RejoinLeader(ACTOR_SHEBA);
  Actor_RejoinLeader(ACTOR_KRADEN);
  Event_End();
}

/// Once the dock crate has sunk and only the huge block is left, while no one
/// in the party has Burst, Kraden asks: "Are you finished here, Felix?" Sheba
/// and Jenna differ over giving up; without Piers, Kraden muses on asking the
/// Madrans for their ship, and the party moves on.
void EasternAlhafra_AskIfFinished(void) {

  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  FieldEvent_SetMessageId((s32)MsgAreYouFinished);
  Actor_WalkToAndWait(ACTOR_FELIX, 232, 392);
  FieldActor_LaunchFromLeader(ACTOR_KRADEN, 0, -16, FACING_SOUTH);
  FieldActor_LaunchFromLeader(ACTOR_JENNA, 16, 0, FACING_WEST);
  FieldActor_LaunchFromLeader(ACTOR_SHEBA, 16, -16, FACING_SOUTHWEST);
  if (Event_GetFlag(FLAG_IN_PARTY(ACTOR_PIERS)) != 0) {
    FieldActor_LaunchFromLeader(ACTOR_PIERS, 8, 16, FACING_NORTH);
  }
  FieldActor_WaitForMoveById(ACTOR_SHEBA);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
  FieldEvent_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  FieldEvent_Wait(10);
  // "Are you finished here, Felix?"
  Event_OpenMessage(ACTOR_KRADEN, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(ACTOR_FELIX, 0) == 0) {
    FieldEvent_Wait(20);
    Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
    // "Wait, you wanted to do this, and now you're telling me we should just
    // give up?"
    FieldEvent_ShowMessageThenWait(ACTOR_SHEBA, 0, 10);
    Actor_FaceDirection(ACTOR_FELIX, FACING_NORTHEAST, 0);
    Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
    FieldEvent_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_SHAKE_HEAD);
    // "I just don't see how much more help we can be here, Sheba."
    FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 10);
    gFieldState->message += 2;
  } else {
    gFieldState->message += 2;
    FieldEvent_Wait(40);
    Actor_SetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
    Actor_FaceDirection(ACTOR_FELIX, FACING_NORTHEAST, 0);
    FieldEvent_Wait(20);
    // "Oh, don't be ridiculous! We've already done so much."
    FieldEvent_ShowMessageThenWait(ACTOR_SHEBA, 0, 10);
    Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_SHAKE_HEAD);
    // "I know how you feel, but there's nothing else we can do right now."
    FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 10);
  }
  if (Event_GetFlag(FLAG_IN_PARTY(ACTOR_PIERS)) != 0) {
    Actor_FaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
    Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 0);
    Actor_FaceDirectionAndWait(ACTOR_SHEBA, FACING_WEST);
    FieldEvent_Wait(10);
  } else {
    FieldActor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
    // "So... Maybe we should start searching for another ship then..."
    FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
    Actor_FaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
    Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 0);
    Actor_FaceDirection(ACTOR_SHEBA, FACING_WEST, 0);
    FieldEvent_Wait(30);
    Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
    Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
    // "...Not that I was planning on "borrowing" the Madran's ship!"
    FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
    FieldActor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
    // "Although...I did plan on asking them if we could use it once we got
    // back to Madra..."
    FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
    Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
    Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTH, 0);
    FieldEvent_Wait(40);
    Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 0);
    Actor_FaceDirection(ACTOR_SHEBA, FACING_WEST, 0);
    FieldEvent_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
    // "But it doesn't look like the ship will be leaving anytime soon."
    FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
  }
  FieldEvent_SetMessageId((s32)MsgSoIThink);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  FieldEvent_Wait(10);
  // "So I think maybe we should just forget about that and start on our..."
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
  Actor_SetAnimation(ACTOR_PIERS, ANIM_NOD);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimation(ACTOR_SHEBA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
  FieldEvent_Wait(10);
  if (Event_GetFlag(FLAG_IN_PARTY(ACTOR_PIERS)) != 0) {
    Actor_RejoinLeader(ACTOR_PIERS);
  }
  Actor_RejoinLeader(ACTOR_JENNA);
  Actor_RejoinLeader(ACTOR_SHEBA);
  Actor_RejoinLeader(ACTOR_KRADEN);
  Event_End();
  return;
}

extern u32 gQuestionMark_Script[];

/// Pops a "?" bubble up at (x, y, z), tilted by `rotation`, for `frames`, on
/// sprite priority 1: shared/question_mark_pop.h's bubble as the dock draws it.
void QuestionMark_Pop(FIXED x, FIXED y, FIXED z, u16 rotation, s32 frames) {
  struct Actor *mark;
  struct Sprite *sprite;

  mark = FieldActor_Create(SPRITE_EMOTE, x, y, z);
  if (mark != NULL) {
    sprite = mark->sprite;
    Actor_SelectAnimation(mark, EMOTE_QUESTION);
    Actor_StartScript(mark, gQuestionMark_Script);
    Actor_SetSpritePriorityDirect(mark, 1);
    mark->motionFlags = 0;
    mark->wait = frames;
    sprite->flags = 0;
    sprite->rotation = rotation;
  }
}

#include "shared/actor_pop_question_marks.h"

/// Heading off the ship once the block is Burst, Felix sees two workers below
/// the ship find it gone: "Hey! That block on the mast has disappeared!" They
/// run to tell the mayor, and Chaucha, brought by Eoleo, guesses the party did
/// it and goes to tell Briggs.
void EasternAlhafra_WorkersSeeBlockGone(void) {
  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  FieldEvent_SetMessageId((s32)MsgHeyThatBlock);
  Actor_SetSpeed(ACTOR_FIRST_WORKER, 2.8);
  Actor_SetSpeed(ACTOR_SECOND_WORKER, 2.8);
  Actor_SetSpeed(ACTOR_CHAUCHA, 1.8);
  Actor_SetSpeed(ACTOR_EOLEO, 1.8);
  Actor_SetPosition(ACTOR_FIRST_WORKER, 328, 616);
  Actor_FaceDirection(ACTOR_FIRST_WORKER, FACING_NORTHEAST, 0);
  Actor_SetPosition(ACTOR_SECOND_WORKER, 296, 616);
  Actor_FaceDirection(ACTOR_SECOND_WORKER, FACING_NORTHEAST, 0);
  // "Hey! That block on the mast has disappeared!"
  FieldEvent_ShowMessageThenWait(ACTOR_FIRST_WORKER, 0, 10);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTH, 0);
  FieldCamera_MoveTo(328, CAMERA_KEEP, 632, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(20);
  FieldActor_StartRepeatedMotion(ACTOR_FIRST_WORKER, 3);
  Actor_SetAttachedEffect(ACTOR_FIRST_WORKER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(50);
  FieldActor_ShowEmote(ACTOR_SECOND_WORKER, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  // "Hey! Now, we can raise the mast and use the sail again!"
  FieldEvent_ShowMessageThenWait(ACTOR_FIRST_WORKER, 0, 10);
  Actor_FaceDirection(ACTOR_FIRST_WORKER, FACING_WEST, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_FIRST_WORKER, ANIM_NOD);
  FieldEvent_Wait(10);
  // "Hey, mate, let's go tell the mayor the good news!"
  FieldEvent_ShowMessageThenWait(ACTOR_FIRST_WORKER, 0, 10);
  Actor_FaceDirection(ACTOR_SECOND_WORKER, FACING_EAST, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_SECOND_WORKER, ANIM_NOD);
  FieldEvent_Wait(10);
  // "Yeah, let's do that!"
  FieldEvent_ShowMessageThenWait(ACTOR_FIRST_WORKER, 0, 10);
  Actor_FaceDirection(ACTOR_SECOND_WORKER, FACING_WEST, 0);
  FieldEvent_Wait(10);
  Actor_WalkBy(ACTOR_FIRST_WORKER, -160, 0);
  Actor_WalkByAndWait(ACTOR_SECOND_WORKER, -160, 0);
  Actor_SetPosition(ACTOR_FIRST_WORKER, 0, 0);
  Actor_SetPosition(ACTOR_SECOND_WORKER, 0, 0);
  FieldEvent_Wait(30);
  Actor_SetPosition(ACTOR_CHAUCHA, 264, 384);
  Actor_FaceDirection(ACTOR_CHAUCHA, FACING_EAST, 0);
  Actor_SetPosition(ACTOR_EOLEO, 280, 392);
  Actor_FaceDirection(ACTOR_EOLEO, FACING_EAST, 0);
  Actor_SetPosition(ACTOR_FELIX, 232, 384);
  Actor_FaceDirection(ACTOR_FELIX, FACING_EAST, 0);
  // "Baba babubu!!"
  FieldEvent_ShowMessageThenWait(ACTOR_EOLEO, 0, 10);
  // "What is it, Eoleo? What are you so excited about?"
  FieldEvent_ShowMessageThenWait(ACTOR_CHAUCHA, 0, 10);
  // "Baba babubu!"
  FieldEvent_ShowMessageThenWait(ACTOR_EOLEO, 0, 10);
  // "It's true! The block that was pinning the mast down is gone!"
  FieldEvent_ShowMessageThenWait(ACTOR_CHAUCHA, 0, 10);
  FieldCamera_MoveTo(264, CAMERA_KEEP, 392, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(20);
  Actor_WalkByAndWait(ACTOR_CHAUCHA, -16, 0);
  FieldEvent_Wait(20);
  // "You were the ones who did it, weren't you?"
  Event_OpenMessage(ACTOR_CHAUCHA, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(ACTOR_FELIX, 0) == 0) {
    FieldEvent_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_CHAUCHA, ANIM_NOD);
    FieldEvent_Wait(10);
    // "I knew you would do it someday."
    FieldEvent_ShowMessageThenWait(ACTOR_CHAUCHA, 0, 10);
    gFieldState->message += 1;
  } else {
    FieldEvent_Wait(40);
    Actor_SetAnimationAndWait(ACTOR_CHAUCHA, ANIM_SHAKE_HEAD);
    FieldEvent_Wait(10);
    gFieldState->message += 1;
    // "But, of course, I also know that you didn't do it for our benefit."
    FieldEvent_ShowMessageThenWait(ACTOR_CHAUCHA, 0, 10);
  }
  Actor_FaceDirection(ACTOR_CHAUCHA, FACING_EAST, 0);
  FieldEvent_Wait(20);
  Actor_FaceDirection(ACTOR_EOLEO, FACING_WEST, 0);
  FieldEvent_Wait(30);
  // "Still, Briggs will be pleased when he hears about this."
  FieldEvent_ShowMessageThenWait(ACTOR_CHAUCHA, 0, 10);
  FieldActor_ShowEmote(ACTOR_EOLEO, EMOTE_IN_FRONT | EMOTE_SMILE, 45);
  FieldActor_Jump(ACTOR_EOLEO, 6, 15);
  FieldActor_Jump(ACTOR_EOLEO, 6, 23);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_CHAUCHA, ANIM_NOD);
  FieldEvent_Wait(10);
  // "Well then, let's go tell him!"
  FieldEvent_ShowMessageThenWait(ACTOR_CHAUCHA, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_EOLEO, ANIM_NOD);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_EOLEO, ANIM_NOD);
  FieldEvent_Wait(20);
  Actor_WalkByAndWait(ACTOR_CHAUCHA, 0, 8);
  FieldActor_WatchAndSetScript(ACTOR_FELIX, ACTOR_CHAUCHA);
  Actor_WalkBy(ACTOR_EOLEO, -96, 0);
  Actor_WalkByAndWait(ACTOR_CHAUCHA, -96, 0);
  Actor_WalkBy(ACTOR_CHAUCHA, 0, 80);
  Actor_WalkByAndWait(ACTOR_EOLEO, -32, 0);
  Actor_WalkByAndWait(ACTOR_EOLEO, 0, 80);
  FieldActor_Stop(ACTOR_FELIX);
  Actor_SetPosition(ACTOR_EOLEO, 0, 0);
  Actor_SetPosition(ACTOR_CHAUCHA, 0, 0);
  EventCamera_FollowActor(ACTOR_FELIX, 1);
  FieldCamera_WaitForArrival();
  Event_End();
}

/// Walking down the pier toward town once the block is Burst (cells (8, 32)
/// and (9, 32)), Felix meets the workers bringing the mayor and Jiya to see:
/// "It's gone! That huge block is really gone!" The mayor sends the workers to
/// gather the townspeople to raise the mast and invites the party back to his
/// manor to rest, where a guard brings word that Briggs has escaped.
void EasternAlhafra_WorkersTellMayor(void) {
  if (Event_GetFlag(FLAG_ALHAFRA_MAST_CLEARED) == 0)
    return;
  Event_SetFlag(FLAG_WORKERS_TOLD_MAYOR);
  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  FieldEvent_SetMessageId((s32)MsgIStillJust);
  FieldCamera_SetSpeed(4);
  Actor_SetPosition(ACTOR_ALHAFRAS_MAYOR, 56, 632);
  Actor_FaceDirection(ACTOR_ALHAFRAS_MAYOR, FACING_EAST, 0);
  Actor_SetPosition(ACTOR_JIYA, 56, 648);
  Actor_FaceDirection(ACTOR_JIYA, FACING_EAST, 0);
  Actor_SetPosition(ACTOR_FIRST_WORKER, 48, 616);
  Actor_FaceDirection(ACTOR_FIRST_WORKER, FACING_EAST, 0);
  Actor_SetPosition(ACTOR_SECOND_WORKER, 32, 608);
  Actor_FaceDirection(ACTOR_SECOND_WORKER, FACING_EAST, 0);
  FieldEvent_Wait(10);
  // "I still just can't quite believe that the stone block on top of the..."
  FieldEvent_ShowMessageThenWait(ACTOR_ALHAFRAS_MAYOR, 0, 10);
  FieldCamera_MoveTo(144, CAMERA_KEEP, 664, 1);
  FieldEvent_Wait(10);
  Actor_SetSpeed(ACTOR_FIRST_WORKER, 1);
  Actor_SetSpeed(ACTOR_SECOND_WORKER, 1);
  Actor_SetSpeed(ACTOR_ALHAFRAS_MAYOR, 1);
  Actor_SetSpeed(ACTOR_JIYA, 1);
  Actor_WalkBy(ACTOR_ALHAFRAS_MAYOR, 32, 0);
  Actor_WalkBy(ACTOR_JIYA, 32, 0);
  Actor_WalkBy(ACTOR_FIRST_WORKER, 24, 24);
  Actor_WalkBy(ACTOR_SECOND_WORKER, 24, 24);
  FieldActor_WaitForMoveById(ACTOR_FIRST_WORKER);
  FieldActor_WaitForMoveById(ACTOR_SECOND_WORKER);
  Actor_FaceDirection(ACTOR_FIRST_WORKER, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_SECOND_WORKER, FACING_EAST, 0);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(10);
  FieldActor_ShowEmote(ACTOR_FIRST_WORKER, EMOTE_IN_FRONT | EMOTE_SCOWL, 0);
  FieldActor_ShowEmote(ACTOR_FIRST_WORKER, EMOTE_IN_FRONT | EMOTE_SCOWL, 40);
  Actor_SetSpeed(ACTOR_FIRST_WORKER, 3);
  Actor_SetSpeed(ACTOR_SECOND_WORKER, 3);
  Actor_SetScript(ACTOR_FIRST_WORKER, gEasternAlhafra_FirstWorkerCheerScript);
  Actor_SetScript(ACTOR_SECOND_WORKER, gEasternAlhafra_SecondWorkerCheerScript);
  FieldActor_WaitForScriptEndById(ACTOR_FIRST_WORKER);
  FieldActor_WaitForScriptEndById(ACTOR_SECOND_WORKER);
  FieldEvent_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_FIRST_WORKER, 2);
  // "I'm telling you, Mayor, I saw it with my own eyes! ...I mean, I..."
  FieldEvent_ShowMessageThenWait(ACTOR_FIRST_WORKER, 0, 10);
  FieldActor_Jump(ACTOR_SECOND_WORKER, 6, 15);
  FieldActor_Jump(ACTOR_SECOND_WORKER, 6, 23);
  // "If you don't believe us, you can go see for yourself!"
  FieldEvent_ShowMessageThenWait(ACTOR_SECOND_WORKER, 0, 10);
  Actor_SetSpeed(ACTOR_FIRST_WORKER, 1.6);
  Actor_SetSpeed(ACTOR_SECOND_WORKER, 1.6);
  Actor_SetSpeed(ACTOR_ALHAFRAS_MAYOR, 1.6);
  Actor_SetSpeed(ACTOR_JIYA, 1.6);
  FieldActor_StartRepeatedMotion(ACTOR_ALHAFRAS_MAYOR, 3);
  Actor_SetAttachedEffect(ACTOR_ALHAFRAS_MAYOR,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldActor_StartRepeatedMotion(ACTOR_JIYA, 3);
  Actor_SetAttachedEffect(ACTOR_JIYA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(40);
  Actor_SetScript(ACTOR_FIRST_WORKER, gEasternAlhafra_FirstWorkerRunScript);
  Actor_SetScript(ACTOR_SECOND_WORKER, gEasternAlhafra_SecondWorkerRunScript);
  FieldEvent_Wait(20);
  EventCamera_FollowActor(ACTOR_ALHAFRAS_MAYOR, 1);
  Actor_WalkBy(ACTOR_ALHAFRAS_MAYOR, 64, 0);
  Actor_WalkByAndWait(ACTOR_JIYA, 64, 0);
  Actor_WalkTo(ACTOR_ALHAFRAS_MAYOR, 168, 616);
  Actor_WalkToAndWait(ACTOR_JIYA, 184, 616);
  FieldActor_WaitForScriptEndById(ACTOR_ALHAFRAS_MAYOR);
  Actor_SetAnimation(ACTOR_ALHAFRAS_MAYOR, ANIM_STAND);
  Actor_FaceDirection(ACTOR_ALHAFRAS_MAYOR, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_JIYA, FACING_NORTHEAST, 0);
  FieldActor_WaitForScriptEndById(ACTOR_FIRST_WORKER);
  FieldActor_WaitForScriptEndById(ACTOR_SECOND_WORKER);
  FieldEvent_Wait(20);
  FieldActor_StartRepeatedMotion(ACTOR_ALHAFRAS_MAYOR, 3);
  Actor_SetAttachedEffect(ACTOR_ALHAFRAS_MAYOR,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldActor_StartRepeatedMotion(ACTOR_JIYA, 3);
  Actor_SetAttachedEffect(ACTOR_JIYA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(40);
  FieldCamera_SetSpeed(6);
  FieldCamera_MoveTo(384, CAMERA_KEEP, 480, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(10);
  // "Huh?"
  FieldEvent_ShowMessageThenWait(ACTOR_ALHAFRAS_MAYOR, 0, 10);
  FieldCamera_SetSpeed(6);
  FieldCamera_MoveTo(480, CAMERA_KEEP, 344, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(10);
  // "What?"
  FieldEvent_ShowMessageThenWait(ACTOR_ALHAFRAS_MAYOR, 0, 10);
  FieldCamera_SetSpeed(6);
  FieldCamera_MoveTo(488, CAMERA_KEEP, 456, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(10);
  // "Whoa, hey!!!"
  FieldEvent_ShowMessageThenWait(ACTOR_ALHAFRAS_MAYOR, 0, 10);
  FieldCamera_SetSpeed(6);
  FieldCamera_MoveTo(192, CAMERA_KEEP, 616, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(20);
  FieldActor_ShowEmote(ACTOR_ALHAFRAS_MAYOR, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  Actor_FaceDirection(ACTOR_ALHAFRAS_MAYOR, FACING_NORTH, 0);
  FieldEvent_Wait(20);
  // "It's gone! That huge block is really gone!"
  FieldEvent_ShowMessageThenWait(ACTOR_ALHAFRAS_MAYOR, 0, 10);
  Actor_FaceDirection(ACTOR_FIRST_WORKER, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_SECOND_WORKER, FACING_SOUTH_SOUTHWEST, 0);
  FieldEvent_Wait(30);
  Actor_SetAnimation(ACTOR_FIRST_WORKER, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_SECOND_WORKER, ANIM_NOD);
  FieldEvent_Wait(20);
  Actor_FaceDirection(ACTOR_JIYA, FACING_WEST, 0);
  FieldEvent_Wait(30);
  FieldActor_ShowEmote(ACTOR_JIYA, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  // "Excellent news, Mayor!"
  FieldEvent_ShowMessageThenWait(ACTOR_JIYA, 0, 10);
  Actor_FaceDirection(ACTOR_ALHAFRAS_MAYOR, FACING_EAST, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_ALHAFRAS_MAYOR, ANIM_NOD);
  FieldEvent_Wait(10);
  // "Unfortunately, the mayor of Madra will probably try to take it from..."
  FieldEvent_ShowMessageThenWait(ACTOR_ALHAFRAS_MAYOR, 0, 10);
  Actor_SetAnimation(ACTOR_FIRST_WORKER, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_SECOND_WORKER, ANIM_NOD);
  FieldEvent_Wait(20);
  Actor_FaceDirection(ACTOR_ALHAFRAS_MAYOR, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_JIYA, FACING_NORTH_NORTHWEST, 0);
  FieldEvent_Wait(40);
  FieldActor_ShowEmote(ACTOR_ALHAFRAS_MAYOR, EMOTE_IN_FRONT | EMOTE_QUESTION,
                       30);
  // "But that can wait. Who was able to move the block?"
  FieldEvent_ShowMessageThenWait(ACTOR_ALHAFRAS_MAYOR, 0, 10);
  FieldActor_StartRepeatedMotion(ACTOR_FIRST_WORKER, 3);
  Actor_SetAttachedEffect(ACTOR_FIRST_WORKER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldActor_StartRepeatedMotion(ACTOR_SECOND_WORKER, 3);
  Actor_SetAttachedEffect(ACTOR_SECOND_WORKER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(50);
  Actor_FaceDirection(ACTOR_FIRST_WORKER, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_SECOND_WORKER, FACING_NORTH_NORTHWEST, 0);
  FieldEvent_Wait(40);
  FieldActor_ShowEmote(ACTOR_FIRST_WORKER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                       0);
  FieldActor_ShowEmote(ACTOR_SECOND_WORKER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                       40);
  Actor_FaceDirection(ACTOR_ALHAFRAS_MAYOR, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_JIYA, FACING_NORTH_NORTHWEST, 0);
  FieldEvent_Wait(20);
  FieldCamera_MoveTo(144, CAMERA_KEEP, 544, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(20);
  // "Felix!"
  FieldEvent_ShowMessageThenWait(ACTOR_ALHAFRAS_MAYOR, 0, 10);
  FieldActor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  Actor_SetSpeed(ACTOR_ALHAFRAS_MAYOR, 1.5);
  Actor_SetSpeed(ACTOR_JIYA, 1.5);
  Actor_WalkBy(ACTOR_ALHAFRAS_MAYOR, -32, -32);
  Actor_WalkByAndWait(ACTOR_JIYA, -32, -32);
  Actor_WalkBy(ACTOR_ALHAFRAS_MAYOR, 0, -24);
  Actor_WalkByAndWait(ACTOR_JIYA, 0, -24);
  FieldActor_WaitForMoveById(ACTOR_ALHAFRAS_MAYOR);
  Actor_FaceDirection(ACTOR_ALHAFRAS_MAYOR, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_JIYA, FACING_NORTH_NORTHWEST, 0);
  FieldEvent_Wait(20);
  FieldActor_ShowEmote(ACTOR_ALHAFRAS_MAYOR, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                       30);
  // "Felix, did you do this?"
  FieldEvent_ShowMessageThenWait(ACTOR_ALHAFRAS_MAYOR, 0, 10);
  Actor_WalkToAndWait(ACTOR_FELIX, 136, 520);
  FieldActor_LaunchFromLeader(ACTOR_KRADEN, 16, 0, FACING_SOUTH_SOUTHWEST);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTH, 0);
  FieldActor_WaitForMoveById(ACTOR_KRADEN);
  FieldEvent_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  FieldEvent_Wait(10);
  // "Surprised to see us, Mayor?"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_ALHAFRAS_MAYOR, ANIM_NOD);
  FieldEvent_Wait(10);
  // "Did you do this?"
  FieldEvent_ShowMessageThenWait(ACTOR_ALHAFRAS_MAYOR, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
  // "We left Alhafra some time ago, but we just returned and destroyed..."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0, 10);
  FieldActor_ShowEmote(ACTOR_ALHAFRAS_MAYOR, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP,
                       30);
  // "Exploring? What brought you back to Alhafra right now?"
  FieldEvent_ShowMessageThenWait(ACTOR_ALHAFRAS_MAYOR, 0, 10);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
  FieldEvent_Wait(20);
  // "Felix had a hunch that we might be needed here."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_JIYA, 2);
  FieldEvent_Wait(20);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_JIYA, FACING_WEST, 0);
  FieldEvent_Wait(20);
  // "Mayor, not to interrupt, but shouldn't we begin repairs on your ship..."
  FieldEvent_ShowMessageThenWait(ACTOR_JIYA, 0, 10);
  FieldActor_ShowEmote(ACTOR_ALHAFRAS_MAYOR, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                       30);
  Actor_FaceDirection(ACTOR_ALHAFRAS_MAYOR, FACING_EAST, 0);
  FieldEvent_Wait(20);
  // "Yes, yes... You're right, of course."
  FieldEvent_ShowMessageThenWait(ACTOR_ALHAFRAS_MAYOR, 0, 10);
  Actor_FaceDirection(ACTOR_JIYA, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_ALHAFRAS_MAYOR, FACING_SOUTH_SOUTHEAST, 0);
  FieldEvent_Wait(20);
  FieldActor_Jump(ACTOR_ALHAFRAS_MAYOR, 6, 15);
  FieldActor_Jump(ACTOR_ALHAFRAS_MAYOR, 6, 23);
  // "Gather the townspeople and get them to work on the mast."
  FieldEvent_ShowMessageThenWait(ACTOR_ALHAFRAS_MAYOR, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_FIRST_WORKER, 2);
  FieldEvent_Wait(10);
  // "Yes, sir! We're on our way!"
  FieldEvent_ShowMessageThenWait(ACTOR_FIRST_WORKER, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_ALHAFRAS_MAYOR, ANIM_NOD);
  FieldEvent_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_SECOND_WORKER, 2);
  FieldEvent_Wait(10);
  // "We shouldn't need too many people to raise the mast!"
  FieldEvent_ShowMessageThenWait(ACTOR_SECOND_WORKER, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_ALHAFRAS_MAYOR, ANIM_NOD);
  FieldEvent_Wait(10);
  // "All right. I'm counting on you."
  FieldEvent_ShowMessageThenWait(ACTOR_ALHAFRAS_MAYOR, 0, 20);
  Actor_WalkBy(ACTOR_FIRST_WORKER, 0, 64);
  Actor_WalkBy(ACTOR_SECOND_WORKER, 0, 64);
  FieldEvent_Wait(60);
  Actor_SetPosition(ACTOR_FIRST_WORKER, 0, 0);
  Actor_SetPosition(ACTOR_SECOND_WORKER, 0, 0);
  Actor_FaceDirection(ACTOR_JIYA, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_ALHAFRAS_MAYOR, FACING_NORTH_NORTHEAST, 0);
  FieldEvent_Wait(20);
  // "Destroying that stone must have been quite a challenge."
  FieldEvent_ShowMessageThenWait(ACTOR_ALHAFRAS_MAYOR, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_ALHAFRAS_MAYOR, ANIM_NOD);
  FieldEvent_Wait(10);
  // "Why don't you come back to my manor to get some rest?"
  Event_OpenMessage(ACTOR_ALHAFRAS_MAYOR, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(ACTOR_FELIX, 0) != 0) {
    FieldEvent_Wait(40);
    Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
    FieldEvent_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
    // "We could use the rest, Felix. We should accept the mayor's offer."
    FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0,
                                   10);
    gFieldState->message += 1;
  } else {
    FieldEvent_Wait(30);
    Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
    FieldEvent_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
    FieldEvent_Wait(10);
    gFieldState->message += 1;
    // "You're too kind, Mayor. We'd be happy to..."
    FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0,
                                   10);
  }
  Actor_FaceDirection(ACTOR_FELIX, FACING_EAST, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
  FieldEvent_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_ALHAFRAS_MAYOR, ANIM_NOD);
  FieldEvent_Wait(30);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 0);
  FieldEvent_Wait(20);
  Actor_FaceDirection(ACTOR_ALHAFRAS_MAYOR, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_JIYA, FACING_SOUTH_SOUTHEAST, 0);
  FieldEvent_Wait(20);
  Actor_SetSpeed(ACTOR_FELIX, 0.8);
  Actor_SetSpeed(ACTOR_KRADEN, 0.8);
  Actor_SetSpeed(ACTOR_ALHAFRAS_MAYOR, 0.8);
  Actor_SetSpeed(ACTOR_JIYA, 0.8);
  Actor_GetById(ACTOR_FELIX)->motionFlags = 0;
  Actor_GetById(ACTOR_KRADEN)->motionFlags = 0;
  Actor_GetById(ACTOR_ALHAFRAS_MAYOR)->motionFlags = 0;
  Actor_GetById(ACTOR_JIYA)->motionFlags = 0;
  Actor_WalkBy(ACTOR_FELIX, 0, 120);
  Actor_WalkBy(ACTOR_KRADEN, 0, 120);
  Actor_WalkBy(ACTOR_JIYA, 0, 80);
  Actor_WalkByAndWait(ACTOR_ALHAFRAS_MAYOR, 0, 80);
  {
    struct FieldState *event = Ram_HeapSlots->fieldState;

    event->transitionFrames = 60;
    event->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  }
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_SetDestination(SCENE_ALHAFRA_MANOR, ENTRANCE_MANOR_BRIGGS_ESCAPES);
  Event_End();
}

/// Back at the foot of the pier after Briggs's escape (cells (7, 34) to
/// (9, 34)), Felix sees Briggs sailing off in the ship with Chaucha: "Briggs,
/// where are you taking my boat?" Briggs says he paid for it fair and square,
/// taunts the party from the deck ("Neener neener neener!") and sails away.
/// The music fades, and the party finds itself back on the pier with the
/// mayor (EasternAlhafra_MayorFindsShipGone).
void EasternAlhafra_BriggsStealsShip(void) {
  struct MapState *map;
  s32 frames;
  FIXED step = toFIXED(1);

  if (Event_GetFlag(FLAG_ALHAFRA_MAST_CLEARED) == 0)
    return;
  Event_SetFlag(FLAG_ALHAFRA_SHIP_GONE);
  Event_SetFlag(FLAG_BRIGGS_SAILED_FROM_ALHAFRA);
  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  FieldEvent_SetMessageId((s32)MsgEasternAlhafraBriggs);
  // "Briggs!!!"
  FieldEvent_ShowMessageThenWait(ACTOR_SECOND_WORKER, 0, 10);
  Actor_WalkToAndWait(ACTOR_FELIX, 136, 552);
  FieldActor_LaunchFromLeader(ACTOR_KRADEN, -16, 0, FACING_NORTHWEST);
  Actor_FaceDirectionById(ACTOR_FELIX, FACING_NORTH, 0);
  FieldActor_WaitForMoveById(ACTOR_KRADEN);
  FieldEvent_Wait(20);
  FieldActor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  FieldCamera_MoveTo(176, CAMERA_KEEP, 392, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_ALHAFRAS_MAYOR, 2);
  FieldEvent_Wait(10);
  // "Briggs, where are you taking my boat?"
  FieldEvent_ShowMessageThenWait(ACTOR_ALHAFRAS_MAYOR, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_JIYA, 2);
  FieldEvent_Wait(10);
  // "He's attempting to steal it! Thief! Thief!"
  FieldEvent_ShowMessageThenWait(ACTOR_JIYA, 0, 10);
  Actor_SetPosition(ACTOR_BRIGGS, 224, 8);
  // "Did you call me a thief?"
  FieldEvent_ShowMessageThenWait(ACTOR_BRIGGS, 0, 10);
  ((struct MapState *)Ram_HeapSlots->mapGrid)->minX = 0;
  ((struct MapState *)Ram_HeapSlots->mapGrid)->maxX = toFIXED(1024);
  ((struct MapState *)Ram_HeapSlots->mapGrid)->minZ = 0;
  ((struct MapState *)Ram_HeapSlots->mapGrid)->maxZ = toFIXED(1024);
  FieldCamera_SetSpeed(4);
  FieldCamera_MoveTo(208, CAMERA_KEEP, 168, 1);
  FieldEvent_Wait(50);
  Event_PlaySound(SOUND_FADE_OUT_BGM);
  Event_FinishScreenTransition();
  PaletteFade_Interpolate(16);
  Event_WaitTransitionFrames();
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  FieldEvent_Wait(10);
  FieldCamera_MoveTo(808, CAMERA_KEEP, 664, 0);
  {
    struct Actor *center = gFieldState->camera;

    FieldActor_SetPositionAndResetMotion(center, center->x.fixed,
                                         center->y.fixed, center->z.fixed);
  }
  FieldEvent_Wait(50);
  FieldActor_SetSpritePriority(ACTOR_BRIGGS, 1);
  FieldActor_SetSpritePriority(ACTOR_CHAUCHA, 1);
  Actor_SetSpeed(ACTOR_CHAUCHA, 1.2);
  Actor_SetSpeed(ACTOR_BRIGGS, 1.2);
  Actor_SetPosition(ACTOR_BRIGGS, 792, 856);
  Actor_SetPosition(ACTOR_CHAUCHA, 824, 856);
  Actor_SetPosition(ACTOR_KRADEN, 744, 1016);
  Actor_SetPosition(ACTOR_ALHAFRAS_MAYOR, 744, 1016);
  Event_PlaySound(MUS_BRIGGS_STEALS_SHIP);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_Wait(30);
  FieldCamera_SetSpeed(2);
  FieldCamera_MoveTo(808, CAMERA_KEEP, 848, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(20);
  FieldActor_Jump(ACTOR_BRIGGS, 6, 15);
  FieldActor_Jump(ACTOR_BRIGGS, 6, 23);
  // "I paid for this boat, fair and square. I'm no thief."
  FieldEvent_ShowMessageThenWait(ACTOR_BRIGGS, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_CHAUCHA, ANIM_NOD);
  FieldEvent_Wait(10);
  // "You're the thieves! You tried to take our boat away, after we paid..."
  FieldEvent_ShowMessageThenWait(ACTOR_CHAUCHA, 0, 10);
  // "... ..."
  FieldEvent_ShowMessageThenWait(ACTOR_ALHAFRAS_MAYOR, 0, 10);
  Actor_FaceDirectionById(ACTOR_BRIGGS, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_CHAUCHA, FACING_WEST, 0);
  FieldEvent_Wait(25);
  Actor_SetAnimationAndWait(ACTOR_BRIGGS, ANIM_NOD);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_CHAUCHA, ANIM_NOD);
  FieldEvent_Wait(20);
  Actor_FaceDirection(ACTOR_BRIGGS, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_CHAUCHA, FACING_SOUTH_SOUTHWEST, 0);
  FieldEvent_Wait(20);
  // "You're the one who stole OUR boat!"
  FieldEvent_ShowMessageThenWait(ACTOR_BRIGGS, 0, 10);
  // "Ahoy, Briggs!"
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
  FieldActor_ShowEmote(ACTOR_CHAUCHA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  FieldActor_ShowEmote(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 50);
  FieldActor_StartRepeatedMotion(ACTOR_BRIGGS, 3);
  Actor_SetAttachedEffect(ACTOR_BRIGGS,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(40);
  Actor_WalkByAndWait(ACTOR_BRIGGS, 0, 16);
  FieldEvent_Wait(20);
  FieldActor_ShowEmote(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  // "Uh-oh... It's Felix!"
  FieldEvent_ShowMessageThenWait(ACTOR_BRIGGS, 0, 10);
  Actor_WalkByAndWait(ACTOR_CHAUCHA, 0, 16);
  Actor_FaceDirection(ACTOR_CHAUCHA, FACING_WEST, 0);
  FieldEvent_Wait(20);
  FieldActor_ShowEmote(ACTOR_CHAUCHA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  // "You're not afraid of them, are you?"
  FieldEvent_ShowMessageThenWait(ACTOR_CHAUCHA, 0, 10);
  Actor_FaceDirection(ACTOR_CHAUCHA, FACING_SOUTH_SOUTHWEST, 0);
  FieldEvent_Wait(50);
  Actor_FaceDirection(ACTOR_CHAUCHA, FACING_WEST, 0);
  FieldEvent_Wait(20);
  // "Look how far away they are... There's no way they can catch us now!"
  FieldEvent_ShowMessageThenWait(ACTOR_CHAUCHA, 0, 10);
  Actor_FaceDirectionById(ACTOR_BRIGGS, FACING_EAST, 0);
  FieldEvent_Wait(20);
  FieldActor_ShowEmote(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_QUESTION, 45);
  Actor_WalkByAndWait(ACTOR_BRIGGS, 0, 32);
  FieldEvent_Wait(30);
  FieldActor_ShowEmote(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 45);
  Actor_WalkByAndWait(ACTOR_BRIGGS, 0, -32);
  Actor_FaceDirectionById(ACTOR_BRIGGS, FACING_EAST, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_BRIGGS, ANIM_NOD);
  FieldEvent_Wait(10);
  // "You're right... They're stuck on the land, and we've got this boat!"
  FieldEvent_ShowMessageThenWait(ACTOR_BRIGGS, 0, 10);
  Actor_FaceDirection(ACTOR_CHAUCHA, FACING_SOUTH_SOUTHWEST, 0);
  FieldEvent_Wait(50);
  Actor_FaceDirectionById(ACTOR_CHAUCHA, FACING_WEST, 0);
  FieldEvent_Wait(20);
  FieldActor_ShowEmote(ACTOR_CHAUCHA, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  // "They can't catch us... What would they do, swim?"
  FieldEvent_ShowMessageThenWait(ACTOR_CHAUCHA, 0, 10);
  FieldActor_ShowEmote(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_QUESTION, 50);
  Actor_WalkByAndWait(ACTOR_BRIGGS, 0, 32);
  FieldEvent_Wait(30);
  FieldActor_ShowEmote(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 50);
  Actor_WalkByAndWait(ACTOR_BRIGGS, 0, -32);
  Actor_FaceDirectionById(ACTOR_BRIGGS, FACING_EAST, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_BRIGGS, ANIM_NOD);
  FieldEvent_Wait(10);
  // "Hah! You're right! They can't board us from way over there!"
  FieldEvent_ShowMessageThenWait(ACTOR_BRIGGS, 0, 10);
  FieldActor_ShowEmote(ACTOR_CHAUCHA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  // "Wasn't there something you wanted to do the next time you saw Felix?"
  FieldEvent_ShowMessageThenWait(ACTOR_CHAUCHA, 0, 10);
  Actor_FaceDirection(ACTOR_BRIGGS, FACING_SOUTH_SOUTHWEST, 0);
  FieldEvent_Wait(30);
  Actor_PopQuestionMarks(ACTOR_BRIGGS);
  Event_PlaySound(SE_CURSE);
  FieldActor_ShowEmote(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 50);
  Actor_FaceDirectionById(ACTOR_BRIGGS, FACING_EAST, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_CHAUCHA, ANIM_SHAKE_HEAD);
  // "Don't tell me you forgot... It's all you ever talked about when you..."
  FieldEvent_ShowMessageThenWait(ACTOR_CHAUCHA, 0, 10);
  Actor_FaceDirection(ACTOR_BRIGGS, FACING_SOUTH_SOUTHWEST, 0);
  FieldEvent_Wait(30);
  Actor_PopQuestionMarks(ACTOR_BRIGGS);
  FieldActor_ShowEmote(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 50);
  Actor_FaceDirectionById(ACTOR_BRIGGS, FACING_EAST, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_BRIGGS, ANIM_NOD);
  FieldEvent_Wait(10);
  // "Oh yeah... I almost forgot."
  FieldEvent_ShowMessageThenWait(ACTOR_BRIGGS, 0, 10);
  FieldActor_ShowEmote(ACTOR_CHAUCHA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  // "So now's your chance! Do what you swore to do!"
  FieldEvent_ShowMessageThenWait(ACTOR_CHAUCHA, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_BRIGGS, 2);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_BRIGGS, ANIM_NOD);
  FieldEvent_Wait(20);
  Actor_FaceDirectionAndWait(ACTOR_BRIGGS, FACING_SOUTH_SOUTHWEST);
  Actor_SetAnimation(ACTOR_BRIGGS, ANIM_BRIGGS_HANDS_TO_FACE);
  FieldEvent_Wait(20);
  Actor_SetAnimation(ACTOR_BRIGGS, ANIM_BRIGGS_TONGUE_OUT);
  FieldEvent_Wait(5);
  // "Bleah!"
  FieldEvent_ShowMessageThenWait(ACTOR_BRIGGS, 0, 10);
  FieldActor_Jump(ACTOR_CHAUCHA, 6, 15);
  FieldActor_Jump(ACTOR_CHAUCHA, 6, 23);
  // "Is that the best you could come up with?"
  FieldEvent_ShowMessageThenWait(ACTOR_CHAUCHA, 0, 10);
  FieldActor_ShowEmote(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
  Actor_SetAnimation(ACTOR_BRIGGS, ANIM_BRIGGS_HANDS_TO_FACE);
  FieldEvent_Wait(20);
  Actor_SetAnimation(ACTOR_BRIGGS, ANIM_STAND);
  Actor_FaceDirectionById(ACTOR_BRIGGS, FACING_EAST, 0);
  FieldEvent_Wait(15);
  // "But, Chaucha, they're an awfully tough bunch..."
  FieldEvent_ShowMessageThenWait(ACTOR_BRIGGS, 0, 10);
  FieldActor_ShowEmote(ACTOR_CHAUCHA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 30);
  // "See? It's that attitude right there! That's why you'll never amount..."
  FieldEvent_ShowMessageThenWait(ACTOR_CHAUCHA, 0, 10);
  FieldActor_ShowEmote(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 40);
  // "You know what? You're right!"
  FieldEvent_ShowMessageThenWait(ACTOR_BRIGGS, 0, 10);
  Actor_WalkByAndWait(ACTOR_BRIGGS, 0, 8);
  Actor_WalkByAndWait(ACTOR_BRIGGS, 10, 16);
  Actor_FaceDirection(ACTOR_CHAUCHA, FACING_SOUTH_SOUTHWEST, 0);
  Actor_WalkByAndWait(ACTOR_BRIGGS, 6, 16);
  Actor_FaceDirectionById(ACTOR_BRIGGS, FACING_SOUTH_SOUTHWEST, 0);
  FieldEvent_Wait(10);
  FieldActor_Jump(ACTOR_BRIGGS, 6, 15);
  FieldActor_Jump(ACTOR_BRIGGS, 6, 23);
  // "Hey! Felix! You guys take care! And keep Alhafra safe from thieves..."
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_BRIGGS, 0, 10);
  // "What!?!"
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
  FieldActor_ShowEmote(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  // "I doubt we'll ever meet again, but if we do, remember this!"
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_BRIGGS, 0, 20);
  Actor_SetAnimation(ACTOR_BRIGGS, ANIM_BRIGGS_WAG_HEAD);
  FieldEvent_Wait(10);
  // "Neener neener neener! Neener neener neener!"
  FieldEvent_ShowMessageThenWait(ACTOR_BRIGGS, 0, 10);
  FieldActor_Jump(ACTOR_BRIGGS, 7, 0);
  frames = 15;
  do {
    *(s16 *)&Actor_GetById(ACTOR_BRIGGS)->facing -= TRIG_TURN / 16;
    frames--;
    System_Wait(1);
  } while (frames >= 0);
  Actor_SetAnimation(ACTOR_BRIGGS, ANIM_BRIGGS_PULL_FACE);
  FieldEvent_Wait(7);
  // "Neener neener bleah!"
  FieldEvent_ShowMessageThenWait(ACTOR_BRIGGS, 0, 20);
  FieldActor_ShowEmote(ACTOR_CHAUCHA, EMOTE_IN_FRONT | EMOTE_SMILE, 50);
  Actor_WalkByAndWait(ACTOR_CHAUCHA, 0, 36);
  FieldEvent_Wait(15);
  Actor_SetAnimation(ACTOR_BRIGGS, ANIM_STAND);
  FieldEvent_Wait(20);
  FieldActor_Jump(ACTOR_CHAUCHA, 6, 15);
  FieldActor_Jump(ACTOR_CHAUCHA, 6, 23);
  // "That may have been a bit much. If you make him mad, he might chase us
  // right back to Champa."
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_CHAUCHA, 0, 15);
  Actor_SetAnimationAndWait(ACTOR_BRIGGS, ANIM_NOD);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_BRIGGS, ANIM_NOD);
  FieldEvent_Wait(10);
  Actor_SetAnimation(ACTOR_BRIGGS, ANIM_BRIGGS_PULL_FACE_SLOWLY);
  FieldEvent_Wait(70);
  map = Ram_HeapSlots->mapGrid;
  Actor_GetById(ACTOR_BRIGGS)->motionFlags = 0;
  Actor_GetById(ACTOR_CHAUCHA)->motionFlags = 0;
  MapGrid_CopyCellsLayered(42, 60, 17, 4, 42, 64);
  MapGrid_CopyCellsLayered(42, 60, 17, 4, 42, 68);
  MapGrid_CopyCellsLayered(106, 60, 17, 4, 106, 64);
  MapGrid_CopyCellsLayered(106, 60, 17, 4, 106, 68);
  frames = 239;
  do {
    map->layers[0].offsetY += step;
    map->layers[1].offsetY += toFIXED(1);
    Actor_GetById(ACTOR_BRIGGS)->z.fixed -= step;
    frames--;
    Actor_GetById(ACTOR_CHAUCHA)->z.fixed -= step;
    System_Wait(1);
  } while (frames >= 0);
  Event_PlaySound(SOUND_FADE_OUT_BGM);
  FieldEvent_Wait(30);
  Event_End();
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  Event_SetDestination(SCENE_EASTERN_ALHAFRA, ENTRANCE_AFTER_SHIP_STOLEN);
}

/// Back on the pier after Briggs has sailed off: the mayor shouts "What!!! You
/// let Briggs get away!", hauls the knocked-down workers to their feet, blames
/// them all, tells Felix there will be no reward and storms off to his manor.
/// Kraden thinks Briggs is heading back to Champa.
void EasternAlhafra_MayorFindsShipGone(void) {
  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  FieldEvent_SetMessageId((s32)MsgWhatYouLet);
  FieldCamera_MoveTo(152, CAMERA_KEEP, 272, 0);
  {
    struct Actor *center = gFieldState->camera;

    FieldActor_SetPositionAndResetMotion(center, center->x.fixed,
                                         center->y.fixed, center->z.fixed);
  }
  FieldEvent_Wait(30);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_Wait(15);
  // "What!!! You let Briggs get away!"
  FieldEvent_ShowMessageThenWait(ACTOR_ALHAFRAS_MAYOR, 0, 10);
  FieldCamera_MoveTo(152, CAMERA_KEEP, 392, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(20);
  FieldActor_ShowEmote(ACTOR_ALHAFRAS_MAYOR, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN,
                       30);
  // "You and your friends talk a good game, but when trouble happens..."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_ALHAFRAS_MAYOR,
                                 0, 10);
  Actor_FaceDirection(ACTOR_ALHAFRAS_MAYOR, FACING_SOUTH_SOUTHWEST, 0);
  FieldEvent_Wait(30);
  // "Well, isn't this a fine mess!"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_ALHAFRAS_MAYOR,
                                 0, 10);
  FieldActor_Jump(ACTOR_ALHAFRAS_MAYOR, 6, 15);
  FieldActor_Jump(ACTOR_ALHAFRAS_MAYOR, 6, 23);
  FieldEvent_Wait(10);
  FieldActor_ShowEmote(ACTOR_ALHAFRAS_MAYOR, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP,
                       30);
  // "My prize ship, stolen by a bunch of filthy pirate scum!"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_ALHAFRAS_MAYOR,
                                 0, 10);
  Actor_FaceDirection(ACTOR_ALHAFRAS_MAYOR, FACING_NORTH_NORTHEAST, 0);
  FieldEvent_Wait(30);
  FieldActor_ShowEmote(ACTOR_ALHAFRAS_MAYOR, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                       30);
  Actor_SetSpeed(ACTOR_ALHAFRAS_MAYOR, 0.7);
  Actor_GetById(ACTOR_ALHAFRAS_MAYOR)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkByAndWait(ACTOR_ALHAFRAS_MAYOR, 0, 8);
  FieldEvent_Wait(1);
  Actor_GetById(ACTOR_ALHAFRAS_MAYOR)->steeringFlags |= ACTOR_FACES_MOTION;
  FieldEvent_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_ALHAFRAS_MAYOR, 2);
  FieldEvent_Wait(10);
  Actor_SetSpeed(ACTOR_ALHAFRAS_MAYOR, 2.4);
  Actor_GetById(ACTOR_ALHAFRAS_MAYOR)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkByAndWait(ACTOR_ALHAFRAS_MAYOR, 0, -16);
  FieldEvent_Wait(1);
  Actor_GetById(ACTOR_ALHAFRAS_MAYOR)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_GetById(ACTOR_ALHAFRAS_MAYOR)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkBy(ACTOR_ALHAFRAS_MAYOR, 0, 8);
  Event_PlaySound(SE_IMPACT);
  Actor_SetSpeed(ACTOR_SECOND_WORKER, 2.4);
  Actor_GetById(ACTOR_SECOND_WORKER)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkByAndWait(ACTOR_SECOND_WORKER, 0, -4);
  FieldEvent_Wait(1);
  Actor_GetById(ACTOR_SECOND_WORKER)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_GetById(ACTOR_SECOND_WORKER)->facing = FACING_WEST;
  Actor_GetById(ACTOR_SECOND_WORKER)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkByAndWait(ACTOR_SECOND_WORKER, 0, -4);
  FieldEvent_Wait(1);
  Actor_GetById(ACTOR_SECOND_WORKER)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_GetById(ACTOR_SECOND_WORKER)->facing = FACING_NORTH_NORTHWEST;
  Actor_GetById(ACTOR_SECOND_WORKER)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkByAndWait(ACTOR_SECOND_WORKER, 0, -4);
  FieldEvent_Wait(1);
  Actor_GetById(ACTOR_SECOND_WORKER)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_GetById(ACTOR_SECOND_WORKER)->facing = FACING_NORTH_NORTHEAST;
  Actor_GetById(ACTOR_SECOND_WORKER)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkByAndWait(ACTOR_SECOND_WORKER, 0, -4);
  FieldEvent_Wait(1);
  Actor_GetById(ACTOR_SECOND_WORKER)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_GetById(ACTOR_SECOND_WORKER)->facing = 0;
  Actor_GetById(ACTOR_SECOND_WORKER)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkByAndWait(ACTOR_SECOND_WORKER, 0, -4);
  FieldEvent_Wait(1);
  Actor_GetById(ACTOR_SECOND_WORKER)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_GetById(ACTOR_SECOND_WORKER)->facing = FACING_SOUTH_SOUTHEAST;
  FieldEvent_Wait(20);
  Actor_GetById(ACTOR_ALHAFRAS_MAYOR)->steeringFlags |= ACTOR_FACES_MOTION;
  // "Get up!"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_ALHAFRAS_MAYOR,
                                 0, 10);
  Actor_SetPosition(ACTOR_SECOND_WORKER, 168, 368);
  FieldActor_SetSpriteRotationById(ACTOR_SECOND_WORKER, 0);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_SECOND_WORKER), SPRITE_FLAG_SHADOW);
  Actor_FaceDirection(ACTOR_SECOND_WORKER, FACING_SOUTH_SOUTHWEST, 0);
  FieldActor_Jump(ACTOR_SECOND_WORKER, 6, 40);
  FieldActor_ShowEmote(ACTOR_SECOND_WORKER, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Actor_FaceDirectionById(ACTOR_ALHAFRAS_MAYOR, FACING_SOUTH_SOUTHEAST, 0);
  FieldEvent_Wait(30);
  Actor_SetSpeed(ACTOR_ALHAFRAS_MAYOR, 0.7);
  Actor_GetById(ACTOR_ALHAFRAS_MAYOR)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkByAndWait(ACTOR_ALHAFRAS_MAYOR, 0, -8);
  FieldEvent_Wait(1);
  Actor_GetById(ACTOR_ALHAFRAS_MAYOR)->steeringFlags |= ACTOR_FACES_MOTION;
  FieldEvent_Wait(20);
  Actor_SetSpeed(ACTOR_ALHAFRAS_MAYOR, 2.4);
  Actor_GetById(ACTOR_ALHAFRAS_MAYOR)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkByAndWait(ACTOR_ALHAFRAS_MAYOR, 0, 16);
  FieldEvent_Wait(1);
  Actor_GetById(ACTOR_ALHAFRAS_MAYOR)->steeringFlags |= ACTOR_FACES_MOTION;
  Event_PlaySound(SE_IMPACT);
  Actor_SetSpeed(ACTOR_SECOND_WORKER, 2.4);
  Actor_GetById(ACTOR_JIYA)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkByAndWait(ACTOR_JIYA, 0, 3);
  FieldEvent_Wait(1);
  Actor_GetById(ACTOR_JIYA)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_GetById(ACTOR_JIYA)->facing = 0;
  Actor_GetById(ACTOR_JIYA)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkByAndWait(ACTOR_JIYA, 0, 3);
  FieldEvent_Wait(1);
  Actor_GetById(ACTOR_JIYA)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_GetById(ACTOR_JIYA)->facing = FACING_NORTH_NORTHEAST;
  Actor_GetById(ACTOR_JIYA)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkByAndWait(ACTOR_JIYA, 0, 3);
  FieldEvent_Wait(1);
  Actor_GetById(ACTOR_JIYA)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_GetById(ACTOR_JIYA)->facing = FACING_NORTH_NORTHWEST;
  Actor_GetById(ACTOR_JIYA)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkByAndWait(ACTOR_JIYA, 0, 3);
  FieldEvent_Wait(1);
  Actor_GetById(ACTOR_JIYA)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_GetById(ACTOR_JIYA)->facing = FACING_WEST;
  Actor_GetById(ACTOR_JIYA)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkByAndWait(ACTOR_JIYA, 0, 3);
  FieldEvent_Wait(1);
  Actor_GetById(ACTOR_JIYA)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_GetById(ACTOR_JIYA)->facing = FACING_SOUTH_SOUTHWEST;
  FieldEvent_Wait(20);
  // "They're gone, and you let them escape!"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_ALHAFRAS_MAYOR,
                                 0, 10);
  Actor_SetPosition(ACTOR_JIYA, 168, 424);
  FieldActor_SetSpriteRotationById(ACTOR_JIYA, 0);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_JIYA), SPRITE_FLAG_SHADOW);
  Actor_GetById(ACTOR_JIYA)->facing = FACING_NORTH_NORTHWEST;
  FieldActor_Jump(ACTOR_JIYA, 6, 40);
  FieldActor_ShowEmote(ACTOR_JIYA, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Actor_FaceDirectionById(ACTOR_ALHAFRAS_MAYOR, FACING_NORTH_NORTHEAST, 0);
  FieldEvent_Wait(30);
  FieldActor_ShowEmote(ACTOR_ALHAFRAS_MAYOR, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  // "You guys have royally botched this one. This ruins everything!"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_ALHAFRAS_MAYOR,
                                 0, 10);
  FieldActor_ShowEmote(ACTOR_SECOND_WORKER, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP,
                       50);
  Actor_FaceDirectionById(ACTOR_ALHAFRAS_MAYOR, FACING_SOUTH_SOUTHEAST, 0);
  FieldEvent_Wait(30);
  FieldActor_ShowEmote(ACTOR_ALHAFRAS_MAYOR, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  // "You all ought to be ashamed! All of you were cowering, while I was..."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_ALHAFRAS_MAYOR,
                                 0, 10);
  FieldActor_ShowEmote(ACTOR_JIYA, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
  Actor_SetAnimation(ACTOR_ALHAFRAS_MAYOR, ANIM_MAYOR_KEEP_SHAKING_HEAD);
  FieldEvent_Wait(30);
  // "All of you!"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_ALHAFRAS_MAYOR,
                                 0, 10);
  Actor_FaceDirectionById(ACTOR_SECOND_WORKER, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirectionById(ACTOR_JIYA, FACING_NORTH_NORTHEAST, 0);
  FieldEvent_Wait(20);
  FieldActor_StartRepeatedMotion(ACTOR_SECOND_WORKER, 3);
  Actor_SetAttachedEffect(ACTOR_SECOND_WORKER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldActor_StartRepeatedMotion(ACTOR_JIYA, 3);
  Actor_SetAttachedEffect(ACTOR_JIYA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(40);
  Actor_SetSpeed(ACTOR_SECOND_WORKER, 1.8);
  Actor_WalkByAndWait(ACTOR_SECOND_WORKER, 16, 16);
  Actor_WalkByAndWait(ACTOR_SECOND_WORKER, 0, 40);
  Actor_FaceDirectionById(ACTOR_SECOND_WORKER, FACING_NORTH_NORTHWEST, 0);
  FieldEvent_Wait(30);
  Actor_SetAnimation(ACTOR_ALHAFRAS_MAYOR, ANIM_STAND);
  FieldEvent_Wait(30);
  Actor_FaceDirectionById(ACTOR_SECOND_WORKER, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirectionById(ACTOR_JIYA, FACING_NORTH_NORTHWEST, 0);
  // "I'm going back to my manor!"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_ALHAFRAS_MAYOR,
                                 0, 10);
  Actor_SetAnimation(ACTOR_JIYA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_SECOND_WORKER, ANIM_NOD);
  FieldEvent_Wait(20);
  Actor_SetSpeed(ACTOR_ALHAFRAS_MAYOR, 0.8);
  Actor_SetSpeed(ACTOR_JIYA, 0.8);
  Actor_SetSpeed(ACTOR_SECOND_WORKER, 0.8);
  Actor_SetSpeed(ACTOR_FIRST_WORKER, 0.8);
  Actor_SetSpeed(ACTOR_YOUNG_MAN_KNOCKED_DOWN, 0.8);
  Actor_SetSpeed(ACTOR_MAN_KNOCKED_DOWN, 0.8);
  EventCamera_FollowActor(ACTOR_ALHAFRAS_MAYOR, 1);
  Actor_WalkByAndWait(ACTOR_ALHAFRAS_MAYOR, -16, 16);
  Actor_WalkByAndWait(ACTOR_ALHAFRAS_MAYOR, 0, 16);
  Actor_WalkTo(ACTOR_ALHAFRAS_MAYOR, 152, 504);
  Actor_WalkBy(ACTOR_SECOND_WORKER, -16, 0);
  Actor_WalkByAndWait(ACTOR_JIYA, -16, 16);
  Actor_WalkBy(ACTOR_SECOND_WORKER, -16, 16);
  Actor_WalkByAndWait(ACTOR_JIYA, 0, 16);
  Actor_WalkTo(ACTOR_JIYA, 152, 488);
  Actor_WalkToAndWait(ACTOR_SECOND_WORKER, 152, 472);
  Actor_SetAnimation(ACTOR_ALHAFRAS_MAYOR, ANIM_STAND);
  Actor_SetAnimation(ACTOR_JIYA, ANIM_STAND);
  Actor_FaceDirectionById(ACTOR_SECOND_WORKER, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirectionById(ACTOR_JIYA, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirectionById(ACTOR_ALHAFRAS_MAYOR, FACING_SOUTH_SOUTHWEST, 0);
  FieldEvent_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_ALHAFRAS_MAYOR, ANIM_SHAKE_HEAD);
  // "And you, Felix. I am extremely disappointed in you."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_ABOVE |
                                     ACTOR_ALHAFRAS_MAYOR,
                                 0, 10);
  FieldActor_StartRepeatedMotion(ACTOR_FELIX, 3);
  Actor_SetAttachedEffect(ACTOR_FELIX,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldActor_StartRepeatedMotion(ACTOR_KRADEN, 3);
  Actor_SetAttachedEffect(ACTOR_KRADEN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(40);
  Actor_WalkBy(ACTOR_JIYA, 0, 64);
  Actor_WalkBy(ACTOR_SECOND_WORKER, 0, 64);
  Actor_WalkByAndWait(ACTOR_ALHAFRAS_MAYOR, 0, 64);
  FieldActor_ShowEmote(ACTOR_ALHAFRAS_MAYOR, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB,
                       40);
  // "There will be no reward for you, Felix!"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALHAFRAS_MAYOR,
                                 0, 10);
  Actor_FaceDirectionById(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirectionById(ACTOR_FELIX, FACING_SOUTHEAST, 0);
  FieldEvent_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_ALHAFRAS_MAYOR, ANIM_NOD);
  FieldEvent_Wait(20);
  // "You let that Briggs steal MY ship! You're lucky I don't lock you up..."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALHAFRAS_MAYOR,
                                 0, 10);
  Actor_WalkByAndWait(ACTOR_ALHAFRAS_MAYOR, 0, 16);
  FieldEvent_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_ALHAFRAS_MAYOR, 2);
  FieldEvent_Wait(10);
  // "Bah! I've had enough of this! Don't just stand there with your..."
  FieldEvent_ShowMessageThenWait(ACTOR_ALHAFRAS_MAYOR, 0, 10);
  FieldActor_StartRepeatedMotion(ACTOR_YOUNG_MAN_KNOCKED_DOWN, 2);
  FieldActor_StartRepeatedMotion(ACTOR_MAN_KNOCKED_DOWN, 2);
  Actor_RunRepeatedMotion(ACTOR_FIRST_WORKER, 2);
  FieldEvent_Wait(20);
  Actor_SetPosition(ACTOR_FIRST_WORKER, 176, 592);
  FieldActor_SetSpriteRotationById(ACTOR_FIRST_WORKER, 0);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_FIRST_WORKER), SPRITE_FLAG_SHADOW);
  FieldActor_Jump(ACTOR_FIRST_WORKER, 6, 20);
  Actor_FaceDirectionById(ACTOR_FIRST_WORKER, FACING_WEST, 0);
  Actor_SetPosition(ACTOR_MAN_KNOCKED_DOWN, 128, 608);
  FieldActor_SetSpriteRotationById(ACTOR_MAN_KNOCKED_DOWN, 0);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_MAN_KNOCKED_DOWN),
                       SPRITE_FLAG_SHADOW);
  FieldActor_Jump(ACTOR_MAN_KNOCKED_DOWN, 6, 20);
  Actor_FaceDirectionById(ACTOR_MAN_KNOCKED_DOWN, FACING_NORTH_NORTHEAST, 0);
  Actor_SetPosition(ACTOR_YOUNG_MAN_KNOCKED_DOWN, 192, 616);
  FieldActor_SetSpriteRotationById(ACTOR_YOUNG_MAN_KNOCKED_DOWN, 0);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_YOUNG_MAN_KNOCKED_DOWN),
                       SPRITE_FLAG_SHADOW);
  FieldActor_Jump(ACTOR_YOUNG_MAN_KNOCKED_DOWN, 6, 20);
  Actor_FaceDirectionById(ACTOR_YOUNG_MAN_KNOCKED_DOWN, FACING_NORTH_NORTHWEST,
                          0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_ALHAFRAS_MAYOR, ANIM_SHAKE_HEAD);
  // "We don't have a boat, so there's no reason for anyone to be here at..."
  FieldEvent_ShowMessageThenWait(ACTOR_ALHAFRAS_MAYOR, 0, 10);
  Actor_SetAnimation(ACTOR_YOUNG_MAN_KNOCKED_DOWN, ANIM_NOD);
  Actor_SetAnimation(ACTOR_MAN_KNOCKED_DOWN, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_FIRST_WORKER, ANIM_NOD);
  FieldEvent_Wait(30);
  FieldActor_WatchAndSetScript(ACTOR_FELIX, ACTOR_ALHAFRAS_MAYOR);
  FieldActor_WatchAndSetScript(ACTOR_KRADEN, ACTOR_ALHAFRAS_MAYOR);
  FieldActor_WatchAndSetScript(ACTOR_YOUNG_MAN_KNOCKED_DOWN,
                               ACTOR_ALHAFRAS_MAYOR);
  FieldActor_WatchAndSetScript(ACTOR_MAN_KNOCKED_DOWN, ACTOR_ALHAFRAS_MAYOR);
  FieldActor_WatchAndSetScript(ACTOR_FIRST_WORKER, ACTOR_ALHAFRAS_MAYOR);
  Actor_SetScript(ACTOR_ALHAFRAS_MAYOR, gEasternAlhafra_RunOffScript);
  Actor_SetScript(ACTOR_JIYA, gEasternAlhafra_RunOffScript);
  FieldEvent_Wait(10);
  Actor_SetScript(ACTOR_SECOND_WORKER, gEasternAlhafra_RunOffScript);
  FieldEvent_Wait(70);
  Actor_WalkByAndWait(ACTOR_FIRST_WORKER, -16, 0);
  Actor_SetScript(ACTOR_FIRST_WORKER, gEasternAlhafra_RunOffScript);
  FieldEvent_Wait(33);
  Actor_WalkByAndWait(ACTOR_MAN_KNOCKED_DOWN, 16, 0);
  Actor_SetScript(ACTOR_MAN_KNOCKED_DOWN, gEasternAlhafra_RunOffScript);
  FieldEvent_Wait(15);
  Actor_SetScript(ACTOR_YOUNG_MAN_KNOCKED_DOWN, gEasternAlhafra_RunOffScript);
  FieldActor_WaitForScriptEndById(ACTOR_ALHAFRAS_MAYOR);
  Actor_FaceDirectionById(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
  FieldActor_WaitForScriptEndById(ACTOR_YOUNG_MAN_KNOCKED_DOWN);
  FieldEvent_Wait(20);
  EventCamera_FollowActor(ACTOR_FELIX, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(40);
  FieldActor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 50);
  Actor_FaceDirectionById(ACTOR_KRADEN, FACING_EAST, 0);
  Actor_FaceDirectionById(ACTOR_FELIX, FACING_WEST, 0);
  FieldEvent_Wait(20);
  // "What an awful person. Do you suppose he's always this self-centered?"
  Event_OpenMessage(ACTOR_KRADEN, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(ACTOR_FELIX, 0) == 0) {
    FieldEvent_Wait(20);
    // "Yes, he's terribly irritating. You're quite right, Felix."
    FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
    gFieldState->message += 1;
  } else {
    FieldEvent_Wait(40);
    gFieldState->message += 1;
    // "You can stay calm after listening to that windbag? That's quite mature
    // of you."
    FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
  }
  Actor_SetSpeed(ACTOR_KRADEN, 0.8);
  Actor_WalkByAndWait(ACTOR_KRADEN, 0, -16);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
  Actor_FaceDirectionById(ACTOR_KRADEN, FACING_NORTH_NORTHEAST, 0);
  FieldEvent_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  FieldEvent_Wait(20);
  // "I'll wager Briggs is returning to Champa, personally."
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
  Actor_WalkByAndWait(ACTOR_KRADEN, 0, 16);
  Actor_FaceDirection(ACTOR_FELIX, FACING_WEST, 0);
  Actor_FaceDirectionById(ACTOR_KRADEN, FACING_EAST, 0);
  FieldEvent_Wait(20);
  // "Well, it doesn't matter. I think I've had enough of this town. Shall we
  // be leaving?"
  Event_OpenMessage(ACTOR_KRADEN, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(ACTOR_FELIX, 0) != 0) {
    FieldEvent_Wait(40);
    // "Ah, yes, well... If you still have business to attend to here."
    FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
    Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
    FieldEvent_Wait(10);
    // "All right. But the sooner we leave, the better. Lead on, Felix!"
    FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
    gFieldState->message += 2;
  } else {
    FieldEvent_Wait(20);
    gFieldState->message += 2;
    // "Great! Let's leave as soon as possible, shall we?"
    FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
  }
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
  FieldEvent_Wait(20);
  Actor_SetSpeed(ACTOR_KRADEN, 1.2);
  Actor_SetAnimation(ACTOR_KRADEN, ANIM_WALK);
  {
    struct Actor *leader = Actor_GetById(gMainRoutineState.leaderActorId);

    if (leader != NULL)
      Actor_SetDestination(ACTOR_KRADEN, leader->x.part[0].integer,
                           leader->z.part[0].integer);
  }
  FieldActor_WaitForMoveById(ACTOR_KRADEN);
  Actor_SetPosition(ACTOR_KRADEN, 0, 0);
  FieldEvent_Wait(10);
  Event_End();
}

void BattleEffect_StepVector3(struct Particle *particle, s32 damping, s32 step);

#include "shared/particle_update_falling.h"

/// Spawns count rocks around (x, y, z), each pushed out at a random angle and
/// thrown up and out along it.
#include "shared/rock_spawn.h"

/// Loads the falling rocks' tiles and sprites into the menu heap slot and
/// starts their update.
void EasternAlhafra_StartFallingRocks(void) {
  struct RockState *state =
      HeapSlot_Alloc(HEAP_SLOT_MENU, sizeof(struct RockState));
  s32 entry;
  s32 tile;
  s32 i;
  struct Rock *rock;
  struct EffectSprite *sprite;

  Decompress_TaggedLz(Resource_GetTableEntry(RESOURCE_FALLING_ROCK), state);
  entry = VramCache_FindFreeEntry();
  tile = Vram_Load(entry, sizeof(state->tiles), state);
  state->tileBase = tile;
  state->resource = entry;
  rock = state->rocks;
  for (i = 0; i < ROCK_COUNT; i++) {
    sprite = &state->sprites[i];
    EasternAlhafra_InitializeRockSprite(sprite, 8, 8, OBJ_SIZE_16X16, tile);
    sprite->oam.fullColor = 1;
    sprite->oam.palette = 0;
    sprite->oam.priority = 0;
    sprite->depth = OAM_DEPTH_WINDOW;
    rock->timer = -1;
    rock++;
  }
  state->next = 0;
  FieldTask_AddFrame(Particle_UpdateFalling, TASK_PASS_FRAME);
}

#include "shared/rock_end_falling.h"

extern s8 gPushRectangles[][4];

#include "shared/push_target_fill_rectangle.h"

#include "../field/map_cell_fill.c"

extern s32 gPushLeaderSteps[];

#include "shared/push_target_find_stop.h"

#include "shared/push_target_find_ahead.h"

#include "shared/push_target_push.h"

#include "shared/actor_stop_at_push_target.h"

#include "shared/pillar_place.h"

#include "shared/pound_sink_target.h"

#include "shared/actor_set_cell_open.h"

#include "text/msg_ids.h"

// clang-format off
#include "shared/push_kinds.c"

#include "shared/push_leader_steps.c"

/// Unused: a list of one actor, the first crate, ended by ACTOR_LIST_END.
u16 gEasternAlhafra_UnusedActorList[] = {
    ACTOR_DOCK_CRATE, ACTOR_LIST_END,
};

/// The stake Pound sinks: actor, how its cell opens (Actor_SetCellOpen) and its
/// flag.
u16 gEasternAlhafra_PoundStakes[] = {
    ACTOR_STAKE, 1, FLAG_EASTERN_ALHAFRA_STAKE_POUNDED, ACTOR_LIST_END,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 488, 0, 424, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {10, CONDITION_ALWAYS, 520, 0, 520, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {11, CONDITION_ALWAYS, 264, 0, 424, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {12, CONDITION_ALWAYS, 440, 0, 344, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {ENTRANCE_AFTER_SHIP_STOLEN, CONDITION_ALWAYS, 136, 0, 568, FACING_NORTH, 0, 32, 0, 608, 704, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(EasternAlhafra),
    SCENE_EXIT_UNLESS(EASTERN_ALHAFRA_TO_ALHAFRA, Alhafra, ALHAFRA_FROM_EASTERN_ALHAFRA, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(EASTERN_ALHAFRA_TO_EASTERN_ALHAFRA_SHIP, EasternAlhafraShip, EASTERN_ALHAFRA_SHIP_FROM_EASTERN_ALHAFRA, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

struct ActorPlacement gEasternAlhafra_Placements[] = {
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 280, 0, 232, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 376, 0, 232, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_TALL_CRATE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 552, 0, 520, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TALL_CRATE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 408, 0, 520, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 344, 0, 312, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 440, 0, 312, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 312, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 336, 0, 456, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WHITE_ROCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 344, 0, 472, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WHITE_ROCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 440, 0, 456, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 444, 0, 312, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 482, 0, 312, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 444, 0, 376, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 492, 0, 312, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_CHAMPA_SAILOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 440, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_CHAMPA_SAILOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 440, 0, 424, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_CHAMPA_SAILOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 344, 0, 488, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_CHAMPA_SAILOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 440, 0, 376, FACING_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SHEBA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PIERS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_NORTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gEasternAlhafra_PlacementsAfterBriggsCaught[] = {
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 280, 0, 232, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 376, 0, 232, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_TALL_CRATE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 552, 0, 520, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TALL_CRATE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 408, 0, 520, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 344, 0, 312, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 440, 0, 312, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 312, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 336, 0, 456, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WHITE_ROCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 344, 0, 472, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WHITE_ROCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 440, 0, 456, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 444, 0, 312, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 482, 0, 312, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 444, 0, 376, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 492, 0, 312, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BRIGGS, CONDITION_ALWAYS, MOVEMENT_TYPE_IGNORE_RISES, -56, 0, -56, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_CHAUCHA, CONDITION_ALWAYS, MOVEMENT_TYPE_IGNORE_RISES, -56, 0, -56, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_EOLEO, CONDITION_ALWAYS, MOVEMENT_TYPE_IGNORE_RISES, -56, 0, -56, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_ALHAFRAS_MAYOR, CONDITION_ALWAYS, MOVEMENT_TYPE_IGNORE_RISES, -56, 0, -56, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SHEBA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PIERS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_GREY_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_IGNORE_RISES, -56, 0, -56, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BALD_MUSCULAR_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_IGNORE_RISES, -56, 0, -56, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_IGNORE_RISES, -56, 0, -56, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS, MOVEMENT_TYPE_IGNORE_RISES, -56, 0, -56, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_CURLY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_IGNORE_RISES, -56, 0, -56, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

/// How the ship's door opens: one frame of cells copied over it.
u16 gEasternAlhafra_ShipDoorOpening[] = {
    MAPGRID_ANIMATION_STEP(0, 0, 1, 2, 6),
    MAPGRID_ANIMATION_STEPS_END,
};

/// The ship's door opening as Felix walks into it.
struct MapGridCellAnimation gEasternAlhafra_ShipDoor = {
    .steps = gEasternAlhafra_ShipDoorOpening, .x = 16, .y = 17,
};

struct Trigger gSceneEvents[] = {
    {TRIGGER_EXIT, EASTERN_ALHAFRA_TO_ALHAFRA, CONDITION_ALWAYS, EASTERN_ALHAFRA_TO_ALHAFRA},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), EASTERN_ALHAFRA_TO_EASTERN_ALHAFRA_SHIP, CONDITION_ALWAYS, EVENT(EasternAlhafra_EnterShip)},
    {TRIGGER_TALK, ACTOR_SAILOR_GIVING_UP, CONDITION_ALWAYS, EVENT(EasternAlhafra_TalkToSailor)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SAILOR_GIVING_UP | TRIGGER_SUBJECT_OWN_EFFECT, FLAG_TALKED_TO_SAILOR, EVENT(EasternAlhafra_TalkToSailor)},
    {TRIGGER_TALK, ACTOR_SAILOR_WHO_PAID_FOR_THE_SHIP, CONDITION_ALWAYS, MSGNO(MsgWePaidBig)},
    {TRIGGER_TALK, ACTOR_SAILOR_WAITING_FOR_HIS_PAL, CONDITION_ALWAYS, MSGNO(MsgWeSentMy)},
    {TRIGGER_TALK, ACTOR_SAILOR_LONGING_FOR_CHAMPA, CONDITION_ALWAYS, MSGNO(MsgIfWeCould)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SAILOR_GIVING_UP, CONDITION_ALWAYS, MSGNO(MsgPeopleCallUs)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SAILOR_WHO_PAID_FOR_THE_SHIP, CONDITION_ALWAYS, MSGNO(MsgIThoughtIt)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SAILOR_WAITING_FOR_HIS_PAL, CONDITION_ALWAYS, MSGNO(MsgThatLousyRat)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SAILOR_LONGING_FOR_CHAMPA, CONDITION_ALWAYS, MSGNO(MsgWhenIThink)},
    {TRIGGER_TALK, ACTOR_YOUNG_MAN_KNOCKED_DOWN, CONDITION_ALWAYS, MSGNO(MsgOooogOw)},
    {TRIGGER_TALK, ACTOR_MAN_KNOCKED_DOWN, CONDITION_ALWAYS, MSGNO(MsgWhyDoThese)},
    {TRIGGER_TALK, ACTOR_FIRST_WORKER, CONDITION_ALWAYS, MSGNO(MsgStupidBriggs)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_MAN_KNOCKED_DOWN, CONDITION_ALWAYS, MSGNO(MsgBriggsSPirates)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_KNOCKED_DOWN, CONDITION_ALWAYS, MSGNO(MsgIJustCame)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FIRST_WORKER, CONDITION_ALWAYS, MSGNO(MsgBriggsSaidTheBoat)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), CELL_MAST_BLOCK, FLAG_ALHAFRA_MAST_CLEARED, EVENT(EasternAlhafra_BurstMastBlock)},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), ACTOR_ROPE_POST, FLAG_EASTERN_ALHAFRA_ROPE_LASHED, 0},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), ACTOR_STAKE, FLAG_EASTERN_ALHAFRA_STAKE_POUNDED, EVENT(EasternAlhafra_PoundStake)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_DOCK_CRATE, FLAG_DOCK_CRATE_SUNK, EVENT(EasternAlhafra_StartPushing)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_DOCK_CRATE, FLAG_DOCK_CRATE_SUNK, EVENT(EasternAlhafra_FinishPushing)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_SAIL_CRATE, FLAG_SAIL_CRATE_SUNK, EVENT(EasternAlhafra_StartPushing)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SAIL_CRATE, FLAG_SAIL_CRATE_SUNK, EVENT(EasternAlhafra_FinishPushing)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_ROCK_UNDER_LOG, FLAG_ROCK_UNDER_LOG_SUNK, EVENT(EasternAlhafra_StartPushingRockUnderLog)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_ROCK_UNDER_LOG, FLAG_ROCK_UNDER_LOG_SUNK, EVENT(EasternAlhafra_FinishPushingRockUnderLog)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_EAST_POLE_ROCK, FLAG_EAST_POLE_ROCK_SUNK, EVENT(EasternAlhafra_StartPushing)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_EAST_POLE_ROCK, FLAG_EAST_POLE_ROCK_SUNK, EVENT(EasternAlhafra_FinishPushing)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(EasternAlhafra_StartPushing)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(EasternAlhafra_FinishPushing)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_WEST_POLE, CONDITION_ALWAYS, EVENT(EasternAlhafra_PushAhead)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_WEST_POLE, CONDITION_ALWAYS, EVENT(EasternAlhafra_PushAhead)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_WEST_POLE_TIP, CONDITION_ALWAYS, EVENT(EasternAlhafra_PushAhead)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BESIDE_LOG, CONDITION_ALWAYS, EVENT(EasternAlhafra_RollLog)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_BESIDE_LOG, CONDITION_ALWAYS, EVENT(EasternAlhafra_RollLog)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTHWEST), CELL_WEST_POLE, CONDITION_ALWAYS, EVENT(EasternAlhafra_IgnoreDiagonalStep)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH_NORTHWEST), CELL_WEST_POLE, CONDITION_ALWAYS, EVENT(EasternAlhafra_IgnoreDiagonalStep)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTHEAST), CELL_WEST_POLE, CONDITION_ALWAYS, EVENT(EasternAlhafra_IgnoreDiagonalStep)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH_SOUTHEAST), CELL_WEST_POLE, CONDITION_ALWAYS, EVENT(EasternAlhafra_IgnoreDiagonalStep)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTHWEST), CELL_BESIDE_LOG, CONDITION_ALWAYS, EVENT(EasternAlhafra_IgnoreDiagonalStep)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH_NORTHWEST), CELL_BESIDE_LOG, CONDITION_ALWAYS, EVENT(EasternAlhafra_IgnoreDiagonalStep)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTHEAST), CELL_BESIDE_LOG, CONDITION_ALWAYS, EVENT(EasternAlhafra_IgnoreDiagonalStep)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH_SOUTHEAST), CELL_BESIDE_LOG, CONDITION_ALWAYS, EVENT(EasternAlhafra_IgnoreDiagonalStep)},
    {TRIGGER_STEP, CELL_BY_CABIN_DOOR, CONDITION_ALWAYS, EVENT(EasternAlhafra_StepByCabinDoor)},
    {TRIGGER_STEP, CELL_PIER, FLAG_WORKERS_TOLD_MAYOR, EVENT(EasternAlhafra_WorkersTellMayor)},
    {TRIGGER_STEP, CELL_PIER_FOOT, FLAG_ALHAFRA_SHIP_GONE, EVENT(EasternAlhafra_BriggsStealsShip)},
    {TRIGGER_STEP, CELL_ACROSS_DECK, FLAG_ASKED_TO_HELP, EVENT(EasternAlhafra_AskToHelpWithMast)},
    {TRIGGER_STEP, CELL_BELOW_MAST_BLOCK, FLAG_GAVE_UP_ON_MAST_BLOCK, EVENT(EasternAlhafra_GiveUpOnMastBlock)},
    {TRIGGER_STEP, CELL_BESIDE_MAST_BLOCK, FLAG_REACHED_MAST_BLOCK, EVENT(EasternAlhafra_ReachMastBlock)},
    {TRIGGER_TABLE_END},
};

/// Which of gEasternAlhafra_WaveLines' two tables the screen reads; the other
/// is being filled.
u8 gEasternAlhafra_WaveTableIndex[] = {
    0,
};

/// The two workers' steps about the mayor as they insist the block is gone
/// (EasternAlhafra_WorkersTellMayor), each ending facing west.
s32 gEasternAlhafra_FirstWorkerCheerScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE_BY(0, 0, 8),
    SC_SYNC,
    SC_MOVE_BY(16, 0, 16),
    SC_SYNC,
    SC_MOVE_BY(16, 0, -16),
    SC_SYNC,
    SC_MOVE_BY(0, 0, -16),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(1),
    SC_END,
};

s32 gEasternAlhafra_SecondWorkerCheerScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE_BY(32, 0, 32),
    SC_SYNC,
    SC_MOVE_BY(16, 0, -16),
    SC_SYNC,
    SC_MOVE_BY(2, 0, -6),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(1),
    SC_END,
};

/// The two workers running ahead of the mayor to the pier's foot, each ending
/// turned north-east.
u32 gEasternAlhafra_FirstWorkerRunScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE_BY(48, 0, 0),
    SC_SYNC,
    SC_MOVE(168, 0, 584),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_SET(SW_FACING, FACING_NORTHEAST),
    SC_WAIT(1),
    SC_END,
};

u32 gEasternAlhafra_SecondWorkerRunScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE_BY(48, 0, 0),
    SC_SYNC,
    SC_MOVE(184, 0, 584),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_SET(SW_FACING, FACING_NORTHEAST),
    SC_WAIT(1),
    SC_END,
};

/// The mayor, Jiya and the workers leaving the pier for town, one after another
/// (EasternAlhafra_MayorFindsShipGone).
s32 gEasternAlhafra_RunOffScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(152, 0, 640),
    SC_SYNC,
    SC_MOVE_BY(-80, 0, 0),
    SC_SYNC,
    SC_MOVE_BY(-32, 0, -32),
    SC_SYNC,
    SC_MOVE_BY(-24, 0, 0),
    SC_SYNC,
    SC_WAIT(1),
    SC_END,
};
// clang-format on

u32 gQuestionMark_Script[] = {
    SC_RELEASE,
};
