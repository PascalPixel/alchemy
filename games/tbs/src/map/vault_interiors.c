/// @file
/// Vault (MapId_VaultInteriors): the insides of the town's houses, shops and
/// inn, and the inn's loft, while Hammet's Shaman's Rod is missing and just
/// after it is found. Every room is on one map, each a warp destination with a
/// camera box of its own, and Vault's doors (vault.c) lead to the warp
/// destinations named for them: the gem owner's house, the child's house, the
/// inn and its guests' room upstairs, the item shop, the weapon and armor
/// shop, the mayor's house and a room with a crate, behind which a way climbs
/// to the inn's loft. The loft has object event templates and events of its
/// own (VaultInteriors_GetObjectEventTemplates and _GetEvents), and
/// FLAG_VAULT_THIEVES_JAILED chooses the other rooms' tables.
///
/// The story flags drive the rooms in order:
/// - At the mayor's, talking to Ivan plays EventScript_MeetIvan: he reads
/// Isaac's
///   mind (FLAG_VAULT_MET_IVAN), asks for help with the rod, and on a yes
///   joins the party (VaultInteriors_IvanJoinsRodSearch,
///   FLAG_VAULT_IVAN_JOINED_ROD_SEARCH).
/// - In the inn's guests' room the two suspects run from Ivan
///   (EventScript_ApproachSuspects), and stepping out the party plans to corner
///   one (EventScript_DiscussSuspiciousGuests). The chase
///   (EventScript_StartCorneringSuspects) moves Ivan and the suspects along a
///   network of stops (gVaultInteriors_Stops); once one has been blocked for
///   half a second, VaultInteriors_UpdateObjectEventStops raises
///   VAULT_INTERIORS_LEADER_CORNERED, _THIEF_CORNERED or _BOTH_CORNERED, and
///   Ivan reads the cornered suspect's mind
///   (EventScript_ReadCorneredSuspectsMinds, FLAG_VAULT_SUSPECTS_MIND_READ).
///   Meanwhile the menus answer with Garet's refusal (FLAG_MENUS_RAISE_EVENT).
/// - In the room with the crate, Move pushes the crate aside
///   (EventScript_CheckCrateMoved, FLAG_VAULT_CRATE_MOVED), and the way behind
///   it climbs to the loft (EventScript_ClimbToLoft). Freeing the villager tied
///   up there brings out the three thieves, who attack
///   (EventScript_FreeRooferAndConfrontThieves, FLAG_VAULT_THIEVES_ATTACKED). A
///   loss returns to VAULT_INTERIORS_LOFT_AFTER_LOSING_TO_THIEVES and the party
///   comes to in the Vault Sanctum (VaultInteriors_LoseToThieves); coming back
///   up replays the challenge (VaultInteriors_ConfrontThievesAgain). A win
///   returns to VAULT_INTERIORS_LOFT_AFTER_THIEVES_DEFEATED
///   (VaultInteriors_DefeatThieves, VaultInteriors_ReturnStolenGoods): the
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
/// be read. The cells the events answer are enum VaultInteriorsCell
/// (vault_interiors.h).
///
/// EventScript_ApproachSuspects plays once: it copies
/// VAULT_INTERIORS_CELL_GUESTS_ROOM_DOOR cells over the guests' room's
/// VAULT_INTERIORS_CELL_GUESTS_ROOM cells, EventScript_DiscussSuspiciousGuests
/// makes them VAULT_INTERIORS_CELL_CORNERING_START, and every later arrival
/// with FLAG_VAULT_SUSPECTS_APPROACHED set rewrites them again (render probe
/// vault-interiors: the room has no GUESTS_ROOM cell once the flag is set).
///
/// Open questions: none.

#include "game_state.h"
#include "flags.h"
#include "animspr.h"
#include "version.h"
#include "map_ids.h"
#include "unit/import.h"
#include "unit/map/vault_interiors.h"
#include "vault_interiors.h"
#include "unit/map/common/vault_itemobj.h"
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
#include "audio_engine_symbols.h"

#if TBS_PATCH_3
#define VAULT_INTERIORS_ENTRY_OBJ_EVENT_Z PIXELS(451)
#else
#define VAULT_INTERIORS_ENTRY_OBJ_EVENT_Z PIXELS(464)
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
/// gVaultInteriors_ObjectEventTemplates' rows from LOCALID_FIRST_PLACED;
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

/// Map section flags of Vault's map section (0x300 to 0x3ff, kept while the
/// party stays in Vault and its buildings).
enum {
  /// Set while the two suspects run from the party, until the party steps
  /// aside to talk it over.
  /// ★ Named from the game: "Don't let him near me!" (MsgVaultWant)
  FLAG_SUSPECTS_FLEEING = 0x300
};

// Map flags here:
// - FLAG_MAP_TEMP_50: at the inn once the thieves are jailed, the leader
//   stands beside the Bone on the floor (Vault_UpdateBoneProximityFlag); in
//   the guests' room, set while the party corners the suspects, when talking
//   to Ivan or the suspects does nothing.

/// The inn's loft (VAULT_INTERIORS_LOFT_FROM_CRATE_ROOM and the two after the
/// battle), where the thieves hid the stolen goods, numbering
/// gVaultInteriors_LoftObjectEventTemplates' rows from LOCALID_FIRST_PLACED.
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

static void VaultInteriors_IvanJoinsRodSearch(void);
static void VaultInteriors_DiscussMindReading(void);
static void VaultInteriors_AdvanceSuspectRoute(s32 localId, s32 expected,
                                               s32 next, const s32 *actions);
static void VaultInteriors_FaceSuspectsTowardLeader(void);
static void VaultInteriors_LoseToThieves(void);
static void VaultInteriors_ConfrontThievesAgain(void);
static void VaultInteriors_DefeatThieves(void);
static void VaultInteriors_ReturnStolenGoods(void);
static void VaultInteriors_OpenWindowTransition(void);
static void VaultInteriors_ShowMessageAndWait(s32 localId, s32 waitFrames);
static void VaultInteriors_FaceObjectEventAndWait(s32 localId, s32 targetId,
                                                  s32 waitFrames);
static void VaultInteriors_FaceEachOtherAndWait(s32 firstId, s32 secondId,
                                                s32 waitFrames);
static void VaultInteriors_PlayAnimationAndWait(s32 localId, s32 animation,
                                                s32 waitFrames);
static void VaultInteriors_StartMindRead(s32 casterId, s32 targetId);
static void VaultInteriors_EndMindRead(void);
static void VaultInteriors_UpdateObjectEventStops(void);
static void VaultInteriors_StartObjectEventStops(void);

const struct MapWarpDest *Map_GetWarpDests(void) {
  return gVaultInteriors_WarpDests;
}

/// Returns the one warp region: the box around (648, 416) in the room with the
/// crate, which takes VAULT_INTERIORS_TO_VAULT_WEST back out to Vault.
const struct MapWarpRegion *Map_GetWarpRegions(void) {
  return gVaultInteriors_Regions;
}

const u32 *Map_GetWarps(void) { return gVaultInteriors_Warps; }

/// Places the townspeople, or, for the inn's loft
/// (VAULT_INTERIORS_LOFT_FROM_CRATE_ROOM and the two after the battle), the
/// tied-up villager, the mayor, the thieves, their chests and the found
/// treasures.
const struct ObjectEventTemplate *Map_GetObjectEventTemplates(void) {
  struct ObjectEventTemplate *table;

  switch (gGameState.warpDest) {
  case VAULT_INTERIORS_LOFT_FROM_CRATE_ROOM:
  case VAULT_INTERIORS_LOFT_AFTER_LOSING_TO_THIEVES:
  case VAULT_INTERIORS_LOFT_AFTER_THIEVES_DEFEATED:
    table = gVaultInteriors_LoftObjectEventTemplates;
    break;
  default:
    table = gVaultInteriors_ObjectEventTemplates;
    break;
  }
  Map_PrepareObjectEvents(table);
  return table;
}

#if !TBS_PATCH_1
/// Jumps the leader to the requested destination when nothing blocks it there;
/// returns 1 when it jumped. Only the editions before the first patch have it;
/// the patched ones look ahead instead (Engine_LeaderCheckAhead).
static s32 VaultInteriors_TryJumpTo(struct FieldPosition *position) {
  struct ObjectEvent *leader;
  u8 *flags;
  u8 savedMotionFlags;

  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  flags = &leader->motionFlags;
  savedMotionFlags = *flags;
  if (FieldObject_CheckMovementCollision(leader, position) == 0) {
    Engine_ScriptStart();
    ObjectEvent_SetMode(leader, 6);
    Engine_TaskWait(6);
    Engine_AudioPlayCue(SE_JUMP);
    ObjectEvent_SetMode(leader, 7);
    leader->speed = PIXELS(3);
    leader->acceleration = PIXELS(2);
    leader->velocityY = PIXELS(4);
    *flags &= 0x7e;
    Engine_ObjectEventSetSpriteFlags(leader, 0);
    Engine_ObjectEventMoveToAndWait(
        LOCALID_PARTY_LEADER,
        ((union FieldCoordinate *)&position->x)->part.pixel,
        ((union FieldCoordinate *)&position->z)->part.pixel);
    ObjectEvent_SetMode(leader, 6);
    Engine_ObjectEventSetSpriteFlags(leader, SPRITE_FLAG_SHADOW);
    Engine_TaskWait(6);
    *flags = savedMotionFlags;
    Engine_ScriptEnd();
    return 1;
  }
  return 0;
}
#endif

/// Indoors in Vault, facing north or south across the gap in the roof, the
/// leader jumps it.
///
/// Reached by walking onto VAULT_INTERIORS_CELL_ROOF_GAP. Then copies the cell
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

  if (ObjectEvent_GetById(LOCALID_PARTY_LEADER)->facing >= 0xa000 &&
      ObjectEvent_GetById(LOCALID_PARTY_LEADER)->facing <= 0xe000) {
#if TBS_PATCH_1
    Engine_LeaderCheckAhead();
#else
    leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
    position = &target;
    SET_POSITION(position, leader->x.fixed,
                 ObjectEvent_GetById(LOCALID_PARTY_LEADER)->y.fixed,
                 ObjectEvent_GetById(LOCALID_PARTY_LEADER)->z.fixed -
                     PIXELS(30));
    VaultInteriors_TryJumpTo(position);
#endif
    x = 42;
    z = 85;
    MapGrid_CopyCellAttributes(41, 85, 1, 1, x, z);
  } else if (ObjectEvent_GetById(LOCALID_PARTY_LEADER)->facing >= 0x2000 &&
             ObjectEvent_GetById(LOCALID_PARTY_LEADER)->facing <= 0x6000) {
#if TBS_PATCH_1
    Engine_LeaderCheckAhead();
#else
    leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
    position = &target;
    SET_POSITION(position, leader->x.fixed,
                 ObjectEvent_GetById(LOCALID_PARTY_LEADER)->y.fixed,
                 ObjectEvent_GetById(LOCALID_PARTY_LEADER)->z.fixed +
                     PIXELS(30));
    VaultInteriors_TryJumpTo(position);
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

  Engine_ScriptStart();
  crate = ObjectEvent_GetById(LOCALID_CRATE);
  if ((crate->x.fixed >> 20) == 42) {
    MapGrid_CopyCellAttributes(101, 24, 3, 4, 41, 24);
    Engine_GameFlagSet(FLAG_VAULT_CRATE_MOVED);
  }
  Engine_ScriptEnd();
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

  Engine_ScriptStart();
  if (leaderFacing >= 0xa001 && leaderFacing <= 0xdfff) {
    Engine_ShopOpen(4, LOCALID_WEAPON_MERCHANT);
  } else {
    if (Engine_GameFlagIsSet(FLAG_VAULT_THIEVES_JAILED) == 0) {
      Engine_ScriptSetMessage(MsgVaultMasterHammetIsntOnlyOne);
    } else {
      Engine_ScriptSetMessage(MsgVaultWeFoundOurStolenWeapons);
    }
    Engine_ScriptShowMessage(LOCALID_WEAPON_MERCHANT, 0);
  }
  Engine_ScriptEnd();
}

/// In Vault's weapon and armor shop, across the counter the armor shop opens;
/// otherwise the merchant wants the thieves to pay, or advises preparing for
/// the cave in the Goma Range.
///
/// Reached by talking to the armor merchant (LOCALID_ARMOR_MERCHANT).
static void EventScript_TalkToArmorMerchant(void) {
  s32 leaderFacing = ObjectEvent_GetById(LOCALID_PARTY_LEADER)->facing;

  Engine_ScriptStart();
  if (leaderFacing >= 0xa001 && leaderFacing <= 0xdfff) {
    Engine_ShopOpen(5, LOCALID_ARMOR_MERCHANT);
  } else {
    if (Engine_GameFlagIsSet(FLAG_VAULT_THIEVES_JAILED) == 0) {
      Engine_ScriptSetMessage(MsgVaultThievesDidntHitOurHouse);
    } else {
      Engine_ScriptSetMessage(MsgVaultIfYoureGonnaHeadInto);
    }
    Engine_ScriptShowMessage(LOCALID_ARMOR_MERCHANT, 0);
  }
  Engine_ScriptEnd();
}

/// At Vault's inn, across the counter the inn opens; otherwise the innkeeper
/// defends the guests everyone suspects, or learns the goods were hidden in
/// his loft.
///
/// Reached by talking to the innkeeper (LOCALID_INNKEEPER).
static void EventScript_TalkToInnkeeper(void) {
  s32 leaderFacing = ObjectEvent_GetById(LOCALID_PARTY_LEADER)->facing;

  Engine_ScriptStart();
  if (leaderFacing >= 0xa001 && leaderFacing <= 0xdfff) {
    Engine_InnOpen(1, LOCALID_INNKEEPER);
  } else {
    if (Engine_GameFlagIsSet(FLAG_VAULT_THIEVES_JAILED) == 0) {
      Engine_ScriptSetMessage(MsgVaultEveryoneThinksOurGuestsThieves);
    } else {
      Engine_ScriptSetMessage(MsgVaultTheyHidThoseStolenGoods);
    }
    Engine_ScriptShowMessage(LOCALID_INNKEEPER, 0);
  }
  Engine_ScriptEnd();
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
  Engine_ScriptSetMessage(MsgVaultWeDontHaveTimeFor);
  Engine_ScriptShowMessage(GARET, 0);
}

/// At Vault's inn, Isaac checks the barrel but doesn't find anything.
///
/// Reached by searching VAULT_INTERIORS_CELL_INN_BARREL before the thieves are
/// jailed; after, the same spot holds the Bone (Vault_FindBone).
static void EventScript_SearchEmptyBarrel(void) {
  Engine_ScriptStart();
  Engine_MessageShowCentered(MsgVaultIsaacCheckedBarrel, 1);
  Engine_MessageShowCentered(MsgVaultButDidntFindAnything, 1);
  Engine_ScriptEnd();
}

/// In the inn's loft in Vault, Isaac checks the chest, but it is empty.
///
/// Reached by talking to one of the loft's three chests (LOCALID_FIRST_CHEST to
/// LOCALID_THIRD_CHEST).
static void EventScript_SearchEmptyChest(void) {
  Engine_ScriptStart();
  Engine_MessageShowCentered(MsgVaultIsaacCheckedChest, 1);
  Engine_MessageShowCentered(MsgVaultButChestWasEmpty, 1);
  Engine_ScriptEnd();
}

/// Returns the loft's events for the loft's three warp destinations, else the
/// houses' events, which change once FLAG_VAULT_THIEVES_JAILED is set.
const struct MapEvent *Map_GetEvents(void) {
  switch (gGameState.warpDest) {
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
  Engine_ObjectEventSetAnimation(x, ANIMATION_STAND);
  VaultInteriors_FaceEachOtherAndWait(x, 0, 2);
  Engine_ScriptShowMessage(x, 0);
}

#include "common/vault_talk.h"

/// Faces the leader and selects the next reply when the answer is no.
static void VaultInteriors_AskYesNoAndShowReply(s32 x) {
  VaultInteriors_FaceEachOtherAndWait(x, 0, 2);
  Engine_ScriptOpenMessage(x, 0);
  if (Engine_ScriptChooseYesNo(0, 0) != 0) {
    gFieldWork->message++;
  }
  Engine_ScriptShowMessage(x, 0);
}

/// In a house of Vault, the small boy asks about travel.
/// "Hey, mister! Is it fun to see strange new places?"
///
/// Reached by talking to the boy (LOCALID_BOY_ASKING_ABOUT_TRAVEL) before the
/// thieves are jailed.
static void EventScript_BoyAsksAboutTravel(void) {
  Engine_ScriptStart();
  Engine_ScriptSetMessage(MsgVaultMisterFunSeeStrangeNew);
  Engine_ObjectEventSetAnimation(LOCALID_BOY_ASKING_ABOUT_TRAVEL,
                                 ANIMATION_STAND);
  VaultInteriors_AskYesNoAndShowReply(LOCALID_BOY_ASKING_ABOUT_TRAVEL);
  Engine_ScriptEnd();
}

/// At the mayor's in Vault, a little girl offers to tell Ivan's secret.
/// "Ivan has great powers. Wouldn't you like to know what they are?"
///
/// Reached by talking to LOCALID_GIRL_TELLING_A_SECRET until
/// FLAG_VAULT_IVAN_JOINED_ROD_SEARCH is set; then the row after it tells the
/// secret: Ivan can talk with animals.
static void EventScript_OfferIvansSecret(void) {
  Engine_ScriptStart();
  Engine_ScriptSetMessage(MsgVaultIvanHasGreatPowersWouldnt);
  VaultInteriors_AskYesNoAndShowReply(LOCALID_GIRL_TELLING_A_SECRET);
  ObjectEvent_FaceDirection(LOCALID_GIRL_TELLING_A_SECRET, FACING_WEST, 0);
  Engine_ScriptEnd();
}

/// In Vault's item shop, across the counter the shop opens; otherwise a
/// greeting, or, after the thieves are jailed, the bridge is out.
/// "Did you just arrive in town today?"
///
/// Reached by talking to the item merchant (LOCALID_ITEM_MERCHANT).
static void EventScript_TalkToItemMerchant(void) {
  s32 leaderFacing = ObjectEvent_GetById(LOCALID_PARTY_LEADER)->facing;

  Engine_ScriptStart();
  if (leaderFacing >= 0xa001 && leaderFacing <= 0xdfff) {
    Engine_ShopOpen(6, LOCALID_ITEM_MERCHANT);
  } else if (Engine_GameFlagIsSet(FLAG_VAULT_THIEVES_JAILED) == 0) {
    Engine_ScriptSetMessage(MsgVaultDidJustArriveInTown);
    VaultInteriors_AskYesNoAndShowReply(LOCALID_ITEM_MERCHANT);
  } else {
    Engine_ScriptSetMessage(MsgVaultWithBridgeOutWillQuite);
    Engine_ScriptShowMessage(LOCALID_ITEM_MERCHANT, 0);
  }
  Engine_ScriptEnd();
}

/// The wife of the man with the sparkling rock talks about their rocks.
/// She wishes their rocks were gems, or, after the thieves are jailed, is glad
/// they were caught before stealing the gem.
///
/// Reached by talking to the gem owner's wife (LOCALID_GEM_OWNERS_WIFE).
static void EventScript_TalkAboutRocks(void) {
  Engine_ScriptStart();
  if (Engine_GameFlagIsSet(FLAG_VAULT_THIEVES_JAILED) == 0) {
    Engine_ScriptSetMessage(MsgVaultIfOnlyTheseRocksWere);
  } else {
    Engine_ScriptSetMessage(MsgVaultThankGoodnessThoseThievesWere);
  }
  VaultInteriors_StandAndShowMessage(LOCALID_GEM_OWNERS_WIFE);
  Engine_ScriptEnd();
}

/// Grandpa in the item shop talks about missing things.
/// After the thieves are jailed, he says instead that they hid the treasure in
/// the loft and more in the cemetery.
/// "Is it just me, or am I missing something?"
///
/// Reached by talking to Grandpa (LOCALID_GRANDPA).
static void EventScript_TalkAboutMissingThings(void) {
  Engine_ScriptStart();
  if (Engine_GameFlagIsSet(FLAG_VAULT_THIEVES_JAILED) != 0) {
    Engine_ScriptSetMessage(MsgVaultThievesHidStolenTreasureIn);
  } else {
    Engine_ScriptSetMessage(MsgVaultJustMeOrAmMissing);
  }
  VaultInteriors_StandAndShowMessage(LOCALID_GRANDPA);
  Engine_ScriptEnd();
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
  Engine_ScriptStart();
  if (Engine_GameFlagIsSet(FLAG_VAULT_IVAN_JOINED_ROD_SEARCH) != 0) {
    if (Engine_GameFlagIsSet(thankedForHelpingFlag) == 0) {
      Engine_ScriptSetMessage(MsgVaultYoureGoingHelpIvan);
      VaultInteriors_StandAndShowMessage(LOCALID_MAYOR);
      Engine_ScriptWait(10);
      VaultInteriors_PlayAnimationAndWait(LOCALID_MAYOR, ANIMATION_NOD, 20);
      Engine_GameFlagSet(thankedForHelpingFlag);
    } else {
      Engine_ScriptSetMessage(MsgVaultPleaseLookAfterIvan);
    }
  } else {
    Engine_ScriptSetMessage(MsgVaultCouldSomeonePleaseHelpIvan);
  }
  VaultInteriors_StandAndShowMessage(LOCALID_MAYOR);
  Engine_ScriptEnd();
}

/// At Vault's inn, the girl at the inn says the three strangers have fooled
/// the innkeeper and his wife.
///
/// Reached by talking to the girl at the inn (LOCALID_GIRL_AT_THE_INN) before
/// the thieves are jailed.
static void EventScript_WarnAboutThreeStrangers(void) {
  Engine_ScriptStart();
  Engine_ScriptSetMessage(MsgVaultThoseThreeStrangersSureHave);
  VaultInteriors_StandAndShowMessage(LOCALID_GIRL_AT_THE_INN);
  Engine_ScriptEnd();
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
  Engine_ScriptStart();
  Engine_ObjectEventRunRepeatedMotion(LOCALID_SUSPECTED_LEADER, 2);
  Engine_ScriptSetMessage(MsgVaultOwStop);
  Engine_ScriptShowMessage(LOCALID_SUSPECTED_LEADER, 0);
  Engine_ObjectEventSetSpeed(LOCALID_SUSPECTED_LEADER, PIXELS(4), PIXELS(2));
  if ((u32)((suspect->facing & 0xf000) - 0x5000) <= 0x6000) {
    routeState = (s16 *)&suspect->timer;
    routeIndex = *routeState;
    if (routeIndex <= 2) {
      ObjectEvent_SetActionScript(LOCALID_SUSPECTED_LEADER,
                                  gVaultInteriors_StepActions[routeIndex]);
      *(u16 *)routeState = *(u16 *)routeState + 1;
    } else {
      ObjectEvent_SetActionScript(LOCALID_SUSPECTED_LEADER,
                                  gVaultInteriors_IdleActions[routeIndex]);
      nextRoute = *(u16 *)routeState - 1;
      *(u16 *)routeState = nextRoute;
    }
  } else {
    routeState = (s16 *)&suspect->timer;
    routeIndex = *routeState;
    if (routeIndex > 2) {
      ObjectEvent_SetActionScript(LOCALID_SUSPECTED_LEADER,
                                  gVaultInteriors_StepActions[routeIndex]);
      *(u16 *)routeState = *(u16 *)routeState + 1;
    } else {
      ObjectEvent_SetActionScript(LOCALID_SUSPECTED_LEADER,
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
  ObjectEvent_RefreshSelectorById(LOCALID_SUSPECTED_LEADER);
  Engine_ScriptEnd();
}

/// Selects a response script and advances the second suspect's route state.
static inline void
VaultInteriors_ApplySecondSuspectRoute(struct ObjectEvent *objectEvent,
                                       s32 scriptRow, s32 routePair,
                                       u16 routeDelta) {
  ObjectEvent_SetActionScript(
      LOCALID_SUSPECTED_THIEF,
      gVaultInteriors_ResponseScripts[scriptRow * 4 + objectEvent->timer]);
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
  Engine_ScriptStart();
  Engine_ObjectEventRunRepeatedMotion(LOCALID_SUSPECTED_THIEF, 2);
  Engine_ScriptSetMessage(MsgVaultNoLeaveAlone);
  Engine_ScriptShowMessage(LOCALID_SUSPECTED_THIEF, 0);
  Engine_ObjectEventSetSpeed(LOCALID_SUSPECTED_THIEF, PIXELS(7) / 2,
                             PIXELS(7) / 4);
  switch (objectEvent->timer) {
  case 4:
    if (facing > 0x2000 && facing < 0xa000) {
      ObjectEvent_SetActionScript(
          LOCALID_SUSPECTED_THIEF,
          gVaultInteriors_SuspectAlongToSouthEastCorner);
      objectEvent->timer = 2;
    } else {
      ObjectEvent_SetActionScript(
          LOCALID_SUSPECTED_THIEF,
          gVaultInteriors_SuspectAlongToSouthWestCorner);
      objectEvent->timer = 3;
    }
    break;
  case 0:
  case 2:
    if (facing > 0x2000 && facing < 0xa000) {
      VaultInteriors_ApplySecondSuspectRoute(objectEvent, routePair, routePair,
                                             1);
    } else {
      VaultInteriors_ApplySecondSuspectRoute(objectEvent, routePair ^ 1,
                                             routePair, -1);
    }
    break;
  case 1:
  case 3:
    if (facing > 0x6000 && facing < 0xe000) {
      VaultInteriors_ApplySecondSuspectRoute(objectEvent, routePair, routePair,
                                             1);
    } else {
      VaultInteriors_ApplySecondSuspectRoute(objectEvent, routePair ^ 1,
                                             routePair, -1);
    }
    break;
  }
  objectEvent->timer &= 3;
  ObjectEvent_RefreshSelectorById(LOCALID_SUSPECTED_THIEF);
  Engine_ScriptEnd();
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
  Engine_ScriptStart();
  Engine_ScriptSetMessage(MsgVaultDidThoseThievesComeFrom);
  VaultInteriors_AskYesNoAndShowReply(LOCALID_VILLAGER_SUSPECTING_THE_NEIGHBOR);
  Engine_ScriptEnd();
}

/// The boy says Father worries about the prisoners from Lunpa.
/// He's in a house of Vault, and says it after the thieves are jailed.
static void EventScript_BoyWorriesAboutFather(void) {
  Engine_ScriptStart();
  Engine_ScriptSetMessage(MsgVaultMyFatherWorriedAboutThose);
  VaultInteriors_StandAndShowMessage(LOCALID_BOY_ASKING_ABOUT_TRAVEL);
  Engine_ScriptEnd();
}

/// At the mayor's in Vault after the thieves are jailed, a villager asks about
/// defeating them. "I heard you defeated those thieves!"
static void EventScript_AskAboutDefeatingThieves(void) {
  Engine_ScriptStart();
  Engine_ScriptSetMessage(MsgVaultHeardDefeatedThoseThieves);
  VaultInteriors_AskYesNoAndShowReply(LOCALID_BOY_WISHING_TO_BE_OLDER);
  Engine_ScriptEnd();
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
  Engine_ScriptStart();
  if (GameFlag_IsSet(FLAG_VAULT_WATER_OF_LIFE_RECEIVED) == 0) {
    Engine_ScriptSetMessage(MsgVaultYouWereSuchGreatHelp);
    VaultInteriors_ShowMessageAndWait(LOCALID_MAYOR, 20);
    VaultInteriors_PlayAnimationAndWait(LOCALID_MAYOR, ANIMATION_NOD, 20);
    VaultInteriors_ShowMessageAndWait(LOCALID_MAYOR, 30);
    ObjectEvent_FaceDirection(LOCALID_MAYOR, FACING_EAST, 0);
    Engine_ScriptWait(30);
    Engine_ObjectEventRunRepeatedMotion(LOCALID_MAYOR, 2);
    Engine_ScriptWait(30);
    VaultInteriors_FaceEachOtherAndWait(LOCALID_PARTY_LEADER, LOCALID_MAYOR,
                                        20);
    VaultInteriors_PlayAnimationAndWait(LOCALID_MAYOR, ANIMATION_NOD, 20);
    gFieldWork->message++;
    if (PartyInventory_HasSpace() == 0) {
      Engine_ScriptSetMessage(MsgVaultYouWereSuchGreatHelp + 3);
      VaultInteriors_ShowMessageAndWait(LOCALID_MAYOR, 20);
      Engine_ScriptEnd();
      return;
    }
    GameFlag_Set(FLAG_VAULT_WATER_OF_LIFE_RECEIVED);
    Engine_PartyGiveItem(ITEM_WATER_OF_LIFE, 0);
  }
  Engine_ScriptSetMessage(MsgVaultHeadingOutBeyondGomaRange);
  Script_OpenMessage(LOCALID_MAYOR, 0);
  Engine_ScriptWait(20);
  if (Engine_ScriptChooseYesNo(0, 0) != 0) {
    gFieldWork->message++;
  }
  Script_ShowMessage(LOCALID_MAYOR, 0);
  Engine_ScriptEnd();
}

/// At Vault's inn after the thieves are jailed, the girl at the inn offers
/// leftover bones from dinner; a yes gets a Bone if there is room, a no a nod.
///
/// Reached by talking to the girl at the inn (LOCALID_GIRL_AT_THE_INN) after
/// the thieves are jailed, while the leader is not beside the Bone on the floor
/// (FLAG_MAP_TEMP_50 clear). Gives a Bone and sets FLAG_VAULT_BONE_RECEIVED.
static void EventScript_OfferBones(void) {
  Engine_ScriptStart();
  VaultInteriors_FaceEachOtherAndWait(LOCALID_GIRL_AT_THE_INN,
                                      LOCALID_PARTY_LEADER, 2);
  if (GameFlag_IsSet(FLAG_VAULT_BONE_RECEIVED) == 0) {
    Engine_ScriptSetMessage(MsgVaultHaveLotLeftoverBonesFrom);
    Script_OpenMessage(LOCALID_GIRL_AT_THE_INN, 0);
  } else {
    Engine_ScriptSetMessage(MsgVaultWantMoreBones);
    Script_OpenMessage(LOCALID_GIRL_AT_THE_INN, 0);
  }
  if (Engine_ScriptChooseYesNo(0, 0) == 0) {
    Engine_ScriptWait(20);
    Script_ShowMessage(LOCALID_GIRL_AT_THE_INN, 0);
    Engine_ScriptWait(20);
    Engine_ObjectEventRunRepeatedMotion(LOCALID_GIRL_AT_THE_INN, 2);
    Engine_ScriptWait(20);
    if (PartyInventory_HasSpace() == 0) {
      Engine_ObjectEventSetAnimationAndWait(LOCALID_GIRL_AT_THE_INN,
                                            ANIMATION_SHAKE_HEAD);
      Engine_ScriptWait(20);
      Engine_ScriptSetMessage(MsgVaultWowHaveManyThingsArent);
      Script_ShowMessage(LOCALID_GIRL_AT_THE_INN, 0);
    } else {
      Engine_ItemShowFound(ITEM_BONE, 3);
      Engine_PartyGiveItem(ITEM_BONE, 0);
      GameFlag_Set(FLAG_VAULT_BONE_RECEIVED);
    }
  } else {
    gFieldWork->message++;
    Engine_ScriptWait(20);
    Engine_ObjectEventSetAnimationAndWait(LOCALID_GIRL_AT_THE_INN,
                                          ANIMATION_NOD);
    Engine_ScriptWait(20);
    Script_ShowMessage(LOCALID_GIRL_AT_THE_INN, 0);
  }
  ObjectEvent_FaceDirection(LOCALID_GIRL_AT_THE_INN, FACING_SOUTH, 0);
  Engine_ScriptEnd();
}

/// In a house of Vault, Mind Read on the wife of the man with the sparkling
/// rock: let me see that rock, or maybe it is worthless.
///
/// Reached by Mind Read on the gem owner's wife (LOCALID_GEM_OWNERS_WIFE); the
/// Mind Read events below are reached the same way on the object event each
/// names, before and after the thieves are jailed (FLAG_VAULT_THIEVES_JAILED
/// picks the thought).
static void EventScript_ReadMindOnRock(void) {
  Engine_ScriptStart();
  if (Engine_GameFlagIsSet(FLAG_VAULT_THIEVES_JAILED) == 0) {
    Engine_ScriptSetMessage(MsgVaultOkMisterLetMeSee);
  } else {
    Engine_ScriptSetMessage(MsgVaultIfRockWorthlessMaybeThats);
  }
  Vault_ShowThought(LOCALID_GEM_OWNERS_WIFE);
  Engine_ScriptEnd();
}

/// In a house of Vault, Mind Read on the boy: what is the outside world
/// like, or Father looks sad.
static void EventScript_ReadBoysMind(void) {
  Engine_ScriptStart();
  if (Engine_GameFlagIsSet(FLAG_VAULT_THIEVES_JAILED) == 0) {
    Engine_ScriptSetMessage(MsgVaultWonderOutsideWorldLike);
  } else {
    Engine_ScriptSetMessage(MsgVaultFatherLooksSadWorryingLike);
  }
  Vault_ShowThought(LOCALID_BOY_ASKING_ABOUT_TRAVEL);
  Engine_ScriptEnd();
}

/// In Vault's item shop, Mind Read on Grandpa: could they be thieves? Who can
/// be trusted? Or nothing in the house was worth stealing.
static void EventScript_ReadMindOnThieves(void) {
  Engine_ScriptStart();
  if (Engine_GameFlagIsSet(FLAG_VAULT_THIEVES_JAILED) == 0) {
    Engine_ScriptSetMessage(MsgVaultCouldTheyThievesGoodDont);
  } else {
    Engine_ScriptSetMessage(MsgVaultGuessNothingInOurHouse);
  }
  Vault_ShowThought(LOCALID_GRANDPA);
  Engine_ScriptEnd();
}

/// At the mayor's in Vault, Mind Read on the mayor: a laugh, or the cave in
/// the Goma Range is dangerous, but the party will be okay. "That tickles..."
static void EventScript_ReadMayorMind(void) {
  Engine_ScriptStart();
  if (Engine_GameFlagIsSet(FLAG_VAULT_THIEVES_JAILED) == 0) {
    Engine_ScriptSetMessage(MsgVaultTicklesBeingTickledByBoy);
  } else {
    Engine_ScriptSetMessage(MsgVaultCaveInGomaRangeDangerous);
  }
  Vault_ShowThought(LOCALID_MAYOR);
  Engine_ScriptEnd();
}

/// At Vault's inn, Mind Read on the girl at the inn: the master and his wife
/// are blinded by money; after the thieves are jailed, hungry while cooking,
/// or, once a Bone was given, wondering about calcium.
static void EventScript_ReadBoneGiverMind(void) {
  Engine_ScriptStart();
  if (Engine_GameFlagIsSet(FLAG_VAULT_THIEVES_JAILED) == 0) {
    Engine_ScriptSetMessage(MsgVaultMasterHisWifeBlindedBy);
  } else if (Engine_GameFlagIsSet(FLAG_VAULT_BONE_RECEIVED) == 0) {
    Engine_ScriptSetMessage(MsgVaultGeeAlwaysGetHungryWhen);
  } else {
    Engine_ScriptSetMessage(MsgVaultHeReallyLikesBonesWonder);
  }
  Vault_ShowThought(LOCALID_GIRL_AT_THE_INN);
  Engine_ScriptEnd();
}

/// At the mayor's in Vault, talking to Ivan: the first time, he reads Isaac's
/// mind, admits his strange powers and learns they are called Psynergy; then
/// he asks for help finding the rod. A yes has him join the party.
///
/// Reached by talking to Ivan, whose row takes his party slot (IVAN), except
/// while the party corners the suspects. Sets FLAG_VAULT_MET_IVAN the first
/// time, and on a yes plays VaultInteriors_IvanJoinsRodSearch, sets
/// FLAG_VAULT_IVAN_JOINED_ROD_SEARCH and has Ivan walk into the leader.
static void EventScript_MeetIvan(void) {

  struct ObjectEvent *objectEvent;

  Engine_ScriptStart();
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_SetSpeed(GARET, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_SetSpeed(IVAN, PIXELS(4) / 5, PIXELS(2) / 5);
  // Isaac steps up in front of Ivan and Garet comes out beside him.
  Script_PlaySong(MUS_SILENCE);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 384, 408);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  ObjectEvent_SetPosition(GARET, PIXELS(384), PIXELS(408));
  ObjectEvent_WalkToAndWait(GARET, 368, 408);
  ObjectEvent_FaceDirection(GARET, FACING_NORTH + FACING_STEP, 20);
  // Introduce Ivan's ability only on the first meeting.
  if (GameFlag_IsSet(FLAG_VAULT_MET_IVAN) == 0) {
    GameFlag_Set(FLAG_VAULT_MET_IVAN);
    // Ivan reads Isaac's mind and admits his strange powers.
    VaultInteriors_StartMindRead(IVAN, LOCALID_PARTY_LEADER);
    Engine_ScriptWait(40);
    VaultInteriors_EndMindRead();
    Engine_ScriptSetMessage(MsgVaultDoPossessStrangePowers);
    Script_PlaySong(MUS_060);
    Engine_ScriptWait(30);
    VaultInteriors_PlayAnimationAndWait(IVAN, ANIMATION_NOD, 30);
    VaultInteriors_ShowMessageAndWait(IVAN, 30);
    // The two start; he reads again: "You have them, too, don't you?"
    Engine_ObjectEventStartRepeatedMotion(LOCALID_PARTY_LEADER, 1);
    Engine_ObjectEventRunRepeatedMotion(GARET, 1);
    Engine_ScriptWait(20);
    VaultInteriors_StartMindRead(IVAN, LOCALID_PARTY_LEADER);
    Engine_ScriptWait(40);
    VaultInteriors_EndMindRead();
    VaultInteriors_ShowMessageAndWait(IVAN, 30);
    VaultInteriors_FaceEachOtherAndWait(LOCALID_PARTY_LEADER, GARET, 50);
    ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
    ObjectEvent_FaceDirection(GARET, FACING_NORTH + FACING_STEP, 0);
    Engine_ScriptWait(20);
    // A third reading: his power is called Psynergy; then they have so much
    // in common he can trust them.
    VaultInteriors_StartMindRead(IVAN, LOCALID_PARTY_LEADER);
    Engine_ScriptWait(40);
    VaultInteriors_EndMindRead();
    Engine_ObjectEventRunRepeatedMotion(IVAN, 1);
    VaultInteriors_ShowMessageAndWait(IVAN, 50);
    Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_NOD);
    VaultInteriors_PlayAnimationAndWait(GARET, ANIMATION_NOD, 20);
    VaultInteriors_PlayAnimationAndWait(IVAN, ANIMATION_NOD, 20);
    VaultInteriors_ShowMessageAndWait(IVAN, 40);
    // He walks off a step, turns back and tells of Hammet's stolen rod.
    Engine_ObjectEventRunRepeatedMotion(IVAN, 1);
    Engine_ScriptWait(30);
    ObjectEvent_FaceDirection(IVAN, FACING_NORTH, 0);
    Engine_ScriptWait(30);
    ObjectEvent_WalkToAndWait(IVAN, 376, 376);
    Engine_ScriptWait(40);
    VaultInteriors_FaceEachOtherAndWait(LOCALID_PARTY_LEADER, GARET, 50);
    ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
    ObjectEvent_FaceDirection(GARET, FACING_NORTH + FACING_STEP, 0);
    Engine_ObjectEventRunRepeatedMotion(IVAN, 1);
    Engine_ScriptWait(50);
    VaultInteriors_PlayAnimationAndWait(IVAN, ANIMATION_NOD, 30);
    ObjectEvent_FaceDirection(IVAN, FACING_SOUTH, 0);
    Engine_ScriptWait(10);
    Script_ShowMessage(IVAN, 0);
    // "Would you please help me get my master's rod back?"
    Script_OpenMessage(IVAN, 0);
  } else {
    // Asked again: "Would you really? Would you help me find the rod?"
    Script_PlaySong(MUS_060);
    Engine_ScriptSetMessage(MsgVaultWouldReallyWouldHelpMe);
    Script_OpenMessage(IVAN, 0);
  }
  // Accepting the search lets Ivan merge into the leader's position; a no gets
  // his apology for asking.
  if (Engine_ScriptChooseYesNo(0, 0) == 0) {
    VaultInteriors_IvanJoinsRodSearch();
    GameFlag_Set(FLAG_VAULT_IVAN_JOINED_ROD_SEARCH);
    Engine_ObjectEventSetAnimation(IVAN, ANIMATION_WALK);
    objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
    if (objectEvent != 0) {
      ObjectEvent_SetDestination(IVAN, objectEvent->x.part.pixel,
                                 objectEvent->z.part.pixel);
    }
    Engine_ObjectEventWaitForMove(IVAN);
    ObjectEvent_SetPosition(IVAN, 0, 0);
  } else {
    Script_ShowMessage(IVAN, 0);
  }
  // Garet walks back into the leader and the earlier music returns.
  Engine_ObjectEventSetAnimation(GARET, ANIMATION_WALK);
  objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (objectEvent != 0) {
    ObjectEvent_SetDestination(GARET, objectEvent->x.part.pixel,
                               objectEvent->z.part.pixel);
  }
  Engine_ObjectEventWaitForMove(GARET);
  ObjectEvent_SetPosition(GARET, 0, 0);
  Map_PlayMusic();
  Engine_ScriptEnd();
}

/// Ivan thanks them, they exchange names, and he shows Isaac and Garet Mind
/// Read; he joins the party.
///
/// Called by EventScript_MeetIvan when the party agrees to help. Adds Ivan to
/// the party ("Ivan joined your party!").
static void VaultInteriors_IvanJoinsRodSearch(void) {
  s32 message;

  // Ivan jumps for joy: "Thank you. With your help, I believe we can recover
  // the rod." Then he asks their names.
  Engine_ScriptWait(20);
  ObjectEvent_GetById(IVAN)->paused = 0;
  Engine_ObjectEventJump(IVAN, 4, 0);
  Engine_ScriptWait(40);
  message = MsgVaultThankHelpBelieve;
  Engine_ScriptSetMessage(message);
  Engine_ScriptWait(20);
  VaultInteriors_ShowMessageAndWait(IVAN, 20);
  ObjectEvent_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Engine_ScriptWait(50);
  VaultInteriors_ShowMessageAndWait(IVAN, 30);
  // He reads their names from Isaac's mind; Isaac backs away from him.
  ObjectEvent_WalkToAndWait(IVAN, 376, 392);
  VaultInteriors_StartMindRead(IVAN, LOCALID_PARTY_LEADER);
  Engine_ScriptWait(40);
  VaultInteriors_EndMindRead();
  Engine_ObjectEventRunRepeatedMotion(LOCALID_PARTY_LEADER, 1);
  ObjectEvent_GetById(LOCALID_PARTY_LEADER)->steeringFlags &=
      ~OBJ_EVENT_FACES_MOTION;
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 384, 424);
  Engine_ScriptWait(1);
  ObjectEvent_GetById(LOCALID_PARTY_LEADER)->steeringFlags |=
      OBJ_EVENT_FACES_MOTION;
  Engine_ScriptWait(30);
  Engine_ScriptShowMessage(IVAN, 0);
  // Demonstrate Mind Read and let Garet and Isaac react to Ivan's explanation.
  ObjectEvent_SetAttachedEffect(LOCALID_PARTY_LEADER, ATTACHED_EFFECT_IN_FRONT |
                                                          ATTACHED_EFFECT_ONCE);
  ObjectEvent_SetAttachedEffect(GARET, ATTACHED_EFFECT_IN_FRONT |
                                           ATTACHED_EFFECT_ONCE);
  Engine_ScriptWait(60);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_NOD);
  VaultInteriors_PlayAnimationAndWait(GARET, ANIMATION_NOD, 30);
  ObjectEvent_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Engine_ScriptWait(60);
  Engine_ObjectEventRunRepeatedMotion(IVAN, 1);
  Engine_ScriptWait(10);
  VaultInteriors_PlayAnimationAndWait(IVAN, ANIMATION_NOD, 20);
  Engine_ScriptShowMessage(IVAN, 0);
  Engine_ObjectEventRunRepeatedMotion(GARET, 2);
  Engine_ScriptWait(10);
  VaultInteriors_FaceObjectEventAndWait(GARET, LOCALID_PARTY_LEADER, 30);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_PARTY_LEADER, 2);
  Engine_ScriptWait(10);
  VaultInteriors_FaceObjectEventAndWait(LOCALID_PARTY_LEADER, GARET, 40);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Engine_ScriptWait(60);
  // "Is my Mind Read so frightening?": either answer goes on the same way.
  ObjectEvent_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Engine_ScriptWait(60);
  Engine_ScriptOpenMessage(IVAN, 0);
  Engine_ObjectEventFaceObjectEvent(LOCALID_PARTY_LEADER, IVAN, 0);
  Engine_ObjectEventFaceObjectEvent(GARET, IVAN, 0);
  Engine_ScriptChooseYesNo(0, 0);
  Engine_ScriptWait(30);
  // Ivan and Isaac clasp hands, and Garet cries out that his mind is read.
  VaultInteriors_PlayAnimationAndWait(IVAN, ANIMATION_NOD, 10);
  ObjectEvent_WalkToAndWait(IVAN, 384, 408);
  Engine_ScriptWait(10);
  Engine_MessageShowCentered(message + 5, 1);
  Engine_ScriptSetMessage(message + 6);
  Engine_ObjectEventFaceEachOther(IVAN, GARET, 0);
  VaultInteriors_FaceObjectEventAndWait(LOCALID_PARTY_LEADER, GARET, 20);
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Engine_ScriptWait(60);
  Engine_ObjectEventRunRepeatedMotion(GARET, 1);
  ObjectEvent_GetById(GARET)->steeringFlags &= ~OBJ_EVENT_FACES_MOTION;
  ObjectEvent_WalkToAndWait(GARET, 352, 408);
  Engine_ScriptWait(1);
  ObjectEvent_GetById(GARET)->steeringFlags |= OBJ_EVENT_FACES_MOTION;
  Engine_ScriptWait(10);
  // Garet backs off; Ivan asks whether Isaac could read Garet's mind, and the
  // two read it together. "Just as I thought."
  ObjectEvent_WalkTo(IVAN, 368, 408);
  Engine_ScriptWait(20);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 368, 424);
  Engine_ObjectEventFaceObjectEvent(LOCALID_PARTY_LEADER, GARET, 0);
  Engine_ObjectEventWaitForMove(IVAN);
  VaultInteriors_StartMindRead(IVAN, GARET);
  VaultInteriors_PlayAnimationAndWait(GARET, ANIMATION_SHAKE_HEAD, 10);
  Engine_ScriptShowMessage(GARET, 0);
  VaultInteriors_EndMindRead();
  VaultInteriors_FaceEachOtherAndWait(IVAN, LOCALID_PARTY_LEADER, 30);
  Engine_ScriptShowMessage(IVAN, 0);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_PARTY_LEADER, 1);
  Engine_ScriptWait(30);
  VaultInteriors_PlayAnimationAndWait(LOCALID_PARTY_LEADER, ANIMATION_NOD, 30);
  VaultInteriors_PlayAnimationAndWait(IVAN, ANIMATION_NOD, 10);
  Engine_ScriptShowMessage(IVAN, 0);
  Engine_ObjectEventRunRepeatedMotion(GARET, 2);
  Engine_ObjectEventFaceObjectEvent(GARET, LOCALID_PARTY_LEADER, 0);
  Engine_ScriptShowMessage(GARET, 0);
  Engine_ObjectEventFaceObjectEvent(LOCALID_PARTY_LEADER, GARET, 0);
  VaultInteriors_PlayAnimationAndWait(LOCALID_PARTY_LEADER, ANIMATION_NOD, 40);
  Engine_ObjectEventFaceObjectEvent(LOCALID_PARTY_LEADER, IVAN, 0);
  VaultInteriors_FaceObjectEventAndWait(GARET, IVAN, 30);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION,
                        0);
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Engine_ScriptWait(60);
  // Sharing their Psynergy let Isaac read minds too; Garet protests that it
  // isn't fair, and Ivan agrees it is one-way.
  VaultInteriors_PlayAnimationAndWait(IVAN, ANIMATION_NOD, 20);
  Engine_ScriptShowMessage(IVAN, 0);
  Engine_ObjectEventShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
  Engine_ScriptWait(60);
  VaultInteriors_FaceObjectEventAndWait(GARET, LOCALID_PARTY_LEADER, 10);
  Engine_ScriptShowMessage(GARET, 0);
  Engine_ObjectEventShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Engine_ScriptWait(60);
  Engine_ScriptShowMessage(IVAN, 0);
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Engine_ScriptWait(60);
  // Ivan and Garet clasp hands, and Garet reads Isaac's mind.
  Engine_MessageShowCentered(message + 13, 1);
  Engine_ScriptSetMessage(message + 14);
  Engine_ObjectEventFaceEachOther(IVAN, LOCALID_PARTY_LEADER, 0);
  Engine_ObjectEventFaceObjectEvent(GARET, LOCALID_PARTY_LEADER, 0);
  Engine_ObjectEventShowEmote(LOCALID_PARTY_LEADER,
                              EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Engine_ScriptWait(60);
  VaultInteriors_StartMindRead(IVAN, LOCALID_PARTY_LEADER);
  Ui_SetBank15PaletteAndClearRenderMode();
  VaultInteriors_ShowMessageAndWait(GARET, 30);
  VaultInteriors_EndMindRead();
  Engine_ObjectEventFaceEachOther(IVAN, GARET, 0);
  VaultInteriors_PlayAnimationAndWait(IVAN, ANIMATION_NOD, 10);
  VaultInteriors_ShowMessageAndWait(IVAN, 20);
  Engine_ObjectEventFaceEachOther(IVAN, LOCALID_PARTY_LEADER, 0);
  VaultInteriors_PlayAnimationAndWait(LOCALID_PARTY_LEADER, ANIMATION_NOD, 20);
  Engine_ObjectEventRunRepeatedMotion(GARET, 1);
  Engine_ScriptWait(20);
  // "Let's use Mind Read to find the thieves!" Garet worries people will
  // notice.
  VaultInteriors_PlayAnimationAndWait(GARET, ANIMATION_SHAKE_HEAD, 10);
  VaultInteriors_ShowMessageAndWait(GARET, 30);
  Engine_ObjectEventFaceObjectEvent(IVAN, GARET, 0);
  ObjectEvent_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Engine_ScriptWait(30);
  VaultInteriors_FaceEachOtherAndWait(LOCALID_PARTY_LEADER, GARET, 10);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_NOD);
  VaultInteriors_PlayAnimationAndWait(GARET, ANIMATION_NOD, 30);
  Engine_ObjectEventFaceObjectEvent(LOCALID_PARTY_LEADER, IVAN, 0);
  Engine_ObjectEventFaceObjectEvent(GARET, IVAN, 0);
  Engine_ObjectEventFaceObjectEvent(IVAN, LOCALID_PARTY_LEADER, 0);
  // Only Adepts can tell when Ivan uses Psynergy; ordinary people see
  // nothing, and he promises not to read their minds again.
  Engine_ObjectEventShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 0);
  Engine_ScriptWait(60);
  VaultInteriors_ShowMessageAndWait(IVAN, 10);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_NOD);
  VaultInteriors_PlayAnimationAndWait(GARET, ANIMATION_NOD, 20);
  VaultInteriors_PlayAnimationAndWait(IVAN, ANIMATION_SHAKE_HEAD, 20);
  VaultInteriors_ShowMessageAndWait(IVAN, 20);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION,
                        0);
  Engine_ObjectEventShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Engine_ScriptWait(60);
  VaultInteriors_PlayAnimationAndWait(IVAN, ANIMATION_NOD, 10);
  VaultInteriors_ShowMessageAndWait(IVAN, 30);
  Engine_ObjectEventShowEmote(LOCALID_PARTY_LEADER,
                              EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Engine_ScriptWait(60);
  VaultInteriors_PlayAnimationAndWait(IVAN, ANIMATION_SHAKE_HEAD, 10);
  VaultInteriors_ShowMessageAndWait(IVAN, 30);
  VaultInteriors_PlayAnimationAndWait(IVAN, ANIMATION_NOD, 10);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_NOD);
  VaultInteriors_PlayAnimationAndWait(GARET, ANIMATION_NOD, 20);
  // "Ivan joined your party!", then he will read the townspeople's minds.
  FieldScript_PrepareObjectAndApplyValue(IVAN, 1);
  Engine_ScriptSetMessage(message + 22);
  Engine_ObjectEventRunRepeatedMotion(IVAN, 1);
  Engine_ScriptShowMessage(IVAN, 0);
}

/// In the inn's loft in Vault, Isaac cuts the tied-up villager free; the three
/// thieves arrive, tell of Hammet in Lunpa and of Dodonpa, ask to be let go,
/// and attack.
///
/// Reached by talking to the tied-up villager (LOCALID_TIED_UP_VILLAGER) in the
/// loft; the loft's setup replays the thieves' part through
/// VaultInteriors_ConfrontThievesAgain once FLAG_VAULT_THIEVES_ATTACKED is
/// set. Sets that flag and starts the battle (encounter 12, 5): a win returns
/// to VAULT_INTERIORS_LOFT_AFTER_THIEVES_DEFEATED, a loss to
/// VAULT_INTERIORS_LOFT_AFTER_LOSING_TO_THIEVES.
static void EventScript_FreeRooferAndConfrontThieves(void) {
  s32 base;

  Engine_ScriptStart();
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_SetSpeed(GARET, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_SetSpeed(IVAN, PIXELS(4) / 5, PIXELS(2) / 5);
  // Isaac cuts the tied-up villager free from the ropes; he gets up and nods.
  Engine_ObjectEventRunRepeatedMotion(LOCALID_PARTY_LEADER, 3);
  Engine_ScriptWait(20);
  base = MsgVaultVaultCutFree;
  Engine_MessageShowCentered(base, 1);
  base++;
  Engine_ScriptSetMessage(base);
  Engine_ScriptWait(30);
  Engine_ObjectEventSetAnimation(LOCALID_TIED_UP_VILLAGER, ANIMATION_STAND);
  Engine_ScriptWait(60);
  VaultInteriors_PlayAnimationAndWait(LOCALID_TIED_UP_VILLAGER, ANIMATION_NOD,
                                      40);
  // Garet and Ivan come out of the leader around the tied-up villager.
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 792, 440);
  VaultInteriors_FaceObjectEventAndWait(LOCALID_PARTY_LEADER,
                                        LOCALID_TIED_UP_VILLAGER, 20);
  ObjectEvent_SetPosition(GARET, PIXELS(792), PIXELS(440));
  ObjectEvent_SetPosition(IVAN, PIXELS(792), PIXELS(440));
  ObjectEvent_WalkTo(GARET, 808, 432);
  ObjectEvent_WalkTo(IVAN, 792, 456);
  Engine_ObjectEventWaitForMove(GARET);
  Engine_ObjectEventFaceObjectEvent(GARET, LOCALID_TIED_UP_VILLAGER, 0);
  Engine_ObjectEventWaitForMove(IVAN);
  VaultInteriors_FaceObjectEventAndWait(IVAN, LOCALID_TIED_UP_VILLAGER, 60);
  // The tied-up villager tells how he came up to fix the roof, was whacked from
  // behind and woke up tied.
  ObjectEvent_ShowEmote(LOCALID_TIED_UP_VILLAGER, EMOTE_IN_FRONT | EMOTE_WINCE,
                        0);
  Engine_ScriptWait(60);
  VaultInteriors_ShowMessageAndWait(LOCALID_TIED_UP_VILLAGER, 20);
  Engine_ObjectEventSetAnimation(IVAN, ANIMATION_NOD);
  VaultInteriors_PlayAnimationAndWait(GARET, ANIMATION_NOD, 30);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_TIED_UP_VILLAGER, 2);
  Engine_ScriptWait(20);
  VaultInteriors_ShowMessageAndWait(LOCALID_TIED_UP_VILLAGER, 20);
  ObjectEvent_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Engine_ScriptWait(60);
  ObjectEvent_FaceDirection(LOCALID_TIED_UP_VILLAGER,
                            FACING_SOUTHEAST + FACING_STEP, 0);
  Engine_ScriptWait(10);
  VaultInteriors_ShowMessageAndWait(LOCALID_TIED_UP_VILLAGER, 30);
  // He looks around at chests he doesn't remember; Ivan guesses the stolen
  // goods are in them.
  ObjectEvent_FaceDirection(LOCALID_TIED_UP_VILLAGER,
                            FACING_NORTH + FACING_STEP, 0);
  Engine_ScriptWait(30);
  ObjectEvent_ShowEmote(LOCALID_TIED_UP_VILLAGER,
                        EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Engine_ScriptWait(60);
  ObjectEvent_FaceDirection(LOCALID_TIED_UP_VILLAGER,
                            FACING_NORTHWEST + FACING_STEP, 0);
  Engine_ScriptWait(40);
  ObjectEvent_FaceDirection(LOCALID_TIED_UP_VILLAGER,
                            FACING_NORTH + FACING_STEP, 0);
  Engine_ScriptWait(40);
  VaultInteriors_FaceObjectEventAndWait(LOCALID_TIED_UP_VILLAGER,
                                        LOCALID_PARTY_LEADER, 20);
  VaultInteriors_PlayAnimationAndWait(LOCALID_TIED_UP_VILLAGER,
                                      ANIMATION_SHAKE_HEAD, 30);
  VaultInteriors_ShowMessageAndWait(LOCALID_TIED_UP_VILLAGER, 20);
  Engine_ObjectEventRunRepeatedMotion(IVAN, 1);
  VaultInteriors_ShowMessageAndWait(IVAN, 40);
  // The thieves' leader appears at the ladder to MUS_061: "Looks like we've
  // been found out!" Everyone turns.
  ObjectEvent_SetPosition(LOCALID_THIEF_LEADER, PIXELS(744), PIXELS(408));
  Engine_AudioPlayCue(MUS_061);
  VaultInteriors_ShowMessageAndWait(LOCALID_THIEF_LEADER, 20);
  Engine_ObjectEventStartRepeatedMotion(LOCALID_PARTY_LEADER, 1);
  Engine_ObjectEventStartRepeatedMotion(GARET, 1);
  Engine_ObjectEventStartRepeatedMotion(IVAN, 1);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_TIED_UP_VILLAGER, 1);
  Engine_ScriptWait(30);
  Engine_ObjectEventFaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_THIEF_LEADER,
                                    0);
  Engine_ObjectEventFaceObjectEvent(GARET, LOCALID_THIEF_LEADER, 0);
  Engine_ObjectEventFaceObjectEvent(IVAN, LOCALID_THIEF_LEADER, 0);
  VaultInteriors_FaceObjectEventAndWait(LOCALID_TIED_UP_VILLAGER,
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
  Engine_ScriptWait(70);
  Engine_ObjectEventFaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_THIEF_LEADER,
                                    0);
  ObjectEvent_FaceObjectEvent(GARET, LOCALID_THIEF_LEADER, 0);
  Engine_ObjectEventFaceObjectEvent(IVAN, LOCALID_THIEF_LEADER, 0);
  Engine_ObjectEventFaceObjectEvent(LOCALID_TIED_UP_VILLAGER,
                                    LOCALID_THIEF_LEADER, 0);
  ObjectEvent_SetActionScript(LOCALID_FIRST_THIEF,
                              gVaultInteriors_FirstThiefEntryScript);
  Engine_ScriptWait(40);
  ObjectEvent_SetActionScript(LOCALID_SECOND_THIEF,
                              gVaultInteriors_SecondThiefEntryScript);
  ObjectEvent_RefreshSelectorById(LOCALID_SECOND_THIEF);
  ObjectEvent_FaceDirection(LOCALID_FIRST_THIEF, FACING_SOUTHEAST, 0);
  ObjectEvent_FaceDirection(LOCALID_SECOND_THIEF, FACING_SOUTHEAST, 0);
  Engine_ScriptWait(40);
  // The three taunt Hammet's whelp and say Hammet was caught by worse thieves
  // after fleeing to Lunpa.
  VaultInteriors_PlayAnimationAndWait(LOCALID_THIEF_LEADER,
                                      ANIMATION_SHAKE_HEAD, 20);
  VaultInteriors_ShowMessageAndWait(LOCALID_THIEF_LEADER, 20);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_FIRST_THIEF, 1);
  Engine_ScriptWait(10);
  VaultInteriors_ShowMessageAndWait(LOCALID_FIRST_THIEF, 20);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_SECOND_THIEF, 1);
  Engine_ScriptWait(10);
  VaultInteriors_ShowMessageAndWait(LOCALID_SECOND_THIEF, 40);
  ObjectEvent_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Engine_ScriptWait(60);
  VaultInteriors_PlayAnimationAndWait(LOCALID_THIEF_LEADER,
                                      ANIMATION_SHAKE_HEAD, 20);
  VaultInteriors_ShowMessageAndWait(LOCALID_THIEF_LEADER, 20);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION,
                        0);
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Engine_ScriptWait(60);
  // The tied-up villager is alarmed at the name Lunpa; Garet asks what kind of
  // place it is, and the tied-up villager and the thieves tell of Lunpa, Donpa
  // and Dodonpa.
  ObjectEvent_ShowEmote(LOCALID_TIED_UP_VILLAGER,
                        EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Engine_ScriptWait(40);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_TIED_UP_VILLAGER, 2);
  Engine_ScriptWait(20);
  VaultInteriors_ShowMessageAndWait(LOCALID_TIED_UP_VILLAGER, 20);
  ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_TIED_UP_VILLAGER,
                              0);
  Engine_ObjectEventFaceObjectEvent(GARET, LOCALID_TIED_UP_VILLAGER, 0);
  VaultInteriors_FaceObjectEventAndWait(IVAN, LOCALID_TIED_UP_VILLAGER, 20);
  VaultInteriors_PlayAnimationAndWait(LOCALID_TIED_UP_VILLAGER,
                                      ANIMATION_SHAKE_HEAD, 30);
  VaultInteriors_ShowMessageAndWait(LOCALID_TIED_UP_VILLAGER, 20);
  Engine_ObjectEventRunRepeatedMotion(IVAN, 2);
  Engine_ScriptWait(20);
  Engine_ObjectEventFaceObjectEvent(LOCALID_PARTY_LEADER, IVAN, 0);
  VaultInteriors_FaceObjectEventAndWait(GARET, IVAN, 30);
  VaultInteriors_ShowMessageAndWait(GARET, 20);
  ObjectEvent_ShowEmote(LOCALID_TIED_UP_VILLAGER, EMOTE_IN_FRONT | EMOTE_WINCE,
                        0);
  Engine_ScriptWait(60);
  VaultInteriors_ShowMessageAndWait(LOCALID_TIED_UP_VILLAGER, 20);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_THIEF_LEADER, 1);
  Engine_ScriptWait(10);
  VaultInteriors_ShowMessageAndWait(LOCALID_THIEF_LEADER, 20);
  Engine_ObjectEventFaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_THIEF_LEADER,
                                    0);
  ObjectEvent_FaceObjectEvent(GARET, LOCALID_THIEF_LEADER, 0);
  VaultInteriors_FaceObjectEventAndWait(IVAN, LOCALID_THIEF_LEADER, 20);
  VaultInteriors_PlayAnimationAndWait(LOCALID_FIRST_THIEF, ANIMATION_NOD, 20);
  VaultInteriors_ShowMessageAndWait(LOCALID_FIRST_THIEF, 20);
  VaultInteriors_PlayAnimationAndWait(LOCALID_SECOND_THIEF,
                                      ANIMATION_SHAKE_HEAD, 20);
  VaultInteriors_ShowMessageAndWait(LOCALID_SECOND_THIEF, 20);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  ObjectEvent_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Engine_ScriptWait(60);
  // "See, we're not so bad. How about it? Will you let us go?" Yes: if they
  // return the goods and repent? No: even returning everything is a crime?
  VaultInteriors_PlayAnimationAndWait(LOCALID_THIEF_LEADER, ANIMATION_NOD, 20);
  Engine_ScriptOpenMessage(LOCALID_THIEF_LEADER, 0);
  Engine_ScriptWait(50);
  Engine_ObjectEventFaceObjectEvent(IVAN, LOCALID_PARTY_LEADER, 0);
  VaultInteriors_FaceObjectEventAndWait(GARET, LOCALID_PARTY_LEADER, 30);
  if (Engine_ScriptChooseYesNo(0, 0) == 0) {
    Engine_ScriptWait(40);
    Engine_ObjectEventFaceObjectEvent(GARET, LOCALID_THIEF_LEADER, 0);
    ObjectEvent_FaceObjectEvent(IVAN, LOCALID_THIEF_LEADER, 0);
    Engine_ObjectEventRunRepeatedMotion(LOCALID_THIEF_LEADER, 2);
    Engine_ScriptWait(20);
    Engine_ScriptShowMessage(LOCALID_THIEF_LEADER, 0);
  } else {
    Engine_ScriptWait(40);
    Engine_ObjectEventFaceObjectEvent(GARET, LOCALID_THIEF_LEADER, 0);
    Engine_ObjectEventFaceObjectEvent(IVAN, LOCALID_THIEF_LEADER, 0);
    base = MsgVaultVaultThievesCaught;
    Engine_ScriptSetMessage(base);
    Engine_ObjectEventRunRepeatedMotion(LOCALID_THIEF_LEADER, 2);
    Engine_ScriptWait(20);
    Engine_ScriptShowMessage(LOCALID_THIEF_LEADER, 0);
    // Both answers go on with the same refusal.
    base -= 3;
    Engine_ScriptSetMessage(base);
  }
  // "We stole this stuff fair and square!" They agree to keep the party quiet,
  // and the leader cries "Get 'em!!!"
  ObjectEvent_ShowEmote(LOCALID_FIRST_THIEF, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN,
                        0);
  Engine_ScriptWait(60);
  VaultInteriors_ShowMessageAndWait(LOCALID_FIRST_THIEF, 20);
  VaultInteriors_PlayAnimationAndWait(LOCALID_SECOND_THIEF,
                                      ANIMATION_SHAKE_HEAD, 20);
  VaultInteriors_ShowMessageAndWait(LOCALID_SECOND_THIEF, 30);
  VaultInteriors_FaceEachOtherAndWait(LOCALID_THIEF_LEADER, LOCALID_FIRST_THIEF,
                                      30);
  Engine_ObjectEventSetAnimation(LOCALID_THIEF_LEADER, ANIMATION_NOD);
  VaultInteriors_PlayAnimationAndWait(LOCALID_FIRST_THIEF, ANIMATION_NOD, 30);
  VaultInteriors_FaceEachOtherAndWait(LOCALID_THIEF_LEADER,
                                      LOCALID_SECOND_THIEF, 30);
  Engine_ObjectEventSetAnimation(LOCALID_THIEF_LEADER, ANIMATION_NOD);
  VaultInteriors_PlayAnimationAndWait(LOCALID_SECOND_THIEF, ANIMATION_NOD, 40);
  ObjectEvent_FaceDirection(LOCALID_THIEF_LEADER,
                            FACING_SOUTHEAST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_FIRST_THIEF, FACING_SOUTHEAST + FACING_STEP,
                            0);
  ObjectEvent_FaceDirection(LOCALID_SECOND_THIEF,
                            FACING_SOUTHEAST + FACING_STEP, 0);
  Engine_ScriptWait(20);
  VaultInteriors_PlayAnimationAndWait(LOCALID_THIEF_LEADER,
                                      ANIMATION_SHAKE_HEAD, 20);
  Engine_ScriptShowMessage(LOCALID_THIEF_LEADER, 0);
  // Preserve the return destinations before requesting the thieves' encounter.
  GameFlag_Set(FLAG_VAULT_THIEVES_ATTACKED);
  gFieldWork->transition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
  base = (s32)&MapId_VaultInteriors;
  FieldScript_SetNextDestination(base, 17);
  FieldScript_SetDefeatDestination(base, 16);
  Battle_SetStart(BATTLE_START_SCRIPTED);
  Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED_VALE_VAULT, 5);
  Engine_ScriptEnd();
}

/// At Vault's inn, the party leaves the guests' room for the floor below
/// (VAULT_INTERIORS_INN_FROM_INN_UPSTAIRS).
///
/// Reached by stepping on VAULT_INTERIORS_CELL_GUESTS_ROOM_STAIRS before the
/// thieves are jailed (after, the same cell is a plain warp); takes
/// VAULT_INTERIORS_INN_UPSTAIRS_TO_INN.
static void EventScript_LeaveGuestsRoom(void) {
  Engine_AudioPlayCue(SE_MAP_EXIT);
  Engine_ScriptRequestWarp(VAULT_INTERIORS_INN_UPSTAIRS_TO_INN);
}

/// In Vault's room with the crate, the party climbs up into the inn's loft by
/// the way behind the crate.
///
/// Reached by stepping on VAULT_INTERIORS_CELL_LOFT_CLIMB; takes
/// VAULT_INTERIORS_CRATE_ROOM_TO_LOFT.
static void EventScript_ClimbToLoft(void) {
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(1) / 2, PIXELS(1) / 4);
  ObjectEvent_WalkTo(LOCALID_PARTY_LEADER, 728, 408);
  gFieldWork->transitionFrames = 16;
  Script_PlaySong(SE_MAP_EXIT);
  Engine_ScriptRequestWarp(VAULT_INTERIORS_CRATE_ROOM_TO_LOFT);
}

/// Leaving the inn's loft, the party hears the tied-up villager groan.
/// It hears him when it leaves before the thieves are confronted.
/// "Uhnn... Uhnn..."
///
/// Reached by stepping on VAULT_INTERIORS_CELL_LOFT_DESCENT; takes
/// VAULT_INTERIORS_LOFT_TO_CRATE_ROOM back down.
static void EventScript_LeaveLoft(void) {
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(1) / 2, PIXELS(1) / 4);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 744, 408);
  if (GameFlag_IsSet(FLAG_VAULT_THIEVES_ATTACKED) == 0) {
    Engine_ScriptStart();
    Engine_ScriptSetMessage(MsgVaultUhnnUhnn);
    Script_ShowMessage(LOCALID_TIED_UP_VILLAGER, 0);
    Engine_ScriptEnd();
  }
  gFieldWork->transitionFrames = 16;
  Engine_AudioPlayCue(SE_MAP_EXIT);
  Engine_ScriptRequestWarp(VAULT_INTERIORS_LOFT_TO_CRATE_ROOM);
}

/// In the guests' room at Vault's inn, the two suspects spot "that weird kid"
/// with the party and run from him. "What do you want?"
///
/// Reached by stepping on VAULT_INTERIORS_CELL_GUESTS_ROOM while
/// FLAG_VAULT_IVAN_JOINED_ROD_SEARCH is set. Sets
/// FLAG_VAULT_SUSPECTS_APPROACHED and FLAG_SUSPECTS_FLEEING, starts their
/// routes and the callback that keeps them facing the leader.
/// It plays once: it copies VAULT_INTERIORS_CELL_GUESTS_ROOM_DOOR cells over
/// the cells that reach it.
static void EventScript_ApproachSuspects(void) {

  struct ObjectEvent *suspectedLeader;
  struct ObjectEvent *suspectedThief;

  suspectedLeader = ObjectEvent_GetById(LOCALID_SUSPECTED_LEADER);
  suspectedThief = ObjectEvent_GetById(LOCALID_SUSPECTED_THIEF);
  Engine_ScriptStart();
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_SetSpeed(GARET, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_SetSpeed(IVAN, PIXELS(4) / 5, PIXELS(2) / 5);
  // Isaac walks in; the two start and spot Ivan with him: "It looks like that
  // weird kid is with them!"
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 232, 696);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 200, 696);
  Engine_ScriptWait(10);
  ObjectEvent_ShowEmote(LOCALID_SUSPECTED_THIEF,
                        EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  ObjectEvent_ShowEmote(LOCALID_SUSPECTED_LEADER,
                        EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Engine_ScriptWait(60);
  VaultInteriors_FaceObjectEventAndWait(LOCALID_SUSPECTED_THIEF,
                                        LOCALID_PARTY_LEADER, 10);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_SUSPECTED_LEADER, 2);
  Engine_ScriptWait(20);
  Engine_ScriptSetMessage(MsgVaultWant);
  VaultInteriors_ShowMessageAndWait(LOCALID_SUSPECTED_LEADER, 20);
  ObjectEvent_SetAttachedEffect(
      LOCALID_SUSPECTED_THIEF, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Engine_ScriptWait(60);
  VaultInteriors_ShowMessageAndWait(LOCALID_SUSPECTED_THIEF, 20);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_SUSPECTED_LEADER, 1);
  VaultInteriors_ShowMessageAndWait(LOCALID_SUSPECTED_LEADER, 30);
  // "Don't let him near me!" They run off to their routes' first spots.
  ObjectEvent_SetSpeed(LOCALID_SUSPECTED_LEADER, PIXELS(4), PIXELS(2));
  ObjectEvent_SetSpeed(LOCALID_SUSPECTED_THIEF, PIXELS(7) / 2, PIXELS(7) / 4);
  ObjectEvent_SetActionScript(LOCALID_SUSPECTED_THIEF,
                              gVaultInteriors_SuspectToSouthWestCorner);
  ObjectEvent_SetActionScript(LOCALID_SUSPECTED_LEADER,
                              gVaultInteriors_SuspectRoundToWestBedHead);
  ObjectEvent_RefreshSelectorById(LOCALID_SUSPECTED_LEADER);
  MapGrid_CopyCellAttributes(14, 45, 3, 1, 14, 44);
  GameFlag_Set(FLAG_VAULT_SUSPECTS_APPROACHED);
  GameFlag_Set(FLAG_SUSPECTS_FLEEING);
  FieldScheduler_AddCallback(VaultInteriors_FaceSuspectsTowardLeader,
                             TASK_PRIORITY_MAP);
  suspectedLeader->timer = 1;
  suspectedThief->timer = 3;
  Engine_ScriptEnd();
}

/// Stepping out of the guests' room at Vault's inn, the party talks it over:
/// Ivan can't read their minds while they run; Garet tells Isaac to take the
/// lead and corner them.
///
/// Reached by stepping on VAULT_INTERIORS_CELL_GUESTS_ROOM_DOOR while
/// FLAG_SUSPECTS_FLEEING is set. Clears it and stops the facing callback; the
/// suspects go back to their places.
static void EventScript_DiscussSuspiciousGuests(void) {
  struct ObjectEvent *suspectedLeader;
  struct ObjectEvent *suspectedThief;
  s32 base;

  suspectedLeader = ObjectEvent_GetById(LOCALID_SUSPECTED_LEADER);
  suspectedThief = ObjectEvent_GetById(LOCALID_SUSPECTED_THIEF);
  Engine_ScriptStart();
  Task_Remove(VaultInteriors_FaceSuspectsTowardLeader);
  GameFlag_Clear(FLAG_SUSPECTS_FLEEING);
  if (suspectedLeader->timer <= 3) {
    ObjectEvent_SetActionScript(LOCALID_SUSPECTED_LEADER,
                                gVaultInteriors_SuspectRoundToWestBedFoot);
  } else {
    ObjectEvent_SetActionScript(LOCALID_SUSPECTED_LEADER,
                                gVaultInteriors_SuspectToWestBedFoot);
  }
  if (suspectedThief->timer <= 2) {
    ObjectEvent_SetActionScript(LOCALID_SUSPECTED_THIEF,
                                gVaultInteriors_SuspectAcrossToWestOfBeds);
  } else {
    ObjectEvent_SetActionScript(LOCALID_SUSPECTED_THIEF,
                                gVaultInteriors_SuspectToWestBedFoot);
  }
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_SetSpeed(GARET, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_SetSpeed(IVAN, PIXELS(4) / 5, PIXELS(2) / 5);
  // Outside the room, Garet and Ivan step out of the leader.
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 248, 728);
  ObjectEvent_SetPosition(IVAN, PIXELS(248), PIXELS(728));
  ObjectEvent_SetPosition(GARET, PIXELS(248), PIXELS(728));
  ObjectEvent_WalkTo(IVAN, 264, 744);
  ObjectEvent_WalkToAndWait(GARET, 232, 744);
  Engine_ObjectEventWaitForMove(IVAN);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTH, 0);
  ObjectEvent_FaceObjectEvent(GARET, LOCALID_PARTY_LEADER, 0);
  VaultInteriors_FaceObjectEventAndWait(IVAN, LOCALID_PARTY_LEADER, 30);
  // Ivan: they're acting suspicious.
  Engine_ObjectEventRunRepeatedMotion(IVAN, 1);
  base = MsgVaultTheyreActingSuspiciousSomethingsNot;
  Engine_ScriptSetMessage(base);
  Script_ShowMessage(IVAN, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, IVAN, 0);
  VaultInteriors_FaceObjectEventAndWait(GARET, IVAN, 20);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_NOD);
  VaultInteriors_PlayAnimationAndWait(GARET, ANIMATION_NOD, 20);
  VaultInteriors_FaceEachOtherAndWait(LOCALID_PARTY_LEADER, GARET, 10);
  // Garet: "But you can't use Mind Read if they keep running away, can you?"
  // Yes: Ivan asks how he can catch them; no: there must be a way.
  Script_OpenMessage(GARET, 0);
  if (Engine_ScriptChooseYesNo(0, 0) != 0) {
    gFieldWork->message++;
  }
  VaultInteriors_ShowMessageAndWait(GARET, 30);
  // Garet's plan: the three of them surround one of the two.
  Engine_ScriptSetMessage((base + 4));
  ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, IVAN, 0);
  VaultInteriors_FaceObjectEventAndWait(GARET, IVAN, 50);
  ObjectEvent_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Engine_ScriptWait(60);
  VaultInteriors_FaceEachOtherAndWait(LOCALID_PARTY_LEADER, GARET, 50);
  ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, IVAN, 0);
  VaultInteriors_FaceObjectEventAndWait(GARET, IVAN, 30);
  VaultInteriors_PlayAnimationAndWait(IVAN, ANIMATION_NOD, 10);
  VaultInteriors_ShowMessageAndWait(IVAN, 20);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Engine_ScriptWait(60);
  VaultInteriors_PlayAnimationAndWait(IVAN, ANIMATION_NOD, 20);
  VaultInteriors_ShowMessageAndWait(IVAN, 30);
  // "Isaac, you take the lead." The two walk back into him, and the suspects
  // are put back at their table.
  Engine_ScriptSetMessage(MsgVaultIsaacTakeLead);
  Script_ShowMessage(GARET, 0);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTH, 0);
  Engine_ScriptWait(40);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_NOD);
  Engine_ObjectEventSetAnimation(GARET, ANIMATION_NOD);
  VaultInteriors_PlayAnimationAndWait(IVAN, ANIMATION_NOD, 50);
  ObjectEvent_WalkTo(IVAN, 248, 728);
  ObjectEvent_WalkToAndWait(GARET, 248, 728);
  ObjectEvent_SetPosition(GARET, 0, 0);
  ObjectEvent_SetPosition(IVAN, 0, 0);
  ObjectEvent_SetPosition(LOCALID_SUSPECTED_LEADER, PIXELS(104), PIXELS(696));
  ObjectEvent_SetPosition(LOCALID_SUSPECTED_THIEF, PIXELS(120), PIXELS(696));
  ObjectEvent_FaceDirection(LOCALID_SUSPECTED_LEADER, FACING_EAST, 0);
  ObjectEvent_FaceDirection(LOCALID_SUSPECTED_THIEF, FACING_WEST, 0);
  MapGrid_CopyCellAttributes(14, 50, 3, 1, 14, 44);
  Engine_ScriptEnd();
}

/// In the guests' room at Vault's inn, the party sets out to corner the two
/// suspects.
///
/// Reached by stepping on VAULT_INTERIORS_CELL_CORNERING_START while
/// FLAG_VAULT_SUSPECTS_APPROACHED is set. Sets FLAG_MENUS_RAISE_EVENT, so the
/// menus answer with Garet's refusal, and FLAG_MAP_TEMP_50, so talking to
/// Ivan or the suspects does nothing; then starts the chase
/// (VaultInteriors_StartObjectEventStops).
static void EventScript_StartCorneringSuspects(void) {
  Engine_GameFlagSet(FLAG_MENUS_RAISE_EVENT);
  Engine_GameFlagSet(FLAG_MAP_TEMP_50);
  VaultInteriors_StartObjectEventStops();
}

/// Has the suspects react to the party's return and resets their route states.
/// "Hey, they're back!"
///
/// Called by VaultInteriors_StartObjectEventStops.
static void VaultInteriors_ResetSuspectRoutes(void) {
  Engine_ScriptWait(30);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_SUSPECTED_LEADER, 1);
  Engine_ScriptWait(20);
  Engine_ScriptSetMessage(MsgVaultTheyreBack);
  VaultInteriors_ShowMessageAndWait(LOCALID_SUSPECTED_LEADER, 20);
  ObjectEvent_FaceDirection(LOCALID_SUSPECTED_THIEF, FACING_EAST, 20);
  ObjectEvent_SetAttachedEffect(
      LOCALID_SUSPECTED_THIEF, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_SUSPECTED_THIEF, 2);
  VaultInteriors_ShowMessageAndWait(LOCALID_SUSPECTED_THIEF, 20);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_SUSPECTED_LEADER,
                                        ANIMATION_SHAKE_HEAD);
  Engine_ScriptWait(20);
  VaultInteriors_ShowMessageAndWait(LOCALID_SUSPECTED_LEADER, 20);
  ObjectEvent_SetSpeed(LOCALID_SUSPECTED_LEADER, PIXELS(4), PIXELS(2));
  ObjectEvent_SetSpeed(LOCALID_SUSPECTED_THIEF, PIXELS(7) / 2, PIXELS(7) / 4);
  ObjectEvent_SetActionScript(LOCALID_SUSPECTED_THIEF,
                              gVaultInteriors_SuspectToSouthWestCorner);
  ObjectEvent_SetActionScript(LOCALID_SUSPECTED_LEADER,
                              gVaultInteriors_SuspectRoundToWestBedHead);
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
/// scene.
static inline void VaultInteriors_SetPartyImagePriority(void) {
  {
    struct AnimationObject *sprite =
        ObjectEvent_GetById(LOCALID_PARTY_LEADER)->sprite;
    sprite->image.priority = 1;
  }
  {
    struct AnimationObject *sprite = ObjectEvent_GetById(GARET)->sprite;
    sprite->image.priority = 1;
  }
  {
    struct AnimationObject *sprite = ObjectEvent_GetById(IVAN)->sprite;
    sprite->image.priority = 1;
  }
}

/// Once a suspect is cornered, Garet calls Ivan and Ivan reads their minds.
/// This is in the guests' room at Vault's inn. "Uh-oh! I'm surrounded!" "Now,
/// Ivan!"
///
/// Reached through raised events 201 to 203, which
/// VaultInteriors_UpdateObjectEventStops raises after thirty frames with at
/// least one suspect blocked: 201 the leader, 202 the thief, 203 both. Stops
/// the chase, clears FLAG_MENUS_RAISE_EVENT and FLAG_MAP_TEMP_50, plays the
/// party's talk (VaultInteriors_DiscussMindReading) and sets
/// FLAG_VAULT_SUSPECTS_MIND_READ.
static void EventScript_ReadCorneredSuspectsMinds(void) {
  s32 localId;
  struct EventWork *work;

  work = gFieldWork;
  Engine_ScriptStart();
  Task_Remove(VaultInteriors_UpdateObjectEventStops);
  GameFlag_Clear(FLAG_MENUS_RAISE_EVENT);
  GameFlag_Clear(FLAG_MAP_TEMP_50);
  Engine_ObjectEventSetAnimation(LOCALID_SUSPECTED_LEADER, ANIMATION_STAND);
  Engine_ObjectEventSetAnimation(LOCALID_SUSPECTED_THIEF, ANIMATION_STAND);
  Engine_ObjectEventSetAnimation(IVAN, ANIMATION_STAND);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_STAND);
  Engine_ObjectEventSetAnimation(GARET, ANIMATION_STAND);
  ObjectEvent_FaceObjectEvent(GARET, IVAN, 0);
  Engine_ObjectEventFaceEachOther(LOCALID_PARTY_LEADER, IVAN, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_SUSPECTED_LEADER, IVAN, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_SUSPECTED_THIEF, IVAN, 0);
  Engine_ScriptWait(10);
  localId = LOCALID_SUSPECTED_LEADER;
  // The event identifies the suspects blocked on the last frame of the wait.
  switch (work->raisedEvent) {
  case VAULT_INTERIORS_THIEF_CORNERED:
  case VAULT_INTERIORS_BOTH_CORNERED:
    Engine_ScriptSetMessage(MsgVaultImSurrounded);
    ObjectEvent_SetAttachedEffect(LOCALID_SUSPECTED_THIEF,
                                  ATTACHED_EFFECT_IN_FRONT |
                                      ATTACHED_EFFECT_ONCE);
    Engine_ObjectEventRunRepeatedMotion(LOCALID_SUSPECTED_THIEF, 2);
    VaultInteriors_ShowMessageAndWait(LOCALID_SUSPECTED_THIEF, 20);
    if (work->raisedEvent == VAULT_INTERIORS_THIEF_CORNERED) {
      localId = LOCALID_SUSPECTED_THIEF;
      break;
    }
    // When both are cornered, the leader's reaction follows the thief's.
    // fall through
  case VAULT_INTERIORS_LEADER_CORNERED:
    Engine_ScriptSetMessage(MsgVaultTheresNowhereRun);
    ObjectEvent_SetAttachedEffect(LOCALID_SUSPECTED_LEADER,
                                  ATTACHED_EFFECT_IN_FRONT |
                                      ATTACHED_EFFECT_ONCE);
    Engine_ObjectEventRunRepeatedMotion(LOCALID_SUSPECTED_LEADER, 2);
    localId = LOCALID_SUSPECTED_LEADER;
    VaultInteriors_ShowMessageAndWait(LOCALID_SUSPECTED_LEADER, 20);
    break;
  }
  Engine_ObjectEventSetAnimation(IVAN, ANIMATION_STAND);
  ObjectEvent_FaceObjectEvent(IVAN, localId, 0);
  ObjectEvent_FaceObjectEvent(GARET, IVAN, 0);
  Engine_ObjectEventRunRepeatedMotion(GARET, 2);
  Engine_ScriptWait(20);
  // "Now, Ivan!" "A-All right!" Ivan reads the cornered one's mind while the
  // two wonder what he is doing.
  Engine_ScriptSetMessage(MsgVaultNowIvan);
  VaultInteriors_ShowMessageAndWait(GARET, 20);
  Engine_ObjectEventRunRepeatedMotion(IVAN, 2);
  Engine_ScriptWait(20);
  VaultInteriors_ShowMessageAndWait(IVAN, 20);
  VaultInteriors_StartMindRead(IVAN, localId);
  Ui_SetBank15PaletteAndClearRenderMode();
  Engine_ScriptWait(60);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_SUSPECTED_LEADER, 2);
  VaultInteriors_ShowMessageAndWait(LOCALID_SUSPECTED_LEADER, 20);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_SUSPECTED_THIEF, 2);
  VaultInteriors_ShowMessageAndWait(LOCALID_SUSPECTED_THIEF, 20);
  // "Are you done, Ivan?" He nods, and the window closes.
  ObjectEvent_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  VaultInteriors_EndMindRead();
  Engine_ObjectEventStartRepeatedMotion(GARET, 2);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_PARTY_LEADER, 2);
  Engine_ScriptWait(20);
  VaultInteriors_ShowMessageAndWait(GARET, 20);
  ObjectEvent_FaceObjectEvent(IVAN, GARET, 0);
  Engine_ScriptWait(20);
  Engine_ObjectEventSetAnimationAndWait(IVAN, ANIMATION_NOD);
  Engine_ScriptWait(40);
  VaultInteriors_SetPartyImagePriority();
  gFieldWork->transitionFrames = 24;
  gFieldWork->transition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
  Engine_ScriptCloseScreen();
  Engine_ScriptWaitForScreen();
  // Move the discussion away from the suspects while the screen is closed.
  VaultInteriors_DiscussMindReading();
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
  FieldScheduler_AddCallback(VaultInteriors_FaceSuspectsTowardLeader,
                             TASK_PRIORITY_MAP);
  gFieldWork->transition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  Engine_ScriptEnd();
}

/// Shows the party's branching conversation about what Ivan learned: the
/// stolen goods are hidden in the inn, which they must search.
///
/// Called by EventScript_ReadCorneredSuspectsMinds while the screen is closed;
/// sets the party outside the room and opens the screen on them.
static void VaultInteriors_DiscussMindReading(void) {
  s32 persistedWithQuestions;

  // Move the party outside the room and the suspects back to their table.
  Map_PlayMusic();
  Engine_ObjectEventSetSpritePriority(LOCALID_PARTY_LEADER, 1);
  {
    struct ObjectEvent *leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);

    leader->priorityFlags =
        OBJ_EVENT_PRIORITY_AUTOMATIC | leader->priorityFlags;
  }
  Field_SetViewBounds(PIXELS(32), PIXELS(576), PIXELS(400), PIXELS(936));
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_SetSpeed(GARET, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_SetSpeed(IVAN, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_SetPosition(LOCALID_PARTY_LEADER, PIXELS(248), PIXELS(728));
  ObjectEvent_SetPosition(IVAN, PIXELS(264), PIXELS(744));
  Engine_ObjectEventSetPosition(GARET, PIXELS(232), PIXELS(744));
  VaultInteriors_SetPartyImagePriority();
  Engine_ObjectEventFaceEachOther(LOCALID_PARTY_LEADER, IVAN, 0);
  VaultInteriors_FaceObjectEventAndWait(GARET, IVAN, 30);
  ObjectEvent_SetPosition(LOCALID_SUSPECTED_LEADER, PIXELS(104), PIXELS(696));
  ObjectEvent_SetPosition(LOCALID_SUSPECTED_THIEF, PIXELS(120), PIXELS(696));
  Engine_ObjectEventFaceEachOther(LOCALID_SUSPECTED_LEADER,
                                  LOCALID_SUSPECTED_THIEF, 0);
  Engine_CameraFollowObjectEvent(LOCALID_PARTY_LEADER, 0);
  Engine_CameraWaitForMove();
  Engine_MapGridRedraw();
  Engine_ScriptWait(30);
  {
    struct EventWork *work = gFieldWork;

    work->transitionFrames = 24;
    work->transition =
        MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
  }
  Engine_ScriptOpenScreen();
  Engine_ScriptWaitForScreen();
  Engine_ScriptWait(40);
  // "What did you learn, Ivan?" It is them: they hid the stolen goods
  // somewhere in this inn, but won't let him near again.
  Engine_ObjectEventRunRepeatedMotion(GARET, 1);
  Engine_ScriptWait(10);
  Engine_ScriptSetMessage(MsgVaultLearn);
  VaultInteriors_ShowMessageAndWait(GARET, 20);
  VaultInteriors_PlayAnimationAndWait(IVAN, ANIMATION_NOD, 20);
  VaultInteriors_ShowMessageAndWait(IVAN, 20);
  VaultInteriors_FaceEachOtherAndWait(LOCALID_PARTY_LEADER, GARET, 50);
  Engine_ObjectEventFaceObjectEvent(LOCALID_PARTY_LEADER, IVAN, 0);
  VaultInteriors_FaceObjectEventAndWait(GARET, IVAN, 20);
  VaultInteriors_PlayAnimationAndWait(IVAN, ANIMATION_SHAKE_HEAD, 20);
  Engine_ScriptShowMessage(IVAN, 0);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Engine_ScriptWait(60);
  VaultInteriors_PlayAnimationAndWait(IVAN, ANIMATION_NOD, 20);
  VaultInteriors_ShowMessageAndWait(IVAN, 30);
  Engine_ObjectEventRunRepeatedMotion(GARET, 1);
  Engine_ScriptWait(20);
  VaultInteriors_FaceEachOtherAndWait(LOCALID_PARTY_LEADER, GARET, 20);
  // Garet: they have to search the inn. Each no leads to another plea, from
  // Garet and then Ivan; three noes and Garet vouches for Isaac's sense of
  // duty, while any yes gets "Leave it to us!"
  Engine_ScriptOpenMessage(GARET, 0);
  persistedWithQuestions = 0;
  if (Engine_ScriptChooseYesNo(0, 0) != 0) {
    Engine_ScriptWait(20);
    Engine_ObjectEventStartRepeatedMotion(IVAN, 2);
    VaultInteriors_PlayAnimationAndWait(GARET, ANIMATION_SHAKE_HEAD, 30);
    Engine_ScriptOpenMessage(GARET, 0);
    persistedWithQuestions = 0;
    if (Engine_ScriptChooseYesNo(0, 0) != 0) {
      Engine_ScriptWait(20);
      Engine_ObjectEventShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
      Engine_ScriptWait(60);
      Engine_ObjectEventRunRepeatedMotion(IVAN, 2);
      VaultInteriors_FaceEachOtherAndWait(LOCALID_PARTY_LEADER, IVAN, 20);
      Engine_ScriptOpenMessage(IVAN, 0);
      persistedWithQuestions = 0;
      if (Engine_ScriptChooseYesNo(2, 0) != 0) {
        Engine_ScriptWait(20);
        Engine_ObjectEventShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
        Engine_ScriptWait(60);
        VaultInteriors_ShowMessageAndWait(IVAN, 20);
        VaultInteriors_FaceObjectEventAndWait(GARET, IVAN, 10);
        Engine_ObjectEventRunRepeatedMotion(GARET, 1);
        Engine_ScriptWait(10);
        VaultInteriors_ShowMessageAndWait(GARET, 10);
        VaultInteriors_FaceObjectEventAndWait(IVAN, GARET, 20);
        Engine_ObjectEventShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
        Engine_ScriptWait(60);
        VaultInteriors_PlayAnimationAndWait(IVAN, ANIMATION_SHAKE_HEAD, 20);
        VaultInteriors_ShowMessageAndWait(GARET, 10);
        VaultInteriors_FaceEachOtherAndWait(GARET, LOCALID_PARTY_LEADER, 20);
        VaultInteriors_PlayAnimationAndWait(GARET, ANIMATION_NOD, 20);
        persistedWithQuestions = 1;
        VaultInteriors_ShowMessageAndWait(GARET, 20);
      }
    }
  }
  if (persistedWithQuestions == 0) {
    Engine_ScriptSetMessage(MsgVaultLeave);
    VaultInteriors_ShowMessageAndWait(GARET, 20);
    VaultInteriors_FaceEachOtherAndWait(GARET, LOCALID_PARTY_LEADER, 20);
    VaultInteriors_ShowMessageAndWait(GARET, 20);
  }
  // "Let's go, Ivan." Garet and Ivan walk back into the leader.
  Engine_ObjectEventShowEmote(LOCALID_PARTY_LEADER,
                              EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Engine_ScriptWait(60);
  Engine_ObjectEventRunRepeatedMotion(GARET, 1);
  VaultInteriors_ShowMessageAndWait(GARET, 10);
  VaultInteriors_PlayAnimationAndWait(GARET, ANIMATION_NOD, 10);
  Engine_ObjectEventFaceEachOther(GARET, IVAN, 0);
  VaultInteriors_FaceEachOtherAndWait(LOCALID_PARTY_LEADER, IVAN, 10);
  Engine_ScriptShowMessage(GARET, 0);
  VaultInteriors_PlayAnimationAndWait(IVAN, ANIMATION_NOD, 10);
  ObjectEvent_WalkTo(IVAN, 248, 728);
  ObjectEvent_WalkToAndWait(GARET, 248, 728);
  Engine_ObjectEventSetPosition(GARET, 0, 0);
  Engine_ObjectEventSetPosition(IVAN, 0, 0);
}

/// The fleeing suspects move on to their next spots.
/// This is in the guests' room at Vault's inn, when the leader steps on one of
/// the route cells, VAULT_INTERIORS_CELL_FIRST_SUSPECT_ROUTE to
/// _LAST_SUSPECT_ROUTE.
///
/// Reached by stepping on one of them while FLAG_SUSPECTS_FLEEING is
/// set.
static void EventScript_AdvanceSuspectRoutes(void) {
  switch (gFieldWork->coordEvent) {
  case VAULT_INTERIORS_CELL_FIRST_SUSPECT_ROUTE:
    VaultInteriors_AdvanceSuspectRoute(LOCALID_SUSPECTED_LEADER, 1, 2,
                                       gVaultInteriors_SuspectToMiddleBedHead);
    VaultInteriors_AdvanceSuspectRoute(
        LOCALID_SUSPECTED_THIEF, 3, 4,
        gVaultInteriors_SuspectAlongToSouthEastCorner);
    break;
  case VAULT_INTERIORS_CELL_FIRST_SUSPECT_ROUTE + 1:
    VaultInteriors_AdvanceSuspectRoute(
        LOCALID_SUSPECTED_LEADER, 1, 4,
        gVaultInteriors_SuspectRoundToWestBedFoot);
    VaultInteriors_AdvanceSuspectRoute(LOCALID_SUSPECTED_LEADER, 2, 3,
                                       gVaultInteriors_SuspectToEastBedHead);
    VaultInteriors_AdvanceSuspectRoute(
        LOCALID_SUSPECTED_THIEF, 1, 3,
        gVaultInteriors_SuspectToSouthWestCorner);
    break;
  case VAULT_INTERIORS_CELL_FIRST_SUSPECT_ROUTE + 2:
    VaultInteriors_AdvanceSuspectRoute(LOCALID_SUSPECTED_LEADER, 2, 1,
                                       gVaultInteriors_SuspectToWestBedHead);
    VaultInteriors_AdvanceSuspectRoute(
        LOCALID_SUSPECTED_LEADER, 3, 6,
        gVaultInteriors_SuspectRoundToEastBedFoot);
    VaultInteriors_AdvanceSuspectRoute(
        LOCALID_SUSPECTED_THIEF, 2, 4,
        gVaultInteriors_SuspectToSouthEastCorner);
    break;
  case VAULT_INTERIORS_CELL_FIRST_SUSPECT_ROUTE + 3:
    VaultInteriors_AdvanceSuspectRoute(LOCALID_SUSPECTED_LEADER, 3, 2,
                                       gVaultInteriors_SuspectToMiddleBedHead);
    VaultInteriors_AdvanceSuspectRoute(
        LOCALID_SUSPECTED_THIEF, 4, 3,
        gVaultInteriors_SuspectAlongToSouthWestCorner);
    break;
  case VAULT_INTERIORS_CELL_FIRST_SUSPECT_ROUTE + 4:
    VaultInteriors_AdvanceSuspectRoute(LOCALID_SUSPECTED_LEADER, 4, 5,
                                       gVaultInteriors_SuspectToMiddleBedFoot);
    VaultInteriors_AdvanceSuspectRoute(
        LOCALID_SUSPECTED_THIEF, 1, 2,
        gVaultInteriors_SuspectAcrossToEastOfBeds);
    break;
  case VAULT_INTERIORS_CELL_FIRST_SUSPECT_ROUTE + 5:
    VaultInteriors_AdvanceSuspectRoute(
        LOCALID_SUSPECTED_LEADER, 4, 1,
        gVaultInteriors_SuspectRoundToWestBedHead);
    VaultInteriors_AdvanceSuspectRoute(LOCALID_SUSPECTED_LEADER, 5, 6,
                                       gVaultInteriors_SuspectToEastBedFoot);
    VaultInteriors_AdvanceSuspectRoute(LOCALID_SUSPECTED_THIEF, 3, 1,
                                       gVaultInteriors_SuspectToWestOfBeds);
    break;
  case VAULT_INTERIORS_CELL_FIRST_SUSPECT_ROUTE + 6:
    VaultInteriors_AdvanceSuspectRoute(LOCALID_SUSPECTED_LEADER, 5, 4,
                                       gVaultInteriors_SuspectToWestBedFoot);
    VaultInteriors_AdvanceSuspectRoute(
        LOCALID_SUSPECTED_LEADER, 6, 3,
        gVaultInteriors_SuspectRoundToEastBedHead);
    VaultInteriors_AdvanceSuspectRoute(LOCALID_SUSPECTED_THIEF, 4, 2,
                                       gVaultInteriors_SuspectToEastOfBeds);
    break;
  case VAULT_INTERIORS_CELL_LAST_SUSPECT_ROUTE:
    VaultInteriors_AdvanceSuspectRoute(LOCALID_SUSPECTED_LEADER, 6, 5,
                                       gVaultInteriors_SuspectToMiddleBedFoot);
    VaultInteriors_AdvanceSuspectRoute(
        LOCALID_SUSPECTED_THIEF, 2, 1,
        gVaultInteriors_SuspectAcrossToWestOfBeds);
    break;
  }
}

/// Installs a route script only when the suspect is in its expected state.
static void VaultInteriors_AdvanceSuspectRoute(s32 localId, s32 expected,
                                               s32 next, const s32 *actions) {
  struct ObjectEvent *suspect = ObjectEvent_GetById(localId);

  if (suspect->timer == expected) {
    ObjectEvent_SetActionScript(localId, actions);
    suspect->timer = next;
  }
}

#include "common/vault_spotflag.h"

/// Arriving in any room of Vault's houses, shops and inn: restores the room
/// for the story so far, and plays the loft's scenes: the tied-up villager, the
/// rematch, and the defeat or victory.
///
/// Called by the engine once the object events are placed, by warp destination.
/// At the inn, after the thieves are jailed, places the Bone and starts
/// Vault_UpdateBoneProximityFlag. After a lost battle
/// (VAULT_INTERIORS_LOFT_AFTER_LOSING_TO_THIEVES) takes
/// VAULT_INTERIORS_TO_SANCTUM; after a won one (17) sets
/// FLAG_VAULT_THIEVES_JAILED through VaultInteriors_ReturnStolenGoods, which
/// also removes Ivan from the party.
s32 Map_Initialize(void) {
  gFieldWork->transition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  switch (gGameState.warpDest) {
  case VAULT_INTERIORS_FROM_VAULT_EAST_SOUTHEAST: {
    s32 zero = 0;

    // The gem owner's house: he stands on the floor.
    ObjectEvent_GetById(LOCALID_GEM_OWNER)->motionFlags = zero;
    ObjectEvent_GetById(LOCALID_GEM_OWNER)->y.fixed = zero;
    ObjectEvent_GetById(LOCALID_GEM_OWNER)->groundHeight = zero;
  } break;
  case VAULT_INTERIORS_FROM_VAULT_NORTHWEST:
    // The mayor's house: Ivan waits a step back once met and is gone once he
    // has joined; after the thieves are jailed, the mayor stands by the door.
    if (Engine_GameFlagIsSet(FLAG_VAULT_MET_IVAN) != 0) {
      ObjectEvent_SetPosition(IVAN, PIXELS(376), PIXELS(376));
      ObjectEvent_FaceDirection(IVAN, FACING_SOUTH, 0);
    }
    if (Engine_GameFlagIsSet(FLAG_VAULT_IVAN_JOINED_ROD_SEARCH) != 0) {
      Engine_ObjectEventSetPosition(IVAN, 0, 0);
    }
    if (Engine_GameFlagIsSet(FLAG_VAULT_THIEVES_JAILED) != 0) {
      ObjectEvent_SetPosition(LOCALID_MAYOR, PIXELS(536),
                              VAULT_INTERIORS_ENTRY_OBJ_EVENT_Z);
      ObjectEvent_SetBuiltinAction(LOCALID_MAYOR, OBJ_EVENT_ACTION_IDLE);
      ObjectEvent_FaceDirection(LOCALID_MAYOR, FACING_SOUTH + FACING_STEP, 0);
    }
    break;
  case VAULT_INTERIORS_FROM_VAULT_SOUTHWEST:
  case VAULT_INTERIORS_INN_FROM_INN_UPSTAIRS:
    // The inn: once the thieves are jailed, the Bone lies on the floor beside
    // the girl at the inn; after the mind reading and before that, she is
    // out.
    if (Engine_GameFlagIsSet(FLAG_VAULT_THIEVES_JAILED) != 0) {
      ObjectEvent_SetPosition(LOCALID_GIRL_AT_THE_INN, PIXELS(568),
                              PIXELS(648));
      ObjectEvent_SetBuiltinAction(LOCALID_GIRL_AT_THE_INN,
                                   OBJ_EVENT_ACTION_IDLE);
      ObjectEvent_FaceDirection(LOCALID_GIRL_AT_THE_INN, FACING_SOUTH, 0);
      Vault_PlaceItemIcon(ITEM_BONE, PIXELS(568), PIXELS(16), PIXELS(672));
      Task_AddCallback(Vault_UpdateBoneProximityFlag, TASK_PRIORITY_MAP);
    } else if (Engine_GameFlagIsSet(FLAG_VAULT_SUSPECTS_MIND_READ) != 0) {
      Engine_ObjectEventSetPosition(LOCALID_GIRL_AT_THE_INN, 0, 0);
    }
    break;
  case VAULT_INTERIORS_INN_UPSTAIRS_FROM_INN:
    // The guests' room upstairs at the inn: back from a battle mid-chase, the
    // suspects keep running.
    if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) != 0 &&
        Engine_GameFlagIsSet(FLAG_VAULT_SUSPECTS_APPROACHED) != 0 &&
        Engine_GameFlagIsSet(FLAG_VAULT_SUSPECTS_MIND_READ) == 0 &&
        Engine_GameFlagIsSet(FLAG_SUSPECTS_FLEEING) != 0) {
      MapGrid_CopyCellAttributes(14, 45, 3, 1, 14, 44);
      Task_AddCallback(VaultInteriors_FaceSuspectsTowardLeader,
                       TASK_PRIORITY_MAP);
      break;
    }
    if (Engine_GameFlagIsSet(FLAG_VAULT_IVAN_JOINED_ROD_SEARCH) != 0) {
      ObjectEvent_SetPosition(LOCALID_SUSPECTED_THIEF, PIXELS(120),
                              PIXELS(696));
      ObjectEvent_FaceDirection(LOCALID_SUSPECTED_THIEF, FACING_WEST, 0);
    }
    // Once approached, the room's doorway metatile behavior changes; after the
    // mind reading the suspects sit turned toward the leader.
    if (Engine_GameFlagIsSet(FLAG_VAULT_SUSPECTS_APPROACHED) != 0) {
      MapGrid_CopyCellAttributes(14, 45, 3, 1, 14, 44);
      if (Engine_GameFlagIsSet(FLAG_VAULT_SUSPECTS_MIND_READ) == 0) {
        MapGrid_CopyCellAttributes(14, 50, 3, 1, 14, 44);
        break;
      }
      ObjectEvent_GetById(LOCALID_SUSPECTED_LEADER)->timer = 5;
      ObjectEvent_GetById(LOCALID_SUSPECTED_THIEF)->timer = 4;
      FieldScheduler_AddCallback(VaultInteriors_FaceSuspectsTowardLeader,
                                 TASK_PRIORITY_MAP);
    }
    break;
  case VAULT_INTERIORS_FROM_VAULT_WEST:
  case VAULT_INTERIORS_CRATE_ROOM_FROM_LOFT:
    // The room with the crate: the crate keeps its place once moved, and the
    // passage behind it stays open.
    Engine_ScriptWait(2);
    MapGrid_CopyCellsTo(54, 2, 35, 20, 2, 10);
    MapGrid_CopyCellsTo(54, 2, 95, 20, 2, 10);
    MapGrid_CopyCellsTo(54, 2, 35, 80, 2, 10);
    MapGrid_CopyCellsTo(54, 2, 46, 21, 4, 8);
    MapGrid_CopyCellsTo(54, 2, 46, 81, 4, 8);
    ObjectEvent_GetById(LOCALID_CRATE)->collisionFlags |= 4;
    Engine_ObjectEventSetSpriteFlags(ObjectEvent_GetById(LOCALID_CRATE), 0);
    if (Engine_GameFlagIsSet(FLAG_VAULT_CRATE_MOVED) != 0) {
      ObjectEvent_SetPosition(LOCALID_CRATE, PIXELS(676), PIXELS(411));
      MapGrid_CopyCellAttributes(101, 24, 3, 4, 41, 24);
    }
    break;
  case VAULT_INTERIORS_LOFT_FROM_CRATE_ROOM:
    Engine_ScriptWait(2);
    MapGrid_CopyCellsTo(54, 2, 44, 21, 2, 8);
    MapGrid_CopyCellsTo(54, 2, 44, 81, 2, 8);
    Engine_ScriptStart();
    // The loft: emptied chests after the thieves are jailed; else the tied-up
    // villager lies tied, groaning to AUDIO_CUE_FADE_BGM, or the thieves wait
    // for a rematch.
    if (Engine_GameFlagIsSet(FLAG_VAULT_THIEVES_JAILED) != 0) {
      Engine_ObjectEventSetAnimation(LOCALID_FIRST_CHEST, ANIMATION_WALK);
      Engine_ObjectEventSetAnimation(LOCALID_SECOND_CHEST, ANIMATION_WALK);
      Engine_ScriptWait(1);
      Engine_ObjectEventSetAnimation(LOCALID_THIRD_CHEST, ANIMATION_WALK);
      break;
    }
    Engine_ObjectEventSetPosition(LOCALID_TIED_UP_VILLAGER, PIXELS(824),
                                  PIXELS(456));
    if (Engine_GameFlagIsSet(FLAG_VAULT_THIEVES_ATTACKED) != 0) {
      VaultInteriors_ConfrontThievesAgain();
      Engine_ScriptEnd();
      break;
    }
    Engine_ObjectEventSetAnimation(LOCALID_TIED_UP_VILLAGER, 7);
    VaultInteriors_OpenWindowTransition();
    Engine_AudioPlayCue(AUDIO_CUE_FADE_BGM);
    Engine_ScriptSetMessage(MsgVaultUhnnUhnn);
    VaultInteriors_ShowMessageAndWait(LOCALID_TIED_UP_VILLAGER, 10);
    Engine_ScriptEnd();
    break;
  case VAULT_INTERIORS_LOFT_AFTER_LOSING_TO_THIEVES:
    // Lost to the thieves: they gloat, and the party wakes up elsewhere.
    MapGrid_CopyCells(54, 2, 2, 8, 44, 21);
    MapGrid_CopyCells(54, 2, 2, 8, 44, 81);
    Engine_ObjectEventSetPosition(LOCALID_TIED_UP_VILLAGER, PIXELS(824),
                                  PIXELS(456));
    VaultInteriors_LoseToThieves();
    Engine_ScriptRequestWarp(VAULT_INTERIORS_TO_SANCTUM);
    break;
  case VAULT_INTERIORS_LOFT_AFTER_THIEVES_DEFEATED:
    // Won: the thieves are caught and the stolen goods handed back.
    MapGrid_CopyCells(54, 2, 2, 8, 44, 21);
    MapGrid_CopyCells(54, 2, 2, 8, 44, 81);
    if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) == 0) {
      ObjectEvent_SetPosition(LOCALID_TIED_UP_VILLAGER, PIXELS(824),
                              PIXELS(456));
      VaultInteriors_DefeatThieves();
      VaultInteriors_ReturnStolenGoods();
      break;
    }
    Engine_ObjectEventSetAnimation(LOCALID_FIRST_CHEST, ANIMATION_WALK);
    Engine_ObjectEventSetAnimation(LOCALID_SECOND_CHEST, ANIMATION_WALK);
    Engine_ScriptWait(1);
    Engine_ObjectEventSetAnimation(LOCALID_THIRD_CHEST, ANIMATION_WALK);
    break;
  }
  return 0;
}

/// Keeps both suspects facing the leader while they evade the party.
///
/// Run each frame as a map callback, from EventScript_ApproachSuspects until
/// EventScript_DiscussSuspiciousGuests, and again once their minds are read.
static void VaultInteriors_FaceSuspectsTowardLeader(void) {
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
/// Called by Map_Initialize at VAULT_INTERIORS_LOFT_AFTER_LOSING_TO_THIEVES,
/// the battle's defeat destination; the scene then takes
/// VAULT_INTERIORS_TO_SANCTUM to the Vault Sanctum (MapId_Sanctum,
/// SANCTUM_FROM_VAULT_INTERIORS).
static void VaultInteriors_LoseToThieves(void) {
  struct ObjectEvent *objectEvent;
  s32 priorityFlags;

  // The party lies on the floor (animation 19) before the three thieves.
  Engine_ScriptStart();
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
  ObjectEvent_SetPosition(GARET, PIXELS(808), PIXELS(432));
  ObjectEvent_SetPosition(IVAN, PIXELS(776), PIXELS(432));
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, 19);
  Engine_ObjectEventSetAnimation(GARET, 19);
  Engine_ObjectEventSetAnimation(IVAN, 19);
  priorityFlags = OBJ_EVENT_PRIORITY_UNDERFOOT;
  ObjectEvent_GetById(LOCALID_PARTY_LEADER)->priorityFlags = priorityFlags;
  ObjectEvent_GetById(GARET)->priorityFlags = priorityFlags;
  ObjectEvent_GetById(IVAN)->priorityFlags = priorityFlags;
  objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  Engine_ObjectEventSetSpriteFlags(objectEvent, 0);
  objectEvent = ObjectEvent_GetById(IVAN);
  Engine_ObjectEventSetSpriteFlags(objectEvent, 0);
  objectEvent = ObjectEvent_GetById(GARET);
  Engine_ObjectEventSetSpriteFlags(objectEvent, 0);
  ObjectEvent_FaceDirection(LOCALID_TIED_UP_VILLAGER,
                            FACING_NORTHWEST + FACING_STEP, 0);
  gFieldWork->transition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  Engine_CameraFollowObjectEvent(LOCALID_PARTY_LEADER, 0);
  Engine_CameraWaitForMove();
  Engine_MapGridRedraw();
  VaultInteriors_OpenWindowTransition();
  Engine_ScriptWait(60);
  // "These kids are nothing to worry about." The tied-up villager groans.
  VaultInteriors_PlayAnimationAndWait(LOCALID_THIEF_LEADER, ANIMATION_NOD, 20);
  Engine_ScriptSetMessage(MsgVaultTheseKidsNothingWorryAbout);
  VaultInteriors_ShowMessageAndWait(LOCALID_THIEF_LEADER, 30);
  VaultInteriors_ShowMessageAndWait(LOCALID_TIED_UP_VILLAGER, 30);
  // The two men step to the tied-up villager: he stays as their hostage.
  ObjectEvent_WalkTo(LOCALID_FIRST_THIEF, 808, 456);
  ObjectEvent_WalkTo(LOCALID_SECOND_THIEF, 792, 456);
  Engine_ObjectEventWaitForMove(LOCALID_SECOND_THIEF);
  ObjectEvent_FaceDirection(LOCALID_SECOND_THIEF, FACING_EAST, 0);
  Engine_ObjectEventWaitForMove(LOCALID_FIRST_THIEF);
  ObjectEvent_FaceDirection(LOCALID_FIRST_THIEF, FACING_EAST, 0);
  Engine_ScriptWait(30);
  VaultInteriors_PlayAnimationAndWait(LOCALID_FIRST_THIEF, ANIMATION_NOD, 20);
  VaultInteriors_ShowMessageAndWait(LOCALID_FIRST_THIEF, 20);
  VaultInteriors_FaceObjectEventAndWait(LOCALID_SECOND_THIEF,
                                        LOCALID_PARTY_LEADER, 30);
  VaultInteriors_ShowMessageAndWait(LOCALID_SECOND_THIEF, 60);
  Engine_ScriptEnd();
}

/// Arranges the loft confrontation and restarts the thieves' battle. "They
/// won't go away, will they? They're back!"
///
/// Called by Map_Initialize at VAULT_INTERIORS_LOFT_FROM_CRATE_ROOM once
/// FLAG_VAULT_THIEVES_ATTACKED is set. Starts the same battle as
/// EventScript_FreeRooferAndConfrontThieves.
static void VaultInteriors_ConfrontThievesAgain(void) {

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
  ObjectEvent_SetPosition(GARET, PIXELS(808), PIXELS(432));
  ObjectEvent_SetPosition(IVAN, PIXELS(792), PIXELS(456));
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(GARET, FACING_NORTHWEST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTHWEST + FACING_STEP, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_TIED_UP_VILLAGER, LOCALID_THIEF_LEADER,
                              0);
  gFieldWork->transition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  Engine_CameraFollowObjectEvent(LOCALID_PARTY_LEADER, 0);
  Engine_CameraWaitForMove();
  Engine_MapGridRedraw();
  Engine_TaskWait(1);
  gFieldWork->transitionFrames = 32;
  VaultInteriors_OpenWindowTransition();
  Engine_ScriptWait(60);
  Engine_ScriptSetMessage(MsgVaultWontGoAway);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_FIRST_THIEF, 1);
  Engine_ScriptWait(20);
  VaultInteriors_ShowMessageAndWait(LOCALID_FIRST_THIEF, 30);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_SECOND_THIEF, 1);
  Engine_ScriptWait(20);
  Script_ShowMessage(LOCALID_SECOND_THIEF, 0);
  VaultInteriors_FaceEachOtherAndWait(LOCALID_THIEF_LEADER, LOCALID_FIRST_THIEF,
                                      30);
  Engine_ObjectEventSetAnimation(LOCALID_THIEF_LEADER, ANIMATION_NOD);
  VaultInteriors_PlayAnimationAndWait(LOCALID_FIRST_THIEF, ANIMATION_NOD, 30);
  VaultInteriors_FaceEachOtherAndWait(LOCALID_THIEF_LEADER,
                                      LOCALID_SECOND_THIEF, 30);
  Engine_ObjectEventSetAnimation(LOCALID_THIEF_LEADER, ANIMATION_NOD);
  VaultInteriors_PlayAnimationAndWait(LOCALID_SECOND_THIEF, ANIMATION_NOD, 40);
  ObjectEvent_FaceDirection(LOCALID_THIEF_LEADER,
                            FACING_SOUTHEAST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_FIRST_THIEF, FACING_SOUTHEAST + FACING_STEP,
                            0);
  ObjectEvent_FaceDirection(LOCALID_SECOND_THIEF,
                            FACING_SOUTHEAST + FACING_STEP, 0);
  Engine_ScriptWait(20);
  // "We'll make you sorry you were ever born! Get 'em!!" and the battle.
  VaultInteriors_PlayAnimationAndWait(LOCALID_THIEF_LEADER,
                                      ANIMATION_SHAKE_HEAD, 20);
  Script_ShowMessage(LOCALID_THIEF_LEADER, 0);
  gFieldWork->transition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
  FieldScript_SetNextDestination((s32)&MapId_VaultInteriors, 17);
  FieldScript_SetDefeatDestination((s32)&MapId_VaultInteriors, 16);
  Battle_SetStart(BATTLE_START_SCRIPTED);
  Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED_VALE_VAULT, 5);
}

/// Counts the Venus Djinn Ivan holds, which he gives back before he leaves the
/// party.
static s32 VaultInteriors_GetIvanVenusDjinnCount(void) {
  return Unit_Get(IVAN)->djinnOwnedCounts[ELEMENT_VENUS];
}

#include "common/vault_djinn.h"

/// Stages the defeated thieves and gathers the townspeople in the loft.
///
/// Called by Map_Initialize at VAULT_INTERIORS_LOFT_AFTER_THIEVES_DEFEATED,
/// the battle's victory destination, unless the party is only coming back to
/// where it was; then VaultInteriors_ReturnStolenGoods follows.
static void VaultInteriors_DefeatThieves(void) {
  struct ObjectEvent *objectEvent;
  struct EventWork *work;
  const s32 *actions;
  struct AnimationObject *thirdThiefSprite;

  objectEvent = ObjectEvent_GetById(LOCALID_SECOND_THIEF);
  thirdThiefSprite = objectEvent->sprite;
  // The beaten thieves lie heaving (animation 9 and the action table's
  // squash) before the party.
  Engine_ScriptStart();
  ObjectEvent_SetPosition(LOCALID_THIEF_LEADER, PIXELS(792), PIXELS(416));
  ObjectEvent_SetPosition(LOCALID_FIRST_THIEF, PIXELS(800), PIXELS(400));
  ObjectEvent_SetPosition(LOCALID_SECOND_THIEF, PIXELS(776), PIXELS(392));
  objectEvent = ObjectEvent_GetById(LOCALID_THIEF_LEADER);
  Engine_ObjectEventSetSpriteFlags(objectEvent, 0);
  objectEvent = ObjectEvent_GetById(LOCALID_FIRST_THIEF);
  Engine_ObjectEventSetSpriteFlags(objectEvent, 0);
  objectEvent = ObjectEvent_GetById(LOCALID_SECOND_THIEF);
  Engine_ObjectEventSetSpriteFlags(objectEvent, 0);
  Engine_ObjectEventSetAnimation(LOCALID_THIEF_LEADER, 9);
  Engine_ObjectEventSetAnimation(LOCALID_FIRST_THIEF, 9);
  Engine_ObjectEventSetAnimation(LOCALID_SECOND_THIEF, 9);
  ObjectEvent_GetById(LOCALID_SECOND_THIEF)->priorityFlags &= 254;
  thirdThiefSprite->image.priority = 3;
  actions = gVaultInteriors_ActionTable;
  ObjectEvent_SetActionScript(LOCALID_THIEF_LEADER, actions);
  ObjectEvent_SetPosition(LOCALID_PARTY_LEADER, PIXELS(792), PIXELS(440));
  ObjectEvent_SetPosition(GARET, PIXELS(808), PIXELS(432));
  ObjectEvent_SetPosition(IVAN, PIXELS(776), PIXELS(440));
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(GARET, FACING_NORTHWEST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTHWEST + FACING_STEP, 0);
  ObjectEvent_SetSpeed(LOCALID_TIED_UP_VILLAGER, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_FaceDirection(LOCALID_TIED_UP_VILLAGER,
                            FACING_NORTHWEST + FACING_STEP, 0);
  gFieldWork->transition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  Engine_CameraFollowObjectEvent(LOCALID_PARTY_LEADER, 0);
  Engine_CameraWaitForMove();
  Engine_MapGridRedraw();
  VaultInteriors_OpenWindowTransition();
  ObjectEvent_SetActionScript(LOCALID_FIRST_THIEF, actions);
  Engine_ScriptWait(30);
  ObjectEvent_SetActionScript(LOCALID_SECOND_THIEF, actions);
  Engine_ScriptWait(30);
  // "They... They got us..." The tied-up villager thought they were goners; he
  // slips past Garet, cheers the win and runs for the mayor.
  Engine_ScriptSetMessage(MsgVaultTheyTheyGotUs);
  VaultInteriors_ShowMessageAndWait(LOCALID_THIEF_LEADER, 20);
  ObjectEvent_ShowEmote(LOCALID_TIED_UP_VILLAGER, EMOTE_IN_FRONT | EMOTE_WINCE,
                        0);
  Engine_ScriptWait(60);
  VaultInteriors_ShowMessageAndWait(LOCALID_TIED_UP_VILLAGER, 20);
  ObjectEvent_WalkToAndWait(LOCALID_TIED_UP_VILLAGER, 808, 456);
  ObjectEvent_SetSpeed(GARET, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_WalkToAndWait(GARET, 792, 432);
  ObjectEvent_FaceDirection(GARET, FACING_EAST, 0);
  ObjectEvent_WalkToAndWait(LOCALID_TIED_UP_VILLAGER, 808, 408);
  ObjectEvent_FaceDirection(LOCALID_TIED_UP_VILLAGER, FACING_WEST, 0);
  ObjectEvent_WalkToAndWait(GARET, 808, 432);
  ObjectEvent_FaceDirection(GARET, FACING_NORTH, 0);
  Engine_ScriptWait(20);
  VaultInteriors_PlayAnimationAndWait(LOCALID_TIED_UP_VILLAGER, ANIMATION_NOD,
                                      20);
  VaultInteriors_ShowMessageAndWait(LOCALID_TIED_UP_VILLAGER, 20);
  ObjectEvent_WalkToAndWait(LOCALID_TIED_UP_VILLAGER, 768, 408);
  Engine_ScriptWait(20);
  ObjectEvent_FaceObjectEvent(LOCALID_TIED_UP_VILLAGER, LOCALID_PARTY_LEADER,
                              0);
  ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_TIED_UP_VILLAGER,
                              0);
  ObjectEvent_FaceObjectEvent(GARET, LOCALID_TIED_UP_VILLAGER, 0);
  VaultInteriors_FaceObjectEventAndWait(IVAN, LOCALID_TIED_UP_VILLAGER, 40);
  VaultInteriors_ShowMessageAndWait(LOCALID_TIED_UP_VILLAGER, 30);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_NOD);
  Engine_ObjectEventSetAnimation(GARET, ANIMATION_NOD);
  VaultInteriors_PlayAnimationAndWait(IVAN, ANIMATION_NOD, 20);
  ObjectEvent_WalkToAndWait(LOCALID_TIED_UP_VILLAGER, 744, 408);
  Engine_ScriptWait(50);
  // The thieves grumble; Garet: stealing is wrong, evil never prevails.
  Engine_ObjectEventRunRepeatedMotion(LOCALID_FIRST_THIEF, 2);
  VaultInteriors_ShowMessageAndWait(LOCALID_FIRST_THIEF, 20);
  ObjectEvent_SetActionScript(LOCALID_FIRST_THIEF, actions);
  Engine_ObjectEventRunRepeatedMotion(GARET, 1);
  Engine_ScriptWait(20);
  ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_FIRST_THIEF, 0);
  ObjectEvent_FaceObjectEvent(GARET, LOCALID_FIRST_THIEF, 0);
  VaultInteriors_FaceObjectEventAndWait(IVAN, LOCALID_FIRST_THIEF, 20);
  VaultInteriors_ShowMessageAndWait(GARET, 20);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_SECOND_THIEF, 2);
  VaultInteriors_ShowMessageAndWait(LOCALID_SECOND_THIEF, 30);
  ObjectEvent_SetActionScript(LOCALID_SECOND_THIEF, actions);
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
  Engine_ScriptWait(60);
  VaultInteriors_ShowMessageAndWait(GARET, 30);
  // Ivan will take the rod to Hammet in Lunpa, and asks what the two will do:
  // go after Saturos and the others.
  VaultInteriors_FaceObjectEventAndWait(IVAN, LOCALID_PARTY_LEADER, 30);
  VaultInteriors_FaceObjectEventAndWait(LOCALID_PARTY_LEADER, IVAN, 30);
  VaultInteriors_PlayAnimationAndWait(IVAN, ANIMATION_NOD, 20);
  VaultInteriors_ShowMessageAndWait(IVAN, 20);
  VaultInteriors_FaceObjectEventAndWait(GARET, IVAN, 30);
  VaultInteriors_PlayAnimationAndWait(LOCALID_PARTY_LEADER, ANIMATION_NOD, 30);
  VaultInteriors_ShowMessageAndWait(IVAN, 20);
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Engine_ScriptWait(60);
  VaultInteriors_ShowMessageAndWait(GARET, 30);
  ObjectEvent_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Engine_ScriptWait(60);
  VaultInteriors_FaceEachOtherAndWait(LOCALID_PARTY_LEADER, GARET, 20);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_NOD);
  VaultInteriors_PlayAnimationAndWait(GARET, ANIMATION_NOD, 20);
  // Ivan reads Isaac's mind. Garet: "Shouldn't we stop him?" Yes: too late,
  // he's done. No: "If you say so."
  VaultInteriors_StartMindRead(IVAN, LOCALID_PARTY_LEADER);
  Ui_SetBank15PaletteAndClearRenderMode();
  ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, IVAN, 0);
  VaultInteriors_FaceObjectEventAndWait(GARET, IVAN, 10);
  Engine_ObjectEventStartRepeatedMotion(LOCALID_PARTY_LEADER, 1);
  Engine_ObjectEventRunRepeatedMotion(GARET, 1);
  Engine_ScriptWait(20);
  ObjectEvent_SetAttachedEffect(GARET, ATTACHED_EFFECT_IN_FRONT |
                                           ATTACHED_EFFECT_ONCE);
  Engine_ScriptWait(60);
  VaultInteriors_FaceObjectEventAndWait(LOCALID_PARTY_LEADER, GARET, 10);
  Script_OpenMessage(GARET, 0);
  if (Engine_ScriptChooseYesNo(0, 0) == 0) {
    Engine_ScriptWait(20);
    ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, IVAN, 0);
    VaultInteriors_FaceObjectEventAndWait(GARET, IVAN, 20);
    VaultInteriors_EndMindRead();
    Engine_ObjectEventStartRepeatedMotion(LOCALID_PARTY_LEADER, 1);
    Engine_ObjectEventRunRepeatedMotion(GARET, 1);
    Engine_ScriptWait(20);
    VaultInteriors_ShowMessageAndWait(GARET, 20);
  } else {
    gFieldWork->message++;
    ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    Engine_ScriptWait(60);
    VaultInteriors_ShowMessageAndWait(GARET, 20);
    ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, IVAN, 0);
    VaultInteriors_FaceObjectEventAndWait(GARET, IVAN, 20);
    VaultInteriors_EndMindRead();
    Engine_ObjectEventStartRepeatedMotion(LOCALID_PARTY_LEADER, 1);
    Engine_ObjectEventRunRepeatedMotion(GARET, 1);
    Engine_ScriptWait(20);
  }
  // "I see... So that's what happened..." The tied-up villager comes back with
  // the mayor and two townsmen.
  ObjectEvent_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Engine_ScriptWait(60);
  VaultInteriors_PlayAnimationAndWait(IVAN, ANIMATION_SHAKE_HEAD, 20);
  Engine_ScriptSetMessage(MsgVaultSeeThatsHappened);
  VaultInteriors_ShowMessageAndWait(IVAN, 20);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_NOD);
  VaultInteriors_PlayAnimationAndWait(GARET, ANIMATION_NOD, 40);
  ObjectEvent_WalkToAndWait(LOCALID_TIED_UP_VILLAGER, 808, 408);
  ObjectEvent_FaceDirection(LOCALID_TIED_UP_VILLAGER, FACING_WEST, 0);
  Engine_ScriptWait(30);
  ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_TIED_UP_VILLAGER,
                              0);
  ObjectEvent_FaceObjectEvent(GARET, LOCALID_TIED_UP_VILLAGER, 0);
  VaultInteriors_FaceObjectEventAndWait(IVAN, LOCALID_TIED_UP_VILLAGER, 20);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_TIED_UP_VILLAGER, 1);
  Engine_ScriptWait(20);
  VaultInteriors_ShowMessageAndWait(LOCALID_TIED_UP_VILLAGER, 20);
  ObjectEvent_SetSpeed(LOCALID_LOFT_MAYOR, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_SetSpeed(LOCALID_LOFT_ESCORT, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_SetSpeed(LOCALID_STRONGMAN_OPENING_THE_CHESTS, PIXELS(4) / 5,
                       PIXELS(2) / 5);
  ObjectEvent_SetPosition(LOCALID_LOFT_MAYOR, PIXELS(744), PIXELS(408));
  ObjectEvent_WalkToAndWait(LOCALID_LOFT_MAYOR, 768, 408);
  VaultInteriors_FaceObjectEventAndWait(LOCALID_LOFT_MAYOR,
                                        LOCALID_THIEF_LEADER, 30);
  ObjectEvent_SetPosition(LOCALID_LOFT_ESCORT, PIXELS(744), PIXELS(408));
  ObjectEvent_WalkToAndWait(LOCALID_LOFT_ESCORT, 768, 408);
  ObjectEvent_SetPosition(LOCALID_STRONGMAN_OPENING_THE_CHESTS, PIXELS(744),
                          PIXELS(408));
  ObjectEvent_WalkTo(LOCALID_STRONGMAN_OPENING_THE_CHESTS, 784, 400);
  ObjectEvent_WalkToAndWait(LOCALID_LOFT_ESCORT, 776, 424);
  Engine_ObjectEventWaitForMove(LOCALID_STRONGMAN_OPENING_THE_CHESTS);
  ObjectEvent_FaceObjectEvent(LOCALID_LOFT_ESCORT, LOCALID_THIEF_LEADER, 0);
  VaultInteriors_FaceObjectEventAndWait(LOCALID_STRONGMAN_OPENING_THE_CHESTS,
                                        LOCALID_THIEF_LEADER, 20);
  ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_LOFT_MAYOR, 0);
  ObjectEvent_FaceObjectEvent(GARET, LOCALID_LOFT_MAYOR, 0);
  VaultInteriors_FaceObjectEventAndWait(IVAN, LOCALID_LOFT_MAYOR, 20);
  // The mayor scolds them for stealing in a disaster; the thieves talk back.
  VaultInteriors_PlayAnimationAndWait(LOCALID_LOFT_MAYOR, ANIMATION_SHAKE_HEAD,
                                      20);
  VaultInteriors_ShowMessageAndWait(LOCALID_LOFT_MAYOR, 20);
  VaultInteriors_ShowMessageAndWait(LOCALID_FIRST_THIEF, 20);
  VaultInteriors_ShowMessageAndWait(LOCALID_SECOND_THIEF, 30);
  // "What should we do with them?" Locked up for a long, long time; the two
  // townsmen rouse the thieves to tie them up.
  VaultInteriors_FaceEachOtherAndWait(LOCALID_LOFT_MAYOR, LOCALID_LOFT_ESCORT,
                                      20);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_LOFT_ESCORT, 1);
  Engine_ScriptWait(20);
  VaultInteriors_ShowMessageAndWait(LOCALID_LOFT_ESCORT, 20);
  VaultInteriors_PlayAnimationAndWait(LOCALID_LOFT_MAYOR, ANIMATION_NOD, 30);
  VaultInteriors_FaceEachOtherAndWait(LOCALID_LOFT_MAYOR,
                                      LOCALID_STRONGMAN_OPENING_THE_CHESTS, 20);
  VaultInteriors_PlayAnimationAndWait(LOCALID_LOFT_MAYOR, ANIMATION_NOD, 30);
  VaultInteriors_FaceObjectEventAndWait(LOCALID_LOFT_MAYOR,
                                        LOCALID_THIEF_LEADER, 20);
  VaultInteriors_ShowMessageAndWait(LOCALID_LOFT_MAYOR, 20);
  Engine_ObjectEventSetAnimation(LOCALID_LOFT_ESCORT, ANIMATION_NOD);
  VaultInteriors_PlayAnimationAndWait(LOCALID_STRONGMAN_OPENING_THE_CHESTS,
                                      ANIMATION_NOD, 20);
  VaultInteriors_FaceEachOtherAndWait(LOCALID_LOFT_ESCORT,
                                      LOCALID_STRONGMAN_OPENING_THE_CHESTS, 20);
  Engine_ObjectEventSetAnimation(LOCALID_LOFT_ESCORT, ANIMATION_NOD);
  VaultInteriors_PlayAnimationAndWait(LOCALID_STRONGMAN_OPENING_THE_CHESTS,
                                      ANIMATION_NOD, 20);
  ObjectEvent_WalkTo(LOCALID_STRONGMAN_OPENING_THE_CHESTS, 792, 392);
  ObjectEvent_WalkToAndWait(LOCALID_LOFT_ESCORT, 784, 400);
  ObjectEvent_FaceObjectEvent(LOCALID_LOFT_ESCORT, LOCALID_SECOND_THIEF, 0);
  Engine_ObjectEventWaitForMove(LOCALID_STRONGMAN_OPENING_THE_CHESTS);
  VaultInteriors_FaceObjectEventAndWait(LOCALID_STRONGMAN_OPENING_THE_CHESTS,
                                        LOCALID_FIRST_THIEF, 20);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_LOFT_ESCORT, 1);
  Engine_ScriptWait(20);
  VaultInteriors_ShowMessageAndWait(LOCALID_LOFT_ESCORT, 20);
  VaultInteriors_PlayAnimationAndWait(LOCALID_STRONGMAN_OPENING_THE_CHESTS,
                                      ANIMATION_SHAKE_HEAD, 20);
  VaultInteriors_ShowMessageAndWait(LOCALID_STRONGMAN_OPENING_THE_CHESTS, 30);
  VaultInteriors_FaceEachOtherAndWait(LOCALID_LOFT_ESCORT, LOCALID_PARTY_LEADER,
                                      20);
  VaultInteriors_ShowMessageAndWait(LOCALID_LOFT_ESCORT, 20);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_NOD);
  Engine_ObjectEventSetAnimation(GARET, ANIMATION_NOD);
  VaultInteriors_PlayAnimationAndWait(IVAN, ANIMATION_NOD, 50);
  // The window closes while the thieves are tied up.
  work = gFieldWork;
  work->transitionFrames = 30;
  work->transition = MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
  Engine_ScriptCloseScreen();
  Engine_ScriptWaitForScreen();
  Engine_ScriptWait(60);
  Engine_ScriptEnd();
}

/// Returns the stolen goods and rod, thanks the party, and clears the loft.
///
/// Called by Map_Initialize after VaultInteriors_DefeatThieves. Sets
/// FLAG_VAULT_THIEVES_JAILED; the thieves are led away, the mayor's urn and
/// the Sanctum's gold statue come out of the chests, Ivan takes the Shaman's
/// Rod, gives back any Venus Djinn and leaves the party, and every object event
/// of the loft is cleared away.
static void VaultInteriors_ReturnStolenGoods(void) {
  struct ObjectEvent *temporaryObjectEvent;
  struct ObjectEvent *objectEvent;
  s32 aftermath;
  s32 rod;
  struct EventWork *work;

  work = gFieldWork;
  GameFlag_Set(FLAG_VAULT_THIEVES_JAILED);
  // With the screen still closed, the chests go back along the wall and the
  // thieves stand tied before the mayor.
  Engine_ScriptStart();
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
  Engine_ObjectEventSetAnimation(LOCALID_THIEF_LEADER, 5);
  Engine_ObjectEventSetAnimation(LOCALID_FIRST_THIEF, 5);
  Engine_ObjectEventSetAnimation(LOCALID_SECOND_THIEF, 5);
  ObjectEvent_FaceObjectEvent(LOCALID_FIRST_THIEF, LOCALID_PARTY_LEADER, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_THIEF_LEADER, LOCALID_PARTY_LEADER, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_SECOND_THIEF, LOCALID_PARTY_LEADER, 0);
  objectEvent = ObjectEvent_GetById(LOCALID_THIEF_LEADER);
  Engine_ObjectEventSetSpriteFlags(objectEvent, SPRITE_FLAG_SHADOW);
  objectEvent = ObjectEvent_GetById(LOCALID_FIRST_THIEF);
  Engine_ObjectEventSetSpriteFlags(objectEvent, SPRITE_FLAG_SHADOW);
  objectEvent = ObjectEvent_GetById(LOCALID_SECOND_THIEF);
  Engine_ObjectEventSetSpriteFlags(objectEvent, SPRITE_FLAG_SHADOW);
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
  ObjectEvent_SetPosition(GARET, PIXELS(808), PIXELS(440));
  ObjectEvent_SetPosition(IVAN, PIXELS(776), PIXELS(440));
  ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_THIEF_LEADER, 0);
  ObjectEvent_FaceObjectEvent(GARET, LOCALID_THIEF_LEADER, 0);
  ObjectEvent_FaceObjectEvent(IVAN, LOCALID_THIEF_LEADER, 0);
  gFieldWork->transitionFrames = 30;
  gFieldWork->transition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
  Engine_ScriptOpenScreen();
  Engine_ScriptWaitForScreen();
  Engine_ScriptWait(40);
  // "Isaac, right? I won't forget this!" "Take them away!"
  Engine_ObjectEventRunRepeatedMotion(LOCALID_THIEF_LEADER, 2);
  aftermath = MsgVaultYouIsaacRightWontForget;
  Engine_ScriptSetMessage(aftermath);
  VaultInteriors_ShowMessageAndWait(LOCALID_THIEF_LEADER, 20);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_LOFT_MAYOR, 1);
  Engine_ScriptWait(20);
  VaultInteriors_PlayAnimationAndWait(LOCALID_LOFT_MAYOR, ANIMATION_SHAKE_HEAD,
                                      20);
  VaultInteriors_ShowMessageAndWait(LOCALID_LOFT_MAYOR, 20);
  Engine_ObjectEventSetAnimation(LOCALID_LOFT_ESCORT, ANIMATION_NOD);
  VaultInteriors_PlayAnimationAndWait(LOCALID_TIED_UP_VILLAGER, ANIMATION_NOD,
                                      20);
  Engine_ObjectEventStartRepeatedMotion(LOCALID_FIRST_THIEF, 2);
  Engine_ObjectEventStartRepeatedMotion(LOCALID_SECOND_THIEF, 2);
  Engine_ScriptWait(60);
  // The escort leads the three thieves down the ladder, the tied-up villager
  // after them.
  ObjectEvent_SetSpeed(LOCALID_LOFT_ESCORT, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_WalkTo(LOCALID_LOFT_ESCORT, 746, 408);
  ObjectEvent_FaceDirection(LOCALID_LOFT_MAYOR, FACING_NORTHWEST + FACING_STEP,
                            0);
  ObjectEvent_FaceDirection(LOCALID_STRONGMAN_OPENING_THE_CHESTS,
                            FACING_NORTHWEST + FACING_STEP, 0);
  ObjectEvent_SetActionScript(LOCALID_FIRST_THIEF,
                              gVaultInteriors_VaultScriptB);
  Engine_ScriptWait(20);
  ObjectEvent_SetActionScript(LOCALID_THIEF_LEADER,
                              gVaultInteriors_VaultScriptB);
  Engine_ScriptWait(15);
  ObjectEvent_SetActionScript(LOCALID_SECOND_THIEF,
                              gVaultInteriors_VaultScriptB);
  Engine_ScriptWait(35);
  ObjectEvent_SetActionScript(LOCALID_TIED_UP_VILLAGER,
                              gVaultInteriors_VaultScriptA);
  Engine_ScriptWait(20);
  Engine_ObjectEventWaitForMove(LOCALID_LOFT_ESCORT);
  ObjectEvent_SetPosition(LOCALID_LOFT_ESCORT, 0, 0);
  Engine_ScriptWait(40);
  // The mayor and the villager opening the chests thank the party.
  ObjectEvent_SetSpeed(LOCALID_LOFT_MAYOR, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_SetSpeed(LOCALID_STRONGMAN_OPENING_THE_CHESTS, PIXELS(4) / 5,
                       PIXELS(2) / 5);
  ObjectEvent_WalkToAndWait(LOCALID_LOFT_MAYOR, 784, 408);
  ObjectEvent_FaceObjectEvent(LOCALID_LOFT_MAYOR, LOCALID_PARTY_LEADER, 0);
  ObjectEvent_WalkToAndWait(LOCALID_STRONGMAN_OPENING_THE_CHESTS, 768, 408);
  VaultInteriors_FaceObjectEventAndWait(LOCALID_STRONGMAN_OPENING_THE_CHESTS,
                                        LOCALID_PARTY_LEADER, 20);
  VaultInteriors_PlayAnimationAndWait(LOCALID_LOFT_MAYOR, ANIMATION_NOD, 20);
  VaultInteriors_ShowMessageAndWait(LOCALID_LOFT_MAYOR, 20);
  VaultInteriors_PlayAnimationAndWait(LOCALID_STRONGMAN_OPENING_THE_CHESTS,
                                      ANIMATION_NOD, 20);
  ObjectEvent_FaceDirection(LOCALID_STRONGMAN_OPENING_THE_CHESTS,
                            FACING_SOUTHEAST, 10);
  VaultInteriors_ShowMessageAndWait(LOCALID_STRONGMAN_OPENING_THE_CHESTS, 20);
  VaultInteriors_FaceEachOtherAndWait(LOCALID_PARTY_LEADER, GARET, 50);
  VaultInteriors_FaceEachOtherAndWait(LOCALID_PARTY_LEADER, IVAN, 50);
  ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_LOFT_MAYOR, 0);
  ObjectEvent_FaceObjectEvent(GARET, LOCALID_LOFT_MAYOR, 0);
  VaultInteriors_FaceObjectEventAndWait(IVAN, LOCALID_LOFT_MAYOR, 20);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_NOD);
  Engine_ObjectEventSetAnimation(GARET, ANIMATION_NOD);
  VaultInteriors_PlayAnimationAndWait(IVAN, ANIMATION_NOD, 50);
  // "Let's take a look at what they stole." The two go round to the chests.
  Engine_ObjectEventFaceEachOther(LOCALID_LOFT_MAYOR,
                                  LOCALID_STRONGMAN_OPENING_THE_CHESTS, 0);
  VaultInteriors_PlayAnimationAndWait(LOCALID_LOFT_MAYOR, ANIMATION_NOD, 20);
  VaultInteriors_ShowMessageAndWait(LOCALID_LOFT_MAYOR, 20);
  ObjectEvent_SetActionScript(LOCALID_STRONGMAN_OPENING_THE_CHESTS,
                              gVaultInteriors_VaultScriptC);
  Engine_ScriptWait(50);
  ObjectEvent_SetActionScript(LOCALID_LOFT_MAYOR, gVaultInteriors_VaultScriptD);
  ObjectEvent_SetSpeed(GARET, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_SetSpeed(IVAN, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_WalkToAndWait(GARET, 792, 456);
  ObjectEvent_FaceDirection(GARET, FACING_NORTH + FACING_STEP, 0);
  Engine_ScriptWait(30);
  ObjectEvent_WalkToAndWait(IVAN, 792, 408);
  ObjectEvent_FaceDirection(IVAN, FACING_EAST, 0);
  ObjectEvent_WalkToAndWait(GARET, 808, 456);
  ObjectEvent_FaceDirection(GARET, FACING_NORTH + FACING_STEP, 0);
  Engine_ScriptWait(100);
  VaultInteriors_FaceObjectEventAndWait(LOCALID_STRONGMAN_OPENING_THE_CHESTS,
                                        LOCALID_LOFT_MAYOR, 60);
  VaultInteriors_FaceObjectEventAndWait(
      LOCALID_LOFT_MAYOR, LOCALID_STRONGMAN_OPENING_THE_CHESTS, 40);
  VaultInteriors_PlayAnimationAndWait(LOCALID_LOFT_MAYOR, ANIMATION_NOD, 40);
  ObjectEvent_FaceDirection(LOCALID_LOFT_MAYOR, FACING_EAST, 0);
  Engine_ScriptWait(20);
  // The mayor opens the first chest and his urn rises out of it.
  Engine_ObjectEventRunRepeatedMotion(LOCALID_LOFT_MAYOR, 2);
  Script_PlaySong(SE_124);
  Engine_ObjectEventSetAnimation(LOCALID_FIRST_CHEST, ANIMATION_SHAKE_HEAD);
  ObjectEvent_SetPosition(LOCALID_FOUND_URN, PIXELS(872), PIXELS(424));
  Engine_ObjectEventSetSpritePriority(LOCALID_FOUND_URN, 1);
  ObjectEvent_SetSpeed(LOCALID_FOUND_URN, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_SetDestinationOffset(LOCALID_FOUND_URN, 0, -8);
  Engine_ObjectEventWaitForMove(LOCALID_FOUND_URN);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_FOUND_URN, 2);
  Engine_ScriptWait(60);
  Engine_MessageShowCentered((aftermath + 5), 1);
  Engine_ObjectEventSetAnimation(LOCALID_FIRST_CHEST, ANIMATION_WALK);
  ObjectEvent_SetPosition(LOCALID_FOUND_URN, 0, 0);
  gFieldWork->message++;
  Engine_ObjectEventRunRepeatedMotion(LOCALID_STRONGMAN_OPENING_THE_CHESTS, 1);
  VaultInteriors_ShowMessageAndWait(LOCALID_STRONGMAN_OPENING_THE_CHESTS, 20);
  VaultInteriors_FaceEachOtherAndWait(LOCALID_PARTY_LEADER, GARET, 40);
  VaultInteriors_FaceObjectEventAndWait(
      LOCALID_LOFT_MAYOR, LOCALID_STRONGMAN_OPENING_THE_CHESTS, 20);
  VaultInteriors_PlayAnimationAndWait(LOCALID_LOFT_MAYOR, ANIMATION_NOD, 20);
  VaultInteriors_ShowMessageAndWait(LOCALID_LOFT_MAYOR, 30);
  ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER,
                              LOCALID_STRONGMAN_OPENING_THE_CHESTS, 0);
  VaultInteriors_FaceObjectEventAndWait(
      GARET, LOCALID_STRONGMAN_OPENING_THE_CHESTS, 40);
  ObjectEvent_FaceDirection(LOCALID_STRONGMAN_OPENING_THE_CHESTS, FACING_EAST,
                            0);
  Engine_ScriptWait(40);
  // The villager opening the chests opens the second: the gold statue stolen
  // from the Sanctum.
  Engine_ObjectEventRunRepeatedMotion(LOCALID_STRONGMAN_OPENING_THE_CHESTS, 2);
  Script_PlaySong(SE_124);
  Engine_ObjectEventSetAnimation(LOCALID_SECOND_CHEST, ANIMATION_SHAKE_HEAD);
  ObjectEvent_GetById(LOCALID_FOUND_STATUE)->motionFlags = 0;
  Engine_ObjectEventSetSpritePriority(LOCALID_FOUND_STATUE, 1);
  ObjectEvent_SetPosition(LOCALID_FOUND_STATUE, PIXELS(872), PIXELS(408));
  ObjectEvent_SetSpeed(LOCALID_FOUND_STATUE, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_SetDestinationOffset(LOCALID_FOUND_STATUE, 0, -8);
  Engine_ObjectEventWaitForMove(LOCALID_FOUND_STATUE);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_FOUND_STATUE, 2);
  Engine_ScriptWait(60);
  Engine_MessageShowCentered((aftermath + 8), 1);
  Engine_ObjectEventSetAnimation(LOCALID_SECOND_CHEST, ANIMATION_WALK);
  ObjectEvent_SetPosition(LOCALID_FOUND_STATUE, 0, 0);
  gFieldWork->message++;
  ObjectEvent_ShowEmote(LOCALID_LOFT_MAYOR, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Engine_ScriptWait(60);
  VaultInteriors_ShowMessageAndWait(LOCALID_LOFT_MAYOR, 20);
  Engine_ObjectEventStartRepeatedMotion(LOCALID_PARTY_LEADER, 2);
  Engine_ObjectEventStartRepeatedMotion(GARET, 2);
  Engine_ObjectEventRunRepeatedMotion(IVAN, 2);
  Engine_ScriptWait(20);
  VaultInteriors_PlayAnimationAndWait(LOCALID_STRONGMAN_OPENING_THE_CHESTS,
                                      ANIMATION_NOD, 50);
  VaultInteriors_FaceObjectEventAndWait(LOCALID_LOFT_MAYOR,
                                        LOCALID_PARTY_LEADER, 20);
  VaultInteriors_ShowMessageAndWait(LOCALID_LOFT_MAYOR, 30);
  // The villager opening the chests finds another chest; the mayor asks whether
  // it is what Ivan was looking for, and he can return to Master Hammet.
  ObjectEvent_FaceDirection(LOCALID_STRONGMAN_OPENING_THE_CHESTS,
                            FACING_NORTH + FACING_STEP, 0);
  Engine_ScriptWait(30);
  ObjectEvent_ShowEmote(LOCALID_STRONGMAN_OPENING_THE_CHESTS,
                        EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Engine_ScriptWait(60);
  ObjectEvent_WalkToAndWait(LOCALID_STRONGMAN_OPENING_THE_CHESTS, 856, 376);
  Engine_ScriptWait(20);
  VaultInteriors_FaceObjectEventAndWait(LOCALID_STRONGMAN_OPENING_THE_CHESTS,
                                        LOCALID_LOFT_MAYOR, 20);
  VaultInteriors_ShowMessageAndWait(LOCALID_STRONGMAN_OPENING_THE_CHESTS, 20);
  ObjectEvent_FaceObjectEvent(LOCALID_LOFT_MAYOR,
                              LOCALID_STRONGMAN_OPENING_THE_CHESTS, 0);
  ObjectEvent_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Engine_ScriptWait(60);
  VaultInteriors_FaceObjectEventAndWait(
      IVAN, LOCALID_STRONGMAN_OPENING_THE_CHESTS, 30);
  VaultInteriors_FaceObjectEventAndWait(LOCALID_LOFT_MAYOR, IVAN, 20);
  VaultInteriors_PlayAnimationAndWait(LOCALID_LOFT_MAYOR, ANIMATION_NOD, 20);
  Script_ShowMessage(LOCALID_LOFT_MAYOR, 0);
  ObjectEvent_FaceDirection(LOCALID_STRONGMAN_OPENING_THE_CHESTS,
                            FACING_SOUTH + FACING_STEP, 0);
  Engine_ScriptWait(30);
  VaultInteriors_FaceObjectEventAndWait(IVAN, LOCALID_LOFT_MAYOR, 20);
  VaultInteriors_PlayAnimationAndWait(IVAN, ANIMATION_NOD, 20);
  VaultInteriors_ShowMessageAndWait(LOCALID_LOFT_MAYOR, 20);
  Engine_ObjectEventFaceEachOther(LOCALID_PARTY_LEADER, IVAN, 0);
  VaultInteriors_FaceObjectEventAndWait(GARET, IVAN, 20);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_NOD);
  VaultInteriors_PlayAnimationAndWait(GARET, ANIMATION_NOD, 40);
  VaultInteriors_PlayAnimationAndWait(IVAN, ANIMATION_NOD, 30);
  // Hammet was seen heading toward Lunpa: Dodonpa must have kidnapped him.
  Engine_ObjectEventRunRepeatedMotion(LOCALID_LOFT_MAYOR, 1);
  Engine_ScriptWait(20);
  VaultInteriors_PlayAnimationAndWait(LOCALID_LOFT_MAYOR, ANIMATION_SHAKE_HEAD,
                                      20);
  VaultInteriors_ShowMessageAndWait(LOCALID_LOFT_MAYOR, 20);
  ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_LOFT_MAYOR, 0);
  ObjectEvent_FaceObjectEvent(GARET, LOCALID_LOFT_MAYOR, 0);
  VaultInteriors_FaceObjectEventAndWait(IVAN, LOCALID_LOFT_MAYOR, 20);
  Engine_ObjectEventStartRepeatedMotion(LOCALID_PARTY_LEADER, 1);
  Engine_ObjectEventStartRepeatedMotion(GARET, 1);
  ObjectEvent_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Engine_ScriptWait(60);
  VaultInteriors_ShowMessageAndWait(IVAN, 40);
  VaultInteriors_PlayAnimationAndWait(LOCALID_LOFT_MAYOR, ANIMATION_NOD, 20);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_SHAKE_HEAD);
  Engine_ObjectEventSetAnimationAndWait(GARET, ANIMATION_SHAKE_HEAD);
  Engine_ObjectEventRunRepeatedMotion(IVAN, 3);
  Engine_ScriptWait(30);
  VaultInteriors_PlayAnimationAndWait(LOCALID_LOFT_MAYOR, ANIMATION_SHAKE_HEAD,
                                      20);
  VaultInteriors_ShowMessageAndWait(LOCALID_LOFT_MAYOR, 20);
  VaultInteriors_FaceEachOtherAndWait(LOCALID_PARTY_LEADER, GARET, 20);
  // Ivan rushes forward: what will happen to Hammet? The mayor calms him:
  // Dodonpa wants a ransom, so Hammet is safe for now.
  Engine_ObjectEventRunRepeatedMotion(IVAN, 2);
  VaultInteriors_PlayAnimationAndWait(IVAN, ANIMATION_SHAKE_HEAD, 30);
  ObjectEvent_SetSpeed(IVAN, PIXELS(3) / 2, PIXELS(3) / 4);
  ObjectEvent_WalkToAndWait(IVAN, 800, 408);
  Engine_ObjectEventRunRepeatedMotion(IVAN, 2);
  Script_ShowMessage(IVAN, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_LOFT_MAYOR, 0);
  VaultInteriors_FaceObjectEventAndWait(GARET, LOCALID_LOFT_MAYOR, 30);
  VaultInteriors_PlayAnimationAndWait(LOCALID_LOFT_MAYOR, ANIMATION_SHAKE_HEAD,
                                      20);
  VaultInteriors_ShowMessageAndWait(LOCALID_LOFT_MAYOR, 20);
  ObjectEvent_SetAttachedEffect(LOCALID_PARTY_LEADER, ATTACHED_EFFECT_IN_FRONT |
                                                          ATTACHED_EFFECT_ONCE);
  ObjectEvent_SetAttachedEffect(GARET, ATTACHED_EFFECT_IN_FRONT |
                                           ATTACHED_EFFECT_ONCE);
  ObjectEvent_SetAttachedEffect(IVAN, ATTACHED_EFFECT_IN_FRONT |
                                          ATTACHED_EFFECT_ONCE);
  Engine_ScriptWait(60);
  VaultInteriors_ShowMessageAndWait(IVAN, 20);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_LOFT_MAYOR, 1);
  Engine_ScriptWait(20);
  VaultInteriors_ShowMessageAndWait(LOCALID_LOFT_MAYOR, 20);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION,
                        0);
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  ObjectEvent_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Engine_ScriptWait(60);
  VaultInteriors_PlayAnimationAndWait(LOCALID_LOFT_MAYOR, ANIMATION_SHAKE_HEAD,
                                      20);
  VaultInteriors_ShowMessageAndWait(LOCALID_LOFT_MAYOR, 20);
  Engine_ObjectEventStartRepeatedMotion(LOCALID_PARTY_LEADER, 1);
  Engine_ObjectEventStartRepeatedMotion(GARET, 1);
  Engine_ObjectEventRunRepeatedMotion(IVAN, 1);
  Engine_ScriptWait(20);
  VaultInteriors_PlayAnimationAndWait(LOCALID_LOFT_MAYOR, ANIMATION_NOD, 20);
  VaultInteriors_ShowMessageAndWait(LOCALID_LOFT_MAYOR, 40);
  // The mayor asks the two their names, thanks them, and asks them to come
  // see him before they leave town.
  ObjectEvent_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Engine_ScriptWait(60);
  ObjectEvent_WalkToAndWait(LOCALID_LOFT_MAYOR, 840, 424);
  VaultInteriors_FaceEachOtherAndWait(LOCALID_LOFT_MAYOR, LOCALID_PARTY_LEADER,
                                      20);
  VaultInteriors_ShowMessageAndWait(LOCALID_LOFT_MAYOR, 20);
  VaultInteriors_PlayAnimationAndWait(LOCALID_PARTY_LEADER, ANIMATION_NOD, 20);
  ObjectEvent_FaceDirection(LOCALID_LOFT_MAYOR, FACING_SOUTH + FACING_STEP, 0);
  Engine_ScriptWait(20);
  VaultInteriors_ShowMessageAndWait(LOCALID_LOFT_MAYOR, 20);
  Engine_ObjectEventRunRepeatedMotion(GARET, 1);
  Engine_ScriptWait(20);
  VaultInteriors_ShowMessageAndWait(GARET, 20);
  VaultInteriors_PlayAnimationAndWait(LOCALID_LOFT_MAYOR, ANIMATION_NOD, 20);
  VaultInteriors_ShowMessageAndWait(LOCALID_LOFT_MAYOR, 30);
  VaultInteriors_FaceEachOtherAndWait(LOCALID_LOFT_MAYOR,
                                      LOCALID_STRONGMAN_OPENING_THE_CHESTS, 20);
  VaultInteriors_ShowMessageAndWait(LOCALID_LOFT_MAYOR, 20);
  // The mayor and the villager opening the chests leave down the ladder.
  ObjectEvent_WalkToAndWait(LOCALID_STRONGMAN_OPENING_THE_CHESTS, 856, 408);
  ObjectEvent_SetActionScript(LOCALID_LOFT_MAYOR, gVaultInteriors_VaultScriptE);
  ObjectEvent_SetActionScript(LOCALID_STRONGMAN_OPENING_THE_CHESTS,
                              gVaultInteriors_VaultScriptE);
  ObjectEvent_WalkTo(GARET, 792, 456);
  ObjectEvent_SetSpeed(IVAN, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_WalkTo(IVAN, 776, 432);
  Engine_ObjectEventWaitForMove(GARET);
  ObjectEvent_FaceDirection(GARET, FACING_NORTH + FACING_STEP, 0);
  Engine_ObjectEventWaitForMove(IVAN);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTH + FACING_STEP, 0);
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
  Engine_ObjectEventSetAnimation(LOCALID_STRONGMAN_OPENING_THE_CHESTS,
                                 ANIMATION_STAND);
  Engine_ScriptWait(50);
  ObjectEvent_FaceObjectEvent(LOCALID_LOFT_MAYOR, LOCALID_PARTY_LEADER, 0);
  ObjectEvent_WalkToAndWait(GARET, 808, 440);
  ObjectEvent_FaceDirection(GARET, FACING_NORTHWEST + FACING_STEP, 0);
  Engine_ScriptWait(20);
  VaultInteriors_ShowMessageAndWait(LOCALID_LOFT_MAYOR, 20);
  VaultInteriors_FaceEachOtherAndWait(LOCALID_PARTY_LEADER, GARET, 50);
  ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_LOFT_MAYOR, 0);
  VaultInteriors_FaceObjectEventAndWait(GARET, LOCALID_LOFT_MAYOR, 20);
  VaultInteriors_PlayAnimationAndWait(LOCALID_LOFT_MAYOR, ANIMATION_NOD, 20);
  ObjectEvent_WalkTo(LOCALID_LOFT_MAYOR, 744, 408);
  ObjectEvent_WalkTo(LOCALID_STRONGMAN_OPENING_THE_CHESTS, 744, 408);
  Engine_ObjectEventWaitForMove(LOCALID_LOFT_MAYOR);
  Engine_ObjectEventWaitForMove(LOCALID_STRONGMAN_OPENING_THE_CHESTS);
  Engine_ScriptWait(30);
  // Alone, Garet tries to cheer Ivan up; Ivan only wants to rescue Hammet,
  // but has seen the two's mission in their minds and won't ask their help.
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 792, 408);
  VaultInteriors_FaceEachOtherAndWait(LOCALID_PARTY_LEADER, GARET, 30);
  ObjectEvent_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Engine_ScriptWait(50);
  ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, IVAN, 0);
  VaultInteriors_FaceObjectEventAndWait(GARET, IVAN, 20);
  Engine_ObjectEventRunRepeatedMotion(GARET, 1);
  Engine_ScriptWait(20);
  VaultInteriors_ShowMessageAndWait(GARET, 40);
  VaultInteriors_PlayAnimationAndWait(IVAN, ANIMATION_NOD, 30);
  VaultInteriors_FaceEachOtherAndWait(LOCALID_PARTY_LEADER, GARET, 40);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_SHAKE_HEAD);
  VaultInteriors_PlayAnimationAndWait(GARET, ANIMATION_SHAKE_HEAD, 30);
  ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, IVAN, 0);
  VaultInteriors_FaceObjectEventAndWait(GARET, IVAN, 20);
  Engine_ObjectEventRunRepeatedMotion(GARET, 1);
  Engine_ScriptWait(20);
  VaultInteriors_ShowMessageAndWait(GARET, 40);
  Engine_ObjectEventRunRepeatedMotion(IVAN, 2);
  Engine_ScriptWait(20);
  VaultInteriors_ShowMessageAndWait(IVAN, 20);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_NOD);
  VaultInteriors_PlayAnimationAndWait(GARET, ANIMATION_NOD, 20);
  VaultInteriors_PlayAnimationAndWait(IVAN, ANIMATION_SHAKE_HEAD, 20);
  VaultInteriors_ShowMessageAndWait(IVAN, 20);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION,
                        0);
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Engine_ScriptWait(60);
  VaultInteriors_PlayAnimationAndWait(IVAN, ANIMATION_NOD, 20);
  VaultInteriors_ShowMessageAndWait(IVAN, 20);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER,
                        EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Engine_ScriptWait(60);
  VaultInteriors_PlayAnimationAndWait(IVAN, ANIMATION_NOD, 20);
  VaultInteriors_ShowMessageAndWait(IVAN, 20);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Engine_ScriptWait(60);
  VaultInteriors_PlayAnimationAndWait(IVAN, ANIMATION_SHAKE_HEAD, 20);
  VaultInteriors_ShowMessageAndWait(IVAN, 20);
  VaultInteriors_PlayAnimationAndWait(GARET, ANIMATION_NOD, 20);
  VaultInteriors_ShowMessageAndWait(GARET, 30);
  // "Then, I suppose this is good-bye." A no has Garet say Isaac would help,
  // and Ivan decline.
  Script_OpenMessage(IVAN, 0);
  if (Engine_ScriptChooseYesNo(0, 0) != 0) {
    Engine_ScriptWait(20);
    Engine_ObjectEventRunRepeatedMotion(GARET, 1);
    Engine_ScriptWait(20);
    VaultInteriors_ShowMessageAndWait(GARET, 20);
    VaultInteriors_PlayAnimationAndWait(IVAN, ANIMATION_SHAKE_HEAD, 20);
    VaultInteriors_ShowMessageAndWait(IVAN, 20);
  }
  // Ivan walks round to the third chest and takes the Shaman's Rod from it.
  VaultInteriors_PlayAnimationAndWait(IVAN, ANIMATION_NOD, 30);
  ObjectEvent_WalkToAndWait(IVAN, 800, 456);
  ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, IVAN, 0);
  ObjectEvent_FaceObjectEvent(GARET, IVAN, 0);
  ObjectEvent_WalkToAndWait(IVAN, 856, 456);
  ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, IVAN, 0);
  ObjectEvent_FaceObjectEvent(GARET, IVAN, 0);
  ObjectEvent_WalkToAndWait(IVAN, 856, 376);
  ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, IVAN, 0);
  ObjectEvent_FaceObjectEvent(GARET, IVAN, 0);
  ObjectEvent_FaceDirection(IVAN, FACING_EAST, 0);
  Engine_ScriptWait(20);
  Engine_ObjectEventRunRepeatedMotion(IVAN, 2);
  temporaryObjectEvent =
      (struct ObjectEvent *)FieldEffect_PlayCueAndStartEmitterOnTarget(
          IVAN, LOCALID_THIRD_CHEST, ITEM_SHAMANS_ROD);
  Engine_ScriptWait(60);
  rod = MsgVaultIvanGotShamansRod;
  Engine_MessageShowCentered(rod, 1);
  Engine_ObjectDispatchRelease(temporaryObjectEvent);
  Engine_ObjectEventSetAnimation(LOCALID_THIRD_CHEST, ANIMATION_WALK);
  Engine_ScriptWait(20);
  VaultInteriors_PlayAnimationAndWait(IVAN, ANIMATION_NOD, 20);
  ObjectEvent_WalkToAndWait(IVAN, 856, 456);
  ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, IVAN, 0);
  ObjectEvent_FaceObjectEvent(GARET, IVAN, 0);
  ObjectEvent_WalkToAndWait(IVAN, 800, 456);
  ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, IVAN, 0);
  ObjectEvent_FaceObjectEvent(GARET, IVAN, 0);
  ObjectEvent_WalkToAndWait(IVAN, 776, 424);
  Engine_ObjectEventFaceEachOther(LOCALID_PARTY_LEADER, IVAN, 0);
  VaultInteriors_FaceObjectEventAndWait(GARET, IVAN, 30);
  // He thanks the town, hands back any Venus Djinni and leaves the party.
  VaultInteriors_PlayAnimationAndWait(IVAN, ANIMATION_NOD, 20);
  Engine_ScriptSetMessage((rod + 1));
  VaultInteriors_ShowMessageAndWait(IVAN, 20);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_NOD);
  {
    u16 *slot = &work->message;
    s32 saved = (s16)*slot;

    if (VaultInteriors_GetIvanVenusDjinnCount() != 0) {
      Engine_ScriptSetMessage(MsgVaultWaitDontWantTakeYour);
      Script_ShowMessage(IVAN, 0);
      Vault_ReturnIsaacsDjinni();
    }
    Party_RemoveOwnerRestored(IVAN);
    *slot = saved;
  }
  VaultInteriors_PlayAnimationAndWait(GARET, ANIMATION_NOD, 50);
  ObjectEvent_WalkToAndWait(IVAN, 776, 408);
  ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, IVAN, 0);
  ObjectEvent_FaceObjectEvent(GARET, IVAN, 0);
  // Ivan goes down the ladder; Garet: they'd better get moving too.
  ObjectEvent_WalkToAndWait(IVAN, 744, 408);
  Engine_ScriptWait(40);
  VaultInteriors_FaceEachOtherAndWait(LOCALID_PARTY_LEADER, GARET, 20);
  Engine_ObjectEventRunRepeatedMotion(GARET, 1);
  Engine_ScriptWait(20);
  VaultInteriors_ShowMessageAndWait(GARET, 20);
  VaultInteriors_PlayAnimationAndWait(LOCALID_PARTY_LEADER, ANIMATION_NOD, 20);
  Engine_ObjectEventSetAnimation(GARET, ANIMATION_WALK);
  objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (objectEvent != NULL) {
    ObjectEvent_SetDestination(GARET, objectEvent->x.part.pixel,
                               objectEvent->z.part.pixel);
  }
  Engine_ObjectEventWaitForMove(GARET);
  ObjectEvent_SetPosition(GARET, 0, 0);
  Engine_ScriptWait(30);
  // Clear the loft of everyone the cutscene used.
  ObjectEvent_SetPosition(LOCALID_TIED_UP_VILLAGER, 0, 0);
  ObjectEvent_SetPosition(LOCALID_LOFT_MAYOR, 0, 0);
  ObjectEvent_SetPosition(LOCALID_LOFT_ESCORT, 0, 0);
  ObjectEvent_SetPosition(LOCALID_STRONGMAN_OPENING_THE_CHESTS, 0, 0);
  ObjectEvent_SetPosition(LOCALID_THIEF_LEADER, 0, 0);
  ObjectEvent_SetPosition(LOCALID_FIRST_THIEF, 0, 0);
  ObjectEvent_SetPosition(LOCALID_SECOND_THIEF, 0, 0);
  ObjectEvent_SetPosition(IVAN, 0, 0);
  gFieldWork->transition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  Engine_ScriptEnd();
}

/// Opens the window transition and waits one frame after it finishes.
static void VaultInteriors_OpenWindowTransition(void) {
  struct EventWork *state;

  state = gFieldWork;
  state->transition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  Engine_ScriptOpenScreen();
  Engine_ScriptWaitForScreen();
  Engine_ScriptWait(1);
}

static void VaultInteriors_ShowMessageAndWait(s32 localId, s32 waitFrames) {
  Engine_ScriptShowMessage(localId, 0);
  Engine_ScriptWait(waitFrames);
}

static void VaultInteriors_FaceObjectEventAndWait(s32 localId, s32 targetId,
                                                  s32 waitFrames) {
  Engine_ObjectEventFaceObjectEvent(localId, targetId, 0);
  Engine_ScriptWait(waitFrames);
}

static void VaultInteriors_FaceEachOtherAndWait(s32 firstId, s32 secondId,
                                                s32 waitFrames) {
  Engine_ObjectEventFaceEachOther(firstId, secondId, 0);
  Engine_ScriptWait(waitFrames);
}

/// Waits for the animation to change before the following pause.
static void VaultInteriors_PlayAnimationAndWait(s32 localId, s32 animation,
                                                s32 waitFrames) {
  Engine_ObjectEventSetAnimation(localId, animation, waitFrames);
  ObjectMotion_WaitForAnimationChange(localId);
  Engine_ScriptWait(waitFrames);
}

/// Begins Mind Read, chooses its target, and shows its opening effect.
static void VaultInteriors_StartMindRead(s32 casterId, s32 targetId) {
  Engine_PsynergyStart(ABILITY_MIND_READ, 1);
  Engine_PsynergySetTarget(casterId, targetId);
  Engine_PsynergyRaiseHands();
  Engine_PsynergyPlayEffect(1);
  Engine_TaskWait(1);
}

/// Finishes Mind Read and releases its scene effect objects.
static void VaultInteriors_EndMindRead(void) {
  Engine_PsynergyPlayEffect(2);
  Engine_PsynergyLowerHands();
  FieldEffect_End();
}

#include "common/vault_itemobj.h"

/// Finds the route node containing the object event's position on the
/// eight-pixel grid.
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

/// Checks whether an object event's destination or current position lies within
/// eight pixels.
static s32 VaultInteriors_IsObjectEventWithinEightPixels(s32 x, s32 z, s32 id) {
  struct ObjectEvent *objectEvent = Engine_ObjectEventLookup(id);
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
static s32 VaultInteriors_IsObjectEventWithinSixteenPixels(s32 x, s32 z,
                                                           s32 id) {
  struct ObjectEvent *objectEvent = Engine_ObjectEventLookup(id);
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
static s32 VaultInteriors_CheckStopOccupancy(struct VaultInteriorsStop *stop) {
  s32 x;
  s32 z;

  if (stop == NULL) {
    return 1;
  }
  x = (stop->gridX << 19) + PIXELS(72);
  z = (stop->gridZ << 19) + PIXELS(632);
  if (VaultInteriors_IsObjectEventWithinSixteenPixels(
          x, z, LOCALID_PARTY_LEADER) != 0 ||
      VaultInteriors_IsObjectEventWithinEightPixels(x, z, IVAN) != 0 ||
      VaultInteriors_IsObjectEventWithinEightPixels(
          x, z, LOCALID_SUSPECTED_LEADER) != 0 ||
      VaultInteriors_IsObjectEventWithinEightPixels(
          x, z, LOCALID_SUSPECTED_THIEF) != 0) {
    return -1;
  }
  return 0;
}

/// Sets the object event's destination to the route stop's cell center.
static void
VaultInteriors_MoveObjectEventToStop(struct ObjectEvent *objectEvent,
                                     struct VaultInteriorsStop *stop) {
  Engine_ObjectSetPosition(objectEvent, (stop->gridX << 19) + PIXELS(72), 0,
                           (stop->gridZ << 19) + PIXELS(632));
}

/// Steers Ivan and both suspects through the graph and detects a sustained
/// cornering.
///
/// Run each frame as a map callback from VaultInteriors_StartObjectEventStops
/// until EventScript_ReadCorneredSuspectsMinds removes it. Raises event 200
/// plus the blocked suspects (1 the leader, 2 the thief) after thirty frames
/// with at least one suspect blocked; a frame when both can move restarts the
/// wait.
static void VaultInteriors_UpdateObjectEventStops(void) {
  struct ObjectEvent *leader;
  struct ObjectEvent *objectEvent;
  struct EventWork *work;
  struct VaultInteriorsStop *entry;
  struct VaultInteriorsStop *destination;
  s32 dx;
  s32 dz;
  s32 blocked;
  u16 angle;
  s16 facing;
  s16 facingDifference;

  leader = Engine_ObjectEventLookup(LOCALID_PARTY_LEADER);
  blocked = 0;
  work = gFieldWork;
  objectEvent = Engine_ObjectEventLookup(IVAN);
  entry = VaultInteriors_FindStopAtPosition(&objectEvent->x.fixed);
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
    destination = VaultInteriors_SnapToNearestStop(entry, &facing);
    if (VaultInteriors_CheckStopOccupancy(destination) == 0) {
      VaultInteriors_MoveObjectEventToStop(objectEvent, destination);
      ObjectEvent_SetMode(objectEvent, 2);
    } else {
      ObjectEvent_SetMode(objectEvent, 1);
    }
  }

  // Each suspect tries a random turn, then reverses when that route is
  // occupied.
  objectEvent = Engine_ObjectEventLookup(LOCALID_SUSPECTED_LEADER);
  entry = VaultInteriors_FindStopAtPosition(&objectEvent->x.fixed);
  if (entry != NULL && objectEvent->targetX == OBJ_EVENT_NO_TARGET) {
    u32 turn = (u32)Engine_RandomNext() * 2 >> 16;
    s32 currentFacing;

    turn *= 0x60000000;
    currentFacing = objectEvent->facing;
    facing = ((turn - 0x30000000) >> 16) + currentFacing;
    destination = VaultInteriors_SnapToNearestStop(entry, &facing);
    if (VaultInteriors_CheckStopOccupancy(destination) != 0) {
      facing = objectEvent->facing + 0x8000;
      destination = VaultInteriors_SnapToNearestStop(entry, &facing);
      if (VaultInteriors_CheckStopOccupancy(destination) == 0) {
        Engine_ObjectEventSetAttachedEffect(LOCALID_SUSPECTED_LEADER,
                                            ATTACHED_EFFECT_ONCE);
        goto move_first;
      }
      ObjectEvent_SetMode(objectEvent, 4);
      blocked = 1;
    } else {
    move_first:
      VaultInteriors_MoveObjectEventToStop(objectEvent, destination);
      ObjectEvent_SetMode(objectEvent, 2);
    }
  }

  objectEvent = Engine_ObjectEventLookup(LOCALID_SUSPECTED_THIEF);
  entry = VaultInteriors_FindStopAtPosition(&objectEvent->x.fixed);
  if (entry != NULL && objectEvent->targetX == OBJ_EVENT_NO_TARGET) {
    u32 turn = (u32)Engine_RandomNext() * 3 >> 16;
    s32 currentFacing;

    turn *= 0x30000000;
    currentFacing = objectEvent->facing;
    facing = ((turn - 0x30000000) >> 16) + currentFacing;
    destination = VaultInteriors_SnapToNearestStop(entry, &facing);
    if (VaultInteriors_CheckStopOccupancy(destination) != 0) {
      facing = objectEvent->facing + 0x8000;
      destination = VaultInteriors_SnapToNearestStop(entry, &facing);
      if (VaultInteriors_CheckStopOccupancy(destination) == 0) {
        Engine_ObjectEventSetAttachedEffect(LOCALID_SUSPECTED_THIEF,
                                            ATTACHED_EFFECT_ONCE);
        goto move_second;
      }
      ObjectEvent_SetMode(objectEvent, 4);
      blocked += 2;
    } else {
    move_second:
      VaultInteriors_MoveObjectEventToStop(objectEvent, destination);
      ObjectEvent_SetMode(objectEvent, 2);
    }
  }
  // A failed route choice alone is not enough to end the chase.
  if (blocked != 0) {
    if (++gVaultInteriors_StopTimer > VAULT_INTERIORS_CORNERED_FRAMES - 1)
      work->raisedEvent = blocked + VAULT_INTERIORS_CORNERED_EVENT_BASE;
  } else {
    gVaultInteriors_StopTimer = blocked;
  }
}

/// Arranges the chase, constrains its camera, and starts the route callback.
///
/// Called by EventScript_StartCorneringSuspects. Starts
/// VaultInteriors_UpdateObjectEventStops and the chase music, MUS_009.
static void VaultInteriors_StartObjectEventStops(void) {
  Engine_ScriptStart();
  FieldCamera_MoveTo(PIXELS(168), -1, PIXELS(656), 1);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_SetSpeed(GARET, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_SetSpeed(IVAN, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 248, 696);
  ObjectEvent_SetPosition(GARET, PIXELS(248), PIXELS(696));
  ObjectEvent_SetPosition(IVAN, PIXELS(248), PIXELS(696));
  ObjectEvent_WalkTo(LOCALID_PARTY_LEADER, 200, 696);
  ObjectEvent_WalkTo(GARET, 248, 712);
  ObjectEvent_WalkToAndWait(IVAN, 232, 696);
  Engine_ObjectEventWaitForMove(GARET);
  ObjectEvent_FaceDirection(GARET, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(IVAN, FACING_WEST, 0);
  Engine_ObjectEventWaitForMove(LOCALID_PARTY_LEADER);
  Engine_ObjectEventSetAnimation(GARET, 12);
  VaultInteriors_ResetSuspectRoutes();
  Field_SetViewBounds(PIXELS(48), PIXELS(576), PIXELS(288), PIXELS(736));
  ObjectEvent_SetSpeed(GARET, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_SetSpeed(IVAN, PIXELS(3) / 4, PIXELS(3) / 8);
  ObjectEvent_SetSpeed(LOCALID_SUSPECTED_LEADER, PIXELS(1), PIXELS(6) / 5);
  ObjectEvent_SetSpeed(LOCALID_SUSPECTED_THIEF, PIXELS(3) / 2, PIXELS(3) / 2);
  gVaultInteriors_StopTimer = 0;
  FieldScheduler_AddOrUpdateCallback(VaultInteriors_UpdateObjectEventStops,
                                     0xc94);
  // Flag 0x1ff holds nothing: no table, script or function sets or reads it,
  // and render probe flag-reads sees no read of it, so clearing it changes
  // nothing.
  GameFlag_Clear(0x1ff);
  Engine_ScriptEnd();
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

s32 gVaultInteriors_SecondThiefEntryScript[] = {
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

s32 gVaultInteriors_VaultScriptA[] = {
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

s32 gVaultInteriors_VaultScriptB[] = {
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

s32 gVaultInteriors_VaultScriptC[] = {
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

s32 gVaultInteriors_VaultScriptD[] = {
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

s32 gVaultInteriors_VaultScriptE[] = {
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
// (EventScript_AdvanceSuspectRoutes): the leader's 1 to 3 the heads and 4 to 6 the
// feet, west to east; the thief's 1 and 2 beside the west and east beds, 3
// and 4 the south-west and south-east corners.
s32 gVaultInteriors_SuspectToWestBedHead[] = {
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(104), 0, PIXELS(632)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_END,
};

s32 gVaultInteriors_SuspectRoundToWestBedHead[] = {
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

s32 gVaultInteriors_SuspectToMiddleBedHead[] = {
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(168), 0, PIXELS(632)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_END,
};

s32 gVaultInteriors_SuspectToEastBedHead[] = {
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(232), 0, PIXELS(632)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_END,
};

s32 gVaultInteriors_SuspectRoundToEastBedHead[] = {
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

s32 gVaultInteriors_SuspectToWestBedFoot[] = {
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(104), 0, PIXELS(696)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_END,
};

s32 gVaultInteriors_SuspectRoundToWestBedFoot[] = {
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

s32 gVaultInteriors_SuspectToMiddleBedFoot[] = {
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(168), 0, PIXELS(696)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_END,
};

s32 gVaultInteriors_SuspectToEastBedFoot[] = {
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(232), 0, PIXELS(696)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_END,
};

s32 gVaultInteriors_SuspectRoundToEastBedFoot[] = {
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

s32 gVaultInteriors_SuspectAcrossToWestOfBeds[] = {
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(168), 0, PIXELS(648)),
    SC_SYNC,
    SC_MOVE(PIXELS(72), 0, PIXELS(648)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_END,
};

s32 gVaultInteriors_SuspectToWestOfBeds[] = {
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(72), 0, PIXELS(648)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_END,
};

s32 gVaultInteriors_SuspectAcrossToEastOfBeds[] = {
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(168), 0, PIXELS(648)),
    SC_SYNC,
    SC_MOVE(PIXELS(264), 0, PIXELS(648)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_END,
};

s32 gVaultInteriors_SuspectToEastOfBeds[] = {
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(264), 0, PIXELS(648)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_END,
};

s32 gVaultInteriors_SuspectToSouthWestCorner[] = {
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(72), 0, PIXELS(696)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_END,
};

s32 gVaultInteriors_SuspectAlongToSouthWestCorner[] = {
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(120), 0, PIXELS(696)),
    SC_SYNC,
    SC_MOVE(PIXELS(72), 0, PIXELS(696)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_END,
};

s32 gVaultInteriors_SuspectToSouthEastCorner[] = {
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(264), 0, PIXELS(696)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_END,
};

s32 gVaultInteriors_SuspectAlongToSouthEastCorner[] = {
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(168), 0, PIXELS(696)),
    SC_SYNC,
    SC_MOVE(PIXELS(264), 0, PIXELS(696)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_END,
};

struct MapWarpDest gVaultInteriors_WarpDests[] = {
    { WARP_DEST_DEFAULT, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
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
    { MAP_TABLE_END },
};

struct MapWarpRegion gVaultInteriors_Regions[] = {
    {639, 33, 407, 657, 51, 425, CONDITION_ALWAYS, VAULT_INTERIORS_TO_VAULT_WEST},
    {MAP_TABLE_END},
};

u32 gVaultInteriors_Warps[] = {
    MAP_WARPS(VaultInteriors),
    MAP_WARP(VAULT_INTERIORS_TO_VAULT_EAST_SOUTHEAST, Vault, VAULT_FROM_VAULT_INTERIORS_EAST_SOUTHEAST),
    MAP_WARP(VAULT_INTERIORS_TO_VAULT_SOUTHEAST, Vault, VAULT_FROM_VAULT_INTERIORS_SOUTHEAST),
    MAP_WARP(VAULT_INTERIORS_TO_VAULT_SOUTHWEST, Vault, VAULT_FROM_VAULT_INTERIORS_SOUTHWEST),
    MAP_WARP(VAULT_INTERIORS_TO_VAULT_EAST, Vault, VAULT_FROM_VAULT_INTERIORS_EAST),
    MAP_WARP(VAULT_INTERIORS_TO_VAULT_NORTH_NORTHEAST, Vault, VAULT_FROM_VAULT_INTERIORS_NORTH_NORTHEAST),
    MAP_WARP(VAULT_INTERIORS_TO_VAULT_NORTHWEST, Vault, VAULT_FROM_VAULT_INTERIORS_NORTHWEST),
    MAP_WARP(VAULT_INTERIORS_INN_UPSTAIRS_TO_INN, VaultInteriors, VAULT_INTERIORS_INN_FROM_INN_UPSTAIRS),
    MAP_WARP(VAULT_INTERIORS_INN_TO_INN_UPSTAIRS, VaultInteriors, VAULT_INTERIORS_INN_UPSTAIRS_FROM_INN),
    MAP_WARP(VAULT_INTERIORS_TO_VAULT_WEST, Vault, VAULT_FROM_VAULT_INTERIORS_WEST),
    MAP_WARP(VAULT_INTERIORS_LOFT_TO_CRATE_ROOM, VaultInteriors, VAULT_INTERIORS_CRATE_ROOM_FROM_LOFT),
    MAP_WARP(VAULT_INTERIORS_CRATE_ROOM_TO_LOFT, VaultInteriors, VAULT_INTERIORS_LOFT_FROM_CRATE_ROOM),
    MAP_WARP(VAULT_INTERIORS_TO_SANCTUM, Sanctum, SANCTUM_FROM_VAULT_INTERIORS),
    MAP_WARPS_END,
};

struct ObjectEventTemplate gVaultInteriors_ObjectEventTemplates[] = {
    { GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(8), 0, PIXELS(8), FACING_NORTH, TALK_FACING_PARTY, 0 },
    { IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
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

struct ObjectEventTemplate gVaultInteriors_LoftObjectEventTemplates[] = {
    { GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(8), 0, PIXELS(8), FACING_NORTH, TALK_FACING_PARTY, 0 },
    { IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
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

struct MapEvent gVaultInteriors_Events[] = {
    { MAP_EVENT_WARP, 1, CONDITION_ALWAYS, VAULT_INTERIORS_TO_VAULT_EAST_SOUTHEAST },
    { MAP_EVENT_WARP, 2, CONDITION_ALWAYS, VAULT_INTERIORS_TO_VAULT_SOUTHEAST },
    { MAP_EVENT_WARP, 3, CONDITION_ALWAYS, VAULT_INTERIORS_TO_VAULT_SOUTHWEST },
    { MAP_EVENT_WARP, 4, CONDITION_ALWAYS, VAULT_INTERIORS_TO_VAULT_EAST },
    { MAP_EVENT_WARP, 5, CONDITION_ALWAYS, VAULT_INTERIORS_TO_VAULT_NORTH_NORTHEAST },
    { MAP_EVENT_WARP, 6, CONDITION_ALWAYS, VAULT_INTERIORS_TO_VAULT_NORTHWEST },
    { MAP_EVENT_COORD, VAULT_INTERIORS_CELL_GUESTS_ROOM_STAIRS, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveGuestsRoom) },
    { MAP_EVENT_WARP, 8, CONDITION_ALWAYS, VAULT_INTERIORS_INN_TO_INN_UPSTAIRS },
    { MAP_EVENT_COORD, VAULT_INTERIORS_CELL_GUESTS_ROOM, IF_FLAG_SET(FLAG_VAULT_IVAN_JOINED_ROD_SEARCH), EVENT_SCRIPT(EventScript_ApproachSuspects) },
    { MAP_EVENT_COORD, VAULT_INTERIORS_CELL_FIRST_SUSPECT_ROUTE, IF_FLAG_SET(FLAG_SUSPECTS_FLEEING), EVENT_SCRIPT(EventScript_AdvanceSuspectRoutes) },
    { MAP_EVENT_COORD, VAULT_INTERIORS_CELL_FIRST_SUSPECT_ROUTE + 1, IF_FLAG_SET(FLAG_SUSPECTS_FLEEING), EVENT_SCRIPT(EventScript_AdvanceSuspectRoutes) },
    { MAP_EVENT_COORD, VAULT_INTERIORS_CELL_FIRST_SUSPECT_ROUTE + 2, IF_FLAG_SET(FLAG_SUSPECTS_FLEEING), EVENT_SCRIPT(EventScript_AdvanceSuspectRoutes) },
    { MAP_EVENT_COORD, VAULT_INTERIORS_CELL_FIRST_SUSPECT_ROUTE + 3, IF_FLAG_SET(FLAG_SUSPECTS_FLEEING), EVENT_SCRIPT(EventScript_AdvanceSuspectRoutes) },
    { MAP_EVENT_COORD, VAULT_INTERIORS_CELL_FIRST_SUSPECT_ROUTE + 4, IF_FLAG_SET(FLAG_SUSPECTS_FLEEING), EVENT_SCRIPT(EventScript_AdvanceSuspectRoutes) },
    { MAP_EVENT_COORD, VAULT_INTERIORS_CELL_FIRST_SUSPECT_ROUTE + 5, IF_FLAG_SET(FLAG_SUSPECTS_FLEEING), EVENT_SCRIPT(EventScript_AdvanceSuspectRoutes) },
    { MAP_EVENT_COORD, VAULT_INTERIORS_CELL_FIRST_SUSPECT_ROUTE + 6, IF_FLAG_SET(FLAG_SUSPECTS_FLEEING), EVENT_SCRIPT(EventScript_AdvanceSuspectRoutes) },
    { MAP_EVENT_COORD, VAULT_INTERIORS_CELL_LAST_SUSPECT_ROUTE, IF_FLAG_SET(FLAG_SUSPECTS_FLEEING), EVENT_SCRIPT(EventScript_AdvanceSuspectRoutes) },
    { MAP_EVENT_COORD, VAULT_INTERIORS_CELL_GUESTS_ROOM_DOOR, IF_FLAG_SET(FLAG_SUSPECTS_FLEEING), EVENT_SCRIPT(EventScript_DiscussSuspiciousGuests) },
    { MAP_EVENT_COORD, VAULT_INTERIORS_CELL_CORNERING_START, IF_FLAG_SET(FLAG_VAULT_SUSPECTS_APPROACHED), EVENT_SCRIPT(EventScript_StartCorneringSuspects) },
    { MAP_EVENT_RAISED, VAULT_INTERIORS_LEADER_CORNERED, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_ReadCorneredSuspectsMinds) },
    { MAP_EVENT_RAISED, VAULT_INTERIORS_THIEF_CORNERED, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_ReadCorneredSuspectsMinds) },
    { MAP_EVENT_RAISED, VAULT_INTERIORS_BOTH_CORNERED, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_ReadCorneredSuspectsMinds) },
    { MAP_EVENT_RAISED, FIELD_RAISED_EVENT_MENUS, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_GaretRefusesToLeave) },
    { MAP_EVENT_COORD, VAULT_INTERIORS_CELL_LOFT_CLIMB, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_ClimbToLoft) },
    { COORD_EVENT_WALKING, VAULT_INTERIORS_CELL_ROOF_GAP, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_JumpAcrossRoofGap) },
    { MAP_EVENT_TALK, IVAN, IF_FLAG_SET(FLAG_MAP_TEMP_50), 0 },
    { MAP_EVENT_TALK, IVAN, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_MeetIvan) },
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
    { MAP_EVENT_TALK, LOCALID_SUSPECTED_LEADER, IF_FLAG_SET(FLAG_MAP_TEMP_50), 0 },
    { MAP_EVENT_TALK, LOCALID_SUSPECTED_THIEF, IF_FLAG_SET(FLAG_MAP_TEMP_50), 0 },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_SUSPECTED_LEADER, IF_FLAG_SET(FLAG_MAP_TEMP_50), 0 },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_SUSPECTED_THIEF, IF_FLAG_SET(FLAG_MAP_TEMP_50), 0 },
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
    { BG_EVENT(BG_EVENT_SILENT), VAULT_INTERIORS_CELL_INN_BARREL, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_SearchEmptyBarrel) },
    { BG_EVENT(BG_EVENT_JAR), VAULT_INTERIORS_CELL_JAR, FLAG_VAULT_INTERIORS_JAR_COINS, FIND_COINS(7) },
    { BG_EVENT(BG_EVENT_BARREL), VAULT_INTERIORS_CELL_FIRST_BARREL, FLAG_VAULT_INTERIORS_BARREL_COINS, FIND_COINS(4) },
    { BG_EVENT(BG_EVENT_BARREL), VAULT_INTERIORS_CELL_SECOND_BARREL, FLAG_VAULT_INTERIORS_BARREL_SLEEP_BOMB, FIND_ITEM(ITEM_SLEEP_BOMB) },
    { BG_EVENT(BG_EVENT_WOODEN_BOX), VAULT_INTERIORS_CELL_WOODEN_BOX, FLAG_VAULT_INTERIORS_BOX_MINT, FIND_ITEM(ITEM_MINT) },
    { BG_EVENT(BG_EVENT_BOOKCASE) | MAP_EVENT_FLAG_FACING | FACING_NORTH, VAULT_INTERIORS_CELL_HISTORY_BOOKCASE, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgVaultTheresABookEntitledHistoryOf)) },
    { BG_EVENT(BG_EVENT_BOOKCASE), VAULT_INTERIORS_CELL_GEMS_BOOKCASE, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgVaultTheresABookEntitledGemsOf)) },
    { MAP_EVENTS_END },
};

struct MapEvent gVaultInteriors_LoftEvents[] = {
    { MAP_EVENT_COORD, VAULT_INTERIORS_CELL_LOFT_DESCENT, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveLoft) },
    { MAP_EVENT_TALK, LOCALID_TIED_UP_VILLAGER, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_FreeRooferAndConfrontThieves) },
    { MAP_EVENT_TALK, LOCALID_FIRST_CHEST, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_SearchEmptyChest) },
    { MAP_EVENT_TALK, LOCALID_SECOND_CHEST, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_SearchEmptyChest) },
    { MAP_EVENT_TALK, LOCALID_THIRD_CHEST, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_SearchEmptyChest) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_TIED_UP_VILLAGER, CONDITION_ALWAYS, MSGNO(MsgVaultAhhHelpMe) },
    { MAP_EVENTS_END },
};

struct MapEvent gVaultInteriors_EventsAfterThieves[] = {
    { BG_EVENT(BG_EVENT_SILENT), VAULT_INTERIORS_CELL_INN_BARREL, CONDITION_ALWAYS, EVENT_SCRIPT(Vault_FindBone) },
    { MAP_EVENT_WARP, 1, CONDITION_ALWAYS, VAULT_INTERIORS_TO_VAULT_EAST_SOUTHEAST },
    { MAP_EVENT_WARP, 2, CONDITION_ALWAYS, VAULT_INTERIORS_TO_VAULT_SOUTHEAST },
    { MAP_EVENT_WARP, 3, CONDITION_ALWAYS, VAULT_INTERIORS_TO_VAULT_SOUTHWEST },
    { MAP_EVENT_WARP, 4, CONDITION_ALWAYS, VAULT_INTERIORS_TO_VAULT_EAST },
    { MAP_EVENT_WARP, 5, CONDITION_ALWAYS, VAULT_INTERIORS_TO_VAULT_NORTH_NORTHEAST },
    { MAP_EVENT_WARP, 6, CONDITION_ALWAYS, VAULT_INTERIORS_TO_VAULT_NORTHWEST },
    { MAP_EVENT_WARP, 7, CONDITION_ALWAYS, VAULT_INTERIORS_INN_UPSTAIRS_TO_INN },
    { MAP_EVENT_WARP, 8, CONDITION_ALWAYS, VAULT_INTERIORS_INN_TO_INN_UPSTAIRS },
    { MAP_EVENT_COORD, VAULT_INTERIORS_CELL_LOFT_CLIMB, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_ClimbToLoft) },
    { MAP_EVENT_COORD, VAULT_INTERIORS_CELL_LOFT_DESCENT, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveLoft) },
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
    { MAP_EVENT_TALK, LOCALID_GIRL_AT_THE_INN, FLAG_MAP_TEMP_50, EVENT_SCRIPT(EventScript_OfferBones) },
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
    { BG_EVENT(BG_EVENT_JAR), VAULT_INTERIORS_CELL_JAR, FLAG_VAULT_INTERIORS_JAR_COINS, FIND_COINS(7) },
    { BG_EVENT(BG_EVENT_BARREL), VAULT_INTERIORS_CELL_FIRST_BARREL, FLAG_VAULT_INTERIORS_BARREL_COINS, FIND_COINS(4) },
    { BG_EVENT(BG_EVENT_BARREL), VAULT_INTERIORS_CELL_SECOND_BARREL, FLAG_VAULT_INTERIORS_BARREL_SLEEP_BOMB, FIND_ITEM(ITEM_SLEEP_BOMB) },
    { BG_EVENT(BG_EVENT_WOODEN_BOX), VAULT_INTERIORS_CELL_WOODEN_BOX, FLAG_VAULT_INTERIORS_BOX_MINT, FIND_ITEM(ITEM_MINT) },
    { BG_EVENT(BG_EVENT_BOOKCASE) | MAP_EVENT_FLAG_FACING | FACING_NORTH, VAULT_INTERIORS_CELL_HISTORY_BOOKCASE, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgVaultTheresABookEntitledHistoryOf)) },
    { BG_EVENT(BG_EVENT_BOOKCASE), VAULT_INTERIORS_CELL_GEMS_BOOKCASE, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgVaultTheresABookEntitledGemsOf)) },
    { MAP_EVENTS_END },
};
// clang-format on

s32 *gVaultInteriors_StepActions[] = {
    gVaultInteriors_SuspectToMiddleBedHead,
    gVaultInteriors_SuspectToEastBedHead,
    gVaultInteriors_SuspectRoundToEastBedFoot,
    gVaultInteriors_SuspectToMiddleBedFoot,
    gVaultInteriors_SuspectToWestBedFoot,
    gVaultInteriors_SuspectRoundToWestBedHead,
};

s32 *gVaultInteriors_IdleActions[] = {
    gVaultInteriors_SuspectRoundToWestBedFoot,
    gVaultInteriors_SuspectToWestBedHead,
    gVaultInteriors_SuspectToMiddleBedHead,
    gVaultInteriors_SuspectRoundToEastBedHead,
    gVaultInteriors_SuspectToEastBedFoot,
    gVaultInteriors_SuspectToMiddleBedFoot,
};

s32 *gVaultInteriors_ResponseScripts[] = {
    gVaultInteriors_SuspectAcrossToEastOfBeds,
    gVaultInteriors_SuspectToSouthEastCorner,
    gVaultInteriors_SuspectAlongToSouthWestCorner,
    gVaultInteriors_SuspectToWestOfBeds,
    gVaultInteriors_SuspectToSouthWestCorner,
    gVaultInteriors_SuspectAcrossToWestOfBeds,
    gVaultInteriors_SuspectToEastOfBeds,
    gVaultInteriors_SuspectAlongToSouthEastCorner,
};

u16 gVaultInteriors_StopTimer;
