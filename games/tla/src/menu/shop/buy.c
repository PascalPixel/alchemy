#include "types.h"
#include "item.h"
#include "ram_buffer.h"

s32 Inventory_AddItemFar(s32 unitId, s32 item_id);
s32 Inventory_FindEquippedFar(s32 unitId, s32 type);
s32 Party_AdjustCoinsFar(s32 amount);
s32 Party_AdjustShopCreditFar(s32 amount);
extern u8 MsgHereYouGo[];
void Audio_PlayCue(s32 cue);
void Func_0810857c(s32 messageId);
void Func_08109188(void);
s32 Shop_ConfirmEquip(s32 unitId, s32 slot);
s32 Shop_SellOld(s32 unitId, s32 slot);

void Shop_BuyDone(s32 unitId, s32 itemId, s32 quantity) {
  s32 replacedSlot;
  s32 remaining;
  s32 addedSlot;
  struct ItemDefinition *item;

  remaining = quantity;
  item = Item_Get(itemId);
  addedSlot = 0;
  replacedSlot = Inventory_FindEquippedFar(unitId, item->type);
  Audio_PlayCue(101);
  if (addedSlot < remaining) {
    do {
      addedSlot = Inventory_AddItemFar(unitId, itemId);
      Party_AdjustCoinsFar(0 - (u16)item->price);
      remaining -= 1;
      Party_AdjustShopCreditFar((u16)item->price);
      Func_08109188();
    } while (remaining != 0);
  }
  Func_0810857c((s32)MsgHereYouGo);
  if (Shop_ConfirmEquip(unitId, addedSlot) != 0)
    Shop_SellOld(unitId, replacedSlot);
}

struct ShopUnit {
  u8 unk00[216];
  u16 inventory[15];
};

struct ShopWork {
  u8 unk00[0x24];
  void *itemWindow;
};

void *UnitGet(s32 unitId);
void Inventory_EquipFar(s32 unitId, s32 slot);
void UiText_DrawQuantity(s32 value, s32 slot);
void Func_081084f4(s32 messageId);
s32 Func_08108630(s32 choice);
void Shop_DrawItemCount(void *window, s32 item);
void UiWork_FinalizePendingCoreFar(void);
void UiText_OpenMessageWindowFar(s32 messageId, s32 x, s32 y, s32 flags);
s32 UiWork_IsCompleteFar(void);
void WaitFrames(s32 frames);
extern u8 MsgEquipNowPrompt[];
extern u8 MsgBecameCursed[];
extern u8 MsgLookBolder[];

s32 Shop_ConfirmEquip(s32 unitId, s32 slot) {
  struct ShopWork *shop = Ram_HeapSlots->menuRuntime;
  struct ShopUnit *unit = UnitGet(unitId);
  u32 raw = unit->inventory[slot];
  s32 itemId;
  struct ItemDefinition *info;
  s32 replaced;
  void *window;

  /* FAKEMATCH: the barrier keeps the slot halfword load ahead of the 0x1ff
   * mask; without it the mask is started first (one reordered pair, differ 2).
   */
  asm volatile("");
  itemId = 0x1ff;
  itemId &= raw;
  info = Item_Get(itemId);

  if (unit->inventory[slot] & 0x200)
    return 0;

  if (Item_CanOwnerEquip(unitId, itemId) == 0)
    return 0;

  replaced = Inventory_FindEquippedFar(unitId, info->type);
  if (replaced != -1) {
    struct ItemDefinition *oldInfo = Item_Get(unit->inventory[replaced]);

    if (oldInfo->flags & 2)
      return 0;
  }

  UiText_DrawQuantity(unitId, 1);
  Func_081084f4((s32)MsgEquipNowPrompt);
  if (Func_08108630(0) != 0)
    return 0;

  Inventory_EquipFar(unitId, slot);
  window = shop->itemWindow;
  if (window != 0)
    Shop_DrawItemCount(window, unitId);

  if (info->flags & 1) {
    Audio_PlayCue(103);
    UiWork_FinalizePendingCoreFar();
    {
      s32 x = 8;
      s32 y = 4;
      s32 flags = 2;

      /* FAKEMATCH: the native loads the three small arguments before the
       * message; the plain call loads the message first (one instruction moved,
       * differ 2). */
      asm volatile("" : "+l"(x), "+l"(y), "+l"(flags));
      UiText_OpenMessageWindowFar((s32)MsgBecameCursed, x, y, flags);
    }
    while (UiWork_IsCompleteFar() == 0)
      WaitFrames(1);
  }

  Func_0810857c((s32)MsgLookBolder);
  return 1;
}

void Func_0810a108(s32 unitId, s32 slot, s32 count);

s32 Shop_SellOld(s32 unitId, s32 slot) {
  u8 *unit;
  s32 itemOffset;
  s32 itemId;
  u32 raw;

  unit = UnitGet(unitId);
  if (slot == -1)
    return 0;
  itemOffset = slot * 2 + 216;
  raw = *(u16 *)(unit + itemOffset);
  /* FAKEMATCH: the barrier keeps the slot halfword load ahead of the 0x1ff
   * mask; without it the mask is started first (one reordered pair, differ 2).
   */
  asm volatile("");
  itemId = 0x1ff;
  itemId &= raw;
  if (Item_Get(itemId)->type == 6)
    return 0;
  if (Item_Get(itemId)->flags & 8)
    return 0;
  Func_0810a108(unitId, slot, -1);
  return 1;
}
