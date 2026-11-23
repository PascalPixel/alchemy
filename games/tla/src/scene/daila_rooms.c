/// @file
/// Daila's rooms, in two scenes that share this overlay: the houses
/// (DailaRoomsHouses) and the sanctuary (DailaRoomsSanctum).
///
/// The houses are six rooms, one behind each door of the village: Riki's
/// home, Tavi's home, the middle house, the mayor's house, the inn and the
/// weapon and armor shop. The same scene also holds the house on the islet
/// with the cow, reached from North Osenia. Once Tavi is rescued
/// (FLAG_TAVI_RESCUED) the people have other things to say, the two boys are
/// home eating, and a woman at the inn is caught practicing for the dinner
/// show.
///
/// The sanctuary, up the steps at the north of the village, has a healer at
/// his counter, and behind him the Sea God's statue with the Sea God's Tear at
/// its foot, beyond a pool of water. Once Tavi is rescued the water has drained
/// to three puddles (Scene_PrepareMap). Felix can hop over them, and Frost
/// raises each into a pillar of ice; with the two on the left frozen he can
/// reach the Tear and take it (DailaRooms_TakeSeaGodsTear).

#include "shared/tables.h"
#include "field/emote.h"
#include "field/event.h"
#include "sounds.h"
#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "flags.h"
#include "item_ids.h"
#include "field/scene_tables.h"
#include "scene_ids.h"
#include "field/field_event.h"
#include "system/main_routine_work.h"
#include "field/scene_call.h"
#include "system/ram_buffer.h"
#include "field/actor.h"
#include "field/actor_script.h"
#include "field/field_sequence_event.h"
#include "field/field_sequence.h"
#include "game/inventory.h"
#include "scene/scene.h"
#include "scene/ground_effects.h"
#include "window/window.h"
#include "game/item.h"
#include "shop/shop.h"
#include "version.h"
#include "text/msg_ids.h"

/// The cells beside each of the sanctuary's puddles once it has drained, named
/// by the side they lie on: walking from one into the puddle hops over it.
enum DailaRoomsCell {
  CELL_NORTH_OF_PUDDLE = 30,
  CELL_WEST_OF_PUDDLE,
  CELL_EAST_OF_PUDDLE,
  CELL_SOUTH_OF_PUDDLE,
  /// The foot of the statue where the Sea God's Tear lies, cell (14, 12).
  CELL_SEA_GODS_TEAR = 40
};

/// The houses, shops and inn of DailaRoomsHouses, numbering the rows its
/// two placement tables share.
enum DailaRoomsActorId {
  /// Riki's parents, in his home at the west of the village, scolding the boy
  /// who took his father's fishing pole.
  ACTOR_RIKIS_FATHER = ACTOR_FIRST_PLACED,
  ACTOR_RIKIS_MOTHER,
  /// Tavi's parents, next door, waiting for him with his three o'clock snack.
  ACTOR_TAVIS_FATHER,
  ACTOR_TAVIS_MOTHER,
  /// An old man in the middle house who sends boat seekers to Madra.
  ACTOR_OLD_MAN_ON_MADRA,
  /// An old woman in the middle house who tells the way to the Kandorean
  /// Temple.
  ACTOR_OLD_WOMAN_ON_THE_KANDOREAN_TEMPLE,
  /// "In all my years as mayor, I've never seen a wave like that before..."
  ACTOR_MAYOR,
  /// An older woman in the mayor's house, shaken by the noise after the wave.
  ACTOR_WOMAN_ON_THE_NOISE,
  /// A man in the mayor's house who heard the rumbling from the southeast.
  ACTOR_MAN_ON_THE_RUMBLING,
  /// A woman in the mayor's house on Daila's bad luck with pirates and waves.
  ACTOR_WOMAN_ON_DAILAS_LUCK,
  ACTOR_WEAPON_MERCHANT,
  ACTOR_ARMOR_MERCHANT,
  ACTOR_INNKEEPER,
  /// The innkeeper's one helper, who later practices for the dinner show.
  ACTOR_INN_WORKER,
  /// A guest of the inn, a man carrying a pack, who misses Madra's food.
  ACTOR_TRAVELER_FROM_MADRA,
  /// A guest of the inn who longs for Madra's seafood.
  ACTOR_GUEST_ON_MADRAS_SEAFOOD,
  /// Until Tavi is rescued: the family in the house on the islet with the cow,
  /// whose sons try the Sea of Time.
  ACTOR_WOMAN_ON_THE_CURRENTS,
  ACTOR_OLD_MAN_ON_THE_RED_ROCKS,
  ACTOR_OLD_WOMAN_ON_HER_SONS,
  /// An unseen actor on the armor shop's counter, so that the merchant behind
  /// it answers Felix from across it.
  ACTOR_ARMOR_COUNTER
};

/// The rows of gDailaRooms_HousesPlacementsOnceTaviIsRescued after those it
/// shares with gDailaRooms_HousesPlacements.
enum DailaRoomsActorIdAfterRescue {
  /// The two boys, back home, with their mouths full.
  ACTOR_RIKI = ACTOR_GUEST_ON_MADRAS_SEAFOOD + 1,
  ACTOR_TAVI,
  /// A woman of the inn caught practicing for the dinner show.
  ACTOR_PRACTICING_PERFORMER,
  ACTOR_WOMAN_ON_THE_CURRENTS_AFTER_RESCUE,
  ACTOR_OLD_MAN_ON_THE_RED_ROCKS_AFTER_RESCUE,
  ACTOR_OLD_WOMAN_ON_HER_SONS_AFTER_RESCUE,
  ACTOR_ARMOR_COUNTER_AFTER_RESCUE
};

/// Riki's and Tavi's own animation 5: eating at the table at home, their mouths
/// full.
enum { ANIM_EATING = 5 };

/// The way into the house on the islet with the cow (NorthOseniaIslet), which
/// this overlay also holds: the family by entrance 7.
enum { ENTRANCE_COW_ISLET_HOUSE = 7 };

/// The sanctuary's actors, numbering the rows of
/// gDailaRooms_SanctumPlacements.
enum DailaSanctuaryActorId {
  /// The sanctuary's healer, who sends travelers with nowhere to go to Poi at
  /// the Kandorean Temple, and once that is done heals the party.
  ACTOR_HEALER = ACTOR_FIRST_PLACED,
  /// The Sea God's Tear at the statue's foot: an actor with no picture of its
  /// own, on which the game draws the item (Actor_SetItemIcon) until it is
  /// taken.
  ACTOR_SEA_GODS_TEAR,
  /// The three puddles on the drained floor, from west to east, which Frost
  /// raises into pillars of ice.
  ACTOR_FIRST_PUDDLE,
  ACTOR_SECOND_PUDDLE,
  ACTOR_THIRD_PUDDLE,
  /// An actor with no picture, put where the Tear lay once it is taken.
  ACTOR_TEAR_SPOT
};

/// The first of three flags, one for each of the sanctuary's puddles Frost has
/// frozen into a pillar for this visit, by gDailaRooms_PillarFrozenFlags.
#define FLAG_DAILA_SANCTUARY_PUDDLE_FROZEN_FIRST 0x220
/// Holds which of his two spots the practicing man has jumped to last.
#define FLAG_DAILA_PRACTICE_AT_DOOR 0x200

extern s32 gDailaRooms_ExitsOnceTaviIsRescued[];
extern struct ActorPlacement gDailaRooms_HousesPlacements[];
extern struct ActorPlacement gDailaRooms_SanctumPlacements[];
extern struct ActorPlacement gDailaRooms_HousesPlacementsOnceTaviIsRescued[];
extern struct Trigger gDailaRooms_SanctumEvents[];
extern struct Trigger gDailaRooms_HousesEventsOnceTaviIsRescued[];
extern struct Trigger gDailaRooms_SanctumEventsOnceTaviIsRescued[];

void Sanctum_Run(s32 actor);

extern s16 gDailaRooms_PillarFrozenFlags[];

void FlagBlock_ApplyFlags(const s16 *flags);

s32 DailaRooms_GiveItem(s32 item, s32 mode);
void Scene_FadeInAndClearFlag(void);

void FieldEffect_StartBlankOverlay(s32 mode);

void Field_TryJumpForward(void);

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

extern s32 gDailaRooms_Exits[];

const void *Scene_GetExits(void) {
  if (GameFlag_Get(FLAG_TAVI_RESCUED))
    return gDailaRooms_ExitsOnceTaviIsRescued;
  return gDailaRooms_Exits;
}

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  if (gMainRoutineWork.next == SCENE_DAILA_ROOMS_SANCTUM)
    return gDailaRooms_SanctumPlacements;
  if (GameFlag_Get(FLAG_TAVI_RESCUED))
    return gDailaRooms_HousesPlacementsOnceTaviIsRescued;
  return gDailaRooms_HousesPlacements;
}

/// Unused: does nothing.
UNUSED void DailaRooms_EmptyHook(void) {}

/// Unused: the practicing man shouts and jumps to the other of his two spots.
UNUSED void DailaRooms_PracticeShout(s32 unused, s32 actor) {
  FieldActor_ShowEmote(actor, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 50);
  Actor_Jump(actor, 6, 15);
  Actor_Jump(actor, 6, 15);
  FieldEvent_Wait(10);
  if (!Event_GetFlag(FLAG_DAILA_PRACTICE_AT_DOOR)) {
    Event_SetMessageId((s32)MsgDailaInteriorsGah);
    // "GAH!"
    Event_ShowMessageAndWait(actor, 0);
    Actor_WalkToAndWait(actor, 360, 72);
    Actor_FaceDirectionById(actor, FACING_SOUTH, 0);
    GameFlag_Set(FLAG_DAILA_PRACTICE_AT_DOOR);
  } else {
    Event_SetMessageId((s32)MsgDailaInteriorsYarg);
    // "YARG!"
    Event_ShowMessageAndWait(actor, 0);
    Actor_WalkToAndWait(actor, 392, 104);
    Actor_FaceDirectionById(actor, FACING_WEST, 0);
    GameFlag_Clear(FLAG_DAILA_PRACTICE_AT_DOOR);
  }
  FieldEvent_Wait(20);
}

/// Unused: the practicing man turns to the leader, startled.
UNUSED void DailaRooms_StartleAtTheLeader(s32 unused, s32 actor) {
  Actor_FaceActorById(actor, ACTOR_FELIX, 0);
  FieldEvent_Wait(10);
  FieldActor_ShowEmote(actor, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 50);
  Actor_Jump(actor, 6, 15);
  Actor_Jump(actor, 6, 15);
  FieldEvent_Wait(10);
  Event_SetMessageId((s32)MsgDailaInteriorsWhatAreYouDoing);
  // "What are you doing???"
  Event_ShowMessageAndWait(actor, 0);
  FieldEvent_Wait(10);
}

/// Mind Read on the armor merchant behind the left-hand counter of the shop,
/// before Tavi is rescued: he thinks of the two scamps.
s32 DailaRooms_ReadArmorMerchantsMind(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgDailaInteriorsScamps);
  // "If those two scamps knew there were travelers in town, they'd have a..."
  Event_ShowMessageAndWait(ACTOR_ARMOR_MERCHANT, 0);
  Event_End();
  return 0;
}

/// Mind Read on the armor merchant once Tavi is rescued: he thinks of hidden
/// treasure.
s32 DailaRooms_ReadArmorMerchantsMindAfterRescue(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgDailaInteriorsMazeTreasure);
  // "Maybe they hid treasure here because this town is so mazelike..."
  Event_ShowMessageAndWait(ACTOR_ARMOR_MERCHANT, 0);
  Event_End();
  return 0;
}

/// Unused: starts an event and draws the leader in front of the counters.
UNUSED void DailaRooms_StartLeaderEvent(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_SetSpritePriority(gMainRoutineWork.leaderActorId, 1);
}

extern struct Trigger gDailaRooms_HousesEvents[];

const struct Trigger *Scene_GetTriggers(void) {
  s32 taviRescued = GameFlag_Get(FLAG_TAVI_RESCUED);
  struct MainRoutineWork *work = &gMainRoutineWork;
  s32 sanctum = SCENE_DAILA_ROOMS_SANCTUM;

  if (taviRescued) {
    if (work->next == sanctum)
      return gDailaRooms_SanctumEventsOnceTaviIsRescued;
    return gDailaRooms_HousesEventsOnceTaviIsRescued;
  }
  if (work->next == sanctum)
    return gDailaRooms_SanctumEvents;
  return gDailaRooms_HousesEvents;
}

s32 Inn_CheckIn(s32 mode, s32 objectId);

/// The weapon merchant at the right-hand counter of the shop sells to Felix
/// when he talks to him from in front of it, facing north; from any other side
/// he warns of the roads south.
void DailaRooms_TalkToWeaponMerchant(s32 actor) {
  u16 facing = Actor_GetById(gMainRoutineWork.leaderActorId)->facing;

  if (FACING_NEAREST_CARDINAL(facing) == FACING_NORTH) {
    Shop_Run(SHOP_DAILA_WEAPONS, actor);
  } else {
    Event_Start();
    Event_ResolvePendingActions(0);
    Event_SetMessageId((s32)MsgDailaInteriorsSouthRoadDanger);
    // "You folks ain't thinkin' 'bout goin' south, are ya? The roads to the..."
    Event_ShowMessageAndWait(actor, 0);
    Event_End();
  }
}

/// The armor merchant at the left-hand counter of the shop sells to Felix when
/// he talks to him from in front of it, facing north; from any other side he
/// asks what brings the party to Daila.
void DailaRooms_TalkToArmorMerchant(s32 actor) {
  u16 facing = Actor_GetById(gMainRoutineWork.leaderActorId)->facing;

  if (FACING_NEAREST_CARDINAL(facing) == FACING_NORTH) {
    Shop_Run(SHOP_DAILA_ARMOR, actor);
  } else {
    Event_Start();
    Event_ResolvePendingActions(0);
    Event_SetMessageId((s32)MsgDailaInteriorsNewTravelers);
    // "You guys look new 'round these parts! What brings you to here?..."
    Event_ShowMessageAndWait(actor, 0);
    Event_End();
  }
}

/// The innkeeper offers a room when Felix talks to him across the counter at
/// his east side, facing west; from any other side he wonders what to cook.
void DailaRooms_TalkToInnkeeper(s32 actor) {
  u16 facing = Actor_GetById(gMainRoutineWork.leaderActorId)->facing;

  if (FACING_NEAREST_CARDINAL(facing) == FACING_WEST) {
    Inn_CheckIn(DAILA_INN, actor);
  } else {
    Event_Start();
    Event_ResolvePendingActions(0);
    Event_SetMessageId((s32)MsgDailaInteriorsDinnerChoice);
    // "What should I cook for our guests tonight? I just can't decide."
    Event_ShowMessageAndWait(actor, 0);
    Event_End();
  }
}

/// The sanctuary's healer first asks whether the party has a lot of time and
/// nowhere to go, and sends them to Poi at the Kandorean Temple whatever they
/// answer. After that he heals when Felix talks to him from in front of his
/// counter, facing north, and from any other side asks again.
void DailaRooms_TalkToHealer(s32 actor) {
  s32 msg;
  u16 facing = Actor_GetById(gMainRoutineWork.leaderActorId)->facing;

  if (FACING_NEAREST_CARDINAL(facing) == FACING_NORTH) {
    if (GameFlag_Get(FLAG_DAILA_HEALER_SENT_TO_POI) == 0) {
      Event_Start();
      Event_ResolvePendingActions(0);
      msg = (s32)MsgDailaInteriorsTravelersFreeTime;
      Event_SetMessageId(msg);
      // "Travelers! Well, I'll be! Do you have a lot of time and nowhere to..."
      FieldMessage_ShowAtActor(actor, 0);
      if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
        FieldEvent_Wait(10);
        Event_SetMessageId(msg + 1);
      } else {
        FieldEvent_Wait(20);
        Event_SetMessageId(msg + 2);
      }
      Event_ShowMessageAndWait(actor, 0);
      Event_End();
      GameFlag_Set(FLAG_DAILA_HEALER_SENT_TO_POI);
    } else {
      Sanctum_Run(actor);
    }
  } else {
    Event_Start();
    Event_ResolvePendingActions(0);
    msg = (s32)MsgDailaInteriorsTravelersFreeTime;
    Event_SetMessageId(msg);
    // "Travelers! Well, I'll be! Do you have a lot of time and nowhere to go?"
    FieldMessage_ShowAtActor(actor, 0);
    if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
      FieldEvent_Wait(10);
      Event_SetMessageId(msg + 1);
    } else {
      FieldEvent_Wait(20);
      Event_SetMessageId(msg + 2);
    }
    Event_ShowMessageAndWait(actor, 0);
    Event_End();
    GameFlag_Set(FLAG_DAILA_HEALER_SENT_TO_POI);
  }
}

/// Once Tavi is rescued, the sanctuary's healer heals when Felix talks to him
/// from in front of his counter, facing north, and from any other side tells
/// the party there is nothing left for them in Daila.
void DailaRooms_TalkToHealerOnceTaviIsRescued(s32 actor) {
  u16 facing = Actor_GetById(gMainRoutineWork.leaderActorId)->facing;

  if (FACING_NEAREST_CARDINAL(facing) == FACING_NORTH) {
    Sanctum_Run(actor);
  } else {
    Event_Start();
    Event_ResolvePendingActions(0);
    Event_SetMessageId((s32)MsgDailaInteriorsLeaveForMadra);
    // "There is nothing left for you in Daila... Please be careful on your..."
    Event_ShowMessageAndWait(actor, 0);
    Event_End();
  }
}

/// Once Tavi is rescued, the weapon merchant still sells from in front of his
/// counter, and from any other side tells of Daila's pirate past.
void DailaRooms_TalkToWeaponMerchantOnceTaviIsRescued(s32 actor) {
  u16 facing = Actor_GetById(gMainRoutineWork.leaderActorId)->facing;

  if (FACING_NEAREST_CARDINAL(facing) == FACING_NORTH) {
    Shop_Run(SHOP_DAILA_WEAPONS, actor);
  } else {
    Event_Start();
    Event_ResolvePendingActions(0);
    Event_SetMessageId((s32)MsgDailaInteriorsPirateTreasure);
    // "Way back when, Daila used to be a hideout for a band of pirates......"
    Event_ShowMessageAndWait(actor, 0);
    Event_End();
  }
}

/// Once Tavi is rescued, the armor merchant still sells from in front of his
/// counter, and from any other side talks of how spacious the village is.
void DailaRooms_TalkToArmorMerchantOnceTaviIsRescued(s32 actor) {
  u16 facing = Actor_GetById(gMainRoutineWork.leaderActorId)->facing;

  if (FACING_NEAREST_CARDINAL(facing) == FACING_NORTH) {
    Shop_Run(SHOP_DAILA_ARMOR, actor);
  } else {
    Event_Start();
    Event_ResolvePendingActions(0);
    Event_SetMessageId((s32)MsgDailaInteriorsSpaciousVillage);
    // "We don't have a lot of people in Daila, but it's a very spacious..."
    Event_ShowMessageAndWait(actor, 0);
    Event_End();
  }
}

/// Once Tavi is rescued, the innkeeper still offers a room across the counter
/// at his east side, and from any other side wonders when the fishermen can
/// put to sea again.
void DailaRooms_TalkToInnkeeperOnceTaviIsRescued(s32 actor) {
  u16 facing = Actor_GetById(gMainRoutineWork.leaderActorId)->facing;

  if (FACING_NEAREST_CARDINAL(facing) == FACING_WEST) {
    Inn_CheckIn(DAILA_INN, actor);
  } else {
    Event_Start();
    Event_ResolvePendingActions(0);
    Event_SetMessageId((s32)MsgDailaInteriorsFishermenWaiting);
    // "Our fishermen still can't head out to sea... When will they start..."
    Event_ShowMessageAndWait(actor, 0);
    Event_End();
  }
}

/// Once Tavi is rescued, the old man in the middle house asks whether Felix is
/// going to Madra, and answers yes or no.
void DailaRooms_AskGoingToMadra(void) {
  s32 msg;

  Event_Start();
  Event_ResolvePendingActions(0);
  msg = (s32)MsgDailaInteriorsGoingToMadra;
  Event_SetMessageId(msg);
  // "Are you going to Madra?"
  FieldMessage_ShowAtActor(ACTOR_OLD_MAN_ON_MADRA, 0);
  if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
    FieldEvent_Wait(10);
    Event_SetMessageId(msg + 1);
  } else {
    FieldEvent_Wait(20);
    Event_SetMessageId(msg + 2);
  }
  Event_ShowMessageAndWait(ACTOR_OLD_MAN_ON_MADRA, 0);
  Event_End();
}

/// Once Tavi is rescued, the mayor in his house asks whether the party is
/// looking for a boat too, and answers yes or no.
void DailaRooms_AskLookingForBoat(void) {
  s32 msg;

  Event_Start();
  Event_ResolvePendingActions(0);
  msg = (s32)MsgDailaInteriorsLookingForBoat;
  Event_SetMessageId(msg);
  // "Don't tell me you're all looking for a boat, too!"
  FieldMessage_ShowAtActor(ACTOR_MAYOR, 0);
  if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
    FieldEvent_Wait(10);
    Event_SetMessageId(msg + 1);
  } else {
    FieldEvent_Wait(20);
    Event_SetMessageId(msg + 2);
  }
  Event_ShowMessageAndWait(ACTOR_MAYOR, 0);
  Event_End();
}

/// Once Tavi is rescued, the woman at the inn caught practicing for the dinner
/// show hopes Felix was not watching, then explains.
void DailaRooms_TalkCaughtPracticing(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgDailaInteriorsWatchingPractice);
  // "Er... Aheh heh... You... weren't watching, were you?"
  Event_ShowMessageAndWait(ACTOR_PRACTICING_PERFORMER, 0);
  FieldEvent_Wait(30);
  Actor_SetAnimationById(ACTOR_PRACTICING_PERFORMER, ANIM_NOD);
  FieldEvent_Wait(30);
  // "I was just practicing for the show we're putting on tonight."
  Event_ShowMessageAndWait(ACTOR_PRACTICING_PERFORMER, 0);
  Event_End();
}

#include "shared/scene_freeze_puddle_into_pillar.h"

void ItemDisplay_ShowPicture(struct Actor *actor, s32 mode);
void Actor_StartIdle(struct Actor *actor);

/// Felix picks up the Sea God's Tear at the foot of the statue behind the
/// healer (cell (14, 12)): "Felix got the Sea God's Tear." When the party's
/// bags are full he is asked what to drop for it, and if he keeps everything
/// the Tear stays where it was.
void DailaRooms_TakeSeaGodsTear(void) {
  s32 savedGravity;
  s32 savedVelocityY;
  FIXED savedY;
  s32 item;

  savedGravity = Actor_GetById(ACTOR_SEA_GODS_TEAR)->gravity;
  item = ITEM_SEA_GODS_TEAR;
  savedVelocityY = Actor_GetById(ACTOR_SEA_GODS_TEAR)->velocityY;
  savedY = Actor_GetById(ACTOR_SEA_GODS_TEAR)->y.fixed;
  Event_Start();
  Event_ResolvePendingActions(0);
  if (PartyInventory_FindRoom(item) >= 0) {
    ItemDisplay_ShowPicture(Actor_GetById(ACTOR_SEA_GODS_TEAR),
                            ITEM_DISPLAY_SPARKLES | ITEM_DISPLAY_RAISED);
    Sound_Play(MUS_TREASURE_FOUND);
    MessageArgument_Push(item, MESSAGE_ARGUMENT_ITEM);
    // "Felix got ...."
    FieldMessage_Show((s32)MsgItemReceived, FIELD_MESSAGE_ZOOM);
    PartyInventory_Add(item);
    Actor_SetPosition(ACTOR_SEA_GODS_TEAR, 0, 0);
    Actor_SetPosition(ACTOR_TEAR_SPOT, 232, 194);
    GameFlag_Set(FLAG_SEA_GODS_TEAR_TAKEN);
  } else {
    ItemDisplay_ShowPicture(Actor_GetById(ACTOR_SEA_GODS_TEAR),
                            ITEM_DISPLAY_SPARKLES | ITEM_DISPLAY_RAISED);
    Sound_Play(MUS_TREASURE_FOUND);
    if (DailaRooms_GiveItem(item, 0) != -1) {
      Actor_SetPosition(ACTOR_SEA_GODS_TEAR, 0, 0);
      Actor_SetPosition(ACTOR_TEAR_SPOT, 232, 196);
      GameFlag_Set(FLAG_SEA_GODS_TEAR_TAKEN);
    } else {
      Actor_SetPosition(ACTOR_SEA_GODS_TEAR, 0, 0);
      FieldEvent_Wait(1);
      Actor_GetById(ACTOR_SEA_GODS_TEAR)->velocityY = savedVelocityY;
      Actor_GetById(ACTOR_SEA_GODS_TEAR)->y.fixed = savedY;
      Actor_GetById(ACTOR_SEA_GODS_TEAR)->gravity = savedGravity;
      Actor_GetById(ACTOR_SEA_GODS_TEAR)->update = NULL;
      Actor_StartIdle(Actor_GetById(ACTOR_SEA_GODS_TEAR));
      Actor_SetSpritePriority(ACTOR_SEA_GODS_TEAR, 1);
      Actor_GetById(ACTOR_SEA_GODS_TEAR)->motionFlags = ACTOR_FALLS;
      Actor_GetById(ACTOR_SEA_GODS_TEAR)->scaleY = FIXED_ONE * 6 / 5;
      Actor_GetById(ACTOR_SEA_GODS_TEAR)->scaleX = FIXED_ONE * 6 / 5;
      Actor_GetById(ACTOR_SEA_GODS_TEAR)->collisionFlags |= ACTOR_NEAR_TALK;
      Actor_SetPosition(ACTOR_SEA_GODS_TEAR, 232, 196);
    }
  }
  Event_End();
}

/// Gives an item to the party, asking what to drop when no one has room for
/// it; returns who took it, or -1. The overlay's own copy of the field's
/// PartyInventory_GiveItem.
s32 DailaRooms_GiveItem(s32 item, s32 unused) {
  struct FieldWork *work;
  s32 saved;
  s32 owner;
  s32 result;
  s32 count;
  s32 i;
  s32 member;
  s32 slot;

  work = Ram_HeapSlots->fieldWork;
  saved = work->message;
  owner = PartyInventory_Add(item);
  if (owner == -1) {
    MessageArgument_Push(item, MESSAGE_ARGUMENT_ITEM);
    // "Felix got ...."
    FieldMessage_Show((s32)MsgItemReceived, FIELD_MESSAGE_ZOOM);
    // "But you can't carry any more items."
    FieldMessage_Show((s32)MsgItemCannotCarry, FIELD_MESSAGE_ZOOM);
    for (;;) {
      // "What will you drop?"
      FieldMessage_Show((s32)MsgItemDropPrompt, FIELD_MESSAGE_ZOOM);
      result = ItemPicker_RunToDrop(&member, &slot);
      if (result == -1) {
        if (Item_GetDirect(item)->flags & ITEM_INDISPENSABLE) {
          MessageArgument_Push(item, MESSAGE_ARGUMENT_ITEM);
          // "That ...'s valuable! You must get it!"
          FieldMessage_Show((s32)MsgItemDropPrompt + 4, FIELD_MESSAGE_ZOOM);
          continue;
        }
        MessageArgument_Push(item, MESSAGE_ARGUMENT_ITEM);
        // "Drop the ...?"
        FieldMessage_Show((s32)MsgItemDropPrompt + 1,
                          FIELD_MESSAGE_ZOOM | FIELD_MESSAGE_KEEP_OPEN);
        result = FieldYesNo_Ask(1);
        Message_CloseFinishedWindows();
        if (result != 0)
          continue;
        MessageArgument_Push(item, MESSAGE_ARGUMENT_ITEM);
        // "Felix drops the ...."
        FieldMessage_Show((s32)MsgItemDropPrompt + 2, FIELD_MESSAGE_ZOOM);
        work->message = saved;
        return owner;
      }
      Unit_Get(member);
      count = Inventory_GetQuantity(member, slot);
      if (Inventory_CountItem(member, item) >= INVENTORY_STACK_MAX) {
        MessageArgument_Push(member, MESSAGE_ARGUMENT_ACTOR);
        MessageArgument_Push(item, MESSAGE_ARGUMENT_ITEM);
        // That member already carries a full stack of the item; the line
        // this shows reads only "Empty".
        FieldMessage_Show((s32)MsgItemDropPrompt + 7, FIELD_MESSAGE_ZOOM);
        continue;
      }
      break;
    }
    for (i = 0; i < count; i++)
      Inventory_Discard(member, slot);
    Unit_RefreshPsynergy(member);
    Unit_Calculate(member);
    owner = PartyInventory_Add(item);
    Sound_Play(MUS_TREASURE_FOUND);
    if (owner == gMainRoutineWork.leaderActorId) {
      MessageArgument_Push(item, MESSAGE_ARGUMENT_ITEM);
      // "Felix got ...."
      FieldMessage_Show((s32)MsgItemReceived,
                        FIELD_MESSAGE_ZOOM | FIELD_MESSAGE_WAITING_FOR_SOUND);
    } else {
      MessageArgument_Push(item, MESSAGE_ARGUMENT_ITEM);
      MessageArgument_Push(owner, MESSAGE_ARGUMENT_ACTOR);
      // "Felix gave ... to ...."
      FieldMessage_Show((s32)MsgItemGiven,
                        FIELD_MESSAGE_ZOOM | FIELD_MESSAGE_WAITING_FOR_SOUND);
    }
    work->message = saved;
    return owner;
  } else {
    Sound_Play(MUS_TREASURE_FOUND);
    MessageArgument_Push(item, MESSAGE_ARGUMENT_ITEM);
    // "Felix got ...."
    FieldMessage_Show((s32)MsgItemReceived,
                      FIELD_MESSAGE_ZOOM | FIELD_MESSAGE_WAITING_FOR_SOUND);
    if (owner != gMainRoutineWork.leaderActorId) {
      MessageArgument_Push(item, MESSAGE_ARGUMENT_ITEM);
      MessageArgument_Push(owner, MESSAGE_ARGUMENT_ACTOR);
      // "Felix gave ... to ...."
      FieldMessage_Show((s32)MsgItemReceived + 1, FIELD_MESSAGE_ZOOM);
    }
    work->message = saved;
  }
  return owner;
}

void TilesetAnim_Pause(s32 mode);

void FlagBlock_PlaceAll(const s16 *flags);

/// Sets up the houses (the boys eating once they are home, the armor merchant
/// behind his counter) or the sanctuary (the Tear on the pillars' top until it
/// is taken, the puddles until Tavi is rescued, the frozen pillars after).
s32 Scene_Initialize(void) {
  struct MapWorkGroundEffects *map;
  s32 *effect = &gFieldWork->transition;
  s32 value = SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);

  *effect = value;
  if (gMainRoutineWork.next == SCENE_DAILA_ROOMS_HOUSES) {
    if (gMainRoutineWork.entrance == ENTRANCE_COW_ISLET_HOUSE) {
      Scene_FadeInAndClearFlag();
    } else {
      if (GameFlag_Get(FLAG_TAVI_RESCUED)) {
        Actor_SetAnimationById(ACTOR_RIKI, ANIM_EATING);
        Actor_SetSpriteFlags(Actor_GetById(ACTOR_RIKI), 0);
        Actor_SetAnimationById(ACTOR_TAVI, ANIM_EATING);
        Actor_SetSpriteFlags(Actor_GetById(ACTOR_TAVI), 0);
      }
      Actor_GetById(gMainRoutineWork.leaderActorId)->priorityFlags |=
          ACTOR_PRIORITY_IGNORES_RISES;
      Actor_GetById(ACTOR_OLD_WOMAN_ON_THE_KANDOREAN_TEMPLE)->priorityFlags |=
          ACTOR_PRIORITY_IGNORES_RISES;
      Actor_GetById(ACTOR_ARMOR_MERCHANT)->priorityFlags |=
          ACTOR_PRIORITY_IGNORES_RISES;
      Actor_SetSpritePriority(ACTOR_ARMOR_MERCHANT, 2);
      Actor_GetById(ACTOR_ARMOR_MERCHANT)->collisionFlags |= ACTOR_NEAR_TALK;
    }
  } else if (gMainRoutineWork.next == SCENE_DAILA_ROOMS_SANCTUM) {
    map = (struct MapWorkGroundEffects *)Ram_HeapSlots->mapGrid;
    Event_Start();
    Event_ResolvePendingActions(0);
    Actor_SetSpriteFlags(Actor_GetById(ACTOR_TEAR_SPOT), 0);
    FieldEffect_StartBlankOverlay(0);
    if (!GameFlag_Get(FLAG_SEA_GODS_TEAR_TAKEN)) {
      Actor_SetItemIcon(Actor_GetById(ACTOR_SEA_GODS_TEAR), ITEM_SEA_GODS_TEAR);
      Actor_SetSpriteFlags(Actor_GetById(ACTOR_SEA_GODS_TEAR), 0);
      Actor_SetSpritePriority(ACTOR_SEA_GODS_TEAR, 1);
      Actor_GetById(ACTOR_SEA_GODS_TEAR)->scaleY = FIXED_ONE * 6 / 5;
      Actor_GetById(ACTOR_SEA_GODS_TEAR)->scaleX = FIXED_ONE * 6 / 5;
      Actor_GetById(ACTOR_SEA_GODS_TEAR)->collisionFlags |= ACTOR_NEAR_TALK;
      Actor_GetById(ACTOR_TEAR_SPOT)->collisionFlags |= ACTOR_NEAR_TALK;
    } else {
      Actor_GetById(ACTOR_SEA_GODS_TEAR)->collisionFlags |= ACTOR_NEAR_TALK;
      Actor_SetSpriteFlags(Actor_GetById(ACTOR_SEA_GODS_TEAR), 0);
    }
    if (!GameFlag_Get(FLAG_TAVI_RESCUED)) {
      Actor_SetPosition(ACTOR_FIRST_PUDDLE, 0, 0);
      Actor_SetPosition(ACTOR_SECOND_PUDDLE, 0, 0);
      Actor_SetPosition(ACTOR_THIRD_PUDDLE, 0, 0);
      map->wading = MAP_WADING_UPPER_LEVEL;
    } else {
      FlagBlock_PlaceAll(gDailaRooms_PillarFrozenFlags);
    }
    Event_End();
  }
  return 0;
}

/// Once Tavi is rescued, draws the sanctuary's pool drained down to its three
/// puddles, from the map's spare copy of it.
s32 Scene_PrepareMap(void) {
  if (gMainRoutineWork.next == SCENE_DAILA_ROOMS_SANCTUM &&
      GameFlag_Get(FLAG_TAVI_RESCUED)) {
    TilesetAnim_Pause(0);
    MapGrid_CopyCells(29, 33, 10, 6, 10, 12);
  }
  return 0;
}

#include "shared/scene_fade_in_and_clear_flag.h"

/// Felix hops over one of the sanctuary's drained puddles when he walks into
/// it from the cell beside it on any side, landing on the far side if the
/// ground there is clear.
void DailaRooms_JumpForward(void) { Field_TryJumpForward(); }

// clang-format off
s16 gDailaRooms_PillarFrozenFlags[] = {
    ACTOR_FIRST_PUDDLE,  FLAG_DAILA_SANCTUARY_PUDDLE_FROZEN_FIRST + 0,
    ACTOR_SECOND_PUDDLE, FLAG_DAILA_SANCTUARY_PUDDLE_FROZEN_FIRST + 1,
    ACTOR_THIRD_PUDDLE,  FLAG_DAILA_SANCTUARY_PUDDLE_FROZEN_FIRST + 2,
    -1, 0,
};

/// Tavi's father's script: he paces his house back and forth.
s32 gDailaRooms_PacingScript[] = {
#if !TLA_PATCH_1
    SC_SET(SW_COLLISION, ACTOR_BLOCKED_BY_SOLID | ACTOR_PUSHABLE | ACTOR_SOLID),
#else
    SC_SET(SW_COLLISION, ACTOR_BLOCKED_BY_SOLID | ACTOR_SOLID),
#endif
    SC_ACCELERATION(0.1),
    SC_SPEED(0.3),
    SC_LABEL(1),
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(312, 0, 168),
    SC_SYNC,
    SC_WAIT(20),
    SC_ANIMATION(ANIM_STAND),
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(20),
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(392, 0, 168),
    SC_SYNC,
    SC_WAIT(20),
    SC_ANIMATION(ANIM_STAND),
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(20),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
    SC_END,
};

/// The inn's two performers' script once Tavi is rescued: they turn on the
/// spot, rehearsing the dinner show.
s32 gDailaRooms_LookAroundScript[] = {
    SC_SET(SW_COLLISION, ACTOR_BLOCKED_BY_SOLID | ACTOR_SOLID),
    SC_ACCELERATION(0.1),
    SC_SPEED(0.3),
    SC_LABEL(1),
    SC_ANIMATION(ANIM_STAND),
    SC_SET(SW_FACING, FACING_SOUTHWEST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_NORTHWEST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_NORTH),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_SOUTHEAST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_SOUTHWEST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_NORTHWEST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_NORTH),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_SOUTHEAST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_SOUTHWEST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_NORTHWEST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_NORTH),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_SOUTHEAST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_WAIT(5),
    SC_SET(SW_Y, toFIXED(8)),
    SC_WAIT(30),
    SC_SET(SW_Y, toFIXED(8)),
    SC_WAIT(30),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
    SC_END,
    // Unused: a second script after the end, wandering near home.
#if !TLA_PATCH_1
    SC_SET(SW_COLLISION, ACTOR_BLOCKED_BY_SOLID | ACTOR_PUSHABLE | ACTOR_SOLID),
#else
    SC_SET(SW_COLLISION, ACTOR_BLOCKED_BY_SOLID | ACTOR_SOLID),
#endif
    SC_ACCELERATION(0.1),
    SC_SPEED(0.3),
    SC_LABEL(1),
    SC_ANIMATION(ANIM_STAND),
    SC_ANIMATION(ANIM_WALK),
    SC_WANDER_HOME(2, 2, 6),
    SC_SYNC,
    SC_WAIT(4),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
    SC_END,
};
// clang-format on

#include "shared/scene_entrances_default.c"

// clang-format off
/// Where the houses' doors lead, before Tavi is rescued, out into the flooded village.
s32 gDailaRooms_Exits[] = {
    SCENE_EXITS(DailaRoomsHouses),
    SCENE_EXIT_UNLESS(1, DailaFlooded, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, DailaFlooded, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, DailaFlooded, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, DailaFlooded, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(5, DailaFlooded, 5, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(6, DailaFlooded, 6, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(7, NorthOseniaIslet, 3, EXIT_FLAG_NONE),
    SCENE_EXITS(DailaRoomsSanctum),
    SCENE_EXIT_UNLESS(1, DailaFlooded, 7, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

/// Where the houses' doors lead once Tavi is rescued, out into the drained village.
s32 gDailaRooms_ExitsOnceTaviIsRescued[] = {
    SCENE_EXITS(DailaRoomsHouses),
    SCENE_EXIT_UNLESS(1, Daila, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, Daila, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, Daila, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, Daila, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(5, Daila, 5, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(6, Daila, 6, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(7, NorthOseniaIslet, 3, EXIT_FLAG_NONE),
    SCENE_EXITS(DailaRoomsSanctum),
    SCENE_EXIT_UNLESS(1, Daila, 7, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

/// The houses, shops and inn before Tavi is rescued (enum
/// DailaRoomsActorId).
struct ActorPlacement gDailaRooms_HousesPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 104, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 88, 0, 88, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_CURLY_HAIR, CONDITION_ALWAYS, (s32)gDailaRooms_PacingScript, 360, 0, 168, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 72, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_GREY_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 632, 0, 168, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN_WITH_HEADBAND, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 568, 0, 88, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_CURLY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 160, 0, 360, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 456, FACING_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 440, FACING_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 216, 0, 472, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WEAPON_MERCHANT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 712, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_ARMOR_MERCHANT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 88, 0, 724, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_INNKEEPER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 456, 0, 468, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_INN_WORKER, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 536, 0, 456, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_CARRYING_A_PACK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 440, 0, 376, FACING_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 424, 0, 360, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_AUBURN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 472, 0, 746, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_BLUE_COAT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 568, 0, 712, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN_IN_GREEN_SHAWL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 504, 0, 712, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 88, 0, 728, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

/// The sanctuary (enum DailaSanctuaryActorId).
struct ActorPlacement gDailaRooms_SanctumPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_HEALER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 333, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 196, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, FLAG_SCENE_TEMP_23, MOVEMENT_TYPE_STAND, 184, 0, 248, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, FLAG_SCENE_TEMP_24, MOVEMENT_TYPE_STAND, 216, 0, 248, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, FLAG_SCENE_TEMP_25, MOVEMENT_TYPE_STAND, 248, 0, 232, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

/// The houses, shops and inn once Tavi is rescued, with Riki and Tavi home.
struct ActorPlacement gDailaRooms_HousesPlacementsOnceTaviIsRescued[] = {
    ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 176, 0, 120, FACING_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 88, 0, 88, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_CURLY_HAIR, CONDITION_ALWAYS, (s32)gDailaRooms_PacingScript, 360, 0, 168, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 72, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_GREY_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 632, 0, 168, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN_WITH_HEADBAND, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 568, 0, 88, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_CURLY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 160, 0, 360, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 88, 0, 440, FACING_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 488, FACING_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 216, 0, 472, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WEAPON_MERCHANT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 712, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_ARMOR_MERCHANT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 88, 0, 724, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_INNKEEPER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 456, 0, 468, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_INN_WORKER, CONDITION_ALWAYS, (s32)gDailaRooms_LookAroundScript, 424, 0, 520, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_CARRYING_A_PACK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 440, 0, 376, FACING_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 424, 0, 360, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_RIKI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 161, 0, 111, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_TAVI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 112, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS, (s32)gDailaRooms_LookAroundScript, 456, 0, 520, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_AUBURN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 472, 0, 746, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_BLUE_COAT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 568, 0, 712, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN_IN_GREEN_SHAWL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 504, 0, 712, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 88, 0, 728, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

/// The houses' doors, talk and Mind Read before Tavi is rescued.
struct Trigger gDailaRooms_HousesEvents[] = {
    {TRIGGER_EXIT, DAILA_ROOMS_HOUSES_TO_DAILA_WEST, CONDITION_ALWAYS, DAILA_ROOMS_HOUSES_TO_DAILA_WEST},
    {TRIGGER_EXIT, DAILA_ROOMS_HOUSES_TO_DAILA_NORTHWEST, CONDITION_ALWAYS, DAILA_ROOMS_HOUSES_TO_DAILA_NORTHWEST},
    {TRIGGER_EXIT, DAILA_ROOMS_HOUSES_TO_DAILA_NORTHEAST, CONDITION_ALWAYS, DAILA_ROOMS_HOUSES_TO_DAILA_NORTHEAST},
    {TRIGGER_EXIT, DAILA_ROOMS_HOUSES_TO_DAILA_EAST, CONDITION_ALWAYS, DAILA_ROOMS_HOUSES_TO_DAILA_EAST},
    {TRIGGER_EXIT, DAILA_ROOMS_HOUSES_TO_DAILA_SOUTHWEST, CONDITION_ALWAYS, DAILA_ROOMS_HOUSES_TO_DAILA_SOUTHWEST},
    {TRIGGER_EXIT, DAILA_ROOMS_HOUSES_TO_DAILA_SOUTHEAST, CONDITION_ALWAYS, DAILA_ROOMS_HOUSES_TO_DAILA_SOUTHEAST},
    {TRIGGER_EXIT, DAILA_ROOMS_HOUSES_TO_NORTH_OSENIA_ISLET, CONDITION_ALWAYS, DAILA_ROOMS_HOUSES_TO_NORTH_OSENIA_ISLET},
    {TRIGGER_TALK, ACTOR_RIKIS_FATHER, CONDITION_ALWAYS, MSGNO(MsgFieldRikisReallyInForItWhen)},
    {TRIGGER_TALK, ACTOR_RIKIS_MOTHER, CONDITION_ALWAYS, MSGNO(MsgFieldThatNoGoodKidOfMine)},
    {TRIGGER_TALK, ACTOR_TAVIS_FATHER, CONDITION_ALWAYS, MSGNO(MsgFieldIWonderWhereOurLittleTavi)},
    {TRIGGER_TALK, ACTOR_TAVIS_MOTHER, CONDITION_ALWAYS, MSGNO(MsgFieldTaviNeverMissesThreeOclockSnacksies)},
    {TRIGGER_TALK, ACTOR_OLD_MAN_ON_MADRA, CONDITION_ALWAYS, MSGNO(MsgFieldIfYoureLookingForABoat)},
    {TRIGGER_TALK, ACTOR_OLD_WOMAN_ON_THE_KANDOREAN_TEMPLE, CONDITION_ALWAYS, MSGNO(MsgFieldIfYouHeadSouthYoullFind)},
    {TRIGGER_TALK, ACTOR_MAYOR, CONDITION_ALWAYS, MSGNO(MsgFieldInAllMyYearsAsMayor)},
    {TRIGGER_TALK, ACTOR_WOMAN_ON_THE_NOISE, CONDITION_ALWAYS, MSGNO(MsgFieldTheWaveWasAwfulButThe)},
    {TRIGGER_TALK, ACTOR_MAN_ON_THE_RUMBLING, CONDITION_ALWAYS, MSGNO(MsgFieldALotOfFolkSaidThey)},
    {TRIGGER_TALK, ACTOR_WOMAN_ON_DAILAS_LUCK, CONDITION_ALWAYS, MSGNO(MsgFieldBetweenThePiratesAndTheTidal)},
    {TRIGGER_TALK, ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS, EVENT(DailaRooms_TalkToWeaponMerchant)},
    {TRIGGER_TALK, ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS, EVENT(DailaRooms_TalkToArmorMerchant)},
    {TRIGGER_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS, EVENT(DailaRooms_TalkToInnkeeper)},
    {TRIGGER_TALK, ACTOR_INN_WORKER, CONDITION_ALWAYS, MSGNO(MsgFieldWeRunAVerySmallInn)},
    {TRIGGER_TALK, ACTOR_TRAVELER_FROM_MADRA, CONDITION_ALWAYS, MSGNO(MsgFieldIDontKnowWhenIllBe)},
    {TRIGGER_TALK, ACTOR_GUEST_ON_MADRAS_SEAFOOD, CONDITION_ALWAYS, MSGNO(MsgFieldIHearTheSeafoodInMadra)},
    {TRIGGER_TALK, ACTOR_WOMAN_ON_THE_CURRENTS, CONDITION_ALWAYS, MSGNO(MsgSeaOfTimeCurrents)},
    {TRIGGER_TALK, ACTOR_OLD_MAN_ON_THE_RED_ROCKS, CONDITION_ALWAYS, MSGNO(MsgSeaOfTimeRedRocks)},
    {TRIGGER_TALK, ACTOR_OLD_WOMAN_ON_HER_SONS, CONDITION_ALWAYS, MSGNO(MsgSeaOfTimeThreeGenerations)},
    {TRIGGER_TALK, ACTOR_ARMOR_COUNTER, CONDITION_ALWAYS, EVENT(DailaRooms_TalkToArmorMerchant)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_RIKIS_FATHER, CONDITION_ALWAYS, MSGNO(MsgFieldRikiTaviWhereAreYouWhat)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_RIKIS_MOTHER, CONDITION_ALWAYS, MSGNO(MsgFieldThatTavisABadInfluenceOn)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TAVIS_FATHER, CONDITION_ALWAYS, MSGNO(MsgFieldIllBetThatWhereverTaviIs)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TAVIS_MOTHER, CONDITION_ALWAYS, MSGNO(MsgFieldIfTaviIsntHomeSoonWere)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ON_MADRA, CONDITION_ALWAYS, MSGNO(MsgFieldIWonderHowTheRoadTo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_ON_THE_KANDOREAN_TEMPLE, CONDITION_ALWAYS, MSGNO(MsgFieldIWonderIfTheHolyMan)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAYOR, CONDITION_ALWAYS, MSGNO(MsgFieldAtLeastNobodySeemsToHave)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_THE_NOISE, CONDITION_ALWAYS, MSGNO(MsgFieldIStillDontKnowWhatThat)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_THE_RUMBLING, CONDITION_ALWAYS, MSGNO(MsgFieldWeShouldFindADifferentWay)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_DAILAS_LUCK, CONDITION_ALWAYS, MSGNO(MsgFieldNowThatBriggsIsLockedAway)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgFieldImGladImNotGoingTo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgDailaInteriorsScamps)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, CONDITION_ALWAYS, MSGNO(MsgFieldWeCantSailAndWeCant)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INN_WORKER, CONDITION_ALWAYS, MSGNO(MsgFieldLookAtAllTheseGuestsWe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TRAVELER_FROM_MADRA, CONDITION_ALWAYS, MSGNO(MsgFieldAfterAllThatShakingIWouldnt)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GUEST_ON_MADRAS_SEAFOOD, CONDITION_ALWAYS, MSGNO(MsgFieldImSoTornDoILike)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_THE_CURRENTS, CONDITION_ALWAYS, MSGNO(MsgSeaOfTimeHusbandDizzy)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ON_THE_RED_ROCKS, CONDITION_ALWAYS, MSGNO(MsgSeaOfTimeYeppToldMe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_ON_HER_SONS, CONDITION_ALWAYS, MSGNO(MsgSeaOfTimeGiveUp)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARMOR_COUNTER, CONDITION_ALWAYS, EVENT(DailaRooms_ReadArmorMerchantsMind)},
    {TRIGGER_TABLE_END},
};

/// The sanctuary's doors, talk and Mind Read before Tavi is rescued.
struct Trigger gDailaRooms_SanctumEvents[] = {
    {TRIGGER_EXIT, DAILA_ROOMS_SANCTUM_TO_DAILA, CONDITION_ALWAYS, DAILA_ROOMS_SANCTUM_TO_DAILA},
    // Unused: the sanctuary has no cells for exits 2 to 6, nor exits of those numbers.
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_TALK, ACTOR_HEALER, CONDITION_ALWAYS, EVENT(DailaRooms_TalkToHealer)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, CONDITION_ALWAYS, MSGNO(MsgFieldTravelersWithNowhereToGoThis)},
    {TRIGGER_TABLE_END},
};

/// The houses' doors, talk and Mind Read once Tavi is rescued.
struct Trigger gDailaRooms_HousesEventsOnceTaviIsRescued[] = {
    {TRIGGER_EXIT, DAILA_ROOMS_HOUSES_TO_DAILA_WEST, CONDITION_ALWAYS, DAILA_ROOMS_HOUSES_TO_DAILA_WEST},
    {TRIGGER_EXIT, DAILA_ROOMS_HOUSES_TO_DAILA_NORTHWEST, CONDITION_ALWAYS, DAILA_ROOMS_HOUSES_TO_DAILA_NORTHWEST},
    {TRIGGER_EXIT, DAILA_ROOMS_HOUSES_TO_DAILA_NORTHEAST, CONDITION_ALWAYS, DAILA_ROOMS_HOUSES_TO_DAILA_NORTHEAST},
    {TRIGGER_EXIT, DAILA_ROOMS_HOUSES_TO_DAILA_EAST, CONDITION_ALWAYS, DAILA_ROOMS_HOUSES_TO_DAILA_EAST},
    {TRIGGER_EXIT, DAILA_ROOMS_HOUSES_TO_DAILA_SOUTHWEST, CONDITION_ALWAYS, DAILA_ROOMS_HOUSES_TO_DAILA_SOUTHWEST},
    {TRIGGER_EXIT, DAILA_ROOMS_HOUSES_TO_DAILA_SOUTHEAST, CONDITION_ALWAYS, DAILA_ROOMS_HOUSES_TO_DAILA_SOUTHEAST},
    {TRIGGER_EXIT, DAILA_ROOMS_HOUSES_TO_NORTH_OSENIA_ISLET, CONDITION_ALWAYS, DAILA_ROOMS_HOUSES_TO_NORTH_OSENIA_ISLET},
    {TRIGGER_TALK, ACTOR_RIKIS_FATHER, CONDITION_ALWAYS, MSGNO(MsgFieldLetMeTellYouKidsAre)},
    {TRIGGER_TALK, ACTOR_RIKIS_MOTHER, CONDITION_ALWAYS, MSGNO(MsgFieldWhoStaysOutsideUntilHesAbsolutely)},
    {TRIGGER_TALK, ACTOR_TAVIS_FATHER, CONDITION_ALWAYS, MSGNO(MsgFieldOurLittleOneIsFinallyHome)},
    {TRIGGER_TALK, ACTOR_TAVIS_MOTHER, CONDITION_ALWAYS, MSGNO(MsgFieldTaviWolfedDownHisSnackAnd)},
    {TRIGGER_TALK, ACTOR_OLD_MAN_ON_MADRA, CONDITION_ALWAYS, EVENT(DailaRooms_AskGoingToMadra)},
    {TRIGGER_TALK, ACTOR_OLD_WOMAN_ON_THE_KANDOREAN_TEMPLE, CONDITION_ALWAYS, MSGNO(MsgFieldIThinkMadraIsEastOf)},
    {TRIGGER_TALK, ACTOR_MAYOR, CONDITION_ALWAYS, EVENT(DailaRooms_AskLookingForBoat)},
    {TRIGGER_TALK, ACTOR_WOMAN_ON_THE_NOISE, CONDITION_ALWAYS, MSGNO(MsgFieldItsTooDangerousToGoTo)},
    {TRIGGER_TALK, ACTOR_MAN_ON_THE_RUMBLING, CONDITION_ALWAYS, MSGNO(MsgFieldWhoWasThatGuyIWonder)},
    {TRIGGER_TALK, ACTOR_WOMAN_ON_DAILAS_LUCK, CONDITION_ALWAYS, MSGNO(MsgFieldWhyDidThePirateBriggsPlague)},
    {TRIGGER_TALK, ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS, EVENT(DailaRooms_TalkToWeaponMerchantOnceTaviIsRescued)},
    {TRIGGER_TALK, ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS, EVENT(DailaRooms_TalkToArmorMerchantOnceTaviIsRescued)},
    {TRIGGER_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS, EVENT(DailaRooms_TalkToInnkeeperOnceTaviIsRescued)},
    {TRIGGER_TALK, ACTOR_INN_WORKER, CONDITION_ALWAYS, MSGNO(MsgDailaInteriorsDinnerShowPractice)},
    {TRIGGER_TALK, ACTOR_TRAVELER_FROM_MADRA, CONDITION_ALWAYS, MSGNO(MsgDailaInteriorsSeafoodWhen)},
    {TRIGGER_TALK, ACTOR_GUEST_ON_MADRAS_SEAFOOD, CONDITION_ALWAYS, MSGNO(MsgDailaInteriorsVegetablesAgain)},
    {TRIGGER_TALK, ACTOR_RIKI, CONDITION_ALWAYS, MSGNO(MsgFieldMmmMmnphDadWasSoMad)},
    {TRIGGER_TALK, ACTOR_TAVI, CONDITION_ALWAYS, MSGNO(MsgFieldMmmphMmmmDontTellAnyoneWe)},
    {TRIGGER_TALK, ACTOR_PRACTICING_PERFORMER, CONDITION_ALWAYS, EVENT(DailaRooms_TalkCaughtPracticing)},
    {TRIGGER_TALK, ACTOR_WOMAN_ON_THE_CURRENTS_AFTER_RESCUE, CONDITION_ALWAYS, MSGNO(MsgSeaOfTimeCurrents)},
    {TRIGGER_TALK, ACTOR_OLD_MAN_ON_THE_RED_ROCKS_AFTER_RESCUE, CONDITION_ALWAYS, MSGNO(MsgSeaOfTimeRedRocks)},
    {TRIGGER_TALK, ACTOR_OLD_WOMAN_ON_HER_SONS_AFTER_RESCUE, CONDITION_ALWAYS, MSGNO(MsgSeaOfTimeThreeGenerations)},
    {TRIGGER_TALK, ACTOR_ARMOR_COUNTER_AFTER_RESCUE, CONDITION_ALWAYS, EVENT(DailaRooms_TalkToArmorMerchant)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_RIKIS_FATHER, CONDITION_ALWAYS, MSGNO(MsgFieldDarnThatRikiAhImGlad)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_RIKIS_MOTHER, CONDITION_ALWAYS, MSGNO(MsgFieldHeeHeeMyRikiIsMommys)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TAVIS_FATHER, CONDITION_ALWAYS, MSGNO(MsgFieldIGuessRikiNextDoorMade)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TAVIS_MOTHER, CONDITION_ALWAYS, MSGNO(MsgFieldOnceTaviFinishesEatingImPutting)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ON_MADRA, CONDITION_ALWAYS, MSGNO(MsgFieldDailaGotHitPrettyHardI)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_ON_THE_KANDOREAN_TEMPLE, CONDITION_ALWAYS, MSGNO(MsgFieldSomeoneShouldSeeHowTheRoad)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAYOR, CONDITION_ALWAYS, MSGNO(MsgFieldHeSaidHedPayMeA)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_THE_NOISE, CONDITION_ALWAYS, MSGNO(MsgFieldTheRoadToMadraGoesThrough)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_THE_RUMBLING, CONDITION_ALWAYS, MSGNO(MsgFieldMaybeThatGuyWashedUpOn)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_DAILAS_LUCK, CONDITION_ALWAYS, MSGNO(MsgFieldBriggsProbablyHasLotsOfMen)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgFieldThisTownMayBeDirtPoor)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgDailaInteriorsMazeTreasure)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, CONDITION_ALWAYS, MSGNO(MsgDailaInteriorsTiredPerformer)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INN_WORKER, CONDITION_ALWAYS, MSGNO(MsgDailaInteriorsDragonKingLine)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TRAVELER_FROM_MADRA, CONDITION_ALWAYS, MSGNO(MsgDailaInteriorsBadShows)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GUEST_ON_MADRAS_SEAFOOD, CONDITION_ALWAYS, MSGNO(MsgDailaInteriorsGuestDance)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_RIKI, CONDITION_ALWAYS, MSGNO(MsgFieldIStillWantToCatchThat)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TAVI, CONDITION_ALWAYS, MSGNO(MsgFieldImSureGladIMadeIt)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PRACTICING_PERFORMER, CONDITION_ALWAYS, MSGNO(MsgDailaInteriorsSeafoodStory)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_THE_CURRENTS_AFTER_RESCUE, CONDITION_ALWAYS, MSGNO(MsgSeaOfTimeHusbandDizzy)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ON_THE_RED_ROCKS_AFTER_RESCUE, CONDITION_ALWAYS, MSGNO(MsgSeaOfTimeYeppToldMe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_ON_HER_SONS_AFTER_RESCUE, CONDITION_ALWAYS, MSGNO(MsgSeaOfTimeGiveUp)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARMOR_COUNTER_AFTER_RESCUE, CONDITION_ALWAYS, EVENT(DailaRooms_ReadArmorMerchantsMindAfterRescue)},
    {TRIGGER_TABLE_END},
};

/// The sanctuary once Tavi is rescued: Frost on the three puddles, the jumps
/// between the pillars and the Tear at their top.
struct Trigger gDailaRooms_SanctumEventsOnceTaviIsRescued[] = {
    {TRIGGER_EXIT, DAILA_ROOMS_SANCTUM_TO_DAILA, CONDITION_ALWAYS, DAILA_ROOMS_SANCTUM_TO_DAILA},
    // Unused: the sanctuary has no cells for exits 2 to 6, nor exits of those numbers.
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_NORTH_OF_PUDDLE, CONDITION_ALWAYS, EVENT(DailaRooms_JumpForward)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), CELL_WEST_OF_PUDDLE, CONDITION_ALWAYS, EVENT(DailaRooms_JumpForward)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), CELL_EAST_OF_PUDDLE, CONDITION_ALWAYS, EVENT(DailaRooms_JumpForward)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_SOUTH_OF_PUDDLE, CONDITION_ALWAYS, EVENT(DailaRooms_JumpForward)},
    {TRIGGER_TALK, ACTOR_HEALER, CONDITION_ALWAYS, EVENT(DailaRooms_TalkToHealerOnceTaviIsRescued)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, CONDITION_ALWAYS, MSGNO(MsgDailaInteriorsBoringVillage)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_FIRST_PUDDLE, FLAG_DAILA_SANCTUARY_PUDDLE_FROZEN_FIRST + 0, EVENT(Scene_FreezePuddleIntoPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_SECOND_PUDDLE, FLAG_DAILA_SANCTUARY_PUDDLE_FROZEN_FIRST + 1, EVENT(Scene_FreezePuddleIntoPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_THIRD_PUDDLE, FLAG_DAILA_SANCTUARY_PUDDLE_FROZEN_FIRST + 2, EVENT(Scene_FreezePuddleIntoPillar)},
    {TRIGGER_EXAMINE, CELL_SEA_GODS_TEAR, FLAG_SEA_GODS_TEAR_TAKEN, EVENT(DailaRooms_TakeSeaGodsTear)},
    {TRIGGER_TABLE_END},
};
