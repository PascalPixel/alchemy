/// @file
/// Eastern Alhafra, the dock where the mayor's ship lies with its mast fallen.
/// On the first visit the party overhears the workers
/// (EasternAlhafra_OverhearWorkers) and, in the cabin, Briggs, who is fought
/// and caught. The mast is then cleared as a puzzle: blocks and crates pushed
/// or Moved into the water (EasternAlhafra_FinishPushingBlock), the log rolled
/// in (EasternAlhafra_RollLog), the cone Pounded to free the rope
/// (EasternAlhafra_PoundCone), and the stone block on the mast Burst
/// (EasternAlhafra_BurstMastBlock). The workers tell the mayor
/// (EasternAlhafra_WorkersTellMayor), Briggs escapes and sails off in the ship
/// (EasternAlhafra_BriggsStealsShip), and the mayor finds it gone
/// (EasternAlhafra_MayorFindsShipGone). The water ripples by an HBlank wave on
/// BG1 (EasternAlhafra_StartWave).
#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "flags.h"
#include "field/scene_tables.h"
#include "system/main_routine_work.h"
#include "field/event_inline.h"
#include "scene/scene.h"
#include "scene/shared.h"
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
#include "field/field_bank.h"
#include "field/field_event.h"
#include "field/actor_script.h"
#include "fixed_math.h"

/// Story flags of the mast puzzle and its events.
#define FLAG_FIRST_CRATE_SUNK 0x8a0  ///< ACTOR_FIRST_CRATE
#define FLAG_SECOND_CRATE_SUNK 0x8a1 ///< ACTOR_SECOND_CRATE
#define FLAG_CONE_POUNDED 0x8a2
#define FLAG_ROPE_LASHED 0x8a3
#define FLAG_LAST_ROCK_SUNK 0x8a4  ///< ACTOR_LAST_ROCK
#define FLAG_FIRST_ROCK_SUNK 0x8a5 ///< ACTOR_FIRST_ROCK
#define FLAG_LOG_ROLLED 0x8a6
#define FLAG_TALKED_TO_SAILOR 0x8a8
#define FLAG_OVERHEARD_WORKERS 0x8a9
#define FLAG_ASKED_TO_HELP 0x8ac
#define FLAG_ASKED_IF_FINISHED 0x8ad
#define FLAG_REACHED_ROCK 0x8be
#define FLAG_GAVE_UP_ON_ROCK 0x8bf
#define FLAG_WORKERS_SAW_BLOCK_GONE 0x904
#define FLAG_WORKERS_TOLD_MAYOR 0x905

/// The dock's things and people before Briggs is caught, numbering the rows of
/// gEasternAlhafra_Placements whose sprites are not the party's; the things
/// keep their rows in gEasternAlhafra_PlacementsAfterBriggsCaught.
enum EasternAlhafraActorId {
  /// The posts the rope Lash ties is strung between (Rope_Place).
  ///@{
  ACTOR_ROPE_POST = ACTOR_FIRST_PLACED,
  ACTOR_SECOND_ROPE_POST,
  ///@}
  /// The two crates on the mast, pushed or Moved into the water.
  ///@{
  ACTOR_FIRST_CRATE,
  ACTOR_SECOND_CRATE,
  ///@}
  /// The ends of the tightrope from the dock to the mast (Tightrope_Place).
  ///@{
  ACTOR_TIGHTROPE_START,
  ACTOR_TIGHTROPE_END,
  ///@}
  /// The cone Pound drives down to free the mast's ropes.
  ACTOR_CONE,
  /// The log rolled into the water (EasternAlhafra_RollLog).
  ACTOR_LOG,
  /// The rocks on the mast; the last is pushed by its own events.
  ///@{
  ACTOR_LAST_ROCK,
  ACTOR_FIRST_ROCK,
  ///@}
  /// The ends of the two ropes the cone holds taut until it is Pounded.
  ///@{
  ACTOR_FIRST_CONE_ROPE_START,
  ACTOR_FIRST_CONE_ROPE_END,
  ACTOR_SECOND_CONE_ROPE_START,
  ACTOR_SECOND_CONE_ROPE_END,
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
  /// Alhafra's mayor, whose ship this is.
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

struct Actor;
struct RockSprite;
struct Vector3;

struct PushSlide {
  s32 value[6];
};

struct EasternAlhafraSparkWork {
  u8 unk0000[0x1604];
  s16 resource;
};

struct PushState {
  s32 x;
  s32 y;
  s32 z;
  s32 kind;
  s32 number;
  s32 unk14;
};

void EasternAlhafra_PlaceClearedDebris(void);
void Rocks_Spawn(s32, s32, s32, s32);

void EasternAlhafra_StartFallingRocks(void);
void EasternAlhafra_EndFallingRocks(void);

void EasternAlhafra_AskIfFinished(void);
void EasternAlhafra_WorkersSeeBlockGone(void);
void EasternAlhafra_MayorFindsShipGone(void);

extern s32 gEasternAlhafra_FirstWorkerCheerScript[];
extern s32 gEasternAlhafra_SecondWorkerCheerScript[];
extern u32 gEasternAlhafra_FirstWorkerRunScript[];
extern u32 gEasternAlhafra_SecondWorkerRunScript[];
extern s32 gEasternAlhafra_RunOffScript[];

void EasternAlhafra_RollLog(void);

void EasternAlhafra_TalkToSailor(void);

void EasternAlhafra_StepOntoMast(void);

void EasternAlhafra_IgnoreDiagonalStep(void);
void EasternAlhafra_GiveUpOnRock(void);
void EasternAlhafra_AskToHelpWithMast(void);
void EasternAlhafra_WorkersTellMayor(void);
void EasternAlhafra_BriggsStealsShip(void);

extern s16 gEasternAlhafra_CabinDoorFrames[];

void EasternAlhafra_InitializeRockSprite(struct RockSprite *sprite, s32 width,
                                         s32 height, u32 attributes, s32 tile);

void Particles_UpdateFalling(void);

extern u16 gPushKinds[];

struct Actor *PushTarget_FindAhead(s32 *direction, s32 *number, s32 *kind);

s32 MapGrid_CheckCollision(struct Actor *actor, struct Vector3 *cell);

void Actor_StopAtPushTarget(struct Actor *object);

struct Actor *Actor_FindAtCell(s32 *position, s32 mode);

void Actor_SetCellOpen(s32 id, u32 kind, s32 flag, s32 layer);

s32 Actor_GetCellIndex(struct Actor *object);

s32 Tightrope_Place();
s32 Tightrope_Release();
s32 Leader_CanPushObjectAhead();
s32 EasternAlhafra_PushObjectAhead();
extern u16 gEasternAlhafra_WaveLines[2][160];
void Actor_FallOverWithAnimationById();
void Rope_Initialize(void);
void Tightrope_Initialize(void);
void MapGrid_SetAttributeBAt();

extern s16 gEasternAlhafra_PoundCone[];
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

void EasternAlhafra_BurstMastBlock(s32 mode, s32 id) {
  if (id == 0)
    EasternAlhafra_StartFallingRocks();
  if (mode == 1) {
    Event_SetFlag(FLAG_ALHAFRA_MAST_CLEARED);
    EasternAlhafra_PlaceClearedDebris();
  }
  if (mode == 2)
    Rocks_Spawn(px(498), px(16), px(412), 2);
  if (mode == 3)
    Rocks_Spawn(px(498), px(16), px(412), 30);
  if (id == 0x3e7)
    EasternAlhafra_EndFallingRocks();
}

void EasternAlhafra_PoundCone(s32 mode, s32 arg) {
  struct Actor *a;
  struct Actor *b;
  s32 i;

  Pound_SinkTarget(gEasternAlhafra_PoundCone, mode, arg);
  if (mode == 1) {
    a = Actor_Get(ACTOR_FIRST_CONE_ROPE_END);
    b = Actor_Get(ACTOR_SECOND_CONE_ROPE_END);
    a->speedLimit = px(4);
    a->acceleration = px(1) / 2;
    b->acceleration = px(1) / 2;
    b->speedLimit = px(4);
    FieldActor_SetPositionAndResetMotion(a, px(488), a->y.fixed, px(312));
    FieldActor_SetMoveTarget(a, 440, 0, 312);
    FieldActor_SetPositionAndResetMotion(b, px(488), b->y.fixed, px(312));
    FieldActor_SetMoveTarget(b, 440, 0, 376);
    Tightrope_Release(1);
    Tightrope_Release(2);
    i = 0;
    do {
      Tightrope_Place(1, ACTOR_FIRST_CONE_ROPE_START,
                      ACTOR_FIRST_CONE_ROPE_END);
      Tightrope_Place(2, ACTOR_SECOND_CONE_ROPE_START,
                      ACTOR_SECOND_CONE_ROPE_END);
      System_Wait(1);
      Tightrope_Release(1);
      Tightrope_Release(2);
      if (Actor_HasArrived(a) != 0 && Actor_HasArrived(b) != 0)
        break;
      i++;
    } while (i < 60);
  }
}

void EasternAlhafra_StartPushingBlock(s32 unused, s32 id) {
  struct Actor *leader;
  struct Actor *object;
  s32 leaderX;
  s32 x;
  s32 z;

  leader = Actor_Get(gMainRoutineWork.leaderActorId);
  object = Actor_Get(id);
  object->motionFlags = 0;
  if (id == ACTOR_FIRST_CRATE) {
    leaderX = FIELD_CELL(leader->x.fixed);
    x = FIELD_CELL(object->x.fixed);
    z = FIELD_CELL(object->z.fixed);
    if (x == 34 && z == 32 && leaderX == 33)
      FieldMapGrid_SetCellBehavior(px(528), px(512), 2, 255);
    if (x == 33 && z == 32 && leaderX == 32)
      FieldMapGrid_SetCellBehavior(px(512), px(512), 2, 255);
  }
}

#include "text/msg_ids.h"

extern s32 gEasternAlhafra_CabinDoor[];

s32 Party_FindOwnerWithAction();
void Event_Start();
void Event_End();

void EasternAlhafra_FinishPushingBlock(s32 mode, s32 id) {
  struct Actor *object;
  struct Actor *sparks[4];
  struct Actor *spark;
  s32 x;
  s32 z;
  s32 hit;
  s32 i;

  object = Actor_Get(id);
  x = FIELD_CELL(object->x.fixed);
  hit = 0;
  z = FIELD_CELL(object->z.fixed);
  if (id == ACTOR_FIRST_CRATE) {
    FieldMapGrid_SetCellBehavior(px(528), px(512), 2, 0);
    FieldMapGrid_SetCellBehavior(px(512), px(512), 2, 0);
  }
  if (x == 31 && z == 32) {
    Event_SetFlag(FLAG_FIRST_CRATE_SUNK);
    hit = 1;
  }
  if (x == 25 && z == 33) {
    Event_SetFlag(FLAG_SECOND_CRATE_SUNK);
    hit = 1;
  }
  if (x == 21 && z == 33) {
    Event_SetFlag(FLAG_LAST_ROCK_SUNK);
    hit = 1;
  }
  if (x == 27 && z == 33) {
    Event_SetFlag(FLAG_FIRST_ROCK_SUNK);
    hit = 1;
  }
  if (hit) {
    if (id != ACTOR_FIRST_CRATE)
      Event_PlaySound(SE_310);
    object->gravity = 0x170a;
    object->motionFlags = 3;
    Actor_SetSpriteFlags(Actor_Get(id), 1);
    System_Wait(5);
    for (i = 0; i < 60 && object->y.fixed > 0; i++)
      System_Wait(1);
    Event_PlaySound(SE_STOP_MUSIC_PLAYER_5);
    System_Wait(1);
    Event_PlaySound(SE_LOG_FALL);
    object->collisionFlags = 0;
    object->priorityFlags |= 2;
    Actor_SelectAnimation(object, 4);
    i = 0;
  create:
    spark = (FieldActor_Create(276, object->x.fixed, object->y.fixed,
                               object->z.fixed));
    sparks[i] = spark;
    spark->scaleX = 0xa666;
    Actor_SelectAnimation(spark, 1);
    Actor_SetSpriteFlags(spark, 0);
    Event_PlaySound(SE_CAST);
    System_Wait(5);
    i++;
    if (i <= 2)
      goto create;
    EasternAlhafra_PlaceClearedDebris();
    FieldActor_Stop(gMainRoutineWork.leaderActorId);
    System_Wait(30);
    for (i = 0; i <= 2; i++)
      Actor_Release(sparks[i]);
  }
}

void EasternAlhafra_StartPushingLastRock(s32 unused, s32 id) {
  EasternAlhafra_StartPushingBlock(unused, id);
  FieldMapGrid_SetCellBehavior(px(320), px(464), 2, 255);
  FieldMapGrid_SetCellBehavior(px(320), px(480), 2, 255);
  FieldMapGrid_SetCellBehavior(px(320), px(496), 2, 255);
  FieldMapGrid_SetCellBehavior(px(320), px(512), 2, 255);
  FieldMapGrid_SetCellBehavior(px(352), px(464), 2, 255);
  FieldMapGrid_SetCellBehavior(px(352), px(480), 2, 255);
  FieldMapGrid_SetCellBehavior(px(352), px(496), 2, 255);
  FieldMapGrid_SetCellBehavior(px(352), px(512), 2, 255);
  FieldMapGrid_SetCellBehavior(px(336), px(528), 2, 0);
}

void EasternAlhafra_FinishPushingLastRock(s32 mode, s32 id) {
  struct Actor *object;
  s32 x;
  s32 z;
  s32 open;

  object = Actor_Get(id);
  x = FIELD_CELL(object->x.fixed);
  z = FIELD_CELL(object->z.fixed);
  open = 0;
  if (x == 21 && z == 33)
    open = 1;
  FieldMapGrid_SetCellBehavior(px(320), px(464), 2, 0);
  FieldMapGrid_SetCellBehavior(px(320), px(480), 2, 0);
  FieldMapGrid_SetCellBehavior(px(320), px(496), 2, 0);
  FieldMapGrid_SetCellBehavior(px(320), px(512), 2, 0);
  FieldMapGrid_SetCellBehavior(px(352), px(464), 2, 0);
  FieldMapGrid_SetCellBehavior(px(352), px(480), 2, 0);
  FieldMapGrid_SetCellBehavior(px(352), px(496), 2, 0);
  FieldMapGrid_SetCellBehavior(px(352), px(512), 2, 0);
  FieldMapGrid_SetCellBehavior(px(336), px(528), 2, 255);
  if (open)
    FieldCamera_MoveTo(336, CAMERA_KEEP, 560, 1);
  EasternAlhafra_FinishPushingBlock(mode, id);
  if (open)
    EventCamera_FollowActor(gMainRoutineWork.leaderActorId, 1);
}

void EasternAlhafra_OverhearWorkers(void);
/* VIEW(s0514): routine PushTarget_Push as void PushTarget_Push(struct
 * PushSlide); eastern_alhafra has void PushTarget_Push(struct DoorPlan); tried
 * as the definition's void PushTarget_Push(struct PushTarget target), which the
 * object already declares, EasternAlhafra_RollLog moves: 1 of 198 instructions
 * in tla-ja. */
extern void PushTarget_Push__log(struct PushSlide) __asm__("PushTarget_Push");

void EasternAlhafra_RollLog(void) {
  struct Actor *log;
  struct Actor *lastRock;
  struct PushSlide unk;
  s32 facing;
  s32 i;

  log = Actor_GetById(ACTOR_LOG);
  lastRock = Actor_GetById(ACTOR_LAST_ROCK);
  facing =
      (Actor_GetById(gMainRoutineWork.leaderActorId)->facing + 0x2000) & 0xc000;
  if (lastRock != NULL && Event_GetFlag(FLAG_LAST_ROCK_SUNK) == 0)
    MapGrid_SetCellBehavior(lastRock->x.fixed, lastRock->z.fixed, 2, 255);
  if (PushTarget_FindSlide(&unk) != 0) {
    if (Event_GetFlag(FLAG_LAST_ROCK_SUNK) == 0 || facing == 0xc000) {
      PushTarget_Push__log(unk);
    } else {
      Event_SetFlag(FLAG_LOG_ROLLED);
      Event_Start();
      FieldEvent_ResolvePendingActions(0);
      FieldCamera_MoveTo(336, CAMERA_KEEP, 560, 1);
      PushTarget_Push__log(unk);
      MapGrid_SetCellBehavior(log->x.fixed - px(24), log->z.fixed, 0, 0);
      MapGrid_SetCellBehavior(log->x.fixed + px(8), log->z.fixed, 0, 0);
      MapGrid_SetCellBehavior(log->x.fixed - px(24), log->z.fixed, 2, 0);
      MapGrid_SetCellBehavior(log->x.fixed - px(8), log->z.fixed, 2, 0);
      MapGrid_SetCellBehavior(log->x.fixed + px(8), log->z.fixed, 2, 0);
      MapGrid_SetCellBehavior(log->x.fixed + px(24), log->z.fixed, 2, 0);
      Actor_SetAnimation(ACTOR_LOG, 3);
      Actor_SetDestinationOffset(ACTOR_LOG, 0, 16);
      FieldEvent_Wait(20);
      log->gravity = 0xccc;
      log->motionFlags = 3;
      i = 0;
      if (log->y.fixed > 0) {
        do {
          System_Wait(1);
          i++;
        } while (i <= 59 && log->y.fixed > 0);
      }
      EasternAlhafra_PlaceClearedDebris();
      Event_PlaySound(SE_LOG_FALL);
      Actor_SetAnimation(ACTOR_LOG, 8);
      FieldEvent_Wait(60);
      EventCamera_FollowActor(gMainRoutineWork.leaderActorId, 1);
      Event_End();
    }
  }
  if (lastRock != NULL && Event_GetFlag(FLAG_LAST_ROCK_SUNK) == 0)
    MapGrid_SetCellBehavior(lastRock->x.fixed, lastRock->z.fixed, 2, 0);
}

void EasternAlhafra_PushAhead(s32 mode) {
  s32 tile;

  tile = Field_FindNearbyTalkTarget(gMainRoutineWork.leaderActorId);
  if (tile == 16) {
    EasternAlhafra_StartPushingLastRock(mode, ACTOR_LAST_ROCK);
    if (Leader_CanPushObjectAhead() != 0)
      EasternAlhafra_PushObjectAhead();
    EasternAlhafra_FinishPushingLastRock(mode, ACTOR_LAST_ROCK);
  } else if (tile == 15) {
    EasternAlhafra_RollLog();
  }
}

void EasternAlhafra_TalkToSailor(void) {
  Event_SetFlag(FLAG_TALKED_TO_SAILOR);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId(MsgWhatSYour);
  Actor_ShowEmote(ACTOR_SAILOR_GIVING_UP, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Actor_FaceActor(ACTOR_SAILOR_GIVING_UP, gMainRoutineWork.leaderActorId, 0);
  FieldEvent_Wait(20);
  // "What's your problem?"
  FieldEvent_ShowMessageThenWait(ACTOR_SAILOR_GIVING_UP, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_SAILOR_GIVING_UP, 2);
  FieldEvent_Wait(10);
  // "You're new in town, aren't you?" Yes: "You're probably curious about this
  // here ship..." No: "Well, I've never seen you before, but whatever..."
  Event_OpenMessage(22, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(4, 0) != 0) {
    FieldEvent_Wait(30);
    // "Well, I've never seen you before, but whatever..."
    FieldEvent_ShowMessageThenWait(ACTOR_SAILOR_GIVING_UP, 0, 0);
  } else {
    FieldEvent_Wait(20);
    gFieldWork->message += 1;
    // "You're probably curious about this here ship, huh?" Yes: "They call this
    // a "sailing" ship... Wind is..." No: "Masted ships like this were
    // originally built..."
    Event_OpenMessage(22, 0);
    if (FieldEvent_AskYesNoWithActorAnimation(4, 0) != 0) {
      FieldEvent_Wait(30);
      // "Masted ships like this were originally built by the Champa... Oh..."
      FieldEvent_ShowMessageThenWait(ACTOR_SAILOR_GIVING_UP, 0, 0);
    } else {
      FieldEvent_Wait(20);
      gFieldWork->message += 1;
      // "They call this a "sailing" ship... Wind is the only thing that'll..."
      // Yes: "Yeah, well, think how surprised we were to..." No: "You kids
      // today, with your "been there, done..."
      Event_OpenMessage(22, 0);
      if (FieldEvent_AskYesNoWithActorAnimation(4, 0) != 0) {
        FieldEvent_Wait(30);
        // "You kids today, with your "been there, done that" attitudes...
        // It's..."
        FieldEvent_ShowMessageThenWait(ACTOR_SAILOR_GIVING_UP, 0, 0);
      } else {
        FieldEvent_Wait(20);
        gFieldWork->message += 1;
        // "Yeah, well, think how surprised we were to find that Alhafra had..."
        FieldEvent_ShowMessageThenWait(ACTOR_SAILOR_GIVING_UP, 0, 0);
      }
    }
  }
  Actor_FaceDirection(ACTOR_SAILOR_GIVING_UP, FACING_EAST, 0);
  Event_End();
}

void EasternAlhafra_StepOntoMast(void) {
  if (Event_GetFlag(FLAG_ALHAFRA_MAST_CLEARED)) {
    if (Event_GetFlag(FLAG_WORKERS_SAW_BLOCK_GONE) == 0) {
      Event_SetFlag(FLAG_WORKERS_SAW_BLOCK_GONE);
      EasternAlhafra_WorkersSeeBlockGone();
    }
  } else if (Event_GetFlag(FLAG_BRIGGS_CAUGHT)) {
    if (Event_GetFlag(FLAG_FIRST_CRATE_SUNK) &&
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

/// Stepping into a door: it opens, the leader walks through and the scene
/// leaves by the exit given.
void EasternAlhafra_OpenDoorAndEnter(struct DoorStep *step, s32 countdown,
                                     s32 unused) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Event_PlaySound(SE_DOOR_OPEN);
  MapGrid_AnimateCells(step->script, step->x, step->z);
  Actor_Get(gMainRoutineWork.leaderActorId)->motionFlags = 2;
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 0.5);
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_WALK);
  Actor_CenterAndWalk(gMainRoutineWork.leaderActorId, 2, -8);
  FieldEvent_Wait(10);
  FieldEvent_SetExitRequest(countdown);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_End();
}

void EasternAlhafra_EnterCabin(s32 countdown) {
  EasternAlhafra_OpenDoorAndEnter(gEasternAlhafra_CabinDoor, countdown, 0);
}

void EasternAlhafra_IgnoreDiagonalStep(void) {}

void EasternAlhafra_ReachRock(void) { GameFlag_Set(FLAG_REACHED_ROCK); }

void EasternAlhafra_GiveUpOnRock(void) {
  struct Actor *leader;

  goto check;
run:
  if (Party_FindOwnerWithAction(ABILITY_BURST) != -1)
    return;
  Event_SetFlag(FLAG_GAVE_UP_ON_ROCK);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_WalkToAndWait(ACTOR_FELIX, 488, 440);
  FieldActor_LaunchFromLeader(ACTOR_JENNA, -16, 0, FACING_EAST);
  Actor_FaceDirectionAndWait(ACTOR_FELIX, FACING_WEST);
  FieldEvent_Wait(20);
  FieldEvent_SetMessageId(MsgIDonTKnowWhatWe);
  // "I don't know what we're going to do about this rock... I think we've..."
  FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 5);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_WALK);
  leader = Actor_Get(gMainRoutineWork.leaderActorId);
  if (leader != NULL)
    Actor_SetDestination(ACTOR_JENNA, ((s16 *)leader)[5], ((s16 *)leader)[9]);
  FieldActor_WaitForMoveById(ACTOR_JENNA);
  Actor_SetPosition(ACTOR_JENNA, 0, 0);
  Event_End();
  return;
check:
  if (Event_GetFlag(FLAG_BRIGGS_CAUGHT) &&
      Event_GetFlag(FLAG_ALHAFRA_MAST_CLEARED) == 0 &&
      Event_GetFlag(FLAG_REACHED_ROCK))
    goto run;
}

const struct Trigger *Scene_GetTriggers(void) { return gSceneEvents; }

extern u8 gEasternAlhafra_WaveTableIndex[];

// BG1's wave: each frame the finished table of scanline offsets is handed to
// an HBlank DMA, while the other table is filled for the next frame.

/// Starts DMA 0 copying the finished table into BG1's horizontal offset at
/// each HBlank.
// With ☀️'s way of stopping DMA 0.
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

/// Fills the other table: each of the 160 lines takes layer 2's scroll plus a
/// sine of the frame count less the line.
void EasternAlhafra_FillWaveLines(void) {
  struct MapLayer *layer = &Ram_HeapSlots->mapGrid->layers[2];
  u16 *line;
  s32 i = 0;

  line = gEasternAlhafra_WaveLines[gEasternAlhafra_WaveTableIndex[0] ^ 1];
  for (i = 0; i != 160; i++) {
    *line++ =
        FIELD_PIXEL(layer->y) +
        (Math_Sin(((u8)gFrameCount - i - FIELD_PIXEL(layer->y) + 160) << 9) >>
         15);
  }
  gEasternAlhafra_WaveTableIndex[0] ^= 1;
}

void EasternAlhafra_StartWave(void) {
  Task_AddFrame(EasternAlhafra_FillWaveLines, TASK_PASS_VBLANK);
  Task_AddFrame(EasternAlhafra_StartWaveDma, TASK_PASS_FRAME);
}

#define EVENT_WORK ((u8 *)Ram_HeapSlots->fieldWork)

void Pillars_Place();

s32 Scene_Initialize(void) {
  struct Actor *object;
  s32 flag;

  ((struct FieldWork *)EVENT_WORK)->transition = 0x100;
  EasternAlhafra_StartWave();
  Actor_Get(gMainRoutineWork.leaderActorId)->priorityFlags |= 32;
  if (Event_GetFlag(FLAG_ALHAFRA_MAST_CLEARED) != 0) {
    if (Event_GetFlag(FLAG_WORKERS_TOLD_MAYOR) != 0 &&
        Event_GetFlag(FLAG_ALHAFRA_SHIP_GONE) == 0) {
      Actor_SetPosition(ACTOR_YOUNG_MAN_KNOCKED_DOWN, 184, 616);
      Actor_SetPosition(ACTOR_MAN_KNOCKED_DOWN, 136, 600);
      Actor_SetPosition(ACTOR_FIRST_WORKER, 168, 584);
      Actor_FaceDirection(ACTOR_FIRST_WORKER, FACING_SOUTH, 0);
      Actor_FallOverWithAnimationById(ACTOR_YOUNG_MAN_KNOCKED_DOWN, 0, 13);
      Call3(Actor_FallOverWithAnimationById, 31, 0x8000, 13);
      Actor_FallOverWithAnimationById(ACTOR_FIRST_WORKER, 0, 13);
      Actor_SetPosition(ACTOR_ALHAFRAS_MAYOR, 112, 432);
      Actor_SetPosition(ACTOR_JIYA, 176, 408);
      Actor_SetPosition(ACTOR_SECOND_WORKER, 152, 376);
      Actor_FaceDirection(ACTOR_SECOND_WORKER, FACING_SOUTH, 0);
      FieldActor_SetSpriteRotationById(ACTOR_ALHAFRAS_MAYOR, 0x4000);
      Actor_SetSpriteFlags((s32)Actor_Get(ACTOR_ALHAFRAS_MAYOR), 0);
      FieldActor_SetSpriteRotationById(ACTOR_JIYA, 0xc000);
      Actor_SetSpriteFlags((s32)Actor_Get(ACTOR_JIYA), 0);
      FieldActor_SetSpriteRotationById(ACTOR_SECOND_WORKER, 0x4000);
      Actor_SetSpriteFlags((s32)Actor_Get(ACTOR_SECOND_WORKER), 0);
      Actor_SetAnimation(ACTOR_ALHAFRAS_MAYOR, 13);
      Actor_SetAnimation(ACTOR_JIYA, 13);
      Actor_SetAnimation(ACTOR_SECOND_WORKER, 13);
    }
    if (Event_GetFlag(FLAG_WORKERS_TOLD_MAYOR) != 0) {
      MapGrid_CopyCellsLayered(40, 0, 24, 31, 11, 6);
      MapGrid_CopyCellsLayered(84, 15, 10, 10, 75, 15);
      MapGrid_CopyCellAttributes(10, 24, 1, 1, 11, 24);
      Actor_SetPosition(ACTOR_SECOND_ROPE_POST, 0, 0);
      Actor_SetPosition(ACTOR_ROPE_POST, 0, 0);
      Actor_SetPosition(ACTOR_FIRST_CRATE, 0, 0);
      Actor_SetPosition(ACTOR_SECOND_CRATE, 0, 0);
      Actor_SetPosition(ACTOR_LOG, 0, 0);
    }
  }
  if (gMainRoutineWork.entrance == 99) {
    gMainRoutineWork.returnEntrance = 1;
    gMainRoutineWork.entrance = 1;
    Actor_SetPosition(ACTOR_FELIX, 136, 552);
    Actor_SetPosition(ACTOR_KRADEN, 120, 552);
    Actor_SetPosition(ACTOR_YOUNG_MAN_KNOCKED_DOWN, 184, 616);
    Actor_SetPosition(ACTOR_MAN_KNOCKED_DOWN, 136, 600);
    Actor_SetPosition(ACTOR_FIRST_WORKER, 160, 584);
    FieldActor_SetSpriteRotationById(ACTOR_YOUNG_MAN_KNOCKED_DOWN, 0x4000);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_YOUNG_MAN_KNOCKED_DOWN), 0);
    FieldActor_SetSpriteRotationById(ACTOR_MAN_KNOCKED_DOWN, 0xc000);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_MAN_KNOCKED_DOWN), 0);
    FieldActor_SetSpriteRotationById(ACTOR_FIRST_WORKER, 0x4000);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_FIRST_WORKER), 0);
    FieldActor_SetSpritePriority(ACTOR_JIYA, 2);
    FieldActor_SetSpritePriority(ACTOR_ALHAFRAS_MAYOR, 2);
    FieldActor_SetSpritePriority(ACTOR_SECOND_WORKER, 2);
    Actor_SetPosition(ACTOR_ALHAFRAS_MAYOR, 160, 392);
    Actor_SetPosition(ACTOR_JIYA, 176, 408);
    Actor_SetPosition(ACTOR_SECOND_WORKER, 152, 376);
    FieldActor_SetSpriteRotationById(ACTOR_JIYA, 0xc000);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_JIYA), 0);
    FieldActor_SetSpriteRotationById(ACTOR_SECOND_WORKER, 0x4000);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_SECOND_WORKER), 0);
    Actor_FaceDirection(ACTOR_ALHAFRAS_MAYOR, FACING_NORTH_NORTHEAST, 0);
    Actor_FaceDirection(ACTOR_SECOND_WORKER, FACING_SOUTH, 0);
    Actor_SetAnimation(ACTOR_JIYA, 13);
    Actor_SetAnimation(ACTOR_SECOND_WORKER, 13);
    Actor_SetAnimation(ACTOR_YOUNG_MAN_KNOCKED_DOWN, 13);
    Actor_SetAnimation(ACTOR_MAN_KNOCKED_DOWN, 13);
    Actor_SetAnimation(ACTOR_FIRST_WORKER, 13);
    EasternAlhafra_MayorFindsShipGone();
  }
  EasternAlhafra_PlaceClearedDebris();
  if (Event_GetFlag(FLAG_LOG_ROLLED) == 0)
    PushTarget_FillFootprint(ACTOR_LOG);
  Rope_Initialize();
  Rope_Place(0, FLAG_ROPE_LASHED, ACTOR_ROPE_POST, ACTOR_SECOND_ROPE_POST);
  Actor_Get(ACTOR_ROPE_POST)->priorityFlags |= 32;
  Actor_Get(ACTOR_SECOND_ROPE_POST)->priorityFlags |= 32;
  Tightrope_Initialize();
  Tightrope_Place(0, ACTOR_TIGHTROPE_START, ACTOR_TIGHTROPE_END);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_TIGHTROPE_START), 0);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_TIGHTROPE_END), 0);
  Pillars_Place(gEasternAlhafra_PoundCone);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_CONE), 0);
  flag = Event_GetFlag(FLAG_CONE_POUNDED);
  if (flag == 0) {
    object = Actor_Get(ACTOR_FIRST_CONE_ROPE_START);
    /* FAKEMATCH(dead): the solid bit set in a once-only block. */
    do {
      object->collisionFlags |= 8;
    } while (0);
    object = Actor_Get(ACTOR_FIRST_CONE_ROPE_END);
    object->motionFlags = flag;
    object->y.fixed = px(16);
    Tightrope_Place(1, ACTOR_FIRST_CONE_ROPE_START, ACTOR_FIRST_CONE_ROPE_END);
    Tightrope_Settle(1);
    object = Actor_Get(ACTOR_SECOND_CONE_ROPE_START);
    object->collisionFlags |= 8;
    object = Actor_Get(ACTOR_SECOND_CONE_ROPE_END);
    object->motionFlags = flag;
    object->y.fixed = px(16);
    Tightrope_Place(2, ACTOR_SECOND_CONE_ROPE_START,
                    ACTOR_SECOND_CONE_ROPE_END);
  }
  Actor_SetSpriteFlags(Actor_Get(ACTOR_LAST_ROCK), 0);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_FIRST_ROCK), 0);
  FieldActor_SetSpritePriority(ACTOR_FIRST_CRATE, 2);
  Actor_Get(ACTOR_SAILOR_WHO_PAID_FOR_THE_SHIP)->riseCounter = 1;
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

void EasternAlhafra_PlaceClearedDebris(void) {
  struct Actor *object;

  if (Event_GetFlag(FLAG_LOG_ROLLED)) {
    object = Actor_Get(ACTOR_LOG);
    Actor_SetPosition(ACTOR_LOG, 336, 536);
    Actor_SetAnimation(ACTOR_LOG, 4);
    object->priorityFlags |= 2;
    object->motionFlags = 2;
    object->velocityY = 0;
    object->y.fixed = -px(4);
    object->groundHeight = -px(4);
    FieldMapGrid_SetCellBehavior(px(336), px(528), 2, 0);
    FieldMapGrid_SetCellBehavior(px(336), px(528), 0, 0);
    FieldMapGrid_SetCellBehavior(px(352), px(528), 0, 0);
  }
  if (Event_GetFlag(FLAG_LAST_ROCK_SUNK)) {
    Actor_SetPosition(ACTOR_LAST_ROCK, 0, 0);
  }
  if (Event_GetFlag(FLAG_FIRST_ROCK_SUNK)) {
    Actor_SetPosition(ACTOR_FIRST_ROCK, 0, 0);
  }
  if (Event_GetFlag(FLAG_FIRST_CRATE_SUNK)) {
    object = Actor_Get(ACTOR_FIRST_CRATE);
    Actor_SetPosition(ACTOR_FIRST_CRATE, 504, 520);
    Actor_SetAnimation(ACTOR_FIRST_CRATE, 2);
    object->collisionFlags = 0;
    /* FAKEMATCH(dead): an empty once-only block ends the scheduling block here.
     */
    do {
    } while (0);
    object->priorityFlags |= 2;
    FieldMapGrid_SetCellBehavior(object->x.fixed, object->z.fixed, 0, 0);
    MapGrid_SetAttributeBAt(object->x.fixed, object->z.fixed, 0, 128);
  }
  if (Event_GetFlag(FLAG_SECOND_CRATE_SUNK)) {
    object = Actor_Get(ACTOR_SECOND_CRATE);
    Actor_SetPosition(ACTOR_SECOND_CRATE, 408, 536);
    Actor_SetAnimation(ACTOR_SECOND_CRATE, 2);
    object->collisionFlags = 0;
    /* FAKEMATCH(dead): an empty once-only block ends the scheduling block here.
     */
    do {
    } while (0);
    object->priorityFlags |= 2;
    FieldMapGrid_SetCellBehavior(object->x.fixed, object->z.fixed, 0, 0);
    MapGrid_SetAttributeBAt(object->x.fixed, object->z.fixed, 0, 128);
  }
  if (Event_GetFlag(FLAG_ALHAFRA_MAST_CLEARED)) {
    MapGrid_CopyCells(32, 24, 2, 2, 30, 24);
  }
}

void EasternAlhafra_OverhearWorkers(void) {
  struct Actor *leader;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
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
  EventCamera_MoveToActor(ACTOR_SECOND_CRATE, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(10);
  // "If we could just get some decent footing, we could have this thing..."
  FieldEvent_ShowMessageThenWait(ACTOR_SAILOR_WHO_PAID_FOR_THE_SHIP, 0, 10);
  EventCamera_MoveToActor(ACTOR_SAILOR_WAITING_FOR_HIS_PAL, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_SAILOR_WAITING_FOR_HIS_PAL, 4);
  // "There's more to it than that, though..."
  FieldEvent_ShowMessageThenWait(ACTOR_SAILOR_WAITING_FOR_HIS_PAL, 0, 10);
  Actor_FaceDirection(ACTOR_SAILOR_WAITING_FOR_HIS_PAL, FACING_NORTHEAST, 0);
  FieldEvent_Wait(20);
  EventCamera_MoveToActor(ACTOR_SAILOR_LONGING_FOR_CHAMPA, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(10);
  Actor_FaceDirection(ACTOR_SAILOR_LONGING_FOR_CHAMPA, FACING_NORTHEAST, 0);
  FieldEvent_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_SAILOR_LONGING_FOR_CHAMPA, 3);
  FieldEvent_Wait(10);
  // "We still have to do something about that rope over there."
  FieldEvent_ShowMessageThenWait(ACTOR_SAILOR_LONGING_FOR_CHAMPA, 0, 10);
  Actor_FaceDirection(ACTOR_SAILOR_GIVING_UP, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_SAILOR_WHO_PAID_FOR_THE_SHIP,
                      FACING_NORTH_NORTHEAST, 0);
  FieldEvent_Wait(20);
  EventCamera_MoveToActor(ACTOR_CONE, 1);
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
  Actor_SetAnimationAndWait(ACTOR_SAILOR_WAITING_FOR_HIS_PAL, 4);
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
  Actor_SetAnimation(ACTOR_SAILOR_GIVING_UP, 3);
  Actor_SetAnimation(ACTOR_SAILOR_WHO_PAID_FOR_THE_SHIP, 3);
  Actor_SetAnimationAndWait(ACTOR_SAILOR_WAITING_FOR_HIS_PAL, 3);
  FieldEvent_Wait(20);
  Actor_FaceDirection(ACTOR_SAILOR_GIVING_UP, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_SAILOR_WHO_PAID_FOR_THE_SHIP,
                      FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_SAILOR_WAITING_FOR_HIS_PAL, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_SAILOR_LONGING_FOR_CHAMPA, FACING_NORTHEAST, 0);
  FieldEvent_Wait(20);
  EventCamera_MoveToActor(gMainRoutineWork.leaderActorId, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(20);
  Actor_SetSpeed(ACTOR_JENNA, 1.2);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_WALK);
  leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  if (leader != 0) {
    Actor_SetDestination(ACTOR_JENNA, leader->x.part[0].pixel,
                         leader->z.part[0].pixel);
  }
  FieldActor_WaitForMoveById(ACTOR_JENNA);
  Actor_SetPosition(ACTOR_JENNA, 0, 0);
  Actor_SetSpeed(ACTOR_SHEBA, 1.2);
  Actor_SetAnimation(ACTOR_SHEBA, ANIM_WALK);
  leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  if (leader != 0) {
    Actor_SetDestination(ACTOR_SHEBA, leader->x.part[0].pixel,
                         leader->z.part[0].pixel);
  }
  FieldActor_WaitForMoveById(ACTOR_SHEBA);
  Actor_SetPosition(ACTOR_SHEBA, 0, 0);
  Actor_SetSpeed(ACTOR_KRADEN, 1.2);
  Actor_SetAnimation(ACTOR_KRADEN, 2);
  leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  if (leader != 0) {
    Actor_SetDestination(ACTOR_KRADEN, leader->x.part[0].pixel,
                         leader->z.part[0].pixel);
  }
  FieldActor_WaitForMoveById(ACTOR_KRADEN);
  Actor_SetPosition(ACTOR_KRADEN, 0, 0);
  EventCamera_FollowActor(gMainRoutineWork.leaderActorId, 1);
  FieldCamera_WaitForArrival();
  Event_End();
  return;
}

void EasternAlhafra_AskToHelpWithMast(void) {
  struct Actor *leader;

  if (Event_GetFlag(FLAG_BRIGGS_CAUGHT) == 0) {
    goto L_020097dc;
  }
  if (Event_GetFlag(FLAG_ALHAFRA_MAST_CLEARED) != 0) {
    goto L_020097dc;
  }
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
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
  Actor_SetAnimationAndWait(ACTOR_KRADEN, 3);
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
    Actor_SetSpeed(ACTOR_PIERS, 1.2);
    Actor_SetAnimation(ACTOR_PIERS, ANIM_WALK);
    leader = Actor_GetById(gMainRoutineWork.leaderActorId);
    if (leader != 0) {
      Actor_SetDestination(ACTOR_PIERS, leader->x.part[0].pixel,
                           leader->z.part[0].pixel);
    }
    FieldActor_WaitForMoveById(ACTOR_PIERS);
    Actor_SetPosition(ACTOR_PIERS, 0, 0);
  }
  Actor_SetSpeed(ACTOR_JENNA, 1.2);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_WALK);
  leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  if (leader != 0) {
    Actor_SetDestination(ACTOR_JENNA, leader->x.part[0].pixel,
                         leader->z.part[0].pixel);
  }
  FieldActor_WaitForMoveById(ACTOR_JENNA);
  Actor_SetPosition(ACTOR_JENNA, 0, 0);
  Actor_SetSpeed(ACTOR_SHEBA, 1.2);
  Actor_SetAnimation(ACTOR_SHEBA, ANIM_WALK);
  leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  if (leader != 0) {
    Actor_SetDestination(ACTOR_SHEBA, leader->x.part[0].pixel,
                         leader->z.part[0].pixel);
  }
  FieldActor_WaitForMoveById(ACTOR_SHEBA);
  Actor_SetPosition(ACTOR_SHEBA, 0, 0);
  Actor_SetSpeed(ACTOR_KRADEN, 1.2);
  Actor_SetAnimation(ACTOR_KRADEN, 2);
  leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  if (leader != 0) {
    Actor_SetDestination(ACTOR_KRADEN, leader->x.part[0].pixel,
                         leader->z.part[0].pixel);
  }
  FieldActor_WaitForMoveById(ACTOR_KRADEN);
  Actor_SetPosition(ACTOR_KRADEN, 0, 0);
  Event_End();
L_020097dc:
  return;
}

void EasternAlhafra_AskIfFinished(void) {
  struct Actor *leader;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
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
  Event_OpenMessage(26, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(4, 0) == 0) {
    FieldEvent_Wait(20);
    Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
    FieldEvent_ShowMessageThenWait(ACTOR_SHEBA, 0, 10);
    Actor_FaceDirection(ACTOR_FELIX, FACING_NORTHEAST, 0);
    Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
    FieldEvent_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_SHAKE_HEAD);
    FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 10);
    gFieldWork->message += 2;
  } else {
    gFieldWork->message += 2;
    FieldEvent_Wait(40);
    Actor_SetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
    Actor_FaceDirection(ACTOR_FELIX, FACING_NORTHEAST, 0);
    FieldEvent_Wait(20);
    FieldEvent_ShowMessageThenWait(ACTOR_SHEBA, 0, 10);
    Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_SHAKE_HEAD);
    FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 10);
  }
  if (((s32 (*)())GameFlag_Get)(7) != 0) {
    Actor_FaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
    Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 0);
    Actor_FaceDirectionAndWait(ACTOR_SHEBA, FACING_WEST);
    FieldEvent_Wait(10);
  } else {
    ((s32 (*)())Actor_ShowEmoteById)(26, 0x102, 30);
    FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
    Actor_FaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
    Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 0);
    Actor_FaceDirection(ACTOR_SHEBA, FACING_WEST, 0);
    FieldEvent_Wait(30);
    Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
    Actor_SetAnimationAndWait(ACTOR_KRADEN, 4);
    FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
    ((s32 (*)())Actor_ShowEmoteById)(26, 0x109, 30);
    FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
    Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
    Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTH, 0);
    FieldEvent_Wait(40);
    Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 0);
    Actor_FaceDirection(ACTOR_SHEBA, FACING_WEST, 0);
    FieldEvent_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_KRADEN, 4);
    FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
  }
  FieldEvent_SetMessageId((s32)MsgSoIThink);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, 3);
  FieldEvent_Wait(10);
  // "So I think maybe we should just forget about that and start on our..."
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
  Actor_SetAnimation(ACTOR_PIERS, ANIM_NOD);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimation(ACTOR_SHEBA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
  FieldEvent_Wait(10);
  if (Event_GetFlag(FLAG_IN_PARTY(ACTOR_PIERS)) != 0) {
    Actor_SetSpeed(ACTOR_PIERS, 1.2);
    Actor_SetAnimation(ACTOR_PIERS, ANIM_WALK);
    leader = Actor_GetById(gMainRoutineWork.leaderActorId);
    if (leader != 0) {
      Actor_SetDestination(ACTOR_PIERS, leader->x.part[0].pixel,
                           leader->z.part[0].pixel);
    }
    FieldActor_WaitForMoveById(ACTOR_PIERS);
    Actor_SetPosition(ACTOR_PIERS, 0, 0);
  }
  Actor_SetSpeed(ACTOR_JENNA, 1.2);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_WALK);
  leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  if (leader != 0) {
    Actor_SetDestination(ACTOR_JENNA, leader->x.part[0].pixel,
                         leader->z.part[0].pixel);
  }
  FieldActor_WaitForMoveById(ACTOR_JENNA);
  Actor_SetPosition(ACTOR_JENNA, 0, 0);
  Actor_SetSpeed(ACTOR_SHEBA, 1.2);
  Actor_SetAnimation(ACTOR_SHEBA, ANIM_WALK);
  leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  if (leader != 0) {
    Actor_SetDestination(ACTOR_SHEBA, leader->x.part[0].pixel,
                         leader->z.part[0].pixel);
  }
  FieldActor_WaitForMoveById(ACTOR_SHEBA);
  Actor_SetPosition(ACTOR_SHEBA, 0, 0);
  Actor_SetSpeed(ACTOR_KRADEN, 1.2);
  Actor_SetAnimation(ACTOR_KRADEN, 2);
  leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  if (leader != 0) {
    Actor_SetDestination(ACTOR_KRADEN, leader->x.part[0].pixel,
                         leader->z.part[0].pixel);
  }
  FieldActor_WaitForMoveById(ACTOR_KRADEN);
  Actor_SetPosition(ACTOR_KRADEN, 0, 0);
  Event_End();
  return;
}

extern u32 gEasternAlhafra_PuffScript[];

void Actor_StartScript(struct SpawnObject *object, const void *script);
void Actor_SetSpritePriorityDirect(struct Actor *object, s32 priority);

/// Spawns a puff (actor type 0x1e8) at x, y, z, turned to value, for timer
/// frames.
void EasternAlhafra_SpawnPuff(s32 x, s32 y, s32 z, s16 value, s32 timer) {
  struct SpawnObject *object;
  struct FieldSprite *sprite;

  object = ((struct SpawnObject *)FieldActor_Create(0x1e8, x, y, z));
  if (object != 0) {
    sprite = object->sprite;
    Actor_SelectAnimation(object, 1);
    Actor_StartScript(object, gEasternAlhafra_PuffScript);
    Actor_SetSpritePriorityDirect(object, 1);
    object->motionFlags = 0;
    {
      /* FAKEMATCH(signature): the timer copied into an s64 local. */
      s64 wide = timer;

      object->unk5e = wide;
    }
    sprite->nextFrame = 0;
    sprite->rotation = value;
  }
}
#undef EVENT_WORK
#undef OBJECT
#undef FIELD_AT

/* VIEW(s1bf4): routine EasternAlhafra_SpawnPuff as s32
 * EasternAlhafra_SpawnPuff(s32 x, s32 y, s32 z, s32 rotation, s32 timer); the
 * definition has void EasternAlhafra_SpawnPuff(s32 x, s32 y, s32 z, s16 value,
 * s32 timer); tried as the definition's void EasternAlhafra_SpawnPuff(s32 x,
 * s32 y, s32 z, s16 value, s32 timer), which the object already declares,
 * EasternAlhafra_PuffFrom moves: 2 of 79 instructions in tla-ja. */
extern s32
EasternAlhafra_SpawnPuff__puffs(s32 x, s32 y, s32 z, s32 rotation,
                                s32 timer) __asm__("EasternAlhafra_SpawnPuff");

void EasternAlhafra_PuffFrom(s32 id) {
  struct Actor *object = Actor_Get(id);

  EasternAlhafra_SpawnPuff__puffs(object->x.fixed + px(8),
                                  object->y.fixed + px(28), object->z.fixed,
                                  0x1000, 15);
  Event_PlaySound(SE_LIGHT_APPEAR);
  FieldEvent_Wait(15);
  EasternAlhafra_SpawnPuff__puffs(object->x.fixed - px(8),
                                  object->y.fixed + px(28), object->z.fixed,
                                  0xf000, 15);
  Event_PlaySound(SE_LIGHT_APPEAR);
  FieldEvent_Wait(15);
  EasternAlhafra_SpawnPuff__puffs(object->x.fixed + px(8),
                                  object->y.fixed + px(28), object->z.fixed,
                                  0x1000, 15);
  Event_PlaySound(SE_LIGHT_APPEAR);
  FieldEvent_Wait(15);
  EasternAlhafra_SpawnPuff__puffs(object->x.fixed - px(8),
                                  object->y.fixed + px(28), object->z.fixed,
                                  0xf000, 15);
  Event_PlaySound(SE_LIGHT_APPEAR);
  FieldEvent_Wait(15);
}

static __inline__ void FieldMessage_ShowAtActor_(s32 actor, s32 flags) {
  s32 f = flags;

  FieldMessage_ShowAtActor(actor, f);
}

/// The scene that opens with MsgHeyThatBlock: either answer to the prompt
/// steps the event to its next message.
void EasternAlhafra_WorkersSeeBlockGone(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
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
  {
    s32 mode = 1;

    FieldCamera_MoveTo(328, CAMERA_KEEP, 632, mode);
  }
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(20);
  FieldActor_StartRepeatedMotion(ACTOR_FIRST_WORKER, 3);
  Actor_SetAttachedEffect(ACTOR_FIRST_WORKER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(50);
  FieldActor_ShowEmote(ACTOR_SECOND_WORKER, EMOTE_IN_FRONT | EMOTE_WINCE, 0x1e);
  // "Hey! Now, we can raise the mast and use the sail again!"
  FieldEvent_ShowMessageThenWait(ACTOR_FIRST_WORKER, 0, 10);
  Actor_FaceDirection(ACTOR_FIRST_WORKER, FACING_WEST, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_FIRST_WORKER, 3);
  FieldEvent_Wait(10);
  // "Hey, mate, let's go tell the mayor the good news!"
  FieldEvent_ShowMessageThenWait(ACTOR_FIRST_WORKER, 0, 10);
  Actor_FaceDirection(ACTOR_SECOND_WORKER, FACING_EAST, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_SECOND_WORKER, 3);
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
  FieldMessage_ShowAtActor_(0x17, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(4, 0) == 0) {
    FieldEvent_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_CHAUCHA, 3);
    FieldEvent_Wait(10);
    FieldEvent_ShowMessageThenWait(ACTOR_CHAUCHA, 0, 10);
    gFieldWork->message += 1;
  } else {
    FieldEvent_Wait(40);
    Actor_SetAnimationAndWait(ACTOR_CHAUCHA, 4);
    FieldEvent_Wait(10);
    gFieldWork->message += 1;
    FieldEvent_ShowMessageThenWait(ACTOR_CHAUCHA, 0, 10);
  }
  Actor_FaceDirection(ACTOR_CHAUCHA, FACING_EAST, 0);
  FieldEvent_Wait(20);
  Actor_FaceDirection(ACTOR_EOLEO, FACING_WEST, 0);
  FieldEvent_Wait(30);
  FieldEvent_ShowMessageThenWait(ACTOR_CHAUCHA, 0, 10);
  FieldActor_ShowEmote(ACTOR_EOLEO, EMOTE_IN_FRONT | EMOTE_SMILE, 0x2d);
  FieldActor_Jump(ACTOR_EOLEO, 6, 15);
  FieldActor_Jump(ACTOR_EOLEO, 6, 23);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_CHAUCHA, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(ACTOR_CHAUCHA, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_EOLEO, 3);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_EOLEO, 3);
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

/// Once the mast is clear, the workers can't believe the stone block has
/// vanished and take the party to the mayor (SceneId_AlhafraManor, entrance
/// 81).
void EasternAlhafra_WorkersTellMayor(void) {
  if (Event_GetFlag(FLAG_ALHAFRA_MAST_CLEARED) == 0)
    return;
  Event_SetFlag(FLAG_WORKERS_TOLD_MAYOR);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
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
  FieldActor_ShowEmote(ACTOR_FIRST_WORKER, EMOTE_IN_FRONT | EMOTE_SCOWL, 0x28);
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
  Actor_SetAnimation(ACTOR_ALHAFRAS_MAYOR, 1);
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
  FieldActor_ShowEmote(ACTOR_ALHAFRAS_MAYOR, EMOTE_IN_FRONT | EMOTE_WINCE,
                       0x1e);
  Actor_FaceDirection(ACTOR_ALHAFRAS_MAYOR, FACING_NORTH, 0);
  FieldEvent_Wait(20);
  // "It's gone! That huge block is really gone!"
  FieldEvent_ShowMessageThenWait(ACTOR_ALHAFRAS_MAYOR, 0, 10);
  Actor_FaceDirection(ACTOR_FIRST_WORKER, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_SECOND_WORKER, FACING_SOUTH_SOUTHWEST, 0);
  FieldEvent_Wait(30);
  Actor_SetAnimation(ACTOR_FIRST_WORKER, 3);
  Actor_SetAnimationAndWait(ACTOR_SECOND_WORKER, 3);
  FieldEvent_Wait(20);
  Actor_FaceDirection(ACTOR_JIYA, FACING_WEST, 0);
  FieldEvent_Wait(30);
  FieldActor_ShowEmote(ACTOR_JIYA, EMOTE_IN_FRONT | EMOTE_SMILE, 0x1e);
  // "Excellent news, Mayor!"
  FieldEvent_ShowMessageThenWait(ACTOR_JIYA, 0, 10);
  Actor_FaceDirection(ACTOR_ALHAFRAS_MAYOR, FACING_EAST, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_ALHAFRAS_MAYOR, 3);
  FieldEvent_Wait(10);
  // "Unfortunately, the mayor of Madra will probably try to take it from..."
  FieldEvent_ShowMessageThenWait(ACTOR_ALHAFRAS_MAYOR, 0, 10);
  Actor_SetAnimation(ACTOR_FIRST_WORKER, 3);
  Actor_SetAnimationAndWait(ACTOR_SECOND_WORKER, 3);
  FieldEvent_Wait(20);
  Actor_FaceDirection(ACTOR_ALHAFRAS_MAYOR, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_JIYA, FACING_NORTH_NORTHWEST, 0);
  FieldEvent_Wait(40);
  FieldActor_ShowEmote(ACTOR_ALHAFRAS_MAYOR, EMOTE_IN_FRONT | EMOTE_QUESTION,
                       0x1e);
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
                       0x28);
  Actor_FaceDirection(ACTOR_ALHAFRAS_MAYOR, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_JIYA, FACING_NORTH_NORTHWEST, 0);
  FieldEvent_Wait(20);
  FieldCamera_MoveTo(144, CAMERA_KEEP, 544, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(20);
  // "Felix!"
  FieldEvent_ShowMessageThenWait(ACTOR_ALHAFRAS_MAYOR, 0, 10);
  FieldActor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_WINCE, 0x1e);
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
                       0x1e);
  // "Felix, did you do this?"
  FieldEvent_ShowMessageThenWait(ACTOR_ALHAFRAS_MAYOR, 0, 10);
  Actor_WalkToAndWait(ACTOR_FELIX, 136, 520);
  FieldActor_LaunchFromLeader(ACTOR_KRADEN, 16, 0, FACING_SOUTH_SOUTHWEST);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTH, 0);
  FieldActor_WaitForMoveById(ACTOR_KRADEN);
  FieldEvent_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, 3);
  FieldEvent_Wait(10);
  // "Surprised to see us, Mayor?"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_ALHAFRAS_MAYOR, 3);
  FieldEvent_Wait(10);
  // "Did you do this?"
  FieldEvent_ShowMessageThenWait(ACTOR_ALHAFRAS_MAYOR, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, 4);
  // "We left Alhafra some time ago, but we just returned and destroyed..."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0, 10);
  FieldActor_ShowEmote(ACTOR_ALHAFRAS_MAYOR, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP,
                       0x1e);
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
                       0x1e);
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
  Actor_SetAnimationAndWait(ACTOR_ALHAFRAS_MAYOR, 3);
  FieldEvent_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_SECOND_WORKER, 2);
  FieldEvent_Wait(10);
  // "We shouldn't need too many people to raise the mast!"
  FieldEvent_ShowMessageThenWait(ACTOR_SECOND_WORKER, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_ALHAFRAS_MAYOR, 3);
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
  Actor_SetAnimationAndWait(ACTOR_ALHAFRAS_MAYOR, 3);
  FieldEvent_Wait(10);
  FieldMessage_ShowAtActor_(0x19, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(4, 0) != 0) {
    FieldEvent_Wait(40);
    Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
    FieldEvent_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_KRADEN, 4);
    FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0,
                                   10);
    gFieldWork->message += 1;
  } else {
    FieldEvent_Wait(30);
    Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
    FieldEvent_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_KRADEN, 3);
    FieldEvent_Wait(10);
    gFieldWork->message += 1;
    FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0,
                                   10);
  }
  Actor_FaceDirection(ACTOR_FELIX, FACING_EAST, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
  FieldEvent_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_ALHAFRAS_MAYOR, 3);
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
    struct FieldWork *event = Ram_HeapSlots->fieldWork;

    event->transitionFrames = 60;
    event->transition = 0x100;
  }
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_SetDestination(SCENE_ALHAFRA_MANOR, 81);
  Event_End();
}

/// Once the mast is clear and Briggs has escaped, he sails off in the mayor's
/// ship, taunting the party, and the scene starts over at entrance 99 for the
/// mayor (EasternAlhafra_MayorFindsShipGone).
void EasternAlhafra_BriggsStealsShip(void) {
  struct MapWork *map;
  s32 i;
  s32 step = 0x10000;

  if (Event_GetFlag(FLAG_ALHAFRA_MAST_CLEARED) == 0)
    return;
  Event_SetFlag(FLAG_ALHAFRA_SHIP_GONE);
  Event_SetFlag(FLAG_BRIGGS_SAILED_FROM_ALHAFRA);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgBriggs2);
  // "Briggs!!!"
  FieldEvent_ShowMessageThenWait(ACTOR_SECOND_WORKER, 0, 10);
  Actor_WalkToAndWait(ACTOR_FELIX, 136, 552);
  FieldActor_LaunchFromLeader(ACTOR_KRADEN, -16, 0, FACING_NORTHWEST);
  Actor_FaceDirectionById(ACTOR_FELIX, FACING_NORTH, 0);
  FieldActor_WaitForMoveById(ACTOR_KRADEN);
  FieldEvent_Wait(20);
  FieldActor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0x1e);
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
  Ram_HeapSlots->mapGrid->minX = 0;
  Ram_HeapSlots->mapGrid->maxX = 0x4000000;
  Ram_HeapSlots->mapGrid->minZ = 0;
  Ram_HeapSlots->mapGrid->maxZ = 0x4000000;
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
    struct Actor *center = gFieldWork->camera;

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
  Event_PlaySound(MUS_035);
  PaletteFade_ApplyTarget(0x10000, 0);
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
  Actor_SetAnimationAndWait(ACTOR_CHAUCHA, 3);
  FieldEvent_Wait(10);
  // "You're the thieves! You tried to take our boat away, after we paid..."
  FieldEvent_ShowMessageThenWait(ACTOR_CHAUCHA, 0, 10);
  // "... ..."
  FieldEvent_ShowMessageThenWait(ACTOR_ALHAFRAS_MAYOR, 0, 10);
  Actor_FaceDirectionById(ACTOR_BRIGGS, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_CHAUCHA, FACING_WEST, 0);
  FieldEvent_Wait(25);
  Actor_SetAnimationAndWait(ACTOR_BRIGGS, 3);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_CHAUCHA, 3);
  FieldEvent_Wait(20);
  Actor_FaceDirection(ACTOR_BRIGGS, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_CHAUCHA, FACING_SOUTH_SOUTHWEST, 0);
  FieldEvent_Wait(20);
  // "You're the one who stole OUR boat!"
  FieldEvent_ShowMessageThenWait(ACTOR_BRIGGS, 0, 10);
  // "Ahoy, Briggs!"
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
  FieldActor_ShowEmote(ACTOR_CHAUCHA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  FieldActor_ShowEmote(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0x32);
  FieldActor_StartRepeatedMotion(ACTOR_BRIGGS, 3);
  Actor_SetAttachedEffect(ACTOR_BRIGGS,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(40);
  Actor_WalkByAndWait(ACTOR_BRIGGS, 0, 16);
  FieldEvent_Wait(20);
  FieldActor_ShowEmote(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_WINCE, 0x1e);
  // "Uh-oh... It's Felix!"
  FieldEvent_ShowMessageThenWait(ACTOR_BRIGGS, 0, 10);
  Actor_WalkByAndWait(ACTOR_CHAUCHA, 0, 16);
  Actor_FaceDirection(ACTOR_CHAUCHA, FACING_WEST, 0);
  FieldEvent_Wait(20);
  FieldActor_ShowEmote(ACTOR_CHAUCHA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 0x1e);
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
  FieldActor_ShowEmote(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_QUESTION, 0x2d);
  Actor_WalkByAndWait(ACTOR_BRIGGS, 0, 32);
  FieldEvent_Wait(30);
  FieldActor_ShowEmote(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 0x2d);
  Actor_WalkByAndWait(ACTOR_BRIGGS, 0, -32);
  Actor_FaceDirectionById(ACTOR_BRIGGS, FACING_EAST, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_BRIGGS, 3);
  FieldEvent_Wait(10);
  // "You're right... They're stuck on the land, and we've got this boat!"
  FieldEvent_ShowMessageThenWait(ACTOR_BRIGGS, 0, 10);
  Actor_FaceDirection(ACTOR_CHAUCHA, FACING_SOUTH_SOUTHWEST, 0);
  FieldEvent_Wait(50);
  Actor_FaceDirectionById(ACTOR_CHAUCHA, FACING_WEST, 0);
  FieldEvent_Wait(20);
  FieldActor_ShowEmote(ACTOR_CHAUCHA, EMOTE_IN_FRONT | EMOTE_SMILE, 0x1e);
  // "They can't catch us... What would they do, swim?"
  FieldEvent_ShowMessageThenWait(ACTOR_CHAUCHA, 0, 10);
  FieldActor_ShowEmote(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_QUESTION, 0x32);
  Actor_WalkByAndWait(ACTOR_BRIGGS, 0, 32);
  FieldEvent_Wait(30);
  FieldActor_ShowEmote(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 0x32);
  Actor_WalkByAndWait(ACTOR_BRIGGS, 0, -32);
  Actor_FaceDirectionById(ACTOR_BRIGGS, FACING_EAST, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_BRIGGS, 3);
  FieldEvent_Wait(10);
  // "Hah! You're right! They can't board us from way over there!"
  FieldEvent_ShowMessageThenWait(ACTOR_BRIGGS, 0, 10);
  FieldActor_ShowEmote(ACTOR_CHAUCHA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 0x1e);
  // "Wasn't there something you wanted to do the next time you saw Felix?"
  FieldEvent_ShowMessageThenWait(ACTOR_CHAUCHA, 0, 10);
  Actor_FaceDirection(ACTOR_BRIGGS, FACING_SOUTH_SOUTHWEST, 0);
  FieldEvent_Wait(30);
  EasternAlhafra_PuffFrom(ACTOR_BRIGGS);
  Event_PlaySound(SE_CURSE);
  FieldActor_ShowEmote(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0x32);
  Actor_FaceDirectionById(ACTOR_BRIGGS, FACING_EAST, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_CHAUCHA, 4);
  // "Don't tell me you forgot... It's all you ever talked about when you..."
  FieldEvent_ShowMessageThenWait(ACTOR_CHAUCHA, 0, 10);
  Actor_FaceDirection(ACTOR_BRIGGS, FACING_SOUTH_SOUTHWEST, 0);
  FieldEvent_Wait(30);
  EasternAlhafra_PuffFrom(ACTOR_BRIGGS);
  FieldActor_ShowEmote(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 0x32);
  Actor_FaceDirectionById(ACTOR_BRIGGS, FACING_EAST, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_BRIGGS, 3);
  FieldEvent_Wait(10);
  // "Oh yeah... I almost forgot."
  FieldEvent_ShowMessageThenWait(ACTOR_BRIGGS, 0, 10);
  FieldActor_ShowEmote(ACTOR_CHAUCHA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 0x1e);
  // "So now's your chance! Do what you swore to do!"
  FieldEvent_ShowMessageThenWait(ACTOR_CHAUCHA, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_BRIGGS, 2);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_BRIGGS, 3);
  FieldEvent_Wait(20);
  Actor_FaceDirectionAndWait(ACTOR_BRIGGS, FACING_SOUTH_SOUTHWEST);
  Actor_SetAnimation(ACTOR_BRIGGS, 7);
  FieldEvent_Wait(20);
  Actor_SetAnimation(ACTOR_BRIGGS, 6);
  FieldEvent_Wait(5);
  // "Bleah!"
  FieldEvent_ShowMessageThenWait(ACTOR_BRIGGS, 0, 10);
  FieldActor_Jump(ACTOR_CHAUCHA, 6, 15);
  FieldActor_Jump(ACTOR_CHAUCHA, 6, 23);
  // "Is that the best you could come up with?"
  FieldEvent_ShowMessageThenWait(ACTOR_CHAUCHA, 0, 10);
  FieldActor_ShowEmote(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_WINCE, 0x32);
  Actor_SetAnimation(ACTOR_BRIGGS, 7);
  FieldEvent_Wait(20);
  Actor_SetAnimation(ACTOR_BRIGGS, 1);
  Actor_FaceDirectionById(ACTOR_BRIGGS, FACING_EAST, 0);
  FieldEvent_Wait(15);
  // "But, Chaucha, they're an awfully tough bunch..."
  FieldEvent_ShowMessageThenWait(ACTOR_BRIGGS, 0, 10);
  FieldActor_ShowEmote(ACTOR_CHAUCHA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0x1e);
  // "See? It's that attitude right there! That's why you'll never amount..."
  FieldEvent_ShowMessageThenWait(ACTOR_CHAUCHA, 0, 10);
  FieldActor_ShowEmote(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 0x28);
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
  FieldActor_ShowEmote(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_SMILE, 0x1e);
  // "I doubt we'll ever meet again, but if we do, remember this!"
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_BRIGGS, 0, 20);
  Actor_SetAnimation(ACTOR_BRIGGS, 8);
  FieldEvent_Wait(10);
  // "Neener neener neener! Neener neener neener!"
  FieldEvent_ShowMessageThenWait(ACTOR_BRIGGS, 0, 10);
  FieldActor_Jump(ACTOR_BRIGGS, 7, 0);
  i = 15;
turn:
  ((s16 *)Actor_GetById(ACTOR_BRIGGS))[3] -= 0x1000;
  i--;
  System_Wait(1);
  if (i >= 0)
    goto turn;
  Actor_SetAnimation(ACTOR_BRIGGS, 5);
  FieldEvent_Wait(7);
  FieldEvent_ShowMessageThenWait(ACTOR_BRIGGS, 0, 20);
  FieldActor_ShowEmote(ACTOR_CHAUCHA, EMOTE_IN_FRONT | EMOTE_SMILE, 0x32);
  Actor_WalkByAndWait(ACTOR_CHAUCHA, 0, 36);
  FieldEvent_Wait(15);
  Actor_SetAnimation(ACTOR_BRIGGS, 1);
  FieldEvent_Wait(20);
  FieldActor_Jump(ACTOR_CHAUCHA, 6, 15);
  FieldActor_Jump(ACTOR_CHAUCHA, 6, 23);
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_CHAUCHA, 0, 15);
  Actor_SetAnimationAndWait(ACTOR_BRIGGS, 3);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_BRIGGS, 3);
  FieldEvent_Wait(10);
  Actor_SetAnimation(ACTOR_BRIGGS, 9);
  FieldEvent_Wait(70);
  map = Ram_HeapSlots->mapGrid;
  Actor_GetById(ACTOR_BRIGGS)->motionFlags = 0;
  Actor_GetById(ACTOR_CHAUCHA)->motionFlags = 0;
  MapGrid_CopyCellsLayered(42, 60, 17, 4, 42, 64);
  MapGrid_CopyCellsLayered(42, 60, 17, 4, 42, 68);
  MapGrid_CopyCellsLayered(106, 60, 17, 4, 106, 64);
  MapGrid_CopyCellsLayered(106, 60, 17, 4, 106, 68);
  i = 239;
scroll:
  map->layers[0].offsetY += step;
  map->layers[1].offsetY += 0x10000;
  Actor_GetById(ACTOR_BRIGGS)->z.fixed -= step;
  i--;
  Actor_GetById(ACTOR_CHAUCHA)->z.fixed -= step;
  System_Wait(1);
  if (i >= 0)
    goto scroll;
  Event_PlaySound(SOUND_FADE_OUT_BGM);
  FieldEvent_Wait(30);
  Event_End();
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  Event_SetDestination(SCENE_EASTERN_ALHAFRA, 99);
}

/// The scene that opens with MsgWhatYouLet; either answer to each of its two
/// prompts steps the event's message number, by 1 and then by 2.
void EasternAlhafra_MayorFindsShipGone(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgWhatYouLet);
  FieldCamera_MoveTo(152, CAMERA_KEEP, 272, 0);
  {
    struct Actor *center = gFieldWork->camera;

    FieldActor_SetPositionAndResetMotion(center, center->x.fixed,
                                         center->y.fixed, center->z.fixed);
  }
  FieldEvent_Wait(30);
  PaletteFade_ApplyTarget(0x10000, 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_Wait(15);
  // "What!!! You let Briggs get away!"
  FieldEvent_ShowMessageThenWait(ACTOR_ALHAFRAS_MAYOR, 0, 10);
  FieldCamera_MoveTo(152, CAMERA_KEEP, 392, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(20);
  FieldActor_ShowEmote(ACTOR_ALHAFRAS_MAYOR, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN,
                       0x1e);
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
                       0x1e);
  // "My prize ship, stolen by a bunch of filthy pirate scum!"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_ALHAFRAS_MAYOR,
                                 0, 10);
  Actor_FaceDirection(ACTOR_ALHAFRAS_MAYOR, FACING_NORTH_NORTHEAST, 0);
  FieldEvent_Wait(30);
  FieldActor_ShowEmote(ACTOR_ALHAFRAS_MAYOR, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                       0x1e);
  Actor_SetSpeed(ACTOR_ALHAFRAS_MAYOR, 0.7);
  Actor_GetById(ACTOR_ALHAFRAS_MAYOR)->steeringFlags &= 0xfe;
  Actor_WalkByAndWait(ACTOR_ALHAFRAS_MAYOR, 0, 8);
  FieldEvent_Wait(1);
  Actor_GetById(ACTOR_ALHAFRAS_MAYOR)->steeringFlags |= 1;
  FieldEvent_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_ALHAFRAS_MAYOR, 2);
  FieldEvent_Wait(10);
  Actor_SetSpeed(ACTOR_ALHAFRAS_MAYOR, 2.4);
  Actor_GetById(ACTOR_ALHAFRAS_MAYOR)->steeringFlags &= 0xfe;
  Actor_WalkByAndWait(ACTOR_ALHAFRAS_MAYOR, 0, -16);
  FieldEvent_Wait(1);
  Actor_GetById(ACTOR_ALHAFRAS_MAYOR)->steeringFlags |= 1;
  Actor_GetById(ACTOR_ALHAFRAS_MAYOR)->steeringFlags &= 0xfe;
  Actor_WalkBy(ACTOR_ALHAFRAS_MAYOR, 0, 8);
  Event_PlaySound(SE_IMPACT);
  Actor_SetSpeed(ACTOR_SECOND_WORKER, 2.4);
  Actor_GetById(ACTOR_SECOND_WORKER)->steeringFlags &= 0xfe;
  Actor_WalkByAndWait(ACTOR_SECOND_WORKER, 0, -4);
  FieldEvent_Wait(1);
  Actor_GetById(ACTOR_SECOND_WORKER)->steeringFlags |= 1;
  Actor_GetById(ACTOR_SECOND_WORKER)->facing = FACING_WEST;
  Actor_GetById(ACTOR_SECOND_WORKER)->steeringFlags &= 0xfe;
  Actor_WalkByAndWait(ACTOR_SECOND_WORKER, 0, -4);
  FieldEvent_Wait(1);
  Actor_GetById(ACTOR_SECOND_WORKER)->steeringFlags |= 1;
  Actor_GetById(ACTOR_SECOND_WORKER)->facing = FACING_NORTH_NORTHWEST;
  Actor_GetById(ACTOR_SECOND_WORKER)->steeringFlags &= 0xfe;
  Actor_WalkByAndWait(ACTOR_SECOND_WORKER, 0, -4);
  FieldEvent_Wait(1);
  Actor_GetById(ACTOR_SECOND_WORKER)->steeringFlags |= 1;
  Actor_GetById(ACTOR_SECOND_WORKER)->facing = FACING_NORTH_NORTHEAST;
  Actor_GetById(ACTOR_SECOND_WORKER)->steeringFlags &= 0xfe;
  Actor_WalkByAndWait(ACTOR_SECOND_WORKER, 0, -4);
  FieldEvent_Wait(1);
  Actor_GetById(ACTOR_SECOND_WORKER)->steeringFlags |= 1;
  Actor_GetById(ACTOR_SECOND_WORKER)->facing = 0;
  Actor_GetById(ACTOR_SECOND_WORKER)->steeringFlags &= 0xfe;
  Actor_WalkByAndWait(ACTOR_SECOND_WORKER, 0, -4);
  FieldEvent_Wait(1);
  Actor_GetById(ACTOR_SECOND_WORKER)->steeringFlags |= 1;
  Actor_GetById(ACTOR_SECOND_WORKER)->facing = FACING_SOUTH_SOUTHEAST;
  FieldEvent_Wait(20);
  Actor_GetById(ACTOR_ALHAFRAS_MAYOR)->steeringFlags |= 1;
  // "Get up!"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_ALHAFRAS_MAYOR,
                                 0, 10);
  Actor_SetPosition(ACTOR_SECOND_WORKER, 168, 368);
  FieldActor_SetSpriteRotationById(ACTOR_SECOND_WORKER, 0);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_SECOND_WORKER), 1);
  Actor_FaceDirection(ACTOR_SECOND_WORKER, FACING_SOUTH_SOUTHWEST, 0);
  FieldActor_Jump(ACTOR_SECOND_WORKER, 6, 40);
  FieldActor_ShowEmote(ACTOR_SECOND_WORKER, EMOTE_IN_FRONT | EMOTE_WINCE, 0x28);
  Actor_FaceDirectionById(ACTOR_ALHAFRAS_MAYOR, FACING_SOUTH_SOUTHEAST, 0);
  FieldEvent_Wait(30);
  Actor_SetSpeed(ACTOR_ALHAFRAS_MAYOR, 0.7);
  Actor_GetById(ACTOR_ALHAFRAS_MAYOR)->steeringFlags &= 0xfe;
  Actor_WalkByAndWait(ACTOR_ALHAFRAS_MAYOR, 0, -8);
  FieldEvent_Wait(1);
  Actor_GetById(ACTOR_ALHAFRAS_MAYOR)->steeringFlags |= 1;
  FieldEvent_Wait(20);
  Actor_SetSpeed(ACTOR_ALHAFRAS_MAYOR, 2.4);
  Actor_GetById(ACTOR_ALHAFRAS_MAYOR)->steeringFlags &= 0xfe;
  Actor_WalkByAndWait(ACTOR_ALHAFRAS_MAYOR, 0, 16);
  FieldEvent_Wait(1);
  Actor_GetById(ACTOR_ALHAFRAS_MAYOR)->steeringFlags |= 1;
  Event_PlaySound(SE_IMPACT);
  Actor_SetSpeed(ACTOR_SECOND_WORKER, 2.4);
  Actor_GetById(ACTOR_JIYA)->steeringFlags &= 0xfe;
  Actor_WalkByAndWait(ACTOR_JIYA, 0, 3);
  FieldEvent_Wait(1);
  Actor_GetById(ACTOR_JIYA)->steeringFlags |= 1;
  Actor_GetById(ACTOR_JIYA)->facing = 0;
  Actor_GetById(ACTOR_JIYA)->steeringFlags &= 0xfe;
  Actor_WalkByAndWait(ACTOR_JIYA, 0, 3);
  FieldEvent_Wait(1);
  Actor_GetById(ACTOR_JIYA)->steeringFlags |= 1;
  Actor_GetById(ACTOR_JIYA)->facing = FACING_NORTH_NORTHEAST;
  Actor_GetById(ACTOR_JIYA)->steeringFlags &= 0xfe;
  Actor_WalkByAndWait(ACTOR_JIYA, 0, 3);
  FieldEvent_Wait(1);
  Actor_GetById(ACTOR_JIYA)->steeringFlags |= 1;
  Actor_GetById(ACTOR_JIYA)->facing = FACING_NORTH_NORTHWEST;
  Actor_GetById(ACTOR_JIYA)->steeringFlags &= 0xfe;
  Actor_WalkByAndWait(ACTOR_JIYA, 0, 3);
  FieldEvent_Wait(1);
  Actor_GetById(ACTOR_JIYA)->steeringFlags |= 1;
  Actor_GetById(ACTOR_JIYA)->facing = FACING_WEST;
  Actor_GetById(ACTOR_JIYA)->steeringFlags &= 0xfe;
  Actor_WalkByAndWait(ACTOR_JIYA, 0, 3);
  FieldEvent_Wait(1);
  Actor_GetById(ACTOR_JIYA)->steeringFlags |= 1;
  Actor_GetById(ACTOR_JIYA)->facing = FACING_SOUTH_SOUTHWEST;
  FieldEvent_Wait(20);
  // "They're gone, and you let them escape!"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_ALHAFRAS_MAYOR,
                                 0, 10);
  Actor_SetPosition(ACTOR_JIYA, 168, 424);
  FieldActor_SetSpriteRotationById(ACTOR_JIYA, 0);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_JIYA), 1);
  Actor_GetById(ACTOR_JIYA)->facing = FACING_NORTH_NORTHWEST;
  FieldActor_Jump(ACTOR_JIYA, 6, 40);
  FieldActor_ShowEmote(ACTOR_JIYA, EMOTE_IN_FRONT | EMOTE_WINCE, 0x28);
  Actor_FaceDirectionById(ACTOR_ALHAFRAS_MAYOR, FACING_NORTH_NORTHEAST, 0);
  FieldEvent_Wait(30);
  FieldActor_ShowEmote(ACTOR_ALHAFRAS_MAYOR, EMOTE_IN_FRONT | EMOTE_SCOWL,
                       0x1e);
  // "You guys have royally botched this one. This ruins everything!"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_ALHAFRAS_MAYOR,
                                 0, 10);
  FieldActor_ShowEmote(ACTOR_SECOND_WORKER, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP,
                       0x32);
  Actor_FaceDirectionById(ACTOR_ALHAFRAS_MAYOR, FACING_SOUTH_SOUTHEAST, 0);
  FieldEvent_Wait(30);
  FieldActor_ShowEmote(ACTOR_ALHAFRAS_MAYOR, EMOTE_IN_FRONT | EMOTE_SCOWL,
                       0x1e);
  // "You all ought to be ashamed! All of you were cowering, while I was..."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_ALHAFRAS_MAYOR,
                                 0, 10);
  FieldActor_ShowEmote(ACTOR_JIYA, EMOTE_IN_FRONT | EMOTE_WINCE, 0x32);
  Actor_SetAnimation(ACTOR_ALHAFRAS_MAYOR, 5);
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
  Actor_SetAnimation(ACTOR_ALHAFRAS_MAYOR, 1);
  FieldEvent_Wait(30);
  Actor_FaceDirectionById(ACTOR_SECOND_WORKER, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirectionById(ACTOR_JIYA, FACING_NORTH_NORTHWEST, 0);
  // "I'm going back to my manor!"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_ALHAFRAS_MAYOR,
                                 0, 10);
  Actor_SetAnimation(ACTOR_JIYA, 3);
  Actor_SetAnimationAndWait(ACTOR_SECOND_WORKER, 3);
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
  Actor_SetAnimation(ACTOR_ALHAFRAS_MAYOR, 1);
  Actor_SetAnimation(ACTOR_JIYA, 1);
  Actor_FaceDirectionById(ACTOR_SECOND_WORKER, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirectionById(ACTOR_JIYA, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirectionById(ACTOR_ALHAFRAS_MAYOR, FACING_SOUTH_SOUTHWEST, 0);
  FieldEvent_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_ALHAFRAS_MAYOR, 4);
  // "And you, Felix. I am extremely disappointed in you."
  FieldEvent_ShowMessageThenWait(
      SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_ABOVE | 25, 0, 10);
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
                       0x28);
  // "There will be no reward for you, Felix!"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALHAFRAS_MAYOR,
                                 0, 10);
  Actor_FaceDirectionById(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirectionById(ACTOR_FELIX, FACING_SOUTHEAST, 0);
  FieldEvent_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_ALHAFRAS_MAYOR, 3);
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
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_FIRST_WORKER), 1);
  FieldActor_Jump(ACTOR_FIRST_WORKER, 6, 20);
  Actor_FaceDirectionById(ACTOR_FIRST_WORKER, FACING_WEST, 0);
  Actor_SetPosition(ACTOR_MAN_KNOCKED_DOWN, 128, 608);
  FieldActor_SetSpriteRotationById(ACTOR_MAN_KNOCKED_DOWN, 0);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_MAN_KNOCKED_DOWN), 1);
  FieldActor_Jump(ACTOR_MAN_KNOCKED_DOWN, 6, 20);
  Actor_FaceDirectionById(ACTOR_MAN_KNOCKED_DOWN, FACING_NORTH_NORTHEAST, 0);
  Actor_SetPosition(ACTOR_YOUNG_MAN_KNOCKED_DOWN, 192, 616);
  FieldActor_SetSpriteRotationById(ACTOR_YOUNG_MAN_KNOCKED_DOWN, 0);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_YOUNG_MAN_KNOCKED_DOWN), 1);
  FieldActor_Jump(ACTOR_YOUNG_MAN_KNOCKED_DOWN, 6, 20);
  Actor_FaceDirectionById(ACTOR_YOUNG_MAN_KNOCKED_DOWN, FACING_NORTH_NORTHWEST,
                          0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_ALHAFRAS_MAYOR, 4);
  // "We don't have a boat, so there's no reason for anyone to be here at..."
  FieldEvent_ShowMessageThenWait(ACTOR_ALHAFRAS_MAYOR, 0, 10);
  Actor_SetAnimation(ACTOR_YOUNG_MAN_KNOCKED_DOWN, 3);
  Actor_SetAnimation(ACTOR_MAN_KNOCKED_DOWN, 3);
  Actor_SetAnimationAndWait(ACTOR_FIRST_WORKER, 3);
  FieldEvent_Wait(30);
  FieldActor_WatchAndSetScript(ACTOR_FELIX, ACTOR_ALHAFRAS_MAYOR);
  FieldActor_WatchAndSetScript(ACTOR_KRADEN, ACTOR_ALHAFRAS_MAYOR);
  FieldActor_WatchAndSetScript(ACTOR_YOUNG_MAN_KNOCKED_DOWN,
                               ACTOR_ALHAFRAS_MAYOR);
  FieldActor_WatchAndSetScript(ACTOR_MAN_KNOCKED_DOWN, ACTOR_ALHAFRAS_MAYOR);
  FieldActor_WatchAndSetScript(ACTOR_FIRST_WORKER, ACTOR_ALHAFRAS_MAYOR);
  Actor_SetScript(ACTOR_ALHAFRAS_MAYOR, (s32)gEasternAlhafra_RunOffScript);
  Actor_SetScript(ACTOR_JIYA, (s32)gEasternAlhafra_RunOffScript);
  FieldEvent_Wait(10);
  Actor_SetScript(ACTOR_SECOND_WORKER, (s32)gEasternAlhafra_RunOffScript);
  FieldEvent_Wait(70);
  Actor_WalkByAndWait(ACTOR_FIRST_WORKER, -16, 0);
  Actor_SetScript(ACTOR_FIRST_WORKER, (s32)gEasternAlhafra_RunOffScript);
  FieldEvent_Wait(33);
  Actor_WalkByAndWait(ACTOR_MAN_KNOCKED_DOWN, 16, 0);
  Actor_SetScript(ACTOR_MAN_KNOCKED_DOWN, (s32)gEasternAlhafra_RunOffScript);
  FieldEvent_Wait(15);
  Actor_SetScript(ACTOR_YOUNG_MAN_KNOCKED_DOWN,
                  (s32)gEasternAlhafra_RunOffScript);
  FieldActor_WaitForScriptEndById(ACTOR_ALHAFRAS_MAYOR);
  Actor_FaceDirectionById(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
  FieldActor_WaitForScriptEndById(ACTOR_YOUNG_MAN_KNOCKED_DOWN);
  FieldEvent_Wait(20);
  EventCamera_FollowActor(ACTOR_FELIX, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(40);
  FieldActor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0x32);
  Actor_FaceDirectionById(ACTOR_KRADEN, FACING_EAST, 0);
  Actor_FaceDirectionById(ACTOR_FELIX, FACING_WEST, 0);
  FieldEvent_Wait(20);
  FieldMessage_ShowAtActor_(0x1a, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(4, 0) == 0) {
    FieldEvent_Wait(20);
    FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
    gFieldWork->message += 1;
  } else {
    FieldEvent_Wait(40);
    gFieldWork->message += 1;
    FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
  }
  Actor_SetSpeed(ACTOR_KRADEN, 0.8);
  Actor_WalkByAndWait(ACTOR_KRADEN, 0, -16);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
  Actor_FaceDirectionById(ACTOR_KRADEN, FACING_NORTH_NORTHEAST, 0);
  FieldEvent_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
  Actor_WalkByAndWait(ACTOR_KRADEN, 0, 16);
  Actor_FaceDirection(ACTOR_FELIX, FACING_WEST, 0);
  Actor_FaceDirectionById(ACTOR_KRADEN, FACING_EAST, 0);
  FieldEvent_Wait(20);
  FieldMessage_ShowAtActor_(0x1a, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(4, 0) != 0) {
    FieldEvent_Wait(40);
    FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
    Actor_SetAnimationAndWait(ACTOR_KRADEN, 3);
    FieldEvent_Wait(10);
    FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
    gFieldWork->message += 2;
  } else {
    FieldEvent_Wait(20);
    gFieldWork->message += 2;
    FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
  }
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
  FieldEvent_Wait(20);
  Actor_SetSpeed(ACTOR_KRADEN, 1.2);
  Actor_SetAnimation(ACTOR_KRADEN, 2);
  {
    struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);

    if (leader != NULL)
      Actor_SetDestination(ACTOR_KRADEN, ((s16 *)leader)[5],
                           ((s16 *)leader)[9]);
  }
  FieldActor_WaitForMoveById(ACTOR_KRADEN);
  Actor_SetPosition(ACTOR_KRADEN, 0, 0);
  FieldEvent_Wait(10);
  Event_End();
}

void BattleEffect_StepVector3(struct Particle *particle, s32 unk1, s32 step);

#include "shared/particles_update_falling.h"

/// The falling rocks' work: their tiles, 64 rocks and their sprites, in a
/// ring that next walks round.
struct RockWork {
  u8 tiles[0x500];
  struct Rock rocks[64];
  struct RockSprite sprites[64];
  s16 next;
  u16 tileBase;
  s16 resource;
};

/// Spawns count rocks around (x, y, z), each pushed out at a random angle and
/// thrown up and out along it.
#include "shared/rocks_spawn.h"

static __inline__ void StartTask(void (*func)(void), s32 priority) {
  /* FAKEMATCH(wrapper): Task_Add forwarded inline. */
  Task_Add(func, priority);
}

void EasternAlhafra_StartFallingRocks(void) {
  struct RockWork *work =
      HeapSlot_Alloc(HEAP_SLOT_MENU, sizeof(struct RockWork));
  s32 entry;
  s32 tile;
  s32 i;
  struct Rock *rock;
  struct RockSprite *sprite;

  Decompress_TaggedLz(Resource_GetTableEntry((s32)&ResourceId_FallingRock),
                      work);
  entry = VramCache_FindFreeEntry();
  tile = Vram_Load(entry, 0x500, work);
  work->tileBase = tile;
  work->resource = entry;
  rock = work->rocks;
  sprite = work->sprites;
  for (i = 63; i >= 0; i--, sprite++, rock++) {
    EasternAlhafra_InitializeRockSprite(sprite, 8, 8, 0x40000000, tile);
    sprite->unk05_5 = 1;
    sprite->palette = 0;
    sprite->priority = 0;
    sprite->unk1e = 240;
    rock->timer = -1;
  }
  {
    s32 zero = 0;

    work->next = zero;
  }
  StartTask(Particles_UpdateFalling, 0x480);
}

void EasternAlhafra_EndFallingRocks(void) {
  struct EasternAlhafraSparkWork *work = Ram_HeapSlots->menuWork;

  FieldTask_Remove(Particles_UpdateFalling);
  VramCache_FreeEntry(work->resource);
  HeapSlot_Release(HEAP_SLOT_MENU);
}

// Rewrites the map cells under a pushable actor. The actor's sprite
// shape picks one of the six footprints in gPushFootprints (four signed bytes
// each, in sixteenths of a cell); the cells under it are copied from the
// map's spare area (Map_CopyCellAttributeRect) and byte 2 of both collision
// layers there is filled with 255. Returns 1, or 0 for a shape with no
// footprint.

extern s8 gPushFootprints[][4];

static __inline__ s32 Abs(s32 value) { return value < 0 ? -value : value; }

/// Rewrites the map cells under a pushable actor. The actor's sprite shape
/// picks one of the six footprints in gPushFootprints (four signed bytes each,
/// in sixteenths of a cell); the cells under it are copied from the map's spare
/// area (Map_CopyCellAttributeRect) and byte 2 of both collision layers there
/// is filled with 255. Returns 1, or 0 for a shape with no footprint.
s32 PushTarget_FillFootprint(s32 id) {
  u8 *map = (u8 *)Ram_HeapSlots->mapGrid;
  struct Actor *actor = Actor_GetById(id);
  s16 kind = *((struct ObjectAnimationState *)actor->sprite)->value28;
  s16 i = 0;
  s32 next;
  u32 index;
  s32 a, b;
  s32 fromX, fromZ;
  s32 width, height, x, z;

  if ((u16)kind != gPushKinds[(u16)i]) {
  again:
    next = (i + 1) * 65536;
    index = (u32)next >> 16;
    i = next >> 16;
    if (index <= 5 && (u16)kind != gPushKinds[index])
      goto again;
  }
  // fromX and b are reused for the next values they hold.
  fromX = (u16)i;
  if (fromX == 6)
    return 0;
  a = gPushFootprints[fromX][0];
  width = (Abs(a) + Abs(gPushFootprints[fromX][2])) >> 4;
  b = gPushFootprints[fromX][1];
  height = (Abs(b) + Abs(gPushFootprints[fromX][3])) >> 4;
  x = actor->x.fixed + px(a);
  z = actor->z.fixed + px(b);
  b = *(s32 *)(map + 0x148) >> 20;
  x >>= 20;
  z >>= 20;
  fromZ = *(s32 *)(map + 0x14c) >> 20;
  fromX = (u16)b;
  Map_CopyCellAttributeRect(x, z, width, height, x + fromX, z + (u16)fromZ);
  Map_FillCellRectByte2(0, x, z, width, height, 255);
  Map_FillCellRectByte2(2, x, z, width, height, 255);
  return 1;
}

#include "../field/map_cell_fill.c"

extern s32 gPushLeaderSteps[];

static __inline__ s32 PushTarget_CheckCollision(struct Actor *actor,
                                                struct Vector3 *cell) {
  return MapGrid_CheckCollision(actor, cell);
}

/// Measures a push: finds the pushable actor ahead of the leader
/// (PushTarget_FindAhead, recording its number and kind in the push state),
/// then steps the actor's box one cell at a time along the leader's direction
/// until a cell of it is blocked (MapGrid_CheckCollision returns 2); with at
/// least one free step it records the end position in the push state and
/// returns 1, else 0. The actor stays on layer 2 while it is measured.
s32 PushTarget_FindSlide(struct PushState *push) {
  struct Actor *actor;
  struct Vector3 cell;
  s32 direction, kind, width, height, dx, dz, steps, i, j, offset;
  u8 *layer;

  actor = PushTarget_FindAhead(&direction, &push->number, &push->kind);
  if (actor != 0) {
    kind = push->kind;
    width =
        (Abs(gPushFootprints[kind][0]) + Abs(gPushFootprints[kind][2])) >> 4;
    height =
        (Abs(gPushFootprints[kind][1]) + Abs(gPushFootprints[kind][3])) >> 4;
    offset = gPushLeaderSteps[direction];
    dx = offset & FIELD_PIXEL_MASK;
    dz = px(offset);
    push->unk14 = 0;
    steps = 0;
    layer = &actor->layer;
    *layer = 2;
    push->x = actor->x.fixed + dx;
    push->z = actor->z.fixed + dz;
    cell.y = actor->y.fixed;
    for (;;) {
      cell.z = push->z + px(gPushFootprints[kind][1]);
      for (j = 0; j < height; j++) {
        cell.x = push->x + px(gPushFootprints[kind][0]);
        for (i = 0; i < width; i++) {
          /* FAKEMATCH(wrapper): the call through PushTarget_CheckCollision. */
          if (PushTarget_CheckCollision(actor, &cell) == 2)
            goto blocked;
          cell.x += px(16);
        }
        cell.z += px(16);
      }
      push->x += dx;
      steps++;
      push->z += dz;
    }
  blocked:
    *layer = 0;
    if (steps == 0)
      return 0;
    {
      s32 move = dx * steps;

      push->x = actor->x.fixed + move;
    }
    push->y = actor->y.fixed;
    push->z = actor->z.fixed + dz * steps;
    return 1;
  }
  return 0;
}

/// The whole-pixel half of a 16.16 coordinate.
#define PIXEL_PART(v) (((s16 *)&(v))[1])

#include "shared/push_target_find_ahead.h"

struct PushTarget {
  s32 x; ///< 16.16 fixed-point pixels
  s32 y;
  s32 z;
  s32 footprint; ///< index into the footprint table
  s32 id;
  void (*callback)(void);
};

#define ABS(x) ((x) < 0 ? -(x) : (x))

void PushTarget_Push(struct PushTarget target) {
  u8 *map = (u8 *)((struct HeapSlotWords *)Ram_HeapSlots)->mapWork;
  struct Actor *leader = Actor_Get(gMainRoutineWork.leaderActorId);
  struct Actor *object = Actor_Get(target.id);
  s32 cell[3];
  s32 from[3];
  s32 footprint;
  s32 width;
  s32 height;
  s32 originX;
  s32 originZ;
  s32 direction;
  s32 speed;
  s32 acceleration;

  direction = leader->facing >> 16;
  footprint = target.footprint;
  speed = leader->speedLimit;
  acceleration = leader->acceleration;
  from[0] = object->x.fixed;
  from[2] = object->z.fixed;
  cell[0] = FIELD_CELL(object->x.fixed + px(gPushFootprints[footprint][0]));
  cell[2] = FIELD_CELL(object->z.fixed + px(gPushFootprints[footprint][1]));
  width = (ABS(gPushFootprints[footprint][0]) +
           ABS(gPushFootprints[footprint][2])) >>
          4;
  height = (ABS(gPushFootprints[footprint][1]) +
            ABS(gPushFootprints[footprint][3])) >>
           4;
  Map_FillCellRectByte2(0, cell[0], cell[2], width, height, 0);
  Actor_SetSpeedAndAcceleration(gMainRoutineWork.leaderActorId, 0.5, 0.1);
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, 8);
  System_Wait(15);
  {
    s32 deltaX = (target.x - from[0]) >> 17;
    s32 deltaZ = (target.z - from[2]) >> 17;

    Actor_SetDestinationOffset(gMainRoutineWork.leaderActorId, deltaX, deltaZ);
  }
  Actor_Get(gMainRoutineWork.leaderActorId)->update = Actor_StopAtPushTarget;
  System_Wait(4);
  if ((u32)(direction - 6) <= 7)
    Actor_SelectAnimation(object, 3);
  else
    Actor_SelectAnimation(object, 2);
  Event_PlaySound(SE_HEAVY_MOVE);
  Actor_SetSpeedAndAcceleration(target.id, 0.5, 0.1);
  FieldActor_SetMoveTargetFixed(object, target.x, target.y, target.z);
  FieldActor_WaitForMoveById(gMainRoutineWork.leaderActorId);
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_WALK);
  Actor_SetSpeedAndAcceleration(gMainRoutineWork.leaderActorId, 0.3, 0.1);
  Actor_SetDestinationOffset(gMainRoutineWork.leaderActorId,
                             px(gPushLeaderSteps[direction]) >> 31,
                             px(gPushLeaderSteps[direction]) >> 17);
  if (target.callback != 0)
    target.callback();
  FieldActor_WaitForMoveById(gMainRoutineWork.leaderActorId);
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_STAND);
  leader->update = 0;
  leader->speedLimit = speed;
  leader->acceleration = acceleration;
  FieldActor_WaitForMove(object);
  Event_PlaySound(SE_STOP_MUSIC_PLAYER_5);
  Event_PlaySound(SE_HEAVY_STOP);
  FieldActor_SetPositionAndResetMotion(object, target.x, object->y.fixed,
                                       target.z);
  Actor_SelectAnimation(object, 1);
  target.x = target.x + px(gPushFootprints[footprint][0]);
  target.z = target.z + px(gPushFootprints[footprint][1]);
  target.x >>= 20;
  target.z >>= 20;
  originX = *(s32 *)(map + 328) >> 20;
  originZ = *(s32 *)(map + 332) >> 20;
  MapGrid_CopyCellAttributes(target.x, target.z, width, height,
                             originX + target.x, originZ + target.z);
  Map_FillCellRectByte2(0, target.x, target.z, width, height, 255);
  Map_FillCellRectByte2(2, target.x, target.z, width, height, 255);
  cell[0] = FIELD_CELL(from[0] + px(gPushFootprints[footprint][0]));
  cell[2] = FIELD_CELL(from[2] + px(gPushFootprints[footprint][1]));
  originX += cell[0];
  originZ += cell[2];
  MapGrid_CopyCellAttributes(originX, originZ, width, height, cell[0], cell[2]);
  Map_FillCellRectByte2(2, cell[0], cell[2], width, height, 0);
  Event_NoOp();
}

#include "shared/actor_stop_at_push_target.h"

void Pillars_Place(const u16 *list) {
  const u16 *spec;
  u16 id;
  struct Actor *object;
  s16 mode;
  s16 flag;
  s32 limit;
  s32 first;
  limit = 0xff;
  first = list[0];
  if (first == ((limit << 8) + 0xff)) {
    return;
  }
  spec = list;
next:
  id = spec[0];
  object = Actor_Get(id);
  mode = spec[1];
  flag = spec[2];
  object->collisionFlags &= 0xfe;
  Actor_SetSpriteFlags(object, 0);
  Actor_SetCellOpen(id, (u16)mode, (u16)flag, object->layer);
  limit = 0xff;
  spec += 3;
  first = spec[0];
  if (first != ((limit << 8) + 0xff)) {
    goto next;
  }
}

// Finds the current target's table row and shrinks it.
#include "shared/pound_sink_target.h"

#include "shared/actor_set_cell_open.h"

#undef ABS
#undef EVENT_WORK
#undef OBJECT
#undef FIELD_AT
#undef PIXEL_PART
#include "text/msg_ids.h"

void EasternAlhafra_ReachRock(void);

// clang-format off
u16 gPushKinds[] = {
    258, 259, 293, 294, 332, 331,
};

s8 gPushFootprints[][4] = {
    {-32, -8, 32, 8},  {-8, -32, 8, 32}, {-32, -16, 32, 0},
    {-8, -32, 8, 32},  {-32, -8, 32, 8},  {-8, -32, 8, 32},
};

s32 gPushLeaderSteps[] = {
    0x100000, 0x100000, 0x100000, 0x10, 0x10, 0x10,
    0x10, 0xfff00000, 0xfff00000, 0xfff00000, 0xfff00000, 0xfff0,
    0xfff0, 0xfff0, 0xfff0, 0x100000, 0xffff000a,
};

s16 gEasternAlhafra_PoundCone[] = {
    14, 1, FLAG_CONE_POUNDED, -1,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 488, 0, 424, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {10, CONDITION_ALWAYS, 520, 0, 520, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {11, CONDITION_ALWAYS, 264, 0, 424, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {12, CONDITION_ALWAYS, 440, 0, 344, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {99, CONDITION_ALWAYS, 136, 0, 568, FACING_NORTH, 0, 32, 0, 608, 704, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(EasternAlhafra),
    SCENE_EXIT_UNLESS(1, Alhafra, 11, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, EasternAlhafraShip, 1, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

struct ActorPlacement gEasternAlhafra_Placements[] = {
    ACTOR_PLACEMENT(407, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 280, 0, 232, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(407, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 376, 0, 232, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(287, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 552, 0, 520, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(287, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 408, 0, 520, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 344, 0, 312, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 440, 0, 312, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 312, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(258, CONDITION_ALWAYS, 7, 336, 0, 456, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(467, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 344, 0, 472, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(467, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 440, 0, 456, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, 7, 444, 0, 312, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, 7, 482, 0, 312, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, 7, 444, 0, 376, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, 7, 492, 0, 312, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(39, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 440, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(39, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 440, 0, 424, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(39, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 344, 0, 488, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(39, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 440, 0, 376, FACING_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(5, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(6, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(7, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(56, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_NORTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gEasternAlhafra_PlacementsAfterBriggsCaught[] = {
    ACTOR_PLACEMENT(407, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 280, 0, 232, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(407, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 376, 0, 232, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(287, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 552, 0, 520, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(287, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 408, 0, 520, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 344, 0, 312, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 440, 0, 312, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 312, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(258, CONDITION_ALWAYS, 7, 336, 0, 456, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(467, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 344, 0, 472, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(467, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 440, 0, 456, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, 7, 444, 0, 312, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, 7, 482, 0, 312, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, 7, 444, 0, 376, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, 7, 492, 0, 312, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(16, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION, -56, 0, -56, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(37, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION, -56, 0, -56, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(38, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION, -56, 0, -56, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(24, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION, -56, 0, -56, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(5, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(6, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(7, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(56, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(112, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION, -56, 0, -56, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(195, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION, -56, 0, -56, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(187, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION, -56, 0, -56, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(115, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION, -56, 0, -56, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(114, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION, -56, 0, -56, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

s16 gEasternAlhafra_CabinDoorFrames[] = {
    0, 0, 1, 2, 6, -1,
};

s32 gEasternAlhafra_CabinDoor[] = {
    (s32)gEasternAlhafra_CabinDoorFrames,
    0x110010,
};

struct Trigger gSceneEvents[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS, EVENT(EasternAlhafra_EnterCabin)},
    {TRIGGER_TALK, 22, CONDITION_ALWAYS, EVENT(EasternAlhafra_TalkToSailor)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22 | TRIGGER_SUBJECT_OWN_EFFECT, FLAG_TALKED_TO_SAILOR, EVENT(EasternAlhafra_TalkToSailor)},
    {TRIGGER_TALK, 23, CONDITION_ALWAYS, MSGNO(MsgWePaidBig)},
    {TRIGGER_TALK, 24, CONDITION_ALWAYS, MSGNO(MsgWeSentMy)},
    {TRIGGER_TALK, 25, CONDITION_ALWAYS, MSGNO(MsgIfWeCould)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS, MSGNO(MsgPeopleCallUs)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 23, CONDITION_ALWAYS, MSGNO(MsgIThoughtIt)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 24, CONDITION_ALWAYS, MSGNO(MsgThatLousyRat)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 25, CONDITION_ALWAYS, MSGNO(MsgWhenIThink)},
    {TRIGGER_TALK, 30, CONDITION_ALWAYS, MSGNO(MsgOooogOw2)},
    {TRIGGER_TALK, 31, CONDITION_ALWAYS, MSGNO(MsgWhyDoThese)},
    {TRIGGER_TALK, 28, CONDITION_ALWAYS, MSGNO(MsgStupidBriggs2)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 30, CONDITION_ALWAYS, MSGNO(MsgBriggsSPirates)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 31, CONDITION_ALWAYS, MSGNO(MsgIJustCame)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 28, CONDITION_ALWAYS, MSGNO(MsgBriggsSaidTheBoat)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 10, FLAG_ALHAFRA_MAST_CLEARED, EVENT(EasternAlhafra_BurstMastBlock)},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), 8, FLAG_ROPE_LASHED, 0},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 14, FLAG_CONE_POUNDED, EVENT(EasternAlhafra_PoundCone)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 10, FLAG_FIRST_CRATE_SUNK, EVENT(EasternAlhafra_StartPushingBlock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 10, FLAG_FIRST_CRATE_SUNK, EVENT(EasternAlhafra_FinishPushingBlock)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 11, FLAG_SECOND_CRATE_SUNK, EVENT(EasternAlhafra_StartPushingBlock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 11, FLAG_SECOND_CRATE_SUNK, EVENT(EasternAlhafra_FinishPushingBlock)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 16, FLAG_LAST_ROCK_SUNK, EVENT(EasternAlhafra_StartPushingLastRock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 16, FLAG_LAST_ROCK_SUNK, EVENT(EasternAlhafra_FinishPushingLastRock)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 17, FLAG_FIRST_ROCK_SUNK, EVENT(EasternAlhafra_StartPushingBlock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 17, FLAG_FIRST_ROCK_SUNK, EVENT(EasternAlhafra_FinishPushingBlock)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(EasternAlhafra_StartPushingBlock)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(EasternAlhafra_FinishPushingBlock)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 12, CONDITION_ALWAYS, EVENT(EasternAlhafra_PushAhead)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 12, CONDITION_ALWAYS, EVENT(EasternAlhafra_PushAhead)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 13, CONDITION_ALWAYS, EVENT(EasternAlhafra_PushAhead)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 11, CONDITION_ALWAYS, EVENT(EasternAlhafra_RollLog)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 11, CONDITION_ALWAYS, EVENT(EasternAlhafra_RollLog)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTHWEST), 12, CONDITION_ALWAYS, EVENT(EasternAlhafra_IgnoreDiagonalStep)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH_NORTHWEST), 12, CONDITION_ALWAYS, EVENT(EasternAlhafra_IgnoreDiagonalStep)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTHEAST), 12, CONDITION_ALWAYS, EVENT(EasternAlhafra_IgnoreDiagonalStep)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH_SOUTHEAST), 12, CONDITION_ALWAYS, EVENT(EasternAlhafra_IgnoreDiagonalStep)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTHWEST), 11, CONDITION_ALWAYS, EVENT(EasternAlhafra_IgnoreDiagonalStep)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH_NORTHWEST), 11, CONDITION_ALWAYS, EVENT(EasternAlhafra_IgnoreDiagonalStep)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTHEAST), 11, CONDITION_ALWAYS, EVENT(EasternAlhafra_IgnoreDiagonalStep)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH_SOUTHEAST), 11, CONDITION_ALWAYS, EVENT(EasternAlhafra_IgnoreDiagonalStep)},
    {TRIGGER_STEP, 20, CONDITION_ALWAYS, EVENT(EasternAlhafra_StepOntoMast)},
    {TRIGGER_STEP, 21, FLAG_WORKERS_TOLD_MAYOR, EVENT(EasternAlhafra_WorkersTellMayor)},
    {TRIGGER_STEP, 22, FLAG_ALHAFRA_SHIP_GONE, EVENT(EasternAlhafra_BriggsStealsShip)},
    {TRIGGER_STEP, 23, FLAG_ASKED_TO_HELP, EVENT(EasternAlhafra_AskToHelpWithMast)},
    {TRIGGER_STEP, 24, FLAG_GAVE_UP_ON_ROCK, EVENT(EasternAlhafra_GiveUpOnRock)},
    {TRIGGER_STEP, 25, FLAG_REACHED_ROCK, EVENT(EasternAlhafra_ReachRock)},
    {TRIGGER_TABLE_END},
};

u8 gEasternAlhafra_WaveTableIndex[] = {
    0,
};

s32 gEasternAlhafra_FirstWorkerCheerScript[] = {
    SC_ANIMATION(2),
    SC_MOVE_BY(0, 0, 8),
    SC_SYNC,
    SC_MOVE_BY(16, 0, 16),
    SC_SYNC,
    SC_MOVE_BY(16, 0, -16),
    SC_SYNC,
    SC_MOVE_BY(0, 0, -16),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(1),
    SC_END,
};

s32 gEasternAlhafra_SecondWorkerCheerScript[] = {
    SC_ANIMATION(2),
    SC_MOVE_BY(32, 0, 32),
    SC_SYNC,
    SC_MOVE_BY(16, 0, -16),
    SC_SYNC,
    SC_MOVE_BY(2, 0, -6),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(1),
    SC_END,
};

u32 gEasternAlhafra_FirstWorkerRunScript[] = {
    SC_ANIMATION(2),
    SC_MOVE_BY(48, 0, 0),
    SC_SYNC,
    SC_MOVE(168, 0, 584),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_SET(SW_FACING, FACING_NORTHEAST),
    SC_WAIT(1),
    SC_END,
};

u32 gEasternAlhafra_SecondWorkerRunScript[] = {
    SC_ANIMATION(2),
    SC_MOVE_BY(48, 0, 0),
    SC_SYNC,
    SC_MOVE(184, 0, 584),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_SET(SW_FACING, FACING_NORTHEAST),
    SC_WAIT(1),
    SC_END,
};

s32 gEasternAlhafra_RunOffScript[] = {
    SC_ANIMATION(2),
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

u32 gEasternAlhafra_PuffScript[] = {
    SC_RELEASE,
};
