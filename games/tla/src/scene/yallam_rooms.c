/// @file
/// Yallam's houses and shops, with Sunshine the blacksmith, who forges the
/// rare materials the party brings him into weapons and armor once rain has
/// let him reach Taopo Swamp.
#include "fixed_math.h"
#include "flags.h"
#include "item_ids.h"
#include "scene_ids.h"
#include "sprite_ids.h"
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

/// Sunshine is at work on the party's material; a night at the inn finishes it.
#define FLAG_SUNSHINE_FORGING FLAG_AREA_TEMP_01

void MessageArgument_Push(u32 value, u32 kind);

/// The inn's actors (SceneId_YallamRoomsInn), numbering the rows of
/// gYallamRoomsInnPlacements; Sunshine's house is beside it.
enum YallamRoomsInnActorId {
  /// "Yepp had three different songs he used to sing."
  ACTOR_YEPP_SONGS_VILLAGER = ACTOR_FIRST_PLACED,
  /// "Do you like children's songs?" (YallamRooms_AskLikeChildrensSongs)
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

/// The shops' actors (SceneId_YallamRoomsShops), numbering the rows of
/// gYallamRoomsShopsPlacements.
enum YallamRoomsShopsActorId {
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

extern struct ActorPlacement gYallamRooms_Placements[];
extern struct ActorPlacement gYallamRoomsInnPlacements[];
extern struct ActorPlacement gYallamRoomsShopsPlacements[];
extern struct Trigger gYallamRooms_Triggers[];
extern struct Trigger gYallamRoomsInnTriggers[];
extern struct Trigger gYallamRoomsShopsTriggers[];

void Forge_Run(s32 actor);

void Forge_SetItem(s32 value);

void ItemDisplay_ShowItem(s32 item, s32 mode);

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  s16 scene = gMainRoutineWork.next;

  if (scene == SCENE_YALLAM_ROOMS_INN)
    return gYallamRoomsInnPlacements;
  if (scene == SCENE_YALLAM_ROOMS_SHOPS)
    return gYallamRoomsShopsPlacements;
  return gYallamRooms_Placements;
}

/// Sunshine the blacksmith thinks of the swamp he cannot reach without rain,
/// and once the party has had him forge, of his joy at working.
s32 YallamRooms_MindReadSunshine(void) {

  Event_Start();
  Event_ResolvePendingActions(0);
  if (Event_GetFlag(FLAG_SUNSHINE_FORGING)) {
    Event_SetMessageId((s32)MsgYallamInteriorsItsBeenALong);
  } else {
    Event_SetMessageId((s32)MsgYallamInteriorsIfItsNotRaining);
  }
  // "If it's not raining, I can't get into Taopo Swamp. And if I can't..."
  Event_ShowMessage(ACTOR_SUNSHINE, 0);
  Event_End();
  return 0;
}

/// Sunshine's wife thinks him stubborn, and once he forges, wonders what came
/// over him.
s32 YallamRooms_MindReadSunshinesWife(void) {

  Event_Start();
  Event_ResolvePendingActions(0);
  if (Event_GetFlag(FLAG_SUNSHINE_FORGING)) {
    Event_SetMessageId((s32)MsgYallamInteriorsWhatHappenedToSunshine);
  } else {
    Event_SetMessageId((s32)MsgYallamInteriorsSunshinesAPrettyStubborn);
  }
  // "Sunshine's a pretty stubborn guy. If he says he isn't going to work..."
  Event_ShowMessage(ACTOR_SUNSHINES_WIFE, 0);
  Event_End();
  return 0;
}

/// Sunshine's child thinks of the artist in Dad, and once he forges, of Dad at
/// work again.
s32 YallamRooms_MindReadSunshinesChild(void) {

  Event_Start();
  Event_ResolvePendingActions(0);
  if (Event_GetFlag(FLAG_SUNSHINE_FORGING)) {
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
s32 YallamRooms_TalkToSunshinesChild(void) {

  Event_Start();
  Event_ResolvePendingActions(0);
  if (Event_GetFlag(FLAG_SUNSHINE_FORGING)) {
    Event_SetMessageId((s32)MsgYallamInteriorsItsSoWeirdBut);
  } else {
    Event_SetMessageId((s32)MsgYallamInteriorsDadsNotDoingAny);
  }
  // "Dad's not doing any work right now, but I'm making some basic things..."
  Event_ShowMessage(ACTOR_SUNSHINES_CHILD, 0);
  Event_End();
  return 0;
}

/// The weapon merchant, read across the counter, thinks of the money Sunshine
/// could make.
s32 YallamRooms_MindReadWeaponMerchant(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgYallamInteriorsIfSunshineSetHis);
  // "If Sunshine set his mind to it, he could make a lot of money! What a..."
  Event_ShowMessage(ACTOR_WEAPON_MERCHANT, 0);
  Event_End();
  return 0;
}

/// The armor merchant, read across the counter, finds his own stock boring.
s32 YallamRooms_MindReadArmorMerchant(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgYallamInteriorsIShouldntTellHim);
  // "I shouldn't tell him how boring all this stuff is. I wish Sunshine..."
  Event_ShowMessage(ACTOR_ARMOR_MERCHANT, 0);
  Event_End();
  return 0;
}

/// The item merchant, read across the counter, wonders what Sunshine wants in
/// Taopo Swamp.
s32 YallamRooms_MindReadItemMerchant(void) {
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

  if (scene == SCENE_YALLAM_ROOMS_INN)
    return gYallamRoomsInnTriggers;
  if (scene == SCENE_YALLAM_ROOMS_SHOPS)
    return gYallamRoomsShopsTriggers;
  return gYallamRooms_Triggers;
}

/// "Do you like children's songs?"
void YallamRooms_AskLikeChildrensSongs(void) {
  s32 msg;

  Event_Start();
  Event_ResolvePendingActions(0);
  msg = (s32)MsgYallamInteriorsDoYouLikeChildrens;
  Event_SetMessageId(msg);
  // "Do you like children's songs?"
  Event_OpenMessage(ACTOR_CHILDRENS_SONGS_VILLAGER, 0);
  if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
    Event_Wait(10);
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
void YallamRooms_TalkToWeaponMerchant(s32 actor) {
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
void YallamRooms_TalkToArmorMerchant(s32 actor) {
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
void YallamRooms_TalkToItemMerchant(s32 actor) {
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
void YallamRooms_TalkToInnkeeper(s32 actor) {
  u16 facing = Actor_GetById(gMainRoutineWork.leaderActorId)->facing;

  if (FACING_NEAREST_CARDINAL(facing) == FACING_NORTH) {
    Inn_CheckIn(YALLAM_INN, actor);
    Event_ClearFlag(FLAG_SUNSHINE_FORGING);
  } else {
    Event_Start();
    Event_ResolvePendingActions(0);
    Event_SetMessageId((s32)MsgYallamInteriorsItsABitStrange);
    // "It's a bit strange trying to run an inn in a city that never gets..."
    Event_ShowMessage(actor, 0);
    Event_End();
  }
}

/// The healer tends the party at the Sanctum across the counter, and otherwise
/// speaks of the hope a Sanctum brings to far-off shores.
void YallamRooms_TalkToHealer(s32 actor) {
  u16 facing = Actor_GetById(gMainRoutineWork.leaderActorId)->facing;

  if (FACING_NEAREST_CARDINAL(facing) == FACING_NORTH) {
    Sanctum_Run(actor);
  } else {
    Event_Start();
    Event_ResolvePendingActions(0);
    Event_SetMessageId((s32)MsgEvenOutInFarOffShores);
    // "Even out in these far-off shores, people need the services and hope..."
    Event_ShowMessage(actor, 0);
    Event_End();
  }
}

/// "Ah, so you want me to make you a little something special?"
void YallamRooms_OfferToForge(void) {
  s32 msg;

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_StartRepeatedMotion(ACTOR_SUNSHINE, 2);
  msg = (s32)MsgYallamInteriorsAhSoYouWant;
  Event_SetMessageId(msg);
  // "Ah, so you want me to make you a little something special?"
  Event_OpenMessage(ACTOR_SUNSHINE, 0);
  if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
    Event_Wait(10);
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

/// Whether anyone in the party carries something Sunshine can forge: a rusty
/// weapon, or a material from Tear Stone to Orihalcon.
s32 YallamRooms_PartyHasForgeMaterial(void) {
  s32 item;

  for (item = ITEM_TEAR_STONE; item <= ITEM_ORIHALCON; item++) {
    if (PartyInventory_FindMember(item) != -1)
      return 1;
  }
  for (item = ITEM_RUSTY_SWORD; item <= ITEM_RUSTY_STAFF_GOBLINS_ROD; item++) {
    if (PartyInventory_FindMember(item) != -1)
      return 1;
  }
  return 0;
}

/// Sunshine, talked to in bed or across his counter. The forge opens once the
/// party carries a material, and stays open while he works or his wife holds
/// what he made. With it open and nothing pending, a yes brings him to the
/// forge; if he takes a material, he walks off to work and his child moves
/// aside.
void YallamRooms_TalkToSunshine(void) {
  s32 message;

  if (YallamRooms_PartyHasForgeMaterial())
    Event_SetFlag(FLAG_FORGE_OPEN);
  else if (!Event_GetFlag(FLAG_SUNSHINE_FORGING) && !Forge_GetItem())
    Event_ClearFlag(FLAG_FORGE_OPEN);

  if (!Event_GetFlag(FLAG_FORGE_OPEN)) {
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
  } else if (Event_GetFlag(FLAG_SUNSHINE_FORGING)) {
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
      Actor_FallOverById(ACTOR_SUNSHINE, DEGtoANG(270));
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
        Event_SetFlag(FLAG_SUNSHINE_FORGING);
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
void YallamRooms_TalkToSunshinesWife(s32 actor) {
  if (!Event_GetFlag(FLAG_FORGE_OPEN)) {
    Event_SetMessageId((s32)MsgThisThisIs);
    // "This? This is the blacksmith Sunshine's smithy, the best forge in..."
    Event_ShowMessage(actor, 0);
  } else if (Event_GetFlag(FLAG_SUNSHINE_FORGING)) {
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

/// Opens the houses through the window transition: in the inn Sunshine lies in
/// bed or, while he forges, stands at work; in the shops the merchants are
/// talked to across their counters.
s32 Scene_Initialize(void) {
  s32 scene;

  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
  {
    struct MainRoutineWork *party = &gMainRoutineWork;

    scene = party->next;
  }
  if (scene == SCENE_YALLAM_ROOMS_INN) {
    if (Event_GetFlag(FLAG_SUNSHINE_FORGING)) {
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
  if (gMainRoutineWork.next == SCENE_YALLAM_ROOMS_SHOPS) {
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
u32 gYallamRooms_SongLoverWanders[] = {
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

#include "shared/scene_entrances_default.c"

u32 gSceneExits[] = {
    SCENE_EXITS(YallamRoomsInn),
    SCENE_EXIT_UNLESS(1, Yallam, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, Yallam, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, Yallam, 7, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, Yallam, 4, EXIT_FLAG_NONE),
    SCENE_EXITS(YallamRoomsShops),
    SCENE_EXIT_UNLESS(1, Yallam, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, Yallam, 6, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, Yallam, 5, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

/// No room but the inn and the shops places actors.
struct ActorPlacement gYallamRooms_Placements[] = {
    {SCENE_TABLE_END},
};
// clang-format on

/// The inn and Sunshine's house, in YallamRoomsInnActorId order.
struct ActorPlacement gYallamRoomsInnPlacements[] = {
#if !TLA_PATCH_2
    ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 152, 0, 136, FACING_SOUTHWEST,
                    TALK_FACING_PARTY_AND_BACK, 0),
#else
    ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 152, 0, 140, FACING_SOUTHWEST,
                    TALK_FACING_PARTY_AND_BACK, 0),
#endif
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS,
                    (s32)gYallamRooms_SongLoverWanders, 88, 0, 104, FACING_EAST,
                    TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_CURLY_HAIR, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 424, 0, 88, FACING_SOUTHEAST,
                    TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_SKIRT, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 454, 0, 120, FACING_SOUTHEAST,
                    TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_INNKEEPER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
                    294, 0, 408, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_INN_WORKER, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
                    184, 0, 392, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SUNSHINE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 491,
                    0, 398, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SUNSHINES_WIFE, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 560, 0, 440, FACING_WEST,
                    TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 600, 0, 346, FACING_NORTH,
                    TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
                    506, 0, 398, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

// clang-format off
/// The shops, in YallamRoomsShopsActorId order.
struct ActorPlacement gYallamRoomsShopsPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_GREY_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 144, 0, 188, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN_WITH_HEADBAND, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 184, 0, 152, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WEAPON_MERCHANT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 405, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_ARMOR_MERCHANT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 112, 0, 405, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_TEAL_HEADBAND, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 348, 0, 405, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 408, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 112, 0, 408, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 348, 0, 408, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    {SCENE_TABLE_END},
};

/// The other rooms' two exits.
struct Trigger gYallamRooms_Triggers[] = {
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_TABLE_END},
};
// clang-format on

/// The inn's exits, its people talked to and read, and the forge's oven.
struct Trigger gYallamRoomsInnTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_TALK, ACTOR_YEPP_SONGS_VILLAGER, CONDITION_ALWAYS,
     MSGNO(MsgYallamInteriorsYeppHadThreeDifferent)},
    {TRIGGER_TALK, ACTOR_CHILDRENS_SONGS_VILLAGER, CONDITION_ALWAYS,
     EVENT(YallamRooms_AskLikeChildrensSongs)},
    {TRIGGER_TALK, ACTOR_DRY_MONTHS_VILLAGER, CONDITION_ALWAYS,
     MSGNO(MsgYallamInteriorsItsBeenReallyDry)},
    {TRIGGER_TALK, ACTOR_RAINY_SEASON_VILLAGER, CONDITION_ALWAYS,
     MSGNO(MsgYallamInteriorsWereSupposedToBe)},
    {TRIGGER_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS,
     EVENT(YallamRooms_TalkToInnkeeper)},
    {TRIGGER_TALK, ACTOR_DRIED_SWAMP_VILLAGER, CONDITION_ALWAYS,
     MSGNO(MsgYallamInteriorsTaopoSwampHasDried)},
    {TRIGGER_TALK, ACTOR_SUNSHINE, CONDITION_ALWAYS,
     EVENT(YallamRooms_TalkToSunshine)},
    {TRIGGER_TALK, ACTOR_SUNSHINES_WIFE, CONDITION_ALWAYS,
     EVENT(YallamRooms_TalkToSunshinesWife)},
    {TRIGGER_TALK, ACTOR_SUNSHINES_CHILD, CONDITION_ALWAYS,
     EVENT(YallamRooms_TalkToSunshinesChild)},
    {TRIGGER_TALK, ACTOR_SUNSHINES_COUNTER, CONDITION_ALWAYS,
     EVENT(YallamRooms_TalkToSunshine)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YEPP_SONGS_VILLAGER,
     CONDITION_ALWAYS, MSGNO(MsgYallamInteriorsTheSongsAreSupposed)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CHILDRENS_SONGS_VILLAGER,
     CONDITION_ALWAYS, MSGNO(MsgYallamInteriorsIWonderWhyYepp)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_DRY_MONTHS_VILLAGER,
     CONDITION_ALWAYS, MSGNO(MsgYallamInteriorsWhenTaopoSwampDries)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_RAINY_SEASON_VILLAGER,
     CONDITION_ALWAYS, MSGNO(MsgYallamInteriorsICantStandThis)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, CONDITION_ALWAYS,
     MSGNO(MsgYallamInteriorsYallamWasAlwaysSuch)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_DRIED_SWAMP_VILLAGER,
     CONDITION_ALWAYS, MSGNO(MsgYallamInteriorsThereWerentALot)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SUNSHINE, CONDITION_ALWAYS,
     EVENT(YallamRooms_MindReadSunshine)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SUNSHINES_WIFE,
     CONDITION_ALWAYS, EVENT(YallamRooms_MindReadSunshinesWife)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SUNSHINES_CHILD,
     CONDITION_ALWAYS, EVENT(YallamRooms_MindReadSunshinesChild)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SUNSHINES_COUNTER,
     CONDITION_ALWAYS, EVENT(YallamRooms_MindReadSunshine)},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 200, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgYallamInteriorsTheSteelRestingIn))},
    {TRIGGER_TABLE_END},
};

/// The shops' exits and their people, talked to and read; each merchant answers
/// across his counter too.
struct Trigger gYallamRoomsShopsTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_TALK, ACTOR_COMPLAINT_VILLAGER, CONDITION_ALWAYS,
     MSGNO(MsgYallamInteriorsSunshineHasStartedComplaining)},
    {TRIGGER_TALK, ACTOR_BLACKSMITH_VILLAGER, CONDITION_ALWAYS,
     MSGNO(MsgYallamInteriorsOurBlacksmithSunshineMakes)},
    {TRIGGER_TALK, ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS,
     EVENT(YallamRooms_TalkToWeaponMerchant)},
    {TRIGGER_TALK, ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS,
     EVENT(YallamRooms_TalkToArmorMerchant)},
    {TRIGGER_TALK, ACTOR_ITEM_MERCHANT, CONDITION_ALWAYS,
     EVENT(YallamRooms_TalkToItemMerchant)},
    {TRIGGER_TALK, ACTOR_WEAPON_COUNTER, CONDITION_ALWAYS,
     EVENT(YallamRooms_TalkToWeaponMerchant)},
    {TRIGGER_TALK, ACTOR_ARMOR_COUNTER, CONDITION_ALWAYS,
     EVENT(YallamRooms_TalkToArmorMerchant)},
    {TRIGGER_TALK, ACTOR_ITEM_COUNTER, CONDITION_ALWAYS,
     EVENT(YallamRooms_TalkToItemMerchant)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_COMPLAINT_VILLAGER,
     CONDITION_ALWAYS, MSGNO(MsgYallamInteriorsWhatKindOfPerson)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BLACKSMITH_VILLAGER,
     CONDITION_ALWAYS, MSGNO(MsgYallamInteriorsSunshinesALittleEccentric)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEAPON_MERCHANT,
     CONDITION_ALWAYS, MSGNO(MsgYallamInteriorsIfSunshineSetHis)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARMOR_MERCHANT,
     CONDITION_ALWAYS, MSGNO(MsgYallamInteriorsIShouldntTellHim)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ITEM_MERCHANT,
     CONDITION_ALWAYS, MSGNO(MsgYallamInteriorsWhatDoesSunshineWant)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEAPON_COUNTER,
     CONDITION_ALWAYS, EVENT(YallamRooms_MindReadWeaponMerchant)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARMOR_COUNTER,
     CONDITION_ALWAYS, EVENT(YallamRooms_MindReadArmorMerchant)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ITEM_COUNTER, CONDITION_ALWAYS,
     EVENT(YallamRooms_MindReadItemMerchant)},
    {TRIGGER_TABLE_END},
};
