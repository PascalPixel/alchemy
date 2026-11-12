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

void Func_02000038(void) {
  Func_080d7788(14, 0, 15);
  GameFlag_ClearBit(0x300);
}

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern u8 gData_02001194[];
extern u8 gData_020011ac[];
extern u8 gData_020012cc[];
extern u8 gData_0200147c[];
extern u8 gData_0200159c[];
extern u8 gData_0200165c[];

void *Func_02000064(void) {
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

extern const u8 gData_0200171c[];
extern const u8 gData_02001728[];
extern const u8 gData_020018f0[];

const u8 *Func_020000e0(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov693Room1)
    return gData_02001728;
  if (scene == (s32)&SceneId_Ov693Room2)
    return gData_020018f0;
  return gData_0200171c;
}

void Func_02001010(void);
void Func_02001018(void);

/// Object 18 asks whether to set out: a yes sets the owner override and, once
/// flag 0x9d0 is set, flags 0x8df and 0x9b2; a no counts the refusal and
/// object 18 walks back to the party leader.
void Func_02000120(void) {
  struct Actor *object;

  Func_02001010();
  Event_ResolvePendingActions(0);
  Event_SetMessageId(MsgAreWeSetting);
  FieldMessage_ShowAtActor(18, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 0) {
    Actor_SetModeById(18, 3);
    Event_ShowMessageAndWait(18, 0);
    gMainRoutineWork.ownerOverride = 1;
    /* FAKEMATCH(wrapper): the flag test forwarded through event.h's
     * Event_GetFlag; as a direct GameFlag_Test call, gMainRoutineWork's offsets
     * take r3 instead of r2, 22 of 89 instructions differ in tla-en (found
     * by alchemy permute). */
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
    Func_02001018();
  }
}

/// Once flag 0x9d0 is set and while flag 0x9b2 is clear, objects 9 and 10 are
/// placed, jump in and talk, four cells of the map change and flag 0x9b0 is
/// set.
void Func_02000208(void) {
  if (GameFlag_Test(0x9d0) && !GameFlag_Test(0x9b2)) {
    Func_02001010();
    Event_ResolvePendingActions(0);
    Camera_MoveWithinMapBounds(96 << 16, -1, 308 << 16, 1);
    {
      /* FAKEMATCH(temporary): the angle shifted from a local, so it is built
       * first, as the ROM builds it; as a constant, it is built last, 2 of
       * 195 instructions differ in tla-en. */
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
      /* FAKEMATCH(temporary): the angle shifted from a local, as above; as a
       * constant, 2 of 195 instructions differ in tla-en. */
      s32 angle = 160;

      Actor_SetPositionAndFacing(10, 94 << 16, 272 << 16, angle << 7);
    }
    wait(1);
    Actor_SetSpeed(10, 0x20000, 0x10000);
    Actor_Jump(10, 6, 0);
    Actor_MoveToAndWait(10, 90, 288);
    Actor_SetPositionAndReset(10, 80, 316);
    Actor_ArmCallbackAndRefresh(10, FACING_EAST);
    Actor_GetById(9)->velocityY = 0x40000;
    Event_Wait(10);
    Event_SetMessageId((s32)MsgWhereDoYouThink);
    Event_ShowMessage(0x4009, 0);
    Func_080d47b4(10, 265, 0);
    Event_ShowMessageAndWait(10, 0);
    Actor_FaceDirection(9, 0x3000, 0);
    Event_ShowMessage(0x4009, 0);
    Actor_FaceDirectionAndWait(10, 0xb000);
    Actor_SetModeById(10, 3);
    Event_ShowMessageAndWait(10, 0);
    Actor_ArmCallback(10, FACING_EAST, 0);
    Func_080d47b4(9, 264, 0);
    Actor_ArmCallback(9, FACING_EAST, 20);
    Event_ShowMessageAndWait(0x4009, 0);
    Event_ShowMessageAndWait(10, 0);
    Map_CopyCellAttributeRect(1, 16, 1, 1, 3, 19);
    Map_CopyCellAttributeRect(1, 16, 1, 1, 4, 19);
    Map_CopyCellAttributeRect(1, 16, 1, 1, 4, 20);
    Map_CopyCellAttributeRect(1, 16, 1, 1, 4, 21);
    GameFlag_SetBit(0x9b0);
    Func_02001018();
  }
}

void Func_020010e0(void);

/// Once flag 0x9d0 is set, the camera pans over object 23, who greets the
/// party, turns, talks and walks to its place; flag 0x9b1 is set.
void Func_020003e4(void) {
  if (GameFlag_Test(0x9d0)) {
    Func_02001010();
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
    Func_080d4568()->motionFlags = 0;
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
    Func_080d47b4(gMainRoutineWork.currentOwner, 257, 40);
    Func_080d47b4(23, 265, 20);
    Event_ShowMessageAndWait(23, 0);
    Func_080d47b4(23, 264, 20);
    Event_ShowMessageAndWait(23, 0);
    Motion_SetModeAndWaitAnimation(23, 3);
    Event_ShowMessageAndWait(23, 0);
    Actor_SetSpeedParameters(23, 0xcccc, 0x6666);
    Actor_GetById(23)->actionFlags &= ~1;
    Actor_SetPositionAndReset(23, 504, 360);
    Event_Wait(1);
    Actor_GetById(23)->actionFlags |= 1;
    Actor_ArmCallbackAndRefresh(23, 0x3000);
    Motion_SetModeAndWaitAnimation(23, 3);
    Event_ShowMessageAndWait(23, 0);
    GameFlag_SetBit(0x9b1);
    Func_02001018();
  }
}

void Func_02001070(s32 id, s32 target);

/// Object 18 comes to the party leader and asks whether to set out, as in
/// Func_02000120; after a no the leader steps back.
void Func_02000640(void) {
  struct Actor *object;

  Func_02001010();
  Event_ResolvePendingActions(0);
  Func_02001070(18, gMainRoutineWork.currentOwner);
  wait(1);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_NORTHWEST, 0);
  Actor_SetSpeed(18, 0xcccc, 0x6666);
  Actor_SetPositionAndReset(18, 532, 248);
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
    Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 528, 276);
    Func_02001018();
  }
}

void Func_0200097c(void);
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
  struct FieldActor *object;
  s32 moved;

  MapCopy(9, 23, 1, 1, 7, 23, 0);
  if (!GameFlag_Test(63)) {
    object =
        ((struct FieldActor *)Actor_GetById(gMainRoutineWork.currentOwner));
    Func_02001010();
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
    Func_02001018();
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

  Func_02001010();
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
  Func_080d4568()->motionFlags = 0;
  FieldCamera_MoveTo(PIXELS(512), -1, PIXELS(380), 1);
  Actor_SetSpeed(24, 0x19999, 0xcccc);
  Actor_WalkToAndWait(24, 520, 432);
  Actor_WalkToAndWait(24, 520, 388);
  Actor_FaceDirection(23, 0x3000, 0);
  Event_ShowMessageAndWait(24, 0);
  Func_080d47b4(23, 256, 10);
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
  Func_080d47b4(23, 265, 0);
  Event_ShowMessageAndWait(23, 0);
  Actor_SetModeById(23, 3);
  Event_ShowMessageAndWait(23, 0);
  Motion_SetModeAndWaitAnimation(23, 3);
  Func_080d47b4(23, 264, 0);
  Event_ShowMessageAndWait(23, 0);
  GameFlag_SetBit(0x9ad);
  Func_02001018();
}

void Func_02000bb8(void) {
  s32 variant =
      ((struct FieldActor *)Actor_GetById(gMainRoutineWork.currentOwner))
          ->sprite->priority;

  Actor_SetSpritePriority(16, variant);
  Actor_SetSpritePriority(8, variant);
  Actor_SetSpritePriority(10, variant);
}

void Func_02000bf0(void) {
  Actor_SetSpritePriority(
      13, ((struct FieldActor *)Actor_GetById(gMainRoutineWork.currentOwner))
              ->sprite->priority);
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

  Func_02001010();
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

void Func_02000dc4(void);
void Func_02000ed0(void);

s32 Func_02000d08(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov693Room1)
    Func_02000dc4();
  else if (scene == (s32)&SceneId_Ov693Room2)
    Func_02000ed0();
  return 0;
}

s32 Func_02000d3c(void) {
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

struct MapBits {
  u8 unk00[23];
  u8 low : 2;
  u8 mode : 2;
};

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
