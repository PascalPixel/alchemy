/// @file
/// Garoh's houses, in three scenes that share this overlay: the inn, where
/// the weapon and armor merchants keep their counters
/// (SceneId_GarohInteriorsHouses), the house of the item merchant
/// (SceneId_GarohInteriorsShop) and the sanctum
/// (SceneId_GarohInteriorsSanctum).
///
/// Garoh is first seen on a night of the full moon (FLAG_GAROH_NIGHT), when
/// its people hide that they are werewolves: the merchants sell only claws and
/// fangs, and two travelers who failed Air's Rock sleep at the inn. After the
/// night with Maha the party wakes at the inn (GarohInteriors_WakeAtInn,
/// entrance 99), the secret is shared (FLAG_GAROH_SECRET_SHARED) and the
/// rooms take their morning tables.

#include "battle/battle.h"
#include "field/field_sequence.h"
#include "field/field_sequence_event.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "flags.h"
#include "field/scene_tables.h"
#include "system/ram_buffer.h"
#include "field/actor.h"
#include "system/main_routine_work.h"
#include "scene_ids.h"
#include "field/event_inline.h"
#include "text/msg_ids.h"
#include "scene/scene.h"
#include "sprite_ids.h"

/// The morning after a night at the inn (GarohInteriors_WakeAtInn).
enum { ENTRANCE_WAKE_AT_INN = 99 };

/// The inn's actors, numbering the rows of gGarohInteriors_InnActorPlacements.
enum GarohInnActorId {
  /// The father of the young werewolf who led the party to Maha. His event is
  /// GarohInteriors_AskWhatTheWanderersSeek.
  ACTOR_PUPS_FATHER = ACTOR_FIRST_PLACED,
  ACTOR_PUPS_MOTHER,
  ACTOR_PUPS_BROTHER,
  /// A werewolf who climbed Air's Rock for Maha's powers and did not gain
  /// them. His event is GarohInteriors_AskAboutReveal.
  ACTOR_REVEAL_SEEKER,
  ACTOR_REVEAL_SEEKERS_MATE,
  ACTOR_WEAPON_MERCHANT,
  ACTOR_ARMOR_MERCHANT,
  ACTOR_INNKEEPER, ///< GarohInteriors_TalkToInnkeeper checks the party in
  /// The one who greets the party the morning after.
  ACTOR_INN_ATTENDANT,
  /// A traveler who climbed Air's Rock and found nothing, asleep at night.
  ACTOR_DISAPPOINTED_TRAVELER,
  /// His companion, still puzzling over the rock that would not spin.
  ACTOR_STUBBORN_TRAVELER,
  /// Placed beside the two travelers at night and taken away in the morning;
  /// talking to them wakes the travelers.
  /// XXX: what sprite 489 shows over the sleeping travelers; a render of the
  /// inn at night would settle it.
  ACTOR_DISAPPOINTED_TRAVELERS_BED,
  ACTOR_STUBBORN_TRAVELERS_BED
};

/// The item merchant's house's actors, numbering the rows of
/// gGarohInteriors_HouseActorPlacements.
enum GarohHouseActorId {
  ACTOR_AIRS_ROCK_GUARDIAN = ACTOR_FIRST_PLACED,
  ACTOR_WHIRLWIND_TALKER,
  ACTOR_WIND_STONE_TALKER,
  ACTOR_ITEM_MERCHANT
};

/// The sanctum's healer, its one actor.
enum GarohSanctumActorId { ACTOR_HEALER = ACTOR_FIRST_PLACED };

extern struct ActorPlacement gGarohInteriors_NoActorPlacements[];
extern struct ActorPlacement gGarohInteriors_InnActorPlacements[];
extern struct ActorPlacement gGarohInteriors_HouseActorPlacements[];
extern struct ActorPlacement gGarohInteriors_SanctumActorPlacements[];
extern struct Trigger gGarohInteriors_DoorTriggers[];
extern struct Trigger gGarohInteriors_InnNightTriggers[];
extern struct Trigger gGarohInteriors_HouseNightTriggers[];
extern struct Trigger gGarohInteriors_SanctumTriggers[];
extern struct Trigger gGarohInteriors_InnMorningTriggers[];
extern struct Trigger gGarohInteriors_HouseMorningTriggers[];

void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void MapGrid_CenterOnFollowedPoint(void);
void ActorSystem_SuspendProjectionAndShowSprites(void);
void ActorSystem_ResumeProjectionAndHideLayers(void);
void Event_End(void);
void Shop_Run(s32 slot, s32 actor);
void Sanctum_Run(s32 id);
void Inn_CheckIn(s32 slot, s32 actor);

void GarohInteriors_WakeAtInn(void);

void MapGrid_SetShownLayers(s32 value);

void FieldEffect_StartBlankOverlay(s32 value);

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  s16 scene = gMainRoutineWork.next;

  if (scene == SCENE_GAROH_INTERIORS_HOUSES)
    return gGarohInteriors_InnActorPlacements;
  if (scene == SCENE_GAROH_INTERIORS_SHOP)
    return gGarohInteriors_HouseActorPlacements;
  if (scene == SCENE_GAROH_INTERIORS_SANCTUM)
    return gGarohInteriors_SanctumActorPlacements;
  return gGarohInteriors_NoActorPlacements;
}

/// Garoh's shops, as Shop_Run numbers them.
enum { SHOP_GAROH_WEAPONS = 10, SHOP_GAROH_ARMOR = 11, SHOP_GAROH_ITEMS = 12 };

/// Garoh's inn, as Inn_CheckIn numbers the inns.
enum { GAROH_INN = 3 };

/// The weapon merchant: across the counter, once the secret is shared, his
/// shop; else what he says.
void GarohInteriors_TalkToWeaponMerchant(s32 actor) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);

  u16 facing = FACING_NEAREST_CARDINAL(leader->facing);

  if (facing == FACING_NORTH && GameFlag_Get(FLAG_GAROH_SECRET_SHARED)) {
    Shop_Run(SHOP_GAROH_WEAPONS, actor);
  } else {
    if (GameFlag_Get(FLAG_GAROH_SECRET_SHARED))
      // "What do you think? I'll bet these weapons suit you better than any
      // fangs would!"
      Event_SetMessageId((s32)MsgGarohInteriorsWhatDoYouThinkIll);
    else
      // "I'm sorry, but on nights of the full moon, we only sell claws and
      // fangs."
      Event_SetMessageId((s32)MsgGarohInteriorsImSorryButOnNights);
    // "I'm sorry, but on nights of the full moon, we only sell claws and..."
    Event_ShowMessageAndWait(actor, 0);
  }
}

/// The armor merchant: across the counter, once the secret is shared, his
/// shop; else what he says.
void GarohInteriors_TalkToArmorMerchant(s32 actor) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);

  u16 facing = FACING_NEAREST_CARDINAL(leader->facing);

  if (facing == FACING_NORTH && GameFlag_Get(FLAG_GAROH_SECRET_SHARED)) {
    Shop_Run(SHOP_GAROH_ARMOR, actor);
  } else {
    if (GameFlag_Get(FLAG_GAROH_SECRET_SHARED))
      // "When we change form, it's best to be unclothed."
      Event_SetMessageId((s32)MsgGarohInteriorsWhenWeChangeFormIts);
    else
      // "Unfortunate that you should come on a night of the full moon, sir..."
      Event_SetMessageId((s32)MsgGarohInteriorsUnfortunateThatYouShouldCome);
    // "Unfortunate that you should come on a night of the full moon, sir......"
    Event_ShowMessageAndWait(actor, 0);
  }
}

/// The item merchant: across the counter, his shop, night or day; else his
/// talk, at night a question about the way the party came.
void GarohInteriors_TalkToItemMerchant(s32 actor) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);

  u16 facing = FACING_NEAREST_CARDINAL(leader->facing);

  if (facing == FACING_NORTH) {
    Shop_Run(SHOP_GAROH_ITEMS, actor);
  } else if (GameFlag_Get(FLAG_GAROH_SECRET_SHARED)) {
    // "I've heard stories about other mountains like our own Air's Rock..."
    Event_SetMessageId((s32)MsgGarohInteriorsIveHeardStoriesAboutOther);
    // "I've heard stories about other mountains like our own Air's Rock......"
    Event_ShowMessageAndWait(actor, 0);
  } else {
    s32 message = (s32)MsgGarohInteriorsDidYouComeHereFrom;

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

/// The innkeeper: across the counter, the inn; else what he says.
void GarohInteriors_TalkToInnkeeper(s32 actor) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);

  u16 facing = FACING_NEAREST_CARDINAL(leader->facing);

  if (facing == FACING_NORTH) {
    Inn_CheckIn(GAROH_INN, actor);
  } else {
    if (GameFlag_Get(FLAG_GAROH_SECRET_SHARED))
      // "If you can pass the tests of Air's Rock, you can do anything!"
      Event_SetMessageId((s32)MsgGarohInteriorsIfYouCanPassThe);
    else
      // "A clear night around here always means fair weather on the morrow..."
      Event_SetMessageId((s32)MsgGarohInteriorsAClearNightAroundHere);
    // "A clear night around here always means fair weather on the morrow......"
    Event_ShowMessageAndWait(actor, 0);
  }
}

/// The healer turns to the leader: across the counter, the sanctum; else what
/// he says. He then faces south again.
void GarohInteriors_TalkToHealer(void) {
  s16 facing;

  facing = FACING_NEAREST_CARDINAL(
      Actor_GetById(gMainRoutineWork.leaderActorId)->facing);
  Actor_FaceActorById(ACTOR_HEALER, gMainRoutineWork.leaderActorId, 0);
  if ((u16)facing == FACING_NORTH) {
    Sanctum_Run(ACTOR_HEALER);
  } else {
    if (GameFlag_Get(FLAG_GAROH_SECRET_SHARED))
      // "I've heard about you. They say you're special, like the people in this
      // town."
      Event_SetMessageId((s32)MsgGarohInteriorsIveHeardAboutYouThey);
    else
      // "The people here may be different, but they certainly are nice..."
      Event_SetMessageId((s32)MsgGarohInteriorsThePeopleHereMayBe);
    // "The people here may be different, but they certainly are nice... And..."
    Event_ShowMessageAndWait(ACTOR_HEALER, 0);
  }
  Actor_FaceDirection(ACTOR_HEALER, FACING_SOUTH, 0);
}

/// The young werewolf's father asks the party, at night, what they are
/// looking for, and then whether they are searching for werewolves.
void GarohInteriors_AskWhatTheWanderersSeek(s32 actor) {
  s32 message;

  Event_Start();
  Event_ResolvePendingActions(0);
  // "Good evening, wanderers! Are you looking for something?"
  message = (s32)MsgGarohInteriorsGoodEveningWanderersAreYou;
  Event_SetMessageId(message);
  // "Good evening, wanderers! Are you looking for something?"
  FieldMessage_ShowAtActor(actor, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.leaderActorId, 0) ==
      0) {
    // "Searching for werewolves, eh?"
    Event_SetMessageId(message + 2);
    // "Searching for werewolves, eh?"
    FieldMessage_ShowAtActor(actor, 0);
    if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.leaderActorId, 0) ==
        0) {
      // "You don't really believe they exist, do you? If so, you'd best..."
      Event_ShowMessageAndWait(actor, 0);
    } else {
      // Past "You don't really believe they exist, do you?" to the next line.
      gFieldWork->message++;
      // "Hehrrr... Sorry. Just thought I'd ask. We get some crazy folk..."
      Event_ShowMessageAndWait(actor, 0);
    }
  } else {
    // "Just...staying up late, eh? Can't say I blame you, on a night like..."
    Event_ShowMessageAndWait(actor, 0);
  }
  Event_End();
}

/// Talking to the disappointed traveler, asleep at night.
void GarohInteriors_TalkToDisappointedTraveler(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  if (GameFlag_Get(FLAG_GAROH_SECRET_SHARED))
    // "Those wolves were really noisy last night, too."
    Event_SetMessageId((s32)MsgGarohInteriorsThoseWolvesWereReallyNoisy);
  else
    // "Zzzz... Zzzz... What a waste... Just a big rock..."
    Event_SetMessageId((s32)MsgGarohInteriorsZzzzZzzzWhatAWaste);
  // "Zzzz... Zzzz... What a waste... Just a big rock..."
  Event_ShowMessageAndWait(ACTOR_DISAPPOINTED_TRAVELER, 0);
  Event_End();
}

/// Talking to the stubborn traveler, asleep at night.
void GarohInteriors_TalkToStubbornTraveler(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  if (GameFlag_Get(FLAG_GAROH_SECRET_SHARED))
    // "Maybe we should stay and try climbing Air's Rock one more time."
    Event_SetMessageId((s32)MsgGarohInteriorsMaybeWeShouldStayAnd);
  else
    // "Zzz... Whuzza? Whirlwind? Wind Stone? Zzzzrrrgg..."
    Event_SetMessageId((s32)MsgGarohInteriorsZzzWhuzzaWhirlwindWindStone);
  // "Zzz... Whuzza? Whirlwind? Wind Stone? Zzzzrrrgg..."
  Event_ShowMessageAndWait(ACTOR_STUBBORN_TRAVELER, 0);
  Event_End();
}

/// Mind Read on the disappointed traveler.
void GarohInteriors_ReadDisappointedTravelersMind(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  if (GameFlag_Get(FLAG_GAROH_SECRET_SHARED))
    // "I'm scared of wolves, so I don't go out much at night..."
    Event_SetMessageId((s32)MsgGarohInteriors64);
  else
    // "...Climbed all over that stupid rock, and for what? Nothing!"
    Event_SetMessageId((s32)MsgGarohInteriorsClimbedAllOverThatStupid);
  // "...Climbed all over that stupid rock, and for what? Nothing!"
  Event_ShowMessageAndWait(ACTOR_DISAPPOINTED_TRAVELER, 0);
  Event_End();
}

/// Mind Read on the stubborn traveler.
void GarohInteriors_ReadStubbornTravelersMind(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  if (GameFlag_Get(FLAG_GAROH_SECRET_SHARED))
    // "If I used a lever, I could probably make that rock spin..."
    Event_SetMessageId((s32)MsgGarohInteriorsIfIUsedALever);
  else
    // "That rock was huge. How was I supposed to make it spin?"
    Event_SetMessageId((s32)MsgGarohInteriorsThatRockWasHugeHow);
  // "That rock was huge. How was I supposed to make it spin?"
  Event_ShowMessageAndWait(ACTOR_STUBBORN_TRAVELER, 0);
  Event_End();
}

/// The Reveal seeker asks whether the party learned Reveal, once
/// (FLAG_GAROH_ASKED_ABOUT_REVEAL).
void GarohInteriors_AskAboutReveal(void) {
  s32 message = (s32)MsgGarohInteriorsYouIsItTrueThat;

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
  struct MainRoutineWork *party = &gMainRoutineWork;
  s32 houses = SCENE_GAROH_INTERIORS_HOUSES;
  s32 scene;

  if (shared) {
    scene = party->next;
    if (scene == houses)
      return gGarohInteriors_InnMorningTriggers;
    if (scene == SCENE_GAROH_INTERIORS_SHOP)
      return gGarohInteriors_HouseMorningTriggers;
  } else {
    scene = party->next;
    if (scene == houses) {
      if (party->entrance == ENTRANCE_WAKE_AT_INN)
        return gGarohInteriors_InnMorningTriggers;
      return gGarohInteriors_InnNightTriggers;
    }
    if (scene == SCENE_GAROH_INTERIORS_SHOP)
      return gGarohInteriors_HouseNightTriggers;
  }
  if (scene == SCENE_GAROH_INTERIORS_SANCTUM)
    return gGarohInteriors_SanctumTriggers;
  return gGarohInteriors_DoorTriggers;
}

/// The map grid layers the rooms show at night (MapGrid_SetShownLayers): BG1
/// and BG3.
#define GAROH_INTERIORS_NIGHT_LAYERS (DISPCNT_BG1_ON | DISPCNT_BG3_ON)

/// Sets up the room being entered: the travelers in their beds at night, or
/// up once the secret is shared; waking at the inn, and night's layers.
s32 Scene_Initialize(void) {
  s32 turn;

  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
  Actor_GetById(gMainRoutineWork.leaderActorId)->priorityFlags |=
      ACTOR_PRIORITY_IGNORES_RISES;
  FieldEffect_StartBlankOverlay(0);
  if (gMainRoutineWork.next == SCENE_GAROH_INTERIORS_HOUSES) {
    if (GameFlag_Get(FLAG_GAROH_SECRET_SHARED) ||
        gMainRoutineWork.entrance == ENTRANCE_WAKE_AT_INN) {
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
    if (gMainRoutineWork.entrance == ENTRANCE_WAKE_AT_INN &&
        !GameFlag_Get(FLAG_GAROH_SECRET_SHARED))
      GarohInteriors_WakeAtInn();
  }
  if (GameFlag_Get(FLAG_GAROH_SECRET_SHARED))
    GameFlag_Clear(FLAG_GAROH_NIGHT);
  if (GameFlag_Get(FLAG_GAROH_NIGHT))
    MapGrid_SetShownLayers(GAROH_INTERIORS_NIGHT_LAYERS);
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

/// The morning after the night with Maha: the party wakes at the inn, the
/// secret is shared, and the inn's people say the stay is on the house.
void GarohInteriors_WakeAtInn(void) {
  GameFlag_Set(FLAG_GAROH_SECRET_SHARED);
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_Wait(30);
  ActorSystem_SuspendProjectionAndShowSprites();
  Sound_Play(MUS_INN);
  // "Felix and party stayed the night at the inn."
  FieldMessage_ShowCentered((s32)MsgGarohInteriorsAndPartyStayedTheNight, 0, 0);
  Event_Wait(20);
  ActorSystem_ResumeProjectionAndHideLayers();
  MapGrid_CenterOnFollowedPoint();
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  // "You folks didn't sleep much last night..."
  Event_SetMessageId((s32)MsgGarohInteriorsYouFolksDidntSleepMuch);
  // "You folks didn't sleep much last night... You don't look like you..."
  Event_ShowMessageThenWait(ACTOR_INN_ATTENDANT, 0, 10);
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
    SCENE_EXITS(GarohInteriorsHouses),
    SCENE_EXIT_UNLESS(1, Garoh, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, Garoh, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, Garoh, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(5, Garoh, 5, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(6, GarohTunnels, 7, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(7, GarohTunnels, 8, EXIT_FLAG_NONE),
    SCENE_EXITS(GarohInteriorsShop),
    SCENE_EXIT_UNLESS(3, GarohTunnels, 5, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, Garoh, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(5, GarohTunnels, 6, EXIT_FLAG_NONE),
    SCENE_EXITS(GarohInteriorsSanctum),
    SCENE_EXIT_UNLESS(2, Garoh, 6, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

// clang-format off
struct ActorPlacement gGarohInteriors_NoActorPlacements[] = {
    {SCENE_TABLE_END},
};

struct ActorPlacement gGarohInteriors_InnActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 336, 0, 120, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 376, 0, 104, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 360, 0, 136, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 568, 0, 136, FACING_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 592, 0, 96, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BALD_MAN_IN_RED, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 134, 0, 120, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(105, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 112, 0, 120, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_DARK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 424, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PINK_APRON, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 420, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WARRIOR_IN_GOLDEN_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 280, 0, 344, FACING_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WARRIOR_IN_GOLDEN_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 330, 0, 376, FACING_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 336, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 352, 0, 344, FACING_SOUTHWEST, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gGarohInteriors_HouseActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 372, 0, 406, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 424, 0, 424, FACING_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 352, 0, 456, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_PONYTAIL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 412, FACING_SOUTH, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gGarohInteriors_SanctumActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 448, 0, 152, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    {SCENE_TABLE_END},
};

struct Trigger gGarohInteriors_DoorTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_TABLE_END},
};
// clang-format on

struct Trigger gGarohInteriors_InnNightTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_TALK, ACTOR_PUPS_FATHER, CONDITION_ALWAYS,
     EVENT(GarohInteriors_AskWhatTheWanderersSeek)},
    {TRIGGER_TALK, ACTOR_PUPS_MOTHER, CONDITION_ALWAYS,
     MSGNO(MsgGarohInteriorsOurOldestSonIsAlways)},
    {TRIGGER_TALK, ACTOR_PUPS_BROTHER, CONDITION_ALWAYS,
     MSGNO(MsgGarohInteriorsNoFairMyBrotherAlways)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PUPS_FATHER, CONDITION_ALWAYS,
     MSGNO(MsgGarohInteriorsOutsidersCouldNeverUnderstandThe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PUPS_MOTHER, CONDITION_ALWAYS,
     MSGNO(MsgGarohInteriorsThatPupOfMineJust)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PUPS_BROTHER, CONDITION_ALWAYS,
     MSGNO(MsgGarohInteriorsWhatMadeMasterMahaTake)},
    {TRIGGER_TALK, ACTOR_REVEAL_SEEKER, CONDITION_ALWAYS,
     MSGNO(MsgGarohInteriorsWhenOurPeopleGrowOlder)},
    {TRIGGER_TALK, ACTOR_REVEAL_SEEKERS_MATE, CONDITION_ALWAYS,
     MSGNO(MsgGarohInteriorsMyMateIsCloseTo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_REVEAL_SEEKER,
     CONDITION_ALWAYS, MSGNO(MsgGarohInteriorsICastWhirlwindOnThe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_REVEAL_SEEKERS_MATE,
     CONDITION_ALWAYS, MSGNO(MsgGarohInteriors16)},
    {TRIGGER_TALK, ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS,
     EVENT(GarohInteriors_TalkToWeaponMerchant)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEAPON_MERCHANT,
     CONDITION_ALWAYS, MSGNO(MsgGarohInteriorsIShouldBeOutsideHowling)},
    {TRIGGER_TALK, ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS,
     EVENT(GarohInteriors_TalkToArmorMerchant)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARMOR_MERCHANT,
     CONDITION_ALWAYS, MSGNO(MsgGarohInteriorsTheseStrangersSeemNormalEnough)},
    {TRIGGER_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS,
     EVENT(GarohInteriors_TalkToInnkeeper)},
    {TRIGGER_TALK, ACTOR_INN_ATTENDANT, CONDITION_ALWAYS,
     MSGNO(MsgGarohInteriorsAirsRockCanGetQuite)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, CONDITION_ALWAYS,
     MSGNO(MsgGarohInteriorsItOnlyRainsAFew)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INN_ATTENDANT,
     CONDITION_ALWAYS, MSGNO(MsgGarohInteriorsMostPeopleSeeNothingOf)},
    {TRIGGER_TALK, ACTOR_DISAPPOINTED_TRAVELERS_BED, CONDITION_ALWAYS,
     EVENT(GarohInteriors_TalkToDisappointedTraveler)},
    {TRIGGER_TALK, ACTOR_STUBBORN_TRAVELERS_BED, CONDITION_ALWAYS,
     EVENT(GarohInteriors_TalkToStubbornTraveler)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_DISAPPOINTED_TRAVELERS_BED,
     CONDITION_ALWAYS, EVENT(GarohInteriors_ReadDisappointedTravelersMind)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUBBORN_TRAVELERS_BED,
     CONDITION_ALWAYS, EVENT(GarohInteriors_ReadStubbornTravelersMind)},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 209, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgGarohInteriorsEeeeewTheyreFryingUpBug))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 210, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgGarohInteriorsItsLambOnTheBone))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 212, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgGarohInteriorsNewtsSkeweredOverAnOpen))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 213, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgGarohInteriorsThereAreLotsOfStrange))},
    {TRIGGER_TABLE_END},
};

struct Trigger gGarohInteriors_HouseNightTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_TALK, ACTOR_AIRS_ROCK_GUARDIAN, CONDITION_ALWAYS,
     MSGNO(MsgGarohInteriorsGarohExistsToProtectAirs)},
    {TRIGGER_TALK, ACTOR_WHIRLWIND_TALKER, CONDITION_ALWAYS,
     MSGNO(MsgGarohInteriorsSoYouveHeardOfWhirlwind)},
    {TRIGGER_TALK, ACTOR_WIND_STONE_TALKER, CONDITION_ALWAYS,
     MSGNO(MsgGarohInteriorsIveHeardAboutTheWind)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_AIRS_ROCK_GUARDIAN,
     CONDITION_ALWAYS, MSGNO(MsgGarohInteriorsWeHaveOtherPowersThan)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WHIRLWIND_TALKER,
     CONDITION_ALWAYS, MSGNO(MsgGarohInteriorsAndDifferentPeopleUseWhirlwind)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WIND_STONE_TALKER,
     CONDITION_ALWAYS, MSGNO(MsgGarohInteriorsIWishIKnewWhat)},
    {TRIGGER_TALK, ACTOR_ITEM_MERCHANT, CONDITION_ALWAYS,
     EVENT(GarohInteriors_TalkToItemMerchant)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ITEM_MERCHANT,
     CONDITION_ALWAYS, MSGNO(MsgGarohInteriorsINeverChangeMyStock)},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 211, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgGarohInteriorsHuhWhatIsThatLobster))},
    {TRIGGER_TABLE_END},
};

struct Trigger gGarohInteriors_SanctumTriggers[] = {
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_TALK, ACTOR_HEALER, CONDITION_ALWAYS,
     EVENT(GarohInteriors_TalkToHealer)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER,
     FLAG_GAROH_SECRET_SHARED, MSGNO(MsgGarohInteriors32)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, CONDITION_ALWAYS,
     MSGNO(MsgGarohInteriorsAmITheOnlyOne)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, EVENT(GarohInteriors_TalkToHealer)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, FLAG_GAROH_SECRET_SHARED,
     MSGNO(MsgGarohInteriors32)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS,
     MSGNO(MsgGarohInteriorsAmITheOnlyOne)},
    {TRIGGER_TABLE_END},
};

struct Trigger gGarohInteriors_InnMorningTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_TALK, ACTOR_PUPS_FATHER, CONDITION_ALWAYS,
     MSGNO(MsgGarohInteriorsHeyItsYouThanksFor)},
    {TRIGGER_TALK, ACTOR_PUPS_MOTHER, CONDITION_ALWAYS,
     MSGNO(MsgGarohInteriorsEveryoneIsAmazedThatYou)},
    {TRIGGER_TALK, ACTOR_PUPS_BROTHER, CONDITION_ALWAYS,
     MSGNO(MsgGarohInteriorsMyBrotherShouldHaveGotten)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PUPS_FATHER, CONDITION_ALWAYS,
     MSGNO(MsgGarohInteriorsTheYoungerTheWerewolfThe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PUPS_MOTHER, CONDITION_ALWAYS,
     MSGNO(MsgGarohInteriorsOhThankGoodnessEverythingWorked)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PUPS_BROTHER, CONDITION_ALWAYS,
     MSGNO(MsgGarohInteriorsAndDadSaidHeWas)},
    {TRIGGER_TALK, ACTOR_REVEAL_SEEKER, FLAG_GAROH_ASKED_ABOUT_REVEAL,
     EVENT(GarohInteriors_AskAboutReveal)},
    {TRIGGER_TALK, ACTOR_REVEAL_SEEKER, CONDITION_ALWAYS,
     MSGNO(MsgGarohInteriorsToThinkIveWastedSo)},
    {TRIGGER_TALK, ACTOR_REVEAL_SEEKERS_MATE, CONDITION_ALWAYS,
     MSGNO(MsgGarohInteriorsHesWorkedSoHardBut)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_REVEAL_SEEKER,
     CONDITION_ALWAYS, MSGNO(MsgGarohInteriorsIThoughtICouldBecome)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_REVEAL_SEEKERS_MATE,
     CONDITION_ALWAYS, MSGNO(MsgGarohInteriorsSoNowEvenOutsidersAre)},
    {TRIGGER_TALK, ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS,
     EVENT(GarohInteriors_TalkToWeaponMerchant)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEAPON_MERCHANT,
     CONDITION_ALWAYS, MSGNO(MsgGarohInteriorsIPreferTheToothAnd)},
    {TRIGGER_TALK, ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS,
     EVENT(GarohInteriors_TalkToArmorMerchant)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARMOR_MERCHANT,
     CONDITION_ALWAYS, MSGNO(MsgGarohInteriorsMahaSaysThatWhenWere)},
    {TRIGGER_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS,
     EVENT(GarohInteriors_TalkToInnkeeper)},
    {TRIGGER_TALK, ACTOR_INN_ATTENDANT, CONDITION_ALWAYS,
     MSGNO(MsgGarohInteriorsMahaCameAlongAndSaid)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, CONDITION_ALWAYS,
     MSGNO(MsgGarohInteriorsIveHeardStoriesAboutOtherPlaces)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INN_ATTENDANT,
     CONDITION_ALWAYS, MSGNO(MsgGarohInteriorsHeSaidIfTheyWere)},
    {TRIGGER_TALK, ACTOR_DISAPPOINTED_TRAVELER, CONDITION_ALWAYS,
     EVENT(GarohInteriors_TalkToDisappointedTraveler)},
    {TRIGGER_TALK, ACTOR_STUBBORN_TRAVELER, CONDITION_ALWAYS,
     EVENT(GarohInteriors_TalkToStubbornTraveler)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_DISAPPOINTED_TRAVELER,
     CONDITION_ALWAYS, EVENT(GarohInteriors_ReadDisappointedTravelersMind)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUBBORN_TRAVELER,
     CONDITION_ALWAYS, EVENT(GarohInteriors_ReadStubbornTravelersMind)},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 209, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgGarohInteriorsEeeeewTheyreFryingUpBug))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 210, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgGarohInteriorsItsLambOnTheBone))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 212, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgGarohInteriorsNewtsSkeweredOverAnOpen))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 213, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgGarohInteriorsThereAreLotsOfStrange))},
    {TRIGGER_TABLE_END},
};

struct Trigger gGarohInteriors_HouseMorningTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_TALK, ACTOR_AIRS_ROCK_GUARDIAN, CONDITION_ALWAYS,
     MSGNO(MsgGarohInteriorsSoYoureSomethingSpecialAn)},
    {TRIGGER_TALK, ACTOR_WHIRLWIND_TALKER, CONDITION_ALWAYS,
     MSGNO(MsgGarohInteriorsYouLearnedRevealAtAirs)},
    {TRIGGER_TALK, ACTOR_WIND_STONE_TALKER, CONDITION_ALWAYS,
     MSGNO(MsgGarohInteriorsIJustCantBelieveThat)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_AIRS_ROCK_GUARDIAN,
     CONDITION_ALWAYS, MSGNO(MsgGarohInteriorsDidMahaMeanThatThey)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WHIRLWIND_TALKER,
     CONDITION_ALWAYS, MSGNO(MsgGarohInteriorsSoThesePeopleCanUse)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WIND_STONE_TALKER,
     CONDITION_ALWAYS, MSGNO(MsgGarohInteriorsNormallyOutsidersNeverGetIn)},
    {TRIGGER_TALK, ACTOR_ITEM_MERCHANT, CONDITION_ALWAYS,
     EVENT(GarohInteriors_TalkToItemMerchant)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ITEM_MERCHANT,
     CONDITION_ALWAYS, MSGNO(MsgGarohInteriorsIHeardThatOneThats)},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 211, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgGarohInteriorsHuhWhatIsThatLobster))},
    {TRIGGER_TABLE_END},
};
