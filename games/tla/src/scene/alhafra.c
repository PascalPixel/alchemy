/// @file
/// Alhafra, the port town of Osenia across the Yampi Desert, where the tidal
/// wave broke the mast of the town's only sailboat. Its events follow the
/// story: Madra's mayor and his party arrive hunting Briggs
/// (Alhafra_MadransArrive) while a Champa lookout runs to warn the crew
/// (Alhafra_LookoutSeesMadrasMayor); once Briggs is caught, bread for a hungry
/// boy (Alhafra_GiveBreadToHungryBoy) gets his father to let the party into the
/// cave under the mayor's manor (Alhafra_FatherLetsPartyToCave), and the
/// getters pick later tables once the mast is cleared and once the ship has
/// gone (FLAG_ALHAFRA_SHIP_GONE). Frost freezes the puddles of seawater
/// (Alhafra_FreezePuddle), and the doors open as the leader walks into them
/// (Alhafra_EnterDoor).

#include "system/main_routine_work.h"
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
#include "event_work.h"
#include "module/scene/scene.h"
#include "shared/tables.h"
#include "actor.h"
#include "field/event_inline.h"
#include "system/ram_buffer.h"

#include "text/msg_ids.h"

void Alhafra_LookoutSeesMadrasMayor(s32 a0);
void Alhafra_TalkToHungryBoy(s32 a);
void Alhafra_TakeLeadersLayer();
s32 Actor_TakeLeaderLayer();
void Alhafra_ReadLookoutsMind(void);
void Alhafra_DrawLookoutBehind(void);
void Alhafra_DrawLookoutInFront(void);
void Alhafra_SailorToldToWork(void);
void Alhafra_RevealTreasureSpot(void);
void Alhafra_HideTreasureSpot(void);
extern u32 gAlhafra_LookAroundScript[];
extern s16 gAlhafra_NoDoorEdit[];
extern s16 gAlhafra_DoorEdit[];
extern s16 gAlhafra_LeftDoorEdit[];

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

// The town's own story, which no other scene reads.
#define FLAG_TOLD_INDRA_MOVED 0x8b0 ///< told the fisherman Indra moved
#define FLAG_LOOKOUT_LEFT 0x8b4     ///< the Champa lookout ran to his crew
#define FLAG_HUNGRY_BOY_FED 0x8b5   ///< the boy at the inn ate the bread
#define FLAG_BOY_TOLD_OF_CAVE 0x8b6 ///< he told of the cave under the manor
#define FLAG_GOLD_COINS_FOUND 0x8b7 ///< his mother found five gold coins
#define FLAG_LET_INTO_CAVE 0x8b9    ///< his father let the party pass

/// Actors the events move; the placement tables give each id its own person.
enum AlhafraActorId {
  /// A Champa sailor watching the road before Briggs is caught.
  ACTOR_LOOKOUT = ACTOR_FIRST_PLACED + 17,
  /// The boy too hungry to move, once Briggs is caught.
  ACTOR_HUNGRY_BOY = ACTOR_FIRST_PLACED + 19
};

extern s16 gAlhafra_ActorPlacements[];
extern s16 gAlhafra_ActorPlacementsOnceBriggsIsCaught[];
extern s16 gAlhafra_ActorPlacementsOnceTheShipIsStolen[];
extern struct ActorPlacement gAlhafra_ActorPlacementsOnceTheMastIsCleared[];
extern struct DoorStep gAlhafra_Doors[];

const struct ActorPlacement *Scene_GetPlacements(void) {
  if (GameFlag_Get(FLAG_ALHAFRA_MAST_CLEARED)) {
    if (GameFlag_Get(FLAG_ALHAFRA_SHIP_GONE) == 0)
      return gAlhafra_ActorPlacementsOnceTheMastIsCleared;
    return gAlhafra_ActorPlacementsOnceTheShipIsStolen;
  }
  if (GameFlag_Get(FLAG_BRIGGS_CAUGHT))
    return gAlhafra_ActorPlacementsOnceBriggsIsCaught;
  return gAlhafra_ActorPlacements;
}

/// Stepping into a door: it opens, the leader walks through and the scene
/// leaves by the exit given.
void Alhafra_OpenDoorAndEnter(struct DoorStep *step, s32 countdown,
                              s32 unused) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Sound_Play(SE_DOOR_OPEN);
  MapGrid_PlayMapEdit(step->script, step->x, step->z);
  Actor_GetById(gMainRoutineWork.currentOwner)->motionFlags = 2;
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x8000, 0x4000);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 2);
  Actor_CenterAndWalk(gMainRoutineWork.currentOwner, 2, -8);
  Event_Wait(10);
  Import_EventSetExitRequest(countdown);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_End();
}

void Alhafra_EnterDoor(s32 index) {
  struct DoorStep *table = gAlhafra_Doors;

  Alhafra_OpenDoorAndEnter(&table[index], index, 0);
}

s32 Shop_Run();

void Alhafra_OpenWeaponShop(s32 a0) { Shop_Run(7, a0); }

void Alhafra_OpenArmorShop(s32 a0) { Shop_Run(8, a0); }

void Alhafra_AskWhetherIndraMoved(s32 actor) {
  s32 msg;

  if (Event_GetFlag(FLAG_TOLD_INDRA_MOVED) == 0) {
    msg = (s32)&MsgAlhafra03;
    Event_SetMessageId(msg);
    FieldMessage_ShowAtActor(actor, 0);
    if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
      Event_SetMessageId(msg + 1);
      GameFlag_Set(FLAG_TOLD_INDRA_MOVED);
    } else {
      Event_SetMessageId(msg + 2);
    }
    Event_ShowMessageAndWait(actor, 0);
  } else {
    Event_SetMessageId((s32)&MsgAlhafra04);
    Event_ShowMessageAndWait(actor, 0);
  }
}

void Alhafra_CanoeMakerBacklogged(s32 actor) {
  s32 msg = (s32)&MsgAlhafra16;

  Event_SetMessageId(msg);
  FieldMessage_ShowAtActor(actor, 0);
  if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
    Event_Wait(10);
    Event_SetMessageId(msg + 1);
  } else {
    Event_Wait(20);
    Event_SetMessageId(msg + 2);
  }
  Event_ShowMessageAndWait(actor, 0);
}

void Alhafra_MayorInAMeeting(s32 actor) {
  s32 msg = (s32)&MsgAlhafra33;

  Event_SetMessageId(msg);
  FieldMessage_ShowAtActor(actor, 0);
  if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
    Event_Wait(10);
    Event_SetMessageId(msg + 1);
  } else {
    Event_Wait(20);
    Event_SetMessageId(msg + 2);
  }
  Event_ShowMessageAndWait(actor, 0);
}

void Alhafra_ReadLookoutsMind(void) { Alhafra_LookoutSeesMadrasMayor(1); }

void Alhafra_LookoutRunsToWarnTheCrew(void) {
  Alhafra_LookoutSeesMadrasMayor(0);
}

void Alhafra_DrawLookoutBehind(void) {
  FieldActor_SetSpritePriority(ACTOR_LOOKOUT, 2);
}

void Alhafra_DrawLookoutInFront(void) {
  FieldActor_SetSpritePriority(ACTOR_LOOKOUT, 3);
}

s32 FlagBlock_ApplyFlags();
extern s16 gAlhafra_Puddles[];

void Alhafra_FreezePuddle(s32 a0, s32 a1) {

  FlagBlock_ApplyFlags((s32)gAlhafra_Puddles);
  Actor_GetById(a1)->priorityFlags = 0;
}

void Alhafra_SailorToldToWork(void) {
  Event_SetMessageId((s32)MsgAlhafra50);
  FieldEvent_ShowMessageThenWait(0x18, 0, 0xa);
  Actor_ArmCallback(19, -256, 0);
  Event_Wait(10);
  Actor_ShowEmoteById(0x13, 0x107, 0x1e);
  FieldEvent_ShowMessageThenWait(0x13, 0, 0xa);
  FieldActor_StartRepeatedMotion(24, 3);
  FieldEffect_SetAttached(0x18, 0x102);
  Event_Wait(40);
  Actor_FaceDirection(19, FACING_WEST, 0);
  FieldEvent_ShowMessageThenWait(0x18, 0, 0);
}

void Alhafra_GiveBreadToHungryBoy(void) {
  GameFlag_Set(FLAG_HUNGRY_BOY_FED);
  PartyInventory_Remove(0x1c9);
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgAlhafra69);
  Event_Wait(10);
  Actor_FaceActorById(ACTOR_HUNGRY_BOY, gMainRoutineWork.currentOwner, 0);
  Event_Wait(10);
  Event_ShowMessageThenWait(ACTOR_HUNGRY_BOY, 0, 0xa);
  Event_ShowMessageThenWait(-1, 0, 0xa);
  Actor_SetModeById(ACTOR_HUNGRY_BOY, 1);
  Actor_Jump(ACTOR_HUNGRY_BOY, 6, 15);
  Actor_Jump(ACTOR_HUNGRY_BOY, 6, 23);
  Event_ShowMessageThenWait(ACTOR_HUNGRY_BOY, 0, 0);
  Alhafra_TalkToHungryBoy(ACTOR_HUNGRY_BOY);
  Actor_FaceDirection(ACTOR_HUNGRY_BOY, 0x4000, 0);
  Event_End();
}

void Alhafra_TalkToHungryBoy(s32 id) {
  if (!GameFlag_Get(FLAG_HUNGRY_BOY_FED)) {
    ((struct AlhafraEventWork *)Ram_HeapSlots->eventWork)->value15a = 1;
    Event_SetMessageId(MsgAlhafra65);
    Event_ShowMessageAndWait(id, 0);
  } else if (Event_GetFlag(FLAG_BOY_TOLD_OF_CAVE)) {
    Event_SetMessageId(MsgAlhafra71);
    Event_ShowMessageAndWait(id, 0);
  } else {
    Event_SetMessageId(MsgAlhafra70);
    Event_Wait(10);
    Motion_SetModeAndWaitAnimation(id, 3);
    Event_ShowMessageThenWait(id, 0, 10);
    Motion_SetVarCbAndRefresh(id, 2);
    FieldMessage_ShowAtActor(id, 0);
    Event_Wait(10);
    if (Event_AskYesNoWithActorAnimation(4, 0) == 0) {
      GameFlag_Set(FLAG_BOY_TOLD_OF_CAVE);
      Event_Wait(20);
      Event_ShowMessageThenWait(id, 0, 10);
      ((struct AlhafraEventWork *)Ram_HeapSlots->eventWork)->value1c4++;
    } else {
      Event_Wait(30);
      ((struct AlhafraEventWork *)Ram_HeapSlots->eventWork)->value1c4++;
      Event_ShowMessageThenWait(id, 0, 10);
    }
  }
}

void Alhafra_FindGoldCoins(void) {
  GameFlag_Set(FLAG_GOLD_COINS_FOUND);
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgAlhafra74);
  FieldCamera_MoveWithinMapBounds(0x2300000, -1, 0x1f00000, 1);
  FieldCamera_WaitForArrival();
  Event_Wait(10);
  Motion_SetVarCbAndRefresh(29, 2);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(0x1d, 0, 0xa);
  Actor_FaceDirection(28, 0, 0);
  Actor_FaceDirection(31, 0x6000, 0);
  Actor_FaceDirection(30, 0x6000, 0);
  Actor_SetModeById(29, 5);
  Sound_Play(MUS_089);
  gMainRoutineWork.currentOwner = 0x1d;
  ItemDisplay_ShowItem(0, 3);
  gMainRoutineWork.currentOwner = 4;
  Event_Wait(50);
  FieldActor_Jump(28, 6, 15);
  FieldActor_Jump(28, 6, 23);
  FieldEvent_ShowMessageThenWait(0x1c, 0, 0xa);
  Actor_ArmCallback(29, 0x8000, 0);
  Event_Wait(20);
  Motion_SetModeAndWaitAnimation(29, 3);
  Event_Wait(20);
  Actor_ShowEmoteById(0x1e, 0x108, 0x1e);
  FieldEvent_ShowMessageThenWait(0x1e, 0, 0xa);
  Actor_ShowEmoteById(0x1d, 0x102, 0x28);
  Actor_FaceDirection(29, 0x4000, 0);
  Event_Wait(20);
  Motion_SetVarCbAndRefresh(29, 2);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(0x1d, 0, 0xa);
  Motion_SetVarCbAndRefresh(30, 2);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(0x1e, 0, 0xa);
  Actor_ArmCallback(29, 0, 0);
  Event_Wait(20);
  Motion_SetModeAndWaitAnimation(29, 3);
  Event_Wait(20);
  FieldActor_Jump(31, 6, 15);
  FieldActor_Jump(31, 6, 23);
  FieldEvent_ShowMessageThenWait(0x1f, 0, 0xa);
  Actor_FaceDirection(30, 0, 0);
  Motion_SetModeAndWaitAnimation(30, 3);
  Event_Wait(20);
  Actor_FaceDirection(31, 0x4000, 0);
  Actor_SetAnimation(31, 6);
  Actor_FaceDirection(30, 0xc000, 0);
  Actor_SetAnimation(30, 6);
  Actor_FaceDirection(29, 0x4000, 0);
  Actor_FaceDirection(28, 0xc000, 0);
  EventCamera_FollowActor(gMainRoutineWork.currentOwner, 1);
  FieldCamera_WaitForArrival();
  Event_End();
}

/// The hungry boy's father, by the way into the cave under the mayor's manor:
/// once the boy is fed he asks whether the party wants to see the cave, and a
/// yes has him step aside for good.
void Alhafra_FatherLetsPartyToCave(s32 actor) {
  s32 message = (s32)MsgAlhafra92;

  if (Event_GetFlag(FLAG_LET_INTO_CAVE) != 0) {
    Event_SetMessageId(message + 4);
    FieldMessage_ShowAtActor(actor, 0);
  } else {
    Event_SetMessageId(message);
    FieldMessage_ShowAtActor(actor, 0);
    if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) != 0) {
      Event_Wait(20);
      Event_SetMessageId(message + 1);
      Event_ShowMessageAndWait(actor, 0);
    } else {
      GameFlag_Set(FLAG_LET_INTO_CAVE);
      Event_Wait(20);
      Event_SetMessageId(message + 2);
      Event_ShowMessage(actor, 0);
      Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 72, 344);
      Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
      Event_Wait(10);
      Actor_SetSpeed(actor, 0x10000, 0x8000);
      Actor_WalkToAndWait(actor, 56, 328);
      Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTHWEST, 0);
      Actor_ArmCallback(actor, FACING_SOUTHEAST, 0);
      Event_Wait(20);
      Motion_SetModeAndWaitAnimation(actor, 3);
      Event_Wait(10);
      Event_ShowMessageAndWait(actor, 0);
    }
  }
}

void Alhafra_RevealTreasureSpot(void) {
  TreasureSpot_SetPosition(0x40, 0xb80000, 0x2280000);
}

void Alhafra_HideTreasureSpot(void) { TreasureSpot_SetPosition(0x40, -1, -1); }

extern struct Trigger gAlhafra_Triggers[];
extern struct Trigger gAlhafra_TriggersOnceBriggsIsCaught[];
extern struct Trigger gAlhafra_TriggersOnceTheShipIsStolen[];
extern struct Trigger gAlhafra_TriggersOnceTheMastIsCleared[];

const struct Trigger *Scene_GetEvents(void) {
  if (GameFlag_Get(FLAG_ALHAFRA_MAST_CLEARED)) {
    if (GameFlag_Get(FLAG_ALHAFRA_SHIP_GONE) == 0)
      return gAlhafra_TriggersOnceTheMastIsCleared;
    return gAlhafra_TriggersOnceTheShipIsStolen;
  }
  if (GameFlag_Get(FLAG_BRIGGS_CAUGHT))
    return gAlhafra_TriggersOnceBriggsIsCaught;
  return gAlhafra_Triggers;
}

s32 Scene_Initialize(void) {
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x100;
  if (GameFlag_Get(FLAG_ALHAFRA_MAST_CLEARED)) {
    if (!GameFlag_Get(FLAG_ALHAFRA_SHIP_GONE)) {
      Actor_GetById(24)->update = (void (*)(void))Alhafra_TakeLeadersLayer;
      Actor_GetById(19)->update = (void (*)(void))Alhafra_TakeLeadersLayer;
      Actor_SetSpritePriority(20, 2);
      Actor_SetSpritePriority(14, 3);
      Actor_SetSpritePriority(16, 3);
      Actor_SetSpritePriority(10, 1);
    } else {
      Actor_Get(14)->update = (void (*)(void))Alhafra_TakeLeadersLayer;
      Actor_GetById(20)->update = (void (*)(void))Alhafra_TakeLeadersLayer;
      Actor_GetById(22)->update = (void (*)(void))Alhafra_TakeLeadersLayer;
      Actor_SetSpritePriority(13, 2);
      Actor_SetSpritePriority(19, 3);
      Actor_SetSpritePriority(23, 2);
      Actor_SetSpritePriority(24, 3);
      Actor_SetSpritePriority(27, 2);
      Actor_SetSpritePriority(28, 3);
      Actor_SetSpritePriority(29, 1);
      Actor_SetSpritePriority(30, 3);
      Actor_SetSpritePriority(31, 3);
      Actor_SetSpritePriority(12, 3);
      Actor_SetSpritePriority(10, 3);
    }
  } else if (GameFlag_Get(FLAG_BRIGGS_CAUGHT)) {
    Actor_GetById(14)->update = (void (*)(void))Alhafra_TakeLeadersLayer;
    Actor_GetById(20)->update = (void (*)(void))Alhafra_TakeLeadersLayer;
    Actor_SetSpritePriority(13, 2);
    Actor_SetSpritePriority(19, 3);
    Actor_SetSpritePriority(23, 3);
    Actor_SetSpritePriority(24, 3);
    Actor_SetSpritePriority(22, 2);
    Actor_SetSpritePriority(27, 2);
    Actor_SetSpritePriority(28, 3);
    Actor_SetSpritePriority(29, 1);
    Actor_SetSpritePriority(30, 3);
    Actor_SetSpritePriority(31, 3);
    Actor_SetSpritePriority(12, 3);
    Actor_SetActionCallback(Actor_GetById(26), 9);
    if (!GameFlag_Get(FLAG_HUNGRY_BOY_FED))
      Actor_SetModeById(27, 5);
    if (GameFlag_Get(FLAG_GOLD_COINS_FOUND)) {
      Actor_SetModeById(30, 6);
      Actor_SetModeById(31, 6);
    }
    if (GameFlag_Get(FLAG_LET_INTO_CAVE)) {
      Actor_SetPosition(12, PIXELS(56), PIXELS(328));
      Actor_ArmCallback(12, FACING_EAST, 0);
    }
  } else {
    Actor_SetSpritePriority(13, 2);
    Actor_SetSpritePriority(23, 2);
    Actor_SetSpritePriority(10, 3);
    Actor_GetById(24)->update = (void (*)(void))Alhafra_TakeLeadersLayer;
    if (GameFlag_Get(FLAG_ALHAFRA_MADRANS_ARRIVED))
      Actor_SetPosition(14, PIXELS(536), PIXELS(760));
    if (GameFlag_Get(FLAG_LOOKOUT_LEFT))
      Actor_SetHorizontalPositionWithTerrain(25, 0, 0);
    FlagBlock_PlaceAllMarkingEdges((s32)&gAlhafra_Puddles);
    Actor_SetActionCallback(Actor_GetById(26), 9);
    Actor_SetActionCallback(Actor_GetById(27), 9);
    Actor_SetActionCallback(Actor_GetById(28), 9);
  }
  return 0;
}

s32 Scene_PrepareMap(void) {
  if (GameFlag_Get(FLAG_BRIGGS_CAUGHT)) {
    u16 *src = (u16 *)0x5000116;

    u32 c;

    /* FAKEMATCH(dead): the four copies in a once-only block, whose end holds
     * the order around it; as plain statements, 4 of 57 instructions differ in
     * tla-en. */
    do {
      c = *src;
      *(u16 *)0x50000f6 = c;
      src++;
      c = *src;
      *(u16 *)0x50000f8 = c;
      src++;
      c = *src;
      *(u16 *)0x50000fa = c;
      {
        u32 last = src[1];

        *(u16 *)0x50000fc = last;
      }
    } while (0);
    MapGrid_CopyCellAttributes(37, 18, 2, 2, 37, 33);
    MapGrid_CopyCellAttributes(37, 18, 2, 3, 39, 31);
    MapGrid_CopyCellAttributes(50, 80, 7, 5, 35, 95);
  }
  return 0;
}

void Alhafra_TakeLeadersLayer(void) { Actor_TakeLeaderLayer(); }

extern s32 gAlhafra_OfficialStepsBackScript[];
extern u32 gAlhafra_PasserbyWalksOffScript[];
extern u32 gAlhafra_PasserbyLeadsFromCell21Script[];
extern u32 gAlhafra_OfficialFollowsFromCell21Script[];
extern u32 gAlhafra_FirstMadranFollowsFromCell21Script[];
extern u32 gAlhafra_MayorFollowsFromCell21Script[];
extern u32 gAlhafra_SecondSoldierFollowsFromCell21Script[];
extern u32 gAlhafra_FirstSoldierFollowsFromCell21Script[];
extern u32 gAlhafra_ElderFollowsFromCell21Script[];
extern u32 gAlhafra_SecondMadranFollowsFromCell21Script[];
extern s32 gAlhafra_PasserbyLeadsFromCell20Script[];
extern u32 gAlhafra_OfficialFollowsFromCell20Script[];
extern u32 gAlhafra_FirstMadranFollowsFromCell20Script[];
extern u32 gAlhafra_MayorFollowsFromCell20Script[];
extern u32 gAlhafra_ElderFollowsFromCell20Script[];
extern u32 gAlhafra_FirstSoldierFollowsFromCell20Script[];
extern s32 gAlhafra_SecondMadranFollowsFromCell20Script[];
extern u32 gAlhafra_SecondSoldierFollowsFromCell20Script[];

void Alhafra_MadransArrive(s32 a0) {
  GameFlag_Set(FLAG_ALHAFRA_MADRANS_ARRIVED);
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgAlhafra10);
  Actor_SetSpeed(29, 0x10000, 0x8000);
  Actor_SetSpeed(30, 0x10000, 0x8000);
  Actor_SetSpeed(31, 0x10000, 0x8000);
  Actor_SetSpeed(32, 0x10000, 0x8000);
  Actor_SetSpeed(33, 0x10000, 0x8000);
  Actor_SetSpeed(34, 0x10000, 0x8000);
  Actor_SetSpeed(35, 0x10000, 0x8000);
  Actor_SetSpeed(17, 0x8000, 0x4000);
  Actor_SetSpeed(14, 0x8000, 0x4000);
  if (a0 == 0x14) {
    Actor_WalkByAndWait(4, 0, -8);
  }
  Actor_SetPosition(31, 0x1980000, 0x3680000);
  Actor_SetPosition(32, 0x1880000, 0x3780000);
  Event_ShowMessageThenWait(0x1f, 0, 0xa);
  if (a0 == 0x14) {
    Actor_FaceDirection(4, 0x4000, 0);
  } else {
    Actor_ArmCallback(4, 0, 0);
  }
  Event_Wait(20);
  FieldCamera_MoveTo(0x1900000, -1, 0x3280000, 1);
  FieldCamera_WaitForArrival();
  Actor_WalkBy(31, 0, -96);
  Actor_WalkByAndWait(32, 0, -96);
  Actor_SetModeById(31, 1);
  Event_Wait(10);
  Actor_FaceDirection(31, 0, 0);
  Actor_FaceDirection(32, 0x8000, 0);
  Event_Wait(20);
  Actor_FaceDirection(31, 0xa000, 0);
  Actor_FaceDirection(32, 0xe000, 0);
  Event_Wait(20);
  Actor_FaceDirection(31, 0, 0);
  Actor_FaceDirection(32, 0x8000, 0);
  Event_Wait(20);
  Actor_FaceDirection(31, 0xa000, 0);
  Actor_FaceDirection(32, 0xe000, 0);
  Event_Wait(20);
  Actor_FaceDirection(31, 0, 0);
  Actor_SetPosition(33, 0x1880000, 0x3680000);
  Actor_SetPosition(35, 0x1980000, 0x3680000);
  FieldEvent_ShowMessageThenWait(0x21, 0, 0xa);
  Actor_FaceDirection(31, 0x4000, 0);
  Actor_FaceDirection(32, 0x3000, 0);
  Event_Wait(20);
  Actor_WalkBy(35, 0, -48);
  Actor_WalkByAndWait(33, 0, -48);
  Actor_WaitForMoveById(35);
  Actor_ArmCallback(35, 0xb000, 0);
  Event_Wait(10);
  Motion_SetVarCbAndRefresh(31, 2);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(0x1f, 0, 0xa);
  Actor_ShowEmote(33, 0x102, 30);
  Actor_FaceDirection(33, 0x3000, 0);
  Event_Wait(40);
  Motion_SetVarCbAndRefresh(33, 2);
  Event_Wait(20);
  Actor_FaceDirection(33, 0xc000, 0);
  Event_Wait(20);
  FieldEvent_ShowMessageThenWait(0x21, 0, 0xa);
  Actor_FaceDirection(31, 0x5000, 0);
  Actor_FaceDirection(32, 0xe000, 0);
  Event_Wait(20);
  Actor_SetAnimation(31, 3);
  Motion_SetModeAndWaitAnimation(32, 3);
  Event_Wait(10);
  Actor_WalkBy(35, 32, -48);
  Actor_WalkByAndWait(33, 32, -48);
  Actor_WaitForMoveById(35);
  Actor_FaceDirection(35, 0x5000, 0);
  Actor_FaceDirection(33, 0x4000, 0);
  Actor_SetSpritePriority(35, 3);
  Event_Wait(20);
  Actor_SetPosition(34, 0x1900000, 0x3680000);
  FieldEvent_ShowMessageThenWait(0x22, 0, 0xa);
  Actor_FaceDirection(31, 0x4000, 0);
  Actor_FaceDirection(32, 0x3000, 0);
  Actor_FaceDirection(33, 0x4000, 0);
  Event_Wait(20);
  Actor_WalkByAndWait(34, 0, -32);
  Event_Wait(1);
  Actor_SetModeById(34, 5);
  Event_Wait(90);
  Motion_SetVarCbAndRefresh(34, 2);
  Event_Wait(20);
  FieldEvent_ShowMessageThenWait(0x22, 0, 0xa);
  Actor_WalkByAndWaitById(34, 0, -16);
  Event_Wait(1);
  Actor_SetModeById(34, 5);
  Event_Wait(60);
  Actor_WalkByAndWait(34, 8, -32);
  Actor_FaceDirection(34, 0x4000, 0);
  Event_Wait(1);
  Actor_SetModeById(34, 5);
  Event_Wait(30);
  Actor_SetPosition(30, 0x1880000, 0x3680000);
  Actor_SetPosition(29, 0x1980000, 0x3680000);
  Actor_WalkBy(30, 0, -32);
  Actor_WalkByAndWait(29, 0, -32);
  Actor_SetModeById(30, 1);
  Event_Wait(1);
  Actor_SetAnimation(29, 5);
  FieldActor_WatchAndSetScript(31, 30);
  FieldActor_WatchAndSetScript(32, 30);
  FieldActor_WatchAndSetScript(33, 30);
  FieldActor_WatchAndSetScript(34, 30);
  Actor_WatchAndSetScript(35, 30);
  Event_Wait(20);
  Actor_ArmCallback(30, 0xa000, 0);
  Event_Wait(20);
  Actor_ArmCallback(30, 0, 0);
  Event_Wait(20);
  Actor_FaceDirection(30, 0xc000, 0);
  Event_Wait(10);
  FieldActor_Jump(30, 6, 15);
  FieldActor_Jump(30, 6, 23);
  Actor_SetAnimation(34, 1);
  FieldEvent_ShowMessageThenWait(0x1e, 0, 0xa);
  Actor_SetMovementTypeById(29, (s32)gAlhafra_OfficialStepsBackScript);
  Event_Wait(20);
  Actor_WalkByAndWait(30, 0, -32);
  Actor_WalkByAndWaitById(30, -32, 0);
  Event_Wait(10);
  Motion_SetVarCbAndRefresh(30, 2);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(0x1e, 0, 0xa);
  Actor_SetAnimationAndWait(29, 4);
  FieldEvent_ShowMessageThenWait(0x1d, 0, 0xa);
  Actor_ShowEmote(0x1e, 0x102, 0x1e);
  Actor_ArmCallback(30, 0, 0);
  Event_Wait(20);
  Motion_SetVarCbAndRefresh(29, 2);
  Event_Wait(20);
  FieldEvent_ShowMessageThenWait(0x1d, 0, 0xa);
  Actor_SetAnimation(29, 5);
  Actor_WalkByAndWait(30, 64, 0);
  Actor_FaceDirection(29, 0x5000, 0);
  Actor_FaceDirection(30, 0xe000, 0);
  Event_Wait(20);
  FieldEvent_ShowMessageThenWait(0x101e, 0, 0xa);
  Actor_SetModeById(29, 1);
  Event_Wait(20);
  Motion_SetModeAndWaitAnimation(29, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(0x1d, 0, 0xa);
  Actor_Stop(31);
  Actor_Stop(32);
  Actor_Stop(33);
  Actor_Stop(34);
  Actor_Stop(35);
  Actor_SetModeById(29, 5);
  Event_Wait(50);
  Actor_FaceDirection(31, 0x5000, 0);
  Actor_FaceDirection(32, 0xe000, 0);
  Event_Wait(40);
  Actor_FaceDirection(31, 0x3000, 0);
  Actor_FaceDirection(32, 0x2000, 0);
  Event_Wait(20);
  Actor_SetMovementTypeById(14, (s32)gAlhafra_PasserbyWalksOffScript);
  FieldActor_Jump(31, 6, 15);
  FieldActor_Jump(31, 6, 23);
  Event_ShowMessageThenWait(0x1f, 0, 0xa);
  Event_Wait(10);
  Actor_ArmCallback(32, 0xc000, 0);
  Event_Wait(20);
  Actor_FaceDirection(32, 0x8000, 0);
  Event_Wait(20);
  Actor_FaceDirection(32, 0x4000, 0);
  Event_Wait(20);
  Actor_FaceDirection(32, 0x8000, 0);
  Event_Wait(20);
  Actor_ArmCallback(32, 0x1000, 0);
  Event_Wait(20);
  FieldEvent_ShowMessageThenWait(0x20, 0, 0xa);
  Actor_SetModeById(29, 1);
  Event_Wait(10);
  Actor_ShowEmoteById(0x1d, 0x107, 0x1e);
  Actor_FaceDirection(29, 0x8000, 0);
  Event_Wait(20);
  FieldEvent_ShowMessageThenWait(0x1d, 0, 0xa);
  EventCamera_FollowActor(29, 1);
  FieldCamera_WaitForArrival();
  Actor_WalkToAndWait(29, 488, 808);
  Actor_FaceDirection(31, 0, 0);
  Actor_FaceDirection(32, 0, 0);
  Actor_FaceDirection(33, 0, 0);
  Actor_FaceDirection(34, 0, 0);
  Actor_FaceDirection(35, 0x3000, 0);
  Actor_FaceDirection(30, 0, 0);
  Actor_FaceDirection(29, 0xd000, 0);
  Event_Wait(20);
  Actor_SetAnimationAndWait(29, 4);
  FieldCamera_SetSpeed(0x40000, 0x8000);
  FieldCamera_MoveTo(0x2880000, -1, 0x2180000, 1);
  FieldCamera_WaitForArrival();
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(0x1d, 0, 0xa);
  FieldCamera_MoveTo(0x1b80000, -1, 0x3280000, 1);
  FieldCamera_WaitForArrival();
  FieldCamera_SetSpeed(0x10000, 0x2000);
  Event_Wait(20);
  Actor_WalkByAndWait(33, 32, 36);
  Actor_WalkByAndWait(33, 32, 0);
  Actor_WalkToAndWait(33, 504, 808);
  Actor_ArmCallback(33, 0xd000, 0);
  Event_Wait(30);
  FieldActor_StartRepeatedMotion(33, 3);
  FieldEffect_SetAttached(0x21, 0x102);
  Event_Wait(50);
  Actor_FaceDirection(33, 0x8000, 0);
  Event_Wait(20);
  Motion_SetModeAndWaitAnimation(33, 3);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(0x21, 0, 0xa);
  Motion_SetVarCbAndRefresh(29, 2);
  Event_Wait(20);
  Actor_ArmCallback(29, 0, 0);
  Event_Wait(20);
  FieldEvent_ShowMessageThenWait(0x201d, 0, 0xa);
  Actor_ShowEmoteById(0x22, 0x100, 0x28);
  FieldEvent_ShowMessageThenWait(0x22, 0, 0xa);
  Actor_FaceDirection(29, 0x8000, 0);
  Event_Wait(20);
  Motion_SetModeAndWaitAnimation(29, 3);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(0x1d, 0, 0xa);
  Motion_SetVarCbAndRefresh(30, 2);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(0x1e, 0, 0xa);
  Actor_FaceDirection(31, 0x5000, 0);
  Actor_ArmCallback(32, 0xe000, 0);
  Event_Wait(30);
  FieldActor_Jump(31, 6, 15);
  FieldActor_Jump(31, 6, 23);
  FieldEvent_ShowMessageThenWait(0x1f, 0, 0xa);
  Motion_SetModeAndWaitAnimation(32, 3);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(0x20, 0, 0xa);
  Motion_SetVarCbAndRefresh(29, 2);
  Event_Wait(10);
  Actor_FaceDirection(31, 0x3000, 0);
  Actor_FaceDirection(32, 0x2000, 0);
  FieldEvent_ShowMessageThenWait(0x1d, 0, 0xa);
  Motion_SetModeAndWaitAnimation(30, 3);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(0x1e, 0, 0xa);
  Actor_SetPosition(17, 0x1980000, 0x2b80000);
  Actor_WalkToAndWait(17, 408, 760);
  Actor_WalkToAndWait(17, 448, 792);
  Actor_WalkToAndWait(17, 448, 808);
  Actor_WalkByAndWait(30, 8, 0);
  FieldEvent_ShowMessageThenWait(0x101e, 0, 0xa);
  Actor_ShowEmoteById(0x11, 0x101, 0x32);
  Actor_FaceDirection(17, 0x8000, 0);
  Event_Wait(20);
  Actor_ShowEmoteById(0x1e, 0x108, 0x1e);
  FieldEvent_ShowMessageThenWait(0x101e, 0, 0xa);
  Motion_SetVarCbAndRefresh(17, 2);
  Event_Wait(10);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->waitMode = 0;
  FieldEvent_ShowMessageThenWait(0x11, 0, 0xa);
  Motion_SetModeAndWaitAnimation(30, 3);
  Event_Wait(20);
  Actor_SetSpeed(29, 0x8000, 0x4000);
  Actor_SetSpeed(30, 0x8000, 0x4000);
  Actor_SetSpeed(31, 0x8000, 0x4000);
  Actor_SetSpeed(32, 0x8000, 0x4000);
  Actor_SetSpeed(33, 0x8000, 0x4000);
  Actor_SetSpeed(34, 0x8000, 0x4000);
  Actor_SetSpeed(35, 0x8000, 0x4000);
  Actor_SetSpeed(17, 0x8000, 0x4000);
  EventCamera_FollowActor(30, 1);
  FieldCamera_WaitForArrival();
  if (a0 == 0x15) {
    FieldActor_SetSpritePriority(30, 2);
    FieldActor_WatchAndSetScript(31, 29);
    FieldActor_WatchAndSetScript(32, 29);
    FieldActor_WatchAndSetScript(34, 29);
    Actor_SetMovementTypeById(17, (s32)gAlhafra_PasserbyLeadsFromCell21Script);
    Actor_SetMovementTypeById(29,
                              (s32)gAlhafra_OfficialFollowsFromCell21Script);
    Actor_SetMovementTypeById(33,
                              (s32)gAlhafra_FirstMadranFollowsFromCell21Script);
    Event_Wait(80);
    FieldActor_SetSpritePriority(29, 3);
    FieldActor_SetSpritePriority(30, 3);
    FieldActor_SetSpritePriority(31, 3);
    FieldActor_SetSpritePriority(32, 3);
    FieldActor_SetSpritePriority(33, 3);
    FieldActor_SetSpritePriority(34, 3);
    FieldActor_SetSpritePriority(35, 3);
    FieldActor_SetSpritePriority(17, 3);
    FieldActor_WatchAndSetScript(35, 29);
    Actor_SetMovementTypeById(30, (s32)gAlhafra_MayorFollowsFromCell21Script);
    Event_Wait(170);
    Actor_SetMovementTypeById(
        32, (s32)gAlhafra_SecondSoldierFollowsFromCell21Script);
    Event_Wait(10);
    Actor_SetMovementTypeById(
        31, (s32)gAlhafra_FirstSoldierFollowsFromCell21Script);
    Actor_SetMovementTypeById(34, (s32)gAlhafra_ElderFollowsFromCell21Script);
    Event_Wait(60);
    Actor_SetMovementTypeById(
        35, (s32)gAlhafra_SecondMadranFollowsFromCell21Script);
  } else {
    FieldActor_SetSpritePriority(30, 2);
    FieldActor_WatchAndSetScript(30, 17);
    FieldActor_WatchAndSetScript(31, 30);
    FieldActor_WatchAndSetScript(32, 30);
    FieldActor_WatchAndSetScript(34, 30);
    Actor_SetMovementTypeById(17, (s32)gAlhafra_PasserbyLeadsFromCell20Script);
    Event_Wait(60);
    Actor_SetMovementTypeById(29,
                              (s32)gAlhafra_OfficialFollowsFromCell20Script);
    Actor_SetMovementTypeById(33,
                              (s32)gAlhafra_FirstMadranFollowsFromCell20Script);
    Event_Wait(70);
    FieldActor_WatchAndSetScript(35, 29);
    Actor_SetMovementTypeById(30, (s32)gAlhafra_MayorFollowsFromCell20Script);
    FieldActor_SetSpritePriority(29, 3);
    FieldActor_SetSpritePriority(30, 3);
    FieldActor_SetSpritePriority(31, 3);
    FieldActor_SetSpritePriority(32, 3);
    FieldActor_SetSpritePriority(33, 3);
    FieldActor_SetSpritePriority(34, 3);
    FieldActor_SetSpritePriority(35, 3);
    Actor_SetSpritePriority(17, 3);
    Event_Wait(120);
    Actor_SetMovementTypeById(34, (s32)gAlhafra_ElderFollowsFromCell20Script);
    Event_Wait(40);
    Actor_SetMovementTypeById(
        31, (s32)gAlhafra_FirstSoldierFollowsFromCell20Script);
    Actor_SetMovementTypeById(
        35, (s32)gAlhafra_SecondMadranFollowsFromCell20Script);
    Event_Wait(30);
    Actor_SetMovementTypeById(
        32, (s32)gAlhafra_SecondSoldierFollowsFromCell20Script);
  }
  Event_Wait(300);
  Actor_RefreshSelectorById(34);
  Event_Wait(50);
  FieldCamera_MoveTo(0xf80000, -1, 0x2980000, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_ShowMessageThenWait(0x11, 0, 0xa);
  Actor_ArmCallback(17, 0x3000, 0);
  Event_Wait(20);
  Motion_SetModeAndWaitAnimation(17, 3);
  Event_Wait(20);
  Motion_SetModeAndWaitAnimation(30, 3);
  Event_Wait(20);
  Actor_WalkByAndWait(17, -16, 0);
  Actor_ArmCallback(17, 0, 0);
  Event_Wait(20);
  Actor_FaceDirection(29, 0x8000, 0);
  Actor_FaceDirection(31, 0x8000, 0);
  Actor_FaceDirection(32, 0x8000, 0);
  Actor_FaceDirection(30, 0, 0);
  Actor_FaceDirection(33, 0, 0);
  Actor_ArmCallback(34, 0, 0);
  Event_Wait(20);
  Actor_SetAnimation(30, 3);
  Actor_SetAnimation(29, 3);
  Actor_SetAnimation(33, 3);
  Actor_SetAnimation(31, 3);
  Actor_SetAnimation(32, 3);
  Actor_SetAnimation(35, 3);
  Motion_SetModeAndWaitAnimation(34, 3);
  Event_Wait(10);
  Actor_FaceDirection(29, 0xc000, 0);
  Actor_FaceDirection(31, 0xc000, 0);
  Actor_FaceDirection(32, 0xc000, 0);
  Actor_FaceDirection(30, 0xc000, 0);
  Actor_FaceDirection(33, 0xc000, 0);
  Actor_FaceDirection(34, 0xc000, 0);
  Actor_SetSpeed(29, 0x1e666, 0xf333);
  Actor_SetSpeed(30, 0x1e666, 0xf333);
  Actor_SetSpeed(31, 0x1e666, 0xf333);
  Actor_SetSpeed(32, 0x1e666, 0xf333);
  Actor_SetSpeed(33, 0x1e666, 0xf333);
  Actor_SetSpeed(34, 0x1e666, 0xf333);
  Actor_SetSpeed(35, 0x1e666, 0xf333);
  Actor_WalkBy(29, 0, -150);
  Actor_WalkBy(30, 0, -150);
  Actor_WalkBy(31, 0, -150);
  Actor_WalkBy(32, 0, -150);
  Actor_WalkBy(33, 0, -150);
  Actor_WalkBy(35, 0, -150);
  Actor_WalkByAndWait(34, 0, -150);
  Actor_SetPosition(29, 0, 0);
  Actor_SetPosition(30, 0, 0);
  Actor_SetPosition(31, 0, 0);
  Actor_SetPosition(32, 0, 0);
  Actor_SetPosition(33, 0, 0);
  Actor_SetPosition(34, 0, 0);
  Actor_SetHorizontalPositionWithTerrain(35, 0, 0);
  Event_Wait(15);
  FieldCamera_FollowActor(gMainRoutineWork.currentOwner, 1);
  FieldCamera_WaitForArrival();
  Event_End();
}

void Alhafra_LookoutSeesMadrasMayor(s32 a0) {

  Event_SetFlag(FLAG_LOOKOUT_LEFT);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgAlhafra28);
  if (a0 == 1) {
    FieldEvent_Wait(10);
    Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 168, 736);
    Actor_FaceDirection(gMainRoutineWork.currentOwner, 0, 0);
  }
  Actor_ShowEmote(ACTOR_LOOKOUT, 0x102, 0x1e);
  FieldEvent_ShowMessageThenWait(ACTOR_LOOKOUT, 0, 0xa);
  Actor_SetAnimationAndWait(ACTOR_LOOKOUT, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(ACTOR_LOOKOUT, 0, 0xa);
  Actor_SetAnimationAndWait(ACTOR_LOOKOUT, 4);
  FieldEvent_ShowMessageThenWait(ACTOR_LOOKOUT, 0, 0xa);
  Actor_RunRepeatedMotion(ACTOR_LOOKOUT, 2);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(ACTOR_LOOKOUT, 0, 0xa);
  if (a0 != 1) {
  } else {
    Actor_WalkByAndWaitById(ACTOR_LOOKOUT, -8, 0);
    Event_Wait(15);
    Actor_StartRepeatedMotion(ACTOR_LOOKOUT, 3);
    FieldEffect_SetAttached(ACTOR_LOOKOUT, 0x102);
    Event_Wait(40);
    Actor_GetById(ACTOR_LOOKOUT)->actionFlags &= 0xfe;
    Actor_WalkToAndWaitById(ACTOR_LOOKOUT, 192, 736);
    Event_Wait(1);
    Actor_GetById(ACTOR_LOOKOUT)->actionFlags |= 1;
    Event_Wait(10);
    Event_ShowMessageThenWait(ACTOR_LOOKOUT, 0, 0xa);
    Actor_ShowEmote(ACTOR_LOOKOUT, 0x107, 30);
    Event_OpenMessage(ACTOR_LOOKOUT, 0);
    if (Event_AskYesNoWithActorAnimation(4, 0) == 0) {
      Event_Wait(30);
      Motion_SetVarCbAndRefresh(ACTOR_LOOKOUT, 2);
      Event_Wait(20);
      Event_ShowMessageThenWait(ACTOR_LOOKOUT, 0, 0xa);
      ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId =
          (((struct EventWork *)Ram_HeapSlots->eventWork)->messageId + 1);
    } else {
      Event_Wait(30);
      ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId =
          (((struct EventWork *)Ram_HeapSlots->eventWork)->messageId + 1);
      Event_ShowMessageThenWait(ACTOR_LOOKOUT, 0, 0xa);
    }
    Actor_GetById(4)->actionFlags &= 0xfe;
    Actor_WalkToAndWait(4, 152, 752);
    Event_Wait(1);
    Actor_GetById(4)->actionFlags |= 1;
    Actor_FaceDirection(4, 0xe000, 0);
    Event_Wait(20);
    Motion_SetModeAndWaitAnimation(ACTOR_LOOKOUT, 3);
    Event_Wait(10);
    Actor_ArmCallback(ACTOR_LOOKOUT, 0xe000, 0);
    Event_Wait(70);
    Actor_ShowEmote(ACTOR_LOOKOUT, 0x100, 50);
    Event_ShowMessageThenWait(ACTOR_LOOKOUT, 0, 0xa);
  }
  Actor_WalkToAndWait(ACTOR_LOOKOUT, 184, 728);
  Actor_WalkByAndWait(ACTOR_LOOKOUT, 0, -110);
  if (a0 == 0) {
    Event_Wait(50);
    Actor_SetPosition(ACTOR_LOOKOUT, 0x1080000, 0x2880000);
    Actor_WalkByAndWaitById(ACTOR_LOOKOUT, 0, 32);
    Actor_WalkByAndWaitById(ACTOR_LOOKOUT, 150, 0);
  }
  Actor_SetHorizontalPositionWithTerrain(ACTOR_LOOKOUT, 0, 0);
  Event_End();
}

// clang-format off
s16 gAlhafra_Puddles[] = {
    26, 512, 27, 513, 28, 514, -1, 0,
};

u32 gAlhafra_LookAroundScript[] = {
    22, 3, 0xd000, 0, 40, 22,
    3, 0xb000, 0, 60, 22, 3,
    0xd000, 0, 20, 22, 3, 0xb000,
    0, 60, 13, 0xffff, 0,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(95),
    SCENE_EXIT_IF(1, 96, 1, CONDITION_ALWAYS),
    SCENE_EXIT_IF(2, 96, 2, CONDITION_ALWAYS),
    SCENE_EXIT_IF(5, 96, 5, CONDITION_ALWAYS),
    SCENE_EXIT_IF(6, 96, 6, CONDITION_ALWAYS),
    SCENE_EXIT_IF(7, 96, 7, CONDITION_ALWAYS),
    SCENE_EXIT_IF(8, 101, 8, CONDITION_ALWAYS),
    SCENE_EXIT_IF(9, 2, 20, CONDITION_ALWAYS),
    SCENE_EXIT_IF(11, 98, 1, CONDITION_ALWAYS),
    SCENE_EXIT_IF(12, 100, 1, CONDITION_ALWAYS),
    SCENE_EXIT_IF(14, 97, 1, CONDITION_ALWAYS),
    SCENE_EXITS_END,
};

s16 gAlhafra_ActorPlacements[] = {
    108, -1, 1, 0, 0, 456, 0, 0,
    0, 544, FACING_SOUTH, 1, 109, -1, 1, 0,
    0, 392, 0, 0, 0, 544, FACING_SOUTH, 1,
    231, -1, 1, 0, 0, 280, 0, 0,
    0, 280, 0, 1, 231, -1, 1, 0,
    0, 272, 0, 0, 0, 296, 0, 1,
    114, -1, 1, 0, 0, 72, 0, 0,
    0, 320, FACING_SOUTH, 1, 115, -1, 1, 0,
    0, 376, 0, 0, 0, 824, FACING_SOUTHEAST, 0,
    116, -1, 1, 0, 0, 504, 0, 0,
    0, 808, FACING_NORTH, 1, 114, -1, 1, 0,
    0, 392, 0, 0, 0, 728, FACING_SOUTH, 1,
    191, -1, 1, 0, 0, 392, 0, 0,
    0, 744, FACING_NORTH, 1, 112, -1, 1, 0,
    0, 248, 0, 0, 0, 664, FACING_NORTH, 1,
    113, -1, 1, 0, 0, 296, 0, 0,
    0, 520, FACING_NORTHWEST, 1, 128, -1, 1, 0,
    0, 576, 0, 0, 0, 520, FACING_SOUTHEAST, 0,
    186, -1, 1, 0, 0, 552, 0, 0,
    0, 560, FACING_SOUTHEAST, 1, 195, -1, 1, 0,
    0, 760, 0, 0, 0, 568, 0, 1,
    215, -1, 1, 0, 0, 520, 0, 0,
    0, 632, 0, 1, 224, -1, 1, 0,
    0, 632, 0, 0, 0, 600, FACING_SOUTH, 1,
    225, -1, 2, 0, 0, 648, 0, 0,
    0, 568, FACING_SOUTH, 0, 39, -1, 1, 0,
    0, 192, 0, 0, 0, 736, FACING_NORTHEAST, 2,
    290, -1, 1, 0, 0, 504, 0, 0,
    0, 568, FACING_SOUTH, 1, 290, -1, 1, 0,
    0, 440, 0, 0, 0, 584, FACING_SOUTH, 1,
    290, -1, 1, 0, 0, 712, 0, 0,
    0, 600, FACING_SOUTH, 1, 19, -1, 1, 0,
    0, 8, 0, 0, 0, 8, FACING_NORTH, 1,
    20, -1, 1, 0, 0, 8, 0, 0,
    0, 8, FACING_NORTH, 1, 88, -1, 1, 0,
    0, 8, 0, 0, 0, 8, FACING_NORTH, 1,
    88, -1, 1, 0, 0, 8, 0, 0,
    0, 8, FACING_NORTH, 1, 114, -1, 1, 0,
    0, 8, 0, 0, 0, 8, FACING_NORTH, 1,
    112, -1, 1, 0, 0, 8, 0, 0,
    0, 8, FACING_NORTH, 1, 93, -1, 1, 0,
    0, 8, 0, 0, 0, 8, FACING_NORTH, 1,
    -1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0,
};

s16 gAlhafra_ActorPlacementsOnceBriggsIsCaught[] = {
    108, -1, 1, 0, 0, 456, 0, 0,
    0, 544, FACING_SOUTH, 1, 109, -1, 1, 0,
    0, 392, 0, 0, 0, 544, FACING_SOUTH, 1,
    231, -1, 1, 0, 0, 344, 0, 0,
    0, 312, FACING_SOUTH, 0, 231, -1, 1, 0,
    0, 296, 0, 0, 0, 312, 0x3000, 0,
    114, -1, 1, 0, 0, 72, 0, 0,
    0, 320, FACING_SOUTH, 0, 115, -1, 1, 0,
    0, 376, 0, 0, 0, 824, FACING_NORTHEAST, 1,
    116, -1, 2, 0, 0, 568, 0, 0,
    0, 792, FACING_SOUTH, 0, 114, -1, 1, 0,
    0, 392, 0, 0, 0, 728, FACING_SOUTH, 0,
    191, -1, 1, 0, 0, 392, 0, 0,
    0, 744, FACING_NORTH, 0, 112, -1, 2, 0,
    0, 248, 0, 0, 0, 616, FACING_SOUTH, 0,
    113, -1, 1, 0, 0, 296, 0, 0,
    0, 520, FACING_NORTHWEST, 0, 128, -1, 1, 0,
    0, 664, 0, 0, 0, 520, FACING_WEST, 1,
    186, -1, 2, 0, 0, 712, 0, 0,
    0, 600, FACING_SOUTH, 0, 195, -1, 1, 0,
    0, 520, 0, 0, 0, 536, 0, 1,
    215, -1, 1, 0, 0, 616, 0, 0,
    0, 600, FACING_NORTHEAST, 0, 224, -1, 1, 0,
    0, 728, 0, 0, 0, 504, FACING_NORTHWEST, 1,
    225, -1, 1, 0, 0, 712, 0, 0,
    0, 504, FACING_NORTH, 1, 115, -1, 1, 0,
    0, 680, 0, 0, 0, 552, 0, 1,
    116, -1, 1, 0, 0, 648, 0, 0,
    0, 576, FACING_NORTHEAST, 1, 191, -1, 1, 0,
    0, 536, 0, 0, 0, 616, FACING_SOUTH, 1,
    114, -1, 1, 0, 0, 536, 0, 0,
    0, 488, FACING_NORTH, 0, 128, -1, 1, 0,
    0, 568, 0, 0, 0, 472, FACING_SOUTH, 0,
    186, -1, 1, 0, 0, 600, 0, 0,
    0, 456, FACING_NORTH, 1, 191, -1, 1, 0,
    0, 616, 0, 0, 0, 456, FACING_SOUTH, 1,
    -1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0,
};

s16 gAlhafra_ActorPlacementsOnceTheShipIsStolen[] = {
    108, -1, 1, 0, 0, 456, 0, 0,
    0, 544, FACING_SOUTH, 1, 109, -1, 1, 0,
    0, 392, 0, 0, 0, 544, FACING_SOUTH, 1,
    231, -1, 1, 0, 0, 280, 0, 0,
    0, 280, 0, 1, 231, -1, 1, 0,
    0, 272, 0, 0, 0, 296, 0, 1,
    114, -1, 1, 0, 0, 56, 0, 0,
    0, 328, 0, 1, 115, -1, 1, 0,
    0, 376, 0, 0, 0, 824, FACING_NORTHEAST, 0,
    116, -1, 2, 0, 0, 568, 0, 0,
    0, 792, FACING_SOUTH, 0, 114, -1, 1, 0,
    0, 392, 0, 0, 0, 728, FACING_SOUTH, 1,
    191, -1, 1, 0, 0, 392, 0, 0,
    0, 744, FACING_NORTH, 1, 112, -1, 2, 0,
    0, 248, 0, 0, 0, 616, FACING_SOUTH, 0,
    113, -1, 1, 0, 0, 296, 0, 0,
    0, 520, FACING_NORTHWEST, 0, 128, -1, 1, 0,
    0, 680, 0, 0, 0, 520, FACING_WEST, 1,
    186, -1, 2, 0, 0, 712, 0, 0,
    0, 600, FACING_SOUTH, 0, 195, -1, 1, 0,
    0, 520, 0, 0, 0, 536, 0, 1,
    215, -1, 2, 0, 0, 440, 0, 0,
    0, 800, FACING_SOUTH, 0, 224, -1, 1, 0,
    0, 632, 0, 0, 0, 600, FACING_SOUTH, 0,
    225, -1, 1, 0, 0, 536, 0, 0,
    0, 656, FACING_NORTHEAST, 0, 115, -1, 1, 0,
    0, 680, 0, 0, 0, 552, 0, 0,
    116, -1, 1, 0, 0, 648, 0, 0,
    0, 568, FACING_NORTHEAST, 0, 191, -1, 1, 0,
    0, 760, 0, 0, 0, 568, FACING_SOUTH, 1,
    114, -1, 1, 0, 0, 536, 0, 0,
    0, 488, FACING_NORTH, 1, 128, -1, 1, 0,
    0, 568, 0, 0, 0, 472, FACING_SOUTH, 1,
    186, -1, 1, 0, 0, 600, 0, 0,
    0, 456, FACING_NORTH, 0, 191, -1, 1, 0,
    0, 616, 0, 0, 0, 456, FACING_SOUTH, 0,
    -1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0,
};

struct ActorPlacement gAlhafra_ActorPlacementsOnceTheMastIsCleared[] = {
    {115, CONDITION_ALWAYS, 1, PIXELS(680), 0, PIXELS(552), FACING_SOUTH, 0, 0},
    {115, CONDITION_ALWAYS, 1, PIXELS(296), 0, PIXELS(520), FACING_SOUTHEAST + FACING_STEP, 1, 0},
    {116, CONDITION_ALWAYS, 1, PIXELS(504), 0, PIXELS(600), FACING_EAST, 0, 0},
    {116, CONDITION_ALWAYS, 1, PIXELS(232), 0, PIXELS(544), FACING_SOUTH + FACING_STEP, 1, 0},
    {116, CONDITION_ALWAYS, 1, PIXELS(360), 0, PIXELS(536), FACING_SOUTH + FACING_STEP, 0, 0},
    {114, CONDITION_ALWAYS, 1, PIXELS(392), 0, PIXELS(616), FACING_SOUTHEAST + FACING_STEP, 1, 0},
    {114, CONDITION_ALWAYS, 1, PIXELS(560), 0, PIXELS(648), FACING_NORTH + FACING_STEP, 0, 0},
    {114, CONDITION_ALWAYS, 1, PIXELS(72), 0, PIXELS(320), FACING_SOUTHEAST + FACING_STEP, 0, 0},
    {128, CONDITION_ALWAYS, 1, PIXELS(568), 0, PIXELS(520), FACING_SOUTHEAST + FACING_STEP, 0, 0},
    {128, CONDITION_ALWAYS, 1, PIXELS(760), 0, PIXELS(568), FACING_EAST, 1, 0},
    {128, CONDITION_ALWAYS, 1, PIXELS(408), 0, PIXELS(600), FACING_SOUTH + FACING_STEP, 1, 0},
    {112, CONDITION_ALWAYS, 1, PIXELS(504), 0, PIXELS(552), FACING_EAST, 1, 0},
    {112, CONDITION_ALWAYS, 1, PIXELS(728), 0, PIXELS(624), FACING_EAST, 0, 0},
    {112, CONDITION_ALWAYS, 1, PIXELS(264), 0, PIXELS(592), FACING_EAST, 0, 0},
    {113, CONDITION_ALWAYS, 1, PIXELS(248), 0, PIXELS(576), FACING_EAST, 0, 0},
    {113, CONDITION_ALWAYS, (s32)gAlhafra_LookAroundScript, PIXELS(648), 0, PIXELS(504), FACING_NORTH, 1, 0},
    {113, CONDITION_ALWAYS, 1, PIXELS(712), 0, PIXELS(608), FACING_EAST, 0, 0},
    {SCENE_TABLE_END},
};

s16 gAlhafra_NoDoorEdit[] = {
    -1,
};

s16 gAlhafra_DoorEdit[] = {
    63, 2, 1, 2, 6, -1,
};

s16 gAlhafra_LeftDoorEdit[] = {
    62, 2, 1, 2, 6, -1, 0,
};

struct DoorStep gAlhafra_Doors[] = {
    {0, 0, 0},
    {(s32)gAlhafra_DoorEdit, 27, 37},
    {(s32)gAlhafra_DoorEdit, 27, 43},
    {0, 0, 0},
    {0, 0, 0},
    {(s32)gAlhafra_DoorEdit, 21, 36},
    {(s32)gAlhafra_DoorEdit, 38, 46},
    {(s32)gAlhafra_NoDoorEdit, 0, 0},
    {(s32)gAlhafra_LeftDoorEdit, 11, 33},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {(s32)gAlhafra_LeftDoorEdit, 5, 9},
    {0, 0, 0},
    {(s32)gAlhafra_DoorEdit, 16, 9},
};

struct Trigger gAlhafra_Triggers[] = {
    {0xc602, 1, CONDITION_ALWAYS, EVENT(Alhafra_EnterDoor)},
    {0xc602, 2, CONDITION_ALWAYS, EVENT(Alhafra_EnterDoor)},
    {0xc602, 5, CONDITION_ALWAYS, EVENT(Alhafra_EnterDoor)},
    {0xc602, 6, CONDITION_ALWAYS, EVENT(Alhafra_EnterDoor)},
    {0xc602, 8, CONDITION_ALWAYS, EVENT(Alhafra_EnterDoor)},
    {0xc602, 12, CONDITION_ALWAYS, EVENT(Alhafra_EnterDoor)},
    {0xc602, 14, CONDITION_ALWAYS, EVENT(Alhafra_EnterDoor)},
    {0xc602, 7, CONDITION_ALWAYS, EVENT(Alhafra_EnterDoor)},
    {0xc401, 97, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {0x1f3, 200, CONDITION_ALWAYS, 0x00400000 | MSGNO(MsgAlhafra180)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, EVENT(Alhafra_RevealTreasureSpot)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(Alhafra_HideTreasureSpot)},
    {0xc400, 8, CONDITION_ALWAYS, EVENT(Alhafra_OpenWeaponShop)},
    {0xc400, 9, CONDITION_ALWAYS, EVENT(Alhafra_OpenArmorShop)},
    {TRIGGER_STEP, 18, CONDITION_ALWAYS, EVENT(Alhafra_DrawLookoutBehind)},
    {TRIGGER_STEP, 19, CONDITION_ALWAYS, EVENT(Alhafra_DrawLookoutInFront)},
    {TRIGGER_STEP, 20, FLAG_ALHAFRA_MADRANS_ARRIVED, EVENT(Alhafra_MadransArrive)},
    {TRIGGER_STEP, 21, FLAG_ALHAFRA_MADRANS_ARRIVED, EVENT(Alhafra_MadransArrive)},
    {TRIGGER_STEP, 22, FLAG_LOOKOUT_LEFT, EVENT(Alhafra_LookoutRunsToWarnTheCrew)},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgAlhafra29)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgAlhafra30)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgAlhafra31)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgAlhafra32)},
    {TRIGGER_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgAlhafra01)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgAlhafra02)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, EVENT(Alhafra_AskWhetherIndraMoved)},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgAlhafra05)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgAlhafra06)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgAlhafra07)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgAlhafra08)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgAlhafra09)},
    {TRIGGER_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgAlhafra11)},
    {TRIGGER_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgAlhafra12)},
    {TRIGGER_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgAlhafra13)},
    {TRIGGER_TALK, 20, CONDITION_ALWAYS, MSGNO(MsgAlhafra14)},
    {TRIGGER_TALK, 21, CONDITION_ALWAYS, MSGNO(MsgAlhafra15)},
    {TRIGGER_TALK, 22, CONDITION_ALWAYS, EVENT(Alhafra_CanoeMakerBacklogged)},
    {TRIGGER_TALK, 23, CONDITION_ALWAYS, MSGNO(MsgAlhafra17)},
    {TRIGGER_TALK, 24, CONDITION_ALWAYS, MSGNO(MsgAlhafra18)},
    {TRIGGER_TALK, 25, CONDITION_ALWAYS, MSGNO(MsgAlhafra19)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgAlhafra20)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgAlhafra21)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, MSGNO(MsgAlhafra22)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS, MSGNO(MsgAlhafra23)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS, MSGNO(MsgAlhafra24)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS, MSGNO(MsgAlhafra25)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 23, CONDITION_ALWAYS, MSGNO(MsgAlhafra26)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 24, CONDITION_ALWAYS, MSGNO(MsgAlhafra27)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 1049, CONDITION_ALWAYS, EVENT(Alhafra_ReadLookoutsMind)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, EVENT(Alhafra_MayorInAMeeting)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgAlhafra34)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgAlhafra35)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgAlhafra36)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgAlhafra37)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgAlhafra38)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 26, 0x200, EVENT(Alhafra_FreezePuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 27, 0x201, EVENT(Alhafra_FreezePuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 28, 0x202, EVENT(Alhafra_FreezePuddle)},
    {TRIGGER_TABLE_END},
};

struct Trigger gAlhafra_TriggersOnceBriggsIsCaught[] = {
    {0xc602, 1, CONDITION_ALWAYS, EVENT(Alhafra_EnterDoor)},
    {0xc602, 2, CONDITION_ALWAYS, EVENT(Alhafra_EnterDoor)},
    {0xc602, 5, CONDITION_ALWAYS, EVENT(Alhafra_EnterDoor)},
    {0xc602, 6, CONDITION_ALWAYS, EVENT(Alhafra_EnterDoor)},
    {0xc602, 8, CONDITION_ALWAYS, EVENT(Alhafra_EnterDoor)},
    {0xc602, 12, CONDITION_ALWAYS, EVENT(Alhafra_EnterDoor)},
    {0xc602, 14, CONDITION_ALWAYS, EVENT(Alhafra_EnterDoor)},
    {0xc602, 7, CONDITION_ALWAYS, EVENT(Alhafra_EnterDoor)},
    {0xc401, 97, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {0x1f3, 200, CONDITION_ALWAYS, 0x00400000 | MSGNO(MsgAlhafra180)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, EVENT(Alhafra_RevealTreasureSpot)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(Alhafra_HideTreasureSpot)},
    {0xc400, 8, CONDITION_ALWAYS, EVENT(Alhafra_OpenWeaponShop)},
    {0xc400, 9, CONDITION_ALWAYS, EVENT(Alhafra_OpenArmorShop)},
    {TRIGGER_STEP, 25, FLAG_GOLD_COINS_FOUND, EVENT(Alhafra_FindGoldCoins)},
    {TRIGGER_TALK, 18, IF_FLAG_SET(0x8ff), MSGNO(MsgAlhafra94)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, IF_FLAG_SET(0x8ff), MSGNO(MsgAlhafra95)},
    {TRIGGER_TALK, 17, IF_FLAG_SET(0x8ff), MSGNO(MsgAlhafra96)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, IF_FLAG_SET(0x8ff), MSGNO(MsgAlhafra97)},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgAlhafra82)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgAlhafra83)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgAlhafra84)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgAlhafra85)},
    {TRIGGER_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgAlhafra39)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgAlhafra40)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgAlhafra41)},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgAlhafra42)},
    {TRIGGER_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgAlhafra43)},
    {TRIGGER_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgAlhafra44)},
    {TRIGGER_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgAlhafra45)},
    {TRIGGER_TALK, 20, CONDITION_ALWAYS, MSGNO(MsgAlhafra46)},
    {TRIGGER_TALK, 21, CONDITION_ALWAYS, MSGNO(MsgAlhafra47)},
    {TRIGGER_TALK, 22, CONDITION_ALWAYS, MSGNO(MsgAlhafra48)},
    {TRIGGER_TALK, 23, CONDITION_ALWAYS, MSGNO(MsgAlhafra49)},
    {TRIGGER_TALK, 24, CONDITION_ALWAYS, EVENT(Alhafra_SailorToldToWork)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgAlhafra51)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgAlhafra52)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgAlhafra53)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgAlhafra54)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgAlhafra55)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgAlhafra56)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, MSGNO(MsgAlhafra57)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS, MSGNO(MsgAlhafra58)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS, MSGNO(MsgAlhafra59)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS, MSGNO(MsgAlhafra60)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 23, CONDITION_ALWAYS, MSGNO(MsgAlhafra61)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 24, CONDITION_ALWAYS, MSGNO(MsgAlhafra62)},
    {TRIGGER_TALK, 25, CONDITION_ALWAYS, MSGNO(MsgAlhafra63)},
    {TRIGGER_TALK, 26, CONDITION_ALWAYS, MSGNO(MsgAlhafra64)},
    {TRIGGER_TALK, 27, CONDITION_ALWAYS, EVENT(Alhafra_TalkToHungryBoy)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 25, CONDITION_ALWAYS, MSGNO(MsgAlhafra66)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 26, CONDITION_ALWAYS, MSGNO(MsgAlhafra67)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 27, FLAG_HUNGRY_BOY_FED, MSGNO(MsgAlhafra68)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 27, CONDITION_ALWAYS, MSGNO(MsgAlhafra72)},
    {0x1c914, 27, CONDITION_ALWAYS, EVENT(Alhafra_GiveBreadToHungryBoy)},
    {TRIGGER_TALK, 28, CONDITION_ALWAYS, MSGNO(MsgAlhafra73)},
    {TRIGGER_TALK, 29, CONDITION_ALWAYS, MSGNO(MsgAlhafra75)},
    {TRIGGER_TALK, 30, CONDITION_ALWAYS, MSGNO(MsgAlhafra77)},
    {TRIGGER_TALK, 31, CONDITION_ALWAYS, MSGNO(MsgAlhafra76)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 28, CONDITION_ALWAYS, MSGNO(MsgAlhafra78)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 29, CONDITION_ALWAYS, MSGNO(MsgAlhafra79)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 30, CONDITION_ALWAYS, MSGNO(MsgAlhafra81)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 31, CONDITION_ALWAYS, MSGNO(MsgAlhafra80)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgAlhafra86)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgAlhafra87)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgAlhafra88)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgAlhafra89)},
    {TRIGGER_TALK, 12, FLAG_BOY_TOLD_OF_CAVE, MSGNO(MsgAlhafra90)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, EVENT(Alhafra_FatherLetsPartyToCave)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, FLAG_LET_INTO_CAVE, MSGNO(MsgAlhafra91)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgAlhafra93)},
    {TRIGGER_TABLE_END},
};

struct Trigger gAlhafra_TriggersOnceTheShipIsStolen[] = {
    {0xc602, 1, CONDITION_ALWAYS, EVENT(Alhafra_EnterDoor)},
    {0xc602, 2, CONDITION_ALWAYS, EVENT(Alhafra_EnterDoor)},
    {0xc602, 5, CONDITION_ALWAYS, EVENT(Alhafra_EnterDoor)},
    {0xc602, 6, CONDITION_ALWAYS, EVENT(Alhafra_EnterDoor)},
    {0xc602, 8, CONDITION_ALWAYS, EVENT(Alhafra_EnterDoor)},
    {0xc602, 12, CONDITION_ALWAYS, EVENT(Alhafra_EnterDoor)},
    {0xc602, 14, CONDITION_ALWAYS, EVENT(Alhafra_EnterDoor)},
    {0xc602, 7, CONDITION_ALWAYS, EVENT(Alhafra_EnterDoor)},
    {0xc401, 97, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {0x1f3, 200, CONDITION_ALWAYS, 0x00400000 | MSGNO(MsgAlhafra180)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, EVENT(Alhafra_RevealTreasureSpot)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(Alhafra_HideTreasureSpot)},
    {0xc400, 8, CONDITION_ALWAYS, EVENT(Alhafra_OpenWeaponShop)},
    {0xc400, 9, CONDITION_ALWAYS, EVENT(Alhafra_OpenArmorShop)},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgAlhafra170)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgAlhafra171)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgAlhafra172)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgAlhafra173)},
    {TRIGGER_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgAlhafra132)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgAlhafra133)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgAlhafra134)},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgAlhafra135)},
    {TRIGGER_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgAlhafra136)},
    {TRIGGER_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgAlhafra137)},
    {TRIGGER_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgAlhafra138)},
    {TRIGGER_TALK, 20, CONDITION_ALWAYS, MSGNO(MsgAlhafra139)},
    {TRIGGER_TALK, 21, CONDITION_ALWAYS, MSGNO(MsgAlhafra140)},
    {TRIGGER_TALK, 22, CONDITION_ALWAYS, MSGNO(MsgAlhafra141)},
    {TRIGGER_TALK, 23, CONDITION_ALWAYS, MSGNO(MsgAlhafra142)},
    {TRIGGER_TALK, 24, CONDITION_ALWAYS, MSGNO(MsgAlhafra143)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgAlhafra144)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgAlhafra145)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgAlhafra146)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgAlhafra147)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgAlhafra148)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgAlhafra155)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, MSGNO(MsgAlhafra149)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS, MSGNO(MsgAlhafra150)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS, MSGNO(MsgAlhafra151)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS, MSGNO(MsgAlhafra152)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 23, CONDITION_ALWAYS, MSGNO(MsgAlhafra153)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 24, CONDITION_ALWAYS, MSGNO(MsgAlhafra154)},
    {TRIGGER_TALK, 25, CONDITION_ALWAYS, MSGNO(MsgAlhafra156)},
    {TRIGGER_TALK, 26, CONDITION_ALWAYS, MSGNO(MsgAlhafra157)},
    {TRIGGER_TALK, 27, CONDITION_ALWAYS, MSGNO(MsgAlhafra158)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 25, CONDITION_ALWAYS, MSGNO(MsgAlhafra159)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 26, CONDITION_ALWAYS, MSGNO(MsgAlhafra160)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 27, CONDITION_ALWAYS, MSGNO(MsgAlhafra161)},
    {TRIGGER_TALK, 28, CONDITION_ALWAYS, MSGNO(MsgAlhafra162)},
    {TRIGGER_TALK, 29, CONDITION_ALWAYS, MSGNO(MsgAlhafra163)},
    {TRIGGER_TALK, 31, CONDITION_ALWAYS, MSGNO(MsgAlhafra164)},
    {TRIGGER_TALK, 30, CONDITION_ALWAYS, MSGNO(MsgAlhafra165)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 28, CONDITION_ALWAYS, MSGNO(MsgAlhafra166)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 29, CONDITION_ALWAYS, MSGNO(MsgAlhafra167)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 31, CONDITION_ALWAYS, MSGNO(MsgAlhafra168)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 30, CONDITION_ALWAYS, MSGNO(MsgAlhafra169)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgAlhafra174)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgAlhafra175)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgAlhafra176)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgAlhafra177)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgAlhafra178)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgAlhafra179)},
    {TRIGGER_TABLE_END},
};

struct Trigger gAlhafra_TriggersOnceTheMastIsCleared[] = {
    {0xc602, 1, CONDITION_ALWAYS, EVENT(Alhafra_EnterDoor)},
    {0xc602, 2, CONDITION_ALWAYS, EVENT(Alhafra_EnterDoor)},
    {0xc602, 5, CONDITION_ALWAYS, EVENT(Alhafra_EnterDoor)},
    {0xc602, 6, CONDITION_ALWAYS, EVENT(Alhafra_EnterDoor)},
    {0xc602, 8, CONDITION_ALWAYS, EVENT(Alhafra_EnterDoor)},
    {0xc602, 12, CONDITION_ALWAYS, EVENT(Alhafra_EnterDoor)},
    {0xc602, 14, CONDITION_ALWAYS, EVENT(Alhafra_EnterDoor)},
    {0xc602, 7, CONDITION_ALWAYS, EVENT(Alhafra_EnterDoor)},
    {0xc401, 97, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {0x1f3, 200, CONDITION_ALWAYS, 0x00400000 | MSGNO(MsgAlhafra180)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, EVENT(Alhafra_RevealTreasureSpot)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(Alhafra_HideTreasureSpot)},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgAlhafra98)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgAlhafra99)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgAlhafra100)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgAlhafra101)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgAlhafra102)},
    {TRIGGER_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgAlhafra103)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgAlhafra104)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgAlhafra105)},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgAlhafra106)},
    {TRIGGER_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgAlhafra107)},
    {TRIGGER_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgAlhafra108)},
    {TRIGGER_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgAlhafra109)},
    {TRIGGER_TALK, 20, CONDITION_ALWAYS, MSGNO(MsgAlhafra110)},
    {TRIGGER_TALK, 21, CONDITION_ALWAYS, MSGNO(MsgAlhafra111)},
    {TRIGGER_TALK, 22, CONDITION_ALWAYS, MSGNO(MsgAlhafra112)},
    {TRIGGER_TALK, 23, CONDITION_ALWAYS, MSGNO(MsgAlhafra113)},
    {TRIGGER_TALK, 24, CONDITION_ALWAYS, MSGNO(MsgAlhafra114)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgAlhafra115)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgAlhafra116)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgAlhafra117)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgAlhafra118)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgAlhafra119)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgAlhafra120)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgAlhafra121)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgAlhafra122)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgAlhafra123)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgAlhafra124)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgAlhafra125)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, MSGNO(MsgAlhafra126)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS, MSGNO(MsgAlhafra127)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS, MSGNO(MsgAlhafra128)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS, MSGNO(MsgAlhafra129)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 23, CONDITION_ALWAYS, MSGNO(MsgAlhafra130)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 24, CONDITION_ALWAYS, MSGNO(MsgAlhafra131)},
    {TRIGGER_TABLE_END},
};

s32 gAlhafra_OfficialStepsBackScript[] = {
    39, 2, 5, 0, 0, -0x80000,
    1, 5, 0x200000, 0, -0x200000, 1,
    39, 1, 22, 3, 0x8000, 0,
    1, 39, 5, 17,
};

u32 gAlhafra_PasserbyWalksOffScript[] = {
    39, 2, 4, 0x2280000, 0, 0x3280000,
    1, 4, 0x2180000, 0, 0x2f80000, 1,
    39, 1, 17,
};

DEFINE_G_DATA_02003434(gAlhafra_PasserbyLeadsFromCell21Script, 0x1b80000, 0x3200000, 0x1980000,
    0x2f80000, 0x2f80000, 0x1980000, 0xf80000, 0x2a80000, 0xf80000, 0x2780000)

DEFINE_G_DATA_020034CC(gAlhafra_MayorFollowsFromCell21Script, 0x1a80000, 0x3180000, 0x1980000,
    0x2f80000, 0x1980000, 0x2a80000, 0xf80000, 0x2a80000, 0x2980000)

DEFINE_G_DATA_02003434(gAlhafra_OfficialFollowsFromCell21Script, 0x1b80000, 0x3280000, 0x1b80000,
    0x3200000, 0x2f80000, 0x1980000, 0x1080000, 0x2a80000, 0x1080000, 0x2980000)

DEFINE_G_DATA_020034CC(gAlhafra_FirstMadranFollowsFromCell21Script, 0x1b80000, 0x3280000, 0x1b80000,
    0x3200000, 0x1980000, 0x2f80000, 0x1980000, 0x2a80000, 0x2a80000)

u32 gAlhafra_FirstSoldierFollowsFromCell21Script[] = {
    39, 2, 4, 0x1980000, 0, 0x2f80000,
    1, 4, 0x1980000, 0, 0x2a80000, 1,
    4, 0x1080000, 0, 0x2a80000, 1, 39,
    1, 22, 3, 0xd000, 17,
};

DEFINE_G_DATA_020034CC(gAlhafra_ElderFollowsFromCell21Script, 0x1980000, 0x2f80000, 0x1980000,
    0x2a80000, 0x1180000, 0x2a80000, 0x1180000, 0x2b80000, 0x2b80000)

u32 gAlhafra_SecondSoldierFollowsFromCell21Script[] = {
    39, 2, 5, 0x100000, 0, 0,
    1, 4, 0x1980000, 0, 0x2f80000, 1,
    4, 0x1980000, 0, 0x2f80000, 1, 4,
    0x1980000, 0, 0x2a80000, 1, 4, 0x1180000,
    0, 0x2a80000, 1, 4, 0x1180000, 0,
    0x2b80000, 1, 4, 0x1080000, 0, 0x2b80000,
    1, 39, 1, 22, 3, 0xd000,
    17,
};

DEFINE_G_DATA_02003434(gAlhafra_SecondMadranFollowsFromCell21Script, 0x1980000, 0x2f80000, 0x1980000,
    0x2f80000, 0x2a80000, 0x1180000, 0x1180000, 0x2b80000, 0x1000000, 0x2c80000)

s32 gAlhafra_PasserbyLeadsFromCell20Script[] = {
    39, 2, 5, 0, 0, -0x100000,
    1, 5, -0x100000, 0, 0, 1,
    5, -0x100000, 0, 0x100000, 1, 4,
    0xf80000, 0, 0x3280000, 1, 4, 0xf80000,
    0, 0x2780000, 1, 39, 1, 22,
    3, 0xd000, 17,
};

u32 gAlhafra_MayorFollowsFromCell20Script[] = {
    39, 2, 4, 0x1480000, 0, 0x3280000,
    1, 4, 0xf80000, 0, 0x3280000, 1,
    4, 0xf80000, 0, 0x2980000, 1, 39,
    1, 22, 3, 0xd000, 17,
};

u32 gAlhafra_OfficialFollowsFromCell20Script[] = {
    39, 2, 4, 0x1480000, 0, 0x3280000,
    1, 4, 0x1080000, 0, 0x3280000, 1,
    4, 0x1080000, 0, 0x2980000, 1, 39,
    1, 22, 3, 0xd000, 17,
};

u32 gAlhafra_FirstMadranFollowsFromCell20Script[] = {
    39, 2, 4, 0x1480000, 0, 0x3280000,
    1, 4, 0xf80000, 0, 0x3280000, 1,
    4, 0xf80000, 0, 0x2a80000, 1, 39,
    1, 22, 3, 0xd000, 17,
};

u32 gAlhafra_ElderFollowsFromCell20Script[] = {
    39, 2, 5, 0, 0, 0x100000,
    1, 4, 0x1480000, 0, 0x3280000, 1,
    4, 0xf80000, 0, 0x3280000, 1, 4,
    0xf80000, 0, 0x2b80000, 1, 39, 1,
    22, 3, 0xd000, 17,
};

u32 gAlhafra_SecondSoldierFollowsFromCell20Script[] = {
    39, 2, 5, 0, 0, 0x100000,
    1, 4, 0x1480000, 0, 0x3280000, 1,
    4, 0x1080000, 0, 0x3280000, 1, 4,
    0x1080000, 0, 0x2a80000, 1, 39, 1,
    22, 3, 0xd000, 17,
};

u32 gAlhafra_FirstSoldierFollowsFromCell20Script[] = {
    39, 2, 5, 0, 0, 0x200000,
    1, 4, 0x1480000, 0, 0x3280000, 1,
    4, 0x1080000, 0, 0x3280000, 1, 4,
    0x1080000, 0, 0x2b80000, 1, 39, 1,
    22, 3, 0xd000, 17,
};

s32 gAlhafra_SecondMadranFollowsFromCell20Script[] = {
    39, 2, 5, -0x140000, 0, 0x200000,
    1, 4, 0x1480000, 0, 0x3280000, 1,
    4, 0x1000000, 0, 0x3280000, 1, 4,
    0x1000000, 0, 0x2c80000, 1, 39, 1,
    22, 3, 0xd000, 17,
};
// clang-format on
