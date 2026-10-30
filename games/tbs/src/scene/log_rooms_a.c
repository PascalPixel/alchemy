/// @file
/// Log rooms A (SceneId_LogRoomsA), a sheet of rooms drawn in logs, all of them
/// Vault's: the insides of the town's houses, shops and inn, and the inn's
/// loft, while Hammet's Shaman's Rod is missing and just after it is found.
/// Every room is on one map, each an entrance with a camera box of its
/// own, and Vault's doors (vault.c) lead to the entrances named for
/// them: the gem owner's house, the child's house, the inn and its guests' room
/// upstairs, the item shop, the weapon and armor shop, the mayor's house and a
/// room with a crate, behind which a way climbs to the inn's loft. The loft has
/// actor placements and events of its own
/// (LogRoomsA_GetActorPlacements and _GetTriggers), and
/// FLAG_VAULT_THIEVES_JAILED chooses the other rooms' tables.
///
/// The story flags drive the rooms in order:
/// - At the mayor's, talking to Ivan plays LogRoomsA_MeetIvan: he reads
/// Isaac's
///   mind (FLAG_VAULT_MET_IVAN), asks for help with the rod, and on a yes
///   joins the party (LogRoomsA_IvanJoinsRodSearch,
///   FLAG_VAULT_IVAN_JOINED_ROD_SEARCH).
/// - In the inn's guests' room the two suspects run from Ivan
///   (LogRoomsA_ApproachSuspects), and stepping out the party plans to corner
///   one (LogRoomsA_DiscussSuspiciousGuests). The chase
///   (LogRoomsA_StartCorneringSuspects) moves Ivan and the suspects along a
///   network of stops (gLogRoomsA_Stops); once one has been blocked for
///   half a second, LogRoomsA_UpdateActorStops raises
///   LOG_ROOMS_A_LEADER_CORNERED, _THIEF_CORNERED or _BOTH_CORNERED, and
///   Ivan reads the cornered suspect's mind
///   (LogRoomsA_ReadCorneredSuspectsMinds, FLAG_VAULT_SUSPECTS_MIND_READ).
///   Meanwhile the menus answer with Garet's refusal
///   (FLAG_MENUS_RAISE_TRIGGER).
/// - In the room with the crate, Move pushes the crate aside
///   (LogRoomsA_CheckCrateMoved, FLAG_VAULT_CRATE_MOVED), and the way behind
///   it climbs to the loft (LogRoomsA_ClimbToLoft). Freeing the villager tied
///   up there brings out the three thieves, who attack
///   (LogRoomsA_FreeRooferAndConfrontThieves, FLAG_VAULT_THIEVES_ATTACKED). A
///   loss returns to LOG_ROOMS_A_LOFT_AFTER_LOSING_TO_THIEVES and the party
///   comes to in the Vault Sanctum (LogRoomsA_LoseToThieves); coming back
///   up replays the challenge (LogRoomsA_ConfrontThievesAgain). A win
///   returns to LOG_ROOMS_A_LOFT_AFTER_THIEVES_DEFEATED
///   (LogRoomsA_DefeatThieves, LogRoomsA_ReturnStolenGoods): the
///   thieves are jailed (FLAG_VAULT_THIEVES_JAILED), the chests give back
///   the stolen goods and the Shaman's Rod, and Ivan leaves the party to
///   rescue Hammet from Lunpa.
/// - Once the thieves are jailed, the mayor gives the promised Water of Life
///   (FLAG_VAULT_WATER_OF_LIFE_RECEIVED) and the girl at the inn offers
///   Bones (FLAG_VAULT_BONE_RECEIVED), with more on the floor beside her
///   (Vault_FindBone) for the dog in town.
///
/// The shops and the inn open across their counters when the leader faces
/// north; otherwise their keepers speak. A jar, two barrels and a wooden box
/// hold coins, a Sleep Bomb and a Mint, each found once, and two bookcases can
/// be read. The cells the events answer are enum LogRoomsACellBehavior
/// (log_rooms_a.h).
///
/// LogRoomsA_ApproachSuspects plays once: it copies
/// CELL_BEHAVIOR_LOG_ROOMS_A_GUESTS_ROOM_DOOR cells over the guests' room's
/// CELL_BEHAVIOR_LOG_ROOMS_A_GUESTS_ROOM cells,
/// LogRoomsA_DiscussSuspiciousGuests makes them
/// CELL_BEHAVIOR_LOG_ROOMS_A_CORNERING_START, and every later arrival with
/// FLAG_VAULT_SUSPECTS_APPROACHED set rewrites them again (render probe
/// log-rooms-a: the room has no GUESTS_ROOM cell once the flag is set).

#include "main_routine_work.h"
#include "flags.h"
#include "sprite.h"
#include "version.h"
#include "scene_ids.h"
#include "module/import.h"
#include "module/scene/log_rooms_a.h"
#include "log_rooms_a.h"
#include "module/scene/shared/vault_item_icon.h"
#include "module/field/field_sequence.h"
#include "module/system/main_routine.h"
#include "module/field/event_commands.h"
#include "module/battle/battle.h"
#include "module/game/game.h"
#include "module/system/system.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "types.h"
#include "edition.h"
#include "far_call.h"
#include "sound.h"
#include "system.h"
#include "heap_release.h"
#include "edition.h"
#include "types.h"
#include "task.h"
#include "item_ids.h"
#include "event.h"
#include "scene_tables.h"
#include "tbs_edition.h"
#include "battle_side.h"
#include "text/msg_ids.h"
#include "event_inline.h"
#include "ability_ids.h"
#include "actor_script.h"
#include "fixed_math.h"
#include "macros.h"
#include "sounds.h"
#include "heap.h"
#include "sound_driver_symbols.h"

#if TBS_PATCH_3
#define LOG_ROOMS_A_ENTRY_ACTOR_Z PIXELS(451)
#else
#define LOG_ROOMS_A_ENTRY_ACTOR_Z PIXELS(464)
#endif

/// A cornering lasts half a second before the chase raises an event.
#define LOG_ROOMS_A_CORNERED_FRAMES 30

/// The chase adds the blocked-suspect mask (1 the leader, 2 the thief) to 200.
enum LogRoomsARaisedTrigger {
  LOG_ROOMS_A_CORNERED_EVENT_BASE = 200,
  LOG_ROOMS_A_LEADER_CORNERED,
  LOG_ROOMS_A_THIEF_CORNERED,
  LOG_ROOMS_A_BOTH_CORNERED
};

/// The townspeople, named by what they say, numbering
/// gLogRoomsA_ActorPlacements' rows from ACTOR_FIRST_PLACED;
/// Garet's and Ivan's rows take their own slots.
enum {
  /// A young man, to whose house a sparkling rock was blown from the volcano.
  /// ★ Named from the game: "I kept worrying that they would take my gem."
  /// (MsgVaultICanSleepWellNowThat)
  ACTOR_GEM_OWNER = 8,
  /// A woman with fair hair.
  /// ★ Named from the game: "My husband says my cat ate the treasure"
  /// (MsgVaultMyHusbandSaysMyCatAte)
  ACTOR_GEM_OWNERS_WIFE = 9,
  /// A burly grey-haired man who distrusts the man next door. "My neighbor
  /// creeps me out."
  /// ★ Named from the game: "Maybe my neighbor is the thief..."
  /// (MsgVaultHeyMaybeMyNeighborIsThe)
  ACTOR_VILLAGER_SUSPECTING_THE_NEIGHBOR = 10,
  /// A small boy who asks about travel.
  /// ★ Named from the game: "Hey, mister! Is it fun to see strange new places?"
  /// (MsgVaultMisterFunSeeStrangeNew)
  ACTOR_BOY_ASKING_ABOUT_TRAVEL = 11,
  /// An old man with a cane who wonders what was stolen.
  /// ★ Named from the game: "Is it just me, or am I missing something?"
  /// (MsgVaultJustMeOrAmMissing)
  ACTOR_GRANDPA = 12,
  /// An old woman, who says Grandpa keeps wondering if anything was stolen.
  /// ★ Named from the game: "You never bought me anything worth stealing!"
  /// (MsgVaultYouNeverBoughtMeAnythingWorth)
  ACTOR_GRANDPAS_WIFE = 13,
  /// A small boy. "I could help Ivan if I were just a little older."
  /// ★ Named from the game: "if I were just a little older."
  /// (MsgVaultICouldHelpIvanIfI)
  ACTOR_BOY_WISHING_TO_BE_OLDER = 14,
  /// A little girl who tells the party Ivan's secret once it helps him.
  /// ★ Named from the game: "I'll tell you a secret..."
  /// (MsgVaultSinceYoureHelpingIvanIllTell)
  ACTOR_GIRL_TELLING_A_SECRET = 15,
  /// Ivan's host, an old man with white hair.
  /// ★ Named from the game: "My husband, the mayor, can't believe anyone from
  /// Vault could be the thief." (MsgVaultMyHusbandTheMayorCantBelieve)
  ACTOR_MAYOR = 16,
  /// A woman in a green dress.
  /// ★ Named from the game: "My husband, the mayor, can't believe anyone from
  /// Vault could be the thief." (MsgVaultMyHusbandTheMayorCantBelieve)
  ACTOR_MAYORS_WIFE = 17,
  /// The girl at the inn, as the Bone's description calls her, a young woman
  /// with fair hair. "I have a lot
  /// of leftover bones from dinner."
  /// ★ Named from the game: "A bone from the girl at the inn"
  /// (MsgItemPlainName)
  ACTOR_GIRL_AT_THE_INN = 18,
  /// ★ Named from the game: "I'm sorry I've only got these cheap weapons to
  /// sell right now." (MsgVaultAllOurBestWeaponsWereStolen)
  ACTOR_WEAPON_MERCHANT = 19,
  /// ★ Named from the game: "I wonder if they left us alone because we don't
  /// carry any valuable armor?" (MsgVaultIWonderIfTheyLeftUs)
  ACTOR_ARMOR_MERCHANT = 20,
  /// ★ Named from what the player sees: the shopkeeper behind a counter of
  /// jars and sacks (its render)
  ACTOR_ITEM_MERCHANT = 21,
  /// A burly grey-haired man who works at the inn.
  /// ★ Named from the game: "The guests upstairs weren't in their room during
  /// the eruption." (MsgVaultTheGuestsUpstairsWerentInTheir)
  ACTOR_INN_STAFF = 22,
  /// A woman in an apron behind the inn's counter.
  /// ★ Named from the game: "Everyone thinks our guests are the thieves"
  /// (MsgVaultEveryoneThinksOurGuestsThieves)
  ACTOR_INNKEEPER = 23,
  /// The two guests at the inn the town suspects: the thieves' leader and one
  /// of his men.
  /// ★ Named from the game: "Everyone thinks our guests are the thieves"
  /// (MsgVaultEveryoneThinksOurGuestsThieves)
  ACTOR_SUSPECTED_LEADER = 24,
  /// ★ Named from the game: "You're a terrible person if you start accusing
  /// people for no good reason." (MsgVaultWeHaventDoneAnythingYoureA)
  ACTOR_SUSPECTED_THIEF = 25,
  /// Pushed aside to open a passage.
  /// ★ Named from what the player sees: a large wooden crate in the room below
  /// the loft (its render)
  ACTOR_CRATE = 26
};

/// Area flags of Vault's area (0x300 to 0x3ff, kept while the
/// party stays in Vault and its buildings).
enum {
  /// Set while the two suspects run from the party, until the party steps
  /// aside to talk it over.
  /// ★ Named from the game: "Don't let him near me!" (MsgVaultWant)
  FLAG_SUSPECTS_FLEEING = FLAG_AREA_TEMP_00
};

// The scene flag the inn keeps as FLAG_VAULT_LEADER_BESIDE_BONE
// (shared/vault_bone_proximity_flag.h) once the thieves are jailed means
// something else in the guests' room:
/// The party is cornering the two suspects in the guests' room, so talking to
/// Ivan or the suspects does nothing.
/// ★ Named from the game: "Uh-oh! I'm surrounded!" (MsgVaultImSurrounded)
#define FLAG_VAULT_CORNERING_SUSPECTS FLAG_SCENE_TEMP_50

/// The inn's loft (LOG_ROOMS_A_LOFT_FROM_CRATE_ROOM and the two after the
/// battle), where the thieves hid the stolen goods, numbering
/// gLogRoomsA_LoftActorPlacements' rows from ACTOR_FIRST_PLACED.
enum {
  /// vault.c's ACTOR_YOUNG_MAN_FIXING_THE_ROOF, tied up while fixing the
  /// roof.
  /// ★ Named from the game: "When I came to, I was all tied up."
  /// (MsgVaultVaultCutFree)
  ACTOR_TIED_UP_VILLAGER = 8,
  /// ★ Named from the game: "Mayor... Isn't that your family's urn?"
  /// (MsgVaultYouIsaacRightWontForget)
  ACTOR_LOFT_MAYOR = 9,
  /// Speaks first and bargains for all three ("See, we're not so bad. How
  /// about it? Will you let us go?").
  /// ★ Named from the game: "Looks like we've been found out!"
  /// (MsgVaultVaultCutFree)
  ACTOR_THIEF_LEADER = 10,
  /// ★ Named from the game: "These are the thieves, Mayor..."
  /// (MsgVaultSeeThatsHappened)
  ACTOR_FIRST_THIEF = 11,
  /// ★ Named from the game: "These are the thieves, Mayor..."
  /// (MsgVaultSeeThatsHappened)
  ACTOR_SECOND_THIEF = 12,
  /// A strongman who comes up with the mayor and helps tie the thieves up.
  /// ★ Named from the game: "What should we do with them?"
  /// (MsgVaultSeeThatsHappened)
  ACTOR_LOFT_ESCORT = 13,
  /// A strongman who comes up with the mayor and opens the chests.
  /// ★ Named from the game: "Mayor, we found another chest over here"
  /// (MsgVaultYouIsaacRightWontForget)
  ACTOR_STRONGMAN_OPENING_THE_CHESTS = 14,
  /// ★ Named from the game: "but the chest was empty."
  /// (MsgVaultButChestWasEmpty)
  ACTOR_FIRST_CHEST = 15,
  /// ★ Named from the game: "but the chest was empty."
  /// (MsgVaultButChestWasEmpty)
  ACTOR_SECOND_CHEST = 16,
  /// ★ Named from the game: "but the chest was empty."
  /// (MsgVaultButChestWasEmpty)
  ACTOR_THIRD_CHEST = 17,
  /// Rises from the first chest.
  /// ★ Named from the game: "Found the mayor's precious urn."
  /// (MsgVaultYouIsaacRightWontForget)
  ACTOR_FOUND_URN = 18,
  /// Rises from the second chest, stolen from the Sanctum.
  /// ★ Named from the game: "Found a gold statue."
  /// (MsgVaultYouIsaacRightWontForget)
  ACTOR_FOUND_STATUE = 19
};

static void LogRoomsA_IvanJoinsRodSearch(void);
static void LogRoomsA_DiscussMindReading(void);
static void LogRoomsA_AdvanceSuspectRoute(s32 actorId, s32 expected, s32 next,
                                          const s32 *actions);
static void LogRoomsA_FaceSuspectsTowardLeader(void);
static void LogRoomsA_LoseToThieves(void);
static void LogRoomsA_ConfrontThievesAgain(void);
static void LogRoomsA_DefeatThieves(void);
static void LogRoomsA_ReturnStolenGoods(void);
static void LogRoomsA_OpenWindowTransition(void);
static void LogRoomsA_ShowMessageAndWait(s32 actorId, s32 waitFrames);
static void LogRoomsA_FaceActorAndWait(s32 actorId, s32 targetId,
                                       s32 waitFrames);
static void LogRoomsA_FaceEachOtherAndWait(s32 firstId, s32 secondId,
                                           s32 waitFrames);
static void LogRoomsA_PlayAnimationAndWait(s32 actorId, s32 animation,
                                           s32 waitFrames);
static void LogRoomsA_StartMindRead(s32 casterId, s32 targetId);
static void LogRoomsA_EndMindRead(void);
static void LogRoomsA_UpdateActorStops(void);
static void LogRoomsA_StartActorStops(void);

const struct SceneEntrance *Scene_GetEntrances(void) {
  return gLogRoomsA_Entrances;
}

/// Returns the one exit region: the box around (648, 416) in the room with the
/// crate, which takes LOG_ROOMS_A_TO_VAULT_WEST back out to Vault.
const struct SceneExitRegion *Scene_GetExitRegions(void) {
  return gLogRoomsA_Regions;
}

const u32 *Scene_GetExits(void) { return gLogRoomsA_Exits; }

/// Places the townspeople, or, for the inn's loft
/// (LOG_ROOMS_A_LOFT_FROM_CRATE_ROOM and the two after the battle), the
/// tied-up villager, the mayor, the thieves, their chests and the found
/// treasures.
const struct ActorPlacement *Scene_GetActorPlacements(void) {
  struct ActorPlacement *table;

  switch (gMainRoutineWork.entrance) {
  case LOG_ROOMS_A_LOFT_FROM_CRATE_ROOM:
  case LOG_ROOMS_A_LOFT_AFTER_LOSING_TO_THIEVES:
  case LOG_ROOMS_A_LOFT_AFTER_THIEVES_DEFEATED:
    table = gLogRoomsA_LoftActorPlacements;
    break;
  default:
    table = gLogRoomsA_ActorPlacements;
    break;
  }
  Scene_AssignViewFlags(table);
  return table;
}

#if !TBS_PATCH_1
/// Jumps the leader to the requested destination when nothing blocks it there;
/// returns 1 when it jumped. Only the editions before the first patch have it;
/// the patched ones look ahead instead (Field_TryJumpForward).
static s32 LogRoomsA_TryJumpTo(struct FieldPosition *position) {
  struct Actor *leader;
  u8 *flags;
  u8 savedMotionFlags;

  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  flags = &leader->motionFlags;
  savedMotionFlags = *flags;
  if (FieldActor_CheckMovementCollision(leader, position) == 0) {
    Event_Start();
    Actor_SelectAnimation(leader, 6);
    wait(6);
    Sound_Play(SE_JUMP);
    Actor_SelectAnimation(leader, 7);
    leader->speedLimit = PIXELS(3);
    leader->acceleration = PIXELS(2);
    leader->velocityY = PIXELS(4);
    *flags &= 0x7e;
    Actor_SetSpriteFlags(leader, 0);
    Actor_SetPositionAndCommit(
        ACTOR_PARTY_LEADER, ((union FieldCoordinate *)&position->x)->part.pixel,
        ((union FieldCoordinate *)&position->z)->part.pixel);
    Actor_SelectAnimation(leader, 6);
    Actor_SetSpriteFlags(leader, SPRITE_FLAG_SHADOW);
    wait(6);
    *flags = savedMotionFlags;
    Event_End();
    return 1;
  }
  return 0;
}
#endif

/// Indoors in Vault, facing north or south across the gap in the roof, the
/// leader jumps it.
///
/// Reached by walking onto CELL_BEHAVIOR_LOG_ROOMS_A_ROOF_GAP. Then copies the
/// cell (41, 85) or (43, 85) over the gap's cell (42, 85), whichever side the
/// leader faced.
static void LogRoomsA_JumpAcrossRoofGap(void) {
  s32 x;
  s32 z;
#if !TBS_PATCH_1
  struct FieldPosition target;
  struct FieldPosition *position;
  struct Actor *leader;
#endif

  if (Actor_GetById(ACTOR_PARTY_LEADER)->facing >= FACING_NORTHWEST &&
      Actor_GetById(ACTOR_PARTY_LEADER)->facing <= FACING_NORTHEAST) {
#if TBS_PATCH_1
    Field_TryJumpForward();
#else
    leader = Actor_GetById(ACTOR_PARTY_LEADER);
    position = &target;
    SET_POSITION(position, leader->x.fixed,
                 Actor_GetById(ACTOR_PARTY_LEADER)->y.fixed,
                 Actor_GetById(ACTOR_PARTY_LEADER)->z.fixed - PIXELS(30));
    LogRoomsA_TryJumpTo(position);
#endif
    x = 42;
    z = 85;
    MapGrid_CopyCellAttributes(41, 85, 1, 1, x, z);
  } else if (Actor_GetById(ACTOR_PARTY_LEADER)->facing >= FACING_SOUTHEAST &&
             Actor_GetById(ACTOR_PARTY_LEADER)->facing <= FACING_SOUTHWEST) {
#if TBS_PATCH_1
    Field_TryJumpForward();
#else
    leader = Actor_GetById(ACTOR_PARTY_LEADER);
    position = &target;
    SET_POSITION(position, leader->x.fixed,
                 Actor_GetById(ACTOR_PARTY_LEADER)->y.fixed,
                 Actor_GetById(ACTOR_PARTY_LEADER)->z.fixed + PIXELS(30));
    LogRoomsA_TryJumpTo(position);
#endif
    x = 42;
    z = 85;
    MapGrid_CopyCellAttributes(43, 85, 1, 1, x, z);
  }
}

/// Once Move pushes the crate aside, the way up to the inn's loft opens.
/// This is in Vault's room with the crate: when Move has pushed the crate to
/// column 42, the passage behind it opens.
///
/// Reached by Move on the crate (ACTOR_CRATE) until FLAG_VAULT_CRATE_MOVED is
/// set; sets it when the crate has reached column 42.
static void LogRoomsA_CheckCrateMoved(void) {
  struct Actor *crate;

  Event_Start();
  crate = Actor_GetById(ACTOR_CRATE);
  if ((crate->x.fixed >> 20) == 42) {
    MapGrid_CopyCellAttributes(101, 24, 3, 4, 41, 24);
    GameFlag_Set(FLAG_VAULT_CRATE_MOVED);
  }
  Event_End();
}

/// In Vault's weapon and armor shop, across the counter the weapon shop opens;
/// otherwise the merchant lost expensive goods too, or found the stolen
/// weapons rusted in the cemetery.
///
/// Reached by talking to the weapon merchant (ACTOR_WEAPON_MERCHANT), before
/// and after the thieves are jailed (FLAG_VAULT_THIEVES_JAILED picks the line).
/// The armor merchant, the inn and the item shop below work the same way: the
/// counter is between them when the leader faces north (strictly between
/// FACING_NORTHWEST and FACING_NORTHEAST).
static void LogRoomsA_TalkToWeaponMerchant(void) {
  s32 leaderFacing = Actor_GetById(ACTOR_PARTY_LEADER)->facing;

  Event_Start();
  if (leaderFacing > FACING_NORTHWEST && leaderFacing < FACING_NORTHEAST) {
    Shop_Run(4, ACTOR_WEAPON_MERCHANT);
  } else {
    if (GameFlag_Get(FLAG_VAULT_THIEVES_JAILED) == 0) {
      Event_SetMessageId(MsgVaultMasterHammetIsntOnlyOne);
    } else {
      Event_SetMessageId(MsgVaultWeFoundOurStolenWeapons);
    }
    Event_ShowMessageAndWait(ACTOR_WEAPON_MERCHANT, 0);
  }
  Event_End();
}

/// In Vault's weapon and armor shop, across the counter the armor shop opens;
/// otherwise the merchant wants the thieves to pay, or advises preparing for
/// the cave in the Goma Range.
///
/// Reached by talking to the armor merchant (ACTOR_ARMOR_MERCHANT).
static void LogRoomsA_TalkToArmorMerchant(void) {
  s32 leaderFacing = Actor_GetById(ACTOR_PARTY_LEADER)->facing;

  Event_Start();
  if (leaderFacing > FACING_NORTHWEST && leaderFacing < FACING_NORTHEAST) {
    Shop_Run(5, ACTOR_ARMOR_MERCHANT);
  } else {
    if (GameFlag_Get(FLAG_VAULT_THIEVES_JAILED) == 0) {
      Event_SetMessageId(MsgVaultThievesDidntHitOurHouse);
    } else {
      Event_SetMessageId(MsgVaultIfYoureGonnaHeadInto);
    }
    Event_ShowMessageAndWait(ACTOR_ARMOR_MERCHANT, 0);
  }
  Event_End();
}

/// At Vault's inn, across the counter the inn opens; otherwise the innkeeper
/// defends the guests everyone suspects, or learns the goods were hidden in
/// his loft.
///
/// Reached by talking to the innkeeper (ACTOR_INNKEEPER).
static void LogRoomsA_TalkToInnkeeper(void) {
  s32 leaderFacing = Actor_GetById(ACTOR_PARTY_LEADER)->facing;

  Event_Start();
  if (leaderFacing > FACING_NORTHWEST && leaderFacing < FACING_NORTHEAST) {
    Inn_CheckIn(1, ACTOR_INNKEEPER);
  } else {
    if (GameFlag_Get(FLAG_VAULT_THIEVES_JAILED) == 0) {
      Event_SetMessageId(MsgVaultEveryoneThinksOurGuestsThieves);
    } else {
      Event_SetMessageId(MsgVaultTheyHidThoseStolenGoods);
    }
    Event_ShowMessageAndWait(ACTOR_INNKEEPER, 0);
  }
  Event_End();
}

#include "shared/vault_bone.h"

/// At Vault's inn, while the party corners the suspects, Garet refuses to stop
/// for anything else. "We don't have time for that, Isaac!"
///
/// Reached through FIELD_RAISED_TRIGGER_MENUS, which the field raises in place
/// of a menu or a shortcut while FLAG_MENUS_RAISE_TRIGGER is set.
static void LogRoomsA_GaretRefusesToLeave(void) {
#if TBS_PATCH_1
  Leader_StopMoving();
#endif
  Event_SetMessageId(MsgVaultWeDontHaveTimeFor);
  Event_ShowMessageAndWait(ACTOR_GARET, 0);
}

/// At Vault's inn, Isaac checks the barrel but doesn't find anything.
///
/// Reached by searching CELL_BEHAVIOR_LOG_ROOMS_A_INN_BARREL before the thieves
/// are jailed; after, the same spot holds the Bone (Vault_FindBone).
static void LogRoomsA_SearchEmptyBarrel(void) {
  Event_Start();
  FieldMessage_Show(MsgVaultIsaacCheckedBarrel, 1);
  FieldMessage_Show(MsgVaultButDidntFindAnything, 1);
  Event_End();
}

/// In the inn's loft in Vault, Isaac checks the chest, but it is empty.
///
/// Reached by talking to one of the loft's three chests (ACTOR_FIRST_CHEST to
/// ACTOR_THIRD_CHEST).
static void LogRoomsA_SearchEmptyChest(void) {
  Event_Start();
  FieldMessage_Show(MsgVaultIsaacCheckedChest, 1);
  FieldMessage_Show(MsgVaultButChestWasEmpty, 1);
  Event_End();
}

/// Returns the loft's events for the loft's three entrances, else the
/// houses' events, which change once FLAG_VAULT_THIEVES_JAILED is set.
const struct Trigger *Scene_GetTriggers(void) {
  switch (gMainRoutineWork.entrance) {
  case LOG_ROOMS_A_LOFT_FROM_CRATE_ROOM:
  case LOG_ROOMS_A_LOFT_AFTER_LOSING_TO_THIEVES:
  case LOG_ROOMS_A_LOFT_AFTER_THIEVES_DEFEATED:
    return gLogRoomsA_LoftTriggers;
  }
  if (GameFlag_Get(FLAG_VAULT_THIEVES_JAILED) != 0) {
    return gLogRoomsA_TriggersAfterThieves;
  }
  return gLogRoomsA_Triggers;
}

/// Stands the speaker, faces the leader, and shows the selected message.
static void LogRoomsA_StandAndShowMessage(s32 x) {
  Import_ActorSetAnimation(x, ANIM_STAND);
  LogRoomsA_FaceEachOtherAndWait(x, 0, 2);
  Event_ShowMessageAndWait(x, 0);
}

#include "shared/vault_talk.h"

/// Faces the leader and selects the next reply when the answer is no.
static void LogRoomsA_AskYesNoAndShowReply(s32 x) {
  LogRoomsA_FaceEachOtherAndWait(x, 0, 2);
  FieldMessage_ShowAtActor(x, 0);
  if (Event_AskYesNoWithActorAnimation(0, 0) != 0) {
    gFieldWork->message++;
  }
  Event_ShowMessageAndWait(x, 0);
}

/// In a house of Vault, the small boy asks about travel.
/// "Hey, mister! Is it fun to see strange new places?"
///
/// Reached by talking to the boy (ACTOR_BOY_ASKING_ABOUT_TRAVEL) before the
/// thieves are jailed.
static void LogRoomsA_BoyAsksAboutTravel(void) {
  Event_Start();
  Event_SetMessageId(MsgVaultMisterFunSeeStrangeNew);
  Import_ActorSetAnimation(ACTOR_BOY_ASKING_ABOUT_TRAVEL, ANIM_STAND);
  LogRoomsA_AskYesNoAndShowReply(ACTOR_BOY_ASKING_ABOUT_TRAVEL);
  Event_End();
}

/// At the mayor's in Vault, a little girl offers to tell Ivan's secret.
/// "Ivan has great powers. Wouldn't you like to know what they are?"
///
/// Reached by talking to ACTOR_GIRL_TELLING_A_SECRET until
/// FLAG_VAULT_IVAN_JOINED_ROD_SEARCH is set; then the row after it tells the
/// secret: Ivan can talk with animals.
static void LogRoomsA_OfferIvansSecret(void) {
  Event_Start();
  Event_SetMessageId(MsgVaultIvanHasGreatPowersWouldnt);
  LogRoomsA_AskYesNoAndShowReply(ACTOR_GIRL_TELLING_A_SECRET);
  Actor_FaceDirection(ACTOR_GIRL_TELLING_A_SECRET, FACING_WEST, 0);
  Event_End();
}

/// In Vault's item shop, across the counter the shop opens; otherwise a
/// greeting, or, after the thieves are jailed, the bridge is out.
/// "Did you just arrive in town today?"
///
/// Reached by talking to the item merchant (ACTOR_ITEM_MERCHANT).
static void LogRoomsA_TalkToItemMerchant(void) {
  s32 leaderFacing = Actor_GetById(ACTOR_PARTY_LEADER)->facing;

  Event_Start();
  if (leaderFacing > FACING_NORTHWEST && leaderFacing < FACING_NORTHEAST) {
    Shop_Run(6, ACTOR_ITEM_MERCHANT);
  } else if (GameFlag_Get(FLAG_VAULT_THIEVES_JAILED) == 0) {
    Event_SetMessageId(MsgVaultDidJustArriveInTown);
    LogRoomsA_AskYesNoAndShowReply(ACTOR_ITEM_MERCHANT);
  } else {
    Event_SetMessageId(MsgVaultWithBridgeOutWillQuite);
    Event_ShowMessageAndWait(ACTOR_ITEM_MERCHANT, 0);
  }
  Event_End();
}

/// The wife of the man with the sparkling rock talks about their rocks.
/// She wishes their rocks were gems, or, after the thieves are jailed, is glad
/// they were caught before stealing the gem.
///
/// Reached by talking to the gem owner's wife (ACTOR_GEM_OWNERS_WIFE).
static void LogRoomsA_TalkAboutRocks(void) {
  Event_Start();
  if (GameFlag_Get(FLAG_VAULT_THIEVES_JAILED) == 0) {
    Event_SetMessageId(MsgVaultIfOnlyTheseRocksWere);
  } else {
    Event_SetMessageId(MsgVaultThankGoodnessThoseThievesWere);
  }
  LogRoomsA_StandAndShowMessage(ACTOR_GEM_OWNERS_WIFE);
  Event_End();
}

/// Grandpa in the item shop talks about missing things.
/// After the thieves are jailed, he says instead that they hid the treasure in
/// the loft and more in the cemetery.
/// "Is it just me, or am I missing something?"
///
/// Reached by talking to Grandpa (ACTOR_GRANDPA).
static void LogRoomsA_TalkAboutMissingThings(void) {
  Event_Start();
  if (GameFlag_Get(FLAG_VAULT_THIEVES_JAILED) != 0) {
    Event_SetMessageId(MsgVaultThievesHidStolenTreasureIn);
  } else {
    Event_SetMessageId(MsgVaultJustMeOrAmMissing);
  }
  LogRoomsA_StandAndShowMessage(ACTOR_GRANDPA);
  Event_End();
}

/// At his house in Vault, the mayor asks for someone to help Ivan; once the
/// party has agreed, he thanks them the first time and asks them to look after
/// Ivan after that.
///
/// Reached by talking to the mayor (ACTOR_MAYOR) before the thieves are
/// jailed. Sets FLAG_VAULT_MAYOR_THANKED_PARTY with the thanks.
static void LogRoomsA_MayorAsksHelpForIvan(void) {
  s32 thankedForHelpingFlag;
  thankedForHelpingFlag = FLAG_VAULT_MAYOR_THANKED_PARTY;
  Event_Start();
  if (GameFlag_Get(FLAG_VAULT_IVAN_JOINED_ROD_SEARCH) != 0) {
    if (GameFlag_Get(thankedForHelpingFlag) == 0) {
      Event_SetMessageId(MsgVaultYoureGoingHelpIvan);
      LogRoomsA_StandAndShowMessage(ACTOR_MAYOR);
      Event_Wait(10);
      LogRoomsA_PlayAnimationAndWait(ACTOR_MAYOR, ANIM_NOD, 20);
      GameFlag_Set(thankedForHelpingFlag);
    } else {
      Event_SetMessageId(MsgVaultPleaseLookAfterIvan);
    }
  } else {
    Event_SetMessageId(MsgVaultCouldSomeonePleaseHelpIvan);
  }
  LogRoomsA_StandAndShowMessage(ACTOR_MAYOR);
  Event_End();
}

/// At Vault's inn, the girl at the inn says the three strangers have fooled
/// the innkeeper and his wife.
///
/// Reached by talking to the girl at the inn (ACTOR_GIRL_AT_THE_INN) before
/// the thieves are jailed.
static void LogRoomsA_WarnAboutThreeStrangers(void) {
  Event_Start();
  Event_SetMessageId(MsgVaultThoseThreeStrangersSureHave);
  LogRoomsA_StandAndShowMessage(ACTOR_GIRL_AT_THE_INN);
  Event_End();
}

/// The suspected leader cries out and runs to his next spot.
/// This is in the guests' room at Vault's inn; the spot is chosen by the way he
/// faces and where he is on his route.
/// "Ow! Stop it!"
///
/// Reached by Mind Read on him (ACTOR_SUSPECTED_LEADER), and by talking to
/// him once FLAG_VAULT_SUSPECTS_MIND_READ is set (before, he only says Ivan
/// creeps him out); neither while the party corners him. His timer holds his
/// place on the six-step route, wrapping from 5 to 0.
static void LogRoomsA_SuspectedLeaderDodges(void) {
  struct Actor *suspect;
  s16 *routeState;
  s32 routeIndex;
  s32 nextRoute;

  suspect = Actor_GetById(ACTOR_SUSPECTED_LEADER);
  Event_Start();
  Actor_RunRepeatedMotion(ACTOR_SUSPECTED_LEADER, 2);
  Event_SetMessageId(MsgVaultOwStop);
  Event_ShowMessageAndWait(ACTOR_SUSPECTED_LEADER, 0);
  Actor_SetSpeedParameters(ACTOR_SUSPECTED_LEADER, PIXELS(4), PIXELS(2));
  if ((u32)((suspect->facing & 0xf000) - TRIG_TURN * 5 / 16) <=
      TRIG_TURN * 3 / 8) {
    routeState = (s16 *)&suspect->timer;
    routeIndex = *routeState;
    if (routeIndex <= 2) {
      Actor_SetScript(ACTOR_SUSPECTED_LEADER,
                      gLogRoomsA_StepActions[routeIndex]);
      *(u16 *)routeState = *(u16 *)routeState + 1;
    } else {
      Actor_SetScript(ACTOR_SUSPECTED_LEADER,
                      gLogRoomsA_IdleActions[routeIndex]);
      nextRoute = *(u16 *)routeState - 1;
      *(u16 *)routeState = nextRoute;
    }
  } else {
    routeState = (s16 *)&suspect->timer;
    routeIndex = *routeState;
    if (routeIndex > 2) {
      Actor_SetScript(ACTOR_SUSPECTED_LEADER,
                      gLogRoomsA_StepActions[routeIndex]);
      *(u16 *)routeState = *(u16 *)routeState + 1;
    } else {
      Actor_SetScript(ACTOR_SUSPECTED_LEADER,
                      gLogRoomsA_IdleActions[routeIndex]);
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
  Actor_RefreshSelectorById(ACTOR_SUSPECTED_LEADER);
  Event_End();
}

/// Selects a response script and advances the second suspect's route state.
static inline void LogRoomsA_ApplySecondSuspectRoute(struct Actor *actor,
                                                     s32 scriptRow,
                                                     s32 routePair,
                                                     u16 routeDelta) {
  Actor_SetScript(ACTOR_SUSPECTED_THIEF,
                  gLogRoomsA_ResponseScripts[scriptRow * 4 + actor->timer]);
  actor->timer = actor->timer - routePair * 2 + routeDelta;
}

/// In the guests' room at Vault's inn, the suspected thief cries out and runs
/// to his next spot. "No! Leave me alone!"
///
/// Reached as LogRoomsA_SuspectedLeaderDodges is, on ACTOR_SUSPECTED_THIEF.
/// His timer holds his place on his route (4 at its start).
static void LogRoomsA_SuspectedThiefDodges(void) {
  struct Actor *actor;
  u32 facing;
  s32 routePair;

  actor = Actor_GetById(ACTOR_SUSPECTED_THIEF);
  facing = actor->facing & 0xf000;
  routePair = actor->timer >> 1;
  Event_Start();
  Actor_RunRepeatedMotion(ACTOR_SUSPECTED_THIEF, 2);
  Event_SetMessageId(MsgVaultNoLeaveAlone);
  Event_ShowMessageAndWait(ACTOR_SUSPECTED_THIEF, 0);
  Actor_SetSpeedParameters(ACTOR_SUSPECTED_THIEF, PIXELS(7) / 2, PIXELS(7) / 4);
  switch (actor->timer) {
  case 4:
    if (facing > FACING_SOUTHEAST && facing < FACING_NORTHWEST) {
      Actor_SetScript(ACTOR_SUSPECTED_THIEF,
                      gLogRoomsA_SuspectAlongToSouthEastCorner);
      actor->timer = 2;
    } else {
      Actor_SetScript(ACTOR_SUSPECTED_THIEF,
                      gLogRoomsA_SuspectAlongToSouthWestCorner);
      actor->timer = 3;
    }
    break;
  case 0:
  case 2:
    if (facing > FACING_SOUTHEAST && facing < FACING_NORTHWEST) {
      LogRoomsA_ApplySecondSuspectRoute(actor, routePair, routePair, 1);
    } else {
      LogRoomsA_ApplySecondSuspectRoute(actor, routePair ^ 1, routePair, -1);
    }
    break;
  case 1:
  case 3:
    if (facing > FACING_SOUTHWEST && facing < FACING_NORTHEAST) {
      LogRoomsA_ApplySecondSuspectRoute(actor, routePair, routePair, 1);
    } else {
      LogRoomsA_ApplySecondSuspectRoute(actor, routePair ^ 1, routePair, -1);
    }
    break;
  }
  actor->timer &= 3;
  Actor_RefreshSelectorById(ACTOR_SUSPECTED_THIEF);
  Event_End();
}

/// The villager suspecting the neighbor asks whether the thieves came from
/// Lunpa.
/// This is in a house of Vault after the thieves are jailed. "Did those thieves
/// come from Lunpa?"
///
/// Reached by talking to the villager suspecting the neighbor
/// (ACTOR_VILLAGER_SUSPECTING_THE_NEIGHBOR) after the thieves are jailed; the
/// boy and ACTOR_BOY_WISHING_TO_BE_OLDER below are reached the same way.
static void LogRoomsA_AskWhetherThievesCameFromLunpa(void) {
  Event_Start();
  Event_SetMessageId(MsgVaultDidThoseThievesComeFrom);
  LogRoomsA_AskYesNoAndShowReply(ACTOR_VILLAGER_SUSPECTING_THE_NEIGHBOR);
  Event_End();
}

/// The boy says Father worries about the prisoners from Lunpa.
/// He's in a house of Vault, and says it after the thieves are jailed.
static void LogRoomsA_BoyWorriesAboutFather(void) {
  Event_Start();
  Event_SetMessageId(MsgVaultMyFatherWorriedAboutThose);
  LogRoomsA_StandAndShowMessage(ACTOR_BOY_ASKING_ABOUT_TRAVEL);
  Event_End();
}

/// At the mayor's in Vault after the thieves are jailed, a villager asks about
/// defeating them. "I heard you defeated those thieves!"
static void LogRoomsA_AskAboutDefeatingThieves(void) {
  Event_Start();
  Event_SetMessageId(MsgVaultHeardDefeatedThoseThieves);
  LogRoomsA_AskYesNoAndShowReply(ACTOR_BOY_WISHING_TO_BE_OLDER);
  Event_End();
}

/// The mayor thanks Isaac and gives the promised Water of Life.
/// This is at his house after the thieves are jailed, and he gives it if there
/// is room; then he asks whether the party is heading beyond the Goma Range,
/// and points them to the cave on the way to Bilibin.
///
/// Reached by talking to the mayor (ACTOR_MAYOR) after the thieves are
/// jailed. Gives the Water of Life and sets FLAG_VAULT_WATER_OF_LIFE_RECEIVED,
/// once; with a full bag he says to come back for it.
static void LogRoomsA_MayorGivesWaterOfLife(void) {
  Event_Start();
  if (Event_GetFlag(FLAG_VAULT_WATER_OF_LIFE_RECEIVED) == 0) {
    Event_SetMessageId(MsgVaultYouWereSuchGreatHelp);
    LogRoomsA_ShowMessageAndWait(ACTOR_MAYOR, 20);
    LogRoomsA_PlayAnimationAndWait(ACTOR_MAYOR, ANIM_NOD, 20);
    LogRoomsA_ShowMessageAndWait(ACTOR_MAYOR, 30);
    Actor_FaceDirection(ACTOR_MAYOR, FACING_EAST, 0);
    Event_Wait(30);
    Actor_RunRepeatedMotion(ACTOR_MAYOR, 2);
    Event_Wait(30);
    LogRoomsA_FaceEachOtherAndWait(ACTOR_PARTY_LEADER, ACTOR_MAYOR, 20);
    LogRoomsA_PlayAnimationAndWait(ACTOR_MAYOR, ANIM_NOD, 20);
    gFieldWork->message++;
    if (PartyInventory_HasSpace() == 0) {
      Event_SetMessageId(MsgVaultYouWereSuchGreatHelp + 3);
      LogRoomsA_ShowMessageAndWait(ACTOR_MAYOR, 20);
      Event_End();
      return;
    }
    GameFlag_Set(FLAG_VAULT_WATER_OF_LIFE_RECEIVED);
    PartyInventory_GiveItem(ITEM_WATER_OF_LIFE, 0);
  }
  Event_SetMessageId(MsgVaultHeadingOutBeyondGomaRange);
  Event_OpenMessage(ACTOR_MAYOR, 0);
  Event_Wait(20);
  if (Event_AskYesNoWithActorAnimation(0, 0) != 0) {
    gFieldWork->message++;
  }
  Event_ShowMessage(ACTOR_MAYOR, 0);
  Event_End();
}

/// At Vault's inn after the thieves are jailed, the girl at the inn offers
/// leftover bones from dinner; a yes gets a Bone if there is room, a no a nod.
///
/// Reached by talking to the girl at the inn (ACTOR_GIRL_AT_THE_INN) after
/// the thieves are jailed, while the leader is not beside the Bone on the floor
/// (FLAG_VAULT_LEADER_BESIDE_BONE clear). Gives a Bone and sets
/// FLAG_VAULT_BONE_RECEIVED.
static void LogRoomsA_OfferBones(void) {
  Event_Start();
  LogRoomsA_FaceEachOtherAndWait(ACTOR_GIRL_AT_THE_INN, ACTOR_PARTY_LEADER, 2);
  if (GameFlag_Get(FLAG_VAULT_BONE_RECEIVED) == 0) {
    Event_SetMessageId(MsgVaultHaveLotLeftoverBonesFrom);
    Event_OpenMessage(ACTOR_GIRL_AT_THE_INN, 0);
  } else {
    Event_SetMessageId(MsgVaultWantMoreBones);
    Event_OpenMessage(ACTOR_GIRL_AT_THE_INN, 0);
  }
  if (Event_AskYesNoWithActorAnimation(0, 0) == 0) {
    Event_Wait(20);
    Event_ShowMessage(ACTOR_GIRL_AT_THE_INN, 0);
    Event_Wait(20);
    Actor_RunRepeatedMotion(ACTOR_GIRL_AT_THE_INN, 2);
    Event_Wait(20);
    if (PartyInventory_HasSpace() == 0) {
      Actor_SetAnimationAndWait(ACTOR_GIRL_AT_THE_INN, ANIM_SHAKE_HEAD);
      Event_Wait(20);
      Event_SetMessageId(MsgVaultWowHaveManyThingsArent);
      Event_ShowMessage(ACTOR_GIRL_AT_THE_INN, 0);
    } else {
      ItemDisplay_ShowItem(ITEM_BONE, 3);
      PartyInventory_GiveItem(ITEM_BONE, 0);
      GameFlag_Set(FLAG_VAULT_BONE_RECEIVED);
    }
  } else {
    gFieldWork->message++;
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_GIRL_AT_THE_INN, ANIM_NOD);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_GIRL_AT_THE_INN, 0);
  }
  Actor_FaceDirection(ACTOR_GIRL_AT_THE_INN, FACING_SOUTH, 0);
  Event_End();
}

/// In a house of Vault, Mind Read on the wife of the man with the sparkling
/// rock: let me see that rock, or maybe it is worthless.
///
/// Reached by Mind Read on the gem owner's wife (ACTOR_GEM_OWNERS_WIFE); the
/// Mind Read events below are reached the same way on the actor each
/// names, before and after the thieves are jailed (FLAG_VAULT_THIEVES_JAILED
/// picks the thought).
static void LogRoomsA_ReadMindOnRock(void) {
  Event_Start();
  if (GameFlag_Get(FLAG_VAULT_THIEVES_JAILED) == 0) {
    Event_SetMessageId(MsgVaultOkMisterLetMeSee);
  } else {
    Event_SetMessageId(MsgVaultIfRockWorthlessMaybeThats);
  }
  Vault_ShowThought(ACTOR_GEM_OWNERS_WIFE);
  Event_End();
}

/// In a house of Vault, Mind Read on the boy: what is the outside world
/// like, or Father looks sad.
static void LogRoomsA_ReadBoysMind(void) {
  Event_Start();
  if (GameFlag_Get(FLAG_VAULT_THIEVES_JAILED) == 0) {
    Event_SetMessageId(MsgVaultWonderOutsideWorldLike);
  } else {
    Event_SetMessageId(MsgVaultFatherLooksSadWorryingLike);
  }
  Vault_ShowThought(ACTOR_BOY_ASKING_ABOUT_TRAVEL);
  Event_End();
}

/// In Vault's item shop, Mind Read on Grandpa: could they be thieves? Who can
/// be trusted? Or nothing in the house was worth stealing.
static void LogRoomsA_ReadMindOnThieves(void) {
  Event_Start();
  if (GameFlag_Get(FLAG_VAULT_THIEVES_JAILED) == 0) {
    Event_SetMessageId(MsgVaultCouldTheyThievesGoodDont);
  } else {
    Event_SetMessageId(MsgVaultGuessNothingInOurHouse);
  }
  Vault_ShowThought(ACTOR_GRANDPA);
  Event_End();
}

/// At the mayor's in Vault, Mind Read on the mayor: a laugh, or the cave in
/// the Goma Range is dangerous, but the party will be okay. "That tickles..."
static void LogRoomsA_ReadMayorMind(void) {
  Event_Start();
  if (GameFlag_Get(FLAG_VAULT_THIEVES_JAILED) == 0) {
    Event_SetMessageId(MsgVaultTicklesBeingTickledByBoy);
  } else {
    Event_SetMessageId(MsgVaultCaveInGomaRangeDangerous);
  }
  Vault_ShowThought(ACTOR_MAYOR);
  Event_End();
}

/// At Vault's inn, Mind Read on the girl at the inn: the master and his wife
/// are blinded by money; after the thieves are jailed, hungry while cooking,
/// or, once a Bone was given, wondering about calcium.
static void LogRoomsA_ReadBoneGiverMind(void) {
  Event_Start();
  if (GameFlag_Get(FLAG_VAULT_THIEVES_JAILED) == 0) {
    Event_SetMessageId(MsgVaultMasterHisWifeBlindedBy);
  } else if (GameFlag_Get(FLAG_VAULT_BONE_RECEIVED) == 0) {
    Event_SetMessageId(MsgVaultGeeAlwaysGetHungryWhen);
  } else {
    Event_SetMessageId(MsgVaultHeReallyLikesBonesWonder);
  }
  Vault_ShowThought(ACTOR_GIRL_AT_THE_INN);
  Event_End();
}

/// At the mayor's in Vault, talking to Ivan: the first time, he reads Isaac's
/// mind, admits his strange powers and learns they are called Psynergy; then
/// he asks for help finding the rod. A yes has him join the party.
///
/// Reached by talking to Ivan, whose row takes his party slot (IVAN), except
/// while the party corners the suspects. Sets FLAG_VAULT_MET_IVAN the first
/// time, and on a yes plays LogRoomsA_IvanJoinsRodSearch, sets
/// FLAG_VAULT_IVAN_JOINED_ROD_SEARCH and has Ivan walk into the leader.
static void LogRoomsA_MeetIvan(void) {

  struct Actor *actor;

  Event_Start();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(ACTOR_GARET, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(ACTOR_IVAN, PIXELS(4) / 5, PIXELS(2) / 5);
  // Isaac steps up in front of Ivan and Garet comes out beside him.
  Event_PlaySound(MUS_SILENCE);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 384, 408);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_SetPosition(ACTOR_GARET, PIXELS(384), PIXELS(408));
  Actor_WalkToAndWait(ACTOR_GARET, 368, 408);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH + FACING_STEP, 20);
  // Introduce Ivan's ability only on the first meeting.
  if (Event_GetFlag(FLAG_VAULT_MET_IVAN) == 0) {
    GameFlag_Set(FLAG_VAULT_MET_IVAN);
    // Ivan reads Isaac's mind and admits his strange powers.
    LogRoomsA_StartMindRead(ACTOR_IVAN, ACTOR_PARTY_LEADER);
    Event_Wait(40);
    LogRoomsA_EndMindRead();
    Event_SetMessageId(MsgVaultDoPossessStrangePowers);
    Event_PlaySound(MUS_IVAN);
    Event_Wait(30);
    LogRoomsA_PlayAnimationAndWait(ACTOR_IVAN, ANIM_NOD, 30);
    LogRoomsA_ShowMessageAndWait(ACTOR_IVAN, 30);
    // The two start; he reads again: "You have them, too, don't you?"
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Actor_RunRepeatedMotion(ACTOR_GARET, 1);
    Event_Wait(20);
    LogRoomsA_StartMindRead(ACTOR_IVAN, ACTOR_PARTY_LEADER);
    Event_Wait(40);
    LogRoomsA_EndMindRead();
    LogRoomsA_ShowMessageAndWait(ACTOR_IVAN, 30);
    LogRoomsA_FaceEachOtherAndWait(ACTOR_PARTY_LEADER, ACTOR_GARET, 50);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Actor_FaceDirection(ACTOR_GARET, FACING_NORTH + FACING_STEP, 0);
    Event_Wait(20);
    // A third reading: his power is called Psynergy; then they have so much
    // in common he can trust them.
    LogRoomsA_StartMindRead(ACTOR_IVAN, ACTOR_PARTY_LEADER);
    Event_Wait(40);
    LogRoomsA_EndMindRead();
    Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
    LogRoomsA_ShowMessageAndWait(ACTOR_IVAN, 50);
    Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    LogRoomsA_PlayAnimationAndWait(ACTOR_GARET, ANIM_NOD, 20);
    LogRoomsA_PlayAnimationAndWait(ACTOR_IVAN, ANIM_NOD, 20);
    LogRoomsA_ShowMessageAndWait(ACTOR_IVAN, 40);
    // He walks off a step, turns back and tells of Hammet's stolen rod.
    Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
    Event_Wait(30);
    Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
    Event_Wait(30);
    Actor_WalkToAndWait(ACTOR_IVAN, 376, 376);
    Event_Wait(40);
    LogRoomsA_FaceEachOtherAndWait(ACTOR_PARTY_LEADER, ACTOR_GARET, 50);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Actor_FaceDirection(ACTOR_GARET, FACING_NORTH + FACING_STEP, 0);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
    Event_Wait(50);
    LogRoomsA_PlayAnimationAndWait(ACTOR_IVAN, ANIM_NOD, 30);
    Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTH, 0);
    Event_Wait(10);
    Event_ShowMessage(ACTOR_IVAN, 0);
    // "Would you please help me get my master's rod back?"
    Event_OpenMessage(ACTOR_IVAN, 0);
  } else {
    // Asked again: "Would you really? Would you help me find the rod?"
    Event_PlaySound(MUS_IVAN);
    Event_SetMessageId(MsgVaultWouldReallyWouldHelpMe);
    Event_OpenMessage(ACTOR_IVAN, 0);
  }
  // Accepting the search lets Ivan merge into the leader's position; a no gets
  // his apology for asking.
  if (Event_AskYesNoWithActorAnimation(0, 0) == 0) {
    LogRoomsA_IvanJoinsRodSearch();
    GameFlag_Set(FLAG_VAULT_IVAN_JOINED_ROD_SEARCH);
    Import_ActorSetAnimation(ACTOR_IVAN, ANIM_WALK);
    actor = Actor_GetById(ACTOR_PARTY_LEADER);
    if (actor != 0) {
      Actor_SetDestination(ACTOR_IVAN, actor->x.part.pixel,
                           actor->z.part.pixel);
    }
    Actor_WaitForMoveById(ACTOR_IVAN);
    Actor_SetPosition(ACTOR_IVAN, 0, 0);
  } else {
    Event_ShowMessage(ACTOR_IVAN, 0);
  }
  // Garet walks back into the leader and the earlier music returns.
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_WALK);
  actor = Actor_GetById(ACTOR_PARTY_LEADER);
  if (actor != 0) {
    Actor_SetDestination(ACTOR_GARET, actor->x.part.pixel, actor->z.part.pixel);
  }
  Actor_WaitForMoveById(ACTOR_GARET);
  Actor_SetPosition(ACTOR_GARET, 0, 0);
  Scene_PlayMusic();
  Event_End();
}

/// Ivan thanks them, they exchange names, and he shows Isaac and Garet Mind
/// Read; he joins the party.
///
/// Called by LogRoomsA_MeetIvan when the party agrees to help. Adds Ivan to
/// the party ("Ivan joined your party!").
static void LogRoomsA_IvanJoinsRodSearch(void) {
  s32 message;

  // Ivan jumps for joy: "Thank you. With your help, I believe we can recover
  // the rod." Then he asks their names.
  Event_Wait(20);
  Actor_GetById(ACTOR_IVAN)->paused = 0;
  Actor_Jump(ACTOR_IVAN, 4, 0);
  Event_Wait(40);
  message = MsgVaultThankHelpBelieve;
  Event_SetMessageId(message);
  Event_Wait(20);
  LogRoomsA_ShowMessageAndWait(ACTOR_IVAN, 20);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Event_Wait(50);
  LogRoomsA_ShowMessageAndWait(ACTOR_IVAN, 30);
  // He reads their names from Isaac's mind; Isaac backs away from him.
  Actor_WalkToAndWait(ACTOR_IVAN, 376, 392);
  LogRoomsA_StartMindRead(ACTOR_IVAN, ACTOR_PARTY_LEADER);
  Event_Wait(40);
  LogRoomsA_EndMindRead();
  Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  Actor_GetById(ACTOR_PARTY_LEADER)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 384, 424);
  Event_Wait(1);
  Actor_GetById(ACTOR_PARTY_LEADER)->steeringFlags |= ACTOR_FACES_MOTION;
  Event_Wait(30);
  Event_ShowMessageAndWait(ACTOR_IVAN, 0);
  // Demonstrate Mind Read and let Garet and Isaac react to Ivan's explanation.
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_GARET,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(60);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  LogRoomsA_PlayAnimationAndWait(ACTOR_GARET, ANIM_NOD, 30);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Event_Wait(60);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
  Event_Wait(10);
  LogRoomsA_PlayAnimationAndWait(ACTOR_IVAN, ANIM_NOD, 20);
  Event_ShowMessageAndWait(ACTOR_IVAN, 0);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Event_Wait(10);
  LogRoomsA_FaceActorAndWait(ACTOR_GARET, ACTOR_PARTY_LEADER, 30);
  Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Event_Wait(10);
  LogRoomsA_FaceActorAndWait(ACTOR_PARTY_LEADER, ACTOR_GARET, 40);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Event_Wait(60);
  // "Is my Mind Read so frightening?": either answer goes on the same way.
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Event_Wait(60);
  FieldMessage_ShowAtActor(ACTOR_IVAN, 0);
  Actor_SetAngleToward(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
  Actor_SetAngleToward(ACTOR_GARET, ACTOR_IVAN, 0);
  Event_AskYesNoWithActorAnimation(0, 0);
  Event_Wait(30);
  // Ivan and Isaac clasp hands, and Garet cries out that his mind is read.
  LogRoomsA_PlayAnimationAndWait(ACTOR_IVAN, ANIM_NOD, 10);
  Actor_WalkToAndWait(ACTOR_IVAN, 384, 408);
  Event_Wait(10);
  FieldMessage_Show(message + 5, 1);
  Event_SetMessageId(message + 6);
  Actor_TurnToFaceEachOther(ACTOR_IVAN, ACTOR_GARET, 0);
  LogRoomsA_FaceActorAndWait(ACTOR_PARTY_LEADER, ACTOR_GARET, 20);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Event_Wait(60);
  Actor_RunRepeatedMotion(ACTOR_GARET, 1);
  Actor_GetById(ACTOR_GARET)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(ACTOR_GARET, 352, 408);
  Event_Wait(1);
  Actor_GetById(ACTOR_GARET)->steeringFlags |= ACTOR_FACES_MOTION;
  Event_Wait(10);
  // Garet backs off; Ivan asks whether Isaac could read Garet's mind, and the
  // two read it together. "Just as I thought."
  Actor_WalkTo(ACTOR_IVAN, 368, 408);
  Event_Wait(20);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 368, 424);
  Actor_SetAngleToward(ACTOR_PARTY_LEADER, ACTOR_GARET, 0);
  Actor_WaitForMoveById(ACTOR_IVAN);
  LogRoomsA_StartMindRead(ACTOR_IVAN, ACTOR_GARET);
  LogRoomsA_PlayAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD, 10);
  Event_ShowMessageAndWait(ACTOR_GARET, 0);
  LogRoomsA_EndMindRead();
  LogRoomsA_FaceEachOtherAndWait(ACTOR_IVAN, ACTOR_PARTY_LEADER, 30);
  Event_ShowMessageAndWait(ACTOR_IVAN, 0);
  Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  Event_Wait(30);
  LogRoomsA_PlayAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD, 30);
  LogRoomsA_PlayAnimationAndWait(ACTOR_IVAN, ANIM_NOD, 10);
  Event_ShowMessageAndWait(ACTOR_IVAN, 0);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Actor_SetAngleToward(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
  Event_ShowMessageAndWait(ACTOR_GARET, 0);
  Actor_SetAngleToward(ACTOR_PARTY_LEADER, ACTOR_GARET, 0);
  LogRoomsA_PlayAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD, 40);
  Actor_SetAngleToward(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
  LogRoomsA_FaceActorAndWait(ACTOR_GARET, ACTOR_IVAN, 30);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Event_Wait(60);
  // Sharing their Psynergy let Isaac read minds too; Garet protests that it
  // isn't fair, and Ivan agrees it is one-way.
  LogRoomsA_PlayAnimationAndWait(ACTOR_IVAN, ANIM_NOD, 20);
  Event_ShowMessageAndWait(ACTOR_IVAN, 0);
  FieldEffect_SpawnAttached(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
  Event_Wait(60);
  LogRoomsA_FaceActorAndWait(ACTOR_GARET, ACTOR_PARTY_LEADER, 10);
  Event_ShowMessageAndWait(ACTOR_GARET, 0);
  FieldEffect_SpawnAttached(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Event_Wait(60);
  Event_ShowMessageAndWait(ACTOR_IVAN, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Event_Wait(60);
  // Ivan and Garet clasp hands, and Garet reads Isaac's mind.
  FieldMessage_Show(message + 13, 1);
  Event_SetMessageId(message + 14);
  Actor_TurnToFaceEachOther(ACTOR_IVAN, ACTOR_PARTY_LEADER, 0);
  Actor_SetAngleToward(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
  FieldEffect_SpawnAttached(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE,
                            0);
  Event_Wait(60);
  LogRoomsA_StartMindRead(ACTOR_IVAN, ACTOR_PARTY_LEADER);
  FieldPsynergy_RestoreMindReadPalette();
  LogRoomsA_ShowMessageAndWait(ACTOR_GARET, 30);
  LogRoomsA_EndMindRead();
  Actor_TurnToFaceEachOther(ACTOR_IVAN, ACTOR_GARET, 0);
  LogRoomsA_PlayAnimationAndWait(ACTOR_IVAN, ANIM_NOD, 10);
  LogRoomsA_ShowMessageAndWait(ACTOR_IVAN, 20);
  Actor_TurnToFaceEachOther(ACTOR_IVAN, ACTOR_PARTY_LEADER, 0);
  LogRoomsA_PlayAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD, 20);
  Actor_RunRepeatedMotion(ACTOR_GARET, 1);
  Event_Wait(20);
  // "Let's use Mind Read to find the thieves!" Garet worries people will
  // notice.
  LogRoomsA_PlayAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD, 10);
  LogRoomsA_ShowMessageAndWait(ACTOR_GARET, 30);
  Actor_SetAngleToward(ACTOR_IVAN, ACTOR_GARET, 0);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Event_Wait(30);
  LogRoomsA_FaceEachOtherAndWait(ACTOR_PARTY_LEADER, ACTOR_GARET, 10);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  LogRoomsA_PlayAnimationAndWait(ACTOR_GARET, ANIM_NOD, 30);
  Actor_SetAngleToward(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
  Actor_SetAngleToward(ACTOR_GARET, ACTOR_IVAN, 0);
  Actor_SetAngleToward(ACTOR_IVAN, ACTOR_PARTY_LEADER, 0);
  // Only Adepts can tell when Ivan uses Psynergy; ordinary people see
  // nothing, and he promises not to read their minds again.
  FieldEffect_SpawnAttached(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 0);
  Event_Wait(60);
  LogRoomsA_ShowMessageAndWait(ACTOR_IVAN, 10);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  LogRoomsA_PlayAnimationAndWait(ACTOR_GARET, ANIM_NOD, 20);
  LogRoomsA_PlayAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD, 20);
  LogRoomsA_ShowMessageAndWait(ACTOR_IVAN, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  FieldEffect_SpawnAttached(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Event_Wait(60);
  LogRoomsA_PlayAnimationAndWait(ACTOR_IVAN, ANIM_NOD, 10);
  LogRoomsA_ShowMessageAndWait(ACTOR_IVAN, 30);
  FieldEffect_SpawnAttached(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS,
                            0);
  Event_Wait(60);
  LogRoomsA_PlayAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD, 10);
  LogRoomsA_ShowMessageAndWait(ACTOR_IVAN, 30);
  LogRoomsA_PlayAnimationAndWait(ACTOR_IVAN, ANIM_NOD, 10);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  LogRoomsA_PlayAnimationAndWait(ACTOR_GARET, ANIM_NOD, 20);
  // "Ivan joined your party!", then he will read the townspeople's minds.
  Event_PrepareActorAndApplyValue(IVAN, 1);
  Event_SetMessageId(message + 22);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
  Event_ShowMessageAndWait(ACTOR_IVAN, 0);
}

/// In the inn's loft in Vault, Isaac cuts the tied-up villager free; the three
/// thieves arrive, tell of Hammet in Lunpa and of Dodonpa, ask to be let go,
/// and attack.
///
/// Reached by talking to the tied-up villager (ACTOR_TIED_UP_VILLAGER) in the
/// loft; the loft's setup replays the thieves' part through
/// LogRoomsA_ConfrontThievesAgain once FLAG_VAULT_THIEVES_ATTACKED is
/// set. Sets that flag and starts the battle (encounter 12, 5): a win returns
/// to LOG_ROOMS_A_LOFT_AFTER_THIEVES_DEFEATED, a loss to
/// LOG_ROOMS_A_LOFT_AFTER_LOSING_TO_THIEVES.
static void LogRoomsA_FreeRooferAndConfrontThieves(void) {
  s32 base;

  Event_Start();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(ACTOR_GARET, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(ACTOR_IVAN, PIXELS(4) / 5, PIXELS(2) / 5);
  // Isaac cuts the tied-up villager free from the ropes; he gets up and nods.
  Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 3);
  Event_Wait(20);
  base = MsgVaultVaultCutFree;
  FieldMessage_Show(base, 1);
  base++;
  Event_SetMessageId(base);
  Event_Wait(30);
  Import_ActorSetAnimation(ACTOR_TIED_UP_VILLAGER, ANIM_STAND);
  Event_Wait(60);
  LogRoomsA_PlayAnimationAndWait(ACTOR_TIED_UP_VILLAGER, ANIM_NOD, 40);
  // Garet and Ivan come out of the leader around the tied-up villager.
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 792, 440);
  LogRoomsA_FaceActorAndWait(ACTOR_PARTY_LEADER, ACTOR_TIED_UP_VILLAGER, 20);
  Actor_SetPosition(ACTOR_GARET, PIXELS(792), PIXELS(440));
  Actor_SetPosition(ACTOR_IVAN, PIXELS(792), PIXELS(440));
  Actor_WalkTo(ACTOR_GARET, 808, 432);
  Actor_WalkTo(ACTOR_IVAN, 792, 456);
  Actor_WaitForMoveById(ACTOR_GARET);
  Actor_SetAngleToward(ACTOR_GARET, ACTOR_TIED_UP_VILLAGER, 0);
  Actor_WaitForMoveById(ACTOR_IVAN);
  LogRoomsA_FaceActorAndWait(ACTOR_IVAN, ACTOR_TIED_UP_VILLAGER, 60);
  // The tied-up villager tells how he came up to fix the roof, was whacked from
  // behind and woke up tied.
  Actor_ShowEmote(ACTOR_TIED_UP_VILLAGER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Event_Wait(60);
  LogRoomsA_ShowMessageAndWait(ACTOR_TIED_UP_VILLAGER, 20);
  Import_ActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
  LogRoomsA_PlayAnimationAndWait(ACTOR_GARET, ANIM_NOD, 30);
  Actor_RunRepeatedMotion(ACTOR_TIED_UP_VILLAGER, 2);
  Event_Wait(20);
  LogRoomsA_ShowMessageAndWait(ACTOR_TIED_UP_VILLAGER, 20);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Event_Wait(60);
  Actor_FaceDirection(ACTOR_TIED_UP_VILLAGER, FACING_SOUTHEAST + FACING_STEP,
                      0);
  Event_Wait(10);
  LogRoomsA_ShowMessageAndWait(ACTOR_TIED_UP_VILLAGER, 30);
  // He looks around at chests he doesn't remember; Ivan guesses the stolen
  // goods are in them.
  Actor_FaceDirection(ACTOR_TIED_UP_VILLAGER, FACING_NORTH + FACING_STEP, 0);
  Event_Wait(30);
  Actor_ShowEmote(ACTOR_TIED_UP_VILLAGER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                  0);
  Event_Wait(60);
  Actor_FaceDirection(ACTOR_TIED_UP_VILLAGER, FACING_NORTHWEST + FACING_STEP,
                      0);
  Event_Wait(40);
  Actor_FaceDirection(ACTOR_TIED_UP_VILLAGER, FACING_NORTH + FACING_STEP, 0);
  Event_Wait(40);
  LogRoomsA_FaceActorAndWait(ACTOR_TIED_UP_VILLAGER, ACTOR_PARTY_LEADER, 20);
  LogRoomsA_PlayAnimationAndWait(ACTOR_TIED_UP_VILLAGER, ANIM_SHAKE_HEAD, 30);
  LogRoomsA_ShowMessageAndWait(ACTOR_TIED_UP_VILLAGER, 20);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
  LogRoomsA_ShowMessageAndWait(ACTOR_IVAN, 40);
  // The ominous music starts as the thieves' leader appears at the ladder
  // and says "Looks like we've been found out!"; everyone turns to him.
  Actor_SetPosition(ACTOR_THIEF_LEADER, PIXELS(744), PIXELS(408));
  Sound_Play(MUS_OMINOUS);
  LogRoomsA_ShowMessageAndWait(ACTOR_THIEF_LEADER, 20);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  Actor_StartRepeatedMotion(ACTOR_GARET, 1);
  Actor_StartRepeatedMotion(ACTOR_IVAN, 1);
  Actor_RunRepeatedMotion(ACTOR_TIED_UP_VILLAGER, 1);
  Event_Wait(30);
  Actor_SetAngleToward(ACTOR_PARTY_LEADER, ACTOR_THIEF_LEADER, 0);
  Actor_SetAngleToward(ACTOR_GARET, ACTOR_THIEF_LEADER, 0);
  Actor_SetAngleToward(ACTOR_IVAN, ACTOR_THIEF_LEADER, 0);
  LogRoomsA_FaceActorAndWait(ACTOR_TIED_UP_VILLAGER, ACTOR_THIEF_LEADER, 40);
  // He walks up and his two men run in behind him.
  Actor_SetSpeed(ACTOR_THIEF_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(ACTOR_FIRST_THIEF, PIXELS(3) / 2, PIXELS(3) / 4);
  Actor_SetSpeed(ACTOR_SECOND_THIEF, PIXELS(3) / 2, PIXELS(3) / 4);
  Actor_SetPosition(ACTOR_FIRST_THIEF, PIXELS(744), PIXELS(408));
  Actor_SetPosition(ACTOR_SECOND_THIEF, PIXELS(744), PIXELS(408));
  Actor_WalkToAndWait(ACTOR_THIEF_LEADER, 792, 416);
  Actor_FaceDirection(ACTOR_THIEF_LEADER, FACING_SOUTHEAST + FACING_STEP, 0);
  Event_Wait(70);
  Actor_SetAngleToward(ACTOR_PARTY_LEADER, ACTOR_THIEF_LEADER, 0);
  Actor_FaceActor(ACTOR_GARET, ACTOR_THIEF_LEADER, 0);
  Actor_SetAngleToward(ACTOR_IVAN, ACTOR_THIEF_LEADER, 0);
  Actor_SetAngleToward(ACTOR_TIED_UP_VILLAGER, ACTOR_THIEF_LEADER, 0);
  Actor_SetScript(ACTOR_FIRST_THIEF, gLogRoomsA_FirstThiefEntryScript);
  Event_Wait(40);
  Actor_SetScript(ACTOR_SECOND_THIEF, gLogRoomsA_SecondThiefEntryScript);
  Actor_RefreshSelectorById(ACTOR_SECOND_THIEF);
  Actor_FaceDirection(ACTOR_FIRST_THIEF, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_SECOND_THIEF, FACING_SOUTHEAST, 0);
  Event_Wait(40);
  // The three taunt Hammet's whelp and say Hammet was caught by worse thieves
  // after fleeing to Lunpa.
  LogRoomsA_PlayAnimationAndWait(ACTOR_THIEF_LEADER, ANIM_SHAKE_HEAD, 20);
  LogRoomsA_ShowMessageAndWait(ACTOR_THIEF_LEADER, 20);
  Actor_RunRepeatedMotion(ACTOR_FIRST_THIEF, 1);
  Event_Wait(10);
  LogRoomsA_ShowMessageAndWait(ACTOR_FIRST_THIEF, 20);
  Actor_RunRepeatedMotion(ACTOR_SECOND_THIEF, 1);
  Event_Wait(10);
  LogRoomsA_ShowMessageAndWait(ACTOR_SECOND_THIEF, 40);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Event_Wait(60);
  LogRoomsA_PlayAnimationAndWait(ACTOR_THIEF_LEADER, ANIM_SHAKE_HEAD, 20);
  LogRoomsA_ShowMessageAndWait(ACTOR_THIEF_LEADER, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Event_Wait(60);
  // The tied-up villager is alarmed at the name Lunpa; Garet asks what kind of
  // place it is, and the tied-up villager and the thieves tell of Lunpa, Donpa
  // and Dodonpa.
  Actor_ShowEmote(ACTOR_TIED_UP_VILLAGER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                  0);
  Event_Wait(40);
  Actor_RunRepeatedMotion(ACTOR_TIED_UP_VILLAGER, 2);
  Event_Wait(20);
  LogRoomsA_ShowMessageAndWait(ACTOR_TIED_UP_VILLAGER, 20);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_TIED_UP_VILLAGER, 0);
  Actor_SetAngleToward(ACTOR_GARET, ACTOR_TIED_UP_VILLAGER, 0);
  LogRoomsA_FaceActorAndWait(ACTOR_IVAN, ACTOR_TIED_UP_VILLAGER, 20);
  LogRoomsA_PlayAnimationAndWait(ACTOR_TIED_UP_VILLAGER, ANIM_SHAKE_HEAD, 30);
  LogRoomsA_ShowMessageAndWait(ACTOR_TIED_UP_VILLAGER, 20);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
  Event_Wait(20);
  Actor_SetAngleToward(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
  LogRoomsA_FaceActorAndWait(ACTOR_GARET, ACTOR_IVAN, 30);
  LogRoomsA_ShowMessageAndWait(ACTOR_GARET, 20);
  Actor_ShowEmote(ACTOR_TIED_UP_VILLAGER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Event_Wait(60);
  LogRoomsA_ShowMessageAndWait(ACTOR_TIED_UP_VILLAGER, 20);
  Actor_RunRepeatedMotion(ACTOR_THIEF_LEADER, 1);
  Event_Wait(10);
  LogRoomsA_ShowMessageAndWait(ACTOR_THIEF_LEADER, 20);
  Actor_SetAngleToward(ACTOR_PARTY_LEADER, ACTOR_THIEF_LEADER, 0);
  Actor_FaceActor(ACTOR_GARET, ACTOR_THIEF_LEADER, 0);
  LogRoomsA_FaceActorAndWait(ACTOR_IVAN, ACTOR_THIEF_LEADER, 20);
  LogRoomsA_PlayAnimationAndWait(ACTOR_FIRST_THIEF, ANIM_NOD, 20);
  LogRoomsA_ShowMessageAndWait(ACTOR_FIRST_THIEF, 20);
  LogRoomsA_PlayAnimationAndWait(ACTOR_SECOND_THIEF, ANIM_SHAKE_HEAD, 20);
  LogRoomsA_ShowMessageAndWait(ACTOR_SECOND_THIEF, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Event_Wait(60);
  // "See, we're not so bad. How about it? Will you let us go?" Yes: if they
  // return the goods and repent? No: even returning everything is a crime?
  LogRoomsA_PlayAnimationAndWait(ACTOR_THIEF_LEADER, ANIM_NOD, 20);
  FieldMessage_ShowAtActor(ACTOR_THIEF_LEADER, 0);
  Event_Wait(50);
  Actor_SetAngleToward(ACTOR_IVAN, ACTOR_PARTY_LEADER, 0);
  LogRoomsA_FaceActorAndWait(ACTOR_GARET, ACTOR_PARTY_LEADER, 30);
  if (Event_AskYesNoWithActorAnimation(0, 0) == 0) {
    Event_Wait(40);
    Actor_SetAngleToward(ACTOR_GARET, ACTOR_THIEF_LEADER, 0);
    Actor_FaceActor(ACTOR_IVAN, ACTOR_THIEF_LEADER, 0);
    Actor_RunRepeatedMotion(ACTOR_THIEF_LEADER, 2);
    Event_Wait(20);
    Event_ShowMessageAndWait(ACTOR_THIEF_LEADER, 0);
  } else {
    Event_Wait(40);
    Actor_SetAngleToward(ACTOR_GARET, ACTOR_THIEF_LEADER, 0);
    Actor_SetAngleToward(ACTOR_IVAN, ACTOR_THIEF_LEADER, 0);
    base = MsgVaultVaultThievesCaught;
    Event_SetMessageId(base);
    Actor_RunRepeatedMotion(ACTOR_THIEF_LEADER, 2);
    Event_Wait(20);
    Event_ShowMessageAndWait(ACTOR_THIEF_LEADER, 0);
    // Both answers go on with the same refusal.
    base -= 3;
    Event_SetMessageId(base);
  }
  // "We stole this stuff fair and square!" They agree to keep the party quiet,
  // and the leader cries "Get 'em!!!"
  Actor_ShowEmote(ACTOR_FIRST_THIEF, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
  Event_Wait(60);
  LogRoomsA_ShowMessageAndWait(ACTOR_FIRST_THIEF, 20);
  LogRoomsA_PlayAnimationAndWait(ACTOR_SECOND_THIEF, ANIM_SHAKE_HEAD, 20);
  LogRoomsA_ShowMessageAndWait(ACTOR_SECOND_THIEF, 30);
  LogRoomsA_FaceEachOtherAndWait(ACTOR_THIEF_LEADER, ACTOR_FIRST_THIEF, 30);
  Import_ActorSetAnimation(ACTOR_THIEF_LEADER, ANIM_NOD);
  LogRoomsA_PlayAnimationAndWait(ACTOR_FIRST_THIEF, ANIM_NOD, 30);
  LogRoomsA_FaceEachOtherAndWait(ACTOR_THIEF_LEADER, ACTOR_SECOND_THIEF, 30);
  Import_ActorSetAnimation(ACTOR_THIEF_LEADER, ANIM_NOD);
  LogRoomsA_PlayAnimationAndWait(ACTOR_SECOND_THIEF, ANIM_NOD, 40);
  Actor_FaceDirection(ACTOR_THIEF_LEADER, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_FIRST_THIEF, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_SECOND_THIEF, FACING_SOUTHEAST + FACING_STEP, 0);
  Event_Wait(20);
  LogRoomsA_PlayAnimationAndWait(ACTOR_THIEF_LEADER, ANIM_SHAKE_HEAD, 20);
  Event_ShowMessageAndWait(ACTOR_THIEF_LEADER, 0);
  // Preserve the return destinations before requesting the thieves' encounter.
  GameFlag_Set(FLAG_VAULT_THIEVES_ATTACKED);
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
  base = (s32)&SceneId_LogRoomsA;
  Event_SetAfterBattleDestination(base,
                                  LOG_ROOMS_A_LOFT_AFTER_THIEVES_DEFEATED);
  Event_SetDefeatDestination(base, LOG_ROOMS_A_LOFT_AFTER_LOSING_TO_THIEVES);
  Battle_SetType(BATTLE_TYPE_SCRIPTED);
  Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED_VALE_VAULT, 5);
  Event_End();
}

/// At Vault's inn, the party leaves the guests' room for the floor below
/// (LOG_ROOMS_A_INN_FROM_INN_UPSTAIRS).
///
/// Reached by stepping on CELL_BEHAVIOR_LOG_ROOMS_A_GUESTS_ROOM_STAIRS before
/// the thieves are jailed (after, the same cell is a plain exit); takes
/// LOG_ROOMS_A_INN_UPSTAIRS_TO_INN.
static void LogRoomsA_LeaveGuestsRoom(void) {
  Sound_Play(SE_MAP_EXIT);
  Import_EventSetExitRequest(LOG_ROOMS_A_INN_UPSTAIRS_TO_INN);
}

/// In Vault's room with the crate, the party climbs up into the inn's loft by
/// the way behind the crate.
///
/// Reached by stepping on CELL_BEHAVIOR_LOG_ROOMS_A_LOFT_CLIMB; takes
/// LOG_ROOMS_A_CRATE_ROOM_TO_LOFT.
static void LogRoomsA_ClimbToLoft(void) {
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1) / 2, PIXELS(1) / 4);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 728, 408);
  gFieldWork->transitionFrames = 16;
  Event_PlaySound(SE_MAP_EXIT);
  Import_EventSetExitRequest(LOG_ROOMS_A_CRATE_ROOM_TO_LOFT);
}

/// Leaving the inn's loft, the party hears the tied-up villager groan.
/// It hears him when it leaves before the thieves are confronted.
/// "Uhnn... Uhnn..."
///
/// Reached by stepping on CELL_BEHAVIOR_LOG_ROOMS_A_LOFT_DESCENT; takes
/// LOG_ROOMS_A_LOFT_TO_CRATE_ROOM back down.
static void LogRoomsA_LeaveLoft(void) {
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1) / 2, PIXELS(1) / 4);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 744, 408);
  if (GameFlag_Get(FLAG_VAULT_THIEVES_ATTACKED) == 0) {
    Event_Start();
    Event_SetMessageId(MsgVaultUhnnUhnn);
    Event_ShowMessage(ACTOR_TIED_UP_VILLAGER, 0);
    Event_End();
  }
  gFieldWork->transitionFrames = 16;
  Sound_Play(SE_MAP_EXIT);
  Import_EventSetExitRequest(LOG_ROOMS_A_LOFT_TO_CRATE_ROOM);
}

/// In the guests' room at Vault's inn, the two suspects spot "that weird kid"
/// with the party and run from him. "What do you want?"
///
/// Reached by stepping on CELL_BEHAVIOR_LOG_ROOMS_A_GUESTS_ROOM while
/// FLAG_VAULT_IVAN_JOINED_ROD_SEARCH is set. Sets
/// FLAG_VAULT_SUSPECTS_APPROACHED and FLAG_SUSPECTS_FLEEING, starts their
/// routes and the task that keeps them facing the leader.
/// It plays once: it copies CELL_BEHAVIOR_LOG_ROOMS_A_GUESTS_ROOM_DOOR cells
/// over the cells that reach it.
static void LogRoomsA_ApproachSuspects(void) {

  struct Actor *suspectedLeader;
  struct Actor *suspectedThief;

  suspectedLeader = Actor_GetById(ACTOR_SUSPECTED_LEADER);
  suspectedThief = Actor_GetById(ACTOR_SUSPECTED_THIEF);
  Event_Start();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(ACTOR_GARET, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(ACTOR_IVAN, PIXELS(4) / 5, PIXELS(2) / 5);
  // Isaac walks in; the two start and spot Ivan with him: "It looks like that
  // weird kid is with them!"
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 232, 696);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 200, 696);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_SUSPECTED_THIEF, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_SUSPECTED_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                  0);
  Event_Wait(60);
  LogRoomsA_FaceActorAndWait(ACTOR_SUSPECTED_THIEF, ACTOR_PARTY_LEADER, 10);
  Actor_RunRepeatedMotion(ACTOR_SUSPECTED_LEADER, 2);
  Event_Wait(20);
  Event_SetMessageId(MsgVaultWant);
  LogRoomsA_ShowMessageAndWait(ACTOR_SUSPECTED_LEADER, 20);
  Actor_SetAttachedEffect(ACTOR_SUSPECTED_THIEF,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(60);
  LogRoomsA_ShowMessageAndWait(ACTOR_SUSPECTED_THIEF, 20);
  Actor_RunRepeatedMotion(ACTOR_SUSPECTED_LEADER, 1);
  LogRoomsA_ShowMessageAndWait(ACTOR_SUSPECTED_LEADER, 30);
  // "Don't let him near me!" They run off to their routes' first spots.
  Actor_SetSpeed(ACTOR_SUSPECTED_LEADER, PIXELS(4), PIXELS(2));
  Actor_SetSpeed(ACTOR_SUSPECTED_THIEF, PIXELS(7) / 2, PIXELS(7) / 4);
  Actor_SetScript(ACTOR_SUSPECTED_THIEF, gLogRoomsA_SuspectToSouthWestCorner);
  Actor_SetScript(ACTOR_SUSPECTED_LEADER, gLogRoomsA_SuspectRoundToWestBedHead);
  Actor_RefreshSelectorById(ACTOR_SUSPECTED_LEADER);
  MapGrid_CopyCellAttributes(14, 45, 3, 1, 14, 44);
  GameFlag_Set(FLAG_VAULT_SUSPECTS_APPROACHED);
  GameFlag_Set(FLAG_SUSPECTS_FLEEING);
  Task_AddFrame(LogRoomsA_FaceSuspectsTowardLeader, TASK_PASS_FRAME);
  suspectedLeader->timer = 1;
  suspectedThief->timer = 3;
  Event_End();
}

/// Stepping out of the guests' room at Vault's inn, the party talks it over:
/// Ivan can't read their minds while they run; Garet tells Isaac to take the
/// lead and corner them.
///
/// Reached by stepping on CELL_BEHAVIOR_LOG_ROOMS_A_GUESTS_ROOM_DOOR while
/// FLAG_SUSPECTS_FLEEING is set. Clears it and stops the facing task; the
/// suspects go back to their places.
static void LogRoomsA_DiscussSuspiciousGuests(void) {
  struct Actor *suspectedLeader;
  struct Actor *suspectedThief;
  s32 base;

  suspectedLeader = Actor_GetById(ACTOR_SUSPECTED_LEADER);
  suspectedThief = Actor_GetById(ACTOR_SUSPECTED_THIEF);
  Event_Start();
  Task_Remove(LogRoomsA_FaceSuspectsTowardLeader);
  GameFlag_Clear(FLAG_SUSPECTS_FLEEING);
  if (suspectedLeader->timer <= 3) {
    Actor_SetScript(ACTOR_SUSPECTED_LEADER,
                    gLogRoomsA_SuspectRoundToWestBedFoot);
  } else {
    Actor_SetScript(ACTOR_SUSPECTED_LEADER, gLogRoomsA_SuspectToWestBedFoot);
  }
  if (suspectedThief->timer <= 2) {
    Actor_SetScript(ACTOR_SUSPECTED_THIEF,
                    gLogRoomsA_SuspectAcrossToWestOfBeds);
  } else {
    Actor_SetScript(ACTOR_SUSPECTED_THIEF, gLogRoomsA_SuspectToWestBedFoot);
  }
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(ACTOR_GARET, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(ACTOR_IVAN, PIXELS(4) / 5, PIXELS(2) / 5);
  // Outside the room, Garet and Ivan step out of the leader.
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 248, 728);
  Actor_SetPosition(ACTOR_IVAN, PIXELS(248), PIXELS(728));
  Actor_SetPosition(ACTOR_GARET, PIXELS(248), PIXELS(728));
  Actor_WalkTo(ACTOR_IVAN, 264, 744);
  Actor_WalkToAndWait(ACTOR_GARET, 232, 744);
  Actor_WaitForMoveById(ACTOR_IVAN);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Actor_FaceActor(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
  LogRoomsA_FaceActorAndWait(ACTOR_IVAN, ACTOR_PARTY_LEADER, 30);
  // Ivan: they're acting suspicious.
  Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
  base = MsgVaultTheyreActingSuspiciousSomethingsNot;
  Event_SetMessageId(base);
  Event_ShowMessage(ACTOR_IVAN, 0);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
  LogRoomsA_FaceActorAndWait(ACTOR_GARET, ACTOR_IVAN, 20);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  LogRoomsA_PlayAnimationAndWait(ACTOR_GARET, ANIM_NOD, 20);
  LogRoomsA_FaceEachOtherAndWait(ACTOR_PARTY_LEADER, ACTOR_GARET, 10);
  // Garet: "But you can't use Mind Read if they keep running away, can you?"
  // Yes: Ivan asks how he can catch them; no: there must be a way.
  Event_OpenMessage(ACTOR_GARET, 0);
  if (Event_AskYesNoWithActorAnimation(0, 0) != 0) {
    gFieldWork->message++;
  }
  LogRoomsA_ShowMessageAndWait(ACTOR_GARET, 30);
  // Garet's plan: the three of them surround one of the two.
  Event_SetMessageId((base + 4));
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
  LogRoomsA_FaceActorAndWait(ACTOR_GARET, ACTOR_IVAN, 50);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Event_Wait(60);
  LogRoomsA_FaceEachOtherAndWait(ACTOR_PARTY_LEADER, ACTOR_GARET, 50);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
  LogRoomsA_FaceActorAndWait(ACTOR_GARET, ACTOR_IVAN, 30);
  LogRoomsA_PlayAnimationAndWait(ACTOR_IVAN, ANIM_NOD, 10);
  LogRoomsA_ShowMessageAndWait(ACTOR_IVAN, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Event_Wait(60);
  LogRoomsA_PlayAnimationAndWait(ACTOR_IVAN, ANIM_NOD, 20);
  LogRoomsA_ShowMessageAndWait(ACTOR_IVAN, 30);
  // "Isaac, you take the lead." The two walk back into him, and the suspects
  // are put back at their table.
  Event_SetMessageId(MsgVaultIsaacTakeLead);
  Event_ShowMessage(ACTOR_GARET, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Event_Wait(40);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_NOD);
  LogRoomsA_PlayAnimationAndWait(ACTOR_IVAN, ANIM_NOD, 50);
  Actor_WalkTo(ACTOR_IVAN, 248, 728);
  Actor_WalkToAndWait(ACTOR_GARET, 248, 728);
  Actor_SetPosition(ACTOR_GARET, 0, 0);
  Actor_SetPosition(ACTOR_IVAN, 0, 0);
  Actor_SetPosition(ACTOR_SUSPECTED_LEADER, PIXELS(104), PIXELS(696));
  Actor_SetPosition(ACTOR_SUSPECTED_THIEF, PIXELS(120), PIXELS(696));
  Actor_FaceDirection(ACTOR_SUSPECTED_LEADER, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_SUSPECTED_THIEF, FACING_WEST, 0);
  MapGrid_CopyCellAttributes(14, 50, 3, 1, 14, 44);
  Event_End();
}

/// In the guests' room at Vault's inn, the party sets out to corner the two
/// suspects.
///
/// Reached by stepping on CELL_BEHAVIOR_LOG_ROOMS_A_CORNERING_START while
/// FLAG_VAULT_SUSPECTS_APPROACHED is set. Sets FLAG_MENUS_RAISE_TRIGGER, so the
/// menus answer with Garet's refusal, and FLAG_VAULT_CORNERING_SUSPECTS, so
/// talking to Ivan or the suspects does nothing; then starts the chase
/// (LogRoomsA_StartActorStops).
static void LogRoomsA_StartCorneringSuspects(void) {
  GameFlag_Set(FLAG_MENUS_RAISE_TRIGGER);
  GameFlag_Set(FLAG_VAULT_CORNERING_SUSPECTS);
  LogRoomsA_StartActorStops();
}

/// Has the suspects react to the party's return and resets their route states.
/// "Hey, they're back!"
///
/// Called by LogRoomsA_StartActorStops.
static void LogRoomsA_ResetSuspectRoutes(void) {
  Event_Wait(30);
  Actor_RunRepeatedMotion(ACTOR_SUSPECTED_LEADER, 1);
  Event_Wait(20);
  Event_SetMessageId(MsgVaultTheyreBack);
  LogRoomsA_ShowMessageAndWait(ACTOR_SUSPECTED_LEADER, 20);
  Actor_FaceDirection(ACTOR_SUSPECTED_THIEF, FACING_EAST, 20);
  Actor_SetAttachedEffect(ACTOR_SUSPECTED_THIEF,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_RunRepeatedMotion(ACTOR_SUSPECTED_THIEF, 2);
  LogRoomsA_ShowMessageAndWait(ACTOR_SUSPECTED_THIEF, 20);
  Actor_SetAnimationAndWait(ACTOR_SUSPECTED_LEADER, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  LogRoomsA_ShowMessageAndWait(ACTOR_SUSPECTED_LEADER, 20);
  Actor_SetSpeed(ACTOR_SUSPECTED_LEADER, PIXELS(4), PIXELS(2));
  Actor_SetSpeed(ACTOR_SUSPECTED_THIEF, PIXELS(7) / 2, PIXELS(7) / 4);
  Actor_SetScript(ACTOR_SUSPECTED_THIEF, gLogRoomsA_SuspectToSouthWestCorner);
  Actor_SetScript(ACTOR_SUSPECTED_LEADER, gLogRoomsA_SuspectRoundToWestBedHead);
  Actor_RefreshSelectorById(ACTOR_SUSPECTED_LEADER);
  {
    struct Actor *suspect = Actor_GetById(ACTOR_SUSPECTED_LEADER);
    u16 routeIndex = 1;

    suspect->timer = routeIndex;
  }
  {
    struct Actor *suspect = Actor_GetById(ACTOR_SUSPECTED_THIEF);
    u16 routeIndex = 3;

    suspect->timer = routeIndex;
  }
  MapGrid_CopyCellAttributes(14, 48, 4, 1, 14, 44);
}

/// Sets the three party images to the priority used during the mind-reading
/// event.
static inline void LogRoomsA_SetPartyImagePriority(void) {
  {
    struct Sprite *sprite = Actor_GetById(ACTOR_PARTY_LEADER)->sprite;
    sprite->image.priority = 1;
  }
  {
    struct Sprite *sprite = Actor_GetById(ACTOR_GARET)->sprite;
    sprite->image.priority = 1;
  }
  {
    struct Sprite *sprite = Actor_GetById(ACTOR_IVAN)->sprite;
    sprite->image.priority = 1;
  }
}

/// Once a suspect is cornered, Garet calls Ivan and Ivan reads their minds.
/// This is in the guests' room at Vault's inn. "Uh-oh! I'm surrounded!" "Now,
/// Ivan!"
///
/// Reached through raised triggers 201 to 203, which
/// LogRoomsA_UpdateActorStops raises after thirty frames with at
/// least one suspect blocked: 201 the leader, 202 the thief, 203 both. Stops
/// the chase, clears FLAG_MENUS_RAISE_TRIGGER and
/// FLAG_VAULT_CORNERING_SUSPECTS, plays the party's talk
/// (LogRoomsA_DiscussMindReading) and sets FLAG_VAULT_SUSPECTS_MIND_READ.
static void LogRoomsA_ReadCorneredSuspectsMinds(void) {
  s32 actorId;
  struct FieldWork *work;

  work = gFieldWork;
  Event_Start();
  Task_Remove(LogRoomsA_UpdateActorStops);
  GameFlag_Clear(FLAG_MENUS_RAISE_TRIGGER);
  GameFlag_Clear(FLAG_VAULT_CORNERING_SUSPECTS);
  Import_ActorSetAnimation(ACTOR_SUSPECTED_LEADER, ANIM_STAND);
  Import_ActorSetAnimation(ACTOR_SUSPECTED_THIEF, ANIM_STAND);
  Import_ActorSetAnimation(ACTOR_IVAN, ANIM_STAND);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_STAND);
  Actor_FaceActor(ACTOR_GARET, ACTOR_IVAN, 0);
  Actor_TurnToFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
  Actor_FaceActor(ACTOR_SUSPECTED_LEADER, ACTOR_IVAN, 0);
  Actor_FaceActor(ACTOR_SUSPECTED_THIEF, ACTOR_IVAN, 0);
  Event_Wait(10);
  actorId = ACTOR_SUSPECTED_LEADER;
  // The event identifies the suspects blocked on the last frame of the wait.
  switch (work->raisedTrigger) {
  case LOG_ROOMS_A_THIEF_CORNERED:
  case LOG_ROOMS_A_BOTH_CORNERED:
    Event_SetMessageId(MsgVaultImSurrounded);
    Actor_SetAttachedEffect(ACTOR_SUSPECTED_THIEF,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Actor_RunRepeatedMotion(ACTOR_SUSPECTED_THIEF, 2);
    LogRoomsA_ShowMessageAndWait(ACTOR_SUSPECTED_THIEF, 20);
    if (work->raisedTrigger == LOG_ROOMS_A_THIEF_CORNERED) {
      actorId = ACTOR_SUSPECTED_THIEF;
      break;
    }
    // When both are cornered, the leader's reaction follows the thief's.
    // fall through
  case LOG_ROOMS_A_LEADER_CORNERED:
    Event_SetMessageId(MsgVaultTheresNowhereRun);
    Actor_SetAttachedEffect(ACTOR_SUSPECTED_LEADER,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Actor_RunRepeatedMotion(ACTOR_SUSPECTED_LEADER, 2);
    actorId = ACTOR_SUSPECTED_LEADER;
    LogRoomsA_ShowMessageAndWait(ACTOR_SUSPECTED_LEADER, 20);
    break;
  }
  Import_ActorSetAnimation(ACTOR_IVAN, ANIM_STAND);
  Actor_FaceActor(ACTOR_IVAN, actorId, 0);
  Actor_FaceActor(ACTOR_GARET, ACTOR_IVAN, 0);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Event_Wait(20);
  // "Now, Ivan!" "A-All right!" Ivan reads the cornered one's mind while the
  // two wonder what he is doing.
  Event_SetMessageId(MsgVaultNowIvan);
  LogRoomsA_ShowMessageAndWait(ACTOR_GARET, 20);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
  Event_Wait(20);
  LogRoomsA_ShowMessageAndWait(ACTOR_IVAN, 20);
  LogRoomsA_StartMindRead(ACTOR_IVAN, actorId);
  FieldPsynergy_RestoreMindReadPalette();
  Event_Wait(60);
  Actor_RunRepeatedMotion(ACTOR_SUSPECTED_LEADER, 2);
  LogRoomsA_ShowMessageAndWait(ACTOR_SUSPECTED_LEADER, 20);
  Actor_RunRepeatedMotion(ACTOR_SUSPECTED_THIEF, 2);
  LogRoomsA_ShowMessageAndWait(ACTOR_SUSPECTED_THIEF, 20);
  // "Are you done, Ivan?" He nods, and the window closes.
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  LogRoomsA_EndMindRead();
  Actor_StartRepeatedMotion(ACTOR_GARET, 2);
  Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Event_Wait(20);
  LogRoomsA_ShowMessageAndWait(ACTOR_GARET, 20);
  Actor_FaceActor(ACTOR_IVAN, ACTOR_GARET, 0);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  Event_Wait(40);
  LogRoomsA_SetPartyImagePriority();
  gFieldWork->transitionFrames = 24;
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  // Move the discussion away from the suspects while the screen is closed.
  LogRoomsA_DiscussMindReading();
  // The suspects stay at their table, turned toward the leader from now on.
  MapGrid_CopyCellAttributes(14, 45, 3, 1, 14, 44);
  GameFlag_Set(FLAG_VAULT_SUSPECTS_MIND_READ);
  {
    struct Actor *suspect = Actor_GetById(ACTOR_SUSPECTED_LEADER);
    s32 routeIndex = 5;

    suspect->timer = routeIndex;
  }
  {
    struct Actor *suspect = Actor_GetById(ACTOR_SUSPECTED_THIEF);
    s32 routeIndex = 4;

    suspect->timer = routeIndex;
  }
  Task_AddFrame(LogRoomsA_FaceSuspectsTowardLeader, TASK_PASS_FRAME);
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  Event_End();
}

/// Shows the party's branching conversation about what Ivan learned: the
/// stolen goods are hidden in the inn, which they must search.
///
/// Called by LogRoomsA_ReadCorneredSuspectsMinds while the screen is closed;
/// sets the party outside the room and opens the screen on them.
static void LogRoomsA_DiscussMindReading(void) {
  s32 persistedWithQuestions;

  // Move the party outside the room and the suspects back to their table.
  Scene_PlayMusic();
  Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 1);
  {
    struct Actor *leader = Actor_GetById(ACTOR_PARTY_LEADER);

    leader->priorityFlags = ACTOR_PRIORITY_AUTOMATIC | leader->priorityFlags;
  }
  Field_SetCameraBounds(PIXELS(32), PIXELS(576), PIXELS(400), PIXELS(936));
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(ACTOR_GARET, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(ACTOR_IVAN, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(248), PIXELS(728));
  Actor_SetPosition(ACTOR_IVAN, PIXELS(264), PIXELS(744));
  Import_ActorSetPosition(ACTOR_GARET, PIXELS(232), PIXELS(744));
  LogRoomsA_SetPartyImagePriority();
  Actor_TurnToFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
  LogRoomsA_FaceActorAndWait(ACTOR_GARET, ACTOR_IVAN, 30);
  Actor_SetPosition(ACTOR_SUSPECTED_LEADER, PIXELS(104), PIXELS(696));
  Actor_SetPosition(ACTOR_SUSPECTED_THIEF, PIXELS(120), PIXELS(696));
  Actor_TurnToFaceEachOther(ACTOR_SUSPECTED_LEADER, ACTOR_SUSPECTED_THIEF, 0);
  FieldCamera_FollowActor(ACTOR_PARTY_LEADER, 0);
  FieldCamera_WaitForArrival();
  MapGrid_CenterOnFollowedPoint();
  Event_Wait(30);
  {
    struct FieldWork *work = gFieldWork;

    work->transitionFrames = 24;
    work->transition =
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
  }
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(40);
  // "What did you learn, Ivan?" It is them: they hid the stolen goods
  // somewhere in this inn, but won't let him near again.
  Actor_RunRepeatedMotion(ACTOR_GARET, 1);
  Event_Wait(10);
  Event_SetMessageId(MsgVaultLearn);
  LogRoomsA_ShowMessageAndWait(ACTOR_GARET, 20);
  LogRoomsA_PlayAnimationAndWait(ACTOR_IVAN, ANIM_NOD, 20);
  LogRoomsA_ShowMessageAndWait(ACTOR_IVAN, 20);
  LogRoomsA_FaceEachOtherAndWait(ACTOR_PARTY_LEADER, ACTOR_GARET, 50);
  Actor_SetAngleToward(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
  LogRoomsA_FaceActorAndWait(ACTOR_GARET, ACTOR_IVAN, 20);
  LogRoomsA_PlayAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD, 20);
  Event_ShowMessageAndWait(ACTOR_IVAN, 0);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Event_Wait(60);
  LogRoomsA_PlayAnimationAndWait(ACTOR_IVAN, ANIM_NOD, 20);
  LogRoomsA_ShowMessageAndWait(ACTOR_IVAN, 30);
  Actor_RunRepeatedMotion(ACTOR_GARET, 1);
  Event_Wait(20);
  LogRoomsA_FaceEachOtherAndWait(ACTOR_PARTY_LEADER, ACTOR_GARET, 20);
  // Garet: they have to search the inn. Each no leads to another plea, from
  // Garet and then Ivan; three noes and Garet vouches for Isaac's sense of
  // duty, while any yes gets "Leave it to us!"
  FieldMessage_ShowAtActor(ACTOR_GARET, 0);
  persistedWithQuestions = 0;
  if (Event_AskYesNoWithActorAnimation(0, 0) != 0) {
    Event_Wait(20);
    Actor_StartRepeatedMotion(ACTOR_IVAN, 2);
    LogRoomsA_PlayAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD, 30);
    FieldMessage_ShowAtActor(ACTOR_GARET, 0);
    persistedWithQuestions = 0;
    if (Event_AskYesNoWithActorAnimation(0, 0) != 0) {
      Event_Wait(20);
      FieldEffect_SpawnAttached(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
      Event_Wait(60);
      Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
      LogRoomsA_FaceEachOtherAndWait(ACTOR_PARTY_LEADER, ACTOR_IVAN, 20);
      FieldMessage_ShowAtActor(ACTOR_IVAN, 0);
      persistedWithQuestions = 0;
      if (Event_AskYesNoWithActorAnimation(2, 0) != 0) {
        Event_Wait(20);
        FieldEffect_SpawnAttached(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS,
                                  0);
        Event_Wait(60);
        LogRoomsA_ShowMessageAndWait(ACTOR_IVAN, 20);
        LogRoomsA_FaceActorAndWait(ACTOR_GARET, ACTOR_IVAN, 10);
        Actor_RunRepeatedMotion(ACTOR_GARET, 1);
        Event_Wait(10);
        LogRoomsA_ShowMessageAndWait(ACTOR_GARET, 10);
        LogRoomsA_FaceActorAndWait(ACTOR_IVAN, ACTOR_GARET, 20);
        FieldEffect_SpawnAttached(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION,
                                  0);
        Event_Wait(60);
        LogRoomsA_PlayAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD, 20);
        LogRoomsA_ShowMessageAndWait(ACTOR_GARET, 10);
        LogRoomsA_FaceEachOtherAndWait(ACTOR_GARET, ACTOR_PARTY_LEADER, 20);
        LogRoomsA_PlayAnimationAndWait(ACTOR_GARET, ANIM_NOD, 20);
        persistedWithQuestions = 1;
        LogRoomsA_ShowMessageAndWait(ACTOR_GARET, 20);
      }
    }
  }
  if (persistedWithQuestions == 0) {
    Event_SetMessageId(MsgVaultLeave);
    LogRoomsA_ShowMessageAndWait(ACTOR_GARET, 20);
    LogRoomsA_FaceEachOtherAndWait(ACTOR_GARET, ACTOR_PARTY_LEADER, 20);
    LogRoomsA_ShowMessageAndWait(ACTOR_GARET, 20);
  }
  // "Let's go, Ivan." Garet and Ivan walk back into the leader.
  FieldEffect_SpawnAttached(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS,
                            0);
  Event_Wait(60);
  Actor_RunRepeatedMotion(ACTOR_GARET, 1);
  LogRoomsA_ShowMessageAndWait(ACTOR_GARET, 10);
  LogRoomsA_PlayAnimationAndWait(ACTOR_GARET, ANIM_NOD, 10);
  Actor_TurnToFaceEachOther(ACTOR_GARET, ACTOR_IVAN, 0);
  LogRoomsA_FaceEachOtherAndWait(ACTOR_PARTY_LEADER, ACTOR_IVAN, 10);
  Event_ShowMessageAndWait(ACTOR_GARET, 0);
  LogRoomsA_PlayAnimationAndWait(ACTOR_IVAN, ANIM_NOD, 10);
  Actor_WalkTo(ACTOR_IVAN, 248, 728);
  Actor_WalkToAndWait(ACTOR_GARET, 248, 728);
  Import_ActorSetPosition(ACTOR_GARET, 0, 0);
  Import_ActorSetPosition(ACTOR_IVAN, 0, 0);
}

/// The fleeing suspects move on to their next spots.
/// This is in the guests' room at Vault's inn, when the leader steps on one of
/// the route cells, CELL_BEHAVIOR_LOG_ROOMS_A_FIRST_SUSPECT_ROUTE to
/// _LAST_SUSPECT_ROUTE.
///
/// Reached by stepping on one of them while FLAG_SUSPECTS_FLEEING is
/// set.
static void LogRoomsA_SuspectsRunFromParty(void) {
  switch (gFieldWork->stepTrigger) {
  case CELL_BEHAVIOR_LOG_ROOMS_A_FIRST_SUSPECT_ROUTE:
    LogRoomsA_AdvanceSuspectRoute(ACTOR_SUSPECTED_LEADER, 1, 2,
                                  gLogRoomsA_SuspectToMiddleBedHead);
    LogRoomsA_AdvanceSuspectRoute(ACTOR_SUSPECTED_THIEF, 3, 4,
                                  gLogRoomsA_SuspectAlongToSouthEastCorner);
    break;
  case CELL_BEHAVIOR_LOG_ROOMS_A_FIRST_SUSPECT_ROUTE + 1:
    LogRoomsA_AdvanceSuspectRoute(ACTOR_SUSPECTED_LEADER, 1, 4,
                                  gLogRoomsA_SuspectRoundToWestBedFoot);
    LogRoomsA_AdvanceSuspectRoute(ACTOR_SUSPECTED_LEADER, 2, 3,
                                  gLogRoomsA_SuspectToEastBedHead);
    LogRoomsA_AdvanceSuspectRoute(ACTOR_SUSPECTED_THIEF, 1, 3,
                                  gLogRoomsA_SuspectToSouthWestCorner);
    break;
  case CELL_BEHAVIOR_LOG_ROOMS_A_FIRST_SUSPECT_ROUTE + 2:
    LogRoomsA_AdvanceSuspectRoute(ACTOR_SUSPECTED_LEADER, 2, 1,
                                  gLogRoomsA_SuspectToWestBedHead);
    LogRoomsA_AdvanceSuspectRoute(ACTOR_SUSPECTED_LEADER, 3, 6,
                                  gLogRoomsA_SuspectRoundToEastBedFoot);
    LogRoomsA_AdvanceSuspectRoute(ACTOR_SUSPECTED_THIEF, 2, 4,
                                  gLogRoomsA_SuspectToSouthEastCorner);
    break;
  case CELL_BEHAVIOR_LOG_ROOMS_A_FIRST_SUSPECT_ROUTE + 3:
    LogRoomsA_AdvanceSuspectRoute(ACTOR_SUSPECTED_LEADER, 3, 2,
                                  gLogRoomsA_SuspectToMiddleBedHead);
    LogRoomsA_AdvanceSuspectRoute(ACTOR_SUSPECTED_THIEF, 4, 3,
                                  gLogRoomsA_SuspectAlongToSouthWestCorner);
    break;
  case CELL_BEHAVIOR_LOG_ROOMS_A_FIRST_SUSPECT_ROUTE + 4:
    LogRoomsA_AdvanceSuspectRoute(ACTOR_SUSPECTED_LEADER, 4, 5,
                                  gLogRoomsA_SuspectToMiddleBedFoot);
    LogRoomsA_AdvanceSuspectRoute(ACTOR_SUSPECTED_THIEF, 1, 2,
                                  gLogRoomsA_SuspectAcrossToEastOfBeds);
    break;
  case CELL_BEHAVIOR_LOG_ROOMS_A_FIRST_SUSPECT_ROUTE + 5:
    LogRoomsA_AdvanceSuspectRoute(ACTOR_SUSPECTED_LEADER, 4, 1,
                                  gLogRoomsA_SuspectRoundToWestBedHead);
    LogRoomsA_AdvanceSuspectRoute(ACTOR_SUSPECTED_LEADER, 5, 6,
                                  gLogRoomsA_SuspectToEastBedFoot);
    LogRoomsA_AdvanceSuspectRoute(ACTOR_SUSPECTED_THIEF, 3, 1,
                                  gLogRoomsA_SuspectToWestOfBeds);
    break;
  case CELL_BEHAVIOR_LOG_ROOMS_A_FIRST_SUSPECT_ROUTE + 6:
    LogRoomsA_AdvanceSuspectRoute(ACTOR_SUSPECTED_LEADER, 5, 4,
                                  gLogRoomsA_SuspectToWestBedFoot);
    LogRoomsA_AdvanceSuspectRoute(ACTOR_SUSPECTED_LEADER, 6, 3,
                                  gLogRoomsA_SuspectRoundToEastBedHead);
    LogRoomsA_AdvanceSuspectRoute(ACTOR_SUSPECTED_THIEF, 4, 2,
                                  gLogRoomsA_SuspectToEastOfBeds);
    break;
  case CELL_BEHAVIOR_LOG_ROOMS_A_LAST_SUSPECT_ROUTE:
    LogRoomsA_AdvanceSuspectRoute(ACTOR_SUSPECTED_LEADER, 6, 5,
                                  gLogRoomsA_SuspectToMiddleBedFoot);
    LogRoomsA_AdvanceSuspectRoute(ACTOR_SUSPECTED_THIEF, 2, 1,
                                  gLogRoomsA_SuspectAcrossToWestOfBeds);
    break;
  }
}

/// Installs a route script only when the suspect is in its expected state.
static void LogRoomsA_AdvanceSuspectRoute(s32 actorId, s32 expected, s32 next,
                                          const s32 *actions) {
  struct Actor *suspect = Actor_GetById(actorId);

  if (suspect->timer == expected) {
    Actor_SetScript(actorId, actions);
    suspect->timer = next;
  }
}

#include "shared/vault_bone_proximity_flag.h"

/// Arriving in any room of Vault's houses, shops and inn: restores the room
/// for the story so far, and plays the loft's events: the tied-up villager, the
/// rematch, and the defeat or victory.
///
/// Called by the field once the actors are placed, by entrance.
/// At the inn, after the thieves are jailed, places the Bone and starts
/// Vault_UpdateBoneProximityFlag. After a lost battle
/// (LOG_ROOMS_A_LOFT_AFTER_LOSING_TO_THIEVES) takes
/// LOG_ROOMS_A_TO_SANCTUM; after a won one (17) sets
/// FLAG_VAULT_THIEVES_JAILED through LogRoomsA_ReturnStolenGoods, which
/// also removes Ivan from the party.
s32 Scene_Initialize(void) {
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  switch (gMainRoutineWork.entrance) {
  case LOG_ROOMS_A_FROM_VAULT_EAST_SOUTHEAST: {
    s32 zero = 0;

    // The gem owner's house: he stands on the floor.
    Actor_GetById(ACTOR_GEM_OWNER)->motionFlags = zero;
    Actor_GetById(ACTOR_GEM_OWNER)->y.fixed = zero;
    Actor_GetById(ACTOR_GEM_OWNER)->groundHeight = zero;
  } break;
  case LOG_ROOMS_A_FROM_VAULT_NORTHWEST:
    // The mayor's house: Ivan waits a step back once met and is gone once he
    // has joined; after the thieves are jailed, the mayor stands by the door.
    if (GameFlag_Get(FLAG_VAULT_MET_IVAN) != 0) {
      Actor_SetPosition(ACTOR_IVAN, PIXELS(376), PIXELS(376));
      Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTH, 0);
    }
    if (GameFlag_Get(FLAG_VAULT_IVAN_JOINED_ROD_SEARCH) != 0) {
      Import_ActorSetPosition(ACTOR_IVAN, 0, 0);
    }
    if (GameFlag_Get(FLAG_VAULT_THIEVES_JAILED) != 0) {
      Actor_SetPosition(ACTOR_MAYOR, PIXELS(536), LOG_ROOMS_A_ENTRY_ACTOR_Z);
      Actor_SetMovementTypeById(ACTOR_MAYOR, MOVEMENT_TYPE_STAND);
      Actor_FaceDirection(ACTOR_MAYOR, FACING_SOUTH + FACING_STEP, 0);
    }
    break;
  case LOG_ROOMS_A_FROM_VAULT_SOUTHWEST:
  case LOG_ROOMS_A_INN_FROM_INN_UPSTAIRS:
    // The inn: once the thieves are jailed, the Bone lies on the floor beside
    // the girl at the inn; after the mind reading and before that, she is
    // out.
    if (GameFlag_Get(FLAG_VAULT_THIEVES_JAILED) != 0) {
      Actor_SetPosition(ACTOR_GIRL_AT_THE_INN, PIXELS(568), PIXELS(648));
      Actor_SetMovementTypeById(ACTOR_GIRL_AT_THE_INN, MOVEMENT_TYPE_STAND);
      Actor_FaceDirection(ACTOR_GIRL_AT_THE_INN, FACING_SOUTH, 0);
      Vault_PlaceItemIcon(ITEM_BONE, PIXELS(568), PIXELS(16), PIXELS(672));
      Task_AddFrame(Vault_UpdateBoneProximityFlag, TASK_PASS_FRAME);
    } else if (GameFlag_Get(FLAG_VAULT_SUSPECTS_MIND_READ) != 0) {
      Import_ActorSetPosition(ACTOR_GIRL_AT_THE_INN, 0, 0);
    }
    break;
  case LOG_ROOMS_A_INN_UPSTAIRS_FROM_INN:
    // The guests' room upstairs at the inn: back from a battle mid-chase, the
    // suspects keep running.
    if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) != 0 &&
        GameFlag_Get(FLAG_VAULT_SUSPECTS_APPROACHED) != 0 &&
        GameFlag_Get(FLAG_VAULT_SUSPECTS_MIND_READ) == 0 &&
        GameFlag_Get(FLAG_SUSPECTS_FLEEING) != 0) {
      MapGrid_CopyCellAttributes(14, 45, 3, 1, 14, 44);
      Task_AddFrame(LogRoomsA_FaceSuspectsTowardLeader, TASK_PASS_FRAME);
      break;
    }
    if (GameFlag_Get(FLAG_VAULT_IVAN_JOINED_ROD_SEARCH) != 0) {
      Actor_SetPosition(ACTOR_SUSPECTED_THIEF, PIXELS(120), PIXELS(696));
      Actor_FaceDirection(ACTOR_SUSPECTED_THIEF, FACING_WEST, 0);
    }
    // Once approached, the room's doorway cell behavior changes; after the
    // mind reading the suspects sit turned toward the leader.
    if (GameFlag_Get(FLAG_VAULT_SUSPECTS_APPROACHED) != 0) {
      MapGrid_CopyCellAttributes(14, 45, 3, 1, 14, 44);
      if (GameFlag_Get(FLAG_VAULT_SUSPECTS_MIND_READ) == 0) {
        MapGrid_CopyCellAttributes(14, 50, 3, 1, 14, 44);
        break;
      }
      Actor_GetById(ACTOR_SUSPECTED_LEADER)->timer = 5;
      Actor_GetById(ACTOR_SUSPECTED_THIEF)->timer = 4;
      Task_AddFrame(LogRoomsA_FaceSuspectsTowardLeader, TASK_PASS_FRAME);
    }
    break;
  case LOG_ROOMS_A_FROM_VAULT_WEST:
  case LOG_ROOMS_A_CRATE_ROOM_FROM_LOFT:
    // The room with the crate: the crate keeps its place once moved, and the
    // passage behind it stays open.
    Event_Wait(2);
    MapGrid_CopyCellsTo(54, 2, 35, 20, 2, 10);
    MapGrid_CopyCellsTo(54, 2, 95, 20, 2, 10);
    MapGrid_CopyCellsTo(54, 2, 35, 80, 2, 10);
    MapGrid_CopyCellsTo(54, 2, 46, 21, 4, 8);
    MapGrid_CopyCellsTo(54, 2, 46, 81, 4, 8);
    Actor_GetById(ACTOR_CRATE)->collisionFlags |= 4;
    Actor_SetSpriteFlags(Actor_GetById(ACTOR_CRATE), 0);
    if (GameFlag_Get(FLAG_VAULT_CRATE_MOVED) != 0) {
      Actor_SetPosition(ACTOR_CRATE, PIXELS(676), PIXELS(411));
      MapGrid_CopyCellAttributes(101, 24, 3, 4, 41, 24);
    }
    break;
  case LOG_ROOMS_A_LOFT_FROM_CRATE_ROOM:
    Event_Wait(2);
    MapGrid_CopyCellsTo(54, 2, 44, 21, 2, 8);
    MapGrid_CopyCellsTo(54, 2, 44, 81, 2, 8);
    Event_Start();
    // The loft: emptied chests after the thieves are jailed; else the tied-up
    // villager lies tied, groaning to SOUND_FADE_OUT_BGM, or the thieves wait
    // for a rematch.
    if (GameFlag_Get(FLAG_VAULT_THIEVES_JAILED) != 0) {
      Import_ActorSetAnimation(ACTOR_FIRST_CHEST, ANIM_WALK);
      Import_ActorSetAnimation(ACTOR_SECOND_CHEST, ANIM_WALK);
      Event_Wait(1);
      Import_ActorSetAnimation(ACTOR_THIRD_CHEST, ANIM_WALK);
      break;
    }
    Import_ActorSetPosition(ACTOR_TIED_UP_VILLAGER, PIXELS(824), PIXELS(456));
    if (GameFlag_Get(FLAG_VAULT_THIEVES_ATTACKED) != 0) {
      LogRoomsA_ConfrontThievesAgain();
      Event_End();
      break;
    }
    Import_ActorSetAnimation(ACTOR_TIED_UP_VILLAGER, 7);
    LogRoomsA_OpenWindowTransition();
    Sound_Play(SOUND_FADE_OUT_BGM);
    Event_SetMessageId(MsgVaultUhnnUhnn);
    LogRoomsA_ShowMessageAndWait(ACTOR_TIED_UP_VILLAGER, 10);
    Event_End();
    break;
  case LOG_ROOMS_A_LOFT_AFTER_LOSING_TO_THIEVES:
    // Lost to the thieves: they gloat, and the party wakes up elsewhere.
    MapGrid_CopyCells(54, 2, 2, 8, 44, 21);
    MapGrid_CopyCells(54, 2, 2, 8, 44, 81);
    Import_ActorSetPosition(ACTOR_TIED_UP_VILLAGER, PIXELS(824), PIXELS(456));
    LogRoomsA_LoseToThieves();
    Import_EventSetExitRequest(LOG_ROOMS_A_TO_SANCTUM);
    break;
  case LOG_ROOMS_A_LOFT_AFTER_THIEVES_DEFEATED:
    // Won: the thieves are caught and the stolen goods handed back.
    MapGrid_CopyCells(54, 2, 2, 8, 44, 21);
    MapGrid_CopyCells(54, 2, 2, 8, 44, 81);
    if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0) {
      Actor_SetPosition(ACTOR_TIED_UP_VILLAGER, PIXELS(824), PIXELS(456));
      LogRoomsA_DefeatThieves();
      LogRoomsA_ReturnStolenGoods();
      break;
    }
    Import_ActorSetAnimation(ACTOR_FIRST_CHEST, ANIM_WALK);
    Import_ActorSetAnimation(ACTOR_SECOND_CHEST, ANIM_WALK);
    Event_Wait(1);
    Import_ActorSetAnimation(ACTOR_THIRD_CHEST, ANIM_WALK);
    break;
  }
  return 0;
}

/// Keeps both suspects facing the leader while they evade the party.
///
/// Run each frame as a task, from LogRoomsA_ApproachSuspects until
/// LogRoomsA_DiscussSuspiciousGuests, and again once their minds are read.
static void LogRoomsA_FaceSuspectsTowardLeader(void) {
  struct Actor *origin = Actor_GetById(ACTOR_PARTY_LEADER);
  struct Actor *first = Actor_GetById(ACTOR_SUSPECTED_LEADER);
  struct Actor *second = Actor_GetById(ACTOR_SUSPECTED_THIEF);

  first->facing = ArcTan2(origin->z.fixed - first->z.fixed,
                          origin->x.fixed - first->x.fixed);
  second->facing = ArcTan2(origin->z.fixed - second->z.fixed,
                           origin->x.fixed - second->x.fixed);
}

/// Shows the thieves' reaction after the party loses their battle: the party
/// lies beaten, and the thieves hold it and the tied-up villager as hostages.
///
/// Called by Scene_Initialize at LOG_ROOMS_A_LOFT_AFTER_LOSING_TO_THIEVES,
/// the battle's defeat destination; the event then takes
/// LOG_ROOMS_A_TO_SANCTUM to the Vault Sanctum (SceneId_Sanctum,
/// SANCTUM_FROM_LOG_ROOMS_A).
static void LogRoomsA_LoseToThieves(void) {
  struct Actor *actor;
  s32 priorityFlags;

  // The party lies on the floor (animation 19) before the three thieves.
  Event_Start();
  Actor_SetPosition(ACTOR_THIEF_LEADER, PIXELS(792), PIXELS(416));
  Actor_SetPosition(ACTOR_FIRST_THIEF, PIXELS(800), PIXELS(400));
  Actor_SetPosition(ACTOR_SECOND_THIEF, PIXELS(776), PIXELS(416));
  Actor_FaceDirection(ACTOR_THIEF_LEADER, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_FIRST_THIEF, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_SECOND_THIEF, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_SetSpeed(ACTOR_FIRST_THIEF, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(ACTOR_SECOND_THIEF, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(784), PIXELS(448));
  Actor_SetPosition(ACTOR_GARET, PIXELS(808), PIXELS(432));
  Actor_SetPosition(ACTOR_IVAN, PIXELS(776), PIXELS(432));
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_COLLAPSE);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_COLLAPSE);
  Import_ActorSetAnimation(ACTOR_IVAN, ANIM_COLLAPSE);
  priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  Actor_GetById(ACTOR_PARTY_LEADER)->priorityFlags = priorityFlags;
  Actor_GetById(ACTOR_GARET)->priorityFlags = priorityFlags;
  Actor_GetById(ACTOR_IVAN)->priorityFlags = priorityFlags;
  actor = Actor_GetById(ACTOR_PARTY_LEADER);
  Actor_SetSpriteFlags(actor, 0);
  actor = Actor_GetById(ACTOR_IVAN);
  Actor_SetSpriteFlags(actor, 0);
  actor = Actor_GetById(ACTOR_GARET);
  Actor_SetSpriteFlags(actor, 0);
  Actor_FaceDirection(ACTOR_TIED_UP_VILLAGER, FACING_NORTHWEST + FACING_STEP,
                      0);
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  FieldCamera_FollowActor(ACTOR_PARTY_LEADER, 0);
  FieldCamera_WaitForArrival();
  MapGrid_CenterOnFollowedPoint();
  LogRoomsA_OpenWindowTransition();
  Event_Wait(60);
  // "These kids are nothing to worry about." The tied-up villager groans.
  LogRoomsA_PlayAnimationAndWait(ACTOR_THIEF_LEADER, ANIM_NOD, 20);
  Event_SetMessageId(MsgVaultTheseKidsNothingWorryAbout);
  LogRoomsA_ShowMessageAndWait(ACTOR_THIEF_LEADER, 30);
  LogRoomsA_ShowMessageAndWait(ACTOR_TIED_UP_VILLAGER, 30);
  // The two men step to the tied-up villager: he stays as their hostage.
  Actor_WalkTo(ACTOR_FIRST_THIEF, 808, 456);
  Actor_WalkTo(ACTOR_SECOND_THIEF, 792, 456);
  Actor_WaitForMoveById(ACTOR_SECOND_THIEF);
  Actor_FaceDirection(ACTOR_SECOND_THIEF, FACING_EAST, 0);
  Actor_WaitForMoveById(ACTOR_FIRST_THIEF);
  Actor_FaceDirection(ACTOR_FIRST_THIEF, FACING_EAST, 0);
  Event_Wait(30);
  LogRoomsA_PlayAnimationAndWait(ACTOR_FIRST_THIEF, ANIM_NOD, 20);
  LogRoomsA_ShowMessageAndWait(ACTOR_FIRST_THIEF, 20);
  LogRoomsA_FaceActorAndWait(ACTOR_SECOND_THIEF, ACTOR_PARTY_LEADER, 30);
  LogRoomsA_ShowMessageAndWait(ACTOR_SECOND_THIEF, 60);
  Event_End();
}

/// Arranges the loft confrontation and restarts the thieves' battle. "They
/// won't go away, will they? They're back!"
///
/// Called by Scene_Initialize at LOG_ROOMS_A_LOFT_FROM_CRATE_ROOM once
/// FLAG_VAULT_THIEVES_ATTACKED is set. Starts the same battle as
/// LogRoomsA_FreeRooferAndConfrontThieves.
static void LogRoomsA_ConfrontThievesAgain(void) {

  // The thieves stand over the tied-up villager as the party comes up.
  Actor_SetPosition(ACTOR_THIEF_LEADER, PIXELS(792), PIXELS(416));
  Actor_SetPosition(ACTOR_FIRST_THIEF, PIXELS(800), PIXELS(400));
  Actor_SetPosition(ACTOR_SECOND_THIEF, PIXELS(776), PIXELS(416));
  Actor_FaceDirection(ACTOR_THIEF_LEADER, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_FIRST_THIEF, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_SECOND_THIEF, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(792), PIXELS(440));
  Actor_SetPosition(ACTOR_GARET, PIXELS(808), PIXELS(432));
  Actor_SetPosition(ACTOR_IVAN, PIXELS(792), PIXELS(456));
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_FaceActor(ACTOR_TIED_UP_VILLAGER, ACTOR_THIEF_LEADER, 0);
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  FieldCamera_FollowActor(ACTOR_PARTY_LEADER, 0);
  FieldCamera_WaitForArrival();
  MapGrid_CenterOnFollowedPoint();
  wait(1);
  gFieldWork->transitionFrames = 32;
  LogRoomsA_OpenWindowTransition();
  Event_Wait(60);
  Event_SetMessageId(MsgVaultWontGoAway);
  Actor_RunRepeatedMotion(ACTOR_FIRST_THIEF, 1);
  Event_Wait(20);
  LogRoomsA_ShowMessageAndWait(ACTOR_FIRST_THIEF, 30);
  Actor_RunRepeatedMotion(ACTOR_SECOND_THIEF, 1);
  Event_Wait(20);
  Event_ShowMessage(ACTOR_SECOND_THIEF, 0);
  LogRoomsA_FaceEachOtherAndWait(ACTOR_THIEF_LEADER, ACTOR_FIRST_THIEF, 30);
  Import_ActorSetAnimation(ACTOR_THIEF_LEADER, ANIM_NOD);
  LogRoomsA_PlayAnimationAndWait(ACTOR_FIRST_THIEF, ANIM_NOD, 30);
  LogRoomsA_FaceEachOtherAndWait(ACTOR_THIEF_LEADER, ACTOR_SECOND_THIEF, 30);
  Import_ActorSetAnimation(ACTOR_THIEF_LEADER, ANIM_NOD);
  LogRoomsA_PlayAnimationAndWait(ACTOR_SECOND_THIEF, ANIM_NOD, 40);
  Actor_FaceDirection(ACTOR_THIEF_LEADER, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_FIRST_THIEF, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_SECOND_THIEF, FACING_SOUTHEAST + FACING_STEP, 0);
  Event_Wait(20);
  // "We'll make you sorry you were ever born! Get 'em!!" and the battle.
  LogRoomsA_PlayAnimationAndWait(ACTOR_THIEF_LEADER, ANIM_SHAKE_HEAD, 20);
  Event_ShowMessage(ACTOR_THIEF_LEADER, 0);
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
  Event_SetAfterBattleDestination((s32)&SceneId_LogRoomsA,
                                  LOG_ROOMS_A_LOFT_AFTER_THIEVES_DEFEATED);
  Event_SetDefeatDestination((s32)&SceneId_LogRoomsA,
                             LOG_ROOMS_A_LOFT_AFTER_LOSING_TO_THIEVES);
  Battle_SetType(BATTLE_TYPE_SCRIPTED);
  Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED_VALE_VAULT, 5);
}

/// Counts the Venus Djinn Ivan holds, which he gives back before he leaves the
/// party.
static s32 LogRoomsA_GetIvanVenusDjinnCount(void) {
  return Unit_Get(IVAN)->djinnHeldCounts[ELEMENT_VENUS];
}

#include "shared/vault_djinn.h"

/// Stages the defeated thieves and gathers the townspeople in the loft.
///
/// Called by Scene_Initialize at LOG_ROOMS_A_LOFT_AFTER_THIEVES_DEFEATED,
/// the battle's victory destination, unless the party is only coming back to
/// where it was; then LogRoomsA_ReturnStolenGoods follows.
static void LogRoomsA_DefeatThieves(void) {
  struct Actor *actor;
  struct FieldWork *work;
  const s32 *actions;
  struct Sprite *thirdThiefSprite;

  actor = Actor_GetById(ACTOR_SECOND_THIEF);
  thirdThiefSprite = actor->sprite;
  // The beaten thieves lie heaving (animation 9 and the action table's
  // squash) before the party.
  Event_Start();
  Actor_SetPosition(ACTOR_THIEF_LEADER, PIXELS(792), PIXELS(416));
  Actor_SetPosition(ACTOR_FIRST_THIEF, PIXELS(800), PIXELS(400));
  Actor_SetPosition(ACTOR_SECOND_THIEF, PIXELS(776), PIXELS(392));
  actor = Actor_GetById(ACTOR_THIEF_LEADER);
  Actor_SetSpriteFlags(actor, 0);
  actor = Actor_GetById(ACTOR_FIRST_THIEF);
  Actor_SetSpriteFlags(actor, 0);
  actor = Actor_GetById(ACTOR_SECOND_THIEF);
  Actor_SetSpriteFlags(actor, 0);
  Import_ActorSetAnimation(ACTOR_THIEF_LEADER, 9);
  Import_ActorSetAnimation(ACTOR_FIRST_THIEF, 9);
  Import_ActorSetAnimation(ACTOR_SECOND_THIEF, 9);
  Actor_GetById(ACTOR_SECOND_THIEF)->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  thirdThiefSprite->image.priority = 3;
  actions = gLogRoomsA_ActionTable;
  Actor_SetScript(ACTOR_THIEF_LEADER, actions);
  Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(792), PIXELS(440));
  Actor_SetPosition(ACTOR_GARET, PIXELS(808), PIXELS(432));
  Actor_SetPosition(ACTOR_IVAN, PIXELS(776), PIXELS(440));
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_SetSpeed(ACTOR_TIED_UP_VILLAGER, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_FaceDirection(ACTOR_TIED_UP_VILLAGER, FACING_NORTHWEST + FACING_STEP,
                      0);
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  FieldCamera_FollowActor(ACTOR_PARTY_LEADER, 0);
  FieldCamera_WaitForArrival();
  MapGrid_CenterOnFollowedPoint();
  LogRoomsA_OpenWindowTransition();
  Actor_SetScript(ACTOR_FIRST_THIEF, actions);
  Event_Wait(30);
  Actor_SetScript(ACTOR_SECOND_THIEF, actions);
  Event_Wait(30);
  // "They... They got us..." The tied-up villager thought they were goners; he
  // slips past Garet, cheers the win and runs for the mayor.
  Event_SetMessageId(MsgVaultTheyTheyGotUs);
  LogRoomsA_ShowMessageAndWait(ACTOR_THIEF_LEADER, 20);
  Actor_ShowEmote(ACTOR_TIED_UP_VILLAGER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Event_Wait(60);
  LogRoomsA_ShowMessageAndWait(ACTOR_TIED_UP_VILLAGER, 20);
  Actor_WalkToAndWait(ACTOR_TIED_UP_VILLAGER, 808, 456);
  Actor_SetSpeed(ACTOR_GARET, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(ACTOR_GARET, 792, 432);
  Actor_FaceDirection(ACTOR_GARET, FACING_EAST, 0);
  Actor_WalkToAndWait(ACTOR_TIED_UP_VILLAGER, 808, 408);
  Actor_FaceDirection(ACTOR_TIED_UP_VILLAGER, FACING_WEST, 0);
  Actor_WalkToAndWait(ACTOR_GARET, 808, 432);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Event_Wait(20);
  LogRoomsA_PlayAnimationAndWait(ACTOR_TIED_UP_VILLAGER, ANIM_NOD, 20);
  LogRoomsA_ShowMessageAndWait(ACTOR_TIED_UP_VILLAGER, 20);
  Actor_WalkToAndWait(ACTOR_TIED_UP_VILLAGER, 768, 408);
  Event_Wait(20);
  Actor_FaceActor(ACTOR_TIED_UP_VILLAGER, ACTOR_PARTY_LEADER, 0);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_TIED_UP_VILLAGER, 0);
  Actor_FaceActor(ACTOR_GARET, ACTOR_TIED_UP_VILLAGER, 0);
  LogRoomsA_FaceActorAndWait(ACTOR_IVAN, ACTOR_TIED_UP_VILLAGER, 40);
  LogRoomsA_ShowMessageAndWait(ACTOR_TIED_UP_VILLAGER, 30);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_NOD);
  LogRoomsA_PlayAnimationAndWait(ACTOR_IVAN, ANIM_NOD, 20);
  Actor_WalkToAndWait(ACTOR_TIED_UP_VILLAGER, 744, 408);
  Event_Wait(50);
  // The thieves grumble; Garet: stealing is wrong, evil never prevails.
  Actor_RunRepeatedMotion(ACTOR_FIRST_THIEF, 2);
  LogRoomsA_ShowMessageAndWait(ACTOR_FIRST_THIEF, 20);
  Actor_SetScript(ACTOR_FIRST_THIEF, actions);
  Actor_RunRepeatedMotion(ACTOR_GARET, 1);
  Event_Wait(20);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_FIRST_THIEF, 0);
  Actor_FaceActor(ACTOR_GARET, ACTOR_FIRST_THIEF, 0);
  LogRoomsA_FaceActorAndWait(ACTOR_IVAN, ACTOR_FIRST_THIEF, 20);
  LogRoomsA_ShowMessageAndWait(ACTOR_GARET, 20);
  Actor_RunRepeatedMotion(ACTOR_SECOND_THIEF, 2);
  LogRoomsA_ShowMessageAndWait(ACTOR_SECOND_THIEF, 30);
  Actor_SetScript(ACTOR_SECOND_THIEF, actions);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
  Event_Wait(60);
  LogRoomsA_ShowMessageAndWait(ACTOR_GARET, 30);
  // Ivan will take the rod to Hammet in Lunpa, and asks what the two will do:
  // go after Saturos and the others.
  LogRoomsA_FaceActorAndWait(ACTOR_IVAN, ACTOR_PARTY_LEADER, 30);
  LogRoomsA_FaceActorAndWait(ACTOR_PARTY_LEADER, ACTOR_IVAN, 30);
  LogRoomsA_PlayAnimationAndWait(ACTOR_IVAN, ANIM_NOD, 20);
  LogRoomsA_ShowMessageAndWait(ACTOR_IVAN, 20);
  LogRoomsA_FaceActorAndWait(ACTOR_GARET, ACTOR_IVAN, 30);
  LogRoomsA_PlayAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD, 30);
  LogRoomsA_ShowMessageAndWait(ACTOR_IVAN, 20);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Event_Wait(60);
  LogRoomsA_ShowMessageAndWait(ACTOR_GARET, 30);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Event_Wait(60);
  LogRoomsA_FaceEachOtherAndWait(ACTOR_PARTY_LEADER, ACTOR_GARET, 20);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  LogRoomsA_PlayAnimationAndWait(ACTOR_GARET, ANIM_NOD, 20);
  // Ivan reads Isaac's mind. Garet: "Shouldn't we stop him?" Yes: too late,
  // he's done. No: "If you say so."
  LogRoomsA_StartMindRead(ACTOR_IVAN, ACTOR_PARTY_LEADER);
  FieldPsynergy_RestoreMindReadPalette();
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
  LogRoomsA_FaceActorAndWait(ACTOR_GARET, ACTOR_IVAN, 10);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  Actor_RunRepeatedMotion(ACTOR_GARET, 1);
  Event_Wait(20);
  Actor_SetAttachedEffect(ACTOR_GARET,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(60);
  LogRoomsA_FaceActorAndWait(ACTOR_PARTY_LEADER, ACTOR_GARET, 10);
  Event_OpenMessage(ACTOR_GARET, 0);
  if (Event_AskYesNoWithActorAnimation(0, 0) == 0) {
    Event_Wait(20);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
    LogRoomsA_FaceActorAndWait(ACTOR_GARET, ACTOR_IVAN, 20);
    LogRoomsA_EndMindRead();
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Actor_RunRepeatedMotion(ACTOR_GARET, 1);
    Event_Wait(20);
    LogRoomsA_ShowMessageAndWait(ACTOR_GARET, 20);
  } else {
    gFieldWork->message++;
    Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    Event_Wait(60);
    LogRoomsA_ShowMessageAndWait(ACTOR_GARET, 20);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
    LogRoomsA_FaceActorAndWait(ACTOR_GARET, ACTOR_IVAN, 20);
    LogRoomsA_EndMindRead();
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Actor_RunRepeatedMotion(ACTOR_GARET, 1);
    Event_Wait(20);
  }
  // "I see... So that's what happened..." The tied-up villager comes back with
  // the mayor and two townsmen.
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Event_Wait(60);
  LogRoomsA_PlayAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD, 20);
  Event_SetMessageId(MsgVaultSeeThatsHappened);
  LogRoomsA_ShowMessageAndWait(ACTOR_IVAN, 20);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  LogRoomsA_PlayAnimationAndWait(ACTOR_GARET, ANIM_NOD, 40);
  Actor_WalkToAndWait(ACTOR_TIED_UP_VILLAGER, 808, 408);
  Actor_FaceDirection(ACTOR_TIED_UP_VILLAGER, FACING_WEST, 0);
  Event_Wait(30);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_TIED_UP_VILLAGER, 0);
  Actor_FaceActor(ACTOR_GARET, ACTOR_TIED_UP_VILLAGER, 0);
  LogRoomsA_FaceActorAndWait(ACTOR_IVAN, ACTOR_TIED_UP_VILLAGER, 20);
  Actor_RunRepeatedMotion(ACTOR_TIED_UP_VILLAGER, 1);
  Event_Wait(20);
  LogRoomsA_ShowMessageAndWait(ACTOR_TIED_UP_VILLAGER, 20);
  Actor_SetSpeed(ACTOR_LOFT_MAYOR, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(ACTOR_LOFT_ESCORT, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(ACTOR_STRONGMAN_OPENING_THE_CHESTS, PIXELS(4) / 5,
                 PIXELS(2) / 5);
  Actor_SetPosition(ACTOR_LOFT_MAYOR, PIXELS(744), PIXELS(408));
  Actor_WalkToAndWait(ACTOR_LOFT_MAYOR, 768, 408);
  LogRoomsA_FaceActorAndWait(ACTOR_LOFT_MAYOR, ACTOR_THIEF_LEADER, 30);
  Actor_SetPosition(ACTOR_LOFT_ESCORT, PIXELS(744), PIXELS(408));
  Actor_WalkToAndWait(ACTOR_LOFT_ESCORT, 768, 408);
  Actor_SetPosition(ACTOR_STRONGMAN_OPENING_THE_CHESTS, PIXELS(744),
                    PIXELS(408));
  Actor_WalkTo(ACTOR_STRONGMAN_OPENING_THE_CHESTS, 784, 400);
  Actor_WalkToAndWait(ACTOR_LOFT_ESCORT, 776, 424);
  Actor_WaitForMoveById(ACTOR_STRONGMAN_OPENING_THE_CHESTS);
  Actor_FaceActor(ACTOR_LOFT_ESCORT, ACTOR_THIEF_LEADER, 0);
  LogRoomsA_FaceActorAndWait(ACTOR_STRONGMAN_OPENING_THE_CHESTS,
                             ACTOR_THIEF_LEADER, 20);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_LOFT_MAYOR, 0);
  Actor_FaceActor(ACTOR_GARET, ACTOR_LOFT_MAYOR, 0);
  LogRoomsA_FaceActorAndWait(ACTOR_IVAN, ACTOR_LOFT_MAYOR, 20);
  // The mayor scolds them for stealing in a disaster; the thieves talk back.
  LogRoomsA_PlayAnimationAndWait(ACTOR_LOFT_MAYOR, ANIM_SHAKE_HEAD, 20);
  LogRoomsA_ShowMessageAndWait(ACTOR_LOFT_MAYOR, 20);
  LogRoomsA_ShowMessageAndWait(ACTOR_FIRST_THIEF, 20);
  LogRoomsA_ShowMessageAndWait(ACTOR_SECOND_THIEF, 30);
  // "What should we do with them?" Locked up for a long, long time; the two
  // townsmen rouse the thieves to tie them up.
  LogRoomsA_FaceEachOtherAndWait(ACTOR_LOFT_MAYOR, ACTOR_LOFT_ESCORT, 20);
  Actor_RunRepeatedMotion(ACTOR_LOFT_ESCORT, 1);
  Event_Wait(20);
  LogRoomsA_ShowMessageAndWait(ACTOR_LOFT_ESCORT, 20);
  LogRoomsA_PlayAnimationAndWait(ACTOR_LOFT_MAYOR, ANIM_NOD, 30);
  LogRoomsA_FaceEachOtherAndWait(ACTOR_LOFT_MAYOR,
                                 ACTOR_STRONGMAN_OPENING_THE_CHESTS, 20);
  LogRoomsA_PlayAnimationAndWait(ACTOR_LOFT_MAYOR, ANIM_NOD, 30);
  LogRoomsA_FaceActorAndWait(ACTOR_LOFT_MAYOR, ACTOR_THIEF_LEADER, 20);
  LogRoomsA_ShowMessageAndWait(ACTOR_LOFT_MAYOR, 20);
  Import_ActorSetAnimation(ACTOR_LOFT_ESCORT, ANIM_NOD);
  LogRoomsA_PlayAnimationAndWait(ACTOR_STRONGMAN_OPENING_THE_CHESTS, ANIM_NOD,
                                 20);
  LogRoomsA_FaceEachOtherAndWait(ACTOR_LOFT_ESCORT,
                                 ACTOR_STRONGMAN_OPENING_THE_CHESTS, 20);
  Import_ActorSetAnimation(ACTOR_LOFT_ESCORT, ANIM_NOD);
  LogRoomsA_PlayAnimationAndWait(ACTOR_STRONGMAN_OPENING_THE_CHESTS, ANIM_NOD,
                                 20);
  Actor_WalkTo(ACTOR_STRONGMAN_OPENING_THE_CHESTS, 792, 392);
  Actor_WalkToAndWait(ACTOR_LOFT_ESCORT, 784, 400);
  Actor_FaceActor(ACTOR_LOFT_ESCORT, ACTOR_SECOND_THIEF, 0);
  Actor_WaitForMoveById(ACTOR_STRONGMAN_OPENING_THE_CHESTS);
  LogRoomsA_FaceActorAndWait(ACTOR_STRONGMAN_OPENING_THE_CHESTS,
                             ACTOR_FIRST_THIEF, 20);
  Actor_RunRepeatedMotion(ACTOR_LOFT_ESCORT, 1);
  Event_Wait(20);
  LogRoomsA_ShowMessageAndWait(ACTOR_LOFT_ESCORT, 20);
  LogRoomsA_PlayAnimationAndWait(ACTOR_STRONGMAN_OPENING_THE_CHESTS,
                                 ANIM_SHAKE_HEAD, 20);
  LogRoomsA_ShowMessageAndWait(ACTOR_STRONGMAN_OPENING_THE_CHESTS, 30);
  LogRoomsA_FaceEachOtherAndWait(ACTOR_LOFT_ESCORT, ACTOR_PARTY_LEADER, 20);
  LogRoomsA_ShowMessageAndWait(ACTOR_LOFT_ESCORT, 20);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_NOD);
  LogRoomsA_PlayAnimationAndWait(ACTOR_IVAN, ANIM_NOD, 50);
  // The window closes while the thieves are tied up.
  work = gFieldWork;
  work->transitionFrames = 30;
  work->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(60);
  Event_End();
}

/// Returns the stolen goods and rod, thanks the party, and clears the loft.
///
/// Called by Scene_Initialize after LogRoomsA_DefeatThieves. Sets
/// FLAG_VAULT_THIEVES_JAILED; the thieves are led away, the mayor's urn and
/// the Sanctum's gold statue come out of the chests, Ivan takes the Shaman's
/// Rod, gives back any Venus Djinn and leaves the party, and every actor
/// of the loft is cleared away.
static void LogRoomsA_ReturnStolenGoods(void) {
  struct Actor *temporaryActor;
  struct Actor *actor;
  s32 aftermath;
  s32 rod;
  struct FieldWork *work;

  work = gFieldWork;
  GameFlag_Set(FLAG_VAULT_THIEVES_JAILED);
  // With the screen still closed, the chests go back along the wall and the
  // thieves stand tied before the mayor.
  Event_Start();
  {
    struct Actor *actor = Actor_GetById(ACTOR_SECOND_THIEF);

    actor->priorityFlags = ACTOR_PRIORITY_AUTOMATIC | actor->priorityFlags;
  }
  Actor_MoveToAndWait(ACTOR_FIRST_CHEST, 872, 425);
  Actor_MoveToAndWait(ACTOR_SECOND_CHEST, 872, 409);
  Actor_MoveToAndWait(ACTOR_THIRD_CHEST, 872, 377);
  Actor_SetPosition(ACTOR_FIRST_THIEF, PIXELS(776), PIXELS(392));
  Actor_SetPosition(ACTOR_THIEF_LEADER, PIXELS(792), PIXELS(392));
  Actor_SetPosition(ACTOR_SECOND_THIEF, PIXELS(808), PIXELS(392));
  Import_ActorSetAnimation(ACTOR_THIEF_LEADER, 5);
  Import_ActorSetAnimation(ACTOR_FIRST_THIEF, 5);
  Import_ActorSetAnimation(ACTOR_SECOND_THIEF, 5);
  Actor_FaceActor(ACTOR_FIRST_THIEF, ACTOR_PARTY_LEADER, 0);
  Actor_FaceActor(ACTOR_THIEF_LEADER, ACTOR_PARTY_LEADER, 0);
  Actor_FaceActor(ACTOR_SECOND_THIEF, ACTOR_PARTY_LEADER, 0);
  actor = Actor_GetById(ACTOR_THIEF_LEADER);
  Actor_SetSpriteFlags(actor, SPRITE_FLAG_SHADOW);
  actor = Actor_GetById(ACTOR_FIRST_THIEF);
  Actor_SetSpriteFlags(actor, SPRITE_FLAG_SHADOW);
  actor = Actor_GetById(ACTOR_SECOND_THIEF);
  Actor_SetSpriteFlags(actor, SPRITE_FLAG_SHADOW);
  Actor_SetPosition(ACTOR_LOFT_ESCORT, PIXELS(768), PIXELS(408));
  Actor_SetPosition(ACTOR_STRONGMAN_OPENING_THE_CHESTS, PIXELS(768),
                    PIXELS(424));
  Actor_MoveToAndWait(ACTOR_LOFT_MAYOR, 784, 424);
  Actor_SetPosition(ACTOR_TIED_UP_VILLAGER, PIXELS(808), PIXELS(408));
  Actor_FaceActor(ACTOR_LOFT_ESCORT, ACTOR_LOFT_MAYOR, 0);
  Actor_FaceActor(ACTOR_TIED_UP_VILLAGER, ACTOR_LOFT_MAYOR, 0);
  Actor_FaceActor(ACTOR_STRONGMAN_OPENING_THE_CHESTS, ACTOR_THIEF_LEADER, 0);
  Actor_FaceActor(ACTOR_LOFT_MAYOR, ACTOR_THIEF_LEADER, 0);
  Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(792), PIXELS(440));
  Actor_SetPosition(ACTOR_GARET, PIXELS(808), PIXELS(440));
  Actor_SetPosition(ACTOR_IVAN, PIXELS(776), PIXELS(440));
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_THIEF_LEADER, 0);
  Actor_FaceActor(ACTOR_GARET, ACTOR_THIEF_LEADER, 0);
  Actor_FaceActor(ACTOR_IVAN, ACTOR_THIEF_LEADER, 0);
  gFieldWork->transitionFrames = 30;
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(40);
  // "Isaac, right? I won't forget this!" "Take them away!"
  Actor_RunRepeatedMotion(ACTOR_THIEF_LEADER, 2);
  aftermath = MsgVaultYouIsaacRightWontForget;
  Event_SetMessageId(aftermath);
  LogRoomsA_ShowMessageAndWait(ACTOR_THIEF_LEADER, 20);
  Actor_RunRepeatedMotion(ACTOR_LOFT_MAYOR, 1);
  Event_Wait(20);
  LogRoomsA_PlayAnimationAndWait(ACTOR_LOFT_MAYOR, ANIM_SHAKE_HEAD, 20);
  LogRoomsA_ShowMessageAndWait(ACTOR_LOFT_MAYOR, 20);
  Import_ActorSetAnimation(ACTOR_LOFT_ESCORT, ANIM_NOD);
  LogRoomsA_PlayAnimationAndWait(ACTOR_TIED_UP_VILLAGER, ANIM_NOD, 20);
  Actor_StartRepeatedMotion(ACTOR_FIRST_THIEF, 2);
  Actor_StartRepeatedMotion(ACTOR_SECOND_THIEF, 2);
  Event_Wait(60);
  // The escort leads the three thieves down the ladder, the tied-up villager
  // after them.
  Actor_SetSpeed(ACTOR_LOFT_ESCORT, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkTo(ACTOR_LOFT_ESCORT, 746, 408);
  Actor_FaceDirection(ACTOR_LOFT_MAYOR, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_STRONGMAN_OPENING_THE_CHESTS,
                      FACING_NORTHWEST + FACING_STEP, 0);
  Actor_SetScript(ACTOR_FIRST_THIEF, gLogRoomsA_VaultScriptB);
  Event_Wait(20);
  Actor_SetScript(ACTOR_THIEF_LEADER, gLogRoomsA_VaultScriptB);
  Event_Wait(15);
  Actor_SetScript(ACTOR_SECOND_THIEF, gLogRoomsA_VaultScriptB);
  Event_Wait(35);
  Actor_SetScript(ACTOR_TIED_UP_VILLAGER, gLogRoomsA_VaultScriptA);
  Event_Wait(20);
  Actor_WaitForMoveById(ACTOR_LOFT_ESCORT);
  Actor_SetPosition(ACTOR_LOFT_ESCORT, 0, 0);
  Event_Wait(40);
  // The mayor and the villager opening the chests thank the party.
  Actor_SetSpeed(ACTOR_LOFT_MAYOR, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(ACTOR_STRONGMAN_OPENING_THE_CHESTS, PIXELS(4) / 5,
                 PIXELS(2) / 5);
  Actor_WalkToAndWait(ACTOR_LOFT_MAYOR, 784, 408);
  Actor_FaceActor(ACTOR_LOFT_MAYOR, ACTOR_PARTY_LEADER, 0);
  Actor_WalkToAndWait(ACTOR_STRONGMAN_OPENING_THE_CHESTS, 768, 408);
  LogRoomsA_FaceActorAndWait(ACTOR_STRONGMAN_OPENING_THE_CHESTS,
                             ACTOR_PARTY_LEADER, 20);
  LogRoomsA_PlayAnimationAndWait(ACTOR_LOFT_MAYOR, ANIM_NOD, 20);
  LogRoomsA_ShowMessageAndWait(ACTOR_LOFT_MAYOR, 20);
  LogRoomsA_PlayAnimationAndWait(ACTOR_STRONGMAN_OPENING_THE_CHESTS, ANIM_NOD,
                                 20);
  Actor_FaceDirection(ACTOR_STRONGMAN_OPENING_THE_CHESTS, FACING_SOUTHEAST, 10);
  LogRoomsA_ShowMessageAndWait(ACTOR_STRONGMAN_OPENING_THE_CHESTS, 20);
  LogRoomsA_FaceEachOtherAndWait(ACTOR_PARTY_LEADER, ACTOR_GARET, 50);
  LogRoomsA_FaceEachOtherAndWait(ACTOR_PARTY_LEADER, ACTOR_IVAN, 50);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_LOFT_MAYOR, 0);
  Actor_FaceActor(ACTOR_GARET, ACTOR_LOFT_MAYOR, 0);
  LogRoomsA_FaceActorAndWait(ACTOR_IVAN, ACTOR_LOFT_MAYOR, 20);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_NOD);
  LogRoomsA_PlayAnimationAndWait(ACTOR_IVAN, ANIM_NOD, 50);
  // "Let's take a look at what they stole." The two go round to the chests.
  Actor_TurnToFaceEachOther(ACTOR_LOFT_MAYOR,
                            ACTOR_STRONGMAN_OPENING_THE_CHESTS, 0);
  LogRoomsA_PlayAnimationAndWait(ACTOR_LOFT_MAYOR, ANIM_NOD, 20);
  LogRoomsA_ShowMessageAndWait(ACTOR_LOFT_MAYOR, 20);
  Actor_SetScript(ACTOR_STRONGMAN_OPENING_THE_CHESTS, gLogRoomsA_VaultScriptC);
  Event_Wait(50);
  Actor_SetScript(ACTOR_LOFT_MAYOR, gLogRoomsA_VaultScriptD);
  Actor_SetSpeed(ACTOR_GARET, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(ACTOR_IVAN, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(ACTOR_GARET, 792, 456);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH + FACING_STEP, 0);
  Event_Wait(30);
  Actor_WalkToAndWait(ACTOR_IVAN, 792, 408);
  Actor_FaceDirection(ACTOR_IVAN, FACING_EAST, 0);
  Actor_WalkToAndWait(ACTOR_GARET, 808, 456);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH + FACING_STEP, 0);
  Event_Wait(100);
  LogRoomsA_FaceActorAndWait(ACTOR_STRONGMAN_OPENING_THE_CHESTS,
                             ACTOR_LOFT_MAYOR, 60);
  LogRoomsA_FaceActorAndWait(ACTOR_LOFT_MAYOR,
                             ACTOR_STRONGMAN_OPENING_THE_CHESTS, 40);
  LogRoomsA_PlayAnimationAndWait(ACTOR_LOFT_MAYOR, ANIM_NOD, 40);
  Actor_FaceDirection(ACTOR_LOFT_MAYOR, FACING_EAST, 0);
  Event_Wait(20);
  // The mayor opens the first chest and his urn rises out of it.
  Actor_RunRepeatedMotion(ACTOR_LOFT_MAYOR, 2);
  Event_PlaySound(SE_CHEST_OPEN);
  Import_ActorSetAnimation(ACTOR_FIRST_CHEST, ANIM_SHAKE_HEAD);
  Actor_SetPosition(ACTOR_FOUND_URN, PIXELS(872), PIXELS(424));
  Actor_SetSpritePriority(ACTOR_FOUND_URN, 1);
  Actor_SetSpeed(ACTOR_FOUND_URN, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetDestinationOffset(ACTOR_FOUND_URN, 0, -8);
  Actor_WaitForMoveById(ACTOR_FOUND_URN);
  Actor_RunRepeatedMotion(ACTOR_FOUND_URN, 2);
  Event_Wait(60);
  FieldMessage_Show((aftermath + 5), 1);
  Import_ActorSetAnimation(ACTOR_FIRST_CHEST, ANIM_WALK);
  Actor_SetPosition(ACTOR_FOUND_URN, 0, 0);
  gFieldWork->message++;
  Actor_RunRepeatedMotion(ACTOR_STRONGMAN_OPENING_THE_CHESTS, 1);
  LogRoomsA_ShowMessageAndWait(ACTOR_STRONGMAN_OPENING_THE_CHESTS, 20);
  LogRoomsA_FaceEachOtherAndWait(ACTOR_PARTY_LEADER, ACTOR_GARET, 40);
  LogRoomsA_FaceActorAndWait(ACTOR_LOFT_MAYOR,
                             ACTOR_STRONGMAN_OPENING_THE_CHESTS, 20);
  LogRoomsA_PlayAnimationAndWait(ACTOR_LOFT_MAYOR, ANIM_NOD, 20);
  LogRoomsA_ShowMessageAndWait(ACTOR_LOFT_MAYOR, 30);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_STRONGMAN_OPENING_THE_CHESTS, 0);
  LogRoomsA_FaceActorAndWait(ACTOR_GARET, ACTOR_STRONGMAN_OPENING_THE_CHESTS,
                             40);
  Actor_FaceDirection(ACTOR_STRONGMAN_OPENING_THE_CHESTS, FACING_EAST, 0);
  Event_Wait(40);
  // The villager opening the chests opens the second: the gold statue stolen
  // from the Sanctum.
  Actor_RunRepeatedMotion(ACTOR_STRONGMAN_OPENING_THE_CHESTS, 2);
  Event_PlaySound(SE_CHEST_OPEN);
  Import_ActorSetAnimation(ACTOR_SECOND_CHEST, ANIM_SHAKE_HEAD);
  Actor_GetById(ACTOR_FOUND_STATUE)->motionFlags = 0;
  Actor_SetSpritePriority(ACTOR_FOUND_STATUE, 1);
  Actor_SetPosition(ACTOR_FOUND_STATUE, PIXELS(872), PIXELS(408));
  Actor_SetSpeed(ACTOR_FOUND_STATUE, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetDestinationOffset(ACTOR_FOUND_STATUE, 0, -8);
  Actor_WaitForMoveById(ACTOR_FOUND_STATUE);
  Actor_RunRepeatedMotion(ACTOR_FOUND_STATUE, 2);
  Event_Wait(60);
  FieldMessage_Show((aftermath + 8), 1);
  Import_ActorSetAnimation(ACTOR_SECOND_CHEST, ANIM_WALK);
  Actor_SetPosition(ACTOR_FOUND_STATUE, 0, 0);
  gFieldWork->message++;
  Actor_ShowEmote(ACTOR_LOFT_MAYOR, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Event_Wait(60);
  LogRoomsA_ShowMessageAndWait(ACTOR_LOFT_MAYOR, 20);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Actor_StartRepeatedMotion(ACTOR_GARET, 2);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
  Event_Wait(20);
  LogRoomsA_PlayAnimationAndWait(ACTOR_STRONGMAN_OPENING_THE_CHESTS, ANIM_NOD,
                                 50);
  LogRoomsA_FaceActorAndWait(ACTOR_LOFT_MAYOR, ACTOR_PARTY_LEADER, 20);
  LogRoomsA_ShowMessageAndWait(ACTOR_LOFT_MAYOR, 30);
  // The villager opening the chests finds another chest; the mayor asks whether
  // it is what Ivan was looking for, and he can return to Master Hammet.
  Actor_FaceDirection(ACTOR_STRONGMAN_OPENING_THE_CHESTS,
                      FACING_NORTH + FACING_STEP, 0);
  Event_Wait(30);
  Actor_ShowEmote(ACTOR_STRONGMAN_OPENING_THE_CHESTS,
                  EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Event_Wait(60);
  Actor_WalkToAndWait(ACTOR_STRONGMAN_OPENING_THE_CHESTS, 856, 376);
  Event_Wait(20);
  LogRoomsA_FaceActorAndWait(ACTOR_STRONGMAN_OPENING_THE_CHESTS,
                             ACTOR_LOFT_MAYOR, 20);
  LogRoomsA_ShowMessageAndWait(ACTOR_STRONGMAN_OPENING_THE_CHESTS, 20);
  Actor_FaceActor(ACTOR_LOFT_MAYOR, ACTOR_STRONGMAN_OPENING_THE_CHESTS, 0);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Event_Wait(60);
  LogRoomsA_FaceActorAndWait(ACTOR_IVAN, ACTOR_STRONGMAN_OPENING_THE_CHESTS,
                             30);
  LogRoomsA_FaceActorAndWait(ACTOR_LOFT_MAYOR, ACTOR_IVAN, 20);
  LogRoomsA_PlayAnimationAndWait(ACTOR_LOFT_MAYOR, ANIM_NOD, 20);
  Event_ShowMessage(ACTOR_LOFT_MAYOR, 0);
  Actor_FaceDirection(ACTOR_STRONGMAN_OPENING_THE_CHESTS,
                      FACING_SOUTH + FACING_STEP, 0);
  Event_Wait(30);
  LogRoomsA_FaceActorAndWait(ACTOR_IVAN, ACTOR_LOFT_MAYOR, 20);
  LogRoomsA_PlayAnimationAndWait(ACTOR_IVAN, ANIM_NOD, 20);
  LogRoomsA_ShowMessageAndWait(ACTOR_LOFT_MAYOR, 20);
  Actor_TurnToFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
  LogRoomsA_FaceActorAndWait(ACTOR_GARET, ACTOR_IVAN, 20);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  LogRoomsA_PlayAnimationAndWait(ACTOR_GARET, ANIM_NOD, 40);
  LogRoomsA_PlayAnimationAndWait(ACTOR_IVAN, ANIM_NOD, 30);
  // Hammet was seen heading toward Lunpa: Dodonpa must have kidnapped him.
  Actor_RunRepeatedMotion(ACTOR_LOFT_MAYOR, 1);
  Event_Wait(20);
  LogRoomsA_PlayAnimationAndWait(ACTOR_LOFT_MAYOR, ANIM_SHAKE_HEAD, 20);
  LogRoomsA_ShowMessageAndWait(ACTOR_LOFT_MAYOR, 20);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_LOFT_MAYOR, 0);
  Actor_FaceActor(ACTOR_GARET, ACTOR_LOFT_MAYOR, 0);
  LogRoomsA_FaceActorAndWait(ACTOR_IVAN, ACTOR_LOFT_MAYOR, 20);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  Actor_StartRepeatedMotion(ACTOR_GARET, 1);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Event_Wait(60);
  LogRoomsA_ShowMessageAndWait(ACTOR_IVAN, 40);
  LogRoomsA_PlayAnimationAndWait(ACTOR_LOFT_MAYOR, ANIM_NOD, 20);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_SHAKE_HEAD);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 3);
  Event_Wait(30);
  LogRoomsA_PlayAnimationAndWait(ACTOR_LOFT_MAYOR, ANIM_SHAKE_HEAD, 20);
  LogRoomsA_ShowMessageAndWait(ACTOR_LOFT_MAYOR, 20);
  LogRoomsA_FaceEachOtherAndWait(ACTOR_PARTY_LEADER, ACTOR_GARET, 20);
  // Ivan rushes forward: what will happen to Hammet? The mayor calms him:
  // Dodonpa wants a ransom, so Hammet is safe for now.
  Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
  LogRoomsA_PlayAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD, 30);
  Actor_SetSpeed(ACTOR_IVAN, PIXELS(3) / 2, PIXELS(3) / 4);
  Actor_WalkToAndWait(ACTOR_IVAN, 800, 408);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
  Event_ShowMessage(ACTOR_IVAN, 0);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_LOFT_MAYOR, 0);
  LogRoomsA_FaceActorAndWait(ACTOR_GARET, ACTOR_LOFT_MAYOR, 30);
  LogRoomsA_PlayAnimationAndWait(ACTOR_LOFT_MAYOR, ANIM_SHAKE_HEAD, 20);
  LogRoomsA_ShowMessageAndWait(ACTOR_LOFT_MAYOR, 20);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_GARET,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_IVAN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(60);
  LogRoomsA_ShowMessageAndWait(ACTOR_IVAN, 20);
  Actor_RunRepeatedMotion(ACTOR_LOFT_MAYOR, 1);
  Event_Wait(20);
  LogRoomsA_ShowMessageAndWait(ACTOR_LOFT_MAYOR, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Event_Wait(60);
  LogRoomsA_PlayAnimationAndWait(ACTOR_LOFT_MAYOR, ANIM_SHAKE_HEAD, 20);
  LogRoomsA_ShowMessageAndWait(ACTOR_LOFT_MAYOR, 20);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  Actor_StartRepeatedMotion(ACTOR_GARET, 1);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
  Event_Wait(20);
  LogRoomsA_PlayAnimationAndWait(ACTOR_LOFT_MAYOR, ANIM_NOD, 20);
  LogRoomsA_ShowMessageAndWait(ACTOR_LOFT_MAYOR, 40);
  // The mayor asks the two their names, thanks them, and asks them to come
  // see him before they leave town.
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Event_Wait(60);
  Actor_WalkToAndWait(ACTOR_LOFT_MAYOR, 840, 424);
  LogRoomsA_FaceEachOtherAndWait(ACTOR_LOFT_MAYOR, ACTOR_PARTY_LEADER, 20);
  LogRoomsA_ShowMessageAndWait(ACTOR_LOFT_MAYOR, 20);
  LogRoomsA_PlayAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD, 20);
  Actor_FaceDirection(ACTOR_LOFT_MAYOR, FACING_SOUTH + FACING_STEP, 0);
  Event_Wait(20);
  LogRoomsA_ShowMessageAndWait(ACTOR_LOFT_MAYOR, 20);
  Actor_RunRepeatedMotion(ACTOR_GARET, 1);
  Event_Wait(20);
  LogRoomsA_ShowMessageAndWait(ACTOR_GARET, 20);
  LogRoomsA_PlayAnimationAndWait(ACTOR_LOFT_MAYOR, ANIM_NOD, 20);
  LogRoomsA_ShowMessageAndWait(ACTOR_LOFT_MAYOR, 30);
  LogRoomsA_FaceEachOtherAndWait(ACTOR_LOFT_MAYOR,
                                 ACTOR_STRONGMAN_OPENING_THE_CHESTS, 20);
  LogRoomsA_ShowMessageAndWait(ACTOR_LOFT_MAYOR, 20);
  // The mayor and the villager opening the chests leave down the ladder.
  Actor_WalkToAndWait(ACTOR_STRONGMAN_OPENING_THE_CHESTS, 856, 408);
  Actor_SetScript(ACTOR_LOFT_MAYOR, gLogRoomsA_VaultScriptE);
  Actor_SetScript(ACTOR_STRONGMAN_OPENING_THE_CHESTS, gLogRoomsA_VaultScriptE);
  Actor_WalkTo(ACTOR_GARET, 792, 456);
  Actor_SetSpeed(ACTOR_IVAN, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkTo(ACTOR_IVAN, 776, 432);
  Actor_WaitForMoveById(ACTOR_GARET);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH + FACING_STEP, 0);
  Actor_WaitForMoveById(ACTOR_IVAN);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH + FACING_STEP, 0);
  Actor_RefreshSelectorById(ACTOR_LOFT_MAYOR);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_LOFT_MAYOR, 0);
  temporaryActor = Actor_GetById(ACTOR_STRONGMAN_OPENING_THE_CHESTS);
  {
    u8 *pauseFlag = &temporaryActor->paused;
    u8 paused = 1;

    *pauseFlag = paused;
  }
  temporaryActor->targetX = ACTOR_NO_TARGET;
  temporaryActor->targetY = ACTOR_NO_TARGET;
  temporaryActor->targetZ = ACTOR_NO_TARGET;
  Actor_ShowEmote(ACTOR_LOFT_MAYOR, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Import_ActorSetAnimation(ACTOR_STRONGMAN_OPENING_THE_CHESTS, ANIM_STAND);
  Event_Wait(50);
  Actor_FaceActor(ACTOR_LOFT_MAYOR, ACTOR_PARTY_LEADER, 0);
  Actor_WalkToAndWait(ACTOR_GARET, 808, 440);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHWEST + FACING_STEP, 0);
  Event_Wait(20);
  LogRoomsA_ShowMessageAndWait(ACTOR_LOFT_MAYOR, 20);
  LogRoomsA_FaceEachOtherAndWait(ACTOR_PARTY_LEADER, ACTOR_GARET, 50);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_LOFT_MAYOR, 0);
  LogRoomsA_FaceActorAndWait(ACTOR_GARET, ACTOR_LOFT_MAYOR, 20);
  LogRoomsA_PlayAnimationAndWait(ACTOR_LOFT_MAYOR, ANIM_NOD, 20);
  Actor_WalkTo(ACTOR_LOFT_MAYOR, 744, 408);
  Actor_WalkTo(ACTOR_STRONGMAN_OPENING_THE_CHESTS, 744, 408);
  Actor_WaitForMoveById(ACTOR_LOFT_MAYOR);
  Actor_WaitForMoveById(ACTOR_STRONGMAN_OPENING_THE_CHESTS);
  Event_Wait(30);
  // Alone, Garet tries to cheer Ivan up; Ivan only wants to rescue Hammet,
  // but has seen the two's mission in their minds and won't ask their help.
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 792, 408);
  LogRoomsA_FaceEachOtherAndWait(ACTOR_PARTY_LEADER, ACTOR_GARET, 30);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Event_Wait(50);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
  LogRoomsA_FaceActorAndWait(ACTOR_GARET, ACTOR_IVAN, 20);
  Actor_RunRepeatedMotion(ACTOR_GARET, 1);
  Event_Wait(20);
  LogRoomsA_ShowMessageAndWait(ACTOR_GARET, 40);
  LogRoomsA_PlayAnimationAndWait(ACTOR_IVAN, ANIM_NOD, 30);
  LogRoomsA_FaceEachOtherAndWait(ACTOR_PARTY_LEADER, ACTOR_GARET, 40);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_SHAKE_HEAD);
  LogRoomsA_PlayAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD, 30);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
  LogRoomsA_FaceActorAndWait(ACTOR_GARET, ACTOR_IVAN, 20);
  Actor_RunRepeatedMotion(ACTOR_GARET, 1);
  Event_Wait(20);
  LogRoomsA_ShowMessageAndWait(ACTOR_GARET, 40);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
  Event_Wait(20);
  LogRoomsA_ShowMessageAndWait(ACTOR_IVAN, 20);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  LogRoomsA_PlayAnimationAndWait(ACTOR_GARET, ANIM_NOD, 20);
  LogRoomsA_PlayAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD, 20);
  LogRoomsA_ShowMessageAndWait(ACTOR_IVAN, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Event_Wait(60);
  LogRoomsA_PlayAnimationAndWait(ACTOR_IVAN, ANIM_NOD, 20);
  LogRoomsA_ShowMessageAndWait(ACTOR_IVAN, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Event_Wait(60);
  LogRoomsA_PlayAnimationAndWait(ACTOR_IVAN, ANIM_NOD, 20);
  LogRoomsA_ShowMessageAndWait(ACTOR_IVAN, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Event_Wait(60);
  LogRoomsA_PlayAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD, 20);
  LogRoomsA_ShowMessageAndWait(ACTOR_IVAN, 20);
  LogRoomsA_PlayAnimationAndWait(ACTOR_GARET, ANIM_NOD, 20);
  LogRoomsA_ShowMessageAndWait(ACTOR_GARET, 30);
  // "Then, I suppose this is good-bye." A no has Garet say Isaac would help,
  // and Ivan decline.
  Event_OpenMessage(ACTOR_IVAN, 0);
  if (Event_AskYesNoWithActorAnimation(0, 0) != 0) {
    Event_Wait(20);
    Actor_RunRepeatedMotion(ACTOR_GARET, 1);
    Event_Wait(20);
    LogRoomsA_ShowMessageAndWait(ACTOR_GARET, 20);
    LogRoomsA_PlayAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD, 20);
    LogRoomsA_ShowMessageAndWait(ACTOR_IVAN, 20);
  }
  // Ivan walks round to the third chest and takes the Shaman's Rod from it.
  LogRoomsA_PlayAnimationAndWait(ACTOR_IVAN, ANIM_NOD, 30);
  Actor_WalkToAndWait(ACTOR_IVAN, 800, 456);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
  Actor_FaceActor(ACTOR_GARET, ACTOR_IVAN, 0);
  Actor_WalkToAndWait(ACTOR_IVAN, 856, 456);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
  Actor_FaceActor(ACTOR_GARET, ACTOR_IVAN, 0);
  Actor_WalkToAndWait(ACTOR_IVAN, 856, 376);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
  Actor_FaceActor(ACTOR_GARET, ACTOR_IVAN, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_EAST, 0);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
  temporaryActor = (struct Actor *)FieldEffect_PlayCueAndStartEmitterOnTarget(
      ACTOR_IVAN, ACTOR_THIRD_CHEST, ITEM_SHAMANS_ROD);
  Event_Wait(60);
  rod = MsgVaultIvanGotShamansRod;
  FieldMessage_Show(rod, 1);
  Actor_Release(temporaryActor);
  Import_ActorSetAnimation(ACTOR_THIRD_CHEST, ANIM_WALK);
  Event_Wait(20);
  LogRoomsA_PlayAnimationAndWait(ACTOR_IVAN, ANIM_NOD, 20);
  Actor_WalkToAndWait(ACTOR_IVAN, 856, 456);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
  Actor_FaceActor(ACTOR_GARET, ACTOR_IVAN, 0);
  Actor_WalkToAndWait(ACTOR_IVAN, 800, 456);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
  Actor_FaceActor(ACTOR_GARET, ACTOR_IVAN, 0);
  Actor_WalkToAndWait(ACTOR_IVAN, 776, 424);
  Actor_TurnToFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
  LogRoomsA_FaceActorAndWait(ACTOR_GARET, ACTOR_IVAN, 30);
  // He thanks the town, hands back any Venus Djinni and leaves the party.
  LogRoomsA_PlayAnimationAndWait(ACTOR_IVAN, ANIM_NOD, 20);
  Event_SetMessageId((rod + 1));
  LogRoomsA_ShowMessageAndWait(ACTOR_IVAN, 20);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  {
    u16 *slot = &work->message;
    s32 saved = (s16)*slot;

    if (LogRoomsA_GetIvanVenusDjinnCount() != 0) {
      Event_SetMessageId(MsgVaultWaitDontWantTakeYour);
      Event_ShowMessage(ACTOR_IVAN, 0);
      Vault_ReturnIsaacsDjinni();
    }
    Party_RemoveMemberRestored(IVAN);
    *slot = saved;
  }
  LogRoomsA_PlayAnimationAndWait(ACTOR_GARET, ANIM_NOD, 50);
  Actor_WalkToAndWait(ACTOR_IVAN, 776, 408);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
  Actor_FaceActor(ACTOR_GARET, ACTOR_IVAN, 0);
  // Ivan goes down the ladder; Garet: they'd better get moving too.
  Actor_WalkToAndWait(ACTOR_IVAN, 744, 408);
  Event_Wait(40);
  LogRoomsA_FaceEachOtherAndWait(ACTOR_PARTY_LEADER, ACTOR_GARET, 20);
  Actor_RunRepeatedMotion(ACTOR_GARET, 1);
  Event_Wait(20);
  LogRoomsA_ShowMessageAndWait(ACTOR_GARET, 20);
  LogRoomsA_PlayAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD, 20);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_WALK);
  actor = Actor_GetById(ACTOR_PARTY_LEADER);
  if (actor != NULL) {
    Actor_SetDestination(ACTOR_GARET, actor->x.part.pixel, actor->z.part.pixel);
  }
  Actor_WaitForMoveById(ACTOR_GARET);
  Actor_SetPosition(ACTOR_GARET, 0, 0);
  Event_Wait(30);
  // Clear the loft of everyone the event used.
  Actor_SetPosition(ACTOR_TIED_UP_VILLAGER, 0, 0);
  Actor_SetPosition(ACTOR_LOFT_MAYOR, 0, 0);
  Actor_SetPosition(ACTOR_LOFT_ESCORT, 0, 0);
  Actor_SetPosition(ACTOR_STRONGMAN_OPENING_THE_CHESTS, 0, 0);
  Actor_SetPosition(ACTOR_THIEF_LEADER, 0, 0);
  Actor_SetPosition(ACTOR_FIRST_THIEF, 0, 0);
  Actor_SetPosition(ACTOR_SECOND_THIEF, 0, 0);
  Actor_SetPosition(ACTOR_IVAN, 0, 0);
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  Event_End();
}

/// Opens the window transition and waits one frame after it finishes.
static void LogRoomsA_OpenWindowTransition(void) {
  struct FieldWork *state;

  state = gFieldWork;
  state->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(1);
}

static void LogRoomsA_ShowMessageAndWait(s32 actorId, s32 waitFrames) {
  Event_ShowMessageAndWait(actorId, 0);
  Event_Wait(waitFrames);
}

static void LogRoomsA_FaceActorAndWait(s32 actorId, s32 targetId,
                                       s32 waitFrames) {
  Actor_SetAngleToward(actorId, targetId, 0);
  Event_Wait(waitFrames);
}

static void LogRoomsA_FaceEachOtherAndWait(s32 firstId, s32 secondId,
                                           s32 waitFrames) {
  Actor_TurnToFaceEachOther(firstId, secondId, 0);
  Event_Wait(waitFrames);
}

/// Waits for the animation to change before the following pause.
static void LogRoomsA_PlayAnimationAndWait(s32 actorId, s32 animation,
                                           s32 waitFrames) {
  Import_ActorSetAnimation(actorId, animation, waitFrames);
  Actor_WaitForAnimationChange(actorId);
  Event_Wait(waitFrames);
}

/// Begins Mind Read, chooses its target, and shows its opening effect.
static void LogRoomsA_StartMindRead(s32 casterId, s32 targetId) {
  FieldPsynergy_LoadResources(ABILITY_MIND_READ, 1);
  FieldPsynergy_SetupActorPair(casterId, targetId);
  FieldPsynergy_StartCast();
  FieldPsynergy_RunHandler(1);
  wait(1);
}

/// Finishes Mind Read and releases its event effect actors.
static void LogRoomsA_EndMindRead(void) {
  FieldPsynergy_RunHandler(2);
  FieldPsynergy_StopCasterUpdate();
  FieldPsynergy_End();
}

#include "shared/vault_item_icon.h"

/// Finds the route node containing the actor's position on the
/// eight-pixel grid.
static struct LogRoomsAStop *LogRoomsA_FindStopAtPosition(s32 *position) {
  s32 gridX = (position[0] - PIXELS(64)) >> 19;
  s32 gridZ = (position[2] - PIXELS(624)) >> 19;
  struct LogRoomsAStop *stop = gLogRoomsA_Stops;
  struct LogRoomsAStop *result = NULL;
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
static struct LogRoomsAStop *
LogRoomsA_SnapToNearestStop(struct LogRoomsAStop *node, s16 *requestedFacing) {
  struct LogRoomsAStopConnection *connection;
  s32 stopIndex;
  s32 nearestDifference;
  s32 chosenFacing;
  s32 angleDifference;
  u32 connectionIndex;
  u16 connectionFacing;

  stopIndex = -1;
  chosenFacing = *requestedFacing;
  connection = node->connections;
  nearestDifference = TRIG_TURN / 2;
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
  return &gLogRoomsA_Stops[stopIndex];
}

/// Checks whether an actor's destination or current position lies within
/// eight pixels.
static s32 LogRoomsA_IsActorWithinEightPixels(s32 x, s32 z, s32 id) {
  struct Actor *actor = Actor_Lookup(id);
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
static s32 LogRoomsA_IsActorWithinSixteenPixels(s32 x, s32 z, s32 id) {
  struct Actor *actor = Actor_Lookup(id);
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
  if (ax * ax + az * az <= 16 * 16)
    return 1;
  return 0;
}

/// Reserves a full cell around the leader and half a cell around each
/// actor following a route. Testing destinations as well as current positions
/// prevents two actors choosing the same stop while still walking.
static s32 LogRoomsA_CheckStopOccupancy(struct LogRoomsAStop *stop) {
  s32 x;
  s32 z;

  if (stop == NULL) {
    return 1;
  }
  x = (stop->gridX << 19) + PIXELS(72);
  z = (stop->gridZ << 19) + PIXELS(632);
  if (LogRoomsA_IsActorWithinSixteenPixels(x, z, ACTOR_PARTY_LEADER) != 0 ||
      LogRoomsA_IsActorWithinEightPixels(x, z, ACTOR_IVAN) != 0 ||
      LogRoomsA_IsActorWithinEightPixels(x, z, ACTOR_SUSPECTED_LEADER) != 0 ||
      LogRoomsA_IsActorWithinEightPixels(x, z, ACTOR_SUSPECTED_THIEF) != 0) {
    return -1;
  }
  return 0;
}

/// Sets the actor's destination to the route stop's cell center.
static void LogRoomsA_MoveActorToStop(struct Actor *actor,
                                      struct LogRoomsAStop *stop) {
  FieldActor_SetMoveTarget(actor, (stop->gridX << 19) + PIXELS(72), 0,
                           (stop->gridZ << 19) + PIXELS(632));
}

/// Steers Ivan and both suspects through the graph and detects a sustained
/// cornering.
///
/// Run each frame as a task from LogRoomsA_StartActorStops
/// until LogRoomsA_ReadCorneredSuspectsMinds removes it. Raises event 200
/// plus the blocked suspects (1 the leader, 2 the thief) after thirty frames
/// with at least one suspect blocked; a frame when both can move restarts the
/// wait.
static void LogRoomsA_UpdateActorStops(void) {
  struct Actor *leader;
  struct Actor *actor;
  struct FieldWork *work;
  struct LogRoomsAStop *entry;
  struct LogRoomsAStop *destination;
  s32 dx;
  s32 dz;
  s32 blocked;
  u16 angle;
  s16 facing;
  s16 facingDifference;

  leader = Actor_Lookup(ACTOR_PARTY_LEADER);
  blocked = 0;
  work = gFieldWork;
  actor = Actor_Lookup(ACTOR_IVAN);
  entry = LogRoomsA_FindStopAtPosition(&actor->x.fixed);
  if (entry != NULL && actor->targetX == ACTOR_NO_TARGET) {
    dx = actor->x.fixed - leader->x.fixed;
    dz = actor->z.fixed - leader->z.fixed;
    facing = leader->facing;
    angle = ArcTan2(dz, dx);
    dx >>= 16;
    dz >>= 16;
    if (work->blockedSteps > 0 && dx * dx + dz * dz <= 400 &&
        (facingDifference = (u16)facing - angle) > -FACING_STEP &&
        facingDifference < FACING_STEP) {
      // DEAD: facingDifference is not read again; the arm it fills is the
      // ROM's b past the else, after the bge to it.
      facingDifference = 0;
#if TBS_PATCH_1
    } else if (dx * dx + dz * dz > 64) {
#else
    } else {
#endif
      facing = actor->facing;
    }
    destination = LogRoomsA_SnapToNearestStop(entry, &facing);
    if (LogRoomsA_CheckStopOccupancy(destination) == 0) {
      LogRoomsA_MoveActorToStop(actor, destination);
      Actor_SelectAnimation(actor, 2);
    } else {
      Actor_SelectAnimation(actor, 1);
    }
  }

  // Each suspect tries a random turn, then reverses when that route is
  // occupied.
  actor = Actor_Lookup(ACTOR_SUSPECTED_LEADER);
  entry = LogRoomsA_FindStopAtPosition(&actor->x.fixed);
  if (entry != NULL && actor->targetX == ACTOR_NO_TARGET) {
    u32 turn = (u32)Import_RandomNext() * 2 >> 16;
    s32 currentFacing;

    turn *= 0x60000000;
    currentFacing = actor->facing;
    facing = ((turn - 0x30000000) >> 16) + currentFacing;
    destination = LogRoomsA_SnapToNearestStop(entry, &facing);
    if (LogRoomsA_CheckStopOccupancy(destination) != 0) {
      facing = actor->facing + TRIG_TURN / 2;
      destination = LogRoomsA_SnapToNearestStop(entry, &facing);
      if (LogRoomsA_CheckStopOccupancy(destination) == 0) {
        FieldEffect_SetAttached(ACTOR_SUSPECTED_LEADER, ATTACHED_EFFECT_ONCE);
        goto move_first;
      }
      Actor_SelectAnimation(actor, 4);
      blocked = 1;
    } else {
    move_first:
      LogRoomsA_MoveActorToStop(actor, destination);
      Actor_SelectAnimation(actor, 2);
    }
  }

  actor = Actor_Lookup(ACTOR_SUSPECTED_THIEF);
  entry = LogRoomsA_FindStopAtPosition(&actor->x.fixed);
  if (entry != NULL && actor->targetX == ACTOR_NO_TARGET) {
    u32 turn = (u32)Import_RandomNext() * 3 >> 16;
    s32 currentFacing;

    turn *= 0x30000000;
    currentFacing = actor->facing;
    facing = ((turn - 0x30000000) >> 16) + currentFacing;
    destination = LogRoomsA_SnapToNearestStop(entry, &facing);
    if (LogRoomsA_CheckStopOccupancy(destination) != 0) {
      facing = actor->facing + TRIG_TURN / 2;
      destination = LogRoomsA_SnapToNearestStop(entry, &facing);
      if (LogRoomsA_CheckStopOccupancy(destination) == 0) {
        FieldEffect_SetAttached(ACTOR_SUSPECTED_THIEF, ATTACHED_EFFECT_ONCE);
        goto move_second;
      }
      Actor_SelectAnimation(actor, 4);
      blocked += 2;
    } else {
    move_second:
      LogRoomsA_MoveActorToStop(actor, destination);
      Actor_SelectAnimation(actor, 2);
    }
  }
  // A failed route choice alone is not enough to end the chase.
  if (blocked != 0) {
    if (++gLogRoomsA_StopTimer > LOG_ROOMS_A_CORNERED_FRAMES - 1)
      work->raisedTrigger = blocked + LOG_ROOMS_A_CORNERED_EVENT_BASE;
  } else {
    gLogRoomsA_StopTimer = blocked;
  }
}

/// Arranges the chase, constrains its camera, and starts the route task.
///
/// Called by LogRoomsA_StartCorneringSuspects. Starts
/// LogRoomsA_UpdateActorStops and the chase music, MUS_DJINN_LESSON.
static void LogRoomsA_StartActorStops(void) {
  Event_Start();
  FieldCamera_MoveTo(PIXELS(168), -1, PIXELS(656), 1);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(ACTOR_GARET, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(ACTOR_IVAN, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 248, 696);
  Actor_SetPosition(ACTOR_GARET, PIXELS(248), PIXELS(696));
  Actor_SetPosition(ACTOR_IVAN, PIXELS(248), PIXELS(696));
  Actor_WalkTo(ACTOR_PARTY_LEADER, 200, 696);
  Actor_WalkTo(ACTOR_GARET, 248, 712);
  Actor_WalkToAndWait(ACTOR_IVAN, 232, 696);
  Actor_WaitForMoveById(ACTOR_GARET);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_WEST, 0);
  Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
  Import_ActorSetAnimation(ACTOR_GARET, 12);
  LogRoomsA_ResetSuspectRoutes();
  Field_SetCameraBounds(PIXELS(48), PIXELS(576), PIXELS(288), PIXELS(736));
  Actor_SetSpeed(ACTOR_GARET, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(ACTOR_IVAN, PIXELS(3) / 4, PIXELS(3) / 8);
  Actor_SetSpeed(ACTOR_SUSPECTED_LEADER, PIXELS(1), PIXELS(6) / 5);
  Actor_SetSpeed(ACTOR_SUSPECTED_THIEF, PIXELS(3) / 2, PIXELS(3) / 2);
  gLogRoomsA_StopTimer = 0;
  Task_AddOrUpdateFrame(LogRoomsA_UpdateActorStops, TASK_PASS_FRAME + 20);
  // Flag 0x1ff holds nothing: no table, script or function sets or reads it,
  // and render probe flag-reads sees no read of it, so clearing it changes
  // nothing.
  Event_ClearFlag(0x1ff);
  Event_End();
  Sound_Play(MUS_DJINN_LESSON);
}

// clang-format off
// A missing edge keeps both sentinel bytes; valid facings use the angle's high
// byte.
#define NO_CONNECTION {255, 0xff, {0, 0}}
#define CONNECTION(stop, direction) {stop, (direction) >> 8, {0, 0}}
struct LogRoomsAStop gLogRoomsA_Stops[] = {
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

s32 gLogRoomsA_ActionTable[] = {
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

s32 gLogRoomsA_FirstThiefEntryScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(776), 0, PIXELS(408)),
    SC_SYNC,
    SC_MOVE(PIXELS(776), 0, PIXELS(400)),
    SC_SYNC,
    SC_MOVE(PIXELS(800), 0, PIXELS(400)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_SET(SW_FACING, TRIG_TURN * 3 / 16),
    SC_WAIT(20),
    SC_END,
};

s32 gLogRoomsA_SecondThiefEntryScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(776), 0, PIXELS(408)),
    SC_SYNC,
    SC_MOVE(PIXELS(776), 0, PIXELS(416)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_SET(SW_FACING, TRIG_TURN * 3 / 16),
    SC_WAIT(20),
    SC_END,
};

s32 gLogRoomsA_VaultScriptA[] = {
    SC_SET(SW_SPEED, PIXELS(4) / 5),
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(776), 0, PIXELS(408)),
    SC_SYNC,
    SC_MOVE(PIXELS(744), 0, PIXELS(408)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_POSITION(0, 0, 0),
    SC_END,
};

s32 gLogRoomsA_VaultScriptB[] = {
    SC_SET(SW_SPEED, PIXELS(4) / 5),
    SC_ANIMATION(6),
    SC_MOVE(PIXELS(776), 0, PIXELS(408)),
    SC_SYNC,
    SC_MOVE(PIXELS(744), 0, PIXELS(408)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_POSITION(0, 0, 0),
    SC_END,
};

s32 gLogRoomsA_VaultScriptC[] = {
    SC_SET(SW_SPEED, PIXELS(4) / 5),
    SC_ANIMATION(2),
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
    SC_ANIMATION(1),
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(10),
    SC_END,
};

s32 gLogRoomsA_VaultScriptD[] = {
    SC_SET(SW_SPEED, PIXELS(4) / 5),
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(808), 0, PIXELS(408)),
    SC_SYNC,
    SC_MOVE(PIXELS(808), 0, PIXELS(456)),
    SC_SYNC,
    SC_MOVE(PIXELS(840), 0, PIXELS(456)),
    SC_SYNC,
    SC_MOVE(PIXELS(856), 0, PIXELS(424)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(10),
    SC_END,
};

s32 gLogRoomsA_VaultScriptE[] = {
    SC_SET(SW_SPEED, PIXELS(4) / 5),
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(840), 0, PIXELS(456)),
    SC_SYNC,
    SC_MOVE(PIXELS(808), 0, PIXELS(456)),
    SC_SYNC,
    SC_MOVE(PIXELS(808), 0, PIXELS(408)),
    SC_SYNC,
    SC_MOVE(PIXELS(776), 0, PIXELS(408)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_END,
};

// The suspects' runs about the guests' room, named by the spot each ends at
// among the room's three beds: the head of each bed (z 632), its foot
// (z 696), beside the outer beds (x 72 and 264, z 648) and the room's two
// south corners (render map-12 with the spots drawn on it). A Round run goes
// about the end of the outer bed, an Across run over the beds' row, an Along
// run past the feet of the beds. The suspects' timers number the spots
// (LogRoomsA_SuspectsRunFromParty): the leader's 1 to 3 the heads and 4 to 6 the
// feet, west to east; the thief's 1 and 2 beside the west and east beds, 3
// and 4 the south-west and south-east corners.
s32 gLogRoomsA_SuspectToWestBedHead[] = {
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(104), 0, PIXELS(632)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_END,
};

s32 gLogRoomsA_SuspectRoundToWestBedHead[] = {
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(72), 0, PIXELS(696)),
    SC_SYNC,
    SC_MOVE(PIXELS(80), 0, PIXELS(640)),
    SC_SYNC,
    SC_MOVE(PIXELS(104), 0, PIXELS(632)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_END,
};

s32 gLogRoomsA_SuspectToMiddleBedHead[] = {
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(168), 0, PIXELS(632)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_END,
};

s32 gLogRoomsA_SuspectToEastBedHead[] = {
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(232), 0, PIXELS(632)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_END,
};

s32 gLogRoomsA_SuspectRoundToEastBedHead[] = {
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(264), 0, PIXELS(696)),
    SC_SYNC,
    SC_MOVE(PIXELS(264), 0, PIXELS(632)),
    SC_SYNC,
    SC_MOVE(PIXELS(232), 0, PIXELS(632)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_END,
};

s32 gLogRoomsA_SuspectToWestBedFoot[] = {
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(104), 0, PIXELS(696)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_END,
};

s32 gLogRoomsA_SuspectRoundToWestBedFoot[] = {
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(80), 0, PIXELS(640)),
    SC_SYNC,
    SC_MOVE(PIXELS(72), 0, PIXELS(696)),
    SC_SYNC,
    SC_MOVE(PIXELS(104), 0, PIXELS(696)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_END,
};

s32 gLogRoomsA_SuspectToMiddleBedFoot[] = {
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(168), 0, PIXELS(696)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_END,
};

s32 gLogRoomsA_SuspectToEastBedFoot[] = {
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(232), 0, PIXELS(696)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_END,
};

s32 gLogRoomsA_SuspectRoundToEastBedFoot[] = {
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(264), 0, PIXELS(632)),
    SC_SYNC,
    SC_MOVE(PIXELS(264), 0, PIXELS(696)),
    SC_SYNC,
    SC_MOVE(PIXELS(232), 0, PIXELS(696)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_END,
};

s32 gLogRoomsA_SuspectAcrossToWestOfBeds[] = {
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(168), 0, PIXELS(648)),
    SC_SYNC,
    SC_MOVE(PIXELS(72), 0, PIXELS(648)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_END,
};

s32 gLogRoomsA_SuspectToWestOfBeds[] = {
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(72), 0, PIXELS(648)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_END,
};

s32 gLogRoomsA_SuspectAcrossToEastOfBeds[] = {
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(168), 0, PIXELS(648)),
    SC_SYNC,
    SC_MOVE(PIXELS(264), 0, PIXELS(648)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_END,
};

s32 gLogRoomsA_SuspectToEastOfBeds[] = {
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(264), 0, PIXELS(648)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_END,
};

s32 gLogRoomsA_SuspectToSouthWestCorner[] = {
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(72), 0, PIXELS(696)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_END,
};

s32 gLogRoomsA_SuspectAlongToSouthWestCorner[] = {
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(120), 0, PIXELS(696)),
    SC_SYNC,
    SC_MOVE(PIXELS(72), 0, PIXELS(696)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_END,
};

s32 gLogRoomsA_SuspectToSouthEastCorner[] = {
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(264), 0, PIXELS(696)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_END,
};

s32 gLogRoomsA_SuspectAlongToSouthEastCorner[] = {
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(168), 0, PIXELS(696)),
    SC_SYNC,
    SC_MOVE(PIXELS(264), 0, PIXELS(696)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_END,
};

struct SceneEntrance gLogRoomsA_Entrances[] = {
    { ENTRANCE_DEFAULT, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { LOG_ROOMS_A_FROM_VAULT_EAST_SOUTHEAST, CONDITION_ALWAYS, 192, 0, 220, FACING_NORTH, 0, 32, 0, 272, 240, 0 },
    { LOG_ROOMS_A_FROM_VAULT_SOUTHEAST, CONDITION_ALWAYS, 432, 0, 254, FACING_NORTH, 0, 288, 0, 528, 272, 0 },
    { LOG_ROOMS_A_FROM_VAULT_SOUTHWEST, CONDITION_ALWAYS, 720, 0, 782, FACING_NORTH, 0, 432, 576, 800, 800, 0 },
    { LOG_ROOMS_A_FROM_VAULT_EAST, CONDITION_ALWAYS, 624, 0, 236, FACING_NORTH, 0, 560, 32, 848, 256, 0 },
    { LOG_ROOMS_A_FROM_VAULT_NORTH_NORTHEAST, CONDITION_ALWAYS, 144, 0, 492, FACING_NORTH, 0, 16, 288, 256, 512, 0 },
    { LOG_ROOMS_A_FROM_VAULT_NORTHWEST, CONDITION_ALWAYS, 416, 0, 526, FACING_NORTH, 0, 320, 288, 592, 544, 0 },
    { LOG_ROOMS_A_INN_FROM_INN_UPSTAIRS, CONDITION_ALWAYS, 744, 0, 640, FACING_SOUTH, 0, 432, 576, 800, 800, 0 },
    { LOG_ROOMS_A_INN_UPSTAIRS_FROM_INN, CONDITION_ALWAYS, 358, 0, 632, FACING_SOUTH, 0, 32, 576, 400, 776, 0 },
    { LOG_ROOMS_A_FROM_VAULT_WEST, CONDITION_ALWAYS, 648, 0, 424, FACING_SOUTH, 0, 560, 320, 800, 480, 0 },
    { LOG_ROOMS_A_CRATE_ROOM_FROM_LOFT, CONDITION_ALWAYS, 704, 0, 408, FACING_WEST, 0, 560, 320, 800, 480, 0 },
    { LOG_ROOMS_A_LOFT_FROM_CRATE_ROOM, CONDITION_ALWAYS, 768, 0, 408, FACING_EAST, 0, 712, 288, 952, 496, 0 },
    { LOG_ROOMS_A_LOFT_AFTER_LOSING_TO_THIEVES, CONDITION_ALWAYS, 792, 0, 432, FACING_NORTH, 0, 712, 288, 952, 496, 0 },
    { LOG_ROOMS_A_LOFT_AFTER_THIEVES_DEFEATED, CONDITION_ALWAYS, 792, 0, 432, FACING_NORTH, 0, 712, 288, 952, 496, 0 },
    { SCENE_TABLE_END },
};

struct SceneExitRegion gLogRoomsA_Regions[] = {
    {639, 33, 407, 657, 51, 425, CONDITION_ALWAYS, LOG_ROOMS_A_TO_VAULT_WEST},
    {SCENE_TABLE_END},
};

u32 gLogRoomsA_Exits[] = {
    SCENE_EXITS(LogRoomsA),
    SCENE_EXIT(LOG_ROOMS_A_TO_VAULT_EAST_SOUTHEAST, Vault, VAULT_FROM_LOG_ROOMS_A_EAST_SOUTHEAST),
    SCENE_EXIT(LOG_ROOMS_A_TO_VAULT_SOUTHEAST, Vault, VAULT_FROM_LOG_ROOMS_A_SOUTHEAST),
    SCENE_EXIT(LOG_ROOMS_A_TO_VAULT_SOUTHWEST, Vault, VAULT_FROM_LOG_ROOMS_A_SOUTHWEST),
    SCENE_EXIT(LOG_ROOMS_A_TO_VAULT_EAST, Vault, VAULT_FROM_LOG_ROOMS_A_EAST),
    SCENE_EXIT(LOG_ROOMS_A_TO_VAULT_NORTH_NORTHEAST, Vault, VAULT_FROM_LOG_ROOMS_A_NORTH_NORTHEAST),
    SCENE_EXIT(LOG_ROOMS_A_TO_VAULT_NORTHWEST, Vault, VAULT_FROM_LOG_ROOMS_A_NORTHWEST),
    SCENE_EXIT(LOG_ROOMS_A_INN_UPSTAIRS_TO_INN, LogRoomsA, LOG_ROOMS_A_INN_FROM_INN_UPSTAIRS),
    SCENE_EXIT(LOG_ROOMS_A_INN_TO_INN_UPSTAIRS, LogRoomsA, LOG_ROOMS_A_INN_UPSTAIRS_FROM_INN),
    SCENE_EXIT(LOG_ROOMS_A_TO_VAULT_WEST, Vault, VAULT_FROM_LOG_ROOMS_A_WEST),
    SCENE_EXIT(LOG_ROOMS_A_LOFT_TO_CRATE_ROOM, LogRoomsA, LOG_ROOMS_A_CRATE_ROOM_FROM_LOFT),
    SCENE_EXIT(LOG_ROOMS_A_CRATE_ROOM_TO_LOFT, LogRoomsA, LOG_ROOMS_A_LOFT_FROM_CRATE_ROOM),
    SCENE_EXIT(LOG_ROOMS_A_TO_SANCTUM, Sanctum, SANCTUM_FROM_LOG_ROOMS_A),
    SCENE_EXITS_END,
};

struct ActorPlacement gLogRoomsA_ActorPlacements[] = {
    { ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(8), 0, PIXELS(8), FACING_NORTH, TALK_FACING_PARTY, 0 },
    { ACTOR_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(376), 0, PIXELS(392), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        PIXELS(160), 0, PIXELS(136), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_IN_VIEW, 3,
        PIXELS(152), 0, PIXELS(72), FACING_WEST, TALK_FACING_PARTY, 0 },
    { SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        PIXELS(456), 0, PIXELS(184), FACING_EAST, TALK_FACING_PARTY, 0 },
    { SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_WANDER,
        PIXELS(368), 0, PIXELS(120), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_OLD_MAN_WITH_CANE, CONDITION_IN_VIEW, MOVEMENT_TYPE_WANDER,
        PIXELS(744), 0, PIXELS(200), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_OLD_WOMAN, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        PIXELS(752), 0, PIXELS(128), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        PIXELS(496), 0, PIXELS(416), FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_GIRL_WITH_PIGTAILS, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        PIXELS(512), 0, PIXELS(416), FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_OLD_MAN_WITH_WHITE_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        PIXELS(536), 0, PIXELS(451), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        PIXELS(416), 0, PIXELS(464), FACING_NORTH, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_WANDER,
        PIXELS(536), 0, PIXELS(672), FACING_NORTH, TALK_FACING_PARTY, 0 },
    { SPRITE_BALD_MAN_IN_RED, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        PIXELS(200), 0, PIXELS(392), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_YOUNG_MAN_IN_GREEN, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        PIXELS(88), 0, PIXELS(392), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_WOMAN_WITH_PONYTAIL, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        PIXELS(632), 0, PIXELS(136), FACING_SOUTH, 3, 0 },
    { SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        PIXELS(664), 0, PIXELS(632), FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_WOMAN_IN_PINK_APRON, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        PIXELS(696), 0, PIXELS(696), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_MAN_WITH_LONG_DARK_HAIR, FLAG_VAULT_THIEVES_ATTACKED, MOVEMENT_TYPE_STAND,
        PIXELS(104), 0, PIXELS(696), FACING_EAST, 3, 0 },
    { SPRITE_PERSON_IN_PURPLE, FLAG_VAULT_THIEVES_ATTACKED, MOVEMENT_TYPE_STAND,
        PIXELS(200), 0, PIXELS(680), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_CRATE, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        PIXELS(692), 0, PIXELS(411), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SCENE_TABLE_END },
};

struct ActorPlacement gLogRoomsA_LoftActorPlacements[] = {
    { ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(8), 0, PIXELS(8), FACING_NORTH, TALK_FACING_PARTY, 0 },
    { ACTOR_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(376), 0, PIXELS(392), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(160), 0, PIXELS(136), FACING_NORTHWEST + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_OLD_MAN_WITH_WHITE_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(504), 0, PIXELS(456), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_MAN_WITH_LONG_DARK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(104), 0, PIXELS(696), FACING_EAST, 3, 0 },
    { SPRITE_PERSON_IN_PURPLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(200), 0, PIXELS(680), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_PERSON_IN_PURPLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(8), 0, PIXELS(8), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(8), 0, PIXELS(8), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(8), 0, PIXELS(8), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_CHEST, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        PIXELS(872), 0, PIXELS(424), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_CHEST, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        PIXELS(872), 0, PIXELS(408), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_CHEST, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        PIXELS(872), 0, PIXELS(376), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    {SPRITE_251, CONDITION_ALWAYS, 7,
        PIXELS(8), 0, PIXELS(8), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    {SPRITE_221, CONDITION_ALWAYS, 7,
        PIXELS(8), 0, PIXELS(8), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct Trigger gLogRoomsA_Triggers[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, LOG_ROOMS_A_TO_VAULT_EAST_SOUTHEAST },
    { TRIGGER_EXIT, 2, CONDITION_ALWAYS, LOG_ROOMS_A_TO_VAULT_SOUTHEAST },
    { TRIGGER_EXIT, 3, CONDITION_ALWAYS, LOG_ROOMS_A_TO_VAULT_SOUTHWEST },
    { TRIGGER_EXIT, 4, CONDITION_ALWAYS, LOG_ROOMS_A_TO_VAULT_EAST },
    { TRIGGER_EXIT, 5, CONDITION_ALWAYS, LOG_ROOMS_A_TO_VAULT_NORTH_NORTHEAST },
    { TRIGGER_EXIT, 6, CONDITION_ALWAYS, LOG_ROOMS_A_TO_VAULT_NORTHWEST },
    { TRIGGER_STEP, CELL_BEHAVIOR_LOG_ROOMS_A_GUESTS_ROOM_STAIRS, CONDITION_ALWAYS, EVENT(LogRoomsA_LeaveGuestsRoom) },
    { TRIGGER_EXIT, 8, CONDITION_ALWAYS, LOG_ROOMS_A_INN_TO_INN_UPSTAIRS },
    { TRIGGER_STEP, CELL_BEHAVIOR_LOG_ROOMS_A_GUESTS_ROOM, IF_FLAG_SET(FLAG_VAULT_IVAN_JOINED_ROD_SEARCH), EVENT(LogRoomsA_ApproachSuspects) },
    { TRIGGER_STEP, CELL_BEHAVIOR_LOG_ROOMS_A_FIRST_SUSPECT_ROUTE, IF_FLAG_SET(FLAG_SUSPECTS_FLEEING), EVENT(LogRoomsA_SuspectsRunFromParty) },
    { TRIGGER_STEP, CELL_BEHAVIOR_LOG_ROOMS_A_FIRST_SUSPECT_ROUTE + 1, IF_FLAG_SET(FLAG_SUSPECTS_FLEEING), EVENT(LogRoomsA_SuspectsRunFromParty) },
    { TRIGGER_STEP, CELL_BEHAVIOR_LOG_ROOMS_A_FIRST_SUSPECT_ROUTE + 2, IF_FLAG_SET(FLAG_SUSPECTS_FLEEING), EVENT(LogRoomsA_SuspectsRunFromParty) },
    { TRIGGER_STEP, CELL_BEHAVIOR_LOG_ROOMS_A_FIRST_SUSPECT_ROUTE + 3, IF_FLAG_SET(FLAG_SUSPECTS_FLEEING), EVENT(LogRoomsA_SuspectsRunFromParty) },
    { TRIGGER_STEP, CELL_BEHAVIOR_LOG_ROOMS_A_FIRST_SUSPECT_ROUTE + 4, IF_FLAG_SET(FLAG_SUSPECTS_FLEEING), EVENT(LogRoomsA_SuspectsRunFromParty) },
    { TRIGGER_STEP, CELL_BEHAVIOR_LOG_ROOMS_A_FIRST_SUSPECT_ROUTE + 5, IF_FLAG_SET(FLAG_SUSPECTS_FLEEING), EVENT(LogRoomsA_SuspectsRunFromParty) },
    { TRIGGER_STEP, CELL_BEHAVIOR_LOG_ROOMS_A_FIRST_SUSPECT_ROUTE + 6, IF_FLAG_SET(FLAG_SUSPECTS_FLEEING), EVENT(LogRoomsA_SuspectsRunFromParty) },
    { TRIGGER_STEP, CELL_BEHAVIOR_LOG_ROOMS_A_LAST_SUSPECT_ROUTE, IF_FLAG_SET(FLAG_SUSPECTS_FLEEING), EVENT(LogRoomsA_SuspectsRunFromParty) },
    { TRIGGER_STEP, CELL_BEHAVIOR_LOG_ROOMS_A_GUESTS_ROOM_DOOR, IF_FLAG_SET(FLAG_SUSPECTS_FLEEING), EVENT(LogRoomsA_DiscussSuspiciousGuests) },
    { TRIGGER_STEP, CELL_BEHAVIOR_LOG_ROOMS_A_CORNERING_START, IF_FLAG_SET(FLAG_VAULT_SUSPECTS_APPROACHED), EVENT(LogRoomsA_StartCorneringSuspects) },
    { TRIGGER_RAISED, LOG_ROOMS_A_LEADER_CORNERED, CONDITION_ALWAYS, EVENT(LogRoomsA_ReadCorneredSuspectsMinds) },
    { TRIGGER_RAISED, LOG_ROOMS_A_THIEF_CORNERED, CONDITION_ALWAYS, EVENT(LogRoomsA_ReadCorneredSuspectsMinds) },
    { TRIGGER_RAISED, LOG_ROOMS_A_BOTH_CORNERED, CONDITION_ALWAYS, EVENT(LogRoomsA_ReadCorneredSuspectsMinds) },
    { TRIGGER_RAISED, FIELD_RAISED_TRIGGER_MENUS, CONDITION_ALWAYS, EVENT(LogRoomsA_GaretRefusesToLeave) },
    { TRIGGER_STEP, CELL_BEHAVIOR_LOG_ROOMS_A_LOFT_CLIMB, CONDITION_ALWAYS, EVENT(LogRoomsA_ClimbToLoft) },
    { STEP_TRIGGER_WALKING, CELL_BEHAVIOR_LOG_ROOMS_A_ROOF_GAP, CONDITION_ALWAYS, EVENT(LogRoomsA_JumpAcrossRoofGap) },
    { TRIGGER_TALK, ACTOR_IVAN, IF_FLAG_SET(FLAG_VAULT_CORNERING_SUSPECTS), 0 },
    { TRIGGER_TALK, ACTOR_IVAN, CONDITION_ALWAYS, EVENT(LogRoomsA_MeetIvan) },
    { TRIGGER_TALK, ACTOR_GEM_OWNER, CONDITION_ALWAYS, MSGNO(MsgVaultThisRockWasBlownHereAll) },
    { TRIGGER_TALK, ACTOR_GEM_OWNERS_WIFE, CONDITION_ALWAYS, EVENT(LogRoomsA_TalkAboutRocks) },
    { TRIGGER_TALK, ACTOR_VILLAGER_SUSPECTING_THE_NEIGHBOR, CONDITION_ALWAYS, MSGNO(MsgVaultMyNeighborCreepsMeOutHe) },
    { TRIGGER_TALK, ACTOR_BOY_ASKING_ABOUT_TRAVEL, CONDITION_ALWAYS, EVENT(LogRoomsA_BoyAsksAboutTravel) },
    { TRIGGER_TALK, ACTOR_GRANDPA, CONDITION_ALWAYS, EVENT(LogRoomsA_TalkAboutMissingThings) },
    { TRIGGER_TALK, ACTOR_GRANDPAS_WIFE, CONDITION_ALWAYS, MSGNO(MsgVaultEverSinceTheEruptionGrandpaKeeps) },
    { TRIGGER_TALK, ACTOR_BOY_WISHING_TO_BE_OLDER, FLAG_VAULT_IVAN_JOINED_ROD_SEARCH, MSGNO(MsgVaultICouldHelpIvanIfI) },
    { TRIGGER_TALK, ACTOR_BOY_WISHING_TO_BE_OLDER, CONDITION_ALWAYS, MSGNO(MsgVaultYoureHelpingIvanYouDontLook) },
    { TRIGGER_TALK, ACTOR_GIRL_TELLING_A_SECRET, FLAG_VAULT_IVAN_JOINED_ROD_SEARCH, EVENT(LogRoomsA_OfferIvansSecret) },
    { TRIGGER_TALK, ACTOR_GIRL_TELLING_A_SECRET, CONDITION_ALWAYS, MSGNO(MsgVaultSinceYoureHelpingIvanIllTell) },
    { TRIGGER_TALK, ACTOR_MAYOR, CONDITION_ALWAYS, EVENT(LogRoomsA_MayorAsksHelpForIvan) },
    { TRIGGER_TALK, ACTOR_MAYORS_WIFE, FLAG_VAULT_IVAN_JOINED_ROD_SEARCH, MSGNO(MsgVaultMyHusbandTheMayorCantBelieve) },
    { TRIGGER_TALK, ACTOR_MAYORS_WIFE, CONDITION_ALWAYS, MSGNO(MsgVaultReallyYoureGoingToHelpIvan) },
    { TRIGGER_TALK, ACTOR_GIRL_AT_THE_INN, CONDITION_ALWAYS, EVENT(LogRoomsA_WarnAboutThreeStrangers) },
    { TRIGGER_TALK, ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS, EVENT(LogRoomsA_TalkToWeaponMerchant) },
    { TRIGGER_TALK, ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS, EVENT(LogRoomsA_TalkToArmorMerchant) },
    { TRIGGER_TALK, ACTOR_ITEM_MERCHANT, CONDITION_ALWAYS, EVENT(LogRoomsA_TalkToItemMerchant) },
    { TRIGGER_TALK, ACTOR_INN_STAFF, CONDITION_ALWAYS, MSGNO(MsgVaultTheGuestsUpstairsWerentInTheir) },
    { TRIGGER_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS, EVENT(LogRoomsA_TalkToInnkeeper) },
    { TRIGGER_TALK, ACTOR_SUSPECTED_LEADER, IF_FLAG_SET(FLAG_VAULT_CORNERING_SUSPECTS), 0 },
    { TRIGGER_TALK, ACTOR_SUSPECTED_THIEF, IF_FLAG_SET(FLAG_VAULT_CORNERING_SUSPECTS), 0 },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SUSPECTED_LEADER, IF_FLAG_SET(FLAG_VAULT_CORNERING_SUSPECTS), 0 },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SUSPECTED_THIEF, IF_FLAG_SET(FLAG_VAULT_CORNERING_SUSPECTS), 0 },
    { TRIGGER_TALK, ACTOR_SUSPECTED_LEADER, FLAG_VAULT_SUSPECTS_MIND_READ, MSGNO(MsgVaultThatKidIvanHeCreepsMe) },
    { TRIGGER_TALK, ACTOR_SUSPECTED_THIEF, FLAG_VAULT_SUSPECTS_MIND_READ, MSGNO(MsgVaultWeHaventDoneAnythingYoureA) },
    { TRIGGER_TALK, ACTOR_SUSPECTED_LEADER, CONDITION_ALWAYS, EVENT(LogRoomsA_SuspectedLeaderDodges) },
    { TRIGGER_TALK, ACTOR_SUSPECTED_THIEF, CONDITION_ALWAYS, EVENT(LogRoomsA_SuspectedThiefDodges) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GEM_OWNER, CONDITION_ALWAYS, MSGNO(MsgVaultTheOtherSparklingRocksAllCrumbled) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GEM_OWNERS_WIFE, CONDITION_ALWAYS, EVENT(LogRoomsA_ReadMindOnRock) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_SUSPECTING_THE_NEIGHBOR, CONDITION_ALWAYS, MSGNO(MsgVaultHeyMaybeMyNeighborIsThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_ASKING_ABOUT_TRAVEL, CONDITION_ALWAYS, EVENT(LogRoomsA_ReadBoysMind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GRANDPA, CONDITION_ALWAYS, EVENT(LogRoomsA_ReadMindOnThieves) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GRANDPAS_WIFE, CONDITION_ALWAYS, MSGNO(MsgVaultYouNeverBoughtMeAnythingWorth) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_WISHING_TO_BE_OLDER, CONDITION_ALWAYS, MSGNO(MsgVaultTheseGuysLookReliableIGuess) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_TELLING_A_SECRET, CONDITION_ALWAYS, MSGNO(MsgVaultIWonderWhatSortOfThing) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAYOR, CONDITION_ALWAYS, EVENT(LogRoomsA_ReadMayorMind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAYORS_WIFE, CONDITION_ALWAYS, MSGNO(MsgVaultMaybeTheyllFindOurStolenFamily) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_AT_THE_INN, CONDITION_ALWAYS, EVENT(LogRoomsA_ReadBoneGiverMind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgVaultAllOurBestWeaponsWereStolen) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgVaultIWonderIfTheyLeftUs) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ITEM_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgVaultHahhWereSureToSeeA) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INN_STAFF, CONDITION_ALWAYS, MSGNO(MsgVaultItWouldBeAwfulIfOur) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, CONDITION_ALWAYS, MSGNO(MsgVaultThatVolcanoCostUsALot) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TRIGGER_SUBJECT_OWN_EFFECT | ACTOR_SUSPECTED_LEADER, CONDITION_ALWAYS, EVENT(LogRoomsA_SuspectedLeaderDodges) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TRIGGER_SUBJECT_OWN_EFFECT | ACTOR_SUSPECTED_THIEF, CONDITION_ALWAYS, EVENT(LogRoomsA_SuspectedThiefDodges) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_CRATE, FLAG_VAULT_CRATE_MOVED, EVENT(LogRoomsA_CheckCrateMoved) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_LOG_ROOMS_A_INN_BARREL, CONDITION_ALWAYS, EVENT(LogRoomsA_SearchEmptyBarrel) },
    { EXAMINE_TRIGGER(EXAMINE_JAR), CELL_BEHAVIOR_LOG_ROOMS_A_JAR, FLAG_LOG_ROOMS_A_JAR_COINS, FIND_COINS(7) },
    { EXAMINE_TRIGGER(EXAMINE_BARREL), CELL_BEHAVIOR_LOG_ROOMS_A_FIRST_BARREL, FLAG_LOG_ROOMS_A_BARREL_COINS, FIND_COINS(4) },
    { EXAMINE_TRIGGER(EXAMINE_BARREL), CELL_BEHAVIOR_LOG_ROOMS_A_SECOND_BARREL, FLAG_LOG_ROOMS_A_BARREL_SLEEP_BOMB, FIND_ITEM(ITEM_SLEEP_BOMB) },
    { EXAMINE_TRIGGER(EXAMINE_WOODEN_BOX), CELL_BEHAVIOR_LOG_ROOMS_A_WOODEN_BOX, FLAG_LOG_ROOMS_A_BOX_MINT, FIND_ITEM(ITEM_MINT) },
    { EXAMINE_TRIGGER(EXAMINE_BOOKCASE) | TRIGGER_FLAG_FACING | FACING_NORTH, CELL_BEHAVIOR_LOG_ROOMS_A_HISTORY_BOOKCASE, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgVaultTheresABookEntitledHistoryOf)) },
    { EXAMINE_TRIGGER(EXAMINE_BOOKCASE), CELL_BEHAVIOR_LOG_ROOMS_A_GEMS_BOOKCASE, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgVaultTheresABookEntitledGemsOf)) },
    { TRIGGERS_END },
};

struct Trigger gLogRoomsA_LoftTriggers[] = {
    { TRIGGER_STEP, CELL_BEHAVIOR_LOG_ROOMS_A_LOFT_DESCENT, CONDITION_ALWAYS, EVENT(LogRoomsA_LeaveLoft) },
    { TRIGGER_TALK, ACTOR_TIED_UP_VILLAGER, CONDITION_ALWAYS, EVENT(LogRoomsA_FreeRooferAndConfrontThieves) },
    { TRIGGER_TALK, ACTOR_FIRST_CHEST, CONDITION_ALWAYS, EVENT(LogRoomsA_SearchEmptyChest) },
    { TRIGGER_TALK, ACTOR_SECOND_CHEST, CONDITION_ALWAYS, EVENT(LogRoomsA_SearchEmptyChest) },
    { TRIGGER_TALK, ACTOR_THIRD_CHEST, CONDITION_ALWAYS, EVENT(LogRoomsA_SearchEmptyChest) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TIED_UP_VILLAGER, CONDITION_ALWAYS, MSGNO(MsgVaultAhhHelpMe) },
    { TRIGGERS_END },
};

struct Trigger gLogRoomsA_TriggersAfterThieves[] = {
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_LOG_ROOMS_A_INN_BARREL, CONDITION_ALWAYS, EVENT(Vault_FindBone) },
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, LOG_ROOMS_A_TO_VAULT_EAST_SOUTHEAST },
    { TRIGGER_EXIT, 2, CONDITION_ALWAYS, LOG_ROOMS_A_TO_VAULT_SOUTHEAST },
    { TRIGGER_EXIT, 3, CONDITION_ALWAYS, LOG_ROOMS_A_TO_VAULT_SOUTHWEST },
    { TRIGGER_EXIT, 4, CONDITION_ALWAYS, LOG_ROOMS_A_TO_VAULT_EAST },
    { TRIGGER_EXIT, 5, CONDITION_ALWAYS, LOG_ROOMS_A_TO_VAULT_NORTH_NORTHEAST },
    { TRIGGER_EXIT, 6, CONDITION_ALWAYS, LOG_ROOMS_A_TO_VAULT_NORTHWEST },
    { TRIGGER_EXIT, 7, CONDITION_ALWAYS, LOG_ROOMS_A_INN_UPSTAIRS_TO_INN },
    { TRIGGER_EXIT, 8, CONDITION_ALWAYS, LOG_ROOMS_A_INN_TO_INN_UPSTAIRS },
    { TRIGGER_STEP, CELL_BEHAVIOR_LOG_ROOMS_A_LOFT_CLIMB, CONDITION_ALWAYS, EVENT(LogRoomsA_ClimbToLoft) },
    { TRIGGER_STEP, CELL_BEHAVIOR_LOG_ROOMS_A_LOFT_DESCENT, CONDITION_ALWAYS, EVENT(LogRoomsA_LeaveLoft) },
    { TRIGGER_TALK, ACTOR_GEM_OWNER, CONDITION_ALWAYS, MSGNO(MsgVaultICanSleepWellNowThat) },
    { TRIGGER_TALK, ACTOR_GEM_OWNERS_WIFE, CONDITION_ALWAYS, EVENT(LogRoomsA_TalkAboutRocks) },
    { TRIGGER_TALK, ACTOR_VILLAGER_SUSPECTING_THE_NEIGHBOR, CONDITION_ALWAYS, EVENT(LogRoomsA_AskWhetherThievesCameFromLunpa) },
    { TRIGGER_TALK, ACTOR_BOY_ASKING_ABOUT_TRAVEL, CONDITION_ALWAYS, EVENT(LogRoomsA_BoyWorriesAboutFather) },
    { TRIGGER_TALK, ACTOR_GRANDPA, CONDITION_ALWAYS, EVENT(LogRoomsA_TalkAboutMissingThings) },
    { TRIGGER_TALK, ACTOR_GRANDPAS_WIFE, CONDITION_ALWAYS, MSGNO(MsgVaultJustAsIThoughtNothingWas) },
    { TRIGGER_TALK, ACTOR_BOY_WISHING_TO_BE_OLDER, CONDITION_ALWAYS, EVENT(LogRoomsA_AskAboutDefeatingThieves) },
    { TRIGGER_TALK, ACTOR_GIRL_TELLING_A_SECRET, CONDITION_ALWAYS, MSGNO(MsgVaultIvanLearnedALotByTalking) },
    { TRIGGER_TALK, ACTOR_MAYOR, CONDITION_ALWAYS, EVENT(LogRoomsA_MayorGivesWaterOfLife) },
    { TRIGGER_TALK, ACTOR_MAYORS_WIFE, CONDITION_ALWAYS, MSGNO(MsgVaultYoureThePersonWhoGotOur) },
    { TRIGGER_TALK, ACTOR_GIRL_AT_THE_INN, FLAG_VAULT_LEADER_BESIDE_BONE, EVENT(LogRoomsA_OfferBones) },
    { TRIGGER_TALK, ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS, EVENT(LogRoomsA_TalkToWeaponMerchant) },
    { TRIGGER_TALK, ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS, EVENT(LogRoomsA_TalkToArmorMerchant) },
    { TRIGGER_TALK, ACTOR_ITEM_MERCHANT, CONDITION_ALWAYS, EVENT(LogRoomsA_TalkToItemMerchant) },
    { TRIGGER_TALK, ACTOR_INN_STAFF, CONDITION_ALWAYS, MSGNO(MsgVaultImShockedThoseGuestsOfOurs) },
    { TRIGGER_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS, EVENT(LogRoomsA_TalkToInnkeeper) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GEM_OWNER, CONDITION_ALWAYS, MSGNO(MsgVaultEverybodysAfterMyRockTheyWant) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GEM_OWNERS_WIFE, CONDITION_ALWAYS, EVENT(LogRoomsA_ReadMindOnRock) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_SUSPECTING_THE_NEIGHBOR, CONDITION_ALWAYS, MSGNO(MsgVaultTheReasonLunpaTurnedBadIs) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_ASKING_ABOUT_TRAVEL, CONDITION_ALWAYS, EVENT(LogRoomsA_ReadBoysMind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GRANDPA, CONDITION_ALWAYS, EVENT(LogRoomsA_ReadMindOnThieves) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GRANDPAS_WIFE, CONDITION_ALWAYS, MSGNO(MsgVaultIWishIHadSomethingWorth) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_WISHING_TO_BE_OLDER, CONDITION_ALWAYS, MSGNO(MsgVaultWhenIGrowUpIWant) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_TELLING_A_SECRET, CONDITION_ALWAYS, MSGNO(MsgVaultIWishIHadWingsLike) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAYOR, CONDITION_ALWAYS, EVENT(LogRoomsA_ReadMayorMind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAYORS_WIFE, CONDITION_ALWAYS, MSGNO(MsgVaultImSoRelievedNowThatWe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_AT_THE_INN, CONDITION_ALWAYS, EVENT(LogRoomsA_ReadBoneGiverMind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgVaultMyWeaponsWereFoundInThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgVaultItMustBeSnowingUpIn) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ITEM_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgVaultWhyAreThesePeopleSoAnxious) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INN_STAFF, CONDITION_ALWAYS, MSGNO(MsgVaultWhoCouldHaveKnownTheyWere) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, CONDITION_ALWAYS, MSGNO(MsgVaultOfCourseIAlwaysSuspectedThey) },
    { EXAMINE_TRIGGER(EXAMINE_JAR), CELL_BEHAVIOR_LOG_ROOMS_A_JAR, FLAG_LOG_ROOMS_A_JAR_COINS, FIND_COINS(7) },
    { EXAMINE_TRIGGER(EXAMINE_BARREL), CELL_BEHAVIOR_LOG_ROOMS_A_FIRST_BARREL, FLAG_LOG_ROOMS_A_BARREL_COINS, FIND_COINS(4) },
    { EXAMINE_TRIGGER(EXAMINE_BARREL), CELL_BEHAVIOR_LOG_ROOMS_A_SECOND_BARREL, FLAG_LOG_ROOMS_A_BARREL_SLEEP_BOMB, FIND_ITEM(ITEM_SLEEP_BOMB) },
    { EXAMINE_TRIGGER(EXAMINE_WOODEN_BOX), CELL_BEHAVIOR_LOG_ROOMS_A_WOODEN_BOX, FLAG_LOG_ROOMS_A_BOX_MINT, FIND_ITEM(ITEM_MINT) },
    { EXAMINE_TRIGGER(EXAMINE_BOOKCASE) | TRIGGER_FLAG_FACING | FACING_NORTH, CELL_BEHAVIOR_LOG_ROOMS_A_HISTORY_BOOKCASE, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgVaultTheresABookEntitledHistoryOf)) },
    { EXAMINE_TRIGGER(EXAMINE_BOOKCASE), CELL_BEHAVIOR_LOG_ROOMS_A_GEMS_BOOKCASE, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgVaultTheresABookEntitledGemsOf)) },
    { TRIGGERS_END },
};
// clang-format on

s32 *gLogRoomsA_StepActions[] = {
    gLogRoomsA_SuspectToMiddleBedHead,    gLogRoomsA_SuspectToEastBedHead,
    gLogRoomsA_SuspectRoundToEastBedFoot, gLogRoomsA_SuspectToMiddleBedFoot,
    gLogRoomsA_SuspectToWestBedFoot,      gLogRoomsA_SuspectRoundToWestBedHead,
};

s32 *gLogRoomsA_IdleActions[] = {
    gLogRoomsA_SuspectRoundToWestBedFoot, gLogRoomsA_SuspectToWestBedHead,
    gLogRoomsA_SuspectToMiddleBedHead,    gLogRoomsA_SuspectRoundToEastBedHead,
    gLogRoomsA_SuspectToEastBedFoot,      gLogRoomsA_SuspectToMiddleBedFoot,
};

s32 *gLogRoomsA_ResponseScripts[] = {
    gLogRoomsA_SuspectAcrossToEastOfBeds,
    gLogRoomsA_SuspectToSouthEastCorner,
    gLogRoomsA_SuspectAlongToSouthWestCorner,
    gLogRoomsA_SuspectToWestOfBeds,
    gLogRoomsA_SuspectToSouthWestCorner,
    gLogRoomsA_SuspectAcrossToWestOfBeds,
    gLogRoomsA_SuspectToEastOfBeds,
    gLogRoomsA_SuspectAlongToSouthEastCorner,
};

u16 gLogRoomsA_StopTimer;
