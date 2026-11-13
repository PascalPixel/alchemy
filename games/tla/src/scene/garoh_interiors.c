/// @file
/// Garoh's houses, in three scenes that share this overlay: the inn, where
/// the weapon and armor merchants keep their counters (SceneId_Ov660Room1),
/// the house of the item merchant (SceneId_Ov660Room2) and the sanctum
/// (SceneId_Ov660Room3).
///
/// Garoh is first seen on a night of the full moon (FLAG_GAROH_NIGHT), when
/// its people hide that they are werewolves: the merchants sell only claws and
/// fangs, and two travelers who failed Air's Rock sleep at the inn. After the
/// night with Maha the party wakes at the inn (GarohInteriors_WakeAtInn,
/// entrance 99), the secret is shared (FLAG_GAROH_SECRET_SHARED) and the
/// rooms take their morning tables.

#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "flags.h"
#include "scene_tables.h"
#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "scene_ids.h"
#include "text/msg_ids.h"
#include "event_inline.h"

/// The inn's actors, numbering the rows of gGarohInteriors_InnActorPlacements.
enum GarohInnActorId {
  /// The father of the young werewolf who led the party to Maha. His event is
  /// GarohInteriors_AskWhatTheWanderersSeek.
  /// ★ Named from the game: "Thanks for looking out for our son last night!"
  /// (MsgGarohInteriors35)
  ACTOR_PUPS_FATHER = ACTOR_FIRST_PLACED,
  /// ★ Named from the game: "Our oldest son is always scampering off late at
  /// night... Where is he?" (MsgGarohInteriors02)
  ACTOR_PUPS_MOTHER,
  /// ★ Named from the game: "No fair! My brother always gets to go out late!"
  /// (MsgGarohInteriors03)
  ACTOR_PUPS_BROTHER,
  /// A werewolf who climbed Air's Rock for Maha's powers and did not gain
  /// them. His event is GarohInteriors_AskAboutReveal.
  /// ★ Named from the game: "I thought I could become a leader if I learned
  /// Reveal. How foolish of me." (MsgGarohInteriors50)
  ACTOR_REVEAL_SEEKER,
  /// ★ Named from the game: "My mate is close to obtaining the power he's
  /// been seeking..." (MsgGarohInteriors14)
  ACTOR_REVEAL_SEEKERS_MATE,
  /// ★ Named from the game: "I'll bet these weapons suit you better than any
  /// fangs would!" (MsgGarohInteriors52)
  ACTOR_WEAPON_MERCHANT,
  /// ★ Named from the game: "But when you're human, armor always comes in
  /// handy." (MsgGarohInteriors54)
  ACTOR_ARMOR_MERCHANT,
  ACTOR_INNKEEPER, ///< GarohInteriors_TalkToInnkeeper checks the party in
  /// The one who greets the party the morning after.
  /// ★ Named from the game: "You folks didn't sleep much last night..."
  /// (MsgGarohInteriors34)
  ACTOR_INN_ATTENDANT,
  /// A traveler who climbed Air's Rock and found nothing, asleep at night.
  /// ★ Named from the game: "...Climbed all over that stupid rock, and for
  /// what? Nothing!" (MsgGarohInteriors29)
  ACTOR_DISAPPOINTED_TRAVELER,
  /// His companion, still puzzling over the rock that would not spin.
  /// ★ Named from the game: "Maybe we should stay and try climbing Air's Rock
  /// one more time." (MsgGarohInteriors61)
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
  /// ★ Named from the game: "Garoh exists to protect Air's Rock."
  /// (MsgGarohInteriors07)
  ACTOR_AIRS_ROCK_GUARDIAN = ACTOR_FIRST_PLACED,
  /// ★ Named from the game: "So you've heard of Whirlwind? Most people
  /// haven't..." (MsgGarohInteriors08)
  ACTOR_WHIRLWIND_TALKER,
  /// ★ Named from the game: "I've heard about the Wind Stone..."
  /// (MsgGarohInteriors09)
  ACTOR_WIND_STONE_TALKER,
  /// ★ Named from the game: "I never change my stock, unlike the weapons and
  /// armor shopkeeps..." (MsgGarohInteriors22)
  ACTOR_ITEM_MERCHANT
};

/// The sanctum's healer, its one actor.
enum GarohSanctumActorId { ACTOR_HEALER = ACTOR_FIRST_PLACED };

struct EventWork1c4 {
  u8 unk000[0x1c4];
  u16 count;
};

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
void Map_CenterOnFollowedPoint(void);
void Graphics_EnableObjLayerAndCallbacks(void);
void ActorSystem_ResumeProjectionAndHideLayers(void);
void Event_End(void);
void Shop_Run(s32 slot, s32 actor);
void Sanctum_Run(s32 id);
void Inn_CheckIn(s32 slot, s32 actor);
#include "event_inline.h"
#include "module/scene/garoh_interiors.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov660Room1)
    return gGarohInteriors_InnActorPlacements;
  if (scene == (s32)&SceneId_Ov660Room2)
    return gGarohInteriors_HouseActorPlacements;
  if (scene == (s32)&SceneId_Ov660Room3)
    return gGarohInteriors_SanctumActorPlacements;
  return gGarohInteriors_NoActorPlacements;
}

/// The weapon merchant: across the counter, once the secret is shared, his
/// shop; else what he says.
void GarohInteriors_TalkToWeaponMerchant(s32 actor) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);

  u16 facing = (leader->facing + 0x2000) & ~0x3fff;

  if (facing == FACING_NORTH && GameFlag_Test(FLAG_GAROH_SECRET_SHARED)) {
    Shop_Run(10, actor);
  } else {
    if (GameFlag_Test(FLAG_GAROH_SECRET_SHARED))
      // "What do you think? I'll bet these weapons suit you better than any
      // fangs would!"
      Event_SetMessageId((s32)MsgGarohInteriors52);
    else
      // "I'm sorry, but on nights of the full moon, we only sell claws and
      // fangs."
      Event_SetMessageId((s32)MsgGarohInteriors17);
    Event_ShowMessageAndWait(actor, 0);
  }
}

/// The armor merchant: across the counter, once the secret is shared, his
/// shop; else what he says.
void GarohInteriors_TalkToArmorMerchant(s32 actor) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);

  u16 facing = (leader->facing + 0x2000) & ~0x3fff;

  if (facing == FACING_NORTH && GameFlag_Test(FLAG_GAROH_SECRET_SHARED)) {
    Shop_Run(11, actor);
  } else {
    if (GameFlag_Test(FLAG_GAROH_SECRET_SHARED))
      // "When we change form, it's best to be unclothed."
      Event_SetMessageId((s32)MsgGarohInteriors54);
    else
      // "Unfortunate that you should come on a night of the full moon, sir..."
      Event_SetMessageId((s32)MsgGarohInteriors19);
    Event_ShowMessageAndWait(actor, 0);
  }
}

/// The item merchant: across the counter, his shop, night or day; else his
/// talk, at night a question about the way the party came.
void GarohInteriors_TalkToItemMerchant(s32 actor) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);

  u16 facing = (leader->facing + 0x2000) & ~0x3fff;

  if (facing == FACING_NORTH) {
    Shop_Run(12, actor);
  } else if (GameFlag_Test(FLAG_GAROH_SECRET_SHARED)) {
    // "I've heard stories about other mountains like our own Air's Rock..."
    Event_SetMessageId((s32)MsgGarohInteriors56);
    Event_ShowMessageAndWait(actor, 0);
  } else {
    s32 message = (s32)MsgGarohInteriors21;

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
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);

  u16 facing = (leader->facing + 0x2000) & ~0x3fff;

  if (facing == FACING_NORTH) {
    Inn_CheckIn(3, actor);
  } else {
    if (GameFlag_Test(FLAG_GAROH_SECRET_SHARED))
      // "If you can pass the tests of Air's Rock, you can do anything!"
      Event_SetMessageId((s32)MsgGarohInteriors58);
    else
      // "A clear night around here always means fair weather on the morrow..."
      Event_SetMessageId((s32)MsgGarohInteriors23);
    Event_ShowMessageAndWait(actor, 0);
  }
}

/// The healer turns to the leader: across the counter, the sanctum; else what
/// he says. He then faces south again.
void GarohInteriors_TalkToHealer(void) {
  s16 facing;

  facing = ((Actor_GetById(gMainRoutineWork.currentOwner)->facing) + 0x2000) &
           ~0x3fff;
  Actor_SetAngleToward(ACTOR_HEALER, gMainRoutineWork.currentOwner, 0);
  if ((u16)facing == FACING_NORTH) {
    Sanctum_Run(ACTOR_HEALER);
  } else {
    if (GameFlag_Test(FLAG_GAROH_SECRET_SHARED))
      // "I've heard about you. They say you're special, like the people in this
      // town."
      Event_SetMessageId((s32)MsgGarohInteriors66);
    else
      // "The people here may be different, but they certainly are nice..."
      Event_SetMessageId((s32)MsgGarohInteriors31);
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
  message = (s32)MsgGarohInteriors01;
  Event_SetMessageId(message);
  FieldMessage_ShowAtActor(actor, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 0) {
    // "Searching for werewolves, eh?"
    Event_SetMessageId(message + 2);
    FieldMessage_ShowAtActor(actor, 0);
    if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) ==
        0) {
      Event_ShowMessageAndWait(actor, 0);
    } else {
      ((struct EventWork1c4 *)Ram_HeapSlots->eventWork)->count++;
      Event_ShowMessageAndWait(actor, 0);
    }
  } else {
    Event_ShowMessageAndWait(actor, 0);
  }
  Event_End();
}

/// Talking to the disappointed traveler, asleep at night.
void GarohInteriors_TalkToDisappointedTraveler(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  if (GameFlag_Test(FLAG_GAROH_SECRET_SHARED))
    // "Those wolves were really noisy last night, too."
    Event_SetMessageId((s32)MsgGarohInteriors60);
  else
    // "Zzzz... Zzzz... What a waste... Just a big rock..."
    Event_SetMessageId((s32)MsgGarohInteriors25);
  Event_ShowMessageAndWait(ACTOR_DISAPPOINTED_TRAVELER, 0);
  Event_End();
}

/// Talking to the stubborn traveler, asleep at night.
void GarohInteriors_TalkToStubbornTraveler(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  if (GameFlag_Test(FLAG_GAROH_SECRET_SHARED))
    // "Maybe we should stay and try climbing Air's Rock one more time."
    Event_SetMessageId((s32)MsgGarohInteriors61);
  else
    // "Zzz... Whuzza? Whirlwind? Wind Stone? Zzzzrrrgg..."
    Event_SetMessageId((s32)MsgGarohInteriors26);
  Event_ShowMessageAndWait(ACTOR_STUBBORN_TRAVELER, 0);
  Event_End();
}

/// Mind Read on the disappointed traveler.
void GarohInteriors_ReadDisappointedTravelersMind(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  if (GameFlag_Test(FLAG_GAROH_SECRET_SHARED))
    // "I'm scared of wolves, so I don't go out much at night..."
    Event_SetMessageId((s32)MsgGarohInteriors64);
  else
    // "...Climbed all over that stupid rock, and for what? Nothing!"
    Event_SetMessageId((s32)MsgGarohInteriors29);
  Event_ShowMessageAndWait(ACTOR_DISAPPOINTED_TRAVELER, 0);
  Event_End();
}

/// Mind Read on the stubborn traveler.
void GarohInteriors_ReadStubbornTravelersMind(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  if (GameFlag_Test(FLAG_GAROH_SECRET_SHARED))
    // "If I used a lever, I could probably make that rock spin..."
    Event_SetMessageId((s32)MsgGarohInteriors65);
  else
    // "That rock was huge. How was I supposed to make it spin?"
    Event_SetMessageId((s32)MsgGarohInteriors30);
  Event_ShowMessageAndWait(ACTOR_STUBBORN_TRAVELER, 0);
  Event_End();
}

/// The Reveal seeker asks whether the party learned Reveal, once
/// (FLAG_GAROH_ASKED_ABOUT_REVEAL).
void GarohInteriors_AskAboutReveal(void) {
  s32 message = (s32)MsgGarohInteriors47;

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
  GameFlag_SetBit(FLAG_GAROH_ASKED_ABOUT_REVEAL);
}

/// Returns the room's triggers: the night's until the secret is shared, the
/// morning's after, and the inn's morning ones as the party wakes there.
const struct Trigger *Scene_GetEvents(void) {
  s32 shared = GameFlag_Test(FLAG_GAROH_SECRET_SHARED);
  struct MainRoutineWork *party = &gMainRoutineWork;
  s32 room1 = (s32)&SceneId_Ov660Room1;
  s32 scene;

  if (shared) {
    scene = party->scene;
    if (scene == room1)
      return gGarohInteriors_InnMorningTriggers;
    if (scene == (s32)&SceneId_Ov660Room2)
      return gGarohInteriors_HouseMorningTriggers;
  } else {
    scene = party->scene;
    if (scene == room1) {
      if (party->entrance == 99)
        return gGarohInteriors_InnMorningTriggers;
      return gGarohInteriors_InnNightTriggers;
    }
    if (scene == (s32)&SceneId_Ov660Room2)
      return gGarohInteriors_HouseNightTriggers;
  }
  if (scene == (s32)&SceneId_Ov660Room3)
    return gGarohInteriors_SanctumTriggers;
  return gGarohInteriors_DoorTriggers;
}

s32 Scene_Initialize(void) {
  s32 turn;

  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 521;
  Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 32;
  Func_0200087c(0);
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov660Room1) {
    if (GameFlag_Test(FLAG_GAROH_SECRET_SHARED) ||
        gMainRoutineWork.entrance == 99) {
      Actor_SetHorizontalPositionWithTerrain(ACTOR_DISAPPOINTED_TRAVELERS_BED,
                                             0, 0);
      Actor_SetHorizontalPositionWithTerrain(ACTOR_STUBBORN_TRAVELERS_BED, 0,
                                             0);
      Actor_SetMovementTypeById(ACTOR_STUBBORN_TRAVELER, 3);
    } else {
      {
        s32 x;
        s32 z;
        s32 id;

        turn = FACING_SOUTHEAST;
        x = 153;
        id = ACTOR_DISAPPOINTED_TRAVELER;
        /* FAKEMATCH(barrier): the forced id keeps its load right after that
         * half; as no asm, 2 of 158 instructions differ in tla-en. */
        asm volatile("" : "+l"(id));
        x <<= 17;
        z = 335 << 16;
        Actor_SetHorizontalPositionAndAngleWithTerrain(id, x, z, turn);
      }
      {
        s32 angle = FACING_SOUTHWEST;
        s32 z = 336 << 16;

        Actor_SetHorizontalPositionAndAngleWithTerrain(ACTOR_STUBBORN_TRAVELER,
                                                       345 << 16, z, angle);
      }
      Actor_SetSingleChildFlags(Actor_GetById(ACTOR_DISAPPOINTED_TRAVELERS_BED),
                                0);
      Actor_SetSingleChildFlags(Actor_GetById(ACTOR_STUBBORN_TRAVELERS_BED), 0);
      Actor_GetById(ACTOR_DISAPPOINTED_TRAVELER)->motionFlags = 0;
      Actor_GetById(ACTOR_DISAPPOINTED_TRAVELER)->y = 0;
      Actor_GetById(ACTOR_STUBBORN_TRAVELER)->motionFlags = 0;
      Actor_GetById(ACTOR_STUBBORN_TRAVELER)->y = 0;
      Actor_GetById(ACTOR_DISAPPOINTED_TRAVELERS_BED)->motionFlags = 0;
      Actor_GetById(ACTOR_DISAPPOINTED_TRAVELERS_BED)->y = 0;
      Actor_GetById(ACTOR_STUBBORN_TRAVELERS_BED)->motionFlags = 0;
      Actor_GetById(ACTOR_STUBBORN_TRAVELERS_BED)->y = 0;
      Func_080d3928(ACTOR_DISAPPOINTED_TRAVELER, 0xe000);
      Actor_SetSingleChildFlags(Actor_GetById(ACTOR_DISAPPOINTED_TRAVELER), 0);
      Func_080d3928(ACTOR_STUBBORN_TRAVELER, turn);
      Actor_SetSingleChildFlags(Actor_GetById(ACTOR_STUBBORN_TRAVELER), 0);
    }
    if (gMainRoutineWork.entrance == 99 &&
        !GameFlag_Test(FLAG_GAROH_SECRET_SHARED))
      GarohInteriors_WakeAtInn();
  }
  if (GameFlag_Test(FLAG_GAROH_SECRET_SHARED))
    GameFlag_ClearBit(FLAG_GAROH_NIGHT);
  if (GameFlag_Test(FLAG_GAROH_NIGHT))
    Map_SetShownLayers(0xa00);
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

/// The morning after the night with Maha: the party wakes at the inn, the
/// secret is shared, and the inn's people say the stay is on the house.
void GarohInteriors_WakeAtInn(void) {
  GameFlag_SetBit(FLAG_GAROH_SECRET_SHARED);
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_Wait(30);
  Graphics_EnableObjLayerAndCallbacks();
  Sound_Play(86);
  // "Felix and party stayed the night at the inn."
  Func_0803ce64((s32)MsgGarohInteriors33, 0, 0);
  Event_Wait(20);
  ActorSystem_ResumeProjectionAndHideLayers();
  Map_CenterOnFollowedPoint();
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  // "You folks didn't sleep much last night..."
  Event_SetMessageId((s32)MsgGarohInteriors34);
  Event_ShowMessageThenWait(ACTOR_INN_ATTENDANT, 0, 10);
  Motion_SetVarCbAndRefresh(ACTOR_INNKEEPER, 2);
  Event_Wait(20);
  // "Master Maha told us all about you. Don't you worry about the bill. It's
  // on us."
  Event_ShowMessageThenWait(ACTOR_INNKEEPER, 0, 10);
  Motion_SetModeAndWaitAnimation(ACTOR_INNKEEPER, 3);
  Event_Wait(30);
  // "Farewell, friends."
  Event_ShowMessageThenWait(ACTOR_INNKEEPER, 0, 10);
  Event_End();
}

#include "text/msg_ids.h"

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(64),
    SCENE_EXIT_IF(1, 59, 1, -1),
    SCENE_EXIT_IF(3, 59, 3, -1),
    SCENE_EXIT_IF(4, 59, 4, -1),
    SCENE_EXIT_IF(5, 59, 5, -1),
    SCENE_EXIT_IF(6, 62, 7, -1),
    SCENE_EXIT_IF(7, 62, 8, -1),
    SCENE_EXITS(65),
    SCENE_EXIT_IF(3, 62, 5, -1),
    SCENE_EXIT_IF(4, 59, 2, -1),
    SCENE_EXIT_IF(5, 62, 6, -1),
    SCENE_EXITS(66),
    SCENE_EXIT_IF(2, 59, 6, -1),
    SCENE_EXITS_END,
};

// clang-format off
struct ActorPlacement gGarohInteriors_NoActorPlacements[] = {
    {SCENE_TABLE_END},
};

struct ActorPlacement gGarohInteriors_InnActorPlacements[] = {
    {192, CONDITION_ALWAYS, 2, PIXELS(336), 0, PIXELS(120), FACING_SOUTH, 0, 0},
    {190, CONDITION_ALWAYS, 1, PIXELS(376), 0, PIXELS(104), FACING_WEST, 0, 0},
    {191, CONDITION_ALWAYS, 1, PIXELS(360), 0, PIXELS(136), FACING_NORTH, 1, 0},
    {188, CONDITION_ALWAYS, 1, PIXELS(568), 0, PIXELS(136), FACING_SOUTHEAST, 0, 0},
    {194, CONDITION_ALWAYS, 1, PIXELS(592), 0, PIXELS(96), FACING_SOUTH, 1, 0},
    {205, CONDITION_ALWAYS, 1, PIXELS(134), 0, PIXELS(120), FACING_SOUTH, 1, 0},
    {105, CONDITION_ALWAYS, 1, PIXELS(112), 0, PIXELS(120), FACING_SOUTH, 1, 0},
    {199, CONDITION_ALWAYS, 1, PIXELS(248), 0, PIXELS(424), FACING_SOUTH, 1, 0},
    {209, CONDITION_ALWAYS, 1, PIXELS(264), 0, PIXELS(420), FACING_WEST, 0, 0},
    {239, CONDITION_ALWAYS, 1, PIXELS(280), 0, PIXELS(344), FACING_NORTHEAST, 1, 0},
    {239, CONDITION_ALWAYS, 1, PIXELS(330), 0, PIXELS(376), FACING_SOUTHWEST, 1, 0},
    {489, CONDITION_ALWAYS, 1, PIXELS(296), 0, PIXELS(336), FACING_SOUTH, 2, 0},
    {489, CONDITION_ALWAYS, 1, PIXELS(352), 0, PIXELS(344), FACING_SOUTHWEST, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gGarohInteriors_HouseActorPlacements[] = {
    {185, CONDITION_ALWAYS, 1, PIXELS(372), 0, PIXELS(406), FACING_SOUTHEAST, 1, 0},
    {196, CONDITION_ALWAYS, 1, PIXELS(424), 0, PIXELS(424), FACING_NORTHWEST, 0, 0},
    {186, CONDITION_ALWAYS, 1, PIXELS(352), 0, PIXELS(456), FACING_SOUTH, 1, 0},
    {198, CONDITION_ALWAYS, 1, PIXELS(200), 0, PIXELS(412), FACING_SOUTH, 0, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gGarohInteriors_SanctumActorPlacements[] = {
    {202, CONDITION_ALWAYS, 1, PIXELS(448), 0, PIXELS(152), FACING_SOUTH, 1, 0},
    {SCENE_TABLE_END},
};

struct Trigger gGarohInteriors_DoorTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGERS_END},
};
// clang-format on

struct Trigger gGarohInteriors_InnNightTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {0x31, 6, CONDITION_ALWAYS, 6},
    {0x31, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_TALK, ACTOR_PUPS_FATHER, CONDITION_ALWAYS,
     EVENT(GarohInteriors_AskWhatTheWanderersSeek)},
    {TRIGGER_TALK, ACTOR_PUPS_MOTHER, CONDITION_ALWAYS,
     MSGNO(MsgGarohInteriors02)},
    {TRIGGER_TALK, ACTOR_PUPS_BROTHER, CONDITION_ALWAYS,
     MSGNO(MsgGarohInteriors03)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PUPS_FATHER, CONDITION_ALWAYS,
     MSGNO(MsgGarohInteriors04)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PUPS_MOTHER, CONDITION_ALWAYS,
     MSGNO(MsgGarohInteriors05)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PUPS_BROTHER, CONDITION_ALWAYS,
     MSGNO(MsgGarohInteriors06)},
    {TRIGGER_TALK, ACTOR_REVEAL_SEEKER, CONDITION_ALWAYS,
     MSGNO(MsgGarohInteriors13)},
    {TRIGGER_TALK, ACTOR_REVEAL_SEEKERS_MATE, CONDITION_ALWAYS,
     MSGNO(MsgGarohInteriors14)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_REVEAL_SEEKER,
     CONDITION_ALWAYS, MSGNO(MsgGarohInteriors15)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_REVEAL_SEEKERS_MATE,
     CONDITION_ALWAYS, MSGNO(MsgGarohInteriors16)},
    {TRIGGER_TALK, ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS,
     EVENT(GarohInteriors_TalkToWeaponMerchant)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEAPON_MERCHANT,
     CONDITION_ALWAYS, MSGNO(MsgGarohInteriors18)},
    {TRIGGER_TALK, ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS,
     EVENT(GarohInteriors_TalkToArmorMerchant)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARMOR_MERCHANT,
     CONDITION_ALWAYS, MSGNO(MsgGarohInteriors20)},
    {TRIGGER_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS,
     EVENT(GarohInteriors_TalkToInnkeeper)},
    {TRIGGER_TALK, ACTOR_INN_ATTENDANT, CONDITION_ALWAYS,
     MSGNO(MsgGarohInteriors24)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, CONDITION_ALWAYS,
     MSGNO(MsgGarohInteriors27)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INN_ATTENDANT,
     CONDITION_ALWAYS, MSGNO(MsgGarohInteriors28)},
    {TRIGGER_TALK, ACTOR_DISAPPOINTED_TRAVELERS_BED, CONDITION_ALWAYS,
     EVENT(GarohInteriors_TalkToDisappointedTraveler)},
    {TRIGGER_TALK, ACTOR_STUBBORN_TRAVELERS_BED, CONDITION_ALWAYS,
     EVENT(GarohInteriors_TalkToStubbornTraveler)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_DISAPPOINTED_TRAVELERS_BED,
     CONDITION_ALWAYS, EVENT(GarohInteriors_ReadDisappointedTravelersMind)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUBBORN_TRAVELERS_BED,
     CONDITION_ALWAYS, EVENT(GarohInteriors_ReadStubbornTravelersMind)},
    {0x173, 209, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgGarohInteriors68))},
    {0x173, 210, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgGarohInteriors69))},
    {0x173, 212, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgGarohInteriors71))},
    {0x173, 213, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgGarohInteriors72))},
    {TRIGGERS_END},
};

struct Trigger gGarohInteriors_HouseNightTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {0x31, 5, CONDITION_ALWAYS, 5},
    {0x31, 6, CONDITION_ALWAYS, 6},
    {0x31, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_TALK, ACTOR_AIRS_ROCK_GUARDIAN, CONDITION_ALWAYS,
     MSGNO(MsgGarohInteriors07)},
    {TRIGGER_TALK, ACTOR_WHIRLWIND_TALKER, CONDITION_ALWAYS,
     MSGNO(MsgGarohInteriors08)},
    {TRIGGER_TALK, ACTOR_WIND_STONE_TALKER, CONDITION_ALWAYS,
     MSGNO(MsgGarohInteriors09)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_AIRS_ROCK_GUARDIAN,
     CONDITION_ALWAYS, MSGNO(MsgGarohInteriors10)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WHIRLWIND_TALKER,
     CONDITION_ALWAYS, MSGNO(MsgGarohInteriors11)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WIND_STONE_TALKER,
     CONDITION_ALWAYS, MSGNO(MsgGarohInteriors12)},
    {TRIGGER_TALK, ACTOR_ITEM_MERCHANT, CONDITION_ALWAYS,
     EVENT(GarohInteriors_TalkToItemMerchant)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ITEM_MERCHANT,
     CONDITION_ALWAYS, MSGNO(MsgGarohInteriors22)},
    {0x173, 211, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgGarohInteriors70))},
    {TRIGGERS_END},
};

struct Trigger gGarohInteriors_SanctumTriggers[] = {
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_TALK, ACTOR_HEALER, CONDITION_ALWAYS,
     EVENT(GarohInteriors_TalkToHealer)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER,
     FLAG_GAROH_SECRET_SHARED, MSGNO(MsgGarohInteriors32)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, CONDITION_ALWAYS,
     MSGNO(MsgGarohInteriors67)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, EVENT(GarohInteriors_TalkToHealer)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, FLAG_GAROH_SECRET_SHARED,
     MSGNO(MsgGarohInteriors32)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS,
     MSGNO(MsgGarohInteriors67)},
    {TRIGGERS_END},
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
     MSGNO(MsgGarohInteriors35)},
    {TRIGGER_TALK, ACTOR_PUPS_MOTHER, CONDITION_ALWAYS,
     MSGNO(MsgGarohInteriors36)},
    {TRIGGER_TALK, ACTOR_PUPS_BROTHER, CONDITION_ALWAYS,
     MSGNO(MsgGarohInteriors37)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PUPS_FATHER, CONDITION_ALWAYS,
     MSGNO(MsgGarohInteriors38)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PUPS_MOTHER, CONDITION_ALWAYS,
     MSGNO(MsgGarohInteriors39)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PUPS_BROTHER, CONDITION_ALWAYS,
     MSGNO(MsgGarohInteriors40)},
    {TRIGGER_TALK, ACTOR_REVEAL_SEEKER, FLAG_GAROH_ASKED_ABOUT_REVEAL,
     EVENT(GarohInteriors_AskAboutReveal)},
    {TRIGGER_TALK, ACTOR_REVEAL_SEEKER, CONDITION_ALWAYS,
     MSGNO(MsgGarohInteriors48)},
    {TRIGGER_TALK, ACTOR_REVEAL_SEEKERS_MATE, CONDITION_ALWAYS,
     MSGNO(MsgGarohInteriors49)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_REVEAL_SEEKER,
     CONDITION_ALWAYS, MSGNO(MsgGarohInteriors50)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_REVEAL_SEEKERS_MATE,
     CONDITION_ALWAYS, MSGNO(MsgGarohInteriors51)},
    {TRIGGER_TALK, ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS,
     EVENT(GarohInteriors_TalkToWeaponMerchant)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEAPON_MERCHANT,
     CONDITION_ALWAYS, MSGNO(MsgGarohInteriors53)},
    {TRIGGER_TALK, ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS,
     EVENT(GarohInteriors_TalkToArmorMerchant)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARMOR_MERCHANT,
     CONDITION_ALWAYS, MSGNO(MsgGarohInteriors55)},
    {TRIGGER_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS,
     EVENT(GarohInteriors_TalkToInnkeeper)},
    {TRIGGER_TALK, ACTOR_INN_ATTENDANT, CONDITION_ALWAYS,
     MSGNO(MsgGarohInteriors59)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, CONDITION_ALWAYS,
     MSGNO(MsgGarohInteriors62)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INN_ATTENDANT,
     CONDITION_ALWAYS, MSGNO(MsgGarohInteriors63)},
    {TRIGGER_TALK, ACTOR_DISAPPOINTED_TRAVELER, CONDITION_ALWAYS,
     EVENT(GarohInteriors_TalkToDisappointedTraveler)},
    {TRIGGER_TALK, ACTOR_STUBBORN_TRAVELER, CONDITION_ALWAYS,
     EVENT(GarohInteriors_TalkToStubbornTraveler)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_DISAPPOINTED_TRAVELER,
     CONDITION_ALWAYS, EVENT(GarohInteriors_ReadDisappointedTravelersMind)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUBBORN_TRAVELER,
     CONDITION_ALWAYS, EVENT(GarohInteriors_ReadStubbornTravelersMind)},
    {0x173, 209, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgGarohInteriors68))},
    {0x173, 210, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgGarohInteriors69))},
    {0x173, 212, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgGarohInteriors71))},
    {0x173, 213, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgGarohInteriors72))},
    {TRIGGERS_END},
};

struct Trigger gGarohInteriors_HouseMorningTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {0x31, 5, CONDITION_ALWAYS, 5},
    {0x31, 6, CONDITION_ALWAYS, 6},
    {0x31, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_TALK, ACTOR_AIRS_ROCK_GUARDIAN, CONDITION_ALWAYS,
     MSGNO(MsgGarohInteriors41)},
    {TRIGGER_TALK, ACTOR_WHIRLWIND_TALKER, CONDITION_ALWAYS,
     MSGNO(MsgGarohInteriors42)},
    {TRIGGER_TALK, ACTOR_WIND_STONE_TALKER, CONDITION_ALWAYS,
     MSGNO(MsgGarohInteriors43)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_AIRS_ROCK_GUARDIAN,
     CONDITION_ALWAYS, MSGNO(MsgGarohInteriors44)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WHIRLWIND_TALKER,
     CONDITION_ALWAYS, MSGNO(MsgGarohInteriors45)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WIND_STONE_TALKER,
     CONDITION_ALWAYS, MSGNO(MsgGarohInteriors46)},
    {TRIGGER_TALK, ACTOR_ITEM_MERCHANT, CONDITION_ALWAYS,
     EVENT(GarohInteriors_TalkToItemMerchant)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ITEM_MERCHANT,
     CONDITION_ALWAYS, MSGNO(MsgGarohInteriors57)},
    {0x173, 211, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgGarohInteriors70))},
    {TRIGGERS_END},
};
