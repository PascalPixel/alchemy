/// @file
/// The inn: room prices, checking in and the night's sleep.
// Part of shop.c.

#include "shop_internal.h"
#include "sounds.h"
#include "gamepak/far_call/field_sequence.h"
#include "shop/shop.h"

/// The innkeeper's lines, in message order from MsgInnWelcome ("Welcome to our
/// inn.").
enum InnMessageId {
  INN_MESSAGE_WELCOME,          ///< "Welcome to our inn."
  INN_MESSAGE_ENJOY_STAY,       ///< "Please enjoy your stay."
  INN_MESSAGE_NOT_ENOUGH_COINS, ///< "I'm afraid you don't have enough."
  INN_MESSAGE_COME_AGAIN,       ///< "Please come again."
  INN_MESSAGE_SLEPT_WELL,       ///< "I hope you slept well."
};

/// Returns the night's price: the inn's price per guest times the party
/// members still standing.
s32 Inn_RoomPrice(s32 innId) {
  struct ShopState *shop;
  s32 guestCount;
  s32 pricePerGuest;
  s32 memberIndex;

  shop = Ram_HeapSlots->menuState;
  guestCount = 0;
  pricePerGuest = gInn_PricesPerGuest[innId];
  for (memberIndex = 0; memberIndex < shop->partyMemberCount; memberIndex++) {
    if (Unit_Get(shop->partyMemberIds[memberIndex])->hp != 0)
      guestCount++;
  }
  return pricePerGuest * guestCount;
}

/// Returns the night's price at inn `innId` with the shop's state opened for
/// it, for an event to quote before checking in.
s32 Inn_QuoteRoomPrice(s32 innId) {
  struct ShopState *shop;

  Shop_InitializeState();
  shop = Ram_HeapSlots->menuState;
  shop->shopType = SHOP_TYPE_WEAPONS;
  innId = Inn_RoomPrice(innId);
  Shop_ReleaseState();
  return innId;
}

/// Runs an inn's counter: the innkeeper names the price, and on Yes the party
/// sleeps. `innId` is the row of gInn_PricesPerGuest and `actorId` the
/// innkeeper, whose portrait the lines show.
s32 Inn_CheckIn(s32 innId, s32 actorId) {
  struct ShopState *shop;
  struct Actor *innkeeper;
  struct Window *innkeeperWindow;
  s32 roomPrice;
  s32 messageBase;

  Shop_InitializeState();
  shop = Ram_HeapSlots->menuState;
  shop->shopType = SHOP_TYPE_WEAPONS;

  innkeeper = Actor_GetByIdFar(actorId);
  shop->keeperGraphics =
      ((struct Sprite *)innkeeper->sprite)->entries[0]->animationId;
  innkeeperWindow = Portrait_OpenWindowFar(shop->keeperGraphics, 0, 0, 0);

  roomPrice = Inn_RoomPrice(innId);
  MessageArgument_PushFar(roomPrice, MESSAGE_ARGUMENT_NUMBER);
  messageBase = (s32)MsgInnWelcome;
  Shop_ShowKeeperMessage(messageBase);
  shop->moneyWindow =
      Window_CreateFar(0, 16, SHOP_COIN_WINDOW_WIDTH, 4, WINDOW_FLAG_INSTANT);
  Shop_DrawMoney();

  if (Shop_AskYesNo(0) != 0) {
    Shop_ShowKeeperMessage(messageBase + INN_MESSAGE_COME_AGAIN);
    Window_CloseFar(shop->moneyWindow, 2);
  } else if ((u32)roomPrice > gMainRoutineState.coins) {
    Shop_ShowKeeperMessage(messageBase + INN_MESSAGE_NOT_ENOUGH_COINS);
    Window_CloseFar(shop->moneyWindow, 2);
  } else {
    Window_CloseFar(shop->moneyWindow, 2);
    Shop_ShowKeeperMessage(messageBase + INN_MESSAGE_ENJOY_STAY);
    Window_CloseFar(innkeeperWindow, 2);
    Inn_PlaySleep(roomPrice);

    innkeeper = Actor_GetByIdFar(actorId);
    shop->keeperGraphics =
        ((struct Sprite *)innkeeper->sprite)->entries[0]->animationId;
    innkeeperWindow = Portrait_OpenWindowFar(shop->keeperGraphics, 0, 0, 0);
    Shop_ShowKeeperMessage(messageBase + INN_MESSAGE_SLEPT_WELL);
  }

  Window_CloseFar(innkeeperWindow, 2);
  Shop_ReleaseState();
  return 0;
}

/// Charges the room and restores the HP and PP of each party member still
/// standing over a night: a fade to black, the inn's music and a fade back.
void Inn_PlaySleep(s32 roomPrice) {
  s16 partyMemberIds[8];
  s32 memberCount;
  s32 memberIndex;
  struct Unit *unit;
  struct FieldState *state;

  memberCount = Party_ListMembersFar(partyMemberIds);
  Party_AdjustCoinsFar(-roomPrice);

  for (memberIndex = 0; memberIndex < memberCount; memberIndex++) {
    unit = Unit_Get(partyMemberIds[memberIndex]);
    if (unit->hp != 0) {
      unit->hp = unit->hpMax;
      unit->pp = unit->ppMax;
      Unit_RecalculateGaugesFar(partyMemberIds[memberIndex]);
    }
  }

  // The night's fades take 60 frames each, a window closing round the leader
  // as The Broken Seal's.
  state = Ram_HeapSlots->fieldState;
  state->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  state->transitionFrames = 60;
  wait(20);
  Event_FinishScreenTransitionFar();
  Event_WaitTransitionFramesFar();
  Sound_PlayFar(MUS_OVERNIGHT_AT_THE_INN);
  Sound_WaitForJingle();
  wait(10);
  Event_StartScreenTransitionFar();
  Event_WaitTransitionFramesFar();
  wait(30);
  gFieldState->transitionFrames = 16;
}
