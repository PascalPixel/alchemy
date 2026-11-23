/// @file
/// Stucco rooms B (SceneId_StuccoRoomsB), a sheet of rooms drawn in stucco, all
/// of them Tolbi's.
///
/// They are its houses, inns and shops, which the area names call Tolbi, and
/// Tolbi Sanctum, as the same scene is named at STUCCO_ROOMS_B_SANCTUM and
/// STUCCO_ROOMS_B_SANCTUM_NORTH.
///
/// - Tolbi, the rooms behind the town's doors, which Tolbi's exits
///   TOLBI_TO_HOUSE_NORTHWEST to TOLBI_TO_HOUSE_SOUTHWEST lead into. This
///   scene's exits STUCCO_ROOMS_B_HOUSE_NORTHWEST_TO_TOLBI to
///   STUCCO_ROOMS_B_SANCTUM_TO_TOLBI lead back out. During the festival one
///   household lines up beds and becomes a makeshift inn, and visitors have
///   rented another house. The weapon, armor and item shops
///   (STUCCO_ROOMS_B_WEAPON_SHOP, STUCCO_ROOMS_B_ARMOR_SHOP and
///   STUCCO_ROOMS_B_ITEM_SHOP) open across their counters. The inn behind
///   TOLBI_TO_INN is full, `Inn Full / No Vacancies`.
///   Stairs lead down from it to its basement
///   (STUCCO_ROOMS_B_INN_TO_INN_PASSAGE and STUCCO_ROOMS_B_INN_PASSAGE_TO_INN),
///   a passage with two wooden doors in its north wall into the upper guest
///   rooms, and two doorways in its south wall into the lower ones
///   (STUCCO_ROOMS_B_INN_PASSAGE_TO_GUEST_ROOM_SOUTHWEST and the rest).
/// - Tolbi Sanctum, behind TOLBI_TO_SANCTUM (STUCCO_ROOMS_B_SANCTUM), whose
///   healer opens the sanctum across the counter. STUCCO_ROOMS_B_SANCTUM_NORTH
///   is where a defeat returns the party while Iodem waits at Gondowan Passage.
///
/// The rooms' talk follows Colosso in the same three stages as the town. Each
/// stage has its own trigger table (Scene_GetTriggers) and, after
/// Colosso, an actor placement table of its own.
///
/// - During the festival Babi is missing and the town is full of visitors,
///   among them a tour group from Kalay that reserved only two rooms.
/// - On finals day (FLAG_BABI_SAVED) Babi has shown himself at the finals.
///   The Spanish and Italian editions add a question about it (TBS_PATCH_4).
/// - After Colosso (FLAG_COLOSSO_OVER) no ship sails the Karagol, and the
///   visitors are stranded. One of them says they made it to Tolbi with the
///   party's help.
///
/// After Colosso two short events play, each once, on entering their
/// rooms (Scene_Initialize).
///
/// In the upper right guest room of the inn's basement a woman finds something
/// on the floor, starts at the sight of the party and hides it behind the trash
/// pile (StuccoRoomsB_WatchWomanHideMedal, FLAG_STUCCO_ROOMS_B_MEDAL_HIDDEN).
/// It is a Lucky Medal, which the party can then take from that barrel
/// (FLAG_STUCCO_ROOMS_B_MEDAL_TAKEN). After that she greets them, startled,
/// and Mind Read hears her hope its owner will not ask for it back.
///
/// In the lower right one, stranded passengers argue that the captain will not
/// sail although they agreed to do whatever it takes
/// (StuccoRoomsB_WatchPassengersArgue, an area flag).
///
/// A man who rode the ship from Kalay with the party greets it as
/// `the would-be warrior`. He and ACTOR_STUCCO_ROOMS_B_COLOSSO_FAN leave once
/// the party has watched the passengers confront the tour guide on the Tolbi
/// docks (FLAG_TOLBI_DOCKS_TOUR_GROUP_BLAMED).
///
/// Barrels hold a Vial and a Lucky Medal, and searching the ovens shows what
/// is cooking.

#include "ability_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "sounds.h"
#include "types.h"
#include "field/actor_script.h"
#include "field/event.h"
#include "field/scene_tables.h"
#include "scene/colosso_story_flags.h"
#include "system/main_routine_work.h"
#include "text/msg_ids.h"

extern struct SceneEntrance gStuccoRoomsBEntrances[];
extern u32 gStuccoRoomsBExits[];
extern struct ActorPlacement gStuccoRoomsBActorPlacements[];
extern struct ActorPlacement gStuccoRoomsBActorPlacementsAfterColosso[];
extern struct Trigger gStuccoRoomsBTriggersFestival[];
extern struct Trigger gStuccoRoomsBTriggersFinalsDay[];
extern struct Trigger gStuccoRoomsBTriggersAfterColosso[];
extern struct MapGridCellAnimation gStuccoRoomsBDoors[];

/// The placed actors, numbering the rows of the actor placement
/// table from ACTOR_FIRST_PLACED. Both tables hold the same people in the
/// same order up to the healer, except that after Colosso
/// (gStuccoRoomsBActorPlacementsAfterColosso) two others stand in the
/// rows of ACTOR_STUCCO_ROOMS_B_SLEEPLESS_GUEST and
/// ACTOR_STUCCO_ROOMS_B_CROWD_SICK_GUEST, and six more rows follow the
/// healer.
enum StuccoRoomsBActorId {
  /// A young man with brown hair (SPRITE_YOUNG_MAN_WITH_BROWN_HAIR) who runs a
  /// makeshift inn during the festival.
  ACTOR_STUCCO_ROOMS_B_MAKESHIFT_INNKEEPER = ACTOR_FIRST_PLACED,
  /// A woman with fair hair (SPRITE_WOMAN_WITH_FAIR_HAIR) who offers a stay.
  ACTOR_STUCCO_ROOMS_B_HOST,
  /// A strongman (SPRITE_STRONGMAN), during the festival.
  ACTOR_STUCCO_ROOMS_B_SLEEPLESS_GUEST,
  /// A stout man (SPRITE_STOUT_MAN), during the festival.
  ACTOR_STUCCO_ROOMS_B_CROWD_SICK_GUEST,
  /// An old woman (SPRITE_OLD_WOMAN); on finals day, "I'm stuck
  /// housesitting!"
  ACTOR_STUCCO_ROOMS_B_HOUSE_TENANT,
  /// A burly grey-haired man (SPRITE_BURLY_MAN_WITH_GREY_HAIR).
  ACTOR_STUCCO_ROOMS_B_FINALS_WATCHER,
  /// A small boy with fair hair (SPRITE_BOY_WITH_FAIR_HAIR).
  ACTOR_STUCCO_ROOMS_B_CRYING_BOY,
  /// A strongman (SPRITE_STRONGMAN). "I wonder if they really can't find
  /// Babi..."
  ACTOR_STUCCO_ROOMS_B_SOLDIER_WATCHER,
  /// A strongman (SPRITE_STRONGMAN). "Babi wasn't at Colosso according to all
  /// the spectators..."
  ACTOR_STUCCO_ROOMS_B_BABI_SKEPTIC,
  /// A stout man (SPRITE_STOUT_MAN).
  ACTOR_STUCCO_ROOMS_B_MERCHANT,
  /// A stout man (SPRITE_STOUT_MAN), after Colosso.
  ACTOR_STUCCO_ROOMS_B_TRAVELING_MERCHANT,
  /// A bald man in red (SPRITE_BALD_MAN_IN_RED) behind the weapon shop's
  /// counter (StuccoRoomsB_TalkToWeaponMerchant).
  ACTOR_STUCCO_ROOMS_B_WEAPON_MERCHANT,
  /// A young man in green (SPRITE_YOUNG_MAN_IN_GREEN) behind the armor shop's
  /// counter (StuccoRoomsB_TalkToArmorMerchant).
  ACTOR_STUCCO_ROOMS_B_ARMOR_MERCHANT,
  /// A young woman with a ponytail (SPRITE_WOMAN_WITH_PONYTAIL) behind the item
  /// shop's counter (StuccoRoomsB_TalkToItemMerchant).
  ACTOR_STUCCO_ROOMS_B_ITEM_MERCHANT,
  /// The inn's master, a man with dark hair (SPRITE_MAN_WITH_DARK_HAIR). His
  /// own thought: "I'm the one who really wants to skip work to watch the
  /// finals!"
  ACTOR_STUCCO_ROOMS_B_INN_MASTER,
  /// The inn's lady, a woman in a pink apron (SPRITE_WOMAN_IN_PINK_APRON).
  /// "This year I'm not letting anyone skip work for Colosso!"
  ACTOR_STUCCO_ROOMS_B_INN_LADY,
  /// A young man with brown hair (SPRITE_YOUNG_MAN_WITH_BROWN_HAIR).
  ACTOR_STUCCO_ROOMS_B_INN_WORKER,
  /// A woman with fair hair (SPRITE_WOMAN_WITH_FAIR_HAIR) who, after Colosso,
  /// hides the Lucky Medal she found in a barrel
  /// (FLAG_STUCCO_ROOMS_B_BARREL_LUCKY_MEDAL).
  ACTOR_STUCCO_ROOMS_B_MEDAL_WOMAN,
  /// The inn's cook (SPRITE_CHEF).
  ACTOR_STUCCO_ROOMS_B_CHEF,
  /// A young woman in green (SPRITE_YOUNG_WOMAN_IN_GREEN) waiting at the
  /// tables.
  ACTOR_STUCCO_ROOMS_B_WAITRESS,
  /// A young man with blue hair (SPRITE_YOUNG_MAN_WITH_BLUE_HAIR).
  ACTOR_STUCCO_ROOMS_B_LATE_ENTRANT,
  /// A young man with red hair (SPRITE_YOUNG_MAN_WITH_RED_HAIR).
  ACTOR_STUCCO_ROOMS_B_SECOND_LATE_ENTRANT,
  /// Tolbi Sanctum's healer, a man in a tall hat (SPRITE_MAN_IN_TALL_HAT),
  /// who opens the sanctum across his counter (StuccoRoomsB_TalkToHealer).
  ACTOR_STUCCO_ROOMS_B_HEALER,
  /// After Colosso, passengers stranded at an inn because no ship sails: the
  /// angry one, a man carrying a pack (SPRITE_MAN_CARRYING_A_PACK), his wife,
  /// an old woman (SPRITE_OLD_WOMAN), and a third, a woman in a green dress
  /// (SPRITE_WOMAN_IN_GREEN_DRESS).
  ACTOR_STUCCO_ROOMS_B_ANGRY_PASSENGER,
  ACTOR_STUCCO_ROOMS_B_PASSENGER_WIFE,
  ACTOR_STUCCO_ROOMS_B_THIRD_PASSENGER,
  /// A small boy with fair hair (SPRITE_BOY_WITH_FAIR_HAIR), after Colosso.
  ACTOR_STUCCO_ROOMS_B_BORED_BOY,
  /// After Colosso, a young man with brown hair
  /// (SPRITE_YOUNG_MAN_WITH_BROWN_HAIR) who rode the ship from Kalay with the
  /// party.
  ACTOR_STUCCO_ROOMS_B_FELLOW_PASSENGER,
  /// A woman with fair hair (SPRITE_WOMAN_WITH_FAIR_HAIR), after Colosso.
  ACTOR_STUCCO_ROOMS_B_COLOSSO_FAN,
  /// A burly grey-haired man (SPRITE_BURLY_MAN_WITH_GREY_HAIR), after Colosso,
  /// in ACTOR_STUCCO_ROOMS_B_SLEEPLESS_GUEST's row.
  ACTOR_STUCCO_ROOMS_B_STRANDED_TRAVELER = ACTOR_STUCCO_ROOMS_B_SLEEPLESS_GUEST,
  /// A woman in a green dress (SPRITE_WOMAN_IN_GREEN_DRESS), after Colosso, in
  /// ACTOR_STUCCO_ROOMS_B_CROWD_SICK_GUEST's row. "The tourists are all in a
  /// fix, because the ships aren't sailing."
  ACTOR_STUCCO_ROOMS_B_CAVE_RUMOR_GUEST = ACTOR_STUCCO_ROOMS_B_CROWD_SICK_GUEST
};

/// The cell behaviors the rooms' coord and examine triggers answer: the
/// things the party can search, and the four wooden doors of the inn's
/// basement. A door's cell carries the number of the exit it leaves by, from
/// STUCCO_ROOMS_B_INN_PASSAGE_TO_GUEST_ROOM_NORTHWEST to
/// STUCCO_ROOMS_B_GUEST_ROOM_SOUTHWEST_TO_INN_PASSAGE, so the trigger rows name
/// the exits, and gStuccoRoomsBDoors lists the doors in that order.
enum StuccoRoomsBCellBehavior {
  CELL_BEHAVIOR_STUCCO_ROOMS_B_FIRST_DOOR =
      STUCCO_ROOMS_B_INN_PASSAGE_TO_GUEST_ROOM_NORTHWEST,
  CELL_BEHAVIOR_STUCCO_ROOMS_B_INN_SIGN = 40,
  CELL_BEHAVIOR_STUCCO_ROOMS_B_VIAL_BARREL = 100,
  CELL_BEHAVIOR_STUCCO_ROOMS_B_MEDAL_BARREL = 101,
  CELL_BEHAVIOR_STUCCO_ROOMS_B_HIDDEN_MEDAL_BARREL =
      102, ///< where the woman hid her find
  CELL_BEHAVIOR_STUCCO_ROOMS_B_PIZZA_OVEN = 200,
  CELL_BEHAVIOR_STUCCO_ROOMS_B_PASTA_OVEN = 201,
  CELL_BEHAVIOR_STUCCO_ROOMS_B_SEAFOOD_PASTA_OVEN = 202,
};

// Area flags: Scene_Arrive clears 0x300 to 0x3ff when the
// party enters another area, and other areas use the numbers for
// their own.
/// The stranded passengers' argument has played.
#define FLAG_STUCCO_ROOMS_B_PASSENGERS_SEEN FLAG_AREA_TEMP_00

/// From TBS_PATCH_1 on, the camera stops 8 pixels further in the inn's
/// corridor (INN) and its guest rooms, the southern pair lying east on the map
/// (EAST_ROOM) and the northern pair south of it (SOUTH_ROOM). Renders of
/// tbs-ja and tbs-en show what it fixes: before it, the last 8 pixels of these
/// rooms stayed off screen, cutting off the corridor's south openings, the
/// southern rooms' east walls and the northern rooms' doorways.
#if !TBS_PATCH_1
#define STUCCO_ROOMS_B_INN_CAMERA_MAX_Z 736
#define STUCCO_ROOMS_B_EAST_ROOM_CAMERA_MAX_X 944
#define STUCCO_ROOMS_B_SOUTH_ROOM_CAMERA_MAX_Z 928
#else
#define STUCCO_ROOMS_B_INN_CAMERA_MAX_Z 744
#define STUCCO_ROOMS_B_EAST_ROOM_CAMERA_MAX_X 952
#define STUCCO_ROOMS_B_SOUTH_ROOM_CAMERA_MAX_Z 936
#endif

/// Tolbi's shops, as Shop_Run numbers their stock.
enum {
  STUCCO_ROOMS_B_WEAPON_SHOP = 26,
  STUCCO_ROOMS_B_ARMOR_SHOP = 27,
  STUCCO_ROOMS_B_ITEM_SHOP = 28
};

/// Asks a yes/no question, then answers with the message after the question
/// for yes or the one after that for no.
static inline void StuccoRoomsB_AskYesNo(s32 question, s32 speaker) {
  Event_SetMessageId(question);
  Event_OpenMessage(speaker, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
    Event_Wait(10);
    Event_SetMessageId(question + 1);
  } else {
    Event_SetMessageId(question + 2);
  }
  Event_ShowMessage(speaker, 0);
}

const struct SceneEntrance *Scene_GetEntrances(void) {
  return gStuccoRoomsBEntrances;
}

const struct SceneExitRegion *Scene_GetExitRegions(void) { return NULL; }

const u32 *Scene_GetExits(void) { return gStuccoRoomsBExits; }

/// After Colosso the rooms hold a different crowd.
const struct ActorPlacement *Scene_GetActorPlacements(void) {
  if (GameFlag_Get(FLAG_COLOSSO_OVER) != 0) {
    return gStuccoRoomsBActorPlacementsAfterColosso;
  }
  return gStuccoRoomsBActorPlacements;
}

#if TBS_PATCH_4
/// On finals day, in the Spanish and Italian editions, the strongman in the
/// storeroom of hay bales and barrels asks Isaac whether Babi's disappearance
/// was a misunderstanding, now that Babi has shown himself at the finals. Yes:
/// it must have been, since he is at the Colosseum now. No: so they found him
/// in time for the finals? Ridiculous!
///
/// The other editions skip the question and keep only the yes answer, "Well, I
/// guess so, since he's in the colosseum now."
static void StuccoRoomsB_AskWhetherBabiWasMissing(s32 speaker) {
  StuccoRoomsB_AskYesNo(MsgTolbiInteriorsBabiShowedHimselfAtFinals, speaker);
}
#endif

/// Picks the trigger table for the current point in the Colosso story.
const struct Trigger *Scene_GetTriggers(void) {
  if (GameFlag_Get(FLAG_COLOSSO_OVER) != 0) {
    return gStuccoRoomsBTriggersAfterColosso;
  }
  if (GameFlag_Get(FLAG_BABI_SAVED) != 0) {
    return gStuccoRoomsBTriggersFinalsDay;
  }
  return gStuccoRoomsBTriggersFestival;
}

/// Unused: leaves through TRANSITION_WINDOW, a screen transition of 24
/// frames.
///
/// No event row names it.
UNUSED static void StuccoRoomsB_ExitThroughWindow(void) {
  struct FieldWork *work = gFieldWork;

  work->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
  work->transitionFrames = 24;
  Import_EventSetExitRequest();
}

/// Isaac walks north into one of the four wooden doors of Tolbi's inn
/// basement, down the stairs from the inn, and it opens as he walks through:
/// the two in the north wall of the passage (cells (24, 42) and (33, 42)),
/// into the guest rooms north of it, or the door in the north wall of either
/// guest room behind the passage's south doorways (cells (50, 41) and
/// (52, 51)), back out to the passage.
static void StuccoRoomsB_EnterDoor(void) {
  u32 actorId;
  struct Actor *actor;
  struct FieldWork *work;
  s16 *trigger;
  s32 doorIndex;

  work = gFieldWork;
  for (actorId = ACTOR_FIRST_PLACED; actorId < ACTOR_ID_COUNT; actorId++) {
    actor = Actor_Get(actorId);
    if (actor != NULL) {
      actor->motionFlags = 0;
    }
  }
  trigger = &work->stepTrigger;
  doorIndex = *trigger - CELL_BEHAVIOR_STUCCO_ROOMS_B_FIRST_DOOR;
  Sound_Play(SE_DOOR_OPEN);
  MapGrid_AnimateCells(gStuccoRoomsBDoors[doorIndex].steps,
                       gStuccoRoomsBDoors[doorIndex].x,
                       gStuccoRoomsBDoors[doorIndex].y);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.5);
  Actor_Get(ACTOR_PARTY_LEADER)->motionFlags = 0;
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
  Import_EventSetExitRequest(*trigger);
}

/// The first time the party enters the upper right guest room of the inn's
/// basement after Colosso, through the passage's east door, a woman with fair
/// hair finds something sparkling on the floor, starts at the sight of them,
/// and hides it behind the trash pile. It is a Lucky Medal, in the barrel there
/// (cell (35, 52)).
static void StuccoRoomsB_WatchWomanHideMedal(void) {
  struct Actor *sparkle;

  Event_Start();
  Event_SetMessageId(MsgTolbiHeyWhatsThis);

  // A sparkle on the floor where she finds it.
  Event_Wait(40);
  sparkle = FieldActor_Create(SPRITE_PARTICLE, toFIXED(600), 0, toFIXED(824));
  Actor_SetSpriteFlags(sparkle, 0);
  Actor_SelectAnimation(sparkle, 6);
  Event_Wait(10);
  Actor_SelectAnimation(sparkle, 1);
  Event_Wait(40);
  Actor_Release(sparkle);
  Event_Wait(2);

  // She runs to it and picks it up.
  Actor_ShowEmote(ACTOR_STUCCO_ROOMS_B_MEDAL_WOMAN,
                  EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 50);
  Actor_SetSpeed(ACTOR_STUCCO_ROOMS_B_MEDAL_WOMAN, 1);
  Actor_WalkToAndWait(ACTOR_STUCCO_ROOMS_B_MEDAL_WOMAN, 600, 848);
  Actor_FaceDirection(ACTOR_STUCCO_ROOMS_B_MEDAL_WOMAN, FACING_NORTH, 0);
  Event_Wait(40);
  // "Hey, what's this?"
  Event_ShowMessage(ACTOR_STUCCO_ROOMS_B_MEDAL_WOMAN, 0);
  Actor_RunRepeatedMotion(ACTOR_STUCCO_ROOMS_B_MEDAL_WOMAN, 2);
  Event_Wait(30);
  Actor_WalkToAndWait(ACTOR_STUCCO_ROOMS_B_MEDAL_WOMAN, 568, 848);
  Actor_FaceDirection(ACTOR_STUCCO_ROOMS_B_MEDAL_WOMAN, FACING_NORTH, 0);
  Event_Wait(30);

  // She notices the party.
  Actor_ShowEmote(ACTOR_STUCCO_ROOMS_B_MEDAL_WOMAN,
                  EMOTE_IN_FRONT | EMOTE_SMILE, 50);
  Event_Wait(20);
  Actor_WalkByAndWait(ACTOR_PARTY_LEADER, 0, -16);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_STUCCO_ROOMS_B_MEDAL_WOMAN, FACING_SOUTH_SOUTHEAST,
                      0);
  Event_Wait(30);
  Actor_RunRepeatedMotion(ACTOR_STUCCO_ROOMS_B_MEDAL_WOMAN, 2);
  Event_Wait(20);
  // "Uh-oh."
  Event_ShowMessage(ACTOR_STUCCO_ROOMS_B_MEDAL_WOMAN, 0);
  Event_Wait(20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 50);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_STUCCO_ROOMS_B_MEDAL_WOMAN, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  // "I didn't find anything!"
  Event_ShowMessage(ACTOR_STUCCO_ROOMS_B_MEDAL_WOMAN, 0);
  Event_Wait(30);
  Actor_ShowEmote(ACTOR_STUCCO_ROOMS_B_MEDAL_WOMAN,
                  EMOTE_IN_FRONT | EMOTE_WINCE, 50);
  // "I was just doing some work I...had to do."
  Event_ShowMessage(ACTOR_STUCCO_ROOMS_B_MEDAL_WOMAN, 0);

  // She hides it and comes back, then leaves.
  Actor_SetSpeed(ACTOR_STUCCO_ROOMS_B_MEDAL_WOMAN, 1.4);
  Actor_WalkByAndWait(ACTOR_STUCCO_ROOMS_B_MEDAL_WOMAN, 16, 0);
  Actor_WalkByAndWait(ACTOR_STUCCO_ROOMS_B_MEDAL_WOMAN, 0, 32);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_STUCCO_ROOMS_B_MEDAL_WOMAN, ANIM_NOD);
  Event_Wait(20);
  // "It's rude to startle people!"
  Event_ShowMessage(ACTOR_STUCCO_ROOMS_B_MEDAL_WOMAN, 0);
  Actor_WalkByAndWait(ACTOR_PARTY_LEADER, 16, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  Event_Wait(20);
  Actor_SetSpeed(ACTOR_STUCCO_ROOMS_B_MEDAL_WOMAN, 1.8);
  Actor_WalkByAndWait(ACTOR_STUCCO_ROOMS_B_MEDAL_WOMAN, 0, 48);
  Actor_SetPosition(ACTOR_STUCCO_ROOMS_B_MEDAL_WOMAN, 0, 0);
  Event_End();
}

/// The first time the party enters the lower right guest room of the inn's
/// basement after Colosso, through the passage's south-east doorway (cell
/// (33, 46)), three stranded passengers argue that the captain won't sail the
/// ship, though they agreed to do whatever it takes.
static void StuccoRoomsB_WatchPassengersArgue(void) {
  Event_Start();
  Event_SetMessageId(MsgTolbiHeWontSailShipEven);
  Event_Wait(30);
  Actor_Jump(ACTOR_STUCCO_ROOMS_B_ANGRY_PASSENGER, 4, 13);
  Actor_Jump(ACTOR_STUCCO_ROOMS_B_ANGRY_PASSENGER, 4, 30);
  // "He won't sail the ship, even though we agreed to do whatever..."
  Event_ShowMessage(ACTOR_STUCCO_ROOMS_B_ANGRY_PASSENGER, 0);
  Event_Wait(10);

  Actor_ShowEmote(ACTOR_STUCCO_ROOMS_B_PASSENGER_WIFE,
                  EMOTE_IN_FRONT | EMOTE_WINCE, 50);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_STUCCO_ROOMS_B_PASSENGER_WIFE, ANIM_NOD);
  Event_Wait(30);
  // "Co-Correct. The ships cannot go out for any reason."
  Event_ShowMessage(ACTOR_STUCCO_ROOMS_B_PASSENGER_WIFE, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_STUCCO_ROOMS_B_THIRD_PASSENGER,
                            ANIM_SHAKE_HEAD);
  Event_Wait(20);
  // "What kind of person would refuse such an offer?"
  Event_ShowMessage(ACTOR_STUCCO_ROOMS_B_THIRD_PASSENGER, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_STUCCO_ROOMS_B_ANGRY_PASSENGER, 2);
  Event_Wait(20);
  // "You tell the captain again that he needs to set sail tomorrow."
  Event_ShowMessage(ACTOR_STUCCO_ROOMS_B_ANGRY_PASSENGER, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_STUCCO_ROOMS_B_PASSENGER_WIFE, ANIM_NOD);
  Event_Wait(30);
  Event_End();
}

/// After Colosso, sets the stranded passengers and the woman who hid the medal
/// where they now stand, and plays each guest room's short event the first
/// time the party walks in.
s32 Scene_Initialize(void) {
  struct Actor *actor;

  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  if (GameFlag_Get(FLAG_COLOSSO_OVER) != 0) {
    MapGrid_CopyCellAttributes(51, 47, 3, 1, 51, 45);
    actor = Actor_Get(ACTOR_STUCCO_ROOMS_B_ANGRY_PASSENGER);
    actor->priorityFlags = 0;
    actor->sprite->image.priority = 2;
    actor = Actor_Get(ACTOR_STUCCO_ROOMS_B_PASSENGER_WIFE);
    actor->priorityFlags = 0;
    actor->sprite->image.priority = 2;
    if (Event_GetFlag(FLAG_STUCCO_ROOMS_B_MEDAL_HIDDEN) != 0) {
      Actor_SetPosition(ACTOR_STUCCO_ROOMS_B_MEDAL_WOMAN, 560, 680);
      Actor_FaceDirection(ACTOR_STUCCO_ROOMS_B_MEDAL_WOMAN, FACING_WEST, 0);
    }
    // Each short event plays once, only when entering its room.
    if (gMainRoutineWork.entrance ==
        STUCCO_ROOMS_B_GUEST_ROOM_NORTHEAST_FROM_INN_PASSAGE) {
      if (Event_GetFlag(FLAG_STUCCO_ROOMS_B_MEDAL_HIDDEN) == 0) {
        GameFlag_Set(FLAG_STUCCO_ROOMS_B_MEDAL_HIDDEN);
        Event_StartScreenTransition();
        StuccoRoomsB_WatchWomanHideMedal();
      }
    }
    if (gMainRoutineWork.entrance ==
        STUCCO_ROOMS_B_GUEST_ROOM_SOUTHEAST_FROM_INN_PASSAGE) {
      if (Event_GetFlag(FLAG_STUCCO_ROOMS_B_PASSENGERS_SEEN) == 0) {
        GameFlag_Set(FLAG_STUCCO_ROOMS_B_PASSENGERS_SEEN);
        Event_StartScreenTransition();
        StuccoRoomsB_WatchPassengersArgue();
      }
    }
    // Once the docks' confrontation (the refund) is seen, these two have left.
    if (GameFlag_Get(FLAG_TOLBI_DOCKS_TOUR_GROUP_BLAMED) != 0) {
      Actor_SetPosition(ACTOR_STUCCO_ROOMS_B_FELLOW_PASSENGER, 0, 0);
      Actor_SetPosition(ACTOR_STUCCO_ROOMS_B_COLOSSO_FAN, 0, 0);
    }
  }
  return 0;
}

/// In Tolbi, the item merchant sells or chats, by where the leader stands.
static void StuccoRoomsB_TalkToItemMerchant(s32 speaker) {
  struct Actor *leader = Actor_Get(ACTOR_PARTY_LEADER);
  if ((u16)FACING_NEAREST_CARDINAL(leader->facing) == FACING_WEST) {
    Shop_Run(STUCCO_ROOMS_B_ITEM_SHOP, speaker);
  } else if (GameFlag_Get(FLAG_COLOSSO_OVER) != 0) {
    Event_SetMessageId(MsgTolbiRightOneWell);
    // "You're Isaac, right? The one that did so well in the finals!..."
    Event_ShowMessage(speaker, 0);
  } else if (GameFlag_Get(FLAG_BABI_SAVED) != 0) {
    Event_SetMessageId(MsgTolbiRequireLotHealing);
    // "They require a lot of healing herbs once the finals begin...."
    Event_ShowMessage(speaker, 0);
  } else {
    // "Is this your first time to Tolbi?"
    // Yes: "Tolbi offers many chances to powerful warriors..."
    // No: "Tolbi is a fun place, no matter how many times you visit!..."
    StuccoRoomsB_AskYesNo(MsgTolbiFirstTimeTolbi, speaker);
  }
}

/// In Tolbi, the weapon merchant sells or chats, by where the leader stands.
static void StuccoRoomsB_TalkToWeaponMerchant(s32 speaker) {
  struct Actor *leader = Actor_Get(ACTOR_PARTY_LEADER);
  if ((u16)FACING_NEAREST_CARDINAL(leader->facing) == FACING_NORTH) {
    Shop_Run(STUCCO_ROOMS_B_WEAPON_SHOP, speaker);
  } else if (GameFlag_Get(FLAG_COLOSSO_OVER) != 0) {
    // "Did you come all the way from Kalay?"
    // Yes: "It's too bad you can't go back..."
    // No: "The people visiting from the east are all stuck at Karagol Sea
    // Port."
    StuccoRoomsB_AskYesNo(MsgTolbiComeWayKalay, speaker);
  } else if (GameFlag_Get(FLAG_BABI_SAVED) != 0) {
    Event_SetMessageId(MsgTolbiColossoFinalsFinally);
    // "So, the Colosso finals are finally here! Once Colosso ends,..."
    Event_ShowMessage(speaker, 0);
  } else {
    Event_SetMessageId(MsgTolbiLookStrongGo);
    // "You look strong... Go fight at Colosso!"
    Event_ShowMessage(speaker, 0);
    Actor_ShowEmote(speaker, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 0);
    Event_Wait(40);
    // "Hang on! Now that I think about it, the tryouts have already..."
    Event_ShowMessage(speaker, 0);
  }
}

/// In Tolbi, the armor merchant sells or chats, by where the leader stands.
static void StuccoRoomsB_TalkToArmorMerchant(s32 speaker) {
  struct Actor *leader = Actor_Get(ACTOR_PARTY_LEADER);
  if ((u16)FACING_NEAREST_CARDINAL(leader->facing) == FACING_NORTH) {
    Shop_Run(STUCCO_ROOMS_B_ARMOR_SHOP, speaker);
  } else {
    if (GameFlag_Get(FLAG_COLOSSO_OVER) != 0) {
      Event_SetMessageId(MsgTolbiWasntAbleWatch);
      // "I wasn't able to watch the finals this year! My mother says..."
      Event_ShowMessage(speaker, 0);
    } else if (GameFlag_Get(FLAG_BABI_SAVED) != 0) {
      Event_SetMessageId(MsgTolbiMaybeCloseShop);
      // "Hmm... Maybe I'll close the shop so I can go watch the finals."
      Event_ShowMessage(speaker, 0);
    } else {
      Event_SetMessageId(MsgTolbiIfCanMakeNameFor);
      // "If you can make a name for yourself at Colosso, you might..."
      Event_ShowMessage(speaker, 0);
    }
  }
}

/// In Tolbi, the chef asks whether the party watched Colosso, which he missed.
static void StuccoRoomsB_TalkToChef(s32 speaker) {
  Event_Start();
  // "You look like a warrior, but you were watching Colosso, right?"
  // Yes: "Tsk! Just watching doesn't make you any better than the next guy!..."
  // No: "Oh? Didn't even watch? What are you, crazy?..."
  StuccoRoomsB_AskYesNo(MsgTolbiLookLikeWarrior, speaker);
  Event_End();
}

/// In Tolbi after Colosso, the waitress grumbles about the chef's bad mood.
static void StuccoRoomsB_TalkToWaitress(s32 speaker) {
  Event_Start();
  Event_SetMessageId(MsgTolbiGrrrChefInBadMood);
  // "Grrr! The chef is in a bad mood because he didn't get to see..."
  Event_ShowMessage(speaker, 0);
  Event_End();
}

/// During the festival a woman with fair hair asks Isaac whether he wants to
/// stay with them, then admits the house is already full.
static void StuccoRoomsB_TalkToHost(s32 speaker) {
  Event_Start();
  // "Do you want to stay with us?"
  // Yes: "Actually, I'm sorry, but we're already full..."
  // No: "That's good... Our place is already full, anyway."
  StuccoRoomsB_AskYesNo(MsgTolbiWantStay, speaker);
  Event_End();
}

/// Isaac talks to the woman who hid the Lucky Medal in the upper right guest
/// room of the inn's basement. Until he takes it she giggles, "Hee hee! Do I
/// look like I'm that excited?"; afterwards she greets him, startled, "Oh...
/// Hello!", and says it's nothing.
static void StuccoRoomsB_TalkToMedalWoman(s32 speaker) {
  Event_Start();
  if (GameFlag_Get(FLAG_STUCCO_ROOMS_B_MEDAL_TAKEN) == 0) {
    // "Hee hee! Do I look like I'm that excited?"
    // Yes: "I see. That must be because the weather is nice."
    // No: "Yeah! I'm the same as always."
    StuccoRoomsB_AskYesNo(MsgTolbiHeeHeeLook, speaker);
  } else {
    if (Event_GetFlag(FLAG_STUCCO_ROOMS_B_MEDAL_WOMAN_SAID_HELLO) == 0) {
      GameFlag_Set(FLAG_STUCCO_ROOMS_B_MEDAL_WOMAN_SAID_HELLO);
      Event_SetMessageId(MsgTolbiHello);
      // "Oh... Hello!"
      Event_ShowMessage(speaker, 0);
      Event_Wait(10);
      Actor_RunRepeatedMotion(speaker, 2);
      Event_Wait(20);
    }
    Event_SetMessageId(MsgTolbiHoHumFine);
    // "Ho hum. I'm fine. It's nothing."
    Event_ShowMessage(speaker, 0);
  }
  Event_End();
}

/// Isaac reads the medal woman's mind: she just hid the thing she found behind
/// the trash pile, and nobody will ever know. Once he has taken it, she hopes
/// its owner won't ask her to return it.
static void StuccoRoomsB_ReadMedalWomanMind(void) {
  Event_Start();
  if (GameFlag_Get(FLAG_STUCCO_ROOMS_B_MEDAL_WOMAN_SAID_HELLO) == 0) {
    Event_SetMessageId(MsgTolbiHehHehSheJustHid);
  } else {
    Event_SetMessageId(MsgTolbiThingFoundDefinitelySameAs);
  }
  Event_ShowMessage(ACTOR_STUCCO_ROOMS_B_MEDAL_WOMAN, 0);
  Event_End();
}

/// In Tolbi after Colosso, the angry passenger complains the ships won't sail.
static void StuccoRoomsB_TalkToAngryPassenger(s32 speaker) {
  Event_Start();
  Event_SetMessageId(MsgTolbiGrrrScamWhyWontThey);
  Actor_ShowEmote(ACTOR_STUCCO_ROOMS_B_ANGRY_PASSENGER,
                  EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 40);
  // "Grrr! What a scam! Why won't they let the ships sail out..."
  Event_ShowMessage(speaker, 0);
  Event_End();
}

/// A man who rode the ship from Kalay with the party greets Isaac, "Hey!
/// You're the would-be warrior! Do you remember me?", and is stuck in Tolbi
/// with no way back.
static void StuccoRoomsB_TalkToFellowPassenger(s32 speaker) {
  Event_Start();
  // "Hey! You're the would-be warrior! Do you remember me?"
  // Yes: "You do? Wow! That's great! Neither of us can get back to Kalay..."
  // No: "It's me! We rode the same ship from Kalay! I guess you forgot."
  StuccoRoomsB_AskYesNo(MsgTolbiWarriorRemember, speaker);
  Event_End();
}

/// In Tolbi Sanctum, the healer heals or chats, by where the leader stands.
static void StuccoRoomsB_TalkToHealer(s32 speaker) {
  struct Actor *leader = Actor_Get(ACTOR_PARTY_LEADER);
  if ((u16)FACING_NEAREST_CARDINAL(leader->facing) == FACING_NORTH) {
    Sanctum_Run(speaker);
  } else {
    if (GameFlag_Get(FLAG_COLOSSO_OVER) != 0) {
      Event_SetMessageId(MsgTolbiShipsArentGoing);
      // "If the ships aren't going, why not forget heading east and..."
      Event_ShowMessage(speaker, 0);
    } else if (GameFlag_Get(FLAG_BABI_SAVED) != 0) {
      Event_SetMessageId(MsgTolbiMissFinalsTolbis);
      // "If you miss the finals at Tolbi's Festival, you'll regret..."
      Event_ShowMessage(speaker, 0);
    } else {
      Event_SetMessageId(MsgTolbiWasteStuckHereWhenSuch);
      // "What a waste to be stuck here when such a big festival's..."
      Event_ShowMessage(speaker, 0);
    }
  }
}

/// An actor that squashes and stretches twice, then rests 40 frames.
s32 gStuccoRoomsBSquashStretch[] = {
    SC_SET(SW_SCALE_X, FIXED_ONE),
    SC_SET(SW_SCALE_Y, FIXED_ONE),
    SC_ADD(SW_SCALE_X, -5 * FIXED_ONE / 16),
    SC_ADD(SW_SCALE_Y, FIXED_ONE / 16),
    SC_WAIT(1),
    SC_ADD(SW_SCALE_X, 5 * FIXED_ONE / 32),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 32),
    SC_WAIT(1),
    SC_ADD(SW_SCALE_X, 5 * FIXED_ONE / 64),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 64),
    SC_WAIT(1),
    SC_ADD(SW_SCALE_X, 5 * FIXED_ONE / 64),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 64),
    SC_WAIT(1),
    SC_ADD(SW_SCALE_X, -5 * FIXED_ONE / 16),
    SC_ADD(SW_SCALE_Y, FIXED_ONE / 16),
    SC_WAIT(1),
    SC_ADD(SW_SCALE_X, 5 * FIXED_ONE / 32),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 32),
    SC_WAIT(1),
    SC_ADD(SW_SCALE_X, 5 * FIXED_ONE / 64),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 64),
    SC_WAIT(1),
    SC_ADD(SW_SCALE_X, 5 * FIXED_ONE / 64),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 64),
    SC_WAIT(40),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

struct SceneEntrance gStuccoRoomsBEntrances[] = {
    {ENTRANCE_DEFAULT, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1,
     -1, -1, 0},
    {STUCCO_ROOMS_B_HOUSE_EAST, CONDITION_ALWAYS, 160, 0, 216, FACING_NORTH, 0,
     0, 0, 256, 256, 0},
    {STUCCO_ROOMS_B_HOUSE_NORTH, CONDITION_ALWAYS, 336, 0, 200, FACING_NORTH, 0,
     248, 64, 488, 216, 0},
    {STUCCO_ROOMS_B_HOUSE_SOUTHEAST, CONDITION_ALWAYS, 544, 0, 200,
     FACING_NORTH, 0, 520, 48, 696, 216, 0},
    {STUCCO_ROOMS_B_HOUSE_NORTHWEST, CONDITION_ALWAYS, 792, 0, 216,
     FACING_NORTH, 0, 688, 0, 960, 240, 0},
    {STUCCO_ROOMS_B_HOUSE_SOUTHWEST, CONDITION_ALWAYS, 544, 0, 424,
     FACING_NORTH, 0, 488, 264, 696, 448, 0},
    {STUCCO_ROOMS_B_WEAPON_AND_ARMOR_SHOP, CONDITION_ALWAYS, 312, 0, 472,
     FACING_NORTH, 0, 208, 272, 456, 488, 0},
    {STUCCO_ROOMS_B_SANCTUM, CONDITION_ALWAYS, 824, 0, 528, FACING_NORTH, 0,
     688, 248, 944, 568, 0},
    {STUCCO_ROOMS_B_INN, CONDITION_ALWAYS, 128, 0, 768, FACING_NORTH, 0, 0, 536,
     304, 800, 0},
    {STUCCO_ROOMS_B_INN_FROM_INN_PASSAGE, CONDITION_ALWAYS, 56, 0, 608,
     FACING_NORTH, 0, 0, 536, 304, 800, 0},
    {STUCCO_ROOMS_B_INN_PASSAGE_FROM_INN, CONDITION_ALWAYS, 464, 0, 624,
     FACING_SOUTH, 0, 320, 568, 616, STUCCO_ROOMS_B_INN_CAMERA_MAX_Z, 0},
    {STUCCO_ROOMS_B_INN_PASSAGE_FROM_GUEST_ROOM_SOUTHWEST, CONDITION_ALWAYS,
     392, 0, 728, FACING_NORTH, 0, 320, 568, 616,
     STUCCO_ROOMS_B_INN_CAMERA_MAX_Z, 0},
    {STUCCO_ROOMS_B_INN_PASSAGE_FROM_GUEST_ROOM_SOUTHEAST, CONDITION_ALWAYS,
     536, 0, 728, FACING_NORTH, 0, 320, 568, 616,
     STUCCO_ROOMS_B_INN_CAMERA_MAX_Z, 0},
    {STUCCO_ROOMS_B_INN_PASSAGE_FROM_GUEST_ROOM_NORTHWEST, CONDITION_ALWAYS,
     392, 0, 680, FACING_SOUTH, 0, 320, 568, 616,
     STUCCO_ROOMS_B_INN_CAMERA_MAX_Z, 0},
    {STUCCO_ROOMS_B_INN_PASSAGE_FROM_GUEST_ROOM_NORTHEAST, CONDITION_ALWAYS,
     536, 0, 680, FACING_SOUTH, 0, 320, 568, 616,
     STUCCO_ROOMS_B_INN_CAMERA_MAX_Z, 0},
    {STUCCO_ROOMS_B_GUEST_ROOM_SOUTHEAST_FROM_INN_PASSAGE, CONDITION_ALWAYS,
     808, 0, 664, FACING_SOUTH, 0, 704, 608,
     STUCCO_ROOMS_B_EAST_ROOM_CAMERA_MAX_X, 768, 0},
    {STUCCO_ROOMS_B_GUEST_ROOM_SOUTHWEST_FROM_INN_PASSAGE, CONDITION_ALWAYS,
     840, 0, 824, FACING_SOUTH, 0, 704, 768,
     STUCCO_ROOMS_B_EAST_ROOM_CAMERA_MAX_X, 928, 0},
    {STUCCO_ROOMS_B_GUEST_ROOM_NORTHWEST_FROM_INN_PASSAGE, CONDITION_ALWAYS,
     440, 0, 920, FACING_NORTH, 0, 288, 768, 528,
     STUCCO_ROOMS_B_SOUTH_ROOM_CAMERA_MAX_Z, 0},
    {STUCCO_ROOMS_B_GUEST_ROOM_NORTHEAST_FROM_INN_PASSAGE, CONDITION_ALWAYS,
     584, 0, 920, FACING_NORTH, 0, 496, 768, 736,
     STUCCO_ROOMS_B_SOUTH_ROOM_CAMERA_MAX_Z, 0},

    {STUCCO_ROOMS_B_SANCTUM_NORTH, CONDITION_ALWAYS, 824, 0, 352, FACING_NORTH,
     0, 688, 248, 944, 568, 0},
    {SCENE_TABLE_END},
};

u32 gStuccoRoomsBExits[] = {
    SCENE_EXITS(StuccoRoomsB),
    SCENE_EXIT(STUCCO_ROOMS_B_HOUSE_NORTHWEST_TO_TOLBI, Tolbi,
               TOLBI_FROM_HOUSE_NORTHWEST),
    SCENE_EXIT(STUCCO_ROOMS_B_HOUSE_NORTH_TO_TOLBI, Tolbi,
               TOLBI_FROM_HOUSE_NORTH),
    SCENE_EXIT(STUCCO_ROOMS_B_INN_TO_TOLBI, Tolbi, TOLBI_FROM_INN),
    SCENE_EXIT(STUCCO_ROOMS_B_UNUSED_TO_TOLBI, Tolbi, TOLBI_FROM_LUCKY_WHEELS),
    SCENE_EXIT(STUCCO_ROOMS_B_HOUSE_EAST_TO_TOLBI, Tolbi,
               TOLBI_FROM_HOUSE_EAST),
    SCENE_EXIT(STUCCO_ROOMS_B_HOUSE_SOUTHEAST_TO_TOLBI, Tolbi,
               TOLBI_FROM_HOUSE_SOUTHEAST),
    SCENE_EXIT(STUCCO_ROOMS_B_WEAPON_AND_ARMOR_SHOP_TO_TOLBI, Tolbi,
               TOLBI_FROM_WEAPON_AND_ARMOR_SHOP),
    SCENE_EXIT(STUCCO_ROOMS_B_HOUSE_SOUTHWEST_TO_TOLBI, Tolbi,
               TOLBI_FROM_HOUSE_SOUTHWEST),
    SCENE_EXIT(STUCCO_ROOMS_B_SANCTUM_TO_TOLBI, Tolbi, TOLBI_FROM_SANCTUM),
    SCENE_EXIT(STUCCO_ROOMS_B_INN_TO_INN_PASSAGE, StuccoRoomsB,
               STUCCO_ROOMS_B_INN_PASSAGE_FROM_INN),
    SCENE_EXIT(STUCCO_ROOMS_B_INN_PASSAGE_TO_INN, StuccoRoomsB,
               STUCCO_ROOMS_B_INN_FROM_INN_PASSAGE),
    SCENE_EXIT(STUCCO_ROOMS_B_INN_PASSAGE_TO_GUEST_ROOM_SOUTHWEST, StuccoRoomsB,
               STUCCO_ROOMS_B_GUEST_ROOM_SOUTHWEST_FROM_INN_PASSAGE),
    SCENE_EXIT(STUCCO_ROOMS_B_INN_PASSAGE_TO_GUEST_ROOM_SOUTHEAST, StuccoRoomsB,
               STUCCO_ROOMS_B_GUEST_ROOM_SOUTHEAST_FROM_INN_PASSAGE),
    SCENE_EXIT(STUCCO_ROOMS_B_INN_PASSAGE_TO_GUEST_ROOM_NORTHWEST, StuccoRoomsB,
               STUCCO_ROOMS_B_GUEST_ROOM_NORTHWEST_FROM_INN_PASSAGE),
    SCENE_EXIT(STUCCO_ROOMS_B_INN_PASSAGE_TO_GUEST_ROOM_NORTHEAST, StuccoRoomsB,
               STUCCO_ROOMS_B_GUEST_ROOM_NORTHEAST_FROM_INN_PASSAGE),
    SCENE_EXIT(STUCCO_ROOMS_B_GUEST_ROOM_SOUTHEAST_TO_INN_PASSAGE, StuccoRoomsB,
               STUCCO_ROOMS_B_INN_PASSAGE_FROM_GUEST_ROOM_SOUTHEAST),
    SCENE_EXIT(STUCCO_ROOMS_B_GUEST_ROOM_SOUTHWEST_TO_INN_PASSAGE, StuccoRoomsB,
               STUCCO_ROOMS_B_INN_PASSAGE_FROM_GUEST_ROOM_SOUTHWEST),
    SCENE_EXIT(STUCCO_ROOMS_B_GUEST_ROOM_NORTHWEST_TO_INN_PASSAGE, StuccoRoomsB,
               STUCCO_ROOMS_B_INN_PASSAGE_FROM_GUEST_ROOM_NORTHWEST),
    SCENE_EXIT(STUCCO_ROOMS_B_GUEST_ROOM_NORTHEAST_TO_INN_PASSAGE, StuccoRoomsB,
               STUCCO_ROOMS_B_INN_PASSAGE_FROM_GUEST_ROOM_NORTHEAST),
    SCENE_EXITS_END,
};

// clang-format off
struct ActorPlacement gStuccoRoomsBActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        200, 0, 168, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        152, 0, 184, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        184, 0, 104, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_STOUT_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        144, 0, 104, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        352, 0, 120, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        568, 0, 120, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        600, 0, 198, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        824, 0, 144, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        840, 0, 106, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_STOUT_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        608, 0, 348, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_STOUT_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        536, 0, 392, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BALD_MAN_IN_RED, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        280, 0, 360, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_IN_GREEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        392, 0, 360, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_PONYTAIL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        264, 0, 440, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_DARK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        106, 0, 696, FACING_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PINK_APRON, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        392, 0, 832, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        488, 0, 688, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        584, 0, 848, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_CHEF, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        232, 0, 592, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_WOMAN_IN_GREEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        230, 0, 648, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BLUE_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        864, 0, 848, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_RED_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        848, 0, 864, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_TALL_HAT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        824, 0, 328, FACING_SOUTH, TALK_FACING_PARTY, 0),
    { SCENE_TABLE_END },
};

struct ActorPlacement gStuccoRoomsBActorPlacementsAfterColosso[] = {
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        200, 0, 168, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        152, 0, 184, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        392, 0, 168, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        344, 0, 120, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        520, 0, 520, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        568, 0, 120, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        600, 0, 198, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        824, 0, 144, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        840, 0, 106, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_STOUT_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        608, 0, 348, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_STOUT_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        536, 0, 392, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BALD_MAN_IN_RED, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        280, 0, 360, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_IN_GREEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        392, 0, 360, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_PONYTAIL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        264, 0, 440, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_DARK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        106, 0, 696, FACING_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PINK_APRON, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        112, 0, 600, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        488, 0, 688, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        568, 0, 848, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_CHEF, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        232, 0, 592, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_WOMAN_IN_GREEN, CONDITION_ALWAYS, (s32)gStuccoRoomsBSquashStretch,
        230, 0, 648, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BLUE_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        864, 0, 848, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_RED_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        848, 0, 864, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_TALL_HAT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        824, 0, 328, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_CARRYING_A_PACK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        848, 0, 712, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        816, 0, 712, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        848, 0, 688, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        856, 0, 672, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        440, 0, 864, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        416, 0, 840, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    { SCENE_TABLE_END },
};

struct Trigger gStuccoRoomsBTriggersFestival[] = {
    { TRIGGER_EXIT, STUCCO_ROOMS_B_HOUSE_NORTHWEST_TO_TOLBI, CONDITION_ALWAYS, STUCCO_ROOMS_B_HOUSE_NORTHWEST_TO_TOLBI },
    { TRIGGER_EXIT, STUCCO_ROOMS_B_HOUSE_NORTH_TO_TOLBI, CONDITION_ALWAYS, STUCCO_ROOMS_B_HOUSE_NORTH_TO_TOLBI },
    { TRIGGER_EXIT, STUCCO_ROOMS_B_INN_TO_TOLBI, CONDITION_ALWAYS, STUCCO_ROOMS_B_INN_TO_TOLBI },
    { TRIGGER_EXIT, STUCCO_ROOMS_B_UNUSED_TO_TOLBI, CONDITION_ALWAYS, STUCCO_ROOMS_B_UNUSED_TO_TOLBI },
    { TRIGGER_EXIT, STUCCO_ROOMS_B_HOUSE_EAST_TO_TOLBI, CONDITION_ALWAYS, STUCCO_ROOMS_B_HOUSE_EAST_TO_TOLBI },
    { TRIGGER_EXIT, STUCCO_ROOMS_B_HOUSE_SOUTHEAST_TO_TOLBI, CONDITION_ALWAYS, STUCCO_ROOMS_B_HOUSE_SOUTHEAST_TO_TOLBI },
    { TRIGGER_EXIT, STUCCO_ROOMS_B_WEAPON_AND_ARMOR_SHOP_TO_TOLBI, CONDITION_ALWAYS, STUCCO_ROOMS_B_WEAPON_AND_ARMOR_SHOP_TO_TOLBI },
    { TRIGGER_EXIT, STUCCO_ROOMS_B_HOUSE_SOUTHWEST_TO_TOLBI, CONDITION_ALWAYS, STUCCO_ROOMS_B_HOUSE_SOUTHWEST_TO_TOLBI },
    { TRIGGER_EXIT, STUCCO_ROOMS_B_SANCTUM_TO_TOLBI, CONDITION_ALWAYS, STUCCO_ROOMS_B_SANCTUM_TO_TOLBI },
    { TRIGGER_EXIT, STUCCO_ROOMS_B_INN_TO_INN_PASSAGE, CONDITION_ALWAYS, STUCCO_ROOMS_B_INN_TO_INN_PASSAGE },
    { TRIGGER_EXIT, STUCCO_ROOMS_B_INN_PASSAGE_TO_INN, CONDITION_ALWAYS, STUCCO_ROOMS_B_INN_PASSAGE_TO_INN },
    { TRIGGER_EXIT, STUCCO_ROOMS_B_INN_PASSAGE_TO_GUEST_ROOM_SOUTHWEST, CONDITION_ALWAYS, STUCCO_ROOMS_B_INN_PASSAGE_TO_GUEST_ROOM_SOUTHWEST },
    { TRIGGER_EXIT, STUCCO_ROOMS_B_INN_PASSAGE_TO_GUEST_ROOM_SOUTHEAST, CONDITION_ALWAYS, STUCCO_ROOMS_B_INN_PASSAGE_TO_GUEST_ROOM_SOUTHEAST },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), STUCCO_ROOMS_B_INN_PASSAGE_TO_GUEST_ROOM_NORTHWEST, CONDITION_ALWAYS, EVENT(StuccoRoomsB_EnterDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), STUCCO_ROOMS_B_INN_PASSAGE_TO_GUEST_ROOM_NORTHEAST, CONDITION_ALWAYS, EVENT(StuccoRoomsB_EnterDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), STUCCO_ROOMS_B_GUEST_ROOM_SOUTHEAST_TO_INN_PASSAGE, CONDITION_ALWAYS, EVENT(StuccoRoomsB_EnterDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), STUCCO_ROOMS_B_GUEST_ROOM_SOUTHWEST_TO_INN_PASSAGE, CONDITION_ALWAYS, EVENT(StuccoRoomsB_EnterDoor) },
    { TRIGGER_EXIT, STUCCO_ROOMS_B_GUEST_ROOM_NORTHWEST_TO_INN_PASSAGE, CONDITION_ALWAYS, STUCCO_ROOMS_B_GUEST_ROOM_NORTHWEST_TO_INN_PASSAGE },
    { TRIGGER_EXIT, STUCCO_ROOMS_B_GUEST_ROOM_NORTHEAST_TO_INN_PASSAGE, CONDITION_ALWAYS, STUCCO_ROOMS_B_GUEST_ROOM_NORTHEAST_TO_INN_PASSAGE },
    { TRIGGER_TALK, ACTOR_STUCCO_ROOMS_B_MAKESHIFT_INNKEEPER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsTolbiIsPackedWithTouristsDuring) },
    { TRIGGER_TALK, ACTOR_STUCCO_ROOMS_B_HOST, CONDITION_ALWAYS, EVENT(StuccoRoomsB_TalkToHost) },
    { TRIGGER_TALK, ACTOR_STUCCO_ROOMS_B_SLEEPLESS_GUEST, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsICouldntSleepAtAllLast) },
    { TRIGGER_TALK, ACTOR_STUCCO_ROOMS_B_CROWD_SICK_GUEST, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsTheCrowdsAtTheColosseumMade) },
    { TRIGGER_TALK, ACTOR_STUCCO_ROOMS_B_HOUSE_TENANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsWePaidGoodMoneyToRent) },
    { TRIGGER_TALK, ACTOR_STUCCO_ROOMS_B_FINALS_WATCHER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIReallyWantToSeeColosso) },
    { TRIGGER_TALK, ACTOR_STUCCO_ROOMS_B_CRYING_BOY, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsWahhhIWantToSeeColosso) },
    { TRIGGER_TALK, ACTOR_STUCCO_ROOMS_B_SOLDIER_WATCHER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIWonderIfTheyReallyCant) },
    { TRIGGER_TALK, ACTOR_STUCCO_ROOMS_B_BABI_SKEPTIC, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsBabiWasntAtColossoAccordingTo) },
    { TRIGGER_TALK, ACTOR_STUCCO_ROOMS_B_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsWereGoingToMakeSomeMoney) },
    { TRIGGER_TALK, ACTOR_STUCCO_ROOMS_B_TRAVELING_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIHeardBabiIsUsingThe) },
    { TRIGGER_TALK, ACTOR_STUCCO_ROOMS_B_WEAPON_MERCHANT, CONDITION_ALWAYS, EVENT(StuccoRoomsB_TalkToWeaponMerchant) },
    { TRIGGER_TALK, ACTOR_STUCCO_ROOMS_B_ITEM_MERCHANT, CONDITION_ALWAYS, EVENT(StuccoRoomsB_TalkToItemMerchant) },
    { TRIGGER_TALK, ACTOR_STUCCO_ROOMS_B_ARMOR_MERCHANT, CONDITION_ALWAYS, EVENT(StuccoRoomsB_TalkToArmorMerchant) },
    { TRIGGER_TALK, ACTOR_STUCCO_ROOMS_B_INN_MASTER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsWereFullyBookedWithReservationsDuring) },
    { TRIGGER_TALK, ACTOR_STUCCO_ROOMS_B_INN_LADY, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsColossoIsTheBusiestTimeOf) },
    { TRIGGER_TALK, ACTOR_STUCCO_ROOMS_B_INN_WORKER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsWereSoBusyRunningTheInn) },
    { TRIGGER_TALK, ACTOR_STUCCO_ROOMS_B_MEDAL_WOMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsATourGroupCameFromKalay) },
    { TRIGGER_TALK, ACTOR_STUCCO_ROOMS_B_CHEF, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsWeArentGoingToGetTo) },
    { TRIGGER_TALK, ACTOR_STUCCO_ROOMS_B_WAITRESS, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsImNotVeryInterestedInColosso) },
    { TRIGGER_TALK, ACTOR_STUCCO_ROOMS_B_LATE_ENTRANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsOhNoWeBarelyMadeIt) },
    { TRIGGER_TALK, ACTOR_STUCCO_ROOMS_B_SECOND_LATE_ENTRANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIfWedGottenHereJustA) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_MAKESHIFT_INNKEEPER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsOnceColossoStartsWereGoingTo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_HOST, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsItsTooLateToStartLooking) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_SLEEPLESS_GUEST, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIfICouldHaveSleptLast) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_CROWD_SICK_GUEST, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsHuhUhCrowdsAreADefinite) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_HOUSE_TENANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsWeGetToStayHereUntil) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_FINALS_WATCHER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIWasStubbornISaidIm) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_CRYING_BOY, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIKeepOnCryingButDaddy) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_SOLDIER_WATCHER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsItsStrangeThatBabisSoldiersAre) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_BABI_SKEPTIC, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsItsStrangeBabiLovesColossoBut) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsHeJustTakesACutOf) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_TRAVELING_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsNamingTheLighthouseHeBuiltAfter) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_WEAPON_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIGuessINeverWouldHave) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_ITEM_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsTheyHaveSomeNiceThingsFor) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_ARMOR_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsBabiChoosesColossosVictorsToBe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_INN_MASTER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsTheTourGroupIsFeelingCramped) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_INN_LADY, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsWeCantHandleTooManyMore) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_INN_WORKER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsImGoingToGoToThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_MEDAL_WOMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsThereArentEnoughBedsForThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_CHEF, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsTheInnkeeperToldMeToCook) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_WAITRESS, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIHaveAHeadacheJustThinking) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_LATE_ENTRANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsEvenThoughICantEnterColosso) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_SECOND_LATE_ENTRANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIfWeCouldHaveEnteredColosso) },
    { TRIGGER_TALK, ACTOR_STUCCO_ROOMS_B_HEALER, CONDITION_ALWAYS, EVENT(StuccoRoomsB_TalkToHealer) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_HEALER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIWonderHowTheTryoutsAre) },
    { EXAMINE_TRIGGER(EXAMINE_BARREL), CELL_BEHAVIOR_STUCCO_ROOMS_B_VIAL_BARREL, FLAG_STUCCO_ROOMS_B_BARREL_VIAL, FIND_ITEM(ITEM_VIAL) },
    { EXAMINE_TRIGGER(EXAMINE_BARREL), CELL_BEHAVIOR_STUCCO_ROOMS_B_MEDAL_BARREL, FLAG_STUCCO_ROOMS_B_BARREL_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { EXAMINE_TRIGGER(EXAMINE_OVEN), CELL_BEHAVIOR_STUCCO_ROOMS_B_PIZZA_OVEN, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgTolbiInteriorsShesMakingTheWorldRenownedTolbi)) },
    { EXAMINE_TRIGGER(EXAMINE_OVEN), CELL_BEHAVIOR_STUCCO_ROOMS_B_PASTA_OVEN, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgTolbiInteriorsHeIsBoilingThePastaIt)) },
    { EXAMINE_TRIGGER(EXAMINE_OVEN), CELL_BEHAVIOR_STUCCO_ROOMS_B_SEAFOOD_PASTA_OVEN, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgTolbiInteriorsItsTolbiSeafoodPastaWithLots)) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_STUCCO_ROOMS_B_INN_SIGN, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgTolbiInteriorsIsaacCheckedTheSignInnFull)) },
    { TRIGGER_TABLE_END },
};

struct Trigger gStuccoRoomsBTriggersFinalsDay[] = {
    { TRIGGER_EXIT, STUCCO_ROOMS_B_HOUSE_NORTHWEST_TO_TOLBI, CONDITION_ALWAYS, STUCCO_ROOMS_B_HOUSE_NORTHWEST_TO_TOLBI },
    { TRIGGER_EXIT, STUCCO_ROOMS_B_HOUSE_NORTH_TO_TOLBI, CONDITION_ALWAYS, STUCCO_ROOMS_B_HOUSE_NORTH_TO_TOLBI },
    { TRIGGER_EXIT, STUCCO_ROOMS_B_INN_TO_TOLBI, CONDITION_ALWAYS, STUCCO_ROOMS_B_INN_TO_TOLBI },
    { TRIGGER_EXIT, STUCCO_ROOMS_B_UNUSED_TO_TOLBI, CONDITION_ALWAYS, STUCCO_ROOMS_B_UNUSED_TO_TOLBI },
    { TRIGGER_EXIT, STUCCO_ROOMS_B_HOUSE_EAST_TO_TOLBI, CONDITION_ALWAYS, STUCCO_ROOMS_B_HOUSE_EAST_TO_TOLBI },
    { TRIGGER_EXIT, STUCCO_ROOMS_B_HOUSE_SOUTHEAST_TO_TOLBI, CONDITION_ALWAYS, STUCCO_ROOMS_B_HOUSE_SOUTHEAST_TO_TOLBI },
    { TRIGGER_EXIT, STUCCO_ROOMS_B_WEAPON_AND_ARMOR_SHOP_TO_TOLBI, CONDITION_ALWAYS, STUCCO_ROOMS_B_WEAPON_AND_ARMOR_SHOP_TO_TOLBI },
    { TRIGGER_EXIT, STUCCO_ROOMS_B_HOUSE_SOUTHWEST_TO_TOLBI, CONDITION_ALWAYS, STUCCO_ROOMS_B_HOUSE_SOUTHWEST_TO_TOLBI },
    { TRIGGER_EXIT, STUCCO_ROOMS_B_SANCTUM_TO_TOLBI, CONDITION_ALWAYS, STUCCO_ROOMS_B_SANCTUM_TO_TOLBI },
    { TRIGGER_EXIT, STUCCO_ROOMS_B_INN_TO_INN_PASSAGE, CONDITION_ALWAYS, STUCCO_ROOMS_B_INN_TO_INN_PASSAGE },
    { TRIGGER_EXIT, STUCCO_ROOMS_B_INN_PASSAGE_TO_INN, CONDITION_ALWAYS, STUCCO_ROOMS_B_INN_PASSAGE_TO_INN },
    { TRIGGER_EXIT, STUCCO_ROOMS_B_INN_PASSAGE_TO_GUEST_ROOM_SOUTHWEST, CONDITION_ALWAYS, STUCCO_ROOMS_B_INN_PASSAGE_TO_GUEST_ROOM_SOUTHWEST },
    { TRIGGER_EXIT, STUCCO_ROOMS_B_INN_PASSAGE_TO_GUEST_ROOM_SOUTHEAST, CONDITION_ALWAYS, STUCCO_ROOMS_B_INN_PASSAGE_TO_GUEST_ROOM_SOUTHEAST },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), STUCCO_ROOMS_B_INN_PASSAGE_TO_GUEST_ROOM_NORTHWEST, CONDITION_ALWAYS, EVENT(StuccoRoomsB_EnterDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), STUCCO_ROOMS_B_INN_PASSAGE_TO_GUEST_ROOM_NORTHEAST, CONDITION_ALWAYS, EVENT(StuccoRoomsB_EnterDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), STUCCO_ROOMS_B_GUEST_ROOM_SOUTHEAST_TO_INN_PASSAGE, CONDITION_ALWAYS, EVENT(StuccoRoomsB_EnterDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), STUCCO_ROOMS_B_GUEST_ROOM_SOUTHWEST_TO_INN_PASSAGE, CONDITION_ALWAYS, EVENT(StuccoRoomsB_EnterDoor) },
    { TRIGGER_EXIT, STUCCO_ROOMS_B_GUEST_ROOM_NORTHWEST_TO_INN_PASSAGE, CONDITION_ALWAYS, STUCCO_ROOMS_B_GUEST_ROOM_NORTHWEST_TO_INN_PASSAGE },
    { TRIGGER_EXIT, STUCCO_ROOMS_B_GUEST_ROOM_NORTHEAST_TO_INN_PASSAGE, CONDITION_ALWAYS, STUCCO_ROOMS_B_GUEST_ROOM_NORTHEAST_TO_INN_PASSAGE },
    { TRIGGER_TALK, ACTOR_STUCCO_ROOMS_B_MAKESHIFT_INNKEEPER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsSoColossoHasAlreadyReachedThe) },
    { TRIGGER_TALK, ACTOR_STUCCO_ROOMS_B_HOST, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsTheFestivalScheduleWasDelayedFor) },
    { TRIGGER_TALK, ACTOR_STUCCO_ROOMS_B_SLEEPLESS_GUEST, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsThatsStrangeIWonderWhatHappened) },
    { TRIGGER_TALK, ACTOR_STUCCO_ROOMS_B_CROWD_SICK_GUEST, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIJustSawBabiTheTowns) },
    { TRIGGER_TALK, ACTOR_STUCCO_ROOMS_B_HOUSE_TENANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsHereIAmInTolbiFor) },
    { TRIGGER_TALK, ACTOR_STUCCO_ROOMS_B_FINALS_WATCHER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsImGoingToWatchTheFinals) },
    { TRIGGER_TALK, ACTOR_STUCCO_ROOMS_B_CRYING_BOY, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsHoorayWereGoingToColossoNow) },
#if TBS_PATCH_4
    { TRIGGER_TALK, ACTOR_STUCCO_ROOMS_B_SOLDIER_WATCHER, CONDITION_ALWAYS, EVENT(StuccoRoomsB_AskWhetherBabiWasMissing) },
    { TRIGGER_TALK, ACTOR_STUCCO_ROOMS_B_BABI_SKEPTIC, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsBabiDecidedShowUpForFinals) },
#else
    { TRIGGER_TALK, ACTOR_STUCCO_ROOMS_B_SOLDIER_WATCHER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsWellIGuessSoSinceHes) },
    { TRIGGER_TALK, ACTOR_STUCCO_ROOMS_B_BABI_SKEPTIC, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsHuhThenTheyFoundHimIn) },
#endif
    { TRIGGER_TALK, ACTOR_STUCCO_ROOMS_B_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsBabiApparentlyHasSomeReasonFor) },
    { TRIGGER_TALK, ACTOR_STUCCO_ROOMS_B_TRAVELING_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIHeardBabiIsSearchingFor) },
    { TRIGGER_TALK, ACTOR_STUCCO_ROOMS_B_WEAPON_MERCHANT, CONDITION_ALWAYS, EVENT(StuccoRoomsB_TalkToWeaponMerchant) },
    { TRIGGER_TALK, ACTOR_STUCCO_ROOMS_B_ITEM_MERCHANT, CONDITION_ALWAYS, EVENT(StuccoRoomsB_TalkToItemMerchant) },
    { TRIGGER_TALK, ACTOR_STUCCO_ROOMS_B_ARMOR_MERCHANT, CONDITION_ALWAYS, EVENT(StuccoRoomsB_TalkToArmorMerchant) },
    { TRIGGER_TALK, ACTOR_STUCCO_ROOMS_B_INN_MASTER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsOnceTheFinalsBeginEveryoneTries) },
    { TRIGGER_TALK, ACTOR_STUCCO_ROOMS_B_INN_LADY, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsThisYearImNotLettingAnyone) },
    { TRIGGER_TALK, ACTOR_STUCCO_ROOMS_B_INN_WORKER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsTheWifeHasACloseEye) },
    { TRIGGER_TALK, ACTOR_STUCCO_ROOMS_B_MEDAL_WOMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsHmmIThoughtAllMenLoved) },
    { TRIGGER_TALK, ACTOR_STUCCO_ROOMS_B_CHEF, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsTheMomentIHearTheFinals) },
    { TRIGGER_TALK, ACTOR_STUCCO_ROOMS_B_WAITRESS, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsSoColossoIsEndingSoonIts) },
    { TRIGGER_TALK, ACTOR_STUCCO_ROOMS_B_LATE_ENTRANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsTheColossoFinalsAreStartingSoon) },
    { TRIGGER_TALK, ACTOR_STUCCO_ROOMS_B_SECOND_LATE_ENTRANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsMaybeIllDoSomeResearchOn) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_MAKESHIFT_INNKEEPER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsAreWeMakingMoreMoneyNow) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_HOST, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsSoTheyDidExtendTheFestival) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_SLEEPLESS_GUEST, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsICantAffordToStayHere) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_CROWD_SICK_GUEST, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsTheAirInTheColosseumIs) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_HOUSE_TENANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIfSomebodyElseWouldWatchThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_FINALS_WATCHER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsColossoIsAlreadyAtTheFinals) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_CRYING_BOY, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsSinceWeDidntGetToSee) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_SOLDIER_WATCHER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsSoWhatWereBabisSoldiersDoing) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_BABI_SKEPTIC, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsMaybeWatchingTheTrialsIsToo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsBeforeHeStartedTheLighthouseBabi) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_TRAVELING_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsBabiHasBeenMakingStrangeReferences) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_WEAPON_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsTheTouristsAllBuyWeaponsAnd) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_ITEM_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsYouCouldSayTheWarriorsIn) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_ARMOR_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIfMomFindsOutIWent) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_INN_MASTER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsImTheOneWhoReallyWants) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_INN_LADY, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsOnceThingsSettleDownImSure) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_INN_WORKER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIfTheMasterSlipsOutThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_MEDAL_WOMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsWeWomenAreTheOnlyOnes) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_CHEF, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsHasntTheMasterGoneToColosso) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_WAITRESS, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIDontHaveAnyParticularInterest) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_LATE_ENTRANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIWantToSeeTheFinals) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_SECOND_LATE_ENTRANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsTheTrialBattlesWerentQuiteLike) },
    { TRIGGER_TALK, ACTOR_STUCCO_ROOMS_B_HEALER, CONDITION_ALWAYS, EVENT(StuccoRoomsB_TalkToHealer) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_HEALER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIfYoureNotInterestedInThe) },
    { EXAMINE_TRIGGER(EXAMINE_BARREL), CELL_BEHAVIOR_STUCCO_ROOMS_B_VIAL_BARREL, FLAG_STUCCO_ROOMS_B_BARREL_VIAL, FIND_ITEM(ITEM_VIAL) },
    { EXAMINE_TRIGGER(EXAMINE_BARREL), CELL_BEHAVIOR_STUCCO_ROOMS_B_MEDAL_BARREL, FLAG_STUCCO_ROOMS_B_BARREL_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { EXAMINE_TRIGGER(EXAMINE_OVEN), CELL_BEHAVIOR_STUCCO_ROOMS_B_PIZZA_OVEN, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgTolbiInteriorsShesMakingTheWorldRenownedTolbi)) },
    { EXAMINE_TRIGGER(EXAMINE_OVEN), CELL_BEHAVIOR_STUCCO_ROOMS_B_PASTA_OVEN, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgTolbiInteriorsHeIsBoilingThePastaIt)) },
    { EXAMINE_TRIGGER(EXAMINE_OVEN), CELL_BEHAVIOR_STUCCO_ROOMS_B_SEAFOOD_PASTA_OVEN, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgTolbiInteriorsItsTolbiSeafoodPastaWithLots)) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_STUCCO_ROOMS_B_INN_SIGN, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgTolbiInteriorsIsaacCheckedTheSignInnFull)) },
    { TRIGGER_TABLE_END },
};

struct Trigger gStuccoRoomsBTriggersAfterColosso[] = {
    { TRIGGER_EXIT, STUCCO_ROOMS_B_HOUSE_NORTHWEST_TO_TOLBI, CONDITION_ALWAYS, STUCCO_ROOMS_B_HOUSE_NORTHWEST_TO_TOLBI },
    { TRIGGER_EXIT, STUCCO_ROOMS_B_HOUSE_NORTH_TO_TOLBI, CONDITION_ALWAYS, STUCCO_ROOMS_B_HOUSE_NORTH_TO_TOLBI },
    { TRIGGER_EXIT, STUCCO_ROOMS_B_INN_TO_TOLBI, CONDITION_ALWAYS, STUCCO_ROOMS_B_INN_TO_TOLBI },
    { TRIGGER_EXIT, STUCCO_ROOMS_B_UNUSED_TO_TOLBI, CONDITION_ALWAYS, STUCCO_ROOMS_B_UNUSED_TO_TOLBI },
    { TRIGGER_EXIT, STUCCO_ROOMS_B_HOUSE_EAST_TO_TOLBI, CONDITION_ALWAYS, STUCCO_ROOMS_B_HOUSE_EAST_TO_TOLBI },
    { TRIGGER_EXIT, STUCCO_ROOMS_B_HOUSE_SOUTHEAST_TO_TOLBI, CONDITION_ALWAYS, STUCCO_ROOMS_B_HOUSE_SOUTHEAST_TO_TOLBI },
    { TRIGGER_EXIT, STUCCO_ROOMS_B_WEAPON_AND_ARMOR_SHOP_TO_TOLBI, CONDITION_ALWAYS, STUCCO_ROOMS_B_WEAPON_AND_ARMOR_SHOP_TO_TOLBI },
    { TRIGGER_EXIT, STUCCO_ROOMS_B_HOUSE_SOUTHWEST_TO_TOLBI, CONDITION_ALWAYS, STUCCO_ROOMS_B_HOUSE_SOUTHWEST_TO_TOLBI },
    { TRIGGER_EXIT, STUCCO_ROOMS_B_SANCTUM_TO_TOLBI, CONDITION_ALWAYS, STUCCO_ROOMS_B_SANCTUM_TO_TOLBI },
    { TRIGGER_EXIT, STUCCO_ROOMS_B_INN_TO_INN_PASSAGE, CONDITION_ALWAYS, STUCCO_ROOMS_B_INN_TO_INN_PASSAGE },
    { TRIGGER_EXIT, STUCCO_ROOMS_B_INN_PASSAGE_TO_INN, CONDITION_ALWAYS, STUCCO_ROOMS_B_INN_PASSAGE_TO_INN },
    { TRIGGER_EXIT, STUCCO_ROOMS_B_INN_PASSAGE_TO_GUEST_ROOM_SOUTHWEST, CONDITION_ALWAYS, STUCCO_ROOMS_B_INN_PASSAGE_TO_GUEST_ROOM_SOUTHWEST },
    { TRIGGER_EXIT, STUCCO_ROOMS_B_INN_PASSAGE_TO_GUEST_ROOM_SOUTHEAST, CONDITION_ALWAYS, STUCCO_ROOMS_B_INN_PASSAGE_TO_GUEST_ROOM_SOUTHEAST },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), STUCCO_ROOMS_B_INN_PASSAGE_TO_GUEST_ROOM_NORTHWEST, CONDITION_ALWAYS, EVENT(StuccoRoomsB_EnterDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), STUCCO_ROOMS_B_INN_PASSAGE_TO_GUEST_ROOM_NORTHEAST, CONDITION_ALWAYS, EVENT(StuccoRoomsB_EnterDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), STUCCO_ROOMS_B_GUEST_ROOM_SOUTHEAST_TO_INN_PASSAGE, CONDITION_ALWAYS, EVENT(StuccoRoomsB_EnterDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), STUCCO_ROOMS_B_GUEST_ROOM_SOUTHWEST_TO_INN_PASSAGE, CONDITION_ALWAYS, EVENT(StuccoRoomsB_EnterDoor) },
    { TRIGGER_EXIT, STUCCO_ROOMS_B_GUEST_ROOM_NORTHWEST_TO_INN_PASSAGE, CONDITION_ALWAYS, STUCCO_ROOMS_B_GUEST_ROOM_NORTHWEST_TO_INN_PASSAGE },
    { TRIGGER_EXIT, STUCCO_ROOMS_B_GUEST_ROOM_NORTHEAST_TO_INN_PASSAGE, CONDITION_ALWAYS, STUCCO_ROOMS_B_GUEST_ROOM_NORTHEAST_TO_INN_PASSAGE },
    { TRIGGER_TALK, ACTOR_STUCCO_ROOMS_B_MAKESHIFT_INNKEEPER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsNowThatColossoIsOverWeve) },
    { TRIGGER_TALK, ACTOR_STUCCO_ROOMS_B_HOST, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsWeHadALotOfGuests) },
    { TRIGGER_TALK, ACTOR_STUCCO_ROOMS_B_STRANDED_TRAVELER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIRentedMyHouseToA) },
    { TRIGGER_TALK, ACTOR_STUCCO_ROOMS_B_CAVE_RUMOR_GUEST, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsTheTouristsAreAllInA) },
    { TRIGGER_TALK, ACTOR_STUCCO_ROOMS_B_FINALS_WATCHER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIWasSoMovedByThis) },
    { TRIGGER_TALK, ACTOR_STUCCO_ROOMS_B_CRYING_BOY, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsThatLittleGuyJumpedStraightInto) },
    { TRIGGER_TALK, ACTOR_STUCCO_ROOMS_B_SOLDIER_WATCHER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsDidYouKnowMtAlephErupted) },
    { TRIGGER_TALK, ACTOR_STUCCO_ROOMS_B_BABI_SKEPTIC, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsMaybeAlephsEruptionCausedThoseSea) },
    { TRIGGER_TALK, ACTOR_STUCCO_ROOMS_B_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsTheWarriorsSaidTheyWouldRush) },
    { TRIGGER_TALK, ACTOR_STUCCO_ROOMS_B_TRAVELING_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsThereWontBeManyCustomersUntil) },
    { TRIGGER_TALK, ACTOR_STUCCO_ROOMS_B_WEAPON_MERCHANT, CONDITION_ALWAYS, EVENT(StuccoRoomsB_TalkToWeaponMerchant) },
    { TRIGGER_TALK, ACTOR_STUCCO_ROOMS_B_ITEM_MERCHANT, CONDITION_ALWAYS, EVENT(StuccoRoomsB_TalkToItemMerchant) },
    { TRIGGER_TALK, ACTOR_STUCCO_ROOMS_B_ARMOR_MERCHANT, CONDITION_ALWAYS, EVENT(StuccoRoomsB_TalkToArmorMerchant) },
    { TRIGGER_TALK, ACTOR_STUCCO_ROOMS_B_INN_MASTER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIThoughtWedHaveSomeFree) },
    { TRIGGER_TALK, ACTOR_STUCCO_ROOMS_B_INN_LADY, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsTheSpectatorsFromTheEastCant) },
    { TRIGGER_TALK, ACTOR_STUCCO_ROOMS_B_INN_WORKER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsItsBusyAllDayEveryDay) },
    { TRIGGER_TALK, ACTOR_STUCCO_ROOMS_B_MEDAL_WOMAN, CONDITION_ALWAYS, EVENT(StuccoRoomsB_TalkToMedalWoman) },
    { TRIGGER_TALK, ACTOR_STUCCO_ROOMS_B_CHEF, CONDITION_ALWAYS, EVENT(StuccoRoomsB_TalkToChef) },
    { TRIGGER_TALK, ACTOR_STUCCO_ROOMS_B_WAITRESS, CONDITION_ALWAYS, EVENT(StuccoRoomsB_TalkToWaitress) },
    { TRIGGER_TALK, ACTOR_STUCCO_ROOMS_B_LATE_ENTRANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIWentToThePortBut) },
    { TRIGGER_TALK, ACTOR_STUCCO_ROOMS_B_SECOND_LATE_ENTRANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsColossoHasEndedButWeStill) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_MAKESHIFT_INNKEEPER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsSighMakingTheBedsHasWorn) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_HOST, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIdLoveAVacationButWhere) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_STRANDED_TRAVELER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsOnceTheShipsStartSailingAgain) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_CAVE_RUMOR_GUEST, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIHeardOfACaveThat) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_FINALS_WATCHER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsTheWarriorsWhoFoughtInThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_CRYING_BOY, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsDaringAndBraveryAreNiceAnd) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_SOLDIER_WATCHER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsTheSparklingStonesThatRainedFrom) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_BABI_SKEPTIC, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIllBetTheMonstersWereFrightened) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsItBothersMeABitThat) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_TRAVELING_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsThereArentManyMerchantsAtThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_WEAPON_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsEvenWithoutTheShipsICould) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_ITEM_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsDidAllTheWarriorsWhoFought) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_ARMOR_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsImGonnaWatchTheFinalsNext) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_INN_MASTER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsSinceTheShipsArentSailingThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_INN_LADY, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsHeeHeeItsGreatTheInns) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_INN_WORKER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsEveryoneIsWorkingOnBabiLighthouse) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_MEDAL_WOMAN, CONDITION_ALWAYS, EVENT(StuccoRoomsB_ReadMedalWomanMind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_CHEF, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsGrrrIDidntGetToSee) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_WAITRESS, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsHeyChefSoWhatIfYou) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_LATE_ENTRANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsITriedGoingToTheEastern) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_SECOND_LATE_ENTRANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsYoudThinkYouCouldGetThrough) },
    { TRIGGER_TALK, ACTOR_STUCCO_ROOMS_B_HEALER, CONDITION_ALWAYS, EVENT(StuccoRoomsB_TalkToHealer) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_HEALER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsYouAllGotToSeeColosso) },
    { TRIGGER_TALK, ACTOR_STUCCO_ROOMS_B_ANGRY_PASSENGER, CONDITION_ALWAYS, EVENT(StuccoRoomsB_TalkToAngryPassenger) },
    { TRIGGER_TALK, ACTOR_STUCCO_ROOMS_B_PASSENGER_WIFE, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsOnceThisGuyGetsMadTheres) },
    { TRIGGER_TALK, ACTOR_STUCCO_ROOMS_B_THIRD_PASSENGER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIWonderIfThisReallyIs) },
    { TRIGGER_TALK, ACTOR_STUCCO_ROOMS_B_BORED_BOY, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsTolbiGetsSoBoringOnceThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_ANGRY_PASSENGER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsYoureSayingIfWeRowWe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_PASSENGER_WIFE, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsTheCrewIsSayingThatIf) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_THIRD_PASSENGER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsImSickOfStayingAtThis) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_BORED_BOY, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIfMomHadJustBoughtMe) },
    { TRIGGER_TALK, ACTOR_STUCCO_ROOMS_B_FELLOW_PASSENGER, CONDITION_ALWAYS, EVENT(StuccoRoomsB_TalkToFellowPassenger) },
    { TRIGGER_TALK, ACTOR_STUCCO_ROOMS_B_COLOSSO_FAN, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIGotToSeeColossoSo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_FELLOW_PASSENGER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsThatVoyageAcrossTheSeaWas) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_COLOSSO_FAN, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsNoMatterWhatHappensImComing) },
    { EXAMINE_TRIGGER(EXAMINE_BARREL), CELL_BEHAVIOR_STUCCO_ROOMS_B_VIAL_BARREL, FLAG_STUCCO_ROOMS_B_BARREL_VIAL, FIND_ITEM(ITEM_VIAL) },
    { EXAMINE_TRIGGER(EXAMINE_BARREL), CELL_BEHAVIOR_STUCCO_ROOMS_B_MEDAL_BARREL, FLAG_STUCCO_ROOMS_B_BARREL_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { EXAMINE_TRIGGER(EXAMINE_OVEN), CELL_BEHAVIOR_STUCCO_ROOMS_B_PIZZA_OVEN, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgTolbiInteriorsShesMakingTheWorldRenownedTolbi)) },
    { EXAMINE_TRIGGER(EXAMINE_OVEN), CELL_BEHAVIOR_STUCCO_ROOMS_B_PASTA_OVEN, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgTolbiInteriorsHeIsBoilingThePastaIt)) },
    { EXAMINE_TRIGGER(EXAMINE_OVEN), CELL_BEHAVIOR_STUCCO_ROOMS_B_SEAFOOD_PASTA_OVEN, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgTolbiInteriorsItsTolbiSeafoodPastaWithLots)) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_STUCCO_ROOMS_B_INN_SIGN, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgTolbiInteriorsIsaacCheckedTheSignInnFull)) },
    { EXAMINE_TRIGGER(EXAMINE_BARREL), CELL_BEHAVIOR_STUCCO_ROOMS_B_HIDDEN_MEDAL_BARREL, FLAG_STUCCO_ROOMS_B_MEDAL_TAKEN, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { TRIGGER_TABLE_END },
};
// clang-format on

/// A door of the rooms opening, and where each door is.
u16 gStuccoRoomsBDoorOpening[] = {
    MAPGRID_ANIMATION_STEP(2, 123, 1, 2, 6),
    MAPGRID_ANIMATION_STEP(4, 123, 1, 2, 6),
    MAPGRID_ANIMATION_STEPS_END,
};
struct MapGridCellAnimation gStuccoRoomsBDoors[] = {
    {gStuccoRoomsBDoorOpening, 24, 100},
    {gStuccoRoomsBDoorOpening, 33, 100},
    {gStuccoRoomsBDoorOpening, 50, 99},
    {gStuccoRoomsBDoorOpening, 52, 109},
};
