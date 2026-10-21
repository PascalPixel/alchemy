/// @file
/// Vault (SceneId_VaultInteriors): the insides of the town's houses,
/// shops and inn, and the inn's loft, as the party first finds them, while
/// Master Hammet's Shaman's Rod is missing. Every room is on one map, each an
/// entrance with a camera box of its own, and Vault's doors lead to the
/// entrances named for them (vault.c): the house of a man keeping a sparkling
/// rock the volcano threw (VAULT_INTERIORS_FROM_VAULT_EAST_SOUTHEAST), a
/// child's house (VAULT_INTERIORS_FROM_VAULT_SOUTHEAST), the inn
/// (VAULT_INTERIORS_FROM_VAULT_SOUTHWEST, with its guests' room upstairs at
/// VAULT_INTERIORS_INN_UPSTAIRS_FROM_INN), the item shop, an old couple's home
/// too (VAULT_INTERIORS_FROM_VAULT_EAST), the weapon and armor shop
/// (VAULT_INTERIORS_FROM_VAULT_NORTH_NORTHEAST), the mayor's house
/// (VAULT_INTERIORS_FROM_VAULT_NORTHWEST) and a room with a crate
/// (VAULT_INTERIORS_FROM_VAULT_WEST). Behind the crate a way climbs to the
/// inn's loft (VAULT_INTERIORS_LOFT_FROM_CRATE_ROOM and the two after the
/// battle), which has placements and events of its own. In story order:
///
/// - Ivan, Master Hammet's servant, waits at the mayor's. Talking to him plays
///   Event_MeetIvan: he reads Isaac's mind, admits to strange powers of his
///   own and learns from it that they are called Psynergy
///   (FLAG_VAULT_MET_IVAN), tells of Hammet's stolen rod and asks for help. A
///   yes has him exchange names, show Isaac and Garet Mind Read and join the
///   party (VaultInteriors_IvanJoinsRodSearch,
///   FLAG_VAULT_IVAN_JOINED_ROD_SEARCH).
/// - In the guests' room at the inn, the two guests the town suspects run
///   from Ivan (Event_ApproachSuspects, FLAG_SUSPECTS_FLEEING). Stepping
///   out, the party plans to corner one of them
///   (Event_DiscussSuspiciousGuests), and the chase begins
///   (Event_BeginCorneringSuspects): Ivan and the two suspects move along a
///   network of stops (gVaultInteriors_Stops), the suspects turning away from
///   the leader, and once one has been blocked for half a second,
///   VaultInteriors_UpdateActorStops raises event 201 to 203. Ivan then reads
///   the cornered suspect's mind (Event_ReadCorneredSuspectsMinds,
///   FLAG_VAULT_SUSPECTS_MIND_READ): they are the thieves, and they have
///   hidden the stolen goods somewhere in the inn. During the chase the menus
///   answer with Garet's refusal (FLAG_MENUS_RAISE_EVENT,
///   FIELD_EVENT_MENUS_RAISED).
/// - In the room with the crate, Move pushes the crate aside
///   (Event_CheckCrateMoved, FLAG_VAULT_CRATE_MOVED), and the way behind it
///   climbs to the loft (Event_ClimbToLoft). There the roofer lies tied up;
///   cutting him free brings the three thieves, who tell them Hammet has
///   fled to Lunpa; the roofer explains it is named after Lunpa, a noble
///   thief, and the thieves add that Donpa's son, Dodonpa, is evil. They ask to
///   be let go and attack (Event_FreeRooferAndConfrontThieves,
///   FLAG_VAULT_THIEVES_CONFRONTED, slot 5 of
///   ENCOUNTER_ZONE_SCRIPTED_VALE_VAULT). A loss returns to
///   VAULT_INTERIORS_LOFT_AFTER_LOSING_TO_THIEVES, where they gloat that the
///   party will stay as their hostages and warn them to keep quiet if they want
///   to help the roofer, and the party comes to in the Vault Sanctum
///   (VaultInteriors_LoseToThieves); coming back up replays the challenge
///   (VaultInteriors_ConfrontThievesAgain). A win returns to
///   VAULT_INTERIORS_LOFT_AFTER_THIEVES_DEFEATED (VaultInteriors_DefeatThieves,
///   then VaultInteriors_ReturnStolenGoods): the mayor has the thieves locked
///   up (FLAG_VAULT_THIEVES_JAILED), the chests give back his urn and the
///   Sanctum's gold statue, and another chest the Shaman's Rod, and word comes
///   that Hammet was seen heading for Lunpa, where Dodonpa must have kidnapped
///   him to extort a ransom from Kalay. Ivan, set on rescuing him, hands back
///   any Venus Djinni of Isaac's and leaves the party.
/// - After the thieves are jailed, the townspeople thank the party, the mayor
///   gives the promised Water of Life (FLAG_VAULT_WATER_OF_LIFE_RECEIVED),
///   and the girl at the inn offers leftover bones from dinner
///   (FLAG_VAULT_BONE_RECEIVED), with more Bones on the floor beside her
///   (Vault_FindBone) for the dog in town (vault.c).
///
/// The shops and the inn open across their counters when the leader faces
/// north; otherwise their keepers speak. A jar, two barrels and a wooden box
/// hold coins, a Sleep Bomb and a Mint, each found once, and two bookcases
/// can be read.

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

/// A cornering lasts half a second before the chase raises an event.
#define VAULT_INTERIORS_CORNERED_FRAMES 30

/// The chase adds the blocked-suspect mask (1 the leader, 2 the thief) to 200.
enum VaultInteriorsRaisedEvent {
  VAULT_INTERIORS_CORNERED_EVENT_BASE = 200,
  VAULT_INTERIORS_LEADER_CORNERED,
  VAULT_INTERIORS_THIEF_CORNERED,
  VAULT_INTERIORS_BOTH_CORNERED
};

/// The townspeople, named by what they say, numbering
/// gVaultInteriors_Placements' rows from ACTOR_FIRST_PLACED; Garet's and Ivan's
/// rows take their own slots.
enum {
  ACTOR_GEM_OWNER = 8, ///< a sparkling rock was blown here from the volcano
  ACTOR_GEM_OWNERS_WIFE = 9,
  /// Distrusts the man next door. "My neighbor creeps me out."
  ACTOR_WARY_NEIGHBOR = 10,
  /// Asks about travel. "Hey, mister! Is it fun to see strange new places?"
  ACTOR_CHILD = 11,
  /// Wonders what was stolen. "Is it just me, or am I missing something?"
  ACTOR_GRANDPA = 12,
  /// Says Grandpa keeps wondering if anything was stolen.
  ACTOR_GRANDPAS_WIFE = 13,
  /// "I could help Ivan if I were just a little older."
  ACTOR_WOULD_BE_HELPER = 14,
  /// Tells the party Ivan's secret once it helps him.
  ACTOR_SECRET_KEEPER = 15,
  ACTOR_MAYOR = 16, ///< Ivan's host; his wife calls him the mayor
  ACTOR_MAYORS_WIFE = 17,
  /// The girl at the inn, as the Bone's description calls her. "I have a lot
  /// of leftover bones from dinner."
  ACTOR_BONE_GIVER = 18,
  ACTOR_WEAPON_MERCHANT = 19,
  ACTOR_ARMOR_MERCHANT = 20,
  ACTOR_ITEM_MERCHANT = 21,
  /// Works at the inn. "The guests upstairs weren't in their room during the
  /// eruption."
  ACTOR_INN_STAFF = 22,
  ACTOR_INNKEEPER = 23,
  /// The two guests at the inn the town suspects: the thieves' leader and one
  /// of his men.
  ACTOR_SUSPECTED_LEADER = 24,
  ACTOR_SUSPECTED_THIEF = 25,
  ACTOR_CRATE = 26 ///< pushed aside to open a passage
};

/// Place flags of Vault's group (0x300 to 0x3ff, kept while the party stays in
/// Vault and its buildings).
enum {
  /// Set while the two suspects run from the party, until the party steps
  /// aside to talk it over.
  FLAG_SUSPECTS_FLEEING = 0x300
};

// Scene flags here:
// - FLAG_SCENE_TEMP_50: at the inn once the thieves are jailed, the leader
//   stands beside the Bone on the floor (Vault_UpdateBoneProximityFlag); in
//   the guests' room, set while the party corners the suspects, when talking
//   to Ivan or the suspects does nothing.

/// The inn's loft (VAULT_INTERIORS_LOFT_FROM_CRATE_ROOM and the two after the
/// battle), where the thieves hid the stolen goods, numbering
/// gVaultInteriors_LoftPlacements' rows from ACTOR_FIRST_PLACED.
enum {
  ACTOR_LOFT_ROOFER = 8, ///< tied up while fixing the roof
  ACTOR_LOFT_MAYOR = 9,
  ACTOR_THIEF_LEADER = 10,
  ACTOR_FIRST_THIEF = 11,
  ACTOR_SECOND_THIEF = 12,
  /// Comes up with the mayor and helps tie the thieves up. "What should we do
  /// with them?"
  ACTOR_LOFT_ESCORT = 13,
  /// Comes up with the mayor and opens the chests. "Mayor... Isn't that your
  /// family's urn?"
  ACTOR_LOFT_SEARCHER = 14,
  ACTOR_FIRST_CHEST = 15,
  ACTOR_SECOND_CHEST = 16,
  ACTOR_THIRD_CHEST = 17,
  ACTOR_FOUND_URN = 18,   ///< rises from the first chest: the mayor's urn
  ACTOR_FOUND_STATUE = 19 ///< rises from the second: the Sanctum's gold statue
};

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

const struct SceneEntrance *Scene_GetEntrances(void) {
  return gVaultInteriors_Entrances;
}

/// Returns the one exit region: the box around (648, 416) in the room with the
/// crate, which takes VAULT_INTERIORS_TO_VAULT_WEST back out to Vault.
const struct SceneRegion *Scene_GetRegions(void) {
  return gVaultInteriors_Regions;
}

const u32 *Scene_GetExits(void) { return gVaultInteriors_Exits; }

/// Places the townspeople, or, for the inn's loft
/// (VAULT_INTERIORS_LOFT_FROM_CRATE_ROOM and the two after the battle), the
/// roofer, the mayor, the thieves, their chests and the found treasures.
const struct ScenePlacement *Scene_GetPlacements(void) {
  struct ScenePlacement *table;

  switch (gGameState.entrance) {
  case VAULT_INTERIORS_LOFT_FROM_CRATE_ROOM:
  case VAULT_INTERIORS_LOFT_AFTER_LOSING_TO_THIEVES:
  case VAULT_INTERIORS_LOFT_AFTER_THIEVES_DEFEATED:
    table = gVaultInteriors_LoftPlacements;
    break;
  default:
    table = gVaultInteriors_Placements;
    break;
  }
  FieldScene_PrepareActors(table);
  return table;
}

#if !TBS_PATCH_1
/// Jumps the leader to the requested destination when nothing blocks it there;
/// returns 1 when it jumped. Only the editions before the first patch have it;
/// the patched ones look ahead instead (Engine_LeaderCheckAhead).
static s32 VaultInteriors_TryJumpTo(struct FieldPosition *pos) {
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
    Engine_ActorSetSpriteFlags(leader, SPRITE_FLAG_SHADOW);
    Engine_TaskWait(6);
    *flags = savedMotionFlags;
    Engine_EventEnd();
    return 1;
  }
  return 0;
}
#endif

/// Indoors in Vault, facing north or south across the gap in the roof, the
/// leader jumps it.
///
/// Reached by walking onto cell code 40. Then copies the cell (41, 85) or
/// (43, 85) over the gap's cell (42, 85), whichever side the leader faced.
static void Event_JumpAcrossRoofGap(void) {
  s32 x;
  s32 z;
#if !TBS_PATCH_1
  struct FieldPosition target;
  struct FieldPosition *pos;
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

/// In Vault's room with the crate, once Move has pushed the crate aside to
/// column 42, the passage behind it, the way up to the inn's loft, opens.
///
/// Reached by Move on the crate (ACTOR_CRATE) until FLAG_VAULT_CRATE_MOVED is
/// set; sets it when the crate has reached column 42.
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

/// In Vault's weapon and armor shop, across the counter the weapon shop opens;
/// otherwise the merchant lost expensive goods too, or found the stolen
/// weapons rusted in the cemetery.
///
/// Reached by talking to the weapon merchant (ACTOR_WEAPON_MERCHANT), before
/// and after the thieves are jailed (FLAG_VAULT_THIEVES_JAILED picks the line).
/// The armor merchant, the inn and the item shop below work the same way: the
/// counter is between them when the leader faces north (0xa001 to 0xdfff).
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

/// In Vault's weapon and armor shop, across the counter the armor shop opens;
/// otherwise the merchant wants the thieves to pay, or advises preparing for
/// the cave in the Goma Range.
///
/// Reached by talking to the armor merchant (ACTOR_ARMOR_MERCHANT).
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

/// At Vault's inn, across the counter the inn opens; otherwise the innkeeper
/// defends the guests everyone suspects, or learns the goods were hidden in
/// his loft.
///
/// Reached by talking to the innkeeper (ACTOR_INNKEEPER).
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

#include "common/vault_bone.h"

/// At Vault's inn, while the party corners the suspects, Garet refuses to stop
/// for anything else. "We don't have time for that, Isaac!"
///
/// Reached through FIELD_EVENT_MENUS_RAISED, which the engine raises in place
/// of a menu or a shortcut while FLAG_MENUS_RAISE_EVENT is set.
static void Event_GaretRefusesToLeave(void) {
#if TBS_PATCH_1
  Leader_StopMoving();
#endif
  Engine_EventSetMessage(MsgVaultWeDontHaveTimeFor);
  Engine_EventShowMessage(GARET, 0);
}

/// At Vault's inn, Isaac checks the barrel but doesn't find anything.
///
/// Reached by searching cell code 41 before the thieves are jailed; after, the
/// same spot holds the Bone (Vault_FindBone).
static void Event_SearchEmptyBarrel(void) {
  Engine_EventBegin();
  Engine_MessageShowCentered(MsgVaultIsaacCheckedBarrel, 1);
  Engine_MessageShowCentered(MsgVaultButDidntFindAnything, 1);
  Engine_EventEnd();
}

/// In the inn's loft in Vault, Isaac checks the chest, but it is empty.
///
/// Reached by talking to one of the loft's three chests (ACTOR_FIRST_CHEST to
/// ACTOR_THIRD_CHEST).
static void Event_SearchEmptyChest(void) {
  Engine_EventBegin();
  Engine_MessageShowCentered(MsgVaultIsaacCheckedChest, 1);
  Engine_MessageShowCentered(MsgVaultButChestWasEmpty, 1);
  Engine_EventEnd();
}

/// Returns the loft's events for the loft's three entrances, else the houses'
/// events, which change once FLAG_VAULT_THIEVES_JAILED is set.
const struct SceneEvent *Scene_GetEvents(void) {
  switch (gGameState.entrance) {
  case VAULT_INTERIORS_LOFT_FROM_CRATE_ROOM:
  case VAULT_INTERIORS_LOFT_AFTER_LOSING_TO_THIEVES:
  case VAULT_INTERIORS_LOFT_AFTER_THIEVES_DEFEATED:
    return gVaultInteriors_LoftEvents;
  }
  if (Engine_GameFlagIsSet(FLAG_VAULT_THIEVES_JAILED) != 0) {
    return gVaultInteriors_EventsAfterThieves;
  }
  return gVaultInteriors_Events;
}

/// Stands the speaker, faces the leader, and shows the selected message.
static void VaultInteriors_StandAndShowMessage(s32 x) {
  Engine_ActorSetAnimation(x, ANIM_STAND);
  VaultInteriors_FaceEachOtherAndWait(x, 0, 2);
  Engine_EventShowMessage(x, 0);
}

#include "common/vault_talk.h"

/// Faces the leader and selects the next reply when the answer is no.
static void VaultInteriors_AskYesNoAndShowReply(s32 x) {
  VaultInteriors_FaceEachOtherAndWait(x, 0, 2);
  Engine_EventOpenMessage(x, 0);
  if (Engine_EventChooseYesNo(0, 0) != 0) {
    gEventWork->message++;
  }
  Engine_EventShowMessage(x, 0);
}

/// In a house of Vault, the child asks about travel.
/// "Hey, mister! Is it fun to see strange new places?"
///
/// Reached by talking to the child (ACTOR_CHILD) before the thieves are jailed.
static void Event_ChildAsksAboutTravel(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgVaultMisterFunSeeStrangeNew);
  Engine_ActorSetAnimation(ACTOR_CHILD, ANIM_STAND);
  VaultInteriors_AskYesNoAndShowReply(ACTOR_CHILD);
  Engine_EventEnd();
}

/// At the mayor's in Vault, a villager offers to tell Ivan's secret.
/// "Ivan has great powers. Wouldn't you like to know what they are?"
///
/// Reached by talking to ACTOR_SECRET_KEEPER until
/// FLAG_VAULT_IVAN_JOINED_ROD_SEARCH is set; then the row after it tells the
/// secret: Ivan can talk with animals.
static void Event_OfferIvansSecret(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgVaultIvanHasGreatPowersWouldnt);
  VaultInteriors_AskYesNoAndShowReply(ACTOR_SECRET_KEEPER);
  Actor_FaceDirection(ACTOR_SECRET_KEEPER, FACING_WEST, 0);
  Engine_EventEnd();
}

/// In Vault's item shop, across the counter the shop opens; otherwise a
/// greeting, or, after the thieves are jailed, the bridge is out.
/// "Did you just arrive in town today?"
///
/// Reached by talking to the item merchant (ACTOR_ITEM_MERCHANT).
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

/// In a house of Vault, the wife of the man with the sparkling rock wishes
/// their rocks were gems, or, after the thieves are jailed, is glad they were
/// caught before stealing the gem.
///
/// Reached by talking to the gem owner's wife (ACTOR_GEM_OWNERS_WIFE).
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

/// In Vault's item shop, Grandpa talks about missing things, or, after the
/// thieves are jailed, says they hid the treasure in the loft and more in the
/// cemetery.
/// "Is it just me, or am I missing something?"
///
/// Reached by talking to Grandpa (ACTOR_GRANDPA).
static void Event_TalkAboutMissingThings(void) {
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(FLAG_VAULT_THIEVES_JAILED) != 0) {
    Engine_EventSetMessage(MsgVaultThievesHidStolenTreasureIn);
  } else {
    Engine_EventSetMessage(MsgVaultJustMeOrAmMissing);
  }
  VaultInteriors_StandAndShowMessage(ACTOR_GRANDPA);
  Engine_EventEnd();
}

/// At his house in Vault, the mayor asks for someone to help Ivan; once the
/// party has agreed, he thanks them the first time and asks them to look after
/// Ivan after that.
///
/// Reached by talking to the mayor (ACTOR_MAYOR) before the thieves are jailed.
/// Sets FLAG_VAULT_MAYOR_THANKED_PARTY with the thanks.
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

/// At Vault's inn, the girl at the inn says the three strangers have fooled
/// the innkeeper and his wife.
///
/// Reached by talking to the bone giver (ACTOR_BONE_GIVER) before the thieves
/// are jailed.
static void Event_WarnAboutThreeStrangers(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgVaultThoseThreeStrangersSureHave);
  VaultInteriors_StandAndShowMessage(ACTOR_BONE_GIVER);
  Engine_EventEnd();
}

/// In the guests' room at Vault's inn, the suspected leader cries out and runs
/// to his next spot, chosen by the way he faces and where he is on his route.
/// "Ow! Stop it!"
///
/// Reached by Mind Read on him (ACTOR_SUSPECTED_LEADER), and by talking to him
/// once FLAG_VAULT_SUSPECTS_MIND_READ is set (before, he only says Ivan creeps
/// him out); neither while the party corners him. His timer holds his place on
/// the six-step route, wrapping from 5 to 0.
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

/// Selects a response script and advances the second suspect's route state.
static inline void
VaultInteriors_ApplySecondSuspectRoute(struct FieldActor *actor, s32 scriptRow,
                                       s32 routePair, u16 routeDelta) {
  Actor_SetActionScript(
      ACTOR_SUSPECTED_THIEF,
      gVaultInteriors_ResponseScripts[scriptRow * 4 + actor->timer]);
  actor->timer = actor->timer - routePair * 2 + routeDelta;
}

/// In the guests' room at Vault's inn, the suspected thief cries out and runs
/// to his next spot. "No! Leave me alone!"
///
/// Reached as Event_SuspectedLeaderDodges is, on ACTOR_SUSPECTED_THIEF. His
/// timer holds his place on his route (4 at its start).
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

/// In a house of Vault after the thieves are jailed, the wary neighbor asks
/// whether they came from Lunpa. "Did those thieves come from Lunpa?"
///
/// Reached by talking to the wary neighbor (ACTOR_WARY_NEIGHBOR) after the
/// thieves are jailed; the child and ACTOR_WOULD_BE_HELPER below are reached
/// the same way.
static void Event_AskWhetherThievesCameFromLunpa(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgVaultDidThoseThievesComeFrom);
  VaultInteriors_AskYesNoAndShowReply(ACTOR_WARY_NEIGHBOR);
  Engine_EventEnd();
}

/// In a house of Vault after the thieves are jailed, the child says Father
/// worries about the prisoners from Lunpa.
static void Event_ChildWorriesAboutFather(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgVaultMyFatherWorriedAboutThose);
  VaultInteriors_StandAndShowMessage(ACTOR_CHILD);
  Engine_EventEnd();
}

/// At the mayor's in Vault after the thieves are jailed, a villager asks about
/// defeating them. "I heard you defeated those thieves!"
static void Event_AskAboutDefeatingThieves(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgVaultHeardDefeatedThoseThieves);
  VaultInteriors_AskYesNoAndShowReply(ACTOR_WOULD_BE_HELPER);
  Engine_EventEnd();
}

/// At his house in Vault after the thieves are jailed, the mayor thanks Isaac
/// and gives the promised Water of Life, if there is room; then asks whether
/// the party is heading beyond the Goma Range, and points them to the cave on
/// the way to Bilibin.
///
/// Reached by talking to the mayor (ACTOR_MAYOR) after the thieves are jailed.
/// Gives the Water of Life and sets FLAG_VAULT_WATER_OF_LIFE_RECEIVED, once;
/// with a full bag he says to come back for it.
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
    VaultInteriors_FaceEachOtherAndWait(ACTOR_PARTY_LEADER, ACTOR_MAYOR, 20);
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

/// At Vault's inn after the thieves are jailed, the girl at the inn offers
/// leftover bones from dinner; a yes gets a Bone if there is room, a no a nod.
///
/// Reached by talking to the bone giver (ACTOR_BONE_GIVER) after the thieves
/// are jailed, while the leader is not beside the Bone on the floor
/// (FLAG_SCENE_TEMP_50 clear). Gives a Bone and sets FLAG_VAULT_BONE_RECEIVED.
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

/// In a house of Vault, Mind Read on the wife of the man with the sparkling
/// rock: let me see that rock, or maybe it is worthless.
///
/// Reached by Mind Read on the gem owner's wife (ACTOR_GEM_OWNERS_WIFE); the
/// Mind Read events below are reached the same way on the actor each names,
/// before and after the thieves are jailed (FLAG_VAULT_THIEVES_JAILED picks the
/// thought).
static void Event_ReadMindOnRock(void) {
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(FLAG_VAULT_THIEVES_JAILED) == 0) {
    Engine_EventSetMessage(MsgVaultOkMisterLetMeSee);
  } else {
    Engine_EventSetMessage(MsgVaultIfRockWorthlessMaybeThats);
  }
  Vault_ShowThought(ACTOR_GEM_OWNERS_WIFE);
  Engine_EventEnd();
}

/// In a house of Vault, Mind Read on the child: what is the outside world
/// like, or Father looks sad.
static void Event_ReadChildMind(void) {
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(FLAG_VAULT_THIEVES_JAILED) == 0) {
    Engine_EventSetMessage(MsgVaultWonderOutsideWorldLike);
  } else {
    Engine_EventSetMessage(MsgVaultFatherLooksSadWorryingLike);
  }
  Vault_ShowThought(ACTOR_CHILD);
  Engine_EventEnd();
}

/// In Vault's item shop, Mind Read on Grandpa: could they be thieves? Who can
/// be trusted? Or nothing in the house was worth stealing.
static void Event_ReadMindOnThieves(void) {
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(FLAG_VAULT_THIEVES_JAILED) == 0) {
    Engine_EventSetMessage(MsgVaultCouldTheyThievesGoodDont);
  } else {
    Engine_EventSetMessage(MsgVaultGuessNothingInOurHouse);
  }
  Vault_ShowThought(ACTOR_GRANDPA);
  Engine_EventEnd();
}

/// At the mayor's in Vault, Mind Read on the mayor: a laugh, or the cave in
/// the Goma Range is dangerous, but the party will be okay. "That tickles..."
static void Event_ReadMayorMind(void) {
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(FLAG_VAULT_THIEVES_JAILED) == 0) {
    Engine_EventSetMessage(MsgVaultTicklesBeingTickledByBoy);
  } else {
    Engine_EventSetMessage(MsgVaultCaveInGomaRangeDangerous);
  }
  Vault_ShowThought(ACTOR_MAYOR);
  Engine_EventEnd();
}

/// At Vault's inn, Mind Read on the girl at the inn: the master and his wife
/// are blinded by money; after the thieves are jailed, hungry while cooking,
/// or, once a Bone was given, wondering about calcium.
static void Event_ReadBoneGiverMind(void) {
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(FLAG_VAULT_THIEVES_JAILED) == 0) {
    Engine_EventSetMessage(MsgVaultMasterHisWifeBlindedBy);
  } else if (Engine_GameFlagIsSet(FLAG_VAULT_BONE_RECEIVED) == 0) {
    Engine_EventSetMessage(MsgVaultGeeAlwaysGetHungryWhen);
  } else {
    Engine_EventSetMessage(MsgVaultHeReallyLikesBonesWonder);
  }
  Vault_ShowThought(ACTOR_BONE_GIVER);
  Engine_EventEnd();
}

/// At the mayor's in Vault, talking to Ivan: the first time, he reads Isaac's
/// mind, admits his strange powers and learns they are called Psynergy; then
/// he asks for help finding the rod. A yes has him join the party.
///
/// Reached by talking to Ivan, whose row takes his party slot (IVAN), except
/// while the party corners the suspects. Sets FLAG_VAULT_MET_IVAN the first
/// time, and on a yes plays VaultInteriors_IvanJoinsRodSearch, sets
/// FLAG_VAULT_IVAN_JOINED_ROD_SEARCH and has Ivan walk into the leader.
static void Event_MeetIvan(void) {

  struct FieldActor *actor;

  Engine_EventBegin();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(GARET, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(IVAN, PIXELS(4) / 5, PIXELS(2) / 5);
  // Isaac steps up in front of Ivan and Garet comes out beside him.
  Event_PlayCue(MUS_019);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 384, 408);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_SetPosition(GARET, PIXELS(384), PIXELS(408));
  Actor_WalkToAndWait(GARET, 368, 408);
  Actor_FaceDirection(GARET, FACING_NORTH + FACING_STEP, 20);
  // Introduce Ivan's ability only on the first meeting.
  if (GameFlag_IsSet(FLAG_VAULT_MET_IVAN) == 0) {
    GameFlag_Set(FLAG_VAULT_MET_IVAN);
    // Ivan reads Isaac's mind and admits his strange powers.
    VaultInteriors_BeginMindRead(IVAN, ACTOR_PARTY_LEADER);
    Engine_EventWait(40);
    VaultInteriors_EndMindRead();
    Engine_EventSetMessage(MsgVaultDoPossessStrangePowers);
    Event_PlayCue(MUS_060);
    Engine_EventWait(30);
    VaultInteriors_PlayAnimationAndWait(IVAN, ANIM_NOD, 30);
    VaultInteriors_ShowMessageAndWait(IVAN, 30);
    // The two start; he reads again: "You have them, too, don't you?"
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
    // A third reading: his power is called Psynergy; then they have so much
    // in common he can trust them.
    VaultInteriors_BeginMindRead(IVAN, ACTOR_PARTY_LEADER);
    Engine_EventWait(40);
    VaultInteriors_EndMindRead();
    Engine_ActorRunRepeatedMotion(IVAN, 1);
    VaultInteriors_ShowMessageAndWait(IVAN, 50);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    VaultInteriors_PlayAnimationAndWait(GARET, ANIM_NOD, 20);
    VaultInteriors_PlayAnimationAndWait(IVAN, ANIM_NOD, 20);
    VaultInteriors_ShowMessageAndWait(IVAN, 40);
    // He walks off a step, turns back and tells of Hammet's stolen rod.
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
    // "Would you please help me get my master's rod back?"
    Event_OpenMessage(IVAN, 0);
  } else {
    // Asked again: "Would you really? Would you help me find the rod?"
    Event_PlayCue(MUS_060);
    Engine_EventSetMessage(MsgVaultWouldReallyWouldHelpMe);
    Event_OpenMessage(IVAN, 0);
  }
  // Accepting the search lets Ivan merge into the leader's position; a no gets
  // his apology for asking.
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
  // Garet walks back into the leader and the earlier music returns.
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

/// Ivan thanks them, they exchange names, and he shows Isaac and Garet Mind
/// Read; he joins the party.
///
/// Called by Event_MeetIvan when the party agrees to help. Adds Ivan to the
/// party ("Ivan joined your party!").
static void VaultInteriors_IvanJoinsRodSearch(void) {
  s32 message;

  // Ivan jumps for joy: "Thank you. With your help, I believe we can recover
  // the rod." Then he asks their names.
  Engine_EventWait(20);
  Object_GetById(IVAN)->paused = 0;
  Engine_ActorJump(IVAN, 4, 0);
  Engine_EventWait(40);
  message = MsgVaultThankHelpBelieve;
  Engine_EventSetMessage(message);
  Engine_EventWait(20);
  VaultInteriors_ShowMessageAndWait(IVAN, 20);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | 1, 0);
  Engine_EventWait(50);
  VaultInteriors_ShowMessageAndWait(IVAN, 30);
  // He reads their names from Isaac's mind; Isaac backs away from him.
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
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(GARET,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Engine_EventWait(60);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  VaultInteriors_PlayAnimationAndWait(GARET, ANIM_NOD, 30);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | 1, 0);
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
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 2, 0);
  Engine_EventWait(60);
  // "Is my Mind Read so frightening?": either answer goes on the same way.
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | 2, 0);
  Engine_EventWait(60);
  Engine_EventOpenMessage(IVAN, 0);
  Engine_ActorFaceActor(ACTOR_PARTY_LEADER, IVAN, 0);
  Engine_ActorFaceActor(GARET, IVAN, 0);
  Engine_EventChooseYesNo(0, 0);
  Engine_EventWait(30);
  // Ivan and Isaac clasp hands, and Garet cries out that his mind is read.
  VaultInteriors_PlayAnimationAndWait(IVAN, ANIM_NOD, 10);
  Actor_WalkToAndWait(IVAN, 384, 408);
  Engine_EventWait(10);
  Engine_MessageShowCentered(message + 5, 1);
  Engine_EventSetMessage(message + 6);
  Engine_ActorFaceEachOther(IVAN, GARET, 0);
  VaultInteriors_FaceActorAndWait(ACTOR_PARTY_LEADER, GARET, 20);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 2, 0);
  Engine_EventWait(60);
  Engine_ActorRunRepeatedMotion(GARET, 1);
  Object_GetById(GARET)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(GARET, 352, 408);
  Engine_EventWait(1);
  Object_GetById(GARET)->steeringFlags |= ACTOR_FACES_MOTION;
  Engine_EventWait(10);
  // Garet backs off; Ivan asks whether Isaac could read Garet's mind, and the
  // two read it together. "Just as I thought."
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
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 1, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 1, 0);
  Engine_EventWait(60);
  // Sharing their Psynergy let Isaac read minds too; Garet protests that it
  // isn't fair, and Ivan agrees it is one-way.
  VaultInteriors_PlayAnimationAndWait(IVAN, ANIM_NOD, 20);
  Engine_EventShowMessage(IVAN, 0);
  Engine_ActorShowEmote(GARET, EMOTE_IN_FRONT | 3, 0);
  Engine_EventWait(60);
  VaultInteriors_FaceActorAndWait(GARET, ACTOR_PARTY_LEADER, 10);
  Engine_EventShowMessage(GARET, 0);
  Engine_ActorShowEmote(IVAN, EMOTE_IN_FRONT | 0, 0);
  Engine_EventWait(60);
  Engine_EventShowMessage(IVAN, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 2, 0);
  Engine_EventWait(60);
  // Ivan and Garet clasp hands, and Garet reads Isaac's mind.
  Engine_MessageShowCentered(message + 13, 1);
  Engine_EventSetMessage(message + 14);
  Engine_ActorFaceEachOther(IVAN, ACTOR_PARTY_LEADER, 0);
  Engine_ActorFaceActor(GARET, ACTOR_PARTY_LEADER, 0);
  Engine_ActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 0);
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
  // "Let's use Mind Read to find the thieves!" Garet worries people will
  // notice.
  VaultInteriors_PlayAnimationAndWait(GARET, ANIM_SHAKE_HEAD, 10);
  VaultInteriors_ShowMessageAndWait(GARET, 30);
  Engine_ActorFaceActor(IVAN, GARET, 0);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | 1, 0);
  Engine_EventWait(30);
  VaultInteriors_FaceEachOtherAndWait(ACTOR_PARTY_LEADER, GARET, 10);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  VaultInteriors_PlayAnimationAndWait(GARET, ANIM_NOD, 30);
  Engine_ActorFaceActor(ACTOR_PARTY_LEADER, IVAN, 0);
  Engine_ActorFaceActor(GARET, IVAN, 0);
  Engine_ActorFaceActor(IVAN, ACTOR_PARTY_LEADER, 0);
  // Only Adepts can tell when Ivan uses Psynergy; ordinary people see
  // nothing, and he promises not to read their minds again.
  Engine_ActorShowEmote(IVAN, EMOTE_IN_FRONT | 6, 0);
  Engine_EventWait(60);
  VaultInteriors_ShowMessageAndWait(IVAN, 10);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  VaultInteriors_PlayAnimationAndWait(GARET, ANIM_NOD, 20);
  VaultInteriors_PlayAnimationAndWait(IVAN, ANIM_SHAKE_HEAD, 20);
  VaultInteriors_ShowMessageAndWait(IVAN, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 1, 0);
  Engine_ActorShowEmote(GARET, EMOTE_IN_FRONT | 1, 0);
  Engine_EventWait(60);
  VaultInteriors_PlayAnimationAndWait(IVAN, ANIM_NOD, 10);
  VaultInteriors_ShowMessageAndWait(IVAN, 30);
  Engine_ActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 5, 0);
  Engine_EventWait(60);
  VaultInteriors_PlayAnimationAndWait(IVAN, ANIM_SHAKE_HEAD, 10);
  VaultInteriors_ShowMessageAndWait(IVAN, 30);
  VaultInteriors_PlayAnimationAndWait(IVAN, ANIM_NOD, 10);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  VaultInteriors_PlayAnimationAndWait(GARET, ANIM_NOD, 20);
  // "Ivan joined your party!", then he will read the townspeople's minds.
  Event_PrepareObjectAndApplyValue(IVAN, 1);
  Engine_EventSetMessage(message + 22);
  Engine_ActorRunRepeatedMotion(IVAN, 1);
  Engine_EventShowMessage(IVAN, 0);
}

/// In the inn's loft in Vault, Isaac cuts the roofer free; the three thieves
/// arrive, tell of Hammet in Lunpa and of Dodonpa, ask to be let go, and
/// attack.
///
/// Reached by talking to the tied-up roofer (ACTOR_LOFT_ROOFER) in the loft;
/// the loft's setup replays the thieves' part through
/// VaultInteriors_ConfrontThievesAgain once FLAG_VAULT_THIEVES_CONFRONTED is
/// set. Sets that flag and starts the battle (encounter 12, 5): a win returns
/// to VAULT_INTERIORS_LOFT_AFTER_THIEVES_DEFEATED, a loss to
/// VAULT_INTERIORS_LOFT_AFTER_LOSING_TO_THIEVES.
static void Event_FreeRooferAndConfrontThieves(void) {
  s32 base;

  Engine_EventBegin();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(GARET, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(IVAN, PIXELS(4) / 5, PIXELS(2) / 5);
  // Isaac cuts the roofer free from the ropes; he gets up and nods.
  Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 3);
  Engine_EventWait(20);
  base = MsgVaultVaultCutFree;
  Engine_MessageShowCentered(base, 1);
  base++;
  Engine_EventSetMessage(base);
  Engine_EventWait(30);
  Engine_ActorSetAnimation(ACTOR_LOFT_ROOFER, ANIM_STAND);
  Engine_EventWait(60);
  VaultInteriors_PlayAnimationAndWait(ACTOR_LOFT_ROOFER, ANIM_NOD, 40);
  // Garet and Ivan come out of the leader around the roofer.
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 792, 440);
  VaultInteriors_FaceActorAndWait(ACTOR_PARTY_LEADER, ACTOR_LOFT_ROOFER, 20);
  Actor_SetPosition(GARET, PIXELS(792), PIXELS(440));
  Actor_SetPosition(IVAN, PIXELS(792), PIXELS(440));
  Actor_WalkTo(GARET, 808, 432);
  Actor_WalkTo(IVAN, 792, 456);
  Engine_ActorWaitForMove(GARET);
  Engine_ActorFaceActor(GARET, ACTOR_LOFT_ROOFER, 0);
  Engine_ActorWaitForMove(IVAN);
  VaultInteriors_FaceActorAndWait(IVAN, ACTOR_LOFT_ROOFER, 60);
  // The roofer tells how he came up to fix the roof, was whacked from behind
  // and woke up tied.
  Actor_ShowEmote(ACTOR_LOFT_ROOFER, EMOTE_IN_FRONT | 2, 0);
  Engine_EventWait(60);
  VaultInteriors_ShowMessageAndWait(ACTOR_LOFT_ROOFER, 20);
  Engine_ActorSetAnimation(IVAN, ANIM_NOD);
  VaultInteriors_PlayAnimationAndWait(GARET, ANIM_NOD, 30);
  Engine_ActorRunRepeatedMotion(ACTOR_LOFT_ROOFER, 2);
  Engine_EventWait(20);
  VaultInteriors_ShowMessageAndWait(ACTOR_LOFT_ROOFER, 20);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | 2, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 2, 0);
  Engine_EventWait(60);
  Actor_FaceDirection(ACTOR_LOFT_ROOFER, FACING_SOUTHEAST + FACING_STEP, 0);
  Engine_EventWait(10);
  VaultInteriors_ShowMessageAndWait(ACTOR_LOFT_ROOFER, 30);
  // He looks around at chests he doesn't remember; Ivan guesses the stolen
  // goods are in them.
  Actor_FaceDirection(ACTOR_LOFT_ROOFER, FACING_NORTH + FACING_STEP, 0);
  Engine_EventWait(30);
  Actor_ShowEmote(ACTOR_LOFT_ROOFER, EMOTE_IN_FRONT | 0, 0);
  Engine_EventWait(60);
  Actor_FaceDirection(ACTOR_LOFT_ROOFER, FACING_NORTHWEST + FACING_STEP, 0);
  Engine_EventWait(40);
  Actor_FaceDirection(ACTOR_LOFT_ROOFER, FACING_NORTH + FACING_STEP, 0);
  Engine_EventWait(40);
  VaultInteriors_FaceActorAndWait(ACTOR_LOFT_ROOFER, ACTOR_PARTY_LEADER, 20);
  VaultInteriors_PlayAnimationAndWait(ACTOR_LOFT_ROOFER, ANIM_SHAKE_HEAD, 30);
  VaultInteriors_ShowMessageAndWait(ACTOR_LOFT_ROOFER, 20);
  Engine_ActorRunRepeatedMotion(IVAN, 1);
  VaultInteriors_ShowMessageAndWait(IVAN, 40);
  // The thieves' leader appears at the ladder to MUS_061: "Looks like we've
  // been found out!" Everyone turns.
  Actor_SetPosition(ACTOR_THIEF_LEADER, PIXELS(744), PIXELS(408));
  Engine_AudioPlayCue(MUS_061);
  VaultInteriors_ShowMessageAndWait(ACTOR_THIEF_LEADER, 20);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  Engine_ActorStartRepeatedMotion(GARET, 1);
  Engine_ActorStartRepeatedMotion(IVAN, 1);
  Engine_ActorRunRepeatedMotion(ACTOR_LOFT_ROOFER, 1);
  Engine_EventWait(30);
  Engine_ActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_THIEF_LEADER, 0);
  Engine_ActorFaceActor(GARET, ACTOR_THIEF_LEADER, 0);
  Engine_ActorFaceActor(IVAN, ACTOR_THIEF_LEADER, 0);
  VaultInteriors_FaceActorAndWait(ACTOR_LOFT_ROOFER, ACTOR_THIEF_LEADER, 40);
  // He walks up and his two men run in behind him.
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
  Engine_ActorFaceActor(ACTOR_LOFT_ROOFER, ACTOR_THIEF_LEADER, 0);
  Actor_SetActionScript(ACTOR_FIRST_THIEF,
                        gVaultInteriors_FirstThiefEntryScript);
  Engine_EventWait(40);
  Actor_SetActionScript(ACTOR_SECOND_THIEF,
                        gVaultInteriors_SecondThiefEntryScript);
  Object_RefreshSelectorById(ACTOR_SECOND_THIEF);
  Actor_FaceDirection(ACTOR_FIRST_THIEF, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_SECOND_THIEF, FACING_SOUTHEAST, 0);
  Engine_EventWait(40);
  // The three taunt Hammet's whelp and say Hammet was caught by worse thieves
  // after fleeing to Lunpa.
  VaultInteriors_PlayAnimationAndWait(ACTOR_THIEF_LEADER, ANIM_SHAKE_HEAD, 20);
  VaultInteriors_ShowMessageAndWait(ACTOR_THIEF_LEADER, 20);
  Engine_ActorRunRepeatedMotion(ACTOR_FIRST_THIEF, 1);
  Engine_EventWait(10);
  VaultInteriors_ShowMessageAndWait(ACTOR_FIRST_THIEF, 20);
  Engine_ActorRunRepeatedMotion(ACTOR_SECOND_THIEF, 1);
  Engine_EventWait(10);
  VaultInteriors_ShowMessageAndWait(ACTOR_SECOND_THIEF, 40);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | 1, 0);
  Engine_EventWait(60);
  VaultInteriors_PlayAnimationAndWait(ACTOR_THIEF_LEADER, ANIM_SHAKE_HEAD, 20);
  VaultInteriors_ShowMessageAndWait(ACTOR_THIEF_LEADER, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 1, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 1, 0);
  Engine_EventWait(60);
  // The roofer is alarmed at the name Lunpa; Garet asks what kind of place it
  // is, and the roofer and the thieves tell of Lunpa, Donpa and Dodonpa.
  Actor_ShowEmote(ACTOR_LOFT_ROOFER, EMOTE_IN_FRONT | 0, 0);
  Engine_EventWait(40);
  Engine_ActorRunRepeatedMotion(ACTOR_LOFT_ROOFER, 2);
  Engine_EventWait(20);
  VaultInteriors_ShowMessageAndWait(ACTOR_LOFT_ROOFER, 20);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_LOFT_ROOFER, 0);
  Engine_ActorFaceActor(GARET, ACTOR_LOFT_ROOFER, 0);
  VaultInteriors_FaceActorAndWait(IVAN, ACTOR_LOFT_ROOFER, 20);
  VaultInteriors_PlayAnimationAndWait(ACTOR_LOFT_ROOFER, ANIM_SHAKE_HEAD, 30);
  VaultInteriors_ShowMessageAndWait(ACTOR_LOFT_ROOFER, 20);
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  Engine_EventWait(20);
  Engine_ActorFaceActor(ACTOR_PARTY_LEADER, IVAN, 0);
  VaultInteriors_FaceActorAndWait(GARET, IVAN, 30);
  VaultInteriors_ShowMessageAndWait(GARET, 20);
  Actor_ShowEmote(ACTOR_LOFT_ROOFER, EMOTE_IN_FRONT | 2, 0);
  Engine_EventWait(60);
  VaultInteriors_ShowMessageAndWait(ACTOR_LOFT_ROOFER, 20);
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
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 2, 0);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | 2, 0);
  Engine_EventWait(60);
  // "See, we're not so bad. How about it? Will you let us go?" Yes: if they
  // return the goods and repent? No: even returning everything is a crime?
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
    // Both answers go on with the same refusal.
    base -= 3;
    Engine_EventSetMessage(base);
  }
  // "We stole this stuff fair and square!" They agree to keep the party quiet,
  // and the leader cries "Get 'em!!!"
  Actor_ShowEmote(ACTOR_FIRST_THIEF, EMOTE_IN_FRONT | 3, 0);
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
  gEventWork->startTransition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
  base = (s32)&SceneId_VaultInteriors;
  Event_SetNextDestination(base, 17);
  Event_SetDefeatDestination(base, 16);
  Battle_SetStart(BATTLE_START_SCENE);
  Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED_VALE_VAULT, 5);
  Engine_EventEnd();
}

/// At Vault's inn, the party leaves the guests' room for the floor below
/// (VAULT_INTERIORS_INN_FROM_INN_UPSTAIRS).
///
/// Reached by stepping on cell code 7 before the thieves are jailed (after,
/// the same cell is a plain exit); takes VAULT_INTERIORS_INN_UPSTAIRS_TO_INN.
static void Event_LeaveGuestsRoom(void) {
  Engine_AudioPlayCue(SE_MAP_EXIT);
  Engine_EventRequestExit(VAULT_INTERIORS_INN_UPSTAIRS_TO_INN);
}

/// In Vault's room with the crate, the party climbs up into the inn's loft by
/// the way behind the crate.
///
/// Reached by stepping on cell code 26; takes
/// VAULT_INTERIORS_CRATE_ROOM_TO_LOFT.
static void Event_ClimbToLoft(void) {
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1) / 2, PIXELS(1) / 4);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 728, 408);
  gEventWork->transitionFrames = 16;
  Event_PlayCue(SE_MAP_EXIT);
  Engine_EventRequestExit(VAULT_INTERIORS_CRATE_ROOM_TO_LOFT);
}

/// Leaving the inn's loft in Vault before the thieves are confronted, the
/// party hears the roofer groan. "Uhnn... Uhnn..."
///
/// Reached by stepping on cell code 27 in the loft; takes
/// VAULT_INTERIORS_LOFT_TO_CRATE_ROOM back down.
static void Event_LeaveLoft(void) {
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1) / 2, PIXELS(1) / 4);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 744, 408);
  if (GameFlag_IsSet(FLAG_VAULT_THIEVES_CONFRONTED) == 0) {
    Engine_EventBegin();
    Engine_EventSetMessage(MsgVaultUhnnUhnn);
    Event_ShowMessage(ACTOR_LOFT_ROOFER, 0);
    Engine_EventEnd();
  }
  gEventWork->transitionFrames = 16;
  Engine_AudioPlayCue(SE_MAP_EXIT);
  Engine_EventRequestExit(VAULT_INTERIORS_LOFT_TO_CRATE_ROOM);
}

/// In the guests' room at Vault's inn, the two suspects spot "that weird kid"
/// with the party and run from him. "What do you want?"
///
/// Reached by stepping on cell code 10 at the suspects' room while
/// FLAG_VAULT_IVAN_JOINED_ROD_SEARCH is set. Sets
/// FLAG_VAULT_SUSPECTS_APPROACHED and FLAG_SUSPECTS_FLEEING, starts their
/// routes and the callback that keeps them facing the leader.
/// XXX: the row has no condition that stops it after the first time; whether
/// it plays again depends on the leader's path to cell code 10.
static void Event_ApproachSuspects(void) {

  struct FieldActor *actor24;
  struct FieldActor *actor25;

  actor24 = Object_GetById(ACTOR_SUSPECTED_LEADER);
  actor25 = Object_GetById(ACTOR_SUSPECTED_THIEF);
  Engine_EventBegin();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(GARET, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(IVAN, PIXELS(4) / 5, PIXELS(2) / 5);
  // Isaac walks in; the two start and spot Ivan with him: "It looks like that
  // weird kid is with them!"
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 232, 696);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 200, 696);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_SUSPECTED_THIEF, EMOTE_IN_FRONT | 0, 0);
  Actor_ShowEmote(ACTOR_SUSPECTED_LEADER, EMOTE_IN_FRONT | 0, 0);
  Engine_EventWait(60);
  VaultInteriors_FaceActorAndWait(ACTOR_SUSPECTED_THIEF, ACTOR_PARTY_LEADER,
                                  10);
  Engine_ActorRunRepeatedMotion(ACTOR_SUSPECTED_LEADER, 2);
  Engine_EventWait(20);
  Engine_EventSetMessage(MsgVaultWant);
  VaultInteriors_ShowMessageAndWait(ACTOR_SUSPECTED_LEADER, 20);
  Actor_SetAttachedEffect(ACTOR_SUSPECTED_THIEF,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Engine_EventWait(60);
  VaultInteriors_ShowMessageAndWait(ACTOR_SUSPECTED_THIEF, 20);
  Engine_ActorRunRepeatedMotion(ACTOR_SUSPECTED_LEADER, 1);
  VaultInteriors_ShowMessageAndWait(ACTOR_SUSPECTED_LEADER, 30);
  // "Don't let him near me!" They run off to their routes' first spots.
  Actor_SetSpeed(ACTOR_SUSPECTED_LEADER, PIXELS(4), PIXELS(2));
  Actor_SetSpeed(ACTOR_SUSPECTED_THIEF, PIXELS(7) / 2, PIXELS(7) / 4);
  Actor_SetActionScript(ACTOR_SUSPECTED_THIEF, gVaultInteriors_PairScriptO);
  Actor_SetActionScript(ACTOR_SUSPECTED_LEADER, gVaultInteriors_PairScriptB);
  Object_RefreshSelectorById(ACTOR_SUSPECTED_LEADER);
  Map_CopyCellAttributes(14, 45, 3, 1, 14, 44);
  GameFlag_Set(FLAG_VAULT_SUSPECTS_APPROACHED);
  GameFlag_Set(FLAG_SUSPECTS_FLEEING);
  FieldScheduler_AddCallback(VaultInteriors_FaceSuspectsTowardLeader,
                             TASK_PRIORITY_SCENE);
  actor24->timer = 1;
  actor25->timer = 3;
  Engine_EventEnd();
}

/// Stepping out of the guests' room at Vault's inn, the party talks it over:
/// Ivan can't read their minds while they run; Garet tells Isaac to take the
/// lead and corner them.
///
/// Reached by stepping on cell code 20, at the room's door, while
/// FLAG_SUSPECTS_FLEEING is set. Clears it and stops the facing callback; the
/// suspects go back to their places.
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
  // Outside the room, Garet and Ivan step out of the leader.
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 248, 728);
  Actor_SetPosition(IVAN, PIXELS(248), PIXELS(728));
  Actor_SetPosition(GARET, PIXELS(248), PIXELS(728));
  Actor_WalkTo(IVAN, 264, 744);
  Actor_WalkToAndWait(GARET, 232, 744);
  Engine_ActorWaitForMove(IVAN);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Actor_FaceActor(GARET, ACTOR_PARTY_LEADER, 0);
  VaultInteriors_FaceActorAndWait(IVAN, ACTOR_PARTY_LEADER, 30);
  // Ivan: they're acting suspicious.
  Engine_ActorRunRepeatedMotion(IVAN, 1);
  base = MsgVaultTheyreActingSuspiciousSomethingsNot;
  Engine_EventSetMessage(base);
  Event_ShowMessage(IVAN, 0);
  Actor_FaceActor(ACTOR_PARTY_LEADER, IVAN, 0);
  VaultInteriors_FaceActorAndWait(GARET, IVAN, 20);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  VaultInteriors_PlayAnimationAndWait(GARET, ANIM_NOD, 20);
  VaultInteriors_FaceEachOtherAndWait(ACTOR_PARTY_LEADER, GARET, 10);
  // Garet: "But you can't use Mind Read if they keep running away, can you?"
  // Yes: Ivan asks how he can catch them; no: there must be a way.
  Event_OpenMessage(GARET, 0);
  if (Engine_EventChooseYesNo(0, 0) != 0) {
    gEventWork->message++;
  }
  VaultInteriors_ShowMessageAndWait(GARET, 30);
  // Garet's plan: the three of them surround one of the two.
  Engine_EventSetMessage((base + 4));
  Actor_FaceActor(ACTOR_PARTY_LEADER, IVAN, 0);
  VaultInteriors_FaceActorAndWait(GARET, IVAN, 50);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | 0, 0);
  Engine_EventWait(60);
  VaultInteriors_FaceEachOtherAndWait(ACTOR_PARTY_LEADER, GARET, 50);
  Actor_FaceActor(ACTOR_PARTY_LEADER, IVAN, 0);
  VaultInteriors_FaceActorAndWait(GARET, IVAN, 30);
  VaultInteriors_PlayAnimationAndWait(IVAN, ANIM_NOD, 10);
  VaultInteriors_ShowMessageAndWait(IVAN, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 2, 0);
  Engine_EventWait(60);
  VaultInteriors_PlayAnimationAndWait(IVAN, ANIM_NOD, 20);
  VaultInteriors_ShowMessageAndWait(IVAN, 30);
  // "Isaac, you take the lead." The two walk back into him, and the suspects
  // are put back at their table.
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

/// In the guests' room at Vault's inn, the party sets out to corner the two
/// suspects.
///
/// Reached by stepping on cell code 21 while FLAG_VAULT_SUSPECTS_APPROACHED is
/// set. Sets FLAG_MENUS_RAISE_EVENT, so the menus answer with Garet's refusal,
/// and FLAG_SCENE_TEMP_50, so talking to Ivan or the suspects does nothing;
/// then starts the chase (VaultInteriors_StartActorStops).
static void Event_BeginCorneringSuspects(void) {
  Engine_GameFlagSet(FLAG_MENUS_RAISE_EVENT);
  Engine_GameFlagSet(FLAG_SCENE_TEMP_50);
  VaultInteriors_StartActorStops();
}

/// Has the suspects react to the party's return and resets their route states.
/// "Hey, they're back!"
///
/// Called by VaultInteriors_StartActorStops.
static void VaultInteriors_ResetSuspectRoutes(void) {
  Engine_EventWait(30);
  Engine_ActorRunRepeatedMotion(ACTOR_SUSPECTED_LEADER, 1);
  Engine_EventWait(20);
  Engine_EventSetMessage(MsgVaultTheyreBack);
  VaultInteriors_ShowMessageAndWait(ACTOR_SUSPECTED_LEADER, 20);
  Actor_FaceDirection(ACTOR_SUSPECTED_THIEF, FACING_EAST, 20);
  Actor_SetAttachedEffect(ACTOR_SUSPECTED_THIEF,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
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

/// Sets the three party images to the priority used during the mind-reading
/// scene.
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

/// In the guests' room at Vault's inn, once a suspect is cornered, Garet calls
/// Ivan and Ivan reads their minds. "Uh-oh! I'm surrounded!" "Now, Ivan!"
///
/// Reached through raised events 201 to 203, which
/// VaultInteriors_UpdateActorStops raises after thirty frames with at least
/// one suspect blocked: 201 the leader, 202 the thief, 203 both. Stops the
/// chase, clears FLAG_MENUS_RAISE_EVENT and FLAG_SCENE_TEMP_50, plays the
/// party's talk (VaultInteriors_DiscussMindReading) and sets
/// FLAG_VAULT_SUSPECTS_MIND_READ.
static void Event_ReadCorneredSuspectsMinds(void) {
  s32 actor;
  struct EventWork *work;

  work = gEventWork;
  Engine_EventBegin();
  Scheduler_RemoveCallback(VaultInteriors_UpdateActorStops);
  GameFlag_Clear(FLAG_MENUS_RAISE_EVENT);
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
  actor = ACTOR_SUSPECTED_LEADER;
  // The event identifies the suspects blocked on the last frame of the wait.
  switch (work->raisedTrigger) {
  case VAULT_INTERIORS_THIEF_CORNERED:
  case VAULT_INTERIORS_BOTH_CORNERED:
    Engine_EventSetMessage(MsgVaultImSurrounded);
    Actor_SetAttachedEffect(ACTOR_SUSPECTED_THIEF,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Engine_ActorRunRepeatedMotion(ACTOR_SUSPECTED_THIEF, 2);
    VaultInteriors_ShowMessageAndWait(ACTOR_SUSPECTED_THIEF, 20);
    if (work->raisedTrigger == VAULT_INTERIORS_THIEF_CORNERED) {
      actor = ACTOR_SUSPECTED_THIEF;
      break;
    }
    // When both are cornered, the leader's reaction follows the thief's.
    // fall through
  case VAULT_INTERIORS_LEADER_CORNERED:
    Engine_EventSetMessage(MsgVaultTheresNowhereRun);
    Actor_SetAttachedEffect(ACTOR_SUSPECTED_LEADER,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Engine_ActorRunRepeatedMotion(ACTOR_SUSPECTED_LEADER, 2);
    actor = ACTOR_SUSPECTED_LEADER;
    VaultInteriors_ShowMessageAndWait(ACTOR_SUSPECTED_LEADER, 20);
    break;
  }
  Engine_ActorSetAnimation(IVAN, ANIM_STAND);
  Actor_FaceActor(IVAN, actor, 0);
  Actor_FaceActor(GARET, IVAN, 0);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Engine_EventWait(20);
  // "Now, Ivan!" "A-All right!" Ivan reads the cornered one's mind while the
  // two wonder what he is doing.
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
  // "Are you done, Ivan?" He nods, and the window closes.
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | 0, 60);
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
  gEventWork->startTransition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  // Move the discussion away from the suspects while the screen is closed.
  VaultInteriors_DiscussMindReading();
  // The suspects stay at their table, turned toward the leader from now on.
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
  gEventWork->startTransition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  Engine_EventEnd();
}

/// Shows the party's branching conversation about what Ivan learned: the
/// stolen goods are hidden in the inn, which they must search.
///
/// Called by Event_ReadCorneredSuspectsMinds while the screen is closed; sets
/// the party outside the room and opens the screen on them.
static void VaultInteriors_DiscussMindReading(void) {
  s32 persistedWithQuestions;

  // Move the party outside the room and the suspects back to their table.
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
    work->startTransition =
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
  }
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(40);
  // "What did you learn, Ivan?" It is them: they hid the stolen goods
  // somewhere in this inn, but won't let him near again.
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
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 2, 0);
  Engine_EventWait(60);
  VaultInteriors_PlayAnimationAndWait(IVAN, ANIM_NOD, 20);
  VaultInteriors_ShowMessageAndWait(IVAN, 30);
  Engine_ActorRunRepeatedMotion(GARET, 1);
  Engine_EventWait(20);
  VaultInteriors_FaceEachOtherAndWait(ACTOR_PARTY_LEADER, GARET, 20);
  // Garet: they have to search the inn. Each no leads to another plea, from
  // Garet and then Ivan; three noes and Garet vouches for Isaac's sense of
  // duty, while any yes gets "Leave it to us!"
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
      Engine_ActorShowEmote(IVAN, EMOTE_IN_FRONT | 2, 0);
      Engine_EventWait(60);
      Engine_ActorRunRepeatedMotion(IVAN, 2);
      VaultInteriors_FaceEachOtherAndWait(ACTOR_PARTY_LEADER, IVAN, 20);
      Engine_EventOpenMessage(IVAN, 0);
      persistedWithQuestions = 0;
      if (Engine_EventChooseYesNo(2, 0) != 0) {
        Engine_EventWait(20);
        Engine_ActorShowEmote(IVAN, EMOTE_IN_FRONT | 5, 0);
        Engine_EventWait(60);
        VaultInteriors_ShowMessageAndWait(IVAN, 20);
        VaultInteriors_FaceActorAndWait(GARET, IVAN, 10);
        Engine_ActorRunRepeatedMotion(GARET, 1);
        Engine_EventWait(10);
        VaultInteriors_ShowMessageAndWait(GARET, 10);
        VaultInteriors_FaceActorAndWait(IVAN, GARET, 20);
        Engine_ActorShowEmote(IVAN, EMOTE_IN_FRONT | 1, 0);
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
  // "Let's go, Ivan." Garet and Ivan walk back into the leader.
  Engine_ActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 5, 0);
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

/// In the guests' room at Vault's inn, the leader stepping on one of the route
/// triggers 11 to 18 moves the fleeing suspects on to their next spots.
///
/// Reached by stepping on cell codes 11 to 18 while FLAG_SUSPECTS_FLEEING is
/// set.
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

/// Installs a route script only when the suspect is in its expected state.
static void VaultInteriors_AdvanceSuspectRoute(s32 actor, s32 expected,
                                               s32 next, const s32 *actions) {
  struct FieldActor *suspect = Object_GetById(actor);

  if (suspect->timer == expected) {
    Actor_SetActionScript(actor, actions);
    suspect->timer = next;
  }
}

#include "common/vault_spotflag.h"

/// Arriving in any room of Vault's houses, shops and inn: restores the room
/// for the story so far, and plays the loft's scenes: the tied-up roofer, the
/// rematch, and the defeat or victory.
///
/// Called by the engine once the actors are placed, by entrance. At the inn,
/// after the thieves are jailed, places the Bone and starts
/// Vault_UpdateBoneProximityFlag. After a lost battle
/// (VAULT_INTERIORS_LOFT_AFTER_LOSING_TO_THIEVES) takes
/// VAULT_INTERIORS_TO_SANCTUM; after a won one (17) sets
/// FLAG_VAULT_THIEVES_JAILED through VaultInteriors_ReturnStolenGoods, which
/// also removes Ivan from the party.
s32 Scene_Initialize(void) {
  gEventWork->startTransition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  switch (gGameState.entrance) {
  case VAULT_INTERIORS_FROM_VAULT_EAST_SOUTHEAST: {
    s32 zero = 0;

    // The gem owner's house: he stands on the floor.
    Object_GetById(ACTOR_GEM_OWNER)->motionFlags = zero;
    Object_GetById(ACTOR_GEM_OWNER)->y.fixed = zero;
    Object_GetById(ACTOR_GEM_OWNER)->terrainHeight = zero;
  } break;
  case VAULT_INTERIORS_FROM_VAULT_NORTHWEST:
    // The mayor's house: Ivan waits a step back once met and is gone once he
    // has joined; after the thieves are jailed, the mayor stands by the door.
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
  case VAULT_INTERIORS_FROM_VAULT_SOUTHWEST:
  case VAULT_INTERIORS_INN_FROM_INN_UPSTAIRS:
    // The inn: once the thieves are jailed, the Bone lies on the floor beside
    // the girl at the inn; after the mind reading and before that, she is
    // out.
    if (Engine_GameFlagIsSet(FLAG_VAULT_THIEVES_JAILED) != 0) {
      Actor_SetPosition(ACTOR_BONE_GIVER, PIXELS(568), PIXELS(648));
      Actor_SetBuiltinAction(ACTOR_BONE_GIVER, ACTOR_ACTION_IDLE);
      Actor_FaceDirection(ACTOR_BONE_GIVER, FACING_SOUTH, 0);
      Vault_PlaceItemIcon(ITEM_BONE, PIXELS(568), PIXELS(16), PIXELS(672));
      Task_AddCallback(Vault_UpdateBoneProximityFlag, TASK_PRIORITY_SCENE);
    } else if (Engine_GameFlagIsSet(FLAG_VAULT_SUSPECTS_MIND_READ) != 0) {
      Engine_ActorSetPosition(ACTOR_BONE_GIVER, 0, 0);
    }
    break;
  case VAULT_INTERIORS_INN_UPSTAIRS_FROM_INN:
    // The guests' room upstairs at the inn: back from a battle mid-chase, the
    // suspects keep running.
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
    // Once approached, the room's doorway collision changes; after the mind
    // reading the suspects sit turned toward the leader.
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
  case VAULT_INTERIORS_FROM_VAULT_WEST:
  case VAULT_INTERIORS_CRATE_ROOM_FROM_LOFT:
    // The room with the crate: the crate keeps its place once moved, and the
    // passage behind it stays open.
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
  case VAULT_INTERIORS_LOFT_FROM_CRATE_ROOM:
    Engine_EventWait(2);
    Map_CopyCellsTo(54, 2, 44, 21, 2, 8);
    Map_CopyCellsTo(54, 2, 44, 81, 2, 8);
    Engine_EventBegin();
    // The loft: emptied chests after the thieves are jailed; else the roofer
    // lies tied, groaning to MUS_017, or the thieves wait for a rematch.
    if (Engine_GameFlagIsSet(FLAG_VAULT_THIEVES_JAILED) != 0) {
      Engine_ActorSetAnimation(ACTOR_FIRST_CHEST, ANIM_WALK);
      Engine_ActorSetAnimation(ACTOR_SECOND_CHEST, ANIM_WALK);
      Engine_EventWait(1);
      Engine_ActorSetAnimation(ACTOR_THIRD_CHEST, ANIM_WALK);
      break;
    }
    Engine_ActorSetPosition(ACTOR_LOFT_ROOFER, PIXELS(824), PIXELS(456));
    if (Engine_GameFlagIsSet(FLAG_VAULT_THIEVES_CONFRONTED) != 0) {
      VaultInteriors_ConfrontThievesAgain();
      Engine_EventEnd();
      break;
    }
    Engine_ActorSetAnimation(ACTOR_LOFT_ROOFER, 7);
    VaultInteriors_OpenWindowTransition();
    Engine_AudioPlayCue(MUS_017);
    Engine_EventSetMessage(MsgVaultUhnnUhnn);
    VaultInteriors_ShowMessageAndWait(ACTOR_LOFT_ROOFER, 10);
    Engine_EventEnd();
    break;
  case VAULT_INTERIORS_LOFT_AFTER_LOSING_TO_THIEVES:
    // Lost to the thieves: they gloat, and the party wakes up elsewhere.
    Map_CopyCells(54, 2, 2, 8, 44, 21);
    Map_CopyCells(54, 2, 2, 8, 44, 81);
    Engine_ActorSetPosition(ACTOR_LOFT_ROOFER, PIXELS(824), PIXELS(456));
    VaultInteriors_LoseToThieves();
    Engine_EventRequestExit(VAULT_INTERIORS_TO_SANCTUM);
    break;
  case VAULT_INTERIORS_LOFT_AFTER_THIEVES_DEFEATED:
    // Won: the thieves are caught and the stolen goods handed back.
    Map_CopyCells(54, 2, 2, 8, 44, 21);
    Map_CopyCells(54, 2, 2, 8, 44, 81);
    if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) == 0) {
      Actor_SetPosition(ACTOR_LOFT_ROOFER, PIXELS(824), PIXELS(456));
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

/// Keeps both suspects facing the leader while they evade the party.
///
/// Run each frame as a scene callback, from Event_ApproachSuspects until
/// Event_DiscussSuspiciousGuests, and again once their minds are read.
static void VaultInteriors_FaceSuspectsTowardLeader(void) {
  struct FieldActor *origin = Object_GetById(ACTOR_PARTY_LEADER);
  struct FieldActor *first = Object_GetById(ACTOR_SUSPECTED_LEADER);
  struct FieldActor *second = Object_GetById(ACTOR_SUSPECTED_THIEF);

  first->facing = ArcTan2(origin->z.fixed - first->z.fixed,
                          origin->x.fixed - first->x.fixed);
  second->facing = ArcTan2(origin->z.fixed - second->z.fixed,
                           origin->x.fixed - second->x.fixed);
}

/// Shows the thieves' reaction after the party loses their battle: the party
/// lies beaten, and the thieves hold it and the roofer as hostages.
///
/// Called by Scene_Initialize at VAULT_INTERIORS_LOFT_AFTER_LOSING_TO_THIEVES,
/// the battle's defeat destination; the scene then takes
/// VAULT_INTERIORS_TO_SANCTUM to the Vault Sanctum (SceneId_Sanctum,
/// SANCTUM_FROM_VAULT_INTERIORS).
static void VaultInteriors_LoseToThieves(void) {
  struct FieldActor *actor;
  s32 priorityFlags;

  // The party lies on the floor (animation 19) before the three thieves.
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
  Actor_FaceDirection(ACTOR_LOFT_ROOFER, FACING_NORTHWEST + FACING_STEP, 0);
  gEventWork->startTransition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 0);
  Engine_CameraWaitForMove();
  Engine_MapRedraw();
  VaultInteriors_OpenWindowTransition();
  Engine_EventWait(60);
  // "These kids are nothing to worry about." The roofer groans.
  VaultInteriors_PlayAnimationAndWait(ACTOR_THIEF_LEADER, ANIM_NOD, 20);
  Engine_EventSetMessage(MsgVaultTheseKidsNothingWorryAbout);
  VaultInteriors_ShowMessageAndWait(ACTOR_THIEF_LEADER, 30);
  VaultInteriors_ShowMessageAndWait(ACTOR_LOFT_ROOFER, 30);
  // The two men step to the roofer: he stays as their hostage.
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

/// Arranges the loft confrontation and restarts the thieves' battle. "They
/// won't go away, will they? They're back!"
///
/// Called by Scene_Initialize at VAULT_INTERIORS_LOFT_FROM_CRATE_ROOM once
/// FLAG_VAULT_THIEVES_CONFRONTED is set. Starts the same battle as
/// Event_FreeRooferAndConfrontThieves.
static void VaultInteriors_ConfrontThievesAgain(void) {

  // The thieves stand over the roofer as the party comes up.
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
  Actor_FaceActor(ACTOR_LOFT_ROOFER, ACTOR_THIEF_LEADER, 0);
  gEventWork->startTransition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
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
  // "We'll make you sorry you were ever born! Get 'em!!" and the battle.
  VaultInteriors_PlayAnimationAndWait(ACTOR_THIEF_LEADER, ANIM_SHAKE_HEAD, 20);
  Event_ShowMessage(ACTOR_THIEF_LEADER, 0);
  gEventWork->startTransition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
  Event_SetNextDestination((s32)&SceneId_VaultInteriors, 17);
  Event_SetDefeatDestination((s32)&SceneId_VaultInteriors, 16);
  Battle_SetStart(BATTLE_START_SCENE);
  Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED_VALE_VAULT, 5);
}

/// Counts the Venus Djinn Ivan holds, which he gives back before he leaves the
/// party.
static s32 VaultInteriors_GetIvanVenusDjinnCount(void) {
  return UnitGet(IVAN)->djinnOwnedCounts[ELEMENT_VENUS];
}

#include "common/vault_djinn.h"

/// Stages the defeated thieves and gathers the townspeople in the loft.
///
/// Called by Scene_Initialize at VAULT_INTERIORS_LOFT_AFTER_THIEVES_DEFEATED,
/// the battle's victory destination, unless the party is only coming back to
/// where it was; then VaultInteriors_ReturnStolenGoods follows.
static void VaultInteriors_DefeatThieves(void) {
  struct FieldActor *actor;
  struct EventWork *work;
  const s32 *actions;
  struct AnimationObject *thirdThiefSprite;

  actor = Object_GetById(ACTOR_SECOND_THIEF);
  thirdThiefSprite = actor->sprite;
  // The beaten thieves lie heaving (animation 9 and the action table's
  // squash) before the party.
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
  Actor_SetSpeed(ACTOR_LOFT_ROOFER, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_FaceDirection(ACTOR_LOFT_ROOFER, FACING_NORTHWEST + FACING_STEP, 0);
  gEventWork->startTransition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 0);
  Engine_CameraWaitForMove();
  Engine_MapRedraw();
  VaultInteriors_OpenWindowTransition();
  Actor_SetActionScript(ACTOR_FIRST_THIEF, actions);
  Engine_EventWait(30);
  Actor_SetActionScript(ACTOR_SECOND_THIEF, actions);
  Engine_EventWait(30);
  // "They... They got us..." The roofer thought they were goners; he slips
  // past Garet, cheers the win and runs for the mayor.
  Engine_EventSetMessage(MsgVaultTheyTheyGotUs);
  VaultInteriors_ShowMessageAndWait(ACTOR_THIEF_LEADER, 20);
  Actor_ShowEmote(ACTOR_LOFT_ROOFER, EMOTE_IN_FRONT | 2, 0);
  Engine_EventWait(60);
  VaultInteriors_ShowMessageAndWait(ACTOR_LOFT_ROOFER, 20);
  Actor_WalkToAndWait(ACTOR_LOFT_ROOFER, 808, 456);
  Actor_SetSpeed(GARET, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(GARET, 792, 432);
  Actor_FaceDirection(GARET, FACING_EAST, 0);
  Actor_WalkToAndWait(ACTOR_LOFT_ROOFER, 808, 408);
  Actor_FaceDirection(ACTOR_LOFT_ROOFER, FACING_WEST, 0);
  Actor_WalkToAndWait(GARET, 808, 432);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Engine_EventWait(20);
  VaultInteriors_PlayAnimationAndWait(ACTOR_LOFT_ROOFER, ANIM_NOD, 20);
  VaultInteriors_ShowMessageAndWait(ACTOR_LOFT_ROOFER, 20);
  Actor_WalkToAndWait(ACTOR_LOFT_ROOFER, 768, 408);
  Engine_EventWait(20);
  Actor_FaceActor(ACTOR_LOFT_ROOFER, ACTOR_PARTY_LEADER, 0);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_LOFT_ROOFER, 0);
  Actor_FaceActor(GARET, ACTOR_LOFT_ROOFER, 0);
  VaultInteriors_FaceActorAndWait(IVAN, ACTOR_LOFT_ROOFER, 40);
  VaultInteriors_ShowMessageAndWait(ACTOR_LOFT_ROOFER, 30);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_ActorSetAnimation(GARET, ANIM_NOD);
  VaultInteriors_PlayAnimationAndWait(IVAN, ANIM_NOD, 20);
  Actor_WalkToAndWait(ACTOR_LOFT_ROOFER, 744, 408);
  Engine_EventWait(50);
  // The thieves grumble; Garet: stealing is wrong, evil never prevails.
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
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 3, 0);
  Engine_EventWait(60);
  VaultInteriors_ShowMessageAndWait(GARET, 30);
  // Ivan will take the rod to Hammet in Lunpa, and asks what the two will do:
  // go after Saturos and the others.
  VaultInteriors_FaceActorAndWait(IVAN, ACTOR_PARTY_LEADER, 30);
  VaultInteriors_FaceActorAndWait(ACTOR_PARTY_LEADER, IVAN, 30);
  VaultInteriors_PlayAnimationAndWait(IVAN, ANIM_NOD, 20);
  VaultInteriors_ShowMessageAndWait(IVAN, 20);
  VaultInteriors_FaceActorAndWait(GARET, IVAN, 30);
  VaultInteriors_PlayAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD, 30);
  VaultInteriors_ShowMessageAndWait(IVAN, 20);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 0, 0);
  Engine_EventWait(60);
  VaultInteriors_ShowMessageAndWait(GARET, 30);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | 1, 0);
  Engine_EventWait(60);
  VaultInteriors_FaceEachOtherAndWait(ACTOR_PARTY_LEADER, GARET, 20);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  VaultInteriors_PlayAnimationAndWait(GARET, ANIM_NOD, 20);
  // Ivan reads Isaac's mind. Garet: "Shouldn't we stop him?" Yes: too late,
  // he's done. No: "If you say so."
  VaultInteriors_BeginMindRead(IVAN, ACTOR_PARTY_LEADER);
  Ui_SetBank15PaletteAndClearRenderMode();
  Actor_FaceActor(ACTOR_PARTY_LEADER, IVAN, 0);
  VaultInteriors_FaceActorAndWait(GARET, IVAN, 10);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  Engine_ActorRunRepeatedMotion(GARET, 1);
  Engine_EventWait(20);
  Actor_SetAttachedEffect(GARET,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
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
    Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 2, 0);
    Engine_EventWait(60);
    VaultInteriors_ShowMessageAndWait(GARET, 20);
    Actor_FaceActor(ACTOR_PARTY_LEADER, IVAN, 0);
    VaultInteriors_FaceActorAndWait(GARET, IVAN, 20);
    VaultInteriors_EndMindRead();
    Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Engine_ActorRunRepeatedMotion(GARET, 1);
    Engine_EventWait(20);
  }
  // "I see... So that's what happened..." The roofer comes back with the
  // mayor and two townsmen.
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | 2, 0);
  Engine_EventWait(60);
  VaultInteriors_PlayAnimationAndWait(IVAN, ANIM_SHAKE_HEAD, 20);
  Engine_EventSetMessage(MsgVaultSeeThatsHappened);
  VaultInteriors_ShowMessageAndWait(IVAN, 20);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  VaultInteriors_PlayAnimationAndWait(GARET, ANIM_NOD, 40);
  Actor_WalkToAndWait(ACTOR_LOFT_ROOFER, 808, 408);
  Actor_FaceDirection(ACTOR_LOFT_ROOFER, FACING_WEST, 0);
  Engine_EventWait(30);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_LOFT_ROOFER, 0);
  Actor_FaceActor(GARET, ACTOR_LOFT_ROOFER, 0);
  VaultInteriors_FaceActorAndWait(IVAN, ACTOR_LOFT_ROOFER, 20);
  Engine_ActorRunRepeatedMotion(ACTOR_LOFT_ROOFER, 1);
  Engine_EventWait(20);
  VaultInteriors_ShowMessageAndWait(ACTOR_LOFT_ROOFER, 20);
  Actor_SetSpeed(ACTOR_LOFT_MAYOR, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(ACTOR_LOFT_ESCORT, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(ACTOR_LOFT_SEARCHER, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetPosition(ACTOR_LOFT_MAYOR, PIXELS(744), PIXELS(408));
  Actor_WalkToAndWait(ACTOR_LOFT_MAYOR, 768, 408);
  VaultInteriors_FaceActorAndWait(ACTOR_LOFT_MAYOR, ACTOR_THIEF_LEADER, 30);
  Actor_SetPosition(ACTOR_LOFT_ESCORT, PIXELS(744), PIXELS(408));
  Actor_WalkToAndWait(ACTOR_LOFT_ESCORT, 768, 408);
  Actor_SetPosition(ACTOR_LOFT_SEARCHER, PIXELS(744), PIXELS(408));
  Actor_WalkTo(ACTOR_LOFT_SEARCHER, 784, 400);
  Actor_WalkToAndWait(ACTOR_LOFT_ESCORT, 776, 424);
  Engine_ActorWaitForMove(ACTOR_LOFT_SEARCHER);
  Actor_FaceActor(ACTOR_LOFT_ESCORT, ACTOR_THIEF_LEADER, 0);
  VaultInteriors_FaceActorAndWait(ACTOR_LOFT_SEARCHER, ACTOR_THIEF_LEADER, 20);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_LOFT_MAYOR, 0);
  Actor_FaceActor(GARET, ACTOR_LOFT_MAYOR, 0);
  VaultInteriors_FaceActorAndWait(IVAN, ACTOR_LOFT_MAYOR, 20);
  // The mayor scolds them for stealing in a disaster; the thieves talk back.
  VaultInteriors_PlayAnimationAndWait(ACTOR_LOFT_MAYOR, ANIM_SHAKE_HEAD, 20);
  VaultInteriors_ShowMessageAndWait(ACTOR_LOFT_MAYOR, 20);
  VaultInteriors_ShowMessageAndWait(ACTOR_FIRST_THIEF, 20);
  VaultInteriors_ShowMessageAndWait(ACTOR_SECOND_THIEF, 30);
  // "What should we do with them?" Locked up for a long, long time; the two
  // townsmen rouse the thieves to tie them up.
  VaultInteriors_FaceEachOtherAndWait(ACTOR_LOFT_MAYOR, ACTOR_LOFT_ESCORT, 20);
  Engine_ActorRunRepeatedMotion(ACTOR_LOFT_ESCORT, 1);
  Engine_EventWait(20);
  VaultInteriors_ShowMessageAndWait(ACTOR_LOFT_ESCORT, 20);
  VaultInteriors_PlayAnimationAndWait(ACTOR_LOFT_MAYOR, ANIM_NOD, 30);
  VaultInteriors_FaceEachOtherAndWait(ACTOR_LOFT_MAYOR, ACTOR_LOFT_SEARCHER,
                                      20);
  VaultInteriors_PlayAnimationAndWait(ACTOR_LOFT_MAYOR, ANIM_NOD, 30);
  VaultInteriors_FaceActorAndWait(ACTOR_LOFT_MAYOR, ACTOR_THIEF_LEADER, 20);
  VaultInteriors_ShowMessageAndWait(ACTOR_LOFT_MAYOR, 20);
  Engine_ActorSetAnimation(ACTOR_LOFT_ESCORT, ANIM_NOD);
  VaultInteriors_PlayAnimationAndWait(ACTOR_LOFT_SEARCHER, ANIM_NOD, 20);
  VaultInteriors_FaceEachOtherAndWait(ACTOR_LOFT_ESCORT, ACTOR_LOFT_SEARCHER,
                                      20);
  Engine_ActorSetAnimation(ACTOR_LOFT_ESCORT, ANIM_NOD);
  VaultInteriors_PlayAnimationAndWait(ACTOR_LOFT_SEARCHER, ANIM_NOD, 20);
  Actor_WalkTo(ACTOR_LOFT_SEARCHER, 792, 392);
  Actor_WalkToAndWait(ACTOR_LOFT_ESCORT, 784, 400);
  Actor_FaceActor(ACTOR_LOFT_ESCORT, ACTOR_SECOND_THIEF, 0);
  Engine_ActorWaitForMove(ACTOR_LOFT_SEARCHER);
  VaultInteriors_FaceActorAndWait(ACTOR_LOFT_SEARCHER, ACTOR_FIRST_THIEF, 20);
  Engine_ActorRunRepeatedMotion(ACTOR_LOFT_ESCORT, 1);
  Engine_EventWait(20);
  VaultInteriors_ShowMessageAndWait(ACTOR_LOFT_ESCORT, 20);
  VaultInteriors_PlayAnimationAndWait(ACTOR_LOFT_SEARCHER, ANIM_SHAKE_HEAD, 20);
  VaultInteriors_ShowMessageAndWait(ACTOR_LOFT_SEARCHER, 30);
  VaultInteriors_FaceEachOtherAndWait(ACTOR_LOFT_ESCORT, ACTOR_PARTY_LEADER,
                                      20);
  VaultInteriors_ShowMessageAndWait(ACTOR_LOFT_ESCORT, 20);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_ActorSetAnimation(GARET, ANIM_NOD);
  VaultInteriors_PlayAnimationAndWait(IVAN, ANIM_NOD, 50);
  // The window closes while the thieves are tied up.
  work = gEventWork;
  work->transitionFrames = 30;
  work->startTransition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(60);
  Engine_EventEnd();
}

/// Returns the stolen goods and rod, thanks the party, and clears the loft.
///
/// Called by Scene_Initialize after VaultInteriors_DefeatThieves. Sets
/// FLAG_VAULT_THIEVES_JAILED; the thieves are led away, the mayor's urn and
/// the Sanctum's gold statue come out of the chests, Ivan takes the Shaman's
/// Rod, gives back any Venus Djinn and leaves the party, and every actor of
/// the loft is cleared away.
static void VaultInteriors_ReturnStolenGoods(void) {
  struct FieldActor *temporaryActor;
  struct FieldActor *actor;
  s32 aftermath;
  s32 rod;
  struct EventWork *event;

  event = gEventWork;
  GameFlag_Set(FLAG_VAULT_THIEVES_JAILED);
  // With the screen still closed, the chests go back along the wall and the
  // thieves stand tied before the mayor.
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
  Engine_ActorSetSpriteFlags(actor, SPRITE_FLAG_SHADOW);
  actor = Object_GetById(ACTOR_FIRST_THIEF);
  Engine_ActorSetSpriteFlags(actor, SPRITE_FLAG_SHADOW);
  actor = Object_GetById(ACTOR_SECOND_THIEF);
  Engine_ActorSetSpriteFlags(actor, SPRITE_FLAG_SHADOW);
  Actor_SetPosition(ACTOR_LOFT_ESCORT, PIXELS(768), PIXELS(408));
  Actor_SetPosition(ACTOR_LOFT_SEARCHER, PIXELS(768), PIXELS(424));
  Actor_MoveToAndWait(ACTOR_LOFT_MAYOR, 784, 424);
  Actor_SetPosition(ACTOR_LOFT_ROOFER, PIXELS(808), PIXELS(408));
  Actor_FaceActor(ACTOR_LOFT_ESCORT, ACTOR_LOFT_MAYOR, 0);
  Actor_FaceActor(ACTOR_LOFT_ROOFER, ACTOR_LOFT_MAYOR, 0);
  Actor_FaceActor(ACTOR_LOFT_SEARCHER, ACTOR_THIEF_LEADER, 0);
  Actor_FaceActor(ACTOR_LOFT_MAYOR, ACTOR_THIEF_LEADER, 0);
  Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(792), PIXELS(440));
  Actor_SetPosition(GARET, PIXELS(808), PIXELS(440));
  Actor_SetPosition(IVAN, PIXELS(776), PIXELS(440));
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_THIEF_LEADER, 0);
  Actor_FaceActor(GARET, ACTOR_THIEF_LEADER, 0);
  Actor_FaceActor(IVAN, ACTOR_THIEF_LEADER, 0);
  gEventWork->transitionFrames = 30;
  gEventWork->startTransition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(40);
  // "Isaac, right? I won't forget this!" "Take them away!"
  Engine_ActorRunRepeatedMotion(ACTOR_THIEF_LEADER, 2);
  aftermath = MsgVaultYouIsaacRightWontForget;
  Engine_EventSetMessage(aftermath);
  VaultInteriors_ShowMessageAndWait(ACTOR_THIEF_LEADER, 20);
  Engine_ActorRunRepeatedMotion(ACTOR_LOFT_MAYOR, 1);
  Engine_EventWait(20);
  VaultInteriors_PlayAnimationAndWait(ACTOR_LOFT_MAYOR, ANIM_SHAKE_HEAD, 20);
  VaultInteriors_ShowMessageAndWait(ACTOR_LOFT_MAYOR, 20);
  Engine_ActorSetAnimation(ACTOR_LOFT_ESCORT, ANIM_NOD);
  VaultInteriors_PlayAnimationAndWait(ACTOR_LOFT_ROOFER, ANIM_NOD, 20);
  Engine_ActorStartRepeatedMotion(ACTOR_FIRST_THIEF, 2);
  Engine_ActorStartRepeatedMotion(ACTOR_SECOND_THIEF, 2);
  Engine_EventWait(60);
  // The escort leads the three thieves down the ladder, the roofer after them.
  Actor_SetSpeed(ACTOR_LOFT_ESCORT, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkTo(ACTOR_LOFT_ESCORT, 746, 408);
  Actor_FaceDirection(ACTOR_LOFT_MAYOR, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_LOFT_SEARCHER, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_SetActionScript(ACTOR_FIRST_THIEF, gVaultInteriors_VaultScriptB);
  Engine_EventWait(20);
  Actor_SetActionScript(ACTOR_THIEF_LEADER, gVaultInteriors_VaultScriptB);
  Engine_EventWait(15);
  Actor_SetActionScript(ACTOR_SECOND_THIEF, gVaultInteriors_VaultScriptB);
  Engine_EventWait(35);
  Actor_SetActionScript(ACTOR_LOFT_ROOFER, gVaultInteriors_VaultScriptA);
  Engine_EventWait(20);
  Engine_ActorWaitForMove(ACTOR_LOFT_ESCORT);
  Actor_SetPosition(ACTOR_LOFT_ESCORT, 0, 0);
  Engine_EventWait(40);
  // The mayor and the searcher thank the party.
  Actor_SetSpeed(ACTOR_LOFT_MAYOR, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(ACTOR_LOFT_SEARCHER, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(ACTOR_LOFT_MAYOR, 784, 408);
  Actor_FaceActor(ACTOR_LOFT_MAYOR, ACTOR_PARTY_LEADER, 0);
  Actor_WalkToAndWait(ACTOR_LOFT_SEARCHER, 768, 408);
  VaultInteriors_FaceActorAndWait(ACTOR_LOFT_SEARCHER, ACTOR_PARTY_LEADER, 20);
  VaultInteriors_PlayAnimationAndWait(ACTOR_LOFT_MAYOR, ANIM_NOD, 20);
  VaultInteriors_ShowMessageAndWait(ACTOR_LOFT_MAYOR, 20);
  VaultInteriors_PlayAnimationAndWait(ACTOR_LOFT_SEARCHER, ANIM_NOD, 20);
  Actor_FaceDirection(ACTOR_LOFT_SEARCHER, FACING_SOUTHEAST, 10);
  VaultInteriors_ShowMessageAndWait(ACTOR_LOFT_SEARCHER, 20);
  VaultInteriors_FaceEachOtherAndWait(ACTOR_PARTY_LEADER, GARET, 50);
  VaultInteriors_FaceEachOtherAndWait(ACTOR_PARTY_LEADER, IVAN, 50);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_LOFT_MAYOR, 0);
  Actor_FaceActor(GARET, ACTOR_LOFT_MAYOR, 0);
  VaultInteriors_FaceActorAndWait(IVAN, ACTOR_LOFT_MAYOR, 20);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_ActorSetAnimation(GARET, ANIM_NOD);
  VaultInteriors_PlayAnimationAndWait(IVAN, ANIM_NOD, 50);
  // "Let's take a look at what they stole." The two go round to the chests.
  Engine_ActorFaceEachOther(ACTOR_LOFT_MAYOR, ACTOR_LOFT_SEARCHER, 0);
  VaultInteriors_PlayAnimationAndWait(ACTOR_LOFT_MAYOR, ANIM_NOD, 20);
  VaultInteriors_ShowMessageAndWait(ACTOR_LOFT_MAYOR, 20);
  Actor_SetActionScript(ACTOR_LOFT_SEARCHER, gVaultInteriors_VaultScriptC);
  Engine_EventWait(50);
  Actor_SetActionScript(ACTOR_LOFT_MAYOR, gVaultInteriors_VaultScriptD);
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
  VaultInteriors_FaceActorAndWait(ACTOR_LOFT_SEARCHER, ACTOR_LOFT_MAYOR, 60);
  VaultInteriors_FaceActorAndWait(ACTOR_LOFT_MAYOR, ACTOR_LOFT_SEARCHER, 40);
  VaultInteriors_PlayAnimationAndWait(ACTOR_LOFT_MAYOR, ANIM_NOD, 40);
  Actor_FaceDirection(ACTOR_LOFT_MAYOR, FACING_EAST, 0);
  Engine_EventWait(20);
  // The mayor opens the first chest and his urn rises out of it.
  Engine_ActorRunRepeatedMotion(ACTOR_LOFT_MAYOR, 2);
  Event_PlayCue(SE_124);
  Engine_ActorSetAnimation(ACTOR_FIRST_CHEST, ANIM_SHAKE_HEAD);
  Actor_SetPosition(ACTOR_FOUND_URN, PIXELS(872), PIXELS(424));
  Engine_ActorSetSpritePriority(ACTOR_FOUND_URN, 1);
  Actor_SetSpeed(ACTOR_FOUND_URN, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetDestinationOffset(ACTOR_FOUND_URN, 0, -8);
  Engine_ActorWaitForMove(ACTOR_FOUND_URN);
  Engine_ActorRunRepeatedMotion(ACTOR_FOUND_URN, 2);
  Engine_EventWait(60);
  Engine_MessageShowCentered((aftermath + 5), 1);
  Engine_ActorSetAnimation(ACTOR_FIRST_CHEST, ANIM_WALK);
  Actor_SetPosition(ACTOR_FOUND_URN, 0, 0);
  gEventWork->message++;
  Engine_ActorRunRepeatedMotion(ACTOR_LOFT_SEARCHER, 1);
  VaultInteriors_ShowMessageAndWait(ACTOR_LOFT_SEARCHER, 20);
  VaultInteriors_FaceEachOtherAndWait(ACTOR_PARTY_LEADER, GARET, 40);
  VaultInteriors_FaceActorAndWait(ACTOR_LOFT_MAYOR, ACTOR_LOFT_SEARCHER, 20);
  VaultInteriors_PlayAnimationAndWait(ACTOR_LOFT_MAYOR, ANIM_NOD, 20);
  VaultInteriors_ShowMessageAndWait(ACTOR_LOFT_MAYOR, 30);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_LOFT_SEARCHER, 0);
  VaultInteriors_FaceActorAndWait(GARET, ACTOR_LOFT_SEARCHER, 40);
  Actor_FaceDirection(ACTOR_LOFT_SEARCHER, FACING_EAST, 0);
  Engine_EventWait(40);
  // The searcher opens the second: the gold statue stolen from the Sanctum.
  Engine_ActorRunRepeatedMotion(ACTOR_LOFT_SEARCHER, 2);
  Event_PlayCue(SE_124);
  Engine_ActorSetAnimation(ACTOR_SECOND_CHEST, ANIM_SHAKE_HEAD);
  Object_GetById(ACTOR_FOUND_STATUE)->motionFlags = 0;
  Engine_ActorSetSpritePriority(ACTOR_FOUND_STATUE, 1);
  Actor_SetPosition(ACTOR_FOUND_STATUE, PIXELS(872), PIXELS(408));
  Actor_SetSpeed(ACTOR_FOUND_STATUE, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetDestinationOffset(ACTOR_FOUND_STATUE, 0, -8);
  Engine_ActorWaitForMove(ACTOR_FOUND_STATUE);
  Engine_ActorRunRepeatedMotion(ACTOR_FOUND_STATUE, 2);
  Engine_EventWait(60);
  Engine_MessageShowCentered((aftermath + 8), 1);
  Engine_ActorSetAnimation(ACTOR_SECOND_CHEST, ANIM_WALK);
  Actor_SetPosition(ACTOR_FOUND_STATUE, 0, 0);
  gEventWork->message++;
  Actor_ShowEmote(ACTOR_LOFT_MAYOR, EMOTE_IN_FRONT | 2, 0);
  Engine_EventWait(60);
  VaultInteriors_ShowMessageAndWait(ACTOR_LOFT_MAYOR, 20);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_ActorStartRepeatedMotion(GARET, 2);
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  Engine_EventWait(20);
  VaultInteriors_PlayAnimationAndWait(ACTOR_LOFT_SEARCHER, ANIM_NOD, 50);
  VaultInteriors_FaceActorAndWait(ACTOR_LOFT_MAYOR, ACTOR_PARTY_LEADER, 20);
  VaultInteriors_ShowMessageAndWait(ACTOR_LOFT_MAYOR, 30);
  // The searcher finds another chest; the mayor asks whether it is what Ivan
  // was looking for, and he can return to Master Hammet.
  Actor_FaceDirection(ACTOR_LOFT_SEARCHER, FACING_NORTH + FACING_STEP, 0);
  Engine_EventWait(30);
  Actor_ShowEmote(ACTOR_LOFT_SEARCHER, EMOTE_IN_FRONT | 0, 0);
  Engine_EventWait(60);
  Actor_WalkToAndWait(ACTOR_LOFT_SEARCHER, 856, 376);
  Engine_EventWait(20);
  VaultInteriors_FaceActorAndWait(ACTOR_LOFT_SEARCHER, ACTOR_LOFT_MAYOR, 20);
  VaultInteriors_ShowMessageAndWait(ACTOR_LOFT_SEARCHER, 20);
  Actor_FaceActor(ACTOR_LOFT_MAYOR, ACTOR_LOFT_SEARCHER, 0);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | 0, 0);
  Engine_EventWait(60);
  VaultInteriors_FaceActorAndWait(IVAN, ACTOR_LOFT_SEARCHER, 30);
  VaultInteriors_FaceActorAndWait(ACTOR_LOFT_MAYOR, IVAN, 20);
  VaultInteriors_PlayAnimationAndWait(ACTOR_LOFT_MAYOR, ANIM_NOD, 20);
  Event_ShowMessage(ACTOR_LOFT_MAYOR, 0);
  Actor_FaceDirection(ACTOR_LOFT_SEARCHER, FACING_SOUTH + FACING_STEP, 0);
  Engine_EventWait(30);
  VaultInteriors_FaceActorAndWait(IVAN, ACTOR_LOFT_MAYOR, 20);
  VaultInteriors_PlayAnimationAndWait(IVAN, ANIM_NOD, 20);
  VaultInteriors_ShowMessageAndWait(ACTOR_LOFT_MAYOR, 20);
  Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, IVAN, 0);
  VaultInteriors_FaceActorAndWait(GARET, IVAN, 20);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  VaultInteriors_PlayAnimationAndWait(GARET, ANIM_NOD, 40);
  VaultInteriors_PlayAnimationAndWait(IVAN, ANIM_NOD, 30);
  // Hammet was seen heading toward Lunpa: Dodonpa must have kidnapped him.
  Engine_ActorRunRepeatedMotion(ACTOR_LOFT_MAYOR, 1);
  Engine_EventWait(20);
  VaultInteriors_PlayAnimationAndWait(ACTOR_LOFT_MAYOR, ANIM_SHAKE_HEAD, 20);
  VaultInteriors_ShowMessageAndWait(ACTOR_LOFT_MAYOR, 20);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_LOFT_MAYOR, 0);
  Actor_FaceActor(GARET, ACTOR_LOFT_MAYOR, 0);
  VaultInteriors_FaceActorAndWait(IVAN, ACTOR_LOFT_MAYOR, 20);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  Engine_ActorStartRepeatedMotion(GARET, 1);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | 2, 0);
  Engine_EventWait(60);
  VaultInteriors_ShowMessageAndWait(IVAN, 40);
  VaultInteriors_PlayAnimationAndWait(ACTOR_LOFT_MAYOR, ANIM_NOD, 20);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_SHAKE_HEAD);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_SHAKE_HEAD);
  Engine_ActorRunRepeatedMotion(IVAN, 3);
  Engine_EventWait(30);
  VaultInteriors_PlayAnimationAndWait(ACTOR_LOFT_MAYOR, ANIM_SHAKE_HEAD, 20);
  VaultInteriors_ShowMessageAndWait(ACTOR_LOFT_MAYOR, 20);
  VaultInteriors_FaceEachOtherAndWait(ACTOR_PARTY_LEADER, GARET, 20);
  // Ivan rushes forward: what will happen to Hammet? The mayor calms him:
  // Dodonpa wants a ransom, so Hammet is safe for now.
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  VaultInteriors_PlayAnimationAndWait(IVAN, ANIM_SHAKE_HEAD, 30);
  Actor_SetSpeed(IVAN, PIXELS(3) / 2, PIXELS(3) / 4);
  Actor_WalkToAndWait(IVAN, 800, 408);
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  Event_ShowMessage(IVAN, 0);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_LOFT_MAYOR, 0);
  VaultInteriors_FaceActorAndWait(GARET, ACTOR_LOFT_MAYOR, 30);
  VaultInteriors_PlayAnimationAndWait(ACTOR_LOFT_MAYOR, ANIM_SHAKE_HEAD, 20);
  VaultInteriors_ShowMessageAndWait(ACTOR_LOFT_MAYOR, 20);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(GARET,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(IVAN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Engine_EventWait(60);
  VaultInteriors_ShowMessageAndWait(IVAN, 20);
  Engine_ActorRunRepeatedMotion(ACTOR_LOFT_MAYOR, 1);
  Engine_EventWait(20);
  VaultInteriors_ShowMessageAndWait(ACTOR_LOFT_MAYOR, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 1, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 1, 0);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | 1, 0);
  Engine_EventWait(60);
  VaultInteriors_PlayAnimationAndWait(ACTOR_LOFT_MAYOR, ANIM_SHAKE_HEAD, 20);
  VaultInteriors_ShowMessageAndWait(ACTOR_LOFT_MAYOR, 20);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  Engine_ActorStartRepeatedMotion(GARET, 1);
  Engine_ActorRunRepeatedMotion(IVAN, 1);
  Engine_EventWait(20);
  VaultInteriors_PlayAnimationAndWait(ACTOR_LOFT_MAYOR, ANIM_NOD, 20);
  VaultInteriors_ShowMessageAndWait(ACTOR_LOFT_MAYOR, 40);
  // The mayor asks the two their names, thanks them, and asks them to come
  // see him before they leave town.
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | 5, 0);
  Engine_EventWait(60);
  Actor_WalkToAndWait(ACTOR_LOFT_MAYOR, 840, 424);
  VaultInteriors_FaceEachOtherAndWait(ACTOR_LOFT_MAYOR, ACTOR_PARTY_LEADER, 20);
  VaultInteriors_ShowMessageAndWait(ACTOR_LOFT_MAYOR, 20);
  VaultInteriors_PlayAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD, 20);
  Actor_FaceDirection(ACTOR_LOFT_MAYOR, FACING_SOUTH + FACING_STEP, 0);
  Engine_EventWait(20);
  VaultInteriors_ShowMessageAndWait(ACTOR_LOFT_MAYOR, 20);
  Engine_ActorRunRepeatedMotion(GARET, 1);
  Engine_EventWait(20);
  VaultInteriors_ShowMessageAndWait(GARET, 20);
  VaultInteriors_PlayAnimationAndWait(ACTOR_LOFT_MAYOR, ANIM_NOD, 20);
  VaultInteriors_ShowMessageAndWait(ACTOR_LOFT_MAYOR, 30);
  VaultInteriors_FaceEachOtherAndWait(ACTOR_LOFT_MAYOR, ACTOR_LOFT_SEARCHER,
                                      20);
  VaultInteriors_ShowMessageAndWait(ACTOR_LOFT_MAYOR, 20);
  // The mayor and the searcher leave down the ladder.
  Actor_WalkToAndWait(ACTOR_LOFT_SEARCHER, 856, 408);
  Actor_SetActionScript(ACTOR_LOFT_MAYOR, gVaultInteriors_VaultScriptE);
  Actor_SetActionScript(ACTOR_LOFT_SEARCHER, gVaultInteriors_VaultScriptE);
  Actor_WalkTo(GARET, 792, 456);
  Actor_SetSpeed(IVAN, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkTo(IVAN, 776, 432);
  Engine_ActorWaitForMove(GARET);
  Actor_FaceDirection(GARET, FACING_NORTH + FACING_STEP, 0);
  Engine_ActorWaitForMove(IVAN);
  Actor_FaceDirection(IVAN, FACING_NORTH + FACING_STEP, 0);
  Object_RefreshSelectorById(ACTOR_LOFT_MAYOR);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_LOFT_MAYOR, 0);
  temporaryActor = Object_GetById(ACTOR_LOFT_SEARCHER);
  {
    u8 *pauseFlag = &temporaryActor->paused;
    u8 paused = 1;

    *pauseFlag = paused;
  }
  temporaryActor->targetX = ACTOR_NO_TARGET;
  temporaryActor->targetY = ACTOR_NO_TARGET;
  temporaryActor->targetZ = ACTOR_NO_TARGET;
  Actor_ShowEmote(ACTOR_LOFT_MAYOR, EMOTE_IN_FRONT | 0, 0);
  Engine_ActorSetAnimation(ACTOR_LOFT_SEARCHER, ANIM_STAND);
  Engine_EventWait(50);
  Actor_FaceActor(ACTOR_LOFT_MAYOR, ACTOR_PARTY_LEADER, 0);
  Actor_WalkToAndWait(GARET, 808, 440);
  Actor_FaceDirection(GARET, FACING_NORTHWEST + FACING_STEP, 0);
  Engine_EventWait(20);
  VaultInteriors_ShowMessageAndWait(ACTOR_LOFT_MAYOR, 20);
  VaultInteriors_FaceEachOtherAndWait(ACTOR_PARTY_LEADER, GARET, 50);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_LOFT_MAYOR, 0);
  VaultInteriors_FaceActorAndWait(GARET, ACTOR_LOFT_MAYOR, 20);
  VaultInteriors_PlayAnimationAndWait(ACTOR_LOFT_MAYOR, ANIM_NOD, 20);
  Actor_WalkTo(ACTOR_LOFT_MAYOR, 744, 408);
  Actor_WalkTo(ACTOR_LOFT_SEARCHER, 744, 408);
  Engine_ActorWaitForMove(ACTOR_LOFT_MAYOR);
  Engine_ActorWaitForMove(ACTOR_LOFT_SEARCHER);
  Engine_EventWait(30);
  // Alone, Garet tries to cheer Ivan up; Ivan only wants to rescue Hammet,
  // but has seen the two's mission in their minds and won't ask their help.
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 792, 408);
  VaultInteriors_FaceEachOtherAndWait(ACTOR_PARTY_LEADER, GARET, 30);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | 5, 0);
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
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 1, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 1, 0);
  Engine_EventWait(60);
  VaultInteriors_PlayAnimationAndWait(IVAN, ANIM_NOD, 20);
  VaultInteriors_ShowMessageAndWait(IVAN, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 0, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 0, 0);
  Engine_EventWait(60);
  VaultInteriors_PlayAnimationAndWait(IVAN, ANIM_NOD, 20);
  VaultInteriors_ShowMessageAndWait(IVAN, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 2, 0);
  Engine_EventWait(60);
  VaultInteriors_PlayAnimationAndWait(IVAN, ANIM_SHAKE_HEAD, 20);
  VaultInteriors_ShowMessageAndWait(IVAN, 20);
  VaultInteriors_PlayAnimationAndWait(GARET, ANIM_NOD, 20);
  VaultInteriors_ShowMessageAndWait(GARET, 30);
  // "Then, I suppose this is good-bye." A no has Garet say Isaac would help,
  // and Ivan decline.
  Event_OpenMessage(IVAN, 0);
  if (Engine_EventChooseYesNo(0, 0) != 0) {
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(GARET, 1);
    Engine_EventWait(20);
    VaultInteriors_ShowMessageAndWait(GARET, 20);
    VaultInteriors_PlayAnimationAndWait(IVAN, ANIM_SHAKE_HEAD, 20);
    VaultInteriors_ShowMessageAndWait(IVAN, 20);
  }
  // Ivan walks round to the third chest and takes the Shaman's Rod from it.
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
  temporaryActor = (struct FieldActor *)BattleFx_PlayCueAndStartEmitterOnTarget(
      IVAN, ACTOR_THIRD_CHEST, ITEM_SHAMANS_ROD);
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
  // He thanks the town, hands back any Venus Djinni and leaves the party.
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
      Vault_ReturnIsaacsDjinni();
    }
    Party_RemoveOwnerRestored(IVAN);
    *slot = saved;
  }
  VaultInteriors_PlayAnimationAndWait(GARET, ANIM_NOD, 50);
  Actor_WalkToAndWait(IVAN, 776, 408);
  Actor_FaceActor(ACTOR_PARTY_LEADER, IVAN, 0);
  Actor_FaceActor(GARET, IVAN, 0);
  // Ivan goes down the ladder; Garet: they'd better get moving too.
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
  // Clear the loft of everyone the cutscene used.
  Actor_SetPosition(ACTOR_LOFT_ROOFER, 0, 0);
  Actor_SetPosition(ACTOR_LOFT_MAYOR, 0, 0);
  Actor_SetPosition(ACTOR_LOFT_ESCORT, 0, 0);
  Actor_SetPosition(ACTOR_LOFT_SEARCHER, 0, 0);
  Actor_SetPosition(ACTOR_THIEF_LEADER, 0, 0);
  Actor_SetPosition(ACTOR_FIRST_THIEF, 0, 0);
  Actor_SetPosition(ACTOR_SECOND_THIEF, 0, 0);
  Actor_SetPosition(IVAN, 0, 0);
  gEventWork->startTransition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  Engine_EventEnd();
}

/// Opens the window transition and waits one frame after it finishes.
static void VaultInteriors_OpenWindowTransition(void) {
  struct EventWork *state;

  state = gEventWork;
  state->startTransition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(1);
}

static void VaultInteriors_ShowMessageAndWait(s32 actorId, s32 waitFrames) {
  Engine_EventShowMessage(actorId, 0);
  Engine_EventWait(waitFrames);
}

static void VaultInteriors_FaceActorAndWait(s32 actorId, s32 targetId,
                                            s32 waitFrames) {
  Engine_ActorFaceActor(actorId, targetId, 0);
  Engine_EventWait(waitFrames);
}

static void VaultInteriors_FaceEachOtherAndWait(s32 firstId, s32 secondId,
                                                s32 waitFrames) {
  Engine_ActorFaceEachOther(firstId, secondId, 0);
  Engine_EventWait(waitFrames);
}

/// Waits for the animation to change before the following pause.
static void VaultInteriors_PlayAnimationAndWait(s32 actorId, s32 animation,
                                                s32 waitFrames) {
  Engine_ActorSetAnimation(actorId, animation, waitFrames);
  ObjectMotion_WaitForAnimationChange(actorId);
  Engine_EventWait(waitFrames);
}

/// Begins Mind Read, chooses its target, and shows its opening effect.
static void VaultInteriors_BeginMindRead(s32 casterId, s32 targetId) {
  Engine_PsynergyBegin(ABILITY_MIND_READ, 1);
  Engine_PsynergySetTarget(casterId, targetId);
  Engine_PsynergyRaiseHands();
  Engine_PsynergyPlayEffect(1);
  Engine_TaskWait(1);
}

/// Finishes Mind Read and releases its scene effect objects.
static void VaultInteriors_EndMindRead(void) {
  Engine_PsynergyPlayEffect(2);
  Engine_PsynergyLowerHands();
  BattleEffect_CleanupSceneObjects();
}

#include "common/vault_itemobj.h"

/// Finds the route node containing the actor's position on the eight-pixel
/// grid.
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

/// Chooses the outgoing connection nearest the requested facing.
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
    /* FAKEMATCH(volatile): the used-stop test reads stopIndex through a
     * volatile lvalue, so the stopIndex assignment loads it again; as a plain
     * test, 1 of 49 instructions differ in tbs-en. */
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

/// Checks whether an actor's destination or current position lies within eight
/// pixels.
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

/// Checks whether an actor's destination or current position lies within
/// sixteen pixels.
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

/// Reserves a full cell around the leader and half a cell around each actor
/// following a route. Testing destinations as well as current positions
/// prevents two actors choosing the same stop while still walking.
static s32 VaultInteriors_CheckStopOccupancy(struct VaultInteriorsStop *stop) {
  s32 x;
  s32 z;

  if (stop == NULL) {
    return 1;
  }
  x = (stop->gridX << 19) + PIXELS(72);
  z = (stop->gridZ << 19) + PIXELS(632);
  if (VaultInteriors_IsActorWithinSixteenPixels(x, z, ACTOR_PARTY_LEADER) !=
          0 ||
      VaultInteriors_IsActorWithinEightPixels(x, z, IVAN) != 0 ||
      VaultInteriors_IsActorWithinEightPixels(x, z, ACTOR_SUSPECTED_LEADER) !=
          0 ||
      VaultInteriors_IsActorWithinEightPixels(x, z, ACTOR_SUSPECTED_THIEF) !=
          0) {
    return -1;
  }
  return 0;
}

/// Sets the actor's destination to the route stop's cell center.
static void VaultInteriors_MoveActorToStop(struct FieldActor *actor,
                                           struct VaultInteriorsStop *stop) {
  Engine_ObjectSetPosition(actor, (stop->gridX << 19) + PIXELS(72), 0,
                           (stop->gridZ << 19) + PIXELS(632));
}

/// Steers Ivan and both suspects through the graph and detects a sustained
/// cornering.
///
/// Run each frame as a scene callback from VaultInteriors_StartActorStops
/// until Event_ReadCorneredSuspectsMinds removes it. Raises event 200 plus the
/// blocked suspects (1 the leader, 2 the thief) after thirty frames with at
/// least one suspect blocked; a frame when both can move restarts the wait.
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
      /* FAKEMATCH(dead): a dead store keeps this branch through jump
       * optimization (bge to the else, then b past it); as an empty branch, 1
       * of 269 instructions differ in tbs-en. */
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
        Engine_ActorSetAttachedEffect(ACTOR_SUSPECTED_LEADER,
                                      ATTACHED_EFFECT_ONCE);
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
        Engine_ActorSetAttachedEffect(ACTOR_SUSPECTED_THIEF,
                                      ATTACHED_EFFECT_ONCE);
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
  // A failed route choice alone is not enough to end the chase.
  if (blocked != 0) {
    if (++gVaultInteriors_StopTimer > VAULT_INTERIORS_CORNERED_FRAMES - 1)
      work->raisedTrigger = blocked + VAULT_INTERIORS_CORNERED_EVENT_BASE;
  } else {
    gVaultInteriors_StopTimer = blocked;
  }
}

/// Arranges the chase, constrains its camera, and starts the route callback.
///
/// Called by Event_BeginCorneringSuspects. Starts
/// VaultInteriors_UpdateActorStops and the chase music, MUS_009.
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
  // XXX: flag 0x1ff is cleared here and named nowhere else; a reader of it in
  // the engine or another scene would say what it holds.
  GameFlag_Clear(0x1ff);
  Engine_EventEnd();
  Engine_AudioPlayCue(MUS_009);
}

// clang-format off
// A missing edge keeps both sentinel bytes; valid facings use the angle's high
// byte.
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

s32 gVaultInteriors_FirstThiefEntryScript[] = {
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

s32 gVaultInteriors_SecondThiefEntryScript[] = {
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
    { ENTRANCE_DEFAULT, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { VAULT_INTERIORS_FROM_VAULT_EAST_SOUTHEAST, CONDITION_ALWAYS, 192, 0, 220, FACING_NORTH, 0, 32, 0, 272, 240, 0 },
    { VAULT_INTERIORS_FROM_VAULT_SOUTHEAST, CONDITION_ALWAYS, 432, 0, 254, FACING_NORTH, 0, 288, 0, 528, 272, 0 },
    { VAULT_INTERIORS_FROM_VAULT_SOUTHWEST, CONDITION_ALWAYS, 720, 0, 782, FACING_NORTH, 0, 432, 576, 800, 800, 0 },
    { VAULT_INTERIORS_FROM_VAULT_EAST, CONDITION_ALWAYS, 624, 0, 236, FACING_NORTH, 0, 560, 32, 848, 256, 0 },
    { VAULT_INTERIORS_FROM_VAULT_NORTH_NORTHEAST, CONDITION_ALWAYS, 144, 0, 492, FACING_NORTH, 0, 16, 288, 256, 512, 0 },
    { VAULT_INTERIORS_FROM_VAULT_NORTHWEST, CONDITION_ALWAYS, 416, 0, 526, FACING_NORTH, 0, 320, 288, 592, 544, 0 },
    { VAULT_INTERIORS_INN_FROM_INN_UPSTAIRS, CONDITION_ALWAYS, 744, 0, 640, FACING_SOUTH, 0, 432, 576, 800, 800, 0 },
    { VAULT_INTERIORS_INN_UPSTAIRS_FROM_INN, CONDITION_ALWAYS, 358, 0, 632, FACING_SOUTH, 0, 32, 576, 400, 776, 0 },
    { VAULT_INTERIORS_FROM_VAULT_WEST, CONDITION_ALWAYS, 648, 0, 424, FACING_SOUTH, 0, 560, 320, 800, 480, 0 },
    { VAULT_INTERIORS_CRATE_ROOM_FROM_LOFT, CONDITION_ALWAYS, 704, 0, 408, FACING_WEST, 0, 560, 320, 800, 480, 0 },
    { VAULT_INTERIORS_LOFT_FROM_CRATE_ROOM, CONDITION_ALWAYS, 768, 0, 408, FACING_EAST, 0, 712, 288, 952, 496, 0 },
    { VAULT_INTERIORS_LOFT_AFTER_LOSING_TO_THIEVES, CONDITION_ALWAYS, 792, 0, 432, FACING_NORTH, 0, 712, 288, 952, 496, 0 },
    { VAULT_INTERIORS_LOFT_AFTER_THIEVES_DEFEATED, CONDITION_ALWAYS, 792, 0, 432, FACING_NORTH, 0, 712, 288, 952, 496, 0 },
    { SCENE_TABLE_END },
};

struct SceneRegion gVaultInteriors_Regions[] = {
    {639, 33, 407, 657, 51, 425, CONDITION_ALWAYS, VAULT_INTERIORS_TO_VAULT_WEST},
    {SCENE_TABLE_END},
};

u32 gVaultInteriors_Exits[] = {
    SCENE_EXITS(VaultInteriors),
    SCENE_EXIT(VAULT_INTERIORS_TO_VAULT_EAST_SOUTHEAST, Vault, VAULT_FROM_VAULT_INTERIORS_EAST_SOUTHEAST),
    SCENE_EXIT(VAULT_INTERIORS_TO_VAULT_SOUTHEAST, Vault, VAULT_FROM_VAULT_INTERIORS_SOUTHEAST),
    SCENE_EXIT(VAULT_INTERIORS_TO_VAULT_SOUTHWEST, Vault, VAULT_FROM_VAULT_INTERIORS_SOUTHWEST),
    SCENE_EXIT(VAULT_INTERIORS_TO_VAULT_EAST, Vault, VAULT_FROM_VAULT_INTERIORS_EAST),
    SCENE_EXIT(VAULT_INTERIORS_TO_VAULT_NORTH_NORTHEAST, Vault, VAULT_FROM_VAULT_INTERIORS_NORTH_NORTHEAST),
    SCENE_EXIT(VAULT_INTERIORS_TO_VAULT_NORTHWEST, Vault, VAULT_FROM_VAULT_INTERIORS_NORTHWEST),
    SCENE_EXIT(VAULT_INTERIORS_INN_UPSTAIRS_TO_INN, VaultInteriors, VAULT_INTERIORS_INN_FROM_INN_UPSTAIRS),
    SCENE_EXIT(VAULT_INTERIORS_INN_TO_INN_UPSTAIRS, VaultInteriors, VAULT_INTERIORS_INN_UPSTAIRS_FROM_INN),
    SCENE_EXIT(VAULT_INTERIORS_TO_VAULT_WEST, Vault, VAULT_FROM_VAULT_INTERIORS_WEST),
    SCENE_EXIT(VAULT_INTERIORS_LOFT_TO_CRATE_ROOM, VaultInteriors, VAULT_INTERIORS_CRATE_ROOM_FROM_LOFT),
    SCENE_EXIT(VAULT_INTERIORS_CRATE_ROOM_TO_LOFT, VaultInteriors, VAULT_INTERIORS_LOFT_FROM_CRATE_ROOM),
    SCENE_EXIT(VAULT_INTERIORS_TO_SANCTUM, Sanctum, SANCTUM_FROM_VAULT_INTERIORS),
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

struct ScenePlacement gVaultInteriors_LoftPlacements[] = {
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
    { EVENT_EXIT, 1, CONDITION_ALWAYS, VAULT_INTERIORS_TO_VAULT_EAST_SOUTHEAST },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, VAULT_INTERIORS_TO_VAULT_SOUTHEAST },
    { EVENT_EXIT, 3, CONDITION_ALWAYS, VAULT_INTERIORS_TO_VAULT_SOUTHWEST },
    { EVENT_EXIT, 4, CONDITION_ALWAYS, VAULT_INTERIORS_TO_VAULT_EAST },
    { EVENT_EXIT, 5, CONDITION_ALWAYS, VAULT_INTERIORS_TO_VAULT_NORTH_NORTHEAST },
    { EVENT_EXIT, 6, CONDITION_ALWAYS, VAULT_INTERIORS_TO_VAULT_NORTHWEST },
    { EVENT_TOUCH, 7, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveGuestsRoom) },
    { EVENT_EXIT, 8, CONDITION_ALWAYS, VAULT_INTERIORS_INN_TO_INN_UPSTAIRS },
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
    { EVENT_RAISED, VAULT_INTERIORS_LEADER_CORNERED, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReadCorneredSuspectsMinds) },
    { EVENT_RAISED, VAULT_INTERIORS_THIEF_CORNERED, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReadCorneredSuspectsMinds) },
    { EVENT_RAISED, VAULT_INTERIORS_BOTH_CORNERED, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReadCorneredSuspectsMinds) },
    { EVENT_RAISED, FIELD_EVENT_MENUS_RAISED, CONDITION_ALWAYS, EVENT_SCRIPT(Event_GaretRefusesToLeave) },
    { EVENT_TOUCH, 26, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ClimbToLoft) },
    { TOUCH_WALKING, 40, CONDITION_ALWAYS, EVENT_SCRIPT(Event_JumpAcrossRoofGap) },
    { EVENT_TALK, IVAN, IF_FLAG_SET(FLAG_SCENE_TEMP_50), 0 },
    { EVENT_TALK, IVAN, CONDITION_ALWAYS, EVENT_SCRIPT(Event_MeetIvan) },
    { EVENT_TALK, ACTOR_GEM_OWNER, CONDITION_ALWAYS, MSGNO(MsgVaultThisRockWasBlownHereAll) },
    { EVENT_TALK, ACTOR_GEM_OWNERS_WIFE, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkAboutRocks) },
    { EVENT_TALK, ACTOR_WARY_NEIGHBOR, CONDITION_ALWAYS, MSGNO(MsgVaultMyNeighborCreepsMeOutHe) },
    { EVENT_TALK, ACTOR_CHILD, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ChildAsksAboutTravel) },
    { EVENT_TALK, ACTOR_GRANDPA, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkAboutMissingThings) },
    { EVENT_TALK, ACTOR_GRANDPAS_WIFE, CONDITION_ALWAYS, MSGNO(MsgVaultEverSinceTheEruptionGrandpaKeeps) },
    { EVENT_TALK, ACTOR_WOULD_BE_HELPER, FLAG_VAULT_IVAN_JOINED_ROD_SEARCH, MSGNO(MsgVaultICouldHelpIvanIfI) },
    { EVENT_TALK, ACTOR_WOULD_BE_HELPER, CONDITION_ALWAYS, MSGNO(MsgVaultYoureHelpingIvanYouDontLook) },
    { EVENT_TALK, ACTOR_SECRET_KEEPER, FLAG_VAULT_IVAN_JOINED_ROD_SEARCH, EVENT_SCRIPT(Event_OfferIvansSecret) },
    { EVENT_TALK, ACTOR_SECRET_KEEPER, CONDITION_ALWAYS, MSGNO(MsgVaultSinceYoureHelpingIvanIllTell) },
    { EVENT_TALK, ACTOR_MAYOR, CONDITION_ALWAYS, EVENT_SCRIPT(Event_MayorAsksHelpForIvan) },
    { EVENT_TALK, ACTOR_MAYORS_WIFE, FLAG_VAULT_IVAN_JOINED_ROD_SEARCH, MSGNO(MsgVaultMyHusbandTheMayorCantBelieve) },
    { EVENT_TALK, ACTOR_MAYORS_WIFE, CONDITION_ALWAYS, MSGNO(MsgVaultReallyYoureGoingToHelpIvan) },
    { EVENT_TALK, ACTOR_BONE_GIVER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_WarnAboutThreeStrangers) },
    { EVENT_TALK, ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToWeaponMerchant) },
    { EVENT_TALK, ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToArmorMerchant) },
    { EVENT_TALK, ACTOR_ITEM_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToItemMerchant) },
    { EVENT_TALK, ACTOR_INN_STAFF, CONDITION_ALWAYS, MSGNO(MsgVaultTheGuestsUpstairsWerentInTheir) },
    { EVENT_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToInnkeeper) },
    { EVENT_TALK, ACTOR_SUSPECTED_LEADER, IF_FLAG_SET(FLAG_SCENE_TEMP_50), 0 },
    { EVENT_TALK, ACTOR_SUSPECTED_THIEF, IF_FLAG_SET(FLAG_SCENE_TEMP_50), 0 },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SUSPECTED_LEADER, IF_FLAG_SET(FLAG_SCENE_TEMP_50), 0 },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SUSPECTED_THIEF, IF_FLAG_SET(FLAG_SCENE_TEMP_50), 0 },
    { EVENT_TALK, ACTOR_SUSPECTED_LEADER, FLAG_VAULT_SUSPECTS_MIND_READ, MSGNO(MsgVaultThatKidIvanHeCreepsMe) },
    { EVENT_TALK, ACTOR_SUSPECTED_THIEF, FLAG_VAULT_SUSPECTS_MIND_READ, MSGNO(MsgVaultWeHaventDoneAnythingYoureA) },
    { EVENT_TALK, ACTOR_SUSPECTED_LEADER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SuspectedLeaderDodges) },
    { EVENT_TALK, ACTOR_SUSPECTED_THIEF, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SuspectedThiefDodges) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GEM_OWNER, CONDITION_ALWAYS, MSGNO(MsgVaultTheOtherSparklingRocksAllCrumbled) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GEM_OWNERS_WIFE, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReadMindOnRock) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WARY_NEIGHBOR, CONDITION_ALWAYS, MSGNO(MsgVaultHeyMaybeMyNeighborIsThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CHILD, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReadChildMind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GRANDPA, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReadMindOnThieves) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GRANDPAS_WIFE, CONDITION_ALWAYS, MSGNO(MsgVaultYouNeverBoughtMeAnythingWorth) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOULD_BE_HELPER, CONDITION_ALWAYS, MSGNO(MsgVaultTheseGuysLookReliableIGuess) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SECRET_KEEPER, CONDITION_ALWAYS, MSGNO(MsgVaultIWonderWhatSortOfThing) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAYOR, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReadMayorMind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAYORS_WIFE, CONDITION_ALWAYS, MSGNO(MsgVaultMaybeTheyllFindOurStolenFamily) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BONE_GIVER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReadBoneGiverMind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgVaultAllOurBestWeaponsWereStolen) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgVaultIWonderIfTheyLeftUs) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ITEM_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgVaultHahhWereSureToSeeA) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INN_STAFF, CONDITION_ALWAYS, MSGNO(MsgVaultItWouldBeAwfulIfOur) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, CONDITION_ALWAYS, MSGNO(MsgVaultThatVolcanoCostUsALot) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x400 | ACTOR_SUSPECTED_LEADER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SuspectedLeaderDodges) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x400 | ACTOR_SUSPECTED_THIEF, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SuspectedThiefDodges) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_CRATE, FLAG_VAULT_CRATE_MOVED, EVENT_SCRIPT(Event_CheckCrateMoved) },
    { SEARCH(SEARCH_UNNAMED), 41, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SearchEmptyBarrel) },
    { SEARCH(SEARCH_JAR), 100, FLAG_VAULT_INTERIORS_JAR_COINS, FIND_COINS(7) },
    { SEARCH(SEARCH_BARREL), 101, FLAG_VAULT_INTERIORS_BARREL_COINS, FIND_COINS(4) },
    { SEARCH(SEARCH_BARREL), 102, FLAG_VAULT_INTERIORS_BARREL_SLEEP_BOMB, FIND_ITEM(ITEM_SLEEP_BOMB) },
    { SEARCH(SEARCH_WOODEN_BOX), 103, FLAG_VAULT_INTERIORS_BOX_MINT, FIND_ITEM(ITEM_MINT) },
    { SEARCH(SEARCH_BOOKCASE) | EVENT_TOUCH_FACING | FACING_NORTH, 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgVaultTheresABookEntitledHistoryOf)) },
    { SEARCH(SEARCH_BOOKCASE), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgVaultTheresABookEntitledGemsOf)) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gVaultInteriors_LoftEvents[] = {
    { EVENT_TOUCH, 27, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveLoft) },
    { EVENT_TALK, ACTOR_LOFT_ROOFER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_FreeRooferAndConfrontThieves) },
    { EVENT_TALK, ACTOR_FIRST_CHEST, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SearchEmptyChest) },
    { EVENT_TALK, ACTOR_SECOND_CHEST, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SearchEmptyChest) },
    { EVENT_TALK, ACTOR_THIRD_CHEST, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SearchEmptyChest) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LOFT_ROOFER, CONDITION_ALWAYS, MSGNO(MsgVaultAhhHelpMe) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gVaultInteriors_EventsAfterThieves[] = {
    { SEARCH(SEARCH_UNNAMED), 41, CONDITION_ALWAYS, EVENT_SCRIPT(Vault_FindBone) },
    { EVENT_EXIT, 1, CONDITION_ALWAYS, VAULT_INTERIORS_TO_VAULT_EAST_SOUTHEAST },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, VAULT_INTERIORS_TO_VAULT_SOUTHEAST },
    { EVENT_EXIT, 3, CONDITION_ALWAYS, VAULT_INTERIORS_TO_VAULT_SOUTHWEST },
    { EVENT_EXIT, 4, CONDITION_ALWAYS, VAULT_INTERIORS_TO_VAULT_EAST },
    { EVENT_EXIT, 5, CONDITION_ALWAYS, VAULT_INTERIORS_TO_VAULT_NORTH_NORTHEAST },
    { EVENT_EXIT, 6, CONDITION_ALWAYS, VAULT_INTERIORS_TO_VAULT_NORTHWEST },
    { EVENT_EXIT, 7, CONDITION_ALWAYS, VAULT_INTERIORS_INN_UPSTAIRS_TO_INN },
    { EVENT_EXIT, 8, CONDITION_ALWAYS, VAULT_INTERIORS_INN_TO_INN_UPSTAIRS },
    { EVENT_TOUCH, 26, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ClimbToLoft) },
    { EVENT_TOUCH, 27, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveLoft) },
    { EVENT_TALK, ACTOR_GEM_OWNER, CONDITION_ALWAYS, MSGNO(MsgVaultICanSleepWellNowThat) },
    { EVENT_TALK, ACTOR_GEM_OWNERS_WIFE, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkAboutRocks) },
    { EVENT_TALK, ACTOR_WARY_NEIGHBOR, CONDITION_ALWAYS, EVENT_SCRIPT(Event_AskWhetherThievesCameFromLunpa) },
    { EVENT_TALK, ACTOR_CHILD, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ChildWorriesAboutFather) },
    { EVENT_TALK, ACTOR_GRANDPA, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkAboutMissingThings) },
    { EVENT_TALK, ACTOR_GRANDPAS_WIFE, CONDITION_ALWAYS, MSGNO(MsgVaultJustAsIThoughtNothingWas) },
    { EVENT_TALK, ACTOR_WOULD_BE_HELPER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_AskAboutDefeatingThieves) },
    { EVENT_TALK, ACTOR_SECRET_KEEPER, CONDITION_ALWAYS, MSGNO(MsgVaultIvanLearnedALotByTalking) },
    { EVENT_TALK, ACTOR_MAYOR, CONDITION_ALWAYS, EVENT_SCRIPT(Event_MayorGivesWaterOfLife) },
    { EVENT_TALK, ACTOR_MAYORS_WIFE, CONDITION_ALWAYS, MSGNO(MsgVaultYoureThePersonWhoGotOur) },
    { EVENT_TALK, ACTOR_BONE_GIVER, FLAG_SCENE_TEMP_50, EVENT_SCRIPT(Event_OfferBones) },
    { EVENT_TALK, ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToWeaponMerchant) },
    { EVENT_TALK, ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToArmorMerchant) },
    { EVENT_TALK, ACTOR_ITEM_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToItemMerchant) },
    { EVENT_TALK, ACTOR_INN_STAFF, CONDITION_ALWAYS, MSGNO(MsgVaultImShockedThoseGuestsOfOurs) },
    { EVENT_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToInnkeeper) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GEM_OWNER, CONDITION_ALWAYS, MSGNO(MsgVaultEverybodysAfterMyRockTheyWant) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GEM_OWNERS_WIFE, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReadMindOnRock) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WARY_NEIGHBOR, CONDITION_ALWAYS, MSGNO(MsgVaultTheReasonLunpaTurnedBadIs) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CHILD, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReadChildMind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GRANDPA, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReadMindOnThieves) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GRANDPAS_WIFE, CONDITION_ALWAYS, MSGNO(MsgVaultIWishIHadSomethingWorth) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOULD_BE_HELPER, CONDITION_ALWAYS, MSGNO(MsgVaultWhenIGrowUpIWant) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SECRET_KEEPER, CONDITION_ALWAYS, MSGNO(MsgVaultIWishIHadWingsLike) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAYOR, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReadMayorMind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAYORS_WIFE, CONDITION_ALWAYS, MSGNO(MsgVaultImSoRelievedNowThatWe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BONE_GIVER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReadBoneGiverMind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgVaultMyWeaponsWereFoundInThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgVaultItMustBeSnowingUpIn) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ITEM_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgVaultWhyAreThesePeopleSoAnxious) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INN_STAFF, CONDITION_ALWAYS, MSGNO(MsgVaultWhoCouldHaveKnownTheyWere) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, CONDITION_ALWAYS, MSGNO(MsgVaultOfCourseIAlwaysSuspectedThey) },
    { SEARCH(SEARCH_JAR), 100, FLAG_VAULT_INTERIORS_JAR_COINS, FIND_COINS(7) },
    { SEARCH(SEARCH_BARREL), 101, FLAG_VAULT_INTERIORS_BARREL_COINS, FIND_COINS(4) },
    { SEARCH(SEARCH_BARREL), 102, FLAG_VAULT_INTERIORS_BARREL_SLEEP_BOMB, FIND_ITEM(ITEM_SLEEP_BOMB) },
    { SEARCH(SEARCH_WOODEN_BOX), 103, FLAG_VAULT_INTERIORS_BOX_MINT, FIND_ITEM(ITEM_MINT) },
    { SEARCH(SEARCH_BOOKCASE) | EVENT_TOUCH_FACING | FACING_NORTH, 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgVaultTheresABookEntitledHistoryOf)) },
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
