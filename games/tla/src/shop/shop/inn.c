/// @file
/// The inn: room prices, checking in and the night's sleep.
// Part of shop.c.

#include "../shop_internal.h"
#include "sounds.h"
#include "gamepak/far_call/field_sequence.h"
#include "module/shop/shop.h"

/// Multiplies the inn's price per guest by the number of living party members.
// As ☀️'s Inn_RoomPrice.
s32 Inn_RoomPrice(s32 innId) {
  struct InnRuntime *inn;
  s32 guestCount;
  s32 pricePerGuest;
  s32 memberIndex;

  inn = Ram_HeapSlots->menuRuntime;
  guestCount = 0;
  pricePerGuest = gData_0810cf2c[innId];
  for (memberIndex = 0; memberIndex < inn->partyMemberCount; memberIndex++) {
    if (((struct InnUnit *)Unit_Get(inn->partyMemberIds[memberIndex]))->hp != 0)
      guestCount++;
  }
  return pricePerGuest * guestCount;
}

/// Returns the night's price at inn `value` with the shop's work opened for
/// it, for an event to quote before checking in. Called through the far-call
/// table.
s32 Inn_QuoteRoomPrice(s32 value) {
  struct InnRuntime *work;

  Shop_InitializeWork();
  work = Ram_HeapSlots->menuRuntime;
  work->busy = 1;
  value = Inn_RoomPrice(value);
  Shop_ReleaseWork();
  return value;
}

// Inn_CheckIn is the inn. It shows the keeper's window and the price, then on
// yes and enough coins closes the windows, plays the night (Inn_PlaySleep) and
// greets the party in the morning. The Broken Seal's Inn_CheckIn in
// games/tbs/src/shop/shop.c, written with this game's calls
// (Shop_InitializeWork for the cursor work, Portrait_OpenWindowFar for the
// keeper's window, Inn_RoomPrice for the room price, MessageArgument_PushFar,
// Shop_ShowKeeperMessage to show a message, Shop_AskYesNo for the choice,
// Shop_DrawMoney for the coins, Shop_ReleaseWork to clean up) and its
// work-block offsets; this game has no special-inn flag. Messages from
// MsgInnWelcome ("Welcome to our inn.").

s32 Inn_CheckIn(s32 mode, s32 objectId) {
  struct InnRuntimeState *state;
  struct InnObject *object;
  s32 win;
  s32 amount;
  s32 messageBase;

  Shop_InitializeWork();
  state = Ram_HeapSlots->menuRuntime;
  state->active = 1;

  object = Actor_GetByIdFar(objectId);
  state->resourceId = *object->component->resourceId;
  win = Portrait_OpenWindowFar(state->resourceId, 0, 0, 0);

  amount = Inn_RoomPrice(mode);
  MessageArgument_PushFar(amount, 5);
  messageBase = (s32)MsgInnWelcome;
  Shop_ShowKeeperMessage(messageBase);
  state->window = Window_CreateFar(0, 16, SHOP_COIN_WINDOW_WIDTH, 4, 2);
  Shop_DrawMoney();

  if (Shop_AskYesNo(0) != 0) {
    Shop_ShowKeeperMessage(messageBase + INN_MESSAGE_GOODBYE);
    Window_CloseFar(state->window, 2);
  } else if ((u32)amount > gMainRoutineWork.coins) {
    Shop_ShowKeeperMessage(messageBase + INN_MESSAGE_NOT_ENOUGH_COINS);
    Window_CloseFar(state->window, 2);
  } else {
    Window_CloseFar(state->window, 2);
    Shop_ShowKeeperMessage(messageBase + INN_MESSAGE_STAY_COMPLETE);
    Window_CloseFar(win, 2);
    Inn_PlaySleep(amount);

    object = Actor_GetByIdFar(objectId);
    state->resourceId = *object->component->resourceId;
    win = Portrait_OpenWindowFar(state->resourceId, 0, 0, 0);
    Shop_ShowKeeperMessage(messageBase + INN_MESSAGE_REST_COMPLETE);
  }

  Window_CloseFar(win, 2);
  Shop_ReleaseWork();
  return 0;
}

void Inn_PlaySleep(s32 roomPrice) {
  s16 objects[8];
  s32 count;
  s32 index;
  struct FieldObject *object;
  struct FieldEffectState *state;

  count = Party_ListMembersFar(objects);
  Party_AdjustCoinsFar(-roomPrice);

  for (index = 0; index < count; index++) {
    object = Unit_Get(objects[index]);
    if (object->x != 0) {
      object->x = object->savedX;
      object->y = object->savedY;
      Unit_RecalculateGaugesFar(objects[index]);
    }
  }

  state = Ram_HeapSlots->eventWork;
  state->effect = 0x209;
  state->delay = 60;
  wait(20);
  Event_FinishScreenTransitionFar();
  Event_WaitTransitionFramesFar();
  Sound_PlayFar(MUS_086);
  Sound_WaitForJingle();
  wait(10);
  Event_StartScreenTransitionFar();
  Event_WaitTransitionFramesFar();
  wait(30);
  ((struct FieldEffectState *)Ram_HeapSlots->eventWork)->delay = 16;
}
