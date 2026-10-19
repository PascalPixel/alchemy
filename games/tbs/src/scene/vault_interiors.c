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
#include "songs.h"

#if TBS_PATCH_3
#define VAULT_INTERIORS_ENTRY_ACTOR_Z PIXELS(451)
#else
#define VAULT_INTERIORS_ENTRY_ACTOR_Z PIXELS(464)
#endif

// The houses, shops and inn of Vault while Hammet's rod is missing. The
// townspeople are named by what they say.
enum {
  ACTOR_GEM_OWNER = 8, // a sparkling rock was blown here from the volcano
  ACTOR_GEM_OWNERS_WIFE = 9,
  ACTOR_CHILD = 11, // "Hey, mister! Is it fun to see strange new places?"
  ACTOR_MAYOR = 16, // Ivan's host; his wife calls him the mayor
  ACTOR_MAYORS_WIFE = 17,
  ACTOR_BONE_GIVER = 18,
  ACTOR_WEAPON_MERCHANT = 19,
  ACTOR_ARMOR_MERCHANT = 20,
  ACTOR_ITEM_MERCHANT = 21,
  ACTOR_INNKEEPER = 23,
  // The two guests at the inn the town suspects: the thieves' leader and one
  // of his men.
  ACTOR_SUSPECTED_LEADER = 24,
  ACTOR_SUSPECTED_THIEF = 25,
  ACTOR_CRATE = 26 // pushed aside to open a passage
};

// Set while the two suspects run from the party, until the party steps aside
// to talk it over.
enum { FLAG_SUSPECTS_FLEEING = FLAG_SCENE_TEMP_00 + 0x100 };

// The inn's loft (entrances 15 to 17), where the thieves hid the stolen goods.
enum {
  ACTOR_ATTIC_ROOFER = 8, // tied up while fixing the roof
  ACTOR_ATTIC_MAYOR = 9,
  ACTOR_THIEF_LEADER = 10,
  ACTOR_FIRST_THIEF = 11,
  ACTOR_SECOND_THIEF = 12,
  ACTOR_FIRST_CHEST = 15,
  ACTOR_SECOND_CHEST = 16,
  ACTOR_THIRD_CHEST = 17
};

// Forward declarations.
static void VaultInteriors_IvanJoinsRodSearch(void);
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
struct SceneEntrance *Scene_GetEntrances(void) {
  return gVaultInteriors_Entrances;
}

// Return the trigger regions for the Vault interiors overlay.
s16 *Scene_GetRegions(void) { return gVaultInteriors_Regions; }

// Return the exits for the Vault interiors overlay.
u32 *Scene_GetExits(void) { return gVaultInteriors_Exits; }

// Select the attic placements for entrances 15–17 and prepare the actors.
const struct ScenePlacement *Scene_GetPlacements(void) {
  struct ScenePlacement *table;

  switch (gGameState.entrance) {
  case 15:
  case 16:
  case 17:
    table = gVaultInteriors_AtticPlacements;
    break;
  default:
    table = gVaultInteriors_Placements;
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

  leader = Object_GetById(ACTOR_PARTY_LEADER);
  flags = &leader->motionFlags;
  savedMotionFlags = *flags;
  if (FieldObject_CheckMovementCollision(leader, pos) == 0) {
    Engine_EventBegin();
    Object_SetMode(leader, 6);
    Engine_TaskWait(6);
    Engine_AudioPlayCue(SE_JUMP);
    Object_SetMode(leader, 7);
    leader->speed = PIXELS(3);
    leader->acceleration = PIXELS(2);
    leader->velocityY = PIXELS(4);
    *flags &= 0x7e;
    Engine_ActorSetSpriteFlags(leader, 0);
    Engine_ActorMoveToAndWait(ACTOR_PARTY_LEADER,
                              ((union FieldCoordinate *)&pos->x)->part.pixel,
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

// Facing north or south across the gap in the roof, the leader jumps it.
static void Event_JumpAcrossRoofGap(void) {
  s32 x;
  s32 z;
#if !TBS_PATCH_1
  struct FixedPointPosition target;
  struct FixedPointPosition *pos;
  struct FieldActor *leader;
#endif

  if (Object_GetById(ACTOR_PARTY_LEADER)->facing >= 0xa000 &&
      Object_GetById(ACTOR_PARTY_LEADER)->facing <= 0xe000) {
#if TBS_PATCH_1
    Engine_LeaderCheckAhead();
#else
    leader = Object_GetById(ACTOR_PARTY_LEADER);
    pos = &target;
    SET_POSITION(pos, leader->x.fixed,
                 Object_GetById(ACTOR_PARTY_LEADER)->y.fixed,
                 Object_GetById(ACTOR_PARTY_LEADER)->z.fixed - PIXELS(30));
    VaultInteriors_TryJumpTo(pos);
#endif
    x = 42;
    z = 85;
    Map_CopyCellAttributes(41, 85, 1, 1, x, z);
  } else if (Object_GetById(ACTOR_PARTY_LEADER)->facing >= 0x2000 &&
             Object_GetById(ACTOR_PARTY_LEADER)->facing <= 0x6000) {
#if TBS_PATCH_1
    Engine_LeaderCheckAhead();
#else
    leader = Object_GetById(ACTOR_PARTY_LEADER);
    pos = &target;
    SET_POSITION(pos, leader->x.fixed,
                 Object_GetById(ACTOR_PARTY_LEADER)->y.fixed,
                 Object_GetById(ACTOR_PARTY_LEADER)->z.fixed + PIXELS(30));
    VaultInteriors_TryJumpTo(pos);
#endif
    x = 42;
    z = 85;
    Map_CopyCellAttributes(43, 85, 1, 1, x, z);
  }
}

// Once the crate has been moved aside to column 42, the passage behind it
// opens.
static void Event_CheckCrateMoved(void) {
  struct FieldActor *crate;

  Engine_EventBegin();
  crate = Object_GetById(ACTOR_CRATE);
  if ((crate->x.fixed >> 20) == 42) {
    Map_CopyCellAttributes(101, 24, 3, 4, 41, 24);
    Engine_GameFlagSet(FLAG_VAULT_CRATE_MOVED);
  }
  Engine_EventEnd();
}

// Across the counter the weapon shop opens; otherwise the merchant lost
// expensive goods too, or found the stolen weapons rusted in the cemetery.
static void Event_TalkToWeaponMerchant(void) {
  s32 leaderFacing = Object_GetById(ACTOR_PARTY_LEADER)->facing;

  Engine_EventBegin();
  if (leaderFacing >= 0xa001 && leaderFacing <= 0xdfff) {
    Engine_ShopOpen(4, ACTOR_WEAPON_MERCHANT);
  } else {
    if (Engine_GameFlagIsSet(FLAG_VAULT_THIEVES_JAILED) == 0) {
      Engine_EventSetMessage(MsgVaultMasterHammetIsntOnlyOne);
    } else {
      Engine_EventSetMessage(MsgVaultWeFoundOurStolenWeapons);
    }
    Engine_EventShowMessage(ACTOR_WEAPON_MERCHANT, 0);
  }
  Engine_EventEnd();
}

// Across the counter the armor shop opens; otherwise the merchant wants the
// thieves to pay, or advises preparing for the cave in the Goma Range.
static void Event_TalkToArmorMerchant(void) {
  s32 leaderFacing = Object_GetById(ACTOR_PARTY_LEADER)->facing;

  Engine_EventBegin();
  if (leaderFacing >= 0xa001 && leaderFacing <= 0xdfff) {
    Engine_ShopOpen(5, ACTOR_ARMOR_MERCHANT);
  } else {
    if (Engine_GameFlagIsSet(FLAG_VAULT_THIEVES_JAILED) == 0) {
      Engine_EventSetMessage(MsgVaultThievesDidntHitOurHouse);
    } else {
      Engine_EventSetMessage(MsgVaultIfYoureGonnaHeadInto);
    }
    Engine_EventShowMessage(ACTOR_ARMOR_MERCHANT, 0);
  }
  Engine_EventEnd();
}

// Across the counter the inn opens; otherwise the innkeeper defends the
// guests everyone suspects, or learns the goods were hidden in his loft.
static void Event_TalkToInnkeeper(void) {
  s32 leaderFacing = Object_GetById(ACTOR_PARTY_LEADER)->facing;

  Engine_EventBegin();
  if (leaderFacing >= 0xa001 && leaderFacing <= 0xdfff) {
    Engine_InnOpen(1, ACTOR_INNKEEPER);
  } else {
    if (Engine_GameFlagIsSet(FLAG_VAULT_THIEVES_JAILED) == 0) {
      Engine_EventSetMessage(MsgVaultEveryoneThinksOurGuestsThieves);
    } else {
      Engine_EventSetMessage(MsgVaultTheyHidThoseStolenGoods);
    }
    Engine_EventShowMessage(ACTOR_INNKEEPER, 0);
  }
  Engine_EventEnd();
}

// Shared source is included here in ROM order.
#include "common/vault_bone.h"

// "We don't have time for that, Isaac!"
static void Event_GaretRefusesToLeave(void) {
#if TBS_PATCH_1
  Leader_StopMoving();
#endif
  Engine_EventSetMessage(MsgVaultWeDontHaveTimeFor);
  Engine_EventShowMessage(GARET, 0);
}

// Isaac checks the barrel but doesn't find anything.
static void Event_SearchEmptyBarrel(void) {
  Engine_EventBegin();
  Engine_MessageShowCentered(MsgVaultIsaacCheckedBarrel, 1);
  Engine_MessageShowCentered(MsgVaultButDidntFindAnything, 1);
  Engine_EventEnd();
}

// Isaac checks the chest, but it is empty.
static void Event_SearchEmptyChest(void) {
  Engine_EventBegin();
  Engine_MessageShowCentered(MsgVaultIsaacCheckedChest, 1);
  Engine_MessageShowCentered(MsgVaultButChestWasEmpty, 1);
  Engine_EventEnd();
}

// Choose the attic events or the town events before and after the theft.
const struct SceneEvent *Scene_GetEvents(void) {
  switch (gGameState.entrance) {
  case 15:
  case 16:
  case 17:
    return gVaultInteriors_AtticEvents;
  }
  if (Engine_GameFlagIsSet(FLAG_VAULT_THIEVES_JAILED) != 0) {
    return gVaultInteriors_EventsAfterThieves;
  }
  return gVaultInteriors_Events;
}

// Stand the speaker, face the leader, and show the selected message.
static void VaultInteriors_StandAndShowMessage(s32 x) {
  Engine_ActorSetAnimation(x, ANIM_STAND);
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

// The child: "Hey, mister! Is it fun to see strange new places?"
static void Event_ChildAsksAboutTravel(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgVaultMisterFunSeeStrangeNew);
  Engine_ActorSetAnimation(ACTOR_CHILD, ANIM_STAND);
  VaultInteriors_AskYesNoAndShowReply(ACTOR_CHILD);
  Engine_EventEnd();
}

// "Ivan has great powers. Wouldn't you like to know what they are?"
static void Event_OfferIvansSecret(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgVaultIvanHasGreatPowersWouldnt);
  VaultInteriors_AskYesNoAndShowReply(15);
  Actor_FaceDirection(15, FACING_WEST, 0);
  Engine_EventEnd();
}

// Across the counter the item shop opens; otherwise "Did you just arrive in
// town today?", or, after the thieves are jailed, the bridge is out.
static void Event_TalkToItemMerchant(void) {
  s32 leaderFacing = Object_GetById(ACTOR_PARTY_LEADER)->facing;

  Engine_EventBegin();
  if (leaderFacing >= 0xa001 && leaderFacing <= 0xdfff) {
    Engine_ShopOpen(6, ACTOR_ITEM_MERCHANT);
  } else if (Engine_GameFlagIsSet(FLAG_VAULT_THIEVES_JAILED) == 0) {
    Engine_EventSetMessage(MsgVaultDidJustArriveInTown);
    VaultInteriors_AskYesNoAndShowReply(ACTOR_ITEM_MERCHANT);
  } else {
    Engine_EventSetMessage(MsgVaultWithBridgeOutWillQuite);
    Engine_EventShowMessage(ACTOR_ITEM_MERCHANT, 0);
  }
  Engine_EventEnd();
}

// The gem owner's wife wishes their rocks were gems, or, after the thieves
// are jailed, is glad they were caught before stealing the gem.
static void Event_TalkAboutRocks(void) {
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(FLAG_VAULT_THIEVES_JAILED) == 0) {
    Engine_EventSetMessage(MsgVaultIfOnlyTheseRocksWere);
  } else {
    Engine_EventSetMessage(MsgVaultThankGoodnessThoseThievesWere);
  }
  VaultInteriors_StandAndShowMessage(ACTOR_GEM_OWNERS_WIFE);
  Engine_EventEnd();
}

// "Is it just me, or am I missing something?", or, after the thieves are
// jailed, they hid the treasure in the loft and more in the cemetery.
static void Event_TalkAboutMissingThings(void) {
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(FLAG_VAULT_THIEVES_JAILED) != 0) {
    Engine_EventSetMessage(MsgVaultThievesHidStolenTreasureIn);
  } else {
    Engine_EventSetMessage(MsgVaultJustMeOrAmMissing);
  }
  VaultInteriors_StandAndShowMessage(12);
  Engine_EventEnd();
}

// The mayor asks for someone to help Ivan; once the party has agreed, he
// thanks them the first time and asks them to look after Ivan after that.
static void Event_MayorAsksHelpForIvan(void) {
  s32 thankedForHelpingFlag;
  thankedForHelpingFlag = FLAG_VAULT_MAYOR_THANKED_PARTY;
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(FLAG_VAULT_IVAN_JOINED_ROD_SEARCH) != 0) {
    if (Engine_GameFlagIsSet(thankedForHelpingFlag) == 0) {
      Engine_EventSetMessage(MsgVaultYoureGoingHelpIvan);
      VaultInteriors_StandAndShowMessage(ACTOR_MAYOR);
      Engine_EventWait(10);
      VaultInteriors_PlayAnimationAndWait(ACTOR_MAYOR, ANIM_NOD, 20);
      Engine_GameFlagSet(thankedForHelpingFlag);
    } else {
      Engine_EventSetMessage(MsgVaultPleaseLookAfterIvan);
    }
  } else {
    Engine_EventSetMessage(MsgVaultCouldSomeonePleaseHelpIvan);
  }
  VaultInteriors_StandAndShowMessage(ACTOR_MAYOR);
  Engine_EventEnd();
}

// The three strangers have fooled the innkeeper and his wife.
static void Event_WarnAboutThreeStrangers(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgVaultThoseThreeStrangersSureHave);
  VaultInteriors_StandAndShowMessage(ACTOR_BONE_GIVER);
  Engine_EventEnd();
}

// The suspected leader cries "Ow! Stop it!" and runs to his next spot, chosen
// by the way he faces and where he is on his route.
static void Event_SuspectedLeaderDodges(void) {
  struct FieldActor *suspect;
  s16 *routeState;
  s32 routeIndex;
  s32 nextRoute;

  suspect = Object_GetById(ACTOR_SUSPECTED_LEADER);
  Engine_EventBegin();
  Engine_ActorRunRepeatedMotion(ACTOR_SUSPECTED_LEADER, 2);
  Engine_EventSetMessage(MsgVaultOwStop);
  Engine_EventShowMessage(ACTOR_SUSPECTED_LEADER, 0);
  Engine_ActorSetSpeed(ACTOR_SUSPECTED_LEADER, PIXELS(4), PIXELS(2));
  if ((u32)((suspect->facing & 0xf000) - 0x5000) <= 0x6000) {
    routeState = (s16 *)&suspect->timer;
    routeIndex = *routeState;
    if (routeIndex <= 2) {
      Actor_SetActionScript(ACTOR_SUSPECTED_LEADER,
                            gVaultInteriors_StepActions[routeIndex]);
      *(u16 *)routeState = *(u16 *)routeState + 1;
    } else {
      Actor_SetActionScript(ACTOR_SUSPECTED_LEADER,
                            gVaultInteriors_IdleActions[routeIndex]);
      nextRoute = *(u16 *)routeState - 1;
      *(u16 *)routeState = nextRoute;
    }
  } else {
    routeState = (s16 *)&suspect->timer;
    routeIndex = *routeState;
    if (routeIndex > 2) {
      Actor_SetActionScript(ACTOR_SUSPECTED_LEADER,
                            gVaultInteriors_StepActions[routeIndex]);
      *(u16 *)routeState = *(u16 *)routeState + 1;
    } else {
      Actor_SetActionScript(ACTOR_SUSPECTED_LEADER,
                            gVaultInteriors_IdleActions[routeIndex]);
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
  Object_RefreshSelectorById(ACTOR_SUSPECTED_LEADER);
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

// The suspected thief cries "No! Leave me alone!" and runs to his next spot.
static void Event_SuspectedThiefDodges(void) {
  struct FieldActor *actor;
  u32 facing;
  s32 routePair;

  actor = Object_GetById(ACTOR_SUSPECTED_THIEF);
  facing = actor->facing & 0xf000;
  routePair = actor->timer >> 1;
  Engine_EventBegin();
  Engine_ActorRunRepeatedMotion(ACTOR_SUSPECTED_THIEF, 2);
  Engine_EventSetMessage(MsgVaultNoLeaveAlone);
  Engine_EventShowMessage(ACTOR_SUSPECTED_THIEF, 0);
  Engine_ActorSetSpeed(ACTOR_SUSPECTED_THIEF, PIXELS(7) / 2, PIXELS(7) / 4);
  switch (actor->timer) {
  case 4:
    if (facing > 0x2000 && facing < 0xa000) {
      Actor_SetActionScript(ACTOR_SUSPECTED_THIEF, gVaultInteriors_PairScriptR);
      actor->timer = 2;
    } else {
      Actor_SetActionScript(ACTOR_SUSPECTED_THIEF, gVaultInteriors_PairScriptP);
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
  Object_RefreshSelectorById(ACTOR_SUSPECTED_THIEF);
  Engine_EventEnd();
}

// "Did those thieves come from Lunpa?"
static void Event_AskWhetherThievesCameFromLunpa(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgVaultDidThoseThievesComeFrom);
  VaultInteriors_AskYesNoAndShowReply(10);
  Engine_EventEnd();
}

// The child's father worries about the prisoners from Lunpa.
static void Event_ChildWorriesAboutFather(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgVaultMyFatherWorriedAboutThose);
  VaultInteriors_StandAndShowMessage(ACTOR_CHILD);
  Engine_EventEnd();
}

// "I heard you defeated those thieves!"
static void Event_AskAboutDefeatingThieves(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgVaultHeardDefeatedThoseThieves);
  VaultInteriors_AskYesNoAndShowReply(14);
  Engine_EventEnd();
}

// After the thieves are jailed, the mayor thanks Isaac and gives the promised
// Water of Life, if there is room; then asks whether the party is heading
// beyond the Goma Range, and points them to the cave on the way to Bilibin.
static void Event_MayorGivesWaterOfLife(void) {
  Engine_EventBegin();
  if (GameFlag_IsSet(FLAG_VAULT_WATER_OF_LIFE_RECEIVED) == 0) {
    Engine_EventSetMessage(MsgVaultYouWereSuchGreatHelp);
    VaultInteriors_ShowMessageAndWait(ACTOR_MAYOR, 20);
    VaultInteriors_PlayAnimationAndWait(ACTOR_MAYOR, ANIM_NOD, 20);
    VaultInteriors_ShowMessageAndWait(ACTOR_MAYOR, 30);
    Actor_FaceDirection(ACTOR_MAYOR, FACING_EAST, 0);
    Engine_EventWait(30);
    Engine_ActorRunRepeatedMotion(ACTOR_MAYOR, 2);
    Engine_EventWait(30);
    VaultInteriors_FaceEachOtherAndWait(ACTOR_PARTY_LEADER, 16, 20);
    VaultInteriors_PlayAnimationAndWait(ACTOR_MAYOR, ANIM_NOD, 20);
    gEventWork->message++;
    if (PartyInventory_HasSpace() == 0) {
      Engine_EventSetMessage(MsgVaultYouWereSuchGreatHelp + 3);
      VaultInteriors_ShowMessageAndWait(ACTOR_MAYOR, 20);
      Engine_EventEnd();
      return;
    }
    GameFlag_Set(FLAG_VAULT_WATER_OF_LIFE_RECEIVED);
    Engine_PartyGiveItem(ITEM_WATER_OF_LIFE, 0);
  }
  Engine_EventSetMessage(MsgVaultHeadingOutBeyondGomaRange);
  Event_OpenMessage(ACTOR_MAYOR, 0);
  Engine_EventWait(20);
  if (Engine_EventChooseYesNo(0, 0) != 0) {
    gEventWork->message++;
  }
  Event_ShowMessage(ACTOR_MAYOR, 0);
  Engine_EventEnd();
}

// The bone giver offers leftover bones from dinner; a yes gets a Bone if
// there is room, a no a nod.
static void Event_OfferBones(void) {
  Engine_EventBegin();
  VaultInteriors_FaceEachOtherAndWait(ACTOR_BONE_GIVER, ACTOR_PARTY_LEADER, 2);
  if (GameFlag_IsSet(FLAG_VAULT_BONE_RECEIVED) == 0) {
    Engine_EventSetMessage(MsgVaultHaveLotLeftoverBonesFrom);
    Event_OpenMessage(ACTOR_BONE_GIVER, 0);
  } else {
    Engine_EventSetMessage(MsgVaultWantMoreBones);
    Event_OpenMessage(ACTOR_BONE_GIVER, 0);
  }
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_EventWait(20);
    Event_ShowMessage(ACTOR_BONE_GIVER, 0);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(ACTOR_BONE_GIVER, 2);
    Engine_EventWait(20);
    if (PartyInventory_HasSpace() == 0) {
      Engine_ActorSetAnimationAndWait(ACTOR_BONE_GIVER, ANIM_SHAKE_HEAD);
      Engine_EventWait(20);
      Engine_EventSetMessage(MsgVaultWowHaveManyThingsArent);
      Event_ShowMessage(ACTOR_BONE_GIVER, 0);
    } else {
      Engine_ItemShowFound(ITEM_BONE, 3);
      Engine_PartyGiveItem(ITEM_BONE, 0);
      GameFlag_Set(FLAG_VAULT_BONE_RECEIVED);
    }
  } else {
    gEventWork->message++;
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(ACTOR_BONE_GIVER, ANIM_NOD);
    Engine_EventWait(20);
    Event_ShowMessage(ACTOR_BONE_GIVER, 0);
  }
  Actor_FaceDirection(ACTOR_BONE_GIVER, FACING_SOUTH, 0);
  Engine_EventEnd();
}

// Mind Read on the wife: let me see that rock, or maybe it is worthless.
static void Event_ReadMindOnRock(void) {
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(FLAG_VAULT_THIEVES_JAILED) == 0) {
    Engine_EventSetMessage(MsgVaultOkMisterLetMeSee);
  } else {
    Engine_EventSetMessage(MsgVaultIfRockWorthlessMaybeThats);
  }
  SceneActor_RunActorCommandWithFlag91(ACTOR_GEM_OWNERS_WIFE);
  Engine_EventEnd();
}

// Mind Read on the child: what is the outside world like, or Father looks
// sad.
static void Event_ReadChildMind(void) {
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(FLAG_VAULT_THIEVES_JAILED) == 0) {
    Engine_EventSetMessage(MsgVaultWonderOutsideWorldLike);
  } else {
    Engine_EventSetMessage(MsgVaultFatherLooksSadWorryingLike);
  }
  SceneActor_RunActorCommandWithFlag91(ACTOR_CHILD);
  Engine_EventEnd();
}

// Mind Read: could they be thieves? Who can be trusted? Or nothing in the
// house was worth stealing.
static void Event_ReadMindOnThieves(void) {
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(FLAG_VAULT_THIEVES_JAILED) == 0) {
    Engine_EventSetMessage(MsgVaultCouldTheyThievesGoodDont);
  } else {
    Engine_EventSetMessage(MsgVaultGuessNothingInOurHouse);
  }
  SceneActor_RunActorCommandWithFlag91(12);
  Engine_EventEnd();
}

// Mind Read on the mayor: "That tickles...", or the cave in the Goma Range is
// dangerous, but the party will be okay.
static void Event_ReadMayorMind(void) {
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(FLAG_VAULT_THIEVES_JAILED) == 0) {
    Engine_EventSetMessage(MsgVaultTicklesBeingTickledByBoy);
  } else {
    Engine_EventSetMessage(MsgVaultCaveInGomaRangeDangerous);
  }
  SceneActor_RunActorCommandWithFlag91(ACTOR_MAYOR);
  Engine_EventEnd();
}

// Mind Read on the bone giver: the master and his wife are blinded by money;
// after the thieves are jailed, hungry while cooking, or, once a Bone was
// given, wondering about calcium.
static void Event_ReadBoneGiverMind(void) {
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(FLAG_VAULT_THIEVES_JAILED) == 0) {
    Engine_EventSetMessage(MsgVaultMasterHisWifeBlindedBy);
  } else if (Engine_GameFlagIsSet(FLAG_VAULT_BONE_RECEIVED) == 0) {
    Engine_EventSetMessage(MsgVaultGeeAlwaysGetHungryWhen);
  } else {
    Engine_EventSetMessage(MsgVaultHeReallyLikesBonesWonder);
  }
  SceneActor_RunActorCommandWithFlag91(ACTOR_BONE_GIVER);
  Engine_EventEnd();
}

// Talking to Ivan at the mayor's: the first time, he reads the two's minds,
// admits his strange powers and learns they are called Psynergy; then he asks
// for help finding the rod. A yes has him join the party.
static void Event_MeetIvan(void) {

  struct FieldActor *actor;

  Engine_EventBegin();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(GARET, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(IVAN, PIXELS(4) / 5, PIXELS(2) / 5);
  Event_PlayCue(MUS_019);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 384, 408);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_SetPosition(GARET, PIXELS(384), PIXELS(408));
  Actor_WalkToAndWait(GARET, 368, 408);
  Actor_FaceDirection(GARET, FACING_NORTH + FACING_STEP, 20);
  // Introduce Ivan's ability only on the first meeting.
  if (GameFlag_IsSet(FLAG_VAULT_MET_IVAN) == 0) {
    GameFlag_Set(FLAG_VAULT_MET_IVAN);
    VaultInteriors_BeginMindRead(IVAN, ACTOR_PARTY_LEADER);
    Engine_EventWait(40);
    VaultInteriors_EndMindRead();
    Engine_EventSetMessage(MsgVaultDoPossessStrangePowers);
    Event_PlayCue(MUS_060);
    Engine_EventWait(30);
    VaultInteriors_PlayAnimationAndWait(IVAN, ANIM_NOD, 30);
    VaultInteriors_ShowMessageAndWait(IVAN, 30);
    Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Engine_ActorRunRepeatedMotion(GARET, 1);
    Engine_EventWait(20);
    VaultInteriors_BeginMindRead(IVAN, ACTOR_PARTY_LEADER);
    Engine_EventWait(40);
    VaultInteriors_EndMindRead();
    VaultInteriors_ShowMessageAndWait(IVAN, 30);
    VaultInteriors_FaceEachOtherAndWait(ACTOR_PARTY_LEADER, GARET, 50);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Actor_FaceDirection(GARET, FACING_NORTH + FACING_STEP, 0);
    Engine_EventWait(20);
    VaultInteriors_BeginMindRead(IVAN, ACTOR_PARTY_LEADER);
    Engine_EventWait(40);
    VaultInteriors_EndMindRead();
    Engine_ActorRunRepeatedMotion(IVAN, 1);
    VaultInteriors_ShowMessageAndWait(IVAN, 50);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    VaultInteriors_PlayAnimationAndWait(GARET, ANIM_NOD, 20);
    VaultInteriors_PlayAnimationAndWait(IVAN, ANIM_NOD, 20);
    VaultInteriors_ShowMessageAndWait(IVAN, 40);
    Engine_ActorRunRepeatedMotion(IVAN, 1);
    Engine_EventWait(30);
    Actor_FaceDirection(IVAN, FACING_NORTH, 0);
    Engine_EventWait(30);
    Actor_WalkToAndWait(IVAN, 376, 376);
    Engine_EventWait(40);
    VaultInteriors_FaceEachOtherAndWait(ACTOR_PARTY_LEADER, GARET, 50);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Actor_FaceDirection(GARET, FACING_NORTH + FACING_STEP, 0);
    Engine_ActorRunRepeatedMotion(IVAN, 1);
    Engine_EventWait(50);
    VaultInteriors_PlayAnimationAndWait(IVAN, ANIM_NOD, 30);
    Actor_FaceDirection(IVAN, FACING_SOUTH, 0);
    Engine_EventWait(10);
    Event_ShowMessage(IVAN, 0);
    Event_OpenMessage(IVAN, 0);
  } else {
    Event_PlayCue(MUS_060);
    Engine_EventSetMessage(MsgVaultWouldReallyWouldHelpMe);
    Event_OpenMessage(IVAN, 0);
  }
  // Accepting the search lets Ivan merge into the leader's position.
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    VaultInteriors_IvanJoinsRodSearch();
    GameFlag_Set(FLAG_VAULT_IVAN_JOINED_ROD_SEARCH);
    Engine_ActorSetAnimation(IVAN, ANIM_WALK);
    actor = Object_GetById(ACTOR_PARTY_LEADER);
    if (actor != 0) {
      Actor_SetDestination(IVAN, actor->x.part.pixel, actor->z.part.pixel);
    }
    Engine_ActorWaitForMove(IVAN);
    Actor_SetPosition(IVAN, 0, 0);
  } else {
    Event_ShowMessage(IVAN, 0);
  }
  Engine_ActorSetAnimation(GARET, ANIM_WALK);
  actor = Object_GetById(ACTOR_PARTY_LEADER);
  if (actor != 0) {
    Actor_SetDestination(GARET, actor->x.part.pixel, actor->z.part.pixel);
  }
  Engine_ActorWaitForMove(GARET);
  Actor_SetPosition(GARET, 0, 0);
  Audio_PlayCueFromEventWork();
  Engine_EventEnd();
}

// Ivan thanks them, they exchange names, and he shows Isaac and Garet Mind
// Read.
static void VaultInteriors_IvanJoinsRodSearch(void) {
  s32 message;

  Engine_EventWait(20);
  Object_GetById(IVAN)->paused = 0;
  Engine_ActorJump(IVAN, 4, 0);
  Engine_EventWait(40);
  message = MsgVaultThankHelpBelieve;
  Engine_EventSetMessage(message);
  Engine_EventWait(20);
  VaultInteriors_ShowMessageAndWait(IVAN, 20);
  Actor_ShowEmote(IVAN, 0x101, 0);
  Engine_EventWait(50);
  VaultInteriors_ShowMessageAndWait(IVAN, 30);
  Actor_WalkToAndWait(IVAN, 376, 392);
  VaultInteriors_BeginMindRead(IVAN, ACTOR_PARTY_LEADER);
  Engine_EventWait(40);
  VaultInteriors_EndMindRead();
  Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  Object_GetById(ACTOR_PARTY_LEADER)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 384, 424);
  Engine_EventWait(1);
  Object_GetById(ACTOR_PARTY_LEADER)->steeringFlags |= ACTOR_FACES_MOTION;
  Engine_EventWait(30);
  Engine_EventShowMessage(IVAN, 0);
  // Demonstrate Mind Read and let Garet and Isaac react to Ivan's explanation.
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
  Actor_SetAttachedEffect(GARET, 0x102);
  Engine_EventWait(60);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  VaultInteriors_PlayAnimationAndWait(GARET, ANIM_NOD, 30);
  Actor_ShowEmote(IVAN, 0x101, 0);
  Engine_EventWait(60);
  Engine_ActorRunRepeatedMotion(IVAN, 1);
  Engine_EventWait(10);
  VaultInteriors_PlayAnimationAndWait(IVAN, ANIM_NOD, 20);
  Engine_EventShowMessage(IVAN, 0);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Engine_EventWait(10);
  VaultInteriors_FaceActorAndWait(GARET, ACTOR_PARTY_LEADER, 30);
  Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_EventWait(10);
  VaultInteriors_FaceActorAndWait(ACTOR_PARTY_LEADER, GARET, 40);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 0);
  Actor_ShowEmote(GARET, 0x102, 0);
  Engine_EventWait(60);
  Actor_ShowEmote(IVAN, 0x102, 0);
  Engine_EventWait(60);
  Engine_EventOpenMessage(IVAN, 0);
  Engine_ActorFaceActor(ACTOR_PARTY_LEADER, IVAN, 0);
  Engine_ActorFaceActor(GARET, IVAN, 0);
  Engine_EventChooseYesNo(0, 0);
  Engine_EventWait(30);
  VaultInteriors_PlayAnimationAndWait(IVAN, ANIM_NOD, 10);
  Actor_WalkToAndWait(IVAN, 384, 408);
  Engine_EventWait(10);
  Engine_MessageShowCentered(message + 5, 1);
  Engine_EventSetMessage(message + 6);
  Engine_ActorFaceEachOther(IVAN, GARET, 0);
  VaultInteriors_FaceActorAndWait(ACTOR_PARTY_LEADER, GARET, 20);
  Actor_ShowEmote(GARET, 0x102, 0);
  Engine_EventWait(60);
  Engine_ActorRunRepeatedMotion(GARET, 1);
  Object_GetById(GARET)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(GARET, 352, 408);
  Engine_EventWait(1);
  Object_GetById(GARET)->steeringFlags |= ACTOR_FACES_MOTION;
  Engine_EventWait(10);
  Actor_WalkTo(IVAN, 368, 408);
  Engine_EventWait(20);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 368, 424);
  Engine_ActorFaceActor(ACTOR_PARTY_LEADER, GARET, 0);
  Engine_ActorWaitForMove(IVAN);
  VaultInteriors_BeginMindRead(IVAN, GARET);
  VaultInteriors_PlayAnimationAndWait(GARET, ANIM_SHAKE_HEAD, 10);
  Engine_EventShowMessage(GARET, 0);
  VaultInteriors_EndMindRead();
  VaultInteriors_FaceEachOtherAndWait(IVAN, ACTOR_PARTY_LEADER, 30);
  Engine_EventShowMessage(IVAN, 0);
  Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  Engine_EventWait(30);
  VaultInteriors_PlayAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD, 30);
  VaultInteriors_PlayAnimationAndWait(IVAN, ANIM_NOD, 10);
  Engine_EventShowMessage(IVAN, 0);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Engine_ActorFaceActor(GARET, ACTOR_PARTY_LEADER, 0);
  Engine_EventShowMessage(GARET, 0);
  Engine_ActorFaceActor(ACTOR_PARTY_LEADER, GARET, 0);
  VaultInteriors_PlayAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD, 40);
  Engine_ActorFaceActor(ACTOR_PARTY_LEADER, IVAN, 0);
  VaultInteriors_FaceActorAndWait(GARET, IVAN, 30);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
  Actor_ShowEmote(GARET, 0x101, 0);
  Engine_EventWait(60);
  VaultInteriors_PlayAnimationAndWait(IVAN, ANIM_NOD, 20);
  Engine_EventShowMessage(IVAN, 0);
  Engine_ActorShowEmote(GARET, 0x103, 0);
  Engine_EventWait(60);
  VaultInteriors_FaceActorAndWait(GARET, ACTOR_PARTY_LEADER, 10);
  Engine_EventShowMessage(GARET, 0);
  Engine_ActorShowEmote(IVAN, 0x100, 0);
  Engine_EventWait(60);
  Engine_EventShowMessage(IVAN, 0);
  Actor_ShowEmote(GARET, 0x102, 0);
  Engine_EventWait(60);
  Engine_MessageShowCentered(message + 13, 1);
  Engine_EventSetMessage(message + 14);
  Engine_ActorFaceEachOther(IVAN, ACTOR_PARTY_LEADER, 0);
  Engine_ActorFaceActor(GARET, ACTOR_PARTY_LEADER, 0);
  Engine_ActorShowEmote(ACTOR_PARTY_LEADER, 0x102, 0);
  Engine_EventWait(60);
  VaultInteriors_BeginMindRead(IVAN, ACTOR_PARTY_LEADER);
  Ui_SetBank15PaletteAndClearRenderMode();
  VaultInteriors_ShowMessageAndWait(GARET, 30);
  VaultInteriors_EndMindRead();
  Engine_ActorFaceEachOther(IVAN, GARET, 0);
  VaultInteriors_PlayAnimationAndWait(IVAN, ANIM_NOD, 10);
  VaultInteriors_ShowMessageAndWait(IVAN, 20);
  Engine_ActorFaceEachOther(IVAN, ACTOR_PARTY_LEADER, 0);
  VaultInteriors_PlayAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD, 20);
  Engine_ActorRunRepeatedMotion(GARET, 1);
  Engine_EventWait(20);
  VaultInteriors_PlayAnimationAndWait(GARET, ANIM_SHAKE_HEAD, 10);
  VaultInteriors_ShowMessageAndWait(GARET, 30);
  Engine_ActorFaceActor(IVAN, GARET, 0);
  Actor_ShowEmote(IVAN, 0x101, 0);
  Engine_EventWait(30);
  VaultInteriors_FaceEachOtherAndWait(ACTOR_PARTY_LEADER, GARET, 10);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  VaultInteriors_PlayAnimationAndWait(GARET, ANIM_NOD, 30);
  Engine_ActorFaceActor(ACTOR_PARTY_LEADER, IVAN, 0);
  Engine_ActorFaceActor(GARET, IVAN, 0);
  Engine_ActorFaceActor(IVAN, ACTOR_PARTY_LEADER, 0);
  Engine_ActorShowEmote(IVAN, 0x106, 0);
  Engine_EventWait(60);
  VaultInteriors_ShowMessageAndWait(IVAN, 10);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  VaultInteriors_PlayAnimationAndWait(GARET, ANIM_NOD, 20);
  VaultInteriors_PlayAnimationAndWait(IVAN, ANIM_SHAKE_HEAD, 20);
  VaultInteriors_ShowMessageAndWait(IVAN, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
  Engine_ActorShowEmote(GARET, 0x101, 0);
  Engine_EventWait(60);
  VaultInteriors_PlayAnimationAndWait(IVAN, ANIM_NOD, 10);
  VaultInteriors_ShowMessageAndWait(IVAN, 30);
  Engine_ActorShowEmote(ACTOR_PARTY_LEADER, 0x105, 0);
  Engine_EventWait(60);
  VaultInteriors_PlayAnimationAndWait(IVAN, ANIM_SHAKE_HEAD, 10);
  VaultInteriors_ShowMessageAndWait(IVAN, 30);
  VaultInteriors_PlayAnimationAndWait(IVAN, ANIM_NOD, 10);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  VaultInteriors_PlayAnimationAndWait(GARET, ANIM_NOD, 20);
  Event_PrepareObjectAndApplyValue(IVAN, 1);
  Engine_EventSetMessage(message + 22);
  Engine_ActorRunRepeatedMotion(IVAN, 1);
  Engine_EventShowMessage(IVAN, 0);
}

// In the loft, Isaac cuts the roofer free; the three thieves arrive, boast of
// Lunpa and Dodonpa, ask to be let go, and attack.
static void Event_FreeRooferAndConfrontThieves(void) {
  s32 base;

  Engine_EventBegin();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(GARET, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(IVAN, PIXELS(4) / 5, PIXELS(2) / 5);
  Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 3);
  Engine_EventWait(20);
  base = MsgVaultVaultCutFree;
  Engine_MessageShowCentered(base, 1);
  base++;
  Engine_EventSetMessage(base);
  Engine_EventWait(30);
  Engine_ActorSetAnimation(ACTOR_ATTIC_ROOFER, ANIM_STAND);
  Engine_EventWait(60);
  VaultInteriors_PlayAnimationAndWait(ACTOR_ATTIC_ROOFER, ANIM_NOD, 40);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 792, 440);
  VaultInteriors_FaceActorAndWait(ACTOR_PARTY_LEADER, ACTOR_ATTIC_ROOFER, 20);
  Actor_SetPosition(GARET, PIXELS(792), PIXELS(440));
  Actor_SetPosition(IVAN, PIXELS(792), PIXELS(440));
  Actor_WalkTo(GARET, 808, 432);
  Actor_WalkTo(IVAN, 792, 456);
  Engine_ActorWaitForMove(GARET);
  Engine_ActorFaceActor(GARET, ACTOR_ATTIC_ROOFER, 0);
  Engine_ActorWaitForMove(IVAN);
  VaultInteriors_FaceActorAndWait(IVAN, ACTOR_ATTIC_ROOFER, 60);
  Actor_ShowEmote(ACTOR_ATTIC_ROOFER, 258, 0);
  Engine_EventWait(60);
  VaultInteriors_ShowMessageAndWait(ACTOR_ATTIC_ROOFER, 20);
  Engine_ActorSetAnimation(IVAN, ANIM_NOD);
  VaultInteriors_PlayAnimationAndWait(GARET, ANIM_NOD, 30);
  Engine_ActorRunRepeatedMotion(ACTOR_ATTIC_ROOFER, 2);
  Engine_EventWait(20);
  VaultInteriors_ShowMessageAndWait(ACTOR_ATTIC_ROOFER, 20);
  Actor_ShowEmote(IVAN, 258, 0);
  Actor_ShowEmote(GARET, 258, 0);
  Engine_EventWait(60);
  Actor_FaceDirection(ACTOR_ATTIC_ROOFER, FACING_SOUTHEAST + FACING_STEP, 0);
  Engine_EventWait(10);
  VaultInteriors_ShowMessageAndWait(ACTOR_ATTIC_ROOFER, 30);
  Actor_FaceDirection(ACTOR_ATTIC_ROOFER, FACING_NORTH + FACING_STEP, 0);
  Engine_EventWait(30);
  Actor_ShowEmote(ACTOR_ATTIC_ROOFER, 256, 0);
  Engine_EventWait(60);
  Actor_FaceDirection(ACTOR_ATTIC_ROOFER, FACING_NORTHWEST + FACING_STEP, 0);
  Engine_EventWait(40);
  Actor_FaceDirection(ACTOR_ATTIC_ROOFER, FACING_NORTH + FACING_STEP, 0);
  Engine_EventWait(40);
  VaultInteriors_FaceActorAndWait(ACTOR_ATTIC_ROOFER, ACTOR_PARTY_LEADER, 20);
  VaultInteriors_PlayAnimationAndWait(ACTOR_ATTIC_ROOFER, ANIM_SHAKE_HEAD, 30);
  VaultInteriors_ShowMessageAndWait(ACTOR_ATTIC_ROOFER, 20);
  Engine_ActorRunRepeatedMotion(IVAN, 1);
  VaultInteriors_ShowMessageAndWait(IVAN, 40);
  // Bring the thieves into the attic confrontation.
  Actor_SetPosition(ACTOR_THIEF_LEADER, PIXELS(744), PIXELS(408));
  Engine_AudioPlayCue(MUS_061);
  VaultInteriors_ShowMessageAndWait(ACTOR_THIEF_LEADER, 20);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  Engine_ActorStartRepeatedMotion(GARET, 1);
  Engine_ActorStartRepeatedMotion(IVAN, 1);
  Engine_ActorRunRepeatedMotion(ACTOR_ATTIC_ROOFER, 1);
  Engine_EventWait(30);
  Engine_ActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_THIEF_LEADER, 0);
  Engine_ActorFaceActor(GARET, ACTOR_THIEF_LEADER, 0);
  Engine_ActorFaceActor(IVAN, ACTOR_THIEF_LEADER, 0);
  VaultInteriors_FaceActorAndWait(ACTOR_ATTIC_ROOFER, ACTOR_THIEF_LEADER, 40);
  Actor_SetSpeed(ACTOR_THIEF_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(ACTOR_FIRST_THIEF, PIXELS(3) / 2, PIXELS(3) / 4);
  Actor_SetSpeed(ACTOR_SECOND_THIEF, PIXELS(3) / 2, PIXELS(3) / 4);
  Actor_SetPosition(ACTOR_FIRST_THIEF, PIXELS(744), PIXELS(408));
  Actor_SetPosition(ACTOR_SECOND_THIEF, PIXELS(744), PIXELS(408));
  Actor_WalkToAndWait(ACTOR_THIEF_LEADER, 792, 416);
  Actor_FaceDirection(ACTOR_THIEF_LEADER, FACING_SOUTHEAST + FACING_STEP, 0);
  Engine_EventWait(70);
  Engine_ActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_THIEF_LEADER, 0);
  Actor_FaceActor(GARET, ACTOR_THIEF_LEADER, 0);
  Engine_ActorFaceActor(IVAN, ACTOR_THIEF_LEADER, 0);
  Engine_ActorFaceActor(ACTOR_ATTIC_ROOFER, ACTOR_THIEF_LEADER, 0);
  Actor_SetActionScript(ACTOR_FIRST_THIEF,
                        gVaultInteriors_Villager11ExitScript);
  Engine_EventWait(40);
  Actor_SetActionScript(ACTOR_SECOND_THIEF,
                        gVaultInteriors_Villager12ExitScript);
  Object_RefreshSelectorById(ACTOR_SECOND_THIEF);
  Actor_FaceDirection(ACTOR_FIRST_THIEF, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_SECOND_THIEF, FACING_SOUTHEAST, 0);
  Engine_EventWait(40);
  VaultInteriors_PlayAnimationAndWait(ACTOR_THIEF_LEADER, ANIM_SHAKE_HEAD, 20);
  VaultInteriors_ShowMessageAndWait(ACTOR_THIEF_LEADER, 20);
  Engine_ActorRunRepeatedMotion(ACTOR_FIRST_THIEF, 1);
  Engine_EventWait(10);
  VaultInteriors_ShowMessageAndWait(ACTOR_FIRST_THIEF, 20);
  Engine_ActorRunRepeatedMotion(ACTOR_SECOND_THIEF, 1);
  Engine_EventWait(10);
  VaultInteriors_ShowMessageAndWait(ACTOR_SECOND_THIEF, 40);
  Actor_ShowEmote(IVAN, 257, 0);
  Engine_EventWait(60);
  VaultInteriors_PlayAnimationAndWait(ACTOR_THIEF_LEADER, ANIM_SHAKE_HEAD, 20);
  VaultInteriors_ShowMessageAndWait(ACTOR_THIEF_LEADER, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, 257, 0);
  Actor_ShowEmote(GARET, 257, 0);
  Engine_EventWait(60);
  Actor_ShowEmote(ACTOR_ATTIC_ROOFER, 256, 0);
  Engine_EventWait(40);
  Engine_ActorRunRepeatedMotion(ACTOR_ATTIC_ROOFER, 2);
  Engine_EventWait(20);
  VaultInteriors_ShowMessageAndWait(ACTOR_ATTIC_ROOFER, 20);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_ATTIC_ROOFER, 0);
  Engine_ActorFaceActor(GARET, ACTOR_ATTIC_ROOFER, 0);
  VaultInteriors_FaceActorAndWait(IVAN, ACTOR_ATTIC_ROOFER, 20);
  VaultInteriors_PlayAnimationAndWait(ACTOR_ATTIC_ROOFER, ANIM_SHAKE_HEAD, 30);
  VaultInteriors_ShowMessageAndWait(ACTOR_ATTIC_ROOFER, 20);
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  Engine_EventWait(20);
  Engine_ActorFaceActor(ACTOR_PARTY_LEADER, IVAN, 0);
  VaultInteriors_FaceActorAndWait(GARET, IVAN, 30);
  VaultInteriors_ShowMessageAndWait(GARET, 20);
  Actor_ShowEmote(ACTOR_ATTIC_ROOFER, 258, 0);
  Engine_EventWait(60);
  VaultInteriors_ShowMessageAndWait(ACTOR_ATTIC_ROOFER, 20);
  Engine_ActorRunRepeatedMotion(ACTOR_THIEF_LEADER, 1);
  Engine_EventWait(10);
  VaultInteriors_ShowMessageAndWait(ACTOR_THIEF_LEADER, 20);
  Engine_ActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_THIEF_LEADER, 0);
  Actor_FaceActor(GARET, ACTOR_THIEF_LEADER, 0);
  VaultInteriors_FaceActorAndWait(IVAN, ACTOR_THIEF_LEADER, 20);
  VaultInteriors_PlayAnimationAndWait(ACTOR_FIRST_THIEF, ANIM_NOD, 20);
  VaultInteriors_ShowMessageAndWait(ACTOR_FIRST_THIEF, 20);
  VaultInteriors_PlayAnimationAndWait(ACTOR_SECOND_THIEF, ANIM_SHAKE_HEAD, 20);
  VaultInteriors_ShowMessageAndWait(ACTOR_SECOND_THIEF, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, 258, 0);
  Actor_ShowEmote(GARET, 258, 0);
  Actor_ShowEmote(IVAN, 258, 0);
  Engine_EventWait(60);
  VaultInteriors_PlayAnimationAndWait(ACTOR_THIEF_LEADER, ANIM_NOD, 20);
  Engine_EventOpenMessage(ACTOR_THIEF_LEADER, 0);
  Engine_EventWait(50);
  Engine_ActorFaceActor(IVAN, ACTOR_PARTY_LEADER, 0);
  VaultInteriors_FaceActorAndWait(GARET, ACTOR_PARTY_LEADER, 30);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_EventWait(40);
    Engine_ActorFaceActor(GARET, ACTOR_THIEF_LEADER, 0);
    Actor_FaceActor(IVAN, ACTOR_THIEF_LEADER, 0);
    Engine_ActorRunRepeatedMotion(ACTOR_THIEF_LEADER, 2);
    Engine_EventWait(20);
    Engine_EventShowMessage(ACTOR_THIEF_LEADER, 0);
  } else {
    Engine_EventWait(40);
    Engine_ActorFaceActor(GARET, ACTOR_THIEF_LEADER, 0);
    Engine_ActorFaceActor(IVAN, ACTOR_THIEF_LEADER, 0);
    base = MsgVaultVaultThievesCaught;
    Engine_EventSetMessage(base);
    Engine_ActorRunRepeatedMotion(ACTOR_THIEF_LEADER, 2);
    Engine_EventWait(20);
    Engine_EventShowMessage(ACTOR_THIEF_LEADER, 0);
    base -= 3;
    Engine_EventSetMessage(base);
  }
  Actor_ShowEmote(ACTOR_FIRST_THIEF, 259, 0);
  Engine_EventWait(60);
  VaultInteriors_ShowMessageAndWait(ACTOR_FIRST_THIEF, 20);
  VaultInteriors_PlayAnimationAndWait(ACTOR_SECOND_THIEF, ANIM_SHAKE_HEAD, 20);
  VaultInteriors_ShowMessageAndWait(ACTOR_SECOND_THIEF, 30);
  VaultInteriors_FaceEachOtherAndWait(ACTOR_THIEF_LEADER, ACTOR_FIRST_THIEF,
                                      30);
  Engine_ActorSetAnimation(ACTOR_THIEF_LEADER, ANIM_NOD);
  VaultInteriors_PlayAnimationAndWait(ACTOR_FIRST_THIEF, ANIM_NOD, 30);
  VaultInteriors_FaceEachOtherAndWait(ACTOR_THIEF_LEADER, ACTOR_SECOND_THIEF,
                                      30);
  Engine_ActorSetAnimation(ACTOR_THIEF_LEADER, ANIM_NOD);
  VaultInteriors_PlayAnimationAndWait(ACTOR_SECOND_THIEF, ANIM_NOD, 40);
  Actor_FaceDirection(ACTOR_THIEF_LEADER, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_FIRST_THIEF, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_SECOND_THIEF, FACING_SOUTHEAST + FACING_STEP, 0);
  Engine_EventWait(20);
  VaultInteriors_PlayAnimationAndWait(ACTOR_THIEF_LEADER, ANIM_SHAKE_HEAD, 20);
  Engine_EventShowMessage(ACTOR_THIEF_LEADER, 0);
  // Preserve the return destinations before requesting the thieves' encounter.
  GameFlag_Set(FLAG_VAULT_THIEVES_CONFRONTED);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
  base = (s32)&SceneId_VaultInteriors;
  Event_SetNextDestination(base, 17);
  Event_SetDefeatDestination(base, 16);
  Battle_SetStart(3);
  Field_RequestBattleFromEncounterTable(12, 5);
  Engine_EventEnd();
}

// The party walks through to entrance 11.
static void Event_GoToEntrance11(void) {
  Engine_AudioPlayCue(SE_MAP_EXIT);
  Engine_EventRequestExit(11);
}

// The party climbs up into the inn's loft.
static void Event_ClimbToAttic(void) {
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1) / 2, PIXELS(1) / 4);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 728, 408);
  gEventWork->transitionFrames = 16;
  Event_PlayCue(SE_MAP_EXIT);
  Engine_EventRequestExit(15);
}

// Leaving the loft before the thieves are confronted, the party hears the
// roofer groan "Uhnn... Uhnn...".
static void Event_LeaveAttic(void) {
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1) / 2, PIXELS(1) / 4);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 744, 408);
  if (GameFlag_IsSet(FLAG_VAULT_THIEVES_CONFRONTED) == 0) {
    Engine_EventBegin();
    Engine_EventSetMessage(MsgVaultUhnnUhnn);
    Event_ShowMessage(ACTOR_ATTIC_ROOFER, 0);
    Engine_EventEnd();
  }
  gEventWork->transitionFrames = 16;
  Engine_AudioPlayCue(SE_MAP_EXIT);
  Engine_EventRequestExit(14);
}

// The suspects at the inn: "What do you want?", they spot that weird kid
// with the party and run from him.
static void Event_ApproachSuspects(void) {

  struct FieldActor *actor24;
  struct FieldActor *actor25;

  actor24 = Object_GetById(ACTOR_SUSPECTED_LEADER);
  actor25 = Object_GetById(ACTOR_SUSPECTED_THIEF);
  Engine_EventBegin();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(GARET, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(IVAN, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 232, 696);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 200, 696);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_SUSPECTED_THIEF, 256, 0);
  Actor_ShowEmote(ACTOR_SUSPECTED_LEADER, 256, 0);
  Engine_EventWait(60);
  VaultInteriors_FaceActorAndWait(ACTOR_SUSPECTED_THIEF, ACTOR_PARTY_LEADER,
                                  10);
  Engine_ActorRunRepeatedMotion(ACTOR_SUSPECTED_LEADER, 2);
  Engine_EventWait(20);
  Engine_EventSetMessage(MsgVaultWant);
  VaultInteriors_ShowMessageAndWait(ACTOR_SUSPECTED_LEADER, 20);
  Actor_SetAttachedEffect(ACTOR_SUSPECTED_THIEF, 258);
  Engine_EventWait(60);
  VaultInteriors_ShowMessageAndWait(ACTOR_SUSPECTED_THIEF, 20);
  Engine_ActorRunRepeatedMotion(ACTOR_SUSPECTED_LEADER, 1);
  VaultInteriors_ShowMessageAndWait(ACTOR_SUSPECTED_LEADER, 30);
  Actor_SetSpeed(ACTOR_SUSPECTED_LEADER, PIXELS(4), PIXELS(2));
  Actor_SetSpeed(ACTOR_SUSPECTED_THIEF, PIXELS(7) / 2, PIXELS(7) / 4);
  Actor_SetActionScript(ACTOR_SUSPECTED_THIEF, gVaultInteriors_PairScriptO);
  Actor_SetActionScript(ACTOR_SUSPECTED_LEADER, gVaultInteriors_PairScriptB);
  Object_RefreshSelectorById(ACTOR_SUSPECTED_LEADER);
  Map_CopyCellAttributes(14, 45, 3, 1, 14, 44);
  GameFlag_Set(FLAG_VAULT_SUSPECTS_APPROACHED);
  GameFlag_Set(FLAG_SUSPECTS_FLEEING);
  FieldScheduler_AddCallback(VaultInteriors_FaceSuspectsTowardLeader, 3200);
  actor24->timer = 1;
  actor25->timer = 3;
  Engine_EventEnd();
}

// Stepping out, the party talks it over: Ivan can't read their minds while
// they run; Garet tells Isaac to take the lead and corner them.
static void Event_DiscussSuspiciousGuests(void) {
  struct FieldActor *actor24;
  struct FieldActor *actor25;
  s32 base;

  actor24 = Object_GetById(ACTOR_SUSPECTED_LEADER);
  actor25 = Object_GetById(ACTOR_SUSPECTED_THIEF);
  Engine_EventBegin();
  Scheduler_RemoveCallback(VaultInteriors_FaceSuspectsTowardLeader);
  GameFlag_Clear(FLAG_SUSPECTS_FLEEING);
  if (actor24->timer <= 3) {
    Actor_SetActionScript(ACTOR_SUSPECTED_LEADER, gVaultInteriors_PairScriptG);
  } else {
    Actor_SetActionScript(ACTOR_SUSPECTED_LEADER, gVaultInteriors_PairScriptF);
  }
  if (actor25->timer <= 2) {
    Actor_SetActionScript(ACTOR_SUSPECTED_THIEF, gVaultInteriors_PairScriptK);
  } else {
    Actor_SetActionScript(ACTOR_SUSPECTED_THIEF, gVaultInteriors_PairScriptF);
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
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Actor_FaceActor(GARET, ACTOR_PARTY_LEADER, 0);
  VaultInteriors_FaceActorAndWait(IVAN, ACTOR_PARTY_LEADER, 30);
  Engine_ActorRunRepeatedMotion(IVAN, 1);
  base = MsgVaultTheyreActingSuspiciousSomethingsNot;
  Engine_EventSetMessage(base);
  Event_ShowMessage(IVAN, 0);
  Actor_FaceActor(ACTOR_PARTY_LEADER, IVAN, 0);
  VaultInteriors_FaceActorAndWait(GARET, IVAN, 20);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  VaultInteriors_PlayAnimationAndWait(GARET, ANIM_NOD, 20);
  VaultInteriors_FaceEachOtherAndWait(ACTOR_PARTY_LEADER, GARET, 10);
  Event_OpenMessage(GARET, 0);
  if (Engine_EventChooseYesNo(0, 0) != 0) {
    gEventWork->message++;
  }
  VaultInteriors_ShowMessageAndWait(GARET, 30);
  Engine_EventSetMessage((base + 4));
  Actor_FaceActor(ACTOR_PARTY_LEADER, IVAN, 0);
  VaultInteriors_FaceActorAndWait(GARET, IVAN, 50);
  Actor_ShowEmote(IVAN, 0x100, 0);
  Engine_EventWait(60);
  VaultInteriors_FaceEachOtherAndWait(ACTOR_PARTY_LEADER, GARET, 50);
  Actor_FaceActor(ACTOR_PARTY_LEADER, IVAN, 0);
  VaultInteriors_FaceActorAndWait(GARET, IVAN, 30);
  VaultInteriors_PlayAnimationAndWait(IVAN, ANIM_NOD, 10);
  VaultInteriors_ShowMessageAndWait(IVAN, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 0);
  Actor_ShowEmote(GARET, 0x102, 0);
  Engine_EventWait(60);
  VaultInteriors_PlayAnimationAndWait(IVAN, ANIM_NOD, 20);
  VaultInteriors_ShowMessageAndWait(IVAN, 30);
  Engine_EventSetMessage(MsgVaultIsaacTakeLead);
  Event_ShowMessage(GARET, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Engine_EventWait(40);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_ActorSetAnimation(GARET, ANIM_NOD);
  VaultInteriors_PlayAnimationAndWait(IVAN, ANIM_NOD, 50);
  Actor_WalkTo(IVAN, 248, 728);
  Actor_WalkToAndWait(GARET, 248, 728);
  Actor_SetPosition(GARET, 0, 0);
  Actor_SetPosition(IVAN, 0, 0);
  Actor_SetPosition(ACTOR_SUSPECTED_LEADER, PIXELS(104), PIXELS(696));
  Actor_SetPosition(ACTOR_SUSPECTED_THIEF, PIXELS(120), PIXELS(696));
  Actor_FaceDirection(ACTOR_SUSPECTED_LEADER, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_SUSPECTED_THIEF, FACING_WEST, 0);
  Map_CopyCellAttributes(14, 50, 3, 1, 14, 44);
  Engine_EventEnd();
}

// The party sets out to corner the two suspects.
static void Event_BeginCorneringSuspects(void) {
  Engine_GameFlagSet(0x107);
  Engine_GameFlagSet(FLAG_SCENE_TEMP_50);
  VaultInteriors_StartActorStops();
}

// Have the suspects react to the party's return and reset their route states.
static void VaultInteriors_ResetSuspectRoutes(void) {
  Engine_EventWait(30);
  Engine_ActorRunRepeatedMotion(ACTOR_SUSPECTED_LEADER, 1);
  Engine_EventWait(20);
  Engine_EventSetMessage(MsgVaultTheyreBack);
  VaultInteriors_ShowMessageAndWait(ACTOR_SUSPECTED_LEADER, 20);
  Actor_FaceDirection(ACTOR_SUSPECTED_THIEF, FACING_EAST, 20);
  Actor_SetAttachedEffect(ACTOR_SUSPECTED_THIEF, 0x102);
  Engine_ActorRunRepeatedMotion(ACTOR_SUSPECTED_THIEF, 2);
  VaultInteriors_ShowMessageAndWait(ACTOR_SUSPECTED_THIEF, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_SUSPECTED_LEADER, ANIM_SHAKE_HEAD);
  Engine_EventWait(20);
  VaultInteriors_ShowMessageAndWait(ACTOR_SUSPECTED_LEADER, 20);
  Actor_SetSpeed(ACTOR_SUSPECTED_LEADER, PIXELS(4), PIXELS(2));
  Actor_SetSpeed(ACTOR_SUSPECTED_THIEF, PIXELS(7) / 2, PIXELS(7) / 4);
  Actor_SetActionScript(ACTOR_SUSPECTED_THIEF, gVaultInteriors_PairScriptO);
  Actor_SetActionScript(ACTOR_SUSPECTED_LEADER, gVaultInteriors_PairScriptB);
  Object_RefreshSelectorById(ACTOR_SUSPECTED_LEADER);
  {
    struct FieldActor *suspect = Object_GetById(ACTOR_SUSPECTED_LEADER);
    u16 routeIndex = 1;

    suspect->timer = routeIndex;
  }
  {
    struct FieldActor *suspect = Object_GetById(ACTOR_SUSPECTED_THIEF);
    u16 routeIndex = 3;

    suspect->timer = routeIndex;
  }
  Map_CopyCellAttributes(14, 48, 4, 1, 14, 44);
}

// Set the three party images to the priority used during the mind-reading
// scene.
static inline void VaultInteriors_SetPartyImagePriority(void) {
  {
    struct AnimationObject *sprite = Object_GetById(ACTOR_PARTY_LEADER)->sprite;
    sprite->image.priority = 1;
  }
  {
    struct AnimationObject *sprite = Object_GetById(GARET)->sprite;
    sprite->image.priority = 1;
  }
  {
    struct AnimationObject *sprite = Object_GetById(IVAN)->sprite;
    sprite->image.priority = 1;
  }
}

// Once a suspect is cornered ("Uh-oh! I'm surrounded!"), Garet calls "Now,
// Ivan!" and Ivan reads their minds.
static void Event_ReadCorneredSuspectsMinds(void) {
  s32 actor;
  struct EventWork *work;

  work = gEventWork;
  Engine_EventBegin();
  Scheduler_RemoveCallback(VaultInteriors_UpdateActorStops);
  GameFlag_Clear(0x107);
  GameFlag_Clear(FLAG_SCENE_TEMP_50);
  Engine_ActorSetAnimation(ACTOR_SUSPECTED_LEADER, ANIM_STAND);
  Engine_ActorSetAnimation(ACTOR_SUSPECTED_THIEF, ANIM_STAND);
  Engine_ActorSetAnimation(IVAN, ANIM_STAND);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Engine_ActorSetAnimation(GARET, ANIM_STAND);
  Actor_FaceActor(GARET, IVAN, 0);
  Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, IVAN, 0);
  Actor_FaceActor(ACTOR_SUSPECTED_LEADER, IVAN, 0);
  Actor_FaceActor(ACTOR_SUSPECTED_THIEF, IVAN, 0);
  Engine_EventWait(10);
  actor = 24;
  // The movement callback reports which suspect remained blocked.
  switch (work->raisedTrigger) {
  case 202:
  case 203:
    Engine_EventSetMessage(MsgVaultImSurrounded);
    Actor_SetAttachedEffect(ACTOR_SUSPECTED_THIEF, 0x102);
    Engine_ActorRunRepeatedMotion(ACTOR_SUSPECTED_THIEF, 2);
    VaultInteriors_ShowMessageAndWait(ACTOR_SUSPECTED_THIEF, 20);
    if (work->raisedTrigger == 202) {
      actor = 25;
      break;
    }
    // Fall through to the first suspect when trigger 203 selects both.
    // fall through
  case 201:
    Engine_EventSetMessage(MsgVaultTheresNowhereRun);
    Actor_SetAttachedEffect(ACTOR_SUSPECTED_LEADER, 0x102);
    Engine_ActorRunRepeatedMotion(ACTOR_SUSPECTED_LEADER, 2);
    actor = 24;
    VaultInteriors_ShowMessageAndWait(ACTOR_SUSPECTED_LEADER, 20);
    break;
  }
  Engine_ActorSetAnimation(IVAN, ANIM_STAND);
  Actor_FaceActor(IVAN, actor, 0);
  Actor_FaceActor(GARET, IVAN, 0);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Engine_EventWait(20);
  Engine_EventSetMessage(MsgVaultNowIvan);
  VaultInteriors_ShowMessageAndWait(GARET, 20);
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  Engine_EventWait(20);
  VaultInteriors_ShowMessageAndWait(IVAN, 20);
  VaultInteriors_BeginMindRead(IVAN, actor);
  Ui_SetBank15PaletteAndClearRenderMode();
  Engine_EventWait(60);
  Engine_ActorRunRepeatedMotion(ACTOR_SUSPECTED_LEADER, 2);
  VaultInteriors_ShowMessageAndWait(ACTOR_SUSPECTED_LEADER, 20);
  Engine_ActorRunRepeatedMotion(ACTOR_SUSPECTED_THIEF, 2);
  VaultInteriors_ShowMessageAndWait(ACTOR_SUSPECTED_THIEF, 20);
  Actor_ShowEmote(IVAN, 0x100, 60);
  VaultInteriors_EndMindRead();
  Engine_ActorStartRepeatedMotion(GARET, 2);
  Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_EventWait(20);
  VaultInteriors_ShowMessageAndWait(GARET, 20);
  Actor_FaceActor(IVAN, GARET, 0);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
  Engine_EventWait(40);
  VaultInteriors_SetPartyImagePriority();
  gEventWork->transitionFrames = 24;
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  // Move the discussion away from the suspects while the screen is closed.
  VaultInteriors_DiscussMindReading();
  Map_CopyCellAttributes(14, 45, 3, 1, 14, 44);
  GameFlag_Set(FLAG_VAULT_SUSPECTS_MIND_READ);
  {
    struct FieldActor *suspect = Object_GetById(ACTOR_SUSPECTED_LEADER);
    s32 routeIndex = 5;

    suspect->timer = routeIndex;
  }
  {
    struct FieldActor *suspect = Object_GetById(ACTOR_SUSPECTED_THIEF);
    s32 routeIndex = 4;

    suspect->timer = routeIndex;
  }
  FieldScheduler_AddCallback(VaultInteriors_FaceSuspectsTowardLeader,
                             TASK_PRIORITY_SCENE);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
  Engine_EventEnd();
}

// Show the party's branching conversation about what Ivan learned.
static void VaultInteriors_DiscussMindReading(void) {
  s32 persistedWithQuestions;

  Audio_PlayCueFromEventWork();
  Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 1);
  {
    struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);

    leader->priorityFlags = ACTOR_PRIORITY_AUTOMATIC | leader->priorityFlags;
  }
  FieldMap_SetViewBounds(PIXELS(32), PIXELS(576), PIXELS(400), PIXELS(936));
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(GARET, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(IVAN, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(248), PIXELS(728));
  Actor_SetPosition(IVAN, PIXELS(264), PIXELS(744));
  Engine_ActorSetPosition(GARET, PIXELS(232), PIXELS(744));
  VaultInteriors_SetPartyImagePriority();
  Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, IVAN, 0);
  VaultInteriors_FaceActorAndWait(GARET, IVAN, 30);
  Actor_SetPosition(ACTOR_SUSPECTED_LEADER, PIXELS(104), PIXELS(696));
  Actor_SetPosition(ACTOR_SUSPECTED_THIEF, PIXELS(120), PIXELS(696));
  Engine_ActorFaceEachOther(ACTOR_SUSPECTED_LEADER, ACTOR_SUSPECTED_THIEF, 0);
  Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 0);
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
  Engine_ActorRunRepeatedMotion(GARET, 1);
  Engine_EventWait(10);
  Engine_EventSetMessage(MsgVaultLearn);
  VaultInteriors_ShowMessageAndWait(GARET, 20);
  VaultInteriors_PlayAnimationAndWait(IVAN, ANIM_NOD, 20);
  VaultInteriors_ShowMessageAndWait(IVAN, 20);
  VaultInteriors_FaceEachOtherAndWait(ACTOR_PARTY_LEADER, GARET, 50);
  Engine_ActorFaceActor(ACTOR_PARTY_LEADER, IVAN, 0);
  VaultInteriors_FaceActorAndWait(GARET, IVAN, 20);
  VaultInteriors_PlayAnimationAndWait(IVAN, ANIM_SHAKE_HEAD, 20);
  Engine_EventShowMessage(IVAN, 0);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 0);
  Actor_ShowEmote(GARET, 0x102, 0);
  Engine_EventWait(60);
  VaultInteriors_PlayAnimationAndWait(IVAN, ANIM_NOD, 20);
  VaultInteriors_ShowMessageAndWait(IVAN, 30);
  Engine_ActorRunRepeatedMotion(GARET, 1);
  Engine_EventWait(20);
  VaultInteriors_FaceEachOtherAndWait(ACTOR_PARTY_LEADER, GARET, 20);
  Engine_EventOpenMessage(GARET, 0);
  persistedWithQuestions = 0;
  if (Engine_EventChooseYesNo(0, 0) != 0) {
    Engine_EventWait(20);
    Engine_ActorStartRepeatedMotion(IVAN, 2);
    VaultInteriors_PlayAnimationAndWait(GARET, ANIM_SHAKE_HEAD, 30);
    Engine_EventOpenMessage(GARET, 0);
    persistedWithQuestions = 0;
    if (Engine_EventChooseYesNo(0, 0) != 0) {
      Engine_EventWait(20);
      Engine_ActorShowEmote(IVAN, 0x102, 0);
      Engine_EventWait(60);
      Engine_ActorRunRepeatedMotion(IVAN, 2);
      VaultInteriors_FaceEachOtherAndWait(ACTOR_PARTY_LEADER, IVAN, 20);
      Engine_EventOpenMessage(IVAN, 0);
      persistedWithQuestions = 0;
      if (Engine_EventChooseYesNo(2, 0) != 0) {
        Engine_EventWait(20);
        Engine_ActorShowEmote(IVAN, 0x105, 0);
        Engine_EventWait(60);
        VaultInteriors_ShowMessageAndWait(IVAN, 20);
        VaultInteriors_FaceActorAndWait(GARET, IVAN, 10);
        Engine_ActorRunRepeatedMotion(GARET, 1);
        Engine_EventWait(10);
        VaultInteriors_ShowMessageAndWait(GARET, 10);
        VaultInteriors_FaceActorAndWait(IVAN, GARET, 20);
        Engine_ActorShowEmote(IVAN, 0x101, 0);
        Engine_EventWait(60);
        VaultInteriors_PlayAnimationAndWait(IVAN, ANIM_SHAKE_HEAD, 20);
        VaultInteriors_ShowMessageAndWait(GARET, 10);
        VaultInteriors_FaceEachOtherAndWait(GARET, ACTOR_PARTY_LEADER, 20);
        VaultInteriors_PlayAnimationAndWait(GARET, ANIM_NOD, 20);
        persistedWithQuestions = 1;
        VaultInteriors_ShowMessageAndWait(GARET, 20);
      }
    }
  }
  if (persistedWithQuestions == 0) {
    Engine_EventSetMessage(MsgVaultLeave);
    VaultInteriors_ShowMessageAndWait(GARET, 20);
    VaultInteriors_FaceEachOtherAndWait(GARET, ACTOR_PARTY_LEADER, 20);
    VaultInteriors_ShowMessageAndWait(GARET, 20);
  }
  Engine_ActorShowEmote(ACTOR_PARTY_LEADER, 0x105, 0);
  Engine_EventWait(60);
  Engine_ActorRunRepeatedMotion(GARET, 1);
  VaultInteriors_ShowMessageAndWait(GARET, 10);
  VaultInteriors_PlayAnimationAndWait(GARET, ANIM_NOD, 10);
  Engine_ActorFaceEachOther(GARET, IVAN, 0);
  VaultInteriors_FaceEachOtherAndWait(ACTOR_PARTY_LEADER, IVAN, 10);
  Engine_EventShowMessage(GARET, 0);
  VaultInteriors_PlayAnimationAndWait(IVAN, ANIM_NOD, 10);
  Actor_WalkTo(IVAN, 248, 728);
  Actor_WalkToAndWait(GARET, 248, 728);
  Engine_ActorSetPosition(GARET, 0, 0);
  Engine_ActorSetPosition(IVAN, 0, 0);
}

// The leader stepping on one of the route triggers 11 to 18 moves the
// fleeing suspects on to their next spots.
static void Event_AdvanceSuspectRoutes(void) {
  switch (gEventWork->touchedTrigger) {
  case 11:
    VaultInteriors_AdvanceSuspectRoute(ACTOR_SUSPECTED_LEADER, 1, 2,
                                       gVaultInteriors_PairScriptC);
    VaultInteriors_AdvanceSuspectRoute(ACTOR_SUSPECTED_THIEF, 3, 4,
                                       gVaultInteriors_PairScriptR);
    break;
  case 12:
    VaultInteriors_AdvanceSuspectRoute(ACTOR_SUSPECTED_LEADER, 1, 4,
                                       gVaultInteriors_PairScriptG);
    VaultInteriors_AdvanceSuspectRoute(ACTOR_SUSPECTED_LEADER, 2, 3,
                                       gVaultInteriors_PairScriptD);
    VaultInteriors_AdvanceSuspectRoute(ACTOR_SUSPECTED_THIEF, 1, 3,
                                       gVaultInteriors_PairScriptO);
    break;
  case 13:
    VaultInteriors_AdvanceSuspectRoute(ACTOR_SUSPECTED_LEADER, 2, 1,
                                       gVaultInteriors_PairScriptA);
    VaultInteriors_AdvanceSuspectRoute(ACTOR_SUSPECTED_LEADER, 3, 6,
                                       gVaultInteriors_PairScriptJ);
    VaultInteriors_AdvanceSuspectRoute(ACTOR_SUSPECTED_THIEF, 2, 4,
                                       gVaultInteriors_PairScriptQ);
    break;
  case 14:
    VaultInteriors_AdvanceSuspectRoute(ACTOR_SUSPECTED_LEADER, 3, 2,
                                       gVaultInteriors_PairScriptC);
    VaultInteriors_AdvanceSuspectRoute(ACTOR_SUSPECTED_THIEF, 4, 3,
                                       gVaultInteriors_PairScriptP);
    break;
  case 15:
    VaultInteriors_AdvanceSuspectRoute(ACTOR_SUSPECTED_LEADER, 4, 5,
                                       gVaultInteriors_PairScriptH);
    VaultInteriors_AdvanceSuspectRoute(ACTOR_SUSPECTED_THIEF, 1, 2,
                                       gVaultInteriors_PairScriptM);
    break;
  case 16:
    VaultInteriors_AdvanceSuspectRoute(ACTOR_SUSPECTED_LEADER, 4, 1,
                                       gVaultInteriors_PairScriptB);
    VaultInteriors_AdvanceSuspectRoute(ACTOR_SUSPECTED_LEADER, 5, 6,
                                       gVaultInteriors_PairScriptI);
    VaultInteriors_AdvanceSuspectRoute(ACTOR_SUSPECTED_THIEF, 3, 1,
                                       gVaultInteriors_PairScriptL);
    break;
  case 17:
    VaultInteriors_AdvanceSuspectRoute(ACTOR_SUSPECTED_LEADER, 5, 4,
                                       gVaultInteriors_PairScriptF);
    VaultInteriors_AdvanceSuspectRoute(ACTOR_SUSPECTED_LEADER, 6, 3,
                                       gVaultInteriors_PairScriptE);
    VaultInteriors_AdvanceSuspectRoute(ACTOR_SUSPECTED_THIEF, 4, 2,
                                       gVaultInteriors_PairScriptN);
    break;
  case 18:
    VaultInteriors_AdvanceSuspectRoute(ACTOR_SUSPECTED_LEADER, 6, 5,
                                       gVaultInteriors_PairScriptH);
    VaultInteriors_AdvanceSuspectRoute(ACTOR_SUSPECTED_THIEF, 2, 1,
                                       gVaultInteriors_PairScriptK);
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

// On arrival: restores each room for the story so far, and plays the loft's
// scenes: the tied-up roofer, the rematch, and the defeat or victory.
s32 Scene_Initialize(void) {
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
  switch (gGameState.entrance) {
  case 5: {
    s32 zero = 0;

    Object_GetById(ACTOR_GEM_OWNER)->motionFlags = zero;
    Object_GetById(ACTOR_GEM_OWNER)->y.fixed = zero;
    Object_GetById(ACTOR_GEM_OWNER)->terrainHeight = zero;
  } break;
  case 10:
    if (Engine_GameFlagIsSet(FLAG_VAULT_MET_IVAN) != 0) {
      Actor_SetPosition(IVAN, PIXELS(376), PIXELS(376));
      Actor_FaceDirection(IVAN, FACING_SOUTH, 0);
    }
    if (Engine_GameFlagIsSet(FLAG_VAULT_IVAN_JOINED_ROD_SEARCH) != 0) {
      Engine_ActorSetPosition(IVAN, 0, 0);
    }
    if (Engine_GameFlagIsSet(FLAG_VAULT_THIEVES_JAILED) != 0) {
      Actor_SetPosition(ACTOR_MAYOR, PIXELS(536),
                        VAULT_INTERIORS_ENTRY_ACTOR_Z);
      Actor_SetBuiltinAction(ACTOR_MAYOR, ACTOR_ACTION_IDLE);
      Actor_FaceDirection(ACTOR_MAYOR, FACING_SOUTH + FACING_STEP, 0);
    }
    break;
  case 7:
  case 11:
    if (Engine_GameFlagIsSet(FLAG_VAULT_THIEVES_JAILED) != 0) {
      Actor_SetPosition(ACTOR_BONE_GIVER, PIXELS(568), PIXELS(648));
      Actor_SetBuiltinAction(ACTOR_BONE_GIVER, ACTOR_ACTION_IDLE);
      Actor_FaceDirection(ACTOR_BONE_GIVER, FACING_SOUTH, 0);
      OverlayObject_ConfigureObject22WithResource17(ITEM_BONE, PIXELS(568),
                                                    PIXELS(16), PIXELS(672));
      Task_AddCallback(Vault_UpdateBoneProximityFlag, TASK_PRIORITY_SCENE);
    } else if (Engine_GameFlagIsSet(FLAG_VAULT_SUSPECTS_MIND_READ) != 0) {
      Engine_ActorSetPosition(ACTOR_BONE_GIVER, 0, 0);
    }
    break;
  case 12:
    if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) != 0 &&
        Engine_GameFlagIsSet(FLAG_VAULT_SUSPECTS_APPROACHED) != 0 &&
        Engine_GameFlagIsSet(FLAG_VAULT_SUSPECTS_MIND_READ) == 0 &&
        Engine_GameFlagIsSet(FLAG_SUSPECTS_FLEEING) != 0) {
      Map_CopyCellAttributes(14, 45, 3, 1, 14, 44);
      Task_AddCallback(VaultInteriors_FaceSuspectsTowardLeader,
                       TASK_PRIORITY_SCENE);
      break;
    }
    if (Engine_GameFlagIsSet(FLAG_VAULT_IVAN_JOINED_ROD_SEARCH) != 0) {
      Actor_SetPosition(ACTOR_SUSPECTED_THIEF, PIXELS(120), PIXELS(696));
      Actor_FaceDirection(ACTOR_SUSPECTED_THIEF, FACING_WEST, 0);
    }
    if (Engine_GameFlagIsSet(FLAG_VAULT_SUSPECTS_APPROACHED) != 0) {
      Map_CopyCellAttributes(14, 45, 3, 1, 14, 44);
      if (Engine_GameFlagIsSet(FLAG_VAULT_SUSPECTS_MIND_READ) == 0) {
        Map_CopyCellAttributes(14, 50, 3, 1, 14, 44);
        break;
      }
      Object_GetById(ACTOR_SUSPECTED_LEADER)->timer = 5;
      Object_GetById(ACTOR_SUSPECTED_THIEF)->timer = 4;
      FieldScheduler_AddCallback(VaultInteriors_FaceSuspectsTowardLeader,
                                 TASK_PRIORITY_SCENE);
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
    Object_GetById(ACTOR_CRATE)->collisionFlags |= 4;
    Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_CRATE), 0);
    if (Engine_GameFlagIsSet(FLAG_VAULT_CRATE_MOVED) != 0) {
      Actor_SetPosition(ACTOR_CRATE, PIXELS(676), PIXELS(411));
      Map_CopyCellAttributes(101, 24, 3, 4, 41, 24);
    }
    break;
  case 15:
    Engine_EventWait(2);
    Map_CopyCellsTo(54, 2, 44, 21, 2, 8);
    Map_CopyCellsTo(54, 2, 44, 81, 2, 8);
    Engine_EventBegin();
    if (Engine_GameFlagIsSet(FLAG_VAULT_THIEVES_JAILED) != 0) {
      Engine_ActorSetAnimation(ACTOR_FIRST_CHEST, ANIM_WALK);
      Engine_ActorSetAnimation(ACTOR_SECOND_CHEST, ANIM_WALK);
      Engine_EventWait(1);
      Engine_ActorSetAnimation(ACTOR_THIRD_CHEST, ANIM_WALK);
      break;
    }
    Engine_ActorSetPosition(ACTOR_ATTIC_ROOFER, PIXELS(824), PIXELS(456));
    if (Engine_GameFlagIsSet(FLAG_VAULT_THIEVES_CONFRONTED) != 0) {
      VaultInteriors_ConfrontThievesAgain();
      Engine_EventEnd();
      break;
    }
    Engine_ActorSetAnimation(ACTOR_ATTIC_ROOFER, 7);
    VaultInteriors_OpenWindowTransition();
    Engine_AudioPlayCue(MUS_017);
    Engine_EventSetMessage(MsgVaultUhnnUhnn);
    VaultInteriors_ShowMessageAndWait(ACTOR_ATTIC_ROOFER, 10);
    Engine_EventEnd();
    break;
  case 16:
    Map_CopyCells(54, 2, 2, 8, 44, 21);
    Map_CopyCells(54, 2, 2, 8, 44, 81);
    Engine_ActorSetPosition(ACTOR_ATTIC_ROOFER, PIXELS(824), PIXELS(456));
    VaultInteriors_LoseToThieves();
    Engine_EventRequestExit(16);
    break;
  case 17:
    Map_CopyCells(54, 2, 2, 8, 44, 21);
    Map_CopyCells(54, 2, 2, 8, 44, 81);
    if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) == 0) {
      Actor_SetPosition(ACTOR_ATTIC_ROOFER, PIXELS(824), PIXELS(456));
      VaultInteriors_DefeatThieves();
      VaultInteriors_ReturnStolenGoods();
      break;
    }
    Engine_ActorSetAnimation(ACTOR_FIRST_CHEST, ANIM_WALK);
    Engine_ActorSetAnimation(ACTOR_SECOND_CHEST, ANIM_WALK);
    Engine_EventWait(1);
    Engine_ActorSetAnimation(ACTOR_THIRD_CHEST, ANIM_WALK);
    break;
  }
  return 0;
}

// Keep both suspects facing the leader while they evade the party.
static void VaultInteriors_FaceSuspectsTowardLeader(void) {
  struct FieldActor *origin = Object_GetById(ACTOR_PARTY_LEADER);
  struct FieldActor *first = Object_GetById(ACTOR_SUSPECTED_LEADER);
  struct FieldActor *second = Object_GetById(ACTOR_SUSPECTED_THIEF);

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
  Actor_SetPosition(ACTOR_THIEF_LEADER, PIXELS(792), PIXELS(416));
  Actor_SetPosition(ACTOR_FIRST_THIEF, PIXELS(800), PIXELS(400));
  Actor_SetPosition(ACTOR_SECOND_THIEF, PIXELS(776), PIXELS(416));
  Actor_FaceDirection(ACTOR_THIEF_LEADER, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_FIRST_THIEF, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_SECOND_THIEF, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_SetSpeed(ACTOR_FIRST_THIEF, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(ACTOR_SECOND_THIEF, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(784), PIXELS(448));
  Actor_SetPosition(GARET, PIXELS(808), PIXELS(432));
  Actor_SetPosition(IVAN, PIXELS(776), PIXELS(432));
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 19);
  Engine_ActorSetAnimation(GARET, 19);
  Engine_ActorSetAnimation(IVAN, 19);
  priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  Object_GetById(ACTOR_PARTY_LEADER)->priorityFlags = priorityFlags;
  Object_GetById(GARET)->priorityFlags = priorityFlags;
  Object_GetById(IVAN)->priorityFlags = priorityFlags;
  actor = Object_GetById(ACTOR_PARTY_LEADER);
  Engine_ActorSetSpriteFlags(actor, 0);
  actor = Object_GetById(IVAN);
  Engine_ActorSetSpriteFlags(actor, 0);
  actor = Object_GetById(GARET);
  Engine_ActorSetSpriteFlags(actor, 0);
  Actor_FaceDirection(ACTOR_ATTIC_ROOFER, FACING_NORTHWEST + FACING_STEP, 0);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
  Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 0);
  Engine_CameraWaitForMove();
  Engine_MapRedraw();
  VaultInteriors_OpenWindowTransition();
  Engine_EventWait(60);
  VaultInteriors_PlayAnimationAndWait(ACTOR_THIEF_LEADER, ANIM_NOD, 20);
  Engine_EventSetMessage(MsgVaultTheseKidsNothingWorryAbout);
  VaultInteriors_ShowMessageAndWait(ACTOR_THIEF_LEADER, 30);
  VaultInteriors_ShowMessageAndWait(ACTOR_ATTIC_ROOFER, 30);
  Actor_WalkTo(ACTOR_FIRST_THIEF, 808, 456);
  Actor_WalkTo(ACTOR_SECOND_THIEF, 792, 456);
  Engine_ActorWaitForMove(ACTOR_SECOND_THIEF);
  Actor_FaceDirection(ACTOR_SECOND_THIEF, FACING_EAST, 0);
  Engine_ActorWaitForMove(ACTOR_FIRST_THIEF);
  Actor_FaceDirection(ACTOR_FIRST_THIEF, FACING_EAST, 0);
  Engine_EventWait(30);
  VaultInteriors_PlayAnimationAndWait(ACTOR_FIRST_THIEF, ANIM_NOD, 20);
  VaultInteriors_ShowMessageAndWait(ACTOR_FIRST_THIEF, 20);
  VaultInteriors_FaceActorAndWait(ACTOR_SECOND_THIEF, ACTOR_PARTY_LEADER, 30);
  VaultInteriors_ShowMessageAndWait(ACTOR_SECOND_THIEF, 60);
  Engine_EventEnd();
}

// Arrange the attic confrontation and restart the thieves' battle.
static void VaultInteriors_ConfrontThievesAgain(void) {

  Actor_SetPosition(ACTOR_THIEF_LEADER, PIXELS(792), PIXELS(416));
  Actor_SetPosition(ACTOR_FIRST_THIEF, PIXELS(800), PIXELS(400));
  Actor_SetPosition(ACTOR_SECOND_THIEF, PIXELS(776), PIXELS(416));
  Actor_FaceDirection(ACTOR_THIEF_LEADER, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_FIRST_THIEF, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_SECOND_THIEF, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(792), PIXELS(440));
  Actor_SetPosition(GARET, PIXELS(808), PIXELS(432));
  Actor_SetPosition(IVAN, PIXELS(792), PIXELS(456));
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_FaceDirection(IVAN, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_FaceActor(ACTOR_ATTIC_ROOFER, ACTOR_THIEF_LEADER, 0);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
  Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 0);
  Engine_CameraWaitForMove();
  Engine_MapRedraw();
  Engine_TaskWait(1);
  gEventWork->transitionFrames = 32;
  VaultInteriors_OpenWindowTransition();
  Engine_EventWait(60);
  Engine_EventSetMessage(MsgVaultWontGoAway);
  Engine_ActorRunRepeatedMotion(ACTOR_FIRST_THIEF, 1);
  Engine_EventWait(20);
  VaultInteriors_ShowMessageAndWait(ACTOR_FIRST_THIEF, 30);
  Engine_ActorRunRepeatedMotion(ACTOR_SECOND_THIEF, 1);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_SECOND_THIEF, 0);
  VaultInteriors_FaceEachOtherAndWait(ACTOR_THIEF_LEADER, ACTOR_FIRST_THIEF,
                                      30);
  Engine_ActorSetAnimation(ACTOR_THIEF_LEADER, ANIM_NOD);
  VaultInteriors_PlayAnimationAndWait(ACTOR_FIRST_THIEF, ANIM_NOD, 30);
  VaultInteriors_FaceEachOtherAndWait(ACTOR_THIEF_LEADER, ACTOR_SECOND_THIEF,
                                      30);
  Engine_ActorSetAnimation(ACTOR_THIEF_LEADER, ANIM_NOD);
  VaultInteriors_PlayAnimationAndWait(ACTOR_SECOND_THIEF, ANIM_NOD, 40);
  Actor_FaceDirection(ACTOR_THIEF_LEADER, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_FIRST_THIEF, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_SECOND_THIEF, FACING_SOUTHEAST + FACING_STEP, 0);
  Engine_EventWait(20);
  VaultInteriors_PlayAnimationAndWait(ACTOR_THIEF_LEADER, ANIM_SHAKE_HEAD, 20);
  Event_ShowMessage(ACTOR_THIEF_LEADER, 0);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
  Event_SetNextDestination((s32)&SceneId_VaultInteriors, 17);
  Event_SetDefeatDestination((s32)&SceneId_VaultInteriors, 16);
  Battle_SetStart(3);
  Field_RequestBattleFromEncounterTable(12, 5);
}

// Read Ivan's owned Venus Djinn count for the shared dialogue.
static s32 VaultInteriors_GetIvanVenusDjinnCount(void) {
  return UnitGet(IVAN)->djinnOwnedCounts[ELEMENT_VENUS];
}

// Shared source is included here in ROM order.
#include "common/vault_djinn.h"

// Stage the defeated thieves and gather the townspeople in the attic.
static void VaultInteriors_DefeatThieves(void) {
  struct FieldActor *actor;
  struct EventWork *work;
  const s32 *actions;
  struct AnimationObject *thirdThiefSprite;

  actor = Object_GetById(ACTOR_SECOND_THIEF);
  thirdThiefSprite = actor->sprite;
  Engine_EventBegin();
  Actor_SetPosition(ACTOR_THIEF_LEADER, PIXELS(792), PIXELS(416));
  Actor_SetPosition(ACTOR_FIRST_THIEF, PIXELS(800), PIXELS(400));
  Actor_SetPosition(ACTOR_SECOND_THIEF, PIXELS(776), PIXELS(392));
  actor = Object_GetById(ACTOR_THIEF_LEADER);
  Engine_ActorSetSpriteFlags(actor, 0);
  actor = Object_GetById(ACTOR_FIRST_THIEF);
  Engine_ActorSetSpriteFlags(actor, 0);
  actor = Object_GetById(ACTOR_SECOND_THIEF);
  Engine_ActorSetSpriteFlags(actor, 0);
  Engine_ActorSetAnimation(ACTOR_THIEF_LEADER, 9);
  Engine_ActorSetAnimation(ACTOR_FIRST_THIEF, 9);
  Engine_ActorSetAnimation(ACTOR_SECOND_THIEF, 9);
  Object_GetById(ACTOR_SECOND_THIEF)->priorityFlags &= 254;
  thirdThiefSprite->image.priority = 3;
  actions = gVaultInteriors_ActionTable;
  Actor_SetActionScript(ACTOR_THIEF_LEADER, actions);
  Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(792), PIXELS(440));
  Actor_SetPosition(GARET, PIXELS(808), PIXELS(432));
  Actor_SetPosition(IVAN, PIXELS(776), PIXELS(440));
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_FaceDirection(IVAN, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_SetSpeed(ACTOR_ATTIC_ROOFER, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_FaceDirection(ACTOR_ATTIC_ROOFER, FACING_NORTHWEST + FACING_STEP, 0);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
  Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 0);
  Engine_CameraWaitForMove();
  Engine_MapRedraw();
  VaultInteriors_OpenWindowTransition();
  Actor_SetActionScript(ACTOR_FIRST_THIEF, actions);
  Engine_EventWait(30);
  Actor_SetActionScript(ACTOR_SECOND_THIEF, actions);
  Engine_EventWait(30);
  Engine_EventSetMessage(MsgVaultTheyTheyGotUs);
  VaultInteriors_ShowMessageAndWait(ACTOR_THIEF_LEADER, 20);
  Actor_ShowEmote(ACTOR_ATTIC_ROOFER, 0x102, 0);
  Engine_EventWait(60);
  VaultInteriors_ShowMessageAndWait(ACTOR_ATTIC_ROOFER, 20);
  Actor_WalkToAndWait(ACTOR_ATTIC_ROOFER, 808, 456);
  Actor_SetSpeed(GARET, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(GARET, 792, 432);
  Actor_FaceDirection(GARET, FACING_EAST, 0);
  Actor_WalkToAndWait(ACTOR_ATTIC_ROOFER, 808, 408);
  Actor_FaceDirection(ACTOR_ATTIC_ROOFER, FACING_WEST, 0);
  Actor_WalkToAndWait(GARET, 808, 432);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Engine_EventWait(20);
  VaultInteriors_PlayAnimationAndWait(ACTOR_ATTIC_ROOFER, ANIM_NOD, 20);
  VaultInteriors_ShowMessageAndWait(ACTOR_ATTIC_ROOFER, 20);
  Actor_WalkToAndWait(ACTOR_ATTIC_ROOFER, 768, 408);
  Engine_EventWait(20);
  Actor_FaceActor(ACTOR_ATTIC_ROOFER, ACTOR_PARTY_LEADER, 0);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_ATTIC_ROOFER, 0);
  Actor_FaceActor(GARET, ACTOR_ATTIC_ROOFER, 0);
  VaultInteriors_FaceActorAndWait(IVAN, ACTOR_ATTIC_ROOFER, 40);
  VaultInteriors_ShowMessageAndWait(ACTOR_ATTIC_ROOFER, 30);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_ActorSetAnimation(GARET, ANIM_NOD);
  VaultInteriors_PlayAnimationAndWait(IVAN, ANIM_NOD, 20);
  Actor_WalkToAndWait(ACTOR_ATTIC_ROOFER, 744, 408);
  Engine_EventWait(50);
  Engine_ActorRunRepeatedMotion(ACTOR_FIRST_THIEF, 2);
  VaultInteriors_ShowMessageAndWait(ACTOR_FIRST_THIEF, 20);
  Actor_SetActionScript(ACTOR_FIRST_THIEF, actions);
  Engine_ActorRunRepeatedMotion(GARET, 1);
  Engine_EventWait(20);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_FIRST_THIEF, 0);
  Actor_FaceActor(GARET, ACTOR_FIRST_THIEF, 0);
  VaultInteriors_FaceActorAndWait(IVAN, ACTOR_FIRST_THIEF, 20);
  VaultInteriors_ShowMessageAndWait(GARET, 20);
  Engine_ActorRunRepeatedMotion(ACTOR_SECOND_THIEF, 2);
  VaultInteriors_ShowMessageAndWait(ACTOR_SECOND_THIEF, 30);
  Actor_SetActionScript(ACTOR_SECOND_THIEF, actions);
  Actor_ShowEmote(GARET, 0x103, 0);
  Engine_EventWait(60);
  VaultInteriors_ShowMessageAndWait(GARET, 30);
  VaultInteriors_FaceActorAndWait(IVAN, ACTOR_PARTY_LEADER, 30);
  VaultInteriors_FaceActorAndWait(ACTOR_PARTY_LEADER, IVAN, 30);
  VaultInteriors_PlayAnimationAndWait(IVAN, ANIM_NOD, 20);
  VaultInteriors_ShowMessageAndWait(IVAN, 20);
  VaultInteriors_FaceActorAndWait(GARET, IVAN, 30);
  VaultInteriors_PlayAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD, 30);
  VaultInteriors_ShowMessageAndWait(IVAN, 20);
  Actor_ShowEmote(GARET, 0x100, 0);
  Engine_EventWait(60);
  VaultInteriors_ShowMessageAndWait(GARET, 30);
  Actor_ShowEmote(IVAN, 0x101, 0);
  Engine_EventWait(60);
  VaultInteriors_FaceEachOtherAndWait(ACTOR_PARTY_LEADER, GARET, 20);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  VaultInteriors_PlayAnimationAndWait(GARET, ANIM_NOD, 20);
  VaultInteriors_BeginMindRead(IVAN, ACTOR_PARTY_LEADER);
  Ui_SetBank15PaletteAndClearRenderMode();
  Actor_FaceActor(ACTOR_PARTY_LEADER, IVAN, 0);
  VaultInteriors_FaceActorAndWait(GARET, IVAN, 10);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  Engine_ActorRunRepeatedMotion(GARET, 1);
  Engine_EventWait(20);
  Actor_SetAttachedEffect(GARET, 0x102);
  Engine_EventWait(60);
  VaultInteriors_FaceActorAndWait(ACTOR_PARTY_LEADER, GARET, 10);
  Event_OpenMessage(GARET, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_EventWait(20);
    Actor_FaceActor(ACTOR_PARTY_LEADER, IVAN, 0);
    VaultInteriors_FaceActorAndWait(GARET, IVAN, 20);
    VaultInteriors_EndMindRead();
    Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Engine_ActorRunRepeatedMotion(GARET, 1);
    Engine_EventWait(20);
    VaultInteriors_ShowMessageAndWait(GARET, 20);
  } else {
    gEventWork->message++;
    Actor_ShowEmote(GARET, 0x102, 0);
    Engine_EventWait(60);
    VaultInteriors_ShowMessageAndWait(GARET, 20);
    Actor_FaceActor(ACTOR_PARTY_LEADER, IVAN, 0);
    VaultInteriors_FaceActorAndWait(GARET, IVAN, 20);
    VaultInteriors_EndMindRead();
    Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Engine_ActorRunRepeatedMotion(GARET, 1);
    Engine_EventWait(20);
  }
  Actor_ShowEmote(IVAN, 0x102, 0);
  Engine_EventWait(60);
  VaultInteriors_PlayAnimationAndWait(IVAN, ANIM_SHAKE_HEAD, 20);
  Engine_EventSetMessage(MsgVaultSeeThatsHappened);
  VaultInteriors_ShowMessageAndWait(IVAN, 20);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  VaultInteriors_PlayAnimationAndWait(GARET, ANIM_NOD, 40);
  Actor_WalkToAndWait(ACTOR_ATTIC_ROOFER, 808, 408);
  Actor_FaceDirection(ACTOR_ATTIC_ROOFER, FACING_WEST, 0);
  Engine_EventWait(30);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_ATTIC_ROOFER, 0);
  Actor_FaceActor(GARET, ACTOR_ATTIC_ROOFER, 0);
  VaultInteriors_FaceActorAndWait(IVAN, ACTOR_ATTIC_ROOFER, 20);
  Engine_ActorRunRepeatedMotion(ACTOR_ATTIC_ROOFER, 1);
  Engine_EventWait(20);
  VaultInteriors_ShowMessageAndWait(ACTOR_ATTIC_ROOFER, 20);
  Actor_SetSpeed(ACTOR_ATTIC_MAYOR, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(13, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(14, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetPosition(ACTOR_ATTIC_MAYOR, PIXELS(744), PIXELS(408));
  Actor_WalkToAndWait(ACTOR_ATTIC_MAYOR, 768, 408);
  VaultInteriors_FaceActorAndWait(ACTOR_ATTIC_MAYOR, ACTOR_THIEF_LEADER, 30);
  Actor_SetPosition(13, PIXELS(744), PIXELS(408));
  Actor_WalkToAndWait(13, 768, 408);
  Actor_SetPosition(14, PIXELS(744), PIXELS(408));
  Actor_WalkTo(14, 784, 400);
  Actor_WalkToAndWait(13, 776, 424);
  Engine_ActorWaitForMove(14);
  Actor_FaceActor(13, ACTOR_THIEF_LEADER, 0);
  VaultInteriors_FaceActorAndWait(14, ACTOR_THIEF_LEADER, 20);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_ATTIC_MAYOR, 0);
  Actor_FaceActor(GARET, ACTOR_ATTIC_MAYOR, 0);
  VaultInteriors_FaceActorAndWait(IVAN, ACTOR_ATTIC_MAYOR, 20);
  VaultInteriors_PlayAnimationAndWait(ACTOR_ATTIC_MAYOR, ANIM_SHAKE_HEAD, 20);
  VaultInteriors_ShowMessageAndWait(ACTOR_ATTIC_MAYOR, 20);
  VaultInteriors_ShowMessageAndWait(ACTOR_FIRST_THIEF, 20);
  VaultInteriors_ShowMessageAndWait(ACTOR_SECOND_THIEF, 30);
  VaultInteriors_FaceEachOtherAndWait(ACTOR_ATTIC_MAYOR, 13, 20);
  Engine_ActorRunRepeatedMotion(13, 1);
  Engine_EventWait(20);
  VaultInteriors_ShowMessageAndWait(13, 20);
  VaultInteriors_PlayAnimationAndWait(ACTOR_ATTIC_MAYOR, ANIM_NOD, 30);
  VaultInteriors_FaceEachOtherAndWait(ACTOR_ATTIC_MAYOR, 14, 20);
  VaultInteriors_PlayAnimationAndWait(ACTOR_ATTIC_MAYOR, ANIM_NOD, 30);
  VaultInteriors_FaceActorAndWait(ACTOR_ATTIC_MAYOR, ACTOR_THIEF_LEADER, 20);
  VaultInteriors_ShowMessageAndWait(ACTOR_ATTIC_MAYOR, 20);
  Engine_ActorSetAnimation(13, ANIM_NOD);
  VaultInteriors_PlayAnimationAndWait(14, ANIM_NOD, 20);
  VaultInteriors_FaceEachOtherAndWait(13, 14, 20);
  Engine_ActorSetAnimation(13, ANIM_NOD);
  VaultInteriors_PlayAnimationAndWait(14, ANIM_NOD, 20);
  Actor_WalkTo(14, 792, 392);
  Actor_WalkToAndWait(13, 784, 400);
  Actor_FaceActor(13, ACTOR_SECOND_THIEF, 0);
  Engine_ActorWaitForMove(14);
  VaultInteriors_FaceActorAndWait(14, ACTOR_FIRST_THIEF, 20);
  Engine_ActorRunRepeatedMotion(13, 1);
  Engine_EventWait(20);
  VaultInteriors_ShowMessageAndWait(13, 20);
  VaultInteriors_PlayAnimationAndWait(14, ANIM_SHAKE_HEAD, 20);
  VaultInteriors_ShowMessageAndWait(14, 30);
  VaultInteriors_FaceEachOtherAndWait(13, ACTOR_PARTY_LEADER, 20);
  VaultInteriors_ShowMessageAndWait(13, 20);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_ActorSetAnimation(GARET, ANIM_NOD);
  VaultInteriors_PlayAnimationAndWait(IVAN, ANIM_NOD, 50);
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
  GameFlag_Set(FLAG_VAULT_THIEVES_JAILED);
  Engine_EventBegin();
  {
    struct FieldActor *actor = Object_GetById(ACTOR_SECOND_THIEF);

    actor->priorityFlags = ACTOR_PRIORITY_AUTOMATIC | actor->priorityFlags;
  }
  Actor_MoveToAndWait(ACTOR_FIRST_CHEST, 872, 425);
  Actor_MoveToAndWait(ACTOR_SECOND_CHEST, 872, 409);
  Actor_MoveToAndWait(ACTOR_THIRD_CHEST, 872, 377);
  Actor_SetPosition(ACTOR_FIRST_THIEF, PIXELS(776), PIXELS(392));
  Actor_SetPosition(ACTOR_THIEF_LEADER, PIXELS(792), PIXELS(392));
  Actor_SetPosition(ACTOR_SECOND_THIEF, PIXELS(808), PIXELS(392));
  Engine_ActorSetAnimation(ACTOR_THIEF_LEADER, 5);
  Engine_ActorSetAnimation(ACTOR_FIRST_THIEF, 5);
  Engine_ActorSetAnimation(ACTOR_SECOND_THIEF, 5);
  Actor_FaceActor(ACTOR_FIRST_THIEF, ACTOR_PARTY_LEADER, 0);
  Actor_FaceActor(ACTOR_THIEF_LEADER, ACTOR_PARTY_LEADER, 0);
  Actor_FaceActor(ACTOR_SECOND_THIEF, ACTOR_PARTY_LEADER, 0);
  actor = Object_GetById(ACTOR_THIEF_LEADER);
  Engine_ActorSetSpriteFlags(actor, 1);
  actor = Object_GetById(ACTOR_FIRST_THIEF);
  Engine_ActorSetSpriteFlags(actor, 1);
  actor = Object_GetById(ACTOR_SECOND_THIEF);
  Engine_ActorSetSpriteFlags(actor, 1);
  Actor_SetPosition(13, PIXELS(768), PIXELS(408));
  Actor_SetPosition(14, PIXELS(768), PIXELS(424));
  Actor_MoveToAndWait(ACTOR_ATTIC_MAYOR, 784, 424);
  Actor_SetPosition(ACTOR_ATTIC_ROOFER, PIXELS(808), PIXELS(408));
  Actor_FaceActor(13, ACTOR_ATTIC_MAYOR, 0);
  Actor_FaceActor(ACTOR_ATTIC_ROOFER, ACTOR_ATTIC_MAYOR, 0);
  Actor_FaceActor(14, ACTOR_THIEF_LEADER, 0);
  Actor_FaceActor(ACTOR_ATTIC_MAYOR, ACTOR_THIEF_LEADER, 0);
  Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(792), PIXELS(440));
  Actor_SetPosition(GARET, PIXELS(808), PIXELS(440));
  Actor_SetPosition(IVAN, PIXELS(776), PIXELS(440));
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_THIEF_LEADER, 0);
  Actor_FaceActor(GARET, ACTOR_THIEF_LEADER, 0);
  Actor_FaceActor(IVAN, ACTOR_THIEF_LEADER, 0);
  gEventWork->transitionFrames = 30;
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(40);
  Engine_ActorRunRepeatedMotion(ACTOR_THIEF_LEADER, 2);
  aftermath = MsgVaultYouIsaacRightWontForget;
  Engine_EventSetMessage(aftermath);
  VaultInteriors_ShowMessageAndWait(ACTOR_THIEF_LEADER, 20);
  Engine_ActorRunRepeatedMotion(ACTOR_ATTIC_MAYOR, 1);
  Engine_EventWait(20);
  VaultInteriors_PlayAnimationAndWait(ACTOR_ATTIC_MAYOR, ANIM_SHAKE_HEAD, 20);
  VaultInteriors_ShowMessageAndWait(ACTOR_ATTIC_MAYOR, 20);
  Engine_ActorSetAnimation(13, ANIM_NOD);
  VaultInteriors_PlayAnimationAndWait(ACTOR_ATTIC_ROOFER, ANIM_NOD, 20);
  Engine_ActorStartRepeatedMotion(ACTOR_FIRST_THIEF, 2);
  Engine_ActorStartRepeatedMotion(ACTOR_SECOND_THIEF, 2);
  Engine_EventWait(60);
  Actor_SetSpeed(13, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkTo(13, 746, 408);
  Actor_FaceDirection(ACTOR_ATTIC_MAYOR, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_FaceDirection(14, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_SetActionScript(ACTOR_FIRST_THIEF, gVaultInteriors_VaultScriptB);
  Engine_EventWait(20);
  Actor_SetActionScript(ACTOR_THIEF_LEADER, gVaultInteriors_VaultScriptB);
  Engine_EventWait(15);
  Actor_SetActionScript(ACTOR_SECOND_THIEF, gVaultInteriors_VaultScriptB);
  Engine_EventWait(35);
  Actor_SetActionScript(ACTOR_ATTIC_ROOFER, gVaultInteriors_VaultScriptA);
  Engine_EventWait(20);
  Engine_ActorWaitForMove(13);
  Actor_SetPosition(13, 0, 0);
  Engine_EventWait(40);
  Actor_SetSpeed(ACTOR_ATTIC_MAYOR, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(14, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(ACTOR_ATTIC_MAYOR, 784, 408);
  Actor_FaceActor(ACTOR_ATTIC_MAYOR, ACTOR_PARTY_LEADER, 0);
  Actor_WalkToAndWait(14, 768, 408);
  VaultInteriors_FaceActorAndWait(14, ACTOR_PARTY_LEADER, 20);
  VaultInteriors_PlayAnimationAndWait(ACTOR_ATTIC_MAYOR, ANIM_NOD, 20);
  VaultInteriors_ShowMessageAndWait(ACTOR_ATTIC_MAYOR, 20);
  VaultInteriors_PlayAnimationAndWait(14, ANIM_NOD, 20);
  Actor_FaceDirection(14, FACING_SOUTHEAST, 10);
  VaultInteriors_ShowMessageAndWait(14, 20);
  VaultInteriors_FaceEachOtherAndWait(ACTOR_PARTY_LEADER, GARET, 50);
  VaultInteriors_FaceEachOtherAndWait(ACTOR_PARTY_LEADER, IVAN, 50);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_ATTIC_MAYOR, 0);
  Actor_FaceActor(GARET, ACTOR_ATTIC_MAYOR, 0);
  VaultInteriors_FaceActorAndWait(IVAN, ACTOR_ATTIC_MAYOR, 20);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_ActorSetAnimation(GARET, ANIM_NOD);
  VaultInteriors_PlayAnimationAndWait(IVAN, ANIM_NOD, 50);
  Engine_ActorFaceEachOther(ACTOR_ATTIC_MAYOR, 14, 0);
  VaultInteriors_PlayAnimationAndWait(ACTOR_ATTIC_MAYOR, ANIM_NOD, 20);
  VaultInteriors_ShowMessageAndWait(ACTOR_ATTIC_MAYOR, 20);
  Actor_SetActionScript(14, gVaultInteriors_VaultScriptC);
  Engine_EventWait(50);
  Actor_SetActionScript(ACTOR_ATTIC_MAYOR, gVaultInteriors_VaultScriptD);
  Actor_SetSpeed(GARET, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(IVAN, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(GARET, 792, 456);
  Actor_FaceDirection(GARET, FACING_NORTH + FACING_STEP, 0);
  Engine_EventWait(30);
  Actor_WalkToAndWait(IVAN, 792, 408);
  Actor_FaceDirection(IVAN, FACING_EAST, 0);
  Actor_WalkToAndWait(GARET, 808, 456);
  Actor_FaceDirection(GARET, FACING_NORTH + FACING_STEP, 0);
  Engine_EventWait(100);
  VaultInteriors_FaceActorAndWait(14, ACTOR_ATTIC_MAYOR, 60);
  VaultInteriors_FaceActorAndWait(ACTOR_ATTIC_MAYOR, 14, 40);
  VaultInteriors_PlayAnimationAndWait(ACTOR_ATTIC_MAYOR, ANIM_NOD, 40);
  Actor_FaceDirection(ACTOR_ATTIC_MAYOR, FACING_EAST, 0);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(ACTOR_ATTIC_MAYOR, 2);
  Event_PlayCue(SE_124);
  Engine_ActorSetAnimation(ACTOR_FIRST_CHEST, ANIM_SHAKE_HEAD);
  Actor_SetPosition(18, PIXELS(872), PIXELS(424));
  Engine_ActorSetSpritePriority(18, 1);
  Actor_SetSpeed(18, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetDestinationOffset(18, 0, -8);
  Engine_ActorWaitForMove(18);
  Engine_ActorRunRepeatedMotion(18, 2);
  Engine_EventWait(60);
  Engine_MessageShowCentered((aftermath + 5), 1);
  Engine_ActorSetAnimation(ACTOR_FIRST_CHEST, ANIM_WALK);
  Actor_SetPosition(18, 0, 0);
  gEventWork->message++;
  Engine_ActorRunRepeatedMotion(14, 1);
  VaultInteriors_ShowMessageAndWait(14, 20);
  VaultInteriors_FaceEachOtherAndWait(ACTOR_PARTY_LEADER, GARET, 40);
  VaultInteriors_FaceActorAndWait(ACTOR_ATTIC_MAYOR, 14, 20);
  VaultInteriors_PlayAnimationAndWait(ACTOR_ATTIC_MAYOR, ANIM_NOD, 20);
  VaultInteriors_ShowMessageAndWait(ACTOR_ATTIC_MAYOR, 30);
  Actor_FaceActor(ACTOR_PARTY_LEADER, 14, 0);
  VaultInteriors_FaceActorAndWait(GARET, 14, 40);
  Actor_FaceDirection(14, FACING_EAST, 0);
  Engine_EventWait(40);
  Engine_ActorRunRepeatedMotion(14, 2);
  Event_PlayCue(SE_124);
  Engine_ActorSetAnimation(ACTOR_SECOND_CHEST, ANIM_SHAKE_HEAD);
  Object_GetById(19)->motionFlags = 0;
  Engine_ActorSetSpritePriority(19, 1);
  Actor_SetPosition(19, PIXELS(872), PIXELS(408));
  Actor_SetSpeed(19, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetDestinationOffset(19, 0, -8);
  Engine_ActorWaitForMove(19);
  Engine_ActorRunRepeatedMotion(19, 2);
  Engine_EventWait(60);
  Engine_MessageShowCentered((aftermath + 8), 1);
  Engine_ActorSetAnimation(ACTOR_SECOND_CHEST, ANIM_WALK);
  Actor_SetPosition(19, 0, 0);
  gEventWork->message++;
  Actor_ShowEmote(ACTOR_ATTIC_MAYOR, 0x102, 0);
  Engine_EventWait(60);
  VaultInteriors_ShowMessageAndWait(ACTOR_ATTIC_MAYOR, 20);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_ActorStartRepeatedMotion(GARET, 2);
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  Engine_EventWait(20);
  VaultInteriors_PlayAnimationAndWait(14, ANIM_NOD, 50);
  VaultInteriors_FaceActorAndWait(ACTOR_ATTIC_MAYOR, ACTOR_PARTY_LEADER, 20);
  VaultInteriors_ShowMessageAndWait(ACTOR_ATTIC_MAYOR, 30);
  Actor_FaceDirection(14, FACING_NORTH + FACING_STEP, 0);
  Engine_EventWait(30);
  Actor_ShowEmote(14, 0x100, 0);
  Engine_EventWait(60);
  Actor_WalkToAndWait(14, 856, 376);
  Engine_EventWait(20);
  VaultInteriors_FaceActorAndWait(14, ACTOR_ATTIC_MAYOR, 20);
  VaultInteriors_ShowMessageAndWait(14, 20);
  Actor_FaceActor(ACTOR_ATTIC_MAYOR, 14, 0);
  Actor_ShowEmote(IVAN, 0x100, 0);
  Engine_EventWait(60);
  VaultInteriors_FaceActorAndWait(IVAN, 14, 30);
  VaultInteriors_FaceActorAndWait(ACTOR_ATTIC_MAYOR, IVAN, 20);
  VaultInteriors_PlayAnimationAndWait(ACTOR_ATTIC_MAYOR, ANIM_NOD, 20);
  Event_ShowMessage(ACTOR_ATTIC_MAYOR, 0);
  Actor_FaceDirection(14, FACING_SOUTH + FACING_STEP, 0);
  Engine_EventWait(30);
  VaultInteriors_FaceActorAndWait(IVAN, ACTOR_ATTIC_MAYOR, 20);
  VaultInteriors_PlayAnimationAndWait(IVAN, ANIM_NOD, 20);
  VaultInteriors_ShowMessageAndWait(ACTOR_ATTIC_MAYOR, 20);
  Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, IVAN, 0);
  VaultInteriors_FaceActorAndWait(GARET, IVAN, 20);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  VaultInteriors_PlayAnimationAndWait(GARET, ANIM_NOD, 40);
  VaultInteriors_PlayAnimationAndWait(IVAN, ANIM_NOD, 30);
  Engine_ActorRunRepeatedMotion(ACTOR_ATTIC_MAYOR, 1);
  Engine_EventWait(20);
  VaultInteriors_PlayAnimationAndWait(ACTOR_ATTIC_MAYOR, ANIM_SHAKE_HEAD, 20);
  VaultInteriors_ShowMessageAndWait(ACTOR_ATTIC_MAYOR, 20);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_ATTIC_MAYOR, 0);
  Actor_FaceActor(GARET, ACTOR_ATTIC_MAYOR, 0);
  VaultInteriors_FaceActorAndWait(IVAN, ACTOR_ATTIC_MAYOR, 20);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  Engine_ActorStartRepeatedMotion(GARET, 1);
  Actor_ShowEmote(IVAN, 0x102, 0);
  Engine_EventWait(60);
  VaultInteriors_ShowMessageAndWait(IVAN, 40);
  VaultInteriors_PlayAnimationAndWait(ACTOR_ATTIC_MAYOR, ANIM_NOD, 20);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_SHAKE_HEAD);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_SHAKE_HEAD);
  Engine_ActorRunRepeatedMotion(IVAN, 3);
  Engine_EventWait(30);
  VaultInteriors_PlayAnimationAndWait(ACTOR_ATTIC_MAYOR, ANIM_SHAKE_HEAD, 20);
  VaultInteriors_ShowMessageAndWait(ACTOR_ATTIC_MAYOR, 20);
  VaultInteriors_FaceEachOtherAndWait(ACTOR_PARTY_LEADER, GARET, 20);
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  VaultInteriors_PlayAnimationAndWait(IVAN, ANIM_SHAKE_HEAD, 30);
  Actor_SetSpeed(IVAN, PIXELS(3) / 2, PIXELS(3) / 4);
  Actor_WalkToAndWait(IVAN, 800, 408);
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  Event_ShowMessage(IVAN, 0);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_ATTIC_MAYOR, 0);
  VaultInteriors_FaceActorAndWait(GARET, ACTOR_ATTIC_MAYOR, 30);
  VaultInteriors_PlayAnimationAndWait(ACTOR_ATTIC_MAYOR, ANIM_SHAKE_HEAD, 20);
  VaultInteriors_ShowMessageAndWait(ACTOR_ATTIC_MAYOR, 20);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
  Actor_SetAttachedEffect(GARET, 0x102);
  Actor_SetAttachedEffect(IVAN, 0x102);
  Engine_EventWait(60);
  VaultInteriors_ShowMessageAndWait(IVAN, 20);
  Engine_ActorRunRepeatedMotion(ACTOR_ATTIC_MAYOR, 1);
  Engine_EventWait(20);
  VaultInteriors_ShowMessageAndWait(ACTOR_ATTIC_MAYOR, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
  Actor_ShowEmote(GARET, 0x101, 0);
  Actor_ShowEmote(IVAN, 0x101, 0);
  Engine_EventWait(60);
  VaultInteriors_PlayAnimationAndWait(ACTOR_ATTIC_MAYOR, ANIM_SHAKE_HEAD, 20);
  VaultInteriors_ShowMessageAndWait(ACTOR_ATTIC_MAYOR, 20);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  Engine_ActorStartRepeatedMotion(GARET, 1);
  Engine_ActorRunRepeatedMotion(IVAN, 1);
  Engine_EventWait(20);
  VaultInteriors_PlayAnimationAndWait(ACTOR_ATTIC_MAYOR, ANIM_NOD, 20);
  VaultInteriors_ShowMessageAndWait(ACTOR_ATTIC_MAYOR, 40);
  Actor_ShowEmote(IVAN, 0x105, 0);
  Engine_EventWait(60);
  Actor_WalkToAndWait(ACTOR_ATTIC_MAYOR, 840, 424);
  VaultInteriors_FaceEachOtherAndWait(ACTOR_ATTIC_MAYOR, ACTOR_PARTY_LEADER,
                                      20);
  VaultInteriors_ShowMessageAndWait(ACTOR_ATTIC_MAYOR, 20);
  VaultInteriors_PlayAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD, 20);
  Actor_FaceDirection(ACTOR_ATTIC_MAYOR, FACING_SOUTH + FACING_STEP, 0);
  Engine_EventWait(20);
  VaultInteriors_ShowMessageAndWait(ACTOR_ATTIC_MAYOR, 20);
  Engine_ActorRunRepeatedMotion(GARET, 1);
  Engine_EventWait(20);
  VaultInteriors_ShowMessageAndWait(GARET, 20);
  VaultInteriors_PlayAnimationAndWait(ACTOR_ATTIC_MAYOR, ANIM_NOD, 20);
  VaultInteriors_ShowMessageAndWait(ACTOR_ATTIC_MAYOR, 30);
  VaultInteriors_FaceEachOtherAndWait(ACTOR_ATTIC_MAYOR, 14, 20);
  VaultInteriors_ShowMessageAndWait(ACTOR_ATTIC_MAYOR, 20);
  Actor_WalkToAndWait(14, 856, 408);
  Actor_SetActionScript(ACTOR_ATTIC_MAYOR, gVaultInteriors_VaultScriptE);
  Actor_SetActionScript(14, gVaultInteriors_VaultScriptE);
  Actor_WalkTo(GARET, 792, 456);
  Actor_SetSpeed(IVAN, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkTo(IVAN, 776, 432);
  Engine_ActorWaitForMove(GARET);
  Actor_FaceDirection(GARET, FACING_NORTH + FACING_STEP, 0);
  Engine_ActorWaitForMove(IVAN);
  Actor_FaceDirection(IVAN, FACING_NORTH + FACING_STEP, 0);
  Object_RefreshSelectorById(ACTOR_ATTIC_MAYOR);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_ATTIC_MAYOR, 0);
  temporaryActor = Object_GetById(14);
  {
    u8 *pauseFlag = &temporaryActor->paused;
    u8 paused = 1;

    *pauseFlag = paused;
  }
  temporaryActor->targetX = ACTOR_NO_TARGET;
  temporaryActor->targetY = ACTOR_NO_TARGET;
  temporaryActor->targetZ = ACTOR_NO_TARGET;
  Actor_ShowEmote(ACTOR_ATTIC_MAYOR, 0x100, 0);
  Engine_ActorSetAnimation(14, ANIM_STAND);
  Engine_EventWait(50);
  Actor_FaceActor(ACTOR_ATTIC_MAYOR, ACTOR_PARTY_LEADER, 0);
  Actor_WalkToAndWait(GARET, 808, 440);
  Actor_FaceDirection(GARET, FACING_NORTHWEST + FACING_STEP, 0);
  Engine_EventWait(20);
  VaultInteriors_ShowMessageAndWait(ACTOR_ATTIC_MAYOR, 20);
  VaultInteriors_FaceEachOtherAndWait(ACTOR_PARTY_LEADER, GARET, 50);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_ATTIC_MAYOR, 0);
  VaultInteriors_FaceActorAndWait(GARET, ACTOR_ATTIC_MAYOR, 20);
  VaultInteriors_PlayAnimationAndWait(ACTOR_ATTIC_MAYOR, ANIM_NOD, 20);
  Actor_WalkTo(ACTOR_ATTIC_MAYOR, 744, 408);
  Actor_WalkTo(14, 744, 408);
  Engine_ActorWaitForMove(ACTOR_ATTIC_MAYOR);
  Engine_ActorWaitForMove(14);
  Engine_EventWait(30);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 792, 408);
  VaultInteriors_FaceEachOtherAndWait(ACTOR_PARTY_LEADER, GARET, 30);
  Actor_ShowEmote(IVAN, 0x105, 0);
  Engine_EventWait(50);
  Actor_FaceActor(ACTOR_PARTY_LEADER, IVAN, 0);
  VaultInteriors_FaceActorAndWait(GARET, IVAN, 20);
  Engine_ActorRunRepeatedMotion(GARET, 1);
  Engine_EventWait(20);
  VaultInteriors_ShowMessageAndWait(GARET, 40);
  VaultInteriors_PlayAnimationAndWait(IVAN, ANIM_NOD, 30);
  VaultInteriors_FaceEachOtherAndWait(ACTOR_PARTY_LEADER, GARET, 40);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_SHAKE_HEAD);
  VaultInteriors_PlayAnimationAndWait(GARET, ANIM_SHAKE_HEAD, 30);
  Actor_FaceActor(ACTOR_PARTY_LEADER, IVAN, 0);
  VaultInteriors_FaceActorAndWait(GARET, IVAN, 20);
  Engine_ActorRunRepeatedMotion(GARET, 1);
  Engine_EventWait(20);
  VaultInteriors_ShowMessageAndWait(GARET, 40);
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  Engine_EventWait(20);
  VaultInteriors_ShowMessageAndWait(IVAN, 20);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  VaultInteriors_PlayAnimationAndWait(GARET, ANIM_NOD, 20);
  VaultInteriors_PlayAnimationAndWait(IVAN, ANIM_SHAKE_HEAD, 20);
  VaultInteriors_ShowMessageAndWait(IVAN, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
  Actor_ShowEmote(GARET, 0x101, 0);
  Engine_EventWait(60);
  VaultInteriors_PlayAnimationAndWait(IVAN, ANIM_NOD, 20);
  VaultInteriors_ShowMessageAndWait(IVAN, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 0);
  Actor_ShowEmote(GARET, 0x100, 0);
  Engine_EventWait(60);
  VaultInteriors_PlayAnimationAndWait(IVAN, ANIM_NOD, 20);
  VaultInteriors_ShowMessageAndWait(IVAN, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 0);
  Actor_ShowEmote(GARET, 0x102, 0);
  Engine_EventWait(60);
  VaultInteriors_PlayAnimationAndWait(IVAN, ANIM_SHAKE_HEAD, 20);
  VaultInteriors_ShowMessageAndWait(IVAN, 20);
  VaultInteriors_PlayAnimationAndWait(GARET, ANIM_NOD, 20);
  VaultInteriors_ShowMessageAndWait(GARET, 30);
  Event_OpenMessage(IVAN, 0);
  if (Engine_EventChooseYesNo(0, 0) != 0) {
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(GARET, 1);
    Engine_EventWait(20);
    VaultInteriors_ShowMessageAndWait(GARET, 20);
    VaultInteriors_PlayAnimationAndWait(IVAN, ANIM_SHAKE_HEAD, 20);
    VaultInteriors_ShowMessageAndWait(IVAN, 20);
  }
  VaultInteriors_PlayAnimationAndWait(IVAN, ANIM_NOD, 30);
  Actor_WalkToAndWait(IVAN, 800, 456);
  Actor_FaceActor(ACTOR_PARTY_LEADER, IVAN, 0);
  Actor_FaceActor(GARET, IVAN, 0);
  Actor_WalkToAndWait(IVAN, 856, 456);
  Actor_FaceActor(ACTOR_PARTY_LEADER, IVAN, 0);
  Actor_FaceActor(GARET, IVAN, 0);
  Actor_WalkToAndWait(IVAN, 856, 376);
  Actor_FaceActor(ACTOR_PARTY_LEADER, IVAN, 0);
  Actor_FaceActor(GARET, IVAN, 0);
  Actor_FaceDirection(IVAN, FACING_EAST, 0);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  temporaryActor =
      (struct FieldActor *)BattleFx_PlayCueAndStartEmitterOnTarget(2, 17, 65);
  Engine_EventWait(60);
  rod = MsgVaultIvanGotShamansRod;
  Engine_MessageShowCentered(rod, 1);
  Engine_ObjectDispatchRelease(temporaryActor);
  Engine_ActorSetAnimation(ACTOR_THIRD_CHEST, ANIM_WALK);
  Engine_EventWait(20);
  VaultInteriors_PlayAnimationAndWait(IVAN, ANIM_NOD, 20);
  Actor_WalkToAndWait(IVAN, 856, 456);
  Actor_FaceActor(ACTOR_PARTY_LEADER, IVAN, 0);
  Actor_FaceActor(GARET, IVAN, 0);
  Actor_WalkToAndWait(IVAN, 800, 456);
  Actor_FaceActor(ACTOR_PARTY_LEADER, IVAN, 0);
  Actor_FaceActor(GARET, IVAN, 0);
  Actor_WalkToAndWait(IVAN, 776, 424);
  Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, IVAN, 0);
  VaultInteriors_FaceActorAndWait(GARET, IVAN, 30);
  VaultInteriors_PlayAnimationAndWait(IVAN, ANIM_NOD, 20);
  Engine_EventSetMessage((rod + 1));
  VaultInteriors_ShowMessageAndWait(IVAN, 20);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  {
    u16 *slot = &event->message;
    s32 saved = (s16)*slot;

    if (VaultInteriors_GetIvanVenusDjinnCount() != 0) {
      Engine_EventSetMessage(MsgVaultWaitDontWantTakeYour);
      Event_ShowMessage(IVAN, 0);
      OverlayObject_RunObject2WhenFlagged();
    }
    Party_RemoveOwnerRestored(IVAN);
    *slot = saved;
  }
  VaultInteriors_PlayAnimationAndWait(GARET, ANIM_NOD, 50);
  Actor_WalkToAndWait(IVAN, 776, 408);
  Actor_FaceActor(ACTOR_PARTY_LEADER, IVAN, 0);
  Actor_FaceActor(GARET, IVAN, 0);
  Actor_WalkToAndWait(IVAN, 744, 408);
  Engine_EventWait(40);
  VaultInteriors_FaceEachOtherAndWait(ACTOR_PARTY_LEADER, GARET, 20);
  Engine_ActorRunRepeatedMotion(GARET, 1);
  Engine_EventWait(20);
  VaultInteriors_ShowMessageAndWait(GARET, 20);
  VaultInteriors_PlayAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD, 20);
  Engine_ActorSetAnimation(GARET, ANIM_WALK);
  actor = Object_GetById(ACTOR_PARTY_LEADER);
  if (actor != NULL) {
    Actor_SetDestination(GARET, actor->x.part.pixel, actor->z.part.pixel);
  }
  Engine_ActorWaitForMove(GARET);
  Actor_SetPosition(GARET, 0, 0);
  Engine_EventWait(30);
  Actor_SetPosition(ACTOR_ATTIC_ROOFER, 0, 0);
  Actor_SetPosition(ACTOR_ATTIC_MAYOR, 0, 0);
  Actor_SetPosition(13, 0, 0);
  Actor_SetPosition(14, 0, 0);
  Actor_SetPosition(ACTOR_THIEF_LEADER, 0, 0);
  Actor_SetPosition(ACTOR_FIRST_THIEF, 0, 0);
  Actor_SetPosition(ACTOR_SECOND_THIEF, 0, 0);
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

  leader = Engine_ActorLookup(ACTOR_PARTY_LEADER);
  blocked = 0;
  work = gEventWork;
  actor = Engine_ActorLookup(IVAN);
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
  actor = Engine_ActorLookup(ACTOR_SUSPECTED_LEADER);
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
        Engine_ActorSetAttachedEffect(ACTOR_SUSPECTED_LEADER, 2);
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

  actor = Engine_ActorLookup(ACTOR_SUSPECTED_THIEF);
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
        Engine_ActorSetAttachedEffect(ACTOR_SUSPECTED_THIEF, 2);
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
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(GARET, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(IVAN, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 248, 696);
  Actor_SetPosition(GARET, PIXELS(248), PIXELS(696));
  Actor_SetPosition(IVAN, PIXELS(248), PIXELS(696));
  Actor_WalkTo(ACTOR_PARTY_LEADER, 200, 696);
  Actor_WalkTo(GARET, 248, 712);
  Actor_WalkToAndWait(IVAN, 232, 696);
  Engine_ActorWaitForMove(GARET);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_WEST, 0);
  Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
  Engine_ActorSetAnimation(GARET, 12);
  VaultInteriors_ResetSuspectRoutes();
  FieldMap_SetViewBounds(PIXELS(48), PIXELS(576), PIXELS(288), PIXELS(736));
  Actor_SetSpeed(GARET, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(IVAN, PIXELS(3) / 4, PIXELS(3) / 8);
  Actor_SetSpeed(ACTOR_SUSPECTED_LEADER, PIXELS(1), PIXELS(6) / 5);
  Actor_SetSpeed(ACTOR_SUSPECTED_THIEF, PIXELS(3) / 2, PIXELS(3) / 2);
  gVaultInteriors_StopTimer = 0;
  FieldScheduler_AddOrUpdateCallback(VaultInteriors_UpdateActorStops, 0xc94);
  GameFlag_Clear(0x1ff);
  Engine_EventEnd();
  Engine_AudioPlayCue(MUS_009);
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

struct SceneEntrance gVaultInteriors_Entrances[] = {
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

u32 gVaultInteriors_Exits[] = {
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

struct ScenePlacement gVaultInteriors_Placements[] = {
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
    { 0x3f, FLAG_VAULT_THIEVES_CONFRONTED, ACTOR_STAND,
        PIXELS(104), 0, PIXELS(696), FACING_EAST, 3, 0 },
    { 0x40, FLAG_VAULT_THIEVES_CONFRONTED, ACTOR_STAND,
        PIXELS(200), 0, PIXELS(680), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xdf, 0x0, ACTOR_STAND,
        PIXELS(692), 0, PIXELS(411), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gVaultInteriors_AtticPlacements[] = {
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

struct SceneEvent gVaultInteriors_Events[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 5 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 6 },
    { EVENT_EXIT, 3, CONDITION_ALWAYS, 7 },
    { EVENT_EXIT, 4, CONDITION_ALWAYS, 8 },
    { EVENT_EXIT, 5, CONDITION_ALWAYS, 9 },
    { EVENT_EXIT, 6, CONDITION_ALWAYS, 10 },
    { EVENT_TOUCH, 7, CONDITION_ALWAYS, EVENT_SCRIPT(Event_GoToEntrance11) },
    { EVENT_EXIT, 8, CONDITION_ALWAYS, 12 },
    { EVENT_TOUCH, 10, IF_FLAG_SET(FLAG_VAULT_IVAN_JOINED_ROD_SEARCH), EVENT_SCRIPT(Event_ApproachSuspects) },
    { EVENT_TOUCH, 11, IF_FLAG_SET(FLAG_SUSPECTS_FLEEING), EVENT_SCRIPT(Event_AdvanceSuspectRoutes) },
    { EVENT_TOUCH, 12, IF_FLAG_SET(FLAG_SUSPECTS_FLEEING), EVENT_SCRIPT(Event_AdvanceSuspectRoutes) },
    { EVENT_TOUCH, 13, IF_FLAG_SET(FLAG_SUSPECTS_FLEEING), EVENT_SCRIPT(Event_AdvanceSuspectRoutes) },
    { EVENT_TOUCH, 14, IF_FLAG_SET(FLAG_SUSPECTS_FLEEING), EVENT_SCRIPT(Event_AdvanceSuspectRoutes) },
    { EVENT_TOUCH, 15, IF_FLAG_SET(FLAG_SUSPECTS_FLEEING), EVENT_SCRIPT(Event_AdvanceSuspectRoutes) },
    { EVENT_TOUCH, 16, IF_FLAG_SET(FLAG_SUSPECTS_FLEEING), EVENT_SCRIPT(Event_AdvanceSuspectRoutes) },
    { EVENT_TOUCH, 17, IF_FLAG_SET(FLAG_SUSPECTS_FLEEING), EVENT_SCRIPT(Event_AdvanceSuspectRoutes) },
    { EVENT_TOUCH, 18, IF_FLAG_SET(FLAG_SUSPECTS_FLEEING), EVENT_SCRIPT(Event_AdvanceSuspectRoutes) },
    { EVENT_TOUCH, 20, IF_FLAG_SET(FLAG_SUSPECTS_FLEEING), EVENT_SCRIPT(Event_DiscussSuspiciousGuests) },
    { EVENT_TOUCH, 21, IF_FLAG_SET(FLAG_VAULT_SUSPECTS_APPROACHED), EVENT_SCRIPT(Event_BeginCorneringSuspects) },
    { EVENT_RAISED, 201, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReadCorneredSuspectsMinds) },
    { EVENT_RAISED, 202, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReadCorneredSuspectsMinds) },
    { EVENT_RAISED, 203, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReadCorneredSuspectsMinds) },
    { EVENT_RAISED, 250, CONDITION_ALWAYS, EVENT_SCRIPT(Event_GaretRefusesToLeave) },
    { EVENT_TOUCH, 26, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ClimbToAttic) },
    { TOUCH_WALKING, 40, CONDITION_ALWAYS, EVENT_SCRIPT(Event_JumpAcrossRoofGap) },
    { EVENT_TALK, 2, IF_FLAG_SET(FLAG_SCENE_TEMP_50), 0 },
    { EVENT_TALK, 2, CONDITION_ALWAYS, EVENT_SCRIPT(Event_MeetIvan) },
    { EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgVaultThisRockWasBlownHereAll) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkAboutRocks) },
    { EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgVaultMyNeighborCreepsMeOutHe) },
    { EVENT_TALK, 11, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ChildAsksAboutTravel) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkAboutMissingThings) },
    { EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgVaultEverSinceTheEruptionGrandpaKeeps) },
    { EVENT_TALK, 14, FLAG_VAULT_IVAN_JOINED_ROD_SEARCH, MSGNO(MsgVaultICouldHelpIvanIfI) },
    { EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgVaultYoureHelpingIvanYouDontLook) },
    { EVENT_TALK, 15, FLAG_VAULT_IVAN_JOINED_ROD_SEARCH, EVENT_SCRIPT(Event_OfferIvansSecret) },
    { EVENT_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgVaultSinceYoureHelpingIvanIllTell) },
    { EVENT_TALK, 16, CONDITION_ALWAYS, EVENT_SCRIPT(Event_MayorAsksHelpForIvan) },
    { EVENT_TALK, 17, FLAG_VAULT_IVAN_JOINED_ROD_SEARCH, MSGNO(MsgVaultMyHusbandTheMayorCantBelieve) },
    { EVENT_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgVaultReallyYoureGoingToHelpIvan) },
    { EVENT_TALK, 18, CONDITION_ALWAYS, EVENT_SCRIPT(Event_WarnAboutThreeStrangers) },
    { EVENT_TALK, 19, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToWeaponMerchant) },
    { EVENT_TALK, 20, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToArmorMerchant) },
    { EVENT_TALK, 21, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToItemMerchant) },
    { EVENT_TALK, 22, CONDITION_ALWAYS, MSGNO(MsgVaultTheGuestsUpstairsWerentInTheir) },
    { EVENT_TALK, 23, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToInnkeeper) },
    { EVENT_TALK, 24, IF_FLAG_SET(FLAG_SCENE_TEMP_50), 0 },
    { EVENT_TALK, 25, IF_FLAG_SET(FLAG_SCENE_TEMP_50), 0 },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 24, IF_FLAG_SET(FLAG_SCENE_TEMP_50), 0 },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 25, IF_FLAG_SET(FLAG_SCENE_TEMP_50), 0 },
    { EVENT_TALK, 24, FLAG_VAULT_SUSPECTS_MIND_READ, MSGNO(MsgVaultThatKidIvanHeCreepsMe) },
    { EVENT_TALK, 25, FLAG_VAULT_SUSPECTS_MIND_READ, MSGNO(MsgVaultWeHaventDoneAnythingYoureA) },
    { EVENT_TALK, 24, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SuspectedLeaderDodges) },
    { EVENT_TALK, 25, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SuspectedThiefDodges) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgVaultTheOtherSparklingRocksAllCrumbled) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReadMindOnRock) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgVaultHeyMaybeMyNeighborIsThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReadChildMind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReadMindOnThieves) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgVaultYouNeverBoughtMeAnythingWorth) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgVaultTheseGuysLookReliableIGuess) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgVaultIWonderWhatSortOfThing) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReadMayorMind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgVaultMaybeTheyllFindOurStolenFamily) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReadBoneGiverMind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, MSGNO(MsgVaultAllOurBestWeaponsWereStolen) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS, MSGNO(MsgVaultIWonderIfTheyLeftUs) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS, MSGNO(MsgVaultHahhWereSureToSeeA) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS, MSGNO(MsgVaultItWouldBeAwfulIfOur) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 23, CONDITION_ALWAYS, MSGNO(MsgVaultThatVolcanoCostUsALot) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x418, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SuspectedLeaderDodges) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x419, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SuspectedThiefDodges) },
    { PSYNERGY_ON_ACTOR(0x8c), 26, 0x859, EVENT_SCRIPT(Event_CheckCrateMoved) },
    { SEARCH(SEARCH_UNNAMED), 41, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SearchEmptyBarrel) },
    { SEARCH(SEARCH_JAR), 100, FLAG_VAULT_INTERIORS_JAR_COINS, FIND_COINS(7) },
    { SEARCH(SEARCH_BARREL), 101, FLAG_VAULT_INTERIORS_BARREL_COINS, FIND_COINS(4) },
    { SEARCH(SEARCH_BARREL), 102, FLAG_VAULT_INTERIORS_BARREL_SLEEP_BOMB, FIND_ITEM(ITEM_SLEEP_BOMB) },
    { SEARCH(SEARCH_WOODEN_BOX), 103, FLAG_VAULT_INTERIORS_BOX_MINT, FIND_ITEM(ITEM_MINT) },
    { 0xc4f3, 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgVaultTheresABookEntitledHistoryOf)) },
    { SEARCH(SEARCH_BOOKCASE), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgVaultTheresABookEntitledGemsOf)) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gVaultInteriors_AtticEvents[] = {
    { EVENT_TOUCH, 27, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveAttic) },
    { EVENT_TALK, 8, CONDITION_ALWAYS, EVENT_SCRIPT(Event_FreeRooferAndConfrontThieves) },
    { EVENT_TALK, 15, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SearchEmptyChest) },
    { EVENT_TALK, 16, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SearchEmptyChest) },
    { EVENT_TALK, 17, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SearchEmptyChest) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgVaultAhhHelpMe) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gVaultInteriors_EventsAfterThieves[] = {
    { SEARCH(SEARCH_UNNAMED), 41, CONDITION_ALWAYS, EVENT_SCRIPT(Vault_FindBone) },
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 5 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 6 },
    { EVENT_EXIT, 3, CONDITION_ALWAYS, 7 },
    { EVENT_EXIT, 4, CONDITION_ALWAYS, 8 },
    { EVENT_EXIT, 5, CONDITION_ALWAYS, 9 },
    { EVENT_EXIT, 6, CONDITION_ALWAYS, 10 },
    { EVENT_EXIT, 7, CONDITION_ALWAYS, 11 },
    { EVENT_EXIT, 8, CONDITION_ALWAYS, 12 },
    { EVENT_TOUCH, 26, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ClimbToAttic) },
    { EVENT_TOUCH, 27, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveAttic) },
    { EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgVaultICanSleepWellNowThat) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkAboutRocks) },
    { EVENT_TALK, 10, CONDITION_ALWAYS, EVENT_SCRIPT(Event_AskWhetherThievesCameFromLunpa) },
    { EVENT_TALK, 11, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ChildWorriesAboutFather) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkAboutMissingThings) },
    { EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgVaultJustAsIThoughtNothingWas) },
    { EVENT_TALK, 14, CONDITION_ALWAYS, EVENT_SCRIPT(Event_AskAboutDefeatingThieves) },
    { EVENT_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgVaultIvanLearnedALotByTalking) },
    { EVENT_TALK, 16, CONDITION_ALWAYS, EVENT_SCRIPT(Event_MayorGivesWaterOfLife) },
    { EVENT_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgVaultYoureThePersonWhoGotOur) },
    { EVENT_TALK, 18, FLAG_SCENE_TEMP_50, EVENT_SCRIPT(Event_OfferBones) },
    { EVENT_TALK, 19, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToWeaponMerchant) },
    { EVENT_TALK, 20, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToArmorMerchant) },
    { EVENT_TALK, 21, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToItemMerchant) },
    { EVENT_TALK, 22, CONDITION_ALWAYS, MSGNO(MsgVaultImShockedThoseGuestsOfOurs) },
    { EVENT_TALK, 23, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToInnkeeper) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgVaultEverybodysAfterMyRockTheyWant) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReadMindOnRock) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgVaultTheReasonLunpaTurnedBadIs) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReadChildMind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReadMindOnThieves) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgVaultIWishIHadSomethingWorth) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgVaultWhenIGrowUpIWant) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgVaultIWishIHadWingsLike) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReadMayorMind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgVaultImSoRelievedNowThatWe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReadBoneGiverMind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, MSGNO(MsgVaultMyWeaponsWereFoundInThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS, MSGNO(MsgVaultItMustBeSnowingUpIn) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS, MSGNO(MsgVaultWhyAreThesePeopleSoAnxious) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS, MSGNO(MsgVaultWhoCouldHaveKnownTheyWere) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 23, CONDITION_ALWAYS, MSGNO(MsgVaultOfCourseIAlwaysSuspectedThey) },
    { SEARCH(SEARCH_JAR), 100, FLAG_VAULT_INTERIORS_JAR_COINS, FIND_COINS(7) },
    { SEARCH(SEARCH_BARREL), 101, FLAG_VAULT_INTERIORS_BARREL_COINS, FIND_COINS(4) },
    { SEARCH(SEARCH_BARREL), 102, FLAG_VAULT_INTERIORS_BARREL_SLEEP_BOMB, FIND_ITEM(ITEM_SLEEP_BOMB) },
    { SEARCH(SEARCH_WOODEN_BOX), 103, FLAG_VAULT_INTERIORS_BOX_MINT, FIND_ITEM(ITEM_MINT) },
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
