/// @file
/// Stucco rooms B (SceneId_StuccoRoomsB), a sheet of rooms drawn in stucco, all
/// of them Tolbi's: its houses, inns and shops, which the area names call
/// Tolbi, and Tolbi Sanctum, as the same map is named at STUCCO_ROOMS_B_SANCTUM
/// and STUCCO_ROOMS_B_SANCTUM_NORTH.
///
/// - Tolbi, the rooms behind the town's doors, which Tolbi's warps
///   TOLBI_TO_HOUSE_SOUTH to TOLBI_TO_HOUSE_SOUTHWEST lead into; this map's
///   warps STUCCO_ROOMS_B_HOUSE_NORTHWEST_TO_TOLBI to
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
/// with its own event table (StuccoRoomsB_SelectEvents) and, after
/// Colosso, an object event template table of its own. During the festival Babi
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
/// (StuccoRoomsB_WatchPassengersArgue, a map section flag). A man who rode
/// the ship from Kalay with the party greets it as `the would-be warrior`, and
/// he and LOCALID_STUCCO_ROOMS_B_COLOSSO_FAN leave once the party has watched
/// the passengers confront the tour guide on the Tolbi docks
/// (FLAG_TOLBI_DOCKS_TOUR_GROUP_BLAMED). Barrels hold a Vial and a Lucky
/// Medal, and searching the ovens shows what is cooking.

#include "save_block.h"
#include "unit/scene/stucco_rooms_b.h"
#include "unit/overworld/overworld.h"
#include "unit/overworld/field_services.h"
#include "stucco_rooms_b.h"
#include "script_layer.h"
#include "text/msg_ids.h"
#include "types.h"
#include "ability_ids.h"
#include "edition.h"
#include "map_tables.h"
#include "item_ids.h"
#include "objcmd.h"
#include "songs.h"

// Map section flags: Scene_ResetFlagsOnEnter clears 0x300 to 0x3ff when the
// party enters another map section, and other map sections use the numbers for
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

/// The shop rows used by the room event tables below.
enum {
  STUCCO_ROOMS_B_WEAPON_SHOP = 26,
  STUCCO_ROOMS_B_ARMOR_SHOP = 27,
  STUCCO_ROOMS_B_ITEM_SHOP = 28,
  STUCCO_ROOMS_B_OBJ_EVENT_SLOT_END = 66
};

/// Asks a yes/no question, then answers with the message after the question
/// for yes or the one after that for no.
static inline void StuccoRoomsB_AskYesNo(s32 question, s32 speaker) {
  FieldScript_SetMessageId(question);
  FieldMessage_ShowAtObject(speaker, 0);
  if (FieldScript_AskYesNoWithObjectEventAnimation(0, 0) == 0) {
    FieldScript_WaitFrames(10);
    FieldScript_SetMessageId(question + 1);
  } else {
    FieldScript_SetMessageId(question + 2);
  }
  FieldScript_ShowMessageAndWait(speaker, 0);
}

const struct MapWarpDest *StuccoRoomsB_GetWarpDests(void) {
  return gStuccoRoomsBWarpDests;
}

const struct MapWarpRegion *StuccoRoomsB_GetRegions(void) { return 0; }

const u32 *StuccoRoomsB_GetWarps(void) { return gStuccoRoomsBWarps; }

/// After Colosso the rooms hold a different crowd.
const struct ObjectEventTemplate *
StuccoRoomsB_SelectObjectEventTemplates(void) {
  if (GameFlag_Get(FLAG_COLOSSO_OVER) != 0) {
    return gStuccoRoomsBObjectEventTemplatesAfterColosso;
  }
  return gStuccoRoomsBObjectEventTemplates;
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

/// Picks the event table for the current point in the Colosso story.
const struct MapEvent *StuccoRoomsB_SelectEvents(void) {
  if (GameFlag_Get(FLAG_COLOSSO_OVER) != 0) {
    return gStuccoRoomsBEventsAfterColosso;
  }
  if (GameFlag_Get(FLAG_BABI_SAVED) != 0) {
    return gStuccoRoomsBEventsFinalsDay;
  }
  return gStuccoRoomsBEventsFestival;
}

/// Unused: leaves through TRANSITION_WINDOW, a screen transition of 24
/// frames; no event row names it.
UNUSED static void StuccoRoomsB_ExitThroughWindow(void) {
  struct FieldWork *work = gFieldWork;

  work->transition = MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
  work->transitionFrames = 24;
  Import_ScriptRequestWarp();
}

/// In Tolbi, walking into one of the four doors at metatile behaviors 14 to 17:
/// every placed object event stops, the door opens and the leader walks through
/// it.
static void StuccoRoomsB_EnterDoor(void) {
  u32 localId;
  struct ObjectEvent *objectEvent;
  struct FieldWork *work;
  s16 *trigger;
  s32 doorIndex;

  work = gFieldWork;
  // Stop the room's object events before animating the selected doorway.
  for (localId = LOCALID_FIRST_PLACED;
       localId < STUCCO_ROOMS_B_OBJ_EVENT_SLOT_END; localId++) {
    objectEvent = ObjectEvent_GetById(localId);
    if (objectEvent != 0) {
      objectEvent->motionFlags = 0;
    }
  }
  trigger = &work->coordEvent;
  doorIndex = *trigger - MB_STUCCO_ROOMS_B_FIRST_DOOR;
  Sound_PlaySong(SE_DOOR_OPEN);
  MapGrid_AnimateCells(gStuccoRoomsBDoors[doorIndex].steps,
                       gStuccoRoomsBDoors[doorIndex].x,
                       gStuccoRoomsBDoors[doorIndex].y);
  ObjectMotion_SetSpeedParameters(LOCALID_PARTY_LEADER, PIXELS(1) / 2,
                                  PIXELS(1) / 4);
  ObjectEvent_GetById(LOCALID_PARTY_LEADER)->motionFlags = 0;
  Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_WALK);
  Import_ScriptRequestWarp(*trigger);
}

/// In Tolbi after Colosso, entering the northeast guest room
/// (STUCCO_ROOMS_B_GUEST_ROOM_NORTHEAST_FROM_INN_PASSAGE): a woman
/// finds something on the floor, is startled to see the party and hides it in
/// the barrel behind the trash pile. "Hey, what's this?" The thing is a Lucky
/// Medal, found later in that barrel.
static void StuccoRoomsB_WatchWomanHideMedal(void) {
  struct ObjectEvent *sparkle;

  FieldScript_Start();
  FieldScript_SetMessageId(MsgTolbiHeyWhatsThis);

  // A sparkle on the floor where she finds it.
  FieldScript_WaitFrames(40);
  sparkle = ObjectEvent_Create(SPRITE_PARTICLE, PIXELS(600), 0, PIXELS(824));
  ObjectEvent_SetSpriteFlags(sparkle, 0);
  Object_SelectAnimation(sparkle, 6);
  FieldScript_WaitFrames(10);
  Object_SelectAnimation(sparkle, 1);
  FieldScript_WaitFrames(40);
  ObjectEvent_Release(sparkle);
  FieldScript_WaitFrames(2);

  // She runs to it and picks it up.
  ObjectEvent_ShowEmote(LOCALID_STUCCO_ROOMS_B_MEDAL_WOMAN,
                        EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 50);
  ObjectEvent_SetSpeed(LOCALID_STUCCO_ROOMS_B_MEDAL_WOMAN, PIXELS(1),
                       PIXELS(1) / 2);
  ObjectEvent_WalkToAndWait(LOCALID_STUCCO_ROOMS_B_MEDAL_WOMAN, 600, 848);
  ObjectEvent_FaceDirection(LOCALID_STUCCO_ROOMS_B_MEDAL_WOMAN, FACING_NORTH,
                            0);
  FieldScript_WaitFrames(40);
  Script_ShowMessage(LOCALID_STUCCO_ROOMS_B_MEDAL_WOMAN, 0);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_STUCCO_ROOMS_B_MEDAL_WOMAN, 2);
  FieldScript_WaitFrames(30);
  ObjectEvent_WalkToAndWait(LOCALID_STUCCO_ROOMS_B_MEDAL_WOMAN, 568, 848);
  ObjectEvent_FaceDirection(LOCALID_STUCCO_ROOMS_B_MEDAL_WOMAN, FACING_NORTH,
                            0);
  FieldScript_WaitFrames(30);

  // She notices the party.
  ObjectEvent_ShowEmote(LOCALID_STUCCO_ROOMS_B_MEDAL_WOMAN,
                        EMOTE_IN_FRONT | EMOTE_SMILE, 50);
  FieldScript_WaitFrames(20);
  ObjectEvent_WalkByAndWait(LOCALID_PARTY_LEADER, 0, -16);
  FieldScript_WaitFrames(20);
  ObjectEvent_FaceDirection(LOCALID_STUCCO_ROOMS_B_MEDAL_WOMAN,
                            FACING_SOUTHEAST + FACING_STEP, 0);
  FieldScript_WaitFrames(30);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_STUCCO_ROOMS_B_MEDAL_WOMAN, 2);
  FieldScript_WaitFrames(20);
  Script_ShowMessage(LOCALID_STUCCO_ROOMS_B_MEDAL_WOMAN, 0);
  FieldScript_WaitFrames(20);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION,
                        50);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetAnimationAndWait(LOCALID_STUCCO_ROOMS_B_MEDAL_WOMAN,
                                   ANIM_SHAKE_HEAD);
  FieldScript_WaitFrames(20);
  Script_ShowMessage(LOCALID_STUCCO_ROOMS_B_MEDAL_WOMAN, 0);
  FieldScript_WaitFrames(30);
  ObjectEvent_ShowEmote(LOCALID_STUCCO_ROOMS_B_MEDAL_WOMAN,
                        EMOTE_IN_FRONT | EMOTE_WINCE, 50);
  Script_ShowMessage(LOCALID_STUCCO_ROOMS_B_MEDAL_WOMAN, 0);

  // She hides it and comes back, then leaves.
  ObjectEvent_SetSpeed(LOCALID_STUCCO_ROOMS_B_MEDAL_WOMAN, PIXELS(7) / 5,
                       PIXELS(7) / 10);
  ObjectEvent_WalkByAndWait(LOCALID_STUCCO_ROOMS_B_MEDAL_WOMAN, 16, 0);
  ObjectEvent_WalkByAndWait(LOCALID_STUCCO_ROOMS_B_MEDAL_WOMAN, 0, 32);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetAnimationAndWait(LOCALID_STUCCO_ROOMS_B_MEDAL_WOMAN,
                                   ANIM_NOD);
  FieldScript_WaitFrames(20);
  Script_ShowMessage(LOCALID_STUCCO_ROOMS_B_MEDAL_WOMAN, 0);
  ObjectEvent_WalkByAndWait(LOCALID_PARTY_LEADER, 16, 0);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_WEST, 0);
  FieldScript_WaitFrames(20);
  ObjectEvent_SetSpeed(LOCALID_STUCCO_ROOMS_B_MEDAL_WOMAN, PIXELS(9) / 5,
                       PIXELS(9) / 10);
  ObjectEvent_WalkByAndWait(LOCALID_STUCCO_ROOMS_B_MEDAL_WOMAN, 0, 48);
  ObjectEvent_SetPosition(LOCALID_STUCCO_ROOMS_B_MEDAL_WOMAN, 0, 0);
  FieldScript_End();
}

/// In Tolbi after Colosso, entering the southeast guest room
/// (STUCCO_ROOMS_B_GUEST_ROOM_SOUTHEAST_FROM_INN_PASSAGE): three
/// stranded passengers argue about the captain, who won't sail the ship. "He
/// won't sail the ship, even though we agreed to do whatever it takes?"
static void StuccoRoomsB_WatchPassengersArgue(void) {
  FieldScript_Start();
  FieldScript_SetMessageId(MsgTolbiHeWontSailShipEven);
  FieldScript_WaitFrames(30);
  ObjectMotion_Launch(LOCALID_STUCCO_ROOMS_B_ANGRY_PASSENGER, 4, 13);
  ObjectMotion_Launch(LOCALID_STUCCO_ROOMS_B_ANGRY_PASSENGER, 4, 30);
  FieldScript_ShowMessageAndWait(LOCALID_STUCCO_ROOMS_B_ANGRY_PASSENGER, 0);
  FieldScript_WaitFrames(10);

  FieldEffect_SpawnLinked(LOCALID_STUCCO_ROOMS_B_PASSENGER_WIFE,
                          EMOTE_IN_FRONT | EMOTE_WINCE, 50);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetAnimationAndWait(LOCALID_STUCCO_ROOMS_B_PASSENGER_WIFE,
                                   ANIM_NOD);
  FieldScript_WaitFrames(30);
  FieldScript_ShowMessageAndWait(LOCALID_STUCCO_ROOMS_B_PASSENGER_WIFE, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetAnimationAndWait(LOCALID_STUCCO_ROOMS_B_THIRD_PASSENGER,
                                   ANIM_SHAKE_HEAD);
  FieldScript_WaitFrames(20);
  FieldScript_ShowMessageAndWait(LOCALID_STUCCO_ROOMS_B_THIRD_PASSENGER, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_STUCCO_ROOMS_B_ANGRY_PASSENGER, 2);
  FieldScript_WaitFrames(20);
  FieldScript_ShowMessageAndWait(LOCALID_STUCCO_ROOMS_B_ANGRY_PASSENGER, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetAnimationAndWait(LOCALID_STUCCO_ROOMS_B_PASSENGER_WIFE,
                                   ANIM_NOD);
  FieldScript_WaitFrames(30);
  FieldScript_End();
}

/// Sets up the rooms on entry. After Colosso: a row of three map cells takes
/// new metatile behaviors, two of the passengers are drawn at a fixed priority,
/// the medal woman stands where she hid the medal, the two short events play
/// once each on their warp destinations, and the passenger who rode the ship
/// with the party and LOCALID_STUCCO_ROOMS_B_COLOSSO_FAN have left once the
/// party has seen the tour guide on the Tolbi docks tell a passenger his ticket
/// was refunded.
s32 StuccoRoomsB_ApplyEntryState(void) {
  s32 clearedPriorityFlags;
  struct ObjectEvent *objectEvent;

  gFieldWork->transition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  if (GameFlag_Get(FLAG_COLOSSO_OVER) != 0) {
    // Restore the room's changed cells and the passengers' fixed draw priority.
    MapGrid_CopyCellAttributes(51, 47, 3, 1, 51, 45);
    objectEvent = ObjectEvent_GetById(LOCALID_STUCCO_ROOMS_B_ANGRY_PASSENGER);
    // Stored as a plain byte: the field store allocates the zero differently.
    *(u8 *)&objectEvent->priorityFlags = clearedPriorityFlags = 0;
    objectEvent->sprite->image.priority = 2;
    objectEvent = ObjectEvent_GetById(LOCALID_STUCCO_ROOMS_B_PASSENGER_WIFE);
    objectEvent->priorityFlags = clearedPriorityFlags;
    objectEvent->sprite->image.priority = 2;
    if (Script_GetFlag(FLAG_STUCCO_ROOMS_B_MEDAL_HIDDEN) != 0) {
      ObjectEvent_SetPosition(LOCALID_STUCCO_ROOMS_B_MEDAL_WOMAN, PIXELS(560),
                              PIXELS(680));
      ObjectEvent_FaceDirection(LOCALID_STUCCO_ROOMS_B_MEDAL_WOMAN, FACING_WEST,
                                0);
    }
    // Each short event plays once, only when entering its room.
    if (gSaveBlock.warpDest ==
        STUCCO_ROOMS_B_GUEST_ROOM_NORTHEAST_FROM_INN_PASSAGE) {
      if (Script_GetFlag(FLAG_STUCCO_ROOMS_B_MEDAL_HIDDEN) == 0) {
        GameFlag_Set(FLAG_STUCCO_ROOMS_B_MEDAL_HIDDEN);
        FieldScript_StartScreenTransition();
        StuccoRoomsB_WatchWomanHideMedal();
      }
    }
    if (gSaveBlock.warpDest ==
        STUCCO_ROOMS_B_GUEST_ROOM_SOUTHEAST_FROM_INN_PASSAGE) {
      if (Script_GetFlag(FLAG_STUCCO_ROOMS_B_PASSENGERS_SEEN) == 0) {
        GameFlag_Set(FLAG_STUCCO_ROOMS_B_PASSENGERS_SEEN);
        FieldScript_StartScreenTransition();
        StuccoRoomsB_WatchPassengersArgue();
      }
    }
    // Once the docks' confrontation (the refund) is seen, these two have left.
    if (GameFlag_Get(FLAG_TOLBI_DOCKS_TOUR_GROUP_BLAMED) != 0) {
      Import_ObjectEventSetPosition(LOCALID_STUCCO_ROOMS_B_FELLOW_PASSENGER, 0,
                                    0);
      Import_ObjectEventSetPosition(LOCALID_STUCCO_ROOMS_B_COLOSSO_FAN, 0, 0);
    }
  }
  return 0;
}

/// In Tolbi, the item merchant: from across the counter (the leader facing
/// west) opens STUCCO_ROOMS_B_ITEM_SHOP; from elsewhere talks about the
/// finals.
static void StuccoRoomsB_TalkToItemMerchant(s32 speaker) {
  struct ObjectEvent *leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (FACING_NEAREST_CARDINAL(leader->facing) == FACING_WEST) {
    Shop_Run(STUCCO_ROOMS_B_ITEM_SHOP, speaker);
  } else if (GameFlag_Get(FLAG_COLOSSO_OVER) != 0) {
    FieldScript_SetMessageId(MsgTolbiRightOneWell);
    FieldScript_ShowMessageAndWait(speaker, 0);
  } else if (GameFlag_Get(FLAG_BABI_SAVED) != 0) {
    FieldScript_SetMessageId(MsgTolbiRequireLotHealing);
    FieldScript_ShowMessageAndWait(speaker, 0);
  } else {
    StuccoRoomsB_AskYesNo(MsgTolbiFirstTimeTolbi, speaker);
  }
}

/// In Tolbi, the weapon merchant: across the counter (the leader facing
/// north) opens the shop; otherwise talks about Colosso.
static void StuccoRoomsB_TalkToWeaponMerchant(s32 speaker) {
  struct ObjectEvent *leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (FACING_NEAREST_CARDINAL(leader->facing) == FACING_NORTH) {
    Shop_Run(STUCCO_ROOMS_B_WEAPON_SHOP, speaker);
  } else if (GameFlag_Get(FLAG_COLOSSO_OVER) != 0) {
    StuccoRoomsB_AskYesNo(MsgTolbiComeWayKalay, speaker);
  } else if (GameFlag_Get(FLAG_BABI_SAVED) != 0) {
    FieldScript_SetMessageId(MsgTolbiColossoFinalsFinally);
    FieldScript_ShowMessageAndWait(speaker, 0);
  } else {
    FieldScript_SetMessageId(MsgTolbiLookStrongGo);
    FieldScript_ShowMessageAndWait(speaker, 0);
    FieldEffect_SpawnLinked(speaker, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 0);
    FieldScript_WaitFrames(40);
    FieldScript_ShowMessageAndWait(speaker, 0);
  }
}

/// In Tolbi, the armor merchant: across the counter (the leader facing north)
/// opens the shop; otherwise talks about the finals.
static void StuccoRoomsB_TalkToArmorMerchant(s32 speaker) {
  struct ObjectEvent *leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (FACING_NEAREST_CARDINAL(leader->facing) == FACING_NORTH) {
    Shop_Run(STUCCO_ROOMS_B_ARMOR_SHOP, speaker);
  } else {
    if (GameFlag_Get(FLAG_COLOSSO_OVER) != 0) {
      FieldScript_SetMessageId(MsgTolbiWasntAbleWatch);
      FieldScript_ShowMessageAndWait(speaker, 0);
    } else if (GameFlag_Get(FLAG_BABI_SAVED) != 0) {
      FieldScript_SetMessageId(MsgTolbiMaybeCloseShop);
      FieldScript_ShowMessageAndWait(speaker, 0);
    } else {
      FieldScript_SetMessageId(MsgTolbiIfCanMakeNameFor);
      FieldScript_ShowMessageAndWait(speaker, 0);
    }
  }
}

/// In Tolbi, the chef asks whether the party watched Colosso, which he missed.
static void StuccoRoomsB_TalkToChef(s32 speaker) {
  FieldScript_Start();
  StuccoRoomsB_AskYesNo(MsgTolbiLookLikeWarrior, speaker);
  FieldScript_End();
}

/// In Tolbi after Colosso, the waitress grumbles about the chef's bad mood.
static void StuccoRoomsB_TalkToWaitress(s32 speaker) {
  FieldScript_Start();
  FieldScript_SetMessageId(MsgTolbiGrrrChefInBadMood);
  FieldScript_ShowMessageAndWait(speaker, 0);
  FieldScript_End();
}

/// In Tolbi during the festival, a resident offers the party a bed.
static void StuccoRoomsB_TalkToHost(s32 speaker) {
  FieldScript_Start();
  StuccoRoomsB_AskYesNo(MsgTolbiWantStay, speaker);
  FieldScript_End();
}

/// In Tolbi, the woman who hid the Lucky Medal. Until the party takes it from
/// the barrel she teases; once it is gone she greets them, startled, the first
/// time, and then claims she is fine.
static void StuccoRoomsB_TalkToMedalWoman(s32 speaker) {
  FieldScript_Start();
  if (GameFlag_Get(FLAG_STUCCO_ROOMS_B_MEDAL_TAKEN) == 0) {
    StuccoRoomsB_AskYesNo(MsgTolbiHeeHeeLook, speaker);
  } else {
    if (Script_GetFlag(FLAG_STUCCO_ROOMS_B_MEDAL_WOMAN_SAID_HELLO) == 0) {
      GameFlag_Set(FLAG_STUCCO_ROOMS_B_MEDAL_WOMAN_SAID_HELLO);
      FieldScript_SetMessageId(MsgTolbiHello);
      Script_ShowMessage(speaker, 0);
      FieldScript_WaitFrames(10);
      ObjectMotion_SetVarCbAndRefresh(speaker, 2);
      FieldScript_WaitFrames(20);
    }
    FieldScript_SetMessageId(MsgTolbiHoHumFine);
    Script_ShowMessage(speaker, 0);
  }
  FieldScript_End();
}

/// In Tolbi, Mind Read on the medal woman gives the hiding place away.
static void StuccoRoomsB_ReadMedalWomanMind(void) {
  FieldScript_Start();
  if (GameFlag_Get(FLAG_STUCCO_ROOMS_B_MEDAL_WOMAN_SAID_HELLO) == 0) {
    FieldScript_SetMessageId(MsgTolbiHehHehSheJustHid);
  } else {
    FieldScript_SetMessageId(MsgTolbiThingFoundDefinitelySameAs);
  }
  FieldScript_ShowMessageAndWait(LOCALID_STUCCO_ROOMS_B_MEDAL_WOMAN, 0);
  FieldScript_End();
}

/// In Tolbi after Colosso, the angry passenger complains the ships won't sail.
/// "Grrr! What a scam!"
static void StuccoRoomsB_TalkToAngryPassenger(s32 speaker) {
  FieldScript_Start();
  FieldScript_SetMessageId(MsgTolbiGrrrScamWhyWontThey);
  ObjectEvent_ShowEmote(LOCALID_STUCCO_ROOMS_B_ANGRY_PASSENGER,
                        EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 40);
  Script_ShowMessage(speaker, 0);
  FieldScript_End();
}

/// The man from the ship asks whether the party remembers him. He's in Tolbi
/// after Colosso, and rode the ship from Kalay with the party. "Hey! You're the
/// would-be warrior!"
static void StuccoRoomsB_TalkToFellowPassenger(s32 speaker) {
  FieldScript_Start();
  StuccoRoomsB_AskYesNo(MsgTolbiWarriorRemember, speaker);
  FieldScript_End();
}

/// In Tolbi Sanctum, the healer: across the counter (the leader facing north)
/// opens the sanctum; otherwise talks about the festival.
static void StuccoRoomsB_TalkToHealer(s32 speaker) {
  struct ObjectEvent *leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (FACING_NEAREST_CARDINAL(leader->facing) == FACING_NORTH) {
    Sanctum_Run(speaker);
  } else {
    if (GameFlag_Get(FLAG_COLOSSO_OVER) != 0) {
      FieldScript_SetMessageId(MsgTolbiShipsArentGoing);
      FieldScript_ShowMessageAndWait(speaker, 0);
    } else if (GameFlag_Get(FLAG_BABI_SAVED) != 0) {
      FieldScript_SetMessageId(MsgTolbiMissFinalsTolbis);
      FieldScript_ShowMessageAndWait(speaker, 0);
    } else {
      FieldScript_SetMessageId(MsgTolbiWasteStuckHereWhenSuch);
      FieldScript_ShowMessageAndWait(speaker, 0);
    }
  }
}

/// An object event that squashes and stretches twice, then rests 40 frames.
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

struct MapWarpDest gStuccoRoomsBWarpDests[] = {
    {WARP_DEST_DEFAULT, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1,
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
    {MAP_TABLE_END},
};

u32 gStuccoRoomsBWarps[] = {
    MAP_WARPS(StuccoRoomsB),
    MAP_WARP(STUCCO_ROOMS_B_HOUSE_NORTHWEST_TO_TOLBI, Tolbi,
             TOLBI_FROM_HOUSE_NORTHWEST),
    MAP_WARP(STUCCO_ROOMS_B_INN_TO_TOLBI, Tolbi, TOLBI_FROM_INN),
    MAP_WARP(STUCCO_ROOMS_B_TO_TOLBI_EAST, Tolbi,
             TOLBI_FROM_STUCCO_ROOMS_B_EAST),
    MAP_WARP(STUCCO_ROOMS_B_HOUSE_NORTHEAST_TO_TOLBI, Tolbi,
             TOLBI_FROM_HOUSE_NORTHEAST),
    MAP_WARP(STUCCO_ROOMS_B_HOUSE_EAST_TO_TOLBI, Tolbi, TOLBI_FROM_HOUSE_EAST),
    MAP_WARP(STUCCO_ROOMS_B_WEAPON_AND_ARMOR_SHOP_TO_TOLBI, Tolbi,
             TOLBI_FROM_WEAPON_AND_ARMOR_SHOP),
    MAP_WARP(STUCCO_ROOMS_B_HOUSE_SOUTHWEST_TO_TOLBI, Tolbi,
             TOLBI_FROM_HOUSE_SOUTHWEST),
    MAP_WARP(STUCCO_ROOMS_B_SANCTUM_TO_TOLBI, Tolbi, TOLBI_FROM_SANCTUM),
    MAP_WARP(STUCCO_ROOMS_B_TO_TOLBI_WEST, Tolbi,
             TOLBI_FROM_STUCCO_ROOMS_B_WEST),
    MAP_WARP(STUCCO_ROOMS_B_INN_TO_INN_PASSAGE, StuccoRoomsB,
             STUCCO_ROOMS_B_INN_PASSAGE_FROM_INN),
    MAP_WARP(STUCCO_ROOMS_B_INN_PASSAGE_TO_INN, StuccoRoomsB,
             STUCCO_ROOMS_B_INN_FROM_INN_PASSAGE),
    MAP_WARP(STUCCO_ROOMS_B_INN_PASSAGE_TO_GUEST_ROOM_SOUTHWEST, StuccoRoomsB,
             STUCCO_ROOMS_B_GUEST_ROOM_SOUTHWEST_FROM_INN_PASSAGE),
    MAP_WARP(STUCCO_ROOMS_B_INN_PASSAGE_TO_GUEST_ROOM_SOUTHEAST, StuccoRoomsB,
             STUCCO_ROOMS_B_GUEST_ROOM_SOUTHEAST_FROM_INN_PASSAGE),
    MAP_WARP(STUCCO_ROOMS_B_INN_PASSAGE_TO_GUEST_ROOM_NORTHWEST, StuccoRoomsB,
             STUCCO_ROOMS_B_GUEST_ROOM_NORTHWEST_FROM_INN_PASSAGE),
    MAP_WARP(STUCCO_ROOMS_B_INN_PASSAGE_TO_GUEST_ROOM_NORTHEAST, StuccoRoomsB,
             STUCCO_ROOMS_B_GUEST_ROOM_NORTHEAST_FROM_INN_PASSAGE),
    MAP_WARP(STUCCO_ROOMS_B_GUEST_ROOM_SOUTHEAST_TO_INN_PASSAGE, StuccoRoomsB,
             STUCCO_ROOMS_B_INN_PASSAGE_FROM_GUEST_ROOM_SOUTHEAST),
    MAP_WARP(STUCCO_ROOMS_B_GUEST_ROOM_SOUTHWEST_TO_INN_PASSAGE, StuccoRoomsB,
             STUCCO_ROOMS_B_INN_PASSAGE_FROM_GUEST_ROOM_SOUTHWEST),
    MAP_WARP(STUCCO_ROOMS_B_GUEST_ROOM_NORTHWEST_TO_INN_PASSAGE, StuccoRoomsB,
             STUCCO_ROOMS_B_INN_PASSAGE_FROM_GUEST_ROOM_NORTHWEST),
    MAP_WARP(STUCCO_ROOMS_B_GUEST_ROOM_NORTHEAST_TO_INN_PASSAGE, StuccoRoomsB,
             STUCCO_ROOMS_B_INN_PASSAGE_FROM_GUEST_ROOM_NORTHEAST),
    MAP_WARPS_END,
};

// clang-format off
struct ObjectEventTemplate gStuccoRoomsBObjectEventTemplates[] = {
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
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gStuccoRoomsBObjectEventTemplatesAfterColosso[] = {
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
    { MAP_TABLE_END },
};

struct MapEvent gStuccoRoomsBEventsFestival[] = {
    { MAP_EVENT_WARP, 1, CONDITION_ALWAYS, STUCCO_ROOMS_B_HOUSE_NORTHWEST_TO_TOLBI },
    { MAP_EVENT_WARP, 2, CONDITION_ALWAYS, STUCCO_ROOMS_B_INN_TO_TOLBI },
    { MAP_EVENT_WARP, 3, CONDITION_ALWAYS, STUCCO_ROOMS_B_TO_TOLBI_EAST },
    { MAP_EVENT_WARP, 4, CONDITION_ALWAYS, STUCCO_ROOMS_B_HOUSE_NORTHEAST_TO_TOLBI },
    { MAP_EVENT_WARP, 5, CONDITION_ALWAYS, STUCCO_ROOMS_B_HOUSE_EAST_TO_TOLBI },
    { MAP_EVENT_WARP, 6, CONDITION_ALWAYS, STUCCO_ROOMS_B_WEAPON_AND_ARMOR_SHOP_TO_TOLBI },
    { MAP_EVENT_WARP, 7, CONDITION_ALWAYS, STUCCO_ROOMS_B_HOUSE_SOUTHWEST_TO_TOLBI },
    { MAP_EVENT_WARP, 8, CONDITION_ALWAYS, STUCCO_ROOMS_B_SANCTUM_TO_TOLBI },
    { MAP_EVENT_WARP, 9, CONDITION_ALWAYS, STUCCO_ROOMS_B_TO_TOLBI_WEST },
    { MAP_EVENT_WARP, 10, CONDITION_ALWAYS, STUCCO_ROOMS_B_INN_TO_INN_PASSAGE },
    { MAP_EVENT_WARP, 11, CONDITION_ALWAYS, STUCCO_ROOMS_B_INN_PASSAGE_TO_INN },
    { MAP_EVENT_WARP, 12, CONDITION_ALWAYS, STUCCO_ROOMS_B_INN_PASSAGE_TO_GUEST_ROOM_SOUTHWEST },
    { MAP_EVENT_WARP, 13, CONDITION_ALWAYS, STUCCO_ROOMS_B_INN_PASSAGE_TO_GUEST_ROOM_SOUTHEAST },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 14, CONDITION_ALWAYS, EVENT_SCRIPT(StuccoRoomsB_EnterDoor) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 15, CONDITION_ALWAYS, EVENT_SCRIPT(StuccoRoomsB_EnterDoor) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 16, CONDITION_ALWAYS, EVENT_SCRIPT(StuccoRoomsB_EnterDoor) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 17, CONDITION_ALWAYS, EVENT_SCRIPT(StuccoRoomsB_EnterDoor) },
    { MAP_EVENT_WARP, 18, CONDITION_ALWAYS, STUCCO_ROOMS_B_GUEST_ROOM_NORTHWEST_TO_INN_PASSAGE },
    { MAP_EVENT_WARP, 19, CONDITION_ALWAYS, STUCCO_ROOMS_B_GUEST_ROOM_NORTHEAST_TO_INN_PASSAGE },
    { MAP_EVENT_TALK, LOCALID_STUCCO_ROOMS_B_MAKESHIFT_INNKEEPER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsTolbiIsPackedWithTouristsDuring) },
    { MAP_EVENT_TALK, LOCALID_STUCCO_ROOMS_B_HOST, CONDITION_ALWAYS, EVENT_SCRIPT(StuccoRoomsB_TalkToHost) },
    { MAP_EVENT_TALK, LOCALID_STUCCO_ROOMS_B_SLEEPLESS_GUEST, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsICouldntSleepAtAllLast) },
    { MAP_EVENT_TALK, LOCALID_STUCCO_ROOMS_B_CROWD_SICK_GUEST, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsTheCrowdsAtTheColosseumMade) },
    { MAP_EVENT_TALK, LOCALID_STUCCO_ROOMS_B_HOUSE_TENANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsWePaidGoodMoneyToRent) },
    { MAP_EVENT_TALK, LOCALID_STUCCO_ROOMS_B_FINALS_WATCHER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIReallyWantToSeeColosso) },
    { MAP_EVENT_TALK, LOCALID_STUCCO_ROOMS_B_CRYING_BOY, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsWahhhIWantToSeeColosso) },
    { MAP_EVENT_TALK, LOCALID_STUCCO_ROOMS_B_SOLDIER_WATCHER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIWonderIfTheyReallyCant) },
    { MAP_EVENT_TALK, LOCALID_STUCCO_ROOMS_B_BABI_SKEPTIC, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsBabiWasntAtColossoAccordingTo) },
    { MAP_EVENT_TALK, LOCALID_STUCCO_ROOMS_B_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsWereGoingToMakeSomeMoney) },
    { MAP_EVENT_TALK, LOCALID_STUCCO_ROOMS_B_TRAVELING_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIHeardBabiIsUsingThe) },
    { MAP_EVENT_TALK, LOCALID_STUCCO_ROOMS_B_WEAPON_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(StuccoRoomsB_TalkToWeaponMerchant) },
    { MAP_EVENT_TALK, LOCALID_STUCCO_ROOMS_B_ITEM_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(StuccoRoomsB_TalkToItemMerchant) },
    { MAP_EVENT_TALK, LOCALID_STUCCO_ROOMS_B_ARMOR_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(StuccoRoomsB_TalkToArmorMerchant) },
    { MAP_EVENT_TALK, LOCALID_STUCCO_ROOMS_B_INN_MASTER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsWereFullyBookedWithReservationsDuring) },
    { MAP_EVENT_TALK, LOCALID_STUCCO_ROOMS_B_INN_LADY, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsColossoIsTheBusiestTimeOf) },
    { MAP_EVENT_TALK, LOCALID_STUCCO_ROOMS_B_INN_WORKER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsWereSoBusyRunningTheInn) },
    { MAP_EVENT_TALK, LOCALID_STUCCO_ROOMS_B_MEDAL_WOMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsATourGroupCameFromKalay) },
    { MAP_EVENT_TALK, LOCALID_STUCCO_ROOMS_B_CHEF, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsWeArentGoingToGetTo) },
    { MAP_EVENT_TALK, LOCALID_STUCCO_ROOMS_B_WAITRESS, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsImNotVeryInterestedInColosso) },
    { MAP_EVENT_TALK, LOCALID_STUCCO_ROOMS_B_LATE_ENTRANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsOhNoWeBarelyMadeIt) },
    { MAP_EVENT_TALK, LOCALID_STUCCO_ROOMS_B_SECOND_LATE_ENTRANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIfWedGottenHereJustA) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_STUCCO_ROOMS_B_MAKESHIFT_INNKEEPER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsOnceColossoStartsWereGoingTo) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_STUCCO_ROOMS_B_HOST, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsItsTooLateToStartLooking) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_STUCCO_ROOMS_B_SLEEPLESS_GUEST, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIfICouldHaveSleptLast) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_STUCCO_ROOMS_B_CROWD_SICK_GUEST, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsHuhUhCrowdsAreADefinite) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_STUCCO_ROOMS_B_HOUSE_TENANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsWeGetToStayHereUntil) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_STUCCO_ROOMS_B_FINALS_WATCHER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIWasStubbornISaidIm) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_STUCCO_ROOMS_B_CRYING_BOY, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIKeepOnCryingButDaddy) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_STUCCO_ROOMS_B_SOLDIER_WATCHER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsItsStrangeThatBabisSoldiersAre) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_STUCCO_ROOMS_B_BABI_SKEPTIC, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsItsStrangeBabiLovesColossoBut) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_STUCCO_ROOMS_B_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsHeJustTakesACutOf) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_STUCCO_ROOMS_B_TRAVELING_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsNamingTheLighthouseHeBuiltAfter) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_STUCCO_ROOMS_B_WEAPON_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIGuessINeverWouldHave) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_STUCCO_ROOMS_B_ITEM_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsTheyHaveSomeNiceThingsFor) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_STUCCO_ROOMS_B_ARMOR_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsBabiChoosesColossosVictorsToBe) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_STUCCO_ROOMS_B_INN_MASTER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsTheTourGroupIsFeelingCramped) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_STUCCO_ROOMS_B_INN_LADY, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsWeCantHandleTooManyMore) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_STUCCO_ROOMS_B_INN_WORKER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsImGoingToGoToThe) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_STUCCO_ROOMS_B_MEDAL_WOMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsThereArentEnoughBedsForThe) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_STUCCO_ROOMS_B_CHEF, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsTheInnkeeperToldMeToCook) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_STUCCO_ROOMS_B_WAITRESS, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIHaveAHeadacheJustThinking) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_STUCCO_ROOMS_B_LATE_ENTRANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsEvenThoughICantEnterColosso) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_STUCCO_ROOMS_B_SECOND_LATE_ENTRANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIfWeCouldHaveEnteredColosso) },
    { MAP_EVENT_TALK, LOCALID_STUCCO_ROOMS_B_HEALER, CONDITION_ALWAYS, EVENT_SCRIPT(StuccoRoomsB_TalkToHealer) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_STUCCO_ROOMS_B_HEALER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIWonderHowTheTryoutsAre) },
    { BG_EVENT(BG_EVENT_BARREL), MB_STUCCO_ROOMS_B_VIAL_BARREL, FLAG_STUCCO_ROOMS_B_BARREL_VIAL, FIND_ITEM(ITEM_VIAL) },
    { BG_EVENT(BG_EVENT_BARREL), MB_STUCCO_ROOMS_B_MEDAL_BARREL, FLAG_STUCCO_ROOMS_B_BARREL_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { BG_EVENT(BG_EVENT_OVEN), MB_STUCCO_ROOMS_B_PIZZA_OVEN, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgTolbiInteriorsShesMakingTheWorldRenownedTolbi)) },
    { BG_EVENT(BG_EVENT_OVEN), MB_STUCCO_ROOMS_B_PASTA_OVEN, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgTolbiInteriorsHeIsBoilingThePastaIt)) },
    { BG_EVENT(BG_EVENT_OVEN), MB_STUCCO_ROOMS_B_SEAFOOD_PASTA_OVEN, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgTolbiInteriorsItsTolbiSeafoodPastaWithLots)) },
    { BG_EVENT(BG_EVENT_SILENT), MB_STUCCO_ROOMS_B_INN_SIGN, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgTolbiInteriorsIsaacCheckedTheSignInnFull)) },
    { MAP_EVENTS_END },
};

struct MapEvent gStuccoRoomsBEventsFinalsDay[] = {
    { MAP_EVENT_WARP, 1, CONDITION_ALWAYS, STUCCO_ROOMS_B_HOUSE_NORTHWEST_TO_TOLBI },
    { MAP_EVENT_WARP, 2, CONDITION_ALWAYS, STUCCO_ROOMS_B_INN_TO_TOLBI },
    { MAP_EVENT_WARP, 3, CONDITION_ALWAYS, STUCCO_ROOMS_B_TO_TOLBI_EAST },
    { MAP_EVENT_WARP, 4, CONDITION_ALWAYS, STUCCO_ROOMS_B_HOUSE_NORTHEAST_TO_TOLBI },
    { MAP_EVENT_WARP, 5, CONDITION_ALWAYS, STUCCO_ROOMS_B_HOUSE_EAST_TO_TOLBI },
    { MAP_EVENT_WARP, 6, CONDITION_ALWAYS, STUCCO_ROOMS_B_WEAPON_AND_ARMOR_SHOP_TO_TOLBI },
    { MAP_EVENT_WARP, 7, CONDITION_ALWAYS, STUCCO_ROOMS_B_HOUSE_SOUTHWEST_TO_TOLBI },
    { MAP_EVENT_WARP, 8, CONDITION_ALWAYS, STUCCO_ROOMS_B_SANCTUM_TO_TOLBI },
    { MAP_EVENT_WARP, 9, CONDITION_ALWAYS, STUCCO_ROOMS_B_TO_TOLBI_WEST },
    { MAP_EVENT_WARP, 10, CONDITION_ALWAYS, STUCCO_ROOMS_B_INN_TO_INN_PASSAGE },
    { MAP_EVENT_WARP, 11, CONDITION_ALWAYS, STUCCO_ROOMS_B_INN_PASSAGE_TO_INN },
    { MAP_EVENT_WARP, 12, CONDITION_ALWAYS, STUCCO_ROOMS_B_INN_PASSAGE_TO_GUEST_ROOM_SOUTHWEST },
    { MAP_EVENT_WARP, 13, CONDITION_ALWAYS, STUCCO_ROOMS_B_INN_PASSAGE_TO_GUEST_ROOM_SOUTHEAST },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 14, CONDITION_ALWAYS, EVENT_SCRIPT(StuccoRoomsB_EnterDoor) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 15, CONDITION_ALWAYS, EVENT_SCRIPT(StuccoRoomsB_EnterDoor) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 16, CONDITION_ALWAYS, EVENT_SCRIPT(StuccoRoomsB_EnterDoor) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 17, CONDITION_ALWAYS, EVENT_SCRIPT(StuccoRoomsB_EnterDoor) },
    { MAP_EVENT_WARP, 18, CONDITION_ALWAYS, STUCCO_ROOMS_B_GUEST_ROOM_NORTHWEST_TO_INN_PASSAGE },
    { MAP_EVENT_WARP, 19, CONDITION_ALWAYS, STUCCO_ROOMS_B_GUEST_ROOM_NORTHEAST_TO_INN_PASSAGE },
    { MAP_EVENT_TALK, LOCALID_STUCCO_ROOMS_B_MAKESHIFT_INNKEEPER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsSoColossoHasAlreadyReachedThe) },
    { MAP_EVENT_TALK, LOCALID_STUCCO_ROOMS_B_HOST, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsTheFestivalScheduleWasDelayedFor) },
    { MAP_EVENT_TALK, LOCALID_STUCCO_ROOMS_B_SLEEPLESS_GUEST, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsThatsStrangeIWonderWhatHappened) },
    { MAP_EVENT_TALK, LOCALID_STUCCO_ROOMS_B_CROWD_SICK_GUEST, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIJustSawBabiTheTowns) },
    { MAP_EVENT_TALK, LOCALID_STUCCO_ROOMS_B_HOUSE_TENANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsHereIAmInTolbiFor) },
    { MAP_EVENT_TALK, LOCALID_STUCCO_ROOMS_B_FINALS_WATCHER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsImGoingToWatchTheFinals) },
    { MAP_EVENT_TALK, LOCALID_STUCCO_ROOMS_B_CRYING_BOY, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsHoorayWereGoingToColossoNow) },
#if TBS_PATCH_4
    { MAP_EVENT_TALK, LOCALID_STUCCO_ROOMS_B_SOLDIER_WATCHER, CONDITION_ALWAYS, EVENT_SCRIPT(StuccoRoomsB_AskWhetherBabiWasMissing) },
    { MAP_EVENT_TALK, LOCALID_STUCCO_ROOMS_B_BABI_SKEPTIC, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsBabiDecidedShowUpForFinals) },
#else
    { MAP_EVENT_TALK, LOCALID_STUCCO_ROOMS_B_SOLDIER_WATCHER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsWellIGuessSoSinceHes) },
    { MAP_EVENT_TALK, LOCALID_STUCCO_ROOMS_B_BABI_SKEPTIC, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsHuhThenTheyFoundHimIn) },
#endif
    { MAP_EVENT_TALK, LOCALID_STUCCO_ROOMS_B_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsBabiApparentlyHasSomeReasonFor) },
    { MAP_EVENT_TALK, LOCALID_STUCCO_ROOMS_B_TRAVELING_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIHeardBabiIsSearchingFor) },
    { MAP_EVENT_TALK, LOCALID_STUCCO_ROOMS_B_WEAPON_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(StuccoRoomsB_TalkToWeaponMerchant) },
    { MAP_EVENT_TALK, LOCALID_STUCCO_ROOMS_B_ITEM_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(StuccoRoomsB_TalkToItemMerchant) },
    { MAP_EVENT_TALK, LOCALID_STUCCO_ROOMS_B_ARMOR_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(StuccoRoomsB_TalkToArmorMerchant) },
    { MAP_EVENT_TALK, LOCALID_STUCCO_ROOMS_B_INN_MASTER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsOnceTheFinalsBeginEveryoneTries) },
    { MAP_EVENT_TALK, LOCALID_STUCCO_ROOMS_B_INN_LADY, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsThisYearImNotLettingAnyone) },
    { MAP_EVENT_TALK, LOCALID_STUCCO_ROOMS_B_INN_WORKER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsTheWifeHasACloseEye) },
    { MAP_EVENT_TALK, LOCALID_STUCCO_ROOMS_B_MEDAL_WOMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsHmmIThoughtAllMenLoved) },
    { MAP_EVENT_TALK, LOCALID_STUCCO_ROOMS_B_CHEF, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsTheMomentIHearTheFinals) },
    { MAP_EVENT_TALK, LOCALID_STUCCO_ROOMS_B_WAITRESS, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsSoColossoIsEndingSoonIts) },
    { MAP_EVENT_TALK, LOCALID_STUCCO_ROOMS_B_LATE_ENTRANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsTheColossoFinalsAreStartingSoon) },
    { MAP_EVENT_TALK, LOCALID_STUCCO_ROOMS_B_SECOND_LATE_ENTRANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsMaybeIllDoSomeResearchOn) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_STUCCO_ROOMS_B_MAKESHIFT_INNKEEPER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsAreWeMakingMoreMoneyNow) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_STUCCO_ROOMS_B_HOST, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsSoTheyDidExtendTheFestival) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_STUCCO_ROOMS_B_SLEEPLESS_GUEST, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsICantAffordToStayHere) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_STUCCO_ROOMS_B_CROWD_SICK_GUEST, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsTheAirInTheColosseumIs) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_STUCCO_ROOMS_B_HOUSE_TENANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIfSomebodyElseWouldWatchThe) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_STUCCO_ROOMS_B_FINALS_WATCHER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsColossoIsAlreadyAtTheFinals) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_STUCCO_ROOMS_B_CRYING_BOY, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsSinceWeDidntGetToSee) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_STUCCO_ROOMS_B_SOLDIER_WATCHER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsSoWhatWereBabisSoldiersDoing) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_STUCCO_ROOMS_B_BABI_SKEPTIC, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsMaybeWatchingTheTrialsIsToo) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_STUCCO_ROOMS_B_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsBeforeHeStartedTheLighthouseBabi) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_STUCCO_ROOMS_B_TRAVELING_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsBabiHasBeenMakingStrangeReferences) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_STUCCO_ROOMS_B_WEAPON_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsTheTouristsAllBuyWeaponsAnd) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_STUCCO_ROOMS_B_ITEM_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsYouCouldSayTheWarriorsIn) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_STUCCO_ROOMS_B_ARMOR_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIfMomFindsOutIWent) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_STUCCO_ROOMS_B_INN_MASTER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsImTheOneWhoReallyWants) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_STUCCO_ROOMS_B_INN_LADY, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsOnceThingsSettleDownImSure) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_STUCCO_ROOMS_B_INN_WORKER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIfTheMasterSlipsOutThe) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_STUCCO_ROOMS_B_MEDAL_WOMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsWeWomenAreTheOnlyOnes) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_STUCCO_ROOMS_B_CHEF, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsHasntTheMasterGoneToColosso) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_STUCCO_ROOMS_B_WAITRESS, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIDontHaveAnyParticularInterest) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_STUCCO_ROOMS_B_LATE_ENTRANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIWantToSeeTheFinals) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_STUCCO_ROOMS_B_SECOND_LATE_ENTRANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsTheTrialBattlesWerentQuiteLike) },
    { MAP_EVENT_TALK, LOCALID_STUCCO_ROOMS_B_HEALER, CONDITION_ALWAYS, EVENT_SCRIPT(StuccoRoomsB_TalkToHealer) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_STUCCO_ROOMS_B_HEALER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIfYoureNotInterestedInThe) },
    { BG_EVENT(BG_EVENT_BARREL), MB_STUCCO_ROOMS_B_VIAL_BARREL, FLAG_STUCCO_ROOMS_B_BARREL_VIAL, FIND_ITEM(ITEM_VIAL) },
    { BG_EVENT(BG_EVENT_BARREL), MB_STUCCO_ROOMS_B_MEDAL_BARREL, FLAG_STUCCO_ROOMS_B_BARREL_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { BG_EVENT(BG_EVENT_OVEN), MB_STUCCO_ROOMS_B_PIZZA_OVEN, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgTolbiInteriorsShesMakingTheWorldRenownedTolbi)) },
    { BG_EVENT(BG_EVENT_OVEN), MB_STUCCO_ROOMS_B_PASTA_OVEN, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgTolbiInteriorsHeIsBoilingThePastaIt)) },
    { BG_EVENT(BG_EVENT_OVEN), MB_STUCCO_ROOMS_B_SEAFOOD_PASTA_OVEN, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgTolbiInteriorsItsTolbiSeafoodPastaWithLots)) },
    { BG_EVENT(BG_EVENT_SILENT), MB_STUCCO_ROOMS_B_INN_SIGN, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgTolbiInteriorsIsaacCheckedTheSignInnFull)) },
    { MAP_EVENTS_END },
};

struct MapEvent gStuccoRoomsBEventsAfterColosso[] = {
    { MAP_EVENT_WARP, 1, CONDITION_ALWAYS, STUCCO_ROOMS_B_HOUSE_NORTHWEST_TO_TOLBI },
    { MAP_EVENT_WARP, 2, CONDITION_ALWAYS, STUCCO_ROOMS_B_INN_TO_TOLBI },
    { MAP_EVENT_WARP, 3, CONDITION_ALWAYS, STUCCO_ROOMS_B_TO_TOLBI_EAST },
    { MAP_EVENT_WARP, 4, CONDITION_ALWAYS, STUCCO_ROOMS_B_HOUSE_NORTHEAST_TO_TOLBI },
    { MAP_EVENT_WARP, 5, CONDITION_ALWAYS, STUCCO_ROOMS_B_HOUSE_EAST_TO_TOLBI },
    { MAP_EVENT_WARP, 6, CONDITION_ALWAYS, STUCCO_ROOMS_B_WEAPON_AND_ARMOR_SHOP_TO_TOLBI },
    { MAP_EVENT_WARP, 7, CONDITION_ALWAYS, STUCCO_ROOMS_B_HOUSE_SOUTHWEST_TO_TOLBI },
    { MAP_EVENT_WARP, 8, CONDITION_ALWAYS, STUCCO_ROOMS_B_SANCTUM_TO_TOLBI },
    { MAP_EVENT_WARP, 9, CONDITION_ALWAYS, STUCCO_ROOMS_B_TO_TOLBI_WEST },
    { MAP_EVENT_WARP, 10, CONDITION_ALWAYS, STUCCO_ROOMS_B_INN_TO_INN_PASSAGE },
    { MAP_EVENT_WARP, 11, CONDITION_ALWAYS, STUCCO_ROOMS_B_INN_PASSAGE_TO_INN },
    { MAP_EVENT_WARP, 12, CONDITION_ALWAYS, STUCCO_ROOMS_B_INN_PASSAGE_TO_GUEST_ROOM_SOUTHWEST },
    { MAP_EVENT_WARP, 13, CONDITION_ALWAYS, STUCCO_ROOMS_B_INN_PASSAGE_TO_GUEST_ROOM_SOUTHEAST },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 14, CONDITION_ALWAYS, EVENT_SCRIPT(StuccoRoomsB_EnterDoor) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 15, CONDITION_ALWAYS, EVENT_SCRIPT(StuccoRoomsB_EnterDoor) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 16, CONDITION_ALWAYS, EVENT_SCRIPT(StuccoRoomsB_EnterDoor) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 17, CONDITION_ALWAYS, EVENT_SCRIPT(StuccoRoomsB_EnterDoor) },
    { MAP_EVENT_WARP, 18, CONDITION_ALWAYS, STUCCO_ROOMS_B_GUEST_ROOM_NORTHWEST_TO_INN_PASSAGE },
    { MAP_EVENT_WARP, 19, CONDITION_ALWAYS, STUCCO_ROOMS_B_GUEST_ROOM_NORTHEAST_TO_INN_PASSAGE },
    { MAP_EVENT_TALK, LOCALID_STUCCO_ROOMS_B_MAKESHIFT_INNKEEPER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsNowThatColossoIsOverWeve) },
    { MAP_EVENT_TALK, LOCALID_STUCCO_ROOMS_B_HOST, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsWeHadALotOfGuests) },
    { MAP_EVENT_TALK, LOCALID_STUCCO_ROOMS_B_STRANDED_TRAVELER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIRentedMyHouseToA) },
    { MAP_EVENT_TALK, LOCALID_STUCCO_ROOMS_B_CAVE_RUMOR_GUEST, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsTheTouristsAreAllInA) },
    { MAP_EVENT_TALK, LOCALID_STUCCO_ROOMS_B_FINALS_WATCHER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIWasSoMovedByThis) },
    { MAP_EVENT_TALK, LOCALID_STUCCO_ROOMS_B_CRYING_BOY, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsThatLittleGuyJumpedStraightInto) },
    { MAP_EVENT_TALK, LOCALID_STUCCO_ROOMS_B_SOLDIER_WATCHER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsDidYouKnowMtAlephErupted) },
    { MAP_EVENT_TALK, LOCALID_STUCCO_ROOMS_B_BABI_SKEPTIC, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsMaybeAlephsEruptionCausedThoseSea) },
    { MAP_EVENT_TALK, LOCALID_STUCCO_ROOMS_B_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsTheWarriorsSaidTheyWouldRush) },
    { MAP_EVENT_TALK, LOCALID_STUCCO_ROOMS_B_TRAVELING_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsThereWontBeManyCustomersUntil) },
    { MAP_EVENT_TALK, LOCALID_STUCCO_ROOMS_B_WEAPON_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(StuccoRoomsB_TalkToWeaponMerchant) },
    { MAP_EVENT_TALK, LOCALID_STUCCO_ROOMS_B_ITEM_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(StuccoRoomsB_TalkToItemMerchant) },
    { MAP_EVENT_TALK, LOCALID_STUCCO_ROOMS_B_ARMOR_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(StuccoRoomsB_TalkToArmorMerchant) },
    { MAP_EVENT_TALK, LOCALID_STUCCO_ROOMS_B_INN_MASTER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIThoughtWedHaveSomeFree) },
    { MAP_EVENT_TALK, LOCALID_STUCCO_ROOMS_B_INN_LADY, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsTheSpectatorsFromTheEastCant) },
    { MAP_EVENT_TALK, LOCALID_STUCCO_ROOMS_B_INN_WORKER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsItsBusyAllDayEveryDay) },
    { MAP_EVENT_TALK, LOCALID_STUCCO_ROOMS_B_MEDAL_WOMAN, CONDITION_ALWAYS, EVENT_SCRIPT(StuccoRoomsB_TalkToMedalWoman) },
    { MAP_EVENT_TALK, LOCALID_STUCCO_ROOMS_B_CHEF, CONDITION_ALWAYS, EVENT_SCRIPT(StuccoRoomsB_TalkToChef) },
    { MAP_EVENT_TALK, LOCALID_STUCCO_ROOMS_B_WAITRESS, CONDITION_ALWAYS, EVENT_SCRIPT(StuccoRoomsB_TalkToWaitress) },
    { MAP_EVENT_TALK, LOCALID_STUCCO_ROOMS_B_LATE_ENTRANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIWentToThePortBut) },
    { MAP_EVENT_TALK, LOCALID_STUCCO_ROOMS_B_SECOND_LATE_ENTRANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsColossoHasEndedButWeStill) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_STUCCO_ROOMS_B_MAKESHIFT_INNKEEPER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsSighMakingTheBedsHasWorn) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_STUCCO_ROOMS_B_HOST, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIdLoveAVacationButWhere) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_STUCCO_ROOMS_B_STRANDED_TRAVELER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsOnceTheShipsStartSailingAgain) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_STUCCO_ROOMS_B_CAVE_RUMOR_GUEST, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIHeardOfACaveThat) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_STUCCO_ROOMS_B_FINALS_WATCHER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsTheWarriorsWhoFoughtInThe) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_STUCCO_ROOMS_B_CRYING_BOY, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsDaringAndBraveryAreNiceAnd) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_STUCCO_ROOMS_B_SOLDIER_WATCHER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsTheSparklingStonesThatRainedFrom) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_STUCCO_ROOMS_B_BABI_SKEPTIC, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIllBetTheMonstersWereFrightened) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_STUCCO_ROOMS_B_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsItBothersMeABitThat) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_STUCCO_ROOMS_B_TRAVELING_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsThereArentManyMerchantsAtThe) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_STUCCO_ROOMS_B_WEAPON_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsEvenWithoutTheShipsICould) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_STUCCO_ROOMS_B_ITEM_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsDidAllTheWarriorsWhoFought) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_STUCCO_ROOMS_B_ARMOR_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsImGonnaWatchTheFinalsNext) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_STUCCO_ROOMS_B_INN_MASTER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsSinceTheShipsArentSailingThe) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_STUCCO_ROOMS_B_INN_LADY, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsHeeHeeItsGreatTheInns) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_STUCCO_ROOMS_B_INN_WORKER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsEveryoneIsWorkingOnBabiLighthouse) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_STUCCO_ROOMS_B_MEDAL_WOMAN, CONDITION_ALWAYS, EVENT_SCRIPT(StuccoRoomsB_ReadMedalWomanMind) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_STUCCO_ROOMS_B_CHEF, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsGrrrIDidntGetToSee) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_STUCCO_ROOMS_B_WAITRESS, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsHeyChefSoWhatIfYou) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_STUCCO_ROOMS_B_LATE_ENTRANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsITriedGoingToTheEastern) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_STUCCO_ROOMS_B_SECOND_LATE_ENTRANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsYoudThinkYouCouldGetThrough) },
    { MAP_EVENT_TALK, LOCALID_STUCCO_ROOMS_B_HEALER, CONDITION_ALWAYS, EVENT_SCRIPT(StuccoRoomsB_TalkToHealer) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_STUCCO_ROOMS_B_HEALER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsYouAllGotToSeeColosso) },
    { MAP_EVENT_TALK, LOCALID_STUCCO_ROOMS_B_ANGRY_PASSENGER, CONDITION_ALWAYS, EVENT_SCRIPT(StuccoRoomsB_TalkToAngryPassenger) },
    { MAP_EVENT_TALK, LOCALID_STUCCO_ROOMS_B_PASSENGER_WIFE, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsOnceThisGuyGetsMadTheres) },
    { MAP_EVENT_TALK, LOCALID_STUCCO_ROOMS_B_THIRD_PASSENGER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIWonderIfThisReallyIs) },
    { MAP_EVENT_TALK, LOCALID_STUCCO_ROOMS_B_BORED_BOY, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsTolbiGetsSoBoringOnceThe) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_STUCCO_ROOMS_B_ANGRY_PASSENGER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsYoureSayingIfWeRowWe) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_STUCCO_ROOMS_B_PASSENGER_WIFE, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsTheCrewIsSayingThatIf) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_STUCCO_ROOMS_B_THIRD_PASSENGER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsImSickOfStayingAtThis) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_STUCCO_ROOMS_B_BORED_BOY, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIfMomHadJustBoughtMe) },
    { MAP_EVENT_TALK, LOCALID_STUCCO_ROOMS_B_FELLOW_PASSENGER, CONDITION_ALWAYS, EVENT_SCRIPT(StuccoRoomsB_TalkToFellowPassenger) },
    { MAP_EVENT_TALK, LOCALID_STUCCO_ROOMS_B_COLOSSO_FAN, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIGotToSeeColossoSo) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_STUCCO_ROOMS_B_FELLOW_PASSENGER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsThatVoyageAcrossTheSeaWas) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_STUCCO_ROOMS_B_COLOSSO_FAN, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsNoMatterWhatHappensImComing) },
    { BG_EVENT(BG_EVENT_BARREL), MB_STUCCO_ROOMS_B_VIAL_BARREL, FLAG_STUCCO_ROOMS_B_BARREL_VIAL, FIND_ITEM(ITEM_VIAL) },
    { BG_EVENT(BG_EVENT_BARREL), MB_STUCCO_ROOMS_B_MEDAL_BARREL, FLAG_STUCCO_ROOMS_B_BARREL_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { BG_EVENT(BG_EVENT_OVEN), MB_STUCCO_ROOMS_B_PIZZA_OVEN, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgTolbiInteriorsShesMakingTheWorldRenownedTolbi)) },
    { BG_EVENT(BG_EVENT_OVEN), MB_STUCCO_ROOMS_B_PASTA_OVEN, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgTolbiInteriorsHeIsBoilingThePastaIt)) },
    { BG_EVENT(BG_EVENT_OVEN), MB_STUCCO_ROOMS_B_SEAFOOD_PASTA_OVEN, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgTolbiInteriorsItsTolbiSeafoodPastaWithLots)) },
    { BG_EVENT(BG_EVENT_SILENT), MB_STUCCO_ROOMS_B_INN_SIGN, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgTolbiInteriorsIsaacCheckedTheSignInnFull)) },
    { BG_EVENT(BG_EVENT_BARREL), MB_STUCCO_ROOMS_B_HIDDEN_MEDAL_BARREL, FLAG_STUCCO_ROOMS_B_MEDAL_TAKEN, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { MAP_EVENTS_END },
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
