// Near miss, differ 6 of 167 (2026-10-19; was 8):
// sells count of a unit's item at slot (count -1
// sells the whole old item), ☀️'s Shop_SellItem in games/tbs/src/shop/shop.c,
// rewritten with ⚓️'s calls (UnitGet, Shop_GetSellPrice, UiText_DrawQuantity,
// the shop message and choice routines Func_0810857c and Func_08108630,
// Inventory_DiscardFar, then Unit_RefreshPsynergyFar and UnitCalcFar, which ☀️
// does not call) and numbered messages, their ☀️ names beside them. Instruction
// order only: the work pointer's address build against the frame setup, the
// slot offset's copy and the 0x1ff mask's first move against their neighbours.
// buy.c's barrier before the 0x1ff mask now orders the mask (8 to 6).
#include "battle_system.h"
#include "battle_unit.h"
#include "item.h"
#include "ram_buffer.h"

// The shop work block, as this function reads it.
struct ShopRuntime {
  u8 unk000[0x24];
  s32 itemWindow;
};

extern u8 Msg04700[]; // MsgCannotSellEquipped
extern u8 Msg04701[]; // MsgWeDoNotBuy
extern u8 Msg04703[]; // MsgSellOffer
extern u8 Msg04704[]; // MsgRareSellOffer
extern u8 Msg04705[]; // MsgSellTotal
extern u8 Msg04706[]; // MsgDamagedSellOffer
extern u8 Msg04707[]; // MsgOldItemSellOffer
extern u8 Msg04708[]; // MsgDeal
extern u8 Msg04709[]; // MsgChangedMind
extern u8 Msg04710[]; // MsgSold
extern u8 Msg04711[]; // MsgKeepIt

struct BattleUnit *UnitGet(s32 unitId);
s32 Shop_GetSellPrice(s32 item);
void UiText_DrawQuantity(s32 value, s32 slot);
void Func_0810857c(s32 message);
s32 Func_08108630(s32 value);
void Audio_PlayCue(s32 cue);
void Inventory_DiscardFar(s32 unitId, s32 slot);
void Unit_RefreshPsynergyFar(s32 unitId);
void UnitCalcFar(s32 unitId);
void Party_AdjustCoinsFar(s32 amount);
void Func_08109188(void);
void Shop_DrawItemCount(s32 window, s32 unitId);

void Shop_SellItem(s32 unitId, s32 slot, s32 count) {
  struct ShopRuntime *shop = Ram_HeapSlots->menuRuntime;
  struct BattleUnit *unit = UnitGet(unitId);
  u32 raw = unit->inventory[slot];
  s32 itemId;
  struct ItemDefinition *item;
  u8 rare;
  s32 all;
  s32 total;
  s32 message;
  s32 i;

  asm volatile("");
  itemId = 0x1ff;
  itemId &= raw;
  item = Item_Get(itemId);
  rare = item->flags & 4;
  all = 0;
  if (count == -1) {
    all = 1;
    count = 1;
  }
  total = Shop_GetSellPrice(unit->inventory[slot]) * count;
  if (total == 0) {
    UiText_DrawQuantity(itemId, 2);
    Func_0810857c((s32)Msg04701);
    return;
  }
  if ((unit->inventory[slot] & 0x200) && (item->flags & 2)) {
    UiText_DrawQuantity(itemId, 2);
    Func_0810857c((s32)Msg04700);
    return;
  }
  if (all)
    message = (s32)Msg04707;
  else if (unit->inventory[slot] & 0x400)
    message = (s32)Msg04706;
  else if (count > 1)
    message = (s32)Msg04705;
  else if (rare)
    message = (s32)Msg04704;
  else
    message = (s32)Msg04703;
  UiText_DrawQuantity(itemId, 2);
  UiText_DrawQuantity(total, 5);
  Func_0810857c(message);
  if (Func_08108630(0) != 0) {
    if (rare || all)
      message = (s32)Msg04711;
    else
      message = (s32)Msg04709;
    Func_0810857c(message);
    return;
  }
  Audio_PlayCue(102);
  for (i = 0; i < count; i++)
    Inventory_DiscardFar(unitId, slot);
  Unit_RefreshPsynergyFar(unitId);
  UnitCalcFar(unitId);
  Party_AdjustCoinsFar(total);
  Func_08109188();
  Shop_DrawItemCount(shop->itemWindow, unitId);
  if (rare || all)
    message = (s32)Msg04710;
  else
    message = (s32)Msg04708;
  Func_0810857c(message);
}
