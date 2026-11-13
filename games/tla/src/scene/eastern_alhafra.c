/// @file
/// The scene code of overlay 0x668, in ROM order.

// ---- games/tla/src/scene/eastern_alhafra.c ----

#include "shared/effects.h"
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

/// What games/tla/src/scene/ declares for the files that use it, written once.

#include "types.h"

struct Actor;
struct RockSprite;
struct Vector3;

struct Unk_02004240 {
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

void Func_02000fa4(void);
void Rocks_Spawn(s32, s32, s32, s32);

void Func_02004004(void);
void Func_020040c0(void);

void Func_020017ec(void);
void Func_02001cb0(void);
void Func_02003274(void);

extern s32 gData_020054f4[];
extern s32 gData_0200556c[];
extern u32 gData_0200d5d0[];
extern u32 gData_02005620[];
extern s32 gData_02005670[];

void Func_02000514(void);

void Func_02000730(void);

void Func_02000838(void);

void Func_02000954(void);
void Func_02000968(void);
void Func_02001574(void);
void Func_02002048(void);
void Func_02002a08(void);

extern s16 gData_02005284[];

void Func_02004c64(struct RockSprite *sprite, s32 width, s32 height,
                   u32 attributes, s32 tile);

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
s32 Func_02004c4c();
extern u16 gData_020056e0[2][160];
void Actor_FallOverWithAnimationById();
void Rope_Initialize(void);
void Tightrope_Initialize(void);
void MapGrid_SetAttributeBAt();

#include "module/scene/scene.h"
#include "module/tla.h"
#include "field/particle.h"

/* VIEW(eastern_alhafra): routine gData_02004d0c as this part declared it; the
 * joined file has another. */
extern u8 gData_02004d0c__eastern_alhafra[] __asm__("gData_02004d0c");
#define gData_02004d0c gData_02004d0c__eastern_alhafra
/* VIEW(eastern_alhafra): routine gData_02004dbc as this part declared it; the
 * joined file has another. */
extern u8 gData_02004dbc__eastern_alhafra[] __asm__("gData_02004dbc");
#define gData_02004dbc gData_02004dbc__eastern_alhafra
/* VIEW(eastern_alhafra): routine gData_02004fe4 as this part declared it; the
 * joined file has another. */
extern u8 gData_02004fe4__eastern_alhafra[] __asm__("gData_02004fe4");
#define gData_02004fe4 gData_02004fe4__eastern_alhafra

s32 Actor_HasArrived();

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

void *Scene_GetPlacements(void) {
  if (Event_GetFlag(FLAG_BRIGGS_CAUGHT))
    return gData_02004fe4;
  return gData_02004dbc;
}

void Func_0200006c(s32 mode, s32 id) {
  if (id == 0)
    Func_02004004();
  if (mode == 1) {
    Event_SetFlag(FLAG_ALHAFRA_MAST_CLEARED);
    Func_02000fa4();
  }
  if (mode == 2)
    Rocks_Spawn(0x1f20000, 0x100000, 0x19c0000, 2);
  if (mode == 3)
    Rocks_Spawn(0x1f20000, 0x100000, 0x19c0000, 30);
  if (id == 0x3e7)
    Func_020040c0();
}

void Func_020000c8(s32 mode, s32 arg) {
  struct Actor *a;
  struct Actor *b;
  s32 i;

  Statue_Sink(gData_02004d0c, mode, arg);
  if (mode == 1) {
    a = Actor_Get(19);
    b = Actor_Get(21);
    a->speedLimit = 0x40000;
    a->acceleration = 0x8000;
    b->acceleration = 0x8000;
    b->speedLimit = 0x40000;
    FieldActor_SetPositionAndResetMotion(a, 0x1e80000, a->y, 0x1380000);
    FieldActor_SetMoveTarget(a, 0x1b80000, 0, 0x1380000);
    FieldActor_SetPositionAndResetMotion(b, 0x1e80000, b->y, 0x1380000);
    FieldActor_SetMoveTarget(b, 0x1b80000, 0, 0x1780000);
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

void Func_02000190(s32 unused, s32 id) {
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
      FieldMapGrid_SetCellBehavior(0x2100000, 0x2000000, 2, 255);
    if (x == 33 && z == 32 && leaderX == 32)
      FieldMapGrid_SetCellBehavior(0x2000000, 0x2000000, 2, 255);
  }
}

#include "system/ram_buffer.h"
#include "event_work.h"
#include "text/msg_ids.h"

/* VIEW(eastern_alhafra): routine gData_02005290 as this part declared it; the
 * joined file has another. */
extern u8 gData_02005290__eastern_alhafra[] __asm__("gData_02005290");
#define gData_02005290 gData_02005290__eastern_alhafra

/* VIEW(eastern_alhafra): routine Actor_SelectAnimation as s32
 * Actor_SelectAnimation(); module/scene/scene.h has void
 * Actor_SelectAnimation(struct Actor *object, s32 mode); not tried:
 * Actor_SelectAnimation's bound declaration was not found. */
extern s32 Func_02004a0c__eastern_alhafra() __asm__("Actor_SelectAnimation");
s32 Party_FindOwnerWithAction();
/* VIEW(eastern_alhafra): routine Field_FindNearbyTalkTarget as s32
 * Field_FindNearbyTalkTarget(); module/scene/airs_rock_inside.h has void
 * Field_FindNearbyTalkTarget(void); not tried: Field_FindNearbyTalkTarget's
 * bound declaration was not found. */
extern s32
Func_02004bb4__eastern_alhafra() __asm__("Field_FindNearbyTalkTarget");
void Event_Start();
void Event_End();

void Func_02000200(s32 mode, s32 id) {
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
    FieldMapGrid_SetCellBehavior(0x2100000, 0x2000000, 2, 0);
    FieldMapGrid_SetCellBehavior(0x2000000, 0x2000000, 2, 0);
  }
  if (x == 31 && z == 32) {
    Event_SetFlag(0x8a0);
    hit = 1;
  }
  if (x == 25 && z == 33) {
    Event_SetFlag(0x8a1);
    hit = 1;
  }
  if (x == 21 && z == 33) {
    Event_SetFlag(0x8a4);
    hit = 1;
  }
  if (x == 27 && z == 33) {
    Event_SetFlag(0x8a5);
    hit = 1;
  }
  if (hit) {
    if (id != 10)
      Event_PlaySound(310);
    object->gravity = 0x170a;
    object->motionFlags = 3;
    Actor_SetSpriteFlags(Actor_Get(id), 1);
    System_Wait(5);
    for (i = 0; i < 60 && object->y > 0; i++)
      System_Wait(1);
    Event_PlaySound(298);
    System_Wait(1);
    Event_PlaySound(240);
    object->collisionFlags = 0;
    object->priorityFlags |= 2;
    Func_02004a0c__eastern_alhafra(object, 4);
    i = 0;
  create:
    spark = (FieldActor_Create(276, object->x, object->y, object->z));
    sparks[i] = spark;
    spark->scaleX = 0xa666;
    Func_02004a0c__eastern_alhafra(spark, 1);
    Actor_SetSpriteFlags(spark, 0);
    Event_PlaySound(154);
    System_Wait(5);
    i++;
    if (i <= 2)
      goto create;
    Func_02000fa4();
    FieldActor_Stop(gMainRoutineWork.currentOwner);
    System_Wait(30);
    for (i = 0; i <= 2; i++)
      Actor_Release(sparks[i]);
  }
}

#include "actor.h"

void Func_02000394(s32 unused, s32 id) {
  Func_02000190(unused, id);
  FieldMapGrid_SetCellBehavior(0x1400000, 0x1d00000, 2, 255);
  FieldMapGrid_SetCellBehavior(0x1400000, 0x1e00000, 2, 255);
  FieldMapGrid_SetCellBehavior(0x1400000, 0x1f00000, 2, 255);
  FieldMapGrid_SetCellBehavior(0x1400000, 0x2000000, 2, 255);
  FieldMapGrid_SetCellBehavior(0x1600000, 0x1d00000, 2, 255);
  FieldMapGrid_SetCellBehavior(0x1600000, 0x1e00000, 2, 255);
  FieldMapGrid_SetCellBehavior(0x1600000, 0x1f00000, 2, 255);
  FieldMapGrid_SetCellBehavior(0x1600000, 0x2000000, 2, 255);
  FieldMapGrid_SetCellBehavior(0x1500000, 0x2100000, 2, 0);
}

void Func_0200042c(s32 mode, s32 id) {
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
  FieldMapGrid_SetCellBehavior(0x1400000, 0x1d00000, 2, 0);
  FieldMapGrid_SetCellBehavior(0x1400000, 0x1e00000, 2, 0);
  FieldMapGrid_SetCellBehavior(0x1400000, 0x1f00000, 2, 0);
  FieldMapGrid_SetCellBehavior(0x1400000, 0x2000000, 2, 0);
  FieldMapGrid_SetCellBehavior(0x1600000, 0x1d00000, 2, 0);
  FieldMapGrid_SetCellBehavior(0x1600000, 0x1e00000, 2, 0);
  FieldMapGrid_SetCellBehavior(0x1600000, 0x1f00000, 2, 0);
  FieldMapGrid_SetCellBehavior(0x1600000, 0x2000000, 2, 0);
  FieldMapGrid_SetCellBehavior(0x1500000, 0x2100000, 2, 255);
  if (open)
    FieldCamera_MoveTo(PIXELS(336), -1, PIXELS(560), 1);
  Func_02000200(mode, id);
  if (open)
    EventCamera_FollowActor(gMainRoutineWork.currentOwner, 1);
}

// ---- games/tla/src/scene/eastern_alhafra/s0514.c ----
// Scene code of overlay 0x668, in ROM order, from s0514's offset up to the next
// object the overlay links.

#include "field/field.h"

void Func_0200112c(void);
/* VIEW(s0514): routine Func_020044c4 as void Func_020044c4(struct
 * Unk_02004240); eastern_alhafra has void Func_020044c4(struct DoorPlan); tried
 * as the definition's void Func_020044c4(struct PushTarget target), which the
 * object already declares, Func_02000514 moves: 1 of 198 instructions in
 * tla-ja. */
extern void Func_020044c4__s0514(struct Unk_02004240) __asm__("Func_020044c4");

void Func_02000514(void) {
  struct Actor *actor15;
  struct Actor *actor16;
  struct Unk_02004240 unk;
  s32 facing;
  s32 i;

  actor15 = Actor_GetById(15);
  actor16 = Actor_GetById(16);
  facing =
      (Actor_GetById(gMainRoutineWork.currentOwner)->facing + 0x2000) & 0xc000;
  if (actor16 != NULL && Event_GetFlag(0x8a4) == 0)
    MapGrid_SetCellBehavior(actor16->x, actor16->z, 2, 255);
  if (Func_02004240(&unk) != 0) {
    if (Event_GetFlag(0x8a4) == 0 || facing == 0xc000) {
      Func_020044c4__s0514(unk);
    } else {
      Event_SetFlag(0x8a6);
      Event_Start();
      FieldEvent_ResolvePendingActions(0);
      FieldCamera_MoveTo(PIXELS(336), -1, PIXELS(560), 1);
      Func_020044c4__s0514(unk);
      MapGrid_SetCellBehavior(actor15->x - PIXELS(24), actor15->z, 0, 0);
      MapGrid_SetCellBehavior(actor15->x + PIXELS(8), actor15->z, 0, 0);
      MapGrid_SetCellBehavior(actor15->x - PIXELS(24), actor15->z, 2, 0);
      MapGrid_SetCellBehavior(actor15->x - PIXELS(8), actor15->z, 2, 0);
      MapGrid_SetCellBehavior(actor15->x + PIXELS(8), actor15->z, 2, 0);
      MapGrid_SetCellBehavior(actor15->x + PIXELS(24), actor15->z, 2, 0);
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
      Func_02000fa4();
      Event_PlaySound(240);
      Actor_SetAnimation(15, 8);
      FieldEvent_Wait(60);
      EventCamera_FollowActor(gMainRoutineWork.currentOwner, 1);
      Event_End();
    }
  }
  if (actor16 != NULL && Event_GetFlag(0x8a4) == 0)
    MapGrid_SetCellBehavior(actor16->x, actor16->z, 2, 0);
}

void Func_020006ec(s32 mode) {
  s32 tile;

  tile = Func_02004bb4__eastern_alhafra(gMainRoutineWork.currentOwner);
  if (tile == 16) {
    Func_02000394(mode, 16);
    if (Leader_CanPushObjectAhead() != 0)
      Func_02004c4c();
    Func_0200042c(mode, 16);
  } else if (tile == 15) {
    Func_02000514();
  }
}

void Func_02000730(void) {
  Event_SetFlag(0x8a8);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId(MsgWhatSYour);
  Actor_ShowEmote(22, 0x101, 40);
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
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
    Event_OpenMessage(22, 0);
    if (FieldEvent_AskYesNoWithActorAnimation(4, 0) != 0) {
      FieldEvent_Wait(30);
      FieldEvent_ShowMessageThenWait(22, 0, 0);
    } else {
      FieldEvent_Wait(20);
      ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
      Event_OpenMessage(22, 0);
      if (FieldEvent_AskYesNoWithActorAnimation(4, 0) != 0) {
        FieldEvent_Wait(30);
        FieldEvent_ShowMessageThenWait(22, 0, 0);
      } else {
        FieldEvent_Wait(20);
        ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
        FieldEvent_ShowMessageThenWait(22, 0, 0);
      }
    }
  }
  Actor_FaceDirection(22, FACING_EAST, 0);
  Event_End();
}

void Func_02000838(void) {
  if (Event_GetFlag(FLAG_ALHAFRA_MAST_CLEARED)) {
    if (Event_GetFlag(0x904) == 0) {
      Event_SetFlag(0x904);
      Func_02001cb0();
    }
  } else if (Event_GetFlag(FLAG_BRIGGS_CAUGHT)) {
    if (Event_GetFlag(0x8a0) && Party_FindOwnerWithAction(151) == -1 &&
        Event_GetFlag(0x8ad) == 0) {
      Event_SetFlag(0x8ad);
      Func_020017ec();
    }
  } else if (Event_GetFlag(0x8a9) == 0) {
    Event_SetFlag(0x8a9);
    Func_0200112c();
  }
}

/// Stepping into a door: it opens, the leader walks through and the scene
/// leaves by the exit given.
void EasternAlhafra_OpenDoorAndEnter(struct DoorStep *step, s32 countdown,
                                     s32 unused) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Event_PlaySound(158);
  MapGrid_AnimateCells(step->script, step->x, step->z);
  Actor_Get(gMainRoutineWork.currentOwner)->motionFlags = 2;
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x8000, 0x4000);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 2);
  Actor_CenterAndWalk(gMainRoutineWork.currentOwner, 2, -8);
  FieldEvent_Wait(10);
  FieldEvent_SetExitRequest(countdown);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_End();
}

void Func_02000940(s32 countdown) {
  EasternAlhafra_OpenDoorAndEnter(gData_02005290, countdown, 0);
}

void Func_02000954(void) {}

void Func_02000958(void) { GameFlag_Set(0x8be); }

void Func_02000968(void) {
  struct Actor *leader;

  goto check;
run:
  if (Party_FindOwnerWithAction(151) != -1)
    return;
  Event_SetFlag(0x8bf);
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
      Event_GetFlag(FLAG_ALHAFRA_MAST_CLEARED) == 0 && Event_GetFlag(0x8be))
    goto run;
}

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }

#include "system/io_reg.h"
#include "system/dma.h"
#include "map.h"
#include "system/frame.h"

/* VIEW(s0514): routine gData_020054f0 as this part declared it; the joined file
 * has another. */
extern u8 gData_020054f0__s0514[] __asm__("gData_020054f0");
#define gData_020054f0 gData_020054f0__s0514

// BG1's wave: each frame the finished table of scanline offsets is handed to
// an HBlank DMA, while the other table is filled for the next frame.

/// Starts DMA 0 copying the finished table into BG1's horizontal offset at
/// each HBlank.
// With ☀️'s way of stopping DMA 0.
void Func_02000a44(void) {
  u16 *line = gData_020056e0[gData_020054f0[0] ^ 1];
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
void Func_02000a98(void) {
  struct MapLayer *layer = &Ram_HeapSlots->mapWork->layers[2];
  u16 *line;
  s32 i = 0;

  line = gData_020056e0[gData_020054f0[0] ^ 1];
  for (i = 0; i != 160; i++) {
    *line++ =
        (layer->y >> 16) +
        (Math_Sin(((u8)gFrameCount - i - (layer->y >> 16) + 160) << 9) >> 15);
  }
  gData_020054f0[0] ^= 1;
}

void Func_02000af8(void) {
  Task_AddFrame(Func_02000a98, 0x480);
  Task_AddFrame(Func_02000a44, 0xc80);
}

#include "call.h"

#define EVENT_WORK ((u8 *)Ram_HeapSlots->eventWork)
#define OBJECT(id) (Actor_Get((id)))

/* VIEW(eastern_alhafra): routine Rope_Place as void Rope_Place();
 * module/scene/mars_lighthouse_aerie.h has void Rope_Place(void); not tried:
 * Rope_Place's bound declaration was not found. */
extern void Func_02004c2c__eastern_alhafra() __asm__("Rope_Place");
void Func_020047fc();
/* VIEW(eastern_alhafra): routine Tightrope_Settle as void Tightrope_Settle();
 * module/scene/mars_lighthouse_aerie.h has void Tightrope_Settle(void); not
 * tried: Tightrope_Settle's bound declaration was not found. */
extern void Func_02004c94__eastern_alhafra() __asm__("Tightrope_Settle");

s32 Scene_Initialize(void) {
  struct Actor *object;
  s32 flag;

  *(s32 *)(EVENT_WORK + 0x1ac) = 0x100;
  Func_02000af8();
  OBJECT(gMainRoutineWork.currentOwner)->priorityFlags |= 32;
  if (Event_GetFlag(FLAG_ALHAFRA_MAST_CLEARED) != 0) {
    if (Event_GetFlag(0x905) != 0 &&
        Event_GetFlag(FLAG_ALHAFRA_SHIP_GONE) == 0) {
      Actor_SetPosition(30, PIXELS(184), PIXELS(616));
      Actor_SetPosition(31, PIXELS(136), PIXELS(600));
      Actor_SetPosition(28, PIXELS(168), PIXELS(584));
      Actor_FaceDirection(28, FACING_SOUTH, 0);
      Actor_FallOverWithAnimationById(30, 0, 13);
      Call3(Actor_FallOverWithAnimationById, 31, 0x8000, 13);
      Actor_FallOverWithAnimationById(28, 0, 13);
      Actor_SetPosition(25, PIXELS(112), PIXELS(432));
      Actor_SetPosition(27, PIXELS(176), PIXELS(408));
      Actor_SetPosition(29, PIXELS(152), PIXELS(376));
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
    if (Event_GetFlag(0x905) != 0) {
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
    Actor_SetPosition(4, PIXELS(136), PIXELS(552));
    Actor_SetPosition(26, PIXELS(120), PIXELS(552));
    Actor_SetPosition(30, PIXELS(184), PIXELS(616));
    Actor_SetPosition(31, PIXELS(136), PIXELS(600));
    Actor_SetPosition(28, PIXELS(160), PIXELS(584));
    FieldActor_SetSpriteRotationById(30, 0x4000);
    Actor_SetSpriteFlags(Actor_Get(30), 0);
    FieldActor_SetSpriteRotationById(31, 0xc000);
    Actor_SetSpriteFlags(Actor_Get(31), 0);
    FieldActor_SetSpriteRotationById(28, 0x4000);
    Actor_SetSpriteFlags(Actor_Get(28), 0);
    FieldActor_SetSpritePriority(27, 2);
    FieldActor_SetSpritePriority(25, 2);
    FieldActor_SetSpritePriority(29, 2);
    Actor_SetPosition(25, PIXELS(160), PIXELS(392));
    Actor_SetPosition(27, PIXELS(176), PIXELS(408));
    Actor_SetPosition(29, PIXELS(152), PIXELS(376));
    FieldActor_SetSpriteRotationById(27, 0xc000);
    Actor_SetSpriteFlags(Actor_Get(27), 0);
    FieldActor_SetSpriteRotationById(29, 0x4000);
    Actor_SetSpriteFlags(Actor_Get(29), 0);
    Actor_FaceDirection(25, 0xd000, 0);
    Actor_FaceDirection(29, FACING_SOUTH, 0);
    Actor_SetAnimation(27, 13);
    Actor_SetAnimation(29, 13);
    Actor_SetAnimation(30, 13);
    Actor_SetAnimation(31, 13);
    Actor_SetAnimation(28, 13);
    Func_02003274();
  }
  Func_02000fa4();
  if (Event_GetFlag(0x8a6) == 0)
    Func_020040ec(15);
  Rope_Initialize();
  Func_02004c2c__eastern_alhafra(0, 0x8a3, 8, 9);
  OBJECT(8)->priorityFlags |= 32;
  OBJECT(9)->priorityFlags |= 32;
  Tightrope_Initialize();
  Tightrope_Place(0, 12, 13);
  Actor_SetSpriteFlags(Actor_Get(12), 0);
  Actor_SetSpriteFlags(Actor_Get(13), 0);
  Func_020047fc(gData_02004d0c);
  Actor_SetSpriteFlags(Actor_Get(14), 0);
  flag = Event_GetFlag(0x8a2);
  if (flag == 0) {
    object = OBJECT(18);
    /* FAKEMATCH(dead): the solid bit set in a once-only block, which lays out
     * the branch over it as the native does; as the plain statement, 18 of 467
     * instructions differ in tla-en. */
    do {
      object->collisionFlags |= 8;
    } while (0);
    object = OBJECT(19);
    object->motionFlags = flag;
    object->y = 0x100000;
    Tightrope_Place(1, 18, 19);
    Func_02004c94__eastern_alhafra(1);
    object = OBJECT(20);
    object->collisionFlags |= 8;
    object = OBJECT(21);
    object->motionFlags = flag;
    object->y = 0x100000;
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

void Func_02000fa4(void) {
  s32 t1, t2, t8, t10, t12, t13, t18, t19, t24, v_r5;

  t1 = Event_GetFlag(0x8a6);
  if (t1 == 0)
    goto L_02009022;
  t2 = ((s32)Actor_Get(15));
  v_r5 = t2;
  Actor_SetPosition(15, PIXELS(336), PIXELS(536));
  Actor_SetAnimation(15, 4);
  FIELD_AT(v_r5, u8, 35) = (2 | FIELD_AT(v_r5, u8, 35));
  FIELD_AT(v_r5, u8, 85) = 2;
  FIELD_AT(v_r5, s32, 40) = 0;
  FIELD_AT(v_r5, s32, 12) = -0x40000;
  FIELD_AT(v_r5, s32, 20) = -0x40000;
  FieldMapGrid_SetCellBehavior(0x1500000, 0x2100000, 2, 0);
  FieldMapGrid_SetCellBehavior(0x1500000, 0x2100000, 0, 0);
  FieldMapGrid_SetCellBehavior(0x1600000, 0x2100000, 0, 0);
L_02009022:
  t8 = Event_GetFlag(0x8a4);
  if (t8 == 0)
    goto L_0200903a;
  Actor_SetPosition(16, 0, 0);
L_0200903a:
  t10 = Event_GetFlag(0x8a5);
  if (t10 == 0)
    goto L_02009052;
  Actor_SetPosition(17, 0, 0);
L_02009052:
  t12 = Event_GetFlag(0x8a0);
  if (t12 == 0)
    goto L_020090a8;
  t13 = ((s32)Actor_Get(10));
  v_r5 = t13;
  Actor_SetPosition(10, PIXELS(504), PIXELS(520));
  Actor_SetAnimation(10, 2);
  FIELD_AT(v_r5, u8, 89) = 0;
  /* FAKEMATCH(dead): an empty once-only block ends the scheduling block here,
   * as the listing orders it; without it, 4 of 168 instructions differ in
   * tla-en. */
  do {
  } while (0);
  FIELD_AT(v_r5, u8, 35) = (2 | FIELD_AT(v_r5, u8, 35));
  FieldMapGrid_SetCellBehavior(FIELD_AT(v_r5, s32, 8), FIELD_AT(v_r5, s32, 16),
                               0, 0);
  MapGrid_SetAttributeBAt(FIELD_AT(v_r5, s32, 8), FIELD_AT(v_r5, s32, 16), 0,
                          128);
L_020090a8:
  t18 = Event_GetFlag(0x8a1);
  if (t18 == 0)
    goto L_02009100;
  t19 = ((s32)Actor_Get(11));
  v_r5 = t19;
  Actor_SetPosition(11, PIXELS(408), PIXELS(536));
  Actor_SetAnimation(11, 2);
  FIELD_AT(v_r5, u8, 89) = 0;
  /* FAKEMATCH(dead): an empty once-only block ends the scheduling block here,
   * as the listing orders it; without it, 4 of 168 instructions differ in
   * tla-en. */
  do {
  } while (0);
  FIELD_AT(v_r5, u8, 35) = (2 | FIELD_AT(v_r5, u8, 35));
  FieldMapGrid_SetCellBehavior(FIELD_AT(v_r5, s32, 8), FIELD_AT(v_r5, s32, 16),
                               0, 0);
  MapGrid_SetAttributeBAt(FIELD_AT(v_r5, s32, 8), FIELD_AT(v_r5, s32, 16), 0,
                          128);
L_02009100:
  t24 = Event_GetFlag(FLAG_ALHAFRA_MAST_CLEARED);
  if (t24 == 0)
    goto L_02009122;
  MapGrid_CopyCells(32, 24, 2, 2, 30, 24);
L_02009122:
}

#include "field_sprite.h"

void Func_0200112c(void) {
  struct Actor *object0;
  struct Actor *object1;
  struct Actor *object2;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgItSIt);
  FieldCamera_SetSpeed(0x40000, 0x8000);
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
  Actor_FaceDirection(25, 0x5000, 0);
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
  Actor_FaceDirection(23, 0xd000, 0);
  FieldEvent_Wait(20);
  EventCamera_MoveToActor(14, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(25, 0, 10);
  EventCamera_MoveToActor(22, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(10);
  Actor_ShowEmote(22, 0x107, 40);
  FieldEvent_ShowMessageThenWait(22, 0, 10);
  FieldCamera_MoveTo(PIXELS(488), 0, PIXELS(416), 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(20);
  Actor_ShowEmote(23, 0x102, 50);
  Actor_FaceDirection(23, FACING_EAST, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(23, 0, 10);
  Actor_FaceDirection(24, FACING_EAST, 0);
  Actor_FaceDirection(25, FACING_SOUTHEAST, 0);
  FieldEvent_Wait(20);
  FieldActor_StartRepeatedMotion(25, 3);
  Actor_SetAttachedEffect(25, 0x102);
  FieldEvent_Wait(40);
  FieldEvent_ShowMessageThenWait(25, 0, 10);
  FieldCamera_MoveTo(PIXELS(392), PIXELS(16), PIXELS(416), 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(24, 4);
  FieldEvent_ShowMessageThenWait(24, 0, 10);
  Actor_FaceDirection(23, FACING_WEST, 0);
  FieldEvent_Wait(30);
  Actor_ShowEmote(22, 0x105, 0);
  Actor_ShowEmote(23, 0x105, 0);
  Actor_ShowEmote(25, 0x105, 50);
  FieldEvent_Wait(20);
  Actor_RunRepeatedMotion(25, 2);
  FieldEvent_Wait(10);
  Actor_FaceDirection(25, 0x5000, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(25, 0, 20);
  Actor_SetAnimation(22, 3);
  Actor_SetAnimation(23, 3);
  Actor_SetAnimationAndWait(24, 3);
  FieldEvent_Wait(20);
  Actor_FaceDirection(22, FACING_EAST, 0);
  Actor_FaceDirection(23, 0x5000, 0);
  Actor_FaceDirection(24, FACING_NORTH, 0);
  Actor_FaceDirection(25, FACING_NORTHEAST, 0);
  FieldEvent_Wait(20);
  EventCamera_MoveToActor(gMainRoutineWork.currentOwner, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(20);
  Actor_SetSpeed(5, 0x13333, 0x9999);
  Actor_SetAnimation(5, 2);
  object0 = Actor_GetById(gMainRoutineWork.currentOwner);
  if (object0 == 0)
    goto L_020094c6;
  Actor_SetDestination(5, *(s16 *)((u8 *)object0 + 10),
                       *(s16 *)((u8 *)object0 + 18));
L_020094c6:
  FieldActor_WaitForMoveById(5);
  Actor_SetPosition(5, 0, 0);
  Actor_SetSpeed(6, 0x13333, 0x9999);
  Actor_SetAnimation(6, 2);
  object1 = Actor_GetById(gMainRoutineWork.currentOwner);
  if (object1 == 0)
    goto L_02009504;
  Actor_SetDestination(6, *(s16 *)((u8 *)object1 + 10),
                       *(s16 *)((u8 *)object1 + 18));
L_02009504:
  FieldActor_WaitForMoveById(6);
  Actor_SetPosition(6, 0, 0);
  Actor_SetSpeed(26, 0x13333, 0x9999);
  Actor_SetAnimation(26, 2);
  object2 = Actor_GetById(gMainRoutineWork.currentOwner);
  if (object2 == 0)
    goto L_02009550;
  Actor_SetDestination(26, *(s16 *)((u8 *)object2 + 10),
                       *(s16 *)((u8 *)object2 + 18));
L_02009550:
  FieldActor_WaitForMoveById(26);
  Actor_SetPosition(26, 0, 0);
  EventCamera_FollowActor(gMainRoutineWork.currentOwner, 1);
  FieldCamera_WaitForArrival();
  Event_End();
  return;
}

void Func_02001574(void) {
  s32 result0;
  s32 result1;
  s32 result2;
  s32 result3;
  struct Actor *object4;
  struct Actor *object5;
  struct Actor *object6;
  struct Actor *object7;

  result0 = Event_GetFlag(FLAG_BRIGGS_CAUGHT);
  if (result0 != 0)
    goto L_02009586;
  goto L_020097dc;
L_02009586:
  result1 = Event_GetFlag(FLAG_ALHAFRA_MAST_CLEARED);
  if (result1 == 0)
    goto L_02009596;
  goto L_020097dc;
L_02009596:
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgYouDoPlan);
  Event_SetFlag(0x8ac);
  Actor_WalkToAndWait(4, 264, 448);
  FieldActor_LaunchFromLeader(26, 0, -16, 0x4000);
  FieldActor_LaunchFromLeader(5, 16, 0, 0x8000);
  FieldActor_LaunchFromLeader(6, 16, -16, 0x6000);
  result2 = Event_GetFlag(7);
  if (result2 == 0)
    goto L_02009604;
  FieldActor_LaunchFromLeader(7, 8, 16, 0xc000);
L_02009604:
  FieldActor_WaitForMoveById(6);
  Actor_FaceDirection(4, FACING_EAST, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(26, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(26, 0, 10);
  Actor_FaceDirection(4, FACING_NORTH, 0);
  FieldEvent_Wait(20);
  Actor_ShowEmote(6, 0x102, 30);
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
  Actor_ShowEmote(26, 0x106, 30);
  FieldEvent_ShowMessageThenWait(26, 0, 10);
  result3 = Event_GetFlag(7);
  if (result3 == 0)
    goto L_02009716;
  Actor_SetSpeed(7, 0x13333, 0x9999);
  Actor_SetAnimation(7, 2);
  object4 = Actor_GetById(gMainRoutineWork.currentOwner);
  if (object4 == 0)
    goto L_02009706;
  Actor_SetDestination(7, *(s16 *)((u8 *)object4 + 10),
                       *(s16 *)((u8 *)object4 + 18));
L_02009706:
  FieldActor_WaitForMoveById(7);
  Actor_SetPosition(7, 0, 0);
L_02009716:
  Actor_SetSpeed(5, 0x13333, 0x9999);
  Actor_SetAnimation(5, 2);
  object5 = Actor_GetById(gMainRoutineWork.currentOwner);
  if (object5 == 0)
    goto L_0200974c;
  Actor_SetDestination(5, *(s16 *)((u8 *)object5 + 10),
                       *(s16 *)((u8 *)object5 + 18));
L_0200974c:
  FieldActor_WaitForMoveById(5);
  Actor_SetPosition(5, 0, 0);
  Actor_SetSpeed(6, 0x13333, 0x9999);
  Actor_SetAnimation(6, 2);
  object6 = Actor_GetById(gMainRoutineWork.currentOwner);
  if (object6 == 0)
    goto L_0200978a;
  Actor_SetDestination(6, *(s16 *)((u8 *)object6 + 10),
                       *(s16 *)((u8 *)object6 + 18));
L_0200978a:
  FieldActor_WaitForMoveById(6);
  Actor_SetPosition(6, 0, 0);
  Actor_SetSpeed(26, 0x13333, 0x9999);
  Actor_SetAnimation(26, 2);
  object7 = Actor_GetById(gMainRoutineWork.currentOwner);
  if (object7 == 0)
    goto L_020097c8;
  Actor_SetDestination(26, *(s16 *)((u8 *)object7 + 10),
                       *(s16 *)((u8 *)object7 + 18));
L_020097c8:
  FieldActor_WaitForMoveById(26);
  Actor_SetPosition(26, 0, 0);
  Event_End();
L_020097dc:
  return;
}

void Func_020017ec(void) {
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
  result0 = Event_GetFlag(7);
  if (result0 == 0)
    goto L_02009850;
  FieldActor_LaunchFromLeader(7, 8, 16, 0xc000);
L_02009850:
  FieldActor_WaitForMoveById(6);
  Actor_FaceDirection(4, 0xc000, 0);
  FieldEvent_Wait(20);
  Actor_RunRepeatedMotion(26, 2);
  FieldEvent_Wait(10);
  Event_OpenMessage(26, 0);
  result1 = FieldEvent_AskYesNoWithActorAnimation(4, 0);
  if (result1 != 0)
    goto L_020098ea;
  FieldEvent_Wait(20);
  Actor_ShowEmote(6, 0x100, 30);
  FieldEvent_ShowMessageThenWait(6, 0, 10);
  Actor_FaceDirection(4, FACING_NORTHEAST, 0);
  Actor_FaceDirection(5, FACING_NORTH, 0);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(5, 4);
  FieldEvent_ShowMessageThenWait(5, 0, 10);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 2;
  goto L_02009938;
L_020098ea:
  ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 2;
  FieldEvent_Wait(40);
  Actor_SetAnimationAndWait(6, 3);
  Actor_FaceDirection(4, FACING_NORTHEAST, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(6, 0, 10);
  Actor_SetAnimationAndWait(5, 4);
  FieldEvent_ShowMessageThenWait(5, 0, 10);
L_02009938:
  result2 = ((s32 (*)())GameFlag_Get)(7);
  if (result2 == 0)
    goto L_0200996c;
  Actor_FaceDirection(4, FACING_NORTH, 0);
  Actor_FaceDirection(5, FACING_NORTHWEST, 0);
  Actor_FaceDirectionAndWait(6, 0x8000);
  FieldEvent_Wait(10);
  goto L_02009a2e;
L_0200996c:
  ((s32 (*)())Actor_ShowEmoteById)(26, 0x102, 30);
  FieldEvent_ShowMessageThenWait(26, 0, 10);
  Actor_FaceDirection(4, FACING_NORTH, 0);
  Actor_FaceDirection(5, FACING_NORTHWEST, 0);
  Actor_FaceDirection(6, 0x8000, 0);
  FieldEvent_Wait(30);
  Actor_ShowEmote(26, 0x101, 30);
  Actor_SetAnimationAndWait(26, 4);
  FieldEvent_ShowMessageThenWait(26, 0, 10);
  ((s32 (*)())Actor_ShowEmoteById)(26, 0x109, 30);
  FieldEvent_ShowMessageThenWait(26, 0, 10);
  Actor_FaceDirection(5, FACING_NORTH, 0);
  Actor_FaceDirection(6, 0x4000, 0);
  FieldEvent_Wait(40);
  Actor_FaceDirection(5, FACING_NORTHWEST, 0);
  Actor_FaceDirection(6, 0x8000, 0);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(26, 4);
  FieldEvent_ShowMessageThenWait(26, 0, 10);
L_02009a2e:
  FieldEvent_SetMessageId((s32)MsgSoIThink);
  Actor_SetAnimationAndWait(26, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(26, 0, 10);
  Actor_SetAnimation(7, 3);
  Actor_SetAnimation(5, 3);
  Actor_SetAnimation(6, 3);
  Actor_SetAnimationAndWait(4, 3);
  FieldEvent_Wait(10);
  result3 = Event_GetFlag(7);
  if (result3 == 0)
    goto L_02009ac2;
  Actor_SetSpeed(7, 0x13333, 0x9999);
  Actor_SetAnimation(7, 2);
  object4 = Actor_GetById(gMainRoutineWork.currentOwner);
  if (object4 == 0)
    goto L_02009ab2;
  Actor_SetDestination(7, *(s16 *)((u8 *)object4 + 10),
                       *(s16 *)((u8 *)object4 + 18));
L_02009ab2:
  FieldActor_WaitForMoveById(7);
  Actor_SetPosition(7, 0, 0);
L_02009ac2:
  Actor_SetSpeed(5, 0x13333, 0x9999);
  Actor_SetAnimation(5, 2);
  object5 = Actor_GetById(gMainRoutineWork.currentOwner);
  if (object5 == 0)
    goto L_02009af8;
  Actor_SetDestination(5, *(s16 *)((u8 *)object5 + 10),
                       *(s16 *)((u8 *)object5 + 18));
L_02009af8:
  FieldActor_WaitForMoveById(5);
  Actor_SetPosition(5, 0, 0);
  Actor_SetSpeed(6, 0x13333, 0x9999);
  Actor_SetAnimation(6, 2);
  object6 = Actor_GetById(gMainRoutineWork.currentOwner);
  if (object6 == 0)
    goto L_02009b36;
  Actor_SetDestination(6, *(s16 *)((u8 *)object6 + 10),
                       *(s16 *)((u8 *)object6 + 18));
L_02009b36:
  FieldActor_WaitForMoveById(6);
  Actor_SetPosition(6, 0, 0);
  Actor_SetSpeed(26, 0x13333, 0x9999);
  Actor_SetAnimation(26, 2);
  object7 = Actor_GetById(gMainRoutineWork.currentOwner);
  if (object7 == 0)
    goto L_02009b74;
  Actor_SetDestination(26, *(s16 *)((u8 *)object7 + 10),
                       *(s16 *)((u8 *)object7 + 18));
L_02009b74:
  FieldActor_WaitForMoveById(26);
  Actor_SetPosition(26, 0, 0);
  Event_End();
  return;
}

/* VIEW(s0514): routine gData_020056d4 as this part declared it; the joined file
 * has another. */
extern const u8 gData_020056d4__s0514[] __asm__("gData_020056d4");
#define gData_020056d4 gData_020056d4__s0514

void Actor_StartScript(struct SpawnObject *object, const void *script);
void Actor_SetSpritePriorityDirect(struct Actor *object, s32 priority);

/// Spawns object type 0x1e8 at x, y, z with its sprite turned to value and
/// its timer set.
void Func_02001b9c(s32 x, s32 y, s32 z, s16 value, s32 timer) {
  struct SpawnObject *object;
  struct FieldSprite *sprite;

  object = ((struct SpawnObject *)FieldActor_Create(0x1e8, x, y, z));
  if (object != 0) {
    sprite = object->sprite;
    Func_02004a0c__eastern_alhafra(object, 1);
    Actor_StartScript(object, gData_020056d4);
    Actor_SetSpritePriorityDirect(object, 1);
    object->motionFlags = 0;
    {
      /* FAKEMATCH(signature): the timer copied into an s64 local, so it loads
       * before the pooled zero for nextFrame; as object->unk5e = timer, 2 of 35
       * instructions differ in tla-en. */
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

// ---- games/tla/src/scene/eastern_alhafra/s1bf4.c ----
// Scene code of overlay 0x668, in ROM order, from s1bf4's offset up to the next
// object the overlay links.

/* VIEW(s1bf4): routine Func_02001b9c as s32 Func_02001b9c(s32 x, s32 y, s32 z,
 * s32 rotation, s32 timer); the definition has void Func_02001b9c(s32 x, s32 y,
 * s32 z, s16 value, s32 timer); tried as the definition's void
 * Func_02001b9c(s32 x, s32 y, s32 z, s16 value, s32 timer), which the object
 * already declares, Func_02001bf4 moves: 2 of 79 instructions in tla-ja. */
extern s32 Func_02001b9c__s1bf4(s32 x, s32 y, s32 z, s32 rotation,
                                s32 timer) __asm__("Func_02001b9c");

void Func_02001bf4(s32 id) {
  struct Actor *object = Actor_Get(id);

  Func_02001b9c__s1bf4(object->x + 0x80000, object->y + 0x1c0000, object->z,
                       0x1000, 15);
  Event_PlaySound(151);
  FieldEvent_Wait(15);
  Func_02001b9c__s1bf4(object->x - 0x80000, object->y + 0x1c0000, object->z,
                       0xf000, 15);
  Event_PlaySound(151);
  FieldEvent_Wait(15);
  Func_02001b9c__s1bf4(object->x + 0x80000, object->y + 0x1c0000, object->z,
                       0x1000, 15);
  Event_PlaySound(151);
  FieldEvent_Wait(15);
  Func_02001b9c__s1bf4(object->x - 0x80000, object->y + 0x1c0000, object->z,
                       0xf000, 15);
  Event_PlaySound(151);
  FieldEvent_Wait(15);
}

#include "scene_ids.h"

static __inline__ void Func_02004b6c_(s32 actor, s32 a, s32 b) {
  Actor_ShowEmoteById(actor, a, b);
}

static __inline__ void FieldMessage_ShowAtActor_(s32 actor, s32 flags) {
  s32 f = flags;

  FieldMessage_ShowAtActor(actor, f);
}

/// The scene that opens with MsgHeyThatBlock: either answer to the prompt
/// steps the event to its next message.
void Func_02001cb0(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgHeyThatBlock);
  Actor_SetSpeed(28, 0x2cccc, 0x16666);
  Actor_SetSpeed(29, 0x2cccc, 0x16666);
  Actor_SetSpeed(23, 0x1cccc, 0xe666);
  Actor_SetSpeed(24, 0x1cccc, 0xe666);
  Actor_SetPosition(28, PIXELS(328), PIXELS(616));
  Actor_FaceDirection(28, FACING_NORTHEAST, 0);
  Actor_SetPosition(29, PIXELS(296), PIXELS(616));
  Actor_FaceDirection(29, FACING_NORTHEAST, 0);
  FieldEvent_ShowMessageThenWait(0x1c, 0, 0xa);
  Actor_FaceDirection(4, FACING_SOUTH, 0);
  {
    s32 mode = 1;

    FieldCamera_MoveTo(PIXELS(328), -1, PIXELS(632), mode);
  }
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(20);
  FieldActor_StartRepeatedMotion(28, 3);
  Actor_SetAttachedEffect(0x1c, 0x102);
  FieldEvent_Wait(50);
  Func_02004b6c_(0x1d, 0x102, 0x1e);
  FieldEvent_ShowMessageThenWait(0x1c, 0, 0xa);
  Actor_FaceDirection(28, FACING_WEST, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(28, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0x1c, 0, 0xa);
  Actor_FaceDirection(29, FACING_EAST, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(29, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0x1c, 0, 0xa);
  Actor_FaceDirection(29, FACING_WEST, 0);
  FieldEvent_Wait(10);
  Actor_WalkBy(28, -160, 0);
  Actor_WalkByAndWait(29, -160, 0);
  Actor_SetPosition(28, 0, 0);
  Actor_SetPosition(29, 0, 0);
  FieldEvent_Wait(30);
  Actor_SetPosition(23, PIXELS(264), PIXELS(384));
  Actor_FaceDirection(23, FACING_EAST, 0);
  Actor_SetPosition(24, PIXELS(280), PIXELS(392));
  Actor_FaceDirection(24, FACING_EAST, 0);
  Actor_SetPosition(4, PIXELS(232), PIXELS(384));
  Actor_FaceDirection(4, FACING_EAST, 0);
  FieldEvent_ShowMessageThenWait(0x18, 0, 0xa);
  FieldEvent_ShowMessageThenWait(0x17, 0, 0xa);
  FieldEvent_ShowMessageThenWait(0x18, 0, 0xa);
  FieldEvent_ShowMessageThenWait(0x17, 0, 0xa);
  FieldCamera_MoveTo(PIXELS(264), -1, PIXELS(392), 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(20);
  Actor_WalkByAndWait(23, -16, 0);
  FieldEvent_Wait(20);
  FieldMessage_ShowAtActor_(0x17, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(4, 0) == 0) {
    FieldEvent_Wait(20);
    Actor_SetAnimationAndWait(23, 3);
    FieldEvent_Wait(10);
    FieldEvent_ShowMessageThenWait(0x17, 0, 0xa);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
  } else {
    FieldEvent_Wait(40);
    Actor_SetAnimationAndWait(23, 4);
    FieldEvent_Wait(10);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
    FieldEvent_ShowMessageThenWait(0x17, 0, 0xa);
  }
  Actor_FaceDirection(23, FACING_EAST, 0);
  FieldEvent_Wait(20);
  Actor_FaceDirection(24, FACING_WEST, 0);
  FieldEvent_Wait(30);
  FieldEvent_ShowMessageThenWait(0x17, 0, 0xa);
  Func_02004b6c_(0x18, 0x108, 0x2d);
  FieldActor_Jump(24, 6, 15);
  FieldActor_Jump(24, 6, 23);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(23, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0x17, 0, 0xa);
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

/// Only once flag 0x8a7 is set: the scene that opens with MsgIStillJust, which
/// sets flag 0x905 and ends by sending the party to SceneId_Ov667, entrance 81.
void Func_02002048(void) {
  if (Event_GetFlag(FLAG_ALHAFRA_MAST_CLEARED) == 0)
    return;
  Event_SetFlag(0x905);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgIStillJust);
  FieldCamera_SetSpeed(0x40000, 0x8000);
  Actor_SetPosition(25, PIXELS(56), PIXELS(632));
  Actor_FaceDirection(25, FACING_EAST, 0);
  Actor_SetPosition(27, PIXELS(56), PIXELS(648));
  Actor_FaceDirection(27, FACING_EAST, 0);
  Actor_SetPosition(28, PIXELS(48), PIXELS(616));
  Actor_FaceDirection(28, FACING_EAST, 0);
  Actor_SetPosition(29, PIXELS(32), PIXELS(608));
  Actor_FaceDirection(29, FACING_EAST, 0);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0x19, 0, 0xa);
  FieldCamera_MoveTo(PIXELS(144), -1, PIXELS(664), 1);
  FieldEvent_Wait(10);
  Actor_SetSpeed(28, 0x10000, 0x8000);
  Actor_SetSpeed(29, 0x10000, 0x8000);
  Actor_SetSpeed(25, 0x10000, 0x8000);
  Actor_SetSpeed(27, 0x10000, 0x8000);
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
  Func_02004b6c_(0x1c, 0x107, 0);
  Func_02004b6c_(0x1c, 0x107, 0x28);
  Actor_SetSpeed(28, 0x30000, 0x18000);
  Actor_SetSpeed(29, 0x30000, 0x18000);
  Actor_SetScript(28, gData_020054f4);
  Actor_SetScript(29, gData_0200556c);
  FieldActor_RefreshSelectorById(28);
  FieldActor_RefreshSelectorById(29);
  FieldEvent_Wait(10);
  Actor_RunRepeatedMotion(28, 2);
  FieldEvent_ShowMessageThenWait(0x1c, 0, 0xa);
  FieldActor_Jump(29, 6, 15);
  FieldActor_Jump(29, 6, 23);
  FieldEvent_ShowMessageThenWait(0x1d, 0, 0xa);
  Actor_SetSpeed(28, 0x19999, 0xcccc);
  Actor_SetSpeed(29, 0x19999, 0xcccc);
  Actor_SetSpeed(25, 0x19999, 0xcccc);
  Actor_SetSpeed(27, 0x19999, 0xcccc);
  FieldActor_StartRepeatedMotion(25, 3);
  Actor_SetAttachedEffect(0x19, 0x102);
  FieldActor_StartRepeatedMotion(27, 3);
  Actor_SetAttachedEffect(0x1b, 0x102);
  FieldEvent_Wait(40);
  Actor_SetScript(28, gData_0200d5d0);
  Actor_SetScript(29, gData_02005620);
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
  Actor_SetAttachedEffect(0x19, 0x102);
  FieldActor_StartRepeatedMotion(27, 3);
  Actor_SetAttachedEffect(0x1b, 0x102);
  FieldEvent_Wait(40);
  FieldCamera_SetSpeed(0x60000, 0xc000);
  FieldCamera_MoveTo(PIXELS(384), -1, PIXELS(480), 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0x19, 0, 0xa);
  FieldCamera_SetSpeed(0x60000, 0xc000);
  FieldCamera_MoveTo(PIXELS(480), -1, PIXELS(344), 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0x19, 0, 0xa);
  FieldCamera_SetSpeed(0x60000, 0xc000);
  FieldCamera_MoveTo(PIXELS(488), -1, PIXELS(456), 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0x19, 0, 0xa);
  FieldCamera_SetSpeed(0x60000, 0xc000);
  FieldCamera_MoveTo(PIXELS(192), -1, PIXELS(616), 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(20);
  Func_02004b6c_(0x19, 0x102, 0x1e);
  Actor_FaceDirection(25, FACING_NORTH, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(0x19, 0, 0xa);
  Actor_FaceDirection(28, 0x3000, 0);
  Actor_FaceDirection(29, 0x5000, 0);
  FieldEvent_Wait(30);
  Actor_SetAnimation(28, 3);
  Actor_SetAnimationAndWait(29, 3);
  FieldEvent_Wait(20);
  Actor_FaceDirection(27, FACING_WEST, 0);
  FieldEvent_Wait(30);
  Func_02004b6c_(0x1b, 0x108, 0x1e);
  FieldEvent_ShowMessageThenWait(0x1b, 0, 0xa);
  Actor_FaceDirection(25, FACING_EAST, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(25, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0x19, 0, 0xa);
  Actor_SetAnimation(28, 3);
  Actor_SetAnimationAndWait(29, 3);
  FieldEvent_Wait(20);
  Actor_FaceDirection(25, 0xd000, 0);
  Actor_FaceDirection(27, 0xb000, 0);
  FieldEvent_Wait(40);
  Func_02004b6c_(0x19, 0x101, 0x1e);
  FieldEvent_ShowMessageThenWait(0x19, 0, 0xa);
  FieldActor_StartRepeatedMotion(28, 3);
  Actor_SetAttachedEffect(0x1c, 0x102);
  FieldActor_StartRepeatedMotion(29, 3);
  Actor_SetAttachedEffect(0x1d, 0x102);
  FieldEvent_Wait(50);
  Actor_FaceDirection(28, 0xb000, 0);
  Actor_FaceDirection(29, 0xb000, 0);
  FieldEvent_Wait(40);
  Func_02004b6c_(0x1c, 0x100, 0);
  Func_02004b6c_(0x1d, 0x100, 0x28);
  Actor_FaceDirection(25, 0xb000, 0);
  Actor_FaceDirection(27, 0xb000, 0);
  FieldEvent_Wait(20);
  FieldCamera_MoveTo(PIXELS(144), -1, PIXELS(544), 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(0x19, 0, 0xa);
  Func_02004b6c_(4, 0x102, 0x1e);
  Actor_SetSpeed(25, 0x18000, 0xc000);
  Actor_SetSpeed(27, 0x18000, 0xc000);
  Actor_WalkBy(25, -32, -32);
  Actor_WalkByAndWait(27, -32, -32);
  Actor_WalkBy(25, 0, -24);
  Actor_WalkByAndWait(27, 0, -24);
  FieldActor_WaitForMoveById(25);
  Actor_FaceDirection(25, 0xd000, 0);
  Actor_FaceDirection(27, 0xb000, 0);
  FieldEvent_Wait(20);
  Func_02004b6c_(0x19, 0x100, 0x1e);
  FieldEvent_ShowMessageThenWait(0x19, 0, 0xa);
  Actor_WalkToAndWait(4, 136, 520);
  FieldActor_LaunchFromLeader(0x1a, 0x10, 0, 0x5000);
  Actor_FaceDirection(4, FACING_SOUTH, 0);
  FieldActor_WaitForMoveById(26);
  FieldEvent_Wait(30);
  Actor_SetAnimationAndWait(26, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0x201a, 0, 0xa);
  Actor_SetAnimationAndWait(25, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0x19, 0, 0xa);
  Actor_SetAnimationAndWait(26, 4);
  FieldEvent_ShowMessageThenWait(0x201a, 0, 0xa);
  Func_02004b6c_(0x19, 0x109, 0x1e);
  FieldEvent_ShowMessageThenWait(0x19, 0, 0xa);
  Actor_FaceDirection(26, FACING_WEST, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(0x201a, 0, 0xa);
  Actor_RunRepeatedMotion(27, 2);
  FieldEvent_Wait(20);
  Actor_FaceDirection(26, 0x5000, 0);
  Actor_FaceDirection(27, FACING_WEST, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(0x1b, 0, 0xa);
  Func_02004b6c_(0x19, 0x100, 0x1e);
  Actor_FaceDirection(25, FACING_EAST, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(0x19, 0, 0xa);
  Actor_FaceDirection(27, 0x3000, 0);
  Actor_FaceDirection(25, 0x3000, 0);
  FieldEvent_Wait(20);
  FieldActor_Jump(25, 6, 15);
  FieldActor_Jump(25, 6, 23);
  FieldEvent_ShowMessageThenWait(0x19, 0, 0xa);
  Actor_RunRepeatedMotion(28, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0x1c, 0, 0xa);
  Actor_SetAnimationAndWait(25, 3);
  FieldEvent_Wait(20);
  Actor_RunRepeatedMotion(29, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0x1d, 0, 0xa);
  Actor_SetAnimationAndWait(25, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0x19, 0, 0x14);
  Actor_WalkBy(28, 0, 64);
  Actor_WalkBy(29, 0, 64);
  FieldEvent_Wait(60);
  Actor_SetPosition(28, 0, 0);
  Actor_SetPosition(29, 0, 0);
  Actor_FaceDirection(27, 0xb000, 0);
  Actor_FaceDirection(25, 0xd000, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(0x19, 0, 0xa);
  Actor_SetAnimationAndWait(25, 3);
  FieldEvent_Wait(10);
  FieldMessage_ShowAtActor_(0x19, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(4, 0) != 0) {
    FieldEvent_Wait(40);
    Actor_FaceDirection(26, FACING_WEST, 0);
    FieldEvent_Wait(20);
    Actor_SetAnimationAndWait(26, 4);
    FieldEvent_ShowMessageThenWait(0x201a, 0, 0xa);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
  } else {
    FieldEvent_Wait(30);
    Actor_FaceDirection(26, FACING_WEST, 0);
    FieldEvent_Wait(20);
    Actor_SetAnimationAndWait(26, 3);
    FieldEvent_Wait(10);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
    FieldEvent_ShowMessageThenWait(0x201a, 0, 0xa);
  }
  Actor_FaceDirection(4, FACING_EAST, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(4, 3);
  FieldEvent_Wait(30);
  Actor_SetAnimationAndWait(25, 3);
  FieldEvent_Wait(30);
  Actor_FaceDirection(4, FACING_SOUTH, 0);
  Actor_FaceDirection(26, 0x3000, 0);
  FieldEvent_Wait(20);
  Actor_FaceDirection(25, 0x3000, 0);
  Actor_FaceDirection(27, 0x3000, 0);
  FieldEvent_Wait(20);
  Actor_SetSpeed(4, 0xcccc, 0x6666);
  Actor_SetSpeed(26, 0xcccc, 0x6666);
  Actor_SetSpeed(25, 0xcccc, 0x6666);
  Actor_SetSpeed(27, 0xcccc, 0x6666);
  Actor_GetById(4)->motionFlags = 0;
  Actor_GetById(26)->motionFlags = 0;
  Actor_GetById(25)->motionFlags = 0;
  Actor_GetById(27)->motionFlags = 0;
  Actor_WalkBy(4, 0, 120);
  Actor_WalkBy(26, 0, 120);
  Actor_WalkBy(27, 0, 80);
  Actor_WalkByAndWait(25, 0, 80);
  {
    struct EventWork *event = Ram_HeapSlots->eventWork;

    event->delay = 60;
    event->screenEffect = 0x100;
  }
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Call2((void (*)())Event_SetDestination, (s32)&SceneId_Ov667, 81);
  Event_End();
}

/// Only once flag 0x8a7 is set: the scene that opens with MsgBriggs2, which
/// sets flags FLAG_ALHAFRA_SHIP_GONE and FLAG_BRIGGS_SAILED_FROM_ALHAFRA and
/// ends by sending the party to SceneId_Ov668Room1, entrance 99.
void Func_02002a08(void) {
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
  FieldEvent_ShowMessageThenWait(0x1d, 0, 0xa);
  Actor_WalkToAndWait(4, 136, 552);
  FieldActor_LaunchFromLeader(0x1a, -0x10, 0, 0xa000);
  Actor_ArmCallback(4, FACING_NORTH, 0);
  FieldActor_WaitForMoveById(26);
  FieldEvent_Wait(20);
  Func_02004b6c_(0x1a, 0x100, 0x1e);
  FieldCamera_MoveTo(PIXELS(176), -1, PIXELS(392), 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(20);
  Actor_RunRepeatedMotion(25, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0x19, 0, 0xa);
  Actor_RunRepeatedMotion(27, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0x1b, 0, 0xa);
  Actor_SetPosition(22, PIXELS(224), PIXELS(8));
  FieldEvent_ShowMessageThenWait(0x16, 0, 0xa);
  Ram_HeapSlots->mapWork->minX = 0;
  Ram_HeapSlots->mapWork->maxX = 0x4000000;
  Ram_HeapSlots->mapWork->minZ = 0;
  Ram_HeapSlots->mapWork->maxZ = 0x4000000;
  FieldCamera_SetSpeed(0x40000, 0x8000);
  FieldCamera_MoveTo(PIXELS(208), -1, PIXELS(168), 1);
  FieldEvent_Wait(50);
  Event_PlaySound(0x4e);
  Event_FinishScreenTransition();
  PaletteFade_Interpolate(0x10);
  Event_WaitTransitionFrames();
  FieldCamera_MoveTo(-1, -1, -1, 0);
  FieldEvent_Wait(10);
  FieldCamera_MoveTo(PIXELS(808), -1, PIXELS(664), 0);
  {
    struct Actor *center =
        ((struct EventWork *)Ram_HeapSlots->eventWork)->viewCenter;

    FieldActor_SetPositionAndResetMotion(center, center->x, center->y,
                                         center->z);
  }
  FieldEvent_Wait(50);
  FieldActor_SetSpritePriority(22, 1);
  FieldActor_SetSpritePriority(23, 1);
  Actor_SetSpeed(23, 0x13333, 0x9999);
  Actor_SetSpeed(22, 0x13333, 0x9999);
  Actor_SetPosition(22, PIXELS(792), PIXELS(856));
  Actor_SetPosition(23, PIXELS(824), PIXELS(856));
  Actor_SetPosition(26, PIXELS(744), PIXELS(1016));
  Actor_SetPosition(25, PIXELS(744), PIXELS(1016));
  Event_PlaySound(0x23);
  PaletteFade_ApplyTarget(0x10000, 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_Wait(30);
  FieldCamera_SetSpeed(0x20000, 0x4000);
  FieldCamera_MoveTo(PIXELS(808), -1, PIXELS(848), 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(20);
  FieldActor_Jump(22, 6, 15);
  FieldActor_Jump(22, 6, 23);
  FieldEvent_ShowMessageThenWait(0x16, 0, 0xa);
  Actor_SetAnimationAndWait(23, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0x17, 0, 0xa);
  FieldEvent_ShowMessageThenWait(0x19, 0, 0xa);
  Actor_ArmCallback(22, FACING_EAST, 0);
  Actor_FaceDirection(23, FACING_WEST, 0);
  FieldEvent_Wait(25);
  Actor_SetAnimationAndWait(22, 3);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(23, 3);
  FieldEvent_Wait(20);
  Actor_FaceDirection(22, 0x5000, 0);
  Actor_FaceDirection(23, 0x5000, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(0x16, 0, 0xa);
  FieldEvent_ShowMessageThenWait(0x1a, 0, 0xa);
  Func_02004b6c_(0x17, 0x100, 0);
  Func_02004b6c_(0x16, 0x100, 0x32);
  FieldActor_StartRepeatedMotion(22, 3);
  Actor_SetAttachedEffect(0x16, 0x102);
  FieldEvent_Wait(40);
  Actor_WalkByAndWait(22, 0, 16);
  FieldEvent_Wait(20);
  Func_02004b6c_(0x16, 0x102, 0x1e);
  FieldEvent_ShowMessageThenWait(0x16, 0, 0xa);
  Actor_WalkByAndWait(23, 0, 16);
  Actor_FaceDirection(23, FACING_WEST, 0);
  FieldEvent_Wait(20);
  Func_02004b6c_(0x17, 0x109, 0x1e);
  FieldEvent_ShowMessageThenWait(0x17, 0, 0xa);
  Actor_FaceDirection(23, 0x5000, 0);
  FieldEvent_Wait(50);
  Actor_FaceDirection(23, FACING_WEST, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(0x17, 0, 0xa);
  Actor_ArmCallback(22, FACING_EAST, 0);
  FieldEvent_Wait(20);
  Func_02004b6c_(0x16, 0x101, 0x2d);
  Actor_WalkByAndWait(22, 0, 32);
  FieldEvent_Wait(30);
  Func_02004b6c_(0x16, 0x106, 0x2d);
  Actor_WalkByAndWait(22, 0, -32);
  Actor_ArmCallback(22, FACING_EAST, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(22, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0x16, 0, 0xa);
  Actor_FaceDirection(23, 0x5000, 0);
  FieldEvent_Wait(50);
  Actor_ArmCallback(23, FACING_WEST, 0);
  FieldEvent_Wait(20);
  Func_02004b6c_(0x17, 0x108, 0x1e);
  FieldEvent_ShowMessageThenWait(0x17, 0, 0xa);
  Func_02004b6c_(0x16, 0x101, 0x32);
  Actor_WalkByAndWait(22, 0, 32);
  FieldEvent_Wait(30);
  Func_02004b6c_(0x16, 0x106, 0x32);
  Actor_WalkByAndWait(22, 0, -32);
  Actor_ArmCallback(22, FACING_EAST, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(22, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0x16, 0, 0xa);
  Func_02004b6c_(0x17, 0x109, 0x1e);
  FieldEvent_ShowMessageThenWait(0x17, 0, 0xa);
  Actor_FaceDirection(22, 0x5000, 0);
  FieldEvent_Wait(30);
  Func_02001bf4(0x16);
  Event_PlaySound(0x67);
  Func_02004b6c_(0x16, 0x105, 0x32);
  Actor_ArmCallback(22, FACING_EAST, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(23, 4);
  FieldEvent_ShowMessageThenWait(0x17, 0, 0xa);
  Actor_FaceDirection(22, 0x5000, 0);
  FieldEvent_Wait(30);
  Func_02001bf4(0x16);
  Func_02004b6c_(0x16, 0x106, 0x32);
  Actor_ArmCallback(22, FACING_EAST, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(22, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0x16, 0, 0xa);
  Func_02004b6c_(0x17, 0x109, 0x1e);
  FieldEvent_ShowMessageThenWait(0x17, 0, 0xa);
  Actor_RunRepeatedMotion(22, 2);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(22, 3);
  FieldEvent_Wait(20);
  Actor_FaceDirectionAndWait(22, 0x5000);
  Actor_SetAnimation(22, 7);
  FieldEvent_Wait(20);
  Actor_SetAnimation(22, 6);
  FieldEvent_Wait(5);
  FieldEvent_ShowMessageThenWait(0x16, 0, 0xa);
  FieldActor_Jump(23, 6, 15);
  FieldActor_Jump(23, 6, 23);
  FieldEvent_ShowMessageThenWait(0x17, 0, 0xa);
  Func_02004b6c_(0x16, 0x102, 0x32);
  Actor_SetAnimation(22, 7);
  FieldEvent_Wait(20);
  Actor_SetAnimation(22, 1);
  Actor_ArmCallback(22, FACING_EAST, 0);
  FieldEvent_Wait(15);
  FieldEvent_ShowMessageThenWait(0x16, 0, 0xa);
  Func_02004b6c_(0x17, 0x103, 0x1e);
  FieldEvent_ShowMessageThenWait(0x17, 0, 0xa);
  Func_02004b6c_(0x16, 0x106, 0x28);
  FieldEvent_ShowMessageThenWait(0x16, 0, 0xa);
  Actor_WalkByAndWait(22, 0, 8);
  Actor_WalkByAndWait(22, 10, 16);
  Actor_FaceDirection(23, 0x5000, 0);
  Actor_WalkByAndWait(22, 6, 16);
  Actor_ArmCallback(22, 0x5000, 0);
  FieldEvent_Wait(10);
  FieldActor_Jump(22, 6, 15);
  FieldActor_Jump(22, 6, 23);
  FieldEvent_ShowMessageThenWait(0x4016, 0, 0xa);
  FieldEvent_ShowMessageThenWait(0x1a, 0, 0xa);
  Func_02004b6c_(0x16, 0x108, 0x1e);
  FieldEvent_ShowMessageThenWait(0x4016, 0, 0x14);
  Actor_SetAnimation(22, 8);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0x16, 0, 0xa);
  FieldActor_Jump(22, 7, 0);
  i = 15;
turn:
  // The angle, through s16: as the u16 field its halfword zero is shared with
  // the motion-flag stores below and loads from the pool (differ 19 more).
  ((s16 *)Actor_GetById(22))[3] -= 0x1000;
  i--;
  System_Wait(1);
  if (i >= 0)
    goto turn;
  Actor_SetAnimation(22, 5);
  FieldEvent_Wait(7);
  FieldEvent_ShowMessageThenWait(0x16, 0, 0x14);
  Func_02004b6c_(0x17, 0x108, 0x32);
  Actor_WalkByAndWait(23, 0, 36);
  FieldEvent_Wait(15);
  Actor_SetAnimation(22, 1);
  FieldEvent_Wait(20);
  FieldActor_Jump(23, 6, 15);
  FieldActor_Jump(23, 6, 23);
  FieldEvent_ShowMessageThenWait(0x4017, 0, 0xf);
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
  Event_PlaySound(0x4e);
  FieldEvent_Wait(30);
  Event_End();
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Event_SetDestination((s32)&SceneId_Ov668Room1, 99);
}

/// The scene that opens with MsgWhatYouLet; either answer to each of its two
/// prompts steps the event's message number, by 1 and then by 2.
void Func_02003274(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgWhatYouLet);
  FieldCamera_MoveTo(PIXELS(152), -1, PIXELS(272), 0);
  {
    struct Actor *center =
        ((struct EventWork *)Ram_HeapSlots->eventWork)->viewCenter;

    FieldActor_SetPositionAndResetMotion(center, center->x, center->y,
                                         center->z);
  }
  FieldEvent_Wait(30);
  PaletteFade_ApplyTarget(0x10000, 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_Wait(15);
  FieldEvent_ShowMessageThenWait(0x19, 0, 0xa);
  FieldCamera_MoveTo(PIXELS(152), -1, PIXELS(392), 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(20);
  Func_02004b6c_(0x19, 0x103, 0x1e);
  FieldEvent_ShowMessageThenWait(0x1019, 0, 0xa);
  Actor_FaceDirection(25, 0x5000, 0);
  FieldEvent_Wait(30);
  FieldEvent_ShowMessageThenWait(0x1019, 0, 0xa);
  FieldActor_Jump(25, 6, 15);
  FieldActor_Jump(25, 6, 23);
  FieldEvent_Wait(10);
  Func_02004b6c_(0x19, 0x109, 0x1e);
  FieldEvent_ShowMessageThenWait(0x1019, 0, 0xa);
  Actor_FaceDirection(25, 0xd000, 0);
  FieldEvent_Wait(30);
  Func_02004b6c_(0x19, 0x100, 0x1e);
  Actor_SetSpeed(25, 0xb333, 0x5999);
  Actor_GetById(25)->actionFlags &= 0xfe;
  Actor_WalkByAndWait(25, 0, 8);
  FieldEvent_Wait(1);
  Actor_GetById(25)->actionFlags |= 1;
  FieldEvent_Wait(20);
  Actor_RunRepeatedMotion(25, 2);
  FieldEvent_Wait(10);
  Actor_SetSpeed(25, 0x26666, 0x13333);
  Actor_GetById(25)->actionFlags &= 0xfe;
  Actor_WalkByAndWait(25, 0, -16);
  FieldEvent_Wait(1);
  Actor_GetById(25)->actionFlags |= 1;
  Actor_GetById(25)->actionFlags &= 0xfe;
  Actor_WalkBy(25, 0, 8);
  Event_PlaySound(0x85);
  Actor_SetSpeed(29, 0x26666, 0x13333);
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
  FieldEvent_ShowMessageThenWait(0x1019, 0, 0xa);
  Actor_SetPosition(29, PIXELS(168), PIXELS(368));
  FieldActor_SetSpriteRotationById(0x1d, 0);
  Actor_SetSpriteFlags(Actor_GetById(29), 1);
  Actor_FaceDirection(29, 0x5000, 0);
  FieldActor_Jump(29, 6, 40);
  Func_02004b6c_(0x1d, 0x102, 0x28);
  Actor_ArmCallback(25, 0x3000, 0);
  FieldEvent_Wait(30);
  Actor_SetSpeed(25, 0xb333, 0x5999);
  Actor_GetById(25)->actionFlags &= 0xfe;
  Actor_WalkByAndWait(25, 0, -8);
  FieldEvent_Wait(1);
  Actor_GetById(25)->actionFlags |= 1;
  FieldEvent_Wait(20);
  Actor_SetSpeed(25, 0x26666, 0x13333);
  Actor_GetById(25)->actionFlags &= 0xfe;
  Actor_WalkByAndWait(25, 0, 16);
  FieldEvent_Wait(1);
  Actor_GetById(25)->actionFlags |= 1;
  Event_PlaySound(0x85);
  Actor_SetSpeed(29, 0x26666, 0x13333);
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
  FieldEvent_ShowMessageThenWait(0x1019, 0, 0xa);
  Actor_SetPosition(27, PIXELS(168), PIXELS(424));
  FieldActor_SetSpriteRotationById(0x1b, 0);
  Actor_SetSpriteFlags(Actor_GetById(27), 1);
  Actor_GetById(27)->facing = 0xb000;
  FieldActor_Jump(27, 6, 40);
  Func_02004b6c_(0x1b, 0x102, 0x28);
  Actor_ArmCallback(25, 0xd000, 0);
  FieldEvent_Wait(30);
  Func_02004b6c_(0x19, 0x107, 0x1e);
  FieldEvent_ShowMessageThenWait(0x1019, 0, 0xa);
  Func_02004b6c_(0x1d, 0x109, 0x32);
  Actor_ArmCallback(25, 0x3000, 0);
  FieldEvent_Wait(30);
  Func_02004b6c_(0x19, 0x107, 0x1e);
  FieldEvent_ShowMessageThenWait(0x1019, 0, 0xa);
  Func_02004b6c_(0x1b, 0x102, 0x32);
  Actor_SetAnimation(25, 5);
  FieldEvent_Wait(30);
  FieldEvent_ShowMessageThenWait(0x1019, 0, 0xa);
  Actor_ArmCallback(29, 0x3000, 0);
  Actor_ArmCallback(27, 0xd000, 0);
  FieldEvent_Wait(20);
  FieldActor_StartRepeatedMotion(29, 3);
  Actor_SetAttachedEffect(0x1d, 0x102);
  FieldActor_StartRepeatedMotion(27, 3);
  Actor_SetAttachedEffect(0x1b, 0x102);
  FieldEvent_Wait(40);
  Actor_SetSpeed(29, 0x1cccc, 0xe666);
  Actor_WalkByAndWait(29, 16, 16);
  Actor_WalkByAndWait(29, 0, 40);
  Actor_ArmCallback(29, 0xb000, 0);
  FieldEvent_Wait(30);
  Actor_SetAnimation(25, 1);
  FieldEvent_Wait(30);
  Actor_ArmCallback(29, 0xb000, 0);
  Actor_ArmCallback(27, 0xb000, 0);
  FieldEvent_ShowMessageThenWait(0x1019, 0, 0xa);
  Actor_SetAnimation(27, 3);
  Actor_SetAnimationAndWait(29, 3);
  FieldEvent_Wait(20);
  Actor_SetSpeed(25, 0xcccc, 0x6666);
  Actor_SetSpeed(27, 0xcccc, 0x6666);
  Actor_SetSpeed(29, 0xcccc, 0x6666);
  Actor_SetSpeed(28, 0xcccc, 0x6666);
  Actor_SetSpeed(30, 0xcccc, 0x6666);
  Actor_SetSpeed(31, 0xcccc, 0x6666);
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
  FieldEvent_ShowMessageThenWait(0x6019, 0, 0xa);
  FieldActor_StartRepeatedMotion(4, 3);
  Actor_SetAttachedEffect(4, 0x102);
  FieldActor_StartRepeatedMotion(26, 3);
  Actor_SetAttachedEffect(0x1a, 0x102);
  FieldEvent_Wait(40);
  Actor_WalkBy(27, 0, 64);
  Actor_WalkBy(29, 0, 64);
  Actor_WalkByAndWait(25, 0, 64);
  Func_02004b6c_(0x19, 0x106, 0x28);
  FieldEvent_ShowMessageThenWait(0x2019, 0, 0xa);
  Actor_ArmCallback(26, 0x3000, 0);
  Actor_ArmCallback(4, FACING_SOUTHEAST, 0);
  FieldEvent_Wait(30);
  Actor_SetAnimationAndWait(25, 3);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(0x2019, 0, 0xa);
  Actor_WalkByAndWait(25, 0, 16);
  FieldEvent_Wait(10);
  Actor_RunRepeatedMotion(25, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0x19, 0, 0xa);
  FieldActor_StartRepeatedMotion(30, 2);
  FieldActor_StartRepeatedMotion(31, 2);
  Actor_RunRepeatedMotion(28, 2);
  FieldEvent_Wait(20);
  Actor_SetPosition(28, PIXELS(176), PIXELS(592));
  FieldActor_SetSpriteRotationById(0x1c, 0);
  Actor_SetSpriteFlags(Actor_GetById(28), 1);
  FieldActor_Jump(28, 6, 20);
  Actor_ArmCallback(28, FACING_WEST, 0);
  Actor_SetPosition(31, PIXELS(128), PIXELS(608));
  FieldActor_SetSpriteRotationById(0x1f, 0);
  Actor_SetSpriteFlags(Actor_GetById(31), 1);
  FieldActor_Jump(31, 6, 20);
  Actor_ArmCallback(31, 0xd000, 0);
  Actor_SetPosition(30, PIXELS(192), PIXELS(616));
  FieldActor_SetSpriteRotationById(0x1e, 0);
  Actor_SetSpriteFlags(Actor_GetById(30), 1);
  FieldActor_Jump(30, 6, 20);
  Actor_ArmCallback(30, 0xb000, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(25, 4);
  FieldEvent_ShowMessageThenWait(0x19, 0, 0xa);
  Actor_SetAnimation(30, 3);
  Actor_SetAnimation(31, 3);
  Actor_SetAnimationAndWait(28, 3);
  FieldEvent_Wait(30);
  FieldActor_WatchAndSetScript(4, 25);
  FieldActor_WatchAndSetScript(26, 25);
  FieldActor_WatchAndSetScript(30, 25);
  FieldActor_WatchAndSetScript(31, 25);
  FieldActor_WatchAndSetScript(28, 25);
  Actor_SetScript(25, (s32)gData_02005670);
  Actor_SetScript(27, (s32)gData_02005670);
  FieldEvent_Wait(10);
  Actor_SetScript(29, (s32)gData_02005670);
  FieldEvent_Wait(70);
  Actor_WalkByAndWait(28, -16, 0);
  Actor_SetScript(28, (s32)gData_02005670);
  FieldEvent_Wait(33);
  Actor_WalkByAndWait(31, 16, 0);
  Actor_SetScript(31, (s32)gData_02005670);
  FieldEvent_Wait(15);
  Actor_SetScript(30, (s32)gData_02005670);
  FieldActor_RefreshSelectorById(25);
  Actor_ArmCallback(26, 0x5000, 0);
  FieldActor_RefreshSelectorById(30);
  FieldEvent_Wait(20);
  EventCamera_FollowActor(4, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(40);
  Func_02004b6c_(0x1a, 0x105, 0x32);
  Actor_ArmCallback(26, FACING_EAST, 0);
  Actor_ArmCallback(4, FACING_WEST, 0);
  FieldEvent_Wait(20);
  FieldMessage_ShowAtActor_(0x1a, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(4, 0) == 0) {
    FieldEvent_Wait(20);
    FieldEvent_ShowMessageThenWait(0x1a, 0, 0xa);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
  } else {
    FieldEvent_Wait(40);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
    FieldEvent_ShowMessageThenWait(0x1a, 0, 0xa);
  }
  Actor_SetSpeed(26, 0xcccc, 0x6666);
  Actor_WalkByAndWait(26, 0, -16);
  Actor_FaceDirection(4, FACING_NORTH, 0);
  Actor_ArmCallback(26, 0xd000, 0);
  FieldEvent_Wait(20);
  Actor_RunRepeatedMotion(26, 2);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(0x1a, 0, 0xa);
  Actor_WalkByAndWait(26, 0, 16);
  Actor_FaceDirection(4, FACING_WEST, 0);
  Actor_ArmCallback(26, FACING_EAST, 0);
  FieldEvent_Wait(20);
  FieldMessage_ShowAtActor_(0x1a, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(4, 0) != 0) {
    FieldEvent_Wait(40);
    FieldEvent_ShowMessageThenWait(0x1a, 0, 0xa);
    Actor_SetAnimationAndWait(26, 3);
    FieldEvent_Wait(10);
    FieldEvent_ShowMessageThenWait(0x1a, 0, 0xa);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 2;
  } else {
    FieldEvent_Wait(20);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 2;
    FieldEvent_ShowMessageThenWait(0x1a, 0, 0xa);
  }
  Actor_SetAnimationAndWait(4, 3);
  FieldEvent_Wait(20);
  Actor_SetSpeed(26, 0x13333, 0x9999);
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

DEFINE_PARTICLES_UPDATE_FALLING(Particles_UpdateFalling,
                                EffectSprite_DrawAtWorldPosition)

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
DEFINE_ROCKS_SPAWN(Rocks_Spawn)

// ---- scene/eastern_alhafra/s4004.c ----

/// Scene code of overlay 0x668, in ROM order.

#include "shared/push.h"
#include "system/task.h"
#include "system/heap_release.h"
#include "item_icon.h"
#include "graphics/resource.h"

static __inline__ void StartTask(void (*func)(void), s32 priority) {
  /* FAKEMATCH(wrapper): Task_Add forwarded inline; as a direct call, 6 of 83
   * instructions differ in tla-en. */
  Task_Add(func, priority);
}

void Func_02004004(void) {
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
    Func_02004c64(sprite, 8, 8, 0x40000000, tile);
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

void Func_020040c0(void) {
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

/* VIEW(s4004): routine gPushFootprints as this part declared it; the joined
 * file has another. */
extern s8 gPushFootprints__s4004[][4] __asm__("gPushFootprints");
#define gPushFootprints gPushFootprints__s4004

static __inline__ s32 Abs(s32 value) { return value < 0 ? -value : value; }

/// Rewrites the map cells under a pushable actor. The actor's sprite shape
/// picks one of the six footprints in gPushFootprints (four signed bytes each,
/// in sixteenths of a cell); the cells under it are copied from the map's spare
/// area (Map_CopyCellAttributeRect) and byte 2 of both collision layers there
/// is filled with 255. Returns 1, or 0 for a shape with no footprint.
s32 Func_020040ec(s32 id) {
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
  struct Actor *actor = Actor_GetById(id);
  s16 kind = *((struct ObjectAnimationState *)actor->sprite)->value28;
  s16 i = 0;
  s32 next;
  u32 index;
  s32 a, b;
  s32 fromX, fromZ;
  s32 width, height, x, z;

  // A goto loop: a C loop is strength-reduced, which the original's is not.
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

#include "system/vector3.h"

/* VIEW(s4004): routine gPushDirectionSteps as this part declared it; the joined
 * file has another. */
extern s32 gPushDirectionSteps__s4004[] __asm__("gPushDirectionSteps");
#define gPushDirectionSteps gPushDirectionSteps__s4004

static __inline__ s32 CallFunc_02004a64(struct Actor *actor,
                                        struct Vector3 *cell) {
  return MapGrid_CheckCollision(actor, cell);
}

/// Measures a push: finds the pushable actor ahead of the leader
/// (PushTarget_FindAhead, recording its number and kind in the push state),
/// then steps the actor's box one cell at a time along the leader's direction
/// until a cell of it is blocked (MapGrid_CheckCollision returns 2); with at
/// least one free step it records the end position in the push state and
/// returns 1, else 0. The actor stays on layer 2 while it is measured. The same
/// code as overlay 674's Func_020037d8 and overlay 662's Func_020001d4.
s32 Func_02004240(struct PushState *push) {
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
    offset = gPushDirectionSteps[direction];
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
          /* FAKEMATCH(wrapper): the call through CallFunc_02004a64; as a direct
           * call, 2 of 171 instructions differ in tla-en. */
          if (CallFunc_02004a64(actor, &cell) == 2)
            goto blocked;
          cell.x += 0x100000;
        }
        cell.z += 0x100000;
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
#define PIXELS(v) (((s16 *)&(v))[1])

DEFINE_PUSH_TARGET_FIND_AHEAD(PushTarget_FindAhead, gPushKinds,
                              gPushDirectionSteps, gPushFootprints)

// ---- scene/eastern_alhafra/s44c4.c ----

/// Scene code of overlay 0x668, in ROM order.

#include "shared/map_cells.h"
#include "module/field/field.h"

struct PushTarget {
  s32 x; ///< 16.16 fixed-point pixels
  s32 y;
  s32 z;
  s32 footprint; ///< index into the footprint table
  s32 id;
  void (*callback)(void);
};

/* VIEW(s44c4): variable gPushFootprints as s8 gPushFootprints[];
 * scene/eastern_alhafra/s4004 has s8 gPushFootprints[][4]; tried as the
 * definition's extern u32 gPushFootprints[], it does not compile in tla-ja:
 * games/tla/src/scene/eastern_alhafra.c:2348: conflicting types for
 * `gPushFootprints'. */
extern s8 gPushFootprints__s44c4[] __asm__("gPushFootprints");

#define ABS(x) ((x) < 0 ? -(x) : (x))

void Func_020044c4(struct PushTarget target) {
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
  cell[0] = (object->x + (gPushFootprints__s44c4[footprint * 4] << 16)) >> 20;
  cell[2] =
      (object->z + (gPushFootprints__s44c4[footprint * 4 + 1] << 16)) >> 20;
  width = (ABS(gPushFootprints__s44c4[footprint * 4]) +
           ABS(gPushFootprints__s44c4[footprint * 4 + 2])) >>
          4;
  height = (ABS(gPushFootprints__s44c4[footprint * 4 + 1]) +
            ABS(gPushFootprints__s44c4[footprint * 4 + 3])) >>
           4;
  Map_FillCellRectByte2(0, cell[0], cell[2], width, height, 0);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x8000, 0x1999);
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
  Event_PlaySound(239);
  Actor_SetSpeed(target.id, 0x8000, 0x1999);
  FieldActor_SetMoveTarget(object, target.x, target.y, target.z);
  FieldActor_WaitForMoveById(gMainRoutineWork.currentOwner);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 2);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x4ccc, 0x1999);
  Actor_SetDestinationOffset(gMainRoutineWork.currentOwner,
                             (gPushDirectionSteps[direction] << 16) >> 31,
                             (gPushDirectionSteps[direction] << 16) >> 17);
  if (target.callback != 0)
    target.callback();
  FieldActor_WaitForMoveById(gMainRoutineWork.currentOwner);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 1);
  leader->update = 0;
  leader->speedLimit = speed;
  leader->acceleration = acceleration;
  FieldActor_WaitForMove(object);
  Event_PlaySound(298);
  Event_PlaySound(213);
  FieldActor_SetPositionAndResetMotion(object, target.x, object->y, target.z);
  Actor_SelectAnimation(object, 1);
  target.x = target.x + (gPushFootprints__s44c4[footprint * 4] << 16);
  target.z = target.z + (gPushFootprints__s44c4[footprint * 4 + 1] << 16);
  target.x >>= 20;
  target.z >>= 20;
  originX = *(s32 *)(map + 328) >> 20;
  originZ = *(s32 *)(map + 332) >> 20;
  MapGrid_CopyCellAttributes(target.x, target.z, width, height,
                             originX + target.x, originZ + target.z);
  Map_FillCellRectByte2(0, target.x, target.z, width, height, 255);
  Map_FillCellRectByte2(2, target.x, target.z, width, height, 255);
  cell[0] = (from[0] + (gPushFootprints__s44c4[footprint * 4] << 16)) >> 20;
  cell[2] = (from[2] + (gPushFootprints__s44c4[footprint * 4 + 1] << 16)) >> 20;
  originX += cell[0];
  originZ += cell[2];
  MapGrid_CopyCellAttributes(originX, originZ, width, height, cell[0], cell[2]);
  Map_FillCellRectByte2(2, cell[0], cell[2], width, height, 0);
  Event_NoOp();
}

DEFINE_ACTOR_STOP_AT_PUSH_TARGET(Actor_StopAtPushTarget, Actor_FindAtCell,
                                 gPushKinds, MapGrid_CheckCollision)

void Func_020047fc(const u16 *list) {
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
DEFINE_STATUE_SINK(Statue_Sink, Actor_SetCellOpen)

#include "field_event.h"

DEFINE_ACTOR_SET_CELL_OPEN(Actor_SetCellOpen, Actor_GetCellIndex,
                           Actor_SelectAnimation)

#undef ABS
#undef EVENT_WORK
#undef OBJECT
#undef FIELD_AT
#undef PIXELS
#define PIXELS(value) ((value) << 16)
#undef gData_02004d0c
#undef gData_02004dbc
#undef gData_02004fe4
#undef gData_02005290
#undef gData_020054f0
#undef gData_020056d4
#undef gPushFootprints
#undef gPushDirectionSteps
// ---- from scene/data/eastern_alhafra.c ----
#include "types.h"
#include "flags.h"
#include "scene_tables.h"
#include "text/msg_ids.h"

/* VIEW(data): routine Func_0200006c as this part declared it; the joined file
 * has another. */
void Func_0200006c__data(void) __asm__("Func_0200006c");
#define Func_0200006c Func_0200006c__data
/* VIEW(data): routine Func_020000c8 as this part declared it; the joined file
 * has another. */
void Func_020000c8__data(void) __asm__("Func_020000c8");
#define Func_020000c8 Func_020000c8__data
/* VIEW(data): routine Func_02000190 as this part declared it; the joined file
 * has another. */
void Func_02000190__data(void) __asm__("Func_02000190");
#define Func_02000190 Func_02000190__data
/* VIEW(data): routine Func_02000200 as this part declared it; the joined file
 * has another. */
void Func_02000200__data(void) __asm__("Func_02000200");
#define Func_02000200 Func_02000200__data
/* VIEW(data): routine Func_02000394 as this part declared it; the joined file
 * has another. */
void Func_02000394__data(void) __asm__("Func_02000394");
#define Func_02000394 Func_02000394__data
/* VIEW(data): routine Func_0200042c as this part declared it; the joined file
 * has another. */
void Func_0200042c__data(void) __asm__("Func_0200042c");
#define Func_0200042c Func_0200042c__data
/* VIEW(data): routine Func_020006ec as this part declared it; the joined file
 * has another. */
void Func_020006ec__data(void) __asm__("Func_020006ec");
#define Func_020006ec Func_020006ec__data
/* VIEW(data): routine Func_02000940 as this part declared it; the joined file
 * has another. */
void Func_02000940__data(void) __asm__("Func_02000940");
#define Func_02000940 Func_02000940__data
void Func_02000958(void);

// clang-format off
u16 gPushKinds[] = {
    258, 259, 293, 294, 332, 331,
};

u32 gPushFootprints[] = {
    0x820f8e0, 0x2008e0f8, 0x20f0e0, 0x2008e0f8, 0x820f8e0, 0x2008e0f8,
};

s16 gPushDirectionSteps[] = {
    0, 16, 0, 16, 0, 16, 16, 0,
    16, 0, 16, 0, 16, 0, 0, -16,
    0, -16, 0, -16, 0, -16, -16, 0,
    -16, 0, -16, 0, -16, 0, 0, 16,
    10, -1,
};

s16 gData_02004d0c[] = {
    14, 1, 0x8a2, -1,
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
    SCENE_EXITS(98),
    SCENE_EXIT_IF(1, 95, 11, CONDITION_ALWAYS),
    SCENE_EXIT_IF(2, 99, 1, CONDITION_ALWAYS),
    SCENE_EXITS_END,
};

s16 gData_02004dbc[] = {
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
    39, -1, 1, 0, 0, 312, 0, 0,
    0, 440, 0, 2, 39, -1, 1, 0,
    0, 440, 0, 0, 0, 424, 0x5000, 1,
    39, -1, 1, 0, 0, 344, 0, 0,
    0, 488, FACING_NORTH, 1, 39, -1, 1, 0,
    0, 440, 0, 0, 0, 376, FACING_NORTHEAST, 1,
    5, -1, 1, 0, 0, 8, 0, 0,
    0, 8, FACING_NORTH, 2, 6, -1, 1, 0,
    0, 8, 0, 0, 0, 8, FACING_NORTH, 2,
    7, -1, 1, 0, 0, 8, 0, 0,
    0, 8, FACING_NORTH, 2, 56, -1, 1, 0,
    0, 8, 0, 0, 0, 8, FACING_NORTH, 2,
    -1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0,
};

s16 gData_02004fe4[] = {
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

s16 gData_02005284[] = {
    0, 0, 1, 2, 6, -1,
};

s32 gData_02005290[] = {
    (s32)gData_02005284,
    0x110010,
};

struct Trigger gSceneEvents[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {0xc602, 2, CONDITION_ALWAYS, EVENT(Func_02000940)},
    {TRIGGER_TALK, 22, CONDITION_ALWAYS, EVENT(Func_02000730)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 1046, 0x8a8, EVENT(Func_02000730)},
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
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 10, FLAG_ALHAFRA_MAST_CLEARED, EVENT(Func_0200006c)},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), 8, 0x8a3, 0},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 14, 0x8a2, EVENT(Func_020000c8)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 10, 0x8a0, EVENT(Func_02000190)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 10, 0x8a0, EVENT(Func_02000200)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 11, 0x8a1, EVENT(Func_02000190)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 11, 0x8a1, EVENT(Func_02000200)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 16, 0x8a4, EVENT(Func_02000394)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 16, 0x8a4, EVENT(Func_0200042c)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 17, 0x8a5, EVENT(Func_02000190)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 17, 0x8a5, EVENT(Func_02000200)},
    {8, 0, CONDITION_ALWAYS, EVENT(Func_02000190)},
    {9, 0, CONDITION_ALWAYS, EVENT(Func_02000200)},
    {0xc602, 12, CONDITION_ALWAYS, EVENT(Func_020006ec)},
    {0x4602, 12, CONDITION_ALWAYS, EVENT(Func_020006ec)},
    {0xc602, 13, CONDITION_ALWAYS, EVENT(Func_020006ec)},
    {0xc602, 11, CONDITION_ALWAYS, EVENT(Func_02000514)},
    {0x4602, 11, CONDITION_ALWAYS, EVENT(Func_02000514)},
    {0xa602, 12, CONDITION_ALWAYS, EVENT(Func_02000954)},
    {0xb602, 12, CONDITION_ALWAYS, EVENT(Func_02000954)},
    {0x2602, 12, CONDITION_ALWAYS, EVENT(Func_02000954)},
    {0x3602, 12, CONDITION_ALWAYS, EVENT(Func_02000954)},
    {0xa602, 11, CONDITION_ALWAYS, EVENT(Func_02000954)},
    {0xb602, 11, CONDITION_ALWAYS, EVENT(Func_02000954)},
    {0x2602, 11, CONDITION_ALWAYS, EVENT(Func_02000954)},
    {0x3602, 11, CONDITION_ALWAYS, EVENT(Func_02000954)},
    {TRIGGER_STEP, 20, CONDITION_ALWAYS, EVENT(Func_02000838)},
    {TRIGGER_STEP, 21, 0x905, EVENT(Func_02002048)},
    {TRIGGER_STEP, 22, FLAG_ALHAFRA_SHIP_GONE, EVENT(Func_02002a08)},
    {TRIGGER_STEP, 23, 0x8ac, EVENT(Func_02001574)},
    {TRIGGER_STEP, 24, 0x8bf, EVENT(Func_02000968)},
    {TRIGGER_STEP, 25, 0x8be, EVENT(Func_02000958)},
    {TRIGGER_TABLE_END},
};

u32 gData_020054f0[] = {
    0,
};

s32 gData_020054f4[] = {
    39, 2, 5, 0, 0, 0x80000,
    1, 5, 0x100000, 0, 0x100000, 1,
    5, 0x100000, 0, -0x100000, 1, 5,
    0, 0, -0x100000, 1, 39, 1,
    22, 3, 0x8000, 0, 1, 17,
};

s32 gData_0200556c[] = {
    39, 2, 5, 0x200000, 0, 0x200000,
    1, 5, 0x100000, 0, -0x100000, 1,
    5, 0x20000, 0, -0x60000, 1, 39,
    1, 22, 3, 0x8000, 0, 1,
    17,
};

u32 gData_0200d5d0[] = {
    39, 2, 5, 0x300000, 0, 0,
    1, 4, 0xa80000, 0, 0x2480000, 1,
    39, 1, 22, 3, 0xe000, 0,
    1, 17,
};

u32 gData_02005620[] = {
    39, 2, 5, 0x300000, 0, 0,
    1, 4, 0xb80000, 0, 0x2480000, 1,
    39, 1, 22, 3, 0xe000, 0,
    1, 17,
};

s32 gData_02005670[] = {
    39, 2, 4, 0x980000, 0, 0x2800000,
    1, 5, -0x500000, 0, 0, 1,
    5, -0x200000, 0, -0x200000, 1, 5,
    -0x180000, 0, 0, 1, 0, 1,
    17,
};
// clang-format on

u32 gData_020056d4[] = {
    38,
};
#undef Func_0200006c
#undef Func_020000c8
#undef Func_02000190
#undef Func_02000200
#undef Func_02000394
#undef Func_0200042c
#undef Func_020006ec
#undef Func_02000940
