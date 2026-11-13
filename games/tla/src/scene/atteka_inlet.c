/// @file
/// The scene code of overlay 0x693, in ROM order.

#include "shared/tables.h"
#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "scene_tables.h"
#include "event_inline.h"
#include "main_routine_work.h"
#include "scene_ids.h"
#include "ram_buffer.h"
#include "actor.h"
#include "text/msg_ids.h"
#include "field_event.h"
#include "field_sprite.h"
#include "event_work.h"
#include "module/scene/atteka_inlet.h"
#include "module/scene/scene.h"

void Func_02000038(void) {
  Djinn_JoinParty(14, 0, 15);
  GameFlag_ClearBit(0x300);
}

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov693Room1) {
    if (GameFlag_Test(0x9b2))
      return gData_0200147c;
    if (GameFlag_Test(0x9d0))
      return gData_020012cc;
    return gData_020011ac;
  }
  if (scene == (s32)&SceneId_Ov693Room2) {
    if (GameFlag_Test(0x9d0))
      return gData_0200159c;
    return gData_0200165c;
  }
  return gData_02001194;
}

extern struct Trigger gData_02001728[];

const struct Trigger *Scene_GetEvents(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov693Room1)
    return gData_02001728;
  if (scene == (s32)&SceneId_Ov693Room2)
    return gData_020018f0;
  return gData_0200171c;
}

/// Object 18 asks whether to set out: a yes sets the owner override and, once
/// flag 0x9d0 is set, flags 0x8df and 0x9b2; a no counts the refusal and
/// object 18 walks back to the party leader.
void Func_02000120(void) {
  struct Actor *object;

  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId(MsgAreWeSetting);
  FieldMessage_ShowAtActor(18, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 0) {
    Actor_SetModeById(18, 3);
    Event_ShowMessageAndWait(18, 0);
    gMainRoutineWork.ownerOverride = 1;
    if (Event_GetFlag(0x9d0)) {
      GameFlag_SetBit(0x8df);
      GameFlag_SetBit(0x9b2);
    }
    Import_EventSetExitRequest(4);
  } else {
    (((struct EventWork *)Ram_HeapSlots->eventWork)->messageId)++;
    Event_ShowMessageAndWait(18, 0);
    Actor_SetSpeed(18, 0x19999, 0xcccc);
    Actor_SetModeById(18, 2);
    object = Actor_GetById(gMainRoutineWork.currentOwner);
    if (object != NULL)
      Actor_SetDestinationById(18, *(s16 *)((u8 *)object + 10),
                               *(s16 *)((u8 *)object + 18));
    Actor_WaitForMoveById(18);
    Actor_SetHorizontalPositionWithTerrain(18, 0, 0);
    GameFlag_SetBit(0x201);
    Event_End();
  }
}

/// Once flag 0x9d0 is set and while flag 0x9b2 is clear, objects 9 and 10 are
/// placed, jump in and talk, four cells of the map change and flag 0x9b0 is
/// set.
void Func_02000208(void) {
  if (GameFlag_Test(0x9d0) && !GameFlag_Test(0x9b2)) {
    Event_Start();
    Event_ResolvePendingActions(0);
    FieldCamera_MoveWithinMapBounds(96 << 16, -1, 308 << 16, 1);
    {
      s32 angle = 208;

      Actor_SetPositionAndFacing(9, 60 << 16, 362 << 16, angle << 8);
    }
    wait(1);
    Actor_SetSpeed(9, 0x20000, 0x10000);
    Actor_Jump(9, 6, 0);
    Actor_MoveToAndWait(9, 64, 336);
    Actor_WalkToAndWait(9, 70, 308);
    Actor_ArmCallback(9, FACING_EAST, 0);
    {
      s32 angle = 160;

      Actor_SetPositionAndFacing(10, 94 << 16, 272 << 16, angle << 7);
    }
    wait(1);
    Actor_SetSpeed(10, 0x20000, 0x10000);
    Actor_Jump(10, 6, 0);
    Actor_MoveToAndWait(10, 90, 288);
    Actor_WalkToAndWaitById(10, 80, 316);
    Actor_FaceDirectionById(10, FACING_EAST);
    Actor_GetById(9)->velocityY = 0x40000;
    Event_Wait(10);
    Event_SetMessageId((s32)MsgWhereDoYouThink);
    Event_ShowMessage(0x4009, 0);
    Actor_ShowEmoteById(10, 265, 0);
    Event_ShowMessageAndWait(10, 0);
    Actor_FaceDirection(9, 0x3000, 0);
    Event_ShowMessage(0x4009, 0);
    Actor_FaceDirectionAndWait(10, 0xb000);
    Actor_SetModeById(10, 3);
    Event_ShowMessageAndWait(10, 0);
    Actor_ArmCallback(10, FACING_EAST, 0);
    Actor_ShowEmoteById(9, 264, 0);
    Actor_ArmCallback(9, FACING_EAST, 20);
    Event_ShowMessageAndWait(0x4009, 0);
    Event_ShowMessageAndWait(10, 0);
    Map_CopyCellAttributeRect(1, 16, 1, 1, 3, 19);
    Map_CopyCellAttributeRect(1, 16, 1, 1, 4, 19);
    Map_CopyCellAttributeRect(1, 16, 1, 1, 4, 20);
    Map_CopyCellAttributeRect(1, 16, 1, 1, 4, 21);
    GameFlag_SetBit(0x9b0);
    Event_End();
  }
}

/// Once flag 0x9d0 is set, the camera pans over object 23, who greets the
/// party, turns, talks and walks to its place; flag 0x9b1 is set.
void Func_020003e4(void) {
  if (GameFlag_Test(0x9d0)) {
    Event_Start();
    Event_ResolvePendingActions(0);
    FieldCamera_MoveTo(-1, -1, -1, 0);
    Camera_SetSpeedParameters(0x26666, 0x4ccc);
    FieldCamera_MoveTo(PIXELS(520), -1, PIXELS(372), 1);
    FieldCamera_WaitForArrival();
    Actor_ShowEmote(23, 264, 0);
    Actor_FaceDirection(23, 0x3000, 20);
    Event_SetMessageId((s32)MsgIVeBeenWaiting);
    Event_ShowMessageAndWait(23, 0);
    Actor_FaceDirectionAndWait(23, 0xb000);
    Event_ShowMessage(0x2017, 0);
    FieldCamera_GetActor()->motionFlags = 0;
    FieldCamera_SetSpeed(0x4cccc, 0x9999);
    FieldCamera_MoveTo(PIXELS(440), PIXELS(32), PIXELS(368), 1);
    FieldCamera_WaitForArrival();
    Event_Wait(40);
    FieldCamera_MoveTo(PIXELS(432), PIXELS(32), PIXELS(232), 1);
    FieldCamera_WaitForArrival();
    Event_Wait(40);
    Camera_SetSpeedParameters(0x4cccc, 0x9999);
    FieldCamera_MoveWithinMapBounds(PIXELS(520), PIXELS(32), PIXELS(372), 1);
    FieldCamera_WaitForArrival();
    Event_Wait(20);
    Actor_FaceDirectionAndWait(23, 0x3000);
    Motion_SetModeAndWaitAnimation(23, 3);
    Event_ShowMessageAndWait(23, 0);
    Actor_ShowEmote(23, 265, 20);
    Event_ShowMessageAndWait(23, 0);
    Actor_SetModeById(23, 4);
    Event_ShowMessageAndWait(23, 0);
    Event_ShowMessageAndWait(23, 0);
    Actor_FaceDirectionAndWait(23, 0xb000);
    Event_ShowMessageAndWait(0x2017, 0);
    Actor_FaceDirectionAndWait(23, 0x3000);
    Event_ShowMessageAndWait(23, 0);
    Motion_SetModeAndWaitAnimation(23, 3);
    Event_ShowMessageAndWait(23, 0);
    Actor_SetModeById(23, 4);
    Event_ShowMessageAndWait(23, 0);
    Actor_ShowEmoteById(gMainRoutineWork.currentOwner, 257, 40);
    Actor_ShowEmoteById(23, 265, 20);
    Event_ShowMessageAndWait(23, 0);
    Actor_ShowEmoteById(23, 264, 20);
    Event_ShowMessageAndWait(23, 0);
    Motion_SetModeAndWaitAnimation(23, 3);
    Event_ShowMessageAndWait(23, 0);
    Actor_SetSpeedParameters(23, 0xcccc, 0x6666);
    Actor_GetById(23)->actionFlags &= ~1;
    Actor_WalkToAndWaitById(23, 504, 360);
    Event_Wait(1);
    Actor_GetById(23)->actionFlags |= 1;
    Actor_FaceDirectionById(23, 0x3000);
    Motion_SetModeAndWaitAnimation(23, 3);
    Event_ShowMessageAndWait(23, 0);
    GameFlag_SetBit(0x9b1);
    Event_End();
  }
}

void Actor_MoveToObject(s32 id, s32 target);

/// Object 18 comes to the party leader and asks whether to set out, as in
/// Func_02000120; after a no the leader steps back.
void Func_02000640(void) {
  struct Actor *object;

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_MoveToObject(18, gMainRoutineWork.currentOwner);
  wait(1);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_NORTHWEST, 0);
  Actor_SetSpeed(18, 0xcccc, 0x6666);
  Actor_WalkToAndWaitById(18, 532, 248);
  Actor_FaceDirectionById(18, 0x3000);
  Event_SetMessageId(MsgAreWeSetting);
  FieldMessage_ShowAtActor(18, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 0) {
    Actor_SetModeById(18, 3);
    Event_ShowMessageAndWait(18, 0);
    gMainRoutineWork.ownerOverride = 1;
    if (Event_GetFlag(0x9d0)) {
      GameFlag_SetBit(0x8df);
      GameFlag_SetBit(0x9b2);
    }
    Import_EventSetExitRequest(4);
  } else {
    (((struct EventWork *)Ram_HeapSlots->eventWork)->messageId)++;
    Event_ShowMessageAndWait(18, 0);
    Actor_SetSpeed(18, 0x19999, 0xcccc);
    Actor_SetModeById(18, 2);
    object = Actor_GetById(gMainRoutineWork.currentOwner);
    if (object != NULL)
      Actor_SetDestinationById(18, *(s16 *)((u8 *)object + 10),
                               *(s16 *)((u8 *)object + 18));
    Actor_WaitForMoveById(18);
    Actor_SetHorizontalPositionWithTerrain(18, 0, 0);
    Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
    Actor_WalkToAndWaitById(gMainRoutineWork.currentOwner, 528, 276);
    Event_End();
  }
}

void Func_02001120(s32 srcX, s32 srcY, s32 width, s32 height, s32 destX,
                   s32 destY, s32 layer);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static inline void MapCopy(s32 srcX, s32 srcY, s32 width, s32 height, s32 destX,
                           s32 destY, s32 layer) {
  Func_02001120(srcX, srcY, width, height, destX, destY, layer);
}

void Func_0200077c(void) {
  if (GameFlag_Test(0x820) && GameFlag_Test(0x9b1) && !GameFlag_Test(0x9ad))
    Func_0200097c();
}

void Func_020007ac(void) {
  struct Actor *object;
  s32 moved;

  MapCopy(9, 23, 1, 1, 7, 23, 0);
  if (!GameFlag_Test(63)) {
    object = Actor_GetById(gMainRoutineWork.currentOwner);
    Event_Start();
    Event_ResolvePendingActions(0);
    moved = 0;
    if ((object->x >> 20) == 7 && (object->z >> 20) == 23) {
      Actor_Jump(gMainRoutineWork.currentOwner, 6, 0);
      Actor_SetDestination(gMainRoutineWork.currentOwner, 104, 376);
      moved = 1;
    }
    Actor_SetHorizontalPositionWithTerrain(14, 120 << 16, 376 << 16);
    wait(1);
    Actor_Jump(14, 4, 10);
    GameFlag_SetBit(0x300);
    if (moved)
      Actor_FaceDirectionById(gMainRoutineWork.currentOwner, FACING_EAST);
    Event_End();
  }
}

void Func_0200085c(void) { MapCopy(9, 23, 1, 1, 4, 10, 0); }

void Func_0200087c(void) { MapCopy(9, 23, 1, 1, 6, 9, 0); }

void Func_0200089c(void) { MapCopy(9, 23, 1, 1, 6, 6, 0); }

void Func_020008bc(void) { MapCopy(12, 5, 1, 1, 12, 6, 0); }

void Func_020008dc(void) { MapCopy(9, 23, 1, 1, 13, 7, 0); }

void Func_020008fc(void) { MapCopy(9, 23, 1, 1, 14, 6, 0); }

void Func_0200091c(void) { MapCopy(9, 23, 1, 1, 21, 6, 0); }

void Func_0200093c(void) { MapCopy(9, 23, 1, 1, 22, 6, 0); }

void Func_0200095c(void) { MapCopy(9, 23, 1, 1, 23, 7, 0); }

/// Object 24 calls out to the party, jumps down to it and talks with object
/// 23; flag 0x9ad is set.
void Func_0200097c(void) {
  s32 angle;

  Event_Start();
  Event_ResolvePendingActions(0);
  angle = 0xd000;
  Actor_SetHorizontalPositionAndAngleWithTerrain(24, PIXELS(504), PIXELS(460),
                                                 angle);
  wait(1);
  Event_SetMessageId((s32)MsgHeyWait2);
  Event_ShowMessageAndWait(24, 0);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_SOUTH, 0);
  Actor_FaceDirection(23, 0x3000, 0);
  FieldCamera_SetSpeed(0x19999, 0x3333);
  FieldCamera_GetActor()->motionFlags = 0;
  FieldCamera_MoveTo(PIXELS(512), -1, PIXELS(380), 1);
  Actor_SetSpeed(24, 0x19999, 0xcccc);
  Actor_WalkToAndWait(24, 520, 432);
  Actor_WalkToAndWait(24, 520, 388);
  Actor_FaceDirection(23, 0x3000, 0);
  Event_ShowMessageAndWait(24, 0);
  Actor_ShowEmoteById(23, 256, 10);
  Event_ShowMessageAndWait(23, 0);
  Actor_ShowEmote(24, 265, 0);
  Event_ShowMessageAndWait(24, 0);
  Motion_SetModeAndWaitAnimation(23, 3);
  Actor_FaceDirectionById(24, angle);
  Actor_GetById(24)->actionFlags &= ~1;
  Actor_WalkToAndWait(24, 520, 384);
  Event_Wait(1);
  Actor_GetById(24)->actionFlags |= 1;
  Motion_SetVarCbAndRefresh(24, 2);
  Actor_SetHorizontalPositionWithTerrain(65, PIXELS(520), PIXELS(380));
  wait(1);
  Actor_GetById(24)->actionFlags &= ~1;
  Actor_WalkToAndWait(24, 520, 388);
  Event_Wait(1);
  Actor_GetById(24)->actionFlags |= 1;
  Event_Wait(10);
  Actor_FaceDirection(24, 0xb000, 10);
  Motion_SetModeAndWaitAnimation(24, 3);
  Actor_WalkToAndWait(24, 520, 434);
  FieldCamera_MoveWithinMapBounds(PIXELS(512), -1, PIXELS(348), 1);
  Actor_WalkToAndWait(24, 504, 460);
  Actor_SetHorizontalPositionWithTerrain(24, 0, 0);
  Actor_FaceDirectionById(23, angle);
  Event_ShowMessageAndWait(23, 0);
  Actor_ShowEmoteById(23, 265, 0);
  Event_ShowMessageAndWait(23, 0);
  Actor_SetModeById(23, 3);
  Event_ShowMessageAndWait(23, 0);
  Motion_SetModeAndWaitAnimation(23, 3);
  Actor_ShowEmoteById(23, 264, 0);
  Event_ShowMessageAndWait(23, 0);
  GameFlag_SetBit(0x9ad);
  Event_End();
}

void Func_02000bb8(void) {
  s32 variant =
      ((struct FieldSprite *)Actor_GetById(gMainRoutineWork.currentOwner)
           ->sprite)
          ->priority;

  Actor_SetSpritePriority(16, variant);
  Actor_SetSpritePriority(8, variant);
  Actor_SetSpritePriority(10, variant);
}

void Func_02000bf0(void) {
  Actor_SetSpritePriority(
      13, ((struct FieldSprite *)Actor_GetById(gMainRoutineWork.currentOwner)
               ->sprite)
              ->priority);
}

/// Rotates palette colours 5-9 and 13-15 by one entry every eight frames.
void Func_02000c14(void) {
  if ((*Ram_FrameCount & 7) == 0) {
    volatile u16 *palette;
    s16 saved;
    s32 index;

    saved = *(volatile u16 *)0x05000172;
    index = 9;
  first:
    palette = (u16 *)0x05000160;
    palette[index] = (palette - 1)[index];
    index--;
    if (index != 4)
      goto first;
    *(volatile u16 *)0x05000168 = saved;
  }
  if ((*Ram_FrameCount & 7) == 0) {
    volatile u16 *palette;
    s16 saved;
    s32 index;

    saved = *(volatile u16 *)0x0500017e;
    index = 15;
  second:
    palette = (u16 *)0x05000160;
    palette[index] = (palette - 1)[index];
    index--;
    if (index != 12)
      goto second;
    *(volatile u16 *)0x05000178 = saved;
  }
}

/// Stepping into a door: it opens, the leader walks through and the scene
/// leaves by the exit given.
void AttekaInlet_OpenDoorAndEnter(void) {
  u8 *work = Ram_HeapSlots->eventWork;

  Event_Start();
  Event_ResolvePendingActions(0);
  Sound_Play(158);
  MapGrid_CopyCellsTo(51, 38, 70, 22, 1, 2);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 2);
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x8000, 0x4000);
  Actor_CenterAndWalk(gMainRoutineWork.currentOwner, 2, -4);
  Event_Wait(8);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(*(s16 *)(work + 340));
}

s32 Scene_Initialize(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov693Room1)
    Func_02000dc4();
  else if (scene == (s32)&SceneId_Ov693Room2)
    Func_02000ed0();
  return 0;
}

s32 Scene_PrepareMap(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov693Room1 && GameFlag_Test(0x9d0)) {
    MapGrid_CopyCellPictureIndicesRect(54, 47, 4, 15, 10, 4);
    MapGrid_CopyCellPictureIndicesRect(54, 38, 73, 7, 10, 4);
    MapGrid_CopyCellPictureIndicesRect(54, 43, 21, 55, 10, 3);
    MapGrid_CopyCellPictureIndicesRect(65, 43, 21, 20, 10, 3);
    MapGrid_CopyCellPictureIndicesRect(4, 47, 4, 50, 10, 3);
  }
  return 0;
}

static __inline__ void Task_Add_(void (*func)(void), s32 priority) {
  /* FAKEMATCH(wrapper): the call forwarded through an inline, which keeps each
   * call's own constants, as The Broken Seal's scene layer does; as direct
   * calls to Task_Add, 28 of 107 instructions of Func_02000dc4 differ in
   * tla-en. */
  Task_Add(func, priority);
}

void Func_02000dc4(void) {
  struct MapBits *map = (struct MapBits *)Ram_HeapSlots->mapWork;
  struct MainRoutineWork *party;

  GameFlag_SetBit(0x9b4);
  if (!GameFlag_Test(0x9ad))
    Actor_SetHorizontalPositionWithTerrain(65, 0, 0);
  map->mode = 1;
  party = &gMainRoutineWork;
  Actor_GetById(party->currentOwner)->priorityFlags |= 32;
  Task_Add_(Func_02000c14, 0x480);
  Task_Add_(Func_02000bb8, 0x480);
  Actor_SetSpriteFlags(Actor_GetById(17), 0);
  *((u8 *)Actor_GetById(17) + 85) = 0;
  if (GameFlag_Test(0x9b2)) {
    Actor_SetHorizontalPositionWithTerrain(19, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(20, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(21, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(22, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(23, 0, 0);
  } else {
    if (GameFlag_Test(0x9b1)) {
      s32 speed = 0xc0;

      Actor_SetPositionAndFacing(23, PIXELS(504), PIXELS(360), speed << 6);
    }
    Actor_WatchAndSetScript(19, party->currentOwner);
    Actor_WatchAndSetScript(20, party->currentOwner);
    Actor_WatchAndSetScript(21, party->currentOwner);
    Actor_WatchAndSetScript(22, party->currentOwner);
  }
}

void Func_02000ed0(void) {
  if (gMainRoutineWork.entrance == 3)
    GameFlag_ClearBit(0x12f);
  if (GameFlag_Test(0x9b2)) {
    Actor_SetHorizontalPositionWithTerrain(9, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(10, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(11, 0, 0);
  } else if (GameFlag_Test(0x9b0)) {
    Actor_SetPositionAndFacing(9, 70 << 16, 308 << 16, FACING_EAST);
    Actor_SetPositionAndFacing(10, 80 << 16, 316 << 16, FACING_EAST);
    Map_CopyCellAttributeRect(1, 16, 1, 1, 3, 19);
    Map_CopyCellAttributeRect(1, 16, 1, 1, 4, 19);
    Map_CopyCellAttributeRect(1, 16, 1, 1, 4, 20);
    Map_CopyCellAttributeRect(1, 16, 1, 1, 4, 21);
    wait(1);
  }
  if (GameFlag_Test(0x300))
    Actor_SetPosition(14, 120 << 16, 376 << 16);
  {
    s32 priority = 0x480;

    Task_Add(Func_02000bf0, priority);
  }
}

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 72, 0, 295, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(234),      SCENE_EXIT(1, 40, 1),  SCENE_EXIT(2, 235, 1),
    SCENE_EXIT(3, 235, 5), SCENE_EXIT(4, 2, 52),  SCENE_EXITS(235),
    SCENE_EXIT(1, 234, 2), SCENE_EXIT(2, 2, 53),  SCENE_EXIT(3, 237, 21),
    SCENE_EXIT(4, 235, 4), SCENE_EXIT(5, 234, 3), SCENE_EXIT(6, 2, 53),
    SCENE_EXITS_END,
};

struct ActorPlacement gData_02001194[] = {
    {SCENE_TABLE_END},
};

DEFINE_G_DATA_020011AC(gData_020011ac, 0x980000, 0x1280000, 0x500000, 0x1300000,
                       0xd00000, 0x1180000, 1, 0xb80000, 0xe00000, 0x14000)

struct ActorPlacement gData_020012cc[] = {
    {333, CONDITION_ALWAYS, 7, PIXELS(88), 0, PIXELS(376), FACING_SOUTH, 2, 0},
    {333, CONDITION_ALWAYS, 7, PIXELS(216), 0, PIXELS(328), FACING_SOUTH, 2, 1},
    {333, CONDITION_ALWAYS, 7, PIXELS(168), 0, PIXELS(184), FACING_SOUTH, 2, 1},
    {333, CONDITION_ALWAYS, 7, PIXELS(488), 0, PIXELS(408), FACING_SOUTH, 2, 1},
    {333, CONDITION_ALWAYS, 7, PIXELS(552), 0, PIXELS(424), FACING_SOUTH, 2, 1},
    {115, CONDITION_ALWAYS, 1, PIXELS(216), 0, PIXELS(168), FACING_SOUTHEAST, 1,
     0},
    {115, CONDITION_ALWAYS, 1, PIXELS(55), 0, PIXELS(234), FACING_NORTH, 1, 0},
    {116, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_WEST, 1, 0},
    {106, CONDITION_ALWAYS, 1, PIXELS(260), 0, PIXELS(158), FACING_SOUTH, 1, 0},
    {420, IF_FLAG_SET(0x90a), 1, PIXELS(464), 0, PIXELS(264), FACING_EAST, 2,
     0},
    {56, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 0, 0},
    {202, CONDITION_ALWAYS, 1, PIXELS(504), 0, PIXELS(424), FACING_EAST, 3, 0},
    {202, CONDITION_ALWAYS, 1, PIXELS(536), 0, PIXELS(424), FACING_WEST, 3, 0},
    {202, CONDITION_ALWAYS, 1, PIXELS(504), 0, PIXELS(408), FACING_EAST, 3, 0},
    {202, CONDITION_ALWAYS, 1, PIXELS(536), 0, PIXELS(408), FACING_EAST, 3, 0},
    {59, CONDITION_ALWAYS, 1, PIXELS(516), 0, PIXELS(360),
     FACING_SOUTH + FACING_STEP, 3, 0},
    {79, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 0, 0},
    {SCENE_TABLE_END},
};

DEFINE_G_DATA_020011AC(gData_0200147c, 0xa20000, 0x12c0000, 0x6f0000, 0x1470000,
                       0, 0, 3, 0xcc0000, 0xcc0000, 0x4000)

struct ActorPlacement gData_0200159c[] = {
    {116, CONDITION_ALWAYS, 3, PIXELS(264), 0, PIXELS(352), FACING_WEST, 0, 0},
    {79, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 0, 0},
    {79, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 0, 0},
    {202, CONDITION_ALWAYS, 1, PIXELS(380), 0, PIXELS(280), FACING_SOUTH, 1, 0},
    {420, IF_FLAG_SET(0x90a), 1, PIXELS(464), 0, PIXELS(264), FACING_EAST, 2,
     0},
    {329, CONDITION_ALWAYS, 1, PIXELS(104), 0, PIXELS(392), FACING_SOUTH, 2, 0},
    {243, 0x3f, 1, 0, 0, 0, FACING_SOUTH, 0, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_0200165c[] = {
    {116, CONDITION_ALWAYS, 3, 0, 0, 0, FACING_WEST, 0, 0},
    {79, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 0, 0},
    {79, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 0, 0},
    {202, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 1, 0},
    {420, IF_FLAG_SET(0x90a), 1, 0, 0, 0, FACING_EAST, 2, 0},
    {329, CONDITION_ALWAYS, 1, PIXELS(104), 0, PIXELS(392), FACING_SOUTH, 2, 0},
    {243, 0x3f, 1, 0, 0, 0, FACING_SOUTH, 0, 0},
    {SCENE_TABLE_END},
};

struct Trigger gData_0200171c[] = {
    {TRIGGERS_END},
};

struct Trigger gData_02001728[] = {
    {0xc602, 1, CONDITION_ALWAYS, EVENT(AttekaInlet_OpenDoorAndEnter)},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_STEP, 10, 0x9b1, EVENT(Func_020003e4)},
    {TRIGGER_TALK, 18, CONDITION_ALWAYS, EVENT(Func_02000120)},
    {TRIGGER_STEP, 11, CONDITION_ALWAYS, EVENT(Func_02000640)},
    {TRIGGER_STEP, 12, CONDITION_ALWAYS, EVENT(Func_0200077c)},
    {TRIGGER_TALK, 13, 0x9d0, MSGNO(MsgWeVeMade)},
    {TRIGGER_TALK, 13, 0x9b2, MSGNO(MsgHammaReturnedTo)},
    {TRIGGER_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgIVeBeenWatching)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, 0x9d0, MSGNO(MsgWeFirstHeard)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, 0x9b2, MSGNO(MsgWeVeBeen)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS,
     MSGNO(MsgWheneverILook)},
    {TRIGGER_TALK, 14, 0x9d0, MSGNO(MsgWeMustBe)},
    {TRIGGER_TALK, 14, 0x9b2, MSGNO(MsgWasYourBoat)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgIWonderWhat)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, 0x9d0, MSGNO(MsgTheseWingsAre)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, 0x9b2, MSGNO(MsgWhoBuiltThe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS,
     MSGNO(MsgIWishI)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgWeBroughtThese)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS,
     MSGNO(MsgIDoubtAnyone)},
    {TRIGGER_TALK, 16, 0x9d0, MSGNO(MsgIWonderIfThese)},
    {TRIGGER_TALK, 16, 0x9b2, MSGNO(MsgAMassiveRock)},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgYouReGoingNorth)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, 0x9d0, MSGNO(MsgItSKind)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, 0x9b2, MSGNO(MsgIfThatBoat)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS,
     MSGNO(MsgIVeDecided)},
    {TRIGGER_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgItSAlmost)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS,
     MSGNO(MsgTheDayHas)},
    {TRIGGER_TALK, 20, CONDITION_ALWAYS, MSGNO(MsgIfEveryoneWill)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS,
     MSGNO(MsgTheAnemosWere)},
    {TRIGGER_TALK, 21, CONDITION_ALWAYS, MSGNO(MsgWhenTheWinged)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS,
     MSGNO(MsgTheseWarriorsWill)},
    {TRIGGER_TALK, 22, CONDITION_ALWAYS, MSGNO(MsgOnceYouRemove)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS,
     MSGNO(MsgIHearThatThe)},
    {TRIGGER_TALK, 23, CONDITION_ALWAYS, MSGNO(MsgUseYourPsynergy)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 23, CONDITION_ALWAYS,
     MSGNO(MsgTravelSafelyThe)},
    {TRIGGERS_END},
};

struct Trigger gData_020018f0[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {0xc401, 3, CONDITION_ALWAYS, 3},
    {0xc401, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_STEP, 10, 0x9b0, EVENT(Func_02000208)},
    {0x400, 14, CONDITION_ALWAYS, EVENT(Func_02000038)},
    {TRIGGER_TALK, 8, 0x9b2, MSGNO(MsgEveryoneSWaiting)},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgAllOfOur)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, 0x9b2, MSGNO(MsgHammaIsThe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS,
     MSGNO(MsgEveryoneInContigo)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgTheVillagersAre)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS,
     MSGNO(MsgItSAmazing)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgHammaWouldnT)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS,
     MSGNO(MsgHammaSaysThat)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgWhenPeopleWork)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS,
     MSGNO(MsgWeCanT)},
    {PSYNERGY_ON_ACTOR(ABILITY_LIFT), 13, CONDITION_ALWAYS, 0},
    {0x50008905, 20, CONDITION_ALWAYS, EVENT(Func_020007ac)},
    {0x50008905, 21, CONDITION_ALWAYS, EVENT(Func_0200085c)},
    {0x50008905, 22, CONDITION_ALWAYS, EVENT(Func_0200087c)},
    {0x50008905, 23, CONDITION_ALWAYS, EVENT(Func_0200089c)},
    {0x50008905, 24, CONDITION_ALWAYS, EVENT(Func_020008bc)},
    {0x50008905, 25, CONDITION_ALWAYS, EVENT(Func_020008dc)},
    {0x50008905, 26, CONDITION_ALWAYS, EVENT(Func_020008fc)},
    {0x50008905, 27, CONDITION_ALWAYS, EVENT(Func_0200091c)},
    {0x50008905, 28, CONDITION_ALWAYS, EVENT(Func_0200093c)},
    {0x50008905, 29, CONDITION_ALWAYS, EVENT(Func_0200095c)},
    {TRIGGERS_END},
};
