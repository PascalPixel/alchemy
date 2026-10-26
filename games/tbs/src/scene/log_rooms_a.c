/// @file
/// Log rooms A (SceneId_LogRoomsA), a sheet of rooms drawn in logs, all of them
/// Vault's: the insides of the town's houses, shops and inn, and the inn's
/// loft, while Hammet's Shaman's Rod is missing and just after it is found.
/// Every room is on one map, each a warp destination with a camera box of its
/// own, and Vault's doors (vault.c) lead to the warp destinations named for
/// them: the gem owner's house, the child's house, the inn and its guests' room
/// upstairs, the item shop, the weapon and armor shop, the mayor's house and a
/// room with a crate, behind which a way climbs to the inn's loft. The loft has
/// object event templates and events of its own
/// (LogRoomsA_GetObjectEventTemplates and _GetEvents), and
/// FLAG_VAULT_THIEVES_JAILED chooses the other rooms' tables.
///
/// The story flags drive the rooms in order:
/// - At the mayor's, talking to Ivan plays EventScript_MeetIvan: he reads
/// Isaac's
///   mind (FLAG_VAULT_MET_IVAN), asks for help with the rod, and on a yes
///   joins the party (LogRoomsA_IvanJoinsRodSearch,
///   FLAG_VAULT_IVAN_JOINED_ROD_SEARCH).
/// - In the inn's guests' room the two suspects run from Ivan
///   (EventScript_ApproachSuspects), and stepping out the party plans to corner
///   one (EventScript_DiscussSuspiciousGuests). The chase
///   (EventScript_StartCorneringSuspects) moves Ivan and the suspects along a
///   network of stops (gLogRoomsA_Stops); once one has been blocked for
///   half a second, LogRoomsA_UpdateObjectEventStops raises
///   LOG_ROOMS_A_LEADER_CORNERED, _THIEF_CORNERED or _BOTH_CORNERED, and
///   Ivan reads the cornered suspect's mind
///   (EventScript_ReadCorneredSuspectsMinds, FLAG_VAULT_SUSPECTS_MIND_READ).
///   Meanwhile the menus answer with Garet's refusal (FLAG_MENUS_RAISE_EVENT).
/// - In the room with the crate, Move pushes the crate aside
///   (EventScript_CheckCrateMoved, FLAG_VAULT_CRATE_MOVED), and the way behind
///   it climbs to the loft (EventScript_ClimbToLoft). Freeing the villager tied
///   up there brings out the three thieves, who attack
///   (EventScript_FreeRooferAndConfrontThieves, FLAG_VAULT_THIEVES_ATTACKED). A
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
/// be read. The cells the events answer are enum LogRoomsAMetatileBehavior
/// (log_rooms_a.h).
///
/// EventScript_ApproachSuspects plays once: it copies
/// MB_LOG_ROOMS_A_GUESTS_ROOM_DOOR cells over the guests' room's
/// MB_LOG_ROOMS_A_GUESTS_ROOM cells, EventScript_DiscussSuspiciousGuests makes
/// them MB_LOG_ROOMS_A_CORNERING_START, and every later arrival with
/// FLAG_VAULT_SUSPECTS_APPROACHED set rewrites them again (render probe
/// log-rooms-a: the room has no GUESTS_ROOM cell once the flag is set).

#include "save_block.h"
#include "flags.h"
#include "animspr.h"
#include "version.h"
#include "scene_ids.h"
#include "unit/import.h"
#include "unit/scene/log_rooms_a.h"
#include "log_rooms_a.h"
#include "unit/scene/common/vault_itemobj.h"
#include "unit/overworld/overworld.h"
#include "unit/overworld/field_services.h"
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
#include "task.h"
#include "item_ids.h"
#include "script_layer.h"
#include "map_tables.h"
#include "tbs_edition.h"
#include "battle_party.h"
#include "text/msg_ids.h"
#include "field_service.h"
#include "ability_ids.h"
#include "objcmd.h"
#include "fixed_math.h"
#include "macros.h"
#include "songs.h"
#include "heap.h"
#include "audio_engine_symbols.h"

#if TBS_PATCH_3
#define LOG_ROOMS_A_ENTRY_OBJ_EVENT_Z PIXELS(451)
#else
#define LOG_ROOMS_A_ENTRY_OBJ_EVENT_Z PIXELS(464)
#endif

/// A cornering lasts half a second before the chase raises an event.
#define LOG_ROOMS_A_CORNERED_FRAMES 30

/// The chase adds the blocked-suspect mask (1 the leader, 2 the thief) to 200.
enum LogRoomsARaisedEvent {
  LOG_ROOMS_A_CORNERED_EVENT_BASE = 200,
  LOG_ROOMS_A_LEADER_CORNERED,
  LOG_ROOMS_A_THIEF_CORNERED,
  LOG_ROOMS_A_BOTH_CORNERED
};

/// The townspeople, named by what they say, numbering
/// gLogRoomsA_ObjectEventTemplates' rows from LOCALID_FIRST_PLACED;
/// Garet's and Ivan's rows take their own slots.
enum {
  /// A young man, to whose house a sparkling rock was blown from the volcano.
  /// ★ Named from the game: "I kept worrying that they would take my gem."
  /// (MsgVaultICanSleepWellNowThat)
  LOCALID_GEM_OWNER = 8,
  /// A woman with fair hair.
  /// ★ Named from the game: "My husband says my cat ate the treasure"
  /// (MsgVaultMyHusbandSaysMyCatAte)
  LOCALID_GEM_OWNERS_WIFE = 9,
  /// A burly grey-haired man who distrusts the man next door. "My neighbor
  /// creeps me out."
  /// ★ Named from the game: "Maybe my neighbor is the thief..."
  /// (MsgVaultHeyMaybeMyNeighborIsThe)
  LOCALID_VILLAGER_SUSPECTING_THE_NEIGHBOR = 10,
  /// A small boy who asks about travel.
  /// ★ Named from the game: "Hey, mister! Is it fun to see strange new places?"
  /// (MsgVaultMisterFunSeeStrangeNew)
  LOCALID_BOY_ASKING_ABOUT_TRAVEL = 11,
  /// An old man with a cane who wonders what was stolen.
  /// ★ Named from the game: "Is it just me, or am I missing something?"
  /// (MsgVaultJustMeOrAmMissing)
  LOCALID_GRANDPA = 12,
  /// An old woman, who says Grandpa keeps wondering if anything was stolen.
  /// ★ Named from the game: "You never bought me anything worth stealing!"
  /// (MsgVaultYouNeverBoughtMeAnythingWorth)
  LOCALID_GRANDPAS_WIFE = 13,
  /// A small boy. "I could help Ivan if I were just a little older."
  /// ★ Named from the game: "if I were just a little older."
  /// (MsgVaultICouldHelpIvanIfI)
  LOCALID_BOY_WISHING_TO_BE_OLDER = 14,
  /// A little girl who tells the party Ivan's secret once it helps him.
  /// ★ Named from the game: "I'll tell you a secret..."
  /// (MsgVaultSinceYoureHelpingIvanIllTell)
  LOCALID_GIRL_TELLING_A_SECRET = 15,
  /// Ivan's host, an old man with white hair.
  /// ★ Named from the game: "My husband, the mayor, can't believe anyone from
  /// Vault could be the thief." (MsgVaultMyHusbandTheMayorCantBelieve)
  LOCALID_MAYOR = 16,
  /// A woman in a green dress.
  /// ★ Named from the game: "My husband, the mayor, can't believe anyone from
  /// Vault could be the thief." (MsgVaultMyHusbandTheMayorCantBelieve)
  LOCALID_MAYORS_WIFE = 17,
  /// The girl at the inn, as the Bone's description calls her, a young woman
  /// with fair hair. "I have a lot
  /// of leftover bones from dinner."
  /// ★ Named from the game: "A bone from the girl at the inn"
  /// (MsgItemPlainName)
  LOCALID_GIRL_AT_THE_INN = 18,
  /// ★ Named from the game: "I'm sorry I've only got these cheap weapons to
  /// sell right now." (MsgVaultAllOurBestWeaponsWereStolen)
  LOCALID_WEAPON_MERCHANT = 19,
  /// ★ Named from the game: "I wonder if they left us alone because we don't
  /// carry any valuable armor?" (MsgVaultIWonderIfTheyLeftUs)
  LOCALID_ARMOR_MERCHANT = 20,
  /// ★ Named from what the player sees: the shopkeeper behind a counter of
  /// jars and sacks (its render)
  LOCALID_ITEM_MERCHANT = 21,
  /// A burly grey-haired man who works at the inn.
  /// ★ Named from the game: "The guests upstairs weren't in their room during
  /// the eruption." (MsgVaultTheGuestsUpstairsWerentInTheir)
  LOCALID_INN_STAFF = 22,
  /// A woman in an apron behind the inn's counter.
  /// ★ Named from the game: "Everyone thinks our guests are the thieves"
  /// (MsgVaultEveryoneThinksOurGuestsThieves)
  LOCALID_INNKEEPER = 23,
  /// The two guests at the inn the town suspects: the thieves' leader and one
  /// of his men.
  /// ★ Named from the game: "Everyone thinks our guests are the thieves"
  /// (MsgVaultEveryoneThinksOurGuestsThieves)
  LOCALID_SUSPECTED_LEADER = 24,
  /// ★ Named from the game: "You're a terrible person if you start accusing
  /// people for no good reason." (MsgVaultWeHaventDoneAnythingYoureA)
  LOCALID_SUSPECTED_THIEF = 25,
  /// Pushed aside to open a passage.
  /// ★ Named from what the player sees: a large wooden crate in the room below
  /// the loft (its render)
  LOCALID_CRATE = 26
};

/// Area flags of Vault's area (0x300 to 0x3ff, kept while the
/// party stays in Vault and its buildings).
enum {
  /// Set while the two suspects run from the party, until the party steps
  /// aside to talk it over.
  /// ★ Named from the game: "Don't let him near me!" (MsgVaultWant)
  FLAG_SUSPECTS_FLEEING = FLAG_AREA_TEMP_00
};

// The map flag the inn keeps as FLAG_VAULT_LEADER_BESIDE_BONE
// (common/vault_spotflag.h) once the thieves are jailed means something else in
// the guests' room:
/// The party is cornering the two suspects in the guests' room, so talking to
/// Ivan or the suspects does nothing.
/// ★ Named from the game: "Uh-oh! I'm surrounded!" (MsgVaultImSurrounded)
#define FLAG_VAULT_CORNERING_SUSPECTS FLAG_SCENE_TEMP_50

/// The inn's loft (LOG_ROOMS_A_LOFT_FROM_CRATE_ROOM and the two after the
/// battle), where the thieves hid the stolen goods, numbering
/// gLogRoomsA_LoftObjectEventTemplates' rows from LOCALID_FIRST_PLACED.
enum {
  /// vault.c's LOCALID_YOUNG_MAN_FIXING_THE_ROOF, tied up while fixing the
  /// roof.
  /// ★ Named from the game: "When I came to, I was all tied up."
  /// (MsgVaultVaultCutFree)
  LOCALID_TIED_UP_VILLAGER = 8,
  /// ★ Named from the game: "Mayor... Isn't that your family's urn?"
  /// (MsgVaultYouIsaacRightWontForget)
  LOCALID_LOFT_MAYOR = 9,
  /// Speaks first and bargains for all three ("See, we're not so bad. How
  /// about it? Will you let us go?").
  /// ★ Named from the game: "Looks like we've been found out!"
  /// (MsgVaultVaultCutFree)
  LOCALID_THIEF_LEADER = 10,
  /// ★ Named from the game: "These are the thieves, Mayor..."
  /// (MsgVaultSeeThatsHappened)
  LOCALID_FIRST_THIEF = 11,
  /// ★ Named from the game: "These are the thieves, Mayor..."
  /// (MsgVaultSeeThatsHappened)
  LOCALID_SECOND_THIEF = 12,
  /// A strongman who comes up with the mayor and helps tie the thieves up.
  /// ★ Named from the game: "What should we do with them?"
  /// (MsgVaultSeeThatsHappened)
  LOCALID_LOFT_ESCORT = 13,
  /// A strongman who comes up with the mayor and opens the chests.
  /// ★ Named from the game: "Mayor, we found another chest over here"
  /// (MsgVaultYouIsaacRightWontForget)
  LOCALID_STRONGMAN_OPENING_THE_CHESTS = 14,
  /// ★ Named from the game: "but the chest was empty."
  /// (MsgVaultButChestWasEmpty)
  LOCALID_FIRST_CHEST = 15,
  /// ★ Named from the game: "but the chest was empty."
  /// (MsgVaultButChestWasEmpty)
  LOCALID_SECOND_CHEST = 16,
  /// ★ Named from the game: "but the chest was empty."
  /// (MsgVaultButChestWasEmpty)
  LOCALID_THIRD_CHEST = 17,
  /// Rises from the first chest.
  /// ★ Named from the game: "Found the mayor's precious urn."
  /// (MsgVaultYouIsaacRightWontForget)
  LOCALID_FOUND_URN = 18,
  /// Rises from the second chest, stolen from the Sanctum.
  /// ★ Named from the game: "Found a gold statue."
  /// (MsgVaultYouIsaacRightWontForget)
  LOCALID_FOUND_STATUE = 19
};

static void LogRoomsA_IvanJoinsRodSearch(void);
static void LogRoomsA_DiscussMindReading(void);
static void LogRoomsA_AdvanceSuspectRoute(s32 localId, s32 expected, s32 next,
                                          const s32 *actions);
static void LogRoomsA_FaceSuspectsTowardLeader(void);
static void LogRoomsA_LoseToThieves(void);
static void LogRoomsA_ConfrontThievesAgain(void);
static void LogRoomsA_DefeatThieves(void);
static void LogRoomsA_ReturnStolenGoods(void);
static void LogRoomsA_OpenWindowTransition(void);
static void LogRoomsA_ShowMessageAndWait(s32 localId, s32 waitFrames);
static void LogRoomsA_FaceObjectEventAndWait(s32 localId, s32 targetId,
                                             s32 waitFrames);
static void LogRoomsA_FaceEachOtherAndWait(s32 firstId, s32 secondId,
                                           s32 waitFrames);
static void LogRoomsA_PlayAnimationAndWait(s32 localId, s32 animation,
                                           s32 waitFrames);
static void LogRoomsA_StartMindRead(s32 casterId, s32 targetId);
static void LogRoomsA_EndMindRead(void);
static void LogRoomsA_UpdateObjectEventStops(void);
static void LogRoomsA_StartObjectEventStops(void);

const struct MapWarpDest *Scene_GetWarpDests(void) {
  return gLogRoomsA_WarpDests;
}

/// Returns the one warp region: the box around (648, 416) in the room with the
/// crate, which takes LOG_ROOMS_A_TO_VAULT_WEST back out to Vault.
const struct MapWarpRegion *Scene_GetWarpRegions(void) {
  return gLogRoomsA_Regions;
}

const u32 *Scene_GetWarps(void) { return gLogRoomsA_Warps; }

/// Places the townspeople, or, for the inn's loft
/// (LOG_ROOMS_A_LOFT_FROM_CRATE_ROOM and the two after the battle), the
/// tied-up villager, the mayor, the thieves, their chests and the found
/// treasures.
const struct ObjectEventTemplate *Scene_GetObjectEventTemplates(void) {
  struct ObjectEventTemplate *table;

  switch (gSaveBlock.warpDest) {
  case LOG_ROOMS_A_LOFT_FROM_CRATE_ROOM:
  case LOG_ROOMS_A_LOFT_AFTER_LOSING_TO_THIEVES:
  case LOG_ROOMS_A_LOFT_AFTER_THIEVES_DEFEATED:
    table = gLogRoomsA_LoftObjectEventTemplates;
    break;
  default:
    table = gLogRoomsA_ObjectEventTemplates;
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
  struct ObjectEvent *leader;
  u8 *flags;
  u8 savedMotionFlags;

  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  flags = &leader->motionFlags;
  savedMotionFlags = *flags;
  if (FieldObject_CheckMovementCollision(leader, position) == 0) {
    FieldScript_Start();
    Object_SelectAnimation(leader, 6);
    WaitFrames(6);
    Sound_PlaySong(SE_JUMP);
    Object_SelectAnimation(leader, 7);
    leader->speed = PIXELS(3);
    leader->acceleration = PIXELS(2);
    leader->velocityY = PIXELS(4);
    *flags &= 0x7e;
    ObjectEvent_SetSpriteFlags(leader, 0);
    ObjectMotion_SetPositionAndCommit(
        LOCALID_PARTY_LEADER,
        ((union FieldCoordinate *)&position->x)->part.pixel,
        ((union FieldCoordinate *)&position->z)->part.pixel);
    Object_SelectAnimation(leader, 6);
    ObjectEvent_SetSpriteFlags(leader, SPRITE_FLAG_SHADOW);
    WaitFrames(6);
    *flags = savedMotionFlags;
    FieldScript_End();
    return 1;
  }
  return 0;
}
#endif

/// Indoors in Vault, facing north or south across the gap in the roof, the
/// leader jumps it.
///
/// Reached by walking onto MB_LOG_ROOMS_A_ROOF_GAP. Then copies the cell
/// (41, 85) or (43, 85) over the gap's cell (42, 85), whichever side the leader
/// faced.
static void EventScript_JumpAcrossRoofGap(void) {
  s32 x;
  s32 z;
#if !TBS_PATCH_1
  struct FieldPosition target;
  struct FieldPosition *position;
  struct ObjectEvent *leader;
#endif

  if (ObjectEvent_GetById(LOCALID_PARTY_LEADER)->facing >= FACING_NORTHWEST &&
      ObjectEvent_GetById(LOCALID_PARTY_LEADER)->facing <= FACING_NORTHEAST) {
#if TBS_PATCH_1
    Field_TryJumpForward();
#else
    leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
    position = &target;
    SET_POSITION(position, leader->x.fixed,
                 ObjectEvent_GetById(LOCALID_PARTY_LEADER)->y.fixed,
                 ObjectEvent_GetById(LOCALID_PARTY_LEADER)->z.fixed -
                     PIXELS(30));
    LogRoomsA_TryJumpTo(position);
#endif
    x = 42;
    z = 85;
    MapGrid_CopyCellAttributes(41, 85, 1, 1, x, z);
  } else if (ObjectEvent_GetById(LOCALID_PARTY_LEADER)->facing >=
                 FACING_SOUTHEAST &&
             ObjectEvent_GetById(LOCALID_PARTY_LEADER)->facing <=
                 FACING_SOUTHWEST) {
#if TBS_PATCH_1
    Field_TryJumpForward();
#else
    leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
    position = &target;
    SET_POSITION(position, leader->x.fixed,
                 ObjectEvent_GetById(LOCALID_PARTY_LEADER)->y.fixed,
                 ObjectEvent_GetById(LOCALID_PARTY_LEADER)->z.fixed +
                     PIXELS(30));
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
/// Reached by Move on the crate (LOCALID_CRATE) until FLAG_VAULT_CRATE_MOVED is
/// set; sets it when the crate has reached column 42.
static void EventScript_CheckCrateMoved(void) {
  struct ObjectEvent *crate;

  FieldScript_Start();
  crate = ObjectEvent_GetById(LOCALID_CRATE);
  if ((crate->x.fixed >> 20) == 42) {
    MapGrid_CopyCellAttributes(101, 24, 3, 4, 41, 24);
    GameFlag_Set(FLAG_VAULT_CRATE_MOVED);
  }
  FieldScript_End();
}

/// In Vault's weapon and armor shop, across the counter the weapon shop opens;
/// otherwise the merchant lost expensive goods too, or found the stolen
/// weapons rusted in the cemetery.
///
/// Reached by talking to the weapon merchant (LOCALID_WEAPON_MERCHANT), before
/// and after the thieves are jailed (FLAG_VAULT_THIEVES_JAILED picks the line).
/// The armor merchant, the inn and the item shop below work the same way: the
/// counter is between them when the leader faces north (0xa001 to 0xdfff).
static void EventScript_TalkToWeaponMerchant(void) {
  s32 leaderFacing = ObjectEvent_GetById(LOCALID_PARTY_LEADER)->facing;

  FieldScript_Start();
  if (leaderFacing > FACING_NORTHWEST && leaderFacing < FACING_NORTHEAST) {
    Shop_Run(4, LOCALID_WEAPON_MERCHANT);
  } else {
    if (GameFlag_Get(FLAG_VAULT_THIEVES_JAILED) == 0) {
      FieldScript_SetMessageId(MsgVaultMasterHammetIsntOnlyOne);
    } else {
      FieldScript_SetMessageId(MsgVaultWeFoundOurStolenWeapons);
    }
    FieldScript_ShowMessageAndWait(LOCALID_WEAPON_MERCHANT, 0);
  }
  FieldScript_End();
}

/// In Vault's weapon and armor shop, across the counter the armor shop opens;
/// otherwise the merchant wants the thieves to pay, or advises preparing for
/// the cave in the Goma Range.
///
/// Reached by talking to the armor merchant (LOCALID_ARMOR_MERCHANT).
static void EventScript_TalkToArmorMerchant(void) {
  s32 leaderFacing = ObjectEvent_GetById(LOCALID_PARTY_LEADER)->facing;

  FieldScript_Start();
  if (leaderFacing > FACING_NORTHWEST && leaderFacing < FACING_NORTHEAST) {
    Shop_Run(5, LOCALID_ARMOR_MERCHANT);
  } else {
    if (GameFlag_Get(FLAG_VAULT_THIEVES_JAILED) == 0) {
      FieldScript_SetMessageId(MsgVaultThievesDidntHitOurHouse);
    } else {
      FieldScript_SetMessageId(MsgVaultIfYoureGonnaHeadInto);
    }
    FieldScript_ShowMessageAndWait(LOCALID_ARMOR_MERCHANT, 0);
  }
  FieldScript_End();
}

/// At Vault's inn, across the counter the inn opens; otherwise the innkeeper
/// defends the guests everyone suspects, or learns the goods were hidden in
/// his loft.
///
/// Reached by talking to the innkeeper (LOCALID_INNKEEPER).
static void EventScript_TalkToInnkeeper(void) {
  s32 leaderFacing = ObjectEvent_GetById(LOCALID_PARTY_LEADER)->facing;

  FieldScript_Start();
  if (leaderFacing > FACING_NORTHWEST && leaderFacing < FACING_NORTHEAST) {
    Inn_CheckIn(1, LOCALID_INNKEEPER);
  } else {
    if (GameFlag_Get(FLAG_VAULT_THIEVES_JAILED) == 0) {
      FieldScript_SetMessageId(MsgVaultEveryoneThinksOurGuestsThieves);
    } else {
      FieldScript_SetMessageId(MsgVaultTheyHidThoseStolenGoods);
    }
    FieldScript_ShowMessageAndWait(LOCALID_INNKEEPER, 0);
  }
  FieldScript_End();
}

#include "common/vault_bone.h"

/// At Vault's inn, while the party corners the suspects, Garet refuses to stop
/// for anything else. "We don't have time for that, Isaac!"
///
/// Reached through FIELD_RAISED_EVENT_MENUS, which the engine raises in place
/// of a menu or a shortcut while FLAG_MENUS_RAISE_EVENT is set.
static void EventScript_GaretRefusesToLeave(void) {
#if TBS_PATCH_1
  Leader_StopMoving();
#endif
  FieldScript_SetMessageId(MsgVaultWeDontHaveTimeFor);
  FieldScript_ShowMessageAndWait(LOCALID_GARET, 0);
}

/// At Vault's inn, Isaac checks the barrel but doesn't find anything.
///
/// Reached by searching MB_LOG_ROOMS_A_INN_BARREL before the thieves are
/// jailed; after, the same spot holds the Bone (Vault_FindBone).
static void EventScript_SearchEmptyBarrel(void) {
  FieldScript_Start();
  FieldMessage_Show(MsgVaultIsaacCheckedBarrel, 1);
  FieldMessage_Show(MsgVaultButDidntFindAnything, 1);
  FieldScript_End();
}

/// In the inn's loft in Vault, Isaac checks the chest, but it is empty.
///
/// Reached by talking to one of the loft's three chests (LOCALID_FIRST_CHEST to
/// LOCALID_THIRD_CHEST).
static void EventScript_SearchEmptyChest(void) {
  FieldScript_Start();
  FieldMessage_Show(MsgVaultIsaacCheckedChest, 1);
  FieldMessage_Show(MsgVaultButChestWasEmpty, 1);
  FieldScript_End();
}

/// Returns the loft's events for the loft's three warp destinations, else the
/// houses' events, which change once FLAG_VAULT_THIEVES_JAILED is set.
const struct MapEvent *Scene_GetEvents(void) {
  switch (gSaveBlock.warpDest) {
  case LOG_ROOMS_A_LOFT_FROM_CRATE_ROOM:
  case LOG_ROOMS_A_LOFT_AFTER_LOSING_TO_THIEVES:
  case LOG_ROOMS_A_LOFT_AFTER_THIEVES_DEFEATED:
    return gLogRoomsA_LoftEvents;
  }
  if (GameFlag_Get(FLAG_VAULT_THIEVES_JAILED) != 0) {
    return gLogRoomsA_EventsAfterThieves;
  }
  return gLogRoomsA_Events;
}

/// Stands the speaker, faces the leader, and shows the selected message.
static void LogRoomsA_StandAndShowMessage(s32 x) {
  Import_ObjectEventSetAnimation(x, ANIM_STAND);
  LogRoomsA_FaceEachOtherAndWait(x, 0, 2);
  FieldScript_ShowMessageAndWait(x, 0);
}

#include "common/vault_talk.h"

/// Faces the leader and selects the next reply when the answer is no.
static void LogRoomsA_AskYesNoAndShowReply(s32 x) {
  LogRoomsA_FaceEachOtherAndWait(x, 0, 2);
  FieldMessage_ShowAtObject(x, 0);
  if (FieldScript_AskYesNoWithObjectEventAnimation(0, 0) != 0) {
    gFieldWork->message++;
  }
  FieldScript_ShowMessageAndWait(x, 0);
}

/// In a house of Vault, the small boy asks about travel.
/// "Hey, mister! Is it fun to see strange new places?"
///
/// Reached by talking to the boy (LOCALID_BOY_ASKING_ABOUT_TRAVEL) before the
/// thieves are jailed.
static void EventScript_BoyAsksAboutTravel(void) {
  FieldScript_Start();
  FieldScript_SetMessageId(MsgVaultMisterFunSeeStrangeNew);
  Import_ObjectEventSetAnimation(LOCALID_BOY_ASKING_ABOUT_TRAVEL, ANIM_STAND);
  LogRoomsA_AskYesNoAndShowReply(LOCALID_BOY_ASKING_ABOUT_TRAVEL);
  FieldScript_End();
}

/// At the mayor's in Vault, a little girl offers to tell Ivan's secret.
/// "Ivan has great powers. Wouldn't you like to know what they are?"
///
/// Reached by talking to LOCALID_GIRL_TELLING_A_SECRET until
/// FLAG_VAULT_IVAN_JOINED_ROD_SEARCH is set; then the row after it tells the
/// secret: Ivan can talk with animals.
static void EventScript_OfferIvansSecret(void) {
  FieldScript_Start();
  FieldScript_SetMessageId(MsgVaultIvanHasGreatPowersWouldnt);
  LogRoomsA_AskYesNoAndShowReply(LOCALID_GIRL_TELLING_A_SECRET);
  ObjectEvent_FaceDirection(LOCALID_GIRL_TELLING_A_SECRET, FACING_WEST, 0);
  FieldScript_End();
}

/// In Vault's item shop, across the counter the shop opens; otherwise a
/// greeting, or, after the thieves are jailed, the bridge is out.
/// "Did you just arrive in town today?"
///
/// Reached by talking to the item merchant (LOCALID_ITEM_MERCHANT).
static void EventScript_TalkToItemMerchant(void) {
  s32 leaderFacing = ObjectEvent_GetById(LOCALID_PARTY_LEADER)->facing;

  FieldScript_Start();
  if (leaderFacing > FACING_NORTHWEST && leaderFacing < FACING_NORTHEAST) {
    Shop_Run(6, LOCALID_ITEM_MERCHANT);
  } else if (GameFlag_Get(FLAG_VAULT_THIEVES_JAILED) == 0) {
    FieldScript_SetMessageId(MsgVaultDidJustArriveInTown);
    LogRoomsA_AskYesNoAndShowReply(LOCALID_ITEM_MERCHANT);
  } else {
    FieldScript_SetMessageId(MsgVaultWithBridgeOutWillQuite);
    FieldScript_ShowMessageAndWait(LOCALID_ITEM_MERCHANT, 0);
  }
  FieldScript_End();
}

/// The wife of the man with the sparkling rock talks about their rocks.
/// She wishes their rocks were gems, or, after the thieves are jailed, is glad
/// they were caught before stealing the gem.
///
/// Reached by talking to the gem owner's wife (LOCALID_GEM_OWNERS_WIFE).
static void EventScript_TalkAboutRocks(void) {
  FieldScript_Start();
  if (GameFlag_Get(FLAG_VAULT_THIEVES_JAILED) == 0) {
    FieldScript_SetMessageId(MsgVaultIfOnlyTheseRocksWere);
  } else {
    FieldScript_SetMessageId(MsgVaultThankGoodnessThoseThievesWere);
  }
  LogRoomsA_StandAndShowMessage(LOCALID_GEM_OWNERS_WIFE);
  FieldScript_End();
}

/// Grandpa in the item shop talks about missing things.
/// After the thieves are jailed, he says instead that they hid the treasure in
/// the loft and more in the cemetery.
/// "Is it just me, or am I missing something?"
///
/// Reached by talking to Grandpa (LOCALID_GRANDPA).
static void EventScript_TalkAboutMissingThings(void) {
  FieldScript_Start();
  if (GameFlag_Get(FLAG_VAULT_THIEVES_JAILED) != 0) {
    FieldScript_SetMessageId(MsgVaultThievesHidStolenTreasureIn);
  } else {
    FieldScript_SetMessageId(MsgVaultJustMeOrAmMissing);
  }
  LogRoomsA_StandAndShowMessage(LOCALID_GRANDPA);
  FieldScript_End();
}

/// At his house in Vault, the mayor asks for someone to help Ivan; once the
/// party has agreed, he thanks them the first time and asks them to look after
/// Ivan after that.
///
/// Reached by talking to the mayor (LOCALID_MAYOR) before the thieves are
/// jailed. Sets FLAG_VAULT_MAYOR_THANKED_PARTY with the thanks.
static void EventScript_MayorAsksHelpForIvan(void) {
  s32 thankedForHelpingFlag;
  thankedForHelpingFlag = FLAG_VAULT_MAYOR_THANKED_PARTY;
  FieldScript_Start();
  if (GameFlag_Get(FLAG_VAULT_IVAN_JOINED_ROD_SEARCH) != 0) {
    if (GameFlag_Get(thankedForHelpingFlag) == 0) {
      FieldScript_SetMessageId(MsgVaultYoureGoingHelpIvan);
      LogRoomsA_StandAndShowMessage(LOCALID_MAYOR);
      FieldScript_WaitFrames(10);
      LogRoomsA_PlayAnimationAndWait(LOCALID_MAYOR, ANIM_NOD, 20);
      GameFlag_Set(thankedForHelpingFlag);
    } else {
      FieldScript_SetMessageId(MsgVaultPleaseLookAfterIvan);
    }
  } else {
    FieldScript_SetMessageId(MsgVaultCouldSomeonePleaseHelpIvan);
  }
  LogRoomsA_StandAndShowMessage(LOCALID_MAYOR);
  FieldScript_End();
}

/// At Vault's inn, the girl at the inn says the three strangers have fooled
/// the innkeeper and his wife.
///
/// Reached by talking to the girl at the inn (LOCALID_GIRL_AT_THE_INN) before
/// the thieves are jailed.
static void EventScript_WarnAboutThreeStrangers(void) {
  FieldScript_Start();
  FieldScript_SetMessageId(MsgVaultThoseThreeStrangersSureHave);
  LogRoomsA_StandAndShowMessage(LOCALID_GIRL_AT_THE_INN);
  FieldScript_End();
}

/// The suspected leader cries out and runs to his next spot.
/// This is in the guests' room at Vault's inn; the spot is chosen by the way he
/// faces and where he is on his route.
/// "Ow! Stop it!"
///
/// Reached by Mind Read on him (LOCALID_SUSPECTED_LEADER), and by talking to
/// him once FLAG_VAULT_SUSPECTS_MIND_READ is set (before, he only says Ivan
/// creeps him out); neither while the party corners him. His timer holds his
/// place on the six-step route, wrapping from 5 to 0.
static void EventScript_SuspectedLeaderDodges(void) {
  struct ObjectEvent *suspect;
  s16 *routeState;
  s32 routeIndex;
  s32 nextRoute;

  suspect = ObjectEvent_GetById(LOCALID_SUSPECTED_LEADER);
  FieldScript_Start();
  ObjectMotion_SetVarCbAndRefresh(LOCALID_SUSPECTED_LEADER, 2);
  FieldScript_SetMessageId(MsgVaultOwStop);
  FieldScript_ShowMessageAndWait(LOCALID_SUSPECTED_LEADER, 0);
  ObjectMotion_SetSpeedParameters(LOCALID_SUSPECTED_LEADER, PIXELS(4),
                                  PIXELS(2));
  if ((u32)((suspect->facing & 0xf000) - 0x5000) <= 0x6000) {
    routeState = (s16 *)&suspect->timer;
    routeIndex = *routeState;
    if (routeIndex <= 2) {
      ObjectEvent_SetActionScript(LOCALID_SUSPECTED_LEADER,
                                  gLogRoomsA_StepActions[routeIndex]);
      *(u16 *)routeState = *(u16 *)routeState + 1;
    } else {
      ObjectEvent_SetActionScript(LOCALID_SUSPECTED_LEADER,
                                  gLogRoomsA_IdleActions[routeIndex]);
      nextRoute = *(u16 *)routeState - 1;
      *(u16 *)routeState = nextRoute;
    }
  } else {
    routeState = (s16 *)&suspect->timer;
    routeIndex = *routeState;
    if (routeIndex > 2) {
      ObjectEvent_SetActionScript(LOCALID_SUSPECTED_LEADER,
                                  gLogRoomsA_StepActions[routeIndex]);
      *(u16 *)routeState = *(u16 *)routeState + 1;
    } else {
      ObjectEvent_SetActionScript(LOCALID_SUSPECTED_LEADER,
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
  ObjectEvent_RefreshSelectorById(LOCALID_SUSPECTED_LEADER);
  FieldScript_End();
}

/// Selects a response script and advances the second suspect's route state.
static inline void
LogRoomsA_ApplySecondSuspectRoute(struct ObjectEvent *objectEvent,
                                  s32 scriptRow, s32 routePair,
                                  u16 routeDelta) {
  ObjectEvent_SetActionScript(
      LOCALID_SUSPECTED_THIEF,
      gLogRoomsA_ResponseScripts[scriptRow * 4 + objectEvent->timer]);
  objectEvent->timer = objectEvent->timer - routePair * 2 + routeDelta;
}

/// In the guests' room at Vault's inn, the suspected thief cries out and runs
/// to his next spot. "No! Leave me alone!"
///
/// Reached as EventScript_SuspectedLeaderDodges is, on LOCALID_SUSPECTED_THIEF.
/// His timer holds his place on his route (4 at its start).
static void EventScript_SuspectedThiefDodges(void) {
  struct ObjectEvent *objectEvent;
  u32 facing;
  s32 routePair;

  objectEvent = ObjectEvent_GetById(LOCALID_SUSPECTED_THIEF);
  facing = objectEvent->facing & 0xf000;
  routePair = objectEvent->timer >> 1;
  FieldScript_Start();
  ObjectMotion_SetVarCbAndRefresh(LOCALID_SUSPECTED_THIEF, 2);
  FieldScript_SetMessageId(MsgVaultNoLeaveAlone);
  FieldScript_ShowMessageAndWait(LOCALID_SUSPECTED_THIEF, 0);
  ObjectMotion_SetSpeedParameters(LOCALID_SUSPECTED_THIEF, PIXELS(7) / 2,
                                  PIXELS(7) / 4);
  switch (objectEvent->timer) {
  case 4:
    if (facing > FACING_SOUTHEAST && facing < FACING_NORTHWEST) {
      ObjectEvent_SetActionScript(LOCALID_SUSPECTED_THIEF,
                                  gLogRoomsA_SuspectAlongToSouthEastCorner);
      objectEvent->timer = 2;
    } else {
      ObjectEvent_SetActionScript(LOCALID_SUSPECTED_THIEF,
                                  gLogRoomsA_SuspectAlongToSouthWestCorner);
      objectEvent->timer = 3;
    }
    break;
  case 0:
  case 2:
    if (facing > FACING_SOUTHEAST && facing < FACING_NORTHWEST) {
      LogRoomsA_ApplySecondSuspectRoute(objectEvent, routePair, routePair, 1);
    } else {
      LogRoomsA_ApplySecondSuspectRoute(objectEvent, routePair ^ 1, routePair,
                                        -1);
    }
    break;
  case 1:
  case 3:
    if (facing > FACING_SOUTHWEST && facing < FACING_NORTHEAST) {
      LogRoomsA_ApplySecondSuspectRoute(objectEvent, routePair, routePair, 1);
    } else {
      LogRoomsA_ApplySecondSuspectRoute(objectEvent, routePair ^ 1, routePair,
                                        -1);
    }
    break;
  }
  objectEvent->timer &= 3;
  ObjectEvent_RefreshSelectorById(LOCALID_SUSPECTED_THIEF);
  FieldScript_End();
}

/// The villager suspecting the neighbor asks whether the thieves came from
/// Lunpa.
/// This is in a house of Vault after the thieves are jailed. "Did those thieves
/// come from Lunpa?"
///
/// Reached by talking to the villager suspecting the neighbor
/// (LOCALID_VILLAGER_SUSPECTING_THE_NEIGHBOR) after the thieves are jailed; the
/// boy and LOCALID_BOY_WISHING_TO_BE_OLDER below are reached the same way.
static void EventScript_AskWhetherThievesCameFromLunpa(void) {
  FieldScript_Start();
  FieldScript_SetMessageId(MsgVaultDidThoseThievesComeFrom);
  LogRoomsA_AskYesNoAndShowReply(LOCALID_VILLAGER_SUSPECTING_THE_NEIGHBOR);
  FieldScript_End();
}

/// The boy says Father worries about the prisoners from Lunpa.
/// He's in a house of Vault, and says it after the thieves are jailed.
static void EventScript_BoyWorriesAboutFather(void) {
  FieldScript_Start();
  FieldScript_SetMessageId(MsgVaultMyFatherWorriedAboutThose);
  LogRoomsA_StandAndShowMessage(LOCALID_BOY_ASKING_ABOUT_TRAVEL);
  FieldScript_End();
}

/// At the mayor's in Vault after the thieves are jailed, a villager asks about
/// defeating them. "I heard you defeated those thieves!"
static void EventScript_AskAboutDefeatingThieves(void) {
  FieldScript_Start();
  FieldScript_SetMessageId(MsgVaultHeardDefeatedThoseThieves);
  LogRoomsA_AskYesNoAndShowReply(LOCALID_BOY_WISHING_TO_BE_OLDER);
  FieldScript_End();
}

/// The mayor thanks Isaac and gives the promised Water of Life.
/// This is at his house after the thieves are jailed, and he gives it if there
/// is room; then he asks whether the party is heading beyond the Goma Range,
/// and points them to the cave on the way to Bilibin.
///
/// Reached by talking to the mayor (LOCALID_MAYOR) after the thieves are
/// jailed. Gives the Water of Life and sets FLAG_VAULT_WATER_OF_LIFE_RECEIVED,
/// once; with a full bag he says to come back for it.
static void EventScript_MayorGivesWaterOfLife(void) {
  FieldScript_Start();
  if (Script_GetFlag(FLAG_VAULT_WATER_OF_LIFE_RECEIVED) == 0) {
    FieldScript_SetMessageId(MsgVaultYouWereSuchGreatHelp);
    LogRoomsA_ShowMessageAndWait(LOCALID_MAYOR, 20);
    LogRoomsA_PlayAnimationAndWait(LOCALID_MAYOR, ANIM_NOD, 20);
    LogRoomsA_ShowMessageAndWait(LOCALID_MAYOR, 30);
    ObjectEvent_FaceDirection(LOCALID_MAYOR, FACING_EAST, 0);
    FieldScript_WaitFrames(30);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_MAYOR, 2);
    FieldScript_WaitFrames(30);
    LogRoomsA_FaceEachOtherAndWait(LOCALID_PARTY_LEADER, LOCALID_MAYOR, 20);
    LogRoomsA_PlayAnimationAndWait(LOCALID_MAYOR, ANIM_NOD, 20);
    gFieldWork->message++;
    if (PartyInventory_HasSpace() == 0) {
      FieldScript_SetMessageId(MsgVaultYouWereSuchGreatHelp + 3);
      LogRoomsA_ShowMessageAndWait(LOCALID_MAYOR, 20);
      FieldScript_End();
      return;
    }
    GameFlag_Set(FLAG_VAULT_WATER_OF_LIFE_RECEIVED);
    PartyInventory_GiveItem(ITEM_WATER_OF_LIFE, 0);
  }
  FieldScript_SetMessageId(MsgVaultHeadingOutBeyondGomaRange);
  Script_OpenMessage(LOCALID_MAYOR, 0);
  FieldScript_WaitFrames(20);
  if (FieldScript_AskYesNoWithObjectEventAnimation(0, 0) != 0) {
    gFieldWork->message++;
  }
  Script_ShowMessage(LOCALID_MAYOR, 0);
  FieldScript_End();
}

/// At Vault's inn after the thieves are jailed, the girl at the inn offers
/// leftover bones from dinner; a yes gets a Bone if there is room, a no a nod.
///
/// Reached by talking to the girl at the inn (LOCALID_GIRL_AT_THE_INN) after
/// the thieves are jailed, while the leader is not beside the Bone on the floor
/// (FLAG_VAULT_LEADER_BESIDE_BONE clear). Gives a Bone and sets
/// FLAG_VAULT_BONE_RECEIVED.
static void EventScript_OfferBones(void) {
  FieldScript_Start();
  LogRoomsA_FaceEachOtherAndWait(LOCALID_GIRL_AT_THE_INN, LOCALID_PARTY_LEADER,
                                 2);
  if (GameFlag_Get(FLAG_VAULT_BONE_RECEIVED) == 0) {
    FieldScript_SetMessageId(MsgVaultHaveLotLeftoverBonesFrom);
    Script_OpenMessage(LOCALID_GIRL_AT_THE_INN, 0);
  } else {
    FieldScript_SetMessageId(MsgVaultWantMoreBones);
    Script_OpenMessage(LOCALID_GIRL_AT_THE_INN, 0);
  }
  if (FieldScript_AskYesNoWithObjectEventAnimation(0, 0) == 0) {
    FieldScript_WaitFrames(20);
    Script_ShowMessage(LOCALID_GIRL_AT_THE_INN, 0);
    FieldScript_WaitFrames(20);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_GIRL_AT_THE_INN, 2);
    FieldScript_WaitFrames(20);
    if (PartyInventory_HasSpace() == 0) {
      ObjectMotion_SetAnimationAndWait(LOCALID_GIRL_AT_THE_INN,
                                       ANIM_SHAKE_HEAD);
      FieldScript_WaitFrames(20);
      FieldScript_SetMessageId(MsgVaultWowHaveManyThingsArent);
      Script_ShowMessage(LOCALID_GIRL_AT_THE_INN, 0);
    } else {
      ItemDisplay_ShowItem(ITEM_BONE, 3);
      PartyInventory_GiveItem(ITEM_BONE, 0);
      GameFlag_Set(FLAG_VAULT_BONE_RECEIVED);
    }
  } else {
    gFieldWork->message++;
    FieldScript_WaitFrames(20);
    ObjectMotion_SetAnimationAndWait(LOCALID_GIRL_AT_THE_INN, ANIM_NOD);
    FieldScript_WaitFrames(20);
    Script_ShowMessage(LOCALID_GIRL_AT_THE_INN, 0);
  }
  ObjectEvent_FaceDirection(LOCALID_GIRL_AT_THE_INN, FACING_SOUTH, 0);
  FieldScript_End();
}

/// In a house of Vault, Mind Read on the wife of the man with the sparkling
/// rock: let me see that rock, or maybe it is worthless.
///
/// Reached by Mind Read on the gem owner's wife (LOCALID_GEM_OWNERS_WIFE); the
/// Mind Read events below are reached the same way on the object event each
/// names, before and after the thieves are jailed (FLAG_VAULT_THIEVES_JAILED
/// picks the thought).
static void EventScript_ReadMindOnRock(void) {
  FieldScript_Start();
  if (GameFlag_Get(FLAG_VAULT_THIEVES_JAILED) == 0) {
    FieldScript_SetMessageId(MsgVaultOkMisterLetMeSee);
  } else {
    FieldScript_SetMessageId(MsgVaultIfRockWorthlessMaybeThats);
  }
  Vault_ShowThought(LOCALID_GEM_OWNERS_WIFE);
  FieldScript_End();
}

/// In a house of Vault, Mind Read on the boy: what is the outside world
/// like, or Father looks sad.
static void EventScript_ReadBoysMind(void) {
  FieldScript_Start();
  if (GameFlag_Get(FLAG_VAULT_THIEVES_JAILED) == 0) {
    FieldScript_SetMessageId(MsgVaultWonderOutsideWorldLike);
  } else {
    FieldScript_SetMessageId(MsgVaultFatherLooksSadWorryingLike);
  }
  Vault_ShowThought(LOCALID_BOY_ASKING_ABOUT_TRAVEL);
  FieldScript_End();
}

/// In Vault's item shop, Mind Read on Grandpa: could they be thieves? Who can
/// be trusted? Or nothing in the house was worth stealing.
static void EventScript_ReadMindOnThieves(void) {
  FieldScript_Start();
  if (GameFlag_Get(FLAG_VAULT_THIEVES_JAILED) == 0) {
    FieldScript_SetMessageId(MsgVaultCouldTheyThievesGoodDont);
  } else {
    FieldScript_SetMessageId(MsgVaultGuessNothingInOurHouse);
  }
  Vault_ShowThought(LOCALID_GRANDPA);
  FieldScript_End();
}

/// At the mayor's in Vault, Mind Read on the mayor: a laugh, or the cave in
/// the Goma Range is dangerous, but the party will be okay. "That tickles..."
static void EventScript_ReadMayorMind(void) {
  FieldScript_Start();
  if (GameFlag_Get(FLAG_VAULT_THIEVES_JAILED) == 0) {
    FieldScript_SetMessageId(MsgVaultTicklesBeingTickledByBoy);
  } else {
    FieldScript_SetMessageId(MsgVaultCaveInGomaRangeDangerous);
  }
  Vault_ShowThought(LOCALID_MAYOR);
  FieldScript_End();
}

/// At Vault's inn, Mind Read on the girl at the inn: the master and his wife
/// are blinded by money; after the thieves are jailed, hungry while cooking,
/// or, once a Bone was given, wondering about calcium.
static void EventScript_ReadBoneGiverMind(void) {
  FieldScript_Start();
  if (GameFlag_Get(FLAG_VAULT_THIEVES_JAILED) == 0) {
    FieldScript_SetMessageId(MsgVaultMasterHisWifeBlindedBy);
  } else if (GameFlag_Get(FLAG_VAULT_BONE_RECEIVED) == 0) {
    FieldScript_SetMessageId(MsgVaultGeeAlwaysGetHungryWhen);
  } else {
    FieldScript_SetMessageId(MsgVaultHeReallyLikesBonesWonder);
  }
  Vault_ShowThought(LOCALID_GIRL_AT_THE_INN);
  FieldScript_End();
}

/// At the mayor's in Vault, talking to Ivan: the first time, he reads Isaac's
/// mind, admits his strange powers and learns they are called Psynergy; then
/// he asks for help finding the rod. A yes has him join the party.
///
/// Reached by talking to Ivan, whose row takes his party slot (IVAN), except
/// while the party corners the suspects. Sets FLAG_VAULT_MET_IVAN the first
/// time, and on a yes plays LogRoomsA_IvanJoinsRodSearch, sets
/// FLAG_VAULT_IVAN_JOINED_ROD_SEARCH and has Ivan walk into the leader.
static void EventScript_MeetIvan(void) {

  struct ObjectEvent *objectEvent;

  FieldScript_Start();
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_SetSpeed(LOCALID_GARET, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_SetSpeed(LOCALID_IVAN, PIXELS(4) / 5, PIXELS(2) / 5);
  // Isaac steps up in front of Ivan and Garet comes out beside him.
  Script_PlaySong(MUS_SILENCE);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 384, 408);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  ObjectEvent_SetPosition(LOCALID_GARET, PIXELS(384), PIXELS(408));
  ObjectEvent_WalkToAndWait(LOCALID_GARET, 368, 408);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTH + FACING_STEP, 20);
  // Introduce Ivan's ability only on the first meeting.
  if (Script_GetFlag(FLAG_VAULT_MET_IVAN) == 0) {
    GameFlag_Set(FLAG_VAULT_MET_IVAN);
    // Ivan reads Isaac's mind and admits his strange powers.
    LogRoomsA_StartMindRead(LOCALID_IVAN, LOCALID_PARTY_LEADER);
    FieldScript_WaitFrames(40);
    LogRoomsA_EndMindRead();
    FieldScript_SetMessageId(MsgVaultDoPossessStrangePowers);
    Script_PlaySong(MUS_IVAN);
    FieldScript_WaitFrames(30);
    LogRoomsA_PlayAnimationAndWait(LOCALID_IVAN, ANIM_NOD, 30);
    LogRoomsA_ShowMessageAndWait(LOCALID_IVAN, 30);
    // The two start; he reads again: "You have them, too, don't you?"
    ObjectMotion_SetVariantCallback(LOCALID_PARTY_LEADER, 1);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 1);
    FieldScript_WaitFrames(20);
    LogRoomsA_StartMindRead(LOCALID_IVAN, LOCALID_PARTY_LEADER);
    FieldScript_WaitFrames(40);
    LogRoomsA_EndMindRead();
    LogRoomsA_ShowMessageAndWait(LOCALID_IVAN, 30);
    LogRoomsA_FaceEachOtherAndWait(LOCALID_PARTY_LEADER, LOCALID_GARET, 50);
    ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
    ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTH + FACING_STEP, 0);
    FieldScript_WaitFrames(20);
    // A third reading: his power is called Psynergy; then they have so much
    // in common he can trust them.
    LogRoomsA_StartMindRead(LOCALID_IVAN, LOCALID_PARTY_LEADER);
    FieldScript_WaitFrames(40);
    LogRoomsA_EndMindRead();
    ObjectMotion_SetVarCbAndRefresh(LOCALID_IVAN, 1);
    LogRoomsA_ShowMessageAndWait(LOCALID_IVAN, 50);
    Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_NOD);
    LogRoomsA_PlayAnimationAndWait(LOCALID_GARET, ANIM_NOD, 20);
    LogRoomsA_PlayAnimationAndWait(LOCALID_IVAN, ANIM_NOD, 20);
    LogRoomsA_ShowMessageAndWait(LOCALID_IVAN, 40);
    // He walks off a step, turns back and tells of Hammet's stolen rod.
    ObjectMotion_SetVarCbAndRefresh(LOCALID_IVAN, 1);
    FieldScript_WaitFrames(30);
    ObjectEvent_FaceDirection(LOCALID_IVAN, FACING_NORTH, 0);
    FieldScript_WaitFrames(30);
    ObjectEvent_WalkToAndWait(LOCALID_IVAN, 376, 376);
    FieldScript_WaitFrames(40);
    LogRoomsA_FaceEachOtherAndWait(LOCALID_PARTY_LEADER, LOCALID_GARET, 50);
    ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
    ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTH + FACING_STEP, 0);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_IVAN, 1);
    FieldScript_WaitFrames(50);
    LogRoomsA_PlayAnimationAndWait(LOCALID_IVAN, ANIM_NOD, 30);
    ObjectEvent_FaceDirection(LOCALID_IVAN, FACING_SOUTH, 0);
    FieldScript_WaitFrames(10);
    Script_ShowMessage(LOCALID_IVAN, 0);
    // "Would you please help me get my master's rod back?"
    Script_OpenMessage(LOCALID_IVAN, 0);
  } else {
    // Asked again: "Would you really? Would you help me find the rod?"
    Script_PlaySong(MUS_IVAN);
    FieldScript_SetMessageId(MsgVaultWouldReallyWouldHelpMe);
    Script_OpenMessage(LOCALID_IVAN, 0);
  }
  // Accepting the search lets Ivan merge into the leader's position; a no gets
  // his apology for asking.
  if (FieldScript_AskYesNoWithObjectEventAnimation(0, 0) == 0) {
    LogRoomsA_IvanJoinsRodSearch();
    GameFlag_Set(FLAG_VAULT_IVAN_JOINED_ROD_SEARCH);
    Import_ObjectEventSetAnimation(LOCALID_IVAN, ANIM_WALK);
    objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
    if (objectEvent != 0) {
      ObjectEvent_SetDestination(LOCALID_IVAN, objectEvent->x.part.pixel,
                                 objectEvent->z.part.pixel);
    }
    ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_IVAN);
    ObjectEvent_SetPosition(LOCALID_IVAN, 0, 0);
  } else {
    Script_ShowMessage(LOCALID_IVAN, 0);
  }
  // Garet walks back into the leader and the earlier music returns.
  Import_ObjectEventSetAnimation(LOCALID_GARET, ANIM_WALK);
  objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (objectEvent != 0) {
    ObjectEvent_SetDestination(LOCALID_GARET, objectEvent->x.part.pixel,
                               objectEvent->z.part.pixel);
  }
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_GARET);
  ObjectEvent_SetPosition(LOCALID_GARET, 0, 0);
  Scene_PlayMusic();
  FieldScript_End();
}

/// Ivan thanks them, they exchange names, and he shows Isaac and Garet Mind
/// Read; he joins the party.
///
/// Called by EventScript_MeetIvan when the party agrees to help. Adds Ivan to
/// the party ("Ivan joined your party!").
static void LogRoomsA_IvanJoinsRodSearch(void) {
  s32 message;

  // Ivan jumps for joy: "Thank you. With your help, I believe we can recover
  // the rod." Then he asks their names.
  FieldScript_WaitFrames(20);
  ObjectEvent_GetById(LOCALID_IVAN)->paused = 0;
  ObjectMotion_Launch(LOCALID_IVAN, 4, 0);
  FieldScript_WaitFrames(40);
  message = MsgVaultThankHelpBelieve;
  FieldScript_SetMessageId(message);
  FieldScript_WaitFrames(20);
  LogRoomsA_ShowMessageAndWait(LOCALID_IVAN, 20);
  ObjectEvent_ShowEmote(LOCALID_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  FieldScript_WaitFrames(50);
  LogRoomsA_ShowMessageAndWait(LOCALID_IVAN, 30);
  // He reads their names from Isaac's mind; Isaac backs away from him.
  ObjectEvent_WalkToAndWait(LOCALID_IVAN, 376, 392);
  LogRoomsA_StartMindRead(LOCALID_IVAN, LOCALID_PARTY_LEADER);
  FieldScript_WaitFrames(40);
  LogRoomsA_EndMindRead();
  ObjectMotion_SetVarCbAndRefresh(LOCALID_PARTY_LEADER, 1);
  ObjectEvent_GetById(LOCALID_PARTY_LEADER)->steeringFlags &=
      ~OBJ_EVENT_FACES_MOTION;
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 384, 424);
  FieldScript_WaitFrames(1);
  ObjectEvent_GetById(LOCALID_PARTY_LEADER)->steeringFlags |=
      OBJ_EVENT_FACES_MOTION;
  FieldScript_WaitFrames(30);
  FieldScript_ShowMessageAndWait(LOCALID_IVAN, 0);
  // Demonstrate Mind Read and let Garet and Isaac react to Ivan's explanation.
  ObjectEvent_SetAttachedEffect(LOCALID_PARTY_LEADER, ATTACHED_EFFECT_IN_FRONT |
                                                          ATTACHED_EFFECT_ONCE);
  ObjectEvent_SetAttachedEffect(LOCALID_GARET, ATTACHED_EFFECT_IN_FRONT |
                                                   ATTACHED_EFFECT_ONCE);
  FieldScript_WaitFrames(60);
  Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_NOD);
  LogRoomsA_PlayAnimationAndWait(LOCALID_GARET, ANIM_NOD, 30);
  ObjectEvent_ShowEmote(LOCALID_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  FieldScript_WaitFrames(60);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_IVAN, 1);
  FieldScript_WaitFrames(10);
  LogRoomsA_PlayAnimationAndWait(LOCALID_IVAN, ANIM_NOD, 20);
  FieldScript_ShowMessageAndWait(LOCALID_IVAN, 0);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 2);
  FieldScript_WaitFrames(10);
  LogRoomsA_FaceObjectEventAndWait(LOCALID_GARET, LOCALID_PARTY_LEADER, 30);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_PARTY_LEADER, 2);
  FieldScript_WaitFrames(10);
  LogRoomsA_FaceObjectEventAndWait(LOCALID_PARTY_LEADER, LOCALID_GARET, 40);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  FieldScript_WaitFrames(60);
  // "Is my Mind Read so frightening?": either answer goes on the same way.
  ObjectEvent_ShowEmote(LOCALID_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  FieldScript_WaitFrames(60);
  FieldMessage_ShowAtObject(LOCALID_IVAN, 0);
  ObjectMotion_SetAngleToward(LOCALID_PARTY_LEADER, LOCALID_IVAN, 0);
  ObjectMotion_SetAngleToward(LOCALID_GARET, LOCALID_IVAN, 0);
  FieldScript_AskYesNoWithObjectEventAnimation(0, 0);
  FieldScript_WaitFrames(30);
  // Ivan and Isaac clasp hands, and Garet cries out that his mind is read.
  LogRoomsA_PlayAnimationAndWait(LOCALID_IVAN, ANIM_NOD, 10);
  ObjectEvent_WalkToAndWait(LOCALID_IVAN, 384, 408);
  FieldScript_WaitFrames(10);
  FieldMessage_Show(message + 5, 1);
  FieldScript_SetMessageId(message + 6);
  ObjectEvent_LinkPair(LOCALID_IVAN, LOCALID_GARET, 0);
  LogRoomsA_FaceObjectEventAndWait(LOCALID_PARTY_LEADER, LOCALID_GARET, 20);
  ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  FieldScript_WaitFrames(60);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 1);
  ObjectEvent_GetById(LOCALID_GARET)->steeringFlags &= ~OBJ_EVENT_FACES_MOTION;
  ObjectEvent_WalkToAndWait(LOCALID_GARET, 352, 408);
  FieldScript_WaitFrames(1);
  ObjectEvent_GetById(LOCALID_GARET)->steeringFlags |= OBJ_EVENT_FACES_MOTION;
  FieldScript_WaitFrames(10);
  // Garet backs off; Ivan asks whether Isaac could read Garet's mind, and the
  // two read it together. "Just as I thought."
  ObjectEvent_WalkTo(LOCALID_IVAN, 368, 408);
  FieldScript_WaitFrames(20);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 368, 424);
  ObjectMotion_SetAngleToward(LOCALID_PARTY_LEADER, LOCALID_GARET, 0);
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_IVAN);
  LogRoomsA_StartMindRead(LOCALID_IVAN, LOCALID_GARET);
  LogRoomsA_PlayAnimationAndWait(LOCALID_GARET, ANIM_SHAKE_HEAD, 10);
  FieldScript_ShowMessageAndWait(LOCALID_GARET, 0);
  LogRoomsA_EndMindRead();
  LogRoomsA_FaceEachOtherAndWait(LOCALID_IVAN, LOCALID_PARTY_LEADER, 30);
  FieldScript_ShowMessageAndWait(LOCALID_IVAN, 0);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_PARTY_LEADER, 1);
  FieldScript_WaitFrames(30);
  LogRoomsA_PlayAnimationAndWait(LOCALID_PARTY_LEADER, ANIM_NOD, 30);
  LogRoomsA_PlayAnimationAndWait(LOCALID_IVAN, ANIM_NOD, 10);
  FieldScript_ShowMessageAndWait(LOCALID_IVAN, 0);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 2);
  ObjectMotion_SetAngleToward(LOCALID_GARET, LOCALID_PARTY_LEADER, 0);
  FieldScript_ShowMessageAndWait(LOCALID_GARET, 0);
  ObjectMotion_SetAngleToward(LOCALID_PARTY_LEADER, LOCALID_GARET, 0);
  LogRoomsA_PlayAnimationAndWait(LOCALID_PARTY_LEADER, ANIM_NOD, 40);
  ObjectMotion_SetAngleToward(LOCALID_PARTY_LEADER, LOCALID_IVAN, 0);
  LogRoomsA_FaceObjectEventAndWait(LOCALID_GARET, LOCALID_IVAN, 30);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION,
                        0);
  ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  FieldScript_WaitFrames(60);
  // Sharing their Psynergy let Isaac read minds too; Garet protests that it
  // isn't fair, and Ivan agrees it is one-way.
  LogRoomsA_PlayAnimationAndWait(LOCALID_IVAN, ANIM_NOD, 20);
  FieldScript_ShowMessageAndWait(LOCALID_IVAN, 0);
  FieldEffect_SpawnLinked(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
  FieldScript_WaitFrames(60);
  LogRoomsA_FaceObjectEventAndWait(LOCALID_GARET, LOCALID_PARTY_LEADER, 10);
  FieldScript_ShowMessageAndWait(LOCALID_GARET, 0);
  FieldEffect_SpawnLinked(LOCALID_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  FieldScript_WaitFrames(60);
  FieldScript_ShowMessageAndWait(LOCALID_IVAN, 0);
  ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  FieldScript_WaitFrames(60);
  // Ivan and Garet clasp hands, and Garet reads Isaac's mind.
  FieldMessage_Show(message + 13, 1);
  FieldScript_SetMessageId(message + 14);
  ObjectEvent_LinkPair(LOCALID_IVAN, LOCALID_PARTY_LEADER, 0);
  ObjectMotion_SetAngleToward(LOCALID_GARET, LOCALID_PARTY_LEADER, 0);
  FieldEffect_SpawnLinked(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE,
                          0);
  FieldScript_WaitFrames(60);
  LogRoomsA_StartMindRead(LOCALID_IVAN, LOCALID_PARTY_LEADER);
  FieldEffect_RestoreMindReadPalette();
  LogRoomsA_ShowMessageAndWait(LOCALID_GARET, 30);
  LogRoomsA_EndMindRead();
  ObjectEvent_LinkPair(LOCALID_IVAN, LOCALID_GARET, 0);
  LogRoomsA_PlayAnimationAndWait(LOCALID_IVAN, ANIM_NOD, 10);
  LogRoomsA_ShowMessageAndWait(LOCALID_IVAN, 20);
  ObjectEvent_LinkPair(LOCALID_IVAN, LOCALID_PARTY_LEADER, 0);
  LogRoomsA_PlayAnimationAndWait(LOCALID_PARTY_LEADER, ANIM_NOD, 20);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 1);
  FieldScript_WaitFrames(20);
  // "Let's use Mind Read to find the thieves!" Garet worries people will
  // notice.
  LogRoomsA_PlayAnimationAndWait(LOCALID_GARET, ANIM_SHAKE_HEAD, 10);
  LogRoomsA_ShowMessageAndWait(LOCALID_GARET, 30);
  ObjectMotion_SetAngleToward(LOCALID_IVAN, LOCALID_GARET, 0);
  ObjectEvent_ShowEmote(LOCALID_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  FieldScript_WaitFrames(30);
  LogRoomsA_FaceEachOtherAndWait(LOCALID_PARTY_LEADER, LOCALID_GARET, 10);
  Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_NOD);
  LogRoomsA_PlayAnimationAndWait(LOCALID_GARET, ANIM_NOD, 30);
  ObjectMotion_SetAngleToward(LOCALID_PARTY_LEADER, LOCALID_IVAN, 0);
  ObjectMotion_SetAngleToward(LOCALID_GARET, LOCALID_IVAN, 0);
  ObjectMotion_SetAngleToward(LOCALID_IVAN, LOCALID_PARTY_LEADER, 0);
  // Only Adepts can tell when Ivan uses Psynergy; ordinary people see
  // nothing, and he promises not to read their minds again.
  FieldEffect_SpawnLinked(LOCALID_IVAN, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 0);
  FieldScript_WaitFrames(60);
  LogRoomsA_ShowMessageAndWait(LOCALID_IVAN, 10);
  Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_NOD);
  LogRoomsA_PlayAnimationAndWait(LOCALID_GARET, ANIM_NOD, 20);
  LogRoomsA_PlayAnimationAndWait(LOCALID_IVAN, ANIM_SHAKE_HEAD, 20);
  LogRoomsA_ShowMessageAndWait(LOCALID_IVAN, 20);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION,
                        0);
  FieldEffect_SpawnLinked(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  FieldScript_WaitFrames(60);
  LogRoomsA_PlayAnimationAndWait(LOCALID_IVAN, ANIM_NOD, 10);
  LogRoomsA_ShowMessageAndWait(LOCALID_IVAN, 30);
  FieldEffect_SpawnLinked(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS,
                          0);
  FieldScript_WaitFrames(60);
  LogRoomsA_PlayAnimationAndWait(LOCALID_IVAN, ANIM_SHAKE_HEAD, 10);
  LogRoomsA_ShowMessageAndWait(LOCALID_IVAN, 30);
  LogRoomsA_PlayAnimationAndWait(LOCALID_IVAN, ANIM_NOD, 10);
  Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_NOD);
  LogRoomsA_PlayAnimationAndWait(LOCALID_GARET, ANIM_NOD, 20);
  // "Ivan joined your party!", then he will read the townspeople's minds.
  FieldScript_PrepareObjectAndApplyValue(IVAN, 1);
  FieldScript_SetMessageId(message + 22);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_IVAN, 1);
  FieldScript_ShowMessageAndWait(LOCALID_IVAN, 0);
}

/// In the inn's loft in Vault, Isaac cuts the tied-up villager free; the three
/// thieves arrive, tell of Hammet in Lunpa and of Dodonpa, ask to be let go,
/// and attack.
///
/// Reached by talking to the tied-up villager (LOCALID_TIED_UP_VILLAGER) in the
/// loft; the loft's setup replays the thieves' part through
/// LogRoomsA_ConfrontThievesAgain once FLAG_VAULT_THIEVES_ATTACKED is
/// set. Sets that flag and starts the battle (encounter 12, 5): a win returns
/// to LOG_ROOMS_A_LOFT_AFTER_THIEVES_DEFEATED, a loss to
/// LOG_ROOMS_A_LOFT_AFTER_LOSING_TO_THIEVES.
static void EventScript_FreeRooferAndConfrontThieves(void) {
  s32 base;

  FieldScript_Start();
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_SetSpeed(LOCALID_GARET, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_SetSpeed(LOCALID_IVAN, PIXELS(4) / 5, PIXELS(2) / 5);
  // Isaac cuts the tied-up villager free from the ropes; he gets up and nods.
  ObjectMotion_SetVarCbAndRefresh(LOCALID_PARTY_LEADER, 3);
  FieldScript_WaitFrames(20);
  base = MsgVaultVaultCutFree;
  FieldMessage_Show(base, 1);
  base++;
  FieldScript_SetMessageId(base);
  FieldScript_WaitFrames(30);
  Import_ObjectEventSetAnimation(LOCALID_TIED_UP_VILLAGER, ANIM_STAND);
  FieldScript_WaitFrames(60);
  LogRoomsA_PlayAnimationAndWait(LOCALID_TIED_UP_VILLAGER, ANIM_NOD, 40);
  // Garet and Ivan come out of the leader around the tied-up villager.
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 792, 440);
  LogRoomsA_FaceObjectEventAndWait(LOCALID_PARTY_LEADER,
                                   LOCALID_TIED_UP_VILLAGER, 20);
  ObjectEvent_SetPosition(LOCALID_GARET, PIXELS(792), PIXELS(440));
  ObjectEvent_SetPosition(LOCALID_IVAN, PIXELS(792), PIXELS(440));
  ObjectEvent_WalkTo(LOCALID_GARET, 808, 432);
  ObjectEvent_WalkTo(LOCALID_IVAN, 792, 456);
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_GARET);
  ObjectMotion_SetAngleToward(LOCALID_GARET, LOCALID_TIED_UP_VILLAGER, 0);
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_IVAN);
  LogRoomsA_FaceObjectEventAndWait(LOCALID_IVAN, LOCALID_TIED_UP_VILLAGER, 60);
  // The tied-up villager tells how he came up to fix the roof, was whacked from
  // behind and woke up tied.
  ObjectEvent_ShowEmote(LOCALID_TIED_UP_VILLAGER, EMOTE_IN_FRONT | EMOTE_WINCE,
                        0);
  FieldScript_WaitFrames(60);
  LogRoomsA_ShowMessageAndWait(LOCALID_TIED_UP_VILLAGER, 20);
  Import_ObjectEventSetAnimation(LOCALID_IVAN, ANIM_NOD);
  LogRoomsA_PlayAnimationAndWait(LOCALID_GARET, ANIM_NOD, 30);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_TIED_UP_VILLAGER, 2);
  FieldScript_WaitFrames(20);
  LogRoomsA_ShowMessageAndWait(LOCALID_TIED_UP_VILLAGER, 20);
  ObjectEvent_ShowEmote(LOCALID_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  FieldScript_WaitFrames(60);
  ObjectEvent_FaceDirection(LOCALID_TIED_UP_VILLAGER,
                            FACING_SOUTHEAST + FACING_STEP, 0);
  FieldScript_WaitFrames(10);
  LogRoomsA_ShowMessageAndWait(LOCALID_TIED_UP_VILLAGER, 30);
  // He looks around at chests he doesn't remember; Ivan guesses the stolen
  // goods are in them.
  ObjectEvent_FaceDirection(LOCALID_TIED_UP_VILLAGER,
                            FACING_NORTH + FACING_STEP, 0);
  FieldScript_WaitFrames(30);
  ObjectEvent_ShowEmote(LOCALID_TIED_UP_VILLAGER,
                        EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  FieldScript_WaitFrames(60);
  ObjectEvent_FaceDirection(LOCALID_TIED_UP_VILLAGER,
                            FACING_NORTHWEST + FACING_STEP, 0);
  FieldScript_WaitFrames(40);
  ObjectEvent_FaceDirection(LOCALID_TIED_UP_VILLAGER,
                            FACING_NORTH + FACING_STEP, 0);
  FieldScript_WaitFrames(40);
  LogRoomsA_FaceObjectEventAndWait(LOCALID_TIED_UP_VILLAGER,
                                   LOCALID_PARTY_LEADER, 20);
  LogRoomsA_PlayAnimationAndWait(LOCALID_TIED_UP_VILLAGER, ANIM_SHAKE_HEAD, 30);
  LogRoomsA_ShowMessageAndWait(LOCALID_TIED_UP_VILLAGER, 20);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_IVAN, 1);
  LogRoomsA_ShowMessageAndWait(LOCALID_IVAN, 40);
  // The thieves' leader appears at the ladder to MUS_SATUROS_AND_MENARDI:
  // "Looks like we've been found out!" Everyone turns.
  ObjectEvent_SetPosition(LOCALID_THIEF_LEADER, PIXELS(744), PIXELS(408));
  Sound_PlaySong(MUS_SATUROS_AND_MENARDI);
  LogRoomsA_ShowMessageAndWait(LOCALID_THIEF_LEADER, 20);
  ObjectMotion_SetVariantCallback(LOCALID_PARTY_LEADER, 1);
  ObjectMotion_SetVariantCallback(LOCALID_GARET, 1);
  ObjectMotion_SetVariantCallback(LOCALID_IVAN, 1);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_TIED_UP_VILLAGER, 1);
  FieldScript_WaitFrames(30);
  ObjectMotion_SetAngleToward(LOCALID_PARTY_LEADER, LOCALID_THIEF_LEADER, 0);
  ObjectMotion_SetAngleToward(LOCALID_GARET, LOCALID_THIEF_LEADER, 0);
  ObjectMotion_SetAngleToward(LOCALID_IVAN, LOCALID_THIEF_LEADER, 0);
  LogRoomsA_FaceObjectEventAndWait(LOCALID_TIED_UP_VILLAGER,
                                   LOCALID_THIEF_LEADER, 40);
  // He walks up and his two men run in behind him.
  ObjectEvent_SetSpeed(LOCALID_THIEF_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_SetSpeed(LOCALID_FIRST_THIEF, PIXELS(3) / 2, PIXELS(3) / 4);
  ObjectEvent_SetSpeed(LOCALID_SECOND_THIEF, PIXELS(3) / 2, PIXELS(3) / 4);
  ObjectEvent_SetPosition(LOCALID_FIRST_THIEF, PIXELS(744), PIXELS(408));
  ObjectEvent_SetPosition(LOCALID_SECOND_THIEF, PIXELS(744), PIXELS(408));
  ObjectEvent_WalkToAndWait(LOCALID_THIEF_LEADER, 792, 416);
  ObjectEvent_FaceDirection(LOCALID_THIEF_LEADER,
                            FACING_SOUTHEAST + FACING_STEP, 0);
  FieldScript_WaitFrames(70);
  ObjectMotion_SetAngleToward(LOCALID_PARTY_LEADER, LOCALID_THIEF_LEADER, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_GARET, LOCALID_THIEF_LEADER, 0);
  ObjectMotion_SetAngleToward(LOCALID_IVAN, LOCALID_THIEF_LEADER, 0);
  ObjectMotion_SetAngleToward(LOCALID_TIED_UP_VILLAGER, LOCALID_THIEF_LEADER,
                              0);
  ObjectEvent_SetActionScript(LOCALID_FIRST_THIEF,
                              gLogRoomsA_FirstThiefEntryScript);
  FieldScript_WaitFrames(40);
  ObjectEvent_SetActionScript(LOCALID_SECOND_THIEF,
                              gLogRoomsA_SecondThiefEntryScript);
  ObjectEvent_RefreshSelectorById(LOCALID_SECOND_THIEF);
  ObjectEvent_FaceDirection(LOCALID_FIRST_THIEF, FACING_SOUTHEAST, 0);
  ObjectEvent_FaceDirection(LOCALID_SECOND_THIEF, FACING_SOUTHEAST, 0);
  FieldScript_WaitFrames(40);
  // The three taunt Hammet's whelp and say Hammet was caught by worse thieves
  // after fleeing to Lunpa.
  LogRoomsA_PlayAnimationAndWait(LOCALID_THIEF_LEADER, ANIM_SHAKE_HEAD, 20);
  LogRoomsA_ShowMessageAndWait(LOCALID_THIEF_LEADER, 20);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_FIRST_THIEF, 1);
  FieldScript_WaitFrames(10);
  LogRoomsA_ShowMessageAndWait(LOCALID_FIRST_THIEF, 20);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_SECOND_THIEF, 1);
  FieldScript_WaitFrames(10);
  LogRoomsA_ShowMessageAndWait(LOCALID_SECOND_THIEF, 40);
  ObjectEvent_ShowEmote(LOCALID_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  FieldScript_WaitFrames(60);
  LogRoomsA_PlayAnimationAndWait(LOCALID_THIEF_LEADER, ANIM_SHAKE_HEAD, 20);
  LogRoomsA_ShowMessageAndWait(LOCALID_THIEF_LEADER, 20);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION,
                        0);
  ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  FieldScript_WaitFrames(60);
  // The tied-up villager is alarmed at the name Lunpa; Garet asks what kind of
  // place it is, and the tied-up villager and the thieves tell of Lunpa, Donpa
  // and Dodonpa.
  ObjectEvent_ShowEmote(LOCALID_TIED_UP_VILLAGER,
                        EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  FieldScript_WaitFrames(40);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_TIED_UP_VILLAGER, 2);
  FieldScript_WaitFrames(20);
  LogRoomsA_ShowMessageAndWait(LOCALID_TIED_UP_VILLAGER, 20);
  ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_TIED_UP_VILLAGER,
                              0);
  ObjectMotion_SetAngleToward(LOCALID_GARET, LOCALID_TIED_UP_VILLAGER, 0);
  LogRoomsA_FaceObjectEventAndWait(LOCALID_IVAN, LOCALID_TIED_UP_VILLAGER, 20);
  LogRoomsA_PlayAnimationAndWait(LOCALID_TIED_UP_VILLAGER, ANIM_SHAKE_HEAD, 30);
  LogRoomsA_ShowMessageAndWait(LOCALID_TIED_UP_VILLAGER, 20);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_IVAN, 2);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetAngleToward(LOCALID_PARTY_LEADER, LOCALID_IVAN, 0);
  LogRoomsA_FaceObjectEventAndWait(LOCALID_GARET, LOCALID_IVAN, 30);
  LogRoomsA_ShowMessageAndWait(LOCALID_GARET, 20);
  ObjectEvent_ShowEmote(LOCALID_TIED_UP_VILLAGER, EMOTE_IN_FRONT | EMOTE_WINCE,
                        0);
  FieldScript_WaitFrames(60);
  LogRoomsA_ShowMessageAndWait(LOCALID_TIED_UP_VILLAGER, 20);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_THIEF_LEADER, 1);
  FieldScript_WaitFrames(10);
  LogRoomsA_ShowMessageAndWait(LOCALID_THIEF_LEADER, 20);
  ObjectMotion_SetAngleToward(LOCALID_PARTY_LEADER, LOCALID_THIEF_LEADER, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_GARET, LOCALID_THIEF_LEADER, 0);
  LogRoomsA_FaceObjectEventAndWait(LOCALID_IVAN, LOCALID_THIEF_LEADER, 20);
  LogRoomsA_PlayAnimationAndWait(LOCALID_FIRST_THIEF, ANIM_NOD, 20);
  LogRoomsA_ShowMessageAndWait(LOCALID_FIRST_THIEF, 20);
  LogRoomsA_PlayAnimationAndWait(LOCALID_SECOND_THIEF, ANIM_SHAKE_HEAD, 20);
  LogRoomsA_ShowMessageAndWait(LOCALID_SECOND_THIEF, 20);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  ObjectEvent_ShowEmote(LOCALID_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  FieldScript_WaitFrames(60);
  // "See, we're not so bad. How about it? Will you let us go?" Yes: if they
  // return the goods and repent? No: even returning everything is a crime?
  LogRoomsA_PlayAnimationAndWait(LOCALID_THIEF_LEADER, ANIM_NOD, 20);
  FieldMessage_ShowAtObject(LOCALID_THIEF_LEADER, 0);
  FieldScript_WaitFrames(50);
  ObjectMotion_SetAngleToward(LOCALID_IVAN, LOCALID_PARTY_LEADER, 0);
  LogRoomsA_FaceObjectEventAndWait(LOCALID_GARET, LOCALID_PARTY_LEADER, 30);
  if (FieldScript_AskYesNoWithObjectEventAnimation(0, 0) == 0) {
    FieldScript_WaitFrames(40);
    ObjectMotion_SetAngleToward(LOCALID_GARET, LOCALID_THIEF_LEADER, 0);
    ObjectEvent_FaceObjectEvent(LOCALID_IVAN, LOCALID_THIEF_LEADER, 0);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_THIEF_LEADER, 2);
    FieldScript_WaitFrames(20);
    FieldScript_ShowMessageAndWait(LOCALID_THIEF_LEADER, 0);
  } else {
    FieldScript_WaitFrames(40);
    ObjectMotion_SetAngleToward(LOCALID_GARET, LOCALID_THIEF_LEADER, 0);
    ObjectMotion_SetAngleToward(LOCALID_IVAN, LOCALID_THIEF_LEADER, 0);
    base = MsgVaultVaultThievesCaught;
    FieldScript_SetMessageId(base);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_THIEF_LEADER, 2);
    FieldScript_WaitFrames(20);
    FieldScript_ShowMessageAndWait(LOCALID_THIEF_LEADER, 0);
    // Both answers go on with the same refusal.
    base -= 3;
    FieldScript_SetMessageId(base);
  }
  // "We stole this stuff fair and square!" They agree to keep the party quiet,
  // and the leader cries "Get 'em!!!"
  ObjectEvent_ShowEmote(LOCALID_FIRST_THIEF, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN,
                        0);
  FieldScript_WaitFrames(60);
  LogRoomsA_ShowMessageAndWait(LOCALID_FIRST_THIEF, 20);
  LogRoomsA_PlayAnimationAndWait(LOCALID_SECOND_THIEF, ANIM_SHAKE_HEAD, 20);
  LogRoomsA_ShowMessageAndWait(LOCALID_SECOND_THIEF, 30);
  LogRoomsA_FaceEachOtherAndWait(LOCALID_THIEF_LEADER, LOCALID_FIRST_THIEF, 30);
  Import_ObjectEventSetAnimation(LOCALID_THIEF_LEADER, ANIM_NOD);
  LogRoomsA_PlayAnimationAndWait(LOCALID_FIRST_THIEF, ANIM_NOD, 30);
  LogRoomsA_FaceEachOtherAndWait(LOCALID_THIEF_LEADER, LOCALID_SECOND_THIEF,
                                 30);
  Import_ObjectEventSetAnimation(LOCALID_THIEF_LEADER, ANIM_NOD);
  LogRoomsA_PlayAnimationAndWait(LOCALID_SECOND_THIEF, ANIM_NOD, 40);
  ObjectEvent_FaceDirection(LOCALID_THIEF_LEADER,
                            FACING_SOUTHEAST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_FIRST_THIEF, FACING_SOUTHEAST + FACING_STEP,
                            0);
  ObjectEvent_FaceDirection(LOCALID_SECOND_THIEF,
                            FACING_SOUTHEAST + FACING_STEP, 0);
  FieldScript_WaitFrames(20);
  LogRoomsA_PlayAnimationAndWait(LOCALID_THIEF_LEADER, ANIM_SHAKE_HEAD, 20);
  FieldScript_ShowMessageAndWait(LOCALID_THIEF_LEADER, 0);
  // Preserve the return destinations before requesting the thieves' encounter.
  GameFlag_Set(FLAG_VAULT_THIEVES_ATTACKED);
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
  base = (s32)&SceneId_LogRoomsA;
  FieldScript_SetNextDestination(base, LOG_ROOMS_A_LOFT_AFTER_THIEVES_DEFEATED);
  FieldScript_SetDefeatDestination(base,
                                   LOG_ROOMS_A_LOFT_AFTER_LOSING_TO_THIEVES);
  Battle_SetType(BATTLE_TYPE_SCRIPTED);
  Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED_VALE_VAULT, 5);
  FieldScript_End();
}

/// At Vault's inn, the party leaves the guests' room for the floor below
/// (LOG_ROOMS_A_INN_FROM_INN_UPSTAIRS).
///
/// Reached by stepping on MB_LOG_ROOMS_A_GUESTS_ROOM_STAIRS before the
/// thieves are jailed (after, the same cell is a plain warp); takes
/// LOG_ROOMS_A_INN_UPSTAIRS_TO_INN.
static void EventScript_LeaveGuestsRoom(void) {
  Sound_PlaySong(SE_MAP_EXIT);
  Import_ScriptRequestWarp(LOG_ROOMS_A_INN_UPSTAIRS_TO_INN);
}

/// In Vault's room with the crate, the party climbs up into the inn's loft by
/// the way behind the crate.
///
/// Reached by stepping on MB_LOG_ROOMS_A_LOFT_CLIMB; takes
/// LOG_ROOMS_A_CRATE_ROOM_TO_LOFT.
static void EventScript_ClimbToLoft(void) {
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(1) / 2, PIXELS(1) / 4);
  ObjectEvent_WalkTo(LOCALID_PARTY_LEADER, 728, 408);
  gFieldWork->transitionFrames = 16;
  Script_PlaySong(SE_MAP_EXIT);
  Import_ScriptRequestWarp(LOG_ROOMS_A_CRATE_ROOM_TO_LOFT);
}

/// Leaving the inn's loft, the party hears the tied-up villager groan.
/// It hears him when it leaves before the thieves are confronted.
/// "Uhnn... Uhnn..."
///
/// Reached by stepping on MB_LOG_ROOMS_A_LOFT_DESCENT; takes
/// LOG_ROOMS_A_LOFT_TO_CRATE_ROOM back down.
static void EventScript_LeaveLoft(void) {
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(1) / 2, PIXELS(1) / 4);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 744, 408);
  if (GameFlag_Get(FLAG_VAULT_THIEVES_ATTACKED) == 0) {
    FieldScript_Start();
    FieldScript_SetMessageId(MsgVaultUhnnUhnn);
    Script_ShowMessage(LOCALID_TIED_UP_VILLAGER, 0);
    FieldScript_End();
  }
  gFieldWork->transitionFrames = 16;
  Sound_PlaySong(SE_MAP_EXIT);
  Import_ScriptRequestWarp(LOG_ROOMS_A_LOFT_TO_CRATE_ROOM);
}

/// In the guests' room at Vault's inn, the two suspects spot "that weird kid"
/// with the party and run from him. "What do you want?"
///
/// Reached by stepping on MB_LOG_ROOMS_A_GUESTS_ROOM while
/// FLAG_VAULT_IVAN_JOINED_ROD_SEARCH is set. Sets
/// FLAG_VAULT_SUSPECTS_APPROACHED and FLAG_SUSPECTS_FLEEING, starts their
/// routes and the callback that keeps them facing the leader.
/// It plays once: it copies MB_LOG_ROOMS_A_GUESTS_ROOM_DOOR cells over
/// the cells that reach it.
static void EventScript_ApproachSuspects(void) {

  struct ObjectEvent *suspectedLeader;
  struct ObjectEvent *suspectedThief;

  suspectedLeader = ObjectEvent_GetById(LOCALID_SUSPECTED_LEADER);
  suspectedThief = ObjectEvent_GetById(LOCALID_SUSPECTED_THIEF);
  FieldScript_Start();
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_SetSpeed(LOCALID_GARET, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_SetSpeed(LOCALID_IVAN, PIXELS(4) / 5, PIXELS(2) / 5);
  // Isaac walks in; the two start and spot Ivan with him: "It looks like that
  // weird kid is with them!"
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 232, 696);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 200, 696);
  FieldScript_WaitFrames(10);
  ObjectEvent_ShowEmote(LOCALID_SUSPECTED_THIEF,
                        EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  ObjectEvent_ShowEmote(LOCALID_SUSPECTED_LEADER,
                        EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  FieldScript_WaitFrames(60);
  LogRoomsA_FaceObjectEventAndWait(LOCALID_SUSPECTED_THIEF,
                                   LOCALID_PARTY_LEADER, 10);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_SUSPECTED_LEADER, 2);
  FieldScript_WaitFrames(20);
  FieldScript_SetMessageId(MsgVaultWant);
  LogRoomsA_ShowMessageAndWait(LOCALID_SUSPECTED_LEADER, 20);
  ObjectEvent_SetAttachedEffect(
      LOCALID_SUSPECTED_THIEF, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldScript_WaitFrames(60);
  LogRoomsA_ShowMessageAndWait(LOCALID_SUSPECTED_THIEF, 20);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_SUSPECTED_LEADER, 1);
  LogRoomsA_ShowMessageAndWait(LOCALID_SUSPECTED_LEADER, 30);
  // "Don't let him near me!" They run off to their routes' first spots.
  ObjectEvent_SetSpeed(LOCALID_SUSPECTED_LEADER, PIXELS(4), PIXELS(2));
  ObjectEvent_SetSpeed(LOCALID_SUSPECTED_THIEF, PIXELS(7) / 2, PIXELS(7) / 4);
  ObjectEvent_SetActionScript(LOCALID_SUSPECTED_THIEF,
                              gLogRoomsA_SuspectToSouthWestCorner);
  ObjectEvent_SetActionScript(LOCALID_SUSPECTED_LEADER,
                              gLogRoomsA_SuspectRoundToWestBedHead);
  ObjectEvent_RefreshSelectorById(LOCALID_SUSPECTED_LEADER);
  MapGrid_CopyCellAttributes(14, 45, 3, 1, 14, 44);
  GameFlag_Set(FLAG_VAULT_SUSPECTS_APPROACHED);
  GameFlag_Set(FLAG_SUSPECTS_FLEEING);
  FieldScheduler_AddCallback(LogRoomsA_FaceSuspectsTowardLeader,
                             TASK_PRIORITY_MAP);
  suspectedLeader->timer = 1;
  suspectedThief->timer = 3;
  FieldScript_End();
}

/// Stepping out of the guests' room at Vault's inn, the party talks it over:
/// Ivan can't read their minds while they run; Garet tells Isaac to take the
/// lead and corner them.
///
/// Reached by stepping on MB_LOG_ROOMS_A_GUESTS_ROOM_DOOR while
/// FLAG_SUSPECTS_FLEEING is set. Clears it and stops the facing callback; the
/// suspects go back to their places.
static void EventScript_DiscussSuspiciousGuests(void) {
  struct ObjectEvent *suspectedLeader;
  struct ObjectEvent *suspectedThief;
  s32 base;

  suspectedLeader = ObjectEvent_GetById(LOCALID_SUSPECTED_LEADER);
  suspectedThief = ObjectEvent_GetById(LOCALID_SUSPECTED_THIEF);
  FieldScript_Start();
  Task_Remove(LogRoomsA_FaceSuspectsTowardLeader);
  GameFlag_Clear(FLAG_SUSPECTS_FLEEING);
  if (suspectedLeader->timer <= 3) {
    ObjectEvent_SetActionScript(LOCALID_SUSPECTED_LEADER,
                                gLogRoomsA_SuspectRoundToWestBedFoot);
  } else {
    ObjectEvent_SetActionScript(LOCALID_SUSPECTED_LEADER,
                                gLogRoomsA_SuspectToWestBedFoot);
  }
  if (suspectedThief->timer <= 2) {
    ObjectEvent_SetActionScript(LOCALID_SUSPECTED_THIEF,
                                gLogRoomsA_SuspectAcrossToWestOfBeds);
  } else {
    ObjectEvent_SetActionScript(LOCALID_SUSPECTED_THIEF,
                                gLogRoomsA_SuspectToWestBedFoot);
  }
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_SetSpeed(LOCALID_GARET, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_SetSpeed(LOCALID_IVAN, PIXELS(4) / 5, PIXELS(2) / 5);
  // Outside the room, Garet and Ivan step out of the leader.
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 248, 728);
  ObjectEvent_SetPosition(LOCALID_IVAN, PIXELS(248), PIXELS(728));
  ObjectEvent_SetPosition(LOCALID_GARET, PIXELS(248), PIXELS(728));
  ObjectEvent_WalkTo(LOCALID_IVAN, 264, 744);
  ObjectEvent_WalkToAndWait(LOCALID_GARET, 232, 744);
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_IVAN);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTH, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_GARET, LOCALID_PARTY_LEADER, 0);
  LogRoomsA_FaceObjectEventAndWait(LOCALID_IVAN, LOCALID_PARTY_LEADER, 30);
  // Ivan: they're acting suspicious.
  ObjectMotion_SetVarCbAndRefresh(LOCALID_IVAN, 1);
  base = MsgVaultTheyreActingSuspiciousSomethingsNot;
  FieldScript_SetMessageId(base);
  Script_ShowMessage(LOCALID_IVAN, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_IVAN, 0);
  LogRoomsA_FaceObjectEventAndWait(LOCALID_GARET, LOCALID_IVAN, 20);
  Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_NOD);
  LogRoomsA_PlayAnimationAndWait(LOCALID_GARET, ANIM_NOD, 20);
  LogRoomsA_FaceEachOtherAndWait(LOCALID_PARTY_LEADER, LOCALID_GARET, 10);
  // Garet: "But you can't use Mind Read if they keep running away, can you?"
  // Yes: Ivan asks how he can catch them; no: there must be a way.
  Script_OpenMessage(LOCALID_GARET, 0);
  if (FieldScript_AskYesNoWithObjectEventAnimation(0, 0) != 0) {
    gFieldWork->message++;
  }
  LogRoomsA_ShowMessageAndWait(LOCALID_GARET, 30);
  // Garet's plan: the three of them surround one of the two.
  FieldScript_SetMessageId((base + 4));
  ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_IVAN, 0);
  LogRoomsA_FaceObjectEventAndWait(LOCALID_GARET, LOCALID_IVAN, 50);
  ObjectEvent_ShowEmote(LOCALID_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  FieldScript_WaitFrames(60);
  LogRoomsA_FaceEachOtherAndWait(LOCALID_PARTY_LEADER, LOCALID_GARET, 50);
  ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_IVAN, 0);
  LogRoomsA_FaceObjectEventAndWait(LOCALID_GARET, LOCALID_IVAN, 30);
  LogRoomsA_PlayAnimationAndWait(LOCALID_IVAN, ANIM_NOD, 10);
  LogRoomsA_ShowMessageAndWait(LOCALID_IVAN, 20);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  FieldScript_WaitFrames(60);
  LogRoomsA_PlayAnimationAndWait(LOCALID_IVAN, ANIM_NOD, 20);
  LogRoomsA_ShowMessageAndWait(LOCALID_IVAN, 30);
  // "Isaac, you take the lead." The two walk back into him, and the suspects
  // are put back at their table.
  FieldScript_SetMessageId(MsgVaultIsaacTakeLead);
  Script_ShowMessage(LOCALID_GARET, 0);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTH, 0);
  FieldScript_WaitFrames(40);
  Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_NOD);
  Import_ObjectEventSetAnimation(LOCALID_GARET, ANIM_NOD);
  LogRoomsA_PlayAnimationAndWait(LOCALID_IVAN, ANIM_NOD, 50);
  ObjectEvent_WalkTo(LOCALID_IVAN, 248, 728);
  ObjectEvent_WalkToAndWait(LOCALID_GARET, 248, 728);
  ObjectEvent_SetPosition(LOCALID_GARET, 0, 0);
  ObjectEvent_SetPosition(LOCALID_IVAN, 0, 0);
  ObjectEvent_SetPosition(LOCALID_SUSPECTED_LEADER, PIXELS(104), PIXELS(696));
  ObjectEvent_SetPosition(LOCALID_SUSPECTED_THIEF, PIXELS(120), PIXELS(696));
  ObjectEvent_FaceDirection(LOCALID_SUSPECTED_LEADER, FACING_EAST, 0);
  ObjectEvent_FaceDirection(LOCALID_SUSPECTED_THIEF, FACING_WEST, 0);
  MapGrid_CopyCellAttributes(14, 50, 3, 1, 14, 44);
  FieldScript_End();
}

/// In the guests' room at Vault's inn, the party sets out to corner the two
/// suspects.
///
/// Reached by stepping on MB_LOG_ROOMS_A_CORNERING_START while
/// FLAG_VAULT_SUSPECTS_APPROACHED is set. Sets FLAG_MENUS_RAISE_EVENT, so the
/// menus answer with Garet's refusal, and FLAG_VAULT_CORNERING_SUSPECTS, so
/// talking to Ivan or the suspects does nothing; then starts the chase
/// (LogRoomsA_StartObjectEventStops).
static void EventScript_StartCorneringSuspects(void) {
  GameFlag_Set(FLAG_MENUS_RAISE_EVENT);
  GameFlag_Set(FLAG_VAULT_CORNERING_SUSPECTS);
  LogRoomsA_StartObjectEventStops();
}

/// Has the suspects react to the party's return and resets their route states.
/// "Hey, they're back!"
///
/// Called by LogRoomsA_StartObjectEventStops.
static void LogRoomsA_ResetSuspectRoutes(void) {
  FieldScript_WaitFrames(30);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_SUSPECTED_LEADER, 1);
  FieldScript_WaitFrames(20);
  FieldScript_SetMessageId(MsgVaultTheyreBack);
  LogRoomsA_ShowMessageAndWait(LOCALID_SUSPECTED_LEADER, 20);
  ObjectEvent_FaceDirection(LOCALID_SUSPECTED_THIEF, FACING_EAST, 20);
  ObjectEvent_SetAttachedEffect(
      LOCALID_SUSPECTED_THIEF, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_SUSPECTED_THIEF, 2);
  LogRoomsA_ShowMessageAndWait(LOCALID_SUSPECTED_THIEF, 20);
  ObjectMotion_SetAnimationAndWait(LOCALID_SUSPECTED_LEADER, ANIM_SHAKE_HEAD);
  FieldScript_WaitFrames(20);
  LogRoomsA_ShowMessageAndWait(LOCALID_SUSPECTED_LEADER, 20);
  ObjectEvent_SetSpeed(LOCALID_SUSPECTED_LEADER, PIXELS(4), PIXELS(2));
  ObjectEvent_SetSpeed(LOCALID_SUSPECTED_THIEF, PIXELS(7) / 2, PIXELS(7) / 4);
  ObjectEvent_SetActionScript(LOCALID_SUSPECTED_THIEF,
                              gLogRoomsA_SuspectToSouthWestCorner);
  ObjectEvent_SetActionScript(LOCALID_SUSPECTED_LEADER,
                              gLogRoomsA_SuspectRoundToWestBedHead);
  ObjectEvent_RefreshSelectorById(LOCALID_SUSPECTED_LEADER);
  {
    struct ObjectEvent *suspect = ObjectEvent_GetById(LOCALID_SUSPECTED_LEADER);
    u16 routeIndex = 1;

    suspect->timer = routeIndex;
  }
  {
    struct ObjectEvent *suspect = ObjectEvent_GetById(LOCALID_SUSPECTED_THIEF);
    u16 routeIndex = 3;

    suspect->timer = routeIndex;
  }
  MapGrid_CopyCellAttributes(14, 48, 4, 1, 14, 44);
}

/// Sets the three party images to the priority used during the mind-reading
/// event.
static inline void LogRoomsA_SetPartyImagePriority(void) {
  {
    struct Sprite *sprite = ObjectEvent_GetById(LOCALID_PARTY_LEADER)->sprite;
    sprite->image.priority = 1;
  }
  {
    struct Sprite *sprite = ObjectEvent_GetById(LOCALID_GARET)->sprite;
    sprite->image.priority = 1;
  }
  {
    struct Sprite *sprite = ObjectEvent_GetById(LOCALID_IVAN)->sprite;
    sprite->image.priority = 1;
  }
}

/// Once a suspect is cornered, Garet calls Ivan and Ivan reads their minds.
/// This is in the guests' room at Vault's inn. "Uh-oh! I'm surrounded!" "Now,
/// Ivan!"
///
/// Reached through raised events 201 to 203, which
/// LogRoomsA_UpdateObjectEventStops raises after thirty frames with at
/// least one suspect blocked: 201 the leader, 202 the thief, 203 both. Stops
/// the chase, clears FLAG_MENUS_RAISE_EVENT and FLAG_VAULT_CORNERING_SUSPECTS,
/// plays the party's talk (LogRoomsA_DiscussMindReading) and sets
/// FLAG_VAULT_SUSPECTS_MIND_READ.
static void EventScript_ReadCorneredSuspectsMinds(void) {
  s32 localId;
  struct FieldWork *work;

  work = gFieldWork;
  FieldScript_Start();
  Task_Remove(LogRoomsA_UpdateObjectEventStops);
  GameFlag_Clear(FLAG_MENUS_RAISE_EVENT);
  GameFlag_Clear(FLAG_VAULT_CORNERING_SUSPECTS);
  Import_ObjectEventSetAnimation(LOCALID_SUSPECTED_LEADER, ANIM_STAND);
  Import_ObjectEventSetAnimation(LOCALID_SUSPECTED_THIEF, ANIM_STAND);
  Import_ObjectEventSetAnimation(LOCALID_IVAN, ANIM_STAND);
  Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_STAND);
  Import_ObjectEventSetAnimation(LOCALID_GARET, ANIM_STAND);
  ObjectEvent_FaceObjectEvent(LOCALID_GARET, LOCALID_IVAN, 0);
  ObjectEvent_LinkPair(LOCALID_PARTY_LEADER, LOCALID_IVAN, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_SUSPECTED_LEADER, LOCALID_IVAN, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_SUSPECTED_THIEF, LOCALID_IVAN, 0);
  FieldScript_WaitFrames(10);
  localId = LOCALID_SUSPECTED_LEADER;
  // The event identifies the suspects blocked on the last frame of the wait.
  switch (work->raisedEvent) {
  case LOG_ROOMS_A_THIEF_CORNERED:
  case LOG_ROOMS_A_BOTH_CORNERED:
    FieldScript_SetMessageId(MsgVaultImSurrounded);
    ObjectEvent_SetAttachedEffect(LOCALID_SUSPECTED_THIEF,
                                  ATTACHED_EFFECT_IN_FRONT |
                                      ATTACHED_EFFECT_ONCE);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_SUSPECTED_THIEF, 2);
    LogRoomsA_ShowMessageAndWait(LOCALID_SUSPECTED_THIEF, 20);
    if (work->raisedEvent == LOG_ROOMS_A_THIEF_CORNERED) {
      localId = LOCALID_SUSPECTED_THIEF;
      break;
    }
    // When both are cornered, the leader's reaction follows the thief's.
    // fall through
  case LOG_ROOMS_A_LEADER_CORNERED:
    FieldScript_SetMessageId(MsgVaultTheresNowhereRun);
    ObjectEvent_SetAttachedEffect(LOCALID_SUSPECTED_LEADER,
                                  ATTACHED_EFFECT_IN_FRONT |
                                      ATTACHED_EFFECT_ONCE);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_SUSPECTED_LEADER, 2);
    localId = LOCALID_SUSPECTED_LEADER;
    LogRoomsA_ShowMessageAndWait(LOCALID_SUSPECTED_LEADER, 20);
    break;
  }
  Import_ObjectEventSetAnimation(LOCALID_IVAN, ANIM_STAND);
  ObjectEvent_FaceObjectEvent(LOCALID_IVAN, localId, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_GARET, LOCALID_IVAN, 0);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 2);
  FieldScript_WaitFrames(20);
  // "Now, Ivan!" "A-All right!" Ivan reads the cornered one's mind while the
  // two wonder what he is doing.
  FieldScript_SetMessageId(MsgVaultNowIvan);
  LogRoomsA_ShowMessageAndWait(LOCALID_GARET, 20);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_IVAN, 2);
  FieldScript_WaitFrames(20);
  LogRoomsA_ShowMessageAndWait(LOCALID_IVAN, 20);
  LogRoomsA_StartMindRead(LOCALID_IVAN, localId);
  FieldEffect_RestoreMindReadPalette();
  FieldScript_WaitFrames(60);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_SUSPECTED_LEADER, 2);
  LogRoomsA_ShowMessageAndWait(LOCALID_SUSPECTED_LEADER, 20);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_SUSPECTED_THIEF, 2);
  LogRoomsA_ShowMessageAndWait(LOCALID_SUSPECTED_THIEF, 20);
  // "Are you done, Ivan?" He nods, and the window closes.
  ObjectEvent_ShowEmote(LOCALID_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  LogRoomsA_EndMindRead();
  ObjectMotion_SetVariantCallback(LOCALID_GARET, 2);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_PARTY_LEADER, 2);
  FieldScript_WaitFrames(20);
  LogRoomsA_ShowMessageAndWait(LOCALID_GARET, 20);
  ObjectEvent_FaceObjectEvent(LOCALID_IVAN, LOCALID_GARET, 0);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetAnimationAndWait(LOCALID_IVAN, ANIM_NOD);
  FieldScript_WaitFrames(40);
  LogRoomsA_SetPartyImagePriority();
  gFieldWork->transitionFrames = 24;
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
  FieldScript_FinishScreenTransition();
  FieldScript_WaitTransitionFrames();
  // Move the discussion away from the suspects while the screen is closed.
  LogRoomsA_DiscussMindReading();
  // The suspects stay at their table, turned toward the leader from now on.
  MapGrid_CopyCellAttributes(14, 45, 3, 1, 14, 44);
  GameFlag_Set(FLAG_VAULT_SUSPECTS_MIND_READ);
  {
    struct ObjectEvent *suspect = ObjectEvent_GetById(LOCALID_SUSPECTED_LEADER);
    s32 routeIndex = 5;

    suspect->timer = routeIndex;
  }
  {
    struct ObjectEvent *suspect = ObjectEvent_GetById(LOCALID_SUSPECTED_THIEF);
    s32 routeIndex = 4;

    suspect->timer = routeIndex;
  }
  FieldScheduler_AddCallback(LogRoomsA_FaceSuspectsTowardLeader,
                             TASK_PRIORITY_MAP);
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  FieldScript_End();
}

/// Shows the party's branching conversation about what Ivan learned: the
/// stolen goods are hidden in the inn, which they must search.
///
/// Called by EventScript_ReadCorneredSuspectsMinds while the screen is closed;
/// sets the party outside the room and opens the screen on them.
static void LogRoomsA_DiscussMindReading(void) {
  s32 persistedWithQuestions;

  // Move the party outside the room and the suspects back to their table.
  Scene_PlayMusic();
  ObjectMotion_SetActionVariant(LOCALID_PARTY_LEADER, 1);
  {
    struct ObjectEvent *leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);

    leader->priorityFlags =
        OBJ_EVENT_PRIORITY_AUTOMATIC | leader->priorityFlags;
  }
  Field_SetViewBounds(PIXELS(32), PIXELS(576), PIXELS(400), PIXELS(936));
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_SetSpeed(LOCALID_GARET, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_SetSpeed(LOCALID_IVAN, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_SetPosition(LOCALID_PARTY_LEADER, PIXELS(248), PIXELS(728));
  ObjectEvent_SetPosition(LOCALID_IVAN, PIXELS(264), PIXELS(744));
  Import_ObjectEventSetPosition(LOCALID_GARET, PIXELS(232), PIXELS(744));
  LogRoomsA_SetPartyImagePriority();
  ObjectEvent_LinkPair(LOCALID_PARTY_LEADER, LOCALID_IVAN, 0);
  LogRoomsA_FaceObjectEventAndWait(LOCALID_GARET, LOCALID_IVAN, 30);
  ObjectEvent_SetPosition(LOCALID_SUSPECTED_LEADER, PIXELS(104), PIXELS(696));
  ObjectEvent_SetPosition(LOCALID_SUSPECTED_THIEF, PIXELS(120), PIXELS(696));
  ObjectEvent_LinkPair(LOCALID_SUSPECTED_LEADER, LOCALID_SUSPECTED_THIEF, 0);
  FieldCamera_FollowObjectEvent(LOCALID_PARTY_LEADER, 0);
  FieldCamera_WaitForArrival();
  MapGrid_CenterOnFollowedPoint();
  FieldScript_WaitFrames(30);
  {
    struct FieldWork *work = gFieldWork;

    work->transitionFrames = 24;
    work->transition =
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
  }
  FieldScript_StartScreenTransition();
  FieldScript_WaitTransitionFrames();
  FieldScript_WaitFrames(40);
  // "What did you learn, Ivan?" It is them: they hid the stolen goods
  // somewhere in this inn, but won't let him near again.
  ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 1);
  FieldScript_WaitFrames(10);
  FieldScript_SetMessageId(MsgVaultLearn);
  LogRoomsA_ShowMessageAndWait(LOCALID_GARET, 20);
  LogRoomsA_PlayAnimationAndWait(LOCALID_IVAN, ANIM_NOD, 20);
  LogRoomsA_ShowMessageAndWait(LOCALID_IVAN, 20);
  LogRoomsA_FaceEachOtherAndWait(LOCALID_PARTY_LEADER, LOCALID_GARET, 50);
  ObjectMotion_SetAngleToward(LOCALID_PARTY_LEADER, LOCALID_IVAN, 0);
  LogRoomsA_FaceObjectEventAndWait(LOCALID_GARET, LOCALID_IVAN, 20);
  LogRoomsA_PlayAnimationAndWait(LOCALID_IVAN, ANIM_SHAKE_HEAD, 20);
  FieldScript_ShowMessageAndWait(LOCALID_IVAN, 0);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  FieldScript_WaitFrames(60);
  LogRoomsA_PlayAnimationAndWait(LOCALID_IVAN, ANIM_NOD, 20);
  LogRoomsA_ShowMessageAndWait(LOCALID_IVAN, 30);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 1);
  FieldScript_WaitFrames(20);
  LogRoomsA_FaceEachOtherAndWait(LOCALID_PARTY_LEADER, LOCALID_GARET, 20);
  // Garet: they have to search the inn. Each no leads to another plea, from
  // Garet and then Ivan; three noes and Garet vouches for Isaac's sense of
  // duty, while any yes gets "Leave it to us!"
  FieldMessage_ShowAtObject(LOCALID_GARET, 0);
  persistedWithQuestions = 0;
  if (FieldScript_AskYesNoWithObjectEventAnimation(0, 0) != 0) {
    FieldScript_WaitFrames(20);
    ObjectMotion_SetVariantCallback(LOCALID_IVAN, 2);
    LogRoomsA_PlayAnimationAndWait(LOCALID_GARET, ANIM_SHAKE_HEAD, 30);
    FieldMessage_ShowAtObject(LOCALID_GARET, 0);
    persistedWithQuestions = 0;
    if (FieldScript_AskYesNoWithObjectEventAnimation(0, 0) != 0) {
      FieldScript_WaitFrames(20);
      FieldEffect_SpawnLinked(LOCALID_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
      FieldScript_WaitFrames(60);
      ObjectMotion_SetVarCbAndRefresh(LOCALID_IVAN, 2);
      LogRoomsA_FaceEachOtherAndWait(LOCALID_PARTY_LEADER, LOCALID_IVAN, 20);
      FieldMessage_ShowAtObject(LOCALID_IVAN, 0);
      persistedWithQuestions = 0;
      if (FieldScript_AskYesNoWithObjectEventAnimation(2, 0) != 0) {
        FieldScript_WaitFrames(20);
        FieldEffect_SpawnLinked(LOCALID_IVAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS,
                                0);
        FieldScript_WaitFrames(60);
        LogRoomsA_ShowMessageAndWait(LOCALID_IVAN, 20);
        LogRoomsA_FaceObjectEventAndWait(LOCALID_GARET, LOCALID_IVAN, 10);
        ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 1);
        FieldScript_WaitFrames(10);
        LogRoomsA_ShowMessageAndWait(LOCALID_GARET, 10);
        LogRoomsA_FaceObjectEventAndWait(LOCALID_IVAN, LOCALID_GARET, 20);
        FieldEffect_SpawnLinked(LOCALID_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION,
                                0);
        FieldScript_WaitFrames(60);
        LogRoomsA_PlayAnimationAndWait(LOCALID_IVAN, ANIM_SHAKE_HEAD, 20);
        LogRoomsA_ShowMessageAndWait(LOCALID_GARET, 10);
        LogRoomsA_FaceEachOtherAndWait(LOCALID_GARET, LOCALID_PARTY_LEADER, 20);
        LogRoomsA_PlayAnimationAndWait(LOCALID_GARET, ANIM_NOD, 20);
        persistedWithQuestions = 1;
        LogRoomsA_ShowMessageAndWait(LOCALID_GARET, 20);
      }
    }
  }
  if (persistedWithQuestions == 0) {
    FieldScript_SetMessageId(MsgVaultLeave);
    LogRoomsA_ShowMessageAndWait(LOCALID_GARET, 20);
    LogRoomsA_FaceEachOtherAndWait(LOCALID_GARET, LOCALID_PARTY_LEADER, 20);
    LogRoomsA_ShowMessageAndWait(LOCALID_GARET, 20);
  }
  // "Let's go, Ivan." Garet and Ivan walk back into the leader.
  FieldEffect_SpawnLinked(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS,
                          0);
  FieldScript_WaitFrames(60);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 1);
  LogRoomsA_ShowMessageAndWait(LOCALID_GARET, 10);
  LogRoomsA_PlayAnimationAndWait(LOCALID_GARET, ANIM_NOD, 10);
  ObjectEvent_LinkPair(LOCALID_GARET, LOCALID_IVAN, 0);
  LogRoomsA_FaceEachOtherAndWait(LOCALID_PARTY_LEADER, LOCALID_IVAN, 10);
  FieldScript_ShowMessageAndWait(LOCALID_GARET, 0);
  LogRoomsA_PlayAnimationAndWait(LOCALID_IVAN, ANIM_NOD, 10);
  ObjectEvent_WalkTo(LOCALID_IVAN, 248, 728);
  ObjectEvent_WalkToAndWait(LOCALID_GARET, 248, 728);
  Import_ObjectEventSetPosition(LOCALID_GARET, 0, 0);
  Import_ObjectEventSetPosition(LOCALID_IVAN, 0, 0);
}

/// The fleeing suspects move on to their next spots.
/// This is in the guests' room at Vault's inn, when the leader steps on one of
/// the route cells, MB_LOG_ROOMS_A_FIRST_SUSPECT_ROUTE to
/// _LAST_SUSPECT_ROUTE.
///
/// Reached by stepping on one of them while FLAG_SUSPECTS_FLEEING is
/// set.
static void EventScript_SuspectsRunFromParty(void) {
  switch (gFieldWork->coordEvent) {
  case MB_LOG_ROOMS_A_FIRST_SUSPECT_ROUTE:
    LogRoomsA_AdvanceSuspectRoute(LOCALID_SUSPECTED_LEADER, 1, 2,
                                  gLogRoomsA_SuspectToMiddleBedHead);
    LogRoomsA_AdvanceSuspectRoute(LOCALID_SUSPECTED_THIEF, 3, 4,
                                  gLogRoomsA_SuspectAlongToSouthEastCorner);
    break;
  case MB_LOG_ROOMS_A_FIRST_SUSPECT_ROUTE + 1:
    LogRoomsA_AdvanceSuspectRoute(LOCALID_SUSPECTED_LEADER, 1, 4,
                                  gLogRoomsA_SuspectRoundToWestBedFoot);
    LogRoomsA_AdvanceSuspectRoute(LOCALID_SUSPECTED_LEADER, 2, 3,
                                  gLogRoomsA_SuspectToEastBedHead);
    LogRoomsA_AdvanceSuspectRoute(LOCALID_SUSPECTED_THIEF, 1, 3,
                                  gLogRoomsA_SuspectToSouthWestCorner);
    break;
  case MB_LOG_ROOMS_A_FIRST_SUSPECT_ROUTE + 2:
    LogRoomsA_AdvanceSuspectRoute(LOCALID_SUSPECTED_LEADER, 2, 1,
                                  gLogRoomsA_SuspectToWestBedHead);
    LogRoomsA_AdvanceSuspectRoute(LOCALID_SUSPECTED_LEADER, 3, 6,
                                  gLogRoomsA_SuspectRoundToEastBedFoot);
    LogRoomsA_AdvanceSuspectRoute(LOCALID_SUSPECTED_THIEF, 2, 4,
                                  gLogRoomsA_SuspectToSouthEastCorner);
    break;
  case MB_LOG_ROOMS_A_FIRST_SUSPECT_ROUTE + 3:
    LogRoomsA_AdvanceSuspectRoute(LOCALID_SUSPECTED_LEADER, 3, 2,
                                  gLogRoomsA_SuspectToMiddleBedHead);
    LogRoomsA_AdvanceSuspectRoute(LOCALID_SUSPECTED_THIEF, 4, 3,
                                  gLogRoomsA_SuspectAlongToSouthWestCorner);
    break;
  case MB_LOG_ROOMS_A_FIRST_SUSPECT_ROUTE + 4:
    LogRoomsA_AdvanceSuspectRoute(LOCALID_SUSPECTED_LEADER, 4, 5,
                                  gLogRoomsA_SuspectToMiddleBedFoot);
    LogRoomsA_AdvanceSuspectRoute(LOCALID_SUSPECTED_THIEF, 1, 2,
                                  gLogRoomsA_SuspectAcrossToEastOfBeds);
    break;
  case MB_LOG_ROOMS_A_FIRST_SUSPECT_ROUTE + 5:
    LogRoomsA_AdvanceSuspectRoute(LOCALID_SUSPECTED_LEADER, 4, 1,
                                  gLogRoomsA_SuspectRoundToWestBedHead);
    LogRoomsA_AdvanceSuspectRoute(LOCALID_SUSPECTED_LEADER, 5, 6,
                                  gLogRoomsA_SuspectToEastBedFoot);
    LogRoomsA_AdvanceSuspectRoute(LOCALID_SUSPECTED_THIEF, 3, 1,
                                  gLogRoomsA_SuspectToWestOfBeds);
    break;
  case MB_LOG_ROOMS_A_FIRST_SUSPECT_ROUTE + 6:
    LogRoomsA_AdvanceSuspectRoute(LOCALID_SUSPECTED_LEADER, 5, 4,
                                  gLogRoomsA_SuspectToWestBedFoot);
    LogRoomsA_AdvanceSuspectRoute(LOCALID_SUSPECTED_LEADER, 6, 3,
                                  gLogRoomsA_SuspectRoundToEastBedHead);
    LogRoomsA_AdvanceSuspectRoute(LOCALID_SUSPECTED_THIEF, 4, 2,
                                  gLogRoomsA_SuspectToEastOfBeds);
    break;
  case MB_LOG_ROOMS_A_LAST_SUSPECT_ROUTE:
    LogRoomsA_AdvanceSuspectRoute(LOCALID_SUSPECTED_LEADER, 6, 5,
                                  gLogRoomsA_SuspectToMiddleBedFoot);
    LogRoomsA_AdvanceSuspectRoute(LOCALID_SUSPECTED_THIEF, 2, 1,
                                  gLogRoomsA_SuspectAcrossToWestOfBeds);
    break;
  }
}

/// Installs a route script only when the suspect is in its expected state.
static void LogRoomsA_AdvanceSuspectRoute(s32 localId, s32 expected, s32 next,
                                          const s32 *actions) {
  struct ObjectEvent *suspect = ObjectEvent_GetById(localId);

  if (suspect->timer == expected) {
    ObjectEvent_SetActionScript(localId, actions);
    suspect->timer = next;
  }
}

#include "common/vault_spotflag.h"

/// Arriving in any room of Vault's houses, shops and inn: restores the room
/// for the story so far, and plays the loft's events: the tied-up villager, the
/// rematch, and the defeat or victory.
///
/// Called by the engine once the object events are placed, by warp destination.
/// At the inn, after the thieves are jailed, places the Bone and starts
/// Vault_UpdateBoneProximityFlag. After a lost battle
/// (LOG_ROOMS_A_LOFT_AFTER_LOSING_TO_THIEVES) takes
/// LOG_ROOMS_A_TO_SANCTUM; after a won one (17) sets
/// FLAG_VAULT_THIEVES_JAILED through LogRoomsA_ReturnStolenGoods, which
/// also removes Ivan from the party.
s32 Scene_Initialize(void) {
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  switch (gSaveBlock.warpDest) {
  case LOG_ROOMS_A_FROM_VAULT_EAST_SOUTHEAST: {
    s32 zero = 0;

    // The gem owner's house: he stands on the floor.
    ObjectEvent_GetById(LOCALID_GEM_OWNER)->motionFlags = zero;
    ObjectEvent_GetById(LOCALID_GEM_OWNER)->y.fixed = zero;
    ObjectEvent_GetById(LOCALID_GEM_OWNER)->groundHeight = zero;
  } break;
  case LOG_ROOMS_A_FROM_VAULT_NORTHWEST:
    // The mayor's house: Ivan waits a step back once met and is gone once he
    // has joined; after the thieves are jailed, the mayor stands by the door.
    if (GameFlag_Get(FLAG_VAULT_MET_IVAN) != 0) {
      ObjectEvent_SetPosition(LOCALID_IVAN, PIXELS(376), PIXELS(376));
      ObjectEvent_FaceDirection(LOCALID_IVAN, FACING_SOUTH, 0);
    }
    if (GameFlag_Get(FLAG_VAULT_IVAN_JOINED_ROD_SEARCH) != 0) {
      Import_ObjectEventSetPosition(LOCALID_IVAN, 0, 0);
    }
    if (GameFlag_Get(FLAG_VAULT_THIEVES_JAILED) != 0) {
      ObjectEvent_SetPosition(LOCALID_MAYOR, PIXELS(536),
                              LOG_ROOMS_A_ENTRY_OBJ_EVENT_Z);
      ObjectEvent_SetBuiltinAction(LOCALID_MAYOR, OBJ_EVENT_ACTION_IDLE);
      ObjectEvent_FaceDirection(LOCALID_MAYOR, FACING_SOUTH + FACING_STEP, 0);
    }
    break;
  case LOG_ROOMS_A_FROM_VAULT_SOUTHWEST:
  case LOG_ROOMS_A_INN_FROM_INN_UPSTAIRS:
    // The inn: once the thieves are jailed, the Bone lies on the floor beside
    // the girl at the inn; after the mind reading and before that, she is
    // out.
    if (GameFlag_Get(FLAG_VAULT_THIEVES_JAILED) != 0) {
      ObjectEvent_SetPosition(LOCALID_GIRL_AT_THE_INN, PIXELS(568),
                              PIXELS(648));
      ObjectEvent_SetBuiltinAction(LOCALID_GIRL_AT_THE_INN,
                                   OBJ_EVENT_ACTION_IDLE);
      ObjectEvent_FaceDirection(LOCALID_GIRL_AT_THE_INN, FACING_SOUTH, 0);
      Vault_PlaceItemIcon(ITEM_BONE, PIXELS(568), PIXELS(16), PIXELS(672));
      Task_AddCallback(Vault_UpdateBoneProximityFlag, TASK_PRIORITY_MAP);
    } else if (GameFlag_Get(FLAG_VAULT_SUSPECTS_MIND_READ) != 0) {
      Import_ObjectEventSetPosition(LOCALID_GIRL_AT_THE_INN, 0, 0);
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
      Task_AddCallback(LogRoomsA_FaceSuspectsTowardLeader, TASK_PRIORITY_MAP);
      break;
    }
    if (GameFlag_Get(FLAG_VAULT_IVAN_JOINED_ROD_SEARCH) != 0) {
      ObjectEvent_SetPosition(LOCALID_SUSPECTED_THIEF, PIXELS(120),
                              PIXELS(696));
      ObjectEvent_FaceDirection(LOCALID_SUSPECTED_THIEF, FACING_WEST, 0);
    }
    // Once approached, the room's doorway metatile behavior changes; after the
    // mind reading the suspects sit turned toward the leader.
    if (GameFlag_Get(FLAG_VAULT_SUSPECTS_APPROACHED) != 0) {
      MapGrid_CopyCellAttributes(14, 45, 3, 1, 14, 44);
      if (GameFlag_Get(FLAG_VAULT_SUSPECTS_MIND_READ) == 0) {
        MapGrid_CopyCellAttributes(14, 50, 3, 1, 14, 44);
        break;
      }
      ObjectEvent_GetById(LOCALID_SUSPECTED_LEADER)->timer = 5;
      ObjectEvent_GetById(LOCALID_SUSPECTED_THIEF)->timer = 4;
      FieldScheduler_AddCallback(LogRoomsA_FaceSuspectsTowardLeader,
                                 TASK_PRIORITY_MAP);
    }
    break;
  case LOG_ROOMS_A_FROM_VAULT_WEST:
  case LOG_ROOMS_A_CRATE_ROOM_FROM_LOFT:
    // The room with the crate: the crate keeps its place once moved, and the
    // passage behind it stays open.
    FieldScript_WaitFrames(2);
    MapGrid_CopyCellsTo(54, 2, 35, 20, 2, 10);
    MapGrid_CopyCellsTo(54, 2, 95, 20, 2, 10);
    MapGrid_CopyCellsTo(54, 2, 35, 80, 2, 10);
    MapGrid_CopyCellsTo(54, 2, 46, 21, 4, 8);
    MapGrid_CopyCellsTo(54, 2, 46, 81, 4, 8);
    ObjectEvent_GetById(LOCALID_CRATE)->collisionFlags |= 4;
    ObjectEvent_SetSpriteFlags(ObjectEvent_GetById(LOCALID_CRATE), 0);
    if (GameFlag_Get(FLAG_VAULT_CRATE_MOVED) != 0) {
      ObjectEvent_SetPosition(LOCALID_CRATE, PIXELS(676), PIXELS(411));
      MapGrid_CopyCellAttributes(101, 24, 3, 4, 41, 24);
    }
    break;
  case LOG_ROOMS_A_LOFT_FROM_CRATE_ROOM:
    FieldScript_WaitFrames(2);
    MapGrid_CopyCellsTo(54, 2, 44, 21, 2, 8);
    MapGrid_CopyCellsTo(54, 2, 44, 81, 2, 8);
    FieldScript_Start();
    // The loft: emptied chests after the thieves are jailed; else the tied-up
    // villager lies tied, groaning to SONG_FADE_OUT_BGM, or the thieves wait
    // for a rematch.
    if (GameFlag_Get(FLAG_VAULT_THIEVES_JAILED) != 0) {
      Import_ObjectEventSetAnimation(LOCALID_FIRST_CHEST, ANIM_WALK);
      Import_ObjectEventSetAnimation(LOCALID_SECOND_CHEST, ANIM_WALK);
      FieldScript_WaitFrames(1);
      Import_ObjectEventSetAnimation(LOCALID_THIRD_CHEST, ANIM_WALK);
      break;
    }
    Import_ObjectEventSetPosition(LOCALID_TIED_UP_VILLAGER, PIXELS(824),
                                  PIXELS(456));
    if (GameFlag_Get(FLAG_VAULT_THIEVES_ATTACKED) != 0) {
      LogRoomsA_ConfrontThievesAgain();
      FieldScript_End();
      break;
    }
    Import_ObjectEventSetAnimation(LOCALID_TIED_UP_VILLAGER, 7);
    LogRoomsA_OpenWindowTransition();
    Sound_PlaySong(SONG_FADE_OUT_BGM);
    FieldScript_SetMessageId(MsgVaultUhnnUhnn);
    LogRoomsA_ShowMessageAndWait(LOCALID_TIED_UP_VILLAGER, 10);
    FieldScript_End();
    break;
  case LOG_ROOMS_A_LOFT_AFTER_LOSING_TO_THIEVES:
    // Lost to the thieves: they gloat, and the party wakes up elsewhere.
    MapGrid_CopyCells(54, 2, 2, 8, 44, 21);
    MapGrid_CopyCells(54, 2, 2, 8, 44, 81);
    Import_ObjectEventSetPosition(LOCALID_TIED_UP_VILLAGER, PIXELS(824),
                                  PIXELS(456));
    LogRoomsA_LoseToThieves();
    Import_ScriptRequestWarp(LOG_ROOMS_A_TO_SANCTUM);
    break;
  case LOG_ROOMS_A_LOFT_AFTER_THIEVES_DEFEATED:
    // Won: the thieves are caught and the stolen goods handed back.
    MapGrid_CopyCells(54, 2, 2, 8, 44, 21);
    MapGrid_CopyCells(54, 2, 2, 8, 44, 81);
    if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0) {
      ObjectEvent_SetPosition(LOCALID_TIED_UP_VILLAGER, PIXELS(824),
                              PIXELS(456));
      LogRoomsA_DefeatThieves();
      LogRoomsA_ReturnStolenGoods();
      break;
    }
    Import_ObjectEventSetAnimation(LOCALID_FIRST_CHEST, ANIM_WALK);
    Import_ObjectEventSetAnimation(LOCALID_SECOND_CHEST, ANIM_WALK);
    FieldScript_WaitFrames(1);
    Import_ObjectEventSetAnimation(LOCALID_THIRD_CHEST, ANIM_WALK);
    break;
  }
  return 0;
}

/// Keeps both suspects facing the leader while they evade the party.
///
/// Run each frame as a map callback, from EventScript_ApproachSuspects until
/// EventScript_DiscussSuspiciousGuests, and again once their minds are read.
static void LogRoomsA_FaceSuspectsTowardLeader(void) {
  struct ObjectEvent *origin = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  struct ObjectEvent *first = ObjectEvent_GetById(LOCALID_SUSPECTED_LEADER);
  struct ObjectEvent *second = ObjectEvent_GetById(LOCALID_SUSPECTED_THIEF);

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
  struct ObjectEvent *objectEvent;
  s32 priorityFlags;

  // The party lies on the floor (animation 19) before the three thieves.
  FieldScript_Start();
  ObjectEvent_SetPosition(LOCALID_THIEF_LEADER, PIXELS(792), PIXELS(416));
  ObjectEvent_SetPosition(LOCALID_FIRST_THIEF, PIXELS(800), PIXELS(400));
  ObjectEvent_SetPosition(LOCALID_SECOND_THIEF, PIXELS(776), PIXELS(416));
  ObjectEvent_FaceDirection(LOCALID_THIEF_LEADER,
                            FACING_SOUTHEAST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_FIRST_THIEF, FACING_SOUTHEAST + FACING_STEP,
                            0);
  ObjectEvent_FaceDirection(LOCALID_SECOND_THIEF,
                            FACING_SOUTHEAST + FACING_STEP, 0);
  ObjectEvent_SetSpeed(LOCALID_FIRST_THIEF, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_SetSpeed(LOCALID_SECOND_THIEF, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_SetPosition(LOCALID_PARTY_LEADER, PIXELS(784), PIXELS(448));
  ObjectEvent_SetPosition(LOCALID_GARET, PIXELS(808), PIXELS(432));
  ObjectEvent_SetPosition(LOCALID_IVAN, PIXELS(776), PIXELS(432));
  Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_COLLAPSE);
  Import_ObjectEventSetAnimation(LOCALID_GARET, ANIM_COLLAPSE);
  Import_ObjectEventSetAnimation(LOCALID_IVAN, ANIM_COLLAPSE);
  priorityFlags = OBJ_EVENT_PRIORITY_UNDERFOOT;
  ObjectEvent_GetById(LOCALID_PARTY_LEADER)->priorityFlags = priorityFlags;
  ObjectEvent_GetById(LOCALID_GARET)->priorityFlags = priorityFlags;
  ObjectEvent_GetById(LOCALID_IVAN)->priorityFlags = priorityFlags;
  objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  ObjectEvent_SetSpriteFlags(objectEvent, 0);
  objectEvent = ObjectEvent_GetById(LOCALID_IVAN);
  ObjectEvent_SetSpriteFlags(objectEvent, 0);
  objectEvent = ObjectEvent_GetById(LOCALID_GARET);
  ObjectEvent_SetSpriteFlags(objectEvent, 0);
  ObjectEvent_FaceDirection(LOCALID_TIED_UP_VILLAGER,
                            FACING_NORTHWEST + FACING_STEP, 0);
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  FieldCamera_FollowObjectEvent(LOCALID_PARTY_LEADER, 0);
  FieldCamera_WaitForArrival();
  MapGrid_CenterOnFollowedPoint();
  LogRoomsA_OpenWindowTransition();
  FieldScript_WaitFrames(60);
  // "These kids are nothing to worry about." The tied-up villager groans.
  LogRoomsA_PlayAnimationAndWait(LOCALID_THIEF_LEADER, ANIM_NOD, 20);
  FieldScript_SetMessageId(MsgVaultTheseKidsNothingWorryAbout);
  LogRoomsA_ShowMessageAndWait(LOCALID_THIEF_LEADER, 30);
  LogRoomsA_ShowMessageAndWait(LOCALID_TIED_UP_VILLAGER, 30);
  // The two men step to the tied-up villager: he stays as their hostage.
  ObjectEvent_WalkTo(LOCALID_FIRST_THIEF, 808, 456);
  ObjectEvent_WalkTo(LOCALID_SECOND_THIEF, 792, 456);
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_SECOND_THIEF);
  ObjectEvent_FaceDirection(LOCALID_SECOND_THIEF, FACING_EAST, 0);
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_FIRST_THIEF);
  ObjectEvent_FaceDirection(LOCALID_FIRST_THIEF, FACING_EAST, 0);
  FieldScript_WaitFrames(30);
  LogRoomsA_PlayAnimationAndWait(LOCALID_FIRST_THIEF, ANIM_NOD, 20);
  LogRoomsA_ShowMessageAndWait(LOCALID_FIRST_THIEF, 20);
  LogRoomsA_FaceObjectEventAndWait(LOCALID_SECOND_THIEF, LOCALID_PARTY_LEADER,
                                   30);
  LogRoomsA_ShowMessageAndWait(LOCALID_SECOND_THIEF, 60);
  FieldScript_End();
}

/// Arranges the loft confrontation and restarts the thieves' battle. "They
/// won't go away, will they? They're back!"
///
/// Called by Scene_Initialize at LOG_ROOMS_A_LOFT_FROM_CRATE_ROOM once
/// FLAG_VAULT_THIEVES_ATTACKED is set. Starts the same battle as
/// EventScript_FreeRooferAndConfrontThieves.
static void LogRoomsA_ConfrontThievesAgain(void) {

  // The thieves stand over the tied-up villager as the party comes up.
  ObjectEvent_SetPosition(LOCALID_THIEF_LEADER, PIXELS(792), PIXELS(416));
  ObjectEvent_SetPosition(LOCALID_FIRST_THIEF, PIXELS(800), PIXELS(400));
  ObjectEvent_SetPosition(LOCALID_SECOND_THIEF, PIXELS(776), PIXELS(416));
  ObjectEvent_FaceDirection(LOCALID_THIEF_LEADER,
                            FACING_SOUTHEAST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_FIRST_THIEF, FACING_SOUTHEAST + FACING_STEP,
                            0);
  ObjectEvent_FaceDirection(LOCALID_SECOND_THIEF,
                            FACING_SOUTHEAST + FACING_STEP, 0);
  ObjectEvent_SetPosition(LOCALID_PARTY_LEADER, PIXELS(792), PIXELS(440));
  ObjectEvent_SetPosition(LOCALID_GARET, PIXELS(808), PIXELS(432));
  ObjectEvent_SetPosition(LOCALID_IVAN, PIXELS(792), PIXELS(456));
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTHWEST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_IVAN, FACING_NORTHWEST + FACING_STEP, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_TIED_UP_VILLAGER, LOCALID_THIEF_LEADER,
                              0);
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  FieldCamera_FollowObjectEvent(LOCALID_PARTY_LEADER, 0);
  FieldCamera_WaitForArrival();
  MapGrid_CenterOnFollowedPoint();
  WaitFrames(1);
  gFieldWork->transitionFrames = 32;
  LogRoomsA_OpenWindowTransition();
  FieldScript_WaitFrames(60);
  FieldScript_SetMessageId(MsgVaultWontGoAway);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_FIRST_THIEF, 1);
  FieldScript_WaitFrames(20);
  LogRoomsA_ShowMessageAndWait(LOCALID_FIRST_THIEF, 30);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_SECOND_THIEF, 1);
  FieldScript_WaitFrames(20);
  Script_ShowMessage(LOCALID_SECOND_THIEF, 0);
  LogRoomsA_FaceEachOtherAndWait(LOCALID_THIEF_LEADER, LOCALID_FIRST_THIEF, 30);
  Import_ObjectEventSetAnimation(LOCALID_THIEF_LEADER, ANIM_NOD);
  LogRoomsA_PlayAnimationAndWait(LOCALID_FIRST_THIEF, ANIM_NOD, 30);
  LogRoomsA_FaceEachOtherAndWait(LOCALID_THIEF_LEADER, LOCALID_SECOND_THIEF,
                                 30);
  Import_ObjectEventSetAnimation(LOCALID_THIEF_LEADER, ANIM_NOD);
  LogRoomsA_PlayAnimationAndWait(LOCALID_SECOND_THIEF, ANIM_NOD, 40);
  ObjectEvent_FaceDirection(LOCALID_THIEF_LEADER,
                            FACING_SOUTHEAST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_FIRST_THIEF, FACING_SOUTHEAST + FACING_STEP,
                            0);
  ObjectEvent_FaceDirection(LOCALID_SECOND_THIEF,
                            FACING_SOUTHEAST + FACING_STEP, 0);
  FieldScript_WaitFrames(20);
  // "We'll make you sorry you were ever born! Get 'em!!" and the battle.
  LogRoomsA_PlayAnimationAndWait(LOCALID_THIEF_LEADER, ANIM_SHAKE_HEAD, 20);
  Script_ShowMessage(LOCALID_THIEF_LEADER, 0);
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
  FieldScript_SetNextDestination((s32)&SceneId_LogRoomsA,
                                 LOG_ROOMS_A_LOFT_AFTER_THIEVES_DEFEATED);
  FieldScript_SetDefeatDestination((s32)&SceneId_LogRoomsA,
                                   LOG_ROOMS_A_LOFT_AFTER_LOSING_TO_THIEVES);
  Battle_SetType(BATTLE_TYPE_SCRIPTED);
  Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED_VALE_VAULT, 5);
}

/// Counts the Venus Djinn Ivan holds, which he gives back before he leaves the
/// party.
static s32 LogRoomsA_GetIvanVenusDjinnCount(void) {
  return Unit_Get(IVAN)->djinnHeldCounts[ELEMENT_VENUS];
}

#include "common/vault_djinn.h"

/// Stages the defeated thieves and gathers the townspeople in the loft.
///
/// Called by Scene_Initialize at LOG_ROOMS_A_LOFT_AFTER_THIEVES_DEFEATED,
/// the battle's victory destination, unless the party is only coming back to
/// where it was; then LogRoomsA_ReturnStolenGoods follows.
static void LogRoomsA_DefeatThieves(void) {
  struct ObjectEvent *objectEvent;
  struct FieldWork *work;
  const s32 *actions;
  struct Sprite *thirdThiefSprite;

  objectEvent = ObjectEvent_GetById(LOCALID_SECOND_THIEF);
  thirdThiefSprite = objectEvent->sprite;
  // The beaten thieves lie heaving (animation 9 and the action table's
  // squash) before the party.
  FieldScript_Start();
  ObjectEvent_SetPosition(LOCALID_THIEF_LEADER, PIXELS(792), PIXELS(416));
  ObjectEvent_SetPosition(LOCALID_FIRST_THIEF, PIXELS(800), PIXELS(400));
  ObjectEvent_SetPosition(LOCALID_SECOND_THIEF, PIXELS(776), PIXELS(392));
  objectEvent = ObjectEvent_GetById(LOCALID_THIEF_LEADER);
  ObjectEvent_SetSpriteFlags(objectEvent, 0);
  objectEvent = ObjectEvent_GetById(LOCALID_FIRST_THIEF);
  ObjectEvent_SetSpriteFlags(objectEvent, 0);
  objectEvent = ObjectEvent_GetById(LOCALID_SECOND_THIEF);
  ObjectEvent_SetSpriteFlags(objectEvent, 0);
  Import_ObjectEventSetAnimation(LOCALID_THIEF_LEADER, 9);
  Import_ObjectEventSetAnimation(LOCALID_FIRST_THIEF, 9);
  Import_ObjectEventSetAnimation(LOCALID_SECOND_THIEF, 9);
  ObjectEvent_GetById(LOCALID_SECOND_THIEF)->priorityFlags &=
      ~OBJ_EVENT_PRIORITY_AUTOMATIC;
  thirdThiefSprite->image.priority = 3;
  actions = gLogRoomsA_ActionTable;
  ObjectEvent_SetActionScript(LOCALID_THIEF_LEADER, actions);
  ObjectEvent_SetPosition(LOCALID_PARTY_LEADER, PIXELS(792), PIXELS(440));
  ObjectEvent_SetPosition(LOCALID_GARET, PIXELS(808), PIXELS(432));
  ObjectEvent_SetPosition(LOCALID_IVAN, PIXELS(776), PIXELS(440));
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTHWEST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_IVAN, FACING_NORTHWEST + FACING_STEP, 0);
  ObjectEvent_SetSpeed(LOCALID_TIED_UP_VILLAGER, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_FaceDirection(LOCALID_TIED_UP_VILLAGER,
                            FACING_NORTHWEST + FACING_STEP, 0);
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  FieldCamera_FollowObjectEvent(LOCALID_PARTY_LEADER, 0);
  FieldCamera_WaitForArrival();
  MapGrid_CenterOnFollowedPoint();
  LogRoomsA_OpenWindowTransition();
  ObjectEvent_SetActionScript(LOCALID_FIRST_THIEF, actions);
  FieldScript_WaitFrames(30);
  ObjectEvent_SetActionScript(LOCALID_SECOND_THIEF, actions);
  FieldScript_WaitFrames(30);
  // "They... They got us..." The tied-up villager thought they were goners; he
  // slips past Garet, cheers the win and runs for the mayor.
  FieldScript_SetMessageId(MsgVaultTheyTheyGotUs);
  LogRoomsA_ShowMessageAndWait(LOCALID_THIEF_LEADER, 20);
  ObjectEvent_ShowEmote(LOCALID_TIED_UP_VILLAGER, EMOTE_IN_FRONT | EMOTE_WINCE,
                        0);
  FieldScript_WaitFrames(60);
  LogRoomsA_ShowMessageAndWait(LOCALID_TIED_UP_VILLAGER, 20);
  ObjectEvent_WalkToAndWait(LOCALID_TIED_UP_VILLAGER, 808, 456);
  ObjectEvent_SetSpeed(LOCALID_GARET, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_WalkToAndWait(LOCALID_GARET, 792, 432);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_EAST, 0);
  ObjectEvent_WalkToAndWait(LOCALID_TIED_UP_VILLAGER, 808, 408);
  ObjectEvent_FaceDirection(LOCALID_TIED_UP_VILLAGER, FACING_WEST, 0);
  ObjectEvent_WalkToAndWait(LOCALID_GARET, 808, 432);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTH, 0);
  FieldScript_WaitFrames(20);
  LogRoomsA_PlayAnimationAndWait(LOCALID_TIED_UP_VILLAGER, ANIM_NOD, 20);
  LogRoomsA_ShowMessageAndWait(LOCALID_TIED_UP_VILLAGER, 20);
  ObjectEvent_WalkToAndWait(LOCALID_TIED_UP_VILLAGER, 768, 408);
  FieldScript_WaitFrames(20);
  ObjectEvent_FaceObjectEvent(LOCALID_TIED_UP_VILLAGER, LOCALID_PARTY_LEADER,
                              0);
  ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_TIED_UP_VILLAGER,
                              0);
  ObjectEvent_FaceObjectEvent(LOCALID_GARET, LOCALID_TIED_UP_VILLAGER, 0);
  LogRoomsA_FaceObjectEventAndWait(LOCALID_IVAN, LOCALID_TIED_UP_VILLAGER, 40);
  LogRoomsA_ShowMessageAndWait(LOCALID_TIED_UP_VILLAGER, 30);
  Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_NOD);
  Import_ObjectEventSetAnimation(LOCALID_GARET, ANIM_NOD);
  LogRoomsA_PlayAnimationAndWait(LOCALID_IVAN, ANIM_NOD, 20);
  ObjectEvent_WalkToAndWait(LOCALID_TIED_UP_VILLAGER, 744, 408);
  FieldScript_WaitFrames(50);
  // The thieves grumble; Garet: stealing is wrong, evil never prevails.
  ObjectMotion_SetVarCbAndRefresh(LOCALID_FIRST_THIEF, 2);
  LogRoomsA_ShowMessageAndWait(LOCALID_FIRST_THIEF, 20);
  ObjectEvent_SetActionScript(LOCALID_FIRST_THIEF, actions);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 1);
  FieldScript_WaitFrames(20);
  ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_FIRST_THIEF, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_GARET, LOCALID_FIRST_THIEF, 0);
  LogRoomsA_FaceObjectEventAndWait(LOCALID_IVAN, LOCALID_FIRST_THIEF, 20);
  LogRoomsA_ShowMessageAndWait(LOCALID_GARET, 20);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_SECOND_THIEF, 2);
  LogRoomsA_ShowMessageAndWait(LOCALID_SECOND_THIEF, 30);
  ObjectEvent_SetActionScript(LOCALID_SECOND_THIEF, actions);
  ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
  FieldScript_WaitFrames(60);
  LogRoomsA_ShowMessageAndWait(LOCALID_GARET, 30);
  // Ivan will take the rod to Hammet in Lunpa, and asks what the two will do:
  // go after Saturos and the others.
  LogRoomsA_FaceObjectEventAndWait(LOCALID_IVAN, LOCALID_PARTY_LEADER, 30);
  LogRoomsA_FaceObjectEventAndWait(LOCALID_PARTY_LEADER, LOCALID_IVAN, 30);
  LogRoomsA_PlayAnimationAndWait(LOCALID_IVAN, ANIM_NOD, 20);
  LogRoomsA_ShowMessageAndWait(LOCALID_IVAN, 20);
  LogRoomsA_FaceObjectEventAndWait(LOCALID_GARET, LOCALID_IVAN, 30);
  LogRoomsA_PlayAnimationAndWait(LOCALID_PARTY_LEADER, ANIM_NOD, 30);
  LogRoomsA_ShowMessageAndWait(LOCALID_IVAN, 20);
  ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  FieldScript_WaitFrames(60);
  LogRoomsA_ShowMessageAndWait(LOCALID_GARET, 30);
  ObjectEvent_ShowEmote(LOCALID_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  FieldScript_WaitFrames(60);
  LogRoomsA_FaceEachOtherAndWait(LOCALID_PARTY_LEADER, LOCALID_GARET, 20);
  Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_NOD);
  LogRoomsA_PlayAnimationAndWait(LOCALID_GARET, ANIM_NOD, 20);
  // Ivan reads Isaac's mind. Garet: "Shouldn't we stop him?" Yes: too late,
  // he's done. No: "If you say so."
  LogRoomsA_StartMindRead(LOCALID_IVAN, LOCALID_PARTY_LEADER);
  FieldEffect_RestoreMindReadPalette();
  ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_IVAN, 0);
  LogRoomsA_FaceObjectEventAndWait(LOCALID_GARET, LOCALID_IVAN, 10);
  ObjectMotion_SetVariantCallback(LOCALID_PARTY_LEADER, 1);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 1);
  FieldScript_WaitFrames(20);
  ObjectEvent_SetAttachedEffect(LOCALID_GARET, ATTACHED_EFFECT_IN_FRONT |
                                                   ATTACHED_EFFECT_ONCE);
  FieldScript_WaitFrames(60);
  LogRoomsA_FaceObjectEventAndWait(LOCALID_PARTY_LEADER, LOCALID_GARET, 10);
  Script_OpenMessage(LOCALID_GARET, 0);
  if (FieldScript_AskYesNoWithObjectEventAnimation(0, 0) == 0) {
    FieldScript_WaitFrames(20);
    ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_IVAN, 0);
    LogRoomsA_FaceObjectEventAndWait(LOCALID_GARET, LOCALID_IVAN, 20);
    LogRoomsA_EndMindRead();
    ObjectMotion_SetVariantCallback(LOCALID_PARTY_LEADER, 1);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 1);
    FieldScript_WaitFrames(20);
    LogRoomsA_ShowMessageAndWait(LOCALID_GARET, 20);
  } else {
    gFieldWork->message++;
    ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    FieldScript_WaitFrames(60);
    LogRoomsA_ShowMessageAndWait(LOCALID_GARET, 20);
    ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_IVAN, 0);
    LogRoomsA_FaceObjectEventAndWait(LOCALID_GARET, LOCALID_IVAN, 20);
    LogRoomsA_EndMindRead();
    ObjectMotion_SetVariantCallback(LOCALID_PARTY_LEADER, 1);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 1);
    FieldScript_WaitFrames(20);
  }
  // "I see... So that's what happened..." The tied-up villager comes back with
  // the mayor and two townsmen.
  ObjectEvent_ShowEmote(LOCALID_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  FieldScript_WaitFrames(60);
  LogRoomsA_PlayAnimationAndWait(LOCALID_IVAN, ANIM_SHAKE_HEAD, 20);
  FieldScript_SetMessageId(MsgVaultSeeThatsHappened);
  LogRoomsA_ShowMessageAndWait(LOCALID_IVAN, 20);
  Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_NOD);
  LogRoomsA_PlayAnimationAndWait(LOCALID_GARET, ANIM_NOD, 40);
  ObjectEvent_WalkToAndWait(LOCALID_TIED_UP_VILLAGER, 808, 408);
  ObjectEvent_FaceDirection(LOCALID_TIED_UP_VILLAGER, FACING_WEST, 0);
  FieldScript_WaitFrames(30);
  ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_TIED_UP_VILLAGER,
                              0);
  ObjectEvent_FaceObjectEvent(LOCALID_GARET, LOCALID_TIED_UP_VILLAGER, 0);
  LogRoomsA_FaceObjectEventAndWait(LOCALID_IVAN, LOCALID_TIED_UP_VILLAGER, 20);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_TIED_UP_VILLAGER, 1);
  FieldScript_WaitFrames(20);
  LogRoomsA_ShowMessageAndWait(LOCALID_TIED_UP_VILLAGER, 20);
  ObjectEvent_SetSpeed(LOCALID_LOFT_MAYOR, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_SetSpeed(LOCALID_LOFT_ESCORT, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_SetSpeed(LOCALID_STRONGMAN_OPENING_THE_CHESTS, PIXELS(4) / 5,
                       PIXELS(2) / 5);
  ObjectEvent_SetPosition(LOCALID_LOFT_MAYOR, PIXELS(744), PIXELS(408));
  ObjectEvent_WalkToAndWait(LOCALID_LOFT_MAYOR, 768, 408);
  LogRoomsA_FaceObjectEventAndWait(LOCALID_LOFT_MAYOR, LOCALID_THIEF_LEADER,
                                   30);
  ObjectEvent_SetPosition(LOCALID_LOFT_ESCORT, PIXELS(744), PIXELS(408));
  ObjectEvent_WalkToAndWait(LOCALID_LOFT_ESCORT, 768, 408);
  ObjectEvent_SetPosition(LOCALID_STRONGMAN_OPENING_THE_CHESTS, PIXELS(744),
                          PIXELS(408));
  ObjectEvent_WalkTo(LOCALID_STRONGMAN_OPENING_THE_CHESTS, 784, 400);
  ObjectEvent_WalkToAndWait(LOCALID_LOFT_ESCORT, 776, 424);
  ObjectMotion_CommitCurrentPositionAndActivate(
      LOCALID_STRONGMAN_OPENING_THE_CHESTS);
  ObjectEvent_FaceObjectEvent(LOCALID_LOFT_ESCORT, LOCALID_THIEF_LEADER, 0);
  LogRoomsA_FaceObjectEventAndWait(LOCALID_STRONGMAN_OPENING_THE_CHESTS,
                                   LOCALID_THIEF_LEADER, 20);
  ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_LOFT_MAYOR, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_GARET, LOCALID_LOFT_MAYOR, 0);
  LogRoomsA_FaceObjectEventAndWait(LOCALID_IVAN, LOCALID_LOFT_MAYOR, 20);
  // The mayor scolds them for stealing in a disaster; the thieves talk back.
  LogRoomsA_PlayAnimationAndWait(LOCALID_LOFT_MAYOR, ANIM_SHAKE_HEAD, 20);
  LogRoomsA_ShowMessageAndWait(LOCALID_LOFT_MAYOR, 20);
  LogRoomsA_ShowMessageAndWait(LOCALID_FIRST_THIEF, 20);
  LogRoomsA_ShowMessageAndWait(LOCALID_SECOND_THIEF, 30);
  // "What should we do with them?" Locked up for a long, long time; the two
  // townsmen rouse the thieves to tie them up.
  LogRoomsA_FaceEachOtherAndWait(LOCALID_LOFT_MAYOR, LOCALID_LOFT_ESCORT, 20);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_LOFT_ESCORT, 1);
  FieldScript_WaitFrames(20);
  LogRoomsA_ShowMessageAndWait(LOCALID_LOFT_ESCORT, 20);
  LogRoomsA_PlayAnimationAndWait(LOCALID_LOFT_MAYOR, ANIM_NOD, 30);
  LogRoomsA_FaceEachOtherAndWait(LOCALID_LOFT_MAYOR,
                                 LOCALID_STRONGMAN_OPENING_THE_CHESTS, 20);
  LogRoomsA_PlayAnimationAndWait(LOCALID_LOFT_MAYOR, ANIM_NOD, 30);
  LogRoomsA_FaceObjectEventAndWait(LOCALID_LOFT_MAYOR, LOCALID_THIEF_LEADER,
                                   20);
  LogRoomsA_ShowMessageAndWait(LOCALID_LOFT_MAYOR, 20);
  Import_ObjectEventSetAnimation(LOCALID_LOFT_ESCORT, ANIM_NOD);
  LogRoomsA_PlayAnimationAndWait(LOCALID_STRONGMAN_OPENING_THE_CHESTS, ANIM_NOD,
                                 20);
  LogRoomsA_FaceEachOtherAndWait(LOCALID_LOFT_ESCORT,
                                 LOCALID_STRONGMAN_OPENING_THE_CHESTS, 20);
  Import_ObjectEventSetAnimation(LOCALID_LOFT_ESCORT, ANIM_NOD);
  LogRoomsA_PlayAnimationAndWait(LOCALID_STRONGMAN_OPENING_THE_CHESTS, ANIM_NOD,
                                 20);
  ObjectEvent_WalkTo(LOCALID_STRONGMAN_OPENING_THE_CHESTS, 792, 392);
  ObjectEvent_WalkToAndWait(LOCALID_LOFT_ESCORT, 784, 400);
  ObjectEvent_FaceObjectEvent(LOCALID_LOFT_ESCORT, LOCALID_SECOND_THIEF, 0);
  ObjectMotion_CommitCurrentPositionAndActivate(
      LOCALID_STRONGMAN_OPENING_THE_CHESTS);
  LogRoomsA_FaceObjectEventAndWait(LOCALID_STRONGMAN_OPENING_THE_CHESTS,
                                   LOCALID_FIRST_THIEF, 20);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_LOFT_ESCORT, 1);
  FieldScript_WaitFrames(20);
  LogRoomsA_ShowMessageAndWait(LOCALID_LOFT_ESCORT, 20);
  LogRoomsA_PlayAnimationAndWait(LOCALID_STRONGMAN_OPENING_THE_CHESTS,
                                 ANIM_SHAKE_HEAD, 20);
  LogRoomsA_ShowMessageAndWait(LOCALID_STRONGMAN_OPENING_THE_CHESTS, 30);
  LogRoomsA_FaceEachOtherAndWait(LOCALID_LOFT_ESCORT, LOCALID_PARTY_LEADER, 20);
  LogRoomsA_ShowMessageAndWait(LOCALID_LOFT_ESCORT, 20);
  Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_NOD);
  Import_ObjectEventSetAnimation(LOCALID_GARET, ANIM_NOD);
  LogRoomsA_PlayAnimationAndWait(LOCALID_IVAN, ANIM_NOD, 50);
  // The window closes while the thieves are tied up.
  work = gFieldWork;
  work->transitionFrames = 30;
  work->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
  FieldScript_FinishScreenTransition();
  FieldScript_WaitTransitionFrames();
  FieldScript_WaitFrames(60);
  FieldScript_End();
}

/// Returns the stolen goods and rod, thanks the party, and clears the loft.
///
/// Called by Scene_Initialize after LogRoomsA_DefeatThieves. Sets
/// FLAG_VAULT_THIEVES_JAILED; the thieves are led away, the mayor's urn and
/// the Sanctum's gold statue come out of the chests, Ivan takes the Shaman's
/// Rod, gives back any Venus Djinn and leaves the party, and every object event
/// of the loft is cleared away.
static void LogRoomsA_ReturnStolenGoods(void) {
  struct ObjectEvent *temporaryObjectEvent;
  struct ObjectEvent *objectEvent;
  s32 aftermath;
  s32 rod;
  struct FieldWork *work;

  work = gFieldWork;
  GameFlag_Set(FLAG_VAULT_THIEVES_JAILED);
  // With the screen still closed, the chests go back along the wall and the
  // thieves stand tied before the mayor.
  FieldScript_Start();
  {
    struct ObjectEvent *objectEvent = ObjectEvent_GetById(LOCALID_SECOND_THIEF);

    objectEvent->priorityFlags =
        OBJ_EVENT_PRIORITY_AUTOMATIC | objectEvent->priorityFlags;
  }
  ObjectEvent_MoveToAndWait(LOCALID_FIRST_CHEST, 872, 425);
  ObjectEvent_MoveToAndWait(LOCALID_SECOND_CHEST, 872, 409);
  ObjectEvent_MoveToAndWait(LOCALID_THIRD_CHEST, 872, 377);
  ObjectEvent_SetPosition(LOCALID_FIRST_THIEF, PIXELS(776), PIXELS(392));
  ObjectEvent_SetPosition(LOCALID_THIEF_LEADER, PIXELS(792), PIXELS(392));
  ObjectEvent_SetPosition(LOCALID_SECOND_THIEF, PIXELS(808), PIXELS(392));
  Import_ObjectEventSetAnimation(LOCALID_THIEF_LEADER, 5);
  Import_ObjectEventSetAnimation(LOCALID_FIRST_THIEF, 5);
  Import_ObjectEventSetAnimation(LOCALID_SECOND_THIEF, 5);
  ObjectEvent_FaceObjectEvent(LOCALID_FIRST_THIEF, LOCALID_PARTY_LEADER, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_THIEF_LEADER, LOCALID_PARTY_LEADER, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_SECOND_THIEF, LOCALID_PARTY_LEADER, 0);
  objectEvent = ObjectEvent_GetById(LOCALID_THIEF_LEADER);
  ObjectEvent_SetSpriteFlags(objectEvent, SPRITE_FLAG_SHADOW);
  objectEvent = ObjectEvent_GetById(LOCALID_FIRST_THIEF);
  ObjectEvent_SetSpriteFlags(objectEvent, SPRITE_FLAG_SHADOW);
  objectEvent = ObjectEvent_GetById(LOCALID_SECOND_THIEF);
  ObjectEvent_SetSpriteFlags(objectEvent, SPRITE_FLAG_SHADOW);
  ObjectEvent_SetPosition(LOCALID_LOFT_ESCORT, PIXELS(768), PIXELS(408));
  ObjectEvent_SetPosition(LOCALID_STRONGMAN_OPENING_THE_CHESTS, PIXELS(768),
                          PIXELS(424));
  ObjectEvent_MoveToAndWait(LOCALID_LOFT_MAYOR, 784, 424);
  ObjectEvent_SetPosition(LOCALID_TIED_UP_VILLAGER, PIXELS(808), PIXELS(408));
  ObjectEvent_FaceObjectEvent(LOCALID_LOFT_ESCORT, LOCALID_LOFT_MAYOR, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_TIED_UP_VILLAGER, LOCALID_LOFT_MAYOR, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_STRONGMAN_OPENING_THE_CHESTS,
                              LOCALID_THIEF_LEADER, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_LOFT_MAYOR, LOCALID_THIEF_LEADER, 0);
  ObjectEvent_SetPosition(LOCALID_PARTY_LEADER, PIXELS(792), PIXELS(440));
  ObjectEvent_SetPosition(LOCALID_GARET, PIXELS(808), PIXELS(440));
  ObjectEvent_SetPosition(LOCALID_IVAN, PIXELS(776), PIXELS(440));
  ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_THIEF_LEADER, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_GARET, LOCALID_THIEF_LEADER, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_IVAN, LOCALID_THIEF_LEADER, 0);
  gFieldWork->transitionFrames = 30;
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
  FieldScript_StartScreenTransition();
  FieldScript_WaitTransitionFrames();
  FieldScript_WaitFrames(40);
  // "Isaac, right? I won't forget this!" "Take them away!"
  ObjectMotion_SetVarCbAndRefresh(LOCALID_THIEF_LEADER, 2);
  aftermath = MsgVaultYouIsaacRightWontForget;
  FieldScript_SetMessageId(aftermath);
  LogRoomsA_ShowMessageAndWait(LOCALID_THIEF_LEADER, 20);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_LOFT_MAYOR, 1);
  FieldScript_WaitFrames(20);
  LogRoomsA_PlayAnimationAndWait(LOCALID_LOFT_MAYOR, ANIM_SHAKE_HEAD, 20);
  LogRoomsA_ShowMessageAndWait(LOCALID_LOFT_MAYOR, 20);
  Import_ObjectEventSetAnimation(LOCALID_LOFT_ESCORT, ANIM_NOD);
  LogRoomsA_PlayAnimationAndWait(LOCALID_TIED_UP_VILLAGER, ANIM_NOD, 20);
  ObjectMotion_SetVariantCallback(LOCALID_FIRST_THIEF, 2);
  ObjectMotion_SetVariantCallback(LOCALID_SECOND_THIEF, 2);
  FieldScript_WaitFrames(60);
  // The escort leads the three thieves down the ladder, the tied-up villager
  // after them.
  ObjectEvent_SetSpeed(LOCALID_LOFT_ESCORT, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_WalkTo(LOCALID_LOFT_ESCORT, 746, 408);
  ObjectEvent_FaceDirection(LOCALID_LOFT_MAYOR, FACING_NORTHWEST + FACING_STEP,
                            0);
  ObjectEvent_FaceDirection(LOCALID_STRONGMAN_OPENING_THE_CHESTS,
                            FACING_NORTHWEST + FACING_STEP, 0);
  ObjectEvent_SetActionScript(LOCALID_FIRST_THIEF, gLogRoomsA_VaultScriptB);
  FieldScript_WaitFrames(20);
  ObjectEvent_SetActionScript(LOCALID_THIEF_LEADER, gLogRoomsA_VaultScriptB);
  FieldScript_WaitFrames(15);
  ObjectEvent_SetActionScript(LOCALID_SECOND_THIEF, gLogRoomsA_VaultScriptB);
  FieldScript_WaitFrames(35);
  ObjectEvent_SetActionScript(LOCALID_TIED_UP_VILLAGER,
                              gLogRoomsA_VaultScriptA);
  FieldScript_WaitFrames(20);
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_LOFT_ESCORT);
  ObjectEvent_SetPosition(LOCALID_LOFT_ESCORT, 0, 0);
  FieldScript_WaitFrames(40);
  // The mayor and the villager opening the chests thank the party.
  ObjectEvent_SetSpeed(LOCALID_LOFT_MAYOR, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_SetSpeed(LOCALID_STRONGMAN_OPENING_THE_CHESTS, PIXELS(4) / 5,
                       PIXELS(2) / 5);
  ObjectEvent_WalkToAndWait(LOCALID_LOFT_MAYOR, 784, 408);
  ObjectEvent_FaceObjectEvent(LOCALID_LOFT_MAYOR, LOCALID_PARTY_LEADER, 0);
  ObjectEvent_WalkToAndWait(LOCALID_STRONGMAN_OPENING_THE_CHESTS, 768, 408);
  LogRoomsA_FaceObjectEventAndWait(LOCALID_STRONGMAN_OPENING_THE_CHESTS,
                                   LOCALID_PARTY_LEADER, 20);
  LogRoomsA_PlayAnimationAndWait(LOCALID_LOFT_MAYOR, ANIM_NOD, 20);
  LogRoomsA_ShowMessageAndWait(LOCALID_LOFT_MAYOR, 20);
  LogRoomsA_PlayAnimationAndWait(LOCALID_STRONGMAN_OPENING_THE_CHESTS, ANIM_NOD,
                                 20);
  ObjectEvent_FaceDirection(LOCALID_STRONGMAN_OPENING_THE_CHESTS,
                            FACING_SOUTHEAST, 10);
  LogRoomsA_ShowMessageAndWait(LOCALID_STRONGMAN_OPENING_THE_CHESTS, 20);
  LogRoomsA_FaceEachOtherAndWait(LOCALID_PARTY_LEADER, LOCALID_GARET, 50);
  LogRoomsA_FaceEachOtherAndWait(LOCALID_PARTY_LEADER, LOCALID_IVAN, 50);
  ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_LOFT_MAYOR, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_GARET, LOCALID_LOFT_MAYOR, 0);
  LogRoomsA_FaceObjectEventAndWait(LOCALID_IVAN, LOCALID_LOFT_MAYOR, 20);
  Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_NOD);
  Import_ObjectEventSetAnimation(LOCALID_GARET, ANIM_NOD);
  LogRoomsA_PlayAnimationAndWait(LOCALID_IVAN, ANIM_NOD, 50);
  // "Let's take a look at what they stole." The two go round to the chests.
  ObjectEvent_LinkPair(LOCALID_LOFT_MAYOR, LOCALID_STRONGMAN_OPENING_THE_CHESTS,
                       0);
  LogRoomsA_PlayAnimationAndWait(LOCALID_LOFT_MAYOR, ANIM_NOD, 20);
  LogRoomsA_ShowMessageAndWait(LOCALID_LOFT_MAYOR, 20);
  ObjectEvent_SetActionScript(LOCALID_STRONGMAN_OPENING_THE_CHESTS,
                              gLogRoomsA_VaultScriptC);
  FieldScript_WaitFrames(50);
  ObjectEvent_SetActionScript(LOCALID_LOFT_MAYOR, gLogRoomsA_VaultScriptD);
  ObjectEvent_SetSpeed(LOCALID_GARET, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_SetSpeed(LOCALID_IVAN, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_WalkToAndWait(LOCALID_GARET, 792, 456);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTH + FACING_STEP, 0);
  FieldScript_WaitFrames(30);
  ObjectEvent_WalkToAndWait(LOCALID_IVAN, 792, 408);
  ObjectEvent_FaceDirection(LOCALID_IVAN, FACING_EAST, 0);
  ObjectEvent_WalkToAndWait(LOCALID_GARET, 808, 456);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTH + FACING_STEP, 0);
  FieldScript_WaitFrames(100);
  LogRoomsA_FaceObjectEventAndWait(LOCALID_STRONGMAN_OPENING_THE_CHESTS,
                                   LOCALID_LOFT_MAYOR, 60);
  LogRoomsA_FaceObjectEventAndWait(LOCALID_LOFT_MAYOR,
                                   LOCALID_STRONGMAN_OPENING_THE_CHESTS, 40);
  LogRoomsA_PlayAnimationAndWait(LOCALID_LOFT_MAYOR, ANIM_NOD, 40);
  ObjectEvent_FaceDirection(LOCALID_LOFT_MAYOR, FACING_EAST, 0);
  FieldScript_WaitFrames(20);
  // The mayor opens the first chest and his urn rises out of it.
  ObjectMotion_SetVarCbAndRefresh(LOCALID_LOFT_MAYOR, 2);
  Script_PlaySong(SE_CHEST_OPEN);
  Import_ObjectEventSetAnimation(LOCALID_FIRST_CHEST, ANIM_SHAKE_HEAD);
  ObjectEvent_SetPosition(LOCALID_FOUND_URN, PIXELS(872), PIXELS(424));
  ObjectMotion_SetActionVariant(LOCALID_FOUND_URN, 1);
  ObjectEvent_SetSpeed(LOCALID_FOUND_URN, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_SetDestinationOffset(LOCALID_FOUND_URN, 0, -8);
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_FOUND_URN);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_FOUND_URN, 2);
  FieldScript_WaitFrames(60);
  FieldMessage_Show((aftermath + 5), 1);
  Import_ObjectEventSetAnimation(LOCALID_FIRST_CHEST, ANIM_WALK);
  ObjectEvent_SetPosition(LOCALID_FOUND_URN, 0, 0);
  gFieldWork->message++;
  ObjectMotion_SetVarCbAndRefresh(LOCALID_STRONGMAN_OPENING_THE_CHESTS, 1);
  LogRoomsA_ShowMessageAndWait(LOCALID_STRONGMAN_OPENING_THE_CHESTS, 20);
  LogRoomsA_FaceEachOtherAndWait(LOCALID_PARTY_LEADER, LOCALID_GARET, 40);
  LogRoomsA_FaceObjectEventAndWait(LOCALID_LOFT_MAYOR,
                                   LOCALID_STRONGMAN_OPENING_THE_CHESTS, 20);
  LogRoomsA_PlayAnimationAndWait(LOCALID_LOFT_MAYOR, ANIM_NOD, 20);
  LogRoomsA_ShowMessageAndWait(LOCALID_LOFT_MAYOR, 30);
  ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER,
                              LOCALID_STRONGMAN_OPENING_THE_CHESTS, 0);
  LogRoomsA_FaceObjectEventAndWait(LOCALID_GARET,
                                   LOCALID_STRONGMAN_OPENING_THE_CHESTS, 40);
  ObjectEvent_FaceDirection(LOCALID_STRONGMAN_OPENING_THE_CHESTS, FACING_EAST,
                            0);
  FieldScript_WaitFrames(40);
  // The villager opening the chests opens the second: the gold statue stolen
  // from the Sanctum.
  ObjectMotion_SetVarCbAndRefresh(LOCALID_STRONGMAN_OPENING_THE_CHESTS, 2);
  Script_PlaySong(SE_CHEST_OPEN);
  Import_ObjectEventSetAnimation(LOCALID_SECOND_CHEST, ANIM_SHAKE_HEAD);
  ObjectEvent_GetById(LOCALID_FOUND_STATUE)->motionFlags = 0;
  ObjectMotion_SetActionVariant(LOCALID_FOUND_STATUE, 1);
  ObjectEvent_SetPosition(LOCALID_FOUND_STATUE, PIXELS(872), PIXELS(408));
  ObjectEvent_SetSpeed(LOCALID_FOUND_STATUE, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_SetDestinationOffset(LOCALID_FOUND_STATUE, 0, -8);
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_FOUND_STATUE);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_FOUND_STATUE, 2);
  FieldScript_WaitFrames(60);
  FieldMessage_Show((aftermath + 8), 1);
  Import_ObjectEventSetAnimation(LOCALID_SECOND_CHEST, ANIM_WALK);
  ObjectEvent_SetPosition(LOCALID_FOUND_STATUE, 0, 0);
  gFieldWork->message++;
  ObjectEvent_ShowEmote(LOCALID_LOFT_MAYOR, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  FieldScript_WaitFrames(60);
  LogRoomsA_ShowMessageAndWait(LOCALID_LOFT_MAYOR, 20);
  ObjectMotion_SetVariantCallback(LOCALID_PARTY_LEADER, 2);
  ObjectMotion_SetVariantCallback(LOCALID_GARET, 2);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_IVAN, 2);
  FieldScript_WaitFrames(20);
  LogRoomsA_PlayAnimationAndWait(LOCALID_STRONGMAN_OPENING_THE_CHESTS, ANIM_NOD,
                                 50);
  LogRoomsA_FaceObjectEventAndWait(LOCALID_LOFT_MAYOR, LOCALID_PARTY_LEADER,
                                   20);
  LogRoomsA_ShowMessageAndWait(LOCALID_LOFT_MAYOR, 30);
  // The villager opening the chests finds another chest; the mayor asks whether
  // it is what Ivan was looking for, and he can return to Master Hammet.
  ObjectEvent_FaceDirection(LOCALID_STRONGMAN_OPENING_THE_CHESTS,
                            FACING_NORTH + FACING_STEP, 0);
  FieldScript_WaitFrames(30);
  ObjectEvent_ShowEmote(LOCALID_STRONGMAN_OPENING_THE_CHESTS,
                        EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  FieldScript_WaitFrames(60);
  ObjectEvent_WalkToAndWait(LOCALID_STRONGMAN_OPENING_THE_CHESTS, 856, 376);
  FieldScript_WaitFrames(20);
  LogRoomsA_FaceObjectEventAndWait(LOCALID_STRONGMAN_OPENING_THE_CHESTS,
                                   LOCALID_LOFT_MAYOR, 20);
  LogRoomsA_ShowMessageAndWait(LOCALID_STRONGMAN_OPENING_THE_CHESTS, 20);
  ObjectEvent_FaceObjectEvent(LOCALID_LOFT_MAYOR,
                              LOCALID_STRONGMAN_OPENING_THE_CHESTS, 0);
  ObjectEvent_ShowEmote(LOCALID_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  FieldScript_WaitFrames(60);
  LogRoomsA_FaceObjectEventAndWait(LOCALID_IVAN,
                                   LOCALID_STRONGMAN_OPENING_THE_CHESTS, 30);
  LogRoomsA_FaceObjectEventAndWait(LOCALID_LOFT_MAYOR, LOCALID_IVAN, 20);
  LogRoomsA_PlayAnimationAndWait(LOCALID_LOFT_MAYOR, ANIM_NOD, 20);
  Script_ShowMessage(LOCALID_LOFT_MAYOR, 0);
  ObjectEvent_FaceDirection(LOCALID_STRONGMAN_OPENING_THE_CHESTS,
                            FACING_SOUTH + FACING_STEP, 0);
  FieldScript_WaitFrames(30);
  LogRoomsA_FaceObjectEventAndWait(LOCALID_IVAN, LOCALID_LOFT_MAYOR, 20);
  LogRoomsA_PlayAnimationAndWait(LOCALID_IVAN, ANIM_NOD, 20);
  LogRoomsA_ShowMessageAndWait(LOCALID_LOFT_MAYOR, 20);
  ObjectEvent_LinkPair(LOCALID_PARTY_LEADER, LOCALID_IVAN, 0);
  LogRoomsA_FaceObjectEventAndWait(LOCALID_GARET, LOCALID_IVAN, 20);
  Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_NOD);
  LogRoomsA_PlayAnimationAndWait(LOCALID_GARET, ANIM_NOD, 40);
  LogRoomsA_PlayAnimationAndWait(LOCALID_IVAN, ANIM_NOD, 30);
  // Hammet was seen heading toward Lunpa: Dodonpa must have kidnapped him.
  ObjectMotion_SetVarCbAndRefresh(LOCALID_LOFT_MAYOR, 1);
  FieldScript_WaitFrames(20);
  LogRoomsA_PlayAnimationAndWait(LOCALID_LOFT_MAYOR, ANIM_SHAKE_HEAD, 20);
  LogRoomsA_ShowMessageAndWait(LOCALID_LOFT_MAYOR, 20);
  ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_LOFT_MAYOR, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_GARET, LOCALID_LOFT_MAYOR, 0);
  LogRoomsA_FaceObjectEventAndWait(LOCALID_IVAN, LOCALID_LOFT_MAYOR, 20);
  ObjectMotion_SetVariantCallback(LOCALID_PARTY_LEADER, 1);
  ObjectMotion_SetVariantCallback(LOCALID_GARET, 1);
  ObjectEvent_ShowEmote(LOCALID_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  FieldScript_WaitFrames(60);
  LogRoomsA_ShowMessageAndWait(LOCALID_IVAN, 40);
  LogRoomsA_PlayAnimationAndWait(LOCALID_LOFT_MAYOR, ANIM_NOD, 20);
  Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_SHAKE_HEAD);
  ObjectMotion_SetAnimationAndWait(LOCALID_GARET, ANIM_SHAKE_HEAD);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_IVAN, 3);
  FieldScript_WaitFrames(30);
  LogRoomsA_PlayAnimationAndWait(LOCALID_LOFT_MAYOR, ANIM_SHAKE_HEAD, 20);
  LogRoomsA_ShowMessageAndWait(LOCALID_LOFT_MAYOR, 20);
  LogRoomsA_FaceEachOtherAndWait(LOCALID_PARTY_LEADER, LOCALID_GARET, 20);
  // Ivan rushes forward: what will happen to Hammet? The mayor calms him:
  // Dodonpa wants a ransom, so Hammet is safe for now.
  ObjectMotion_SetVarCbAndRefresh(LOCALID_IVAN, 2);
  LogRoomsA_PlayAnimationAndWait(LOCALID_IVAN, ANIM_SHAKE_HEAD, 30);
  ObjectEvent_SetSpeed(LOCALID_IVAN, PIXELS(3) / 2, PIXELS(3) / 4);
  ObjectEvent_WalkToAndWait(LOCALID_IVAN, 800, 408);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_IVAN, 2);
  Script_ShowMessage(LOCALID_IVAN, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_LOFT_MAYOR, 0);
  LogRoomsA_FaceObjectEventAndWait(LOCALID_GARET, LOCALID_LOFT_MAYOR, 30);
  LogRoomsA_PlayAnimationAndWait(LOCALID_LOFT_MAYOR, ANIM_SHAKE_HEAD, 20);
  LogRoomsA_ShowMessageAndWait(LOCALID_LOFT_MAYOR, 20);
  ObjectEvent_SetAttachedEffect(LOCALID_PARTY_LEADER, ATTACHED_EFFECT_IN_FRONT |
                                                          ATTACHED_EFFECT_ONCE);
  ObjectEvent_SetAttachedEffect(LOCALID_GARET, ATTACHED_EFFECT_IN_FRONT |
                                                   ATTACHED_EFFECT_ONCE);
  ObjectEvent_SetAttachedEffect(LOCALID_IVAN, ATTACHED_EFFECT_IN_FRONT |
                                                  ATTACHED_EFFECT_ONCE);
  FieldScript_WaitFrames(60);
  LogRoomsA_ShowMessageAndWait(LOCALID_IVAN, 20);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_LOFT_MAYOR, 1);
  FieldScript_WaitFrames(20);
  LogRoomsA_ShowMessageAndWait(LOCALID_LOFT_MAYOR, 20);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION,
                        0);
  ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  ObjectEvent_ShowEmote(LOCALID_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  FieldScript_WaitFrames(60);
  LogRoomsA_PlayAnimationAndWait(LOCALID_LOFT_MAYOR, ANIM_SHAKE_HEAD, 20);
  LogRoomsA_ShowMessageAndWait(LOCALID_LOFT_MAYOR, 20);
  ObjectMotion_SetVariantCallback(LOCALID_PARTY_LEADER, 1);
  ObjectMotion_SetVariantCallback(LOCALID_GARET, 1);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_IVAN, 1);
  FieldScript_WaitFrames(20);
  LogRoomsA_PlayAnimationAndWait(LOCALID_LOFT_MAYOR, ANIM_NOD, 20);
  LogRoomsA_ShowMessageAndWait(LOCALID_LOFT_MAYOR, 40);
  // The mayor asks the two their names, thanks them, and asks them to come
  // see him before they leave town.
  ObjectEvent_ShowEmote(LOCALID_IVAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  FieldScript_WaitFrames(60);
  ObjectEvent_WalkToAndWait(LOCALID_LOFT_MAYOR, 840, 424);
  LogRoomsA_FaceEachOtherAndWait(LOCALID_LOFT_MAYOR, LOCALID_PARTY_LEADER, 20);
  LogRoomsA_ShowMessageAndWait(LOCALID_LOFT_MAYOR, 20);
  LogRoomsA_PlayAnimationAndWait(LOCALID_PARTY_LEADER, ANIM_NOD, 20);
  ObjectEvent_FaceDirection(LOCALID_LOFT_MAYOR, FACING_SOUTH + FACING_STEP, 0);
  FieldScript_WaitFrames(20);
  LogRoomsA_ShowMessageAndWait(LOCALID_LOFT_MAYOR, 20);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 1);
  FieldScript_WaitFrames(20);
  LogRoomsA_ShowMessageAndWait(LOCALID_GARET, 20);
  LogRoomsA_PlayAnimationAndWait(LOCALID_LOFT_MAYOR, ANIM_NOD, 20);
  LogRoomsA_ShowMessageAndWait(LOCALID_LOFT_MAYOR, 30);
  LogRoomsA_FaceEachOtherAndWait(LOCALID_LOFT_MAYOR,
                                 LOCALID_STRONGMAN_OPENING_THE_CHESTS, 20);
  LogRoomsA_ShowMessageAndWait(LOCALID_LOFT_MAYOR, 20);
  // The mayor and the villager opening the chests leave down the ladder.
  ObjectEvent_WalkToAndWait(LOCALID_STRONGMAN_OPENING_THE_CHESTS, 856, 408);
  ObjectEvent_SetActionScript(LOCALID_LOFT_MAYOR, gLogRoomsA_VaultScriptE);
  ObjectEvent_SetActionScript(LOCALID_STRONGMAN_OPENING_THE_CHESTS,
                              gLogRoomsA_VaultScriptE);
  ObjectEvent_WalkTo(LOCALID_GARET, 792, 456);
  ObjectEvent_SetSpeed(LOCALID_IVAN, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_WalkTo(LOCALID_IVAN, 776, 432);
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_GARET);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTH + FACING_STEP, 0);
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_IVAN);
  ObjectEvent_FaceDirection(LOCALID_IVAN, FACING_NORTH + FACING_STEP, 0);
  ObjectEvent_RefreshSelectorById(LOCALID_LOFT_MAYOR);
  ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_LOFT_MAYOR, 0);
  temporaryObjectEvent =
      ObjectEvent_GetById(LOCALID_STRONGMAN_OPENING_THE_CHESTS);
  {
    u8 *pauseFlag = &temporaryObjectEvent->paused;
    u8 paused = 1;

    *pauseFlag = paused;
  }
  temporaryObjectEvent->targetX = OBJ_EVENT_NO_TARGET;
  temporaryObjectEvent->targetY = OBJ_EVENT_NO_TARGET;
  temporaryObjectEvent->targetZ = OBJ_EVENT_NO_TARGET;
  ObjectEvent_ShowEmote(LOCALID_LOFT_MAYOR, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                        0);
  Import_ObjectEventSetAnimation(LOCALID_STRONGMAN_OPENING_THE_CHESTS,
                                 ANIM_STAND);
  FieldScript_WaitFrames(50);
  ObjectEvent_FaceObjectEvent(LOCALID_LOFT_MAYOR, LOCALID_PARTY_LEADER, 0);
  ObjectEvent_WalkToAndWait(LOCALID_GARET, 808, 440);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTHWEST + FACING_STEP, 0);
  FieldScript_WaitFrames(20);
  LogRoomsA_ShowMessageAndWait(LOCALID_LOFT_MAYOR, 20);
  LogRoomsA_FaceEachOtherAndWait(LOCALID_PARTY_LEADER, LOCALID_GARET, 50);
  ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_LOFT_MAYOR, 0);
  LogRoomsA_FaceObjectEventAndWait(LOCALID_GARET, LOCALID_LOFT_MAYOR, 20);
  LogRoomsA_PlayAnimationAndWait(LOCALID_LOFT_MAYOR, ANIM_NOD, 20);
  ObjectEvent_WalkTo(LOCALID_LOFT_MAYOR, 744, 408);
  ObjectEvent_WalkTo(LOCALID_STRONGMAN_OPENING_THE_CHESTS, 744, 408);
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_LOFT_MAYOR);
  ObjectMotion_CommitCurrentPositionAndActivate(
      LOCALID_STRONGMAN_OPENING_THE_CHESTS);
  FieldScript_WaitFrames(30);
  // Alone, Garet tries to cheer Ivan up; Ivan only wants to rescue Hammet,
  // but has seen the two's mission in their minds and won't ask their help.
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 792, 408);
  LogRoomsA_FaceEachOtherAndWait(LOCALID_PARTY_LEADER, LOCALID_GARET, 30);
  ObjectEvent_ShowEmote(LOCALID_IVAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  FieldScript_WaitFrames(50);
  ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_IVAN, 0);
  LogRoomsA_FaceObjectEventAndWait(LOCALID_GARET, LOCALID_IVAN, 20);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 1);
  FieldScript_WaitFrames(20);
  LogRoomsA_ShowMessageAndWait(LOCALID_GARET, 40);
  LogRoomsA_PlayAnimationAndWait(LOCALID_IVAN, ANIM_NOD, 30);
  LogRoomsA_FaceEachOtherAndWait(LOCALID_PARTY_LEADER, LOCALID_GARET, 40);
  Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_SHAKE_HEAD);
  LogRoomsA_PlayAnimationAndWait(LOCALID_GARET, ANIM_SHAKE_HEAD, 30);
  ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_IVAN, 0);
  LogRoomsA_FaceObjectEventAndWait(LOCALID_GARET, LOCALID_IVAN, 20);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 1);
  FieldScript_WaitFrames(20);
  LogRoomsA_ShowMessageAndWait(LOCALID_GARET, 40);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_IVAN, 2);
  FieldScript_WaitFrames(20);
  LogRoomsA_ShowMessageAndWait(LOCALID_IVAN, 20);
  Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_NOD);
  LogRoomsA_PlayAnimationAndWait(LOCALID_GARET, ANIM_NOD, 20);
  LogRoomsA_PlayAnimationAndWait(LOCALID_IVAN, ANIM_SHAKE_HEAD, 20);
  LogRoomsA_ShowMessageAndWait(LOCALID_IVAN, 20);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION,
                        0);
  ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  FieldScript_WaitFrames(60);
  LogRoomsA_PlayAnimationAndWait(LOCALID_IVAN, ANIM_NOD, 20);
  LogRoomsA_ShowMessageAndWait(LOCALID_IVAN, 20);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER,
                        EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  FieldScript_WaitFrames(60);
  LogRoomsA_PlayAnimationAndWait(LOCALID_IVAN, ANIM_NOD, 20);
  LogRoomsA_ShowMessageAndWait(LOCALID_IVAN, 20);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  FieldScript_WaitFrames(60);
  LogRoomsA_PlayAnimationAndWait(LOCALID_IVAN, ANIM_SHAKE_HEAD, 20);
  LogRoomsA_ShowMessageAndWait(LOCALID_IVAN, 20);
  LogRoomsA_PlayAnimationAndWait(LOCALID_GARET, ANIM_NOD, 20);
  LogRoomsA_ShowMessageAndWait(LOCALID_GARET, 30);
  // "Then, I suppose this is good-bye." A no has Garet say Isaac would help,
  // and Ivan decline.
  Script_OpenMessage(LOCALID_IVAN, 0);
  if (FieldScript_AskYesNoWithObjectEventAnimation(0, 0) != 0) {
    FieldScript_WaitFrames(20);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 1);
    FieldScript_WaitFrames(20);
    LogRoomsA_ShowMessageAndWait(LOCALID_GARET, 20);
    LogRoomsA_PlayAnimationAndWait(LOCALID_IVAN, ANIM_SHAKE_HEAD, 20);
    LogRoomsA_ShowMessageAndWait(LOCALID_IVAN, 20);
  }
  // Ivan walks round to the third chest and takes the Shaman's Rod from it.
  LogRoomsA_PlayAnimationAndWait(LOCALID_IVAN, ANIM_NOD, 30);
  ObjectEvent_WalkToAndWait(LOCALID_IVAN, 800, 456);
  ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_IVAN, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_GARET, LOCALID_IVAN, 0);
  ObjectEvent_WalkToAndWait(LOCALID_IVAN, 856, 456);
  ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_IVAN, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_GARET, LOCALID_IVAN, 0);
  ObjectEvent_WalkToAndWait(LOCALID_IVAN, 856, 376);
  ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_IVAN, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_GARET, LOCALID_IVAN, 0);
  ObjectEvent_FaceDirection(LOCALID_IVAN, FACING_EAST, 0);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_IVAN, 2);
  temporaryObjectEvent =
      (struct ObjectEvent *)FieldEffect_PlayCueAndStartEmitterOnTarget(
          LOCALID_IVAN, LOCALID_THIRD_CHEST, ITEM_SHAMANS_ROD);
  FieldScript_WaitFrames(60);
  rod = MsgVaultIvanGotShamansRod;
  FieldMessage_Show(rod, 1);
  ObjectEvent_Release(temporaryObjectEvent);
  Import_ObjectEventSetAnimation(LOCALID_THIRD_CHEST, ANIM_WALK);
  FieldScript_WaitFrames(20);
  LogRoomsA_PlayAnimationAndWait(LOCALID_IVAN, ANIM_NOD, 20);
  ObjectEvent_WalkToAndWait(LOCALID_IVAN, 856, 456);
  ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_IVAN, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_GARET, LOCALID_IVAN, 0);
  ObjectEvent_WalkToAndWait(LOCALID_IVAN, 800, 456);
  ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_IVAN, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_GARET, LOCALID_IVAN, 0);
  ObjectEvent_WalkToAndWait(LOCALID_IVAN, 776, 424);
  ObjectEvent_LinkPair(LOCALID_PARTY_LEADER, LOCALID_IVAN, 0);
  LogRoomsA_FaceObjectEventAndWait(LOCALID_GARET, LOCALID_IVAN, 30);
  // He thanks the town, hands back any Venus Djinni and leaves the party.
  LogRoomsA_PlayAnimationAndWait(LOCALID_IVAN, ANIM_NOD, 20);
  FieldScript_SetMessageId((rod + 1));
  LogRoomsA_ShowMessageAndWait(LOCALID_IVAN, 20);
  Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_NOD);
  {
    u16 *slot = &work->message;
    s32 saved = (s16)*slot;

    if (LogRoomsA_GetIvanVenusDjinnCount() != 0) {
      FieldScript_SetMessageId(MsgVaultWaitDontWantTakeYour);
      Script_ShowMessage(LOCALID_IVAN, 0);
      Vault_ReturnIsaacsDjinni();
    }
    Party_RemoveMemberRestored(IVAN);
    *slot = saved;
  }
  LogRoomsA_PlayAnimationAndWait(LOCALID_GARET, ANIM_NOD, 50);
  ObjectEvent_WalkToAndWait(LOCALID_IVAN, 776, 408);
  ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_IVAN, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_GARET, LOCALID_IVAN, 0);
  // Ivan goes down the ladder; Garet: they'd better get moving too.
  ObjectEvent_WalkToAndWait(LOCALID_IVAN, 744, 408);
  FieldScript_WaitFrames(40);
  LogRoomsA_FaceEachOtherAndWait(LOCALID_PARTY_LEADER, LOCALID_GARET, 20);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 1);
  FieldScript_WaitFrames(20);
  LogRoomsA_ShowMessageAndWait(LOCALID_GARET, 20);
  LogRoomsA_PlayAnimationAndWait(LOCALID_PARTY_LEADER, ANIM_NOD, 20);
  Import_ObjectEventSetAnimation(LOCALID_GARET, ANIM_WALK);
  objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (objectEvent != NULL) {
    ObjectEvent_SetDestination(LOCALID_GARET, objectEvent->x.part.pixel,
                               objectEvent->z.part.pixel);
  }
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_GARET);
  ObjectEvent_SetPosition(LOCALID_GARET, 0, 0);
  FieldScript_WaitFrames(30);
  // Clear the loft of everyone the event used.
  ObjectEvent_SetPosition(LOCALID_TIED_UP_VILLAGER, 0, 0);
  ObjectEvent_SetPosition(LOCALID_LOFT_MAYOR, 0, 0);
  ObjectEvent_SetPosition(LOCALID_LOFT_ESCORT, 0, 0);
  ObjectEvent_SetPosition(LOCALID_STRONGMAN_OPENING_THE_CHESTS, 0, 0);
  ObjectEvent_SetPosition(LOCALID_THIEF_LEADER, 0, 0);
  ObjectEvent_SetPosition(LOCALID_FIRST_THIEF, 0, 0);
  ObjectEvent_SetPosition(LOCALID_SECOND_THIEF, 0, 0);
  ObjectEvent_SetPosition(LOCALID_IVAN, 0, 0);
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  FieldScript_End();
}

/// Opens the window transition and waits one frame after it finishes.
static void LogRoomsA_OpenWindowTransition(void) {
  struct FieldWork *state;

  state = gFieldWork;
  state->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  FieldScript_StartScreenTransition();
  FieldScript_WaitTransitionFrames();
  FieldScript_WaitFrames(1);
}

static void LogRoomsA_ShowMessageAndWait(s32 localId, s32 waitFrames) {
  FieldScript_ShowMessageAndWait(localId, 0);
  FieldScript_WaitFrames(waitFrames);
}

static void LogRoomsA_FaceObjectEventAndWait(s32 localId, s32 targetId,
                                             s32 waitFrames) {
  ObjectMotion_SetAngleToward(localId, targetId, 0);
  FieldScript_WaitFrames(waitFrames);
}

static void LogRoomsA_FaceEachOtherAndWait(s32 firstId, s32 secondId,
                                           s32 waitFrames) {
  ObjectEvent_LinkPair(firstId, secondId, 0);
  FieldScript_WaitFrames(waitFrames);
}

/// Waits for the animation to change before the following pause.
static void LogRoomsA_PlayAnimationAndWait(s32 localId, s32 animation,
                                           s32 waitFrames) {
  Import_ObjectEventSetAnimation(localId, animation, waitFrames);
  ObjectMotion_WaitForAnimationChange(localId);
  FieldScript_WaitFrames(waitFrames);
}

/// Begins Mind Read, chooses its target, and shows its opening effect.
static void LogRoomsA_StartMindRead(s32 casterId, s32 targetId) {
  FieldEffect_LoadActionEffectResources(ABILITY_MIND_READ, 1);
  FieldEffect_SetupObjectPair(casterId, targetId);
  FieldEffect_StartCast();
  Field_DispatchTypeHandler(1);
  WaitFrames(1);
}

/// Finishes Mind Read and releases its event effect objects.
static void LogRoomsA_EndMindRead(void) {
  Field_DispatchTypeHandler(2);
  FieldEffect_StopCasterHook();
  FieldEffect_End();
}

#include "common/vault_itemobj.h"

/// Finds the route node containing the object event's position on the
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
  return &gLogRoomsA_Stops[stopIndex];
}

/// Checks whether an object event's destination or current position lies within
/// eight pixels.
static s32 LogRoomsA_IsObjectEventWithinEightPixels(s32 x, s32 z, s32 id) {
  struct ObjectEvent *objectEvent = ObjectEvent_GetByLocalId(id);
  s32 targetX = objectEvent->targetX;
  s32 targetZ;

  if (targetX == OBJ_EVENT_NO_TARGET) {
    targetX = objectEvent->x.fixed;
  }
  targetZ = objectEvent->targetZ;
  if (targetZ == OBJ_EVENT_NO_TARGET) {
    targetZ = objectEvent->z.fixed;
  }

  targetX = (targetX - x) >> 16;
  targetZ = (targetZ - z) >> 16;
  if (targetX * targetX + targetZ * targetZ <= 64)
    return 1;
  return 0;
}

/// Checks whether an object event's destination or current position lies within
/// sixteen pixels.
static s32 LogRoomsA_IsObjectEventWithinSixteenPixels(s32 x, s32 z, s32 id) {
  struct ObjectEvent *objectEvent = ObjectEvent_GetByLocalId(id);
  s32 ax;
  s32 az;

  ax = objectEvent->targetX;
  if (ax == OBJ_EVENT_NO_TARGET)
    ax = objectEvent->x.fixed;
  az = objectEvent->targetZ;
  if (az == OBJ_EVENT_NO_TARGET)
    az = objectEvent->z.fixed;
  ax = (ax - x) >> 16;
  az = (az - z) >> 16;
  if (ax * ax + az * az <= 0x100)
    return 1;
  return 0;
}

/// Reserves a full cell around the leader and half a cell around each object
/// event following a route. Testing destinations as well as current positions
/// prevents two object events choosing the same stop while still walking.
static s32 LogRoomsA_CheckStopOccupancy(struct LogRoomsAStop *stop) {
  s32 x;
  s32 z;

  if (stop == NULL) {
    return 1;
  }
  x = (stop->gridX << 19) + PIXELS(72);
  z = (stop->gridZ << 19) + PIXELS(632);
  if (LogRoomsA_IsObjectEventWithinSixteenPixels(x, z, LOCALID_PARTY_LEADER) !=
          0 ||
      LogRoomsA_IsObjectEventWithinEightPixels(x, z, LOCALID_IVAN) != 0 ||
      LogRoomsA_IsObjectEventWithinEightPixels(x, z,
                                               LOCALID_SUSPECTED_LEADER) != 0 ||
      LogRoomsA_IsObjectEventWithinEightPixels(x, z, LOCALID_SUSPECTED_THIEF) !=
          0) {
    return -1;
  }
  return 0;
}

/// Sets the object event's destination to the route stop's cell center.
static void LogRoomsA_MoveObjectEventToStop(struct ObjectEvent *objectEvent,
                                            struct LogRoomsAStop *stop) {
  Object_SetPosition(objectEvent, (stop->gridX << 19) + PIXELS(72), 0,
                     (stop->gridZ << 19) + PIXELS(632));
}

/// Steers Ivan and both suspects through the graph and detects a sustained
/// cornering.
///
/// Run each frame as a map callback from LogRoomsA_StartObjectEventStops
/// until EventScript_ReadCorneredSuspectsMinds removes it. Raises event 200
/// plus the blocked suspects (1 the leader, 2 the thief) after thirty frames
/// with at least one suspect blocked; a frame when both can move restarts the
/// wait.
static void LogRoomsA_UpdateObjectEventStops(void) {
  struct ObjectEvent *leader;
  struct ObjectEvent *objectEvent;
  struct FieldWork *work;
  struct LogRoomsAStop *entry;
  struct LogRoomsAStop *destination;
  s32 dx;
  s32 dz;
  s32 blocked;
  u16 angle;
  s16 facing;
  s16 facingDifference;

  leader = ObjectEvent_GetByLocalId(LOCALID_PARTY_LEADER);
  blocked = 0;
  work = gFieldWork;
  objectEvent = ObjectEvent_GetByLocalId(LOCALID_IVAN);
  entry = LogRoomsA_FindStopAtPosition(&objectEvent->x.fixed);
  if (entry != NULL && objectEvent->targetX == OBJ_EVENT_NO_TARGET) {
    dx = objectEvent->x.fixed - leader->x.fixed;
    dz = objectEvent->z.fixed - leader->z.fixed;
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
      facing = objectEvent->facing;
    }
    destination = LogRoomsA_SnapToNearestStop(entry, &facing);
    if (LogRoomsA_CheckStopOccupancy(destination) == 0) {
      LogRoomsA_MoveObjectEventToStop(objectEvent, destination);
      Object_SelectAnimation(objectEvent, 2);
    } else {
      Object_SelectAnimation(objectEvent, 1);
    }
  }

  // Each suspect tries a random turn, then reverses when that route is
  // occupied.
  objectEvent = ObjectEvent_GetByLocalId(LOCALID_SUSPECTED_LEADER);
  entry = LogRoomsA_FindStopAtPosition(&objectEvent->x.fixed);
  if (entry != NULL && objectEvent->targetX == OBJ_EVENT_NO_TARGET) {
    u32 turn = (u32)Import_RandomNext() * 2 >> 16;
    s32 currentFacing;

    turn *= 0x60000000;
    currentFacing = objectEvent->facing;
    facing = ((turn - 0x30000000) >> 16) + currentFacing;
    destination = LogRoomsA_SnapToNearestStop(entry, &facing);
    if (LogRoomsA_CheckStopOccupancy(destination) != 0) {
      facing = objectEvent->facing + 0x8000;
      destination = LogRoomsA_SnapToNearestStop(entry, &facing);
      if (LogRoomsA_CheckStopOccupancy(destination) == 0) {
        FieldEffect_ConfigureLinkedObject(LOCALID_SUSPECTED_LEADER,
                                          ATTACHED_EFFECT_ONCE);
        goto move_first;
      }
      Object_SelectAnimation(objectEvent, 4);
      blocked = 1;
    } else {
    move_first:
      LogRoomsA_MoveObjectEventToStop(objectEvent, destination);
      Object_SelectAnimation(objectEvent, 2);
    }
  }

  objectEvent = ObjectEvent_GetByLocalId(LOCALID_SUSPECTED_THIEF);
  entry = LogRoomsA_FindStopAtPosition(&objectEvent->x.fixed);
  if (entry != NULL && objectEvent->targetX == OBJ_EVENT_NO_TARGET) {
    u32 turn = (u32)Import_RandomNext() * 3 >> 16;
    s32 currentFacing;

    turn *= 0x30000000;
    currentFacing = objectEvent->facing;
    facing = ((turn - 0x30000000) >> 16) + currentFacing;
    destination = LogRoomsA_SnapToNearestStop(entry, &facing);
    if (LogRoomsA_CheckStopOccupancy(destination) != 0) {
      facing = objectEvent->facing + 0x8000;
      destination = LogRoomsA_SnapToNearestStop(entry, &facing);
      if (LogRoomsA_CheckStopOccupancy(destination) == 0) {
        FieldEffect_ConfigureLinkedObject(LOCALID_SUSPECTED_THIEF,
                                          ATTACHED_EFFECT_ONCE);
        goto move_second;
      }
      Object_SelectAnimation(objectEvent, 4);
      blocked += 2;
    } else {
    move_second:
      LogRoomsA_MoveObjectEventToStop(objectEvent, destination);
      Object_SelectAnimation(objectEvent, 2);
    }
  }
  // A failed route choice alone is not enough to end the chase.
  if (blocked != 0) {
    if (++gLogRoomsA_StopTimer > LOG_ROOMS_A_CORNERED_FRAMES - 1)
      work->raisedEvent = blocked + LOG_ROOMS_A_CORNERED_EVENT_BASE;
  } else {
    gLogRoomsA_StopTimer = blocked;
  }
}

/// Arranges the chase, constrains its camera, and starts the route callback.
///
/// Called by EventScript_StartCorneringSuspects. Starts
/// LogRoomsA_UpdateObjectEventStops and the chase music, MUS_DJINN_LESSON.
static void LogRoomsA_StartObjectEventStops(void) {
  FieldScript_Start();
  FieldCamera_MoveTo(PIXELS(168), -1, PIXELS(656), 1);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_SetSpeed(LOCALID_GARET, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_SetSpeed(LOCALID_IVAN, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 248, 696);
  ObjectEvent_SetPosition(LOCALID_GARET, PIXELS(248), PIXELS(696));
  ObjectEvent_SetPosition(LOCALID_IVAN, PIXELS(248), PIXELS(696));
  ObjectEvent_WalkTo(LOCALID_PARTY_LEADER, 200, 696);
  ObjectEvent_WalkTo(LOCALID_GARET, 248, 712);
  ObjectEvent_WalkToAndWait(LOCALID_IVAN, 232, 696);
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_GARET);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(LOCALID_IVAN, FACING_WEST, 0);
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_PARTY_LEADER);
  Import_ObjectEventSetAnimation(LOCALID_GARET, 12);
  LogRoomsA_ResetSuspectRoutes();
  Field_SetViewBounds(PIXELS(48), PIXELS(576), PIXELS(288), PIXELS(736));
  ObjectEvent_SetSpeed(LOCALID_GARET, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_SetSpeed(LOCALID_IVAN, PIXELS(3) / 4, PIXELS(3) / 8);
  ObjectEvent_SetSpeed(LOCALID_SUSPECTED_LEADER, PIXELS(1), PIXELS(6) / 5);
  ObjectEvent_SetSpeed(LOCALID_SUSPECTED_THIEF, PIXELS(3) / 2, PIXELS(3) / 2);
  gLogRoomsA_StopTimer = 0;
  FieldScheduler_AddOrUpdateCallback(LogRoomsA_UpdateObjectEventStops,
                                     TASK_PRIORITY_MAP + 20);
  // Flag 0x1ff holds nothing: no table, script or function sets or reads it,
  // and render probe flag-reads sees no read of it, so clearing it changes
  // nothing.
  Script_ClearFlag(0x1ff);
  FieldScript_End();
  Sound_PlaySong(MUS_DJINN_LESSON);
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
    SC_SET(SW_FACING, 0x3000),
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
    SC_SET(SW_FACING, 0x3000),
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
// (EventScript_SuspectsRunFromParty): the leader's 1 to 3 the heads and 4 to 6 the
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

struct MapWarpDest gLogRoomsA_WarpDests[] = {
    { WARP_DEST_DEFAULT, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
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
    { MAP_TABLE_END },
};

struct MapWarpRegion gLogRoomsA_Regions[] = {
    {639, 33, 407, 657, 51, 425, CONDITION_ALWAYS, LOG_ROOMS_A_TO_VAULT_WEST},
    {MAP_TABLE_END},
};

u32 gLogRoomsA_Warps[] = {
    MAP_WARPS(LogRoomsA),
    MAP_WARP(LOG_ROOMS_A_TO_VAULT_EAST_SOUTHEAST, Vault, VAULT_FROM_LOG_ROOMS_A_EAST_SOUTHEAST),
    MAP_WARP(LOG_ROOMS_A_TO_VAULT_SOUTHEAST, Vault, VAULT_FROM_LOG_ROOMS_A_SOUTHEAST),
    MAP_WARP(LOG_ROOMS_A_TO_VAULT_SOUTHWEST, Vault, VAULT_FROM_LOG_ROOMS_A_SOUTHWEST),
    MAP_WARP(LOG_ROOMS_A_TO_VAULT_EAST, Vault, VAULT_FROM_LOG_ROOMS_A_EAST),
    MAP_WARP(LOG_ROOMS_A_TO_VAULT_NORTH_NORTHEAST, Vault, VAULT_FROM_LOG_ROOMS_A_NORTH_NORTHEAST),
    MAP_WARP(LOG_ROOMS_A_TO_VAULT_NORTHWEST, Vault, VAULT_FROM_LOG_ROOMS_A_NORTHWEST),
    MAP_WARP(LOG_ROOMS_A_INN_UPSTAIRS_TO_INN, LogRoomsA, LOG_ROOMS_A_INN_FROM_INN_UPSTAIRS),
    MAP_WARP(LOG_ROOMS_A_INN_TO_INN_UPSTAIRS, LogRoomsA, LOG_ROOMS_A_INN_UPSTAIRS_FROM_INN),
    MAP_WARP(LOG_ROOMS_A_TO_VAULT_WEST, Vault, VAULT_FROM_LOG_ROOMS_A_WEST),
    MAP_WARP(LOG_ROOMS_A_LOFT_TO_CRATE_ROOM, LogRoomsA, LOG_ROOMS_A_CRATE_ROOM_FROM_LOFT),
    MAP_WARP(LOG_ROOMS_A_CRATE_ROOM_TO_LOFT, LogRoomsA, LOG_ROOMS_A_LOFT_FROM_CRATE_ROOM),
    MAP_WARP(LOG_ROOMS_A_TO_SANCTUM, Sanctum, SANCTUM_FROM_LOG_ROOMS_A),
    MAP_WARPS_END,
};

struct ObjectEventTemplate gLogRoomsA_ObjectEventTemplates[] = {
    { LOCALID_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(8), 0, PIXELS(8), FACING_NORTH, TALK_FACING_PARTY, 0 },
    { LOCALID_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(376), 0, PIXELS(392), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(160), 0, PIXELS(136), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, 0x0, 3,
        PIXELS(152), 0, PIXELS(72), FACING_WEST, TALK_FACING_PARTY, 0 },
    { SPRITE_BURLY_MAN_WITH_GREY_HAIR, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(456), 0, PIXELS(184), FACING_EAST, TALK_FACING_PARTY, 0 },
    { SPRITE_BOY_WITH_FAIR_HAIR, 0x0, MOVEMENT_TYPE_WANDER,
        PIXELS(368), 0, PIXELS(120), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_OLD_MAN_WITH_CANE, 0x0, MOVEMENT_TYPE_WANDER,
        PIXELS(744), 0, PIXELS(200), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_OLD_WOMAN, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(752), 0, PIXELS(128), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_BOY_WITH_FAIR_HAIR, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(496), 0, PIXELS(416), FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_GIRL_WITH_PIGTAILS, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(512), 0, PIXELS(416), FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_OLD_MAN_WITH_WHITE_HAIR, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(536), 0, PIXELS(451), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_IN_GREEN_DRESS, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(416), 0, PIXELS(464), FACING_NORTH, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, 0x0, MOVEMENT_TYPE_WANDER,
        PIXELS(536), 0, PIXELS(672), FACING_NORTH, TALK_FACING_PARTY, 0 },
    { SPRITE_BALD_MAN_IN_RED, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(200), 0, PIXELS(392), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_YOUNG_MAN_IN_GREEN, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(88), 0, PIXELS(392), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_WOMAN_WITH_PONYTAIL, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(632), 0, PIXELS(136), FACING_SOUTH, 3, 0 },
    { SPRITE_BURLY_MAN_WITH_GREY_HAIR, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(664), 0, PIXELS(632), FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_WOMAN_IN_PINK_APRON, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(696), 0, PIXELS(696), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_MAN_WITH_LONG_DARK_HAIR, FLAG_VAULT_THIEVES_ATTACKED, MOVEMENT_TYPE_STAND,
        PIXELS(104), 0, PIXELS(696), FACING_EAST, 3, 0 },
    { SPRITE_PERSON_IN_PURPLE, FLAG_VAULT_THIEVES_ATTACKED, MOVEMENT_TYPE_STAND,
        PIXELS(200), 0, PIXELS(680), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { 0xdf, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(692), 0, PIXELS(411), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gLogRoomsA_LoftObjectEventTemplates[] = {
    { LOCALID_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(8), 0, PIXELS(8), FACING_NORTH, TALK_FACING_PARTY, 0 },
    { LOCALID_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
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
    { SPRITE_CHEST, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(872), 0, PIXELS(424), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_CHEST, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(872), 0, PIXELS(408), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_CHEST, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(872), 0, PIXELS(376), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { 0xfb, CONDITION_ALWAYS, 7,
        PIXELS(8), 0, PIXELS(8), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { 0xdd, CONDITION_ALWAYS, 7,
        PIXELS(8), 0, PIXELS(8), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { MAP_TABLE_END },
};

struct MapEvent gLogRoomsA_Events[] = {
    { MAP_EVENT_WARP, 1, CONDITION_ALWAYS, LOG_ROOMS_A_TO_VAULT_EAST_SOUTHEAST },
    { MAP_EVENT_WARP, 2, CONDITION_ALWAYS, LOG_ROOMS_A_TO_VAULT_SOUTHEAST },
    { MAP_EVENT_WARP, 3, CONDITION_ALWAYS, LOG_ROOMS_A_TO_VAULT_SOUTHWEST },
    { MAP_EVENT_WARP, 4, CONDITION_ALWAYS, LOG_ROOMS_A_TO_VAULT_EAST },
    { MAP_EVENT_WARP, 5, CONDITION_ALWAYS, LOG_ROOMS_A_TO_VAULT_NORTH_NORTHEAST },
    { MAP_EVENT_WARP, 6, CONDITION_ALWAYS, LOG_ROOMS_A_TO_VAULT_NORTHWEST },
    { MAP_EVENT_COORD, MB_LOG_ROOMS_A_GUESTS_ROOM_STAIRS, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveGuestsRoom) },
    { MAP_EVENT_WARP, 8, CONDITION_ALWAYS, LOG_ROOMS_A_INN_TO_INN_UPSTAIRS },
    { MAP_EVENT_COORD, MB_LOG_ROOMS_A_GUESTS_ROOM, IF_FLAG_SET(FLAG_VAULT_IVAN_JOINED_ROD_SEARCH), EVENT_SCRIPT(EventScript_ApproachSuspects) },
    { MAP_EVENT_COORD, MB_LOG_ROOMS_A_FIRST_SUSPECT_ROUTE, IF_FLAG_SET(FLAG_SUSPECTS_FLEEING), EVENT_SCRIPT(EventScript_SuspectsRunFromParty) },
    { MAP_EVENT_COORD, MB_LOG_ROOMS_A_FIRST_SUSPECT_ROUTE + 1, IF_FLAG_SET(FLAG_SUSPECTS_FLEEING), EVENT_SCRIPT(EventScript_SuspectsRunFromParty) },
    { MAP_EVENT_COORD, MB_LOG_ROOMS_A_FIRST_SUSPECT_ROUTE + 2, IF_FLAG_SET(FLAG_SUSPECTS_FLEEING), EVENT_SCRIPT(EventScript_SuspectsRunFromParty) },
    { MAP_EVENT_COORD, MB_LOG_ROOMS_A_FIRST_SUSPECT_ROUTE + 3, IF_FLAG_SET(FLAG_SUSPECTS_FLEEING), EVENT_SCRIPT(EventScript_SuspectsRunFromParty) },
    { MAP_EVENT_COORD, MB_LOG_ROOMS_A_FIRST_SUSPECT_ROUTE + 4, IF_FLAG_SET(FLAG_SUSPECTS_FLEEING), EVENT_SCRIPT(EventScript_SuspectsRunFromParty) },
    { MAP_EVENT_COORD, MB_LOG_ROOMS_A_FIRST_SUSPECT_ROUTE + 5, IF_FLAG_SET(FLAG_SUSPECTS_FLEEING), EVENT_SCRIPT(EventScript_SuspectsRunFromParty) },
    { MAP_EVENT_COORD, MB_LOG_ROOMS_A_FIRST_SUSPECT_ROUTE + 6, IF_FLAG_SET(FLAG_SUSPECTS_FLEEING), EVENT_SCRIPT(EventScript_SuspectsRunFromParty) },
    { MAP_EVENT_COORD, MB_LOG_ROOMS_A_LAST_SUSPECT_ROUTE, IF_FLAG_SET(FLAG_SUSPECTS_FLEEING), EVENT_SCRIPT(EventScript_SuspectsRunFromParty) },
    { MAP_EVENT_COORD, MB_LOG_ROOMS_A_GUESTS_ROOM_DOOR, IF_FLAG_SET(FLAG_SUSPECTS_FLEEING), EVENT_SCRIPT(EventScript_DiscussSuspiciousGuests) },
    { MAP_EVENT_COORD, MB_LOG_ROOMS_A_CORNERING_START, IF_FLAG_SET(FLAG_VAULT_SUSPECTS_APPROACHED), EVENT_SCRIPT(EventScript_StartCorneringSuspects) },
    { MAP_EVENT_RAISED, LOG_ROOMS_A_LEADER_CORNERED, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_ReadCorneredSuspectsMinds) },
    { MAP_EVENT_RAISED, LOG_ROOMS_A_THIEF_CORNERED, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_ReadCorneredSuspectsMinds) },
    { MAP_EVENT_RAISED, LOG_ROOMS_A_BOTH_CORNERED, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_ReadCorneredSuspectsMinds) },
    { MAP_EVENT_RAISED, FIELD_RAISED_EVENT_MENUS, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_GaretRefusesToLeave) },
    { MAP_EVENT_COORD, MB_LOG_ROOMS_A_LOFT_CLIMB, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_ClimbToLoft) },
    { COORD_EVENT_WALKING, MB_LOG_ROOMS_A_ROOF_GAP, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_JumpAcrossRoofGap) },
    { MAP_EVENT_TALK, LOCALID_IVAN, IF_FLAG_SET(FLAG_VAULT_CORNERING_SUSPECTS), 0 },
    { MAP_EVENT_TALK, LOCALID_IVAN, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_MeetIvan) },
    { MAP_EVENT_TALK, LOCALID_GEM_OWNER, CONDITION_ALWAYS, MSGNO(MsgVaultThisRockWasBlownHereAll) },
    { MAP_EVENT_TALK, LOCALID_GEM_OWNERS_WIFE, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkAboutRocks) },
    { MAP_EVENT_TALK, LOCALID_VILLAGER_SUSPECTING_THE_NEIGHBOR, CONDITION_ALWAYS, MSGNO(MsgVaultMyNeighborCreepsMeOutHe) },
    { MAP_EVENT_TALK, LOCALID_BOY_ASKING_ABOUT_TRAVEL, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_BoyAsksAboutTravel) },
    { MAP_EVENT_TALK, LOCALID_GRANDPA, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkAboutMissingThings) },
    { MAP_EVENT_TALK, LOCALID_GRANDPAS_WIFE, CONDITION_ALWAYS, MSGNO(MsgVaultEverSinceTheEruptionGrandpaKeeps) },
    { MAP_EVENT_TALK, LOCALID_BOY_WISHING_TO_BE_OLDER, FLAG_VAULT_IVAN_JOINED_ROD_SEARCH, MSGNO(MsgVaultICouldHelpIvanIfI) },
    { MAP_EVENT_TALK, LOCALID_BOY_WISHING_TO_BE_OLDER, CONDITION_ALWAYS, MSGNO(MsgVaultYoureHelpingIvanYouDontLook) },
    { MAP_EVENT_TALK, LOCALID_GIRL_TELLING_A_SECRET, FLAG_VAULT_IVAN_JOINED_ROD_SEARCH, EVENT_SCRIPT(EventScript_OfferIvansSecret) },
    { MAP_EVENT_TALK, LOCALID_GIRL_TELLING_A_SECRET, CONDITION_ALWAYS, MSGNO(MsgVaultSinceYoureHelpingIvanIllTell) },
    { MAP_EVENT_TALK, LOCALID_MAYOR, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_MayorAsksHelpForIvan) },
    { MAP_EVENT_TALK, LOCALID_MAYORS_WIFE, FLAG_VAULT_IVAN_JOINED_ROD_SEARCH, MSGNO(MsgVaultMyHusbandTheMayorCantBelieve) },
    { MAP_EVENT_TALK, LOCALID_MAYORS_WIFE, CONDITION_ALWAYS, MSGNO(MsgVaultReallyYoureGoingToHelpIvan) },
    { MAP_EVENT_TALK, LOCALID_GIRL_AT_THE_INN, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_WarnAboutThreeStrangers) },
    { MAP_EVENT_TALK, LOCALID_WEAPON_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToWeaponMerchant) },
    { MAP_EVENT_TALK, LOCALID_ARMOR_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToArmorMerchant) },
    { MAP_EVENT_TALK, LOCALID_ITEM_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToItemMerchant) },
    { MAP_EVENT_TALK, LOCALID_INN_STAFF, CONDITION_ALWAYS, MSGNO(MsgVaultTheGuestsUpstairsWerentInTheir) },
    { MAP_EVENT_TALK, LOCALID_INNKEEPER, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToInnkeeper) },
    { MAP_EVENT_TALK, LOCALID_SUSPECTED_LEADER, IF_FLAG_SET(FLAG_VAULT_CORNERING_SUSPECTS), 0 },
    { MAP_EVENT_TALK, LOCALID_SUSPECTED_THIEF, IF_FLAG_SET(FLAG_VAULT_CORNERING_SUSPECTS), 0 },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_SUSPECTED_LEADER, IF_FLAG_SET(FLAG_VAULT_CORNERING_SUSPECTS), 0 },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_SUSPECTED_THIEF, IF_FLAG_SET(FLAG_VAULT_CORNERING_SUSPECTS), 0 },
    { MAP_EVENT_TALK, LOCALID_SUSPECTED_LEADER, FLAG_VAULT_SUSPECTS_MIND_READ, MSGNO(MsgVaultThatKidIvanHeCreepsMe) },
    { MAP_EVENT_TALK, LOCALID_SUSPECTED_THIEF, FLAG_VAULT_SUSPECTS_MIND_READ, MSGNO(MsgVaultWeHaventDoneAnythingYoureA) },
    { MAP_EVENT_TALK, LOCALID_SUSPECTED_LEADER, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_SuspectedLeaderDodges) },
    { MAP_EVENT_TALK, LOCALID_SUSPECTED_THIEF, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_SuspectedThiefDodges) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_GEM_OWNER, CONDITION_ALWAYS, MSGNO(MsgVaultTheOtherSparklingRocksAllCrumbled) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_GEM_OWNERS_WIFE, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_ReadMindOnRock) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_VILLAGER_SUSPECTING_THE_NEIGHBOR, CONDITION_ALWAYS, MSGNO(MsgVaultHeyMaybeMyNeighborIsThe) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_BOY_ASKING_ABOUT_TRAVEL, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_ReadBoysMind) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_GRANDPA, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_ReadMindOnThieves) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_GRANDPAS_WIFE, CONDITION_ALWAYS, MSGNO(MsgVaultYouNeverBoughtMeAnythingWorth) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_BOY_WISHING_TO_BE_OLDER, CONDITION_ALWAYS, MSGNO(MsgVaultTheseGuysLookReliableIGuess) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_GIRL_TELLING_A_SECRET, CONDITION_ALWAYS, MSGNO(MsgVaultIWonderWhatSortOfThing) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_MAYOR, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_ReadMayorMind) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_MAYORS_WIFE, CONDITION_ALWAYS, MSGNO(MsgVaultMaybeTheyllFindOurStolenFamily) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_GIRL_AT_THE_INN, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_ReadBoneGiverMind) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WEAPON_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgVaultAllOurBestWeaponsWereStolen) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_ARMOR_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgVaultIWonderIfTheyLeftUs) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_ITEM_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgVaultHahhWereSureToSeeA) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_INN_STAFF, CONDITION_ALWAYS, MSGNO(MsgVaultItWouldBeAwfulIfOur) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_INNKEEPER, CONDITION_ALWAYS, MSGNO(MsgVaultThatVolcanoCostUsALot) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), 0x400 | LOCALID_SUSPECTED_LEADER, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_SuspectedLeaderDodges) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), 0x400 | LOCALID_SUSPECTED_THIEF, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_SuspectedThiefDodges) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_CRATE, FLAG_VAULT_CRATE_MOVED, EVENT_SCRIPT(EventScript_CheckCrateMoved) },
    { BG_EVENT(BG_EVENT_SILENT), MB_LOG_ROOMS_A_INN_BARREL, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_SearchEmptyBarrel) },
    { BG_EVENT(BG_EVENT_JAR), MB_LOG_ROOMS_A_JAR, FLAG_LOG_ROOMS_A_JAR_COINS, FIND_COINS(7) },
    { BG_EVENT(BG_EVENT_BARREL), MB_LOG_ROOMS_A_FIRST_BARREL, FLAG_LOG_ROOMS_A_BARREL_COINS, FIND_COINS(4) },
    { BG_EVENT(BG_EVENT_BARREL), MB_LOG_ROOMS_A_SECOND_BARREL, FLAG_LOG_ROOMS_A_BARREL_SLEEP_BOMB, FIND_ITEM(ITEM_SLEEP_BOMB) },
    { BG_EVENT(BG_EVENT_WOODEN_BOX), MB_LOG_ROOMS_A_WOODEN_BOX, FLAG_LOG_ROOMS_A_BOX_MINT, FIND_ITEM(ITEM_MINT) },
    { BG_EVENT(BG_EVENT_BOOKCASE) | MAP_EVENT_FLAG_FACING | FACING_NORTH, MB_LOG_ROOMS_A_HISTORY_BOOKCASE, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgVaultTheresABookEntitledHistoryOf)) },
    { BG_EVENT(BG_EVENT_BOOKCASE), MB_LOG_ROOMS_A_GEMS_BOOKCASE, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgVaultTheresABookEntitledGemsOf)) },
    { MAP_EVENTS_END },
};

struct MapEvent gLogRoomsA_LoftEvents[] = {
    { MAP_EVENT_COORD, MB_LOG_ROOMS_A_LOFT_DESCENT, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveLoft) },
    { MAP_EVENT_TALK, LOCALID_TIED_UP_VILLAGER, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_FreeRooferAndConfrontThieves) },
    { MAP_EVENT_TALK, LOCALID_FIRST_CHEST, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_SearchEmptyChest) },
    { MAP_EVENT_TALK, LOCALID_SECOND_CHEST, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_SearchEmptyChest) },
    { MAP_EVENT_TALK, LOCALID_THIRD_CHEST, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_SearchEmptyChest) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_TIED_UP_VILLAGER, CONDITION_ALWAYS, MSGNO(MsgVaultAhhHelpMe) },
    { MAP_EVENTS_END },
};

struct MapEvent gLogRoomsA_EventsAfterThieves[] = {
    { BG_EVENT(BG_EVENT_SILENT), MB_LOG_ROOMS_A_INN_BARREL, CONDITION_ALWAYS, EVENT_SCRIPT(Vault_FindBone) },
    { MAP_EVENT_WARP, 1, CONDITION_ALWAYS, LOG_ROOMS_A_TO_VAULT_EAST_SOUTHEAST },
    { MAP_EVENT_WARP, 2, CONDITION_ALWAYS, LOG_ROOMS_A_TO_VAULT_SOUTHEAST },
    { MAP_EVENT_WARP, 3, CONDITION_ALWAYS, LOG_ROOMS_A_TO_VAULT_SOUTHWEST },
    { MAP_EVENT_WARP, 4, CONDITION_ALWAYS, LOG_ROOMS_A_TO_VAULT_EAST },
    { MAP_EVENT_WARP, 5, CONDITION_ALWAYS, LOG_ROOMS_A_TO_VAULT_NORTH_NORTHEAST },
    { MAP_EVENT_WARP, 6, CONDITION_ALWAYS, LOG_ROOMS_A_TO_VAULT_NORTHWEST },
    { MAP_EVENT_WARP, 7, CONDITION_ALWAYS, LOG_ROOMS_A_INN_UPSTAIRS_TO_INN },
    { MAP_EVENT_WARP, 8, CONDITION_ALWAYS, LOG_ROOMS_A_INN_TO_INN_UPSTAIRS },
    { MAP_EVENT_COORD, MB_LOG_ROOMS_A_LOFT_CLIMB, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_ClimbToLoft) },
    { MAP_EVENT_COORD, MB_LOG_ROOMS_A_LOFT_DESCENT, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveLoft) },
    { MAP_EVENT_TALK, LOCALID_GEM_OWNER, CONDITION_ALWAYS, MSGNO(MsgVaultICanSleepWellNowThat) },
    { MAP_EVENT_TALK, LOCALID_GEM_OWNERS_WIFE, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkAboutRocks) },
    { MAP_EVENT_TALK, LOCALID_VILLAGER_SUSPECTING_THE_NEIGHBOR, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_AskWhetherThievesCameFromLunpa) },
    { MAP_EVENT_TALK, LOCALID_BOY_ASKING_ABOUT_TRAVEL, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_BoyWorriesAboutFather) },
    { MAP_EVENT_TALK, LOCALID_GRANDPA, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkAboutMissingThings) },
    { MAP_EVENT_TALK, LOCALID_GRANDPAS_WIFE, CONDITION_ALWAYS, MSGNO(MsgVaultJustAsIThoughtNothingWas) },
    { MAP_EVENT_TALK, LOCALID_BOY_WISHING_TO_BE_OLDER, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_AskAboutDefeatingThieves) },
    { MAP_EVENT_TALK, LOCALID_GIRL_TELLING_A_SECRET, CONDITION_ALWAYS, MSGNO(MsgVaultIvanLearnedALotByTalking) },
    { MAP_EVENT_TALK, LOCALID_MAYOR, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_MayorGivesWaterOfLife) },
    { MAP_EVENT_TALK, LOCALID_MAYORS_WIFE, CONDITION_ALWAYS, MSGNO(MsgVaultYoureThePersonWhoGotOur) },
    { MAP_EVENT_TALK, LOCALID_GIRL_AT_THE_INN, FLAG_VAULT_LEADER_BESIDE_BONE, EVENT_SCRIPT(EventScript_OfferBones) },
    { MAP_EVENT_TALK, LOCALID_WEAPON_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToWeaponMerchant) },
    { MAP_EVENT_TALK, LOCALID_ARMOR_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToArmorMerchant) },
    { MAP_EVENT_TALK, LOCALID_ITEM_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToItemMerchant) },
    { MAP_EVENT_TALK, LOCALID_INN_STAFF, CONDITION_ALWAYS, MSGNO(MsgVaultImShockedThoseGuestsOfOurs) },
    { MAP_EVENT_TALK, LOCALID_INNKEEPER, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToInnkeeper) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_GEM_OWNER, CONDITION_ALWAYS, MSGNO(MsgVaultEverybodysAfterMyRockTheyWant) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_GEM_OWNERS_WIFE, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_ReadMindOnRock) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_VILLAGER_SUSPECTING_THE_NEIGHBOR, CONDITION_ALWAYS, MSGNO(MsgVaultTheReasonLunpaTurnedBadIs) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_BOY_ASKING_ABOUT_TRAVEL, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_ReadBoysMind) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_GRANDPA, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_ReadMindOnThieves) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_GRANDPAS_WIFE, CONDITION_ALWAYS, MSGNO(MsgVaultIWishIHadSomethingWorth) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_BOY_WISHING_TO_BE_OLDER, CONDITION_ALWAYS, MSGNO(MsgVaultWhenIGrowUpIWant) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_GIRL_TELLING_A_SECRET, CONDITION_ALWAYS, MSGNO(MsgVaultIWishIHadWingsLike) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_MAYOR, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_ReadMayorMind) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_MAYORS_WIFE, CONDITION_ALWAYS, MSGNO(MsgVaultImSoRelievedNowThatWe) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_GIRL_AT_THE_INN, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_ReadBoneGiverMind) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WEAPON_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgVaultMyWeaponsWereFoundInThe) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_ARMOR_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgVaultItMustBeSnowingUpIn) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_ITEM_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgVaultWhyAreThesePeopleSoAnxious) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_INN_STAFF, CONDITION_ALWAYS, MSGNO(MsgVaultWhoCouldHaveKnownTheyWere) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_INNKEEPER, CONDITION_ALWAYS, MSGNO(MsgVaultOfCourseIAlwaysSuspectedThey) },
    { BG_EVENT(BG_EVENT_JAR), MB_LOG_ROOMS_A_JAR, FLAG_LOG_ROOMS_A_JAR_COINS, FIND_COINS(7) },
    { BG_EVENT(BG_EVENT_BARREL), MB_LOG_ROOMS_A_FIRST_BARREL, FLAG_LOG_ROOMS_A_BARREL_COINS, FIND_COINS(4) },
    { BG_EVENT(BG_EVENT_BARREL), MB_LOG_ROOMS_A_SECOND_BARREL, FLAG_LOG_ROOMS_A_BARREL_SLEEP_BOMB, FIND_ITEM(ITEM_SLEEP_BOMB) },
    { BG_EVENT(BG_EVENT_WOODEN_BOX), MB_LOG_ROOMS_A_WOODEN_BOX, FLAG_LOG_ROOMS_A_BOX_MINT, FIND_ITEM(ITEM_MINT) },
    { BG_EVENT(BG_EVENT_BOOKCASE) | MAP_EVENT_FLAG_FACING | FACING_NORTH, MB_LOG_ROOMS_A_HISTORY_BOOKCASE, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgVaultTheresABookEntitledHistoryOf)) },
    { BG_EVENT(BG_EVENT_BOOKCASE), MB_LOG_ROOMS_A_GEMS_BOOKCASE, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgVaultTheresABookEntitledGemsOf)) },
    { MAP_EVENTS_END },
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
