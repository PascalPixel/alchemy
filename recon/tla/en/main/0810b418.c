// Near miss, differ 2 of 108: the inn. Shows the keeper's window and the price,
// then on yes and enough coins closes the windows, plays the night
// (Inn_PlaySleep) and greets the party in the morning. ☀️'s Inn_CheckIn in
// games/tbs/src/shop/shop.c, written with ⚓️'s calls (Func_08108148 for the
// cursor work, Func_080380f8 for the keeper's window, Func_0810b378 for the
// room price, UiText_DrawQuantity, Func_081084f4 to show a message,
// Func_08108630 for the choice, Func_08109188 for the coins, Func_0810824c to
// clean up) and its work-block offsets; ⚓️ has no special-inn flag. Messages
// from Msg04813 ("Welcome to our inn."). Instruction order only: after the
// night, the reload of the keeper's resource and the window call's last zero
// argument swap.
#include "types.h"
#include "party_state.h"
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

extern u8 Msg04813[]; // "Welcome to our inn." (MsgInnWelcome)

void Func_08108148(void);
void Func_0810824c(void);
struct InnObject *Object_GetByIdFar(s32 id);
s32 Func_080380f8(s32 resource, s32 x, s32 y, s32 flags);
s32 Func_0810b378(s32 mode);
void UiText_DrawQuantity(s32 value, s32 slot);
void Func_081084f4(s32 message);
s32 UiWindow_CreateFar(s32 x, s32 y, s32 width, s32 height, s32 style);
void Func_08109188(void);
s32 Func_08108630(s32 value);
void UiWork_FinalizeFar(s32 window, s32 style);
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

  object = Object_GetByIdFar(objectId);
  state->resourceId = *object->component->resourceId;
  win = Func_080380f8(state->resourceId, 0, 0, 0);

  amount = Func_0810b378(mode);
  UiText_DrawQuantity(amount, 5);
  messageBase = (s32)Msg04813;
  Func_081084f4(messageBase);
  state->window = UiWindow_CreateFar(0, 16, 12, 4, 2);
  Func_08109188();

  if (Func_08108630(0) != 0) {
    Func_081084f4(messageBase + INN_MESSAGE_GOODBYE);
    UiWork_FinalizeFar(state->window, 2);
  } else if ((u32)amount > gPartyState.coins) {
    Func_081084f4(messageBase + INN_MESSAGE_NOT_ENOUGH_COINS);
    UiWork_FinalizeFar(state->window, 2);
  } else {
    UiWork_FinalizeFar(state->window, 2);
    Func_081084f4(messageBase + INN_MESSAGE_STAY_COMPLETE);
    UiWork_FinalizeFar(win, 2);
    Inn_PlaySleep(amount);

    object = Object_GetByIdFar(objectId);
    state->resourceId = *object->component->resourceId;
    win = Func_080380f8(state->resourceId, 0, 0, 0);
    Func_081084f4(messageBase + INN_MESSAGE_REST_COMPLETE);
  }

  UiWork_FinalizeFar(win, 2);
  Func_0810824c();
  return 0;
}
