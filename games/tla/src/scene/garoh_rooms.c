/// @file
/// Inside Garoh's houses, in three scenes that share this overlay. The first
/// (SceneId_GarohRoomsHouses) holds four round houses side by side: the home
/// of the young werewolf's family, the old couple's home, the weapons and armor
/// shop, and the inn. The second is the Item Shopkeeper's house
/// (SceneId_GarohRoomsShop) and the third the sanctum
/// (SceneId_GarohRoomsSanctum). Stairs in some of the houses lead down into the
/// tunnels under the village.
///
/// Garoh is first seen on a night of the full moon (FLAG_GAROH_NIGHT), when
/// its people hide that they are werewolves: the weapons and armor shop sells
/// only claws and fangs, and two travelers who climbed Air's Rock for nothing
/// lie asleep at the inn. After the night with Maha the party wakes at the inn
/// and is told the stay is on the house (GarohRooms_WakeAtInn); from then on
/// it is day (FLAG_GAROH_SECRET_SHARED) and everyone talks openly.

#include "battle/battle.h"
#include "field/field_sequence.h"
#include "field/field_sequence_event.h"
#include "field/field.h"
#include "game/game.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "flags.h"
#include "field/scene_tables.h"
#include "system/ram_buffer.h"
#include "field/actor.h"
#include "system/main_routine_state.h"
#include "scene_ids.h"
#include "field/event_inline.h"
#include "text/msg_ids.h"
#include "scene/scene.h"
#include "sprite_ids.h"
#include "shop/shop.h"

/// The morning after a night at the inn (GarohRooms_WakeAtInn).
enum { ENTRANCE_WAKE_AT_INN = 99 };

/// The actors of the four houses, numbering the rows of
/// gGarohRooms_HousesActorPlacements.
enum GarohHousesActorId {
  /// The father of the young werewolf who led the party to Maha. His event is
  /// GarohRooms_AskWhatTheWanderersSeek.
  ACTOR_PUPS_FATHER = ACTOR_FIRST_PLACED,
  ACTOR_PUPS_MOTHER,
  ACTOR_PUPS_BROTHER,
  /// A werewolf who climbed Air's Rock for Maha's powers and did not gain
  /// them. His event is GarohRooms_AskAboutReveal.
  ACTOR_REVEAL_SEEKER,
  ACTOR_REVEAL_SEEKERS_MATE,
  ACTOR_WEAPONS_VENDOR,
  ACTOR_ARMOR_VENDOR,
  ACTOR_INNKEEPER, ///< GarohRooms_TalkToInnkeeper checks the party in
  /// The woman beside him, who greets the party the morning after.
  ACTOR_INN_WORKER,
  /// A traveler who climbed Air's Rock and found nothing, asleep at night.
  ACTOR_DISAPPOINTED_TRAVELER,
  /// His companion, still puzzling over the rock that would not spin.
  ACTOR_STUBBORN_TRAVELER,
  /// Unseen markers on the two beds the travelers sleep in at night, so that
  /// talking to a bed, or reading its sleeper's mind, reaches the traveler
  /// lying in it; they are taken away in the morning.
  ACTOR_DISAPPOINTED_TRAVELERS_BED,
  ACTOR_STUBBORN_TRAVELERS_BED
};

/// The Item Shopkeeper's house's actors, numbering the rows of
/// gGarohRooms_ShopActorPlacements.
enum GarohShopActorId {
  ACTOR_AIRS_ROCK_GUARDIAN = ACTOR_FIRST_PLACED,
  ACTOR_VILLAGER_ON_WHIRLWIND,
  ACTOR_VILLAGER_ON_THE_WIND_STONE,
  ACTOR_ITEM_SHOPKEEPER
};

/// The Sanctum's priest, its one actor.
enum GarohSanctumActorId { ACTOR_PRIEST = ACTOR_FIRST_PLACED };

/// The stoves, by their cells' behaviour in the map, each with something
/// different cooking on it.
enum GarohRoomsCellBehavior {
  /// Bug larvae frying, in the young werewolf's family's home.
  CELL_BEHAVIOR_LARVAE_STOVE = 209,
  /// Lamb on the bone, at the inn.
  CELL_BEHAVIOR_LAMB_STOVE,
  /// Lobster grilled in the shell, in the Item Shopkeeper's house.
  CELL_BEHAVIOR_LOBSTER_STOVE,
  /// Newts on skewers, in the old couple's home.
  CELL_BEHAVIOR_NEWT_STOVE,
  /// A pot of strange plants, in the weapons and armor shop.
  CELL_BEHAVIOR_PLANT_POT_STOVE
};

extern struct ActorPlacement gGarohRooms_NoActorPlacements[];
extern struct ActorPlacement gGarohRooms_HousesActorPlacements[];
extern struct ActorPlacement gGarohRooms_ShopActorPlacements[];
extern struct ActorPlacement gGarohRooms_SanctumActorPlacements[];
extern struct Trigger gGarohRooms_DoorTriggers[];
extern struct Trigger gGarohRooms_HousesNightTriggers[];
extern struct Trigger gGarohRooms_ShopNightTriggers[];
extern struct Trigger gGarohRooms_SanctumTriggers[];
extern struct Trigger gGarohRooms_HousesMorningTriggers[];
extern struct Trigger gGarohRooms_ShopMorningTriggers[];

void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void MapGrid_CenterOnFollowedPoint(void);
void ActorSystem_SuspendProjectionAndShowSprites(void);
void ActorSystem_ResumeProjectionAndHideLayers(void);
void Event_End(void);

void GarohRooms_WakeAtInn(void);

void MapGrid_SetShownLayers(s32 value);

void FieldEffect_StartBlankOverlay(s32 value);

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  s16 scene = gMainRoutineState.next;

  if (scene == SCENE_GAROH_ROOMS_HOUSES)
    return gGarohRooms_HousesActorPlacements;
  if (scene == SCENE_GAROH_ROOMS_SHOP)
    return gGarohRooms_ShopActorPlacements;
  if (scene == SCENE_GAROH_ROOMS_SANCTUM)
    return gGarohRooms_SanctumActorPlacements;
  return gGarohRooms_NoActorPlacements;
}

/// Garoh's shops, as Shop_Run numbers them.
enum { SHOP_GAROH_WEAPONS = 10, SHOP_GAROH_ARMOR = 11, SHOP_GAROH_ITEMS = 12 };

/// Garoh's inn, as Inn_CheckIn numbers the inns.
enum { GAROH_INN = 3 };

/// Talking to the Weapons Vendor, the bald man in red behind the counter of
/// the weapons and armor shop. By day, from in front of the counter, his shop
/// opens; at night he is sorry, but on nights of the full moon they only sell
/// claws and fangs.
void GarohRooms_TalkToWeaponsVendor(s32 actor) {
  struct Actor *leader = Actor_GetById(gMainRoutineState.leaderActorId);

  u16 facing = FACING_NEAREST_CARDINAL(leader->facing);

  if (facing == FACING_NORTH && GameFlag_Get(FLAG_GAROH_SECRET_SHARED)) {
    Shop_Run(SHOP_GAROH_WEAPONS, actor);
  } else {
    if (GameFlag_Get(FLAG_GAROH_SECRET_SHARED))
      // "What do you think? I'll bet these weapons suit you better than any
      // fangs would!"
      Event_SetMessageId((s32)MsgGarohRoomsWhatDoYouThinkIll);
    else
      // "I'm sorry, but on nights of the full moon, we only sell claws and
      // fangs."
      Event_SetMessageId((s32)MsgGarohRoomsImSorryButOnNights);
    Event_ShowMessageAndWait(actor, 0);
  }
}

/// Talking to the Armor Vendor, the man with the orange beard beside the
/// Weapons Vendor. By day, from in front of the counter, his shop opens; at
/// night he only says it is unfortunate they came on a night of the full moon.
void GarohRooms_TalkToArmorVendor(s32 actor) {
  struct Actor *leader = Actor_GetById(gMainRoutineState.leaderActorId);

  u16 facing = FACING_NEAREST_CARDINAL(leader->facing);

  if (facing == FACING_NORTH && GameFlag_Get(FLAG_GAROH_SECRET_SHARED)) {
    Shop_Run(SHOP_GAROH_ARMOR, actor);
  } else {
    if (GameFlag_Get(FLAG_GAROH_SECRET_SHARED))
      // "When we change form, it's best to be unclothed."
      Event_SetMessageId((s32)MsgGarohRoomsWhenWeChangeFormIts);
    else
      // "Unfortunate that you should come on a night of the full moon, sir..."
      Event_SetMessageId((s32)MsgGarohRoomsUnfortunateThatYouShouldCome);
    Event_ShowMessageAndWait(actor, 0);
  }
}

/// Talking to the Item Shopkeeper, the woman with a ponytail in her house:
/// from in front of the counter her shop opens, night or day. From anywhere
/// else she talks; at night she asks whether the party came from Alhafra.
void GarohRooms_TalkToItemShopkeeper(s32 actor) {
  struct Actor *leader = Actor_GetById(gMainRoutineState.leaderActorId);

  u16 facing = FACING_NEAREST_CARDINAL(leader->facing);

  if (facing == FACING_NORTH) {
    Shop_Run(SHOP_GAROH_ITEMS, actor);
  } else if (GameFlag_Get(FLAG_GAROH_SECRET_SHARED)) {
    Event_SetMessageId((s32)MsgGarohRoomsIveHeardStoriesAboutOther);
    // "I've heard stories about other mountains like our own Air's Rock..."
    Event_ShowMessageAndWait(actor, 0);
  } else {
    s32 message = (s32)MsgGarohRoomsDidYouComeHereFrom;

    // "Did you come here from Alhafra?" Yes: "Really? You must have gone
    // through the cave..." No: "Oh, so you must have come by way of
    // Mikasalla."
    Event_SetMessageId(message);
    FieldMessage_ShowAtActor(actor, 0);
    if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
      Event_Wait(10);
      Event_SetMessageId(message + 1);
    } else {
      Event_Wait(20);
      Event_SetMessageId(message + 2);
    }
    Event_ShowMessageAndWait(actor, 0);
  }
}

/// Talking to the innkeeper behind the inn's counter: from in front of it he
/// offers a night's stay, and from elsewhere he only chats.
void GarohRooms_TalkToInnkeeper(s32 actor) {
  struct Actor *leader = Actor_GetById(gMainRoutineState.leaderActorId);

  u16 facing = FACING_NEAREST_CARDINAL(leader->facing);

  if (facing == FACING_NORTH) {
    Inn_CheckIn(GAROH_INN, actor);
  } else {
    if (GameFlag_Get(FLAG_GAROH_SECRET_SHARED))
      // "If you can pass the tests of Air's Rock, you can do anything!"
      Event_SetMessageId((s32)MsgGarohRoomsIfYouCanPassThe);
    else
      // "A clear night around here always means fair weather on the morrow..."
      Event_SetMessageId((s32)MsgGarohRoomsAClearNightAroundHere);
    Event_ShowMessageAndWait(actor, 0);
  }
}

/// Talking to the priest in Garoh's sanctum: he turns to Felix, and from in
/// front of his counter offers the sanctum's services; from elsewhere he only
/// chats. He then faces south again.
void GarohRooms_TalkToPriest(void) {
  s16 facing;

  facing = FACING_NEAREST_CARDINAL(
      Actor_GetById(gMainRoutineState.leaderActorId)->facing);
  Actor_FaceActorById(ACTOR_PRIEST, gMainRoutineState.leaderActorId, 0);
  if ((u16)facing == FACING_NORTH) {
    Sanctum_Run(ACTOR_PRIEST);
  } else {
    if (GameFlag_Get(FLAG_GAROH_SECRET_SHARED))
      // "I've heard about you. They say you're special, like the people in this
      // town."
      Event_SetMessageId((s32)MsgGarohRoomsIveHeardAboutYouThey);
    else
      // "The people here may be different, but they certainly are nice..."
      Event_SetMessageId((s32)MsgGarohRoomsThePeopleHereMayBe);
    Event_ShowMessageAndWait(ACTOR_PRIEST, 0);
  }
  Actor_FaceDirection(ACTOR_PRIEST, FACING_SOUTH, 0);
}

/// Talking at night to the young werewolf's father, the man with brown hair
/// pacing his family's home: "Good evening, wanderers! Are you looking for
/// something?", and then whether they are searching for werewolves.
void GarohRooms_AskWhatTheWanderersSeek(s32 actor) {
  s32 message;

  Event_Start();
  Event_EndLastingPsynergy(0);
  message = (s32)MsgGarohRoomsGoodEveningWanderersAreYou;
  Event_SetMessageId(message);
  // "Good evening, wanderers! Are you looking for something?"
  FieldMessage_ShowAtActor(actor, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineState.leaderActorId, 0) ==
      0) {
    Event_SetMessageId(message + 2);
    // "Searching for werewolves, eh?"
    FieldMessage_ShowAtActor(actor, 0);
    if (Event_AskYesNoWithActorAnimation(gMainRoutineState.leaderActorId, 0) ==
        0) {
      // "You don't really believe they exist, do you? If so, you'd best..."
      Event_ShowMessageAndWait(actor, 0);
    } else {
      // Past "You don't really believe they exist, do you?" to the next line.
      gFieldState->message++;
      // "Hehrrr... Sorry. Just thought I'd ask. We get some crazy folk..."
      Event_ShowMessageAndWait(actor, 0);
    }
  } else {
    // "Just...staying up late, eh? Can't say I blame you, on a night like..."
    Event_ShowMessageAndWait(actor, 0);
  }
  Event_End();
}

/// Talking to the disappointed traveler, one of the two warriors in golden
/// helmets at the inn. At night, asleep in his bed, he mumbles "What a
/// waste... Just a big rock..."; by day he grumbles that the wolves were
/// noisy again last night.
void GarohRooms_TalkToDisappointedTraveler(void) {
  Event_Start();
  Event_EndLastingPsynergy(0);
  if (GameFlag_Get(FLAG_GAROH_SECRET_SHARED))
    // "Those wolves were really noisy last night, too."
    Event_SetMessageId((s32)MsgGarohRoomsThoseWolvesWereReallyNoisy);
  else
    // "Zzzz... Zzzz... What a waste... Just a big rock..."
    Event_SetMessageId((s32)MsgGarohRoomsZzzzZzzzWhatAWaste);
  Event_ShowMessageAndWait(ACTOR_DISAPPOINTED_TRAVELER, 0);
  Event_End();
}

/// Talking to the stubborn traveler, the other warrior in a golden helmet at
/// the inn. At night, asleep in his bed, he mumbles "Whirlwind? Wind
/// Stone?"; by day he wants to stay and try climbing Air's Rock once more.
void GarohRooms_TalkToStubbornTraveler(void) {
  Event_Start();
  Event_EndLastingPsynergy(0);
  if (GameFlag_Get(FLAG_GAROH_SECRET_SHARED))
    // "Maybe we should stay and try climbing Air's Rock one more time."
    Event_SetMessageId((s32)MsgGarohRoomsMaybeWeShouldStayAnd);
  else
    // "Zzz... Whuzza? Whirlwind? Wind Stone? Zzzzrrrgg..."
    Event_SetMessageId((s32)MsgGarohRoomsZzzWhuzzaWhirlwindWindStone);
  Event_ShowMessageAndWait(ACTOR_STUBBORN_TRAVELER, 0);
  Event_End();
}

/// Mind Read on the disappointed traveler at the inn. At night he is bitter
/// that he climbed all over that stupid rock for nothing; by day he is
/// scared of wolves and keeps indoors at night.
void GarohRooms_ReadDisappointedTravelersMind(void) {
  Event_Start();
  Event_EndLastingPsynergy(0);
  if (GameFlag_Get(FLAG_GAROH_SECRET_SHARED))
    // "I'm scared of wolves, so I don't go out much at night..."
    Event_SetMessageId((s32)MsgGarohRooms64);
  else
    // "...Climbed all over that stupid rock, and for what? Nothing!"
    Event_SetMessageId((s32)MsgGarohRoomsClimbedAllOverThatStupid);
  Event_ShowMessageAndWait(ACTOR_DISAPPOINTED_TRAVELER, 0);
  Event_End();
}

/// Mind Read on the stubborn traveler at the inn. At night he wonders how he
/// was supposed to make so huge a rock spin; by day he thinks a lever would
/// do it.
void GarohRooms_ReadStubbornTravelersMind(void) {
  Event_Start();
  Event_EndLastingPsynergy(0);
  if (GameFlag_Get(FLAG_GAROH_SECRET_SHARED))
    // "If I used a lever, I could probably make that rock spin..."
    Event_SetMessageId((s32)MsgGarohRoomsIfIUsedALever);
  else
    // "That rock was huge. How was I supposed to make it spin?"
    Event_SetMessageId((s32)MsgGarohRoomsThatRockWasHugeHow);
  Event_ShowMessageAndWait(ACTOR_STUBBORN_TRAVELER, 0);
  Event_End();
}

/// Talking by day to the old man with a cane in the old couple's home, the
/// first time: "You... Is it true that you learned Reveal?", with an answer
/// for Yes and one for No.
void GarohRooms_AskAboutReveal(void) {
  s32 message = (s32)MsgGarohRoomsYouIsItTrueThat;

  // "You... Is it true that you learned Reveal?"
  Event_SetMessageId(message);
  FieldMessage_ShowAtActor(ACTOR_REVEAL_SEEKER, 0);
  if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
    Event_Wait(10);
    Event_SetMessageId(message + 1);
  } else {
    Event_Wait(20);
    Event_SetMessageId(message + 2);
  }
  Event_ShowMessageAndWait(ACTOR_REVEAL_SEEKER, 0);
  GameFlag_Set(FLAG_GAROH_ASKED_ABOUT_REVEAL);
}

/// Returns the room's triggers: the night's until the secret is shared, the
/// morning's after, and the inn's morning ones as the party wakes there.
const struct Trigger *Scene_GetTriggers(void) {
  s32 shared = GameFlag_Get(FLAG_GAROH_SECRET_SHARED);
  struct MainRoutineState *party = &gMainRoutineState;
  s32 houses = SCENE_GAROH_ROOMS_HOUSES;
  s32 scene;

  if (shared) {
    scene = party->next;
    if (scene == houses)
      return gGarohRooms_HousesMorningTriggers;
    if (scene == SCENE_GAROH_ROOMS_SHOP)
      return gGarohRooms_ShopMorningTriggers;
  } else {
    scene = party->next;
    if (scene == houses) {
      if (party->entrance == ENTRANCE_WAKE_AT_INN)
        return gGarohRooms_HousesMorningTriggers;
      return gGarohRooms_HousesNightTriggers;
    }
    if (scene == SCENE_GAROH_ROOMS_SHOP)
      return gGarohRooms_ShopNightTriggers;
  }
  if (scene == SCENE_GAROH_ROOMS_SANCTUM)
    return gGarohRooms_SanctumTriggers;
  return gGarohRooms_DoorTriggers;
}

/// The map layers the rooms show at night (MapGrid_SetShownLayers), which
/// leave out the light falling through the windows.
#define GAROH_ROOMS_NIGHT_LAYERS (DISPCNT_BG1_ON | DISPCNT_BG3_ON)

/// Sets up the room being entered: the travelers in their beds at night, or
/// up once the secret is shared; waking at the inn, and night's layers.
s32 Scene_Initialize(void) {
  s32 turn;

  gFieldState->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
  Actor_GetById(gMainRoutineState.leaderActorId)->priorityFlags |=
      ACTOR_PRIORITY_IGNORES_RISES;
  FieldEffect_StartBlankOverlay(0);
  if (gMainRoutineState.next == SCENE_GAROH_ROOMS_HOUSES) {
    if (GameFlag_Get(FLAG_GAROH_SECRET_SHARED) ||
        gMainRoutineState.entrance == ENTRANCE_WAKE_AT_INN) {
      Actor_SetPosition(ACTOR_DISAPPOINTED_TRAVELERS_BED, 0, 0);
      Actor_SetPosition(ACTOR_STUBBORN_TRAVELERS_BED, 0, 0);
      Actor_SetMovementTypeById(ACTOR_STUBBORN_TRAVELER,
                                MOVEMENT_TYPE_WANDER_NEAR);
    } else {
      turn = FACING_SOUTHEAST;
      Actor_SetPositionAndFacing(ACTOR_DISAPPOINTED_TRAVELER, toFIXED(306),
                                 toFIXED(335), turn);
      {
        s32 angle = FACING_SOUTHWEST;
        FIXED z = toFIXED(336);

        Actor_SetHorizontalPositionAndAngleWithTerrain(ACTOR_STUBBORN_TRAVELER,
                                                       toFIXED(345), z, angle);
      }
      Actor_SetSpriteFlags(Actor_GetById(ACTOR_DISAPPOINTED_TRAVELERS_BED), 0);
      Actor_SetSpriteFlags(Actor_GetById(ACTOR_STUBBORN_TRAVELERS_BED), 0);
      Actor_GetById(ACTOR_DISAPPOINTED_TRAVELER)->motionFlags = 0;
      Actor_GetById(ACTOR_DISAPPOINTED_TRAVELER)->y.fixed = 0;
      Actor_GetById(ACTOR_STUBBORN_TRAVELER)->motionFlags = 0;
      Actor_GetById(ACTOR_STUBBORN_TRAVELER)->y.fixed = 0;
      Actor_GetById(ACTOR_DISAPPOINTED_TRAVELERS_BED)->motionFlags = 0;
      Actor_GetById(ACTOR_DISAPPOINTED_TRAVELERS_BED)->y.fixed = 0;
      Actor_GetById(ACTOR_STUBBORN_TRAVELERS_BED)->motionFlags = 0;
      Actor_GetById(ACTOR_STUBBORN_TRAVELERS_BED)->y.fixed = 0;
      Actor_SetSpriteRotationById(ACTOR_DISAPPOINTED_TRAVELER, DEGtoANG(315));
      Actor_SetSpriteFlags(Actor_GetById(ACTOR_DISAPPOINTED_TRAVELER), 0);
      Actor_SetSpriteRotationById(ACTOR_STUBBORN_TRAVELER, turn);
      Actor_SetSpriteFlags(Actor_GetById(ACTOR_STUBBORN_TRAVELER), 0);
    }
    if (gMainRoutineState.entrance == ENTRANCE_WAKE_AT_INN &&
        !GameFlag_Get(FLAG_GAROH_SECRET_SHARED))
      GarohRooms_WakeAtInn();
  }
  if (GameFlag_Get(FLAG_GAROH_SECRET_SHARED))
    GameFlag_Clear(FLAG_GAROH_NIGHT);
  if (GameFlag_Get(FLAG_GAROH_NIGHT))
    MapGrid_SetShownLayers(GAROH_ROOMS_NIGHT_LAYERS);
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

/// The morning after the night with Maha, at the inn: "Felix and party stayed
/// the night at the inn.", the woman at the counter says they did not sleep
/// much, and the innkeeper says Master Maha told them all about the party,
/// the bill is on them, and bids them farewell.
void GarohRooms_WakeAtInn(void) {
  GameFlag_Set(FLAG_GAROH_SECRET_SHARED);
  Event_Start();
  Event_EndLastingPsynergy(0);
  Event_Wait(30);
  ActorSystem_SuspendProjectionAndShowSprites();
  Sound_Play(MUS_OVERNIGHT_AT_THE_INN);
  // "Felix and party stayed the night at the inn."
  FieldMessage_ShowCentered((s32)MsgGarohRoomsAndPartyStayedTheNight, 0, 0);
  Event_Wait(20);
  ActorSystem_ResumeProjectionAndHideLayers();
  MapGrid_CenterOnFollowedPoint();
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  // "You folks didn't sleep much last night..."
  Event_SetMessageId((s32)MsgGarohRoomsYouFolksDidntSleepMuch);
  // "You folks didn't sleep much last night... You don't look like you..."
  Event_ShowMessageThenWait(ACTOR_INN_WORKER, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_INNKEEPER, 2);
  Event_Wait(20);
  // "Master Maha told us all about you. Don't you worry about the bill. It's
  // on us."
  Event_ShowMessageThenWait(ACTOR_INNKEEPER, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_INNKEEPER, ANIM_NOD);
  Event_Wait(30);
  // "Farewell, friends."
  Event_ShowMessageThenWait(ACTOR_INNKEEPER, 0, 10);
  Event_End();
}

#include "shared/scene_entrances_default.c"

u32 gSceneExits[] = {
    SCENE_EXITS(GarohRoomsHouses),
    SCENE_EXIT_UNLESS(GAROH_ROOMS_HOUSES_TO_GAROH_SOUTH, Garoh,
                      GAROH_FROM_GAROH_ROOMS_HOUSES_SOUTH, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(GAROH_ROOMS_HOUSES_TO_GAROH_NORTHWEST, Garoh,
                      GAROH_FROM_GAROH_ROOMS_HOUSES_NORTHWEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(GAROH_ROOMS_HOUSES_TO_GAROH_SOUTHWEST, Garoh,
                      GAROH_FROM_GAROH_ROOMS_HOUSES_SOUTHWEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(GAROH_ROOMS_HOUSES_TO_GAROH_SOUTHEAST, Garoh,
                      GAROH_FROM_GAROH_ROOMS_HOUSES_SOUTHEAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(GAROH_ROOMS_HOUSES_TO_GAROH_TUNNELS_EAST, GarohTunnels,
                      GAROH_TUNNELS_FROM_GAROH_ROOMS_HOUSES_EAST,
                      EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(GAROH_ROOMS_HOUSES_TO_GAROH_TUNNELS_WEST, GarohTunnels,
                      GAROH_TUNNELS_FROM_GAROH_ROOMS_HOUSES_WEST,
                      EXIT_FLAG_NONE),
    SCENE_EXITS(GarohRoomsShop),
    SCENE_EXIT_UNLESS(GAROH_ROOMS_SHOP_TO_GAROH_TUNNELS_WEST, GarohTunnels,
                      GAROH_TUNNELS_FROM_GAROH_ROOMS_SHOP_WEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(GAROH_ROOMS_SHOP_TO_GAROH, Garoh,
                      GAROH_FROM_GAROH_ROOMS_SHOP, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(GAROH_ROOMS_SHOP_TO_GAROH_TUNNELS_EAST, GarohTunnels,
                      GAROH_TUNNELS_FROM_GAROH_ROOMS_SHOP_EAST, EXIT_FLAG_NONE),
    SCENE_EXITS(GarohRoomsSanctum),
    SCENE_EXIT_UNLESS(GAROH_ROOMS_SANCTUM_TO_GAROH, Garoh,
                      GAROH_FROM_GAROH_ROOMS_SANCTUM, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

// clang-format off
struct ActorPlacement gGarohRooms_NoActorPlacements[] = {
    {SCENE_TABLE_END},
};

struct ActorPlacement gGarohRooms_HousesActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 336, 0, 120, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 376, 0, 104, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 360, 0, 136, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 568, 0, 136, FACING_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 592, 0, 96, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BALD_MAN_IN_RED, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 134, 0, 120, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_ORANGE_BEARD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 112, 0, 120, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_DARK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 424, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PINK_APRON, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 420, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WARRIOR_IN_GOLDEN_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 280, 0, 344, FACING_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WARRIOR_IN_GOLDEN_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 330, 0, 376, FACING_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 336, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 352, 0, 344, FACING_SOUTHWEST, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gGarohRooms_ShopActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 372, 0, 406, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 424, 0, 424, FACING_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 352, 0, 456, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_PONYTAIL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 412, FACING_SOUTH, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gGarohRooms_SanctumActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 448, 0, 152, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    {SCENE_TABLE_END},
};

struct Trigger gGarohRooms_DoorTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_TABLE_END},
};
// clang-format on

struct Trigger gGarohRooms_HousesNightTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_TALK, ACTOR_PUPS_FATHER, CONDITION_ALWAYS,
     EVENT(GarohRooms_AskWhatTheWanderersSeek)},
    {TRIGGER_TALK, ACTOR_PUPS_MOTHER, CONDITION_ALWAYS,
     MSGNO(MsgGarohRoomsOurOldestSonIsAlways)},
    {TRIGGER_TALK, ACTOR_PUPS_BROTHER, CONDITION_ALWAYS,
     MSGNO(MsgGarohRoomsNoFairMyBrotherAlways)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PUPS_FATHER, CONDITION_ALWAYS,
     MSGNO(MsgGarohRoomsOutsidersCouldNeverUnderstandThe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PUPS_MOTHER, CONDITION_ALWAYS,
     MSGNO(MsgGarohRoomsThatPupOfMineJust)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PUPS_BROTHER, CONDITION_ALWAYS,
     MSGNO(MsgGarohRoomsWhatMadeMasterMahaTake)},
    {TRIGGER_TALK, ACTOR_REVEAL_SEEKER, CONDITION_ALWAYS,
     MSGNO(MsgGarohRoomsWhenOurPeopleGrowOlder)},
    {TRIGGER_TALK, ACTOR_REVEAL_SEEKERS_MATE, CONDITION_ALWAYS,
     MSGNO(MsgGarohRoomsMyMateIsCloseTo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_REVEAL_SEEKER,
     CONDITION_ALWAYS, MSGNO(MsgGarohRoomsICastWhirlwindOnThe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_REVEAL_SEEKERS_MATE,
     CONDITION_ALWAYS, MSGNO(MsgGarohRooms16)},
    {TRIGGER_TALK, ACTOR_WEAPONS_VENDOR, CONDITION_ALWAYS,
     EVENT(GarohRooms_TalkToWeaponsVendor)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEAPONS_VENDOR,
     CONDITION_ALWAYS, MSGNO(MsgGarohRoomsIShouldBeOutsideHowling)},
    {TRIGGER_TALK, ACTOR_ARMOR_VENDOR, CONDITION_ALWAYS,
     EVENT(GarohRooms_TalkToArmorVendor)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARMOR_VENDOR, CONDITION_ALWAYS,
     MSGNO(MsgGarohRoomsTheseStrangersSeemNormalEnough)},
    {TRIGGER_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS,
     EVENT(GarohRooms_TalkToInnkeeper)},
    {TRIGGER_TALK, ACTOR_INN_WORKER, CONDITION_ALWAYS,
     MSGNO(MsgGarohRoomsAirsRockCanGetQuite)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, CONDITION_ALWAYS,
     MSGNO(MsgGarohRoomsItOnlyRainsAFew)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INN_WORKER, CONDITION_ALWAYS,
     MSGNO(MsgGarohRoomsMostPeopleSeeNothingOf)},
    {TRIGGER_TALK, ACTOR_DISAPPOINTED_TRAVELERS_BED, CONDITION_ALWAYS,
     EVENT(GarohRooms_TalkToDisappointedTraveler)},
    {TRIGGER_TALK, ACTOR_STUBBORN_TRAVELERS_BED, CONDITION_ALWAYS,
     EVENT(GarohRooms_TalkToStubbornTraveler)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_DISAPPOINTED_TRAVELERS_BED,
     CONDITION_ALWAYS, EVENT(GarohRooms_ReadDisappointedTravelersMind)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUBBORN_TRAVELERS_BED,
     CONDITION_ALWAYS, EVENT(GarohRooms_ReadStubbornTravelersMind)},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), CELL_BEHAVIOR_LARVAE_STOVE,
     CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgGarohRoomsEeeeewTheyreFryingUpBug))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), CELL_BEHAVIOR_LAMB_STOVE, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgGarohRoomsItsLambOnTheBone))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), CELL_BEHAVIOR_NEWT_STOVE, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgGarohRoomsNewtsSkeweredOverAnOpen))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), CELL_BEHAVIOR_PLANT_POT_STOVE,
     CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgGarohRoomsThereAreLotsOfStrange))},
    {TRIGGER_TABLE_END},
};

struct Trigger gGarohRooms_ShopNightTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_TALK, ACTOR_AIRS_ROCK_GUARDIAN, CONDITION_ALWAYS,
     MSGNO(MsgGarohRoomsGarohExistsToProtectAirs)},
    {TRIGGER_TALK, ACTOR_VILLAGER_ON_WHIRLWIND, CONDITION_ALWAYS,
     MSGNO(MsgGarohRoomsSoYouveHeardOfWhirlwind)},
    {TRIGGER_TALK, ACTOR_VILLAGER_ON_THE_WIND_STONE, CONDITION_ALWAYS,
     MSGNO(MsgGarohRoomsIveHeardAboutTheWind)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_AIRS_ROCK_GUARDIAN,
     CONDITION_ALWAYS, MSGNO(MsgGarohRoomsWeHaveOtherPowersThan)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ON_WHIRLWIND,
     CONDITION_ALWAYS, MSGNO(MsgGarohRoomsAndDifferentPeopleUseWhirlwind)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ON_THE_WIND_STONE,
     CONDITION_ALWAYS, MSGNO(MsgGarohRoomsIWishIKnewWhat)},
    {TRIGGER_TALK, ACTOR_ITEM_SHOPKEEPER, CONDITION_ALWAYS,
     EVENT(GarohRooms_TalkToItemShopkeeper)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ITEM_SHOPKEEPER,
     CONDITION_ALWAYS, MSGNO(MsgGarohRoomsINeverChangeMyStock)},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), CELL_BEHAVIOR_LOBSTER_STOVE,
     CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgGarohRoomsHuhWhatIsThatLobster))},
    {TRIGGER_TABLE_END},
};

struct Trigger gGarohRooms_SanctumTriggers[] = {
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_TALK, ACTOR_PRIEST, CONDITION_ALWAYS,
     EVENT(GarohRooms_TalkToPriest)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PRIEST,
     FLAG_GAROH_SECRET_SHARED, MSGNO(MsgGarohRooms32)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PRIEST, CONDITION_ALWAYS,
     MSGNO(MsgGarohRoomsAmITheOnlyOne)},
    // The same rows again for an actor 9, which the sanctum does not place.
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, EVENT(GarohRooms_TalkToPriest)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, FLAG_GAROH_SECRET_SHARED,
     MSGNO(MsgGarohRooms32)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS,
     MSGNO(MsgGarohRoomsAmITheOnlyOne)},
    {TRIGGER_TABLE_END},
};

struct Trigger gGarohRooms_HousesMorningTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_TALK, ACTOR_PUPS_FATHER, CONDITION_ALWAYS,
     MSGNO(MsgGarohRoomsHeyItsYouThanksFor)},
    {TRIGGER_TALK, ACTOR_PUPS_MOTHER, CONDITION_ALWAYS,
     MSGNO(MsgGarohRoomsEveryoneIsAmazedThatYou)},
    {TRIGGER_TALK, ACTOR_PUPS_BROTHER, CONDITION_ALWAYS,
     MSGNO(MsgGarohRoomsMyBrotherShouldHaveGotten)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PUPS_FATHER, CONDITION_ALWAYS,
     MSGNO(MsgGarohRoomsTheYoungerTheWerewolfThe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PUPS_MOTHER, CONDITION_ALWAYS,
     MSGNO(MsgGarohRoomsOhThankGoodnessEverythingWorked)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PUPS_BROTHER, CONDITION_ALWAYS,
     MSGNO(MsgGarohRoomsAndDadSaidHeWas)},
    {TRIGGER_TALK, ACTOR_REVEAL_SEEKER, FLAG_GAROH_ASKED_ABOUT_REVEAL,
     EVENT(GarohRooms_AskAboutReveal)},
    {TRIGGER_TALK, ACTOR_REVEAL_SEEKER, CONDITION_ALWAYS,
     MSGNO(MsgGarohRoomsToThinkIveWastedSo)},
    {TRIGGER_TALK, ACTOR_REVEAL_SEEKERS_MATE, CONDITION_ALWAYS,
     MSGNO(MsgGarohRoomsHesWorkedSoHardBut)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_REVEAL_SEEKER,
     CONDITION_ALWAYS, MSGNO(MsgGarohRoomsIThoughtICouldBecome)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_REVEAL_SEEKERS_MATE,
     CONDITION_ALWAYS, MSGNO(MsgGarohRoomsSoNowEvenOutsidersAre)},
    {TRIGGER_TALK, ACTOR_WEAPONS_VENDOR, CONDITION_ALWAYS,
     EVENT(GarohRooms_TalkToWeaponsVendor)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEAPONS_VENDOR,
     CONDITION_ALWAYS, MSGNO(MsgGarohRoomsIPreferTheToothAnd)},
    {TRIGGER_TALK, ACTOR_ARMOR_VENDOR, CONDITION_ALWAYS,
     EVENT(GarohRooms_TalkToArmorVendor)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARMOR_VENDOR, CONDITION_ALWAYS,
     MSGNO(MsgGarohRoomsMahaSaysThatWhenWere)},
    {TRIGGER_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS,
     EVENT(GarohRooms_TalkToInnkeeper)},
    {TRIGGER_TALK, ACTOR_INN_WORKER, CONDITION_ALWAYS,
     MSGNO(MsgGarohRoomsMahaCameAlongAndSaid)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, CONDITION_ALWAYS,
     MSGNO(MsgGarohRoomsIveHeardStoriesAboutOtherPlaces)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INN_WORKER, CONDITION_ALWAYS,
     MSGNO(MsgGarohRoomsHeSaidIfTheyWere)},
    {TRIGGER_TALK, ACTOR_DISAPPOINTED_TRAVELER, CONDITION_ALWAYS,
     EVENT(GarohRooms_TalkToDisappointedTraveler)},
    {TRIGGER_TALK, ACTOR_STUBBORN_TRAVELER, CONDITION_ALWAYS,
     EVENT(GarohRooms_TalkToStubbornTraveler)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_DISAPPOINTED_TRAVELER,
     CONDITION_ALWAYS, EVENT(GarohRooms_ReadDisappointedTravelersMind)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUBBORN_TRAVELER,
     CONDITION_ALWAYS, EVENT(GarohRooms_ReadStubbornTravelersMind)},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), CELL_BEHAVIOR_LARVAE_STOVE,
     CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgGarohRoomsEeeeewTheyreFryingUpBug))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), CELL_BEHAVIOR_LAMB_STOVE, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgGarohRoomsItsLambOnTheBone))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), CELL_BEHAVIOR_NEWT_STOVE, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgGarohRoomsNewtsSkeweredOverAnOpen))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), CELL_BEHAVIOR_PLANT_POT_STOVE,
     CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgGarohRoomsThereAreLotsOfStrange))},
    {TRIGGER_TABLE_END},
};

struct Trigger gGarohRooms_ShopMorningTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_TALK, ACTOR_AIRS_ROCK_GUARDIAN, CONDITION_ALWAYS,
     MSGNO(MsgGarohRoomsSoYoureSomethingSpecialAn)},
    {TRIGGER_TALK, ACTOR_VILLAGER_ON_WHIRLWIND, CONDITION_ALWAYS,
     MSGNO(MsgGarohRoomsYouLearnedRevealAtAirs)},
    {TRIGGER_TALK, ACTOR_VILLAGER_ON_THE_WIND_STONE, CONDITION_ALWAYS,
     MSGNO(MsgGarohRoomsIJustCantBelieveThat)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_AIRS_ROCK_GUARDIAN,
     CONDITION_ALWAYS, MSGNO(MsgGarohRoomsDidMahaMeanThatThey)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ON_WHIRLWIND,
     CONDITION_ALWAYS, MSGNO(MsgGarohRoomsSoThesePeopleCanUse)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ON_THE_WIND_STONE,
     CONDITION_ALWAYS, MSGNO(MsgGarohRoomsNormallyOutsidersNeverGetIn)},
    {TRIGGER_TALK, ACTOR_ITEM_SHOPKEEPER, CONDITION_ALWAYS,
     EVENT(GarohRooms_TalkToItemShopkeeper)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ITEM_SHOPKEEPER,
     CONDITION_ALWAYS, MSGNO(MsgGarohRoomsIHeardThatOneThats)},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), CELL_BEHAVIOR_LOBSTER_STOVE,
     CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgGarohRoomsHuhWhatIsThatLobster))},
    {TRIGGER_TABLE_END},
};
