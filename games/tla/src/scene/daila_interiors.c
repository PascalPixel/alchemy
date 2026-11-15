/// @file
/// Daila's interiors, in two scenes that share this overlay.
///
/// DailaInteriorsHouses is the village's houses, shops and inn, with other
/// tables once Tavi is rescued (FLAG_TAVI_RESCUED). DailaInteriorsSanctum is
/// the sanctuary of the Sea God, where Frost raises three puddles into pillars
/// of ice (Scene_RaiseHeldActor, gDailaInteriors_PillarFrozenFlags) up to the
/// Sea God's Tear (DailaInteriors_TakeSeaGodsTear), and where, once Tavi is
/// rescued, the drained floor opens the way on (Scene_Initialize).

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
#include "field/field_sequence_event.h"
#include "game/inventory.h"
#include "game/game_bank.h"
#include "scene/scene.h"
#include "scene/shared.h"
#include "window/window.h"
#include "game/item.h"
#include "version.h"
#include "text/msg_ids.h"

void Map_CopyCellsRect(s32 srcX, s32 srcY, s32 destX, s32 destY, s32 width,
                       s32 height);

/// Reads a game flag.
static inline s32 Daila_FlagCheck(s32 flag) { return GameFlag_Get(flag); }

/// The houses, shops and inn of DailaInteriorsHouses, numbering the rows its
/// two placement tables share.
enum DailaInteriorsActorId {
  /// Riki's parents, in the house by entrance 1, scolding the boy who took his
  /// father's fishing pole.
  ACTOR_RIKIS_FATHER = ACTOR_FIRST_PLACED,
  ACTOR_RIKIS_MOTHER,
  /// Tavi's parents, next door, waiting for him with his three o'clock snack.
  ACTOR_TAVIS_FATHER,
  ACTOR_TAVIS_MOTHER,
  /// An old man who sends boat seekers to Madra.
  ACTOR_OLD_MAN_ON_MADRA,
  /// An old woman who tells the way to the Kandorean Temple.
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
  /// The innkeeper's one helper, who later practises for the dinner show.
  ACTOR_INN_WORKER,
  /// A guest of the inn, a man carrying a pack, who misses Madra's food.
  ACTOR_TRAVELER_FROM_MADRA,
  /// A guest of the inn who longs for Madra's seafood.
  ACTOR_GUEST_ON_MADRAS_SEAFOOD,
  /// Until Tavi is rescued: the family in the house by entrance 7, whose sons
  /// try the Sea of Time.
  ACTOR_WOMAN_ON_THE_CURRENTS,
  ACTOR_OLD_MAN_ON_THE_RED_ROCKS,
  ACTOR_OLD_WOMAN_ON_HER_SONS,
  /// A blank actor on the armor shop's counter, so the merchant behind it
  /// answers from across it.
  ACTOR_ARMOR_COUNTER
};

/// The rows of gDailaInteriors1PlacementsOnceTaviIsRescued after those it
/// shares with gDailaInteriors1Placements.
enum DailaInteriorsActorIdAfterRescue {
  /// The two boys, back home, with their mouths full.
  ACTOR_RIKI = ACTOR_GUEST_ON_MADRAS_SEAFOOD + 1,
  ACTOR_TAVI,
  /// A woman of the inn caught practising for the dinner show.
  ACTOR_PRACTISING_PERFORMER,
  ACTOR_WOMAN_ON_THE_CURRENTS_AFTER_RESCUE,
  ACTOR_OLD_MAN_ON_THE_RED_ROCKS_AFTER_RESCUE,
  ACTOR_OLD_WOMAN_ON_HER_SONS_AFTER_RESCUE,
  ACTOR_ARMOR_COUNTER_AFTER_RESCUE
};

/// The shops of Daila, as Shop_Run numbers them.
enum { SHOP_DAILA_WEAPONS = 1, SHOP_DAILA_ARMOR = 2 };

/// Daila's inn, as Inn_CheckIn numbers the inns.
enum { DAILA_INN = 0 };

/// The sanctuary's actors, numbering the rows of gDailaInteriors2Placements.
enum DailaSanctuaryActorId {
  /// The sanctuary's healer, who sends travelers with nowhere to go to Poi at
  /// the Kandorean Temple, and once that is done heals the party.
  ACTOR_HEALER = ACTOR_FIRST_PLACED,
  /// An actor with no picture at the pillars' top that holds the Tear: the
  /// game shows the item on it (Actor_SetItemIcon) until it is taken.
  ACTOR_SEA_GODS_TEAR,
  ACTOR_FIRST_PILLAR,
  ACTOR_SECOND_PILLAR,
  ACTOR_THIRD_PILLAR,
  /// An actor with no picture, put where the Tear lay once it is taken.
  ACTOR_TEAR_SPOT
};

/// The first of three flags, one for each of the sanctuary's puddles Frost has
/// frozen into a pillar for this visit, by gDailaInteriors_PillarFrozenFlags.
#define FLAG_DAILA_SANCTUARY_PILLAR_FROZEN_FIRST 0x220
/// Holds which of his two spots the practising man has jumped to last.
#define FLAG_DAILA_PRACTICE_AT_DOOR 0x200

extern s32 gDailaInteriorsExitsOnceTaviIsRescued[];
extern struct ActorPlacement gDailaInteriors1Placements[];
extern struct ActorPlacement gDailaInteriors2Placements[];
extern struct ActorPlacement gDailaInteriors1PlacementsOnceTaviIsRescued[];
extern struct Trigger gDailaInteriors2Events[];
extern struct Trigger gDailaInteriors1EventsOnceTaviIsRescued[];
extern struct Trigger gDailaInteriors2EventsOnceTaviIsRescued[];

void Sanctum_Run(s32 actor);

extern s16 gDailaInteriors_PillarFrozenFlags[];

void FlagBlock_ApplyFlags(const s16 *flags);

s32 DailaInteriors_GiveItem(s32 item, s32 mode);
void Scene_FadeInAndClearFlag(void);

void FieldEffect_StartBlankOverlay(s32 mode);

void DailaInteriors_TalkToArmorMerchantOnceTaviIsRescued(s32 a0);

void DailaInteriors_AskGoingToMadra(void);
void DailaInteriors_TalkCaughtPracticing(void);

void Scene_RaiseHeldActor(void);

void DailaInteriors_TakeSeaGodsTear(void);

s32 FieldYesNo_Ask(s32 mode);
void Field_TryJumpForward(void);

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

extern s32 gDailaInteriorsExits[];

const void *Scene_GetExits(void) {
  if (GameFlag_Get(FLAG_TAVI_RESCUED))
    return gDailaInteriorsExitsOnceTaviIsRescued;
  return gDailaInteriorsExits;
}

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  if (gMainRoutineWork.next == SCENE_DAILA_INTERIORS_SANCTUM)
    return gDailaInteriors2Placements;
  if (GameFlag_Get(FLAG_TAVI_RESCUED))
    return gDailaInteriors1PlacementsOnceTaviIsRescued;
  return gDailaInteriors1Placements;
}

/// Unused: does nothing.
UNUSED void DailaInteriors_EmptyHook(void) {}

/// Unused: the practising man shouts and jumps to the other of his two spots.
UNUSED void DailaInteriors_PracticeShout(s32 unused, s32 actor) {
  FieldActor_ShowEmote(actor, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 50);
  Actor_Jump(actor, 6, 15);
  Actor_Jump(actor, 6, 15);
  FieldEvent_Wait(10);
  if (!Daila_FlagCheck(FLAG_DAILA_PRACTICE_AT_DOOR)) {
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

/// Unused: the practising man turns to the leader, startled.
UNUSED void DailaInteriors_StartleAtTheLeader(s32 unused, s32 actor) {
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

/// Mind Read on the armor shop's counter before Tavi is rescued: the merchant
/// behind it thinks of the two scamps.
s32 DailaInteriors_ReadArmorMerchantsMind(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgDailaInteriorsScamps);
  // "If those two scamps knew there were travelers in town, they'd have a..."
  Event_ShowMessageAndWait(ACTOR_ARMOR_MERCHANT, 0);
  Event_End();
  return 0;
}

/// Mind Read on the armor shop's counter once Tavi is rescued: the merchant
/// behind it thinks of hidden treasure.
s32 DailaInteriors_ReadArmorMerchantsMindAfterRescue(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgDailaInteriorsMazeTreasure);
  // "Maybe they hid treasure here because this town is so mazelike..."
  Event_ShowMessageAndWait(ACTOR_ARMOR_MERCHANT, 0);
  Event_End();
  return 0;
}

/// Unused: starts an event and draws the leader in front of the counters.
UNUSED void DailaInteriors_StartLeaderEvent(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_SetSpritePriority(gMainRoutineWork.leaderActorId, 1);
}

extern struct Trigger gDailaInteriors1Events[];

const struct Trigger *Scene_GetTriggers(void) {
  s32 flag = GameFlag_Get(FLAG_TAVI_RESCUED);
  struct MainRoutineWork *party = &gMainRoutineWork;
  s32 id = SCENE_DAILA_INTERIORS_SANCTUM;

  if (flag) {
    if (party->next == id)
      return gDailaInteriors2EventsOnceTaviIsRescued;
    return gDailaInteriors1EventsOnceTaviIsRescued;
  }
  if (party->next == id)
    return gDailaInteriors2Events;
  return gDailaInteriors1Events;
}

struct Actor *Actor_GetById(u32 objectId);
s32 Inn_CheckIn(s32 mode, s32 objectId);

/// The weapon merchant sells across his counter, and otherwise warns of the
/// roads south.
void DailaInteriors_TalkToWeaponMerchant(s32 actor) {
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

/// The armor merchant sells across his counter, and otherwise asks what brings
/// the party to Daila.
void DailaInteriors_TalkToArmorMerchant(s32 actor) {
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

/// The innkeeper takes guests across the counter to the north, and otherwise
/// wonders what to cook.
void DailaInteriors_TalkToInnkeeper(s32 actor) {
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

/// The sanctuary's healer, until he has sent the party on, asks whether they
/// have time and nowhere to go, whatever they answer, and remembers it
/// (FLAG_DAILA_HEALER_SENT_TO_POI); after that he heals across his counter.
void DailaInteriors_TalkToHealer(s32 actor) {
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

/// The sanctuary's healer once Tavi is rescued: he heals across his counter,
/// and otherwise sends the party to Madra.
void DailaInteriors_TalkToHealerOnceTaviIsRescued(s32 actor) {
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

/// As DailaInteriors_TalkToWeaponMerchant, telling of Daila's pirate past.
void DailaInteriors_TalkToWeaponMerchantOnceTaviIsRescued(s32 actor) {
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

/// As DailaInteriors_TalkToArmorMerchant, on how spacious the village is.
void DailaInteriors_TalkToArmorMerchantOnceTaviIsRescued(s32 actor) {
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

/// As DailaInteriors_TalkToInnkeeper, wondering when the boats will be mended.
void DailaInteriors_TalkToInnkeeperOnceTaviIsRescued(s32 actor) {
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

/// The old man asks whether the party is going to Madra once Tavi is rescued.
void DailaInteriors_AskGoingToMadra(void) {
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

/// The mayor asks whether the party is looking for a boat too.
void DailaInteriors_TalkNoBoats(void) {
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

/// The performer caught practising for the dinner show hopes no one was
/// watching.
void DailaInteriors_TalkCaughtPracticing(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgDailaInteriorsWatchingPractice);
  // "Er... Aheh heh... You... weren't watching, were you?"
  Event_ShowMessageAndWait(ACTOR_PRACTISING_PERFORMER, 0);
  FieldEvent_Wait(30);
  Actor_SetAnimationById(ACTOR_PRACTISING_PERFORMER, 3);
  FieldEvent_Wait(30);
  // "I was just practicing for the show we're putting on tonight."
  Event_ShowMessageAndWait(ACTOR_PRACTISING_PERFORMER, 0);
  Event_End();
}

#include "shared/scene_raise_held_actor.h"

void ItemDisplay_ShowPicture(struct Actor *object, s32 mode);
void Sound_Play(s32 cue);
void Actor_StartIdle(struct Actor *object);

/// Takes the Sea God's Tear from the top of the pillars, or, when the bags are
/// full and nothing is dropped for it, puts it back.
void DailaInteriors_TakeSeaGodsTear(void) {
  s32 saved48;
  s32 saved28;
  s32 savedY;
  s32 item;

  saved48 = Actor_GetById(ACTOR_SEA_GODS_TEAR)->gravity;
  item = ITEM_SEA_GODS_TEAR;
  saved28 = Actor_GetById(ACTOR_SEA_GODS_TEAR)->velocityY;
  savedY = Actor_GetById(ACTOR_SEA_GODS_TEAR)->y.fixed;
  Event_Start();
  Event_ResolvePendingActions(0);
  if (PartyInventory_FindRoom(item) >= 0) {
    ItemDisplay_ShowPicture(Actor_GetById(ACTOR_SEA_GODS_TEAR), 3);
    Sound_Play(MUS_TREASURE_FOUND);
    MessageArgument_Push(item, MESSAGE_ARGUMENT_ITEM);
    // "Felix got ...."
    FieldMessage_Show((s32)MsgItemReceived, 1);
    PartyInventory_Add(item);
    Actor_SetPosition(ACTOR_SEA_GODS_TEAR, 0, 0);
    Actor_SetPosition(ACTOR_TEAR_SPOT, 232, 194);
    GameFlag_Set(FLAG_SEA_GODS_TEAR_TAKEN);
  } else {
    ItemDisplay_ShowPicture(Actor_GetById(ACTOR_SEA_GODS_TEAR), 3);
    Sound_Play(MUS_TREASURE_FOUND);
    if (DailaInteriors_GiveItem(item, 0) != -1) {
      Actor_SetPosition(ACTOR_SEA_GODS_TEAR, 0, 0);
      Actor_SetPosition(ACTOR_TEAR_SPOT, 232, 196);
      GameFlag_Set(FLAG_SEA_GODS_TEAR_TAKEN);
    } else {
      Actor_SetPosition(ACTOR_SEA_GODS_TEAR, 0, 0);
      FieldEvent_Wait(1);
      Actor_GetById(ACTOR_SEA_GODS_TEAR)->velocityY = saved28;
      Actor_GetById(ACTOR_SEA_GODS_TEAR)->y.fixed = savedY;
      Actor_GetById(ACTOR_SEA_GODS_TEAR)->gravity = saved48;
      Actor_GetById(ACTOR_SEA_GODS_TEAR)->update = NULL;
      Actor_StartIdle(Actor_GetById(ACTOR_SEA_GODS_TEAR));
      Actor_SetSpritePriority(ACTOR_SEA_GODS_TEAR, 1);
      Actor_GetById(ACTOR_SEA_GODS_TEAR)->motionFlags = 2;
      Actor_GetById(ACTOR_SEA_GODS_TEAR)->scaleY = 0x13333;
      Actor_GetById(ACTOR_SEA_GODS_TEAR)->scaleX = 0x13333;
      Actor_GetById(ACTOR_SEA_GODS_TEAR)->collisionFlags |= 4;
      Actor_SetPosition(ACTOR_SEA_GODS_TEAR, 232, 196);
    }
  }
  Event_End();
}

struct ItemDefinition *Item_GetDirect(s32 item);

/// Gives an item to the party, asking what to drop when no one has room for
/// it; returns who took it, or -1.
s32 DailaInteriors_GiveItem(s32 item, s32 unused) {
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
    FieldMessage_Show((s32)MsgItemReceived, 1);
    // "But you can't carry any more items."
    FieldMessage_Show((s32)MsgItemCannotCarry, 1);
  retry:
    // "What will you drop?"
    FieldMessage_Show((s32)MsgItemDropPrompt, 1);
    result = ItemPicker_RunToDrop(&member, &slot);
    if (result == -1) {
      if (Item_GetDirect(item)->flags & 8) {
        MessageArgument_Push(item, MESSAGE_ARGUMENT_ITEM);
        // "That ...'s valuable! You must get it!"
        FieldMessage_Show((s32)MsgItemDropPrompt + 4, 1);
        goto retry;
      }
      MessageArgument_Push(item, MESSAGE_ARGUMENT_ITEM);
      // "Drop the ...?"
      FieldMessage_Show((s32)MsgItemDropPrompt + 1, 5);
      result = FieldYesNo_Ask(1);
      Message_CloseFinishedWindows();
      if (result != 0)
        goto retry;
      MessageArgument_Push(item, MESSAGE_ARGUMENT_ITEM);
      // "Felix drops the ...."
      FieldMessage_Show((s32)MsgItemDropPrompt + 2, 1);
      work->message = saved;
      return owner;
    } else {
      Unit_Get(member);
      count = Inventory_GetQuantity(member, slot);
      if (Inventory_CountItem(member, item) > 29) {
        MessageArgument_Push(member, MESSAGE_ARGUMENT_ACTOR);
        MessageArgument_Push(item, MESSAGE_ARGUMENT_ITEM);
        // "Empty"
        FieldMessage_Show((s32)MsgItemDropPrompt + 7, 1);
        goto retry;
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
        FieldMessage_Show((s32)MsgItemReceived, 3);
      } else {
        MessageArgument_Push(item, MESSAGE_ARGUMENT_ITEM);
        MessageArgument_Push(owner, MESSAGE_ARGUMENT_ACTOR);
        // "Felix gave ... to ...."
        FieldMessage_Show((s32)MsgItemGiven, 3);
      }
      work->message = saved;
      return owner;
    }
  } else {
    Sound_Play(MUS_TREASURE_FOUND);
    MessageArgument_Push(item, MESSAGE_ARGUMENT_ITEM);
    // "Felix got ...."
    FieldMessage_Show((s32)MsgItemReceived, 3);
    if (owner != gMainRoutineWork.leaderActorId) {
      MessageArgument_Push(item, MESSAGE_ARGUMENT_ITEM);
      MessageArgument_Push(owner, MESSAGE_ARGUMENT_ACTOR);
      // "Felix gave ... to ...."
      FieldMessage_Show((s32)MsgItemReceived + 1, 1);
    }
    work->message = saved;
  }
  return owner;
}

void TilesetAnim_Pause(s32 mode);

void Actor_SetItemIcon(struct Actor *object, s32 item);
void FlagBlock_PlaceAll(const s16 *flags);

/// Sets up the houses (the boys eating once they are home, the armor merchant
/// behind his counter) or the sanctuary (the Tear on the pillars' top until it
/// is taken, the puddles until Tavi is rescued, the frozen pillars after).
s32 Scene_Initialize(void) {
  struct DailaMapWork *map;
  s32 *effect = &gFieldWork->transition;
  s32 value = 0x209;

  *effect = value;
  if (gMainRoutineWork.next == SCENE_DAILA_INTERIORS_HOUSES) {
    if (gMainRoutineWork.entrance == 7) {
      Scene_FadeInAndClearFlag();
    } else {
      if (GameFlag_Get(FLAG_TAVI_RESCUED)) {
        Actor_SetAnimationById(ACTOR_RIKI, 5);
        Actor_SetSpriteFlags(Actor_GetById(ACTOR_RIKI), 0);
        Actor_SetAnimationById(ACTOR_TAVI, 5);
        Actor_SetSpriteFlags(Actor_GetById(ACTOR_TAVI), 0);
      }
      Actor_GetById(gMainRoutineWork.leaderActorId)->priorityFlags |= 32;
      Actor_GetById(ACTOR_TEAR_SPOT)->priorityFlags |= 32;
      Actor_GetById(ACTOR_ARMOR_MERCHANT)->priorityFlags |= 32;
      Actor_SetSpritePriority(ACTOR_ARMOR_MERCHANT, 2);
      Actor_GetById(ACTOR_ARMOR_MERCHANT)->collisionFlags |= 4;
    }
  } else if (gMainRoutineWork.next == SCENE_DAILA_INTERIORS_SANCTUM) {
    map = (struct DailaMapWork *)Ram_HeapSlots->mapGrid;
    Event_Start();
    Event_ResolvePendingActions(0);
    Actor_SetSpriteFlags(Actor_GetById(ACTOR_TEAR_SPOT), 0);
    FieldEffect_StartBlankOverlay(0);
    if (!GameFlag_Get(FLAG_SEA_GODS_TEAR_TAKEN)) {
      Actor_SetItemIcon(Actor_GetById(ACTOR_SEA_GODS_TEAR), ITEM_SEA_GODS_TEAR);
      Actor_SetSpriteFlags(Actor_GetById(ACTOR_SEA_GODS_TEAR), 0);
      Actor_SetSpritePriority(ACTOR_SEA_GODS_TEAR, 1);
      Actor_GetById(ACTOR_SEA_GODS_TEAR)->scaleY = 0x13333;
      Actor_GetById(ACTOR_SEA_GODS_TEAR)->scaleX = 0x13333;
      Actor_GetById(ACTOR_SEA_GODS_TEAR)->collisionFlags |= 4;
      Actor_GetById(ACTOR_TEAR_SPOT)->collisionFlags |= 4;
    } else {
      Actor_GetById(ACTOR_SEA_GODS_TEAR)->collisionFlags |= 4;
      Actor_SetSpriteFlags(Actor_GetById(ACTOR_SEA_GODS_TEAR), 0);
    }
    if (!GameFlag_Get(FLAG_TAVI_RESCUED)) {
      Actor_SetPosition(ACTOR_FIRST_PILLAR, 0, 0);
      Actor_SetPosition(ACTOR_SECOND_PILLAR, 0, 0);
      Actor_SetPosition(ACTOR_THIRD_PILLAR, 0, 0);
      map->mode17 = 1;
    } else {
      FlagBlock_PlaceAll(gDailaInteriors_PillarFrozenFlags);
    }
    Event_End();
  }
  return 0;
}

s32 Scene_PrepareMap(void) {
  if (gMainRoutineWork.next == SCENE_DAILA_INTERIORS_SANCTUM &&
      GameFlag_Get(FLAG_TAVI_RESCUED)) {
    TilesetAnim_Pause(0);
    MapGrid_CopyCells(29, 33, 10, 6, 10, 12);
  }
  return 0;
}

#include "shared/scene_fade_in_and_clear_flag.h"

/// The step triggers of the cells to jump from, one for each heading.
void DailaInteriors_JumpForward(void) { Field_TryJumpForward(); }

// clang-format off
s16 gDailaInteriors_PillarFrozenFlags[] = {
    ACTOR_FIRST_PILLAR,  FLAG_DAILA_SANCTUARY_PILLAR_FROZEN_FIRST + 0,
    ACTOR_SECOND_PILLAR, FLAG_DAILA_SANCTUARY_PILLAR_FROZEN_FIRST + 1,
    ACTOR_THIRD_PILLAR,  FLAG_DAILA_SANCTUARY_PILLAR_FROZEN_FIRST + 2,
    -1, 0,
};

/// Tavi's father's script: he paces his house back and forth.
s32 gDailaInteriors_PacingScript[] = {
    22, 30,
#if !TLA_PATCH_1
    131,
#else
    129,
#endif
    22, 16, 0x1999, 22, 15, 0x4ccc,
    (s32)0x80010000, 39, 2, 4, 0x1380000, 0,
    0xa80000, 1, 0, 20, 39, 1,
    22, 3, 0, 0, 20, 39,
    2, 4, 0x1880000, 0, 0xa80000, 1,
    0, 20, 39, 1, 22, 3,
    0, 0, 20, 13, 0xffff, (s32)0xc0010000,
    17,
};

/// The inn's two performers' script once Tavi is rescued: they turn on the
/// spot, rehearsing the dinner show.
s32 gDailaInteriors_LookAroundScript[] = {
    22, 30, 129, 22, 16, 0x1999,
    22, 15, 0x4ccc, (s32)0x80010000, 39, 1,
    22, 3, 0x6000, 0, 5, 22,
    3, 0x8000, 0, 5, 22, 3,
    0xa000, 0, 5, 22, 3, 0xc000,
    0, 5, 22, 3, 0, 0,
    5, 22, 3, 0x2000, 0, 5,
    22, 3, 0x4000, 0, 5, 22,
    3, 0x6000, 0, 5, 22, 3,
    0x8000, 0, 5, 22, 3, 0xa000,
    0, 5, 22, 3, 0xc000, 0,
    5, 22, 3, 0, 0, 5,
    22, 3, 0x2000, 0, 5, 22,
    3, 0x4000, 0, 5, 22, 3,
    0x6000, 0, 5, 22, 3, 0x8000,
    0, 5, 22, 3, 0xa000, 0,
    5, 22, 3, 0xc000, 0, 5,
    22, 3, 0, 0, 5, 22,
    3, 0x2000, 0, 5, 22, 3,
    0x4000, 0, 5, 22, 6, 0x80000,
    0, 30, 22, 6, 0x80000, 0,
    30, 13, 0xffff, (s32)0xc0010000, 17, 22,
    30,
#if !TLA_PATCH_1
    131,
#else
    129,
#endif
    22, 16, 0x1999, 22, 15, 0x4ccc,
    (s32)0x80010000, 39, 1, 39, 2, 10,
    0x20000, 0x20000, 0x60000, 1, 0, 4,
    13, 0xffff, (s32)0xc0010000, 17,
};
// clang-format on

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

// clang-format off
/// Where the houses' doors lead, before Tavi is rescued, out into Daila1.
s32 gDailaInteriorsExits[] = {
    SCENE_EXITS(DailaInteriorsHouses),
    SCENE_EXIT_UNLESS(1, Daila1, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, Daila1, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, Daila1, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, Daila1, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(5, Daila1, 5, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(6, Daila1, 6, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(7, Islets3, 3, EXIT_FLAG_NONE),
    SCENE_EXITS(DailaInteriorsSanctum),
    SCENE_EXIT_UNLESS(1, Daila1, 7, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

/// Where the houses' doors lead once Tavi is rescued, out into Daila2.
s32 gDailaInteriorsExitsOnceTaviIsRescued[] = {
    SCENE_EXITS(DailaInteriorsHouses),
    SCENE_EXIT_UNLESS(1, Daila2, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, Daila2, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, Daila2, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, Daila2, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(5, Daila2, 5, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(6, Daila2, 6, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(7, Islets3, 3, EXIT_FLAG_NONE),
    SCENE_EXITS(DailaInteriorsSanctum),
    SCENE_EXIT_UNLESS(1, Daila2, 7, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

/// The houses, shops and inn before Tavi is rescued (enum
/// DailaInteriorsActorId).
struct ActorPlacement gDailaInteriors1Placements[] = {
    ACTOR_PLACEMENT(115, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 104, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(116, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 88, 0, 88, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(114, CONDITION_ALWAYS, (s32)gDailaInteriors_PacingScript, 360, 0, 168, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(128, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 72, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(112, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 632, 0, 168, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(113, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 568, 0, 88, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(114, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 160, 0, 360, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(128, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 456, FACING_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(115, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 440, FACING_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(116, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 216, 0, 472, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(108, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 712, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(109, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 88, 0, 724, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(134, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 456, 0, 468, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(135, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 536, 0, 456, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(238, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 440, 0, 376, FACING_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(116, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 424, 0, 360, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(74, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 472, 0, 746, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(75, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 568, 0, 712, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(76, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 504, 0, 712, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 88, 0, 728, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

/// The sanctuary (enum DailaSanctuaryActorId).
struct ActorPlacement gDailaInteriors2Placements[] = {
    ACTOR_PLACEMENT(201, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 333, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 196, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(290, FLAG_SCENE_TEMP_23, MOVEMENT_TYPE_STAND, 184, 0, 248, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(290, FLAG_SCENE_TEMP_24, MOVEMENT_TYPE_STAND, 216, 0, 248, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(290, FLAG_SCENE_TEMP_25, MOVEMENT_TYPE_STAND, 248, 0, 232, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

/// The houses, shops and inn once Tavi is rescued, with Riki and Tavi home.
struct ActorPlacement gDailaInteriors1PlacementsOnceTaviIsRescued[] = {
    ACTOR_PLACEMENT(115, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 176, 0, 120, FACING_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(116, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 88, 0, 88, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(114, CONDITION_ALWAYS, (s32)gDailaInteriors_PacingScript, 360, 0, 168, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(128, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 72, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(112, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 632, 0, 168, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(113, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 568, 0, 88, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(114, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 160, 0, 360, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(128, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 88, 0, 440, FACING_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(115, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 488, FACING_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(116, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 216, 0, 472, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(108, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 712, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(109, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 88, 0, 724, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(134, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 456, 0, 468, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(135, CONDITION_ALWAYS, (s32)gDailaInteriors_LookAroundScript, 424, 0, 520, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(238, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 440, 0, 376, FACING_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(116, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 424, 0, 360, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(18, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 161, 0, 111, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(17, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 112, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(116, CONDITION_ALWAYS, (s32)gDailaInteriors_LookAroundScript, 456, 0, 520, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(74, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 472, 0, 746, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(75, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 568, 0, 712, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(76, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 504, 0, 712, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 88, 0, 728, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

/// The houses' doors, talk and Mind Read before Tavi is rescued.
struct Trigger gDailaInteriors1Events[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
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
    {TRIGGER_TALK, ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS, EVENT(DailaInteriors_TalkToWeaponMerchant)},
    {TRIGGER_TALK, ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS, EVENT(DailaInteriors_TalkToArmorMerchant)},
    {TRIGGER_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS, EVENT(DailaInteriors_TalkToInnkeeper)},
    {TRIGGER_TALK, ACTOR_INN_WORKER, CONDITION_ALWAYS, MSGNO(MsgFieldWeRunAVerySmallInn)},
    {TRIGGER_TALK, ACTOR_TRAVELER_FROM_MADRA, CONDITION_ALWAYS, MSGNO(MsgFieldIDontKnowWhenIllBe)},
    {TRIGGER_TALK, ACTOR_GUEST_ON_MADRAS_SEAFOOD, CONDITION_ALWAYS, MSGNO(MsgFieldIHearTheSeafoodInMadra)},
    {TRIGGER_TALK, ACTOR_WOMAN_ON_THE_CURRENTS, CONDITION_ALWAYS, MSGNO(MsgSeaOfTimeCurrents)},
    {TRIGGER_TALK, ACTOR_OLD_MAN_ON_THE_RED_ROCKS, CONDITION_ALWAYS, MSGNO(MsgSeaOfTimeRedRocks)},
    {TRIGGER_TALK, ACTOR_OLD_WOMAN_ON_HER_SONS, CONDITION_ALWAYS, MSGNO(MsgSeaOfTimeThreeGenerations)},
    {TRIGGER_TALK, ACTOR_ARMOR_COUNTER, CONDITION_ALWAYS, EVENT(DailaInteriors_TalkToArmorMerchant)},
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
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARMOR_COUNTER, CONDITION_ALWAYS, EVENT(DailaInteriors_ReadArmorMerchantsMind)},
    {TRIGGER_TABLE_END},
};

/// The sanctuary's doors, talk and Mind Read before Tavi is rescued.
struct Trigger gDailaInteriors2Events[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_TALK, ACTOR_HEALER, CONDITION_ALWAYS, EVENT(DailaInteriors_TalkToHealer)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, CONDITION_ALWAYS, MSGNO(MsgFieldTravelersWithNowhereToGoThis)},
    {TRIGGER_TABLE_END},
};

/// The houses' doors, talk and Mind Read once Tavi is rescued.
struct Trigger gDailaInteriors1EventsOnceTaviIsRescued[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_TALK, ACTOR_RIKIS_FATHER, CONDITION_ALWAYS, MSGNO(MsgFieldLetMeTellYouKidsAre)},
    {TRIGGER_TALK, ACTOR_RIKIS_MOTHER, CONDITION_ALWAYS, MSGNO(MsgFieldWhoStaysOutsideUntilHesAbsolutely)},
    {TRIGGER_TALK, ACTOR_TAVIS_FATHER, CONDITION_ALWAYS, MSGNO(MsgFieldOurLittleOneIsFinallyHome)},
    {TRIGGER_TALK, ACTOR_TAVIS_MOTHER, CONDITION_ALWAYS, MSGNO(MsgFieldTaviWolfedDownHisSnackAnd)},
    {TRIGGER_TALK, ACTOR_OLD_MAN_ON_MADRA, CONDITION_ALWAYS, EVENT(DailaInteriors_AskGoingToMadra)},
    {TRIGGER_TALK, ACTOR_OLD_WOMAN_ON_THE_KANDOREAN_TEMPLE, CONDITION_ALWAYS, MSGNO(MsgFieldIThinkMadraIsEastOf)},
    {TRIGGER_TALK, ACTOR_MAYOR, CONDITION_ALWAYS, EVENT(DailaInteriors_TalkNoBoats)},
    {TRIGGER_TALK, ACTOR_WOMAN_ON_THE_NOISE, CONDITION_ALWAYS, MSGNO(MsgFieldItsTooDangerousToGoTo)},
    {TRIGGER_TALK, ACTOR_MAN_ON_THE_RUMBLING, CONDITION_ALWAYS, MSGNO(MsgFieldWhoWasThatGuyIWonder)},
    {TRIGGER_TALK, ACTOR_WOMAN_ON_DAILAS_LUCK, CONDITION_ALWAYS, MSGNO(MsgFieldWhyDidThePirateBriggsPlague)},
    {TRIGGER_TALK, ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS, EVENT(DailaInteriors_TalkToWeaponMerchantOnceTaviIsRescued)},
    {TRIGGER_TALK, ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS, EVENT(DailaInteriors_TalkToArmorMerchantOnceTaviIsRescued)},
    {TRIGGER_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS, EVENT(DailaInteriors_TalkToInnkeeperOnceTaviIsRescued)},
    {TRIGGER_TALK, ACTOR_INN_WORKER, CONDITION_ALWAYS, MSGNO(MsgDailaInteriorsDinnerShowPractice)},
    {TRIGGER_TALK, ACTOR_TRAVELER_FROM_MADRA, CONDITION_ALWAYS, MSGNO(MsgDailaInteriorsSeafoodWhen)},
    {TRIGGER_TALK, ACTOR_GUEST_ON_MADRAS_SEAFOOD, CONDITION_ALWAYS, MSGNO(MsgDailaInteriorsVegetablesAgain)},
    {TRIGGER_TALK, ACTOR_RIKI, CONDITION_ALWAYS, MSGNO(MsgFieldMmmMmnphDadWasSoMad)},
    {TRIGGER_TALK, ACTOR_TAVI, CONDITION_ALWAYS, MSGNO(MsgFieldMmmphMmmmDontTellAnyoneWe)},
    {TRIGGER_TALK, ACTOR_PRACTISING_PERFORMER, CONDITION_ALWAYS, EVENT(DailaInteriors_TalkCaughtPracticing)},
    {TRIGGER_TALK, ACTOR_WOMAN_ON_THE_CURRENTS_AFTER_RESCUE, CONDITION_ALWAYS, MSGNO(MsgSeaOfTimeCurrents)},
    {TRIGGER_TALK, ACTOR_OLD_MAN_ON_THE_RED_ROCKS_AFTER_RESCUE, CONDITION_ALWAYS, MSGNO(MsgSeaOfTimeRedRocks)},
    {TRIGGER_TALK, ACTOR_OLD_WOMAN_ON_HER_SONS_AFTER_RESCUE, CONDITION_ALWAYS, MSGNO(MsgSeaOfTimeThreeGenerations)},
    {TRIGGER_TALK, ACTOR_ARMOR_COUNTER_AFTER_RESCUE, CONDITION_ALWAYS, EVENT(DailaInteriors_TalkToArmorMerchant)},
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
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PRACTISING_PERFORMER, CONDITION_ALWAYS, MSGNO(MsgDailaInteriorsSeafoodStory)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_THE_CURRENTS_AFTER_RESCUE, CONDITION_ALWAYS, MSGNO(MsgSeaOfTimeHusbandDizzy)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ON_THE_RED_ROCKS_AFTER_RESCUE, CONDITION_ALWAYS, MSGNO(MsgSeaOfTimeYeppToldMe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_ON_HER_SONS_AFTER_RESCUE, CONDITION_ALWAYS, MSGNO(MsgSeaOfTimeGiveUp)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARMOR_COUNTER_AFTER_RESCUE, CONDITION_ALWAYS, EVENT(DailaInteriors_ReadArmorMerchantsMindAfterRescue)},
    {TRIGGER_TABLE_END},
};

/// The sanctuary once Tavi is rescued: Frost on the three puddles, the jumps
/// between the pillars and the Tear at their top.
struct Trigger gDailaInteriors2EventsOnceTaviIsRescued[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 30, CONDITION_ALWAYS, EVENT(DailaInteriors_JumpForward)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), 31, CONDITION_ALWAYS, EVENT(DailaInteriors_JumpForward)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), 32, CONDITION_ALWAYS, EVENT(DailaInteriors_JumpForward)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 33, CONDITION_ALWAYS, EVENT(DailaInteriors_JumpForward)},
    {TRIGGER_TALK, ACTOR_HEALER, CONDITION_ALWAYS, EVENT(DailaInteriors_TalkToHealerOnceTaviIsRescued)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, CONDITION_ALWAYS, MSGNO(MsgDailaInteriorsBoringVillage)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_FIRST_PILLAR, FLAG_DAILA_SANCTUARY_PILLAR_FROZEN_FIRST + 0, EVENT(Scene_RaiseHeldActor)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_SECOND_PILLAR, FLAG_DAILA_SANCTUARY_PILLAR_FROZEN_FIRST + 1, EVENT(Scene_RaiseHeldActor)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_THIRD_PILLAR, FLAG_DAILA_SANCTUARY_PILLAR_FROZEN_FIRST + 2, EVENT(Scene_RaiseHeldActor)},
    {TRIGGER_EXAMINE, 40, FLAG_SEA_GODS_TEAR_TAKEN, EVENT(DailaInteriors_TakeSeaGodsTear)},
    {TRIGGER_TABLE_END},
};
