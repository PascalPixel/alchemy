/// @file
/// The inn: room prices, checking in and the night's sleep.
// Part of shop.c.

#include "../shop_internal.h"

/// The innkeeper's lines, in message order from MsgInnWelcome.
enum InnMessageId {
  INN_MESSAGE_WELCOME = MSGNO(MsgInnWelcome),
  INN_MESSAGE_STAY_COMPLETE,
  INN_MESSAGE_NOT_ENOUGH_COINS,
  INN_MESSAGE_GOODBYE,
  INN_MESSAGE_REST_COMPLETE,
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
/// party members still standing. Called by Inn_CheckIn.
s32 Inn_RoomPrice(s32 innId) {
  struct ShopWork *shop = gMenuWork;
  s32 guestCount = 0;
  s32 pricePerGuest = sInnPricesPerGuest[innId];
  s32 memberIndex;

  for (memberIndex = 0; memberIndex < shop->partyMemberCount; memberIndex++) {
    if (Unit_GetFar(shop->partyMemberIds[memberIndex])->hp != 0)
      guestCount++;
  }
  return pricePerGuest * guestCount;
}

/// Runs an inn's counter: the innkeeper names the price, and on Yes, with the
/// coins, the party sleeps (Inn_PlaySleep) and the innkeeper speaks again.
/// Holds the shop's work block while it runs; returns 0. Called by the town
/// scenes' innkeepers through SHOP.S. `innId` is the row of
/// sInnPricesPerGuest, 0 to 11, which nothing checks; `objectId` is the
/// innkeeper, whose portrait the lines show.
s32 Inn_CheckIn(s32 innId, s32 actorId) {
  struct InnWork *state;
  struct Actor *innkeeper;
  struct Window *innkeeperWindow;
  s32 roomPrice;
  s32 messageBase;

  Shop_InitializeWork();
  state = gMenuWork;
  state->active = 1;
  // Xian's inn has lines of its own, as far past the other inns' as the
  // warriors' shops' are past the weapon shop's (warriorShop).
  if (innId == INN_XIAN)
    state->specialActive = 1;

  innkeeper = Actor_GetByIdFar(actorId);
  state->resourceId =
      ((struct Sprite *)innkeeper->sprite)->entries[0]->animationId;
  innkeeperWindow = Portrait_OpenWindowFar(state->resourceId, 0, 0, 0);

  roomPrice = Inn_RoomPrice(innId);
  MessageArgument_PushFar(roomPrice, MESSAGE_ARGUMENT_NUMBER);
  messageBase = MsgInnWelcome;
  Shop_ShowKeeperMessage(messageBase);
  state->window =
      Window_CreateFar(0, 16, SHOP_MONEY_WINDOW_WIDTH, 4, WINDOW_FLAG_INSTANT);
  Shop_DrawMoney();

  if (Shop_AskYesNo(0) != 0) {
    Shop_ShowKeeperMessage(messageBase +
                           (INN_MESSAGE_GOODBYE - INN_MESSAGE_WELCOME));
    Window_CloseFar(state->window, 2);
  } else if (roomPrice > (u32)gMainRoutineWork.coins) {
    Shop_ShowKeeperMessage(
        messageBase + (INN_MESSAGE_NOT_ENOUGH_COINS - INN_MESSAGE_WELCOME));
    Window_CloseFar(state->window, 2);
  } else {
    Window_CloseFar(state->window, 2);
    Shop_ShowKeeperMessage(messageBase +
                           (INN_MESSAGE_STAY_COMPLETE - INN_MESSAGE_WELCOME));
    Window_CloseFar(innkeeperWindow, 2);
    Inn_PlaySleep(roomPrice);

    innkeeper = Actor_GetByIdFar(actorId);
    state->resourceId =
        ((struct Sprite *)innkeeper->sprite)->entries[0]->animationId;
    innkeeperWindow = Portrait_OpenWindowFar(state->resourceId, 0, 0, 0);
    Shop_ShowKeeperMessage(messageBase +
                           (INN_MESSAGE_REST_COMPLETE - INN_MESSAGE_WELCOME));
  }

  Window_CloseFar(innkeeperWindow, 2);
  Shop_ReleaseWork();
  return 0;
}

/// Charges `roomPrice`, restores the HP and PP of each party member still
/// standing, and plays the night: a fade to black, the inn's music and a fade
/// back. A downed party member stays down: the inn neither charges for it nor
/// heals it. Called by Inn_CheckIn.
void Inn_PlaySleep(s32 roomPrice) {
  s16 partyMemberIds[8];
  s32 memberCount;
  s32 memberIndex;
  struct Unit *unit;
  struct FieldWork *work;

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

  // Sleep between the two screen-transition halves and the rest sound.
  // The night's fades take 60 frames each; the 16 set at the end is what
  // the field's transitions take again afterward
  work = gFieldWork;
  work->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  work->transitionFrames = 60;
  wait(20);
  Event_FinishScreenTransitionFar();
  Event_WaitTransitionFramesFar();
  Sound_PlayFar(MUS_INN);
  Sound_WaitForJingle();
  wait(10);
  Event_StartScreenTransitionFar();
  Event_WaitTransitionFramesFar();
  wait(30);
  gFieldWork->transitionFrames = 16;
}
