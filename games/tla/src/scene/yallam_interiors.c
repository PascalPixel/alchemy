/// @file
/// Yallam's houses and shops, with Sunshine the blacksmith, who forges the
/// rare materials the party brings him into weapons and armor once rain has
/// let him reach Taopo Swamp.
#include "fixed_math.h"
#include "flags.h"
#include "scene_ids.h"
#include "types.h"
#include "version.h"
#include "battle/battle.h"
#include "field/actor_script.h"
#include "field/event_inline.h"
#include "field/field.h"
#include "field/field_sprite.h"
#include "field/field_work.h"
#include "field/scene_tables.h"
#include "game/game.h"
#include "game/inventory.h"
#include "graphics/graphics.h"
#include "menu/menu.h"
#include "scene/scene.h"
#include "sound/sound.h"
#include "system/frame.h"
#include "system/main_routine_work.h"
#include "system/math_table.h"
#include "system/ram_buffer.h"
#include "system/system.h"
#include "window/message_argument.h"
#include "text/msg_ids.h"

void MessageArgument_Push(u32 value, u32 kind);

/// The inn's actors (SceneId_YallamInteriorsInn), numbering the rows of
/// gYallamInteriorsInnPlacements; Sunshine's house is beside it.
enum YallamInteriorsInnActorId {
  /// "Yepp had three different songs he used to sing."
  ACTOR_YEPP_SONGS_VILLAGER = ACTOR_FIRST_PLACED,
  /// "Do you like children's songs?" (YallamInteriors_AskLikeChildrensSongs)
  ACTOR_CHILDRENS_SONGS_VILLAGER,
  /// "It's been really dry the past few months."
  ACTOR_DRY_MONTHS_VILLAGER,
  /// "We're supposed to be right in the middle of the rainy season..."
  ACTOR_RAINY_SEASON_VILLAGER,
  ACTOR_INNKEEPER,
  /// "Taopo Swamp has dried up, and now it's inaccessible."
  ACTOR_DRIED_SWAMP_VILLAGER,
  /// Sunshine the blacksmith, lying in bed until rain comes or the party
  /// brings him something to forge.
  ACTOR_SUNSHINE,
  ACTOR_SUNSHINES_WIFE,
  ACTOR_SUNSHINES_CHILD,
  /// The counter in front of Sunshine's bed, talked to across.
  ACTOR_SUNSHINES_COUNTER
};

/// The shops' actors (SceneId_YallamInteriorsShops), numbering the rows of
/// gYallamInteriorsShopsPlacements.
enum YallamInteriorsShopsActorId {
  /// "Sunshine has started complaining that he can't work lately."
  ACTOR_COMPLAINT_VILLAGER = ACTOR_FIRST_PLACED,
  /// "Our blacksmith, Sunshine, makes the kinds of items you can't..."
  ACTOR_BLACKSMITH_VILLAGER,
  ACTOR_WEAPON_MERCHANT,
  ACTOR_ARMOR_MERCHANT,
  ACTOR_ITEM_MERCHANT,
  /// The three counters the merchants are talked to across.
  ///@{
  ACTOR_WEAPON_COUNTER,
  ACTOR_ARMOR_COUNTER,
  ACTOR_ITEM_COUNTER
  ///@}
};

void Event_End(void);
void Shop_Run(s32 shop, s32 keeper);
void Sanctum_Run(s32 keeper);
void Inn_CheckIn(s32 shop, s32 keeper);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);

void FieldEffect_StartBlankOverlay(s32 a);
void Event_StartScreenTransition(void);

extern struct ActorPlacement gYallamInteriors_Placements[];
extern struct ActorPlacement gYallamInteriorsInnPlacements[];
extern struct ActorPlacement gYallamInteriorsShopsPlacements[];
extern struct Trigger gYallamInteriors_Triggers[];
extern struct Trigger gYallamInteriorsInnTriggers[];
extern struct Trigger gYallamInteriorsShopsTriggers[];

void Forge_Run(s32 actor);

void Forge_SetItem(s32 value);

void ItemDisplay_ShowItem(s32 item, s32 mode);

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  s16 scene = gMainRoutineWork.next;

  if (scene == SCENE_YALLAM_INTERIORS_INN)
    return gYallamInteriorsInnPlacements;
  if (scene == SCENE_YALLAM_INTERIORS_SHOPS)
    return gYallamInteriorsShopsPlacements;
  return gYallamInteriors_Placements;
}

/// Sunshine the blacksmith thinks of the swamp he cannot reach without rain,
/// and once the party has had him forge, of his joy at working (flag 0x301).
s32 YallamInteriors_MindReadSunshine(void) {

  Event_Start();
  Event_ResolvePendingActions(0);
  if (Event_GetFlag(FLAG_AREA_TEMP_01)) {
    Event_SetMessageId((s32)MsgYallamInteriorsItsBeenALong);
  } else {
    Event_SetMessageId((s32)MsgYallamInteriorsIfItsNotRaining);
  }
  // "If it's not raining, I can't get into Taopo Swamp. And if I can't..."
  Event_ShowMessage(ACTOR_SUNSHINE, 0);
  Event_End();
  return 0;
}

s32 YallamInteriors_MindReadSunshinesWife(void) {

  Event_Start();
  Event_ResolvePendingActions(0);
  if (Event_GetFlag(FLAG_AREA_TEMP_01)) {
    Event_SetMessageId((s32)MsgYallamInteriorsWhatHappenedToSunshine);
  } else {
    Event_SetMessageId((s32)MsgYallamInteriorsSunshinesAPrettyStubborn);
  }
  // "Sunshine's a pretty stubborn guy. If he says he isn't going to work..."
  Event_ShowMessage(ACTOR_SUNSHINES_WIFE, 0);
  Event_End();
  return 0;
}

s32 YallamInteriors_MindReadSunshinesChild(void) {

  Event_Start();
  Event_ResolvePendingActions(0);
  if (Event_GetFlag(FLAG_AREA_TEMP_01)) {
    Event_SetMessageId((s32)MsgYallamInteriorsNowThatMyDads);
  } else {
    Event_SetMessageId((s32)MsgYallamInteriorsEveryoneThinksDadIs);
  }
  // "Everyone thinks Dad is stubborn, but he's not... He's a true artist..."
  Event_ShowMessage(ACTOR_SUNSHINES_CHILD, 0);
  Event_End();
  return 0;
}

/// "Dad's not doing any work right now, but I'm making some basic things for
/// the villagers."
s32 YallamInteriors_TalkToSunshinesChild(void) {

  Event_Start();
  Event_ResolvePendingActions(0);
  if (Event_GetFlag(FLAG_AREA_TEMP_01)) {
    Event_SetMessageId((s32)MsgYallamInteriorsItsSoWeirdBut);
  } else {
    Event_SetMessageId((s32)MsgYallamInteriorsDadsNotDoingAny);
  }
  // "Dad's not doing any work right now, but I'm making some basic things..."
  Event_ShowMessage(ACTOR_SUNSHINES_CHILD, 0);
  Event_End();
  return 0;
}

s32 YallamInteriors_MindReadWeaponMerchant(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgYallamInteriorsIfSunshineSetHis);
  // "If Sunshine set his mind to it, he could make a lot of money! What a..."
  Event_ShowMessage(ACTOR_WEAPON_MERCHANT, 0);
  Event_End();
  return 0;
}

s32 YallamInteriors_MindReadArmorMerchant(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgYallamInteriorsIShouldntTellHim);
  // "I shouldn't tell him how boring all this stuff is. I wish Sunshine..."
  Event_ShowMessage(ACTOR_ARMOR_MERCHANT, 0);
  Event_End();
  return 0;
}

s32 YallamInteriors_MindReadItemMerchant(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgYallamInteriorsWhatDoesSunshineWant);
  // "What does Sunshine want in Taopo Swamp? There's nothing up there but..."
  Event_ShowMessage(ACTOR_ITEM_MERCHANT, 0);
  Event_End();
  return 0;
}

const struct Trigger *Scene_GetTriggers(void) {
  s16 scene = gMainRoutineWork.next;

  if (scene == SCENE_YALLAM_INTERIORS_INN)
    return gYallamInteriorsInnTriggers;
  if (scene == SCENE_YALLAM_INTERIORS_SHOPS)
    return gYallamInteriorsShopsTriggers;
  return gYallamInteriors_Triggers;
}

/// "Do you like children's songs?"
void YallamInteriors_AskLikeChildrensSongs(void) {
  s32 msg;

  Event_Start();
  Event_ResolvePendingActions(0);
  msg = (s32)MsgYallamInteriorsDoYouLikeChildrens;
  Event_SetMessageId(msg);
  // "Do you like children's songs?"
  Event_OpenMessage(ACTOR_CHILDRENS_SONGS_VILLAGER, 0);
  if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
    Event_Wait(ACTOR_DRY_MONTHS_VILLAGER);
    Event_SetMessageId(msg + 1);
  } else {
    Event_Wait(20);
    Event_SetMessageId(msg + 2);
  }
  Event_ShowMessage(ACTOR_CHILDRENS_SONGS_VILLAGER, 0);
  Event_End();
}

/// Yallam's shops, as Shop_Run numbers them.
enum {
  SHOP_YALLAM_WEAPONS = 20,
  SHOP_YALLAM_ARMOR = 21,
  SHOP_YALLAM_ITEMS = 22
};

/// Yallam's inn, as Inn_CheckIn numbers the inns.
enum { YALLAM_INN = 7 };

/// Yallam's weapon merchant sells across the counter, and otherwise wishes
/// Sunshine would work.
void YallamInteriors_TalkToWeaponMerchant(s32 actor) {
  u16 facing = Actor_GetById(gMainRoutineWork.leaderActorId)->facing;

  if (FACING_NEAREST_CARDINAL(facing) == FACING_NORTH) {
    Shop_Run(SHOP_YALLAM_WEAPONS, actor);
  } else {
    Event_Start();
    Event_ResolvePendingActions(0);
    Event_SetMessageId((s32)MsgYallamInteriorsIfTheBlacksmithIsnt);
    // "If the blacksmith isn't willing to work, then how am I supposed to..."
    Event_ShowMessage(actor, 0);
    Event_End();
  }
}

/// Yallam's armor merchant sells across the counter, and otherwise sends the
/// party to Sunshine for something special.
void YallamInteriors_TalkToArmorMerchant(s32 actor) {
  u16 facing = Actor_GetById(gMainRoutineWork.leaderActorId)->facing;

  if (FACING_NEAREST_CARDINAL(facing) == FACING_NORTH) {
    Shop_Run(SHOP_YALLAM_ARMOR, actor);
  } else {
    Event_Start();
    Event_ResolvePendingActions(0);
    Event_SetMessageId((s32)MsgYallamInteriorsIfYouWantPlain);
    // "If you want plain old, boring armor, have a look around. If you're..."
    Event_ShowMessage(actor, 0);
    Event_End();
  }
}

/// Yallam's item merchant sells across the counter, and otherwise points the
/// way to Taopo Swamp.
void YallamInteriors_TalkToItemMerchant(s32 actor) {
  u16 facing = Actor_GetById(gMainRoutineWork.leaderActorId)->facing;

  if (FACING_NEAREST_CARDINAL(facing) == FACING_NORTH) {
    Shop_Run(SHOP_YALLAM_ITEMS, actor);
  } else {
    Event_Start();
    Event_ResolvePendingActions(0);
    Event_SetMessageId((s32)MsgYallamInteriorsIfYoureLookingFor);
    // "If you're looking for Taopo Swamp, head west around the northern..."
    Event_ShowMessage(actor, 0);
    Event_End();
  }
}

/// Yallam's innkeeper takes the party in across the counter, and otherwise
/// talks of the town that never gets tourists.
void YallamInteriors_TalkToInnkeeper(s32 actor) {
  u16 facing = Actor_GetById(gMainRoutineWork.leaderActorId)->facing;

  if (FACING_NEAREST_CARDINAL(facing) == FACING_NORTH) {
    Inn_CheckIn(YALLAM_INN, actor);
    Event_ClearFlag(FLAG_AREA_TEMP_01);
  } else {
    Event_Start();
    Event_ResolvePendingActions(0);
    Event_SetMessageId((s32)MsgYallamInteriorsItsABitStrange);
    // "It's a bit strange trying to run an inn in a city that never gets..."
    Event_ShowMessage(actor, 0);
    Event_End();
  }
}

void YallamInteriors_TalkToHealer(s32 actor) {
  u16 facing = Actor_GetById(gMainRoutineWork.leaderActorId)->facing;

  if (FACING_NEAREST_CARDINAL(facing) == FACING_NORTH) {
    Sanctum_Run(actor);
  } else {
    Event_Start();
    Event_ResolvePendingActions(0);
    Event_SetMessageId((s32)MsgMikasallaInteriors27);
    // "Even out in these far-off shores, people need the services and hope..."
    Event_ShowMessage(actor, 0);
    Event_End();
  }
}

/// "Ah, so you want me to make you a little something special?"
void YallamInteriors_OfferToForge(void) {
  s32 msg;

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_StartRepeatedMotion(ACTOR_SUNSHINE, 2);
  msg = (s32)MsgYallamInteriorsAhSoYouWant;
  Event_SetMessageId(msg);
  // "Ah, so you want me to make you a little something special?"
  Event_OpenMessage(ACTOR_SUNSHINE, 0);
  if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
    Event_Wait(ACTOR_DRY_MONTHS_VILLAGER);
    Event_SetMessageId(msg + 1);
  } else {
    Event_Wait(20);
    Event_SetMessageId(msg + 2);
  }
  Event_ShowMessage(ACTOR_SUNSHINE, 0);
  Event_End();
}

s32 Forge_GetItem(void);
void Actor_FallOverById(s32 actor, s32 angle);

/// Whether anyone in the party carries a material Sunshine can forge: items 417
/// to 427 or 429 to 437.
s32 YallamInteriors_PartyHasForgeMaterial(void) {
  s32 item;

  for (item = 429; item <= 437; item++) {
    if (PartyInventory_FindMember(item) != -1)
      return 1;
  }
  for (item = 417; item <= 427; item++) {
    if (PartyInventory_FindMember(item) != -1)
      return 1;
  }
  return 0;
}

/// Actor 14's talk. Flag 0x10f follows YallamInteriors_PartyHasForgeMaterial's
/// item check (it stays set while flag 0x301 or Forge_GetItem holds). Once
/// 0x10f is set and neither holds, a yes moves actor 14 and the leader, and if
/// Forge_GetItem then holds, places actor 16 and sets flag 0x301.
void YallamInteriors_TalkToSunshine(void) {
  s32 message;

  if (YallamInteriors_PartyHasForgeMaterial())
    Event_SetFlag(0x10f);
  else if (!Event_GetFlag(FLAG_AREA_TEMP_01) && !Forge_GetItem())
    Event_ClearFlag(0x10f);

  if (!Event_GetFlag(0x10f)) {
    message = (s32)MsgAhSoYou;
    Event_SetMessageId(message);
    // "Ah, so you're looking for a really unique item, huh?"
    Event_OpenMessage(ACTOR_SUNSHINE, 0);
    if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
      FieldEvent_Wait(10);
      Event_SetMessageId(message + 1);
    } else {
      FieldEvent_Wait(20);
      Event_SetMessageId(message + 2);
    }
    Event_ShowMessage(ACTOR_SUNSHINE, 0);
  } else if (Event_GetFlag(FLAG_AREA_TEMP_01)) {
    Event_SetMessageId((s32)MsgICanTWait);
    // "I can't wait to see what I'm going to wind up making with this......"
    Event_ShowMessage(ACTOR_SUNSHINE, 0);
  } else if (Forge_GetItem()) {
    Event_SetMessageId((s32)MsgMyWifeHas);
    // "My wife has the item I made you. She can give it to you."
    Event_ShowMessage(ACTOR_SUNSHINE, 0);
  } else {
    message = (s32)MsgYouWantMe;
    Event_SetMessageId(message);
    // "You want me to make you a little something special?"
    Event_OpenMessage(ACTOR_SUNSHINE, 0);
    if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
      Event_SetMessageId(message + 1);
      // "Well, look at that! You went to get some raw materials for me..."
      Event_ShowMessage(ACTOR_SUNSHINE, 0);
      Actor_FallOverById(ACTOR_SUNSHINE, 0xc000);
      Actor_SetSpriteFlags(Actor_GetById(ACTOR_SUNSHINE), 1);
      Actor_SetDestination(gMainRoutineWork.leaderActorId, 488, 408);
      Actor_WaitForMoveById(gMainRoutineWork.leaderActorId);
      Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_EAST, 0);
      Actor_SetDestination(ACTOR_SUNSHINE, 504, 408);
      Actor_WaitForMoveById(ACTOR_SUNSHINE);
      Actor_FaceActor(ACTOR_SUNSHINE, gMainRoutineWork.leaderActorId, 0);
      Forge_Run(ACTOR_SUNSHINE);
      if (Forge_GetItem()) {
        Actor_SetSpeed(ACTOR_SUNSHINE, 0.4);
        Actor_WalkTo(ACTOR_SUNSHINE, 568, 408);
        Actor_WaitForMoveById(ACTOR_SUNSHINE);
        Actor_WalkTo(ACTOR_SUNSHINE, 568, 360);
        Actor_WaitForMoveById(ACTOR_SUNSHINE);
        Actor_WalkTo(ACTOR_SUNSHINES_CHILD, 632, 346);
        Actor_FaceDirection(ACTOR_SUNSHINES_CHILD, FACING_SOUTH, 0);
        Actor_SetAnimation(ACTOR_SUNSHINES_CHILD, 0);
        Actor_WalkTo(ACTOR_SUNSHINE, 600, 360);
        Actor_WaitForMoveById(ACTOR_SUNSHINE);
        Actor_SetAnimation(ACTOR_SUNSHINE, 0);
        Actor_FaceDirection(ACTOR_SUNSHINE, FACING_SOUTH, 0);
        Actor_GetById(ACTOR_SUNSHINE)->collisionFlags =
            ACTOR_BLOCKED_BY_SOLID | ACTOR_SOLID;
        Actor_GetById(ACTOR_SUNSHINE)->priorityFlags = ACTOR_PRIORITY_AUTOMATIC;
        Actor_SetSpritePriority(ACTOR_SUNSHINE, 1);
        Actor_GetById(ACTOR_SUNSHINE)->priorityFlags |=
            ACTOR_PRIORITY_AUTOMATIC;
#if TLA_PATCH_1
        Actor_SetPosition(ACTOR_SUNSHINES_COUNTER, 0, 0);
#endif
        Event_SetFlag(FLAG_AREA_TEMP_01);
      } else {
        Actor_SetDestination(ACTOR_SUNSHINE, 491, 398);
        Actor_WaitForMoveById(ACTOR_SUNSHINE);
        Actor_FaceDirection(ACTOR_SUNSHINE, FACING_SOUTH, 0);
        Actor_SetSpriteFlags(Actor_GetById(ACTOR_SUNSHINE), 0);
        Actor_FallOverById(ACTOR_SUNSHINE, 0);
      }
    } else {
      Event_SetMessageId(message + 2);
      // "Hm... Maybe I made a mistake. It's hard to tell... If you want..."
      Event_ShowMessage(ACTOR_SUNSHINE, 0);
    }
  }
}

/// Sunshine's wife keeps the smithy: she explains the forge, hands over what
/// Sunshine made and takes the payment.
/// "This? This is the blacksmith Sunshine's smithy, the best forge in Weyard!"
void YallamInteriors_TalkToSunshinesWife(s32 actor) {
  if (!Event_GetFlag(0x10f)) {
    Event_SetMessageId((s32)MsgThisThisIs);
    // "This? This is the blacksmith Sunshine's smithy, the best forge in..."
    Event_ShowMessage(actor, 0);
  } else if (Event_GetFlag(FLAG_AREA_TEMP_01)) {
    Event_SetMessageId((s32)MsgItTakesOur);
    // "It takes our Sunshine a long time to finish the items he works on...."
    Event_ShowMessage(actor, 0);
  } else if (Forge_GetItem()) {
    s32 item = Forge_GetItem();
    u32 price = Item_GetDirect(item)->price;
    s32 message;

    MessageArgument_Push(item, MESSAGE_ARGUMENT_ITEM);
    MessageArgument_Push(price, MESSAGE_ARGUMENT_NUMBER);
    message = (s32)MsgHereSThe;
    Event_SetMessageId(message);
    // "Here's the item you wanted. Now, about the payment... We only take..."
    Event_OpenMessage(actor, 0);
    if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) != 0) {
      Event_SetMessageId(message + 1);
      // "Hey, we're not a charity! If you can't pay for it, you don't get it!"
      Event_ShowMessage(actor, 0);
    } else if (PartyInventory_FindRoom(item) < 0) {
      Event_SetMessageId(message + 2);
      // "Whoops! Looks like you've got a full load there! This'll have to..."
      Event_ShowMessage(actor, 0);
    } else if ((u32)gMainRoutineWork.coins < price) {
      Event_SetMessageId(message + 3);
      // "You don't enough money! What are you trying to pull? I can't give..."
      Event_ShowMessage(actor, 0);
    } else {
      Event_SetMessageId(message + 4);
      // "OK, here you go!"
      Event_ShowMessage(actor, 0);
      ItemDisplay_ShowItem(item, 3);
      Party_GiveItem(item, 0);
      Forge_SetItem(0);
      Party_AdjustCoins(-price);
    }
  } else {
    Event_SetMessageId((s32)MsgManSunshineWould);
    // "Man, Sunshine would be rich if he worked more. What a waste..."
    Event_ShowMessage(actor, 0);
  }
}

/// The scene's start entry: the screen effect 0x209, then the actors of
/// rooms 1 and 2.
s32 Scene_Initialize(void) {
  s32 scene;

  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
  {
    struct MainRoutineWork *party = &gMainRoutineWork;

    scene = party->next;
  }
  if (scene == SCENE_YALLAM_INTERIORS_INN) {
    if (Event_GetFlag(FLAG_AREA_TEMP_01)) {
      Actor_SetPosition(ACTOR_SUNSHINE, 600, 360);
      Actor_SetPosition(ACTOR_SUNSHINES_CHILD, 632, 346);
      Actor_FaceDirection(ACTOR_SUNSHINES_CHILD, FACING_SOUTH, 0);
#if TLA_PATCH_1
      Actor_SetPosition(ACTOR_SUNSHINES_COUNTER, 0, 0);
#endif
    } else {
      Actor_FallOverById(ACTOR_SUNSHINE, 0);
    }
    Actor_SetSpriteFlags(Actor_GetById(ACTOR_SUNSHINES_COUNTER), 0);
    Actor_SetSpritePriority(ACTOR_RAINY_SEASON_VILLAGER, 2);
  }
  if (gMainRoutineWork.next == SCENE_YALLAM_INTERIORS_SHOPS) {
    FieldEffect_StartBlankOverlay(0);
    Actor_SetSpritePriority(ACTOR_ITEM_MERCHANT, 2);
    Actor_GetById(ACTOR_ITEM_MERCHANT)->collisionFlags |= ACTOR_NEAR_TALK;
    Actor_SetSpritePriority(ACTOR_WEAPON_MERCHANT, 2);
    Actor_GetById(ACTOR_WEAPON_MERCHANT)->collisionFlags |= ACTOR_NEAR_TALK;
    Actor_SetSpritePriority(ACTOR_ARMOR_MERCHANT, 2);
    Actor_GetById(ACTOR_ARMOR_MERCHANT)->collisionFlags |= ACTOR_NEAR_TALK;
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

// clang-format off
/// The villager who asks about children's songs wanders between two corners of
/// the room.
u32 gYallamInteriors_SongLoverWanders[] = {
    SC_SET(SW_COLLISION, 131),
    SC_ACCELERATION(0.1),
    SC_SPEED(0.3),
    SC_LABEL(1),
    SC_ANIMATION(2),
    SC_MOVE(120, 0, 104),
    SC_SYNC,
    SC_MOVE(128, 0, 92),
    SC_SYNC,
    SC_WAIT(20),
    SC_ANIMATION(1),
    SC_WAIT(20),
    SC_ANIMATION(2),
    SC_MOVE(120, 0, 104),
    SC_SYNC,
    SC_MOVE(88, 0, 104),
    SC_SYNC,
    SC_WAIT(20),
    SC_ANIMATION(1),
    SC_WAIT(20),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
    SC_END,
    SC_SET(SW_COLLISION, 129),
    SC_ACCELERATION(0.1),
    SC_SPEED(0.3),
    SC_LABEL(1),
    SC_ANIMATION(2),
    SC_MOVE(437, 0, 127),
    SC_SYNC,
    SC_WAIT(20),
    SC_ANIMATION(1),
    SC_SET(SW_FACING, FACING_SOUTHEAST),
    SC_WAIT(60),
    SC_ANIMATION(2),
    SC_MOVE(453, 0, 114),
    SC_SYNC,
    SC_WAIT(20),
    SC_ANIMATION(1),
    SC_SET(SW_FACING, FACING_SOUTHEAST),
    SC_WAIT(60),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
    SC_END,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(YallamInteriorsInn),
    SCENE_EXIT_UNLESS(1, Yallam, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, Yallam, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, Yallam, 7, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, Yallam, 4, EXIT_FLAG_NONE),
    SCENE_EXITS(YallamInteriorsShops),
    SCENE_EXIT_UNLESS(1, Yallam, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, Yallam, 6, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, Yallam, 5, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

struct ActorPlacement gYallamInteriors_Placements[] = {
    {SCENE_TABLE_END},
};
// clang-format on

struct ActorPlacement gYallamInteriorsInnPlacements[] = {
#if !TLA_PATCH_2
    ACTOR_PLACEMENT(115, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 136,
                    FACING_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
#else
    ACTOR_PLACEMENT(115, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 140,
                    FACING_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
#endif
    ACTOR_PLACEMENT(116, CONDITION_ALWAYS,
                    (s32)gYallamInteriors_SongLoverWanders, 88, 0, 104,
                    FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(114, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 424, 0, 88,
                    FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(128, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 454, 0, 120,
                    FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(134, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 294, 0, 408,
                    FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(135, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 184, 0, 392,
                    FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(104, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 491, 0, 398,
                    FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(107, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 560, 0, 440,
                    FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(115, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 600, 0, 346,
                    FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 506, 0, 398,
                    FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

// clang-format off
struct ActorPlacement gYallamInteriorsShopsPlacements[] = {
    ACTOR_PLACEMENT(112, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 144, 0, 188, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(113, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 184, 0, 152, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(108, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 405, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(109, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 112, 0, 405, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(110, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 348, 0, 405, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 408, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 112, 0, 408, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 348, 0, 408, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    {SCENE_TABLE_END},
};

struct Trigger gYallamInteriors_Triggers[] = {
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_TABLE_END},
};
// clang-format on

struct Trigger gYallamInteriorsInnTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS,
     MSGNO(MsgYallamInteriorsYeppHadThreeDifferent)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS,
     EVENT(YallamInteriors_AskLikeChildrensSongs)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS,
     MSGNO(MsgYallamInteriorsItsBeenReallyDry)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS,
     MSGNO(MsgYallamInteriorsWereSupposedToBe)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS,
     EVENT(YallamInteriors_TalkToInnkeeper)},
    {TRIGGER_TALK, 13, CONDITION_ALWAYS,
     MSGNO(MsgYallamInteriorsTaopoSwampHasDried)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS, EVENT(YallamInteriors_TalkToSunshine)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS,
     EVENT(YallamInteriors_TalkToSunshinesWife)},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS,
     EVENT(YallamInteriors_TalkToSunshinesChild)},
    {TRIGGER_TALK, 17, CONDITION_ALWAYS, EVENT(YallamInteriors_TalkToSunshine)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS,
     MSGNO(MsgYallamInteriorsTheSongsAreSupposed)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS,
     MSGNO(MsgYallamInteriorsIWonderWhyYepp)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS,
     MSGNO(MsgYallamInteriorsWhenTaopoSwampDries)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS,
     MSGNO(MsgYallamInteriorsICantStandThis)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS,
     MSGNO(MsgYallamInteriorsYallamWasAlwaysSuch)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS,
     MSGNO(MsgYallamInteriorsThereWerentALot)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS,
     EVENT(YallamInteriors_MindReadSunshine)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS,
     EVENT(YallamInteriors_MindReadSunshinesWife)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS,
     EVENT(YallamInteriors_MindReadSunshinesChild)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS,
     EVENT(YallamInteriors_MindReadSunshine)},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 200, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgYallamInteriorsTheSteelRestingIn))},
    {TRIGGER_TABLE_END},
};

struct Trigger gYallamInteriorsShopsTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS,
     MSGNO(MsgYallamInteriorsSunshineHasStartedComplaining)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS,
     MSGNO(MsgYallamInteriorsOurBlacksmithSunshineMakes)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS,
     EVENT(YallamInteriors_TalkToWeaponMerchant)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS,
     EVENT(YallamInteriors_TalkToArmorMerchant)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS,
     EVENT(YallamInteriors_TalkToItemMerchant)},
    {TRIGGER_TALK, 13, CONDITION_ALWAYS,
     EVENT(YallamInteriors_TalkToWeaponMerchant)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS,
     EVENT(YallamInteriors_TalkToArmorMerchant)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS,
     EVENT(YallamInteriors_TalkToItemMerchant)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS,
     MSGNO(MsgYallamInteriorsWhatKindOfPerson)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS,
     MSGNO(MsgYallamInteriorsSunshinesALittleEccentric)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS,
     MSGNO(MsgYallamInteriorsIfSunshineSetHis)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS,
     MSGNO(MsgYallamInteriorsIShouldntTellHim)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS,
     MSGNO(MsgYallamInteriorsWhatDoesSunshineWant)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS,
     EVENT(YallamInteriors_MindReadWeaponMerchant)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS,
     EVENT(YallamInteriors_MindReadArmorMerchant)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS,
     EVENT(YallamInteriors_MindReadItemMerchant)},
    {TRIGGER_TABLE_END},
};
