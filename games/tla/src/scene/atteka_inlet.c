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

void *Scene_GetPlacements(void) {
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

extern s32 gData_02001728[];

s32 *Scene_GetEvents(void) {
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
    (*(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++;
    Event_ShowMessageAndWait(18, 0);
    Actor_SetSpeed(18, 0x19999, 0xcccc);
    Actor_SetModeById(18, 2);
    object = Actor_GetById(gMainRoutineWork.currentOwner);
    if (object != NULL)
      Actor_ResetAndSetPosition(18, *(s16 *)((u8 *)object + 10),
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
    Camera_MoveWithinMapBounds(96 << 16, -1, 308 << 16, 1);
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
    Actor_ArmCallbackAndRefresh(10, FACING_EAST);
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
    Func_020010e0();
    Actor_ShowEmote(23, 264, 0);
    Actor_FaceDirection(23, 0x3000, 20);
    Event_SetMessageId((s32)MsgIVeBeenWaiting);
    Event_ShowMessageAndWait(23, 0);
    Actor_FaceDirectionAndWait(23, 0xb000);
    Event_ShowMessage(0x2017, 0);
    FieldCamera_GetActor()->motionFlags = 0;
    FieldCamera_SetSpeed(0x4cccc, 0x9999);
    FieldCamera_MoveTo(PIXELS(440), PIXELS(32), PIXELS(368), 1);
    Func_020010e0();
    Event_Wait(40);
    FieldCamera_MoveTo(PIXELS(432), PIXELS(32), PIXELS(232), 1);
    Func_020010e0();
    Event_Wait(40);
    Camera_SetSpeedParameters(0x4cccc, 0x9999);
    Camera_MoveWithinMapBounds(PIXELS(520), PIXELS(32), PIXELS(372), 1);
    Func_020010e0();
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
    Actor_ArmCallbackAndRefresh(23, 0x3000);
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
  Actor_ArmCallbackAndRefresh(18, 0x3000);
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
    (*(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++;
    Event_ShowMessageAndWait(18, 0);
    Actor_SetSpeed(18, 0x19999, 0xcccc);
    Actor_SetModeById(18, 2);
    object = Actor_GetById(gMainRoutineWork.currentOwner);
    if (object != NULL)
      Actor_ResetAndSetPosition(18, *(s16 *)((u8 *)object + 10),
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
      Actor_ArmCallbackAndRefresh(gMainRoutineWork.currentOwner, FACING_EAST);
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
  Actor_ArmCallbackAndRefresh(24, angle);
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
  Camera_MoveWithinMapBounds(PIXELS(512), -1, PIXELS(348), 1);
  Actor_WalkToAndWait(24, 504, 460);
  Actor_SetHorizontalPositionWithTerrain(24, 0, 0);
  Actor_ArmCallbackAndRefresh(23, angle);
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

void Func_02000c8c(void) {
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
    Map_CopyCellPictureIndicesRect(54, 47, 4, 15, 10, 4);
    Map_CopyCellPictureIndicesRect(54, 38, 73, 7, 10, 4);
    Map_CopyCellPictureIndicesRect(54, 43, 21, 55, 10, 3);
    Map_CopyCellPictureIndicesRect(65, 43, 21, 20, 10, 3);
    Map_CopyCellPictureIndicesRect(4, 47, 4, 50, 10, 3);
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
  Actor_SetSingleChildFlags(Actor_GetById(17), 0);
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

// clang-format off
u32 gData_02001194[] = {
    0xffff, 0, 0, 0, 0, 0,
};

DEFINE_G_DATA_020011AC(gData_020011ac, 0x980000, 0x1280000, 0x500000, 0x1300000,
    0xd00000, 0x1180000, 1, 0xb80000, 0xe00000, 0x14000)

u32 gData_020012cc[] = {
    0xffff014d, 7, 0x580000, 0, 0x1780000, 0x24000,
    0xffff014d, 7, 0xd80000, 0, 0x1480000, 0x1024000,
    0xffff014d, 7, 0xa80000, 0, 0xb80000, 0x1024000,
    0xffff014d, 7, 0x1e80000, 0, 0x1980000, 0x1024000,
    0xffff014d, 7, 0x2280000, 0, 0x1a80000, 0x1024000,
    0xffff0073, 1, 0xd80000, 0, 0xa80000, 0x12000,
    0xffff0073, 1, 0x370000, 0, 0xea0000, 0x1c000,
    0xffff0074, 1, 0, 0, 0, 0x18000,
    0xffff006a, 1, 0x1040000, 0, 0x9e0000, 0x14000,
    0x190a01a4, 1, 0x1d00000, 0, 0x1080000, 0x20000,
    0xffff0038, 1, 0, 0, 0, 0x4000,
    0xffff00ca, 1, 0x1f80000, 0, 0x1a80000, 0x30000,
    0xffff00ca, 1, 0x2180000, 0, 0x1a80000, 0x38000,
    0xffff00ca, 1, 0x1f80000, 0, 0x1980000, 0x30000,
    0xffff00ca, 1, 0x2180000, 0, 0x1980000, 0x30000,
    0xffff003b, 1, 0x2040000, 0, 0x1680000, 0x35000,
    0xffff004f, 1, 0, 0, 0, 0x4000,
    0xffff, 0, 0, 0, 0, 0,
};

DEFINE_G_DATA_020011AC(gData_0200147c, 0xa20000, 0x12c0000, 0x6f0000, 0x1470000,
    0, 0, 3, 0xcc0000, 0xcc0000, 0x4000)

u32 gData_0200159c[] = {
    0xffff0074, 3, 0x1080000, 0, 0x1600000, 0x8000,
    0xffff004f, 1, 0, 0, 0, 0x4000,
    0xffff004f, 1, 0, 0, 0, 0x4000,
    0xffff00ca, 1, 0x17c0000, 0, 0x1180000, 0x14000,
    0x190a01a4, 1, 0x1d00000, 0, 0x1080000, 0x20000,
    0xffff0149, 1, 0x680000, 0, 0x1880000, 0x24000,
    0x3f00f3, 1, 0, 0, 0, 0x4000,
    0xffff, 0, 0, 0, 0, 0,
};

u32 gData_0200165c[] = {
    0xffff0074, 3, 0, 0, 0, 0x8000,
    0xffff004f, 1, 0, 0, 0, 0x4000,
    0xffff004f, 1, 0, 0, 0, 0x4000,
    0xffff00ca, 1, 0, 0, 0, 0x14000,
    0x190a01a4, 1, 0, 0, 0, 0x20000,
    0xffff0149, 1, 0x680000, 0, 0x1880000, 0x24000,
    0x3f00f3, 1, 0, 0, 0, 0x4000,
    0xffff, 0, 0, 0, 0, 0,
};
// clang-format on

s32 gData_0200171c[] = {
    -1,
    0,
    0,
};

s32 gData_02001728[] = {
    0xc602,
    (s32)0xffff0001,
    (s32)Func_02000c8c,
    1,
    (s32)0xffff0002,
    2,
    1,
    (s32)0xffff0003,
    3,
    2,
    0x9b1000a,
    (s32)Func_020003e4,
    0,
    (s32)0xffff0012,
    (s32)Func_02000120,
    2,
    (s32)0xffff000b,
    (s32)Func_02000640,
    2,
    (s32)0xffff000c,
    (s32)Func_0200077c,
    0,
    0x9d0000d,
    MSGNO(MsgWeVeMade),
    0,
    0x9b2000d,
    MSGNO(MsgHammaReturnedTo),
    0,
    (s32)0xffff000d,
    MSGNO(MsgIVeBeenWatching),
    0x8d15,
    0x9d0000d,
    MSGNO(MsgWeFirstHeard),
    0x8d15,
    0x9b2000d,
    MSGNO(MsgWeVeBeen),
    0x8d15,
    (s32)0xffff000d,
    MSGNO(MsgWheneverILook),
    0,
    0x9d0000e,
    MSGNO(MsgWeMustBe),
    0,
    0x9b2000e,
    MSGNO(MsgWasYourBoat),
    0,
    (s32)0xffff000e,
    MSGNO(MsgIWonderWhat),
    0x8d15,
    0x9d0000e,
    MSGNO(MsgTheseWingsAre),
    0x8d15,
    0x9b2000e,
    MSGNO(MsgWhoBuiltThe),
    0x8d15,
    (s32)0xffff000e,
    MSGNO(MsgIWishI),
    0,
    (s32)0xffff000f,
    MSGNO(MsgWeBroughtThese),
    0x8d15,
    (s32)0xffff000f,
    MSGNO(MsgIDoubtAnyone),
    0,
    0x9d00010,
    MSGNO(MsgIWonderIfThese),
    0,
    0x9b20010,
    MSGNO(MsgAMassiveRock),
    0,
    (s32)0xffff0010,
    MSGNO(MsgYouReGoingNorth),
    0x8d15,
    0x9d00010,
    MSGNO(MsgItSKind),
    0x8d15,
    0x9b20010,
    MSGNO(MsgIfThatBoat),
    0x8d15,
    (s32)0xffff0010,
    MSGNO(MsgIVeDecided),
    0,
    (s32)0xffff0013,
    MSGNO(MsgItSAlmost),
    0x8d15,
    (s32)0xffff0013,
    MSGNO(MsgTheDayHas),
    0,
    (s32)0xffff0014,
    MSGNO(MsgIfEveryoneWill),
    0x8d15,
    (s32)0xffff0014,
    MSGNO(MsgTheAnemosWere),
    0,
    (s32)0xffff0015,
    MSGNO(MsgWhenTheWinged),
    0x8d15,
    (s32)0xffff0015,
    MSGNO(MsgTheseWarriorsWill),
    0,
    (s32)0xffff0016,
    MSGNO(MsgOnceYouRemove),
    0x8d15,
    (s32)0xffff0016,
    MSGNO(MsgIHearThatThe),
    0,
    (s32)0xffff0017,
    MSGNO(MsgUseYourPsynergy),
    0x8d15,
    (s32)0xffff0017,
    MSGNO(MsgTravelSafelyThe),
    -1,
    0,
    0,
};

s32 gData_020018f0[] = {
    1,
    (s32)0xffff0001,
    1,
    1,
    (s32)0xffff0002,
    2,
    0xc401,
    (s32)0xffff0003,
    3,
    0xc401,
    (s32)0xffff0004,
    4,
    1,
    (s32)0xffff0005,
    5,
    1,
    (s32)0xffff0006,
    6,
    2,
    0x9b0000a,
    (s32)Func_02000208,
    0x400,
    (s32)0xffff000e,
    (s32)Func_02000038,
    0,
    0x9b20008,
    MSGNO(MsgEveryoneSWaiting),
    0,
    (s32)0xffff0008,
    MSGNO(MsgAllOfOur),
    0x8d15,
    0x9b20008,
    MSGNO(MsgHammaIsThe),
    0x8d15,
    (s32)0xffff0008,
    MSGNO(MsgEveryoneInContigo),
    0,
    (s32)0xffff0009,
    MSGNO(MsgTheVillagersAre),
    0x8d15,
    (s32)0xffff0009,
    MSGNO(MsgItSAmazing),
    0,
    (s32)0xffff000a,
    MSGNO(MsgHammaWouldnT),
    0x8d15,
    (s32)0xffff000a,
    MSGNO(MsgHammaSaysThat),
    0,
    (s32)0xffff000b,
    MSGNO(MsgWhenPeopleWork),
    0x8d15,
    (s32)0xffff000b,
    MSGNO(MsgWeCanT),
    0x8f15,
    (s32)0xffff000d,
    0,
    0x50008905,
    (s32)0xffff0014,
    (s32)Func_020007ac,
    0x50008905,
    (s32)0xffff0015,
    (s32)Func_0200085c,
    0x50008905,
    (s32)0xffff0016,
    (s32)Func_0200087c,
    0x50008905,
    (s32)0xffff0017,
    (s32)Func_0200089c,
    0x50008905,
    (s32)0xffff0018,
    (s32)Func_020008bc,
    0x50008905,
    (s32)0xffff0019,
    (s32)Func_020008dc,
    0x50008905,
    (s32)0xffff001a,
    (s32)Func_020008fc,
    0x50008905,
    (s32)0xffff001b,
    (s32)Func_0200091c,
    0x50008905,
    (s32)0xffff001c,
    (s32)Func_0200093c,
    0x50008905,
    (s32)0xffff001d,
    (s32)Func_0200095c,
    -1,
    0,
    0,
};
