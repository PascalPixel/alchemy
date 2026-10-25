// Shop_ConfirmAct is the sanctum's service loop. It opens the priest's window
// (his face resource from the actor's sprite, or a plain window), the cursor
// and the coins window, then offers the party service until the menu is
// cancelled. The Broken Seal's Shop_ConfirmAct in games/tbs/src/shop/shop.c,
// written with this game's calls (Func_08108148 for the cursor work,
// Func_080380f8 for the side-object window, Func_08108aa8 to place the cursor,
// Func_0810a9ac to show a message, Shop_CountUnits to count the downed,
// Func_0810abf0 for the service, Func_0810824c to clean up), its work-block
// offsets and numbered messages.
#include "types.h"
#include "render_input.h"
#include "ram_buffer.h"

struct ShopCursor {
  struct RenderOutput *anchor;
  u16 x;
  u16 y;
  s16 targetX;
  s16 targetY;
  s8 active;
  s8 kind;
};

// The shop work block, as this function reads it.
struct ShopRuntime {
  u8 unk000[0x0c];
  s32 moneyWindow;
  u8 unk010[0x4cc];
  struct ShopCursor cursor;
  u16 cursorResource;
  u8 unk4ee[0x0c];
  u16 keeperResource;
  u8 unk4fc[0x0b];
  u8 partyAction;
};

struct ShopCursorAnchor {
  u8 unk00[5];
  u8 kind;
};

extern u8 MsgWelcomeWearyWanderers[];
extern u8 MsgSanctumMoreAid[];
extern u8 MsgSanctumFarewell[];
extern u8 MsgReviveService[]; // "Hmm, you were downed ...", then "Fear not ..."

void Func_08108148(void);
void *Object_GetByIdFar(s32 unitId);
s32 Func_080380f8(s32 resource, s32 x, s32 y, s32 flags);
s32 UiWindow_CreateFar(s32 x, s32 y, s32 width, s32 height, s32 style);
void SideObject_CreateFar(s32 a, s32 b, s32 c, s32 window, s32 d, s32 e);
void Func_08108aa8(struct ShopCursor *cursor, s32 targetX, s32 targetY);
void Func_0810a9ac(s32 message);
void Func_08109188(void);
s32 Shop_CountUnits(void);
void Func_0810abf0(void);
s32 Menu_SelectSanctumServiceFar(s32 prev);
void UiWork_FinalizeFar(s32 window, s32 style);
void Func_0810824c(void);

s32 Shop_ConfirmAct(s32 unitId) {
  s32 partyAction = 0;
  s32 listWindow = 0;
  struct ShopRuntime *shop;
  struct ShopCursorAnchor *cursorAnchor;

  Func_08108148();
  shop = Ram_HeapSlots->menuRuntime;
  /* FAKEMATCH(barrier): an empty asm on the work pointer with an r2 clobber,
   * so its load comes before the action offset's first move; as the plain
   * load, 4 of 153 instructions differ in tla-en. */
  asm("" : "+l"(shop) : : "r2");
  shop->partyAction = listWindow;

  shop->keeperResource =
      **(u16 **)(*(u8 **)((u8 *)Object_GetByIdFar(unitId) + 80) + 40);

  listWindow = Func_080380f8(shop->keeperResource, 0, 0, 0);
  if (listWindow == 0) {
    listWindow = UiWindow_CreateFar(-5, 0, 5, 5, 2);
    if (listWindow == 0) {
      listWindow = UiWindow_CreateFar(0, 0, 5, 5, 2);
      SideObject_CreateFar(2, 0, 0, listWindow, -4, -4);
    }
  }

  cursorAnchor = (struct ShopCursorAnchor *)RenderOutput_CreateFar(
      shop->cursorResource, 0x40000000, (struct RenderInput *)listWindow, 0, 0);
  cursorAnchor->kind = 1;
  cursorAnchor->unk00[4] = 0;
  Func_08108aa8(&shop->cursor, -32, 112);
  shop->cursor.anchor = (struct RenderOutput *)cursorAnchor;
  Func_0810a9ac((s32)MsgWelcomeWearyWanderers);

  shop->moneyWindow = UiWindow_CreateFar(16, 11, 12, 4, 2);
  Func_08109188();

  for (;;) {
    partyAction = Menu_SelectSanctumServiceFar(partyAction);
    shop->partyAction = partyAction;
    if (partyAction == -1)
      break;

    Func_0810a9ac((s32)MsgReviveService);
    if (Shop_CountUnits() == 0) {
      Func_0810a9ac((s32)MsgReviveService + 1);
    } else {
      Func_0810abf0();
    }
    shop->partyAction = 0;
    Func_08108aa8(&shop->cursor, -32, 112);
    Func_0810a9ac((s32)MsgSanctumMoreAid);
  }

  Func_0810a9ac((s32)MsgSanctumFarewell);
  UiWork_FinalizeFar(shop->moneyWindow, 2);
  UiWork_FinalizeFar(listWindow, 2);
  Func_0810824c();
  return 0;
}
