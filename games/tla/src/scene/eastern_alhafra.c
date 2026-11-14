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
#include "lib.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "flags.h"
#include "scene_tables.h"
#include "system/main_routine_work.h"
#include "field/event_inline.h"
#include "module/scene/scene.h"
#include "module/tla.h"
#include "field/particle.h"
#include "system/ram_buffer.h"
#include "field/field_sequence_event.h"
#include "actor.h"
#include "system/io_reg.h"
#include "system/dma.h"
#include "map.h"
#include "system/frame.h"
#include "call.h"
#include "field_sprite.h"
#include "scene_ids.h"
#include "system/task.h"
#include "system/heap_release.h"
#include "item_icon.h"
#include "graphics/resource.h"
#include "system/vector3.h"
#include "module/field/field.h"
#include "field_event.h"

// Story flags of the mast puzzle and its events.
#define FLAG_FIRST_BLOCK_SUNK 0x8a0  // actor 10
#define FLAG_SECOND_BLOCK_SUNK 0x8a1 // actor 11
#define FLAG_CONE_POUNDED 0x8a2
#define FLAG_ROPE_LASHED 0x8a3
#define FLAG_LAST_CRATE_SUNK 0x8a4  // actor 16
#define FLAG_FIRST_CRATE_SUNK 0x8a5 // actor 17
#define FLAG_LOG_ROLLED 0x8a6
#define FLAG_TALKED_TO_SAILOR 0x8a8
#define FLAG_OVERHEARD_WORKERS 0x8a9
#define FLAG_ASKED_TO_HELP 0x8ac
#define FLAG_ASKED_IF_FINISHED 0x8ad
#define FLAG_REACHED_ROCK 0x8be
#define FLAG_GAVE_UP_ON_ROCK 0x8bf
#define FLAG_WORKERS_SAW_BLOCK_GONE 0x904
#define FLAG_WORKERS_TOLD_MAYOR 0x905

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
extern s16 gEasternAlhafra_PlacementsAfterBriggsCaught[];

s32 Actor_HasArrived();

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

void *Scene_GetPlacements(void) {
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
    Rocks_Spawn(0x1f20000, 0x100000, 0x19c0000, 2);
  if (mode == 3)
    Rocks_Spawn(0x1f20000, 0x100000, 0x19c0000, 30);
  if (id == 0x3e7)
    EasternAlhafra_EndFallingRocks();
}

void EasternAlhafra_PoundCone(s32 mode, s32 arg) {
  struct Actor *a;
  struct Actor *b;
  s32 i;

  Pound_SinkTarget(gEasternAlhafra_PoundCone, mode, arg);
  if (mode == 1) {
    a = Actor_Get(19);
    b = Actor_Get(21);
    a->speedLimit = px(4);
    a->acceleration = px(1) / 2;
    b->acceleration = px(1) / 2;
    b->speedLimit = px(4);
    FieldActor_SetPositionAndResetMotion(a, px(488), a->y, px(312));
    FieldActor_SetMoveTarget(a, 440, 0, 312);
    FieldActor_SetPositionAndResetMotion(b, px(488), b->y, px(312));
    FieldActor_SetMoveTarget(b, 440, 0, 376);
    Tightrope_Release(1);
    Tightrope_Release(2);
    i = 0;
    do {
      Tightrope_Place(1, 18, 19);
      Tightrope_Place(2, 20, 21);
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

  leader = Actor_Get(gMainRoutineWork.currentOwner);
  object = Actor_Get(id);
  object->motionFlags = 0;
  if (id == 10) {
    leaderX = leader->x >> 20;
    x = object->x >> 20;
    z = object->z >> 20;
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
  x = object->x >> 20;
  hit = 0;
  z = object->z >> 20;
  if (id == 10) {
    FieldMapGrid_SetCellBehavior(px(528), px(512), 2, 0);
    FieldMapGrid_SetCellBehavior(px(512), px(512), 2, 0);
  }
  if (x == 31 && z == 32) {
    Event_SetFlag(FLAG_FIRST_BLOCK_SUNK);
    hit = 1;
  }
  if (x == 25 && z == 33) {
    Event_SetFlag(FLAG_SECOND_BLOCK_SUNK);
    hit = 1;
  }
  if (x == 21 && z == 33) {
    Event_SetFlag(FLAG_LAST_CRATE_SUNK);
    hit = 1;
  }
  if (x == 27 && z == 33) {
    Event_SetFlag(FLAG_FIRST_CRATE_SUNK);
    hit = 1;
  }
  if (hit) {
    if (id != 10)
      Event_PlaySound(SE_310);
    object->gravity = 0x170a;
    object->motionFlags = 3;
    Actor_SetSpriteFlags(Actor_Get(id), 1);
    System_Wait(5);
    for (i = 0; i < 60 && object->y > 0; i++)
      System_Wait(1);
    Event_PlaySound(SE_298);
    System_Wait(1);
    Event_PlaySound(SE_LOG_FALL);
    object->collisionFlags = 0;
    object->priorityFlags |= 2;
    Actor_SelectAnimation(object, 4);
    i = 0;
  create:
    spark = (FieldActor_Create(276, object->x, object->y, object->z));
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
    FieldActor_Stop(gMainRoutineWork.currentOwner);
    System_Wait(30);
    for (i = 0; i <= 2; i++)
      Actor_Release(sparks[i]);
  }
}

void EasternAlhafra_StartPushingLastCrate(s32 unused, s32 id) {
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

void EasternAlhafra_FinishPushingLastCrate(s32 mode, s32 id) {
  struct Actor *object;
  s32 x;
  s32 z;
  s32 open;

  object = Actor_Get(id);
  x = object->x >> 20;
  z = object->z >> 20;
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
    EventCamera_FollowActor(gMainRoutineWork.currentOwner, 1);
}

void EasternAlhafra_OverhearWorkers(void);
/* VIEW(s0514): routine PushTarget_Push as void PushTarget_Push(struct
 * PushSlide); eastern_alhafra has void PushTarget_Push(struct DoorPlan);
 * tried as the definition's void PushTarget_Push(struct PushTarget target),
 * which the object already declares, EasternAlhafra_RollLog moves: 1 of 198
 * instructions in tla-ja. */
extern void PushTarget_Push__log(struct PushSlide) __asm__("PushTarget_Push");

void EasternAlhafra_RollLog(void) {
  struct Actor *actor15;
  struct Actor *actor16;
  struct PushSlide unk;
  s32 facing;
  s32 i;

  actor15 = Actor_GetById(15);
  actor16 = Actor_GetById(16);
  facing =
      (Actor_GetById(gMainRoutineWork.currentOwner)->facing + 0x2000) & 0xc000;
  if (actor16 != NULL && Event_GetFlag(FLAG_LAST_CRATE_SUNK) == 0)
    MapGrid_SetCellBehavior(actor16->x, actor16->z, 2, 255);
  if (PushTarget_FindSlide(&unk) != 0) {
    if (Event_GetFlag(FLAG_LAST_CRATE_SUNK) == 0 || facing == 0xc000) {
      PushTarget_Push__log(unk);
    } else {
      Event_SetFlag(FLAG_LOG_ROLLED);
      Event_Start();
      FieldEvent_ResolvePendingActions(0);
      FieldCamera_MoveTo(336, CAMERA_KEEP, 560, 1);
      PushTarget_Push__log(unk);
      MapGrid_SetCellBehavior(actor15->x - px(24), actor15->z, 0, 0);
      MapGrid_SetCellBehavior(actor15->x + px(8), actor15->z, 0, 0);
      MapGrid_SetCellBehavior(actor15->x - px(24), actor15->z, 2, 0);
      MapGrid_SetCellBehavior(actor15->x - px(8), actor15->z, 2, 0);
      MapGrid_SetCellBehavior(actor15->x + px(8), actor15->z, 2, 0);
      MapGrid_SetCellBehavior(actor15->x + px(24), actor15->z, 2, 0);
      Actor_SetAnimation(15, 3);
      Actor_SetDestinationOffset(15, 0, 16);
      FieldEvent_Wait(20);
      actor15->gravity = 0xccc;
      actor15->motionFlags = 3;
      i = 0;
      if (actor15->y > 0) {
        do {
          System_Wait(1);
          i++;
        } while (i <= 59 && actor15->y > 0);
      }
      EasternAlhafra_PlaceClearedDebris();
      Event_PlaySound(SE_LOG_FALL);
      Actor_SetAnimation(15, 8);
      FieldEvent_Wait(60);
      EventCamera_FollowActor(gMainRoutineWork.currentOwner, 1);
      Event_End();
    }
  }
  if (actor16 != NULL && Event_GetFlag(FLAG_LAST_CRATE_SUNK) == 0)
    MapGrid_SetCellBehavior(actor16->x, actor16->z, 2, 0);
}

void EasternAlhafra_PushAhead(s32 mode) {
  s32 tile;

  tile = Field_FindNearbyTalkTarget(gMainRoutineWork.currentOwner);
  if (tile == 16) {
    EasternAlhafra_StartPushingLastCrate(mode, 16);
    if (Leader_CanPushObjectAhead() != 0)
      EasternAlhafra_PushObjectAhead();
    EasternAlhafra_FinishPushingLastCrate(mode, 16);
  } else if (tile == 15) {
    EasternAlhafra_RollLog();
  }
}

void EasternAlhafra_TalkToSailor(void) {
  Event_SetFlag(FLAG_TALKED_TO_SAILOR);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId(MsgWhatSYour);
  Actor_ShowEmote(22, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Actor_FaceActor(22, gMainRoutineWork.currentOwner, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(22, 0, 10);
  Actor_RunRepeatedMotion(22, 2);
  FieldEvent_Wait(10);
  Event_OpenMessage(22, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(4, 0) != 0) {
    FieldEvent_Wait(30);
    FieldEvent_ShowMessageThenWait(22, 0, 0);
  } else {
    FieldEvent_Wait(20);
    gFieldWork->message += 1;
    Event_OpenMessage(22, 0);
    if (FieldEvent_AskYesNoWithActorAnimation(4, 0) != 0) {
      FieldEvent_Wait(30);
      FieldEvent_ShowMessageThenWait(22, 0, 0);
    } else {
      FieldEvent_Wait(20);
      gFieldWork->message += 1;
      Event_OpenMessage(22, 0);
      if (FieldEvent_AskYesNoWithActorAnimation(4, 0) != 0) {
        FieldEvent_Wait(30);
        FieldEvent_ShowMessageThenWait(22, 0, 0);
      } else {
        FieldEvent_Wait(20);
        gFieldWork->message += 1;
        FieldEvent_ShowMessageThenWait(22, 0, 0);
      }
    }
  }
  Actor_FaceDirection(22, FACING_EAST, 0);
  Event_End();
}

void EasternAlhafra_StepOntoMast(void) {
  if (Event_GetFlag(FLAG_ALHAFRA_MAST_CLEARED)) {
    if (Event_GetFlag(FLAG_WORKERS_SAW_BLOCK_GONE) == 0) {
      Event_SetFlag(FLAG_WORKERS_SAW_BLOCK_GONE);
      EasternAlhafra_WorkersSeeBlockGone();
    }
  } else if (Event_GetFlag(FLAG_BRIGGS_CAUGHT)) {
    if (Event_GetFlag(FLAG_FIRST_BLOCK_SUNK) &&
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
  Actor_Get(gMainRoutineWork.currentOwner)->motionFlags = 2;
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0.5);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 2);
  Actor_CenterAndWalk(gMainRoutineWork.currentOwner, 2, -8);
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
  Actor_WalkToAndWait(4, 488, 440);
  FieldActor_LaunchFromLeader(5, -16, 0, 0);
  Actor_FaceDirectionAndWait(4, FACING_WEST);
  FieldEvent_Wait(20);
  FieldEvent_SetMessageId(MsgIDonTKnowWhatWe);
  FieldEvent_ShowMessageThenWait(5, 0, 5);
  Actor_SetAnimation(5, 2);
  leader = Actor_Get(gMainRoutineWork.currentOwner);
  if (leader != NULL)
    Actor_SetDestination(5, ((s16 *)leader)[5], ((s16 *)leader)[9]);
  FieldActor_WaitForMoveById(5);
  Actor_SetPosition(5, 0, 0);
  Event_End();
  return;
check:
  if (Event_GetFlag(FLAG_BRIGGS_CAUGHT) &&
      Event_GetFlag(FLAG_ALHAFRA_MAST_CLEARED) == 0 &&
      Event_GetFlag(FLAG_REACHED_ROCK))
    goto run;
}

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }

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
  struct MapLayer *layer = &Ram_HeapSlots->mapWork->layers[2];
  u16 *line;
  s32 i = 0;

  line = gEasternAlhafra_WaveLines[gEasternAlhafra_WaveTableIndex[0] ^ 1];
  for (i = 0; i != 160; i++) {
    *line++ =
        (layer->y >> 16) +
        (Math_Sin(((u8)gFrameCount - i - (layer->y >> 16) + 160) << 9) >> 15);
  }
  gEasternAlhafra_WaveTableIndex[0] ^= 1;
}

void EasternAlhafra_StartWave(void) {
  Task_AddFrame(EasternAlhafra_FillWaveLines, 0x480);
  Task_AddFrame(EasternAlhafra_StartWaveDma, 0xc80);
}

#define EVENT_WORK ((u8 *)Ram_HeapSlots->fieldWork)
#define OBJECT(id) (Actor_Get((id)))

void Pillars_Place();

s32 Scene_Initialize(void) {
  struct Actor *object;
  s32 flag;

  ((struct FieldWork *)EVENT_WORK)->transition = 0x100;
  EasternAlhafra_StartWave();
  OBJECT(gMainRoutineWork.currentOwner)->priorityFlags |= 32;
  if (Event_GetFlag(FLAG_ALHAFRA_MAST_CLEARED) != 0) {
    if (Event_GetFlag(FLAG_WORKERS_TOLD_MAYOR) != 0 &&
        Event_GetFlag(FLAG_ALHAFRA_SHIP_GONE) == 0) {
      Actor_SetPosition(30, 184, 616);
      Actor_SetPosition(31, 136, 600);
      Actor_SetPosition(28, 168, 584);
      Actor_FaceDirection(28, FACING_SOUTH, 0);
      Actor_FallOverWithAnimationById(30, 0, 13);
      Call3(Actor_FallOverWithAnimationById, 31, 0x8000, 13);
      Actor_FallOverWithAnimationById(28, 0, 13);
      Actor_SetPosition(25, 112, 432);
      Actor_SetPosition(27, 176, 408);
      Actor_SetPosition(29, 152, 376);
      Actor_FaceDirection(29, FACING_SOUTH, 0);
      FieldActor_SetSpriteRotationById(25, 0x4000);
      Actor_SetSpriteFlags((s32)Actor_Get(25), 0);
      FieldActor_SetSpriteRotationById(27, 0xc000);
      Actor_SetSpriteFlags((s32)Actor_Get(27), 0);
      FieldActor_SetSpriteRotationById(29, 0x4000);
      Actor_SetSpriteFlags((s32)Actor_Get(29), 0);
      Actor_SetAnimation(25, 13);
      Actor_SetAnimation(27, 13);
      Actor_SetAnimation(29, 13);
    }
    if (Event_GetFlag(FLAG_WORKERS_TOLD_MAYOR) != 0) {
      MapGrid_CopyCellsLayered(40, 0, 24, 31, 11, 6);
      MapGrid_CopyCellsLayered(84, 15, 10, 10, 75, 15);
      MapGrid_CopyCellAttributes(10, 24, 1, 1, 11, 24);
      Actor_SetPosition(9, 0, 0);
      Actor_SetPosition(8, 0, 0);
      Actor_SetPosition(10, 0, 0);
      Actor_SetPosition(11, 0, 0);
      Actor_SetPosition(15, 0, 0);
    }
  }
  if (gMainRoutineWork.entrance == 99) {
    gMainRoutineWork.secondary = 1;
    gMainRoutineWork.entrance = 1;
    Actor_SetPosition(4, 136, 552);
    Actor_SetPosition(26, 120, 552);
    Actor_SetPosition(30, 184, 616);
    Actor_SetPosition(31, 136, 600);
    Actor_SetPosition(28, 160, 584);
    FieldActor_SetSpriteRotationById(30, 0x4000);
    Actor_SetSpriteFlags(Actor_Get(30), 0);
    FieldActor_SetSpriteRotationById(31, 0xc000);
    Actor_SetSpriteFlags(Actor_Get(31), 0);
    FieldActor_SetSpriteRotationById(28, 0x4000);
    Actor_SetSpriteFlags(Actor_Get(28), 0);
    FieldActor_SetSpritePriority(27, 2);
    FieldActor_SetSpritePriority(25, 2);
    FieldActor_SetSpritePriority(29, 2);
    Actor_SetPosition(25, 160, 392);
    Actor_SetPosition(27, 176, 408);
    Actor_SetPosition(29, 152, 376);
    FieldActor_SetSpriteRotationById(27, 0xc000);
    Actor_SetSpriteFlags(Actor_Get(27), 0);
    FieldActor_SetSpriteRotationById(29, 0x4000);
    Actor_SetSpriteFlags(Actor_Get(29), 0);
    Actor_FaceDirection(25, FACING_NORTH_NORTHEAST, 0);
    Actor_FaceDirection(29, FACING_SOUTH, 0);
    Actor_SetAnimation(27, 13);
    Actor_SetAnimation(29, 13);
    Actor_SetAnimation(30, 13);
    Actor_SetAnimation(31, 13);
    Actor_SetAnimation(28, 13);
    EasternAlhafra_MayorFindsShipGone();
  }
  EasternAlhafra_PlaceClearedDebris();
  if (Event_GetFlag(FLAG_LOG_ROLLED) == 0)
    PushTarget_FillFootprint(15);
  Rope_Initialize();
  Rope_Place(0, FLAG_ROPE_LASHED, 8, 9);
  OBJECT(8)->priorityFlags |= 32;
  OBJECT(9)->priorityFlags |= 32;
  Tightrope_Initialize();
  Tightrope_Place(0, 12, 13);
  Actor_SetSpriteFlags(Actor_Get(12), 0);
  Actor_SetSpriteFlags(Actor_Get(13), 0);
  Pillars_Place(gEasternAlhafra_PoundCone);
  Actor_SetSpriteFlags(Actor_Get(14), 0);
  flag = Event_GetFlag(FLAG_CONE_POUNDED);
  if (flag == 0) {
    object = OBJECT(18);
    /* FAKEMATCH(dead): the solid bit set in a once-only block. */
    do {
      object->collisionFlags |= 8;
    } while (0);
    object = OBJECT(19);
    object->motionFlags = flag;
    object->y = px(16);
    Tightrope_Place(1, 18, 19);
    Tightrope_Settle(1);
    object = OBJECT(20);
    object->collisionFlags |= 8;
    object = OBJECT(21);
    object->motionFlags = flag;
    object->y = px(16);
    Tightrope_Place(2, 20, 21);
  }
  Actor_SetSpriteFlags(Actor_Get(16), 0);
  Actor_SetSpriteFlags(Actor_Get(17), 0);
  FieldActor_SetSpritePriority(10, 2);
  ((u8 *)OBJECT(23))[0x62] = 1;
  if (Event_GetFlag(FLAG_BRIGGS_CAUGHT) != 0 &&
      Event_GetFlag(FLAG_ALHAFRA_MAST_CLEARED) == 0) {
    Actor_SetPosition(22, 0, 0);
    Actor_SetPosition(23, 0, 0);
    Actor_SetPosition(24, 0, 0);
    Actor_SetPosition(25, 0, 0);
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void EasternAlhafra_PlaceClearedDebris(void) {
  struct Actor *object;

  if (Event_GetFlag(FLAG_LOG_ROLLED)) {
    object = Actor_Get(15);
    Actor_SetPosition(15, 336, 536);
    Actor_SetAnimation(15, 4);
    object->priorityFlags |= 2;
    object->motionFlags = 2;
    object->velocityY = 0;
    object->y = -px(4);
    object->terrainHeight = -px(4);
    FieldMapGrid_SetCellBehavior(px(336), px(528), 2, 0);
    FieldMapGrid_SetCellBehavior(px(336), px(528), 0, 0);
    FieldMapGrid_SetCellBehavior(px(352), px(528), 0, 0);
  }
  if (Event_GetFlag(FLAG_LAST_CRATE_SUNK)) {
    Actor_SetPosition(16, 0, 0);
  }
  if (Event_GetFlag(FLAG_FIRST_CRATE_SUNK)) {
    Actor_SetPosition(17, 0, 0);
  }
  if (Event_GetFlag(FLAG_FIRST_BLOCK_SUNK)) {
    object = Actor_Get(10);
    Actor_SetPosition(10, 504, 520);
    Actor_SetAnimation(10, 2);
    object->collisionFlags = 0;
    /* FAKEMATCH(dead): an empty once-only block ends the scheduling block here.
     */
    do {
    } while (0);
    object->priorityFlags |= 2;
    FieldMapGrid_SetCellBehavior(object->x, object->z, 0, 0);
    MapGrid_SetAttributeBAt(object->x, object->z, 0, 128);
  }
  if (Event_GetFlag(FLAG_SECOND_BLOCK_SUNK)) {
    object = Actor_Get(11);
    Actor_SetPosition(11, 408, 536);
    Actor_SetAnimation(11, 2);
    object->collisionFlags = 0;
    /* FAKEMATCH(dead): an empty once-only block ends the scheduling block here.
     */
    do {
    } while (0);
    object->priorityFlags |= 2;
    FieldMapGrid_SetCellBehavior(object->x, object->z, 0, 0);
    MapGrid_SetAttributeBAt(object->x, object->z, 0, 128);
  }
  if (Event_GetFlag(FLAG_ALHAFRA_MAST_CLEARED)) {
    MapGrid_CopyCells(32, 24, 2, 2, 30, 24);
  }
}

void EasternAlhafra_OverhearWorkers(void) {
  struct Actor *object0;
  struct Actor *object1;
  struct Actor *object2;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgItSIt);
  FieldCamera_SetSpeed(4);
  FieldEvent_ShowMessageThenWait(22, 0, 10);
  Actor_WalkToAndWait(4, 232, 392);
  FieldActor_LaunchFromLeader(26, 0, -16, 0x2000);
  FieldActor_LaunchFromLeader(5, 0, 0, 0x2000);
  FieldActor_LaunchFromLeader(6, 0, 16, 0x2000);
  Actor_WalkByAndWait(4, 16, 0);
  Actor_FaceDirection(4, FACING_SOUTHEAST, 0);
  FieldEvent_Wait(30);
  EventCamera_MoveToActor(22, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(10);
  Actor_RunRepeatedMotion(22, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(22, 0, 10);
  EventCamera_MoveToActor(23, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(10);
  FieldActor_Jump(23, 6, 15);
  FieldActor_Jump(23, 6, 23);
  FieldEvent_ShowMessageThenWait(23, 0, 10);
  Actor_FaceDirection(22, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(24, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(25, FACING_SOUTH_SOUTHWEST, 0);
  FieldEvent_Wait(20);
  EventCamera_MoveToActor(11, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(23, 0, 10);
  EventCamera_MoveToActor(24, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(24, 4);
  FieldEvent_ShowMessageThenWait(24, 0, 10);
  Actor_FaceDirection(24, FACING_NORTHEAST, 0);
  FieldEvent_Wait(20);
  EventCamera_MoveToActor(25, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(10);
  Actor_FaceDirection(25, FACING_NORTHEAST, 0);
  FieldEvent_Wait(30);
  Actor_SetAnimationAndWait(25, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(25, 0, 10);
  Actor_FaceDirection(22, FACING_EAST, 0);
  Actor_FaceDirection(23, FACING_NORTH_NORTHEAST, 0);
  FieldEvent_Wait(20);
  EventCamera_MoveToActor(14, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(25, 0, 10);
  EventCamera_MoveToActor(22, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(10);
  Actor_ShowEmote(22, EMOTE_IN_FRONT | EMOTE_SCOWL, 40);
  FieldEvent_ShowMessageThenWait(22, 0, 10);
  FieldCamera_MoveTo(488, 0, 416, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(20);
  Actor_ShowEmote(23, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
  Actor_FaceDirection(23, FACING_EAST, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(23, 0, 10);
  Actor_FaceDirection(24, FACING_EAST, 0);
  Actor_FaceDirection(25, FACING_SOUTHEAST, 0);
  FieldEvent_Wait(20);
  FieldActor_StartRepeatedMotion(25, 3);
  Actor_SetAttachedEffect(25, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(40);
  FieldEvent_ShowMessageThenWait(25, 0, 10);
  FieldCamera_MoveTo(392, 16, 416, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(24, 4);
  FieldEvent_ShowMessageThenWait(24, 0, 10);
  Actor_FaceDirection(23, FACING_WEST, 0);
  FieldEvent_Wait(30);
  Actor_ShowEmote(22, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_ShowEmote(23, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_ShowEmote(25, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 50);
  FieldEvent_Wait(20);
  Actor_RunRepeatedMotion(25, 2);
  FieldEvent_Wait(10);
  Actor_FaceDirection(25, FACING_SOUTH_SOUTHWEST, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(25, 0, 20);
  Actor_SetAnimation(22, 3);
  Actor_SetAnimation(23, 3);
  Actor_SetAnimationAndWait(24, 3);
  FieldEvent_Wait(20);
  Actor_FaceDirection(22, FACING_EAST, 0);
  Actor_FaceDirection(23, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirection(24, FACING_NORTH, 0);
  Actor_FaceDirection(25, FACING_NORTHEAST, 0);
  FieldEvent_Wait(20);
  EventCamera_MoveToActor(gMainRoutineWork.currentOwner, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(20);
  Actor_SetSpeed(5, 1.2);
  Actor_SetAnimation(5, 2);
  object0 = Actor_GetById(gMainRoutineWork.currentOwner);
  if (object0 != 0) {
    Actor_SetDestination(5, *(s16 *)((u8 *)object0 + 10),
                         *(s16 *)((u8 *)object0 + 18));
  }
  FieldActor_WaitForMoveById(5);
  Actor_SetPosition(5, 0, 0);
  Actor_SetSpeed(6, 1.2);
  Actor_SetAnimation(6, 2);
  object1 = Actor_GetById(gMainRoutineWork.currentOwner);
  if (object1 != 0) {
    Actor_SetDestination(6, *(s16 *)((u8 *)object1 + 10),
                         *(s16 *)((u8 *)object1 + 18));
  }
  FieldActor_WaitForMoveById(6);
  Actor_SetPosition(6, 0, 0);
  Actor_SetSpeed(26, 1.2);
  Actor_SetAnimation(26, 2);
  object2 = Actor_GetById(gMainRoutineWork.currentOwner);
  if (object2 != 0) {
    Actor_SetDestination(26, *(s16 *)((u8 *)object2 + 10),
                         *(s16 *)((u8 *)object2 + 18));
  }
  FieldActor_WaitForMoveById(26);
  Actor_SetPosition(26, 0, 0);
  EventCamera_FollowActor(gMainRoutineWork.currentOwner, 1);
  FieldCamera_WaitForArrival();
  Event_End();
  return;
}

void EasternAlhafra_AskToHelpWithMast(void) {
  s32 result0;
  s32 result1;
  s32 result2;
  s32 result3;
  struct Actor *object4;
  struct Actor *object5;
  struct Actor *object6;
  struct Actor *object7;

  result0 = Event_GetFlag(FLAG_BRIGGS_CAUGHT);
  if (result0 == 0) {
    goto L_020097dc;
  }
  result1 = Event_GetFlag(FLAG_ALHAFRA_MAST_CLEARED);
  if (result1 != 0) {
    goto L_020097dc;
  }
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgYouDoPlan);
  Event_SetFlag(FLAG_ASKED_TO_HELP);
  Actor_WalkToAndWait(4, 264, 448);
  FieldActor_LaunchFromLeader(26, 0, -16, 0x4000);
  FieldActor_LaunchFromLeader(5, 16, 0, 0x8000);
  FieldActor_LaunchFromLeader(6, 16, -16, 0x6000);
  result2 = Event_GetFlag(FLAG_IN_PARTY(ACTOR_PIERS));
  if (result2 != 0) {
    FieldActor_LaunchFromLeader(7, 8, 16, 0xc000);
  }
  FieldActor_WaitForMoveById(6);
  Actor_FaceDirection(4, FACING_EAST, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(26, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(26, 0, 10);
  Actor_FaceDirection(4, FACING_NORTH, 0);
  FieldEvent_Wait(20);
  Actor_ShowEmote(6, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  FieldEvent_ShowMessageThenWait(6, 0, 10);
  Actor_FaceDirection(5, FACING_NORTH, 0);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(5, 0, 10);
  Actor_FaceDirection(5, FACING_WEST, 0);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(5, 0, 10);
  Actor_FaceDirection(4, FACING_EAST, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(4, 3);
  FieldEvent_Wait(30);
  Actor_ShowEmote(26, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 30);
  FieldEvent_ShowMessageThenWait(26, 0, 10);
  result3 = Event_GetFlag(FLAG_IN_PARTY(ACTOR_PIERS));
  if (result3 != 0) {
    Actor_SetSpeed(7, 1.2);
    Actor_SetAnimation(7, 2);
    object4 = Actor_GetById(gMainRoutineWork.currentOwner);
    if (object4 != 0) {
      Actor_SetDestination(7, *(s16 *)((u8 *)object4 + 10),
                           *(s16 *)((u8 *)object4 + 18));
    }
    FieldActor_WaitForMoveById(7);
    Actor_SetPosition(7, 0, 0);
  }
  Actor_SetSpeed(5, 1.2);
  Actor_SetAnimation(5, 2);
  object5 = Actor_GetById(gMainRoutineWork.currentOwner);
  if (object5 != 0) {
    Actor_SetDestination(5, *(s16 *)((u8 *)object5 + 10),
                         *(s16 *)((u8 *)object5 + 18));
  }
  FieldActor_WaitForMoveById(5);
  Actor_SetPosition(5, 0, 0);
  Actor_SetSpeed(6, 1.2);
  Actor_SetAnimation(6, 2);
  object6 = Actor_GetById(gMainRoutineWork.currentOwner);
  if (object6 != 0) {
    Actor_SetDestination(6, *(s16 *)((u8 *)object6 + 10),
                         *(s16 *)((u8 *)object6 + 18));
  }
  FieldActor_WaitForMoveById(6);
  Actor_SetPosition(6, 0, 0);
  Actor_SetSpeed(26, 1.2);
  Actor_SetAnimation(26, 2);
  object7 = Actor_GetById(gMainRoutineWork.currentOwner);
  if (object7 != 0) {
    Actor_SetDestination(26, *(s16 *)((u8 *)object7 + 10),
                         *(s16 *)((u8 *)object7 + 18));
  }
  FieldActor_WaitForMoveById(26);
  Actor_SetPosition(26, 0, 0);
  Event_End();
L_020097dc:
  return;
}

void EasternAlhafra_AskIfFinished(void) {
  s32 result0;
  s32 result1;
  s32 result2;
  s32 result3;
  struct Actor *object4;
  struct Actor *object5;
  struct Actor *object6;
  struct Actor *object7;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgAreYouFinished);
  Actor_WalkToAndWait(4, 232, 392);
  FieldActor_LaunchFromLeader(26, 0, -16, 0x4000);
  FieldActor_LaunchFromLeader(5, 16, 0, 0x8000);
  FieldActor_LaunchFromLeader(6, 16, -16, 0x6000);
  result0 = Event_GetFlag(FLAG_IN_PARTY(ACTOR_PIERS));
  if (result0 != 0) {
    FieldActor_LaunchFromLeader(7, 8, 16, 0xc000);
  }
  FieldActor_WaitForMoveById(6);
  Actor_FaceDirection(4, FACING_NORTH, 0);
  FieldEvent_Wait(20);
  Actor_RunRepeatedMotion(26, 2);
  FieldEvent_Wait(10);
  Event_OpenMessage(26, 0);
  result1 = FieldEvent_AskYesNoWithActorAnimation(4, 0);
  if (result1 == 0) {
    FieldEvent_Wait(20);
    Actor_ShowEmote(6, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
    FieldEvent_ShowMessageThenWait(6, 0, 10);
    Actor_FaceDirection(4, FACING_NORTHEAST, 0);
    Actor_FaceDirection(5, FACING_NORTH, 0);
    FieldEvent_Wait(10);
    Actor_SetAnimationAndWait(5, 4);
    FieldEvent_ShowMessageThenWait(5, 0, 10);
    gFieldWork->message += 2;
  } else {
    gFieldWork->message += 2;
    FieldEvent_Wait(40);
    Actor_SetAnimationAndWait(6, 3);
    Actor_FaceDirection(4, FACING_NORTHEAST, 0);
    FieldEvent_Wait(20);
    FieldEvent_ShowMessageThenWait(6, 0, 10);
    Actor_SetAnimationAndWait(5, 4);
    FieldEvent_ShowMessageThenWait(5, 0, 10);
  }
  result2 = ((s32 (*)())GameFlag_Get)(7);
  if (result2 != 0) {
    Actor_FaceDirection(4, FACING_NORTH, 0);
    Actor_FaceDirection(5, FACING_NORTHWEST, 0);
    Actor_FaceDirectionAndWait(6, FACING_WEST);
    FieldEvent_Wait(10);
  } else {
    ((s32 (*)())Actor_ShowEmoteById)(26, 0x102, 30);
    FieldEvent_ShowMessageThenWait(26, 0, 10);
    Actor_FaceDirection(4, FACING_NORTH, 0);
    Actor_FaceDirection(5, FACING_NORTHWEST, 0);
    Actor_FaceDirection(6, FACING_WEST, 0);
    FieldEvent_Wait(30);
    Actor_ShowEmote(26, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
    Actor_SetAnimationAndWait(26, 4);
    FieldEvent_ShowMessageThenWait(26, 0, 10);
    ((s32 (*)())Actor_ShowEmoteById)(26, 0x109, 30);
    FieldEvent_ShowMessageThenWait(26, 0, 10);
    Actor_FaceDirection(5, FACING_NORTH, 0);
    Actor_FaceDirection(6, FACING_SOUTH, 0);
    FieldEvent_Wait(40);
    Actor_FaceDirection(5, FACING_NORTHWEST, 0);
    Actor_FaceDirection(6, FACING_WEST, 0);
    FieldEvent_Wait(10);
    Actor_SetAnimationAndWait(26, 4);
    FieldEvent_ShowMessageThenWait(26, 0, 10);
  }
  FieldEvent_SetMessageId((s32)MsgSoIThink);
  Actor_SetAnimationAndWait(26, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(26, 0, 10);
  Actor_SetAnimation(7, 3);
  Actor_SetAnimation(5, 3);
  Actor_SetAnimation(6, 3);
  Actor_SetAnimationAndWait(4, 3);
  FieldEvent_Wait(10);
  result3 = Event_GetFlag(FLAG_IN_PARTY(ACTOR_PIERS));
  if (result3 != 0) {
    Actor_SetSpeed(7, 1.2);
    Actor_SetAnimation(7, 2);
    object4 = Actor_GetById(gMainRoutineWork.currentOwner);
    if (object4 != 0) {
      Actor_SetDestination(7, *(s16 *)((u8 *)object4 + 10),
                           *(s16 *)((u8 *)object4 + 18));
    }
    FieldActor_WaitForMoveById(7);
    Actor_SetPosition(7, 0, 0);
  }
  Actor_SetSpeed(5, 1.2);
  Actor_SetAnimation(5, 2);
  object5 = Actor_GetById(gMainRoutineWork.currentOwner);
  if (object5 != 0) {
    Actor_SetDestination(5, *(s16 *)((u8 *)object5 + 10),
                         *(s16 *)((u8 *)object5 + 18));
  }
  FieldActor_WaitForMoveById(5);
  Actor_SetPosition(5, 0, 0);
  Actor_SetSpeed(6, 1.2);
  Actor_SetAnimation(6, 2);
  object6 = Actor_GetById(gMainRoutineWork.currentOwner);
  if (object6 != 0) {
    Actor_SetDestination(6, *(s16 *)((u8 *)object6 + 10),
                         *(s16 *)((u8 *)object6 + 18));
  }
  FieldActor_WaitForMoveById(6);
  Actor_SetPosition(6, 0, 0);
  Actor_SetSpeed(26, 1.2);
  Actor_SetAnimation(26, 2);
  object7 = Actor_GetById(gMainRoutineWork.currentOwner);
  if (object7 != 0) {
    Actor_SetDestination(26, *(s16 *)((u8 *)object7 + 10),
                         *(s16 *)((u8 *)object7 + 18));
  }
  FieldActor_WaitForMoveById(26);
  Actor_SetPosition(26, 0, 0);
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

  EasternAlhafra_SpawnPuff__puffs(object->x + px(8), object->y + px(28),
                                  object->z, 0x1000, 15);
  Event_PlaySound(SE_LIGHT_APPEAR);
  FieldEvent_Wait(15);
  EasternAlhafra_SpawnPuff__puffs(object->x - px(8), object->y + px(28),
                                  object->z, 0xf000, 15);
  Event_PlaySound(SE_LIGHT_APPEAR);
  FieldEvent_Wait(15);
  EasternAlhafra_SpawnPuff__puffs(object->x + px(8), object->y + px(28),
                                  object->z, 0x1000, 15);
  Event_PlaySound(SE_LIGHT_APPEAR);
  FieldEvent_Wait(15);
  EasternAlhafra_SpawnPuff__puffs(object->x - px(8), object->y + px(28),
                                  object->z, 0xf000, 15);
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
  Actor_SetSpeed(28, 2.8);
  Actor_SetSpeed(29, 2.8);
  Actor_SetSpeed(23, 1.8);
  Actor_SetSpeed(24, 1.8);
  Actor_SetPosition(28, 328, 616);
  Actor_FaceDirection(28, FACING_NORTHEAST, 0);
  Actor_SetPosition(29, 296, 616);
  Actor_FaceDirection(29, FACING_NORTHEAST, 0);
  FieldEvent_ShowMessageThenWait(28, 0, 10);
  Actor_FaceDirection(4, FACING_SOUTH, 0);
  {
    s32 mode = 1;

    FieldCamera_MoveTo(328, CAMERA_KEEP, 632, mode);
  }
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(20);
  FieldActor_StartRepeatedMotion(28, 3);
  Actor_SetAttachedEffect(28, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(50);
  FieldActor_ShowEmote(0x1d, 0x102, 0x1e);
  FieldEvent_ShowMessageThenWait(28, 0, 10);
  Actor_FaceDirection(28, FACING_WEST, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(28, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(28, 0, 10);
  Actor_FaceDirection(29, FACING_EAST, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(29, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(28, 0, 10);
  Actor_FaceDirection(29, FACING_WEST, 0);
  FieldEvent_Wait(10);
  Actor_WalkBy(28, -160, 0);
  Actor_WalkByAndWait(29, -160, 0);
  Actor_SetPosition(28, 0, 0);
  Actor_SetPosition(29, 0, 0);
  FieldEvent_Wait(30);
  Actor_SetPosition(23, 264, 384);
  Actor_FaceDirection(23, FACING_EAST, 0);
  Actor_SetPosition(24, 280, 392);
  Actor_FaceDirection(24, FACING_EAST, 0);
  Actor_SetPosition(4, 232, 384);
  Actor_FaceDirection(4, FACING_EAST, 0);
  FieldEvent_ShowMessageThenWait(24, 0, 10);
  FieldEvent_ShowMessageThenWait(23, 0, 10);
  FieldEvent_ShowMessageThenWait(24, 0, 10);
  FieldEvent_ShowMessageThenWait(23, 0, 10);
  FieldCamera_MoveTo(264, CAMERA_KEEP, 392, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(20);
  Actor_WalkByAndWait(23, -16, 0);
  FieldEvent_Wait(20);
  FieldMessage_ShowAtActor_(0x17, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(4, 0) == 0) {
    FieldEvent_Wait(20);
    Actor_SetAnimationAndWait(23, 3);
    FieldEvent_Wait(10);
    FieldEvent_ShowMessageThenWait(23, 0, 10);
    gFieldWork->message += 1;
  } else {
    FieldEvent_Wait(40);
    Actor_SetAnimationAndWait(23, 4);
    FieldEvent_Wait(10);
    gFieldWork->message += 1;
    FieldEvent_ShowMessageThenWait(23, 0, 10);
  }
  Actor_FaceDirection(23, FACING_EAST, 0);
  FieldEvent_Wait(20);
  Actor_FaceDirection(24, FACING_WEST, 0);
  FieldEvent_Wait(30);
  FieldEvent_ShowMessageThenWait(23, 0, 10);
  FieldActor_ShowEmote(0x18, 0x108, 0x2d);
  FieldActor_Jump(24, 6, 15);
  FieldActor_Jump(24, 6, 23);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(23, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(23, 0, 10);
  Actor_SetAnimationAndWait(24, 3);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(24, 3);
  FieldEvent_Wait(20);
  Actor_WalkByAndWait(23, 0, 8);
  FieldActor_WatchAndSetScript(4, 23);
  Actor_WalkBy(24, -96, 0);
  Actor_WalkByAndWait(23, -96, 0);
  Actor_WalkBy(23, 0, 80);
  Actor_WalkByAndWait(24, -32, 0);
  Actor_WalkByAndWait(24, 0, 80);
  FieldActor_Stop(4);
  Actor_SetPosition(24, 0, 0);
  Actor_SetPosition(23, 0, 0);
  EventCamera_FollowActor(4, 1);
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
  Actor_SetPosition(25, 56, 632);
  Actor_FaceDirection(25, FACING_EAST, 0);
  Actor_SetPosition(27, 56, 648);
  Actor_FaceDirection(27, FACING_EAST, 0);
  Actor_SetPosition(28, 48, 616);
  Actor_FaceDirection(28, FACING_EAST, 0);
  Actor_SetPosition(29, 32, 608);
  Actor_FaceDirection(29, FACING_EAST, 0);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(25, 0, 10);
  FieldCamera_MoveTo(144, CAMERA_KEEP, 664, 1);
  FieldEvent_Wait(10);
  Actor_SetSpeed(28, 1);
  Actor_SetSpeed(29, 1);
  Actor_SetSpeed(25, 1);
  Actor_SetSpeed(27, 1);
  Actor_WalkBy(25, 32, 0);
  Actor_WalkBy(27, 32, 0);
  Actor_WalkBy(28, 24, 24);
  Actor_WalkBy(29, 24, 24);
  FieldActor_WaitForMoveById(28);
  FieldActor_WaitForMoveById(29);
  Actor_FaceDirection(28, FACING_EAST, 0);
  Actor_FaceDirection(29, FACING_EAST, 0);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(10);
  FieldActor_ShowEmote(0x1c, 0x107, 0);
  FieldActor_ShowEmote(0x1c, 0x107, 0x28);
  Actor_SetSpeed(28, 3);
  Actor_SetSpeed(29, 3);
  Actor_SetScript(28, gEasternAlhafra_FirstWorkerCheerScript);
  Actor_SetScript(29, gEasternAlhafra_SecondWorkerCheerScript);
  FieldActor_RefreshSelectorById(28);
  FieldActor_RefreshSelectorById(29);
  FieldEvent_Wait(10);
  Actor_RunRepeatedMotion(28, 2);
  FieldEvent_ShowMessageThenWait(28, 0, 10);
  FieldActor_Jump(29, 6, 15);
  FieldActor_Jump(29, 6, 23);
  FieldEvent_ShowMessageThenWait(29, 0, 10);
  Actor_SetSpeed(28, 1.6);
  Actor_SetSpeed(29, 1.6);
  Actor_SetSpeed(25, 1.6);
  Actor_SetSpeed(27, 1.6);
  FieldActor_StartRepeatedMotion(25, 3);
  Actor_SetAttachedEffect(25, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldActor_StartRepeatedMotion(27, 3);
  Actor_SetAttachedEffect(27, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(40);
  Actor_SetScript(28, gEasternAlhafra_FirstWorkerRunScript);
  Actor_SetScript(29, gEasternAlhafra_SecondWorkerRunScript);
  FieldEvent_Wait(20);
  EventCamera_FollowActor(25, 1);
  Actor_WalkBy(25, 64, 0);
  Actor_WalkByAndWait(27, 64, 0);
  Actor_WalkTo(25, 168, 616);
  Actor_WalkToAndWait(27, 184, 616);
  FieldActor_RefreshSelectorById(25);
  Actor_SetAnimation(25, 1);
  Actor_FaceDirection(25, FACING_NORTHEAST, 0);
  Actor_FaceDirection(27, FACING_NORTHEAST, 0);
  FieldActor_RefreshSelectorById(28);
  FieldActor_RefreshSelectorById(29);
  FieldEvent_Wait(20);
  FieldActor_StartRepeatedMotion(25, 3);
  Actor_SetAttachedEffect(25, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldActor_StartRepeatedMotion(27, 3);
  Actor_SetAttachedEffect(27, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(40);
  FieldCamera_SetSpeed(6);
  FieldCamera_MoveTo(384, CAMERA_KEEP, 480, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(25, 0, 10);
  FieldCamera_SetSpeed(6);
  FieldCamera_MoveTo(480, CAMERA_KEEP, 344, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(25, 0, 10);
  FieldCamera_SetSpeed(6);
  FieldCamera_MoveTo(488, CAMERA_KEEP, 456, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(25, 0, 10);
  FieldCamera_SetSpeed(6);
  FieldCamera_MoveTo(192, CAMERA_KEEP, 616, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(20);
  FieldActor_ShowEmote(0x19, 0x102, 0x1e);
  Actor_FaceDirection(25, FACING_NORTH, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(25, 0, 10);
  Actor_FaceDirection(28, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(29, FACING_SOUTH_SOUTHWEST, 0);
  FieldEvent_Wait(30);
  Actor_SetAnimation(28, 3);
  Actor_SetAnimationAndWait(29, 3);
  FieldEvent_Wait(20);
  Actor_FaceDirection(27, FACING_WEST, 0);
  FieldEvent_Wait(30);
  FieldActor_ShowEmote(0x1b, 0x108, 0x1e);
  FieldEvent_ShowMessageThenWait(27, 0, 10);
  Actor_FaceDirection(25, FACING_EAST, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(25, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(25, 0, 10);
  Actor_SetAnimation(28, 3);
  Actor_SetAnimationAndWait(29, 3);
  FieldEvent_Wait(20);
  Actor_FaceDirection(25, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirection(27, FACING_NORTH_NORTHWEST, 0);
  FieldEvent_Wait(40);
  FieldActor_ShowEmote(0x19, 0x101, 0x1e);
  FieldEvent_ShowMessageThenWait(25, 0, 10);
  FieldActor_StartRepeatedMotion(28, 3);
  Actor_SetAttachedEffect(28, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldActor_StartRepeatedMotion(29, 3);
  Actor_SetAttachedEffect(29, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(50);
  Actor_FaceDirection(28, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirection(29, FACING_NORTH_NORTHWEST, 0);
  FieldEvent_Wait(40);
  FieldActor_ShowEmote(0x1c, 0x100, 0);
  FieldActor_ShowEmote(0x1d, 0x100, 0x28);
  Actor_FaceDirection(25, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirection(27, FACING_NORTH_NORTHWEST, 0);
  FieldEvent_Wait(20);
  FieldCamera_MoveTo(144, CAMERA_KEEP, 544, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(25, 0, 10);
  FieldActor_ShowEmote(4, 0x102, 0x1e);
  Actor_SetSpeed(25, 1.5);
  Actor_SetSpeed(27, 1.5);
  Actor_WalkBy(25, -32, -32);
  Actor_WalkByAndWait(27, -32, -32);
  Actor_WalkBy(25, 0, -24);
  Actor_WalkByAndWait(27, 0, -24);
  FieldActor_WaitForMoveById(25);
  Actor_FaceDirection(25, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirection(27, FACING_NORTH_NORTHWEST, 0);
  FieldEvent_Wait(20);
  FieldActor_ShowEmote(0x19, 0x100, 0x1e);
  FieldEvent_ShowMessageThenWait(25, 0, 10);
  Actor_WalkToAndWait(4, 136, 520);
  FieldActor_LaunchFromLeader(26, 16, 0, 0x5000);
  Actor_FaceDirection(4, FACING_SOUTH, 0);
  FieldActor_WaitForMoveById(26);
  FieldEvent_Wait(30);
  Actor_SetAnimationAndWait(26, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | 26, 0, 10);
  Actor_SetAnimationAndWait(25, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(25, 0, 10);
  Actor_SetAnimationAndWait(26, 4);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | 26, 0, 10);
  FieldActor_ShowEmote(0x19, 0x109, 0x1e);
  FieldEvent_ShowMessageThenWait(25, 0, 10);
  Actor_FaceDirection(26, FACING_WEST, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | 26, 0, 10);
  Actor_RunRepeatedMotion(27, 2);
  FieldEvent_Wait(20);
  Actor_FaceDirection(26, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirection(27, FACING_WEST, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(27, 0, 10);
  FieldActor_ShowEmote(0x19, 0x100, 0x1e);
  Actor_FaceDirection(25, FACING_EAST, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(25, 0, 10);
  Actor_FaceDirection(27, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(25, FACING_SOUTH_SOUTHEAST, 0);
  FieldEvent_Wait(20);
  FieldActor_Jump(25, 6, 15);
  FieldActor_Jump(25, 6, 23);
  FieldEvent_ShowMessageThenWait(25, 0, 10);
  Actor_RunRepeatedMotion(28, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(28, 0, 10);
  Actor_SetAnimationAndWait(25, 3);
  FieldEvent_Wait(20);
  Actor_RunRepeatedMotion(29, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(29, 0, 10);
  Actor_SetAnimationAndWait(25, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(25, 0, 20);
  Actor_WalkBy(28, 0, 64);
  Actor_WalkBy(29, 0, 64);
  FieldEvent_Wait(60);
  Actor_SetPosition(28, 0, 0);
  Actor_SetPosition(29, 0, 0);
  Actor_FaceDirection(27, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirection(25, FACING_NORTH_NORTHEAST, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(25, 0, 10);
  Actor_SetAnimationAndWait(25, 3);
  FieldEvent_Wait(10);
  FieldMessage_ShowAtActor_(0x19, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(4, 0) != 0) {
    FieldEvent_Wait(40);
    Actor_FaceDirection(26, FACING_WEST, 0);
    FieldEvent_Wait(20);
    Actor_SetAnimationAndWait(26, 4);
    FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | 26, 0, 10);
    gFieldWork->message += 1;
  } else {
    FieldEvent_Wait(30);
    Actor_FaceDirection(26, FACING_WEST, 0);
    FieldEvent_Wait(20);
    Actor_SetAnimationAndWait(26, 3);
    FieldEvent_Wait(10);
    gFieldWork->message += 1;
    FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | 26, 0, 10);
  }
  Actor_FaceDirection(4, FACING_EAST, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(4, 3);
  FieldEvent_Wait(30);
  Actor_SetAnimationAndWait(25, 3);
  FieldEvent_Wait(30);
  Actor_FaceDirection(4, FACING_SOUTH, 0);
  Actor_FaceDirection(26, FACING_SOUTH_SOUTHEAST, 0);
  FieldEvent_Wait(20);
  Actor_FaceDirection(25, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(27, FACING_SOUTH_SOUTHEAST, 0);
  FieldEvent_Wait(20);
  Actor_SetSpeed(4, 0.8);
  Actor_SetSpeed(26, 0.8);
  Actor_SetSpeed(25, 0.8);
  Actor_SetSpeed(27, 0.8);
  Actor_GetById(4)->motionFlags = 0;
  Actor_GetById(26)->motionFlags = 0;
  Actor_GetById(25)->motionFlags = 0;
  Actor_GetById(27)->motionFlags = 0;
  Actor_WalkBy(4, 0, 120);
  Actor_WalkBy(26, 0, 120);
  Actor_WalkBy(27, 0, 80);
  Actor_WalkByAndWait(25, 0, 80);
  {
    struct FieldWork *event = Ram_HeapSlots->fieldWork;

    event->transitionFrames = 60;
    event->transition = 0x100;
  }
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_SetDestination((s32)&SceneId_AlhafraManor, 81);
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
  FieldEvent_ShowMessageThenWait(29, 0, 10);
  Actor_WalkToAndWait(4, 136, 552);
  FieldActor_LaunchFromLeader(26, -16, 0, 0xa000);
  Actor_ArmCallback(4, FACING_NORTH, 0);
  FieldActor_WaitForMoveById(26);
  FieldEvent_Wait(20);
  FieldActor_ShowEmote(0x1a, 0x100, 0x1e);
  FieldCamera_MoveTo(176, CAMERA_KEEP, 392, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(20);
  Actor_RunRepeatedMotion(25, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(25, 0, 10);
  Actor_RunRepeatedMotion(27, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(27, 0, 10);
  Actor_SetPosition(22, 224, 8);
  FieldEvent_ShowMessageThenWait(22, 0, 10);
  Ram_HeapSlots->mapWork->minX = 0;
  Ram_HeapSlots->mapWork->maxX = 0x4000000;
  Ram_HeapSlots->mapWork->minZ = 0;
  Ram_HeapSlots->mapWork->maxZ = 0x4000000;
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

    FieldActor_SetPositionAndResetMotion(center, center->x, center->y,
                                         center->z);
  }
  FieldEvent_Wait(50);
  FieldActor_SetSpritePriority(22, 1);
  FieldActor_SetSpritePriority(23, 1);
  Actor_SetSpeed(23, 1.2);
  Actor_SetSpeed(22, 1.2);
  Actor_SetPosition(22, 792, 856);
  Actor_SetPosition(23, 824, 856);
  Actor_SetPosition(26, 744, 1016);
  Actor_SetPosition(25, 744, 1016);
  Event_PlaySound(MUS_035);
  PaletteFade_ApplyTarget(0x10000, 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_Wait(30);
  FieldCamera_SetSpeed(2);
  FieldCamera_MoveTo(808, CAMERA_KEEP, 848, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(20);
  FieldActor_Jump(22, 6, 15);
  FieldActor_Jump(22, 6, 23);
  FieldEvent_ShowMessageThenWait(22, 0, 10);
  Actor_SetAnimationAndWait(23, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(23, 0, 10);
  FieldEvent_ShowMessageThenWait(25, 0, 10);
  Actor_ArmCallback(22, FACING_EAST, 0);
  Actor_FaceDirection(23, FACING_WEST, 0);
  FieldEvent_Wait(25);
  Actor_SetAnimationAndWait(22, 3);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(23, 3);
  FieldEvent_Wait(20);
  Actor_FaceDirection(22, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirection(23, FACING_SOUTH_SOUTHWEST, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(22, 0, 10);
  FieldEvent_ShowMessageThenWait(26, 0, 10);
  FieldActor_ShowEmote(0x17, 0x100, 0);
  FieldActor_ShowEmote(0x16, 0x100, 0x32);
  FieldActor_StartRepeatedMotion(22, 3);
  Actor_SetAttachedEffect(22, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(40);
  Actor_WalkByAndWait(22, 0, 16);
  FieldEvent_Wait(20);
  FieldActor_ShowEmote(0x16, 0x102, 0x1e);
  FieldEvent_ShowMessageThenWait(22, 0, 10);
  Actor_WalkByAndWait(23, 0, 16);
  Actor_FaceDirection(23, FACING_WEST, 0);
  FieldEvent_Wait(20);
  FieldActor_ShowEmote(0x17, 0x109, 0x1e);
  FieldEvent_ShowMessageThenWait(23, 0, 10);
  Actor_FaceDirection(23, FACING_SOUTH_SOUTHWEST, 0);
  FieldEvent_Wait(50);
  Actor_FaceDirection(23, FACING_WEST, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(23, 0, 10);
  Actor_ArmCallback(22, FACING_EAST, 0);
  FieldEvent_Wait(20);
  FieldActor_ShowEmote(0x16, 0x101, 0x2d);
  Actor_WalkByAndWait(22, 0, 32);
  FieldEvent_Wait(30);
  FieldActor_ShowEmote(0x16, 0x106, 0x2d);
  Actor_WalkByAndWait(22, 0, -32);
  Actor_ArmCallback(22, FACING_EAST, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(22, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(22, 0, 10);
  Actor_FaceDirection(23, FACING_SOUTH_SOUTHWEST, 0);
  FieldEvent_Wait(50);
  Actor_ArmCallback(23, FACING_WEST, 0);
  FieldEvent_Wait(20);
  FieldActor_ShowEmote(0x17, 0x108, 0x1e);
  FieldEvent_ShowMessageThenWait(23, 0, 10);
  FieldActor_ShowEmote(0x16, 0x101, 0x32);
  Actor_WalkByAndWait(22, 0, 32);
  FieldEvent_Wait(30);
  FieldActor_ShowEmote(0x16, 0x106, 0x32);
  Actor_WalkByAndWait(22, 0, -32);
  Actor_ArmCallback(22, FACING_EAST, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(22, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(22, 0, 10);
  FieldActor_ShowEmote(0x17, 0x109, 0x1e);
  FieldEvent_ShowMessageThenWait(23, 0, 10);
  Actor_FaceDirection(22, FACING_SOUTH_SOUTHWEST, 0);
  FieldEvent_Wait(30);
  EasternAlhafra_PuffFrom(22);
  Event_PlaySound(SE_CURSE);
  FieldActor_ShowEmote(0x16, 0x105, 0x32);
  Actor_ArmCallback(22, FACING_EAST, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(23, 4);
  FieldEvent_ShowMessageThenWait(23, 0, 10);
  Actor_FaceDirection(22, FACING_SOUTH_SOUTHWEST, 0);
  FieldEvent_Wait(30);
  EasternAlhafra_PuffFrom(22);
  FieldActor_ShowEmote(0x16, 0x106, 0x32);
  Actor_ArmCallback(22, FACING_EAST, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(22, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(22, 0, 10);
  FieldActor_ShowEmote(0x17, 0x109, 0x1e);
  FieldEvent_ShowMessageThenWait(23, 0, 10);
  Actor_RunRepeatedMotion(22, 2);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(22, 3);
  FieldEvent_Wait(20);
  Actor_FaceDirectionAndWait(22, FACING_SOUTH_SOUTHWEST);
  Actor_SetAnimation(22, 7);
  FieldEvent_Wait(20);
  Actor_SetAnimation(22, 6);
  FieldEvent_Wait(5);
  FieldEvent_ShowMessageThenWait(22, 0, 10);
  FieldActor_Jump(23, 6, 15);
  FieldActor_Jump(23, 6, 23);
  FieldEvent_ShowMessageThenWait(23, 0, 10);
  FieldActor_ShowEmote(0x16, 0x102, 0x32);
  Actor_SetAnimation(22, 7);
  FieldEvent_Wait(20);
  Actor_SetAnimation(22, 1);
  Actor_ArmCallback(22, FACING_EAST, 0);
  FieldEvent_Wait(15);
  FieldEvent_ShowMessageThenWait(22, 0, 10);
  FieldActor_ShowEmote(0x17, 0x103, 0x1e);
  FieldEvent_ShowMessageThenWait(23, 0, 10);
  FieldActor_ShowEmote(0x16, 0x106, 0x28);
  FieldEvent_ShowMessageThenWait(22, 0, 10);
  Actor_WalkByAndWait(22, 0, 8);
  Actor_WalkByAndWait(22, 10, 16);
  Actor_FaceDirection(23, FACING_SOUTH_SOUTHWEST, 0);
  Actor_WalkByAndWait(22, 6, 16);
  Actor_ArmCallback(22, 0x5000, 0);
  FieldEvent_Wait(10);
  FieldActor_Jump(22, 6, 15);
  FieldActor_Jump(22, 6, 23);
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | 22, 0, 10);
  FieldEvent_ShowMessageThenWait(26, 0, 10);
  FieldActor_ShowEmote(0x16, 0x108, 0x1e);
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | 22, 0, 20);
  Actor_SetAnimation(22, 8);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(22, 0, 10);
  FieldActor_Jump(22, 7, 0);
  i = 15;
turn:
  ((s16 *)Actor_GetById(22))[3] -= 0x1000;
  i--;
  System_Wait(1);
  if (i >= 0)
    goto turn;
  Actor_SetAnimation(22, 5);
  FieldEvent_Wait(7);
  FieldEvent_ShowMessageThenWait(22, 0, 20);
  FieldActor_ShowEmote(0x17, 0x108, 0x32);
  Actor_WalkByAndWait(23, 0, 36);
  FieldEvent_Wait(15);
  Actor_SetAnimation(22, 1);
  FieldEvent_Wait(20);
  FieldActor_Jump(23, 6, 15);
  FieldActor_Jump(23, 6, 23);
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | 23, 0, 15);
  Actor_SetAnimationAndWait(22, 3);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(22, 3);
  FieldEvent_Wait(10);
  Actor_SetAnimation(22, 9);
  FieldEvent_Wait(70);
  map = Ram_HeapSlots->mapWork;
  Actor_GetById(22)->motionFlags = 0;
  Actor_GetById(23)->motionFlags = 0;
  MapGrid_CopyCellsLayered(42, 60, 17, 4, 42, 64);
  MapGrid_CopyCellsLayered(42, 60, 17, 4, 42, 68);
  MapGrid_CopyCellsLayered(106, 60, 17, 4, 106, 64);
  MapGrid_CopyCellsLayered(106, 60, 17, 4, 106, 68);
  i = 239;
scroll:
  *(s32 *)(map->layers[0].unk08 + 4) += step;
  *(s32 *)(map->layers[1].unk08 + 4) += 0x10000;
  Actor_GetById(22)->z -= step;
  i--;
  Actor_GetById(23)->z -= step;
  System_Wait(1);
  if (i >= 0)
    goto scroll;
  Event_PlaySound(SOUND_FADE_OUT_BGM);
  FieldEvent_Wait(30);
  Event_End();
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  Event_SetDestination((s32)&SceneId_EasternAlhafra1, 99);
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

    FieldActor_SetPositionAndResetMotion(center, center->x, center->y,
                                         center->z);
  }
  FieldEvent_Wait(30);
  PaletteFade_ApplyTarget(0x10000, 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_Wait(15);
  FieldEvent_ShowMessageThenWait(25, 0, 10);
  FieldCamera_MoveTo(152, CAMERA_KEEP, 392, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(20);
  FieldActor_ShowEmote(0x19, 0x103, 0x1e);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | 25, 0, 10);
  Actor_FaceDirection(25, FACING_SOUTH_SOUTHWEST, 0);
  FieldEvent_Wait(30);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | 25, 0, 10);
  FieldActor_Jump(25, 6, 15);
  FieldActor_Jump(25, 6, 23);
  FieldEvent_Wait(10);
  FieldActor_ShowEmote(0x19, 0x109, 0x1e);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | 25, 0, 10);
  Actor_FaceDirection(25, FACING_NORTH_NORTHEAST, 0);
  FieldEvent_Wait(30);
  FieldActor_ShowEmote(0x19, 0x100, 0x1e);
  Actor_SetSpeed(25, 0.7);
  Actor_GetById(25)->actionFlags &= 0xfe;
  Actor_WalkByAndWait(25, 0, 8);
  FieldEvent_Wait(1);
  Actor_GetById(25)->actionFlags |= 1;
  FieldEvent_Wait(20);
  Actor_RunRepeatedMotion(25, 2);
  FieldEvent_Wait(10);
  Actor_SetSpeed(25, 2.4);
  Actor_GetById(25)->actionFlags &= 0xfe;
  Actor_WalkByAndWait(25, 0, -16);
  FieldEvent_Wait(1);
  Actor_GetById(25)->actionFlags |= 1;
  Actor_GetById(25)->actionFlags &= 0xfe;
  Actor_WalkBy(25, 0, 8);
  Event_PlaySound(SE_IMPACT);
  Actor_SetSpeed(29, 2.4);
  Actor_GetById(29)->actionFlags &= 0xfe;
  Actor_WalkByAndWait(29, 0, -4);
  FieldEvent_Wait(1);
  Actor_GetById(29)->actionFlags |= 1;
  Actor_GetById(29)->facing = FACING_WEST;
  Actor_GetById(29)->actionFlags &= 0xfe;
  Actor_WalkByAndWait(29, 0, -4);
  FieldEvent_Wait(1);
  Actor_GetById(29)->actionFlags |= 1;
  Actor_GetById(29)->facing = 0xb000;
  Actor_GetById(29)->actionFlags &= 0xfe;
  Actor_WalkByAndWait(29, 0, -4);
  FieldEvent_Wait(1);
  Actor_GetById(29)->actionFlags |= 1;
  Actor_GetById(29)->facing = 0xd000;
  Actor_GetById(29)->actionFlags &= 0xfe;
  Actor_WalkByAndWait(29, 0, -4);
  FieldEvent_Wait(1);
  Actor_GetById(29)->actionFlags |= 1;
  Actor_GetById(29)->facing = 0;
  Actor_GetById(29)->actionFlags &= 0xfe;
  Actor_WalkByAndWait(29, 0, -4);
  FieldEvent_Wait(1);
  Actor_GetById(29)->actionFlags |= 1;
  Actor_GetById(29)->facing = 0x3000;
  FieldEvent_Wait(20);
  Actor_GetById(25)->actionFlags |= 1;
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | 25, 0, 10);
  Actor_SetPosition(29, 168, 368);
  FieldActor_SetSpriteRotationById(29, 0);
  Actor_SetSpriteFlags(Actor_GetById(29), 1);
  Actor_FaceDirection(29, FACING_SOUTH_SOUTHWEST, 0);
  FieldActor_Jump(29, 6, 40);
  FieldActor_ShowEmote(0x1d, 0x102, 0x28);
  Actor_ArmCallback(25, 0x3000, 0);
  FieldEvent_Wait(30);
  Actor_SetSpeed(25, 0.7);
  Actor_GetById(25)->actionFlags &= 0xfe;
  Actor_WalkByAndWait(25, 0, -8);
  FieldEvent_Wait(1);
  Actor_GetById(25)->actionFlags |= 1;
  FieldEvent_Wait(20);
  Actor_SetSpeed(25, 2.4);
  Actor_GetById(25)->actionFlags &= 0xfe;
  Actor_WalkByAndWait(25, 0, 16);
  FieldEvent_Wait(1);
  Actor_GetById(25)->actionFlags |= 1;
  Event_PlaySound(SE_IMPACT);
  Actor_SetSpeed(29, 2.4);
  Actor_GetById(27)->actionFlags &= 0xfe;
  Actor_WalkByAndWait(27, 0, 3);
  FieldEvent_Wait(1);
  Actor_GetById(27)->actionFlags |= 1;
  Actor_GetById(27)->facing = 0;
  Actor_GetById(27)->actionFlags &= 0xfe;
  Actor_WalkByAndWait(27, 0, 3);
  FieldEvent_Wait(1);
  Actor_GetById(27)->actionFlags |= 1;
  Actor_GetById(27)->facing = 0xd000;
  Actor_GetById(27)->actionFlags &= 0xfe;
  Actor_WalkByAndWait(27, 0, 3);
  FieldEvent_Wait(1);
  Actor_GetById(27)->actionFlags |= 1;
  Actor_GetById(27)->facing = 0xb000;
  Actor_GetById(27)->actionFlags &= 0xfe;
  Actor_WalkByAndWait(27, 0, 3);
  FieldEvent_Wait(1);
  Actor_GetById(27)->actionFlags |= 1;
  Actor_GetById(27)->facing = FACING_WEST;
  Actor_GetById(27)->actionFlags &= 0xfe;
  Actor_WalkByAndWait(27, 0, 3);
  FieldEvent_Wait(1);
  Actor_GetById(27)->actionFlags |= 1;
  Actor_GetById(27)->facing = 0x5000;
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | 25, 0, 10);
  Actor_SetPosition(27, 168, 424);
  FieldActor_SetSpriteRotationById(27, 0);
  Actor_SetSpriteFlags(Actor_GetById(27), 1);
  Actor_GetById(27)->facing = 0xb000;
  FieldActor_Jump(27, 6, 40);
  FieldActor_ShowEmote(0x1b, 0x102, 0x28);
  Actor_ArmCallback(25, 0xd000, 0);
  FieldEvent_Wait(30);
  FieldActor_ShowEmote(0x19, 0x107, 0x1e);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | 25, 0, 10);
  FieldActor_ShowEmote(0x1d, 0x109, 0x32);
  Actor_ArmCallback(25, 0x3000, 0);
  FieldEvent_Wait(30);
  FieldActor_ShowEmote(0x19, 0x107, 0x1e);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | 25, 0, 10);
  FieldActor_ShowEmote(0x1b, 0x102, 0x32);
  Actor_SetAnimation(25, 5);
  FieldEvent_Wait(30);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | 25, 0, 10);
  Actor_ArmCallback(29, 0x3000, 0);
  Actor_ArmCallback(27, 0xd000, 0);
  FieldEvent_Wait(20);
  FieldActor_StartRepeatedMotion(29, 3);
  Actor_SetAttachedEffect(29, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldActor_StartRepeatedMotion(27, 3);
  Actor_SetAttachedEffect(27, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(40);
  Actor_SetSpeed(29, 1.8);
  Actor_WalkByAndWait(29, 16, 16);
  Actor_WalkByAndWait(29, 0, 40);
  Actor_ArmCallback(29, 0xb000, 0);
  FieldEvent_Wait(30);
  Actor_SetAnimation(25, 1);
  FieldEvent_Wait(30);
  Actor_ArmCallback(29, 0xb000, 0);
  Actor_ArmCallback(27, 0xb000, 0);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | 25, 0, 10);
  Actor_SetAnimation(27, 3);
  Actor_SetAnimationAndWait(29, 3);
  FieldEvent_Wait(20);
  Actor_SetSpeed(25, 0.8);
  Actor_SetSpeed(27, 0.8);
  Actor_SetSpeed(29, 0.8);
  Actor_SetSpeed(28, 0.8);
  Actor_SetSpeed(30, 0.8);
  Actor_SetSpeed(31, 0.8);
  EventCamera_FollowActor(25, 1);
  Actor_WalkByAndWait(25, -16, 16);
  Actor_WalkByAndWait(25, 0, 16);
  Actor_WalkTo(25, 152, 504);
  Actor_WalkBy(29, -16, 0);
  Actor_WalkByAndWait(27, -16, 16);
  Actor_WalkBy(29, -16, 16);
  Actor_WalkByAndWait(27, 0, 16);
  Actor_WalkTo(27, 152, 488);
  Actor_WalkToAndWait(29, 152, 472);
  Actor_SetAnimation(25, 1);
  Actor_SetAnimation(27, 1);
  Actor_ArmCallback(29, 0x5000, 0);
  Actor_ArmCallback(27, 0x5000, 0);
  Actor_ArmCallback(25, 0x5000, 0);
  FieldEvent_Wait(30);
  Actor_SetAnimationAndWait(25, 4);
  FieldEvent_ShowMessageThenWait(
      SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_ABOVE | 25, 0, 10);
  FieldActor_StartRepeatedMotion(4, 3);
  Actor_SetAttachedEffect(4, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldActor_StartRepeatedMotion(26, 3);
  Actor_SetAttachedEffect(26, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(40);
  Actor_WalkBy(27, 0, 64);
  Actor_WalkBy(29, 0, 64);
  Actor_WalkByAndWait(25, 0, 64);
  FieldActor_ShowEmote(0x19, 0x106, 0x28);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | 25, 0, 10);
  Actor_ArmCallback(26, 0x3000, 0);
  Actor_ArmCallback(4, FACING_SOUTHEAST, 0);
  FieldEvent_Wait(30);
  Actor_SetAnimationAndWait(25, 3);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | 25, 0, 10);
  Actor_WalkByAndWait(25, 0, 16);
  FieldEvent_Wait(10);
  Actor_RunRepeatedMotion(25, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(25, 0, 10);
  FieldActor_StartRepeatedMotion(30, 2);
  FieldActor_StartRepeatedMotion(31, 2);
  Actor_RunRepeatedMotion(28, 2);
  FieldEvent_Wait(20);
  Actor_SetPosition(28, 176, 592);
  FieldActor_SetSpriteRotationById(28, 0);
  Actor_SetSpriteFlags(Actor_GetById(28), 1);
  FieldActor_Jump(28, 6, 20);
  Actor_ArmCallback(28, FACING_WEST, 0);
  Actor_SetPosition(31, 128, 608);
  FieldActor_SetSpriteRotationById(31, 0);
  Actor_SetSpriteFlags(Actor_GetById(31), 1);
  FieldActor_Jump(31, 6, 20);
  Actor_ArmCallback(31, 0xd000, 0);
  Actor_SetPosition(30, 192, 616);
  FieldActor_SetSpriteRotationById(30, 0);
  Actor_SetSpriteFlags(Actor_GetById(30), 1);
  FieldActor_Jump(30, 6, 20);
  Actor_ArmCallback(30, 0xb000, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(25, 4);
  FieldEvent_ShowMessageThenWait(25, 0, 10);
  Actor_SetAnimation(30, 3);
  Actor_SetAnimation(31, 3);
  Actor_SetAnimationAndWait(28, 3);
  FieldEvent_Wait(30);
  FieldActor_WatchAndSetScript(4, 25);
  FieldActor_WatchAndSetScript(26, 25);
  FieldActor_WatchAndSetScript(30, 25);
  FieldActor_WatchAndSetScript(31, 25);
  FieldActor_WatchAndSetScript(28, 25);
  Actor_SetScript(25, (s32)gEasternAlhafra_RunOffScript);
  Actor_SetScript(27, (s32)gEasternAlhafra_RunOffScript);
  FieldEvent_Wait(10);
  Actor_SetScript(29, (s32)gEasternAlhafra_RunOffScript);
  FieldEvent_Wait(70);
  Actor_WalkByAndWait(28, -16, 0);
  Actor_SetScript(28, (s32)gEasternAlhafra_RunOffScript);
  FieldEvent_Wait(33);
  Actor_WalkByAndWait(31, 16, 0);
  Actor_SetScript(31, (s32)gEasternAlhafra_RunOffScript);
  FieldEvent_Wait(15);
  Actor_SetScript(30, (s32)gEasternAlhafra_RunOffScript);
  FieldActor_RefreshSelectorById(25);
  Actor_ArmCallback(26, 0x5000, 0);
  FieldActor_RefreshSelectorById(30);
  FieldEvent_Wait(20);
  EventCamera_FollowActor(4, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(40);
  FieldActor_ShowEmote(0x1a, 0x105, 0x32);
  Actor_ArmCallback(26, FACING_EAST, 0);
  Actor_ArmCallback(4, FACING_WEST, 0);
  FieldEvent_Wait(20);
  FieldMessage_ShowAtActor_(0x1a, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(4, 0) == 0) {
    FieldEvent_Wait(20);
    FieldEvent_ShowMessageThenWait(26, 0, 10);
    gFieldWork->message += 1;
  } else {
    FieldEvent_Wait(40);
    gFieldWork->message += 1;
    FieldEvent_ShowMessageThenWait(26, 0, 10);
  }
  Actor_SetSpeed(26, 0.8);
  Actor_WalkByAndWait(26, 0, -16);
  Actor_FaceDirection(4, FACING_NORTH, 0);
  Actor_ArmCallback(26, 0xd000, 0);
  FieldEvent_Wait(20);
  Actor_RunRepeatedMotion(26, 2);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(26, 0, 10);
  Actor_WalkByAndWait(26, 0, 16);
  Actor_FaceDirection(4, FACING_WEST, 0);
  Actor_ArmCallback(26, FACING_EAST, 0);
  FieldEvent_Wait(20);
  FieldMessage_ShowAtActor_(0x1a, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(4, 0) != 0) {
    FieldEvent_Wait(40);
    FieldEvent_ShowMessageThenWait(26, 0, 10);
    Actor_SetAnimationAndWait(26, 3);
    FieldEvent_Wait(10);
    FieldEvent_ShowMessageThenWait(26, 0, 10);
    gFieldWork->message += 2;
  } else {
    FieldEvent_Wait(20);
    gFieldWork->message += 2;
    FieldEvent_ShowMessageThenWait(26, 0, 10);
  }
  Actor_SetAnimationAndWait(4, 3);
  FieldEvent_Wait(20);
  Actor_SetSpeed(26, 1.2);
  Actor_SetAnimation(26, 2);
  {
    struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);

    if (leader != NULL)
      Actor_SetDestination(26, ((s16 *)leader)[5], ((s16 *)leader)[9]);
  }
  FieldActor_WaitForMoveById(26);
  Actor_SetPosition(26, 0, 0);
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
  struct RockWork *work = HeapSlot_Alloc(220, sizeof(struct RockWork));
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
  struct EasternAlhafraSparkWork *work = Ram_HeapSlots->menuRuntime;

  FieldTask_Remove(Particles_UpdateFalling);
  VramCache_FreeEntry(work->resource);
  HeapSlot_Release(220);
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
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
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
  x = actor->x + (a << 16);
  z = actor->z + (b << 16);
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

#include "../field/map/cellfill.c"

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
    dx = offset & 0xffff0000;
    dz = offset << 16;
    push->unk14 = 0;
    steps = 0;
    layer = &actor->terrainId;
    *layer = 2;
    push->x = actor->x + dx;
    push->z = actor->z + dz;
    cell.y = actor->y;
    for (;;) {
      cell.z = push->z + (gPushFootprints[kind][1] << 16);
      for (j = 0; j < height; j++) {
        cell.x = push->x + (gPushFootprints[kind][0] << 16);
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

      push->x = actor->x + move;
    }
    push->y = actor->y;
    push->z = actor->z + dz * steps;
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
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
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
  from[0] = object->x;
  from[2] = object->z;
  cell[0] = (object->x + (gPushFootprints[footprint][0] << 16)) >> 20;
  cell[2] = (object->z + (gPushFootprints[footprint][1] << 16)) >> 20;
  width = (ABS(gPushFootprints[footprint][0]) +
           ABS(gPushFootprints[footprint][2])) >>
          4;
  height = (ABS(gPushFootprints[footprint][1]) +
            ABS(gPushFootprints[footprint][3])) >>
           4;
  Map_FillCellRectByte2(0, cell[0], cell[2], width, height, 0);
  Actor_SetSpeedAndAcceleration(gMainRoutineWork.currentOwner, 0.5, 0.1);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 8);
  System_Wait(15);
  {
    s32 deltaX = (target.x - from[0]) >> 17;
    s32 deltaZ = (target.z - from[2]) >> 17;

    Actor_SetDestinationOffset(gMainRoutineWork.currentOwner, deltaX, deltaZ);
  }
  Actor_Get(gMainRoutineWork.currentOwner)->update = Actor_StopAtPushTarget;
  System_Wait(4);
  if ((u32)(direction - 6) <= 7)
    Actor_SelectAnimation(object, 3);
  else
    Actor_SelectAnimation(object, 2);
  Event_PlaySound(SE_HEAVY_MOVE);
  Actor_SetSpeedAndAcceleration(target.id, 0.5, 0.1);
  FieldActor_SetMoveTargetFixed(object, target.x, target.y, target.z);
  FieldActor_WaitForMoveById(gMainRoutineWork.currentOwner);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 2);
  Actor_SetSpeedAndAcceleration(gMainRoutineWork.currentOwner, 0.3, 0.1);
  Actor_SetDestinationOffset(gMainRoutineWork.currentOwner,
                             (gPushLeaderSteps[direction] << 16) >> 31,
                             (gPushLeaderSteps[direction] << 16) >> 17);
  if (target.callback != 0)
    target.callback();
  FieldActor_WaitForMoveById(gMainRoutineWork.currentOwner);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 1);
  leader->update = 0;
  leader->speedLimit = speed;
  leader->acceleration = acceleration;
  FieldActor_WaitForMove(object);
  Event_PlaySound(SE_298);
  Event_PlaySound(SE_HEAVY_STOP);
  FieldActor_SetPositionAndResetMotion(object, target.x, object->y, target.z);
  Actor_SelectAnimation(object, 1);
  target.x = target.x + (gPushFootprints[footprint][0] << 16);
  target.z = target.z + (gPushFootprints[footprint][1] << 16);
  target.x >>= 20;
  target.z >>= 20;
  originX = *(s32 *)(map + 328) >> 20;
  originZ = *(s32 *)(map + 332) >> 20;
  MapGrid_CopyCellAttributes(target.x, target.z, width, height,
                             originX + target.x, originZ + target.z);
  Map_FillCellRectByte2(0, target.x, target.z, width, height, 255);
  Map_FillCellRectByte2(2, target.x, target.z, width, height, 255);
  cell[0] = (from[0] + (gPushFootprints[footprint][0] << 16)) >> 20;
  cell[2] = (from[2] + (gPushFootprints[footprint][1] << 16)) >> 20;
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
  Actor_SetCellOpen(id, (u16)mode, (u16)flag, object->terrainId);
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
    SCENE_EXITS(EasternAlhafra1),
    SCENE_EXIT_UNLESS(1, Alhafra1, 11, EXIT_FLAG_NONE),
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

s16 gEasternAlhafra_PlacementsAfterBriggsCaught[] = {
    407, -1, 1, 0, 0, 280, 0, 0,
    0, 232, FACING_SOUTH, 2, 407, -1, 1, 0,
    0, 376, 0, 0, 0, 232, FACING_SOUTH, 258,
    287, -1, 1, 0, 0, 552, 0, 0,
    0, 520, FACING_SOUTH, 2, 287, -1, 1, 0,
    0, 408, 0, 0, 0, 520, FACING_SOUTH, 2,
    489, -1, 1, 0, 0, 344, 0, 0,
    0, 312, FACING_SOUTH, 2, 489, -1, 1, 0,
    0, 440, 0, 0, 0, 312, FACING_SOUTH, 2,
    406, -1, 1, 0, 0, 488, 0, 0,
    0, 312, FACING_SOUTH, 2, 258, -1, 7, 0,
    0, 336, 0, 0, 0, 456, FACING_SOUTH, 2,
    467, -1, 1, 0, 0, 344, 0, 0,
    0, 472, FACING_SOUTH, 2, 467, -1, 1, 0,
    0, 440, 0, 0, 0, 456, FACING_SOUTH, 2,
    489, -1, 7, 0, 0, 444, 0, 0,
    0, 312, FACING_SOUTH, 2, 489, -1, 7, 0,
    0, 482, 0, 0, 0, 312, FACING_SOUTH, 2,
    489, -1, 7, 0, 0, 444, 0, 0,
    0, 376, FACING_SOUTH, 2, 489, -1, 7, 0,
    0, 492, 0, 0, 0, 312, FACING_SOUTH, 2,
    16, -1, 10, 0, 0, -56, 0, 0,
    0, -56, FACING_SOUTH, 1, 37, -1, 10, 0,
    0, -56, 0, 0, 0, -56, FACING_SOUTH, 1,
    38, -1, 10, 0, 0, -56, 0, 0,
    0, -56, FACING_WEST, 1, 24, -1, 10, 0,
    0, -56, 0, 0, 0, -56, FACING_SOUTH, 2,
    5, -1, 1, 0, 0, 8, 0, 0,
    0, 8, FACING_NORTH, 2, 6, -1, 1, 0,
    0, 8, 0, 0, 0, 8, FACING_NORTH, 2,
    7, -1, 1, 0, 0, 8, 0, 0,
    0, 8, FACING_NORTH, 2, 56, -1, 1, 0,
    0, 8, 0, 0, 0, 8, FACING_NORTH, 2,
    112, -1, 10, 0, 0, -56, 0, 0,
    0, -56, FACING_SOUTH, 2, 195, -1, 10, 0,
    0, -56, 0, 0, 0, -56, FACING_SOUTH, 2,
    187, -1, 10, 0, 0, -56, 0, 0,
    0, -56, 0, 2, 115, -1, 10, 0,
    0, -56, 0, 0, 0, -56, FACING_SOUTH, 2,
    114, -1, 10, 0, 0, -56, 0, 0,
    0, -56, FACING_SOUTH, 2, -1, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
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
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 1046, FLAG_TALKED_TO_SAILOR, EVENT(EasternAlhafra_TalkToSailor)},
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
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 10, FLAG_FIRST_BLOCK_SUNK, EVENT(EasternAlhafra_StartPushingBlock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 10, FLAG_FIRST_BLOCK_SUNK, EVENT(EasternAlhafra_FinishPushingBlock)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 11, FLAG_SECOND_BLOCK_SUNK, EVENT(EasternAlhafra_StartPushingBlock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 11, FLAG_SECOND_BLOCK_SUNK, EVENT(EasternAlhafra_FinishPushingBlock)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 16, FLAG_LAST_CRATE_SUNK, EVENT(EasternAlhafra_StartPushingLastCrate)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 16, FLAG_LAST_CRATE_SUNK, EVENT(EasternAlhafra_FinishPushingLastCrate)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 17, FLAG_FIRST_CRATE_SUNK, EVENT(EasternAlhafra_StartPushingBlock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 17, FLAG_FIRST_CRATE_SUNK, EVENT(EasternAlhafra_FinishPushingBlock)},
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
    39, 2, 5, 0, 0, 0x80000,
    1, 5, 0x100000, 0, 0x100000, 1,
    5, 0x100000, 0, -0x100000, 1, 5,
    0, 0, -0x100000, 1, 39, 1,
    22, 3, 0x8000, 0, 1, 17,
};

s32 gEasternAlhafra_SecondWorkerCheerScript[] = {
    39, 2, 5, 0x200000, 0, 0x200000,
    1, 5, 0x100000, 0, -0x100000, 1,
    5, 0x20000, 0, -0x60000, 1, 39,
    1, 22, 3, 0x8000, 0, 1,
    17,
};

u32 gEasternAlhafra_FirstWorkerRunScript[] = {
    39, 2, 5, 0x300000, 0, 0,
    1, 4, 0xa80000, 0, 0x2480000, 1,
    39, 1, 22, 3, 0xe000, 0,
    1, 17,
};

u32 gEasternAlhafra_SecondWorkerRunScript[] = {
    39, 2, 5, 0x300000, 0, 0,
    1, 4, 0xb80000, 0, 0x2480000, 1,
    39, 1, 22, 3, 0xe000, 0,
    1, 17,
};

s32 gEasternAlhafra_RunOffScript[] = {
    39, 2, 4, 0x980000, 0, 0x2800000,
    1, 5, -0x500000, 0, 0, 1,
    5, -0x200000, 0, -0x200000, 1, 5,
    -0x180000, 0, 0, 1, 0, 1,
    17,
};
// clang-format on

u32 gEasternAlhafra_PuffScript[] = {
    38,
};
