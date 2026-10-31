// Near miss, differ 2 of 167 (tla-en, 2026-10-31): sched2 order only. The
// native spills the slot argument (str r1) before it builds the heap slots'
// address; this builds the address first. In sched2 both are ready in the
// second cycle, and the address chain (movs, lsls, adds, then the load) has
// priority 90 against the spill's 89, so its movs goes first; the drafts
// 08109068 and 080da660 keep the same swap. No single empty asm
// or one-pass block reaches 0 (2026-10-26); a 4-minute permuter run found
// nothing better (2026-10-31).
// Sells count of a unit's item at slot (count -1 sells the whole old item),
// ☀️'s Shop_SellItem in games/tbs/src/shop/shop/sell.c, rewritten with ⚓️'s
// calls (Unit_Get, Shop_GetSellPrice, MessageArgument_PushFar, the shop
// message and choice routines Func_0810857c and Func_08108630,
// Inventory_DiscardFar, then Unit_RefreshPsynergyFar and Unit_CalculateFar,
// which ☀️ does not call) and numbered messages, their ☀️ names beside them.
// Reading the item id as ☀️ does (inventory[slot] & 0x1ff in its initializer)
// orders the slot offset's spill and the 0x1ff mask with no barrier (4 to 2;
// the earlier draft's empty asm measured 4 either way).
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

struct BattleUnit *Unit_Get(s32 unitId);
s32 Shop_GetSellPrice(s32 item);
void MessageArgument_PushFar(s32 value, s32 slot);
void Func_0810857c(s32 message);
s32 Func_08108630(s32 value);
void Sound_PlayFar(s32 cue);
void Inventory_DiscardFar(s32 unitId, s32 slot);
void Unit_RefreshPsynergyFar(s32 unitId);
void Unit_CalculateFar(s32 unitId);
void Party_AdjustCoinsFar(s32 amount);
void Func_08109188(void);
void Shop_DrawItemCount(s32 window, s32 unitId);

void Func_0810a108(s32 unitId, s32 slot, s32 count) {
  struct ShopRuntime *shop = Ram_HeapSlots->menuRuntime;
  struct BattleUnit *unit = Unit_Get(unitId);
  s32 itemId = unit->inventory[slot] & 0x1ff;
  struct ItemDefinition *item = Item_Get(itemId);
  u8 rare = item->flags & 4;
  s32 all = 0;
  s32 total;
  s32 message;
  s32 i;

  if (count == -1) {
    all = 1;
    count = 1;
  }
  total = Shop_GetSellPrice(unit->inventory[slot]) * count;
  if (total == 0) {
    MessageArgument_PushFar(itemId, 2);
    Func_0810857c((s32)Msg04701);
    return;
  }
  if ((unit->inventory[slot] & 0x200) && (item->flags & 2)) {
    MessageArgument_PushFar(itemId, 2);
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
  MessageArgument_PushFar(itemId, 2);
  MessageArgument_PushFar(total, 5);
  Func_0810857c(message);
  if (Func_08108630(0) != 0) {
    if (rare || all)
      message = (s32)Msg04711;
    else
      message = (s32)Msg04709;
    Func_0810857c(message);
    return;
  }
  Sound_PlayFar(102);
  for (i = 0; i < count; i++)
    Inventory_DiscardFar(unitId, slot);
  Unit_RefreshPsynergyFar(unitId);
  Unit_CalculateFar(unitId);
  Party_AdjustCoinsFar(total);
  Func_08109188();
  Shop_DrawItemCount(shop->itemWindow, unitId);
  if (rare || all)
    message = (s32)Msg04710;
  else
    message = (s32)Msg04708;
  Func_0810857c(message);
}
