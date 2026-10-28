/// @file
/// Stucco rooms B (SceneId_StuccoRoomsB), a sheet of rooms drawn in stucco, all
/// of them Tolbi's: its houses, inns and shops, which the area names call
/// Tolbi, and Tolbi Sanctum, as the same scene is named at
/// STUCCO_ROOMS_B_SANCTUM and STUCCO_ROOMS_B_SANCTUM_NORTH.
///
/// - Tolbi, the rooms behind the town's doors, which Tolbi's exits
///   TOLBI_TO_HOUSE_SOUTH to TOLBI_TO_HOUSE_SOUTHWEST lead into; this scene's
///   exits STUCCO_ROOMS_B_HOUSE_NORTHWEST_TO_TOLBI to
///   STUCCO_ROOMS_B_TO_TOLBI_WEST lead back out. During the festival one
///   household lines up beds and becomes a makeshift inn, visitors have
///   rented another house, and the weapon, armor and item shops
///   (STUCCO_ROOMS_B_WEAPON_SHOP, STUCCO_ROOMS_B_ARMOR_SHOP and
///   STUCCO_ROOMS_B_ITEM_SHOP) open across their counters. The inn behind
///   TOLBI_TO_INN is full, `Inn Full / No Vacancies`;
///   STUCCO_ROOMS_B_INN_TO_INN_PASSAGE and
///   STUCCO_ROOMS_B_INN_PASSAGE_TO_INN join it to the inn's corridor, whose
///   doors lead into four guest rooms
///   (STUCCO_ROOMS_B_INN_PASSAGE_TO_GUEST_ROOM_SOUTHWEST and the rest).
/// - Tolbi Sanctum, behind TOLBI_TO_SANCTUM (STUCCO_ROOMS_B_SANCTUM), whose
///   healer opens the sanctum across the counter. STUCCO_ROOMS_B_SANCTUM_NORTH
///   is where a defeat returns the party while Iodem waits at Gondowan Passage.
///
/// The rooms' talk follows Colosso in the same three stages as the town, each
/// with its own trigger table (StuccoRoomsB_SelectTriggers) and, after
/// Colosso, an actor template table of its own. During the festival Babi
/// is missing and the town is full of visitors, among them a tour group from
/// Kalay that reserved only two rooms. On finals day (FLAG_BABI_SAVED) Babi
/// has shown himself at the finals; the Spanish and Italian editions add a
/// question about it (TBS_PATCH_4). After Colosso (FLAG_COLOSSO_OVER) no ship
/// sails the Karagol, and the visitors are stranded; one of them says they
/// made it to Tolbi with the party's help.
///
/// After Colosso two short events play, each once, on entering their
/// rooms (StuccoRoomsB_ApplyEntryState). In the northeast guest room a
/// woman finds something on the floor, starts at the sight of the party and
/// hides it behind the trash pile (StuccoRoomsB_WatchWomanHideMedal,
/// FLAG_STUCCO_ROOMS_B_MEDAL_HIDDEN): a Lucky Medal, which the party can then
/// take from that barrel (FLAG_STUCCO_ROOMS_B_MEDAL_TAKEN), after which she
/// greets them, startled, and Mind Read hears her hope its owner will not ask
/// for it back. In the southeast one stranded passengers argue that the
/// captain will not sail although they agreed to do whatever it takes
/// (StuccoRoomsB_WatchPassengersArgue, an area flag). A man who rode
/// the ship from Kalay with the party greets it as `the would-be warrior`, and
/// he and ACTOR_STUCCO_ROOMS_B_COLOSSO_FAN leave once the party has watched
/// the passengers confront the tour guide on the Tolbi docks
/// (FLAG_TOLBI_DOCKS_TOUR_GROUP_BLAMED). Barrels hold a Vial and a Lucky
/// Medal, and searching the ovens shows what is cooking.

#include "adventure.h"
#include "unit/scene/stucco_rooms_b.h"
#include "unit/overworld/overworld.h"
#include "unit/overworld/event_commands.h"
#include "stucco_rooms_b.h"
#include "event_layer.h"
#include "text/msg_ids.h"
#include "types.h"
#include "ability_ids.h"
#include "edition.h"
#include "scene_tables.h"
#include "item_ids.h"
#include "actor_script.h"
#include "songs.h"

// Area flags: Scene_Arrive clears 0x300 to 0x3ff when the
// party enters another area, and other areas use the numbers for
// their own.
/// The stranded passengers' argument has played.
/// ★ Named from the game: "He won't sail the ship"
/// (MsgTolbiHeWontSailShipEven)
#define FLAG_STUCCO_ROOMS_B_PASSENGERS_SEEN 0x300

// From TBS_PATCH_1 on, the camera stops 8 pixels further in the inn's
// corridor (INN) and its guest rooms, the southern pair lying east on the map
// (EAST_ROOM) and the northern pair south of it (SOUTH_ROOM). Renders of
// tbs-ja and tbs-en show what it fixes: before it, the last 8 pixels of these
// rooms stayed off screen, cutting off the corridor's south openings, the
// southern rooms' east walls and the northern rooms' doorways.
#if !TBS_PATCH_1
#define STUCCO_ROOMS_B_INN_CAMERA_MAX_Z 736
#define STUCCO_ROOMS_B_EAST_ROOM_CAMERA_MAX_X 944
#define STUCCO_ROOMS_B_SOUTH_ROOM_CAMERA_MAX_Z 928
#else
#define STUCCO_ROOMS_B_INN_CAMERA_MAX_Z 744
#define STUCCO_ROOMS_B_EAST_ROOM_CAMERA_MAX_X 952
#define STUCCO_ROOMS_B_SOUTH_ROOM_CAMERA_MAX_Z 936
#endif

/// The shop rows used by the room trigger tables below.
enum {
  STUCCO_ROOMS_B_WEAPON_SHOP = 26,
  STUCCO_ROOMS_B_ARMOR_SHOP = 27,
  STUCCO_ROOMS_B_ITEM_SHOP = 28,
  STUCCO_ROOMS_B_ACTOR_SLOT_END = 66
};

/// Asks a yes/no question, then answers with the message after the question
/// for yes or the one after that for no.
static inline void StuccoRoomsB_AskYesNo(s32 question, s32 speaker) {
  Event_SetMessageId(question);
  FieldMessage_ShowAtActor(speaker, 0);
  if (Event_AskYesNoWithActorAnimation(0, 0) == 0) {
    Event_WaitFrames(10);
    Event_SetMessageId(question + 1);
  } else {
    Event_SetMessageId(question + 2);
  }
  Event_ShowMessageAndWait(speaker, 0);
}

const struct SceneEntrance *StuccoRoomsB_GetEntrances(void) {
  return gStuccoRoomsBEntrances;
}

const struct SceneExitRegion *StuccoRoomsB_GetRegions(void) { return 0; }

const u32 *StuccoRoomsB_GetExits(void) { return gStuccoRoomsBExits; }

/// After Colosso the rooms hold a different crowd.
const struct ActorTemplate *StuccoRoomsB_SelectActorTemplates(void) {
  if (GameFlag_Get(FLAG_COLOSSO_OVER) != 0) {
    return gStuccoRoomsBActorTemplatesAfterColosso;
  }
  return gStuccoRoomsBActorTemplates;
}

#if TBS_PATCH_4
/// In Tolbi on finals day, patch 4 (the Spanish and Italian editions) adds a
/// question: was Babi missing at all, since he showed himself at the finals?
/// Its lines are in every edition's text, Japanese included, so it is a
/// later change to the script, not a translation's.
static void StuccoRoomsB_AskWhetherBabiWasMissing(s32 speaker) {
  StuccoRoomsB_AskYesNo(MsgTolbiInteriorsBabiShowedHimselfAtFinals, speaker);
}
#endif

/// Picks the trigger table for the current point in the Colosso story.
const struct Trigger *StuccoRoomsB_SelectTriggers(void) {
  if (GameFlag_Get(FLAG_COLOSSO_OVER) != 0) {
    return gStuccoRoomsBTriggersAfterColosso;
  }
  if (GameFlag_Get(FLAG_BABI_SAVED) != 0) {
    return gStuccoRoomsBTriggersFinalsDay;
  }
  return gStuccoRoomsBTriggersFestival;
}

/// Unused: leaves through TRANSITION_WINDOW, a screen transition of 24
/// frames; no event row names it.
UNUSED static void StuccoRoomsB_ExitThroughWindow(void) {
  struct FieldWork *work = gFieldWork;

  work->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
  work->transitionFrames = 24;
  Import_EventSetExitRequest();
}

/// In Tolbi, walking into one of the four doors at cell behaviors 14 to 17:
/// every placed actor stops, the door opens and the leader walks through
/// it.
static void StuccoRoomsB_EnterDoor(void) {
  u32 actorId;
  struct Actor *actor;
  struct FieldWork *work;
  s16 *trigger;
  s32 doorIndex;

  work = gFieldWork;
  // Stop the room's actors before animating the selected doorway.
  for (actorId = ACTOR_FIRST_PLACED; actorId < STUCCO_ROOMS_B_ACTOR_SLOT_END;
       actorId++) {
    actor = Actor_GetById(actorId);
    if (actor != 0) {
      actor->motionFlags = 0;
    }
  }
  trigger = &work->stepTrigger;
  doorIndex = *trigger - CELL_BEHAVIOR_STUCCO_ROOMS_B_FIRST_DOOR;
  Sound_PlaySong(SE_DOOR_OPEN);
  MapGrid_AnimateCells(gStuccoRoomsBDoors[doorIndex].steps,
                       gStuccoRoomsBDoors[doorIndex].x,
                       gStuccoRoomsBDoors[doorIndex].y);
  Actor_SetSpeedParameters(ACTOR_PARTY_LEADER, PIXELS(1) / 2, PIXELS(1) / 4);
  Actor_GetById(ACTOR_PARTY_LEADER)->motionFlags = 0;
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
  Import_EventSetExitRequest(*trigger);
}

/// In Tolbi after Colosso, entering the northeast guest room
/// (STUCCO_ROOMS_B_GUEST_ROOM_NORTHEAST_FROM_INN_PASSAGE): a woman
/// finds something on the floor, is startled to see the party and hides it in
/// the barrel behind the trash pile. "Hey, what's this?" The thing is a Lucky
/// Medal, found later in that barrel.
static void StuccoRoomsB_WatchWomanHideMedal(void) {
  struct Actor *sparkle;

  Event_Start();
  Event_SetMessageId(MsgTolbiHeyWhatsThis);

  // A sparkle on the floor where she finds it.
  Event_WaitFrames(40);
  sparkle = FieldActor_Create(SPRITE_PARTICLE, PIXELS(600), 0, PIXELS(824));
  Actor_SetSpriteFlags(sparkle, 0);
  Actor_SelectAnimation(sparkle, 6);
  Event_WaitFrames(10);
  Actor_SelectAnimation(sparkle, 1);
  Event_WaitFrames(40);
  Actor_Release(sparkle);
  Event_WaitFrames(2);

  // She runs to it and picks it up.
  Actor_ShowEmote(ACTOR_STUCCO_ROOMS_B_MEDAL_WOMAN,
                  EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 50);
  Actor_SetSpeed(ACTOR_STUCCO_ROOMS_B_MEDAL_WOMAN, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkToAndWait(ACTOR_STUCCO_ROOMS_B_MEDAL_WOMAN, 600, 848);
  Actor_FaceDirection(ACTOR_STUCCO_ROOMS_B_MEDAL_WOMAN, FACING_NORTH, 0);
  Event_WaitFrames(40);
  Event_ShowMessage(ACTOR_STUCCO_ROOMS_B_MEDAL_WOMAN, 0);
  Actor_RunRepeatedMotion(ACTOR_STUCCO_ROOMS_B_MEDAL_WOMAN, 2);
  Event_WaitFrames(30);
  Actor_WalkToAndWait(ACTOR_STUCCO_ROOMS_B_MEDAL_WOMAN, 568, 848);
  Actor_FaceDirection(ACTOR_STUCCO_ROOMS_B_MEDAL_WOMAN, FACING_NORTH, 0);
  Event_WaitFrames(30);

  // She notices the party.
  Actor_ShowEmote(ACTOR_STUCCO_ROOMS_B_MEDAL_WOMAN,
                  EMOTE_IN_FRONT | EMOTE_SMILE, 50);
  Event_WaitFrames(20);
  Actor_WalkByAndWait(ACTOR_PARTY_LEADER, 0, -16);
  Event_WaitFrames(20);
  Actor_FaceDirection(ACTOR_STUCCO_ROOMS_B_MEDAL_WOMAN,
                      FACING_SOUTHEAST + FACING_STEP, 0);
  Event_WaitFrames(30);
  Actor_RunRepeatedMotion(ACTOR_STUCCO_ROOMS_B_MEDAL_WOMAN, 2);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_STUCCO_ROOMS_B_MEDAL_WOMAN, 0);
  Event_WaitFrames(20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 50);
  Event_WaitFrames(20);
  Actor_SetAnimationAndWait(ACTOR_STUCCO_ROOMS_B_MEDAL_WOMAN, ANIM_SHAKE_HEAD);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_STUCCO_ROOMS_B_MEDAL_WOMAN, 0);
  Event_WaitFrames(30);
  Actor_ShowEmote(ACTOR_STUCCO_ROOMS_B_MEDAL_WOMAN,
                  EMOTE_IN_FRONT | EMOTE_WINCE, 50);
  Event_ShowMessage(ACTOR_STUCCO_ROOMS_B_MEDAL_WOMAN, 0);

  // She hides it and comes back, then leaves.
  Actor_SetSpeed(ACTOR_STUCCO_ROOMS_B_MEDAL_WOMAN, PIXELS(7) / 5,
                 PIXELS(7) / 10);
  Actor_WalkByAndWait(ACTOR_STUCCO_ROOMS_B_MEDAL_WOMAN, 16, 0);
  Actor_WalkByAndWait(ACTOR_STUCCO_ROOMS_B_MEDAL_WOMAN, 0, 32);
  Event_WaitFrames(20);
  Actor_SetAnimationAndWait(ACTOR_STUCCO_ROOMS_B_MEDAL_WOMAN, ANIM_NOD);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_STUCCO_ROOMS_B_MEDAL_WOMAN, 0);
  Actor_WalkByAndWait(ACTOR_PARTY_LEADER, 16, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  Event_WaitFrames(20);
  Actor_SetSpeed(ACTOR_STUCCO_ROOMS_B_MEDAL_WOMAN, PIXELS(9) / 5,
                 PIXELS(9) / 10);
  Actor_WalkByAndWait(ACTOR_STUCCO_ROOMS_B_MEDAL_WOMAN, 0, 48);
  Actor_SetPosition(ACTOR_STUCCO_ROOMS_B_MEDAL_WOMAN, 0, 0);
  Event_End();
}

/// In Tolbi after Colosso, entering the southeast guest room
/// (STUCCO_ROOMS_B_GUEST_ROOM_SOUTHEAST_FROM_INN_PASSAGE): three
/// stranded passengers argue about the captain, who won't sail the ship. "He
/// won't sail the ship, even though we agreed to do whatever it takes?"
static void StuccoRoomsB_WatchPassengersArgue(void) {
  Event_Start();
  Event_SetMessageId(MsgTolbiHeWontSailShipEven);
  Event_WaitFrames(30);
  Actor_Jump(ACTOR_STUCCO_ROOMS_B_ANGRY_PASSENGER, 4, 13);
  Actor_Jump(ACTOR_STUCCO_ROOMS_B_ANGRY_PASSENGER, 4, 30);
  Event_ShowMessageAndWait(ACTOR_STUCCO_ROOMS_B_ANGRY_PASSENGER, 0);
  Event_WaitFrames(10);

  FieldEffect_SpawnAttached(ACTOR_STUCCO_ROOMS_B_PASSENGER_WIFE,
                            EMOTE_IN_FRONT | EMOTE_WINCE, 50);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_STUCCO_ROOMS_B_PASSENGER_WIFE, ANIM_NOD);
  Event_WaitFrames(30);
  Event_ShowMessageAndWait(ACTOR_STUCCO_ROOMS_B_PASSENGER_WIFE, 0);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_STUCCO_ROOMS_B_THIRD_PASSENGER,
                            ANIM_SHAKE_HEAD);
  Event_WaitFrames(20);
  Event_ShowMessageAndWait(ACTOR_STUCCO_ROOMS_B_THIRD_PASSENGER, 0);
  Event_WaitFrames(10);
  Actor_RunRepeatedMotion(ACTOR_STUCCO_ROOMS_B_ANGRY_PASSENGER, 2);
  Event_WaitFrames(20);
  Event_ShowMessageAndWait(ACTOR_STUCCO_ROOMS_B_ANGRY_PASSENGER, 0);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_STUCCO_ROOMS_B_PASSENGER_WIFE, ANIM_NOD);
  Event_WaitFrames(30);
  Event_End();
}

/// Sets up the rooms on entry. After Colosso: a row of three map cells takes
/// new cell behaviors, two of the passengers are drawn at a fixed priority,
/// the medal woman stands where she hid the medal, the two short events play
/// once each on their entrances, and the passenger who rode the ship
/// with the party and ACTOR_STUCCO_ROOMS_B_COLOSSO_FAN have left once the
/// party has seen the tour guide on the Tolbi docks tell a passenger his ticket
/// was refunded.
s32 StuccoRoomsB_ApplyEntryState(void) {
  s32 clearedPriorityFlags;
  struct Actor *actor;

  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  if (GameFlag_Get(FLAG_COLOSSO_OVER) != 0) {
    // Restore the room's changed cells and the passengers' fixed draw priority.
    MapGrid_CopyCellAttributes(51, 47, 3, 1, 51, 45);
    actor = Actor_GetById(ACTOR_STUCCO_ROOMS_B_ANGRY_PASSENGER);
    // Stored as a plain byte: the field store allocates the zero differently.
    *(u8 *)&actor->priorityFlags = clearedPriorityFlags = 0;
    actor->sprite->image.priority = 2;
    actor = Actor_GetById(ACTOR_STUCCO_ROOMS_B_PASSENGER_WIFE);
    actor->priorityFlags = clearedPriorityFlags;
    actor->sprite->image.priority = 2;
    if (Event_GetFlag(FLAG_STUCCO_ROOMS_B_MEDAL_HIDDEN) != 0) {
      Actor_SetPosition(ACTOR_STUCCO_ROOMS_B_MEDAL_WOMAN, PIXELS(560),
                        PIXELS(680));
      Actor_FaceDirection(ACTOR_STUCCO_ROOMS_B_MEDAL_WOMAN, FACING_WEST, 0);
    }
    // Each short event plays once, only when entering its room.
    if (gAdventure.entrance ==
        STUCCO_ROOMS_B_GUEST_ROOM_NORTHEAST_FROM_INN_PASSAGE) {
      if (Event_GetFlag(FLAG_STUCCO_ROOMS_B_MEDAL_HIDDEN) == 0) {
        GameFlag_Set(FLAG_STUCCO_ROOMS_B_MEDAL_HIDDEN);
        Event_StartScreenTransition();
        StuccoRoomsB_WatchWomanHideMedal();
      }
    }
    if (gAdventure.entrance ==
        STUCCO_ROOMS_B_GUEST_ROOM_SOUTHEAST_FROM_INN_PASSAGE) {
      if (Event_GetFlag(FLAG_STUCCO_ROOMS_B_PASSENGERS_SEEN) == 0) {
        GameFlag_Set(FLAG_STUCCO_ROOMS_B_PASSENGERS_SEEN);
        Event_StartScreenTransition();
        StuccoRoomsB_WatchPassengersArgue();
      }
    }
    // Once the docks' confrontation (the refund) is seen, these two have left.
    if (GameFlag_Get(FLAG_TOLBI_DOCKS_TOUR_GROUP_BLAMED) != 0) {
      Import_ActorSetPosition(ACTOR_STUCCO_ROOMS_B_FELLOW_PASSENGER, 0, 0);
      Import_ActorSetPosition(ACTOR_STUCCO_ROOMS_B_COLOSSO_FAN, 0, 0);
    }
  }
  return 0;
}

/// In Tolbi, the item merchant: from across the counter (the leader facing
/// west) opens STUCCO_ROOMS_B_ITEM_SHOP; from elsewhere talks about the
/// finals.
static void StuccoRoomsB_TalkToItemMerchant(s32 speaker) {
  struct Actor *leader = Actor_GetById(ACTOR_PARTY_LEADER);
  if (FACING_NEAREST_CARDINAL(leader->facing) == FACING_WEST) {
    Shop_Run(STUCCO_ROOMS_B_ITEM_SHOP, speaker);
  } else if (GameFlag_Get(FLAG_COLOSSO_OVER) != 0) {
    Event_SetMessageId(MsgTolbiRightOneWell);
    Event_ShowMessageAndWait(speaker, 0);
  } else if (GameFlag_Get(FLAG_BABI_SAVED) != 0) {
    Event_SetMessageId(MsgTolbiRequireLotHealing);
    Event_ShowMessageAndWait(speaker, 0);
  } else {
    StuccoRoomsB_AskYesNo(MsgTolbiFirstTimeTolbi, speaker);
  }
}

/// In Tolbi, the weapon merchant: across the counter (the leader facing
/// north) opens the shop; otherwise talks about Colosso.
static void StuccoRoomsB_TalkToWeaponMerchant(s32 speaker) {
  struct Actor *leader = Actor_GetById(ACTOR_PARTY_LEADER);
  if (FACING_NEAREST_CARDINAL(leader->facing) == FACING_NORTH) {
    Shop_Run(STUCCO_ROOMS_B_WEAPON_SHOP, speaker);
  } else if (GameFlag_Get(FLAG_COLOSSO_OVER) != 0) {
    StuccoRoomsB_AskYesNo(MsgTolbiComeWayKalay, speaker);
  } else if (GameFlag_Get(FLAG_BABI_SAVED) != 0) {
    Event_SetMessageId(MsgTolbiColossoFinalsFinally);
    Event_ShowMessageAndWait(speaker, 0);
  } else {
    Event_SetMessageId(MsgTolbiLookStrongGo);
    Event_ShowMessageAndWait(speaker, 0);
    FieldEffect_SpawnAttached(speaker, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 0);
    Event_WaitFrames(40);
    Event_ShowMessageAndWait(speaker, 0);
  }
}

/// In Tolbi, the armor merchant: across the counter (the leader facing north)
/// opens the shop; otherwise talks about the finals.
static void StuccoRoomsB_TalkToArmorMerchant(s32 speaker) {
  struct Actor *leader = Actor_GetById(ACTOR_PARTY_LEADER);
  if (FACING_NEAREST_CARDINAL(leader->facing) == FACING_NORTH) {
    Shop_Run(STUCCO_ROOMS_B_ARMOR_SHOP, speaker);
  } else {
    if (GameFlag_Get(FLAG_COLOSSO_OVER) != 0) {
      Event_SetMessageId(MsgTolbiWasntAbleWatch);
      Event_ShowMessageAndWait(speaker, 0);
    } else if (GameFlag_Get(FLAG_BABI_SAVED) != 0) {
      Event_SetMessageId(MsgTolbiMaybeCloseShop);
      Event_ShowMessageAndWait(speaker, 0);
    } else {
      Event_SetMessageId(MsgTolbiIfCanMakeNameFor);
      Event_ShowMessageAndWait(speaker, 0);
    }
  }
}

/// In Tolbi, the chef asks whether the party watched Colosso, which he missed.
static void StuccoRoomsB_TalkToChef(s32 speaker) {
  Event_Start();
  StuccoRoomsB_AskYesNo(MsgTolbiLookLikeWarrior, speaker);
  Event_End();
}

/// In Tolbi after Colosso, the waitress grumbles about the chef's bad mood.
static void StuccoRoomsB_TalkToWaitress(s32 speaker) {
  Event_Start();
  Event_SetMessageId(MsgTolbiGrrrChefInBadMood);
  Event_ShowMessageAndWait(speaker, 0);
  Event_End();
}

/// In Tolbi during the festival, a resident offers the party a bed.
static void StuccoRoomsB_TalkToHost(s32 speaker) {
  Event_Start();
  StuccoRoomsB_AskYesNo(MsgTolbiWantStay, speaker);
  Event_End();
}

/// In Tolbi, the woman who hid the Lucky Medal. Until the party takes it from
/// the barrel she teases; once it is gone she greets them, startled, the first
/// time, and then claims she is fine.
static void StuccoRoomsB_TalkToMedalWoman(s32 speaker) {
  Event_Start();
  if (GameFlag_Get(FLAG_STUCCO_ROOMS_B_MEDAL_TAKEN) == 0) {
    StuccoRoomsB_AskYesNo(MsgTolbiHeeHeeLook, speaker);
  } else {
    if (Event_GetFlag(FLAG_STUCCO_ROOMS_B_MEDAL_WOMAN_SAID_HELLO) == 0) {
      GameFlag_Set(FLAG_STUCCO_ROOMS_B_MEDAL_WOMAN_SAID_HELLO);
      Event_SetMessageId(MsgTolbiHello);
      Event_ShowMessage(speaker, 0);
      Event_WaitFrames(10);
      Actor_RunRepeatedMotion(speaker, 2);
      Event_WaitFrames(20);
    }
    Event_SetMessageId(MsgTolbiHoHumFine);
    Event_ShowMessage(speaker, 0);
  }
  Event_End();
}

/// In Tolbi, Mind Read on the medal woman gives the hiding place away.
static void StuccoRoomsB_ReadMedalWomanMind(void) {
  Event_Start();
  if (GameFlag_Get(FLAG_STUCCO_ROOMS_B_MEDAL_WOMAN_SAID_HELLO) == 0) {
    Event_SetMessageId(MsgTolbiHehHehSheJustHid);
  } else {
    Event_SetMessageId(MsgTolbiThingFoundDefinitelySameAs);
  }
  Event_ShowMessageAndWait(ACTOR_STUCCO_ROOMS_B_MEDAL_WOMAN, 0);
  Event_End();
}

/// In Tolbi after Colosso, the angry passenger complains the ships won't sail.
/// "Grrr! What a scam!"
static void StuccoRoomsB_TalkToAngryPassenger(s32 speaker) {
  Event_Start();
  Event_SetMessageId(MsgTolbiGrrrScamWhyWontThey);
  Actor_ShowEmote(ACTOR_STUCCO_ROOMS_B_ANGRY_PASSENGER,
                  EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 40);
  Event_ShowMessage(speaker, 0);
  Event_End();
}

/// The man from the ship asks whether the party remembers him. He's in Tolbi
/// after Colosso, and rode the ship from Kalay with the party. "Hey! You're the
/// would-be warrior!"
static void StuccoRoomsB_TalkToFellowPassenger(s32 speaker) {
  Event_Start();
  StuccoRoomsB_AskYesNo(MsgTolbiWarriorRemember, speaker);
  Event_End();
}

/// In Tolbi Sanctum, the healer: across the counter (the leader facing north)
/// opens the sanctum; otherwise talks about the festival.
static void StuccoRoomsB_TalkToHealer(s32 speaker) {
  struct Actor *leader = Actor_GetById(ACTOR_PARTY_LEADER);
  if (FACING_NEAREST_CARDINAL(leader->facing) == FACING_NORTH) {
    Sanctum_Run(speaker);
  } else {
    if (GameFlag_Get(FLAG_COLOSSO_OVER) != 0) {
      Event_SetMessageId(MsgTolbiShipsArentGoing);
      Event_ShowMessageAndWait(speaker, 0);
    } else if (GameFlag_Get(FLAG_BABI_SAVED) != 0) {
      Event_SetMessageId(MsgTolbiMissFinalsTolbis);
      Event_ShowMessageAndWait(speaker, 0);
    } else {
      Event_SetMessageId(MsgTolbiWasteStuckHereWhenSuch);
      Event_ShowMessageAndWait(speaker, 0);
    }
  }
}

/// An actor that squashes and stretches twice, then rests 40 frames.
s32 gStuccoRoomsBSquashStretch[] = {
    SC_SET(SW_SCALE_X, 0x10000),
    SC_SET(SW_SCALE_Y, 0x10000),
    SC_ADD(SW_SCALE_X, -0x5000),
    SC_ADD(SW_SCALE_Y, 0x1000),
    SC_WAIT(1),
    SC_ADD(SW_SCALE_X, 0x2800),
    SC_ADD(SW_SCALE_Y, -2048),
    SC_WAIT(1),
    SC_ADD(SW_SCALE_X, 0x1400),
    SC_ADD(SW_SCALE_Y, -1024),
    SC_WAIT(1),
    SC_ADD(SW_SCALE_X, 0x1400),
    SC_ADD(SW_SCALE_Y, -1024),
    SC_WAIT(1),
    SC_ADD(SW_SCALE_X, -0x5000),
    SC_ADD(SW_SCALE_Y, 0x1000),
    SC_WAIT(1),
    SC_ADD(SW_SCALE_X, 0x2800),
    SC_ADD(SW_SCALE_Y, -2048),
    SC_WAIT(1),
    SC_ADD(SW_SCALE_X, 0x1400),
    SC_ADD(SW_SCALE_Y, -1024),
    SC_WAIT(1),
    SC_ADD(SW_SCALE_X, 0x1400),
    SC_ADD(SW_SCALE_Y, -1024),
    SC_WAIT(40),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

struct SceneEntrance gStuccoRoomsBEntrances[] = {
    {ENTRANCE_DEFAULT, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1,
     -1, -1, 0},
    {STUCCO_ROOMS_B_HOUSE_NORTHEAST, CONDITION_ALWAYS, 160, 0, 216,
     FACING_NORTH, 0, 0, 0, 256, 256, 0},
    {STUCCO_ROOMS_B_HOUSE_NORTHWEST, CONDITION_ALWAYS, 336, 0, 200,
     FACING_NORTH, 0, 248, 64, 488, 216, 0},
    {STUCCO_ROOMS_B_HOUSE_EAST, CONDITION_ALWAYS, 544, 0, 200, FACING_NORTH, 0,
     520, 48, 696, 216, 0},
    {STUCCO_ROOMS_B_HOUSE_SOUTH, CONDITION_ALWAYS, 792, 0, 216, FACING_NORTH, 0,
     688, 0, 960, 240, 0},
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
    SCENE_EXIT(STUCCO_ROOMS_B_INN_TO_TOLBI, Tolbi, TOLBI_FROM_INN),
    SCENE_EXIT(STUCCO_ROOMS_B_TO_TOLBI_EAST, Tolbi,
               TOLBI_FROM_STUCCO_ROOMS_B_EAST),
    SCENE_EXIT(STUCCO_ROOMS_B_HOUSE_NORTHEAST_TO_TOLBI, Tolbi,
               TOLBI_FROM_HOUSE_NORTHEAST),
    SCENE_EXIT(STUCCO_ROOMS_B_HOUSE_EAST_TO_TOLBI, Tolbi,
               TOLBI_FROM_HOUSE_EAST),
    SCENE_EXIT(STUCCO_ROOMS_B_WEAPON_AND_ARMOR_SHOP_TO_TOLBI, Tolbi,
               TOLBI_FROM_WEAPON_AND_ARMOR_SHOP),
    SCENE_EXIT(STUCCO_ROOMS_B_HOUSE_SOUTHWEST_TO_TOLBI, Tolbi,
               TOLBI_FROM_HOUSE_SOUTHWEST),
    SCENE_EXIT(STUCCO_ROOMS_B_SANCTUM_TO_TOLBI, Tolbi, TOLBI_FROM_SANCTUM),
    SCENE_EXIT(STUCCO_ROOMS_B_TO_TOLBI_WEST, Tolbi,
               TOLBI_FROM_STUCCO_ROOMS_B_WEST),
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
struct ActorTemplate gStuccoRoomsBActorTemplates[] = {
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(200), 0, PIXELS(168), FACING_NORTH + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(152), 0, PIXELS(184), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(184), 0, PIXELS(104), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_STOUT_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(144), 0, PIXELS(104), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(352), 0, PIXELS(120), FACING_NORTH + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(568), 0, PIXELS(120), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(600), 0, PIXELS(198), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(824), 0, PIXELS(144), FACING_NORTH + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(840), 0, PIXELS(106), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_STOUT_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(608), 0, PIXELS(348), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_STOUT_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(536), 0, PIXELS(392), FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_BALD_MAN_IN_RED, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(280), 0, PIXELS(360), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_YOUNG_MAN_IN_GREEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(392), 0, PIXELS(360), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_WITH_PONYTAIL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(264), 0, PIXELS(440), FACING_EAST, TALK_FACING_PARTY, 0 },
    { SPRITE_MAN_WITH_DARK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(106), 0, PIXELS(696), FACING_SOUTHEAST, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_IN_PINK_APRON, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(392), 0, PIXELS(832), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(488), 0, PIXELS(688), FACING_NORTH + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(584), 0, PIXELS(848), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_CHEF, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(232), 0, PIXELS(592), FACING_NORTH + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_YOUNG_WOMAN_IN_GREEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(230), 0, PIXELS(648), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_YOUNG_MAN_WITH_BLUE_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(864), 0, PIXELS(848), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_YOUNG_MAN_WITH_RED_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(848), 0, PIXELS(864), FACING_NORTH + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_MAN_IN_TALL_HAT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(824), 0, PIXELS(328), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct ActorTemplate gStuccoRoomsBActorTemplatesAfterColosso[] = {
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(200), 0, PIXELS(168), FACING_NORTH + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(152), 0, PIXELS(184), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(392), 0, PIXELS(168), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(344), 0, PIXELS(120), FACING_NORTH + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(520), 0, PIXELS(520), FACING_NORTH + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(568), 0, PIXELS(120), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(600), 0, PIXELS(198), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(824), 0, PIXELS(144), FACING_NORTH + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(840), 0, PIXELS(106), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_STOUT_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(608), 0, PIXELS(348), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_STOUT_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(536), 0, PIXELS(392), FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_BALD_MAN_IN_RED, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(280), 0, PIXELS(360), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_YOUNG_MAN_IN_GREEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(392), 0, PIXELS(360), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_WITH_PONYTAIL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(264), 0, PIXELS(440), FACING_EAST, TALK_FACING_PARTY, 0 },
    { SPRITE_MAN_WITH_DARK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(106), 0, PIXELS(696), FACING_SOUTHEAST, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_IN_PINK_APRON, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(112), 0, PIXELS(600), FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(488), 0, PIXELS(688), FACING_NORTH + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(568), 0, PIXELS(848), FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_CHEF, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(232), 0, PIXELS(592), FACING_NORTH + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_YOUNG_WOMAN_IN_GREEN, CONDITION_ALWAYS, (s32)gStuccoRoomsBSquashStretch,
        PIXELS(230), 0, PIXELS(648), FACING_NORTH, TALK_FACING_KEEP, 0 },
    { SPRITE_YOUNG_MAN_WITH_BLUE_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(864), 0, PIXELS(848), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_YOUNG_MAN_WITH_RED_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(848), 0, PIXELS(864), FACING_NORTH + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_MAN_IN_TALL_HAT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(824), 0, PIXELS(328), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_MAN_CARRYING_A_PACK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(848), 0, PIXELS(712), FACING_WEST, TALK_FACING_PARTY, 0 },
    { SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(816), 0, PIXELS(712), FACING_EAST, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(848), 0, PIXELS(688), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(856), 0, PIXELS(672), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(440), 0, PIXELS(864), FACING_NORTHWEST + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(416), 0, PIXELS(840), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SCENE_TABLE_END },
};

struct Trigger gStuccoRoomsBTriggersFestival[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, STUCCO_ROOMS_B_HOUSE_NORTHWEST_TO_TOLBI },
    { TRIGGER_EXIT, 2, CONDITION_ALWAYS, STUCCO_ROOMS_B_INN_TO_TOLBI },
    { TRIGGER_EXIT, 3, CONDITION_ALWAYS, STUCCO_ROOMS_B_TO_TOLBI_EAST },
    { TRIGGER_EXIT, 4, CONDITION_ALWAYS, STUCCO_ROOMS_B_HOUSE_NORTHEAST_TO_TOLBI },
    { TRIGGER_EXIT, 5, CONDITION_ALWAYS, STUCCO_ROOMS_B_HOUSE_EAST_TO_TOLBI },
    { TRIGGER_EXIT, 6, CONDITION_ALWAYS, STUCCO_ROOMS_B_WEAPON_AND_ARMOR_SHOP_TO_TOLBI },
    { TRIGGER_EXIT, 7, CONDITION_ALWAYS, STUCCO_ROOMS_B_HOUSE_SOUTHWEST_TO_TOLBI },
    { TRIGGER_EXIT, 8, CONDITION_ALWAYS, STUCCO_ROOMS_B_SANCTUM_TO_TOLBI },
    { TRIGGER_EXIT, 9, CONDITION_ALWAYS, STUCCO_ROOMS_B_TO_TOLBI_WEST },
    { TRIGGER_EXIT, 10, CONDITION_ALWAYS, STUCCO_ROOMS_B_INN_TO_INN_PASSAGE },
    { TRIGGER_EXIT, 11, CONDITION_ALWAYS, STUCCO_ROOMS_B_INN_PASSAGE_TO_INN },
    { TRIGGER_EXIT, 12, CONDITION_ALWAYS, STUCCO_ROOMS_B_INN_PASSAGE_TO_GUEST_ROOM_SOUTHWEST },
    { TRIGGER_EXIT, 13, CONDITION_ALWAYS, STUCCO_ROOMS_B_INN_PASSAGE_TO_GUEST_ROOM_SOUTHEAST },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 14, CONDITION_ALWAYS, EVENT(StuccoRoomsB_EnterDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 15, CONDITION_ALWAYS, EVENT(StuccoRoomsB_EnterDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 16, CONDITION_ALWAYS, EVENT(StuccoRoomsB_EnterDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 17, CONDITION_ALWAYS, EVENT(StuccoRoomsB_EnterDoor) },
    { TRIGGER_EXIT, 18, CONDITION_ALWAYS, STUCCO_ROOMS_B_GUEST_ROOM_NORTHWEST_TO_INN_PASSAGE },
    { TRIGGER_EXIT, 19, CONDITION_ALWAYS, STUCCO_ROOMS_B_GUEST_ROOM_NORTHEAST_TO_INN_PASSAGE },
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
    { TRIGGERS_END },
};

struct Trigger gStuccoRoomsBTriggersFinalsDay[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, STUCCO_ROOMS_B_HOUSE_NORTHWEST_TO_TOLBI },
    { TRIGGER_EXIT, 2, CONDITION_ALWAYS, STUCCO_ROOMS_B_INN_TO_TOLBI },
    { TRIGGER_EXIT, 3, CONDITION_ALWAYS, STUCCO_ROOMS_B_TO_TOLBI_EAST },
    { TRIGGER_EXIT, 4, CONDITION_ALWAYS, STUCCO_ROOMS_B_HOUSE_NORTHEAST_TO_TOLBI },
    { TRIGGER_EXIT, 5, CONDITION_ALWAYS, STUCCO_ROOMS_B_HOUSE_EAST_TO_TOLBI },
    { TRIGGER_EXIT, 6, CONDITION_ALWAYS, STUCCO_ROOMS_B_WEAPON_AND_ARMOR_SHOP_TO_TOLBI },
    { TRIGGER_EXIT, 7, CONDITION_ALWAYS, STUCCO_ROOMS_B_HOUSE_SOUTHWEST_TO_TOLBI },
    { TRIGGER_EXIT, 8, CONDITION_ALWAYS, STUCCO_ROOMS_B_SANCTUM_TO_TOLBI },
    { TRIGGER_EXIT, 9, CONDITION_ALWAYS, STUCCO_ROOMS_B_TO_TOLBI_WEST },
    { TRIGGER_EXIT, 10, CONDITION_ALWAYS, STUCCO_ROOMS_B_INN_TO_INN_PASSAGE },
    { TRIGGER_EXIT, 11, CONDITION_ALWAYS, STUCCO_ROOMS_B_INN_PASSAGE_TO_INN },
    { TRIGGER_EXIT, 12, CONDITION_ALWAYS, STUCCO_ROOMS_B_INN_PASSAGE_TO_GUEST_ROOM_SOUTHWEST },
    { TRIGGER_EXIT, 13, CONDITION_ALWAYS, STUCCO_ROOMS_B_INN_PASSAGE_TO_GUEST_ROOM_SOUTHEAST },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 14, CONDITION_ALWAYS, EVENT(StuccoRoomsB_EnterDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 15, CONDITION_ALWAYS, EVENT(StuccoRoomsB_EnterDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 16, CONDITION_ALWAYS, EVENT(StuccoRoomsB_EnterDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 17, CONDITION_ALWAYS, EVENT(StuccoRoomsB_EnterDoor) },
    { TRIGGER_EXIT, 18, CONDITION_ALWAYS, STUCCO_ROOMS_B_GUEST_ROOM_NORTHWEST_TO_INN_PASSAGE },
    { TRIGGER_EXIT, 19, CONDITION_ALWAYS, STUCCO_ROOMS_B_GUEST_ROOM_NORTHEAST_TO_INN_PASSAGE },
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
    { TRIGGERS_END },
};

struct Trigger gStuccoRoomsBTriggersAfterColosso[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, STUCCO_ROOMS_B_HOUSE_NORTHWEST_TO_TOLBI },
    { TRIGGER_EXIT, 2, CONDITION_ALWAYS, STUCCO_ROOMS_B_INN_TO_TOLBI },
    { TRIGGER_EXIT, 3, CONDITION_ALWAYS, STUCCO_ROOMS_B_TO_TOLBI_EAST },
    { TRIGGER_EXIT, 4, CONDITION_ALWAYS, STUCCO_ROOMS_B_HOUSE_NORTHEAST_TO_TOLBI },
    { TRIGGER_EXIT, 5, CONDITION_ALWAYS, STUCCO_ROOMS_B_HOUSE_EAST_TO_TOLBI },
    { TRIGGER_EXIT, 6, CONDITION_ALWAYS, STUCCO_ROOMS_B_WEAPON_AND_ARMOR_SHOP_TO_TOLBI },
    { TRIGGER_EXIT, 7, CONDITION_ALWAYS, STUCCO_ROOMS_B_HOUSE_SOUTHWEST_TO_TOLBI },
    { TRIGGER_EXIT, 8, CONDITION_ALWAYS, STUCCO_ROOMS_B_SANCTUM_TO_TOLBI },
    { TRIGGER_EXIT, 9, CONDITION_ALWAYS, STUCCO_ROOMS_B_TO_TOLBI_WEST },
    { TRIGGER_EXIT, 10, CONDITION_ALWAYS, STUCCO_ROOMS_B_INN_TO_INN_PASSAGE },
    { TRIGGER_EXIT, 11, CONDITION_ALWAYS, STUCCO_ROOMS_B_INN_PASSAGE_TO_INN },
    { TRIGGER_EXIT, 12, CONDITION_ALWAYS, STUCCO_ROOMS_B_INN_PASSAGE_TO_GUEST_ROOM_SOUTHWEST },
    { TRIGGER_EXIT, 13, CONDITION_ALWAYS, STUCCO_ROOMS_B_INN_PASSAGE_TO_GUEST_ROOM_SOUTHEAST },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 14, CONDITION_ALWAYS, EVENT(StuccoRoomsB_EnterDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 15, CONDITION_ALWAYS, EVENT(StuccoRoomsB_EnterDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 16, CONDITION_ALWAYS, EVENT(StuccoRoomsB_EnterDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 17, CONDITION_ALWAYS, EVENT(StuccoRoomsB_EnterDoor) },
    { TRIGGER_EXIT, 18, CONDITION_ALWAYS, STUCCO_ROOMS_B_GUEST_ROOM_NORTHWEST_TO_INN_PASSAGE },
    { TRIGGER_EXIT, 19, CONDITION_ALWAYS, STUCCO_ROOMS_B_GUEST_ROOM_NORTHEAST_TO_INN_PASSAGE },
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
    { TRIGGERS_END },
};
// clang-format on

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
