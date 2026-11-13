/// @file
/// The inn: room prices, checking in and the night's sleep.
// Part of shop.c.

#include "../shop_internal.h"

/// The inn's view of the shop work (heap slot 220).
struct InnRuntime {
  u8 unk000[0x4ca];
  s16 partyMemberIds[8];
  u8 unk4da[0x2a];
  s8 partyMemberCount;
  u8 unk505;
  u8 busy;
};

struct InnUnit {
  u8 unk00[0x38];
  s16 hp;
};

/// The price per guest of each inn.
extern const s8 gData_0810cf2c[];

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

s32 Func_0810b3ec(s32 value) {
  struct InnRuntime *work;

  Shop_InitializeWork();
  work = Ram_HeapSlots->menuRuntime;
  work->busy = 1;
  value = Inn_RoomPrice(value);
  Func_0810824c();
  return value;
}

// Inn_CheckIn is the inn. It shows the keeper's window and the price, then on
// yes and enough coins closes the windows, plays the night (Inn_PlaySleep) and
// greets the party in the morning. The Broken Seal's Inn_CheckIn in
// games/tbs/src/shop/shop.c, written with this game's calls
// (Shop_InitializeWork for the cursor work, Func_080380f8 for the keeper's
// window, Inn_RoomPrice for the room price, MessageArgument_PushFar,
// Shop_ShowKeeperMessage to show a message, Shop_AskYesNo for the choice,
// Func_08109188 for the coins, Func_0810824c to clean up) and its work-block
// offsets; this game has no special-inn flag. Messages from MsgInnWelcome
// ("Welcome to our inn.").

// The inn work block, as this function reads it.
struct InnRuntimeState {
  u8 unk000[0x0c];
  s32 window;
  u8 unk010[0x4ea];
  u16 resourceId;
  u8 unk4fc[0x0a];
  u8 active;
};

struct InnObjectComponent {
  u8 padding00[0x28];
  u16 *resourceId;
};

struct InnObject {
  u8 padding00[0x50];
  struct InnObjectComponent *component;
};

enum InnMessageId {
  INN_MESSAGE_WELCOME,
  INN_MESSAGE_STAY_COMPLETE,
  INN_MESSAGE_NOT_ENOUGH_COINS,
  INN_MESSAGE_GOODBYE,
  INN_MESSAGE_REST_COMPLETE,
};

void Inn_PlaySleep(s32 price);

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
  win = Func_080380f8(state->resourceId, 0, 0, 0);

  amount = Inn_RoomPrice(mode);
  MessageArgument_PushFar(amount, 5);
  messageBase = (s32)MsgInnWelcome;
  Shop_ShowKeeperMessage(messageBase);
  state->window = Window_CreateFar(0, 16, SHOP_COIN_WINDOW_WIDTH, 4, 2);
  Func_08109188();

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
    win = Func_080380f8(state->resourceId, 0, 0, 0);
    Shop_ShowKeeperMessage(messageBase + INN_MESSAGE_REST_COMPLETE);
  }

  Window_CloseFar(win, 2);
  Func_0810824c();
  return 0;
}

struct FieldEffectState {
  u8 padding0[0x1AC];
  s32 effect;
  u8 padding1B0[4];
  s32 delay;
};

void Event_WaitTransitionFramesFar(void);
void Event_StartScreenTransitionFar(void);

void Inn_PlaySleep(s32 roomPrice) {
  s16 objects[8];
  s32 count;
  s32 index;
  struct FieldObject *object;
  struct FieldEffectState *state;

  count = Party_ListActiveOwnersFar(objects);
  Party_AdjustCoinsFar(-roomPrice);

  for (index = 0; index < count; index++) {
    object = Unit_Get(objects[index]);
    if (object->x != 0) {
      object->x = object->savedX;
      object->y = object->savedY;
      Unit_UpdateRatiosFar(objects[index]);
    }
  }

  state = Ram_HeapSlots->eventWork;
  state->effect = 0x209;
  state->delay = 60;
  wait(20);
  Event_FinishScreenTransitionFar();
  Event_WaitTransitionFramesFar();
  Sound_PlayFar(86);
  Sound_WaitForJingle();
  wait(10);
  Event_StartScreenTransitionFar();
  Event_WaitTransitionFramesFar();
  wait(30);
  ((struct FieldEffectState *)Ram_HeapSlots->eventWork)->delay = 16;
}
