// Func_081082c4 runs the shop's buy, sell, artifacts and repair menu with the
// selected shopkeeper; the twin of The Broken Seal's Shop_Run
// (games/tbs/src/shop/shop.c), without its warrior shop rows. The messages take
// the twin's names.
#include "types.h"
#include "ram_buffer.h"
#include "render_input.h"

struct ShopCursor {
  struct RenderOutput *anchor;
};

struct ShopRuntime {
  u8 unk000[0x270];
  u16 stockItemIds[0x136];
  struct ShopCursor cursor;
  u8 unk4e0[0xc];
  u16 cursorIcon;
  u8 unk4ee[0xc];
  u16 keeperResource;
  u8 unk4fc[6];
  u16 stockCount;
  u8 unk504[2];
  s8 shopType;
  s8 partyAction;
};

extern u8 MsgWeaponShopWelcome[], MsgWhatWouldYouLike[], MsgWhatToSell[],
    MsgConnoisseur[], MsgOutOfStock[], MsgFixDamaged[], MsgAnythingElse[],
    MsgShopFarewell[];

s32 EventTable_GetRowLimit(void);
s32 EventTable_GetRowType(s32 row);
void EventTable_ApplyRowAbilities(s32 row);
void Func_08108148(void);
void *Object_GetByIdFar(s32 id);
struct RenderInput *Func_080380f8(s32 resource, s32 x, s32 y, s32 mode);
struct RenderInput *UiWindow_CreateFar(s32 x, s32 y, s32 width, s32 height,
                                       s32 style);
void Func_08108aa8(struct ShopCursor *cursor, s32 x, s32 y);
void Func_081084f4(s32 message);
s32 Menu_SelectShopCommandFar(s32 action);
s32 Func_0810a6b8(s32 row, u16 *items);
void Func_08108b70(void);
void Func_08109ad8(void);
s32 Func_081080a8(void);
void Func_0810a2c8(void);
void UiWork_FinalizeFar(struct RenderInput *window, s32 mode);
void Func_0810824c(void);
void WaitTicks(s32 ticks);

s32 Func_081082c4(s32 stockRow, s32 keeperId) {
  struct ShopRuntime *shop;
  struct RenderOutput *anchor;
  struct RenderInput *window;
  s32 action = 0;

  if (stockRow >= EventTable_GetRowLimit() || stockRow < 0)
    stockRow = 0;
  EventTable_ApplyRowAbilities(stockRow);
  Func_08108148();
  shop = Ram_HeapSlots->menuRuntime;
  shop->shopType = EventTable_GetRowType(stockRow);
  shop->keeperResource =
      **(u16 **)(*(u8 **)((u8 *)Object_GetByIdFar(keeperId) + 80) + 40);
  window = Func_080380f8(shop->keeperResource, 0, 0, 0);
  if (window == 0)
    window = UiWindow_CreateFar(30, 0, 2, 2, 2);
  anchor = RenderOutput_CreateFar(shop->cursorIcon, 0x40000000, window, 0, 0);
  anchor->sentinel = 255;
  anchor->active = 1;
  anchor->kind = 0;
  Func_08108aa8(&shop->cursor, -32, 112);
  shop->cursor.anchor = anchor;
  Func_081084f4((s32)MsgWeaponShopWelcome);
  while (1) {
    action = Menu_SelectShopCommandFar(action);
    shop->partyAction = action;
    if (action == 0) {
      shop->stockCount = Func_0810a6b8(stockRow, shop->stockItemIds);
      Func_081084f4((s32)MsgWhatWouldYouLike);
      Func_08108b70();
    } else if (action == 1) {
      Func_081084f4((s32)MsgWhatToSell);
      Func_08109ad8();
    } else if (action == 2) {
      if (Func_081080a8() != 0) {
        Func_081084f4((s32)MsgConnoisseur);
        Func_08108b70();
      } else {
        Func_081084f4((s32)MsgOutOfStock);
        WaitTicks(1);
      }
    } else if (action == 3) {
      Func_081084f4((s32)MsgFixDamaged);
      Func_0810a2c8();
    } else {
      break;
    }
    Func_08108aa8(&shop->cursor, -32, 112);
    Func_081084f4((s32)MsgAnythingElse);
  }
  Func_081084f4((s32)MsgShopFarewell);
  UiWork_FinalizeFar(window, 2);
  Func_0810824c();
  return 0;
}
