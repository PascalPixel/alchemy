/// @file
/// The cave by the Shaman Village: overgrowth Whirlwind clears, a pillar Move
/// pushes into a gap, rocks Lift raises, Eddy the Mercury Djinni who runs from
/// the party, and in a linked game the three gladiators from Colosso.

#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "lib.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "scene_tables.h"
#include "actor.h"
#include "system/main_routine_work.h"
#include "system/ram_buffer.h"
#include "event_work.h"
#include "field/event_inline.h"
#include "module/scene/scene.h"
#include "field_sprite.h"
#include "call.h"
#include "scene_ids.h"
#include "edition.h"
#include "version.h"
#include "text/msg_ids.h"

/// What games/tla/src/scene/ declares for the files that use it, written once.

extern u32 gShamanVillageCave_GladiatorLeaves[];

extern s32 gShamanVillageCave_GladiatorFollowsOut[];

void Field_TryJumpForward();

extern s32 gShamanVillageCave_OvergrowthRests[];
extern u32 gShamanVillageCave_Settle[];

void FieldCamera_WaitForArrival();

void WorldMap_SetReturnLeavingScene();
s32 Event_PickUpItem();

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

/// Whirlwind blows away one of the two overgrowths (actors 8 and 10) for good,
/// opening the cell behind it (flags 0x9a9 and 0x9aa).
void ShamanVillageCave_BlowAwayOvergrowth(s32 arg0, s32 arg1) {
  if (arg1 != 8)
    goto notFirst;
  Event_SetFlag(0x9a9);
  MapGrid_CopyCellAttributes(0, 2, 1, 1, 2, arg1);
notFirst:
  if (arg1 != 10)
    goto done;
  Event_SetFlag(0x9aa);
  MapGrid_CopyCellAttributes(0, 32, 1, 1, 14, 38);
done:
  return;
}

struct ShamanVillageCaveEventWork {
  u8 unk000[0x154];
  s16 value154;
};

void Field_RequestBattleFromEncounterTable(s32 a, s32 b);

void ShamanVillageCave_DropPillarIntoGap(s32 id);

/// The pillar falls until it lands, or for a second, then vanishes into the
/// gap it fills.
void ShamanVillageCave_DropPillarIntoGap(s32 id) {
  struct Actor *object = Actor_Get(id);
  s32 frames = 60;

  object->motionFlags = 3;
  while (frames != 0) {
    System_Wait(1);
    if (object->velocityY == 0)
      break;
    frames--;
  }
  System_Wait(10);
  Actor_SetSpriteFlags(object, 0);
  object->motionFlags = 0;
}

/// Move on the brown pillar: its old cells open first.
void ShamanVillageCave_StartMovingPillar(void) {
  MapGrid_CopyCellAttributes(99, 10, 4, 3, 99, 18);
}

/// Pushed two spots to column 38, the pillar drops into the gap there and makes
/// the shortcut (flag 0x9a8).
void ShamanVillageCave_FinishMovingPillar(s32 unused, s32 id) {
  struct Actor *object = Actor_Get(id);
  s32 x = object->x >> 20;

  if (x == 38) {
    Event_SetFlag(0x9a8);
    object->priorityFlags = 0;
    ShamanVillageCave_DropPillarIntoGap(id);
    MapGrid_CopyCellAttributes(40, 19, 1, 1, x, 19);
  }
  MapGrid_CopyCellAttributes(99, 6, 4, 3, 99, 18);
}

/// Frost freezes the puddle (actor 16) into a pillar of ice (flag 0x302).
void ShamanVillageCave_FreezePuddle(void) {
  struct Actor *object = Actor_Get(16);

  Event_SetFlag(0x302);
  object->motionFlags = 0;
  object->priorityFlags = 0;
  MapGrid_CopyCellAttributes(8, 42, 1, 1, 6, 42);
}

void FieldCamera_MoveToActorById();

/// Eddy, the Mercury Djinni (actor 17), runs from the leader, unless the two
/// stand on opposite sides of the boundary between 8-pixel columns 36 and 37.
/// With flag 514 set and the leader in the upper half (z cell 95 or less) it
/// slides down two cells, turns, settles with emote 258 and sets flag 516, only
/// once; otherwise it slides two cells along z, one and then three along x
/// away from the leader with a hop, and back along z.
void ShamanVillageCave_EddyFlees(void) {
  struct Actor *object = Actor_GetById(17);
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 leaderX = leader->x >> 19;
  s32 leaderZ = leader->z >> 19;
  s32 objectX = object->x >> 19;
  s32 dx;
  s32 dz;

  if (leaderX <= 36 && objectX > 37)
    return;
  if (leaderX > 37 && objectX <= 36)
    return;
  dx = 1;
  if (leaderX > 36)
    dx = -dx;
  dz = -1;
  if (leaderZ <= 95)
    dz = 1;
  if (dz == 1 && Event_GetFlag(514) != 0) {
    if (Event_GetFlag(516) != 0)
      return;
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    FieldActor_Jump(17, 4, 20);
    Actor_SetDestinationOffset(17, 0, 32);
    FieldActor_WaitForMoveById(17);
    FieldCamera_MoveToActorById(17, 1);
    FieldActor_StartRepeatedMotion(17, 2);
    Actor_FaceDirection(17, FACING_WEST, 0);
    Actor_SetDestinationOffset(17, -8, 0);
    FieldActor_WaitForMoveById(17);
    FieldActor_Jump(17, 4, 20);
    Actor_SetAttachedEffect(17, 258);
    FieldEvent_Wait(30);
    Actor_GetById(17)->actionFlags &= ~1;
    Actor_WalkBy(17, 8, 0);
    FieldActor_WaitForMoveById(17);
    FieldEvent_Wait(30);
    Actor_ShowEmote(17, 258, 0);
    FieldEvent_Wait(30);
    Event_End();
    Event_SetFlag(516);
    return;
  }
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  dz <<= 5;
  FieldActor_Jump(17, 4, 20);
  Actor_SetDestinationOffset(17, 0, dz);
  FieldActor_WaitForMoveById(17);
  Actor_SetDestinationOffset(17, dx << 4, 0);
  FieldActor_WaitForMoveById(17);
  Actor_SetDestinationOffset(17, dx * 48, 0);
  object->velocityY = 0x80000;
  dz = -dz;
  FieldActor_WaitForMoveById(17);
  Actor_SetDestinationOffset(17, 0, dz);
  FieldActor_WaitForMoveById(17);
  Event_End();
}

/// Talking to Eddy, the Mercury Djinni, starts its battle.
void ShamanVillageCave_FightEddy(void) {
  Event_SetFlag(0x205);
  Event_SetFlag(0x206);
  Event_RequestDjinnBattle(17, 35);
}

s32 ShamanVillageCave_SettleOvergrowth(struct Actor *object) {
  Actor_SetSpriteFlags(object, 0);
  ((struct FieldSprite *)object->sprite)->priority = 2;
  object->motionFlags = 0;
  return 0;
}

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gActorPlacements;
}

/// Lift raises one of the four rocks (actors 11 to 14), each for good.
void ShamanVillageCave_LiftFirstRock(void) {
  Event_SetFlag(0x200);
  return;
}

void ShamanVillageCave_LiftSecondRock(void) {
  Event_SetFlag(0x201);
  return;
}

void ShamanVillageCave_LiftThirdRock(void) {
  Event_SetFlag(0x202);
  return;
}

void ShamanVillageCave_LiftFourthRock(void) {
  Event_SetFlag(0x203);
  return;
}

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }

/// The Golden Shirt the gladiators leave goes to the party, if there is room.
void ShamanVillageCave_TakeGoldenShirt(void) {
  s32 result0;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  result0 = Value3((s32 (*)())Event_PickUpItem, 22, 0x190, -1);
  if (result0 != 0)
    goto taken;
  Event_ClearFlag(0xa73);
taken:
  Event_End();
  return;
}

/// The three gladiators stand where their rematch left them.
void ShamanVillageCave_PlaceGladiators(void) {
  s32 angle;

  angle = 0x5000;
  Actor_SetPositionAndFacing(19, PIXELS(231), PIXELS(199), angle);
  Actor_SetPositionAndFacing(20, PIXELS(220), PIXELS(191), angle);
  Actor_SetPositionAndFacing(21, PIXELS(204), PIXELS(191), angle);
  return;
}

/// The Golden Shirt (item 400) lies where the gladiators left it (flag 0xa73).
void ShamanVillageCave_PlaceGoldenShirt(void) {
  s32 angle;

  FieldActor_ShowItemIconById(22, 0x190);
  angle = 0xc000;
  Actor_SetPositionAndFacing(22, PIXELS(212), PIXELS(199), angle);
  Event_SetFlag(0xa73);
  return;
}

/// The fighters from Colosso, after a linked game, find the party and challenge
/// it.
/// ★ Named from the game: "I found !" (MsgIFound2)
void ShamanVillageCave_MeetColossoFans(void) {
  s32 angle;

  if (Event_GetFlag(0x821) == 0)
    return;
  if (Event_GetFlag(0) == 0)
    return;
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Event_SetFlag(0xa70);
  FieldEvent_SetMessageId((s32)MsgIFound2);
  Actor_SetAnimation(4, 1);
  FieldEvent_ShowMessageThenWait(19, 0, 1);
  Actor_WalkToAndWait(4, 200, 221);
  Actor_RunRepeatedMotion(4, 2);
  FieldEvent_Wait(5);
  Actor_WalkBy(4, -16, 8);
  FieldActor_LaunchFromLeader(0, 0, 0, 0xc000);
  FieldActor_LaunchFromLeader(18, 8, 16, 0xc000);
  FieldActor_WaitForMoveById(0);
  FieldActor_WaitForMoveById(18);
  Actor_FaceDirection(0, FACING_SOUTH, 0);
  FieldEvent_Wait(20);
  Actor_FaceDirection(0, FACING_NORTH, 0);
  Actor_FaceDirection(4, FACING_NORTH, 0);
  Actor_FaceDirection(18, FACING_NORTH, 0);
  FieldEvent_Wait(10);
  Actor_ShowEmote(0, 256, 0);
  Actor_ShowEmote(18, 256, 0);
  Actor_ShowEmote(4, 256, 40);
  FieldCamera_MoveTo(PIXELS(199), -1, PIXELS(177), 1);
  FieldCamera_WaitForArrival();
  angle = 0x4000;
  Actor_SetPositionAndFacing(19, PIXELS(199), PIXELS(130), angle);
  Actor_WalkTo(19, 231, 199);
  FieldEvent_Wait(20);
  Actor_SetPositionAndFacing(20, PIXELS(199), PIXELS(130), angle);
  Actor_WalkTo(20, 220, 191);
  FieldEvent_Wait(20);
  Actor_SetPositionAndFacing(21, PIXELS(199), PIXELS(130), angle);
  Actor_WalkTo(21, 204, 191);
  FieldCamera_MoveTo(PIXELS(199), -1, PIXELS(225), 1);
  FieldCamera_WaitForArrival();
  FieldActor_WaitForMoveById(19);
  Actor_FaceDirection(19, 0x5000, 0);
  FieldActor_WaitForMoveById(20);
  Actor_FaceDirection(20, 0x5000, 0);
  FieldActor_WaitForMoveById(21);
  Actor_FaceDirectionAndWait(21, 0x5000);
  Actor_RunRepeatedMotion(21, 2);
  FieldEvent_ShowMessageThenWait(21, 0, 5);
  Actor_ShowEmote(18, 258, 30);
  FieldEvent_ShowMessageThenWait(18, 0, 5);
  Actor_ShowEmote(19, 264, 30);
  FieldEvent_ShowMessageThenWait(19, 0, 5);
  Actor_RunRepeatedMotion(20, 2);
  FieldEvent_Wait(5);
  FieldEvent_ShowMessageThenWait(20, 0, 5);
  Actor_ShowEmote(21, 263, 30);
  FieldEvent_ShowMessageThenWait(21, 0, 5);
  Actor_FaceDirectionAndWait(0, FACING_NORTHEAST);
  Actor_SetAnimationAndWait(0, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0, 0, 5);
  Actor_RunRepeatedMotion(19, 2);
  FieldEvent_Wait(5);
  FieldEvent_ShowMessageThenWait(19, 0, 5);
  Actor_FaceEachOther(18, 4, 0);
  FieldEvent_Wait(10);
  Actor_ShowEmote(0, 257, 30);
  FieldEvent_ShowMessageThenWait(0, 0, 5);
  Actor_ShowEmote(20, 263, 30);
  FieldEvent_ShowMessageThenWait(20, 0, 5);
  Actor_FaceDirection(4, FACING_NORTHEAST, 0);
  Actor_FaceDirectionAndWait(18, FACING_NORTHEAST);
  Actor_SetAnimation(18, 3);
  Actor_SetAnimationAndWait(4, 3);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(21, 4);
  FieldEvent_ShowMessageThenWait(21, 0, 5);
  Actor_ShowEmote(18, 265, 30);
  FieldEvent_ShowMessageThenWait(18, 0, 5);
  Actor_SetAnimation(21, 4);
  Actor_SetAnimation(19, 4);
  Actor_SetAnimationAndWait(20, 4);
  FieldEvent_ShowMessageThenWait(19, 0, 5);
  Actor_ShowEmote(4, 256, 0);
  Actor_ShowEmote(18, 256, 30);
  Actor_ShowEmote(0, 258, 30);
  FieldEvent_ShowMessageThenWait(0, 0, 5);
  Actor_FaceEachOther(20, 21, 0);
  FieldEvent_Wait(10);
  Actor_ShowEmote(20, 263, 30);
  FieldEvent_ShowMessageThenWait(20, 0, 5);
  Actor_FaceDirection(20, 0x5000, 0);
  Actor_FaceDirection(21, 0x5000, 0);
  Actor_FaceDirectionAndWait(18, FACING_NORTHEAST);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(18, 4);
  FieldEvent_ShowMessageThenWait(18, 0, 5);
  Actor_SetAnimationAndWait(21, 4);
  FieldEvent_ShowMessageThenWait(21, 0, 5);
  Actor_SetDestinationOffset(20, -16, 24);
  Actor_SetDestinationOffset(21, -16, 24);
  Actor_SetDestinationOffset(19, -16, 24);
  FieldActor_WaitForMoveById(19);
  /* FAKEMATCH(dead): the once-only block loads gMainRoutineWork before its
   * offset is built; as the plain store, 4 of 408 instructions differ in
   * tla-en. */
  do {
    gMainRoutineWork.battleRule24b = 2;
  } while (0);
  FieldEvent_SetAfterBattleDestination((s32)&SceneId_Ov695, 20);
  Event_SetDefeatDestination((s32)&SceneId_Ov695, 21);
  Field_RequestBattleFromEncounterTable(102, 2);
}

/// Talking to a gladiator starts the rematch.
void ShamanVillageCave_FightGladiators(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  {
    struct MainRoutineWork *party = &gMainRoutineWork;

    party->battleRule24b = 2;
    /* FAKEMATCH(dead): the empty once-only block's loop note ends the
     * scheduling block after the store; without it, 4 of 22 instructions
     * differ in tla-en (the scene id is loaded before the store). */
    do {
    } while (0);
  }
  FieldEvent_SetAfterBattleDestination((s32)&SceneId_Ov695, 20);
  Event_SetDefeatDestination((s32)&SceneId_Ov695, 21);
  Field_RequestBattleFromEncounterTable(102, 2);
}

void Event_SetDestination();

#define EVENT_MESSAGE                                                          \
  (((struct {                                                                  \
      u8 unk000[0x1c4];                                                        \
      s16 message;                                                             \
    } *)Ram_HeapSlots->eventWork)                                              \
       ->message)

/// ★ Named from the game: "Hey, these guys aren't very strong at all!"
/// (MsgHeyTheseGuys)
void ShamanVillageCave_LoseToColossoFans(void) {
  s32 angle = 0xa000;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_MoveTo(PIXELS(199), -1, PIXELS(273), 0);
  FieldEvent_SetMessageId((s32)MsgHeyTheseGuys);
  Actor_SetPositionAndFacing(4, PIXELS(184), PIXELS(229), angle);
  Actor_SetPositionAndFacing(0, PIXELS(200), PIXELS(221), angle);
  {
    s32 turn = 0xc000;

    Actor_SetPositionAndFacing(18, PIXELS(218), PIXELS(320), turn);
  }
  Actor_SetAnimation(4, 19);
  Actor_SetAnimation(0, 39);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Actor_RunRepeatedMotion(21, 2);
  FieldEvent_Wait(5);
  Actor_ShowEmote(21, 263, 30);
  FieldEvent_ShowMessageThenWait(21, 0, 5);
  Actor_ShowEmote(19, 263, 30);
  Event_OpenMessage(19, 0);
  if (EventYesNo_Ask(1) == 0) {
    Message_CloseFinishedWindows();
    FieldEvent_ShowMessageThenWait(20, 0, 5);
    EVENT_MESSAGE++;
  } else {
    Message_CloseFinishedWindows();
    EVENT_MESSAGE++;
    FieldEvent_ShowMessageThenWait(20, 0, 5);
  }
  Actor_SetAnimationAndWait(21, 4);
  FieldEvent_ShowMessageThenWait(21, 0, 5);
  Actor_SetAnimationAndWait(19, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(19, 0, 5);
  FieldCamera_SetSpeed(0x8000, 0x1000);
  FieldCamera_MoveTo(PIXELS(199), -1, PIXELS(373), 1);
  FieldEvent_Wait(100);
  Actor_ShowEmote(18, 261, 1);
  FieldEvent_Wait(20);
  Event_SetDestination((s16)gMainRoutineWork.savedScene,
                       (s16)gMainRoutineWork.savedEntrance);
#if TLA_PATCH_1
  WorldMap_SetReturnLeavingScene(gMainRoutineWork.scene,
                                 gMainRoutineWork.entrance);
#endif
  ((struct {
     u8 unk000[0x1ac];
     s32 value;
   } *)Ram_HeapSlots->eventWork)
      ->value = 0;
}

/// ★ Named from the game: "These guys were phenomenal!" (MsgTheseGuysWere)
void ShamanVillageCave_BeatColossoFans(void) {
  s16 *target;
  s32 angle = 0xe000;

  Event_ClearFlag(0xa71);
  Event_SetFlag(0xa72);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_MoveTo(PIXELS(199), -1, PIXELS(273), 0);
  FieldEvent_SetMessageId((s32)MsgTheseGuysWere);
  Actor_SetPositionAndFacing(4, PIXELS(184), PIXELS(229), angle);
  Actor_SetPositionAndFacing(0, PIXELS(200), PIXELS(221), angle);
  Actor_SetPositionAndFacing(18, PIXELS(208), PIXELS(237), angle);
  Actor_SetAnimation(20, 8);
  Actor_SetAnimation(19, 8);
  Actor_SetAnimation(21, 8);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Actor_RunRepeatedMotion(20, 2);
  FieldEvent_Wait(5);
  FieldEvent_ShowMessageThenWait(21, 0, 5);
  FieldEvent_ShowMessageThenWait(19, 0, 5);
  FieldEvent_ShowMessageThenWait(20, 0, 5);
  Actor_ShowEmote(19, 258, 0);
  Actor_ShowEmote(21, 258, 30);
  Actor_ShowEmote(0, 263, 30);
  FieldEvent_ShowMessageThenWait(0, 0, 5);
  Actor_SetAnimationAndWait(21, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(21, 0, 5);
  Actor_ShowEmote(19, 265, 30);
  Event_OpenMessage(19, 0);
  while (FieldEvent_AskYesNoWithActorAnimation(0, 0) != 0) {
    Actor_ShowEmote(19, 258, 30);
    Event_OpenMessage(20, 0);
    EVENT_MESSAGE--;
  }
  EVENT_MESSAGE++;
  FieldEvent_ShowMessageThenWait(20, 0, 5);
  Actor_SetAnimation(20, 1);
  Actor_SetAnimation(19, 1);
  Actor_SetAnimation(21, 1);
  FieldEvent_Wait(30);
  Actor_SetAnimationAndWait(21, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(21, 0, 5);
  ShamanVillageCave_PlaceGoldenShirt();
  FieldEvent_Wait(60);
  Actor_ShowEmote(0, 257, 30);
  Actor_SetAnimationAndWait(19, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(19, 0, 5);
  Actor_FaceDirectionAndWait(0, FACING_SOUTH);
  FieldEvent_Wait(10);
  Actor_ShowEmote(20, 264, 30);
  Actor_FaceDirection(0, FACING_NORTHEAST, 0);
  FieldEvent_ShowMessageThenWait(20, 0, 5);
  Actor_ShowEmote(21, 265, 30);
  FieldEvent_ShowMessageThenWait(21, 0, 5);
  Actor_SetAnimationAndWait(19, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(19, 0, 5);
  Actor_SetAnimationAndWait(20, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(20, 0, 5);
  Actor_SetAnimation(0, 3);
  Actor_SetAnimation(4, 3);
  Actor_SetAnimationAndWait(18, 3);
  FieldEvent_Wait(10);
  Actor_SetSpeed(19, 0x10000, 0x8000);
  Actor_SetSpeed(20, 0x10000, 0x8000);
  Actor_SetSpeed(21, 0x10000, 0x8000);
  Actor_SetScript(19, gShamanVillageCave_GladiatorLeaves);
  FieldEvent_Wait(20);
  Actor_SetScript(20, gShamanVillageCave_GladiatorFollowsOut);
  FieldEvent_Wait(20);
  Actor_SetScript(21, gShamanVillageCave_GladiatorFollowsOut);
  FieldEvent_Wait(100);
  Actor_FaceEachOther(0, 4, 0);
  Actor_FaceActor(18, 4, 0);
  FieldEvent_Wait(40);
  Actor_SetAnimation(0, 3);
  Actor_SetAnimation(4, 3);
  Actor_SetAnimationAndWait(18, 3);
  FieldEvent_Wait(10);
  Actor_SetAnimation(0, 2);
  target = (s16 *)Actor_Get(4);
  if (target != 0)
    Actor_SetDestination(0, target[5], target[9]);
  FieldActor_WaitForMoveById(0);
  Actor_SetPosition(0, 0, 0);
  Actor_SetAnimation(18, 2);
  target = (s16 *)Actor_Get(4);
  if (target != 0)
    Actor_SetDestination(18, target[5], target[9]);
  FieldActor_WaitForMoveById(18);
  Actor_SetPosition(18, 0, 0);
  Event_End();
}

/// The party goes through a passage in the rock: everyone stops and the screen
/// fades.
void ShamanVillageCave_EnterPassage(void) {
  struct ShamanVillageCaveEventWork *work = Ram_HeapSlots->eventWork;
  struct Actor *object;
  u32 id;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  id = 8;
next:
  object = Actor_Get(id);
  if (object)
    object->motionFlags = 0;
  id++;
  if (id < 64)
    goto next;
  Actor_Get(gMainRoutineWork.currentOwner)->motionFlags = 0;
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 2);
  Actor_CenterAndWalk(gMainRoutineWork.currentOwner, 2, -8);
  FieldEvent_Wait(5);
  Event_PlaySound(123);
  FieldEvent_SetExitRequest(work->value154);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_End();
}

s32 Scene_Initialize(void) {
  struct Actor *object;

  ((struct ShamanVillageCaveSceneWork *)Ram_HeapSlots->eventWork)
      ->screenEffect = 516;
  if (Event_GetFlag(0x9a9))
    Actor_SetPosition(8, 0, 0);
  if (Event_GetFlag(0x9aa))
    Actor_SetPosition(10, 0, 0);
  if (Event_GetFlag(83) || Event_GetFlag(163))
    Event_SetFlag(0x205);
  if (Event_GetFlag(0x9a8)) {
    object = Actor_Get(15);
    object->motionFlags = 0;
    object->priorityFlags = 0;
    object->x = 0x2680000;
    object->y = 0;
    Actor_SetSpriteFlags(object, 0);
    MapGrid_CopyCellAttributes(40, 19, 1, 1, 38, 19);
  }
  Actor_SetSpriteFlags(Actor_Get(16), 0);
  Actor_SetAnimation(16, 1);
  if (Event_GetFlag(770)) {
    object = Actor_Get(16);
    Actor_SetAnimation(16, 5);
    FieldActor_SetSpriteColor(Actor_Get(16), 0);
    object->motionFlags = 0;
    object->priorityFlags = 0;
    object->y = 0;
    MapGrid_CopyCellAttributes(8, 42, 1, 1, 6, 42);
  }
  if (Event_GetFlag(518))
    Actor_SetPosition(17, PIXELS(328), PIXELS(792));
  FieldActor_SetSpriteColor(Actor_Get(19), 1);
  FieldActor_SetSpriteColor(Actor_Get(20), 0);
  FieldActor_SetSpriteColor(Actor_Get(21), 2);
  if ((!Event_GetFlag(265) &&
       (gMainRoutineWork.entrance == 20 || gMainRoutineWork.entrance == 21)) ||
      Event_GetFlag(0xa71)) {
    ShamanVillageCave_PlaceGladiators();
    if (!Event_GetFlag(265) && gMainRoutineWork.entrance == 20) {
      ShamanVillageCave_BeatColossoFans();
    } else if (!Event_GetFlag(265) && gMainRoutineWork.entrance == 21) {
      Event_SetFlag(0xa71);
      ShamanVillageCave_LoseToColossoFans();
    }
  }
  if (Event_GetFlag(0xa73))
    ShamanVillageCave_PlaceGoldenShirt();
  return 0;
}

s32 Scene_PrepareMap(void) {
  s32 result0;
  s32 result1;

  result0 = Event_GetFlag(0x9a9);
  if (result0 == 0)
    goto firstDone;
  MapGrid_CopyCellAttributes(0, 2, 1, 1, 2, 8);
firstDone:
  result1 = Event_GetFlag(0x9aa);
  if (result1 == 0)
    goto done;
  MapGrid_CopyCellAttributes(0, 32, 1, 1, 14, 38);
done:
  return 0;
}

void ShamanVillageCave_JumpForward(void) {
  Field_TryJumpForward();
  return;
}

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(240),
    SCENE_EXIT_IF(1, 2, 56, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(2, 240, 5, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(3, 240, 6, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(4, 240, 9, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(5, 240, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(6, 240, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(7, 240, 8, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(8, 240, 7, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(9, 240, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(10, 2, 57, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

s32 gShamanVillageCave_OvergrowthRests[] = {
    22, 11, 0x18000, 46, (s32)ShamanVillageCave_SettleOvergrowth, 17,
};

// clang-format off
u32 gShamanVillageCave_Settle[] = {
    22, 26, 0, 17,
};

struct ActorPlacement gActorPlacements[] = {
    {307, CONDITION_ALWAYS, (s32)gShamanVillageCave_OvergrowthRests, PIXELS(40), 0, PIXELS(128),
     FACING_NORTH, 2, 0},
    {329, CONDITION_ALWAYS, 1, PIXELS(8), 0, PIXELS(8), FACING_NORTH, 2, 0},
    {307, CONDITION_ALWAYS, (s32)gShamanVillageCave_OvergrowthRests, PIXELS(232), 0, PIXELS(608),
     FACING_NORTH, 2, 0},
    {329, CONDITION_ALWAYS, 1, PIXELS(136), 0, PIXELS(808), FACING_NORTH, 2, 0},
    {329, CONDITION_ALWAYS, 1, PIXELS(216), 0, PIXELS(808), FACING_NORTH, 2, 0},
    {329, CONDITION_ALWAYS, 1, PIXELS(296), 0, PIXELS(792), FACING_NORTH, 2, 0},
    {329, CONDITION_ALWAYS, 1, PIXELS(360), 0, PIXELS(792), FACING_NORTH, 2, 0},
    {272, CONDITION_ALWAYS, (s32)gShamanVillageCave_Settle, PIXELS(584), 0, PIXELS(312),
     FACING_NORTH, 2, 0},
    {290, CONDITION_ALWAYS, 1, PIXELS(104), 0, PIXELS(680), FACING_SOUTH, 2, 0},
    {244, 0x53, 1, PIXELS(264), 0, PIXELS(760), FACING_SOUTH, 0, 0},
    {56, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {239, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {239, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {239, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {489, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_NORTH, 2, 0},
    {0, CONDITION_ALWAYS, 1, PIXELS(8), 0, PIXELS(8), FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

struct Trigger gSceneEvents[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {0xc402, 2, CONDITION_ALWAYS, EVENT(ShamanVillageCave_EnterPassage)},
    {0xc402, 3, CONDITION_ALWAYS, EVENT(ShamanVillageCave_EnterPassage)},
    {0xc402, 4, CONDITION_ALWAYS, EVENT(ShamanVillageCave_EnterPassage)},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {0xc402, 8, CONDITION_ALWAYS, EVENT(ShamanVillageCave_EnterPassage)},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_STEP, 50, 0x205, EVENT(ShamanVillageCave_EddyFlees)},
    {0x602, 41, 0x201, EVENT(ShamanVillageCave_JumpForward)},
    {0x8602, 42, 0x201, EVENT(ShamanVillageCave_JumpForward)},
    {0x602, 43, 0x202, EVENT(ShamanVillageCave_JumpForward)},
    {0x8602, 44, 0x202, EVENT(ShamanVillageCave_JumpForward)},
    {TRIGGER_STEP, 100, 0xa70, EVENT(ShamanVillageCave_MeetColossoFans)},
    {TRIGGER_TALK, 17, CONDITION_ALWAYS, EVENT(ShamanVillageCave_FightEddy)},
    {TRIGGER_TALK, 19, CONDITION_ALWAYS, EVENT(ShamanVillageCave_FightGladiators)},
    {TRIGGER_TALK, 20, CONDITION_ALWAYS, EVENT(ShamanVillageCave_FightGladiators)},
    {TRIGGER_TALK, 21, CONDITION_ALWAYS, EVENT(ShamanVillageCave_FightGladiators)},
    {TRIGGER_TALK, 22, CONDITION_ALWAYS, EVENT(ShamanVillageCave_TakeGoldenShirt)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 8, CONDITION_ALWAYS, EVENT(ShamanVillageCave_BlowAwayOvergrowth)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 10, CONDITION_ALWAYS, EVENT(ShamanVillageCave_BlowAwayOvergrowth)},
    {PSYNERGY_ON_ACTOR(ABILITY_LIFT), 11, 0x200, EVENT(ShamanVillageCave_LiftFirstRock)},
    {PSYNERGY_ON_ACTOR(ABILITY_LIFT), 12, 0x201, EVENT(ShamanVillageCave_LiftSecondRock)},
    {PSYNERGY_ON_ACTOR(ABILITY_LIFT), 13, 0x202, EVENT(ShamanVillageCave_LiftThirdRock)},
    {PSYNERGY_ON_ACTOR(ABILITY_LIFT), 14, 0x203, EVENT(ShamanVillageCave_LiftFourthRock)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 16, 0x302, EVENT(ShamanVillageCave_FreezePuddle)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 15, 0x9a8, EVENT(ShamanVillageCave_StartMovingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 15, 0x9a8, EVENT(ShamanVillageCave_FinishMovingPillar)},
    {9, 0, CONDITION_ALWAYS, EVENT(ShamanVillageCave_FinishMovingPillar)},
    {TRIGGER_TABLE_END},
};

/// The beaten gladiators walk out of the cave, one after another.
u32 gShamanVillageCave_GladiatorLeaves[] = {
    39, 2, 48, 0xe80000, 0xe30000, 1,
    48, 0xd90000, 0xf30000, 1, 48, 0xd90000,
    0x14e0000, 1, 38,
};
// clang-format on

s32 gShamanVillageCave_GladiatorFollowsOut[] = {
    39,       2, 48, 0xe00000,
    0xc30000, 1, 43, (s32)gShamanVillageCave_GladiatorLeaves,
};
