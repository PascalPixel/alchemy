/// @file
/// The inn: room prices, checking in and the night's sleep.
/// Part of shop.c, the shop object, compiled only through it.

#include "../shop_internal.h"

enum InnMessageId {
  INN_MESSAGE_WELCOME = 0xd1c,
  INN_MESSAGE_STAY_COMPLETE,
  INN_MESSAGE_NOT_ENOUGH_COINS,
  INN_MESSAGE_GOODBYE,
  INN_MESSAGE_REST_COMPLETE,
};

/// Inn price multiplier
// clang-format off
static const s8 sInnPricesPerGuest[12] = {
    2, 4, 7, 8, 8, 10, 12, 15, 20, 0, 18, 25,
};
// clang-format on

/// Multiplies the inn's price per guest by the number of living party members.
s32 Inn_RoomPrice(s32 innId) {
  struct ShopRuntime *shop = gMenuWork;
  s32 guestCount = 0;
  s32 pricePerGuest = sInnPricesPerGuest[innId];
  s32 memberIndex;

  for (memberIndex = 0; memberIndex < shop->partyMemberCount; memberIndex++) {
    if (UnitGetFar(shop->partyMemberIds[memberIndex])->hp != 0)
      guestCount++;
  }
  return pricePerGuest * guestCount;
}

/// Offers an inn stay, checks the fee and restores the innkeeper window after
/// resting.
s32 Inn_CheckIn(s32 innId, s32 objectId) {
  struct InnRuntimeState *state;
  struct FieldActor *innkeeper;
  struct UiWindow *innkeeperWindow;
  s32 roomPrice;
  s32 messageBase;

  Shop_InitializeCursorWork();
  state = gMenuWork;
  state->active = 1;
  if (innId == 5)
    state->specialActive = 1;

  innkeeper = Object_GetByIdFar(objectId);
  state->resourceId =
      ((struct AnimationObject *)innkeeper->sprite)->entries[0]->animId;
  innkeeperWindow =
      UiWindow_CreateWithSideObjectFar(state->resourceId, 0, 0, 0);

  roomPrice = Inn_RoomPrice(innId);
  UiWork_PushValueSlotFar(roomPrice, 5);
  messageBase = MsgInnWelcome;
  UiMessage_ShowAndWait(messageBase);
  state->window = UiWindow_CreateFar(0, 16, SHOP_MONEY_WINDOW_WIDTH, 4, 2);
  Shop_DrawMoney();

  if (UiMessage_ShowChoice(0) != 0) {
    UiMessage_ShowAndWait(messageBase +
                          (INN_MESSAGE_GOODBYE - INN_MESSAGE_WELCOME));
    UiWork_FinalizeFar(state->window, 2);
  } else if (roomPrice > (u32)gGameState.coins) {
    UiMessage_ShowAndWait(messageBase +
                          (INN_MESSAGE_NOT_ENOUGH_COINS - INN_MESSAGE_WELCOME));
    UiWork_FinalizeFar(state->window, 2);
  } else {
    UiWork_FinalizeFar(state->window, 2);
    UiMessage_ShowAndWait(messageBase +
                          (INN_MESSAGE_STAY_COMPLETE - INN_MESSAGE_WELCOME));
    UiWork_FinalizeFar(innkeeperWindow, 2);
    Inn_PlaySleep(roomPrice);

    innkeeper = Object_GetByIdFar(objectId);
    state->resourceId =
        ((struct AnimationObject *)innkeeper->sprite)->entries[0]->animId;
    innkeeperWindow =
        UiWindow_CreateWithSideObjectFar(state->resourceId, 0, 0, 0);
    UiMessage_ShowAndWait(messageBase +
                          (INN_MESSAGE_REST_COMPLETE - INN_MESSAGE_WELCOME));
  }

  UiWork_FinalizeFar(innkeeperWindow, 2);
  Shop_ReleaseCursorWork();
  return 0;
}

/// Charges for the stay, restores living members' HP and PP, then plays the
/// rest transition.
void Inn_PlaySleep(s32 roomPrice) {
  s16 partyMemberIds[8];
  s32 memberCount;
  s32 memberIndex;
  struct BattleUnit *unit;
  struct FieldWork *fieldWork;

  memberCount = Party_ListActiveOwnersFar(partyMemberIds);
  // Charge once and restore only party members who are alive.
  Party_AdjustCoinsFar(-roomPrice);

  for (memberIndex = 0; memberIndex < memberCount; memberIndex++) {
    unit = UnitGetFar(partyMemberIds[memberIndex]);
    if (unit->hp != 0) {
      unit->hp = unit->maxHp;
      unit->pp = unit->maxPp;
      UnitRatioFar(partyMemberIds[memberIndex]);
    }
  }

  // Sleep between the two screen-transition halves and the rest sound.
  fieldWork = (struct FieldWork *)gEventWork;
  fieldWork->transition = 0x209;
  fieldWork->transitionFrames = 60;
  WaitFrames(20);
  Event_FinishScreenTransitionFar();
  Event_WaitTransitionFramesFar();
  AudioCommand_PlayFar(MUS_086);
  AudioCommand_WaitForStateByteClear();
  WaitFrames(10);
  Event_StartScreenTransitionFar();
  Event_WaitTransitionFramesFar();
  WaitFrames(30);
  ((struct FieldWork *)gEventWork)->transitionFrames = 16;
}
