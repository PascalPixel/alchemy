#include "battle_system.h"
#include "battle_unit.h"
#include "item.h"
#include "ram_buffer.h"

// The shop work block, as this function reads it.
struct ShopRuntime {
  u8 unk000[0x24];
  s32 itemWindow;
};

extern u8 MsgCannotSellEquipped[];
extern u8 MsgWeDoNotBuy[];
extern u8 MsgSellOffer[];
extern u8 MsgRareSellOffer[];
extern u8 MsgSellTotal[];
extern u8 MsgDamagedSellOffer[];
extern u8 MsgOldItemSellOffer[];
extern u8 MsgDeal[];
extern u8 MsgChangedMind[];
extern u8 MsgSold[];
extern u8 MsgKeepIt[];

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

void Func_0810a108(s32 unitIdArg, s32 slot, s32 countArg) {
  struct ShopRuntime *shop;
  s32 unitId;
  s32 count;

  shop = Ram_HeapSlots->menuRuntime;
  /* FAKEMATCH(temporary): unitId and count copied from the arguments after
   * the block, so that their moves to r8 and r7 fall among the address's; as
   * the arguments used directly, 4 of 167 instructions differ in tla-en. */
  unitId = unitIdArg;
  /* FAKEMATCH(dead): the dead store keeps GCSE from propagating the copy back
   * to the argument, which then moves to r8 before the block; as the copy
   * alone, 2 of 167 instructions differ in tla-en. */
  unitIdArg = 0;
  count = countArg;
  // The inner block gives its locals their pseudos after the item id's
  // offset, which puts the stack slots in the native's order.
  {
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
      Func_0810857c((s32)MsgWeDoNotBuy);
      return;
    }
    if ((unit->inventory[slot] & 0x200) && (item->flags & 2)) {
      MessageArgument_PushFar(itemId, 2);
      Func_0810857c((s32)MsgCannotSellEquipped);
      return;
    }
    if (all)
      message = (s32)MsgOldItemSellOffer;
    else if (unit->inventory[slot] & 0x400)
      message = (s32)MsgDamagedSellOffer;
    else if (count > 1)
      message = (s32)MsgSellTotal;
    else if (rare)
      message = (s32)MsgRareSellOffer;
    else
      message = (s32)MsgSellOffer;
    MessageArgument_PushFar(itemId, 2);
    MessageArgument_PushFar(total, 5);
    Func_0810857c(message);
    if (Func_08108630(0) != 0) {
      if (rare || all)
        message = (s32)MsgKeepIt;
      else
        message = (s32)MsgChangedMind;
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
      message = (s32)MsgSold;
    else
      message = (s32)MsgDeal;
    Func_0810857c(message);
  }
}
