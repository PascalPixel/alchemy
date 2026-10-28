#include "types.h"
#include "menu.h"
#include "render_input.h"
#include "ram_buffer.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "sound.h"
#include "system.h"
#include "dma.h"
#include "resource.h"
#include "mem.h"
#include "io_reg.h"
#include "input.h"
#include "text/msg_ids.h"
#include "edition.h"

struct ShopRuntime {
  u8 unk000[0x4dc];
  struct WindowSprite *anchor;
  u8 unk4e0[0x1a];
  u16 keeperResource;
  u8 unk4fc[0xa];
  s8 shopType;
};

struct Record_080b06c0 {
  u8 filler0[4];
  u8 values[21];
};
s32 Message_AllTypedFar(void);
void Message_CloseFinishedWindowsFar(void);

void Func_081084f4(s32 first) {
  struct ShopRuntime *state = Ram_HeapSlots->menuRuntime;
  s32 message;
  s32 value;
  s8 mode;

  /* FAKEMATCH: the native loads the shop work before it builds the keeper
   * offset, and saves the message after; the barrier and the copy after it give
   * that order (differ 2 without them). */
  asm volatile("");
  message = first;
  value = BattleEffect_GetResourceIdFar(state->keeperResource);
  Message_CloseFinishedWindowsFar();
  mode = state->shopType;
  if (mode == 3)
    message += MsgGeneralStoreWelcome - MsgWeaponShopWelcome;
  if (mode == 2)
    message += MsgArmorShopWelcome - MsgWeaponShopWelcome;
  if (mode == 0)
    message += MsgItemShopWelcome - MsgWeaponShopWelcome;
  Message_OpenWindowFar(message, 5, 0, (value << 16) | 0x22);
  while (Message_AllTypedFar() == 0)
    wait(1);
  wait(1);
}

void Func_0810857c(s32 messageId) {
  s32 variant;
  s32 no;
  s8 mode;
  u8 saved;
  struct ShopRuntime *state;
  struct WindowSprite **slot;

  /* FAKEMATCH: the one-pass block loads the shop work before the anchor offset
   * is built (differ 2 without it). */
  do {
    state = Ram_HeapSlots->menuRuntime;
  } while (0);
  slot = &state->anchor;
  saved = (*slot)->active;
  no = messageId;
  variant = BattleEffect_GetResourceIdFar(state->keeperResource);
  mode = state->shopType;
  if (mode == 3)
    no += MsgGeneralStoreWelcome - MsgWeaponShopWelcome;
  if (mode == 2)
    no += MsgArmorShopWelcome - MsgWeaponShopWelcome;
  if (mode == 0)
    no += MsgItemShopWelcome - MsgWeaponShopWelcome;
  (*slot)->active = 0xD;
  Message_CloseFinishedWindowsFar();
  Message_OpenWindowFar(no, 5, 0, (variant << 0x10) | 0x22);
  while (Message_AllTypedFar() == 0)
    wait(1);
  wait(1);
  state->anchor->active = saved;
}

s32 Func_08108630(s32 arg0) {
  struct WindowSprite **slot =
      &((struct ShopRuntime *)Ram_HeapSlots->menuRuntime)->anchor;
  u8 saved = (*slot)->active;

  Icon_PrepareActorFar(*slot);
#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_SPANISH) ||                      \
    (LANGUAGE == LANGUAGE_ITALIAN)
  arg0 = Menu_RunConfirmSelectionAtFar(6, 5, arg0);
#else
  arg0 = Menu_RunConfirmSelectionAtFar(7, 5, arg0);
#endif
  (*slot)->active = saved;
  return arg0;
}

s32 Func_08108660(s32 arg0) {
  struct WindowSprite **slot =
      &((struct ShopRuntime *)Ram_HeapSlots->menuRuntime)->anchor;
  u8 saved = (*slot)->active;

  Icon_PrepareActorFar(*slot);
  arg0 = Menu_RunConfirmSelectionAtFar(7, 7, arg0);
  (*slot)->active = saved;
  return arg0;
}
