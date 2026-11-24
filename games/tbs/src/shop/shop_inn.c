/// @file
/// The inn: room prices, checking in and the night's sleep.
// Part of shop.c.

#include "shop_internal.h"

/// The innkeeper's lines, in message order from MsgInnWelcome.
enum InnMessageId {
  INN_MESSAGE_WELCOME = MSGNO(MsgInnWelcome),
  INN_MESSAGE_ENJOY_YOUR_STAY,
  INN_MESSAGE_NOT_ENOUGH_COINS,
  INN_MESSAGE_COME_AGAIN,
  INN_MESSAGE_SLEPT_WELL,
};

/// The coins a night costs each living guest, by inn: Vale, Vault, Bilibin,
/// Kolima, Imil, Xian, Altin, Kalay, Lunpa, Tolbi, Suhalla, Lalivero, in the
/// order of their area names. Unused: Tolbi's row, 9, costs nothing and no
/// event opens it, since Tolbi's inns are always full
/// (MsgTolbiInteriorsIsaacCheckedTheSignInnFull).
// clang-format off
static const s8 sInnPricesPerGuest[12] = {
    2, 4, 7, 8, 8, 10, 12, 15, 20, 0, 18, 25,
};
// clang-format on

/// Returns the night's price: the inn's price per guest times the party
/// members still standing.
s32 Inn_RoomPrice(s32 innId) {
  struct ShopState *shop = gMenuState;
  s32 guestCount = 0;
  s32 pricePerGuest = sInnPricesPerGuest[innId];
  s32 memberIndex;

  for (memberIndex = 0; memberIndex < shop->partyMemberCount; memberIndex++) {
    if (Unit_GetFar(shop->partyMemberIds[memberIndex])->hp != 0)
      guestCount++;
  }
  return pricePerGuest * guestCount;
}

/// Runs an inn's counter: the innkeeper names the price, and on Yes the party
/// sleeps. actorId is the innkeeper, whose portrait the lines show.
s32 Inn_CheckIn(s32 innId, s32 actorId) {
  struct ShopState *inn;
  struct Actor *innkeeper;
  struct Window *innkeeperWindow;
  u32 roomPrice;
  s32 messageBase;

  Shop_InitializeState();
  inn = gMenuState;
  // The innkeeper's lines are given as they are, as a weapon shop's are
  // (Shop_ShowKeeperMessage); Xian's inn has lines of its own, as far past the
  // other inns' as the warriors' shops' are past the weapon shop's.
  inn->shopType = SHOP_TYPE_WEAPONS;
  if (innId == INN_XIAN)
    inn->warriorShop = 1;

  innkeeper = Actor_GetByIdFar(actorId);
  inn->keeperGraphics = innkeeper->sprite->entries[0]->animationId;
  innkeeperWindow = Portrait_OpenWindowFar(inn->keeperGraphics, 0, 0, 0);

  roomPrice = Inn_RoomPrice(innId);
  MessageArgument_PushFar(roomPrice, MESSAGE_ARGUMENT_NUMBER);
  messageBase = MsgInnWelcome;
  // "Welcome to our inn. It's (the price) coins for a room. Would you like to
  // stay?" These are the inns' lines; Xian's says its own.
  Shop_ShowKeeperMessage(messageBase);
  inn->moneyWindow =
      Window_CreateFar(0, 16, SHOP_MONEY_WINDOW_WIDTH, 4, WINDOW_FLAG_INSTANT);
  Shop_DrawMoney();

  if (Shop_AskYesNo(0) != 0) {
    // "Please come again."
    Shop_ShowKeeperMessage(messageBase +
                           (INN_MESSAGE_COME_AGAIN - INN_MESSAGE_WELCOME));
    Window_CloseFar(inn->moneyWindow, 2);
  } else if (roomPrice > gMainRoutineState.holdings.coins) {
    // "I'm afraid you don't have enough."
    Shop_ShowKeeperMessage(
        messageBase + (INN_MESSAGE_NOT_ENOUGH_COINS - INN_MESSAGE_WELCOME));
    Window_CloseFar(inn->moneyWindow, 2);
  } else {
    Window_CloseFar(inn->moneyWindow, 2);
    // "Please enjoy your stay."
    Shop_ShowKeeperMessage(messageBase +
                           (INN_MESSAGE_ENJOY_YOUR_STAY - INN_MESSAGE_WELCOME));
    Window_CloseFar(innkeeperWindow, 2);
    Inn_PlaySleep(roomPrice);

    innkeeper = Actor_GetByIdFar(actorId);
    inn->keeperGraphics = innkeeper->sprite->entries[0]->animationId;
    innkeeperWindow = Portrait_OpenWindowFar(inn->keeperGraphics, 0, 0, 0);
    // "I hope you slept well. Have a nice trip!"
    Shop_ShowKeeperMessage(messageBase +
                           (INN_MESSAGE_SLEPT_WELL - INN_MESSAGE_WELCOME));
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
  // Charge once and restore only party members who are alive.
  Party_AdjustCoinsFar(-roomPrice);

  for (memberIndex = 0; memberIndex < memberCount; memberIndex++) {
    unit = Unit_GetFar(partyMemberIds[memberIndex]);
    if (unit->hp != 0) {
      unit->hp = unit->hpMax;
      unit->pp = unit->ppMax;
      Unit_RecalculateGaugesFar(partyMemberIds[memberIndex]);
    }
  }

  // The night's fades take 60 frames each.
  state = gFieldState;
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
  gFieldState->transitionFrames = MAP_TRANSITION_FRAMES;
}
