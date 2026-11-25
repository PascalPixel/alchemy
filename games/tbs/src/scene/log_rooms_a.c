/// @file
/// Log rooms A (SceneId_LogRoomsA), a rooms map drawn in logs, holds only
/// Vault's rooms while Hammet's Shaman's Rod is missing and just after it is
/// found: the insides of the town's houses, shops and inn, the inn's attic and
/// its loft.
///
/// Every room is on one map, each an entrance with a camera box of its
/// own. Vault's doors (vault.c) lead to the entrances named for them:
/// - the gem owner's house, nearest the town's lower gate, where a big pale
///   blue gem stands on the round table;
/// - the house of a burly grey-haired man and a small boy, in the middle of
///   town;
/// - the inn, with the guests' room and its two beds upstairs, where the
///   three strangers stay;
/// - the item shop, which is also the home of Grandpa and his wife;
/// - the weapon and armor shop;
/// - the mayor's house, where Ivan is.
///
/// The ladder outside up to the inn's broken roof (LOG_ROOMS_A_FROM_VAULT_WEST)
/// comes into the attic, a small room with a big crate and a dark hole in the
/// floor. Behind the crate is the way into the loft, where the thieves hid
/// the stolen goods.
///
/// The loft has actor placements and events of its own
/// (Scene_GetActorPlacements and Scene_GetTriggers).
/// FLAG_VAULT_THIEVES_JAILED chooses the other rooms' tables.
///
/// The story flags drive the rooms in order:
/// - At the mayor's, talking to Ivan plays LogRoomsA_MeetIvan. He reads
///   Isaac's mind (FLAG_VAULT_MET_IVAN) and asks for help with the rod. On a
///   yes he joins the party (LogRoomsA_IvanJoinsRodSearch,
///   FLAG_VAULT_IVAN_JOINED_ROD_SEARCH).
/// - In the guests' room upstairs at the inn, the two suspects see Ivan with
///   the party and run from him round the beds
///   (LogRoomsA_ApproachSuspects). Stepping back out of the room, the party
///   plans to surround one of them (LogRoomsA_DiscussSuspiciousGuests).
///   Going back in starts the chase (LogRoomsA_StartCorneringSuspects): Garet
///   spreads his arms and Ivan follows, and the suspects dodge between spots
///   round the beds (gLogRoomsA_Stops). Once one of them has been boxed in
///   for half a second, Ivan reads both minds
///   (LogRoomsA_ReadCorneredSuspectsMinds, FLAG_VAULT_SUSPECTS_MIND_READ).
///   During the chase the menu only gets Garet's "We don't have time for
///   that" (LogRoomsA_GaretHasNoTimeForMenu).
/// - In the attic, Move pushes the crate aside (LogRoomsA_CheckCrateMoved,
///   FLAG_VAULT_CRATE_MOVED), and the way behind it leads into the loft
///   (LogRoomsA_EnterLoft). Cutting free the young man tied up there brings
///   out the three thieves, who attack (LogRoomsA_FreeVillagerAndFightThieves,
///   FLAG_VAULT_THIEVES_ATTACKED). After a loss the thieves gloat over the
///   fallen party, which comes to in the Vault Sanctum
///   (LogRoomsA_LoseToThieves). Coming back up replays the challenge
///   (LogRoomsA_FightThievesAgain). After a win (LogRoomsA_DefeatThieves,
///   LogRoomsA_ReturnStolenGoods) the mayor comes up and has the thieves
///   taken to jail (FLAG_VAULT_THIEVES_JAILED), the chests give back the
///   stolen goods and the Shaman's Rod, and Ivan leaves the party to rescue
///   Hammet from Lunpa.
/// - Once the thieves are jailed, the mayor gives the promised Water of Life
///   (FLAG_VAULT_WATER_OF_LIFE_RECEIVED) and the girl at the inn offers
///   Bones (FLAG_VAULT_BONE_RECEIVED), with more on the barrel beside her
///   (Vault_FindBone) for the dog in town.
///
/// The shops and the inn open across their counters when Isaac faces north.
/// Spoken to from anywhere else, their keepers only talk.
///
/// A jar, two barrels and a wooden box hold coins, a Sleep Bomb and a Mint,
/// each found once, and two bookcases can be read.
///
/// The cells the events answer are enum LogRoomsACellBehavior
/// (log_rooms_a.h).
///
/// The three cells of the guests' room doorway, (14, 44) to (16, 44), change
/// what they do as the story goes on. At first, stepping on them sets off
/// LogRoomsA_ApproachSuspects. After it they are rewritten from copies kept
/// lower on the map, so that stepping on them on the way out plays
/// LogRoomsA_DiscussSuspiciousGuests, and after that, on the way back in,
/// LogRoomsA_StartCorneringSuspects. Every later arrival with
/// FLAG_VAULT_SUSPECTS_APPROACHED set rewrites them again.

#include "scene/log_rooms_a.h"
#include "ability_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "macros.h"
#include "scene_ids.h"
#include "static_assert.h"
#include "types.h"
#include "battle/battle.h"
#include "battle/battle_side.h"
#include "debug/debug.h"
#include "field/actor_script.h"
#include "field/event.h"
#include "field/event_commands.h"
#include "field/event_inline.h"
#include "field/field.h"
#include "field/field_sequence.h"
#include "field/map_grid.h"
#include "field/scene.h"
#include "field/scene_tables.h"
#include "field/sprite.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "menu/menu.h"
#include "scene/import.h"
#include "scene/vault_item_icon.h"
#include "sound/sound.h"
#include "sound/sound_driver_symbols.h"
#include "system/main_routine.h"
#include "system/main_routine_state.h"
#include "system/system.h"
#include "text/msg_ids.h"

/// The lines of Ivan's joining (LogRoomsA_IvanJoinsRodSearch), counted from
/// MsgVaultThankHelpBelieve, where the event moves on to a new one.
enum IvanJoiningMessage {
  IVAN_JOINING_ISAAC_CLASPS_HANDS = 5,
  IVAN_JOINING_GARET_FEELS_MIND_READ = 6,
  IVAN_JOINING_GARET_CLASPS_HANDS = 13,
  IVAN_JOINING_GARET_READS_MIND = 14,
  IVAN_JOINING_IVAN_WILL_READ_TOWN = 22
};

/// A link's stopIndex where a stop has fewer than three links.
enum { LOG_STOP_NONE = 255 };

/// The chase's stops (gLogRoomsA_Stops), LOG_STOP_COUNT of them, are placed
/// in 8-pixel half cells counted from (LOG_STOPS_ORIGIN_X, LOG_STOPS_ORIGIN_Z);
/// each covers two by two of them, around its middle.
#define LOG_STOP_COUNT 37
#define LOG_STOPS_ORIGIN_X 64
#define LOG_STOPS_ORIGIN_Z 624
/// The 16.16 middle of a stop.
#define LOG_STOP_X(stop)                                                       \
  (FIELD_HALF_CELL_ORIGIN((stop)->gridX) + toFIXED(LOG_STOPS_ORIGIN_X + 8))
#define LOG_STOP_Z(stop)                                                       \
  (FIELD_HALF_CELL_ORIGIN((stop)->gridZ) + toFIXED(LOG_STOPS_ORIGIN_Z + 8))

/// Where Move leaves the crate aside, its map column, which opens the way up
/// to the inn's loft.
#define CRATE_MOVED_COLUMN 42

/// One of a stop's links: the stop it leads to and the heading toward it.
struct LogRoomsAStopConnection {
  u8 stopIndex; ///< the stop it leads to, or LOG_STOP_NONE
  u8 facing;    ///< the heading toward it, a facing's high byte
  u8 padding[2];
};

/// A stop on the route network, two half cells square, with its links to
/// others.
struct LogRoomsAStop {
  u8 gridX; ///< half cells from LOG_STOPS_ORIGIN_X
  u8 gridZ; ///< half cells from LOG_STOPS_ORIGIN_Z
  u8 padding[2];
  struct LogRoomsAStopConnection connections[3];
};

STATIC_ASSERT(sizeof(struct LogRoomsAStopConnection) == 4);
STATIC_ASSERT(sizeof(struct LogRoomsAStop) == 16);

extern struct LogRoomsAStop gLogRoomsA_Stops[LOG_STOP_COUNT];
extern s32 gLogRoomsA_BeatenThiefGasps[];
extern s32 gLogRoomsA_FirstThiefEntryScript[];
extern s32 gLogRoomsA_SecondThiefEntryScript[];
extern s32 gLogRoomsA_VillagerLeavesLoft[];
extern s32 gLogRoomsA_TiedThiefLeavesLoft[];
extern s32 gLogRoomsA_ToSecondChest[];
extern s32 gLogRoomsA_ToFirstChest[];
extern s32 gLogRoomsA_BackFromChests[];
extern s32 gLogRoomsA_SuspectToWestBedHead[];
extern s32 gLogRoomsA_SuspectRoundToWestBedHead[];
extern s32 gLogRoomsA_SuspectToMiddleBedHead[];
extern s32 gLogRoomsA_SuspectToEastBedHead[];
extern s32 gLogRoomsA_SuspectRoundToEastBedHead[];
extern s32 gLogRoomsA_SuspectToWestBedFoot[];
extern s32 gLogRoomsA_SuspectRoundToWestBedFoot[];
extern s32 gLogRoomsA_SuspectToMiddleBedFoot[];
extern s32 gLogRoomsA_SuspectToEastBedFoot[];
extern s32 gLogRoomsA_SuspectRoundToEastBedFoot[];
extern s32 gLogRoomsA_SuspectAcrossToWestOfBeds[];
extern s32 gLogRoomsA_SuspectToWestOfBeds[];
extern s32 gLogRoomsA_SuspectAcrossToEastOfBeds[];
extern s32 gLogRoomsA_SuspectToEastOfBeds[];
extern s32 gLogRoomsA_SuspectToSouthWestCorner[];
extern s32 gLogRoomsA_SuspectAlongToSouthWestCorner[];
extern s32 gLogRoomsA_SuspectToSouthEastCorner[];
extern s32 gLogRoomsA_SuspectAlongToSouthEastCorner[];
extern struct SceneEntrance gLogRoomsA_Entrances[];
extern struct SceneExitRegion gLogRoomsA_Regions[];
extern u32 gLogRoomsA_Exits[];
extern struct ActorPlacement gLogRoomsA_ActorPlacements[];
extern struct ActorPlacement gLogRoomsA_LoftActorPlacements[];
extern struct Trigger gLogRoomsA_Triggers[];
extern struct Trigger gLogRoomsA_LoftTriggers[];
extern struct Trigger gLogRoomsA_TriggersAfterThieves[];
extern s32 *gLogRoomsA_LeaderRunsOn[];
extern s32 *gLogRoomsA_LeaderRunsBack[];
extern s32 *gLogRoomsA_ThiefRuns[];

/// Where the mayor stands once the thieves are jailed: a step south of his
/// place by the east wall, where the French, Spanish and Italian editions
/// leave him.
#if TBS_PATCH_3
#define MAYOR_AFTER_THIEVES_Z 451
#else
#define MAYOR_AFTER_THIEVES_Z 464
#endif

/// The spots the suspected leader runs between in the guests' room, kept in his
/// timer: the three beds' heads, west to east, then their feet, east to west
/// (gLogRoomsA_LeaderRunsOn runs him from each to the next, and on round).
enum LogRoomsALeaderSpot {
  LEADER_SPOT_WEST_BED_HEAD,
  LEADER_SPOT_MIDDLE_BED_HEAD,
  LEADER_SPOT_EAST_BED_HEAD,
  LEADER_SPOT_EAST_BED_FOOT,
  LEADER_SPOT_MIDDLE_BED_FOOT,
  LEADER_SPOT_WEST_BED_FOOT
};

/// The spots the suspected thief runs between, kept in his timer: west and east
/// of the beds and the south corners, which gLogRoomsA_ThiefRuns runs him
/// between, and a spot on the south wall, (120, 696), from which he runs to
/// either corner.
enum LogRoomsAThiefSpot {
  THIEF_SPOT_WEST_OF_BEDS,
  THIEF_SPOT_EAST_OF_BEDS,
  THIEF_SPOT_SOUTH_EAST_CORNER,
  THIEF_SPOT_SOUTH_WEST_CORNER,
  THIEF_SPOT_SOUTH_WALL
};

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
/// gLogRoomsA_ActorPlacements' rows from ACTOR_FIRST_PLACED.
/// Garet's and Ivan's rows take their own slots.
enum {
  /// A young man, to whose house a sparkling rock was blown from the volcano.
  ACTOR_GEM_OWNER = 8,
  /// A woman with fair hair.
  ACTOR_GEM_OWNERS_WIFE = 9,
  /// A burly grey-haired man who distrusts the man next door. "My neighbor
  /// creeps me out."
  ACTOR_VILLAGER_SUSPECTING_THE_NEIGHBOR = 10,
  /// A small boy who asks about travel.
  ACTOR_BOY_ASKING_ABOUT_TRAVEL = 11,
  /// An old man with a cane who wonders what was stolen.
  ACTOR_GRANDPA = 12,
  /// An old woman, who says Grandpa keeps wondering if anything was stolen.
  ACTOR_GRANDPAS_WIFE = 13,
  /// A small boy. "I could help Ivan if I were just a little older."
  ACTOR_BOY_WISHING_TO_BE_OLDER = 14,
  /// A little girl who tells the party Ivan's secret once it helps him.
  ACTOR_GIRL_TELLING_A_SECRET = 15,
  /// Ivan's host, an old man with white hair.
  ACTOR_MAYOR = 16,
  /// A woman in a green dress.
  ACTOR_MAYORS_WIFE = 17,
  /// The girl at the inn, as the Bone's description calls her, a young woman
  /// with fair hair. "I have a lot of leftover bones from dinner."
  ACTOR_GIRL_AT_THE_INN = 18,
  ACTOR_WEAPON_MERCHANT = 19,
  ACTOR_ARMOR_MERCHANT = 20,
  ACTOR_ITEM_MERCHANT = 21,
  /// A burly grey-haired man who works at the inn.
  ACTOR_INN_STAFF = 22,
  /// A woman in an apron behind the inn's counter.
  ACTOR_INNKEEPER = 23,
  /// The two guests at the inn the town suspects: the thieves' leader and one
  /// of his men.
  ACTOR_SUSPECTED_LEADER = 24,
  ACTOR_SUSPECTED_THIEF = 25,
  /// Pushed aside to open a passage.
  ACTOR_CRATE = 26
};

/// Area flags of Vault's area (0x300 to 0x3ff, kept while the
/// party stays in Vault and its buildings).
enum {
  /// Set while the two suspects run from the party, until the party steps
  /// aside to talk it over.
  FLAG_SUSPECTS_FLEEING = FLAG_AREA_TEMP_00
};

// The scene flag the inn keeps as FLAG_VAULT_LEADER_BESIDE_BONE
// (shared/vault_leader_beside_bone_flag.c) once the thieves are jailed means
// something else in the guests' room:
/// The party is cornering the two suspects in the guests' room, so talking to
/// Ivan or the suspects does nothing.
#define FLAG_VAULT_CORNERING_SUSPECTS FLAG_SCENE_TEMP_50

/// The inn's loft (LOG_ROOMS_A_LOFT_FROM_CRATE_ROOM and the two after the
/// battle), where the thieves hid the stolen goods, numbering
/// gLogRoomsA_LoftActorPlacements' rows from ACTOR_FIRST_PLACED.
enum {
  /// vault.c's ACTOR_YOUNG_MAN_FIXING_THE_ROOF, tied up while fixing the
  /// roof.
  ACTOR_TIED_UP_VILLAGER = 8,
  ACTOR_LOFT_MAYOR = 9,
  /// Speaks first and bargains for all three ("See, we're not so bad. How
  /// about it? Will you let us go?").
  ACTOR_THIEF_LEADER = 10,
  ACTOR_FIRST_THIEF = 11,
  ACTOR_SECOND_THIEF = 12,
  /// A strongman who comes up with the mayor and helps tie the thieves up.
  ACTOR_LOFT_ESCORT = 13,
  /// A strongman who comes up with the mayor and opens the chests.
  ACTOR_STRONGMAN_OPENING_THE_CHESTS = 14,
  ACTOR_FIRST_CHEST = 15,
  ACTOR_SECOND_CHEST = 16,
  ACTOR_THIRD_CHEST = 17,
  /// Rises from the first chest.
  ACTOR_FOUND_URN = 18,
  /// Rises from the second chest, stolen from the Sanctum.
  ACTOR_FOUND_STATUE = 19
};

/// Animations of the loft's sprites beyond enum ActorAnimation, as their
/// animation tables and frames under graphics/sprite draw them.
enum LogRoomsAAnimation {
  /// Garet seen from behind, his arms spread wide (garet.png).
  ANIM_GARET_ARMS_SPREAD = 12,
  /// A thief standing bound, a rope wound round his arms
  /// (man_with_long_dark_hair.png,
  /// person_in_purple.png).
  ANIM_THIEF_TIED_UP = 5,
  ANIM_THIEF_TIED_UP_WALK = 6, ///< walking with the rope round his arms
  ANIM_THIEF_BEATEN = 9 ///< lying on the floor (man_with_long_dark_hair.png,
                        ///< person_in_purple.png)
};

static void LogRoomsA_IvanJoinsRodSearch(void);
static void LogRoomsA_DiscussMindReading(void);
static void LogRoomsA_FaceSuspectsTowardLeader(void);
static void LogRoomsA_AdvanceSuspectRoute(s32 actorId, s32 expected, s32 next,
                                          const s32 *actions);
static void LogRoomsA_LoseToThieves(void);
static void LogRoomsA_FightThievesAgain(void);
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
static void Scene_StartMindRead(s32 casterId, s32 targetId);
static void Scene_EndMindRead(void);
static void LogRoomsA_UpdateCorneringChase(void);
static void LogRoomsA_SpreadOutToCornerSuspects(void);

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

  switch (gMainRoutineState.entrance) {
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
/// Jumps the leader to the requested destination when nothing blocks it there.
/// Returns 1 when it jumped.
static s32 LogRoomsA_TryJumpTo(struct FieldPosition *position) {
  struct Actor *leader;
  u8 savedMotionFlags;

  leader = Actor_Get(ACTOR_PARTY_LEADER);
  savedMotionFlags = leader->motionFlags;
  if (FieldActor_CheckMovementCollision(leader, position) == 0) {
    Event_Start();
    Actor_SelectAnimation(leader, ANIM_LEADER_SPRING);
    wait(6);
    Sound_Play(SE_JUMP);
    Actor_SelectAnimation(leader, ANIM_LEADER_LEAP);
    leader->speedLimit = toFIXED(3);
    leader->acceleration = toFIXED(2);
    leader->velocityY = toFIXED(4);
    leader->motionFlags &= ACTOR_JUMP_KEEPS_MOTION;
    Actor_SetSpriteFlags(leader, 0);
    Actor_MoveToAndWait(
        ACTOR_PARTY_LEADER,
        ((struct FieldCoordinate *)&position->x)->part[0].integer,
        ((struct FieldCoordinate *)&position->z)->part[0].integer);
    Actor_SelectAnimation(leader, ANIM_LEADER_SPRING);
    Actor_SetSpriteFlags(leader, SPRITE_FLAG_SHADOW);
    wait(6);
    leader->motionFlags = savedMotionFlags;
    Event_End();
    return 1;
  }
  return 0;
}
#endif

/// In the inn's attic, walking north or south at the dark hole in the floor
/// (cells (41, 25) and (41, 27), either side of it), Isaac jumps across.
///
/// The overhead layer's cell over the hole's north edge, (42, 25) (the map's
/// row 85, the layer beginning at row 60), then takes the attributes of the
/// one west of it, floor 32 pixels up, as he lands north, or of the one east
/// of it, none, as he lands south (render probe scene LogRoomsA --entrance 13
/// --cells 40,84,44,87).
// XXX: what the overhead cell's floor changes for the leader there; a probe of
// cell (42, 85) and of his height after each jump would settle it.
static void LogRoomsA_JumpAcrossHoleInAtticFloor(void) {
  s32 x;
  s32 z;
#if !TBS_PATCH_1
  struct FieldPosition target;
#endif

  if (Actor_Get(ACTOR_PARTY_LEADER)->facing >= FACING_NORTHWEST &&
      Actor_Get(ACTOR_PARTY_LEADER)->facing <= FACING_NORTHEAST) {
#if TBS_PATCH_1
    Field_TryJumpForward();
#else
    SET_POSITION(&target, Actor_Get(ACTOR_PARTY_LEADER)->x.fixed,
                 Actor_Get(ACTOR_PARTY_LEADER)->y.fixed,
                 Actor_Get(ACTOR_PARTY_LEADER)->z.fixed - toFIXED(30));
    LogRoomsA_TryJumpTo(&target);
#endif
    x = 42;
    z = 85;
    MapGrid_CopyCellAttributes(41, 85, 1, 1, x, z);
  } else if (Actor_Get(ACTOR_PARTY_LEADER)->facing >= FACING_SOUTHEAST &&
             Actor_Get(ACTOR_PARTY_LEADER)->facing <= FACING_SOUTHWEST) {
#if TBS_PATCH_1
    Field_TryJumpForward();
#else
    SET_POSITION(&target, Actor_Get(ACTOR_PARTY_LEADER)->x.fixed,
                 Actor_Get(ACTOR_PARTY_LEADER)->y.fixed,
                 Actor_Get(ACTOR_PARTY_LEADER)->z.fixed + toFIXED(30));
    LogRoomsA_TryJumpTo(&target);
#endif
    x = 42;
    z = 85;
    MapGrid_CopyCellAttributes(43, 85, 1, 1, x, z);
  }
}

/// Move on the big crate in the inn's attic: once it has been pushed a cell
/// west, to map column 42, the way behind it into the loft opens.
static void LogRoomsA_CheckCrateMoved(void) {
  Event_Start();
  if (FIELD_CELL(Actor_Get(ACTOR_CRATE)->x.fixed) == CRATE_MOVED_COLUMN) {
    MapGrid_CopyCellAttributes(101, 24, 3, 4, 41, 24);
    GameFlag_Set(FLAG_VAULT_CRATE_MOVED);
  }
  Event_End();
}

/// Spoken to across the counter of Vault's weapon and armor shop, the weapon
/// merchant opens his shop. From anywhere else he says Hammet wasn't the only
/// one robbed, or, once the thieves are jailed, that their stolen weapons were
/// found rusted in the cemetery.
static void LogRoomsA_TalkToWeaponMerchant(void) {
  s32 leaderFacing = Actor_Get(ACTOR_PARTY_LEADER)->facing;

  Event_Start();
  if (leaderFacing > FACING_NORTHWEST && leaderFacing < FACING_NORTHEAST) {
    Shop_Run(VAULT_WEAPON_STOCK, ACTOR_WEAPON_MERCHANT);
  } else {
    if (GameFlag_Get(FLAG_VAULT_THIEVES_JAILED) == 0) {
      Event_SetMessageId(MsgVaultMasterHammetIsntOnlyOne);
    } else {
      Event_SetMessageId(MsgVaultWeFoundOurStolenWeapons);
    }
    Event_ShowMessage(ACTOR_WEAPON_MERCHANT, 0);
  }
  Event_End();
}

/// Spoken to across the counter of Vault's weapon and armor shop, the armor
/// merchant opens his shop. From anywhere else he is angry at the thieves, or,
/// once they are jailed, warns the party to be prepared for the cave in Goma
/// Range.
static void LogRoomsA_TalkToArmorMerchant(void) {
  s32 leaderFacing = Actor_Get(ACTOR_PARTY_LEADER)->facing;

  Event_Start();
  if (leaderFacing > FACING_NORTHWEST && leaderFacing < FACING_NORTHEAST) {
    Shop_Run(VAULT_ARMOR_STOCK, ACTOR_ARMOR_MERCHANT);
  } else {
    if (GameFlag_Get(FLAG_VAULT_THIEVES_JAILED) == 0) {
      Event_SetMessageId(MsgVaultThievesDidntHitOurHouse);
    } else {
      Event_SetMessageId(MsgVaultIfYoureGonnaHeadInto);
    }
    Event_ShowMessage(ACTOR_ARMOR_MERCHANT, 0);
  }
  Event_End();
}

/// Spoken to across the counter of Vault's inn, the woman behind it offers a
/// room. From anywhere else she thinks it unfair to accuse the polite guests
/// upstairs, or, once they are jailed, is shocked they hid the stolen goods in
/// the loft.
static void LogRoomsA_TalkToInnkeeper(void) {
  s32 leaderFacing = Actor_Get(ACTOR_PARTY_LEADER)->facing;

  Event_Start();
  if (leaderFacing > FACING_NORTHWEST && leaderFacing < FACING_NORTHEAST) {
    Inn_CheckIn(VAULT_INN, ACTOR_INNKEEPER);
  } else {
    if (GameFlag_Get(FLAG_VAULT_THIEVES_JAILED) == 0) {
      Event_SetMessageId(MsgVaultEveryoneThinksOurGuestsThieves);
    } else {
      Event_SetMessageId(MsgVaultTheyHidThoseStolenGoods);
    }
    Event_ShowMessage(ACTOR_INNKEEPER, 0);
  }
  Event_End();
}

#include "shared/vault_bone.c"

/// Opening the menu while the party is cornering the suspects in the guests'
/// room at Vault's inn only has Garet tell Isaac they don't have time for
/// that.
static void LogRoomsA_GaretHasNoTimeForMenu(void) {
#if TBS_PATCH_1
  Leader_StopMoving();
#endif
  Event_SetMessageId(MsgVaultWeDontHaveTimeFor);
  // "We don't have time for that, Isaac!"
  Event_ShowMessage(ACTOR_GARET, 0);
}

/// Searching the barrel beside the girl at the inn before the thieves are
/// caught, Isaac doesn't find anything. Later the Bones are there.
static void LogRoomsA_SearchEmptyBarrel(void) {
  Event_Start();
  FieldMessage_Show(MsgVaultIsaacCheckedBarrel, 1);
  FieldMessage_Show(MsgVaultButDidntFindAnything, 1);
  Event_End();
}

/// Searching one of the three chests in the inn's loft, Isaac finds it empty.
static void LogRoomsA_SearchEmptyChest(void) {
  Event_Start();
  FieldMessage_Show(MsgVaultIsaacCheckedChest, 1);
  FieldMessage_Show(MsgVaultButChestWasEmpty, 1);
  Event_End();
}

/// Returns the loft's events for the loft's three entrances, else the
/// houses' events, which change once FLAG_VAULT_THIEVES_JAILED is set.
const struct Trigger *Scene_GetTriggers(void) {
  switch (gMainRoutineState.entrance) {
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
static void LogRoomsA_StandAndShowMessage(s32 speakerId) {
  Actor_SetAnimation(speakerId, ANIM_STAND);
  LogRoomsA_FaceEachOtherAndWait(speakerId, ACTOR_PARTY_LEADER, 2);
  Event_ShowMessage(speakerId, 0);
}

#include "shared/vault_talk.c"

/// The speaker and the leader face each other, the speaker asks the selected
/// question, and answers a yes with the next message and a no with the one
/// after it.
static void LogRoomsA_AskYesNoAndAnswer(s32 speakerId) {
  LogRoomsA_FaceEachOtherAndWait(speakerId, ACTOR_PARTY_LEADER, 2);
  Event_OpenMessage(speakerId, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) != 0) {
    gFieldState->message++;
  }
  Event_ShowMessage(speakerId, 0);
}

/// In the house in the middle of Vault, a small boy asks Isaac whether it is
/// fun to see strange new places. A Yes has him dream of being an adventurer
/// and sleeping in a tent; a No, wonder why bother then.
static void LogRoomsA_BoyAsksAboutTravel(void) {
  Event_Start();
  Event_SetMessageId(MsgVaultMisterFunSeeStrangeNew);
  Actor_SetAnimation(ACTOR_BOY_ASKING_ABOUT_TRAVEL, ANIM_STAND);
  LogRoomsA_AskYesNoAndAnswer(ACTOR_BOY_ASKING_ABOUT_TRAVEL);
  Event_End();
}

/// Before Ivan has joined the party, a little girl at the mayor's house in
/// Vault teases that Ivan has great powers and asks whether Isaac would like
/// to know them. A Yes gets no way, it's their secret; a No, a warning not to
/// say what he doesn't mean, as Ivan can tell in an instant.
static void LogRoomsA_OfferIvansSecret(void) {
  Event_Start();
  Event_SetMessageId(MsgVaultIvanHasGreatPowersWouldnt);
  LogRoomsA_AskYesNoAndAnswer(ACTOR_GIRL_TELLING_A_SECRET);
  Actor_FaceDirection(ACTOR_GIRL_TELLING_A_SECRET, FACING_WEST, 0);
  Event_End();
}

/// Spoken to across the counter of Vault's item shop, the item merchant opens
/// her shop. From anywhere else she asks whether the party arrived in town
/// today, and finds them fishy on a No; once the thieves are jailed she says it
/// will be a while before anyone can go to Kalay with the bridge out.
static void LogRoomsA_TalkToItemMerchant(void) {
  s32 leaderFacing = Actor_Get(ACTOR_PARTY_LEADER)->facing;

  Event_Start();
  if (leaderFacing > FACING_NORTHWEST && leaderFacing < FACING_NORTHEAST) {
    Shop_Run(VAULT_ITEM_STOCK, ACTOR_ITEM_MERCHANT);
  } else if (GameFlag_Get(FLAG_VAULT_THIEVES_JAILED) == 0) {
    Event_SetMessageId(MsgVaultDidJustArriveInTown);
    LogRoomsA_AskYesNoAndAnswer(ACTOR_ITEM_MERCHANT);
  } else {
    Event_SetMessageId(MsgVaultWithBridgeOutWillQuite);
    // "With the bridge out, it will be quite a while 'til we can go..."
    Event_ShowMessage(ACTOR_ITEM_MERCHANT, 0);
  }
  Event_End();
}

/// In the gem owner's house, his wife wishes their rocks were gems, or, once
/// the thieves are jailed, is thankful they were caught before they stole the
/// gem.
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

/// In Vault's item shop, Grandpa wonders whether he is missing something, or,
/// once the thieves are jailed, says they hid the stolen treasure in the
/// hotel's loft and more in the cemetery.
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

/// In his house, Vault's mayor asks whether someone could please help Ivan.
/// Once Ivan has joined the party, he thanks Isaac the first time, and after
/// that asks him to look after Ivan.
static void LogRoomsA_MayorAsksHelpForIvan(void) {
  s32 thankedForHelpingFlag = FLAG_VAULT_MAYOR_THANKED_PARTY;
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

/// Before the thieves are caught, the girl at Vault's inn says the three
/// strangers have fooled the innkeeper and his wife.
static void LogRoomsA_WarnAboutThreeStrangers(void) {
  Event_Start();
  Event_SetMessageId(MsgVaultThoseThreeStrangersSureHave);
  LogRoomsA_StandAndShowMessage(ACTOR_GIRL_AT_THE_INN);
  Event_End();
}

/// Talking to, or reading the mind of, the suspect with long dark hair in the
/// guests' room at Vault's inn makes him cry out to stop it and run round the
/// beds to his next spot, on or back the way he was going by which side he is
/// pushed from.
static void LogRoomsA_SuspectedLeaderDodges(void) {
  struct Actor *suspect;
  u32 facing;

  suspect = Actor_Get(ACTOR_SUSPECTED_LEADER);
  Event_Start();
  Actor_RunRepeatedMotion(ACTOR_SUSPECTED_LEADER, 2);
  Event_SetMessageId(MsgVaultOwStop);
  // "Ow! Stop it!"
  Event_ShowMessage(ACTOR_SUSPECTED_LEADER, 0);
  Actor_SetSpeed(ACTOR_SUSPECTED_LEADER, 4);
  // Pushed from the north he runs on along his route, from the south back.
  facing = FACING_SIXTEENTH(suspect->facing);
  if (facing >= FACING_SOUTH_SOUTHWEST && facing <= FACING_NORTH_NORTHWEST) {
    if (suspect->timer <= LEADER_SPOT_EAST_BED_HEAD) {
      Actor_SetScript(ACTOR_SUSPECTED_LEADER,
                      gLogRoomsA_LeaderRunsOn[suspect->timer]);
      suspect->timer++;
    } else {
      Actor_SetScript(ACTOR_SUSPECTED_LEADER,
                      gLogRoomsA_LeaderRunsBack[suspect->timer]);
      suspect->timer--;
    }
  } else {
    if (suspect->timer > LEADER_SPOT_EAST_BED_HEAD) {
      Actor_SetScript(ACTOR_SUSPECTED_LEADER,
                      gLogRoomsA_LeaderRunsOn[suspect->timer]);
      suspect->timer++;
    } else {
      Actor_SetScript(ACTOR_SUSPECTED_LEADER,
                      gLogRoomsA_LeaderRunsBack[suspect->timer]);
      suspect->timer--;
    }
  }
  if (suspect->timer > LEADER_SPOT_WEST_BED_FOOT) {
    suspect->timer = LEADER_SPOT_WEST_BED_HEAD;
  }
  if (suspect->timer < LEADER_SPOT_WEST_BED_HEAD) {
    suspect->timer = LEADER_SPOT_WEST_BED_FOOT;
  }
  Actor_WaitForScriptEndById(ACTOR_SUSPECTED_LEADER);
  Event_End();
}

/// Starts one of the suspected thief's runs (gLogRoomsA_ThiefRuns) and moves
/// his timer on to the spot it ends at.
static inline void LogRoomsA_StartThiefRun(struct Actor *actor, s32 scriptRow,
                                           s32 routePair, u16 routeDelta) {
  Actor_SetScript(ACTOR_SUSPECTED_THIEF,
                  gLogRoomsA_ThiefRuns[scriptRow * 4 + actor->timer]);
  actor->timer = actor->timer - routePair * 2 + routeDelta;
}

/// Talking to, or reading the mind of, the suspect in purple in the guests'
/// room at Vault's inn makes him cry out to be left alone and run to his next
/// spot by the beds or in a corner, picked by the way he is pushed.
static void LogRoomsA_SuspectedThiefDodges(void) {
  struct Actor *actor;
  u32 facing;
  s32 routePair;

  actor = Actor_Get(ACTOR_SUSPECTED_THIEF);
  facing = FACING_SIXTEENTH(actor->facing);
  routePair = actor->timer >> 1;
  Event_Start();
  Actor_RunRepeatedMotion(ACTOR_SUSPECTED_THIEF, 2);
  Event_SetMessageId(MsgVaultNoLeaveAlone);
  // "No! Leave me alone!"
  Event_ShowMessage(ACTOR_SUSPECTED_THIEF, 0);
  Actor_SetSpeed(ACTOR_SUSPECTED_THIEF, 3.5);
  switch (actor->timer) {
  case THIEF_SPOT_SOUTH_WALL:
    if (facing > FACING_SOUTHEAST && facing < FACING_NORTHWEST) {
      Actor_SetScript(ACTOR_SUSPECTED_THIEF,
                      gLogRoomsA_SuspectAlongToSouthEastCorner);
      actor->timer = THIEF_SPOT_SOUTH_EAST_CORNER;
    } else {
      Actor_SetScript(ACTOR_SUSPECTED_THIEF,
                      gLogRoomsA_SuspectAlongToSouthWestCorner);
      actor->timer = THIEF_SPOT_SOUTH_WEST_CORNER;
    }
    break;
  case THIEF_SPOT_WEST_OF_BEDS:
  case THIEF_SPOT_SOUTH_EAST_CORNER:
    if (facing > FACING_SOUTHEAST && facing < FACING_NORTHWEST) {
      LogRoomsA_StartThiefRun(actor, routePair, routePair, 1);
    } else {
      LogRoomsA_StartThiefRun(actor, routePair ^ 1, routePair, -1);
    }
    break;
  case THIEF_SPOT_EAST_OF_BEDS:
  case THIEF_SPOT_SOUTH_WEST_CORNER:
    if (facing > FACING_SOUTHWEST && facing < FACING_NORTHEAST) {
      LogRoomsA_StartThiefRun(actor, routePair, routePair, 1);
    } else {
      LogRoomsA_StartThiefRun(actor, routePair ^ 1, routePair, -1);
    }
    break;
  }
  actor->timer &= 3;
  Actor_WaitForScriptEndById(ACTOR_SUSPECTED_THIEF);
  Event_End();
}

/// Once the thieves are jailed, the burly grey-haired man in the house in the
/// middle of Vault asks whether they came from Lunpa. A Yes has him say
/// Lunpa's master Dodonpa is an awful man and catching his thieves means
/// trouble; a No, that the Lunpans were noble thieves until Dodonpa.
static void LogRoomsA_AskWhetherThievesCameFromLunpa(void) {
  Event_Start();
  Event_SetMessageId(MsgVaultDidThoseThievesComeFrom);
  LogRoomsA_AskYesNoAndAnswer(ACTOR_VILLAGER_SUSPECTING_THE_NEIGHBOR);
  Event_End();
}

/// Once the thieves are jailed, the small boy in the house in the middle of
/// Vault says his father is worried about the prisoners from Lunpa, and asks
/// the party to protect them if Dodonpa's men attack.
static void LogRoomsA_BoyWorriesAboutFather(void) {
  Event_Start();
  Event_SetMessageId(MsgVaultMyFatherWorriedAboutThose);
  LogRoomsA_StandAndShowMessage(ACTOR_BOY_ASKING_ABOUT_TRAVEL);
  Event_End();
}

/// Once the thieves are jailed, a small boy at the mayor's house in Vault has
/// heard the party defeated them. A Yes has him say you can't judge people by
/// their looks; a No, that his father said it was them.
static void LogRoomsA_AskAboutDefeatingThieves(void) {
  Event_Start();
  Event_SetMessageId(MsgVaultHeardDefeatedThoseThieves);
  LogRoomsA_AskYesNoAndAnswer(ACTOR_BOY_WISHING_TO_BE_OLDER);
  Event_End();
}

/// Once the thieves are jailed, Vault's mayor thanks Isaac in his house and
/// gives the promised Water of Life, or asks him to come back with fewer things
/// if his bags are full. Then he asks whether the party is heading beyond Goma
/// Range: a Yes gets directions to the cave in its mountains north-east of
/// Vault, past which lies Bilibin; a No, that there is no other way out.
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
    gFieldState->message++;
    if (PartyInventory_HasSpace() == 0) {
      // "Oh no, Isaac and his friends have too many items!"
      Event_SetMessageId(MsgVaultYouWereSuchGreatHelp + 3);
      LogRoomsA_ShowMessageAndWait(ACTOR_MAYOR, 20);
      Event_End();
      return;
    }
    GameFlag_Set(FLAG_VAULT_WATER_OF_LIFE_RECEIVED);
    Party_GiveItem(ITEM_WATER_OF_LIFE, 0);
  }
  Event_SetMessageId(MsgVaultHeadingOutBeyondGomaRange);
  // "Are you heading out beyond Goma Range, by any chance?"
  Event_OpenMessage(ACTOR_MAYOR, 0);
  Event_Wait(20);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) != 0) {
    gFieldState->message++;
  }
  Event_ShowMessage(ACTOR_MAYOR, 0);
  Event_End();
}

/// Once the thieves are jailed, the girl at Vault's inn offers Isaac leftover
/// bones from dinner, or asks whether he wants more. Accepting gives a Bone,
/// unless his bags are full and she calls him a greedy lad.
static void LogRoomsA_OfferBones(void) {
  Event_Start();
  LogRoomsA_FaceEachOtherAndWait(ACTOR_GIRL_AT_THE_INN, ACTOR_PARTY_LEADER, 2);
  if (GameFlag_Get(FLAG_VAULT_BONE_RECEIVED) == 0) {
    Event_SetMessageId(MsgVaultHaveLotLeftoverBonesFrom);
    // "I have a lot of leftover bones from dinner. Here, do you..."
    Event_OpenMessage(ACTOR_GIRL_AT_THE_INN, 0);
  } else {
    Event_SetMessageId(MsgVaultWantMoreBones);
    // "What? You want more bones?"
    Event_OpenMessage(ACTOR_GIRL_AT_THE_INN, 0);
  }
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
    Event_Wait(20);
    Event_ShowMessage(ACTOR_GIRL_AT_THE_INN, 0);
    Event_Wait(20);
    Actor_RunRepeatedMotion(ACTOR_GIRL_AT_THE_INN, 2);
    Event_Wait(20);
    if (PartyInventory_HasSpace() == 0) {
      Actor_SetAnimationAndWait(ACTOR_GIRL_AT_THE_INN, ANIM_SHAKE_HEAD);
      Event_Wait(20);
      Event_SetMessageId(MsgVaultWowHaveManyThingsArent);
      // "Wow, you have so many things. Aren't you a greedy lad!"
      Event_ShowMessage(ACTOR_GIRL_AT_THE_INN, 0);
    } else {
      Item_ShowFound(ITEM_BONE, 3);
      Party_GiveItem(ITEM_BONE, 0);
      GameFlag_Set(FLAG_VAULT_BONE_RECEIVED);
    }
  } else {
    gFieldState->message++;
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_GIRL_AT_THE_INN, ANIM_NOD);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_GIRL_AT_THE_INN, 0);
  }
  Actor_FaceDirection(ACTOR_GIRL_AT_THE_INN, FACING_SOUTH, 0);
  Event_End();
}

/// Mind Read on the gem owner's wife in their house: she wants a look at that
/// rock, or, once the thieves are jailed, wonders whether it is worthless and
/// that is why they didn't steal it.
static void LogRoomsA_ReadGemOwnersWifesMind(void) {
  Event_Start();
  if (GameFlag_Get(FLAG_VAULT_THIEVES_JAILED) == 0) {
    Event_SetMessageId(MsgVaultOkMisterLetMeSee);
  } else {
    Event_SetMessageId(MsgVaultIfRockWorthlessMaybeThats);
  }
  Vault_ShowThought(ACTOR_GEM_OWNERS_WIFE);
  Event_End();
}

/// Mind Read on the small boy in the house in the middle of Vault: he wonders
/// what the outside world is like, or, once the thieves are jailed, thinks his
/// father looks sad with worry.
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

/// Mind Read on Grandpa in Vault's item shop: he wonders whether the party
/// could be the thieves and doesn't know who to trust anymore. Once the thieves
/// are jailed, he thinks nothing in their house was valuable enough to steal.
static void LogRoomsA_ReadGrandpasMind(void) {
  Event_Start();
  if (GameFlag_Get(FLAG_VAULT_THIEVES_JAILED) == 0) {
    Event_SetMessageId(MsgVaultCouldTheyThievesGoodDont);
  } else {
    Event_SetMessageId(MsgVaultGuessNothingInOurHouse);
  }
  Vault_ShowThought(ACTOR_GRANDPA);
  Event_End();
}

/// Mind Read on Vault's mayor in his house: he laughs that it tickles, being
/// tickled by a boy, or, once the thieves are jailed, thinks the cave in the
/// Goma Range is dangerous, but the party will be okay.
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

/// Mind Read on the girl at Vault's inn: before the thieves are caught she
/// thinks the master and his wife are blinded by money. After, she is tempted
/// to taste the cooking, or, once Isaac has a Bone, wonders whether he has a
/// calcium deficiency.
static void LogRoomsA_ReadInnGirlsMind(void) {
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

/// Talking to Ivan at the mayor's house in Vault: the first time, he reads
/// Isaac's mind, admits his strange powers, learns they are called Psynergy,
/// and asks for help getting Master Hammet's rod back. Later he only asks
/// again. A Yes has him join the party.
static void LogRoomsA_MeetIvan(void) {
  struct Actor *actor;

  Event_Start();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.8);
  Actor_SetSpeed(ACTOR_GARET, 0.8);
  Actor_SetSpeed(ACTOR_IVAN, 0.8);
  // Isaac steps up in front of Ivan and Garet comes out beside him.
  Sound_Play(MUS_SILENCE);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 384, 408);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_SetPosition(ACTOR_GARET, 384, 408);
  Actor_WalkToAndWait(ACTOR_GARET, 368, 408);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH_NORTHEAST, 20);
  // Introduce Ivan's ability only on the first meeting.
  if (Event_GetFlag(FLAG_VAULT_MET_IVAN) == 0) {
    GameFlag_Set(FLAG_VAULT_MET_IVAN);
    // Ivan reads Isaac's mind and admits his strange powers.
    Scene_StartMindRead(ACTOR_IVAN, ACTOR_PARTY_LEADER);
    Event_Wait(40);
    Scene_EndMindRead();
    Event_SetMessageId(MsgVaultDoPossessStrangePowers);
    Sound_Play(MUS_WIND_ADEPTS);
    Event_Wait(30);
    LogRoomsA_PlayAnimationAndWait(ACTOR_IVAN, ANIM_NOD, 30);
    LogRoomsA_ShowMessageAndWait(ACTOR_IVAN, 30);
    // The two start; he reads again: "You have them, too, don't you?"
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Actor_RunRepeatedMotion(ACTOR_GARET, 1);
    Event_Wait(20);
    Scene_StartMindRead(ACTOR_IVAN, ACTOR_PARTY_LEADER);
    Event_Wait(40);
    Scene_EndMindRead();
    LogRoomsA_ShowMessageAndWait(ACTOR_IVAN, 30);
    LogRoomsA_FaceEachOtherAndWait(ACTOR_PARTY_LEADER, ACTOR_GARET, 50);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Actor_FaceDirection(ACTOR_GARET, FACING_NORTH_NORTHEAST, 0);
    Event_Wait(20);
    // A third reading: his power is called Psynergy; then they have so much
    // in common he can trust them.
    Scene_StartMindRead(ACTOR_IVAN, ACTOR_PARTY_LEADER);
    Event_Wait(40);
    Scene_EndMindRead();
    Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
    LogRoomsA_ShowMessageAndWait(ACTOR_IVAN, 50);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
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
    Actor_FaceDirection(ACTOR_GARET, FACING_NORTH_NORTHEAST, 0);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
    Event_Wait(50);
    LogRoomsA_PlayAnimationAndWait(ACTOR_IVAN, ANIM_NOD, 30);
    Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTH, 0);
    Event_Wait(10);
    Event_ShowMessage(ACTOR_IVAN, 0);
    // "Would you please help me get my master's rod back?"
    Event_OpenMessage(ACTOR_IVAN, 0);
  } else {
    Sound_Play(MUS_WIND_ADEPTS);
    Event_SetMessageId(MsgVaultWouldReallyWouldHelpMe);
    // "Would you really? Would you help me find the rod?"
    Event_OpenMessage(ACTOR_IVAN, 0);
  }
  // On a Yes Ivan joins and walks into Isaac, as a party member does; a No
  // gets his apology for asking.
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
    LogRoomsA_IvanJoinsRodSearch();
    GameFlag_Set(FLAG_VAULT_IVAN_JOINED_ROD_SEARCH);
    Actor_SetAnimation(ACTOR_IVAN, ANIM_WALK);
    actor = Actor_Get(ACTOR_PARTY_LEADER);
    if (actor != NULL) {
      Actor_SetDestination(ACTOR_IVAN, actor->x.part[0].integer,
                           actor->z.part[0].integer);
    }
    Actor_WaitForMoveById(ACTOR_IVAN);
    Actor_SetPosition(ACTOR_IVAN, 0, 0);
  } else {
    Event_ShowMessage(ACTOR_IVAN, 0);
  }
  // Garet walks back into the leader and the earlier music returns.
  Actor_SetAnimation(ACTOR_GARET, ANIM_WALK);
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  if (actor != NULL) {
    Actor_SetDestination(ACTOR_GARET, actor->x.part[0].integer,
                         actor->z.part[0].integer);
  }
  Actor_WaitForMoveById(ACTOR_GARET);
  Actor_SetPosition(ACTOR_GARET, 0, 0);
  Scene_PlayMusic();
  Event_End();
}

/// Ivan jumps for joy, thanks Isaac and Garet, reads their names from Isaac's
/// mind, shows them Mind Read on each other, and joins the party.
static void LogRoomsA_IvanJoinsRodSearch(void) {
  // Ivan jumps for joy: "Thank you. With your help, I believe we can recover
  // the rod." Then he asks their names.
  Event_Wait(20);
  Actor_Get(ACTOR_IVAN)->paused = 0;
  Actor_Jump(ACTOR_IVAN, 4, 0);
  Event_Wait(40);
  Event_SetMessageId(MsgVaultThankHelpBelieve);
  Event_Wait(20);
  LogRoomsA_ShowMessageAndWait(ACTOR_IVAN, 20);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Event_Wait(50);
  LogRoomsA_ShowMessageAndWait(ACTOR_IVAN, 30);
  // He reads their names from Isaac's mind; Isaac backs away from him.
  Actor_WalkToAndWait(ACTOR_IVAN, 376, 392);
  Scene_StartMindRead(ACTOR_IVAN, ACTOR_PARTY_LEADER);
  Event_Wait(40);
  Scene_EndMindRead();
  Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  Actor_Get(ACTOR_PARTY_LEADER)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 384, 424);
  Event_Wait(1);
  Actor_Get(ACTOR_PARTY_LEADER)->steeringFlags |= ACTOR_FACES_MOTION;
  Event_Wait(30);
  Event_ShowMessage(ACTOR_IVAN, 0);
  // Demonstrate Mind Read and let Garet and Isaac react to Ivan's explanation.
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_GARET,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(60);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  LogRoomsA_PlayAnimationAndWait(ACTOR_GARET, ANIM_NOD, 30);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Event_Wait(60);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
  Event_Wait(10);
  LogRoomsA_PlayAnimationAndWait(ACTOR_IVAN, ANIM_NOD, 20);
  Event_ShowMessage(ACTOR_IVAN, 0);
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
  Event_OpenMessage(ACTOR_IVAN, 0);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
  Actor_FaceActor(ACTOR_GARET, ACTOR_IVAN, 0);
  Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0);
  Event_Wait(30);
  // Ivan and Isaac clasp hands, and Garet cries out that his mind is read.
  LogRoomsA_PlayAnimationAndWait(ACTOR_IVAN, ANIM_NOD, 10);
  Actor_WalkToAndWait(ACTOR_IVAN, 384, 408);
  Event_Wait(10);
  FieldMessage_Show(MsgVaultThankHelpBelieve + IVAN_JOINING_ISAAC_CLASPS_HANDS,
                    1);
  Event_SetMessageId(MsgVaultThankHelpBelieve +
                     IVAN_JOINING_GARET_FEELS_MIND_READ);
  Actor_FaceEachOther(ACTOR_IVAN, ACTOR_GARET, 0);
  LogRoomsA_FaceActorAndWait(ACTOR_PARTY_LEADER, ACTOR_GARET, 20);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Event_Wait(60);
  Actor_RunRepeatedMotion(ACTOR_GARET, 1);
  Actor_Get(ACTOR_GARET)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(ACTOR_GARET, 352, 408);
  Event_Wait(1);
  Actor_Get(ACTOR_GARET)->steeringFlags |= ACTOR_FACES_MOTION;
  Event_Wait(10);
  // Garet backs off; Ivan asks whether Isaac could read Garet's mind, and the
  // two read it together. "Just as I thought."
  Actor_WalkTo(ACTOR_IVAN, 368, 408);
  Event_Wait(20);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 368, 424);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_GARET, 0);
  Actor_WaitForMoveById(ACTOR_IVAN);
  Scene_StartMindRead(ACTOR_IVAN, ACTOR_GARET);
  LogRoomsA_PlayAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD, 10);
  Event_ShowMessage(ACTOR_GARET, 0);
  Scene_EndMindRead();
  LogRoomsA_FaceEachOtherAndWait(ACTOR_IVAN, ACTOR_PARTY_LEADER, 30);
  Event_ShowMessage(ACTOR_IVAN, 0);
  Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  Event_Wait(30);
  LogRoomsA_PlayAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD, 30);
  LogRoomsA_PlayAnimationAndWait(ACTOR_IVAN, ANIM_NOD, 10);
  Event_ShowMessage(ACTOR_IVAN, 0);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Actor_FaceActor(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
  Event_ShowMessage(ACTOR_GARET, 0);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_GARET, 0);
  LogRoomsA_PlayAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD, 40);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
  LogRoomsA_FaceActorAndWait(ACTOR_GARET, ACTOR_IVAN, 30);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Event_Wait(60);
  // Sharing their Psynergy let Isaac read minds too; Garet protests that it
  // isn't fair, and Ivan agrees it is one-way.
  LogRoomsA_PlayAnimationAndWait(ACTOR_IVAN, ANIM_NOD, 20);
  Event_ShowMessage(ACTOR_IVAN, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
  Event_Wait(60);
  LogRoomsA_FaceActorAndWait(ACTOR_GARET, ACTOR_PARTY_LEADER, 10);
  Event_ShowMessage(ACTOR_GARET, 0);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Event_Wait(60);
  Event_ShowMessage(ACTOR_IVAN, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Event_Wait(60);
  // Ivan and Garet clasp hands, and Garet reads Isaac's mind.
  FieldMessage_Show(MsgVaultThankHelpBelieve + IVAN_JOINING_GARET_CLASPS_HANDS,
                    1);
  Event_SetMessageId(MsgVaultThankHelpBelieve + IVAN_JOINING_GARET_READS_MIND);
  Actor_FaceEachOther(ACTOR_IVAN, ACTOR_PARTY_LEADER, 0);
  Actor_FaceActor(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Event_Wait(60);
  Scene_StartMindRead(ACTOR_IVAN, ACTOR_PARTY_LEADER);
  FieldPsynergy_RestoreMindReadPalette();
  LogRoomsA_ShowMessageAndWait(ACTOR_GARET, 30);
  Scene_EndMindRead();
  Actor_FaceEachOther(ACTOR_IVAN, ACTOR_GARET, 0);
  LogRoomsA_PlayAnimationAndWait(ACTOR_IVAN, ANIM_NOD, 10);
  LogRoomsA_ShowMessageAndWait(ACTOR_IVAN, 20);
  Actor_FaceEachOther(ACTOR_IVAN, ACTOR_PARTY_LEADER, 0);
  LogRoomsA_PlayAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD, 20);
  Actor_RunRepeatedMotion(ACTOR_GARET, 1);
  Event_Wait(20);
  // "Let's use Mind Read to find the thieves!" Garet worries people will
  // notice.
  LogRoomsA_PlayAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD, 10);
  LogRoomsA_ShowMessageAndWait(ACTOR_GARET, 30);
  Actor_FaceActor(ACTOR_IVAN, ACTOR_GARET, 0);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Event_Wait(30);
  LogRoomsA_FaceEachOtherAndWait(ACTOR_PARTY_LEADER, ACTOR_GARET, 10);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  LogRoomsA_PlayAnimationAndWait(ACTOR_GARET, ANIM_NOD, 30);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
  Actor_FaceActor(ACTOR_GARET, ACTOR_IVAN, 0);
  Actor_FaceActor(ACTOR_IVAN, ACTOR_PARTY_LEADER, 0);
  // Only Adepts can tell when Ivan uses Psynergy; ordinary people see
  // nothing, and he promises not to read their minds again.
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 0);
  Event_Wait(60);
  LogRoomsA_ShowMessageAndWait(ACTOR_IVAN, 10);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  LogRoomsA_PlayAnimationAndWait(ACTOR_GARET, ANIM_NOD, 20);
  LogRoomsA_PlayAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD, 20);
  LogRoomsA_ShowMessageAndWait(ACTOR_IVAN, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Event_Wait(60);
  LogRoomsA_PlayAnimationAndWait(ACTOR_IVAN, ANIM_NOD, 10);
  LogRoomsA_ShowMessageAndWait(ACTOR_IVAN, 30);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Event_Wait(60);
  LogRoomsA_PlayAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD, 10);
  LogRoomsA_ShowMessageAndWait(ACTOR_IVAN, 30);
  LogRoomsA_PlayAnimationAndWait(ACTOR_IVAN, ANIM_NOD, 10);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  LogRoomsA_PlayAnimationAndWait(ACTOR_GARET, ANIM_NOD, 20);
  // "Ivan joined your party!", then he will read the townspeople's minds.
  Event_AddPartyMember(IVAN, 1);
  Event_SetMessageId(MsgVaultThankHelpBelieve +
                     IVAN_JOINING_IVAN_WILL_READ_TOWN);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
  // "Let me read the minds of the townspeople to find the rod."
  Event_ShowMessage(ACTOR_IVAN, 0);
}

/// In the inn's loft, Isaac cuts free the young man tied up on the floor. He
/// tells how he was whacked from behind while fixing the roof, and notices the
/// chests; then the three thieves come in, tell of Lunpa and Dodonpa, ask to
/// be let go, and attack.
static void LogRoomsA_FreeVillagerAndFightThieves(void) {
  s32 base;

  Event_Start();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.8);
  Actor_SetSpeed(ACTOR_GARET, 0.8);
  Actor_SetSpeed(ACTOR_IVAN, 0.8);
  // Isaac cuts the tied-up villager free from the ropes; he gets up and nods.
  Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 3);
  Event_Wait(20);
  base = MsgVaultVaultCutFree;
  FieldMessage_Show(base, 1);
  base++;
  Event_SetMessageId(base);
  Event_Wait(30);
  Actor_SetAnimation(ACTOR_TIED_UP_VILLAGER, ANIM_STAND);
  Event_Wait(60);
  LogRoomsA_PlayAnimationAndWait(ACTOR_TIED_UP_VILLAGER, ANIM_NOD, 40);
  // Garet and Ivan come out of the leader around the tied-up villager.
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 792, 440);
  LogRoomsA_FaceActorAndWait(ACTOR_PARTY_LEADER, ACTOR_TIED_UP_VILLAGER, 20);
  Actor_SetPosition(ACTOR_GARET, 792, 440);
  Actor_SetPosition(ACTOR_IVAN, 792, 440);
  Actor_WalkTo(ACTOR_GARET, 808, 432);
  Actor_WalkTo(ACTOR_IVAN, 792, 456);
  Actor_WaitForMoveById(ACTOR_GARET);
  Actor_FaceActor(ACTOR_GARET, ACTOR_TIED_UP_VILLAGER, 0);
  Actor_WaitForMoveById(ACTOR_IVAN);
  LogRoomsA_FaceActorAndWait(ACTOR_IVAN, ACTOR_TIED_UP_VILLAGER, 60);
  // The tied-up villager tells how he came up to fix the roof, was whacked from
  // behind and woke up tied.
  Actor_ShowEmote(ACTOR_TIED_UP_VILLAGER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Event_Wait(60);
  LogRoomsA_ShowMessageAndWait(ACTOR_TIED_UP_VILLAGER, 20);
  Actor_SetAnimation(ACTOR_IVAN, ANIM_NOD);
  LogRoomsA_PlayAnimationAndWait(ACTOR_GARET, ANIM_NOD, 30);
  Actor_RunRepeatedMotion(ACTOR_TIED_UP_VILLAGER, 2);
  Event_Wait(20);
  LogRoomsA_ShowMessageAndWait(ACTOR_TIED_UP_VILLAGER, 20);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Event_Wait(60);
  Actor_FaceDirection(ACTOR_TIED_UP_VILLAGER, FACING_SOUTH_SOUTHEAST, 0);
  Event_Wait(10);
  LogRoomsA_ShowMessageAndWait(ACTOR_TIED_UP_VILLAGER, 30);
  // He looks around at chests he doesn't remember; Ivan guesses the stolen
  // goods are in them.
  Actor_FaceDirection(ACTOR_TIED_UP_VILLAGER, FACING_NORTH_NORTHEAST, 0);
  Event_Wait(30);
  Actor_ShowEmote(ACTOR_TIED_UP_VILLAGER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                  0);
  Event_Wait(60);
  Actor_FaceDirection(ACTOR_TIED_UP_VILLAGER, FACING_NORTH_NORTHWEST, 0);
  Event_Wait(40);
  Actor_FaceDirection(ACTOR_TIED_UP_VILLAGER, FACING_NORTH_NORTHEAST, 0);
  Event_Wait(40);
  LogRoomsA_FaceActorAndWait(ACTOR_TIED_UP_VILLAGER, ACTOR_PARTY_LEADER, 20);
  LogRoomsA_PlayAnimationAndWait(ACTOR_TIED_UP_VILLAGER, ANIM_SHAKE_HEAD, 30);
  LogRoomsA_ShowMessageAndWait(ACTOR_TIED_UP_VILLAGER, 20);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
  LogRoomsA_ShowMessageAndWait(ACTOR_IVAN, 40);
  // The ominous music starts as the thieves' leader appears at the ladder
  // and says "Looks like we've been found out!"; everyone turns to him.
  Actor_SetPosition(ACTOR_THIEF_LEADER, 744, 408);
  Sound_Play(MUS_CHANCE_MEETING_WITH_SWORN_ENEMIES);
  LogRoomsA_ShowMessageAndWait(ACTOR_THIEF_LEADER, 20);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  Actor_StartRepeatedMotion(ACTOR_GARET, 1);
  Actor_StartRepeatedMotion(ACTOR_IVAN, 1);
  Actor_RunRepeatedMotion(ACTOR_TIED_UP_VILLAGER, 1);
  Event_Wait(30);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_THIEF_LEADER, 0);
  Actor_FaceActor(ACTOR_GARET, ACTOR_THIEF_LEADER, 0);
  Actor_FaceActor(ACTOR_IVAN, ACTOR_THIEF_LEADER, 0);
  LogRoomsA_FaceActorAndWait(ACTOR_TIED_UP_VILLAGER, ACTOR_THIEF_LEADER, 40);
  // He walks up and his two men run in behind him.
  Actor_SetSpeed(ACTOR_THIEF_LEADER, 0.8);
  Actor_SetSpeed(ACTOR_FIRST_THIEF, 1.5);
  Actor_SetSpeed(ACTOR_SECOND_THIEF, 1.5);
  Actor_SetPosition(ACTOR_FIRST_THIEF, 744, 408);
  Actor_SetPosition(ACTOR_SECOND_THIEF, 744, 408);
  Actor_WalkToAndWait(ACTOR_THIEF_LEADER, 792, 416);
  Actor_FaceDirection(ACTOR_THIEF_LEADER, FACING_SOUTH_SOUTHEAST, 0);
  Event_Wait(70);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_THIEF_LEADER, 0);
  Actor_FaceActor(ACTOR_GARET, ACTOR_THIEF_LEADER, 0);
  Actor_FaceActor(ACTOR_IVAN, ACTOR_THIEF_LEADER, 0);
  Actor_FaceActor(ACTOR_TIED_UP_VILLAGER, ACTOR_THIEF_LEADER, 0);
  Actor_SetScript(ACTOR_FIRST_THIEF, gLogRoomsA_FirstThiefEntryScript);
  Event_Wait(40);
  Actor_SetScript(ACTOR_SECOND_THIEF, gLogRoomsA_SecondThiefEntryScript);
  Actor_WaitForScriptEndById(ACTOR_SECOND_THIEF);
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
  Actor_FaceActor(ACTOR_GARET, ACTOR_TIED_UP_VILLAGER, 0);
  LogRoomsA_FaceActorAndWait(ACTOR_IVAN, ACTOR_TIED_UP_VILLAGER, 20);
  LogRoomsA_PlayAnimationAndWait(ACTOR_TIED_UP_VILLAGER, ANIM_SHAKE_HEAD, 30);
  LogRoomsA_ShowMessageAndWait(ACTOR_TIED_UP_VILLAGER, 20);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
  Event_Wait(20);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
  LogRoomsA_FaceActorAndWait(ACTOR_GARET, ACTOR_IVAN, 30);
  LogRoomsA_ShowMessageAndWait(ACTOR_GARET, 20);
  Actor_ShowEmote(ACTOR_TIED_UP_VILLAGER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Event_Wait(60);
  LogRoomsA_ShowMessageAndWait(ACTOR_TIED_UP_VILLAGER, 20);
  Actor_RunRepeatedMotion(ACTOR_THIEF_LEADER, 1);
  Event_Wait(10);
  LogRoomsA_ShowMessageAndWait(ACTOR_THIEF_LEADER, 20);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_THIEF_LEADER, 0);
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
  Event_OpenMessage(ACTOR_THIEF_LEADER, 0);
  Event_Wait(50);
  Actor_FaceActor(ACTOR_IVAN, ACTOR_PARTY_LEADER, 0);
  LogRoomsA_FaceActorAndWait(ACTOR_GARET, ACTOR_PARTY_LEADER, 30);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
    Event_Wait(40);
    Actor_FaceActor(ACTOR_GARET, ACTOR_THIEF_LEADER, 0);
    Actor_FaceActor(ACTOR_IVAN, ACTOR_THIEF_LEADER, 0);
    Actor_RunRepeatedMotion(ACTOR_THIEF_LEADER, 2);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_THIEF_LEADER, 0);
  } else {
    Event_Wait(40);
    Actor_FaceActor(ACTOR_GARET, ACTOR_THIEF_LEADER, 0);
    Actor_FaceActor(ACTOR_IVAN, ACTOR_THIEF_LEADER, 0);
    base = MsgVaultVaultThievesCaught;
    Event_SetMessageId(base);
    Actor_RunRepeatedMotion(ACTOR_THIEF_LEADER, 2);
    Event_Wait(20);
    // "What? You mean, even if we return everything we stole and..."
    Event_ShowMessage(ACTOR_THIEF_LEADER, 0);
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
  Actor_SetAnimation(ACTOR_THIEF_LEADER, ANIM_NOD);
  LogRoomsA_PlayAnimationAndWait(ACTOR_FIRST_THIEF, ANIM_NOD, 30);
  LogRoomsA_FaceEachOtherAndWait(ACTOR_THIEF_LEADER, ACTOR_SECOND_THIEF, 30);
  Actor_SetAnimation(ACTOR_THIEF_LEADER, ANIM_NOD);
  LogRoomsA_PlayAnimationAndWait(ACTOR_SECOND_THIEF, ANIM_NOD, 40);
  Actor_FaceDirection(ACTOR_THIEF_LEADER, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_FIRST_THIEF, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_SECOND_THIEF, FACING_SOUTH_SOUTHEAST, 0);
  Event_Wait(20);
  LogRoomsA_PlayAnimationAndWait(ACTOR_THIEF_LEADER, ANIM_SHAKE_HEAD, 20);
  Event_ShowMessage(ACTOR_THIEF_LEADER, 0);
  GameFlag_Set(FLAG_VAULT_THIEVES_ATTACKED);
  gFieldState->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
  base = SCENE_LOG_ROOMS_A;
  Event_SetAfterBattleDestination(base,
                                  LOG_ROOMS_A_LOFT_AFTER_THIEVES_DEFEATED);
  Event_SetDefeatDestination(base, LOG_ROOMS_A_LOFT_AFTER_LOSING_TO_THIEVES);
  Battle_SetType(BATTLE_TYPE_SCRIPTED);
  Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED_VALE_VAULT, 5);
  Event_End();
}

/// Going down the stairs from the inn's upstairs in Vault takes the party to
/// the floor below.
static void LogRoomsA_LeaveGuestsRoom(void) {
  Sound_Play(SE_MAP_EXIT);
  Import_EventSetExitRequest(LOG_ROOMS_A_INN_UPSTAIRS_TO_INN);
}

/// Once the crate is pushed aside, walking east from the inn's attic through
/// the way behind it (cell (45, 25)) takes the party into the loft.
static void LogRoomsA_EnterLoft(void) {
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.5);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 728, 408);
  gFieldState->transitionFrames = MAP_TRANSITION_FRAMES;
  Sound_Play(SE_MAP_EXIT);
  Import_EventSetExitRequest(LOG_ROOMS_A_CRATE_ROOM_TO_LOFT);
}

/// Walking west out of the inn's loft (cell (46, 25)) back into the attic;
/// before the thieves have attacked, the tied-up young man groans as the party
/// goes.
static void LogRoomsA_LeaveLoft(void) {
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.5);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 744, 408);
  if (GameFlag_Get(FLAG_VAULT_THIEVES_ATTACKED) == 0) {
    Event_Start();
    Event_SetMessageId(MsgVaultUhnnUhnn);
    // "Uhnn... Uhnn..."
    Event_ShowMessage(ACTOR_TIED_UP_VILLAGER, 0);
    Event_End();
  }
  gFieldState->transitionFrames = MAP_TRANSITION_FRAMES;
  Sound_Play(SE_MAP_EXIT);
  Import_EventSetExitRequest(LOG_ROOMS_A_LOFT_TO_CRATE_ROOM);
}

/// In the guests' room upstairs at Vault's inn, once Ivan is with the party,
/// the two suspects at the west end of the room start, see that weird kid is
/// with them, and run off round the beds.
static void LogRoomsA_ApproachSuspects(void) {
  struct Actor *suspectedLeader;
  struct Actor *suspectedThief;

  suspectedLeader = Actor_Get(ACTOR_SUSPECTED_LEADER);
  suspectedThief = Actor_Get(ACTOR_SUSPECTED_THIEF);
  Event_Start();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.8);
  Actor_SetSpeed(ACTOR_GARET, 0.8);
  Actor_SetSpeed(ACTOR_IVAN, 0.8);
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
  Actor_SetSpeed(ACTOR_SUSPECTED_LEADER, 4);
  Actor_SetSpeed(ACTOR_SUSPECTED_THIEF, 3.5);
  Actor_SetScript(ACTOR_SUSPECTED_THIEF, gLogRoomsA_SuspectToSouthWestCorner);
  Actor_SetScript(ACTOR_SUSPECTED_LEADER, gLogRoomsA_SuspectRoundToWestBedHead);
  Actor_WaitForScriptEndById(ACTOR_SUSPECTED_LEADER);
  MapGrid_CopyCellAttributes(14, 45, 3, 1, 14, 44);
  GameFlag_Set(FLAG_VAULT_SUSPECTS_APPROACHED);
  GameFlag_Set(FLAG_SUSPECTS_FLEEING);
  Task_AddFrame(LogRoomsA_FaceSuspectsTowardLeader, TASK_PASS_FRAME);
  // The leader stands at the west bed's head but takes up his route as from
  // the middle one's: his first run on goes straight to the east bed's head.
  suspectedLeader->timer = LEADER_SPOT_MIDDLE_BED_HEAD;
  suspectedThief->timer = THIEF_SPOT_SOUTH_WEST_CORNER;
  Event_End();
}

/// Stepping back out of the guests' room at Vault's inn after the suspects
/// have run, the party talks it over: Mind Read is no use while they keep
/// running, so Garet plans for the three to surround one of them, with Isaac
/// in the lead.
static void LogRoomsA_DiscussSuspiciousGuests(void) {
  struct Actor *suspectedLeader;
  struct Actor *suspectedThief;
  s32 base;

  suspectedLeader = Actor_Get(ACTOR_SUSPECTED_LEADER);
  suspectedThief = Actor_Get(ACTOR_SUSPECTED_THIEF);
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
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.8);
  Actor_SetSpeed(ACTOR_GARET, 0.8);
  Actor_SetSpeed(ACTOR_IVAN, 0.8);
  // Outside the room, Garet and Ivan step out of the leader.
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 248, 728);
  Actor_SetPosition(ACTOR_IVAN, 248, 728);
  Actor_SetPosition(ACTOR_GARET, 248, 728);
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
  // "They're acting suspicious. Something's not right..."
  Event_ShowMessage(ACTOR_IVAN, 0);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
  LogRoomsA_FaceActorAndWait(ACTOR_GARET, ACTOR_IVAN, 20);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  LogRoomsA_PlayAnimationAndWait(ACTOR_GARET, ANIM_NOD, 20);
  LogRoomsA_FaceEachOtherAndWait(ACTOR_PARTY_LEADER, ACTOR_GARET, 10);
  // Garet: "But you can't use Mind Read if they keep running away, can you?"
  // Yes: Ivan asks how he can catch them; no: there must be a way.
  Event_OpenMessage(ACTOR_GARET, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) != 0) {
    gFieldState->message++;
  }
  LogRoomsA_ShowMessageAndWait(ACTOR_GARET, 30);
  // Garet's plan: the three of them surround one of the two.
  Event_SetMessageId(base + 4);
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
  // "Isaac, you take the lead."
  Event_ShowMessage(ACTOR_GARET, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Event_Wait(40);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
  LogRoomsA_PlayAnimationAndWait(ACTOR_IVAN, ANIM_NOD, 50);
  Actor_WalkTo(ACTOR_IVAN, 248, 728);
  Actor_WalkToAndWait(ACTOR_GARET, 248, 728);
  Actor_SetPosition(ACTOR_GARET, 0, 0);
  Actor_SetPosition(ACTOR_IVAN, 0, 0);
  Actor_SetPosition(ACTOR_SUSPECTED_LEADER, 104, 696);
  Actor_SetPosition(ACTOR_SUSPECTED_THIEF, 120, 696);
  Actor_FaceDirection(ACTOR_SUSPECTED_LEADER, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_SUSPECTED_THIEF, FACING_WEST, 0);
  MapGrid_CopyCellAttributes(14, 50, 3, 1, 14, 44);
  Event_End();
}

/// Walking back into the guests' room at Vault's inn after the plan, the
/// party sets out to corner the two suspects.
static void LogRoomsA_StartCorneringSuspects(void) {
  GameFlag_Set(FLAG_MENUS_RAISE_TRIGGER);
  GameFlag_Set(FLAG_VAULT_CORNERING_SUSPECTS);
  LogRoomsA_SpreadOutToCornerSuspects();
}

/// The suspects see the party back, cry that they're coming for them
/// one-on-one, and run to their first spots again.
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
  Actor_SetSpeed(ACTOR_SUSPECTED_LEADER, 4);
  Actor_SetSpeed(ACTOR_SUSPECTED_THIEF, 3.5);
  Actor_SetScript(ACTOR_SUSPECTED_THIEF, gLogRoomsA_SuspectToSouthWestCorner);
  Actor_SetScript(ACTOR_SUSPECTED_LEADER, gLogRoomsA_SuspectRoundToWestBedHead);
  Actor_WaitForScriptEndById(ACTOR_SUSPECTED_LEADER);
  // As LogRoomsA_ApproachSuspects leaves them.
  Actor_Get(ACTOR_SUSPECTED_LEADER)->timer = LEADER_SPOT_MIDDLE_BED_HEAD;
  Actor_Get(ACTOR_SUSPECTED_THIEF)->timer = THIEF_SPOT_SOUTH_WEST_CORNER;
  MapGrid_CopyCellAttributes(14, 48, 4, 1, 14, 44);
}

/// Draws Isaac, Garet and Ivan in front of the room's furniture for the talk
/// after the Mind Read.
static inline void LogRoomsA_SetPartyImagePriority(void) {
  {
    struct Sprite *sprite = Actor_Get(ACTOR_PARTY_LEADER)->sprite;
    sprite->image.priority = 1;
  }
  {
    struct Sprite *sprite = Actor_Get(ACTOR_GARET)->sprite;
    sprite->image.priority = 1;
  }
  {
    struct Sprite *sprite = Actor_Get(ACTOR_IVAN)->sprite;
    sprite->image.priority = 1;
  }
}

/// Once one of the suspects is boxed in for half a second in the guests' room
/// at Vault's inn, he cries out, Garet calls Ivan, and Ivan reads their minds
/// while they wonder what he is doing; the screen flashes green and purple.
static void LogRoomsA_ReadCorneredSuspectsMinds(void) {
  s32 actorId;
  struct FieldState *state;

  state = gFieldState;
  Event_Start();
  Task_Remove(LogRoomsA_UpdateCorneringChase);
  GameFlag_Clear(FLAG_MENUS_RAISE_TRIGGER);
  GameFlag_Clear(FLAG_VAULT_CORNERING_SUSPECTS);
  Actor_SetAnimation(ACTOR_SUSPECTED_LEADER, ANIM_STAND);
  Actor_SetAnimation(ACTOR_SUSPECTED_THIEF, ANIM_STAND);
  Actor_SetAnimation(ACTOR_IVAN, ANIM_STAND);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Actor_SetAnimation(ACTOR_GARET, ANIM_STAND);
  Actor_FaceActor(ACTOR_GARET, ACTOR_IVAN, 0);
  Actor_FaceEachOther(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
  Actor_FaceActor(ACTOR_SUSPECTED_LEADER, ACTOR_IVAN, 0);
  Actor_FaceActor(ACTOR_SUSPECTED_THIEF, ACTOR_IVAN, 0);
  Event_Wait(10);
  actorId = ACTOR_SUSPECTED_LEADER;
  // The event identifies the suspects blocked on the last frame of the wait.
  switch (state->raisedTrigger) {
  case LOG_ROOMS_A_THIEF_CORNERED:
  case LOG_ROOMS_A_BOTH_CORNERED:
    Event_SetMessageId(MsgVaultImSurrounded);
    Actor_SetAttachedEffect(ACTOR_SUSPECTED_THIEF,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Actor_RunRepeatedMotion(ACTOR_SUSPECTED_THIEF, 2);
    LogRoomsA_ShowMessageAndWait(ACTOR_SUSPECTED_THIEF, 20);
    if (state->raisedTrigger == LOG_ROOMS_A_THIEF_CORNERED) {
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
  Actor_SetAnimation(ACTOR_IVAN, ANIM_STAND);
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
  Scene_StartMindRead(ACTOR_IVAN, actorId);
  FieldPsynergy_RestoreMindReadPalette();
  Event_Wait(60);
  Actor_RunRepeatedMotion(ACTOR_SUSPECTED_LEADER, 2);
  LogRoomsA_ShowMessageAndWait(ACTOR_SUSPECTED_LEADER, 20);
  Actor_RunRepeatedMotion(ACTOR_SUSPECTED_THIEF, 2);
  LogRoomsA_ShowMessageAndWait(ACTOR_SUSPECTED_THIEF, 20);
  // "Are you done, Ivan?" He nods, and the window closes.
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  Scene_EndMindRead();
  Actor_StartRepeatedMotion(ACTOR_GARET, 2);
  Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Event_Wait(20);
  LogRoomsA_ShowMessageAndWait(ACTOR_GARET, 20);
  Actor_FaceActor(ACTOR_IVAN, ACTOR_GARET, 0);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  Event_Wait(40);
  LogRoomsA_SetPartyImagePriority();
  gFieldState->transitionFrames = 24;
  gFieldState->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  LogRoomsA_DiscussMindReading();
  // The suspects stay at their table, turned toward the leader from now on.
  MapGrid_CopyCellAttributes(14, 45, 3, 1, 14, 44);
  GameFlag_Set(FLAG_VAULT_SUSPECTS_MIND_READ);
  Actor_Get(ACTOR_SUSPECTED_LEADER)->timer = LEADER_SPOT_WEST_BED_FOOT;
  Actor_Get(ACTOR_SUSPECTED_THIEF)->timer = THIEF_SPOT_SOUTH_WALL;
  Task_AddFrame(LogRoomsA_FaceSuspectsTowardLeader, TASK_PASS_FRAME);
  gFieldState->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  Event_End();
}

/// Outside the guests' room after the Mind Read, Ivan tells what he learned:
/// it is them, and they hid the stolen goods somewhere in the inn. Garet says
/// they have to search it. A Yes gets Garet's leave it to us; a No, another
/// plea, from Garet and then Ivan, and after three Garet vouches for Isaac's
/// sense of duty.
static void LogRoomsA_DiscussMindReading(void) {
  s32 persistedWithQuestions;

  Scene_PlayMusic();
  Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 1);
  {
    struct Actor *leader = Actor_Get(ACTOR_PARTY_LEADER);

    leader->priorityFlags = ACTOR_PRIORITY_AUTOMATIC | leader->priorityFlags;
  }
  Field_SetCameraBounds(toFIXED(32), toFIXED(576), toFIXED(400), toFIXED(936));
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.8);
  Actor_SetSpeed(ACTOR_GARET, 0.8);
  Actor_SetSpeed(ACTOR_IVAN, 0.8);
  Actor_SetPosition(ACTOR_PARTY_LEADER, 248, 728);
  Actor_SetPosition(ACTOR_IVAN, 264, 744);
  Actor_SetPosition(ACTOR_GARET, 232, 744);
  LogRoomsA_SetPartyImagePriority();
  Actor_FaceEachOther(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
  LogRoomsA_FaceActorAndWait(ACTOR_GARET, ACTOR_IVAN, 30);
  Actor_SetPosition(ACTOR_SUSPECTED_LEADER, 104, 696);
  Actor_SetPosition(ACTOR_SUSPECTED_THIEF, 120, 696);
  Actor_FaceEachOther(ACTOR_SUSPECTED_LEADER, ACTOR_SUSPECTED_THIEF, 0);
  FieldCamera_FollowActor(ACTOR_PARTY_LEADER, 0);
  FieldCamera_WaitForMove();
  MapGrid_CenterOnFollowedPoint();
  Event_Wait(30);
  {
    struct FieldState *state = gFieldState;

    state->transitionFrames = 24;
    state->transition =
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
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
  LogRoomsA_FaceActorAndWait(ACTOR_GARET, ACTOR_IVAN, 20);
  LogRoomsA_PlayAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD, 20);
  Event_ShowMessage(ACTOR_IVAN, 0);
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
  Event_OpenMessage(ACTOR_GARET, 0);
  persistedWithQuestions = 0;
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) != 0) {
    Event_Wait(20);
    Actor_StartRepeatedMotion(ACTOR_IVAN, 2);
    LogRoomsA_PlayAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD, 30);
    Event_OpenMessage(ACTOR_GARET, 0);
    persistedWithQuestions = 0;
    if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) != 0) {
      Event_Wait(20);
      Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
      Event_Wait(60);
      Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
      LogRoomsA_FaceEachOtherAndWait(ACTOR_PARTY_LEADER, ACTOR_IVAN, 20);
      Event_OpenMessage(ACTOR_IVAN, 0);
      persistedWithQuestions = 0;
      if (Event_AskYesNoWithActorAnimation(ACTOR_IVAN, 0) != 0) {
        Event_Wait(20);
        Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
        Event_Wait(60);
        LogRoomsA_ShowMessageAndWait(ACTOR_IVAN, 20);
        LogRoomsA_FaceActorAndWait(ACTOR_GARET, ACTOR_IVAN, 10);
        Actor_RunRepeatedMotion(ACTOR_GARET, 1);
        Event_Wait(10);
        LogRoomsA_ShowMessageAndWait(ACTOR_GARET, 10);
        LogRoomsA_FaceActorAndWait(ACTOR_IVAN, ACTOR_GARET, 20);
        Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
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
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Event_Wait(60);
  Actor_RunRepeatedMotion(ACTOR_GARET, 1);
  LogRoomsA_ShowMessageAndWait(ACTOR_GARET, 10);
  LogRoomsA_PlayAnimationAndWait(ACTOR_GARET, ANIM_NOD, 10);
  Actor_FaceEachOther(ACTOR_GARET, ACTOR_IVAN, 0);
  LogRoomsA_FaceEachOtherAndWait(ACTOR_PARTY_LEADER, ACTOR_IVAN, 10);
  Event_ShowMessage(ACTOR_GARET, 0);
  LogRoomsA_PlayAnimationAndWait(ACTOR_IVAN, ANIM_NOD, 10);
  Actor_WalkTo(ACTOR_IVAN, 248, 728);
  Actor_WalkToAndWait(ACTOR_GARET, 248, 728);
  Actor_SetPosition(ACTOR_GARET, 0, 0);
  Actor_SetPosition(ACTOR_IVAN, 0, 0);
}

/// While the suspects are running from the party in the guests' room, Isaac
/// stepping on one of the spots at the heads and feet of the beds (cells
/// (4, 39) to (16, 43)) sends a suspect waiting at the matching spot on to the
/// next.
static void LogRoomsA_SuspectsRunFromParty(void) {
  switch (gFieldState->stepTrigger) {
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

/// Sends the suspect on a run when he waits at the expected spot, and numbers
/// the spot it ends at.
static void LogRoomsA_AdvanceSuspectRoute(s32 actorId, s32 expected, s32 next,
                                          const s32 *actions) {
  struct Actor *suspect = Actor_Get(actorId);

  if (suspect->timer == expected) {
    Actor_SetScript(actorId, actions);
    suspect->timer = next;
  }
}

#include "shared/vault_leader_beside_bone_flag.c"

/// Arriving in any room of Vault's houses, shops and inn sets the room up for
/// the story so far, and in the loft plays its events.
s32 Scene_Initialize(void) {
  gFieldState->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  switch (gMainRoutineState.entrance) {
  case LOG_ROOMS_A_FROM_VAULT_EAST_SOUTHEAST: {
    // The gem owner's house: he stands on the floor.
    Actor_Get(ACTOR_GEM_OWNER)->motionFlags = 0;
    Actor_Get(ACTOR_GEM_OWNER)->y.fixed = 0;
    Actor_Get(ACTOR_GEM_OWNER)->groundHeight = 0;
  } break;
  case LOG_ROOMS_A_FROM_VAULT_NORTHWEST:
    // The mayor's house: Ivan waits a step back once met and is gone once he
    // has joined; after the thieves are jailed, the mayor stands still,
    // turned south-southwest.
    if (GameFlag_Get(FLAG_VAULT_MET_IVAN) != 0) {
      Actor_SetPosition(ACTOR_IVAN, 376, 376);
      Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTH, 0);
    }
    if (GameFlag_Get(FLAG_VAULT_IVAN_JOINED_ROD_SEARCH) != 0) {
      Actor_SetPosition(ACTOR_IVAN, 0, 0);
    }
    if (GameFlag_Get(FLAG_VAULT_THIEVES_JAILED) != 0) {
      Actor_SetPosition(ACTOR_MAYOR, 536, MAYOR_AFTER_THIEVES_Z);
      Actor_SetMovementTypeById(ACTOR_MAYOR, MOVEMENT_TYPE_STAND);
      Actor_FaceDirection(ACTOR_MAYOR, FACING_SOUTH_SOUTHWEST, 0);
    }
    break;
  case LOG_ROOMS_A_FROM_VAULT_SOUTHWEST:
  case LOG_ROOMS_A_INN_FROM_INN_UPSTAIRS:
    // The inn: once the thieves are jailed, the Bone lies on the barrel beside
    // the girl at the inn; after the mind reading and before that, she is
    // out.
    if (GameFlag_Get(FLAG_VAULT_THIEVES_JAILED) != 0) {
      Actor_SetPosition(ACTOR_GIRL_AT_THE_INN, 568, 648);
      Actor_SetMovementTypeById(ACTOR_GIRL_AT_THE_INN, MOVEMENT_TYPE_STAND);
      Actor_FaceDirection(ACTOR_GIRL_AT_THE_INN, FACING_SOUTH, 0);
      Vault_PlaceItemIcon(ITEM_BONE, toFIXED(568), toFIXED(16), toFIXED(672));
      Task_AddFrame(Vault_UpdateLeaderBesideBoneFlag, TASK_PASS_FRAME);
    } else if (GameFlag_Get(FLAG_VAULT_SUSPECTS_MIND_READ) != 0) {
      Actor_SetPosition(ACTOR_GIRL_AT_THE_INN, 0, 0);
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
      Actor_SetPosition(ACTOR_SUSPECTED_THIEF, 120, 696);
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
      Actor_Get(ACTOR_SUSPECTED_LEADER)->timer = LEADER_SPOT_WEST_BED_FOOT;
      Actor_Get(ACTOR_SUSPECTED_THIEF)->timer = THIEF_SPOT_SOUTH_WALL;
      Task_AddFrame(LogRoomsA_FaceSuspectsTowardLeader, TASK_PASS_FRAME);
    }
    break;
  case LOG_ROOMS_A_FROM_VAULT_WEST:
  case LOG_ROOMS_A_CRATE_ROOM_FROM_LOFT:
    // The room with the crate: the crate keeps its place once moved, and the
    // passage behind it stays open. Cell (54, 2)'s blank picture covers what
    // its camera box takes in of the rooms beside it, (35, 20) to (36, 29) on
    // the west and (46, 21) to (49, 28) on the east, the loft's side, on the
    // layers that draw them (the middle one from column 60, the overhead one
    // from row 60): they show black (render probe scene LogRoomsA --entrance
    // 13 --map).
    Event_Wait(2);
    MapGrid_CopyCellsTo(54, 2, 35, 20, 2, 10);
    MapGrid_CopyCellsTo(54, 2, 95, 20, 2, 10);
    MapGrid_CopyCellsTo(54, 2, 35, 80, 2, 10);
    MapGrid_CopyCellsTo(54, 2, 46, 21, 4, 8);
    MapGrid_CopyCellsTo(54, 2, 46, 81, 4, 8);
    Actor_Get(ACTOR_CRATE)->collisionFlags |= ACTOR_NEAR_TALK;
    Actor_SetSpriteFlags(Actor_Get(ACTOR_CRATE), 0);
    // The cells LogRoomsA_CheckCrateMoved copies as it sets the flag, redrawn
    // on arrival.
    if (GameFlag_Get(FLAG_VAULT_CRATE_MOVED) != 0) {
      Actor_SetPosition(ACTOR_CRATE, 676, 411);
      MapGrid_CopyCellAttributes(101, 24, 3, 4, 41, 24);
    }
    break;
  case LOG_ROOMS_A_LOFT_FROM_CRATE_ROOM:
    // The loft's camera box takes in the crate room's east side, (44, 21) to
    // (45, 28): the blank cell covers it on the ground and overhead layers.
    Event_Wait(2);
    MapGrid_CopyCellsTo(54, 2, 44, 21, 2, 8);
    MapGrid_CopyCellsTo(54, 2, 44, 81, 2, 8);
    Event_Start();
    // The loft: emptied chests after the thieves are jailed; else the tied-up
    // villager lies tied, groaning as the music fades out, or the thieves wait
    // for a rematch.
    if (GameFlag_Get(FLAG_VAULT_THIEVES_JAILED) != 0) {
      Actor_SetAnimation(ACTOR_FIRST_CHEST, CHEST_ANIMATION_OPEN_EMPTY);
      Actor_SetAnimation(ACTOR_SECOND_CHEST, CHEST_ANIMATION_OPEN_EMPTY);
      Event_Wait(1);
      Actor_SetAnimation(ACTOR_THIRD_CHEST, CHEST_ANIMATION_OPEN_EMPTY);
      break;
    }
    Actor_SetPosition(ACTOR_TIED_UP_VILLAGER, 824, 456);
    if (GameFlag_Get(FLAG_VAULT_THIEVES_ATTACKED) != 0) {
      LogRoomsA_FightThievesAgain();
      Event_End();
      break;
    }
    Actor_SetAnimation(ACTOR_TIED_UP_VILLAGER,
                       YOUNG_MAN_WITH_BROWN_HAIR_ANIMATION_TIED_UP);
    LogRoomsA_OpenWindowTransition();
    Sound_Play(SOUND_FADE_OUT_BGM);
    Event_SetMessageId(MsgVaultUhnnUhnn);
    LogRoomsA_ShowMessageAndWait(ACTOR_TIED_UP_VILLAGER, 10);
    Event_End();
    break;
  case LOG_ROOMS_A_LOFT_AFTER_LOSING_TO_THIEVES:
    // Lost to the thieves: they gloat, and the party wakes up elsewhere. The
    // crate room's east side is covered as on the way in.
    MapGrid_CopyCells(54, 2, 2, 8, 44, 21);
    MapGrid_CopyCells(54, 2, 2, 8, 44, 81);
    Actor_SetPosition(ACTOR_TIED_UP_VILLAGER, 824, 456);
    LogRoomsA_LoseToThieves();
    Import_EventSetExitRequest(LOG_ROOMS_A_TO_SANCTUM);
    break;
  case LOG_ROOMS_A_LOFT_AFTER_THIEVES_DEFEATED:
    // Won: the thieves are caught and the stolen goods handed back. The crate
    // room's east side is covered as on the way in.
    MapGrid_CopyCells(54, 2, 2, 8, 44, 21);
    MapGrid_CopyCells(54, 2, 2, 8, 44, 81);
    if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0) {
      Actor_SetPosition(ACTOR_TIED_UP_VILLAGER, 824, 456);
      LogRoomsA_DefeatThieves();
      LogRoomsA_ReturnStolenGoods();
      break;
    }
    Actor_SetAnimation(ACTOR_FIRST_CHEST, CHEST_ANIMATION_OPEN_EMPTY);
    Actor_SetAnimation(ACTOR_SECOND_CHEST, CHEST_ANIMATION_OPEN_EMPTY);
    Event_Wait(1);
    Actor_SetAnimation(ACTOR_THIRD_CHEST, CHEST_ANIMATION_OPEN_EMPTY);
    break;
  }
  return 0;
}

/// Every frame while the suspects are running from the party, both keep
/// facing Isaac.
static void LogRoomsA_FaceSuspectsTowardLeader(void) {
  struct Actor *origin = Actor_Get(ACTOR_PARTY_LEADER);
  struct Actor *first = Actor_Get(ACTOR_SUSPECTED_LEADER);
  struct Actor *second = Actor_Get(ACTOR_SUSPECTED_THIEF);

  first->facing = Trig_Atan(origin->z.fixed - first->z.fixed,
                            origin->x.fixed - first->x.fixed);
  second->facing = Trig_Atan(origin->z.fixed - second->z.fixed,
                             origin->x.fixed - second->x.fixed);
}

/// After a lost battle with the thieves in the inn's loft, the party lies
/// collapsed while the thieves say these kids are nothing to worry about and
/// keep the tied-up young man as their hostage.
static void LogRoomsA_LoseToThieves(void) {
  s32 priorityFlags = ACTOR_PRIORITY_UNDERFOOT;

  // The party lies collapsed on the floor before the three thieves.
  Event_Start();
  Actor_SetPosition(ACTOR_THIEF_LEADER, 792, 416);
  Actor_SetPosition(ACTOR_FIRST_THIEF, 800, 400);
  Actor_SetPosition(ACTOR_SECOND_THIEF, 776, 416);
  Actor_FaceDirection(ACTOR_THIEF_LEADER, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_FIRST_THIEF, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_SECOND_THIEF, FACING_SOUTH_SOUTHEAST, 0);
  Actor_SetSpeed(ACTOR_FIRST_THIEF, 0.8);
  Actor_SetSpeed(ACTOR_SECOND_THIEF, 0.8);
  Actor_SetPosition(ACTOR_PARTY_LEADER, 784, 448);
  Actor_SetPosition(ACTOR_GARET, 808, 432);
  Actor_SetPosition(ACTOR_IVAN, 776, 432);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_COLLAPSE);
  Actor_SetAnimation(ACTOR_GARET, ANIM_COLLAPSE);
  Actor_SetAnimation(ACTOR_IVAN, ANIM_COLLAPSE);
  Actor_Get(ACTOR_PARTY_LEADER)->priorityFlags = priorityFlags;
  Actor_Get(ACTOR_GARET)->priorityFlags = priorityFlags;
  Actor_Get(ACTOR_IVAN)->priorityFlags = priorityFlags;
  Actor_SetSpriteFlags(Actor_Get(ACTOR_PARTY_LEADER), 0);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_IVAN), 0);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_GARET), 0);
  Actor_FaceDirection(ACTOR_TIED_UP_VILLAGER, FACING_NORTH_NORTHWEST, 0);
  gFieldState->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  FieldCamera_FollowActor(ACTOR_PARTY_LEADER, 0);
  FieldCamera_WaitForMove();
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

/// Coming back up into the inn's loft after losing to the thieves, they are
/// waiting: they complain the party won't go away, and attack again.
static void LogRoomsA_FightThievesAgain(void) {
  // The thieves stand over the tied-up villager as the party comes up.
  Actor_SetPosition(ACTOR_THIEF_LEADER, 792, 416);
  Actor_SetPosition(ACTOR_FIRST_THIEF, 800, 400);
  Actor_SetPosition(ACTOR_SECOND_THIEF, 776, 416);
  Actor_FaceDirection(ACTOR_THIEF_LEADER, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_FIRST_THIEF, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_SECOND_THIEF, FACING_SOUTH_SOUTHEAST, 0);
  Actor_SetPosition(ACTOR_PARTY_LEADER, 792, 440);
  Actor_SetPosition(ACTOR_GARET, 808, 432);
  Actor_SetPosition(ACTOR_IVAN, 792, 456);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceActor(ACTOR_TIED_UP_VILLAGER, ACTOR_THIEF_LEADER, 0);
  gFieldState->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  FieldCamera_FollowActor(ACTOR_PARTY_LEADER, 0);
  FieldCamera_WaitForMove();
  MapGrid_CenterOnFollowedPoint();
  wait(1);
  gFieldState->transitionFrames = 32;
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
  Actor_SetAnimation(ACTOR_THIEF_LEADER, ANIM_NOD);
  LogRoomsA_PlayAnimationAndWait(ACTOR_FIRST_THIEF, ANIM_NOD, 30);
  LogRoomsA_FaceEachOtherAndWait(ACTOR_THIEF_LEADER, ACTOR_SECOND_THIEF, 30);
  Actor_SetAnimation(ACTOR_THIEF_LEADER, ANIM_NOD);
  LogRoomsA_PlayAnimationAndWait(ACTOR_SECOND_THIEF, ANIM_NOD, 40);
  Actor_FaceDirection(ACTOR_THIEF_LEADER, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_FIRST_THIEF, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_SECOND_THIEF, FACING_SOUTH_SOUTHEAST, 0);
  Event_Wait(20);
  // "We'll make you sorry you were ever born! Get 'em!!" and the battle.
  LogRoomsA_PlayAnimationAndWait(ACTOR_THIEF_LEADER, ANIM_SHAKE_HEAD, 20);
  Event_ShowMessage(ACTOR_THIEF_LEADER, 0);
  gFieldState->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
  Event_SetAfterBattleDestination(SCENE_LOG_ROOMS_A,
                                  LOG_ROOMS_A_LOFT_AFTER_THIEVES_DEFEATED);
  Event_SetDefeatDestination(SCENE_LOG_ROOMS_A,
                             LOG_ROOMS_A_LOFT_AFTER_LOSING_TO_THIEVES);
  Battle_SetType(BATTLE_TYPE_SCRIPTED);
  Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED_VALE_VAULT, 5);
}

/// Counts the Venus Djinn Ivan holds, which he gives back before he leaves the
/// party.
static s32 LogRoomsA_GetIvanVenusDjinnCount(void) {
  return Unit_Get(IVAN)->djinnHeldCounts[ELEMENT_VENUS];
}

#include "shared/vault_djinn.c"

/// After the battle in the inn's loft, the beaten thieves lie gasping on the
/// floor. The freed young man runs for the mayor, who comes up with two
/// strongmen; the thieves are tied up.
static void LogRoomsA_DefeatThieves(void) {
  struct FieldState *state;
  const s32 *actions;
  struct Sprite *secondThiefSprite;

  secondThiefSprite = Actor_Get(ACTOR_SECOND_THIEF)->sprite;
  // The beaten thieves lie gasping before the party.
  Event_Start();
  Actor_SetPosition(ACTOR_THIEF_LEADER, 792, 416);
  Actor_SetPosition(ACTOR_FIRST_THIEF, 800, 400);
  Actor_SetPosition(ACTOR_SECOND_THIEF, 776, 392);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_THIEF_LEADER), 0);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_FIRST_THIEF), 0);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_SECOND_THIEF), 0);
  Actor_SetAnimation(ACTOR_THIEF_LEADER, ANIM_THIEF_BEATEN);
  Actor_SetAnimation(ACTOR_FIRST_THIEF, ANIM_THIEF_BEATEN);
  Actor_SetAnimation(ACTOR_SECOND_THIEF, ANIM_THIEF_BEATEN);
  Actor_Get(ACTOR_SECOND_THIEF)->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  secondThiefSprite->image.priority = 3;
  actions = gLogRoomsA_BeatenThiefGasps;
  Actor_SetScript(ACTOR_THIEF_LEADER, actions);
  Actor_SetPosition(ACTOR_PARTY_LEADER, 792, 440);
  Actor_SetPosition(ACTOR_GARET, 808, 432);
  Actor_SetPosition(ACTOR_IVAN, 776, 440);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH_NORTHWEST, 0);
  Actor_SetSpeed(ACTOR_TIED_UP_VILLAGER, 0.8);
  Actor_FaceDirection(ACTOR_TIED_UP_VILLAGER, FACING_NORTH_NORTHWEST, 0);
  gFieldState->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  FieldCamera_FollowActor(ACTOR_PARTY_LEADER, 0);
  FieldCamera_WaitForMove();
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
  Actor_SetSpeed(ACTOR_GARET, 0.8);
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
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
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
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  LogRoomsA_PlayAnimationAndWait(ACTOR_GARET, ANIM_NOD, 20);
  // Ivan reads Isaac's mind. Garet: "Shouldn't we stop him?" Yes: too late,
  // he's done. No: "If you say so."
  Scene_StartMindRead(ACTOR_IVAN, ACTOR_PARTY_LEADER);
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
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
    Event_Wait(20);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
    LogRoomsA_FaceActorAndWait(ACTOR_GARET, ACTOR_IVAN, 20);
    Scene_EndMindRead();
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Actor_RunRepeatedMotion(ACTOR_GARET, 1);
    Event_Wait(20);
    LogRoomsA_ShowMessageAndWait(ACTOR_GARET, 20);
  } else {
    gFieldState->message++;
    Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    Event_Wait(60);
    LogRoomsA_ShowMessageAndWait(ACTOR_GARET, 20);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
    LogRoomsA_FaceActorAndWait(ACTOR_GARET, ACTOR_IVAN, 20);
    Scene_EndMindRead();
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Actor_RunRepeatedMotion(ACTOR_GARET, 1);
    Event_Wait(20);
  }
  // "I see... So that's what happened..." The tied-up villager comes back with
  // the mayor and two strongmen.
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Event_Wait(60);
  LogRoomsA_PlayAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD, 20);
  Event_SetMessageId(MsgVaultSeeThatsHappened);
  LogRoomsA_ShowMessageAndWait(ACTOR_IVAN, 20);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
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
  Actor_SetSpeed(ACTOR_LOFT_MAYOR, 0.8);
  Actor_SetSpeed(ACTOR_LOFT_ESCORT, 0.8);
  Actor_SetSpeed(ACTOR_STRONGMAN_OPENING_THE_CHESTS, 0.8);
  Actor_SetPosition(ACTOR_LOFT_MAYOR, 744, 408);
  Actor_WalkToAndWait(ACTOR_LOFT_MAYOR, 768, 408);
  LogRoomsA_FaceActorAndWait(ACTOR_LOFT_MAYOR, ACTOR_THIEF_LEADER, 30);
  Actor_SetPosition(ACTOR_LOFT_ESCORT, 744, 408);
  Actor_WalkToAndWait(ACTOR_LOFT_ESCORT, 768, 408);
  Actor_SetPosition(ACTOR_STRONGMAN_OPENING_THE_CHESTS, 744, 408);
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
  // strongmen rouse the thieves to tie them up.
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
  Actor_SetAnimation(ACTOR_LOFT_ESCORT, ANIM_NOD);
  LogRoomsA_PlayAnimationAndWait(ACTOR_STRONGMAN_OPENING_THE_CHESTS, ANIM_NOD,
                                 20);
  LogRoomsA_FaceEachOtherAndWait(ACTOR_LOFT_ESCORT,
                                 ACTOR_STRONGMAN_OPENING_THE_CHESTS, 20);
  Actor_SetAnimation(ACTOR_LOFT_ESCORT, ANIM_NOD);
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
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
  LogRoomsA_PlayAnimationAndWait(ACTOR_IVAN, ANIM_NOD, 50);
  // The window closes while the thieves are tied up.
  state = gFieldState;
  state->transitionFrames = 30;
  state->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(60);
  Event_End();
}

/// With the thieves tied up, the mayor has them taken down the ladder to
/// jail. The chests give back his urn, the gold statue stolen from the
/// Sanctum and Master Hammet's Shaman's Rod; the mayor tells that Hammet was
/// kidnapped by Dodonpa of Lunpa, and Ivan takes the rod and leaves the
/// party to rescue him.
static void LogRoomsA_ReturnStolenGoods(void) {
  struct Actor *temporaryActor;
  struct Actor *actor;
  s32 aftermath;
  s32 rod;
  struct FieldState *state;

  state = gFieldState;
  GameFlag_Set(FLAG_VAULT_THIEVES_JAILED);
  // With the screen still closed, the chests go back along the wall and the
  // thieves stand tied before the mayor.
  Event_Start();
  {
    struct Actor *actor = Actor_Get(ACTOR_SECOND_THIEF);

    actor->priorityFlags = ACTOR_PRIORITY_AUTOMATIC | actor->priorityFlags;
  }
  Actor_MoveToAndWait(ACTOR_FIRST_CHEST, 872, 425);
  Actor_MoveToAndWait(ACTOR_SECOND_CHEST, 872, 409);
  Actor_MoveToAndWait(ACTOR_THIRD_CHEST, 872, 377);
  Actor_SetPosition(ACTOR_FIRST_THIEF, 776, 392);
  Actor_SetPosition(ACTOR_THIEF_LEADER, 792, 392);
  Actor_SetPosition(ACTOR_SECOND_THIEF, 808, 392);
  Actor_SetAnimation(ACTOR_THIEF_LEADER, ANIM_THIEF_TIED_UP);
  Actor_SetAnimation(ACTOR_FIRST_THIEF, ANIM_THIEF_TIED_UP);
  Actor_SetAnimation(ACTOR_SECOND_THIEF, ANIM_THIEF_TIED_UP);
  Actor_FaceActor(ACTOR_FIRST_THIEF, ACTOR_PARTY_LEADER, 0);
  Actor_FaceActor(ACTOR_THIEF_LEADER, ACTOR_PARTY_LEADER, 0);
  Actor_FaceActor(ACTOR_SECOND_THIEF, ACTOR_PARTY_LEADER, 0);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_THIEF_LEADER), SPRITE_FLAG_SHADOW);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_FIRST_THIEF), SPRITE_FLAG_SHADOW);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_SECOND_THIEF), SPRITE_FLAG_SHADOW);
  Actor_SetPosition(ACTOR_LOFT_ESCORT, 768, 408);
  Actor_SetPosition(ACTOR_STRONGMAN_OPENING_THE_CHESTS, 768, 424);
  Actor_MoveToAndWait(ACTOR_LOFT_MAYOR, 784, 424);
  Actor_SetPosition(ACTOR_TIED_UP_VILLAGER, 808, 408);
  Actor_FaceActor(ACTOR_LOFT_ESCORT, ACTOR_LOFT_MAYOR, 0);
  Actor_FaceActor(ACTOR_TIED_UP_VILLAGER, ACTOR_LOFT_MAYOR, 0);
  Actor_FaceActor(ACTOR_STRONGMAN_OPENING_THE_CHESTS, ACTOR_THIEF_LEADER, 0);
  Actor_FaceActor(ACTOR_LOFT_MAYOR, ACTOR_THIEF_LEADER, 0);
  Actor_SetPosition(ACTOR_PARTY_LEADER, 792, 440);
  Actor_SetPosition(ACTOR_GARET, 808, 440);
  Actor_SetPosition(ACTOR_IVAN, 776, 440);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_THIEF_LEADER, 0);
  Actor_FaceActor(ACTOR_GARET, ACTOR_THIEF_LEADER, 0);
  Actor_FaceActor(ACTOR_IVAN, ACTOR_THIEF_LEADER, 0);
  gFieldState->transitionFrames = 30;
  gFieldState->transition =
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
  Actor_SetAnimation(ACTOR_LOFT_ESCORT, ANIM_NOD);
  LogRoomsA_PlayAnimationAndWait(ACTOR_TIED_UP_VILLAGER, ANIM_NOD, 20);
  Actor_StartRepeatedMotion(ACTOR_FIRST_THIEF, 2);
  Actor_StartRepeatedMotion(ACTOR_SECOND_THIEF, 2);
  Event_Wait(60);
  // The escort leads the three thieves down the ladder, the tied-up villager
  // after them.
  Actor_SetSpeed(ACTOR_LOFT_ESCORT, 0.8);
  Actor_WalkTo(ACTOR_LOFT_ESCORT, 746, 408);
  Actor_FaceDirection(ACTOR_LOFT_MAYOR, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_STRONGMAN_OPENING_THE_CHESTS,
                      FACING_NORTH_NORTHWEST, 0);
  Actor_SetScript(ACTOR_FIRST_THIEF, gLogRoomsA_TiedThiefLeavesLoft);
  Event_Wait(20);
  Actor_SetScript(ACTOR_THIEF_LEADER, gLogRoomsA_TiedThiefLeavesLoft);
  Event_Wait(15);
  Actor_SetScript(ACTOR_SECOND_THIEF, gLogRoomsA_TiedThiefLeavesLoft);
  Event_Wait(35);
  Actor_SetScript(ACTOR_TIED_UP_VILLAGER, gLogRoomsA_VillagerLeavesLoft);
  Event_Wait(20);
  Actor_WaitForMoveById(ACTOR_LOFT_ESCORT);
  Actor_SetPosition(ACTOR_LOFT_ESCORT, 0, 0);
  Event_Wait(40);
  // The mayor and the strongman thank the party.
  Actor_SetSpeed(ACTOR_LOFT_MAYOR, 0.8);
  Actor_SetSpeed(ACTOR_STRONGMAN_OPENING_THE_CHESTS, 0.8);
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
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
  LogRoomsA_PlayAnimationAndWait(ACTOR_IVAN, ANIM_NOD, 50);
  // "Let's take a look at what they stole." The two go round to the chests.
  Actor_FaceEachOther(ACTOR_LOFT_MAYOR, ACTOR_STRONGMAN_OPENING_THE_CHESTS, 0);
  LogRoomsA_PlayAnimationAndWait(ACTOR_LOFT_MAYOR, ANIM_NOD, 20);
  LogRoomsA_ShowMessageAndWait(ACTOR_LOFT_MAYOR, 20);
  Actor_SetScript(ACTOR_STRONGMAN_OPENING_THE_CHESTS, gLogRoomsA_ToSecondChest);
  Event_Wait(50);
  Actor_SetScript(ACTOR_LOFT_MAYOR, gLogRoomsA_ToFirstChest);
  Actor_SetSpeed(ACTOR_GARET, 0.8);
  Actor_SetSpeed(ACTOR_IVAN, 0.8);
  Actor_WalkToAndWait(ACTOR_GARET, 792, 456);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH_NORTHEAST, 0);
  Event_Wait(30);
  Actor_WalkToAndWait(ACTOR_IVAN, 792, 408);
  Actor_FaceDirection(ACTOR_IVAN, FACING_EAST, 0);
  Actor_WalkToAndWait(ACTOR_GARET, 808, 456);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH_NORTHEAST, 0);
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
  Sound_Play(SE_CHEST_OPEN);
  Actor_SetAnimation(ACTOR_FIRST_CHEST, CHEST_ANIMATION_OPEN_ON_TREASURE);
  Actor_SetPosition(ACTOR_FOUND_URN, 872, 424);
  Actor_SetSpritePriority(ACTOR_FOUND_URN, 1);
  Actor_SetSpeed(ACTOR_FOUND_URN, 0.8);
  Actor_SetDestinationOffset(ACTOR_FOUND_URN, 0, -8);
  Actor_WaitForMoveById(ACTOR_FOUND_URN);
  Actor_RunRepeatedMotion(ACTOR_FOUND_URN, 2);
  Event_Wait(60);
  // "Found the mayor's precious urn."
  FieldMessage_Show((aftermath + 5), 1);
  Actor_SetAnimation(ACTOR_FIRST_CHEST, CHEST_ANIMATION_OPEN_EMPTY);
  Actor_SetPosition(ACTOR_FOUND_URN, 0, 0);
  gFieldState->message++;
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
  // The strongman opens the second: the gold statue stolen from the
  // Sanctum.
  Actor_RunRepeatedMotion(ACTOR_STRONGMAN_OPENING_THE_CHESTS, 2);
  Sound_Play(SE_CHEST_OPEN);
  Actor_SetAnimation(ACTOR_SECOND_CHEST, CHEST_ANIMATION_OPEN_ON_TREASURE);
  Actor_Get(ACTOR_FOUND_STATUE)->motionFlags = 0;
  Actor_SetSpritePriority(ACTOR_FOUND_STATUE, 1);
  Actor_SetPosition(ACTOR_FOUND_STATUE, 872, 408);
  Actor_SetSpeed(ACTOR_FOUND_STATUE, 0.8);
  Actor_SetDestinationOffset(ACTOR_FOUND_STATUE, 0, -8);
  Actor_WaitForMoveById(ACTOR_FOUND_STATUE);
  Actor_RunRepeatedMotion(ACTOR_FOUND_STATUE, 2);
  Event_Wait(60);
  // "Found a gold statue."
  FieldMessage_Show((aftermath + 8), 1);
  Actor_SetAnimation(ACTOR_SECOND_CHEST, CHEST_ANIMATION_OPEN_EMPTY);
  Actor_SetPosition(ACTOR_FOUND_STATUE, 0, 0);
  gFieldState->message++;
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
  // The strongman finds another chest; the mayor asks whether it is what Ivan
  // was looking for, and he can return to Master Hammet.
  Actor_FaceDirection(ACTOR_STRONGMAN_OPENING_THE_CHESTS,
                      FACING_NORTH_NORTHEAST, 0);
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
                      FACING_SOUTH_SOUTHWEST, 0);
  Event_Wait(30);
  LogRoomsA_FaceActorAndWait(ACTOR_IVAN, ACTOR_LOFT_MAYOR, 20);
  LogRoomsA_PlayAnimationAndWait(ACTOR_IVAN, ANIM_NOD, 20);
  LogRoomsA_ShowMessageAndWait(ACTOR_LOFT_MAYOR, 20);
  Actor_FaceEachOther(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
  LogRoomsA_FaceActorAndWait(ACTOR_GARET, ACTOR_IVAN, 20);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
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
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_SHAKE_HEAD);
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
  Actor_SetSpeed(ACTOR_IVAN, 1.5);
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
  Actor_FaceDirection(ACTOR_LOFT_MAYOR, FACING_SOUTH_SOUTHWEST, 0);
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
  // The mayor and the strongman leave down the ladder.
  Actor_WalkToAndWait(ACTOR_STRONGMAN_OPENING_THE_CHESTS, 856, 408);
  Actor_SetScript(ACTOR_LOFT_MAYOR, gLogRoomsA_BackFromChests);
  Actor_SetScript(ACTOR_STRONGMAN_OPENING_THE_CHESTS,
                  gLogRoomsA_BackFromChests);
  Actor_WalkTo(ACTOR_GARET, 792, 456);
  Actor_SetSpeed(ACTOR_IVAN, 0.8);
  Actor_WalkTo(ACTOR_IVAN, 776, 432);
  Actor_WaitForMoveById(ACTOR_GARET);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH_NORTHEAST, 0);
  Actor_WaitForMoveById(ACTOR_IVAN);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH_NORTHEAST, 0);
  Actor_WaitForScriptEndById(ACTOR_LOFT_MAYOR);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_LOFT_MAYOR, 0);
  temporaryActor = Actor_Get(ACTOR_STRONGMAN_OPENING_THE_CHESTS);
  {
    u8 *pauseFlag = &temporaryActor->paused;
    u8 paused = 1;

    *pauseFlag = paused;
  }
  temporaryActor->targetX = ACTOR_NO_TARGET;
  temporaryActor->targetY = ACTOR_NO_TARGET;
  temporaryActor->targetZ = ACTOR_NO_TARGET;
  Actor_ShowEmote(ACTOR_LOFT_MAYOR, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_SetAnimation(ACTOR_STRONGMAN_OPENING_THE_CHESTS, ANIM_STAND);
  Event_Wait(50);
  Actor_FaceActor(ACTOR_LOFT_MAYOR, ACTOR_PARTY_LEADER, 0);
  Actor_WalkToAndWait(ACTOR_GARET, 808, 440);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH_NORTHWEST, 0);
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
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_SHAKE_HEAD);
  LogRoomsA_PlayAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD, 30);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
  LogRoomsA_FaceActorAndWait(ACTOR_GARET, ACTOR_IVAN, 20);
  Actor_RunRepeatedMotion(ACTOR_GARET, 1);
  Event_Wait(20);
  LogRoomsA_ShowMessageAndWait(ACTOR_GARET, 40);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
  Event_Wait(20);
  LogRoomsA_ShowMessageAndWait(ACTOR_IVAN, 20);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
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
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) != 0) {
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
  temporaryActor = (struct Actor *)Chest_OpenAndShowItem(
      ACTOR_IVAN, ACTOR_THIRD_CHEST, ITEM_SHAMANS_ROD);
  Event_Wait(60);
  rod = MsgVaultIvanGotShamansRod;
  FieldMessage_Show(rod, 1);
  Actor_Release(temporaryActor);
  Actor_SetAnimation(ACTOR_THIRD_CHEST, CHEST_ANIMATION_OPEN_EMPTY);
  Event_Wait(20);
  LogRoomsA_PlayAnimationAndWait(ACTOR_IVAN, ANIM_NOD, 20);
  Actor_WalkToAndWait(ACTOR_IVAN, 856, 456);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
  Actor_FaceActor(ACTOR_GARET, ACTOR_IVAN, 0);
  Actor_WalkToAndWait(ACTOR_IVAN, 800, 456);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
  Actor_FaceActor(ACTOR_GARET, ACTOR_IVAN, 0);
  Actor_WalkToAndWait(ACTOR_IVAN, 776, 424);
  Actor_FaceEachOther(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
  LogRoomsA_FaceActorAndWait(ACTOR_GARET, ACTOR_IVAN, 30);
  // He thanks the town, hands back any Venus Djinni and leaves the party.
  LogRoomsA_PlayAnimationAndWait(ACTOR_IVAN, ANIM_NOD, 20);
  Event_SetMessageId(rod + 1);
  LogRoomsA_ShowMessageAndWait(ACTOR_IVAN, 20);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  {
    s32 saved = (s16)state->message;

    if (LogRoomsA_GetIvanVenusDjinnCount() != 0) {
      Event_SetMessageId(MsgVaultWaitDontWantTakeYour);
      // "Oh, wait, I don't want to take your Djinni. I'll just leave..."
      Event_ShowMessage(ACTOR_IVAN, 0);
      Vault_ReturnIsaacsDjinni();
    }
    Party_RemoveMemberRestored(IVAN);
    state->message = saved;
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
  Actor_SetAnimation(ACTOR_GARET, ANIM_WALK);
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  if (actor != NULL) {
    Actor_SetDestination(ACTOR_GARET, actor->x.part[0].integer,
                         actor->z.part[0].integer);
  }
  Actor_WaitForMoveById(ACTOR_GARET);
  Actor_SetPosition(ACTOR_GARET, 0, 0);
  Event_Wait(30);
  Actor_SetPosition(ACTOR_TIED_UP_VILLAGER, 0, 0);
  Actor_SetPosition(ACTOR_LOFT_MAYOR, 0, 0);
  Actor_SetPosition(ACTOR_LOFT_ESCORT, 0, 0);
  Actor_SetPosition(ACTOR_STRONGMAN_OPENING_THE_CHESTS, 0, 0);
  Actor_SetPosition(ACTOR_THIEF_LEADER, 0, 0);
  Actor_SetPosition(ACTOR_FIRST_THIEF, 0, 0);
  Actor_SetPosition(ACTOR_SECOND_THIEF, 0, 0);
  Actor_SetPosition(ACTOR_IVAN, 0, 0);
  gFieldState->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  Event_End();
}

/// Opens the window transition and waits one frame after it finishes.
static void LogRoomsA_OpenWindowTransition(void) {
  gFieldState->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(1);
}

static void LogRoomsA_ShowMessageAndWait(s32 actorId, s32 waitFrames) {
  Event_ShowMessage(actorId, 0);
  Event_Wait(waitFrames);
}

static void LogRoomsA_FaceActorAndWait(s32 actorId, s32 targetId,
                                       s32 waitFrames) {
  Actor_FaceActor(actorId, targetId, 0);
  Event_Wait(waitFrames);
}

static void LogRoomsA_FaceEachOtherAndWait(s32 firstId, s32 secondId,
                                           s32 waitFrames) {
  Actor_FaceEachOther(firstId, secondId, 0);
  Event_Wait(waitFrames);
}

/// Plays the animation, waits until it has taken over, then waits waitFrames.
static void LogRoomsA_PlayAnimationAndWait(s32 actorId, s32 animation,
                                           s32 waitFrames) {
  Import_ActorSetAnimation(actorId, animation, waitFrames);
  Actor_WaitForAnimationChange(actorId);
  Event_Wait(waitFrames);
}

#include "shared/mind_read.c"

#include "shared/vault_item_icon.c"

/// The chase's stop under a position (its x, y and z), or NULL between
/// stops.
static struct LogRoomsAStop *LogRoomsA_FindStopAtPosition(s32 *position) {
  s32 gridX = FIELD_HALF_CELL(position[0] - toFIXED(LOG_STOPS_ORIGIN_X));
  s32 gridZ = FIELD_HALF_CELL(position[2] - toFIXED(LOG_STOPS_ORIGIN_Z));
  struct LogRoomsAStop *stop = gLogRoomsA_Stops;
  struct LogRoomsAStop *result = NULL;
  u32 i;

  for (i = 0; i < LOG_STOP_COUNT; i++, stop++) {
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

/// The stop linked to this one in the direction nearest requestedFacing,
/// which is turned to that link's heading; NULL when it has no link.
static struct LogRoomsAStop *
LogRoomsA_ChooseStopTowardFacing(struct LogRoomsAStop *node,
                                 s16 *requestedFacing) {
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
  nearestDifference = DEGtoANG(180);
  for (connectionIndex = 0; connectionIndex <= 2;
       connectionIndex++, connection++) {
    connectionFacing = connection->facing << 8;
    angleDifference = (s16)(connectionFacing - *requestedFacing);
    if (angleDifference < 0)
      angleDifference = -angleDifference;
    if (connection->stopIndex != LOG_STOP_NONE &&
        angleDifference < nearestDifference) {
      nearestDifference = angleDifference;
      /* FAKEMATCH(dead): updates of chosenFacing that cancel out, under a test
       * of it against the difference. */
      if (chosenFacing != angleDifference) {
        chosenFacing++;
        chosenFacing--;
      }
      stopIndex = connection->stopIndex;
      chosenFacing = (s16)connectionFacing;
    }
  }
  if (stopIndex == -1)
    return 0;
  *requestedFacing = chosenFacing;
  return &gLogRoomsA_Stops[stopIndex];
}

/// Whether the actor's destination, or where it stands when it has none, is
/// within half a cell (8 pixels) of the stop at (x, z).
static s32 LogRoomsA_IsWithinHalfCellOfStop(s32 x, s32 z, s32 id) {
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

  targetX = FIXEDtoINT(targetX - x);
  targetZ = FIXEDtoINT(targetZ - z);
  if (targetX * targetX + targetZ * targetZ <=
      (MAPGRID_CELL_PIXELS / 2) * (MAPGRID_CELL_PIXELS / 2))
    return 1;
  return 0;
}

/// Whether the actor's destination, or where it stands when it has none, is
/// within a cell (16 pixels) of the stop at (x, z).
static s32 LogRoomsA_IsWithinCellOfStop(s32 x, s32 z, s32 id) {
  struct Actor *actor = Actor_Lookup(id);
  s32 ax;
  s32 az;

  ax = actor->targetX;
  if (ax == ACTOR_NO_TARGET)
    ax = actor->x.fixed;
  az = actor->targetZ;
  if (az == ACTOR_NO_TARGET)
    az = actor->z.fixed;
  ax = FIXEDtoINT(ax - x);
  az = FIXEDtoINT(az - z);
  if (ax * ax + az * az <= MAPGRID_CELL_PIXELS * MAPGRID_CELL_PIXELS)
    return 1;
  return 0;
}

/// Whether a stop is taken: -1 when Isaac stands within a cell of it, or Ivan
/// or a suspect within half a cell; 1 when there is no stop; else 0.
static s32 LogRoomsA_CheckStopBlocked(struct LogRoomsAStop *stop) {
  FIXED x;
  FIXED z;

  if (stop == NULL) {
    return 1;
  }
  x = LOG_STOP_X(stop);
  z = LOG_STOP_Z(stop);
  if (LogRoomsA_IsWithinCellOfStop(x, z, ACTOR_PARTY_LEADER) != 0 ||
      LogRoomsA_IsWithinHalfCellOfStop(x, z, ACTOR_IVAN) != 0 ||
      LogRoomsA_IsWithinHalfCellOfStop(x, z, ACTOR_SUSPECTED_LEADER) != 0 ||
      LogRoomsA_IsWithinHalfCellOfStop(x, z, ACTOR_SUSPECTED_THIEF) != 0) {
    return -1;
  }
  return 0;
}

/// Sends the actor walking to the middle of the stop.
static void LogRoomsA_MoveActorToStop(struct Actor *actor,
                                      struct LogRoomsAStop *stop) {
  FieldActor_SetMoveTargetFX(actor, LOG_STOP_X(stop), 0, LOG_STOP_Z(stop));
}

/// Every frame of the chase in the guests' room, moves Ivan and the two
/// suspects on from stop to stop round the beds, and raises the cornering
/// event once a suspect has had nowhere to run for half a second.
static void LogRoomsA_UpdateCorneringChase(void) {
  struct Actor *leader;
  struct Actor *actor;
  struct FieldState *state;
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
  state = gFieldState;

  // Ivan runs ahead of the leader when the leader pushes toward him from
  // close by, and otherwise walks on the way he faces.
  actor = Actor_Lookup(ACTOR_IVAN);
  entry = LogRoomsA_FindStopAtPosition(&actor->x.fixed);
  if (entry != NULL && actor->targetX == ACTOR_NO_TARGET) {
    dx = actor->x.fixed - leader->x.fixed;
    dz = actor->z.fixed - leader->z.fixed;
    facing = leader->facing;
    angle = Trig_Atan(dz, dx);
    dx = FIXEDtoINT(dx);
    dz = FIXEDtoINT(dz);
    if (state->blockedSteps > 0 && dx * dx + dz * dz <= 20 * 20 &&
        (facingDifference = facing - angle) > -FACING_STEP &&
        facingDifference < FACING_STEP) {
      // Pushed toward from close by, he takes the leader's way.
      /* FAKEMATCH(dead): a dead store. */
      facingDifference = 0;
#if TBS_PATCH_1
    } else if (dx * dx + dz * dz > 8 * 8) {
#else
    } else {
#endif
      facing = actor->facing;
    }
    destination = LogRoomsA_ChooseStopTowardFacing(entry, &facing);
    if (LogRoomsA_CheckStopBlocked(destination) == 0) {
      LogRoomsA_MoveActorToStop(actor, destination);
      Actor_SelectAnimation(actor, ANIM_WALK);
    } else {
      Actor_SelectAnimation(actor, ANIM_STAND);
    }
  }

  // Each suspect tries a random turn of three sixteenths, the leader to
  // either side and the thief to either side or straight on, reckoned in
  // 16.16 so that it wraps round. Where someone stands he turns back, and
  // with both ways taken he shakes his head.
  actor = Actor_Lookup(ACTOR_SUSPECTED_LEADER);
  entry = LogRoomsA_FindStopAtPosition(&actor->x.fixed);
  if (entry != NULL && actor->targetX == ACTOR_NO_TARGET) {
    u32 turn = Random_Below(2);
    s32 currentFacing;

    turn *= INTtoFIXED(FACING_STEP * 6);
    currentFacing = actor->facing;
    facing = FIXEDtoINT(turn - INTtoFIXED(FACING_STEP * 3)) + currentFacing;
    destination = LogRoomsA_ChooseStopTowardFacing(entry, &facing);
    if (LogRoomsA_CheckStopBlocked(destination) != 0) {
      facing = actor->facing + DEGtoANG(180);
      destination = LogRoomsA_ChooseStopTowardFacing(entry, &facing);
      if (LogRoomsA_CheckStopBlocked(destination) == 0) {
        Actor_SetAttachedEffect(ACTOR_SUSPECTED_LEADER, ATTACHED_EFFECT_ONCE);
        goto leaderMoves;
      }
      Actor_SelectAnimation(actor, ANIM_SHAKE_HEAD);
      blocked = 1;
    } else {
    leaderMoves:
      LogRoomsA_MoveActorToStop(actor, destination);
      Actor_SelectAnimation(actor, ANIM_WALK);
    }
  }
  actor = Actor_Lookup(ACTOR_SUSPECTED_THIEF);
  entry = LogRoomsA_FindStopAtPosition(&actor->x.fixed);
  if (entry != NULL && actor->targetX == ACTOR_NO_TARGET) {
    u32 turn = Random_Below(3);
    s32 currentFacing;

    turn *= INTtoFIXED(FACING_STEP * 3);
    currentFacing = actor->facing;
    facing = FIXEDtoINT(turn - INTtoFIXED(FACING_STEP * 3)) + currentFacing;
    destination = LogRoomsA_ChooseStopTowardFacing(entry, &facing);
    if (LogRoomsA_CheckStopBlocked(destination) != 0) {
      facing = actor->facing + DEGtoANG(180);
      destination = LogRoomsA_ChooseStopTowardFacing(entry, &facing);
      if (LogRoomsA_CheckStopBlocked(destination) == 0) {
        Actor_SetAttachedEffect(ACTOR_SUSPECTED_THIEF, ATTACHED_EFFECT_ONCE);
        goto thiefMoves;
      }
      Actor_SelectAnimation(actor, ANIM_SHAKE_HEAD);
      blocked += 2;
    } else {
    thiefMoves:
      LogRoomsA_MoveActorToStop(actor, destination);
      Actor_SelectAnimation(actor, ANIM_WALK);
    }
  }

  // Only half a second with nowhere to run counts as cornered, naming who.
  if (blocked != 0) {
    if (++gLogRoomsA_CorneredFrames >= LOG_ROOMS_A_CORNERED_FRAMES)
      state->raisedTrigger = LOG_ROOMS_A_CORNERED_EVENT_BASE + blocked;
  } else {
    gLogRoomsA_CorneredFrames = 0;
  }
}

/// Sets the chase in the guests' room going: Isaac walks in, Garet and Ivan
/// step out beside him, Garet spreads his arms, and the camera is held on the
/// room.
static void LogRoomsA_SpreadOutToCornerSuspects(void) {
  Event_Start();
  FieldCamera_MoveTo(168, CAMERA_KEEP, 656, 1);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.8);
  Actor_SetSpeed(ACTOR_GARET, 0.8);
  Actor_SetSpeed(ACTOR_IVAN, 0.8);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 248, 696);
  Actor_SetPosition(ACTOR_GARET, 248, 696);
  Actor_SetPosition(ACTOR_IVAN, 248, 696);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 200, 696);
  Actor_WalkTo(ACTOR_GARET, 248, 712);
  Actor_WalkToAndWait(ACTOR_IVAN, 232, 696);
  Actor_WaitForMoveById(ACTOR_GARET);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_WEST, 0);
  Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
  Actor_SetAnimation(ACTOR_GARET, ANIM_GARET_ARMS_SPREAD);
  LogRoomsA_ResetSuspectRoutes();
  Field_SetCameraBounds(toFIXED(48), toFIXED(576), toFIXED(288), toFIXED(736));
  Actor_SetSpeed(ACTOR_GARET, 1);
  Actor_SetSpeed(ACTOR_IVAN, 0.75);
  Actor_SetSpeedAndAcceleration(ACTOR_SUSPECTED_LEADER, 1, 1.2);
  Actor_SetSpeedAndAcceleration(ACTOR_SUSPECTED_THIEF, 1.5, 1.5);
  gLogRoomsA_CorneredFrames = 0;
  Task_AddOrUpdateFrame(LogRoomsA_UpdateCorneringChase, TASK_PASS_FRAME + 20);
  // DEAD: the last system flag holds nothing, since no table, script or
  // routine sets or reads it; the ROM keeps the call that clears it.
  Event_ClearFlag(FLAG_SYSTEM_LAST);
  Event_End();
  Sound_Play(MUS_TRY_YOUR_LUCK);
}

// clang-format off
/// A link: the stop it leads to and the heading toward it, kept as the
/// facing's high byte. An unused link is LOG_STOP_NONE with heading 255.
#define NO_CONNECTION {LOG_STOP_NONE, 255, {0, 0}}
#define CONNECTION(stop, direction) {stop, (direction) >> 8, {0, 0}}
/// The route network's stops, each with its links to the others.
struct LogRoomsAStop gLogRoomsA_Stops[LOG_STOP_COUNT] = {
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

/// A beaten thief lying on the floor gasps twice, his sprite squashed and let
/// go, then lies still for three seconds, over and over.
s32 gLogRoomsA_BeatenThiefGasps[] = {
    SC_ADD(SW_SCALE_Y, (-FIXED_ONE / 16)),
    SC_ADD(SW_SCALE_X, (-FIXED_ONE / 256)),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_Y, (-FIXED_ONE / 16)),
    SC_ADD(SW_SCALE_X, (FIXED_ONE / 256)),
    SC_WAIT(2),
    SC_SET(SW_SCALE_Y, FIXED_ONE),
    SC_SET(SW_SCALE_X, FIXED_ONE),
    SC_WAIT(4),
    SC_ADD(SW_SCALE_Y, (-FIXED_ONE / 16)),
    SC_ADD(SW_SCALE_X, (-FIXED_ONE / 256)),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_Y, (-FIXED_ONE / 16)),
    SC_ADD(SW_SCALE_X, (FIXED_ONE / 256)),
    SC_WAIT(2),
    SC_SET(SW_SCALE_Y, FIXED_ONE),
    SC_SET(SW_SCALE_X, FIXED_ONE),
    SC_WAIT(180),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 gLogRoomsA_FirstThiefEntryScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(776, 0, 408),
    SC_SYNC,
    SC_MOVE(776, 0, 400),
    SC_SYNC,
    SC_MOVE(800, 0, 400),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_SET(SW_FACING, FACING_SOUTH_SOUTHEAST),
    SC_WAIT(20),
    SC_END,
};

s32 gLogRoomsA_SecondThiefEntryScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(776, 0, 408),
    SC_SYNC,
    SC_MOVE(776, 0, 416),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_SET(SW_FACING, FACING_SOUTH_SOUTHEAST),
    SC_WAIT(20),
    SC_END,
};

// The loft's walks once the thieves are beaten: the freed villager, and the
// thieves bound, go down the way to the room with the crate and are gone; the
// strongman and the mayor go round to the second and the first chest and come
// back from them.
s32 gLogRoomsA_VillagerLeavesLoft[] = {
    SC_SPEED(0.8),
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(776, 0, 408),
    SC_SYNC,
    SC_MOVE(744, 0, 408),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_POSITION(0, 0, 0),
    SC_END,
};

s32 gLogRoomsA_TiedThiefLeavesLoft[] = {
    SC_SPEED(0.8),
    SC_ANIMATION(ANIM_THIEF_TIED_UP_WALK),
    SC_MOVE(776, 0, 408),
    SC_SYNC,
    SC_MOVE(744, 0, 408),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_POSITION(0, 0, 0),
    SC_END,
};

s32 gLogRoomsA_ToSecondChest[] = {
    SC_SPEED(0.8),
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(792, 0, 408),
    SC_SYNC,
    SC_MOVE(808, 0, 440),
    SC_SYNC,
    SC_MOVE(808, 0, 456),
    SC_SYNC,
    SC_MOVE(840, 0, 456),
    SC_SYNC,
    SC_MOVE(856, 0, 408),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(10),
    SC_END,
};

s32 gLogRoomsA_ToFirstChest[] = {
    SC_SPEED(0.8),
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(808, 0, 408),
    SC_SYNC,
    SC_MOVE(808, 0, 456),
    SC_SYNC,
    SC_MOVE(840, 0, 456),
    SC_SYNC,
    SC_MOVE(856, 0, 424),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(10),
    SC_END,
};

s32 gLogRoomsA_BackFromChests[] = {
    SC_SPEED(0.8),
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(840, 0, 456),
    SC_SYNC,
    SC_MOVE(808, 0, 456),
    SC_SYNC,
    SC_MOVE(808, 0, 408),
    SC_SYNC,
    SC_MOVE(776, 0, 408),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_END,
};

// The suspects' runs about the guests' room, named by the spot each ends at
// among the room's three beds: the head of each bed (z 632), its foot
// (z 696), beside the outer beds (x 72 and 264, z 648) and the room's two
// south corners. A Round run goes
// about the end of the outer bed, an Across run over the beds' row, an Along
// run past the feet of the beds. The suspects' timers number the spots
// (LogRoomsA_SuspectsRunFromParty): the leader's 1 to 3 the heads and 4 to 6 the
// feet, west to east; the thief's 1 and 2 beside the west and east beds, 3
// and 4 the south-west and south-east corners.
s32 gLogRoomsA_SuspectToWestBedHead[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(104, 0, 632),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_END,
};

s32 gLogRoomsA_SuspectRoundToWestBedHead[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(72, 0, 696),
    SC_SYNC,
    SC_MOVE(80, 0, 640),
    SC_SYNC,
    SC_MOVE(104, 0, 632),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_END,
};

s32 gLogRoomsA_SuspectToMiddleBedHead[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(168, 0, 632),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_END,
};

s32 gLogRoomsA_SuspectToEastBedHead[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(232, 0, 632),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_END,
};

s32 gLogRoomsA_SuspectRoundToEastBedHead[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(264, 0, 696),
    SC_SYNC,
    SC_MOVE(264, 0, 632),
    SC_SYNC,
    SC_MOVE(232, 0, 632),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_END,
};

s32 gLogRoomsA_SuspectToWestBedFoot[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(104, 0, 696),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_END,
};

s32 gLogRoomsA_SuspectRoundToWestBedFoot[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(80, 0, 640),
    SC_SYNC,
    SC_MOVE(72, 0, 696),
    SC_SYNC,
    SC_MOVE(104, 0, 696),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_END,
};

s32 gLogRoomsA_SuspectToMiddleBedFoot[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(168, 0, 696),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_END,
};

s32 gLogRoomsA_SuspectToEastBedFoot[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(232, 0, 696),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_END,
};

s32 gLogRoomsA_SuspectRoundToEastBedFoot[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(264, 0, 632),
    SC_SYNC,
    SC_MOVE(264, 0, 696),
    SC_SYNC,
    SC_MOVE(232, 0, 696),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_END,
};

s32 gLogRoomsA_SuspectAcrossToWestOfBeds[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(168, 0, 648),
    SC_SYNC,
    SC_MOVE(72, 0, 648),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_END,
};

s32 gLogRoomsA_SuspectToWestOfBeds[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(72, 0, 648),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_END,
};

s32 gLogRoomsA_SuspectAcrossToEastOfBeds[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(168, 0, 648),
    SC_SYNC,
    SC_MOVE(264, 0, 648),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_END,
};

s32 gLogRoomsA_SuspectToEastOfBeds[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(264, 0, 648),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_END,
};

s32 gLogRoomsA_SuspectToSouthWestCorner[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(72, 0, 696),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_END,
};

s32 gLogRoomsA_SuspectAlongToSouthWestCorner[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(120, 0, 696),
    SC_SYNC,
    SC_MOVE(72, 0, 696),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_END,
};

s32 gLogRoomsA_SuspectToSouthEastCorner[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(264, 0, 696),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_END,
};

s32 gLogRoomsA_SuspectAlongToSouthEastCorner[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(168, 0, 696),
    SC_SYNC,
    SC_MOVE(264, 0, 696),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
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
    ACTOR_PLACEMENT(ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        8, 0, 8, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(ACTOR_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        376, 0, 392, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        160, 0, 136, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_WANDER_NEAR,
        152, 0, 72, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        456, 0, 184, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_WANDER,
        368, 0, 120, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_IN_VIEW, MOVEMENT_TYPE_WANDER,
        744, 0, 200, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        752, 0, 128, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        496, 0, 416, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_WITH_PIGTAILS, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        512, 0, 416, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_WHITE_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        536, 0, 451, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        416, 0, 464, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_WANDER,
        536, 0, 672, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BALD_MAN_IN_RED, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        200, 0, 392, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_IN_GREEN, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        88, 0, 392, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_PONYTAIL, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        632, 0, 136, FACING_SOUTH, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        664, 0, 632, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PINK_APRON, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        696, 0, 696, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_LONG_DARK_HAIR, FLAG_VAULT_THIEVES_ATTACKED, MOVEMENT_TYPE_STAND,
        104, 0, 696, FACING_EAST, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(SPRITE_PERSON_IN_PURPLE, FLAG_VAULT_THIEVES_ATTACKED, MOVEMENT_TYPE_STAND,
        200, 0, 680, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_CRATE, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        692, 0, 411, FACING_SOUTH, TALK_FACING_KEEP, 0),
    { SCENE_TABLE_END },
};

struct ActorPlacement gLogRoomsA_LoftActorPlacements[] = {
    ACTOR_PLACEMENT(ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        8, 0, 8, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(ACTOR_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        376, 0, 392, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        160, 0, 136, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_WHITE_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        504, 0, 456, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_LONG_DARK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        104, 0, 696, FACING_EAST, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(SPRITE_PERSON_IN_PURPLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        200, 0, 680, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_PERSON_IN_PURPLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        8, 0, 8, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        8, 0, 8, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        8, 0, 8, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_CHEST, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        872, 0, 424, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_CHEST, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        872, 0, 408, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_CHEST, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        872, 0, 376, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_URN, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        8, 0, 8, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SANCTUM_GOLD_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        8, 0, 8, FACING_SOUTH, TALK_FACING_PARTY, 0),
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
    { TRIGGER_RAISED, FIELD_RAISED_TRIGGER_MENUS, CONDITION_ALWAYS, EVENT(LogRoomsA_GaretHasNoTimeForMenu) },
    { TRIGGER_STEP, CELL_BEHAVIOR_LOG_ROOMS_A_LOFT_WAY_IN, CONDITION_ALWAYS, EVENT(LogRoomsA_EnterLoft) },
    { STEP_TRIGGER_WALKING, CELL_BEHAVIOR_LOG_ROOMS_A_ATTIC_FLOOR_HOLE, CONDITION_ALWAYS, EVENT(LogRoomsA_JumpAcrossHoleInAtticFloor) },
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
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GEM_OWNERS_WIFE, CONDITION_ALWAYS, EVENT(LogRoomsA_ReadGemOwnersWifesMind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_SUSPECTING_THE_NEIGHBOR, CONDITION_ALWAYS, MSGNO(MsgVaultHeyMaybeMyNeighborIsThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_ASKING_ABOUT_TRAVEL, CONDITION_ALWAYS, EVENT(LogRoomsA_ReadBoysMind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GRANDPA, CONDITION_ALWAYS, EVENT(LogRoomsA_ReadGrandpasMind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GRANDPAS_WIFE, CONDITION_ALWAYS, MSGNO(MsgVaultYouNeverBoughtMeAnythingWorth) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_WISHING_TO_BE_OLDER, CONDITION_ALWAYS, MSGNO(MsgVaultTheseGuysLookReliableIGuess) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_TELLING_A_SECRET, CONDITION_ALWAYS, MSGNO(MsgVaultIWonderWhatSortOfThing) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAYOR, CONDITION_ALWAYS, EVENT(LogRoomsA_ReadMayorMind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAYORS_WIFE, CONDITION_ALWAYS, MSGNO(MsgVaultMaybeTheyllFindOurStolenFamily) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_AT_THE_INN, CONDITION_ALWAYS, EVENT(LogRoomsA_ReadInnGirlsMind) },
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
    { TRIGGER_TABLE_END },
};

struct Trigger gLogRoomsA_LoftTriggers[] = {
    { TRIGGER_STEP, CELL_BEHAVIOR_LOG_ROOMS_A_LOFT_WAY_OUT, CONDITION_ALWAYS, EVENT(LogRoomsA_LeaveLoft) },
    { TRIGGER_TALK, ACTOR_TIED_UP_VILLAGER, CONDITION_ALWAYS, EVENT(LogRoomsA_FreeVillagerAndFightThieves) },
    { TRIGGER_TALK, ACTOR_FIRST_CHEST, CONDITION_ALWAYS, EVENT(LogRoomsA_SearchEmptyChest) },
    { TRIGGER_TALK, ACTOR_SECOND_CHEST, CONDITION_ALWAYS, EVENT(LogRoomsA_SearchEmptyChest) },
    { TRIGGER_TALK, ACTOR_THIRD_CHEST, CONDITION_ALWAYS, EVENT(LogRoomsA_SearchEmptyChest) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TIED_UP_VILLAGER, CONDITION_ALWAYS, MSGNO(MsgVaultAhhHelpMe) },
    { TRIGGER_TABLE_END },
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
    { TRIGGER_STEP, CELL_BEHAVIOR_LOG_ROOMS_A_LOFT_WAY_IN, CONDITION_ALWAYS, EVENT(LogRoomsA_EnterLoft) },
    { TRIGGER_STEP, CELL_BEHAVIOR_LOG_ROOMS_A_LOFT_WAY_OUT, CONDITION_ALWAYS, EVENT(LogRoomsA_LeaveLoft) },
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
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GEM_OWNERS_WIFE, CONDITION_ALWAYS, EVENT(LogRoomsA_ReadGemOwnersWifesMind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_SUSPECTING_THE_NEIGHBOR, CONDITION_ALWAYS, MSGNO(MsgVaultTheReasonLunpaTurnedBadIs) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_ASKING_ABOUT_TRAVEL, CONDITION_ALWAYS, EVENT(LogRoomsA_ReadBoysMind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GRANDPA, CONDITION_ALWAYS, EVENT(LogRoomsA_ReadGrandpasMind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GRANDPAS_WIFE, CONDITION_ALWAYS, MSGNO(MsgVaultIWishIHadSomethingWorth) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_WISHING_TO_BE_OLDER, CONDITION_ALWAYS, MSGNO(MsgVaultWhenIGrowUpIWant) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_TELLING_A_SECRET, CONDITION_ALWAYS, MSGNO(MsgVaultIWishIHadWingsLike) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAYOR, CONDITION_ALWAYS, EVENT(LogRoomsA_ReadMayorMind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAYORS_WIFE, CONDITION_ALWAYS, MSGNO(MsgVaultImSoRelievedNowThatWe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_AT_THE_INN, CONDITION_ALWAYS, EVENT(LogRoomsA_ReadInnGirlsMind) },
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
    { TRIGGER_TABLE_END },
};
// clang-format on

/// The suspected leader's runs when the party talks to him, by his timer:
/// on round the beds, from the heads' row eastward, or back the other way.
s32 *gLogRoomsA_LeaderRunsOn[] = {
    gLogRoomsA_SuspectToMiddleBedHead,    gLogRoomsA_SuspectToEastBedHead,
    gLogRoomsA_SuspectRoundToEastBedFoot, gLogRoomsA_SuspectToMiddleBedFoot,
    gLogRoomsA_SuspectToWestBedFoot,      gLogRoomsA_SuspectRoundToWestBedHead,
};
s32 *gLogRoomsA_LeaderRunsBack[] = {
    gLogRoomsA_SuspectRoundToWestBedFoot, gLogRoomsA_SuspectToWestBedHead,
    gLogRoomsA_SuspectToMiddleBedHead,    gLogRoomsA_SuspectRoundToEastBedHead,
    gLogRoomsA_SuspectToEastBedFoot,      gLogRoomsA_SuspectToMiddleBedFoot,
};

/// The suspected thief's runs when the party talks to him, four to a row:
/// LogRoomsA_SuspectedThiefDodges picks the row by which way he is pushed and
/// the run by his timer.
s32 *gLogRoomsA_ThiefRuns[] = {
    gLogRoomsA_SuspectAcrossToEastOfBeds,
    gLogRoomsA_SuspectToSouthEastCorner,
    gLogRoomsA_SuspectAlongToSouthWestCorner,
    gLogRoomsA_SuspectToWestOfBeds,
    gLogRoomsA_SuspectToSouthWestCorner,
    gLogRoomsA_SuspectAcrossToWestOfBeds,
    gLogRoomsA_SuspectToEastOfBeds,
    gLogRoomsA_SuspectAlongToSouthEastCorner,
};

u16 gLogRoomsA_CorneredFrames;
