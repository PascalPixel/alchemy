// Measured 2026-10-26 under the Makefile's options (-mthumb-slow-loads), differ
// 8 of 170 in tla-en. The native spills its arguments to the stack before it
// builds the heap slots' address; this builds the address first (row 8). An
// empty asm after `equipped = Inventory_FindEquippedFar(unitId, item->type);`
// measured 6. No single empty asm or one-pass block reaches 0.
// Near miss, differ 8 of 170 (2026-10-19; was 10):
// the shop repairs a unit's damaged item at slot
// (refusing other kinds, undamaged items, cursed equipped items and a price
// over the party's coins), then offers to equip it. ☀️'s Shop_RepairItem in
// games/tbs/src/shop/shop.c, rewritten with ⚓️'s calls (Unit_Get,
// Shop_GetItemPrice, UiText_DrawQuantity, the shop message and choice routines
// Func_0810857c and Func_08108630, Inventory_RepairFar, Func_08109188 for the
// coins) and numbered messages, their ☀️ names beside them. Instruction order
// only, the same pattern as Shop_SellItem (0810a108): the work pointer's
// address build against the frame setup, the slot offset's copy and the 0x1ff
// mask's first move, and one register copy against the equipped slot's spill.
// buy.c's barrier before the 0x1ff mask now orders the mask (10 to 8); a
// barrier ahead of the work's load puts its build after sub sp as native but
// swaps r5 and r6 and two spill slots (differ 20).
#include "party_state.h"
#include "battle_system.h"
#include "battle_unit.h"
#include "item.h"
#include "ram_buffer.h"

// The shop work block, as this function reads it.
struct ShopRuntime {
  u8 unk000[0x24];
  s32 itemWindow;
};

extern u8 Msg04715[]; // MsgCannotRepairKind
extern u8 Msg04716[]; // MsgNotDamaged
extern u8 Msg04717[]; // MsgCursedWontComeOff
extern u8 Msg04718[]; // MsgNotEnoughMoney
extern u8 Msg04719[]; // MsgShopRepairConfirm, then its three replies

struct BattleUnit *Unit_Get(s32 unitId);
s32 Inventory_FindEquippedFar(s32 unitId, u8 kind);
u32 Shop_GetItemPrice(s32 item);
void UiText_DrawQuantity(s32 value, s32 slot);
void Func_0810857c(s32 message);
s32 Func_08108630(s32 value);
void UiWork_FinalizePendingCoreFar(void);
void WaitFrames(s32 frames);
void Audio_PlayCue(s32 cue);
void Inventory_RepairFar(s32 unitId, s32 slot);
void Party_AdjustCoinsFar(s32 amount);
void Func_08109188(void);
void Shop_DrawItemCount(s32 window, s32 unitId);
s32 Shop_ConfirmEquip(s32 unitId, s32 slot);
void Shop_SellOld(s32 unitId, s32 slot);

void Func_0810a2d8(s32 unitId, s32 slot) {
  struct ShopRuntime *shop = Ram_HeapSlots->menuRuntime;
  struct BattleUnit *unit = Unit_Get(unitId);
  u32 raw = unit->inventory[slot];
  s32 itemId;
  struct ItemDefinition *item;
  s32 equipped;
  u32 price;
  s32 message;
  u8 kind;
  u32 saved;

  asm volatile("");
  itemId = 0x1ff;
  itemId &= raw;
  item = Item_Get(itemId);
  equipped = Inventory_FindEquippedFar(unitId, item->type);
  price = Shop_GetItemPrice(unit->inventory[slot]);
  kind = item->useType;
  if (kind != 2) {
    UiText_DrawQuantity(itemId, 2);
    Func_0810857c((s32)Msg04715);
    return;
  }
  if (!(unit->inventory[slot] & 0x400)) {
    UiText_DrawQuantity(itemId, 2);
    Func_0810857c((s32)Msg04716);
    return;
  }
  if ((unit->inventory[slot] & 0x200) && (kind & item->flags)) {
    UiText_DrawQuantity(itemId, 2);
    Func_0810857c((s32)Msg04717);
    return;
  }
  if (price > gPartyState.coins) {
    Func_0810857c((s32)Msg04718);
    return;
  }
  UiText_DrawQuantity(itemId, 2);
  UiText_DrawQuantity(price, 5);
  message = (s32)Msg04719;
  Func_0810857c(message);
  if (Func_08108630(0) != 0) {
    Func_0810857c(message + 1);
    return;
  }
  saved = unit->inventory[slot];
  unit->inventory[slot] = 0;
  Shop_DrawItemCount(shop->itemWindow, unitId);
  UiText_DrawQuantity(itemId, 2);
  Func_0810857c(message + 2);
  UiWork_FinalizePendingCoreFar();
  WaitFrames(10);
  Audio_PlayCue(100);
  WaitFrames(110);
  Audio_PlayCue(100);
  WaitFrames(110);
  Audio_PlayCue(100);
  WaitFrames(110);
  Audio_PlayCue(112);
  WaitFrames(20);
  unit->inventory[slot] = saved;
  Inventory_RepairFar(unitId, slot);
  Party_AdjustCoinsFar(-price);
  Func_08109188();
  Shop_DrawItemCount(shop->itemWindow, unitId);
  UiText_DrawQuantity(itemId, 2);
  Func_0810857c(message + 3);
  if (Shop_ConfirmEquip(unitId, slot))
    Shop_SellOld(unitId, equipped);
}
