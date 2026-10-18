#include "game_state.h"
#include "flags.h"
#include "animspr.h"
#include "version.h"
#include "scene_ids.h"
#include "unit/import.h"
#include "unit/scene/vault_interiors.h"
#include "unit/scene/common/vault_itemobj.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/battle/battle.h"
#include "unit/game/game.h"
#include "unit/runtime/runtime.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "mem.h"
#include "edition.h"
#include "types.h"
#include "callback_scheduler.h"
#include "item_ids.h"
#include "field_event.h"
#include "field_scene.h"
#include "fixed_point_position.h"
#include "tbs_edition.h"
#include "battle_party.h"
#include "text/msg_ids.h"
#include "field_service.h"
#include "ability_ids.h"
#include "objcmd.h"
#include "fixed_math.h"
#include "macros.h"

#if TBS_PATCH_3
#define VAULT_INTERIORS_ENTRY_ACTOR_Z PIXELS(451)
#else
#define VAULT_INTERIORS_ENTRY_ACTOR_Z PIXELS(464)
#endif

// Forward declarations.
static void VaultInteriors_JoinRodSearch(void);
static void VaultInteriors_DiscussMindReading(void);
static void VaultInteriors_AdvanceSuspectRoute(s32 actor, s32 expected,
                                               s32 next, const s32 *actions);
static void VaultInteriors_FaceSuspectsTowardLeader(void);
static void VaultInteriors_LoseToThieves(void);
static void VaultInteriors_ConfrontThievesAgain(void);
static void VaultInteriors_DefeatThieves(void);
static void VaultInteriors_ReturnStolenGoods(void);
static void VaultInteriors_OpenWindowTransition(void);
static void VaultInteriors_ShowMessageAndWait(s32 actorId, s32 waitFrames);
static void VaultInteriors_FaceActorAndWait(s32 actorId, s32 targetId,
                                            s32 waitFrames);
static void VaultInteriors_FaceEachOtherAndWait(s32 firstId, s32 secondId,
                                                s32 waitFrames);
static void VaultInteriors_PlayAnimationAndWait(s32 actorId, s32 animation,
                                                s32 waitFrames);
static void VaultInteriors_BeginMindRead(s32 casterId, s32 targetId);
static void VaultInteriors_EndMindRead(void);
static void VaultInteriors_UpdateActorStops(void);
static void VaultInteriors_StartActorStops(void);

// Return the entrances for the Vault interiors overlay.
struct SceneEntrance *VaultInteriors_GetEntrances(void) {
  return gVaultInteriors_Scripts;
}

// Return the trigger regions for the Vault interiors overlay.
s16 *VaultInteriors_GetRegions(void) { return gVaultInteriors_Regions; }

// Return the exits for the Vault interiors overlay.
u32 *VaultInteriors_GetExits(void) { return gVaultInteriors_Messages; }

// Select the attic placements for entrances 15–17 and prepare the actors.
const struct ScenePlacement *VaultInteriors_GetPlacements(void) {
  struct ScenePlacement *table;

  switch (gGameState.entrance) {
  case 15:
  case 16:
  case 17:
    table = gVaultInteriors_SceneTableB;
    break;
  default:
    table = gVaultInteriors_SceneTableA;
    break;
  }
  FieldScene_PrepareActors(table);
  return table;
}

#if !TBS_PATCH_1
// Jump to the requested destination when its collision check succeeds.
static s32 VaultInteriors_TryJumpTo(struct FixedPointPosition *pos) {
  struct FieldActor *leader;
  u8 *flags;
  u8 savedMotionFlags;

  leader = Object_GetById(0);
  flags = &leader->motionFlags;
  savedMotionFlags = *flags;
  if (FieldObject_CheckMovementCollision(leader, pos) == 0) {
    Engine_EventBegin();
    Object_SetMode(leader, 6);
    Engine_TaskWait(6);
    Engine_AudioPlayCue(152);
    Object_SetMode(leader, 7);
    leader->speed = PIXELS(3);
    leader->acceleration = PIXELS(2);
    leader->velocityY = PIXELS(4);
    *flags &= 0x7e;
    Engine_ActorSetSpriteFlags(leader, 0);
    Engine_ActorMoveToAndWait(0, ((union FieldCoordinate *)&pos->x)->part.pixel,
                              ((union FieldCoordinate *)&pos->z)->part.pixel);
    Object_SetMode(leader, 6);
    Engine_ActorSetSpriteFlags(leader, 1);
    Engine_TaskWait(6);
    *flags = savedMotionFlags;
    Engine_EventEnd();
    return 1;
  }
  return 0;
}
#endif

// Jump across the roof gap and copy the landing side's cell attributes.
static void VaultInteriors_JumpAcrossRoofGap(void) {
  s32 x;
  s32 z;
#if !TBS_PATCH_1
  struct FixedPointPosition target;
  struct FixedPointPosition *pos;
  struct FieldActor *leader;
#endif

  if (Object_GetById(0)->facing >= 0xa000 &&
      Object_GetById(0)->facing <= 0xe000) {
#if TBS_PATCH_1
    Engine_LeaderCheckAhead();
#else
    leader = Object_GetById(0);
    pos = &target;
    SET_POSITION(pos, leader->x.fixed, Object_GetById(0)->y.fixed,
                 Object_GetById(0)->z.fixed - PIXELS(30));
    VaultInteriors_TryJumpTo(pos);
#endif
    x = 42;
    z = 85;
    Map_CopyCellAttributes(41, 85, 1, 1, x, z);
  } else if (Object_GetById(0)->facing >= 0x2000 &&
             Object_GetById(0)->facing <= 0x6000) {
#if TBS_PATCH_1
    Engine_LeaderCheckAhead();
#else
    leader = Object_GetById(0);
    pos = &target;
    SET_POSITION(pos, leader->x.fixed, Object_GetById(0)->y.fixed,
                 Object_GetById(0)->z.fixed + PIXELS(30));
    VaultInteriors_TryJumpTo(pos);
#endif
    x = 42;
    z = 85;
    Map_CopyCellAttributes(43, 85, 1, 1, x, z);
  }
}

// Open the passage once crate 26 reaches column 42.
static void VaultInteriors_OpenMovedCratePassage(void) {
  struct FieldActor *crate;

  Engine_EventBegin();
  crate = Object_GetById(26);
  if ((crate->x.fixed >> 20) == 42) {
    Map_CopyCellAttributes(101, 24, 3, 4, 41, 24);
    Engine_GameFlagSet(0x859);
  }
  Engine_EventEnd();
}

// Open the weapon shop from the counter or speak to its owner.
static void VaultInteriors_TalkToWeaponShopkeeper(void) {
  s32 leaderFacing = Object_GetById(0)->facing;

  Engine_EventBegin();
  if (leaderFacing >= 0xa001 && leaderFacing <= 0xdfff) {
    Engine_ShopOpen(4, 19);
  } else {
    if (Engine_GameFlagIsSet(0x855) == 0) {
      Engine_EventSetMessage(MsgVaultMasterHammetIsntOnlyOne);
    } else {
      Engine_EventSetMessage(MsgVaultWeFoundOurStolenWeapons);
    }
    Engine_EventShowMessage(19, 0);
  }
  Engine_EventEnd();
}

// Open the armor shop from the counter or speak to its owner.
static void VaultInteriors_TalkToArmorShopkeeper(void) {
  s32 leaderFacing = Object_GetById(0)->facing;

  Engine_EventBegin();
  if (leaderFacing >= 0xa001 && leaderFacing <= 0xdfff) {
    Engine_ShopOpen(5, 20);
  } else {
    if (Engine_GameFlagIsSet(0x855) == 0) {
      Engine_EventSetMessage(MsgVaultThievesDidntHitOurHouse);
    } else {
      Engine_EventSetMessage(MsgVaultIfYoureGonnaHeadInto);
    }
    Engine_EventShowMessage(20, 0);
  }
  Engine_EventEnd();
}

// Open the inn from the counter or speak to its owner.
static void VaultInteriors_TalkToInnkeeper(void) {
  s32 leaderFacing = Object_GetById(0)->facing;

  Engine_EventBegin();
  if (leaderFacing >= 0xa001 && leaderFacing <= 0xdfff) {
    Engine_InnOpen(1, 23);
  } else {
    if (Engine_GameFlagIsSet(0x855) == 0) {
      Engine_EventSetMessage(MsgVaultEveryoneThinksOurGuestsThieves);
    } else {
      Engine_EventSetMessage(MsgVaultTheyHidThoseStolenGoods);
    }
    Engine_EventShowMessage(23, 0);
  }
  Engine_EventEnd();
}

// Shared source is included here in ROM order.
#include "common/vault_bone.h"

// Have Garet remind the leader to finish the search.
static void VaultInteriors_RefuseToLeaveSearch(void) {
#if TBS_PATCH_1
  Leader_StopMoving();
#endif
  Engine_EventSetMessage(MsgVaultWeDontHaveTimeFor);
  Engine_EventShowMessage(GARET, 0);
}

// Show the two messages for searching an empty barrel.
static void VaultInteriors_SearchEmptyBarrel(void) {
  Engine_EventBegin();
  Engine_MessageShowCentered(MsgVaultIsaacCheckedBarrel, 1);
  Engine_MessageShowCentered(MsgVaultButDidntFindAnything, 1);
  Engine_EventEnd();
}

// Show the two messages for searching an empty chest.
static void VaultInteriors_SearchEmptyChest(void) {
  Engine_EventBegin();
  Engine_MessageShowCentered(MsgVaultIsaacCheckedChest, 1);
  Engine_MessageShowCentered(MsgVaultButChestWasEmpty, 1);
  Engine_EventEnd();
}

// Choose the attic events or the town events before and after the theft.
const struct SceneEvent *VaultInteriors_GetEvents(void) {
  switch (gGameState.entrance) {
  case 15:
  case 16:
  case 17:
    return gVaultInteriorsEventsEntrances15To17;
  }
  if (Engine_GameFlagIsSet(0x855) != 0) {
    return gVaultInteriorsEventsFlag855;
  }
  return gVaultInteriorsEvents;
}

// Stand the speaker, face the leader, and show the selected message.
static void VaultInteriors_StandAndShowMessage(s32 x) {
  Engine_ActorSetAnimation(x, 1);
  VaultInteriors_FaceEachOtherAndWait(x, 0, 2);
  Engine_EventShowMessage(x, 0);
}

// Shared source is included here in ROM order.
#include "common/vault_talk.h"

// Face the leader and select the next reply when the answer is no.
static void VaultInteriors_AskYesNoAndShowReply(s32 x) {
  VaultInteriors_FaceEachOtherAndWait(x, 0, 2);
  Engine_EventOpenMessage(x, 0);
  if (Engine_EventChooseYesNo(0, 0) != 0) {
    gEventWork->message++;
  }
  Engine_EventShowMessage(x, 0);
}

// Ask whether Ivan's unusual powers are fun.
static void VaultInteriors_AskAboutIvansPowers(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgVaultMisterFunSeeStrangeNew);
  Engine_ActorSetAnimation(11, 1);
  VaultInteriors_AskYesNoAndShowReply(11);
  Engine_EventEnd();
}

// Ask the leader to help Ivan and restore the speaker's facing.
static void VaultInteriors_AskToHelpIvan(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgVaultIvanHasGreatPowersWouldnt);
  VaultInteriors_AskYesNoAndShowReply(15);
  Actor_FaceDirection(15, 0x8000, 0);
  Engine_EventEnd();
}

// Open the item shop or discuss arriving in town and the broken bridge.
static void VaultInteriors_TalkToItemShopkeeper(void) {
  s32 leaderFacing = Object_GetById(0)->facing;

  Engine_EventBegin();
  if (leaderFacing >= 0xa001 && leaderFacing <= 0xdfff) {
    Engine_ShopOpen(6, 21);
  } else if (Engine_GameFlagIsSet(0x855) == 0) {
    Engine_EventSetMessage(MsgVaultDidJustArriveInTown);
    VaultInteriors_AskYesNoAndShowReply(21);
  } else {
    Engine_EventSetMessage(MsgVaultWithBridgeOutWillQuite);
    Engine_EventShowMessage(21, 0);
  }
  Engine_EventEnd();
}

// Discuss the rocks before the theft is solved and the captured thieves
// afterward.
static void VaultInteriors_TalkAboutRocks(void) {
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(0x855) == 0) {
    Engine_EventSetMessage(MsgVaultIfOnlyTheseRocksWere);
  } else {
    Engine_EventSetMessage(MsgVaultThankGoodnessThoseThievesWere);
  }
  VaultInteriors_StandAndShowMessage(9);
  Engine_EventEnd();
}

// Discuss missing possessions or where the thieves hid the treasure.
static void VaultInteriors_TalkAboutMissingTreasure(void) {
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(0x855) != 0) {
    Engine_EventSetMessage(MsgVaultThievesHidStolenTreasureIn);
  } else {
    Engine_EventSetMessage(MsgVaultJustMeOrAmMissing);
  }
  VaultInteriors_StandAndShowMessage(12);
  Engine_EventEnd();
}

// Remember the host's first thanks for agreeing to help Ivan.
static void VaultInteriors_TalkToIvansHost(void) {
  s32 thankedForHelpingFlag;
  thankedForHelpingFlag = 0x851;
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(0x856) != 0) {
    if (Engine_GameFlagIsSet(thankedForHelpingFlag) == 0) {
      Engine_EventSetMessage(MsgVaultYoureGoingHelpIvan);
      VaultInteriors_StandAndShowMessage(16);
      Engine_EventWait(10);
      VaultInteriors_PlayAnimationAndWait(16, 3, 20);
      Engine_GameFlagSet(thankedForHelpingFlag);
    } else {
      Engine_EventSetMessage(MsgVaultPleaseLookAfterIvan);
    }
  } else {
    Engine_EventSetMessage(MsgVaultCouldSomeonePleaseHelpIvan);
  }
  VaultInteriors_StandAndShowMessage(16);
  Engine_EventEnd();
}

// Show the villager's suspicions about the three strangers.
static void VaultInteriors_TalkAboutThreeStrangers(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgVaultThoseThreeStrangersSureHave);
  VaultInteriors_StandAndShowMessage(18);
  Engine_EventEnd();
}

// Choose the first suspect's next route from his facing and six route states.
static void VaultInteriors_MoveFirstSuspectAside(void) {
  struct FieldActor *suspect;
  s16 *routeState;
  s32 routeIndex;
  s32 nextRoute;

  suspect = Object_GetById(24);
  Engine_EventBegin();
  Engine_ActorRunRepeatedMotion(24, 2);
  Engine_EventSetMessage(MsgVaultOwStop);
  Engine_EventShowMessage(24, 0);
  Engine_ActorSetSpeed(24, PIXELS(4), PIXELS(2));
  if ((u32)((suspect->facing & 0xf000) - 0x5000) <= 0x6000) {
    routeState = (s16 *)&suspect->timer;
    routeIndex = *routeState;
    if (routeIndex <= 2) {
      Actor_SetActionScript(24, gVaultInteriors_StepActions[routeIndex]);
      *(u16 *)routeState = *(u16 *)routeState + 1;
    } else {
      Actor_SetActionScript(24, gVaultInteriors_IdleActions[routeIndex]);
      nextRoute = *(u16 *)routeState - 1;
      *(u16 *)routeState = nextRoute;
    }
  } else {
    routeState = (s16 *)&suspect->timer;
    routeIndex = *routeState;
    if (routeIndex > 2) {
      Actor_SetActionScript(24, gVaultInteriors_StepActions[routeIndex]);
      *(u16 *)routeState = *(u16 *)routeState + 1;
    } else {
      Actor_SetActionScript(24, gVaultInteriors_IdleActions[routeIndex]);
      nextRoute = *(u16 *)routeState - 1;
      *(u16 *)routeState = nextRoute;
    }
  }
  if (*routeState > 5) {
    nextRoute = 0;
    *(u16 *)routeState = nextRoute;
  }
  if (*routeState < 0) {
    nextRoute = 5;
    *(u16 *)routeState = nextRoute;
  }
  Object_RefreshSelectorById(24);
  Engine_EventEnd();
}

// Select a response script and advance the second suspect's route state.
static inline void
VaultInteriors_ApplySecondSuspectRoute(struct FieldActor *actor, s32 scriptRow,
                                       s32 routePair, u16 routeDelta) {
  Actor_SetActionScript(
      25, gVaultInteriors_ResponseScripts[scriptRow * 4 + actor->timer]);
  actor->timer = actor->timer - routePair * 2 + routeDelta;
}

// Choose the second suspect's escape route from his facing and route state.
static void VaultInteriors_MoveSecondSuspectAside(void) {
  struct FieldActor *actor;
  u32 facing;
  s32 routePair;

  actor = Object_GetById(25);
  facing = actor->facing & 0xf000;
  routePair = actor->timer >> 1;
  Engine_EventBegin();
  Engine_ActorRunRepeatedMotion(25, 2);
  Engine_EventSetMessage(MsgVaultNoLeaveAlone);
  Engine_EventShowMessage(25, 0);
  Engine_ActorSetSpeed(25, PIXELS(7) / 2, PIXELS(7) / 4);
  switch (actor->timer) {
  case 4:
    if (facing > 0x2000 && facing < 0xa000) {
      Actor_SetActionScript(25, gVaultInteriors_PairScriptR);
      actor->timer = 2;
    } else {
      Actor_SetActionScript(25, gVaultInteriors_PairScriptP);
      actor->timer = 3;
    }
    break;
  case 0:
  case 2:
    if (facing > 0x2000 && facing < 0xa000) {
      VaultInteriors_ApplySecondSuspectRoute(actor, routePair, routePair, 1);
    } else {
      VaultInteriors_ApplySecondSuspectRoute(actor, routePair ^ 1, routePair,
                                             -1);
    }
    break;
  case 1:
  case 3:
    if (facing > 0x6000 && facing < 0xe000) {
      VaultInteriors_ApplySecondSuspectRoute(actor, routePair, routePair, 1);
    } else {
      VaultInteriors_ApplySecondSuspectRoute(actor, routePair ^ 1, routePair,
                                             -1);
    }
    break;
  }
  actor->timer &= 3;
  Object_RefreshSelectorById(25);
  Engine_EventEnd();
}

// Ask the villager where the thieves came from.
static void VaultInteriors_AskWhereThievesCameFrom(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgVaultDidThoseThievesComeFrom);
  VaultInteriors_AskYesNoAndShowReply(10);
  Engine_EventEnd();
}

// Show the child's concern about the worried father.
static void VaultInteriors_TalkAboutWorriedFather(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgVaultMyFatherWorriedAboutThose);
  VaultInteriors_StandAndShowMessage(11);
  Engine_EventEnd();
}

// Ask whether the party defeated the thieves.
static void VaultInteriors_AskAboutDefeatingThieves(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgVaultHeardDefeatedThoseThieves);
  VaultInteriors_AskYesNoAndShowReply(14);
  Engine_EventEnd();
}

// Give the one-time Water of Life reward when inventory space permits.
static void VaultInteriors_ReceiveWaterOfLife(void) {
  Engine_EventBegin();
  if (GameFlag_IsSet(0x857) == 0) {
    Engine_EventSetMessage(MsgVaultYouWereSuchGreatHelp);
    VaultInteriors_ShowMessageAndWait(16, 20);
    VaultInteriors_PlayAnimationAndWait(16, 3, 20);
    VaultInteriors_ShowMessageAndWait(16, 30);
    Actor_FaceDirection(16, 0, 0);
    Engine_EventWait(30);
    Engine_ActorRunRepeatedMotion(16, 2);
    Engine_EventWait(30);
    VaultInteriors_FaceEachOtherAndWait(0, 16, 20);
    VaultInteriors_PlayAnimationAndWait(16, 3, 20);
    gEventWork->message++;
    if (PartyInventory_HasSpace() == 0) {
      Engine_EventSetMessage(MsgVaultYouWereSuchGreatHelp + 3);
      VaultInteriors_ShowMessageAndWait(16, 20);
      Engine_EventEnd();
      return;
    }
    GameFlag_Set(0x857);
    Engine_PartyGiveItem(ITEM_WATER_OF_LIFE, 0);
  }
  Engine_EventSetMessage(MsgVaultHeadingOutBeyondGomaRange);
  Event_OpenMessage(16, 0);
  Engine_EventWait(20);
  if (Engine_EventChooseYesNo(0, 0) != 0) {
    gEventWork->message++;
  }
  Event_ShowMessage(16, 0);
  Engine_EventEnd();
}

// Offer another bone and give it only when the player accepts and has room.
static void VaultInteriors_OfferBone(void) {
  Engine_EventBegin();
  VaultInteriors_FaceEachOtherAndWait(18, 0, 2);
  if (GameFlag_IsSet(FLAG_VAULT_BONE_RECEIVED) == 0) {
    Engine_EventSetMessage(MsgVaultHaveLotLeftoverBonesFrom);
    Event_OpenMessage(18, 0);
  } else {
    Engine_EventSetMessage(MsgVaultWantMoreBones);
    Event_OpenMessage(18, 0);
  }
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_EventWait(20);
    Event_ShowMessage(18, 0);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(18, 2);
    Engine_EventWait(20);
    if (PartyInventory_HasSpace() == 0) {
      Engine_ActorSetAnimationAndWait(18, 4);
      Engine_EventWait(20);
      Engine_EventSetMessage(MsgVaultWowHaveManyThingsArent);
      Event_ShowMessage(18, 0);
    } else {
      Engine_ItemShowFound(ITEM_BONE, 3);
      Engine_PartyGiveItem(ITEM_BONE, 0);
      GameFlag_Set(FLAG_VAULT_BONE_RECEIVED);
    }
  } else {
    gEventWork->message++;
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(18, 3);
    Engine_EventWait(20);
    Event_ShowMessage(18, 0);
  }
  Actor_FaceDirection(18, 0x4000, 0);
  Engine_EventEnd();
}

// Read the rock seeker's thoughts before or after the theft is solved.
static void VaultInteriors_ReadRockSeekersMind(void) {
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(0x855) == 0) {
    Engine_EventSetMessage(MsgVaultOkMisterLetMeSee);
  } else {
    Engine_EventSetMessage(MsgVaultIfRockWorthlessMaybeThats);
  }
  SceneActor_RunActorCommandWithFlag91(9);
  Engine_EventEnd();
}

// Read the child's thoughts about the outside world or worried father.
static void VaultInteriors_ReadChildsMind(void) {
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(0x855) == 0) {
    Engine_EventSetMessage(MsgVaultWonderOutsideWorldLike);
  } else {
    Engine_EventSetMessage(MsgVaultFatherLooksSadWorryingLike);
  }
  SceneActor_RunActorCommandWithFlag91(11);
  Engine_EventEnd();
}

// Read the villager's thoughts about the thieves and missing goods.
static void VaultInteriors_ReadTreasureSeekersMind(void) {
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(0x855) == 0) {
    Engine_EventSetMessage(MsgVaultCouldTheyThievesGoodDont);
  } else {
    Engine_EventSetMessage(MsgVaultGuessNothingInOurHouse);
  }
  SceneActor_RunActorCommandWithFlag91(12);
  Engine_EventEnd();
}

// Read the host's thoughts about Ivan or the dangerous cave.
static void VaultInteriors_ReadIvansHostsMind(void) {
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(0x855) == 0) {
    Engine_EventSetMessage(MsgVaultTicklesBeingTickledByBoy);
  } else {
    Engine_EventSetMessage(MsgVaultCaveInGomaRangeDangerous);
  }
  SceneActor_RunActorCommandWithFlag91(16);
  Engine_EventEnd();
}

// Read the bone owner's thoughts, including whether a bone has been taken.
static void VaultInteriors_ReadBoneOwnersMind(void) {
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(0x855) == 0) {
    Engine_EventSetMessage(MsgVaultMasterHisWifeBlindedBy);
  } else if (Engine_GameFlagIsSet(FLAG_VAULT_BONE_RECEIVED) == 0) {
    Engine_EventSetMessage(MsgVaultGeeAlwaysGetHungryWhen);
  } else {
    Engine_EventSetMessage(MsgVaultHeReallyLikesBonesWonder);
  }
  SceneActor_RunActorCommandWithFlag91(18);
  Engine_EventEnd();
}

// Introduce Ivan's powers and ask the party to help find the stolen rod.
static void VaultInteriors_MeetIvan(void) {

  struct FieldActor *actor;

  Engine_EventBegin();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(GARET, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(IVAN, PIXELS(4) / 5, PIXELS(2) / 5);
  Event_PlayCue(19);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 384, 408);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
  Actor_SetPosition(GARET, PIXELS(384), PIXELS(408));
  Actor_WalkToAndWait(GARET, 368, 408);
  Actor_FaceDirection(GARET, 0xd000, 20);
  // Introduce Ivan's ability only on the first meeting.
  if (GameFlag_IsSet(0x850) == 0) {
    GameFlag_Set(0x850);
    VaultInteriors_BeginMindRead(2, 0);
    Engine_EventWait(40);
    VaultInteriors_EndMindRead();
    Engine_EventSetMessage(MsgVaultDoPossessStrangePowers);
    Event_PlayCue(60);
    Engine_EventWait(30);
    VaultInteriors_PlayAnimationAndWait(2, 3, 30);
    VaultInteriors_ShowMessageAndWait(2, 30);
    Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Engine_ActorRunRepeatedMotion(GARET, 1);
    Engine_EventWait(20);
    VaultInteriors_BeginMindRead(2, 0);
    Engine_EventWait(40);
    VaultInteriors_EndMindRead();
    VaultInteriors_ShowMessageAndWait(2, 30);
    VaultInteriors_FaceEachOtherAndWait(0, 1, 50);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_FaceDirection(GARET, 0xd000, 0);
    Engine_EventWait(20);
    VaultInteriors_BeginMindRead(2, 0);
    Engine_EventWait(40);
    VaultInteriors_EndMindRead();
    Engine_ActorRunRepeatedMotion(IVAN, 1);
    VaultInteriors_ShowMessageAndWait(2, 50);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    VaultInteriors_PlayAnimationAndWait(1, 3, 20);
    VaultInteriors_PlayAnimationAndWait(2, 3, 20);
    VaultInteriors_ShowMessageAndWait(2, 40);
    Engine_ActorRunRepeatedMotion(IVAN, 1);
    Engine_EventWait(30);
    Actor_FaceDirection(2, 0xc000, 0);
    Engine_EventWait(30);
    Actor_WalkToAndWait(IVAN, 376, 376);
    Engine_EventWait(40);
    VaultInteriors_FaceEachOtherAndWait(0, 1, 50);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_FaceDirection(GARET, 0xd000, 0);
    Engine_ActorRunRepeatedMotion(IVAN, 1);
    Engine_EventWait(50);
    VaultInteriors_PlayAnimationAndWait(2, 3, 30);
    Actor_FaceDirection(IVAN, 0x4000, 0);
    Engine_EventWait(10);
    Event_ShowMessage(IVAN, 0);
    Event_OpenMessage(IVAN, 0);
  } else {
    Event_PlayCue(60);
    Engine_EventSetMessage(MsgVaultWouldReallyWouldHelpMe);
    Event_OpenMessage(IVAN, 0);
  }
  // Accepting the search lets Ivan merge into the leader's position.
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    VaultInteriors_JoinRodSearch();
    GameFlag_Set(0x856);
    Engine_ActorSetAnimation(IVAN, 2);
    actor = Object_GetById(0);
    if (actor != 0) {
      Actor_SetDestination(IVAN, actor->x.part.pixel, actor->z.part.pixel);
    }
    Engine_ActorWaitForMove(IVAN);
    Actor_SetPosition(IVAN, 0, 0);
  } else {
    Event_ShowMessage(IVAN, 0);
  }
  Engine_ActorSetAnimation(GARET, 2);
  actor = Object_GetById(0);
  if (actor != 0) {
    Actor_SetDestination(GARET, actor->x.part.pixel, actor->z.part.pixel);
  }
  Engine_ActorWaitForMove(GARET);
  Actor_SetPosition(GARET, 0, 0);
  Audio_PlayCueFromEventWork();
  Engine_EventEnd();
}

// Have Ivan join the search and demonstrate Mind Read to the party.
static void VaultInteriors_JoinRodSearch(void) {
  s32 message;

  Engine_EventWait(20);
  Object_GetById(2)->paused = 0;
  Engine_ActorJump(2, 4, 0);
  Engine_EventWait(40);
  message = MsgVaultThankHelpBelieve;
  Engine_EventSetMessage(message);
  Engine_EventWait(20);
  VaultInteriors_ShowMessageAndWait(2, 20);
  Actor_ShowEmote(2, 0x101, 0);
  Engine_EventWait(50);
  VaultInteriors_ShowMessageAndWait(2, 30);
  Actor_WalkToAndWait(2, 376, 392);
  VaultInteriors_BeginMindRead(2, 0);
  Engine_EventWait(40);
  VaultInteriors_EndMindRead();
  Engine_ActorRunRepeatedMotion(0, 1);
  Object_GetById(0)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(0, 384, 424);
  Engine_EventWait(1);
  Object_GetById(0)->steeringFlags |= ACTOR_FACES_MOTION;
  Engine_EventWait(30);
  Engine_EventShowMessage(2, 0);
  // Demonstrate Mind Read and let Garet and Isaac react to Ivan's explanation.
  Actor_SetAttachedEffect(0, 0x102);
  Actor_SetAttachedEffect(1, 0x102);
  Engine_EventWait(60);
  Engine_ActorSetAnimation(0, 3);
  VaultInteriors_PlayAnimationAndWait(1, 3, 30);
  Actor_ShowEmote(2, 0x101, 0);
  Engine_EventWait(60);
  Engine_ActorRunRepeatedMotion(2, 1);
  Engine_EventWait(10);
  VaultInteriors_PlayAnimationAndWait(2, 3, 20);
  Engine_EventShowMessage(2, 0);
  Engine_ActorRunRepeatedMotion(1, 2);
  Engine_EventWait(10);
  VaultInteriors_FaceActorAndWait(1, 0, 30);
  Engine_ActorRunRepeatedMotion(0, 2);
  Engine_EventWait(10);
  VaultInteriors_FaceActorAndWait(0, 1, 40);
  Actor_ShowEmote(0, 0x102, 0);
  Actor_ShowEmote(1, 0x102, 0);
  Engine_EventWait(60);
  Actor_ShowEmote(2, 0x102, 0);
  Engine_EventWait(60);
  Engine_EventOpenMessage(2, 0);
  Engine_ActorFaceActor(0, 2, 0);
  Engine_ActorFaceActor(1, 2, 0);
  Engine_EventChooseYesNo(0, 0);
  Engine_EventWait(30);
  VaultInteriors_PlayAnimationAndWait(2, 3, 10);
  Actor_WalkToAndWait(2, 384, 408);
  Engine_EventWait(10);
  Engine_MessageShowCentered(message + 5, 1);
  Engine_EventSetMessage(message + 6);
  Engine_ActorFaceEachOther(2, 1, 0);
  VaultInteriors_FaceActorAndWait(0, 1, 20);
  Actor_ShowEmote(1, 0x102, 0);
  Engine_EventWait(60);
  Engine_ActorRunRepeatedMotion(1, 1);
  Object_GetById(1)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(1, 352, 408);
  Engine_EventWait(1);
  Object_GetById(1)->steeringFlags |= ACTOR_FACES_MOTION;
  Engine_EventWait(10);
  Actor_WalkTo(2, 368, 408);
  Engine_EventWait(20);
  Actor_WalkToAndWait(0, 368, 424);
  Engine_ActorFaceActor(0, 1, 0);
  Engine_ActorWaitForMove(2);
  VaultInteriors_BeginMindRead(2, 1);
  VaultInteriors_PlayAnimationAndWait(1, 4, 10);
  Engine_EventShowMessage(1, 0);
  VaultInteriors_EndMindRead();
  VaultInteriors_FaceEachOtherAndWait(2, 0, 30);
  Engine_EventShowMessage(2, 0);
  Engine_ActorRunRepeatedMotion(0, 1);
  Engine_EventWait(30);
  VaultInteriors_PlayAnimationAndWait(0, 3, 30);
  VaultInteriors_PlayAnimationAndWait(2, 3, 10);
  Engine_EventShowMessage(2, 0);
  Engine_ActorRunRepeatedMotion(1, 2);
  Engine_ActorFaceActor(1, 0, 0);
  Engine_EventShowMessage(1, 0);
  Engine_ActorFaceActor(0, 1, 0);
  VaultInteriors_PlayAnimationAndWait(0, 3, 40);
  Engine_ActorFaceActor(0, 2, 0);
  VaultInteriors_FaceActorAndWait(1, 2, 30);
  Actor_ShowEmote(0, 0x101, 0);
  Actor_ShowEmote(1, 0x101, 0);
  Engine_EventWait(60);
  VaultInteriors_PlayAnimationAndWait(2, 3, 20);
  Engine_EventShowMessage(2, 0);
  Engine_ActorShowEmote(1, 0x103, 0);
  Engine_EventWait(60);
  VaultInteriors_FaceActorAndWait(1, 0, 10);
  Engine_EventShowMessage(1, 0);
  Engine_ActorShowEmote(2, 0x100, 0);
  Engine_EventWait(60);
  Engine_EventShowMessage(2, 0);
  Actor_ShowEmote(1, 0x102, 0);
  Engine_EventWait(60);
  Engine_MessageShowCentered(message + 13, 1);
  Engine_EventSetMessage(message + 14);
  Engine_ActorFaceEachOther(2, 0, 0);
  Engine_ActorFaceActor(1, 0, 0);
  Engine_ActorShowEmote(0, 0x102, 0);
  Engine_EventWait(60);
  VaultInteriors_BeginMindRead(2, 0);
  Ui_SetBank15PaletteAndClearRenderMode();
  VaultInteriors_ShowMessageAndWait(1, 30);
  VaultInteriors_EndMindRead();
  Engine_ActorFaceEachOther(2, 1, 0);
  VaultInteriors_PlayAnimationAndWait(2, 3, 10);
  VaultInteriors_ShowMessageAndWait(2, 20);
  Engine_ActorFaceEachOther(2, 0, 0);
  VaultInteriors_PlayAnimationAndWait(0, 3, 20);
  Engine_ActorRunRepeatedMotion(1, 1);
  Engine_EventWait(20);
  VaultInteriors_PlayAnimationAndWait(1, 4, 10);
  VaultInteriors_ShowMessageAndWait(1, 30);
  Engine_ActorFaceActor(2, 1, 0);
  Actor_ShowEmote(2, 0x101, 0);
  Engine_EventWait(30);
  VaultInteriors_FaceEachOtherAndWait(0, 1, 10);
  Engine_ActorSetAnimation(0, 3);
  VaultInteriors_PlayAnimationAndWait(1, 3, 30);
  Engine_ActorFaceActor(0, 2, 0);
  Engine_ActorFaceActor(1, 2, 0);
  Engine_ActorFaceActor(2, 0, 0);
  Engine_ActorShowEmote(2, 0x106, 0);
  Engine_EventWait(60);
  VaultInteriors_ShowMessageAndWait(2, 10);
  Engine_ActorSetAnimation(0, 3);
  VaultInteriors_PlayAnimationAndWait(1, 3, 20);
  VaultInteriors_PlayAnimationAndWait(2, 4, 20);
  VaultInteriors_ShowMessageAndWait(2, 20);
  Actor_ShowEmote(0, 0x101, 0);
  Engine_ActorShowEmote(1, 0x101, 0);
  Engine_EventWait(60);
  VaultInteriors_PlayAnimationAndWait(2, 3, 10);
  VaultInteriors_ShowMessageAndWait(2, 30);
  Engine_ActorShowEmote(0, 0x105, 0);
  Engine_EventWait(60);
  VaultInteriors_PlayAnimationAndWait(2, 4, 10);
  VaultInteriors_ShowMessageAndWait(2, 30);
  VaultInteriors_PlayAnimationAndWait(2, 3, 10);
  Engine_ActorSetAnimation(0, 3);
  VaultInteriors_PlayAnimationAndWait(1, 3, 20);
  Event_PrepareObjectAndApplyValue(2, 1);
  Engine_EventSetMessage(message + 22);
  Engine_ActorRunRepeatedMotion(2, 1);
  Engine_EventShowMessage(2, 0);
}

// Free the captive, confront the three thieves, and start their battle.
static void VaultInteriors_FreeCaptiveAndConfrontThieves(void) {
  s32 base;

  Engine_EventBegin();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(GARET, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(IVAN, PIXELS(4) / 5, PIXELS(2) / 5);
  Engine_ActorRunRepeatedMotion(0, 3);
  Engine_EventWait(20);
  base = MsgVaultVaultCutFree;
  Engine_MessageShowCentered(base, 1);
  base++;
  Engine_EventSetMessage(base);
  Engine_EventWait(30);
  Engine_ActorSetAnimation(8, 1);
  Engine_EventWait(60);
  VaultInteriors_PlayAnimationAndWait(8, 3, 40);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 792, 440);
  VaultInteriors_FaceActorAndWait(0, 8, 20);
  Actor_SetPosition(GARET, PIXELS(792), PIXELS(440));
  Actor_SetPosition(IVAN, PIXELS(792), PIXELS(440));
  Actor_WalkTo(GARET, 808, 432);
  Actor_WalkTo(IVAN, 792, 456);
  Engine_ActorWaitForMove(1);
  Engine_ActorFaceActor(1, 8, 0);
  Engine_ActorWaitForMove(2);
  VaultInteriors_FaceActorAndWait(2, 8, 60);
  Actor_ShowEmote(8, 258, 0);
  Engine_EventWait(60);
  VaultInteriors_ShowMessageAndWait(8, 20);
  Engine_ActorSetAnimation(2, 3);
  VaultInteriors_PlayAnimationAndWait(1, 3, 30);
  Engine_ActorRunRepeatedMotion(8, 2);
  Engine_EventWait(20);
  VaultInteriors_ShowMessageAndWait(8, 20);
  Actor_ShowEmote(IVAN, 258, 0);
  Actor_ShowEmote(GARET, 258, 0);
  Engine_EventWait(60);
  Actor_FaceDirection(8, 12288, 0);
  Engine_EventWait(10);
  VaultInteriors_ShowMessageAndWait(8, 30);
  Actor_FaceDirection(8, 53248, 0);
  Engine_EventWait(30);
  Actor_ShowEmote(8, 256, 0);
  Engine_EventWait(60);
  Actor_FaceDirection(8, 45056, 0);
  Engine_EventWait(40);
  Actor_FaceDirection(8, 53248, 0);
  Engine_EventWait(40);
  VaultInteriors_FaceActorAndWait(8, 0, 20);
  VaultInteriors_PlayAnimationAndWait(8, 4, 30);
  VaultInteriors_ShowMessageAndWait(8, 20);
  Engine_ActorRunRepeatedMotion(2, 1);
  VaultInteriors_ShowMessageAndWait(2, 40);
  // Bring the thieves into the attic confrontation.
  Actor_SetPosition(10, PIXELS(744), PIXELS(408));
  Engine_AudioPlayCue(61);
  VaultInteriors_ShowMessageAndWait(10, 20);
  Engine_ActorStartRepeatedMotion(0, 1);
  Engine_ActorStartRepeatedMotion(1, 1);
  Engine_ActorStartRepeatedMotion(2, 1);
  Engine_ActorRunRepeatedMotion(8, 1);
  Engine_EventWait(30);
  Engine_ActorFaceActor(0, 10, 0);
  Engine_ActorFaceActor(1, 10, 0);
  Engine_ActorFaceActor(2, 10, 0);
  VaultInteriors_FaceActorAndWait(8, 10, 40);
  Actor_SetSpeed(10, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(11, PIXELS(3) / 2, PIXELS(3) / 4);
  Actor_SetSpeed(12, PIXELS(3) / 2, PIXELS(3) / 4);
  Actor_SetPosition(11, PIXELS(744), PIXELS(408));
  Actor_SetPosition(12, PIXELS(744), PIXELS(408));
  Actor_WalkToAndWait(10, 792, 416);
  Actor_FaceDirection(10, 12288, 0);
  Engine_EventWait(70);
  Engine_ActorFaceActor(0, 10, 0);
  Actor_FaceActor(GARET, 10, 0);
  Engine_ActorFaceActor(2, 10, 0);
  Engine_ActorFaceActor(8, 10, 0);
  Actor_SetActionScript(11, gVaultInteriors_Villager11ExitScript);
  Engine_EventWait(40);
  Actor_SetActionScript(12, gVaultInteriors_Villager12ExitScript);
  Object_RefreshSelectorById(12);
  Actor_FaceDirection(11, 8192, 0);
  Actor_FaceDirection(12, 8192, 0);
  Engine_EventWait(40);
  VaultInteriors_PlayAnimationAndWait(10, 4, 20);
  VaultInteriors_ShowMessageAndWait(10, 20);
  Engine_ActorRunRepeatedMotion(11, 1);
  Engine_EventWait(10);
  VaultInteriors_ShowMessageAndWait(11, 20);
  Engine_ActorRunRepeatedMotion(12, 1);
  Engine_EventWait(10);
  VaultInteriors_ShowMessageAndWait(12, 40);
  Actor_ShowEmote(IVAN, 257, 0);
  Engine_EventWait(60);
  VaultInteriors_PlayAnimationAndWait(10, 4, 20);
  VaultInteriors_ShowMessageAndWait(10, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, 257, 0);
  Actor_ShowEmote(GARET, 257, 0);
  Engine_EventWait(60);
  Actor_ShowEmote(8, 256, 0);
  Engine_EventWait(40);
  Engine_ActorRunRepeatedMotion(8, 2);
  Engine_EventWait(20);
  VaultInteriors_ShowMessageAndWait(8, 20);
  Actor_FaceActor(ACTOR_PARTY_LEADER, 8, 0);
  Engine_ActorFaceActor(1, 8, 0);
  VaultInteriors_FaceActorAndWait(2, 8, 20);
  VaultInteriors_PlayAnimationAndWait(8, 4, 30);
  VaultInteriors_ShowMessageAndWait(8, 20);
  Engine_ActorRunRepeatedMotion(2, 2);
  Engine_EventWait(20);
  Engine_ActorFaceActor(0, 2, 0);
  VaultInteriors_FaceActorAndWait(1, 2, 30);
  VaultInteriors_ShowMessageAndWait(1, 20);
  Actor_ShowEmote(8, 258, 0);
  Engine_EventWait(60);
  VaultInteriors_ShowMessageAndWait(8, 20);
  Engine_ActorRunRepeatedMotion(10, 1);
  Engine_EventWait(10);
  VaultInteriors_ShowMessageAndWait(10, 20);
  Engine_ActorFaceActor(0, 10, 0);
  Actor_FaceActor(GARET, 10, 0);
  VaultInteriors_FaceActorAndWait(2, 10, 20);
  VaultInteriors_PlayAnimationAndWait(11, 3, 20);
  VaultInteriors_ShowMessageAndWait(11, 20);
  VaultInteriors_PlayAnimationAndWait(12, 4, 20);
  VaultInteriors_ShowMessageAndWait(12, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, 258, 0);
  Actor_ShowEmote(GARET, 258, 0);
  Actor_ShowEmote(IVAN, 258, 0);
  Engine_EventWait(60);
  VaultInteriors_PlayAnimationAndWait(10, 3, 20);
  Engine_EventOpenMessage(10, 0);
  Engine_EventWait(50);
  Engine_ActorFaceActor(2, 0, 0);
  VaultInteriors_FaceActorAndWait(1, 0, 30);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_EventWait(40);
    Engine_ActorFaceActor(1, 10, 0);
    Actor_FaceActor(IVAN, 10, 0);
    Engine_ActorRunRepeatedMotion(10, 2);
    Engine_EventWait(20);
    Engine_EventShowMessage(10, 0);
  } else {
    Engine_EventWait(40);
    Engine_ActorFaceActor(1, 10, 0);
    Engine_ActorFaceActor(2, 10, 0);
    base = MsgVaultVaultThievesCaught;
    Engine_EventSetMessage(base);
    Engine_ActorRunRepeatedMotion(10, 2);
    Engine_EventWait(20);
    Engine_EventShowMessage(10, 0);
    base -= 3;
    Engine_EventSetMessage(base);
  }
  Actor_ShowEmote(11, 259, 0);
  Engine_EventWait(60);
  VaultInteriors_ShowMessageAndWait(11, 20);
  VaultInteriors_PlayAnimationAndWait(12, 4, 20);
  VaultInteriors_ShowMessageAndWait(12, 30);
  VaultInteriors_FaceEachOtherAndWait(10, 11, 30);
  Engine_ActorSetAnimation(10, 3);
  VaultInteriors_PlayAnimationAndWait(11, 3, 30);
  VaultInteriors_FaceEachOtherAndWait(10, 12, 30);
  Engine_ActorSetAnimation(10, 3);
  VaultInteriors_PlayAnimationAndWait(12, 3, 40);
  Actor_FaceDirection(10, 12288, 0);
  Actor_FaceDirection(11, 12288, 0);
  Actor_FaceDirection(12, 12288, 0);
  Engine_EventWait(20);
  VaultInteriors_PlayAnimationAndWait(10, 4, 20);
  Engine_EventShowMessage(10, 0);
  // Preserve the return destinations before requesting the thieves' encounter.
  GameFlag_Set(2132);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
  base = (s32)&SceneId_VaultInteriors;
  Event_SetNextDestination(base, 17);
  Event_SetDefeatDestination(base, 16);
  Battle_SetStart(3);
  Field_RequestBattleFromEncounterTable(12, 5);
  Engine_EventEnd();
}

// Play the door cue and request exit 11.
static void VaultInteriors_LeaveThroughExit11(void) {
  Engine_AudioPlayCue(123);
  Engine_EventRequestExit(11);
}

// Walk through the doorway and request exit 15 with a short transition.
static void VaultInteriors_EnterThroughExit15(void) {
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1) / 2, PIXELS(1) / 4);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 728, 408);
  gEventWork->transitionFrames = 16;
  Event_PlayCue(123);
  Engine_EventRequestExit(15);
}

// Walk through the doorway, hear the captive if needed, and request exit 14.
static void VaultInteriors_EnterThroughExit14(void) {
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1) / 2, PIXELS(1) / 4);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 744, 408);
  if (GameFlag_IsSet(0x854) == 0) {
    Engine_EventBegin();
    Engine_EventSetMessage(MsgVaultUhnnUhnn);
    Event_ShowMessage(8, 0);
    Engine_EventEnd();
  }
  gEventWork->transitionFrames = 16;
  Engine_AudioPlayCue(123);
  Engine_EventRequestExit(14);
}

// Approach the two suspects and start their evasive routes.
static void VaultInteriors_ApproachSuspects(void) {

  struct FieldActor *actor24;
  struct FieldActor *actor25;

  actor24 = Object_GetById(24);
  actor25 = Object_GetById(25);
  Engine_EventBegin();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(GARET, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(IVAN, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 232, 696);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 200, 696);
  Engine_EventWait(10);
  Actor_ShowEmote(25, 256, 0);
  Actor_ShowEmote(24, 256, 0);
  Engine_EventWait(60);
  VaultInteriors_FaceActorAndWait(25, 0, 10);
  Engine_ActorRunRepeatedMotion(24, 2);
  Engine_EventWait(20);
  Engine_EventSetMessage(MsgVaultWant);
  VaultInteriors_ShowMessageAndWait(24, 20);
  Actor_SetAttachedEffect(25, 258);
  Engine_EventWait(60);
  VaultInteriors_ShowMessageAndWait(25, 20);
  Engine_ActorRunRepeatedMotion(24, 1);
  VaultInteriors_ShowMessageAndWait(24, 30);
  Actor_SetSpeed(24, PIXELS(4), PIXELS(2));
  Actor_SetSpeed(25, PIXELS(7) / 2, PIXELS(7) / 4);
  Actor_SetActionScript(25, gVaultInteriors_PairScriptO);
  Actor_SetActionScript(24, gVaultInteriors_PairScriptB);
  Object_RefreshSelectorById(24);
  Map_CopyCellAttributes(14, 45, 3, 1, 14, 44);
  GameFlag_Set(2130);
  GameFlag_Set(768);
  FieldScheduler_AddCallback(VaultInteriors_FaceSuspectsTowardLeader, 3200);
  actor24->timer = 1;
  actor25->timer = 3;
  Engine_EventEnd();
}

// Move the party aside to discuss the suspects, then reset their positions.
static void VaultInteriors_DiscussSuspiciousGuests(void) {
  struct FieldActor *actor24;
  struct FieldActor *actor25;
  s32 base;

  actor24 = Object_GetById(24);
  actor25 = Object_GetById(25);
  Engine_EventBegin();
  Scheduler_RemoveCallback(VaultInteriors_FaceSuspectsTowardLeader);
  GameFlag_Clear(0x300);
  if (actor24->timer <= 3) {
    Actor_SetActionScript(24, gVaultInteriors_PairScriptG);
  } else {
    Actor_SetActionScript(24, gVaultInteriors_PairScriptF);
  }
  if (actor25->timer <= 2) {
    Actor_SetActionScript(25, gVaultInteriors_PairScriptK);
  } else {
    Actor_SetActionScript(25, gVaultInteriors_PairScriptF);
  }
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(GARET, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(IVAN, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 248, 728);
  Actor_SetPosition(IVAN, PIXELS(248), PIXELS(728));
  Actor_SetPosition(GARET, PIXELS(248), PIXELS(728));
  Actor_WalkTo(IVAN, 264, 744);
  Actor_WalkToAndWait(GARET, 232, 744);
  Engine_ActorWaitForMove(IVAN);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 0);
  Actor_FaceActor(GARET, ACTOR_PARTY_LEADER, 0);
  VaultInteriors_FaceActorAndWait(2, 0, 30);
  Engine_ActorRunRepeatedMotion(IVAN, 1);
  base = MsgVaultTheyreActingSuspiciousSomethingsNot;
  Engine_EventSetMessage(base);
  Event_ShowMessage(IVAN, 0);
  Actor_FaceActor(ACTOR_PARTY_LEADER, IVAN, 0);
  VaultInteriors_FaceActorAndWait(1, 2, 20);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
  VaultInteriors_PlayAnimationAndWait(1, 3, 20);
  VaultInteriors_FaceEachOtherAndWait(0, 1, 10);
  Event_OpenMessage(GARET, 0);
  if (Engine_EventChooseYesNo(0, 0) != 0) {
    gEventWork->message++;
  }
  VaultInteriors_ShowMessageAndWait(1, 30);
  Engine_EventSetMessage((base + 4));
  Actor_FaceActor(ACTOR_PARTY_LEADER, IVAN, 0);
  VaultInteriors_FaceActorAndWait(1, 2, 50);
  Actor_ShowEmote(IVAN, 0x100, 0);
  Engine_EventWait(60);
  VaultInteriors_FaceEachOtherAndWait(0, 1, 50);
  Actor_FaceActor(ACTOR_PARTY_LEADER, IVAN, 0);
  VaultInteriors_FaceActorAndWait(1, 2, 30);
  VaultInteriors_PlayAnimationAndWait(2, 3, 10);
  VaultInteriors_ShowMessageAndWait(2, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 0);
  Actor_ShowEmote(GARET, 0x102, 0);
  Engine_EventWait(60);
  VaultInteriors_PlayAnimationAndWait(2, 3, 20);
  VaultInteriors_ShowMessageAndWait(2, 30);
  Engine_EventSetMessage(MsgVaultIsaacTakeLead);
  Event_ShowMessage(GARET, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 0);
  Engine_EventWait(40);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
  Engine_ActorSetAnimation(GARET, 3);
  VaultInteriors_PlayAnimationAndWait(2, 3, 50);
  Actor_WalkTo(IVAN, 248, 728);
  Actor_WalkToAndWait(GARET, 248, 728);
  Actor_SetPosition(GARET, 0, 0);
  Actor_SetPosition(IVAN, 0, 0);
  Actor_SetPosition(24, PIXELS(104), PIXELS(696));
  Actor_SetPosition(25, PIXELS(120), PIXELS(696));
  Actor_FaceDirection(24, 0, 0);
  Actor_FaceDirection(25, 0x8000, 0);
  Map_CopyCellAttributes(14, 50, 3, 1, 14, 44);
  Engine_EventEnd();
}

// Enable the chase flags and start the movement callback.
static void VaultInteriors_BeginCorneringSuspects(void) {
  Engine_GameFlagSet(0x107);
  Engine_GameFlagSet(FLAG_SCENE_TEMP_50);
  VaultInteriors_StartActorStops();
}

// Have the suspects react to the party's return and reset their route states.
static void VaultInteriors_ResetSuspectRoutes(void) {
  Engine_EventWait(30);
  Engine_ActorRunRepeatedMotion(24, 1);
  Engine_EventWait(20);
  Engine_EventSetMessage(MsgVaultTheyreBack);
  VaultInteriors_ShowMessageAndWait(24, 20);
  Actor_FaceDirection(25, 0, 20);
  Actor_SetAttachedEffect(25, 0x102);
  Engine_ActorRunRepeatedMotion(25, 2);
  VaultInteriors_ShowMessageAndWait(25, 20);
  Engine_ActorSetAnimationAndWait(24, 4);
  Engine_EventWait(20);
  VaultInteriors_ShowMessageAndWait(24, 20);
  Actor_SetSpeed(24, PIXELS(4), PIXELS(2));
  Actor_SetSpeed(25, PIXELS(7) / 2, PIXELS(7) / 4);
  Actor_SetActionScript(25, gVaultInteriors_PairScriptO);
  Actor_SetActionScript(24, gVaultInteriors_PairScriptB);
  Object_RefreshSelectorById(24);
  {
    struct FieldActor *suspect = Object_GetById(24);
    u16 routeIndex = 1;

    suspect->timer = routeIndex;
  }
  {
    struct FieldActor *suspect = Object_GetById(25);
    u16 routeIndex = 3;

    suspect->timer = routeIndex;
  }
  Map_CopyCellAttributes(14, 48, 4, 1, 14, 44);
}

// Set the three party images to the priority used during the mind-reading
// scene.
static inline void VaultInteriors_SetPartyImagePriority(void) {
  {
    struct AnimationObject *sprite = Object_GetById(0)->sprite;
    sprite->image.priority = 1;
  }
  {
    struct AnimationObject *sprite = Object_GetById(1)->sprite;
    sprite->image.priority = 1;
  }
  {
    struct AnimationObject *sprite = Object_GetById(2)->sprite;
    sprite->image.priority = 1;
  }
}

// Select the cornered suspect, cast Mind Read, and discuss the result.
static void VaultInteriors_ReadCorneredSuspectsMinds(void) {
  s32 actor;
  struct EventWork *work;

  work = gEventWork;
  Engine_EventBegin();
  Scheduler_RemoveCallback(VaultInteriors_UpdateActorStops);
  GameFlag_Clear(0x107);
  GameFlag_Clear(FLAG_SCENE_TEMP_50);
  Engine_ActorSetAnimation(24, 1);
  Engine_ActorSetAnimation(25, 1);
  Engine_ActorSetAnimation(IVAN, 1);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
  Engine_ActorSetAnimation(GARET, 1);
  Actor_FaceActor(GARET, IVAN, 0);
  Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, IVAN, 0);
  Actor_FaceActor(24, IVAN, 0);
  Actor_FaceActor(25, IVAN, 0);
  Engine_EventWait(10);
  actor = 24;
  // The movement callback reports which suspect remained blocked.
  switch (work->raisedTrigger) {
  case 202:
  case 203:
    Engine_EventSetMessage(MsgVaultImSurrounded);
    Actor_SetAttachedEffect(25, 0x102);
    Engine_ActorRunRepeatedMotion(25, 2);
    VaultInteriors_ShowMessageAndWait(25, 20);
    if (work->raisedTrigger == 202) {
      actor = 25;
      break;
    }
    // Fall through to the first suspect when trigger 203 selects both.
    // fall through
  case 201:
    Engine_EventSetMessage(MsgVaultTheresNowhereRun);
    Actor_SetAttachedEffect(24, 0x102);
    Engine_ActorRunRepeatedMotion(24, 2);
    actor = 24;
    VaultInteriors_ShowMessageAndWait(24, 20);
    break;
  }
  Engine_ActorSetAnimation(IVAN, 1);
  Actor_FaceActor(IVAN, actor, 0);
  Actor_FaceActor(GARET, IVAN, 0);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Engine_EventWait(20);
  Engine_EventSetMessage(MsgVaultNowIvan);
  VaultInteriors_ShowMessageAndWait(1, 20);
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  Engine_EventWait(20);
  VaultInteriors_ShowMessageAndWait(2, 20);
  VaultInteriors_BeginMindRead(2, actor);
  Ui_SetBank15PaletteAndClearRenderMode();
  Engine_EventWait(60);
  Engine_ActorRunRepeatedMotion(24, 2);
  VaultInteriors_ShowMessageAndWait(24, 20);
  Engine_ActorRunRepeatedMotion(25, 2);
  VaultInteriors_ShowMessageAndWait(25, 20);
  Actor_ShowEmote(IVAN, 0x100, 60);
  VaultInteriors_EndMindRead();
  Engine_ActorStartRepeatedMotion(GARET, 2);
  Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_EventWait(20);
  VaultInteriors_ShowMessageAndWait(1, 20);
  Actor_FaceActor(IVAN, GARET, 0);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(IVAN, 3);
  Engine_EventWait(40);
  VaultInteriors_SetPartyImagePriority();
  gEventWork->transitionFrames = 24;
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  // Move the discussion away from the suspects while the screen is closed.
  VaultInteriors_DiscussMindReading();
  Map_CopyCellAttributes(14, 45, 3, 1, 14, 44);
  GameFlag_Set(0x853);
  {
    struct FieldActor *suspect = Object_GetById(24);
    s32 routeIndex = 5;

    suspect->timer = routeIndex;
  }
  {
    struct FieldActor *suspect = Object_GetById(25);
    s32 routeIndex = 4;

    suspect->timer = routeIndex;
  }
  FieldScheduler_AddCallback(VaultInteriors_FaceSuspectsTowardLeader, 0xc80);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
  Engine_EventEnd();
}

// Show the party's branching conversation about what Ivan learned.
static void VaultInteriors_DiscussMindReading(void) {
  s32 persistedWithQuestions;

  Audio_PlayCueFromEventWork();
  Engine_ActorSetSpritePriority(0, 1);
  {
    struct FieldActor *leader = Object_GetById(0);

    leader->priorityFlags = ACTOR_PRIORITY_AUTOMATIC | leader->priorityFlags;
  }
  FieldMap_SetViewBounds(PIXELS(32), PIXELS(576), PIXELS(400), PIXELS(936));
  Actor_SetSpeed(0, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(1, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(2, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetPosition(0, PIXELS(248), PIXELS(728));
  Actor_SetPosition(2, PIXELS(264), PIXELS(744));
  Engine_ActorSetPosition(1, PIXELS(232), PIXELS(744));
  VaultInteriors_SetPartyImagePriority();
  Engine_ActorFaceEachOther(0, 2, 0);
  VaultInteriors_FaceActorAndWait(1, 2, 30);
  Actor_SetPosition(24, PIXELS(104), PIXELS(696));
  Actor_SetPosition(25, PIXELS(120), PIXELS(696));
  Engine_ActorFaceEachOther(24, 25, 0);
  Engine_CameraFollowActor(0, 0);
  Engine_CameraWaitForMove();
  Engine_MapRedraw();
  Engine_EventWait(30);
  {
    struct EventWork *work = gEventWork;

    work->transitionFrames = 24;
    work->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
  }
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(40);
  Engine_ActorRunRepeatedMotion(1, 1);
  Engine_EventWait(10);
  Engine_EventSetMessage(MsgVaultLearn);
  VaultInteriors_ShowMessageAndWait(1, 20);
  VaultInteriors_PlayAnimationAndWait(2, 3, 20);
  VaultInteriors_ShowMessageAndWait(2, 20);
  VaultInteriors_FaceEachOtherAndWait(0, 1, 50);
  Engine_ActorFaceActor(0, 2, 0);
  VaultInteriors_FaceActorAndWait(1, 2, 20);
  VaultInteriors_PlayAnimationAndWait(2, 4, 20);
  Engine_EventShowMessage(2, 0);
  Actor_ShowEmote(0, 0x102, 0);
  Actor_ShowEmote(1, 0x102, 0);
  Engine_EventWait(60);
  VaultInteriors_PlayAnimationAndWait(2, 3, 20);
  VaultInteriors_ShowMessageAndWait(2, 30);
  Engine_ActorRunRepeatedMotion(1, 1);
  Engine_EventWait(20);
  VaultInteriors_FaceEachOtherAndWait(0, 1, 20);
  Engine_EventOpenMessage(1, 0);
  persistedWithQuestions = 0;
  if (Engine_EventChooseYesNo(0, 0) != 0) {
    Engine_EventWait(20);
    Engine_ActorStartRepeatedMotion(2, 2);
    VaultInteriors_PlayAnimationAndWait(1, 4, 30);
    Engine_EventOpenMessage(1, 0);
    persistedWithQuestions = 0;
    if (Engine_EventChooseYesNo(0, 0) != 0) {
      Engine_EventWait(20);
      Engine_ActorShowEmote(2, 0x102, 0);
      Engine_EventWait(60);
      Engine_ActorRunRepeatedMotion(2, 2);
      VaultInteriors_FaceEachOtherAndWait(0, 2, 20);
      Engine_EventOpenMessage(2, 0);
      persistedWithQuestions = 0;
      if (Engine_EventChooseYesNo(2, 0) != 0) {
        Engine_EventWait(20);
        Engine_ActorShowEmote(2, 0x105, 0);
        Engine_EventWait(60);
        VaultInteriors_ShowMessageAndWait(2, 20);
        VaultInteriors_FaceActorAndWait(1, 2, 10);
        Engine_ActorRunRepeatedMotion(1, 1);
        Engine_EventWait(10);
        VaultInteriors_ShowMessageAndWait(1, 10);
        VaultInteriors_FaceActorAndWait(2, 1, 20);
        Engine_ActorShowEmote(2, 0x101, 0);
        Engine_EventWait(60);
        VaultInteriors_PlayAnimationAndWait(2, 4, 20);
        VaultInteriors_ShowMessageAndWait(1, 10);
        VaultInteriors_FaceEachOtherAndWait(1, 0, 20);
        VaultInteriors_PlayAnimationAndWait(1, 3, 20);
        persistedWithQuestions = 1;
        VaultInteriors_ShowMessageAndWait(1, 20);
      }
    }
  }
  if (persistedWithQuestions == 0) {
    Engine_EventSetMessage(MsgVaultLeave);
    VaultInteriors_ShowMessageAndWait(1, 20);
    VaultInteriors_FaceEachOtherAndWait(1, 0, 20);
    VaultInteriors_ShowMessageAndWait(1, 20);
  }
  Engine_ActorShowEmote(0, 0x105, 0);
  Engine_EventWait(60);
  Engine_ActorRunRepeatedMotion(1, 1);
  VaultInteriors_ShowMessageAndWait(1, 10);
  VaultInteriors_PlayAnimationAndWait(1, 3, 10);
  Engine_ActorFaceEachOther(1, 2, 0);
  VaultInteriors_FaceEachOtherAndWait(0, 2, 10);
  Engine_EventShowMessage(1, 0);
  VaultInteriors_PlayAnimationAndWait(2, 3, 10);
  Actor_WalkTo(2, 248, 728);
  Actor_WalkToAndWait(1, 248, 728);
  Engine_ActorSetPosition(1, 0, 0);
  Engine_ActorSetPosition(2, 0, 0);
}

// Advance suspect routes when the leader touches each route trigger.
static void VaultInteriors_PoseDialogueActors(void) {
  switch (gEventWork->touchedTrigger) {
  case 11:
    VaultInteriors_AdvanceSuspectRoute(24, 1, 2, gVaultInteriors_PairScriptC);
    VaultInteriors_AdvanceSuspectRoute(25, 3, 4, gVaultInteriors_PairScriptR);
    break;
  case 12:
    VaultInteriors_AdvanceSuspectRoute(24, 1, 4, gVaultInteriors_PairScriptG);
    VaultInteriors_AdvanceSuspectRoute(24, 2, 3, gVaultInteriors_PairScriptD);
    VaultInteriors_AdvanceSuspectRoute(25, 1, 3, gVaultInteriors_PairScriptO);
    break;
  case 13:
    VaultInteriors_AdvanceSuspectRoute(24, 2, 1, gVaultInteriors_PairScriptA);
    VaultInteriors_AdvanceSuspectRoute(24, 3, 6, gVaultInteriors_PairScriptJ);
    VaultInteriors_AdvanceSuspectRoute(25, 2, 4, gVaultInteriors_PairScriptQ);
    break;
  case 14:
    VaultInteriors_AdvanceSuspectRoute(24, 3, 2, gVaultInteriors_PairScriptC);
    VaultInteriors_AdvanceSuspectRoute(25, 4, 3, gVaultInteriors_PairScriptP);
    break;
  case 15:
    VaultInteriors_AdvanceSuspectRoute(24, 4, 5, gVaultInteriors_PairScriptH);
    VaultInteriors_AdvanceSuspectRoute(25, 1, 2, gVaultInteriors_PairScriptM);
    break;
  case 16:
    VaultInteriors_AdvanceSuspectRoute(24, 4, 1, gVaultInteriors_PairScriptB);
    VaultInteriors_AdvanceSuspectRoute(24, 5, 6, gVaultInteriors_PairScriptI);
    VaultInteriors_AdvanceSuspectRoute(25, 3, 1, gVaultInteriors_PairScriptL);
    break;
  case 17:
    VaultInteriors_AdvanceSuspectRoute(24, 5, 4, gVaultInteriors_PairScriptF);
    VaultInteriors_AdvanceSuspectRoute(24, 6, 3, gVaultInteriors_PairScriptE);
    VaultInteriors_AdvanceSuspectRoute(25, 4, 2, gVaultInteriors_PairScriptN);
    break;
  case 18:
    VaultInteriors_AdvanceSuspectRoute(24, 6, 5, gVaultInteriors_PairScriptH);
    VaultInteriors_AdvanceSuspectRoute(25, 2, 1, gVaultInteriors_PairScriptK);
    break;
  }
}

// Install a route script only when the suspect is in its expected state.
static void VaultInteriors_AdvanceSuspectRoute(s32 actor, s32 expected,
                                               s32 next, const s32 *actions) {
  struct FieldActor *suspect = Object_GetById(actor);

  if (suspect->timer == expected) {
    Actor_SetActionScript(actor, actions);
    suspect->timer = next;
  }
}

// Shared source is included here in ROM order.
#include "common/vault_spotflag.h"

// Restore room actors, passages, and the appropriate theft-story sequence.
s32 VaultInteriors_Initialize(void) {
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
  switch (gGameState.entrance) {
  case 5: {
    s32 zero = 0;

    Object_GetById(8)->motionFlags = zero;
    Object_GetById(8)->y.fixed = zero;
    Object_GetById(8)->terrainHeight = zero;
  } break;
  case 10:
    if (Engine_GameFlagIsSet(0x850) != 0) {
      Actor_SetPosition(2, PIXELS(376), PIXELS(376));
      Actor_FaceDirection(2, 0x4000, 0);
    }
    if (Engine_GameFlagIsSet(0x856) != 0) {
      Engine_ActorSetPosition(2, 0, 0);
    }
    if (Engine_GameFlagIsSet(0x855) != 0) {
      Actor_SetPosition(16, PIXELS(536), VAULT_INTERIORS_ENTRY_ACTOR_Z);
      Actor_SetBuiltinAction(16, ACTOR_ACTION_IDLE);
      Actor_FaceDirection(16, 0x5000, 0);
    }
    break;
  case 7:
  case 11:
    if (Engine_GameFlagIsSet(0x855) != 0) {
      Actor_SetPosition(18, PIXELS(568), PIXELS(648));
      Actor_SetBuiltinAction(18, ACTOR_ACTION_IDLE);
      Actor_FaceDirection(18, 0x4000, 0);
      OverlayObject_ConfigureObject22WithResource17(231, PIXELS(568),
                                                    PIXELS(16), PIXELS(672));
      Task_AddCallback(Vault_UpdateBoneProximityFlag, 0xc80);
    } else if (Engine_GameFlagIsSet(0x853) != 0) {
      Engine_ActorSetPosition(18, 0, 0);
    }
    break;
  case 12:
    if (Engine_GameFlagIsSet(0x109) != 0 && Engine_GameFlagIsSet(0x852) != 0 &&
        Engine_GameFlagIsSet(0x853) == 0 && Engine_GameFlagIsSet(0x300) != 0) {
      Map_CopyCellAttributes(14, 45, 3, 1, 14, 44);
      Task_AddCallback(VaultInteriors_FaceSuspectsTowardLeader, 0xc80);
      break;
    }
    if (Engine_GameFlagIsSet(0x856) != 0) {
      Actor_SetPosition(25, PIXELS(120), PIXELS(696));
      Actor_FaceDirection(25, 0x8000, 0);
    }
    if (Engine_GameFlagIsSet(0x852) != 0) {
      Map_CopyCellAttributes(14, 45, 3, 1, 14, 44);
      if (Engine_GameFlagIsSet(0x853) == 0) {
        Map_CopyCellAttributes(14, 50, 3, 1, 14, 44);
        break;
      }
      Object_GetById(24)->timer = 5;
      Object_GetById(25)->timer = 4;
      FieldScheduler_AddCallback(VaultInteriors_FaceSuspectsTowardLeader,
                                 0xc80);
    }
    break;
  case 13:
  case 14:
    Engine_EventWait(2);
    Map_CopyCellsTo(54, 2, 35, 20, 2, 10);
    Map_CopyCellsTo(54, 2, 95, 20, 2, 10);
    Map_CopyCellsTo(54, 2, 35, 80, 2, 10);
    Map_CopyCellsTo(54, 2, 46, 21, 4, 8);
    Map_CopyCellsTo(54, 2, 46, 81, 4, 8);
    Object_GetById(26)->collisionFlags |= 4;
    Engine_ActorSetSpriteFlags(Object_GetById(26), 0);
    if (Engine_GameFlagIsSet(0x859) != 0) {
      Actor_SetPosition(26, PIXELS(676), PIXELS(411));
      Map_CopyCellAttributes(101, 24, 3, 4, 41, 24);
    }
    break;
  case 15:
    Engine_EventWait(2);
    Map_CopyCellsTo(54, 2, 44, 21, 2, 8);
    Map_CopyCellsTo(54, 2, 44, 81, 2, 8);
    Engine_EventBegin();
    if (Engine_GameFlagIsSet(0x855) != 0) {
      Engine_ActorSetAnimation(15, 2);
      Engine_ActorSetAnimation(16, 2);
      Engine_EventWait(1);
      Engine_ActorSetAnimation(17, 2);
      break;
    }
    Engine_ActorSetPosition(8, PIXELS(824), PIXELS(456));
    if (Engine_GameFlagIsSet(0x854) != 0) {
      VaultInteriors_ConfrontThievesAgain();
      Engine_EventEnd();
      break;
    }
    Engine_ActorSetAnimation(8, 7);
    VaultInteriors_OpenWindowTransition();
    Engine_AudioPlayCue(17);
    Engine_EventSetMessage(MsgVaultUhnnUhnn);
    VaultInteriors_ShowMessageAndWait(8, 10);
    Engine_EventEnd();
    break;
  case 16:
    Map_CopyCells(54, 2, 2, 8, 44, 21);
    Map_CopyCells(54, 2, 2, 8, 44, 81);
    Engine_ActorSetPosition(8, PIXELS(824), PIXELS(456));
    VaultInteriors_LoseToThieves();
    Engine_EventRequestExit(16);
    break;
  case 17:
    Map_CopyCells(54, 2, 2, 8, 44, 21);
    Map_CopyCells(54, 2, 2, 8, 44, 81);
    if (Engine_GameFlagIsSet(0x109) == 0) {
      Actor_SetPosition(8, PIXELS(824), PIXELS(456));
      VaultInteriors_DefeatThieves();
      VaultInteriors_ReturnStolenGoods();
      break;
    }
    Engine_ActorSetAnimation(15, 2);
    Engine_ActorSetAnimation(16, 2);
    Engine_EventWait(1);
    Engine_ActorSetAnimation(17, 2);
    break;
  }
  return 0;
}

// Keep both suspects facing the leader while they evade the party.
static void VaultInteriors_FaceSuspectsTowardLeader(void) {
  struct FieldActor *origin = Object_GetById(0);
  struct FieldActor *first = Object_GetById(24);
  struct FieldActor *second = Object_GetById(25);

  first->facing = ArcTan2(origin->z.fixed - first->z.fixed,
                          origin->x.fixed - first->x.fixed);
  second->facing = ArcTan2(origin->z.fixed - second->z.fixed,
                           origin->x.fixed - second->x.fixed);
}

// Show the thieves' reaction after the party loses their battle.
static void VaultInteriors_LoseToThieves(void) {
  struct FieldActor *actor;
  s32 priorityFlags;

  Engine_EventBegin();
  Actor_SetPosition(10, PIXELS(792), PIXELS(416));
  Actor_SetPosition(11, PIXELS(800), PIXELS(400));
  Actor_SetPosition(12, PIXELS(776), PIXELS(416));
  Actor_FaceDirection(10, 0x3000, 0);
  Actor_FaceDirection(11, 0x3000, 0);
  Actor_FaceDirection(12, 0x3000, 0);
  Actor_SetSpeed(11, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(12, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(784), PIXELS(448));
  Actor_SetPosition(GARET, PIXELS(808), PIXELS(432));
  Actor_SetPosition(IVAN, PIXELS(776), PIXELS(432));
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 19);
  Engine_ActorSetAnimation(GARET, 19);
  Engine_ActorSetAnimation(IVAN, 19);
  priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  Object_GetById(0)->priorityFlags = priorityFlags;
  Object_GetById(1)->priorityFlags = priorityFlags;
  Object_GetById(2)->priorityFlags = priorityFlags;
  actor = Object_GetById(0);
  Engine_ActorSetSpriteFlags(actor, 0);
  actor = Object_GetById(2);
  Engine_ActorSetSpriteFlags(actor, 0);
  actor = Object_GetById(1);
  Engine_ActorSetSpriteFlags(actor, 0);
  Actor_FaceDirection(8, 0xb000, 0);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
  Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 0);
  Engine_CameraWaitForMove();
  Engine_MapRedraw();
  VaultInteriors_OpenWindowTransition();
  Engine_EventWait(60);
  VaultInteriors_PlayAnimationAndWait(10, 3, 20);
  Engine_EventSetMessage(MsgVaultTheseKidsNothingWorryAbout);
  VaultInteriors_ShowMessageAndWait(10, 30);
  VaultInteriors_ShowMessageAndWait(8, 30);
  Actor_WalkTo(11, 808, 456);
  Actor_WalkTo(12, 792, 456);
  Engine_ActorWaitForMove(12);
  Actor_FaceDirection(12, 0, 0);
  Engine_ActorWaitForMove(11);
  Actor_FaceDirection(11, 0, 0);
  Engine_EventWait(30);
  VaultInteriors_PlayAnimationAndWait(11, 3, 20);
  VaultInteriors_ShowMessageAndWait(11, 20);
  VaultInteriors_FaceActorAndWait(12, 0, 30);
  VaultInteriors_ShowMessageAndWait(12, 60);
  Engine_EventEnd();
}

// Arrange the attic confrontation and restart the thieves' battle.
static void VaultInteriors_ConfrontThievesAgain(void) {

  Actor_SetPosition(10, PIXELS(792), PIXELS(416));
  Actor_SetPosition(11, PIXELS(800), PIXELS(400));
  Actor_SetPosition(12, PIXELS(776), PIXELS(416));
  Actor_FaceDirection(10, 0x3000, 0);
  Actor_FaceDirection(11, 0x3000, 0);
  Actor_FaceDirection(12, 0x3000, 0);
  Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(792), PIXELS(440));
  Actor_SetPosition(1, PIXELS(808), PIXELS(432));
  Actor_SetPosition(2, PIXELS(792), PIXELS(456));
  Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
  Actor_FaceDirection(1, 0xb000, 0);
  Actor_FaceDirection(2, 0xb000, 0);
  Actor_FaceActor(8, 10, 0);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
  Engine_CameraFollowActor(0, 0);
  Engine_CameraWaitForMove();
  Engine_MapRedraw();
  Engine_TaskWait(1);
  gEventWork->transitionFrames = 32;
  VaultInteriors_OpenWindowTransition();
  Engine_EventWait(60);
  Engine_EventSetMessage(MsgVaultWontGoAway);
  Engine_ActorRunRepeatedMotion(11, 1);
  Engine_EventWait(20);
  VaultInteriors_ShowMessageAndWait(11, 30);
  Engine_ActorRunRepeatedMotion(12, 1);
  Engine_EventWait(20);
  Event_ShowMessage(12, 0);
  VaultInteriors_FaceEachOtherAndWait(10, 11, 30);
  Engine_ActorSetAnimation(10, 3);
  VaultInteriors_PlayAnimationAndWait(11, 3, 30);
  VaultInteriors_FaceEachOtherAndWait(10, 12, 30);
  Engine_ActorSetAnimation(10, 3);
  VaultInteriors_PlayAnimationAndWait(12, 3, 40);
  Actor_FaceDirection(10, 0x3000, 0);
  Actor_FaceDirection(11, 0x3000, 0);
  Actor_FaceDirection(12, 0x3000, 0);
  Engine_EventWait(20);
  VaultInteriors_PlayAnimationAndWait(10, 4, 20);
  Event_ShowMessage(10, 0);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
  Event_SetNextDestination((s32)&SceneId_VaultInteriors, 17);
  Event_SetDefeatDestination((s32)&SceneId_VaultInteriors, 16);
  Battle_SetStart(3);
  Field_RequestBattleFromEncounterTable(12, 5);
}

// Read Ivan's owned Venus Djinn count for the shared dialogue.
static s32 VaultInteriors_GetIvanVenusDjinnCount(void) {
  return UnitGet(2)->djinnOwnedCounts[0];
}

// Shared source is included here in ROM order.
#include "common/vault_djinn.h"

// Stage the defeated thieves and gather the townspeople in the attic.
static void VaultInteriors_DefeatThieves(void) {
  struct FieldActor *actor;
  struct EventWork *work;
  const s32 *actions;
  struct AnimationObject *thirdThiefSprite;

  actor = Object_GetById(12);
  thirdThiefSprite = actor->sprite;
  Engine_EventBegin();
  Actor_SetPosition(10, PIXELS(792), PIXELS(416));
  Actor_SetPosition(11, PIXELS(800), PIXELS(400));
  Actor_SetPosition(12, PIXELS(776), PIXELS(392));
  actor = Object_GetById(10);
  Engine_ActorSetSpriteFlags(actor, 0);
  actor = Object_GetById(11);
  Engine_ActorSetSpriteFlags(actor, 0);
  actor = Object_GetById(12);
  Engine_ActorSetSpriteFlags(actor, 0);
  Engine_ActorSetAnimation(10, 9);
  Engine_ActorSetAnimation(11, 9);
  Engine_ActorSetAnimation(12, 9);
  Object_GetById(12)->priorityFlags &= 254;
  thirdThiefSprite->image.priority = 3;
  actions = gVaultInteriors_ActionTable;
  Actor_SetActionScript(10, actions);
  Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(792), PIXELS(440));
  Actor_SetPosition(GARET, PIXELS(808), PIXELS(432));
  Actor_SetPosition(IVAN, PIXELS(776), PIXELS(440));
  Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
  Actor_FaceDirection(GARET, 0xb000, 0);
  Actor_FaceDirection(IVAN, 0xb000, 0);
  Actor_SetSpeed(8, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_FaceDirection(8, 0xb000, 0);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
  Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 0);
  Engine_CameraWaitForMove();
  Engine_MapRedraw();
  VaultInteriors_OpenWindowTransition();
  Actor_SetActionScript(11, actions);
  Engine_EventWait(30);
  Actor_SetActionScript(12, actions);
  Engine_EventWait(30);
  Engine_EventSetMessage(MsgVaultTheyTheyGotUs);
  VaultInteriors_ShowMessageAndWait(10, 20);
  Actor_ShowEmote(8, 0x102, 0);
  Engine_EventWait(60);
  VaultInteriors_ShowMessageAndWait(8, 20);
  Actor_WalkToAndWait(8, 808, 456);
  Actor_SetSpeed(GARET, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(GARET, 792, 432);
  Actor_FaceDirection(GARET, 0, 0);
  Actor_WalkToAndWait(8, 808, 408);
  Actor_FaceDirection(8, 0x8000, 0);
  Actor_WalkToAndWait(GARET, 808, 432);
  Actor_FaceDirection(GARET, 0xc000, 0);
  Engine_EventWait(20);
  VaultInteriors_PlayAnimationAndWait(8, 3, 20);
  VaultInteriors_ShowMessageAndWait(8, 20);
  Actor_WalkToAndWait(8, 768, 408);
  Engine_EventWait(20);
  Actor_FaceActor(8, ACTOR_PARTY_LEADER, 0);
  Actor_FaceActor(ACTOR_PARTY_LEADER, 8, 0);
  Actor_FaceActor(GARET, 8, 0);
  VaultInteriors_FaceActorAndWait(2, 8, 40);
  VaultInteriors_ShowMessageAndWait(8, 30);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
  Engine_ActorSetAnimation(GARET, 3);
  VaultInteriors_PlayAnimationAndWait(2, 3, 20);
  Actor_WalkToAndWait(8, 744, 408);
  Engine_EventWait(50);
  Engine_ActorRunRepeatedMotion(11, 2);
  VaultInteriors_ShowMessageAndWait(11, 20);
  Actor_SetActionScript(11, actions);
  Engine_ActorRunRepeatedMotion(GARET, 1);
  Engine_EventWait(20);
  Actor_FaceActor(ACTOR_PARTY_LEADER, 11, 0);
  Actor_FaceActor(GARET, 11, 0);
  VaultInteriors_FaceActorAndWait(2, 11, 20);
  VaultInteriors_ShowMessageAndWait(1, 20);
  Engine_ActorRunRepeatedMotion(12, 2);
  VaultInteriors_ShowMessageAndWait(12, 30);
  Actor_SetActionScript(12, actions);
  Actor_ShowEmote(GARET, 0x103, 0);
  Engine_EventWait(60);
  VaultInteriors_ShowMessageAndWait(1, 30);
  VaultInteriors_FaceActorAndWait(2, 0, 30);
  VaultInteriors_FaceActorAndWait(0, 2, 30);
  VaultInteriors_PlayAnimationAndWait(2, 3, 20);
  VaultInteriors_ShowMessageAndWait(2, 20);
  VaultInteriors_FaceActorAndWait(1, 2, 30);
  VaultInteriors_PlayAnimationAndWait(0, 3, 30);
  VaultInteriors_ShowMessageAndWait(2, 20);
  Actor_ShowEmote(GARET, 0x100, 0);
  Engine_EventWait(60);
  VaultInteriors_ShowMessageAndWait(1, 30);
  Actor_ShowEmote(IVAN, 0x101, 0);
  Engine_EventWait(60);
  VaultInteriors_FaceEachOtherAndWait(0, 1, 20);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
  VaultInteriors_PlayAnimationAndWait(1, 3, 20);
  VaultInteriors_BeginMindRead(2, 0);
  Ui_SetBank15PaletteAndClearRenderMode();
  Actor_FaceActor(ACTOR_PARTY_LEADER, IVAN, 0);
  VaultInteriors_FaceActorAndWait(1, 2, 10);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  Engine_ActorRunRepeatedMotion(GARET, 1);
  Engine_EventWait(20);
  Actor_SetAttachedEffect(GARET, 0x102);
  Engine_EventWait(60);
  VaultInteriors_FaceActorAndWait(0, 1, 10);
  Event_OpenMessage(GARET, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_EventWait(20);
    Actor_FaceActor(ACTOR_PARTY_LEADER, IVAN, 0);
    VaultInteriors_FaceActorAndWait(1, 2, 20);
    VaultInteriors_EndMindRead();
    Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Engine_ActorRunRepeatedMotion(GARET, 1);
    Engine_EventWait(20);
    VaultInteriors_ShowMessageAndWait(1, 20);
  } else {
    gEventWork->message++;
    Actor_ShowEmote(GARET, 0x102, 0);
    Engine_EventWait(60);
    VaultInteriors_ShowMessageAndWait(1, 20);
    Actor_FaceActor(ACTOR_PARTY_LEADER, IVAN, 0);
    VaultInteriors_FaceActorAndWait(1, 2, 20);
    VaultInteriors_EndMindRead();
    Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Engine_ActorRunRepeatedMotion(GARET, 1);
    Engine_EventWait(20);
  }
  Actor_ShowEmote(IVAN, 0x102, 0);
  Engine_EventWait(60);
  VaultInteriors_PlayAnimationAndWait(2, 4, 20);
  Engine_EventSetMessage(MsgVaultSeeThatsHappened);
  VaultInteriors_ShowMessageAndWait(2, 20);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
  VaultInteriors_PlayAnimationAndWait(1, 3, 40);
  Actor_WalkToAndWait(8, 808, 408);
  Actor_FaceDirection(8, 0x8000, 0);
  Engine_EventWait(30);
  Actor_FaceActor(ACTOR_PARTY_LEADER, 8, 0);
  Actor_FaceActor(GARET, 8, 0);
  VaultInteriors_FaceActorAndWait(2, 8, 20);
  Engine_ActorRunRepeatedMotion(8, 1);
  Engine_EventWait(20);
  VaultInteriors_ShowMessageAndWait(8, 20);
  Actor_SetSpeed(9, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(13, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(14, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetPosition(9, PIXELS(744), PIXELS(408));
  Actor_WalkToAndWait(9, 768, 408);
  VaultInteriors_FaceActorAndWait(9, 10, 30);
  Actor_SetPosition(13, PIXELS(744), PIXELS(408));
  Actor_WalkToAndWait(13, 768, 408);
  Actor_SetPosition(14, PIXELS(744), PIXELS(408));
  Actor_WalkTo(14, 784, 400);
  Actor_WalkToAndWait(13, 776, 424);
  Engine_ActorWaitForMove(14);
  Actor_FaceActor(13, 10, 0);
  VaultInteriors_FaceActorAndWait(14, 10, 20);
  Actor_FaceActor(ACTOR_PARTY_LEADER, 9, 0);
  Actor_FaceActor(GARET, 9, 0);
  VaultInteriors_FaceActorAndWait(2, 9, 20);
  VaultInteriors_PlayAnimationAndWait(9, 4, 20);
  VaultInteriors_ShowMessageAndWait(9, 20);
  VaultInteriors_ShowMessageAndWait(11, 20);
  VaultInteriors_ShowMessageAndWait(12, 30);
  VaultInteriors_FaceEachOtherAndWait(9, 13, 20);
  Engine_ActorRunRepeatedMotion(13, 1);
  Engine_EventWait(20);
  VaultInteriors_ShowMessageAndWait(13, 20);
  VaultInteriors_PlayAnimationAndWait(9, 3, 30);
  VaultInteriors_FaceEachOtherAndWait(9, 14, 20);
  VaultInteriors_PlayAnimationAndWait(9, 3, 30);
  VaultInteriors_FaceActorAndWait(9, 10, 20);
  VaultInteriors_ShowMessageAndWait(9, 20);
  Engine_ActorSetAnimation(13, 3);
  VaultInteriors_PlayAnimationAndWait(14, 3, 20);
  VaultInteriors_FaceEachOtherAndWait(13, 14, 20);
  Engine_ActorSetAnimation(13, 3);
  VaultInteriors_PlayAnimationAndWait(14, 3, 20);
  Actor_WalkTo(14, 792, 392);
  Actor_WalkToAndWait(13, 784, 400);
  Actor_FaceActor(13, 12, 0);
  Engine_ActorWaitForMove(14);
  VaultInteriors_FaceActorAndWait(14, 11, 20);
  Engine_ActorRunRepeatedMotion(13, 1);
  Engine_EventWait(20);
  VaultInteriors_ShowMessageAndWait(13, 20);
  VaultInteriors_PlayAnimationAndWait(14, 4, 20);
  VaultInteriors_ShowMessageAndWait(14, 30);
  VaultInteriors_FaceEachOtherAndWait(13, 0, 20);
  VaultInteriors_ShowMessageAndWait(13, 20);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
  Engine_ActorSetAnimation(GARET, 3);
  VaultInteriors_PlayAnimationAndWait(2, 3, 50);
  work = gEventWork;
  work->transitionFrames = 30;
  work->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(60);
  Engine_EventEnd();
}

// Return the stolen goods and rod, thank the party, and clear the attic.
static void VaultInteriors_ReturnStolenGoods(void) {
  struct FieldActor *temporaryActor;
  struct FieldActor *actor;
  s32 aftermath;
  s32 rod;
  struct EventWork *event;

  event = gEventWork;
  GameFlag_Set(0x855);
  Engine_EventBegin();
  {
    struct FieldActor *actor = Object_GetById(12);

    actor->priorityFlags = ACTOR_PRIORITY_AUTOMATIC | actor->priorityFlags;
  }
  Actor_MoveToAndWait(15, 872, 425);
  Actor_MoveToAndWait(16, 872, 409);
  Actor_MoveToAndWait(17, 872, 377);
  Actor_SetPosition(11, PIXELS(776), PIXELS(392));
  Actor_SetPosition(10, PIXELS(792), PIXELS(392));
  Actor_SetPosition(12, PIXELS(808), PIXELS(392));
  Engine_ActorSetAnimation(10, 5);
  Engine_ActorSetAnimation(11, 5);
  Engine_ActorSetAnimation(12, 5);
  Actor_FaceActor(11, ACTOR_PARTY_LEADER, 0);
  Actor_FaceActor(10, ACTOR_PARTY_LEADER, 0);
  Actor_FaceActor(12, ACTOR_PARTY_LEADER, 0);
  actor = Object_GetById(10);
  Engine_ActorSetSpriteFlags(actor, 1);
  actor = Object_GetById(11);
  Engine_ActorSetSpriteFlags(actor, 1);
  actor = Object_GetById(12);
  Engine_ActorSetSpriteFlags(actor, 1);
  Actor_SetPosition(13, PIXELS(768), PIXELS(408));
  Actor_SetPosition(14, PIXELS(768), PIXELS(424));
  Actor_MoveToAndWait(9, 784, 424);
  Actor_SetPosition(8, PIXELS(808), PIXELS(408));
  Actor_FaceActor(13, 9, 0);
  Actor_FaceActor(8, 9, 0);
  Actor_FaceActor(14, 10, 0);
  Actor_FaceActor(9, 10, 0);
  Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(792), PIXELS(440));
  Actor_SetPosition(GARET, PIXELS(808), PIXELS(440));
  Actor_SetPosition(IVAN, PIXELS(776), PIXELS(440));
  Actor_FaceActor(ACTOR_PARTY_LEADER, 10, 0);
  Actor_FaceActor(GARET, 10, 0);
  Actor_FaceActor(IVAN, 10, 0);
  gEventWork->transitionFrames = 30;
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(40);
  Engine_ActorRunRepeatedMotion(10, 2);
  aftermath = MsgVaultYouIsaacRightWontForget;
  Engine_EventSetMessage(aftermath);
  VaultInteriors_ShowMessageAndWait(10, 20);
  Engine_ActorRunRepeatedMotion(9, 1);
  Engine_EventWait(20);
  VaultInteriors_PlayAnimationAndWait(9, 4, 20);
  VaultInteriors_ShowMessageAndWait(9, 20);
  Engine_ActorSetAnimation(13, 3);
  VaultInteriors_PlayAnimationAndWait(8, 3, 20);
  Engine_ActorStartRepeatedMotion(11, 2);
  Engine_ActorStartRepeatedMotion(12, 2);
  Engine_EventWait(60);
  Actor_SetSpeed(13, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkTo(13, 746, 408);
  Actor_FaceDirection(9, 0xb000, 0);
  Actor_FaceDirection(14, 0xb000, 0);
  Actor_SetActionScript(11, gVaultInteriors_VaultScriptB);
  Engine_EventWait(20);
  Actor_SetActionScript(10, gVaultInteriors_VaultScriptB);
  Engine_EventWait(15);
  Actor_SetActionScript(12, gVaultInteriors_VaultScriptB);
  Engine_EventWait(35);
  Actor_SetActionScript(8, gVaultInteriors_VaultScriptA);
  Engine_EventWait(20);
  Engine_ActorWaitForMove(13);
  Actor_SetPosition(13, 0, 0);
  Engine_EventWait(40);
  Actor_SetSpeed(9, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(14, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(9, 784, 408);
  Actor_FaceActor(9, ACTOR_PARTY_LEADER, 0);
  Actor_WalkToAndWait(14, 768, 408);
  VaultInteriors_FaceActorAndWait(14, 0, 20);
  VaultInteriors_PlayAnimationAndWait(9, 3, 20);
  VaultInteriors_ShowMessageAndWait(9, 20);
  VaultInteriors_PlayAnimationAndWait(14, 3, 20);
  Actor_FaceDirection(14, 0x2000, 10);
  VaultInteriors_ShowMessageAndWait(14, 20);
  VaultInteriors_FaceEachOtherAndWait(0, 1, 50);
  VaultInteriors_FaceEachOtherAndWait(0, 2, 50);
  Actor_FaceActor(ACTOR_PARTY_LEADER, 9, 0);
  Actor_FaceActor(GARET, 9, 0);
  VaultInteriors_FaceActorAndWait(2, 9, 20);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
  Engine_ActorSetAnimation(GARET, 3);
  VaultInteriors_PlayAnimationAndWait(2, 3, 50);
  Engine_ActorFaceEachOther(9, 14, 0);
  VaultInteriors_PlayAnimationAndWait(9, 3, 20);
  VaultInteriors_ShowMessageAndWait(9, 20);
  Actor_SetActionScript(14, gVaultInteriors_VaultScriptC);
  Engine_EventWait(50);
  Actor_SetActionScript(9, gVaultInteriors_VaultScriptD);
  Actor_SetSpeed(GARET, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(IVAN, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(GARET, 792, 456);
  Actor_FaceDirection(GARET, 0xd000, 0);
  Engine_EventWait(30);
  Actor_WalkToAndWait(IVAN, 792, 408);
  Actor_FaceDirection(IVAN, 0, 0);
  Actor_WalkToAndWait(GARET, 808, 456);
  Actor_FaceDirection(GARET, 0xd000, 0);
  Engine_EventWait(100);
  VaultInteriors_FaceActorAndWait(14, 9, 60);
  VaultInteriors_FaceActorAndWait(9, 14, 40);
  VaultInteriors_PlayAnimationAndWait(9, 3, 40);
  Actor_FaceDirection(9, 0, 0);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(9, 2);
  Event_PlayCue(124);
  Engine_ActorSetAnimation(15, 4);
  Actor_SetPosition(18, PIXELS(872), PIXELS(424));
  Engine_ActorSetSpritePriority(18, 1);
  Actor_SetSpeed(18, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetDestinationOffset(18, 0, -8);
  Engine_ActorWaitForMove(18);
  Engine_ActorRunRepeatedMotion(18, 2);
  Engine_EventWait(60);
  Engine_MessageShowCentered((aftermath + 5), 1);
  Engine_ActorSetAnimation(15, 2);
  Actor_SetPosition(18, 0, 0);
  gEventWork->message++;
  Engine_ActorRunRepeatedMotion(14, 1);
  VaultInteriors_ShowMessageAndWait(14, 20);
  VaultInteriors_FaceEachOtherAndWait(0, 1, 40);
  VaultInteriors_FaceActorAndWait(9, 14, 20);
  VaultInteriors_PlayAnimationAndWait(9, 3, 20);
  VaultInteriors_ShowMessageAndWait(9, 30);
  Actor_FaceActor(ACTOR_PARTY_LEADER, 14, 0);
  VaultInteriors_FaceActorAndWait(1, 14, 40);
  Actor_FaceDirection(14, 0, 0);
  Engine_EventWait(40);
  Engine_ActorRunRepeatedMotion(14, 2);
  Event_PlayCue(124);
  Engine_ActorSetAnimation(16, 4);
  Object_GetById(19)->motionFlags = 0;
  Engine_ActorSetSpritePriority(19, 1);
  Actor_SetPosition(19, PIXELS(872), PIXELS(408));
  Actor_SetSpeed(19, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetDestinationOffset(19, 0, -8);
  Engine_ActorWaitForMove(19);
  Engine_ActorRunRepeatedMotion(19, 2);
  Engine_EventWait(60);
  Engine_MessageShowCentered((aftermath + 8), 1);
  Engine_ActorSetAnimation(16, 2);
  Actor_SetPosition(19, 0, 0);
  gEventWork->message++;
  Actor_ShowEmote(9, 0x102, 0);
  Engine_EventWait(60);
  VaultInteriors_ShowMessageAndWait(9, 20);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_ActorStartRepeatedMotion(GARET, 2);
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  Engine_EventWait(20);
  VaultInteriors_PlayAnimationAndWait(14, 3, 50);
  VaultInteriors_FaceActorAndWait(9, 0, 20);
  VaultInteriors_ShowMessageAndWait(9, 30);
  Actor_FaceDirection(14, 0xd000, 0);
  Engine_EventWait(30);
  Actor_ShowEmote(14, 0x100, 0);
  Engine_EventWait(60);
  Actor_WalkToAndWait(14, 856, 376);
  Engine_EventWait(20);
  VaultInteriors_FaceActorAndWait(14, 9, 20);
  VaultInteriors_ShowMessageAndWait(14, 20);
  Actor_FaceActor(9, 14, 0);
  Actor_ShowEmote(IVAN, 0x100, 0);
  Engine_EventWait(60);
  VaultInteriors_FaceActorAndWait(2, 14, 30);
  VaultInteriors_FaceActorAndWait(9, 2, 20);
  VaultInteriors_PlayAnimationAndWait(9, 3, 20);
  Event_ShowMessage(9, 0);
  Actor_FaceDirection(14, 0x5000, 0);
  Engine_EventWait(30);
  VaultInteriors_FaceActorAndWait(2, 9, 20);
  VaultInteriors_PlayAnimationAndWait(2, 3, 20);
  VaultInteriors_ShowMessageAndWait(9, 20);
  Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, IVAN, 0);
  VaultInteriors_FaceActorAndWait(1, 2, 20);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
  VaultInteriors_PlayAnimationAndWait(1, 3, 40);
  VaultInteriors_PlayAnimationAndWait(2, 3, 30);
  Engine_ActorRunRepeatedMotion(9, 1);
  Engine_EventWait(20);
  VaultInteriors_PlayAnimationAndWait(9, 4, 20);
  VaultInteriors_ShowMessageAndWait(9, 20);
  Actor_FaceActor(ACTOR_PARTY_LEADER, 9, 0);
  Actor_FaceActor(GARET, 9, 0);
  VaultInteriors_FaceActorAndWait(2, 9, 20);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  Engine_ActorStartRepeatedMotion(GARET, 1);
  Actor_ShowEmote(IVAN, 0x102, 0);
  Engine_EventWait(60);
  VaultInteriors_ShowMessageAndWait(2, 40);
  VaultInteriors_PlayAnimationAndWait(9, 3, 20);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 4);
  Engine_ActorSetAnimationAndWait(GARET, 4);
  Engine_ActorRunRepeatedMotion(IVAN, 3);
  Engine_EventWait(30);
  VaultInteriors_PlayAnimationAndWait(9, 4, 20);
  VaultInteriors_ShowMessageAndWait(9, 20);
  VaultInteriors_FaceEachOtherAndWait(0, 1, 20);
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  VaultInteriors_PlayAnimationAndWait(2, 4, 30);
  Actor_SetSpeed(IVAN, PIXELS(3) / 2, PIXELS(3) / 4);
  Actor_WalkToAndWait(IVAN, 800, 408);
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  Event_ShowMessage(IVAN, 0);
  Actor_FaceActor(ACTOR_PARTY_LEADER, 9, 0);
  VaultInteriors_FaceActorAndWait(1, 9, 30);
  VaultInteriors_PlayAnimationAndWait(9, 4, 20);
  VaultInteriors_ShowMessageAndWait(9, 20);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
  Actor_SetAttachedEffect(GARET, 0x102);
  Actor_SetAttachedEffect(IVAN, 0x102);
  Engine_EventWait(60);
  VaultInteriors_ShowMessageAndWait(2, 20);
  Engine_ActorRunRepeatedMotion(9, 1);
  Engine_EventWait(20);
  VaultInteriors_ShowMessageAndWait(9, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
  Actor_ShowEmote(GARET, 0x101, 0);
  Actor_ShowEmote(IVAN, 0x101, 0);
  Engine_EventWait(60);
  VaultInteriors_PlayAnimationAndWait(9, 4, 20);
  VaultInteriors_ShowMessageAndWait(9, 20);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  Engine_ActorStartRepeatedMotion(GARET, 1);
  Engine_ActorRunRepeatedMotion(IVAN, 1);
  Engine_EventWait(20);
  VaultInteriors_PlayAnimationAndWait(9, 3, 20);
  VaultInteriors_ShowMessageAndWait(9, 40);
  Actor_ShowEmote(IVAN, 0x105, 0);
  Engine_EventWait(60);
  Actor_WalkToAndWait(9, 840, 424);
  VaultInteriors_FaceEachOtherAndWait(9, 0, 20);
  VaultInteriors_ShowMessageAndWait(9, 20);
  VaultInteriors_PlayAnimationAndWait(0, 3, 20);
  Actor_FaceDirection(9, 0x5000, 0);
  Engine_EventWait(20);
  VaultInteriors_ShowMessageAndWait(9, 20);
  Engine_ActorRunRepeatedMotion(GARET, 1);
  Engine_EventWait(20);
  VaultInteriors_ShowMessageAndWait(1, 20);
  VaultInteriors_PlayAnimationAndWait(9, 3, 20);
  VaultInteriors_ShowMessageAndWait(9, 30);
  VaultInteriors_FaceEachOtherAndWait(9, 14, 20);
  VaultInteriors_ShowMessageAndWait(9, 20);
  Actor_WalkToAndWait(14, 856, 408);
  Actor_SetActionScript(9, gVaultInteriors_VaultScriptE);
  Actor_SetActionScript(14, gVaultInteriors_VaultScriptE);
  Actor_WalkTo(GARET, 792, 456);
  Actor_SetSpeed(IVAN, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkTo(IVAN, 776, 432);
  Engine_ActorWaitForMove(GARET);
  Actor_FaceDirection(GARET, 0xd000, 0);
  Engine_ActorWaitForMove(IVAN);
  Actor_FaceDirection(IVAN, 0xd000, 0);
  Object_RefreshSelectorById(9);
  Actor_FaceActor(ACTOR_PARTY_LEADER, 9, 0);
  temporaryActor = Object_GetById(14);
  {
    u8 *pauseFlag = &temporaryActor->paused;
    u8 paused = 1;

    *pauseFlag = paused;
  }
  temporaryActor->targetX = ACTOR_NO_TARGET;
  temporaryActor->targetY = ACTOR_NO_TARGET;
  temporaryActor->targetZ = ACTOR_NO_TARGET;
  Actor_ShowEmote(9, 0x100, 0);
  Engine_ActorSetAnimation(14, 1);
  Engine_EventWait(50);
  Actor_FaceActor(9, ACTOR_PARTY_LEADER, 0);
  Actor_WalkToAndWait(GARET, 808, 440);
  Actor_FaceDirection(GARET, 0xb000, 0);
  Engine_EventWait(20);
  VaultInteriors_ShowMessageAndWait(9, 20);
  VaultInteriors_FaceEachOtherAndWait(0, 1, 50);
  Actor_FaceActor(ACTOR_PARTY_LEADER, 9, 0);
  VaultInteriors_FaceActorAndWait(1, 9, 20);
  VaultInteriors_PlayAnimationAndWait(9, 3, 20);
  Actor_WalkTo(9, 744, 408);
  Actor_WalkTo(14, 744, 408);
  Engine_ActorWaitForMove(9);
  Engine_ActorWaitForMove(14);
  Engine_EventWait(30);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 792, 408);
  VaultInteriors_FaceEachOtherAndWait(0, 1, 30);
  Actor_ShowEmote(IVAN, 0x105, 0);
  Engine_EventWait(50);
  Actor_FaceActor(ACTOR_PARTY_LEADER, IVAN, 0);
  VaultInteriors_FaceActorAndWait(1, 2, 20);
  Engine_ActorRunRepeatedMotion(GARET, 1);
  Engine_EventWait(20);
  VaultInteriors_ShowMessageAndWait(1, 40);
  VaultInteriors_PlayAnimationAndWait(2, 3, 30);
  VaultInteriors_FaceEachOtherAndWait(0, 1, 40);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 4);
  VaultInteriors_PlayAnimationAndWait(1, 4, 30);
  Actor_FaceActor(ACTOR_PARTY_LEADER, IVAN, 0);
  VaultInteriors_FaceActorAndWait(1, 2, 20);
  Engine_ActorRunRepeatedMotion(GARET, 1);
  Engine_EventWait(20);
  VaultInteriors_ShowMessageAndWait(1, 40);
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  Engine_EventWait(20);
  VaultInteriors_ShowMessageAndWait(2, 20);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
  VaultInteriors_PlayAnimationAndWait(1, 3, 20);
  VaultInteriors_PlayAnimationAndWait(2, 4, 20);
  VaultInteriors_ShowMessageAndWait(2, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
  Actor_ShowEmote(GARET, 0x101, 0);
  Engine_EventWait(60);
  VaultInteriors_PlayAnimationAndWait(2, 3, 20);
  VaultInteriors_ShowMessageAndWait(2, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 0);
  Actor_ShowEmote(GARET, 0x100, 0);
  Engine_EventWait(60);
  VaultInteriors_PlayAnimationAndWait(2, 3, 20);
  VaultInteriors_ShowMessageAndWait(2, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 0);
  Actor_ShowEmote(GARET, 0x102, 0);
  Engine_EventWait(60);
  VaultInteriors_PlayAnimationAndWait(2, 4, 20);
  VaultInteriors_ShowMessageAndWait(2, 20);
  VaultInteriors_PlayAnimationAndWait(1, 3, 20);
  VaultInteriors_ShowMessageAndWait(1, 30);
  Event_OpenMessage(IVAN, 0);
  if (Engine_EventChooseYesNo(0, 0) != 0) {
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(GARET, 1);
    Engine_EventWait(20);
    VaultInteriors_ShowMessageAndWait(1, 20);
    VaultInteriors_PlayAnimationAndWait(2, 4, 20);
    VaultInteriors_ShowMessageAndWait(2, 20);
  }
  VaultInteriors_PlayAnimationAndWait(2, 3, 30);
  Actor_WalkToAndWait(IVAN, 800, 456);
  Actor_FaceActor(ACTOR_PARTY_LEADER, IVAN, 0);
  Actor_FaceActor(GARET, IVAN, 0);
  Actor_WalkToAndWait(IVAN, 856, 456);
  Actor_FaceActor(ACTOR_PARTY_LEADER, IVAN, 0);
  Actor_FaceActor(GARET, IVAN, 0);
  Actor_WalkToAndWait(IVAN, 856, 376);
  Actor_FaceActor(ACTOR_PARTY_LEADER, IVAN, 0);
  Actor_FaceActor(GARET, IVAN, 0);
  Actor_FaceDirection(IVAN, 0, 0);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  temporaryActor =
      (struct FieldActor *)BattleFx_PlayCueAndStartEmitterOnTarget(2, 17, 65);
  Engine_EventWait(60);
  rod = MsgVaultIvanGotShamansRod;
  Engine_MessageShowCentered(rod, 1);
  Engine_ObjectDispatchRelease(temporaryActor);
  Engine_ActorSetAnimation(17, 2);
  Engine_EventWait(20);
  VaultInteriors_PlayAnimationAndWait(2, 3, 20);
  Actor_WalkToAndWait(IVAN, 856, 456);
  Actor_FaceActor(ACTOR_PARTY_LEADER, IVAN, 0);
  Actor_FaceActor(GARET, IVAN, 0);
  Actor_WalkToAndWait(IVAN, 800, 456);
  Actor_FaceActor(ACTOR_PARTY_LEADER, IVAN, 0);
  Actor_FaceActor(GARET, IVAN, 0);
  Actor_WalkToAndWait(IVAN, 776, 424);
  Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, IVAN, 0);
  VaultInteriors_FaceActorAndWait(1, 2, 30);
  VaultInteriors_PlayAnimationAndWait(2, 3, 20);
  Engine_EventSetMessage((rod + 1));
  VaultInteriors_ShowMessageAndWait(2, 20);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
  {
    u16 *slot = &event->message;
    s32 saved = (s16)*slot;

    if (VaultInteriors_GetIvanVenusDjinnCount() != 0) {
      Engine_EventSetMessage(MsgVaultWaitDontWantTakeYour);
      Event_ShowMessage(IVAN, 0);
      OverlayObject_RunObject2WhenFlagged();
    }
    Party_RemoveOwnerRestored(2);
    *slot = saved;
  }
  VaultInteriors_PlayAnimationAndWait(1, 3, 50);
  Actor_WalkToAndWait(IVAN, 776, 408);
  Actor_FaceActor(ACTOR_PARTY_LEADER, IVAN, 0);
  Actor_FaceActor(GARET, IVAN, 0);
  Actor_WalkToAndWait(IVAN, 744, 408);
  Engine_EventWait(40);
  VaultInteriors_FaceEachOtherAndWait(0, 1, 20);
  Engine_ActorRunRepeatedMotion(GARET, 1);
  Engine_EventWait(20);
  VaultInteriors_ShowMessageAndWait(1, 20);
  VaultInteriors_PlayAnimationAndWait(0, 3, 20);
  Engine_ActorSetAnimation(GARET, 2);
  actor = Object_GetById(0);
  if (actor != NULL) {
    Actor_SetDestination(GARET, actor->x.part.pixel, actor->z.part.pixel);
  }
  Engine_ActorWaitForMove(GARET);
  Actor_SetPosition(GARET, 0, 0);
  Engine_EventWait(30);
  Actor_SetPosition(8, 0, 0);
  Actor_SetPosition(9, 0, 0);
  Actor_SetPosition(13, 0, 0);
  Actor_SetPosition(14, 0, 0);
  Actor_SetPosition(10, 0, 0);
  Actor_SetPosition(11, 0, 0);
  Actor_SetPosition(12, 0, 0);
  Actor_SetPosition(IVAN, 0, 0);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
  Engine_EventEnd();
}

// Open the window transition and wait one frame after it finishes.
static void VaultInteriors_OpenWindowTransition(void) {
  struct EventWork *state;

  state = gEventWork;
  state->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(1);
}

// Show the selected actor message, then pause for the requested frames.
static void VaultInteriors_ShowMessageAndWait(s32 actorId, s32 waitFrames) {
  Engine_EventShowMessage(actorId, 0);
  Engine_EventWait(waitFrames);
}

// Face one actor toward another, then pause for the requested frames.
static void VaultInteriors_FaceActorAndWait(s32 actorId, s32 targetId,
                                            s32 waitFrames) {
  Engine_ActorFaceActor(actorId, targetId, 0);
  Engine_EventWait(waitFrames);
}

// Have two actors face each other, then pause for the requested frames.
static void VaultInteriors_FaceEachOtherAndWait(s32 firstId, s32 secondId,
                                                s32 waitFrames) {
  Engine_ActorFaceEachOther(firstId, secondId, 0);
  Engine_EventWait(waitFrames);
}

// Wait for the animation to change before the following pause.
static void VaultInteriors_PlayAnimationAndWait(s32 actorId, s32 animation,
                                                s32 waitFrames) {
  Engine_ActorSetAnimation(actorId, animation, waitFrames);
  ObjectMotion_WaitForAnimationChange(actorId);
  Engine_EventWait(waitFrames);
}

// Begin Mind Read, choose its target, and show its opening effect.
static void VaultInteriors_BeginMindRead(s32 casterId, s32 targetId) {
  Engine_PsynergyBegin(ABILITY_MIND_READ, 1);
  Engine_PsynergySetTarget(casterId, targetId);
  Engine_PsynergyRaiseHands();
  Engine_PsynergyPlayEffect(1);
  Engine_TaskWait(1);
}

// Finish Mind Read and release its scene effect objects.
static void VaultInteriors_EndMindRead(void) {
  Engine_PsynergyPlayEffect(2);
  Engine_PsynergyLowerHands();
  BattleEffect_CleanupSceneObjects();
}

// Shared source is included here in ROM order.
#include "common/vault_itemobj.h"

// Find the route node containing the actor's position on the eight-pixel grid.
static struct VaultInteriorsStop *
VaultInteriors_FindStopAtPosition(s32 *position) {
  s32 gridX = (position[0] - PIXELS(64)) >> 19;
  s32 gridZ = (position[2] - PIXELS(624)) >> 19;
  struct VaultInteriorsStop *stop = gVaultInteriors_Stops;
  struct VaultInteriorsStop *result = NULL;
  u32 i;

  for (i = 0; i <= 36; i++, stop++) {
    s32 stopX = stop->gridX;

    if (stopX == gridX || stopX + 1 == gridX) {
      s32 stopZ = stop->gridZ;

      if (stopZ == gridZ || stopZ + 1 == gridZ) {
        result = stop;
        break;
      }
    }
  }
  return result;
}

// Choose the outgoing connection nearest the requested facing.
static struct VaultInteriorsStop *
VaultInteriors_SnapToNearestStop(struct VaultInteriorsStop *node,
                                 s16 *requestedFacing) {
  struct VaultInteriorsStopConnection *connection;
  s32 stopIndex;
  s32 nearestDifference;
  s32 chosenFacing;
  s32 angleDifference;
  u32 connectionIndex;
  u16 connectionFacing;

  stopIndex = -1;
  chosenFacing = *requestedFacing;
  connection = node->connections;
  nearestDifference = 0x8000;
  for (connectionIndex = 0; connectionIndex <= 2;
       connectionIndex++, connection++) {
    connectionFacing = connection->facing << 8;
    angleDifference = (s16)(connectionFacing - *requestedFacing);
    if (angleDifference < 0)
      angleDifference = -angleDifference;
    /* FAKEMATCH: the used-stop test reads the stopIndex through a volatile
     * lvalue, so the stopIndex is loaded again for the result. 2026-10-13: the
     * plain test reuses its load (48 instructions for 49, differ 9);
     * node->connections[connectionIndex].stopIndex in the arm reloads it
     * through a second strength-reduced pointer (differ 54). */
    if (*(volatile u8 *)&connection->stopIndex != 0xff &&
        angleDifference < nearestDifference) {
      nearestDifference = angleDifference;
      stopIndex = connection->stopIndex;
      chosenFacing = (s16)connectionFacing;
    }
  }
  if (stopIndex == -1)
    return 0;
  *requestedFacing = chosenFacing;
  return &gVaultInteriors_Stops[stopIndex];
}

// Check whether an actor's destination or current position lies within eight
// pixels.
static s32 VaultInteriors_IsActorWithinEightPixels(s32 x, s32 z, s32 id) {
  struct FieldActor *actor = Engine_ActorLookup(id);
  s32 targetX = actor->targetX;
  s32 targetZ;

  if (targetX == ACTOR_NO_TARGET) {
    targetX = actor->x.fixed;
  }
  targetZ = actor->targetZ;
  if (targetZ == ACTOR_NO_TARGET) {
    targetZ = actor->z.fixed;
  }

  targetX = (targetX - x) >> 16;
  targetZ = (targetZ - z) >> 16;
  if (targetX * targetX + targetZ * targetZ <= 64)
    return 1;
  return 0;
}

// Check whether an actor's destination or current position lies within sixteen
// pixels.
static s32 VaultInteriors_IsActorWithinSixteenPixels(s32 x, s32 z, s32 id) {
  struct FieldActor *actor = Engine_ActorLookup(id);
  s32 ax;
  s32 az;

  ax = actor->targetX;
  if (ax == ACTOR_NO_TARGET)
    ax = actor->x.fixed;
  az = actor->targetZ;
  if (az == ACTOR_NO_TARGET)
    az = actor->z.fixed;
  ax = (ax - x) >> 16;
  az = (az - z) >> 16;
  if (ax * ax + az * az <= 0x100)
    return 1;
  return 0;
}

// Reject route stops occupied by the leader, Ivan, or either suspect.
static s32 VaultInteriors_CheckStopOccupancy(struct VaultInteriorsStop *stop) {
  s32 x;
  s32 z;

  if (stop == NULL) {
    return 1;
  }
  x = (stop->gridX << 19) + PIXELS(72);
  z = (stop->gridZ << 19) + PIXELS(632);
  if (VaultInteriors_IsActorWithinSixteenPixels(x, z, 0) != 0 ||
      VaultInteriors_IsActorWithinEightPixels(x, z, 2) != 0 ||
      VaultInteriors_IsActorWithinEightPixels(x, z, 24) != 0 ||
      VaultInteriors_IsActorWithinEightPixels(x, z, 25) != 0) {
    return -1;
  }
  return 0;
}

// Set the actor's destination to the route stop's cell center.
static void VaultInteriors_MoveActorToStop(struct FieldActor *actor,
                                           struct VaultInteriorsStop *stop) {
  Engine_ObjectSetPosition(actor, (stop->gridX << 19) + PIXELS(72), 0,
                           (stop->gridZ << 19) + PIXELS(632));
}

// Steer Ivan and both suspects through the graph and detect a sustained
// cornering.
static void VaultInteriors_UpdateActorStops(void) {
  struct FieldActor *leader;
  struct FieldActor *actor;
  struct EventWork *work;
  struct VaultInteriorsStop *entry;
  struct VaultInteriorsStop *destination;
  s32 dx;
  s32 dz;
  s32 blocked;
  u16 angle;
  s16 facing;
  s16 facingDifference;

  leader = Engine_ActorLookup(0);
  blocked = 0;
  work = gEventWork;
  actor = Engine_ActorLookup(2);
  entry = VaultInteriors_FindStopAtPosition(&actor->x.fixed);
  if (entry != NULL && actor->targetX == ACTOR_NO_TARGET) {
    dx = actor->x.fixed - leader->x.fixed;
    dz = actor->z.fixed - leader->z.fixed;
    facing = leader->facing;
    angle = ArcTan2(dz, dx);
    dx >>= 16;
    dz >>= 16;
    if (work->blockedSteps > 0 && dx * dx + dz * dz <= 400 &&
        (facingDifference = (u16)facing - angle) > -0x1000 &&
        facingDifference < 0x1000) {
      /* FAKEMATCH: a dead store keeps this empty branch through the first jump
       * optimization, as the native lays out its jumps (bge to the else, then b
       * past it). Measured: an empty branch, an empty statement, the negated
       * test with || and a self-assignment all give the inverted blt (tbs-en 4
       * differ); Japanese matches all of them. */
      facingDifference = 0;
#if TBS_PATCH_1
    } else if (dx * dx + dz * dz > 64) {
#else
    } else {
#endif
      facing = actor->facing;
    }
    destination = VaultInteriors_SnapToNearestStop(entry, &facing);
    if (VaultInteriors_CheckStopOccupancy(destination) == 0) {
      VaultInteriors_MoveActorToStop(actor, destination);
      Object_SetMode(actor, 2);
    } else {
      Object_SetMode(actor, 1);
    }
  }

  // Each suspect tries a random turn, then reverses when that route is
  // occupied.
  actor = Engine_ActorLookup(24);
  entry = VaultInteriors_FindStopAtPosition(&actor->x.fixed);
  if (entry != NULL && actor->targetX == ACTOR_NO_TARGET) {
    u32 turn = (u32)Engine_RandomNext() * 2 >> 16;
    s32 currentFacing;

    turn *= 0x60000000;
    currentFacing = actor->facing;
    facing = ((turn - 0x30000000) >> 16) + currentFacing;
    destination = VaultInteriors_SnapToNearestStop(entry, &facing);
    if (VaultInteriors_CheckStopOccupancy(destination) != 0) {
      facing = actor->facing + 0x8000;
      destination = VaultInteriors_SnapToNearestStop(entry, &facing);
      if (VaultInteriors_CheckStopOccupancy(destination) == 0) {
        Engine_ActorSetAttachedEffect(24, 2);
        goto move_first;
      }
      Object_SetMode(actor, 4);
      blocked = 1;
    } else {
    move_first:
      VaultInteriors_MoveActorToStop(actor, destination);
      Object_SetMode(actor, 2);
    }
  }

  actor = Engine_ActorLookup(25);
  entry = VaultInteriors_FindStopAtPosition(&actor->x.fixed);
  if (entry != NULL && actor->targetX == ACTOR_NO_TARGET) {
    u32 turn = (u32)Engine_RandomNext() * 3 >> 16;
    s32 currentFacing;

    turn *= 0x30000000;
    currentFacing = actor->facing;
    facing = ((turn - 0x30000000) >> 16) + currentFacing;
    destination = VaultInteriors_SnapToNearestStop(entry, &facing);
    if (VaultInteriors_CheckStopOccupancy(destination) != 0) {
      facing = actor->facing + 0x8000;
      destination = VaultInteriors_SnapToNearestStop(entry, &facing);
      if (VaultInteriors_CheckStopOccupancy(destination) == 0) {
        Engine_ActorSetAttachedEffect(25, 2);
        goto move_second;
      }
      Object_SetMode(actor, 4);
      blocked += 2;
    } else {
    move_second:
      VaultInteriors_MoveActorToStop(actor, destination);
      Object_SetMode(actor, 2);
    }
  }
  // Require thirty blocked frames before raising the cornered-suspect event.
  if (blocked != 0) {
    if (++gVaultInteriors_StopTimer > 29)
      work->raisedTrigger = blocked + 200;
  } else {
    gVaultInteriors_StopTimer = blocked;
  }
}

// Arrange the chase, constrain its camera, and start the route callback.
static void VaultInteriors_StartActorStops(void) {
  Engine_EventBegin();
  Camera_MoveTo(PIXELS(168), -1, PIXELS(656), 1);
  Actor_SetSpeed(0, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(1, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(2, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(0, 248, 696);
  Actor_SetPosition(1, PIXELS(248), PIXELS(696));
  Actor_SetPosition(2, PIXELS(248), PIXELS(696));
  Actor_WalkTo(0, 200, 696);
  Actor_WalkTo(1, 248, 712);
  Actor_WalkToAndWait(2, 232, 696);
  Engine_ActorWaitForMove(1);
  Actor_FaceDirection(1, 0xc000, 0);
  Actor_FaceDirection(2, 0x8000, 0);
  Engine_ActorWaitForMove(0);
  Engine_ActorSetAnimation(1, 12);
  VaultInteriors_ResetSuspectRoutes();
  FieldMap_SetViewBounds(PIXELS(48), PIXELS(576), PIXELS(288), PIXELS(736));
  Actor_SetSpeed(1, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(2, PIXELS(3) / 4, PIXELS(3) / 8);
  Actor_SetSpeed(24, PIXELS(1), PIXELS(6) / 5);
  Actor_SetSpeed(25, PIXELS(3) / 2, PIXELS(3) / 2);
  gVaultInteriors_StopTimer = 0;
  FieldScheduler_AddOrUpdateCallback(VaultInteriors_UpdateActorStops, 0xc94);
  GameFlag_Clear(0x1ff);
  Engine_EventEnd();
  Engine_AudioPlayCue(9);
}

// clang-format off
// A missing edge keeps both sentinel bytes; valid facings use the angle's high byte.
#define NO_CONNECTION {255, 0xff, {0, 0}}
#define CONNECTION(stop, direction) {stop, (direction) >> 8, {0, 0}}
struct VaultInteriorsStop gVaultInteriors_Stops[] = {
    {4, 0, {0},
     {CONNECTION(1, FACING_EAST), CONNECTION(30, FACING_WEST), NO_CONNECTION}},
    {6, 0, {0},
     {CONNECTION(2, FACING_EAST), CONNECTION(0, FACING_WEST), NO_CONNECTION}},
    {8, 0, {0},
     {CONNECTION(3, FACING_EAST), CONNECTION(1, FACING_WEST), CONNECTION(31, FACING_SOUTH)}},
    {10, 0, {0},
     {CONNECTION(4, FACING_EAST), CONNECTION(2, FACING_WEST), NO_CONNECTION}},
    {12, 0, {0},
     {CONNECTION(5, FACING_EAST), CONNECTION(3, FACING_WEST), NO_CONNECTION}},
    {14, 0, {0},
     {CONNECTION(6, FACING_EAST), CONNECTION(4, FACING_WEST), NO_CONNECTION}},
    {16, 0, {0},
     {CONNECTION(7, FACING_EAST), CONNECTION(5, FACING_WEST), CONNECTION(34, FACING_SOUTH)}},
    {18, 0, {0},
     {CONNECTION(8, FACING_EAST), CONNECTION(6, FACING_WEST), NO_CONNECTION}},
    {20, 0, {0},
     {CONNECTION(9, FACING_EAST), CONNECTION(7, FACING_WEST), NO_CONNECTION}},
    {22, 0, {0},
     {CONNECTION(10, FACING_EAST), CONNECTION(8, FACING_WEST), NO_CONNECTION}},
    {24, 0, {0},
     {CONNECTION(11, FACING_SOUTH), CONNECTION(9, FACING_WEST), NO_CONNECTION}},
    {24, 2, {0},
     {CONNECTION(12, FACING_SOUTH), CONNECTION(10, FACING_NORTH), NO_CONNECTION}},
    {24, 4, {0},
     {CONNECTION(13, FACING_SOUTH), CONNECTION(11, FACING_NORTH), NO_CONNECTION}},
    {24, 6, {0},
     {CONNECTION(14, FACING_SOUTH), CONNECTION(12, FACING_NORTH), NO_CONNECTION}},
    {24, 8, {0},
     {CONNECTION(15, FACING_WEST), CONNECTION(13, FACING_NORTH), NO_CONNECTION}},
    {22, 8, {0},
     {CONNECTION(16, FACING_WEST), CONNECTION(14, FACING_EAST), NO_CONNECTION}},
    {20, 8, {0},
     {CONNECTION(17, FACING_WEST), CONNECTION(15, FACING_EAST), NO_CONNECTION}},
    {18, 8, {0},
     {CONNECTION(18, FACING_WEST), CONNECTION(16, FACING_EAST), NO_CONNECTION}},
    {16, 8, {0},
     {CONNECTION(19, FACING_WEST), CONNECTION(17, FACING_EAST), CONNECTION(36, FACING_NORTH)}},
    {14, 8, {0},
     {CONNECTION(20, FACING_WEST), CONNECTION(18, FACING_EAST), NO_CONNECTION}},
    {12, 8, {0},
     {CONNECTION(21, FACING_WEST), CONNECTION(19, FACING_EAST), NO_CONNECTION}},
    {10, 8, {0},
     {CONNECTION(22, FACING_WEST), CONNECTION(20, FACING_EAST), NO_CONNECTION}},
    {8, 8, {0},
     {CONNECTION(23, FACING_WEST), CONNECTION(21, FACING_EAST), CONNECTION(33, FACING_NORTH)}},
    {6, 8, {0},
     {CONNECTION(24, FACING_WEST), CONNECTION(22, FACING_EAST), NO_CONNECTION}},
    {4, 8, {0},
     {CONNECTION(25, FACING_WEST), CONNECTION(23, FACING_EAST), NO_CONNECTION}},
    {2, 8, {0},
     {CONNECTION(26, FACING_WEST), CONNECTION(24, FACING_EAST), NO_CONNECTION}},
    {0, 8, {0},
     {CONNECTION(27, FACING_NORTH), CONNECTION(25, FACING_EAST), NO_CONNECTION}},
    {0, 6, {0},
     {CONNECTION(28, FACING_NORTH), CONNECTION(26, FACING_SOUTH), NO_CONNECTION}},
    {0, 4, {0},
     {CONNECTION(29, FACING_NORTH), CONNECTION(27, FACING_SOUTH), NO_CONNECTION}},
    {0, 2, {0},
     {CONNECTION(30, FACING_EAST), CONNECTION(28, FACING_SOUTH), NO_CONNECTION}},
    {2, 1, {0},
     {CONNECTION(0, FACING_EAST), CONNECTION(29, FACING_WEST), NO_CONNECTION}},
    {8, 2, {0},
     {CONNECTION(32, FACING_SOUTH), CONNECTION(2, FACING_NORTH), NO_CONNECTION}},
    {8, 4, {0},
     {CONNECTION(33, FACING_SOUTH), CONNECTION(31, FACING_NORTH), NO_CONNECTION}},
    {8, 6, {0},
     {CONNECTION(22, FACING_SOUTH), CONNECTION(32, FACING_NORTH), NO_CONNECTION}},
    {16, 2, {0},
     {CONNECTION(35, FACING_SOUTH), CONNECTION(6, FACING_NORTH), NO_CONNECTION}},
    {16, 4, {0},
     {CONNECTION(36, FACING_SOUTH), CONNECTION(34, FACING_NORTH), NO_CONNECTION}},
    {16, 6, {0},
     {CONNECTION(18, FACING_SOUTH), CONNECTION(35, FACING_NORTH), NO_CONNECTION}},
};

#undef CONNECTION
#undef NO_CONNECTION

s32 gVaultInteriors_ActionTable[] = {
    SC_ADD(SW_SCALE_Y, (-Q16_ONE / 16)),
    SC_ADD(SW_SCALE_X, (-Q16_ONE / 256)),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_Y, (-Q16_ONE / 16)),
    SC_ADD(SW_SCALE_X, (Q16_ONE / 256)),
    SC_WAIT(2),
    SC_SET(SW_SCALE_Y, Q16_ONE),
    SC_SET(SW_SCALE_X, Q16_ONE),
    SC_WAIT(4),
    SC_ADD(SW_SCALE_Y, (-Q16_ONE / 16)),
    SC_ADD(SW_SCALE_X, (-Q16_ONE / 256)),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_Y, (-Q16_ONE / 16)),
    SC_ADD(SW_SCALE_X, (Q16_ONE / 256)),
    SC_WAIT(2),
    SC_SET(SW_SCALE_Y, Q16_ONE),
    SC_SET(SW_SCALE_X, Q16_ONE),
    SC_WAIT(180),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 gVaultInteriors_Villager11ExitScript[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(776), 0, PIXELS(408)),
    SC_SYNC,
    SC_MOVE(PIXELS(776), 0, PIXELS(400)),
    SC_SYNC,
    SC_MOVE(PIXELS(800), 0, PIXELS(400)),
    SC_SYNC,
    SC_ARG(1),
    SC_SET(SW_FACING, 0x3000),
    SC_WAIT(20),
    SC_END,
};

s32 gVaultInteriors_Villager12ExitScript[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(776), 0, PIXELS(408)),
    SC_SYNC,
    SC_MOVE(PIXELS(776), 0, PIXELS(416)),
    SC_SYNC,
    SC_ARG(1),
    SC_SET(SW_FACING, 0x3000),
    SC_WAIT(20),
    SC_END,
};

s32 gVaultInteriors_VaultScriptA[] = {
    SC_SET(SW_SPEED, PIXELS(4) / 5),
    SC_ARG(2),
    SC_MOVE(PIXELS(776), 0, PIXELS(408)),
    SC_SYNC,
    SC_MOVE(PIXELS(744), 0, PIXELS(408)),
    SC_SYNC,
    SC_ARG(1),
    SC_POS(0, 0, 0),
    SC_END,
};

s32 gVaultInteriors_VaultScriptB[] = {
    SC_SET(SW_SPEED, PIXELS(4) / 5),
    SC_ARG(6),
    SC_MOVE(PIXELS(776), 0, PIXELS(408)),
    SC_SYNC,
    SC_MOVE(PIXELS(744), 0, PIXELS(408)),
    SC_SYNC,
    SC_ARG(1),
    SC_POS(0, 0, 0),
    SC_END,
};

s32 gVaultInteriors_VaultScriptC[] = {
    SC_SET(SW_SPEED, PIXELS(4) / 5),
    SC_ARG(2),
    SC_MOVE(PIXELS(792), 0, PIXELS(408)),
    SC_SYNC,
    SC_MOVE(PIXELS(808), 0, PIXELS(440)),
    SC_SYNC,
    SC_MOVE(PIXELS(808), 0, PIXELS(456)),
    SC_SYNC,
    SC_MOVE(PIXELS(840), 0, PIXELS(456)),
    SC_SYNC,
    SC_MOVE(PIXELS(856), 0, PIXELS(408)),
    SC_SYNC,
    SC_ARG(1),
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(10),
    SC_END,
};

s32 gVaultInteriors_VaultScriptD[] = {
    SC_SET(SW_SPEED, PIXELS(4) / 5),
    SC_ARG(2),
    SC_MOVE(PIXELS(808), 0, PIXELS(408)),
    SC_SYNC,
    SC_MOVE(PIXELS(808), 0, PIXELS(456)),
    SC_SYNC,
    SC_MOVE(PIXELS(840), 0, PIXELS(456)),
    SC_SYNC,
    SC_MOVE(PIXELS(856), 0, PIXELS(424)),
    SC_SYNC,
    SC_ARG(1),
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(10),
    SC_END,
};

s32 gVaultInteriors_VaultScriptE[] = {
    SC_SET(SW_SPEED, PIXELS(4) / 5),
    SC_ARG(2),
    SC_MOVE(PIXELS(840), 0, PIXELS(456)),
    SC_SYNC,
    SC_MOVE(PIXELS(808), 0, PIXELS(456)),
    SC_SYNC,
    SC_MOVE(PIXELS(808), 0, PIXELS(408)),
    SC_SYNC,
    SC_MOVE(PIXELS(776), 0, PIXELS(408)),
    SC_SYNC,
    SC_ARG(1),
    SC_END,
};

s32 gVaultInteriors_PairScriptA[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(104), 0, PIXELS(632)),
    SC_SYNC,
    SC_ARG(1),
    SC_END,
};

s32 gVaultInteriors_PairScriptB[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(72), 0, PIXELS(696)),
    SC_SYNC,
    SC_MOVE(PIXELS(80), 0, PIXELS(640)),
    SC_SYNC,
    SC_MOVE(PIXELS(104), 0, PIXELS(632)),
    SC_SYNC,
    SC_ARG(1),
    SC_END,
};

s32 gVaultInteriors_PairScriptC[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(168), 0, PIXELS(632)),
    SC_SYNC,
    SC_ARG(1),
    SC_END,
};

s32 gVaultInteriors_PairScriptD[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(232), 0, PIXELS(632)),
    SC_SYNC,
    SC_ARG(1),
    SC_END,
};

s32 gVaultInteriors_PairScriptE[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(264), 0, PIXELS(696)),
    SC_SYNC,
    SC_MOVE(PIXELS(264), 0, PIXELS(632)),
    SC_SYNC,
    SC_MOVE(PIXELS(232), 0, PIXELS(632)),
    SC_SYNC,
    SC_ARG(1),
    SC_END,
};

s32 gVaultInteriors_PairScriptF[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(104), 0, PIXELS(696)),
    SC_SYNC,
    SC_ARG(1),
    SC_END,
};

s32 gVaultInteriors_PairScriptG[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(80), 0, PIXELS(640)),
    SC_SYNC,
    SC_MOVE(PIXELS(72), 0, PIXELS(696)),
    SC_SYNC,
    SC_MOVE(PIXELS(104), 0, PIXELS(696)),
    SC_SYNC,
    SC_ARG(1),
    SC_END,
};

s32 gVaultInteriors_PairScriptH[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(168), 0, PIXELS(696)),
    SC_SYNC,
    SC_ARG(1),
    SC_END,
};

s32 gVaultInteriors_PairScriptI[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(232), 0, PIXELS(696)),
    SC_SYNC,
    SC_ARG(1),
    SC_END,
};

s32 gVaultInteriors_PairScriptJ[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(264), 0, PIXELS(632)),
    SC_SYNC,
    SC_MOVE(PIXELS(264), 0, PIXELS(696)),
    SC_SYNC,
    SC_MOVE(PIXELS(232), 0, PIXELS(696)),
    SC_SYNC,
    SC_ARG(1),
    SC_END,
};

s32 gVaultInteriors_PairScriptK[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(168), 0, PIXELS(648)),
    SC_SYNC,
    SC_MOVE(PIXELS(72), 0, PIXELS(648)),
    SC_SYNC,
    SC_ARG(1),
    SC_END,
};

s32 gVaultInteriors_PairScriptL[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(72), 0, PIXELS(648)),
    SC_SYNC,
    SC_ARG(1),
    SC_END,
};

s32 gVaultInteriors_PairScriptM[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(168), 0, PIXELS(648)),
    SC_SYNC,
    SC_MOVE(PIXELS(264), 0, PIXELS(648)),
    SC_SYNC,
    SC_ARG(1),
    SC_END,
};

s32 gVaultInteriors_PairScriptN[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(264), 0, PIXELS(648)),
    SC_SYNC,
    SC_ARG(1),
    SC_END,
};

s32 gVaultInteriors_PairScriptO[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(72), 0, PIXELS(696)),
    SC_SYNC,
    SC_ARG(1),
    SC_END,
};

s32 gVaultInteriors_PairScriptP[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(120), 0, PIXELS(696)),
    SC_SYNC,
    SC_MOVE(PIXELS(72), 0, PIXELS(696)),
    SC_SYNC,
    SC_ARG(1),
    SC_END,
};

s32 gVaultInteriors_PairScriptQ[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(264), 0, PIXELS(696)),
    SC_SYNC,
    SC_ARG(1),
    SC_END,
};

s32 gVaultInteriors_PairScriptR[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(168), 0, PIXELS(696)),
    SC_SYNC,
    SC_MOVE(PIXELS(264), 0, PIXELS(696)),
    SC_SYNC,
    SC_ARG(1),
    SC_END,
};

struct SceneEntrance gVaultInteriors_Scripts[] = {
    { 0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { 5, CONDITION_ALWAYS, 192, 0, 220, FACING_NORTH, 0, 32, 0, 272, 240, 0 },
    { 6, CONDITION_ALWAYS, 432, 0, 254, FACING_NORTH, 0, 288, 0, 528, 272, 0 },
    { 7, CONDITION_ALWAYS, 720, 0, 782, FACING_NORTH, 0, 432, 576, 800, 800, 0 },
    { 8, CONDITION_ALWAYS, 624, 0, 236, FACING_NORTH, 0, 560, 32, 848, 256, 0 },
    { 9, CONDITION_ALWAYS, 144, 0, 492, FACING_NORTH, 0, 16, 288, 256, 512, 0 },
    { 10, CONDITION_ALWAYS, 416, 0, 526, FACING_NORTH, 0, 320, 288, 592, 544, 0 },
    { 11, CONDITION_ALWAYS, 744, 0, 640, FACING_SOUTH, 0, 432, 576, 800, 800, 0 },
    { 12, CONDITION_ALWAYS, 358, 0, 632, FACING_SOUTH, 0, 32, 576, 400, 776, 0 },
    { 13, CONDITION_ALWAYS, 648, 0, 424, FACING_SOUTH, 0, 560, 320, 800, 480, 0 },
    { 14, CONDITION_ALWAYS, 704, 0, 408, FACING_WEST, 0, 560, 320, 800, 480, 0 },
    { 15, CONDITION_ALWAYS, 768, 0, 408, FACING_EAST, 0, 712, 288, 952, 496, 0 },
    { 16, CONDITION_ALWAYS, 792, 0, 432, FACING_NORTH, 0, 712, 288, 952, 496, 0 },
    { 17, CONDITION_ALWAYS, 792, 0, 432, FACING_NORTH, 0, 712, 288, 952, 496, 0 },
    { SCENE_TABLE_END },
};

s16 gVaultInteriors_Regions[] = {
    639, 33, 407, 657, 51, 425, -1, 13,
    -1, 0, 0, 0, 0, 0, 0, 0,
};

u32 gVaultInteriors_Messages[] = {
    SCENE_EXITS(21),
    SCENE_EXIT(5, 20, 5),
    SCENE_EXIT(6, 20, 6),
    SCENE_EXIT(7, 20, 7),
    SCENE_EXIT(8, 20, 8),
    SCENE_EXIT(9, 20, 9),
    SCENE_EXIT(10, 20, 10),
    SCENE_EXIT(11, 21, 11),
    SCENE_EXIT(12, 21, 12),
    SCENE_EXIT(13, 20, 13),
    SCENE_EXIT(14, 21, 14),
    SCENE_EXIT(15, 21, 15),
    SCENE_EXIT(16, 9, 12),
    SCENE_EXITS_END,
};

struct ScenePlacement gVaultInteriors_SceneTableA[] = {
    { GARET, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(8), 0, PIXELS(8), FACING_NORTH, TALK_FACE_PARTY, 0 },
    { IVAN, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(376), 0, PIXELS(392), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x6c, 0x0, ACTOR_STAND,
        PIXELS(160), 0, PIXELS(136), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x6a, 0x0, 3,
        PIXELS(152), 0, PIXELS(72), FACING_WEST, TALK_FACE_PARTY, 0 },
    { 0x65, 0x0, ACTOR_STAND,
        PIXELS(456), 0, PIXELS(184), FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x6b, 0x0, ACTOR_WANDER,
        PIXELS(368), 0, PIXELS(120), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x67, 0x0, ACTOR_WANDER,
        PIXELS(744), 0, PIXELS(200), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x6f, 0x0, ACTOR_STAND,
        PIXELS(752), 0, PIXELS(128), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x6b, 0x0, ACTOR_STAND,
        PIXELS(496), 0, PIXELS(416), FACING_EAST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x66, 0x0, ACTOR_STAND,
        PIXELS(512), 0, PIXELS(416), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x75, 0x0, ACTOR_STAND,
        PIXELS(536), 0, PIXELS(451), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x73, 0x0, ACTOR_STAND,
        PIXELS(416), 0, PIXELS(464), FACING_NORTH, TALK_FACE_PARTY, 0 },
    { 0x6a, 0x0, ACTOR_WANDER,
        PIXELS(536), 0, PIXELS(672), FACING_NORTH, TALK_FACE_PARTY, 0 },
    { 0x7c, 0x0, ACTOR_STAND,
        PIXELS(200), 0, PIXELS(392), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x7d, 0x0, ACTOR_STAND,
        PIXELS(88), 0, PIXELS(392), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x76, 0x0, ACTOR_STAND,
        PIXELS(632), 0, PIXELS(136), FACING_SOUTH, 3, 0 },
    { 0x65, 0x0, ACTOR_STAND,
        PIXELS(664), 0, PIXELS(632), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x80, 0x0, ACTOR_STAND,
        PIXELS(696), 0, PIXELS(696), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x3f, 0x854, ACTOR_STAND,
        PIXELS(104), 0, PIXELS(696), FACING_EAST, 3, 0 },
    { 0x40, 0x854, ACTOR_STAND,
        PIXELS(200), 0, PIXELS(680), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xdf, 0x0, ACTOR_STAND,
        PIXELS(692), 0, PIXELS(411), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gVaultInteriors_SceneTableB[] = {
    { GARET, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(8), 0, PIXELS(8), FACING_NORTH, TALK_FACE_PARTY, 0 },
    { IVAN, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(376), 0, PIXELS(392), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x6c, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(160), 0, PIXELS(136), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x75, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(504), 0, PIXELS(456), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x3f, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(104), 0, PIXELS(696), FACING_EAST, 3, 0 },
    { 0x40, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(200), 0, PIXELS(680), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x40, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(8), 0, PIXELS(8), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x74, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(8), 0, PIXELS(8), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x74, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(8), 0, PIXELS(8), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x14, 0x0, ACTOR_STAND,
        PIXELS(872), 0, PIXELS(424), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x14, 0x0, ACTOR_STAND,
        PIXELS(872), 0, PIXELS(408), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x14, 0x0, ACTOR_STAND,
        PIXELS(872), 0, PIXELS(376), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xfb, CONDITION_ALWAYS, 7,
        PIXELS(8), 0, PIXELS(8), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xdd, CONDITION_ALWAYS, 7,
        PIXELS(8), 0, PIXELS(8), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct SceneEvent gVaultInteriorsEvents[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 5 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 6 },
    { EVENT_EXIT, 3, CONDITION_ALWAYS, 7 },
    { EVENT_EXIT, 4, CONDITION_ALWAYS, 8 },
    { EVENT_EXIT, 5, CONDITION_ALWAYS, 9 },
    { EVENT_EXIT, 6, CONDITION_ALWAYS, 10 },
    { EVENT_TOUCH, 7, CONDITION_ALWAYS, EVENT_SCRIPT(VaultInteriors_LeaveThroughExit11) },
    { EVENT_EXIT, 8, CONDITION_ALWAYS, 12 },
    { EVENT_TOUCH, 10, IF_FLAG_SET(0x856), EVENT_SCRIPT(VaultInteriors_ApproachSuspects) },
    { EVENT_TOUCH, 11, IF_FLAG_SET(0x300), EVENT_SCRIPT(VaultInteriors_PoseDialogueActors) },
    { EVENT_TOUCH, 12, IF_FLAG_SET(0x300), EVENT_SCRIPT(VaultInteriors_PoseDialogueActors) },
    { EVENT_TOUCH, 13, IF_FLAG_SET(0x300), EVENT_SCRIPT(VaultInteriors_PoseDialogueActors) },
    { EVENT_TOUCH, 14, IF_FLAG_SET(0x300), EVENT_SCRIPT(VaultInteriors_PoseDialogueActors) },
    { EVENT_TOUCH, 15, IF_FLAG_SET(0x300), EVENT_SCRIPT(VaultInteriors_PoseDialogueActors) },
    { EVENT_TOUCH, 16, IF_FLAG_SET(0x300), EVENT_SCRIPT(VaultInteriors_PoseDialogueActors) },
    { EVENT_TOUCH, 17, IF_FLAG_SET(0x300), EVENT_SCRIPT(VaultInteriors_PoseDialogueActors) },
    { EVENT_TOUCH, 18, IF_FLAG_SET(0x300), EVENT_SCRIPT(VaultInteriors_PoseDialogueActors) },
    { EVENT_TOUCH, 20, IF_FLAG_SET(0x300), EVENT_SCRIPT(VaultInteriors_DiscussSuspiciousGuests) },
    { EVENT_TOUCH, 21, IF_FLAG_SET(0x852), EVENT_SCRIPT(VaultInteriors_BeginCorneringSuspects) },
    { EVENT_RAISED, 201, CONDITION_ALWAYS, EVENT_SCRIPT(VaultInteriors_ReadCorneredSuspectsMinds) },
    { EVENT_RAISED, 202, CONDITION_ALWAYS, EVENT_SCRIPT(VaultInteriors_ReadCorneredSuspectsMinds) },
    { EVENT_RAISED, 203, CONDITION_ALWAYS, EVENT_SCRIPT(VaultInteriors_ReadCorneredSuspectsMinds) },
    { EVENT_RAISED, 250, CONDITION_ALWAYS, EVENT_SCRIPT(VaultInteriors_RefuseToLeaveSearch) },
    { EVENT_TOUCH, 26, CONDITION_ALWAYS, EVENT_SCRIPT(VaultInteriors_EnterThroughExit15) },
    { 0x202, 40, CONDITION_ALWAYS, EVENT_SCRIPT(VaultInteriors_JumpAcrossRoofGap) },
    { EVENT_TALK, 2, IF_FLAG_SET(FLAG_SCENE_TEMP_50), 0 },
    { EVENT_TALK, 2, CONDITION_ALWAYS, EVENT_SCRIPT(VaultInteriors_MeetIvan) },
    { EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgVaultThisRockWasBlownHereAll) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, EVENT_SCRIPT(VaultInteriors_TalkAboutRocks) },
    { EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgVaultMyNeighborCreepsMeOutHe) },
    { EVENT_TALK, 11, CONDITION_ALWAYS, EVENT_SCRIPT(VaultInteriors_AskAboutIvansPowers) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, EVENT_SCRIPT(VaultInteriors_TalkAboutMissingTreasure) },
    { EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgVaultEverSinceTheEruptionGrandpaKeeps) },
    { EVENT_TALK, 14, 0x856, MSGNO(MsgVaultICouldHelpIvanIfI) },
    { EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgVaultYoureHelpingIvanYouDontLook) },
    { EVENT_TALK, 15, 0x856, EVENT_SCRIPT(VaultInteriors_AskToHelpIvan) },
    { EVENT_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgVaultSinceYoureHelpingIvanIllTell) },
    { EVENT_TALK, 16, CONDITION_ALWAYS, EVENT_SCRIPT(VaultInteriors_TalkToIvansHost) },
    { EVENT_TALK, 17, 0x856, MSGNO(MsgVaultMyHusbandTheMayorCantBelieve) },
    { EVENT_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgVaultReallyYoureGoingToHelpIvan) },
    { EVENT_TALK, 18, CONDITION_ALWAYS, EVENT_SCRIPT(VaultInteriors_TalkAboutThreeStrangers) },
    { EVENT_TALK, 19, CONDITION_ALWAYS, EVENT_SCRIPT(VaultInteriors_TalkToWeaponShopkeeper) },
    { EVENT_TALK, 20, CONDITION_ALWAYS, EVENT_SCRIPT(VaultInteriors_TalkToArmorShopkeeper) },
    { EVENT_TALK, 21, CONDITION_ALWAYS, EVENT_SCRIPT(VaultInteriors_TalkToItemShopkeeper) },
    { EVENT_TALK, 22, CONDITION_ALWAYS, MSGNO(MsgVaultTheGuestsUpstairsWerentInTheir) },
    { EVENT_TALK, 23, CONDITION_ALWAYS, EVENT_SCRIPT(VaultInteriors_TalkToInnkeeper) },
    { EVENT_TALK, 24, IF_FLAG_SET(FLAG_SCENE_TEMP_50), 0 },
    { EVENT_TALK, 25, IF_FLAG_SET(FLAG_SCENE_TEMP_50), 0 },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 24, IF_FLAG_SET(FLAG_SCENE_TEMP_50), 0 },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 25, IF_FLAG_SET(FLAG_SCENE_TEMP_50), 0 },
    { EVENT_TALK, 24, 0x853, MSGNO(MsgVaultThatKidIvanHeCreepsMe) },
    { EVENT_TALK, 25, 0x853, MSGNO(MsgVaultWeHaventDoneAnythingYoureA) },
    { EVENT_TALK, 24, CONDITION_ALWAYS, EVENT_SCRIPT(VaultInteriors_MoveFirstSuspectAside) },
    { EVENT_TALK, 25, CONDITION_ALWAYS, EVENT_SCRIPT(VaultInteriors_MoveSecondSuspectAside) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgVaultTheOtherSparklingRocksAllCrumbled) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, EVENT_SCRIPT(VaultInteriors_ReadRockSeekersMind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgVaultHeyMaybeMyNeighborIsThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, EVENT_SCRIPT(VaultInteriors_ReadChildsMind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, EVENT_SCRIPT(VaultInteriors_ReadTreasureSeekersMind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgVaultYouNeverBoughtMeAnythingWorth) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgVaultTheseGuysLookReliableIGuess) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgVaultIWonderWhatSortOfThing) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, EVENT_SCRIPT(VaultInteriors_ReadIvansHostsMind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgVaultMaybeTheyllFindOurStolenFamily) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, EVENT_SCRIPT(VaultInteriors_ReadBoneOwnersMind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, MSGNO(MsgVaultAllOurBestWeaponsWereStolen) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS, MSGNO(MsgVaultIWonderIfTheyLeftUs) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS, MSGNO(MsgVaultHahhWereSureToSeeA) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS, MSGNO(MsgVaultItWouldBeAwfulIfOur) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 23, CONDITION_ALWAYS, MSGNO(MsgVaultThatVolcanoCostUsALot) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x418, CONDITION_ALWAYS, EVENT_SCRIPT(VaultInteriors_MoveFirstSuspectAside) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x419, CONDITION_ALWAYS, EVENT_SCRIPT(VaultInteriors_MoveSecondSuspectAside) },
    { PSYNERGY_ON_ACTOR(0x8c), 26, 0x859, EVENT_SCRIPT(VaultInteriors_OpenMovedCratePassage) },
    { SEARCH(SEARCH_UNNAMED), 41, CONDITION_ALWAYS, EVENT_SCRIPT(VaultInteriors_SearchEmptyBarrel) },
    { SEARCH(SEARCH_JAR), 100, 0xf4b, FIND_COINS(7) },
    { SEARCH(SEARCH_BARREL), 101, 0xf4c, FIND_COINS(4) },
    { SEARCH(SEARCH_BARREL), 102, 0xf4d, FIND_ITEM(ITEM_SLEEP_BOMB) },
    { SEARCH(SEARCH_WOODEN_BOX), 103, 0xf4e, FIND_ITEM(0xc3) },
    { 0xc4f3, 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgVaultTheresABookEntitledHistoryOf)) },
    { SEARCH(SEARCH_BOOKCASE), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgVaultTheresABookEntitledGemsOf)) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gVaultInteriorsEventsEntrances15To17[] = {
    { EVENT_TOUCH, 27, CONDITION_ALWAYS, EVENT_SCRIPT(VaultInteriors_EnterThroughExit14) },
    { EVENT_TALK, 8, CONDITION_ALWAYS, EVENT_SCRIPT(VaultInteriors_FreeCaptiveAndConfrontThieves) },
    { EVENT_TALK, 15, CONDITION_ALWAYS, EVENT_SCRIPT(VaultInteriors_SearchEmptyChest) },
    { EVENT_TALK, 16, CONDITION_ALWAYS, EVENT_SCRIPT(VaultInteriors_SearchEmptyChest) },
    { EVENT_TALK, 17, CONDITION_ALWAYS, EVENT_SCRIPT(VaultInteriors_SearchEmptyChest) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgVaultAhhHelpMe) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gVaultInteriorsEventsFlag855[] = {
    { SEARCH(SEARCH_UNNAMED), 41, CONDITION_ALWAYS, EVENT_SCRIPT(Vault_FindBone) },
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 5 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 6 },
    { EVENT_EXIT, 3, CONDITION_ALWAYS, 7 },
    { EVENT_EXIT, 4, CONDITION_ALWAYS, 8 },
    { EVENT_EXIT, 5, CONDITION_ALWAYS, 9 },
    { EVENT_EXIT, 6, CONDITION_ALWAYS, 10 },
    { EVENT_EXIT, 7, CONDITION_ALWAYS, 11 },
    { EVENT_EXIT, 8, CONDITION_ALWAYS, 12 },
    { EVENT_TOUCH, 26, CONDITION_ALWAYS, EVENT_SCRIPT(VaultInteriors_EnterThroughExit15) },
    { EVENT_TOUCH, 27, CONDITION_ALWAYS, EVENT_SCRIPT(VaultInteriors_EnterThroughExit14) },
    { EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgVaultICanSleepWellNowThat) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, EVENT_SCRIPT(VaultInteriors_TalkAboutRocks) },
    { EVENT_TALK, 10, CONDITION_ALWAYS, EVENT_SCRIPT(VaultInteriors_AskWhereThievesCameFrom) },
    { EVENT_TALK, 11, CONDITION_ALWAYS, EVENT_SCRIPT(VaultInteriors_TalkAboutWorriedFather) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, EVENT_SCRIPT(VaultInteriors_TalkAboutMissingTreasure) },
    { EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgVaultJustAsIThoughtNothingWas) },
    { EVENT_TALK, 14, CONDITION_ALWAYS, EVENT_SCRIPT(VaultInteriors_AskAboutDefeatingThieves) },
    { EVENT_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgVaultIvanLearnedALotByTalking) },
    { EVENT_TALK, 16, CONDITION_ALWAYS, EVENT_SCRIPT(VaultInteriors_ReceiveWaterOfLife) },
    { EVENT_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgVaultYoureThePersonWhoGotOur) },
    { EVENT_TALK, 18, FLAG_SCENE_TEMP_50, EVENT_SCRIPT(VaultInteriors_OfferBone) },
    { EVENT_TALK, 19, CONDITION_ALWAYS, EVENT_SCRIPT(VaultInteriors_TalkToWeaponShopkeeper) },
    { EVENT_TALK, 20, CONDITION_ALWAYS, EVENT_SCRIPT(VaultInteriors_TalkToArmorShopkeeper) },
    { EVENT_TALK, 21, CONDITION_ALWAYS, EVENT_SCRIPT(VaultInteriors_TalkToItemShopkeeper) },
    { EVENT_TALK, 22, CONDITION_ALWAYS, MSGNO(MsgVaultImShockedThoseGuestsOfOurs) },
    { EVENT_TALK, 23, CONDITION_ALWAYS, EVENT_SCRIPT(VaultInteriors_TalkToInnkeeper) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgVaultEverybodysAfterMyRockTheyWant) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, EVENT_SCRIPT(VaultInteriors_ReadRockSeekersMind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgVaultTheReasonLunpaTurnedBadIs) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, EVENT_SCRIPT(VaultInteriors_ReadChildsMind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, EVENT_SCRIPT(VaultInteriors_ReadTreasureSeekersMind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgVaultIWishIHadSomethingWorth) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgVaultWhenIGrowUpIWant) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgVaultIWishIHadWingsLike) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, EVENT_SCRIPT(VaultInteriors_ReadIvansHostsMind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgVaultImSoRelievedNowThatWe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, EVENT_SCRIPT(VaultInteriors_ReadBoneOwnersMind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, MSGNO(MsgVaultMyWeaponsWereFoundInThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS, MSGNO(MsgVaultItMustBeSnowingUpIn) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS, MSGNO(MsgVaultWhyAreThesePeopleSoAnxious) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS, MSGNO(MsgVaultWhoCouldHaveKnownTheyWere) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 23, CONDITION_ALWAYS, MSGNO(MsgVaultOfCourseIAlwaysSuspectedThey) },
    { SEARCH(SEARCH_JAR), 100, 0xf4b, FIND_COINS(7) },
    { SEARCH(SEARCH_BARREL), 101, 0xf4c, FIND_COINS(4) },
    { SEARCH(SEARCH_BARREL), 102, 0xf4d, FIND_ITEM(ITEM_SLEEP_BOMB) },
    { SEARCH(SEARCH_WOODEN_BOX), 103, 0xf4e, FIND_ITEM(0xc3) },
    { 0xc4f3, 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgVaultTheresABookEntitledHistoryOf)) },
    { SEARCH(SEARCH_BOOKCASE), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgVaultTheresABookEntitledGemsOf)) },
    { SCENE_EVENTS_END },
};
// clang-format on

s32 *gVaultInteriors_StepActions[] = {
    gVaultInteriors_PairScriptC, gVaultInteriors_PairScriptD,
    gVaultInteriors_PairScriptJ, gVaultInteriors_PairScriptH,
    gVaultInteriors_PairScriptF, gVaultInteriors_PairScriptB,
};

s32 *gVaultInteriors_IdleActions[] = {
    gVaultInteriors_PairScriptG, gVaultInteriors_PairScriptA,
    gVaultInteriors_PairScriptC, gVaultInteriors_PairScriptE,
    gVaultInteriors_PairScriptI, gVaultInteriors_PairScriptH,
};

s32 *gVaultInteriors_ResponseScripts[] = {
    gVaultInteriors_PairScriptM, gVaultInteriors_PairScriptQ,
    gVaultInteriors_PairScriptP, gVaultInteriors_PairScriptL,
    gVaultInteriors_PairScriptO, gVaultInteriors_PairScriptK,
    gVaultInteriors_PairScriptN, gVaultInteriors_PairScriptR,
};

u16 gVaultInteriors_StopTimer;
