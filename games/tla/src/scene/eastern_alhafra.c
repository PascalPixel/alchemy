/// @file
/// The scene code of overlay 0x668, in ROM order.

// ---- games/tla/src/scene/eastern_alhafra.c ----

#include "shared/effects.h"
#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "flags.h"
#include "scene_tables.h"
#include "main_routine_work.h"
#include "event_inline.h"
#include "module/scene/eastern_alhafra.h"
#include "module/scene/scene.h"
#include "module/tla.h"
#include "particle.h"

extern u8 gData_02004d0c[];
extern u8 gData_02004dbc[];
extern u8 gData_02004fe4[];

void Statue_Sink(void *, s32, s32);
s32 Func_02004a7c();

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
    Func_02004c8c(1);
    Func_02004c8c(2);
    i = 0;
    do {
      Func_02004c24(1, 18, 19);
      Func_02004c24(2, 20, 21);
      System_Wait(1);
      Func_02004c8c(1);
      Func_02004c8c(2);
      if (Func_02004a7c(a) != 0 && Func_02004a7c(b) != 0)
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
      MapGrid_SetCellBehavior(0x2100000, 0x2000000, 2, 255);
    if (x == 33 && z == 32 && leaderX == 32)
      MapGrid_SetCellBehavior(0x2000000, 0x2000000, 2, 255);
  }
}

#include "ram_buffer.h"
#include "event_work.h"
#include "text/msg_ids.h"

extern u8 gData_02005290[];

void Func_0200112c(void);
s32 Func_02004240(struct DoorPlan *);
void Func_020044c4(struct DoorPlan);
/* VIEW(eastern_alhafra): routine Actor_SelectAnimation as s32
 * Actor_SelectAnimation(); module/scene/scene.h has void
 * Actor_SelectAnimation(struct Actor *object, s32 mode); tried as the rest of
 * the object declares it, Func_02001b9c moves: 1 of 35 instructions in tla-ja.
 */
extern s32 Func_02004a0c__eastern_alhafra() __asm__("Actor_SelectAnimation");
s32 Party_FindOwnerWithAction();
/* VIEW(eastern_alhafra): routine Field_FindNearbyTalkTarget as s32
 * Field_FindNearbyTalkTarget(); module/scene/airs_rock_inside.h has void
 * Field_FindNearbyTalkTarget(void); tried as the definition's void
 * Field_FindNearbyTalkTarget(void), which the object already declares,
 * Func_020006ec moves: 1 of 25 instructions in tla-ja. */
extern s32
Func_02004bb4__eastern_alhafra() __asm__("Field_FindNearbyTalkTarget");

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
    MapGrid_SetCellBehavior(0x2100000, 0x2000000, 2, 0);
    MapGrid_SetCellBehavior(0x2000000, 0x2000000, 2, 0);
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
    Actor_SetSingleChildFlags(Actor_Get(id), 1);
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
    Actor_SetSingleChildFlags(spark, 0);
    Event_PlaySound(154);
    System_Wait(5);
    i++;
    if (i <= 2)
      goto create;
    Func_02000fa4();
    FieldActor_Stop(gMainRoutineWork.currentOwner);
    System_Wait(30);
    for (i = 0; i <= 2; i++)
      Import_ActorDestroy(sparks[i]);
  }
}

#include "actor.h"

void Func_02000394(s32 unused, s32 id) {
  Func_02000190(unused, id);
  MapGrid_SetCellBehavior(0x1400000, 0x1d00000, 2, 255);
  MapGrid_SetCellBehavior(0x1400000, 0x1e00000, 2, 255);
  MapGrid_SetCellBehavior(0x1400000, 0x1f00000, 2, 255);
  MapGrid_SetCellBehavior(0x1400000, 0x2000000, 2, 255);
  MapGrid_SetCellBehavior(0x1600000, 0x1d00000, 2, 255);
  MapGrid_SetCellBehavior(0x1600000, 0x1e00000, 2, 255);
  MapGrid_SetCellBehavior(0x1600000, 0x1f00000, 2, 255);
  MapGrid_SetCellBehavior(0x1600000, 0x2000000, 2, 255);
  MapGrid_SetCellBehavior(0x1500000, 0x2100000, 2, 0);
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
  MapGrid_SetCellBehavior(0x1400000, 0x1d00000, 2, 0);
  MapGrid_SetCellBehavior(0x1400000, 0x1e00000, 2, 0);
  MapGrid_SetCellBehavior(0x1400000, 0x1f00000, 2, 0);
  MapGrid_SetCellBehavior(0x1400000, 0x2000000, 2, 0);
  MapGrid_SetCellBehavior(0x1600000, 0x1d00000, 2, 0);
  MapGrid_SetCellBehavior(0x1600000, 0x1e00000, 2, 0);
  MapGrid_SetCellBehavior(0x1600000, 0x1f00000, 2, 0);
  MapGrid_SetCellBehavior(0x1600000, 0x2000000, 2, 0);
  MapGrid_SetCellBehavior(0x1500000, 0x2100000, 2, 255);
  if (open)
    FieldCamera_MoveTo(PIXELS(336), -1, PIXELS(560), 1);
  Func_02000200(mode, id);
  if (open)
    FieldCamera_FollowActor(gMainRoutineWork.currentOwner, 1);
}

// ---- games/tla/src/scene/eastern_alhafra/s0514.c ----
// Scene code of overlay 0x668, in ROM order, from s0514's offset up to the next
// object the overlay links.

#include "field/field.h"

/* VIEW(s0514): routine Func_020044c4 as void Func_020044c4(struct
 * Unk_02004240); eastern_alhafra has void Func_020044c4(struct DoorPlan); tried
 * as the definition's void Func_020044c4(struct PushTarget target), it does not
 * compile in tla-ja: games/tla/src/scene/eastern_alhafra.c:245: conflicting
 * types for `Func_020044c4'. */
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
    Func_080dbdd4(actor16->x, actor16->z, 2, 255);
  if (Func_02004240(&unk) != 0) {
    if (Event_GetFlag(0x8a4) == 0 || facing == 0xc000) {
      Func_020044c4__s0514(unk);
    } else {
      Event_SetFlag(0x8a6);
      Event_Start();
      FieldEvent_ResolvePendingActions(0);
      FieldCamera_MoveTo(PIXELS(336), -1, PIXELS(560), 1);
      Func_020044c4__s0514(unk);
      Func_080dbdd4(actor15->x - PIXELS(24), actor15->z, 0, 0);
      Func_080dbdd4(actor15->x + PIXELS(8), actor15->z, 0, 0);
      Func_080dbdd4(actor15->x - PIXELS(24), actor15->z, 2, 0);
      Func_080dbdd4(actor15->x - PIXELS(8), actor15->z, 2, 0);
      Func_080dbdd4(actor15->x + PIXELS(8), actor15->z, 2, 0);
      Func_080dbdd4(actor15->x + PIXELS(24), actor15->z, 2, 0);
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
      FieldCamera_FollowActor(gMainRoutineWork.currentOwner, 1);
      Event_End();
    }
  }
  if (actor16 != NULL && Event_GetFlag(0x8a4) == 0)
    Func_080dbdd4(actor16->x, actor16->z, 2, 0);
}

void Func_020006ec(s32 mode) {
  s32 tile;

  tile = Func_02004bb4__eastern_alhafra(gMainRoutineWork.currentOwner);
  if (tile == 16) {
    Func_02000394(mode, 16);
    if (Func_02004c44() != 0)
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

void Func_02000958(void) { GameFlag_SetBit(0x8be); }

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

#include "io_reg.h"
#include "dma.h"
#include "map.h"
#include "frame.h"

extern u8 gData_020054f0[];

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

void Func_020040ec();
/* VIEW(eastern_alhafra): routine Func_02004c2c as void Func_02004c2c();
 * module/scene/mars_lighthouse_aerie.h has void Func_02004c2c(void); tried as
 * the definition's void Func_02004c2c(void), which the object already declares,
 * it does not compile in tla-ja: games/tla/src/scene/eastern_alhafra.c:573: too
 * many arguments to function `Func_02004c2c'. */
extern void Func_02004c2c__eastern_alhafra() __asm__("Func_02004c2c");
void Func_020047fc();
/* VIEW(eastern_alhafra): routine Func_02004c94 as void Func_02004c94();
 * module/scene/mars_lighthouse_aerie.h has void Func_02004c94(void); tried as
 * the definition's void Func_02004c94(void), which the object already declares,
 * it does not compile in tla-ja: games/tla/src/scene/eastern_alhafra.c:595: too
 * many arguments to function `Func_02004c94'. */
extern void Func_02004c94__eastern_alhafra() __asm__("Func_02004c94");

s32 Scene_Initialize(void) {
  struct Actor *object;
  s32 flag;

  *(s32 *)(EVENT_WORK + 0x1ac) = 0x100;
  Func_02000af8();
  OBJECT(gMainRoutineWork.currentOwner)->priorityFlags |= 32;
  if (Event_GetFlag(FLAG_ALHAFRA_MAST_CLEARED) != 0) {
    if (Event_GetFlag(0x905) != 0 && Event_GetFlag(0x90f) == 0) {
      Actor_SetPosition(30, PIXELS(184), PIXELS(616));
      Actor_SetPosition(31, PIXELS(136), PIXELS(600));
      Actor_SetPosition(28, PIXELS(168), PIXELS(584));
      Actor_FaceDirection(28, FACING_SOUTH, 0);
      Func_02004c5c(30, 0, 13);
      Call3(Func_02004c5c, 31, 0x8000, 13);
      Func_02004c5c(28, 0, 13);
      Actor_SetPosition(25, PIXELS(112), PIXELS(432));
      Actor_SetPosition(27, PIXELS(176), PIXELS(408));
      Actor_SetPosition(29, PIXELS(152), PIXELS(376));
      Actor_FaceDirection(29, FACING_SOUTH, 0);
      Actor_SetSpriteRotationById(25, 0x4000);
      Actor_SetSingleChildFlags((s32)Actor_Get(25), 0);
      Actor_SetSpriteRotationById(27, 0xc000);
      Actor_SetSingleChildFlags((s32)Actor_Get(27), 0);
      Actor_SetSpriteRotationById(29, 0x4000);
      Actor_SetSingleChildFlags((s32)Actor_Get(29), 0);
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
    Actor_SetSpriteRotationById(30, 0x4000);
    Actor_SetSingleChildFlags(Actor_Get(30), 0);
    Actor_SetSpriteRotationById(31, 0xc000);
    Actor_SetSingleChildFlags(Actor_Get(31), 0);
    Actor_SetSpriteRotationById(28, 0x4000);
    Actor_SetSingleChildFlags(Actor_Get(28), 0);
    FieldActor_SetSpritePriority(27, 2);
    FieldActor_SetSpritePriority(25, 2);
    FieldActor_SetSpritePriority(29, 2);
    Actor_SetPosition(25, PIXELS(160), PIXELS(392));
    Actor_SetPosition(27, PIXELS(176), PIXELS(408));
    Actor_SetPosition(29, PIXELS(152), PIXELS(376));
    Actor_SetSpriteRotationById(27, 0xc000);
    Actor_SetSingleChildFlags(Actor_Get(27), 0);
    Actor_SetSpriteRotationById(29, 0x4000);
    Actor_SetSingleChildFlags(Actor_Get(29), 0);
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
  Func_02004c14();
  Func_02004c2c__eastern_alhafra(0, 0x8a3, 8, 9);
  OBJECT(8)->priorityFlags |= 32;
  OBJECT(9)->priorityFlags |= 32;
  Func_02004c1c();
  Func_02004c24(0, 12, 13);
  Actor_SetSingleChildFlags(Actor_Get(12), 0);
  Actor_SetSingleChildFlags(Actor_Get(13), 0);
  Func_020047fc(gData_02004d0c);
  Actor_SetSingleChildFlags(Actor_Get(14), 0);
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
    Func_02004c24(1, 18, 19);
    Func_02004c94__eastern_alhafra(1);
    object = OBJECT(20);
    object->collisionFlags |= 8;
    object = OBJECT(21);
    object->motionFlags = flag;
    object->y = 0x100000;
    Func_02004c24(2, 20, 21);
  }
  Actor_SetSingleChildFlags(Actor_Get(16), 0);
  Actor_SetSingleChildFlags(Actor_Get(17), 0);
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
  MapGrid_SetCellBehavior(0x1500000, 0x2100000, 2, 0);
  MapGrid_SetCellBehavior(0x1500000, 0x2100000, 0, 0);
  MapGrid_SetCellBehavior(0x1600000, 0x2100000, 0, 0);
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
  MapGrid_SetCellBehavior(FIELD_AT(v_r5, s32, 8), FIELD_AT(v_r5, s32, 16), 0,
                          0);
  Func_02004c84(FIELD_AT(v_r5, s32, 8), FIELD_AT(v_r5, s32, 16), 0, 128);
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
  MapGrid_SetCellBehavior(FIELD_AT(v_r5, s32, 8), FIELD_AT(v_r5, s32, 16), 0,
                          0);
  Func_02004c84(FIELD_AT(v_r5, s32, 8), FIELD_AT(v_r5, s32, 16), 0, 128);
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
  FieldCamera_MoveToActor(22, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(10);
  Actor_RunRepeatedMotion(22, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(22, 0, 10);
  FieldCamera_MoveToActor(23, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(10);
  FieldActor_Jump(23, 6, 15);
  FieldActor_Jump(23, 6, 23);
  FieldEvent_ShowMessageThenWait(23, 0, 10);
  Actor_FaceDirection(22, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(24, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(25, 0x5000, 0);
  FieldEvent_Wait(20);
  FieldCamera_MoveToActor(11, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(23, 0, 10);
  FieldCamera_MoveToActor(24, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(24, 4);
  FieldEvent_ShowMessageThenWait(24, 0, 10);
  Actor_FaceDirection(24, FACING_NORTHEAST, 0);
  FieldEvent_Wait(20);
  FieldCamera_MoveToActor(25, 1);
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
  FieldCamera_MoveToActor(14, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(25, 0, 10);
  FieldCamera_MoveToActor(22, 1);
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
  FieldCamera_MoveToActor(gMainRoutineWork.currentOwner, 1);
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
  FieldCamera_FollowActor(gMainRoutineWork.currentOwner, 1);
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
  result2 = ((s32 (*)())GameFlag_Test)(7);
  if (result2 == 0)
    goto L_0200996c;
  Actor_FaceDirection(4, FACING_NORTH, 0);
  Actor_FaceDirection(5, FACING_NORTHWEST, 0);
  Actor_FaceDirectionAndWait(6, 0x8000);
  FieldEvent_Wait(10);
  goto L_02009a2e;
L_0200996c:
  ((s32 (*)())Func_080d47b4)(26, 0x102, 30);
  FieldEvent_ShowMessageThenWait(26, 0, 10);
  Actor_FaceDirection(4, FACING_NORTH, 0);
  Actor_FaceDirection(5, FACING_NORTHWEST, 0);
  Actor_FaceDirection(6, 0x8000, 0);
  FieldEvent_Wait(30);
  Actor_ShowEmote(26, 0x101, 30);
  Actor_SetAnimationAndWait(26, 4);
  FieldEvent_ShowMessageThenWait(26, 0, 10);
  ((s32 (*)())Func_080d47b4)(26, 0x109, 30);
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

extern const u8 gData_020056d4[];

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
  Func_080d47b4(actor, a, b);
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
  FieldCamera_FollowActor(4, 1);
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
  FieldCamera_FollowActor(25, 1);
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
/// sets flags 0x90f and FLAG_BRIGGS_SAILED_FROM_ALHAFRA and ends by sending the
/// party to SceneId_Ov668Room1, entrance 99.
void Func_02002a08(void) {
  struct MapWork *map;
  s32 i;
  s32 step = 0x10000;

  if (Event_GetFlag(FLAG_ALHAFRA_MAST_CLEARED) == 0)
    return;
  Event_SetFlag(0x90f);
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
  Actor_SetSpriteRotationById(0x1d, 0);
  Actor_SetSingleChildFlags(Actor_GetById(29), 1);
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
  Actor_SetSpriteRotationById(0x1b, 0);
  Actor_SetSingleChildFlags(Actor_GetById(27), 1);
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
  FieldCamera_FollowActor(25, 1);
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
  Actor_SetSpriteRotationById(0x1c, 0);
  Actor_SetSingleChildFlags(Actor_GetById(28), 1);
  FieldActor_Jump(28, 6, 20);
  Actor_ArmCallback(28, FACING_WEST, 0);
  Actor_SetPosition(31, PIXELS(128), PIXELS(608));
  Actor_SetSpriteRotationById(0x1f, 0);
  Actor_SetSingleChildFlags(Actor_GetById(31), 1);
  FieldActor_Jump(31, 6, 20);
  Actor_ArmCallback(31, 0xd000, 0);
  Actor_SetPosition(30, PIXELS(192), PIXELS(616));
  Actor_SetSpriteRotationById(0x1e, 0);
  Actor_SetSingleChildFlags(Actor_GetById(30), 1);
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
  FieldCamera_FollowActor(4, 1);
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
