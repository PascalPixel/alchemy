// Inn_CheckIn is the inn. It shows the keeper's window and the price, then on
// yes and enough coins closes the windows, plays the night (Inn_PlaySleep) and
// greets the party in the morning. The Broken Seal's Inn_CheckIn in
// games/tbs/src/shop/shop.c, written with this game's calls (Func_08108148 for
// the cursor work, Func_080380f8 for the keeper's window, Inn_RoomPrice for the
// room price, MessageArgument_PushFar, Func_081084f4 to show a message,
// Func_08108630 for the choice, Func_08109188 for the coins, Func_0810824c to
// clean up) and its work-block offsets; this game has no special-inn flag.
// Messages from MsgInnWelcome ("Welcome to our inn.").
#include "types.h"
#include "shop.h"
#include "sequence_parent_work.h"
#include "ram_buffer.h"

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

extern u8 MsgInnWelcome[];

void Func_08108148(void);
void Func_0810824c(void);
struct InnObject *Actor_GetByIdFar(s32 id);
s32 Func_080380f8(s32 resource, s32 x, s32 y, s32 flags);
s32 Inn_RoomPrice(s32 mode);
void MessageArgument_PushFar(s32 value, s32 slot);
void Func_081084f4(s32 message);
s32 Window_CreateFar(s32 x, s32 y, s32 width, s32 height, s32 style);
s32 Func_08108630(s32 value);
void Window_CloseFar(s32 window, s32 style);
void Inn_PlaySleep(s32 price);

s32 Inn_CheckIn(s32 mode, s32 objectId) {
  struct InnRuntimeState *state;
  struct InnObject *object;
  s32 win;
  s32 amount;
  s32 messageBase;

  Func_08108148();
  state = Ram_HeapSlots->menuRuntime;
  state->active = 1;

  object = Actor_GetByIdFar(objectId);
  state->resourceId = *object->component->resourceId;
  win = Func_080380f8(state->resourceId, 0, 0, 0);

  amount = Inn_RoomPrice(mode);
  MessageArgument_PushFar(amount, 5);
  messageBase = (s32)MsgInnWelcome;
  Func_081084f4(messageBase);
  state->window = Window_CreateFar(0, 16, SHOP_COIN_WINDOW_WIDTH, 4, 2);
  Func_08109188();

  if (Func_08108630(0) != 0) {
    Func_081084f4(messageBase + INN_MESSAGE_GOODBYE);
    Window_CloseFar(state->window, 2);
  } else if ((u32)amount > gSequenceParentWork.coins) {
    Func_081084f4(messageBase + INN_MESSAGE_NOT_ENOUGH_COINS);
    Window_CloseFar(state->window, 2);
  } else {
    Window_CloseFar(state->window, 2);
    Func_081084f4(messageBase + INN_MESSAGE_STAY_COMPLETE);
    Window_CloseFar(win, 2);
    Inn_PlaySleep(amount);

    object = Actor_GetByIdFar(objectId);
    state->resourceId = *object->component->resourceId;
    win = Func_080380f8(state->resourceId, 0, 0, 0);
    Func_081084f4(messageBase + INN_MESSAGE_REST_COMPLETE);
  }

  Window_CloseFar(win, 2);
  Func_0810824c();
  return 0;
}
